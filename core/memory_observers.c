/*
 * memory_observers — Core plumbing
 * Decompiled with Ghidra 11.3.2 (arm64 pseudocode) from: libNexusEvasion69252.so, libNexusEvasionRuntime69252.so
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
 * Notes: ng_* observer bind/feed pipeline reading game state.
 */

/* ===== JNI_OnUnload @ 0011d0e8 [libNexusEvasion69252.so] ===== */

void JNI_OnUnload(void)

{
  char cVar1;
  bool bVar2;
  int iVar3;
  
  do {
    iVar3 = DAT_00129128;
    cVar1 = '\x01';
    bVar2 = (bool)ExclusiveMonitorPass(0x129128,0x10);
    if (bVar2) {
      DAT_00129128 = -1;
      cVar1 = ExclusiveMonitorsStatus();
    }
  } while (cVar1 != '\0');
  if (-1 < iVar3) {
    close(iVar3);
  }
  do {
    iVar3 = DAT_0012912c;
    cVar1 = '\x01';
    bVar2 = (bool)ExclusiveMonitorPass(0x12912c,0x10);
    if (bVar2) {
      DAT_0012912c = -1;
      cVar1 = ExclusiveMonitorsStatus();
    }
  } while (cVar1 != '\0');
  if (-1 < iVar3) {
    iVar3 = close(iVar3);
  }
  nexus_evasion_unbind(iVar3);
  return;
}

/* ===== nexus_evasion_unbind @ 0011d580 [libNexusEvasion69252.so] ===== */

void nexus_evasion_unbind(void)

{
  (*(code *)PTR_nexus_evasion_unbind_00124f80)();
  return;
}

/* ===== FUN_00144160 @ 00144160 [libNexusEvasionRuntime69252.so] ===== */

void * FUN_00144160(void *param_1)

