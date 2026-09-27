/*
 * game_state_readers — Core plumbing
 * Decompiled with Ghidra 11.3.2 (arm64 pseudocode) from: libNexusEvasionRuntime69252.so
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
 */

/* ===== FUN_00144f20 @ 00144f20 [libNexusEvasionRuntime69252.so] ===== */

char * FUN_00144f20(void)

{
  int iVar1;
  undefined8 *puVar2;
  uint uVar3;
  char *pcVar4;
  char *__s1;
  long lVar5;
  
  puVar2 = (undefined8 *)FUN_001bd828(&DAT_001cfc98);
  pcVar4 = (char *)*puVar2;
  if (pcVar4 == (char *)0x0) {
    __s1 = "prepare";
  }
  else {
    lVar5 = 0;
    do {
      __s1 = *(char **)((long)&PTR_DAT_001c3298 + lVar5);
      iVar1 = strcmp(__s1,pcVar4);
      if (iVar1 == 0) break;
      lVar5 = lVar5 + 8;
      __s1 = "prepare";
    } while (lVar5 != 0xd0);
  }
  iVar1 = strcmp(__s1,"mmap");
  if ((((iVar1 == 0) || (iVar1 = strcmp(__s1,"seal"), iVar1 == 0)) ||
      (iVar1 = strcmp(__s1,"gap_open"), iVar1 == 0)) ||
     (((iVar1 = strcmp(__s1,"gap_read"), iVar1 == 0 ||
       (iVar1 = strcmp(__s1,"gap_close"), iVar1 == 0)) ||
      ((iVar1 = strcmp(__s1,"gap_mmap"), iVar1 == 0 ||
       (iVar1 = strcmp(__s1,"gap_unmap"), iVar1 == 0)))))) {
    uVar3 = *(uint *)(puVar2 + 1);
    if (0xffe < uVar3 - 1) {
      uVar3 = 0;
    }
  }
  else {
    uVar3 = 0;
  }
  pcVar4 = (char *)FUN_001bd828(&DAT_001cfcf8);
  snprintf(pcVar4,0x32,"near_%s_b337c4_e%u",__s1,(ulong)uVar3);
  return pcVar4;
}

/* ===== FUN_00152af0 @ 00152af0 [libNexusEvasionRuntime69252.so] ===== */

void * FUN_00152af0(void *param_1)

