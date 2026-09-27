/*
 * script_bindings — Core plumbing
 * Decompiled with Ghidra 11.3.2 (arm64 pseudocode) from: libNexusScriptRuntime69252.so
 * Related menu entries (from embedded nexus-overlay-wire/v1):
 *   - menu.killaura "Kill aura" [free]
 *   - menu.autododge "Auto dodge" [free]
 *   - menu.follow "Follow" [Nexus+ PAID]
 *   - menu.aim "Smart aim" [free]
 *   - menu.xray "X-Ray" [Nexus+ PAID]
 *   - menu.hold "Hold fire" [free]
 *   - menu.spin "Spin" [Nexus+ PAID]
 *   - killauraEnabled "Kill aura" [free]
 *   - aopPredictEnabled "Prediction" [free]
 *   - killauraMainAttack "Main attack" [free]
 *   - killauraNoWall "Wall check" [free]
 *   - killauraNoBall "Ignore ball" [free]
 *   - autododgeEnabled "Auto dodge" [free]
 *   - aopAimEnabled "Smart aim" [free]
 *   - isSpinEnabled "Spin" [Nexus+ PAID]
 *   - followEnabled "Follow" [Nexus+ PAID]
 *   - followClosestAllyEnabled "Closest ally" [Nexus+ PAID]
 *   - ballAssistEnabled "Ball assist" [Nexus+ PAID]
 *   - holdToShootEnabled "Hold fire" [free]
 *   - isXrayEnabled "X-Ray" [Nexus+ PAID]
 *   - espEnabled "ESP" [Nexus+ PAID]
 *   - characterOutlineEnabled "Character outline" [free]
 *   - attackRangeIndicator "Attack range" [Nexus+ PAID]
 *   - hitboxRenderer "Hitboxes" [Nexus+ PAID]
 *   - enemyTracer "Enemy tracer" [Nexus+ PAID]
 *   - trophiesAboveHead "Trophies" [Nexus+ PAID]
 *   - pinEnabled "Auto pin" [Nexus+ PAID]
 *   - sprayEnabled "Auto spray" [Nexus+ PAID]
 *   - ... +205 more (see docs/feature_list.json)
 * Notes: QuickJS host layer: JNI natives, bridge bootstrap evaluation, memfd script loading. (Engine itself is stock QuickJS 2024-01-13 frida build - public source, not included.)
 */

/* ===== FUN_00135624 @ 00135624 [libNexusScriptRuntime69252.so] ===== */

long * FUN_00135624(long param_1,long param_2)

{
  int iVar1;
  int iVar2;
  long *__ptr;
  long lVar3;
  int *piVar4;
  undefined8 uVar5;
  undefined1 auVar6 [16];
  undefined1 auVar7 [16];
  
  __ptr = (long *)calloc(1,0x6ce8);
  if (__ptr != (long *)0x0) {
    __ptr[2] = param_1;
    __ptr[3] = param_2;
    lVar3 = FUN_00139fd8();
    *__ptr = lVar3;
    if (lVar3 == 0) {
      free(__ptr);
    }
    else {
      FUN_0013a0a4(lVar3,0x2000000);
      FUN_0013f2a4(*__ptr,0x80000);
      FUN_0013a0bc(*__ptr,0);
      FUN_0013a0b4(*__ptr,FUN_00135824,__ptr);
      FUN_00160768(*__ptr,FUN_001358cc,__ptr);
      lVar3 = FUN_0013cd1c(*__ptr);
      __ptr[1] = lVar3;
      if (lVar3 != 0) {
        FUN_0013eca8(lVar3,__ptr);
        auVar6 = FUN_0014339c(__ptr[1]);
        uVar5 = auVar6._8_8_;
        piVar4 = auVar6._0_8_;
        lVar3 = __ptr[1];
        auVar7 = FUN_001412b0(lVar3,FUN_00135ac0,"__nexusEmit",2,0,0);
        FUN_00149cb0(lVar3,piVar4,uVar5,"__nexusEmit",auVar7._0_8_,auVar7._8_8_);
        lVar3 = __ptr[1];
        auVar7 = FUN_001412b0(lVar3,FUN_00135d88,"__nexusCall",1,0,0);
        FUN_00149cb0(lVar3,piVar4,uVar5,"__nexusCall",auVar7._0_8_,auVar7._8_8_);
        lVar3 = __ptr[1];
        if ((0xfffffff4 < auVar6._8_4_) &&
           (iVar1 = *piVar4, iVar2 = iVar1 + -1, *piVar4 = iVar2, iVar2 == 0 || iVar1 < 1)) {
          FUN_00141a4c(lVar3,piVar4,uVar5);
        }
        auVar6 = FUN_0015a3dc(__ptr[1],
                              "/* Own module layer. No legacy bootstrap, raw pointers or old game offsets. */\n(() => {\n    \'use strict\';\n    const nativeCall = __nexusCall, nativeEmit = __nexusEmit;\n    delete globalThis.__nexusCall; delete globalThis.__nexusEmit;\n    const definitions = new Map(), cache = new Map();\n    const valid = id => typeof id === \'string\' && /^[A-Za-z0-9_./-]{1,160}$/.test(id)\n        && !id.split(\'/\').includes(\'..\');\n    const define = (id, dependencies, factory) => {\n        if (!valid(id) || definitions.has(id) || !Array.isArray(dependencies)\n            || dependencies.some(x => !valid(x)) || new Set(dependencies).size !== dependencies.length\n            || typeof factory !== \'function\' || definitions.size >= 512)\n            throw new TypeError(\'module_definition_refused\');\n        definitions.set(id, { dependencies: new Set(dependencies), factory });\n    };\n    const require = id => {\n        if (!valid(id)) throw new TypeError(\'module_id_refused\');\n        if (cache.has(id)) return cache.get(id).exports;\n        const definition = definitions.get(id);\n        if (!definition) throw new Error(\'module_unavailable:\' + id);\n        const module = { exports: {} };\n        cache.set(id, module); // CommonJS cycles see the partial exports object.\n        try {\n            const ownRequire = dependency => {\n                if (!definition.dependencies.has(dependency)) throw new Error(\'undeclared_dependency:\' + dependency);\n                return require(dependency);\n            };\n            const result = definition.factory.call(module.exports, module, module.exports, ownRequire);\n            if (result && typeof result.then === \'function\') throw new Error(\'async_module_factory_unsupported\');\n            return module.exports;\n        } catch (error) { cache.delete(id); throw error; }\n    };\n    const binding = name => {\n        if (typeof name !== \'string\' || !/^[A-Za-z][A-Za-z0-9_.-]{0,95}$/.test(name))\n            throw new TypeError(\'binding_name_refused\');\n        return (...args) => nativeCall(name, ...args);\n  ..." /* TRUNCATED STRING LITERAL */
                              ,0x95a,"nexus-own-bridge.js",0);
        piVar4 = auVar6._0_8_;
        lVar3 = __ptr[1];
        if ((0xfffffff4 < auVar6._8_4_) &&
           (iVar1 = *piVar4, iVar2 = iVar1 + -1, *piVar4 = iVar2, iVar2 == 0 || iVar1 < 1)) {
          FUN_00141a4c(lVar3,piVar4);
        }
        if ((auVar6._8_8_ & 0xffffffff) != 6) {
          return __ptr;
        }
      }
      FUN_00135a10(__ptr);
    }
    __ptr = (long *)0x0;
  }
  return __ptr;
}

/* ===== FUN_00135824 @ 00135824 [libNexusScriptRuntime69252.so] ===== */

undefined4 FUN_00135824(ulong param_1,long param_2)

{
  long lVar1;
  uint uVar2;
  undefined4 uVar3;
  timespec local_48;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  if (*(long *)(param_2 + 0x6cd0) != 0) {
    uVar3 = 1;
    uVar2 = clock_gettime(1,&local_48);
    param_1 = (ulong)uVar2;
    if (*(ulong *)(param_2 + 0x6cd0) <= local_48.tv_sec * 1000 + (ulong)local_48.tv_nsec / 1000000)
    {
      *(undefined4 *)(param_2 + 0x6cd8) = 1;
      goto LAB_001358a0;
    }
  }
  uVar3 = 0;
LAB_001358a0:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return uVar3;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(param_1);
}

/* ===== FUN_001358cc @ 001358cc [libNexusScriptRuntime69252.so] ===== */

void FUN_001358cc(undefined8 param_1,int *param_2,undefined8 param_3,undefined8 param_4,
                 undefined8 param_5,int param_6,long param_7)

{
  long lVar1;
  long lVar2;
  int iVar3;
  int iVar4;
  ulong uVar5;
  bool bVar6;
  ulong uVar7;
  long lVar8;
  ulong uVar9;
  undefined8 *puVar10;
  ulong uVar11;
  int *piVar12;
  undefined8 uVar13;
  
  uVar7 = *(ulong *)(param_7 + 0x6cb8);
  bVar6 = uVar7 != 0;
  if ((uVar7 == 0) || (*(int **)(param_7 + 0x68b8) == param_2)) {
    uVar9 = 0;
  }
  else {
    puVar10 = (undefined8 *)(param_7 + 0x68c8);
    uVar5 = 1;
    do {
      uVar11 = uVar5;
      uVar9 = uVar7;
      if (uVar7 == uVar11) break;
      piVar12 = (int *)*puVar10;
      uVar9 = uVar11;
      puVar10 = puVar10 + 2;
      uVar5 = uVar11 + 1;
    } while (piVar12 != param_2);
    bVar6 = uVar11 < uVar7;
  }
  if (param_6 == 0) {
    if (uVar9 == uVar7) {
      if (uVar9 == 0x40) {
        *(undefined4 *)(param_7 + 0x6ce0) = 0x10;
      }
      else {
        lVar2 = param_7 + uVar7 * 0x10;
        *(ulong *)(param_7 + 0x6cb8) = uVar7 + 1;
        if (0xfffffff4 < (uint)param_3) {
          *param_2 = *param_2 + 1;
        }
        *(int **)(lVar2 + 0x68b8) = param_2;
        *(undefined8 *)(lVar2 + 0x68c0) = param_3;
      }
    }
  }
  else if (bVar6) {
    lVar2 = param_7 + uVar9 * 0x10;
    piVar12 = *(int **)(lVar2 + 0x68b8);
    if ((0xfffffff4 < (uint)*(undefined8 *)(lVar2 + 0x68c0)) &&
       (iVar3 = *piVar12, iVar4 = iVar3 + -1, *piVar12 = iVar4, iVar4 == 0 || iVar3 < 1)) {
      FUN_00141a4c(param_1,piVar12);
    }
    lVar8 = *(long *)(param_7 + 0x6cb8) + -1;
    lVar1 = param_7 + lVar8 * 0x10;
    *(long *)(param_7 + 0x6cb8) = lVar8;
    uVar13 = *(undefined8 *)(lVar1 + 0x68b8);
    *(undefined8 *)(lVar2 + 0x68c0) = *(undefined8 *)(lVar1 + 0x68c0);
    *(undefined8 *)(lVar2 + 0x68b8) = uVar13;
  }
  return;
}

/* ===== FUN_00135ac0 @ 00135ac0 [libNexusScriptRuntime69252.so] ===== */

undefined1  [16]
FUN_00135ac0(undefined8 param_1,undefined8 param_2,undefined8 param_3,int param_4,
            undefined8 *param_5)

{
  undefined *puVar1;
  byte bVar2;
  long lVar3;
  byte *pbVar4;
  long lVar5;
  byte *__s;
  size_t sVar6;
  char *pcVar7;
  long lVar8;
  ulong uVar9;
  undefined1 auVar10 [16];
  undefined1 auVar11 [16];
  ulong local_240;
  char acStack_238 [480];
  long local_58;
  
  lVar3 = tpidr_el0;
  local_58 = *(long *)(lVar3 + 0x28);
  lVar5 = FUN_0013eca0();
  if ((param_4 == 2) && ((int)param_5[1] == -7)) {
    __s = (byte *)FUN_00140aec(param_1,0,*param_5,param_5[1],0);
    if (__s == (byte *)0x0) {
LAB_00135cf0:
      uVar9 = 0;
      auVar11 = ZEXT816(6) << 0x40;
      goto LAB_00135bc0;
    }
    sVar6 = strlen((char *)__s);
    pbVar4 = __s;
    if (sVar6 - 1 < 0x40) {
      for (; sVar6 != 0; sVar6 = sVar6 - 1) {
        bVar2 = *pbVar4;
        if ((9 < (byte)(bVar2 - 0x30) && 0x19 < (byte)((bVar2 & 0xdf) + 0xbf)) &&
           ((0x32 < bVar2 - 0x2d || ((1L << ((ulong)(bVar2 - 0x2d) & 0x3f) & 0x4000000000003U) == 0)
            ))) goto LAB_00135c0c;
        pbVar4 = pbVar4 + 1;
      }
      pcVar7 = strstr((char *)__s,"..");
      if (((pcVar7 != (char *)0x0) || (*(long *)(lVar5 + 0x6cc8) == 0)) ||
         (0x3f < *(int *)(*(long *)(lVar5 + 0x6cc8) + 0x14))) goto LAB_00135c0c;
      auVar11 = FUN_0015d7fc(param_1,param_5[2],param_5[3],0,3,0,3);
      uVar9 = auVar11._0_8_;
      if (auVar11._8_4_ == 6) {
        FUN_00140de8(param_1,__s);
        uVar9 = uVar9 & 0xffffffff00000000;
        goto LAB_00135bc0;
      }
      local_240 = 0;
      lVar8 = FUN_00140aec(param_1,&local_240,uVar9,auVar11._8_8_,0);
      FUN_0013604c(param_1,uVar9,auVar11._8_8_);
      if (lVar8 == 0) {
        FUN_00140de8(param_1,__s);
        goto LAB_00135cf0;
      }
      if (local_240 < 0x181) {
        snprintf(acStack_238,0x1e0,"%s:%s",__s,lVar8);
        *(int *)(*(long *)(lVar5 + 0x6cc8) + 0x14) = *(int *)(*(long *)(lVar5 + 0x6cc8) + 0x14) + 1;
        if (*(code **)(lVar5 + 0x10) != (code *)0x0) {
          puVar1 = &DAT_0010cd92;
          if (*(undefined **)(lVar5 + 0x6cc0) != (undefined *)0x0) {
            puVar1 = *(undefined **)(lVar5 + 0x6cc0);
          }
          (**(code **)(lVar5 + 0x10))
                    (*(undefined8 *)(lVar5 + 0x18),puVar1,"fixture_event",acStack_238);
        }
        FUN_00140de8(param_1,lVar8);
        FUN_00140de8(param_1,__s);
        uVar9 = 0;
        auVar11 = ZEXT816(3) << 0x40;
        goto LAB_00135bc0;
      }
      FUN_00140de8(param_1,lVar8);
      FUN_00140de8(param_1,__s);
      pcVar7 = "emit_size";
    }
    else {
LAB_00135c0c:
      FUN_00140de8(param_1,__s);
      pcVar7 = "emit_limit";
    }
    auVar11 = FUN_001437a8(param_1,pcVar7);
  }
  else {
    auVar11 = FUN_00143570(param_1,"emit_arguments");
  }
  uVar9 = auVar11._0_8_ & 0xffffffff00000000;
LAB_00135bc0:
  if (*(long *)(lVar3 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  auVar10._0_8_ = uVar9 | auVar11._0_8_ & 0xffffffff;
  auVar10._8_8_ = auVar11._8_8_;
  return auVar10;
}

/* ===== FUN_00135d88 @ 00135d88 [libNexusScriptRuntime69252.so] ===== */

undefined1  [16]
FUN_00135d88(undefined8 param_1,undefined8 param_2,undefined8 param_3,int param_4,
            undefined8 *param_5)

{
  uint uVar1;
  long lVar2;
  bool bVar3;
  int iVar4;
  long lVar5;
  char *__s1;
  ulong extraout_x8;
  ulong uVar6;
  ulong uVar7;
  undefined8 *puVar8;
  long lVar9;
  undefined8 *puVar10;
  ulong unaff_x28;
  undefined1 auVar11 [16];
  undefined1 auVar12 [16];
  undefined1 auVar13 [16];
  double local_98;
  uint local_8c;
  int local_88 [8];
  long local_68;
  
  lVar2 = tpidr_el0;
  local_68 = *(long *)(lVar2 + 0x28);
  lVar5 = FUN_0013eca0();
  if ((param_4 < 1) || ((int)param_5[1] != -7)) {
    auVar13 = FUN_00143570(param_1,"binding_arguments");
    uVar7 = auVar13._0_8_ >> 0x20;
  }
  else {
    __s1 = (char *)FUN_00140aec(param_1,0,*param_5,param_5[1],0);
    if (__s1 == (char *)0x0) {
      auVar13 = ZEXT816(6) << 0x40;
      uVar7 = extraout_x8;
    }
    else {
      lVar9 = *(long *)(lVar5 + 0x3820);
      if (lVar9 != 0) {
        puVar8 = (undefined8 *)(lVar5 + 0x20);
        do {
          iVar4 = strcmp(__s1,(char *)*puVar8);
          if (iVar4 == 0) goto LAB_00135e24;
          lVar9 = lVar9 + -1;
          puVar8 = puVar8 + 0xe;
        } while (lVar9 != 0);
      }
      puVar8 = (undefined8 *)0x0;
LAB_00135e24:
      FUN_00140de8(param_1,__s1);
      if (((puVar8 == (undefined8 *)0x0) || (param_4 + -1 != *(int *)(puVar8 + 1))) ||
         ((*(int *)((long)puVar8 + 0xc) != 0 &&
          ((*(code **)(lVar5 + 0x68a8) == (code *)0x0 ||
           (iVar4 = (**(code **)(lVar5 + 0x68a8))(*(undefined8 *)(lVar5 + 0x68b0),puVar8),
           iVar4 == 0)))))) {
        *(undefined4 *)(lVar5 + 0x6ce0) = 0x11;
        auVar13 = FUN_0014368c(param_1,"binding_unported_or_identity_unverified");
        uVar7 = auVar13._0_8_ >> 0x20;
      }
      else {
        puVar10 = param_5 + 3;
        local_8c = 0;
        uVar7 = 0;
        do {
          uVar6 = uVar7;
          auVar11._8_8_ = param_5;
          auVar11._0_8_ = __s1;
          uVar1 = *(uint *)(puVar8 + 1);
          if (uVar1 <= uVar6) break;
          if ((((((int)*puVar10 != 7) && ((int)*puVar10 != 0)) ||
               (iVar4 = FUN_0014bb08(param_1,&local_98,puVar10[-1]), iVar4 != 0)) ||
              ((ABS(local_98) == INFINITY || (NAN(ABS(local_98)))))) ||
             ((local_98 < -2147483648.0 ||
              ((local_98 != 2147483647.0 && local_98 < 2147483647.0 == NAN(local_98) ||
               ((double)(long)local_98 != local_98)))))) {
            *(undefined4 *)(lVar5 + 0x6ce0) = 0x11;
            auVar11 = FUN_00143570(param_1,"binding_i32_required");
            bVar3 = false;
            unaff_x28 = auVar11._0_8_ >> 0x20;
          }
          else {
            bVar3 = true;
            local_88[uVar6] = (int)local_98;
          }
          param_5 = auVar11._8_8_;
          __s1 = auVar11._0_8_;
          puVar10 = puVar10 + 2;
          uVar7 = uVar6 + 1;
        } while (bVar3);
        if (uVar1 <= uVar6) {
          iVar4 = (*(code *)puVar8[2])(puVar8[3],local_88,*(undefined4 *)(puVar8 + 1),&local_8c);
          if (iVar4 == 0) {
            lVar5 = *(long *)(lVar5 + 0x6cc8);
            if (lVar5 != 0) {
              *(int *)(lVar5 + 0x18) = *(int *)(lVar5 + 0x18) + 1;
            }
            unaff_x28 = 0;
            auVar11 = ZEXT416(local_8c);
          }
          else {
            *(undefined4 *)(lVar5 + 0x6ce0) = 0x11;
            auVar13 = FUN_001405ec(param_1,"binding_failed");
            unaff_x28 = auVar13._0_8_ >> 0x20;
            auVar11._8_8_ = auVar13._8_8_;
            auVar11._0_8_ = auVar13._0_8_ & 0xffffffff;
          }
        }
        auVar13._8_8_ = auVar11._8_8_;
        auVar13._0_8_ = auVar11._0_8_ & 0xffffffff;
        uVar7 = unaff_x28 & 0xffffffff;
      }
    }
  }
  if (*(long *)(lVar2 + 0x28) == local_68) {
    auVar12._0_8_ = auVar13._0_8_ & 0xffffffff | uVar7 << 0x20;
    auVar12._8_8_ = auVar13._8_8_;
    return auVar12;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00136350 @ 00136350 [libNexusScriptRuntime69252.so] ===== */

/* WARNING: Removing unreachable block (ram,0x0013687c) */

void FUN_00136350(undefined8 *param_1,byte *param_2,char *param_3,void *param_4,size_t param_5,
                 int *param_6)

{
  long lVar1;
  undefined *puVar2;
  byte bVar3;
  int iVar4;
  long lVar5;
  byte *pbVar6;
  undefined8 uVar7;
  __time_t _Var8;
  bool bVar9;
  int iVar10;
  size_t sVar11;
  char *pcVar12;
  void *__s;
  undefined8 uVar13;
  long lVar14;
  undefined1 auVar15 [16];
  undefined1 auVar16 [16];
  timespec local_a8 [4];
  long local_68;
  
  lVar5 = tpidr_el0;
  local_68 = *(long *)(lVar5 + 0x28);
  if (param_6 == (int *)0x0) {
    iVar10 = 10;
    goto LAB_001364b8;
  }
  param_6[0x38] = 0;
  param_6[0x39] = 0;
  param_6[2] = 0;
  param_6[3] = 0;
  param_6[0] = 0;
  param_6[1] = 0;
  param_6[6] = 0;
  param_6[7] = 0;
  param_6[4] = 0;
  param_6[5] = 0;
  param_6[10] = 0;
  param_6[0xb] = 0;
  param_6[8] = 0;
  param_6[9] = 0;
  param_6[0xe] = 0;
  param_6[0xf] = 0;
  param_6[0xc] = 0;
  param_6[0xd] = 0;
  param_6[0x12] = 0;
  param_6[0x13] = 0;
  param_6[0x10] = 0;
  param_6[0x11] = 0;
  param_6[0x16] = 0;
  param_6[0x17] = 0;
  param_6[0x14] = 0;
  param_6[0x15] = 0;
  param_6[0x1a] = 0;
  param_6[0x1b] = 0;
  param_6[0x18] = 0;
  param_6[0x19] = 0;
  param_6[0x1e] = 0;
  param_6[0x1f] = 0;
  param_6[0x1c] = 0;
  param_6[0x1d] = 0;
  param_6[0x22] = 0;
  param_6[0x23] = 0;
  param_6[0x20] = 0;
  param_6[0x21] = 0;
  param_6[0x26] = 0;
  param_6[0x27] = 0;
  param_6[0x24] = 0;
  param_6[0x25] = 0;
  param_6[0x2a] = 0;
  param_6[0x2b] = 0;
  param_6[0x28] = 0;
  param_6[0x29] = 0;
  param_6[0x2e] = 0;
  param_6[0x2f] = 0;
  param_6[0x2c] = 0;
  param_6[0x2d] = 0;
  param_6[0x32] = 0;
  param_6[0x33] = 0;
  param_6[0x30] = 0;
  param_6[0x31] = 0;
  param_6[0x36] = 0;
  param_6[0x37] = 0;
  param_6[0x34] = 0;
  param_6[0x35] = 0;
  clock_gettime(1,local_a8);
  lVar1 = local_a8[0].tv_nsec;
  _Var8 = local_a8[0].tv_sec;
  if (param_1 == (undefined8 *)0x0) {
LAB_001364b0:
    iVar10 = 10;
  }
  else {
    if (param_2 == (byte *)0x0) {
      sVar11 = 0;
    }
    else {
      sVar11 = strlen((char *)param_2);
    }
    pbVar6 = param_2;
    if (sVar11 - 1 < 0x60) {
      for (; sVar11 != 0; sVar11 = sVar11 - 1) {
        bVar3 = *pbVar6;
        if ((9 < (byte)(bVar3 - 0x30) && 0x19 < (byte)((bVar3 & 0xdf) + 0xbf)) &&
           ((0x32 < bVar3 - 0x2d || ((1L << ((ulong)(bVar3 - 0x2d) & 0x3f) & 0x4000000000003U) == 0)
            ))) goto LAB_0013645c;
        pbVar6 = pbVar6 + 1;
      }
      pcVar12 = strstr((char *)param_2,"..");
      bVar9 = pcVar12 == (char *)0x0;
    }
    else {
LAB_0013645c:
      bVar9 = false;
    }
    if ((((param_5 - 1 >> 0x15 != 0) || (param_4 == (void *)0x0)) || (param_3 == (char *)0x0)) ||
       (!bVar9)) goto LAB_001364b0;
    iVar10 = strcmp(param_3,"nexus-js-source-v1");
    if (iVar10 == 0) {
      if (*(int *)((long)param_1 + 0x6cdc) == 0) {
        param_1[0xd99] = param_6;
        param_1[0xd98] = param_2;
        *(undefined4 *)(param_1 + 0xd9c) = 0;
        *(int *)(param_1 + 0xd9b) = 0;
        lVar1 = _Var8 * 1000 + (ulong)lVar1 / 1000000;
        param_1[0xd9a] = lVar1 + 500;
        FUN_00139d2c(*param_1);
        iVar10 = FUN_001354d4(param_4,param_5);
        if (iVar10 == 0) {
          iVar10 = 10;
LAB_00136634:
          auVar15 = ZEXT816(3) << 0x40;
          *param_6 = iVar10;
        }
        else {
          param_6[1] = 1;
          if ((code *)param_1[2] != (code *)0x0) {
            puVar2 = &DAT_0010cd92;
            if ((undefined *)param_1[0xd98] != (undefined *)0x0) {
              puVar2 = (undefined *)param_1[0xd98];
            }
            (*(code *)param_1[2])(param_1[3],puVar2,"decode_ack","strict_utf8_source");
          }
          sVar11 = param_5 + 1;
          __s = malloc(sVar11);
          if (__s == (void *)0x0) {
LAB_0013662c:
            iVar10 = 0xc;
            goto LAB_00136634;
          }
          iVar10 = FUN_0013692c(__s,sVar11);
          if (iVar10 == 0) {
            memset(__s,0,sVar11);
            free(__s);
            goto LAB_0013662c;
          }
          memcpy(__s,param_4,param_5);
          uVar13 = param_1[1];
          *(undefined1 *)((long)__s + param_5) = 0;
          auVar15 = FUN_0015a3dc(uVar13,__s,param_5,param_2,0x20);
          memset(__s,0,param_5);
          free(__s);
          if (auVar15._8_4_ == 6) {
            iVar10 = 0xd;
LAB_001366a0:
            *param_6 = iVar10;
          }
          else {
            param_6[2] = 1;
            if ((code *)param_1[2] != (code *)0x0) {
              puVar2 = &DAT_0010cd92;
              if ((undefined *)param_1[0xd98] != (undefined *)0x0) {
                puVar2 = (undefined *)param_1[0xd98];
              }
              (*(code *)param_1[2])(param_1[3],puVar2,"parse_ack","quickjs_compile_only");
            }
            auVar15 = FUN_00159d38(param_1[1],auVar15._0_8_,auVar15._8_8_);
            if (auVar15._8_4_ == 6) {
              iVar10 = 0xe;
              goto LAB_001366a0;
            }
            do {
              uVar13 = FUN_0013a1b8(*param_1);
              if ((int)uVar13 == 0) {
                if (((param_1[0xd97] == 0) &&
                    (iVar10 = FUN_001606b8(param_1[1],auVar15._0_8_,auVar15._8_8_), iVar10 != 0)) &&
                   (uVar13 = FUN_001606b8(param_1[1],auVar15._0_8_,auVar15._8_8_), (int)uVar13 != 2)
                   ) {
                  if (*(int *)(param_1 + 0xd9c) == 0) {
                    iVar10 = FUN_00135824(uVar13,param_1);
                    if (iVar10 == 0) {
                      param_6[3] = 1;
                      if ((code *)param_1[2] != (code *)0x0) {
                        puVar2 = &DAT_0010cd92;
                        if ((undefined *)param_1[0xd98] != (undefined *)0x0) {
                          puVar2 = (undefined *)param_1[0xd98];
                        }
                        (*(code *)param_1[2])
                                  (param_1[3],puVar2,"eval_ack",
                                   "synchronous_source_and_pending_jobs_completed");
                      }
                    }
                    else {
LAB_001368dc:
                      *param_6 = 0xf;
                    }
                  }
                  else {
                    *param_6 = *(int *)(param_1 + 0xd9c);
                  }
                }
                else {
                  *param_6 = 0x10;
                }
                goto LAB_0013672c;
              }
              iVar10 = param_6[4];
              param_6[4] = iVar10 + 1;
              if ((0x3ff < iVar10) || (iVar10 = FUN_00135824(uVar13,param_1), iVar10 != 0))
              goto LAB_001368dc;
              local_a8[0].tv_sec = 0;
              iVar10 = FUN_0013a1cc(*param_1,local_a8);
            } while (-1 < iVar10);
            *param_6 = 0x10;
          }
          auVar16 = FUN_00143434(param_1[1]);
          uVar7 = s_quickjs_exception_0010e230._8_8_;
          uVar13 = s_quickjs_exception_0010e230._0_8_;
          if (auVar16._8_4_ == -7) {
            lVar14 = FUN_00140aec(param_1[1],0,auVar16._0_8_,auVar16._8_8_,0);
            if (lVar14 != 0) {
              snprintf((char *)(param_6 + 10),0xc0,"%s",lVar14);
              FUN_00140de8(param_1[1],lVar14);
            }
          }
          else {
            *(undefined2 *)(param_6 + 0xe) = 0x6e;
            *(undefined8 *)(param_6 + 0xc) = uVar7;
            *(undefined8 *)(param_6 + 10) = uVar13;
          }
          FUN_0013604c(param_1[1],auVar16._0_8_,auVar16._8_8_);
        }
LAB_0013672c:
        local_a8[0].tv_sec = auVar15._0_8_;
        if (*(int *)(param_1 + 0xd9b) == 0) {
          iVar10 = *(int *)(param_1 + 0xd9c);
          if (iVar10 != 0) goto LAB_00136750;
        }
        else {
          iVar10 = 0xf;
LAB_00136750:
          *param_6 = iVar10;
        }
        uVar13 = param_1[1];
        if ((0xfffffff4 < auVar15._8_4_) &&
           (iVar10 = *(int *)local_a8[0].tv_sec, iVar4 = iVar10 + -1,
           *(int *)local_a8[0].tv_sec = iVar4, iVar4 == 0 || iVar10 < 1)) {
          FUN_00141a4c(uVar13,local_a8[0].tv_sec,auVar15._8_8_);
        }
        clock_gettime(1,local_a8);
        *(ulong *)(param_6 + 8) =
             (local_a8[0].tv_sec * 1000 - lVar1) + (ulong)local_a8[0].tv_nsec / 1000000;
        if (*param_6 != 0) {
          *(undefined4 *)((long)param_1 + 0x6cdc) = 1;
          snprintf((char *)local_a8,0x40,"code=%d");
          if ((code *)param_1[2] != (code *)0x0) {
            puVar2 = &DAT_0010cd92;
            if ((undefined *)param_1[0xd98] != (undefined *)0x0) {
              puVar2 = (undefined *)param_1[0xd98];
            }
            (*(code *)param_1[2])(param_1[3],puVar2,"eval_failed",local_a8);
          }
        }
        param_1[0xd99] = 0;
        param_1[0xd9a] = 0;
        param_1[0xd98] = 0;
        iVar10 = *param_6;
        goto LAB_001364b8;
      }
      iVar10 = 0x12;
    }
    else {
      iVar10 = 0xb;
    }
  }
  *param_6 = iVar10;
LAB_001364b8:
  if (*(long *)(lVar5 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar10);
  }
  return;
}

/* ===== FUN_00136c2c @ 00136c2c [libNexusScriptRuntime69252.so] ===== */

bool FUN_00136c2c(char *param_1)

{
  int iVar1;
  size_t sVar2;
  char *pcVar3;
  
  if (param_1 == (char *)0x0) {
    return false;
  }
  sVar2 = strlen(param_1);
  if (((sVar2 < 0x400) && (pcVar3 = strstr(param_1,".."), pcVar3 == (char *)0x0)) &&
     (pcVar3 = strchr(param_1,10), pcVar3 == (char *)0x0)) {
    pcVar3 = strchr(param_1,0xd);
    if (pcVar3 != (char *)0x0) {
      return false;
    }
    if (sVar2 < 8) {
      return false;
    }
    iVar1 = strcmp(param_1 + (sVar2 - 8),"/libg.so");
    if (iVar1 == 0) {
      iVar1 = strncmp(param_1,"/data/user/0/bsd.suitcase.nexusv2/files/",0x28);
      if ((iVar1 == 0) ||
         (iVar1 = strncmp(param_1,"/data/data/bsd.suitcase.nexusv2/files/",0x26), iVar1 == 0)) {
        return true;
      }
      iVar1 = strncmp(param_1,"/data/app/",10);
      if (iVar1 == 0) {
        pcVar3 = strstr(param_1,"/bsd.suitcase.nexusv2-");
        if (pcVar3 == (char *)0x0) {
          return false;
        }
        pcVar3 = strstr(param_1,"/lib/arm64/libg.so");
        return pcVar3 == param_1 + (sVar2 - 0x12);
      }
    }
  }
  return false;
}

/* ===== FUN_001376d4 @ 001376d4 [libNexusScriptRuntime69252.so] ===== */

void FUN_001376d4(long *param_1,undefined8 param_2,undefined4 *param_3)

{
  long lVar1;
  int iVar2;
  undefined4 uVar3;
  char *pcVar4;
  long lVar5;
  code *pcVar6;
  char *__s;
  undefined1 auStack_68 [8];
  long local_60;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  __s = (char *)param_1[1];
  if (__s == (char *)0x0) {
    lVar5 = 0;
  }
  else {
    pcVar4 = strrchr(__s,0x2f);
    if (pcVar4 == (char *)0x0) {
      lVar5 = 0;
    }
    else {
      iVar2 = strcmp(pcVar4 + 1,"libNexusDelivery.so");
      if (iVar2 == 0) {
        iVar2 = param_3[2];
        param_3[2] = iVar2 + 1;
        if (iVar2 != 0) {
          if (-1 < (int)param_3[1]) {
            close(param_3[1]);
          }
          lVar5 = 1;
          param_3[1] = 0xffffffff;
          goto LAB_00137768;
        }
        lVar5 = dlopen(__s,6);
        if (lVar5 == 0) goto LAB_00137768;
        pcVar6 = (code *)dlsym(lVar5,"nexus_protected_process_memory");
        if (((pcVar6 != (code *)0x0) && (iVar2 = dladdr(pcVar6,auStack_68), iVar2 != 0)) &&
           (local_60 == *param_1)) {
          uVar3 = (*pcVar6)(*param_3);
          param_3[1] = uVar3;
        }
        dlclose(lVar5);
      }
      lVar5 = 0;
    }
  }
LAB_00137768:
  if (*(long *)(lVar1 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(lVar5);
  }
  return;
}

/* ===== nexus_script_install_bindings_v1 @ 00137b4c [libNexusScriptRuntime69252.so] ===== */

/* WARNING: Removing unreachable block (ram,0x00137bdc) */

int nexus_script_install_bindings_v1
              (undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  int iVar1;
  
  pthread_mutex_lock((pthread_mutex_t *)&DAT_001f5618);
  if (DAT_001f5640 == 0) {
    iVar1 = 0x13;
  }
  else {
    iVar1 = FUN_00136084(DAT_001f5640,param_1,param_2,FUN_00137be0,0);
    if (iVar1 == 0) {
      DAT_001f5648 = param_3;
      DAT_001f5650 = param_4;
    }
  }
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_001f5618);
  return iVar1;
}

/* ===== nexus_script_verify_game_v1 @ 00137c80 [libNexusScriptRuntime69252.so] ===== */

bool nexus_script_verify_game_v1(undefined8 param_1,undefined8 param_2)

{
  bool bVar1;
  int iVar2;
  
  pthread_mutex_lock((pthread_mutex_t *)&DAT_001f5618);
  DAT_001f5658 = 0;
  if (DAT_001f5640 == 0) {
    bVar1 = false;
  }
  else {
    iVar2 = FUN_00136ed0(&DAT_001f5660,param_1,param_2);
    bVar1 = iVar2 != 0;
    if (bVar1) {
      DAT_001f5658 = DAT_001f5660;
    }
  }
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_001f5618);
  return bVar1;
}

/* ===== nexus_script_game_base_v1 @ 00137d14 [libNexusScriptRuntime69252.so] ===== */

undefined8 nexus_script_game_base_v1(void)

{
  return DAT_001f5658;
}

/* ===== JNI_OnLoad @ 00137d24 [libNexusScriptRuntime69252.so] ===== */

undefined8 JNI_OnLoad(long *param_1)

{
  long lVar1;
  char cVar2;
  char cVar3;
  int iVar4;
  FILE *__stream;
  size_t sVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  char *pcVar10;
  undefined8 uVar11;
  long *local_1b8;
  undefined *local_1b0;
  undefined *puStack_1a8;
  undefined *puStack_1a0;
  undefined *puStack_198;
  undefined *local_190;
  undefined *puStack_188;
  undefined *puStack_180;
  undefined *puStack_178;
  undefined *local_170;
  undefined1 auStack_168 [256];
  long local_68;
  
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  local_1b8 = (long *)0x0;
  __stream = fopen("/proc/self/cmdline","rb");
  if (__stream == (FILE *)0x0) {
code_r0x00137dac:
    pcVar10 = "jni_environment_or_process";
LAB_00137db0:
    FUN_0013816c(param_1,pcVar10);
  }
  else {
    sVar5 = fread(auStack_168,1,0x100,__stream);
    iVar4 = fgetc(__stream);
    fclose(__stream);
    if ((iVar4 != -1) || (iVar4 = FUN_00135428(auStack_168,sVar5), iVar4 == 0))
    goto code_r0x00137dac;
    uVar11 = 0x10006;
    iVar4 = (**(code **)(*param_1 + 0x30))(param_1,&local_1b8,0x10006);
    if ((iVar4 != 0) || (local_1b8 == (long *)0x0)) goto code_r0x00137dac;
    cVar2 = (**(code **)(*local_1b8 + 0x720))();
    if (cVar2 != '\0') {
      pcVar10 = "pending_jni_exception";
      goto LAB_00137db0;
    }
    lVar6 = (**(code **)(*local_1b8 + 0x30))(local_1b8,"nexus/loader/ScriptRuntime");
    if (lVar6 == 0) {
      pcVar10 = "runtime_class";
      goto LAB_00137db0;
    }
    lVar7 = (**(code **)(*local_1b8 + 0x30))(local_1b8,"nexus/loader/NexusLoader");
    if (lVar7 == 0) {
      FUN_0013816c(param_1,"loader_class");
    }
    else {
      lVar8 = (**(code **)(*local_1b8 + 0x388))
                        (local_1b8,lVar7,"NativeLoaded","(Ljava/lang/String;)Z");
      if ((lVar8 == 0) || (cVar2 = (**(code **)(*local_1b8 + 0x720))(), cVar2 != '\0')) {
        pcVar10 = "callback_method";
      }
      else {
        DAT_001f5ab0 = (**(code **)(*local_1b8 + 0x388))
                                 (local_1b8,lVar7,"record",
                                  "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
        if ((DAT_001f5ab0 == 0) || (cVar2 = (**(code **)(*local_1b8 + 0x720))(), cVar2 != '\0')) {
          pcVar10 = "record_method";
        }
        else {
          DAT_001f5ab8 = (**(code **)(*local_1b8 + 0xa8))(local_1b8,lVar7);
          if (DAT_001f5ab8 == 0) {
            pcVar10 = "loader_global_ref";
          }
          else {
            DAT_001f5640 = FUN_00135624(FUN_0013854c,0);
            if (DAT_001f5640 == 0) {
              pcVar10 = "engine_create";
            }
            else {
              puStack_188 = PTR_FUN_001eb9b0;
              local_190 = PTR_s___Ljava_lang_String__001eb9a8;
              puStack_178 = PTR_s__Ljava_lang_String_Ljava_lang_St_001eb9c0;
              puStack_180 = PTR_s_nativeEvaluateDescriptor_001eb9b8;
              local_170 = PTR_FUN_001eb9c8;
              puStack_1a8 = PTR_DAT_001eb990;
              local_1b0 = PTR_s_nativeApiVersion_001eb988;
              puStack_198 = PTR_s_nativeFormats_001eb9a0;
              puStack_1a0 = PTR_FUN_001eb998;
              iVar4 = (**(code **)(*local_1b8 + 0x6b8))(local_1b8,lVar6,&local_1b0,3);
              if ((iVar4 == 0) && (cVar2 = (**(code **)(*local_1b8 + 0x720))(), cVar2 == '\0')) {
                lVar9 = (**(code **)(*local_1b8 + 0x538))(local_1b8,"ScriptRuntime");
                if (lVar9 == 0) {
                  pcVar10 = "callback_argument";
                }
                else {
                  cVar2 = (**(code **)(*local_1b8 + 0x3a8))(local_1b8,lVar7,lVar8,lVar9);
                  (**(code **)(*local_1b8 + 0xb8))(local_1b8,lVar9);
                  cVar3 = (**(code **)(*local_1b8 + 0x720))();
                  pcVar10 = "callback_invocation_or_rejected";
                  if ((cVar3 == '\0') && (cVar2 != '\0')) {
                    (**(code **)(*local_1b8 + 0xb8))(local_1b8,lVar7);
                    (**(code **)(*local_1b8 + 0xb8))(local_1b8,lVar6);
                    goto LAB_00137dbc;
                  }
                }
                FUN_00138b2c(local_1b8,lVar6);
              }
              else {
                pcVar10 = "native_registration";
              }
            }
          }
        }
      }
      FUN_0013816c(param_1,pcVar10);
      FUN_00135a10(DAT_001f5640);
      DAT_001f5640 = 0;
      memset(&DAT_001f5660,0,0x450);
      DAT_001f5658 = 0;
      DAT_001f5650 = 0;
      DAT_001f5648 = 0;
      if (DAT_001f5ab8 != 0) {
        (**(code **)(*local_1b8 + 0xb0))();
        DAT_001f5ab8 = 0;
      }
      (**(code **)(*local_1b8 + 0xb8))(local_1b8,lVar7);
    }
    (**(code **)(*local_1b8 + 0xb8))(local_1b8,lVar6);
  }
  uVar11 = 0xffffffff;
LAB_00137dbc:
  if (*(long *)(lVar1 + 0x28) == local_68) {
    return uVar11;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0013816c @ 0013816c [libNexusScriptRuntime69252.so] ===== */

void FUN_0013816c(long *param_1,long param_2)

{
  bool bVar1;
  undefined4 uVar2;
  byte bVar3;
  long lVar4;
  char cVar5;
  int iVar6;
  uint uVar7;
  undefined4 *puVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  ulong uVar16;
  long *local_b8;
  byte local_ac [68];
  long local_68;
  
  lVar4 = tpidr_el0;
  local_68 = *(long *)(lVar4 + 0x28);
  puVar8 = (undefined4 *)__errno();
  uVar2 = *puVar8;
  local_b8 = (long *)0x0;
  if ((((param_1 != (long *)0x0) && (param_2 != 0)) &&
      (iVar6 = (**(code **)(*param_1 + 0x30))(param_1,&local_b8,0x10006), iVar6 == 0)) &&
     (local_b8 != (long *)0x0)) {
    lVar9 = (**(code **)(*local_b8 + 0x78))();
    if (lVar9 != 0) {
      (**(code **)(*local_b8 + 0x88))();
    }
    iVar6 = (**(code **)(*local_b8 + 0x98))(local_b8,0xc);
    if (iVar6 == 0) {
      uVar7 = snprintf((char *)local_ac,0x41,"native_%s_%s","script",param_2);
      if (0xffffffbf < uVar7 - 0x41) {
        bVar1 = 0 < (int)uVar7;
        if (0 < (int)uVar7) {
          uVar16 = 0;
          do {
            if (local_ac[uVar16] - 0x41 < 0x1a) {
              local_ac[uVar16] = local_ac[uVar16] + 0x20;
            }
            bVar3 = local_ac[uVar16];
            if (((0x19 < bVar3 - 0x61) && (bVar3 != 0x5f)) && (9 < bVar3 - 0x30)) {
              if (bVar1) goto LAB_001382f8;
              break;
            }
            uVar16 = uVar16 + 1;
            bVar1 = (long)uVar16 < (long)(int)uVar7;
          } while (uVar7 != uVar16);
        }
        lVar10 = (**(code **)(*local_b8 + 0x30))(local_b8,"nexus/loader/StartupDiagnostics");
        if ((((lVar10 != 0) && (cVar5 = (**(code **)(*local_b8 + 0x720))(), cVar5 == '\0')) &&
            ((((lVar11 = (**(code **)(*local_b8 + 0x388))
                                   (local_b8,lVar10,"failure",
                                    "(Ljava/lang/String;Ljava/lang/Throwable;)V"), lVar11 != 0 &&
               (((cVar5 = (**(code **)(*local_b8 + 0x720))(), cVar5 == '\0' &&
                 (lVar12 = (**(code **)(*local_b8 + 0x30))(local_b8,"java/lang/LinkageError"),
                 lVar12 != 0)) && (cVar5 = (**(code **)(*local_b8 + 0x720))(), cVar5 == '\0')))) &&
              ((lVar13 = (**(code **)(*local_b8 + 0x108))
                                   (local_b8,lVar12,"<init>","(Ljava/lang/String;)V"), lVar13 != 0
               && (cVar5 = (**(code **)(*local_b8 + 0x720))(), cVar5 == '\0')))) &&
             (lVar14 = (**(code **)(*local_b8 + 0x538))(local_b8,local_ac), lVar14 != 0)))) &&
           (((cVar5 = (**(code **)(*local_b8 + 0x720))(), cVar5 == '\0' &&
             (lVar15 = (**(code **)(*local_b8 + 0x538))(local_b8,"native_link_failed"), lVar15 != 0)
             ) && ((cVar5 = (**(code **)(*local_b8 + 0x720))(), cVar5 == '\0' &&
                   ((lVar12 = (**(code **)(*local_b8 + 0xe0))(local_b8,lVar12,lVar13,lVar15),
                    lVar12 != 0 && (cVar5 = (**(code **)(*local_b8 + 0x720))(), cVar5 == '\0')))))))
           ) {
          (**(code **)(*local_b8 + 0x468))(local_b8,lVar10,lVar11,lVar14,lVar12);
        }
      }
LAB_001382f8:
      cVar5 = (**(code **)(*local_b8 + 0x720))();
      if (cVar5 != '\0') {
        (**(code **)(*local_b8 + 0x88))();
      }
      (**(code **)(*local_b8 + 0xa0))(local_b8,0);
    }
    cVar5 = (**(code **)(*local_b8 + 0x720))();
    if (cVar5 != '\0') {
      (**(code **)(*local_b8 + 0x88))();
    }
    if (lVar9 != 0) {
      (**(code **)(*local_b8 + 0x68))(local_b8,lVar9);
      (**(code **)(*local_b8 + 0xb8))(local_b8,lVar9);
    }
  }
  *puVar8 = uVar2;
  if (*(long *)(lVar4 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0013854c @ 0013854c [libNexusScriptRuntime69252.so] ===== */

void FUN_0013854c(undefined8 param_1,undefined8 param_2,undefined8 param_3,byte *param_4)

{
  byte bVar1;
  long lVar2;
  long *plVar3;
  char cVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  byte local_2a8 [480];
  char acStack_c8 [96];
  long local_68;
  
  lVar2 = tpidr_el0;
  local_68 = *(long *)(lVar2 + 0x28);
  __android_log_print(4,"NexusScriptRuntime","id=%s stage=%s %s",param_2,param_3,param_4);
  plVar3 = DAT_001f5ac0;
  if ((((DAT_001f5ac0 == (long *)0x0) || (DAT_001f5ab8 == 0)) || (DAT_001f5ab0 == 0)) ||
     (cVar4 = (**(code **)(*DAT_001f5ac0 + 0x720))(DAT_001f5ac0), cVar4 != '\0')) goto LAB_00138758;
  snprintf(acStack_c8,0x60,"script_runtime_%s",param_3);
  bVar1 = *param_4;
  if (bVar1 == 0) {
    lVar7 = 0;
  }
  else {
    lVar7 = 0;
    param_4 = param_4 + 1;
    do {
      if (bVar1 - 0x20 < 0x5f) {
        local_2a8[lVar7] = bVar1;
        bVar1 = *param_4;
        lVar7 = lVar7 + 1;
      }
      else {
        snprintf((char *)(local_2a8 + lVar7),5,"\\x%02x");
        bVar1 = *param_4;
        lVar7 = lVar7 + 4;
      }
    } while ((bVar1 != 0) && (param_4 = param_4 + 1, lVar7 + 5U < 0x1e0));
  }
  local_2a8[lVar7] = 0;
  lVar7 = (**(code **)(*plVar3 + 0x538))(plVar3,acStack_c8);
  if (lVar7 == 0) {
    lVar5 = 0;
LAB_001387a4:
    lVar6 = 0;
  }
  else {
    lVar5 = (**(code **)(*plVar3 + 0x538))(plVar3,param_2);
    if (lVar5 == 0) goto LAB_001387a4;
    lVar6 = (**(code **)(*plVar3 + 0x538))(plVar3,local_2a8);
    if (lVar6 != 0) {
      (**(code **)(*plVar3 + 0x468))(plVar3,DAT_001f5ab8,DAT_001f5ab0,lVar7,lVar5,lVar6);
    }
  }
  cVar4 = (**(code **)(*plVar3 + 0x720))(plVar3);
  if (((lVar6 == 0) || (lVar5 == 0)) || ((lVar7 == 0 || (cVar4 != '\0')))) {
    DAT_001f5ac8 = 1;
  }
  if (lVar6 != 0) {
    (**(code **)(*plVar3 + 0xb8))(plVar3,lVar6);
  }
  if (lVar5 != 0) {
    (**(code **)(*plVar3 + 0xb8))(plVar3,lVar5);
  }
  if (lVar7 != 0) {
    (**(code **)(*plVar3 + 0xb8))(plVar3,lVar7);
  }
LAB_00138758:
  if (*(long *)(lVar2 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001387bc @ 001387bc [libNexusScriptRuntime69252.so] ===== */

void FUN_001387bc(long *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x001387cc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*param_1 + 0x538))(param_1,"nexus-js-source-v1");
  return;
}

/* ===== JNI_OnUnload @ 00138bc0 [libNexusScriptRuntime69252.so] ===== */

void JNI_OnUnload(long *param_1)

{
  long lVar1;
  int iVar2;
  long *local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  local_40 = (long *)0x0;
  pthread_mutex_lock((pthread_mutex_t *)&DAT_001f5618);
  FUN_00135a10(DAT_001f5640);
  DAT_001f5640 = 0;
  memset(&DAT_001f5660,0,0x450);
  DAT_001f5658 = 0;
  DAT_001f5648 = 0;
  DAT_001f5650 = 0;
  iVar2 = (**(code **)(*param_1 + 0x30))(param_1,&local_40,0x10006);
  if ((iVar2 == 0) && (DAT_001f5ab8 != 0)) {
    (**(code **)(*local_40 + 0xb0))();
  }
  DAT_001f5ab8 = 0;
  iVar2 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_001f5618);
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

/* ===== FUN_00139d50 @ 00139d50 [libNexusScriptRuntime69252.so] ===== */

void FUN_00139d50(long param_1)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  long lVar4;
  int *piVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  undefined8 *puVar10;
  int *local_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  long local_58;
  
  lVar4 = tpidr_el0;
  local_58 = *(long *)(lVar4 + 0x28);
  local_80 = *(int **)(param_1 + 0xe0);
  if (0xfffffff4 < (uint)*(undefined8 *)(param_1 + 0xe8)) {
    iVar1 = *local_80;
    iVar3 = iVar1 + -1;
    *local_80 = iVar3;
    if (iVar3 == 0 || iVar1 < 1) {
      FUN_00141774(param_1,local_80);
    }
  }
  lVar7 = param_1 + 0x120;
  if (*(long *)(param_1 + 0x128) != lVar7) {
    lVar6 = *(long *)(param_1 + 0x128);
    do {
      lVar8 = *(long *)(lVar6 + 8);
      if (0 < *(int *)(lVar6 + 0x20)) {
        lVar9 = 0;
        puVar10 = (undefined8 *)(lVar6 + 0x30);
        do {
          local_80 = (int *)puVar10[-1];
          if (0xfffffff4 < (uint)*puVar10) {
            iVar1 = *local_80;
            iVar3 = iVar1 + -1;
            *local_80 = iVar3;
            if (iVar3 == 0 || iVar1 < 1) {
              FUN_00141774(param_1,local_80);
            }
          }
          lVar9 = lVar9 + 1;
          puVar10 = puVar10 + 2;
        } while (lVar9 < *(int *)(lVar6 + 0x20));
      }
      (**(code **)(param_1 + 8))(param_1 + 0x20,lVar6);
      lVar6 = lVar8;
    } while (lVar8 != lVar7);
  }
  *(long *)(param_1 + 0x120) = lVar7;
  *(long *)(param_1 + 0x128) = lVar7;
  FUN_0013a388(param_1);
  if (*(long *)(param_1 + 0x90) == param_1 + 0x88) {
    if (0 < *(int *)(param_1 + 0x6c)) {
      lVar6 = 0;
      lVar7 = 0;
      do {
        if ((*(int *)(*(long *)(param_1 + 0x70) + lVar6) != 0) &&
           (uVar2 = *(uint *)(*(long *)(param_1 + 0x70) + lVar6 + 4), 0xe3 < (int)uVar2)) {
          piVar5 = *(int **)(*(long *)(param_1 + 0x60) + (ulong)uVar2 * 8);
          iVar1 = *piVar5;
          iVar3 = iVar1 + -1;
          *piVar5 = iVar3;
          if (iVar3 == 0 || iVar1 < 1) {
            FUN_001418dc(param_1);
          }
        }
        lVar7 = lVar7 + 1;
        lVar6 = lVar6 + 0x28;
      } while (lVar7 < *(int *)(param_1 + 0x6c));
    }
    puVar10 = (undefined8 *)(param_1 + 0x20);
    (**(code **)(param_1 + 8))(puVar10,*(undefined8 *)(param_1 + 0x70));
    thunk_FUN_001d34e8(param_1 + 400);
    if (0 < *(int *)(param_1 + 0x50)) {
      lVar7 = 0;
      do {
        if ((*(ulong *)(*(long *)(param_1 + 0x60) + lVar7 * 8) & 1) == 0) {
          (**(code **)(param_1 + 8))(puVar10);
        }
        lVar7 = lVar7 + 1;
      } while (lVar7 < *(int *)(param_1 + 0x50));
    }
    (**(code **)(param_1 + 8))(puVar10,*(undefined8 *)(param_1 + 0x60));
    (**(code **)(param_1 + 8))(puVar10,*(undefined8 *)(param_1 + 0x58));
    (**(code **)(param_1 + 8))(puVar10,*(undefined8 *)(param_1 + 0x188));
    uStack_78 = *(undefined8 *)(param_1 + 0x28);
    local_80 = (int *)*puVar10;
    uStack_68 = *(undefined8 *)(param_1 + 0x38);
    uStack_70 = *(undefined8 *)(param_1 + 0x30);
    (**(code **)(param_1 + 8))(&local_80,param_1);
    if (*(long *)(lVar4 + 0x28) == local_58) {
      return;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
                    /* WARNING: Subroutine does not return */
  __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
            ,0x82e,"void JS_FreeRuntime(JSRuntime *)","list_empty(&rt->gc_obj_list)");
}

/* ===== FUN_0013a388 @ 0013a388 [libNexusScriptRuntime69252.so] ===== */

void FUN_0013a388(long param_1)

{
  long *plVar1;
  byte bVar2;
  uint uVar3;
  int iVar4;
  long *plVar5;
  code *pcVar6;
  undefined8 *puVar7;
  long *plVar8;
  ulong uVar9;
  long *plVar10;
  undefined8 *puVar11;
  uint *puVar12;
  long lVar13;
  long lVar14;
  int *piVar15;
  long *plVar16;
  int *piVar17;
  long *plVar18;
  long lVar19;
  long *plVar20;
  
  plVar1 = (long *)(param_1 + 0xa8);
  plVar18 = (long *)(param_1 + 0x88);
  *(long **)(param_1 + 0xa8) = plVar1;
  *(long **)(param_1 + 0xb0) = plVar1;
  plVar20 = *(long **)(param_1 + 0x90);
  while (plVar8 = plVar20, plVar8 != plVar18) {
    if (0xf < *(byte *)((long)plVar8 + -4)) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x16df,"void gc_decref(JSRuntime *)","p->mark == 0");
    }
    bVar2 = *(byte *)((long)plVar8 + -4) & 0xf;
    if (5 < bVar2) {
                    /* WARNING: Subroutine does not return */
      abort();
    }
    plVar20 = (long *)plVar8[1];
    switch(bVar2) {
    case 0:
      piVar15 = (int *)plVar8[2];
      if (*piVar15 < 1) goto code_r0x0013c7b4;
      iVar4 = *piVar15 + -1;
      *piVar15 = iVar4;
      if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
        plVar10 = (long *)(piVar15 + 2);
        lVar13 = *plVar10;
        plVar5 = *(long **)(piVar15 + 4);
        *(long **)(lVar13 + 8) = plVar5;
        *plVar5 = lVar13;
        *plVar10 = 0;
        piVar15[4] = 0;
        piVar15[5] = 0;
        lVar13 = *plVar1;
        *(long **)(lVar13 + 8) = plVar10;
        *plVar10 = lVar13;
        *(long **)(piVar15 + 4) = plVar1;
        *plVar1 = (long)plVar10;
      }
      if (0 < piVar15[10]) {
        lVar13 = 0;
        piVar17 = piVar15 + 0x11;
        do {
          if (*piVar17 != 0) {
                    /* WARNING: Could not recover jumptable at 0x0013a4d4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
            (*(code *)(&UNK_0013a4d8 + (ulong)(byte)(&DAT_00112482)[(uint)piVar17[-1] >> 0x1e] * 4))
                      ();
            return;
          }
          lVar13 = lVar13 + 1;
          piVar17 = piVar17 + 2;
        } while (lVar13 < piVar15[10]);
      }
      if (((ulong)*(ushort *)((long)plVar8 + -2) != 1) &&
         (pcVar6 = *(code **)(*(long *)(param_1 + 0x70) +
                              (ulong)*(ushort *)((long)plVar8 + -2) * 0x28 + 0x10),
         pcVar6 != (code *)0x0)) {
        (*pcVar6)(param_1,plVar8 + -1,0xffffffffffffffff,FUN_0016b9cc);
      }
      break;
    case 1:
      if (0 < (int)plVar8[10]) {
        lVar14 = 0;
        lVar13 = 0;
        do {
          piVar15 = *(int **)(plVar8[9] + lVar14);
          if ((~*(uint *)((undefined8 *)(plVar8[9] + lVar14) + 1) & 0xfffffffe) == 0) {
            if (*piVar15 < 1) goto code_r0x0013c7b4;
            iVar4 = *piVar15 + -1;
            *piVar15 = iVar4;
            if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
              plVar10 = (long *)(piVar15 + 2);
              lVar19 = *plVar10;
              plVar5 = *(long **)(piVar15 + 4);
              *(long **)(lVar19 + 8) = plVar5;
              *plVar5 = lVar19;
              *plVar10 = 0;
              piVar15[4] = 0;
              piVar15[5] = 0;
              lVar19 = *plVar1;
              *(long **)(lVar19 + 8) = plVar10;
              *plVar10 = lVar19;
              *(long **)(piVar15 + 4) = plVar1;
              *plVar1 = (long)plVar10;
            }
          }
          lVar13 = lVar13 + 1;
          lVar14 = lVar14 + 0x10;
        } while (lVar13 < (int)plVar8[10]);
      }
      piVar15 = (int *)plVar8[8];
      if (piVar15 == (int *)0x0) break;
      goto LAB_0013b110;
    case 3:
      if ((*plVar8 & 0x10000000000) != 0) {
        uVar3 = *(uint *)((long *)plVar8[2] + 1);
        piVar15 = *(int **)plVar8[2];
        goto LAB_0013b100;
      }
      piVar15 = (int *)plVar8[5];
      goto joined_r0x0013a684;
    case 4:
      if ((int)plVar8[5] == 0) {
        piVar15 = (int *)plVar8[0xb];
        if ((~*(uint *)(plVar8 + 0xc) & 0xfffffffe) == 0) {
          if (*piVar15 < 1) goto code_r0x0013c7b4;
          iVar4 = *piVar15 + -1;
          *piVar15 = iVar4;
          if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
            plVar10 = (long *)(piVar15 + 2);
            lVar13 = *plVar10;
            plVar5 = *(long **)(piVar15 + 4);
            *(long **)(lVar13 + 8) = plVar5;
            *plVar5 = lVar13;
            *plVar10 = 0;
            piVar15[4] = 0;
            piVar15[5] = 0;
            lVar13 = *plVar1;
            *(long **)(lVar13 + 8) = plVar10;
            *plVar10 = lVar13;
            *(long **)(piVar15 + 4) = plVar1;
            *plVar1 = (long)plVar10;
          }
        }
        piVar15 = (int *)plVar8[2];
        if ((~*(uint *)(plVar8 + 3) & 0xfffffffe) == 0) {
          if (*piVar15 < 1) goto code_r0x0013c7b4;
          iVar4 = *piVar15 + -1;
          *piVar15 = iVar4;
          if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
            plVar10 = (long *)(piVar15 + 2);
            lVar13 = *plVar10;
            plVar5 = *(long **)(piVar15 + 4);
            *(long **)(lVar13 + 8) = plVar5;
            *plVar5 = lVar13;
            *plVar10 = 0;
            piVar15[4] = 0;
            piVar15[5] = 0;
            lVar13 = *plVar1;
            *(long **)(lVar13 + 8) = plVar10;
            *plVar10 = lVar13;
            *(long **)(piVar15 + 4) = plVar1;
            *plVar1 = (long)plVar10;
          }
        }
        puVar11 = (undefined8 *)plVar8[0x13];
        if (puVar11 != (undefined8 *)0x0) {
          for (puVar7 = (undefined8 *)plVar8[0xd]; puVar7 < puVar11; puVar7 = puVar7 + 2) {
            piVar15 = (int *)*puVar7;
            if ((~*(uint *)(puVar7 + 1) & 0xfffffffe) == 0) {
              if (*piVar15 < 1) goto code_r0x0013c7b4;
              iVar4 = *piVar15 + -1;
              *piVar15 = iVar4;
              if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
                plVar10 = (long *)(piVar15 + 2);
                lVar13 = *plVar10;
                plVar5 = *(long **)(piVar15 + 4);
                *(long **)(lVar13 + 8) = plVar5;
                *plVar5 = lVar13;
                *plVar10 = 0;
                piVar15[4] = 0;
                piVar15[5] = 0;
                lVar13 = *plVar1;
                *(long **)(lVar13 + 8) = plVar10;
                *plVar10 = lVar13;
                *(long **)(piVar15 + 4) = plVar1;
                *plVar1 = (long)plVar10;
              }
            }
            puVar11 = (undefined8 *)plVar8[0x13];
          }
        }
      }
      piVar15 = (int *)plVar8[6];
      if ((~*(uint *)(plVar8 + 7) & 0xfffffffe) == 0) {
        if (*piVar15 < 1) goto code_r0x0013c7b4;
        iVar4 = *piVar15 + -1;
        *piVar15 = iVar4;
        if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar1;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar1;
          *plVar1 = (long)plVar10;
        }
      }
      uVar3 = *(uint *)(plVar8 + 9);
      piVar15 = (int *)plVar8[8];
LAB_0013b100:
      if ((~uVar3 & 0xfffffffe) == 0) goto LAB_0013b110;
      break;
    case 5:
      for (plVar5 = (long *)plVar8[0x3b]; plVar5 != plVar8 + 0x3a; plVar5 = (long *)plVar5[1]) {
        if (0 < (int)plVar5[5]) {
          lVar14 = 0;
          lVar13 = 0;
          do {
            if ((*(int *)(plVar5[4] + lVar14 + 0x10) == 0) &&
               (piVar15 = *(int **)(plVar5[4] + lVar14 + 8), piVar15 != (int *)0x0)) {
              if (*piVar15 < 1) goto code_r0x0013c7b4;
              iVar4 = *piVar15 + -1;
              *piVar15 = iVar4;
              if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
                plVar16 = (long *)(piVar15 + 2);
                lVar19 = *plVar16;
                plVar10 = *(long **)(piVar15 + 4);
                *(long **)(lVar19 + 8) = plVar10;
                *plVar10 = lVar19;
                *plVar16 = 0;
                piVar15[4] = 0;
                piVar15[5] = 0;
                lVar19 = *plVar1;
                *(long **)(lVar19 + 8) = plVar16;
                *plVar16 = lVar19;
                *(long **)(piVar15 + 4) = plVar1;
                *plVar1 = (long)plVar16;
              }
            }
            lVar13 = lVar13 + 1;
            lVar14 = lVar14 + 0x20;
          } while (lVar13 < (int)plVar5[5]);
        }
        piVar15 = (int *)plVar5[10];
        if ((~*(uint *)(plVar5 + 0xb) & 0xfffffffe) == 0) {
          if (*piVar15 < 1) goto code_r0x0013c7b4;
          iVar4 = *piVar15 + -1;
          *piVar15 = iVar4;
          if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
            plVar16 = (long *)(piVar15 + 2);
            lVar13 = *plVar16;
            plVar10 = *(long **)(piVar15 + 4);
            *(long **)(lVar13 + 8) = plVar10;
            *plVar10 = lVar13;
            *plVar16 = 0;
            piVar15[4] = 0;
            piVar15[5] = 0;
            lVar13 = *plVar1;
            *(long **)(lVar13 + 8) = plVar16;
            *plVar16 = lVar13;
            *(long **)(piVar15 + 4) = plVar1;
            *plVar1 = (long)plVar16;
          }
        }
        piVar15 = (int *)plVar5[0xc];
        if ((~*(uint *)(plVar5 + 0xd) & 0xfffffffe) == 0) {
          if (*piVar15 < 1) goto code_r0x0013c7b4;
          iVar4 = *piVar15 + -1;
          *piVar15 = iVar4;
          if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
            plVar16 = (long *)(piVar15 + 2);
            lVar13 = *plVar16;
            plVar10 = *(long **)(piVar15 + 4);
            *(long **)(lVar13 + 8) = plVar10;
            *plVar10 = lVar13;
            *plVar16 = 0;
            piVar15[4] = 0;
            piVar15[5] = 0;
            lVar13 = *plVar1;
            *(long **)(lVar13 + 8) = plVar16;
            *plVar16 = lVar13;
            *(long **)(piVar15 + 4) = plVar1;
            *plVar1 = (long)plVar16;
          }
        }
        piVar15 = (int *)plVar5[0x1e];
        if ((~*(uint *)(plVar5 + 0x1f) & 0xfffffffe) == 0) {
          if (*piVar15 < 1) goto code_r0x0013c7b4;
          iVar4 = *piVar15 + -1;
          *piVar15 = iVar4;
          if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
            plVar16 = (long *)(piVar15 + 2);
            lVar13 = *plVar16;
            plVar10 = *(long **)(piVar15 + 4);
            *(long **)(lVar13 + 8) = plVar10;
            *plVar10 = lVar13;
            *plVar16 = 0;
            piVar15[4] = 0;
            piVar15[5] = 0;
            lVar13 = *plVar1;
            *(long **)(lVar13 + 8) = plVar16;
            *plVar16 = lVar13;
            *(long **)(piVar15 + 4) = plVar1;
            *plVar1 = (long)plVar16;
          }
        }
        piVar15 = (int *)plVar5[0x20];
        if ((~*(uint *)(plVar5 + 0x21) & 0xfffffffe) == 0) {
          if (*piVar15 < 1) goto code_r0x0013c7b4;
          iVar4 = *piVar15 + -1;
          *piVar15 = iVar4;
          if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
            plVar16 = (long *)(piVar15 + 2);
            lVar13 = *plVar16;
            plVar10 = *(long **)(piVar15 + 4);
            *(long **)(lVar13 + 8) = plVar10;
            *plVar10 = lVar13;
            *plVar16 = 0;
            piVar15[4] = 0;
            piVar15[5] = 0;
            lVar13 = *plVar1;
            *(long **)(lVar13 + 8) = plVar16;
            *plVar16 = lVar13;
            *(long **)(piVar15 + 4) = plVar1;
            *plVar1 = (long)plVar16;
          }
        }
        piVar15 = (int *)plVar5[0x17];
        if ((~*(uint *)(plVar5 + 0x18) & 0xfffffffe) == 0) {
          if (*piVar15 < 1) goto code_r0x0013c7b4;
          iVar4 = *piVar15 + -1;
          *piVar15 = iVar4;
          if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
            plVar16 = (long *)(piVar15 + 2);
            lVar13 = *plVar16;
            plVar10 = *(long **)(piVar15 + 4);
            *(long **)(lVar13 + 8) = plVar10;
            *plVar10 = lVar13;
            *plVar16 = 0;
            piVar15[4] = 0;
            piVar15[5] = 0;
            lVar13 = *plVar1;
            *(long **)(lVar13 + 8) = plVar16;
            *plVar16 = lVar13;
            *(long **)(piVar15 + 4) = plVar1;
            *plVar1 = (long)plVar16;
          }
        }
        piVar15 = (int *)plVar5[0x19];
        if ((~*(uint *)(plVar5 + 0x1a) & 0xfffffffe) == 0) {
          if (*piVar15 < 1) goto code_r0x0013c7b4;
          iVar4 = *piVar15 + -1;
          *piVar15 = iVar4;
          if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
            plVar16 = (long *)(piVar15 + 2);
            lVar13 = *plVar16;
            plVar10 = *(long **)(piVar15 + 4);
            *(long **)(lVar13 + 8) = plVar10;
            *plVar10 = lVar13;
            *plVar16 = 0;
            piVar15[4] = 0;
            piVar15[5] = 0;
            lVar13 = *plVar1;
            *(long **)(lVar13 + 8) = plVar16;
            *plVar16 = lVar13;
            *(long **)(piVar15 + 4) = plVar1;
            *plVar1 = (long)plVar16;
          }
        }
        piVar15 = (int *)plVar5[0x1b];
        if ((~*(uint *)(plVar5 + 0x1c) & 0xfffffffe) == 0) {
          if (*piVar15 < 1) goto code_r0x0013c7b4;
          iVar4 = *piVar15 + -1;
          *piVar15 = iVar4;
          if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
            plVar16 = (long *)(piVar15 + 2);
            lVar13 = *plVar16;
            plVar10 = *(long **)(piVar15 + 4);
            *(long **)(lVar13 + 8) = plVar10;
            *plVar10 = lVar13;
            *plVar16 = 0;
            piVar15[4] = 0;
            piVar15[5] = 0;
            lVar13 = *plVar1;
            *(long **)(lVar13 + 8) = plVar16;
            *plVar16 = lVar13;
            *(long **)(piVar15 + 4) = plVar1;
            *plVar1 = (long)plVar16;
          }
        }
      }
      piVar15 = (int *)plVar8[0x2e];
      if ((~*(uint *)(plVar8 + 0x2f) & 0xfffffffe) == 0) {
        if (*piVar15 < 1) goto code_r0x0013c7b4;
        iVar4 = *piVar15 + -1;
        *piVar15 = iVar4;
        if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar1;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar1;
          *plVar1 = (long)plVar10;
        }
      }
      piVar15 = (int *)plVar8[0x30];
      if ((~*(uint *)(plVar8 + 0x31) & 0xfffffffe) == 0) {
        if (*piVar15 < 1) goto code_r0x0013c7b4;
        iVar4 = *piVar15 + -1;
        *piVar15 = iVar4;
        if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar1;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar1;
          *plVar1 = (long)plVar10;
        }
      }
      piVar15 = (int *)plVar8[0x2a];
      if ((~*(uint *)(plVar8 + 0x2b) & 0xfffffffe) == 0) {
        if (*piVar15 < 1) goto code_r0x0013c7b4;
        iVar4 = *piVar15 + -1;
        *piVar15 = iVar4;
        if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar1;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar1;
          *plVar1 = (long)plVar10;
        }
      }
      piVar15 = (int *)plVar8[0x2c];
      if ((~*(uint *)(plVar8 + 0x2d) & 0xfffffffe) == 0) {
        if (*piVar15 < 1) goto code_r0x0013c7b4;
        iVar4 = *piVar15 + -1;
        *piVar15 = iVar4;
        if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar1;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar1;
          *plVar1 = (long)plVar10;
        }
      }
      piVar15 = (int *)plVar8[0x28];
      if ((~*(uint *)(plVar8 + 0x29) & 0xfffffffe) == 0) {
        if (*piVar15 < 1) goto code_r0x0013c7b4;
        iVar4 = *piVar15 + -1;
        *piVar15 = iVar4;
        if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar1;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar1;
          *plVar1 = (long)plVar10;
        }
      }
      lVar13 = 0;
      do {
        piVar15 = *(int **)((long)plVar8 + lVar13 + 0x90);
        if ((~*(uint *)((long)plVar8 + lVar13 + 0x98) & 0xfffffffe) == 0) {
          if (*piVar15 < 1) goto code_r0x0013c7b4;
          iVar4 = *piVar15 + -1;
          *piVar15 = iVar4;
          if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
            plVar10 = (long *)(piVar15 + 2);
            lVar14 = *plVar10;
            plVar5 = *(long **)(piVar15 + 4);
            *(long **)(lVar14 + 8) = plVar5;
            *plVar5 = lVar14;
            *plVar10 = 0;
            piVar15[4] = 0;
            piVar15[5] = 0;
            lVar14 = *plVar1;
            *(long **)(lVar14 + 8) = plVar10;
            *plVar10 = lVar14;
            *(long **)(piVar15 + 4) = plVar1;
            *plVar1 = (long)plVar10;
          }
        }
        lVar13 = lVar13 + 0x10;
      } while (lVar13 != 0x80);
      piVar15 = (int *)plVar8[0x22];
      if ((~*(uint *)(plVar8 + 0x23) & 0xfffffffe) == 0) {
        if (*piVar15 < 1) goto code_r0x0013c7b4;
        iVar4 = *piVar15 + -1;
        *piVar15 = iVar4;
        if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar1;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar1;
          *plVar1 = (long)plVar10;
        }
      }
      if (0 < *(int *)(param_1 + 0x6c)) {
        lVar14 = 0;
        lVar13 = 0;
        do {
          piVar15 = *(int **)(plVar8[7] + lVar14);
          if ((~*(uint *)((undefined8 *)(plVar8[7] + lVar14) + 1) & 0xfffffffe) == 0) {
            if (*piVar15 < 1) goto code_r0x0013c7b4;
            iVar4 = *piVar15 + -1;
            *piVar15 = iVar4;
            if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
              plVar10 = (long *)(piVar15 + 2);
              lVar19 = *plVar10;
              plVar5 = *(long **)(piVar15 + 4);
              *(long **)(lVar19 + 8) = plVar5;
              *plVar5 = lVar19;
              *plVar10 = 0;
              piVar15[4] = 0;
              piVar15[5] = 0;
              lVar19 = *plVar1;
              *(long **)(lVar19 + 8) = plVar10;
              *plVar10 = lVar19;
              *(long **)(piVar15 + 4) = plVar1;
              *plVar1 = (long)plVar10;
            }
          }
          lVar13 = lVar13 + 1;
          lVar14 = lVar14 + 0x10;
        } while (lVar13 < *(int *)(param_1 + 0x6c));
      }
      piVar15 = (int *)plVar8[0x24];
      if ((~*(uint *)(plVar8 + 0x25) & 0xfffffffe) == 0) {
        if (*piVar15 < 1) goto code_r0x0013c7b4;
        iVar4 = *piVar15 + -1;
        *piVar15 = iVar4;
        if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar1;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar1;
          *plVar1 = (long)plVar10;
        }
      }
      piVar15 = (int *)plVar8[0x26];
      if ((~*(uint *)(plVar8 + 0x27) & 0xfffffffe) == 0) {
        if (*piVar15 < 1) goto code_r0x0013c7b4;
        iVar4 = *piVar15 + -1;
        *piVar15 = iVar4;
        if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar1;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar1;
          *plVar1 = (long)plVar10;
        }
      }
      piVar15 = (int *)plVar8[0x10];
      if ((~*(uint *)(plVar8 + 0x11) & 0xfffffffe) == 0) {
        if (*piVar15 < 1) goto code_r0x0013c7b4;
        iVar4 = *piVar15 + -1;
        *piVar15 = iVar4;
        if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar1;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar1;
          *plVar1 = (long)plVar10;
        }
      }
      piVar15 = (int *)plVar8[0xc];
      if ((~*(uint *)(plVar8 + 0xd) & 0xfffffffe) == 0) {
        if (*piVar15 < 1) goto code_r0x0013c7b4;
        iVar4 = *piVar15 + -1;
        *piVar15 = iVar4;
        if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar1;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar1;
          *plVar1 = (long)plVar10;
        }
      }
      piVar15 = (int *)plVar8[0xe];
      if ((~*(uint *)(plVar8 + 0xf) & 0xfffffffe) == 0) {
        if (*piVar15 < 1) goto code_r0x0013c7b4;
        iVar4 = *piVar15 + -1;
        *piVar15 = iVar4;
        if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar1;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar1;
          *plVar1 = (long)plVar10;
        }
      }
      piVar15 = (int *)plVar8[10];
      if ((~*(uint *)(plVar8 + 0xb) & 0xfffffffe) == 0) {
        if (*piVar15 < 1) goto code_r0x0013c7b4;
        iVar4 = *piVar15 + -1;
        *piVar15 = iVar4;
        if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar1;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar1;
          *plVar1 = (long)plVar10;
        }
      }
      piVar15 = (int *)plVar8[8];
      if ((~*(uint *)(plVar8 + 9) & 0xfffffffe) == 0) {
        if (*piVar15 < 1) goto code_r0x0013c7b4;
        iVar4 = *piVar15 + -1;
        *piVar15 = iVar4;
        if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar1;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar1;
          *plVar1 = (long)plVar10;
        }
      }
    case 2:
      piVar15 = (int *)plVar8[6];
joined_r0x0013a684:
      if (piVar15 != (int *)0x0) {
LAB_0013b110:
        if (*piVar15 < 1) {
code_r0x0013c7b4:
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x16cb,"void gc_decref_child(JSRuntime *, JSGCObjectHeader *)",
                    "p->ref_count > 0");
        }
        iVar4 = *piVar15 + -1;
        *piVar15 = iVar4;
        if ((iVar4 == 0) && ((*(byte *)(piVar15 + 1) & 0xf0) == 0x10)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar1;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar1;
          *plVar1 = (long)plVar10;
        }
      }
    }
    *(byte *)((long)plVar8 + -4) = *(byte *)((long)plVar8 + -4) & 0xf | 0x10;
    if ((int)plVar8[-1] == 0) {
      lVar13 = *plVar8;
      plVar5 = (long *)plVar8[1];
      *(long **)(lVar13 + 8) = plVar5;
      *plVar5 = lVar13;
      *plVar8 = 0;
      plVar8[1] = 0;
      lVar13 = *plVar1;
      *(long **)(lVar13 + 8) = plVar8;
      *plVar8 = lVar13;
      plVar8[1] = (long)plVar1;
      *plVar1 = (long)plVar8;
    }
  }
  for (plVar20 = *(long **)(param_1 + 0x90); plVar20 != plVar18; plVar20 = (long *)plVar20[1]) {
    if ((int)plVar20[-1] < 1) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x1702,"void gc_scan(JSRuntime *)","p->ref_count > 0");
    }
    bVar2 = *(byte *)((long)plVar20 + -4) & 0xf;
    *(byte *)((long)plVar20 + -4) = bVar2;
    switch(bVar2) {
    case 0:
      piVar15 = (int *)plVar20[2];
      iVar4 = *piVar15;
      *piVar15 = iVar4 + 1;
      if (iVar4 == 0) {
        plVar5 = (long *)(piVar15 + 2);
        lVar13 = *plVar5;
        plVar8 = *(long **)(piVar15 + 4);
        *(long **)(lVar13 + 8) = plVar8;
        *plVar8 = lVar13;
        *plVar5 = 0;
        piVar15[4] = 0;
        piVar15[5] = 0;
        lVar13 = *plVar18;
        *(long **)(lVar13 + 8) = plVar5;
        *plVar5 = lVar13;
        *(long **)(piVar15 + 4) = plVar18;
        *plVar18 = (long)plVar5;
        *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
      }
      if (0 < piVar15[10]) {
        lVar13 = 0;
        piVar17 = piVar15 + 0x11;
        do {
          if (*piVar17 != 0) {
                    /* WARNING: Could not recover jumptable at 0x0013b3b0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
            (*(code *)(&UNK_0013b3b4 + (ulong)(byte)(&DAT_00112492)[(uint)piVar17[-1] >> 0x1e] * 4))
                      ();
            return;
          }
          lVar13 = lVar13 + 1;
          piVar17 = piVar17 + 2;
        } while (lVar13 < piVar15[10]);
      }
      if (((ulong)*(ushort *)((long)plVar20 + -2) != 1) &&
         (pcVar6 = *(code **)(*(long *)(param_1 + 0x70) +
                              (ulong)*(ushort *)((long)plVar20 + -2) * 0x28 + 0x10),
         pcVar6 != (code *)0x0)) {
        (*pcVar6)(param_1,plVar20 + -1,0xffffffffffffffff,FUN_0016ba4c);
      }
      break;
    case 1:
      if (0 < (int)plVar20[10]) {
        lVar14 = 0;
        lVar13 = 0;
        do {
          piVar15 = *(int **)(plVar20[9] + lVar14);
          if (((~*(uint *)((undefined8 *)(plVar20[9] + lVar14) + 1) & 0xfffffffe) == 0) &&
             (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
            plVar5 = (long *)(piVar15 + 2);
            lVar19 = *plVar5;
            plVar8 = *(long **)(piVar15 + 4);
            *(long **)(lVar19 + 8) = plVar8;
            *plVar8 = lVar19;
            *plVar5 = 0;
            piVar15[4] = 0;
            piVar15[5] = 0;
            lVar19 = *plVar18;
            *(long **)(lVar19 + 8) = plVar5;
            *plVar5 = lVar19;
            *(long **)(piVar15 + 4) = plVar18;
            *plVar18 = (long)plVar5;
            *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
          }
          lVar13 = lVar13 + 1;
          lVar14 = lVar14 + 0x10;
        } while (lVar13 < (int)plVar20[10]);
      }
      piVar15 = (int *)plVar20[8];
      if (piVar15 == (int *)0x0) break;
      goto LAB_0013be80;
    case 3:
      if ((*plVar20 & 0x10000000000) != 0) {
        uVar3 = *(uint *)((undefined8 *)plVar20[2] + 1);
        piVar15 = *(int **)plVar20[2];
        goto LAB_0013be70;
      }
      piVar15 = (int *)plVar20[5];
      goto joined_r0x0013b518;
    case 4:
      if ((int)plVar20[5] == 0) {
        piVar15 = (int *)plVar20[0xb];
        if (((~*(uint *)(plVar20 + 0xc) & 0xfffffffe) == 0) &&
           (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
          plVar5 = (long *)(piVar15 + 2);
          lVar13 = *plVar5;
          plVar8 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar8;
          *plVar8 = lVar13;
          *plVar5 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar18;
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *(long **)(piVar15 + 4) = plVar18;
          *plVar18 = (long)plVar5;
          *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
        }
        piVar15 = (int *)plVar20[2];
        if (((~*(uint *)(plVar20 + 3) & 0xfffffffe) == 0) &&
           (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
          plVar5 = (long *)(piVar15 + 2);
          lVar13 = *plVar5;
          plVar8 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar8;
          *plVar8 = lVar13;
          *plVar5 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar18;
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *(long **)(piVar15 + 4) = plVar18;
          *plVar18 = (long)plVar5;
          *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
        }
        puVar11 = (undefined8 *)plVar20[0x13];
        if (puVar11 != (undefined8 *)0x0) {
          for (puVar7 = (undefined8 *)plVar20[0xd]; puVar7 < puVar11; puVar7 = puVar7 + 2) {
            piVar15 = (int *)*puVar7;
            if (((~*(uint *)(puVar7 + 1) & 0xfffffffe) == 0) &&
               (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
              plVar5 = (long *)(piVar15 + 2);
              lVar13 = *plVar5;
              plVar8 = *(long **)(piVar15 + 4);
              *(long **)(lVar13 + 8) = plVar8;
              *plVar8 = lVar13;
              *plVar5 = 0;
              piVar15[4] = 0;
              piVar15[5] = 0;
              lVar13 = *plVar18;
              *(long **)(lVar13 + 8) = plVar5;
              *plVar5 = lVar13;
              *(long **)(piVar15 + 4) = plVar18;
              *plVar18 = (long)plVar5;
              *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
            }
            puVar11 = (undefined8 *)plVar20[0x13];
          }
        }
      }
      piVar15 = (int *)plVar20[6];
      if (((~*(uint *)(plVar20 + 7) & 0xfffffffe) == 0) &&
         (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
        plVar5 = (long *)(piVar15 + 2);
        lVar13 = *plVar5;
        plVar8 = *(long **)(piVar15 + 4);
        *(long **)(lVar13 + 8) = plVar8;
        *plVar8 = lVar13;
        *plVar5 = 0;
        piVar15[4] = 0;
        piVar15[5] = 0;
        lVar13 = *plVar18;
        *(long **)(lVar13 + 8) = plVar5;
        *plVar5 = lVar13;
        *(long **)(piVar15 + 4) = plVar18;
        *plVar18 = (long)plVar5;
        *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
      }
      uVar3 = *(uint *)(plVar20 + 9);
      piVar15 = (int *)plVar20[8];
LAB_0013be70:
      if ((~uVar3 & 0xfffffffe) == 0) goto LAB_0013be80;
      break;
    case 5:
      for (plVar8 = (long *)plVar20[0x3b]; plVar8 != plVar20 + 0x3a; plVar8 = (long *)plVar8[1]) {
        if (0 < (int)plVar8[5]) {
          lVar14 = 0;
          lVar13 = 0;
          do {
            if (((*(int *)(plVar8[4] + lVar14 + 0x10) == 0) &&
                (piVar15 = *(int **)(plVar8[4] + lVar14 + 8), piVar15 != (int *)0x0)) &&
               (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
              plVar10 = (long *)(piVar15 + 2);
              lVar19 = *plVar10;
              plVar5 = *(long **)(piVar15 + 4);
              *(long **)(lVar19 + 8) = plVar5;
              *plVar5 = lVar19;
              *plVar10 = 0;
              piVar15[4] = 0;
              piVar15[5] = 0;
              lVar19 = *plVar18;
              *(long **)(lVar19 + 8) = plVar10;
              *plVar10 = lVar19;
              *(long **)(piVar15 + 4) = plVar18;
              *plVar18 = (long)plVar10;
              *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
            }
            lVar13 = lVar13 + 1;
            lVar14 = lVar14 + 0x20;
          } while (lVar13 < (int)plVar8[5]);
        }
        piVar15 = (int *)plVar8[10];
        if (((~*(uint *)(plVar8 + 0xb) & 0xfffffffe) == 0) &&
           (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar18;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar18;
          *plVar18 = (long)plVar10;
          *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
        }
        piVar15 = (int *)plVar8[0xc];
        if (((~*(uint *)(plVar8 + 0xd) & 0xfffffffe) == 0) &&
           (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar18;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar18;
          *plVar18 = (long)plVar10;
          *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
        }
        piVar15 = (int *)plVar8[0x1e];
        if (((~*(uint *)(plVar8 + 0x1f) & 0xfffffffe) == 0) &&
           (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar18;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar18;
          *plVar18 = (long)plVar10;
          *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
        }
        piVar15 = (int *)plVar8[0x20];
        if (((~*(uint *)(plVar8 + 0x21) & 0xfffffffe) == 0) &&
           (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar18;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar18;
          *plVar18 = (long)plVar10;
          *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
        }
        piVar15 = (int *)plVar8[0x17];
        if (((~*(uint *)(plVar8 + 0x18) & 0xfffffffe) == 0) &&
           (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar18;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar18;
          *plVar18 = (long)plVar10;
          *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
        }
        piVar15 = (int *)plVar8[0x19];
        if (((~*(uint *)(plVar8 + 0x1a) & 0xfffffffe) == 0) &&
           (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar18;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar18;
          *plVar18 = (long)plVar10;
          *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
        }
        piVar15 = (int *)plVar8[0x1b];
        if (((~*(uint *)(plVar8 + 0x1c) & 0xfffffffe) == 0) &&
           (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
          plVar10 = (long *)(piVar15 + 2);
          lVar13 = *plVar10;
          plVar5 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *plVar10 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar18;
          *(long **)(lVar13 + 8) = plVar10;
          *plVar10 = lVar13;
          *(long **)(piVar15 + 4) = plVar18;
          *plVar18 = (long)plVar10;
          *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
        }
      }
      piVar15 = (int *)plVar20[0x2e];
      if (((~*(uint *)(plVar20 + 0x2f) & 0xfffffffe) == 0) &&
         (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
        plVar5 = (long *)(piVar15 + 2);
        lVar13 = *plVar5;
        plVar8 = *(long **)(piVar15 + 4);
        *(long **)(lVar13 + 8) = plVar8;
        *plVar8 = lVar13;
        *plVar5 = 0;
        piVar15[4] = 0;
        piVar15[5] = 0;
        lVar13 = *plVar18;
        *(long **)(lVar13 + 8) = plVar5;
        *plVar5 = lVar13;
        *(long **)(piVar15 + 4) = plVar18;
        *plVar18 = (long)plVar5;
        *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
      }
      piVar15 = (int *)plVar20[0x30];
      if (((~*(uint *)(plVar20 + 0x31) & 0xfffffffe) == 0) &&
         (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
        plVar5 = (long *)(piVar15 + 2);
        lVar13 = *plVar5;
        plVar8 = *(long **)(piVar15 + 4);
        *(long **)(lVar13 + 8) = plVar8;
        *plVar8 = lVar13;
        *plVar5 = 0;
        piVar15[4] = 0;
        piVar15[5] = 0;
        lVar13 = *plVar18;
        *(long **)(lVar13 + 8) = plVar5;
        *plVar5 = lVar13;
        *(long **)(piVar15 + 4) = plVar18;
        *plVar18 = (long)plVar5;
        *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
      }
      piVar15 = (int *)plVar20[0x2a];
      if (((~*(uint *)(plVar20 + 0x2b) & 0xfffffffe) == 0) &&
         (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
        plVar5 = (long *)(piVar15 + 2);
        lVar13 = *plVar5;
        plVar8 = *(long **)(piVar15 + 4);
        *(long **)(lVar13 + 8) = plVar8;
        *plVar8 = lVar13;
        *plVar5 = 0;
        piVar15[4] = 0;
        piVar15[5] = 0;
        lVar13 = *plVar18;
        *(long **)(lVar13 + 8) = plVar5;
        *plVar5 = lVar13;
        *(long **)(piVar15 + 4) = plVar18;
        *plVar18 = (long)plVar5;
        *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
      }
      piVar15 = (int *)plVar20[0x2c];
      if (((~*(uint *)(plVar20 + 0x2d) & 0xfffffffe) == 0) &&
         (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
        plVar5 = (long *)(piVar15 + 2);
        lVar13 = *plVar5;
        plVar8 = *(long **)(piVar15 + 4);
        *(long **)(lVar13 + 8) = plVar8;
        *plVar8 = lVar13;
        *plVar5 = 0;
        piVar15[4] = 0;
        piVar15[5] = 0;
        lVar13 = *plVar18;
        *(long **)(lVar13 + 8) = plVar5;
        *plVar5 = lVar13;
        *(long **)(piVar15 + 4) = plVar18;
        *plVar18 = (long)plVar5;
        *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
      }
      piVar15 = (int *)plVar20[0x28];
      if (((~*(uint *)(plVar20 + 0x29) & 0xfffffffe) == 0) &&
         (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
        plVar5 = (long *)(piVar15 + 2);
        lVar13 = *plVar5;
        plVar8 = *(long **)(piVar15 + 4);
        *(long **)(lVar13 + 8) = plVar8;
        *plVar8 = lVar13;
        *plVar5 = 0;
        piVar15[4] = 0;
        piVar15[5] = 0;
        lVar13 = *plVar18;
        *(long **)(lVar13 + 8) = plVar5;
        *plVar5 = lVar13;
        *(long **)(piVar15 + 4) = plVar18;
        *plVar18 = (long)plVar5;
        *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
      }
      lVar13 = 0;
      do {
        piVar15 = *(int **)((long)plVar20 + lVar13 + 0x90);
        if (((~*(uint *)((long)plVar20 + lVar13 + 0x98) & 0xfffffffe) == 0) &&
           (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
          plVar5 = (long *)(piVar15 + 2);
          lVar14 = *plVar5;
          plVar8 = *(long **)(piVar15 + 4);
          *(long **)(lVar14 + 8) = plVar8;
          *plVar8 = lVar14;
          *plVar5 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar14 = *plVar18;
          *(long **)(lVar14 + 8) = plVar5;
          *plVar5 = lVar14;
          *(long **)(piVar15 + 4) = plVar18;
          *plVar18 = (long)plVar5;
          *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
        }
        lVar13 = lVar13 + 0x10;
      } while (lVar13 != 0x80);
      piVar15 = (int *)plVar20[0x22];
      if (((~*(uint *)(plVar20 + 0x23) & 0xfffffffe) == 0) &&
         (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
        plVar5 = (long *)(piVar15 + 2);
        lVar13 = *plVar5;
        plVar8 = *(long **)(piVar15 + 4);
        *(long **)(lVar13 + 8) = plVar8;
        *plVar8 = lVar13;
        *plVar5 = 0;
        piVar15[4] = 0;
        piVar15[5] = 0;
        lVar13 = *plVar18;
        *(long **)(lVar13 + 8) = plVar5;
        *plVar5 = lVar13;
        *(long **)(piVar15 + 4) = plVar18;
        *plVar18 = (long)plVar5;
        *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
      }
      if (0 < *(int *)(param_1 + 0x6c)) {
        lVar14 = 0;
        lVar13 = 0;
        do {
          piVar15 = *(int **)(plVar20[7] + lVar14);
          if (((~*(uint *)((undefined8 *)(plVar20[7] + lVar14) + 1) & 0xfffffffe) == 0) &&
             (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
            plVar5 = (long *)(piVar15 + 2);
            lVar19 = *plVar5;
            plVar8 = *(long **)(piVar15 + 4);
            *(long **)(lVar19 + 8) = plVar8;
            *plVar8 = lVar19;
            *plVar5 = 0;
            piVar15[4] = 0;
            piVar15[5] = 0;
            lVar19 = *plVar18;
            *(long **)(lVar19 + 8) = plVar5;
            *plVar5 = lVar19;
            *(long **)(piVar15 + 4) = plVar18;
            *plVar18 = (long)plVar5;
            *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
          }
          lVar13 = lVar13 + 1;
          lVar14 = lVar14 + 0x10;
        } while (lVar13 < *(int *)(param_1 + 0x6c));
      }
      piVar15 = (int *)plVar20[0x24];
      if (((~*(uint *)(plVar20 + 0x25) & 0xfffffffe) == 0) &&
         (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
        plVar5 = (long *)(piVar15 + 2);
        lVar13 = *plVar5;
        plVar8 = *(long **)(piVar15 + 4);
        *(long **)(lVar13 + 8) = plVar8;
        *plVar8 = lVar13;
        *plVar5 = 0;
        piVar15[4] = 0;
        piVar15[5] = 0;
        lVar13 = *plVar18;
        *(long **)(lVar13 + 8) = plVar5;
        *plVar5 = lVar13;
        *(long **)(piVar15 + 4) = plVar18;
        *plVar18 = (long)plVar5;
        *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
      }
      piVar15 = (int *)plVar20[0x26];
      if (((~*(uint *)(plVar20 + 0x27) & 0xfffffffe) == 0) &&
         (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
        plVar5 = (long *)(piVar15 + 2);
        lVar13 = *plVar5;
        plVar8 = *(long **)(piVar15 + 4);
        *(long **)(lVar13 + 8) = plVar8;
        *plVar8 = lVar13;
        *plVar5 = 0;
        piVar15[4] = 0;
        piVar15[5] = 0;
        lVar13 = *plVar18;
        *(long **)(lVar13 + 8) = plVar5;
        *plVar5 = lVar13;
        *(long **)(piVar15 + 4) = plVar18;
        *plVar18 = (long)plVar5;
        *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
      }
      piVar15 = (int *)plVar20[0x10];
      if (((~*(uint *)(plVar20 + 0x11) & 0xfffffffe) == 0) &&
         (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
        plVar5 = (long *)(piVar15 + 2);
        lVar13 = *plVar5;
        plVar8 = *(long **)(piVar15 + 4);
        *(long **)(lVar13 + 8) = plVar8;
        *plVar8 = lVar13;
        *plVar5 = 0;
        piVar15[4] = 0;
        piVar15[5] = 0;
        lVar13 = *plVar18;
        *(long **)(lVar13 + 8) = plVar5;
        *plVar5 = lVar13;
        *(long **)(piVar15 + 4) = plVar18;
        *plVar18 = (long)plVar5;
        *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
      }
      piVar15 = (int *)plVar20[0xc];
      if (((~*(uint *)(plVar20 + 0xd) & 0xfffffffe) == 0) &&
         (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
        plVar5 = (long *)(piVar15 + 2);
        lVar13 = *plVar5;
        plVar8 = *(long **)(piVar15 + 4);
        *(long **)(lVar13 + 8) = plVar8;
        *plVar8 = lVar13;
        *plVar5 = 0;
        piVar15[4] = 0;
        piVar15[5] = 0;
        lVar13 = *plVar18;
        *(long **)(lVar13 + 8) = plVar5;
        *plVar5 = lVar13;
        *(long **)(piVar15 + 4) = plVar18;
        *plVar18 = (long)plVar5;
        *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
      }
      piVar15 = (int *)plVar20[0xe];
      if (((~*(uint *)(plVar20 + 0xf) & 0xfffffffe) == 0) &&
         (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
        plVar5 = (long *)(piVar15 + 2);
        lVar13 = *plVar5;
        plVar8 = *(long **)(piVar15 + 4);
        *(long **)(lVar13 + 8) = plVar8;
        *plVar8 = lVar13;
        *plVar5 = 0;
        piVar15[4] = 0;
        piVar15[5] = 0;
        lVar13 = *plVar18;
        *(long **)(lVar13 + 8) = plVar5;
        *plVar5 = lVar13;
        *(long **)(piVar15 + 4) = plVar18;
        *plVar18 = (long)plVar5;
        *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
      }
      piVar15 = (int *)plVar20[10];
      if (((~*(uint *)(plVar20 + 0xb) & 0xfffffffe) == 0) &&
         (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
        plVar5 = (long *)(piVar15 + 2);
        lVar13 = *plVar5;
        plVar8 = *(long **)(piVar15 + 4);
        *(long **)(lVar13 + 8) = plVar8;
        *plVar8 = lVar13;
        *plVar5 = 0;
        piVar15[4] = 0;
        piVar15[5] = 0;
        lVar13 = *plVar18;
        *(long **)(lVar13 + 8) = plVar5;
        *plVar5 = lVar13;
        *(long **)(piVar15 + 4) = plVar18;
        *plVar18 = (long)plVar5;
        *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
      }
      piVar15 = (int *)plVar20[8];
      if (((~*(uint *)(plVar20 + 9) & 0xfffffffe) == 0) &&
         (iVar4 = *piVar15, *piVar15 = iVar4 + 1, iVar4 == 0)) {
        plVar5 = (long *)(piVar15 + 2);
        lVar13 = *plVar5;
        plVar8 = *(long **)(piVar15 + 4);
        *(long **)(lVar13 + 8) = plVar8;
        *plVar8 = lVar13;
        *plVar5 = 0;
        piVar15[4] = 0;
        piVar15[5] = 0;
        lVar13 = *plVar18;
        *(long **)(lVar13 + 8) = plVar5;
        *plVar5 = lVar13;
        *(long **)(piVar15 + 4) = plVar18;
        *plVar18 = (long)plVar5;
        *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
      }
    case 2:
      piVar15 = (int *)plVar20[6];
joined_r0x0013b518:
      if (piVar15 != (int *)0x0) {
LAB_0013be80:
        iVar4 = *piVar15;
        *piVar15 = iVar4 + 1;
        if (iVar4 == 0) {
          plVar5 = (long *)(piVar15 + 2);
          lVar13 = *plVar5;
          plVar8 = *(long **)(piVar15 + 4);
          *(long **)(lVar13 + 8) = plVar8;
          *plVar8 = lVar13;
          *plVar5 = 0;
          piVar15[4] = 0;
          piVar15[5] = 0;
          lVar13 = *plVar18;
          *(long **)(lVar13 + 8) = plVar5;
          *plVar5 = lVar13;
          *(long **)(piVar15 + 4) = plVar18;
          *plVar18 = (long)plVar5;
          *(byte *)(piVar15 + 1) = *(byte *)(piVar15 + 1) & 0xf;
        }
      }
      break;
    default:
                    /* WARNING: Subroutine does not return */
      abort();
    }
  }
  plVar18 = *(long **)(param_1 + 0xb0);
  do {
    if (plVar18 == plVar1) {
      plVar18 = *(long **)(param_1 + 0xb0);
      *(undefined1 *)(param_1 + 0xb8) = 2;
      if (plVar18 != plVar1) {
        plVar20 = (long *)(param_1 + 0x98);
        do {
          uVar3 = *(byte *)((long)plVar18 + -4) & 0xf;
          if (uVar3 < 5 && (1 << (ulong)uVar3 & 0x13U) != 0) {
            FUN_0016b224(param_1,plVar18 + -1);
          }
          else {
            lVar13 = *plVar18;
            plVar8 = (long *)plVar18[1];
            *(long **)(lVar13 + 8) = plVar8;
            *plVar8 = lVar13;
            *plVar18 = 0;
            plVar18[1] = 0;
            lVar13 = *plVar20;
            *(long **)(lVar13 + 8) = plVar18;
            *plVar18 = lVar13;
            plVar18[1] = (long)plVar20;
            *plVar20 = (long)plVar18;
          }
          plVar18 = *(long **)(param_1 + 0xb0);
        } while (plVar18 != plVar1);
      }
      lVar13 = param_1 + 0x98;
      *(undefined1 *)(param_1 + 0xb8) = 0;
      if (*(long *)(param_1 + 0xa0) != lVar13) {
        lVar14 = *(long *)(param_1 + 0xa0);
        do {
          bVar2 = *(byte *)(lVar14 + -4) & 0xf;
          if (1 < bVar2 && bVar2 != 4) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0x173a,"void gc_free_cycles(JSRuntime *)",
                      "p->gc_obj_type == JS_GC_OBJ_TYPE_JS_OBJECT || p->gc_obj_type == JS_GC_OBJ_TYPE_FUNCTION_BYTECODE || p->gc_obj_type == JS_GC_OBJ_TYPE_ASYNC_FUNCTION"
                     );
          }
          lVar19 = *(long *)(lVar14 + 8);
          (**(code **)(param_1 + 8))(param_1 + 0x20,lVar14 + -8);
          lVar14 = lVar19;
        } while (lVar19 != lVar13);
      }
      *(long *)(param_1 + 0x98) = lVar13;
      *(long *)(param_1 + 0xa0) = lVar13;
      return;
    }
    switch(*(byte *)((long)plVar18 + -4) & 0xf) {
    case 0:
      piVar15 = (int *)plVar18[2];
      uVar9 = (ulong)(uint)piVar15[10];
      *piVar15 = *piVar15 + 1;
      if (0 < piVar15[10]) {
        piVar15 = piVar15 + 0x11;
        do {
          if (*piVar15 != 0) {
                    /* WARNING: Could not recover jumptable at 0x0013c0c0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
            (*(code *)(&UNK_0013c080 + (ulong)(byte)(&DAT_0011249c)[(uint)piVar15[-1] >> 0x1e] * 4))
                      ();
            return;
          }
          uVar9 = uVar9 - 1;
          piVar15 = piVar15 + 2;
        } while (uVar9 != 0);
      }
      if (((ulong)*(ushort *)((long)plVar18 + -2) != 1) &&
         (pcVar6 = *(code **)(*(long *)(param_1 + 0x70) +
                              (ulong)*(ushort *)((long)plVar18 + -2) * 0x28 + 0x10),
         pcVar6 != (code *)0x0)) {
        (*pcVar6)(param_1,plVar18 + -1,0xffffffffffffffff,FUN_0016ba94);
      }
      break;
    case 1:
      uVar9 = (ulong)*(uint *)(plVar18 + 10);
      if (0 < (int)*(uint *)(plVar18 + 10)) {
        puVar12 = (uint *)(plVar18[9] + 8);
        do {
          if ((~*puVar12 & 0xfffffffe) == 0) {
            **(int **)(puVar12 + -2) = **(int **)(puVar12 + -2) + 1;
          }
          uVar9 = uVar9 - 1;
          puVar12 = puVar12 + 4;
        } while (uVar9 != 0);
      }
      piVar15 = (int *)plVar18[8];
      if (piVar15 == (int *)0x0) break;
      goto LAB_0013c020;
    case 3:
      if ((*plVar18 & 0x10000000000) != 0) {
        uVar3 = *(uint *)((undefined8 *)plVar18[2] + 1);
        piVar15 = *(int **)plVar18[2];
        goto LAB_0013c61c;
      }
      piVar15 = (int *)plVar18[5];
      goto joined_r0x0013c168;
    case 4:
      if ((int)plVar18[5] == 0) {
        if ((~*(uint *)(plVar18 + 0xc) & 0xfffffffe) == 0) {
          *(int *)plVar18[0xb] = *(int *)plVar18[0xb] + 1;
        }
        if ((~*(uint *)(plVar18 + 3) & 0xfffffffe) == 0) {
          *(int *)plVar18[2] = *(int *)plVar18[2] + 1;
        }
        puVar11 = (undefined8 *)plVar18[0x13];
        if (puVar11 != (undefined8 *)0x0) {
          for (puVar7 = (undefined8 *)plVar18[0xd]; puVar7 < puVar11; puVar7 = puVar7 + 2) {
            if ((~*(uint *)(puVar7 + 1) & 0xfffffffe) == 0) {
              *(int *)*puVar7 = *(int *)*puVar7 + 1;
            }
          }
        }
      }
      if ((~*(uint *)(plVar18 + 7) & 0xfffffffe) == 0) {
        *(int *)plVar18[6] = *(int *)plVar18[6] + 1;
      }
      uVar3 = *(uint *)(plVar18 + 9);
      piVar15 = (int *)plVar18[8];
LAB_0013c61c:
      if ((~uVar3 & 0xfffffffe) == 0) goto LAB_0013c020;
      break;
    case 5:
      for (plVar20 = (long *)plVar18[0x3b]; plVar20 != plVar18 + 0x3a; plVar20 = (long *)plVar20[1])
      {
        lVar13 = (long)(int)plVar20[5];
        if (0 < (int)plVar20[5]) {
          piVar15 = (int *)(plVar20[4] + 0x10);
          do {
            if ((*piVar15 == 0) && (piVar17 = *(int **)(piVar15 + -2), piVar17 != (int *)0x0)) {
              *piVar17 = *piVar17 + 1;
            }
            piVar15 = piVar15 + 8;
            lVar13 = lVar13 + -1;
          } while (lVar13 != 0);
        }
        if ((~*(uint *)(plVar20 + 0xb) & 0xfffffffe) == 0) {
          *(int *)plVar20[10] = *(int *)plVar20[10] + 1;
        }
        if ((~*(uint *)(plVar20 + 0xd) & 0xfffffffe) == 0) {
          *(int *)plVar20[0xc] = *(int *)plVar20[0xc] + 1;
        }
        if ((~*(uint *)(plVar20 + 0x1f) & 0xfffffffe) == 0) {
          *(int *)plVar20[0x1e] = *(int *)plVar20[0x1e] + 1;
        }
        if ((~*(uint *)(plVar20 + 0x21) & 0xfffffffe) == 0) {
          *(int *)plVar20[0x20] = *(int *)plVar20[0x20] + 1;
        }
        if ((~*(uint *)(plVar20 + 0x18) & 0xfffffffe) == 0) {
          *(int *)plVar20[0x17] = *(int *)plVar20[0x17] + 1;
        }
        if ((~*(uint *)(plVar20 + 0x1a) & 0xfffffffe) == 0) {
          *(int *)plVar20[0x19] = *(int *)plVar20[0x19] + 1;
        }
        if ((~*(uint *)(plVar20 + 0x1c) & 0xfffffffe) == 0) {
          *(int *)plVar20[0x1b] = *(int *)plVar20[0x1b] + 1;
        }
      }
      if ((~*(uint *)(plVar18 + 0x2f) & 0xfffffffe) == 0) {
        *(int *)plVar18[0x2e] = *(int *)plVar18[0x2e] + 1;
      }
      if ((~*(uint *)(plVar18 + 0x31) & 0xfffffffe) == 0) {
        *(int *)plVar18[0x30] = *(int *)plVar18[0x30] + 1;
      }
      if ((~*(uint *)(plVar18 + 0x2b) & 0xfffffffe) == 0) {
        *(int *)plVar18[0x2a] = *(int *)plVar18[0x2a] + 1;
      }
      if ((~*(uint *)(plVar18 + 0x2d) & 0xfffffffe) == 0) {
        *(int *)plVar18[0x2c] = *(int *)plVar18[0x2c] + 1;
      }
      if ((~*(uint *)(plVar18 + 0x29) & 0xfffffffe) == 0) {
        *(int *)plVar18[0x28] = *(int *)plVar18[0x28] + 1;
      }
      lVar13 = 0;
      do {
        piVar15 = *(int **)((long)plVar18 + lVar13 + 0x90);
        if ((~*(uint *)((long)plVar18 + lVar13 + 0x98) & 0xfffffffe) == 0) {
          *piVar15 = *piVar15 + 1;
        }
        lVar13 = lVar13 + 0x10;
      } while (lVar13 != 0x80);
      if ((~*(uint *)(plVar18 + 0x23) & 0xfffffffe) == 0) {
        *(int *)plVar18[0x22] = *(int *)plVar18[0x22] + 1;
      }
      lVar13 = (long)*(int *)(param_1 + 0x6c);
      if (0 < *(int *)(param_1 + 0x6c)) {
        puVar12 = (uint *)(plVar18[7] + 8);
        do {
          if ((~*puVar12 & 0xfffffffe) == 0) {
            **(int **)(puVar12 + -2) = **(int **)(puVar12 + -2) + 1;
          }
          lVar13 = lVar13 + -1;
          puVar12 = puVar12 + 4;
        } while (lVar13 != 0);
      }
      if ((~*(uint *)(plVar18 + 0x25) & 0xfffffffe) == 0) {
        *(int *)plVar18[0x24] = *(int *)plVar18[0x24] + 1;
      }
      if ((~*(uint *)(plVar18 + 0x27) & 0xfffffffe) == 0) {
        *(int *)plVar18[0x26] = *(int *)plVar18[0x26] + 1;
      }
      if ((~*(uint *)(plVar18 + 0x11) & 0xfffffffe) == 0) {
        *(int *)plVar18[0x10] = *(int *)plVar18[0x10] + 1;
      }
      if ((~*(uint *)(plVar18 + 0xd) & 0xfffffffe) == 0) {
        *(int *)plVar18[0xc] = *(int *)plVar18[0xc] + 1;
      }
      if ((~*(uint *)(plVar18 + 0xf) & 0xfffffffe) == 0) {
        *(int *)plVar18[0xe] = *(int *)plVar18[0xe] + 1;
      }
      if ((~*(uint *)(plVar18 + 0xb) & 0xfffffffe) == 0) {
        *(int *)plVar18[10] = *(int *)plVar18[10] + 1;
      }
      if ((~*(uint *)(plVar18 + 9) & 0xfffffffe) == 0) {
        *(int *)plVar18[8] = *(int *)plVar18[8] + 1;
      }
    case 2:
      piVar15 = (int *)plVar18[6];
joined_r0x0013c168:
      if (piVar15 != (int *)0x0) {
LAB_0013c020:
        *piVar15 = *piVar15 + 1;
      }
      break;
    default:
                    /* WARNING: Subroutine does not return */
      abort();
    }
    plVar18 = (long *)plVar18[1];
  } while( true );
}

/* ===== FUN_0013c864 @ 0013c864 [libNexusScriptRuntime69252.so] ===== */

undefined8 * FUN_0013c864(undefined8 *param_1)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  undefined8 *puVar5;
  size_t sVar6;
  int *piVar7;
  long *plVar8;
  long lVar9;
  undefined8 uVar10;
  long lVar11;
  long *plVar12;
  undefined8 *puVar13;
  char *__s;
  undefined **ppuVar14;
  undefined1 auVar15 [16];
  undefined1 auVar16 [16];
  
  puVar13 = param_1 + 4;
  puVar5 = (undefined8 *)(*(code *)*param_1)(puVar13,0x200);
  if (puVar5 == (undefined8 *)0x0) {
    return (undefined8 *)0x0;
  }
  puVar5[1] = 0;
  *puVar5 = 0;
  puVar5[3] = 0;
  puVar5[2] = 0;
  puVar5[5] = 0;
  puVar5[4] = 0;
  puVar5[7] = 0;
  puVar5[6] = 0;
  puVar5[9] = 0;
  puVar5[8] = 0;
  puVar5[0xb] = 0;
  puVar5[10] = 0;
  puVar5[0xd] = 0;
  puVar5[0xc] = 0;
  puVar5[0xf] = 0;
  puVar5[0xe] = 0;
  puVar5[0x11] = 0;
  puVar5[0x10] = 0;
  puVar5[0x13] = 0;
  puVar5[0x12] = 0;
  puVar5[0x15] = 0;
  puVar5[0x14] = 0;
  puVar5[0x17] = 0;
  puVar5[0x16] = 0;
  puVar5[0x19] = 0;
  puVar5[0x18] = 0;
  puVar5[0x1b] = 0;
  puVar5[0x1a] = 0;
  puVar5[0x1d] = 0;
  puVar5[0x1c] = 0;
  puVar5[0x1f] = 0;
  puVar5[0x1e] = 0;
  puVar5[0x21] = 0;
  puVar5[0x20] = 0;
  puVar5[0x23] = 0;
  puVar5[0x22] = 0;
  puVar5[0x25] = 0;
  puVar5[0x24] = 0;
  puVar5[0x27] = 0;
  puVar5[0x26] = 0;
  puVar5[0x29] = 0;
  puVar5[0x28] = 0;
  puVar5[0x2b] = 0;
  puVar5[0x2a] = 0;
  puVar5[0x2d] = 0;
  puVar5[0x2c] = 0;
  puVar5[0x2f] = 0;
  puVar5[0x2e] = 0;
  puVar5[0x31] = 0;
  puVar5[0x30] = 0;
  puVar5[0x33] = 0;
  puVar5[0x32] = 0;
  puVar5[0x35] = 0;
  puVar5[0x34] = 0;
  puVar5[0x37] = 0;
  puVar5[0x36] = 0;
  puVar5[0x39] = 0;
  puVar5[0x38] = 0;
  puVar5[0x3b] = 0;
  puVar5[0x3a] = 0;
  puVar5[0x3d] = 0;
  puVar5[0x3c] = 0;
  puVar5[0x3f] = 0;
  puVar5[0x3e] = 0;
  *(undefined4 *)puVar5 = 1;
  *(undefined1 *)((long)puVar5 + 4) = 5;
  plVar12 = param_1 + 0x11;
  lVar11 = *plVar12;
  plVar8 = puVar5 + 1;
  *plVar8 = lVar11;
  *(long **)(lVar11 + 8) = plVar8;
  puVar5[2] = plVar12;
  *plVar12 = (long)plVar8;
  lVar11 = (*(code *)*param_1)(puVar13,(long)*(int *)((long)param_1 + 0x6c) << 4);
  puVar5[8] = lVar11;
  if (lVar11 == 0) {
    (*(code *)param_1[1])(puVar13,puVar5);
    return (undefined8 *)0x0;
  }
  plVar8 = param_1 + 0xf;
  lVar11 = *plVar8;
  plVar12 = puVar5 + 4;
  *plVar12 = lVar11;
  puVar5[3] = param_1;
  *(long **)(lVar11 + 8) = plVar12;
  puVar5[5] = plVar8;
  *plVar8 = (long)plVar12;
  puVar5[0x37] = param_1 + 0x32;
  puVar5[0x38] = 0x71;
  *(undefined4 *)(puVar5 + 0x39) = 0x5c8;
  if (0 < *(int *)((long)param_1 + 0x6c)) {
    lVar9 = 0;
    lVar11 = 0;
    do {
      lVar11 = lVar11 + 1;
      puVar1 = (undefined4 *)(puVar5[8] + lVar9);
      lVar9 = lVar9 + 0x10;
      *puVar1 = 0;
      *(undefined8 *)(puVar1 + 2) = 2;
    } while (lVar11 < *(int *)((long)param_1 + 0x6c));
  }
  *(undefined4 *)(puVar5 + 0xd) = 0;
  *(undefined4 *)(puVar5 + 0xf) = 0;
  lVar11 = puVar5[8];
  puVar5[0xe] = 2;
  puVar5[0x10] = 2;
  *(undefined4 *)(puVar5 + 0x11) = 0;
  puVar5[0x12] = 2;
  *(undefined4 *)(puVar5 + 0x23) = 0;
  puVar5[0x24] = 2;
  puVar5[0x3b] = puVar5 + 0x3b;
  puVar5[0x3c] = puVar5 + 0x3b;
  for (piVar7 = *(int **)(*(long *)(puVar5[3] + 0x188) +
                         (ulong)(0x3c6e0001 >> (ulong)(-*(int *)(puVar5[3] + 0x178) & 0x1f)) * 8);
      (piVar7 != (int *)0x0 &&
      (((piVar7[7] != 0x3c6e0001 || (*(long *)(piVar7 + 0xe) != 0)) || (piVar7[10] != 0))));
      piVar7 = *(int **)(piVar7 + 0xc)) {
  }
  if (piVar7 == (int *)0x0) {
    piVar7 = (int *)FUN_0016b030(puVar5,0,2);
    if (piVar7 == (int *)0x0) {
      auVar15 = ZEXT816(6) << 0x40;
      goto LAB_0013ca3c;
    }
  }
  else {
    *piVar7 = *piVar7 + 1;
  }
  auVar15 = FUN_00140ed0(puVar5,piVar7,1);
LAB_0013ca3c:
  *(undefined1 (*) [16])(lVar11 + 0x10) = auVar15;
  auVar15 = FUN_001412b8(puVar5,FUN_001b45e4,&DAT_0010cd92,0,0,0,*(undefined8 *)(puVar5[8] + 0x10),
                         *(undefined8 *)(puVar5[8] + 0x18));
  lVar11 = puVar5[8];
  *(undefined1 (*) [16])(puVar5 + 9) = auVar15;
  if (0xfffffff4 < auVar15._8_4_) {
    *auVar15._0_8_ = *auVar15._0_8_ + 1;
  }
  *(undefined1 (*) [16])(lVar11 + 0xd0) = auVar15;
  lVar11 = puVar5[8];
  auVar15 = FUN_00140e0c(puVar5,*(undefined8 *)(lVar11 + 0x10),*(undefined8 *)(lVar11 + 0x18),1);
  *(undefined1 (*) [16])(lVar11 + 0x30) = auVar15;
  FUN_0015b874(puVar5,*(undefined8 *)(puVar5[8] + 0x30),*(undefined8 *)(puVar5[8] + 0x38),
               &PTR_s_toString_001eea48,3);
  ppuVar14 = &PTR_s_EvalError_001ec8d0;
  lVar11 = 0;
  do {
    auVar15 = FUN_001411c8(puVar5,*(undefined8 *)(puVar5[8] + 0x30),
                           *(undefined8 *)(puVar5[8] + 0x38));
    __s = *ppuVar14;
    sVar6 = strlen(__s);
    uVar4 = FUN_0013f2ec(puVar5,__s,sVar6);
    if (uVar4 == 0) {
      auVar16 = ZEXT816(6) << 0x40;
    }
    else {
      auVar16 = FUN_00140044(puVar5,(ulong)uVar4,1);
      if ((0xe3 < (int)uVar4) &&
         (piVar7 = *(int **)(*(long *)(puVar5[3] + 0x60) + (ulong)uVar4 * 8), iVar2 = *piVar7,
         iVar3 = iVar2 + -1, *piVar7 = iVar3, iVar3 == 0 || iVar2 < 1)) {
        FUN_001418dc();
      }
    }
    piVar7 = auVar16._0_8_;
    FUN_00148018(puVar5,auVar15._0_8_,auVar15._8_8_,0x38,piVar7,auVar16._8_8_,0,3,0,3,0x2703);
    if ((0xfffffff4 < auVar16._8_4_) &&
       (iVar2 = *piVar7, iVar3 = iVar2 + -1, *piVar7 = iVar3, iVar3 == 0 || iVar2 < 1)) {
      FUN_00141774(puVar5[3],piVar7,auVar16._8_8_);
    }
    if (*(uint *)(puVar5[3] + 0x50) < 0x30) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0xc94,"JSValue __JS_AtomToValue(JSContext *, JSAtom, BOOL)","atom < rt->atom_size")
      ;
    }
    piVar7 = *(int **)(*(long *)(puVar5[3] + 0x60) + 0x178);
    *piVar7 = *piVar7 + 1;
    FUN_00148018(puVar5,auVar15._0_8_,auVar15._8_8_,0x33,piVar7,0xfffffffffffffff9,0,3,0,3,0x2703);
    iVar2 = *piVar7;
    iVar3 = iVar2 + -1;
    *piVar7 = iVar3;
    if (iVar3 == 0 || iVar2 < 1) {
      FUN_00141774(puVar5[3],piVar7,0xfffffffffffffff9);
    }
    lVar9 = lVar11 + 0x10;
    ppuVar14 = ppuVar14 + 1;
    *(undefined1 (*) [16])((long)puVar5 + lVar11 + 0x98) = auVar15;
    lVar11 = lVar9;
  } while (lVar9 != 0x80);
  lVar11 = puVar5[8];
  auVar15 = FUN_00140e0c(puVar5,*(undefined8 *)(lVar11 + 0x10),*(undefined8 *)(lVar11 + 0x18),2);
  *(undefined1 (*) [16])(lVar11 + 0x20) = auVar15;
  uVar10 = *(undefined8 *)(puVar5[8] + 0x20);
  if (*(int *)(puVar5[8] + 0x28) != -1) {
    uVar10 = 0;
  }
  uVar10 = FUN_0016b030(puVar5,uVar10,1);
  puVar5[7] = uVar10;
  FUN_0016d598(puVar5,puVar5 + 7,0,0x30,10);
  return puVar5;
}

/* ===== FUN_0013ce74 @ 0013ce74 [libNexusScriptRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0013ce74(long param_1)

{
  undefined8 uVar1;
  int iVar2;
  long lVar3;
  uint uVar4;
  size_t sVar5;
  undefined8 uVar6;
  char *__s;
  char *pcVar7;
  undefined8 uVar8;
  int *piVar9;
  int *piVar10;
  undefined8 uVar11;
  int iVar12;
  ulong uVar13;
  long lVar14;
  undefined *puVar15;
  timeval *__s_00;
  undefined4 uVar16;
  undefined8 *puVar17;
  undefined1 auVar18 [16];
  undefined1 auVar19 [12];
  timeval local_f0;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  char acStack_c8 [8];
  char acStack_c0 [8];
  undefined8 uStack_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  undefined7 uStack_88;
  char local_81;
  char acStack_80 [7];
  char acStack_79 [8];
  long local_68;
  
  lVar3 = tpidr_el0;
  local_68 = *(long *)(lVar3 + 0x28);
  auVar18 = FUN_001412b8(param_1,FUN_001669b8,0,0,0,0,*(undefined8 *)(param_1 + 0x48),
                         *(undefined8 *)(param_1 + 0x50));
  *(undefined1 (*) [16])(param_1 + 0x158) = auVar18;
  auVar18 = FUN_001412b8(param_1,FUN_001669dc,0,0,0,0,*(undefined8 *)(param_1 + 0x48),
                         *(undefined8 *)(param_1 + 0x50));
  uVar8 = auVar18._8_8_;
  piVar9 = auVar18._0_8_;
  FUN_00148018(param_1,*(undefined8 *)(param_1 + 0x48),*(undefined8 *)(param_1 + 0x50),0x51,0,3,
               piVar9,uVar8,*(undefined8 *)(param_1 + 0x158),*(undefined8 *)(param_1 + 0x160),0x1901
              );
  FUN_00148018(param_1,*(undefined8 *)(param_1 + 0x48),*(undefined8 *)(param_1 + 0x50),0x4f,0,3,
               piVar9,uVar8,*(undefined8 *)(param_1 + 0x158),*(undefined8 *)(param_1 + 0x160),0x1901
              );
  local_f0.tv_sec = (__time_t)piVar9;
  if ((0xfffffff4 < auVar18._8_4_) &&
     (iVar12 = *piVar9, iVar2 = iVar12 + -1, *piVar9 = iVar2, iVar2 == 0 || iVar12 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar9,uVar8);
  }
  auVar19 = FUN_00166a6c(param_1);
  local_f0.tv_sec = auVar19._0_8_;
  if ((0xfffffff4 < auVar19._8_4_) &&
     (iVar12 = *(int *)local_f0.tv_sec, iVar2 = iVar12 + -1, *(int *)local_f0.tv_sec = iVar2,
     iVar2 == 0 || iVar12 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_f0.tv_sec);
  }
  auVar18 = FUN_00140e0c(param_1,*(undefined8 *)(*(long *)(param_1 + 0x40) + 0x10),
                         *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x18),1);
  *(undefined1 (*) [16])(param_1 + 0x178) = auVar18;
  for (piVar9 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x188) +
                         (ulong)(0x3c6e0001 >>
                                (ulong)(-*(int *)(*(long *)(param_1 + 0x18) + 0x178) & 0x1f)) * 8);
      (piVar9 != (int *)0x0 &&
      (((piVar9[7] != 0x3c6e0001 || (*(long *)(piVar9 + 0xe) != 0)) || (piVar9[10] != 0))));
      piVar9 = *(int **)(piVar9 + 0xc)) {
  }
  if (piVar9 == (int *)0x0) {
    piVar9 = (int *)FUN_0016b030(param_1,0,2);
    if (piVar9 != (int *)0x0) goto LAB_0013d038;
    auVar18 = ZEXT816(6) << 0x40;
  }
  else {
    *piVar9 = *piVar9 + 1;
LAB_0013d038:
    auVar18 = FUN_00140ed0(param_1,piVar9,1);
  }
  *(undefined1 (*) [16])(param_1 + 0x188) = auVar18;
  uVar8 = *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x10);
  uVar1 = *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x18);
  auVar18 = FUN_001412b8(param_1,FUN_00166d54,"Object",1,4,0,*(undefined8 *)(param_1 + 0x48),
                         *(undefined8 *)(param_1 + 0x50));
  FUN_001605b0(param_1,auVar18._0_8_,auVar18._8_8_,"Object",uVar8,uVar1);
  FUN_0015b874(param_1,auVar18._0_8_,auVar18._8_8_,&PTR_s_create_001ec5d0,0x18);
  FUN_0015b874(param_1,*(undefined8 *)(*(long *)(param_1 + 0x40) + 0x10),
               *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x18),&PTR_s_toString_001eeaa8,0xb);
  FUN_0015b874(param_1,*(undefined8 *)(param_1 + 0x48),*(undefined8 *)(param_1 + 0x50),
               &PTR_DAT_001eec08,7);
  auVar18 = FUN_001412b8(param_1,FUN_00161460,"Function",1,5,0,*(undefined8 *)(param_1 + 0x48),
                         *(undefined8 *)(param_1 + 0x50));
  local_f0.tv_sec = auVar18._0_8_;
  *(undefined1 (*) [16])(param_1 + 0x58) = auVar18;
  if (0xfffffff4 < auVar18._8_4_) {
    *(int *)local_f0.tv_sec = *(int *)local_f0.tv_sec + 1;
  }
  FUN_001605b0(param_1,local_f0.tv_sec,auVar18._8_8_,"Function",*(undefined8 *)(param_1 + 0x48),
               *(undefined8 *)(param_1 + 0x50));
  auVar18 = FUN_001412b8(param_1,FUN_00166dbc,"Error",1,5,0xffffffff,*(undefined8 *)(param_1 + 0x48)
                         ,*(undefined8 *)(param_1 + 0x50));
  local_f0.tv_sec = auVar18._0_8_;
  *(undefined1 (*) [16])(param_1 + 0x118) = auVar18;
  if (0xfffffff4 < auVar18._8_4_) {
    *(int *)local_f0.tv_sec = *(int *)local_f0.tv_sec + 1;
  }
  FUN_001605b0(param_1,local_f0.tv_sec,auVar18._8_8_,"Error",
               *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x30),
               *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x38));
  uVar13 = 0;
  puVar17 = (undefined8 *)(param_1 + 0xa0);
  do {
    puVar15 = (&PTR_s_EvalError_001ec8d0)[uVar13];
    uVar16 = 1;
    if (uVar13 == 7) {
      uVar16 = 2;
    }
    auVar18 = FUN_001412b8(param_1,FUN_00166dbc,puVar15,uVar16,5,uVar13 & 0xffffffff,
                           *(undefined8 *)(param_1 + 0x118),*(undefined8 *)(param_1 + 0x120));
    FUN_001605b0(param_1,auVar18._0_8_,auVar18._8_8_,puVar15,puVar17[-1],*puVar17);
    uVar13 = uVar13 + 1;
    puVar17 = puVar17 + 2;
  } while (uVar13 != 8);
  auVar18 = FUN_00140e0c(param_1,*(undefined8 *)(*(long *)(param_1 + 0x40) + 0x10),
                         *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x18),1);
  *(undefined1 (*) [16])(param_1 + 0x128) = auVar18;
  FUN_0015b874(param_1,auVar18._0_8_,auVar18._8_8_,&PTR_s__Symbol_iterator__001ec910,1);
  FUN_0015b874(param_1,*(undefined8 *)(*(long *)(param_1 + 0x40) + 0x20),
               *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x28),&PTR_DAT_001eece8,0x27);
  uVar8 = *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x20);
  uVar1 = *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x28);
  auVar18 = FUN_001412b8(param_1,FUN_00167380,"Array",1,4,0,*(undefined8 *)(param_1 + 0x48),
                         *(undefined8 *)(param_1 + 0x50));
  piVar9 = auVar18._0_8_;
  FUN_001605b0(param_1,piVar9,auVar18._8_8_,"Array",uVar8,uVar1);
  if (0xfffffff4 < auVar18._8_4_) {
    *piVar9 = *piVar9 + 1;
  }
  *(undefined1 (*) [16])(param_1 + 0x68) = auVar18;
  local_f0.tv_sec = (__time_t)piVar9;
  FUN_0015b874(param_1,piVar9,auVar18._8_8_,&PTR_s_isArray_001ef1c8,4);
  local_f0.tv_usec = ram0x00113609;
  local_f0.tv_sec._0_1_ = s_copyWithin_00113601[0];
  local_f0.tv_sec._1_1_ = s_copyWithin_00113601[1];
  local_f0.tv_sec._2_1_ = s_copyWithin_00113601[2];
  local_f0.tv_sec._3_1_ = s_copyWithin_00113601[3];
  local_f0.tv_sec._4_1_ = s_copyWithin_00113601[4];
  local_f0.tv_sec._5_1_ = s_copyWithin_00113601[5];
  local_f0.tv_sec._6_1_ = s_copyWithin_00113601[6];
  local_f0.tv_sec._7_1_ = s_copyWithin_00113601[7];
  uStack_d8 = _UNK_00113619;
  uStack_e0 = ram0x00113611;
  acStack_c8[0] = s_findLast_00113628[1];
  acStack_c8[1] = s_findLast_00113628[2];
  acStack_c8[2] = s_findLast_00113628[3];
  acStack_c8[3] = s_findLast_00113628[4];
  acStack_c8[4] = s_findLast_00113628[5];
  acStack_c8[5] = s_findLast_00113628[6];
  acStack_c8[6] = s_findLast_00113628[7];
  acStack_c8[7] = s_findLast_00113628[8];
  local_d0 = ram0x00113621;
  uStack_b8 = ram0x00113639;
  acStack_c0[0] = s_findLastIndex_00113631[0];
  acStack_c0[1] = s_findLastIndex_00113631[1];
  acStack_c0[2] = s_findLastIndex_00113631[2];
  acStack_c0[3] = s_findLastIndex_00113631[3];
  acStack_c0[4] = s_findLastIndex_00113631[4];
  acStack_c0[5] = s_findLastIndex_00113631[5];
  acStack_c0[6] = s_findLastIndex_00113631[6];
  acStack_c0[7] = s_findLastIndex_00113631[7];
  acStack_79[0] = s_values_00113678[0];
  acStack_79[1] = s_values_00113678[1];
  acStack_79[2] = s_values_00113678[2];
  acStack_79[3] = s_values_00113678[3];
  acStack_79[4] = s_values_00113678[4];
  acStack_79[5] = s_values_00113678[5];
  acStack_79[6] = s_values_00113678[6];
  acStack_79[7] = s_values_00113678[7];
  acStack_80[0] = s_toSpliced_0011366e[3];
  acStack_80[1] = s_toSpliced_0011366e[4];
  acStack_80[2] = s_toSpliced_0011366e[5];
  acStack_80[3] = s_toSpliced_0011366e[6];
  acStack_80[4] = s_toSpliced_0011366e[7];
  acStack_80[5] = s_toSpliced_0011366e[8];
  acStack_80[6] = s_toSpliced_0011366e[9];
  uStack_98 = _UNK_00113659;
  local_a0 = ram0x00113651;
  uStack_88 = ram0x00113669;
  local_81 = s_toSpliced_0011366e[2];
  uStack_90 = ram0x00113661;
  uStack_a8 = ram0x00113649;
  local_b0 = _DAT_00113641;
  for (piVar9 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x188) +
                         (ulong)(0x3c6e0001 >>
                                (ulong)(-*(int *)(*(long *)(param_1 + 0x18) + 0x178) & 0x1f)) * 8);
      (piVar9 != (int *)0x0 &&
      (((piVar9[7] != 0x3c6e0001 || (*(long *)(piVar9 + 0xe) != 0)) || (piVar9[10] != 0))));
      piVar9 = *(int **)(piVar9 + 0xc)) {
  }
  if (piVar9 == (int *)0x0) {
    piVar9 = (int *)FUN_0016b030(param_1,0,2);
    if (piVar9 != (int *)0x0) goto LAB_0013d3a4;
    auVar18 = ZEXT816(6) << 0x40;
  }
  else {
    *piVar9 = *piVar9 + 1;
LAB_0013d3a4:
    auVar18 = FUN_00140ed0(param_1,piVar9,1);
  }
  uVar8 = auVar18._8_8_;
  piVar9 = auVar18._0_8_;
  if ((char)local_f0.tv_sec != '\0') {
    __s_00 = &local_f0;
    do {
      sVar5 = strlen((char *)__s_00);
      uVar4 = FUN_0013f2ec(param_1,__s_00,sVar5);
      FUN_00148018(param_1,piVar9,uVar8,(ulong)uVar4,1,1,0,3,0,3,0x2707);
      if ((0xe3 < (int)uVar4) &&
         (piVar10 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + (ulong)uVar4 * 8),
         iVar12 = *piVar10, iVar2 = iVar12 + -1, *piVar10 = iVar2, iVar2 == 0 || iVar12 < 1)) {
        FUN_001418dc();
      }
      sVar5 = strlen((char *)__s_00);
      __s_00 = (timeval *)((long)&__s_00->tv_sec + sVar5 + 1);
    } while ((char)__s_00->tv_sec != '\0');
  }
  FUN_00148018(param_1,*(undefined8 *)(*(long *)(param_1 + 0x40) + 0x20),
               *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x28),0xe1,piVar9,uVar8,0,3,0,3,0x2701);
  if ((0xfffffff4 < auVar18._8_4_) &&
     (iVar12 = *piVar9, iVar2 = iVar12 + -1, *piVar9 = iVar2, iVar2 == 0 || iVar12 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar9,uVar8);
  }
  uVar8 = *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x20);
  uVar1 = *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x28);
  auVar18 = FUN_001446d4(param_1,uVar8,uVar1,0x6d,uVar8,uVar1,0);
  *(undefined1 (*) [16])(param_1 + 0x148) = auVar18;
  lVar14 = *(long *)(param_1 + 0x40);
  auVar18 = FUN_001411c8(param_1,*(undefined8 *)(param_1 + 0x128),*(undefined8 *)(param_1 + 0x130));
  *(undefined1 (*) [16])(lVar14 + 0x2c0) = auVar18;
  FUN_0015b874(param_1,*(undefined8 *)(*(long *)(param_1 + 0x40) + 0x2c0),
               *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x2c8),&PTR_DAT_001ef248,2);
  FUN_0015b874(param_1,*(undefined8 *)(param_1 + 0x178),*(undefined8 *)(param_1 + 0x180),
               &PTR_s_parseInt_001ef288,0xd);
  lVar14 = *(long *)(param_1 + 0x40);
  auVar18 = FUN_00140e0c(param_1,*(undefined8 *)(lVar14 + 0x10),*(undefined8 *)(lVar14 + 0x18),4);
  *(undefined1 (*) [16])(lVar14 + 0x40) = auVar18;
  uVar13 = *(ulong *)(*(long *)(param_1 + 0x40) + 0x48);
  lVar14 = *(long *)(*(long *)(param_1 + 0x40) + 0x40);
  iVar12 = (int)uVar13;
  if (iVar12 != 6) {
    if (iVar12 == -1) {
      if ((*(ushort *)(lVar14 + 6) < 0x25) &&
         ((1L << ((ulong)*(ushort *)(lVar14 + 6) & 0x3f) & 0x16000004f0U) != 0)) {
        local_f0.tv_sec = *(undefined8 *)(lVar14 + 0x30);
        if ((0xfffffff4 < (uint)*(undefined8 *)(lVar14 + 0x38)) &&
           (iVar12 = *(int *)local_f0.tv_sec, iVar2 = iVar12 + -1, *(int *)local_f0.tv_sec = iVar2,
           iVar2 == 0 || iVar12 < 1)) {
          FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_f0.tv_sec);
        }
        *(undefined8 *)(lVar14 + 0x30) = 0;
        *(undefined8 *)(lVar14 + 0x38) = 0;
      }
      else if ((uVar13 & 0xffffffff) != 6) goto LAB_0013d61c;
    }
    else {
LAB_0013d61c:
      FUN_00143570(param_1,"invalid object type");
    }
  }
  FUN_0015b874(param_1,*(undefined8 *)(*(long *)(param_1 + 0x40) + 0x40),
               *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x48),&PTR_s_toExponential_001ec930,6);
  uVar8 = *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x40);
  uVar1 = *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x48);
  auVar18 = FUN_001412b8(param_1,FUN_00167658,"Number",1,4,0,*(undefined8 *)(param_1 + 0x48),
                         *(undefined8 *)(param_1 + 0x50));
  FUN_001605b0(param_1,auVar18._0_8_,auVar18._8_8_,"Number",uVar8,uVar1);
  FUN_0015b874(param_1,auVar18._0_8_,auVar18._8_8_,&PTR_s_parseInt_001ef428,0xe);
  lVar14 = *(long *)(param_1 + 0x40);
  auVar18 = FUN_00140e0c(param_1,*(undefined8 *)(lVar14 + 0x10),*(undefined8 *)(lVar14 + 0x18),6);
  *(undefined1 (*) [16])(lVar14 + 0x60) = auVar18;
  uVar13 = *(ulong *)(*(long *)(param_1 + 0x40) + 0x68);
  lVar14 = *(long *)(*(long *)(param_1 + 0x40) + 0x60);
  iVar12 = (int)uVar13;
  if (iVar12 == 6) goto LAB_0013d778;
  if (iVar12 == -1) {
    if ((*(ushort *)(lVar14 + 6) < 0x25) &&
       ((1L << ((ulong)*(ushort *)(lVar14 + 6) & 0x3f) & 0x16000004f0U) != 0)) {
      local_f0.tv_sec = *(undefined8 *)(lVar14 + 0x30);
      if ((0xfffffff4 < (uint)*(undefined8 *)(lVar14 + 0x38)) &&
         (iVar12 = *(int *)local_f0.tv_sec, iVar2 = iVar12 + -1, *(int *)local_f0.tv_sec = iVar2,
         iVar2 == 0 || iVar12 < 1)) {
        FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_f0.tv_sec);
      }
      *(undefined8 *)(lVar14 + 0x30) = 0;
      *(undefined8 *)(lVar14 + 0x38) = 1;
      goto LAB_0013d778;
    }
    if ((uVar13 & 0xffffffff) == 6) goto LAB_0013d778;
  }
  FUN_00143570(param_1,"invalid object type");
LAB_0013d778:
  FUN_0015b874(param_1,*(undefined8 *)(*(long *)(param_1 + 0x40) + 0x60),
               *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x68),&PTR_s_toString_001ec9f0,2);
  uVar8 = *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x60);
  uVar1 = *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x68);
  auVar18 = FUN_001412b8(param_1,FUN_00167858,"Boolean",1,4,0,*(undefined8 *)(param_1 + 0x48),
                         *(undefined8 *)(param_1 + 0x50));
  FUN_001605b0(param_1,auVar18._0_8_,auVar18._8_8_,"Boolean",uVar8,uVar1);
  lVar14 = *(long *)(param_1 + 0x40);
  auVar18 = FUN_00140e0c(param_1,*(undefined8 *)(lVar14 + 0x10),*(undefined8 *)(lVar14 + 0x18),5);
  *(undefined1 (*) [16])(lVar14 + 0x50) = auVar18;
  if (*(uint *)(*(long *)(param_1 + 0x18) + 0x50) < 0x30) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0xc94,"JSValue __JS_AtomToValue(JSContext *, JSAtom, BOOL)","atom < rt->atom_size");
  }
  lVar14 = *(long *)(param_1 + 0x40);
  piVar9 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + 0x178);
  *piVar9 = *piVar9 + 1;
  FUN_00167548(param_1,*(undefined8 *)(lVar14 + 0x50),*(undefined8 *)(lVar14 + 0x58),piVar9,
               0xfffffffffffffff9);
  uVar8 = *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x50);
  uVar1 = *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x58);
  auVar18 = FUN_001412b8(param_1,FUN_00167914,"String",1,4,0,*(undefined8 *)(param_1 + 0x48),
                         *(undefined8 *)(param_1 + 0x50));
  FUN_001605b0(param_1,auVar18._0_8_,auVar18._8_8_,"String",uVar8,uVar1);
  FUN_0015b874(param_1,auVar18._0_8_,auVar18._8_8_,&PTR_s_fromCharCode_001eca30,3);
  FUN_0015b874(param_1,*(undefined8 *)(*(long *)(param_1 + 0x40) + 0x50),
               *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x58),&PTR_s_length_001ef5e8,0x34);
  lVar14 = *(long *)(param_1 + 0x40);
  auVar18 = FUN_001411c8(param_1,*(undefined8 *)(param_1 + 0x128),*(undefined8 *)(param_1 + 0x130));
  *(undefined1 (*) [16])(lVar14 + 0x2d0) = auVar18;
  FUN_0015b874(param_1,*(undefined8 *)(*(long *)(param_1 + 0x40) + 0x2d0),
               *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x2d8),&PTR_DAT_001efd38,2);
  gettimeofday(&local_f0,(__timezone_ptr_t)0x0);
  lVar14 = local_f0.tv_usec + local_f0.tv_sec * 1000000;
  if (lVar14 == 0) {
    lVar14 = 1;
  }
  *(long *)(param_1 + 0x1b0) = lVar14;
  FUN_0015b874(param_1,*(undefined8 *)(param_1 + 0x178),*(undefined8 *)(param_1 + 0x180),
               &PTR_DAT_001efd78,1);
  FUN_0015b874(param_1,*(undefined8 *)(param_1 + 0x178),*(undefined8 *)(param_1 + 0x180),
               &PTR_s_Reflect_001f0318,1);
  lVar14 = *(long *)(param_1 + 0x40);
  auVar18 = FUN_00140e0c(param_1,*(undefined8 *)(lVar14 + 0x10),*(undefined8 *)(lVar14 + 0x18),1);
  *(undefined1 (*) [16])(lVar14 + 0x70) = auVar18;
  FUN_0015b874(param_1,*(undefined8 *)(*(long *)(param_1 + 0x40) + 0x70),
               *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x78),&PTR_s_toString_001f04f8,5);
  uVar8 = *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x70);
  uVar1 = *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x78);
  auVar18 = FUN_001412b8(param_1,FUN_00167b2c,"Symbol",0,4,0,*(undefined8 *)(param_1 + 0x48),
                         *(undefined8 *)(param_1 + 0x50));
  uVar11 = auVar18._8_8_;
  uVar6 = auVar18._0_8_;
  FUN_001605b0(param_1,uVar6,uVar11,"Symbol",uVar8,uVar1);
  FUN_0015b874(param_1,uVar6,uVar11,&PTR_DAT_001eca90,2);
  iVar12 = 0xd6;
  do {
    __s = (char *)FUN_00142bd0(*(undefined8 *)(param_1 + 0x18),&local_f0,iVar12);
    pcVar7 = strchr(__s,0x2e);
    if (pcVar7 != (char *)0x0) {
      __s = pcVar7 + 1;
    }
    auVar18 = FUN_00140044(param_1,iVar12,0);
    FUN_0014aba4(param_1,uVar6,uVar11,__s,auVar18._0_8_,auVar18._8_8_,0);
    iVar12 = iVar12 + 1;
  } while (iVar12 != 0xe4);
  lVar14 = *(long *)(param_1 + 0x40);
  auVar18 = FUN_001411c8(param_1,*(undefined8 *)(param_1 + 0x128),*(undefined8 *)(param_1 + 0x130));
  *(undefined1 (*) [16])(lVar14 + 0x2f0) = auVar18;
  FUN_0015b874(param_1,*(undefined8 *)(*(long *)(param_1 + 0x40) + 0x2f0),
               *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x2f8),&PTR_DAT_001f0598,4);
  lVar14 = *(long *)(param_1 + 0x40);
  auVar18 = FUN_001411c8(param_1,*(undefined8 *)(param_1 + 0x48),*(undefined8 *)(param_1 + 0x50));
  *(undefined1 (*) [16])(lVar14 + 0x100) = auVar18;
  auVar18 = FUN_001412b8(param_1,FUN_00161460,"GeneratorFunction",1,5,1,
                         *(undefined8 *)(param_1 + 0x48),*(undefined8 *)(param_1 + 0x50));
  piVar9 = auVar18._0_8_;
  FUN_0015b874(param_1,*(undefined8 *)(*(long *)(param_1 + 0x40) + 0x100),
               *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x108),
               &PTR_s__Symbol_toStringTag__001f0618,1);
  lVar14 = *(long *)(param_1 + 0x40);
  FUN_0015bf48(param_1,*(undefined8 *)(lVar14 + 0x100),*(undefined8 *)(lVar14 + 0x108),
               *(undefined8 *)(lVar14 + 0x2f0),*(undefined8 *)(lVar14 + 0x2f8),1,1);
  FUN_0015bf48(param_1,piVar9,auVar18._8_8_,*(undefined8 *)(*(long *)(param_1 + 0x40) + 0x100),
               *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x108),0,1);
  local_f0.tv_sec = (__time_t)piVar9;
  if ((0xfffffff4 < auVar18._8_4_) &&
     (iVar12 = *piVar9, iVar2 = iVar12 + -1, *piVar9 = iVar2, iVar2 == 0 || iVar12 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar9,auVar18._8_8_);
  }
  auVar18 = FUN_001412b8(param_1,FUN_00167c28,&DAT_0010e2fc,1,0,0,*(undefined8 *)(param_1 + 0x48),
                         *(undefined8 *)(param_1 + 0x50));
  uVar8 = auVar18._8_8_;
  piVar9 = auVar18._0_8_;
  *(undefined1 (*) [16])(param_1 + 0x168) = auVar18;
  local_f0.tv_sec = (__time_t)piVar9;
  if (auVar18._8_4_ < 0xfffffff5) {
    FUN_00148018(param_1,*(undefined8 *)(param_1 + 0x178),*(undefined8 *)(param_1 + 0x180),0x3c,
                 piVar9,uVar8,0,3,0,3,0x2703);
  }
  else {
    *piVar9 = *piVar9 + 1;
    FUN_00148018(param_1,*(undefined8 *)(param_1 + 0x178),*(undefined8 *)(param_1 + 0x180),0x3c,
                 piVar9,uVar8,0,3,0,3,0x2703);
    iVar12 = *piVar9;
    iVar2 = iVar12 + -1;
    *piVar9 = iVar2;
    if (iVar2 == 0 || iVar12 < 1) {
      FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar9,uVar8);
    }
  }
  piVar9 = *(int **)(param_1 + 0x178);
  uVar8 = *(undefined8 *)(param_1 + 0x180);
  local_f0.tv_sec = (__time_t)piVar9;
  if ((uint)uVar8 < 0xfffffff5) {
    FUN_00148018(param_1,*(undefined8 *)(param_1 + 0x178),*(undefined8 *)(param_1 + 0x180),0x8d,
                 piVar9,uVar8,0,3,0,3,0x2703);
  }
  else {
    *piVar9 = *piVar9 + 1;
    FUN_00148018(param_1,*(undefined8 *)(param_1 + 0x178),*(undefined8 *)(param_1 + 0x180),0x8d,
                 piVar9,uVar8,0,3,0,3,0x2703);
    iVar12 = *piVar9;
    iVar2 = iVar12 + -1;
    *piVar9 = iVar2;
    if (iVar2 == 0 || iVar12 < 1) {
      FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar9,uVar8);
    }
  }
  if (*(long *)(lVar3 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_0013ecb0 @ 0013ecb0 [libNexusScriptRuntime69252.so] ===== */

void FUN_0013ecb0(long param_1,uint param_2,undefined8 param_3,undefined8 param_4)

{
  undefined8 *puVar1;
  int iVar2;
  int iVar3;
  undefined8 uVar4;
  int *piVar5;
  
  if (*(uint *)(*(long *)(param_1 + 0x18) + 0x6c) <= param_2) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0x900,"void JS_SetClassProto(JSContext *, JSClassID, JSValue)",
              "class_id < ctx->rt->class_count");
  }
  puVar1 = (undefined8 *)(*(long *)(param_1 + 0x40) + (ulong)param_2 * 0x10);
  uVar4 = puVar1[1];
  piVar5 = (int *)*puVar1;
  *puVar1 = param_3;
  puVar1[1] = param_4;
  if (0xfffffff4 < (uint)uVar4) {
    iVar2 = *piVar5;
    iVar3 = iVar2 + -1;
    *piVar5 = iVar3;
    if (iVar3 == 0 || iVar2 < 1) {
      FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar5);
      return;
    }
  }
  return;
}

/* ===== FUN_0013ed90 @ 0013ed90 [libNexusScriptRuntime69252.so] ===== */

void FUN_0013ed90(long param_1,uint param_2)

{
  undefined8 *puVar1;
  int *piVar2;
  
  if (param_2 < *(uint *)(*(long *)(param_1 + 0x18) + 0x6c)) {
    puVar1 = (undefined8 *)(*(long *)(param_1 + 0x40) + (ulong)param_2 * 0x10);
    piVar2 = (int *)*puVar1;
    if (0xfffffff4 < (uint)puVar1[1]) {
      *piVar2 = *piVar2 + 1;
    }
    return;
  }
                    /* WARNING: Subroutine does not return */
  __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
            ,0x906,"JSValue JS_GetClassProto(JSContext *, JSClassID)",
            "class_id < ctx->rt->class_count");
}

/* ===== FUN_0013ee10 @ 0013ee10 [libNexusScriptRuntime69252.so] ===== */

void FUN_0013ee10(int *param_1)

{
  long *plVar1;
  int iVar2;
  int iVar3;
  undefined8 uVar4;
  int *piVar5;
  int *piVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  
  iVar2 = *param_1;
  lVar7 = *(long *)(param_1 + 6);
  iVar3 = iVar2 + -1;
  *param_1 = iVar3;
  if (iVar3 != 0 && 0 < iVar2) {
    return;
  }
  if (iVar3 != 0) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0x951,"void JS_FreeContext(JSContext *)","ctx->header.ref_count == 0");
  }
  piVar6 = *(int **)(param_1 + 0x78);
  while (piVar6 != param_1 + 0x76) {
    piVar5 = piVar6 + -2;
    piVar6 = *(int **)(piVar6 + 2);
    FUN_0016a6e8(param_1,piVar5);
  }
  piVar6 = *(int **)(param_1 + 0x5e);
  if ((0xfffffff4 < (uint)*(undefined8 *)(param_1 + 0x60)) &&
     (iVar2 = *piVar6, iVar3 = iVar2 + -1, *piVar6 = iVar3, iVar3 == 0 || iVar2 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 6),piVar6);
  }
  piVar6 = *(int **)(param_1 + 0x62);
  if ((0xfffffff4 < (uint)*(undefined8 *)(param_1 + 100)) &&
     (iVar2 = *piVar6, iVar3 = iVar2 + -1, *piVar6 = iVar3, iVar3 == 0 || iVar2 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 6),piVar6);
  }
  piVar6 = *(int **)(param_1 + 0x56);
  if ((0xfffffff4 < (uint)*(undefined8 *)(param_1 + 0x58)) &&
     (iVar2 = *piVar6, iVar3 = iVar2 + -1, *piVar6 = iVar3, iVar3 == 0 || iVar2 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 6),piVar6);
  }
  piVar6 = *(int **)(param_1 + 0x5a);
  if ((0xfffffff4 < (uint)*(undefined8 *)(param_1 + 0x5c)) &&
     (iVar2 = *piVar6, iVar3 = iVar2 + -1, *piVar6 = iVar3, iVar3 == 0 || iVar2 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 6),piVar6);
  }
  piVar6 = *(int **)(param_1 + 0x52);
  if ((0xfffffff4 < (uint)*(undefined8 *)(param_1 + 0x54)) &&
     (iVar2 = *piVar6, iVar3 = iVar2 + -1, *piVar6 = iVar3, iVar3 == 0 || iVar2 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 6),piVar6);
  }
  lVar8 = 0;
  do {
    piVar6 = *(int **)((long)param_1 + lVar8 + 0x98);
    if ((0xfffffff4 < (uint)*(undefined8 *)((long)param_1 + lVar8 + 0xa0)) &&
       (iVar2 = *piVar6, iVar3 = iVar2 + -1, *piVar6 = iVar3, iVar3 == 0 || iVar2 < 1)) {
      FUN_00141774(*(undefined8 *)(param_1 + 6),piVar6);
    }
    lVar8 = lVar8 + 0x10;
  } while (lVar8 != 0x80);
  piVar6 = *(int **)(param_1 + 0x46);
  if ((0xfffffff4 < (uint)*(undefined8 *)(param_1 + 0x48)) &&
     (iVar2 = *piVar6, iVar3 = iVar2 + -1, *piVar6 = iVar3, iVar3 == 0 || iVar2 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 6),piVar6);
  }
  if (0 < *(int *)(lVar7 + 0x6c)) {
    lVar9 = 0;
    lVar8 = 0;
    do {
      piVar6 = *(int **)(*(long *)(param_1 + 0x10) + lVar9);
      if ((0xfffffff4 < (uint)((undefined8 *)(*(long *)(param_1 + 0x10) + lVar9))[1]) &&
         (iVar2 = *piVar6, iVar3 = iVar2 + -1, *piVar6 = iVar3, iVar3 == 0 || iVar2 < 1)) {
        FUN_00141774(*(undefined8 *)(param_1 + 6),piVar6);
      }
      lVar8 = lVar8 + 1;
      lVar9 = lVar9 + 0x10;
    } while (lVar8 < *(int *)(lVar7 + 0x6c));
  }
  (**(code **)(lVar7 + 8))(lVar7 + 0x20,*(undefined8 *)(param_1 + 0x10));
  piVar6 = *(int **)(param_1 + 0x4a);
  if ((0xfffffff4 < (uint)*(undefined8 *)(param_1 + 0x4c)) &&
     (iVar2 = *piVar6, iVar3 = iVar2 + -1, *piVar6 = iVar3, iVar3 == 0 || iVar2 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 6),piVar6);
  }
  piVar6 = *(int **)(param_1 + 0x4e);
  if ((0xfffffff4 < (uint)*(undefined8 *)(param_1 + 0x50)) &&
     (iVar2 = *piVar6, iVar3 = iVar2 + -1, *piVar6 = iVar3, iVar3 == 0 || iVar2 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 6),piVar6);
  }
  piVar6 = *(int **)(param_1 + 0x22);
  if ((0xfffffff4 < (uint)*(undefined8 *)(param_1 + 0x24)) &&
     (iVar2 = *piVar6, iVar3 = iVar2 + -1, *piVar6 = iVar3, iVar3 == 0 || iVar2 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 6),piVar6);
  }
  piVar6 = *(int **)(param_1 + 0x1a);
  if ((0xfffffff4 < (uint)*(undefined8 *)(param_1 + 0x1c)) &&
     (iVar2 = *piVar6, iVar3 = iVar2 + -1, *piVar6 = iVar3, iVar3 == 0 || iVar2 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 6),piVar6);
  }
  piVar6 = *(int **)(param_1 + 0x1e);
  if ((0xfffffff4 < (uint)*(undefined8 *)(param_1 + 0x20)) &&
     (iVar2 = *piVar6, iVar3 = iVar2 + -1, *piVar6 = iVar3, iVar3 == 0 || iVar2 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 6),piVar6);
  }
  piVar6 = *(int **)(param_1 + 0x16);
  if ((0xfffffff4 < (uint)*(undefined8 *)(param_1 + 0x18)) &&
     (iVar2 = *piVar6, iVar3 = iVar2 + -1, *piVar6 = iVar3, iVar3 == 0 || iVar2 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 6),piVar6);
  }
  piVar6 = *(int **)(param_1 + 0x12);
  if ((0xfffffff4 < (uint)*(undefined8 *)(param_1 + 0x14)) &&
     (iVar2 = *piVar6, iVar3 = iVar2 + -1, *piVar6 = iVar3, iVar3 == 0 || iVar2 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 6),piVar6);
  }
  piVar6 = *(int **)(param_1 + 0xe);
  if (piVar6 != (int *)0x0) {
    iVar2 = *piVar6;
    uVar4 = *(undefined8 *)(param_1 + 6);
    iVar3 = iVar2 + -1;
    *piVar6 = iVar3;
    if (iVar3 == 0 || iVar2 < 1) {
      FUN_0016aef4(uVar4);
    }
  }
  lVar7 = *(long *)(param_1 + 8);
  plVar1 = *(long **)(param_1 + 10);
  *(long **)(lVar7 + 8) = plVar1;
  *plVar1 = lVar7;
  lVar7 = *(long *)(param_1 + 2);
  plVar1 = *(long **)(param_1 + 4);
  param_1[8] = 0;
  param_1[9] = 0;
  param_1[10] = 0;
  param_1[0xb] = 0;
  *(long **)(lVar7 + 8) = plVar1;
  *plVar1 = lVar7;
  param_1[2] = 0;
  param_1[3] = 0;
  param_1[4] = 0;
  param_1[5] = 0;
                    /* WARNING: Could not recover jumptable at 0x0013f24c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*(long *)(param_1 + 6) + 8))(*(long *)(param_1 + 6) + 0x20,param_1);
  return;
}

/* ===== FUN_0013f444 @ 0013f444 [libNexusScriptRuntime69252.so] ===== */

ulong FUN_0013f444(int *param_1,byte *param_2,ulong param_3)

{
  byte *pbVar1;
  byte bVar2;
  long lVar3;
  int iVar4;
  uint uVar5;
  undefined8 *puVar6;
  int *piVar7;
  long lVar8;
  byte *pbVar9;
  ushort *puVar10;
  byte *pbVar11;
  byte *pbVar12;
  int iVar13;
  ulong uVar14;
  undefined1 auVar15 [16];
  undefined8 local_80;
  undefined4 *local_78;
  int local_70;
  int local_6c;
  undefined8 local_68;
  byte *local_60;
  long local_58;
  
  lVar3 = tpidr_el0;
  local_58 = *(long *)(lVar3 + 0x28);
  pbVar1 = param_2 + param_3;
  pbVar12 = param_2;
  if (0 < (long)param_3) {
    do {
      if ((char)*pbVar12 < '\0') break;
      pbVar12 = pbVar12 + 1;
    } while (pbVar12 < pbVar1);
  }
  uVar14 = (long)pbVar12 - (long)param_2;
  if (uVar14 >> 0x1e != 0) {
    FUN_001405ec(param_1,"string too long");
    uVar14 = 0;
    auVar15 = ZEXT816(6) << 0x40;
    goto LAB_0013f7d4;
  }
  iVar4 = (int)param_3;
  if (pbVar12 != pbVar1) {
    local_70 = 0;
    local_68 = 0;
    local_80 = param_1;
    local_6c = iVar4;
    local_78 = (undefined4 *)
               (*(code *)**(undefined8 **)(param_1 + 6))
                         (*(undefined8 **)(param_1 + 6) + 4,(long)iVar4 + 0x11);
    if (local_78 == (undefined4 *)0x0) {
      FUN_00138d58(param_1);
      local_78 = (undefined4 *)0x0;
    }
    else {
      local_78[3] = 0;
      *local_78 = 1;
      *(ulong *)(local_78 + 1) = param_3 & 0x7fffffff;
      if (local_78 != (undefined4 *)0x0) {
        iVar13 = (int)uVar14;
        if ((iVar13 <= iVar4) || (iVar4 = FUN_0016abb8(&local_80,uVar14 & 0xffffffff,0), iVar4 == 0)
           ) {
          if ((int)local_68 == 0) {
            memcpy((void *)((long)local_78 + (long)local_70 + 0x10),param_2,(long)iVar13);
          }
          else if (0 < iVar13) {
            uVar14 = uVar14 & 0xffffffff;
            puVar10 = (ushort *)((long)local_78 + (long)local_70 * 2 + 0x10);
            pbVar11 = param_2;
            do {
              uVar14 = uVar14 - 1;
              *puVar10 = (ushort)*pbVar11;
              puVar10 = puVar10 + 1;
              pbVar11 = pbVar11 + 1;
            } while (uVar14 != 0);
          }
          local_70 = local_70 + iVar13;
        }
        if (pbVar12 < pbVar1) {
LAB_0013f63c:
          bVar2 = *pbVar12;
          if ((char)bVar2 < '\0') {
            uVar5 = FUN_001d2c34(pbVar12,(int)pbVar1 - (int)pbVar12,&local_60);
            pbVar11 = local_60;
            if ((uVar5 & 0xffff0000) != 0) {
              if (uVar5 < 0x110000) {
                lVar8 = (long)local_70;
                if ((local_70 < local_6c) && ((int)local_68 != 0)) {
                  local_70 = local_70 + 1;
                  *(short *)((long)local_78 + lVar8 * 2 + 0x10) =
                       (short)(uVar5 - 0x10000 >> 10) + -0x2800;
                }
                else {
                  FUN_0016ae44(&local_80);
                }
                uVar5 = uVar5 & 0x3ff | 0xdc00;
                goto LAB_0013f754;
              }
              pbVar11 = pbVar12;
              if (pbVar12 < pbVar1) {
                lVar8 = (long)(param_2 + param_3) - (long)pbVar12;
                pbVar9 = pbVar12;
                do {
                  pbVar9 = pbVar9 + 1;
                  if ((char)*pbVar12 + 0x40 < 0 == SCARRY4((int)(char)*pbVar12,0x40))
                  goto LAB_0013f720;
                  pbVar12 = pbVar12 + 1;
                  lVar8 = lVar8 + -1;
                } while (lVar8 != 0);
                uVar5 = 0xfffd;
                pbVar11 = param_2 + param_3;
                goto LAB_0013f754;
              }
              goto LAB_0013f73c;
            }
            goto LAB_0013f754;
          }
          pbVar11 = pbVar12 + 1;
          if ((local_70 < local_6c) ||
             (iVar4 = FUN_0016abb8(&local_80,local_70 + 1,bVar2), iVar4 == 0)) {
            if ((int)local_68 == 0) {
              lVar8 = (long)local_70;
              local_70 = local_70 + 1;
              *(byte *)((long)local_78 + lVar8 + 0x10) = bVar2;
            }
            else {
              lVar8 = (long)local_70;
              local_70 = local_70 + 1;
              *(ushort *)((long)local_78 + lVar8 * 2 + 0x10) = (ushort)bVar2;
            }
          }
          goto LAB_0013f634;
        }
LAB_0013f7c8:
        auVar15 = FUN_00140918(&local_80);
        goto LAB_0013f7d0;
      }
    }
    local_6c = 0;
    local_68 = CONCAT44(0xffffffff,(int)local_68);
    (**(code **)(*(long *)(param_1 + 6) + 8))(*(long *)(param_1 + 6) + 0x20);
    uVar14 = 0;
    auVar15 = ZEXT816(6) << 0x40;
    local_78 = (undefined4 *)0x0;
    goto LAB_0013f7d4;
  }
  puVar6 = *(undefined8 **)(param_1 + 6);
  if (iVar4 < 1) {
    if (*(uint *)(puVar6 + 10) < 0x30) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0xc94,"JSValue __JS_AtomToValue(JSContext *, JSAtom, BOOL)","atom < rt->atom_size")
      ;
    }
    local_80 = *(int **)(puVar6[0xc] + 0x178);
    auVar15._8_8_ = 0xfffffffffffffff9;
    auVar15._0_8_ = local_80;
    *local_80 = *local_80 + 1;
  }
  else {
    piVar7 = (int *)(*(code *)*puVar6)(puVar6 + 4,(long)iVar4 + 0x11);
    if (piVar7 == (int *)0x0) {
      FUN_00138d58(param_1);
    }
    else {
      piVar7[3] = 0;
      *piVar7 = 1;
      *(ulong *)(piVar7 + 1) = param_3 & 0x7fffffff;
      if (piVar7 != (int *)0x0) {
        memcpy(piVar7 + 4,param_2,param_3 & 0xffffffff);
        auVar15._8_8_ = 0xfffffffffffffff9;
        auVar15._0_8_ = piVar7;
        *(undefined1 *)((long)(piVar7 + 4) + (param_3 & 0xffffffff)) = 0;
        local_80 = piVar7;
        goto LAB_0013f7d0;
      }
    }
    local_80 = (int *)((ulong)local_80._4_4_ << 0x20);
    auVar15._8_8_ = 6;
    auVar15._0_8_ = local_80;
  }
LAB_0013f7d0:
  uVar14 = auVar15._0_8_ & 0xffffffff00000000;
LAB_0013f7d4:
  if (*(long *)(lVar3 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(auVar15._0_8_,auVar15._8_8_);
  }
  return uVar14 | auVar15._0_8_ & 0xffffffff;
  while (pbVar9 = pbVar11 + 1, (char)*pbVar11 + 0x40 < 0 != SCARRY4((int)(char)*pbVar11,0x40)) {
LAB_0013f720:
    pbVar11 = pbVar9;
    if (pbVar1 <= pbVar11) break;
  }
LAB_0013f73c:
  uVar5 = 0xfffd;
LAB_0013f754:
  lVar8 = (long)local_70;
  if (local_70 < local_6c) {
    if ((int)local_68 != 0) {
      local_70 = local_70 + 1;
      *(short *)((long)local_78 + lVar8 * 2 + 0x10) = (short)uVar5;
      goto LAB_0013f634;
    }
    if (uVar5 < 0x100) {
      local_70 = local_70 + 1;
      *(char *)((long)local_78 + lVar8 + 0x10) = (char)uVar5;
      goto LAB_0013f634;
    }
  }
  FUN_0016ae44(&local_80);
LAB_0013f634:
  pbVar12 = pbVar11;
  if (pbVar1 <= pbVar11) goto LAB_0013f7c8;
  goto LAB_0013f63c;
}

/* ===== FUN_0013fad4 @ 0013fad4 [libNexusScriptRuntime69252.so] ===== */

ulong FUN_0013fad4(undefined8 *param_1,int *param_2,ulong param_3)

{
  undefined8 *puVar1;
  ulong uVar2;
  ushort *puVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  undefined8 *puVar8;
  undefined8 *puVar9;
  ushort *__s1;
  uint uVar10;
  uint uVar11;
  ulong uVar12;
  int *piVar13;
  ushort *puVar14;
  ulong uVar15;
  ulong uVar16;
  int *piVar17;
  ulong uVar18;
  long lVar19;
  uint uVar20;
  uint uVar21;
  ulong uVar22;
  
  uVar16 = param_3 & 0xffffffff;
  uVar10 = (uint)param_3;
  if ((int)uVar10 < 3) {
    uVar22 = *(ulong *)(param_2 + 1);
    uVar20 = (uint)(uVar22 >> 0x20);
    if (uVar20 >> 0x1e == uVar10) {
      if (uVar22 >> 0x3e < 3) {
        uVar10 = *(uint *)(param_1[0xb] +
                          (ulong)(uVar20 & *(int *)(param_1 + 9) - 1U & 0x3fffffff) * 4);
        while( true ) {
          piVar17 = (int *)(ulong)uVar10;
          piVar13 = *(int **)(param_1[0xc] + (long)piVar17 * 8);
          if (piVar13 == param_2) break;
          if (piVar17 == (int *)0x0) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0xaf4,"JSAtom js_get_atom_index(JSRuntime *, JSAtomStruct *)","i != 0");
          }
          uVar10 = piVar13[3];
        }
      }
      else {
        piVar17 = (int *)(ulong)(uint)param_2[3];
      }
      if ((int)piVar17 < 0xe4) {
        *param_2 = *param_2 + -1;
      }
      goto LAB_0013fff8;
    }
    puVar3 = (ushort *)(param_2 + 4);
    uVar12 = uVar22 & 0x7fffffff;
    uVar21 = (uint)uVar22;
    uVar18 = uVar16;
    uVar20 = uVar10;
    if ((int)uVar21 < 0) {
      for (; uVar12 != 0; uVar12 = uVar12 - 1) {
        uVar20 = (uint)*puVar3 + (int)uVar18 * 0x107;
        uVar18 = (ulong)uVar20;
        puVar3 = puVar3 + 1;
      }
    }
    else {
      for (; uVar12 != 0; uVar12 = uVar12 - 1) {
        uVar20 = (uint)(byte)*puVar3 + (int)uVar18 * 0x107;
        uVar18 = (ulong)uVar20;
        puVar3 = (ushort *)((long)puVar3 + 1);
      }
    }
    uVar20 = uVar20 & 0x3fffffff;
    uVar12 = (ulong)(*(int *)(param_1 + 9) - 1U & uVar20);
    uVar4 = *(uint *)(param_1[0xb] + uVar12 * 4);
    if (uVar4 != 0) {
      lVar19 = param_1[0xc];
      puVar3 = (ushort *)(param_2 + 4);
      uVar18 = uVar22 & 0x7fffffff;
      do {
        piVar13 = (int *)(ulong)uVar4;
        piVar17 = *(int **)(lVar19 + (long)piVar13 * 8);
        uVar6 = (uint)((ulong)*(undefined8 *)(piVar17 + 1) >> 0x20);
        uVar11 = (uint)*(undefined8 *)(piVar17 + 1);
        if (((uVar6 & 0x3fffffff) == uVar20 && uVar6 >> 0x1e == uVar10) &&
            (uVar11 & 0x7fffffff) == (uVar21 & 0x7fffffff)) {
          __s1 = (ushort *)(piVar17 + 4);
          if ((int)uVar11 < 0) {
            if ((int)uVar21 < 0) {
              puVar14 = puVar3;
              uVar15 = uVar18;
              if ((uVar22 & 0x7fffffff) == 0) goto LAB_0013fecc;
              do {
                iVar7 = (uint)*__s1 - (uint)*puVar14;
                if (iVar7 != 0) break;
                uVar15 = uVar15 - 1;
                puVar14 = puVar14 + 1;
                __s1 = __s1 + 1;
              } while (uVar15 != 0);
            }
            else {
              puVar14 = puVar3;
              uVar15 = uVar18;
              if ((uVar22 & 0x7fffffff) == 0) {
LAB_0013fecc:
                iVar7 = 0;
              }
              else {
                do {
                  iVar7 = (uint)*__s1 - (uint)(byte)*puVar14;
                  if (iVar7 != 0) break;
                  uVar15 = uVar15 - 1;
                  puVar14 = (ushort *)((long)puVar14 + 1);
                  __s1 = __s1 + 1;
                } while (uVar15 != 0);
              }
            }
          }
          else {
            puVar14 = puVar3;
            uVar15 = uVar22 & 0x7fffffff;
            if ((int)uVar21 < 0) {
              for (; uVar15 != 0; uVar15 = uVar15 - 1) {
                iVar7 = (uint)*puVar14 - (uint)(byte)*__s1;
                if (iVar7 != 0) {
                  iVar7 = -iVar7;
                  goto LAB_0013fe28;
                }
                __s1 = (ushort *)((long)__s1 + 1);
                puVar14 = puVar14 + 1;
              }
              iVar7 = 0;
            }
            else {
              iVar7 = memcmp(__s1,puVar3,uVar18);
            }
          }
LAB_0013fe28:
          if (iVar7 == 0) {
            if (0xe3 < (int)uVar4) {
              *piVar17 = *piVar17 + 1;
            }
            goto LAB_0013ffb8;
          }
        }
        uVar4 = piVar17[3];
      } while (uVar4 != 0);
    }
  }
  else {
    uVar12 = 0;
    uVar20 = (uint)(uVar10 != 3);
    uVar16 = 3;
  }
  if (*(int *)(param_1 + 0xd) == 0) {
    puVar1 = param_1 + 4;
    iVar7 = *(int *)(param_1 + 10) * 3;
    if (iVar7 < 0) {
      iVar7 = iVar7 + 1;
    }
    uVar10 = iVar7 >> 1;
    if ((int)uVar10 < 0xd4) {
      uVar10 = 0xd3;
    }
    puVar8 = (undefined8 *)(*(code *)param_1[2])(puVar1,param_1[0xc],(ulong)uVar10 << 3);
    if (puVar8 == (undefined8 *)0x0) {
LAB_0013ffa0:
      iVar7 = 5;
    }
    else {
      uVar22 = (ulong)*(uint *)(param_1 + 10);
      if (*(uint *)(param_1 + 10) == 0) {
        puVar9 = (undefined8 *)(*(code *)*param_1)(puVar1,0x10);
        if (puVar9 == (undefined8 *)0x0) {
          (*(code *)param_1[1])(puVar1,puVar8);
          goto LAB_0013ffa0;
        }
        *puVar9 = 0;
        puVar9[1] = 0;
        uVar22 = 1;
        *(undefined4 *)puVar9 = 1;
        *(ulong *)((long)puVar9 + 4) = *(ulong *)((long)puVar9 + 4) | 0xc000000000000000;
        iVar7 = *(int *)((long)param_1 + 0x4c);
        *puVar8 = puVar9;
        *(int *)((long)param_1 + 0x4c) = iVar7 + 1;
      }
      *(uint *)(param_1 + 10) = uVar10;
      param_1[0xc] = puVar8;
      *(uint *)(param_1 + 0xd) = (uint)uVar22;
      if ((uint)uVar22 < uVar10) {
        uVar18 = uVar22 * 2;
        do {
          uVar18 = uVar18 + 2;
          uVar15 = uVar18 & 0x1fffffffe | 1;
          if (uVar10 - 1 == uVar22) {
            uVar15 = 1;
          }
          uVar2 = uVar22 + 1;
          *(ulong *)(param_1[0xc] + uVar22 * 8) = uVar15;
          uVar22 = uVar2;
        } while (uVar10 != uVar2);
      }
      iVar7 = 0;
    }
    piVar13 = (int *)0x0;
    if (iVar7 != 5) {
      piVar17 = piVar13;
      if (iVar7 != 0) goto LAB_0013fff8;
      goto LAB_0013fb54;
    }
LAB_0013ffb8:
    piVar17 = piVar13;
    if ((param_2 != (int *)0x0) &&
       (iVar7 = *param_2, iVar5 = iVar7 + -1, *param_2 = iVar5, iVar5 == 0 || iVar7 < 1)) {
      if (*(ulong *)(param_2 + 1) >> 0x3e == 0) {
        (*(code *)param_1[1])(param_1 + 4,param_2);
      }
      else {
        FUN_001418dc(param_1,param_2);
      }
    }
  }
  else {
LAB_0013fb54:
    if (param_2 == (int *)0x0) {
      piVar13 = (int *)(*(code *)*param_1)(param_1 + 4,0x10);
      if (piVar13 == (int *)0x0) {
        piVar17 = (int *)0x0;
        goto LAB_0013fff8;
      }
      piVar13[0] = 1;
      piVar13[1] = -0x80000000;
    }
    else {
      uVar22 = *(ulong *)(param_2 + 1);
      if (uVar22 >> 0x3e == 0) {
        *(ulong *)(param_2 + 1) = uVar22 & 0x3fffffffffffffff | uVar16 << 0x3e;
        piVar13 = param_2;
      }
      else {
        uVar18 = uVar22 >> 0x1f & 1;
        piVar13 = (int *)(*(code *)*param_1)(param_1 + 4,
                                             ((long)(int)(((uint)uVar22 & 0x7fffffff) << uVar18) -
                                             uVar18) + 0x11);
        if (piVar13 == (int *)0x0) goto LAB_0013ffb8;
        *piVar13 = 1;
        uVar22 = (*(ulong *)(param_2 + 1) >> 0x1f & 1) << 0x1f;
        uVar18 = *(ulong *)(piVar13 + 1) & 0xffffffff00000000;
        *(ulong *)(piVar13 + 1) = uVar18 | *(ulong *)(piVar13 + 1) & 0x7fffffff | uVar22;
        *(ulong *)(piVar13 + 1) = uVar18 | uVar22 | *(ulong *)(param_2 + 1) & 0x7fffffff;
        uVar10 = param_2[1];
        memcpy(piVar13 + 4,param_2 + 4,
               (long)(int)(((uVar10 & 0x7fffffff) << (ulong)(uVar10 >> 0x1f)) -
                          ((int)~uVar10 >> 0x1f)));
        iVar7 = *param_2;
        iVar5 = iVar7 + -1;
        *param_2 = iVar5;
        if (iVar5 == 0 || iVar7 < 1) {
          if (*(ulong *)(param_2 + 1) >> 0x3e == 0) {
            (*(code *)param_1[1])(param_1 + 4,param_2);
          }
          else {
            FUN_001418dc(param_1,param_2);
          }
        }
      }
    }
    uVar10 = *(uint *)(param_1 + 0xd);
    piVar17 = (int *)(ulong)uVar10;
    lVar19 = param_1[0xc];
    piVar13[3] = uVar10;
    uVar22 = *(ulong *)(lVar19 + (long)piVar17 * 8);
    *(int **)(lVar19 + (long)piVar17 * 8) = piVar13;
    *(int *)(param_1 + 0xd) = (int)(uVar22 >> 1);
    *(ulong *)(piVar13 + 1) = CONCAT44(uVar20,piVar13[1]) | uVar16 << 0x3e;
    *(int *)((long)param_1 + 0x4c) = *(int *)((long)param_1 + 0x4c) + 1;
    if ((int)uVar16 != 3) {
      lVar19 = param_1[0xb];
      piVar13[3] = *(int *)(lVar19 + uVar12 * 4);
      *(uint *)(lVar19 + uVar12 * 4) = uVar10;
      if (*(int *)((long)param_1 + 0x54) <= *(int *)((long)param_1 + 0x4c)) {
        FUN_0016aaa4(param_1,*(int *)(param_1 + 9) << 1);
      }
    }
  }
LAB_0013fff8:
  return (ulong)piVar17 & 0xffffffff;
}

/* ===== FUN_00140044 @ 00140044 [libNexusScriptRuntime69252.so] ===== */

void FUN_00140044(long param_1,uint param_2,int param_3)

{
  long lVar1;
  int *piVar2;
  size_t sVar3;
  undefined8 uVar4;
  int iVar5;
  long lVar6;
  undefined1 auVar7 [16];
  char acStack_68 [64];
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  if ((int)param_2 < 0) {
    snprintf(acStack_68,0x40,"%u",(ulong)(param_2 & 0x7fffffff));
    sVar3 = strlen(acStack_68);
    auVar7 = FUN_0013f444(param_1,acStack_68,sVar3);
    goto LAB_001400e8;
  }
  if (*(uint *)(*(long *)(param_1 + 0x18) + 0x50) <= param_2) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0xc94,"JSValue __JS_AtomToValue(JSContext *, JSAtom, BOOL)","atom < rt->atom_size");
  }
  lVar6 = *(long *)(*(long *)(param_1 + 0x18) + 0x60);
  piVar2 = *(int **)(lVar6 + (ulong)param_2 * 8);
  if (*(ulong *)(piVar2 + 1) >> 0x3e == 1) {
LAB_001400a4:
    iVar5 = *piVar2;
    uVar4 = 0xfffffffffffffff9;
  }
  else {
    if (param_3 != 0) {
      if ((int)*(ulong *)(piVar2 + 1) == -0x80000000) {
        piVar2 = *(int **)(lVar6 + 0x178);
      }
      goto LAB_001400a4;
    }
    iVar5 = *piVar2;
    uVar4 = 0xfffffffffffffff8;
  }
  auVar7._8_8_ = uVar4;
  auVar7._0_8_ = piVar2;
  *piVar2 = iVar5 + 1;
LAB_001400e8:
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(auVar7._0_8_,auVar7._8_8_);
}

/* ===== FUN_00140708 @ 00140708 [libNexusScriptRuntime69252.so] ===== */

undefined1  [16] FUN_00140708(long param_1,void *param_2,uint param_3)

{
  undefined8 *puVar1;
  int *piVar2;
  undefined8 uVar3;
  long lVar4;
  undefined1 auVar5 [16];
  int *local_38;
  
  puVar1 = *(undefined8 **)(param_1 + 0x18);
  if ((int)param_3 < 1) {
    if (*(uint *)(puVar1 + 10) < 0x30) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0xc94,"JSValue __JS_AtomToValue(JSContext *, JSAtom, BOOL)","atom < rt->atom_size")
      ;
    }
    uVar3 = 0xfffffffffffffff9;
    local_38 = *(int **)(puVar1[0xc] + 0x178);
    *local_38 = *local_38 + 1;
  }
  else {
    piVar2 = (int *)(*(code *)*puVar1)(puVar1 + 4,(long)(int)param_3 + 0x11);
    if (piVar2 == (int *)0x0) {
      lVar4 = *(long *)(param_1 + 0x18);
      if ((*(ushort *)(lVar4 + 0xf0) & 0xff) == 0) {
        *(ushort *)(lVar4 + 0xf0) = *(ushort *)(lVar4 + 0xf0) & 0xff00 | 1;
        FUN_001405ec(param_1,"out of memory");
        piVar2 = (int *)0x0;
        *(ushort *)(lVar4 + 0xf0) = (ushort)*(byte *)(lVar4 + 0xf1) << 8;
      }
      else {
        piVar2 = (int *)0x0;
      }
    }
    else {
      piVar2[3] = 0;
      *piVar2 = 1;
      *(ulong *)(piVar2 + 1) = (ulong)(param_3 & 0x7fffffff);
    }
    if (piVar2 == (int *)0x0) {
      uVar3 = 6;
      local_38 = (int *)((ulong)local_38 & 0xffffffff00000000);
    }
    else {
      memcpy(piVar2 + 4,param_2,(ulong)param_3);
      uVar3 = 0xfffffffffffffff9;
      *(undefined1 *)((long)(piVar2 + 4) + (ulong)param_3) = 0;
      local_38 = piVar2;
    }
  }
  auVar5._8_8_ = uVar3;
  auVar5._0_8_ = local_38;
  return auVar5;
}

/* ===== FUN_00140918 @ 00140918 [libNexusScriptRuntime69252.so] ===== */

undefined1  [16] FUN_00140918(long *param_1)

{
  int iVar1;
  undefined8 uVar2;
  ulong uVar3;
  ulong uVar4;
  undefined1 auVar5 [16];
  int *local_8;
  
  if (*(int *)((long)param_1 + 0x1c) == 0) {
    iVar1 = (int)param_1[2];
    local_8 = (int *)param_1[1];
    if (iVar1 == 0) {
      (**(code **)(*(long *)(*param_1 + 0x18) + 8))(*(long *)(*param_1 + 0x18) + 0x20);
      param_1[1] = 0;
      if (*(uint *)(*(long *)(*param_1 + 0x18) + 0x50) < 0x30) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0xc94,"JSValue __JS_AtomToValue(JSContext *, JSAtom, BOOL)",
                  "atom < rt->atom_size");
      }
      local_8 = *(int **)(*(long *)(*(long *)(*param_1 + 0x18) + 0x60) + 0x178);
      uVar2 = 0xfffffffffffffff9;
      *local_8 = *local_8 + 1;
    }
    else {
      if (iVar1 < *(int *)((long)param_1 + 0x14)) {
        local_8 = (int *)(**(code **)(*(long *)(*param_1 + 0x18) + 0x10))
                                   (*(long *)(*param_1 + 0x18) + 0x20,local_8,
                                    ((long)(iVar1 << (ulong)(*(uint *)(param_1 + 3) & 0x1f)) -
                                    (long)(int)*(uint *)(param_1 + 3)) + 0x11);
        if (local_8 == (int *)0x0) {
          local_8 = (int *)param_1[1];
        }
        param_1[1] = (long)local_8;
      }
      if ((int)param_1[3] == 0) {
        *(undefined1 *)((long)local_8 + (long)(int)param_1[2] + 0x10) = 0;
      }
      uVar4 = *(ulong *)(local_8 + 1);
      uVar3 = (ulong)(uint)((int)param_1[3] << 0x1f);
      *(ulong *)(local_8 + 1) = uVar4 & 0xffffffff7fffffff | uVar3;
      uVar2 = 0xfffffffffffffff9;
      *(ulong *)(local_8 + 1) =
           uVar4 & 0xffffffff00000000 | uVar3 | (ulong)*(uint *)(param_1 + 2) & 0x7fffffff;
      param_1[1] = 0;
    }
  }
  else {
    uVar2 = 6;
    local_8 = (int *)((ulong)local_8 & 0xffffffff00000000);
  }
  auVar5._8_8_ = uVar2;
  auVar5._0_8_ = local_8;
  return auVar5;
}

/* ===== FUN_00141478 @ 00141478 [libNexusScriptRuntime69252.so] ===== */

undefined1  [16]
FUN_00141478(long param_1,undefined8 param_2,undefined4 param_3,undefined2 param_4,uint param_5,
            long param_6)

{
  undefined8 uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  undefined8 *puVar5;
  ulong uVar6;
  ulong uVar7;
  int *piVar8;
  undefined8 *puVar9;
  long lVar10;
  undefined8 *puVar11;
  int *piVar12;
  undefined1 auVar13 [16];
  undefined1 auVar14 [16];
  
  lVar10 = *(long *)(param_1 + 0x48);
  if (*(int *)(param_1 + 0x50) != -1) {
    lVar10 = 0;
  }
  uVar3 = ((int)((ulong)lVar10 >> 0x20) + (int)lVar10 * -0x61c8ffff) * -0x61c8ffff + 0x3c6e0001;
  for (piVar8 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x188) +
                         (ulong)(uVar3 >> (ulong)(-*(int *)(*(long *)(param_1 + 0x18) + 0x178) &
                                                 0x1f)) * 8);
      (piVar8 != (int *)0x0 &&
      (((piVar8[7] != uVar3 || (*(long *)(piVar8 + 0xe) != lVar10)) || (piVar8[10] != 0))));
      piVar8 = *(int **)(piVar8 + 0xc)) {
  }
  if (piVar8 == (int *)0x0) {
    piVar8 = (int *)FUN_0016b030(param_1,lVar10,2);
    if (piVar8 != (int *)0x0) goto LAB_00141530;
    auVar13 = ZEXT816(6) << 0x40;
  }
  else {
    *piVar8 = *piVar8 + 1;
LAB_00141530:
    auVar13 = FUN_00140ed0(param_1,piVar8,0xf);
  }
  uVar7 = auVar13._8_8_;
  piVar8 = auVar13._0_8_;
  if ((uVar7 & 0xffffffff) == 6) {
LAB_00141690:
    uVar7 = (ulong)piVar8 & 0xffffffff00000000;
  }
  else {
    puVar5 = (undefined8 *)
             (*(code *)**(undefined8 **)(param_1 + 0x18))
                       (*(undefined8 **)(param_1 + 0x18) + 4,
                        (-(ulong)(param_5 >> 0x1f) & 0xfffffff000000000 | (ulong)param_5 << 4) +
                        0x10);
    if (puVar5 == (undefined8 *)0x0) {
      FUN_00138d58(param_1);
    }
    else if (puVar5 != (undefined8 *)0x0) {
      *puVar5 = param_2;
      *(char *)(puVar5 + 1) = (char)param_3;
      *(char *)((long)puVar5 + 9) = (char)param_5;
      *(undefined2 *)((long)puVar5 + 10) = param_4;
      if (0 < (int)param_5) {
        uVar6 = (ulong)param_5;
        puVar9 = (undefined8 *)(param_6 + 8);
        puVar11 = puVar5 + 3;
        do {
          piVar12 = (int *)puVar9[-1];
          uVar1 = *puVar9;
          if (0xfffffff4 < (uint)uVar1) {
            *piVar12 = *piVar12 + 1;
          }
          puVar11[-1] = piVar12;
          *puVar11 = uVar1;
          puVar9 = puVar9 + 2;
          puVar11 = puVar11 + 2;
          uVar6 = uVar6 - 1;
        } while (uVar6 != 0);
      }
      if ((uVar7 & 0xffffffff) == 0xffffffff) {
        *(undefined8 **)(piVar8 + 0xc) = puVar5;
      }
      FUN_00148018(param_1,piVar8,uVar7,0x30,param_3,0,0,3,0,3,0x2701);
      if (*(uint *)(*(long *)(param_1 + 0x18) + 0x50) < 0x30) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0xc94,"JSValue __JS_AtomToValue(JSContext *, JSAtom, BOOL)",
                  "atom < rt->atom_size");
      }
      piVar12 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + 0x178);
      *piVar12 = *piVar12 + 1;
      FUN_00148018(param_1,piVar8,uVar7,0x38,piVar12,0xfffffffffffffff9,0,3,0,3,0x2701);
      iVar2 = *piVar12;
      iVar4 = iVar2 + -1;
      *piVar12 = iVar4;
      if (iVar4 == 0 || iVar2 < 1) {
        FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar12,0xfffffffffffffff9);
      }
      goto LAB_00141690;
    }
    if ((0xfffffff4 < auVar13._8_4_) &&
       (iVar2 = *piVar8, iVar4 = iVar2 + -1, *piVar8 = iVar4, iVar4 == 0 || iVar2 < 1)) {
      FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar8,uVar7);
    }
    uVar7 = 0;
    auVar13 = ZEXT816(6) << 0x40;
  }
  auVar14._0_8_ = uVar7 | auVar13._0_8_ & 0xffffffff;
  auVar14._8_8_ = auVar13._8_8_;
  return auVar14;
}

/* ===== FUN_00141774 @ 00141774 [libNexusScriptRuntime69252.so] ===== */

void FUN_00141774(long param_1,long param_2,uint param_3)

{
  long *plVar1;
  long lVar2;
  code *UNRECOVERED_JUMPTABLE;
  long *plVar3;
  undefined8 *puVar4;
  
  switch(param_3) {
  case 0xfffffff5:
  case 0xfffffff6:
  case 0xfffffff7:
    puVar4 = *(undefined8 **)(param_2 + 8);
    if ((puVar4 != (undefined8 *)0x0) && (*(long *)(param_2 + 0x28) != 0)) {
      (*(code *)puVar4[1])(*puVar4,*(long *)(param_2 + 0x28),0);
    }
    UNRECOVERED_JUMPTABLE = *(code **)(param_1 + 8);
    break;
  case 0xfffffff8:
LAB_00141890:
    FUN_001418dc(param_1,param_2);
    return;
  case 0xfffffff9:
    if (*(ulong *)(param_2 + 4) >> 0x3e != 0) goto LAB_00141890;
    UNRECOVERED_JUMPTABLE = *(code **)(param_1 + 8);
    break;
  default:
    printf("__JS_FreeValue: unknown tag=%d\n",(ulong)param_3);
                    /* WARNING: Subroutine does not return */
    abort();
  case 0xfffffffd:
                    /* WARNING: Subroutine does not return */
    abort();
  case 0xfffffffe:
  case 0xffffffff:
    if (*(char *)(param_1 + 0xb8) != '\x02') {
      plVar3 = (long *)(param_2 + 8);
      lVar2 = *plVar3;
      plVar1 = *(long **)(param_2 + 0x10);
      *(long **)(lVar2 + 8) = plVar1;
      *plVar1 = lVar2;
      *plVar3 = 0;
      *(undefined8 *)(param_2 + 0x10) = 0;
      puVar4 = *(undefined8 **)(param_1 + 0xa0);
      *(long **)(param_1 + 0xa0) = plVar3;
      *plVar3 = param_1 + 0x98;
      *(undefined8 **)(param_2 + 0x10) = puVar4;
      *puVar4 = plVar3;
      if (*(char *)(param_1 + 0xb8) == '\0') {
        lVar2 = *(long *)(param_1 + 0xa0);
        *(undefined1 *)(param_1 + 0xb8) = 1;
        while (lVar2 != param_1 + 0x98) {
          if (*(int *)(lVar2 + -8) != 0) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0x15e6,"void free_zero_refcount(JSRuntime *)","p->ref_count == 0");
          }
          FUN_0016b224(param_1);
          lVar2 = *(long *)(param_1 + 0xa0);
        }
        *(undefined1 *)(param_1 + 0xb8) = 0;
      }
    }
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x001417e8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)(param_1 + 0x20,param_2);
  return;
}

/* ===== FUN_001418dc @ 001418dc [libNexusScriptRuntime69252.so] ===== */

void FUN_001418dc(long param_1,long param_2)

{
  uint uVar1;
  int iVar2;
  ulong uVar3;
  long lVar4;
  long lVar5;
  uint *puVar6;
  
  if (*(ulong *)(param_2 + 4) >> 0x3e < 3) {
    puVar6 = (uint *)(*(long *)(param_1 + 0x58) +
                     (ulong)((uint)(*(ulong *)(param_2 + 4) >> 0x20) & *(int *)(param_1 + 0x48) - 1U
                            & 0x3fffffff) * 4);
    uVar3 = (ulong)*puVar6;
    lVar4 = *(long *)(*(long *)(param_1 + 0x60) + uVar3 * 8);
    if (lVar4 != param_2) {
      do {
        lVar5 = lVar4;
        if ((int)uVar3 == 0) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0xbd4,"void JS_FreeAtomStruct(JSRuntime *, JSAtomStruct *)","i != 0");
        }
        uVar3 = (ulong)*(uint *)(lVar5 + 0xc);
        lVar4 = *(long *)(*(long *)(param_1 + 0x60) + uVar3 * 8);
      } while (lVar4 != param_2);
      puVar6 = (uint *)(lVar5 + 0xc);
    }
    *puVar6 = *(uint *)(lVar4 + 0xc);
  }
  else {
    uVar3 = (ulong)*(uint *)(param_2 + 0xc);
  }
  uVar1 = *(uint *)(param_1 + 0x68);
  *(int *)(param_1 + 0x68) = (int)uVar3;
  *(ulong *)(*(long *)(param_1 + 0x60) + uVar3 * 8) = (ulong)uVar1 << 1 | 1;
  (**(code **)(param_1 + 8))(param_1 + 0x20);
  iVar2 = *(int *)(param_1 + 0x4c);
  *(int *)(param_1 + 0x4c) = iVar2 + -1;
  if (0 < iVar2) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
            ,0xbe8,"void JS_FreeAtomStruct(JSRuntime *, JSAtomStruct *)","rt->atom_count >= 0");
}

/* ===== FUN_001419d8 @ 001419d8 [libNexusScriptRuntime69252.so] ===== */

void FUN_001419d8(long param_1)

{
  long lVar1;
  
  lVar1 = *(long *)(param_1 + 0xa0);
  *(undefined1 *)(param_1 + 0xb8) = 1;
  while( true ) {
    if (lVar1 == param_1 + 0x98) {
      *(undefined1 *)(param_1 + 0xb8) = 0;
      return;
    }
    if (*(int *)(lVar1 + -8) != 0) break;
    FUN_0016b224(param_1);
    lVar1 = *(long *)(param_1 + 0xa0);
  }
                    /* WARNING: Subroutine does not return */
  __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
            ,0x15e6,"void free_zero_refcount(JSRuntime *)","p->ref_count == 0");
}

/* ===== FUN_00142bd0 @ 00142bd0 [libNexusScriptRuntime69252.so] ===== */

char * FUN_00142bd0(char *param_1,char *param_2,uint param_3)

{
  bool bVar1;
  ushort uVar2;
  byte bVar3;
  byte *pbVar4;
  ulong uVar5;
  char *pcVar6;
  char *pcVar7;
  ulong uVar8;
  ulong uVar9;
  
  if ((int)param_3 < 0) {
    snprintf(param_2,0x40,"%u",(ulong)(param_3 & 0x7fffffff));
    return param_2;
  }
  if (*(uint *)(param_1 + 0x50) <= param_3) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0xc5b,"const char *JS_AtomGetStrRT(JSRuntime *, char *, int, JSAtom)",
              "atom < rt->atom_size");
  }
  if (param_3 == 0) {
    builtin_strncpy(param_2,"<null>",7);
  }
  else {
    uVar8 = *(ulong *)(*(long *)(param_1 + 0x60) + (ulong)param_3 * 8);
    if ((uVar8 & 1) != 0) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0xc65,"const char *JS_AtomGetStrRT(JSRuntime *, char *, int, JSAtom)",
                "!atom_is_free(p)");
    }
    pcVar7 = param_2;
    if (uVar8 != 0) {
      uVar5 = *(ulong *)(uVar8 + 4);
      if (-1 < (int)uVar5) {
        if ((uVar5 & 0x7fffffff) != 0) {
          bVar3 = 0;
          uVar5 = uVar5 & 0x7fffffff;
          pbVar4 = (byte *)(uVar8 + 0x10);
          do {
            uVar5 = uVar5 - 1;
            bVar3 = bVar3 | *pbVar4;
            pbVar4 = pbVar4 + 1;
          } while (uVar5 != 0);
          if (0x7f < bVar3) goto LAB_00142c8c;
        }
        bVar1 = false;
        param_1 = (char *)(uVar8 + 0x10);
        goto LAB_00142d08;
      }
LAB_00142c8c:
      uVar5 = *(ulong *)(uVar8 + 4);
      if ((uVar5 & 0x7fffffff) != 0) {
        uVar9 = 0;
        pcVar6 = param_2;
        do {
          if ((int)uVar5 < 0) {
            uVar2 = *(ushort *)(uVar8 + 0x10 + uVar9 * 2);
          }
          else {
            uVar2 = (ushort)*(byte *)(uVar8 + 0x10 + uVar9);
          }
          pcVar7 = pcVar6;
          if (0x39 < (long)pcVar6 - (long)param_2) break;
          if (uVar2 < 0x80) {
            pcVar7 = pcVar6 + 1;
            *pcVar6 = (char)uVar2;
          }
          else {
            param_1 = (char *)FUN_001d2b44(pcVar6);
            pcVar7 = pcVar6 + (int)param_1;
          }
          uVar5 = *(ulong *)(uVar8 + 4);
          uVar9 = uVar9 + 1;
          pcVar6 = pcVar7;
        } while (uVar9 < (uVar5 & 0x7fffffff));
      }
    }
    *pcVar7 = '\0';
  }
  bVar1 = true;
LAB_00142d08:
  if (bVar1) {
    return param_2;
  }
  return param_1;
}

/* ===== FUN_00145030 @ 00145030 [libNexusScriptRuntime69252.so] ===== */

void FUN_00145030(long param_1,long *param_2,uint *param_3,long param_4,uint param_5)

{
  uint *puVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  bool bVar6;
  bool bVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  undefined8 uVar11;
  uint uVar12;
  uint uVar13;
  code *pcVar14;
  ulong uVar15;
  int *piVar16;
  uint uVar17;
  ulong uVar18;
  uint uVar19;
  uint uVar20;
  uint uVar21;
  long lVar22;
  long lVar23;
  uint *puVar24;
  long lVar25;
  uint uVar26;
  int iVar27;
  ulong uVar28;
  uint local_c0;
  uint local_b0 [14];
  undefined1 auStack_78 [4];
  uint local_74;
  long local_70;
  long local_68;
  
  lVar5 = tpidr_el0;
  local_68 = *(long *)(lVar5 + 0x28);
  *param_2 = 0;
  *param_3 = 0;
  lVar22 = *(long *)(param_4 + 0x18);
  local_74 = 0;
  local_70 = 0;
  if (*(int *)(lVar22 + 0x28) < 1) {
    iVar10 = 0;
    iVar27 = 0;
    local_c0 = 0;
  }
  else {
    lVar23 = 0;
    lVar25 = 0;
    iVar27 = 0;
    iVar10 = 0;
    puVar24 = (uint *)(lVar22 + 0x44);
    local_c0 = 0;
    do {
      uVar12 = *puVar24;
      if (uVar12 != 0) {
        if ((int)uVar12 < 0) {
LAB_00145118:
          uVar17 = 0;
          bVar6 = true;
        }
        else {
          uVar18 = *(ulong *)(*(long *)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) +
                                       (ulong)uVar12 * 8) + 4);
          uVar17 = (uint)(uVar18 >> 0x20);
          if (uVar17 >> 0x1e == 1) goto LAB_00145118;
          if (uVar17 >> 0x1e == 3) {
            if ((uVar18 >> 0x20 & 0x3fffffff) == 0) goto LAB_00145140;
            if ((uVar17 & 0x3fffffff) != 1) {
                    /* WARNING: Subroutine does not return */
              abort();
            }
            bVar6 = false;
            uVar17 = 2;
          }
          else {
            if (uVar17 >> 0x1e != 2) {
                    /* WARNING: Subroutine does not return */
              abort();
            }
LAB_00145140:
            bVar6 = false;
            uVar17 = 1;
          }
        }
        if ((((param_5 >> 4 & 1) == 0) || ((puVar24[-1] >> 0x1c & 1) != 0)) &&
           ((param_5 >> (ulong)uVar17 & 1) != 0)) {
          if (((puVar24[-1] >> 0x1e == 2) && ((param_5 & 0x30) != 0)) &&
             (*(int *)(*(long *)(*(long *)(*(long *)(param_4 + 0x20) + lVar23) + 0x18) + 8) == 4)) {
            FUN_00144d04(param_1,(ulong)uVar12);
            goto LAB_0014588c;
          }
          iVar8 = FUN_0016d090(param_1,auStack_78);
          if (iVar8 == 0) {
            if (bVar6) {
              iVar27 = iVar27 + 1;
            }
            else {
              iVar10 = iVar10 + 1;
            }
          }
          else {
            local_c0 = local_c0 + 1;
          }
        }
      }
      lVar25 = lVar25 + 1;
      lVar23 = lVar23 + 0x10;
      puVar24 = puVar24 + 2;
    } while (lVar25 < *(int *)(lVar22 + 0x28));
  }
  if ((*(byte *)(param_4 + 5) >> 2 & 1) == 0) {
LAB_001451c8:
    iVar8 = 0;
LAB_0014542c:
    uVar12 = local_c0 + iVar27;
    uVar17 = iVar8 + iVar10 + uVar12;
    uVar19 = uVar17;
    if ((int)uVar17 < 2) {
      uVar19 = 1;
    }
    lVar22 = (*(code *)**(undefined8 **)(param_1 + 0x18))
                       (*(undefined8 **)(param_1 + 0x18) + 4,(ulong)uVar19 << 3);
    if (lVar22 == 0) {
      FUN_00138d58(param_1);
    }
    else if (lVar22 != 0) {
      lVar25 = *(long *)(param_4 + 0x18);
      uVar19 = local_c0;
      if (*(int *)(lVar25 + 0x28) < 1) {
        uVar15 = 0;
        uVar18 = (ulong)uVar12;
        bVar6 = true;
      }
      else {
        iVar27 = 0;
        uVar26 = 0;
        puVar24 = (uint *)(lVar25 + 0x44);
        uVar18 = (ulong)uVar12;
        bVar6 = true;
        do {
          uVar20 = *puVar24;
          uVar28 = (ulong)uVar20;
          if (uVar20 == 0) {
LAB_001454a0:
            uVar15 = (ulong)uVar26;
          }
          else {
            uVar13 = puVar24[-1];
            if ((int)uVar20 < 0) {
LAB_0014550c:
              uVar21 = 0;
              bVar7 = true;
            }
            else {
              uVar15 = *(ulong *)(*(long *)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + uVar28 * 8
                                           ) + 4);
              uVar21 = (uint)(uVar15 >> 0x20);
              if (uVar21 >> 0x1e == 1) goto LAB_0014550c;
              if (uVar21 >> 0x1e == 3) {
                if ((uVar15 >> 0x20 & 0x3fffffff) == 0) goto LAB_00145534;
                if ((uVar21 & 0x3fffffff) != 1) {
                    /* WARNING: Subroutine does not return */
                  abort();
                }
                bVar7 = false;
                uVar21 = 2;
              }
              else {
                if (uVar21 >> 0x1e != 2) {
                    /* WARNING: Subroutine does not return */
                  abort();
                }
LAB_00145534:
                bVar7 = false;
                uVar21 = 1;
              }
            }
            if ((((param_5 >> 4 & 1) != 0) && ((uVar13 >> 0x1c & 1) == 0)) ||
               ((param_5 >> (ulong)uVar21 & 1) == 0)) goto LAB_001454a0;
            iVar10 = FUN_0016d090(param_1,auStack_78,uVar28);
            if (iVar10 == 0) {
              if (bVar7) {
                uVar15 = (ulong)uVar26;
                uVar21 = uVar19;
                uVar19 = uVar19 + 1;
              }
              else {
                uVar21 = (uint)uVar18;
                uVar18 = (ulong)(uVar21 + 1);
                uVar15 = (ulong)uVar26;
              }
            }
            else {
              bVar6 = false;
              uVar15 = (ulong)(uVar26 + 1);
              uVar21 = uVar26;
            }
            if (0xe3 < (int)uVar20) {
              piVar16 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + uVar28 * 8);
              *piVar16 = *piVar16 + 1;
            }
            puVar1 = (uint *)(lVar22 + (long)(int)uVar21 * 8);
            *puVar1 = uVar13 >> 0x1c & 1;
            puVar1[1] = uVar20;
          }
          iVar27 = iVar27 + 1;
          puVar24 = puVar24 + 2;
          uVar26 = (uint)uVar15;
        } while (iVar27 < *(int *)(lVar25 + 0x28));
      }
      uVar20 = (uint)uVar18;
      uVar26 = (uint)uVar15;
      if ((*(byte *)(param_4 + 5) >> 2 & 1) != 0) {
        if ((*(byte *)(param_4 + 5) >> 3 & 1) == 0) {
          if (*(short *)(param_4 + 6) == 5) {
            if ((((param_5 & 1) != 0) && (*(int *)(param_4 + 0x38) == -7)) &&
               (uVar13 = *(uint *)(*(long *)(param_4 + 0x30) + 4) & 0x7fffffff, uVar13 != 0))
            goto LAB_001457e8;
          }
          else {
            if (local_74 != 0) {
              lVar25 = 0;
              uVar15 = 0;
              do {
                iVar27 = *(int *)(local_70 + lVar25);
                uVar20 = ((int *)(local_70 + lVar25))[1];
                if ((int)uVar20 < 0) {
LAB_00145754:
                  lVar23 = 0;
joined_r0x00145758:
                  if ((param_5 >> 4 & 1) == 0) goto LAB_001456ec;
LAB_00145784:
                  if (iVar27 != 0) goto LAB_001456ec;
LAB_00145788:
                  if ((0xe3 < (int)uVar20) &&
                     (piVar16 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) +
                                         (ulong)uVar20 * 8), iVar27 = *piVar16, iVar10 = iVar27 + -1
                     , *piVar16 = iVar10, iVar10 == 0 || iVar27 < 1)) {
                    FUN_001418dc();
                  }
                }
                else {
                  uVar28 = *(ulong *)(*(long *)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) +
                                               (ulong)uVar20 * 8) + 4);
                  uVar13 = (uint)(uVar28 >> 0x20);
                  if (uVar13 >> 0x1e == 1) goto LAB_00145754;
                  if (uVar13 >> 0x1e != 3) {
                    if (uVar13 >> 0x1e != 2) {
                    /* WARNING: Subroutine does not return */
                      abort();
                    }
LAB_0014577c:
                    lVar23 = 1;
                    goto joined_r0x00145758;
                  }
                  if ((uVar28 >> 0x20 & 0x3fffffff) == 0) goto LAB_0014577c;
                  if ((uVar13 & 0x3fffffff) != 1) {
                    /* WARNING: Subroutine does not return */
                    abort();
                  }
                  lVar23 = 2;
                  if ((param_5 >> 4 & 1) != 0) goto LAB_00145784;
LAB_001456ec:
                  if ((param_5 >> lVar23 & 1) == 0) goto LAB_00145788;
                  piVar16 = (int *)(lVar22 + uVar18 * 8);
                  uVar18 = (ulong)((int)uVar18 + 1);
                  *piVar16 = iVar27;
                  piVar16[1] = uVar20;
                }
                uVar20 = (uint)uVar18;
                uVar15 = uVar15 + 1;
                lVar25 = lVar25 + 8;
              } while (uVar15 < local_74);
            }
            (**(code **)(*(long *)(param_1 + 0x18) + 8))(*(long *)(param_1 + 0x18) + 0x20,local_70);
          }
        }
        else if (((param_5 & 1) != 0) && (uVar13 = *(uint *)(param_4 + 0x40), 0 < (int)uVar13)) {
LAB_001457e8:
          iVar27 = -0x80000000;
          do {
            puVar2 = (undefined4 *)(lVar22 + uVar15 * 8);
            uVar26 = (int)uVar15 + 1;
            uVar15 = (ulong)uVar26;
            uVar13 = uVar13 - 1;
            *puVar2 = 1;
            puVar2[1] = iVar27;
            iVar27 = iVar27 + 1;
          } while (uVar13 != 0);
        }
      }
      if (uVar26 != local_c0) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0x1efc,
                  "int JS_GetOwnPropertyNamesInternal(JSContext *, JSPropertyEnum **, uint32_t *, JSObject *, int)"
                  ,"num_index == num_keys_count",param_1);
      }
      if (uVar19 != uVar12) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0x1efd,
                  "int JS_GetOwnPropertyNamesInternal(JSContext *, JSPropertyEnum **, uint32_t *, JSObject *, int)"
                  ,"str_index == num_keys_count + str_keys_count",param_1);
      }
      if (uVar20 != uVar17) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0x1efe,
                  "int JS_GetOwnPropertyNamesInternal(JSContext *, JSPropertyEnum **, uint32_t *, JSObject *, int)"
                  ,"sym_index == atom_count",param_1);
      }
      if ((local_c0 != 0) && (!bVar6)) {
        FUN_001d2cd8(lVar22,local_c0,8,FUN_0016d318);
      }
      uVar11 = 0;
      *param_2 = lVar22;
      *param_3 = uVar17;
      goto LAB_00145890;
    }
    lVar22 = local_70;
    if (local_70 != 0) {
      uVar18 = (ulong)local_74;
      if (local_74 != 0) {
        puVar24 = (uint *)(local_70 + 4);
        do {
          if ((0xe3 < (int)*puVar24) &&
             (piVar16 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + (ulong)*puVar24 * 8)
             , iVar27 = *piVar16, iVar10 = iVar27 + -1, *piVar16 = iVar10, iVar10 == 0 || iVar27 < 1
             )) {
            FUN_001418dc();
          }
          uVar18 = uVar18 - 1;
          puVar24 = puVar24 + 2;
        } while (uVar18 != 0);
      }
      (**(code **)(*(long *)(param_1 + 0x18) + 8))(*(long *)(param_1 + 0x18) + 0x20,lVar22);
    }
  }
  else {
    if ((*(byte *)(param_4 + 5) >> 3 & 1) != 0) {
      iVar8 = 0;
      if ((param_5 & 1) != 0) {
        uVar12 = *(uint *)(param_4 + 0x40);
LAB_00145264:
        iVar8 = 0;
        local_c0 = uVar12 + local_c0;
      }
      goto LAB_0014542c;
    }
    if ((ulong)*(ushort *)(param_4 + 6) == 5) {
      if ((param_5 & 1) != 0) {
        if (*(int *)(param_4 + 0x38) == -7) {
          uVar12 = *(uint *)(*(long *)(param_4 + 0x30) + 4) & 0x7fffffff;
        }
        else {
          uVar12 = 0;
        }
        goto LAB_00145264;
      }
      goto LAB_001451c8;
    }
    lVar22 = *(long *)(*(long *)(*(long *)(param_1 + 0x18) + 0x70) +
                       (ulong)*(ushort *)(param_4 + 6) * 0x28 + 0x20);
    if ((lVar22 == 0) || (pcVar14 = *(code **)(lVar22 + 8), pcVar14 == (code *)0x0)) {
LAB_00145420:
      iVar8 = 0;
LAB_00145424:
      bVar6 = true;
    }
    else {
      iVar8 = (*pcVar14)(param_1,&local_70,&local_74,param_4,0xffffffffffffffff);
      if (iVar8 == 0) {
        if (local_74 == 0) goto LAB_00145420;
        uVar18 = 0;
        iVar8 = 0;
        do {
          uVar12 = *(uint *)(local_70 + uVar18 * 8 + 4);
          if ((int)uVar12 < 0) {
LAB_001452f8:
            uVar12 = 0;
          }
          else {
            uVar15 = *(ulong *)(*(long *)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) +
                                         (ulong)uVar12 * 8) + 4);
            uVar12 = (uint)(uVar15 >> 0x20);
            if (uVar12 >> 0x1e == 1) goto LAB_001452f8;
            if (uVar12 >> 0x1e == 3) {
              if ((uVar15 >> 0x20 & 0x3fffffff) == 0) goto LAB_00145318;
              if ((uVar12 & 0x3fffffff) != 1) {
                    /* WARNING: Subroutine does not return */
                abort();
              }
              uVar12 = 2;
            }
            else {
              if (uVar12 >> 0x1e != 2) {
                    /* WARNING: Subroutine does not return */
                abort();
              }
LAB_00145318:
              uVar12 = 1;
            }
          }
          if ((param_5 >> (ulong)uVar12 & 1) != 0) {
            if ((param_5 & 0x30) == 0) {
              uVar12 = 0;
            }
            else {
              iVar9 = FUN_001459d8(param_1,local_b0,param_4);
              lVar22 = local_70;
              if (iVar9 < 0) {
                if (local_70 != 0) {
                  uVar15 = (ulong)local_74;
                  if (local_74 != 0) {
                    puVar24 = (uint *)(local_70 + 4);
                    do {
                      if ((0xe3 < (int)*puVar24) &&
                         (piVar16 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) +
                                             (ulong)*puVar24 * 8), iVar3 = *piVar16,
                         iVar4 = iVar3 + -1, *piVar16 = iVar4, iVar4 == 0 || iVar3 < 1)) {
                        FUN_001418dc();
                      }
                      uVar15 = uVar15 - 1;
                      puVar24 = puVar24 + 2;
                    } while (uVar15 != 0);
                  }
                  (**(code **)(*(long *)(param_1 + 0x18) + 8))
                            (*(long *)(param_1 + 0x18) + 0x20,lVar22);
                }
                uVar12 = 0;
              }
              else {
                if (iVar9 == 0) {
                  uVar12 = 0;
                }
                else {
                  uVar12 = local_b0[0] >> 2 & 1;
                  FUN_0016d248(param_1,local_b0);
                }
                *(uint *)(local_70 + uVar18 * 8) = uVar12;
              }
              if (iVar9 < 0) {
                bVar6 = false;
                goto LAB_00145428;
              }
            }
            if ((param_5 & 0x10) == 0 || uVar12 != 0) {
              iVar8 = iVar8 + 1;
            }
          }
          uVar18 = uVar18 + 1;
        } while (uVar18 < local_74);
        goto LAB_00145424;
      }
      iVar8 = 0;
      bVar6 = false;
    }
LAB_00145428:
    if (bVar6) goto LAB_0014542c;
  }
LAB_0014588c:
  uVar11 = 0xffffffff;
LAB_00145890:
  if (*(long *)(lVar5 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar11);
  }
  return;
}

/* ===== FUN_0014624c @ 0014624c [libNexusScriptRuntime69252.so] ===== */

ulong FUN_0014624c(long param_1,long param_2,undefined8 param_3)

{
  ulong uVar1;
  long lVar2;
  int *piVar3;
  ulong extraout_x8;
  long lVar4;
  long lVar5;
  ulong uVar6;
  undefined1 auVar7 [16];
  
  if (((int)param_3 == 0) && (-1 < (int)(uint)param_2)) {
    uVar1 = (ulong)((uint)param_2 | 0x80000000);
  }
  else if ((int)param_3 == -8) {
    lVar2 = *(long *)(param_1 + 0x18);
    if (*(ulong *)(param_2 + 4) >> 0x3e < 3) {
      uVar1 = (ulong)*(uint *)(*(long *)(lVar2 + 0x58) +
                              (ulong)((uint)(*(ulong *)(param_2 + 4) >> 0x20) &
                                      *(int *)(lVar2 + 0x48) - 1U & 0x3fffffff) * 4);
      lVar5 = *(long *)(*(long *)(lVar2 + 0x60) + uVar1 * 8);
      while (lVar5 != param_2) {
        if ((int)uVar1 == 0) {
code_r0x001463b4:
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0xaf4,"JSAtom js_get_atom_index(JSRuntime *, JSAtomStruct *)","i != 0");
        }
        uVar1 = (ulong)*(uint *)(lVar5 + 0xc);
        lVar5 = *(long *)(*(long *)(lVar2 + 0x60) + uVar1 * 8);
      }
    }
    else {
      uVar1 = (ulong)*(uint *)(param_2 + 0xc);
    }
    if (0xe3 < (int)uVar1) {
      piVar3 = *(int **)(*(long *)(lVar2 + 0x60) + uVar1 * 8);
      *piVar3 = *piVar3 + 1;
    }
  }
  else {
    auVar7 = FUN_0014c3ec(param_1,param_2,param_3,1);
    lVar2 = auVar7._0_8_;
    uVar6 = auVar7._8_8_ & 0xffffffff;
    uVar1 = extraout_x8;
    if (uVar6 != 6) {
      if (uVar6 == 0xfffffff8) {
        if (*(ulong *)(lVar2 + 4) >> 0x3e < 3) {
          lVar4 = *(long *)(param_1 + 0x18);
          uVar1 = (ulong)*(uint *)(*(long *)(lVar4 + 0x58) +
                                  (ulong)((uint)(*(ulong *)(lVar2 + 4) >> 0x20) &
                                          *(int *)(lVar4 + 0x48) - 1U & 0x3fffffff) * 4);
          lVar5 = *(long *)(*(long *)(lVar4 + 0x60) + uVar1 * 8);
          while (lVar5 != lVar2) {
            if ((int)uVar1 == 0) goto code_r0x001463b4;
            uVar1 = (ulong)*(uint *)(lVar5 + 0xc);
            lVar5 = *(long *)(*(long *)(lVar4 + 0x60) + uVar1 * 8);
          }
        }
        else {
          uVar1 = (ulong)*(uint *)(lVar2 + 0xc);
        }
      }
      else {
        uVar1 = FUN_0013f8bc(param_1,lVar2);
        uVar1 = uVar1 & 0xffffffff;
      }
    }
    uVar1 = uVar1 & 0xffffffff;
    if (uVar6 == 6) {
      uVar1 = 0;
    }
  }
  return uVar1;
}

/* ===== FUN_0014681c @ 0014681c [libNexusScriptRuntime69252.so] ===== */

/* WARNING: Removing unreachable block (ram,0x00146ce0) */
/* WARNING: Removing unreachable block (ram,0x00146d68) */
/* WARNING: Removing unreachable block (ram,0x00146cec) */

ulong FUN_0014681c(long param_1,int *param_2,int param_3,uint param_4,long param_5,long param_6,
                  int *param_7,undefined8 param_8,uint param_9)

{
  byte bVar1;
  int iVar2;
  long lVar3;
  bool bVar4;
  int *piVar5;
  int iVar6;
  uint uVar7;
  undefined8 uVar8;
  int *piVar9;
  int *piVar10;
  long lVar11;
  char *pcVar12;
  long *plVar13;
  uint *puVar14;
  long lVar15;
  code *pcVar16;
  ulong uVar17;
  undefined8 *puVar18;
  int *piVar19;
  int *piVar20;
  undefined1 (*pauVar21) [16];
  uint unaff_w22;
  ulong uVar22;
  int *piVar23;
  undefined1 auVar24 [16];
  undefined1 auStack_b0 [8];
  int *local_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  long local_80;
  int local_78;
  undefined4 uStack_74;
  long local_70;
  
  lVar3 = tpidr_el0;
  local_70 = *(long *)(lVar3 + 0x28);
  iVar6 = (int)param_8;
  piVar23 = param_2;
  if (iVar6 == -1) {
    piVar10 = param_7;
    if (param_7 != param_2) goto LAB_00146bf8;
    uVar22 = 0x8000000;
    do {
      lVar15 = *(long *)(param_2 + 6);
      uVar7 = *(uint *)(lVar15 + ~(ulong)(*(uint *)(lVar15 + 0x20) & param_4) * 4);
      uVar17 = (ulong)uVar7;
      if (uVar7 == 0) goto LAB_00146e8c;
      do {
        puVar14 = (uint *)(lVar15 + 0x40 + (uVar17 - 1) * 8);
        if (puVar14[1] == param_4) {
          plVar13 = (long *)(*(long *)(param_2 + 8) + (uVar17 - 1) * 0x10);
          break;
        }
        uVar7 = *puVar14;
        plVar13 = (long *)0x0;
        puVar14 = (uint *)0x0;
        uVar17 = (ulong)uVar7 & 0x3ffffff;
      } while ((uVar7 & 0x3ffffff) != 0);
      if (puVar14 == (uint *)0x0) goto LAB_00146e8c;
      uVar7 = *puVar14;
      if ((uVar7 & 0xe8000000) == 0x8000000) {
        lVar15 = plVar13[1];
        local_a8 = (int *)*plVar13;
        *plVar13 = param_5;
        plVar13[1] = param_6;
        if ((0xfffffff4 < (uint)lVar15) &&
           (iVar6 = *local_a8, iVar2 = iVar6 + -1, *local_a8 = iVar2, iVar2 == 0 || iVar6 < 1)) {
          FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_a8);
        }
LAB_0014698c:
        uVar22 = 1;
        goto LAB_00146990;
      }
      if ((uVar7 >> 0x1d & 1) != 0) {
        if (*(short *)((long)param_7 + 6) != 2) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x2211,
                    "int JS_SetPropertyInternal(JSContext *, JSValue, JSAtom, JSValue, JSValue, int)"
                    ,"p->class_id == JS_CLASS_ARRAY");
        }
        if (param_4 != 0x30) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x2212,
                    "int JS_SetPropertyInternal(JSContext *, JSValue, JSAtom, JSValue, JSValue, int)"
                    ,"prop == JS_ATOM_length");
        }
        if (*(long *)(lVar3 + 0x28) == local_70) {
          uVar22 = FUN_00147590(param_1,param_7,param_5,param_6,param_9);
          return uVar22;
        }
        goto LAB_00146b98;
      }
      uVar7 = uVar7 >> 0x1a & 0x30;
      if (uVar7 != 0x30) {
        if (uVar7 == 0x20) {
          if (*(short *)((long)param_7 + 6) != 0xb) {
            FUN_0013ed44(param_1,*(undefined8 *)(*plVar13 + 0x18),param_5,param_6);
            goto LAB_0014698c;
          }
          goto LAB_00147494;
        }
        if (uVar7 != 0x10) goto LAB_00147494;
        if (*(long *)(lVar3 + 0x28) == local_70) {
          uVar22 = FUN_00147900(param_1,plVar13[1],param_7,param_8);
          return uVar22;
        }
        goto LAB_00146b98;
      }
      iVar6 = FUN_00144d7c(param_1,param_7,param_4);
    } while (iVar6 == 0);
    FUN_0013a308(param_1,param_5,param_6);
  }
  else {
    if (param_3 == -1) {
LAB_00146a90:
      piVar10 = (int *)0x0;
      goto LAB_00146bdc;
    }
    if (iVar6 != 3) {
      if (iVar6 == 2) {
        FUN_0013a308(param_1,param_5,param_6);
        FUN_00143944(param_1,param_4,"cannot set property \'%s\' of null");
LAB_00146a28:
        uVar22 = 0xffffffff;
        goto LAB_00146990;
      }
      piVar23 = (int *)FUN_00143fa4(param_1);
      goto LAB_00146a90;
    }
    FUN_0013a308(param_1,param_5,param_6);
    FUN_00143944(param_1,param_4,"cannot set property \'%s\' of undefined");
  }
  uVar22 = 0xffffffff;
  goto LAB_00146990;
LAB_00146e8c:
  unaff_w22 = (uint)uVar22;
  if ((*(byte *)((long)piVar23 + 5) >> 2 & 1) == 0) goto LAB_00147158;
  if ((*(byte *)((long)piVar23 + 5) >> 3 & 1) != 0) {
    if (-1 < (int)param_4) {
      if ((10 < *(ushort *)((long)piVar23 + 6) - 0x15) ||
         (iVar6 = FUN_00144f24(param_1,param_4), iVar6 == 0)) goto LAB_00147158;
      if (-1 < iVar6) goto LAB_001472dc;
      FUN_0013a308(param_1,param_5,param_6);
LAB_00147378:
      uVar22 = 0xffffffff;
      goto LAB_00146990;
    }
    if ((param_4 & 0x7fffffff) < (uint)piVar23[0x10]) {
      iVar6 = 7;
      if (piVar10 == piVar23) {
        uVar22 = FUN_00147a8c(param_1,param_7,param_8,param_4 & 0x7fffffff,0,param_5,param_6,param_9
                             );
        uVar22 = uVar22 & 0xffffffff;
        iVar6 = 1;
      }
    }
    else {
      iVar6 = 9;
      if (10 < *(ushort *)((long)piVar23 + 6) - 0x15) {
        iVar6 = 0;
      }
    }
    unaff_w22 = (uint)uVar22;
    if (iVar6 == 0) goto LAB_00147158;
    if (iVar6 == 7) goto LAB_0014718c;
    if (iVar6 != 9) goto LAB_00146990;
LAB_001472dc:
    if ((*(ushort *)((long)piVar23 + 6) & 0xfffe) == 0x1c) {
      iVar6 = FUN_00147f44(param_1,auStack_b0,param_5,param_6);
      if (iVar6 != 0) {
        unaff_w22 = 0xffffffff;
      }
      uVar22 = (ulong)unaff_w22;
      if (iVar6 != 0) goto LAB_00146990;
    }
    else {
      auVar24 = FUN_0016db18(param_1,param_5,param_6,0);
      FUN_0013a308(param_1,auVar24._0_8_,auVar24._8_8_);
      if (auVar24._8_4_ == 6) {
        uVar22 = 0xffffffff;
        goto LAB_00146990;
      }
    }
    uVar22 = 1;
    goto LAB_00146990;
  }
  puVar18 = *(undefined8 **)
             (*(long *)(*(long *)(param_1 + 0x18) + 0x70) +
              (ulong)*(ushort *)((long)piVar23 + 6) * 0x28 + 0x20);
  if (puVar18 == (undefined8 *)0x0) {
LAB_00146f5c:
    iVar6 = 0;
  }
  else {
    pcVar16 = (code *)puVar18[6];
    lVar15 = param_5;
    lVar11 = param_6;
    if (pcVar16 != (code *)0x0) {
      *piVar23 = *piVar23 + 1;
      uVar22 = (*pcVar16)(param_1,piVar23,0xffffffffffffffff,param_4,param_5,param_6,param_7,param_8
                          ,param_9);
      FUN_0013a308(param_1,piVar23,0xffffffffffffffff);
      uVar17 = uVar22 & 0xffffffff;
      goto LAB_00146f2c;
    }
    pcVar16 = (code *)*puVar18;
    bVar4 = true;
    iVar6 = 0;
    if (pcVar16 != (code *)0x0) {
      *piVar23 = *piVar23 + 1;
      uVar7 = (*pcVar16)(param_1,&local_a8,piVar23,0xffffffffffffffff,param_4);
      FUN_0013a308(param_1,piVar23,0xffffffffffffffff);
      uVar17 = (ulong)uVar7;
      if ((int)uVar7 < 0) goto LAB_00146f2c;
      bVar4 = true;
      if (uVar7 == 0) {
        iVar6 = 0;
        goto LAB_00146f58;
      }
      if (((byte)local_a8 >> 4 & 1) == 0) {
        FUN_0013a308(param_1,local_a0,uStack_98);
        iVar6 = 6;
        bVar4 = false;
        if (((byte)local_a8 >> 1 & 1) != 0) {
          iVar6 = 7;
          if (piVar10 != piVar23) goto LAB_00146f58;
          uVar22 = FUN_00148018(param_1,param_7,param_8,param_4,param_5,param_6,0,3,0,3,0x2000);
          uVar17 = uVar22 & 0xffffffff;
          goto LAB_00146f2c;
        }
      }
      else {
        lVar15 = 0;
        if (local_78 != 3) {
          lVar15 = local_80;
        }
        uVar22 = FUN_00147900(param_1,lVar15,param_7,param_8,param_5,param_6,param_9);
        FUN_0013a308(param_1,local_90,uStack_88);
        lVar15 = local_80;
        lVar11 = CONCAT44(uStack_74,local_78);
        uVar17 = uVar22 & 0xffffffff;
LAB_00146f2c:
        uVar22 = uVar17;
        FUN_0013a308(param_1,lVar15,lVar11);
        iVar6 = 1;
      }
      bVar4 = false;
    }
LAB_00146f58:
    if (bVar4) goto LAB_00146f5c;
  }
  unaff_w22 = (uint)uVar22;
  if (iVar6 != 0) {
    if (iVar6 == 6) goto LAB_00147494;
    if (iVar6 != 7) goto LAB_00146990;
LAB_0014718c:
    if ((param_9 >> 0x10 & 1) != 0) {
      FUN_0013a308(param_1,param_5,param_6);
      FUN_00144f80(param_1,param_4);
      goto LAB_00147378;
    }
    if (piVar10 == (int *)0x0) {
      FUN_0013a308(param_1,param_5,param_6);
      pcVar12 = "not an object";
    }
    else {
      bVar1 = *(byte *)((long)piVar10 + 5);
      if ((bVar1 & 1) != 0) {
        if (piVar10 == param_2) {
          if ((bVar1 >> 2 & 1) == 0) {
            plVar13 = (long *)FUN_001490ec(param_1,piVar10,param_4,7);
            if (plVar13 != (long *)0x0) {
              uVar22 = 1;
              *plVar13 = param_5;
              plVar13[1] = param_6;
              goto LAB_00146990;
            }
            FUN_0013a308(param_1,param_5,param_6);
            goto LAB_00146a28;
          }
          if (((((int)param_4 < 0) && (*(short *)((long)piVar10 + 6) == 2)) &&
              ((bVar1 >> 3 & 1) != 0)) && ((param_4 & 0x7fffffff) == piVar10[0x10])) {
            uVar22 = FUN_00148ff8(param_1,piVar10,param_5,param_6,param_9);
            uVar22 = uVar22 & 0xffffffff;
          }
          else {
LAB_00147234:
            uVar22 = FUN_00149404(param_1,piVar10,param_4,param_5,param_6,0,3);
            uVar22 = uVar22 & 0xffffffff;
            FUN_0013a308(param_1,param_5,param_6);
          }
          goto LAB_00146990;
        }
        uVar7 = FUN_001459d8(param_1,&local_a8,piVar10,param_4);
        if (-1 < (int)uVar7) {
          if (uVar7 == 0) goto LAB_00147234;
          if (((byte)local_a8 >> 4 & 1) != 0) {
            FUN_0013a308(param_1,local_90,uStack_88);
            FUN_0013a308(param_1,local_80,CONCAT44(uStack_74,local_78));
            FUN_0013a308(param_1,param_5,param_6);
            uVar22 = FUN_00148ec8(param_1,param_9,"setter is forbidden");
            uVar22 = uVar22 & 0xffffffff;
            goto LAB_00146990;
          }
          FUN_0013a308(param_1,local_a0,uStack_98);
          if ((((byte)local_a8 >> 1 & 1) == 0) || (*(short *)((long)piVar10 + 6) == 0xb))
          goto LAB_00147494;
          uVar7 = FUN_00148018(param_1,param_7,param_8,param_4,param_5,param_6,0,3,0,3,0x2000);
        }
        uVar22 = (ulong)uVar7;
        FUN_0013a308(param_1,param_5,param_6);
        goto LAB_00146990;
      }
      FUN_0013a308(param_1,param_5,param_6);
      pcVar12 = "object is not extensible";
    }
    uVar22 = FUN_00148ec8(param_1,param_9,pcVar12);
    goto LAB_001474c0;
  }
LAB_00147158:
  piVar23 = *(int **)(*(long *)(piVar23 + 6) + 0x38);
LAB_00146bdc:
  if (piVar23 == (int *)0x0) goto LAB_0014718c;
LAB_00146bf8:
  while( true ) {
    piVar9 = *(int **)(piVar23 + 6);
    uVar22 = (ulong)(uint)piVar9[~(ulong)(piVar9[8] & param_4)];
    if (piVar9[~(ulong)(piVar9[8] & param_4)] == 0) break;
    do {
      puVar14 = (uint *)(piVar9 + 0x10 + (uVar22 - 1) * 2);
      if (puVar14[1] == param_4) {
        pauVar21 = (undefined1 (*) [16])(*(long *)(piVar23 + 8) + (uVar22 - 1) * 0x10);
        break;
      }
      uVar7 = *puVar14;
      pauVar21 = (undefined1 (*) [16])0x0;
      puVar14 = (uint *)0x0;
      uVar22 = (ulong)uVar7 & 0x3ffffff;
    } while ((uVar7 & 0x3ffffff) != 0);
    if (puVar14 == (uint *)0x0) break;
    uVar7 = *puVar14 >> 0x1a & 0x30;
    if (uVar7 != 0x30) {
      uVar22 = (ulong)unaff_w22;
      if (uVar7 == 0x10) {
        uVar22 = FUN_00147900(param_1,*(undefined8 *)(*pauVar21 + 8),param_7,param_8,param_5,param_6
                              ,param_9);
        uVar22 = uVar22 & 0xffffffff;
        goto LAB_00146990;
      }
      if ((*puVar14 >> 0x1b & 1) == 0) goto LAB_00147494;
      goto LAB_00146e8c;
    }
    if ((char)piVar9[6] != '\0') {
      if (*piVar9 == 1) {
        lVar15 = *(long *)(param_1 + 0x18);
        piVar5 = (int *)(*(long *)(lVar15 + 0x188) +
                        (ulong)((uint)piVar9[7] >> ((ulong)(0x20 - *(int *)(lVar15 + 0x178)) & 0x3f)
                               ) * 8);
        do {
          piVar19 = piVar5;
          piVar20 = *(int **)piVar19;
          piVar5 = piVar20 + 0xc;
        } while (piVar20 != piVar9);
        *(undefined8 *)piVar19 = *(undefined8 *)(piVar9 + 0xc);
        *(int *)(lVar15 + 0x180) = *(int *)(lVar15 + 0x180) + -1;
        *(undefined1 *)(piVar9 + 6) = 0;
      }
      else {
        lVar15 = FUN_0016d47c(param_1);
        if (lVar15 == 0) {
          uVar22 = 0xffffffff;
          goto LAB_00146990;
        }
        uVar8 = *(undefined8 *)(param_1 + 0x18);
        iVar6 = **(int **)(piVar23 + 6);
        iVar2 = iVar6 + -1;
        **(int **)(piVar23 + 6) = iVar2;
        if (iVar2 == 0 || iVar6 < 1) {
          FUN_0016aef4(uVar8);
        }
        *(long *)(piVar23 + 6) = lVar15;
        puVar14 = (uint *)(lVar15 + ((ulong)((long)puVar14 - (long)(piVar9 + 0x10)) >> 3 &
                                    0xffffffff) * 8 + 0x40);
      }
    }
    auVar24 = (*(code *)(&PTR_FUN_001ecad0)[*(ulong *)*pauVar21 & 3])
                        (*(ulong *)*pauVar21 & 0xfffffffffffffffc,piVar23,param_4,
                         *(undefined8 *)(*pauVar21 + 8));
    FUN_0013ee10(*(ulong *)*pauVar21 & 0xfffffffffffffffc);
    *puVar14 = *puVar14 & 0x3fffffff;
    *(undefined4 *)*pauVar21 = 0;
    *(undefined8 *)(*pauVar21 + 8) = 3;
    if (auVar24._8_4_ == 6) {
      uVar22 = 0xffffffff;
      goto LAB_00146990;
    }
    *pauVar21 = auVar24;
  }
  uVar22 = (ulong)unaff_w22;
  goto LAB_00146e8c;
LAB_00147494:
  FUN_0013a308(param_1,param_5,param_6);
  uVar22 = FUN_00149378(param_1,param_9,param_4);
LAB_001474c0:
  uVar22 = uVar22 & 0xffffffff;
LAB_00146990:
  if (*(long *)(lVar3 + 0x28) == local_70) {
    return uVar22;
  }
LAB_00146b98:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00148018 @ 00148018 [libNexusScriptRuntime69252.so] ===== */

/* WARNING: Removing unreachable block (ram,0x00148304) */
/* WARNING: Removing unreachable block (ram,0x001481bc) */

ulong FUN_00148018(long param_1,long param_2,undefined8 param_3,uint param_4,int *param_5,
                  undefined8 param_6,int *param_7,undefined8 param_8,int *param_9,uint param_10,
                  uint param_11)

{
  uint uVar1;
  int *piVar2;
  uint uVar3;
  ushort uVar4;
  long lVar5;
  bool bVar6;
  bool bVar7;
  char cVar8;
  int iVar9;
  int iVar10;
  undefined8 uVar11;
  undefined8 *puVar12;
  char *pcVar13;
  uint uVar14;
  long lVar15;
  ulong uVar16;
  int *piVar17;
  int *piVar18;
  uint uVar19;
  int *piVar20;
  uint unaff_w25;
  uint uVar21;
  undefined1 (*pauVar22) [16];
  uint *puVar23;
  double dVar24;
  undefined1 auVar25 [16];
  int *local_a8;
  undefined8 local_a0;
  uint *local_78;
  int *local_70;
  long local_68;
  
  lVar5 = tpidr_el0;
  local_68 = *(long *)(lVar5 + 0x28);
  if ((int)param_3 == -1) {
    uVar19 = param_11 >> 8 & 7;
    uVar1 = param_11 & 0x1800;
    bVar7 = (uVar19 ^ 7 | uVar19 & param_11) != 7;
    uVar19 = param_4 & 0x7fffffff;
    uVar21 = param_11 & 0x202;
    local_a0 = param_6;
    local_a8 = param_5;
LAB_001480f8:
    lVar15 = *(long *)(param_2 + 0x18);
    uVar3 = *(uint *)(lVar15 + ~(ulong)(*(uint *)(lVar15 + 0x20) & param_4) * 4);
    uVar16 = (ulong)uVar3;
    uVar14 = (uint)local_a0;
    if (uVar3 == 0) {
      local_78 = (uint *)0x0;
LAB_00148164:
      if ((*(byte *)(param_2 + 5) >> 3 & 1) == 0) goto LAB_001487fc;
      if (*(ushort *)(param_2 + 6) == 2) {
        if ((-1 < (int)param_4) || (*(uint *)(param_2 + 0x40) <= uVar19)) goto LAB_001480ec;
        if (bVar7 || uVar1 != 0) goto LAB_001482cc;
        if ((param_11 >> 0xd & 1) != 0) {
          lVar15 = *(long *)(param_2 + 0x38);
          if (0xfffffff4 < uVar14) {
            *local_a8 = *local_a8 + 1;
          }
          puVar12 = (undefined8 *)(lVar15 + (ulong)uVar19 * 0x10);
          uVar11 = puVar12[1];
          local_70 = (int *)*puVar12;
          *puVar12 = local_a8;
          puVar12[1] = local_a0;
          if ((0xfffffff4 < (uint)uVar11) &&
             (iVar10 = *local_70, iVar9 = iVar10 + -1, *local_70 = iVar9, iVar9 == 0 || iVar10 < 1))
          {
            FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_70);
          }
        }
        iVar10 = 1;
        unaff_w25 = 1;
      }
      else if (*(ushort *)(param_2 + 6) - 0x15 < 0xb) {
        if ((int)param_4 < 0) {
          if (uVar19 < *(uint *)(param_2 + 0x40)) {
            if (!bVar7 && uVar1 == 0) {
              if ((param_11 >> 0xd & 1) == 0) {
                unaff_w25 = 1;
              }
              else {
                local_70 = local_a8;
                if (0xfffffff4 < uVar14) {
                  *local_a8 = *local_a8 + 1;
                }
                unaff_w25 = FUN_00147a8c(param_1,param_2,param_3,(ulong)uVar19,0,local_a8,local_a0,
                                         param_11);
              }
              goto LAB_00148518;
            }
            pcVar13 = "invalid descriptor flags";
          }
          else {
LAB_001484f8:
            pcVar13 = "out-of-bound index in typed array";
          }
LAB_00148508:
          unaff_w25 = FUN_00148ec8(param_1,param_11,pcVar13);
        }
        else {
          auVar25 = FUN_0014a59c(param_1,param_4);
          uVar11 = auVar25._8_8_;
          piVar20 = auVar25._0_8_;
          uVar19 = auVar25._8_4_;
          if (uVar19 == 3) goto LAB_001480ec;
          if (uVar19 == 6) {
LAB_001481ac:
            unaff_w25 = 0xffffffff;
          }
          else {
            local_70 = piVar20;
            if ((uVar19 == 7) || (uVar19 == 0)) {
              if (0xfffffff4 < uVar19) {
                *piVar20 = *piVar20 + 1;
              }
              if (uVar19 < 3) {
                iVar10 = 0;
                local_70 = (int *)(double)(int)auVar25._0_4_;
              }
              else if (uVar19 == 7) {
                iVar10 = 0;
              }
              else {
                iVar10 = FUN_0016eeb4(param_1,&local_70,piVar20,uVar11);
              }
              if (iVar10 != 0) {
                local_70 = piVar20;
                if ((0xfffffff4 < uVar19) &&
                   (iVar10 = *piVar20, iVar9 = iVar10 + -1, *piVar20 = iVar9,
                   iVar9 == 0 || iVar10 < 1)) {
                  FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar20,uVar11);
                }
                goto LAB_001481ac;
              }
              dVar24 = ABS((double)local_70);
              bVar7 = (double)(long)(double)local_70 == (double)local_70 &&
                      (dVar24 != INFINITY && dVar24 < INFINITY == NAN(dVar24) || dVar24 < INFINITY);
            }
            else {
              bVar7 = false;
            }
            if (bVar7) {
              uVar21 = 0;
              switch(uVar19) {
              case 0:
                uVar21 = auVar25._0_4_ >> 0x1f;
                break;
              case 7:
                uVar21 = auVar25._4_4_ >> 0x1f;
                break;
              case 0xfffffff5:
              case 0xfffffff7:
                uVar21 = piVar20[4];
                break;
              case 0xfffffff6:
                if (piVar20[4] == 0) {
                  uVar21 = 0;
                }
                else {
                  uVar21 = (uint)(*(long *)(piVar20 + 6) != -0x8000000000000000);
                }
              }
              local_70 = piVar20;
              if ((0xfffffff4 < uVar19) &&
                 (iVar10 = *piVar20, iVar9 = iVar10 + -1, *piVar20 = iVar9, iVar9 == 0 || iVar10 < 1
                 )) {
                FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar20,uVar11);
              }
              if (uVar21 != 0) {
                pcVar13 = "negative index in typed array";
                goto LAB_00148508;
              }
              goto LAB_001484f8;
            }
            local_70 = piVar20;
            if ((0xfffffff4 < uVar19) &&
               (iVar10 = *piVar20, iVar9 = iVar10 + -1, *piVar20 = iVar9, iVar9 == 0 || iVar10 < 1))
            {
              FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar20,uVar11);
            }
            unaff_w25 = FUN_00148ec8(param_1,param_11,"non integer index in typed array");
          }
        }
LAB_00148518:
        iVar10 = 1;
      }
      else {
LAB_001480ec:
        iVar10 = 0;
      }
      goto LAB_001487f8;
    }
    do {
      local_78 = (uint *)(lVar15 + 0x40 + (uVar16 - 1) * 8);
      if (local_78[1] == param_4) {
        pauVar22 = (undefined1 (*) [16])(*(long *)(param_2 + 0x20) + (uVar16 - 1) * 0x10);
        break;
      }
      uVar3 = *local_78;
      pauVar22 = (undefined1 (*) [16])0x0;
      uVar16 = (ulong)uVar3 & 0x3ffffff;
      local_78 = (uint *)0x0;
    } while ((uVar3 & 0x3ffffff) != 0);
    if (local_78 == (uint *)0x0) goto LAB_00148164;
    if (((param_11 >> 0xd & 1) != 0) && ((*local_78 >> 0x1d & 1) != 0)) {
      local_70 = local_a8;
      if (0xfffffff4 < uVar14) {
        *local_a8 = *local_a8 + 1;
      }
      iVar10 = FUN_00149d70(param_1,&local_70,local_a8,local_a0,0);
      if (iVar10 == 0) {
        lVar15 = *(long *)(param_2 + 0x18);
        uVar3 = *(uint *)(lVar15 + ~(ulong)(*(uint *)(lVar15 + 0x20) & param_4) * 4);
        uVar16 = (ulong)uVar3;
        local_a8 = (int *)((ulong)local_70 & 0xffffffff);
        if ((int)local_70 < 0) {
          local_a8 = (int *)(double)(long)((ulong)local_70 & 0xffffffff);
        }
        local_a0 = 0;
        if ((int)local_70 < 0) {
          local_a0 = 7;
        }
        if (uVar3 == 0) {
          pauVar22 = (undefined1 (*) [16])0x0;
          local_78 = (uint *)0x0;
        }
        else {
          do {
            local_78 = (uint *)(lVar15 + 0x40 + (uVar16 - 1) * 8);
            if (local_78[1] == param_4) {
              pauVar22 = (undefined1 (*) [16])(*(long *)(param_2 + 0x20) + (uVar16 - 1) * 0x10);
              break;
            }
            uVar3 = *local_78;
            pauVar22 = (undefined1 (*) [16])0x0;
            uVar16 = (ulong)uVar3 & 0x3ffffff;
            local_78 = (uint *)0x0;
          } while ((uVar3 & 0x3ffffff) != 0);
        }
        if (local_78 == (uint *)0x0) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x247d,
                    "int JS_DefineProperty(JSContext *, JSValue, JSAtom, JSValue, JSValue, JSValue, int)"
                    ,"prs != NULL");
        }
      }
      else {
        unaff_w25 = 0xffffffff;
      }
      if (iVar10 != 0) goto LAB_0014885c;
    }
    puVar23 = local_78;
    uVar3 = *local_78;
    if ((((uVar3 >> 0x1a & 1) == 0) &&
        (((param_11 & 0x101) == 0x101 ||
         (((param_11 >> 10 & 1) != 0 && (((uVar3 >> 0x1a ^ param_11) >> 2 & 1) != 0)))))) ||
       (((param_11 & 0x3a00) != 0 &&
        (((uVar3 >> 0x1a & 1) == 0 &&
         (((uVar1 != 0) != ((uVar3 & 0xc0000000) == 0x40000000) ||
          ((((uVar21 == 0x202 && ((uVar3 >> 0x1b & 1) == 0)) && (uVar1 == 0)) &&
           ((uVar3 & 0xc0000000) != 0x40000000)))))))))) {
LAB_00148c2c:
      unaff_w25 = FUN_00148ec8(param_1,param_11,"property is not configurable");
      goto LAB_0014885c;
    }
    uVar14 = uVar3 >> 0x1a & 0x30;
    if (uVar14 == 0x30) {
      piVar20 = *(int **)(param_2 + 0x18);
      if ((char)piVar20[6] == '\0') {
        bVar6 = true;
      }
      else if (*piVar20 == 1) {
        lVar15 = *(long *)(param_1 + 0x18);
        piVar2 = (int *)(*(long *)(lVar15 + 0x188) +
                        (ulong)((uint)piVar20[7] >>
                               ((ulong)(0x20 - *(int *)(lVar15 + 0x178)) & 0x3f)) * 8);
        do {
          piVar17 = piVar2;
          piVar18 = *(int **)piVar17;
          piVar2 = piVar18 + 0xc;
        } while (piVar18 != piVar20);
        *(undefined8 *)piVar17 = *(undefined8 *)(piVar20 + 0xc);
        bVar6 = true;
        *(int *)(lVar15 + 0x180) = *(int *)(lVar15 + 0x180) + -1;
        *(undefined1 *)(piVar20 + 6) = 0;
      }
      else {
        lVar15 = FUN_0016d47c(param_1,piVar20);
        if (lVar15 == 0) {
          bVar6 = false;
        }
        else {
          uVar11 = *(undefined8 *)(param_1 + 0x18);
          iVar10 = **(int **)(param_2 + 0x18);
          iVar9 = iVar10 + -1;
          **(int **)(param_2 + 0x18) = iVar9;
          if (iVar9 == 0 || iVar10 < 1) {
            FUN_0016aef4(uVar11);
          }
          bVar6 = true;
          *(long *)(param_2 + 0x18) = lVar15;
          puVar23 = (uint *)(lVar15 + ((ulong)((long)puVar23 + (-0x40 - (long)piVar20)) >> 3 &
                                      0xffffffff) * 8 + 0x40);
        }
      }
      if (bVar6) {
        auVar25 = (*(code *)(&PTR_FUN_001ecad0)[*(ulong *)*pauVar22 & 3])
                            (*(ulong *)*pauVar22 & 0xfffffffffffffffc,param_2,param_4,
                             *(undefined8 *)(*pauVar22 + 8));
        FUN_0013ee10(*(ulong *)*pauVar22 & 0xfffffffffffffffc);
        *puVar23 = *puVar23 & 0x3fffffff;
        *(undefined4 *)*pauVar22 = 0;
        *(undefined8 *)(*pauVar22 + 8) = 3;
        if (auVar25._8_4_ == 6) goto LAB_00148e94;
        *pauVar22 = auVar25;
        goto LAB_001480f8;
      }
    }
    else {
      if ((param_11 & 0x3a00) != 0) {
        if (uVar1 != 0) {
          if ((uint)param_8 == 0xffffffff) {
            uVar4 = *(ushort *)((long)param_7 + 6);
            if (uVar4 == 0xd) {
              cVar8 = '\x01';
            }
            else if (uVar4 == 0x30) {
              cVar8 = *(char *)(*(long *)(param_7 + 0xc) + 0x20);
            }
            else {
              cVar8 = *(long *)(*(long *)(*(long *)(param_1 + 0x18) + 0x70) + (ulong)uVar4 * 0x28 +
                               0x18) != 0;
            }
          }
          else {
            cVar8 = '\0';
          }
          piVar20 = (int *)0x0;
          if (cVar8 != '\0') {
            piVar20 = param_7;
          }
          local_70 = param_9;
          if (param_10 == 0xffffffff) {
            uVar4 = *(ushort *)((long)param_9 + 6);
            if (uVar4 == 0xd) {
              cVar8 = '\x01';
            }
            else if (uVar4 == 0x30) {
              cVar8 = *(char *)(*(long *)(param_9 + 0xc) + 0x20);
            }
            else {
              cVar8 = *(long *)(*(long *)(*(long *)(param_1 + 0x18) + 0x70) + (ulong)uVar4 * 0x28 +
                               0x18) != 0;
            }
          }
          else {
            cVar8 = '\0';
          }
          piVar2 = (int *)0x0;
          if (cVar8 != '\0') {
            piVar2 = param_9;
          }
          if (uVar3 >> 0x1e == 1) {
            if (((uVar3 >> 0x1a & 1) == 0) &&
               ((((param_11 >> 0xb & 1) != 0 && (piVar20 != *(int **)*pauVar22)) ||
                (((param_11 >> 0xc & 1) != 0 && (piVar2 != *(int **)(*pauVar22 + 8))))))) {
              iVar10 = 3;
            }
            else {
LAB_00148b90:
              if ((param_11 >> 0xb & 1) != 0) {
                if (*(long *)*pauVar22 != 0) {
                  FUN_0013a308(param_1,*(long *)*pauVar22,0xffffffffffffffff);
                }
                if ((piVar20 != (int *)0x0) && (local_70 = param_7, 0xfffffff4 < (uint)param_8)) {
                  *param_7 = *param_7 + 1;
                }
                *(int **)*pauVar22 = piVar20;
              }
              if ((param_11 >> 0xc & 1) == 0) {
                iVar10 = 0;
              }
              else {
                if (*(long *)(*pauVar22 + 8) != 0) {
                  FUN_0013a308(param_1,*(long *)(*pauVar22 + 8),0xffffffffffffffff);
                }
                if ((piVar2 != (int *)0x0) && (local_70 = param_9, 0xfffffff4 < param_10)) {
                  *param_9 = *param_9 + 1;
                }
                iVar10 = 0;
                *(int **)(*pauVar22 + 8) = piVar2;
              }
            }
          }
          else {
            iVar10 = FUN_0014a05c(param_1,param_2,&local_78);
            if (iVar10 == 0) {
              if (*local_78 >> 0x1e == 2) {
                FUN_0014a174(*(undefined8 *)(param_1 + 0x18),*(undefined8 *)*pauVar22);
              }
              else {
                FUN_0013a308(param_1,*(undefined8 *)*pauVar22,*(undefined8 *)(*pauVar22 + 8));
              }
              *local_78 = *local_78 & 0x17ffffff | 0x40000000;
              *(undefined8 *)*pauVar22 = 0;
              *(undefined8 *)(*pauVar22 + 8) = 0;
              goto LAB_00148b90;
            }
            unaff_w25 = 0xffffffff;
            iVar10 = 1;
          }
          if (iVar10 == 0) goto LAB_00148d98;
          if (iVar10 == 3) goto LAB_00148c2c;
          goto LAB_0014885c;
        }
        if (uVar14 != 0x20) {
          if (uVar14 == 0x10) {
            iVar10 = FUN_0014a05c(param_1,param_2,&local_78);
            if (iVar10 != 0) goto LAB_00148e94;
            if (*(long *)*pauVar22 != 0) {
              FUN_0013a308(param_1,*(long *)*pauVar22,0xffffffffffffffff);
            }
            if (*(long *)(*pauVar22 + 8) != 0) {
              FUN_0013a308(param_1,*(long *)(*pauVar22 + 8),0xffffffffffffffff);
            }
            *local_78 = *local_78 & 0x37ffffff;
            *(undefined4 *)*pauVar22 = 0;
            *(undefined8 *)(*pauVar22 + 8) = 3;
            goto LAB_00148984;
          }
          if (((param_11 >> 0xd & 1) == 0) || ((uVar3 & 0xc000000) != 0)) goto LAB_00148984;
          iVar10 = FUN_0014a2b0(param_1,local_a8,local_a0,*(undefined8 *)*pauVar22,
                                *(undefined8 *)(*pauVar22 + 8));
          if (iVar10 == 0) goto LAB_00148c2c;
LAB_00148dc4:
          unaff_w25 = 1;
          goto LAB_0014885c;
        }
LAB_00148984:
        uVar19 = (uint)local_a0;
        if (*local_78 >> 0x1e != 2) {
          if ((*local_78 >> 0x1d & 1) == 0) {
            if ((param_11 >> 0xd & 1) != 0) {
              FUN_0013a308(param_1,*(undefined8 *)*pauVar22,*(undefined8 *)(*pauVar22 + 8));
              local_70 = local_a8;
              if (0xfffffff4 < uVar19) {
                *local_a8 = *local_a8 + 1;
              }
              *(int **)*pauVar22 = local_a8;
              *(undefined8 *)(*pauVar22 + 8) = local_a0;
            }
            if (((param_11 >> 9 & 1) == 0) ||
               (iVar10 = FUN_0014a2f8(param_1,param_2,&local_78,
                                      *local_78 >> 0x1a & 0xfffffffd | param_11 & 2), iVar10 == 0))
            goto LAB_00148d98;
            goto LAB_00148e94;
          }
          if ((param_11 >> 0xd & 1) == 0) {
            unaff_w25 = 1;
          }
          else {
            local_70 = local_a8;
            if (0xfffffff4 < uVar19) {
              *local_a8 = *local_a8 + 1;
            }
            unaff_w25 = FUN_00147590(param_1,param_2,local_a8,local_a0,param_11);
          }
          if (uVar21 != 0x200) goto LAB_0014885c;
          local_78 = (uint *)(*(long *)(param_2 + 0x18) + 0x40);
          iVar10 = FUN_0014a2f8(param_1,param_2,&local_78,*local_78 >> 0x1a & 0xfffffffd);
          if (iVar10 == 0) goto LAB_0014885c;
          goto LAB_00148e94;
        }
        if ((param_11 >> 0xd & 1) != 0) {
          puVar12 = *(undefined8 **)(*(long *)*pauVar22 + 0x18);
          if (*(short *)(param_2 + 6) == 0xb) {
            iVar10 = FUN_0014a2b0(param_1,local_a8,local_a0,*puVar12,puVar12[1]);
            if (iVar10 == 0) goto LAB_00148c2c;
          }
          else {
            local_70 = local_a8;
            if (0xfffffff4 < uVar19) {
              *local_a8 = *local_a8 + 1;
            }
            FUN_0013ed44(param_1,puVar12,local_a8,local_a0);
          }
        }
        if (uVar21 != 0x200) goto LAB_00148d98;
        if (*(short *)(param_2 + 6) == 0xb) {
          unaff_w25 = FUN_00148ec8(param_1,param_11,
                                   "module namespace properties have writable = false");
          bVar7 = false;
        }
        else {
          iVar10 = FUN_0014a05c(param_1,param_2,&local_78);
          if (iVar10 == 0) {
            puVar12 = *(undefined8 **)(*(long *)*pauVar22 + 0x18);
            piVar20 = (int *)*puVar12;
            uVar11 = puVar12[1];
            if (0xfffffff4 < (uint)uVar11) {
              *piVar20 = *piVar20 + 1;
            }
            local_70 = piVar20;
            FUN_0014a174(*(undefined8 *)(param_1 + 0x18),*(undefined8 *)*pauVar22);
            *(int **)*pauVar22 = piVar20;
            *(undefined8 *)(*pauVar22 + 8) = uVar11;
            bVar7 = true;
            *local_78 = *local_78 & 0x37ffffff;
          }
          else {
            bVar7 = false;
            unaff_w25 = 0xffffffff;
          }
        }
        if (!bVar7) goto LAB_0014885c;
      }
LAB_00148d98:
      puVar23 = local_78;
      uVar19 = param_11 >> 8 & 5;
      uVar19 = *local_78 >> 0x1a & (uVar19 ^ 0xffffffff) | uVar19 & param_11;
      if (*local_78 >> 0x1a == uVar19) goto LAB_00148dc4;
      piVar20 = *(int **)(param_2 + 0x18);
      if ((char)piVar20[6] == '\0') goto LAB_00148e7c;
      if (*piVar20 == 1) {
        lVar15 = *(long *)(param_1 + 0x18);
        piVar2 = (int *)(*(long *)(lVar15 + 0x188) +
                        (ulong)((uint)piVar20[7] >>
                               ((ulong)(0x20 - *(int *)(lVar15 + 0x178)) & 0x3f)) * 8);
        do {
          piVar17 = piVar2;
          piVar18 = *(int **)piVar17;
          piVar2 = piVar18 + 0xc;
        } while (piVar18 != piVar20);
        *(undefined8 *)piVar17 = *(undefined8 *)(piVar20 + 0xc);
        *(int *)(lVar15 + 0x180) = *(int *)(lVar15 + 0x180) + -1;
        *(undefined1 *)(piVar20 + 6) = 0;
LAB_00148e7c:
        unaff_w25 = 1;
        *local_78 = *local_78 & 0x3ffffff | uVar19 << 0x1a;
        goto LAB_0014885c;
      }
      lVar15 = FUN_0016d47c(param_1,piVar20);
      if (lVar15 != 0) {
        uVar11 = *(undefined8 *)(param_1 + 0x18);
        iVar10 = **(int **)(param_2 + 0x18);
        iVar9 = iVar10 + -1;
        **(int **)(param_2 + 0x18) = iVar9;
        if (iVar9 == 0 || iVar10 < 1) {
          FUN_0016aef4(uVar11);
        }
        *(long *)(param_2 + 0x18) = lVar15;
        local_78 = (uint *)(lVar15 + ((ulong)((long)puVar23 + (-0x40 - (long)piVar20)) >> 3 &
                                     0xffffffff) * 8 + 0x40);
        goto LAB_00148e7c;
      }
    }
LAB_00148e94:
    unaff_w25 = 0xffffffff;
    goto LAB_0014885c;
  }
  FUN_00143570(param_1,"not an object");
  unaff_w25 = 0xffffffff;
  goto LAB_0014885c;
LAB_001482cc:
  iVar9 = FUN_0014a424(param_1,param_2);
  iVar10 = 1;
  if (iVar9 == 0) {
    iVar10 = 2;
  }
  else {
    unaff_w25 = 0xffffffff;
  }
  if (iVar10 != 2) goto LAB_001487f8;
  goto LAB_001480f8;
LAB_001487f8:
  if (iVar10 == 0) {
LAB_001487fc:
    if (*(long *)(lVar5 + 0x28) == local_68) {
      uVar16 = FUN_00149404(param_1,param_2,param_4,local_a8,local_a0,param_7,param_8);
      return uVar16;
    }
    goto LAB_00148ec4;
  }
LAB_0014885c:
  if (*(long *)(lVar5 + 0x28) == local_68) {
    return (ulong)unaff_w25;
  }
LAB_00148ec4:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001490ec @ 001490ec [libNexusScriptRuntime69252.so] ===== */

long FUN_001490ec(long param_1,long param_2,int param_3,uint param_4)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  undefined8 uVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  ulong uVar10;
  int *piVar11;
  ulong uVar12;
  int *piVar13;
  long *plVar14;
  int *piVar15;
  int *piVar16;
  
  plVar14 = (long *)(param_2 + 0x18);
  piVar15 = (int *)*plVar14;
  if ((char)piVar15[6] != '\0') {
    lVar6 = *(long *)(param_1 + 0x18);
    uVar2 = (param_4 + (piVar15[7] + param_3) * -0x61c8ffff) * -0x61c8ffff;
    piVar16 = *(int **)(*(long *)(lVar6 + 0x188) +
                       (ulong)(uVar2 >> (ulong)(-*(int *)(lVar6 + 0x178) & 0x1f)) * 8);
    if (piVar16 != (int *)0x0) {
      do {
        if ((piVar16[7] == uVar2) && (*(long *)(piVar16 + 0xe) == *(long *)(piVar15 + 0xe))) {
          uVar1 = piVar15[10];
          uVar10 = (ulong)uVar1;
          if (piVar16[10] == uVar1 + 1) {
            if (uVar1 != 0) {
              piVar11 = piVar16 + 0x11;
              uVar12 = uVar10;
              piVar13 = piVar15 + 0x11;
              do {
                if ((*piVar11 != *piVar13) || ((uint)(piVar13[-1] ^ piVar11[-1]) >> 0x1a != 0))
                goto LAB_00149160;
                piVar11 = piVar11 + 2;
                piVar13 = piVar13 + 2;
                uVar12 = uVar12 - 1;
              } while (uVar12 != 0);
            }
            if ((piVar16[uVar10 * 2 + 0x11] == param_3) &&
               (param_4 == (uint)piVar16[uVar10 * 2 + 0x10] >> 0x1a)) break;
          }
        }
LAB_00149160:
        piVar16 = *(int **)(piVar16 + 0xc);
      } while (piVar16 != (int *)0x0);
    }
    if (piVar16 != (int *)0x0) {
      iVar4 = piVar16[9];
      if (iVar4 != piVar15[9]) {
        lVar6 = (**(code **)(lVar6 + 0x10))
                          (lVar6 + 0x20,*(undefined8 *)(param_2 + 0x20),(long)iVar4 << 4);
        if ((iVar4 != 0) && (lVar6 == 0)) {
          FUN_00138d58(param_1);
          return 0;
        }
        if (lVar6 == 0) {
          return 0;
        }
        *(long *)(param_2 + 0x20) = lVar6;
      }
      iVar4 = *piVar16;
      *plVar14 = (long)piVar16;
      uVar5 = *(undefined8 *)(param_1 + 0x18);
      *piVar16 = iVar4 + 1;
      iVar4 = *piVar15;
      iVar3 = iVar4 + -1;
      *piVar15 = iVar3;
      if (iVar3 == 0 || iVar4 < 1) {
        FUN_0016aef4(uVar5,piVar15);
      }
      goto LAB_00149314;
    }
    if (*piVar15 != 1) {
      lVar6 = FUN_0016d47c(param_1,piVar15);
      if (lVar6 == 0) {
        return 0;
      }
      lVar7 = *(long *)(param_1 + 0x18);
      lVar9 = *(long *)(lVar7 + 0x188);
      lVar8 = (ulong)(*(uint *)(lVar6 + 0x1c) >> ((ulong)(0x20 - *(int *)(lVar7 + 0x178)) & 0x3f)) *
              8;
      *(undefined8 *)(lVar6 + 0x30) = *(undefined8 *)(lVar9 + lVar8);
      *(long *)(lVar9 + lVar8) = lVar6;
      iVar4 = *(int *)(lVar7 + 0x180);
      piVar15 = (int *)*plVar14;
      uVar5 = *(undefined8 *)(param_1 + 0x18);
      *(undefined1 *)(lVar6 + 0x18) = 1;
      iVar3 = *piVar15;
      *(int *)(lVar7 + 0x180) = iVar4 + 1;
      iVar4 = iVar3 + -1;
      *piVar15 = iVar4;
      if (iVar4 == 0 || iVar3 < 1) {
        FUN_0016aef4(uVar5);
      }
      *plVar14 = lVar6;
    }
  }
  if (*(int *)*plVar14 != 1) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0x2093,"JSProperty *add_property(JSContext *, JSObject *, JSAtom, int)",
              "p->shape->header.ref_count == 1");
  }
  iVar4 = FUN_0016d598(param_1,plVar14,param_2,param_3,param_4);
  if (iVar4 != 0) {
    return 0;
  }
  piVar16 = (int *)*plVar14;
LAB_00149314:
  return *(long *)(param_2 + 0x20) + (long)piVar16[10] * 0x10 + -0x10;
}

/* ===== FUN_0014a174 @ 0014a174 [libNexusScriptRuntime69252.so] ===== */

void FUN_0014a174(long param_1,int *param_2)

{
  long lVar1;
  long *plVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  long *plVar6;
  undefined8 *puVar7;
  
  if (param_2 != (int *)0x0) {
    if (*param_2 < 1) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x14f2,"void free_var_ref(JSRuntime *, JSVarRef *)","var_ref->header.ref_count > 0"
               );
    }
    iVar3 = *param_2 + -1;
    *param_2 = iVar3;
    if (iVar3 == 0) {
      if ((*(byte *)((long)param_2 + 5) & 1) == 0) {
        lVar1 = *(long *)(param_2 + 8);
        plVar2 = *(long **)(param_2 + 10);
        *(long **)(lVar1 + 8) = plVar2;
        *plVar2 = lVar1;
        piVar5 = *(int **)(param_2 + 0xc);
        param_2[8] = 0;
        param_2[9] = 0;
        param_2[10] = 0;
        param_2[0xb] = 0;
        if (((piVar5 != (int *)0x0) && (iVar3 = *piVar5, *piVar5 = iVar3 + -1, iVar3 + -1 == 0)) &&
           (*(char *)(param_1 + 0xb8) != '\x02')) {
          plVar6 = (long *)(piVar5 + 2);
          lVar1 = *plVar6;
          plVar2 = *(long **)(piVar5 + 4);
          *(long **)(lVar1 + 8) = plVar2;
          *plVar2 = lVar1;
          *plVar6 = 0;
          piVar5[4] = 0;
          piVar5[5] = 0;
          puVar7 = *(undefined8 **)(param_1 + 0xa0);
          *(long **)(param_1 + 0xa0) = plVar6;
          *plVar6 = param_1 + 0x98;
          *(undefined8 **)(piVar5 + 4) = puVar7;
          *puVar7 = plVar6;
          if (*(char *)(param_1 + 0xb8) == '\0') {
            FUN_001419d8(param_1);
          }
        }
      }
      else {
        piVar5 = *(int **)(param_2 + 8);
        if (0xfffffff4 < (uint)*(undefined8 *)(param_2 + 10)) {
          iVar3 = *piVar5;
          iVar4 = iVar3 + -1;
          *piVar5 = iVar4;
          if (iVar4 == 0 || iVar3 < 1) {
            FUN_00141774(param_1,piVar5);
          }
        }
      }
      lVar1 = *(long *)(param_2 + 2);
      plVar2 = *(long **)(param_2 + 4);
      *(long **)(lVar1 + 8) = plVar2;
      *plVar2 = lVar1;
      param_2[2] = 0;
      param_2[3] = 0;
      param_2[4] = 0;
      param_2[5] = 0;
                    /* WARNING: Could not recover jumptable at 0x0014a28c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(param_1 + 8))(param_1 + 0x20,param_2);
      return;
    }
  }
  return;
}

/* ===== FUN_0014a59c @ 0014a59c [libNexusScriptRuntime69252.so] ===== */

undefined1  [16] FUN_0014a59c(long param_1,uint param_2)

{
  ushort *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar6;
  ulong uVar8;
  ulong uVar9;
  ushort *puVar10;
  uint uVar11;
  uint uVar12;
  int *piVar13;
  undefined1 auVar14 [16];
  undefined1 auVar15 [16];
  int *piVar5;
  undefined8 uVar7;
  
  if ((int)param_2 < 0) {
    return ZEXT416(param_2 & 0x7fffffff);
  }
  if (*(uint *)(*(long *)(param_1 + 0x18) + 0x50) <= param_2) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0xcd5,"JSValue JS_AtomIsNumericIndex1(JSContext *, JSAtom)","atom < rt->atom_size");
  }
  piVar13 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + (ulong)param_2 * 8);
  uVar9 = *(ulong *)(piVar13 + 1);
  if (uVar9 >> 0x3e != 1) goto LAB_0014a820;
  uVar11 = (uint)uVar9 & 0x7fffffff;
  puVar1 = (ushort *)(piVar13 + 4);
  uVar8 = uVar9 & 0x7fffffff;
  if ((int)(uint)uVar9 < 0) {
    if (uVar8 == 0) {
      uVar11 = 1;
LAB_0014a714:
      if (uVar11 != 0) {
LAB_0014a718:
        if (uVar11 == 2) {
LAB_0014a720:
          auVar14._8_8_ = 7;
          auVar14._0_8_ = 0x8000000000000000;
          return auVar14;
        }
        goto LAB_0014a820;
      }
    }
    else {
      uVar12 = (uint)*puVar1;
      puVar10 = puVar1;
      if (*puVar1 == 0x2d) {
        puVar10 = (ushort *)((long)piVar13 + 0x12);
        uVar12 = (uint)*puVar10;
        if ((*puVar10 == 0x30) && (uVar11 == 2)) goto LAB_0014a714;
      }
      if (uVar12 - 0x3a < 0xfffffff6) {
        uVar11 = 1;
        if ((uVar12 != 0x49) ||
           ((byte *)((long)puVar1 + (uVar8 * 2 - (long)puVar10)) != (byte *)0x10))
        goto LAB_0014a714;
        uVar11 = (uint)(*(long *)(puVar10 + 1) != 0x6e00690066006e ||
                       *(long *)(puVar10 + 4) != 0x7900740069006e);
        if (uVar11 == 0) goto LAB_0014a734;
        goto LAB_0014a718;
      }
    }
  }
  else {
    if (uVar8 == 0) goto LAB_0014a820;
    uVar12 = (uint)(byte)*puVar1;
    puVar10 = puVar1;
    if ((byte)*puVar1 == 0x2d) {
      puVar10 = (ushort *)((long)piVar13 + 0x11);
      uVar12 = (uint)*(byte *)puVar10;
      if ((*(byte *)puVar10 == 0x30) && (uVar11 == 2)) goto LAB_0014a720;
    }
    if (uVar12 - 0x3a < 0xfffffff6) {
      if (uVar12 != 0x49) {
        return ZEXT816(3) << 0x40;
      }
      if ((byte *)((long)puVar1 + (uVar8 - (long)puVar10)) != (byte *)0x8) {
        return ZEXT816(3) << 0x40;
      }
      if (*(int *)((long)puVar10 + 1) != 0x6e69666e || *(int *)(puVar10 + 2) != 0x7974696e)
      goto LAB_0014a820;
    }
  }
LAB_0014a734:
  *piVar13 = *piVar13 + 1;
  auVar14 = FUN_0016db18(param_1,piVar13,0xfffffffffffffff9,0);
  uVar7 = auVar14._8_8_;
  piVar5 = auVar14._0_8_;
  if (auVar14._8_4_ == 6) {
    return auVar14;
  }
  auVar15 = FUN_0014c3ec(param_1,piVar5,uVar7,0);
  piVar6 = auVar15._0_8_;
  if (auVar15._8_4_ == 6) {
    if (auVar14._8_4_ < 0xfffffff5) {
      return auVar15;
    }
    iVar4 = *piVar5;
    iVar2 = iVar4 + -1;
    *piVar5 = iVar2;
    if (iVar2 != 0 && 0 < iVar4) {
      return auVar15;
    }
    FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar5,uVar7);
    return auVar15;
  }
  iVar4 = FUN_0016cf8c(piVar13,piVar6);
  if ((0xfffffff4 < auVar15._8_4_) &&
     (iVar2 = *piVar6, iVar3 = iVar2 + -1, *piVar6 = iVar3, iVar3 == 0 || iVar2 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar6,auVar15._8_8_);
  }
  if (iVar4 == 0) {
    return auVar14;
  }
  FUN_0013a308(param_1,piVar5,uVar7);
LAB_0014a820:
  return ZEXT816(3) << 0x40;
}

/* ===== FUN_0014b0d4 @ 0014b0d4 [libNexusScriptRuntime69252.so] ===== */

undefined4 FUN_0014b0d4(long param_1,long param_2,uint param_3)

{
  long lVar1;
  undefined4 *puVar2;
  undefined8 *puVar3;
  uint uVar4;
  uint uVar5;
  size_t __n;
  undefined8 *puVar6;
  long lVar7;
  bool bVar8;
  int *piVar9;
  int iVar10;
  int iVar11;
  undefined8 uVar12;
  void *__s;
  int *piVar13;
  uint uVar14;
  long lVar15;
  long *plVar16;
  code *pcVar17;
  long *plVar18;
  ulong uVar19;
  int *piVar20;
  undefined8 *puVar21;
  int *piVar22;
  uint *puVar23;
  uint *puVar24;
  undefined8 *puVar25;
  uint uVar26;
  undefined4 unaff_w21;
  undefined8 *puVar27;
  ulong uVar28;
  uint *puVar29;
  undefined8 uVar30;
  undefined8 uVar31;
  undefined8 uVar32;
  undefined8 uVar33;
  undefined8 uVar34;
  undefined8 uVar35;
  uint local_6c;
  long local_68;
  
  lVar7 = tpidr_el0;
  local_68 = *(long *)(lVar7 + 0x28);
LAB_0014b114:
  piVar13 = *(int **)(param_2 + 0x18);
  uVar4 = piVar13[8];
  if (piVar13[~(ulong)(uVar4 & param_3)] != 0) {
    uVar28 = (ulong)(uint)piVar13[~(ulong)(uVar4 & param_3)] - 1;
    piVar9 = piVar13 + 0x10;
    puVar24 = (uint *)(piVar9 + uVar28 * 2);
    if (puVar24[1] != param_3) {
      while (puVar29 = puVar24, (int)((ulong)*puVar29 & 0x3ffffff) != 0) {
        uVar28 = ((ulong)*puVar29 & 0x3ffffff) - 1;
        puVar24 = (uint *)(piVar9 + uVar28 * 2);
        if (puVar24[1] == param_3) goto LAB_0014b27c;
      }
      goto LAB_0014b174;
    }
    puVar29 = (uint *)0x0;
LAB_0014b27c:
    if ((*(byte *)((long)puVar24 + 3) >> 2 & 1) == 0) {
      unaff_w21 = 0;
      goto LAB_0014b618;
    }
    uVar19 = 0;
    if (puVar29 != (uint *)0x0) {
      uVar19 = (ulong)((long)puVar29 - (long)piVar9) >> 3 & 0xffffffff;
    }
    if ((char)piVar13[6] == '\0') goto LAB_0014b314;
    if (*piVar13 == 1) {
      lVar15 = *(long *)(param_1 + 0x18);
      piVar9 = (int *)(*(long *)(lVar15 + 0x188) +
                      (ulong)((uint)piVar13[7] >> ((ulong)(0x20 - *(int *)(lVar15 + 0x178)) & 0x3f))
                      * 8);
      do {
        piVar20 = piVar9;
        piVar22 = *(int **)piVar20;
        piVar9 = piVar22 + 0xc;
      } while (piVar22 != piVar13);
      *(undefined8 *)piVar20 = *(undefined8 *)(piVar13 + 0xc);
      *(int *)(lVar15 + 0x180) = *(int *)(lVar15 + 0x180) + -1;
      *(undefined1 *)(piVar13 + 6) = 0;
    }
    else {
      lVar15 = FUN_0016d47c(param_1);
      if (lVar15 == 0) {
        unaff_w21 = 0xffffffff;
        goto LAB_0014b618;
      }
      uVar12 = *(undefined8 *)(param_1 + 0x18);
      iVar10 = **(int **)(param_2 + 0x18);
      iVar11 = iVar10 + -1;
      **(int **)(param_2 + 0x18) = iVar11;
      if (iVar11 == 0 || iVar10 < 1) {
        FUN_0016aef4(uVar12);
      }
      puVar24 = (uint *)(lVar15 + (uVar28 & 0xffffffff) * 8 + 0x40);
      *(long *)(param_2 + 0x18) = lVar15;
    }
LAB_0014b314:
    lVar15 = *(long *)(param_2 + 0x18);
    if (puVar29 == (uint *)0x0) {
      *(uint *)(lVar15 + ~(ulong)(uVar4 & param_3) * 4) = *puVar24 & 0x3ffffff;
    }
    else {
      lVar1 = lVar15 + uVar19 * 8;
      *(uint *)(lVar1 + 0x40) = *(uint *)(lVar1 + 0x40) & 0xfc000000 | *puVar24 & 0x3ffffff;
    }
    *(int *)(lVar15 + 0x2c) = *(int *)(lVar15 + 0x2c) + 1;
    puVar2 = (undefined4 *)(*(long *)(param_2 + 0x20) + uVar28 * 0x10);
    FUN_0016b6c8(*(undefined8 *)(param_1 + 0x18),puVar2,*puVar24 >> 0x1a);
    if ((0xe3 < (int)puVar24[1]) &&
       (piVar13 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + (ulong)puVar24[1] * 8),
       iVar10 = *piVar13, iVar11 = iVar10 + -1, *piVar13 = iVar11, iVar11 == 0 || iVar10 < 1)) {
      FUN_001418dc();
    }
    *puVar24 = *puVar24 & 0x3ffffff;
    puVar24[1] = 0;
    *puVar2 = 0;
    *(undefined8 *)(puVar2 + 2) = 3;
    if ((7 < (int)*(uint *)(lVar15 + 0x2c)) &&
       (*(uint *)(lVar15 + 0x28) >> 1 <= *(uint *)(lVar15 + 0x2c))) {
      puVar27 = *(undefined8 **)(param_2 + 0x18);
      if (*(char *)(puVar27 + 3) != '\0') {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0x122d,"int compact_properties(JSContext *, JSObject *)","!sh->is_hashed");
      }
      uVar4 = *(int *)(puVar27 + 5) - *(int *)((long)puVar27 + 0x2c);
      if ((int)uVar4 < 3) {
        uVar4 = 2;
      }
      if (*(uint *)((long)puVar27 + 0x24) < uVar4) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0x1231,"int compact_properties(JSContext *, JSObject *)",
                  "new_size <= sh->prop_size");
      }
      uVar26 = *(int *)(puVar27 + 4) + 1;
      do {
        uVar14 = uVar26;
        uVar26 = uVar14 >> 1;
      } while (uVar4 <= uVar14 >> 1);
      __n = (ulong)uVar14 * 4;
      __s = (void *)(*(code *)**(undefined8 **)(param_1 + 0x18))
                              (*(undefined8 **)(param_1 + 0x18) + 4,__n + (ulong)uVar4 * 8 + 0x40);
      if (__s == (void *)0x0) {
        FUN_00138d58(param_1);
      }
      else if (__s != (void *)0x0) {
        lVar15 = puVar27[1];
        plVar16 = (long *)puVar27[2];
        puVar3 = (undefined8 *)((long)__s + (ulong)uVar14 * 4);
        *(long **)(lVar15 + 8) = plVar16;
        *plVar16 = lVar15;
        puVar27[1] = 0;
        puVar27[2] = 0;
        uVar31 = puVar27[4];
        uVar30 = puVar27[7];
        uVar12 = puVar27[6];
        uVar35 = puVar27[1];
        uVar34 = *puVar27;
        uVar33 = puVar27[3];
        uVar32 = puVar27[2];
        puVar3[5] = puVar27[5];
        puVar3[4] = uVar31;
        puVar3[7] = uVar30;
        puVar3[6] = uVar12;
        puVar3[1] = uVar35;
        *puVar3 = uVar34;
        puVar3[3] = uVar33;
        puVar3[2] = uVar32;
        plVar16 = (long *)(*(long *)(param_1 + 0x18) + 0x88);
        lVar15 = *plVar16;
        plVar18 = puVar3 + 1;
        *plVar18 = lVar15;
        *(long **)(lVar15 + 8) = plVar18;
        puVar3[2] = plVar16;
        *plVar16 = (long)plVar18;
        memset(__s,0,__n);
        if (*(int *)(puVar3 + 5) == 0) {
          uVar28 = 0;
          uVar26 = 0;
        }
        else {
          puVar21 = *(undefined8 **)(param_2 + 0x20);
          uVar19 = 0;
          uVar28 = 0;
          puVar24 = (uint *)((long)puVar27 + 0x44);
          puVar29 = (uint *)(puVar3 + 8);
          puVar25 = puVar21;
          do {
            puVar23 = puVar29;
            if (*puVar24 != 0) {
              puVar29[1] = *puVar24;
              uVar5 = puVar24[-1];
              *puVar29 = uVar5 & 0xfc000000 | *puVar29 & 0x3ffffff;
              lVar15 = ~(ulong)(*puVar24 & uVar14 - 1) * 4;
              uVar26 = (int)uVar28 + 1;
              puVar23 = puVar29 + 2;
              *puVar29 = uVar5 & 0xfc000000 | *(uint *)((long)puVar3 + lVar15) & 0x3ffffff;
              *(uint *)((long)puVar3 + lVar15) = uVar26;
              uVar12 = *puVar25;
              puVar6 = puVar21 + uVar28 * 2;
              puVar6[1] = puVar25[1];
              *puVar6 = uVar12;
              uVar28 = (ulong)uVar26;
            }
            uVar26 = *(uint *)(puVar3 + 5);
            uVar19 = uVar19 + 1;
            puVar25 = puVar25 + 2;
            puVar24 = puVar24 + 2;
            puVar29 = puVar23;
          } while (uVar19 < uVar26);
        }
        if ((int)uVar28 != uVar26 - *(int *)((long)puVar3 + 0x2c)) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x1256,"int compact_properties(JSContext *, JSObject *)",
                    "j == (sh->prop_count - sh->deleted_prop_count)");
        }
        *(uint *)(puVar3 + 4) = uVar14 - 1;
        *(uint *)((long)puVar3 + 0x24) = uVar4;
        *(int *)(puVar3 + 5) = (int)uVar28;
        *(undefined4 *)((long)puVar3 + 0x2c) = 0;
        *(undefined8 **)(param_2 + 0x18) = puVar3;
        (**(code **)(*(long *)(param_1 + 0x18) + 8))
                  (*(long *)(param_1 + 0x18) + 0x20,
                   (long)puVar27 + ~(ulong)*(uint *)(puVar27 + 4) * 4);
        lVar15 = (**(code **)(*(long *)(param_1 + 0x18) + 0x10))
                           (*(long *)(param_1 + 0x18) + 0x20,*(undefined8 *)(param_2 + 0x20),
                            (ulong)uVar4 << 4);
        if (lVar15 == 0) {
          FUN_00138d58(param_1);
          unaff_w21 = 1;
        }
        else {
          unaff_w21 = 1;
          if (lVar15 != 0) {
            unaff_w21 = 1;
            *(long *)(param_2 + 0x20) = lVar15;
          }
        }
        goto LAB_0014b618;
      }
    }
    goto LAB_0014b614;
  }
LAB_0014b174:
  if ((*(byte *)(param_2 + 5) >> 2 & 1) == 0) goto LAB_0014b614;
  if ((*(byte *)(param_2 + 5) >> 3 & 1) == 0) {
    lVar15 = *(long *)(*(long *)(*(long *)(param_1 + 0x18) + 0x70) +
                       (ulong)*(ushort *)(param_2 + 6) * 0x28 + 0x20);
    if ((lVar15 == 0) || (pcVar17 = *(code **)(lVar15 + 0x10), pcVar17 == (code *)0x0)) {
      bVar8 = true;
    }
    else {
      unaff_w21 = (*pcVar17)(param_1,param_2,0xffffffffffffffff,param_3);
      bVar8 = false;
    }
    if (bVar8) goto LAB_0014b614;
    goto LAB_0014b618;
  }
  iVar10 = FUN_0016d090(param_1,&local_6c,param_3);
  if (iVar10 == 0) {
    iVar10 = 0;
  }
  else if (local_6c < *(uint *)(param_2 + 0x40)) {
    if ((*(short *)(param_2 + 6) == 8) || (*(short *)(param_2 + 6) == 2)) {
      if (local_6c != *(uint *)(param_2 + 0x40) - 1) goto LAB_0014b238;
      puVar27 = (undefined8 *)(*(long *)(param_2 + 0x38) + (ulong)local_6c * 0x10);
      piVar13 = (int *)*puVar27;
      if ((0xfffffff4 < (uint)puVar27[1]) &&
         (iVar10 = *piVar13, iVar11 = iVar10 + -1, *piVar13 = iVar11, iVar11 == 0 || iVar10 < 1)) {
        FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar13);
      }
      iVar10 = 1;
      unaff_w21 = 1;
      *(uint *)(param_2 + 0x40) = local_6c;
    }
    else {
      unaff_w21 = 0;
      iVar10 = 1;
    }
  }
  else {
    iVar10 = 0;
  }
  goto LAB_0014b26c;
LAB_0014b238:
  iVar11 = FUN_0014a424(param_1,param_2);
  iVar10 = 1;
  if (iVar11 == 0) {
    iVar10 = 2;
  }
  else {
    unaff_w21 = 0xffffffff;
  }
  if (iVar10 != 2) {
LAB_0014b26c:
    if (iVar10 == 0) {
LAB_0014b614:
      unaff_w21 = 1;
    }
LAB_0014b618:
    if (*(long *)(lVar7 + 0x28) == local_68) {
      return unaff_w21;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  goto LAB_0014b114;
}

/* ===== FUN_0014c3ec @ 0014c3ec [libNexusScriptRuntime69252.so] ===== */

undefined1  [16] FUN_0014c3ec(long param_1,int *param_2,ulong param_3,int param_4)

{
  int iVar1;
  int iVar2;
  long lVar3;
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  size_t sVar6;
  undefined8 uVar7;
  uint uVar8;
  undefined4 uVar9;
  ulong uVar10;
  code *pcVar11;
  long lVar12;
  int *piVar13;
  undefined1 auVar14 [16];
  undefined1 auVar15 [16];
  int *local_c8 [16];
  long local_48;
  char *__s;
  
  auVar4._8_8_ = param_3;
  auVar4._0_8_ = param_2;
  lVar3 = tpidr_el0;
  local_48 = *(long *)(lVar3 + 0x28);
  uVar8 = (uint)param_3;
  if (0x12 < uVar8 + 0xb) {
switchD_0014c450_caseD_fffffffa:
    __s = "[unsupported type]";
    goto switchD_0014c450_caseD_fffffffe;
  }
  uVar10 = 0;
  auVar5._8_8_ = 0;
  auVar5._0_8_ = param_3 & 0xffffffff;
  __s = "[function bytecode]";
  auVar14 = auVar5 << 0x40;
  switch(uVar8) {
  case 0:
    __s = (char *)local_c8;
    snprintf((char *)local_c8,0x20,"%d");
    goto switchD_0014c450_caseD_fffffffe;
  case 1:
    uVar9 = 2;
    if ((int)param_2 != 0) {
      uVar9 = 3;
    }
    auVar14 = FUN_00140044(param_1,uVar9,1);
    goto LAB_0014c640;
  case 2:
    if (*(uint *)(*(long *)(param_1 + 0x18) + 0x50) < 2) {
code_r0x0014c6a4:
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0xc94,"JSValue __JS_AtomToValue(JSContext *, JSAtom, BOOL)","atom < rt->atom_size")
      ;
    }
    lVar12 = *(long *)(*(long *)(param_1 + 0x18) + 0x60);
    piVar13 = *(int **)(lVar12 + 8);
    goto LAB_0014c4a0;
  case 3:
    if (*(uint *)(*(long *)(param_1 + 0x18) + 0x50) < 0x48) goto code_r0x0014c6a4;
    lVar12 = *(long *)(*(long *)(param_1 + 0x18) + 0x60);
    piVar13 = *(int **)(lVar12 + 0x238);
LAB_0014c4a0:
    if (*(ulong *)(piVar13 + 1) >> 0x3e != 1 && (*(ulong *)(piVar13 + 1) & 0xffffffff) == 0x80000000
       ) {
      piVar13 = *(int **)(lVar12 + 0x178);
    }
    auVar14._8_8_ = 0xfffffffffffffff9;
    auVar14._0_8_ = piVar13;
    uVar10 = (ulong)piVar13 & 0xffffffff00000000;
    *piVar13 = *piVar13 + 1;
  case 6:
    goto switchD_0014c450_caseD_6;
  case 7:
    FUN_0016efc4(param_2,local_c8,10,0,0);
    sVar6 = strlen((char *)local_c8);
    __s = (char *)local_c8;
    goto LAB_0014c63c;
  case 0xfffffff5:
    pcVar11 = *(code **)(*(long *)(param_1 + 0x18) + 0x278);
    break;
  case 0xfffffff6:
    pcVar11 = *(code **)(*(long *)(param_1 + 0x18) + 0x208);
    break;
  case 0xfffffff7:
    pcVar11 = *(code **)(*(long *)(param_1 + 0x18) + 0x240);
    break;
  case 0xfffffff8:
    if (param_4 == 0) {
      FUN_00143570(param_1,"cannot convert symbol to string");
      uVar10 = 0;
      auVar14 = ZEXT816(6) << 0x40;
      goto switchD_0014c450_caseD_6;
    }
  case 0xfffffff9:
    if (0xfffffff4 < uVar8) {
      *param_2 = *param_2 + 1;
    }
    uVar10 = (ulong)param_2 & 0xffffffff00000000;
    local_c8[0] = param_2;
    auVar14 = auVar4;
    goto switchD_0014c450_caseD_6;
  default:
    goto switchD_0014c450_caseD_fffffffa;
  case 0xfffffffe:
    goto switchD_0014c450_caseD_fffffffe;
  case 0xffffffff:
    if (0xfffffff4 < uVar8) {
      *param_2 = *param_2 + 1;
    }
    local_c8[0] = param_2;
    auVar14 = FUN_0016de9c(param_1,param_2,param_3,0);
    uVar7 = auVar14._8_8_;
    piVar13 = auVar14._0_8_;
    uVar8 = auVar14._8_4_;
    if (((uVar8 != 6) &&
        (auVar14 = FUN_0014c3ec(param_1,piVar13,uVar7,param_4), local_c8[0] = piVar13,
        0xfffffff4 < uVar8)) &&
       (iVar1 = *piVar13, iVar2 = iVar1 + -1, *piVar13 = iVar2, iVar2 == 0 || iVar1 < 1)) {
      FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar13,uVar7);
    }
    uVar10 = auVar14._0_8_ & 0xffffffff00000000;
    goto switchD_0014c450_caseD_6;
  }
  auVar14 = (*pcVar11)(param_1,param_2);
LAB_0014c640:
  uVar10 = auVar14._0_8_ & 0xffffffff00000000;
switchD_0014c450_caseD_6:
  if (*(long *)(lVar3 + 0x28) == local_48) {
    auVar15._0_8_ = uVar10 | auVar14._0_8_ & 0xffffffff;
    auVar15._8_8_ = auVar14._8_8_;
    return auVar15;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
switchD_0014c450_caseD_fffffffe:
  sVar6 = strlen(__s);
LAB_0014c63c:
  auVar14 = FUN_0013f444(param_1,__s,sVar6);
  goto LAB_0014c640;
}

/* ===== FUN_00159d48 @ 00159d48 [libNexusScriptRuntime69252.so] ===== */

undefined1  [16]
FUN_00159d48(long param_1,int *param_2,undefined8 param_3,undefined8 param_4,undefined8 param_5,
            undefined8 param_6,undefined8 param_7)

{
  int iVar1;
  long lVar2;
  int iVar3;
  uint uVar4;
  int *piVar5;
  ulong uVar6;
  undefined8 uVar7;
  undefined1 auVar8 [16];
  undefined1 auVar9 [16];
  undefined1 auVar10 [12];
  int *local_70;
  undefined8 local_68;
  int *local_60;
  undefined8 local_58;
  long local_50;
  long local_48;
  
  lVar2 = tpidr_el0;
  local_48 = *(long *)(lVar2 + 0x28);
  uVar4 = (uint)param_3;
  local_60 = param_2;
  if (uVar4 == 0xfffffffd) {
    iVar3 = *param_2;
    iVar1 = iVar3 + -1;
    *param_2 = iVar1;
    if (iVar1 == 0 || iVar3 < 1) {
      FUN_00141774(*(undefined8 *)(param_1 + 0x18),param_2);
    }
    iVar3 = FUN_00178684(param_1,param_2);
    if (-1 < iVar3) {
      uVar4 = (uint)param_2[0x20] >> 0x18;
      if (((uVar4 != 0) && (1 < uVar4 - 4)) && (uVar4 != 2)) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0x6fa2,"int js_link_module(JSContext *, JSModuleDef *)",
                  "m->status == JS_MODULE_STATUS_UNLINKED || m->status == JS_MODULE_STATUS_LINKED || m->status == JS_MODULE_STATUS_EVALUATING_ASYNC || m->status == JS_MODULE_STATUS_EVALUATED"
                 );
      }
      local_60 = (int *)0x0;
      iVar3 = FUN_00178b00(param_1,param_2,&local_60,0);
      if (iVar3 < 0) {
        for (; local_60 != (int *)0x0; local_60 = *(int **)(local_60 + 0x24)) {
          if ((uint)local_60[0x20] >> 0x18 != 1) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0x6fa7,"int js_link_module(JSContext *, JSModuleDef *)",
                      "m1->status == JS_MODULE_STATUS_LINKING");
          }
          local_60[0x20] = local_60[0x20] & 0xffffff;
        }
      }
      else {
        if (local_60 != (int *)0x0) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x6fad,"int js_link_module(JSContext *, JSModuleDef *)","stack_top == NULL");
        }
        if ((1 < ((uint)param_2[0x20] >> 0x18) - 4) && ((uint)param_2[0x20] >> 0x18 != 2)) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x6fb0,"int js_link_module(JSContext *, JSModuleDef *)",
                    "m->status == JS_MODULE_STATUS_LINKED || m->status == JS_MODULE_STATUS_EVALUATING_ASYNC || m->status == JS_MODULE_STATUS_EVALUATED"
                   );
        }
      }
      local_60 = (int *)0x0;
      if (-1 < iVar3) {
        uVar4 = (uint)param_2[0x20] >> 0x18;
        if ((1 < uVar4 - 4) && (uVar4 != 2)) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x720e,"JSValue js_evaluate_module(JSContext *, JSModuleDef *)",
                    "m->status == JS_MODULE_STATUS_LINKED || m->status == JS_MODULE_STATUS_EVALUATING_ASYNC || m->status == JS_MODULE_STATUS_EVALUATED"
                   );
        }
        if ((uint)param_2[0x20] >> 0x19 == 2) {
          param_2 = *(int **)(param_2 + 0x2e);
        }
        uVar7 = *(undefined8 *)(param_2 + 0x32);
        uVar4 = (uint)uVar7;
        if (uVar4 == 3) {
          auVar9 = FUN_00160770(param_1,param_2 + 0x34,0,3);
          *(undefined1 (*) [16])(param_2 + 0x30) = auVar9;
          if (auVar9._8_4_ != 6) {
            local_50 = 0;
            iVar3 = FUN_0017914c(param_1,param_2,0,&local_50,&local_60);
            if (iVar3 < 0) {
              if (local_50 != 0) {
                do {
                  if (*(uint *)(local_50 + 0x80) >> 0x18 != 3) {
                    /* WARNING: Subroutine does not return */
                    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                              ,0x721e,"JSValue js_evaluate_module(JSContext *, JSModuleDef *)",
                              "m1->status == JS_MODULE_STATUS_EVALUATING");
                  }
                  *(undefined1 *)(local_50 + 0xf0) = 1;
                  *(uint *)(local_50 + 0x80) = *(uint *)(local_50 + 0x80) & 0xffffff | 0x5000000;
                  if (0xfffffff4 < (uint)local_58) {
                    *local_60 = *local_60 + 1;
                  }
                  *(int **)(local_50 + 0xf8) = local_60;
                  *(undefined8 *)(local_50 + 0x100) = local_58;
                  *(int **)(local_50 + 0xb8) = param_2;
                  local_50 = *(long *)(local_50 + 0x90);
                  local_70 = local_60;
                } while (local_50 != 0);
              }
              local_70 = local_60;
              if ((0xfffffff4 < (uint)local_58) &&
                 (iVar3 = *local_60, iVar1 = iVar3 + -1, *local_60 = iVar1, iVar1 == 0 || iVar3 < 1)
                 ) {
                FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_60);
              }
              if (*(char *)((long)param_2 + 0x83) != '\x05') {
                    /* WARNING: Subroutine does not return */
                __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                          ,0x7226,"JSValue js_evaluate_module(JSContext *, JSModuleDef *)",
                          "m->status == JS_MODULE_STATUS_EVALUATED");
              }
              if ((char)param_2[0x3c] == '\0') {
                    /* WARNING: Subroutine does not return */
                __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                          ,0x7227,"JSValue js_evaluate_module(JSContext *, JSModuleDef *)",
                          "m->eval_has_exception");
              }
              auVar10 = FUN_0014cb88(param_1,*(undefined8 *)(param_2 + 0x38),
                                     *(undefined8 *)(param_2 + 0x3a),0,3,0,3,1,param_2 + 0x3e,2);
              local_70 = auVar10._0_8_;
              if ((0xfffffff4 < auVar10._8_4_) &&
                 (iVar3 = *local_70, iVar1 = iVar3 + -1, *local_70 = iVar1, iVar1 == 0 || iVar3 < 1)
                 ) {
                FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_70);
              }
            }
            else {
              if ((uint)param_2[0x20] >> 0x19 != 2) {
                    /* WARNING: Subroutine does not return */
                __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                          ,0x722d,"JSValue js_evaluate_module(JSContext *, JSModuleDef *)",
                          "m->status == JS_MODULE_STATUS_EVALUATING_ASYNC || m->status == JS_MODULE_STATUS_EVALUATED"
                         );
              }
              if ((char)param_2[0x3c] != '\0') {
                    /* WARNING: Subroutine does not return */
                __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                          ,0x722e,"JSValue js_evaluate_module(JSContext *, JSModuleDef *)",
                          "!m->eval_has_exception");
              }
              if (param_2[0x2b] == 0) {
                if ((uint)param_2[0x20] >> 0x18 != 5) {
                    /* WARNING: Subroutine does not return */
                  __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                            ,0x7231,"JSValue js_evaluate_module(JSContext *, JSModuleDef *)",
                            "m->status == JS_MODULE_STATUS_EVALUATED");
                }
                local_68 = 3;
                local_70 = (int *)((ulong)local_70 & 0xffffffff00000000);
                auVar10 = FUN_0014cb88(param_1,*(undefined8 *)(param_2 + 0x34),
                                       *(undefined8 *)(param_2 + 0x36),0,3,0,3,1,&local_70,2);
                piVar5 = auVar10._0_8_;
                if ((0xfffffff4 < auVar10._8_4_) &&
                   (iVar3 = *piVar5, iVar1 = iVar3 + -1, *piVar5 = iVar1, iVar1 == 0 || iVar3 < 1))
                {
                  FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar5);
                }
              }
              if (local_50 != 0) {
                    /* WARNING: Subroutine does not return */
                __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                          ,0x7237,"JSValue js_evaluate_module(JSContext *, JSModuleDef *)",
                          "stack_top == NULL");
              }
            }
            piVar5 = *(int **)(param_2 + 0x30);
            uVar7 = *(undefined8 *)(param_2 + 0x32);
            uVar4 = (uint)uVar7;
            local_70 = piVar5;
            goto joined_r0x00159fd8;
          }
          piVar5 = (int *)0x0;
          uVar6 = 0;
          uVar7 = 6;
        }
        else {
          piVar5 = *(int **)(param_2 + 0x30);
          local_60 = piVar5;
joined_r0x00159fd8:
          if (0xfffffff4 < uVar4) {
            *piVar5 = *piVar5 + 1;
          }
          uVar6 = (ulong)piVar5 & 0xffffffff00000000;
        }
        auVar9._8_8_ = uVar7;
        auVar9._0_8_ = uVar6 | (ulong)piVar5 & 0xffffffff;
        if ((int)uVar7 != 6) goto LAB_00159f9c;
      }
    }
  }
  else {
    if (uVar4 == 0xfffffffe) {
      auVar8 = FUN_00170348(param_1,param_2,param_3,param_6,param_7);
      piVar5 = auVar8._0_8_;
      auVar9 = FUN_0014cb88(param_1,piVar5,auVar8._8_8_,param_4,param_5,0,3,0,0,2,piVar5);
      local_60 = piVar5;
      if ((0xfffffff4 < auVar8._8_4_) &&
         (iVar3 = *piVar5, iVar1 = iVar3 + -1, *piVar5 = iVar1, iVar1 == 0 || iVar3 < 1)) {
        FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar5,auVar8._8_8_);
      }
      goto LAB_00159f9c;
    }
    if ((0xfffffff4 < uVar4) &&
       (iVar3 = *param_2, iVar1 = iVar3 + -1, *param_2 = iVar1, iVar1 == 0 || iVar3 < 1)) {
      FUN_00141774(*(undefined8 *)(param_1 + 0x18),param_2);
    }
    FUN_00143570(param_1,"bytecode function expected");
  }
  auVar9 = ZEXT816(6) << 0x40;
LAB_00159f9c:
  if (*(long *)(lVar2 + 0x28) == local_48) {
    return auVar9;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0015a37c @ 0015a37c [libNexusScriptRuntime69252.so] ===== */

undefined8 FUN_0015a37c(long param_1)

{
  undefined8 uVar1;
  uint in_w6;
  
  if ((in_w6 >> 1 & 1) != 0) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0x8795,
              "JSValue JS_EvalThis(JSContext *, JSValue, const char *, size_t, const char *, int)",
              "eval_type == JS_EVAL_TYPE_GLOBAL || eval_type == JS_EVAL_TYPE_MODULE");
  }
  if (*(code **)(param_1 + 0x1f0) != (code *)0x0) {
    uVar1 = (**(code **)(param_1 + 0x1f0))();
    return uVar1;
  }
  FUN_00143570(param_1,"eval is not supported");
  return 0;
}

/* ===== FUN_0015a3dc @ 0015a3dc [libNexusScriptRuntime69252.so] ===== */

undefined8
FUN_0015a3dc(long param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,ulong param_5,
            undefined8 param_6)

{
  undefined8 uVar1;
  
  if (((uint)param_5 >> 1 & 1) != 0) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0x8795,
              "JSValue JS_EvalThis(JSContext *, JSValue, const char *, size_t, const char *, int)",
              "eval_type == JS_EVAL_TYPE_GLOBAL || eval_type == JS_EVAL_TYPE_MODULE",param_5,param_6
              ,param_5 & 0xffffffff);
  }
  if (*(code **)(param_1 + 0x1f0) != (code *)0x0) {
    uVar1 = (**(code **)(param_1 + 0x1f0))
                      (param_1,*(undefined8 *)(param_1 + 0x178),*(undefined8 *)(param_1 + 0x180),
                       param_2,param_3,param_4,param_5 & 0xffffffff,0xffffffff);
    return uVar1;
  }
  FUN_00143570(param_1,"eval is not supported");
  return 0;
}

/* ===== FUN_0015aa08 @ 0015aa08 [libNexusScriptRuntime69252.so] ===== */

void FUN_0015aa08(long *param_1,ulong param_2,ulong param_3)

{
  long *plVar1;
  uint uVar2;
  long lVar3;
  ulong uVar4;
  bool bVar5;
  bool bVar6;
  int iVar7;
  undefined8 uVar8;
  char *pcVar9;
  int iVar10;
  ulong uVar11;
  ulong *puVar12;
  uint uVar13;
  long lVar14;
  uint uVar15;
  ulong uVar16;
  ulong uVar17;
  ulong uVar18;
  ulong uVar19;
  int local_78;
  ulong local_70;
  long local_68;
  
  lVar3 = tpidr_el0;
  local_68 = *(long *)(lVar3 + 0x28);
  lVar14 = *param_1;
  if (&stack0xffffffffffffffa0 < *(undefined1 **)(*(long *)(lVar14 + 0x18) + 0xd8)) {
    pcVar9 = "stack overflow";
LAB_0015aa58:
    FUN_001405ec(lVar14,pcVar9);
    goto LAB_0015aa5c;
  }
  iVar10 = (int)param_3;
  switch(iVar10) {
  case 0:
    FUN_001d27c4(param_1 + 1,5);
    local_78 = (int)param_2;
    uVar15 = local_78 << 1 ^ local_78 >> 0x1f;
    uVar13 = uVar15;
    if (0x7f < uVar15) {
      do {
        uVar15 = uVar13 >> 7;
        FUN_001d27c4(param_1 + 1,uVar13 | 0xffffff80);
        uVar2 = uVar13 >> 0xe;
        uVar13 = uVar15;
      } while (uVar2 != 0);
    }
    uVar15 = uVar15 & 0x7f;
    break;
  case 1:
    local_78._0_1_ = (byte)param_2;
    uVar15 = (byte)local_78 + 3;
    break;
  case 2:
    uVar15 = 1;
    break;
  case 3:
    uVar15 = 2;
    break;
  case 7:
    FUN_001d27c4(param_1 + 1,6);
    local_70 = param_2;
    if ((char)param_1[7] != '\0') {
      uVar11 = (param_2 & 0xff00ff00ff00ff00) >> 8 | (param_2 & 0xff00ff00ff00ff) << 8;
      uVar11 = (uVar11 & 0xffff0000ffff0000) >> 0x10 | (uVar11 & 0xffff0000ffff) << 0x10;
      local_70 = uVar11 >> 0x20 | uVar11 << 0x20;
    }
    FUN_001d26dc(param_1 + 1,&local_70,8);
    uVar8 = 0;
    goto LAB_0015aa60;
  case -0xb:
  case -10:
  case -9:
    if (2 < iVar10 + 0xbU) {
                    /* WARNING: Subroutine does not return */
      abort();
    }
    plVar1 = param_1 + 1;
    FUN_001d27c4(plVar1,0xb0a0c >> (ulong)((uint)(((ulong)(iVar10 + 0xbU) & 0x1fffff) << 3) & 0x1f))
    ;
    lVar14 = *(long *)(param_2 + 0x18);
    if (lVar14 == -0x8000000000000000) {
      lVar14 = 0;
    }
    else if (lVar14 == 0x7fffffffffffffff) {
      lVar14 = 2;
    }
    else if (lVar14 == 0x7ffffffffffffffe) {
      lVar14 = 1;
    }
    else if (-1 < lVar14) {
      lVar14 = lVar14 + 3;
    }
    uVar11 = (long)*(int *)(param_2 + 0x10) | lVar14 << 1;
    iVar7 = (int)uVar11;
    if (uVar11 != (long)iVar7) {
      lVar14 = *param_1;
      pcVar9 = "bignum exponent is too large";
      goto LAB_0015aa58;
    }
    uVar15 = iVar7 << 1 ^ iVar7 >> 0x1f;
    uVar13 = uVar15;
    if (0x7f < uVar15) {
      do {
        uVar15 = uVar13 >> 7;
        FUN_001d27c4(plVar1,uVar13 | 0xffffff80);
        uVar2 = uVar13 >> 0xe;
        uVar13 = uVar15;
      } while (uVar2 != 0);
    }
    FUN_001d27c4(plVar1,uVar15 & 0x7f);
    uVar11 = *(ulong *)(param_2 + 0x20);
    if (uVar11 != 0) {
      puVar12 = *(ulong **)(param_2 + 0x28);
      uVar17 = *puVar12;
      if (iVar10 == -0xb) {
        uVar18 = 0;
        if (uVar17 == 0) {
          do {
            if (uVar11 - 1 == uVar18) goto code_r0x0015b168;
            uVar17 = puVar12[uVar18 + 1];
            uVar18 = uVar18 + 1;
          } while (uVar17 == 0);
          bVar5 = uVar18 < uVar11;
        }
        else {
          bVar5 = true;
        }
        if (!bVar5) {
code_r0x0015b168:
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x89b9,"int JS_WriteBigNum(BCWriterState *, JSValue)","i < a->len");
        }
        if ((uVar17 * -0x3333333333333333 >> 1 | uVar17 * -0x3333333333333333 << 0x3f) <
            0x199999999999999a) {
          uVar19 = 0;
          do {
            uVar19 = uVar19 + 1;
            uVar17 = uVar17 / 10;
          } while ((uVar17 * -0x3333333333333333 >> 1 | uVar17 * -0x3333333333333333 << 0x3f) <
                   0x199999999999999a);
        }
        else {
          uVar19 = 0;
        }
        uVar11 = uVar11 * 0x13 - uVar19;
        if (uVar11 == 0) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x89c2,"int JS_WriteBigNum(BCWriterState *, JSValue)","len > 0");
        }
        if (uVar11 >> 0x1f == 0) {
          uVar15 = (uint)uVar11;
          uVar16 = uVar11;
          if (0x7f < uVar15) {
            do {
              uVar15 = (uint)uVar16 >> 7;
              FUN_001d27c4(plVar1,(uint)uVar16 | 0xffffff80);
              uVar4 = uVar16 >> 0xe;
              uVar16 = (ulong)uVar15;
            } while ((uVar4 & 0x3ffff) != 0);
          }
          FUN_001d27c4(plVar1,uVar15 & 0x7f);
          if (uVar18 < *(ulong *)(param_2 + 0x20)) {
            uVar15 = 0;
            bVar5 = false;
            uVar16 = uVar18;
            do {
              if (uVar16 != uVar18) {
                uVar19 = 0;
                uVar17 = *(ulong *)(*(long *)(param_2 + 0x28) + uVar16 * 8);
              }
              if (uVar19 < 0x13) {
                lVar14 = uVar19 - 0x13;
                uVar19 = uVar17;
                uVar13 = uVar15;
                do {
                  while( true ) {
                    uVar17 = uVar19 / 10;
                    uVar15 = (int)uVar19 + (int)uVar17 * -10;
                    uVar19 = uVar17;
                    if (bVar5) break;
                    bVar5 = true;
                    bVar6 = lVar14 == -1;
                    lVar14 = lVar14 + 1;
                    uVar13 = uVar15;
                    if (bVar6) goto LAB_0015b004;
                  }
                  FUN_001d27c4(plVar1,uVar13 | uVar15 * 0x10);
                  bVar5 = false;
                  bVar6 = lVar14 != -1;
                  lVar14 = lVar14 + 1;
                  uVar15 = uVar13;
                } while (bVar6);
LAB_0015b004:
                uVar19 = 0x13;
              }
              uVar16 = uVar16 + 1;
            } while (uVar16 < *(ulong *)(param_2 + 0x20));
          }
          else {
            bVar5 = false;
            uVar15 = 0;
          }
          if (bVar5) {
            FUN_001d27c4(plVar1,uVar15);
          }
        }
        else {
          FUN_001405ec(*param_1,"bignum is too large");
        }
        if (uVar11 >> 0x1f != 0) goto LAB_0015aa5c;
      }
      else {
        if (uVar17 == 0) {
          uVar18 = 1;
          do {
            uVar19 = uVar18;
            if (uVar11 == uVar19) goto code_r0x0015b188;
            uVar17 = puVar12[uVar19];
            uVar18 = uVar19 + 1;
          } while (uVar17 == 0);
          bVar5 = uVar19 < uVar11;
        }
        else {
          bVar5 = true;
          uVar18 = 1;
        }
        if (!bVar5) {
code_r0x0015b188:
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x8990,"int JS_WriteBigNum(BCWriterState *, JSValue)","i < a->len");
        }
        lVar14 = 8;
        uVar19 = uVar17 & 0xff;
        while (uVar19 == 0) {
          lVar14 = lVar14 + -1;
          uVar19 = uVar17 & 0xff00;
          uVar17 = uVar17 >> 8;
        }
        uVar11 = lVar14 + (uVar11 - uVar18) * 8;
        if (uVar11 >> 0x1f != 0) {
          lVar14 = *param_1;
          pcVar9 = "bignum is too large";
          goto LAB_0015aa58;
        }
        uVar15 = (uint)uVar11;
        if (0x7f < uVar15) {
          do {
            uVar15 = (uint)uVar11 >> 7;
            FUN_001d27c4(plVar1,(uint)uVar11 | 0xffffff80);
            uVar19 = uVar11 >> 0xe;
            uVar11 = (ulong)uVar15;
          } while ((uVar19 & 0x3ffff) != 0);
        }
        FUN_001d27c4(plVar1,uVar15 & 0x7f);
        if (lVar14 != 0) {
          uVar11 = 0;
          do {
            FUN_001d27c4(plVar1,uVar17 >> (uVar11 & 0x3f));
            lVar14 = lVar14 + -1;
            uVar11 = uVar11 + 8;
          } while (lVar14 != 0);
        }
        if (uVar18 < *(ulong *)(param_2 + 0x20)) {
          do {
            local_70 = *(ulong *)(*(long *)(param_2 + 0x28) + uVar18 * 8);
            FUN_001d26dc(plVar1,&local_70,8);
            uVar8 = 0;
            uVar18 = uVar18 + 1;
          } while (uVar18 < *(ulong *)(param_2 + 0x20));
          goto LAB_0015aa60;
        }
      }
    }
    goto LAB_0015b0b0;
  default:
    goto switchD_0015aabc_caseD_fffffff8;
  case -7:
    FUN_001d27c4(param_1 + 1,7);
    FUN_0017a100(param_1,param_2);
    uVar8 = 0;
    goto LAB_0015aa60;
  case -3:
    if (*(char *)((long)param_1 + 0x39) != '\0') {
      uVar8 = FUN_0017ad38(param_1,param_2,param_3);
      iVar10 = (int)uVar8;
joined_r0x0015ab3c:
      if (iVar10 == 0) goto LAB_0015aa60;
      goto LAB_0015aa5c;
    }
    goto switchD_0015aabc_caseD_fffffff8;
  case -2:
    if (*(char *)((long)param_1 + 0x39) != '\0') {
      uVar8 = FUN_0017a22c(param_1,param_2,param_3);
      iVar10 = (int)uVar8;
      goto joined_r0x0015ab3c;
    }
switchD_0015aabc_caseD_fffffff8:
    FUN_001405ec(*param_1,"unsupported tag (%d)",param_3 & 0xffffffff);
LAB_0015aa5c:
    uVar8 = 0xffffffff;
    goto LAB_0015aa60;
  case -1:
    if (*(char *)((long)param_1 + 0x3b) != '\0') {
      iVar10 = FUN_0017b2ac(param_1 + 0xe,param_2);
      if (iVar10 < 0) {
        iVar10 = FUN_0017b364(lVar14,param_1 + 0xe,param_2);
        if (iVar10 != 0) goto LAB_0015ad8c;
        goto LAB_0015ada4;
      }
      FUN_001d27c4(param_1 + 1,0x15);
      FUN_0017b308(param_1,iVar10);
      iVar10 = 2;
      goto LAB_0015ad98;
    }
    if ((*(byte *)(param_2 + 5) >> 6 & 1) != 0) {
      FUN_00143570(lVar14,"circular reference");
LAB_0015ad8c:
      iVar10 = 4;
      goto LAB_0015ad98;
    }
    *(byte *)(param_2 + 5) = *(byte *)(param_2 + 5) | 0x40;
LAB_0015ada4:
    switch((uint)*(ushort *)(param_2 + 6)) {
    case 1:
      iVar7 = FUN_0017b698(param_1,param_2,param_3);
      break;
    case 2:
      iVar7 = FUN_0017b44c(param_1,param_2,param_3);
      break;
    default:
      if (*(ushort *)(param_2 + 6) - 0x15 < 0xb) {
        iVar7 = FUN_0017ba8c(param_1,param_2,param_3);
      }
      else {
        FUN_00143570(*param_1,"unsupported object class");
        iVar7 = -1;
      }
      break;
    case 4:
    case 5:
    case 6:
    case 0x21:
    case 0x22:
    case 0x24:
      uVar8 = 0x14;
      goto LAB_0015add4;
    case 10:
      uVar8 = 0x13;
LAB_0015add4:
      FUN_001d27c4(param_1 + 1,uVar8);
      iVar7 = FUN_0015aa08(param_1,*(undefined8 *)(param_2 + 0x30),*(undefined8 *)(param_2 + 0x38));
      break;
    case 0x13:
      iVar7 = FUN_0017b8a4(param_1,param_2,param_3);
      break;
    case 0x14:
      if (*(char *)((long)param_1 + 0x3a) == '\0') goto switchD_0015aabc_caseD_fffffff8;
      iVar7 = FUN_0017b948(param_1,param_2,param_3);
    }
    iVar10 = (uint)(iVar7 != 0) << 2;
    *(byte *)(param_2 + 5) = *(byte *)(param_2 + 5) & 0xbf;
    if ((uint)(iVar7 != 0) << 2 == 3) goto switchD_0015aabc_caseD_fffffff8;
LAB_0015ad98:
    if (iVar10 != 4) {
LAB_0015b0b0:
      uVar8 = 0;
      goto LAB_0015aa60;
    }
    goto LAB_0015aa5c;
  }
  FUN_001d27c4(param_1 + 1,uVar15);
  uVar8 = 0;
LAB_0015aa60:
  if (*(long *)(lVar3 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar8);
  }
  return;
}

/* ===== FUN_0015c65c @ 0015c65c [libNexusScriptRuntime69252.so] ===== */

/* WARNING: Removing unreachable block (ram,0x0015cbbc) */
/* WARNING: Removing unreachable block (ram,0x0015c848) */

undefined1  [16]
FUN_0015c65c(long param_1,int *param_2,undefined8 param_3,undefined8 param_4,
            undefined1 (*param_5) [16])

{
  undefined1 (*pauVar1) [16];
  ulong uVar2;
  ulong uVar3;
  int iVar4;
  int iVar5;
  undefined1 auVar6 [16];
  undefined1 auVar7 [16];
  uint uVar8;
  int iVar9;
  long lVar10;
  ulong in_x10;
  ulong extraout_x10;
  ulong extraout_x10_00;
  ulong extraout_x10_01;
  ulong extraout_x10_02;
  int *piVar11;
  int *piVar12;
  undefined8 uVar13;
  int *piVar14;
  uint uVar15;
  undefined1 auVar16 [16];
  undefined1 auVar17 [16];
  undefined1 auVar18 [16];
  undefined1 auVar19 [16];
  int *local_70;
  
  piVar12 = *(int **)*param_5;
  uVar2 = *(ulong *)(*param_5 + 8);
  auVar17 = *param_5;
  pauVar1 = param_5 + 1;
  piVar14 = *(int **)*pauVar1;
  uVar3 = *(ulong *)(param_5[1] + 8);
  auVar7 = *pauVar1;
  auVar6 = *pauVar1;
  auVar19 = *pauVar1;
  auVar18 = *pauVar1;
  if ((uVar2 & 0xffffffff) == 0xffffffff) {
    auVar16 = FUN_001446d4(param_1,piVar12,uVar2,0xd8,piVar12,uVar2,0);
    in_x10 = extraout_x10;
    if (auVar16._8_4_ == 3) {
      uVar8 = (uint)(*(short *)((long)piVar12 + 6) == 0x12);
    }
    else if ((auVar16._8_4_ == 6) ||
            (uVar8 = FUN_001442a8(param_1,auVar16._0_8_,auVar16._8_8_), in_x10 = extraout_x10_00,
            (int)uVar8 < 0)) {
      auVar17 = ZEXT816(6) << 0x40;
      goto LAB_0015cb84;
    }
  }
  else {
    uVar8 = 0;
  }
  local_70 = param_2;
  if ((int)param_3 == 3) {
    lVar10 = *(long *)(*(long *)(param_1 + 0x18) + 0xf8);
    local_70 = *(int **)(lVar10 + 8);
    param_3 = *(undefined8 *)(lVar10 + 0x10);
    if ((uVar8 != 0) && ((uVar3 & 0xffffffff) == 3)) {
      auVar16 = FUN_001446d4(param_1,piVar12,uVar2,0x3e,piVar12,uVar2,0);
      piVar11 = auVar16._0_8_;
      uVar15 = auVar16._8_4_;
      if (uVar15 == 6) {
        in_x10 = (ulong)piVar11 >> 0x20;
        auVar17 = auVar16;
        goto LAB_0015cb84;
      }
      if (0xfffffff4 < uVar15) {
        *piVar11 = *piVar11 + 1;
      }
      if (0xfffffff4 < (uint)param_3) {
        *local_70 = *local_70 + 1;
      }
      iVar9 = FUN_0016fc84(param_1,piVar11,auVar16._8_8_,local_70,param_3,1);
      in_x10 = extraout_x10_01;
      if ((0xfffffff4 < uVar15) &&
         (iVar5 = *piVar11, iVar4 = iVar5 + -1, *piVar11 = iVar4, iVar4 == 0 || iVar5 < 1)) {
        FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar11,auVar16._8_8_);
        in_x10 = extraout_x10_02;
      }
      if (iVar9 != 0) {
        if (0xfffffff4 < (uint)uVar2) {
          *piVar12 = *piVar12 + 1;
        }
        in_x10 = (ulong)piVar12 >> 0x20;
        goto LAB_0015cb84;
      }
    }
  }
  uVar15 = (uint)uVar3;
  if ((((uVar2 & 0xffffffff) == 0xffffffff) && (*(short *)((long)piVar12 + 6) == 0x12)) &&
     (piVar12 + 0xc != (int *)0x0)) {
    piVar11 = *(int **)(piVar12 + 0xc);
    *piVar11 = *piVar11 + 1;
    if (uVar15 != 3) {
      auVar17._8_8_ = 0xfffffffffffffff9;
      auVar17._0_8_ = piVar11;
      auVar18 = FUN_0014c3ec(param_1,piVar14,uVar3,0);
      if (auVar18._8_4_ == 6) {
        auVar17._8_8_ = 0xfffffffffffffff9;
        goto LAB_0015caa4;
      }
      goto LAB_0015ca74;
    }
    piVar12 = *(int **)(piVar12 + 0xe);
    auVar19._8_8_ = 0xfffffffffffffff9;
    auVar19._0_8_ = piVar12;
    auVar17._8_8_ = 0xfffffffffffffff9;
    auVar17._0_8_ = piVar11;
    *piVar12 = *piVar12 + 1;
LAB_0015cb54:
    auVar17 = FUN_00171688(param_1,local_70,param_3,auVar17._0_8_,auVar17._8_8_,auVar19._0_8_,
                           auVar19._8_8_);
    in_x10 = auVar17._0_8_ >> 0x20;
  }
  else {
    if (uVar8 == 0) {
      if (0xfffffff4 < (uint)uVar2) {
        *piVar12 = *piVar12 + 1;
      }
      if (0xfffffff4 < uVar15) {
        *piVar14 = *piVar14 + 1;
        auVar18 = auVar19;
      }
LAB_0015c9e4:
      uVar13 = auVar17._8_8_;
      piVar12 = auVar17._0_8_;
      uVar8 = auVar17._8_4_;
      if (uVar8 == 3) {
        if (*(uint *)(*(long *)(param_1 + 0x18) + 0x50) < 0x30) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0xc94,"JSValue __JS_AtomToValue(JSContext *, JSAtom, BOOL)",
                    "atom < rt->atom_size");
        }
        piVar12 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + 0x178);
        auVar17._8_8_ = 0xfffffffffffffff9;
        auVar17._0_8_ = piVar12;
        *piVar12 = *piVar12 + 1;
      }
      else {
        auVar17 = FUN_0014c3ec(param_1,piVar12,uVar13,0);
        if ((0xfffffff4 < uVar8) &&
           (iVar9 = *piVar12, iVar5 = iVar9 + -1, *piVar12 = iVar5, iVar5 == 0 || iVar9 < 1)) {
          FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar12,uVar13);
        }
        if (auVar17._8_4_ == 6) goto LAB_0015caa4;
      }
LAB_0015ca74:
      piVar12 = auVar18._0_8_;
      auVar19 = FUN_0015c430(param_1,auVar17._0_8_,auVar17._8_8_,piVar12,auVar18._8_8_);
      if (auVar19._8_4_ != 6) {
        if ((0xfffffff4 < auVar18._8_4_) &&
           (iVar9 = *piVar12, iVar5 = iVar9 + -1, *piVar12 = iVar5, iVar5 == 0 || iVar9 < 1)) {
          FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar12,auVar18._8_8_);
        }
        goto LAB_0015cb54;
      }
    }
    else {
      auVar17 = FUN_001446d4(param_1,piVar12,uVar2,0x6e,piVar12,uVar2,0);
      if (auVar17._8_4_ != 6) {
        if (uVar15 == 3) {
          auVar18 = FUN_001446d4(param_1,piVar12,uVar2,0x6f,piVar12,uVar2,0);
          if (auVar18._8_4_ == 6) goto LAB_0015caa4;
        }
        else {
          auVar18 = auVar6;
          if (0xfffffff4 < uVar15) {
            *piVar14 = *piVar14 + 1;
            auVar18 = auVar7;
          }
        }
        goto LAB_0015c9e4;
      }
      auVar18 = ZEXT816(3) << 0x40;
    }
LAB_0015caa4:
    piVar14 = auVar17._0_8_;
    piVar12 = auVar18._0_8_;
    if ((0xfffffff4 < auVar17._8_4_) &&
       (iVar9 = *piVar14, iVar5 = iVar9 + -1, *piVar14 = iVar5, iVar5 == 0 || iVar9 < 1)) {
      FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar14,auVar17._8_8_);
    }
    if ((0xfffffff4 < auVar18._8_4_) &&
       (iVar9 = *piVar12, iVar5 = iVar9 + -1, *piVar12 = iVar5, iVar5 == 0 || iVar9 < 1)) {
      FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar12,auVar18._8_8_);
    }
    auVar17 = ZEXT816(6) << 0x40;
  }
LAB_0015cb84:
  auVar18._8_8_ = auVar17._8_8_;
  auVar18._0_8_ = auVar17._0_8_ & 0xffffffff | in_x10 << 0x20;
  return auVar18;
}

/* ===== FUN_0015d7fc @ 0015d7fc [libNexusScriptRuntime69252.so] ===== */

/* WARNING: Removing unreachable block (ram,0x0015dbe4) */
/* WARNING: Removing unreachable block (ram,0x0015d964) */

undefined1  [16]
FUN_0015d7fc(long param_1,int *param_2,undefined8 param_3,ulong param_4,ulong param_5,int *param_6,
            undefined8 param_7)

{
  uint uVar1;
  ushort uVar2;
  int iVar3;
  long lVar4;
  undefined1 auVar5 [16];
  undefined1 auVar6 [16];
  undefined1 auVar7 [16];
  undefined8 uVar8;
  long *plVar9;
  ulong uVar10;
  bool bVar11;
  int iVar12;
  uint uVar13;
  int *piVar15;
  undefined8 *puVar16;
  ulong uVar17;
  long lVar18;
  undefined1 auVar19 [16];
  undefined1 auVar20 [16];
  undefined1 auVar21 [16];
  undefined1 auVar22 [16];
  long local_130;
  int *local_128;
  int local_114;
  long local_110;
  undefined1 local_108 [16];
  ulong local_f8;
  ulong local_f0;
  undefined1 local_e8 [16];
  undefined1 local_d8 [16];
  undefined1 local_c8 [16];
  int *local_b8;
  undefined8 local_b0;
  long *local_a8;
  long local_a0;
  undefined4 *local_98;
  ulong local_90;
  undefined8 uStack_88;
  long local_80;
  long lVar14;
  
  auVar19._8_8_ = param_7;
  auVar19._0_8_ = param_6;
  lVar4 = tpidr_el0;
  local_a8 = &local_a0;
  local_80 = *(long *)(lVar4 + 0x28);
  puVar16 = *(undefined8 **)(param_1 + 0x18);
  local_c8._8_8_ = 3;
  local_f0 = 3;
  local_e8._8_8_ = 3;
  local_d8._8_8_ = 3;
  local_f8 = local_f8 & 0xffffffff00000000;
  local_e8._0_8_ = local_e8._0_8_ & 0xffffffff00000000;
  local_d8._0_8_ = local_d8._0_8_ & 0xffffffff00000000;
  local_c8._0_8_ = local_c8._0_8_ & 0xffffffff00000000;
  if (*(uint *)(puVar16 + 10) < 0x30) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0xc94,"JSValue __JS_AtomToValue(JSContext *, JSAtom, BOOL)","atom < rt->atom_size");
  }
  local_b8 = *(int **)(puVar16[0xc] + 0x178);
  *local_b8 = *local_b8 + 1;
  local_b0 = 0xfffffffffffffff9;
  local_90 = 0;
  uStack_88 = 0;
  local_a0 = param_1;
  local_98 = (undefined4 *)(*(code *)*puVar16)(puVar16 + 4,0x11);
  if (local_98 == (undefined4 *)0x0) {
    lVar18 = *(long *)(param_1 + 0x18);
    if ((*(ushort *)(lVar18 + 0xf0) & 0xff) == 0) {
      *(ushort *)(lVar18 + 0xf0) = *(ushort *)(lVar18 + 0xf0) & 0xff00 | 1;
      FUN_001405ec(param_1,"out of memory");
      local_98 = (undefined4 *)0x0;
      *(ushort *)(lVar18 + 0xf0) = (ushort)*(byte *)(lVar18 + 0xf1) << 8;
    }
    else {
      local_98 = (undefined4 *)0x0;
    }
  }
  else {
    *(undefined8 *)(local_98 + 1) = 0;
    local_98[3] = 0;
    *local_98 = 1;
  }
  if (local_98 == (undefined4 *)0x0) {
    local_90 = local_90 & 0xffffffff;
    uStack_88 = CONCAT44(0xffffffff,(undefined4)uStack_88);
  }
  piVar15 = *(int **)(param_1 + 0x38);
  *piVar15 = *piVar15 + 1;
  local_e8 = FUN_00140ed0(param_1,piVar15,2);
  auVar20._8_8_ = local_108._8_8_;
  auVar20._0_8_ = local_108._0_8_;
  if (local_e8._8_4_ == 6) goto LAB_0015ddc4;
  local_108._0_8_ = param_4;
  if ((param_5 & 0xffffffff) != 0xffffffff) goto LAB_0015d938;
  uVar2 = *(ushort *)(param_4 + 6);
  uVar17 = param_4;
  uVar10 = param_5;
  if (uVar2 == 0xd) {
LAB_0015d974:
    local_f0 = uVar10;
    local_f8 = uVar17;
    if (0xfffffff4 < (uint)param_7) {
      *param_6 = *param_6 + 1;
    }
    if ((uint)param_7 == 0xffffffff) {
      if (*(short *)((long)param_6 + 6) == 5) {
        auVar19 = FUN_0014c3ec(param_1,param_6,param_7,0);
        iVar12 = *param_6;
        iVar3 = iVar12 + -1;
        *param_6 = iVar3;
        if (iVar3 == 0 || iVar12 < 1) {
          FUN_00141774(*(undefined8 *)(param_1 + 0x18),param_6,param_7);
        }
      }
      else if (*(short *)((long)param_6 + 6) == 4) {
        auVar19 = FUN_0016db18(param_1,param_6,param_7,0);
      }
      auVar6._8_8_ = local_c8._8_8_;
      auVar6._0_8_ = local_c8._0_8_;
      auVar5._8_8_ = local_108._8_8_;
      auVar5._0_8_ = local_108._0_8_;
      auVar20._8_8_ = local_108._8_8_;
      auVar20._0_8_ = local_108._0_8_;
      local_128 = auVar19._0_8_;
      if ((auVar19._8_8_ & 0xffffffff) == 6) {
        if ((0xfffffff4 < auVar19._8_4_) &&
           (iVar12 = *local_128, iVar3 = iVar12 + -1, *local_128 = iVar3, auVar20 = auVar5,
           local_c8 = auVar6, iVar3 == 0 || iVar12 < 1)) {
          FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_128,auVar19._8_8_);
          auVar20._8_8_ = local_108._8_8_;
          auVar20._0_8_ = local_108._0_8_;
        }
        goto LAB_0015ddc4;
      }
    }
    local_128 = auVar19._0_8_;
    uVar13 = auVar19._8_4_;
    if ((uVar13 == 7) || (uVar13 == 0)) {
      if (0xfffffff4 < uVar13) {
        *local_128 = *local_128 + 1;
      }
      iVar12 = FUN_0014bb84(param_1,&local_114,local_128,auVar19._8_8_);
      auVar20._8_8_ = local_108._8_8_;
      auVar20._0_8_ = local_108._0_8_;
      if (iVar12 != 0) goto LAB_0015ddc4;
      if (local_114 < 0) {
        local_114 = 0;
      }
      else if (10 < local_114) {
        local_114 = 10;
      }
      local_c8 = FUN_0013f444(param_1,"          ",(long)local_114);
    }
    else if (uVar13 == 0xfffffff9) {
      uVar1 = local_128[1] & 0x7fffffff;
      if (9 < uVar1) {
        uVar1 = 10;
      }
      local_c8 = FUN_0015e634(param_1,local_128,0,uVar1);
    }
    else {
      if (0xfffffff4 < (uint)local_b0) {
        *local_b8 = *local_b8 + 1;
      }
      local_c8._8_8_ = local_b0;
      local_c8._0_8_ = local_b8;
    }
    if ((0xfffffff4 < uVar13) &&
       (iVar12 = *local_128, iVar3 = iVar12 + -1, *local_128 = iVar3, iVar3 == 0 || iVar12 < 1)) {
      FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_128,auVar19._8_8_);
    }
    auVar20._8_8_ = local_108._8_8_;
    auVar20._0_8_ = local_108._0_8_;
    bVar11 = local_c8._8_4_ == 6;
    if (bVar11) goto LAB_0015ddc4;
    auVar19 = FUN_00140e0c(param_1,*(undefined8 *)(*(long *)(param_1 + 0x40) + 0x10),
                           *(undefined8 *)(*(long *)(param_1 + 0x40) + 0x18),1);
    if (auVar19._8_4_ == 6) {
LAB_0015e010:
      uVar17 = 6;
    }
    else {
      if (0xfffffff4 < (uint)param_3) {
        *param_2 = *param_2 + 1;
      }
      iVar12 = FUN_0014a868(param_1,auVar19._0_8_,auVar19._8_8_,0x2f,param_2,param_3,7);
      if (iVar12 < 0) goto LAB_0015e010;
      if (0xfffffff4 < (uint)param_3) {
        *param_2 = *param_2 + 1;
      }
      auVar20 = FUN_0015e83c(param_1,&local_f8,auVar19._0_8_,auVar19._8_8_,param_2,param_3,local_b8,
                             local_b0);
      auVar7._8_8_ = local_108._8_8_;
      auVar7._0_8_ = local_108._0_8_;
      if (auVar20._8_4_ == 3) {
        uVar17 = 3;
      }
      else {
        if (auVar20._8_4_ != 6) {
          iVar12 = FUN_0015ec44(param_1,&local_f8,auVar20._0_8_,auVar20._8_8_,local_b8,local_b0);
          if (iVar12 == 0) {
            auVar22 = FUN_00140918(local_a8);
            uVar17 = auVar22._0_8_ & 0xffffffff00000000;
            goto LAB_0015ddf4;
          }
          goto LAB_0015e010;
        }
        uVar17 = auVar20._8_8_ & 0xffffffff;
        local_108 = auVar7;
      }
    }
  }
  else {
    if (uVar2 != 0x30) {
      uVar17 = param_4;
      uVar10 = param_5;
      if (*(long *)(*(long *)(*(long *)(param_1 + 0x18) + 0x70) + (ulong)uVar2 * 0x28 + 0x18) == 0)
      goto LAB_0015d938;
      goto LAB_0015d974;
    }
    uVar17 = param_4;
    uVar10 = param_5;
    if (*(char *)(*(long *)(param_4 + 0x30) + 0x20) != '\0') goto LAB_0015d974;
LAB_0015d938:
    if ((param_5 & 0xffffffff) == 0xffffffff) {
      if (*(short *)(param_4 + 6) == 0x30) {
        uVar13 = FUN_0014c770(param_1,param_4,param_5);
        auVar20._8_8_ = local_108._8_8_;
        auVar20._0_8_ = local_108._0_8_;
        if ((int)uVar13 < 0) goto LAB_0015ddc4;
      }
      else {
        uVar13 = (uint)(*(short *)(param_4 + 6) == 2);
      }
    }
    else {
      uVar13 = 0;
    }
    uVar17 = local_f8;
    uVar10 = local_f0;
    if (uVar13 == 0) goto LAB_0015d974;
    piVar15 = *(int **)(param_1 + 0x38);
    *piVar15 = *piVar15 + 1;
    local_d8 = FUN_00140ed0(param_1,piVar15,2);
    auVar20._8_8_ = local_108._8_8_;
    auVar20._0_8_ = local_108._0_8_;
    if (local_d8._8_4_ != 6) {
      iVar12 = FUN_0015e0c4(param_1,&local_110,param_4,param_5);
      auVar20._8_8_ = local_108._8_8_;
      auVar20._0_8_ = local_108._0_8_;
      if (iVar12 == 0) {
        uVar17 = local_f8;
        uVar10 = local_f0;
        if (0 < local_110) {
          lVar18 = 0;
          local_130 = 0;
          do {
            local_108 = auVar20;
            auVar20 = FUN_0015e1bc(param_1,param_4,param_5,lVar18);
            uVar17 = auVar20._8_8_;
            lVar14 = auVar20._0_8_;
            iVar12 = auVar20._8_4_;
            if (iVar12 == 6) {
LAB_0015dae4:
              iVar12 = 2;
            }
            else {
              local_108 = auVar20;
              if (iVar12 == -1) {
                if ((*(ushort *)(lVar14 + 6) & 0xfffe) == 4) {
                  auVar20 = FUN_0015e310(param_1,lVar14,uVar17);
                  bVar11 = (auVar20._8_8_ & 0xffffffff) == 6;
                  if (!bVar11) goto LAB_0015db0c;
                  iVar12 = (uint)bVar11 << 1;
                }
                else {
                  FUN_0013a308(param_1,lVar14,uVar17);
                  auVar20 = local_108;
LAB_0015dbb4:
                  iVar12 = 5;
                }
              }
              else {
                if ((iVar12 == 7) || (iVar12 == 0)) {
                  auVar20 = FUN_0015e310(param_1,lVar14,uVar17);
                  if (auVar20._8_4_ == 6) goto LAB_0015dae4;
                }
                else if ((uVar17 & 0xffffffff) != 0xfffffff9) {
                  FUN_0013a308(param_1,lVar14,uVar17);
                  auVar20 = local_108;
                  goto LAB_0015dbb4;
                }
LAB_0015db0c:
                local_108._8_8_ = auVar20._8_8_;
                uVar8 = local_108._8_8_;
                local_108 = auVar20;
                auVar20 = FUN_0015e38c(param_1,local_d8._0_8_,local_d8._8_8_,1,local_108);
                if (auVar20._8_4_ == 6) {
                  FUN_0013a308(param_1,local_108._0_8_,uVar8);
                  iVar12 = 2;
                  auVar20 = local_108;
                }
                else {
                  iVar12 = FUN_001442a8(param_1,auVar20._0_8_,auVar20._8_8_);
                  if (iVar12 == 0) {
                    FUN_00149a6c(param_1,local_d8._0_8_,local_d8._8_8_,local_130,local_108._0_8_,
                                 uVar8);
                    iVar12 = 0;
                    local_130 = local_130 + 1;
                    auVar20 = local_108;
                  }
                  else {
                    FUN_0013a308(param_1,local_108._0_8_,uVar8);
                    iVar12 = 0;
                    auVar20 = local_108;
                  }
                }
              }
            }
            local_108._8_8_ = auVar20._8_8_;
            local_108._0_8_ = auVar20._0_8_;
            if ((iVar12 != 0) && (iVar12 != 5)) {
              if (iVar12 == 2) goto LAB_0015ddc4;
              uVar17 = 0;
              auVar22 = ZEXT816(3) << 0x40;
              goto LAB_0015def4;
            }
            lVar18 = lVar18 + 1;
            uVar17 = local_f8;
            uVar10 = local_f0;
          } while (lVar18 < local_110);
        }
        goto LAB_0015d974;
      }
    }
LAB_0015ddc4:
    uVar17 = 6;
    auVar19 = ZEXT816(3) << 0x40;
    local_108 = auVar20;
  }
  plVar9 = local_a8;
  (**(code **)(*(long *)(*local_a8 + 0x18) + 8))(*(long *)(*local_a8 + 0x18) + 0x20,local_a8[1]);
  auVar22._8_8_ = 0;
  auVar22._0_8_ = uVar17;
  auVar22 = auVar22 << 0x40;
  uVar17 = 0;
  plVar9[1] = 0;
LAB_0015ddf4:
  piVar15 = auVar19._0_8_;
  if ((0xfffffff4 < auVar19._8_4_) &&
     (iVar12 = *piVar15, iVar3 = iVar12 + -1, *piVar15 = iVar3, iVar3 == 0 || iVar12 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar15,auVar19._8_8_);
  }
  if ((0xfffffff4 < (uint)local_b0) &&
     (iVar12 = *local_b8, iVar3 = iVar12 + -1, *local_b8 = iVar3, iVar3 == 0 || iVar12 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_b8);
  }
  if ((0xfffffff4 < local_c8._8_4_) &&
     (iVar12 = *(int *)local_c8._0_8_, iVar3 = iVar12 + -1, *(int *)local_c8._0_8_ = iVar3,
     iVar3 == 0 || iVar12 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_c8._0_8_);
  }
  if ((0xfffffff4 < local_d8._8_4_) &&
     (iVar12 = *(int *)local_d8._0_8_, iVar3 = iVar12 + -1, *(int *)local_d8._0_8_ = iVar3,
     iVar3 == 0 || iVar12 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_d8._0_8_);
  }
  auVar20 = local_108;
  if ((0xfffffff4 < local_e8._8_4_) &&
     (iVar12 = *(int *)local_e8._0_8_, iVar3 = iVar12 + -1, *(int *)local_e8._0_8_ = iVar3,
     iVar3 == 0 || iVar12 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_e8._0_8_);
    auVar20 = local_108;
  }
LAB_0015def4:
  if (*(long *)(lVar4 + 0x28) != local_80) {
    local_108 = auVar20;
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  auVar21._0_8_ = uVar17 | auVar22._0_8_ & 0xffffffff;
  auVar21._8_8_ = auVar22._8_8_;
  return auVar21;
}

/* ===== FUN_00160c70 @ 00160c70 [libNexusScriptRuntime69252.so] ===== */

undefined1  [16] FUN_00160c70(long param_1)

{
  long *plVar1;
  int iVar2;
  int *piVar3;
  long lVar4;
  undefined8 *puVar5;
  ulong uVar6;
  ulong uVar7;
  undefined8 uVar8;
  long lVar9;
  long *plVar10;
  undefined1 auVar11 [16];
  
  piVar3 = (int *)FUN_001771c4();
  if (piVar3 != (int *)0x0) {
    auVar11 = FUN_00160770(param_1,piVar3 + 0xe,0,3);
    uVar8 = auVar11._8_8_;
    if (auVar11._8_4_ != 6) {
      FUN_0018c4d0(param_1,piVar3);
      iVar2 = *piVar3;
      lVar9 = *(long *)(param_1 + 0x18);
      *piVar3 = iVar2 + -1;
      if ((iVar2 + -1 == 0) && (*(char *)(lVar9 + 0xb8) != '\x02')) {
        plVar10 = (long *)(piVar3 + 2);
        lVar4 = *plVar10;
        plVar1 = *(long **)(piVar3 + 4);
        *(long **)(lVar4 + 8) = plVar1;
        *plVar1 = lVar4;
        *plVar10 = 0;
        piVar3[4] = 0;
        piVar3[5] = 0;
        puVar5 = *(undefined8 **)(lVar9 + 0xa0);
        *(long **)(lVar9 + 0xa0) = plVar10;
        *plVar10 = lVar9 + 0x98;
        *(undefined8 **)(piVar3 + 4) = puVar5;
        *puVar5 = plVar10;
        if (*(char *)(lVar9 + 0xb8) == '\0') {
          lVar4 = *(long *)(lVar9 + 0xa0);
          *(undefined1 *)(lVar9 + 0xb8) = 1;
          while (lVar4 != lVar9 + 0x98) {
            if (*(int *)(lVar4 + -8) != 0) goto code_r0x00160e0c;
            FUN_0016b224(lVar9);
            lVar4 = *(long *)(lVar9 + 0xa0);
          }
          *(undefined1 *)(lVar9 + 0xb8) = 0;
        }
      }
      uVar6 = auVar11._0_8_ & 0xffffffff00000000;
      uVar7 = auVar11._0_8_ & 0xffffffff;
      goto LAB_00160da8;
    }
    iVar2 = *piVar3;
    lVar9 = *(long *)(param_1 + 0x18);
    *piVar3 = iVar2 + -1;
    if ((iVar2 + -1 == 0) && (*(char *)(lVar9 + 0xb8) != '\x02')) {
      plVar10 = (long *)(piVar3 + 2);
      lVar4 = *plVar10;
      plVar1 = *(long **)(piVar3 + 4);
      *(long **)(lVar4 + 8) = plVar1;
      *plVar1 = lVar4;
      *plVar10 = 0;
      piVar3[4] = 0;
      piVar3[5] = 0;
      puVar5 = *(undefined8 **)(lVar9 + 0xa0);
      *(long **)(lVar9 + 0xa0) = plVar10;
      *plVar10 = lVar9 + 0x98;
      *(undefined8 **)(piVar3 + 4) = puVar5;
      *puVar5 = plVar10;
      if (*(char *)(lVar9 + 0xb8) == '\0') {
        lVar4 = *(long *)(lVar9 + 0xa0);
        *(undefined1 *)(lVar9 + 0xb8) = 1;
        while (lVar4 != lVar9 + 0x98) {
          if (*(int *)(lVar4 + -8) != 0) {
code_r0x00160e0c:
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0x15e6,"void free_zero_refcount(JSRuntime *)","p->ref_count == 0");
          }
          FUN_0016b224(lVar9);
          lVar4 = *(long *)(lVar9 + 0xa0);
        }
        uVar7 = 0;
        uVar6 = 0;
        uVar8 = 6;
        *(undefined1 *)(lVar9 + 0xb8) = 0;
        goto LAB_00160da8;
      }
    }
  }
  uVar7 = 0;
  uVar6 = 0;
  uVar8 = 6;
LAB_00160da8:
  auVar11._0_8_ = uVar6 | uVar7;
  auVar11._8_8_ = uVar8;
  return auVar11;
}

/* ===== FUN_001620b4 @ 001620b4 [libNexusScriptRuntime69252.so] ===== */

undefined1  [16]
FUN_001620b4(long param_1,undefined8 param_2,undefined8 param_3,char *param_4,long param_5,
            char *param_6,uint param_7,int param_8)

{
  char *pcVar1;
  uint *puVar2;
  byte *pbVar3;
  undefined1 *puVar4;
  byte *pbVar5;
  undefined8 *puVar6;
  char cVar7;
  byte bVar8;
  byte bVar9;
  ushort uVar10;
  ushort uVar11;
  long lVar12;
  byte bVar13;
  uint uVar14;
  int iVar15;
  size_t sVar16;
  long lVar17;
  undefined8 uVar18;
  undefined8 uVar19;
  bool bVar20;
  long lVar21;
  long lVar22;
  ulong uVar23;
  ulong uVar24;
  long lVar25;
  int *piVar26;
  long lVar27;
  long lVar28;
  int *piVar29;
  byte bVar30;
  undefined1 auVar31 [16];
  undefined1 auVar32 [16];
  long local_100;
  undefined8 uStack_f8;
  char *local_f0;
  undefined8 local_e8;
  undefined8 local_e0;
  undefined8 uStack_d8;
  undefined8 uStack_d0;
  undefined8 uStack_c8;
  undefined8 local_c0;
  undefined8 uStack_b8;
  undefined8 uStack_b0;
  char *local_a8;
  char *local_a0;
  long lStack_98;
  ulong local_90;
  undefined8 uStack_88;
  char *local_78;
  long local_70;
  
  lVar12 = tpidr_el0;
  local_70 = *(long *)(lVar12 + 0x28);
  uStack_f8 = 0x100000000;
  pcVar1 = param_4 + param_5;
  uStack_d8 = 0;
  local_e0 = 0;
  uStack_c8 = 0;
  uStack_d0 = 0;
  uStack_b8 = 0;
  local_c0 = 0;
  uStack_b0 = 0;
  lStack_98 = 0;
  uStack_88 = 0;
  local_90 = 0;
  local_e8 = 0x100000020;
  local_100 = param_1;
  local_f0 = param_6;
  local_a0 = pcVar1;
  local_78 = param_4;
  if (((*param_4 == '#') && (param_4[1] == '!')) &&
     (local_78 = param_4 + 2, local_a8 = param_4, 2 < param_5)) {
    do {
      cVar7 = *local_78;
      if (cVar7 == '\n' || cVar7 == '\r') break;
      if (cVar7 < '\0') {
        uVar14 = FUN_001d2c34(local_78,6,&local_78);
        if (uVar14 >> 1 == 0x1014) break;
        if (uVar14 == 0xffffffff) goto LAB_00162164;
      }
      else {
LAB_00162164:
        local_78 = local_78 + 1;
      }
    } while (local_78 < pcVar1);
  }
  uVar14 = param_7 & 3;
  local_a8 = local_78;
  if (uVar14 == 2) {
    lVar28 = *(long *)(*(long *)(param_1 + 0x18) + 0xf8);
    if (lVar28 == 0) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x870f,
                "JSValue __JS_EvalInternal(JSContext *, JSValue, const char *, size_t, const char *, int, int)"
                ,"sf != NULL");
    }
    if (*(int *)(lVar28 + 0x10) != -1) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x8710,
                "JSValue __JS_EvalInternal(JSContext *, JSValue, const char *, size_t, const char *, int, int)"
                ,"JS_VALUE_GET_TAG(sf->cur_func) == JS_TAG_OBJECT");
    }
    lVar21 = *(long *)(lVar28 + 8);
    uVar10 = *(ushort *)(lVar21 + 6);
    if (((0x34 < uVar10) || ((1L << ((ulong)uVar10 & 0x3f) & 0x10000000012000U) == 0)) &&
       (uVar10 != 0x38)) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x8712,
                "JSValue __JS_EvalInternal(JSContext *, JSValue, const char *, size_t, const char *, int, int)"
                ,"js_class_has_bytecode(p->class_id)");
    }
    lVar27 = *(long *)(lVar21 + 0x30);
    uVar19 = *(undefined8 *)(lVar21 + 0x38);
    piVar29 = (int *)0x0;
    bVar30 = *(byte *)(lVar27 + 0x18);
  }
  else {
    bVar30 = (byte)(param_7 >> 3) & 3;
    if (uVar14 == 1) {
      sVar16 = strlen(param_6);
      iVar15 = FUN_0013f2ec(param_1,param_6,sVar16);
      if (iVar15 == 0) {
        piVar29 = (int *)0x0;
LAB_001622bc:
        bVar20 = false;
      }
      else {
        piVar29 = (int *)FUN_00159288(param_1,iVar15);
        if (piVar29 == (int *)0x0) goto LAB_001622bc;
        bVar30 = (byte)(param_7 >> 3) & 2 | 1;
        bVar20 = true;
      }
      uVar19 = 0;
      lVar27 = 0;
      lVar28 = 0;
      if (!bVar20) goto LAB_00162880;
    }
    else {
      uVar19 = 0;
      lVar27 = 0;
      lVar28 = 0;
      piVar29 = (int *)0x0;
    }
  }
  lVar21 = FUN_00192ba0(param_1,0,1,0,param_6,1);
  if (lVar21 != 0) {
    *(uint *)(lVar21 + 0x3c) = uVar14;
    *(uint *)(lVar21 + 100) = (uint)(uVar14 != 2);
    *(uint *)(lVar21 + 0x80) = param_7 >> 6 & 1;
    if (uVar14 != 2) {
      uVar14 = 1;
      *(undefined8 *)(lVar21 + 0x68) = 0;
      *(undefined4 *)(lVar21 + 0x70) = 0;
    }
    else {
      *(uint *)(lVar21 + 0x68) = *(ushort *)(lVar27 + 0x19) >> 6 & 1;
      *(uint *)(lVar21 + 0x6c) = *(ushort *)(lVar27 + 0x19) >> 7 & 1;
      *(uint *)(lVar21 + 0x70) = *(ushort *)(lVar27 + 0x19) >> 8 & 1;
      uVar14 = *(ushort *)(lVar27 + 0x19) >> 9 & 1;
    }
    *(uint *)(lVar21 + 0x74) = uVar14;
    *(byte *)(lVar21 + 0x86) = bVar30;
    *(undefined4 *)(lVar21 + 0x88) = 0x52;
    lStack_98 = lVar21;
    if (lVar27 == 0) {
LAB_001627b4:
      *(int **)(lVar21 + 0x218) = piVar29;
      if (((param_7 >> 7 & 1) != 0) || (piVar29 != (int *)0x0)) {
        *(undefined4 *)(lVar21 + 0x7c) = 1;
        *(undefined1 *)(lVar21 + 0x84) = 2;
      }
      local_90 = (ulong)CONCAT14(piVar29 == (int *)0x0,(uint)(piVar29 != (int *)0x0));
      FUN_00192d34(&local_100);
      lVar27 = lStack_98;
      *(undefined4 *)(lVar21 + 0x118) = *(undefined4 *)(lVar21 + 0xe0);
      iVar15 = FUN_001961f4(&local_100);
      if ((iVar15 == 0) && (iVar15 = FUN_00196c6c(&local_100), iVar15 == 0)) {
        if (*(uint *)(lVar27 + 0x3c) < 2) {
          uVar14 = 1;
        }
        else {
          uVar14 = (uint)((*(byte *)(lVar27 + 0x86) & 1) == 0);
        }
        *(uint *)(lVar27 + 0x40) = uVar14;
        if ((int)local_90 == 0) {
          iVar15 = *(int *)(lVar27 + 0x9c);
          if (iVar15 < 0xffff) {
            if ((iVar15 < *(int *)(lVar27 + 0x98)) ||
               (iVar15 = FUN_0016cbcc(local_100,lVar27 + 0x90,0x10,(int *)(lVar27 + 0x98),iVar15 + 1
                                     ), iVar15 == 0)) {
              puVar6 = (undefined8 *)
                       (*(long *)(lVar27 + 0x90) + (long)*(int *)(lVar27 + 0x9c) * 0x10);
              *(int *)(lVar27 + 0x9c) = *(int *)(lVar27 + 0x9c) + 1;
              *puVar6 = 0;
              puVar6[1] = 0;
              *(undefined4 *)((long)puVar6 + 0xc) = 0xffffff00;
              *(undefined4 *)puVar6 = 0x53;
              iVar15 = *(int *)(lVar27 + 0x9c) + -1;
              *(int *)(lVar27 + 200) = iVar15;
              if (iVar15 < 0) goto LAB_00162854;
              goto LAB_0016283c;
            }
          }
          else {
            FUN_001405ec(local_100,"too many local variables");
          }
          *(undefined4 *)(lVar27 + 200) = 0xffffffff;
        }
        else {
LAB_0016283c:
          do {
            lVar17 = lStack_98;
            if ((int)local_e8 == -0x54) {
              if ((int)local_90 == 0) {
                lVar22 = lStack_98 + 0x130;
                if (*(int *)(lStack_98 + 0x164) != (int)uStack_f8) {
                  FUN_001d27c4(lVar22,200);
                  local_78 = (char *)CONCAT44(local_78._4_4_,(int)uStack_f8);
                  FUN_001d26dc(lVar22,&local_78,4);
                  *(int *)(lVar17 + 0x164) = (int)uStack_f8;
                }
                *(int *)(lVar17 + 0x160) = (int)*(undefined8 *)(lVar17 + 0x138);
                FUN_001d27c4(lVar22,0x58);
                local_78 = (char *)CONCAT62(local_78._2_6_,(short)*(undefined4 *)(lVar27 + 200));
                FUN_001d26dc(lStack_98 + 0x130,&local_78,2);
                uVar18 = 1;
              }
              else {
                uVar18 = 0;
              }
              FUN_00197bf4(&local_100,uVar18);
              if (piVar29 != (int *)0x0) {
                *(char *)(piVar29 + 0x20) = (char)*(undefined4 *)(lVar21 + 0x220);
              }
              auVar32 = FUN_00193398(param_1,lVar21);
              if (auVar32._8_4_ == 6) goto LAB_00162870;
              if (piVar29 != (int *)0x0) {
                *(undefined1 (*) [16])(piVar29 + 0x1a) = auVar32;
                iVar15 = FUN_0015a4e0(param_1,piVar29);
                if (iVar15 < 0) goto LAB_00162870;
                auVar32._8_8_ = 0xfffffffffffffffd;
                auVar32._0_8_ = piVar29;
                *piVar29 = *piVar29 + 1;
              }
              if ((param_7 >> 5 & 1) == 0) {
                auVar32 = FUN_00159d48(param_1,auVar32._0_8_,auVar32._8_8_,param_2,param_3,uVar19,
                                       lVar28);
              }
              uVar19 = auVar32._8_8_;
              uVar24 = auVar32._0_8_ & 0xffffffff00000000;
              uVar23 = auVar32._0_8_ & 0xffffffff;
              goto LAB_0016288c;
            }
            iVar15 = FUN_00196f50(&local_100);
          } while (iVar15 == 0);
        }
      }
    }
    else {
      uVar10 = *(ushort *)(lVar27 + 0x40);
      uVar11 = *(ushort *)(lVar27 + 0x42);
      iVar15 = *(int *)(lVar27 + 0x5c);
      *(undefined8 *)(lVar21 + 0x1a0) = 0;
      *(undefined4 *)(lVar21 + 0x198) = 0;
      uVar14 = (uint)uVar11 + (uint)uVar10 + iVar15;
      *(uint *)(lVar21 + 0x19c) = uVar14;
      if (uVar14 == 0) goto LAB_001627b4;
      lVar17 = (*(code *)**(undefined8 **)(param_1 + 0x18))
                         (*(undefined8 **)(param_1 + 0x18) + 4,
                          -(ulong)(uVar14 >> 0x1f) & 0xfffffff800000000 | (ulong)uVar14 << 3);
      if (lVar17 == 0) {
        lVar22 = *(long *)(param_1 + 0x18);
        if ((*(ushort *)(lVar22 + 0xf0) & 0xff) == 0) {
          *(ushort *)(lVar22 + 0xf0) = *(ushort *)(lVar22 + 0xf0) & 0xff00 | 1;
          FUN_001405ec(param_1,"out of memory");
          lVar17 = 0;
          *(ushort *)(lVar22 + 0xf0) = (ushort)*(byte *)(lVar22 + 0xf1) << 8;
        }
        else {
          lVar17 = 0;
        }
      }
      *(long *)(lVar21 + 0x1a0) = lVar17;
      if (lVar17 != 0) {
        if (-1 < param_8) {
          do {
            lVar17 = *(long *)(lVar27 + 0x30);
            iVar15 = param_8 + (uint)*(ushort *)(lVar27 + 0x40);
            if (0 < *(int *)(lVar17 + (long)iVar15 * 0x10 + 4)) {
              pbVar3 = (byte *)(*(long *)(lVar21 + 0x1a0) + (long)*(int *)(lVar21 + 0x198) * 8);
              *(int *)(lVar21 + 0x198) = *(int *)(lVar21 + 0x198) + 1;
              puVar2 = (uint *)(lVar17 + (long)iVar15 * 0x10);
              bVar30 = *pbVar3;
              *pbVar3 = bVar30 & 0xfc | 1;
              bVar13 = (byte)(((byte)puVar2[3] & 1) << 2) | 1;
              *pbVar3 = bVar30 & 0xf8 | bVar13;
              bVar8 = (byte)(((byte)puVar2[3] & 2) << 2);
              *pbVar3 = bVar30 & 0xf0 | bVar13 | bVar8;
              uVar14 = puVar2[3];
              *(short *)(pbVar3 + 2) = (short)param_8;
              *pbVar3 = bVar13 | bVar8 | (byte)uVar14 & 0xf0;
              uVar14 = *puVar2;
              if (0xe3 < (int)uVar14) {
                piVar26 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + (ulong)uVar14 * 8)
                ;
                *piVar26 = *piVar26 + 1;
              }
              *(uint *)(pbVar3 + 4) = uVar14;
            }
            param_8 = *(int *)(lVar17 + (long)iVar15 * 0x10 + 8);
          } while (-1 < param_8);
        }
        if (param_8 == -2) {
          if (*(short *)(lVar27 + 0x42) != 0) {
            lVar17 = 0;
            do {
              lVar25 = *(long *)(lVar27 + 0x30);
              lVar22 = lVar17 + (ulong)*(ushort *)(lVar27 + 0x40);
              puVar2 = (uint *)(lVar25 + lVar22 * 0x10);
              if ((puVar2[1] == 0) &&
                 ((uVar14 = *puVar2 - 0x55,
                  uVar14 < 0x21 && (1L << ((ulong)uVar14 & 0x3f) & 0x1c0000001U) != 0 ||
                  *puVar2 == 8 || ((*(uint *)(lVar25 + lVar22 * 0x10 + 0xc) & 0xf0) == 0x40)))) {
                lVar25 = lVar25 + lVar22 * 0x10;
                pbVar3 = (byte *)(*(long *)(lVar21 + 0x1a0) + (long)*(int *)(lVar21 + 0x198) * 8);
                *(int *)(lVar21 + 0x198) = *(int *)(lVar21 + 0x198) + 1;
                bVar30 = *pbVar3;
                *pbVar3 = bVar30 & 0xfc | 1;
                bVar13 = (byte)((*(byte *)(lVar25 + 0xc) & 1) << 2) | 1;
                *pbVar3 = bVar30 & 0xf8 | bVar13;
                bVar8 = (byte)((*(byte *)(lVar25 + 0xc) & 2) << 2);
                *pbVar3 = bVar30 & 0xf0 | bVar13 | bVar8;
                bVar30 = *(byte *)(lVar25 + 0xc);
                *(short *)(pbVar3 + 2) = (short)lVar17;
                *pbVar3 = bVar13 | bVar8 | bVar30 & 0xf0;
                uVar14 = *puVar2;
                if (0xe3 < (int)uVar14) {
                  piVar26 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) +
                                     (ulong)uVar14 * 8);
                  *piVar26 = *piVar26 + 1;
                }
                *(uint *)(pbVar3 + 4) = uVar14;
              }
              lVar17 = lVar17 + 1;
            } while ((uint)lVar17 < (uint)*(ushort *)(lVar27 + 0x42));
          }
        }
        else {
          if (*(short *)(lVar27 + 0x40) != 0) {
            lVar17 = 0;
            uVar23 = 0;
            do {
              puVar4 = (undefined1 *)
                       (*(long *)(lVar21 + 0x1a0) + (long)*(int *)(lVar21 + 0x198) * 8);
              *(int *)(lVar21 + 0x198) = *(int *)(lVar21 + 0x198) + 1;
              lVar22 = *(long *)(lVar27 + 0x30);
              *puVar4 = 3;
              *(short *)(puVar4 + 2) = (short)uVar23;
              uVar14 = *(uint *)(lVar22 + lVar17);
              if (0xe3 < (int)uVar14) {
                piVar26 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + (ulong)uVar14 * 8)
                ;
                *piVar26 = *piVar26 + 1;
              }
              *(uint *)(puVar4 + 4) = uVar14;
              uVar23 = uVar23 + 1;
              lVar17 = lVar17 + 0x10;
            } while (uVar23 < *(ushort *)(lVar27 + 0x40));
          }
          if (*(short *)(lVar27 + 0x42) != 0) {
            lVar17 = 0;
            do {
              lVar22 = lVar17 + (ulong)*(ushort *)(lVar27 + 0x40);
              puVar2 = (uint *)(*(long *)(lVar27 + 0x30) + lVar22 * 0x10);
              if ((puVar2[1] == 0) && (*puVar2 != 0x53)) {
                lVar22 = *(long *)(lVar27 + 0x30) + lVar22 * 0x10;
                pbVar3 = (byte *)(*(long *)(lVar21 + 0x1a0) + (long)*(int *)(lVar21 + 0x198) * 8);
                *(int *)(lVar21 + 0x198) = *(int *)(lVar21 + 0x198) + 1;
                bVar30 = *pbVar3;
                *pbVar3 = bVar30 & 0xfc | 1;
                bVar13 = (byte)((*(byte *)(lVar22 + 0xc) & 1) << 2) | 1;
                *pbVar3 = bVar30 & 0xf8 | bVar13;
                bVar8 = (byte)((*(byte *)(lVar22 + 0xc) & 2) << 2);
                *pbVar3 = bVar30 & 0xf0 | bVar13 | bVar8;
                bVar30 = *(byte *)(lVar22 + 0xc);
                *(short *)(pbVar3 + 2) = (short)lVar17;
                *pbVar3 = bVar13 | bVar8 | bVar30 & 0xf0;
                uVar14 = *puVar2;
                if (0xe3 < (int)uVar14) {
                  piVar26 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) +
                                     (ulong)uVar14 * 8);
                  *piVar26 = *piVar26 + 1;
                }
                *(uint *)(pbVar3 + 4) = uVar14;
              }
              lVar17 = lVar17 + 1;
            } while ((uint)lVar17 < (uint)*(ushort *)(lVar27 + 0x42));
          }
        }
        if (0 < *(int *)(lVar27 + 0x5c)) {
          lVar22 = 0;
          lVar17 = 0;
          do {
            pbVar3 = (byte *)(*(long *)(lVar21 + 0x1a0) + (long)*(int *)(lVar21 + 0x198) * 8);
            lVar25 = *(long *)(lVar27 + 0x38);
            *(int *)(lVar21 + 0x198) = *(int *)(lVar21 + 0x198) + 1;
            bVar8 = *pbVar3;
            pbVar5 = (byte *)(lVar25 + lVar22);
            *pbVar3 = bVar8 & 0xfe;
            bVar30 = *pbVar5 & 2;
            *pbVar3 = bVar30 | bVar8 & 0xfc;
            bVar13 = *pbVar5 & 4;
            *pbVar3 = bVar30 | bVar8 & 0xf8 | bVar13;
            bVar9 = *pbVar5;
            *pbVar3 = bVar30 | bVar8 & 0xf0 | bVar13 | bVar9 & 8;
            bVar8 = *pbVar5;
            *(short *)(pbVar3 + 2) = (short)lVar17;
            *pbVar3 = bVar30 | bVar13 | bVar9 & 8 | bVar8 & 0xf0;
            uVar14 = *(uint *)(pbVar5 + 4);
            if (0xe3 < (int)uVar14) {
              piVar26 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + (ulong)uVar14 * 8);
              *piVar26 = *piVar26 + 1;
            }
            *(uint *)(pbVar3 + 4) = uVar14;
            lVar17 = lVar17 + 1;
            lVar22 = lVar22 + 8;
          } while (lVar17 < *(int *)(lVar27 + 0x5c));
        }
        goto LAB_001627b4;
      }
    }
LAB_00162854:
    FUN_0015d6ac(&local_100,&local_e8);
    FUN_00192f38(param_1,lVar21);
  }
LAB_00162870:
  if (piVar29 != (int *)0x0) {
    FUN_0016a6e8(param_1,piVar29);
  }
LAB_00162880:
  uVar23 = 0;
  uVar24 = 0;
  uVar19 = 6;
LAB_0016288c:
  if (*(long *)(lVar12 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  auVar31._0_8_ = uVar24 | uVar23;
  auVar31._8_8_ = uVar19;
  return auVar31;
}

/* ===== FUN_00167914 @ 00167914 [libNexusScriptRuntime69252.so] ===== */

undefined1  [16]
FUN_00167914(long param_1,undefined8 param_2,undefined8 param_3,int param_4,long *param_5)

{
  undefined8 uVar1;
  ulong uVar2;
  long lVar3;
  int *piVar4;
  long lVar5;
  long lVar6;
  undefined1 auVar7 [16];
  
  if (param_4 == 0) {
    if (*(uint *)(*(long *)(param_1 + 0x18) + 0x50) < 0x30) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0xc94,"JSValue __JS_AtomToValue(JSContext *, JSAtom, BOOL)","atom < rt->atom_size")
      ;
    }
    piVar4 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + 0x178);
    auVar7._8_8_ = 0xfffffffffffffff9;
    auVar7._0_8_ = piVar4;
    *piVar4 = *piVar4 + 1;
  }
  else {
    if (((int)param_3 == 3) && ((int)param_5[1] == -8)) {
      lVar3 = *param_5;
      if (*(ulong *)(lVar3 + 4) >> 0x3e < 3) {
        lVar5 = *(long *)(param_1 + 0x18);
        uVar2 = (ulong)*(uint *)(*(long *)(lVar5 + 0x58) +
                                (ulong)((uint)(*(ulong *)(lVar3 + 4) >> 0x20) &
                                        *(int *)(lVar5 + 0x48) - 1U & 0x3fffffff) * 4);
        lVar6 = *(long *)(*(long *)(lVar5 + 0x60) + uVar2 * 8);
        while (lVar6 != lVar3) {
          if ((int)uVar2 == 0) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0xaf4,"JSAtom js_get_atom_index(JSRuntime *, JSAtomStruct *)","i != 0");
          }
          uVar2 = (ulong)*(uint *)(lVar6 + 0xc);
          lVar6 = *(long *)(*(long *)(lVar5 + 0x60) + uVar2 * 8);
        }
      }
      else {
        uVar2 = (ulong)*(uint *)(lVar3 + 0xc);
      }
      auVar7 = FUN_00140044(param_1,uVar2,1);
      auVar7 = FUN_00175c1c(param_1,"Symbol(",auVar7._0_8_,auVar7._8_8_,&DAT_0010eb52);
    }
    else {
      auVar7 = FUN_0014c3ec(param_1,*param_5,param_5[1],0);
    }
    if (auVar7._8_4_ == 6) {
      return auVar7;
    }
  }
  uVar1 = auVar7._8_8_;
  lVar3 = auVar7._0_8_;
  if ((int)param_3 != 3) {
    auVar7 = FUN_00174f48(param_1,param_2,param_3,5);
    if (auVar7._8_4_ != 6) {
      FUN_00167548(param_1,auVar7._0_8_,auVar7._8_8_,lVar3,uVar1);
      FUN_00148018(param_1,auVar7._0_8_,auVar7._8_8_,0x30,*(ulong *)(lVar3 + 4) & 0x7fffffff,0,0,3,0
                   ,3,0x2700);
    }
  }
  return auVar7;
}

/* ===== FUN_00169ef0 @ 00169ef0 [libNexusScriptRuntime69252.so] ===== */

void FUN_00169ef0(long param_1,long param_2)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  int *piVar5;
  int *piVar6;
  int *piVar7;
  int *piVar8;
  
  piVar5 = *(int **)(param_2 + 0x30);
  if (piVar5 == (int *)0x0) {
    return;
  }
  if (*(int **)(piVar5 + 4) != piVar5 + 2) {
    piVar8 = *(int **)(piVar5 + 4);
    do {
      piVar6 = piVar8 + -6;
      piVar7 = *(int **)(piVar8 + 2);
      if (piVar8[-5] == 0) {
        if (*piVar5 == 0) {
          piVar4 = *(int **)(piVar8 + 8);
          if (0xfffffff4 < (uint)*(undefined8 *)(piVar8 + 10)) {
            iVar1 = *piVar4;
            iVar2 = iVar1 + -1;
            *piVar4 = iVar2;
            if (iVar2 == 0 || iVar1 < 1) {
              FUN_00141774(param_1,piVar4);
            }
          }
        }
        else {
          piVar4 = (int *)(*(long *)(piVar8 + 8) + 0x28);
          piVar3 = *(int **)piVar4;
          if (piVar3 == (int *)0x0) {
code_r0x0016a058:
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0xb953,"void delete_weak_ref(JSRuntime *, JSMapRecord *)","mr1 != NULL");
          }
          if (piVar3 != piVar6) {
            do {
              piVar4 = piVar3;
              piVar3 = *(int **)(piVar4 + 4);
              if (piVar3 == (int *)0x0) goto code_r0x0016a058;
            } while (piVar3 != piVar6);
            piVar4 = piVar4 + 4;
          }
          *(undefined8 *)piVar4 = *(undefined8 *)(piVar3 + 4);
        }
        piVar4 = *(int **)(piVar8 + 0xc);
        if (0xfffffff4 < (uint)*(undefined8 *)(piVar8 + 0xe)) {
          iVar1 = *piVar4;
          iVar2 = iVar1 + -1;
          *piVar4 = iVar2;
          if (iVar2 == 0 || iVar1 < 1) {
            FUN_00141774(param_1,piVar4);
          }
        }
      }
      (**(code **)(param_1 + 8))(param_1 + 0x20,piVar6);
      piVar8 = piVar7;
    } while (piVar7 != piVar5 + 2);
  }
  (**(code **)(param_1 + 8))(param_1 + 0x20,*(undefined8 *)(piVar5 + 8));
                    /* WARNING: Could not recover jumptable at 0x0016a038. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(param_1 + 8))(param_1 + 0x20,piVar5);
  return;
}

/* ===== FUN_0016a120 @ 0016a120 [libNexusScriptRuntime69252.so] ===== */

void FUN_0016a120(long param_1,long param_2)

{
  long lVar1;
  long *plVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  long *plVar6;
  
  plVar6 = *(long **)(param_2 + 0x30);
  if (plVar6 == (long *)0x0) {
    return;
  }
  if (((((int)plVar6[1] == -1) && ((*(byte *)(*plVar6 + 5) >> 1 & 1) == 0)) &&
      (piVar5 = (int *)plVar6[3], piVar5 != (int *)0x0)) &&
     (iVar3 = *piVar5, *piVar5 = iVar3 + -1, iVar3 + -1 == 0)) {
    if (piVar5[1] == 0) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0xb976,"void map_decref_record(JSRuntime *, JSMapRecord *)","mr->empty");
    }
    lVar1 = *(long *)(piVar5 + 6);
    plVar2 = *(long **)(piVar5 + 8);
    *(long **)(lVar1 + 8) = plVar2;
    *plVar2 = lVar1;
    piVar5[6] = 0;
    piVar5[7] = 0;
    piVar5[8] = 0;
    piVar5[9] = 0;
    (**(code **)(param_1 + 8))(param_1 + 0x20);
  }
  piVar5 = (int *)*plVar6;
  if (0xfffffff4 < (uint)plVar6[1]) {
    iVar3 = *piVar5;
    iVar4 = iVar3 + -1;
    *piVar5 = iVar4;
    if (iVar4 == 0 || iVar3 < 1) {
      FUN_00141774(param_1,piVar5);
    }
  }
                    /* WARNING: Could not recover jumptable at 0x0016a1e4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(param_1 + 8))(param_1 + 0x20,plVar6);
  return;
}

/* ===== FUN_0016a4cc @ 0016a4cc [libNexusScriptRuntime69252.so] ===== */

void FUN_0016a4cc(long param_1,int *param_2)

{
  long *plVar1;
  int iVar2;
  long lVar3;
  int *piVar4;
  long *plVar5;
  undefined8 *puVar6;
  
  if (*param_2 != 4) {
    piVar4 = *(int **)(param_2 + 2);
    if (piVar4 != (int *)0x0) {
      iVar2 = *piVar4;
      *piVar4 = iVar2 + -1;
      if ((iVar2 + -1 == 0) && (*(char *)(param_1 + 0xb8) != '\x02')) {
        plVar5 = (long *)(piVar4 + 2);
        lVar3 = *plVar5;
        plVar1 = *(long **)(piVar4 + 4);
        *(long **)(lVar3 + 8) = plVar1;
        *plVar1 = lVar3;
        *plVar5 = 0;
        piVar4[4] = 0;
        piVar4[5] = 0;
        puVar6 = *(undefined8 **)(param_1 + 0xa0);
        *(long **)(param_1 + 0xa0) = plVar5;
        *plVar5 = param_1 + 0x98;
        *(undefined8 **)(piVar4 + 4) = puVar6;
        *puVar6 = plVar5;
        if (*(char *)(param_1 + 0xb8) == '\0') {
          lVar3 = *(long *)(param_1 + 0xa0);
          *(undefined1 *)(param_1 + 0xb8) = 1;
          while (lVar3 != param_1 + 0x98) {
            if (*(int *)(lVar3 + -8) != 0) {
                    /* WARNING: Subroutine does not return */
              __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                        ,0x15e6,"void free_zero_refcount(JSRuntime *)","p->ref_count == 0");
            }
            FUN_0016b224(param_1);
            lVar3 = *(long *)(param_1 + 0xa0);
          }
          *(undefined1 *)(param_1 + 0xb8) = 0;
        }
      }
      param_2[2] = 0;
      param_2[3] = 0;
    }
    *param_2 = 4;
  }
  return;
}

/* ===== FUN_0016a5b0 @ 0016a5b0 [libNexusScriptRuntime69252.so] ===== */

void * FUN_0016a5b0(long *param_1,size_t param_2)

{
  long lVar1;
  void *pvVar2;
  
  if (param_2 != 0) {
    lVar1 = param_1[1];
    if ((ulong)param_1[2] < lVar1 + param_2) {
      pvVar2 = (void *)0x0;
    }
    else {
      pvVar2 = malloc(param_2);
      if (pvVar2 != (void *)0x0) {
        *param_1 = *param_1 + 1;
        param_1[1] = lVar1 + 8;
      }
    }
    return pvVar2;
  }
                    /* WARNING: Subroutine does not return */
  __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
            ,0x6f5,"void *js_def_malloc(JSMallocState *, size_t)","size != 0");
}

/* ===== FUN_0016aaa4 @ 0016aaa4 [libNexusScriptRuntime69252.so] ===== */

void FUN_0016aaa4(undefined8 *param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  void *__s;
  long lVar4;
  long lVar5;
  long lVar6;
  ulong uVar7;
  
  if ((param_2 & param_2 - 1) == 0) {
    uVar7 = -(ulong)(param_2 >> 0x1f) & 0xfffffffc00000000 | (ulong)param_2 << 2;
    __s = (void *)(*(code *)*param_1)(param_1 + 4,uVar7);
    if (__s != (void *)0x0) {
      memset(__s,0,uVar7);
      if (*(int *)(param_1 + 9) != 0) {
        uVar7 = 0;
        lVar4 = param_1[0xb];
        do {
          uVar2 = *(uint *)(lVar4 + uVar7 * 4);
          if (uVar2 != 0) {
            lVar5 = param_1[0xc];
            do {
              lVar6 = *(long *)(lVar5 + (ulong)uVar2 * 8);
              uVar1 = *(uint *)(lVar6 + 0xc);
              lVar3 = (ulong)(param_2 + 0x3fffffff & 0x3fffffff & *(uint *)(lVar6 + 8)) * 4;
              *(undefined4 *)(lVar6 + 0xc) = *(undefined4 *)((long)__s + lVar3);
              *(uint *)((long)__s + lVar3) = uVar2;
              uVar2 = uVar1;
            } while (uVar1 != 0);
          }
          uVar7 = uVar7 + 1;
        } while (uVar7 < *(uint *)(param_1 + 9));
      }
      (*(code *)param_1[1])(param_1 + 4,param_1[0xb]);
      param_1[0xb] = __s;
      *(uint *)(param_1 + 9) = param_2;
      *(uint *)((long)param_1 + 0x54) = param_2 << 1;
    }
    return;
  }
                    /* WARNING: Subroutine does not return */
  __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
            ,0xa7c,"int JS_ResizeAtomHash(JSRuntime *, int)",
            "(new_hash_size & (new_hash_size - 1)) == 0");
}

/* ===== FUN_0016aef4 @ 0016aef4 [libNexusScriptRuntime69252.so] ===== */

void FUN_0016aef4(long param_1,int *param_2)

{
  long lVar1;
  long *plVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int *piVar6;
  int *piVar7;
  uint uVar8;
  uint *puVar9;
  
  if (*param_2 == 0) {
    if ((char)param_2[6] != '\0') {
      piVar5 = (int *)(*(long *)(param_1 + 0x188) +
                      (ulong)((uint)param_2[7] >> ((ulong)(0x20 - *(int *)(param_1 + 0x178)) & 0x3f)
                             ) * 8);
      do {
        piVar6 = piVar5;
        piVar7 = *(int **)piVar6;
        piVar5 = piVar7 + 0xc;
      } while (piVar7 != param_2);
      iVar3 = *(int *)(param_1 + 0x180);
      *(undefined8 *)piVar6 = *(undefined8 *)(param_2 + 0xc);
      *(int *)(param_1 + 0x180) = iVar3 + -1;
    }
    piVar5 = *(int **)(param_2 + 0xe);
    if (piVar5 != (int *)0x0) {
      iVar3 = *piVar5;
      iVar4 = iVar3 + -1;
      *piVar5 = iVar4;
      if (iVar4 == 0 || iVar3 < 1) {
        FUN_00141774(param_1,piVar5,0xffffffffffffffff);
      }
    }
    if (param_2[10] != 0) {
      uVar8 = 0;
      puVar9 = (uint *)(param_2 + 0x11);
      do {
        if (0xe3 < (int)*puVar9) {
          piVar5 = *(int **)(*(long *)(param_1 + 0x60) + (ulong)*puVar9 * 8);
          iVar3 = *piVar5;
          iVar4 = iVar3 + -1;
          *piVar5 = iVar4;
          if (iVar4 == 0 || iVar3 < 1) {
            FUN_001418dc(param_1);
          }
        }
        uVar8 = uVar8 + 1;
        puVar9 = puVar9 + 2;
      } while (uVar8 < (uint)param_2[10]);
    }
    uVar8 = param_2[8];
    lVar1 = *(long *)(param_2 + 2);
    plVar2 = *(long **)(param_2 + 4);
    *(long **)(lVar1 + 8) = plVar2;
    *plVar2 = lVar1;
    param_2[2] = 0;
    param_2[3] = 0;
    param_2[4] = 0;
    param_2[5] = 0;
                    /* WARNING: Could not recover jumptable at 0x0016b00c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(param_1 + 8))(param_1 + 0x20,param_2 + ~(ulong)uVar8);
    return;
  }
                    /* WARNING: Subroutine does not return */
  __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
            ,0x11c8,"void js_free_shape0(JSRuntime *, JSShape *)","sh->header.ref_count == 0");
}

/* ===== FUN_0016b780 @ 0016b780 [libNexusScriptRuntime69252.so] ===== */

void FUN_0016b780(long param_1,long param_2)

{
  long *plVar1;
  int iVar2;
  int iVar3;
  long lVar4;
  long lVar5;
  int *piVar6;
  
  lVar4 = *(long *)(param_2 + 0x28);
  while( true ) {
    if (lVar4 == 0) {
      if (*(long *)(param_2 + 0x28) != 0) {
        lVar4 = *(long *)(param_2 + 0x28);
        do {
          piVar6 = *(int **)(lVar4 + 0x48);
          lVar5 = *(long *)(lVar4 + 0x10);
          if ((0xfffffff4 < (uint)*(undefined8 *)(lVar4 + 0x50)) &&
             (iVar2 = *piVar6, iVar3 = iVar2 + -1, *piVar6 = iVar3, iVar3 == 0 || iVar2 < 1)) {
            FUN_00141774(param_1,piVar6);
          }
          (**(code **)(param_1 + 8))(param_1 + 0x20,lVar4);
          lVar4 = lVar5;
        } while (lVar5 != 0);
      }
      *(undefined8 *)(param_2 + 0x28) = 0;
      return;
    }
    if (**(int **)(lVar4 + 8) == 0) break;
    if (*(int *)(lVar4 + 4) != 0) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0xb98a,"void reset_weak_ref(JSRuntime *, JSObject *)","!mr->empty");
    }
    lVar5 = *(long *)(lVar4 + 0x28);
    plVar1 = *(long **)(lVar4 + 0x30);
    *plVar1 = lVar5;
    *(long **)(lVar5 + 8) = plVar1;
    lVar5 = *(long *)(lVar4 + 0x18);
    plVar1 = *(long **)(lVar4 + 0x20);
    *(undefined8 *)(lVar4 + 0x28) = 0;
    *(undefined8 *)(lVar4 + 0x30) = 0;
    *(long **)(lVar5 + 8) = plVar1;
    *plVar1 = lVar5;
    *(undefined8 *)(lVar4 + 0x18) = 0;
    *(undefined8 *)(lVar4 + 0x20) = 0;
    lVar4 = *(long *)(lVar4 + 0x10);
  }
                    /* WARNING: Subroutine does not return */
  __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
            ,0xb988,"void reset_weak_ref(JSRuntime *, JSObject *)","s->is_weak");
}

/* ===== FUN_0016b89c @ 0016b89c [libNexusScriptRuntime69252.so] ===== */

void FUN_0016b89c(long param_1,long param_2)

{
  int iVar1;
  int iVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  int *piVar5;
  
  puVar4 = *(undefined8 **)(param_2 + 0x70);
  if (puVar4 != (undefined8 *)0x0) {
    if (*(long *)(param_2 + 0xa0) == 0) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x4ab3,"void async_func_free_frame(JSRuntime *, JSAsyncFunctionState *)",
                "sf->cur_sp != NULL");
    }
    puVar3 = *(undefined8 **)(param_2 + 0xa0);
    for (; puVar4 < puVar3; puVar4 = puVar4 + 2) {
      piVar5 = (int *)*puVar4;
      if (0xfffffff4 < (uint)puVar4[1]) {
        iVar1 = *piVar5;
        iVar2 = iVar1 + -1;
        *piVar5 = iVar2;
        if (iVar2 == 0 || iVar1 < 1) {
          FUN_00141774(param_1,piVar5);
        }
      }
      puVar3 = *(undefined8 **)(param_2 + 0xa0);
    }
    (**(code **)(param_1 + 8))(param_1 + 0x20,*(undefined8 *)(param_2 + 0x70));
    *(undefined8 *)(param_2 + 0x70) = 0;
  }
  piVar5 = *(int **)(param_2 + 0x60);
  if (0xfffffff4 < (uint)*(undefined8 *)(param_2 + 0x68)) {
    iVar1 = *piVar5;
    iVar2 = iVar1 + -1;
    *piVar5 = iVar2;
    if (iVar2 == 0 || iVar1 < 1) {
      FUN_00141774(param_1,piVar5);
    }
  }
  piVar5 = *(int **)(param_2 + 0x18);
  if (0xfffffff4 < (uint)*(undefined8 *)(param_2 + 0x20)) {
    iVar1 = *piVar5;
    iVar2 = iVar1 + -1;
    *piVar5 = iVar2;
    if (iVar2 == 0 || iVar1 < 1) {
      FUN_00141774(param_1,piVar5);
      return;
    }
  }
  return;
}

/* ===== FUN_0016b9cc @ 0016b9cc [libNexusScriptRuntime69252.so] ===== */

void FUN_0016b9cc(long param_1,int *param_2)

{
  int iVar1;
  long *plVar2;
  long *plVar3;
  long lVar4;
  
  if (*param_2 < 1) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0x16cb,"void gc_decref_child(JSRuntime *, JSGCObjectHeader *)","p->ref_count > 0");
  }
  iVar1 = *param_2 + -1;
  *param_2 = iVar1;
  if ((iVar1 == 0) && ((*(byte *)(param_2 + 1) & 0xf0) == 0x10)) {
    plVar3 = (long *)(param_2 + 2);
    lVar4 = *plVar3;
    plVar2 = *(long **)(param_2 + 4);
    *(long **)(lVar4 + 8) = plVar2;
    *plVar2 = lVar4;
    *plVar3 = 0;
    param_2[4] = 0;
    param_2[5] = 0;
    plVar2 = (long *)(param_1 + 0xa8);
    lVar4 = *plVar2;
    *(long **)(lVar4 + 8) = plVar3;
    *plVar3 = lVar4;
    *(long **)(param_2 + 4) = plVar2;
    *plVar2 = (long)plVar3;
    return;
  }
  return;
}

/* ===== FUN_0016c228 @ 0016c228 [libNexusScriptRuntime69252.so] ===== */

void FUN_0016c228(long param_1,long param_2)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  long lVar4;
  bool bVar5;
  undefined1 auVar6 [16];
  uint uVar7;
  int iVar8;
  int *piVar9;
  undefined8 *puVar10;
  undefined8 uVar11;
  long lVar12;
  long lVar13;
  int *piVar14;
  ulong uVar15;
  ulong uVar16;
  undefined8 *puVar17;
  uint uVar18;
  long lVar19;
  long *plVar20;
  undefined1 auVar21 [16];
  long local_b0;
  int *local_a8;
  int *local_a0;
  undefined8 uStack_98;
  long local_90;
  undefined8 uStack_88;
  int *local_78;
  undefined8 uStack_70;
  long local_68;
  
  lVar4 = tpidr_el0;
  local_68 = *(long *)(lVar4 + 0x28);
  if (*(int *)(param_2 + 0x60) == 3) {
    auVar21 = FUN_001410f4(param_1,0xb);
    uVar11 = auVar21._8_8_;
    piVar9 = auVar21._0_8_;
    uVar18 = auVar21._8_4_;
    if (uVar18 != 6) {
      uStack_98 = 0;
      local_a0 = (int *)0x0;
      uStack_88 = 0;
      local_90 = 0;
      iVar8 = FUN_0016c6c8(param_1,&local_a0,param_2,0);
      (**(code **)(*(long *)(param_1 + 0x18) + 8))(*(long *)(param_1 + 0x18) + 0x20,local_a0);
      lVar13 = local_90;
      if (iVar8 == 0) {
        uVar7 = uStack_88._4_4_;
        uVar15 = (ulong)uStack_88._4_4_;
        if (0 < (int)uStack_88._4_4_) {
          uVar16 = 0;
          do {
            puVar1 = (undefined4 *)(lVar13 + uVar16 * 0x10);
            plVar20 = (long *)(puVar1 + 2);
            lVar12 = param_2;
            piVar14 = (int *)*plVar20;
            if ((int *)*plVar20 == (int *)0x0) {
              local_78 = (int *)0x0;
              uStack_70 = 0;
              iVar8 = FUN_0016ccd8(param_1,&local_b0,&local_a8,param_2,*puVar1,&local_78);
              if (0 < uStack_70._4_4_) {
                lVar12 = 0;
                lVar19 = 8;
                do {
                  if ((0xe3 < (int)*(uint *)((long)local_78 + lVar19)) &&
                     (piVar14 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) +
                                         (ulong)*(uint *)((long)local_78 + lVar19) * 8),
                     iVar2 = *piVar14, iVar3 = iVar2 + -1, *piVar14 = iVar3, iVar3 == 0 || iVar2 < 1
                     )) {
                    FUN_001418dc();
                  }
                  lVar12 = lVar12 + 1;
                  lVar19 = lVar19 + 0x10;
                } while (lVar12 < uStack_70._4_4_);
              }
              (**(code **)(*(long *)(param_1 + 0x18) + 8))
                        (*(long *)(param_1 + 0x18) + 0x20,local_78);
              lVar12 = local_b0;
              piVar14 = local_a8;
              if (iVar8 == 0) goto LAB_0016c494;
              if (iVar8 != 3) {
                FUN_0016c910(param_1,iVar8,param_2,*puVar1);
                goto LAB_0016c2c4;
              }
              *(undefined4 *)(lVar13 + (uVar16 * 4 + 1) * 4) = 0;
            }
            else {
LAB_0016c494:
              local_a8 = piVar14;
              local_b0 = lVar12;
              puVar1 = (undefined4 *)(lVar13 + (uVar16 * 4 + 1) * 4);
              if (local_a8[5] == 0x7f) {
                *puVar1 = 2;
                lVar12 = *(long *)(*(long *)(local_b0 + 0x18) + (long)*local_a8 * 0x10 + 8);
              }
              else {
                *puVar1 = 1;
                if (*(long *)(local_a8 + 2) != 0) {
                  *plVar20 = *(long *)(local_a8 + 2);
                  goto LAB_0016c3bc;
                }
                lVar12 = *(long *)(*(long *)(*(long *)(local_b0 + 0x68) + 0x38) +
                                  (long)*local_a8 * 8);
              }
              *plVar20 = lVar12;
            }
LAB_0016c3bc:
            uVar16 = uVar16 + 1;
          } while (uVar16 != uVar15);
        }
        lVar13 = local_90;
        auVar6._8_8_ = uVar11;
        auVar6._0_8_ = local_90;
        FUN_001d2cd8(local_90,(long)(int)uVar7,0x10,FUN_0016c9d8,param_1);
        if (0 < (int)uVar7) {
          uVar15 = (ulong)uVar7;
          puVar17 = (undefined8 *)(lVar13 + 8);
          do {
            if (*(int *)((long)puVar17 + -4) == 2) {
              iVar8 = FUN_0016cad0(param_1,piVar9,uVar11,*(undefined4 *)(puVar17 + -1),1,*puVar17,6)
              ;
              if (iVar8 < 0) {
                bVar5 = true;
                goto LAB_0016c5a0;
              }
LAB_0016c59c:
              bVar5 = false;
            }
            else {
              if (*(int *)((long)puVar17 + -4) != 1) goto LAB_0016c59c;
              piVar14 = (int *)*puVar17;
              puVar10 = (undefined8 *)
                        FUN_001490ec(param_1,piVar9,*(undefined4 *)(puVar17 + -1),0x26);
              if (puVar10 == (undefined8 *)0x0) {
                bVar5 = true;
              }
              else {
                bVar5 = false;
                *piVar14 = *piVar14 + 1;
                *puVar10 = piVar14;
                if (puVar10 != (undefined8 *)0x0) goto LAB_0016c59c;
              }
            }
LAB_0016c5a0:
            if (bVar5) {
              auVar21 = auVar6;
              if (!bVar5) goto LAB_0016c314;
              goto LAB_0016c2c4;
            }
            puVar17 = puVar17 + 2;
            uVar15 = uVar15 - 1;
          } while (uVar15 != 0);
        }
        (**(code **)(*(long *)(param_1 + 0x18) + 8))(*(long *)(param_1 + 0x18) + 0x20,lVar13);
        if (*(uint *)(*(long *)(param_1 + 0x18) + 0x50) < 0x81) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0xc94,"JSValue __JS_AtomToValue(JSContext *, JSAtom, BOOL)",
                    "atom < rt->atom_size");
        }
        lVar13 = *(long *)(*(long *)(param_1 + 0x18) + 0x60);
        piVar14 = *(int **)(lVar13 + 0x400);
        if ((*(ulong *)(piVar14 + 1) >> 0x3e != 1) &&
           ((*(ulong *)(piVar14 + 1) & 0xffffffff) == 0x80000000)) {
          piVar14 = *(int **)(lVar13 + 0x178);
        }
        *piVar14 = *piVar14 + 1;
        FUN_00148018(param_1,piVar9,uVar11,0xdd,piVar14,0xfffffffffffffff9,0,3,0,3,0x2700);
        iVar8 = *piVar14;
        iVar2 = iVar8 + -1;
        *piVar14 = iVar2;
        if (iVar2 == 0 || iVar8 < 1) {
          FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar14,0xfffffffffffffff9);
        }
        *(byte *)((long)piVar9 + 5) = *(byte *)((long)piVar9 + 5) & 0xfe;
      }
      else {
LAB_0016c2c4:
        (**(code **)(*(long *)(param_1 + 0x18) + 8))(*(long *)(param_1 + 0x18) + 0x20,local_90);
        local_78 = piVar9;
        if ((0xfffffff4 < uVar18) &&
           (iVar8 = *piVar9, iVar2 = iVar8 + -1, *piVar9 = iVar2, iVar2 == 0 || iVar8 < 1)) {
          FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar9,uVar11);
        }
        auVar21 = ZEXT816(6) << 0x40;
      }
    }
LAB_0016c314:
    if (auVar21._8_4_ == 6) {
      piVar9 = (int *)0x0;
      uVar11 = 6;
      goto LAB_0016c34c;
    }
    *(undefined1 (*) [16])(param_2 + 0x58) = auVar21;
  }
  piVar9 = *(int **)(param_2 + 0x58);
  uVar11 = *(undefined8 *)(param_2 + 0x60);
  local_a0 = piVar9;
  if (0xfffffff4 < (uint)uVar11) {
    *piVar9 = *piVar9 + 1;
  }
LAB_0016c34c:
  if (*(long *)(lVar4 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(piVar9,uVar11);
  }
  return;
}

/* ===== FUN_0016cad0 @ 0016cad0 [libNexusScriptRuntime69252.so] ===== */

undefined4
FUN_0016cad0(int *param_1,long param_2,int param_3,uint param_4,uint param_5,ulong param_6,
            uint param_7)

{
  uint uVar1;
  ulong *puVar2;
  undefined4 uVar3;
  long lVar4;
  ulong uVar5;
  
  if (param_3 == -1) {
    lVar4 = *(long *)(param_2 + 0x18);
    uVar1 = *(uint *)(lVar4 + ~(ulong)(*(uint *)(lVar4 + 0x20) & param_4) * 4);
    uVar5 = (ulong)uVar1;
    if (uVar1 != 0) {
      do {
        if (*(uint *)(lVar4 + 0x40 + (uVar5 - 1) * 8 + 4) == param_4) {
                    /* WARNING: Subroutine does not return */
          abort();
        }
        uVar1 = *(uint *)(lVar4 + 0x40 + (uVar5 - 1) * 8);
        uVar5 = (ulong)uVar1 & 0x3ffffff;
      } while ((uVar1 & 0x3ffffff) != 0);
    }
    puVar2 = (ulong *)FUN_001490ec(param_1,param_2,param_4,param_7 & 7 | 0x30);
    if (puVar2 == (ulong *)0x0) {
      uVar3 = 0xffffffff;
    }
    else {
      *param_1 = *param_1 + 1;
      *puVar2 = (ulong)param_1;
      if (((ulong)param_1 & 3) != 0) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0x2587,
                  "int JS_DefineAutoInitProperty(JSContext *, JSValue, JSAtom, JSAutoInitIDEnum, void *, int)"
                  ,"(pr->u.init.realm_and_id & 3) == 0");
      }
      uVar3 = 1;
      *puVar2 = (ulong)param_5 | (ulong)param_1;
      puVar2[1] = param_6;
    }
  }
  else {
    uVar3 = 0;
  }
  return uVar3;
}

/* ===== FUN_0016d090 @ 0016d090 [libNexusScriptRuntime69252.so] ===== */

undefined8 FUN_0016d090(long param_1,uint *param_2,uint param_3)

{
  undefined8 uVar1;
  ulong uVar2;
  uint uVar3;
  ulong uVar4;
  long lVar5;
  long lVar6;
  uint uVar7;
  uint uVar8;
  
  if ((int)param_3 < 0) {
    param_3 = param_3 & 0x7fffffff;
    uVar1 = 1;
    goto LAB_0016d190;
  }
  if (*(uint *)(*(long *)(param_1 + 0x18) + 0x50) <= param_3) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0xcbb,"BOOL JS_AtomIsArrayIndex(JSContext *, uint32_t *, JSAtom)",
              "atom < rt->atom_size");
  }
  lVar6 = *(long *)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + (ulong)param_3 * 8);
  uVar4 = *(ulong *)(lVar6 + 4);
  if (uVar4 >> 0x3e == 1) {
    uVar3 = (uint)uVar4;
    uVar8 = uVar3 & 0x7fffffff;
    if (0xfffffff5 < uVar8 - 0xb) {
      if ((int)uVar3 < 0) {
        uVar7 = (uint)*(ushort *)(lVar6 + 0x10);
      }
      else {
        uVar7 = (uint)*(byte *)(lVar6 + 0x10);
      }
      uVar2 = (ulong)(uVar7 - 0x30);
      if (uVar7 - 0x30 < 10) {
        if (uVar7 == 0x30) {
          uVar2 = 0;
          if (uVar8 != 1) goto LAB_0016d188;
        }
        else if (1 < uVar8) {
          lVar5 = 0;
          do {
            if ((int)uVar3 < 0) {
              uVar8 = (uint)*(ushort *)(lVar6 + 0x11 + lVar5 * 2 + 1);
            }
            else {
              uVar8 = (uint)*(byte *)(lVar6 + 0x11 + lVar5);
            }
            if ((9 < uVar8 - 0x30) ||
               (uVar2 = (long)(int)(uVar8 - 0x30) + (uVar2 & 0xffffffff) * 10, uVar2 >> 0x20 != 0))
            goto LAB_0016d188;
            lVar5 = lVar5 + 1;
          } while ((uVar4 & 0x7fffffff) - 1 != lVar5);
        }
        param_3 = (uint)uVar2;
        if (param_3 != 0xffffffff) {
          uVar1 = 1;
          goto LAB_0016d190;
        }
      }
    }
  }
LAB_0016d188:
  param_3 = 0;
  uVar1 = 0;
LAB_0016d190:
  *param_2 = param_3;
  return uVar1;
}

/* ===== FUN_0016d318 @ 0016d318 [libNexusScriptRuntime69252.so] ===== */

void FUN_0016d318(long param_1,long param_2,undefined8 param_3)

{
  undefined4 uVar1;
  long lVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  uint local_40;
  uint uStack_3c;
  long local_38;
  
  lVar2 = tpidr_el0;
  local_38 = *(long *)(lVar2 + 0x28);
  uVar1 = *(undefined4 *)(param_2 + 4);
  iVar3 = FUN_0016d090(param_3,&uStack_3c,*(undefined4 *)(param_1 + 4));
  iVar4 = FUN_0016d090(param_3,&local_40,uVar1);
  if ((iVar3 == 0) || (iVar4 == 0)) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0x1e31,"int num_keys_cmp(const void *, const void *, void *)",
              "atom1_is_integer && atom2_is_integer");
  }
  uVar5 = (uint)(uStack_3c != local_40);
  if (uStack_3c < local_40) {
    uVar5 = 0xffffffff;
  }
  if (*(long *)(lVar2 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_0016efc4 @ 0016efc4 [libNexusScriptRuntime69252.so] ===== */

char * FUN_0016efc4(double param_1,char *param_2,uint param_3,uint param_4,uint param_5)

{
  uint uVar1;
  long lVar2;
  bool bVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  char *pcVar10;
  char *pcVar11;
  ulong uVar12;
  char cVar13;
  char *pcVar14;
  size_t sVar15;
  ulong uVar16;
  ulong uVar17;
  ulong uVar18;
  double dVar19;
  char local_280 [128];
  char local_200;
  undefined1 auStack_1ff [127];
  char local_180 [127];
  char acStack_101 [68];
  char local_bd [61];
  long local_80;
  
  dVar19 = ABS(param_1);
  lVar2 = tpidr_el0;
  local_80 = *(long *)(lVar2 + 0x28);
  if ((dVar19 < INFINITY) || (dVar19 != INFINITY && dVar19 < INFINITY == NAN(dVar19))) {
    if (param_5 == 0) {
      if (((param_1 != 9007199254740991.0 && param_1 < 9007199254740991.0 == NAN(param_1)) ||
          (param_1 < -9007199254740991.0)) || ((double)(long)param_1 != param_1)) goto LAB_0016f1e4;
      uVar17 = (ulong)param_1;
      uVar12 = (ulong)param_3;
      local_bd[2] = 0;
      pcVar14 = local_bd + 1;
      uVar16 = -uVar17;
      if (-1 < (long)uVar17) {
        uVar16 = uVar17;
      }
      do {
        pcVar10 = pcVar14;
        uVar18 = 0;
        if (uVar12 != 0) {
          uVar18 = uVar16 / uVar12;
        }
        iVar4 = (int)uVar16 - (int)uVar18 * param_3;
        cVar13 = '0';
        if (9 < iVar4) {
          cVar13 = 'W';
        }
        bVar3 = uVar12 <= uVar16;
        pcVar14 = pcVar10 + -1;
        *pcVar10 = cVar13 + (char)iVar4;
        uVar16 = uVar18;
      } while (bVar3);
      if ((long)uVar17 < 0) {
        *pcVar14 = '-';
        pcVar10 = pcVar14;
      }
      param_2 = strcpy(param_2,pcVar10);
    }
    else {
      dVar19 = 0.0;
      if (param_1 != 0.0) {
        dVar19 = param_1;
      }
      param_1 = dVar19;
      if (param_5 == 2) {
        uVar7 = param_4 + 1;
        iVar4 = snprintf(acStack_101 + 1,0x80,"%.*f",dVar19,(ulong)uVar7);
        if (0x7f < iVar4) goto code_r0x0016f6f8;
        if (acStack_101[iVar4] == '5') {
          fesetround(2);
          iVar4 = snprintf(acStack_101 + 1,0x80,"%.*f",dVar19,(ulong)uVar7);
          fesetround(0);
          if (0x7f < iVar4) goto code_r0x0016f6f8;
          fesetround(1);
          iVar5 = snprintf(local_180,0x80,"%.*f",dVar19,(ulong)uVar7);
          fesetround(0);
          if (0x7f < iVar5) goto code_r0x0016f6f8;
          if ((iVar4 != iVar5) ||
             (iVar4 = memcmp(acStack_101 + 1,local_180,(long)iVar4), iVar4 != 0)) goto LAB_0016f13c;
          iVar4 = 1;
          if (acStack_101[1] == '-') {
            iVar4 = 2;
          }
          if (iVar4 != 0) {
            fesetround(iVar4);
          }
        }
        else {
LAB_0016f13c:
          iVar4 = 0;
        }
        uVar6 = snprintf(param_2,0x80,"%.*f",dVar19,(ulong)param_4);
        uVar7 = uVar6;
        if (iVar4 != 0) {
          uVar7 = fesetround(0);
        }
        param_2 = (char *)(ulong)uVar7;
        if (0x7f < (int)uVar6) {
code_r0x0016f6f8:
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x2de8,"int js_fcvt1(char *, int, double, int, int)","n < buf_size");
        }
      }
      else {
LAB_0016f1e4:
        if ((param_5 & 3) == 1) {
          pcVar14 = acStack_101 + 1;
          sVar15 = (size_t)(int)param_4;
          snprintf(acStack_101 + 1,0x80,"%+.*e",param_1,(ulong)param_4);
          local_180[0] = acStack_101[2];
          if (0 < (int)param_4) {
            memcpy((void *)((ulong)local_180 | 1),(void *)((ulong)pcVar14 | 3),(long)(int)param_4);
          }
          iVar4 = 0;
          pcVar10 = pcVar14;
          if (0 < (int)param_4) {
            pcVar10 = acStack_101 + 2;
          }
          local_180[sVar15 + 1] = '\0';
          uVar7 = param_4;
          if (local_180[sVar15] == '5') {
            fesetround(2);
            snprintf(acStack_101 + 1,0x80,"%+.*e",param_1,(ulong)param_4);
            fesetround(0);
            cVar13 = acStack_101[1];
            local_180[0] = acStack_101[2];
            if (0 < (int)param_4) {
              memcpy((void *)((ulong)local_180 | 1),(void *)((ulong)pcVar14 | 3),sVar15);
            }
            local_180[sVar15 + 1] = '\0';
            iVar5 = atoi(pcVar10 + sVar15 + 3);
            fesetround(1);
            snprintf(acStack_101 + 1,0x80,"%+.*e",param_1,(ulong)param_4);
            fesetround(0);
            local_200 = acStack_101[2];
            if (0 < (int)param_4) {
              memcpy((void *)((ulong)&local_200 | 1),(void *)((ulong)(acStack_101 + 1) | 3),sVar15);
            }
            auStack_1ff[sVar15] = 0;
            iVar8 = atoi(pcVar10 + sVar15 + 3);
            iVar9 = memcmp(local_180,&local_200,sVar15 + 1);
            iVar4 = 1;
            if (cVar13 == '-') {
              iVar4 = 2;
            }
            if (iVar5 != iVar8 || iVar9 != 0) {
              iVar4 = 0;
            }
          }
        }
        else {
          uVar7 = 1;
          uVar16 = 0x11;
          do {
            uVar6 = uVar7 + (int)uVar16;
            uVar1 = uVar6 >> 1;
            uVar17 = (ulong)uVar1;
            snprintf(acStack_101 + 1,0x80,"%+.*e",param_1,(ulong)(uVar1 - 1));
            local_280[0] = acStack_101[2];
            if (3 < uVar6) {
              memcpy((void *)((ulong)local_280 | 1),(void *)((ulong)(acStack_101 + 1) | 3),
                     (long)(int)(uVar1 - 1));
            }
            local_280[uVar17] = '\0';
            dVar19 = strtod(acStack_101 + 1,(char **)0x0);
            if (dVar19 == param_1) {
              uVar12 = uVar17;
              if (uVar17 != 0) {
                uVar12 = 1;
              }
              do {
                uVar16 = uVar12;
                if ((long)uVar17 < 2) break;
                uVar18 = uVar17 - 1;
                uVar16 = uVar17;
                uVar17 = uVar18;
              } while (local_280[uVar18 & 0xffffffff] == '0');
            }
            else {
              uVar7 = uVar1 + 1;
            }
            param_4 = (uint)uVar16;
          } while (uVar7 < param_4);
          iVar4 = 0;
          uVar7 = 0x15;
        }
        if (iVar4 != 0) {
          fesetround(iVar4);
        }
        snprintf(acStack_101 + 1,0x80,"%+.*e",param_1,(ulong)(param_4 - 1));
        if (iVar4 != 0) {
          fesetround(0);
        }
        local_280[0] = acStack_101[2];
        if (1 < (int)param_4) {
          memcpy((void *)((ulong)local_280 | 1),(void *)((ulong)(acStack_101 + 1) | 3),
                 (long)(int)(param_4 - 1));
        }
        sVar15 = (size_t)(int)param_4;
        pcVar14 = acStack_101 + 1;
        if (param_4 - 1 != 0 && 0 < (int)param_4) {
          pcVar14 = acStack_101 + 2;
        }
        local_280[sVar15] = '\0';
        uVar6 = atoi(pcVar14 + sVar15 + 2);
        pcVar14 = param_2;
        if (acStack_101[1] == '-') {
          pcVar14 = param_2 + 1;
          *param_2 = '-';
        }
        if ((param_5 >> 2 & 1) != 0) {
LAB_0016f5ac:
          pcVar10 = pcVar14 + 1;
          *pcVar14 = local_280[0];
          if (1 < (int)param_4) {
            pcVar14[1] = '.';
            memcpy(pcVar14 + 2,(void *)((ulong)local_280 | 1),(ulong)(param_4 - 1));
            pcVar10 = pcVar14 + (ulong)(param_4 - 2) + 3;
          }
          param_2 = pcVar10 + 1;
          *pcVar10 = 'e';
          if (-1 < (int)uVar6) {
            param_2 = pcVar10 + 2;
            pcVar10[1] = '+';
          }
          if (*(long *)(lVar2 + 0x28) == local_80) {
            uVar7 = sprintf(param_2,"%d",(ulong)uVar6);
            return (char *)(ulong)uVar7;
          }
          goto LAB_0016f724;
        }
        if (((int)uVar6 < 0) || ((int)uVar7 <= (int)uVar6)) {
          if (uVar6 < 0xfffffffa) goto LAB_0016f5ac;
          pcVar10 = pcVar14 + 2;
          pcVar14[0] = '0';
          pcVar14[1] = '.';
          if (uVar6 != 0xffffffff) {
            uVar7 = 1;
            if (1 < (int)~uVar6) {
              uVar7 = ~uVar6;
            }
            memset(pcVar10,0x30,(ulong)uVar7);
            pcVar10 = pcVar14 + (ulong)(uVar7 - 1) + 3;
          }
          param_2 = (char *)memcpy(pcVar10,local_280,sVar15);
          pcVar10[sVar15] = '\0';
        }
        else {
          uVar7 = uVar6 + 1;
          uVar1 = param_4 - uVar7;
          uVar16 = (ulong)uVar1;
          if (uVar1 == 0 || (int)param_4 < (int)uVar7) {
            memcpy(pcVar14,local_280,sVar15);
            param_2 = pcVar14 + sVar15;
            if ((int)param_4 <= (int)uVar6) {
              memset(param_2,0x30,(ulong)(uVar6 - param_4) + 1);
              param_2 = pcVar14 + (uVar6 - param_4) + sVar15 + 1;
            }
            *param_2 = '\0';
          }
          else {
            param_2 = (char *)memcpy(pcVar14,local_280,(ulong)uVar7);
            pcVar10 = pcVar14 + uVar7 + 1;
            pcVar14[uVar7] = '.';
            if (0 < (int)uVar1) {
              pcVar14 = pcVar10;
              pcVar11 = local_280 + (int)uVar7;
              do {
                uVar16 = uVar16 - 1;
                pcVar10 = pcVar14 + 1;
                *pcVar14 = *pcVar11;
                pcVar14 = pcVar10;
                pcVar11 = pcVar11 + 1;
              } while (uVar16 != 0);
            }
            *pcVar10 = '\0';
          }
        }
      }
    }
  }
  else {
    if (!NAN(param_1)) {
      pcVar14 = param_2;
      if (param_1 < 0.0) {
        pcVar14 = param_2 + 1;
        *param_2 = '-';
      }
      if (*(long *)(lVar2 + 0x28) == local_80) {
        pcVar14[8] = '\0';
        *(undefined8 *)pcVar14 = s_Infinity_0010cb48._0_8_;
        return param_2;
      }
      goto LAB_0016f724;
    }
    builtin_strncpy(param_2,"NaN",4);
  }
  if (*(long *)(lVar2 + 0x28) == local_80) {
    return param_2;
  }
LAB_0016f724:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(param_2);
}

/* ===== FUN_001710cc @ 001710cc [libNexusScriptRuntime69252.so] ===== */

undefined8 FUN_001710cc(long param_1,long param_2,int param_3,long param_4,int param_5)

{
  uint uVar1;
  bool bVar2;
  char *pcVar3;
  long lVar4;
  long *plVar5;
  long lVar6;
  ulong uVar7;
  ulong uVar8;
  long lVar9;
  
  if (param_5 == -1) {
    pcVar3 = "not an object";
    if (((*(ushort *)(param_4 + 6) < 0x39) &&
        ((1L << ((ulong)*(ushort *)(param_4 + 6) & 0x3f) & 0x110000000012000U) != 0)) &&
       (lVar6 = *(long *)(param_4 + 0x40), pcVar3 = "not an object", lVar6 != 0)) {
      lVar4 = *(long *)(lVar6 + 0x18);
      uVar1 = *(uint *)(lVar4 + ((ulong)(~*(uint *)(lVar4 + 0x20) << 2) | 0xfffffffffffffca8));
      uVar8 = (ulong)uVar1;
      if (uVar1 != 0) {
        do {
          lVar9 = uVar8 - 1;
          if (*(int *)(lVar4 + 0x40 + lVar9 * 8 + 4) == 0xd5) {
            bVar2 = false;
            plVar5 = (long *)(*(long *)(lVar6 + 0x20) + lVar9 * 0x10);
            break;
          }
          plVar5 = (long *)0x0;
          uVar1 = *(uint *)(lVar4 + 0x40 + lVar9 * 8);
          uVar8 = (ulong)uVar1 & 0x3ffffff;
          bVar2 = true;
        } while ((uVar1 & 0x3ffffff) != 0);
        if (!bVar2) {
          pcVar3 = "not an object";
          if (((int)plVar5[1] == -8) && (pcVar3 = "not an object", param_3 == -1)) {
            lVar6 = *plVar5;
            if (*(ulong *)(lVar6 + 4) >> 0x3e < 3) {
              lVar9 = *(long *)(param_1 + 0x18);
              uVar8 = (ulong)*(uint *)(*(long *)(lVar9 + 0x58) +
                                      (ulong)((uint)(*(ulong *)(lVar6 + 4) >> 0x20) &
                                              *(int *)(lVar9 + 0x48) - 1U & 0x3fffffff) * 4);
              lVar4 = *(long *)(*(long *)(lVar9 + 0x60) + uVar8 * 8);
              while (lVar4 != lVar6) {
                if ((int)uVar8 == 0) {
                    /* WARNING: Subroutine does not return */
                  __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                            ,0xaf4,"JSAtom js_get_atom_index(JSRuntime *, JSAtomStruct *)","i != 0")
                  ;
                }
                uVar8 = (ulong)*(uint *)(lVar4 + 0xc);
                lVar4 = *(long *)(*(long *)(lVar9 + 0x60) + uVar8 * 8);
              }
            }
            else {
              uVar8 = (ulong)*(uint *)(lVar6 + 0xc);
            }
            lVar6 = *(long *)(param_2 + 0x18);
            uVar1 = *(uint *)(lVar6 + ~(*(uint *)(lVar6 + 0x20) & uVar8) * 4);
            uVar7 = (ulong)uVar1;
            if (uVar1 != 0) {
              do {
                if (*(int *)(lVar6 + 0x40 + (uVar7 - 1) * 8 + 4) == (int)uVar8) {
                  return 1;
                }
                uVar1 = *(uint *)(lVar6 + 0x40 + (uVar7 - 1) * 8);
                uVar7 = (ulong)uVar1 & 0x3ffffff;
              } while ((uVar1 & 0x3ffffff) != 0);
              return 0;
            }
            return 0;
          }
          goto LAB_0017118c;
        }
      }
      pcVar3 = "expecting <brand> private field";
    }
  }
  else {
    pcVar3 = "not an object";
  }
LAB_0017118c:
  FUN_00143570(param_1,pcVar3);
  return 0xffffffff;
}

/* ===== FUN_00174bc8 @ 00174bc8 [libNexusScriptRuntime69252.so] ===== */

void FUN_00174bc8(long param_1,long param_2)

{
  undefined8 *puVar1;
  int *piVar2;
  long *plVar3;
  undefined8 uVar4;
  int iVar5;
  long lVar6;
  long *plVar7;
  undefined8 *puVar8;
  undefined8 *puVar9;
  
  if (*(undefined8 **)(param_2 + 0x30) != (undefined8 *)(param_2 + 0x28)) {
    puVar9 = *(undefined8 **)(param_2 + 0x30);
    do {
      puVar1 = (undefined8 *)puVar9[1];
      piVar2 = (int *)puVar9[2];
      if (((piVar2 != (int *)0x0) && (iVar5 = *piVar2, *piVar2 = iVar5 + -1, iVar5 + -1 == 0)) &&
         (*(char *)(param_1 + 0xb8) != '\x02')) {
        plVar7 = (long *)(piVar2 + 2);
        lVar6 = *plVar7;
        plVar3 = *(long **)(piVar2 + 4);
        *(long **)(lVar6 + 8) = plVar3;
        *plVar3 = lVar6;
        *plVar7 = 0;
        piVar2[4] = 0;
        piVar2[5] = 0;
        puVar8 = *(undefined8 **)(param_1 + 0xa0);
        *(long **)(param_1 + 0xa0) = plVar7;
        *plVar7 = param_1 + 0x98;
        *(undefined8 **)(piVar2 + 4) = puVar8;
        *puVar8 = plVar7;
        if (*(char *)(param_1 + 0xb8) == '\0') {
          lVar6 = *(long *)(param_1 + 0xa0);
          *(undefined1 *)(param_1 + 0xb8) = 1;
          while (lVar6 != param_1 + 0x98) {
            if (*(int *)(lVar6 + -8) != 0) {
                    /* WARNING: Subroutine does not return */
              __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                        ,0x15e6,"void free_zero_refcount(JSRuntime *)","p->ref_count == 0");
            }
            FUN_0016b224(param_1);
            lVar6 = *(long *)(param_1 + 0xa0);
          }
          *(undefined1 *)(param_1 + 0xb8) = 0;
        }
      }
      if ((*(byte *)((long)puVar9 + -0x1b) >> 1 & 1) == 0) {
        lVar6 = *(long *)(param_2 + 0x20);
      }
      else {
        lVar6 = *(long *)(param_2 + 0x18);
      }
      puVar8 = (undefined8 *)(lVar6 + (ulong)*(ushort *)((long)puVar9 + -0x1a) * 0x10);
      piVar2 = (int *)*puVar8;
      uVar4 = puVar8[1];
      if (0xfffffff4 < (uint)uVar4) {
        *piVar2 = *piVar2 + 1;
      }
      *puVar9 = piVar2;
      puVar9[1] = uVar4;
      puVar9[-1] = puVar9;
      *(byte *)((long)puVar9 + -0x1b) = *(byte *)((long)puVar9 + -0x1b) | 1;
      puVar9 = puVar1;
    } while (puVar1 != (undefined8 *)(param_2 + 0x28));
  }
  return;
}

/* ===== FUN_001774b4 @ 001774b4 [libNexusScriptRuntime69252.so] ===== */

undefined1  [16] FUN_001774b4(long param_1,long param_2)

{
  long lVar1;
  long lVar2;
  undefined1 auVar3 [16];
  
  if (*(int *)(param_2 + 0x30) != 0) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0x4ac4,"JSValue async_func_resume(JSContext *, JSAsyncFunctionState *)",
              "!s->is_completed");
  }
  lVar2 = *(long *)(param_1 + 0x18);
  if (&stack0xffffffffffffffd0 < *(undefined1 **)(lVar2 + 0xd8)) {
    FUN_001405ec(param_1,"stack overflow");
    auVar3 = ZEXT816(6) << 0x40;
  }
  else {
    auVar3 = FUN_0014cb88(param_1,param_2,0,*(undefined8 *)(param_2 + 0x18),
                          *(undefined8 *)(param_2 + 0x20),0,3,*(undefined4 *)(param_2 + 0x28),
                          *(undefined8 *)(param_2 + 0x70),4);
    if (auVar3._8_4_ != 6) {
      if (auVar3._8_4_ != 3) {
        return auVar3;
      }
      lVar1 = *(long *)(param_2 + 0xa0);
      auVar3 = *(undefined1 (*) [16])(lVar1 + -0x10);
      *(undefined4 *)(lVar1 + -0x10) = 0;
      *(undefined8 *)(lVar1 + -8) = 3;
    }
  }
  *(undefined4 *)(param_2 + 0x30) = 1;
  FUN_00174bc8(lVar2,param_2 + 0x58);
  FUN_0016b89c(lVar2,param_2);
  return auVar3;
}

/* ===== FUN_00178438 @ 00178438 [libNexusScriptRuntime69252.so] ===== */

undefined1  [16] FUN_00178438(long param_1,int param_2,long param_3)

{
  undefined1 (*pauVar1) [16];
  undefined8 *puVar2;
  undefined8 uVar3;
  int iVar4;
  long lVar5;
  bool bVar6;
  int iVar7;
  undefined8 uVar8;
  ulong uVar9;
  long lVar10;
  ulong uVar11;
  int *piVar12;
  undefined8 uVar13;
  undefined1 auVar14 [16];
  undefined1 local_70 [16];
  int *local_60;
  undefined8 uStack_58;
  long local_48;
  
  lVar5 = tpidr_el0;
  local_48 = *(long *)(lVar5 + 0x28);
  if (param_2 != 5) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0xbc0b,"JSValue promise_reaction_job(JSContext *, int, JSValue *)","argc == 5");
  }
  local_60 = *(int **)(param_3 + 0x30);
  uVar8 = *(undefined8 *)(param_3 + 0x20);
  uVar3 = *(undefined8 *)(param_3 + 0x28);
  if (0xfffffff4 < (uint)*(undefined8 *)(param_3 + 0x38)) {
    *local_60 = *local_60 + 1;
  }
  iVar7 = FUN_001442a8(param_1);
  pauVar1 = (undefined1 (*) [16])(param_3 + 0x40);
  uVar13 = *(undefined8 *)(param_3 + 0x48);
  piVar12 = *(int **)*pauVar1;
  auVar14 = *pauVar1;
  local_60 = piVar12;
  uStack_58 = uVar13;
  if ((int)uVar3 == 3) {
    bVar6 = 0xfffffff4 < (uint)uVar13;
    if (iVar7 == 0) {
      local_70 = *pauVar1;
      if (bVar6) {
        *piVar12 = *piVar12 + 1;
        local_70 = auVar14;
      }
    }
    else {
      if (bVar6) {
        *piVar12 = *piVar12 + 1;
      }
      lVar10 = *(long *)(param_1 + 0x18);
      local_70._0_8_ = *(undefined8 *)(lVar10 + 0xe0);
      if ((0xfffffff4 < (uint)*(undefined8 *)(lVar10 + 0xe8)) &&
         (iVar7 = *(int *)local_70._0_8_, iVar4 = iVar7 + -1, *(int *)local_70._0_8_ = iVar4,
         iVar4 == 0 || iVar7 < 1)) {
        FUN_00141774(lVar10,local_70._0_8_);
      }
      *(int **)(lVar10 + 0xe0) = piVar12;
      *(undefined8 *)(lVar10 + 0xe8) = uVar13;
      local_70 = ZEXT816(6) << 0x40;
    }
  }
  else {
    local_70 = FUN_0014cb88(param_1,uVar8,uVar3,0,3,0,3,1,&local_60,2);
  }
  uVar9 = local_70._8_8_ & 0xffffffff;
  if (uVar9 == 6) {
    lVar10 = *(long *)(param_1 + 0x18);
    local_70 = *(undefined1 (*) [16])(lVar10 + 0xe0);
    *(undefined4 *)(lVar10 + 0xe0) = 0;
    *(undefined8 *)(lVar10 + 0xe8) = 2;
  }
  puVar2 = (undefined8 *)(param_3 + (ulong)(uVar9 == 6) * 0x10);
  uVar8 = puVar2[1];
  if ((int)uVar8 == 3) {
    uVar11 = 0;
    uVar9 = 0;
    uVar8 = 3;
  }
  else {
    auVar14 = FUN_0014cb88(param_1,*puVar2,uVar8,0,3,0,3,1,local_70,2);
    uVar8 = auVar14._8_8_;
    uVar9 = auVar14._0_8_ & 0xffffffff00000000;
    uVar11 = auVar14._0_8_ & 0xffffffff;
  }
  if ((0xfffffff4 < local_70._8_4_) &&
     (iVar7 = *(int *)local_70._0_8_, iVar4 = iVar7 + -1, *(int *)local_70._0_8_ = iVar4,
     iVar4 == 0 || iVar7 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_70._0_8_);
  }
  if (*(long *)(lVar5 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  auVar14._0_8_ = uVar9 | uVar11;
  auVar14._8_8_ = uVar8;
  return auVar14;
}

/* ===== FUN_00178b00 @ 00178b00 [libNexusScriptRuntime69252.so] ===== */

int FUN_00178b00(long param_1,long param_2,long *param_3,int param_4)

{
  uint uVar1;
  int iVar2;
  long lVar3;
  bool bVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  long lVar8;
  undefined8 uVar9;
  ulong uVar10;
  long lVar11;
  long lVar12;
  int *piVar13;
  long lVar14;
  undefined1 auVar15 [16];
  long local_88;
  int *local_80;
  int *local_78;
  undefined8 uStack_70;
  long local_68;
  
  lVar3 = tpidr_el0;
  local_68 = *(long *)(lVar3 + 0x28);
  if (&stack0xffffffffffffffa0 < *(undefined1 **)(*(long *)(param_1 + 0x18) + 0xd8)) {
    FUN_001405ec(param_1,"stack overflow");
    param_4 = -1;
  }
  else {
    uVar1 = *(uint *)(param_2 + 0x80) >> 0x18;
    if ((5 < uVar1) || ((1 << (ulong)(uVar1 & 0x1f) & 0x36U) == 0)) {
      if (uVar1 != 0) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0x6eeb,
                  "int js_inner_module_linking(JSContext *, JSModuleDef *, JSModuleDef **, int)",
                  "m->status == JS_MODULE_STATUS_UNLINKED");
      }
      *(int *)(param_2 + 0x84) = param_4;
      *(int *)(param_2 + 0x88) = param_4;
      param_4 = param_4 + 1;
      *(uint *)(param_2 + 0x80) = *(uint *)(param_2 + 0x80) & 0xffffff | 0x1000000;
      *(long *)(param_2 + 0x90) = *param_3;
      *param_3 = param_2;
      if (0 < *(int *)(param_2 + 0x20)) {
        lVar11 = 0;
        lVar12 = 8;
        do {
          lVar8 = *(long *)(*(long *)(param_2 + 0x18) + lVar12);
          param_4 = FUN_00178b00(param_1,lVar8,param_3,param_4);
          if (param_4 < 0) goto LAB_001790c8;
          uVar1 = *(uint *)(lVar8 + 0x80) >> 0x18;
          if (5 < uVar1 || (1 << (ulong)(uVar1 & 0x1f) & 0x36U) == 0) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0x6efd,
                      "int js_inner_module_linking(JSContext *, JSModuleDef *, JSModuleDef **, int)"
                      ,
                      "m1->status == JS_MODULE_STATUS_LINKING || m1->status == JS_MODULE_STATUS_LINKED || m1->status == JS_MODULE_STATUS_EVALUATING_ASYNC || m1->status == JS_MODULE_STATUS_EVALUATED"
                     );
          }
          if (uVar1 == 1) {
            iVar5 = *(int *)(param_2 + 0x88);
            if (*(int *)(lVar8 + 0x88) <= *(int *)(param_2 + 0x88)) {
              iVar5 = *(int *)(lVar8 + 0x88);
            }
            *(int *)(param_2 + 0x88) = iVar5;
          }
          if (param_4 < 0) goto LAB_001790c8;
          lVar11 = lVar11 + 1;
          lVar12 = lVar12 + 0x10;
        } while (lVar11 < *(int *)(param_2 + 0x20));
      }
      if (0 < *(int *)(param_2 + 0x30)) {
        lVar11 = 0;
        do {
          lVar12 = *(long *)(param_2 + 0x28);
          piVar13 = (int *)(lVar12 + lVar11 * 0x20);
          if ((piVar13[4] == 1) && (piVar13[5] != 0x7f)) {
            local_78 = (int *)0x0;
            uStack_70 = 0;
            iVar5 = FUN_0016ccd8(param_1,&local_88,&local_80,
                                 *(undefined8 *)
                                  (*(long *)(param_2 + 0x18) + (long)*piVar13 * 0x10 + 8),piVar13[5]
                                 ,&local_78);
            if (0 < uStack_70._4_4_) {
              lVar8 = 0;
              lVar14 = 8;
              do {
                if ((0xe3 < (int)*(uint *)((long)local_78 + lVar14)) &&
                   (piVar13 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) +
                                       (ulong)*(uint *)((long)local_78 + lVar14) * 8),
                   iVar6 = *piVar13, iVar2 = iVar6 + -1, *piVar13 = iVar2, iVar2 == 0 || iVar6 < 1))
                {
                  FUN_001418dc();
                }
                lVar8 = lVar8 + 1;
                lVar14 = lVar14 + 0x10;
              } while (lVar8 < uStack_70._4_4_);
            }
            (**(code **)(*(long *)(param_1 + 0x18) + 8))(*(long *)(param_1 + 0x18) + 0x20,local_78);
            if (iVar5 == 0) goto LAB_00178ca0;
            FUN_0016c910(param_1,iVar5,param_2,*(undefined4 *)(lVar12 + lVar11 * 0x20 + 0x18));
            iVar6 = 5;
            if (iVar5 == 0) goto LAB_00178ca0;
          }
          else {
LAB_00178ca0:
            iVar6 = 0;
          }
          if (iVar6 != 0) goto LAB_001790c8;
          lVar11 = lVar11 + 1;
        } while (lVar11 < *(int *)(param_2 + 0x30));
      }
      if (*(long *)(param_2 + 0x78) == 0) {
        lVar11 = *(long *)(*(long *)(param_2 + 0x68) + 0x38);
        if (0 < *(int *)(param_2 + 0x50)) {
          lVar12 = 0;
          do {
            piVar13 = (int *)(*(long *)(param_2 + 0x48) + lVar12 * 0xc);
            iVar5 = piVar13[1];
            uVar9 = *(undefined8 *)(*(long *)(param_2 + 0x18) + (long)piVar13[2] * 0x10 + 8);
            if (iVar5 == 0x7f) {
              auVar15 = FUN_0016c228(param_1,uVar9);
              uVar10 = auVar15._8_8_ & 0xffffffff;
              if (uVar10 == 6) goto LAB_001790c8;
              lVar8 = *(long *)(lVar11 + (long)*piVar13 * 8);
              local_78 = *(int **)(lVar8 + 0x20);
              *(undefined1 (*) [16])(lVar8 + 0x20) = auVar15;
              if ((0xfffffff4 < (uint)*(undefined8 *)(lVar8 + 0x28)) &&
                 (iVar5 = *local_78, iVar6 = iVar5 + -1, *local_78 = iVar6, iVar6 == 0 || iVar5 < 1)
                 ) {
                FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_78);
              }
              if (uVar10 == 6) goto LAB_001790c8;
            }
            else {
              local_78 = (int *)0x0;
              uStack_70 = 0;
              iVar5 = FUN_0016ccd8(param_1,&local_88,&local_80,uVar9,iVar5,&local_78);
              if (0 < uStack_70._4_4_) {
                lVar8 = 0;
                lVar14 = 8;
                do {
                  if ((0xe3 < (int)*(uint *)((long)local_78 + lVar14)) &&
                     (piVar7 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) +
                                        (ulong)*(uint *)((long)local_78 + lVar14) * 8),
                     iVar6 = *piVar7, iVar2 = iVar6 + -1, *piVar7 = iVar2, iVar2 == 0 || iVar6 < 1))
                  {
                    FUN_001418dc();
                  }
                  lVar8 = lVar8 + 1;
                  lVar14 = lVar14 + 0x10;
                } while (lVar8 < uStack_70._4_4_);
              }
              (**(code **)(*(long *)(param_1 + 0x18) + 8))
                        (*(long *)(param_1 + 0x18) + 0x20,local_78);
              if (iVar5 == 0) {
                if (local_80[5] == 0x7f) {
                  auVar15 = FUN_0016c228(param_1,*(undefined8 *)
                                                  (*(long *)(local_88 + 0x18) +
                                                   (long)*local_80 * 0x10 + 8));
                  if (auVar15._8_4_ == 6) {
LAB_0017901c:
                    bVar4 = false;
                    iVar5 = 5;
                  }
                  else {
                    lVar8 = FUN_00178a28(param_1,1);
                    if (lVar8 == 0) {
                      FUN_0013a308(param_1,auVar15._0_8_,auVar15._8_8_);
                      goto LAB_0017901c;
                    }
                    FUN_0013ed44(param_1,lVar8 + 0x20,auVar15._0_8_,auVar15._8_8_);
                    iVar5 = 0;
                    bVar4 = true;
                    *(long *)(lVar11 + (long)*piVar13 * 8) = lVar8;
                  }
                  if (!bVar4) goto LAB_00179030;
                }
                else {
                  piVar7 = *(int **)(local_80 + 2);
                  if (piVar7 == (int *)0x0) {
                    piVar7 = *(int **)(*(long *)(*(long *)(local_88 + 0x68) + 0x38) +
                                      (long)*local_80 * 8);
                  }
                  *piVar7 = *piVar7 + 1;
                  *(int **)(lVar11 + (long)*piVar13 * 8) = piVar7;
                }
                iVar5 = 0;
              }
              else {
                FUN_0016c910(param_1,iVar5,uVar9,piVar13[1]);
                iVar5 = 5;
              }
LAB_00179030:
              if (iVar5 != 0) goto LAB_001790c8;
            }
            lVar12 = lVar12 + 1;
          } while (lVar12 < *(int *)(param_2 + 0x50));
        }
        if (0 < *(int *)(param_2 + 0x30)) {
          lVar8 = 0;
          lVar12 = 0;
          do {
            piVar13 = (int *)(*(long *)(param_2 + 0x28) + lVar8);
            if (piVar13[4] == 0) {
              piVar7 = *(int **)(lVar11 + (long)*piVar13 * 8);
              *piVar7 = *piVar7 + 1;
              *(int **)(piVar13 + 2) = piVar7;
            }
            lVar12 = lVar12 + 1;
            lVar8 = lVar8 + 0x20;
          } while (lVar12 < *(int *)(param_2 + 0x30));
        }
        auVar15 = FUN_0014cb88(param_1,*(undefined8 *)(param_2 + 0x68),
                               *(undefined8 *)(param_2 + 0x70),1,1,0,3,0,0,2);
        if (auVar15._8_4_ == 6) {
LAB_001790c8:
          param_4 = -1;
          goto LAB_00178b88;
        }
        FUN_0013a308(param_1,auVar15._0_8_,auVar15._8_8_);
      }
      if (*(int *)(param_2 + 0x84) < *(int *)(param_2 + 0x88)) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0x6f7f,
                  "int js_inner_module_linking(JSContext *, JSModuleDef *, JSModuleDef **, int)",
                  "m->dfs_ancestor_index <= m->dfs_index");
      }
      if (*(int *)(param_2 + 0x84) == *(int *)(param_2 + 0x88)) {
        do {
          lVar11 = *param_3;
          *param_3 = *(long *)(lVar11 + 0x90);
          *(undefined1 *)(lVar11 + 0x83) = 2;
        } while (lVar11 != param_2);
      }
    }
  }
LAB_00178b88:
  if (*(long *)(lVar3 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return param_4;
}

/* ===== FUN_0017914c @ 0017914c [libNexusScriptRuntime69252.so] ===== */

/* WARNING: Removing unreachable block (ram,0x00179358) */

int FUN_0017914c(long param_1,long param_2,int param_3,long *param_4,undefined8 *param_5)

{
  undefined8 uVar1;
  uint uVar2;
  int iVar3;
  undefined1 uVar4;
  undefined8 uVar5;
  long lVar6;
  int *piVar7;
  long lVar8;
  long lVar9;
  
  if (&stack0xffffffffffffffb0 < *(undefined1 **)(*(long *)(param_1 + 0x18) + 0xd8)) {
    FUN_001405ec(param_1,"stack overflow");
LAB_00179190:
    lVar6 = *(long *)(param_1 + 0x18);
    param_3 = -1;
    uVar1 = *(undefined8 *)(lVar6 + 0xe0);
    uVar5 = *(undefined8 *)(lVar6 + 0xe8);
    *(undefined4 *)(lVar6 + 0xe0) = 0;
    *(undefined8 *)(lVar6 + 0xe8) = 2;
    *param_5 = uVar1;
  }
  else {
    uVar2 = *(uint *)(param_2 + 0x80) >> 0x18;
    if (uVar2 - 4 < 2) {
      if (*(char *)(param_2 + 0xf0) != '\0') {
        piVar7 = *(int **)(param_2 + 0xf8);
        uVar5 = *(undefined8 *)(param_2 + 0x100);
LAB_00179208:
        if (0xfffffff4 < (uint)uVar5) {
          *piVar7 = *piVar7 + 1;
        }
        param_3 = -1;
        *param_5 = piVar7;
        goto LAB_00179430;
      }
    }
    else if (uVar2 == 2) {
      *(int *)(param_2 + 0x84) = param_3;
      *(int *)(param_2 + 0x88) = param_3;
      *(undefined4 *)(param_2 + 0xa8) = 0;
      param_3 = param_3 + 1;
      *(uint *)(param_2 + 0x80) = *(uint *)(param_2 + 0x80) & 0xffffff | 0x3000000;
      *(long *)(param_2 + 0x90) = *param_4;
      *param_4 = param_2;
      if (0 < *(int *)(param_2 + 0x20)) {
        lVar6 = 0;
        lVar9 = 8;
        do {
          lVar8 = *(long *)(*(long *)(param_2 + 0x18) + lVar9);
          param_3 = FUN_0017914c(param_1,lVar8,param_3,param_4,param_5);
          if (param_3 < 0) {
            return -1;
          }
          uVar2 = *(uint *)(lVar8 + 0x80) >> 0x18;
          if (2 < uVar2 - 3) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0x71c9,
                      "int js_inner_module_evaluation(JSContext *, JSModuleDef *, int, JSModuleDef **, JSValue *)"
                      ,
                      "m1->status == JS_MODULE_STATUS_EVALUATING || m1->status == JS_MODULE_STATUS_EVALUATING_ASYNC || m1->status == JS_MODULE_STATUS_EVALUATED"
                     );
          }
          if (uVar2 == 3) {
            iVar3 = *(int *)(param_2 + 0x88);
            if (*(int *)(lVar8 + 0x88) <= *(int *)(param_2 + 0x88)) {
              iVar3 = *(int *)(lVar8 + 0x88);
            }
            *(int *)(param_2 + 0x88) = iVar3;
          }
          else {
            lVar8 = *(long *)(lVar8 + 0xb8);
            if (*(uint *)(lVar8 + 0x80) >> 0x19 != 2) {
                    /* WARNING: Subroutine does not return */
              __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                        ,0x71d0,
                        "int js_inner_module_evaluation(JSContext *, JSModuleDef *, int, JSModuleDef **, JSValue *)"
                        ,
                        "m1->status == JS_MODULE_STATUS_EVALUATING_ASYNC || m1->status == JS_MODULE_STATUS_EVALUATED"
                       );
            }
            if (*(char *)(lVar8 + 0xf0) != '\0') {
              piVar7 = *(int **)(lVar8 + 0xf8);
              uVar5 = *(undefined8 *)(lVar8 + 0x100);
              goto LAB_00179208;
            }
          }
          if (*(int *)(lVar8 + 0xac) != 0) {
            *(int *)(param_2 + 0xa8) = *(int *)(param_2 + 0xa8) + 1;
            if ((*(int *)(lVar8 + 0xa4) <= *(int *)(lVar8 + 0xa0)) &&
               (iVar3 = FUN_0016cbcc(param_1,lVar8 + 0x98,8,(int *)(lVar8 + 0xa4),
                                     *(int *)(lVar8 + 0xa0) + 1), iVar3 != 0)) goto LAB_00179190;
            iVar3 = *(int *)(lVar8 + 0xa0);
            *(int *)(lVar8 + 0xa0) = iVar3 + 1;
            *(long *)(*(long *)(lVar8 + 0x98) + (long)iVar3 * 8) = param_2;
          }
          lVar6 = lVar6 + 1;
          lVar9 = lVar9 + 0x10;
        } while (lVar6 < *(int *)(param_2 + 0x20));
      }
      if (*(int *)(param_2 + 0xa8) < 1) {
        if (*(char *)(param_2 + 0x80) == '\0') {
          iVar3 = FUN_001796f4(param_1,param_2,param_5);
          if (iVar3 < 0) {
            return -1;
          }
        }
        else {
          if (*(int *)(param_2 + 0xac) != 0) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0x71e6,
                      "int js_inner_module_evaluation(JSContext *, JSModuleDef *, int, JSModuleDef **, JSValue *)"
                      ,"!m->async_evaluation");
          }
          *(undefined4 *)(param_2 + 0xac) = 1;
          lVar6 = *(long *)(*(long *)(param_1 + 0x18) + 0x148);
          *(long *)(*(long *)(param_1 + 0x18) + 0x148) = lVar6 + 1;
          *(long *)(param_2 + 0xb0) = lVar6;
          FUN_001794fc(param_1,param_2);
        }
      }
      else {
        if (*(int *)(param_2 + 0xac) != 0) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x71e1,
                    "int js_inner_module_evaluation(JSContext *, JSModuleDef *, int, JSModuleDef **, JSValue *)"
                    ,"!m->async_evaluation");
        }
        *(undefined4 *)(param_2 + 0xac) = 1;
        lVar6 = *(long *)(*(long *)(param_1 + 0x18) + 0x148);
        *(long *)(*(long *)(param_1 + 0x18) + 0x148) = lVar6 + 1;
        *(long *)(param_2 + 0xb0) = lVar6;
      }
      if (*(int *)(param_2 + 0x84) < *(int *)(param_2 + 0x88)) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0x71f0,
                  "int js_inner_module_evaluation(JSContext *, JSModuleDef *, int, JSModuleDef **, JSValue *)"
                  ,"m->dfs_ancestor_index <= m->dfs_index");
      }
      if (*(int *)(param_2 + 0x84) == *(int *)(param_2 + 0x88)) {
        do {
          lVar6 = *param_4;
          *param_4 = *(long *)(lVar6 + 0x90);
          *(long *)(lVar6 + 0xb8) = param_2;
          uVar4 = 4;
          if (*(int *)(lVar6 + 0xac) == 0) {
            uVar4 = 5;
          }
          *(undefined1 *)(lVar6 + 0x83) = uVar4;
        } while (lVar6 != param_2);
      }
    }
    else if (uVar2 != 3) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x71b6,
                "int js_inner_module_evaluation(JSContext *, JSModuleDef *, int, JSModuleDef **, JSValue *)"
                ,"m->status == JS_MODULE_STATUS_LINKED");
    }
    uVar5 = 3;
    *(undefined4 *)param_5 = 0;
  }
LAB_00179430:
  param_5[1] = uVar5;
  return param_3;
}

/* ===== FUN_001798dc @ 001798dc [libNexusScriptRuntime69252.so] ===== */

undefined1  [16] FUN_001798dc(long param_1)

{
  uint uVar1;
  int iVar2;
  long lVar3;
  undefined1 auVar4 [16];
  ulong uVar5;
  int iVar6;
  long *in_x6;
  long lVar7;
  ulong uVar8;
  int *piVar9;
  long lVar10;
  int *local_78 [2];
  long local_68;
  ulong local_60;
  long local_58;
  
  lVar3 = tpidr_el0;
  local_58 = *(long *)(lVar3 + 0x28);
  lVar7 = *in_x6;
  uVar1 = *(uint *)(lVar7 + 0x80) >> 0x18;
  if (uVar1 == 4) {
    if (*(char *)(lVar7 + 0xf0) != '\0') {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x7133,
                "JSValue js_async_module_execution_fulfilled(JSContext *, JSValue, int, JSValue *, int, JSValue *)"
                ,"!module->eval_has_exception");
    }
    if (*(int *)(lVar7 + 0xac) == 0) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x7134,
                "JSValue js_async_module_execution_fulfilled(JSContext *, JSValue, int, JSValue *, int, JSValue *)"
                ,"module->async_evaluation");
    }
    *(undefined4 *)(lVar7 + 0xac) = 0;
    FUN_00179e04(param_1,lVar7);
    local_68 = 0;
    local_60 = 0;
    iVar6 = FUN_00179ef4(param_1,lVar7,&local_68);
    uVar5 = local_60;
    lVar7 = local_68;
    if (iVar6 < 0) {
      uVar8 = 6;
    }
    else {
      iVar6 = (int)local_60;
      FUN_001d2cd8(local_68,(long)(int)local_60,8,FUN_0017a0dc,0);
      if (iVar6 < 1) {
        uVar8 = 3;
      }
      else {
        lVar10 = 0;
        uVar8 = 3;
        do {
          piVar9 = *(int **)(lVar7 + lVar10);
          if ((uint)piVar9[0x20] >> 0x18 == 5) {
            if ((char)piVar9[0x3c] == '\0') {
                    /* WARNING: Subroutine does not return */
              __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                        ,29000,
                        "JSValue js_async_module_execution_fulfilled(JSContext *, JSValue, int, JSValue *, int, JSValue *)"
                        ,"m->eval_has_exception");
            }
          }
          else if ((piVar9[0x20] & 0xffU) == 0) {
            iVar6 = FUN_001796f4(param_1,piVar9,local_78);
            if (iVar6 < 0) {
              *piVar9 = *piVar9 + 1;
              FUN_00179b84(param_1);
              iVar6 = *piVar9;
              iVar2 = iVar6 + -1;
              *piVar9 = iVar2;
              if (iVar2 == 0 || iVar6 < 1) {
                FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar9,0xfffffffffffffffd);
              }
              if (0xfffffff4 < (uint)local_78[1]) {
                iVar6 = *local_78[0];
                iVar2 = iVar6 + -1;
                *local_78[0] = iVar2;
                if (iVar2 == 0 || iVar6 < 1) {
                  FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_78[0]);
                }
              }
            }
            else {
              FUN_00179e04(param_1,piVar9);
            }
          }
          else {
            FUN_001794fc(param_1,piVar9);
          }
          lVar10 = lVar10 + 8;
        } while ((uVar5 & 0xffffffff) * 8 - lVar10 != 0);
      }
    }
    (**(code **)(*(long *)(param_1 + 0x18) + 8))(*(long *)(param_1 + 0x18) + 0x20,lVar7);
  }
  else {
    if (uVar1 != 5) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x7132,
                "JSValue js_async_module_execution_fulfilled(JSContext *, JSValue, int, JSValue *, int, JSValue *)"
                ,"module->status == JS_MODULE_STATUS_EVALUATING_ASYNC");
    }
    if (*(char *)(lVar7 + 0xf0) == '\0') {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x712f,
                "JSValue js_async_module_execution_fulfilled(JSContext *, JSValue, int, JSValue *, int, JSValue *)"
                ,"module->eval_has_exception");
    }
    uVar8 = 3;
  }
  if (*(long *)(lVar3 + 0x28) == local_58) {
    auVar4._8_8_ = 0;
    auVar4._0_8_ = uVar8;
    return auVar4 << 0x40;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00179b84 @ 00179b84 [libNexusScriptRuntime69252.so] ===== */

undefined8 FUN_00179b84(int *param_1)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  long lVar4;
  int *piVar5;
  undefined8 uVar6;
  undefined8 *in_x4;
  long *in_x6;
  int *piVar7;
  long lVar8;
  long lVar9;
  undefined1 auVar10 [12];
  int *local_60;
  undefined8 uStack_58;
  long local_48;
  
  lVar4 = tpidr_el0;
  local_48 = *(long *)(lVar4 + 0x28);
  uStack_58 = in_x4[1];
  local_60 = (int *)*in_x4;
  lVar8 = *in_x6;
  if (&stack0xffffffffffffffc0 < *(undefined1 **)(*(long *)(param_1 + 6) + 0xd8)) {
    piVar5 = (int *)FUN_001405ec(param_1,"stack overflow");
    uVar6 = 6;
  }
  else {
    uVar2 = *(uint *)(lVar8 + 0x80) >> 0x18;
    piVar5 = param_1;
    if (uVar2 == 4) {
      if (*(char *)(lVar8 + 0xf0) != '\0') {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0x710e,
                  "JSValue js_async_module_execution_rejected(JSContext *, JSValue, int, JSValue *, int, JSValue *)"
                  ,"!module->eval_has_exception");
      }
      if (*(int *)(lVar8 + 0xac) == 0) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0x710f,
                  "JSValue js_async_module_execution_rejected(JSContext *, JSValue, int, JSValue *, int, JSValue *)"
                  ,"module->async_evaluation");
      }
      *(undefined1 *)(lVar8 + 0xf0) = 1;
      if (0xfffffff4 < (uint)uStack_58) {
        *local_60 = *local_60 + 1;
      }
      *(int **)(lVar8 + 0xf8) = local_60;
      *(undefined8 *)(lVar8 + 0x100) = uStack_58;
      *(undefined1 *)(lVar8 + 0x83) = 5;
      if (0 < *(int *)(lVar8 + 0xa0)) {
        lVar9 = 0;
        do {
          piVar7 = *(int **)(*(long *)(lVar8 + 0x98) + lVar9 * 8);
          *piVar7 = *piVar7 + 1;
          piVar5 = (int *)FUN_00179b84(param_1);
          iVar1 = *piVar7;
          iVar3 = iVar1 + -1;
          *piVar7 = iVar3;
          if (iVar3 == 0 || iVar1 < 1) {
            piVar5 = (int *)FUN_00141774(*(undefined8 *)(param_1 + 6),piVar7,0xfffffffffffffffd);
          }
          lVar9 = lVar9 + 1;
        } while (lVar9 < *(int *)(lVar8 + 0xa0));
      }
      if (*(int *)(lVar8 + 200) != 3) {
        if (*(long *)(lVar8 + 0xb8) != lVar8) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x711f,
                    "JSValue js_async_module_execution_rejected(JSContext *, JSValue, int, JSValue *, int, JSValue *)"
                    ,"module->cycle_root == module");
        }
        auVar10 = FUN_0014cb88(param_1,*(undefined8 *)(lVar8 + 0xe0),*(undefined8 *)(lVar8 + 0xe8),0
                               ,3,0,3,1,&local_60,2);
        piVar5 = auVar10._0_8_;
        if (0xfffffff4 < auVar10._8_4_) {
          iVar1 = *piVar5;
          iVar3 = iVar1 + -1;
          *piVar5 = iVar3;
          if (iVar3 == 0 || iVar1 < 1) {
            piVar5 = (int *)FUN_00141774(*(undefined8 *)(param_1 + 6),piVar5);
          }
        }
      }
    }
    else {
      if (uVar2 != 5) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0x710d,
                  "JSValue js_async_module_execution_rejected(JSContext *, JSValue, int, JSValue *, int, JSValue *)"
                  ,"module->status == JS_MODULE_STATUS_EVALUATING_ASYNC");
      }
      if (*(char *)(lVar8 + 0xf0) == '\0') {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0x7109,
                  "JSValue js_async_module_execution_rejected(JSContext *, JSValue, int, JSValue *, int, JSValue *)"
                  ,"module->eval_has_exception");
      }
    }
    uVar6 = 3;
  }
  if (*(long *)(lVar4 + 0x28) == local_48) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(piVar5,uVar6);
}

/* ===== FUN_00179e04 @ 00179e04 [libNexusScriptRuntime69252.so] ===== */

void FUN_00179e04(long param_1,long param_2)

{
  int iVar1;
  int iVar2;
  long lVar3;
  int *piVar4;
  undefined1 auVar5 [12];
  undefined4 local_38 [2];
  undefined8 local_30;
  long local_28;
  
  lVar3 = tpidr_el0;
  local_28 = *(long *)(lVar3 + 0x28);
  *(undefined1 *)(param_2 + 0x83) = 5;
  if (*(int *)(param_2 + 200) != 3) {
    if (*(long *)(param_2 + 0xb8) != param_2) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x70b8,"void js_set_module_evaluated(JSContext *, JSModuleDef *)",
                "m->cycle_root == m");
    }
    local_30 = 3;
    local_38[0] = 0;
    auVar5 = FUN_0014cb88(param_1,*(undefined8 *)(param_2 + 0xd0),*(undefined8 *)(param_2 + 0xd8),0,
                          3,0,3,1,local_38,2);
    piVar4 = auVar5._0_8_;
    if (0xfffffff4 < auVar5._8_4_) {
      iVar1 = *piVar4;
      iVar2 = iVar1 + -1;
      *piVar4 = iVar2;
      if (iVar2 == 0 || iVar1 < 1) {
        FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar4);
      }
    }
  }
  if (*(long *)(lVar3 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00179ef4 @ 00179ef4 [libNexusScriptRuntime69252.so] ===== */

undefined8 FUN_00179ef4(long param_1,long param_2,long *param_3)

{
  uint uVar1;
  long lVar2;
  int iVar3;
  undefined8 uVar4;
  ulong uVar5;
  long *plVar6;
  long lVar7;
  long lVar8;
  
  if (&stack0xffffffffffffffc0 < *(undefined1 **)(*(long *)(param_1 + 0x18) + 0xd8)) {
    FUN_001405ec(param_1,"stack overflow");
LAB_00179f2c:
    uVar4 = 0xffffffff;
  }
  else {
    if (0 < *(int *)(param_2 + 0xa0)) {
      lVar8 = 0;
      do {
        uVar1 = *(uint *)(param_3 + 1);
        uVar5 = (ulong)uVar1;
        lVar7 = *(long *)(*(long *)(param_2 + 0x98) + lVar8 * 8);
        if (0 < (int)uVar1) {
          plVar6 = (long *)*param_3;
          do {
            if (*plVar6 == lVar7) goto LAB_00179f60;
            uVar5 = uVar5 - 1;
            plVar6 = plVar6 + 1;
          } while (uVar5 != 0);
        }
        if (*(char *)(*(long *)(lVar7 + 0xb8) + 0xf0) == '\0') {
          if (*(char *)(lVar7 + 0x83) != '\x04') {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0x70de,
                      "int gather_available_ancestors(JSContext *, JSModuleDef *, ExecModuleList *)"
                      ,"m->status == JS_MODULE_STATUS_EVALUATING_ASYNC");
          }
          if (*(char *)(lVar7 + 0xf0) != '\0') {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0x70df,
                      "int gather_available_ancestors(JSContext *, JSModuleDef *, ExecModuleList *)"
                      ,"!m->eval_has_exception");
          }
          if (*(int *)(lVar7 + 0xac) == 0) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0x70e0,
                      "int gather_available_ancestors(JSContext *, JSModuleDef *, ExecModuleList *)"
                      ,"m->async_evaluation");
          }
          if (*(int *)(lVar7 + 0xa8) < 1) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0x70e1,
                      "int gather_available_ancestors(JSContext *, JSModuleDef *, ExecModuleList *)"
                      ,"m->pending_async_dependencies > 0");
          }
          iVar3 = *(int *)(lVar7 + 0xa8) + -1;
          *(int *)(lVar7 + 0xa8) = iVar3;
          if (iVar3 == 0) {
            if ((*(int *)((long)param_3 + 0xc) <= (int)uVar1) &&
               (iVar3 = FUN_0016cbcc(param_1,param_3,8,(int *)((long)param_3 + 0xc),uVar1 + 1),
               iVar3 != 0)) goto LAB_00179f2c;
            lVar2 = param_3[1];
            *(long *)(*param_3 + (long)(int)lVar2 * 8) = lVar7;
            *(int *)(param_3 + 1) = (int)lVar2 + 1;
            if ((*(char *)(lVar7 + 0x80) == '\0') &&
               (iVar3 = FUN_00179ef4(param_1,lVar7,param_3), iVar3 != 0)) goto LAB_00179f2c;
          }
        }
LAB_00179f60:
        lVar8 = lVar8 + 1;
      } while (lVar8 < *(int *)(param_2 + 0xa0));
    }
    uVar4 = 0;
  }
  return uVar4;
}

/* ===== FUN_0017b948 @ 0017b948 [libNexusScriptRuntime69252.so] ===== */

void FUN_0017b948(undefined8 *param_1,long param_2)

{
  undefined8 *puVar1;
  uint uVar2;
  ulong uVar3;
  long lVar4;
  int iVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  uint *puVar8;
  uint uVar9;
  uint uVar10;
  ulong local_50;
  long local_48;
  
  lVar4 = tpidr_el0;
  local_48 = *(long *)(lVar4 + 0x28);
  puVar8 = *(uint **)(param_2 + 0x30);
  if ((char)puVar8[1] != '\0') {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0x8b15,"int JS_WriteSharedArrayBuffer(BCWriterState *, JSValue)","!abuf->detached");
  }
  puVar1 = param_1 + 1;
  FUN_001d27c4(puVar1,0x12);
  uVar10 = *puVar8;
  uVar9 = uVar10;
  if (0x7f < uVar10) {
    do {
      uVar10 = uVar9 >> 7;
      FUN_001d27c4(puVar1,uVar9 | 0xffffff80);
      uVar2 = uVar9 >> 0xe;
      uVar9 = uVar10;
    } while (uVar2 != 0);
  }
  FUN_001d27c4(puVar1,uVar10 & 0x7f);
  local_50 = *(ulong *)(puVar8 + 2);
  if (*(char *)(param_1 + 7) != '\0') {
    uVar3 = (local_50 & 0xff00ff00ff00ff00) >> 8 | (local_50 & 0xff00ff00ff00ff) << 8;
    uVar3 = (uVar3 & 0xffff0000ffff0000) >> 0x10 | (uVar3 & 0xffff0000ffff) << 0x10;
    local_50 = uVar3 >> 0x20 | uVar3 << 0x20;
  }
  FUN_001d26dc(puVar1,&local_50,8);
  if ((*(int *)(param_1 + 0xd) < *(int *)((long)param_1 + 0x6c)) ||
     (iVar5 = FUN_0016cbcc(*param_1,param_1 + 0xc,8,(int *)((long)param_1 + 0x6c),
                           *(int *)(param_1 + 0xd) + 1), iVar5 == 0)) {
    iVar5 = *(int *)(param_1 + 0xd);
    uVar6 = 0;
    uVar7 = *(undefined8 *)(puVar8 + 2);
    *(int *)(param_1 + 0xd) = iVar5 + 1;
    *(undefined8 *)(param_1[0xc] + (long)iVar5 * 8) = uVar7;
  }
  else {
    uVar6 = 0xffffffff;
  }
  if (*(long *)(lVar4 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar6);
  }
  return;
}

/* ===== FUN_00180630 @ 00180630 [libNexusScriptRuntime69252.so] ===== */

undefined1  [16]
FUN_00180630(long param_1,int *param_2,undefined8 param_3,undefined8 param_4,undefined8 *param_5)

{
  int *piVar1;
  undefined8 uVar2;
  int iVar3;
  long lVar4;
  int iVar5;
  undefined8 uVar6;
  ulong uVar7;
  ulong uVar8;
  int *piVar9;
  undefined1 auVar10 [16];
  undefined1 auVar11 [16];
  undefined1 auVar12 [16];
  int *local_b8;
  int *local_b0;
  int *local_a8 [8];
  long local_68;
  
  lVar4 = tpidr_el0;
  local_68 = *(long *)(lVar4 + 0x28);
  if (((int)param_3 == -1) && (*(short *)((long)param_2 + 6) == 0x12)) {
    piVar1 = param_2 + 0xc;
    if (piVar1 != (int *)0x0) {
      local_a8[0] = (int *)*param_5;
      uVar6 = param_5[2];
      uVar2 = param_5[3];
      uVar7 = param_5[1] & 0xffffffff;
      if (((uVar7 == 0xffffffff) && (*(short *)((long)local_a8[0] + 6) == 0x12)) &&
         (local_a8[0] + 0xc != (int *)0x0)) {
        if ((int)uVar2 == 3) {
          local_b8 = *(int **)(local_a8[0] + 0xc);
          *local_b8 = *local_b8 + 1;
          local_b0 = *(int **)(local_a8[0] + 0xe);
          *local_b0 = *local_b0 + 1;
LAB_00180834:
          piVar9 = *(int **)piVar1;
          iVar5 = *piVar9;
          iVar3 = iVar5 + -1;
          *piVar9 = iVar3;
          if (iVar3 == 0 || iVar5 < 1) {
            FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar9,0xfffffffffffffff9);
          }
          piVar9 = *(int **)(param_2 + 0xe);
          iVar5 = *piVar9;
          iVar3 = iVar5 + -1;
          *piVar9 = iVar3;
          if (iVar3 == 0 || iVar5 < 1) {
            FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar9,0xfffffffffffffff9);
          }
          *(int **)piVar1 = local_b8;
          *(int **)(param_2 + 0xe) = local_b0;
          iVar5 = FUN_0014681c(param_1,param_2,param_3,0x57,0,0,param_2,param_3,0x4000);
          if (-1 < iVar5) {
            *param_2 = *param_2 + 1;
            uVar7 = (ulong)param_2 & 0xffffffff00000000;
            uVar8 = (ulong)param_2 & 0xffffffff;
            local_a8[0] = param_2;
            goto LAB_001808e0;
          }
        }
        else {
          FUN_00143570(param_1,"flags must be undefined");
        }
      }
      else {
        local_b0 = (int *)((ulong)param_2 & 0xffffffff00000000);
        if (uVar7 == 3) {
          if (*(uint *)(*(long *)(param_1 + 0x18) + 0x50) < 0x30) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0xc94,"JSValue __JS_AtomToValue(JSContext *, JSAtom, BOOL)",
                      "atom < rt->atom_size");
          }
          piVar9 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + 0x178);
          auVar10._8_8_ = 0xfffffffffffffff9;
          auVar10._0_8_ = piVar9;
          *piVar9 = *piVar9 + 1;
        }
        else {
          auVar10 = FUN_0014c3ec(param_1,local_a8[0],param_5[1],0);
        }
        local_b8 = auVar10._0_8_;
        if (auVar10._8_4_ == 6) {
          auVar11._8_8_ = 3;
          auVar11._0_8_ = local_b0;
        }
        else {
          auVar11 = FUN_0015c430(param_1,local_b8,auVar10._8_8_,uVar6,uVar2);
          local_b0 = auVar11._0_8_;
          if (auVar11._8_4_ != 6) goto LAB_00180834;
        }
        local_b0 = auVar11._0_8_;
        if ((0xfffffff4 < auVar10._8_4_) &&
           (iVar5 = *local_b8, iVar3 = iVar5 + -1, *local_b8 = iVar3, iVar3 == 0 || iVar5 < 1)) {
          local_a8[0] = local_b8;
          FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_b8,auVar10._8_8_);
        }
        local_a8[0] = local_b0;
        if ((0xfffffff4 < auVar11._8_4_) &&
           (iVar5 = *local_b0, iVar3 = iVar5 + -1, *local_b0 = iVar3, iVar3 == 0 || iVar5 < 1)) {
          FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_b0,auVar11._8_8_);
        }
      }
    }
  }
  else {
    uVar6 = FUN_00142bd0(*(long *)(param_1 + 0x18),local_a8,
                         *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x18) + 0x70) + 0x2d4));
    FUN_00143570(param_1,"%s object expected",uVar6);
  }
  uVar8 = 0;
  uVar7 = 0;
  param_3 = 6;
LAB_001808e0:
  if (*(long *)(lVar4 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  auVar12._0_8_ = uVar7 | uVar8;
  auVar12._8_8_ = param_3;
  return auVar12;
}

/* ===== FUN_0018a250 @ 0018a250 [libNexusScriptRuntime69252.so] ===== */

/* WARNING: Restarted to delay deadcode elimination for space: stack */

undefined1  [16]
FUN_0018a250(long param_1,int *param_2,undefined8 param_3,int param_4,long *param_5,int param_6)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long *plVar4;
  int iVar5;
  ushort uVar6;
  int iVar7;
  long lVar8;
  char cVar9;
  int *piVar10;
  long lVar11;
  long lVar12;
  ulong uVar13;
  ulong uVar14;
  long *plVar15;
  long *plVar16;
  undefined8 uVar17;
  undefined1 auVar18 [16];
  long local_c0;
  int *local_a0;
  long lStack_98;
  int *local_90;
  long lStack_88;
  int *local_80;
  undefined8 uStack_78;
  long local_68;
  
  lVar8 = tpidr_el0;
  local_68 = *(long *)(lVar8 + 0x28);
  local_a0 = param_2;
  if ((((int)param_3 == -1) && (param_6 + 0x26U == (uint)*(ushort *)((long)param_2 + 6))) &&
     (lVar11 = *(long *)(param_2 + 0xc), lVar11 != 0)) {
    lVar1 = *param_5;
    lVar3 = param_5[1];
    if (param_4 < 2) {
      uVar14 = 0;
      uVar13 = 0;
      local_c0 = 3;
    }
    else {
      local_c0 = param_5[3];
      uVar13 = param_5[2] & 0xffffffff00000000;
      uVar14 = param_5[2] & 0xffffffff;
    }
    if ((int)lVar3 == -1) {
      uVar6 = *(ushort *)(lVar1 + 6);
      if (uVar6 == 0xd) {
        cVar9 = '\x01';
      }
      else if (uVar6 == 0x30) {
        cVar9 = *(char *)(*(long *)(lVar1 + 0x30) + 0x20);
      }
      else {
        cVar9 = *(long *)(*(long *)(*(long *)(param_1 + 0x18) + 0x70) + (ulong)uVar6 * 0x28 + 0x18)
                != 0;
      }
    }
    else {
      cVar9 = '\0';
    }
    if (cVar9 != '\0') {
      plVar15 = *(long **)(lVar11 + 0x10);
      if (plVar15 == (long *)(lVar11 + 8)) {
        uVar14 = 0;
        uVar13 = 0;
        uVar17 = 3;
      }
      else {
        do {
          if (*(int *)((long)plVar15 + -0x14) == 0) {
            local_90 = (int *)plVar15[4];
            lStack_88 = plVar15[5];
            *(int *)(plVar15 + -3) = (int)plVar15[-3] + 1;
            if (0xfffffff4 < (uint)lStack_88) {
              *local_90 = *local_90 + 1;
            }
            local_a0 = local_90;
            lStack_98 = lStack_88;
            if (param_6 == 0) {
              local_a0 = (int *)plVar15[6];
              lStack_98 = plVar15[7];
              if (0xfffffff4 < (uint)lStack_98) {
                *local_a0 = *local_a0 + 1;
              }
            }
            local_80 = param_2;
            uStack_78 = param_3;
            auVar18 = FUN_0014cb88(param_1,lVar1,lVar3,uVar13 | uVar14,local_c0,0,3,3,&local_a0,2);
            uVar17 = auVar18._8_8_;
            piVar10 = auVar18._0_8_;
            if ((0xfffffff4 < (uint)lStack_98) &&
               (iVar5 = *local_a0, iVar7 = iVar5 + -1, *local_a0 = iVar7, iVar7 == 0 || iVar5 < 1))
            {
              FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_a0);
            }
            if (((param_6 == 0) && (0xfffffff4 < (uint)lStack_88)) &&
               (iVar5 = *local_90, iVar7 = iVar5 + -1, *local_90 = iVar7, iVar7 == 0 || iVar5 < 1))
            {
              FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_90);
            }
            plVar16 = (long *)plVar15[1];
            iVar5 = (int)plVar15[-3] + -1;
            lVar12 = *(long *)(param_1 + 0x18);
            *(int *)(plVar15 + -3) = iVar5;
            if (iVar5 == 0) {
              if (*(int *)((long)plVar15 + -0x14) == 0) {
                    /* WARNING: Subroutine does not return */
                __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                          ,0xb976,"void map_decref_record(JSRuntime *, JSMapRecord *)","mr->empty");
              }
              lVar2 = *plVar15;
              plVar4 = (long *)plVar15[1];
              *(long **)(lVar2 + 8) = plVar4;
              *plVar4 = lVar2;
              *plVar15 = 0;
              plVar15[1] = 0;
              (**(code **)(lVar12 + 8))(lVar12 + 0x20,plVar15 + -3);
            }
            if (auVar18._8_4_ == 6) {
              uVar13 = (ulong)piVar10 & 0xffffffff00000000;
              uVar14 = (ulong)piVar10 & 0xffffffff;
              goto LAB_0018a574;
            }
            plVar15 = plVar16;
            if ((0xfffffff4 < auVar18._8_4_) &&
               (iVar5 = *piVar10, iVar7 = iVar5 + -1, *piVar10 = iVar7, iVar7 == 0 || iVar5 < 1)) {
              FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar10,uVar17);
            }
          }
          else {
            plVar15 = (long *)plVar15[1];
          }
        } while (plVar15 != (long *)(lVar11 + 8));
        uVar14 = 0;
        uVar13 = 0;
        uVar17 = 3;
      }
      goto LAB_0018a574;
    }
    FUN_00143570(param_1,"not a function");
  }
  else {
    FUN_00143944(param_1,*(undefined4 *)
                          (*(long *)(*(long *)(param_1 + 0x18) + 0x70) +
                           (ulong)(param_6 + 0x26U) * 0x28 + 4),"%s object expected");
  }
  uVar14 = 0;
  uVar13 = 0;
  uVar17 = 6;
LAB_0018a574:
  if (*(long *)(lVar8 + 0x28) == local_68) {
    auVar18._0_8_ = uVar13 | uVar14;
    auVar18._8_8_ = uVar17;
    return auVar18;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0018a838 @ 0018a838 [libNexusScriptRuntime69252.so] ===== */

void FUN_0018a838(long param_1,int *param_2,int *param_3)

{
  long lVar1;
  long *plVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int *piVar6;
  
  if (param_3[1] == 0) {
    lVar1 = *(long *)(param_3 + 10);
    plVar2 = *(long **)(param_3 + 0xc);
    *(long **)(lVar1 + 8) = plVar2;
    *plVar2 = lVar1;
    param_3[10] = 0;
    param_3[0xb] = 0;
    param_3[0xc] = 0;
    param_3[0xd] = 0;
    if (*param_2 == 0) {
      piVar6 = *(int **)(param_3 + 0xe);
      if (0xfffffff4 < (uint)*(undefined8 *)(param_3 + 0x10)) {
        iVar3 = *piVar6;
        iVar4 = iVar3 + -1;
        *piVar6 = iVar4;
        if (iVar4 == 0 || iVar3 < 1) {
          FUN_00141774(param_1,piVar6);
        }
      }
    }
    else {
      piVar6 = (int *)(*(long *)(param_3 + 0xe) + 0x28);
      piVar5 = *(int **)piVar6;
      if (piVar5 == (int *)0x0) {
code_r0x0018a984:
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0xb953,"void delete_weak_ref(JSRuntime *, JSMapRecord *)","mr1 != NULL");
      }
      if (piVar5 != param_3) {
        do {
          piVar6 = piVar5;
          piVar5 = *(int **)(piVar6 + 4);
          if (piVar5 == (int *)0x0) goto code_r0x0018a984;
        } while (piVar5 != param_3);
        piVar6 = piVar6 + 4;
      }
      *(undefined8 *)piVar6 = *(undefined8 *)(piVar5 + 4);
    }
    piVar6 = *(int **)(param_3 + 0x12);
    if (0xfffffff4 < (uint)*(undefined8 *)(param_3 + 0x14)) {
      iVar3 = *piVar6;
      iVar4 = iVar3 + -1;
      *piVar6 = iVar4;
      if (iVar4 == 0 || iVar3 < 1) {
        FUN_00141774(param_1,piVar6);
      }
    }
    iVar3 = *param_3;
    *param_3 = iVar3 + -1;
    if (iVar3 + -1 == 0) {
      lVar1 = *(long *)(param_3 + 6);
      plVar2 = *(long **)(param_3 + 8);
      *(long **)(lVar1 + 8) = plVar2;
      *plVar2 = lVar1;
      param_3[6] = 0;
      param_3[7] = 0;
      param_3[8] = 0;
      param_3[9] = 0;
      (**(code **)(param_1 + 8))(param_1 + 0x20,param_3);
    }
    else {
      param_3[0xe] = 0;
      param_3[0x12] = 0;
      param_3[1] = 1;
      param_3[0x10] = 3;
      param_3[0x11] = 0;
      param_3[0x14] = 3;
      param_3[0x15] = 0;
    }
    param_2[6] = param_2[6] + -1;
  }
  return;
}

/* ===== FUN_0018a9a0 @ 0018a9a0 [libNexusScriptRuntime69252.so] ===== */

ulong FUN_0018a9a0(long param_1,int *param_2,int param_3,undefined8 param_4,undefined8 param_5,
                  undefined4 *param_6,int param_7)

{
  undefined1 (*pauVar1) [16];
  int iVar2;
  int iVar3;
  long lVar4;
  uint uVar5;
  int *piVar6;
  ulong uVar7;
  long lVar8;
  long *plVar9;
  long lVar10;
  long *plVar11;
  long *plVar12;
  long lVar13;
  undefined1 auVar14 [16];
  int *local_80;
  long lStack_78;
  int *local_70;
  long lStack_68;
  long local_58;
  
  lVar4 = tpidr_el0;
  local_58 = *(long *)(lVar4 + 0x28);
  local_80 = param_2;
  if (((param_3 == -1) && (param_7 + 0x2aU == (uint)*(ushort *)((long)param_2 + 6))) &&
     (plVar11 = *(long **)(param_2 + 0xc), plVar11 != (long *)0x0)) {
    if ((int)plVar11[1] != 3) {
      local_80 = (int *)*plVar11;
      if ((((int)plVar11[1] != -1) || (param_7 + 0x26U != (uint)*(ushort *)((long)local_80 + 6))) ||
         (lVar13 = *(long *)(local_80 + 0xc), lVar13 == 0)) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0xbb26,
                  "JSValue js_map_iterator_next(JSContext *, JSValue, int, JSValue *, BOOL *, int)",
                  "s != NULL");
      }
      piVar6 = (int *)plVar11[3];
      if (piVar6 == (int *)0x0) {
        plVar12 = *(long **)(lVar13 + 0x10);
      }
      else {
        iVar2 = *piVar6;
        plVar12 = *(long **)(piVar6 + 8);
        lVar8 = *(long *)(param_1 + 0x18);
        *piVar6 = iVar2 + -1;
        if (iVar2 + -1 == 0) {
          if (piVar6[1] == 0) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0xb976,"void map_decref_record(JSRuntime *, JSMapRecord *)","mr->empty");
          }
          lVar10 = *(long *)(piVar6 + 6);
          *(long **)(lVar10 + 8) = plVar12;
          *plVar12 = lVar10;
          piVar6[6] = 0;
          piVar6[7] = 0;
          piVar6[8] = 0;
          piVar6[9] = 0;
          (**(code **)(lVar8 + 8))(lVar8 + 0x20);
        }
      }
      for (; plVar12 != (long *)(lVar13 + 8); plVar12 = (long *)plVar12[1]) {
        if (*(int *)((long)plVar12 + -0x14) == 0) {
          plVar9 = plVar12 + -3;
          *(int *)plVar9 = (int)*plVar9 + 1;
          plVar11[3] = (long)plVar9;
          *param_6 = 0;
          if ((int)plVar11[2] == 0) {
            local_80 = *(int **)*(undefined1 (*) [16])(plVar12 + 4);
            auVar14 = *(undefined1 (*) [16])(plVar12 + 4);
            uVar5 = (uint)plVar12[5];
            piVar6 = local_80;
            lVar13 = plVar12[5];
joined_r0x0018ac08:
            if (0xfffffff4 < uVar5) {
              auVar14._8_8_ = lVar13;
              auVar14._0_8_ = piVar6;
              *piVar6 = *piVar6 + 1;
            }
          }
          else {
            lStack_78 = plVar12[5];
            local_80 = (int *)plVar12[4];
            pauVar1 = (undefined1 (*) [16])(plVar12 + 6);
            if (param_7 != 0) {
              pauVar1 = (undefined1 (*) [16])(plVar12 + 4);
            }
            lStack_68 = *(long *)(*pauVar1 + 8);
            local_70 = *(int **)*pauVar1;
            auVar14 = *pauVar1;
            if ((int)plVar11[2] == 1) {
              uVar5 = (uint)lStack_68;
              piVar6 = local_70;
              lVar13 = lStack_68;
              goto joined_r0x0018ac08;
            }
            auVar14 = FUN_001765c0(param_1,2,&local_80);
          }
          uVar7 = auVar14._0_8_ & 0xffffffff00000000;
          goto LAB_0018ab68;
        }
      }
      plVar11[3] = 0;
      local_80 = (int *)*plVar11;
      if ((0xfffffff4 < (uint)plVar11[1]) &&
         (iVar2 = *local_80, iVar3 = iVar2 + -1, *local_80 = iVar3, iVar3 == 0 || iVar2 < 1)) {
        FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_80);
      }
      *(undefined4 *)plVar11 = 0;
      plVar11[1] = 3;
    }
    uVar7 = 0;
    auVar14 = ZEXT816(3) << 0x40;
    *param_6 = 1;
  }
  else {
    FUN_00143944(param_1,*(undefined4 *)
                          (*(long *)(*(long *)(param_1 + 0x18) + 0x70) +
                           (ulong)(param_7 + 0x2aU) * 0x28 + 4),"%s object expected");
    uVar7 = 0;
    auVar14 = ZEXT816(6) << 0x40;
    *param_6 = 0;
  }
LAB_0018ab68:
  if (*(long *)(lVar4 + 0x28) == local_58) {
    return uVar7 | auVar14._0_8_ & 0xffffffff;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(auVar14._0_8_,auVar14._8_8_);
}

/* ===== FUN_0018b740 @ 0018b740 [libNexusScriptRuntime69252.so] ===== */

void FUN_0018b740(long param_1,long param_2)

{
  long *plVar1;
  int iVar2;
  long lVar3;
  int *piVar4;
  long *plVar5;
  undefined8 *puVar6;
  
  piVar4 = *(int **)(param_2 + 0x30);
  if (((piVar4 != (int *)0x0) && (iVar2 = *piVar4, *piVar4 = iVar2 + -1, iVar2 + -1 == 0)) &&
     (*(char *)(param_1 + 0xb8) != '\x02')) {
    plVar5 = (long *)(piVar4 + 2);
    lVar3 = *plVar5;
    plVar1 = *(long **)(piVar4 + 4);
    *(long **)(lVar3 + 8) = plVar1;
    *plVar1 = lVar3;
    *plVar5 = 0;
    piVar4[4] = 0;
    piVar4[5] = 0;
    puVar6 = *(undefined8 **)(param_1 + 0xa0);
    *(long **)(param_1 + 0xa0) = plVar5;
    *plVar5 = param_1 + 0x98;
    *(undefined8 **)(piVar4 + 4) = puVar6;
    *puVar6 = plVar5;
    if (*(char *)(param_1 + 0xb8) == '\0') {
      lVar3 = *(long *)(param_1 + 0xa0);
      *(undefined1 *)(param_1 + 0xb8) = 1;
      while (lVar3 != param_1 + 0x98) {
        if (*(int *)(lVar3 + -8) != 0) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x15e6,"void free_zero_refcount(JSRuntime *)","p->ref_count == 0");
        }
        FUN_0016b224(param_1);
        lVar3 = *(long *)(param_1 + 0xa0);
      }
      *(undefined1 *)(param_1 + 0xb8) = 0;
    }
  }
  return;
}

/* ===== FUN_0018baf0 @ 0018baf0 [libNexusScriptRuntime69252.so] ===== */

void FUN_0018baf0(long param_1,long param_2)

{
  long *plVar1;
  int iVar2;
  int iVar3;
  long lVar4;
  long *plVar5;
  undefined8 *puVar6;
  long lVar7;
  int *piVar8;
  
  if (*(long *)(param_2 + 0x20) != param_2 + 0x18) {
    lVar4 = *(long *)(param_2 + 0x20);
    do {
      piVar8 = *(int **)(lVar4 + 0x18);
      lVar7 = *(long *)(lVar4 + 8);
      if ((0xfffffff4 < (uint)*(undefined8 *)(lVar4 + 0x20)) &&
         (iVar2 = *piVar8, iVar3 = iVar2 + -1, *piVar8 = iVar3, iVar3 == 0 || iVar2 < 1)) {
        FUN_00141774(param_1,piVar8);
      }
      piVar8 = *(int **)(lVar4 + 0x28);
      if ((0xfffffff4 < (uint)*(undefined8 *)(lVar4 + 0x30)) &&
         (iVar2 = *piVar8, iVar3 = iVar2 + -1, *piVar8 = iVar3, iVar3 == 0 || iVar2 < 1)) {
        FUN_00141774(param_1,piVar8);
      }
      piVar8 = *(int **)(lVar4 + 0x38);
      if ((0xfffffff4 < (uint)*(undefined8 *)(lVar4 + 0x40)) &&
         (iVar2 = *piVar8, iVar3 = iVar2 + -1, *piVar8 = iVar3, iVar3 == 0 || iVar2 < 1)) {
        FUN_00141774(param_1,piVar8);
      }
      piVar8 = *(int **)(lVar4 + 0x48);
      if ((0xfffffff4 < (uint)*(undefined8 *)(lVar4 + 0x50)) &&
         (iVar2 = *piVar8, iVar3 = iVar2 + -1, *piVar8 = iVar3, iVar3 == 0 || iVar2 < 1)) {
        FUN_00141774(param_1,piVar8);
      }
      (**(code **)(param_1 + 8))(param_1 + 0x20,lVar4);
      lVar4 = lVar7;
    } while (lVar7 != param_2 + 0x18);
  }
  piVar8 = *(int **)(param_2 + 0x10);
  if (((piVar8 != (int *)0x0) && (iVar2 = *piVar8, *piVar8 = iVar2 + -1, iVar2 + -1 == 0)) &&
     (*(char *)(param_1 + 0xb8) != '\x02')) {
    plVar5 = (long *)(piVar8 + 2);
    lVar4 = *plVar5;
    plVar1 = *(long **)(piVar8 + 4);
    *(long **)(lVar4 + 8) = plVar1;
    *plVar1 = lVar4;
    *plVar5 = 0;
    piVar8[4] = 0;
    piVar8[5] = 0;
    puVar6 = *(undefined8 **)(param_1 + 0xa0);
    *(long **)(param_1 + 0xa0) = plVar5;
    *plVar5 = param_1 + 0x98;
    *(undefined8 **)(piVar8 + 4) = puVar6;
    *puVar6 = plVar5;
    if (*(char *)(param_1 + 0xb8) == '\0') {
      lVar4 = *(long *)(param_1 + 0xa0);
      *(undefined1 *)(param_1 + 0xb8) = 1;
      while (lVar4 != param_1 + 0x98) {
        if (*(int *)(lVar4 + -8) != 0) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x15e6,"void free_zero_refcount(JSRuntime *)","p->ref_count == 0");
        }
        FUN_0016b224(param_1);
        lVar4 = *(long *)(param_1 + 0xa0);
      }
      *(undefined1 *)(param_1 + 0xb8) = 0;
    }
  }
                    /* WARNING: Could not recover jumptable at 0x0018bcc0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(param_1 + 8))(param_1 + 0x20,param_2);
  return;
}

/* ===== FUN_0018bfa0 @ 0018bfa0 [libNexusScriptRuntime69252.so] ===== */

undefined1  [16] FUN_0018bfa0(long param_1,int param_2,undefined8 *param_3)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  int iVar5;
  long lVar6;
  int iVar7;
  long lVar8;
  undefined1 auVar9 [16];
  int *local_88;
  undefined8 local_80;
  int *local_78;
  uint local_70;
  int *local_68;
  undefined8 local_60;
  long local_58;
  
  lVar6 = tpidr_el0;
  local_58 = *(long *)(lVar6 + 0x28);
  if (param_2 != 3) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0xbc6f,"JSValue js_promise_resolve_thenable_job(JSContext *, int, JSValue *)",
              "argc == 3");
  }
  uVar1 = param_3[2];
  uVar3 = param_3[3];
  uVar2 = param_3[4];
  uVar4 = param_3[5];
  iVar7 = FUN_0018c180(param_1,&local_78,*param_3,param_3[1]);
  if (iVar7 < 0) {
    auVar9 = ZEXT816(6) << 0x40;
  }
  else {
    auVar9 = FUN_0014cb88(param_1,uVar2,uVar4,uVar1,uVar3,0,3,2,&local_78,2);
    if (auVar9._8_4_ == 6) {
      lVar8 = *(long *)(param_1 + 0x18);
      local_88 = *(int **)(lVar8 + 0xe0);
      local_80 = *(undefined8 *)(lVar8 + 0xe8);
      *(undefined4 *)(lVar8 + 0xe0) = 0;
      *(undefined8 *)(lVar8 + 0xe8) = 2;
      auVar9 = FUN_0014cb88(param_1,local_68,local_60,0,3,0,3,1,&local_88,2);
      if ((0xfffffff4 < (uint)local_80) &&
         (iVar7 = *local_88, iVar5 = iVar7 + -1, *local_88 = iVar5, iVar5 == 0 || iVar7 < 1)) {
        FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_88);
      }
    }
    local_88 = local_78;
    if ((0xfffffff4 < local_70) &&
       (iVar7 = *local_78, iVar5 = iVar7 + -1, *local_78 = iVar5, iVar5 == 0 || iVar7 < 1)) {
      FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_78);
    }
    local_88 = local_68;
    if ((0xfffffff4 < (uint)local_60) &&
       (iVar7 = *local_68, iVar5 = iVar7 + -1, *local_68 = iVar5, iVar5 == 0 || iVar7 < 1)) {
      FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_68);
    }
  }
  if (*(long *)(lVar6 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return auVar9;
}

/* ===== FUN_0018c180 @ 0018c180 [libNexusScriptRuntime69252.so] ===== */

undefined4 FUN_0018c180(long param_1,undefined8 *param_2,int *param_3,undefined8 param_4)

{
  bool bVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  undefined8 *puVar6;
  int *piVar7;
  long lVar8;
  long lVar9;
  undefined4 uVar10;
  ulong uVar11;
  int *piVar12;
  undefined1 auVar13 [16];
  
  piVar5 = (int *)(*(code *)**(undefined8 **)(param_1 + 0x18))
                            (*(undefined8 **)(param_1 + 0x18) + 4,8);
  if (piVar5 == (int *)0x0) {
    FUN_00138d58(param_1);
  }
  else if (piVar5 != (int *)0x0) {
    piVar5[0] = 1;
    piVar5[1] = 0;
    lVar8 = 0;
    do {
      lVar9 = *(long *)(param_1 + 0x48);
      if (*(int *)(param_1 + 0x50) != -1) {
        lVar9 = 0;
      }
      uVar3 = ((int)lVar9 * -0x61c8ffff + -0x61c8ffff + (int)((ulong)lVar9 >> 0x20)) * -0x61c8ffff;
      for (piVar7 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x188) +
                             (ulong)(uVar3 >> (ulong)(-*(int *)(*(long *)(param_1 + 0x18) + 0x178) &
                                                     0x1f)) * 8);
          (piVar7 != (int *)0x0 &&
          (((piVar7[7] != uVar3 || (*(long *)(piVar7 + 0xe) != lVar9)) || (piVar7[10] != 0))));
          piVar7 = *(int **)(piVar7 + 0xc)) {
      }
      if (piVar7 == (int *)0x0) {
        piVar7 = (int *)FUN_0016b030(param_1,lVar9,2);
        if (piVar7 != (int *)0x0) goto LAB_0018c278;
        auVar13 = ZEXT816(6) << 0x40;
      }
      else {
        *piVar7 = *piVar7 + 1;
LAB_0018c278:
        auVar13 = FUN_00140ed0(param_1,piVar7,(int)lVar8 + 0x32);
      }
      uVar11 = auVar13._8_8_;
      piVar7 = auVar13._0_8_;
      if ((uVar11 & 0xffffffff) == 6) {
LAB_0018c40c:
        if ((((int)lVar8 != 0) && (piVar7 = (int *)*param_2, 0xfffffff4 < (uint)param_2[1])) &&
           (iVar2 = *piVar7, iVar4 = iVar2 + -1, *piVar7 = iVar4, iVar4 == 0 || iVar2 < 1)) {
          FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar7);
        }
        uVar10 = 0xffffffff;
        goto LAB_0018c454;
      }
      puVar6 = (undefined8 *)
               (*(code *)**(undefined8 **)(param_1 + 0x18))
                         (*(undefined8 **)(param_1 + 0x18) + 4,0x18);
      if (puVar6 == (undefined8 *)0x0) {
        FUN_00138d58(param_1);
LAB_0018c3dc:
        if ((0xfffffff4 < auVar13._8_4_) &&
           (iVar2 = *piVar7, iVar4 = iVar2 + -1, *piVar7 = iVar4, iVar4 == 0 || iVar2 < 1)) {
          FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar7,uVar11);
        }
        goto LAB_0018c40c;
      }
      if (puVar6 == (undefined8 *)0x0) goto LAB_0018c3dc;
      *piVar5 = *piVar5 + 1;
      puVar6[2] = piVar5;
      if (0xfffffff4 < (uint)param_4) {
        *param_3 = *param_3 + 1;
      }
      *puVar6 = param_3;
      puVar6[1] = param_4;
      if ((uVar11 & 0xffffffff) == 0xffffffff) {
        *(undefined8 **)(piVar7 + 0xc) = puVar6;
      }
      FUN_00148018(param_1,piVar7,uVar11,0x30,1,0,0,3,0,3,0x2701);
      if (*(uint *)(*(long *)(param_1 + 0x18) + 0x50) < 0x30) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0xc94,"JSValue __JS_AtomToValue(JSContext *, JSAtom, BOOL)",
                  "atom < rt->atom_size");
      }
      piVar12 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + 0x178);
      *piVar12 = *piVar12 + 1;
      FUN_00148018(param_1,piVar7,uVar11,0x38,piVar12,0xfffffffffffffff9,0,3,0,3,0x2701);
      iVar2 = *piVar12;
      iVar4 = iVar2 + -1;
      *piVar12 = iVar4;
      if (iVar4 == 0 || iVar2 < 1) {
        FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar12,0xfffffffffffffff9);
      }
      *(undefined1 (*) [16])(param_2 + lVar8 * 2) = auVar13;
      bVar1 = lVar8 == 0;
      lVar8 = lVar8 + 1;
    } while (bVar1);
    uVar10 = 0;
LAB_0018c454:
    iVar2 = *piVar5;
    lVar8 = *(long *)(param_1 + 0x18);
    *piVar5 = iVar2 + -1;
    if (iVar2 + -1 != 0) {
      return uVar10;
    }
    (**(code **)(lVar8 + 8))(lVar8 + 0x20,piVar5);
    return uVar10;
  }
  return 0xffffffff;
}

/* ===== FUN_0018f2e0 @ 0018f2e0 [libNexusScriptRuntime69252.so] ===== */

void FUN_0018f2e0(long param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  int iVar2;
  int iVar3;
  long lVar4;
  bool bVar5;
  int iVar6;
  int *piVar7;
  undefined8 uVar8;
  long lVar9;
  long lVar10;
  ulong *puVar11;
  uint uVar12;
  uint uVar13;
  ulong uVar14;
  ulong uVar15;
  int *piVar16;
  undefined1 auVar17 [16];
  undefined1 auVar18 [16];
  int *local_a8;
  int *local_a0 [4];
  ulong local_80 [4];
  
  lVar4 = tpidr_el0;
  local_80[3] = *(long *)(lVar4 + 0x28);
  if ((undefined8 *)param_2[4] != param_2 + 3) {
    do {
      uVar12 = *(uint *)(param_2 + 1);
      if (5 < uVar12) {
                    /* WARNING: Subroutine does not return */
        abort();
      }
      lVar9 = param_2[4];
      switch(uVar12) {
      case 0:
        iVar6 = *(int *)(lVar9 + 0x10);
        if (iVar6 == 0) goto LAB_0018f434;
        FUN_0018f938(param_1,param_2);
        goto LAB_0018f354;
      case 3:
        goto switchD_0018f388_caseD_3;
      case 4:
        goto switchD_0018f388_caseD_4;
      case 5:
        if (*(int *)(lVar9 + 0x10) == 1) {
          *(undefined4 *)(param_2 + 1) = 4;
          FUN_0018fad8(param_1,param_2,*(undefined8 *)(lVar9 + 0x18),*(undefined8 *)(lVar9 + 0x20));
        }
        else if (*(int *)(lVar9 + 0x10) == 0) {
          FUN_0018fa18(param_1,param_2,0,3,1);
        }
        else {
          FUN_0018fd58(param_1,param_2,*(undefined8 *)(lVar9 + 0x18),*(undefined8 *)(lVar9 + 0x20),1
                      );
        }
        goto switchD_0018f388_caseD_4;
      }
      piVar7 = *(int **)(lVar9 + 0x18);
      uVar1 = *(undefined8 *)(lVar9 + 0x20);
      if (0xfffffff4 < (uint)uVar1) {
        *piVar7 = *piVar7 + 1;
      }
      if ((uVar12 == 1) && (*(int *)(lVar9 + 0x10) == 2)) {
        lVar9 = *(long *)(param_1 + 0x18);
        local_a0[3] = *(int **)(lVar9 + 0xe0);
        if ((0xfffffff4 < (uint)*(undefined8 *)(lVar9 + 0xe8)) &&
           (iVar6 = *local_a0[3], iVar2 = iVar6 + -1, *local_a0[3] = iVar2, iVar2 == 0 || iVar6 < 1)
           ) {
          FUN_00141774(lVar9,local_a0[3]);
        }
        iVar6 = 1;
        *(int **)(lVar9 + 0xe0) = piVar7;
        *(undefined8 *)(lVar9 + 0xe8) = uVar1;
      }
      else {
        lVar10 = *(long *)(param_2[2] + 0xa0);
        *(int **)(lVar10 + -0x10) = piVar7;
        *(undefined8 *)(lVar10 + -8) = uVar1;
        puVar11 = *(ulong **)(param_2[2] + 0xa0);
        *puVar11 = (ulong)*(uint *)(lVar9 + 0x10);
        puVar11[1] = 0;
        iVar6 = 0;
        *(long *)(param_2[2] + 0xa0) = *(long *)(param_2[2] + 0xa0) + 0x10;
        local_a0[3] = piVar7;
      }
LAB_0018f434:
      *(int *)(param_2[2] + 0x2c) = iVar6;
      *(undefined4 *)(param_2 + 1) = 3;
switchD_0018f388_caseD_3:
      do {
        auVar17 = FUN_001774b4(param_1,param_2[2]);
        piVar7 = auVar17._0_8_;
        uVar12 = auVar17._8_4_;
        if (*(int *)(param_2[2] + 0x30) != 0) {
          if (uVar12 == 6) {
            lVar9 = *(long *)(param_1 + 0x18);
            piVar7 = *(int **)*(undefined1 (*) [16])(lVar9 + 0xe0);
            uVar1 = *(undefined8 *)(lVar9 + 0xe8);
            auVar17 = *(undefined1 (*) [16])(lVar9 + 0xe0);
            *(undefined4 *)(lVar9 + 0xe0) = 0;
            *(undefined8 *)(lVar9 + 0xe8) = 2;
            FUN_0018f938(param_1,param_2);
            FUN_0018fd58(param_1,param_2,piVar7,uVar1,1);
            uVar12 = (uint)uVar1;
            local_a0[3] = piVar7;
          }
          else {
            FUN_0018f938(param_1,param_2);
            if (0xfffffff4 < uVar12) {
              *piVar7 = *piVar7 + 1;
            }
            local_a0[3] = piVar7;
            auVar18 = FUN_001701b4(param_1,piVar7,auVar17._8_8_,1);
            piVar16 = auVar18._0_8_;
            FUN_0018fd58(param_1,param_2,piVar16,auVar18._8_8_,0);
            local_a0[3] = piVar7;
            if ((0xfffffff4 < auVar18._8_4_) &&
               (iVar6 = *piVar16, iVar2 = iVar6 + -1, *piVar16 = iVar2, iVar2 == 0 || iVar6 < 1)) {
              local_a0[3] = piVar16;
              FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar16,auVar18._8_8_);
              local_a0[3] = piVar7;
            }
          }
          if (0xfffffff4 < uVar12) {
            local_a0[3] = auVar17._0_8_;
            iVar6 = *local_a0[3];
            iVar2 = iVar6 + -1;
            *local_a0[3] = iVar2;
            if (iVar2 == 0 || iVar6 < 1) {
              FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_a0[3],auVar17._8_8_);
            }
          }
          goto LAB_0018f354;
        }
        if (uVar12 != 0) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x4d63,
                    "void js_async_generator_resume_next(JSContext *, JSAsyncGeneratorData *)",
                    "JS_VALUE_GET_TAG(func_ret) == JS_TAG_INT");
        }
        lVar9 = *(long *)(param_2[2] + 0xa0);
        iVar6 = auVar17._0_4_;
        piVar7 = *(int **)(lVar9 + -0x10);
        uVar1 = *(undefined8 *)(lVar9 + -8);
        *(undefined4 *)(lVar9 + -0x10) = 0;
        *(undefined8 *)(lVar9 + -8) = 3;
        uVar12 = (uint)uVar1;
        if (iVar6 - 1U < 2) {
          *(int *)(param_2 + 1) = iVar6;
          if (0xfffffff4 < uVar12) {
            *piVar7 = *piVar7 + 1;
          }
          local_a0[3] = piVar7;
          auVar17 = FUN_001701b4(param_1,piVar7,uVar1,0);
          piVar16 = auVar17._0_8_;
          FUN_0018fd58(param_1,param_2,piVar16,auVar17._8_8_,0);
          if ((0xfffffff4 < auVar17._8_4_) &&
             (iVar6 = *piVar16, iVar2 = iVar6 + -1, *piVar16 = iVar2, iVar2 == 0 || iVar6 < 1)) {
            local_a0[3] = piVar16;
            FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar16,auVar17._8_8_);
          }
          local_a0[3] = piVar7;
          if ((0xfffffff4 < uVar12) &&
             (iVar6 = *piVar7, iVar2 = iVar6 + -1, *piVar7 = iVar2, iVar2 == 0 || iVar6 < 1)) {
            FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar7,uVar1);
          }
          iVar6 = 0;
        }
        else {
          if (iVar6 != 0) {
                    /* WARNING: Subroutine does not return */
            abort();
          }
          auVar17 = FUN_0018c86c(param_1,*(undefined8 *)(param_1 + 0x88),
                                 *(undefined8 *)(param_1 + 0x90));
          uVar8 = auVar17._8_8_;
          piVar16 = auVar17._0_8_;
          uVar13 = auVar17._8_4_;
          if (uVar13 == 6) {
LAB_0018f708:
            bVar5 = true;
          }
          else {
            uVar14 = 0;
            local_a8 = (int *)*param_2;
            local_a0[0] = (int *)0xffffffffffffffff;
            puVar11 = local_80;
            do {
              auVar17 = FUN_00141478(param_1,FUN_0018ff00,1,uVar14 & 0xffffffff,1,&local_a8);
              uVar15 = auVar17._8_8_ & 0xffffffff;
              if (uVar15 == 6) {
                if (((uVar14 == 1) && (0xfffffff4 < (uint)local_80[0])) &&
                   (iVar6 = *local_a0[3], iVar2 = iVar6 + -1, *local_a0[3] = iVar2,
                   iVar2 == 0 || iVar6 < 1)) {
                  FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_a0[3]);
                }
                break;
              }
              puVar11[-1] = auVar17._0_8_;
              *puVar11 = auVar17._8_8_;
              uVar14 = uVar14 + 1;
              puVar11 = puVar11 + 2;
            } while (uVar14 == 1);
            if (uVar15 == 6) {
              local_a8 = piVar16;
              if ((0xfffffff4 < uVar13) &&
                 (iVar6 = *piVar16, iVar2 = iVar6 + -1, *piVar16 = iVar2, iVar2 == 0 || iVar6 < 1))
              {
                FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar16,uVar8);
              }
              goto LAB_0018f708;
            }
            lVar9 = 0;
            do {
              lVar10 = lVar9 + 0x10;
              *(undefined4 *)((long)local_a0 + lVar9 + -8) = 0;
              *(undefined8 *)((long)local_a0 + lVar9) = 3;
              lVar9 = lVar10;
            } while (lVar10 == 0x10);
            iVar6 = FUN_00177ff4(param_1,piVar16,uVar8,local_a0 + 3,&local_a8);
            if ((0xfffffff4 < uVar13) &&
               (iVar2 = *piVar16, iVar3 = iVar2 + -1, *piVar16 = iVar3, iVar3 == 0 || iVar2 < 1)) {
              FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar16,uVar8);
            }
            lVar9 = 0;
            do {
              piVar16 = *(int **)((long)local_80 + lVar9 + -8);
              if ((0xfffffff4 < (uint)*(undefined8 *)((long)local_80 + lVar9)) &&
                 (iVar2 = *piVar16, iVar3 = iVar2 + -1, *piVar16 = iVar3, iVar3 == 0 || iVar2 < 1))
              {
                FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar16);
              }
              lVar9 = lVar9 + 0x10;
            } while (lVar9 == 0x10);
            if (iVar6 != 0) goto LAB_0018f708;
            bVar5 = false;
          }
          local_a0[3] = piVar7;
          if ((0xfffffff4 < uVar12) &&
             (iVar6 = *piVar7, iVar2 = iVar6 + -1, *piVar7 = iVar2, iVar2 == 0 || iVar6 < 1)) {
            FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar7,uVar1);
          }
          if (bVar5) {
            iVar6 = 5;
            *(undefined4 *)(param_2[2] + 0x2c) = 1;
          }
          else {
            iVar6 = 6;
          }
        }
      } while (iVar6 == 5);
      if (iVar6 != 0) break;
LAB_0018f354:
    } while ((undefined8 *)param_2[4] != param_2 + 3);
  }
switchD_0018f388_caseD_4:
  if (*(long *)(lVar4 + 0x28) != local_80[3]) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_0018f938 @ 0018f938 [libNexusScriptRuntime69252.so] ===== */

void FUN_0018f938(long param_1,long param_2)

{
  long *plVar1;
  int iVar2;
  long lVar3;
  int *piVar4;
  long *plVar5;
  undefined8 *puVar6;
  long lVar7;
  
  if (*(int *)(param_2 + 8) != 5) {
    piVar4 = *(int **)(param_2 + 0x10);
    lVar7 = *(long *)(param_1 + 0x18);
    iVar2 = *piVar4;
    *(undefined4 *)(param_2 + 8) = 5;
    *piVar4 = iVar2 + -1;
    if ((iVar2 + -1 == 0) && (*(char *)(lVar7 + 0xb8) != '\x02')) {
      plVar5 = (long *)(piVar4 + 2);
      lVar3 = *plVar5;
      plVar1 = *(long **)(piVar4 + 4);
      *(long **)(lVar3 + 8) = plVar1;
      *plVar1 = lVar3;
      *plVar5 = 0;
      piVar4[4] = 0;
      piVar4[5] = 0;
      puVar6 = *(undefined8 **)(lVar7 + 0xa0);
      *(long **)(lVar7 + 0xa0) = plVar5;
      *plVar5 = lVar7 + 0x98;
      *(undefined8 **)(piVar4 + 4) = puVar6;
      *puVar6 = plVar5;
      if (*(char *)(lVar7 + 0xb8) == '\0') {
        lVar3 = *(long *)(lVar7 + 0xa0);
        *(undefined1 *)(lVar7 + 0xb8) = 1;
        while (lVar3 != lVar7 + 0x98) {
          if (*(int *)(lVar3 + -8) != 0) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0x15e6,"void free_zero_refcount(JSRuntime *)","p->ref_count == 0");
          }
          FUN_0016b224(lVar7);
          lVar3 = *(long *)(lVar7 + 0xa0);
        }
        *(undefined1 *)(lVar7 + 0xb8) = 0;
      }
    }
    *(undefined8 *)(param_2 + 0x10) = 0;
  }
  return;
}

/* ===== FUN_0018ff00 @ 0018ff00 [libNexusScriptRuntime69252.so] ===== */

undefined1  [16] FUN_0018ff00(long param_1)

{
  uint uVar1;
  undefined8 uVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  undefined8 *in_x4;
  uint in_w5;
  long *in_x6;
  long lVar6;
  long lVar7;
  uint uVar8;
  undefined8 *puVar9;
  int *piVar10;
  undefined1 auVar11 [16];
  
  uVar1 = in_w5 & 1;
  if (((int)in_x6[1] == -1) && (*(short *)(*in_x6 + 6) == 0x39)) {
    lVar7 = *(long *)(*in_x6 + 0x30);
  }
  else {
    lVar7 = 0;
  }
  piVar5 = (int *)*in_x4;
  uVar2 = in_x4[1];
  uVar8 = (uint)uVar2;
  if ((int)in_w5 < 2) {
    if (*(uint *)(lVar7 + 8) != 3) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x4d9d,
                "JSValue js_async_generator_resolve_function(JSContext *, JSValue, int, JSValue *, int, JSValue *)"
                ,"s->state == JS_ASYNC_GENERATOR_STATE_EXECUTING");
    }
    lVar6 = *(long *)(lVar7 + 0x10);
    *(uint *)(lVar6 + 0x2c) = uVar1;
    if (uVar1 == 0) {
      lVar6 = *(long *)(lVar6 + 0xa0);
      if (0xfffffff4 < uVar8) {
        *piVar5 = *piVar5 + 1;
      }
      puVar9 = (undefined8 *)(lVar6 + -8);
      *(int **)(lVar6 + -0x10) = piVar5;
    }
    else {
      if (0xfffffff4 < uVar8) {
        *piVar5 = *piVar5 + 1;
      }
      lVar6 = *(long *)(param_1 + 0x18);
      piVar10 = *(int **)(lVar6 + 0xe0);
      puVar9 = (undefined8 *)(lVar6 + 0xe8);
      if (0xfffffff4 < (uint)*puVar9) {
        iVar3 = *piVar10;
        iVar4 = iVar3 + -1;
        *piVar10 = iVar4;
        if (iVar4 == 0 || iVar3 < 1) {
          FUN_00141774(lVar6,piVar10);
        }
      }
      *(int **)(lVar6 + 0xe0) = piVar5;
    }
    *puVar9 = uVar2;
    FUN_0018f2e0(param_1,lVar7);
  }
  else {
    if ((*(uint *)(lVar7 + 8) & 0xfffffffe) != 4) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x4d94,
                "JSValue js_async_generator_resolve_function(JSContext *, JSValue, int, JSValue *, int, JSValue *)"
                ,
                "s->state == JS_ASYNC_GENERATOR_STATE_AWAITING_RETURN || s->state == JS_ASYNC_GENERATOR_STATE_COMPLETED"
               );
    }
    *(undefined4 *)(lVar7 + 8) = 5;
    if (uVar1 == 0) {
      if (0xfffffff4 < uVar8) {
        *piVar5 = *piVar5 + 1;
      }
      auVar11 = FUN_001701b4(param_1,piVar5,uVar2,1);
      piVar5 = auVar11._0_8_;
      FUN_0018fd58(param_1,lVar7,piVar5,auVar11._8_8_,0);
      if (0xfffffff4 < auVar11._8_4_) {
        iVar3 = *piVar5;
        iVar4 = iVar3 + -1;
        *piVar5 = iVar4;
        if (iVar4 == 0 || iVar3 < 1) {
          FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar5,auVar11._8_8_);
        }
      }
    }
    else {
      FUN_0018fd58(param_1,lVar7,piVar5,uVar2,1);
    }
  }
  return ZEXT816(3) << 0x40;
}

/* ===== FUN_00193398 @ 00193398 [libNexusScriptRuntime69252.so] ===== */

/* WARNING: Removing unreachable block (ram,0x00193c08) */
/* WARNING: Removing unreachable block (ram,0x00194190) */

void FUN_00193398(int *param_1,undefined8 *param_2)

{
  void *__dest;
  uint uVar1;
  char cVar2;
  ushort uVar3;
  undefined2 uVar4;
  long lVar5;
  ushort uVar6;
  ushort uVar7;
  undefined8 *puVar8;
  bool bVar9;
  int iVar10;
  uint uVar11;
  int iVar12;
  undefined4 *puVar13;
  byte *pbVar14;
  undefined8 uVar15;
  char *pcVar16;
  int *piVar17;
  byte bVar18;
  size_t __n;
  int iVar19;
  undefined4 uVar20;
  ulong uVar21;
  undefined8 *puVar22;
  long *plVar23;
  ushort uVar24;
  uint uVar25;
  int *piVar26;
  int *piVar27;
  ulong uVar28;
  long lVar29;
  long *plVar30;
  long lVar31;
  uint uVar32;
  uint *puVar33;
  undefined8 *puVar34;
  undefined2 uVar35;
  long lVar36;
  uint *puVar37;
  byte bVar38;
  uint uVar39;
  long lVar40;
  long lVar41;
  int iVar42;
  long *plVar43;
  undefined1 auVar44 [16];
  uint local_114;
  int local_f8;
  undefined4 *local_f0;
  long local_e8;
  uint local_e0;
  int local_dc;
  uint local_d8;
  undefined4 local_d4;
  uint local_cc;
  int local_bc;
  uint local_b8;
  undefined4 local_b4;
  int local_b0;
  uint uStack_ac;
  void *pvStack_a8;
  long local_a0;
  long lStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  long local_70;
  
  lVar5 = tpidr_el0;
  local_70 = *(long *)(lVar5 + 0x28);
  uVar25 = *(uint *)((long)param_2 + 0xec);
  if (0 < (int)uVar25) {
    puVar13 = (undefined4 *)(param_2[0x1e] + 4);
    uVar21 = (ulong)uVar25;
    do {
      uVar21 = uVar21 - 1;
      *puVar13 = 0xffffffff;
      puVar13 = puVar13 + 2;
    } while (uVar21 != 0);
  }
  if (*(int *)((long)param_2 + 0x54) != 0) {
    *(undefined4 *)(param_2[0x1e] + 0xc) = 0xfffffffe;
  }
  uVar39 = *(uint *)((long)param_2 + 0x9c);
  uVar21 = (ulong)uVar39;
  if (0 < (int)uVar39) {
    uVar28 = 0;
    lVar31 = param_2[0x1e];
    puVar13 = (undefined4 *)(param_2[0x12] + 8);
    do {
      lVar29 = lVar31 + (long)(int)puVar13[-1] * 8;
      *puVar13 = *(undefined4 *)(lVar29 + 4);
      *(int *)(lVar29 + 4) = (int)uVar28;
      uVar28 = uVar28 + 1;
      puVar13 = puVar13 + 4;
    } while (uVar21 != uVar28);
  }
  if (2 < (int)uVar25) {
    lVar29 = param_2[0x1e];
    lVar31 = (ulong)uVar25 - 2;
    piVar17 = (int *)(lVar29 + 0x14);
    do {
      if (*piVar17 < 0) {
        *piVar17 = *(int *)(lVar29 + (long)piVar17[-1] * 8 + 4);
      }
      piVar17 = piVar17 + 2;
      lVar31 = lVar31 + -1;
    } while (lVar31 != 0);
  }
  if (0 < (int)uVar39) {
    piVar17 = (int *)(param_2[0x12] + 8);
    do {
      if ((*piVar17 < 0) && (1 < piVar17[-1])) {
        *piVar17 = *(int *)(param_2[0x1e] +
                            (long)*(int *)(param_2[0x1e] + (ulong)(uint)piVar17[-1] * 8) * 8 + 4);
      }
      piVar17 = piVar17 + 4;
      uVar21 = uVar21 - 1;
    } while (uVar21 != 0);
  }
  piVar17 = (int *)((long)param_2 + 0x9c);
  if (*(int *)((long)param_2 + 0x5c) != 0) {
    if ((*(int *)(param_2 + 7) == 0) && ((*(byte *)((long)param_2 + 0x86) & 1) == 0)) {
      if ((int)uVar39 < 0xffff) {
        if (((int)uVar39 < *(int *)(param_2 + 0x13)) ||
           (iVar12 = FUN_0016cbcc(param_1,param_2 + 0x12,0x10,param_2 + 0x13,uVar39 + 1),
           iVar12 == 0)) {
          puVar22 = (undefined8 *)(param_2[0x12] + (long)*piVar17 * 0x10);
          *piVar17 = *piVar17 + 1;
          *puVar22 = 0;
          puVar22[1] = 0;
          *(undefined4 *)((long)puVar22 + 0xc) = 0xffffff00;
          *(undefined4 *)puVar22 = 0x54;
          iVar12 = *piVar17 + -1;
        }
        else {
          iVar12 = -1;
        }
      }
      else {
        FUN_001405ec(param_1,"too many local variables");
        iVar12 = -1;
      }
      *(int *)((long)param_2 + 0xb4) = iVar12;
      if (*(int *)((long)param_2 + 0x54) != 0) {
        iVar12 = *piVar17;
        if (iVar12 < 0xffff) {
          if ((iVar12 < *(int *)(param_2 + 0x13)) ||
             (iVar12 = FUN_0016cbcc(param_1,param_2 + 0x12,0x10,param_2 + 0x13,iVar12 + 1),
             iVar12 == 0)) {
            puVar22 = (undefined8 *)(param_2[0x12] + (long)*piVar17 * 0x10);
            *piVar17 = *piVar17 + 1;
            *puVar22 = 0;
            puVar22[1] = 0;
            *(undefined4 *)((long)puVar22 + 0xc) = 0xffffff00;
            *(undefined4 *)puVar22 = 0x55;
            iVar12 = *piVar17 + -1;
          }
          else {
            iVar12 = -1;
          }
        }
        else {
          FUN_001405ec(param_1,"too many local variables");
          iVar12 = -1;
        }
        *(int *)(param_2 + 0x17) = iVar12;
      }
    }
    iVar12 = *(int *)((long)param_2 + 100);
    if (iVar12 != 0) {
      if (*(int *)((long)param_2 + 0xcc) < 0) {
        iVar10 = *piVar17;
        if (iVar10 < 0xffff) {
          if ((iVar10 < *(int *)(param_2 + 0x13)) ||
             (iVar10 = FUN_0016cbcc(param_1,param_2 + 0x12,0x10,param_2 + 0x13,iVar10 + 1),
             iVar10 == 0)) {
            puVar22 = (undefined8 *)(param_2[0x12] + (long)*piVar17 * 0x10);
            *piVar17 = *piVar17 + 1;
            *puVar22 = 0;
            puVar22[1] = 0;
            *(undefined4 *)((long)puVar22 + 0xc) = 0xffffff00;
            *(undefined4 *)puVar22 = 8;
            uVar25 = *piVar17 - 1;
            if ((-1 < (int)uVar25) && (*(int *)(param_2 + 0xf) != 0)) {
              lVar31 = param_2[0x12] + (ulong)uVar25 * 0x10;
              *(uint *)(lVar31 + 0xc) = *(uint *)(lVar31 + 0xc) | 2;
            }
          }
          else {
            uVar25 = 0xffffffff;
          }
        }
        else {
          FUN_001405ec(param_1,"too many local variables");
          uVar25 = 0xffffffff;
        }
        *(uint *)((long)param_2 + 0xcc) = uVar25;
        if (*(int *)(param_2 + 0x1a) < 0) goto LAB_001936ac;
LAB_00193600:
        iVar10 = *(int *)(param_2 + 0xf);
      }
      else {
        if (-1 < *(int *)(param_2 + 0x1a)) goto LAB_00193600;
LAB_001936ac:
        iVar10 = *piVar17;
        if (iVar10 < 0xffff) {
          if ((iVar10 < *(int *)(param_2 + 0x13)) ||
             (iVar10 = FUN_0016cbcc(param_1,param_2 + 0x12,0x10,param_2 + 0x13,iVar10 + 1),
             iVar10 == 0)) {
            puVar22 = (undefined8 *)(param_2[0x12] + (long)*piVar17 * 0x10);
            *piVar17 = *piVar17 + 1;
            *puVar22 = 0;
            puVar22[1] = 0;
            *(undefined4 *)((long)puVar22 + 0xc) = 0xffffff00;
            *(undefined4 *)puVar22 = 0x73;
            iVar10 = *piVar17 + -1;
          }
          else {
            iVar10 = -1;
          }
        }
        else {
          FUN_001405ec(param_1,"too many local variables");
          iVar10 = -1;
        }
        *(int *)(param_2 + 0x1a) = iVar10;
        iVar10 = *(int *)(param_2 + 0xf);
      }
      if ((iVar10 != 0) && (*(int *)((long)param_2 + 0xd4) < 0)) {
        iVar10 = *piVar17;
        if (iVar10 < 0xffff) {
          if ((iVar10 < *(int *)(param_2 + 0x13)) ||
             (iVar10 = FUN_0016cbcc(param_1,param_2 + 0x12,0x10,param_2 + 0x13,iVar10 + 1),
             iVar10 == 0)) {
            puVar22 = (undefined8 *)(param_2[0x12] + (long)*piVar17 * 0x10);
            *piVar17 = *piVar17 + 1;
            *puVar22 = 0;
            puVar22[1] = 0;
            *(undefined4 *)((long)puVar22 + 0xc) = 0xffffff00;
            *(undefined4 *)puVar22 = 0x74;
            iVar10 = *piVar17 + -1;
          }
          else {
            iVar10 = -1;
          }
        }
        else {
          FUN_001405ec(param_1,"too many local variables");
          iVar10 = -1;
        }
        *(int *)((long)param_2 + 0xd4) = iVar10;
      }
      if ((*(int *)(param_2 + 9) != 0) && (*(int *)(param_2 + 0x1b) < 0)) {
        iVar10 = *piVar17;
        if (iVar10 < 0xffff) {
          if ((iVar10 < *(int *)(param_2 + 0x13)) ||
             (iVar10 = FUN_0016cbcc(param_1,param_2 + 0x12,0x10,param_2 + 0x13,iVar10 + 1),
             iVar10 == 0)) {
            puVar22 = (undefined8 *)(param_2[0x12] + (long)*piVar17 * 0x10);
            *piVar17 = *piVar17 + 1;
            *puVar22 = 0;
            puVar22[1] = 0;
            *(undefined4 *)((long)puVar22 + 0xc) = 0xffffff00;
            *(undefined4 *)puVar22 = 0x75;
            iVar10 = *piVar17 + -1;
          }
          else {
            iVar10 = -1;
          }
        }
        else {
          FUN_001405ec(param_1,"too many local variables");
          iVar10 = -1;
        }
        *(int *)(param_2 + 0x1b) = iVar10;
      }
    }
    iVar10 = *(int *)(param_2 + 0xc);
    if (iVar10 != 0) {
      if (*(int *)((long)param_2 + 0xbc) < 0) {
        iVar42 = *piVar17;
        if (iVar42 < 0xffff) {
          if ((iVar42 < *(int *)(param_2 + 0x13)) ||
             (iVar42 = FUN_0016cbcc(param_1,param_2 + 0x12,0x10,param_2 + 0x13,iVar42 + 1),
             iVar42 == 0)) {
            puVar22 = (undefined8 *)(param_2[0x12] + (long)*piVar17 * 0x10);
            *piVar17 = *piVar17 + 1;
            *puVar22 = 0;
            puVar22[1] = 0;
            *(undefined4 *)((long)puVar22 + 0xc) = 0xffffff00;
            *(undefined4 *)puVar22 = 0x4f;
            if (-1 < *piVar17 + -1) {
              *(int *)((long)param_2 + 0xbc) = *piVar17 + -1;
            }
          }
        }
        else {
          FUN_001405ec(param_1,"too many local variables");
        }
      }
      if (((*(int *)((long)param_2 + 0x54) != 0) && ((*(byte *)((long)param_2 + 0x86) & 1) == 0)) &&
         (*(int *)(param_2 + 0x18) < 0)) {
        uVar25 = *(uint *)(param_2[0x1e] + 0xc);
        if (-1 < (int)uVar25) {
          lVar31 = param_2[0x12];
          do {
            uVar21 = (ulong)uVar25;
            if (*(int *)(lVar31 + uVar21 * 0x10 + 4) != 1) break;
            if (*(int *)(lVar31 + uVar21 * 0x10) == 0x4f) goto LAB_001938bc;
            uVar25 = *(uint *)(lVar31 + uVar21 * 0x10 + 8);
          } while (-1 < (int)uVar25);
        }
        uVar25 = 0xffffffff;
LAB_001938bc:
        if ((int)uVar25 < 0) {
          iVar42 = *piVar17;
          if (iVar42 < 0xffff) {
            if ((iVar42 < *(int *)(param_2 + 0x13)) ||
               (iVar42 = FUN_0016cbcc(param_1,param_2 + 0x12,0x10,param_2 + 0x13,iVar42 + 1),
               iVar42 == 0)) {
              puVar22 = (undefined8 *)(param_2[0x12] + (long)*piVar17 * 0x10);
              *piVar17 = *piVar17 + 1;
              *puVar22 = 0;
              puVar22[1] = 0;
              *(undefined4 *)((long)puVar22 + 0xc) = 0xffffff00;
              *(undefined4 *)puVar22 = 0x4f;
              uVar25 = *piVar17 - 1;
              if (-1 < (int)uVar25) {
                lVar29 = param_2[0x1e];
                lVar31 = param_2[0x12] + (ulong)uVar25 * 0x10;
                *(undefined4 *)(lVar31 + 8) = *(undefined4 *)(lVar29 + 0xc);
                *(uint *)(lVar29 + 0xc) = uVar25;
                *(undefined4 *)(lVar31 + 4) = 1;
                *(uint *)(lVar31 + 0xc) = *(uint *)(lVar31 + 0xc) | 2;
                *(uint *)(param_2 + 0x18) = uVar25;
              }
            }
          }
          else {
            FUN_001405ec(param_1,"too many local variables");
          }
        }
      }
    }
    if ((*(int *)((long)param_2 + 0x44) != 0) && (*(int *)(param_2 + 0x11) != 0)) {
      FUN_001ac00c(param_1,param_2);
    }
    if ((*(int *)(param_2 + 7) == 0) && (*(int *)(param_2 + 0x33) != 0)) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x79c2,"void add_eval_variables(JSContext *, JSFunctionDef *)",
                "s->is_eval || s->closure_var_count == 0");
    }
    puVar34 = (undefined8 *)param_2[1];
    puVar22 = param_2;
    while (puVar8 = puVar34, puVar8 != (undefined8 *)0x0) {
      iVar42 = *(int *)((long)puVar22 + 0x14);
      if (iVar12 == 0) {
        if (*(int *)((long)puVar8 + 100) == 0) {
          iVar12 = 0;
        }
        else {
          if (*(int *)((long)puVar8 + 0xcc) < 0) {
            iVar12 = *(int *)((long)puVar8 + 0x9c);
            if (iVar12 < 0xffff) {
              if ((iVar12 < *(int *)(puVar8 + 0x13)) ||
                 (iVar12 = FUN_0016cbcc(param_1,puVar8 + 0x12,0x10,puVar8 + 0x13,iVar12 + 1),
                 iVar12 == 0)) {
                puVar22 = (undefined8 *)(puVar8[0x12] + (long)*(int *)((long)puVar8 + 0x9c) * 0x10);
                *(int *)((long)puVar8 + 0x9c) = *(int *)((long)puVar8 + 0x9c) + 1;
                *puVar22 = 0;
                puVar22[1] = 0;
                *(undefined4 *)((long)puVar22 + 0xc) = 0xffffff00;
                *(undefined4 *)puVar22 = 8;
                uVar25 = *(int *)((long)puVar8 + 0x9c) - 1;
                if ((-1 < (int)uVar25) && (*(int *)(puVar8 + 0xf) != 0)) {
                  lVar31 = puVar8[0x12] + (ulong)uVar25 * 0x10;
                  *(uint *)(lVar31 + 0xc) = *(uint *)(lVar31 + 0xc) | 2;
                }
              }
              else {
                uVar25 = 0xffffffff;
              }
            }
            else {
              FUN_001405ec(param_1,"too many local variables");
              uVar25 = 0xffffffff;
            }
            *(uint *)((long)puVar8 + 0xcc) = uVar25;
            if (*(int *)(puVar8 + 0x1a) < 0) goto LAB_00193a44;
LAB_00193994:
            if (*(int *)(puVar8 + 0xf) == 0) goto LAB_00193ac0;
LAB_00193ab8:
            if (-1 < *(int *)((long)puVar8 + 0xd4)) goto LAB_00193ac0;
            iVar12 = *(int *)((long)puVar8 + 0x9c);
            if (iVar12 < 0xffff) {
              if ((iVar12 < *(int *)(puVar8 + 0x13)) ||
                 (iVar12 = FUN_0016cbcc(param_1,puVar8 + 0x12,0x10,puVar8 + 0x13,iVar12 + 1),
                 iVar12 == 0)) {
                puVar22 = (undefined8 *)(puVar8[0x12] + (long)*(int *)((long)puVar8 + 0x9c) * 0x10);
                *(int *)((long)puVar8 + 0x9c) = *(int *)((long)puVar8 + 0x9c) + 1;
                *puVar22 = 0;
                puVar22[1] = 0;
                *(undefined4 *)((long)puVar22 + 0xc) = 0xffffff00;
                *(undefined4 *)puVar22 = 0x74;
                iVar12 = *(int *)((long)puVar8 + 0x9c) + -1;
              }
              else {
                iVar12 = -1;
              }
            }
            else {
              FUN_001405ec(param_1,"too many local variables");
              iVar12 = -1;
            }
            *(int *)((long)puVar8 + 0xd4) = iVar12;
            iVar12 = *(int *)(puVar8 + 9);
          }
          else {
            if (-1 < *(int *)(puVar8 + 0x1a)) goto LAB_00193994;
LAB_00193a44:
            iVar12 = *(int *)((long)puVar8 + 0x9c);
            if (iVar12 < 0xffff) {
              if ((iVar12 < *(int *)(puVar8 + 0x13)) ||
                 (iVar12 = FUN_0016cbcc(param_1,puVar8 + 0x12,0x10,puVar8 + 0x13,iVar12 + 1),
                 iVar12 == 0)) {
                puVar22 = (undefined8 *)(puVar8[0x12] + (long)*(int *)((long)puVar8 + 0x9c) * 0x10);
                *(int *)((long)puVar8 + 0x9c) = *(int *)((long)puVar8 + 0x9c) + 1;
                *puVar22 = 0;
                puVar22[1] = 0;
                *(undefined4 *)((long)puVar22 + 0xc) = 0xffffff00;
                *(undefined4 *)puVar22 = 0x73;
                iVar12 = *(int *)((long)puVar8 + 0x9c) + -1;
              }
              else {
                iVar12 = -1;
              }
            }
            else {
              FUN_001405ec(param_1,"too many local variables");
              iVar12 = -1;
            }
            *(int *)(puVar8 + 0x1a) = iVar12;
            if (*(int *)(puVar8 + 0xf) != 0) goto LAB_00193ab8;
LAB_00193ac0:
            iVar12 = *(int *)(puVar8 + 9);
          }
          if ((iVar12 == 0) || (-1 < *(int *)(puVar8 + 0x1b))) {
            iVar12 = 1;
          }
          else {
            iVar12 = *(int *)((long)puVar8 + 0x9c);
            if (iVar12 < 0xffff) {
              if ((iVar12 < *(int *)(puVar8 + 0x13)) ||
                 (iVar12 = FUN_0016cbcc(param_1,puVar8 + 0x12,0x10,puVar8 + 0x13,iVar12 + 1),
                 iVar12 == 0)) {
                puVar22 = (undefined8 *)(puVar8[0x12] + (long)*(int *)((long)puVar8 + 0x9c) * 0x10);
                *(int *)((long)puVar8 + 0x9c) = *(int *)((long)puVar8 + 0x9c) + 1;
                *puVar22 = 0;
                puVar22[1] = 0;
                *(undefined4 *)((long)puVar22 + 0xc) = 0xffffff00;
                *(undefined4 *)puVar22 = 0x75;
                iVar19 = *(int *)((long)puVar8 + 0x9c) + -1;
              }
              else {
                iVar19 = -1;
              }
            }
            else {
              FUN_001405ec(param_1,"too many local variables");
              iVar19 = -1;
            }
            iVar12 = 1;
            *(int *)(puVar8 + 0x1b) = iVar19;
          }
        }
      }
      if (iVar10 == 0) {
        if (*(int *)(puVar8 + 0xc) == 0) {
          iVar10 = 0;
        }
        else if (*(int *)((long)puVar8 + 0xbc) < 0) {
          iVar10 = *(int *)((long)puVar8 + 0x9c);
          if (iVar10 < 0xffff) {
            if ((iVar10 < *(int *)(puVar8 + 0x13)) ||
               (iVar10 = FUN_0016cbcc(param_1,puVar8 + 0x12,0x10,puVar8 + 0x13,iVar10 + 1),
               iVar10 == 0)) {
              puVar22 = (undefined8 *)(puVar8[0x12] + (long)*(int *)((long)puVar8 + 0x9c) * 0x10);
              *(int *)((long)puVar8 + 0x9c) = *(int *)((long)puVar8 + 0x9c) + 1;
              *puVar22 = 0;
              puVar22[1] = 0;
              *(undefined4 *)((long)puVar22 + 0xc) = 0xffffff00;
              *(undefined4 *)puVar22 = 0x4f;
              iVar19 = *(int *)((long)puVar8 + 0x9c) + -1;
              iVar10 = 1;
              if (-1 < iVar19) {
                iVar10 = 1;
                *(int *)((long)puVar8 + 0xbc) = iVar19;
              }
              goto LAB_00193ad8;
            }
          }
          else {
            FUN_001405ec(param_1,"too many local variables");
          }
          iVar10 = 1;
        }
        else {
          iVar10 = 1;
        }
      }
LAB_00193ad8:
      if ((*(int *)((long)puVar8 + 0x44) != 0) && (*(int *)(puVar8 + 0x11) != 0)) {
        FUN_001ac00c(param_1,puVar8);
      }
      uVar25 = *(uint *)(puVar8[0x1e] + (long)iVar42 * 8 + 4);
      while (-1 < (int)uVar25) {
        puVar13 = (undefined4 *)(puVar8[0x12] + (ulong)uVar25 * 0x10);
        uVar39 = puVar13[3];
        puVar13[3] = uVar39 | 4;
        FUN_001ac13c(param_1,param_2,puVar8,1,0,(ulong)uVar25,*puVar13,uVar39 & 1,uVar39 >> 1 & 1,
                     uVar39 >> 4 & 0xf);
        uVar25 = puVar13[2];
      }
      if (uVar25 == 0xfffffffe) {
        if (0 < *(int *)((long)puVar8 + 0x9c)) {
          lVar31 = 0;
          uVar21 = 0;
          do {
            lVar29 = puVar8[0x12];
            if ((*(int *)(lVar29 + lVar31 + 4) == 0) &&
               ((iVar42 = *(int *)(lVar29 + lVar31),
                iVar42 - 0x55U < 0x21 &&
                (1L << ((ulong)(iVar42 - 0x55U) & 0x3f) & 0x1c0000001U) != 0 || iVar42 == 8 ||
                ((*(uint *)(lVar29 + lVar31 + 0xc) & 0xf0) == 0x40)))) {
              FUN_001ac13c(param_1,param_2,puVar8,1,0,uVar21 & 0xffffffff,iVar42,0,
                           *(uint *)(lVar29 + lVar31 + 0xc) >> 1 & 1,0);
            }
            uVar21 = uVar21 + 1;
            lVar31 = lVar31 + 0x10;
          } while ((long)uVar21 < (long)*(int *)((long)puVar8 + 0x9c));
        }
      }
      else {
        if (0 < *(int *)((long)puVar8 + 0xac)) {
          lVar31 = 0;
          uVar21 = 0;
          do {
            iVar42 = *(int *)(puVar8[0x14] + lVar31);
            if (iVar42 != 0) {
              FUN_001ac13c(param_1,param_2,puVar8,1,1,uVar21 & 0xffffffff,iVar42,0,
                           *(uint *)(puVar8[0x14] + lVar31 + 0xc) >> 1 & 1,0);
            }
            uVar21 = uVar21 + 1;
            lVar31 = lVar31 + 0x10;
          } while ((long)uVar21 < (long)*(int *)((long)puVar8 + 0xac));
        }
        if (0 < *(int *)((long)puVar8 + 0x9c)) {
          lVar31 = 0;
          uVar21 = 0;
          do {
            lVar29 = puVar8[0x12];
            if (((*(int *)(lVar29 + lVar31 + 4) == 0) &&
                (iVar42 = *(int *)(lVar29 + lVar31), iVar42 != 0)) && (iVar42 != 0x53)) {
              FUN_001ac13c(param_1,param_2,puVar8,1,0,uVar21 & 0xffffffff,iVar42,0,
                           *(uint *)(lVar29 + lVar31 + 0xc) >> 1 & 1,0);
            }
            uVar21 = uVar21 + 1;
            lVar31 = lVar31 + 0x10;
          } while ((long)uVar21 < (long)*(int *)((long)puVar8 + 0x9c));
        }
      }
      if ((*(int *)(puVar8 + 7) != 0) && (0 < *(int *)(puVar8 + 0x33))) {
        lVar31 = 0;
        uVar21 = 0;
        do {
          bVar38 = *(byte *)(puVar8[0x34] + lVar31);
          FUN_001ac13c(param_1,param_2,puVar8,0,bVar38 >> 1 & 1,uVar21 & 0xffffffff,
                       *(undefined4 *)((byte *)(puVar8[0x34] + lVar31) + 4),bVar38 >> 2 & 1,
                       bVar38 >> 3 & 1,bVar38 >> 4);
          uVar21 = uVar21 + 1;
          lVar31 = lVar31 + 8;
        } while ((long)uVar21 < (long)*(int *)(puVar8 + 0x33));
      }
      puVar22 = puVar8;
      puVar34 = (undefined8 *)puVar8[1];
    }
  }
  lVar31 = param_2[0x43];
  if (lVar31 != 0) {
    if (0 < *(int *)((long)param_2 + 0x11c)) {
      lVar40 = 0;
      lVar29 = 0;
      do {
        iVar12 = *(int *)(param_2 + 0x33);
        if (0xfffe < iVar12) {
          FUN_001405ec(param_1,"too many closure variables");
          goto LAB_001955b8;
        }
        uVar25 = *(uint *)(param_2[0x25] + lVar40 + 0xc);
        bVar38 = *(byte *)(param_2[0x25] + lVar40 + 4);
        if ((*(int *)((long)param_2 + 0x19c) <= iVar12) &&
           (iVar12 = FUN_0016cbcc(param_1,param_2 + 0x34,8,(int *)((long)param_2 + 0x19c),iVar12 + 1
                                 ), iVar12 != 0)) goto LAB_001955b8;
        pbVar14 = (byte *)(param_2[0x34] + (long)*(int *)(param_2 + 0x33) * 8);
        *(int *)(param_2 + 0x33) = *(int *)(param_2 + 0x33) + 1;
        *pbVar14 = bVar38 & 4 | (byte)((bVar38 >> 1 & 1) << 3) | 1;
        *(short *)(pbVar14 + 2) = (short)lVar29;
        if (0xe3 < (int)uVar25) {
          piVar26 = *(int **)(*(long *)(*(long *)(param_1 + 6) + 0x60) + (ulong)uVar25 * 8);
          *piVar26 = *piVar26 + 1;
        }
        *(uint *)(pbVar14 + 4) = uVar25;
        if (*(int *)(param_2 + 0x33) + -1 < 0) goto LAB_001955b8;
        lVar29 = lVar29 + 1;
        lVar40 = lVar40 + 0x10;
      } while (lVar29 < *(int *)((long)param_2 + 0x11c));
    }
    if (0 < *(int *)(lVar31 + 0x30)) {
      lVar29 = 0;
      do {
        puVar37 = (uint *)(*(long *)(lVar31 + 0x28) + lVar29 * 0x20);
        if (puVar37[4] == 0) {
          if (0 < (int)*(uint *)(param_2 + 0x33)) {
            uVar28 = param_2[0x34];
            uVar21 = 0;
            puVar33 = (uint *)(uVar28 + 4);
            do {
              uVar25 = (uint)uVar21;
              if (*puVar33 != puVar37[5]) {
                uVar25 = (uint)uVar28;
              }
              uVar28 = (ulong)uVar25;
              if (*puVar33 == puVar37[5]) goto LAB_00194124;
              uVar21 = uVar21 + 1;
              puVar33 = puVar33 + 2;
            } while (*(uint *)(param_2 + 0x33) != uVar21);
          }
          uVar25 = 0xffffffff;
LAB_00194124:
          if ((int)uVar25 < 0) {
            uVar15 = FUN_00142bd0(*(undefined8 *)(param_1 + 6),&local_b0);
            FUN_00143454(param_1,"exported variable \'%s\' does not exist",uVar15);
            goto LAB_001955b8;
          }
          *puVar37 = uVar25;
        }
        lVar29 = lVar29 + 1;
      } while (lVar29 < *(int *)(lVar31 + 0x30));
    }
  }
  puVar22 = (undefined8 *)param_2[4];
  while (puVar22 != param_2 + 3) {
    puVar34 = (undefined8 *)puVar22[1];
    uVar25 = *(uint *)(puVar22 + -3);
    auVar44 = FUN_00193398(param_1,puVar22 + -5);
    if (auVar44._8_4_ == 6) goto LAB_001955b8;
    if ((int)uVar25 < 0) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x82de,"JSValue js_create_function(JSContext *, JSFunctionDef *)","cpool_idx >= 0")
      ;
    }
    *(undefined1 (*) [16])(param_2[0x31] + (ulong)uVar25 * 0x10) = auVar44;
    puVar22 = puVar34;
  }
  lVar31 = param_2[0x26];
  uVar25 = *(uint *)(param_2 + 0x27);
  plVar43 = (long *)(param_1 + 6);
  local_e8 = lVar31;
  local_e0 = uVar25;
  FUN_001d2548(&local_b0,*plVar43,FUN_00138cc0);
  if (0 < *(int *)((long)param_2 + 0x11c)) {
    lVar29 = 0;
    do {
      lVar40 = param_2[0x25];
      piVar26 = (int *)(lVar40 + lVar29 * 0x10);
      if (0 < *(int *)(param_2 + 0x33)) {
        lVar36 = 0;
        lVar41 = 0;
        do {
          uVar39 = *(uint *)(param_2[0x34] + lVar36 + 4);
          if (uVar39 == piVar26[3]) {
            if ((*(int *)((long)param_2 + 0x3c) == 2) &&
               ((*(byte *)(param_2[0x34] + lVar36) >> 3 & 1) != 0)) {
              FUN_001d27c4(&local_b0,0x30);
              local_b4 = piVar26[3];
              if (0xe3 < (int)local_b4) {
                piVar26 = *(int **)(*(long *)(*plVar43 + 0x60) + (ulong)local_b4 * 8);
                *piVar26 = *piVar26 + 1;
              }
              FUN_001d26dc(&local_b0,&local_b4,4);
              FUN_001d27c4(&local_b0,1);
            }
            goto LAB_0019421c;
          }
          if ((uVar39 & 0xfffffffe) == 0x54) goto LAB_0019421c;
          lVar41 = lVar41 + 1;
          lVar36 = lVar36 + 8;
        } while (lVar41 < *(int *)(param_2 + 0x33));
      }
      FUN_001d27c4(&local_b0,0x3f);
      local_b4 = piVar26[3];
      if (0xe3 < (int)local_b4) {
        piVar27 = *(int **)(*(long *)(*plVar43 + 0x60) + (ulong)local_b4 * 8);
        *piVar27 = *piVar27 + 1;
      }
      FUN_001d26dc(&local_b0,&local_b4,4);
      uVar11 = (uint)*(byte *)(lVar40 + lVar29 * 0x10 + 4) << 6;
      uVar39 = uVar11 & 0xffffff80;
      if (-1 < *piVar26) {
        uVar39 = uVar11 | 0x40;
      }
      FUN_001d27c4(&local_b0,uVar39);
LAB_0019421c:
      lVar29 = lVar29 + 1;
    } while (lVar29 < *(int *)((long)param_2 + 0x11c));
  }
  plVar23 = param_2 + 0x26;
  if ((int)uVar25 < 1) {
LAB_00195450:
    FUN_001d2b08(plVar23);
    param_2[0x29] = lStack_98;
    param_2[0x28] = local_a0;
    param_2[0x2b] = uStack_88;
    param_2[0x2a] = local_90;
    param_2[0x27] = pvStack_a8;
    *plVar23 = CONCAT44(uStack_ac,local_b0);
    if (*(int *)(param_2 + 0x29) == 0) {
      iVar12 = FUN_001a98b0(param_1,param_2);
      if (iVar12 == 0) {
        lVar31 = param_2[0x26];
        uVar21 = param_2[0x27];
        local_b0 = (int)uVar21;
        pvStack_a8 = (void *)(**(code **)*plVar43)
                                       ((undefined8 *)*plVar43 + 4,
                                        -(uVar21 >> 0x1f & 1) & 0xfffffffe00000000 |
                                        (uVar21 & 0xffffffff) << 1);
        if (pvStack_a8 == (void *)0x0) {
          FUN_00138d58(param_1);
          pvStack_a8 = (void *)0x0;
LAB_00195b0c:
          bVar9 = true;
        }
        else {
          if (pvStack_a8 == (void *)0x0) goto LAB_00195b0c;
          if (0 < (int)uVar21) {
            memset(pvStack_a8,0xff,(uVar21 & 0xffffffff) << 1);
          }
          lStack_98 = 0;
          local_a0 = (**(code **)*plVar43)
                               ((undefined8 *)*plVar43 + 4,(long)(uVar21 << 0x20) >> 0x1e);
          if (local_a0 == 0) {
            FUN_00138d58(param_1);
            local_a0 = 0;
LAB_0019554c:
            (**(code **)(*plVar43 + 8))(*plVar43 + 0x20,lStack_98);
            (**(code **)(*plVar43 + 8))(*plVar43 + 0x20,local_a0);
            (**(code **)(*plVar43 + 8))(*plVar43 + 0x20,pvStack_a8);
            bVar9 = true;
            uVar25 = 0;
          }
          else {
            if (local_a0 == 0) goto LAB_0019554c;
            uStack_ac = 0;
            local_90 = 0;
            iVar12 = FUN_001ae730(param_1,&local_b0,0,0,0,0xffffffff);
            if (iVar12 != 0) goto LAB_0019554c;
            while (0 < (int)local_90) {
              uVar25 = (int)local_90 - 1;
              uVar21 = (ulong)local_90 >> 0x20;
              local_90 = CONCAT44((int)uVar21,uVar25);
              uVar25 = *(uint *)(lStack_98 + (ulong)uVar25 * 4);
              lVar29 = (long)(int)uVar25;
              pbVar14 = (byte *)(lVar31 + lVar29);
              bVar38 = *pbVar14;
              uVar39 = (uint)bVar38;
              if ((bVar38 + 8 & 0xff) < 9) {
                pcVar16 = "invalid opcode (op=%d, pc=%d)";
LAB_001959e4:
                FUN_001405ec(param_1,pcVar16,uVar39,uVar25);
                goto LAB_0019554c;
              }
              uVar11 = bVar38 + 0x13;
              if (bVar38 < 0xb6) {
                uVar11 = uVar39;
              }
              uVar21 = (ulong)uVar11;
              iVar12 = uVar25 + (byte)(&DAT_00113f23)[uVar21 * 4];
              if (local_b0 < iVar12) {
                pcVar16 = "bytecode buffer overflow (op=%d, pc=%d)";
                goto LAB_001959e4;
              }
              uVar32 = (uint)(byte)(&DAT_00113f24)[uVar21 * 4];
              if ((uVar21 - 0x21 < 6) || (uVar11 == 0x31)) {
                uVar32 = *(ushort *)(pbVar14 + 1) + uVar32;
              }
              else if (uVar21 - 0x103 < 4) {
                uVar32 = (uVar39 + uVar32) - 0xf0;
              }
              uVar6 = *(ushort *)((long)pvStack_a8 + (long)(int)uVar25 * 2);
              if ((int)(uint)uVar6 < (int)uVar32) {
                pcVar16 = "stack underflow (op=%d, pc=%d)";
                goto LAB_001959e4;
              }
              uVar1 = *(uint *)(local_a0 + (long)(int)uVar25 * 4);
              uVar28 = (ulong)uVar1;
              uVar11 = (uVar6 - uVar32) + (uint)(byte)(&DAT_00113f25)[uVar21 * 4];
              if (((int)uStack_ac < (int)uVar11) && (uStack_ac = uVar11, 0xfffe < (int)uVar11)) {
                pcVar16 = "stack overflow (op=%d, pc=%d)";
                goto LAB_001959e4;
              }
              uVar39 = uVar11;
              switch(bVar38) {
              case 0xe:
                goto switchD_00195858_caseD_e;
              case 0xf:
              case 0x10:
                uVar39 = uVar11 - 1;
                goto switchD_00195858_caseD_e;
              case 0x23:
              case 0x25:
              case 0x28:
              case 0x29:
              case 0x2e:
              case 0x2f:
              case 0x30:
              case 0x6f:
                goto switchD_00195858_caseD_23;
              case 0x6a:
              case 0x6b:
                iVar10 = uVar25 + *(int *)(pbVar14 + 1) + 1;
                goto LAB_00195730;
              case 0x6c:
                iVar12 = *(int *)(pbVar14 + 1);
                goto LAB_00195960;
              case 0x6d:
                iVar10 = FUN_001ae730(param_1,&local_b0,uVar25 + *(int *)(pbVar14 + 1) + 1,bVar38,
                                      uVar11,uVar28);
                uVar28 = (ulong)uVar25;
                goto joined_r0x001958d8;
              case 0x6e:
                iVar10 = uVar25 + *(int *)(pbVar14 + 1) + 1;
                goto LAB_00195720;
              case 0x70:
                if ((int)uVar1 < 0) {
                  FUN_001405ec(param_1,"nip_catch: no catch op (pc=%d)",uVar25);
                  goto LAB_0019554c;
                }
                pcVar16 = (char *)(lVar31 + uVar28);
                lVar29 = uVar28 * 2;
                uVar28 = (ulong)*(uint *)(local_a0 + uVar28 * 4);
                uVar11 = (uint)*(ushort *)((long)pvStack_a8 + lVar29);
                if (*pcVar16 != 'm') {
                  uVar11 = uVar11 + 1;
                }
                uVar11 = uVar11 + 1;
                break;
              case 0x74:
              case 0x76:
                iVar10 = uVar25 + *(int *)(pbVar14 + 5) + 5;
LAB_00195720:
                uVar39 = uVar11 + 1;
                goto LAB_00195730;
              case 0x75:
                iVar10 = *(int *)(pbVar14 + 5);
                iVar42 = -1;
                goto LAB_00195918;
              case 0x77:
              case 0x78:
              case 0x79:
                iVar10 = *(int *)(pbVar14 + 5);
                iVar42 = 2;
LAB_00195918:
                iVar10 = uVar25 + iVar10 + 5;
                uVar39 = uVar11 + iVar42;
                goto LAB_00195730;
              case 0x7f:
              case 0x80:
                uVar28 = (ulong)uVar25;
                break;
              case 0x85:
                uVar39 = uVar11 + 2;
switchD_00195858_caseD_e:
                if (-1 < (int)uVar1) {
                  uVar25 = (uint)*(ushort *)((long)pvStack_a8 + uVar28 * 2);
                  if (*(char *)(lVar31 + uVar28) != 'm') {
                    uVar25 = uVar25 + 1;
                  }
                  if (uVar39 == uVar25) {
                    uVar28 = (ulong)*(uint *)(local_a0 + uVar28 * 4);
                  }
                }
                break;
              case 0xec:
              case 0xed:
                iVar10 = (int)(lVar29 + 1) + (int)*(char *)(lVar31 + lVar29 + 1);
LAB_00195730:
                iVar10 = FUN_001ae730(param_1,&local_b0,iVar10,bVar38,uVar39,uVar28);
joined_r0x001958d8:
                if (iVar10 != 0) goto LAB_0019554c;
                break;
              case 0xee:
                iVar12 = (int)(lVar29 + 1) + (int)*(char *)(lVar31 + lVar29 + 1);
                break;
              case 0xef:
                iVar12 = (int)*(short *)(pbVar14 + 1);
LAB_00195960:
                iVar12 = uVar25 + iVar12 + 1;
              }
              iVar12 = FUN_001ae730(param_1,&local_b0,iVar12,bVar38,uVar11,uVar28);
              if (iVar12 != 0) goto LAB_0019554c;
switchD_00195858_caseD_23:
            }
            (**(code **)(*plVar43 + 8))(*plVar43 + 0x20,lStack_98);
            (**(code **)(*plVar43 + 8))(*plVar43 + 0x20,local_a0);
            (**(code **)(*plVar43 + 8))(*plVar43 + 0x20,pvStack_a8);
            bVar9 = false;
            uVar25 = uStack_ac;
          }
        }
        if (!bVar9) {
          lVar31 = 0x80;
          if ((*(byte *)((long)param_2 + 0x86) & 2) != 0) {
            lVar31 = 0x60;
          }
          iVar12 = (int)lVar31 + *(int *)(param_2 + 0x32) * 0x10;
          if (((*(byte *)((long)param_2 + 0x86) >> 1 & 1) == 0) ||
             (iVar10 = iVar12, *(int *)((long)param_2 + 0x5c) != 0)) {
            iVar10 = iVar12 + (*(int *)((long)param_2 + 0x9c) + *(int *)((long)param_2 + 0xac)) *
                              0x10;
          }
          iVar42 = iVar10 + *(int *)(param_2 + 0x33) * 8;
          puVar13 = (undefined4 *)FUN_00138dac(param_1,(long)(iVar42 + *(int *)(param_2 + 0x27)));
          if (puVar13 != (undefined4 *)0x0) {
            __dest = (void *)((long)puVar13 + (long)iVar42);
            *puVar13 = 1;
            *(void **)(puVar13 + 8) = __dest;
            __n = param_2[0x27];
            puVar13[10] = (int)__n;
            memcpy(__dest,(void *)param_2[0x26],__n);
            (**(code **)(*plVar43 + 8))(*plVar43 + 0x20,param_2[0x26]);
            param_2[0x26] = 0;
            puVar13[0xb] = *(undefined4 *)(param_2 + 0x11);
            iVar42 = *(int *)((long)param_2 + 0xac);
            if (0 < *(int *)((long)param_2 + 0x9c) + iVar42) {
              if (((*(byte *)((long)param_2 + 0x86) >> 1 & 1) == 0) ||
                 (*(int *)((long)param_2 + 0x5c) != 0)) {
                *(void **)(puVar13 + 0xc) = (void *)((long)puVar13 + (long)iVar12);
                memcpy((void *)((long)puVar13 + (long)iVar12),(void *)param_2[0x14],
                       (long)iVar42 << 4);
                memcpy((void *)(*(long *)(puVar13 + 0xc) +
                               (long)*(int *)((long)param_2 + 0xac) * 0x10),(void *)param_2[0x12],
                       (long)*(int *)((long)param_2 + 0x9c) << 4);
              }
              else {
                if (0 < *piVar17) {
                  lVar40 = 0;
                  lVar29 = 0;
                  do {
                    if ((0xe3 < (int)*(uint *)(param_2[0x12] + lVar40)) &&
                       (piVar26 = *(int **)(*(long *)(*plVar43 + 0x60) +
                                           (ulong)*(uint *)(param_2[0x12] + lVar40) * 8),
                       iVar12 = *piVar26, iVar42 = iVar12 + -1, *piVar26 = iVar42,
                       iVar42 == 0 || iVar12 < 1)) {
                      FUN_001418dc();
                    }
                    lVar29 = lVar29 + 1;
                    lVar40 = lVar40 + 0x10;
                  } while (lVar29 < *piVar17);
                }
                if (0 < *(int *)((long)param_2 + 0xac)) {
                  lVar40 = 0;
                  lVar29 = 0;
                  do {
                    if ((0xe3 < (int)*(uint *)(param_2[0x14] + lVar40)) &&
                       (piVar17 = *(int **)(*(long *)(*plVar43 + 0x60) +
                                           (ulong)*(uint *)(param_2[0x14] + lVar40) * 8),
                       iVar12 = *piVar17, iVar42 = iVar12 + -1, *piVar17 = iVar42,
                       iVar42 == 0 || iVar12 < 1)) {
                      FUN_001418dc();
                    }
                    lVar29 = lVar29 + 1;
                    lVar40 = lVar40 + 0x10;
                  } while (lVar29 < *(int *)((long)param_2 + 0xac));
                }
                if (0 < *(int *)(param_2 + 0x33)) {
                  lVar29 = 0;
                  lVar40 = 4;
                  do {
                    if ((0xe3 < (int)*(uint *)(param_2[0x34] + lVar40)) &&
                       (piVar17 = *(int **)(*(long *)(*plVar43 + 0x60) +
                                           (ulong)*(uint *)(param_2[0x34] + lVar40) * 8),
                       iVar12 = *piVar17, iVar42 = iVar12 + -1, *piVar17 = iVar42,
                       iVar42 == 0 || iVar12 < 1)) {
                      FUN_001418dc();
                    }
                    lVar29 = lVar29 + 1;
                    *(undefined4 *)(param_2[0x34] + lVar40) = 0;
                    lVar40 = lVar40 + 8;
                  } while (lVar29 < *(int *)(param_2 + 0x33));
                }
              }
              *(short *)((long)puVar13 + 0x42) = (short)*(undefined4 *)((long)param_2 + 0x9c);
              *(short *)(puVar13 + 0x10) = (short)*(undefined4 *)((long)param_2 + 0xac);
              *(short *)(puVar13 + 0x11) = (short)*(undefined4 *)(param_2 + 0x16);
              (**(code **)(*plVar43 + 8))(*plVar43 + 0x20,param_2[0x14]);
              (**(code **)(*plVar43 + 8))(*plVar43 + 0x20,param_2[0x12]);
            }
            iVar12 = *(int *)(param_2 + 0x32);
            puVar13[0x16] = iVar12;
            if (iVar12 != 0) {
              *(void **)(puVar13 + 0x14) = (void *)((long)puVar13 + lVar31);
              memcpy((void *)((long)puVar13 + lVar31),(void *)param_2[0x31],(long)iVar12 << 4);
            }
            (**(code **)(*plVar43 + 8))(*plVar43 + 0x20,param_2[0x31]);
            param_2[0x31] = 0;
            *(short *)((long)puVar13 + 0x46) = (short)uVar25;
            if ((*(byte *)((long)param_2 + 0x86) >> 1 & 1) == 0) {
              *(ushort *)((long)puVar13 + 0x19) = *(ushort *)((long)puVar13 + 0x19) | 0x400;
              puVar13[0x18] = *(undefined4 *)(param_2 + 0x3a);
              puVar13[0x19] = *(undefined4 *)((long)param_2 + 0x1d4);
              lVar31 = param_2[0x3c];
              lVar29 = (**(code **)(*plVar43 + 0x10))(*plVar43 + 0x20,param_2[0x3b],lVar31);
              if ((lVar31 != 0) && (lVar29 == 0)) {
                FUN_00138d58(param_1);
                lVar29 = 0;
              }
              *(long *)(puVar13 + 0x1c) = lVar29;
              if (lVar29 == 0) {
                *(undefined8 *)(puVar13 + 0x1c) = param_2[0x3b];
              }
              puVar13[0x1b] = (int)param_2[0x3c];
              *(undefined8 *)(puVar13 + 0x1e) = param_2[0x41];
              puVar13[0x1a] = *(undefined4 *)(param_2 + 0x42);
            }
            else {
              FUN_00140140(param_1,*(undefined4 *)(param_2 + 0x3a));
              FUN_001d2b08(param_2 + 0x3b);
            }
            if ((undefined8 *)param_2[0x1e] != param_2 + 0x1f) {
              (**(code **)(*plVar43 + 8))(*plVar43 + 0x20);
            }
            iVar12 = *(int *)(param_2 + 0x33);
            puVar13[0x17] = iVar12;
            if (iVar12 != 0) {
              *(void **)(puVar13 + 0xe) = (void *)((long)puVar13 + (long)iVar10);
              memcpy((void *)((long)puVar13 + (long)iVar10),(void *)param_2[0x34],(long)iVar12 << 3)
              ;
            }
            (**(code **)(*plVar43 + 8))(*plVar43 + 0x20,param_2[0x34]);
            param_2[0x34] = 0;
            uVar3 = *(ushort *)((long)puVar13 + 0x19);
            uVar6 = *(ushort *)((long)param_2 + 0x4c) & 1;
            *(ushort *)((long)puVar13 + 0x19) = uVar3 & 0xfffe | uVar6;
            uVar6 = uVar6 | (ushort)((*(ushort *)(param_2 + 10) & 1) << 1);
            *(ushort *)((long)puVar13 + 0x19) = uVar3 & 0xfffc | uVar6;
            *(undefined1 *)(puVar13 + 6) = *(undefined1 *)((long)param_2 + 0x86);
            uVar6 = uVar6 | (ushort)((*(ushort *)(param_2 + 0xf) & 1) << 2);
            *(ushort *)((long)puVar13 + 0x19) = uVar3 & 0xfff8 | uVar6;
            uVar7 = (ushort)((*(ushort *)((long)param_2 + 0x84) & 3) << 4);
            *(ushort *)((long)puVar13 + 0x19) = uVar3 & 0xffc0 | uVar3 & 8 | uVar6 | uVar7;
            if (*(int *)(param_2 + 0x1b) < 0) {
              uVar24 = (ushort)(*(int *)((long)param_2 + 0xdc) != 0) << 3;
            }
            else {
              uVar24 = 8;
            }
            *(ushort *)((long)puVar13 + 0x19) = uVar24 | uVar3 & 0xffc0 | uVar6 | uVar7;
            uVar6 = uVar24 | uVar6 | uVar7 | (ushort)((*(ushort *)(param_2 + 0xd) & 1) << 6);
            *(ushort *)((long)puVar13 + 0x19) = uVar3 & 0xff80 | uVar6;
            uVar6 = uVar6 | (ushort)((*(ushort *)((long)param_2 + 0x6c) & 1) << 7);
            *(ushort *)((long)puVar13 + 0x19) = uVar3 & 0xff00 | uVar6;
            uVar6 = uVar6 | (ushort)((*(ushort *)(param_2 + 0xe) & 1) << 8);
            *(ushort *)((long)puVar13 + 0x19) = uVar3 & 0xfe00 | uVar6;
            uVar6 = uVar6 | (ushort)((*(ushort *)((long)param_2 + 0x74) & 1) << 9);
            *(ushort *)((long)puVar13 + 0x19) = uVar3 & 0xfc00 | uVar6;
            uVar6 = uVar3 & 0x400 | uVar6 | (ushort)((*(ushort *)(param_2 + 0x10) & 1) << 0xb);
            *(ushort *)((long)puVar13 + 0x19) = uVar3 & 0xf000 | uVar6;
            *(ushort *)((long)puVar13 + 0x19) =
                 uVar3 & 0xd000 | uVar6 |
                 (ushort)((*(uint *)((long)param_2 + 0x3c) & 0xfffffffe) == 2) << 0xd;
            lVar31 = *(long *)(param_1 + 6);
            *param_1 = *param_1 + 1;
            *(undefined1 *)(puVar13 + 1) = 1;
            plVar30 = (long *)(lVar31 + 0x88);
            lVar31 = *plVar30;
            plVar23 = (long *)(puVar13 + 2);
            *plVar23 = lVar31;
            *(int **)(puVar13 + 0x12) = param_1;
            *(long **)(lVar31 + 8) = plVar23;
            *(long **)(puVar13 + 4) = plVar30;
            *plVar30 = (long)plVar23;
            if (param_2[1] != 0) {
              lVar31 = param_2[5];
              plVar23 = (long *)param_2[6];
              *(long **)(lVar31 + 8) = plVar23;
              *plVar23 = lVar31;
              param_2[5] = 0;
              param_2[6] = 0;
            }
            (**(code **)(*plVar43 + 8))(*plVar43 + 0x20,param_2);
            uVar15 = 0xfffffffffffffffe;
            local_f0 = puVar13;
            goto LAB_001955d0;
          }
        }
      }
    }
    else {
      lVar31 = *plVar43;
      if ((*(ushort *)(lVar31 + 0xf0) & 0xff) == 0) {
        *(ushort *)(lVar31 + 0xf0) = *(ushort *)(lVar31 + 0xf0) & 0xff00 | 1;
        FUN_001405ec(param_1,"out of memory");
        *(ushort *)(lVar31 + 0xf0) = (ushort)*(byte *)(lVar31 + 0xf1) << 8;
      }
    }
LAB_001955b8:
    FUN_00192f38(param_1,param_2);
    uVar15 = 6;
    local_f0 = (undefined4 *)((ulong)local_f0 & 0xffffffff00000000);
LAB_001955d0:
    if (*(long *)(lVar5 + 0x28) == local_70) {
      return;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(local_f0,uVar15);
  }
  plVar30 = param_2 + 0x2e;
  local_f8 = 0;
  local_114 = 0;
LAB_001943a4:
  pbVar14 = (byte *)(lVar31 + local_f8);
  bVar38 = *pbVar14;
  bVar18 = (&DAT_00113f23)[(ulong)bVar38 * 4];
  iVar12 = local_f8 + (uint)bVar18;
  switch((uint)bVar38) {
  case 0x11:
    iVar10 = FUN_001ad3a4(&local_e8,iVar12,0x6b6a,0xe,0xffffffff);
    uVar11 = local_cc;
    uVar20 = local_d4;
    uVar39 = local_d8;
    iVar42 = local_dc;
    if (iVar10 != 0) {
      if (((int)local_cc < 0) || (*(int *)((long)param_2 + 0x17c) <= (int)local_cc)) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0x7ced,"int resolve_variables(JSContext *, JSFunctionDef *)",
                  "lab1 >= 0 && lab1 < s->label_count");
      }
      do {
        iVar10 = 0;
        uVar32 = local_cc;
        do {
          uVar21 = (ulong)*(int *)(*plVar30 + (long)(int)uVar32 * 0x18 + 4);
          puVar37 = (uint *)(*plVar23 + 1 + uVar21);
          while( true ) {
            cVar2 = *(char *)((long)puVar37 + -1);
            if ((cVar2 != -0x48) && (cVar2 != -0x38)) break;
            uVar21 = (ulong)((int)uVar21 + 5);
            puVar37 = (uint *)((long)puVar37 + 5);
          }
          if (cVar2 != 'l') break;
          uVar32 = *puVar37;
          iVar10 = iVar10 + 1;
        } while (iVar10 != 0x14);
        iVar10 = FUN_001ad3a4(&local_e8,uVar21 & 0xffffffff,0x11,uVar20,0xe,0xffffffff);
      } while (iVar10 != 0);
      iVar10 = FUN_001ad3a4(&local_e8,uVar21 & 0xffffffff,uVar20,0xffffffff);
      uVar32 = local_cc;
      if (iVar10 != 0) {
        iVar12 = *(int *)((long)param_2 + 0x17c);
        *(int *)(param_2 + 0x36) = *(int *)(param_2 + 0x36) + 1;
        if (iVar12 <= (int)uVar11) {
code_r0x001960b4:
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x5536,"int update_label(JSFunctionDef *, int, int)",
                    "label >= 0 && label < s->label_count");
        }
        lVar40 = (ulong)uVar11 * 0x18;
        lVar29 = *plVar30;
        iVar19 = *(int *)(lVar29 + lVar40);
        *(int *)(lVar29 + lVar40) = iVar19 + -1;
        if (iVar19 < 1) {
code_r0x001960f4:
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x5539,"int update_label(JSFunctionDef *, int, int)","ls->ref_count >= 0");
        }
        if (((int)local_cc < 0) || (iVar12 <= (int)local_cc)) goto code_r0x001960b4;
        iVar12 = *(int *)(lVar29 + (ulong)local_cc * 0x18);
        *(int *)(lVar29 + (ulong)local_cc * 0x18) = iVar12 + 1;
        if (iVar12 == -2 || iVar12 + 2 < 0 != SCARRY4(iVar12,2)) goto code_r0x001960f4;
        FUN_001d27c4(&local_b0,uVar20);
        local_b4 = uVar32;
        FUN_001d26dc(&local_b0,&local_b4,4);
        iVar12 = iVar42;
        if ((uVar39 != 0xffffffff) && (uVar39 != local_114)) {
          *(int *)(param_2 + 0x38) = *(int *)(param_2 + 0x38) + 1;
          FUN_001d27c4(&local_b0,200);
          local_b4 = uVar39;
          FUN_001d26dc(&local_b0,&local_b4,4);
          local_114 = uVar39;
        }
      }
      goto joined_r0x001945ec;
    }
  default:
    goto switchD_001943dc_caseD_12;
  case 0x16:
    iVar10 = FUN_001ad3a4(&local_e8,iVar12,0x3d49,0xe,0xffffffff);
    if (iVar10 == 0) goto switchD_001943dc_caseD_12;
    FUN_001d27c4(&local_b0,local_d4);
    uVar39 = local_d8;
    local_f8 = local_dc;
    iVar12 = local_dc;
    if ((local_d8 != 0xffffffff) && (iVar12 = local_dc, local_d8 != local_114)) {
      *(int *)(param_2 + 0x38) = *(int *)(param_2 + 0x38) + 1;
      FUN_001d27c4(&local_b0,200);
      local_b4 = uVar39;
      FUN_001d26dc(&local_b0,&local_b4,4);
      local_114 = uVar39;
      iVar12 = local_f8;
    }
    break;
  case 0x23:
  case 0x25:
  case 0x28:
  case 0x29:
  case 0x2f:
  case 0x30:
  case 0x6f:
    goto switchD_001943dc_caseD_23;
  case 0x31:
    uVar6 = *(ushort *)(pbVar14 + 3);
    uVar35 = *(undefined2 *)(pbVar14 + 1);
    uVar39 = *(uint *)(param_2[0x1e] + (ulong)uVar6 * 8 + 4);
    while (-1 < (int)uVar39) {
      lVar29 = param_2[0x12] + (ulong)uVar39 * 0x10;
      uVar39 = *(uint *)(lVar29 + 8);
      *(uint *)(lVar29 + 0xc) = *(uint *)(lVar29 + 0xc) | 4;
    }
    FUN_001d27c4(&local_b0,bVar38);
    local_b4._0_2_ = uVar35;
    FUN_001d26dc(&local_b0,&local_b4,2);
    local_b4 = CONCAT22(local_b4._2_2_,*(short *)(param_2[0x1e] + (ulong)uVar6 * 8 + 4) + 1);
    FUN_001d26dc(&local_b0,&local_b4,2);
    break;
  case 0x32:
    uVar6 = *(ushort *)(pbVar14 + 1);
    uVar39 = *(uint *)(param_2[0x1e] + (ulong)uVar6 * 8 + 4);
    while (-1 < (int)uVar39) {
      lVar29 = param_2[0x12] + (ulong)uVar39 * 0x10;
      uVar39 = *(uint *)(lVar29 + 8);
      *(uint *)(lVar29 + 0xc) = *(uint *)(lVar29 + 0xc) | 4;
    }
    FUN_001d27c4(&local_b0,bVar38);
    local_b4 = CONCAT22(local_b4._2_2_,*(short *)(param_2[0x1e] + (ulong)uVar6 * 8 + 4) + 1);
    FUN_001d26dc(&local_b0,&local_b4,2);
    break;
  case 0x4d:
    iVar10 = *(int *)(pbVar14 + 1);
joined_r0x001945ec:
    if (iVar10 != 0) goto switchD_001943dc_caseD_12;
    break;
  case 0x6a:
  case 0x6b:
  case 0x6d:
    *(int *)(param_2 + 0x36) = *(int *)(param_2 + 0x36) + 1;
    goto switchD_001943dc_caseD_12;
  case 0x6c:
    *(int *)(param_2 + 0x36) = *(int *)(param_2 + 0x36) + 1;
switchD_001943dc_caseD_23:
    local_b8 = 0xffffffff;
    FUN_001d26dc(&local_b0,pbVar14,bVar18);
    iVar12 = FUN_001ad670(param_2,lVar31,uVar25,iVar12,&local_b8);
    uVar39 = local_b8;
    if (((iVar12 < (int)uVar25) && (-1 < (int)local_b8)) && (local_114 != local_b8)) {
      *(int *)(param_2 + 0x38) = *(int *)(param_2 + 0x38) + 1;
      FUN_001d27c4(&local_b0,200);
      local_b4 = uVar39;
      FUN_001d26dc(&local_b0,&local_b4,4);
      local_114 = uVar39;
    }
    break;
  case 0x6e:
    *(int *)(param_2 + 0x36) = *(int *)(param_2 + 0x36) + 1;
    uVar39 = *(uint *)(pbVar14 + 1);
    if (((int)uVar39 < 0) || (*(int *)((long)param_2 + 0x17c) <= (int)uVar39)) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x7c4c,"int resolve_variables(JSContext *, JSFunctionDef *)",
                "label >= 0 && label < s->label_count");
    }
    lVar29 = *plVar30;
    iVar10 = FUN_001ad3a4(&local_e8,*(undefined4 *)(lVar29 + (ulong)uVar39 * 0x18 + 4),0x6f,
                          0xffffffff);
    if (iVar10 == 0) goto switchD_001943dc_caseD_12;
    *(int *)(lVar29 + (ulong)uVar39 * 0x18) = *(int *)(lVar29 + (ulong)uVar39 * 0x18) + -1;
    break;
  case 0xb5:
  case 199:
    break;
  case 0xb6:
    uVar6 = *(ushort *)(pbVar14 + 1);
    if (*(uint *)(param_2 + 0x23) == (uint)uVar6) {
      if (0 < *(int *)((long)param_2 + 0xac)) {
        lVar29 = 0;
        lVar40 = 0xc;
        do {
          lVar41 = param_2[0x14];
          if (-1 < *(int *)(lVar41 + lVar40)) {
            FUN_001d27c4(&local_b0,3);
            local_b4 = *(int *)(lVar41 + lVar40) >> 8;
            FUN_001d26dc(&local_b0,&local_b4,4);
            FUN_001d27c4(&local_b0,0x5c);
            local_b4 = CONCAT22(local_b4._2_2_,(short)lVar29);
            FUN_001d26dc(&local_b0,&local_b4,2);
          }
          lVar29 = lVar29 + 1;
          lVar40 = lVar40 + 0x10;
        } while (lVar29 < *(int *)((long)param_2 + 0xac));
      }
      if (0 < *piVar17) {
        lVar40 = 0;
        lVar29 = 0;
        do {
          lVar41 = param_2[0x12] + lVar40;
          if ((*(int *)(lVar41 + 4) == 0) && (-1 < *(int *)(lVar41 + 0xc))) {
            FUN_001d27c4(&local_b0,3);
            local_b4 = *(int *)(lVar41 + 0xc) >> 8;
            FUN_001d26dc(&local_b0,&local_b4,4);
            FUN_001d27c4(&local_b0,0x59);
            local_b4 = CONCAT22(local_b4._2_2_,(short)lVar29);
            FUN_001d26dc(&local_b0,&local_b4,2);
          }
          lVar29 = lVar29 + 1;
          lVar40 = lVar40 + 0x10;
        } while (lVar29 < *piVar17);
      }
      if (param_2[0x43] == 0) {
        uVar39 = 0xffffffff;
      }
      else {
        if ((*(int *)((long)param_2 + 0x17c) < *(int *)(param_2 + 0x2f)) ||
           (iVar10 = FUN_0016cbcc(*param_2,plVar30,0x18,param_2 + 0x2f,
                                  *(int *)((long)param_2 + 0x17c) + 1), iVar10 == 0)) {
          uVar39 = *(uint *)((long)param_2 + 0x17c);
          puVar22 = (undefined8 *)(param_2[0x2e] + (long)(int)uVar39 * 0x18);
          *(uint *)((long)param_2 + 0x17c) = uVar39 + 1;
          puVar22[2] = 0;
          *puVar22 = 0xffffffff00000000;
          puVar22[1] = 0xffffffffffffffff;
        }
        else {
          uVar39 = 0xffffffff;
        }
        FUN_001d27c4(&local_b0,8);
        FUN_001d27c4(&local_b0,0x6a);
        local_b4 = uVar39;
        FUN_001d26dc(&local_b0,&local_b4,4);
        if (((int)uVar39 < 0) || (*(int *)((long)param_2 + 0x17c) <= (int)uVar39))
        goto code_r0x001960b4;
        iVar10 = *(int *)(*plVar30 + (ulong)uVar39 * 0x18);
        *(int *)(*plVar30 + (ulong)uVar39 * 0x18) = iVar10 + 1;
        if (iVar10 == -2 || iVar10 + 2 < 0 != SCARRY4(iVar10,2)) goto code_r0x001960f4;
        *(int *)(param_2 + 0x36) = *(int *)(param_2 + 0x36) + 1;
      }
      if (0 < *(int *)((long)param_2 + 0x11c)) {
        lVar29 = 0;
        do {
          lVar40 = param_2[0x25];
          puVar37 = (uint *)(lVar40 + lVar29 * 0x10);
          puVar33 = puVar37 + 1;
          bVar38 = (byte)*puVar33 & 1;
          if (*(int *)(param_2 + 0x33) < 1) {
            uVar35 = 0;
            iVar10 = 0;
LAB_00195018:
            bVar9 = *(int *)((long)param_2 + 0x3c) != 0;
            if (((int)*puVar37 < 0) || (((byte)*puVar33 >> 1 & 1) != 0)) {
              uVar32 = *puVar33;
              uVar11 = 0xffffff82;
              if (((byte)uVar32 & 4) != 0) {
                uVar11 = 0xffffff80;
              }
              FUN_001d27c4(&local_b0,0x3e);
              local_b4 = *(uint *)(lVar40 + lVar29 * 0x10 + 0xc);
              if (0xe3 < (int)local_b4) {
                piVar26 = *(int **)(*(long *)(*plVar43 + 0x60) + (ulong)local_b4 * 8);
                *piVar26 = *piVar26 + 1;
              }
              FUN_001d26dc(&local_b0,&local_b4,4);
              FUN_001d27c4(&local_b0,
                           uVar11 & (int)((uint)(byte)uVar32 << 0x1e) >> 0x1f | (uint)bVar9);
              goto LAB_00195140;
            }
            FUN_001d27c4(&local_b0,3);
            local_b4 = *puVar37;
            FUN_001d26dc(&local_b0,&local_b4,4);
            FUN_001d27c4(&local_b0,0x40);
            local_b4 = *(uint *)(lVar40 + lVar29 * 0x10 + 0xc);
            if (0xe3 < (int)local_b4) {
              piVar26 = *(int **)(*(long *)(*plVar43 + 0x60) + (ulong)local_b4 * 8);
              *piVar26 = *piVar26 + 1;
            }
            FUN_001d26dc(&local_b0,&local_b4,4);
            FUN_001d27c4(&local_b0,bVar9);
          }
          else {
            lVar41 = 0;
            iVar10 = 0;
            lVar36 = 4;
            do {
              if (*(uint *)(param_2[0x34] + lVar36) == puVar37[3]) {
                bVar38 = 0;
                iVar10 = 2;
                break;
              }
              if ((*(uint *)(param_2[0x34] + lVar36) & 0xfffffffe) == 0x54) {
                FUN_001d27c4(&local_b0,0x5e);
                local_b4 = CONCAT22(local_b4._2_2_,(short)lVar41);
                FUN_001d26dc(&local_b0,&local_b4,2);
                iVar10 = 1;
                bVar38 = 1;
                break;
              }
              lVar41 = lVar41 + 1;
              lVar36 = lVar36 + 8;
            } while (lVar41 < *(int *)(param_2 + 0x33));
            uVar35 = (undefined2)lVar41;
            if (iVar10 == 0) goto LAB_00195018;
LAB_00195140:
            if ((-1 < (int)*puVar37) || (bVar38 != 0)) {
              if ((int)*puVar37 < 0) {
                FUN_001d27c4(&local_b0,6);
              }
              else {
                FUN_001d27c4(&local_b0,3);
                local_b4 = *puVar37;
                FUN_001d26dc(&local_b0,&local_b4,4);
                if (*(int *)(lVar40 + lVar29 * 0x10 + 0xc) == 0x7e) {
                  FUN_001d27c4(&local_b0,0x4d);
                  local_b4 = 0x16;
                  FUN_001d26dc(&local_b0,&local_b4,4);
                }
              }
              if (iVar10 == 1) {
                FUN_001d27c4(&local_b0,0x4c);
                local_b4 = *(uint *)(lVar40 + lVar29 * 0x10 + 0xc);
                if (0xe3 < (int)local_b4) {
                  piVar26 = *(int **)(*(long *)(*plVar43 + 0x60) + (ulong)local_b4 * 8);
                  *piVar26 = *piVar26 + 1;
                }
                FUN_001d26dc(&local_b0,&local_b4,4);
                FUN_001d27c4(&local_b0,0xe);
              }
              else {
                if (iVar10 == 2) {
                  FUN_001d27c4(&local_b0,0x5f);
                  local_b4 = CONCAT22(local_b4._2_2_,uVar35);
                  uVar15 = 2;
                }
                else {
                  FUN_001d27c4(&local_b0,0x39);
                  local_b4 = *(uint *)(lVar40 + lVar29 * 0x10 + 0xc);
                  if (0xe3 < (int)local_b4) {
                    piVar26 = *(int **)(*(long *)(*plVar43 + 0x60) + (ulong)local_b4 * 8);
                    *piVar26 = *piVar26 + 1;
                  }
                  uVar15 = 4;
                }
                FUN_001d26dc(&local_b0,&local_b4,uVar15);
              }
            }
          }
          uVar11 = *(uint *)(lVar40 + lVar29 * 0x10 + 0xc);
          if ((0xe3 < (int)uVar11) &&
             (piVar26 = *(int **)(*(long *)(*plVar43 + 0x60) + (ulong)uVar11 * 8), iVar10 = *piVar26
             , iVar42 = iVar10 + -1, *piVar26 = iVar42, iVar42 == 0 || iVar10 < 1)) {
            FUN_001418dc();
          }
          lVar29 = lVar29 + 1;
        } while (lVar29 < *(int *)((long)param_2 + 0x11c));
      }
      if (param_2[0x43] != 0) {
        FUN_001d27c4(&local_b0,0x29);
        FUN_001d27c4(&local_b0,0xb8);
        local_b4 = uVar39;
        FUN_001d26dc(&local_b0,&local_b4,4);
        *(int *)(*plVar30 + (long)(int)uVar39 * 0x18 + 8) = (int)pvStack_a8;
      }
      (**(code **)(*plVar43 + 8))(*plVar43 + 0x20,param_2[0x25]);
      param_2[0x25] = 0;
      *(undefined8 *)((long)param_2 + 0x11c) = 0;
    }
    uVar39 = (uint)uVar6;
    uVar21 = (ulong)*(uint *)(param_2[0x1e] + (ulong)uVar39 * 8 + 4);
    do {
      if ((int)uVar21 < 0) break;
      lVar29 = param_2[0x12];
      uVar11 = *(uint *)(lVar29 + uVar21 * 0x10 + 4);
      if (uVar11 == uVar39) {
        if (uVar21 != *(uint *)(param_2 + 0x18)) {
          puVar37 = (uint *)(lVar29 + uVar21 * 0x10 + 0xc);
          if ((*puVar37 >> 4 & 0xf) - 1 < 2) {
            FUN_001d27c4(&local_b0,3);
            local_b4 = (int)*puVar37 >> 8;
            FUN_001d26dc(&local_b0,&local_b4,4);
            uVar15 = 0x59;
          }
          else {
            uVar15 = 0x61;
          }
          FUN_001d27c4(&local_b0,uVar15);
          local_b4 = CONCAT22(local_b4._2_2_,(short)uVar21);
          FUN_001d26dc(&local_b0,&local_b4,2);
        }
        uVar21 = (ulong)*(uint *)(lVar29 + uVar21 * 0x10 + 8);
      }
    } while (uVar11 == uVar39);
    break;
  case 0xb7:
    uVar6 = *(ushort *)(pbVar14 + 1);
    uVar21 = (ulong)*(uint *)(param_2[0x1e] + (ulong)uVar6 * 8 + 4);
    do {
      if ((int)uVar21 < 0) break;
      lVar40 = param_2[0x12];
      lVar29 = lVar40 + uVar21 * 0x10;
      uVar39 = *(uint *)(lVar29 + 4);
      if (uVar39 == uVar6) {
        if ((*(byte *)(lVar29 + 0xc) >> 2 & 1) != 0) {
          FUN_001d27c4(&local_b0,0x69);
          local_b4 = CONCAT22(local_b4._2_2_,(short)uVar21);
          FUN_001d26dc(&local_b0,&local_b4,2);
        }
        uVar21 = (ulong)*(uint *)(lVar40 + uVar21 * 0x10 + 8);
      }
    } while (uVar39 == uVar6);
    break;
  case 0xb8:
    uVar39 = *(uint *)(pbVar14 + 1);
    if (((int)uVar39 < 0) || (*(int *)((long)param_2 + 0x17c) <= (int)uVar39)) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x7c9c,"int resolve_variables(JSContext *, JSFunctionDef *)",
                "label >= 0 && label < s->label_count");
    }
    *(uint *)(*plVar30 + (ulong)uVar39 * 0x18 + 8) = (int)pvStack_a8 + (uint)bVar18;
    goto switchD_001943dc_caseD_12;
  case 0xb9:
  case 0xba:
  case 0xbb:
  case 0xbc:
  case 0xbe:
  case 0xbf:
  case 0xc0:
    uVar39 = *(uint *)(pbVar14 + 1);
    uVar35 = *(undefined2 *)(pbVar14 + 5);
    piVar26 = (int *)0x0;
    lVar29 = 0;
    goto LAB_00194488;
  case 0xbd:
    uVar39 = *(uint *)(pbVar14 + 1);
    uVar35 = *(undefined2 *)(pbVar14 + 9);
    piVar26 = (int *)(param_2[0x2e] + (long)*(int *)(pbVar14 + 5) * 0x18);
    bVar38 = 0xbd;
    *piVar26 = *piVar26 + -1;
    lVar29 = lVar31;
LAB_00194488:
    iVar12 = FUN_001ac2e4(param_1,param_2,(ulong)uVar39,uVar35,bVar38,&local_b0,lVar29,piVar26,
                          iVar12);
    if (0xe3 < (int)uVar39) {
      piVar26 = *(int **)(*(long *)(*plVar43 + 0x60) + (ulong)uVar39 * 8);
LAB_00194f00:
      iVar10 = *piVar26;
      iVar42 = iVar10 + -1;
      *piVar26 = iVar42;
      if (iVar42 == 0 || iVar10 < 1) {
        FUN_001418dc();
      }
    }
    break;
  case 0xc1:
  case 0xc2:
  case 0xc3:
  case 0xc4:
    uVar39 = *(uint *)(pbVar14 + 1);
    uVar21 = (ulong)uVar39;
    uVar35 = *(undefined2 *)(pbVar14 + 5);
    iVar10 = FUN_001ae048(param_1,&local_bc,&local_b8,param_2,uVar21,uVar35);
    if (iVar10 < 0) goto joined_r0x00195660;
    if (local_b8 == 0) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x7923,
                "int resolve_scope_private_field(JSContext *, JSFunctionDef *, JSAtom, int, int, DynBuf *)"
                ,"var_kind != JS_VAR_NORMAL");
    }
    uVar4 = (undefined2)iVar10;
    if (bVar38 - 0xc1 < 2) {
      switch(local_b8) {
      case 5:
        if (bVar38 == 0xc2) {
          FUN_001d27c4(&local_b0,0x11);
        }
        uVar20 = 0x58;
        if (local_bc != 0) {
          uVar20 = 0x5e;
        }
        FUN_001d27c4(&local_b0,uVar20);
        local_b4 = CONCAT22(local_b4._2_2_,uVar4);
        FUN_001d26dc(&local_b0,&local_b4,2);
        uVar15 = 0x44;
        goto LAB_00194ee4;
      case 6:
        uVar20 = 0x58;
        if (local_bc != 0) {
          uVar20 = 0x5e;
        }
        FUN_001d27c4(&local_b0,uVar20);
        local_b4 = CONCAT22(local_b4._2_2_,uVar4);
        FUN_001d26dc(&local_b0,&local_b4,2);
        FUN_001d27c4(&local_b0,0x2c);
        if (bVar38 != 0xc2) {
          uVar15 = 0xf;
          goto LAB_00194ee4;
        }
        break;
      case 7:
      case 9:
        if (bVar38 == 0xc2) {
          FUN_001d27c4(&local_b0,0x11);
        }
        uVar20 = 0x58;
        if (local_bc != 0) {
          uVar20 = 0x5e;
        }
        FUN_001d27c4(&local_b0,uVar20);
        local_b4._0_2_ = uVar4;
        FUN_001d26dc(&local_b0,&local_b4,2);
        FUN_001d27c4(&local_b0,0x2c);
        FUN_001d27c4(&local_b0,0x24);
        local_b4 = (uint)local_b4._2_2_ << 0x10;
        FUN_001d26dc(&local_b0,&local_b4,2);
        break;
      case 8:
switchD_00194570_caseD_8:
        FUN_001d27c4(&local_b0,0x30);
        if (0xe3 < (int)uVar39) {
          piVar26 = *(int **)(*(long *)(*plVar43 + 0x60) + uVar21 * 8);
          *piVar26 = *piVar26 + 1;
        }
        local_b4 = uVar39;
        FUN_001d26dc(&local_b0,&local_b4,4);
        uVar15 = 0;
        goto LAB_00194ee4;
      default:
                    /* WARNING: Subroutine does not return */
        abort();
      }
    }
    else {
      if (bVar38 == 0xc3) {
        if (local_b8 - 6 < 2) goto switchD_00194570_caseD_8;
        if (1 < local_b8 - 8) {
          if (local_b8 != 5) {
                    /* WARNING: Subroutine does not return */
            abort();
          }
          uVar20 = 0x58;
          if (local_bc != 0) {
            uVar20 = 0x5e;
          }
          FUN_001d27c4(&local_b0,uVar20);
          local_b4 = CONCAT22(local_b4._2_2_,uVar4);
          FUN_001d26dc(&local_b0,&local_b4,2);
          uVar15 = 0x45;
          goto LAB_00194ee4;
        }
        uVar11 = FUN_001a5b50(param_1,uVar21,"<set>");
        if (uVar11 == 0) goto joined_r0x00195660;
        iVar10 = FUN_001ae048(param_1,&local_bc,&local_b8,param_2,(ulong)uVar11,uVar35);
        if ((0xe3 < (int)uVar11) &&
           (piVar26 = *(int **)(*(long *)(*plVar43 + 0x60) + (ulong)uVar11 * 8), iVar42 = *piVar26,
           iVar19 = iVar42 + -1, *piVar26 = iVar19, iVar19 == 0 || iVar42 < 1)) {
          FUN_001418dc();
        }
        if (iVar10 < 0) goto joined_r0x00195660;
        if (local_b8 != 8) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x7960,
                    "int resolve_scope_private_field(JSContext *, JSFunctionDef *, JSAtom, int, int, DynBuf *)"
                    ,"var_kind == JS_VAR_PRIVATE_SETTER");
        }
        uVar20 = 0x58;
        if (local_bc != 0) {
          uVar20 = 0x5e;
        }
        FUN_001d27c4(&local_b0,uVar20);
        local_b4._0_2_ = (short)iVar10;
        FUN_001d26dc(&local_b0,&local_b4,2);
        FUN_001d27c4(&local_b0,0x1b);
        FUN_001d27c4(&local_b0,0x1e);
        FUN_001d27c4(&local_b0,0x2c);
        FUN_001d27c4(&local_b0,0x1d);
        FUN_001d27c4(&local_b0,0x24);
        local_b4 = CONCAT22(local_b4._2_2_,1);
        FUN_001d26dc(&local_b0,&local_b4,2);
        uVar15 = 0xe;
      }
      else {
        if (bVar38 != 0xc4) {
                    /* WARNING: Subroutine does not return */
          abort();
        }
        uVar20 = 0x58;
        if (local_bc != 0) {
          uVar20 = 0x5e;
        }
        FUN_001d27c4(&local_b0,uVar20);
        local_b4 = CONCAT22(local_b4._2_2_,uVar4);
        FUN_001d26dc(&local_b0,&local_b4,2);
        uVar15 = 0xb2;
      }
LAB_00194ee4:
      FUN_001d27c4(&local_b0,uVar15);
    }
    if ((int)uVar39 < 0xe4) break;
    piVar26 = *(int **)(*(long *)(*plVar43 + 0x60) + uVar21 * 8);
    goto LAB_00194f00;
  case 0xc5:
    uVar39 = *(uint *)(pbVar14 + 1);
    FUN_001d27c4(&local_b0,0x41);
    pbVar14 = (byte *)&local_b4;
    bVar18 = 4;
    local_b4 = uVar39;
    goto LAB_00195424;
  case 0xc6:
    FUN_001d27c4(&local_b0,0x47);
    break;
  case 200:
    local_114 = *(uint *)(pbVar14 + 1);
    *(int *)(param_2 + 0x38) = *(int *)(param_2 + 0x38) + 1;
switchD_001943dc_caseD_12:
LAB_00195424:
    FUN_001d26dc(&local_b0,pbVar14,bVar18);
  }
  local_f8 = iVar12;
  if ((int)uVar25 <= local_f8) goto LAB_00195450;
  goto LAB_001943a4;
joined_r0x00195660:
  for (; local_f8 < (int)uVar25; local_f8 = local_f8 + (uint)bVar38) {
    bVar38 = (&DAT_00113f23)[(ulong)*(byte *)(lVar31 + local_f8) * 4];
    FUN_001d26dc(&local_b0);
  }
  FUN_001d2b08(plVar23);
  param_2[0x27] = pvStack_a8;
  *plVar23 = CONCAT44(uStack_ac,local_b0);
  param_2[0x29] = lStack_98;
  param_2[0x28] = local_a0;
  param_2[0x2b] = uStack_88;
  param_2[0x2a] = local_90;
  goto LAB_001955b8;
}

/* ===== FUN_0019e5f4 @ 0019e5f4 [libNexusScriptRuntime69252.so] ===== */

void FUN_0019e5f4(long *param_1,uint param_2)

{
  char *pcVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  int *piVar6;
  long lVar7;
  char cVar8;
  long lVar9;
  long lVar10;
  uint local_4c;
  long local_48;
  
  lVar5 = tpidr_el0;
  local_48 = *(long *)(lVar5 + 0x28);
  lVar10 = param_1[0xd];
  uVar2 = *(uint *)(lVar10 + 0x160);
  if ((int)uVar2 < 0) {
    cVar8 = '\0';
  }
  else {
    cVar8 = *(char *)(*(long *)(lVar10 + 0x130) + (ulong)uVar2);
  }
  if (cVar8 == -0x39) {
    lVar9 = (long)(int)((uVar2 - *(int *)(*(long *)(lVar10 + 0x130) + (long)(int)uVar2 + 1)) + 1);
    pcVar1 = (char *)(*(long *)(lVar10 + 0x130) + lVar9);
    if (*pcVar1 != 'V') {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x594a,"void set_object_name(JSParseState *, JSAtom)",
                "fd->byte_code.buf[define_class_pos] == OP_define_class");
    }
    uVar2 = *(uint *)(pcVar1 + 1);
    if (0xe3 < (int)uVar2) {
      piVar6 = *(int **)(*(long *)(*(long *)(*param_1 + 0x18) + 0x60) + (ulong)uVar2 * 8);
      iVar3 = *piVar6;
      iVar4 = iVar3 + -1;
      *piVar6 = iVar4;
      if (iVar4 == 0 || iVar3 < 1) {
        FUN_001418dc();
      }
    }
    lVar7 = *(long *)(lVar10 + 0x130);
    if (0xe3 < (int)param_2) {
      piVar6 = *(int **)(*(long *)(*(long *)(*param_1 + 0x18) + 0x60) + (ulong)param_2 * 8);
      *piVar6 = *piVar6 + 1;
    }
    *(uint *)(lVar7 + lVar9 + 1) = param_2;
    *(undefined4 *)(lVar10 + 0x160) = 0xffffffff;
  }
  else if (cVar8 == 'M') {
    *(long *)(lVar10 + 0x138) = (long)(int)uVar2;
    lVar9 = lVar10 + 0x130;
    *(undefined4 *)(lVar10 + 0x160) = 0xffffffff;
    if (*(int *)(lVar10 + 0x164) != (int)param_1[1]) {
      FUN_001d27c4(lVar9,200);
      local_4c = *(uint *)(param_1 + 1);
      FUN_001d26dc(lVar9,&local_4c,4);
      *(int *)(lVar10 + 0x164) = (int)param_1[1];
    }
    *(int *)(lVar10 + 0x160) = (int)*(undefined8 *)(lVar10 + 0x138);
    FUN_001d27c4(lVar9,0x4d);
    if (0xe3 < (int)param_2) {
      piVar6 = *(int **)(*(long *)(*(long *)(*param_1 + 0x18) + 0x60) + (ulong)param_2 * 8);
      *piVar6 = *piVar6 + 1;
    }
    local_4c = param_2;
    FUN_001d26dc(param_1[0xd] + 0x130,&local_4c,4);
  }
  if (*(long *)(lVar5 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0019f3c0 @ 0019f3c0 [libNexusScriptRuntime69252.so] ===== */

void FUN_0019f3c0(long *param_1,uint *param_2,uint *param_3,uint *param_4,uint *param_5,
                 undefined4 *param_6,int param_7,uint param_8)

{
  undefined2 uVar1;
  long lVar2;
  int iVar3;
  undefined8 uVar4;
  char *pcVar5;
  undefined4 uVar6;
  long lVar7;
  undefined8 *puVar8;
  int *piVar9;
  undefined8 *puVar10;
  undefined4 uVar11;
  uint uVar12;
  uint uVar13;
  long lVar14;
  uint uVar15;
  uint local_70;
  uint local_6c;
  long local_68;
  
  lVar2 = tpidr_el0;
  local_68 = *(long *)(lVar2 + 0x28);
  puVar10 = (undefined8 *)param_1[0xd];
  uVar12 = *(uint *)(puVar10 + 0x2c);
  if ((int)uVar12 < 0) {
    uVar13 = 0;
  }
  else {
    uVar13 = (uint)*(byte *)(puVar10[0x26] + (ulong)uVar12);
  }
  lVar7 = (long)(int)uVar12;
  if (uVar13 < 0x4a) {
    if (uVar13 == 0x41) {
      local_70 = 0;
      uVar15 = *(uint *)(puVar10[0x26] + lVar7 + 1);
LAB_0019f4cc:
      uVar11 = 1;
    }
    else {
      if (uVar13 != 0x47) goto LAB_0019f9ec;
      local_70 = 0;
      uVar15 = 0;
      uVar11 = 2;
    }
  }
  else {
    if (uVar13 != 0x4a) {
      if (uVar13 == 0xc1) {
        uVar15 = *(uint *)(puVar10[0x26] + (long)(int)uVar12 + 1);
        local_70 = (uint)*(ushort *)(puVar10[0x26] + (long)(int)uVar12 + 5);
        goto LAB_0019f4cc;
      }
      if (uVar13 == 0xba) {
        uVar11 = 2;
        uVar15 = *(uint *)(puVar10[0x26] + lVar7 + 1);
        local_70 = (uint)*(ushort *)(puVar10[0x26] + lVar7 + 5);
        if (0x4e < (int)uVar15) {
          if (uVar15 != 0x73) {
            if (uVar15 == 0x4f) goto LAB_0019f9d4;
            goto LAB_0019f4d0;
          }
          goto LAB_0019f9ec;
        }
        if (uVar15 == 8) goto LAB_0019f9ec;
        if (uVar15 != 0x3c) goto LAB_0019f4d0;
LAB_0019f9d4:
        if ((*(byte *)((long)puVar10 + 0x86) & 1) == 0) {
          if ((uVar15 == 8) || (uVar15 == 0x73)) goto LAB_0019f9ec;
          goto LAB_0019f4d0;
        }
        pcVar5 = "invalid lvalue in strict mode";
      }
      else {
LAB_0019f9ec:
        if (param_8 == 0xffffffbd) {
          pcVar5 = "invalid for in/of left hand-side";
        }
        else if (param_8 + 0x6b < 2) {
          pcVar5 = "invalid increment/decrement operand";
        }
        else if ((param_8 & 0xffffffdf) == 0x5b) {
          pcVar5 = "invalid destructuring target";
        }
        else {
          pcVar5 = "invalid assignment left-hand side";
        }
      }
      FUN_0015846c(param_1,pcVar5);
      uVar4 = 0xffffffff;
      goto LAB_0019fb0c;
    }
    local_70 = 0;
    uVar15 = 0;
    uVar11 = 3;
  }
LAB_0019f4d0:
  puVar10[0x27] = lVar7;
  *(undefined4 *)(puVar10 + 0x2c) = 0xffffffff;
  uVar1 = (undefined2)local_70;
  if (param_7 == 0) {
    if (uVar13 != 0x47) {
      if (uVar13 == 0x4a) {
        puVar8 = puVar10 + 0x26;
        if (*(int *)((long)puVar10 + 0x164) != (int)param_1[1]) {
          FUN_001d27c4(puVar8,200);
          local_6c = *(uint *)(param_1 + 1);
          FUN_001d26dc(puVar8,&local_6c,4);
          *(int *)((long)puVar10 + 0x164) = (int)param_1[1];
        }
        *(int *)(puVar10 + 0x2c) = (int)puVar10[0x27];
        FUN_001d27c4(puVar8,0x72);
        uVar13 = 0x4a;
        goto LAB_0019fa9c;
      }
      if (uVar13 == 0xba) {
        if ((*(int *)((long)puVar10 + 0x17c) < *(int *)(puVar10 + 0x2f)) ||
           (iVar3 = FUN_0016cbcc(*puVar10,puVar10 + 0x2e,0x18,puVar10 + 0x2f,
                                 *(int *)((long)puVar10 + 0x17c) + 1), iVar3 == 0)) {
          uVar12 = *(uint *)((long)puVar10 + 0x17c);
          puVar8 = (undefined8 *)(puVar10[0x2e] + (long)(int)uVar12 * 0x18);
          *(uint *)((long)puVar10 + 0x17c) = uVar12 + 1;
          *puVar8 = 0xffffffff00000000;
          puVar8[1] = 0xffffffffffffffff;
          puVar8[2] = 0;
        }
        else {
          uVar12 = 0xffffffff;
        }
        lVar14 = param_1[0xd];
        lVar7 = lVar14 + 0x130;
        if (*(int *)(lVar14 + 0x164) != (int)param_1[1]) {
          FUN_001d27c4(lVar7,200);
          local_6c = *(uint *)(param_1 + 1);
          FUN_001d26dc(lVar7,&local_6c,4);
          *(int *)(lVar14 + 0x164) = (int)param_1[1];
        }
        *(int *)(lVar14 + 0x160) = (int)*(undefined8 *)(lVar14 + 0x138);
        FUN_001d27c4(lVar7,0xbd);
        if (0xe3 < (int)uVar15) {
          piVar9 = *(int **)(*(long *)(*(long *)(*param_1 + 0x18) + 0x60) + (ulong)uVar15 * 8);
          *piVar9 = *piVar9 + 1;
        }
        local_6c = uVar15;
        FUN_001d26dc(param_1[0xd] + 0x130,&local_6c,4);
        local_6c = uVar12;
        FUN_001d26dc(param_1[0xd] + 0x130,&local_6c,4);
        local_6c = CONCAT22(local_6c._2_2_,uVar1);
        FUN_001d26dc(param_1[0xd] + 0x130,&local_6c,2);
        if (((int)uVar12 < 0) || (*(int *)((long)puVar10 + 0x17c) <= (int)uVar12)) {
code_r0x0019fe00:
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x5536,"int update_label(JSFunctionDef *, int, int)",
                    "label >= 0 && label < s->label_count");
        }
        lVar7 = puVar10[0x2e];
        iVar3 = *(int *)(lVar7 + (ulong)uVar12 * 0x18);
        *(int *)(lVar7 + (ulong)uVar12 * 0x18) = iVar3 + 1;
        if (iVar3 == -2 || iVar3 + 2 < 0 != SCARRY4(iVar3,2)) {
code_r0x0019fe20:
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x5539,"int update_label(JSFunctionDef *, int, int)","ls->ref_count >= 0");
        }
        uVar13 = 0x3c;
        goto LAB_0019fab0;
      }
      goto LAB_0019f898;
    }
    puVar8 = puVar10 + 0x26;
    if (*(int *)((long)puVar10 + 0x164) != (int)param_1[1]) {
      FUN_001d27c4(puVar8,200);
      local_6c = *(uint *)(param_1 + 1);
      FUN_001d26dc(puVar8,&local_6c,4);
      *(int *)((long)puVar10 + 0x164) = (int)param_1[1];
    }
    *(int *)(puVar10 + 0x2c) = (int)puVar10[0x27];
    FUN_001d27c4(puVar8,0x73);
    uVar13 = 0x47;
LAB_0019fa9c:
    uVar12 = 0xffffffff;
  }
  else {
    if (uVar13 < 0x4a) {
      if (uVar13 == 0x41) {
        puVar8 = puVar10 + 0x26;
        if (*(int *)((long)puVar10 + 0x164) != (int)param_1[1]) {
          FUN_001d27c4(puVar8,200);
          local_6c = *(uint *)(param_1 + 1);
          FUN_001d26dc(puVar8,&local_6c,4);
          *(int *)((long)puVar10 + 0x164) = (int)param_1[1];
        }
        *(int *)(puVar10 + 0x2c) = (int)puVar10[0x27];
        FUN_001d27c4(puVar8,0x42);
        if (0xe3 < (int)uVar15) {
          piVar9 = *(int **)(*(long *)(*(long *)(*param_1 + 0x18) + 0x60) + (ulong)uVar15 * 8);
          *piVar9 = *piVar9 + 1;
        }
        local_6c = uVar15;
        FUN_001d26dc(param_1[0xd] + 0x130,&local_6c,4);
        uVar13 = 0x41;
        goto LAB_0019fa9c;
      }
      if (uVar13 != 0x47) {
LAB_0019fe3c:
                    /* WARNING: Subroutine does not return */
        abort();
      }
      puVar8 = puVar10 + 0x26;
      if (*(int *)((long)puVar10 + 0x164) != (int)param_1[1]) {
        FUN_001d27c4(puVar8,200);
        local_6c = *(uint *)(param_1 + 1);
        FUN_001d26dc(puVar8,&local_6c,4);
        *(int *)((long)puVar10 + 0x164) = (int)param_1[1];
      }
      *(int *)(puVar10 + 0x2c) = (int)puVar10[0x27];
      FUN_001d27c4(puVar8,0x73);
      lVar14 = param_1[0xd];
      lVar7 = lVar14 + 0x130;
      if (*(int *)(lVar14 + 0x164) != (int)param_1[1]) {
        FUN_001d27c4(lVar7,200);
        local_6c = *(uint *)(param_1 + 1);
        FUN_001d26dc(lVar7,&local_6c,4);
        *(int *)(lVar14 + 0x164) = (int)param_1[1];
      }
      *(int *)(lVar14 + 0x160) = (int)*(undefined8 *)(lVar14 + 0x138);
      FUN_001d27c4(lVar7,0x13);
      lVar14 = param_1[0xd];
      lVar7 = lVar14 + 0x130;
      if (*(int *)(lVar14 + 0x164) != (int)param_1[1]) {
        FUN_001d27c4(lVar7,200);
        local_6c = *(uint *)(param_1 + 1);
        FUN_001d26dc(lVar7,&local_6c,4);
        *(int *)(lVar14 + 0x164) = (int)param_1[1];
      }
      uVar6 = (undefined4)*(undefined8 *)(lVar14 + 0x138);
      uVar4 = 0x47;
      uVar13 = 0x47;
    }
    else {
      if (uVar13 != 0x4a) {
        if (uVar13 == 0xc1) {
          puVar8 = puVar10 + 0x26;
          if (*(int *)((long)puVar10 + 0x164) != (int)param_1[1]) {
            FUN_001d27c4(puVar8,200);
            local_6c = *(uint *)(param_1 + 1);
            FUN_001d26dc(puVar8,&local_6c,4);
            *(int *)((long)puVar10 + 0x164) = (int)param_1[1];
          }
          *(int *)(puVar10 + 0x2c) = (int)puVar10[0x27];
          FUN_001d27c4(puVar8,0xc2);
          if (0xe3 < (int)uVar15) {
            piVar9 = *(int **)(*(long *)(*(long *)(*param_1 + 0x18) + 0x60) + (ulong)uVar15 * 8);
            *piVar9 = *piVar9 + 1;
          }
          local_6c = uVar15;
          FUN_001d26dc(param_1[0xd] + 0x130,&local_6c,4);
          local_6c = CONCAT22(local_6c._2_2_,uVar1);
          FUN_001d26dc(param_1[0xd] + 0x130,&local_6c,2);
          uVar12 = 0xffffffff;
          uVar13 = 0xc1;
        }
        else {
          if (uVar13 != 0xba) goto LAB_0019fe3c;
          if ((*(int *)((long)puVar10 + 0x17c) < *(int *)(puVar10 + 0x2f)) ||
             (iVar3 = FUN_0016cbcc(*puVar10,puVar10 + 0x2e,0x18,puVar10 + 0x2f,
                                   *(int *)((long)puVar10 + 0x17c) + 1), iVar3 == 0)) {
            uVar12 = *(uint *)((long)puVar10 + 0x17c);
            puVar8 = (undefined8 *)(puVar10[0x2e] + (long)(int)uVar12 * 0x18);
            *(uint *)((long)puVar10 + 0x17c) = uVar12 + 1;
            *puVar8 = 0xffffffff00000000;
            puVar8[1] = 0xffffffffffffffff;
            puVar8[2] = 0;
          }
          else {
            uVar12 = 0xffffffff;
          }
          lVar14 = param_1[0xd];
          lVar7 = lVar14 + 0x130;
          if (*(int *)(lVar14 + 0x164) != (int)param_1[1]) {
            FUN_001d27c4(lVar7,200);
            local_6c = *(uint *)(param_1 + 1);
            FUN_001d26dc(lVar7,&local_6c,4);
            *(int *)(lVar14 + 0x164) = (int)param_1[1];
          }
          *(int *)(lVar14 + 0x160) = (int)*(undefined8 *)(lVar14 + 0x138);
          FUN_001d27c4(lVar7,0xbd);
          if (0xe3 < (int)uVar15) {
            piVar9 = *(int **)(*(long *)(*(long *)(*param_1 + 0x18) + 0x60) + (ulong)uVar15 * 8);
            *piVar9 = *piVar9 + 1;
          }
          local_6c = uVar15;
          FUN_001d26dc(param_1[0xd] + 0x130,&local_6c,4);
          local_6c = uVar12;
          FUN_001d26dc(param_1[0xd] + 0x130,&local_6c,4);
          local_6c = CONCAT22(local_6c._2_2_,uVar1);
          FUN_001d26dc(param_1[0xd] + 0x130,&local_6c,2);
          if (((int)uVar12 < 0) || (*(int *)((long)puVar10 + 0x17c) <= (int)uVar12))
          goto code_r0x0019fe00;
          lVar7 = puVar10[0x2e];
          iVar3 = *(int *)(lVar7 + (ulong)uVar12 * 0x18);
          *(int *)(lVar7 + (ulong)uVar12 * 0x18) = iVar3 + 1;
          if (iVar3 == -2 || iVar3 + 2 < 0 != SCARRY4(iVar3,2)) goto code_r0x0019fe20;
          lVar14 = param_1[0xd];
          lVar7 = lVar14 + 0x130;
          if (*(int *)(lVar14 + 0x164) != (int)param_1[1]) {
            FUN_001d27c4(lVar7,200);
            local_6c = *(uint *)(param_1 + 1);
            FUN_001d26dc(lVar7,&local_6c,4);
            *(int *)(lVar14 + 0x164) = (int)param_1[1];
          }
          uVar13 = 0x3c;
          *(int *)(lVar14 + 0x160) = (int)*(undefined8 *)(lVar14 + 0x138);
          FUN_001d27c4(lVar7,0x3c);
        }
        goto LAB_0019fab0;
      }
      puVar8 = puVar10 + 0x26;
      if (*(int *)((long)puVar10 + 0x164) != (int)param_1[1]) {
        FUN_001d27c4(puVar8,200);
        local_6c = *(uint *)(param_1 + 1);
        FUN_001d26dc(puVar8,&local_6c,4);
        *(int *)((long)puVar10 + 0x164) = (int)param_1[1];
      }
      *(int *)(puVar10 + 0x2c) = (int)puVar10[0x27];
      FUN_001d27c4(puVar8,0x72);
      lVar14 = param_1[0xd];
      lVar7 = lVar14 + 0x130;
      if (*(int *)(lVar14 + 0x164) != (int)param_1[1]) {
        FUN_001d27c4(lVar7,200);
        local_6c = *(uint *)(param_1 + 1);
        FUN_001d26dc(lVar7,&local_6c,4);
        *(int *)(lVar14 + 0x164) = (int)param_1[1];
      }
      *(int *)(lVar14 + 0x160) = (int)*(undefined8 *)(lVar14 + 0x138);
      FUN_001d27c4(lVar7,0x14);
      lVar14 = param_1[0xd];
      lVar7 = lVar14 + 0x130;
      if (*(int *)(lVar14 + 0x164) != (int)param_1[1]) {
        FUN_001d27c4(lVar7,200);
        local_6c = *(uint *)(param_1 + 1);
        FUN_001d26dc(lVar7,&local_6c,4);
        *(int *)(lVar14 + 0x164) = (int)param_1[1];
      }
      uVar6 = (undefined4)*(undefined8 *)(lVar14 + 0x138);
      uVar4 = 0x4a;
      uVar13 = 0x4a;
    }
    *(undefined4 *)(lVar14 + 0x160) = uVar6;
    FUN_001d27c4(lVar7,uVar4);
LAB_0019f898:
    uVar12 = 0xffffffff;
  }
LAB_0019fab0:
  uVar4 = 0;
  *param_2 = uVar13;
  *param_3 = local_70;
  *param_4 = uVar15;
  *param_5 = uVar12;
  if (param_6 != (undefined4 *)0x0) {
    *param_6 = uVar11;
  }
LAB_0019fb0c:
  if (*(long *)(lVar2 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar4);
  }
  return;
}

/* ===== FUN_001a04c8 @ 001a04c8 [libNexusScriptRuntime69252.so] ===== */

void FUN_001a04c8(long *param_1,int param_2,undefined2 param_3,uint param_4,undefined4 param_5,
                 uint param_6,int param_7)

{
  int iVar1;
  int iVar2;
  long lVar3;
  int *piVar4;
  undefined8 uVar5;
  long lVar6;
  undefined4 uVar7;
  long lVar8;
  uint local_6c;
  long local_68;
  
  lVar3 = tpidr_el0;
  local_68 = *(long *)(lVar3 + 0x28);
  switch(param_2 + -0x3c) {
  case 0:
    if ((0xe3 < (int)param_4) &&
       (piVar4 = *(int **)(*(long *)(*(long *)(*param_1 + 0x18) + 0x60) + (ulong)param_4 * 8),
       iVar1 = *piVar4, iVar2 = iVar1 + -1, *piVar4 = iVar2, iVar2 == 0 || iVar1 < 1)) {
      FUN_001418dc();
    }
    FUN_0019e7d0(param_1,param_5);
    break;
  case 1:
  case 2:
  case 3:
  case 4:
  case 6:
  case 7:
  case 8:
  case 9:
  case 10:
  case 0xc:
  case 0xd:
    goto switchD_001a052c_caseD_1;
  case 5:
code_r0x001a05d0:
    switch(param_6) {
    case 0:
    case 1:
      goto switchD_001a05e8_caseD_0;
    case 2:
      lVar6 = param_1[0xd];
      lVar8 = lVar6 + 0x130;
      if (*(int *)(lVar6 + 0x164) != (int)param_1[1]) {
        FUN_001d27c4(lVar8,200);
        local_6c = *(uint *)(param_1 + 1);
        FUN_001d26dc(lVar8,&local_6c,4);
        *(int *)(lVar6 + 0x164) = (int)param_1[1];
      }
      uVar7 = (undefined4)*(undefined8 *)(lVar6 + 0x138);
      uVar5 = 0x15;
      break;
    case 3:
      lVar6 = param_1[0xd];
      lVar8 = lVar6 + 0x130;
      if (*(int *)(lVar6 + 0x164) != (int)param_1[1]) {
        FUN_001d27c4(lVar8,200);
        local_6c = *(uint *)(param_1 + 1);
        FUN_001d26dc(lVar8,&local_6c,4);
        *(int *)(lVar6 + 0x164) = (int)param_1[1];
      }
      uVar7 = (undefined4)*(undefined8 *)(lVar6 + 0x138);
      uVar5 = 0x18;
      break;
    case 4:
      lVar6 = param_1[0xd];
      lVar8 = lVar6 + 0x130;
      if (*(int *)(lVar6 + 0x164) != (int)param_1[1]) {
        FUN_001d27c4(lVar8,200);
        local_6c = *(uint *)(param_1 + 1);
        FUN_001d26dc(lVar8,&local_6c,4);
        *(int *)(lVar6 + 0x164) = (int)param_1[1];
      }
      uVar7 = (undefined4)*(undefined8 *)(lVar6 + 0x138);
      uVar5 = 0x1b;
      break;
    default:
                    /* WARNING: Subroutine does not return */
      abort();
    }
    goto LAB_001a0774;
  case 0xb:
    break;
  case 0xe:
    switch(param_6) {
    case 0:
    case 1:
      goto switchD_001a05e8_caseD_0;
    case 2:
      lVar6 = param_1[0xd];
      lVar8 = lVar6 + 0x130;
      if (*(int *)(lVar6 + 0x164) != (int)param_1[1]) {
        FUN_001d27c4(lVar8,200);
        local_6c = *(uint *)(param_1 + 1);
        FUN_001d26dc(lVar8,&local_6c,4);
        *(int *)(lVar6 + 0x164) = (int)param_1[1];
      }
      uVar7 = (undefined4)*(undefined8 *)(lVar6 + 0x138);
      uVar5 = 0x17;
      break;
    case 3:
      lVar6 = param_1[0xd];
      lVar8 = lVar6 + 0x130;
      if (*(int *)(lVar6 + 0x164) != (int)param_1[1]) {
        FUN_001d27c4(lVar8,200);
        local_6c = *(uint *)(param_1 + 1);
        FUN_001d26dc(lVar8,&local_6c,4);
        *(int *)(lVar6 + 0x164) = (int)param_1[1];
      }
      uVar7 = (undefined4)*(undefined8 *)(lVar6 + 0x138);
      uVar5 = 0x1a;
      break;
    case 4:
      lVar6 = param_1[0xd];
      lVar8 = lVar6 + 0x130;
      if (*(int *)(lVar6 + 0x164) != (int)param_1[1]) {
        FUN_001d27c4(lVar8,200);
        local_6c = *(uint *)(param_1 + 1);
        FUN_001d26dc(lVar8,&local_6c,4);
        *(int *)(lVar6 + 0x164) = (int)param_1[1];
      }
      uVar7 = (undefined4)*(undefined8 *)(lVar6 + 0x138);
      uVar5 = 0x1f;
      break;
    default:
                    /* WARNING: Subroutine does not return */
      abort();
    }
    goto LAB_001a0774;
  default:
    if (param_2 != 0xba) {
      if (param_2 != 0xc1) goto switchD_001a052c_caseD_1;
      goto code_r0x001a05d0;
    }
    goto LAB_001a07d4;
  }
  switch(param_6) {
  case 0:
    lVar6 = param_1[0xd];
    lVar8 = lVar6 + 0x130;
    if (*(int *)(lVar6 + 0x164) != (int)param_1[1]) {
      FUN_001d27c4(lVar8,200);
      local_6c = *(uint *)(param_1 + 1);
      FUN_001d26dc(lVar8,&local_6c,4);
      *(int *)(lVar6 + 0x164) = (int)param_1[1];
    }
    uVar7 = (undefined4)*(undefined8 *)(lVar6 + 0x138);
    uVar5 = 0xb5;
    break;
  case 1:
    goto switchD_001a05e8_caseD_0;
  case 2:
    lVar6 = param_1[0xd];
    lVar8 = lVar6 + 0x130;
    if (*(int *)(lVar6 + 0x164) != (int)param_1[1]) {
      FUN_001d27c4(lVar8,200);
      local_6c = *(uint *)(param_1 + 1);
      FUN_001d26dc(lVar8,&local_6c,4);
      *(int *)(lVar6 + 0x164) = (int)param_1[1];
    }
    uVar7 = (undefined4)*(undefined8 *)(lVar6 + 0x138);
    uVar5 = 0x16;
    break;
  case 3:
    lVar6 = param_1[0xd];
    lVar8 = lVar6 + 0x130;
    if (*(int *)(lVar6 + 0x164) != (int)param_1[1]) {
      FUN_001d27c4(lVar8,200);
      local_6c = *(uint *)(param_1 + 1);
      FUN_001d26dc(lVar8,&local_6c,4);
      *(int *)(lVar6 + 0x164) = (int)param_1[1];
    }
    uVar7 = (undefined4)*(undefined8 *)(lVar6 + 0x138);
    uVar5 = 0x19;
    break;
  case 4:
    lVar6 = param_1[0xd];
    lVar8 = lVar6 + 0x130;
    if (*(int *)(lVar6 + 0x164) != (int)param_1[1]) {
      FUN_001d27c4(lVar8,200);
      local_6c = *(uint *)(param_1 + 1);
      FUN_001d26dc(lVar8,&local_6c,4);
      *(int *)(lVar6 + 0x164) = (int)param_1[1];
    }
    uVar7 = (undefined4)*(undefined8 *)(lVar6 + 0x138);
    uVar5 = 0x1d;
    break;
  default:
                    /* WARNING: Subroutine does not return */
    abort();
  }
LAB_001a0774:
  *(undefined4 *)(lVar6 + 0x160) = uVar7;
  FUN_001d27c4(lVar8,uVar5);
switchD_001a05e8_caseD_0:
  switch(param_2 + -0x3c) {
  case 0:
    lVar6 = param_1[0xd];
    lVar8 = lVar6 + 0x130;
    if (*(int *)(lVar6 + 0x164) != (int)param_1[1]) {
      FUN_001d27c4(lVar8,200);
      local_6c = *(uint *)(param_1 + 1);
      FUN_001d26dc(lVar8,&local_6c,4);
      *(int *)(lVar6 + 0x164) = (int)param_1[1];
    }
    uVar7 = (undefined4)*(undefined8 *)(lVar6 + 0x138);
    uVar5 = 0x3d;
    break;
  case 1:
  case 2:
  case 3:
  case 4:
  case 6:
  case 7:
  case 8:
  case 9:
  case 10:
  case 0xc:
  case 0xd:
switchD_001a052c_caseD_1:
                    /* WARNING: Subroutine does not return */
    abort();
  case 5:
    lVar6 = param_1[0xd];
    lVar8 = lVar6 + 0x130;
    if (*(int *)(lVar6 + 0x164) != (int)param_1[1]) {
      FUN_001d27c4(lVar8,200);
      local_6c = *(uint *)(param_1 + 1);
      FUN_001d26dc(lVar8,&local_6c,4);
      *(int *)(lVar6 + 0x164) = (int)param_1[1];
    }
    *(int *)(lVar6 + 0x160) = (int)*(undefined8 *)(lVar6 + 0x138);
    FUN_001d27c4(lVar8,0x43);
    lVar8 = param_1[0xd];
    uVar5 = 4;
    local_6c = param_4;
    goto LAB_001a0904;
  case 0xb:
    lVar6 = param_1[0xd];
    lVar8 = lVar6 + 0x130;
    if (*(int *)(lVar6 + 0x164) != (int)param_1[1]) {
      FUN_001d27c4(lVar8,200);
      local_6c = *(uint *)(param_1 + 1);
      FUN_001d26dc(lVar8,&local_6c,4);
      *(int *)(lVar6 + 0x164) = (int)param_1[1];
    }
    uVar7 = (undefined4)*(undefined8 *)(lVar6 + 0x138);
    uVar5 = 0x49;
    break;
  case 0xe:
    lVar6 = param_1[0xd];
    lVar8 = lVar6 + 0x130;
    if (*(int *)(lVar6 + 0x164) != (int)param_1[1]) {
      FUN_001d27c4(lVar8,200);
      local_6c = *(uint *)(param_1 + 1);
      FUN_001d26dc(lVar8,&local_6c,4);
      *(int *)(lVar6 + 0x164) = (int)param_1[1];
    }
    uVar7 = (undefined4)*(undefined8 *)(lVar6 + 0x138);
    uVar5 = 0x4b;
    break;
  default:
    if (param_2 == 0xc1) {
      lVar8 = param_1[0xd];
      lVar6 = lVar8 + 0x130;
      if (*(int *)(lVar8 + 0x164) != (int)param_1[1]) {
        FUN_001d27c4(lVar6,200);
        local_6c = *(uint *)(param_1 + 1);
        FUN_001d26dc(lVar6,&local_6c,4);
        *(int *)(lVar8 + 0x164) = (int)param_1[1];
      }
      uVar7 = 0xc3;
      *(int *)(lVar8 + 0x160) = (int)*(undefined8 *)(lVar8 + 0x138);
    }
    else {
      if (param_2 != 0xba) goto switchD_001a052c_caseD_1;
LAB_001a07d4:
      if (1 < param_6) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0x5ddf,
                  "void put_lvalue(JSParseState *, int, int, JSAtom, int, PutLValueEnum, BOOL)",
                  "special == PUT_LVALUE_NOKEEP || special == PUT_LVALUE_NOKEEP_DEPTH");
      }
      lVar8 = param_1[0xd];
      uVar7 = 0xffffffbb;
      if (param_7 != 0) {
        uVar7 = 0xffffffbf;
      }
      lVar6 = lVar8 + 0x130;
      if (*(int *)(lVar8 + 0x164) != (int)param_1[1]) {
        FUN_001d27c4(lVar6,200);
        local_6c = *(uint *)(param_1 + 1);
        FUN_001d26dc(lVar6,&local_6c,4);
        *(int *)(lVar8 + 0x164) = (int)param_1[1];
      }
      *(int *)(lVar8 + 0x160) = (int)*(undefined8 *)(lVar8 + 0x138);
    }
    FUN_001d27c4(lVar6,uVar7);
    local_6c = param_4;
    FUN_001d26dc(param_1[0xd] + 0x130,&local_6c,4);
    lVar8 = param_1[0xd];
    uVar5 = 2;
    local_6c = CONCAT22(local_6c._2_2_,param_3);
LAB_001a0904:
    FUN_001d26dc(lVar8 + 0x130,&local_6c,uVar5);
    goto LAB_001a090c;
  }
  *(undefined4 *)(lVar6 + 0x160) = uVar7;
  FUN_001d27c4(lVar8,uVar5);
LAB_001a090c:
  if (*(long *)(lVar3 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_001a56e8 @ 001a56e8 [libNexusScriptRuntime69252.so] ===== */

void FUN_001a56e8(long param_1)

{
  uint uVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  char cVar5;
  long lVar6;
  long lVar7;
  undefined4 local_3c;
  long local_38;
  
  lVar3 = tpidr_el0;
  local_38 = *(long *)(lVar3 + 0x28);
  lVar7 = *(long *)(param_1 + 0x68);
  uVar1 = *(uint *)(lVar7 + 0x160);
  if ((int)uVar1 < 0) {
    cVar5 = '\0';
  }
  else {
    cVar5 = *(char *)(*(long *)(lVar7 + 0x130) + (ulong)uVar1);
  }
  if (cVar5 == -0x39) {
    lVar6 = *(long *)(lVar7 + 0x130);
    lVar4 = (long)(int)((uVar1 - *(int *)(lVar6 + (int)uVar1 + 1)) + 1);
    if (*(char *)(lVar6 + lVar4) != 'V') {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x5964,"void set_object_name_computed(JSParseState *)",
                "fd->byte_code.buf[define_class_pos] == OP_define_class");
    }
    *(undefined1 *)(lVar6 + lVar4) = 0x57;
    *(undefined4 *)(lVar7 + 0x160) = 0xffffffff;
  }
  else if (cVar5 == 'M') {
    iVar2 = *(int *)(param_1 + 8);
    lVar4 = lVar7 + 0x130;
    *(long *)(lVar7 + 0x138) = (long)(int)uVar1;
    *(undefined4 *)(lVar7 + 0x160) = 0xffffffff;
    if (*(int *)(lVar7 + 0x164) != iVar2) {
      FUN_001d27c4(lVar4,200);
      local_3c = *(undefined4 *)(param_1 + 8);
      FUN_001d26dc(lVar4,&local_3c,4);
      *(undefined4 *)(lVar7 + 0x164) = *(undefined4 *)(param_1 + 8);
    }
    *(int *)(lVar7 + 0x160) = (int)*(undefined8 *)(lVar7 + 0x138);
    FUN_001d27c4(lVar4,0x4e);
  }
  if (*(long *)(lVar3 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001a98b0 @ 001a98b0 [libNexusScriptRuntime69252.so] ===== */

/* WARNING: Removing unreachable block (ram,0x001ab438) */
/* WARNING: Removing unreachable block (ram,0x001aa758) */
/* WARNING: Removing unreachable block (ram,0x001ab9dc) */
/* WARNING: Removing unreachable block (ram,0x001aac30) */

void FUN_001a98b0(long param_1,long *param_2)

{
  char *pcVar1;
  byte *pbVar2;
  byte bVar3;
  undefined1 uVar4;
  char cVar5;
  byte bVar6;
  ushort uVar7;
  int iVar8;
  long lVar9;
  bool bVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  int iVar14;
  int iVar15;
  undefined8 *puVar16;
  void *pvVar17;
  undefined4 uVar18;
  int iVar19;
  long *plVar20;
  int *piVar21;
  uint *puVar22;
  uint uVar23;
  int iVar24;
  long lVar25;
  long lVar26;
  int *piVar27;
  long *plVar28;
  ulong uVar29;
  size_t unaff_x24;
  long lVar30;
  undefined2 uVar31;
  int *piVar32;
  uint uVar33;
  ulong uVar34;
  undefined8 uVar35;
  long local_d8;
  int local_d0;
  uint local_cc;
  uint local_c8;
  uint local_c4;
  undefined4 local_c0;
  uint local_bc;
  uint local_b4;
  long local_b0;
  ulong uStack_a8;
  long local_a0;
  long lStack_98;
  long lStack_90;
  long lStack_88;
  uint local_7c;
  uint local_78;
  uint local_74;
  long local_70;
  
  lVar9 = tpidr_el0;
  local_70 = *(long *)(lVar9 + 0x28);
  lVar26 = param_2[0x26];
  lVar30 = param_2[0x2e];
  local_7c = *(uint *)((long)param_2 + 0x1d4);
  iVar24 = (int)param_2[0x27];
  local_d8 = lVar26;
  local_d0 = iVar24;
  puVar16 = (undefined8 *)FUN_001d2548(&local_b0,*(undefined8 *)(param_1 + 0x18),FUN_00138cc0);
  if ((int)param_2[0x36] == 0) {
LAB_001a9958:
    if (((int)param_2[0x38] == 0) || ((*(byte *)((long)param_2 + 0x86) >> 1 & 1) != 0)) {
LAB_001a99b0:
      if ((int)param_2[0x1b] < 0) {
LAB_001a9a14:
        iVar14 = *(int *)((long)param_2 + 0xd4);
      }
      else {
        FUN_001d27c4(&local_b0,0xc);
        FUN_001d27c4(&local_b0,4);
        iVar14 = (int)param_2[0x1b];
        if (iVar14 < 4) {
          puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,iVar14 + -0x35);
          iVar14 = *(int *)((long)param_2 + 0xd4);
        }
        else {
          if (iVar14 < 0x100) {
            FUN_001d27c4(&local_b0,0xc5);
            puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,iVar14);
            goto LAB_001a9a14;
          }
          FUN_001d27c4(&local_b0,0x59);
          local_74 = CONCAT22(local_74._2_2_,(short)iVar14);
          puVar16 = (undefined8 *)FUN_001d26dc(&local_b0,&local_74,2);
          iVar14 = *(int *)((long)param_2 + 0xd4);
        }
      }
      if (-1 < iVar14) {
        FUN_001d27c4(&local_b0,0xc);
        FUN_001d27c4(&local_b0,2);
        iVar14 = *(int *)((long)param_2 + 0xd4);
        if (iVar14 < 4) {
          puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,iVar14 + -0x35);
        }
        else if (iVar14 < 0x100) {
          FUN_001d27c4(&local_b0,0xc5);
          puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,iVar14);
        }
        else {
          FUN_001d27c4(&local_b0,0x59);
          local_74 = CONCAT22(local_74._2_2_,(short)iVar14);
          puVar16 = (undefined8 *)FUN_001d26dc(&local_b0,&local_74,2);
        }
      }
      if ((int)param_2[0x1a] < 0) {
LAB_001a9b1c:
        iVar14 = *(int *)((long)param_2 + 0xcc);
      }
      else {
        FUN_001d27c4(&local_b0,0xc);
        FUN_001d27c4(&local_b0,3);
        iVar14 = (int)param_2[0x1a];
        if (iVar14 < 4) {
          puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,iVar14 + -0x35);
          iVar14 = *(int *)((long)param_2 + 0xcc);
        }
        else {
          if (iVar14 < 0x100) {
            FUN_001d27c4(&local_b0,0xc5);
            puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,iVar14);
            goto LAB_001a9b1c;
          }
          FUN_001d27c4(&local_b0,0x59);
          local_74 = CONCAT22(local_74._2_2_,(short)iVar14);
          puVar16 = (undefined8 *)FUN_001d26dc(&local_b0,&local_74,2);
          iVar14 = *(int *)((long)param_2 + 0xcc);
        }
      }
      if (-1 < iVar14) {
        if ((int)param_2[0xf] == 0) {
          FUN_001d27c4(&local_b0,8);
          iVar14 = *(int *)((long)param_2 + 0xcc);
          if (iVar14 < 4) {
            iVar14 = iVar14 + -0x35;
          }
          else {
            if (0xff < iVar14) {
              FUN_001d27c4(&local_b0,0x59);
              local_74 = CONCAT22(local_74._2_2_,(short)iVar14);
              goto LAB_001a9b40;
            }
            FUN_001d27c4(&local_b0,0xc5);
          }
          puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,iVar14);
        }
        else {
          FUN_001d27c4(&local_b0,0x61);
          local_74 = CONCAT22(local_74._2_2_,(short)*(undefined4 *)((long)param_2 + 0xcc));
LAB_001a9b40:
          puVar16 = (undefined8 *)FUN_001d26dc(&local_b0,&local_74,2);
        }
      }
      if (*(int *)((long)param_2 + 0xbc) < 0) {
LAB_001a9ca0:
        iVar14 = *(int *)((long)param_2 + 0xc4);
      }
      else {
        if ((*(byte *)((long)param_2 + 0x86) & 1) == 0) {
          bVar10 = (int)param_2[10] != 0;
        }
        else {
          bVar10 = false;
        }
        FUN_001d27c4(&local_b0,0xc);
        FUN_001d27c4(&local_b0,bVar10);
        iVar14 = (int)param_2[0x18];
        if (-1 < iVar14) {
          if (iVar14 < 4) {
            FUN_001d27c4(&local_b0,iVar14 + -0x31);
          }
          else if (iVar14 < 0x100) {
            FUN_001d27c4(&local_b0,0xc6);
            FUN_001d27c4(&local_b0,iVar14);
          }
          else {
            FUN_001d27c4(&local_b0,0x5a);
            local_74 = CONCAT22(local_74._2_2_,(short)iVar14);
            FUN_001d26dc(&local_b0,&local_74,2);
          }
        }
        iVar14 = *(int *)((long)param_2 + 0xbc);
        if (iVar14 < 4) {
          puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,iVar14 + -0x35);
          iVar14 = *(int *)((long)param_2 + 0xc4);
        }
        else {
          if (iVar14 < 0x100) {
            FUN_001d27c4(&local_b0,0xc5);
            puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,iVar14);
            goto LAB_001a9ca0;
          }
          FUN_001d27c4(&local_b0,0x59);
          local_74 = CONCAT22(local_74._2_2_,(short)iVar14);
          puVar16 = (undefined8 *)FUN_001d26dc(&local_b0,&local_74,2);
          iVar14 = *(int *)((long)param_2 + 0xc4);
        }
      }
      if (-1 < iVar14) {
        FUN_001d27c4(&local_b0,0xc);
        FUN_001d27c4(&local_b0,2);
        iVar14 = *(int *)((long)param_2 + 0xc4);
        if (iVar14 < 4) {
          puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,iVar14 + -0x35);
        }
        else if (iVar14 < 0x100) {
          FUN_001d27c4(&local_b0,0xc5);
          puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,iVar14);
        }
        else {
          FUN_001d27c4(&local_b0,0x59);
          local_74 = CONCAT22(local_74._2_2_,(short)iVar14);
          puVar16 = (undefined8 *)FUN_001d26dc(&local_b0,&local_74,2);
        }
      }
      if (-1 < *(int *)((long)param_2 + 0xb4)) {
        FUN_001d27c4(&local_b0,0xc);
        FUN_001d27c4(&local_b0,5);
        iVar14 = *(int *)((long)param_2 + 0xb4);
        if (iVar14 < 4) {
          puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,iVar14 + -0x35);
        }
        else if (iVar14 < 0x100) {
          FUN_001d27c4(&local_b0,0xc5);
          puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,iVar14);
        }
        else {
          FUN_001d27c4(&local_b0,0x59);
          local_74 = CONCAT22(local_74._2_2_,(short)iVar14);
          puVar16 = (undefined8 *)FUN_001d26dc(&local_b0,&local_74,2);
        }
      }
      if (-1 < (int)param_2[0x17]) {
        FUN_001d27c4(&local_b0,0xc);
        FUN_001d27c4(&local_b0,5);
        iVar14 = (int)param_2[0x17];
        if (iVar14 < 4) {
          puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,iVar14 + -0x35);
        }
        else if (iVar14 < 0x100) {
          FUN_001d27c4(&local_b0,0xc5);
          puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,iVar14);
        }
        else {
          FUN_001d27c4(&local_b0,0x59);
          local_74 = CONCAT22(local_74._2_2_,(short)iVar14);
          puVar16 = (undefined8 *)FUN_001d26dc(&local_b0,&local_74,2);
        }
      }
      uVar11 = 0;
      do {
        if (iVar24 <= (int)uVar11) {
          uVar34 = (ulong)*(uint *)((long)param_2 + 0x17c);
          if (0 < (int)*(uint *)((long)param_2 + 0x17c)) {
            plVar20 = (long *)(lVar30 + 0x10);
            do {
              if (*plVar20 != 0) {
                    /* WARNING: Subroutine does not return */
                __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                          ,0x8118,"int resolve_labels(JSContext *, JSFunctionDef *)",
                          "label_slots[i].first_reloc == NULL");
              }
              plVar20 = plVar20 + 3;
              uVar34 = uVar34 - 1;
            } while (uVar34 != 0);
          }
          if (*(int *)((long)param_2 + 0x1b4) < 1) {
            iVar24 = 0;
          }
          else {
            iVar14 = 0;
            iVar24 = 0;
            piVar27 = (int *)param_2[0x35];
            iVar15 = -1;
            do {
              iVar19 = *piVar27;
              if (iVar19 - 0x6aU < 3) {
                uVar11 = 3;
LAB_001aba84:
                uVar12 = piVar27[2];
                lVar26 = (long)(int)uVar12;
                iVar8 = *(int *)(param_2[0x2e] + (long)piVar27[4] * 0x18 + 0xc) - uVar12;
                if (iVar8 + 0x80 < 0 == SCARRY4(iVar8,0x80) && iVar8 <= (int)(uVar11 + 0x7f)) {
                  piVar27[1] = 1;
                  if (iVar19 == 0xef) {
                    iVar19 = 0xee;
                  }
                  else {
                    iVar19 = iVar19 + 0x82;
                  }
                  *piVar27 = iVar19;
                }
                else {
                  if ((iVar19 != 0x6c) || (iVar8 + 0x8000U >> 0x10 != 0)) goto LAB_001aba48;
                  uVar11 = 2;
                  iVar19 = 0xef;
                  piVar27[0] = 0xef;
                  piVar27[1] = 2;
                }
                uVar34 = (ulong)uVar11;
                *(char *)(lVar26 + local_b0 + -1) = (char)iVar19;
                pvVar17 = (void *)(local_b0 + lVar26 + (long)piVar27[1]);
                memmove(pvVar17,(void *)((long)pvVar17 + uVar34),
                        (uStack_a8 - (lVar26 + piVar27[1])) - uVar34);
                iVar19 = *(int *)((long)param_2 + 0x17c);
                uStack_a8 = uStack_a8 - uVar34;
                if (0 < iVar19) {
                  piVar21 = (int *)(param_2[0x2e] + 0xc);
                  do {
                    if ((int)uVar12 < *piVar21) {
                      *piVar21 = *piVar21 - uVar11;
                    }
                    iVar19 = iVar19 + -1;
                    piVar21 = piVar21 + 6;
                  } while (iVar19 != 0);
                }
                if (iVar14 + 1 < *(int *)((long)param_2 + 0x1b4)) {
                  iVar19 = *(int *)((long)param_2 + 0x1b4) + iVar15;
                  lVar26 = 0x18;
                  do {
                    if ((int)uVar12 < *(int *)((long)piVar27 + lVar26)) {
                      *(uint *)((long)piVar27 + lVar26) = *(int *)((long)piVar27 + lVar26) - uVar11;
                    }
                    iVar19 = iVar19 + -1;
                    lVar26 = lVar26 + 0x10;
                  } while (iVar19 != 0);
                }
                uVar34 = (ulong)*(uint *)((long)param_2 + 0x1c4);
                iVar24 = iVar24 + 1;
                if (0 < (int)*(uint *)((long)param_2 + 0x1c4)) {
                  puVar22 = (uint *)param_2[0x37];
                  do {
                    if (uVar12 < *puVar22) {
                      *puVar22 = *puVar22 - uVar11;
                    }
                    puVar22 = puVar22 + 2;
                    uVar34 = uVar34 - 1;
                  } while (uVar34 != 0);
                }
              }
              else if (iVar19 == 0xef) {
                uVar11 = 1;
                goto LAB_001aba84;
              }
LAB_001aba48:
              iVar14 = iVar14 + 1;
              piVar27 = piVar27 + 4;
              iVar15 = iVar15 + -1;
            } while (iVar14 < *(int *)((long)param_2 + 0x1b4));
          }
          if ((iVar24 != 0) && (0 < *(int *)((long)param_2 + 0x1b4))) {
            iVar24 = 0;
            piVar27 = (int *)(param_2[0x35] + 0xc);
            do {
              iVar14 = piVar27[-2];
              lVar26 = (long)piVar27[-1];
              iVar15 = *(int *)(param_2[0x2e] + (long)*piVar27 * 0x18 + 0xc) - piVar27[-1];
              if (iVar14 == 4) {
                *(int *)(local_b0 + lVar26) = iVar15;
              }
              else if (iVar14 == 2) {
                *(short *)(local_b0 + lVar26) = (short)iVar15;
              }
              else if (iVar14 == 1) {
                *(char *)(local_b0 + lVar26) = (char)iVar15;
              }
              iVar24 = iVar24 + 1;
              piVar27 = piVar27 + 4;
            } while (iVar24 < *(int *)((long)param_2 + 0x1b4));
          }
          (**(code **)(*(long *)(param_1 + 0x18) + 8))
                    (*(long *)(param_1 + 0x18) + 0x20,param_2[0x35]);
          param_2[0x35] = 0;
          (**(code **)(*(long *)(param_1 + 0x18) + 8))
                    (*(long *)(param_1 + 0x18) + 0x20,param_2[0x2e]);
          param_2[0x2e] = 0;
          if (((*(byte *)((long)param_2 + 0x86) >> 1 & 1) == 0) && (param_2[0x37] != 0)) {
            plVar20 = param_2 + 0x3b;
            iVar24 = *(int *)((long)param_2 + 0x1d4);
            FUN_001d2548(plVar20,*(undefined8 *)(*param_2 + 0x18),FUN_00138cc0);
            if (0 < *(int *)((long)param_2 + 0x1c4)) {
              lVar26 = 0;
              iVar14 = 0;
              do {
                piVar27 = (int *)(param_2[0x37] + lVar26 * 8);
                iVar15 = piVar27[1];
                if ((-1 < iVar15) && (iVar19 = iVar15 - iVar24, iVar19 != 0)) {
                  iVar8 = *piVar27;
                  uVar11 = iVar8 - iVar14;
                  if (-1 < (int)uVar11) {
                    if ((iVar19 + 1U < 5) && ((int)uVar11 < 0x33)) {
                      uVar11 = iVar19 + 1U + uVar11 * 5 + 1;
                    }
                    else {
                      FUN_001d27c4(plVar20,0);
                      uVar12 = uVar11;
                      if (0x7f < uVar11) {
                        do {
                          uVar11 = uVar12 >> 7;
                          FUN_001d27c4(plVar20,uVar12 | 0xffffff80);
                          uVar23 = uVar12 >> 0xe;
                          uVar12 = uVar11;
                        } while (uVar23 != 0);
                      }
                      FUN_001d27c4(plVar20,uVar11 & 0x7f);
                      uVar11 = iVar19 * 2 ^ iVar19 >> 0x1f;
                      uVar12 = uVar11;
                      if (0x7f < uVar11) {
                        do {
                          uVar11 = uVar12 >> 7;
                          FUN_001d27c4(plVar20,uVar12 | 0xffffff80);
                          uVar23 = uVar12 >> 0xe;
                          uVar12 = uVar11;
                        } while (uVar23 != 0);
                      }
                      uVar11 = uVar11 & 0x7f;
                    }
                    FUN_001d27c4(plVar20,uVar11);
                    iVar24 = iVar15;
                    iVar14 = iVar8;
                  }
                }
                lVar26 = lVar26 + 1;
              } while (lVar26 < *(int *)((long)param_2 + 0x1c4));
            }
          }
          (**(code **)(*(long *)(param_1 + 0x18) + 8))
                    (*(long *)(param_1 + 0x18) + 0x20,param_2[0x37]);
          param_2[0x37] = 0;
          FUN_001d2b08(param_2 + 0x26);
          *(undefined4 *)(param_2 + 0x2d) = 1;
          param_2[0x29] = lStack_98;
          param_2[0x28] = local_a0;
          param_2[0x2b] = lStack_88;
          param_2[0x2a] = lStack_90;
          param_2[0x27] = uStack_a8;
          param_2[0x26] = local_b0;
          if ((int)param_2[0x29] == 0) {
            puVar16 = (undefined8 *)0x0;
          }
          else {
            lVar26 = *(long *)(param_1 + 0x18);
            if ((*(ushort *)(lVar26 + 0xf0) & 0xff) == 0) {
              *(ushort *)(lVar26 + 0xf0) = *(ushort *)(lVar26 + 0xf0) & 0xff00 | 1;
              FUN_001405ec(param_1,"out of memory");
              puVar16 = (undefined8 *)0xffffffff;
              *(ushort *)(lVar26 + 0xf0) = (ushort)*(byte *)(lVar26 + 0xf1) << 8;
            }
            else {
              puVar16 = (undefined8 *)0xffffffff;
            }
          }
          goto LAB_001abe50;
        }
        pbVar2 = (byte *)(lVar26 + (int)uVar11);
        bVar3 = *pbVar2;
        uVar34 = (ulong)bVar3;
        uVar23 = bVar3 - 1;
        bVar6 = (&DAT_00113f23)[uVar34 * 4];
        uVar12 = uVar11 + bVar6;
        if (199 < uVar23) goto switchD_001a9e94_caseD_4;
        uVar33 = (uint)bVar3;
        uVar13 = (uint)bVar3;
        switch(uVar23) {
        case 0:
          uVar23 = *(uint *)(pbVar2 + 1);
          uVar13 = (uint)(uVar23 != 0);
          if (((uVar23 | 0x80000000) == 0x80000000) ||
             (iVar14 = FUN_001ad3a4(&local_d8,uVar12,0x8d,0xffffffff), iVar14 == 0)) {
            puVar16 = (undefined8 *)FUN_001ad3a4(&local_d8,uVar12,0xe,0xffffffff);
            if ((int)puVar16 == 0) {
              puVar16 = (undefined8 *)FUN_001ad3a4(&local_d8,uVar12,0x6b6a,0xffffffff);
              if ((int)puVar16 != 0) goto LAB_001aacec;
              FUN_001ae460(param_2,uStack_a8 & 0xffffffff,local_7c);
              puVar16 = (undefined8 *)FUN_001ae638(&local_b0,uVar23);
            }
            else {
              uVar12 = local_cc;
              if (-1 < (int)local_c8) {
                local_7c = local_c8;
              }
            }
          }
          else {
            if (-1 < (int)local_c8) {
              local_7c = local_c8;
            }
            puVar16 = (undefined8 *)FUN_001ad3a4(&local_d8,local_cc,0xe,0xffffffff);
            if ((int)puVar16 == 0) {
              FUN_001ae460(param_2,uStack_a8 & 0xffffffff,local_7c);
              puVar16 = (undefined8 *)FUN_001ae638(&local_b0,-uVar23);
              uVar12 = local_cc;
            }
            else {
              uVar12 = local_cc;
              if (-1 < (int)local_c8) {
                local_7c = local_c8;
              }
            }
          }
          break;
        case 1:
        case 2:
          iVar14 = *(int *)(pbVar2 + 1);
          if (iVar14 < 0x100) {
            if (param_2[0x37] != 0) {
              iVar15 = *(int *)((long)param_2 + 0x1c4);
              if (((iVar15 < (int)param_2[0x38]) &&
                  (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
                 (*(uint *)(param_2 + 0x39) != local_7c)) {
                puVar22 = (uint *)(param_2[0x37] + (long)iVar15 * 8);
                *puVar22 = (uint)uStack_a8;
                puVar22[1] = local_7c;
                *(int *)((long)param_2 + 0x1c4) = iVar15 + 1;
                *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
                *(uint *)(param_2 + 0x39) = local_7c;
              }
            }
            FUN_001d27c4(&local_b0,uVar13 - 0x41);
            puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,iVar14);
            if (iVar14 < 0x100) break;
          }
          goto switchD_001a9e94_caseD_4;
        case 3:
          uVar11 = *(uint *)(pbVar2 + 1);
          puVar16 = (undefined8 *)FUN_001ad3a4(&local_d8,uVar12,0xe,0xffffffff);
          if ((int)puVar16 == 0) {
            if (uVar11 != 0x2f) goto switchD_001a9e94_caseD_4;
            if (param_2[0x37] != 0) {
              iVar14 = *(int *)((long)param_2 + 0x1c4);
              if (((iVar14 < (int)param_2[0x38]) &&
                  (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
                 (*(uint *)(param_2 + 0x39) != local_7c)) {
                puVar22 = (uint *)(param_2[0x37] + (long)iVar14 * 8);
                *puVar22 = (uint)uStack_a8;
                puVar22[1] = local_7c;
                *(int *)((long)param_2 + 0x1c4) = iVar14 + 1;
                *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
                *(uint *)(param_2 + 0x39) = local_7c;
              }
            }
            puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,0xc3);
          }
          else {
            if (0xe3 < (int)uVar11) {
              puVar16 = *(undefined8 **)(param_1 + 0x18);
              piVar27 = *(int **)(puVar16[0xc] + (ulong)uVar11 * 8);
              iVar14 = *piVar27;
              iVar15 = iVar14 + -1;
              *piVar27 = iVar15;
              if (iVar15 == 0 || iVar14 < 1) {
                puVar16 = (undefined8 *)FUN_001418dc();
              }
            }
            uVar12 = local_cc;
            if (-1 < (int)local_c8) {
              local_7c = local_c8;
            }
          }
          break;
        default:
          goto switchD_001a9e94_caseD_4;
        case 5:
          puVar16 = (undefined8 *)FUN_001ad3a4(&local_d8,uVar12,0xe,0xffffffff);
          if ((int)puVar16 == 0) {
            iVar14 = FUN_001ad3a4(&local_d8,uVar12,0x28,0xffffffff);
            if (iVar14 == 0) {
              puVar16 = (undefined8 *)FUN_001ad3a4(&local_d8,uVar12,0x6b6a,0xffffffff);
              if ((int)puVar16 != 0) {
                uVar13 = 0;
                goto LAB_001aacec;
              }
              iVar14 = FUN_001ad3a4(&local_d8,uVar12,0xac,0xffffffff);
              if (iVar14 == 0) {
                iVar14 = FUN_001ad3a4(&local_d8,uVar12,0xad,0x6b6a,0xffffffff);
                if (iVar14 != 0) {
                  if (-1 < (int)local_c8) {
                    local_7c = local_c8;
                  }
                  FUN_001ae460(param_2,uStack_a8 & 0xffffffff,local_7c);
                  uVar35 = 0xf4;
                  goto LAB_001aacb4;
                }
                goto switchD_001a9e94_caseD_4;
              }
              if (-1 < (int)local_c8) {
                local_7c = local_c8;
              }
              FUN_001ae460(param_2,uStack_a8 & 0xffffffff,local_7c);
              uVar35 = 0xf4;
            }
            else {
              if (-1 < (int)local_c8) {
                local_7c = local_c8;
              }
              if (param_2[0x37] != 0) {
                iVar14 = *(int *)((long)param_2 + 0x1c4);
                if (((iVar14 < (int)param_2[0x38]) &&
                    (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
                   (*(uint *)(param_2 + 0x39) != local_7c)) {
                  puVar22 = (uint *)(param_2[0x37] + (long)iVar14 * 8);
                  *puVar22 = (uint)uStack_a8;
                  puVar22[1] = local_7c;
                  *(int *)((long)param_2 + 0x1c4) = iVar14 + 1;
                  *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
                  *(uint *)(param_2 + 0x39) = local_7c;
                }
              }
              uVar35 = 0x29;
            }
LAB_001ab2f0:
            puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,uVar35);
            uVar12 = local_cc;
          }
          else {
            uVar12 = local_cc;
            if (-1 < (int)local_c8) {
              local_7c = local_c8;
            }
          }
          break;
        case 6:
          iVar14 = FUN_001ad3a4(&local_d8,uVar12,0xac,0xffffffff);
          if (iVar14 != 0) {
            if (-1 < (int)local_c8) {
              local_7c = local_c8;
            }
            if (param_2[0x37] != 0) {
              iVar14 = *(int *)((long)param_2 + 0x1c4);
              if (((iVar14 < (int)param_2[0x38]) &&
                  (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
                 (*(uint *)(param_2 + 0x39) != local_7c)) {
                puVar22 = (uint *)(param_2[0x37] + (long)iVar14 * 8);
                *puVar22 = (uint)uStack_a8;
                puVar22[1] = local_7c;
                *(int *)((long)param_2 + 0x1c4) = iVar14 + 1;
                *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
                *(uint *)(param_2 + 0x39) = local_7c;
              }
            }
            uVar35 = 0xf5;
            goto LAB_001ab2f0;
          }
          iVar14 = FUN_001ad3a4(&local_d8,uVar12,0xad,0x6b6a,0xffffffff);
          if (iVar14 == 0) goto switchD_001a9e94_caseD_8;
          if (-1 < (int)local_c8) {
            local_7c = local_c8;
          }
          if (param_2[0x37] != 0) {
            iVar14 = *(int *)((long)param_2 + 0x1c4);
            if (((iVar14 < (int)param_2[0x38]) &&
                (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
               (*(uint *)(param_2 + 0x39) != local_7c)) {
              puVar22 = (uint *)(param_2[0x37] + (long)iVar14 * 8);
              *puVar22 = (uint)uStack_a8;
              puVar22[1] = local_7c;
              *(int *)((long)param_2 + 0x1c4) = iVar14 + 1;
              *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
              *(uint *)(param_2 + 0x39) = local_7c;
            }
          }
          uVar35 = 0xf5;
LAB_001aacb4:
          puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,uVar35);
          unaff_x24 = (size_t)local_bc;
          uVar34 = (ulong)(local_c4 ^ 1);
          uVar12 = local_cc;
          goto LAB_001aaec4;
        case 8:
        case 9:
switchD_001a9e94_caseD_8:
          uVar13 = (uint)(uVar13 == 10);
          puVar16 = (undefined8 *)FUN_001ad3a4(&local_d8,uVar12,0x6b6a,0xffffffff);
          if ((int)puVar16 == 0) goto switchD_001a9e94_caseD_4;
LAB_001aacec:
          if (-1 < (int)local_c8) {
            local_7c = local_c8;
          }
          uVar12 = local_cc;
          uVar23 = local_bc;
          if (local_c4 - 0x6a == uVar13) goto LAB_001aad18;
          if (((int)local_bc < 0) || (*(int *)((long)param_2 + 0x17c) <= (int)local_bc))
          goto code_r0x001abf4c;
          iVar14 = *(int *)(param_2[0x2e] + (ulong)local_bc * 0x18);
          *(int *)(param_2[0x2e] + (ulong)local_bc * 0x18) = iVar14 + -1;
          if (iVar14 < 1) goto code_r0x001abfac;
          break;
        case 0xd:
          puVar16 = (undefined8 *)FUN_001ad3a4(&local_d8,uVar12,0x29,0xffffffff);
          if ((int)puVar16 == 0) goto switchD_001a9e94_caseD_4;
          if (-1 < (int)local_c8) {
            local_7c = local_c8;
          }
          break;
        case 0x10:
          puVar16 = (undefined8 *)FUN_001ad3a4(&local_d8,uVar12,0x5f5c59,0xffffffff,0xffffffff);
          uVar23 = local_c4;
          uVar11 = local_cc;
          iVar14 = (int)puVar16;
          if (iVar14 != 0) {
            if (-1 < (int)local_c8) {
              local_7c = local_c8;
            }
            uVar13 = local_c4 + 1;
            iVar15 = FUN_001ad3a4(&local_d8,local_cc,0xe,0xffffffff);
            uVar33 = local_cc;
            uVar12 = uVar11;
            uVar11 = 0xffffffff;
            if (iVar15 != 0) {
              if (-1 < (int)local_c8) {
                local_7c = local_c8;
              }
              iVar15 = FUN_001ad3a4(&local_d8,local_cc,uVar23 - 1,local_c0,0xffffffff);
              uVar12 = local_cc;
              uVar11 = local_c8;
              if (iVar15 == 0) {
                uVar12 = uVar33;
                uVar11 = 0xffffffff;
                uVar13 = uVar23;
              }
            }
            if (param_2[0x37] != 0) {
              iVar15 = *(int *)((long)param_2 + 0x1c4);
              if (((iVar15 < (int)param_2[0x38]) &&
                  (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
                 (*(uint *)(param_2 + 0x39) != local_7c)) {
                puVar22 = (uint *)(param_2[0x37] + (long)iVar15 * 8);
                *puVar22 = (uint)uStack_a8;
                puVar22[1] = local_7c;
                *(int *)((long)param_2 + 0x1c4) = iVar15 + 1;
                *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
                *(uint *)(param_2 + 0x39) = local_7c;
              }
            }
            puVar16 = (undefined8 *)FUN_001ae264(&local_b0,uVar13,local_c0);
            if (-1 < (int)uVar11) {
              local_7c = uVar11;
            }
          }
joined_r0x001aa170:
          if (iVar14 == 0) goto switchD_001a9e94_caseD_4;
          break;
        case 0x14:
          iVar14 = FUN_001ad3a4(&local_d8,uVar12,0x3b43,0xe,0xffffffff);
          if (iVar14 == 0) goto switchD_001a9e94_caseD_4;
          if (-1 < (int)local_c8) {
            local_7c = local_c8;
          }
          if (param_2[0x37] != 0) {
            iVar14 = *(int *)((long)param_2 + 0x1c4);
            if (((iVar14 < (int)param_2[0x38]) &&
                (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
               (*(uint *)(param_2 + 0x39) != local_7c)) {
              puVar22 = (uint *)(param_2[0x37] + (long)iVar14 * 8);
              *puVar22 = (uint)uStack_a8;
              puVar22[1] = local_7c;
              *(int *)((long)param_2 + 0x1c4) = iVar14 + 1;
              *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
              *(uint *)(param_2 + 0x39) = local_7c;
            }
          }
          FUN_001d27c4(&local_b0,local_c4);
          local_74 = local_b4;
          puVar16 = (undefined8 *)FUN_001d26dc(&local_b0,&local_74,4);
          uVar12 = local_cc;
          break;
        case 0x21:
        case 0x23:
          uVar31 = *(undefined2 *)(pbVar2 + 1);
          iVar14 = FUN_001ad3a4(&local_d8,uVar12,0x28,0xffffffff);
          if (iVar14 == 0) {
LAB_001aa484:
            if (param_2[0x37] != 0) {
              iVar14 = *(int *)((long)param_2 + 0x1c4);
              if (((iVar14 < (int)param_2[0x38]) &&
                  (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
                 (*(uint *)(param_2 + 0x39) != local_7c)) {
                puVar22 = (uint *)(param_2[0x37] + (long)iVar14 * 8);
                *puVar22 = (uint)uStack_a8;
                puVar22[1] = local_7c;
                *(int *)((long)param_2 + 0x1c4) = iVar14 + 1;
                *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
                *(uint *)(param_2 + 0x39) = local_7c;
              }
            }
            puVar16 = (undefined8 *)FUN_001ae264(&local_b0,uVar13,uVar31);
          }
          else {
            if (-1 < (int)local_c8) {
              local_7c = local_c8;
            }
            if (param_2[0x37] != 0) {
              iVar14 = *(int *)((long)param_2 + 0x1c4);
              if (((iVar14 < (int)param_2[0x38]) &&
                  (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
                 (*(uint *)(param_2 + 0x39) != local_7c)) {
                puVar22 = (uint *)(param_2[0x37] + (long)iVar14 * 8);
                *puVar22 = (uint)uStack_a8;
                puVar22[1] = local_7c;
                *(int *)((long)param_2 + 0x1c4) = iVar14 + 1;
                *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
                *(uint *)(param_2 + 0x39) = local_7c;
              }
            }
            FUN_001ae264(&local_b0,uVar33 + 1,uVar31);
            puVar16 = (undefined8 *)FUN_001ad670(param_2,lVar26,iVar24,local_cc,&local_7c);
            uVar12 = (uint)puVar16;
          }
          break;
        case 0x27:
        case 0x28:
        case 0x2d:
        case 0x2e:
        case 0x2f:
          uVar12 = FUN_001ad670(param_2,lVar26,iVar24,uVar12,&local_7c);
          goto switchD_001a9e94_caseD_4;
        case 0x40:
          if (*(int *)(pbVar2 + 1) != 0x30) goto switchD_001a9e94_caseD_4;
          if (param_2[0x37] != 0) {
            iVar14 = *(int *)((long)param_2 + 0x1c4);
            if (((iVar14 < (int)param_2[0x38]) &&
                (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
               (*(uint *)(param_2 + 0x39) != local_7c)) {
              puVar22 = (uint *)(param_2[0x37] + (long)iVar14 * 8);
              *puVar22 = (uint)uStack_a8;
              puVar22[1] = local_7c;
              *(int *)((long)param_2 + 0x1c4) = iVar14 + 1;
              *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
              *(uint *)(param_2 + 0x39) = local_7c;
            }
          }
          puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,0xeb);
          break;
        case 0x57:
          uVar7 = *(ushort *)(pbVar2 + 1);
          if (0xff < uVar7) goto switchD_001a9e94_caseD_4;
          iVar14 = FUN_001ad3a4(&local_d8,uVar12,0x9291,0x59,uVar7,0xe,0xffffffff);
          if ((iVar14 == 0) &&
             (iVar14 = FUN_001ad3a4(&local_d8,uVar12,0x908f,0x11,0x59,uVar7,0xe,0xffffffff),
             iVar14 == 0)) {
            iVar14 = FUN_001ad3a4(&local_d8,uVar12,4,0x9e,0x11,0x59,uVar7,0xe,0xffffffff);
            if (iVar14 == 0) {
              iVar14 = FUN_001ad3a4(&local_d8,uVar12,1,0x9e,0x11,0x59,uVar7,0xe,0xffffffff);
              if (iVar14 == 0) {
                iVar14 = FUN_001ad3a4(&local_d8,uVar12,0x5e5b58,0xffffffff,0x9e,0x11,0x59,uVar7,0xe,
                                      0xffffffff);
                if (iVar14 == 0) {
                  FUN_001ae460(param_2,uStack_a8 & 0xffffffff,local_7c);
                  puVar16 = (undefined8 *)FUN_001ae264(&local_b0,0x58,uVar7);
                  break;
                }
                if (-1 < (int)local_c8) {
                  local_7c = local_c8;
                }
                FUN_001ae460(param_2,uStack_a8 & 0xffffffff,local_7c);
                FUN_001ae264(&local_b0,local_c4,local_c0);
              }
              else {
                if (-1 < (int)local_c8) {
                  local_7c = local_c8;
                }
                FUN_001ae460(param_2,uStack_a8 & 0xffffffff,local_7c);
                FUN_001ae638(&local_b0,local_bc);
              }
            }
            else {
              if (-1 < (int)local_c8) {
                local_7c = local_c8;
              }
              FUN_001ae460(param_2,uStack_a8 & 0xffffffff,local_7c);
              if (local_b4 == 0x2f) {
                FUN_001d27c4(&local_b0,0xc3);
              }
              else {
                FUN_001d27c4(&local_b0,4);
                local_74 = local_b4;
                FUN_001d26dc(&local_b0,&local_74,4);
              }
            }
            uVar18 = 0x95;
          }
          else {
            if (-1 < (int)local_c8) {
              local_7c = local_c8;
            }
            if (param_2[0x37] != 0) {
              iVar14 = *(int *)((long)param_2 + 0x1c4);
              if (((iVar14 < (int)param_2[0x38]) &&
                  (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
                 (*(uint *)(param_2 + 0x39) != local_7c)) {
                puVar22 = (uint *)(param_2[0x37] + (long)iVar14 * 8);
                *puVar22 = (uint)uStack_a8;
                puVar22[1] = local_7c;
                *(int *)((long)param_2 + 0x1c4) = iVar14 + 1;
                *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
                *(uint *)(param_2 + 0x39) = local_7c;
              }
            }
            uVar18 = 0xffffff93;
            if ((local_c4 & 0xfffffffd) == 0x90) {
              uVar18 = 0xffffff94;
            }
          }
          FUN_001d27c4(&local_b0,uVar18);
          puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,uVar7);
          uVar12 = local_cc;
          break;
        case 0x58:
        case 0x5b:
        case 0x5e:
          uVar31 = *(undefined2 *)(pbVar2 + 1);
          iVar14 = FUN_001ad3a4(&local_d8,uVar12,uVar23,uVar31,0xffffffff);
          if (iVar14 == 0) goto LAB_001aa484;
          if (-1 < (int)local_c8) {
            local_7c = local_c8;
          }
          if (param_2[0x37] != 0) {
            iVar14 = *(int *)((long)param_2 + 0x1c4);
            if (((iVar14 < (int)param_2[0x38]) &&
                (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
               (*(uint *)(param_2 + 0x39) != local_7c)) {
              puVar22 = (uint *)(param_2[0x37] + (long)iVar14 * 8);
              *puVar22 = (uint)uStack_a8;
              puVar22[1] = local_7c;
              *(int *)((long)param_2 + 0x1c4) = iVar14 + 1;
              *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
              *(uint *)(param_2 + 0x39) = local_7c;
            }
          }
          puVar16 = (undefined8 *)FUN_001ae264(&local_b0,uVar33 + 1,uVar31);
          uVar12 = local_cc;
          break;
        case 0x5a:
        case 0x5d:
          uVar31 = *(undefined2 *)(pbVar2 + 1);
          if (param_2[0x37] != 0) {
            iVar14 = *(int *)((long)param_2 + 0x1c4);
            if (((iVar14 < (int)param_2[0x38]) &&
                (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
               (*(uint *)(param_2 + 0x39) != local_7c)) {
              puVar22 = (uint *)(param_2[0x37] + (long)iVar14 * 8);
              *puVar22 = (uint)uStack_a8;
              puVar22[1] = local_7c;
              *(int *)((long)param_2 + 0x1c4) = iVar14 + 1;
              *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
              *(uint *)(param_2 + 0x39) = local_7c;
            }
          }
          puVar16 = (undefined8 *)FUN_001ae264(&local_b0,uVar33,uVar31);
          break;
        case 0x69:
        case 0x6a:
          uVar13 = FUN_001ae4ac(param_2,*(undefined4 *)(pbVar2 + 1),&local_78,0);
          unaff_x24 = (size_t)uVar13;
          uVar23 = uVar12;
          do {
            if (local_d0 <= (int)uVar23) goto LAB_001aa530;
            pcVar1 = (char *)(local_d8 + (int)uVar23);
            cVar5 = *pcVar1;
            if (cVar5 == 'l') {
              if (*(uint *)(pcVar1 + 1) == uVar13) {
                iVar14 = 1;
              }
              else {
LAB_001aa248:
                iVar14 = 3;
              }
            }
            else if (cVar5 == -0x48) {
              iVar14 = 1;
              if (*(uint *)(pcVar1 + 1) != uVar13) {
                iVar14 = 2;
                uVar23 = uVar23 + 5;
              }
            }
            else {
              if (cVar5 != -0x38) goto LAB_001aa248;
              iVar14 = 2;
              uVar23 = uVar23 + 5;
            }
          } while (iVar14 == 2);
          if (iVar14 != 1) {
LAB_001aa530:
            puVar16 = (undefined8 *)FUN_001ad3a4(&local_d8,uVar12,0x6c,0xffffffff);
            uVar23 = local_cc;
            if ((int)puVar16 != 0) {
              do {
                if (local_d0 <= (int)uVar23) goto LAB_001aaec4;
                pcVar1 = (char *)(local_d8 + (int)uVar23);
                cVar5 = *pcVar1;
                if (cVar5 == 'l') {
                  if (*(uint *)(pcVar1 + 1) == uVar13) {
                    iVar14 = 1;
                  }
                  else {
LAB_001aa55c:
                    iVar14 = 3;
                  }
                }
                else if (cVar5 == -0x48) {
                  iVar14 = 1;
                  if (*(uint *)(pcVar1 + 1) != uVar13) {
                    iVar14 = 2;
                    uVar23 = uVar23 + 5;
                  }
                }
                else {
                  if (cVar5 != -0x38) goto LAB_001aa55c;
                  iVar14 = 2;
                  uVar23 = uVar23 + 5;
                }
              } while (iVar14 == 2);
              if (iVar14 == 1) {
                if (-1 < (int)local_c8) {
                  local_7c = local_c8;
                }
                if (((int)uVar13 < 0) || (*(int *)((long)param_2 + 0x17c) <= (int)uVar13))
                goto code_r0x001abf4c;
                iVar14 = *(int *)(param_2[0x2e] + unaff_x24 * 0x18);
                *(int *)(param_2[0x2e] + unaff_x24 * 0x18) = iVar14 + -1;
                if (iVar14 < 1) goto code_r0x001abfac;
                unaff_x24 = (size_t)local_bc;
                uVar34 = (ulong)(uVar33 ^ 1);
                uVar12 = local_cc;
              }
            }
            goto LAB_001aaec4;
          }
          if (((int)uVar13 < 0) || (*(int *)((long)param_2 + 0x17c) <= (int)uVar13))
          goto code_r0x001abf4c;
          iVar14 = *(int *)(param_2[0x2e] + (ulong)uVar13 * 0x18);
          *(int *)(param_2[0x2e] + (ulong)uVar13 * 0x18) = iVar14 + -1;
          if (iVar14 < 1) goto code_r0x001abfac;
          puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,0xe);
          break;
        case 0x6b:
          uVar23 = *(uint *)(pbVar2 + 1);
LAB_001aad18:
          local_74 = 0xffffffff;
          puVar16 = (undefined8 *)FUN_001ae4ac(param_2,uVar23,&local_78,&local_74);
          unaff_x24 = (ulong)puVar16 & 0xffffffff;
          uVar23 = uVar12;
          do {
            iVar14 = (int)puVar16;
            if (local_d0 <= (int)uVar23) goto LAB_001aadf0;
            pcVar1 = (char *)(local_d8 + (int)uVar23);
            cVar5 = *pcVar1;
            if (cVar5 == 'l') {
              if (*(int *)(pcVar1 + 1) == iVar14) {
                iVar15 = 1;
              }
              else {
LAB_001aad44:
                iVar15 = 3;
              }
            }
            else if (cVar5 == -0x48) {
              iVar15 = 1;
              if (*(int *)(pcVar1 + 1) != iVar14) {
                iVar15 = 2;
                uVar23 = uVar23 + 5;
              }
            }
            else {
              if (cVar5 != -0x38) goto LAB_001aad44;
              iVar15 = 2;
              uVar23 = uVar23 + 5;
            }
          } while (iVar15 == 2);
          if (iVar15 == 1) {
            if ((iVar14 < 0) || (*(int *)((long)param_2 + 0x17c) <= iVar14)) {
code_r0x001abf4c:
                    /* WARNING: Subroutine does not return */
              __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                        ,0x5536,"int update_label(JSFunctionDef *, int, int)",
                        "label >= 0 && label < s->label_count");
            }
            lVar25 = ((ulong)puVar16 & 0xffffffff) * 0x18;
            iVar14 = *(int *)(param_2[0x2e] + lVar25);
            *(int *)(param_2[0x2e] + lVar25) = iVar14 + -1;
            if (iVar14 < 1) {
code_r0x001abfac:
                    /* WARNING: Subroutine does not return */
              __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                        ,0x5539,"int update_label(JSFunctionDef *, int, int)","ls->ref_count >= 0");
            }
LAB_001aaeb8:
            bVar10 = false;
          }
          else {
LAB_001aadf0:
            bVar10 = true;
            if ((local_78 < 0x30) && ((1L << ((ulong)local_78 & 0x3f) & 0x830000000000U) != 0)) {
              if ((iVar14 < 0) || (*(int *)((long)param_2 + 0x17c) <= iVar14))
              goto code_r0x001abf4c;
              iVar14 = *(int *)(param_2[0x2e] + unaff_x24 * 0x18);
              *(int *)(param_2[0x2e] + unaff_x24 * 0x18) = iVar14 + -1;
              if (iVar14 < 1) goto code_r0x001abfac;
              if (param_2[0x37] != 0) {
                iVar14 = *(int *)((long)param_2 + 0x1c4);
                if (((iVar14 < (int)param_2[0x38]) &&
                    (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
                   (*(uint *)(param_2 + 0x39) != local_7c)) {
                  puVar22 = (uint *)(param_2[0x37] + (long)iVar14 * 8);
                  *puVar22 = (uint)uStack_a8;
                  puVar22[1] = local_7c;
                  *(int *)((long)param_2 + 0x1c4) = iVar14 + 1;
                  *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
                  *(uint *)(param_2 + 0x39) = local_7c;
                }
              }
              FUN_001d27c4(&local_b0);
              puVar16 = (undefined8 *)FUN_001ad670(param_2,lVar26,iVar24,uVar12,&local_7c);
              uVar12 = (uint)puVar16;
              goto LAB_001aaeb8;
            }
          }
          if (bVar10) {
            uVar34 = 0x6c;
            goto LAB_001aaec4;
          }
          break;
        case 0x6c:
          unaff_x24 = (size_t)*(uint *)(pbVar2 + 1);
          uVar34 = 0x6d;
          goto LAB_001aaec4;
        case 0x6d:
          unaff_x24 = (size_t)*(uint *)(pbVar2 + 1);
          uVar34 = 0x6e;
LAB_001aaec4:
          if (param_2[0x37] != 0) {
            iVar14 = *(int *)((long)param_2 + 0x1c4);
            if (((iVar14 < (int)param_2[0x38]) &&
                (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
               (*(uint *)(param_2 + 0x39) != local_7c)) {
              puVar22 = (uint *)(param_2[0x37] + (long)iVar14 * 8);
              *puVar22 = (uint)uStack_a8;
              puVar22[1] = local_7c;
              *(int *)((long)param_2 + 0x1c4) = iVar14 + 1;
              *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
              *(uint *)(param_2 + 0x39) = local_7c;
            }
          }
          iVar15 = (int)uVar34;
          if (iVar15 == 0x6c) {
            puVar16 = (undefined8 *)FUN_001ad670(param_2,lVar26,iVar24,uVar12,&local_7c);
            uVar12 = (uint)puVar16;
          }
          iVar14 = (int)unaff_x24;
          if ((iVar14 < 0) || (*(int *)((long)param_2 + 0x17c) <= iVar14)) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0x7f00,"int resolve_labels(JSContext *, JSFunctionDef *)",
                      "label >= 0 && label < s->label_count");
          }
          uVar29 = unaff_x24 & 0xffffffff;
          piVar27 = (int *)(param_2[0x35] + (long)*(int *)((long)param_2 + 0x1b4) * 0x10);
          *(int *)((long)param_2 + 0x1b4) = *(int *)((long)param_2 + 0x1b4) + 1;
          *piVar27 = iVar15;
          piVar21 = piVar27 + 1;
          *piVar21 = 4;
          piVar27[2] = (uint)uStack_a8 + 1;
          piVar27[3] = iVar14;
          piVar32 = (int *)(lVar30 + (unaff_x24 & 0xffffffff) * 0x18 + 0xc);
          if (*piVar32 != -1) {
            iVar14 = *piVar32 + ~(uint)uStack_a8;
            if ((iVar14 == (char)iVar14) && (iVar15 - 0x6aU < 3)) {
              *piVar21 = 1;
              *piVar27 = iVar15 + 0x82;
              FUN_001d27c4(&local_b0);
              puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,iVar14);
            }
            else {
              if ((iVar15 != 0x6c) || (iVar14 + 0x8000U >> 0x10 != 0)) goto LAB_001ab128;
              *piVar21 = 2;
              *piVar27 = 0xef;
              FUN_001d27c4(&local_b0,0xef);
              local_74 = CONCAT22(local_74._2_2_,(short)iVar14);
              puVar16 = (undefined8 *)FUN_001d26dc(&local_b0,&local_74,2);
            }
            break;
          }
          iVar19 = *(int *)(lVar30 + uVar29 * 0x18 + 8) + ~uVar11;
          if ((iVar19 < 0x80) && (iVar15 - 0x6aU < 3)) {
            *piVar21 = 1;
            *piVar27 = iVar15 + 0x82;
            FUN_001d27c4(&local_b0);
            FUN_001d27c4(&local_b0,0);
            iVar14 = (uint)uStack_a8;
            puVar16 = (undefined8 *)
                      (*(code *)**(undefined8 **)(param_1 + 0x18))
                                (*(undefined8 **)(param_1 + 0x18) + 4,0x10);
            if (puVar16 == (undefined8 *)0x0) {
              lVar25 = *(long *)(param_1 + 0x18);
              if ((*(ushort *)(lVar25 + 0xf0) & 0xff) == 0) {
                *(ushort *)(lVar25 + 0xf0) = *(ushort *)(lVar25 + 0xf0) & 0xff00 | 1;
                FUN_001405ec(param_1,"out of memory");
                *(ushort *)(lVar25 + 0xf0) = (ushort)*(byte *)(lVar25 + 0xf1) << 8;
              }
              puVar16 = (undefined8 *)0x0;
            }
            else if (puVar16 != (undefined8 *)0x0) {
              iVar14 = iVar14 + -1;
              uVar18 = 1;
LAB_001ab10c:
              lVar25 = lVar30 + uVar29 * 0x18;
              *(int *)(puVar16 + 1) = iVar14;
              *(undefined4 *)((long)puVar16 + 0xc) = uVar18;
              iVar14 = 5;
              *puVar16 = *(undefined8 *)(lVar25 + 0x10);
              *(undefined8 **)(lVar25 + 0x10) = puVar16;
              goto LAB_001ab95c;
            }
            iVar14 = 0xd;
LAB_001ab958:
            if (iVar14 != 0) goto LAB_001ab95c;
LAB_001ab128:
            FUN_001d27c4(&local_b0,uVar34);
            local_74 = *piVar32 - (uint)uStack_a8;
            puVar16 = (undefined8 *)FUN_001d26dc(&local_b0,&local_74,4);
            if (*piVar32 != -1) break;
            iVar14 = (uint)uStack_a8;
            puVar16 = (undefined8 *)
                      (*(code *)**(undefined8 **)(param_1 + 0x18))
                                (*(undefined8 **)(param_1 + 0x18) + 4,0x10);
            if (puVar16 == (undefined8 *)0x0) {
              lVar25 = *(long *)(param_1 + 0x18);
              if ((*(ushort *)(lVar25 + 0xf0) & 0xff) == 0) {
                *(ushort *)(lVar25 + 0xf0) = *(ushort *)(lVar25 + 0xf0) & 0xff00 | 1;
                FUN_001405ec(param_1,"out of memory");
                puVar16 = (undefined8 *)0x0;
                *(ushort *)(lVar25 + 0xf0) = (ushort)*(byte *)(lVar25 + 0xf1) << 8;
              }
              else {
                puVar16 = (undefined8 *)0x0;
              }
            }
            if (puVar16 != (undefined8 *)0x0) {
              lVar25 = lVar30 + uVar29 * 0x18;
              *(int *)(puVar16 + 1) = iVar14 + -4;
              *(undefined4 *)((long)puVar16 + 0xc) = 4;
              *puVar16 = *(undefined8 *)(lVar25 + 0x10);
              *(undefined8 **)(lVar25 + 0x10) = puVar16;
              break;
            }
            iVar14 = 0xd;
            uVar11 = uVar12;
          }
          else {
            iVar14 = 0;
            if ((iVar15 != 0x6c) || (0x7fff < iVar19)) goto LAB_001ab958;
            *piVar21 = 2;
            *piVar27 = 0xef;
            FUN_001d27c4(&local_b0,0xef);
            local_74 = local_74 & 0xffff0000;
            FUN_001d26dc(&local_b0,&local_74,2);
            iVar14 = (uint)uStack_a8;
            puVar16 = (undefined8 *)
                      (*(code *)**(undefined8 **)(param_1 + 0x18))
                                (*(undefined8 **)(param_1 + 0x18) + 4,0x10);
            if (puVar16 == (undefined8 *)0x0) {
              lVar25 = *(long *)(param_1 + 0x18);
              if ((*(ushort *)(lVar25 + 0xf0) & 0xff) == 0) {
                *(ushort *)(lVar25 + 0xf0) = *(ushort *)(lVar25 + 0xf0) & 0xff00 | 1;
                FUN_001405ec(param_1,"out of memory");
                *(ushort *)(lVar25 + 0xf0) = (ushort)*(byte *)(lVar25 + 0xf1) << 8;
              }
              puVar16 = (undefined8 *)0x0;
            }
            else if (puVar16 != (undefined8 *)0x0) {
              iVar14 = iVar14 + -2;
              uVar18 = 2;
              goto LAB_001ab10c;
            }
            iVar14 = 0xd;
LAB_001ab95c:
            uVar11 = uVar12;
            if (iVar14 == 5) break;
          }
          goto LAB_001ab16c;
        case 0x71:
        case 0x72:
          puVar16 = (undefined8 *)FUN_001ad3a4(&local_d8,uVar12,0x5e5b58,0xffffffff,0x49,0xffffffff)
          ;
          if (((int)puVar16 == 0) &&
             (puVar16 = (undefined8 *)FUN_001ad3a4(&local_d8,uVar12,0x40201,0x49,0xffffffff),
             (int)puVar16 == 0)) {
            puVar16 = (undefined8 *)FUN_001ad3a4(&local_d8,uVar12,0x90a0706,0x49,0xffffffff);
            iVar14 = (int)puVar16;
            goto joined_r0x001aa170;
          }
          break;
        case 0x73:
        case 0x74:
        case 0x75:
        case 0x76:
        case 0x77:
        case 0x78:
          uVar23 = *(uint *)(pbVar2 + 1);
          uVar4 = *(undefined1 *)((int)uVar11 + lVar26 + 9);
          uVar11 = FUN_001ae4ac(param_2,*(undefined4 *)(pbVar2 + 5),&local_78,0);
          if (((int)uVar11 < 0) ||
             (unaff_x24 = (size_t)uVar11, *(int *)((long)param_2 + 0x17c) <= (int)uVar11)) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0x7f47,"int resolve_labels(JSContext *, JSFunctionDef *)",
                      "label >= 0 && label < s->label_count");
          }
          if (param_2[0x37] != 0) {
            iVar14 = *(int *)((long)param_2 + 0x1c4);
            if (((iVar14 < (int)param_2[0x38]) &&
                (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
               (*(uint *)(param_2 + 0x39) != local_7c)) {
              puVar22 = (uint *)(param_2[0x37] + (long)iVar14 * 8);
              *puVar22 = (uint)uStack_a8;
              puVar22[1] = local_7c;
              *(int *)((long)param_2 + 0x1c4) = iVar14 + 1;
              *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
              *(uint *)(param_2 + 0x39) = local_7c;
            }
          }
          puVar22 = (uint *)(param_2[0x35] + (long)*(int *)((long)param_2 + 0x1b4) * 0x10);
          *(int *)((long)param_2 + 0x1b4) = *(int *)((long)param_2 + 0x1b4) + 1;
          *puVar22 = uVar13;
          puVar22[1] = 4;
          puVar22[2] = (uint)uStack_a8 + 5;
          puVar22[3] = uVar11;
          FUN_001d27c4(&local_b0,uVar13);
          local_74 = uVar23;
          FUN_001d26dc(&local_b0,&local_74,4);
          lVar25 = lVar30 + unaff_x24 * 0x18;
          local_74 = *(int *)(lVar25 + 0xc) - (uint)uStack_a8;
          FUN_001d26dc(&local_b0,&local_74,4);
          if (*(int *)(lVar25 + 0xc) == -1) {
            iVar14 = (uint)uStack_a8;
            puVar16 = (undefined8 *)
                      (*(code *)**(undefined8 **)(param_1 + 0x18))
                                (*(undefined8 **)(param_1 + 0x18) + 4,0x10);
            if (puVar16 == (undefined8 *)0x0) {
              lVar25 = *(long *)(param_1 + 0x18);
              if ((*(ushort *)(lVar25 + 0xf0) & 0xff) == 0) {
                *(ushort *)(lVar25 + 0xf0) = *(ushort *)(lVar25 + 0xf0) & 0xff00 | 1;
                FUN_001405ec(param_1,"out of memory");
                *(ushort *)(lVar25 + 0xf0) = (ushort)*(byte *)(lVar25 + 0xf1) << 8;
              }
              puVar16 = (undefined8 *)0x0;
            }
            else if (puVar16 != (undefined8 *)0x0) {
              lVar25 = lVar30 + unaff_x24 * 0x18;
              *(int *)(puVar16 + 1) = iVar14 + -4;
              *(undefined4 *)((long)puVar16 + 0xc) = 4;
              *puVar16 = *(undefined8 *)(lVar25 + 0x10);
              *(undefined8 **)(lVar25 + 0x10) = puVar16;
              goto LAB_001a9fdc;
            }
            bVar10 = false;
            iVar14 = 0xd;
          }
          else {
LAB_001a9fdc:
            puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,uVar4);
            iVar14 = 0;
            bVar10 = true;
          }
          uVar11 = uVar12;
          if (bVar10) break;
          goto LAB_001ab16c;
        case 0x90:
        case 0x91:
          iVar14 = FUN_001ad3a4(&local_d8,uVar12,0x5f5c59,0xffffffff,0xe,0xffffffff);
          uVar18 = local_c0;
          uVar11 = local_c4;
          uVar23 = local_cc;
          if (iVar14 == 0) {
            iVar14 = FUN_001ad3a4(&local_d8,uVar12,0x18,0x3b43,0xe,0xffffffff);
            if (iVar14 == 0) {
              iVar14 = FUN_001ad3a4(&local_d8,uVar12,0x19,0x49,0xe,0xffffffff);
              if (iVar14 == 0) goto switchD_001a9e94_caseD_4;
              if (-1 < (int)local_c8) {
                local_7c = local_c8;
              }
              if (param_2[0x37] != 0) {
                iVar14 = *(int *)((long)param_2 + 0x1c4);
                if (((iVar14 < (int)param_2[0x38]) &&
                    (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
                   (*(uint *)(param_2 + 0x39) != local_7c)) {
                  puVar22 = (uint *)(param_2[0x37] + (long)iVar14 * 8);
                  *puVar22 = (uint)uStack_a8;
                  puVar22[1] = local_7c;
                  *(int *)((long)param_2 + 0x1c4) = iVar14 + 1;
                  *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
                  *(uint *)(param_2 + 0x39) = local_7c;
                }
              }
              FUN_001d27c4(&local_b0,bVar3 - 2);
              puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,0x49);
              uVar12 = local_cc;
            }
            else {
              if (-1 < (int)local_c8) {
                local_7c = local_c8;
              }
              if (param_2[0x37] != 0) {
                iVar14 = *(int *)((long)param_2 + 0x1c4);
                if (((iVar14 < (int)param_2[0x38]) &&
                    (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
                   (*(uint *)(param_2 + 0x39) != local_7c)) {
                  puVar22 = (uint *)(param_2[0x37] + (long)iVar14 * 8);
                  *puVar22 = (uint)uStack_a8;
                  puVar22[1] = local_7c;
                  *(int *)((long)param_2 + 0x1c4) = iVar14 + 1;
                  *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
                  *(uint *)(param_2 + 0x39) = local_7c;
                }
              }
              FUN_001d27c4(&local_b0,uVar33 - 2);
              FUN_001d27c4(&local_b0,local_c4);
              local_74 = local_b4;
              puVar16 = (undefined8 *)FUN_001d26dc(&local_b0,&local_74,4);
              uVar12 = local_cc;
            }
          }
          else {
            if (-1 < (int)local_c8) {
              local_7c = local_c8;
            }
            iVar14 = FUN_001ad3a4(&local_d8,local_cc,local_c4 - 1,local_c0,0xffffffff);
            if (iVar14 != 0) {
              if (-1 < (int)local_c8) {
                local_7c = local_c8;
              }
              uVar11 = uVar11 + 1;
              uVar23 = local_cc;
            }
            if (param_2[0x37] != 0) {
              iVar14 = *(int *)((long)param_2 + 0x1c4);
              if (((iVar14 < (int)param_2[0x38]) &&
                  (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
                 (*(uint *)(param_2 + 0x39) != local_7c)) {
                puVar22 = (uint *)(param_2[0x37] + (long)iVar14 * 8);
                *puVar22 = (uint)uStack_a8;
                puVar22[1] = local_7c;
                *(int *)((long)param_2 + 0x1c4) = iVar14 + 1;
                *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
                *(uint *)(param_2 + 0x39) = local_7c;
              }
            }
            FUN_001d27c4(&local_b0,uVar33 - 2);
            puVar16 = (undefined8 *)FUN_001ae264(&local_b0,uVar11,uVar18);
            uVar12 = uVar23;
          }
          break;
        case 0x97:
          puVar16 = (undefined8 *)FUN_001ad3a4(&local_d8,uVar12,4,0xabaaadac,0xffffffff);
          if ((int)puVar16 != 0) {
            if (-1 < (int)local_c8) {
              local_7c = local_c8;
            }
            if (local_b4 == 0x1b) {
              uVar35 = 0xf7;
LAB_001ab458:
              if ((local_c4 == 0xac) || (local_c4 == 0xaa)) {
                if (param_2[0x37] != 0) {
                  iVar14 = *(int *)((long)param_2 + 0x1c4);
                  if (((iVar14 < (int)param_2[0x38]) &&
                      (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
                     (*(uint *)(param_2 + 0x39) != local_7c)) {
                    puVar22 = (uint *)(param_2[0x37] + (long)iVar14 * 8);
                    *puVar22 = (uint)uStack_a8;
                    puVar22[1] = local_7c;
                    *(int *)((long)param_2 + 0x1c4) = iVar14 + 1;
                    *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
                    *(uint *)(param_2 + 0x39) = local_7c;
                  }
                }
                puVar16 = (undefined8 *)FUN_001d27c4(&local_b0,uVar35);
                if (0xe3 < (int)local_b4) {
                  puVar16 = *(undefined8 **)(param_1 + 0x18);
                  piVar27 = *(int **)(puVar16[0xc] + (ulong)local_b4 * 8);
                  iVar14 = *piVar27;
                  iVar15 = iVar14 + -1;
                  *piVar27 = iVar15;
                  if (iVar15 == 0 || iVar14 < 1) {
                    puVar16 = (undefined8 *)FUN_001418dc();
                  }
                }
                uVar34 = 0x98;
                iVar14 = 5;
                uVar12 = local_cc;
              }
              else {
                puVar16 = (undefined8 *)FUN_001ad3a4(&local_d8,local_cc,0x6a,0xffffffff);
                if ((int)puVar16 == 0) goto LAB_001ab6e4;
                if (-1 < (int)local_c8) {
                  local_7c = local_c8;
                }
                FUN_001ae460(param_2,uStack_a8 & 0xffffffff,local_7c);
                FUN_001d27c4(&local_b0,uVar35);
                puVar16 = (undefined8 *)FUN_00140140(param_1,local_b4);
                uVar34 = 0x6b;
                iVar14 = 0xc;
                unaff_x24 = (size_t)local_bc;
                uVar12 = local_cc;
              }
            }
            else {
              if (local_b4 == 0x47) {
                uVar35 = 0xf6;
                goto LAB_001ab458;
              }
LAB_001ab6e4:
              iVar14 = 0;
              uVar34 = 0x98;
            }
            if (iVar14 == 0xc) goto LAB_001aaec4;
            if (iVar14 == 5) break;
          }
switchD_001a9e94_caseD_4:
          if (param_2[0x37] != 0) {
            iVar14 = *(int *)((long)param_2 + 0x1c4);
            if (((iVar14 < (int)param_2[0x38]) &&
                (*(uint *)((long)param_2 + 0x1cc) <= (uint)uStack_a8)) &&
               (*(uint *)(param_2 + 0x39) != local_7c)) {
              puVar22 = (uint *)(param_2[0x37] + (long)iVar14 * 8);
              *puVar22 = (uint)uStack_a8;
              puVar22[1] = local_7c;
              *(int *)((long)param_2 + 0x1c4) = iVar14 + 1;
              *(uint *)((long)param_2 + 0x1cc) = (uint)uStack_a8;
              *(uint *)(param_2 + 0x39) = local_7c;
            }
          }
          puVar16 = (undefined8 *)FUN_001d26dc(&local_b0,pbVar2,bVar6);
          break;
        case 0xb7:
          uVar11 = *(uint *)(pbVar2 + 1);
          unaff_x24 = (size_t)uVar11;
          if (((int)uVar11 < 0) || (*(int *)((long)param_2 + 0x17c) <= (int)uVar11)) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0x7e7e,"int resolve_labels(JSContext *, JSFunctionDef *)",
                      "label >= 0 && label < s->label_count");
          }
          puVar22 = (uint *)(lVar30 + (ulong)uVar11 * 0x18 + 0xc);
          if (*puVar22 != 0xffffffff) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0x7e80,"int resolve_labels(JSContext *, JSFunctionDef *)","ls->addr == -1");
          }
          *puVar22 = (uint)uStack_a8;
          plVar28 = (long *)(lVar30 + (ulong)uVar11 * 0x18 + 0x10);
          plVar20 = (long *)*plVar28;
          while (plVar20 != (long *)0x0) {
            uVar34 = (ulong)*(uint *)(plVar20 + 1);
            iVar14 = *(int *)((long)plVar20 + 0xc);
            lVar25 = *plVar20;
            iVar15 = *puVar22 - *(uint *)(plVar20 + 1);
            if (iVar14 == 1) {
              if (iVar15 != (char)iVar15) {
                    /* WARNING: Subroutine does not return */
                __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                          ,0x7e8f,"int resolve_labels(JSContext *, JSFunctionDef *)",
                          "diff == (int8_t)diff");
              }
              *(char *)(local_b0 + uVar34) = (char)iVar15;
            }
            else if (iVar14 == 2) {
              if (iVar15 != (short)iVar15) {
                    /* WARNING: Subroutine does not return */
                __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                          ,0x7e8b,"int resolve_labels(JSContext *, JSFunctionDef *)",
                          "diff == (int16_t)diff");
              }
              *(short *)(local_b0 + uVar34) = (short)iVar15;
            }
            else if (iVar14 == 4) {
              *(int *)(local_b0 + uVar34) = iVar15;
            }
            puVar16 = (undefined8 *)
                      (**(code **)(*(long *)(param_1 + 0x18) + 8))
                                (*(long *)(param_1 + 0x18) + 0x20,plVar20);
            plVar20 = (long *)lVar25;
          }
          *plVar28 = 0;
          break;
        case 199:
          local_7c = *(uint *)(pbVar2 + 1);
        }
        iVar14 = 0;
        uVar11 = uVar12;
LAB_001ab16c:
      } while (iVar14 == 0);
      if (iVar14 != 0xd) goto LAB_001abe50;
      FUN_001d2b08(&local_b0);
    }
    else {
      lVar25 = *param_2;
      unaff_x24 = (long)(int)param_2[0x38] << 3;
      puVar16 = *(undefined8 **)(lVar25 + 0x18);
      pvVar17 = (void *)(*(code *)*puVar16)(puVar16 + 4,unaff_x24);
      if (pvVar17 == (void *)0x0) {
        FUN_00138d58(lVar25);
        param_2[0x37] = 0;
      }
      else {
        puVar16 = (undefined8 *)memset(pvVar17,0,unaff_x24);
        param_2[0x37] = (long)pvVar17;
        if (pvVar17 != (void *)0x0) {
          *(undefined4 *)((long)param_2 + 0x1cc) = 0;
          *(undefined4 *)(param_2 + 0x39) = *(undefined4 *)((long)param_2 + 0x1d4);
          goto LAB_001a99b0;
        }
      }
    }
  }
  else {
    lVar25 = *param_2;
    unaff_x24 = (long)(int)param_2[0x36] << 4;
    puVar16 = *(undefined8 **)(lVar25 + 0x18);
    pvVar17 = (void *)(*(code *)*puVar16)(puVar16 + 4,unaff_x24);
    if (pvVar17 == (void *)0x0) {
      FUN_00138d58(lVar25);
      param_2[0x35] = 0;
    }
    else {
      puVar16 = (undefined8 *)memset(pvVar17,0,unaff_x24);
      param_2[0x35] = (long)pvVar17;
      if (pvVar17 != (void *)0x0) goto LAB_001a9958;
    }
  }
  puVar16 = (undefined8 *)0xffffffff;
LAB_001abe50:
  if (*(long *)(lVar9 + 0x28) == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(puVar16);
}

/* ===== FUN_001ad670 @ 001ad670 [libNexusScriptRuntime69252.so] ===== */

int FUN_001ad670(long *param_1,long param_2,int param_3,int param_4,undefined4 *param_5)

{
  byte *pbVar1;
  int iVar2;
  byte bVar3;
  byte bVar4;
  uint uVar5;
  int iVar6;
  int *piVar7;
  
  if (param_4 < param_3) {
    do {
      pbVar1 = (byte *)(param_2 + param_4);
      bVar3 = *pbVar1;
      bVar4 = (&DAT_00113f23)[(ulong)bVar3 * 4];
      if (bVar3 == 0xb8) {
        uVar5 = *(uint *)(pbVar1 + 1);
        if (((int)uVar5 < 0) || (*(int *)((long)param_1 + 0x17c) <= (int)uVar5))
        goto code_r0x001ad818;
        iVar2 = *(int *)(param_1[0x2e] + (ulong)uVar5 * 0x18);
        if (iVar2 < 0) goto code_r0x001ad838;
        if (iVar2 != 0) {
          return param_4;
        }
        if (*(long *)(param_1[0x2e] + (long)(int)uVar5 * 0x18 + 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x7b96,"int skip_dead_code(JSFunctionDef *, const uint8_t *, int, int, int *)",
                    "s->label_slots[label].first_reloc == NULL");
        }
      }
      else if (bVar3 == 200) {
        *param_5 = *(undefined4 *)(pbVar1 + 1);
      }
      else if ((byte)(&DAT_00113f26)[(ulong)bVar3 * 4] < 0x1d) {
        uVar5 = 1 << (ulong)((byte)(&DAT_00113f26)[(ulong)bVar3 * 4] & 0x1f);
        if ((uVar5 & 0x3800000) == 0) {
          if ((uVar5 & 0xc000000) == 0) {
            if ((uVar5 & 0x10400000) != 0) {
              uVar5 = *(uint *)(pbVar1 + 1);
              if (((int)uVar5 < 0) || (*(int *)((long)param_1 + 0x17c) <= (int)uVar5))
              goto code_r0x001ad818;
              iVar2 = *(int *)(param_1[0x2e] + (ulong)uVar5 * 0x18);
              *(int *)(param_1[0x2e] + (ulong)uVar5 * 0x18) = iVar2 + -1;
              if (iVar2 < 1) goto code_r0x001ad838;
            }
            goto LAB_001ad6c8;
          }
          uVar5 = *(uint *)(pbVar1 + 5);
          if (((int)uVar5 < 0) || (*(int *)((long)param_1 + 0x17c) <= (int)uVar5)) {
code_r0x001ad818:
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0x5536,"int update_label(JSFunctionDef *, int, int)",
                      "label >= 0 && label < s->label_count");
          }
          iVar2 = *(int *)(param_1[0x2e] + (ulong)uVar5 * 0x18);
          *(int *)(param_1[0x2e] + (ulong)uVar5 * 0x18) = iVar2 + -1;
          if (iVar2 < 1) {
code_r0x001ad838:
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0x5539,"int update_label(JSFunctionDef *, int, int)","ls->ref_count >= 0");
          }
        }
        if ((0xe3 < (int)*(uint *)(pbVar1 + 1)) &&
           (piVar7 = *(int **)(*(long *)(*(long *)(*param_1 + 0x18) + 0x60) +
                              (ulong)*(uint *)(pbVar1 + 1) * 8), iVar2 = *piVar7, iVar6 = iVar2 + -1
           , *piVar7 = iVar6, iVar6 == 0 || iVar2 < 1)) {
          FUN_001418dc();
        }
      }
LAB_001ad6c8:
      param_4 = param_4 + (uint)bVar4;
    } while (param_4 < param_3);
  }
  return param_4;
}

/* ===== FUN_001ad874 @ 001ad874 [libNexusScriptRuntime69252.so] ===== */

void FUN_001ad874(long param_1,undefined8 *param_2,uint param_3,int param_4,undefined8 param_5,
                 uint *param_6,undefined4 param_7)

{
  uint uVar1;
  long lVar2;
  int iVar3;
  int *piVar4;
  undefined8 *puVar5;
  uint local_5c;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  iVar3 = 0x74;
  if (param_4 != 0xb9) {
    iVar3 = param_4 + -0x46;
  }
  FUN_001d27c4(param_5,iVar3);
  if (0xe3 < (int)param_3) {
    piVar4 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + (ulong)param_3 * 8);
    *piVar4 = *piVar4 + 1;
  }
  local_5c = param_3;
  FUN_001d26dc(param_5,&local_5c,4);
  uVar1 = *param_6;
  if ((int)*param_6 < 0) {
    if ((*(int *)((long)param_2 + 0x17c) < *(int *)(param_2 + 0x2f)) ||
       (iVar3 = FUN_0016cbcc(*param_2,param_2 + 0x2e,0x18,param_2 + 0x2f,
                             *(int *)((long)param_2 + 0x17c) + 1), iVar3 == 0)) {
      uVar1 = *(uint *)((long)param_2 + 0x17c);
      puVar5 = (undefined8 *)(param_2[0x2e] + (long)(int)uVar1 * 0x18);
      *(uint *)((long)param_2 + 0x17c) = uVar1 + 1;
      *puVar5 = 0xffffffff00000000;
      puVar5[1] = 0xffffffffffffffff;
      puVar5[2] = 0;
    }
    else {
      uVar1 = 0xffffffff;
    }
  }
  local_5c = uVar1;
  *param_6 = local_5c;
  FUN_001d26dc(param_5,&local_5c,4);
  FUN_001d27c4(param_5,param_7);
  uVar1 = *param_6;
  if (((int)uVar1 < 0) || (*(int *)((long)param_2 + 0x17c) <= (int)uVar1)) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0x5536,"int update_label(JSFunctionDef *, int, int)",
              "label >= 0 && label < s->label_count");
  }
  iVar3 = *(int *)(param_2[0x2e] + (ulong)uVar1 * 0x18);
  *(int *)(param_2[0x2e] + (ulong)uVar1 * 0x18) = iVar3 + 1;
  if (iVar3 == -2 || iVar3 + 2 < 0 != SCARRY4(iVar3,2)) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0x5539,"int update_label(JSFunctionDef *, int, int)","ls->ref_count >= 0");
  }
  *(int *)(param_2 + 0x36) = *(int *)(param_2 + 0x36) + 1;
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001add18 @ 001add18 [libNexusScriptRuntime69252.so] ===== */

int FUN_001add18(undefined8 param_1,long param_2,long param_3,int param_4,undefined4 param_5,
                undefined2 param_6)

{
  char *pcVar1;
  int iVar2;
  long lVar3;
  int iVar4;
  ulong uVar5;
  undefined2 local_5c [2];
  long local_58;
  
  lVar3 = tpidr_el0;
  local_58 = *(long *)(lVar3 + 0x28);
  if (*(char *)(param_2 + param_4) == '<') {
    FUN_001d27c4(param_1,param_5);
    local_5c[0] = param_6;
    FUN_001d26dc(param_1,local_5c,2);
    param_4 = param_4 + 1;
  }
  iVar2 = *(int *)(param_3 + 4);
  uVar5 = (long)iVar2 - 5;
  if (*(char *)(param_2 + uVar5) != -0x48) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0x766e,
              "int optimize_scope_make_ref(JSContext *, JSFunctionDef *, DynBuf *, uint8_t *, LabelSlot *, int, int, int)"
              ,"bc_buf[pos] == OP_label");
  }
  if (*(char *)(param_2 + iVar2) == '\x16') {
    *(undefined1 *)(param_2 + uVar5) = 0x11;
    uVar5 = (ulong)(iVar2 - 4);
  }
  iVar4 = (int)uVar5;
  pcVar1 = (char *)(param_2 + iVar4);
  *pcVar1 = (char)param_5 + '\x01';
  *(undefined2 *)(pcVar1 + 1) = param_6;
  if (iVar4 + 1 < iVar2) {
    memset(pcVar1 + 3,0xb5,(ulong)((iVar2 - iVar4) - 2) + 1);
  }
  if (*(long *)(lVar3 + 0x28) == local_58) {
    return param_4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001ade3c @ 001ade3c [libNexusScriptRuntime69252.so] ===== */

int FUN_001ade3c(long param_1,long param_2,undefined8 param_3,long param_4,long param_5,int param_6,
                uint param_7)

{
  undefined1 *puVar1;
  byte bVar2;
  byte bVar3;
  int iVar4;
  long lVar5;
  int *piVar6;
  int iVar7;
  ulong uVar8;
  undefined1 uVar9;
  uint local_5c;
  long local_58;
  
  lVar5 = tpidr_el0;
  local_58 = *(long *)(lVar5 + 0x28);
  bVar2 = *(byte *)(param_2 + 0x86);
  if ((bVar2 & 1) != 0) {
    FUN_001d27c4(param_3,0x36);
    if (0xe3 < (int)param_7) {
      piVar6 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + (ulong)param_7 * 8);
      *piVar6 = *piVar6 + 1;
    }
    local_5c = param_7;
    FUN_001d26dc(param_3,&local_5c,4);
  }
  if (*(char *)(param_4 + param_6) == '<') {
    FUN_001d27c4(param_3,0x38);
    if (0xe3 < (int)param_7) {
      piVar6 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + (ulong)param_7 * 8);
      *piVar6 = *piVar6 + 1;
    }
    local_5c = param_7;
    FUN_001d26dc(param_3,&local_5c,4);
    param_6 = param_6 + 1;
  }
  iVar4 = *(int *)(param_5 + 4);
  uVar8 = (long)iVar4 - 5;
  if (*(char *)(param_4 + uVar8) != -0x48) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0x769f,
              "int optimize_scope_make_global_ref(JSContext *, JSFunctionDef *, DynBuf *, uint8_t *, LabelSlot *, int, JSAtom)"
              ,"bc_buf[pos] == OP_label");
  }
  bVar3 = *(byte *)(param_4 + iVar4);
  if ((bVar2 & 1) == 0) {
    if (bVar3 != 0x16) goto LAB_001adf8c;
    uVar9 = 0x11;
  }
  else if (bVar3 < 0x1d) {
    if (bVar3 == 0x16) {
      uVar9 = 0x15;
    }
    else {
      if (bVar3 != 0x19) {
LAB_001adf70:
                    /* WARNING: Subroutine does not return */
        abort();
      }
      uVar9 = 0x18;
    }
  }
  else {
    if (bVar3 != 0x1d) {
      if (bVar3 != 0xb5) goto LAB_001adf70;
      goto LAB_001adf8c;
    }
    uVar9 = 0x1b;
  }
  *(undefined1 *)(param_4 + uVar8) = uVar9;
  uVar8 = (ulong)(iVar4 - 4);
LAB_001adf8c:
  iVar7 = (int)uVar8;
  uVar9 = 0x39;
  if ((bVar2 & 1) != 0) {
    uVar9 = 0x3b;
  }
  puVar1 = (undefined1 *)(param_4 + iVar7);
  *puVar1 = uVar9;
  if (0xe3 < (int)param_7) {
    piVar6 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + (ulong)param_7 * 8);
    *piVar6 = *piVar6 + 1;
  }
  *(uint *)(puVar1 + 1) = param_7;
  if (iVar7 + 3 < iVar4) {
    memset((void *)(iVar7 + param_4 + 5),0xb5,(ulong)((iVar4 - iVar7) - 4) + 1);
  }
  if (*(long *)(lVar5 + 0x28) == local_58) {
    return param_6;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001ae4ac @ 001ae4ac [libNexusScriptRuntime69252.so] ===== */

void FUN_001ae4ac(long param_1,uint param_2,undefined4 *param_3,undefined4 *param_4)

{
  byte bVar1;
  ulong uVar2;
  long lVar3;
  int iVar4;
  byte *pbVar5;
  int iVar6;
  ulong uVar7;
  long lVar8;
  
  if (((int)param_2 < 0) || (uVar2 = (ulong)param_2, *(int *)(param_1 + 0x17c) <= (int)param_2))
  goto code_r0x001ae5dc;
  lVar3 = *(long *)(param_1 + 0x170);
  iVar4 = *(int *)(lVar3 + (ulong)param_2 * 0x18);
  *(int *)(lVar3 + (ulong)param_2 * 0x18) = iVar4 + -1;
  if (0 < iVar4) {
    iVar4 = 0;
LAB_001ae4f8:
    if (((int)uVar2 < 0) || (*(int *)(param_1 + 0x17c) <= (int)uVar2)) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x7d9a,"int find_jump_target(JSFunctionDef *, int, int *, int *)",
                "label >= 0 && label < s->label_count");
    }
    lVar8 = *(long *)(param_1 + 0x130);
    iVar6 = *(int *)(lVar3 + uVar2 * 0x18 + 8);
    do {
      pbVar5 = (byte *)(lVar8 + iVar6);
      bVar1 = *pbVar5;
      uVar7 = (ulong)bVar1;
      if (bVar1 < 200) {
        if (bVar1 != 0xb8) goto LAB_001ae558;
      }
      else {
        if (bVar1 != 200) goto LAB_001ae5a0;
        if (param_4 != (undefined4 *)0x0) {
          *param_4 = *(undefined4 *)(pbVar5 + 1);
        }
      }
      iVar6 = iVar6 + (uint)(byte)(&DAT_00113f23)[uVar7 * 4];
    } while( true );
  }
  goto code_r0x001ae61c;
code_r0x001ae578:
  uVar7 = 0x6c;
  goto LAB_001ae5a0;
LAB_001ae558:
  if (bVar1 != 0xe) {
    if (bVar1 != 0x6c) goto LAB_001ae5a0;
    uVar2 = (ulong)*(uint *)(pbVar5 + 1);
    iVar4 = iVar4 + 1;
    if (iVar4 == 10) goto code_r0x001ae578;
    goto LAB_001ae4f8;
  }
  pbVar5 = (byte *)(lVar8 + iVar6);
  do {
    pbVar5 = pbVar5 + 1;
    bVar1 = *pbVar5;
  } while (bVar1 == 0xe);
  if (bVar1 == 0x29) {
    uVar7 = (ulong)bVar1;
  }
LAB_001ae5a0:
  *param_3 = (int)uVar7;
  if (((int)uVar2 < 0) || (*(int *)(param_1 + 0x17c) <= (int)uVar2)) {
code_r0x001ae5dc:
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
              ,0x5536,"int update_label(JSFunctionDef *, int, int)",
              "label >= 0 && label < s->label_count");
  }
  iVar4 = *(int *)(lVar3 + uVar2 * 0x18);
  *(int *)(lVar3 + uVar2 * 0x18) = iVar4 + 1;
  if (iVar4 != -2 && iVar4 + 2 < 0 == SCARRY4(iVar4,2)) {
    return;
  }
code_r0x001ae61c:
                    /* WARNING: Subroutine does not return */
  __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
            ,0x5539,"int update_label(JSFunctionDef *, int, int)","ls->ref_count >= 0");
}

/* ===== FUN_001af468 @ 001af468 [libNexusScriptRuntime69252.so] ===== */

undefined1  [16] FUN_001af468(long param_1,int *param_2,undefined8 param_3,int param_4)

{
  int iVar1;
  long lVar2;
  int iVar3;
  undefined1 auVar4 [16];
  ulong local_40;
  long local_38;
  
  lVar2 = tpidr_el0;
  local_38 = *(long *)(lVar2 + 0x28);
  if ((int)param_3 == -10) {
    if ((((param_4 == 0) || (iVar3 = FUN_001d81f4(&local_40,param_2 + 2,0), iVar3 != 0)) ||
        ((long)local_40 < -0x1fffffffffffff)) || (0x1fffffffffffff < (long)local_40)) {
      if ((*(long *)(param_2 + 6) == -0x8000000000000000) && (param_2[4] != 0)) {
        if (*param_2 != 1) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0x3143,"JSValue JS_CompactBigInt1(JSContext *, JSValue, BOOL)",
                    "p->header.ref_count == 1");
        }
        param_2[4] = 0;
      }
    }
    else {
      iVar3 = *param_2;
      iVar1 = iVar3 + -1;
      *param_2 = iVar1;
      if (iVar1 == 0 || iVar3 < 1) {
        FUN_00141774(*(undefined8 *)(param_1 + 0x18),param_2,param_3);
      }
      param_3 = 0;
      param_2 = (int *)(local_40 & 0xffffffff);
      if ((long)(int)local_40 != local_40) {
        param_3 = 7;
        param_2 = (int *)(double)(long)local_40;
      }
    }
  }
  if (*(long *)(lVar2 + 0x28) != local_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  auVar4._8_8_ = param_3;
  auVar4._0_8_ = param_2;
  return auVar4;
}

/* ===== FUN_001b45f0 @ 001b45f0 [libNexusScriptRuntime69252.so] ===== */

ulong FUN_001b45f0(long param_1,undefined8 param_2,undefined8 param_3)

{
  int iVar1;
  int iVar2;
  long lVar3;
  ulong uVar4;
  ulong uVar5;
  int *piVar6;
  int *piVar7;
  undefined1 auVar8 [16];
  undefined1 auVar9 [16];
  undefined1 auVar10 [16];
  
  if ((int)param_3 == -1) {
    auVar8 = FUN_001446d4(param_1,param_2,param_3,0x38,param_2,param_3,0);
    piVar6 = auVar8._0_8_;
    if (auVar8._8_4_ == 3) {
      if (*(uint *)(*(long *)(param_1 + 0x18) + 0x50) < 0x9b) goto code_r0x001b48a4;
      lVar3 = *(long *)(*(long *)(param_1 + 0x18) + 0x60);
      piVar6 = *(int **)(lVar3 + 0x4d0);
      if (*(ulong *)(piVar6 + 1) >> 0x3e != 1 && (*(ulong *)(piVar6 + 1) & 0xffffffff) == 0x80000000
         ) {
        piVar6 = *(int **)(lVar3 + 0x178);
      }
      auVar10._8_8_ = 0xfffffffffffffff9;
      auVar10._0_8_ = piVar6;
      *piVar6 = *piVar6 + 1;
    }
    else {
      auVar10 = FUN_0014c3ec(param_1,piVar6,auVar8._8_8_,0);
      if ((0xfffffff4 < auVar8._8_4_) &&
         (iVar1 = *piVar6, iVar2 = iVar1 + -1, *piVar6 = iVar2, iVar2 == 0 || iVar1 < 1)) {
        FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar6,auVar8._8_8_);
      }
    }
    uVar5 = auVar10._8_8_;
    piVar6 = auVar10._0_8_;
    if ((uVar5 & 0xffffffff) != 6) {
      auVar8 = FUN_001446d4(param_1,param_2,param_3,0x33,param_2,param_3,0);
      piVar7 = auVar8._0_8_;
      if (auVar8._8_4_ == 3) {
        if (*(uint *)(*(long *)(param_1 + 0x18) + 0x50) < 0x30) {
code_r0x001b48a4:
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                    ,0xc94,"JSValue __JS_AtomToValue(JSContext *, JSAtom, BOOL)",
                    "atom < rt->atom_size");
        }
        piVar7 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + 0x178);
        auVar9._8_8_ = 0xfffffffffffffff9;
        auVar9._0_8_ = piVar7;
        *piVar7 = *piVar7 + 1;
      }
      else {
        auVar9 = FUN_0014c3ec(param_1,piVar7,auVar8._8_8_,0);
        if ((0xfffffff4 < auVar8._8_4_) &&
           (iVar1 = *piVar7, iVar2 = iVar1 + -1, *piVar7 = iVar2, iVar2 == 0 || iVar1 < 1)) {
          FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar7,auVar8._8_8_);
        }
      }
      uVar4 = auVar9._8_8_ & 0xffffffff;
      if (uVar4 != 6) {
        if ((((uVar5 & 0xffffffff) != 0xfffffff9) || ((*(ulong *)(piVar6 + 1) & 0x7fffffff) != 0))
           && ((uVar4 != 0xfffffff9 || ((*(ulong *)(auVar9._0_8_ + 4) & 0x7fffffff) != 0)))) {
          auVar10 = FUN_00175c1c(param_1,&DAT_0010cd92,piVar6,uVar5,&DAT_0010e488);
        }
        uVar5 = FUN_0017279c(param_1,auVar10._0_8_,auVar10._8_8_,auVar9._0_8_,auVar9._8_8_);
        uVar4 = uVar5 & 0xffffffff00000000;
        uVar5 = uVar5 & 0xffffffff;
        goto LAB_001b4710;
      }
      if ((0xfffffff4 < auVar10._8_4_) &&
         (iVar1 = *piVar6, iVar2 = iVar1 + -1, *piVar6 = iVar2, iVar2 == 0 || iVar1 < 1)) {
        FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar6,uVar5);
      }
    }
  }
  else {
    FUN_00143570(param_1,"not an object");
  }
  uVar5 = 0;
  uVar4 = 0;
LAB_001b4710:
  return uVar4 | uVar5;
}

/* ===== FUN_001b73f8 @ 001b73f8 [libNexusScriptRuntime69252.so] ===== */

ulong FUN_001b73f8(long param_1,long param_2,undefined8 param_3)

{
  ushort uVar1;
  char cVar2;
  ulong uVar3;
  int *piVar4;
  long lVar5;
  ulong uVar6;
  uint uVar7;
  char *pcVar8;
  undefined1 auVar9 [16];
  
  if ((int)param_3 == -1) {
    uVar1 = *(ushort *)(param_2 + 6);
    if (uVar1 == 0xd) {
      cVar2 = '\x01';
    }
    else if (uVar1 == 0x30) {
      cVar2 = *(char *)(*(long *)(param_2 + 0x30) + 0x20);
    }
    else {
      cVar2 = *(long *)(*(long *)(*(long *)(param_1 + 0x18) + 0x70) + (ulong)uVar1 * 0x28 + 0x18) !=
              0;
    }
  }
  else {
    cVar2 = '\0';
  }
  if (cVar2 == '\0') {
    FUN_00143570(param_1,"not a function");
    uVar3 = 0;
    uVar6 = 0;
  }
  else {
    uVar7 = 0;
    if ((*(ushort *)(param_2 + 6) < 0x39) &&
       ((1L << ((ulong)*(ushort *)(param_2 + 6) & 0x3f) & 0x110000000012000U) != 0)) {
      lVar5 = *(long *)(param_2 + 0x30);
      if (((*(ushort *)(lVar5 + 0x19) >> 10 & 1) != 0) && (*(long *)(lVar5 + 0x78) != 0)) {
        uVar3 = FUN_0013f444(param_1,*(long *)(lVar5 + 0x78),(long)*(int *)(lVar5 + 0x68));
        uVar6 = uVar3 & 0xffffffff00000000;
        goto LAB_001b7590;
      }
      uVar7 = *(ushort *)(lVar5 + 0x19) >> 4 & 3;
    }
    if (uVar7 - 1 < 3) {
      pcVar8 = &UNK_00114608 + *(int *)(&UNK_00114608 + (long)(int)(uVar7 - 1) * 4);
    }
    else {
      pcVar8 = "function ";
    }
    auVar9 = FUN_001446d4(param_1,param_2,param_3,0x38,param_2,param_3,0);
    if (auVar9._8_4_ == 3) {
      if (*(uint *)(*(long *)(param_1 + 0x18) + 0x50) < 0x30) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                  ,0xc94,"JSValue __JS_AtomToValue(JSContext *, JSAtom, BOOL)",
                  "atom < rt->atom_size");
      }
      piVar4 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + 0x178);
      auVar9._8_8_ = 0xfffffffffffffff9;
      auVar9._0_8_ = piVar4;
      *piVar4 = *piVar4 + 1;
    }
    uVar3 = FUN_00175c1c(param_1,pcVar8,auVar9._0_8_,auVar9._8_8_,"() {\n    [native code]\n}");
    uVar6 = uVar3 & 0xffffffff00000000;
  }
LAB_001b7590:
  return uVar6 | uVar3 & 0xffffffff;
}

/* ===== FUN_001bb32c @ 001bb32c [libNexusScriptRuntime69252.so] ===== */

undefined1  [16]
FUN_001bb32c(long param_1,undefined8 param_2,undefined8 param_3,int param_4,undefined8 *param_5)

{
  double dVar1;
  int *piVar2;
  undefined8 uVar3;
  int iVar4;
  uint uVar5;
  long lVar6;
  undefined1 auVar8 [16];
  undefined1 auVar9 [16];
  int iVar10;
  int *piVar11;
  ulong uVar12;
  undefined8 uVar13;
  long lVar14;
  undefined8 *puVar15;
  long lVar16;
  ulong uVar17;
  undefined8 *puVar18;
  undefined8 *puVar19;
  undefined8 *puVar20;
  ulong uVar21;
  undefined1 auVar22 [16];
  undefined1 auVar23 [16];
  int *local_90;
  long local_80;
  ulong local_78;
  ulong local_70;
  long local_68;
  undefined1 auVar7 [12];
  
  lVar6 = tpidr_el0;
  local_68 = *(long *)(lVar6 + 0x28);
  local_90 = (int *)((ulong)local_90 & 0xffffffff00000000);
  auVar22 = FUN_0014ae60();
  uVar13 = auVar22._8_8_;
  piVar11 = auVar22._0_8_;
  iVar10 = FUN_0015e0c4(param_1,&local_70,piVar11,uVar13);
  uVar17 = local_70;
  if (iVar10 == 0) {
    local_78 = 0;
    if (param_4 < 1) {
      lVar14 = 0;
    }
    else {
      if (0xfffffff4 < (uint)param_5[1]) {
        *(int *)*param_5 = *(int *)*param_5 + 1;
      }
      iVar10 = FUN_0014bda4(param_1,&local_78);
      if (iVar10 != 0) goto LAB_001bb38c;
      if ((long)local_78 < 0) {
        local_78 = local_78 + uVar17;
      }
      if ((long)local_78 < 0) {
        local_78 = 0;
      }
      else if ((long)uVar17 < (long)local_78) {
        local_78 = uVar17;
      }
      lVar14 = local_70 - local_78;
    }
    local_80 = lVar14;
    if (1 < param_4) {
      if (0xfffffff4 < (uint)param_5[3]) {
        *(int *)param_5[2] = *(int *)param_5[2] + 1;
      }
      iVar10 = FUN_0014bda4(param_1,&local_80);
      if (iVar10 != 0) goto LAB_001bb38c;
      if (local_80 < 0) {
        local_80 = 0;
      }
      else if (lVar14 < local_80) {
        local_80 = lVar14;
      }
    }
    uVar5 = param_4 - 2;
    if (uVar5 == 0 || param_4 < 2) {
      uVar5 = 0;
    }
    uVar17 = (ulong)uVar5;
    uVar21 = (local_70 + uVar17) - local_80;
    if ((long)uVar21 < 0x20000000000000) {
      auVar23 = FUN_001bbd98(param_1,uVar21);
      auVar7 = auVar23._0_12_;
      uVar12 = auVar23._0_8_;
      if (auVar23._8_4_ == 6) {
        puVar20 = (undefined8 *)0x0;
        puVar18 = (undefined8 *)0x0;
LAB_001bb598:
        auVar23 = ZEXT816(6) << 0x40;
      }
      else {
        if ((long)uVar21 < 1) {
          puVar20 = (undefined8 *)0x0;
          puVar18 = (undefined8 *)0x0;
        }
        else {
          puVar18 = *(undefined8 **)(uVar12 + 0x38);
          puVar20 = puVar18 + uVar21 * 2;
          if ((((auVar22._8_4_ == 0xffffffff) && (*(short *)((long)piVar11 + 6) == 2)) &&
              ((*(byte *)((long)piVar11 + 5) >> 3 & 1) != 0)) &&
             (lVar14 = *(long *)(piVar11 + 0xe), local_70 == (uint)piVar11[0x10])) {
            if ((long)local_78 < 1) {
              lVar16 = 0;
            }
            else {
              lVar16 = 0;
              puVar15 = (undefined8 *)(lVar14 + 8);
              puVar19 = puVar18;
              do {
                piVar2 = (int *)puVar15[-1];
                uVar3 = *puVar15;
                if (0xfffffff4 < (uint)uVar3) {
                  *piVar2 = *piVar2 + 1;
                }
                puVar18 = puVar19 + 2;
                *puVar19 = piVar2;
                puVar19[1] = uVar3;
                lVar16 = lVar16 + 1;
                puVar15 = puVar15 + 2;
                puVar19 = puVar18;
              } while (lVar16 < (long)local_78);
            }
            if (uVar5 != 0) {
              param_5 = param_5 + 5;
              puVar15 = puVar18;
              do {
                piVar2 = (int *)param_5[-1];
                uVar3 = *param_5;
                if (0xfffffff4 < (uint)uVar3) {
                  *piVar2 = *piVar2 + 1;
                }
                uVar17 = uVar17 - 1;
                param_5 = param_5 + 2;
                puVar18 = puVar15 + 2;
                *puVar15 = piVar2;
                puVar15[1] = uVar3;
                puVar15 = puVar18;
              } while (uVar17 != 0);
            }
            lVar16 = local_80 + lVar16;
            if (lVar16 < (long)local_70) {
              puVar15 = (undefined8 *)(lVar14 + lVar16 * 0x10 + 8);
              puVar19 = puVar18;
              do {
                piVar2 = (int *)puVar15[-1];
                uVar3 = *puVar15;
                if (0xfffffff4 < (uint)uVar3) {
                  *piVar2 = *piVar2 + 1;
                }
                puVar18 = puVar19 + 2;
                *puVar19 = piVar2;
                puVar19[1] = uVar3;
                lVar16 = lVar16 + 1;
                puVar15 = puVar15 + 2;
                puVar19 = puVar18;
              } while (lVar16 < (long)local_70);
            }
          }
          else {
            if ((long)local_78 < 1) {
              lVar14 = 0;
            }
            else {
              lVar14 = 0;
              do {
                iVar10 = FUN_001865e0(param_1,piVar11,uVar13,lVar14,puVar18);
                if (iVar10 == -1) goto LAB_001bb598;
                lVar14 = lVar14 + 1;
                puVar18 = puVar18 + 2;
              } while (lVar14 < (long)local_78);
            }
            if (uVar5 != 0) {
              param_5 = param_5 + 5;
              puVar15 = puVar18;
              do {
                piVar2 = (int *)param_5[-1];
                uVar3 = *param_5;
                if (0xfffffff4 < (uint)uVar3) {
                  *piVar2 = *piVar2 + 1;
                }
                uVar17 = uVar17 - 1;
                param_5 = param_5 + 2;
                puVar18 = puVar15 + 2;
                *puVar15 = piVar2;
                puVar15[1] = uVar3;
                puVar15 = puVar18;
              } while (uVar17 != 0);
            }
            lVar14 = local_80 + lVar14;
            if (lVar14 < (long)local_70) {
              do {
                iVar10 = FUN_001865e0(param_1,piVar11,uVar13,lVar14,puVar18);
                if (iVar10 == -1) goto LAB_001bb598;
                lVar14 = lVar14 + 1;
                puVar18 = puVar18 + 2;
              } while (lVar14 < (long)local_70);
            }
          }
          if (puVar18 != puVar20) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0x9e6b,"JSValue js_array_toSpliced(JSContext *, JSValue, int, JSValue *)",
                      "pval == last");
          }
          uVar3 = 0;
          dVar1 = (double)(uVar21 & 0xffffffff);
          if ((long)(int)uVar21 != uVar21) {
            uVar3 = 7;
            dVar1 = (double)(long)uVar21;
          }
          iVar10 = FUN_0014681c(param_1,uVar12,auVar23._8_8_,0x30,dVar1,uVar3,uVar12,auVar23._8_8_,
                                0x4000);
          if (iVar10 < 0) goto LAB_001bb598;
        }
        local_90 = (int *)(uVar12 & 0xffffffff00000000);
        auVar9._8_8_ = 3;
        auVar9._0_8_ = local_90;
        auVar7 = auVar9._0_12_;
      }
      goto LAB_001bb3a0;
    }
    FUN_00143570(param_1,"invalid array length");
  }
LAB_001bb38c:
  puVar20 = (undefined8 *)0x0;
  puVar18 = (undefined8 *)0x0;
  auVar23 = ZEXT816(6) << 0x40;
  auVar8._8_8_ = 3;
  auVar8._0_8_ = local_90;
  auVar7 = auVar8._0_12_;
LAB_001bb3a0:
  local_90 = auVar7._0_8_;
  for (; puVar18 != puVar20; puVar18 = puVar18 + 2) {
    *(undefined4 *)puVar18 = 0;
    puVar18[1] = 3;
  }
  if ((0xfffffff4 < auVar7._8_4_) &&
     (iVar10 = *local_90, iVar4 = iVar10 + -1, *local_90 = iVar4, iVar4 == 0 || iVar10 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 0x18),local_90);
  }
  if ((0xfffffff4 < auVar22._8_4_) &&
     (iVar10 = *piVar11, iVar4 = iVar10 + -1, *piVar11 = iVar4, iVar4 == 0 || iVar10 < 1)) {
    FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar11,uVar13);
  }
  if (*(long *)(lVar6 + 0x28) == local_68) {
    return auVar23;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001c0178 @ 001c0178 [libNexusScriptRuntime69252.so] ===== */

void FUN_001c0178(long param_1,undefined8 param_2,ulong param_3,undefined8 param_4,
                 undefined8 *param_5,int param_6)

{
  int iVar1;
  long lVar2;
  int iVar3;
  int *piVar4;
  int *piVar5;
  ushort uVar6;
  uint uVar7;
  ulong uVar8;
  undefined1 auVar9 [16];
  uint local_4c;
  long local_48;
  
  lVar2 = tpidr_el0;
  local_48 = *(long *)(lVar2 + 0x28);
  if ((param_3 & 0xfffffffe) == 2) {
    FUN_00143570(param_1,"null or undefined are forbidden");
    auVar9 = ZEXT816(6) << 0x40;
  }
  else {
    auVar9 = FUN_0014c3ec(param_1,param_2,param_3,0);
  }
  piVar4 = auVar9._0_8_;
  uVar7 = auVar9._8_4_;
  if (uVar7 != 6) {
    if (0xfffffff4 < (uint)param_5[1]) {
      *(int *)*param_5 = *(int *)*param_5 + 1;
    }
    iVar3 = FUN_0014bb84(param_1,&local_4c);
    if (iVar3 == 0) {
      if ((param_6 != 0) && ((int)local_4c < 0)) {
        local_4c = (piVar4[1] & 0x7fffffffU) + local_4c;
      }
      if (((int)local_4c < 0) ||
         ((int)((uint)*(undefined8 *)(piVar4 + 1) & 0x7fffffff) <= (int)local_4c)) {
        if (param_6 == 0) {
          if (*(uint *)(*(long *)(param_1 + 0x18) + 0x50) < 0x30) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0xc94,"JSValue __JS_AtomToValue(JSContext *, JSAtom, BOOL)",
                      "atom < rt->atom_size");
          }
          piVar5 = *(int **)(*(long *)(*(long *)(param_1 + 0x18) + 0x60) + 0x178);
          uVar8 = (ulong)piVar5 & 0xffffffff00000000;
          *piVar5 = *piVar5 + 1;
        }
        else {
          piVar5 = (int *)0x0;
          uVar8 = 0;
        }
      }
      else {
        if ((int)(uint)*(undefined8 *)(piVar4 + 1) < 0) {
          uVar6 = *(ushort *)((long)piVar4 + (ulong)local_4c * 2 + 0x10);
        }
        else {
          uVar6 = (ushort)*(byte *)((long)piVar4 + (ulong)local_4c + 0x10);
        }
        piVar5 = (int *)FUN_00144c24(param_1,uVar6);
        uVar8 = (ulong)piVar5 & 0xffffffff00000000;
      }
      if ((0xfffffff4 < uVar7) &&
         (iVar3 = *piVar4, iVar1 = iVar3 + -1, *piVar4 = iVar1, iVar1 == 0 || iVar3 < 1)) {
        FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar4,auVar9._8_8_);
      }
      piVar4 = (int *)(uVar8 | (ulong)piVar5 & 0xffffffff);
    }
    else {
      if ((0xfffffff4 < uVar7) &&
         (iVar3 = *piVar4, iVar1 = iVar3 + -1, *piVar4 = iVar1, iVar1 == 0 || iVar3 < 1)) {
        FUN_00141774(*(undefined8 *)(param_1 + 0x18),piVar4,auVar9._8_8_);
      }
      piVar4 = (int *)0x0;
    }
  }
  if (*(long *)(lVar2 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(piVar4);
  }
  return;
}

/* ===== FUN_001c5554 @ 001c5554 [libNexusScriptRuntime69252.so] ===== */

undefined1  [16] FUN_001c5554(long param_1,int *param_2,undefined8 param_3)

{
  int iVar1;
  int iVar2;
  ulong uVar3;
  long lVar4;
  int *piVar5;
  undefined8 uVar6;
  ulong uVar7;
  undefined1 auVar8 [16];
  
  if ((int)param_3 == -1) {
    if ((*(short *)((long)param_2 + 6) == 7) &&
       (param_3 = *(undefined8 *)(param_2 + 0xe), (int)param_3 == -8)) {
      param_2 = *(int **)(param_2 + 0xc);
      *param_2 = *param_2 + 1;
      goto LAB_001c5610;
    }
  }
  else if ((int)param_3 == -8) {
    *param_2 = *param_2 + 1;
    goto LAB_001c5610;
  }
  FUN_00143570(param_1,"not a symbol");
  param_2 = (int *)0x0;
  param_3 = 6;
LAB_001c5610:
  if ((uint)param_3 != 6) {
    uVar3 = *(ulong *)(param_2 + 1);
    if ((int)uVar3 == -0x80000000) {
      uVar7 = 0;
      uVar3 = 0;
      uVar6 = 3;
    }
    else {
      if (uVar3 >> 0x3e < 3) {
        lVar4 = *(long *)(param_1 + 0x18);
        uVar3 = (ulong)*(uint *)(*(long *)(lVar4 + 0x58) +
                                (ulong)((uint)(uVar3 >> 0x20) & *(int *)(lVar4 + 0x48) - 1U &
                                       0x3fffffff) * 4);
        piVar5 = *(int **)(*(long *)(lVar4 + 0x60) + uVar3 * 8);
        while (piVar5 != param_2) {
          if ((int)uVar3 == 0) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                      ,0xaf4,"JSAtom js_get_atom_index(JSRuntime *, JSAtomStruct *)","i != 0");
          }
          uVar3 = (ulong)(uint)piVar5[3];
          piVar5 = *(int **)(*(long *)(lVar4 + 0x60) + uVar3 * 8);
        }
      }
      else {
        uVar3 = (ulong)(uint)param_2[3];
      }
      auVar8 = FUN_00140044(param_1,uVar3,1);
      uVar6 = auVar8._8_8_;
      uVar3 = auVar8._0_8_ & 0xffffffff00000000;
      uVar7 = auVar8._0_8_ & 0xffffffff;
    }
    if ((0xfffffff4 < (uint)param_3) &&
       (iVar1 = *param_2, iVar2 = iVar1 + -1, *param_2 = iVar2, iVar2 == 0 || iVar1 < 1)) {
      FUN_00141774(*(undefined8 *)(param_1 + 0x18),param_2,param_3);
    }
    param_2 = (int *)(uVar3 | uVar7);
    param_3 = uVar6;
  }
  auVar8._8_8_ = param_3;
  auVar8._0_8_ = param_2;
  return auVar8;
}

/* ===== FUN_001c585c @ 001c585c [libNexusScriptRuntime69252.so] ===== */

undefined1  [16]
FUN_001c585c(long param_1,long param_2,int param_3,undefined8 param_4,undefined8 *param_5,
            undefined4 *param_6,uint param_7)

{
  undefined8 uVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int *piVar5;
  ulong uVar6;
  ulong *puVar7;
  int *piVar8;
  long lVar9;
  undefined8 uVar10;
  long lVar11;
  int *piVar12;
  undefined1 auVar13 [16];
  
  if ((param_3 == -1) && (*(short *)(param_2 + 6) == 0x2f)) {
    piVar8 = *(int **)(param_2 + 0x30);
  }
  else {
    piVar8 = (int *)0x0;
  }
  *param_6 = 1;
  if (piVar8 == (int *)0x0) {
    FUN_00143570(param_1,"not a generator");
    return ZEXT816(6) << 0x40;
  }
  iVar2 = *piVar8;
  lVar11 = *(long *)(piVar8 + 2);
  if (iVar2 - 1U < 2) {
    piVar5 = (int *)*param_5;
    uVar10 = param_5[1];
    if (0xfffffff4 < (uint)uVar10) {
      *piVar5 = *piVar5 + 1;
    }
    if ((param_7 == 2) && (iVar2 == 1)) {
      lVar9 = *(long *)(param_1 + 0x18);
      piVar12 = *(int **)(lVar9 + 0xe0);
      if ((0xfffffff4 < (uint)*(undefined8 *)(lVar9 + 0xe8)) &&
         (iVar2 = *piVar12, iVar3 = iVar2 + -1, *piVar12 = iVar3, iVar3 == 0 || iVar2 < 1)) {
        FUN_00141774(lVar9,piVar12);
      }
      uVar4 = 1;
      *(int **)(lVar9 + 0xe0) = piVar5;
      *(undefined8 *)(lVar9 + 0xe8) = uVar10;
    }
    else {
      lVar9 = *(long *)(lVar11 + 0xa0);
      *(int **)(lVar9 + -0x10) = piVar5;
      *(undefined8 *)(lVar9 + -8) = uVar10;
      puVar7 = *(ulong **)(lVar11 + 0xa0);
      *puVar7 = (ulong)param_7;
      puVar7[1] = 0;
      uVar4 = 0;
      *(long *)(lVar11 + 0xa0) = *(long *)(lVar11 + 0xa0) + 0x10;
    }
LAB_001c5a7c:
    *(undefined4 *)(*(long *)(piVar8 + 2) + 0x2c) = uVar4;
    *piVar8 = 3;
    auVar13 = FUN_001774b4(param_1);
    *piVar8 = 1;
    if (*(int *)(*(long *)(piVar8 + 2) + 0x30) != 0) {
      FUN_0016a4cc(*(undefined8 *)(param_1 + 0x18),piVar8);
      return auVar13;
    }
    if (auVar13._8_4_ != 0) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/quickjs.c"
                ,0x4b63,
                "JSValue js_generator_next(JSContext *, JSValue, int, JSValue *, BOOL *, int)",
                "JS_VALUE_GET_TAG(func_ret) == JS_TAG_INT");
    }
    lVar11 = *(long *)(lVar11 + 0xa0);
    piVar5 = *(int **)(lVar11 + -0x10);
    uVar10 = *(undefined8 *)(lVar11 + -8);
    *(undefined4 *)(lVar11 + -0x10) = 0;
    *(undefined8 *)(lVar11 + -8) = 3;
    uVar6 = (ulong)piVar5 >> 0x20;
    if (auVar13._0_4_ == 2) {
      *piVar8 = 2;
      *param_6 = 2;
    }
    else {
      *param_6 = 0;
    }
  }
  else {
    if (iVar2 == 3) {
      FUN_00143570(param_1,"cannot invoke a running generator");
      piVar5 = (int *)0x0;
      uVar6 = 0;
      uVar10 = 6;
      goto LAB_001c5af8;
    }
    if (iVar2 != 4) {
      if (param_7 == 0) {
        uVar4 = 0;
        goto LAB_001c5a7c;
      }
      FUN_0016a4cc(*(undefined8 *)(param_1 + 0x18),piVar8);
    }
    if (param_7 == 2) {
      piVar8 = (int *)*param_5;
      uVar1 = param_5[1];
      if (0xfffffff4 < (uint)uVar1) {
        *piVar8 = *piVar8 + 1;
      }
      lVar11 = *(long *)(param_1 + 0x18);
      piVar5 = *(int **)(lVar11 + 0xe0);
      if ((0xfffffff4 < (uint)*(undefined8 *)(lVar11 + 0xe8)) &&
         (iVar2 = *piVar5, iVar3 = iVar2 + -1, *piVar5 = iVar3, iVar3 == 0 || iVar2 < 1)) {
        FUN_00141774(lVar11,piVar5);
      }
      piVar5 = (int *)0x0;
      uVar6 = 0;
      uVar10 = 6;
      *(int **)(lVar11 + 0xe0) = piVar8;
      *(undefined8 *)(lVar11 + 0xe8) = uVar1;
    }
    else if (param_7 == 1) {
      piVar5 = (int *)*param_5;
      uVar10 = param_5[1];
      if (0xfffffff4 < (uint)uVar10) {
        *piVar5 = *piVar5 + 1;
      }
      uVar6 = (ulong)piVar5 >> 0x20;
    }
    else {
      piVar5 = (int *)0x0;
      uVar6 = 0;
      uVar10 = 3;
    }
  }
LAB_001c5af8:
  auVar13._8_8_ = uVar10;
  auVar13._0_8_ = (ulong)piVar5 & 0xffffffff | uVar6 << 0x20;
  return auVar13;
}

/* ===== FUN_001cac0c @ 001cac0c [libNexusScriptRuntime69252.so] ===== */

void FUN_001cac0c(undefined4 *param_1,undefined1 *param_2,undefined4 param_3,char *param_4,
                 long param_5,uint param_6,undefined8 param_7)

{
  int iVar1;
  byte bVar2;
  long lVar3;
  undefined *puVar4;
  int iVar5;
  byte *pbVar6;
  char *pcVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  undefined4 local_194;
  byte *local_190;
  undefined8 uStack_188;
  undefined8 uStack_180;
  undefined8 local_178;
  undefined8 local_170;
  undefined8 uStack_168;
  char *local_160;
  char *pcStack_158;
  char *local_150;
  ulong uStack_148;
  ulong local_140;
  undefined8 local_138;
  undefined8 local_130;
  undefined8 uStack_128;
  undefined8 local_120;
  ulong local_118;
  undefined8 local_110;
  undefined8 uStack_108;
  undefined8 uStack_100;
  undefined8 uStack_f8;
  undefined8 local_f0;
  undefined8 uStack_e8;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  undefined8 uStack_c8;
  undefined8 uStack_c0;
  undefined8 uStack_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined8 uStack_78;
  long local_68;
  
  puVar4 = PTR_FUN_001f13c0;
  lVar3 = tpidr_el0;
  local_68 = *(long *)(lVar3 + 0x28);
  pcStack_158 = param_4 + param_5;
  uStack_108 = 0;
  local_110 = 0;
  uStack_f8 = 0;
  uStack_100 = 0;
  uStack_e8 = 0;
  local_f0 = 0;
  uStack_d8 = 0;
  uStack_e0 = 0;
  uStack_c8 = 0;
  local_d0 = 0;
  uStack_b8 = 0;
  uStack_c0 = 0;
  uStack_a8 = 0;
  local_b0 = 0;
  uStack_98 = 0;
  uStack_a0 = 0;
  uStack_88 = 0;
  local_90 = 0;
  uStack_78 = 0;
  uStack_80 = 0;
  uStack_188 = 0;
  local_190 = (byte *)0x0;
  local_178 = 0;
  uStack_180 = 0;
  uStack_168 = 0;
  local_170 = 0;
  local_118 = 0;
  local_120 = 0;
  uStack_148 = CONCAT44(param_6 >> 4,param_6) & 0x1ffffffff;
  local_140 = CONCAT44(param_6 >> 3,param_6 >> 1) & 0x100000001;
  local_138 = 0xffffffff00000001;
  local_130 = 0xffffffff;
  local_160 = param_4;
  local_150 = param_4;
  uStack_128 = param_7;
  FUN_001d2548(&local_190,param_7,PTR_FUN_001f13c0);
  FUN_001d2548(&local_120,param_7,puVar4);
  FUN_001d27c4(&local_190,param_6);
  FUN_001d27c4(&local_190,0);
  FUN_001d27c4(&local_190,0);
  local_194 = 0;
  FUN_001d26dc(&local_190,&local_194,4);
  if ((param_6 >> 5 & 1) == 0) {
    FUN_001d27c4(&local_190,8);
    local_194 = 6;
    FUN_001d26dc(&local_190,&local_194,4);
    FUN_001d27c4(&local_190,4);
    FUN_001d27c4(&local_190,7);
    local_194 = 0xfffffff5;
    FUN_001d26dc(&local_190,&local_194,4);
  }
  FUN_001d27c4(&local_190,0xb);
  FUN_001d27c4(&local_190,0);
  iVar5 = FUN_001cb05c(&local_190,0);
  if (iVar5 == 0) {
    FUN_001d27c4(&local_190,0xc);
    FUN_001d27c4(&local_190,0);
    FUN_001d27c4(&local_190,10);
    if (*local_160 == '\0') {
      if ((int)local_178 == 0) {
        if ((int)uStack_188 < 8) {
          iVar5 = 0;
        }
        else {
          iVar8 = 0;
          iVar5 = 0;
          iVar9 = 0;
          do {
            pbVar6 = local_190 + (long)iVar8 + 7;
            bVar2 = *pbVar6;
            if (0x1c < bVar2) {
                    /* WARNING: Subroutine does not return */
              __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libregexp.c"
                        ,0x6ae,"int compute_stack_size(const uint8_t *, int)","opcode < REOP_COUNT")
              ;
            }
            uVar10 = (uint)(byte)(&DAT_00114920)[(uint)bVar2];
            if ((int)uStack_188 + -7 < (int)(iVar8 + uVar10)) {
                    /* WARNING: Subroutine does not return */
              __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libregexp.c"
                        ,0x6af,"int compute_stack_size(const uint8_t *, int)",
                        "(pos + len) <= bc_buf_len");
            }
            iVar1 = iVar9;
            switch(bVar2) {
            case 0xf:
            case 0x19:
              iVar1 = iVar9 + 1;
              if ((iVar5 <= iVar9) && (iVar5 = iVar1, 0xfe < iVar9)) goto LAB_001caf84;
              break;
            case 0x10:
            case 0x1a:
              if (iVar9 < 1) {
                    /* WARNING: Subroutine does not return */
                __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libregexp.c"
                          ,0x6bc,"int compute_stack_size(const uint8_t *, int)","stack_size > 0");
              }
              iVar1 = iVar9 + -1;
              break;
            case 0x15:
              uVar10 = (uint)(byte)(&DAT_00114920)[(uint)bVar2] + (uint)*(ushort *)(pbVar6 + 1) * 4;
              break;
            case 0x16:
              uVar10 = uVar10 + (uint)*(ushort *)(pbVar6 + 1) * 8;
            }
            iVar9 = iVar1;
            iVar8 = uVar10 + iVar8;
          } while (iVar8 < (int)uStack_188 + -7);
        }
        if (-1 < iVar5) {
          local_190[1] = (byte)local_138;
          local_190[2] = (byte)iVar5;
          *(int *)(local_190 + 3) = (int)uStack_188 + -7;
          if ((long)(int)local_138 - 1U < local_118) {
            FUN_001d26dc(&local_190,local_120);
            *local_190 = *local_190 | 0x80;
          }
          FUN_001d2b08(&local_120);
          *param_2 = 0;
          *param_1 = (int)uStack_188;
          pbVar6 = local_190;
          goto LAB_001cae10;
        }
LAB_001caf84:
        pcVar7 = "too many imbricated quantifiers";
      }
      else {
        pcVar7 = "out of memory";
      }
    }
    else {
      pcVar7 = "extraneous characters at the end";
    }
    FUN_001cb300(&local_190,pcVar7);
  }
  FUN_001d2b08(&local_190);
  FUN_001d2b08(&local_120);
  FUN_001d23e4(param_2,param_3,&local_f0);
  *param_1 = 0;
  pbVar6 = (byte *)0x0;
LAB_001cae10:
  if (*(long *)(lVar3 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(pbVar6);
  }
  return;
}

/* ===== FUN_001cb4dc @ 001cb4dc [libNexusScriptRuntime69252.so] ===== */

/* WARNING: Removing unreachable block (ram,0x001cbe18) */

ushort * FUN_001cb4dc(undefined8 *param_1,undefined8 *param_2,undefined8 *param_3,ulong param_4,
                     uint *param_5,ushort *param_6,int param_7)

{
  uint *puVar1;
  int *piVar2;
  int iVar3;
  byte bVar4;
  byte bVar5;
  ushort uVar6;
  ushort uVar7;
  int iVar8;
  char cVar9;
  int iVar10;
  uint uVar11;
  bool bVar12;
  ushort *puVar13;
  int iVar14;
  uint uVar15;
  char *pcVar16;
  ushort *puVar17;
  undefined1 *puVar18;
  ulong uVar19;
  uint uVar20;
  undefined8 *puVar21;
  long lVar22;
  char *pcVar23;
  undefined8 *puVar24;
  ulong uVar25;
  ushort *puVar26;
  ulong uVar27;
  ushort *puVar28;
  ushort *puVar29;
  ushort *puVar30;
  void *__src;
  uint uVar31;
  uint *puVar32;
  
  iVar3 = *(int *)(param_1 + 2);
  puVar30 = (ushort *)param_1[1];
LAB_001cb52c:
  puVar17 = param_6;
  puVar32 = (uint *)((long)param_5 + 1);
  bVar4 = (byte)*param_5;
  if (0x1b < bVar4 - 1) {
                    /* WARNING: Subroutine does not return */
    abort();
  }
  uVar15 = (uint)bVar4;
  iVar14 = (int)param_4;
  cVar9 = (char)param_4;
  param_6 = puVar17;
  switch((uint)bVar4) {
  case 1:
    uVar15 = (uint)*(ushort *)puVar32;
    lVar22 = 3;
    goto joined_r0x001cb78c;
  case 2:
    uVar15 = *puVar32;
    lVar22 = 5;
joined_r0x001cb78c:
    if (puVar17 < puVar30) {
      if (iVar3 == 0) {
        param_6 = (ushort *)((long)puVar17 + 1);
        uVar31 = (uint)(byte)*puVar17;
      }
      else {
        param_6 = puVar17 + 1;
        uVar7 = *puVar17;
        uVar31 = (uint)uVar7;
        if ((((uVar7 & 0xfc00) == 0xd800) && (iVar3 == 2)) &&
           ((param_6 < puVar30 && (uVar6 = *param_6, (uVar6 & 0xfc00) == 0xdc00)))) {
          param_6 = puVar17 + 2;
          uVar31 = (uVar6 & 0x3ff | (uVar7 & 0x3ff) << 10) + 0x10000;
        }
      }
      if (*(int *)(param_1 + 4) != 0) {
        uVar31 = FUN_001cedf0(uVar31,*(undefined4 *)((long)param_1 + 0x24));
      }
      param_5 = (uint *)((long)param_5 + lVar22);
      if (uVar15 == uVar31) goto LAB_001cb52c;
    }
    break;
  case 3:
    if (puVar17 != puVar30) {
      if (iVar3 == 0) {
        param_6 = (ushort *)((long)puVar17 + 1);
        uVar15 = (uint)(byte)*puVar17;
      }
      else {
        param_6 = puVar17 + 1;
        uVar7 = *puVar17;
        uVar15 = (uint)uVar7;
        if ((((uVar7 & 0xfc00) == 0xd800) && (iVar3 == 2)) &&
           ((param_6 < puVar30 && (uVar6 = *param_6, (uVar6 & 0xfc00) == 0xdc00)))) {
          param_6 = puVar17 + 2;
          uVar15 = (uVar6 & 0x3ff | (uVar7 & 0x3ff) << 10) + 0x10000;
        }
      }
      bVar12 = true;
      if (((uVar15 != 10) && (uVar15 != 0xd)) && (uVar15 != 0x2028)) {
        bVar12 = uVar15 == 0x2029;
      }
      goto joined_r0x001cbc44;
    }
    break;
  case 4:
    if (puVar17 != puVar30) {
      if (iVar3 == 0) {
        param_5 = puVar32;
        param_6 = (ushort *)((long)puVar17 + 1);
      }
      else {
        puVar13 = puVar17 + 1;
        param_5 = puVar32;
        param_6 = puVar13;
        if ((((*puVar17 & 0xfc00) == 0xd800) && (iVar3 == 2)) &&
           ((puVar13 < puVar30 && (param_6 = puVar17 + 2, (puVar17[1] & 0xfc00) != 0xdc00)))) {
          param_6 = puVar13;
        }
      }
      goto LAB_001cb52c;
    }
    break;
  case 5:
    param_5 = puVar32;
    if (puVar17 == (ushort *)*param_1) goto LAB_001cb52c;
    if (*(int *)((long)param_1 + 0x1c) != 0) {
      if (iVar3 == 0) {
        uVar15 = (uint)*(byte *)((long)puVar17 + -1);
      }
      else {
        uVar7 = puVar17[-1];
        uVar15 = (uint)uVar7;
        if ((((uVar7 & 0xfc00) == 0xdc00) && (iVar3 == 2)) &&
           (((ushort *)*param_1 <= puVar17 + -2 && ((puVar17[-2] & 0xfc00) == 0xd800)))) {
          uVar15 = (uVar7 & 0x3ff | (puVar17[-2] & 0x3ff) << 10) + 0x10000;
        }
      }
LAB_001cc260:
      bVar12 = true;
      if (((uVar15 != 10) && (uVar15 != 0xd)) && (uVar15 != 0x2028)) {
        bVar12 = uVar15 == 0x2029;
      }
      param_5 = puVar32;
      if (bVar12) goto LAB_001cb52c;
    }
    break;
  case 6:
    param_5 = puVar32;
    if (puVar17 == puVar30) goto LAB_001cb52c;
    if (*(int *)((long)param_1 + 0x1c) != 0) {
      if (iVar3 == 0) {
        uVar15 = (uint)(byte)*puVar17;
      }
      else {
        uVar15 = (uint)*puVar17;
        if ((((uVar15 & 0xfc00) == 0xd800) && (iVar3 == 2)) &&
           ((puVar17 + 1 < puVar30 && ((puVar17[1] & 0xfc00) == 0xdc00)))) {
          uVar15 = (puVar17[1] & 0x3ff | (uVar15 & 0x3ff) << 10) + 0x10000;
        }
      }
      goto LAB_001cc260;
    }
    break;
  case 7:
    param_5 = (uint *)((long)puVar32 + (long)(int)*puVar32 + 4);
    goto LAB_001cb52c;
  default:
    puVar32 = (uint *)((long)param_5 + 5);
    puVar1 = (uint *)((long)puVar32 + (long)*(int *)((long)param_5 + 1));
    param_5 = puVar32;
    if (uVar15 != 9) {
      param_5 = puVar1;
      puVar1 = puVar32;
    }
    if ((ulong)param_1[8] < param_1[9] + 1) {
      uVar25 = (ulong)(param_1[8] * 3) >> 1;
      if (uVar25 < 9) {
        uVar25 = 8;
      }
      lVar22 = FUN_0015c410(param_1[5],param_1[7],param_1[6] * uVar25);
      if (lVar22 == 0) {
        return (ushort *)0xffffffffffffffff;
      }
      param_1[7] = lVar22;
      param_1[8] = uVar25;
    }
    puVar18 = (undefined1 *)(param_1[7] + param_1[6] * param_1[9]);
    param_1[9] = param_1[9] + 1;
    *puVar18 = 0;
    iVar10 = *(int *)((long)param_1 + 0x14);
    puVar18[1] = cVar9;
    *(undefined8 *)(puVar18 + 8) = 0;
    *(ushort **)(puVar18 + 0x10) = puVar17;
    *(uint **)(puVar18 + 0x18) = puVar1;
    uVar25 = (long)iVar10 * 2;
    if (iVar10 != 0) {
      puVar21 = (undefined8 *)(puVar18 + 0x20);
      puVar24 = param_2;
      if (uVar25 < 2) {
        uVar25 = 1;
      }
      do {
        uVar25 = uVar25 - 1;
        *puVar21 = *puVar24;
        puVar21 = puVar21 + 1;
        puVar24 = puVar24 + 1;
      } while (uVar25 != 0);
    }
    if (iVar14 != 0) {
      lVar22 = (long)iVar14;
      puVar21 = (undefined8 *)(puVar18 + (long)iVar10 * 0x10 + 0x20);
      puVar24 = param_3;
      do {
        lVar22 = lVar22 + -1;
        *puVar21 = *puVar24;
        puVar21 = puVar21 + 1;
        puVar24 = puVar24 + 1;
      } while (lVar22 != 0);
    }
    goto LAB_001cb52c;
  case 10:
    if (param_7 != 0) {
      return puVar17;
    }
    uVar15 = 1;
    goto LAB_001cc3e8;
  case 0xb:
  case 0xc:
    if (*(uint *)((long)param_1 + 0x14) <= (uint)*(byte *)puVar32) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libregexp.c"
                ,0x888,
                "intptr_t lre_exec_backtrack(REExecContext *, uint8_t **, StackInt *, int, const uint8_t *, const uint8_t *, BOOL)"
                ,"val < s->capture_count");
    }
    param_2[(uVar15 + (uint)*(byte *)puVar32 * 2) - 0xb] = puVar17;
    param_5 = (uint *)((long)param_5 + 2);
    goto LAB_001cb52c;
  case 0xd:
    bVar4 = *(byte *)((long)param_5 + 2);
    if (*(uint *)((long)param_1 + 0x14) <= (uint)bVar4) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libregexp.c"
                ,0x891,
                "intptr_t lre_exec_backtrack(REExecContext *, uint8_t **, StackInt *, int, const uint8_t *, const uint8_t *, BOOL)"
                ,"val2 < s->capture_count");
    }
    bVar5 = *(byte *)puVar32;
    param_5 = (uint *)((long)param_5 + 3);
    if ((uint)bVar5 <= (uint)bVar4) {
      memset(param_2 + (ulong)(uint)bVar5 * 2,0,(ulong)((uint)bVar4 - (uint)bVar5) * 0x10 + 0x10);
    }
    goto LAB_001cb52c;
  case 0xe:
    iVar10 = *(int *)((long)param_5 + 1);
    param_5 = (uint *)((long)param_5 + 5);
    lVar22 = param_3[(long)iVar14 + -1];
    param_3[(long)iVar14 + -1] = lVar22 + -1;
    if (lVar22 + -1 != 0) {
      param_5 = (uint *)((long)param_5 + (long)iVar10);
    }
    goto LAB_001cb52c;
  case 0xf:
    param_3[iVar14] = (ulong)*(uint *)((long)param_5 + 1);
    param_4 = (ulong)(iVar14 + 1);
    param_5 = (uint *)((long)param_5 + 5);
    goto LAB_001cb52c;
  case 0x10:
    param_4 = (ulong)(iVar14 - 1);
    param_5 = puVar32;
    goto LAB_001cb52c;
  case 0x11:
  case 0x12:
    if (puVar17 == (ushort *)*param_1) {
      uVar31 = 0;
      if (puVar30 <= puVar17) goto LAB_001cbc64;
LAB_001cbf08:
      if (iVar3 == 0) {
        uVar20 = (uint)(byte)*puVar17;
      }
      else {
        uVar7 = *puVar17;
        uVar20 = (uint)uVar7;
        if (((((uVar7 & 0xfc00) == 0xd800) && (iVar3 == 2)) && (puVar17 + 1 < puVar30)) &&
           ((puVar17[1] & 0xfc00) == 0xdc00)) {
          uVar20 = (puVar17[1] & 0x3ff | (uVar7 & 0x3ff) << 10) + 0x10000;
        }
      }
      uVar20 = (uint)((uVar20 - 0x30 < 10 || (uVar20 & 0xffffffdf) - 0x41 < 0x1a) || uVar20 == 0x5f)
      ;
    }
    else {
      if (iVar3 == 0) {
        uVar31 = (uint)*(byte *)((long)puVar17 + -1);
      }
      else {
        uVar7 = puVar17[-1];
        uVar31 = (uint)uVar7;
        if ((((uVar7 & 0xfc00) == 0xdc00) && (iVar3 == 2)) &&
           (((ushort *)*param_1 <= puVar17 + -2 && ((puVar17[-2] & 0xfc00) == 0xd800)))) {
          uVar31 = (uVar7 & 0x3ff | (puVar17[-2] & 0x3ff) << 10) + 0x10000;
        }
      }
      uVar31 = (uint)((uVar31 - 0x30 < 10 || (uVar31 & 0xffffffdf) - 0x41 < 0x1a) || uVar31 == 0x5f)
      ;
      if (puVar17 < puVar30) goto LAB_001cbf08;
LAB_001cbc64:
      uVar20 = 0;
    }
    param_5 = puVar32;
    if ((uVar20 ^ uVar31) == 0x12 - uVar15) goto LAB_001cb52c;
    break;
  case 0x13:
  case 0x14:
    bVar4 = *(byte *)((long)param_5 + 1);
    if ((uint)bVar4 < *(uint *)((long)param_1 + 0x14)) {
      puVar26 = (ushort *)param_2[(ulong)bVar4 * 2];
      puVar13 = (ushort *)param_2[(uint)bVar4 << 1 | 1];
      if (puVar26 == (ushort *)0x0 || puVar13 == (ushort *)0x0) {
        bVar12 = false;
      }
      else {
        puVar28 = puVar17;
        if (uVar15 == 0x13) {
          do {
            bVar12 = puVar13 > puVar26;
            if ((puVar13 <= puVar26) || (puVar30 <= puVar17)) break;
            if (iVar3 == 0) {
              puVar28 = (ushort *)((long)puVar26 + 1);
              uVar15 = (uint)(byte)*puVar26;
LAB_001cbd58:
              uVar31 = (uint)(byte)*puVar17;
              puVar26 = puVar28;
              puVar29 = (ushort *)((long)puVar17 + 1);
            }
            else {
              puVar28 = puVar26 + 1;
              uVar7 = *puVar26;
              uVar15 = (uint)uVar7;
              if (((((uVar7 & 0xfc00) == 0xd800) && (iVar3 == 2)) && (puVar28 < puVar13)) &&
                 (uVar6 = *puVar28, (uVar6 & 0xfc00) == 0xdc00)) {
                puVar28 = puVar26 + 2;
                uVar15 = (uVar6 & 0x3ff | (uVar7 & 0x3ff) << 10) + 0x10000;
              }
              if (iVar3 == 0) goto LAB_001cbd58;
              puVar29 = puVar17 + 1;
              uVar7 = *puVar17;
              uVar31 = (uint)uVar7;
              puVar26 = puVar28;
              if ((((uVar7 & 0xfc00) == 0xd800) && (iVar3 == 2)) &&
                 ((puVar29 < puVar30 && ((*puVar29 & 0xfc00) == 0xdc00)))) {
                uVar31 = (*puVar29 & 0x3ff | (uVar7 & 0x3ff) << 10) + 0x10000;
                puVar29 = puVar17 + 2;
              }
            }
            puVar17 = puVar29;
            if (*(int *)(param_1 + 4) != 0) {
              uVar15 = FUN_001cedf0(uVar15,*(undefined4 *)((long)param_1 + 0x24));
              uVar31 = FUN_001cedf0(uVar31,*(undefined4 *)((long)param_1 + 0x24));
            }
          } while (uVar15 == uVar31);
        }
        else {
          do {
            bVar12 = puVar26 <= puVar13 && puVar13 != puVar26;
            puVar17 = puVar28;
            if ((puVar26 > puVar13 || puVar13 == puVar26) || (puVar28 == (ushort *)*param_1)) break;
            if (iVar3 == 0) {
              puVar29 = (ushort *)((long)puVar13 + -1);
              uVar15 = (uint)*(byte *)puVar29;
LAB_001cbe24:
              uVar31 = (uint)*(byte *)((long)puVar28 + -1);
              puVar17 = (ushort *)((long)puVar28 + -1);
            }
            else {
              puVar29 = puVar13 + -1;
              uVar7 = *puVar29;
              uVar15 = (uint)uVar7;
              if ((((uVar7 & 0xfc00) == 0xdc00) && (iVar3 == 2)) && (puVar26 < puVar29)) {
                uVar31 = (uint)puVar13[-2];
                if ((uVar31 & 0xfc00) != 0xd800) goto joined_r0x001cbe30;
                uVar15 = (uVar7 & 0x3ff | (uVar31 & 0x3ff) << 10) + 0x10000;
                puVar29 = puVar13 + -2;
              }
              else {
joined_r0x001cbe30:
                if (iVar3 == 0) goto LAB_001cbe24;
              }
              puVar17 = puVar28 + -1;
              uVar7 = *puVar17;
              uVar31 = (uint)uVar7;
              if ((((uVar7 & 0xfc00) == 0xdc00) && (iVar3 == 2)) && ((ushort *)*param_1 < puVar17))
              {
                uVar20 = (uint)puVar28[-2];
                if ((uVar20 & 0xfc00) == 0xd800) {
                  uVar31 = (uVar7 & 0x3ff | (uVar20 & 0x3ff) << 10) + 0x10000;
                  puVar17 = puVar28 + -2;
                }
              }
            }
            if (*(int *)(param_1 + 4) != 0) {
              uVar15 = FUN_001cedf0(uVar15,*(undefined4 *)((long)param_1 + 0x24));
              uVar31 = FUN_001cedf0(uVar31,*(undefined4 *)((long)param_1 + 0x24));
            }
            puVar13 = puVar29;
            puVar28 = puVar17;
          } while (uVar15 == uVar31);
        }
      }
    }
    else {
      bVar12 = true;
    }
    puVar32 = (uint *)((long)param_5 + 2);
    param_6 = puVar17;
joined_r0x001cbc44:
    param_5 = puVar32;
    if (!bVar12) goto LAB_001cb52c;
    break;
  case 0x15:
    puVar32 = (uint *)((long)param_5 + 3);
    if (puVar17 < puVar30) {
      if (iVar3 == 0) {
        param_6 = (ushort *)((long)puVar17 + 1);
        uVar15 = (uint)(byte)*puVar17;
      }
      else {
        param_6 = puVar17 + 1;
        uVar7 = *puVar17;
        uVar15 = (uint)uVar7;
        if (((((uVar7 & 0xfc00) == 0xd800) && (iVar3 == 2)) && (param_6 < puVar30)) &&
           (uVar6 = *param_6, (uVar6 & 0xfc00) == 0xdc00)) {
          param_6 = puVar17 + 2;
          uVar15 = (uVar6 & 0x3ff | (uVar7 & 0x3ff) << 10) + 0x10000;
        }
      }
      uVar7 = *(ushort *)((long)param_5 + 1);
      if (*(int *)(param_1 + 4) != 0) {
        uVar15 = FUN_001cedf0(uVar15,*(undefined4 *)((long)param_1 + 0x24));
      }
      if (*(ushort *)puVar32 <= uVar15) {
        uVar31 = uVar7 - 1;
        uVar20 = (uint)*(ushort *)((long)param_5 + (ulong)(uVar31 * 4) + 5);
        if ((0xfffe < uVar15) && (uVar20 == 0xffff)) {
LAB_001cc35c:
          puVar32 = puVar32 + uVar7;
          bVar12 = true;
          goto LAB_001cc238;
        }
        if (uVar15 <= uVar20) {
          uVar20 = 0;
          do {
            while( true ) {
              uVar11 = uVar31 + uVar20 >> 1;
              puVar17 = (ushort *)((long)puVar32 + (ulong)(uVar11 << 2));
              if (uVar15 < *puVar17) break;
              if ((uVar15 & 0xffff) <= (uint)puVar17[1]) goto LAB_001cc35c;
              uVar20 = uVar11 + 1;
              if (uVar31 < uVar20) goto LAB_001cc178;
            }
            uVar31 = uVar11 - 1;
          } while (uVar20 <= uVar31);
        }
      }
LAB_001cc178:
      bVar12 = false;
    }
    else {
LAB_001cbc4c:
      puVar32 = (uint *)((long)param_5 + 3);
      bVar12 = false;
    }
    goto LAB_001cc238;
  case 0x16:
    puVar32 = (uint *)((long)param_5 + 3);
    if (puVar30 <= puVar17) goto LAB_001cbc4c;
    if (iVar3 == 0) {
      param_6 = (ushort *)((long)puVar17 + 1);
      uVar15 = (uint)(byte)*puVar17;
    }
    else {
      param_6 = puVar17 + 1;
      uVar7 = *puVar17;
      uVar15 = (uint)uVar7;
      if ((((uVar7 & 0xfc00) == 0xd800) && (iVar3 == 2)) &&
         ((param_6 < puVar30 && (uVar6 = *param_6, (uVar6 & 0xfc00) == 0xdc00)))) {
        param_6 = puVar17 + 2;
        uVar15 = (uVar6 & 0x3ff | (uVar7 & 0x3ff) << 10) + 0x10000;
      }
    }
    uVar7 = *(ushort *)((long)param_5 + 1);
    if (*(int *)(param_1 + 4) != 0) {
      uVar15 = FUN_001cedf0(uVar15,*(undefined4 *)((long)param_1 + 0x24));
    }
    if ((*puVar32 <= uVar15) &&
       (uVar31 = uVar7 - 1, uVar15 <= *(uint *)((long)param_5 + (ulong)(uVar31 * 8) + 7))) {
      uVar20 = 0;
      do {
        while( true ) {
          uVar11 = uVar31 + uVar20 >> 1;
          puVar1 = (uint *)((long)puVar32 + (ulong)(uVar11 << 3));
          if (uVar15 < *puVar1) break;
          if (uVar15 <= puVar1[1]) {
            puVar32 = puVar32 + (ulong)uVar7 * 2;
            bVar12 = true;
            goto LAB_001cc238;
          }
          uVar20 = uVar11 + 1;
          if (uVar31 < uVar20) goto LAB_001cc224;
        }
        uVar31 = uVar11 - 1;
      } while (uVar20 <= uVar31);
    }
LAB_001cc224:
    bVar12 = false;
LAB_001cc238:
    param_5 = puVar32;
    if (bVar12) goto LAB_001cb52c;
    break;
  case 0x17:
  case 0x18:
    iVar10 = *(int *)((long)param_5 + 1);
    if ((ulong)param_1[8] < param_1[9] + 1) {
      uVar25 = (ulong)(param_1[8] * 3) >> 1;
      if (uVar25 < 9) {
        uVar25 = 8;
      }
      lVar22 = FUN_0015c410(param_1[5],param_1[7],param_1[6] * uVar25);
      if (lVar22 == 0) {
        return (ushort *)0xffffffffffffffff;
      }
      param_1[7] = lVar22;
      param_1[8] = uVar25;
    }
    param_5 = (uint *)((long)param_5 + 5);
    pcVar16 = (char *)(param_1[7] + param_1[6] * param_1[9]);
    param_1[9] = param_1[9] + 1;
    *pcVar16 = bVar4 - 0x16;
    iVar8 = *(int *)((long)param_1 + 0x14);
    pcVar16[1] = cVar9;
    pcVar16[8] = '\0';
    pcVar16[9] = '\0';
    pcVar16[10] = '\0';
    pcVar16[0xb] = '\0';
    pcVar16[0xc] = '\0';
    pcVar16[0xd] = '\0';
    pcVar16[0xe] = '\0';
    pcVar16[0xf] = '\0';
    *(ushort **)(pcVar16 + 0x10) = puVar17;
    *(long *)(pcVar16 + 0x18) = (long)param_5 + (long)iVar10;
    uVar25 = (long)iVar8 * 2;
    if (iVar8 != 0) {
      pcVar23 = pcVar16 + 0x20;
      puVar21 = param_2;
      if (uVar25 < 2) {
        uVar25 = 1;
      }
      do {
        uVar25 = uVar25 - 1;
        *(undefined8 *)pcVar23 = *puVar21;
        pcVar23 = pcVar23 + 8;
        puVar21 = puVar21 + 1;
      } while (uVar25 != 0);
    }
    if (iVar14 != 0) {
      lVar22 = (long)iVar14;
      pcVar16 = pcVar16 + (long)iVar8 * 0x10 + 0x20;
      puVar21 = param_3;
      do {
        lVar22 = lVar22 + -1;
        *(undefined8 *)pcVar16 = *puVar21;
        pcVar16 = pcVar16 + 8;
        puVar21 = puVar21 + 1;
      } while (lVar22 != 0);
    }
    goto LAB_001cb52c;
  case 0x19:
    param_3[iVar14] = puVar17;
    param_4 = (ulong)(iVar14 + 1);
    param_5 = puVar32;
    goto LAB_001cb52c;
  case 0x1a:
    param_4 = (long)iVar14 - 1U & 0xffffffff;
    param_5 = puVar32;
    if ((ushort *)param_3[(long)iVar14 - 1U] != puVar17) goto LAB_001cb52c;
    break;
  case 0x1b:
    if (puVar17 != (ushort *)*param_1) {
      if (iVar3 == 0) {
        param_5 = puVar32;
        param_6 = (ushort *)((long)puVar17 + -1);
      }
      else {
        puVar13 = puVar17 + -1;
        param_5 = puVar32;
        param_6 = puVar13;
        if ((((iVar3 == 2) && ((*puVar13 & 0xfc00) == 0xdc00)) && ((ushort *)*param_1 < puVar13)) &&
           (param_6 = puVar17 + -2, (puVar17[-2] & 0xfc00) != 0xd800)) {
          param_6 = puVar13;
        }
      }
      goto LAB_001cb52c;
    }
    break;
  case 0x1c:
    iVar10 = *(int *)((long)param_5 + 1);
    uVar25 = 0;
    uVar27 = (ulong)*(uint *)((long)param_5 + 5);
    uVar15 = *(uint *)((long)param_5 + 9);
    do {
      puVar13 = (ushort *)
                FUN_001cb4dc(param_1,param_2,param_3,param_4,(long)param_5 + 0x11,puVar17,1);
      if (puVar13 == (ushort *)0xffffffffffffffff) goto LAB_001cc3ac;
    } while ((puVar13 != (ushort *)0x0) &&
            ((uVar25 = uVar25 + 1, puVar17 = puVar13, uVar15 == 0x7fffffff || (uVar25 < uVar15))));
    if (uVar27 <= uVar25) {
      if (uVar27 <= uVar25 && uVar25 - uVar27 != 0) {
        if ((ulong)param_1[8] < param_1[9] + 1) {
          uVar19 = (ulong)(param_1[8] * 3) >> 1;
          if (uVar19 < 9) {
            uVar19 = 8;
          }
          lVar22 = FUN_0015c410(param_1[5],param_1[7],param_1[6] * uVar19);
          if (lVar22 == 0) {
LAB_001cc3ac:
            iVar14 = 1;
            goto LAB_001cc3d0;
          }
          param_1[7] = lVar22;
          param_1[8] = uVar19;
        }
        puVar18 = (undefined1 *)(param_1[7] + param_1[6] * param_1[9]);
        param_1[9] = param_1[9] + 1;
        puVar18[1] = cVar9;
        *(ulong *)(puVar18 + 8) = uVar25 - uVar27;
        *(ushort **)(puVar18 + 0x10) = puVar17;
        *puVar18 = 3;
        iVar8 = *(int *)((long)param_1 + 0x14);
        *(uint **)(puVar18 + 0x18) = puVar32;
        uVar25 = (long)iVar8 * 2;
        if (iVar8 != 0) {
          puVar21 = (undefined8 *)(puVar18 + 0x20);
          puVar24 = param_2;
          if (uVar25 < 2) {
            uVar25 = 1;
          }
          do {
            uVar25 = uVar25 - 1;
            *puVar21 = *puVar24;
            puVar21 = puVar21 + 1;
            puVar24 = puVar24 + 1;
          } while (uVar25 != 0);
        }
        if (iVar14 == 0) {
          iVar14 = 0;
        }
        else {
          lVar22 = (long)iVar14;
          puVar21 = (undefined8 *)(puVar18 + (long)iVar8 * 0x10 + 0x20);
          puVar24 = param_3;
          do {
            lVar22 = lVar22 + -1;
            *puVar21 = *puVar24;
            puVar21 = puVar21 + 1;
            puVar24 = puVar24 + 1;
          } while (lVar22 != 0);
          iVar14 = 0;
        }
      }
      else {
        iVar14 = 0;
      }
    }
    else {
      iVar14 = 6;
    }
LAB_001cc3d0:
    if (iVar14 == 6) break;
    param_5 = (uint *)((long)param_5 + 0x11 + (long)iVar10);
    param_6 = puVar17;
    if (iVar14 != 0) {
      return (ushort *)0xffffffffffffffff;
    }
    goto LAB_001cb52c;
  }
  if (param_7 != 0) {
    return (ushort *)0x0;
  }
  uVar15 = 0;
LAB_001cc3e8:
  if (param_1[9] == 0) {
LAB_001cc5b0:
    return (ushort *)(ulong)uVar15;
  }
  lVar22 = param_1[9] + -1;
  __src = (void *)(param_1[7] + param_1[6] * lVar22 + 0x20);
LAB_001cc424:
  cVar9 = *(char *)((long)__src + -0x20);
  if (cVar9 == '\x03') {
    if (uVar15 == 0) {
      memcpy(param_2,__src,(long)*(int *)((long)param_1 + 0x14) << 4);
      param_4 = (ulong)*(byte *)((long)__src + -0x1f);
      memcpy(param_3,(void *)((long)__src + (long)*(int *)((long)param_1 + 0x14) * 0x10),
             param_4 << 3);
      piVar2 = *(int **)((long)__src + -8);
      param_6 = *(ushort **)((long)__src + -0x10);
      for (iVar14 = piVar2[3]; iVar14 != 0; iVar14 = iVar14 + -1) {
        if (iVar3 == 0) {
          puVar17 = (ushort *)((long)param_6 + -1);
        }
        else {
          puVar13 = param_6 + -1;
          puVar17 = puVar13;
          if (((iVar3 == 2) && ((*puVar13 & 0xfc00) == 0xdc00)) &&
             (((ushort *)*param_1 < puVar13 &&
              (puVar17 = param_6 + -2, (param_6[-2] & 0xfc00) != 0xd800)))) {
            puVar17 = puVar13;
          }
        }
        param_6 = puVar17;
      }
      lVar22 = *(long *)((long)__src + -0x18) + -1;
      param_5 = (uint *)((long)piVar2 + (long)*piVar2 + 0x10);
      *(long *)((long)__src + -0x18) = lVar22;
      *(ushort **)((long)__src + -0x10) = param_6;
      if (lVar22 == 0) {
        param_1[9] = param_1[9] + -1;
      }
      goto LAB_001cb52c;
    }
LAB_001cc40c:
    uVar15 = 1;
  }
  else {
    if (cVar9 == '\0') {
      if (uVar15 != 0) goto LAB_001cc40c;
      goto LAB_001cc478;
    }
    if (((uVar15 != 0) && (cVar9 == '\x01')) ||
       (uVar15 = (uint)(cVar9 == '\x02' && uVar15 == 0), uVar15 != 0)) goto LAB_001cc464;
  }
  param_1[9] = lVar22;
  lVar22 = lVar22 + -1;
  __src = (void *)((long)__src - param_1[6]);
  if (lVar22 == -1) goto LAB_001cc5b0;
  goto LAB_001cc424;
LAB_001cc464:
  if (cVar9 != '\x01') {
LAB_001cc478:
    memcpy(param_2,__src,(long)*(int *)((long)param_1 + 0x14) << 4);
  }
  param_4 = (ulong)*(byte *)((long)__src + -0x1f);
  param_6 = *(ushort **)((long)__src + -0x10);
  param_5 = *(uint **)((long)__src + -8);
  memcpy(param_3,(void *)((long)__src + (long)*(int *)((long)param_1 + 0x14) * 0x10),param_4 << 3);
  param_1[9] = param_1[9] + -1;
  goto LAB_001cb52c;
}

/* ===== FUN_001cc65c @ 001cc65c [libNexusScriptRuntime69252.so] ===== */

void FUN_001cc65c(long param_1,int param_2)

{
  byte *pbVar1;
  byte bVar2;
  ushort uVar3;
  uint uVar4;
  uint *puVar5;
  uint *puVar6;
  uint uVar7;
  long lVar8;
  byte *pbVar9;
  ulong uVar10;
  
  if (param_2 < 8) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libregexp.c"
              ,0x9b6,"void lre_byte_swap(uint8_t *, int)","bc_buf_len > RE_HEADER_LEN");
  }
  uVar7 = *(uint *)(param_1 + 3);
  if (uVar7 != param_2 - 7U) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libregexp.c"
              ,0x9bb,"void lre_byte_swap(uint8_t *, int)","len == bc_buf_len - RE_HEADER_LEN");
  }
  uVar7 = (uVar7 & 0xff00ff00) >> 8 | (uVar7 & 0xff00ff) << 8;
  pbVar1 = (byte *)(param_1 + param_2);
  pbVar9 = (byte *)(param_1 + 7);
  *(uint *)(param_1 + 3) = uVar7 >> 0x10 | uVar7 << 0x10;
  if (7 < param_2) {
    do {
      puVar5 = (uint *)(pbVar9 + 1);
      bVar2 = *pbVar9;
      if (0x1c < bVar2) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libregexp.c"
                  ,0x9c1,"void lre_byte_swap(uint8_t *, int)","opcode < REOP_COUNT");
      }
      uVar10 = (ulong)(byte)(&DAT_00114920)[bVar2] - 1;
      if (pbVar1 < (byte *)((long)puVar5 + uVar10)) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libregexp.c"
                  ,0x9c4,"void lre_byte_swap(uint8_t *, int)","pc + len <= bc_buf + bc_buf_len");
      }
      switch(bVar2) {
      case 1:
        *(ushort *)puVar5 = (ushort)*puVar5 >> 8 | (ushort)(((ushort)*puVar5 & 0xff00ff) << 8);
        break;
      case 2:
      case 7:
      case 8:
      case 9:
      case 0xe:
      case 0xf:
      case 0x17:
      case 0x18:
        uVar7 = (*puVar5 & 0xff00ff00) >> 8 | (*puVar5 & 0xff00ff) << 8;
        *puVar5 = uVar7 >> 0x10 | uVar7 << 0x10;
        break;
      case 0x15:
        uVar3 = *(ushort *)(pbVar9 + 1);
        uVar10 = (ulong)uVar3;
        puVar5 = (uint *)(pbVar9 + 3);
        *(ushort *)(pbVar9 + 1) = uVar3 >> 8 | (ushort)((uVar3 & 0xff00ff) << 8);
        if (uVar3 != 0) {
          uVar7 = (uint)uVar3 << 1;
          if (uVar7 < 2) {
            uVar7 = 1;
          }
          do {
            uVar7 = uVar7 - 1;
            puVar6 = (uint *)((long)puVar5 + 2);
            *(ushort *)puVar5 = (ushort)*puVar5 >> 8 | (ushort)(((ushort)*puVar5 & 0xff00ff) << 8);
            puVar5 = puVar6;
          } while (uVar7 != 0);
LAB_001cc780:
          uVar10 = 0;
          puVar5 = puVar6;
        }
        break;
      case 0x16:
        uVar3 = *(ushort *)(pbVar9 + 1);
        uVar10 = (ulong)uVar3;
        puVar5 = (uint *)(pbVar9 + 3);
        *(ushort *)(pbVar9 + 1) = uVar3 >> 8 | (ushort)((uVar3 & 0xff00ff) << 8);
        if (uVar3 != 0) {
          uVar7 = (uint)uVar3 << 1;
          if (uVar7 < 2) {
            uVar7 = 1;
          }
          do {
            uVar7 = uVar7 - 1;
            uVar4 = (*puVar5 & 0xff00ff00) >> 8 | (*puVar5 & 0xff00ff) << 8;
            puVar6 = puVar5 + 1;
            *puVar5 = uVar4 >> 0x10 | uVar4 << 0x10;
            puVar5 = puVar6;
          } while (uVar7 != 0);
          goto LAB_001cc780;
        }
        break;
      case 0x1c:
        lVar8 = 0;
        do {
          uVar7 = (*(uint *)((long)puVar5 + lVar8) & 0xff00ff00) >> 8 |
                  (*(uint *)((long)puVar5 + lVar8) & 0xff00ff) << 8;
          *(uint *)((long)puVar5 + lVar8) = uVar7 >> 0x10 | uVar7 << 0x10;
          lVar8 = lVar8 + 4;
        } while ((int)lVar8 != 0x10);
        uVar10 = 0;
        puVar5 = (uint *)((long)puVar5 + lVar8);
      }
      pbVar9 = (byte *)((long)puVar5 + (long)(int)uVar10);
    } while (pbVar9 < pbVar1);
  }
  if (pbVar9 == pbVar1) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libregexp.c"
            ,0x9fe,"void lre_byte_swap(uint8_t *, int)","pc == end");
}

/* ===== FUN_001d07e4 @ 001d07e4 [libNexusScriptRuntime69252.so] ===== */

void FUN_001d07e4(undefined4 *param_1,int param_2)

{
  long lVar1;
  uint *puVar2;
  uint uVar3;
  uint uVar4;
  code *pcVar5;
  uint uVar6;
  long lVar7;
  undefined4 *puVar8;
  ulong uVar9;
  int iVar10;
  int iVar11;
  undefined4 *puVar12;
  long lVar13;
  long lVar14;
  undefined8 uVar15;
  undefined4 uVar16;
  uint uVar17;
  uint uVar18;
  int iVar19;
  ulong uVar20;
  uint uVar21;
  uint uVar22;
  uint uVar23;
  uint uVar24;
  uint uVar25;
  uint uVar26;
  uint uVar27;
  uint uVar28;
  uint uVar29;
  uint uVar30;
  uint local_10c;
  ulong local_100;
  undefined8 local_f8;
  undefined8 local_f0;
  code *pcStack_e8;
  uint local_e0;
  uint uStack_dc;
  long local_d8;
  undefined8 local_d0;
  code *pcStack_c8;
  undefined8 local_c0;
  undefined4 *local_b8;
  undefined8 local_b0;
  code *pcStack_a8;
  undefined8 local_a0;
  long local_98;
  undefined8 local_90;
  code *pcStack_88;
  uint local_7c [3];
  long local_70;
  
  lVar7 = tpidr_el0;
  local_70 = *(long *)(lVar7 + 0x28);
  uVar15 = *(undefined8 *)(param_1 + 4);
  local_c0 = 0;
  local_b8 = (undefined4 *)0x0;
  local_a0 = 0;
  local_98 = 0;
  local_e0 = 0;
  uStack_dc = 0;
  local_d8 = 0;
  local_100 = 0;
  local_f8 = 0;
  pcVar5 = FUN_001cf2d8;
  if (*(code **)(param_1 + 6) != (code *)0x0) {
    pcVar5 = *(code **)(param_1 + 6);
  }
  uVar16 = 4;
  if (param_2 == 0) {
    uVar16 = 1;
  }
  local_f0 = uVar15;
  pcStack_e8 = pcVar5;
  local_d0 = uVar15;
  pcStack_c8 = pcVar5;
  local_b0 = uVar15;
  pcStack_a8 = pcVar5;
  local_90 = uVar15;
  pcStack_88 = pcVar5;
  iVar10 = FUN_001d0df8(&local_c0,uVar16);
  puVar12 = local_b8;
  if (iVar10 == 0) {
    iVar10 = (int)local_c0;
    iVar11 = FUN_001cf400(&local_a0,local_b8,local_c0 & 0xffffffff,*(undefined8 *)(param_1 + 2),
                          *param_1,1);
    if (iVar11 == 0) {
      uVar17 = iVar10 + 2;
      if (local_c0._4_4_ < (int)uVar17) {
        iVar11 = local_c0._4_4_ * 3;
        if (iVar11 < 0) {
          iVar11 = iVar11 + 1;
        }
        uVar24 = uVar17;
        if ((int)uVar17 <= iVar11 >> 1) {
          uVar24 = iVar11 >> 1;
        }
        puVar12 = (undefined4 *)
                  (*pcStack_a8)(local_b0,puVar12,
                                -(ulong)(uVar24 >> 0x1f) & 0xfffffffc00000000 | (ulong)uVar24 << 2);
        if (puVar12 == (undefined4 *)0x0) goto LAB_001d0d0c;
        local_c0 = CONCAT44(uVar24,(int)local_c0);
        local_b8 = puVar12;
      }
      puVar12 = local_b8;
      memmove(local_b8 + 1,local_b8,(long)iVar10 << 2);
      puVar8 = local_b8;
      *puVar12 = 0;
      puVar12[iVar10 + 1] = 0xffffffff;
      if (iVar10 < 0) {
        iVar10 = 0;
      }
      else {
        uVar20 = 0;
        iVar10 = 0;
        iVar11 = 1;
        do {
          iVar19 = (int)uVar20;
          if (puVar12[iVar19] != puVar12[iVar11]) {
            uVar9 = (long)iVar19;
            lVar1 = ((long)iVar19 << 0x20) + 0x100000000;
            do {
              lVar14 = lVar1;
              uVar20 = uVar9;
              if ((long)(int)uVar17 <= (long)(uVar20 + 3)) break;
              uVar9 = uVar20 + 2;
              lVar1 = lVar14 + 0x200000000;
            } while (puVar12[uVar20 + 1] == puVar12[uVar20 + 2]);
            lVar1 = (long)iVar10;
            iVar10 = iVar10 + 2;
            puVar12[lVar1] = puVar12[iVar19];
            (puVar12 + lVar1)[1] = *(int *)((long)puVar12 + (lVar14 >> 0x1e));
          }
          iVar11 = (int)uVar20 + 3;
          uVar20 = (ulong)((int)uVar20 + 2);
        } while (iVar11 < (int)uVar17);
      }
      local_c0 = CONCAT44((int)(local_c0 >> 0x20),iVar10);
      iVar10 = FUN_001cf400(&local_100,local_b8,iVar10,*(undefined8 *)(param_1 + 2),*param_1,1);
      lVar1 = local_98;
      if (iVar10 == 0) {
        if ((uint)local_a0 != 0) {
          uVar25 = 0;
          uVar17 = 0;
          uVar30 = 0x1a;
          uVar23 = 0x41;
          uVar26 = 0x209a30;
          uVar24 = 0xffffffff;
          lVar14 = local_d8;
          uVar22 = 0xffffffff;
          do {
            uVar27 = *(uint *)(lVar1 + (ulong)uVar17 * 4);
            uVar6 = *(uint *)(lVar1 + (ulong)(uVar17 | 1) * 4);
            local_10c = local_e0;
            uVar18 = uStack_dc;
            uVar21 = uVar22;
            if (uVar27 < uVar6) {
              do {
                if ((uVar27 < uVar23) || (uVar23 + uVar30 <= uVar27)) {
                  do {
                    uVar25 = uVar25 + 1;
                    if (0x171 < uVar25) {
                      _local_e0 = CONCAT44(uVar18,local_10c);
                      local_d8 = lVar14;
                    /* WARNING: Subroutine does not return */
                      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libunicode.c"
                                ,0x593,"int cr_regexp_canonicalize(CharRange *, BOOL)",
                                "idx < countof(case_conv_table1)");
                    }
                    uVar26 = (&DAT_001149a4)[uVar25];
                    uVar23 = uVar26 >> 0xf;
                    uVar30 = uVar26 >> 8 & 0x7f;
                  } while (uVar27 < uVar23 || uVar23 + uVar30 <= uVar27);
                }
                if (param_2 == 0) {
                  if (uVar27 < 0x80) {
                    uVar29 = uVar27 - 0x20;
                    if (0x19 < uVar27 - 0x61) {
                      uVar29 = uVar27;
                    }
                  }
                  else {
                    iVar10 = FUN_001ce85c(local_7c,uVar27,0,uVar25,uVar26);
                    uVar29 = local_7c[0];
                    if (local_7c[0] < 0x80 || iVar10 != 1) {
                      uVar29 = uVar27;
                    }
                  }
                }
                else {
                  iVar10 = FUN_001ce85c(local_7c,uVar27,2,uVar25,uVar26);
                  uVar29 = local_7c[0];
                  if (iVar10 != 1) {
                    if (uVar27 == 0xfb06) {
                      uVar29 = 0xfb05;
                    }
                    else if (uVar27 == 0x1fd3) {
                      uVar29 = 0x390;
                    }
                    else {
                      uVar29 = 0x3b0;
                      if (uVar27 != 0x1fe3) {
                        uVar29 = uVar27;
                      }
                    }
                  }
                }
                uVar28 = uVar29;
                uVar21 = uVar29;
                if ((uVar22 != 0xffffffff) && (uVar28 = uVar24, uVar21 = uVar22, uVar24 != uVar29))
                {
                  uVar3 = local_10c + 2;
                  lVar13 = lVar14;
                  uVar4 = uVar18;
                  if ((int)uVar18 < (int)uVar3) {
                    iVar10 = uVar18 * 3;
                    if (iVar10 < 0) {
                      iVar10 = iVar10 + 1;
                    }
                    uVar4 = uVar3;
                    if ((int)uVar3 <= iVar10 >> 1) {
                      uVar4 = iVar10 >> 1;
                    }
                    lVar13 = (*pcVar5)(uVar15,lVar14,
                                       -(ulong)(uVar4 >> 0x1f) & 0xfffffffc00000000 |
                                       (ulong)uVar4 << 2);
                    uVar28 = uVar29;
                    uVar21 = uVar29;
                    if (lVar13 == 0) goto LAB_001d0a50;
                  }
                  uVar18 = uVar4;
                  lVar14 = lVar13;
                  puVar2 = (uint *)(lVar14 + (long)(int)local_10c * 4);
                  *puVar2 = uVar22;
                  puVar2[1] = uVar24;
                  uVar28 = uVar29;
                  uVar21 = uVar29;
                  local_10c = uVar3;
                }
LAB_001d0a50:
                uVar24 = uVar28 + 1;
                uVar27 = uVar27 + 1;
                uVar22 = uVar21;
              } while (uVar27 != uVar6);
            }
            uStack_dc = uVar18;
            uVar17 = uVar17 + 2;
            local_e0 = local_10c;
            uVar22 = uVar21;
          } while (uVar17 < (uint)local_a0);
          local_d8 = lVar14;
          if (uVar21 != 0xffffffff) {
            uVar17 = local_10c + 2;
            uVar25 = uStack_dc;
            if ((int)uStack_dc < (int)uVar17) {
              iVar10 = uStack_dc * 3;
              if (iVar10 < 0) {
                iVar10 = iVar10 + 1;
              }
              if ((int)uVar17 <= iVar10 >> 1) {
                uVar17 = iVar10 >> 1;
              }
              lVar14 = (*pcVar5)(uVar15,lVar14,
                                 -(ulong)(uVar17 >> 0x1f) & 0xfffffffc00000000 | (ulong)uVar17 << 2)
              ;
              uVar25 = uVar17;
              if (lVar14 == 0) goto LAB_001d0d0c;
            }
            uStack_dc = uVar25;
            local_d8 = lVar14;
            local_e0 = local_10c + 2;
            *(uint *)(local_d8 + (long)(int)local_10c * 4) = uVar21;
            *(uint *)(local_d8 + (long)(int)(local_10c + 1) * 4) = uVar24;
          }
        }
        FUN_001d1118(&local_e0);
        lVar1 = local_d8;
        uVar15 = local_f8;
        *param_1 = 0;
        iVar10 = FUN_001cf400(param_1,local_d8,local_e0,local_f8,local_100 & 0xffffffff,0);
        if (iVar10 == 0) {
          (*pcStack_88)(local_90,local_98,0);
          (*pcStack_a8)(local_b0,puVar8,0);
          (*pcStack_c8)(local_d0,lVar1,0);
          (*pcStack_e8)(local_f0,uVar15,0);
          uVar15 = 0;
          goto LAB_001d0d50;
        }
      }
    }
  }
LAB_001d0d0c:
  (*pcStack_88)(local_90,local_98,0);
  (*pcStack_a8)(local_b0,local_b8,0);
  (*pcStack_c8)(local_d0,local_d8,0);
  (*pcStack_e8)(local_f0,local_f8,0);
  uVar15 = 0xffffffff;
LAB_001d0d50:
  if (*(long *)(lVar7 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar15);
  }
  return;
}

/* ===== FUN_001d19e0 @ 001d19e0 [libNexusScriptRuntime69252.so] ===== */

/* WARNING: Removing unreachable block (ram,0x001d1b7c) */
/* WARNING: Removing unreachable block (ram,0x001d1db8) */

undefined4 FUN_001d19e0(uint *param_1)

{
  uint *puVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  long lVar5;
  int iVar6;
  code *pcVar7;
  code *pcVar8;
  long lVar9;
  int *piVar10;
  ulong uVar11;
  ulong uVar12;
  ulong uVar13;
  undefined4 uVar14;
  code **ppcVar15;
  uint uVar16;
  undefined8 uVar17;
  code *pcVar18;
  code *pcVar19;
  int *piVar20;
  int local_f8;
  uint local_f0 [2];
  code *apcStack_e8 [15];
  long local_70;
  
  lVar5 = tpidr_el0;
  local_70 = *(long *)(lVar5 + 0x28);
  local_f8 = -0x38;
  uVar16 = 0;
  piVar20 = (int *)register0x00000008;
LAB_001d1a6c:
  do {
    while( true ) {
      lVar9 = (long)local_f8;
      if ((local_f8 < 0) && (local_f8 = local_f8 + 8, local_f8 < 1)) {
        piVar10 = (int *)((long)local_f0 + lVar9 + -0x20);
      }
      else {
        piVar10 = piVar20;
        piVar20 = piVar20 + 2;
      }
      iVar6 = *piVar10;
      if (iVar6 != 6) break;
      if ((int)uVar16 < 1) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libunicode.c"
                  ,0x603,"int unicode_prop_ops(CharRange *, ...)","stack_len >= 1");
      }
      iVar6 = FUN_001cf66c(local_f0 + (ulong)(uVar16 - 1) * 8);
      if (iVar6 != 0) goto LAB_001d1d3c;
    }
    switch(iVar6) {
    case 0:
      if (3 < (int)uVar16) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libunicode.c"
                  ,0x5da,"int unicode_prop_ops(CharRange *, ...)","stack_len < POP_STACK_LEN_MAX");
      }
      lVar9 = (long)local_f8;
      if ((local_f8 < 0) && (local_f8 = local_f8 + 8, local_f8 < 1)) {
        piVar10 = (int *)((long)local_f0 + lVar9 + -0x20);
      }
      else {
        piVar10 = piVar20;
        piVar20 = piVar20 + 2;
      }
      lVar9 = (long)(int)uVar16;
      iVar6 = *piVar10;
      puVar1 = local_f0 + lVar9 * 8;
      pcVar19 = *(code **)(param_1 + 4);
      pcVar8 = *(code **)(param_1 + 6);
      puVar1[0] = 0;
      puVar1[1] = 0;
      apcStack_e8[lVar9 * 4] = (code *)0x0;
      pcVar18 = FUN_001cf2d8;
      if (pcVar8 != (code *)0x0) {
        pcVar18 = pcVar8;
      }
      apcStack_e8[lVar9 * 4 + 1] = pcVar19;
      apcStack_e8[lVar9 * 4 + 2] = pcVar18;
      iVar6 = FUN_001d12e8(puVar1,iVar6);
      break;
    case 1:
      if (3 < (int)uVar16) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libunicode.c"
                  ,0x5e1,"int unicode_prop_ops(CharRange *, ...)","stack_len < POP_STACK_LEN_MAX");
      }
      lVar9 = (long)local_f8;
      if ((local_f8 < 0) && (local_f8 = local_f8 + 8, local_f8 < 1)) {
        piVar10 = (int *)((long)local_f0 + lVar9 + -0x20);
      }
      else {
        piVar10 = piVar20;
        piVar20 = piVar20 + 2;
      }
      iVar6 = *piVar10;
      lVar9 = (long)(int)uVar16;
      puVar1 = local_f0 + lVar9 * 8;
      pcVar19 = *(code **)(param_1 + 4);
      pcVar8 = *(code **)(param_1 + 6);
      puVar1[0] = 0;
      puVar1[1] = 0;
      apcStack_e8[lVar9 * 4] = (code *)0x0;
      pcVar18 = FUN_001cf2d8;
      if (pcVar8 != (code *)0x0) {
        pcVar18 = pcVar8;
      }
      apcStack_e8[lVar9 * 4 + 1] = pcVar19;
      apcStack_e8[lVar9 * 4 + 2] = pcVar18;
      iVar6 = FUN_001d1ea4(puVar1,iVar6);
      break;
    case 2:
      if (3 < (int)uVar16) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libunicode.c"
                  ,0x5e8,"int unicode_prop_ops(CharRange *, ...)","stack_len < POP_STACK_LEN_MAX");
      }
      lVar9 = (long)local_f8;
      if ((local_f8 < 0) && (local_f8 = local_f8 + 8, local_f8 < 1)) {
        piVar10 = (int *)((long)local_f0 + lVar9 + -0x20);
      }
      else {
        piVar10 = piVar20;
        piVar20 = piVar20 + 2;
      }
      iVar6 = *piVar10;
      lVar9 = (long)(int)uVar16;
      puVar1 = local_f0 + lVar9 * 8;
      pcVar19 = *(code **)(param_1 + 4);
      pcVar8 = *(code **)(param_1 + 6);
      puVar1[0] = 0;
      puVar1[1] = 0;
      apcStack_e8[lVar9 * 4] = (code *)0x0;
      pcVar18 = FUN_001cf2d8;
      if (pcVar8 != (code *)0x0) {
        pcVar18 = pcVar8;
      }
      apcStack_e8[lVar9 * 4 + 1] = pcVar19;
      apcStack_e8[lVar9 * 4 + 2] = pcVar18;
      iVar6 = FUN_001d0df8(puVar1,iVar6);
      break;
    case 3:
    case 4:
    case 5:
      uVar4 = uVar16 - 1;
      uVar13 = (ulong)uVar4;
      if (uVar4 == 0 || (int)uVar16 < 1) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libunicode.c"
                  ,0x5f3,"int unicode_prop_ops(CharRange *, ...)","stack_len >= 2");
      }
      if (3 < (int)uVar16) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libunicode.c"
                  ,0x5f4,"int unicode_prop_ops(CharRange *, ...)","stack_len < POP_STACK_LEN_MAX");
      }
      pcVar19 = *(code **)(param_1 + 4);
      uVar11 = (ulong)(uVar16 - 2);
      uVar12 = (ulong)uVar16;
      puVar1 = local_f0 + uVar12 * 8;
      pcVar8 = apcStack_e8[uVar13 * 4];
      uVar2 = local_f0[uVar13 * 8];
      pcVar18 = FUN_001cf2d8;
      if (*(code **)(param_1 + 6) != (code *)0x0) {
        pcVar18 = *(code **)(param_1 + 6);
      }
      pcVar7 = apcStack_e8[uVar11 * 4];
      uVar3 = local_f0[uVar11 * 8];
      puVar1[0] = 0;
      puVar1[1] = 0;
      apcStack_e8[uVar12 * 4] = (code *)0x0;
      apcStack_e8[uVar12 * 4 + 1] = pcVar19;
      apcStack_e8[uVar12 * 4 + 2] = pcVar18;
      iVar6 = FUN_001cf400(puVar1,pcVar7,uVar3,pcVar8,uVar2,iVar6 + -3);
      if (iVar6 != 0) {
        uVar16 = uVar16 + 1;
        goto LAB_001d1d3c;
      }
      (*apcStack_e8[uVar11 * 4 + 2])(apcStack_e8[uVar11 * 4 + 1],apcStack_e8[uVar11 * 4],0);
      (*apcStack_e8[uVar13 * 4 + 2])(apcStack_e8[uVar13 * 4 + 1],apcStack_e8[uVar13 * 4],0);
      uVar17 = *(undefined8 *)puVar1;
      pcVar19 = apcStack_e8[uVar12 * 4 + 2];
      pcVar18 = apcStack_e8[uVar12 * 4 + 1];
      apcStack_e8[uVar11 * 4] = apcStack_e8[uVar12 * 4];
      *(undefined8 *)(local_f0 + uVar11 * 8) = uVar17;
      apcStack_e8[uVar11 * 4 + 2] = pcVar19;
      apcStack_e8[uVar11 * 4 + 1] = pcVar18;
      uVar16 = uVar4;
      goto LAB_001d1a6c;
    default:
                    /* WARNING: Subroutine does not return */
      abort();
    case 7:
      if (uVar16 != 1) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libunicode.c"
                  ,0x60e,"int unicode_prop_ops(CharRange *, ...)","stack_len == 1");
      }
      if ((int)param_1[1] < (int)local_f0[0]) {
        iVar6 = param_1[1] * 3;
        if (iVar6 < 0) {
          iVar6 = iVar6 + 1;
        }
        uVar16 = local_f0[0];
        if ((int)local_f0[0] <= iVar6 >> 1) {
          uVar16 = iVar6 >> 1;
        }
        lVar9 = (**(code **)(param_1 + 6))
                          (*(undefined8 *)(param_1 + 4),*(undefined8 *)(param_1 + 2),
                           -(ulong)(uVar16 >> 0x1f) & 0xfffffffc00000000 | (ulong)uVar16 << 2);
        if (lVar9 != 0) {
          *(long *)(param_1 + 2) = lVar9;
          param_1[1] = uVar16;
          goto LAB_001d1d08;
        }
        uVar14 = 0xffffffff;
      }
      else {
LAB_001d1d08:
        memcpy(*(void **)(param_1 + 2),apcStack_e8[0],(long)(int)local_f0[0] << 2);
        uVar14 = 0;
        *param_1 = local_f0[0];
      }
      (*apcStack_e8[2])(apcStack_e8[1],apcStack_e8[0],0);
      goto LAB_001d1d70;
    }
    uVar16 = uVar16 + 1;
    if (iVar6 != 0) {
LAB_001d1d3c:
      if (0 < (int)uVar16) {
        uVar13 = (ulong)uVar16;
        ppcVar15 = apcStack_e8 + 2;
        do {
          (**ppcVar15)(ppcVar15[-1],ppcVar15[-2],0);
          ppcVar15 = ppcVar15 + 4;
          uVar13 = uVar13 - 1;
        } while (uVar13 != 0);
      }
      uVar14 = 0xffffffff;
LAB_001d1d70:
      if (*(long *)(lVar5 + 0x28) == local_70) {
        return uVar14;
      }
                    /* WARNING: Subroutine does not return */
      __stack_chk_fail();
    }
  } while( true );
}

/* ===== FUN_001d3a6c @ 001d3a6c [libNexusScriptRuntime69252.so] ===== */

ulong FUN_001d3a6c(undefined8 *param_1,ulong param_2,uint param_3,ulong param_4,uint param_5)

{
  bool bVar1;
  uint uVar2;
  undefined4 uVar3;
  undefined8 uVar4;
  size_t __n;
  long lVar5;
  void *__dest;
  long lVar6;
  long lVar7;
  uint uVar8;
  ulong uVar9;
  ulong *puVar10;
  uint uVar11;
  long lVar12;
  ulong uVar13;
  ulong uVar14;
  ulong uVar15;
  
  uVar11 = param_3 >> 5 & 0x3f;
  uVar2 = 0x3d;
  if (uVar11 != 0x3f) {
    uVar2 = 0x3c - uVar11;
  }
  lVar6 = 1L << ((ulong)uVar2 & 0x3f);
  if ((param_3 >> 4 & 1) == 0) {
    uVar15 = param_2;
    if (((long)param_1[2] < 3 - lVar6) && ((param_3 >> 3 & 1) != 0)) {
      if (param_2 == 0x3fffffffffffffff) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
                  ,0x20a,"int __bf_round(bf_t *, limb_t, bf_flags_t, limb_t, int)",
                  "prec1 != BF_PREC_INF");
      }
      uVar15 = lVar6 + -3 + param_2 + param_1[2];
    }
  }
  else {
    uVar15 = 0x3fffffffffffffff;
    if (param_2 != 0x3fffffffffffffff) {
      uVar15 = param_1[2] + param_2;
    }
  }
  uVar2 = param_3 & 7;
  if (uVar2 == 6) {
    bVar1 = true;
    uVar9 = 1;
  }
  else {
    uVar9 = uVar15;
    if ((long)uVar15 < 0) {
      uVar9 = 0xffffffffffffffff;
    }
    lVar5 = (-2 - uVar9) + param_4 * 0x40;
    if (lVar5 < 0) {
      bVar1 = false;
LAB_001d3b8c:
      uVar9 = 0;
    }
    else {
      lVar5 = lVar5 >> 6;
      uVar11 = ((uint)(-2 - uVar9) & 0x3f) + 1;
      uVar9 = 0xffffffffffffffff;
      if (uVar11 != 0x40) {
        uVar9 = ~(-1L << ((ulong)uVar11 & 0x3f));
      }
      if ((*(ulong *)(param_1[4] + lVar5 * 8) & uVar9) == 0) {
        do {
          bVar1 = 0 < lVar5;
          if (lVar5 < 1) goto LAB_001d3b8c;
          lVar7 = lVar5 * 8;
          lVar5 = lVar5 + -1;
        } while (*(long *)(param_1[4] + -8 + lVar7) == 0);
      }
      uVar9 = 1;
      bVar1 = true;
    }
  }
  lVar7 = param_4 * 0x40;
  uVar13 = 0;
  __dest = (void *)param_1[4];
  lVar5 = lVar7 + ~uVar15;
  if ((-1 < lVar5) && (uVar14 = lVar5 >> 6, uVar14 < param_4)) {
    uVar13 = *(ulong *)((long)__dest + uVar14 * 8) >> (~uVar15 & 0x3f) & 1;
  }
  if (6 < uVar2) {
                    /* WARNING: Subroutine does not return */
    abort();
  }
  uVar8 = (uint)(uVar13 | uVar9);
  uVar11 = 0;
  switch(uVar2) {
  case 0:
    if (uVar13 == 0) {
      bVar1 = true;
    }
    uVar11 = (uint)uVar13;
    if (!bVar1) {
      uVar14 = lVar7 - uVar15;
      uVar11 = 0;
      if ((-1 < (long)uVar14) && ((ulong)((long)uVar14 >> 6) < param_4)) {
        uVar11 = (uint)(*(ulong *)((long)__dest + ((long)uVar14 >> 6) * 8) >> (uVar14 & 0x3f)) & 1;
      }
    }
    break;
  case 1:
    break;
  default:
    uVar11 = uVar8;
    if (*(uint *)(param_1 + 1) != (uint)(uVar2 == 2)) {
      uVar11 = 0;
    }
    break;
  case 4:
  case 6:
    uVar11 = (uint)uVar13;
    break;
  case 5:
    uVar11 = uVar8;
  }
  if ((uVar13 | uVar9) != 0) {
    param_5 = param_5 | 0x10;
  }
  if ((long)uVar15 < 1) {
    if (uVar11 == 0) {
LAB_001d3df0:
      uVar3 = *(undefined4 *)(param_1 + 1);
      if (param_1[3] != 0) {
        uVar4 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,__dest,0);
        param_1[3] = 0;
        param_1[4] = uVar4;
      }
      lVar6 = -0x8000000000000000;
      *(undefined4 *)(param_1 + 1) = uVar3;
    }
    else {
      if ((param_1[3] != 1) &&
         (lVar6 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,__dest,8),
         lVar6 != 0)) {
        param_1[3] = 1;
        param_1[4] = lVar6;
      }
      *(undefined8 *)param_1[4] = 0x8000000000000000;
      lVar6 = (param_1[2] - uVar15) + 1;
    }
    param_5 = param_5 | 0x18;
    param_1[2] = lVar6;
  }
  else {
    if (uVar11 != 0) {
      uVar9 = (long)(lVar7 - uVar15) >> 6;
      if (uVar9 < param_4) {
        uVar13 = 1L << (lVar7 - uVar15 & 0x3f);
        lVar5 = param_4 - uVar9;
        puVar10 = (ulong *)((long)__dest + uVar9 * 8);
        do {
          uVar14 = *puVar10;
          *puVar10 = uVar14 + uVar13;
          if (!CARRY8(uVar14,uVar13)) goto LAB_001d3cd8;
          lVar5 = lVar5 + -1;
          puVar10 = puVar10 + 1;
          uVar13 = 1;
        } while (lVar5 != 0);
      }
      lVar5 = param_4 - 1;
      if ((long)uVar9 <= lVar5) {
        uVar13 = 1;
        do {
          lVar12 = lVar5 * 8;
          lVar5 = lVar5 + -1;
          uVar14 = *(ulong *)((long)__dest + lVar12);
          *(ulong *)((long)__dest + lVar12) = uVar14 >> 1 | uVar13 << 0x3f;
          uVar13 = uVar14;
        } while ((long)uVar9 <= lVar5);
      }
      param_1[2] = param_1[2] + 1;
    }
LAB_001d3cd8:
    if ((long)param_1[2] < 3 - lVar6) {
      if ((param_3 >> 3 & 1) == 0) goto LAB_001d3df0;
      param_5 = param_5 >> 1 & 8 | param_5;
    }
    if (lVar6 < (long)param_1[2]) {
      uVar15 = FUN_001dc87c(param_1,*(undefined4 *)(param_1 + 1));
      return uVar15;
    }
    uVar15 = lVar7 - uVar15;
    if ((long)uVar15 < 0) {
      lVar6 = 0;
    }
    else {
      lVar6 = (long)uVar15 >> 6;
      if ((uVar15 & 0x3f) != 0) {
        *(ulong *)((long)__dest + lVar6 * 8) =
             *(ulong *)((long)__dest + lVar6 * 8) &
             ~(-1L << ((ulong)-((uint)uVar15 & 0x3f) & 0x3f)) << (uVar15 & 0x3f);
      }
    }
    __n = param_4 * 8 + lVar6 * -8 + 8;
    do {
      lVar5 = lVar6;
      lVar6 = lVar5 + 1;
      __n = __n - 8;
    } while (*(long *)((long)__dest + lVar5 * 8) == 0);
    if (0 < lVar5) {
      param_4 = (param_4 - lVar6) + 1;
      memmove(__dest,(void *)((long)__dest + lVar6 * 8 + -8),__n);
    }
    if ((param_1[3] != param_4) &&
       ((lVar6 = (*(code *)((undefined8 *)*param_1)[1])
                           (*(undefined8 *)*param_1,param_1[4],param_4 << 3), param_4 == 0 ||
        (lVar6 != 0)))) {
      param_1[3] = param_4;
      param_1[4] = lVar6;
    }
  }
  return (ulong)param_5;
}

/* ===== FUN_001d4ffc @ 001d4ffc [libNexusScriptRuntime69252.so] ===== */

undefined8
FUN_001d4ffc(undefined8 *param_1,ulong *param_2,ulong *param_3,ulong param_4,ulong *param_5,
            ulong param_6)

{
  undefined1 auVar1 [16];
  undefined1 auVar2 [16];
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  undefined1 auVar6 [16];
  undefined1 auVar7 [16];
  undefined1 auVar8 [16];
  undefined1 auVar9 [16];
  undefined1 auVar10 [16];
  bool bVar11;
  int iVar12;
  ulong uVar13;
  void *__s;
  ulong *puVar14;
  ulong uVar15;
  ulong *puVar16;
  long lVar17;
  long lVar18;
  ulong uVar19;
  ulong *puVar20;
  ulong uVar21;
  ulong uVar22;
  ulong uVar23;
  long lVar24;
  uint uVar25;
  ulong uVar26;
  ulong uVar27;
  
  lVar18 = param_6 - 1;
  uVar27 = param_5[lVar18];
  if (lVar18 == 0) {
    if (param_4 < 3) {
      uVar15 = 0;
      for (lVar18 = param_4 - 1; -1 < lVar18; lVar18 = lVar18 + -1) {
        uVar13 = param_3[lVar18];
        uVar15 = FUN_001e72dc(uVar13,uVar15,uVar27,0);
        param_2[lVar18] = uVar15;
        uVar15 = uVar13 - uVar15 * uVar27;
      }
    }
    else if ((long)(param_4 - 1) < 0) {
      uVar15 = 0;
    }
    else {
      uVar13 = FUN_001e72dc(0xffffffffffffffff,~uVar27,uVar27,0);
      uVar15 = 0;
      lVar18 = param_4 - 1;
      do {
        uVar19 = param_3[lVar18];
        uVar21 = uVar15 - ((long)uVar19 >> 0x3f);
        auVar1._8_8_ = 0;
        auVar1._0_8_ = uVar13;
        auVar6._8_8_ = 0;
        auVar6._0_8_ = uVar21;
        uVar21 = uVar15 + SUB168(auVar1 * auVar6,8) +
                 (ulong)CARRY8(uVar13 * uVar21,((long)uVar19 >> 0x3f & uVar27) + uVar19);
        uVar22 = ~uVar21;
        auVar2._8_8_ = 0;
        auVar2._0_8_ = uVar22;
        auVar7._8_8_ = 0;
        auVar7._0_8_ = uVar27;
        uVar22 = uVar22 * uVar27;
        uVar23 = uVar15 + (SUB168(auVar2 * auVar7,8) - uVar27) + (ulong)CARRY8(uVar19,uVar22);
        lVar17 = lVar18 + -1;
        uVar15 = (uVar27 & uVar23) + uVar19 + uVar22;
        param_2[lVar18] = uVar21 + uVar23 + 1;
        lVar18 = lVar17;
      } while (-1 < lVar17);
    }
    *param_3 = uVar15;
    return 0;
  }
  uVar15 = param_4 - param_6;
  uVar13 = uVar15;
  if ((long)param_6 <= (long)uVar15) {
    uVar13 = param_6;
  }
  if ((long)uVar13 < 0x32) {
    if (uVar15 < 3) {
      uVar13 = 0;
    }
    else {
      uVar13 = FUN_001e72dc(0xffffffffffffffff,~uVar27,uVar27,0);
    }
    if (-1 < lVar18) {
      puVar14 = param_3 + param_4;
      uVar19 = param_6;
      do {
        puVar14 = puVar14 + -1;
        uVar21 = param_5[uVar19 - 1];
        uVar22 = *puVar14;
        if (uVar22 != uVar21) {
          param_2[uVar15] = (ulong)(uVar21 <= uVar22);
          if (uVar21 <= uVar22) goto LAB_001d5258;
          goto LAB_001d52bc;
        }
        uVar19 = uVar19 - 1;
      } while (0 < (long)uVar19);
    }
    param_2[uVar15] = 1;
LAB_001d5258:
    if (0 < (long)param_6) {
      uVar19 = 0;
      lVar18 = -param_6;
      puVar14 = param_5;
      do {
        uVar23 = *puVar14;
        uVar22 = param_3[param_4 + lVar18];
        uVar21 = uVar22 - uVar23;
        param_3[param_4 + lVar18] = uVar21 - uVar19;
        uVar19 = (ulong)(uVar22 < uVar23 || uVar21 < uVar19);
        bVar11 = lVar18 != -1;
        lVar18 = lVar18 + 1;
        puVar14 = puVar14 + 1;
      } while (bVar11);
    }
LAB_001d52bc:
    lVar18 = uVar15 - 1;
    if (-1 < lVar18) {
      puVar14 = param_3 + param_4;
      do {
        lVar17 = lVar18 + param_6;
        uVar15 = param_3[lVar17];
        if (uVar15 < uVar27) {
          if (uVar13 == 0) {
            uVar15 = FUN_001e72dc(param_3[lVar17 + -1],uVar15,uVar27,0);
          }
          else {
            uVar19 = param_3[lVar17 + -1];
            uVar21 = uVar15 - ((long)uVar19 >> 0x3f);
            auVar3._8_8_ = 0;
            auVar3._0_8_ = uVar21;
            auVar8._8_8_ = 0;
            auVar8._0_8_ = uVar13;
            uVar21 = uVar15 + SUB168(auVar3 * auVar8,8) +
                     (ulong)CARRY8(uVar21 * uVar13,((long)uVar19 >> 0x3f & uVar27) + uVar19);
            uVar22 = ~uVar21;
            auVar4._8_8_ = 0;
            auVar4._0_8_ = uVar22;
            auVar9._8_8_ = 0;
            auVar9._0_8_ = uVar27;
            uVar15 = uVar21 + (SUB168(auVar4 * auVar9,8) - uVar27) + uVar15 +
                              (ulong)CARRY8(uVar22 * uVar27,uVar19) + 1;
          }
        }
        else {
          uVar15 = 0xffffffffffffffff;
        }
        uVar19 = 0;
        if (param_6 != 0) {
          uVar19 = 0;
          puVar16 = param_5;
          uVar21 = ~param_6;
          do {
            uVar22 = uVar21 + 1;
            uVar26 = *puVar16 * uVar15;
            auVar5._8_8_ = 0;
            auVar5._0_8_ = *puVar16;
            auVar10._8_8_ = 0;
            auVar10._0_8_ = uVar15;
            lVar24 = SUB168(auVar5 * auVar10,8);
            uVar23 = uVar26 + uVar19;
            if (CARRY8(uVar26,uVar19)) {
              lVar24 = lVar24 + 1;
            }
            uVar19 = lVar24 + (ulong)(puVar14[uVar21] < uVar23);
            puVar14[uVar21] = puVar14[uVar21] - uVar23;
            puVar16 = puVar16 + 1;
            uVar21 = uVar22;
          } while (uVar22 != 0xffffffffffffffff);
        }
        uVar21 = param_3[lVar17];
        param_3[lVar17] = uVar21 - uVar19;
        if (uVar21 < uVar19) {
          do {
            uVar15 = uVar15 - 1;
            uVar21 = 0;
            puVar16 = param_5;
            uVar19 = ~param_6;
            if (param_6 != 0) {
              do {
                uVar22 = uVar19 + 1;
                uVar23 = *puVar16 + puVar14[uVar19];
                uVar26 = uVar23 + uVar21;
                uVar25 = (uint)CARRY8(*puVar16,puVar14[uVar19]);
                if (CARRY8(uVar23,uVar21)) {
                  uVar25 = 1;
                }
                uVar21 = (ulong)uVar25;
                puVar14[uVar19] = uVar26;
                puVar16 = puVar16 + 1;
                uVar19 = uVar22;
              } while (uVar22 != 0xffffffffffffffff);
            }
          } while ((uVar21 == 0) ||
                  (uVar19 = param_3[lVar17], param_3[lVar17] = uVar19 + 1,
                  uVar19 != 0xffffffffffffffff));
        }
        param_2[lVar18] = uVar15;
        lVar18 = lVar18 + -1;
        puVar14 = puVar14 + -1;
      } while (-1 < lVar18);
    }
    return 0;
  }
  if (param_4 == param_6) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
              ,0x5b5,
              "int mp_divnorm_large(bf_context_t *, limb_t *, limb_t *, limb_t, const limb_t *, limb_t)"
              ,"nq >= 1");
  }
  uVar27 = uVar15;
  if (uVar15 < param_6) {
    uVar27 = uVar15 + 1;
  }
  lVar18 = uVar27 + 1;
  __s = (void *)(*(code *)param_1[1])(*param_1,0,lVar18 * 8);
  puVar14 = (ulong *)(*(code *)param_1[1])(*param_1,0,lVar18 * 0x10);
  if ((__s != (void *)0x0) && (puVar14 != (ulong *)0x0)) {
    if (uVar27 < param_6) {
      if (uVar27 != 0) {
        uVar13 = 0;
        do {
          uVar19 = uVar13 + 1;
          puVar14[uVar13] = param_5[(param_6 + uVar13) - uVar27];
          uVar13 = uVar19;
        } while (uVar27 != uVar19);
        uVar13 = 0;
        uVar19 = 1;
        do {
          uVar21 = uVar19;
          uVar22 = puVar14[uVar13];
          puVar14[uVar13] = uVar22 + uVar21;
          if (uVar27 <= uVar13 + 1) break;
          uVar13 = uVar13 + 1;
          uVar19 = (ulong)CARRY8(uVar22,uVar21);
        } while (CARRY8(uVar22,uVar21));
        if (uVar21 <= uVar22 + uVar21) goto LAB_001d5450;
      }
      memset(__s,0,uVar27 * 8);
      *(undefined8 *)((long)__s + uVar27 * 8) = 1;
    }
    else {
      if (uVar27 - param_6 != 0) {
        memset(puVar14,0,(uVar27 - param_6) * 8);
      }
      if (param_6 != 0) {
        lVar17 = -param_6;
        puVar16 = param_5;
        do {
          puVar14[uVar27 + lVar17] = *puVar16;
          bVar11 = lVar17 != -1;
          lVar17 = lVar17 + 1;
          puVar16 = puVar16 + 1;
        } while (bVar11);
      }
LAB_001d5450:
      iVar12 = FUN_001d4c30(param_1,__s,puVar14,uVar27);
      if (iVar12 != 0) goto LAB_001d5498;
    }
    iVar12 = FUN_001d448c(param_1,puVar14,__s,lVar18,param_3 + param_4 + ~uVar27,lVar18);
    if (iVar12 == 0) {
      uVar15 = uVar15 + 1;
      if (uVar15 != 0) {
        lVar18 = param_6 - param_4;
        puVar16 = param_2;
        do {
          lVar17 = lVar18 + 1;
          lVar18 = lVar18 + 1;
          *puVar16 = puVar14[uVar27 * 2 + lVar17];
          puVar16 = puVar16 + 1;
        } while (lVar18 != 1);
      }
      (*(code *)param_1[1])(*param_1,puVar14,0);
      (*(code *)param_1[1])(*param_1,__s,0);
      puVar14 = (ulong *)(*(code *)param_1[1])(*param_1,0,param_4 * 8 + 8);
      if ((puVar14 != (ulong *)0x0) &&
         (iVar12 = FUN_001d448c(param_1,puVar14,param_2,uVar15,param_5,param_6), iVar12 == 0)) {
        if (param_6 < 0x7fffffffffffffff) {
          uVar27 = 0;
          lVar18 = param_6 + 1;
          puVar16 = param_3;
          puVar20 = puVar14;
          do {
            uVar13 = *puVar16 - *puVar20;
            uVar19 = uVar13 - uVar27;
            uVar27 = (ulong)(*puVar16 < *puVar20 || uVar13 < uVar27);
            lVar18 = lVar18 + -1;
            *puVar16 = uVar19;
            puVar16 = puVar16 + 1;
            puVar20 = puVar20 + 1;
          } while (lVar18 != 0);
        }
        (*(code *)param_1[1])(*param_1,puVar14,0);
        do {
          do {
            uVar27 = param_6;
            if (param_3[param_6] == 0) {
              do {
                if ((long)uVar27 < 1) goto LAB_001d55ec;
                lVar18 = uVar27 - 1;
                lVar17 = uVar27 - 1;
                uVar27 = uVar27 - 1;
              } while (param_3[lVar18] == param_5[lVar17]);
              if (param_3[lVar18] < param_5[lVar17]) {
                return 0;
              }
            }
LAB_001d55ec:
            if ((long)param_6 < 1) {
              lVar18 = 0;
            }
            else {
              uVar13 = 0;
              puVar14 = param_3;
              puVar16 = param_5;
              uVar27 = param_6;
              do {
                uVar19 = *puVar14 - *puVar16;
                uVar21 = uVar19 - uVar13;
                uVar13 = (ulong)(*puVar14 < *puVar16 || uVar19 < uVar13);
                uVar27 = uVar27 - 1;
                *puVar14 = uVar21;
                puVar14 = puVar14 + 1;
                puVar16 = puVar16 + 1;
              } while (uVar27 != 0);
              lVar18 = -uVar13;
            }
            param_3[param_6] = param_3[param_6] + lVar18;
          } while (uVar15 == 0);
          uVar13 = 1;
          uVar27 = 0;
          do {
            uVar19 = param_2[uVar27];
            bVar11 = CARRY8(uVar19,uVar13);
            uVar19 = uVar19 + uVar13;
            uVar13 = (ulong)bVar11;
            param_2[uVar27] = uVar19;
            if (uVar15 <= uVar27 + 1) break;
            uVar27 = uVar27 + 1;
          } while (bVar11);
        } while( true );
      }
      goto LAB_001d54ac;
    }
  }
LAB_001d5498:
  if (__s != (void *)0x0) {
    (*(code *)param_1[1])(*param_1,__s,0);
  }
LAB_001d54ac:
  if (puVar14 != (ulong *)0x0) {
    (*(code *)param_1[1])(*param_1,puVar14,0);
  }
  return 0xffffffff;
}

/* ===== FUN_001d5b78 @ 001d5b78 [libNexusScriptRuntime69252.so] ===== */

void FUN_001d5b78(undefined8 *param_1,undefined8 *param_2,undefined8 *param_3,undefined8 *param_4,
                 undefined8 param_5,undefined4 param_6,uint param_7)

{
  uint uVar1;
  long lVar2;
  uint uVar3;
  uint uVar4;
  long lVar5;
  undefined8 uVar6;
  void *pvVar7;
  ulong uVar8;
  ulong uVar9;
  int iVar10;
  long lVar11;
  ulong uVar12;
  ulong uVar13;
  ulong uVar14;
  ulong uVar15;
  ulong uVar16;
  undefined1 auStack_b8 [8];
  undefined4 local_b0;
  long local_a8;
  ulong local_a0;
  long lStack_98;
  undefined8 uStack_90;
  undefined4 local_88;
  long local_80;
  ulong local_78;
  void *local_70;
  long local_68;
  
  lVar2 = tpidr_el0;
  local_68 = *(long *)(lVar2 + 0x28);
  if ((param_1 == param_3) || (param_1 == param_4)) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
              ,0x6df,
              "int bf_divrem(bf_t *, bf_t *, const bf_t *, const bf_t *, limb_t, bf_flags_t, int)",
              "q != a && q != b");
  }
  if ((param_2 == param_3) || (param_2 == param_4)) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
              ,0x6e0,
              "int bf_divrem(bf_t *, bf_t *, const bf_t *, const bf_t *, limb_t, bf_flags_t, int)",
              "r != a && r != b");
  }
  if (param_1 == param_2) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
              ,0x6e1,
              "int bf_divrem(bf_t *, bf_t *, const bf_t *, const bf_t *, limb_t, bf_flags_t, int)",
              "q != r");
  }
  uVar8 = param_3[3];
  if ((uVar8 == 0) || (uVar9 = param_4[3], uVar9 == 0)) {
    if (param_1[3] != 0) {
      uVar6 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
      param_1[3] = 0;
      param_1[4] = uVar6;
    }
    *(undefined4 *)(param_1 + 1) = 0;
    param_1[2] = 0x8000000000000000;
    if ((param_3[2] == 0x7fffffffffffffff) || (param_4[2] == 0x7fffffffffffffff)) {
      if (param_2[3] != 0) {
        uVar6 = (*(code *)((undefined8 *)*param_2)[1])(*(undefined8 *)*param_2,param_2[4],0);
        param_2[3] = 0;
        param_2[4] = uVar6;
      }
      pvVar7 = (void *)0x0;
      param_2[2] = 0x7fffffffffffffff;
    }
    else {
      if ((param_3[2] != 0x7ffffffffffffffe) && (param_4[2] != -0x8000000000000000)) {
        lVar11 = param_3[3];
        if (param_2[3] == lVar11) {
LAB_001d6080:
          *(undefined4 *)(param_2 + 1) = *(undefined4 *)(param_3 + 1);
          param_2[2] = param_3[2];
          pvVar7 = memcpy((void *)param_2[4],(void *)param_3[4],param_3[3] << 3);
        }
        else {
          lVar5 = (*(code *)((undefined8 *)*param_2)[1])
                            (*(undefined8 *)*param_2,param_2[4],lVar11 << 3);
          if ((lVar11 == 0) || (lVar5 != 0)) {
            param_2[3] = lVar11;
            param_2[4] = lVar5;
            goto LAB_001d6080;
          }
          pvVar7 = (void *)0x0;
          if (param_2[3] != 0) {
            pvVar7 = (void *)(*(code *)((undefined8 *)*param_2)[1])
                                       (*(undefined8 *)*param_2,param_2[4],0);
            param_2[3] = 0;
            param_2[4] = pvVar7;
          }
          *(undefined4 *)(param_2 + 1) = 0;
          param_2[2] = 0x7fffffffffffffff;
        }
        if (param_2[3] != 0) {
          if (*(long *)(lVar2 + 0x28) == local_68) {
            FUN_001d3a6c(param_2,param_5,param_6,param_2[3],0);
            return;
          }
          goto LAB_001d621c;
        }
        goto LAB_001d617c;
      }
      if (param_2[3] != 0) {
        uVar6 = (*(code *)((undefined8 *)*param_2)[1])(*(undefined8 *)*param_2,param_2[4],0);
        param_2[3] = 0;
        param_2[4] = uVar6;
      }
      pvVar7 = (void *)0x1;
      param_2[2] = 0x7fffffffffffffff;
    }
LAB_001d5fa4:
    *(undefined4 *)(param_2 + 1) = 0;
  }
  else {
    uVar1 = *(uint *)(param_4 + 1) ^ *(uint *)(param_3 + 1);
    uVar3 = 0;
    switch(param_7) {
    case 2:
      uVar3 = uVar1;
      break;
    case 3:
      uVar3 = uVar1 ^ 1;
      break;
    case 5:
      uVar3 = 1;
      break;
    case 6:
      uVar3 = *(uint *)(param_3 + 1);
    }
    local_80 = param_3[2];
    local_88 = 0;
    local_a8 = param_4[2];
    local_b0 = 0;
    local_70 = (void *)param_3[4];
    lStack_98 = param_4[4];
    lVar11 = local_80 - local_a8;
    local_a0 = uVar9;
    local_78 = uVar8;
    if (lVar11 == 0) {
      uVar12 = uVar9;
      uVar13 = uVar8;
      uVar15 = uVar8;
      if ((long)uVar8 <= (long)uVar9) {
        uVar15 = uVar9;
      }
      do {
        uVar13 = uVar13 - 1;
        uVar12 = uVar12 - 1;
        if ((long)(uVar15 - 1) < 0) goto LAB_001d5e68;
        if (uVar13 < uVar8) {
          uVar14 = *(ulong *)((long)local_70 + uVar13 * 8);
          if (uVar9 <= uVar12) goto LAB_001d5dbc;
LAB_001d5d7c:
          uVar16 = *(ulong *)(lStack_98 + uVar12 * 8);
        }
        else {
          uVar14 = 0;
          if (uVar12 < uVar9) goto LAB_001d5d7c;
LAB_001d5dbc:
          uVar16 = 0;
        }
        uVar15 = uVar15 - 1;
      } while (uVar14 == uVar16);
      if (uVar16 <= uVar14) goto LAB_001d5e68;
LAB_001d5dcc:
      *(undefined4 *)(param_1 + 1) = 0;
      param_1[2] = 0x8000000000000000;
      if (param_1[3] != 0) {
        uVar6 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
        param_1[3] = 0;
        param_1[4] = uVar6;
      }
      uVar8 = local_78;
      if (&uStack_90 != param_2) {
        if (param_2[3] != local_78) {
          lVar11 = (*(code *)((undefined8 *)*param_2)[1])
                             (*(undefined8 *)*param_2,param_2[4],local_78 << 3);
          if ((uVar8 != 0) && (lVar11 == 0)) {
            if (param_2[3] != 0) {
              uVar6 = (*(code *)((undefined8 *)*param_2)[1])(*(undefined8 *)*param_2,param_2[4],0);
              param_2[3] = 0;
              param_2[4] = uVar6;
            }
            *(undefined4 *)(param_2 + 1) = 0;
            param_2[2] = 0x7fffffffffffffff;
            goto LAB_001d5f34;
          }
          param_2[3] = uVar8;
          param_2[4] = lVar11;
        }
        param_2[2] = local_80;
        *(undefined4 *)(param_2 + 1) = local_88;
        memcpy((void *)param_2[4],local_70,local_78 << 3);
      }
    }
    else {
      if (local_80 < local_a8) goto LAB_001d5dcc;
LAB_001d5e68:
      if (lVar11 < 2) {
        lVar11 = 1;
      }
      FUN_001d7210(param_1,&uStack_90,auStack_b8,lVar11 + 1,1,FUN_001d72fc);
      if (param_1[3] != 0) {
        FUN_001d3a6c(param_1,0,0x11,param_1[3],0);
      }
      FUN_001d56bc(param_2,param_1,auStack_b8,0x3fffffffffffffff,1);
      FUN_001e71f4(param_2,&uStack_90,param_2,0x3fffffffffffffff,1);
    }
LAB_001d5f34:
    if ((param_1[2] == 0x7fffffffffffffff) || (lVar11 = param_2[2], lVar11 == 0x7fffffffffffffff)) {
LAB_001d5f50:
      if (param_1[3] != 0) {
        uVar6 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
        param_1[3] = 0;
        param_1[4] = uVar6;
      }
      *(undefined4 *)(param_1 + 1) = 0;
      param_1[2] = 0x7fffffffffffffff;
      if (param_2[3] != 0) {
        uVar6 = (*(code *)((undefined8 *)*param_2)[1])(*(undefined8 *)*param_2,param_2[4],0);
        param_2[3] = 0;
        param_2[4] = uVar6;
      }
      pvVar7 = (void *)0x20;
      param_2[2] = 0x7fffffffffffffff;
      goto LAB_001d5fa4;
    }
    uVar8 = param_2[3];
    if (uVar8 != 0) {
      if ((param_7 | 4) == 4) {
        if (lVar11 == local_a8 + -1) {
          uVar9 = local_a0;
          uVar12 = uVar8;
          uVar13 = uVar8;
          if ((long)uVar8 <= (long)local_a0) {
            uVar13 = local_a0;
          }
          do {
            uVar12 = uVar12 - 1;
            uVar9 = uVar9 - 1;
            if ((long)(uVar13 - 1) < 0) {
              iVar10 = 0;
              goto LAB_001d60f8;
            }
            if (uVar12 < uVar8) {
              uVar15 = *(ulong *)(param_2[4] + uVar12 * 8);
              if (local_a0 <= uVar9) goto LAB_001d6060;
LAB_001d601c:
              uVar14 = *(ulong *)(lStack_98 + uVar9 * 8);
            }
            else {
              uVar15 = 0;
              if (uVar9 < local_a0) goto LAB_001d601c;
LAB_001d6060:
              uVar14 = 0;
            }
            uVar13 = uVar13 - 1;
          } while (uVar15 == uVar14);
          iVar10 = 1;
          if (uVar15 < uVar14) {
            iVar10 = -1;
          }
        }
        else {
          iVar10 = 1;
          if (lVar11 < local_a8 + -1) {
            iVar10 = -1;
          }
        }
LAB_001d60f8:
        if (iVar10 < 1) {
          if (iVar10 != 0) goto LAB_001d6148;
          if (param_7 != 4) {
            uVar8 = param_1[3] * 0x40 - param_1[2];
            if ((((long)uVar8 < 0) || ((ulong)param_1[3] <= (ulong)((long)uVar8 >> 6))) ||
               ((*(ulong *)(param_1[4] + ((long)uVar8 >> 6) * 8) >> (uVar8 & 0x3f) & 1) == 0))
            goto LAB_001d6148;
          }
        }
      }
      else if (uVar3 == 0) goto LAB_001d6148;
      uVar3 = FUN_001d6220(param_1,param_1,1,0x3fffffffffffffff,1);
      uVar4 = FUN_001e71f4(param_2,param_2,auStack_b8,0x3fffffffffffffff,1);
      if (((uVar4 | uVar3) >> 5 & 1) != 0) goto LAB_001d5f50;
    }
LAB_001d6148:
    *(uint *)(param_2 + 1) = *(uint *)(param_2 + 1) ^ *(uint *)(param_3 + 1);
    *(uint *)(param_1 + 1) = uVar1;
    if (param_2[3] == 0) {
LAB_001d617c:
      pvVar7 = (void *)0x0;
    }
    else {
      pvVar7 = (void *)FUN_001d3a6c(param_2,param_5,param_6,param_2[3],0);
    }
  }
  if (*(long *)(lVar2 + 0x28) == local_68) {
    return;
  }
LAB_001d621c:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(pvVar7);
}

/* ===== FUN_001d6dec @ 001d6dec [libNexusScriptRuntime69252.so] ===== */

/* WARNING: Removing unreachable block (ram,0x001d7184) */
/* WARNING: Removing unreachable block (ram,0x001d7194) */
/* WARNING: Removing unreachable block (ram,0x001d704c) */

void FUN_001d6dec(undefined8 *param_1,undefined8 *param_2,long param_3,undefined4 param_4)

{
  bool bVar1;
  ulong uVar2;
  long lVar3;
  long lVar4;
  int iVar5;
  undefined8 uVar6;
  long lVar7;
  void *__s;
  undefined1 *puVar8;
  ulong uVar9;
  long *plVar10;
  long lVar11;
  ulong uVar12;
  undefined8 *puVar13;
  ulong uVar14;
  long lVar15;
  undefined1 auStack_a8 [64];
  long local_68;
  
  lVar4 = tpidr_el0;
  local_68 = *(long *)(lVar4 + 0x28);
  if (param_1 == param_2) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
              ,0x866,"int bf_sqrt(bf_t *, const bf_t *, limb_t, bf_flags_t)","r != a");
  }
  if (param_2[3] == 0) {
    if (param_2[2] == 0x7ffffffffffffffe) {
      if (*(int *)(param_2 + 1) != 0) goto LAB_001d6e38;
    }
    else if (param_2[2] == 0x7fffffffffffffff) {
      if (param_1[3] != 0) {
        uVar6 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
        param_1[3] = 0;
        param_1[4] = uVar6;
      }
      uVar6 = 0;
      goto LAB_001d7078;
    }
    if (param_1[3] != 0) {
      uVar6 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
      param_1[3] = 0;
      param_1[4] = uVar6;
    }
    *(undefined4 *)(param_1 + 1) = *(undefined4 *)(param_2 + 1);
    param_1[2] = param_2[2];
    memcpy((void *)param_1[4],(void *)param_2[4],param_2[3] << 3);
    uVar6 = 0;
    goto LAB_001d7080;
  }
  if (*(int *)(param_2 + 1) == 0) {
    uVar2 = param_3 * 2 + 0x83;
    puVar13 = (undefined8 *)*param_2;
    uVar14 = uVar2 >> 7;
    if (param_1[3] == uVar14) {
LAB_001d6ecc:
      __s = (void *)(*(code *)puVar13[1])(*puVar13,0,uVar14 << 4);
      if (__s != (void *)0x0) {
        lVar15 = uVar14 * 2;
        lVar7 = lVar15;
        if ((long)param_2[3] <= lVar15) {
          lVar7 = param_2[3];
        }
        memset(__s,0,(lVar15 - lVar7) * 8);
        memcpy((void *)((long)__s + lVar7 * -8 + uVar14 * 0x10),
               (void *)(param_2[4] + param_2[3] * 8 + lVar7 * -8),lVar7 * 8);
        uVar12 = 0;
        if (((*(byte *)(param_2 + 2) & 1) != 0) && (0x7f < uVar2)) {
          uVar9 = 0;
          do {
            lVar3 = lVar15 + -1;
            uVar12 = *(ulong *)((long)__s + lVar15 * 8 + -8);
            *(ulong *)((long)__s + lVar15 * 8 + -8) = uVar12 >> 1 | uVar9 << 0x3f;
            bVar1 = 0 < lVar15;
            uVar9 = uVar12;
            lVar15 = lVar3;
          } while (lVar3 != 0 && bVar1);
          uVar12 = uVar12 & 1;
        }
        uVar6 = param_1[4];
        puVar8 = auStack_a8;
        if ((uVar2 < 0x800) ||
           (puVar8 = (undefined1 *)
                     (*(code *)puVar13[1])(*puVar13,0,(uVar14 & 0x1fffffffffffffe) * 4 + 8),
           puVar8 != (undefined1 *)0x0)) {
          iVar5 = FUN_001d679c(puVar13,uVar6,__s,uVar14,puVar8,(void *)((long)__s + uVar14 * 8));
          if ((puVar8 != auStack_a8) && (puVar8 != (undefined1 *)0x0)) {
            (*(code *)puVar13[1])(*puVar13,puVar8,0);
          }
          if (iVar5 == 0) {
            if (uVar12 == 0) {
              lVar15 = 0;
              do {
                lVar11 = *(long *)((long)__s + lVar15);
                lVar3 = uVar14 * 8 - lVar15;
                lVar15 = lVar15 + 8;
              } while (lVar11 == 0 && lVar3 != 0);
              (*(code *)puVar13[1])(*puVar13,__s,0);
              if (lVar11 != 0) goto LAB_001d7140;
              lVar7 = param_2[3] - lVar7;
              if (0 < lVar7) {
                plVar10 = (long *)param_2[4];
                do {
                  if (*plVar10 != 0) goto LAB_001d7140;
                  lVar7 = lVar7 + -1;
                  plVar10 = plVar10 + 1;
                } while (lVar7 != 0);
              }
            }
            else {
              (*(code *)puVar13[1])(*puVar13,__s,0);
LAB_001d7140:
              *(ulong *)param_1[4] = *(ulong *)param_1[4] | 1;
            }
            *(undefined4 *)(param_1 + 1) = 0;
            param_1[2] = param_2[2] + 1 >> 1;
            if (param_1[3] == 0) {
              uVar6 = 0;
            }
            else {
              uVar6 = FUN_001d3a6c(param_1,param_3,param_4,param_1[3],0);
            }
            goto LAB_001d7080;
          }
        }
        (*(code *)puVar13[1])(*puVar13,__s,0);
      }
    }
    else {
      lVar7 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],uVar14 << 3)
      ;
      if ((uVar2 < 0x80) || (lVar7 != 0)) {
        param_1[3] = uVar14;
        param_1[4] = lVar7;
        goto LAB_001d6ecc;
      }
    }
    if (param_1[3] != 0) {
      uVar6 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
      param_1[3] = 0;
      param_1[4] = uVar6;
    }
    uVar6 = 0x20;
  }
  else {
LAB_001d6e38:
    if (param_1[3] != 0) {
      uVar6 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
      param_1[3] = 0;
      param_1[4] = uVar6;
    }
    uVar6 = 1;
  }
LAB_001d7078:
  param_1[2] = 0x7fffffffffffffff;
  *(undefined4 *)(param_1 + 1) = 0;
LAB_001d7080:
  if (*(long *)(lVar4 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar6);
}

/* ===== FUN_001d7938 @ 001d7938 [libNexusScriptRuntime69252.so] ===== */

undefined8 FUN_001d7938(undefined8 *param_1,undefined8 *param_2,undefined8 *param_3,int param_4)

{
  long lVar1;
  int iVar2;
  long lVar3;
  undefined8 uVar4;
  ulong *puVar5;
  ulong uVar6;
  ulong uVar7;
  ulong uVar8;
  long lVar9;
  long lVar10;
  undefined8 *puVar11;
  ulong uVar12;
  ulong uVar13;
  ulong uVar14;
  ulong uVar15;
  long lVar16;
  ulong uVar17;
  ulong uVar18;
  ulong uVar19;
  ulong uVar20;
  undefined8 local_b8;
  undefined4 local_b0;
  undefined8 local_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined4 local_88;
  undefined8 local_80;
  undefined8 local_78;
  undefined8 uStack_70;
  long local_68;
  
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  if ((param_1 == param_2) || (param_1 == param_3)) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
              ,0x930,"int bf_logic_op(bf_t *, const bf_t *, const bf_t *, int)","r != a1 && r != b1"
             );
  }
  if ((long)param_2[2] < 1) {
    uVar19 = 0;
  }
  else {
    uVar19 = (ulong)*(int *)(param_2 + 1);
  }
  if ((long)param_3[2] < 1) {
    uVar20 = 0;
    if (uVar19 != 0) goto LAB_001d79c0;
joined_r0x001d79b0:
    if (uVar20 != 0) {
      local_90 = *param_1;
      local_88 = 0;
      local_78 = 0;
      uStack_70 = 0;
      local_80 = 0x8000000000000000;
      iVar2 = FUN_001d6220(&local_90,param_3,1,0x3fffffffffffffff,1);
      param_3 = &local_90;
      if (iVar2 != 0) goto LAB_001d7c50;
    }
    if (param_4 == 1) {
      uVar17 = uVar20 ^ uVar19;
LAB_001d7a90:
      lVar10 = param_2[2];
      if ((long)param_2[2] <= (long)param_3[2]) {
        lVar10 = param_3[2];
      }
    }
    else {
      if (param_4 == 0) {
        uVar17 = uVar20 | uVar19;
        goto LAB_001d7a90;
      }
      uVar17 = uVar20 & uVar19;
      if ((param_4 != 2) || (uVar17 != 0)) goto LAB_001d7a90;
      if (uVar20 == 0 && uVar19 == 0) {
        lVar10 = param_2[2];
        if ((long)param_3[2] <= (long)param_2[2]) {
          lVar10 = param_3[2];
        }
      }
      else if (uVar19 == 0) {
        lVar10 = param_2[2];
      }
      else {
        lVar10 = param_3[2];
      }
    }
    if (lVar10 < 2) {
      lVar10 = 1;
    }
    uVar18 = lVar10 + 0x3fU >> 6;
    if (param_1[3] != uVar18) {
      lVar3 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],uVar18 << 3)
      ;
      if (lVar3 == 0) goto LAB_001d7c50;
      param_1[3] = uVar18;
      param_1[4] = lVar3;
    }
    lVar16 = param_3[4];
    uVar12 = param_2[3] * 0x40 - param_2[2];
    uVar14 = param_3[3] * 0x40 - param_3[2];
    lVar3 = param_2[4];
    puVar5 = (ulong *)param_1[4];
    uVar13 = uVar12;
    uVar15 = uVar14;
    do {
      uVar8 = (long)uVar13 >> 6;
      if (uVar8 < (ulong)param_2[3]) {
        uVar6 = *(ulong *)(lVar3 + uVar8 * 8);
      }
      else {
        uVar6 = 0;
      }
      if ((uVar12 & 0x3f) != 0) {
        if (uVar8 + 1 < (ulong)param_2[3]) {
          lVar9 = *(long *)(lVar3 + (uVar8 + 1) * 8);
        }
        else {
          lVar9 = 0;
        }
        uVar6 = lVar9 << ((ulong)(0x40 - ((uint)uVar12 & 0x3f)) & 0x3f) | uVar6 >> (uVar12 & 0x3f);
      }
      uVar8 = (long)uVar15 >> 6;
      if (uVar8 < (ulong)param_3[3]) {
        uVar7 = *(ulong *)(lVar16 + uVar8 * 8);
      }
      else {
        uVar7 = 0;
      }
      if ((uVar14 & 0x3f) != 0) {
        if (uVar8 + 1 < (ulong)param_3[3]) {
          lVar9 = *(long *)(lVar16 + (uVar8 + 1) * 8);
        }
        else {
          lVar9 = 0;
        }
        uVar7 = lVar9 << ((ulong)(0x40 - ((uint)uVar14 & 0x3f)) & 0x3f) | uVar7 >> (uVar14 & 0x3f);
      }
      uVar6 = uVar6 ^ -uVar19;
      uVar7 = uVar7 ^ -uVar20;
      if (param_4 == 1) {
        uVar7 = uVar7 ^ uVar6;
      }
      else if (param_4 == 0) {
        uVar7 = uVar7 | uVar6;
      }
      else {
        uVar7 = uVar7 & uVar6;
      }
      uVar18 = uVar18 - 1;
      uVar13 = uVar13 + 0x40;
      uVar15 = uVar15 + 0x40;
      *puVar5 = uVar7 ^ -uVar17;
      puVar5 = puVar5 + 1;
    } while (uVar18 != 0);
    *(int *)(param_1 + 1) = (int)uVar17;
    param_1[2] = lVar10 + 0x3fU & 0xffffffffffffffc0;
    FUN_001d3988(param_1,0x3fffffffffffffff,1);
    if ((uVar17 == 0) ||
       (iVar2 = FUN_001d6220(param_1,param_1,0xffffffffffffffff,0x3fffffffffffffff,1), iVar2 == 0))
    {
      uVar4 = 0;
      goto LAB_001d7c80;
    }
  }
  else {
    uVar20 = (ulong)*(int *)(param_3 + 1);
    if (uVar19 == 0) goto joined_r0x001d79b0;
LAB_001d79c0:
    local_b8 = *param_1;
    local_b0 = 0;
    local_a0 = 0;
    uStack_98 = 0;
    local_a8 = 0x8000000000000000;
    iVar2 = FUN_001d6220(&local_b8,param_2,1,0x3fffffffffffffff,1);
    if (iVar2 == 0) {
      param_2 = &local_b8;
      goto joined_r0x001d79b0;
    }
    param_3 = (undefined8 *)0x0;
    param_2 = &local_b8;
  }
LAB_001d7c50:
  if (param_1[3] != 0) {
    uVar4 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
    param_1[3] = 0;
    param_1[4] = uVar4;
  }
  uVar4 = 0x20;
  *(undefined4 *)(param_1 + 1) = 0;
  param_1[2] = 0x7fffffffffffffff;
LAB_001d7c80:
  if (((param_2 == &local_b8) && (puVar11 = (undefined8 *)*param_2, puVar11 != (undefined8 *)0x0))
     && (param_2[4] != 0)) {
    (*(code *)puVar11[1])(*puVar11,param_2[4],0);
  }
  if (((param_3 == &local_90) && (puVar11 = (undefined8 *)*param_3, puVar11 != (undefined8 *)0x0))
     && (param_3[4] != 0)) {
    (*(code *)puVar11[1])(*puVar11,param_3[4],0);
  }
  if (*(long *)(lVar1 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return uVar4;
}

/* ===== FUN_001d89d8 @ 001d89d8 [libNexusScriptRuntime69252.so] ===== */

ulong FUN_001d89d8(undefined8 *param_1,long *param_2,byte *param_3,long *param_4,uint param_5,
                  undefined8 param_6,uint param_7,int param_8)

{
  byte bVar1;
  long lVar2;
  bool bVar3;
  int iVar4;
  long lVar5;
  undefined8 uVar6;
  void *pvVar7;
  uint uVar8;
  int iVar9;
  uint uVar10;
  byte *pbVar11;
  undefined8 *puVar12;
  ulong uVar13;
  undefined8 *puVar14;
  uint uVar15;
  ulong uVar16;
  bool bVar17;
  long lVar18;
  ulong uVar19;
  byte *pbVar20;
  ulong uVar21;
  long lVar22;
  undefined4 uVar23;
  ulong uVar24;
  long lVar25;
  byte *pbVar26;
  undefined8 *local_c0;
  undefined4 local_b8;
  undefined8 local_b0;
  undefined8 local_a8;
  long local_a0;
  undefined8 local_98;
  undefined4 local_90;
  undefined8 local_88;
  undefined8 local_80;
  undefined8 uStack_78;
  long local_70;
  
  bVar3 = (param_7 & 0x40000) != 0;
  lVar2 = tpidr_el0;
  local_70 = *(long *)(lVar2 + 0x28);
  *param_2 = 0;
  pbVar20 = param_3;
  if (bVar3 || 0x10 < (int)param_5) {
LAB_001d8a7c:
    if (*pbVar20 == 0x2d) {
      pbVar26 = pbVar20 + 1;
      uVar23 = 1;
    }
    else if (*pbVar20 == 0x2b) {
      uVar23 = 0;
      pbVar26 = pbVar20 + 1;
    }
    else {
      uVar23 = 0;
      pbVar26 = pbVar20;
    }
    pbVar20 = pbVar26;
    if (*pbVar26 == 0x30) {
      bVar1 = pbVar26[1];
      if (bVar1 == 0x78) {
LAB_001d8ad8:
        if ((param_7 & 0x10000) != 0 || (param_5 & 0xffffffef) != 0) {
          if (bVar1 != 0x6f) goto LAB_001d8b50;
LAB_001d8af0:
          if (param_5 == 0) {
LAB_001d8b5c:
            if ((param_7 >> 0x11 & 1) != 0) {
              param_5 = 8;
              goto LAB_001d8be0;
            }
          }
          goto LAB_001d8b60;
        }
        param_5 = 0x10;
LAB_001d8be0:
        pbVar20 = pbVar26 + 2;
        bVar1 = *pbVar20;
        uVar8 = bVar1 - 0x30;
        if (9 < uVar8) {
          uVar15 = (uint)bVar1;
          if (bVar1 - 0x41 < 0x1a) {
            uVar8 = uVar15 - 0x37;
          }
          else {
            uVar8 = uVar15 - 0x57;
            if (0x19 < uVar15 - 0x61) {
              uVar8 = 0x24;
            }
          }
        }
        if ((int)param_5 <= (int)uVar8) goto LAB_001d9074;
      }
      else {
        if (bVar1 == 0x6f) goto LAB_001d8af0;
        if (bVar1 == 0x58) goto LAB_001d8ad8;
LAB_001d8b50:
        if ((param_5 == 0) && (bVar1 == 0x4f)) goto LAB_001d8b5c;
LAB_001d8b60:
        if (bVar1 == 0x62) {
          if (param_5 == 0) {
LAB_001d8b7c:
            if ((param_7 >> 0x11 & 1) != 0) {
              param_5 = 2;
              goto LAB_001d8be0;
            }
          }
        }
        else if ((param_5 == 0) && (bVar1 == 0x42)) goto LAB_001d8b7c;
      }
LAB_001d8c24:
      uVar8 = 10;
      if (param_5 != 0) {
        uVar8 = param_5;
      }
      puVar14 = param_1;
      if (param_8 == 0) {
        if ((uVar8 & uVar8 - 1) == 0) {
          iVar4 = 0;
          if (1 < uVar8) {
            iVar4 = 0x40 - (int)LZCOUNT((long)(int)uVar8 + -1);
          }
        }
        else {
          local_90 = 0;
          local_80 = 0;
          uStack_78 = 0;
          local_98 = *param_1;
          local_88 = 0x8000000000000000;
          puVar14 = &local_98;
          iVar4 = 0;
        }
      }
      else {
        if (uVar8 != 10) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
                    ,0xb97,
                    "int bf_atof_internal(bf_t *, slimb_t *, const char *, const char **, int, limb_t, bf_flags_t, BOOL)"
                    ,"radix == 10");
        }
        iVar4 = 0;
      }
      pbVar20 = pbVar20 + -1;
      do {
        pbVar20 = pbVar20 + 1;
      } while (*pbVar20 == 0x30);
      if (iVar4 == 0) {
        uVar16 = (ulong)(byte)(&DAT_0011dfda)[(int)uVar8];
      }
      else {
        uVar16 = 0x40;
      }
      if ((puVar14[3] != 1) &&
         (lVar5 = (*(code *)((undefined8 *)*puVar14)[1])(*(undefined8 *)*puVar14,puVar14[4],8),
         lVar5 != 0)) {
        puVar14[3] = 1;
        puVar14[4] = lVar5;
      }
      lVar25 = 0;
      uVar19 = 0;
      lVar18 = 0;
      lVar5 = 0;
      bVar3 = false;
      uVar21 = uVar16;
      do {
        if (*pbVar20 == 0x2e) {
          if (pbVar20 <= pbVar26) {
            bVar1 = pbVar20[1];
            uVar15 = bVar1 - 0x30;
            if (9 < uVar15) {
              uVar10 = (uint)bVar1;
              if (bVar1 - 0x41 < 0x1a) {
                uVar15 = uVar10 - 0x37;
              }
              else {
                uVar15 = uVar10 - 0x57;
                if (0x19 < uVar10 - 0x61) {
                  uVar15 = 0x24;
                }
              }
            }
            if ((int)uVar8 <= (int)uVar15) goto LAB_001d8d98;
          }
          if (!bVar3) {
            pbVar20 = pbVar20 + 1;
            bVar3 = true;
            lVar18 = lVar5;
            goto LAB_001d8d98;
          }
          iVar9 = 6;
          bVar3 = true;
        }
        else {
LAB_001d8d98:
          bVar1 = *pbVar20;
          uVar15 = bVar1 - 0x30;
          if (9 < uVar15) {
            uVar10 = (uint)bVar1;
            uVar15 = uVar10 - 0x57;
            if (0x19 < bVar1 - 0x61) {
              uVar15 = 0x24;
            }
            if (uVar10 - 0x41 < 0x1a) {
              uVar15 = uVar10 - 0x37;
            }
          }
          uVar24 = (ulong)uVar15;
          if (uVar24 < (ulong)(long)(int)uVar8) {
            lVar5 = lVar5 + 1;
            pbVar20 = pbVar20 + 1;
            if (iVar4 == 0) {
              uVar19 = uVar24 + uVar19 * (long)(int)uVar8;
              uVar15 = (int)uVar21 - 1;
              uVar21 = (ulong)uVar15;
              if (uVar15 == 0) {
                if (lVar25 < 0) {
                  lVar22 = puVar14[3];
                  uVar21 = (ulong)(lVar22 * 3) >> 1;
                  if ((long)uVar21 < lVar22 + 1) {
                    uVar21 = lVar22 + 1;
                  }
                  pvVar7 = (void *)(*(code *)((undefined8 *)*puVar14)[1])
                                             (*(undefined8 *)*puVar14,puVar14[4],uVar21 << 3);
                  if (pvVar7 == (void *)0x0) {
                    uVar21 = 0;
                    iVar9 = 8;
                    goto LAB_001d8d20;
                  }
                  puVar14[4] = pvVar7;
                  lVar22 = uVar21 - puVar14[3];
                  memmove((void *)((long)pvVar7 + lVar22 * 8),pvVar7,puVar14[3] << 3);
                  lVar25 = lVar22 + lVar25;
                  puVar14[3] = uVar21;
                }
                *(ulong *)(puVar14[4] + lVar25 * 8) = uVar19;
                uVar19 = 0;
                uVar21 = uVar16;
                lVar25 = lVar25 + -1;
              }
            }
            else {
              uVar10 = (int)uVar21 - iVar4;
              uVar21 = (ulong)uVar10;
              if ((int)uVar10 < 1) {
                uVar19 = uVar15 >> ((ulong)-uVar10 & 0x3f) | uVar19;
                if (lVar25 < 0) {
                  lVar22 = puVar14[3];
                  uVar13 = (ulong)(lVar22 * 3) >> 1;
                  if ((long)uVar13 < lVar22 + 1) {
                    uVar13 = lVar22 + 1;
                  }
                  pvVar7 = (void *)(*(code *)((undefined8 *)*puVar14)[1])
                                             (*(undefined8 *)*puVar14,puVar14[4],uVar13 << 3);
                  if (pvVar7 != (void *)0x0) {
                    puVar14[4] = pvVar7;
                    lVar22 = uVar13 - puVar14[3];
                    memmove((void *)((long)pvVar7 + lVar22 * 8),pvVar7,puVar14[3] << 3);
                    lVar25 = lVar22 + lVar25;
                    puVar14[3] = uVar13;
                    goto LAB_001d8e40;
                  }
                  bVar17 = false;
                }
                else {
LAB_001d8e40:
                  bVar17 = true;
                  *(ulong *)(puVar14[4] + lVar25 * 8) = uVar19;
                  lVar25 = lVar25 + -1;
                }
                if (!bVar17) {
                  iVar9 = 8;
                  goto LAB_001d8d20;
                }
                uVar19 = uVar24 << (uVar21 & 0x3f);
                uVar21 = (ulong)(uVar10 + 0x40);
                if (-1 < (int)uVar10) {
                  uVar19 = 0;
                }
              }
              else {
                uVar19 = uVar24 << (uVar21 & 0x3f) | uVar19;
              }
            }
            iVar9 = 0;
          }
          else {
            iVar9 = 6;
          }
        }
LAB_001d8d20:
      } while (iVar9 == 0);
      pbVar11 = pbVar20;
      if (iVar9 == 8) {
LAB_001d94b8:
        if (((iVar4 == 0) && (puVar12 = (undefined8 *)*puVar14, puVar12 != (undefined8 *)0x0)) &&
           (puVar14[4] != 0)) {
          (*(code *)puVar12[1])(*puVar12,puVar14[4],0);
        }
        if (param_1[3] != 0) {
          uVar6 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
          param_1[3] = 0;
          param_1[4] = uVar6;
        }
        pbVar26 = (byte *)0x20;
        goto LAB_001d94ec;
      }
      if (iVar9 != 6) goto LAB_001d94fc;
      if (bVar3) {
        lVar5 = lVar18;
      }
      lVar18 = lVar25;
      if ((int)uVar21 != (int)uVar16) {
        if ((iVar4 == 0) && ((int)uVar21 != 0)) {
          do {
            uVar19 = uVar19 * (long)(int)uVar8;
            uVar15 = (int)uVar21 - 1;
            uVar21 = (ulong)uVar15;
          } while (uVar15 != 0);
        }
        if (lVar25 < 0) {
          lVar18 = puVar14[3];
          uVar21 = (ulong)(lVar18 * 3) >> 1;
          if ((long)uVar21 < lVar18 + 1) {
            uVar21 = lVar18 + 1;
          }
          pvVar7 = (void *)(*(code *)((undefined8 *)*puVar14)[1])
                                     (*(undefined8 *)*puVar14,puVar14[4],uVar21 << 3);
          if (pvVar7 == (void *)0x0) goto LAB_001d94b8;
          puVar14[4] = pvVar7;
          lVar18 = uVar21 - puVar14[3];
          memmove((void *)((long)pvVar7 + lVar18 * 8),pvVar7,puVar14[3] << 3);
          lVar25 = lVar18 + lVar25;
          puVar14[3] = uVar21;
        }
        lVar18 = lVar25 + -1;
        *(ulong *)(puVar14[4] + lVar25 * 8) = uVar19;
      }
      memset((void *)puVar14[4],0,lVar18 * 8 + 8);
      if (pbVar20 == pbVar26) {
        if (((iVar4 == 0) && (puVar12 = (undefined8 *)*puVar14, puVar12 != (undefined8 *)0x0)) &&
           (puVar14[4] != 0)) {
          (*(code *)puVar12[1])(*puVar12,puVar14[4],0);
        }
        goto LAB_001d9074;
      }
      bVar1 = *pbVar20;
      if (uVar8 == 10) {
        if ((bVar1 | 0x20) != 0x65) goto LAB_001d90fc;
LAB_001d90bc:
        if (pbVar20 <= pbVar26) goto LAB_001d90fc;
        pbVar11 = pbVar20 + 1;
        bVar3 = (*pbVar20 & 0xdf) == 0x50;
        if (*pbVar11 == 0x2d) {
          bVar17 = false;
          pbVar11 = pbVar20 + 2;
        }
        else if (*pbVar11 == 0x2b) {
          pbVar11 = pbVar20 + 2;
          bVar17 = true;
        }
        else {
          bVar17 = true;
        }
        lVar22 = 0;
        do {
          bVar1 = *pbVar11;
          uVar15 = bVar1 - 0x30;
          if (9 < uVar15) {
            uVar10 = (uint)bVar1;
            uVar15 = uVar10 - 0x57;
            if (0x19 < bVar1 - 0x61) {
              uVar15 = 0x24;
            }
            if (uVar10 - 0x41 < 0x1a) {
              uVar15 = uVar10 - 0x37;
            }
          }
          if ((int)uVar15 < 10) {
            if (lVar22 < 0xccccccccccccccc) {
              iVar9 = 0;
              pbVar11 = pbVar11 + 1;
              lVar22 = lVar22 * 10 + (ulong)uVar15;
            }
            else {
              if (bVar17) {
                if (param_1[3] != 0) {
                  uVar6 = (*(code *)((undefined8 *)*param_1)[1])
                                    (*(undefined8 *)*param_1,param_1[4],0);
                  param_1[3] = 0;
                  param_1[4] = uVar6;
                }
                pbVar26 = (byte *)0x14;
                uVar6 = 0x7ffffffffffffffe;
              }
              else {
                if (param_1[3] != 0) {
                  uVar6 = (*(code *)((undefined8 *)*param_1)[1])
                                    (*(undefined8 *)*param_1,param_1[4],0);
                  param_1[3] = 0;
                  param_1[4] = uVar6;
                }
                pbVar26 = (byte *)0x18;
                uVar6 = 0x8000000000000000;
              }
              iVar9 = 2;
              param_1[2] = uVar6;
              *(undefined4 *)(param_1 + 1) = uVar23;
            }
          }
          else {
            iVar9 = 0xb;
          }
        } while (iVar9 == 0);
        if (iVar9 == 2) goto LAB_001d94f4;
        if (iVar9 != 0xb) goto LAB_001d94fc;
        lVar25 = -lVar22;
        if (bVar17) {
          lVar25 = lVar22;
        }
      }
      else {
        if ((bVar1 == 0x40) || ((iVar4 != 0 && ((bVar1 | 0x20) == 0x70)))) goto LAB_001d90bc;
LAB_001d90fc:
        bVar3 = false;
        lVar25 = 0;
      }
      if (param_8 == 0) {
        if (iVar4 == 0) {
          lVar22 = puVar14[3] - (lVar18 + 1);
          if (lVar22 == 0) {
            if (param_1[3] != 0) {
              uVar6 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
              param_1[3] = 0;
              param_1[4] = uVar6;
            }
            pbVar26 = (byte *)0x0;
            param_1[2] = 0x8000000000000000;
            *(undefined4 *)(param_1 + 1) = uVar23;
          }
          else {
            local_c0 = (undefined8 *)*param_1;
            local_b8 = 0;
            local_a8 = 0;
            local_a0 = 0;
            local_b0 = 0x8000000000000000;
            iVar4 = FUN_001e2e08(&local_c0,puVar14[4] + (lVar18 + 1) * 8,lVar22,(long)(int)uVar8);
            if (iVar4 == 0) {
              lVar25 = (lVar5 - lVar22 * uVar16) + lVar25;
              local_b8 = uVar23;
              if ((param_7 >> 0x13 & 1) == 0) {
                uVar16 = FUN_001d83a8(param_1,&local_c0,(long)(int)uVar8,lVar25,param_6);
              }
              else {
                *param_2 = lVar25;
                uVar16 = FUN_001d3880(param_1,&local_c0);
              }
              pbVar26 = (byte *)(uVar16 & 0xffffffff);
            }
            else {
              if (param_1[3] != 0) {
                uVar6 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0)
                ;
                param_1[3] = 0;
                param_1[4] = uVar6;
              }
              pbVar26 = (byte *)0x20;
              *(undefined4 *)(param_1 + 1) = 0;
              param_1[2] = 0x7fffffffffffffff;
            }
            if ((local_c0 != (undefined8 *)0x0) && (local_a0 != 0)) {
              (*(code *)local_c0[1])(*local_c0,local_a0,0);
            }
          }
          puVar12 = (undefined8 *)*puVar14;
          if ((puVar12 != (undefined8 *)0x0) && (puVar14[4] != 0)) {
            (*(code *)puVar12[1])(*puVar12,puVar14[4],0);
          }
          goto LAB_001d94f4;
        }
        lVar18 = (long)iVar4;
        if (bVar3) {
          lVar18 = 1;
        }
        puVar14[2] = lVar5 * iVar4 + lVar18 * lVar25;
        *(undefined4 *)(puVar14 + 1) = uVar23;
        uVar16 = FUN_001d3988(puVar14,param_6,param_7);
      }
      else {
        puVar14[2] = lVar25 + lVar5;
        *(undefined4 *)(puVar14 + 1) = uVar23;
        uVar16 = FUN_001e030c(puVar14,param_6,param_7);
      }
      pbVar26 = (byte *)(uVar16 & 0xffffffff);
    }
    else {
      if (bVar3 || 0x10 < (int)param_5) goto LAB_001d8c24;
      lVar5 = 0;
      uVar8 = 0x69;
      do {
        uVar10 = uVar8;
        bVar1 = pbVar26[lVar5];
        uVar15 = bVar1 + 0x20;
        if (0x19 < bVar1 - 0x41) {
          uVar15 = (uint)bVar1;
        }
        if (uVar15 != uVar10) {
          pbVar11 = pbVar26;
          if (uVar15 == uVar10) goto LAB_001d8ba0;
          goto LAB_001d8c24;
        }
        lVar25 = lVar5 + 1;
        pbVar11 = &DAT_0010d097 + lVar5;
        lVar5 = lVar25;
        uVar8 = (uint)*pbVar11;
      } while (lVar25 != 3);
      pbVar20 = pbVar26 + 3;
      pbVar11 = pbVar26 + 3;
      if (uVar15 != uVar10) goto LAB_001d8c24;
LAB_001d8ba0:
      if (param_1[3] != 0) {
        uVar6 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
        param_1[3] = 0;
        param_1[4] = uVar6;
      }
      pbVar26 = (byte *)0x0;
      *(undefined4 *)(param_1 + 1) = uVar23;
      param_1[2] = 0x7ffffffffffffffe;
    }
  }
  else {
    lVar5 = 0;
    uVar8 = 0x6e;
    do {
      uVar10 = uVar8;
      bVar1 = param_3[lVar5];
      uVar15 = bVar1 + 0x20;
      if (0x19 < bVar1 - 0x41) {
        uVar15 = (uint)bVar1;
      }
      pbVar20 = param_3;
      if (uVar15 != uVar10) break;
      lVar25 = lVar5 + 1;
      pbVar26 = &DAT_0010cd8f + lVar5;
      lVar5 = lVar25;
      pbVar20 = param_3 + 3;
      uVar8 = (uint)*pbVar26;
    } while (lVar25 != 3);
    if (uVar15 != uVar10) goto LAB_001d8a7c;
LAB_001d9074:
    if (param_1[3] != 0) {
      uVar6 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
      param_1[3] = 0;
      param_1[4] = uVar6;
    }
    pbVar26 = (byte *)0x0;
    pbVar11 = pbVar20;
LAB_001d94ec:
    param_1[2] = 0x7fffffffffffffff;
    *(undefined4 *)(param_1 + 1) = 0;
  }
LAB_001d94f4:
  if (param_4 != (long *)0x0) {
    *param_4 = (long)pbVar11;
  }
LAB_001d94fc:
  if (*(long *)(lVar2 + 0x28) == local_70) {
    return (ulong)pbVar26 & 0xffffffff;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001d96ac @ 001d96ac [libNexusScriptRuntime69252.so] ===== */

void FUN_001d96ac(long *param_1,undefined8 **param_2,uint param_3,undefined8 *param_4,uint param_5,
                 int param_6)

{
  bool bVar1;
  long lVar2;
  uint uVar3;
  long lVar4;
  undefined1 auVar5 [16];
  undefined1 auVar6 [16];
  undefined1 auVar7 [16];
  undefined1 auVar8 [16];
  undefined1 auVar9 [16];
  long lVar10;
  int iVar11;
  uint uVar12;
  void *pvVar13;
  char *pcVar14;
  undefined *puVar15;
  long lVar16;
  undefined4 uVar17;
  undefined8 *puVar18;
  long lVar19;
  ulong uVar20;
  ulong uVar21;
  ulong uVar22;
  undefined8 *puVar23;
  undefined8 *puVar24;
  undefined8 *puVar25;
  long lVar26;
  undefined8 *puVar27;
  undefined8 *puVar28;
  int iVar29;
  ulong uVar30;
  undefined2 uVar31;
  undefined8 uVar32;
  undefined1 auStack_128 [8];
  undefined4 local_120;
  undefined8 *local_118;
  undefined8 *local_110;
  undefined8 *puStack_108;
  undefined8 *local_100;
  undefined4 local_f8;
  undefined8 *local_f0;
  undefined8 *local_e8;
  undefined8 *local_e0;
  undefined8 *local_d8;
  undefined4 local_d0;
  undefined8 *local_c8;
  undefined8 *local_c0;
  void *local_b8;
  undefined8 *local_b0;
  undefined8 *local_a8;
  void *local_a0;
  ulong local_98;
  int local_88;
  long local_70;
  
  lVar10 = tpidr_el0;
  local_70 = *(long *)(lVar10 + 0x28);
  puVar25 = *param_2;
  FUN_001d2548(&local_a0,puVar25,FUN_001e310c);
  if (param_2[2] == (undefined8 *)0x7fffffffffffffff) {
    pcVar14 = "NaN";
LAB_001d9760:
    FUN_001d2864(&local_a0,pcVar14);
LAB_001d9768:
    FUN_001d27c4(&local_a0,0);
    if (local_88 == 0) {
      pvVar13 = local_a0;
      if (param_1 != (long *)0x0) {
        *param_1 = local_98 - 1;
      }
      goto LAB_001da2f4;
    }
  }
  else {
    if (*(int *)(param_2 + 1) != 0) {
      FUN_001d27c4(&local_a0,0x2d);
    }
    puVar18 = param_2[2];
    if (puVar18 == (undefined8 *)0x7ffffffffffffffe) {
      pcVar14 = "Inf";
      if ((param_5 & 0x400000) != 0) {
        pcVar14 = "Infinity";
      }
      goto LAB_001d9760;
    }
    uVar32 = NEON_cnt((ulong)param_3,1);
    uVar31 = NEON_uaddlv(uVar32,1);
    uVar12 = (uint)CONCAT62((int6)((ulong)uVar32 >> 0x10),uVar31);
    if (uVar12 < 2) {
      uVar3 = 0;
      if (1 < param_3) {
        uVar3 = 0x40 - (int)LZCOUNT((long)(int)param_3 + -1);
      }
      uVar30 = (ulong)uVar3;
    }
    else {
      uVar30 = 0;
    }
    local_d0 = 0;
    local_c0 = (undefined8 *)0x0;
    local_b8 = (void *)0x0;
    local_c8 = (undefined8 *)0x8000000000000000;
    iVar29 = (int)uVar30;
    pvVar13 = local_b8;
    local_d8 = puVar25;
    if ((param_5 & 0x30000) == 0x10000) {
      if (param_6 == 0 && iVar29 == 0) {
        local_e8 = param_2[3];
        local_e0 = param_2[4];
        local_f8 = 0;
        puVar27 = puVar18;
        if ((long)puVar18 < 1) {
          puVar27 = (undefined8 *)0x0;
        }
        if (uVar12 < 2) {
          iVar29 = 0;
          if (1 < param_3) {
            iVar29 = 0x40 - (int)LZCOUNT((ulong)param_3 - 1);
          }
          uVar30 = 0;
          if ((long)iVar29 != 0) {
            uVar30 = (ulong)((long)puVar27 + (long)(iVar29 + -1)) / (ulong)(long)iVar29;
          }
        }
        else {
          lVar26 = (ulong)(param_3 - 2) * 0xc;
          uVar21 = CONCAT44(*(undefined4 *)(&DAT_0011daec + lVar26),
                            *(undefined4 *)(&DAT_0011daf0 + lVar26)) * (long)puVar27;
          auVar8._8_8_ = 0;
          auVar8._0_8_ = puVar27;
          uVar30 = SUB168((ZEXT416(*(uint *)(&DAT_0011daf4 + lVar26)) << 0x20) * auVar8,8);
          auVar6._8_8_ = 0;
          auVar6._0_8_ = CONCAT44(*(undefined4 *)(&DAT_0011daec + lVar26),
                                  *(undefined4 *)(&DAT_0011daf0 + lVar26));
          auVar9._8_8_ = 0;
          auVar9._0_8_ = puVar27;
          lVar26 = SUB168(auVar6 * auVar9,8);
          if (CARRY8(uVar30,uVar21)) {
            lVar26 = lVar26 + 1;
          }
          uVar30 = (uVar30 + uVar21 >> 0x3f | lVar26 << 1) + 1;
        }
        puVar27 = (undefined8 *)(uVar30 + 1);
        local_f0 = puVar18;
        local_b0 = puVar27;
        local_a8 = puVar27;
        pvVar13 = (void *)FUN_001e365c(&local_d8,&local_b0,&local_100,param_3,
                                       (long)puVar27 + (long)param_4,param_5 & 7,1);
        uVar30 = local_98;
        if ((int)pvVar13 != 0) goto LAB_001da2c8;
        pvVar13 = (void *)FUN_001e3118(&local_a0,&local_d8,param_3,(long)puVar27 + (long)param_4,
                                       puVar27,0);
        uVar21 = local_98;
        if (local_98 <= uVar30 + 1) {
          uVar21 = uVar30 + 1;
        }
        uVar22 = uVar30;
        do {
          uVar20 = uVar21 - 1;
          if ((local_98 <= uVar22 + 1) ||
             (uVar20 = uVar22, *(char *)((long)local_a0 + uVar22) != '0')) break;
          lVar26 = uVar22 + 1;
          uVar22 = uVar22 + 1;
        } while (*(char *)((long)local_a0 + lVar26) != '.');
        if (uVar30 < uVar20) {
          pvVar13 = memmove((void *)((long)local_a0 + uVar30),(void *)((long)local_a0 + uVar20),
                            local_98 - uVar20);
          local_98 = (uVar30 - uVar20) + local_98;
        }
        goto LAB_001da1f0;
      }
      if (&local_d8 != param_2) {
        puVar27 = param_2[3];
        puVar18 = local_c0;
        if (((puVar27 != (undefined8 *)0x0) &&
            (pvVar13 = (void *)(*(code *)puVar25[1])(*puVar25,0,(long)puVar27 << 3),
            puVar18 = puVar27, puVar27 != (undefined8 *)0x0)) && (pvVar13 == (void *)0x0))
        goto LAB_001d9a6c;
        local_b8 = pvVar13;
        local_c0 = puVar18;
        local_c8 = param_2[2];
        local_d0 = *(undefined4 *)(param_2 + 1);
        memcpy(local_b8,param_2[4],(long)param_2[3] << 3);
      }
      if (param_6 == 0) {
        lVar26 = (long)iVar29;
        if ((local_c0 != (undefined8 *)0x0) &&
           (pvVar13 = (void *)FUN_001d3a6c(&local_d8,lVar26 * (long)param_4,param_5 & 7 | 0x10,
                                           local_c0,0), ((uint)pvVar13 >> 5 & 1) != 0))
        goto LAB_001da2c8;
        lVar19 = 0;
        if (-1 < (long)local_c8) {
          lVar19 = lVar26 + -1;
        }
        puVar28 = (undefined8 *)0x0;
        if (lVar26 != 0) {
          puVar28 = (undefined8 *)(((long)local_c8 + lVar19) / lVar26);
        }
LAB_001d9bd8:
        local_a8 = puVar28;
        if ((param_5 >> 0x15 & 1) != 0) {
          if (param_3 == 2) {
            puVar15 = &DAT_00110a16;
          }
          else if (param_3 == 0x10) {
            puVar15 = &DAT_0010d09a;
          }
          else {
            if (param_3 != 8) goto LAB_001d9f4c;
            puVar15 = &DAT_0010eec1;
          }
          FUN_001d2864(&local_a0,puVar15);
        }
LAB_001d9f4c:
        if (local_c8 == (undefined8 *)0x8000000000000000) {
          pvVar13 = (void *)FUN_001d2864(&local_a0,&DAT_0010b705);
          if (param_4 != (undefined8 *)0x0) {
            FUN_001d2864(&local_a0,&DAT_00110f96);
            do {
              pvVar13 = (void *)FUN_001d27c4(&local_a0,0x30);
              param_4 = (undefined8 *)((long)param_4 + -1);
            } while (param_4 != (undefined8 *)0x0);
          }
          goto LAB_001da1f0;
        }
        puVar27 = (undefined8 *)((long)puVar28 + (long)param_4);
        if ((long)puVar28 < 1) {
          pvVar13 = (void *)FUN_001d2864(&local_a0,&DAT_0010c1c9);
          if ((long)puVar28 < 0) {
            lVar26 = 1;
            if ((long)puVar28 < -1) {
              lVar26 = -(long)puVar28;
            }
            do {
              pvVar13 = (void *)FUN_001d27c4(&local_a0,0x30);
              lVar26 = lVar26 + -1;
            } while (lVar26 != 0);
          }
          puVar28 = puVar27;
          if ((long)puVar27 < 1) goto LAB_001da1f0;
        }
        goto LAB_001da1e8;
      }
      puVar28 = local_c8;
      if ((local_c0 == (undefined8 *)0x0) ||
         (pvVar13 = (void *)FUN_001dfc74(&local_d8,param_4,param_5 & 7 | 0x10), puVar28 = local_c8,
         ((uint)pvVar13 >> 5 & 1) == 0)) goto LAB_001d9bd8;
LAB_001da2c8:
      iVar11 = 0x11;
joined_r0x001da1f8:
      if ((local_d8 != (undefined8 *)0x0) && (local_b8 != (void *)0x0)) {
        pvVar13 = (void *)(*(code *)local_d8[1])(*local_d8,local_b8,0);
      }
    }
    else {
      puVar27 = param_4;
      if (param_6 != 0) {
        if (&local_d8 != param_2) {
          puVar28 = param_2[3];
          puVar18 = local_c0;
          if (((puVar28 != (undefined8 *)0x0) &&
              (pvVar13 = (void *)(*(code *)puVar25[1])(*puVar25,0,(long)puVar28 << 3),
              puVar18 = puVar28, puVar28 != (undefined8 *)0x0)) && (pvVar13 == (void *)0x0)) {
LAB_001d9a6c:
            if (local_c0 != (undefined8 *)0x0) {
              pvVar13 = (void *)(*(code *)local_d8[1])(*local_d8,local_b8,0);
              local_c0 = (undefined8 *)0x0;
              local_b8 = pvVar13;
            }
            local_d0 = 0;
            local_c8 = (undefined8 *)0x7fffffffffffffff;
            goto LAB_001da2c8;
          }
          local_b8 = pvVar13;
          local_c0 = puVar18;
          local_c8 = param_2[2];
          local_d0 = *(undefined4 *)(param_2 + 1);
          memcpy(local_b8,param_2[4],(long)param_2[3] << 3);
        }
        if ((param_5 & 0x30000) != 0) {
          puVar27 = (undefined8 *)((long)local_c0 * 0x13);
          if (1 < (long)puVar27) {
            lVar26 = 0;
            uVar30 = 0;
            do {
              puVar18 = (undefined8 *)(uVar30 / 0x13);
              if (puVar18 < local_c0) {
                uVar21 = *(ulong *)((long)local_b8 + (long)puVar18 * 8);
                lVar19 = lVar26 + (long)puVar18 * -0x1300000000 >> 0x1c;
                auVar5._8_8_ = 0;
                auVar5._0_8_ = *(ulong *)((long)&DAT_0011e000 + lVar19);
                auVar7._8_8_ = 0;
                auVar7._0_8_ = uVar21;
                lVar16 = SUB168(auVar5 * auVar7,8);
                uVar21 = ((uVar21 - lVar16 >> ((long)(char)(&DAT_0011e008)[lVar19] & 0x3fU)) +
                          lVar16 >> ((long)(char)(&DAT_0011e009)[lVar19] & 0x3fU)) % 10;
              }
              else {
                uVar21 = 0;
              }
              if (uVar21 != 0) goto LAB_001d99d4;
              uVar30 = uVar30 + 1;
              lVar26 = lVar26 + 0x100000000;
              bVar1 = 2 < (long)puVar27;
              puVar27 = (undefined8 *)((long)puVar27 + -1);
            } while (bVar1);
            puVar27 = (undefined8 *)0x1;
          }
LAB_001d99d4:
          param_4 = (undefined8 *)((long)puVar27 + 4);
          puVar18 = local_c8;
          goto LAB_001da040;
        }
        if (local_c0 == (undefined8 *)0x0) {
          pvVar13 = (void *)0x0;
        }
        else {
          pvVar13 = (void *)FUN_001dfc74(&local_d8,param_4,param_5 & 7);
        }
        uVar12 = (uint)pvVar13 >> 5 & 1;
        puVar18 = local_c8;
joined_r0x001da2c4:
        if (uVar12 != 0) goto LAB_001da2c8;
LAB_001da040:
        local_a8 = puVar18;
        if ((((param_5 >> 0x14 & 1) == 0) && ((param_5 & 0x30000) != 0)) &&
           (local_c8 == (undefined8 *)0x8000000000000000)) {
          pvVar13 = (void *)FUN_001d2864(&local_a0,&DAT_0010b705);
        }
        else {
          if ((param_5 >> 0x15 & 1) != 0) {
            if (param_3 == 2) {
              puVar15 = &DAT_00110a16;
            }
            else if (param_3 == 0x10) {
              puVar15 = &DAT_0010d09a;
            }
            else {
              if (param_3 != 8) goto LAB_001da0b0;
              puVar15 = &DAT_0010eec1;
            }
            FUN_001d2864(&local_a0,puVar15);
          }
LAB_001da0b0:
          if (local_c8 == (undefined8 *)0x8000000000000000) {
            local_a8 = (undefined8 *)0x1;
          }
          puVar18 = local_a8;
          if ((((param_5 >> 0x14 & 1) == 0) && ((long)local_a8 + 5 < 0 == SCARRY8((long)local_a8,5))
              ) && ((long)local_a8 <= (long)param_4)) {
            if ((long)local_a8 < 1) {
              FUN_001d2864(&local_a0,&DAT_0010c1c9);
              puVar28 = puVar27;
              if ((long)puVar18 < 0) {
                lVar26 = 1;
                if ((long)puVar18 < -1) {
                  lVar26 = -(long)puVar18;
                }
                do {
                  FUN_001d27c4(&local_a0,0x30);
                  lVar26 = lVar26 + -1;
                } while (lVar26 != 0);
              }
            }
            else {
              lVar26 = (long)local_a8 - (long)puVar27;
              puVar28 = local_a8;
              if ((long)puVar27 <= (long)local_a8) {
                pvVar13 = (void *)FUN_001e3118(&local_a0,&local_d8,param_3,puVar27,puVar27,param_6);
                if (0 < lVar26) {
                  do {
                    pvVar13 = (void *)FUN_001d27c4(&local_a0,0x30);
                    lVar26 = lVar26 + -1;
                  } while (lVar26 != 0);
                }
                goto LAB_001da1f0;
              }
            }
LAB_001da1e8:
            pvVar13 = (void *)FUN_001e3118(&local_a0,&local_d8,param_3,puVar27,puVar28,param_6);
          }
          else {
            FUN_001e3118(&local_a0,&local_d8,param_3,puVar27,1,param_6);
            if (((int)param_3 < 0x11) && (iVar29 != 0)) {
              pcVar14 = "p%ld";
              if ((param_5 & 0x400000) != 0) {
                pcVar14 = "p%+ld";
              }
              pvVar13 = (void *)FUN_001d291c(&local_a0,pcVar14,((long)puVar18 + -1) * (long)iVar29);
            }
            else {
              pcVar14 = "%c%ld";
              if ((param_5 & 0x400000) != 0) {
                pcVar14 = "%c%+ld";
              }
              uVar17 = 0x65;
              if (10 < (int)param_3) {
                uVar17 = 0x40;
              }
              pvVar13 = (void *)FUN_001d291c(&local_a0,pcVar14,uVar17,(long)puVar18 + -1);
            }
          }
        }
LAB_001da1f0:
        iVar11 = 0;
        goto joined_r0x001da1f8;
      }
      if (iVar29 != 0) {
        if (&local_d8 != param_2) {
          puVar28 = param_2[3];
          puVar18 = local_c0;
          if (((puVar28 != (undefined8 *)0x0) &&
              (pvVar13 = (void *)(*(code *)puVar25[1])(*puVar25,0,(long)puVar28 << 3),
              puVar18 = puVar28, puVar28 != (undefined8 *)0x0)) && (pvVar13 == (void *)0x0))
          goto LAB_001d9a6c;
          local_b8 = pvVar13;
          local_c0 = puVar18;
          local_c8 = param_2[2];
          local_d0 = *(undefined4 *)(param_2 + 1);
          memcpy(local_b8,param_2[4],(long)param_2[3] << 3);
        }
        if ((param_5 & 0x30000) == 0) {
          if (local_c0 == (undefined8 *)0x0) {
            pvVar13 = (void *)0x0;
          }
          else {
            lVar26 = (long)iVar29;
            lVar19 = 0;
            if (lVar26 != 0) {
              lVar19 = (long)local_c8 / lVar26;
            }
            lVar16 = (long)local_c8 - lVar19 * lVar26;
            lVar19 = lVar26 * (long)param_4;
            if (lVar16 < 1) {
              lVar26 = 0;
            }
            pvVar13 = (void *)FUN_001d3a6c(&local_d8,(lVar16 + lVar19) - lVar26,param_5 & 7,local_c0
                                           ,0);
          }
          if (((uint)pvVar13 >> 5 & 1) != 0) goto LAB_001da2c8;
        }
        else {
          lVar19 = (long)iVar29;
          lVar26 = 0;
          if (lVar19 != 0) {
            lVar26 = (long)local_c8 / lVar19;
          }
          lVar16 = (long)local_c8 - lVar26 * lVar19;
          lVar26 = lVar19;
          if (lVar16 < 1) {
            lVar26 = 0;
          }
          lVar2 = (lVar26 - lVar16) + (long)local_c0 * 0x40;
          lVar4 = 0;
          if (-1 < lVar2) {
            lVar4 = lVar19 + -1;
          }
          puVar27 = (undefined8 *)0x0;
          if (lVar19 != 0) {
            puVar27 = (undefined8 *)((lVar2 + lVar4) / lVar19);
          }
          if (1 < (long)puVar27) {
            uVar21 = ((lVar26 + (long)local_c0 * 0x40) - (long)puVar27 * lVar19) - lVar16;
            do {
              puVar18 = (undefined8 *)((long)uVar21 >> 6);
              if (puVar18 < local_c0) {
                uVar22 = *(ulong *)((long)local_b8 + (long)puVar18 * 8);
              }
              else {
                uVar22 = 0;
              }
              if ((uVar21 & 0x3f) != 0) {
                if ((undefined8 *)((long)puVar18 + 1U) < local_c0) {
                  lVar26 = *(long *)((long)local_b8 + (long)((long)puVar18 + 1U) * 8);
                }
                else {
                  lVar26 = 0;
                }
                uVar22 = lVar26 << ((ulong)-((uint)uVar21 & 0x3f) & 0x3f) |
                         uVar22 >> (uVar21 & 0x3f);
              }
              if ((uVar22 & ~(-1L << (uVar30 & 0x3f))) != 0) goto LAB_001d9eec;
              uVar21 = uVar21 + lVar19;
              bVar1 = 2 < (long)puVar27;
              puVar27 = (undefined8 *)((long)puVar27 + -1);
            } while (bVar1);
            puVar27 = (undefined8 *)0x1;
          }
LAB_001d9eec:
          param_4 = (undefined8 *)((long)puVar27 + 4);
        }
        lVar19 = (long)iVar29;
        lVar26 = 0;
        if (-1 < (long)local_c8) {
          lVar26 = lVar19 + -1;
        }
        puVar18 = (undefined8 *)0x0;
        if (lVar19 != 0) {
          puVar18 = (undefined8 *)(((long)local_c8 + lVar26) / lVar19);
        }
        goto LAB_001da040;
      }
      local_110 = param_2[3];
      puStack_108 = param_2[4];
      local_120 = 0;
      local_118 = puVar18;
      if ((param_5 & 0x30000) == 0) {
LAB_001da2a4:
        pvVar13 = (void *)FUN_001e365c(&local_d8,&local_a8,auStack_128,param_3,puVar27,param_5 & 7,0
                                      );
        uVar12 = (uint)pvVar13;
        puVar18 = local_a8;
        goto joined_r0x001da2c4;
      }
      if (param_4 == (undefined8 *)0x3fffffffffffffff) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
                  ,0xf3a,
                  "char *bf_ftoa_internal(size_t *, const bf_t *, int, limb_t, bf_flags_t, BOOL)",
                  "prec != BF_PREC_INF");
      }
      pvVar13 = (void *)FUN_001d95d4(param_4,param_3,1,1);
      puVar27 = (undefined8 *)((long)pvVar13 + 1);
      if ((param_5 & 0x30000) == 0x30000) {
        bVar1 = 0 < (long)pvVar13;
        local_f8 = 0;
        local_e8 = (undefined8 *)0x0;
        local_e0 = (undefined8 *)0x0;
        local_f0 = (undefined8 *)0x8000000000000000;
        local_100 = puVar25;
        if (0 < (long)pvVar13) {
          lVar26 = 1;
          puVar18 = puVar27;
          do {
            lVar19 = lVar26 + (long)puVar18;
            if (lVar19 < 0) {
              lVar19 = lVar19 + 1;
            }
            puVar27 = (undefined8 *)(lVar19 >> 1);
            iVar11 = FUN_001e365c(&local_d8,&local_a8,auStack_128,param_3,puVar27,param_5 & 7,0);
            if ((iVar11 != 0) ||
               (uVar12 = FUN_001d83a8(&local_100,&local_d8,(long)(int)param_3,
                                      (long)local_a8 - (long)puVar27,param_4,param_5 & 0xfffffff8),
               (uVar12 >> 5 & 1) != 0)) {
              iVar11 = 2;
              if ((local_100 == (undefined8 *)0x0) || (local_e0 == (undefined8 *)0x0))
              goto joined_r0x001da334;
              goto LAB_001da274;
            }
            if (local_f0 == local_118) {
              puVar28 = local_e8;
              puVar23 = local_110;
              puVar24 = local_e8;
              if ((long)local_e8 <= (long)local_110) {
                puVar28 = local_110;
              }
              do {
                puVar24 = (undefined8 *)((long)puVar24 + -1);
                puVar23 = (undefined8 *)((long)puVar23 + -1);
                puVar28 = (undefined8 *)((long)puVar28 + -1);
                uVar12 = (uint)((ulong)puVar28 >> 0x3f);
                if ((long)puVar28 < 0) break;
                if (puVar24 < local_e8) {
                  lVar19 = local_e0[(long)puVar24];
                  if (local_110 <= puVar23) goto LAB_001d9d84;
LAB_001d9d40:
                  lVar16 = puStack_108[(long)puVar23];
                }
                else {
                  lVar19 = 0;
                  if (puVar23 < local_110) goto LAB_001d9d40;
LAB_001d9d84:
                  lVar16 = 0;
                }
              } while (lVar19 == lVar16);
            }
            else {
              uVar12 = 0;
            }
            if (uVar12 == 0) {
              lVar26 = (long)puVar27 + 1;
              puVar27 = puVar18;
            }
            bVar1 = lVar26 < (long)puVar27;
            puVar18 = puVar27;
          } while (lVar26 < (long)puVar27);
        }
        iVar11 = 0;
        if ((local_100 != (undefined8 *)0x0) && (local_e0 != (undefined8 *)0x0)) {
LAB_001da274:
          (*(code *)local_100[1])(*local_100,local_e0,0);
        }
joined_r0x001da334:
        if (!bVar1) goto LAB_001da290;
      }
      else {
LAB_001da290:
        iVar11 = 0;
      }
      if (iVar11 == 2) goto LAB_001da2c8;
      if (iVar11 == 0) {
        param_4 = (undefined8 *)((long)pvVar13 + 5);
        goto LAB_001da2a4;
      }
    }
    if (iVar11 == 0) goto LAB_001d9768;
    if (iVar11 != 0x11) goto LAB_001da2f4;
  }
  if (local_a0 != (void *)0x0) {
    (*(code *)puVar25[1])(*puVar25,local_a0,0);
  }
  pvVar13 = (void *)0x0;
  if (param_1 != (long *)0x0) {
    *param_1 = 0;
  }
LAB_001da2f4:
  if (*(long *)(lVar10 + 0x28) == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(pvVar13);
}

/* ===== FUN_001da718 @ 001da718 [libNexusScriptRuntime69252.so] ===== */

undefined8 FUN_001da718(undefined8 *param_1,undefined8 *param_2,long param_3,undefined4 param_4)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  long lVar3;
  
  if (param_1 == param_2) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
              ,0x114a,"int bf_exp(bf_t *, const bf_t *, limb_t, bf_flags_t)","r != a");
  }
  puVar1 = (undefined8 *)*param_1;
  if (param_2[3] != 0) {
    uVar2 = FUN_001da980(puVar1,param_1,param_2,param_2,param_3,param_4);
    if ((int)uVar2 != 0) {
      return uVar2;
    }
    if ((-1 < (long)param_2[2]) || ((ulong)-param_2[2] < param_3 + 2U)) {
      uVar2 = FUN_001daf60(param_1,param_2,param_3,param_4,FUN_001db16c,0);
      return uVar2;
    }
    *(undefined4 *)(param_1 + 1) = 0;
    if (param_1[3] != 1) {
      lVar3 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],8);
      if (lVar3 == 0) {
        if (param_1[3] != 0) {
          uVar2 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
          param_1[3] = 0;
          param_1[4] = uVar2;
        }
        *(undefined4 *)(param_1 + 1) = 0;
        param_1[2] = 0x7fffffffffffffff;
        goto LAB_001da8e4;
      }
      param_1[3] = 1;
      param_1[4] = lVar3;
    }
    *(undefined8 *)param_1[4] = 0x8000000000000000;
    param_1[2] = 1;
LAB_001da8e4:
    uVar2 = FUN_001dae40(param_1,param_1,-2 - param_3,*(undefined4 *)(param_2 + 1),param_3,param_4);
    return uVar2;
  }
  if (param_2[2] == 0x7ffffffffffffffe) {
    if (*(int *)(param_2 + 1) == 0) {
      if (param_1[3] != 0) {
        uVar2 = (*(code *)puVar1[1])(*puVar1,param_1[4],0);
        param_1[3] = 0;
        param_1[4] = uVar2;
      }
      uVar2 = 0x7ffffffffffffffe;
    }
    else {
      if (param_1[3] != 0) {
        uVar2 = (*(code *)puVar1[1])(*puVar1,param_1[4],0);
        param_1[3] = 0;
        param_1[4] = uVar2;
      }
      uVar2 = 0x8000000000000000;
    }
    goto LAB_001da888;
  }
  if (param_2[2] == 0x7fffffffffffffff) {
    if (param_1[3] != 0) {
      uVar2 = (*(code *)puVar1[1])(*puVar1,param_1[4],0);
LAB_001da7e0:
      param_1[3] = 0;
      param_1[4] = uVar2;
    }
  }
  else {
    *(undefined4 *)(param_1 + 1) = 0;
    if (param_1[3] == 1) {
LAB_001da84c:
      *(undefined8 *)param_1[4] = 0x8000000000000000;
      param_1[2] = 1;
      return 0;
    }
    lVar3 = (*(code *)puVar1[1])(*puVar1,param_1[4],8);
    if (lVar3 != 0) {
      param_1[3] = 1;
      param_1[4] = lVar3;
      goto LAB_001da84c;
    }
    if (param_1[3] != 0) {
      uVar2 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
      goto LAB_001da7e0;
    }
  }
  uVar2 = 0x7fffffffffffffff;
LAB_001da888:
  param_1[2] = uVar2;
  *(undefined4 *)(param_1 + 1) = 0;
  return 0;
}

/* ===== FUN_001db16c @ 001db16c [libNexusScriptRuntime69252.so] ===== */

undefined8 FUN_001db16c(undefined8 *param_1,undefined8 *param_2,long param_3)

{
  bool bVar1;
  ulong uVar2;
  long lVar3;
  ulong uVar4;
  long lVar5;
  long *plVar6;
  undefined8 uVar7;
  long lVar8;
  ulong uVar9;
  long lVar10;
  undefined8 *puVar11;
  undefined8 *local_b8;
  undefined4 local_b0;
  long local_a8;
  long local_a0;
  long *local_98;
  undefined8 *local_90;
  int local_88;
  long local_80;
  long local_78;
  long local_70;
  long local_68;
  
  lVar3 = tpidr_el0;
  local_68 = *(long *)(lVar3 + 0x28);
  if (param_1 == param_2) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
              ,0x10d4,"int bf_exp_internal(bf_t *, const bf_t *, limb_t, void *)","r != a");
  }
  puVar11 = (undefined8 *)*param_1;
  local_88 = 0;
  local_78 = 0;
  local_70 = 0;
  local_80 = -0x8000000000000000;
  local_90 = puVar11;
  if ((long)param_2[2] < 0) {
    uVar9 = -(ulong)(*(int *)(param_2 + 1) != 0);
  }
  else {
    FUN_001da384(&local_90,0x40,1,puVar11 + 2,FUN_001da608,0);
    FUN_001d7210(&local_90,param_2,&local_90,0x40,2,FUN_001d72fc);
    if (local_80 < 0x7ffffffffffffffe) {
      if (local_80 < 1) {
        uVar9 = 0;
      }
      else if (local_80 < 0x40) {
        uVar4 = *(ulong *)(local_70 + local_78 * 8 + -8) >> (-local_80 & 0x3fU);
        uVar9 = -uVar4;
        if (local_88 == 0) {
          uVar9 = uVar4;
        }
      }
      else {
        uVar9 = 0x7fffffffffffffff;
        if (local_88 != 0) {
          uVar9 = 0x8000000000000000;
        }
      }
    }
    else {
      uVar9 = 0x7fffffffffffffff;
      if (local_80 == 0x7ffffffffffffffe) {
        uVar9 = (long)local_88 + 0x7fffffffffffffff;
      }
    }
  }
  uVar4 = FUN_001d6604(param_3 + 1U >> 1);
  uVar2 = 0;
  if (uVar4 != 0) {
    uVar2 = (param_3 - 1U) / uVar4;
  }
  lVar8 = param_2[2];
  if (lVar8 < 1) {
    lVar8 = 0;
  }
  lVar10 = uVar2 + 1;
  lVar8 = param_3 + lVar8 + (lVar10 + uVar4) * 2 + 0x1a;
  FUN_001da384(&local_90,lVar8,6,local_90 + 2,FUN_001da608,0);
  FUN_001d7734(&local_90,&local_90,uVar9,lVar8,0);
  FUN_001e71f4(&local_90,param_2,&local_90,lVar8,0);
  if (local_78 != 0) {
    lVar5 = -0x3fffffffffffffff;
    if (uVar4 != 0x3fffffffffffffff && -0x4000000000000000 < (long)-uVar4) {
      lVar5 = -uVar4;
    }
    if (0x3ffffffffffffffe < lVar5) {
      lVar5 = 0x3fffffffffffffff;
    }
    local_80 = local_80 + lVar5;
    FUN_001d3a6c(&local_90,0x3fffffffffffffff,1,local_78,0);
  }
  local_b0 = 0;
  local_a0 = 0;
  local_98 = (long *)0x0;
  local_a8 = -0x8000000000000000;
  *(undefined4 *)(param_1 + 1) = 0;
  local_b8 = puVar11;
  if (param_1[3] != 1) {
    lVar5 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],8);
    if (lVar5 == 0) {
      if (param_1[3] != 0) {
        uVar7 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
        param_1[3] = 0;
        param_1[4] = uVar7;
      }
      *(undefined4 *)(param_1 + 1) = 0;
      param_1[2] = 0x7fffffffffffffff;
      goto LAB_001db3b0;
    }
    param_1[3] = 1;
    param_1[4] = lVar5;
  }
  *(undefined8 *)param_1[4] = 0x8000000000000000;
  param_1[2] = 1;
LAB_001db3b0:
  if (uVar2 < 0x7fffffffffffffff) {
    do {
      local_b0 = 0;
      if (local_a0 == 1) {
LAB_001db3d8:
        local_a8 = 0x40 - LZCOUNT(lVar10);
        *local_98 = lVar10 << (LZCOUNT(lVar10) & 0x3fU);
      }
      else {
        plVar6 = (long *)(*(code *)local_b8[1])(*local_b8,local_98,8);
        if (plVar6 != (long *)0x0) {
          local_a0 = 1;
          local_98 = plVar6;
          goto LAB_001db3d8;
        }
        if (local_a0 != 0) {
          local_98 = (long *)(*(code *)local_b8[1])(*local_b8,local_98,0);
          local_a0 = 0;
        }
        local_a8 = 0x7fffffffffffffff;
        local_b0 = 0;
      }
      FUN_001d7210(&local_b8,&local_90,&local_b8,lVar8,0,FUN_001d72fc);
      FUN_001d56bc(param_1,param_1,&local_b8,lVar8,0);
      FUN_001d6220(param_1,param_1,1,lVar8,0);
      lVar5 = lVar10 + -1;
      bVar1 = 0 < lVar10;
      lVar10 = lVar5;
    } while (lVar5 != 0 && bVar1);
  }
  if ((local_b8 != (undefined8 *)0x0) && (local_98 != (long *)0x0)) {
    (*(code *)local_b8[1])(*local_b8,local_98,0);
  }
  if ((local_90 != (undefined8 *)0x0) && (local_70 != 0)) {
    (*(code *)local_90[1])(*local_90,local_70,0);
  }
  if (0 < (long)uVar4) {
    do {
      FUN_001d56bc(param_1,param_1,param_1,lVar8,0x7e0);
      uVar4 = uVar4 - 1;
    } while (uVar4 != 0);
  }
  if (param_1[3] != 0) {
    if ((long)uVar9 < -0x3ffffffffffffffe) {
      uVar9 = 0xc000000000000001;
    }
    if (0x3ffffffffffffffe < (long)uVar9) {
      uVar9 = 0x3fffffffffffffff;
    }
    param_1[2] = param_1[2] + uVar9;
    FUN_001d3a6c(param_1,0x3fffffffffffffff,0x7e1,param_1[3],0);
  }
  if (*(long *)(lVar3 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return 0x10;
}

/* ===== FUN_001db5c0 @ 001db5c0 [libNexusScriptRuntime69252.so] ===== */

undefined8
FUN_001db5c0(undefined8 *param_1,undefined8 *param_2,undefined8 param_3,undefined4 param_4)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  long lVar3;
  undefined8 uVar4;
  ulong uVar5;
  ulong uVar6;
  ulong uVar7;
  ulong uVar8;
  long lVar9;
  undefined8 *puVar10;
  
  if (param_1 == param_2) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
              ,0x11cd,"int bf_log(bf_t *, const bf_t *, limb_t, bf_flags_t)","r != a");
  }
  puVar10 = (undefined8 *)*param_1;
  if (param_2[3] == 0) {
    if (param_2[2] != 0x7ffffffffffffffe) {
      if (param_2[2] != 0x7fffffffffffffff) {
        if (param_1[3] != 0) {
          uVar1 = (*(code *)puVar10[1])(*puVar10,param_1[4],0);
          param_1[3] = 0;
          param_1[4] = uVar1;
        }
        param_1[2] = 0x7ffffffffffffffe;
        *(undefined4 *)(param_1 + 1) = 1;
        return 0;
      }
      if (param_1[3] != 0) {
        uVar1 = (*(code *)puVar10[1])(*puVar10,param_1[4],0);
        param_1[3] = 0;
        param_1[4] = uVar1;
      }
      uVar1 = 0;
      uVar4 = 0x7fffffffffffffff;
      goto LAB_001db7dc;
    }
    lVar3 = param_1[3];
    if (*(int *)(param_2 + 1) == 0) {
      if (lVar3 != 0) {
        uVar1 = (*(code *)puVar10[1])(*puVar10,param_1[4],0);
        param_1[3] = 0;
        param_1[4] = uVar1;
      }
      uVar1 = 0;
      uVar4 = 0x7ffffffffffffffe;
      goto LAB_001db7dc;
    }
  }
  else {
    if (*(int *)(param_2 + 1) == 0) {
      puVar2 = (undefined8 *)(*(code *)puVar10[1])(*puVar10,0,8);
      if (puVar2 == (undefined8 *)0x0) {
        puVar2 = (undefined8 *)0x0;
      }
      else {
        *puVar2 = 0x8000000000000000;
        if (puVar2 != (undefined8 *)0x0) {
          uVar7 = 1;
          if (((param_2[2] != 0x7fffffffffffffff) && (*(int *)(param_2 + 1) == 0)) &&
             (param_2[2] == 1)) {
            uVar5 = param_2[3];
            uVar6 = uVar5;
            uVar8 = uVar5;
            if ((long)uVar5 < 2) {
              uVar6 = uVar7;
            }
            do {
              uVar8 = uVar8 - 1;
              uVar7 = uVar7 - 1;
              if ((long)uVar6 < 1) {
                if (param_1[3] != 0) {
                  uVar1 = (*(code *)((undefined8 *)*param_1)[1])
                                    (*(undefined8 *)*param_1,param_1[4],0);
                  param_1[3] = 0;
                  param_1[4] = uVar1;
                }
                *(undefined4 *)(param_1 + 1) = 0;
                param_1[2] = 0x8000000000000000;
                if (puVar2 != (undefined8 *)0x0) {
                  (*(code *)puVar10[1])(*puVar10,puVar2,0);
                }
                return 0;
              }
              if (uVar8 < uVar5) {
                lVar3 = *(long *)(param_2[4] + uVar8 * 8);
                if (uVar7 == 0) goto LAB_001db72c;
LAB_001db770:
                lVar9 = 0;
              }
              else {
                lVar3 = 0;
                if (uVar7 != 0) goto LAB_001db770;
LAB_001db72c:
                lVar9 = puVar2[uVar7];
              }
              uVar6 = uVar6 - 1;
            } while (lVar3 == lVar9);
          }
        }
      }
      if (puVar2 != (undefined8 *)0x0) {
        (*(code *)puVar10[1])(*puVar10,puVar2,0);
      }
      uVar1 = FUN_001daf60(param_1,param_2,param_3,param_4,FUN_001db860,0);
      return uVar1;
    }
    lVar3 = param_1[3];
  }
  if (lVar3 != 0) {
    uVar1 = (*(code *)puVar10[1])(*puVar10,param_1[4],0);
    param_1[3] = 0;
    param_1[4] = uVar1;
  }
  uVar4 = 0x7fffffffffffffff;
  uVar1 = 1;
LAB_001db7dc:
  param_1[2] = uVar4;
  *(undefined4 *)(param_1 + 1) = 0;
  return uVar1;
}

/* ===== FUN_001db860 @ 001db860 [libNexusScriptRuntime69252.so] ===== */

/* WARNING: Removing unreachable block (ram,0x001db990) */

undefined8 FUN_001db860(undefined8 **param_1,undefined8 **param_2,ulong param_3)

{
  bool bVar1;
  long lVar2;
  long lVar3;
  int iVar4;
  void *pvVar5;
  long lVar6;
  long *plVar7;
  ulong uVar8;
  undefined8 *puVar9;
  ulong uVar10;
  undefined8 *puVar11;
  undefined8 *puVar12;
  undefined8 *puVar13;
  undefined8 *puVar14;
  undefined8 *puVar15;
  undefined8 *puVar16;
  long lVar17;
  long lVar18;
  undefined8 *local_138;
  undefined4 local_130;
  undefined8 local_128;
  undefined8 uStack_120;
  long local_118;
  undefined8 *local_110;
  undefined4 local_108;
  undefined8 local_100;
  undefined8 local_f8;
  long local_f0;
  undefined8 *local_e8;
  undefined4 local_e0;
  long local_d8;
  long local_d0;
  long *local_c8;
  undefined8 *local_c0;
  undefined4 local_b8;
  undefined8 local_b0;
  long local_a8;
  undefined8 *local_a0;
  undefined8 *local_98;
  int local_90;
  undefined8 *local_88;
  undefined8 *local_80;
  void *local_78;
  long local_70;
  
  lVar3 = tpidr_el0;
  local_70 = *(long *)(lVar3 + 0x28);
  if (param_1 == param_2) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
              ,0x116d,"int bf_log_internal(bf_t *, const bf_t *, limb_t, void *)","r != a");
  }
  puVar16 = *param_1;
  local_90 = 0;
  local_80 = (undefined8 *)0x0;
  local_78 = (void *)0x0;
  local_88 = (undefined8 *)0x8000000000000000;
  local_98 = puVar16;
  if (&local_98 != param_2) {
    puVar15 = param_2[3];
    puVar14 = local_80;
    pvVar5 = local_78;
    if (((puVar15 == (undefined8 *)0x0) ||
        (pvVar5 = (void *)(*(code *)puVar16[1])(*puVar16,0,(long)puVar15 << 3), puVar14 = puVar15,
        puVar15 == (undefined8 *)0x0)) || (pvVar5 != (void *)0x0)) {
      local_78 = pvVar5;
      local_80 = puVar14;
      local_88 = param_2[2];
      local_90 = *(int *)(param_2 + 1);
      memcpy(local_78,param_2[4],(long)param_2[3] << 3);
    }
    else {
      if (local_80 != (undefined8 *)0x0) {
        local_78 = (void *)(*(code *)local_98[1])(*local_98,local_78,0);
        local_80 = (undefined8 *)0x0;
      }
      local_90 = 0;
      local_88 = (undefined8 *)0x7fffffffffffffff;
    }
  }
  puVar14 = local_88;
  local_88 = (undefined8 *)0x0;
  puVar15 = (undefined8 *)(*(code *)puVar16[1])(*puVar16,0,8);
  if (puVar15 != (undefined8 *)0x0) {
    *puVar15 = 0xaaaaaaaa00000000;
  }
  puVar9 = (undefined8 *)(ulong)(puVar15 != (undefined8 *)0x0);
  if (local_88 != (undefined8 *)0x7fffffffffffffff) {
    if (local_90 == 0) {
      if (local_88 == (undefined8 *)0x0) {
        puVar11 = local_80;
        puVar12 = puVar9;
        puVar13 = local_80;
        if ((long)local_80 <= (long)puVar9) {
          puVar11 = puVar9;
        }
        do {
          puVar13 = (undefined8 *)((long)puVar13 + -1);
          puVar12 = (undefined8 *)((long)puVar12 + -1);
          if ((long)puVar11 < 1) goto LAB_001dba38;
          if (puVar13 < local_80) {
            uVar8 = *(ulong *)((long)local_78 + (long)puVar13 * 8);
            if (puVar12 < puVar9) goto LAB_001db9dc;
LAB_001dba1c:
            uVar10 = 0;
          }
          else {
            uVar8 = 0;
            if (puVar9 <= puVar12) goto LAB_001dba1c;
LAB_001db9dc:
            uVar10 = puVar15[(long)puVar12];
          }
          puVar11 = (undefined8 *)((long)puVar11 + -1);
        } while (uVar8 == uVar10);
        iVar4 = 1;
        if (uVar8 < uVar10) {
          iVar4 = -1;
        }
      }
      else {
        iVar4 = 1;
        if ((long)local_88 < 0) {
          iVar4 = -1;
        }
      }
    }
    else {
      iVar4 = local_90 * -2 + 1;
    }
    if (iVar4 < 0) {
      local_88 = (undefined8 *)((long)local_88 + 1);
      puVar14 = (undefined8 *)((long)puVar14 + -1);
    }
  }
LAB_001dba38:
  if (puVar15 != (undefined8 *)0x0) {
    (*(code *)puVar16[1])(*puVar16,puVar15,0);
  }
  lVar6 = FUN_001d6604(param_3 + 1 >> 1);
  uVar8 = 0;
  if (lVar6 << 1 != 0) {
    uVar8 = param_3 / (ulong)(lVar6 << 1);
  }
  local_b8 = 0;
  local_a8 = 0;
  local_a0 = (undefined8 *)0x0;
  lVar17 = uVar8 + 1;
  local_b0 = 0x8000000000000000;
  local_e0 = 0;
  lVar2 = param_3 + lVar6 + lVar17 * 2 + 0x20;
  local_d0 = 0;
  local_c8 = (long *)0x0;
  local_d8 = -0x8000000000000000;
  local_e8 = puVar16;
  local_c0 = puVar16;
  FUN_001d6220(&local_98,&local_98,0xffffffffffffffff,0x3fffffffffffffff,0);
  lVar18 = lVar6;
  if (0 < lVar6) {
    do {
      FUN_001d6220(&local_c0,&local_98,1,lVar2,0);
      FUN_001d6dec(&local_e8,&local_c0,lVar2,6);
      FUN_001d6220(&local_c0,&local_e8,1,lVar2,0);
      FUN_001d7210(&local_98,&local_98,&local_c0,lVar2,0,FUN_001d72fc);
      lVar18 = lVar18 + -1;
    } while (lVar18 != 0);
  }
  local_118 = 0;
  local_108 = 0;
  local_f8 = 0;
  local_f0 = 0;
  local_100 = 0x8000000000000000;
  local_130 = 0;
  local_128 = 0x8000000000000000;
  uStack_120 = 0;
  local_138 = puVar16;
  local_110 = puVar16;
  FUN_001d6220(&local_110,&local_98,2,lVar2,0);
  FUN_001d7210(&local_110,&local_98,&local_110,lVar2,0,FUN_001d72fc);
  FUN_001d56bc(&local_138,&local_110,&local_110,lVar2,0);
  *(undefined4 *)(param_1 + 1) = 0;
  param_1[2] = (undefined8 *)0x8000000000000000;
  if (param_1[3] != (undefined8 *)0x0) {
    puVar16 = (undefined8 *)(*(code *)(*param_1)[1])(**param_1,param_1[4],0);
    param_1[3] = (undefined8 *)0x0;
    param_1[4] = puVar16;
  }
  if (uVar8 < 0x7fffffffffffffff) {
    do {
      local_b8 = 0;
      if (local_a8 == 1) {
LAB_001dbc8c:
        *local_a0 = 0x8000000000000000;
        local_b0 = 1;
      }
      else {
        puVar16 = (undefined8 *)(*(code *)local_c0[1])(*local_c0,local_a0,8);
        if (puVar16 != (undefined8 *)0x0) {
          local_a8 = 1;
          local_a0 = puVar16;
          goto LAB_001dbc8c;
        }
        if (local_a8 != 0) {
          local_a0 = (undefined8 *)(*(code *)local_c0[1])(*local_c0,local_a0,0);
          local_a8 = 0;
        }
        local_b0 = 0x7fffffffffffffff;
        local_b8 = 0;
      }
      local_e0 = 0;
      if (local_d0 == 1) {
LAB_001dbbf0:
        uVar8 = lVar17 << 1 | 1;
        uVar10 = LZCOUNT(uVar8);
        local_d8 = 0x40 - uVar10;
        *local_c8 = uVar8 << (uVar10 & 0x3f);
      }
      else {
        plVar7 = (long *)(*(code *)local_e8[1])(*local_e8,local_c8,8);
        if (plVar7 != (long *)0x0) {
          local_d0 = 1;
          local_c8 = plVar7;
          goto LAB_001dbbf0;
        }
        if (local_d0 != 0) {
          local_c8 = (long *)(*(code *)local_e8[1])(*local_e8,local_c8,0);
          local_d0 = 0;
        }
        local_d8 = 0x7fffffffffffffff;
        local_e0 = 0;
      }
      FUN_001d7210(&local_c0,&local_c0,&local_e8,lVar2,0,FUN_001d72fc);
      FUN_001e710c(param_1,param_1,&local_c0,lVar2,0);
      FUN_001d56bc(param_1,param_1,&local_138,lVar2,0);
      lVar18 = lVar17 + -1;
      bVar1 = 0 < lVar17;
      lVar17 = lVar18;
    } while (lVar18 != 0 && bVar1);
  }
  FUN_001d6220(param_1,param_1,1,lVar2,0);
  FUN_001d56bc(param_1,param_1,&local_110,lVar2,0);
  if ((local_110 != (undefined8 *)0x0) && (local_f0 != 0)) {
    (*(code *)local_110[1])(*local_110,local_f0,0);
  }
  if ((local_138 != (undefined8 *)0x0) && (local_118 != 0)) {
    (*(code *)local_138[1])(*local_138,local_118,0);
  }
  if ((local_e8 != (undefined8 *)0x0) && (local_c8 != (long *)0x0)) {
    (*(code *)local_e8[1])(*local_e8,local_c8,0);
  }
  if ((local_c0 != (undefined8 *)0x0) && (local_a0 != (undefined8 *)0x0)) {
    (*(code *)local_c0[1])(*local_c0,local_a0,0);
  }
  if (param_1[3] != (undefined8 *)0x0) {
    if (lVar6 < -0x3fffffffffffffff) {
      lVar6 = -0x4000000000000000;
    }
    if (0x3ffffffffffffffd < lVar6) {
      lVar6 = 0x3ffffffffffffffe;
    }
    param_1[2] = (undefined8 *)((long)param_1[2] + lVar6 + 1);
    FUN_001d3a6c(param_1,0x3fffffffffffffff,1,param_1[3],0);
  }
  FUN_001da384(&local_98,lVar2,6,local_98 + 2,FUN_001da608,0);
  FUN_001d7734(&local_98,&local_98,puVar14,lVar2,0);
  FUN_001e710c(param_1,param_1,&local_98,lVar2,0);
  if ((local_98 != (undefined8 *)0x0) && (local_78 != (void *)0x0)) {
    (*(code *)local_98[1])(*local_98,local_78,0);
  }
  if (*(long *)(lVar3 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return 0x10;
}

/* ===== FUN_001dbed4 @ 001dbed4 [libNexusScriptRuntime69252.so] ===== */

void FUN_001dbed4(undefined8 *param_1,undefined8 **param_2,undefined8 **param_3,ulong param_4,
                 uint param_5)

{
  undefined8 **ppuVar1;
  undefined8 **ppuVar2;
  undefined8 **ppuVar3;
  int iVar4;
  undefined8 **ppuVar5;
  long lVar6;
  undefined8 *puVar7;
  uint uVar8;
  int iVar9;
  ulong *puVar10;
  long lVar11;
  undefined8 uVar12;
  undefined8 uVar13;
  code *pcVar14;
  long lVar15;
  undefined8 *puVar16;
  undefined8 *puVar17;
  ulong uVar18;
  undefined8 *puVar19;
  undefined8 *puVar20;
  ulong uVar21;
  uint uVar22;
  uint uVar23;
  undefined8 *puVar24;
  ulong local_e8;
  undefined8 *local_e0;
  undefined4 local_d8;
  undefined8 local_d0;
  undefined8 uStack_c8;
  long local_c0;
  undefined8 *local_b8;
  undefined4 local_b0;
  long local_a8;
  undefined8 *local_a0;
  undefined8 *local_98;
  undefined8 *local_90;
  uint local_88;
  undefined8 *local_80;
  undefined8 *local_78;
  ulong *local_70;
  long local_68;
  
  lVar6 = tpidr_el0;
  local_68 = *(long *)(lVar6 + 0x28);
  puVar24 = param_2[3];
  puVar20 = (undefined8 *)*param_1;
  if ((puVar24 == (undefined8 *)0x0) || (param_3[3] == (undefined8 *)0x0)) {
    if (param_3[2] == (undefined8 *)0x8000000000000000) {
      *(undefined4 *)(param_1 + 1) = 0;
      if (param_1[3] != 1) {
        lVar15 = (*(code *)puVar20[1])(*puVar20,param_1[4],8);
        if (lVar15 == 0) {
          if (param_1[3] != 0) {
            uVar12 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
            goto LAB_001dc3c4;
          }
          goto LAB_001dc3c8;
        }
        param_1[3] = 1;
        param_1[4] = lVar15;
      }
      uVar18 = 0;
      *(undefined8 *)param_1[4] = 0x8000000000000000;
      param_1[2] = 1;
      goto LAB_001dc770;
    }
    if (param_2[2] == (undefined8 *)0x7fffffffffffffff) {
      if (param_1[3] != 0) {
        uVar12 = (*(code *)puVar20[1])(*puVar20,param_1[4],0);
LAB_001dc3c4:
        param_1[3] = 0;
        param_1[4] = uVar12;
      }
LAB_001dc3c8:
      uVar18 = 0;
      *(undefined4 *)(param_1 + 1) = 0;
      param_1[2] = 0x7fffffffffffffff;
      goto LAB_001dc770;
    }
    *(undefined4 *)(param_1 + 1) = 0;
    if (param_1[3] == 1) {
LAB_001dc09c:
      *(undefined8 *)param_1[4] = 0x8000000000000000;
      param_1[2] = 1;
    }
    else {
      lVar15 = (*(code *)puVar20[1])(*puVar20,param_1[4],8);
      if (lVar15 != 0) {
        param_1[3] = 1;
        param_1[4] = lVar15;
        goto LAB_001dc09c;
      }
      if (param_1[3] != 0) {
        uVar12 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
        param_1[3] = 0;
        param_1[4] = uVar12;
      }
      *(undefined4 *)(param_1 + 1) = 0;
      param_1[2] = 0x7fffffffffffffff;
    }
    puVar20 = param_2[2];
    if (puVar20 == (undefined8 *)param_1[2]) {
      puVar16 = param_2[3];
      puVar17 = (undefined8 *)param_1[3];
      puVar24 = puVar17;
      puVar19 = puVar16;
      puVar7 = puVar16;
      if ((long)puVar16 <= (long)puVar17) {
        puVar7 = puVar17;
      }
      do {
        puVar19 = (undefined8 *)((long)puVar19 + -1);
        puVar24 = (undefined8 *)((long)puVar24 + -1);
        if ((long)puVar7 + -1 < 0) {
          iVar9 = 0;
          goto LAB_001dc45c;
        }
        if (puVar19 < puVar16) {
          uVar18 = param_2[4][(long)puVar19];
          if (puVar24 < puVar17) goto LAB_001dc0e0;
LAB_001dc128:
          uVar21 = 0;
        }
        else {
          uVar18 = 0;
          if (puVar17 <= puVar24) goto LAB_001dc128;
LAB_001dc0e0:
          uVar21 = *(ulong *)(param_1[4] + (long)puVar24 * 8);
        }
        puVar7 = (undefined8 *)((long)puVar7 + -1);
      } while (uVar18 == uVar21);
      iVar9 = 1;
      if (uVar18 < uVar21) {
        iVar9 = -1;
      }
    }
    else {
      iVar9 = 1;
      if ((long)puVar20 < (long)param_1[2]) {
        iVar9 = -1;
      }
    }
LAB_001dc45c:
    if ((((param_5 >> 0x10 & 1) == 0) || (iVar9 != 0)) || ((long)param_3[2] < 0x7ffffffffffffffe)) {
      if ((iVar9 != 0) ||
         ((*(int *)(param_2 + 1) != 0 && (param_3[2] == (undefined8 *)0x7fffffffffffffff)))) {
        puVar24 = param_3[2];
        if (puVar24 == (undefined8 *)0x7ffffffffffffffe) {
          lVar15 = param_1[3];
          uVar12 = 0x8000000000000000;
          if (*(uint *)(param_3 + 1) != (uint)(0 < iVar9)) {
            uVar12 = 0x7ffffffffffffffe;
          }
          goto joined_r0x001dc5a4;
        }
        if (puVar24 == (undefined8 *)0x7fffffffffffffff) goto LAB_001dc494;
        puVar19 = param_3[3];
        if (puVar19 != (undefined8 *)0x0) {
          puVar10 = param_3[4];
          lVar15 = (long)puVar19 * 0x40 - (long)puVar24;
          do {
            uVar18 = *puVar10;
            if (uVar18 != 0) {
              uVar18 = (uVar18 & 0xaaaaaaaaaaaaaaaa) >> 1 | (uVar18 & 0x5555555555555555) << 1;
              uVar18 = (uVar18 & 0xcccccccccccccccc) >> 2 | (uVar18 & 0x3333333333333333) << 2;
              uVar18 = (uVar18 & 0xf0f0f0f0f0f0f0f0) >> 4 | (uVar18 & 0xf0f0f0f0f0f0f0f) << 4;
              uVar18 = (uVar18 & 0xff00ff00ff00ff00) >> 8 | (uVar18 & 0xff00ff00ff00ff) << 8;
              uVar18 = (uVar18 & 0xffff0000ffff0000) >> 0x10 | (uVar18 & 0xffff0000ffff) << 0x10;
              uVar23 = (uint)(LZCOUNT(uVar18 >> 0x20 | uVar18 << 0x20) == lVar15);
              goto LAB_001dc7f0;
            }
            lVar15 = lVar15 + -0x40;
            puVar19 = (undefined8 *)((long)puVar19 + -1);
            puVar10 = puVar10 + 1;
          } while (puVar19 != (undefined8 *)0x0);
        }
        uVar23 = 1;
LAB_001dc7f0:
        lVar15 = param_1[3];
        uVar23 = *(uint *)(param_2 + 1) & uVar23;
        if (*(uint *)(param_3 + 1) == (uint)(puVar20 == (undefined8 *)0x8000000000000000)) {
          if (lVar15 != 0) {
            uVar12 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
            param_1[3] = 0;
            param_1[4] = uVar12;
          }
          *(uint *)(param_1 + 1) = uVar23;
          param_1[2] = 0x7ffffffffffffffe;
          if (*(int *)(param_3 + 1) != 0) {
            uVar18 = 2;
            goto LAB_001dc770;
          }
          goto LAB_001dc4c4;
        }
        uVar12 = 0x8000000000000000;
        goto joined_r0x001dc850;
      }
    }
    else {
LAB_001dc494:
      lVar15 = param_1[3];
      uVar12 = 0x7fffffffffffffff;
joined_r0x001dc5a4:
      uVar23 = 0;
joined_r0x001dc850:
      if (lVar15 != 0) {
        uVar13 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
        param_1[3] = 0;
        param_1[4] = uVar13;
      }
      param_1[2] = uVar12;
      *(uint *)(param_1 + 1) = uVar23;
    }
LAB_001dc4c4:
    uVar18 = 0;
    goto LAB_001dc770;
  }
  local_88 = 0;
  local_78 = (undefined8 *)0x0;
  local_70 = (ulong *)0x0;
  local_80 = (undefined8 *)0x8000000000000000;
  local_90 = puVar20;
  if (&local_90 == param_2) {
LAB_001dbf8c:
    puVar24 = param_3[3];
  }
  else {
    puVar19 = local_78;
    puVar10 = local_70;
    if ((puVar24 == (undefined8 *)0x0) ||
       (puVar10 = (ulong *)(*(code *)puVar20[1])(*puVar20,0,(long)puVar24 << 3), puVar19 = puVar24,
       puVar10 != (ulong *)0x0)) {
      local_70 = puVar10;
      local_78 = puVar19;
      local_80 = param_2[2];
      local_88 = *(uint *)(param_2 + 1);
      memcpy(local_70,param_2[4],(long)param_2[3] << 3);
      goto LAB_001dbf8c;
    }
    if (local_78 != (undefined8 *)0x0) {
      local_70 = (ulong *)(*(code *)local_90[1])(*local_90,local_70,0);
      local_78 = (undefined8 *)0x0;
    }
    local_88 = 0;
    local_80 = (undefined8 *)0x7fffffffffffffff;
    puVar24 = param_3[3];
  }
  if (puVar24 != (undefined8 *)0x0) {
    puVar10 = param_3[4];
    lVar15 = (long)puVar24 << 6;
    do {
      uVar18 = *puVar10;
      if (uVar18 != 0) {
        uVar18 = (uVar18 & 0xaaaaaaaaaaaaaaaa) >> 1 | (uVar18 & 0x5555555555555555) << 1;
        uVar18 = (uVar18 & 0xcccccccccccccccc) >> 2 | (uVar18 & 0x3333333333333333) << 2;
        uVar18 = (uVar18 & 0xf0f0f0f0f0f0f0f0) >> 4 | (uVar18 & 0xf0f0f0f0f0f0f0f) << 4;
        uVar18 = (uVar18 & 0xff00ff00ff00ff00) >> 8 | (uVar18 & 0xff00ff00ff00ff) << 8;
        uVar18 = (uVar18 & 0xffff0000ffff0000) >> 0x10 | (uVar18 & 0xffff0000ffff) << 0x10;
        lVar15 = (long)param_3[2] + (LZCOUNT(uVar18 >> 0x20 | uVar18 << 0x20) - lVar15);
        iVar9 = *(int *)(param_2 + 1);
        goto joined_r0x001dc148;
      }
      lVar15 = lVar15 + -0x40;
      puVar24 = (undefined8 *)((long)puVar24 + -1);
      puVar10 = puVar10 + 1;
    } while (puVar24 != (undefined8 *)0x0);
  }
  lVar15 = 0;
  iVar9 = *(int *)(param_2 + 1);
joined_r0x001dc148:
  if (iVar9 == 0) {
    uVar22 = 0;
    uVar23 = param_5;
  }
  else {
    if (lVar15 < 0) {
      if (param_1[3] != 0) {
        uVar12 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
        param_1[3] = 0;
        param_1[4] = uVar12;
      }
      *(undefined4 *)(param_1 + 1) = 0;
      param_1[2] = 0x7fffffffffffffff;
      if ((local_90 == (undefined8 *)0x0) || (local_70 == (ulong *)0x0)) {
        uVar18 = 1;
      }
      else {
        (*(code *)local_90[1])(*local_90,local_70,0);
        uVar18 = 1;
      }
      goto LAB_001dc770;
    }
    uVar22 = (uint)(lVar15 == 0);
    uVar23 = 0;
    if ((param_5 & 6) == 2) {
      uVar23 = (uint)(lVar15 == 0);
    }
    local_88 = local_88 ^ 1;
    uVar23 = uVar23 ^ param_5;
  }
  *(undefined4 *)(param_1 + 1) = 0;
  if (param_1[3] == 1) {
LAB_001dc184:
    *(undefined8 *)param_1[4] = 0x8000000000000000;
    param_1[2] = 1;
  }
  else {
    lVar11 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],8);
    if (lVar11 != 0) {
      param_1[3] = 1;
      param_1[4] = lVar11;
      goto LAB_001dc184;
    }
    if (param_1[3] != 0) {
      uVar12 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
      param_1[3] = 0;
      param_1[4] = uVar12;
    }
    *(undefined4 *)(param_1 + 1) = 0;
    param_1[2] = 0x7fffffffffffffff;
  }
  if ((local_80 == (undefined8 *)0x7fffffffffffffff) ||
     (puVar24 = (undefined8 *)param_1[2], puVar24 == (undefined8 *)0x7fffffffffffffff)) {
LAB_001dc1b4:
    local_c0 = 0;
    local_b0 = 0;
    local_a0 = (undefined8 *)0x0;
    local_98 = (undefined8 *)0x0;
    local_a8 = -0x8000000000000000;
    local_d8 = 0;
    local_d0 = 0x8000000000000000;
    uStack_c8 = 0;
    local_e0 = puVar20;
    local_b8 = puVar20;
    FUN_001db5c0(&local_b8,&local_90,0x40,2);
    FUN_001db5c0(&local_e0,&local_90,0x40,3);
    FUN_001d56bc(&local_b8,&local_b8,param_3,0x40,*(uint *)(param_3 + 1) ^ 2);
    FUN_001d56bc(&local_e0,&local_e0,param_3,0x40,*(uint *)(param_3 + 1) ^ 3);
    uVar8 = FUN_001da980(puVar20,param_1,&local_b8,&local_e0,param_4,uVar23);
    uVar21 = (ulong)uVar8;
    if ((local_b8 != (undefined8 *)0x0) && (local_98 != (undefined8 *)0x0)) {
      (*(code *)local_b8[1])(*local_b8,local_98,0);
    }
    if ((local_e0 != (undefined8 *)0x0) && (local_c0 != 0)) {
      (*(code *)local_e0[1])(*local_e0,local_c0,0);
    }
    if (uVar8 == 0) {
      if (lVar15 < 0) {
        if (((param_5 & 7) != 6) && (iVar9 = FUN_001dcdd0(param_1,&local_90,-lVar15), iVar9 != 0)) {
          FUN_001d3880(&local_90,param_1);
          ppuVar3 = param_3 + 3;
          ppuVar5 = param_3 + 4;
          ppuVar2 = param_3 + 2;
          ppuVar1 = param_3 + 1;
          param_3 = &local_b8;
          local_a0 = *ppuVar3;
          local_98 = *ppuVar5;
          local_b0 = *(undefined4 *)ppuVar1;
          local_a8 = (long)*ppuVar2 - lVar15;
          goto LAB_001dc298;
        }
        goto LAB_001dc728;
      }
LAB_001dc298:
      puVar20 = local_78;
      if (local_78 != (undefined8 *)0x0) {
        lVar15 = (long)local_78 * 0x40 - (long)local_80;
        puVar10 = local_70;
        do {
          uVar18 = *puVar10;
          if (uVar18 != 0) {
            uVar18 = (uVar18 & 0xaaaaaaaaaaaaaaaa) >> 1 | (uVar18 & 0x5555555555555555) << 1;
            uVar18 = (uVar18 & 0xcccccccccccccccc) >> 2 | (uVar18 & 0x3333333333333333) << 2;
            uVar18 = (uVar18 & 0xf0f0f0f0f0f0f0f0) >> 4 | (uVar18 & 0xf0f0f0f0f0f0f0f) << 4;
            uVar18 = (uVar18 & 0xff00ff00ff00ff00) >> 8 | (uVar18 & 0xff00ff00ff00ff) << 8;
            uVar18 = (uVar18 & 0xffff0000ffff0000) >> 0x10 | (uVar18 & 0xffff0000ffff) << 0x10;
            puVar20 = (undefined8 *)(LZCOUNT(uVar18 >> 0x20 | uVar18 << 0x20) - lVar15);
            goto LAB_001dc5ec;
          }
          lVar15 = lVar15 + -0x40;
          puVar20 = (undefined8 *)((long)puVar20 + -1);
          puVar10 = puVar10 + 1;
        } while (puVar20 != (undefined8 *)0x0);
        puVar20 = (undefined8 *)0x0;
      }
LAB_001dc5ec:
      lVar15 = (long)local_80 + (-1 - (long)puVar20);
      if (lVar15 == 0) {
        FUN_001d7734(&local_90,param_3,(long)local_80 + -1,0x40,1);
        FUN_001d81f4(&local_e8,&local_90,0);
        FUN_001d367c(param_1,1);
        uVar18 = FUN_001d5ae8(param_1,local_e8,param_4,uVar23);
      }
      else {
        if (param_4 == 0x3fffffffffffffff) {
          FUN_001d81f4(&local_e0,param_3,0);
          puVar20 = local_e0;
          if (*(int *)(param_3 + 1) != 0) {
                    /* WARNING: Subroutine does not return */
            __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
                      ,0x12b7,"int bf_pow(bf_t *, const bf_t *, const bf_t *, limb_t, bf_flags_t)",
                      "!y->sign");
          }
          if ((long)local_e0 < 0x2000000000000000) {
            uVar18 = FUN_001dca20(param_1,&local_90,local_e0,0x3fffffffffffffff,1);
            uVar21 = uVar18 & 0xffffffff;
          }
          else {
            if ((local_90 != (undefined8 *)0x0) && (local_70 != (ulong *)0x0)) {
              (*(code *)local_90[1])(*local_90,local_70,0);
            }
            uVar18 = FUN_001dc87c(param_1,0,0x3fffffffffffffff,uVar23);
          }
          if (0x1fffffffffffffff < (long)puVar20) goto LAB_001dc770;
          goto LAB_001dc74c;
        }
        if ((long)param_3[2] < 0x20) {
LAB_001dc6b0:
          pcVar14 = FUN_001dcc10;
        }
        else {
          if (((param_5 & 7) != 6) && (*(int *)(param_3 + 1) == 0)) {
            FUN_001d7734(param_1,param_3,lVar15,0x40,1);
            FUN_001d81f4(&local_e8,param_1,0);
            if (local_e8 <= param_4) goto LAB_001dc6b0;
          }
LAB_001dc728:
          pcVar14 = FUN_001dd088;
        }
        uVar18 = FUN_001daf60(param_1,&local_90,param_4,uVar23,pcVar14,param_3);
      }
      uVar21 = uVar18 & 0xffffffff;
    }
  }
  else {
    if (local_88 == *(uint *)(param_1 + 1)) {
      if (local_80 == puVar24) {
        puVar16 = (undefined8 *)param_1[3];
        puVar24 = puVar16;
        puVar19 = local_78;
        puVar7 = local_78;
        if ((long)local_78 <= (long)puVar16) {
          puVar7 = puVar16;
        }
        do {
          puVar19 = (undefined8 *)((long)puVar19 + -1);
          puVar24 = (undefined8 *)((long)puVar24 + -1);
          if ((long)puVar7 + -1 < 0) {
            iVar9 = 0;
            goto LAB_001dc6cc;
          }
          if (puVar19 < local_78) {
            uVar18 = local_70[(long)puVar19];
            if (puVar24 < puVar16) goto LAB_001dc35c;
LAB_001dc3a0:
            uVar21 = 0;
          }
          else {
            uVar18 = 0;
            if (puVar16 <= puVar24) goto LAB_001dc3a0;
LAB_001dc35c:
            uVar21 = *(ulong *)(param_1[4] + (long)puVar24 * 8);
          }
          puVar7 = (undefined8 *)((long)puVar7 + -1);
        } while (uVar18 == uVar21);
        iVar9 = 1;
        if (uVar18 < uVar21) {
          iVar9 = -1;
        }
      }
      else {
        iVar9 = 1;
        if ((long)local_80 < (long)puVar24) {
          iVar9 = -1;
        }
      }
LAB_001dc6cc:
      iVar4 = -iVar9;
      if (local_88 == 0) {
        iVar4 = iVar9;
      }
      if (iVar4 != 0) goto LAB_001dc1b4;
    }
    else if (local_80 != (undefined8 *)0x8000000000000000 ||
             puVar24 != (undefined8 *)0x8000000000000000) goto LAB_001dc1b4;
    uVar21 = 0;
  }
LAB_001dc74c:
  uVar18 = uVar21;
  if ((local_90 != (undefined8 *)0x0) && (local_70 != (ulong *)0x0)) {
    (*(code *)local_90[1])(*local_90,local_70,0);
  }
  *(uint *)(param_1 + 1) = uVar22;
LAB_001dc770:
  if (*(long *)(lVar6 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar18);
  }
  return;
}

/* ===== FUN_001dca20 @ 001dca20 [libNexusScriptRuntime69252.so] ===== */

uint FUN_001dca20(undefined8 *param_1,undefined8 *param_2,ulong param_3,undefined8 param_4,
                 undefined4 param_5)

{
  undefined4 uVar1;
  uint uVar2;
  long lVar3;
  undefined8 uVar4;
  void *__src;
  int iVar5;
  uint uVar6;
  long lVar7;
  ulong uVar8;
  
  if (param_1 == param_2) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
              ,0x8ed,"int bf_pow_ui(bf_t *, const bf_t *, limb_t, limb_t, bf_flags_t)","r != a");
  }
  if (param_3 == 0) {
    *(undefined4 *)(param_1 + 1) = 0;
    if (param_1[3] != 1) {
      lVar7 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],8);
      if (lVar7 == 0) {
        if (param_1[3] != 0) {
          uVar4 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
          param_1[3] = 0;
          param_1[4] = uVar4;
        }
        *(undefined4 *)(param_1 + 1) = 0;
        param_1[2] = 0x7fffffffffffffff;
        return 0x20;
      }
      param_1[3] = 1;
      param_1[4] = lVar7;
    }
    *(undefined8 *)param_1[4] = 0x8000000000000000;
    param_1[2] = 1;
    return 0;
  }
  lVar7 = param_2[3];
  if (param_1[3] != lVar7) {
    lVar3 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],lVar7 << 3);
    if ((lVar7 != 0) && (lVar3 == 0)) {
      if (param_1[3] != 0) {
        uVar4 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
        param_1[3] = 0;
        param_1[4] = uVar4;
      }
      uVar6 = 0x20;
      *(undefined4 *)(param_1 + 1) = 0;
      param_1[2] = 0x7fffffffffffffff;
      goto LAB_001dcb2c;
    }
    param_1[3] = lVar7;
    param_1[4] = lVar3;
  }
  lVar7 = param_2[3];
  uVar1 = *(undefined4 *)(param_2 + 1);
  __src = (void *)param_2[4];
  param_1[2] = param_2[2];
  *(undefined4 *)(param_1 + 1) = uVar1;
  memcpy((void *)param_1[4],__src,lVar7 << 3);
  uVar6 = 0;
LAB_001dcb2c:
  if (LZCOUNT(param_3) != 0x3f) {
    iVar5 = (int)LZCOUNT(param_3);
    uVar8 = (ulong)(0x3e - iVar5);
    iVar5 = 0x3f - iVar5;
    do {
      uVar2 = FUN_001d56bc(param_1,param_1,param_1,param_4,param_5);
      uVar6 = uVar2 | uVar6;
      if ((param_3 >> (uVar8 & 0x3f) & 1) != 0) {
        uVar2 = FUN_001d56bc(param_1,param_1,param_2,param_4,param_5);
        uVar6 = uVar2 | uVar6;
      }
      uVar8 = uVar8 - 1;
      iVar5 = iVar5 + -1;
    } while (0 < iVar5);
  }
  return uVar6;
}

/* ===== FUN_001dd80c @ 001dd80c [libNexusScriptRuntime69252.so] ===== */

undefined8 * FUN_001dd80c(undefined8 *param_1,undefined8 *param_2,long param_3,undefined4 param_4)

{
  ulong uVar1;
  ulong uVar2;
  long lVar3;
  long lVar4;
  undefined4 uVar5;
  long lVar6;
  undefined8 *puVar7;
  undefined8 uVar8;
  void *__src;
  ulong uVar9;
  
  if (param_1 == param_2) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
              ,0x13b5,"int bf_tan(bf_t *, const bf_t *, limb_t, bf_flags_t)","r != a");
  }
  lVar3 = param_2[2];
  lVar4 = param_2[3];
  if (lVar4 == 0) {
    if (lVar3 == 0x7ffffffffffffffe) {
      if (param_1[3] != 0) {
        uVar8 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
        param_1[3] = 0;
        param_1[4] = uVar8;
      }
      puVar7 = (undefined8 *)0x1;
    }
    else {
      if (lVar3 != 0x7fffffffffffffff) {
        uVar5 = *(undefined4 *)(param_2 + 1);
        if (param_1[3] != 0) {
          uVar8 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
          param_1[3] = 0;
          param_1[4] = uVar8;
        }
        *(undefined4 *)(param_1 + 1) = uVar5;
        param_1[2] = 0x8000000000000000;
        return (undefined8 *)0x0;
      }
      if (param_1[3] != 0) {
        uVar8 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
        param_1[3] = 0;
        param_1[4] = uVar8;
      }
      puVar7 = (undefined8 *)0x0;
    }
    param_1[2] = 0x7fffffffffffffff;
    *(undefined4 *)(param_1 + 1) = 0;
    return puVar7;
  }
  if (-1 < lVar3) goto LAB_001dd8f4;
  uVar1 = (lVar3 - 1U) + lVar3 * 2;
  uVar9 = lVar4 << 6 | 2;
  uVar2 = 0x8000000000000000;
  if (-1 < (long)((uVar1 ^ lVar3 * 2) & (uVar1 ^ lVar3 - 1U))) {
    uVar2 = uVar1;
  }
  uVar1 = param_3 + 2U;
  if ((long)(param_3 + 2U) <= (long)uVar9) {
    uVar1 = uVar9;
  }
  puVar7 = param_1;
  if ((long)uVar2 < (long)(lVar3 - uVar1)) {
    if (param_1[3] == lVar4) {
LAB_001dd8b0:
      lVar4 = param_2[3];
      uVar5 = *(undefined4 *)(param_2 + 1);
      __src = (void *)param_2[4];
      param_1[2] = param_2[2];
      *(undefined4 *)(param_1 + 1) = uVar5;
      memcpy((void *)param_1[4],__src,lVar4 << 3);
    }
    else {
      lVar6 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],lVar4 << 3);
      if (lVar6 != 0) {
        param_1[3] = lVar4;
        param_1[4] = lVar6;
        goto LAB_001dd8b0;
      }
      if (param_1[3] != 0) {
        uVar8 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
        param_1[3] = 0;
        param_1[4] = uVar8;
      }
      *(undefined4 *)(param_1 + 1) = 0;
      param_1[2] = 0x7fffffffffffffff;
    }
    puVar7 = (undefined8 *)
             FUN_001dae40(param_1,param_1,uVar2,*(undefined4 *)(param_2 + 1),param_3,param_4);
  }
  if ((long)uVar2 < (long)(lVar3 - uVar1)) {
    return puVar7;
  }
LAB_001dd8f4:
  puVar7 = (undefined8 *)FUN_001daf60(param_1,param_2,param_3,param_4,FUN_001dda3c,0);
  return puVar7;
}

/* ===== FUN_001dfc74 @ 001dfc74 [libNexusScriptRuntime69252.so] ===== */

/* WARNING: Removing unreachable block (ram,0x001dfdf4) */
/* WARNING: Removing unreachable block (ram,0x001dff6c) */

uint FUN_001dfc74(undefined8 *param_1,ulong param_2,uint param_3,ulong param_4)

{
  bool bVar1;
  undefined1 auVar2 [16];
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  undefined1 auVar6 [16];
  undefined1 auVar7 [16];
  undefined1 auVar8 [16];
  undefined1 auVar9 [16];
  bool bVar10;
  bool bVar11;
  bool bVar12;
  void *__dest;
  undefined8 uVar13;
  ulong uVar14;
  size_t __n;
  long lVar15;
  ulong uVar16;
  uint uVar17;
  long lVar18;
  ulong uVar19;
  uint uVar20;
  ulong *puVar21;
  uint uVar22;
  ulong uVar23;
  ulong uVar24;
  long lVar25;
  long lVar26;
  long lVar27;
  undefined4 uVar28;
  
  uVar20 = param_3 >> 5 & 0x3f;
  uVar17 = 0x3d;
  if (uVar20 != 0x3f) {
    uVar17 = 0x3c - uVar20;
  }
  lVar15 = 1L << ((ulong)uVar17 & 0x3f);
  if ((param_3 >> 4 & 1) == 0) {
    uVar19 = param_2;
    if (((long)param_1[2] < 3 - lVar15) && ((param_3 >> 3 & 1) != 0)) {
      if (param_2 == 0x3fffffffffffffff) {
                    /* WARNING: Subroutine does not return */
        __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
                  ,0x192d,"int __bfdec_round(bfdec_t *, limb_t, bf_flags_t, limb_t)",
                  "prec1 != BF_PREC_INF");
      }
      uVar19 = lVar15 + -3 + param_2 + param_1[2];
    }
  }
  else {
    uVar19 = 0x3fffffffffffffff;
    if (param_2 != 0x3fffffffffffffff) {
      uVar19 = param_1[2] + param_2;
    }
  }
  uVar17 = param_3 & 7;
  if (uVar17 == 6) {
LAB_001dfda0:
    bVar1 = false;
    bVar10 = true;
  }
  else {
    uVar16 = uVar19;
    if ((long)uVar19 < 0) {
      uVar16 = 0xffffffffffffffff;
    }
    uVar16 = (param_4 * 0x13 - uVar16) - 2;
    if ((long)uVar16 < 0) {
      bVar10 = false;
      bVar1 = true;
    }
    else {
      lVar18 = uVar16 % 0x13 + 1;
      uVar23 = *(ulong *)(param_1[4] + (uVar16 / 0x13) * 8);
      auVar2._8_8_ = 0;
      auVar2._0_8_ = (&DAT_0011e000)[lVar18 * 2];
      auVar6._8_8_ = 0;
      auVar6._0_8_ = uVar23;
      lVar25 = SUB168(auVar2 * auVar6,8);
      if (uVar23 != ((uVar23 - lVar25 >> ((long)(char)(&DAT_0011e008)[lVar18 * 0x10] & 0x3fU)) +
                     lVar25 >> ((long)(char)(&DAT_0011e009)[lVar18 * 0x10] & 0x3fU)) *
                    (&DAT_0011dda8)[lVar18]) goto LAB_001dfda0;
      bVar10 = true;
      uVar16 = uVar16 / 0x13;
      do {
        bVar1 = (long)uVar16 < 1;
        if ((long)uVar16 < 1) {
          bVar10 = false;
          goto LAB_001dfdbc;
        }
        lVar18 = uVar16 * 8;
        uVar16 = uVar16 - 1;
      } while (*(long *)(param_1[4] + -8 + lVar18) == 0);
      bVar1 = false;
    }
  }
LAB_001dfdbc:
  __dest = (void *)param_1[4];
  lVar18 = param_4 * 0x13;
  uVar16 = lVar18 + ~uVar19;
  if ((long)uVar16 < 0) {
    lVar25 = SUB168(SEXT816((long)(uVar16 - 0x12)) * SEXT816(0xd79435e50d79436),8);
    uVar23 = lVar25 - (lVar25 >> 0x3f);
    uVar24 = 0;
    if (-1 < (long)uVar23) goto LAB_001dfe1c;
  }
  else {
    uVar23 = uVar16 / 0x13;
LAB_001dfe1c:
    uVar24 = 0;
    if (uVar23 < param_4) {
      uVar24 = *(ulong *)((long)__dest + uVar23 * 8);
      lVar25 = (long)((ulong)(uint)((int)uVar16 + (int)uVar23 * -0x13) << 0x20) >> 0x1c;
      auVar3._8_8_ = 0;
      auVar3._0_8_ = *(ulong *)((long)&DAT_0011e000 + lVar25);
      auVar7._8_8_ = 0;
      auVar7._0_8_ = uVar24;
      lVar26 = SUB168(auVar3 * auVar7,8);
      uVar24 = ((uVar24 - lVar26 >> ((long)(char)(&DAT_0011e008)[lVar25] & 0x3fU)) + lVar26 >>
               ((long)(char)(&DAT_0011e009)[lVar25] & 0x3fU)) % 10;
    }
  }
  if (6 < uVar17) {
                    /* WARNING: Subroutine does not return */
    abort();
  }
  uVar22 = (uint)(uVar24 != 0 || bVar10);
  uVar20 = 0;
  switch(uVar17) {
  case 0:
    bVar11 = 4 < uVar24;
    bVar12 = uVar24 == 5;
    if (!bVar12) goto LAB_001dfec4;
    if (bVar1) {
      uVar16 = lVar18 - uVar19;
      if ((long)uVar16 < 0) {
        lVar25 = SUB168(SEXT816((long)(uVar16 - 0x12)) * SEXT816(0xd79435e50d79436),8);
        uVar23 = lVar25 - (lVar25 >> 0x3f);
      }
      else {
        uVar23 = uVar16 / 0x13;
      }
      uVar20 = 0;
      if ((-1 < (long)uVar23) && (uVar23 < param_4)) {
        uVar14 = *(ulong *)((long)__dest + uVar23 * 8);
        lVar25 = (long)((ulong)(uint)((int)uVar16 + (int)uVar23 * -0x13) << 0x20) >> 0x1c;
        auVar5._8_8_ = 0;
        auVar5._0_8_ = *(ulong *)((long)&DAT_0011e000 + lVar25);
        auVar9._8_8_ = 0;
        auVar9._0_8_ = uVar14;
        lVar26 = SUB168(auVar5 * auVar9,8);
        uVar20 = (uint)((uVar14 - lVar26 >> ((long)(char)(&DAT_0011e008)[lVar25] & 0x3fU)) + lVar26
                       >> ((long)(char)(&DAT_0011e009)[lVar25] & 0x3fU)) & 1;
      }
    }
    else {
      uVar20 = 1;
    }
    break;
  case 1:
    break;
  default:
    uVar20 = uVar22;
    if (*(uint *)(param_1 + 1) != (uint)(uVar17 == 2)) {
      uVar20 = 0;
    }
    break;
  case 4:
  case 6:
    bVar11 = 3 < uVar24;
    bVar12 = uVar24 == 4;
LAB_001dfec4:
    uVar20 = (uint)(bVar11 && !bVar12);
    break;
  case 5:
    uVar20 = uVar22;
  }
  if ((long)uVar19 < 1) {
    if (uVar20 != 0) {
      if ((param_1[3] != 1) &&
         (lVar15 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],8),
         lVar15 != 0)) {
        param_1[3] = 1;
        param_1[4] = lVar15;
      }
      *(undefined8 *)param_1[4] = 1000000000000000000;
      param_1[2] = (param_1[2] - uVar19) + 1;
      return 0x18;
    }
LAB_001e0238:
    uVar28 = *(undefined4 *)(param_1 + 1);
    if (param_1[3] != 0) {
      uVar13 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
      param_1[3] = 0;
      param_1[4] = uVar13;
    }
    uVar13 = 0x8000000000000000;
    uVar17 = 0x18;
LAB_001e0264:
    param_1[2] = uVar13;
    *(undefined4 *)(param_1 + 1) = uVar28;
    return uVar17;
  }
  uVar17 = (uint)(uVar24 != 0 || bVar10) << 4;
  if (uVar20 != 0) {
    lVar26 = SUB168(SEXT816((long)(lVar18 - uVar19)) * SEXT816(0xd79435e50d79436),8);
    lVar26 = lVar26 - (lVar26 >> 0x3f);
    lVar27 = (&DAT_0011dda8)[(lVar18 - uVar19) + lVar26 * -0x13];
    lVar25 = param_4 - lVar26;
    if (lVar25 < 1) {
      if (lVar27 == 0) goto LAB_001e007c;
    }
    else {
      puVar21 = (ulong *)((long)__dest + lVar26 * 8);
      lVar26 = lVar25;
      do {
        uVar14 = *puVar21;
        uVar16 = uVar14 + lVar27 + 0x7538dcfb76180000;
        uVar23 = uVar14 + lVar27;
        if (uVar16 <= uVar14) {
          uVar23 = uVar16;
        }
        *puVar21 = uVar23;
        if (uVar14 < uVar16) goto LAB_001e007c;
        lVar26 = lVar26 + -1;
        puVar21 = puVar21 + 1;
        lVar27 = 1;
      } while (lVar26 != 0);
    }
    if (0 < lVar25) {
      puVar21 = (ulong *)((long)__dest + param_4 * 8);
      lVar25 = lVar25 + 1;
      uVar16 = 1;
      do {
        puVar21 = puVar21 + -1;
        uVar23 = *puVar21;
        lVar25 = lVar25 + -1;
        *puVar21 = uVar23 / 10 + uVar16 * 1000000000000000000;
        uVar16 = uVar23 % 10;
      } while (1 < lVar25);
    }
    param_1[2] = param_1[2] + 1;
  }
LAB_001e007c:
  if ((long)param_1[2] < 3 - lVar15) {
    if ((param_3 >> 3 & 1) == 0) goto LAB_001e0238;
    uVar17 = 0x18;
    if (uVar24 == 0 && !bVar10) {
      uVar17 = 0;
    }
  }
  if (lVar15 < (long)param_1[2]) {
    uVar28 = *(undefined4 *)(param_1 + 1);
    if (param_1[3] != 0) {
      uVar13 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
      param_1[3] = 0;
      param_1[4] = uVar13;
    }
    uVar13 = 0x7ffffffffffffffe;
    uVar17 = uVar17 | 0x14;
    goto LAB_001e0264;
  }
  uVar19 = lVar18 - uVar19;
  if ((long)uVar19 < 0) {
    lVar15 = SUB168(SEXT816((long)(uVar19 - 0x12)) * SEXT816(0xd79435e50d79436),8);
    uVar16 = lVar15 - (lVar15 >> 0x3f);
    if ((long)uVar16 < 0) {
      uVar16 = 0;
      goto LAB_001e0150;
    }
  }
  else {
    uVar16 = uVar19 / 0x13;
  }
  lVar15 = SUB168(SEXT816((long)uVar19) * SEXT816(0xd79435e50d79436),8);
  lVar15 = uVar19 + (lVar15 - (lVar15 >> 0x3f)) * -0x13;
  lVar15 = (lVar15 >> 0x3f & 0x13U) + lVar15;
  if ((int)lVar15 != 0) {
    uVar19 = *(ulong *)((long)__dest + uVar16 * 8);
    auVar4._8_8_ = 0;
    auVar4._0_8_ = (&DAT_0011e000)[lVar15 * 2];
    auVar8._8_8_ = 0;
    auVar8._0_8_ = uVar19;
    lVar18 = SUB168(auVar4 * auVar8,8);
    *(ulong *)((long)__dest + uVar16 * 8) =
         ((uVar19 - lVar18 >> ((long)(char)(&DAT_0011e008)[lVar15 * 0x10] & 0x3fU)) + lVar18 >>
         ((long)(char)(&DAT_0011e009)[lVar15 * 0x10] & 0x3fU)) * (&DAT_0011dda8)[lVar15];
  }
LAB_001e0150:
  __n = param_4 * 8 + uVar16 * -8 + 8;
  do {
    uVar19 = uVar16;
    uVar16 = uVar19 + 1;
    __n = __n - 8;
  } while (*(long *)((long)__dest + uVar19 * 8) == 0);
  if (0 < (long)uVar19) {
    param_4 = (param_4 - uVar16) + 1;
    memmove(__dest,(void *)((long)__dest + uVar16 * 8 + -8),__n);
  }
  if (param_1[3] == param_4) {
    return uVar17;
  }
  lVar15 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],param_4 << 3);
  if ((param_4 != 0) && (lVar15 == 0)) {
    return uVar17;
  }
  param_1[3] = param_4;
  param_1[4] = lVar15;
  return uVar17;
}

/* ===== FUN_001e030c @ 001e030c [libNexusScriptRuntime69252.so] ===== */

undefined8 FUN_001e030c(undefined8 *param_1)

{
  char cVar1;
  char cVar2;
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  ulong uVar5;
  undefined8 uVar6;
  long lVar7;
  ulong uVar8;
  long lVar9;
  uint uVar10;
  long lVar11;
  long lVar12;
  ulong uVar13;
  ulong uVar14;
  ulong uVar15;
  long lVar16;
  long lVar17;
  long lVar18;
  
  lVar11 = -0x13;
  lVar16 = param_1[3];
  do {
    lVar12 = lVar16;
    if (lVar12 == 0) {
      param_1[2] = 0x8000000000000000;
      if (param_1[3] != 0) {
        uVar6 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
        param_1[3] = 0;
        param_1[4] = uVar6;
      }
      return 0;
    }
    lVar9 = param_1[4];
    lVar11 = lVar11 + 0x13;
    lVar16 = lVar12 + -1;
  } while (*(long *)(lVar9 + lVar12 * 8 + -8) == 0);
  param_1[2] = param_1[2] - lVar11;
  uVar13 = *(ulong *)(lVar9 + (lVar12 + -1) * 8);
  if (uVar13 == 0) {
    uVar10 = 0x13;
  }
  else {
    uVar10 = 0x12;
    switch((uint)LZCOUNT(uVar13) ^ 0x3f) {
    case 0:
    case 1:
    case 2:
      break;
    case 3:
      uVar10 = 0x11;
      if (uVar13 < 10) {
        uVar10 = 0x12;
      }
      break;
    case 4:
    case 5:
      uVar10 = 0x11;
      break;
    case 6:
      uVar10 = 0x10;
      if (uVar13 < 100) {
        uVar10 = 0x11;
      }
      break;
    case 7:
    case 8:
      uVar10 = 0x10;
      break;
    case 9:
      uVar10 = 0xf;
      if (uVar13 < 1000) {
        uVar10 = 0x10;
      }
      break;
    case 10:
    case 0xb:
    case 0xc:
      uVar10 = 0xf;
      break;
    case 0xd:
      uVar10 = 0xe;
      if (uVar13 >> 4 < 0x271) {
        uVar10 = 0xf;
      }
      break;
    case 0xe:
    case 0xf:
      uVar10 = 0xe;
      break;
    case 0x10:
      uVar10 = 0xd;
      if (uVar13 >> 5 < 0xc35) {
        uVar10 = 0xe;
      }
      break;
    case 0x11:
    case 0x12:
      uVar10 = 0xd;
      break;
    case 0x13:
      uVar10 = 0xc;
      if (uVar13 < 1000000) {
        uVar10 = 0xd;
      }
      break;
    case 0x14:
    case 0x15:
    case 0x16:
      uVar10 = 0xc;
      break;
    case 0x17:
      uVar10 = 0xb;
      if (uVar13 < 10000000) {
        uVar10 = 0xc;
      }
      break;
    case 0x18:
    case 0x19:
      uVar10 = 0xb;
      break;
    case 0x1a:
      uVar10 = 10;
      if (uVar13 < 100000000) {
        uVar10 = 0xb;
      }
      break;
    case 0x1b:
    case 0x1c:
      uVar10 = 10;
      break;
    case 0x1d:
      uVar10 = 9;
      if (uVar13 < 1000000000) {
        uVar10 = 10;
      }
      break;
    case 0x1e:
    case 0x1f:
    case 0x20:
      uVar10 = 9;
      break;
    case 0x21:
      uVar10 = 8;
      if (uVar13 < 10000000000) {
        uVar10 = 9;
      }
      break;
    case 0x22:
    case 0x23:
      uVar10 = 8;
      break;
    case 0x24:
      uVar10 = 7;
      if (uVar13 < 100000000000) {
        uVar10 = 8;
      }
      break;
    case 0x25:
    case 0x26:
      uVar10 = 7;
      break;
    case 0x27:
      uVar10 = 6;
      if (uVar13 < 1000000000000) {
        uVar10 = 7;
      }
      break;
    case 0x28:
    case 0x29:
    case 0x2a:
      uVar10 = 6;
      break;
    case 0x2b:
      uVar10 = 5;
      if (uVar13 < 10000000000000) {
        uVar10 = 6;
      }
      break;
    case 0x2c:
    case 0x2d:
      uVar10 = 5;
      break;
    case 0x2e:
      uVar10 = 4;
      if (uVar13 < 100000000000000) {
        uVar10 = 5;
      }
      break;
    case 0x2f:
    case 0x30:
      uVar10 = 4;
      break;
    case 0x31:
      uVar10 = 3;
      if (uVar13 < 1000000000000000) {
        uVar10 = 4;
      }
      break;
    case 0x32:
    case 0x33:
    case 0x34:
      uVar10 = 3;
      break;
    case 0x35:
      uVar10 = 2;
      if (uVar13 < 10000000000000000) {
        uVar10 = 3;
      }
      break;
    case 0x36:
    case 0x37:
      uVar10 = 2;
      break;
    case 0x38:
      uVar10 = 1;
      if (uVar13 < 100000000000000000) {
        uVar10 = 2;
      }
      break;
    case 0x39:
    case 0x3a:
      uVar10 = 1;
      break;
    case 0x3b:
      uVar10 = (uint)(uVar13 < 1000000000000000000);
      break;
    default:
      uVar10 = 0;
    }
  }
  if (uVar10 != 0) {
    if (0x12 < uVar10) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
                ,0x1750,"limb_t mp_shl_dec(limb_t *, const limb_t *, mp_size_t, limb_t, limb_t)",
                "shift >= 1 && shift < LIMB_DIGITS");
    }
    uVar13 = (ulong)uVar10;
    if (0 < lVar12) {
      lVar11 = 0;
      lVar16 = 0x13 - uVar13;
      uVar14 = (&DAT_0011e000)[lVar16 * 2];
      cVar1 = (&DAT_0011e008)[lVar16 * 0x10];
      cVar2 = (&DAT_0011e009)[lVar16 * 0x10];
      lVar16 = (&DAT_0011dda8)[lVar16];
      lVar17 = (&DAT_0011dda8)[uVar13];
      uVar15 = 0;
      do {
        lVar18 = lVar11 * 8;
        lVar11 = lVar11 + 1;
        uVar5 = *(ulong *)(lVar9 + lVar18);
        auVar3._8_8_ = 0;
        auVar3._0_8_ = uVar5;
        auVar4._8_8_ = 0;
        auVar4._0_8_ = uVar14;
        lVar7 = SUB168(auVar3 * auVar4,8);
        uVar8 = (uVar5 - lVar7 >> ((long)cVar1 & 0x3fU)) + lVar7 >> ((long)cVar2 & 0x3fU);
        *(ulong *)(lVar9 + lVar18) = uVar15 + (uVar5 - uVar8 * lVar16) * lVar17;
        uVar15 = uVar8;
      } while (lVar12 != lVar11);
    }
    param_1[2] = param_1[2] - uVar13;
  }
  uVar6 = FUN_001dfc74(param_1);
  return uVar6;
}

/* ===== FUN_001e1144 @ 001e1144 [libNexusScriptRuntime69252.so] ===== */

void FUN_001e1144(undefined8 **param_1,undefined8 **param_2,undefined8 **param_3,
                 undefined8 **param_4,undefined8 param_5,undefined4 param_6,uint param_7)

{
  uint uVar1;
  long lVar2;
  undefined8 *puVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  void *pvVar7;
  undefined8 *puVar8;
  undefined8 *puVar9;
  undefined8 *puVar10;
  undefined8 *puVar11;
  ulong uVar12;
  ulong uVar13;
  undefined8 *puVar14;
  undefined8 *local_e0;
  undefined4 local_d8;
  undefined8 *local_d0;
  undefined8 *local_c8;
  void *local_c0;
  undefined1 auStack_b8 [8];
  undefined4 local_b0;
  undefined8 *local_a8;
  undefined8 *local_a0;
  undefined8 *puStack_98;
  undefined8 *puStack_90;
  undefined4 local_88;
  undefined8 *local_80;
  undefined8 *local_78;
  undefined8 *local_70;
  long local_68;
  
  lVar2 = tpidr_el0;
  local_68 = *(long *)(lVar2 + 0x28);
  if ((param_1 == param_3) || (param_1 == param_4)) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
              ,0x1b36,
              "int bfdec_divrem(bfdec_t *, bfdec_t *, const bfdec_t *, const bfdec_t *, limb_t, bf_flags_t, int)"
              ,"q != a && q != b");
  }
  if ((param_2 == param_3) || (param_2 == param_4)) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
              ,0x1b37,
              "int bfdec_divrem(bfdec_t *, bfdec_t *, const bfdec_t *, const bfdec_t *, limb_t, bf_flags_t, int)"
              ,"r != a && r != b");
  }
  if (param_1 == param_2) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
              ,0x1b38,
              "int bfdec_divrem(bfdec_t *, bfdec_t *, const bfdec_t *, const bfdec_t *, limb_t, bf_flags_t, int)"
              ,"q != r");
  }
  puVar8 = param_3[3];
  puVar14 = *param_1;
  if ((puVar8 == (undefined8 *)0x0) || (puVar9 = param_4[3], puVar9 == (undefined8 *)0x0)) {
    if (param_1[3] != (undefined8 *)0x0) {
      puVar8 = (undefined8 *)(*(code *)puVar14[1])(*puVar14,param_1[4],0);
      param_1[3] = (undefined8 *)0x0;
      param_1[4] = puVar8;
    }
    *(undefined4 *)(param_1 + 1) = 0;
    param_1[2] = (undefined8 *)0x8000000000000000;
    if ((param_3[2] == (undefined8 *)0x7fffffffffffffff) ||
       (param_4[2] == (undefined8 *)0x7fffffffffffffff)) {
      if (param_2[3] != (undefined8 *)0x0) {
        puVar8 = (undefined8 *)(*(code *)(*param_2)[1])(**param_2,param_2[4],0);
        param_2[3] = (undefined8 *)0x0;
        param_2[4] = puVar8;
      }
      puVar8 = (undefined8 *)0x0;
LAB_001e1270:
      *(undefined4 *)(param_2 + 1) = 0;
      param_2[2] = (undefined8 *)0x7fffffffffffffff;
    }
    else {
      if ((param_3[2] == (undefined8 *)0x7ffffffffffffffe) ||
         (param_4[2] == (undefined8 *)0x8000000000000000)) {
        if (param_2[3] != (undefined8 *)0x0) {
          puVar8 = (undefined8 *)(*(code *)(*param_2)[1])(**param_2,param_2[4],0);
          param_2[3] = (undefined8 *)0x0;
          param_2[4] = puVar8;
        }
        puVar8 = (undefined8 *)0x1;
        goto LAB_001e1270;
      }
      puVar8 = param_3[3];
      if (param_2[3] == puVar8) {
LAB_001e168c:
        *(undefined4 *)(param_2 + 1) = *(undefined4 *)(param_3 + 1);
        param_2[2] = param_3[2];
        puVar8 = (undefined8 *)memcpy(param_2[4],param_3[4],(long)param_3[3] << 3);
      }
      else {
        puVar14 = (undefined8 *)(*(code *)(*param_2)[1])(**param_2,param_2[4],(long)puVar8 << 3);
        if ((puVar8 == (undefined8 *)0x0) || (puVar14 != (undefined8 *)0x0)) {
          param_2[3] = puVar8;
          param_2[4] = puVar14;
          goto LAB_001e168c;
        }
        puVar8 = (undefined8 *)0x0;
        if (param_2[3] != (undefined8 *)0x0) {
          puVar8 = (undefined8 *)(*(code *)(*param_2)[1])(**param_2,param_2[4],0);
          param_2[3] = (undefined8 *)0x0;
          param_2[4] = puVar8;
        }
        *(undefined4 *)(param_2 + 1) = 0;
        param_2[2] = (undefined8 *)0x7fffffffffffffff;
      }
      if (param_2[3] != (undefined8 *)0x0) {
        if (*(long *)(lVar2 + 0x28) == local_68) {
          FUN_001dfc74(param_2,param_5,param_6);
          return;
        }
        goto LAB_001e1894;
      }
LAB_001e1804:
      puVar8 = (undefined8 *)0x0;
    }
  }
  else {
    uVar1 = *(uint *)(param_4 + 1) ^ *(uint *)(param_3 + 1);
    uVar5 = 0;
    switch(param_7) {
    case 2:
      uVar5 = uVar1;
      break;
    case 3:
      uVar5 = uVar1 ^ 1;
      break;
    case 5:
      uVar5 = 1;
      break;
    case 6:
      uVar5 = *(uint *)(param_3 + 1);
    }
    local_70 = param_3[4];
    local_88 = 0;
    local_80 = param_3[2];
    local_b0 = 0;
    puStack_98 = param_4[4];
    local_a8 = param_4[2];
    local_a0 = puVar9;
    local_78 = puVar8;
    if (local_80 == local_a8) {
      puVar10 = puVar9;
      puVar11 = puVar8;
      puVar3 = puVar8;
      if ((long)puVar8 <= (long)puVar9) {
        puVar3 = puVar9;
      }
      do {
        puVar11 = (undefined8 *)((long)puVar11 + -1);
        puVar10 = (undefined8 *)((long)puVar10 + -1);
        if ((long)puVar3 + -1 < 0) goto LAB_001e146c;
        if (puVar11 < puVar8) {
          uVar12 = local_70[(long)puVar11];
          if (puVar9 <= puVar10) goto LAB_001e1398;
LAB_001e1358:
          uVar13 = puStack_98[(long)puVar10];
        }
        else {
          uVar12 = 0;
          if (puVar10 < puVar9) goto LAB_001e1358;
LAB_001e1398:
          uVar13 = 0;
        }
        puVar3 = (undefined8 *)((long)puVar3 + -1);
      } while (uVar12 == uVar13);
      if (uVar13 <= uVar12) goto LAB_001e146c;
LAB_001e13a8:
      if (param_1[3] == (undefined8 *)0x1) {
LAB_001e13d0:
        *param_1[4] = 0;
        param_1[2] = (undefined8 *)0x13;
        *(undefined4 *)(param_1 + 1) = 0;
        FUN_001e030c(param_1,0x3fffffffffffffff,0);
      }
      else {
        puVar8 = (undefined8 *)(*(code *)puVar14[1])(*puVar14,param_1[4],8);
        if (puVar8 != (undefined8 *)0x0) {
          param_1[3] = (undefined8 *)0x1;
          param_1[4] = puVar8;
          goto LAB_001e13d0;
        }
        if (param_1[3] != (undefined8 *)0x0) {
          puVar8 = (undefined8 *)(*(code *)(*param_1)[1])(**param_1,param_1[4],0);
          param_1[3] = (undefined8 *)0x0;
          param_1[4] = puVar8;
        }
        *(undefined4 *)(param_1 + 1) = 0;
        param_1[2] = (undefined8 *)0x7fffffffffffffff;
      }
      puVar8 = local_78;
      if (&puStack_90 != param_2) {
        if (param_2[3] != local_78) {
          puVar9 = (undefined8 *)(*(code *)(*param_2)[1])(**param_2,param_2[4],(long)local_78 << 3);
          if ((puVar8 != (undefined8 *)0x0) && (puVar9 == (undefined8 *)0x0)) {
            if (param_2[3] != (undefined8 *)0x0) {
              puVar8 = (undefined8 *)(*(code *)(*param_2)[1])(**param_2,param_2[4],0);
              param_2[3] = (undefined8 *)0x0;
              param_2[4] = puVar8;
            }
            *(undefined4 *)(param_2 + 1) = 0;
            param_2[2] = (undefined8 *)0x7fffffffffffffff;
            goto LAB_001e1548;
          }
          param_2[3] = puVar8;
          param_2[4] = puVar9;
        }
        param_2[2] = local_80;
        *(undefined4 *)(param_2 + 1) = local_88;
        memcpy(param_2[4],local_70,(long)local_78 << 3);
      }
    }
    else {
      if ((long)local_80 < (long)local_a8) goto LAB_001e13a8;
LAB_001e146c:
      FUN_001d7210(param_1,&puStack_90,auStack_b8,0,0x11,FUN_001e0d60);
      FUN_001e08b0(param_2,param_1,auStack_b8,0x3fffffffffffffff,1);
      FUN_001e7024(param_2,&puStack_90,param_2,0x3fffffffffffffff,1);
    }
LAB_001e1548:
    if ((param_1[2] == (undefined8 *)0x7fffffffffffffff) ||
       (param_2[2] == (undefined8 *)0x7fffffffffffffff)) {
LAB_001e1564:
      if (param_1[3] != (undefined8 *)0x0) {
        puVar8 = (undefined8 *)(*(code *)(*param_1)[1])(**param_1,param_1[4],0);
        param_1[3] = (undefined8 *)0x0;
        param_1[4] = puVar8;
      }
      *(undefined4 *)(param_1 + 1) = 0;
      param_1[2] = (undefined8 *)0x7fffffffffffffff;
      if (param_2[3] != (undefined8 *)0x0) {
        puVar8 = (undefined8 *)(*(code *)(*param_2)[1])(**param_2,param_2[4],0);
        param_2[3] = (undefined8 *)0x0;
        param_2[4] = puVar8;
      }
      puVar8 = (undefined8 *)0x20;
      param_2[2] = (undefined8 *)0x7fffffffffffffff;
      *(undefined4 *)(param_2 + 1) = 0;
    }
    else {
      if (param_2[3] != (undefined8 *)0x0) {
        if ((param_7 | 4) == 4) {
          local_d8 = 0;
          local_c8 = (undefined8 *)0x0;
          local_c0 = (void *)0x0;
          local_d0 = (undefined8 *)0x8000000000000000;
          local_e0 = puVar14;
          if (&local_e0 != param_2) {
            puVar9 = param_2[3];
            puVar8 = local_c8;
            pvVar7 = local_c0;
            if (((puVar9 != (undefined8 *)0x0) &&
                (pvVar7 = (void *)(*(code *)puVar14[1])(*puVar14,0,(long)puVar9 << 3),
                puVar8 = puVar9, puVar9 != (undefined8 *)0x0)) && (pvVar7 == (void *)0x0)) {
              if (local_c8 != (undefined8 *)0x0) {
                local_c0 = (void *)(*(code *)local_e0[1])(*local_e0,local_c0,0);
                local_c8 = (undefined8 *)0x0;
              }
              local_d8 = 0;
              local_d0 = (undefined8 *)0x7fffffffffffffff;
              goto LAB_001e1564;
            }
            local_c0 = pvVar7;
            local_c8 = puVar8;
            local_d0 = param_2[2];
            local_d8 = *(undefined4 *)(param_2 + 1);
            memcpy(local_c0,param_2[4],(long)param_2[3] << 3);
          }
          iVar4 = FUN_001e0b84(&local_e0,&local_e0,2,0x3fffffffffffffff,1);
          if (iVar4 != 0) {
            if ((local_e0 != (undefined8 *)0x0) && (local_c0 != (void *)0x0)) {
              (*(code *)local_e0[1])(*local_e0,local_c0,0);
            }
            goto LAB_001e1564;
          }
          iVar4 = FUN_001e1898(&local_e0,param_4);
          if ((local_e0 != (undefined8 *)0x0) && (local_c0 != (void *)0x0)) {
            (*(code *)local_e0[1])(*local_e0,local_c0,0);
          }
          if ((0 < iVar4) ||
             ((iVar4 == 0 &&
              ((param_7 == 4 ||
               (uVar12 = FUN_001e1938(param_1[4],param_1[3],
                                      (long)param_1[3] * 0x13 - (long)param_1[2]), (uVar12 & 1) != 0
               )))))) goto LAB_001e178c;
        }
        else if (uVar5 != 0) {
LAB_001e178c:
          uVar5 = FUN_001e0c68(param_1,param_1,1,0x3fffffffffffffff,1);
          uVar6 = FUN_001e7024(param_2,param_2,auStack_b8,0x3fffffffffffffff,1);
          if (((uVar6 | uVar5) >> 5 & 1) != 0) goto LAB_001e1564;
        }
      }
      *(uint *)(param_2 + 1) = *(uint *)(param_2 + 1) ^ *(uint *)(param_3 + 1);
      *(uint *)(param_1 + 1) = uVar1;
      if (param_2[3] == (undefined8 *)0x0) goto LAB_001e1804;
      puVar8 = (undefined8 *)FUN_001dfc74(param_2,param_5,param_6);
    }
  }
  if (*(long *)(lVar2 + 0x28) == local_68) {
    return;
  }
LAB_001e1894:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(puVar8);
}

/* ===== FUN_001e1a9c @ 001e1a9c [libNexusScriptRuntime69252.so] ===== */

void FUN_001e1a9c(undefined8 *param_1,undefined8 *param_2,long param_3,uint param_4)

{
  bool bVar1;
  long lVar2;
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  undefined1 auVar6 [16];
  long lVar7;
  long lVar8;
  ulong *__s;
  undefined1 *puVar9;
  ulong uVar10;
  ulong uVar11;
  long lVar12;
  ulong uVar13;
  long lVar14;
  ulong uVar15;
  ulong *puVar16;
  long *plVar17;
  ulong uVar18;
  long lVar19;
  long lVar20;
  ulong uVar21;
  ulong uVar22;
  long lVar23;
  undefined8 uVar24;
  uint uVar25;
  undefined8 *puVar26;
  undefined1 auStack_a8 [64];
  long local_68;
  
  lVar7 = tpidr_el0;
  local_68 = *(long *)(lVar7 + 0x28);
  if (param_1 == param_2) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
              ,0x1baf,"int bfdec_sqrt(bfdec_t *, const bfdec_t *, limb_t, bf_flags_t)","r != a");
  }
  if (param_2[3] == 0) {
    if (param_2[2] == 0x7ffffffffffffffe) {
      if (*(int *)(param_2 + 1) != 0) goto LAB_001e1b44;
LAB_001e1b70:
      lVar12 = param_2[3];
      if (param_1[3] != lVar12) {
        lVar8 = (*(code *)((undefined8 *)*param_1)[1])
                          (*(undefined8 *)*param_1,param_1[4],lVar12 << 3);
        if ((lVar12 != 0) && (lVar8 == 0)) goto LAB_001e1b9c;
        param_1[3] = lVar12;
        param_1[4] = lVar8;
      }
      *(undefined4 *)(param_1 + 1) = *(undefined4 *)(param_2 + 1);
      param_1[2] = param_2[2];
      memcpy((void *)param_1[4],(void *)param_2[4],param_2[3] << 3);
      uVar24 = 0;
      goto LAB_001e1ffc;
    }
    if (param_2[2] != 0x7fffffffffffffff) goto LAB_001e1b70;
LAB_001e1b9c:
    if (param_1[3] != 0) {
      uVar24 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
      param_1[3] = 0;
      param_1[4] = uVar24;
    }
    uVar24 = 0;
  }
  else if ((param_3 == 0x3fffffffffffffff) || (*(int *)(param_2 + 1) != 0)) {
LAB_001e1b44:
    if (param_1[3] != 0) {
      uVar24 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
      param_1[3] = 0;
      param_1[4] = uVar24;
    }
    uVar24 = 1;
  }
  else {
    lVar12 = param_3;
    if ((param_4 >> 4 & 1) != 0) {
      lVar12 = param_2[2];
      if (lVar12 + 1 < 0 == SCARRY8(lVar12,1)) {
        uVar13 = lVar12 + 1U >> 1;
      }
      else {
        if (lVar12 < 0) {
          lVar12 = lVar12 + 1;
        }
        uVar13 = lVar12 >> 1;
      }
      lVar12 = uVar13 + param_3;
      if (lVar12 < 2) {
        lVar12 = 1;
      }
    }
    lVar12 = lVar12 * 2;
    lVar8 = lVar12 + 0x29;
    puVar26 = (undefined8 *)*param_2;
    lVar14 = SUB168(SEXT816(lVar8) * SEXT816(0x6bca1af286bca1b),8);
    uVar13 = lVar14 - (lVar14 >> 0x3f);
    if (param_1[3] == uVar13) {
LAB_001e1c3c:
      __s = (ulong *)(*(code *)puVar26[1])(*puVar26,0,uVar13 * 0x10);
      if (__s != (ulong *)0x0) {
        lVar23 = uVar13 * 2;
        lVar14 = lVar23;
        if ((long)param_2[3] <= lVar23) {
          lVar14 = param_2[3];
        }
        memset(__s,0,(lVar23 - lVar14) * 8);
        memcpy(__s + (uVar13 * 2 - lVar14),(void *)(param_2[4] + param_2[3] * 8 + lVar14 * -8),
               lVar14 * 8);
        uVar22 = 0;
        if (((*(byte *)(param_2 + 2) & 1) != 0) && (0x25 < lVar8)) {
          uVar15 = 0;
          lVar19 = lVar23;
          do {
            lVar2 = lVar19 + -1;
            uVar22 = __s[lVar19 + -1] % 10;
            __s[lVar19 + -1] = __s[lVar19 + -1] / 10 + uVar15 * 1000000000000000000;
            bVar1 = 0 < lVar19;
            uVar15 = uVar22;
            lVar19 = lVar2;
          } while (lVar2 != 0 && bVar1);
        }
        if (lVar8 < 0x4c) {
          uVar25 = 0;
        }
        else {
          uVar15 = __s[uVar13 * 2 + -1];
          if (uVar15 < 2500000000000000000) {
            uVar25 = 0;
            do {
              uVar25 = uVar25 + 1;
              uVar15 = uVar15 << 2;
            } while (uVar15 < 2500000000000000000);
          }
          else {
            uVar25 = 0;
          }
          if ((uVar25 != 0) && (0x25 < lVar8)) {
            uVar15 = 0;
            uVar18 = (ulong)(1 << (ulong)((uVar25 & 0xf) << 1));
            puVar16 = __s;
            do {
              uVar21 = *puVar16 * uVar18;
              auVar3._8_8_ = 0;
              auVar3._0_8_ = *puVar16;
              auVar6._8_8_ = 0;
              auVar6._0_8_ = uVar18;
              lVar19 = SUB168(auVar3 * auVar6,8);
              uVar10 = uVar15 + uVar21;
              if (CARRY8(uVar15,uVar21)) {
                lVar19 = lVar19 + 1;
              }
              auVar4._8_8_ = 0;
              auVar4._0_8_ = uVar10 >> 0x3f | lVar19 << 1;
              uVar15 = SUB168(auVar4 * ZEXT816(0xec1e4a7db69561a5),8);
              auVar5._8_8_ = 0;
              auVar5._0_8_ = uVar15;
              uVar21 = uVar10 + uVar15 * 0x7538dcfb76180000;
              lVar2 = -2;
              if (0x158e460913cfffff < uVar21) {
                lVar2 = -1;
              }
              lVar20 = (lVar19 - SUB168(auVar5 * ZEXT816(10000000000000000000),8)) -
                       (ulong)(uVar10 < uVar15 * -0x7538dcfb76180000);
              uVar21 = uVar21 + 0xea71b9f6ec300000;
              lVar19 = lVar20 + lVar2;
              uVar10 = lVar19 >> 1;
              uVar11 = uVar10 & 10000000000000000000;
              lVar23 = lVar23 + -1;
              uVar15 = uVar15 + uVar10 + lVar19 + (ulong)CARRY8(uVar11,uVar21) + 2;
              *puVar16 = (lVar20 + lVar2 + (ulong)CARRY8(uVar11,uVar21) & 10000000000000000000) +
                         uVar11 + uVar21;
              puVar16 = puVar16 + 1;
            } while (lVar23 != 0);
          }
        }
        uVar24 = param_1[4];
        puVar9 = auStack_a8;
        if ((uVar13 < 0x10) ||
           (puVar9 = (undefined1 *)
                     (*(code *)puVar26[1])(*puVar26,0,(uVar13 & 0x3ffffffffffffffe) * 4 + 8),
           puVar9 != (undefined1 *)0x0)) {
          uVar15 = FUN_001df624(uVar24,__s,uVar13,puVar9);
          __s[uVar13] = uVar15;
          if ((puVar9 != auStack_a8) && (puVar9 != (undefined1 *)0x0)) {
            (*(code *)puVar26[1])(*puVar26,puVar9,0);
          }
          if (uVar25 != 0) {
            FUN_001df2cc(param_1[4],param_1[4],uVar13,(long)(1 << (ulong)(uVar25 & 0x1f)),0);
          }
          if (uVar22 == 0) {
            if (lVar12 + 0x4e < 0 == SCARRY8(lVar8,0x25)) {
              lVar12 = uVar13 + 1;
              uVar22 = 1;
              puVar16 = __s;
              do {
                if (*puVar16 != 0) goto LAB_001e1f24;
                lVar12 = lVar12 + -1;
                puVar16 = puVar16 + 1;
              } while (lVar12 != 0);
            }
            uVar22 = 0;
          }
LAB_001e1f24:
          (*(code *)puVar26[1])(*puVar26,__s,0);
          if (uVar22 == 0) {
            lVar14 = param_2[3] - lVar14;
            if (0 < lVar14) {
              plVar17 = (long *)param_2[4];
              uVar22 = 1;
              do {
                if (*plVar17 != 0) goto LAB_001e1f6c;
                lVar14 = lVar14 + -1;
                plVar17 = plVar17 + 1;
              } while (lVar14 != 0);
            }
            uVar22 = 0;
          }
LAB_001e1f6c:
          if (uVar22 != 0) {
            *(ulong *)param_1[4] = *(ulong *)param_1[4] | 1;
          }
          *(undefined4 *)(param_1 + 1) = 0;
          param_1[2] = param_2[2] + 1 >> 1;
          if (param_1[3] == 0) {
            uVar24 = 0;
          }
          else {
            uVar24 = FUN_001dfc74(param_1,param_3,param_4);
          }
          goto LAB_001e1ffc;
        }
        (*(code *)puVar26[1])(*puVar26,__s,0);
      }
    }
    else {
      lVar14 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],uVar13 * 8)
      ;
      if ((0xffffffffffffffb4 < lVar12 + 3U) || (lVar14 != 0)) {
        param_1[3] = uVar13;
        param_1[4] = lVar14;
        goto LAB_001e1c3c;
      }
    }
    if (param_1[3] != 0) {
      uVar24 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
      param_1[3] = 0;
      param_1[4] = uVar24;
    }
    uVar24 = 0x20;
  }
  param_1[2] = 0x7fffffffffffffff;
  *(undefined4 *)(param_1 + 1) = 0;
LAB_001e1ffc:
  if (*(long *)(lVar7 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar24);
  }
  return;
}

/* ===== FUN_001e217c @ 001e217c [libNexusScriptRuntime69252.so] ===== */

ulong FUN_001e217c(undefined8 *param_1,undefined8 *param_2,ulong param_3)

{
  undefined4 uVar1;
  uint uVar2;
  long lVar3;
  undefined8 uVar4;
  void *__src;
  int iVar5;
  uint uVar6;
  long lVar7;
  ulong uVar8;
  
  if (param_1 == param_2) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
              ,0x1c2f,"int bfdec_pow_ui(bfdec_t *, const bfdec_t *, limb_t)","r != a");
  }
  if (param_3 == 0) {
    if (param_1[3] == 1) {
LAB_001e2234:
      *(undefined8 *)param_1[4] = 1;
      param_1[2] = 0x13;
      *(undefined4 *)(param_1 + 1) = 0;
      uVar8 = FUN_001e030c(param_1,0x3fffffffffffffff,0);
      return uVar8;
    }
    lVar7 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],8);
    if (lVar7 != 0) {
      param_1[3] = 1;
      param_1[4] = lVar7;
      goto LAB_001e2234;
    }
    if (param_1[3] != 0) {
      uVar4 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
      param_1[3] = 0;
      param_1[4] = uVar4;
    }
    uVar6 = 0x20;
    *(undefined4 *)(param_1 + 1) = 0;
    param_1[2] = 0x7fffffffffffffff;
    goto LAB_001e22a0;
  }
  lVar7 = param_2[3];
  if (param_1[3] == lVar7) {
LAB_001e2270:
    lVar7 = param_2[3];
    uVar1 = *(undefined4 *)(param_2 + 1);
    __src = (void *)param_2[4];
    param_1[2] = param_2[2];
    *(undefined4 *)(param_1 + 1) = uVar1;
    memcpy((void *)param_1[4],__src,lVar7 << 3);
    uVar6 = 0;
  }
  else {
    lVar3 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],lVar7 << 3);
    if ((lVar7 == 0) || (lVar3 != 0)) {
      param_1[3] = lVar7;
      param_1[4] = lVar3;
      goto LAB_001e2270;
    }
    if (param_1[3] != 0) {
      uVar4 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
      param_1[3] = 0;
      param_1[4] = uVar4;
    }
    uVar6 = 0x20;
    *(undefined4 *)(param_1 + 1) = 0;
    param_1[2] = 0x7fffffffffffffff;
  }
  if (LZCOUNT(param_3) != 0x3f) {
    iVar5 = (int)LZCOUNT(param_3);
    uVar8 = (ulong)(0x3e - iVar5);
    iVar5 = 0x3f - iVar5;
    do {
      uVar2 = FUN_001e08b0(param_1,param_1,param_1,0x3fffffffffffffff,1);
      uVar6 = uVar2 | uVar6;
      if ((param_3 >> (uVar8 & 0x3f) & 1) != 0) {
        uVar2 = FUN_001e08b0(param_1,param_1,param_2,0x3fffffffffffffff,1);
        uVar6 = uVar2 | uVar6;
      }
      uVar8 = uVar8 - 1;
      iVar5 = iVar5 + -1;
    } while (0 < iVar5);
  }
LAB_001e22a0:
  return (ulong)uVar6;
}

/* ===== FUN_001e3bf8 @ 001e3bf8 [libNexusScriptRuntime69252.so] ===== */

/* WARNING: Removing unreachable block (ram,0x001e3f68) */

void FUN_001e3bf8(long param_1,ulong *param_2,undefined8 *param_3,long param_4,ulong param_5,
                 long param_6,long param_7,ulong param_8)

{
  long lVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  undefined8 *puVar9;
  undefined8 uVar10;
  ulong uVar11;
  ulong uVar12;
  ulong uVar13;
  ulong uVar14;
  ulong uVar15;
  ulong uVar16;
  ulong uVar17;
  ulong uVar18;
  long lVar19;
  long lVar20;
  undefined8 *local_b8;
  int local_b0;
  long local_a8;
  ulong local_a0;
  undefined8 *local_98;
  undefined8 *local_90;
  undefined4 local_88;
  undefined8 local_80;
  long local_78;
  long local_70;
  long local_68;
  
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  if (param_4 == 1) {
    uVar13 = param_3[3];
    uVar12 = uVar13 * 0x40 - param_3[2];
    uVar14 = (long)uVar12 >> 6;
    if (uVar14 < uVar13) {
      uVar15 = *(ulong *)(param_3[4] + uVar14 * 8);
    }
    else {
      uVar15 = 0;
    }
    if ((uVar12 & 0x3f) != 0) {
      if (uVar14 + 1 < uVar13) {
        lVar19 = *(long *)(param_3[4] + (uVar14 + 1) * 8);
      }
      else {
        lVar19 = 0;
      }
      uVar15 = lVar19 << ((ulong)-((uint)uVar12 & 0x3f) & 0x3f) | uVar15 >> (uVar12 & 0x3f);
    }
    uVar10 = 0;
    *param_2 = uVar15;
    goto LAB_001e40fc;
  }
  if (param_4 == 2) {
    uVar13 = param_3[3];
    uVar12 = uVar13 * 0x40 - param_3[2];
    lVar19 = param_3[4];
    uVar14 = (long)(uVar12 + 0x40) >> 6;
    if (uVar14 < uVar13) {
      uVar15 = *(ulong *)(lVar19 + uVar14 * 8);
    }
    else {
      uVar15 = 0;
    }
    if ((uVar12 & 0x3f) != 0) {
      if (uVar14 + 1 < uVar13) {
        lVar20 = *(long *)(lVar19 + (uVar14 + 1) * 8);
      }
      else {
        lVar20 = 0;
      }
      uVar15 = lVar20 << ((ulong)-((uint)uVar12 & 0x3f) & 0x3f) | uVar15 >> (uVar12 & 0x3f);
    }
    uVar14 = (long)uVar12 >> 6;
    if (uVar14 < uVar13) {
      uVar18 = *(ulong *)(lVar19 + uVar14 * 8);
    }
    else {
      uVar18 = 0;
    }
    if ((uVar12 & 0x3f) != 0) {
      if (uVar14 + 1 < uVar13) {
        lVar19 = *(long *)(lVar19 + (uVar14 + 1) * 8);
      }
      else {
        lVar19 = 0;
      }
      uVar18 = lVar19 << ((ulong)-((uint)uVar12 & 0x3f) & 0x3f) | uVar18 >> (uVar12 & 0x3f);
    }
    if (param_7 == -0x7538dcfb76180000) {
      uVar13 = FUN_001e72dc(uVar18,uVar15,10000000000000000000,0);
      param_7 = uVar13 * -0x7538dcfb76180000;
    }
    else {
      uVar13 = FUN_001e72dc(uVar18,uVar15,param_7,0);
      param_7 = uVar13 * param_7;
    }
    uVar10 = 0;
    *param_2 = uVar18 - param_7;
    param_2[1] = uVar13;
    goto LAB_001e40fc;
  }
  if (param_4 == 0) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
              ,0xd4b,
              "int bf_integer_to_radix_rec(bf_t *, limb_t *, const bf_t *, limb_t, int, limb_t, limb_t, unsigned int)"
              ,"n >= 1");
  }
  local_b8 = (undefined8 *)*param_3;
  uVar12 = (ulong)((int)param_5 + 1);
  uVar13 = (param_5 & 0xffffffff) << 1;
  param_8 = param_8 & 0xffffffff;
  lVar20 = param_1 + (long)(int)param_5 * 0x50;
  uVar3 = 0;
  uVar14 = ((ulong)(param_6 << 1) >> (uVar12 & 0x3f)) + 1 >> 1;
  lVar19 = param_1 + (long)(int)((uint)uVar13 | 1) * 0x28;
  local_88 = 0;
  local_78 = 0;
  local_70 = 0;
  local_80 = 0x8000000000000000;
  local_b0 = 0;
  local_a0 = 0;
  local_98 = (undefined8 *)0x0;
  local_a8 = -0x8000000000000000;
  local_90 = local_b8;
  if (*(long *)(lVar20 + 0x18) == 0) {
    uVar2 = FUN_001d8824(lVar20,param_7,uVar14,0x3fffffffffffffff,1);
    local_b0 = 0;
    if (local_a0 == 1) {
LAB_001e3d0c:
      uVar4 = 0;
      *local_98 = 0x8000000000000000;
      local_a8 = 1;
    }
    else {
      puVar9 = (undefined8 *)(*(code *)local_b8[1])(*local_b8,local_98,8);
      if (puVar9 != (undefined8 *)0x0) {
        local_a0 = 1;
        local_98 = puVar9;
        goto LAB_001e3d0c;
      }
      if (local_a0 != 0) {
        local_98 = (undefined8 *)(*(code *)local_b8[1])(*local_b8,local_98,0);
        local_a0 = 0;
      }
      uVar4 = 0x20;
      local_b0 = 0;
      local_a8 = 0x7fffffffffffffff;
    }
    uVar3 = FUN_001d7210(lVar19,&local_b8,lVar20,param_8 + param_8 * uVar14 + 2,0,FUN_001d72fc);
    uVar3 = uVar4 | uVar2 | uVar3;
  }
  uVar2 = FUN_001d56bc(&local_90,param_3,lVar19,(param_4 - uVar14) * param_8,0);
  if (local_78 == 0) {
    uVar4 = 0;
  }
  else {
    uVar4 = FUN_001d3a6c(&local_90,0,0x11,local_78,0);
  }
  uVar5 = FUN_001d56bc(&local_b8,&local_90,lVar20,0x3fffffffffffffff,1);
  uVar6 = FUN_001e71f4(&local_b8,param_3,&local_b8,0x3fffffffffffffff,1);
  if (((uVar2 | uVar3 | uVar4 | uVar5 | uVar6) >> 5 & 1) == 0) {
    iVar8 = 0;
    if ((local_b0 != 0) && (local_a0 != 0)) {
      iVar8 = 0;
      do {
        iVar7 = FUN_001e710c(&local_b8,&local_b8,lVar20,0x3fffffffffffffff,1);
        if (iVar7 != 0) goto LAB_001e40c0;
        iVar8 = iVar8 + -1;
      } while ((local_b0 != 0) && (local_a0 != 0));
    }
    lVar19 = param_1 + (-(param_5 >> 0x1f & 1) & 0xfffffffe00000000 | uVar13) * 0x28;
    do {
      if (local_a8 == *(long *)(lVar19 + 0x10)) {
        uVar11 = *(ulong *)(lVar19 + 0x18);
        uVar13 = uVar11;
        uVar15 = local_a0;
        uVar18 = local_a0;
        if ((long)local_a0 <= (long)uVar11) {
          uVar18 = uVar11;
        }
        do {
          uVar15 = uVar15 - 1;
          uVar13 = uVar13 - 1;
          if ((long)(uVar18 - 1) < 0) goto LAB_001e3f74;
          if (uVar15 < local_a0) {
            uVar16 = local_98[uVar15];
            if (uVar11 <= uVar13) goto LAB_001e3f4c;
LAB_001e3f08:
            uVar17 = *(ulong *)(*(long *)(lVar19 + 0x20) + uVar13 * 8);
          }
          else {
            uVar16 = 0;
            if (uVar13 < uVar11) goto LAB_001e3f08;
LAB_001e3f4c:
            uVar17 = 0;
          }
          uVar18 = uVar18 - 1;
        } while (uVar16 == uVar17);
        iVar7 = 1;
        if (uVar16 < uVar17) {
          iVar7 = -1;
        }
      }
      else {
        iVar7 = 1;
        if (local_a8 < *(long *)(lVar19 + 0x10)) {
          iVar7 = -1;
        }
      }
      if (iVar7 < 0) {
        if ((((iVar8 == 0) ||
             (iVar8 = FUN_001d6220(&local_90,&local_90,(long)iVar8,0x3fffffffffffffff,1), iVar8 == 0
             )) && (iVar8 = FUN_001e3bf8(param_1,param_2 + uVar14,&local_90,param_4 - uVar14,uVar12,
                                         param_6,param_7,param_8), iVar8 == 0)) &&
           (iVar8 = FUN_001e3bf8(param_1,param_2,&local_b8,uVar14,uVar12,param_6,param_7,param_8),
           iVar8 == 0)) {
          if ((local_90 != (undefined8 *)0x0) && (local_70 != 0)) {
            (*(code *)local_90[1])(*local_90,local_70,0);
          }
          if ((local_b8 != (undefined8 *)0x0) && (local_98 != (undefined8 *)0x0)) {
            (*(code *)local_b8[1])(*local_b8,local_98,0);
          }
          uVar10 = 0;
          goto LAB_001e40fc;
        }
        break;
      }
LAB_001e3f74:
      iVar7 = FUN_001e71f4(&local_b8,&local_b8,lVar20,0x3fffffffffffffff,1);
      if (iVar7 != 0) break;
      iVar8 = iVar8 + 1;
    } while( true );
  }
LAB_001e40c0:
  if ((local_90 != (undefined8 *)0x0) && (local_70 != 0)) {
    (*(code *)local_90[1])(*local_90,local_70,0);
  }
  if ((local_b8 != (undefined8 *)0x0) && (local_98 != (undefined8 *)0x0)) {
    (*(code *)local_b8[1])(*local_b8,local_98,0);
  }
  uVar10 = 0xffffffff;
LAB_001e40fc:
  if (*(long *)(lVar1 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar10);
}

/* ===== FUN_001e4ba4 @ 001e4ba4 [libNexusScriptRuntime69252.so] ===== */

void FUN_001e4ba4(undefined8 **param_1,undefined8 **param_2,undefined8 **param_3,ulong param_4)

{
  bool bVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  uint uVar5;
  undefined8 *puVar6;
  long lVar7;
  void *pvVar8;
  long *plVar9;
  long lVar10;
  ulong uVar11;
  uint uVar12;
  undefined8 *puVar13;
  undefined8 *puVar14;
  ulong uVar15;
  long lVar16;
  long lVar17;
  ulong local_f0;
  undefined8 *local_e8;
  uint local_e0;
  long local_d8;
  long local_d0;
  undefined8 *local_c8;
  undefined8 *local_c0;
  undefined4 local_b8;
  long local_b0;
  long local_a8;
  long *local_a0;
  undefined8 *local_98;
  uint local_90;
  undefined8 *local_88;
  undefined8 *local_80;
  void *local_78;
  long local_70;
  
  lVar4 = tpidr_el0;
  local_70 = *(long *)(lVar4 + 0x28);
  if ((param_2 == param_3) || (param_1 == param_3)) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
              ,0x1308,"int bf_sincos(bf_t *, bf_t *, const bf_t *, limb_t)","c != a && s != a");
  }
  puVar14 = *param_3;
  local_90 = 0;
  local_80 = (undefined8 *)0x0;
  local_78 = (void *)0x0;
  local_88 = (undefined8 *)0x8000000000000000;
  local_c8 = (undefined8 *)0x0;
  local_b8 = 0;
  local_a8 = 0;
  local_a0 = (long *)0x0;
  local_b0 = -0x8000000000000000;
  local_e0 = 0;
  local_d8 = 0x8000000000000000;
  local_d0 = 0;
  local_e8 = puVar14;
  local_c0 = puVar14;
  local_98 = puVar14;
  lVar7 = FUN_001d6604(param_4 >> 1);
  uVar15 = lVar7 * 2;
  uVar11 = 0;
  if (uVar15 != 0) {
    uVar11 = param_4 / uVar15;
  }
  lVar16 = uVar11 + 1;
  lVar10 = uVar15 + param_4 + lVar16;
  lVar2 = lVar10 + 8;
  if ((long)param_3[2] < 0) {
    if (&local_98 != param_3) {
      puVar13 = param_3[3];
      puVar6 = local_80;
      pvVar8 = local_78;
      if (((local_80 != puVar13) &&
          (pvVar8 = (void *)(*(code *)puVar14[1])(*puVar14,local_78,(long)puVar13 << 3),
          puVar6 = puVar13, puVar13 != (undefined8 *)0x0)) && (pvVar8 == (void *)0x0)) {
        if (local_80 != (undefined8 *)0x0) {
          local_78 = (void *)(*(code *)local_98[1])(*local_98,local_78,0);
          local_80 = (undefined8 *)0x0;
        }
        uVar15 = 0;
        local_90 = 0;
        local_88 = (undefined8 *)0x7fffffffffffffff;
        goto LAB_001e4dcc;
      }
      local_78 = pvVar8;
      local_80 = puVar6;
      local_88 = param_3[2];
      local_90 = *(uint *)(param_3 + 1);
      memcpy(local_78,param_3[4],(long)param_3[3] << 3);
    }
    uVar15 = 0;
  }
  else {
    lVar17 = 0;
    while( true ) {
      lVar3 = lVar17 + lVar2 + (long)param_3[2];
      FUN_001da384(&local_c0,lVar3,6,local_c0 + 8,FUN_001e4504,0);
      if (local_a8 != 0) {
        local_b0 = local_b0 + -1;
        FUN_001d3a6c(&local_c0,0x3fffffffffffffff,1,local_a8,0);
      }
      FUN_001d64c0(&local_f0,&local_98,param_3,&local_c0,lVar3,0,0);
      if ((local_f0 == 0) ||
         ((local_88 != (undefined8 *)0x8000000000000000 && (lVar10 + 7 <= (long)local_88 + lVar3))))
      break;
      lVar3 = lVar17 * 3 + 3;
      lVar17 = lVar17 * 3 + 4;
      if (-1 < lVar3) {
        lVar17 = lVar3;
      }
      lVar17 = lVar17 >> 1;
      if ((undefined8 *)-lVar17 != local_88 && lVar17 <= -(long)local_88) {
        lVar17 = -(long)local_88;
      }
    }
    uVar15 = local_f0 & 3;
  }
LAB_001e4dcc:
  uVar5 = local_90;
  local_f0 = uVar15;
  FUN_001d56bc(&local_98,&local_98,&local_98,lVar2,0);
  if (local_80 != (undefined8 *)0x0) {
    lVar10 = -0x3fffffffffffffff;
    if (-0x3fffffffffffffff < lVar7 * -2) {
      lVar10 = lVar7 * -2;
    }
    if (0x3ffffffffffffffe < lVar10) {
      lVar10 = 0x3fffffffffffffff;
    }
    local_88 = (undefined8 *)((long)local_88 + lVar10);
    FUN_001d3a6c(&local_98,0x3fffffffffffffff,1,local_80,0);
  }
  local_e0 = 0;
  if (local_d0 != 1) {
    puVar14 = (undefined8 *)(*(code *)local_e8[1])(*local_e8,local_c8,8);
    if (puVar14 == (undefined8 *)0x0) {
      if (local_d0 != 0) {
        local_c8 = (undefined8 *)(*(code *)local_e8[1])(*local_e8,local_c8,0);
        local_d0 = 0;
      }
      local_e0 = 0;
      local_d8 = 0x7fffffffffffffff;
      goto LAB_001e4e78;
    }
    local_d0 = 1;
    local_c8 = puVar14;
  }
  *local_c8 = 0x8000000000000000;
  local_d8 = 1;
LAB_001e4e78:
  if (uVar11 < 0x7fffffffffffffff) {
    do {
      local_b8 = 0;
      if (local_a8 == 1) {
LAB_001e4ed4:
        lVar10 = lVar16 * 2 + -1;
        uVar11 = LZCOUNT(lVar10);
        local_b0 = 0x40 - uVar11;
        *local_a0 = lVar10 << (uVar11 & 0x3f);
      }
      else {
        plVar9 = (long *)(*(code *)local_c0[1])(*local_c0,local_a0,8);
        if (plVar9 != (long *)0x0) {
          local_a8 = 1;
          local_a0 = plVar9;
          goto LAB_001e4ed4;
        }
        if (local_a8 != 0) {
          local_a0 = (long *)(*(code *)local_c0[1])(*local_c0,local_a0,0);
          local_a8 = 0;
        }
        local_b0 = 0x7fffffffffffffff;
        local_b8 = 0;
      }
      FUN_001d75d0(&local_c0,&local_c0,lVar16 * 2,0x3fffffffffffffff,1);
      FUN_001d7210(&local_c0,&local_98,&local_c0,lVar2,0,FUN_001d72fc);
      FUN_001d56bc(&local_e8,&local_e8,&local_c0,lVar2,0);
      local_e0 = local_e0 ^ 1;
      if (lVar16 != 1) {
        FUN_001d6220(&local_e8,&local_e8,1,lVar2,0);
      }
      lVar10 = lVar16 + -1;
      bVar1 = 0 < lVar16;
      lVar16 = lVar10;
    } while (lVar10 != 0 && bVar1);
  }
  if ((local_c0 != (undefined8 *)0x0) && (local_a0 != (long *)0x0)) {
    (*(code *)local_c0[1])(*local_c0,local_a0,0);
  }
  if (0 < lVar7) {
    do {
      FUN_001d56bc(&local_98,&local_e8,&local_e8,lVar2,0);
      if (local_d0 != 0) {
        local_d8 = local_d8 + 1;
        FUN_001d3a6c(&local_e8,0x3fffffffffffffff,1,local_d0,0);
      }
      FUN_001e710c(&local_e8,&local_e8,&local_98,lVar2,0);
      if (local_d0 != 0) {
        local_d8 = local_d8 + 1;
        FUN_001d3a6c(&local_e8,0x3fffffffffffffff,1,local_d0,0);
      }
      lVar7 = lVar7 + -1;
    } while (lVar7 != 0);
  }
  if ((local_98 != (undefined8 *)0x0) && (local_78 != (void *)0x0)) {
    (*(code *)local_98[1])(*local_98,local_78,0);
  }
  uVar12 = (uint)(uVar15 >> 1);
  if (param_2 != (undefined8 **)0x0) {
    if ((uVar15 & 1) == 0) {
      FUN_001d6220(param_2,&local_e8,1,lVar2,0);
    }
    else {
      FUN_001e5194(param_2,&local_e8,lVar2);
      *(uint *)(param_2 + 1) = uVar5 ^ 1;
    }
    *(uint *)(param_2 + 1) = *(uint *)(param_2 + 1) ^ uVar12;
  }
  if (param_1 != (undefined8 **)0x0) {
    if ((uVar15 & 1) == 0) {
      FUN_001e5194(param_1,&local_e8,lVar2);
      *(uint *)(param_1 + 1) = uVar5;
    }
    else {
      FUN_001d6220(param_1,&local_e8,1,lVar2,0);
    }
    *(uint *)(param_1 + 1) = *(uint *)(param_1 + 1) ^ uVar12;
  }
  if ((local_e8 != (undefined8 *)0x0) && (local_c8 != (undefined8 *)0x0)) {
    (*(code *)local_e8[1])(*local_e8,local_c8,0);
  }
  if (*(long *)(lVar4 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_001e5324 @ 001e5324 [libNexusScriptRuntime69252.so] ===== */

void FUN_001e5324(undefined8 *param_1,long param_2,ulong *param_3,long param_4,ulong *param_5,
                 long param_6)

{
  undefined1 auVar1 [16];
  undefined1 auVar2 [16];
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  undefined1 auVar6 [16];
  undefined1 auVar7 [16];
  undefined1 auVar8 [16];
  undefined1 auVar9 [16];
  undefined1 auVar10 [16];
  undefined1 auVar11 [16];
  undefined1 auVar12 [16];
  undefined1 auVar13 [16];
  long lVar14;
  bool bVar15;
  undefined8 uVar16;
  ulong *puVar17;
  long lVar18;
  ulong *puVar19;
  ulong *puVar20;
  long lVar21;
  ulong uVar22;
  long lVar23;
  long lVar24;
  long lVar25;
  ulong uVar26;
  ulong uVar27;
  ulong uVar28;
  ulong uVar29;
  ulong uVar30;
  long lVar31;
  ulong local_128;
  ulong local_118;
  ulong auStack_f0 [16];
  long local_70;
  
  lVar14 = tpidr_el0;
  local_70 = *(long *)(lVar14 + 0x28);
  uVar30 = param_5[param_6 + -1];
  if (uVar30 == 0) {
                    /* WARNING: Subroutine does not return */
    __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
              ,0x16de,
              "int mp_div_dec(bf_context_t *, limb_t *, limb_t *, mp_size_t, const limb_t *, mp_size_t)"
              ,"r != 0");
  }
  lVar31 = param_4 - param_6;
  if (uVar30 < 5000000000000000000) {
    if (param_6 < 0x11) {
      puVar17 = auStack_f0;
    }
    else {
      puVar17 = (ulong *)(*(code *)param_1[1])(*param_1,0,param_6 << 3);
      if (puVar17 == (ulong *)0x0) {
        uVar16 = 0xffffffff;
        goto LAB_001e57cc;
      }
    }
    local_118 = 0;
    if (uVar30 + 1 != 0) {
      local_118 = 10000000000000000000 / (uVar30 + 1);
    }
    if (0 < param_6) {
      uVar30 = 0;
      puVar19 = puVar17;
      lVar21 = param_6;
      do {
        uVar27 = *param_5 * local_118;
        auVar1._8_8_ = 0;
        auVar1._0_8_ = *param_5;
        auVar11._8_8_ = 0;
        auVar11._0_8_ = local_118;
        lVar18 = SUB168(auVar1 * auVar11,8);
        uVar22 = uVar30 + uVar27;
        if (CARRY8(uVar30,uVar27)) {
          lVar18 = lVar18 + 1;
        }
        auVar2._8_8_ = 0;
        auVar2._0_8_ = uVar22 >> 0x3f | lVar18 << 1;
        uVar30 = SUB168(auVar2 * ZEXT816(0xec1e4a7db69561a5),8);
        auVar3._8_8_ = 0;
        auVar3._0_8_ = uVar30;
        uVar27 = uVar22 + uVar30 * 0x7538dcfb76180000;
        lVar24 = -2;
        if (0x158e460913cfffff < uVar27) {
          lVar24 = -1;
        }
        lVar23 = (lVar18 - SUB168(auVar3 * ZEXT816(10000000000000000000),8)) -
                 (ulong)(uVar22 < uVar30 * -0x7538dcfb76180000);
        uVar27 = uVar27 + 0xea71b9f6ec300000;
        lVar18 = lVar23 + lVar24;
        uVar22 = lVar18 >> 1;
        uVar26 = uVar22 & 10000000000000000000;
        lVar21 = lVar21 + -1;
        uVar30 = uVar30 + uVar22 + lVar18 + (ulong)CARRY8(uVar26,uVar27) + 2;
        *puVar19 = (lVar23 + lVar24 + (ulong)CARRY8(uVar26,uVar27) & 10000000000000000000) +
                   uVar26 + uVar27;
        puVar19 = puVar19 + 1;
        param_5 = param_5 + 1;
      } while (lVar21 != 0);
    }
    if (param_4 < 1) {
      uVar30 = 0;
    }
    else {
      uVar30 = 0;
      puVar19 = param_3;
      lVar21 = param_4;
      do {
        uVar27 = *puVar19 * local_118;
        auVar4._8_8_ = 0;
        auVar4._0_8_ = *puVar19;
        auVar12._8_8_ = 0;
        auVar12._0_8_ = local_118;
        lVar18 = SUB168(auVar4 * auVar12,8);
        uVar22 = uVar30 + uVar27;
        if (CARRY8(uVar30,uVar27)) {
          lVar18 = lVar18 + 1;
        }
        auVar5._8_8_ = 0;
        auVar5._0_8_ = uVar22 >> 0x3f | lVar18 << 1;
        uVar30 = SUB168(auVar5 * ZEXT816(0xec1e4a7db69561a5),8);
        auVar6._8_8_ = 0;
        auVar6._0_8_ = uVar30;
        uVar27 = uVar22 + uVar30 * 0x7538dcfb76180000;
        lVar24 = -2;
        if (0x158e460913cfffff < uVar27) {
          lVar24 = -1;
        }
        lVar23 = (lVar18 - SUB168(auVar6 * ZEXT816(10000000000000000000),8)) -
                 (ulong)(uVar22 < uVar30 * -0x7538dcfb76180000);
        uVar27 = uVar27 + 0xea71b9f6ec300000;
        lVar18 = lVar23 + lVar24;
        uVar22 = lVar18 >> 1;
        uVar26 = uVar22 & 10000000000000000000;
        lVar21 = lVar21 + -1;
        uVar30 = uVar30 + uVar22 + lVar18 + (ulong)CARRY8(uVar26,uVar27) + 2;
        *puVar19 = (lVar23 + lVar24 + (ulong)CARRY8(uVar26,uVar27) & 10000000000000000000) +
                   uVar26 + uVar27;
        puVar19 = puVar19 + 1;
      } while (lVar21 != 0);
    }
    param_3[param_4] = uVar30;
    param_5 = puVar17;
  }
  else {
    if (0 < param_6) {
      puVar17 = param_3 + param_4;
      lVar21 = param_6;
      do {
        puVar17 = puVar17 + -1;
        uVar30 = param_5[lVar21 + -1];
        uVar22 = *puVar17;
        if (uVar22 != uVar30) {
          *(ulong *)(param_2 + lVar31 * 8) = (ulong)(uVar30 <= uVar22);
          if (uVar30 <= uVar22) goto LAB_001e540c;
          goto LAB_001e55ac;
        }
        lVar21 = lVar21 + -1;
      } while (0 < lVar21);
    }
    *(undefined8 *)(param_2 + lVar31 * 8) = 1;
LAB_001e540c:
    if (0 < param_6) {
      uVar30 = 0;
      lVar21 = -param_6;
      puVar17 = param_5;
      do {
        uVar26 = param_3[param_4 + lVar21];
        uVar22 = *puVar17 + uVar30;
        uVar30 = (ulong)(uVar26 < uVar22);
        uVar27 = (uVar26 - uVar22) + 10000000000000000000;
        if (uVar26 >= uVar22) {
          uVar27 = uVar26 - uVar22;
        }
        bVar15 = lVar21 != -1;
        param_3[param_4 + lVar21] = uVar27;
        lVar21 = lVar21 + 1;
        puVar17 = puVar17 + 1;
      } while (bVar15);
    }
LAB_001e55ac:
    lVar31 = lVar31 + -1;
    local_118 = 1;
  }
  if (-1 < lVar31) {
    puVar17 = param_3 + lVar31;
    local_128 = 9999999999999999999;
    do {
      lVar21 = lVar31 + param_6;
      uVar30 = param_3[lVar21];
      if (uVar30 < param_5[param_6 + -1]) {
        auVar7._8_8_ = 0;
        auVar7._0_8_ = uVar30;
        lVar18 = SUB168(auVar7 * ZEXT816(10000000000000000000),8);
        if (CARRY8(param_3[lVar21 + -1],uVar30 * -0x7538dcfb76180000)) {
          lVar18 = lVar18 + 1;
        }
        uVar30 = FUN_001e72dc(param_3[lVar21 + -1] + uVar30 * -0x7538dcfb76180000,lVar18,
                              param_5[param_6 + -1],0);
        if (param_6 < 1) goto LAB_001e56f4;
LAB_001e564c:
        uVar22 = 0;
        puVar19 = param_5;
        puVar20 = puVar17;
        lVar18 = param_6;
        do {
          uVar26 = *puVar19 * uVar30;
          auVar8._8_8_ = 0;
          auVar8._0_8_ = *puVar19;
          auVar13._8_8_ = 0;
          auVar13._0_8_ = uVar30;
          lVar24 = SUB168(auVar8 * auVar13,8);
          uVar27 = uVar22 + uVar26;
          if (CARRY8(uVar22,uVar26)) {
            lVar24 = lVar24 + 1;
          }
          auVar9._8_8_ = 0;
          auVar9._0_8_ = uVar27 >> 0x3f | lVar24 << 1;
          uVar26 = SUB168(auVar9 * ZEXT816(0xec1e4a7db69561a5),8);
          auVar10._8_8_ = 0;
          auVar10._0_8_ = uVar26;
          uVar22 = uVar27 + uVar26 * 0x7538dcfb76180000;
          lVar23 = -2;
          if (0x158e460913cfffff < uVar22) {
            lVar23 = -1;
          }
          lVar25 = (lVar24 - SUB168(auVar10 * ZEXT816(10000000000000000000),8)) -
                   (ulong)(uVar27 < uVar26 * -0x7538dcfb76180000);
          uVar22 = uVar22 + 0xea71b9f6ec300000;
          lVar24 = lVar25 + lVar23;
          uVar28 = lVar24 >> 1;
          uVar29 = uVar28 & 10000000000000000000;
          uVar27 = (lVar25 + lVar23 + (ulong)CARRY8(uVar29,uVar22) & 10000000000000000000) +
                   uVar29 + uVar22;
          lVar24 = uVar26 + uVar28 + lVar24 + (ulong)CARRY8(uVar29,uVar22);
          uVar26 = *puVar20 - uVar27;
          if (*puVar20 < uVar27) {
            lVar24 = lVar24 + 1;
            uVar26 = uVar26 + 10000000000000000000;
          }
          uVar22 = lVar24 + 2;
          lVar18 = lVar18 + -1;
          *puVar20 = uVar26;
          puVar19 = puVar19 + 1;
          puVar20 = puVar20 + 1;
        } while (lVar18 != 0);
      }
      else {
        uVar30 = local_128;
        if (0 < param_6) goto LAB_001e564c;
LAB_001e56f4:
        uVar22 = 0;
      }
      uVar26 = param_3[lVar21];
      uVar27 = (uVar26 - uVar22) + 10000000000000000000;
      if (uVar22 <= uVar26) {
        uVar27 = uVar26 - uVar22;
      }
      param_3[lVar21] = uVar27;
      if (uVar22 > uVar26) {
        do {
          do {
            uVar30 = uVar30 - 1;
          } while (param_6 < 1);
          uVar22 = 0;
          lVar18 = 0;
          do {
            lVar24 = lVar18 + 1;
            uVar29 = puVar17[lVar18];
            uVar27 = uVar29 + uVar22 + param_5[lVar18];
            uVar26 = uVar27 + 0x7538dcfb76180000;
            uVar22 = (ulong)(uVar26 <= uVar29);
            uVar28 = uVar26;
            if (uVar29 < uVar26) {
              uVar28 = uVar27;
            }
            puVar17[lVar18] = uVar28;
            lVar18 = lVar24;
          } while (param_6 != lVar24);
        } while ((uVar29 < uVar26) ||
                (uVar22 = param_3[lVar21], param_3[lVar21] = uVar22 + 1,
                uVar22 + 1 != 10000000000000000000));
      }
      puVar17 = puVar17 + -1;
      *(ulong *)(param_2 + lVar31 * 8) = uVar30;
      bVar15 = 0 < lVar31;
      lVar31 = lVar31 + -1;
    } while (bVar15);
  }
  if (((local_118 != 1) &&
      (FUN_001df2cc(param_3,param_3,param_6,local_118,0), param_5 != auStack_f0)) &&
     (param_5 != (ulong *)0x0)) {
    (*(code *)param_1[1])(*param_1,param_5,0);
  }
  uVar16 = 0;
LAB_001e57cc:
  if (*(long *)(lVar14 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar16);
  }
  return;
}

/* ===== FUN_001e5870 @ 001e5870 [libNexusScriptRuntime69252.so] ===== */

undefined8
FUN_001e5870(undefined8 *param_1,undefined8 *param_2,undefined8 *param_3,undefined8 param_4,
            uint param_5,uint param_6)

{
  bool bVar1;
  undefined8 *puVar2;
  uint uVar3;
  undefined4 uVar4;
  char cVar5;
  char cVar6;
  long lVar7;
  undefined1 auVar8 [16];
  undefined1 auVar9 [16];
  uint uVar10;
  int iVar11;
  undefined8 uVar12;
  long lVar13;
  long *plVar14;
  void *__src;
  ulong uVar15;
  long lVar16;
  long lVar17;
  ulong *puVar18;
  ulong uVar19;
  long lVar20;
  ulong uVar21;
  long lVar22;
  ulong uVar23;
  long lVar24;
  ulong *puVar25;
  ulong uVar26;
  long *plVar27;
  ulong uVar28;
  long lVar29;
  ulong uVar30;
  long lVar31;
  long lVar32;
  undefined8 *puVar33;
  
  puVar33 = (undefined8 *)*param_1;
  param_6 = *(uint *)(param_3 + 1) ^ param_6;
  uVar3 = *(uint *)(param_2 + 1);
  if (param_2[2] == param_3[2]) {
    uVar15 = param_2[3];
    uVar19 = param_3[3];
    uVar21 = uVar19;
    uVar23 = uVar15;
    uVar26 = uVar15;
    if ((long)uVar15 <= (long)uVar19) {
      uVar26 = uVar19;
    }
    do {
      uVar23 = uVar23 - 1;
      uVar21 = uVar21 - 1;
      if ((long)(uVar26 - 1) < 0) {
        iVar11 = 0;
        goto LAB_001e5948;
      }
      if (uVar23 < uVar15) {
        uVar28 = *(ulong *)(param_2[4] + uVar23 * 8);
        if (uVar19 <= uVar21) goto LAB_001e5924;
LAB_001e58dc:
        uVar30 = *(ulong *)(param_3[4] + uVar21 * 8);
      }
      else {
        uVar28 = 0;
        if (uVar21 < uVar19) goto LAB_001e58dc;
LAB_001e5924:
        uVar30 = 0;
      }
      uVar26 = uVar26 - 1;
    } while (uVar28 == uVar30);
    iVar11 = 1;
    if (uVar28 < uVar30) {
      iVar11 = -1;
    }
  }
  else {
    iVar11 = 1;
    if ((long)param_2[2] < (long)param_3[2]) {
      iVar11 = -1;
    }
  }
LAB_001e5948:
  puVar2 = param_3;
  uVar10 = param_6;
  if (-1 < iVar11) {
    puVar2 = param_2;
    uVar10 = uVar3;
    param_2 = param_3;
  }
  if (((iVar11 == 0) && (uVar3 != param_6)) && ((long)puVar2[2] < 0x7ffffffffffffffe)) {
    if (param_1[3] != 0) {
      uVar12 = (*(code *)puVar33[1])(*puVar33,param_1[4],0);
      param_1[3] = 0;
      param_1[4] = uVar12;
    }
    *(uint *)(param_1 + 1) = (uint)((param_5 & 7) == 2);
    param_1[2] = 0x8000000000000000;
    return 0;
  }
  lVar16 = puVar2[3];
  if ((lVar16 == 0) || (param_2[3] == 0)) {
    if (0x7ffffffffffffffd < (long)puVar2[2]) {
      if (puVar2[2] == 0x7fffffffffffffff) {
        if (param_1[3] != 0) {
          uVar12 = (*(code *)puVar33[1])(*puVar33,param_1[4],0);
          param_1[3] = 0;
          param_1[4] = uVar12;
        }
        uVar12 = 0;
      }
      else {
        if ((param_2[2] != 0x7ffffffffffffffe) || (uVar3 == param_6)) {
          if (param_1[3] != 0) {
            uVar12 = (*(code *)puVar33[1])(*puVar33,param_1[4],0);
            param_1[3] = 0;
            param_1[4] = uVar12;
          }
          *(uint *)(param_1 + 1) = uVar10;
          param_1[2] = 0x7ffffffffffffffe;
          return 0;
        }
        if (param_1[3] != 0) {
          uVar12 = (*(code *)puVar33[1])(*puVar33,param_1[4],0);
          param_1[3] = 0;
          param_1[4] = uVar12;
        }
        uVar12 = 1;
      }
      goto LAB_001e5e7c;
    }
    if (puVar2 != param_1) {
      lVar16 = puVar2[3];
      if (param_1[3] != lVar16) {
        lVar22 = (*(code *)puVar33[1])(*puVar33,param_1[4],lVar16 << 3);
        if ((lVar16 != 0) && (lVar22 == 0)) goto LAB_001e5e54;
        param_1[3] = lVar16;
        param_1[4] = lVar22;
      }
      lVar16 = puVar2[3];
      uVar4 = *(undefined4 *)(puVar2 + 1);
      __src = (void *)puVar2[4];
      param_1[2] = puVar2[2];
      *(undefined4 *)(param_1 + 1) = uVar4;
      memcpy((void *)param_1[4],__src,lVar16 << 3);
    }
    *(uint *)(param_1 + 1) = uVar10;
    goto LAB_001e5ee0;
  }
  lVar32 = puVar2[2] - param_2[2];
  lVar22 = SUB168(SEXT816(lVar32 + 0x12) * SEXT816(0xd79435e50d79436),8);
  lVar22 = lVar22 - (lVar22 >> 0x3f);
  lVar13 = lVar22 + param_2[3];
  if (lVar16 <= lVar13) {
    lVar16 = lVar13;
  }
  if (param_1[3] != lVar16) {
    lVar13 = (*(code *)puVar33[1])(*puVar33,param_1[4],lVar16 << 3);
    if (lVar16 == 0 || lVar13 != 0) {
      param_1[3] = lVar16;
      param_1[4] = lVar13;
      goto LAB_001e5a30;
    }
LAB_001e5e54:
    if (param_1[3] != 0) {
      uVar12 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,param_1[4],0);
      param_1[3] = 0;
      param_1[4] = uVar12;
    }
    uVar12 = 0x20;
LAB_001e5e7c:
    param_1[2] = 0x7fffffffffffffff;
    *(undefined4 *)(param_1 + 1) = 0;
    return uVar12;
  }
LAB_001e5a30:
  uVar12 = puVar2[2];
  lVar13 = puVar2[3];
  *(uint *)(param_1 + 1) = uVar10;
  param_1[2] = uVar12;
  lVar17 = lVar16 - lVar13;
  if (0 < lVar17) {
    memset((void *)param_1[4],0,lVar17 * 8);
  }
  if (puVar2[3] != 0) {
    lVar17 = param_1[4];
    uVar21 = 0;
    lVar20 = puVar2[4];
    do {
      lVar24 = uVar21 * 8;
      uVar21 = uVar21 + 1;
      *(undefined8 *)(lVar17 + lVar13 * -8 + lVar16 * 8 + lVar24) = *(undefined8 *)(lVar20 + lVar24)
      ;
    } while (uVar21 < (ulong)puVar2[3]);
  }
  lVar13 = param_2[3];
  lVar17 = SUB168(SEXT816(lVar32) * SEXT816(0xd79435e50d79436),8);
  uVar23 = lVar32 + (lVar17 - (lVar17 >> 0x3f)) * -0x13;
  uVar21 = uVar23 & 0xffffffff;
  if (uVar21 == 0) {
    plVar14 = (long *)param_2[4];
  }
  else {
    lVar13 = lVar13 + 1;
    plVar14 = (long *)(*(code *)puVar33[1])(*puVar33,0,lVar13 * 8);
    if (plVar14 == (long *)0x0) goto LAB_001e5e54;
    if (0x11 < uVar23 - 1) {
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
                ,0x173e,"limb_t mp_shr_dec(limb_t *, const limb_t *, mp_size_t, limb_t, limb_t)",
                "shift >= 1 && shift < LIMB_DIGITS");
    }
    if ((long)param_2[3] < 1) {
      lVar32 = 0;
    }
    else {
      lVar31 = param_2[4];
      lVar24 = (&DAT_0011dda8)[uVar23];
      cVar5 = (&DAT_0011e008)[uVar23 * 0x10];
      cVar6 = (&DAT_0011e009)[uVar23 * 0x10];
      lVar29 = (&DAT_0011dda8)[0x13 - uVar23];
      uVar26 = (&DAT_0011e000)[uVar23 * 2];
      lVar17 = param_2[3];
      lVar20 = 0;
      do {
        lVar7 = lVar17 + -1;
        uVar19 = *(ulong *)(lVar31 + -8 + lVar17 * 8);
        auVar8._8_8_ = 0;
        auVar8._0_8_ = uVar19;
        auVar9._8_8_ = 0;
        auVar9._0_8_ = uVar26;
        lVar32 = SUB168(auVar8 * auVar9,8);
        uVar15 = (uVar19 - lVar32 >> ((long)cVar5 & 0x3fU)) + lVar32 >> ((long)cVar6 & 0x3fU);
        lVar32 = uVar19 - uVar15 * lVar24;
        plVar14[lVar17] = uVar15 + lVar20 * lVar29;
        bVar1 = 0 < lVar17;
        lVar17 = lVar7;
        lVar20 = lVar32;
      } while (lVar7 != 0 && bVar1);
    }
    *plVar14 = (&DAT_0011dda8)[0x13 - (int)uVar23] * lVar32;
  }
  lVar22 = lVar16 - (lVar22 + param_2[3]);
  puVar18 = (ulong *)(param_1[4] + lVar22 * 8);
  if (uVar3 == param_6) {
    if (0 < lVar13) {
      uVar23 = 0;
      puVar25 = puVar18;
      plVar27 = plVar14;
      lVar32 = lVar13;
      do {
        uVar19 = *puVar25;
        uVar23 = uVar19 + uVar23 + *plVar27;
        uVar26 = uVar23 + 0x7538dcfb76180000;
        uVar15 = uVar26;
        if (uVar19 < uVar26) {
          uVar15 = uVar23;
        }
        uVar23 = (ulong)(uVar26 <= uVar19);
        lVar32 = lVar32 + -1;
        *puVar25 = uVar15;
        puVar25 = puVar25 + 1;
        plVar27 = plVar27 + 1;
      } while (lVar32 != 0);
      uVar23 = (ulong)(uVar26 <= uVar19);
      if (uVar23 != 0) {
        lVar22 = lVar16 - (lVar13 + lVar22);
        if (0 < lVar22) {
          puVar18 = puVar18 + lVar13;
          do {
            uVar15 = *puVar18;
            uVar26 = uVar15 + uVar23 + 0x7538dcfb76180000;
            uVar23 = uVar15 + uVar23;
            if (uVar26 <= uVar15) {
              uVar23 = uVar26;
            }
            *puVar18 = uVar23;
            if (uVar15 < uVar26) goto LAB_001e5ec0;
            lVar22 = lVar22 + -1;
            puVar18 = puVar18 + 1;
            uVar23 = 1;
          } while (lVar22 != 0);
        }
        iVar11 = FUN_001e06d0(param_1,lVar16 + 1);
        if (iVar11 != 0) {
          if ((uVar21 != 0) && (plVar14 != (long *)0x0)) {
            (*(code *)puVar33[1])(*puVar33,plVar14,0);
          }
          goto LAB_001e5e54;
        }
        *(undefined8 *)(param_1[4] + lVar16 * 8) = 1;
        param_1[2] = param_1[2] + 0x13;
      }
    }
  }
  else if (0 < lVar13) {
    uVar23 = 0;
    puVar25 = puVar18;
    plVar27 = plVar14;
    lVar32 = lVar13;
    do {
      uVar19 = *puVar25;
      uVar26 = *plVar27 + uVar23;
      uVar15 = (uVar19 - uVar26) + 10000000000000000000;
      if (uVar19 >= uVar26) {
        uVar15 = uVar19 - uVar26;
      }
      uVar23 = (ulong)(uVar19 < uVar26);
      lVar32 = lVar32 + -1;
      *puVar25 = uVar15;
      puVar25 = puVar25 + 1;
      plVar27 = plVar27 + 1;
    } while (lVar32 != 0);
    uVar23 = (ulong)(uVar19 < uVar26);
    if (uVar23 != 0) {
      lVar16 = lVar16 - (lVar13 + lVar22);
      if (0 < lVar16) {
        puVar18 = puVar18 + lVar13;
        do {
          uVar15 = *puVar18;
          uVar26 = (uVar15 - uVar23) + 10000000000000000000;
          if (uVar23 <= uVar15) {
            uVar26 = uVar15 - uVar23;
          }
          *puVar18 = uVar26;
          if (uVar23 <= uVar15) goto LAB_001e5ec0;
          lVar16 = lVar16 + -1;
          puVar18 = puVar18 + 1;
          uVar23 = 1;
        } while (lVar16 != 0);
      }
                    /* WARNING: Subroutine does not return */
      __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
                ,0x1a24,
                "int bfdec_add_internal(bfdec_t *, const bfdec_t *, const bfdec_t *, limb_t, bf_flags_t, int)"
                ,"carry == 0");
    }
  }
LAB_001e5ec0:
  if ((uVar21 != 0) && (plVar14 != (long *)0x0)) {
    (*(code *)puVar33[1])(*puVar33,plVar14,0);
  }
LAB_001e5ee0:
  uVar12 = FUN_001e030c(param_1,param_4,param_5);
  return uVar12;
}

/* ===== FUN_001e6944 @ 001e6944 [libNexusScriptRuntime69252.so] ===== */

undefined4
FUN_001e6944(undefined8 *param_1,long param_2,int param_3,int param_4,long param_5,ulong param_6,
            uint param_7,long param_8)

{
  undefined8 *puVar1;
  undefined1 auVar2 [16];
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  undefined1 auVar6 [16];
  undefined1 auVar7 [16];
  int iVar8;
  long lVar9;
  undefined8 *puVar10;
  long lVar11;
  ulong uVar12;
  long lVar13;
  undefined8 *puVar14;
  long lVar15;
  ulong uVar16;
  undefined8 *puVar17;
  undefined8 uVar18;
  ulong uVar19;
  undefined4 uVar20;
  ulong uVar21;
  ulong uVar22;
  ulong uVar23;
  ulong uVar24;
  ulong uVar25;
  long local_98;
  
  uVar21 = (ulong)param_7;
  lVar9 = (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,0,param_5 * 8);
  if (lVar9 != 0) {
    if (param_4 == 0) {
      iVar8 = FUN_001e6c6c(param_1,param_2,param_2,lVar9,param_3,uVar21,param_8);
      if (iVar8 == 0) {
        uVar20 = 0;
        goto LAB_001e6bf8;
      }
    }
    else {
      puVar10 = (undefined8 *)
                (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,0,param_5 << 7);
      if (puVar10 != (undefined8 *)0x0) {
        if ((param_6 & 0xf) != 0) {
                    /* WARNING: Subroutine does not return */
          __assert2("D:/all_prodject/bsd/test_bsd-nexus-brawl/framework-69252/lab/script-port-v69/release-candidate/source/vendor/quickjs/libbf.c"
                    ,0x1ee8,
                    "int ntt_fft_partial(BFNTTState *, NTTLimb *, int, int, limb_t, limb_t, int, limb_t)"
                    ,"(n2 % strip_len) == 0");
        }
        if (param_6 != 0) {
          uVar19 = 0;
          uVar25 = 1;
          uVar24 = (&DAT_0011e140)[param_8];
          uVar23 = param_1[param_8 + 1];
          uVar22 = param_1[param_8 * 0x68 + uVar21 * 0x34 + (long)(param_4 + param_3) + 6];
          local_98 = param_2;
          do {
            if (param_5 != 0) {
              lVar11 = 0;
              lVar13 = local_98;
              puVar14 = puVar10;
              do {
                lVar15 = 0;
                puVar17 = puVar14;
                do {
                  puVar1 = (undefined8 *)(lVar13 + lVar15);
                  lVar15 = lVar15 + 8;
                  *puVar17 = *puVar1;
                  puVar17 = puVar17 + param_5;
                } while (lVar15 != 0x80);
                lVar11 = lVar11 + 1;
                puVar14 = puVar14 + 1;
                lVar13 = lVar13 + param_6 * 8;
              } while (lVar11 != param_5);
            }
            lVar11 = 0x10;
            puVar14 = puVar10;
            do {
              if (param_7 != 0) {
                FUN_001e6e38(puVar14,param_5,uVar25,uVar24,uVar23);
              }
              iVar8 = FUN_001e6c6c(param_1,puVar14,puVar14,lVar9,param_3,uVar21,param_8);
              if (iVar8 != 0) {
                if (puVar10 != (undefined8 *)0x0) {
                  (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,puVar10,0);
                }
                goto LAB_001e6bf0;
              }
              if (param_7 == 0) {
                FUN_001e6e38(puVar14,param_5,uVar25,uVar24,uVar23);
              }
              uVar12 = uVar25 * uVar22;
              auVar2._8_8_ = 0;
              auVar2._0_8_ = uVar25;
              auVar5._8_8_ = 0;
              auVar5._0_8_ = uVar22;
              lVar15 = SUB168(auVar2 * auVar5,8);
              puVar14 = puVar14 + param_5;
              auVar3._8_8_ = 0;
              auVar3._0_8_ = uVar12 >> 0x3d | lVar15 << 3;
              auVar6._8_8_ = 0;
              auVar6._0_8_ = uVar23;
              uVar25 = SUB168(auVar3 * auVar6,8);
              uVar16 = uVar25 * uVar24;
              auVar4._8_8_ = 0;
              auVar4._0_8_ = uVar25;
              auVar7._8_8_ = 0;
              auVar7._0_8_ = uVar24;
              lVar13 = SUB168(auVar4 * auVar7,8);
              uVar25 = uVar16 + uVar24 * 2;
              if (CARRY8(uVar16,uVar24 * 2)) {
                lVar13 = lVar13 + 1;
              }
              uVar16 = uVar12 - uVar25;
              uVar25 = lVar15 - (lVar13 + (ulong)(uVar12 < uVar25));
              uVar12 = uVar24 & (long)uVar25 >> 1;
              if (CARRY8(uVar16,uVar12)) {
                uVar25 = uVar25 + 1;
              }
              lVar11 = lVar11 + -1;
              uVar25 = (uVar24 & uVar25) + uVar16 + uVar12;
            } while (lVar11 != 0);
            if (param_5 != 0) {
              lVar11 = 0;
              lVar13 = local_98;
              puVar14 = puVar10;
              do {
                lVar15 = 0;
                puVar17 = puVar14;
                do {
                  uVar18 = *puVar17;
                  puVar17 = puVar17 + param_5;
                  *(undefined8 *)(lVar13 + lVar15) = uVar18;
                  lVar15 = lVar15 + 8;
                } while (lVar15 != 0x80);
                lVar11 = lVar11 + 1;
                puVar14 = puVar14 + 1;
                lVar13 = lVar13 + param_6 * 8;
              } while (lVar11 != param_5);
            }
            uVar19 = uVar19 + 0x10;
            local_98 = local_98 + 0x80;
          } while (uVar19 < param_6);
        }
        (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,puVar10,0);
        uVar20 = 0;
        goto LAB_001e6bf8;
      }
    }
  }
LAB_001e6bf0:
  uVar20 = 0xffffffff;
  if (lVar9 == 0) {
    return 0xffffffff;
  }
LAB_001e6bf8:
  (*(code *)((undefined8 *)*param_1)[1])(*(undefined8 *)*param_1,lVar9,0);
  return uVar20;
}