{
  void *pvVar1;
  ulong uVar2;
  char cVar3;
  byte bVar4;
  long lVar5;
  bool bVar6;
  undefined8 uVar7;
  undefined *puVar8;
  void *pvVar9;
  bool bVar10;
  int iVar11;
  uint uVar12;
  int iVar13;
  undefined8 *puVar14;
  undefined4 *puVar15;
  undefined4 *puVar16;
  void *pvVar17;
  undefined4 *puVar18;
  int *piVar19;
  FILE *__stream;
  int *piVar20;
  char *pcVar21;
  size_t sVar22;
  void *__addr;
  uint uVar23;
  ulong uVar24;
  ulong uVar25;
  ulong uVar26;
  long lVar27;
  long lVar28;
  void *pvVar29;
  ulong uVar30;
  void *pvVar31;
  char *local_2300;
  void *local_22f8;
  long local_22f0;
  void *local_22b8;
  void *pvStack_22b0;
  ulong local_22a8;
  ulong uStack_22a0;
  undefined8 auStack_2298 [64];
  ulong local_2098;
  long local_2090;
  uint local_2088;
  undefined4 local_2084;
  undefined8 local_2080;
  void *local_2078;
  undefined8 local_2070;
  undefined8 uStack_2068;
  undefined8 uStack_2060;
  ulong local_2058;
  long local_80;
  
  lVar5 = tpidr_el0;
  local_80 = *(long *)(lVar5 + 0x28);
  uVar30 = -DAT_00209d88 & (ulong)param_1;
  puVar14 = (undefined8 *)FUN_001bd828(&DAT_001cfc98);
  *(undefined4 *)(puVar14 + 1) = 0;
  *puVar14 = &DAT_0011e39a;
  puVar15 = (undefined4 *)FUN_001bd828(&DAT_001cfc78);
  puVar16 = (undefined4 *)FUN_001bd828(&DAT_001cfc58);
  uVar7 = DAT_0010e580;
  lVar28 = (long)PTR_DAT_001cb6c0 - (long)PTR_DAT_001cb6b0;
  uVar26 = 0x100000;
  do {
    pvVar29 = (void *)0x0;
    if (uVar26 <= uVar30) {
      pvVar29 = (void *)(uVar30 - uVar26);
    }
    if ((void *)(uVar30 + uVar26) != (void *)0x0) {
      pvVar17 = mmap((void *)(uVar30 + uVar26),DAT_00209d88,3,0x22,-1,0);
      if (pvVar17 == (void *)0xffffffffffffffff) {
        puVar18 = (undefined4 *)__errno();
        *(undefined4 *)(puVar14 + 1) = *puVar18;
        *puVar14 = &DAT_0011e39a;
        goto joined_r0x00144330;
      }
      local_2080 = uVar7;
      uStack_2068 = 0;
      local_2070 = 0;
      local_2058 = 0;
      uStack_2060 = 0;
      *(undefined4 *)(puVar14 + 1) = 0;
      *puVar14 = "prepare";
      *puVar15 = 0;
      *puVar16 = 1;
      local_2078 = param_1;
      iVar11 = ng_prepare_observer_v1(pvVar17,pvVar17,DAT_00209d88,&local_2080);
      *puVar16 = 0;
      if (iVar11 != 0) {
        *(code **)((long)pvVar17 + lVar28) = FUN_001455a4;
        FUN_001bdaf0(pvVar17,(long)pvVar17 + (local_2058 & 0xffffffff));
        uVar12 = mprotect(pvVar17,DAT_00209d88,5);
        piVar19 = (int *)(ulong)uVar12;
        if (uVar12 == 0) goto LAB_00144e98;
        puVar18 = (undefined4 *)__errno();
        *(undefined4 *)(puVar14 + 1) = *puVar18;
        *puVar14 = &DAT_0011a7ce;
      }
      iVar11 = munmap(pvVar17,DAT_00209d88);
      if (iVar11 == 0) goto joined_r0x00144330;
LAB_00144578:
      piVar19 = (int *)__errno();
      iVar11 = *piVar19;
      pvVar17 = (void *)0x0;
      *puVar14 = "gap_unmap";
      *(int *)(puVar14 + 1) = iVar11;
      goto LAB_00144ea4;
    }
joined_r0x00144330:
    if (uVar26 < uVar30) {
      pvVar17 = mmap(pvVar29,DAT_00209d88,3,0x22,-1,0);
      if (pvVar17 == (void *)0xffffffffffffffff) {
        puVar18 = (undefined4 *)__errno();
        *(undefined4 *)(puVar14 + 1) = *puVar18;
        *puVar14 = &DAT_0011e39a;
      }
      else {
        local_2080 = uVar7;
        uStack_2068 = 0;
        local_2070 = 0;
        local_2058 = 0;
        uStack_2060 = 0;
        *(undefined4 *)(puVar14 + 1) = 0;
        *puVar14 = "prepare";
        *puVar15 = 0;
        *puVar16 = 1;
        local_2078 = param_1;
        iVar11 = ng_prepare_observer_v1(pvVar17,pvVar17,DAT_00209d88,&local_2080);
        *puVar16 = 0;
        if (iVar11 != 0) {
          *(code **)((long)pvVar17 + lVar28) = FUN_001455a4;
          FUN_001bdaf0(pvVar17,(long)pvVar17 + (local_2058 & 0xffffffff));
          uVar12 = mprotect(pvVar17,DAT_00209d88,5);
          piVar19 = (int *)(ulong)uVar12;
          if (uVar12 == 0) goto LAB_00144e98;
          puVar18 = (undefined4 *)__errno();
          *(undefined4 *)(puVar14 + 1) = *puVar18;
          *puVar14 = &DAT_0011a7ce;
        }
        iVar11 = munmap(pvVar17,DAT_00209d88);
        if (iVar11 != 0) goto LAB_00144578;
      }
    }
    uVar25 = DAT_00209d88;
    pvVar29 = DAT_001e0978;
    bVar10 = uVar26 < 0x6f00001;
    uVar26 = uVar26 + 0x100000;
  } while (bVar10);
  piVar19 = (int *)memset(auStack_2298,0,0x218);
  puVar8 = PTR_DAT_001cb6b0;
  if ((((((ulong)((long)param_1 - (long)pvVar29) >> 0x16 < 5) && (pvVar29 <= param_1)) &&
       (pvVar29 < (void *)0xfffffffffec00000)) &&
      ((uVar25 == 0x1000 || uVar25 == 0x4000 && (((ulong)param_1 & 3) == 0)))) &&
     ((3 < (ulong)((long)PTR_DAT_001cb6b8 - (long)PTR_DAT_001cb6b0) &&
      (((ulong)((long)PTR_DAT_001cb6b8 - (long)PTR_DAT_001cb6b0) <= uVar25 - 4 &&
       (((int)PTR_DAT_001cb6b8 - (int)PTR_DAT_001cb6b0 & 3U) == 0)))))) {
    local_2090 = (long)PTR_DAT_001cb6b8 - (long)PTR_DAT_001cb6b0;
    local_2098 = uVar25;
    uVar26 = (long)param_1 - 0x8000000;
    local_22b8 = pvVar29;
    if (uVar26 < 0x10001) {
      uVar26 = 0x10000;
    }
    if ((ulong)param_1 >> 0x1b == 0) {
      uVar26 = 0x10000;
    }
    uVar30 = 4;
    if (3 < local_2090 - 4U) {
      uVar30 = local_2090 - 4U;
    }
    uStack_22a0 = (long)param_1 + (0x8000000 - uVar30);
    if ((void *)~(0x8000000 - uVar30) < param_1) {
      uStack_22a0 = 0xffffffffffffffff;
    }
    if (~uVar25 <= uStack_22a0) {
      uStack_22a0 = ~uVar25;
    }
    local_22a8 = (uVar25 - 1) + uVar26 & -uVar25;
    uStack_22a0 = uStack_22a0 & -uVar25;
    pvStack_22b0 = param_1;
    if (local_22a8 <= uStack_22a0) {
      __stream = fopen("/proc/self/maps","r");
      if (__stream != (FILE *)0x0) {
LAB_001445b4:
        piVar20 = (int *)__errno();
        uVar12 = 0;
        local_22f8 = (void *)0x0;
        local_22f0 = 0;
        bVar10 = false;
LAB_001445e0:
        bVar6 = bVar10;
        *piVar20 = 0;
        pcVar21 = fgets((char *)&local_2080,0x2000,__stream);
        if (pcVar21 != (char *)0x0) {
LAB_00144794:
          sVar22 = strlen((char *)&local_2080);
          if ((sVar22 != 0) && (*(char *)((long)&local_2084 + sVar22 + 3) == '\n')) {
            iVar11 = 0;
            uVar12 = uVar12 + 1;
            if (uVar12 < 0x8001) {
              pcVar21 = "gap_bounds";
              if (sVar22 <= 0x800000U - local_22f0) {
                lVar28 = 0;
                pvVar29 = (void *)0x0;
                local_22f0 = sVar22 + local_22f0;
                do {
                  bVar4 = *(byte *)((long)&local_2080 + lVar28);
                  uVar23 = (uint)bVar4;
                  if (bVar4 - 0x30 < 10) {
                    iVar11 = -0x30;
                  }
                  else {
                    iVar11 = -0x57;
                    if (5 < bVar4 - 0x61) {
                      if (5 < uVar23 - 0x41) goto LAB_00144848;
                      iVar11 = -0x37;
                    }
                  }
                  if ((ulong)pvVar29 >> 0x3c != 0) goto LAB_001449d4;
                  lVar28 = lVar28 + 1;
                  pvVar29 = (void *)((ulong)(iVar11 + uVar23) + (long)pvVar29 * 0x10);
                } while( true );
              }
              goto LAB_001449d8;
            }
          }
          iVar11 = 0;
          pcVar21 = "gap_bounds";
          goto LAB_001449d8;
        }
        iVar11 = *piVar20;
        iVar13 = ferror(__stream);
        if ((iVar13 == 0) || (iVar11 != 4)) {
LAB_00144ba4:
          if (iVar13 == 0) {
            iVar13 = feof(__stream);
            iVar11 = 0;
            if (iVar13 != 0) {
              pcVar21 = "gap_maps";
              if ((uVar12 == 0) || (!bVar6)) goto LAB_001449d8;
              FUN_0015b650(&local_22b8,local_22f8,0xffffffffffffffff);
              uVar12 = fclose(__stream);
              lVar28 = local_2090;
              uVar26 = local_2098;
              pvVar9 = pvStack_22b0;
              pvVar29 = local_22b8;
              piVar19 = (int *)(ulong)uVar12;
              if (uVar12 == 0) {
                *puVar14 = "gap_empty";
                *(undefined4 *)(puVar14 + 1) = 0;
                pvVar1 = (void *)((long)local_22b8 + 0x1400000);
                uVar24 = local_2098 - 1;
                uVar25 = (ulong)local_2084;
                pvVar31 = (void *)~local_2098;
                uVar30 = (long)pvStack_22b0 + 4;
                local_2300 = "gap_range";
                lVar27 = (long)PTR_DAT_001cb6c0 - (long)puVar8;
                goto LAB_00144c8c;
              }
              pvVar17 = (void *)0x0;
              *(int *)(puVar14 + 1) = *piVar20;
              *puVar14 = "gap_close";
              goto LAB_00144ea4;
            }
          }
        }
        else {
          clearerr(__stream);
          *piVar20 = 0;
          pcVar21 = fgets((char *)&local_2080,0x2000,__stream);
          if (pcVar21 != (char *)0x0) goto LAB_00144794;
          iVar11 = *piVar20;
          iVar13 = ferror(__stream);
          if ((iVar13 == 0) || (iVar11 != 4)) goto LAB_00144ba4;
          clearerr(__stream);
          *piVar20 = 0;
          pcVar21 = fgets((char *)&local_2080,0x2000,__stream);
          if (pcVar21 != (char *)0x0) goto LAB_00144794;
          iVar11 = *piVar20;
          iVar13 = ferror(__stream);
          if ((iVar13 == 0) || (iVar11 != 4)) goto LAB_00144ba4;
          clearerr(__stream);
          *piVar20 = 0;
          pcVar21 = fgets((char *)&local_2080,0x2000,__stream);
          if (pcVar21 != (char *)0x0) goto LAB_00144794;
          iVar11 = *piVar20;
          iVar13 = ferror(__stream);
          if ((iVar13 == 0) || (iVar11 != 4)) goto LAB_00144ba4;
          clearerr(__stream);
          *piVar20 = 0;
          pcVar21 = fgets((char *)&local_2080,0x2000,__stream);
          if (pcVar21 != (char *)0x0) goto LAB_00144794;
          iVar11 = *piVar20;
          iVar13 = ferror(__stream);
          if ((iVar13 == 0) || (iVar11 != 4)) goto LAB_00144ba4;
          clearerr(__stream);
          *piVar20 = 0;
          pcVar21 = fgets((char *)&local_2080,0x2000,__stream);
          if (pcVar21 != (char *)0x0) goto LAB_00144794;
          iVar11 = *piVar20;
          iVar13 = ferror(__stream);
          if ((iVar13 == 0) || (iVar11 != 4)) goto LAB_00144ba4;
          clearerr(__stream);
          *piVar20 = 0;
          pcVar21 = fgets((char *)&local_2080,0x2000,__stream);
          if (pcVar21 != (char *)0x0) goto LAB_00144794;
          iVar11 = *piVar20;
          iVar13 = ferror(__stream);
          if ((iVar13 == 0) || (iVar11 != 4)) goto LAB_00144ba4;
          clearerr(__stream);
          *piVar20 = 0;
          pcVar21 = fgets((char *)&local_2080,0x2000,__stream);
          if (pcVar21 != (char *)0x0) goto LAB_00144794;
          iVar11 = *piVar20;
          iVar13 = ferror(__stream);
          if ((iVar13 == 0) || (iVar11 != 4)) goto LAB_00144ba4;
        }
        pcVar21 = "gap_read";
        goto LAB_001449d8;
      }
      piVar19 = (int *)__errno();
      iVar11 = *piVar19;
      if (iVar11 == 4) {
        __stream = fopen("/proc/self/maps","r");
        if (__stream != (FILE *)0x0) goto LAB_001445b4;
        piVar19 = (int *)__errno();
        iVar11 = *piVar19;
        if (iVar11 == 4) {
          __stream = fopen("/proc/self/maps","r");
          if (__stream != (FILE *)0x0) goto LAB_001445b4;
          piVar19 = (int *)__errno();
          iVar11 = *piVar19;
          if (iVar11 == 4) {
            __stream = fopen("/proc/self/maps","r");
            if (__stream != (FILE *)0x0) goto LAB_001445b4;
            piVar19 = (int *)__errno();
            iVar11 = *piVar19;
            if (iVar11 == 4) {
              __stream = fopen("/proc/self/maps","r");
              if (__stream != (FILE *)0x0) goto LAB_001445b4;
              piVar19 = (int *)__errno();
              iVar11 = *piVar19;
              if (iVar11 == 4) {
                __stream = fopen("/proc/self/maps","r");
                if (__stream != (FILE *)0x0) goto LAB_001445b4;
                piVar19 = (int *)__errno();
                iVar11 = *piVar19;
                if (iVar11 == 4) {
                  __stream = fopen("/proc/self/maps","r");
                  if (__stream != (FILE *)0x0) goto LAB_001445b4;
                  piVar19 = (int *)__errno();
                  iVar11 = *piVar19;
                  if (iVar11 == 4) {
                    __stream = fopen("/proc/self/maps","r");
                    if (__stream != (FILE *)0x0) goto LAB_001445b4;
                    piVar19 = (int *)__errno();
                    iVar11 = *piVar19;
                  }
                }
              }
            }
          }
        }
      }
      *(int *)(puVar14 + 1) = iVar11;
      pvVar17 = (void *)0x0;
      *puVar14 = "gap_open";
      goto LAB_00144ea4;
    }
  }
  pvVar17 = (void *)0x0;
  *(undefined4 *)(puVar14 + 1) = 0;
  *puVar14 = "gap_window";
LAB_00144ea4:
  if (*(long *)(lVar5 + 0x28) != local_80) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(piVar19);
  }
  return pvVar17;