{
  void *pvVar1;
  ulong uVar2;
  ulong uVar3;
  ulong uVar4;
  ulong uVar5;
  ulong uVar6;
  char cVar7;
  byte bVar8;
  long lVar9;
  bool bVar10;
  ulong uVar11;
  bool bVar12;
  int iVar13;
  int iVar14;
  FILE *__stream;
  int *piVar15;
  char *pcVar16;
  size_t sVar17;
  uint uVar18;
  long lVar19;
  void *pvVar20;
  ulong uVar21;
  void *pvVar22;
  long lVar23;
  ulong uVar24;
  void *pvVar25;
  uint uVar26;
  ulong uVar27;
  ulong *puVar28;
  long local_24f8;
  int local_24cc;
  char *local_24c8;
  ulong local_24c0;
  ulong uStack_24b8;
  undefined1 auStack_24b0 [520];
  ulong local_22a8;
  ulong local_2298;
  ulong local_2290;
  undefined1 auStack_2288 [520];
  ulong local_2080;
  undefined4 local_2078;
  char cStack_2071;
  undefined4 local_2070;
  char local_206c [8188];
  long local_70;
  
  lVar9 = tpidr_el0;
  local_70 = *(long *)(lVar9 + 0x28);
  uVar27 = -DAT_00209d88 & (ulong)param_1;
  uVar26 = (uint)param_1;
  if ((DAT_00215a00 == (void *)0x0) || (2 < DAT_00215a08)) {
    if (DAT_00215a00 == (void *)0x0) {
      iVar14 = 0;
      bVar10 = false;
      sVar17 = DAT_00209d88 * 3;
      uVar24 = 0xffffffffffefffff;
      uVar21 = 0x100000;
      do {
        pvVar1 = (void *)0x0;
        if (uVar21 <= uVar27) {
          pvVar1 = (void *)(uVar27 - uVar21);
        }
        if ((uVar27 <= uVar24) && ((void *)(uVar27 + uVar21) != (void *)0x0)) {
          pvVar20 = mmap((void *)(uVar27 + uVar21),sVar17,3,0x22,-1,0);
          if (pvVar20 == (void *)0xffffffffffffffff) {
            piVar15 = (int *)__errno();
            iVar14 = *piVar15;
            goto joined_r0x00153600;
          }
          iVar13 = FUN_00157860();
          if (iVar13 == 0) {
            munmap(pvVar20,sVar17);
            bVar10 = true;
            goto joined_r0x00153600;
          }
LAB_00153684:
          DAT_00215a08 = 1;
          DAT_00215a00 = pvVar20;
          goto LAB_00152fe0;
        }
joined_r0x00153600:
        if (uVar21 < uVar27) {
          pvVar20 = mmap(pvVar1,sVar17,3,0x22,-1,0);
          if (pvVar20 == (void *)0xffffffffffffffff) {
            piVar15 = (int *)__errno();
            iVar14 = *piVar15;
          }
          else {
            iVar13 = FUN_00157860();
            if (iVar13 != 0) goto LAB_00153684;
            munmap(pvVar20,sVar17);
            bVar10 = true;
          }
        }
        bVar12 = uVar21 < 0x6f00001;
        uVar24 = uVar24 - 0x100000;
        uVar21 = uVar21 + 0x100000;
      } while (bVar12);
      goto LAB_00152c44;
    }
LAB_00152c3c:
    bVar10 = false;
    iVar14 = 0;
  }
  else {
    pvVar20 = (void *)((long)DAT_00215a00 + DAT_00209d88 * DAT_00215a08);
    if (pvVar20 < (void *)0x10000) goto LAB_00152c3c;
    bVar10 = false;
    if (DAT_00209d88 < 0x170) {
LAB_0015367c:
      bVar10 = false;
      iVar14 = 0;
    }
    else {
      iVar14 = 0;
      if (((ulong)pvVar20 & 0xf) == 0) {
        bVar10 = false;
        if (((void *)0xffffffffffffffef < param_1) || (CARRY8(DAT_00209d88,(ulong)pvVar20)))
        goto LAB_0015367c;
        iVar14 = 0;
        if (DAT_001e0978 < (void *)0xfffffffffec00000) {
          if ((((void *)((long)pvVar20 + DAT_00209d88) <= DAT_001e0978) ||
              ((void *)((long)DAT_001e0978 + 0x1400000U) <= pvVar20)) &&
             ((((uint)pvVar20 | uVar26) & 3) == 0)) {
            bVar10 = 0x8000000 < (ulong)((long)param_1 - (long)pvVar20);
            if (param_1 <= pvVar20) {
              bVar10 = ((long)pvVar20 - (long)param_1 & 0xfffffffff8000000U) != 0;
            }
            if (!bVar10) {
              uVar21 = (long)param_1 + 0x10;
              uVar24 = (long)pvVar20 + 0x15c;
              bVar10 = 0x8000000 < uVar24 - uVar21;
              if (uVar24 <= uVar21) {
                bVar10 = (uVar21 - uVar24 & 0xfffffffff8000000) != 0;
              }
              if (!bVar10) {
                DAT_00215a08 = DAT_00215a08 + 1;
                goto LAB_00152fe0;
              }
            }
          }
          goto LAB_00152c3c;
        }
      }
    }
  }
LAB_00152c44:
  uVar24 = 0xffffffffffefffff;
  pvVar1 = (void *)((long)param_1 + 0x10);
  uVar21 = 0x100000;
  do {
    pvVar22 = (void *)0x0;
    if (uVar21 <= uVar27) {
      pvVar22 = (void *)(uVar27 - uVar21);
    }
    if ((uVar27 <= uVar24) && ((void *)(uVar27 + uVar21) != (void *)0x0)) {
      pvVar20 = mmap((void *)(uVar27 + uVar21),DAT_00209d88,3,0x22,-1,0);
      if (pvVar20 == (void *)0xffffffffffffffff) {
        piVar15 = (int *)__errno();
        iVar14 = *piVar15;
      }
      else {
        if ((((void *)0xffff < pvVar20) && (((ulong)pvVar20 & 0xf) == 0)) &&
           ((0x16f < DAT_00209d88 &&
            (((((param_1 < (void *)0xfffffffffffffff0 && (!CARRY8(DAT_00209d88,(ulong)pvVar20))) &&
               (DAT_001e0978 < (void *)0xfffffffffec00000)) &&
              (((void *)(DAT_00209d88 + (long)pvVar20) <= DAT_001e0978 ||
               ((void *)((long)DAT_001e0978 + 0x1400000U) <= pvVar20)))) &&
             ((((uint)pvVar20 | uVar26) & 3) == 0)))))) {
          bVar10 = 0x8000000 < (ulong)((long)param_1 - (long)pvVar20);
          if (param_1 <= pvVar20) {
            bVar10 = ((long)pvVar20 - (long)param_1 & 0xfffffffff8000000U) != 0;
          }
          if (!bVar10) {
            pvVar25 = (void *)((long)pvVar20 + 0x15c);
            bVar10 = 0x8000000 < (ulong)((long)pvVar25 - (long)pvVar1);
            if (pvVar25 <= pvVar1) {
              bVar10 = ((long)pvVar1 - (long)pvVar25 & 0xfffffffff8000000U) != 0;
            }
            if (!bVar10) goto LAB_00152fe0;
          }
        }
        munmap(pvVar20,DAT_00209d88);
        bVar10 = true;
      }
    }
    if (uVar21 < uVar27) {
      pvVar20 = mmap(pvVar22,DAT_00209d88,3,0x22,-1,0);
      if (pvVar20 == (void *)0xffffffffffffffff) {
        piVar15 = (int *)__errno();
        iVar14 = *piVar15;
      }
      else {
        if ((((void *)0xffff < pvVar20) && (((ulong)pvVar20 & 0xf) == 0)) &&
           (((0x16f < DAT_00209d88 &&
             (((param_1 < (void *)0xfffffffffffffff0 && (!CARRY8(DAT_00209d88,(ulong)pvVar20))) &&
              (DAT_001e0978 < (void *)0xfffffffffec00000)))) &&
            ((((void *)(DAT_00209d88 + (long)pvVar20) <= DAT_001e0978 ||
              ((void *)((long)DAT_001e0978 + 0x1400000U) <= pvVar20)) &&
             ((((uint)pvVar20 | uVar26) & 3) == 0)))))) {
          bVar10 = 0x8000000 < (ulong)((long)param_1 - (long)pvVar20);
          if (param_1 <= pvVar20) {
            bVar10 = ((long)pvVar20 - (long)param_1 & 0xfffffffff8000000U) != 0;
          }
          if (!bVar10) {
            pvVar22 = (void *)((long)pvVar20 + 0x15c);
            bVar10 = 0x8000000 < (ulong)((long)pvVar22 - (long)pvVar1);
            if (pvVar22 <= pvVar1) {
              bVar10 = ((long)pvVar1 - (long)pvVar22 & 0xfffffffff8000000U) != 0;
            }
            if (!bVar10) goto LAB_00152fe0;
          }
        }
        munmap(pvVar20,DAT_00209d88);
        bVar10 = true;
      }
    }
    uVar11 = DAT_00209d88;
    pvVar20 = DAT_001e0978;
    bVar12 = uVar21 < 0x6f00001;
    uVar24 = uVar24 - 0x100000;
    uVar21 = uVar21 + 0x100000;
  } while (bVar12);
  local_24c8 = "alloc_mmap";
  if (bVar10) {
    iVar14 = 0;
    local_24c8 = "alloc_range";
  }
  local_24cc = iVar14;
  memset(auStack_24b0,0,0x218);
  if (((((ulong)param_1 & 3) == 0) && (uVar11 == 0x1000 || uVar11 == 0x4000)) &&
     ((pvVar20 < (void *)0xfffffffffec00000 &&
      ((pvVar20 <= param_1 && ((ulong)((long)param_1 - (long)pvVar20) >> 0x16 < 5)))))) {
    uVar24 = -uVar11;
    uVar27 = (long)param_1 - 0x8000000;
    uVar21 = uVar27;
    if ((ulong)param_1 >> 0x1b == 0) {
      uVar21 = 0;
    }
    uVar3 = (long)param_1 + 0x7fffeb4;
    uVar6 = uVar3;
    if ((void *)0xfffffffff800014b < param_1) {
      uVar6 = 0xffffffffffffffff;
    }
    uVar4 = uVar21;
    if (uVar21 < 0x10001) {
      uVar4 = 0x10000;
    }
    uStack_24b8 = uVar6;
    if (~uVar11 <= uVar6) {
      uStack_24b8 = ~uVar11;
    }
    local_24c0 = (uVar11 - 1) + uVar4 & uVar24;
    uStack_24b8 = uStack_24b8 & uVar24;
    local_22a8 = uVar11;
    if (local_24c0 <= uStack_24b8) {
      puVar28 = (ulong *)0x0;
      if ((DAT_00215a00 != (void *)0x0) || ((void *)((long)pvVar20 + 0xb2e674U) != param_1)) {
LAB_00153138:
        __stream = fopen("/proc/self/maps","r");
        if (__stream != (FILE *)0x0) {
LAB_00153158:
          piVar15 = (int *)__errno();
          uVar26 = 0;
          local_24f8 = 0;
          pvVar20 = (void *)0x0;
          bVar10 = false;
LAB_0015317c:
          bVar12 = bVar10;
          *piVar15 = 0;
          pcVar16 = fgets((char *)&local_2070,0x2000,__stream);
          if (pcVar16 != (char *)0x0) {
LAB_0015331c:
            sVar17 = strlen((char *)&local_2070);
            if ((sVar17 != 0) && ((&cStack_2071)[sVar17] == '\n')) {
              iVar14 = 0;
              uVar26 = uVar26 + 1;
              if (uVar26 < 0x8001) {
                pcVar16 = "gap_bounds";
                if (sVar17 <= 0x800000U - local_24f8) {
                  lVar23 = 0;
                  pvVar22 = (void *)0x0;
                  local_24f8 = sVar17 + local_24f8;
                  do {
                    bVar8 = local_206c[lVar23 + -4];
                    uVar18 = (uint)bVar8;
                    if (bVar8 - 0x30 < 10) {
                      iVar14 = -0x30;
                    }
                    else {
                      iVar14 = -0x57;
                      if (5 < bVar8 - 0x61) {
                        if (5 < uVar18 - 0x41) goto LAB_001533d4;
                        iVar14 = -0x37;
                      }
                    }
                    if ((ulong)pvVar22 >> 0x3c != 0) goto LAB_00153660;
                    lVar23 = lVar23 + 1;
                    pvVar22 = (void *)((ulong)(iVar14 + uVar18) + (long)pvVar22 * 0x10);
                  } while( true );
                }
                goto LAB_00153664;
              }
            }
            iVar14 = 0;
            pcVar16 = "gap_bounds";
            goto LAB_00153664;
          }
          iVar14 = *piVar15;
          iVar13 = ferror(__stream);
          if ((iVar13 == 0) || (iVar14 != 4)) {
LAB_0015386c:
            if (iVar13 == 0) {
              iVar13 = feof(__stream);
              iVar14 = 0;
              if (iVar13 != 0) {
                pcVar16 = "gap_maps";
                if ((uVar26 == 0) || (!bVar12)) goto LAB_00153664;
                if (puVar28 != (ulong *)0x0) {
                  FUN_00157cb8(puVar28,param_1,pvVar20,0xffffffffffffffff);
                }
                FUN_00157cb8(&local_24c0,param_1,pvVar20,0xffffffffffffffff);
                iVar14 = fclose(__stream);
                if (iVar14 != 0) {
                  local_24c8 = "gap_close";
                  local_24cc = *piVar15;
                  goto LAB_00152fcc;
                }
                local_24cc = 0;
                local_2070 = 0;
                local_24c8 = "gap_empty";
                if (puVar28 != (ulong *)0x0) {
                  pvVar20 = (void *)FUN_00157ab4(param_1,puVar28,&local_24c8,&local_24cc,&local_2070
                                                );
                  if (pvVar20 != (void *)0x0) {
                    DAT_00215a08 = 1;
                    DAT_00215a00 = pvVar20;
                    goto LAB_00152fe0;
                  }
                  if (local_2070 != 0) goto LAB_00152fcc;
                }
                pvVar20 = (void *)FUN_00157ab4(param_1,&local_24c0,&local_24c8,&local_24cc,
                                               &local_2070);
                if (pvVar20 == (void *)0x0) goto LAB_00152fcc;
                goto LAB_00152fe0;
              }
            }
          }
          else {
            clearerr(__stream);
            *piVar15 = 0;
            pcVar16 = fgets((char *)&local_2070,0x2000,__stream);
            if (pcVar16 != (char *)0x0) goto LAB_0015331c;
            iVar14 = *piVar15;
            iVar13 = ferror(__stream);
            if ((iVar13 == 0) || (iVar14 != 4)) goto LAB_0015386c;
            clearerr(__stream);
            *piVar15 = 0;
            pcVar16 = fgets((char *)&local_2070,0x2000,__stream);
            if (pcVar16 != (char *)0x0) goto LAB_0015331c;
            iVar14 = *piVar15;
            iVar13 = ferror(__stream);
            if ((iVar13 == 0) || (iVar14 != 4)) goto LAB_0015386c;
            clearerr(__stream);
            *piVar15 = 0;
            pcVar16 = fgets((char *)&local_2070,0x2000,__stream);
            if (pcVar16 != (char *)0x0) goto LAB_0015331c;
            iVar14 = *piVar15;
            iVar13 = ferror(__stream);
            if ((iVar13 == 0) || (iVar14 != 4)) goto LAB_0015386c;
            clearerr(__stream);
            *piVar15 = 0;
            pcVar16 = fgets((char *)&local_2070,0x2000,__stream);
            if (pcVar16 != (char *)0x0) goto LAB_0015331c;
            iVar14 = *piVar15;
            iVar13 = ferror(__stream);
            if ((iVar13 == 0) || (iVar14 != 4)) goto LAB_0015386c;
            clearerr(__stream);
            *piVar15 = 0;
            pcVar16 = fgets((char *)&local_2070,0x2000,__stream);
            if (pcVar16 != (char *)0x0) goto LAB_0015331c;
            iVar14 = *piVar15;
            iVar13 = ferror(__stream);
            if ((iVar13 == 0) || (iVar14 != 4)) goto LAB_0015386c;
            clearerr(__stream);
            *piVar15 = 0;
            pcVar16 = fgets((char *)&local_2070,0x2000,__stream);
            if (pcVar16 != (char *)0x0) goto LAB_0015331c;
            iVar14 = *piVar15;
            iVar13 = ferror(__stream);
            if ((iVar13 == 0) || (iVar14 != 4)) goto LAB_0015386c;
            clearerr(__stream);
            *piVar15 = 0;
            pcVar16 = fgets((char *)&local_2070,0x2000,__stream);
            if (pcVar16 != (char *)0x0) goto LAB_0015331c;
            iVar14 = *piVar15;
            iVar13 = ferror(__stream);
            if ((iVar13 == 0) || (iVar14 != 4)) goto LAB_0015386c;
          }
          pcVar16 = "gap_read";
          goto LAB_00153664;
        }
        piVar15 = (int *)__errno();
        iVar14 = *piVar15;
        if (iVar14 == 4) {
          __stream = fopen("/proc/self/maps","r");
          if (__stream != (FILE *)0x0) goto LAB_00153158;
          piVar15 = (int *)__errno();
          iVar14 = *piVar15;
          if (iVar14 == 4) {
            __stream = fopen("/proc/self/maps","r");
            if (__stream != (FILE *)0x0) goto LAB_00153158;
            piVar15 = (int *)__errno();
            iVar14 = *piVar15;
            if (iVar14 == 4) {
              __stream = fopen("/proc/self/maps","r");
              if (__stream != (FILE *)0x0) goto LAB_00153158;
              piVar15 = (int *)__errno();
              iVar14 = *piVar15;
              if (iVar14 == 4) {
                __stream = fopen("/proc/self/maps","r");
                if (__stream != (FILE *)0x0) goto LAB_00153158;
                piVar15 = (int *)__errno();
                iVar14 = *piVar15;
                if (iVar14 == 4) {
                  __stream = fopen("/proc/self/maps","r");
                  if (__stream != (FILE *)0x0) goto LAB_00153158;
                  piVar15 = (int *)__errno();
                  iVar14 = *piVar15;
                  if (iVar14 == 4) {
                    __stream = fopen("/proc/self/maps","r");
                    if (__stream != (FILE *)0x0) goto LAB_00153158;
                    piVar15 = (int *)__errno();
                    iVar14 = *piVar15;
                    if (iVar14 == 4) {
                      __stream = fopen("/proc/self/maps","r");
                      if (__stream != (FILE *)0x0) goto LAB_00153158;
                      piVar15 = (int *)__errno();
                      iVar14 = *piVar15;
                    }
                  }
                }
              }
            }
          }
        }
        local_24c8 = "gap_open";
        local_24cc = iVar14;
        goto LAB_00152fcc;
      }
      memset(auStack_2288,0,0x218);
      uVar4 = (long)pvVar20 - 0x74d1880;
      if (pvVar20 < (void *)0x74d1880) {
        uVar4 = 0;
      }
      uVar2 = (long)pvVar20 - 0x74d166c;
      uVar5 = 0;
      if (uVar11 * 2 <= uVar4) {
        uVar5 = uVar4 + uVar11 * -2;
      }
      if (pvVar20 < (void *)0x74d166c) {
        uVar2 = 0;
      }
      uVar4 = 0;
      if (uVar11 <= uVar2) {
        uVar4 = uVar2 - uVar11;
      }
      if (uVar21 < 0x10001) {
        uVar27 = 0x10000;
      }
      local_2078 = 1;
      if (uVar4 <= uVar27) {
        uVar4 = uVar27;
      }
      local_2080 = uVar11 * 3;
      if (uVar5 <= uVar4) {
        uVar5 = uVar4;
      }
      if (uVar5 <= uVar24) {
        lVar19 = (long)pvVar20 + 0x8b2e634;
        lVar23 = (long)pvVar20 + 0x8b2e848;
        if ((void *)0xfffffffff74d19cb < pvVar20) {
          lVar19 = -1;
        }
        if ((void *)0xfffffffff74d17b7 < pvVar20) {
          lVar23 = -1;
        }
        if (~local_2080 <= uVar6) {
          uVar3 = ~local_2080;
        }
        local_2290 = lVar19 + uVar11 * -2;
        uVar27 = lVar23 - uVar11;
        if (uVar3 <= lVar23 - uVar11) {
          uVar27 = uVar3;
        }
        local_2298 = uVar5 + (uVar11 - 1) & uVar24;
        if (uVar27 <= local_2290) {
          local_2290 = uVar27;
        }
        local_2290 = local_2290 & uVar24;
        if (local_2298 <= local_2290) {
          puVar28 = &local_2298;
          goto LAB_00153138;
        }
      }
    }
  }
  local_24cc = 0;
  local_24c8 = "gap_window";
  goto LAB_00152fcc;
LAB_001533d4:
  iVar14 = 0;
  if ((int)lVar23 != 0) {
    pcVar16 = "gap_parse";
    if (uVar18 == 0x2d) {
      lVar19 = 0;
      pvVar25 = (void *)0x0;
      do {
        bVar8 = local_206c[lVar19 + lVar23 + -3];
        uVar18 = (uint)bVar8;
        if (bVar8 - 0x30 < 10) {
          iVar14 = -0x30;
        }
        else {
          iVar14 = -0x57;
          if (5 < bVar8 - 0x61) {
            if (5 < uVar18 - 0x41) goto LAB_00153448;
            iVar14 = -0x37;
          }
        }
        if ((ulong)pvVar25 >> 0x3c != 0) goto LAB_00153660;
        lVar19 = lVar19 + 1;
        pvVar25 = (void *)((ulong)(iVar14 + uVar18) + (long)pvVar25 * 0x10);
      } while( true );
    }
    goto LAB_00153664;
  }
  goto LAB_00153660;
LAB_00153448:
  if (((int)lVar19 != 0) && (uVar18 == 0x20)) {
    sVar17 = strlen(local_206c + lVar23 + lVar19 + -2);
    if ((sVar17 < 5) || ((cVar7 = local_206c[lVar23 + lVar19 + -2], cVar7 != 'r' && (cVar7 != '-')))
       ) {
      iVar14 = 0;
      pcVar16 = "gap_parse";
      goto LAB_00153664;
    }
    if (((local_206c[lVar23 + lVar19 + -1] != 'w') && (local_206c[lVar23 + lVar19 + -1] != '-')) ||
       ((local_206c[lVar23 + lVar19] != 'x' && (local_206c[lVar23 + lVar19] != '-'))))
    goto LAB_00153804;
    if (((local_206c[lVar23 + lVar19 + 1] != 's') && (local_206c[lVar23 + lVar19 + 1] != 'p')) ||
       (local_206c[lVar23 + lVar19 + 2] != ' ')) goto LAB_00153804;
    iVar14 = 0;
    if (pvVar25 <= pvVar22) {
      pcVar16 = "gap_parse";
      goto LAB_00153664;
    }
    pcVar16 = "gap_parse";
    if (pvVar22 < pvVar20) goto LAB_00153664;
    if ((DAT_00209d88 - 1 & ((ulong)pvVar25 | (ulong)pvVar22)) != 0) goto LAB_00153804;
    if (puVar28 != (ulong *)0x0) {
      FUN_00157cb8(puVar28,param_1,pvVar20,pvVar22);
    }
    FUN_00157cb8(&local_24c0,param_1,pvVar20,pvVar22);
    bVar10 = bVar12;
    if (pvVar1 <= pvVar25) {
      bVar10 = true;
    }
    pvVar20 = pvVar25;
    if (param_1 < pvVar22 || (void *)0xffffffffffffffef < param_1) {
      bVar10 = bVar12;
    }
    goto LAB_0015317c;
  }
LAB_00153660:
  iVar14 = 0;
  pcVar16 = "gap_parse";
  goto LAB_00153664;
LAB_00153804:
  iVar14 = 0;
  pcVar16 = "gap_parse";
LAB_00153664:
  local_24cc = iVar14;
  local_24c8 = pcVar16;
  fclose(__stream);
LAB_00152fcc:
  FUN_00137ca4(local_24c8,param_1,local_24cc);
  pvVar20 = (void *)0x0;
LAB_00152fe0:
  if (*(long *)(lVar9 + 0x28) == local_70) {
    return pvVar20;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00157ab4 @ 00157ab4 [libNexusEvasionRuntime69252.so] ===== */

void * FUN_00157ab4(void *param_1,long param_2,undefined8 *param_3,undefined4 *param_4,
                   undefined4 *param_5)

{
  ulong uVar1;
  ulong uVar2;
  bool bVar3;
  int iVar4;
  void *__addr;
  undefined4 *puVar5;
  size_t sVar6;
  undefined4 uVar7;
  void *__addr_00;
  ulong uVar8;
  
  if (*(int *)(param_2 + 0x210) != 0) {
    uVar8 = 0;
    uVar1 = (long)param_1 + 0x10;
    do {
      __addr_00 = *(void **)(param_2 + 0x10 + uVar8 * 8);
      if (*(int *)(param_2 + 0x220) == 0) {
        if (((((((void *)0xffff < __addr_00) && (((ulong)__addr_00 & 0xf) == 0)) &&
              (sVar6 = *(ulong *)(param_2 + 0x218), 0x16f < sVar6)) &&
             ((param_1 < (void *)0xfffffffffffffff0 && (!CARRY8(sVar6,(ulong)__addr_00))))) &&
            ((DAT_001e0978 < 0xfffffffffec00000 &&
             ((sVar6 + (long)__addr_00 <= DAT_001e0978 ||
              ((void *)(DAT_001e0978 + 0x1400000) <= __addr_00)))))) &&
           ((((uint)__addr_00 | (uint)param_1) & 3) == 0)) {
          bVar3 = 0x8000000 < (ulong)((long)param_1 - (long)__addr_00);
          if (param_1 <= __addr_00) {
            bVar3 = ((long)__addr_00 - (long)param_1 & 0xfffffffff8000000U) != 0;
          }
          if (!bVar3) {
            uVar2 = (long)__addr_00 + 0x15c;
            bVar3 = 0x8000000 < uVar2 - uVar1;
            if (uVar2 <= uVar1) {
              bVar3 = (uVar1 - uVar2 & 0xfffffffff8000000) != 0;
            }
            if (!bVar3) goto LAB_00157c10;
          }
        }
LAB_00157c70:
        uVar7 = 0;
        *param_3 = "gap_range";
LAB_00157c80:
        *param_4 = uVar7;
        *param_5 = 1;
        return (void *)0x0;
      }
      iVar4 = FUN_00157860(__addr_00);
      if (iVar4 == 0) goto LAB_00157c70;
      sVar6 = *(size_t *)(param_2 + 0x218);
LAB_00157c10:
      __addr = mmap(__addr_00,sVar6,3,0x100022,-1,0);
      if (__addr == (void *)0xffffffffffffffff) {
        *param_3 = "gap_mmap";
        puVar5 = (undefined4 *)__errno();
        *param_4 = *puVar5;
      }
      else {
        if (__addr_00 == __addr) {
          return __addr_00;
        }
        sVar6 = *(size_t *)(param_2 + 0x218);
        *param_3 = "gap_return";
        *param_4 = 0;
        iVar4 = munmap(__addr,sVar6);
        if (iVar4 != 0) {
          *param_3 = "gap_unmap";
          puVar5 = (undefined4 *)__errno();
          uVar7 = *puVar5;
          goto LAB_00157c80;
        }
      }
      uVar8 = uVar8 + 1;
    } while (uVar8 < *(uint *)(param_2 + 0x210));
  }
  return (void *)0x0;
}

/* ===== FUN_0015b524 @ 0015b524 [libNexusEvasionRuntime69252.so] ===== */

void FUN_0015b524(FILE *param_1)

{
  long lVar1;
  int iVar2;
  uint uVar3;
  char *pcVar4;
  ulong uVar5;
  int iVar6;
  ulong local_280;
  ulong local_278;
  char local_270;
  char local_26f;
  char acStack_268 [512];
  long local_68;
  
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  uVar5 = (ulong)param_1 & 0xffffffffffffff;
  DAT_002170e8 = 0;
  DAT_002170f0 = 0;
  if ((0xffff < uVar5) && (param_1 = fopen("/proc/self/maps","r"), param_1 != (FILE *)0x0)) {
    iVar6 = 0x1000;
    do {
      pcVar4 = fgets(acStack_268,0x200,param_1);
      if (pcVar4 == (char *)0x0) break;
      iVar2 = sscanf(acStack_268,"%llx-%llx %4s",&local_278,&local_280,&local_270);
      if ((((iVar2 == 3) && (local_278 <= uVar5 + 0xa8)) && (uVar5 + 0xb8 <= local_280)) &&
         ((local_270 == 'r' && (local_26f == 'w')))) {
        DAT_002170f0 = local_278;
        DAT_002170e8 = local_280;
        break;
      }
      iVar6 = iVar6 + -1;
    } while (iVar6 != 0);
    uVar3 = fclose(param_1);
    param_1 = (FILE *)(ulong)uVar3;
  }
  if (*(long *)(lVar1 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(param_1);
}

/* ===== FUN_0015c6d4 @ 0015c6d4 [libNexusEvasionRuntime69252.so] ===== */

void FUN_0015c6d4(char *param_1)

{
  bool bVar1;
  long lVar2;
  ulong uVar3;
  ulong uVar4;
  bool bVar5;
  int iVar6;
  uint uVar7;
  char *pcVar8;
  char *pcVar9;
  undefined8 uVar10;
  void *pvVar11;
  char *__s;
  ulong uVar12;
  long lVar13;
  ulong __n;
  long lVar14;
  int local_1f4;
  char *local_1f0;
  long local_1e8;
  short local_1e0 [2];
  int local_1dc;
  long local_1d8;
  long local_1d0;
  ulong local_1c8;
  ulong local_1c0;
  code *local_1b8;
  undefined8 local_80;
  char *local_78;
  long local_70;
  
  lVar2 = tpidr_el0;
  local_70 = *(long *)(lVar2 + 0x28);
  pcVar8 = param_1;
  if ((param_1 == (char *)0x0) || (pcVar8 = (char *)FUN_001550fc(), (int)pcVar8 == 0))
  goto LAB_0015cd28;
  if (DAT_0021c9f0 == 1) {
LAB_0015c8ac:
    uVar12 = *(ulong *)(param_1 + 0x28);
    if ((DAT_0021c798 != 0) && (uVar12 < DAT_0021c798)) goto LAB_0015cd28;
    DAT_0021c798 = uVar12 + 100;
    local_1f4 = -1;
    pcVar8 = (char *)FUN_001428fc(pcVar8,DAT_0020f678 + 0xe0,&local_1f4,4);
    uVar3 = DAT_0020f680;
    if (((int)pcVar8 == 0) ||
       (((local_1f4 < 0 || (0x3f < local_1f4)) || (DAT_0020f698 != local_1f4)))) {
      if ((DAT_0021ca00 >> 1 & 1) != 0) goto LAB_0015cd28;
      DAT_0021ca00 = DAT_0021ca00 | 2;
      uVar7 = DAT_0021ca10 + 1;
      bVar5 = 0x13 < DAT_0021ca10;
      DAT_0021ca10 = uVar7;
      if (bVar5) goto LAB_0015cd28;
      pcVar8 = "own_identity_refused";
    }
    else {
      local_1c8 = 0;
      local_1c0 = 0;
      local_1d8 = 0;
      local_1d0 = 0;
      local_80 = 0;
      local_1dc = -1;
      if ((((((DAT_0020f680 != 0) &&
             (pcVar8 = (char *)FUN_001428fc(pcVar8,DAT_0020f680 + 0x20,&local_1c0,8),
             (int)pcVar8 != 0)) &&
            ((0x11fff < local_1c0 + 0x2000 &&
             (((local_1c0 & 7) == 0 &&
              (pcVar8 = (char *)FUN_001428fc(pcVar8,local_1c0,&local_1d0,8), (int)pcVar8 != 0))))))
           && (local_1d0 == DAT_001e0978 + 0x11be3f0)) &&
          ((((((pcVar8 = (char *)FUN_001428fc(pcVar8,local_1c0 + 0x10,&local_80,8), (int)pcVar8 != 0
               && (local_80 - 0x10000 < 0xfffffffffffee000)) && ((local_80 & 7) == 0)) &&
             ((local_80 == uVar3 &&
              (pcVar8 = (char *)FUN_001428fc(pcVar8,local_1c0 + 0x590,&local_1c8,8),
              (int)pcVar8 != 0)))) && (0x11fff < local_1c8 + 0x2000)) &&
           (((local_1c8 & 7) == 0 &&
            (pcVar8 = (char *)FUN_001428fc(pcVar8,local_1c8,&local_1d0,8), (int)pcVar8 != 0)))))) &&
         (((local_1d0 == DAT_001e0978 + 0x11ad208 &&
           (((pcVar9 = (char *)(*(code *)(DAT_001e0978 + 0x5d8ad4))(local_1c8,"player_name"),
             pcVar8 = pcVar9, pcVar9 != (char *)0x0 &&
             (pcVar8 = (char *)FUN_001428fc(pcVar9,pcVar9,&local_1d0,8), (int)pcVar8 != 0)) &&
            (local_1d0 == DAT_001e0978 + 0x11abad8)))) &&
          ((pcVar8 = (char *)FUN_001428fc(pcVar8,pcVar9 + 0x38,&local_1d8,8), (int)pcVar8 != 0 &&
           (pcVar8 = (char *)FUN_001428fc(pcVar8,pcVar9 + 0x40,&local_1dc,4), (int)pcVar8 != 0))))))
      {
        pcVar8 = (char *)FUN_0016413c(pcVar9,local_1c8);
        if (((int)pcVar8 == 0) && ((local_1d8 == 0 && (local_1dc == -1)))) {
          local_1e0[0] = 0;
          local_1e8 = 0;
          iVar6 = FUN_001428fc(pcVar8,local_1c8 + 0xc0,local_1e0,2);
          pcVar8 = (char *)0x0;
          if ((iVar6 != 0) && ((0 < local_1e0[0] && (local_1e0[0] < 0x201)))) {
            iVar6 = FUN_0013a78c(local_1c8 + 0x90,&local_1e8);
            pcVar8 = (char *)0x0;
            if ((iVar6 != 0) && (0 < local_1e0[0])) {
              lVar13 = 0;
              lVar14 = 0;
              iVar6 = 0;
              do {
                local_1f0 = (char *)0x0;
                pcVar8 = (char *)FUN_001428fc(pcVar8,lVar13 + local_1e8,&local_1f0,8);
                if ((int)pcVar8 == 0) goto LAB_0015cce4;
                lVar14 = lVar14 + 1;
                lVar13 = lVar13 + 8;
                if (local_1f0 == pcVar9) {
                  iVar6 = iVar6 + 1;
                }
              } while (lVar14 < local_1e0[0]);
              pcVar8 = (char *)(ulong)(iVar6 == 1);
            }
          }
        }
        uVar4 = local_1c0;
        uVar3 = local_1c8;
        if ((int)pcVar8 != 0) {
          if ((pcVar9 + 0xe0 != (char *)0x0) &&
             (uVar10 = FUN_001428fc(pcVar8,pcVar9 + 0xe0,&local_80,0x10), (int)uVar10 != 0)) {
            __n = (ulong)local_80._4_4_;
            if ((local_80._4_4_ != 0) && (local_80._4_4_ < 0x140)) {
              pcVar8 = pcVar9 + 0xe8;
              if (7 < local_80._4_4_) {
                pcVar8 = local_78;
              }
              if ((pcVar8 != (char *)0x0) &&
                 (iVar6 = FUN_001428fc(uVar10,pcVar8,&local_1c0,__n), iVar6 != 0)) {
                pvVar11 = memchr(&local_1c0,0,__n);
                if (pvVar11 == (void *)0x0) {
                  *(undefined1 *)((long)&local_1c0 + __n) = 0;
                  if ((((DAT_0021c7a0 != DAT_0020f6f8) || (DAT_0021c7a8 != uVar4)) ||
                      (DAT_0021c7b0 != uVar3)) || (DAT_0021c7b8 != pcVar9)) {
                    FUN_00161de4();
                    pvVar11 = memset(&DAT_0021c7c0,0,0x230);
                    DAT_0021c7b0 = uVar3;
                    DAT_0021c7a0 = DAT_0020f6f8;
                    DAT_0021c7a8 = uVar4;
                    DAT_0021c7b8 = pcVar9;
                    DAT_0021c818 = FUN_001428fc(pvVar11,pcVar9 + 0x80,&DAT_0021c814,4);
                  }
                  if ((DAT_0021c82c == '\0') ||
                     (uVar7 = strcmp((char *)&local_1c0,&DAT_0021c8ac), uVar7 != 0)) {
                    __s = (char *)FUN_00161ff8(&local_1c0);
                    pcVar8 = __s;
                    if ((__s == (char *)0x0) ||
                       (pcVar8 = (char *)strlen(__s), (char *)0x7f < pcVar8)) goto LAB_0015cd28;
                    uVar7 = snprintf(&DAT_0021c82c,0x80,"%s",__s);
                  }
                  pcVar8 = (char *)(ulong)uVar7;
                  if (DAT_0021c818 == 0) goto LAB_0015cd28;
                  iVar6 = pthread_once((pthread_once_t *)&DAT_0021ca04,FUN_00164598);
                  if (DAT_0021ca08 == (code *)0x0) {
                    bVar5 = false;
                  }
                  else {
                    iVar6 = (*DAT_0021ca08)(iVar6);
                    bVar5 = iVar6 == 1;
                  }
                  iVar6 = strcmp((char *)&local_1c0,&DAT_0021c82c);
                  if (iVar6 != 0) {
                    local_80 = 0;
                    local_78 = (char *)0x0;
                    (*(code *)(DAT_001e0978 + 0x66ae58))(&local_80,&DAT_0021c82c);
                    (*(code *)(DAT_001e0978 + 0x5921a0))(pcVar9,&local_80);
                    (*(code *)(DAT_001e0978 + 0x66ad48))(&local_80);
                    uVar7 = DAT_0021ca10 + 1;
                    bVar1 = DAT_0021ca10 < 0x14;
                    DAT_0021ca10 = uVar7;
                    if (bVar1) {
                      FUN_001417c8("branding","name_restored",0);
                    }
                  }
                  snprintf(&DAT_0021c8ac,0x140,"%s",&DAT_0021c82c);
                  pcVar8 = (char *)FUN_00162104(uVar3,pcVar9);
                  if ((int)pcVar8 == 0) goto LAB_0015cd28;
                  FUN_00162690(bVar5,uVar12);
                  iVar6 = FUN_001628fc(DAT_0021c7c0,uVar3);
                  uVar7 = DAT_0021ca10;
                  if (iVar6 == 0 && DAT_0021c808 == 0) {
                    DAT_0021c7c0 = 0;
                    iVar6 = FUN_00162a78(uVar3);
                    if (iVar6 == 0) {
                      DAT_0021c808 = 1;
                      uVar7 = DAT_0021ca10 + 1;
                      if (DAT_0021ca10 < 0x14) {
                        pcVar8 = "image_refused";
                        goto LAB_0015cfb4;
                      }
                    }
                    else {
                      uVar7 = DAT_0021ca10 + 1;
                      if (DAT_0021ca10 < 0x14) {
                        pcVar8 = "image_created";
LAB_0015cfb4:
                        DAT_0021ca10 = DAT_0021ca10 + 1;
                        FUN_001417c8("branding",pcVar8,0);
                        uVar7 = DAT_0021ca10;
                      }
                    }
                  }
                  DAT_0021ca10 = uVar7;
                  pcVar8 = (char *)FUN_00162f50(pcVar9,bVar5);
                  goto LAB_0015cd28;
                }
              }
            }
          }
          pcVar8 = (char *)FUN_00161da0(8,"name_read_refused");
          goto LAB_0015cd28;
        }
      }
LAB_0015cce4:
      if ((DAT_0021ca00 >> 2 & 1) != 0) goto LAB_0015cd28;
      DAT_0021ca00 = DAT_0021ca00 | 4;
      uVar7 = DAT_0021ca10 + 1;
      bVar5 = 0x13 < DAT_0021ca10;
      DAT_0021ca10 = uVar7;
      if (bVar5) goto LAB_0015cd28;
      pcVar8 = "nameplate_refused";
    }
  }
  else {
    if (DAT_0021c9f0 != 0) goto LAB_0015cd28;
    DAT_0021c9f0 = -1;
    pcVar8 = (char *)FUN_0014b1d0(0x80f09c,0x20,
                                  "ceaed7de0bf298f5764c6e7c23a9e61f0ef78d62461e51956e3ba81f858a05b5"
                                 );
    if (((int)pcVar8 != 0) &&
       (pcVar8 = (char *)FUN_0014b1d0(0x5d8ad4,0x20,
                                      "284c790c5c501cf083a6f4d323aa30275b074eb44bef7b1d64f70d9250bd6a23"
                                     ), (int)pcVar8 != 0)) {
      iVar6 = FUN_0014b1d0(0x5921a0,0x20,
                           "14b79d91644b7f17357a86b8ec6d36e17be6a1c815083ec6c2fa531bee369974");
      if (iVar6 == 0) {
        if (DAT_0021c9f8 == (code *)0x0) {
          local_1c0 = 0;
          local_1b8 = (code *)0x0;
          pcVar8 = (char *)dl_iterate_phdr(FUN_00163228,&local_1c0);
          if (((int)local_1c0 != 1) || (local_1b8 == (code *)0x0)) goto LAB_0015cca0;
          DAT_0021c9f8 = local_1b8;
        }
        pcVar8 = (char *)(*DAT_0021c9f8)(DAT_001e0978);
        if ((int)pcVar8 != 1) goto LAB_0015cca0;
      }
      pcVar8 = (char *)FUN_0014b1d0(0x66ae58,0x20,
                                    "dbee7aa155f50e7bbcb598d7292a63cdcc6b00e7bef42df179bba8e717a01cb2"
                                   );
      if ((((((int)pcVar8 != 0) &&
            (pcVar8 = (char *)FUN_0014b1d0(0x66ad48,0x20,
                                           "23f6ad205540b5dd1d4a6dfec22810176034231102bb53dc25ce7b774778e92c"
                                          ), (int)pcVar8 != 0)) &&
           (pcVar8 = (char *)FUN_0014b1d0(0xb7c41c,0x20,
                                          "40459ad7d604600054570f79962f208d94703d75b3f5903e63af68cbc4ee756a"
                                         ), (int)pcVar8 != 0)) &&
          (((pcVar8 = (char *)FUN_0014b1d0(0x5955dc,0x20,
                                           "c715b651533d6c19dec4381cfd0ed1002d060d936ca20c8b6bdd10aea0d24859"
                                          ), (int)pcVar8 != 0 &&
            (pcVar8 = (char *)FUN_0014b1d0(0x595314,0x20,
                                           "0558f75486a424fcf1edb8b5b3cace86a0dc5620506aab1273a1df1dc7ad2ad8"
                                          ), (int)pcVar8 != 0)) &&
           ((pcVar8 = (char *)FUN_0014b1d0(0x594138,0x20,
                                           "f74689171f152d63ffdcb66f59b8532476d7dd17a559c44c6fb98cfc919d42ee"
                                          ), (int)pcVar8 != 0 &&
            ((pcVar8 = (char *)FUN_0014b1d0(0x592368,0x20,
                                            "74116921e171c3edbd6ae1a89ccc993046c5a12e87b8dc8fa3d44200102f34c6"
                                           ), (int)pcVar8 != 0 &&
             (pcVar8 = (char *)FUN_0014b1d0(0xb7c73c,0x20,
                                            "30c08799bf93e6dc1dc368fb371a3076f43bee2d3ca800b2ea1e1aff828e000b"
                                           ), (int)pcVar8 != 0)))))))) &&
         ((pcVar8 = (char *)FUN_0014b1d0(0x59567c,0x20,
                                         "9e18529b88a6285f2675ad92537b12875a9735c6fc16cbdc947b58b5e235b5f9"
                                        ), (int)pcVar8 != 0 &&
          (pcVar8 = (char *)FUN_0014b1d0(0x11a2840,0x20,
                                         "7624512f3481536b03735eb7d1bad9138b0288494a9458e050b9bd27feef8f66"
                                        ), (int)pcVar8 != 0)))) {
        DAT_0021c9f0 = 1;
        goto LAB_0015c8ac;
      }
    }
LAB_0015cca0:
    if ((DAT_0021ca00 & 1) != 0) goto LAB_0015cd28;
    DAT_0021ca00 = DAT_0021ca00 | 1;
    uVar7 = DAT_0021ca10 + 1;
    bVar5 = 0x13 < DAT_0021ca10;
    DAT_0021ca10 = uVar7;
    if (bVar5) goto LAB_0015cd28;
    pcVar8 = "abi_refused";
  }
  pcVar8 = (char *)FUN_001417c8("branding",pcVar8,0);
LAB_0015cd28:
  if (*(long *)(lVar2 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(pcVar8);
  }
  return;
}

/* ===== FUN_00161098 @ 00161098 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00161098(long param_1,ulong param_2,int param_3,int param_4)

{
  uint uVar1;
  char *pcVar2;
  long lVar3;
  long lVar4;
  uint uVar5;
  bool bVar6;
  int iVar7;
  undefined8 uVar8;
  long lVar9;
  uint uVar10;
  ulong uVar11;
  uint uVar12;
  byte *pbVar13;
  long lVar14;
  long lVar15;
  int local_300;
  int local_2fc;
  ulong local_2f8;
  uint local_2ec;
  ulong local_2e8;
  undefined8 local_2e0;
  ulong local_2d8;
  ulong local_2d0;
  timespec local_2c8 [37];
  long local_70;
  
  lVar3 = tpidr_el0;
  local_70 = *(long *)(lVar3 + 0x28);
  local_2d8 = 0;
  local_2d0 = 0;
  local_2e0 = 0;
  iVar7 = clock_gettime(1,local_2c8);
  uVar5 = DAT_00217358;
  lVar4 = DAT_0020d0f8;
  lVar15 = 0;
  if (iVar7 == 0) {
    lVar15 = local_2c8[0].tv_sec * 1000000 + (ulong)local_2c8[0].tv_nsec / 1000;
  }
  if (param_4 == 0) {
    uVar12 = 1;
    uVar10 = 0;
LAB_00161274:
    iVar7 = clock_gettime(1,local_2c8);
    if (iVar7 == 0) {
      lVar14 = local_2c8[0].tv_sec * 1000000 + (ulong)local_2c8[0].tv_nsec / 1000;
    }
    else {
      lVar14 = 0;
    }
    DAT_0021733c = '\0';
    DAT_00217398 = (DAT_0020d0f8 - lVar4) + DAT_00217398;
    DAT_00217350 = DAT_00217390 + (lVar14 - lVar15);
    DAT_00217340 = 0;
    DAT_00217390 = DAT_00217350;
    if (((local_2d0 != DAT_00215d48) || (local_2d8 != DAT_00217360)) ||
       ((local_2e0._4_4_ != DAT_0020f7f0 || ((int)local_2e0 != DAT_0020f7f4)))) {
      DAT_0020d15c = 0;
    }
  }
  else {
    uVar8 = FUN_001428fc(iVar7,param_1 + 0xf8,&local_2d0,8);
    uVar10 = 0;
    if ((int)uVar8 == 0) {
      uVar12 = 2;
      goto LAB_00161274;
    }
    uVar12 = 2;
    if (((0xfffffffffffedfff < local_2d0 - 0x10000) || ((local_2d0 & 7) != 0)) ||
       (local_2d0 != param_2)) goto LAB_00161274;
    uVar8 = FUN_001428fc(uVar8,param_2 + 0x20,&local_2d8,8);
    uVar10 = 0;
    if ((int)uVar8 == 0) {
      uVar12 = 3;
      goto LAB_00161274;
    }
    uVar12 = 3;
    if ((local_2d8 + 0x2000 < 0x12000) || ((local_2d8 & 7) != 0)) goto LAB_00161274;
    uVar8 = FUN_001428fc(uVar8,local_2d0 + 0xc4,(long)&local_2e0 + 4,4);
    if ((int)uVar8 == 0) {
      uVar10 = 0;
      goto LAB_00161274;
    }
    uVar8 = FUN_001428fc(uVar8,local_2d0 + 200,&local_2e0,4);
    uVar10 = 0;
    if ((((int)uVar8 == 0) || (local_2e0._4_4_ < 8)) ||
       (((int)local_2e0 < 8 || ((0x78 < local_2e0._4_4_ || (0x78 < (int)local_2e0))))))
    goto LAB_00161274;
    if (local_2d0 == DAT_00215d48) {
      bVar6 = true;
      if ((local_2d8 == DAT_00217360) && (local_2e0._4_4_ == DAT_0020f7f0)) {
        bVar6 = (int)local_2e0 != DAT_0020f7f4;
      }
    }
    else {
      bVar6 = true;
    }
    if (DAT_0021733c == '\x01') {
      if ((((local_2d0 != DAT_00217368) || (local_2d8 != DAT_00217370)) ||
          (local_2e0._4_4_ != DAT_00217378)) || ((int)local_2e0 != DAT_0021737c)) {
        DAT_0021733c = '\0';
        DAT_00217340 = 0;
        goto LAB_001614ec;
      }
    }
    else {
LAB_001614ec:
      if ((((bVar6 == false) && (DAT_0020d15c != 0)) && ((uint)(param_3 - DAT_00217380) < 0x1e)) ||
         ((((bVar6 | DAT_0020d15c) != 1 && (DAT_00217338 != 0)) &&
          ((uint)(param_3 - DAT_00217338) < 5)))) goto LAB_0016144c;
      if (bVar6 != false) {
        DAT_0020d15c = 0;
      }
      DAT_00217368 = local_2d0;
      DAT_00217384 = (int)local_2e0 * local_2e0._4_4_;
      DAT_00217370 = local_2d8;
      DAT_00217378 = local_2e0._4_4_;
      DAT_0021737c = (int)local_2e0;
      DAT_00217340 = 0;
      DAT_00217388 = -1;
      DAT_00217390 = 0;
      DAT_00217398 = 0;
      DAT_0021733c = '\x01';
      DAT_00217338 = param_3;
    }
    if (DAT_00217388 == param_3) goto LAB_0016144c;
    uVar11 = (ulong)DAT_00217340;
    uVar1 = DAT_00217340 + 0x200;
    if (DAT_00217384 <= DAT_00217340 + 0x200) {
      uVar1 = DAT_00217384;
    }
    DAT_00217388 = param_3;
    if (DAT_00217340 < uVar1) {
      lVar14 = uVar11 << 3;
      lVar9 = uVar1 - uVar11;
      pbVar13 = &DAT_002173a0 + uVar11;
      do {
        uVar10 = (uint)uVar11;
        uVar8 = FUN_001428fc(uVar8,lVar14 + local_2d8,local_2c8,8);
        if ((int)uVar8 == 0) {
          uVar12 = 4;
          goto LAB_00161274;
        }
        uVar12 = 4;
        if ((local_2c8[0].tv_sec + 0x2000U < 0x12000) || ((local_2c8[0].tv_sec & 7U) != 0))
        goto LAB_00161274;
        uVar8 = FUN_001428fc(uVar8,local_2c8[0].tv_sec,&local_2e8,8);
        if ((int)uVar8 == 0) {
          uVar12 = 5;
          goto LAB_00161274;
        }
        uVar12 = 5;
        if ((local_2e8 + 0x2000 < 0x12000) || ((local_2e8 & 7) != 0)) goto LAB_00161274;
        uVar8 = FUN_001428fc(uVar8,local_2e8 + 0x54,&local_2ec,4);
        if ((int)uVar8 == 0) {
          uVar12 = 6;
          goto LAB_00161274;
        }
        uVar11 = (ulong)(uVar10 + 1);
        lVar14 = lVar14 + 8;
        *pbVar13 = ((local_2ec & 0xff000000) != 0) << 6 | ((local_2ec & 0xff0000) != 0) << 7;
        lVar9 = lVar9 + -1;
        pbVar13 = pbVar13 + 1;
      } while (lVar9 != 0);
    }
    else {
      uVar10 = 0;
    }
    DAT_00217340 = uVar1;
    lVar14 = FUN_0015b4a0();
    DAT_00217390 = (lVar14 - lVar15) + DAT_00217390;
    DAT_00217398 = (DAT_0020d0f8 - lVar4) + DAT_00217398;
    if (DAT_00217340 < DAT_00217384) goto LAB_0016144c;
    uVar8 = FUN_0013a78c(local_2d0 + 0x20,&local_2f8);
    if ((((((int)uVar8 == 0) || (local_2f8 != local_2d8)) ||
         (uVar8 = FUN_001428fc(uVar8,local_2d0 + 0xc4,&local_2fc,4), (int)uVar8 == 0)) ||
        ((local_2fc != local_2e0._4_4_ ||
         (iVar7 = FUN_001428fc(uVar8,local_2d0 + 200,&local_300,4), iVar7 == 0)))) ||
       (local_300 != (int)local_2e0)) {
      uVar12 = 7;
      goto LAB_00161274;
    }
    memcpy(&DAT_0020f7f8,&DAT_002173a0,(ulong)DAT_00217384);
    DAT_00217360 = local_2d8;
    DAT_00215d48 = local_2d0;
    DAT_0020f7f0 = local_2e0._4_4_;
    DAT_0020f7f4 = local_300;
    DAT_0020d15c = 1;
    DAT_00217350 = DAT_00217390;
    DAT_0021733c = '\0';
    DAT_00217380 = param_3;
    FUN_0018e7f0();
    uVar12 = 0;
  }
  DAT_0021abe0 = DAT_0021abe0 + 1;
  DAT_00217358 = uVar12;
  DAT_0021735c = uVar10;
  iVar7 = clock_gettime(1,local_2c8);
  if (iVar7 == 0) {
    lVar15 = (ulong)local_2c8[0].tv_nsec / 1000000 + local_2c8[0].tv_sec * 1000;
  }
  else {
    lVar15 = 0;
  }
  if ((((DAT_0021abe0 < 5) || (uVar12 != uVar5)) || (DAT_0021abe8 == 0)) ||
     (999 < (ulong)(lVar15 - DAT_0021abe8))) {
    snprintf((char *)local_2c8,600,
             ",\"map_known\":%u,\"failure_stage\":%u,\"failure_index\":%u,\"width\":%d,\"height\":%d,\"map\":\"0x%llx\",\"tiles\":\"0x%llx\",\"refresh_us\":%llu,\"refresh_reads\":%llu,\"refresh_attempts\":%llu"
             ,(ulong)DAT_0020d15c,(ulong)uVar12,(ulong)uVar10,local_2e0 >> 0x20,
             local_2e0 & 0xffffffff,local_2d0,local_2d8,DAT_00217350,DAT_00217398,DAT_0021abe0);
    pcVar2 = "complete_source_wall_cache";
    if (uVar12 != 0) {
      pcVar2 = "current_map_unknown";
    }
    FUN_001417c8("function_map",pcVar2,local_2c8);
    DAT_0021abe8 = lVar15;
  }
LAB_0016144c:
  if (*(long *)(lVar3 + 0x28) == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00165c60 @ 00165c60 [libNexusEvasionRuntime69252.so] ===== */

undefined4 FUN_00165c60(undefined8 param_1)

{
  ulong uVar1;
  long lVar2;
  int iVar3;
  undefined8 uVar4;
  ulong uVar5;
  ulong uVar6;
  float fVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  ulong local_c8;
  undefined8 local_c0;
  undefined1 auStack_b8 [20];
  char acStack_a4 [76];
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  uVar4 = FUN_00165d8c(param_1,&local_c0);
  uVar9 = 0;
  if (((((int)uVar4 != 0) && (iVar3 = FUN_001428fc(uVar4,local_c0,&local_c8,8), iVar3 != 0)) &&
      (0x11fff < local_c8 + 0x2000)) &&
     (((local_c8 & 7) == 0 &&
      (iVar3 = FUN_001824d8(DAT_001e0978,local_c8,FUN_001428fc,0,auStack_b8), iVar3 != 0)))) {
    uVar5 = 0;
    uVar6 = 0xf6;
    do {
      uVar1 = uVar5 + (uVar6 - uVar5 >> 1);
      iVar3 = strcmp(acStack_a4,(&PTR_s_AlternatorWeaponDamage_001c3388)[uVar1 * 2]);
      if (iVar3 == 0) {
        fVar7 = (float)NEON_ucvtf(*(undefined4 *)(&UNK_001c3390 + uVar1 * 0x10));
        uVar8 = NEON_fmin(fVar7 / 3.0,0x41a00000);
        uVar9 = 0x3f800000;
        if (1.0 <= fVar7 / 3.0) {
          uVar9 = uVar8;
        }
        break;
      }
      if (-1 < iVar3) {
        uVar5 = uVar1 + 1;
        uVar1 = uVar6;
      }
      uVar6 = uVar1;
    } while (uVar5 < uVar6);
  }
  if (*(long *)(lVar2 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return uVar9;
}

/* ===== FUN_0016acc0 @ 0016acc0 [libNexusEvasionRuntime69252.so] ===== */

void FUN_0016acc0(undefined8 param_1,ulong param_2,ulong *param_3)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  FILE *__stream;
  char *pcVar4;
  size_t sVar5;
  int iVar6;
  ulong uVar7;
  uint uVar8;
  ulong local_1190;
  ulong uStack_1188;
  ulong uStack_1180;
  ulong uStack_1178;
  ulong local_1170;
  ulong uStack_1168;
  ulong local_1160;
  ulong uStack_1158;
  ulong uStack_1150;
  ulong uStack_1148;
  ulong local_1140;
  undefined8 uStack_1138;
  char acStack_1130 [4288];
  long local_70;
  
  lVar1 = tpidr_el0;
  __stream = (FILE *)0x0;
  local_70 = *(long *)(lVar1 + 0x28);
  if (((param_3 != (ulong *)0x0) && (DAT_001e0978 + 0x123e0c8U == param_2)) &&
     (__stream = fopen("/proc/self/maps","re"), __stream != (FILE *)0x0)) {
    pcVar4 = fgets(acStack_1130,0x10c0,__stream);
    if (pcVar4 == (char *)0x0) {
      iVar6 = 0;
    }
    else {
      uVar7 = 0;
      uVar8 = 0;
      iVar6 = 0;
      bVar2 = true;
      do {
        sVar5 = strlen(acStack_1130);
        if ((uVar8 >> 0xf != 0) || (uVar7 = sVar5 + uVar7, 0x800000 < uVar7 || sVar5 == 0))
        goto LAB_0016ae0c;
        if (acStack_1130[sVar5 - 1] != '\n') {
          bVar2 = true;
          goto LAB_0016ae0c;
        }
        iVar3 = FUN_001986c8(acStack_1130,&DAT_00209d98,&local_1190);
        if (((iVar3 != 0) && (local_1190 <= param_2)) && (param_2 + 8 <= uStack_1188)) {
          uStack_1158 = uStack_1188;
          local_1160 = local_1190;
          uStack_1148 = uStack_1178;
          uStack_1150 = uStack_1180;
          iVar6 = iVar6 + 1;
          uStack_1138 = uStack_1168;
          local_1140 = local_1170;
        }
        pcVar4 = fgets(acStack_1130,0x10c0,__stream);
        uVar8 = uVar8 + 1;
      } while (pcVar4 != (char *)0x0);
    }
    bVar2 = false;
LAB_0016ae0c:
    iVar3 = ferror(__stream);
    fclose(__stream);
    __stream = (FILE *)0x0;
    if (((!bVar2) && (iVar3 == 0)) && (iVar6 == 1)) {
      __stream = (FILE *)0x1;
      param_3[1] = uStack_1158;
      *param_3 = local_1160;
      param_3[3] = uStack_1148;
      param_3[2] = uStack_1150;
      param_3[5] = uStack_1138;
      param_3[4] = local_1140;
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(__stream);
}

/* ===== FUN_00182610 @ 00182610 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00182610(ulong *param_1,long param_2,undefined4 *param_3)

{
  long lVar1;
  undefined1 auVar2 [16];
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  undefined1 auVar6 [16];
  undefined1 auVar7 [16];
  float fVar8;
  undefined1 auVar9 [16];
  undefined1 auVar10 [16];
  undefined1 auVar11 [16];
  undefined1 auVar12 [16];
  bool bVar13;
  bool bVar14;
  int iVar15;
  char *pcVar16;
  char *pcVar17;
  uint uVar18;
  ulong uVar19;
  long lVar20;
  ulong uVar21;
  ulong *puVar22;
  ulong uVar23;
  uint uVar24;
  undefined1 (*pauVar25) [16];
  ulong uVar26;
  byte bVar27;
  undefined1 uVar28;
  undefined1 uVar29;
  undefined1 uVar30;
  undefined1 uVar31;
  undefined1 uVar32;
  undefined1 uVar33;
  undefined1 uVar34;
  undefined1 uVar35;
  byte bVar36;
  byte bVar37;
  byte bVar38;
  byte bVar39;
  byte bVar40;
  byte bVar41;
  byte bVar42;
  uint5 uVar43;
  undefined1 auVar44 [16];
  uint5 uVar45;
  undefined1 auVar46 [16];
  undefined1 auVar47 [16];
  undefined1 auVar48 [16];
  undefined1 auVar49 [16];
  ulong local_1a0;
  uint local_198;
  uint local_18c;
  byte local_188 [4];
  byte local_184 [4];
  byte local_180 [4];
  uint local_17c;
  ulong local_178;
  char local_16c [4];
  long local_168;
  int local_160;
  int local_15c;
  ulong local_158;
  ulong local_150;
  ulong local_148;
  ulong local_140;
  ulong local_138;
  ulong local_130;
  ulong local_128 [4];
  undefined1 auStack_108 [96];
  ulong local_a8;
  int local_a0;
  uint local_9c;
  ulong local_98;
  int local_90;
  uint local_8c;
  ulong local_88;
  undefined1 auStack_80 [4];
  uint local_7c;
  ulong local_78;
  long local_70;
  
  lVar1 = tpidr_el0;
  local_70 = *(long *)(lVar1 + 0x28);
  if ((param_2 + 8U < 0x10008) ||
     (iVar15 = (*(code *)param_1[1])(param_1[2],param_2,&local_130,8), iVar15 != 1)) {
LAB_00182898:
    pcVar16 = (char *)0x0;
    goto LAB_0018289c;
  }
  pcVar16 = (char *)0x0;
  if ((local_130 + 0x1000 < 0x11000) ||
     ((((local_130 & 7) != 0 || (pcVar16 = (char *)0x0, param_2 + 0x10U < 0x10008)) ||
      (local_130 != *param_1 + 0x121f350)))) goto LAB_0018289c;
  iVar15 = (*(code *)param_1[1])(param_1[2],param_2 + 8U,&local_138,8);
  if (iVar15 != 1) goto LAB_00182898;
  pcVar16 = (char *)0x0;
  if ((local_138 + 0x1000 < 0x11000) || ((local_138 & 7) != 0)) goto LAB_0018289c;
  iVar15 = (*(code *)param_1[1])(param_1[2],local_138 + 8,&local_15c,4);
  pcVar16 = (char *)0x0;
  if ((iVar15 != 1) || ((local_15c < 0 || (0x2c8 < local_15c)))) goto LAB_0018289c;
  if ((local_138 + 8 < 0x10008) ||
     (iVar15 = (*(code *)param_1[1])(param_1[2],local_138,&local_140,8), iVar15 != 1))
  goto LAB_00182898;
  pcVar16 = (char *)0x0;
  if ((local_140 + 0x1000 < 0x11000) || ((local_140 & 7) != 0)) goto LAB_0018289c;
  iVar15 = (*(code *)param_1[1])(param_1[2],local_140 + 0x38,&local_148,8);
  if (iVar15 != 1) goto LAB_00182898;
  pcVar16 = (char *)0x0;
  if ((local_148 + 0x1000 < 0x11000) || ((local_148 & 7) != 0)) goto LAB_0018289c;
  iVar15 = (*(code *)param_1[1])(param_1[2],local_148,&local_150,8);
  if (iVar15 != 1) goto LAB_00182898;
  pcVar16 = (char *)0x0;
  if ((local_150 + 0x1000 < 0x11000) || ((local_150 & 7) != 0)) goto LAB_0018289c;
  iVar15 = (*(code *)param_1[1])(param_1[2],local_150 + 8,&local_158,8);
  if (iVar15 != 1) goto LAB_00182918;
  pcVar16 = (char *)0x0;
  if ((local_158 + 0x1000 < 0x11000) || ((local_158 & 7) != 0)) goto LAB_0018289c;
  if ((local_158 <= (long)local_15c * -0x10 - 0x11U) &&
     (((lVar20 = (long)local_15c * 0x10 + local_158, 0x1000f < lVar20 + 0x10U &&
       (iVar15 = (*(code *)param_1[1])(param_1[2],lVar20,auStack_80,0x10), iVar15 == 1)) &&
      (uVar26 = (ulong)local_7c, 0xffffffc6 < local_7c - 0x40)))) {
    if (local_7c < 8) {
      uVar26 = 7;
      *(ulong *)(param_3 + 5) = local_78;
    }
    else {
      pcVar16 = (char *)0x0;
      if ((local_78 < 0x10000) || (~local_78 <= uVar26)) goto LAB_0018289c;
      iVar15 = (*(code *)param_1[1])(param_1[2],local_78,param_3 + 5,uVar26 + 1);
      if (iVar15 != 1) goto LAB_00182918;
    }
    pcVar17 = (char *)(param_3 + 5);
    if (pcVar17[uVar26] == '\0') {
      uVar19 = 0;
      do {
        bVar27 = *(byte *)((long)param_3 + uVar19 + 0x14);
        if ((0x19 < (bVar27 & 0xffffffdf) - 0x41) && (bVar27 != 0x5f && 9 < bVar27 - 0x30))
        goto LAB_00182918;
        uVar19 = uVar19 + 1;
      } while (uVar26 != uVar19);
      pcVar16 = strstr(pcVar17,"Weapon");
      if ((pcVar16 == (char *)0x0) ||
         (pcVar17 = (char *)FUN_001830e8(pcVar17), pcVar16 = pcVar17, pcVar17 == (char *)0x0))
      goto LAB_0018289c;
      pcVar16 = (char *)0x0;
      uVar26 = *param_1 + 0x1244c3c;
      if ((uVar26 < 0x10000) || ((*param_1 & 0xfffffffffffffffc) == 0xfffffffffedbb3c0))
      goto LAB_0018289c;
      iVar15 = (*(code *)param_1[1])(param_1[2],uVar26,&local_160,4);
      pcVar16 = (char *)0x0;
      if ((((iVar15 != 1) || (local_160 != 0x34)) ||
          (pcVar16 = (char *)FUN_0018307c(param_1,local_148 + 0x1a0,&local_150), (int)pcVar16 == 0))
         || (pcVar16 = (char *)FUN_0018307c(param_1,local_150 + 0x28,&local_168), (int)pcVar16 == 0)
         ) goto LAB_0018289c;
      if ((((0x10000 < local_168 + local_15c + 1U) &&
           (iVar15 = (*(code *)param_1[1])(param_1[2],local_168 + local_15c,local_16c,1),
           iVar15 == 1)) && (0x10027 < param_2 + 0x80U)) &&
         ((iVar15 = (*(code *)param_1[1])(param_1[2],param_2 + 0x58,&local_a8,0x28), iVar15 == 1 &&
          (local_9c < 0x11)))) {
        pcVar16 = (char *)0x0;
        if ((local_a0 < (int)local_9c) || (0x400 < local_a0)) goto LAB_0018289c;
        if (local_9c == 0) {
          local_18c = 0;
        }
        else {
          pcVar16 = (char *)0x0;
          if ((local_a8 < 0x10000) || ((local_a8 & 7) != 0)) goto LAB_0018289c;
          if (CARRY8(local_a8,(ulong)local_9c << 3)) goto LAB_00182918;
          uVar26 = 0;
          local_18c = 0;
          do {
            pcVar16 = (char *)FUN_0018307c(param_1,local_a8 + uVar26 * 8,&local_178);
            if ((int)pcVar16 == 0) goto LAB_0018289c;
            if (local_18c == 0) {
LAB_00182ad8:
              local_128[local_18c] = local_178;
              local_18c = local_18c + 1;
            }
            else {
              uVar19 = (ulong)local_18c;
              if (local_18c < 8) {
                uVar21 = 0;
                uVar24 = 0;
LAB_00182bac:
                lVar20 = uVar19 - uVar21;
                puVar22 = local_128 + uVar21;
                do {
                  lVar20 = lVar20 + -1;
                  uVar24 = uVar24 | *puVar22 == local_178;
                  puVar22 = puVar22 + 1;
                } while (lVar20 != 0);
              }
              else {
                uVar21 = uVar19 & 0xfffffff8;
                bVar27 = 0;
                bVar36 = 0;
                bVar37 = 0;
                bVar38 = 0;
                bVar39 = 0;
                bVar40 = 0;
                bVar41 = 0;
                bVar42 = 0;
                uVar28 = (undefined1)local_178;
                uVar30 = (undefined1)(local_178 >> 8);
                uVar32 = (undefined1)(local_178 >> 0x10);
                uVar34 = (undefined1)(local_178 >> 0x18);
                uVar29 = (undefined1)(local_178 >> 0x20);
                uVar31 = (undefined1)(local_178 >> 0x28);
                uVar33 = (undefined1)(local_178 >> 0x30);
                uVar35 = (undefined1)(local_178 >> 0x38);
                uVar23 = uVar21;
                pauVar25 = (undefined1 (*) [16])auStack_108;
                do {
                  uVar23 = uVar23 - 8;
                  auVar46[8] = uVar28;
                  auVar46._0_8_ = local_178;
                  auVar46[9] = uVar30;
                  auVar46[10] = uVar32;
                  auVar46[0xb] = uVar34;
                  auVar46[0xc] = uVar29;
                  auVar46[0xd] = uVar31;
                  auVar46[0xe] = uVar33;
                  auVar46[0xf] = uVar35;
                  auVar46 = NEON_cmeq(pauVar25[-2],auVar46,8);
                  auVar47[8] = uVar28;
                  auVar47._0_8_ = local_178;
                  auVar47[9] = uVar30;
                  auVar47[10] = uVar32;
                  auVar47[0xb] = uVar34;
                  auVar47[0xc] = uVar29;
                  auVar47[0xd] = uVar31;
                  auVar47[0xe] = uVar33;
                  auVar47[0xf] = uVar35;
                  auVar47 = NEON_cmeq(pauVar25[-1],auVar47,8);
                  auVar48[8] = uVar28;
                  auVar48._0_8_ = local_178;
                  auVar48[9] = uVar30;
                  auVar48[10] = uVar32;
                  auVar48[0xb] = uVar34;
                  auVar48[0xc] = uVar29;
                  auVar48[0xd] = uVar31;
                  auVar48[0xe] = uVar33;
                  auVar48[0xf] = uVar35;
                  auVar48 = NEON_cmeq(*pauVar25,auVar48,8);
                  auVar49[8] = uVar28;
                  auVar49._0_8_ = local_178;
                  auVar49[9] = uVar30;
                  auVar49[10] = uVar32;
                  auVar49[0xb] = uVar34;
                  auVar49[0xc] = uVar29;
                  auVar49[0xd] = uVar31;
                  auVar49[0xe] = uVar33;
                  auVar49[0xf] = uVar35;
                  auVar49 = NEON_cmeq(pauVar25[1],auVar49,8);
                  uVar43 = CONCAT14(auVar46[8],(uint)(auVar46[0] & 1)) & 0x100ffffff;
                  uVar45 = CONCAT14(auVar48[8],(uint)(auVar48[0] & 1)) & 0x100ffffff;
                  bVar27 = bVar27 | (byte)uVar43;
                  bVar36 = bVar36 | (byte)(uVar43 >> 0x20);
                  bVar37 = bVar37 | auVar47[0] & 1;
                  bVar38 = bVar38 | auVar47[8] & 1;
                  bVar39 = bVar39 | (byte)uVar45;
                  bVar40 = bVar40 | (byte)(uVar45 >> 0x20);
                  bVar41 = bVar41 | auVar49[0] & 1;
                  bVar42 = bVar42 | auVar49[8] & 1;
                  pauVar25 = pauVar25 + 4;
                } while (uVar23 != 0);
                bVar39 = bVar39 | bVar27;
                bVar40 = bVar40 | bVar36;
                auVar6._1_3_ = 0;
                auVar6[0] = bVar39;
                auVar6[4] = bVar40;
                auVar6._5_3_ = 0;
                auVar6[8] = bVar41 | bVar37;
                auVar6._9_3_ = 0;
                auVar6[0xc] = bVar42 | bVar38;
                auVar6._13_3_ = 0;
                auVar7._1_3_ = 0;
                auVar7[0] = bVar39;
                auVar7[4] = bVar40;
                auVar7._5_3_ = 0;
                auVar7[8] = bVar41 | bVar37;
                auVar7._9_3_ = 0;
                auVar7[0xc] = bVar42 | bVar38;
                auVar7._13_3_ = 0;
                auVar46 = NEON_ext(auVar6,auVar7,8,1);
                uVar24 = CONCAT13(auVar46[3],
                                  CONCAT12(auVar46[2],CONCAT11(auVar46[1],bVar39 | auVar46[0]))) |
                         CONCAT13(auVar46[7],
                                  CONCAT12(auVar46[6],CONCAT11(auVar46[5],bVar40 | auVar46[4])));
                if (uVar21 != uVar19) goto LAB_00182bac;
              }
              if (uVar24 == 0) {
                if (local_18c != 0x10) goto LAB_00182ad8;
                goto LAB_00182918;
              }
            }
            uVar26 = uVar26 + 1;
          } while (uVar26 != local_9c);
        }
        if (local_8c < 0x11) {
          pcVar16 = (char *)0x0;
          if ((local_90 < (int)local_8c) || (0x400 < local_90)) goto LAB_0018289c;
          if (local_8c != 0) {
            pcVar16 = (char *)0x0;
            if ((local_98 < 0x10000) || ((local_98 & 7) != 0)) goto LAB_0018289c;
            if (CARRY8(local_98,(ulong)local_8c << 3)) goto LAB_00182918;
            uVar26 = 0;
            do {
              pcVar16 = (char *)FUN_0018307c(param_1,local_98 + uVar26 * 8,&local_178);
              if ((int)pcVar16 == 0) goto LAB_0018289c;
              if (local_18c == 0) {
LAB_00182c44:
                local_128[local_18c] = local_178;
                local_18c = local_18c + 1;
              }
              else {
                uVar19 = (ulong)local_18c;
                if (local_18c < 8) {
                  uVar21 = 0;
                  uVar24 = 0;
LAB_00182d18:
                  lVar20 = uVar19 - uVar21;
                  puVar22 = local_128 + uVar21;
                  do {
                    lVar20 = lVar20 + -1;
                    uVar24 = uVar24 | *puVar22 == local_178;
                    puVar22 = puVar22 + 1;
                  } while (lVar20 != 0);
                }
                else {
                  uVar21 = uVar19 & 0xfffffff8;
                  bVar27 = 0;
                  bVar36 = 0;
                  bVar37 = 0;
                  bVar38 = 0;
                  bVar39 = 0;
                  bVar40 = 0;
                  bVar41 = 0;
                  bVar42 = 0;
                  uVar28 = (undefined1)local_178;
                  uVar30 = (undefined1)(local_178 >> 8);
                  uVar32 = (undefined1)(local_178 >> 0x10);
                  uVar34 = (undefined1)(local_178 >> 0x18);
                  uVar29 = (undefined1)(local_178 >> 0x20);
                  uVar31 = (undefined1)(local_178 >> 0x28);
                  uVar33 = (undefined1)(local_178 >> 0x30);
                  uVar35 = (undefined1)(local_178 >> 0x38);
                  uVar23 = uVar21;
                  pauVar25 = (undefined1 (*) [16])auStack_108;
                  do {
                    uVar23 = uVar23 - 8;
                    auVar9[8] = uVar28;
                    auVar9._0_8_ = local_178;
                    auVar9[9] = uVar30;
                    auVar9[10] = uVar32;
                    auVar9[0xb] = uVar34;
                    auVar9[0xc] = uVar29;
                    auVar9[0xd] = uVar31;
                    auVar9[0xe] = uVar33;
                    auVar9[0xf] = uVar35;
                    auVar46 = NEON_cmeq(pauVar25[-2],auVar9,8);
                    auVar10[8] = uVar28;
                    auVar10._0_8_ = local_178;
                    auVar10[9] = uVar30;
                    auVar10[10] = uVar32;
                    auVar10[0xb] = uVar34;
                    auVar10[0xc] = uVar29;
                    auVar10[0xd] = uVar31;
                    auVar10[0xe] = uVar33;
                    auVar10[0xf] = uVar35;
                    auVar47 = NEON_cmeq(pauVar25[-1],auVar10,8);
                    auVar11[8] = uVar28;
                    auVar11._0_8_ = local_178;
                    auVar11[9] = uVar30;
                    auVar11[10] = uVar32;
                    auVar11[0xb] = uVar34;
                    auVar11[0xc] = uVar29;
                    auVar11[0xd] = uVar31;
                    auVar11[0xe] = uVar33;
                    auVar11[0xf] = uVar35;
                    auVar48 = NEON_cmeq(*pauVar25,auVar11,8);
                    auVar12[8] = uVar28;
                    auVar12._0_8_ = local_178;
                    auVar12[9] = uVar30;
                    auVar12[10] = uVar32;
                    auVar12[0xb] = uVar34;
                    auVar12[0xc] = uVar29;
                    auVar12[0xd] = uVar31;
                    auVar12[0xe] = uVar33;
                    auVar12[0xf] = uVar35;
                    auVar49 = NEON_cmeq(pauVar25[1],auVar12,8);
                    uVar43 = CONCAT14(auVar46[8],(uint)(auVar46[0] & 1)) & 0x100ffffff;
                    uVar45 = CONCAT14(auVar48[8],(uint)(auVar48[0] & 1)) & 0x100ffffff;
                    bVar27 = bVar27 | (byte)uVar43;
                    bVar36 = bVar36 | (byte)(uVar43 >> 0x20);
                    bVar37 = bVar37 | auVar47[0] & 1;
                    bVar38 = bVar38 | auVar47[8] & 1;
                    bVar39 = bVar39 | (byte)uVar45;
                    bVar40 = bVar40 | (byte)(uVar45 >> 0x20);
                    bVar41 = bVar41 | auVar49[0] & 1;
                    bVar42 = bVar42 | auVar49[8] & 1;
                    pauVar25 = pauVar25 + 4;
                  } while (uVar23 != 0);
                  bVar39 = bVar39 | bVar27;
                  bVar40 = bVar40 | bVar36;
                  auVar4._1_3_ = 0;
                  auVar4[0] = bVar39;
                  auVar4[4] = bVar40;
                  auVar4._5_3_ = 0;
                  auVar4[8] = bVar41 | bVar37;
                  auVar4._9_3_ = 0;
                  auVar4[0xc] = bVar42 | bVar38;
                  auVar4._13_3_ = 0;
                  auVar5._1_3_ = 0;
                  auVar5[0] = bVar39;
                  auVar5[4] = bVar40;
                  auVar5._5_3_ = 0;
                  auVar5[8] = bVar41 | bVar37;
                  auVar5._9_3_ = 0;
                  auVar5[0xc] = bVar42 | bVar38;
                  auVar5._13_3_ = 0;
                  auVar46 = NEON_ext(auVar4,auVar5,8,1);
                  uVar24 = CONCAT13(auVar46[3],
                                    CONCAT12(auVar46[2],CONCAT11(auVar46[1],bVar39 | auVar46[0]))) |
                           CONCAT13(auVar46[7],
                                    CONCAT12(auVar46[6],CONCAT11(auVar46[5],bVar40 | auVar46[4])));
                  if (uVar21 != uVar19) goto LAB_00182d18;
                }
                if (uVar24 == 0) {
                  if (local_18c != 0x10) goto LAB_00182c44;
                  goto LAB_00182918;
                }
              }
              uVar26 = uVar26 + 1;
            } while (uVar26 != local_8c);
          }
          local_178 = local_88;
          if (local_88 != 0) {
            if (local_88 < 0x10000) goto LAB_00182918;
            pcVar16 = (char *)0x0;
            if ((0xffffffffffffefff < local_88) || ((local_88 & 7) != 0)) goto LAB_0018289c;
            if (local_18c != 0) {
              uVar26 = (ulong)local_18c;
              if (local_18c < 8) {
                uVar23 = 0;
                uVar24 = 0;
LAB_00182e18:
                lVar20 = uVar26 - uVar23;
                puVar22 = local_128 + uVar23;
                do {
                  lVar20 = lVar20 + -1;
                  uVar24 = uVar24 | *puVar22 == local_88;
                  puVar22 = puVar22 + 1;
                } while (lVar20 != 0);
              }
              else {
                uVar23 = uVar26 & 0xfffffff8;
                bVar27 = 0;
                bVar36 = 0;
                bVar37 = 0;
                bVar38 = 0;
                bVar39 = 0;
                bVar40 = 0;
                bVar41 = 0;
                bVar42 = 0;
                auVar44._8_8_ = local_88;
                auVar44._0_8_ = local_88;
                pauVar25 = (undefined1 (*) [16])auStack_108;
                uVar19 = uVar23;
                do {
                  uVar19 = uVar19 - 8;
                  auVar46 = NEON_cmeq(pauVar25[-2],auVar44,8);
                  auVar47 = NEON_cmeq(pauVar25[-1],auVar44,8);
                  auVar48 = NEON_cmeq(*pauVar25,auVar44,8);
                  auVar49 = NEON_cmeq(pauVar25[1],auVar44,8);
                  uVar43 = CONCAT14(auVar46[8],(uint)(auVar46[0] & 1)) & 0x100ffffff;
                  bVar27 = bVar27 | (byte)uVar43;
                  bVar36 = bVar36 | (byte)(uVar43 >> 0x20);
                  bVar37 = bVar37 | auVar47[0] & 1;
                  bVar38 = bVar38 | auVar47[8] & 1;
                  uVar43 = CONCAT14(auVar48[8],(uint)(auVar48[0] & 1)) & 0x100ffffff;
                  bVar39 = bVar39 | (byte)uVar43;
                  bVar40 = bVar40 | (byte)(uVar43 >> 0x20);
                  bVar41 = bVar41 | auVar49[0] & 1;
                  bVar42 = bVar42 | auVar49[8] & 1;
                  pauVar25 = pauVar25 + 4;
                } while (uVar19 != 0);
                bVar39 = bVar39 | bVar27;
                bVar40 = bVar40 | bVar36;
                auVar2._1_3_ = 0;
                auVar2[0] = bVar39;
                auVar2[4] = bVar40;
                auVar2._5_3_ = 0;
                auVar2[8] = bVar41 | bVar37;
                auVar2._9_3_ = 0;
                auVar2[0xc] = bVar42 | bVar38;
                auVar2._13_3_ = 0;
                auVar3._1_3_ = 0;
                auVar3[0] = bVar39;
                auVar3[4] = bVar40;
                auVar3._5_3_ = 0;
                auVar3[8] = bVar41 | bVar37;
                auVar3._9_3_ = 0;
                auVar3[0xc] = bVar42 | bVar38;
                auVar3._13_3_ = 0;
                auVar46 = NEON_ext(auVar2,auVar3,8,1);
                uVar24 = CONCAT13(auVar46[3],
                                  CONCAT12(auVar46[2],CONCAT11(auVar46[1],bVar39 | auVar46[0]))) |
                         CONCAT13(auVar46[7],
                                  CONCAT12(auVar46[6],CONCAT11(auVar46[5],bVar40 | auVar46[4])));
                if (uVar23 != uVar26) goto LAB_00182e18;
              }
              if (uVar24 != 0) goto LAB_00182e60;
              if (local_18c == 0x10) goto LAB_00182918;
            }
            local_128[local_18c] = local_88;
            local_18c = local_18c + 1;
          }
LAB_00182e60:
          uVar24 = 0;
          local_198 = local_18c;
          if (local_18c != 0) {
            puVar22 = local_128;
            local_1a0 = (ulong)local_18c;
            local_198 = (uint)(local_18c != 0);
            do {
              uVar26 = *puVar22;
              iVar15 = FUN_0018307c(param_1,uVar26,&local_130);
              if (((((iVar15 == 0) || (local_130 != *param_1 + 0x121ea60)) ||
                   (uVar26 + 0x130 < 0x10000)) ||
                  ((((uVar26 & 0xfffffffffffffffc) == 0xfffffffffffffecc ||
                    (iVar15 = (*(code *)param_1[1])(param_1[2],uVar26 + 0x130,&local_17c,4),
                    uVar26 < 0xfef8)) ||
                   ((iVar15 != 1 || (((int)local_17c < 0 || (5000 < (int)local_17c)))))))) ||
                 ((iVar15 = (*(code *)param_1[1])(param_1[2],uVar26 + 0x108,local_180,1),
                  uVar26 + 0x1e6 < 0x10001 ||
                  ((iVar15 != 1 ||
                   (iVar15 = (*(code *)param_1[1])(param_1[2],uVar26 + 0x1e5,local_184,1),
                   iVar15 != 1)))))) goto LAB_00182918;
              if (uVar26 + 0x23d < 0x10001) {
                bVar14 = false;
              }
              else {
                iVar15 = (*(code *)param_1[1])(param_1[2],uVar26 + 0x23c,local_188,1);
                bVar14 = iVar15 == 1;
              }
              if ((((!bVar14) || (1 < local_180[0])) || (1 < local_184[0])) || (1 < local_188[0]))
              goto LAB_00182918;
              puVar22 = puVar22 + 1;
              uVar18 = local_17c;
              if (local_17c <= uVar24) {
                uVar18 = uVar24;
              }
              if ((local_180[0] == 0 && local_184[0] == 0) && local_188[0] == 0) {
                local_198 = 0;
                uVar24 = uVar18;
              }
              local_1a0 = local_1a0 - 1;
            } while (local_1a0 != 0);
          }
          uVar18 = (uint)(local_198 != 0);
          if (local_16c[0] == '\x01') {
            uVar18 = 1;
          }
          if ((uVar24 == *(ushort *)(pcVar17 + 8)) && ((byte)pcVar17[10] == uVar18)) {
            param_3[1] = uVar24;
            param_3[2] = uVar18;
            *(long *)(param_3 + 0x16) = param_2;
            *param_3 = 1;
            param_3[3] = local_18c;
            if (uVar24 == 0) {
              uVar28 = 0xec;
              uVar30 = 0x51;
              uVar32 = 0x38;
              uVar34 = 0x3e;
            }
            else {
              fVar8 = (float)uVar24 / 300.0;
              uVar28 = SUB41(fVar8,0);
              uVar30 = (undefined1)((uint)fVar8 >> 8);
              uVar32 = (undefined1)((uint)fVar8 >> 0x10);
              uVar34 = (undefined1)((uint)fVar8 >> 0x18);
            }
            pcVar16 = (char *)0x1;
            bVar14 = NAN((float)CONCAT13(uVar34,CONCAT12(uVar32,CONCAT11(uVar30,uVar28))));
            bVar13 = true;
            if ((!bVar14 && (float)CONCAT13(uVar34,CONCAT12(uVar32,CONCAT11(uVar30,uVar28))) == 1.25
                 || (float)CONCAT13(uVar34,CONCAT12(uVar32,CONCAT11(uVar30,uVar28))) < 1.25 !=
                    bVar14) &&
               (bVar13 = false,
               !NAN((float)CONCAT13(uVar34,CONCAT12(uVar32,CONCAT11(uVar30,uVar28)))))) {
              bVar13 = (float)CONCAT13(uVar34,CONCAT12(uVar32,CONCAT11(uVar30,uVar28))) < 0.08;
            }
            uVar29 = 0;
            uVar31 = 0;
            uVar33 = 0;
            uVar35 = 0x3f;
            if (!bVar13) {
              uVar29 = uVar28;
              uVar31 = uVar30;
              uVar33 = uVar32;
              uVar35 = uVar34;
            }
            param_3[4] = CONCAT13(uVar35,CONCAT12(uVar33,CONCAT11(uVar31,uVar29)));
            goto LAB_0018289c;
          }
        }
      }
    }
  }
LAB_00182918:
  pcVar16 = (char *)0x0;
LAB_0018289c:
  if (*(long *)(lVar1 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(pcVar16);
  }
  return;
}

/* ===== FUN_001830e8 @ 001830e8 [libNexusEvasionRuntime69252.so] ===== */

undefined8 * FUN_001830e8(char *param_1)

{
  ulong uVar1;
  ulong uVar2;
  int iVar3;
  ulong uVar4;
  ulong uVar5;
  
  uVar4 = 0;
  uVar5 = 0xf6;
  do {
    if (uVar5 < uVar4 || uVar5 - uVar4 == 0) {
      return (undefined8 *)0x0;
    }
    uVar1 = uVar4 + (uVar5 - uVar4 >> 1);
    iVar3 = strcmp(param_1,*(char **)(&UNK_001ca320 + uVar1 * 0x10));
    uVar2 = uVar1;
    if (-1 < iVar3) {
      uVar4 = uVar1 + 1;
      uVar2 = uVar5;
    }
    uVar5 = uVar2;
  } while (iVar3 != 0);
  return (undefined8 *)(&UNK_001ca320 + uVar1 * 0x10);
}

/* ===== FUN_0019df04 @ 0019df04 [libNexusEvasionRuntime69252.so] ===== */

undefined4 FUN_0019df04(long param_1,undefined8 *param_2,long param_3,long param_4)

{
  long lVar1;
  int iVar2;
  long *plVar3;
  undefined4 uVar4;
  long lVar5;
  ulong local_e0;
  long local_d8;
  long lStack_d0;
  char local_c8;
  long local_98;
  long lStack_90;
  long local_88;
  char local_80;
  undefined7 uStack_7f;
  int local_78;
  long local_58;
  
  lVar1 = tpidr_el0;
  uVar4 = 0;
  local_58 = *(long *)(lVar1 + 0x28);
  if ((((param_1 == 0) || (param_2 == (undefined8 *)0x0)) || (uVar4 = 0, param_3 + 8U < 0x10008)) ||
     ((code *)param_2[1] == (code *)0x0)) goto LAB_0019e0b4;
  iVar2 = (*(code *)param_2[1])(*param_2,param_3,&local_e0,8);
  if (iVar2 == 1) {
    uVar4 = 0;
    if ((local_e0 + 0x1001 < 0x11001) || ((local_e0 & 7) != 0)) goto LAB_0019e0b4;
    iVar2 = FUN_0019dbd4(param_2,param_3,&local_98);
    if (iVar2 == 0) goto LAB_0019e0b0;
    if ((local_98 == 0x6c6c6f72746e6f43 && lStack_90 == 0x725069746c557265) &&
        (local_88 == 0x656c697463656a6f && local_80 == '\0')) {
      lVar5 = 0;
LAB_0019e084:
      iVar2 = FUN_0019dbd4(param_2,param_4,&local_d8);
      if ((iVar2 == 0) ||
         (iVar2 = strcmp((char *)&local_d8,(&PTR_s_ControllerUltiProjectile_001cb280)[lVar5]),
         iVar2 != 0)) goto LAB_0019e0b0;
      *(undefined4 *)(param_1 + 0x38) = 0;
      plVar3 = (long *)(param_1 + lVar5 * 0x18);
      uVar4 = 1;
      *plVar3 = param_3;
      plVar3[1] = local_e0;
      plVar3[2] = param_4;
    }
    else {
      if ((((local_98 == 0x6c6c6f72746e6f43 && lStack_90 == 0x764f69746c557265) &&
           local_88 == 0x6567726168637265) && CONCAT71(uStack_7f,local_80) == 0x7463656a6f725064) &&
          local_78 == 0x656c69) {
        lVar5 = 1;
        goto LAB_0019e084;
      }
      uVar4 = 0;
    }
    if (((*(long *)(param_1 + 0x30) == 0) &&
        (iVar2 = FUN_0019dbd4(param_2,param_4,&local_d8), iVar2 != 0)) &&
       ((local_d8 == 0x7250796465657053 && lStack_d0 == 0x656c697463656a6f) && local_c8 == '\0')) {
      *(long *)(param_1 + 0x30) = param_4;
    }
  }
  else {
LAB_0019e0b0:
    uVar4 = 0;
  }
LAB_0019e0b4:
  if (*(long *)(lVar1 + 0x28) == local_58) {
    return uVar4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0019e158 @ 0019e158 [libNexusEvasionRuntime69252.so] ===== */

void FUN_0019e158(long *param_1,undefined8 *param_2,uint param_3,long param_4)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  uint uVar4;
  ulong uVar5;
  bool bVar6;
  char *__s2;
  ulong *puVar7;
  long lVar8;
  long *plVar9;
  ulong local_b8;
  ulong local_b0;
  long local_a8;
  long lStack_a0;
  char local_98;
  long local_68;
  
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  uVar5 = 0;
  if (((param_1 == (long *)0x0) || (param_2 == (undefined8 *)0x0)) || (param_4 == 0))
  goto LAB_0019e224;
  if ((param_3 < 2) && (*param_1 != 0)) {
    uVar5 = 0;
    if ((param_1[3] == 0) || (*param_1 == param_1[3])) goto LAB_0019e224;
    if (param_3 == 0) {
LAB_0019e254:
      lVar8 = 0;
      bVar6 = true;
      while( true ) {
        plVar9 = param_1 + lVar8 * 3;
        if ((*plVar9 + 8U < 0x10008 || (code *)param_2[1] == (code *)0x0) ||
           (iVar3 = (*(code *)param_2[1])(*param_2,*plVar9,&local_b8,8), iVar3 != 1))
        goto LAB_0019e220;
        uVar5 = 0;
        if ((local_b8 + 0x1001 < 0x11001) || ((local_b8 & 7) != 0)) break;
        if (local_b8 != param_1[lVar8 * 3 + 1]) goto LAB_0019e220;
        uVar5 = FUN_0019dbd4(param_2,*plVar9,&local_a8);
        if ((int)uVar5 == 0) break;
        __s2 = (&PTR_s_ControllerUltiProjectile_001cb280)[lVar8];
        iVar3 = strcmp((char *)&local_a8,__s2);
        if (iVar3 != 0) goto LAB_0019e220;
        puVar7 = (ulong *)(param_1 + lVar8 * 3 + 2);
        uVar5 = FUN_0019dbd4(param_2,*puVar7,&local_a8);
        if ((int)uVar5 == 0) break;
        uVar4 = strcmp((char *)&local_a8,__s2);
        uVar5 = (ulong)uVar4;
        if (uVar4 != 0) goto LAB_0019e220;
        if ((*plVar9 + 0x70U < 0x10008) || ((code *)param_2[1] == (code *)0x0)) break;
        iVar3 = (*(code *)param_2[1])(*param_2,*plVar9 + 0x68,&local_b0,8);
        if (iVar3 != 1) goto LAB_0019e220;
        uVar5 = 0;
        if ((local_b0 + 0x1001 < 0x11001) || ((local_b0 & 7) != 0)) break;
        if ((local_b0 != *puVar7) && (local_b0 != param_1[6])) goto LAB_0019e220;
        uVar5 = 1;
        if (param_3 != 0) {
          puVar7 = (ulong *)(param_1 + 6);
        }
        *(ulong *)(param_4 + lVar8 * 8) = *puVar7;
        lVar8 = 1;
        bVar2 = !bVar6;
        bVar6 = false;
        if (bVar2) break;
      }
      goto LAB_0019e224;
    }
    if (param_1[6] != 0) {
      uVar5 = FUN_0019dbd4(param_2,param_1[6],&local_a8);
      if ((int)uVar5 == 0) goto LAB_0019e224;
      if ((local_a8 == 0x7250796465657053 && lStack_a0 == 0x656c697463656a6f) && local_98 == '\0')
      goto LAB_0019e254;
    }
  }
LAB_0019e220:
  uVar5 = 0;
LAB_0019e224:
  if (*(long *)(lVar1 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== mmap @ 0031c1a8 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * mmap(void *__addr,size_t __len,int __prot,int __flags,int __fd,__off_t __offset)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== munmap @ 0031c1b0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int munmap(void *__addr,size_t __len)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