LAB_00144848:
  iVar11 = 0;
  if ((int)lVar28 != 0) {
    pcVar21 = "gap_parse";
    if (uVar23 == 0x2d) {
      lVar27 = 0;
      pvVar17 = (void *)0x0;
      do {
        bVar4 = *(byte *)((long)&local_2080 + lVar27 + lVar28 + 1);
        uVar23 = (uint)bVar4;
        if (bVar4 - 0x30 < 10) {
          iVar11 = -0x30;
        }
        else {
          iVar11 = -0x57;
          if (5 < bVar4 - 0x61) {
            if (5 < uVar23 - 0x41) goto LAB_001448bc;
            iVar11 = -0x37;
          }
        }
        if ((ulong)pvVar17 >> 0x3c != 0) goto LAB_001449d4;
        lVar27 = lVar27 + 1;
        pvVar17 = (void *)((ulong)(iVar11 + uVar23) + (long)pvVar17 * 0x10);
      } while( true );
    }
    goto LAB_001449d8;
  }
LAB_001449d4:
  iVar11 = 0;
  pcVar21 = "gap_parse";
  goto LAB_001449d8;
LAB_001448bc:
  if ((int)lVar27 == 0) goto LAB_001449d4;
  if (uVar23 == 0x20) {
    pcVar21 = (char *)((long)&local_2080 + lVar28 + lVar27 + 2);
    sVar22 = strlen(pcVar21);
    if ((sVar22 < 5) || ((cVar3 = *pcVar21, cVar3 != 'r' && (cVar3 != '-')))) goto LAB_00144b54;
    cVar3 = *(char *)((long)&local_2080 + lVar28 + lVar27 + 3);
    if (((cVar3 == 'w') || (cVar3 == '-')) &&
       ((cVar3 = *(char *)((long)&local_2080 + lVar28 + lVar27 + 4), cVar3 == 'x' || (cVar3 == '-'))
       )) {
      cVar3 = *(char *)((long)&local_2080 + lVar28 + lVar27 + 5);
      if (((cVar3 == 's') || (cVar3 == 'p')) &&
         (*(char *)((long)&local_2080 + lVar28 + lVar27 + 6) == ' ')) {
        iVar11 = 0;
        pcVar21 = "gap_parse";
        if ((pvVar17 <= pvVar29) || (pvVar29 < local_22f8)) goto LAB_001449d8;
        if ((((ulong)pvVar17 | (ulong)pvVar29) & uVar25 - 1) != 0) goto LAB_00144b54;
        FUN_0015b650(&local_22b8,local_22f8,pvVar29);
        bVar10 = bVar6;
        if ((void *)((long)param_1 + 4U) <= pvVar17) {
          bVar10 = true;
        }
        local_22f8 = pvVar17;
        if (param_1 < pvVar29 || (void *)0xfffffffffffffffb < param_1) {
          bVar10 = bVar6;
        }
        goto LAB_001445e0;
      }
    }
    iVar11 = 0;
    pcVar21 = "gap_parse";
    goto LAB_001449d8;
  }
LAB_00144b54:
  iVar11 = 0;
  pcVar21 = "gap_parse";
LAB_001449d8:
  *puVar14 = pcVar21;
  *(int *)(puVar14 + 1) = iVar11;
  uVar12 = fclose(__stream);
  piVar19 = (int *)(ulong)uVar12;
  pvVar17 = (void *)0x0;
  goto LAB_00144ea4;
LAB_00144c8c:
  if (uVar25 < local_2088) {
    do {
      pvVar17 = (void *)auStack_2298[uVar25];
      if ((((pvVar17 < (void *)0x10000) || ((uVar24 & (ulong)pvVar17) != 0 || pvVar31 < pvVar17)) ||
          ((pvVar17 < pvVar1 && (pvVar29 < (void *)(uVar26 + (long)pvVar17))))) ||
         ((((uint)pvVar9 | (uint)pvVar17) & 3) != 0)) {
LAB_00144ee4:
        iVar11 = 0;
        goto LAB_00144ef0;
      }
      bVar10 = 0x8000000 < (ulong)((long)pvVar9 - (long)pvVar17);
      if (pvVar9 <= pvVar17) {
        bVar10 = ((long)pvVar17 - (long)pvVar9 & 0xfffffffff8000000U) != 0;
      }
      if ((bVar10) || (uVar2 = lVar28 + (long)pvVar17, (((uint)uVar2 | (uint)pvVar9) & 3) != 0))
      goto LAB_00144ee4;
      bVar10 = 0x8000000 < uVar2 - uVar30;
      if (uVar2 <= uVar30) {
        bVar10 = (uVar30 - uVar2 & 0xfffffffff8000000) != 0;
      }
      if (bVar10) goto LAB_00144ee4;
      __addr = mmap(pvVar17,uVar26,3,0x100022,-1,0);
      if (__addr == (void *)0xffffffffffffffff) {
        iVar11 = *piVar20;
        *puVar14 = "gap_mmap";
        *(int *)(puVar14 + 1) = iVar11;
        piVar19 = (int *)0xffffffffffffffff;
      }
      else {
        if (pvVar17 == __addr) goto LAB_00144db8;
        *(undefined4 *)(puVar14 + 1) = 0;
        *puVar14 = "gap_return";
        uVar12 = munmap(__addr,uVar26);
        piVar19 = (int *)(ulong)uVar12;
        if (uVar12 != 0) {
          iVar11 = *piVar20;
          local_2300 = "gap_unmap";
          goto LAB_00144ef0;
        }
      }
      pvVar17 = (void *)0x0;
      uVar25 = uVar25 + 1;
      if (local_2088 == uVar25) goto LAB_00144ea4;
    } while( true );
  }
  pvVar17 = (void *)0x0;
  goto LAB_00144ea4;
LAB_00144db8:
  local_2080 = uVar7;
  uStack_2068 = 0;
  local_2070 = 0;
  local_2058 = 0;
  uStack_2060 = 0;
  *(undefined4 *)(puVar14 + 1) = 0;
  *puVar14 = "prepare";
  *puVar15 = 0;
  *puVar16 = 1;
  local_2078 = param_1;
  iVar11 = ng_prepare_observer_v1(pvVar17,pvVar17,DAT_00209d88,&local_2080);
  *puVar16 = 0;
  if (iVar11 != 0) {
    *(code **)((long)pvVar17 + lVar27) = FUN_001455a4;
    FUN_001bdaf0(pvVar17,(long)pvVar17 + (local_2058 & 0xffffffff));
    uVar12 = mprotect(pvVar17,DAT_00209d88,5);
    piVar19 = (int *)(ulong)uVar12;
    if (uVar12 == 0) goto LAB_00144e98;
    iVar11 = *piVar20;
    *puVar14 = &DAT_0011a7ce;
    *(int *)(puVar14 + 1) = iVar11;
  }
  uVar12 = munmap(pvVar17,DAT_00209d88);
  piVar19 = (int *)(ulong)uVar12;
  uVar25 = uVar25 + 1 & 0xffffffff;
  if (uVar12 != 0) goto code_r0x00144e80;
  goto LAB_00144c8c;
LAB_00144e98:
  DAT_0020adb0 = uStack_2060._4_4_;
  goto LAB_00144ea4;
code_r0x00144e80:
  local_2300 = "gap_unmap";
  iVar11 = *piVar20;
LAB_00144ef0:
  pvVar17 = (void *)0x0;
  *puVar14 = local_2300;
  *(int *)(puVar14 + 1) = iVar11;
  goto LAB_00144ea4;
}

/* ===== FUN_00150434 @ 00150434 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00150434(undefined8 param_1,ulong param_2,int param_3,int param_4)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  int local_120;
  int iStack_11c;
  undefined8 local_118;
  undefined8 local_110;
  undefined8 uStack_108;
  undefined8 local_100;
  undefined8 uStack_f8;
  ulong local_f0;
  undefined8 uStack_e8;
  undefined8 local_e0;
  undefined8 uStack_d8;
  undefined8 uStack_d0;
  undefined8 uStack_c8;
  undefined8 local_c0;
  undefined8 uStack_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 local_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 local_50;
  long local_48;
  
  iVar4 = DAT_00209cd8;
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  bVar2 = false;
  uStack_58 = 0;
  local_60 = 0;
  uStack_68 = 0;
  local_70 = 0;
  uStack_78 = 0;
  local_80 = 0;
  uStack_88 = 0;
  local_90 = 0;
  uStack_98 = 0;
  local_a0 = 0;
  uStack_f8 = 0;
  local_100 = 0;
  uStack_e8 = 0;
  local_f0 = 0;
  uStack_d8 = 0;
  local_e0 = 0;
  uStack_c8 = 0;
  uStack_d0 = 0;
  uStack_b8 = 0;
  local_c0 = 0;
  uStack_a8 = 0;
  local_b0 = 0;
  uStack_108 = 0;
  local_110 = 0;
  local_50 = 0;
  local_118 = DAT_0010e808;
  if ((DAT_00209cd0 != 0) && ((int)DAT_001dfff0 != 0)) {
    iVar3 = gettid(0);
    if (iVar4 == iVar3) {
      iVar4 = ng_status_v1(&local_118);
      bVar2 = false;
      if ((((iVar4 != 0) && ((int)local_b0 != 0)) && (bVar2 = false, 0xfffe2b3e < param_4 - 0xea61U)
          ) && (((0xfffe2b3e < param_3 - 0xea61U && (param_2 < 0xfffffffffffff000)) &&
                (local_f0 == param_2)))) {
        local_120 = param_3;
        iStack_11c = param_4;
        lVar5 = FUN_0014bef8(DAT_001cfb4c,&local_120,8,param_2 + 0xfac);
        bVar2 = lVar5 == 8;
      }
    }
    else {
      bVar2 = false;
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar2);
}

/* ===== ng_observe_registers_v1 @ 0018d008 [libNexusEvasionRuntime69252.so] ===== */

void ng_observe_registers_v1(undefined8 *param_1)

{
  undefined4 uVar1;
  char cVar2;
  bool bVar3;
  long lVar4;
  undefined4 *puVar5;
  undefined8 local_60;
  long local_58;
  undefined8 local_50;
  undefined8 uStack_48;
  undefined8 local_40;
  long local_38;
  
  lVar4 = tpidr_el0;
  local_38 = *(long *)(lVar4 + 0x28);
  puVar5 = (undefined4 *)__errno();
  uVar1 = *puVar5;
  if (param_1 != (undefined8 *)0x0) {
    local_60 = DAT_0010e668;
    do {
      local_58 = DAT_002fe098 + 1;
      cVar2 = '\x01';
      bVar3 = (bool)ExclusiveMonitorPass(0x2fe098,0x10);
      if (bVar3) {
        cVar2 = ExclusiveMonitorsStatus();
        DAT_002fe098 = local_58;
      }
    } while (cVar2 != '\0');
    uStack_48 = param_1[1];
    local_50 = *param_1;
    local_40 = param_1[2];
    ng_feed_v1(&local_60);
  }
  *puVar5 = uVar1;
  if (*(long *)(lVar4 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== ng_observer_template_size @ 0018d0a8 [libNexusEvasionRuntime69252.so] ===== */

long ng_observer_template_size(void)

{
  return (long)PTR_FUN_001cb6d0 - (long)PTR_DAT_001cb6b0;
}

/* ===== ng_prepare_observer_v1 @ 0018d0c0 [libNexusEvasionRuntime69252.so] ===== */

void ng_prepare_observer_v1(ulong param_1,void *param_2,ulong param_3,int *param_4)

{
  ulong uVar1;
  long lVar2;
  undefined *puVar3;
  int iVar4;
  ulong uVar5;
  undefined8 uVar6;
  char *pcVar7;
  uint uVar8;
  long lVar9;
  uint uVar10;
  ulong __n;
  long lVar11;
  undefined8 local_140;
  undefined8 local_138;
  undefined8 uStack_130;
  undefined8 local_128;
  undefined8 uStack_120;
  undefined8 local_118;
  undefined8 uStack_110;
  undefined8 local_108;
  undefined8 uStack_100;
  undefined8 local_f8;
  undefined8 uStack_f0;
  undefined8 local_e8;
  undefined8 uStack_e0;
  undefined8 local_d8;
  undefined8 uStack_d0;
  undefined8 local_c8;
  undefined8 uStack_c0;
  undefined8 local_b8;
  undefined8 uStack_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  long local_70;
  
  lVar2 = tpidr_el0;
  local_70 = *(long *)(lVar2 + 0x28);
  local_78 = 0;
  uStack_80 = 0;
  local_88 = 0;
  uStack_90 = 0;
  local_98 = 0;
  uStack_a0 = 0;
  local_a8 = 0;
  uStack_b0 = 0;
  local_b8 = 0;
  uStack_c0 = 0;
  local_c8 = 0;
  uStack_d0 = 0;
  local_d8 = 0;
  uStack_e0 = 0;
  local_e8 = 0;
  uStack_f0 = 0;
  local_f8 = 0;
  uStack_100 = 0;
  local_108 = 0;
  uStack_110 = 0;
  local_118 = 0;
  uStack_120 = 0;
  local_128 = 0;
  uStack_130 = 0;
  local_138 = 0;
  local_140 = DAT_0010e808;
  if (((((param_4 == (int *)0x0) || (*param_4 != 1)) || (param_2 == (void *)0x0)) ||
      ((param_4[1] != 0x30 || (param_1 < 0x10000)))) || ((param_1 & 0xf) != 0)) {
    pcVar7 = "plan_contract";
  }
  else {
    iVar4 = ng_status_v1(&local_140);
    if (iVar4 == 0) {
      pcVar7 = "plan_status";
    }
    else if ((int)uStack_e0 == 0) {
      pcVar7 = "plan_unbound";
    }
    else {
      uVar5 = FUN_0018b930();
      puVar3 = PTR_DAT_001cb6b0;
      if ((uVar5 == 0) || (*(ulong *)(param_4 + 2) != uVar5)) {
        pcVar7 = "plan_current_entry";
      }
      else {
        __n = (long)PTR_FUN_001cb6d0 - (long)PTR_DAT_001cb6b0;
        if ((~__n < param_1) || (param_3 < __n)) {
          pcVar7 = "plan_capacity";
        }
        else {
          if ((((uint)uVar5 | (uint)param_1) & 3) == 0) {
            uVar1 = uVar5 - param_1;
            if (uVar5 < param_1 || uVar1 == 0) {
              if (param_1 - uVar5 >> 0x1b == 0) {
                uVar8 = (uint)(param_1 - uVar5 >> 2);
                goto LAB_0018d27c;
              }
            }
            else if (uVar1 < 0x8000001) {
              uVar8 = -((uint)uVar1 >> 2) & 0x3ffffff;
LAB_0018d27c:
              lVar11 = (long)PTR_DAT_001cb6b8 - (long)PTR_DAT_001cb6b0;
              uVar1 = param_1 + lVar11;
              if ((((uint)uVar5 | (uint)uVar1) & 3) == 0) {
                uVar5 = uVar5 + 4;
                if (uVar5 < uVar1) {
                  if (uVar1 - uVar5 < 0x8000001) {
                    uVar10 = -((uint)(uVar1 - uVar5) >> 2) & 0x3ffffff;
LAB_0018d2d8:
                    memcpy(param_2,PTR_DAT_001cb6b0,__n);
                    uVar6 = 1;
                    lVar9 = (long)PTR_DAT_001cb6c0 - (long)puVar3;
                    *(undefined4 *)((long)param_2 + ((long)PTR_DAT_001cb6c8 - (long)puVar3)) =
                         0xd10243ff;
                    puVar3 = PTR_ng_observe_registers_v1_001cb738;
                    *(uint *)((long)param_2 + lVar11) = uVar10 | 0x14000000;
                    *(undefined **)((long)param_2 + lVar9) = puVar3;
                    *(ulong *)(param_4 + 4) = param_1;
                    *(ulong *)(param_4 + 6) = uVar5;
                    param_4[8] = -0x2efdbc01;
                    param_4[9] = uVar8 | 0x14000000;
                    param_4[10] = (int)__n;
                    param_4[0xb] = 0;
                    goto LAB_0018d1ec;
                  }
                }
                else if (uVar5 - uVar1 >> 0x1b == 0) {
                  uVar10 = (uint)(uVar5 - uVar1 >> 2);
                  goto LAB_0018d2d8;
                }
              }
              pcVar7 = "resume";
              goto LAB_0018d1e4;
            }
          }
          pcVar7 = "enter";
        }
      }
    }
  }
LAB_0018d1e4:
  FUN_00140548(pcVar7);
  uVar6 = 0;
LAB_0018d1ec:
  if (*(long *)(lVar2 + 0x28) == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar6);
}

