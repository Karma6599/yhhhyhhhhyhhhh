/*
 * Trophies Above Head — Feature
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
 * Notes: Trophies above players heads.
 */

/* ===== FUN_00148054 @ 00148054 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00148054(int param_1)

{
  undefined8 *puVar1;
  uint uVar2;
  long lVar3;
  bool bVar4;
  undefined *puVar5;
  ulong *puVar6;
  int iVar7;
  undefined8 uVar8;
  long lVar9;
  size_t __len;
  void *pvVar10;
  char *pcVar11;
  long lVar12;
  char *pcVar13;
  uint uVar14;
  undefined8 *puVar15;
  uint uVar16;
  uint uVar17;
  undefined *puVar18;
  bool bVar19;
  bool bVar20;
  char *pcVar21;
  long lVar22;
  ulong uVar23;
  undefined8 uVar24;
  undefined8 uVar25;
  undefined8 uVar26;
  int local_310;
  int local_30c;
  undefined8 local_308;
  long local_300;
  undefined8 uStack_2f8;
  code *local_2f0;
  code *pcStack_2e8;
  code *local_2e0;
  code *pcStack_2d8;
  code *local_2d0;
  int local_2c4;
  undefined8 local_2c0;
  undefined8 local_2b8;
  code *pcStack_2b0;
  code *local_2a8;
  code *pcStack_2a0;
  undefined1 auStack_298 [552];
  long local_70;
  
  lVar3 = tpidr_el0;
  local_70 = *(long *)(lVar3 + 0x28);
  if ((((param_1 == 0) || (iVar7 = FUN_001419f8(), iVar7 == 0)) || ((int)_DAT_00220784 != 1)) ||
     ((int)_DAT_002208a0 != 1)) goto LAB_001489dc;
  DAT_00228c58 = FUN_0016b778;
  iVar7 = FUN_0014b1d0(0xaa2830,0x21c,
                       "a292999d187dfc3c166d5ec83fceb7b0015e131b9c668312f6bbcbb27e8717d9");
  if (iVar7 == 0) {
    if (*(long *)(lVar3 + 0x28) == local_70) {
      pcVar21 = "queue_code_guards";
      goto LAB_00148808;
    }
    goto LAB_00148a2c;
  }
  iVar7 = FUN_0014b1d0(0xfa9df0,0xd4,
                       "a3ef960a37385ad90bc30ba317bb41b50bfc33ae04a9fb3458041fa5c1364d8b");
  if (((iVar7 == 0) ||
      (iVar7 = FUN_0014b1d0(0xfa9c24,0xc,
                            "4d454fed2f9a182732bf8d0f650d46add5fe1931f842356001787be8c67d341f"),
      iVar7 == 0)) ||
     ((iVar7 = FUN_0014b1d0(0xf9abec,0x144,
                            "33dc3006e0b78ddc38bdcea7ffc76c82da74c9e9d54ec3bc05fc56dbf8a9879a"),
      iVar7 == 0 ||
      (iVar7 = FUN_0014b1d0(0xfa6ab4,0xa0,
                            "1675ebe0ee43bbce74310153f6b89ca6eafd6b94e9867b74076ca101ccc81bc8"),
      iVar7 == 0)))) {
    DAT_0021c790 = 0;
LAB_00148190:
    FUN_001417c8("restored_optional_refused","trophies_native_fields",0);
  }
  else {
    iVar7 = FUN_0014b1d0(0xcfeefc,0xc,
                         "6f33912a3f98b62d143fb59caef52681365a5b8834467f0c4abc5f4551249c3b");
    DAT_0021c790 = (uint)(iVar7 != 0);
    if (iVar7 == 0) goto LAB_00148190;
  }
  iVar7 = FUN_0014b1d0(0xb3d64c,0xf2c,
                       "c6b6c1dcf3826b1ec58c25d2eccca25eed40cdacd658adf11fa2d5e4303bdd0d");
  if ((((iVar7 == 0) ||
       (iVar7 = FUN_0014b1d0(0xe7cd40,0x9c,
                             "db1c4d4bcdd4178b8c864845ac63b1c7e7bc9bf3f2413ef56c292a99bdccdb73"),
       iVar7 == 0)) ||
      (((iVar7 = FUN_0014b1d0(0xecfb74,0x118,
                              "ea4c6b63a891e5697089f97b35e9a9393dacad09f0a7418e29d1d262a8e1625a"),
        iVar7 == 0 ||
        (((iVar7 = FUN_0014b1d0(0xe7cddc,0x48,
                                "757d89fc95443887343488c0d8f239595d21e089408aac54391c603750c91517"),
          iVar7 == 0 ||
          (iVar7 = FUN_0014b1d0(0xe7979c,0x9c,
                                "a48b29bfe6642800e1874e63eae8f032f8c6ce9120c3426bcdad0cfa4971927b"),
          iVar7 == 0)) ||
         (iVar7 = FUN_0014b1d0(0xe798b0,0x20,
                               "82be5b703b18215654561531e7a33d979743afc1b9a026472523cc167dd45801"),
         iVar7 == 0)))) ||
       (((iVar7 = FUN_0014b1d0(0xecf9f4,0x60,
                               "0407300dadcaaf0171830745ded639ef00e298f50019eb8ff4ed6a52c2818673"),
         iVar7 == 0 ||
         (iVar7 = FUN_0014b1d0(0x8580d4,0x7c0,
                               "3333ee1d5b1e8d5a20be1ff24f7c735e10048fb01796389954e0928ad1de1120"),
         iVar7 == 0)) ||
        (iVar7 = FUN_0014b1d0(0x858894,0x7b8,
                              "1a411f9af22f7c7d59a9599a5dfcc60a44070b4b0121116d1f410d02749e26ec"),
        iVar7 == 0)))))) ||
     (iVar7 = FUN_0014b1d0(0x8741d8,0x150,
                           "083d3765e11ad69526df0c046efdbf125848e295419eeffd36c9d07000ca252b"),
     iVar7 == 0)) {
    DAT_00214938._4_4_ = 0;
  }
  else {
    iVar7 = FUN_0014b1d0(0xcf5044,0x44,
                         "e79643035de49ffa02823584702d294402aabd6b1d852c3fed52c40d15c114b7");
    DAT_00214938._4_4_ = (uint)(iVar7 != 0);
    if (iVar7 != 0) {
      iVar7 = FUN_0014b1d0(0x7050a4,0xbc,
                           "34a5074b72bb308ac25ec9ca60db279eb5472b00f936c54d506c99f27879ad7d");
      if (((iVar7 == 0) ||
          (iVar7 = FUN_0014b1d0(0x705160,0x7c,
                                "3fec2737dd5569528dcbea561b5f857c8419aa34de056e3233869b1b32ee5f0b"),
          iVar7 == 0)) ||
         (iVar7 = FUN_0014b1d0(0x7051dc,0x70,
                               "f9da06b9ce7403e941cdf05394a692d2bf59b300270f04e21f83e851110ea045"),
         iVar7 == 0)) {
        if (*(long *)(lVar3 + 0x28) == local_70) {
          pcVar21 = "touch_code_guards";
          goto LAB_00148808;
        }
        goto LAB_00148a2c;
      }
      iVar7 = FUN_001419f8();
      if ((((iVar7 == 0) ||
           (iVar7 = FUN_0014b1d0(0x80c1e0,0x108,
                                 "bb0388a1ee28b984b003b99cd571cab63afe3631f2138b78962fc1a06348b050")
           , iVar7 == 0)) ||
          (iVar7 = FUN_0014b1d0(0xb28f2c,0x1674,
                                "2ada8e2e8d844459416abff76610f32f7f3a7fc233d7acd18690ecacfee44b18"),
          iVar7 == 0)) ||
         ((iVar7 = FUN_0014b1d0(0x671630,0x1bc,
                                "4863dfa470d3525e6e383d72f1e00b98bbd1eef1232423876fe226462486d682"),
          iVar7 == 0 ||
          (uVar8 = FUN_0014b1d0(0x671860,0x120,
                                "f9c0fbbfafdc423f98ffb766e2fff8d5ef3f9ad537a9084f20591b182eb0d1b6"),
          (int)uVar8 == 0)))) {
LAB_001489c4:
        FUN_001417c8("restored_refused","spectator_code_guards",0);
      }
      else {
        local_308 = 0;
        iVar7 = FUN_001428fc(uVar8,DAT_001e0978 + 0x123c4c0,&local_308,8);
        if (((iVar7 == 0) || ((local_308 + 0x2000 < 0x12000 || ((local_308 & 7) != 0)))) ||
           (local_308 != DAT_001e0978 + 0x1303f20U)) goto LAB_001489c4;
        lVar22 = 0;
        uVar23 = 0;
        do {
          *(long *)((long)&DAT_00228ef8 + lVar22) = (&DAT_00121408)[uVar23] + DAT_001e0978;
          lVar9 = FUN_00152af0();
          *(long *)((long)&DAT_00228f00 + lVar22) = lVar9;
          if (lVar9 == 0) goto LAB_001489c4;
          iVar7 = FUN_001428fc(lVar9,*(long *)((long)&DAT_00228ef8 + lVar22),
                               (undefined4 *)((long)&DAT_00228f08 + lVar22),4);
          if (iVar7 == 0) goto LAB_001489c4;
          lVar9 = *(long *)((long)&DAT_00228ef8 + lVar22);
          puVar15 = *(undefined8 **)((long)&DAT_00228f00 + lVar22);
          if ((((((uint)lVar9 | (uint)puVar15) & 3) != 0) ||
              ((lVar9 - (long)puVar15) - 0x7fffffdU < 0xfffffffff0000003)) ||
             (((long)puVar15 - lVar9) - 0x7ffffedU < 0xfffffffff0000003)) goto LAB_001489c4;
          uVar14 = (uint)(lVar9 - (long)puVar15);
          uVar17 = uVar14 + 3;
          if (-1 < (int)uVar14) {
            uVar17 = uVar14;
          }
          iVar7 = (int)((long)puVar15 - lVar9);
          uVar16 = iVar7 + 0x10;
          uVar14 = iVar7 + 0x13;
          if (-1 < (int)uVar16) {
            uVar14 = uVar16;
          }
          *(undefined4 *)((long)&DAT_00228f10 + lVar22) =
               *(undefined4 *)((long)&DAT_00228f08 + lVar22);
          *(uint *)((long)&DAT_00228f14 + lVar22) = uVar17 >> 2 & 0x3ffffff | 0x14000000;
          uVar17 = 0x94000000;
          if (uVar23 < 2) {
            uVar17 = 0x14000000;
          }
          puVar18 = (&PTR_FUN_001c58c0)[uVar23];
          uVar26 = *(undefined8 *)(lVar22 + 0x228f18);
          uVar25 = *(undefined8 *)((long)&DAT_00228f10 + lVar22);
          *(uint *)((long)&DAT_00228f0c + lVar22) = uVar14 >> 2 & 0x3ffffff | uVar17;
          *(undefined8 *)((long)&DAT_00228f20 + lVar22) = 0xd61f020058000050;
          *(undefined **)((long)&DAT_00228f28 + lVar22) = puVar18;
          uVar24 = *(undefined8 *)((long)&DAT_00228f28 + lVar22);
          uVar8 = *(undefined8 *)((long)&DAT_00228f20 + lVar22);
          puVar15[1] = uVar26;
          *puVar15 = uVar25;
          puVar15[3] = uVar24;
          puVar15[2] = uVar8;
          FUN_001bdaf0(*(long *)((long)&DAT_00228f00 + lVar22),
                       *(long *)((long)&DAT_00228f00 + lVar22) + 0x20);
          iVar7 = mprotect(*(void **)((long)&DAT_00228f00 + lVar22),DAT_00209d88,5);
          if (iVar7 != 0) goto LAB_001489c4;
          uVar23 = uVar23 + 1;
          lVar22 = lVar22 + 0x38;
        } while (lVar22 != 0xe0);
        DAT_00228fd8 = DAT_00228f00;
        DAT_00228fe0 = DAT_00228f38;
        iVar7 = FUN_001419f8();
        if ((iVar7 == 0) ||
           (lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_00228f0c,4,DAT_00228ef8), lVar22 != 4)) {
LAB_00148850:
          bVar19 = false;
          bVar4 = false;
          bVar20 = true;
LAB_0014885c:
          lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_00228f08,4,DAT_00228ef8);
          if (lVar22 != 4) {
LAB_00148be4:
                    /* WARNING: Subroutine does not return */
            abort();
          }
          uVar8 = FUN_001bdaf0(DAT_00228ef8,DAT_00228ef8 + 4);
          iVar7 = FUN_001428fc(uVar8,DAT_00228ef8,&local_2c4,4);
          if ((iVar7 == 0) || (local_2c4 != DAT_00228f08)) {
LAB_00148be0:
                    /* WARNING: Subroutine does not return */
            abort();
          }
          if (!bVar20) {
            lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_00228f40,4,DAT_00228f30);
            if (lVar22 != 4) goto LAB_00148be4;
            uVar8 = FUN_001bdaf0(DAT_00228f30,DAT_00228f30 + 4);
            iVar7 = FUN_001428fc(uVar8,DAT_00228f30,&local_2c4,4);
            if ((iVar7 == 0) || (local_2c4 != DAT_00228f40)) goto LAB_00148be0;
            if (!bVar19) {
              lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_00228f78,4,DAT_00228f68);
              if (lVar22 != 4) goto LAB_00148be4;
              uVar8 = FUN_001bdaf0(DAT_00228f68,DAT_00228f68 + 4);
              iVar7 = FUN_001428fc(uVar8,DAT_00228f68,&local_2c4,4);
              if ((iVar7 == 0) || (local_2c4 != DAT_00228f78)) goto LAB_00148be0;
              if (!bVar4) {
                lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_00228fb0,4,DAT_00228fa0);
                if (lVar22 != 4) goto LAB_00148be4;
                uVar8 = FUN_001bdaf0(DAT_00228fa0,DAT_00228fa0 + 4);
                iVar7 = FUN_001428fc(uVar8,DAT_00228fa0,&local_2c4,4);
                if ((iVar7 == 0) || (local_2c4 != DAT_00228fb0)) goto LAB_00148be0;
              }
            }
          }
          goto LAB_001489c4;
        }
        uVar8 = FUN_001bdaf0(DAT_00228ef8,DAT_00228ef8 + 4);
        iVar7 = FUN_001428fc(uVar8,DAT_00228ef8,&local_2c4,4);
        if ((iVar7 == 0) || (local_2c4 != DAT_00228f0c)) goto LAB_00148850;
        iVar7 = FUN_001419f8();
        if ((iVar7 == 0) ||
           (lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_00228f44,4,DAT_00228f30), lVar22 != 4)) {
LAB_00148a0c:
          bVar20 = false;
          bVar4 = false;
          bVar19 = true;
          goto LAB_0014885c;
        }
        uVar8 = FUN_001bdaf0(DAT_00228f30,DAT_00228f30 + 4);
        iVar7 = FUN_001428fc(uVar8,DAT_00228f30,&local_2c4,4);
        if ((iVar7 == 0) || (local_2c4 != DAT_00228f44)) goto LAB_00148a0c;
        iVar7 = FUN_001419f8();
        if ((iVar7 == 0) ||
           (lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_00228f7c,4,DAT_00228f68), lVar22 != 4)) {
LAB_00148a1c:
          bVar20 = false;
          bVar19 = false;
          bVar4 = true;
          goto LAB_0014885c;
        }
        uVar8 = FUN_001bdaf0(DAT_00228f68,DAT_00228f68 + 4);
        iVar7 = FUN_001428fc(uVar8,DAT_00228f68,&local_2c4,4);
        if ((iVar7 == 0) || (local_2c4 != DAT_00228f7c)) goto LAB_00148a1c;
        iVar7 = FUN_001419f8();
        if ((iVar7 == 0) ||
           (lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_00228fb4,4,DAT_00228fa0), lVar22 != 4)) {
LAB_00148a30:
          bVar20 = false;
          bVar19 = false;
          bVar4 = false;
          goto LAB_0014885c;
        }
        uVar8 = FUN_001bdaf0(DAT_00228fa0,DAT_00228fa0 + 4);
        iVar7 = FUN_001428fc(uVar8,DAT_00228fa0,&local_2c4,4);
        if ((iVar7 == 0) || (local_2c4 != DAT_00228fb4)) goto LAB_00148a30;
        DAT_00220800 = 1;
        iVar7 = FUN_001419f8();
        if (((iVar7 == 0) ||
            (iVar7 = FUN_0014b1d0(0xf39550,0x58,
                                  "e4f08a4e7ec1aa3cc89435521ce00675a294f6452b74792f908dc3c4b9bbbf84"
                                 ), iVar7 == 0)) ||
           (iVar7 = FUN_0014b1d0(0xf3a6e0,0xc4,
                                 "dc94a3d1df8b4d4eca9d98c6c378ffdff9966a3e336ab164f24d9371a901781a")
           , iVar7 == 0)) {
LAB_00148c38:
          FUN_001417c8("script_port_unavailable","battle_text_chat",0);
        }
        else {
          iVar7 = FUN_0014b1d0(0x8d8ab8,0x1ec,
                               "ca2e9333fa1f07663da460b1202fe569b68c6c45e5d9b1733d9b87bad2f861f8");
          if ((iVar7 == 0) ||
             (iVar7 = FUN_0014b1d0(0x8d9180,0x90,
                                   "ad4bad5ae27aa2211471eabdef3bc6d939bffd321b22e29129edda7c1c31fd84"
                                  ), iVar7 == 0)) {
            DAT_001e0970 = 0;
          }
          else {
            iVar7 = FUN_0014b1d0(0x8d95a8,0xc,
                                 "2e5a53d684c71177c39ca818dac4f2733319116c45b1f38331d18caafdd13050")
            ;
            DAT_001e0970 = (uint)(iVar7 != 0);
          }
          DAT_00228fe8 = DAT_001e0978 + 0xf39550;
          DAT_00228ff0 = (ulong *)FUN_00152af0();
          if (((DAT_00228ff0 == (ulong *)0x0) ||
              (iVar7 = FUN_001428fc(DAT_00228ff0,DAT_00228fe8,&DAT_00228ff8,4),
              puVar6 = DAT_00228ff0, iVar7 == 0)) ||
             ((DAT_00228ff8 != -0x56418403 ||
              ((((((uint)DAT_00228fe8 | (uint)DAT_00228ff0) & 3) != 0 ||
                ((DAT_00228fe8 - (long)DAT_00228ff0) - 0x7fffffdU < 0xfffffffff0000003)) ||
               (((long)DAT_00228ff0 - DAT_00228fe8) - 0x7ffffedU < 0xfffffffff0000003))))))
          goto LAB_00148c38;
          uVar14 = (uint)(DAT_00228fe8 - (long)DAT_00228ff0);
          iVar7 = (int)((long)DAT_00228ff0 - DAT_00228fe8);
          uVar16 = iVar7 + 0x10;
          uVar17 = uVar14 + 3;
          if (-1 < (int)uVar14) {
            uVar17 = uVar14;
          }
          uVar14 = iVar7 + 0x13;
          if (-1 < (int)uVar16) {
            uVar14 = uVar16;
          }
          uVar17 = uVar17 >> 2 & 0x3ffffff;
          DAT_00229004 = uVar17 | 0x14000000;
          DAT_00229000 = 0xa9be7bfd;
          DAT_00229010 = 0xd61f020058000050;
          DAT_00229018 = FUN_0016fa60;
          DAT_00228ffc = uVar14 >> 2 & 0x3ffffff | 0x14000000;
          DAT_00228ff0[1] = uRam0000000000229008;
          *puVar6 = CONCAT44(uVar17,0xa9be7bfd) | 0x1400000000000000;
          puVar6[3] = (ulong)FUN_0016fa60;
          puVar6[2] = 0xd61f020058000050;
          FUN_001bdaf0(DAT_00228ff0,DAT_00228ff0 + 4);
          iVar7 = mprotect(DAT_00228ff0,DAT_00209d88,5);
          if (iVar7 != 0) goto LAB_00148c38;
          DAT_00229020 = DAT_00228ff0;
          iVar7 = FUN_001419f8();
          if ((iVar7 == 0) ||
             (lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_00228ffc,4,DAT_00228fe8), lVar22 != 4)) {
LAB_00148be8:
            lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_00228ff8,4,DAT_00228fe8);
            if (lVar22 != 4) {
                    /* WARNING: Subroutine does not return */
              abort();
            }
            uVar8 = FUN_001bdaf0(DAT_00228fe8,DAT_00228fe8 + 4);
            iVar7 = FUN_001428fc(uVar8,DAT_00228fe8,&local_308,4);
            if ((iVar7 == 0) || ((uint)local_308 != DAT_00228ff8)) {
                    /* WARNING: Subroutine does not return */
              abort();
            }
            goto LAB_00148c38;
          }
          uVar8 = FUN_001bdaf0(DAT_00228fe8,DAT_00228fe8 + 4);
          iVar7 = FUN_001428fc(uVar8,DAT_00228fe8,&local_308,4);
          if ((iVar7 == 0) || ((uint)local_308 != DAT_00228ffc)) goto LAB_00148be8;
          DAT_001e096c = 1;
        }
        iVar7 = FUN_001419f8();
        if ((((iVar7 == 0) ||
             (iVar7 = FUN_0014b1d0(0xb444a0,0x2fac,
                                   "bd0ebb55201a288b9fb12128fdc0b4747ade4ca5c158ab666d00ecf5551f6f25"
                                  ), iVar7 == 0)) ||
            (iVar7 = FUN_0014b1d0(0xb4071c,0x3b6c,
                                  "dd11a7757ec957fc308cb010fd27ddfc85779f70a71dae6a64864c64b5199efe"
                                 ), iVar7 == 0)) ||
           (iVar7 = FUN_0014b1d0(0xd4c490,0xd24,
                                 "9cb53aaae7005672d808c034dfc90dedf38a4d61a84da198870788098de224ad")
           , iVar7 == 0)) {
LAB_00148dcc:
          FUN_001417c8("script_port_unavailable","extended_ball_trajectory",0);
          iVar7 = FUN_001419f8();
          if (iVar7 == 0) goto LAB_00149230;
LAB_00148dec:
          iVar7 = FUN_0014b1d0(0xfa7db8,0x528,
                               "f476154d195ebf46a630e0a57e5c380bf020c1e80dc3d83d92b265c9eb71c0f2");
          if (((iVar7 == 0) ||
              (iVar7 = FUN_0014b1d0(0x66ae58,0x80,
                                    "d8e23d16dc9125a73ecbc044b9c4180c7c679f55a8b8252364510f5add8f7f88"
                                   ), iVar7 == 0)) ||
             (iVar7 = FUN_0014b1d0(0x66ad48,0x1c,
                                   "9edb63b79febb9496a60828270f995d5b618902d80ee1d7caca7a98df8ad3863"
                                  ), iVar7 == 0)) goto LAB_00149230;
          DAT_002290b0 = DAT_001e0978 + 0xfa7db8;
          DAT_002290b8 = (ulong *)FUN_00152af0();
          if ((((DAT_002290b8 == (ulong *)0x0) ||
               (iVar7 = FUN_001428fc(DAT_002290b8,DAT_002290b0,&DAT_002290c0,4),
               puVar6 = DAT_002290b8, iVar7 == 0)) ||
              ((DAT_002290c0 != -0x56448403 ||
               (((((uint)DAT_002290b0 | (uint)DAT_002290b8) & 3) != 0 ||
                ((DAT_002290b0 - (long)DAT_002290b8) - 0x7fffffdU < 0xfffffffff0000003)))))) ||
             (((long)DAT_002290b8 - DAT_002290b0) - 0x7ffffedU < 0xfffffffff0000003))
          goto LAB_00149230;
          uVar14 = (uint)(DAT_002290b0 - (long)DAT_002290b8);
          iVar7 = (int)((long)DAT_002290b8 - DAT_002290b0);
          uVar16 = iVar7 + 0x10;
          uVar17 = uVar14 + 3;
          if (-1 < (int)uVar14) {
            uVar17 = uVar14;
          }
          uVar14 = iVar7 + 0x13;
          if (-1 < (int)uVar16) {
            uVar14 = uVar16;
          }
          uVar17 = uVar17 >> 2 & 0x3ffffff;
          DAT_002290cc = uVar17 | 0x14000000;
          DAT_002290c8 = 0xa9bb7bfd;
          DAT_002290d8 = 0xd61f020058000050;
          DAT_002290e0 = FUN_00170f9c;
          DAT_002290c4 = uVar14 >> 2 & 0x3ffffff | 0x14000000;
          DAT_002290b8[1] = uRam00000000002290d0;
          *puVar6 = CONCAT44(uVar17,0xa9bb7bfd) | 0x1400000000000000;
          puVar6[3] = (ulong)FUN_00170f9c;
          puVar6[2] = 0xd61f020058000050;
          FUN_001bdaf0(DAT_002290b8,DAT_002290b8 + 4);
          iVar7 = mprotect(DAT_002290b8,DAT_00209d88,5);
          if (iVar7 != 0) goto LAB_00149230;
          DAT_002290e8 = DAT_002290b8;
          iVar7 = FUN_001419f8();
          if ((iVar7 == 0) ||
             (lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_002290c4,4,DAT_002290b0), lVar22 != 4)) {
LAB_001491e0:
            lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_002290c0,4,DAT_002290b0);
            if (lVar22 != 4) {
                    /* WARNING: Subroutine does not return */
              abort();
            }
            uVar8 = FUN_001bdaf0(DAT_002290b0,DAT_002290b0 + 4);
            iVar7 = FUN_001428fc(uVar8,DAT_002290b0,&local_308,4);
            if ((iVar7 == 0) || ((uint)local_308 != DAT_002290c0)) {
                    /* WARNING: Subroutine does not return */
              abort();
            }
            goto LAB_00149230;
          }
          uVar8 = FUN_001bdaf0(DAT_002290b0,DAT_002290b0 + 4);
          iVar7 = FUN_001428fc(uVar8,DAT_002290b0,&local_308,4);
          if ((iVar7 == 0) || ((uint)local_308 != DAT_002290c4)) goto LAB_001491e0;
          DAT_0020afa0._4_1_ = 1;
          iVar7 = FUN_00172368();
          if (iVar7 == 0) {
            pcVar21 = "social_name_metadata";
            goto LAB_00149240;
          }
        }
        else {
          DAT_00229028 = DAT_001e0978 + 0xb444a0;
          DAT_00229030 = (ulong *)FUN_00152af0();
          if ((((DAT_00229030 == (ulong *)0x0) ||
               (iVar7 = FUN_001428fc(DAT_00229030,DAT_00229028,&DAT_00229038,4),
               puVar6 = DAT_00229030, iVar7 == 0)) ||
              ((DAT_00229038 != 0x6db63bef ||
               (((((uint)DAT_00229028 | (uint)DAT_00229030) & 3) != 0 ||
                ((DAT_00229028 - (long)DAT_00229030) - 0x7fffffdU < 0xfffffffff0000003)))))) ||
             (((long)DAT_00229030 - DAT_00229028) - 0x7ffffedU < 0xfffffffff0000003))
          goto LAB_00148dcc;
          uVar14 = (uint)(DAT_00229028 - (long)DAT_00229030);
          iVar7 = (int)((long)DAT_00229030 - DAT_00229028);
          uVar16 = iVar7 + 0x10;
          uVar17 = uVar14 + 3;
          if (-1 < (int)uVar14) {
            uVar17 = uVar14;
          }
          uVar14 = iVar7 + 0x13;
          if (-1 < (int)uVar16) {
            uVar14 = uVar16;
          }
          uVar17 = uVar17 >> 2 & 0x3ffffff;
          DAT_00229044 = uVar17 | 0x14000000;
          DAT_00229040 = 0x6db63bef;
          DAT_00229050 = 0xd61f020058000050;
          DAT_00229058 = FUN_0016ffd8;
          DAT_0022903c = uVar14 >> 2 & 0x3ffffff | 0x14000000;
          DAT_00229030[1] = uRam0000000000229048;
          *puVar6 = CONCAT44(uVar17,0x6db63bef) | 0x1400000000000000;
          puVar6[3] = (ulong)FUN_0016ffd8;
          puVar6[2] = 0xd61f020058000050;
          FUN_001bdaf0(DAT_00229030,DAT_00229030 + 4);
          iVar7 = mprotect(DAT_00229030,DAT_00209d88,5);
          if (iVar7 != 0) goto LAB_00148dcc;
          DAT_00229060 = DAT_001e0978 + 0xb4071c;
          DAT_00229068 = (ulong *)FUN_00152af0();
          if (((((DAT_00229068 == (ulong *)0x0) ||
                (iVar7 = FUN_001428fc(DAT_00229068,DAT_00229060,&DAT_00229070,4),
                puVar6 = DAT_00229068, iVar7 == 0)) || (DAT_00229070 != 0x6db63bef)) ||
              (((((uint)DAT_00229060 | (uint)DAT_00229068) & 3) != 0 ||
               ((DAT_00229060 - (long)DAT_00229068) - 0x7fffffdU < 0xfffffffff0000003)))) ||
             (((long)DAT_00229068 - DAT_00229060) - 0x7ffffedU < 0xfffffffff0000003))
          goto LAB_00148dcc;
          uVar14 = (uint)(DAT_00229060 - (long)DAT_00229068);
          iVar7 = (int)((long)DAT_00229068 - DAT_00229060);
          uVar16 = iVar7 + 0x10;
          uVar17 = uVar14 + 3;
          if (-1 < (int)uVar14) {
            uVar17 = uVar14;
          }
          uVar14 = iVar7 + 0x13;
          if (-1 < (int)uVar16) {
            uVar14 = uVar16;
          }
          uVar17 = uVar17 >> 2 & 0x3ffffff;
          DAT_0022907c = uVar17 | 0x14000000;
          DAT_00229078 = 0x6db63bef;
          DAT_00229088 = 0xd61f020058000050;
          DAT_00229090 = FUN_0017019c;
          DAT_00229074 = uVar14 >> 2 & 0x3ffffff | 0x14000000;
          DAT_00229068[1] = uRam0000000000229080;
          *puVar6 = CONCAT44(uVar17,0x6db63bef) | 0x1400000000000000;
          puVar6[3] = (ulong)FUN_0017019c;
          puVar6[2] = 0xd61f020058000050;
          FUN_001bdaf0(DAT_00229068,DAT_00229068 + 4);
          iVar7 = mprotect(DAT_00229068,DAT_00209d88,5);
          if (iVar7 != 0) goto LAB_00148dcc;
          DAT_00229098 = DAT_00229030;
          DAT_002290a0 = DAT_00229068;
          iVar7 = FUN_001419f8();
          if ((iVar7 == 0) ||
             (lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_0022903c,4,DAT_00229028), lVar22 != 4)) {
LAB_00149d20:
            bVar19 = true;
LAB_00149d24:
            lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_00229038,4,DAT_00229028);
            if (lVar22 != 4) {
LAB_00149f90:
                    /* WARNING: Subroutine does not return */
              abort();
            }
            uVar8 = FUN_001bdaf0(DAT_00229028,DAT_00229028 + 4);
            iVar7 = FUN_001428fc(uVar8,DAT_00229028,&local_308,4);
            if ((iVar7 == 0) || ((uint)local_308 != DAT_00229038)) {
LAB_00149dcc:
                    /* WARNING: Subroutine does not return */
              abort();
            }
            if (!bVar19) {
              lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_00229070,4,DAT_00229060);
              if (lVar22 != 4) goto LAB_00149f90;
              uVar8 = FUN_001bdaf0(DAT_00229060,DAT_00229060 + 4);
              iVar7 = FUN_001428fc(uVar8,DAT_00229060,&local_308,4);
              if ((iVar7 == 0) || ((uint)local_308 != DAT_00229070)) goto LAB_00149dcc;
            }
            goto LAB_00148dcc;
          }
          uVar8 = FUN_001bdaf0(DAT_00229028,DAT_00229028 + 4);
          iVar7 = FUN_001428fc(uVar8,DAT_00229028,&local_308,4);
          if ((iVar7 == 0) || ((uint)local_308 != DAT_0022903c)) goto LAB_00149d20;
          iVar7 = FUN_001419f8();
          if ((iVar7 == 0) ||
             (lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_00229074,4,DAT_00229060), lVar22 != 4)) {
LAB_00149e10:
            bVar19 = false;
            goto LAB_00149d24;
          }
          uVar8 = FUN_001bdaf0(DAT_00229060,DAT_00229060 + 4);
          iVar7 = FUN_001428fc(uVar8,DAT_00229060,&local_308,4);
          if ((iVar7 == 0) || ((uint)local_308 != DAT_00229074)) goto LAB_00149e10;
          DAT_002290a8 = 1;
          iVar7 = FUN_001419f8();
          if (iVar7 != 0) goto LAB_00148dec;
LAB_00149230:
          pcVar21 = "battle_name_decorations";
LAB_00149240:
          FUN_001417c8("script_port_unavailable",pcVar21,0);
        }
        iVar7 = FUN_001419f8();
        if ((((iVar7 == 0) ||
             (iVar7 = FUN_0014b1d0(0x86bdfc,0x174c,
                                   "e5b9679a102c6c0c387d4429aeea2ce5be6401bc95de89aeb40a9947aba62917"
                                  ), iVar7 == 0)) ||
            (iVar7 = FUN_0014b1d0(0xd49da8,0x3c,
                                  "cd09749187c98f768df22dd7ad5ca56d51a007c8b9a7ed5580a95dd0d23be2ed"
                                 ), iVar7 == 0)) ||
           (iVar7 = FUN_0014b1d0(0xfd46fc,0xa8,
                                 "272deb9d7c88eea890a37806decb7cf85d7495cf196322576c01082d7c07cf5d")
           , iVar7 == 0)) {
LAB_00149490:
          FUN_001417c8("script_port_unavailable","native_teammate_arrow",0);
        }
        else {
          DAT_0022b570 = DAT_001e0978 + 0x86bdfc;
          DAT_0022b578 = (ulong *)FUN_00152af0();
          if ((((DAT_0022b578 == (ulong *)0x0) ||
               (iVar7 = FUN_001428fc(DAT_0022b578,DAT_0022b570,&DAT_0022b580,4),
               puVar6 = DAT_0022b578, iVar7 == 0)) ||
              ((DAT_0022b580 != -0x2efcbc01 ||
               (((((uint)DAT_0022b570 | (uint)DAT_0022b578) & 3) != 0 ||
                ((DAT_0022b570 - (long)DAT_0022b578) - 0x7fffffdU < 0xfffffffff0000003)))))) ||
             (((long)DAT_0022b578 - DAT_0022b570) - 0x7ffffedU < 0xfffffffff0000003))
          goto LAB_00149490;
          uVar14 = (uint)(DAT_0022b570 - (long)DAT_0022b578);
          iVar7 = (int)((long)DAT_0022b578 - DAT_0022b570);
          uVar16 = iVar7 + 0x10;
          uVar17 = uVar14 + 3;
          if (-1 < (int)uVar14) {
            uVar17 = uVar14;
          }
          uVar14 = iVar7 + 0x13;
          if (-1 < (int)uVar16) {
            uVar14 = uVar16;
          }
          uVar17 = uVar17 >> 2 & 0x3ffffff;
          DAT_0022b58c = uVar17 | 0x14000000;
          DAT_0022b588 = 0xd10343ff;
          DAT_0022b598 = 0xd61f020058000050;
          DAT_0022b5a0 = FUN_001730bc;
          DAT_0022b584 = uVar14 >> 2 & 0x3ffffff | 0x14000000;
          DAT_0022b578[1] = uRam000000000022b590;
          *puVar6 = CONCAT44(uVar17,0xd10343ff) | 0x1400000000000000;
          puVar6[3] = (ulong)FUN_001730bc;
          puVar6[2] = 0xd61f020058000050;
          FUN_001bdaf0(DAT_0022b578,DAT_0022b578 + 4);
          iVar7 = mprotect(DAT_0022b578,DAT_00209d88,5);
          if (iVar7 != 0) goto LAB_00149490;
          DAT_0022b5a8 = DAT_0022b578;
          iVar7 = FUN_001419f8();
          if ((iVar7 == 0) ||
             (lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_0022b584,4,DAT_0022b570), lVar22 != 4)) {
LAB_00149440:
            lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_0022b580,4,DAT_0022b570);
            if (lVar22 != 4) {
                    /* WARNING: Subroutine does not return */
              abort();
            }
            uVar8 = FUN_001bdaf0(DAT_0022b570,DAT_0022b570 + 4);
            iVar7 = FUN_001428fc(uVar8,DAT_0022b570,&local_308,4);
            if ((iVar7 == 0) || ((uint)local_308 != DAT_0022b580)) {
                    /* WARNING: Subroutine does not return */
              abort();
            }
            goto LAB_00149490;
          }
          uVar8 = FUN_001bdaf0(DAT_0022b570,DAT_0022b570 + 4);
          iVar7 = FUN_001428fc(uVar8,DAT_0022b570,&local_308,4);
          if ((iVar7 == 0) || ((uint)local_308 != DAT_0022b584)) goto LAB_00149440;
          DAT_0022b5b0 = 1;
        }
        iVar7 = FUN_0014b1d0(0xfd4c54,0x94,
                             "80062db5e5955bd4a396675ce1a90487546030590c7ed366391b87c8fff2dd94");
        if (((iVar7 == 0) ||
            (iVar7 = FUN_0014b1d0(0xf86d6c,0x40,
                                  "4063cbdeb9d71a6dab1568d64fd767c7ee3ae7a0f4e9a7178eb7cac1908ed42e"
                                 ), iVar7 == 0)) ||
           (iVar7 = FUN_0014b1d0(0xfd3cbc,0x14,
                                 "6dbc0bc720cd22e1374bcef7325a2163b44671e30d064e2aebc85e1bc0e09bf2")
           , iVar7 == 0)) {
          DAT_00220780 = 0;
LAB_0014952c:
          FUN_001417c8("script_port_unavailable","ally_respawn",0);
        }
        else {
          iVar7 = FUN_0014b1d0(0xfd46fc,0xa8,
                               "272deb9d7c88eea890a37806decb7cf85d7495cf196322576c01082d7c07cf5d");
          DAT_00220780 = (uint)(iVar7 != 0);
          if (iVar7 == 0) goto LAB_0014952c;
        }
        iVar7 = FUN_001419f8();
        if (((iVar7 == 0) ||
            (iVar7 = FUN_0014b1d0(0xe7af70,0x8d4,
                                  "4d6f74916dc6929e7ba905dece78bde36b56eefc5c7651ba17aa53b8f32487ea"
                                 ), iVar7 == 0)) ||
           ((iVar7 = FUN_0014b1d0(0xe7bd74,0x20c,
                                  "bf8fe1ca284cb69aa0e45baa8db2df7fdd7be2c1a1b522d7367f08fcacc7b500"
                                 ), iVar7 == 0 ||
            ((iVar7 = FUN_0014b1d0(0xb35608,0x1bf4,
                                   "b98e10a40d5f978e31b292d3962e6a64374b7c9fab60c4cb622a2756f8db73ab"
                                  ), iVar7 == 0 ||
             (iVar7 = FUN_0014b1d0(0xb533fc,0xc,
                                   "585c75735a026b32b00785df02d8889b85e3925d335841a19644e3e5e557b565"
                                  ), iVar7 == 0)))))) {
LAB_0014979c:
          FUN_001417c8("restored_optional_refused","speed_code_guards",0);
        }
        else {
          DAT_00215d10 = DAT_001e0978 + 0xf86758;
          DAT_00215d20 = (undefined8 *)FUN_00152af0();
          if ((DAT_00215d20 == (undefined8 *)0x0) ||
             (((iVar7 = FUN_001428fc(DAT_00215d20,DAT_00215d10,&DAT_0022b6c0,4),
               uVar24 = DAT_00215d40, uVar8 = DAT_00215d38, puVar15 = DAT_00215d20, iVar7 == 0 ||
               ((((uint)DAT_001e0978 | (uint)DAT_00215d10) & 3) != 0)) ||
              (lVar22 = (0xe7af70 - DAT_00215d10) + DAT_001e0978,
              lVar22 - 0x7fffffdU < 0xfffffffff0000003)))) goto LAB_0014979c;
          uVar14 = (uint)lVar22;
          uVar17 = uVar14 + 3;
          if (-1 < (int)uVar14) {
            uVar17 = uVar14;
          }
          if (((DAT_0022b6c0 != (uVar17 >> 2 & 0x3ffffff | 0x94000000)) ||
              ((((uint)DAT_00215d20 | (uint)DAT_00215d10) & 3) != 0)) ||
             (((long)DAT_00215d20 - DAT_00215d10) - 0x7fffffdU < 0xfffffffff0000003))
          goto LAB_0014979c;
          uVar14 = (uint)((long)DAT_00215d20 - DAT_00215d10);
          uVar17 = uVar14 + 3;
          if (-1 < (int)uVar14) {
            uVar17 = uVar14;
          }
          DAT_00215d18 = uVar17 >> 2 & 0x3ffffff | 0x94000000;
          puVar1 = DAT_00215d20 + 4;
          DAT_00215d28 = 0xd61f020058000050;
          DAT_00215d30 = FUN_00173740;
          DAT_00215d20[1] = FUN_00173740;
          *puVar15 = 0xd61f020058000050;
          puVar15[3] = uVar24;
          puVar15[2] = uVar8;
          FUN_001bdaf0(puVar15,puVar1);
          iVar7 = mprotect(DAT_00215d20,DAT_00209d88,5);
          if (iVar7 != 0) goto LAB_0014979c;
          lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_00215d18,4,DAT_00215d10);
          if (lVar22 != 4) {
LAB_0014974c:
            lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_0022b6c0,4,DAT_00215d10);
            if (lVar22 != 4) {
                    /* WARNING: Subroutine does not return */
              abort();
            }
            uVar8 = FUN_001bdaf0(DAT_00215d10,DAT_00215d10 + 4);
            iVar7 = FUN_001428fc(uVar8,DAT_00215d10,&local_308,4);
            if ((iVar7 == 0) || ((uint)local_308 != DAT_0022b6c0)) {
                    /* WARNING: Subroutine does not return */
              abort();
            }
            goto LAB_0014979c;
          }
          uVar8 = FUN_001bdaf0(DAT_00215d10,DAT_00215d10 + 4);
          iVar7 = FUN_001428fc(uVar8,DAT_00215d10,&local_308,4);
          if ((iVar7 == 0) || ((uint)local_308 != DAT_00215d18)) goto LAB_0014974c;
          DAT_00215d08 = 1;
        }
        iVar7 = FUN_001419f8();
        if (((iVar7 == 0) ||
            (iVar7 = FUN_0014b1d0(0x863fbc,0x75c,
                                  "b3894c39e8de7b2bfa31e0c0e6dc312a3fda0d1d2f5e02950cc7c99f73a70cbb"
                                 ), puVar18 = PTR_FUN_001cb6d0, iVar7 == 0)) ||
           ((long)PTR_FUN_001cb6d0 - (long)PTR_DAT_001cb6b0 != 0x158)) {
LAB_00149ce4:
          pcVar21 = "afk_visual_code_guards";
LAB_00149cf4:
          pcVar11 = "restored_refused";
          pcVar13 = (char *)0x0;
LAB_00149cf8:
          FUN_001417c8(pcVar11,pcVar21,pcVar13);
        }
        else {
          DAT_0022b6f8 = DAT_001e0978 + 0x8640fc;
          DAT_0022b700 = (void *)FUN_00152af0();
          if (((DAT_0022b700 == (void *)0x0) ||
              (iVar7 = FUN_001428fc(DAT_0022b700,DAT_0022b6f8,&DAT_0022b708,4),
              puVar5 = PTR_DAT_001cb6b0, iVar7 == 0)) || (DAT_0022b708 != 0x360013e8))
          goto LAB_00149ce4;
          memcpy(&DAT_0022b70c,PTR_DAT_001cb6b0,0x158);
          pvVar10 = DAT_0022b700;
          if (((((uint)DAT_0022b700 | (uint)DAT_0022b6f8) & 3) != 0) ||
             (((long)DAT_0022b700 - DAT_0022b6f8) - 0x7fffffdU < 0xfffffffff0000003))
          goto LAB_00149ce4;
          uVar14 = (uint)((long)DAT_0022b700 - DAT_0022b6f8);
          uVar17 = uVar14 + 3;
          if (-1 < (int)uVar14) {
            uVar17 = uVar14;
          }
          local_308 = CONCAT44(local_308._4_4_,uVar17 >> 2) & 0xffffffff03ffffff | 0x14000000;
          if ((DAT_0022b6f8 - (long)DAT_0022b700) - 0x8000141U < 0xfffffffff0000003)
          goto LAB_00149ce4;
          iVar7 = (int)(DAT_0022b6f8 - (long)DAT_0022b700);
          uVar16 = iVar7 - 0x144;
          uVar14 = iVar7 - 0x141;
          if (-1 < (int)uVar16) {
            uVar14 = uVar16;
          }
          if (((((uint)DAT_001e0978 | (uint)DAT_0022b700) & 3) != 0) ||
             ((DAT_001e0978 - (long)DAT_0022b700) - 0x779bdd1U < 0xfffffffff0000003))
          goto LAB_00149ce4;
          iVar7 = (int)(DAT_001e0978 - (long)DAT_0022b700);
          uVar16 = iVar7 + 0x86422c;
          uVar2 = iVar7 + 0x86422f;
          if (-1 < (int)uVar16) {
            uVar2 = uVar16;
          }
          DAT_0022b854 = uVar14 >> 2 & 0x3ffffff | 0x14000000;
          DAT_0022b858 = uVar2 >> 2 & 0x3ffffff | 0x14000000;
          DAT_0022b850 = 0x36000048;
          DAT_0022b85c = FUN_00173b54;
          DAT_0022b864 = uVar17 >> 2 & 0x3ffffff | 0x14000000;
          memcpy(DAT_0022b700,&DAT_0022b70c,0x158);
          FUN_001bdaf0(pvVar10,(long)pvVar10 + 0x158);
          iVar7 = mprotect(DAT_0022b700,DAT_00209d88,5);
          if (iVar7 != 0) goto LAB_00149ce4;
          lVar22 = FUN_0014bef8(DAT_001cfb4c,&local_308,4,DAT_0022b6f8);
          if (lVar22 != 4) {
LAB_00149c90:
            lVar22 = FUN_0014bef8(DAT_001cfb4c,&DAT_0022b708,4,DAT_0022b6f8);
            if (lVar22 != 4) {
                    /* WARNING: Subroutine does not return */
              abort();
            }
            uVar8 = FUN_001bdaf0(DAT_0022b6f8,DAT_0022b6f8 + 4);
            iVar7 = FUN_001428fc(uVar8,DAT_0022b6f8,&local_2c4,4);
            if ((iVar7 == 0) || (local_2c4 != DAT_0022b708)) {
                    /* WARNING: Subroutine does not return */
              abort();
            }
            goto LAB_00149ce4;
          }
          uVar8 = FUN_001bdaf0(DAT_0022b6f8,DAT_0022b6f8 + 4);
          iVar7 = FUN_001428fc(uVar8,DAT_0022b6f8,&local_2c4,4);
          if ((iVar7 == 0) || (local_2c4 != (uint)local_308)) goto LAB_00149c90;
          iVar7 = FUN_0016b860();
          if (iVar7 == 0) {
            pcVar21 = "outline_code_guards";
            goto LAB_00149cf4;
          }
          iVar7 = FUN_0016b934();
          if (iVar7 == 0) {
            FUN_001417c8("restored_optional_refused","trophies_hud_native_guards",0);
          }
          iVar7 = FUN_0016bd40();
          if (iVar7 == 0) {
            FUN_001417c8("script_port_refused","optional_gameplay_hook_guard",0);
          }
          FUN_0016c1e8();
          FUN_0016c61c();
          FUN_0016c748();
          FUN_0016c994();
          FUN_0016cb9c();
          FUN_0016ce3c();
          FUN_0016d20c();
          FUN_0016d2dc();
          FUN_0016d50c();
          FUN_0016d7d4();
          FUN_0016dc50();
          DAT_002fd5d0 = FUN_00173c58(0x821428,0x2f0,
                                      "978a122086fb2bc3e68ea887cde5158da0284963edb2483111865403cc52ff8e"
                                      ,0x821428,0xf9431408,FUN_0017e8b0);
          FUN_0016dfa8();
          FUN_0016e208();
          DAT_002fd604 = FUN_00173c58(0x85d6d8,0x3d8,
                                      "be9fca6debb3dcce563144f32c278edbf12eb2587bc56f20432e8b74dbfeced7"
                                      ,0x85d6d8,0x6dbc23e9,FUN_0017f330);
          DAT_00228ef0 = (code *)dlsym(DAT_0020d0f0,"nexus_evasion_register_restored_v1");
          DAT_00214930 = dlsym(DAT_0020d0f0,"nexus_evasion_snapshot_keys_v1");
          iVar7 = FUN_001503ac(DAT_00228ef0);
          if ((iVar7 != 0) && (iVar7 = FUN_001503ac(DAT_00214930), iVar7 != 0)) {
            __len = FUN_0019fb60();
            if (__len - 0x10001 < 0xffffffffffff0000) goto LAB_001489dc;
            DAT_0021e8d0 = mmap((void *)0x0,__len,3,0x22,-1,0);
            if (DAT_0021e8d0 == (void *)0xffffffffffffffff) {
              DAT_0021e8d0 = (void *)0x0;
              goto LAB_001489dc;
            }
            local_300 = DAT_001e0978;
            uStack_2f8 = 0;
            local_2f0 = FUN_001428fc;
            pcStack_2e8 = FUN_0016e2e0;
            local_308 = DAT_0010e7f8;
            local_2e0 = FUN_0016e3b0;
            pcStack_2d8 = FUN_0016e5e0;
            local_2d0 = FUN_0016e640;
            iVar7 = FUN_0019fb68(DAT_0021e8d0,__len,&local_308);
            lVar22 = DAT_001e0978;
            if (iVar7 == 0) {
              pcVar21 = "xray_guarded_binding";
LAB_00149de0:
              DAT_00228c58 = (code *)0x0;
              DAT_0022b868 = 0;
              DAT_00214938._0_4_ = 0;
              if (DAT_00228ef0 != (code *)0x0) {
                (*DAT_00228ef0)(0);
              }
              goto LAB_00149cf4;
            }
            lVar9 = DAT_001e0978 + 0xaa2830;
            pvVar10 = (void *)FUN_00152af0(lVar9);
            pcVar21 = "queue_plan";
            if ((((pvVar10 == (void *)0x0) || ((long)puVar18 - (long)puVar5 != 0x158)) ||
                (iVar7 = FUN_001428fc(pvVar10,lVar9,&local_30c,4), pcVar21 = "queue_plan",
                iVar7 == 0)) ||
               (iVar7 = FUN_00199954(&DAT_00228908,lVar9,pvVar10,FUN_0016e6d0,PTR_DAT_001cb6b0,0x158
                                     ,local_30c), iVar7 == 0)) goto LAB_00149de0;
            memcpy(pvVar10,&DAT_00228928,0x208);
            FUN_001bdaf0(pvVar10,(long)pvVar10 + 0x208);
            iVar7 = mprotect(pvVar10,DAT_00209d88,5);
            if (iVar7 != 0) {
              pcVar21 = "queue_seal";
              goto LAB_00149de0;
            }
            iVar7 = FUN_001419f8();
            if ((iVar7 == 0) ||
               (lVar12 = FUN_0014bef8(DAT_001cfb4c,&DAT_00228924,4,lVar9), lVar12 != 4)) {
              pcVar21 = "queue_publication";
LAB_00149ee0:
              DAT_00228c58 = (code *)0x0;
              DAT_0022b868 = 0;
              DAT_00214938._0_4_ = 0;
              if (DAT_00228ef0 != (code *)0x0) {
                (*DAT_00228ef0)(0);
              }
              lVar12 = FUN_0014bef8(DAT_001cfb4c,&local_30c,4,lVar9);
              if (lVar12 != 4) {
                FUN_001417c8("fatal","queue_restore_failed",0);
                    /* WARNING: Subroutine does not return */
                abort();
              }
              uVar8 = FUN_001bdaf0(lVar9,lVar22 + 0xaa2834);
              iVar7 = FUN_001428fc(uVar8,lVar9,&local_310,4);
              if ((iVar7 == 0) || (local_310 != local_30c)) {
                FUN_001417c8("fatal","queue_restore_unverified",0);
                    /* WARNING: Subroutine does not return */
                abort();
              }
              goto LAB_00149de0;
            }
            FUN_001bdaf0(lVar9,lVar22 + 0xaa2834);
            iVar7 = FUN_00168fe8();
            if (iVar7 == 0) {
              pcVar21 = "queue_readback";
              goto LAB_00149ee0;
            }
            DAT_00214938._0_4_ = 1;
            local_2b8 = DAT_00209d00;
            pcStack_2b0 = FUN_001455a4;
            local_2a8 = FUN_0015cfd8;
            pcStack_2a0 = FUN_00155244;
            local_2c0 = DAT_0010e6a0;
            memcpy(auStack_298,&DAT_00228908,0x228);
            iVar7 = (*DAT_00228ef0)(&local_2c0);
            if (iVar7 != 1) {
              pcVar21 = "queue_owner_registration";
              goto LAB_00149ee0;
            }
            pcVar11 = "restored_ready";
            pcVar21 = "xray_existing_input_and_visual_bound";
            pcVar13 = ",\"queue_observers\":1,\"new_inputs\":0";
            goto LAB_00149cf8;
          }
          DAT_00228ef0 = (code *)0x0;
        }
      }
LAB_001489dc:
      if (*(long *)(lVar3 + 0x28) == local_70) {
        return;
      }
      goto LAB_00148a2c;
    }
  }
  if (*(long *)(lVar3 + 0x28) == local_70) {
    pcVar21 = "hero_code_guards";
LAB_00148808:
    FUN_001417c8("restored_refused",pcVar21,0);
    return;
  }
LAB_00148a2c:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0015bfc4 @ 0015bfc4 [libNexusEvasionRuntime69252.so] ===== */

void FUN_0015bfc4(long param_1)

{
  undefined4 uVar1;
  uint uVar2;
  long lVar3;
  int iVar4;
  int iVar5;
  undefined4 *puVar6;
  long lVar7;
  undefined8 uVar8;
  ulong uVar9;
  long lVar10;
  long lVar11;
  undefined8 uVar12;
  ulong uVar13;
  undefined4 *puVar14;
  long lVar15;
  long lVar16;
  ulong local_1110;
  long local_10f8;
  char *local_10f0;
  undefined8 uStack_10e8;
  undefined4 uStack_10e0;
  int local_10dc;
  uint local_10d8;
  undefined4 uStack_10d4;
  undefined8 local_10d0;
  undefined4 uStack_10c8;
  int iStack_10c4;
  undefined8 uStack_10c0;
  undefined8 uStack_10b8;
  long local_10b0;
  uint local_10a8;
  int local_10a4;
  undefined8 local_10a0;
  long local_1098;
  long local_1090;
  long local_1088;
  long local_1080;
  long local_1078;
  undefined8 local_1070;
  int local_1068;
  long local_70;
  
  lVar3 = tpidr_el0;
  local_70 = *(long *)(lVar3 + 0x28);
  puVar6 = (undefined4 *)__errno();
  uVar1 = *puVar6;
  local_1078 = 0;
  lVar7 = FUN_00139fcc();
  DAT_0021ac40 = 0;
  if (((((param_1 != 0) && (DAT_0021c790 != 0)) && (*(int *)(param_1 + 0x24) != 0)) &&
      ((*(long *)(param_1 + 0x10) != 0 && (*(long *)(param_1 + 8) != 0)))) && (lVar7 != 0)) {
    local_1080 = CONCAT44(local_1080._4_4_,0xffffffff);
    local_10f0 = "trophiesAboveHead";
    if (((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
        ((iVar4 = (*DAT_00214930)(&local_10f0,&local_1070,1,&local_1078,&local_1080), iVar4 == 1 &&
         (((local_1078 != 0 && ((int)local_1080 == 0)) && (local_1068 == 2)))))) &&
       (((local_1070._4_4_ == 1 &&
         (lVar15 = *(long *)(param_1 + 0x10), *(long *)(lVar15 + 0x20) == lVar7)) &&
        (*(long *)(lVar15 + 0x18) == DAT_001e0028)))) {
      DAT_0021ac28 = *(undefined8 *)(lVar15 + 0x40);
      DAT_0021ac20 = *(undefined8 *)(lVar15 + 0x38);
      DAT_0021ac00 = *(undefined8 *)(param_1 + 0x28);
      local_10a8 = 0xffffffff;
      local_10a4 = 0;
      DAT_0021ac18 = *(undefined8 *)(lVar15 + 0x28);
      local_1088 = 0;
      local_1080 = 0;
      local_1098 = 0;
      local_1090 = 0;
      local_10a0 = 0;
      DAT_0021ac08 = local_1078;
      DAT_0021ac44 = 0;
      DAT_0021ac48 = 0;
      DAT_0021abf0 = lVar7;
      DAT_0021abf8 = *(long *)(lVar15 + 0x18);
      memset(&DAT_0021b490,0,0x1300);
      uVar8 = FUN_0013a78c(*(undefined8 *)(lVar15 + 0x38),&local_1080);
      if ((int)uVar8 == 0) {
LAB_0015c278:
        uVar12 = 1;
      }
      else {
        uVar8 = FUN_001428fc(uVar8,*(long *)(lVar15 + 0x38) + 0xc,(long)&local_10a0 + 4,4);
        uVar12 = 1;
        if ((((int)uVar8 != 0) && (0 < (int)local_10a0._4_4_)) && ((int)local_10a0._4_4_ < 0x41)) {
          uVar9 = FUN_001428fc(uVar8,*(long *)(lVar15 + 0x38) + 0xe0,&local_10a8,4);
          uVar12 = 1;
          if (((int)uVar9 != 0) && (-1 < (int)local_10a8)) {
            if ((int)local_10a0._4_4_ <= (int)local_10a8) goto LAB_0015c278;
            DAT_0021ac48 = local_10a0._4_4_;
            if (0 < (int)local_10a0._4_4_) {
              lVar16 = 0;
              uVar13 = 0;
              puVar14 = &DAT_0021b490;
              do {
                local_1070 = 0;
                uVar9 = FUN_001428fc(uVar9,lVar16 + local_1080,&local_1070,8);
                if ((((int)uVar9 != 0) && (uVar9 = local_1070, 0x11fff < local_1070 + 0x2000)) &&
                   ((local_1070 & 7) == 0)) {
                  uVar9 = FUN_00161818(local_1070,uVar13 & 0xffffffff,puVar14);
                }
                uVar13 = uVar13 + 1;
                puVar14 = puVar14 + 0x13;
                lVar16 = lVar16 + 8;
              } while ((long)uVar13 < (long)(int)local_10a0._4_4_);
            }
            iVar4 = FUN_0013a78c(*(long *)(lVar15 + 0x38) + 0x28,&local_1090);
            if ((iVar4 == 0) || (uVar8 = FUN_0013a78c(local_1090,&local_1098), (int)uVar8 == 0)) {
LAB_0015c428:
              uVar12 = 2;
            }
            else {
              uVar8 = FUN_001428fc(uVar8,local_1090 + 0xc,&local_10a0,4);
              uVar12 = 2;
              if ((int)uVar8 != 0) {
                if ((0 < (int)local_10a0) && ((int)local_10a0 < 0x201)) {
                  iVar4 = FUN_001428fc(uVar8,local_1098,&local_1070,(local_10a0 & 0xffffffff) << 3);
                  if (iVar4 == 0) goto LAB_0015c428;
                  iVar4 = FUN_00189c88(DAT_001e0978,*(undefined8 *)(lVar15 + 0x40),FUN_001428fc,0,
                                       &DAT_0021ac50);
                  uVar12 = 3;
                  if ((iVar4 != 0) && (DAT_0021ac7c != 0)) {
                    if ((DAT_0021ac60 == *(int *)(lVar15 + 0x70)) &&
                       (((DAT_0021ac68 == local_10a8 &&
                         ((&DAT_0021b490)[(long)(int)DAT_0021ac68 * 0x13] != 0)) &&
                        (uVar2 = *(uint *)(&DAT_0021b494 + (long)(int)DAT_0021ac68 * 0x4c),
                        uVar2 != 0)))) {
                      if (*(int *)(&DAT_0021b49c + (long)(int)DAT_0021ac68 * 0x4c) != DAT_0021ac64)
                      {
                        lVar16 = 0;
                        do {
                          lVar10 = lVar16;
                          if ((ulong)uVar2 - 1 == lVar10) break;
                          lVar16 = lVar10 + 1;
                        } while (*(int *)(&DAT_0021b4a0 +
                                         lVar10 * 4 + (long)(int)DAT_0021ac68 * 0x4c) !=
                                 DAT_0021ac64);
                        if ((ulong)uVar2 <= lVar10 + 1U) goto LAB_0015c438;
                      }
                      if ((int)local_10a0 < 1) {
                        uVar12 = 4;
                        local_10f8 = 0;
                      }
                      else {
                        lVar16 = 0;
                        iVar4 = 0;
                        local_1110 = 0;
                        do {
                          local_10b0 = 0;
                          if ((&local_1070)[lVar16] == *(long *)(lVar15 + 0x40)) {
                            iVar4 = iVar4 + 1;
                          }
                          else {
                            iVar5 = FUN_0013a78c((&local_1070)[lVar16],&local_10b0);
                            if ((((iVar5 != 0) &&
                                 ((local_10b0 == DAT_001e0978 + 0x1220040 ||
                                  (local_10b0 == DAT_001e0978 + 0x12200a0)))) &&
                                (iVar5 = FUN_00189c88(DAT_001e0978,(&local_1070)[lVar16],
                                                      FUN_001428fc,0,&local_10f0), iVar5 != 0)) &&
                               (iStack_10c4 != 0)) {
                              if (((local_10d8 < local_10a0._4_4_) && (local_10d8 != local_10a8)) &&
                                 (((&DAT_0021b490)[(ulong)local_10d8 * 0x13] != 0 &&
                                  (uVar2 = *(uint *)(&DAT_0021b494 + (ulong)local_10d8 * 0x4c),
                                  uVar2 != 0)))) {
                                if (*(int *)(&DAT_0021b49c + (ulong)local_10d8 * 0x4c) != local_10dc
                                   ) {
                                  lVar10 = 0;
                                  do {
                                    lVar11 = lVar10;
                                    if ((ulong)uVar2 - 1 == lVar11) break;
                                    lVar10 = lVar11 + 1;
                                  } while (*(int *)(&DAT_0021b4a0 +
                                                   lVar11 * 4 + (ulong)local_10d8 * 0x4c) !=
                                           local_10dc);
                                  if ((ulong)uVar2 <= lVar11 + 1U) goto LAB_0015c444;
                                }
                                uVar9 = 1L << ((ulong)local_10d8 & 0x3f);
                                if ((uVar9 & local_1110) == 0) {
                                  uVar13 = (ulong)DAT_0021ac44;
                                  local_1110 = uVar9 | local_1110;
                                  if (DAT_0021ac44 < 0x20) {
                                    DAT_0021ac44 = DAT_0021ac44 + 1;
                                    lVar10 = uVar13 * 0x40;
                                    *(undefined8 *)(lVar10 + 0x21ac98) = uStack_10e8;
                                    *(char **)(&DAT_0021ac90 + lVar10) = local_10f0;
                                    *(ulong *)(lVar10 + 0x21aca8) = CONCAT44(uStack_10d4,local_10d8)
                                    ;
                                    *(ulong *)(&DAT_0021aca0 + lVar10) =
                                         CONCAT44(local_10dc,uStack_10e0);
                                    *(ulong *)(lVar10 + 0x21acb8) =
                                         CONCAT44(iStack_10c4,uStack_10c8);
                                    *(undefined8 *)(&DAT_0021acb0 + lVar10) = local_10d0;
                                    *(undefined8 *)(lVar10 + 0x21acc8) = uStack_10b8;
                                    *(undefined8 *)(&DAT_0021acc0 + lVar10) = uStack_10c0;
                                  }
                                }
                              }
                            }
                          }
LAB_0015c444:
                          lVar16 = lVar16 + 1;
                        } while (lVar16 < (int)local_10a0);
                        local_10f8 = 0;
                        if (((((iVar4 == 1) && (lVar16 = FUN_00139fcc(), lVar16 == lVar7)) &&
                             (iVar4 = FUN_00155034("trophiesAboveHead",&local_10f8), iVar4 != 0)) &&
                            ((local_10f8 == local_1078 &&
                             (uVar8 = FUN_0013a78c(*(undefined8 *)(lVar15 + 0x38),&local_1088),
                             (int)uVar8 != 0)))) && (local_1088 == local_1080)) {
                          uVar12 = 4;
                          iVar4 = FUN_001428fc(uVar8,*(long *)(lVar15 + 0x38) + 0xc,&local_10a4,4);
                          if (iVar4 != 0) {
                            if (((local_10a4 != local_10a0._4_4_) ||
                                (iVar4 = FUN_0013a78c(*(long *)(lVar15 + 0x38) + 0x28,&local_1088),
                                iVar4 == 0)) ||
                               ((local_1088 != local_1090 ||
                                ((uVar8 = FUN_0013a78c(local_1088,&local_1088), (int)uVar8 == 0 ||
                                 (local_1088 != local_1098)))))) goto LAB_0015c6b8;
                            uVar12 = 4;
                            iVar4 = FUN_001428fc(uVar8,local_1090 + 0xc,&local_10a4,4);
                            if (iVar4 != 0) {
                              if (local_10a4 != (int)local_10a0) goto LAB_0015c6b8;
                              DAT_0021ac38 = local_1098;
                              DAT_0021ac30 = local_1090;
                              uVar12 = 0;
                              DAT_0021ac40 = 1;
                              DAT_0021ac4c = local_10a4;
                            }
                          }
                        }
                        else {
LAB_0015c6b8:
                          uVar12 = 4;
                        }
                      }
                    }
                    else {
LAB_0015c438:
                      uVar12 = 3;
                    }
                  }
                }
              }
            }
          }
        }
      }
      FUN_00161c20(uVar12);
      goto LAB_0015c1e4;
    }
  }
  DAT_0021abf0 = 0;
  DAT_0021ac44 = 0;
  DAT_0021ac48 = 0;
  memset(&DAT_0021b490,0,0x1300);
LAB_0015c1e4:
  *puVar6 = uVar1;
  if (*(long *)(lVar3 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_00161c20 @ 00161c20 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00161c20(uint param_1)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  ulong uVar4;
  ulong uVar5;
  ulong uVar6;
  long lVar7;
  int *piVar8;
  int iVar9;
  timespec local_118 [14];
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  iVar2 = clock_gettime(1,local_118);
  if (iVar2 == 0) {
    uVar5 = local_118[0].tv_sec * 1000 + (ulong)local_118[0].tv_nsec / 1000000;
    if ((DAT_0021ac10 - 1 < uVar5) && (uVar5 - DAT_0021ac10 < 2000)) goto LAB_00161d78;
  }
  else {
    uVar5 = 0;
  }
  uVar3 = (ulong)DAT_0021ac48;
  if (DAT_0021ac48 == 0) {
    uVar4 = 0;
  }
  else {
    if (DAT_0021ac48 == 1) {
      uVar6 = 0;
      uVar4 = 0;
    }
    else {
      uVar6 = uVar3 & 0xfffffffe;
      iVar2 = 0;
      iVar9 = 0;
      uVar4 = uVar6;
      piVar8 = &DAT_0021b4dc;
      do {
        uVar4 = uVar4 - 2;
        iVar2 = piVar8[-0x13] + iVar2;
        iVar9 = *piVar8 + iVar9;
        piVar8 = piVar8 + 0x26;
      } while (uVar4 != 0);
      uVar4 = (ulong)(uint)(iVar9 + iVar2);
      if (uVar6 == uVar3) goto LAB_00161d34;
    }
    lVar7 = uVar3 - uVar6;
    piVar8 = &DAT_0021b490 + uVar6 * 0x13;
    do {
      lVar7 = lVar7 + -1;
      uVar4 = (ulong)(uint)(*piVar8 + (int)uVar4);
      piVar8 = piVar8 + 0x13;
    } while (lVar7 != 0);
  }
LAB_00161d34:
  DAT_0021ac10 = uVar5;
  snprintf((char *)local_118,0xdc,
           ",\"capture_reason\":%u,\"roster\":%u,\"known\":%u,\"actors\":%u,\"epoch\":%llu,\"legacy_complete\":%u"
           ,(ulong)param_1,uVar3,uVar4,(ulong)DAT_0021ac44,DAT_0021abf0,DAT_0020f6cc);
  FUN_001417c8("trophies_native","v69_battle_intro_total",local_118);
LAB_00161d78:
  if (*(long *)(lVar1 + 0x28) != local_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_0016b934 @ 0016b934 [libNexusEvasionRuntime69252.so] ===== */

void FUN_0016b934(void)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  code *pcVar4;
  bool bVar5;
  int iVar6;
  int iVar7;
  undefined8 uVar8;
  char *pcVar9;
  ulong uVar10;
  undefined8 uVar11;
  undefined *puVar12;
  undefined **ppuVar13;
  long local_d8 [6];
  int local_a8 [2];
  code *local_a0;
  long local_58;
  
  lVar3 = tpidr_el0;
  local_58 = *(long *)(lVar3 + 0x28);
  iVar6 = FUN_001419f8();
  uVar2 = DAT_0021ca14;
  if ((iVar6 == 0) || (DAT_0022b930 != '\0')) {
LAB_0016ba20:
    bVar5 = uVar2 == 3;
  }
  else {
    uVar10 = 0;
    ppuVar13 = &PTR_DAT_001c5918;
    DAT_0022b930 = '\x01';
    do {
      puVar12 = ppuVar13[-2];
      uVar8 = FUN_0014b1d0(puVar12,*(undefined4 *)(ppuVar13 + -1),*ppuVar13);
      if ((int)uVar8 == 0) {
        if (puVar12 == (undefined *)0x5921a0) {
          pcVar4 = DAT_0021c9f8;
          if (DAT_0021c9f8 == (code *)0x0) {
            local_a8[0] = 0;
            local_a8[1] = 0;
            local_a0 = (code *)0x0;
            dl_iterate_phdr(FUN_00163228,local_a8);
            if ((local_a8[0] != 1) || (pcVar4 = local_a0, local_a0 == (code *)0x0))
            goto LAB_0016bc58;
          }
          DAT_0021c9f8 = pcVar4;
          uVar8 = (*DAT_0021c9f8)(DAT_001e0978);
          if ((int)uVar8 == 1) goto LAB_0016b9b0;
        }
LAB_0016bc58:
        snprintf((char *)local_d8,0x30,"native_body_guard_failed_%u",uVar10 & 0xffffffff);
        uVar2 = DAT_0022b934 + 1;
        bVar5 = 0x1f < DAT_0022b934;
        DAT_0022b934 = uVar2;
        if (bVar5) goto LAB_0016bd00;
        snprintf((char *)local_a8,0x50,",\"detail\":%u",(ulong)puVar12 & 0xffffffff);
        pcVar9 = (char *)local_d8;
        goto LAB_0016bcf8;
      }
LAB_0016b9b0:
      uVar10 = uVar10 + 1;
      ppuVar13 = ppuVar13 + 3;
    } while (uVar10 != 0x17);
    local_d8[0] = 0;
    uVar8 = FUN_001428fc(uVar8,DAT_001e0978 + 0x11be3f0,local_d8,8);
    if ((int)uVar8 == 0) {
      uVar11 = 0x11be3f0;
    }
    else {
      uVar11 = 0x11be3f0;
      if (local_d8[0] == DAT_001e0978 + 0x814068) {
        local_d8[0] = 0;
        uVar8 = FUN_001428fc(uVar8,DAT_001e0978 + 0x11be3f8,local_d8,8);
        uVar11 = 0x11be3f8;
        if (((int)uVar8 != 0) && (local_d8[0] == DAT_001e0978 + 0x814130)) {
          local_d8[0] = 0;
          uVar8 = FUN_001428fc(uVar8,DAT_001e0978 + 0x11be400,local_d8,8);
          uVar11 = 0x11be400;
          if (((int)uVar8 != 0) && (local_d8[0] == DAT_001e0978 + 0x8142e8)) {
            local_d8[0] = 0;
            uVar8 = FUN_001428fc(uVar8,DAT_001e0978 + 0x11ad300,local_d8,8);
            uVar11 = 0x11ad300;
            if (((int)uVar8 != 0) && (local_d8[0] == DAT_001e0978 + 0x5d6594)) {
              local_d8[0] = 0;
              uVar8 = FUN_001428fc(uVar8,DAT_001e0978 + 0x11ad210,local_d8,8);
              if ((int)uVar8 == 0) {
                uVar11 = 0x11ad210;
              }
              else {
                uVar11 = 0x11ad210;
                if (local_d8[0] == DAT_001e0978 + 0x5d48e8) {
                  uVar11 = 0x11abae0;
                  local_d8[0] = 0;
                  iVar6 = FUN_001428fc(uVar8,DAT_001e0978 + 0x11abae0,local_d8,8);
                  if ((iVar6 != 0) && (local_d8[0] == DAT_001e0978 + 0x58d1c4)) {
                    iVar6 = FUN_00173c58(0x8142e8,0x478,
                                         "9f53b0f37ccda2f0d7da4b6308e0b31c97f0a48697e3f84893b37938e8ae7495"
                                         ,0x8142e8,0xa9bd7bfd,FUN_00174b88);
                    iVar7 = FUN_00173c58(0x814068,0xc0,
                                         "aadd8dd7c5744aeaac7c9f0ddefd7fe8fba04b4d2aa8f755d1579959554c906d"
                                         ,0x814068,0xa9be7bfd,FUN_00174b88);
                    uVar2 = (uint)(iVar6 != 0);
                    if (iVar7 != 0) {
                      uVar2 = iVar6 != 0 | 2;
                    }
                    uVar1 = DAT_0022b934 + 1;
                    bVar5 = DAT_0022b934 < 0x20;
                    DAT_0021ca14 = uVar2;
                    DAT_0022b934 = uVar1;
                    if (bVar5) {
                      snprintf((char *)local_a8,0x50,",\"detail\":%u",(ulong)uVar2);
                      FUN_001417c8("trophies_hud","native_owner_hooks",local_a8);
                    }
                    goto LAB_0016ba20;
                  }
                }
              }
            }
          }
        }
      }
    }
    uVar2 = DAT_0022b934 + 1;
    bVar5 = DAT_0022b934 < 0x20;
    DAT_0022b934 = uVar2;
    if (bVar5) {
      snprintf((char *)local_a8,0x50,",\"detail\":%u",uVar11);
      pcVar9 = "native_vtable_guard_failed";
LAB_0016bcf8:
      FUN_001417c8("trophies_hud",pcVar9,local_a8);
    }
LAB_0016bd00:
    bVar5 = false;
  }
  if (*(long *)(lVar3 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(bVar5);
  }
  return;
}

/* ===== FUN_00174b88 @ 00174b88 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00174b88(long *param_1)

{
  bool bVar1;
  uint uVar2;
  code *pcVar3;
  long lVar4;
  long lVar5;
  undefined4 uVar6;
  long lVar7;
  int iVar8;
  undefined4 *puVar9;
  undefined8 uVar10;
  long *plVar11;
  long lVar12;
  long lVar13;
  undefined1 auVar14 [16];
  int local_bc;
  long local_b8 [10];
  long local_68;
  
  lVar7 = tpidr_el0;
  local_68 = *(long *)(lVar7 + 0x28);
  puVar9 = (undefined4 *)__errno();
  uVar6 = *puVar9;
  if ((param_1 != (long *)0x0) && (FUN_00161de4(*param_1), DAT_0021ca14 != 0)) {
    lVar13 = *param_1;
    plVar11 = &DAT_0022b938;
    lVar12 = 0x40;
    do {
      if (*plVar11 == lVar13 && lVar13 != 0) {
        uVar10 = FUN_00174ee8(plVar11);
        if ((int)uVar10 == 0) {
          uVar2 = DAT_0022b934 + 1;
          bVar1 = DAT_0022b934 < 0x20;
          DAT_0022b934 = uVar2;
          if (bVar1) {
            snprintf((char *)local_b8,0x50,",\"detail\":%u",0);
            FUN_001417c8("trophies_hud","release_ownership_refused",local_b8);
          }
        }
        else {
          lVar4 = plVar11[3];
          lVar5 = plVar11[4];
          local_b8[0] = 1;
          local_bc = 0;
          uVar10 = FUN_001428fc(uVar10,lVar4 + 0x38,local_b8,8);
          if (((((int)uVar10 == 0) || (local_b8[0] != 0)) ||
              (uVar10 = FUN_001428fc(uVar10,lVar4 + 0x40,&local_bc,4), (int)uVar10 == 0)) ||
             (local_bc != -1)) {
            uVar10 = (*(code *)(DAT_001e0978 + 0x59567c))(lVar4);
          }
          local_b8[0] = 1;
          local_bc = 0;
          uVar10 = FUN_001428fc(uVar10,lVar5 + 0x38,local_b8,8);
          if ((((int)uVar10 == 0) || (local_b8[0] != 0)) ||
             ((uVar10 = FUN_001428fc(uVar10,lVar5 + 0x40,&local_bc,4), (int)uVar10 == 0 ||
              (local_bc != -1)))) {
            uVar10 = (*(code *)(DAT_001e0978 + 0x59567c))(lVar5);
          }
          local_b8[0] = 1;
          local_bc = 0;
          uVar10 = FUN_001428fc(uVar10,lVar4 + 0x38,local_b8,8);
          if ((((int)uVar10 != 0) && (local_b8[0] == 0)) &&
             ((uVar10 = FUN_001428fc(uVar10,lVar4 + 0x40,&local_bc,4), (int)uVar10 != 0 &&
              (local_bc == -1)))) {
            local_b8[0] = 1;
            local_bc = 0;
            uVar10 = FUN_001428fc(uVar10,lVar5 + 0x38,local_b8,8);
            if (((((int)uVar10 != 0) && (local_b8[0] == 0)) &&
                (iVar8 = FUN_001428fc(uVar10,lVar5 + 0x40,&local_bc,4), iVar8 != 0)) &&
               (local_bc == -1)) {
              pcVar3 = (code *)(DAT_001e0978 + 0x58d1c4);
              plVar11[3] = 0;
              plVar11[2] = 0;
              plVar11[5] = 0;
              plVar11[4] = 0;
              plVar11[7] = 0;
              plVar11[6] = 0;
              plVar11[9] = 0;
              plVar11[8] = 0;
              plVar11[1] = 0;
              *plVar11 = 0;
              (*pcVar3)(lVar4);
              (*(code *)(DAT_001e0978 + 0x5d48e8))(lVar5);
              DAT_0022cf38 = DAT_0022cf38 + 1;
              goto LAB_00174c54;
            }
          }
          uVar2 = DAT_0022b934 + 1;
          bVar1 = DAT_0022b934 < 0x20;
          DAT_0022b934 = uVar2;
          if (bVar1) {
            snprintf((char *)local_b8,0x50,",\"detail\":%u",0);
            FUN_001417c8("trophies_hud","release_detach_refused",local_b8);
          }
        }
      }
LAB_00174c54:
      plVar11 = plVar11 + 10;
      lVar12 = lVar12 + -1;
    } while (lVar12 != 0);
    lVar12 = 0;
    do {
      auVar14._8_8_ = lVar13;
      auVar14._0_8_ = lVar13;
      auVar14 = NEON_cmeq(*(undefined1 (*) [16])((long)&DAT_0022cd38 + lVar12),auVar14,8);
      if ((auVar14 & (undefined1  [16])0x1) != (undefined1  [16])0x0) {
        *(undefined8 *)((long)&DAT_0022cd38 + lVar12) = 0;
      }
      if ((auVar14 & (undefined1  [16])0x1) != (undefined1  [16])0x0) {
        *(undefined8 *)((long)&DAT_0022cd40 + lVar12) = 0;
      }
      lVar12 = lVar12 + 0x10;
    } while (lVar12 != 0x200);
  }
  *puVar9 = uVar6;
  if (*(long *)(lVar7 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00175220 @ 00175220 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00175220(long param_1)

{
  bool bVar1;
  int *piVar2;
  uint uVar3;
  undefined4 uVar4;
  char cVar5;
  bool bVar6;
  long lVar7;
  ulong uVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  undefined4 *puVar12;
  undefined8 uVar13;
  ulong uVar14;
  ulong uVar15;
  ulong uVar16;
  ulong uVar17;
  char *pcVar18;
  long lVar19;
  ulong uVar20;
  code *pcVar21;
  long lVar22;
  ulong *puVar23;
  ulong uVar24;
  long *plVar25;
  undefined1 (*pauVar26) [16];
  ulong uVar27;
  ulong uVar28;
  ulong *puVar29;
  undefined **ppuVar30;
  long lVar31;
  ulong *puVar32;
  float fVar33;
  int iVar34;
  int iVar35;
  float fVar36;
  int iVar37;
  int iVar38;
  int iVar39;
  undefined1 auVar40 [16];
  undefined1 auVar41 [16];
  undefined1 auVar42 [16];
  undefined1 auVar43 [16];
  ulong *local_1190;
  int local_1188;
  undefined4 uStack_1184;
  int local_1178;
  int local_1174;
  uint local_1170;
  int local_1160;
  int local_115c;
  ulong local_1148;
  ulong local_1140;
  ulong local_1138;
  ulong local_1130;
  ulong local_1128;
  ulong local_1120;
  ulong local_1118;
  uint local_110c;
  undefined8 local_1108;
  ulong local_1100;
  ulong local_10f8;
  ulong local_10f0;
  ulong local_10e8;
  undefined8 local_10e0;
  code *pcStack_10d8;
  undefined1 auStack_10c0 [4064];
  undefined8 local_e0;
  undefined8 uStack_d8;
  int local_d0 [16];
  undefined8 local_90;
  long local_88;
  
  lVar7 = tpidr_el0;
  local_88 = *(long *)(lVar7 + 0x28);
  puVar12 = (undefined4 *)__errno();
  uVar4 = *puVar12;
  if (param_1 == 0) goto LAB_00175954;
  uVar28 = *(ulong *)(param_1 + 0x98);
  local_1120 = 0;
  local_1118 = 0;
  local_1130 = 0;
  local_1128 = 0;
  local_1140 = 0;
  local_1138 = 0;
  do {
    lVar19 = DAT_0022cf48;
    cVar5 = '\x01';
    bVar6 = (bool)ExclusiveMonitorPass(0x22cf48,0x10);
    if (bVar6) {
      cVar5 = ExclusiveMonitorsStatus();
      DAT_0022cf48 = DAT_0022cf48 + 1;
    }
  } while (cVar5 != '\0');
  if ((lVar19 == 0) &&
     (uVar3 = DAT_0022b934 + 1, bVar6 = DAT_0022b934 < 0x20, DAT_0022b934 = uVar3, bVar6)) {
    snprintf((char *)&local_10e0,0x50,",\"detail\":%u",0x13);
    FUN_001417c8("trophies_hud","update_callback_seen",&local_10e0);
  }
  iVar9 = DAT_00209cd8;
  if (DAT_0021ca14 == 3) {
    if ((DAT_00209cd8 == 0) || (uVar13 = gettid(), iVar9 != (int)uVar13)) {
      uVar20 = 0;
      lVar19 = 2;
      goto LAB_00175804;
    }
    local_10e0 = 0;
    uVar13 = FUN_001428fc(uVar13,uVar28,&local_10e0,8);
    if (((((int)uVar13 == 0) || (local_10e0 + 0x2000 < 0x12000)) || ((local_10e0 & 7) != 0)) ||
       (local_10e0 != DAT_001e0978 + 0x11be3f0U)) {
      uVar20 = 0;
      lVar19 = 3;
      goto LAB_00175804;
    }
    uVar13 = FUN_001428fc(uVar13,uVar28 + 0x590,&local_1118,8);
    if ((int)uVar13 == 0) {
LAB_00175990:
      uVar20 = 0;
      lVar19 = 4;
      goto LAB_00175804;
    }
    uVar20 = 0;
    lVar19 = 4;
    if ((local_1118 + 0x2000 < 0x12000) || ((local_1118 & 7) != 0)) goto LAB_00175804;
    local_10e0 = 0;
    uVar13 = FUN_001428fc(uVar13,local_1118,&local_10e0,8,0);
    if ((((int)uVar13 == 0) || ((local_10e0 + 0x2000 < 0x12000 || ((local_10e0 & 7) != 0)))) ||
       (local_10e0 != DAT_001e0978 + 0x11ad208U)) goto LAB_00175990;
    iVar9 = FUN_001428fc(uVar13,uVar28 + 0x10,&local_1120,8);
    uVar20 = 0;
    if (iVar9 == 0) {
      lVar19 = 5;
      goto LAB_00175804;
    }
    lVar19 = 5;
    if ((local_1120 + 0x2000 < 0x12000) || ((local_1120 & 7) != 0)) goto LAB_00175804;
    puVar32 = (ulong *)0x0;
    lVar19 = 0x40;
    puVar23 = &DAT_0022b938;
    puVar29 = (ulong *)0x0;
    do {
      local_1190 = puVar23;
      if (*puVar23 != uVar28) {
        local_1190 = puVar29;
      }
      if (*puVar23 == 0 && puVar32 == (ulong *)0x0) {
        puVar32 = puVar23;
      }
      lVar19 = lVar19 + -1;
      puVar23 = puVar23 + 10;
      puVar29 = local_1190;
    } while (lVar19 != 0);
    if (local_1190 != (ulong *)0x0) {
      if ((local_1190[2] == local_1118) && (iVar9 = FUN_00174ee8(local_1190), iVar9 != 0)) {
        local_10e0 = local_10e0 & 0xffffffffffffff00;
        FUN_0014bef8(DAT_001cfb4c,&local_10e0,1,local_1190[3] + 8);
        local_10e0 = local_10e0 & 0xffffffffffffff00;
        FUN_0014bef8(DAT_001cfb4c,&local_10e0,1,local_1190[4] + 8);
        *(undefined4 *)(local_1190 + 9) = 0;
      }
      if (((local_1190[2] != local_1118) || (local_1190[1] != local_1120)) ||
         (iVar9 = FUN_00174ee8(local_1190), iVar9 == 0)) {
        uVar20 = 0;
        lVar19 = 6;
        goto LAB_00175804;
      }
    }
    local_1148 = 0;
    uVar14 = FUN_00139fcc();
    if (uVar14 == 0) {
      uVar20 = 0;
      lVar19 = 7;
      goto LAB_00175804;
    }
    local_1188 = -1;
    local_e0 = "trophiesAboveHead";
    if ((((((int)DAT_00214938 != 1) || (DAT_00214930 == (code *)0x0)) ||
         ((iVar9 = (*DAT_00214930)(&local_e0,&local_10e0,1,&local_1148,&local_1188), iVar9 != 1 ||
          ((local_1148 == 0 || (local_1188 != 0)))))) || ((int)pcStack_10d8 != 2)) ||
       (local_10e0._4_4_ != 1)) {
      uVar20 = 0;
      lVar19 = 8;
      goto LAB_00175804;
    }
    uVar13 = FUN_00189c88(DAT_001e0978,local_1120,FUN_001428fc,0,&local_1188);
    lVar19 = DAT_001e0048;
    if ((int)uVar13 == 0) {
      uVar20 = 0;
      lVar19 = 9;
      goto LAB_00175804;
    }
    if (local_115c == 0) {
      uVar20 = 0;
      lVar19 = 10;
      goto LAB_00175804;
    }
    local_90 = 0;
    local_10f0 = 0;
    local_10e8 = 0;
    local_1100 = 0;
    local_10f8 = 0;
    local_1108 = 0;
    local_110c = 0;
    local_d0[0xd] = 0;
    local_d0[0xe] = 0;
    local_d0[0xc] = 0;
    local_d0[6] = 0;
    local_d0[7] = 0;
    local_d0[4] = 0;
    local_d0[5] = 0;
    local_d0[10] = 0;
    local_d0[0xb] = 0;
    local_d0[8] = 0;
    local_d0[9] = 0;
    uStack_d8 = 0;
    local_e0 = (char *)0x0;
    local_d0[2] = 0;
    local_d0[3] = 0;
    local_d0[0] = 0;
    local_d0[1] = 0;
    if ((((DAT_0021c790 == 0) || (local_1160 == 0)) ||
        (uVar13 = FUN_001428fc(uVar13,DAT_001e0048,&local_90,8), (int)uVar13 == 0)) ||
       ((local_90 + 0x2000 < 0x12000 || ((local_90 & 7) != 0)))) {
LAB_00175a10:
      uVar20 = 0;
      lVar19 = 0xb;
      goto LAB_00175804;
    }
    uVar13 = FUN_001428fc(uVar13,lVar19 + 0xc,(long)&local_1108 + 4,4);
    if ((((int)uVar13 == 0) || (((int)local_1108._4_4_ < 1 || (0x40 < (int)local_1108._4_4_)))) ||
       ((local_1108._4_4_ <= local_1170 ||
        ((((iVar9 = FUN_001428fc(uVar13,local_90 + (ulong)local_1170 * 8,&local_10e8,8), iVar9 == 0
           || (local_10e8 + 0x2000 < 0x12000)) || ((local_10e8 & 7) != 0)) ||
         ((uVar13 = FUN_00161818(local_10e8,local_1170,&local_e0), (int)uVar13 == 0 ||
          ((int)local_e0 == 0)))))))) goto LAB_00175a10;
    if (local_e0._4_4_ == 0) goto LAB_00175a10;
    if (uStack_d8._4_4_ != local_1174) {
      uVar20 = 0;
      do {
        if ((ulong)local_e0._4_4_ - 1 == uVar20) goto LAB_00175a10;
        piVar2 = local_d0 + uVar20;
        uVar20 = uVar20 + 1;
      } while (*piVar2 != local_1174);
      if (local_e0._4_4_ <= uVar20) goto LAB_00175a10;
    }
    uVar13 = FUN_001428fc(uVar13,lVar19,&local_10f0,8);
    if ((((int)uVar13 == 0) || (0xfffffffffffedfff < local_10f0 - 0x10000)) ||
       (((local_10f0 & 7) != 0 ||
        ((local_10f0 != local_90 ||
         (uVar13 = FUN_001428fc(uVar13,lVar19 + 0xc,&local_1108,4), (int)uVar13 == 0))))))
    goto LAB_00175a10;
    if ((((((uint)local_1108 != local_1108._4_4_) ||
          (((uVar13 = FUN_001428fc(uVar13,lVar19 + 0x28,&local_10f8,8), (int)uVar13 == 0 ||
            (local_10f8 + 0x2000 < 0x12000)) || ((local_10f8 & 7) != 0)))) ||
         (((uVar13 = FUN_001428fc(uVar13,local_10f8,&local_1100,8), (int)uVar13 == 0 ||
           (local_1100 + 0x2000 < 0x12000)) ||
          (((local_1100 & 7) != 0 ||
           ((uVar13 = FUN_001428fc(uVar13,local_10f8 + 0xc,&local_110c,4), (int)uVar13 == 0 ||
            ((int)local_110c < 1)))))))) || (0x200 < (int)local_110c)) ||
       ((uVar13 = FUN_001428fc(uVar13,local_1100,&local_10e0,(ulong)local_110c << 3),
        (int)uVar13 == 0 || (uVar20 = (ulong)local_110c, (int)local_110c < 1)))) goto LAB_00175a10;
    lVar19 = CONCAT44(uStack_1184,local_1188);
    if (local_110c < 8) {
      uVar24 = 0;
      iVar9 = 0;
LAB_00175a80:
      lVar22 = uVar20 - uVar24;
      plVar25 = &local_10e0 + uVar24;
      do {
        if (*plVar25 == lVar19) {
          iVar9 = iVar9 + 1;
        }
        lVar22 = lVar22 + -1;
        plVar25 = plVar25 + 1;
      } while (lVar22 != 0);
    }
    else {
      uVar24 = uVar20 & 0xfffffff8;
      iVar9 = 0;
      iVar10 = 0;
      iVar11 = 0;
      iVar34 = 0;
      iVar35 = 0;
      iVar37 = 0;
      iVar38 = 0;
      iVar39 = 0;
      pauVar26 = (undefined1 (*) [16])auStack_10c0;
      uVar27 = uVar24;
      do {
        uVar27 = uVar27 - 8;
        auVar40._8_8_ = lVar19;
        auVar40._0_8_ = lVar19;
        auVar40 = NEON_cmeq(pauVar26[-2],auVar40,8);
        auVar41._8_8_ = lVar19;
        auVar41._0_8_ = lVar19;
        auVar41 = NEON_cmeq(pauVar26[-1],auVar41,8);
        auVar42._8_8_ = lVar19;
        auVar42._0_8_ = lVar19;
        auVar42 = NEON_cmeq(*pauVar26,auVar42,8);
        auVar43._8_8_ = lVar19;
        auVar43._0_8_ = lVar19;
        auVar43 = NEON_cmeq(pauVar26[1],auVar43,8);
        iVar9 = iVar9 - auVar40._0_4_;
        iVar10 = iVar10 - auVar40._8_4_;
        iVar11 = iVar11 - auVar41._0_4_;
        iVar34 = iVar34 - auVar41._8_4_;
        iVar35 = iVar35 - auVar42._0_4_;
        iVar37 = iVar37 - auVar42._8_4_;
        iVar38 = iVar38 - auVar43._0_4_;
        iVar39 = iVar39 - auVar43._8_4_;
        pauVar26 = pauVar26 + 4;
      } while (uVar27 != 0);
      iVar9 = iVar35 + iVar9 + iVar37 + iVar10 + iVar38 + iVar11 + iVar39 + iVar34;
      if (uVar24 != uVar20) goto LAB_00175a80;
    }
    if ((((((iVar9 != 1) ||
           (uVar13 = FUN_001428fc(uVar13,local_10f8,&local_10f0,8), (int)uVar13 == 0)) ||
          (0xfffffffffffedfff < local_10f0 - 0x10000)) ||
         (((local_10f0 & 7) != 0 || (local_10f0 != local_1100)))) ||
        (uVar13 = FUN_001428fc(uVar13,local_10f8 + 0xc,&local_1108,4), uVar27 = uStack_d8,
        (int)uVar13 == 0)) ||
       (((uint)local_1108 != local_110c || (iVar9 = (int)uStack_d8, (int)uStack_d8 < 0))))
    goto LAB_00175a10;
    uVar13 = FUN_001428fc(uVar13,local_1120 + 0x20,&local_1128,8);
    if ((int)uVar13 == 0) {
LAB_00175d8c:
      uVar20 = 0;
      lVar19 = 0xc;
      goto LAB_00175804;
    }
    uVar20 = 0;
    lVar19 = 0xc;
    if (((0xfffffffffffedfff < local_1128 - 0x10000) || ((local_1128 & 7) != 0)) ||
       (local_1128 != uVar28)) goto LAB_00175804;
    uVar24 = uVar28 + 0x2f0;
    local_10e0 = 0;
    uVar13 = FUN_001428fc(uVar13,uVar24,&local_10e0,8,0);
    if ((((int)uVar13 == 0) || (local_10e0 + 0x2000 < 0x12000)) ||
       (((local_10e0 & 7) != 0 ||
        ((local_10e0 != DAT_001e0978 + 0x11be1d8U ||
         (uVar13 = FUN_001428fc(uVar13,uVar28 + 0x388,&local_1130,8), (int)uVar13 == 0))))))
    goto LAB_00175d8c;
    uVar20 = 0;
    lVar19 = 0xc;
    if ((local_1130 + 0x2000 < 0x12000) || ((local_1130 & 7) != 0)) goto LAB_00175804;
    uVar13 = FUN_001428fc(uVar13,uVar28 + 0x390,&local_1138,8,0);
    if ((int)uVar13 == 0) goto LAB_00175d8c;
    uVar20 = 0;
    lVar19 = 0xc;
    if (((0xfffffffffffedfff < local_1138 - 0x10000) || ((local_1138 & 7) != 0)) ||
       (local_1138 != uVar28)) goto LAB_00175804;
    iVar10 = FUN_001428fc(uVar13,uVar28 + 0x398,&local_1140,8,0);
    if (iVar10 == 0) goto LAB_00175d8c;
    uVar20 = 0;
    lVar19 = 0xc;
    if (((0xfffffffffffedfff < local_1140 - 0x10000) || ((local_1140 & 7) != 0)) ||
       (local_1140 != local_1118)) goto LAB_00175804;
    iVar10 = FUN_00164418(local_1140,uVar24);
    if (iVar10 == 0) {
      uVar20 = 0;
      lVar19 = 0xd;
      goto LAB_00175804;
    }
    lVar22 = (*(code *)(DAT_001e0978 + 0x5d8ad4))(local_1118,"player_name");
    local_10e0 = 0;
    iVar10 = FUN_001428fc(lVar22,lVar22,&local_10e0,8);
    if (((iVar10 == 0) || (local_10e0 + 0x2000 < 0x12000)) ||
       (((local_10e0 & 7) != 0 || (local_10e0 != DAT_001e0978 + 0x11abad8U)))) {
      uVar20 = 0;
      lVar19 = 0xe;
      goto LAB_00175804;
    }
    uVar13 = FUN_0016413c(lVar22,local_1118);
    if ((int)uVar13 == 0) {
      uVar20 = 0;
      lVar19 = 0xf;
      goto LAB_00175804;
    }
    uVar13 = FUN_001428fc(uVar13,lVar22 + 0x20,&local_90,8);
    uVar20 = 0;
    if ((int)uVar13 == 0) {
      lVar19 = 0x10;
      goto LAB_00175804;
    }
    lVar19 = 0x10;
    fVar33 = ABS((float)local_90);
    if ((fVar33 == INFINITY) || (NAN(fVar33))) goto LAB_00175804;
    uVar20 = 0;
    fVar36 = ABS(local_90._4_4_);
    if ((fVar36 != 4096.0 && fVar36 < 4096.0 == NAN(fVar36)) ||
       (((fVar36 == INFINITY || (NAN(fVar36))) ||
        (fVar33 != 4096.0 && fVar33 < 4096.0 == NAN(fVar33))))) goto LAB_00175804;
    lVar31 = 0;
    bVar6 = false;
    do {
      uVar20 = (ulong)*(uint *)((long)&DAT_001213c8 + lVar31);
      local_e0 = (char *)0x0;
      local_10e8 = local_10e8 & 0xffffffffffffff00;
      uVar13 = FUN_001428fc(uVar13,uVar28 + uVar20,&local_e0,8);
      if ((int)uVar13 == 0) {
        lVar19 = 0x11;
        goto LAB_00175804;
      }
      if (local_e0 != (char *)0x0) {
        local_10e0 = 0;
        iVar10 = FUN_001428fc(uVar13,local_e0,&local_10e0,8);
        if (((iVar10 == 0) || (local_10e0 + 0x2000 < 0x12000)) ||
           (((local_10e0 & 7) != 0 || (local_10e0 != DAT_001e0978 + 0x11ad208U)))) {
          lVar19 = 0x12;
          goto LAB_00175804;
        }
        uVar13 = FUN_0016413c(local_e0,local_1118);
        if ((int)uVar13 == 0) {
          lVar19 = 0x13;
          goto LAB_00175804;
        }
        uVar13 = FUN_001428fc(uVar13,local_e0 + 8,&local_10e8,1);
        lVar19 = 0x14;
        if (((int)uVar13 == 0) || (1 < (byte)local_10e8)) goto LAB_00175804;
        if ((byte)local_10e8 != 0) {
          bVar6 = true;
        }
      }
      uVar8 = local_1118;
      uVar20 = local_1120;
      iVar10 = local_1178;
      lVar31 = lVar31 + 4;
    } while (lVar31 != 0x20);
    if (local_1190 == (ulong *)0x0) {
      lVar19 = 0;
      do {
        if (*(ulong *)((long)&DAT_0022cd38 + lVar19) == uVar28) {
          uVar20 = 0;
          lVar19 = 0x15;
          goto LAB_00175804;
        }
        lVar19 = lVar19 + 8;
      } while (lVar19 != 0x200);
      if (puVar32 == (ulong *)0x0) {
        lVar19 = 0x16;
      }
      else {
        uVar15 = (*(code *)(DAT_001e0978 + 0x51ecec))("sc/ui.sc","popover_text_left",1);
        local_10e0 = 0;
        uVar13 = FUN_001428fc(uVar15,uVar15,&local_10e0,8);
        if (((((int)uVar13 != 0) && (0x11fff < local_10e0 + 0x2000)) && ((local_10e0 & 7) == 0)) &&
           (local_10e0 == DAT_001e0978 + 0x11ad208U)) {
          local_e0 = (char *)((ulong)local_e0 & 0xffffffff00000000);
          local_10e0 = 1;
          uVar13 = FUN_001428fc(uVar13,uVar15 + 0x38,&local_10e0,8);
          if ((((int)uVar13 != 0) && (local_10e0 == 0)) &&
             ((iVar11 = FUN_001428fc(uVar13,uVar15 + 0x40,&local_e0,4), iVar11 != 0 &&
              ((int)local_e0 == -1)))) {
            uVar16 = (*(code *)(DAT_001e0978 + 0x5d8ad4))(uVar15,&DAT_0011ae2d);
            local_10e0 = 0;
            iVar11 = FUN_001428fc(uVar16,uVar16,&local_10e0,8);
            if (((iVar11 == 0) || (local_10e0 + 0x2000 < 0x12000)) ||
               (((local_10e0 & 7) != 0 || (local_10e0 != DAT_001e0978 + 0x11abad8U))))
            goto LAB_00176208;
            uVar13 = FUN_001642f8(uVar15,uVar16);
            if ((int)uVar13 != 1) {
              FUN_001648a4(uVar15,uVar16,0);
              uVar3 = DAT_0022b934 + 1;
              bVar6 = DAT_0022b934 < 0x20;
              DAT_0022b934 = uVar3;
              if (bVar6) {
                snprintf((char *)&local_10e0,0x50,",\"detail\":%u",0);
                pcVar18 = "donor_named_contract";
LAB_00176978:
                FUN_001417c8("trophies_hud",pcVar18,&local_10e0);
              }
              goto LAB_00176a60;
            }
            local_e0 = (char *)((ulong)local_e0 & 0xffffffff00000000);
            local_10e0 = 1;
            uVar13 = FUN_001428fc(uVar13,uVar16 + 0x38,&local_10e0,8);
            if (((((int)uVar13 != 0) && (local_10e0 == 0)) &&
                (uVar13 = FUN_001428fc(uVar13,uVar16 + 0x40,&local_e0,4), (int)uVar13 != 0)) &&
               ((int)local_e0 == -1)) {
              local_10e0 = local_10e0 & 0xffffffffffff0000;
              iVar11 = FUN_001428fc(uVar13,uVar15 + 0x4e,&local_10e0,2);
              if ((iVar11 == 0) || (0x1ff < (ushort)local_10e0)) goto LAB_00176208;
              (*(code *)(DAT_001e0978 + 0x594138))(uVar15,uVar16);
            }
            iVar11 = FUN_00164418(uVar16,uVar15);
            if (iVar11 == 0) {
LAB_00176208:
              pcVar21 = (code *)(DAT_001e0978 + 0x5d48e8);
            }
            else {
              uVar17 = (*(code *)(DAT_001e0978 + 0x51ecec))("sc/ui.sc","icon_trophy",1);
              local_10e0 = 0;
              uVar13 = FUN_001428fc(uVar17,uVar17,&local_10e0,8);
              if ((((int)uVar13 == 0) || (local_10e0 + 0x2000 < 0x12000)) ||
                 (((local_10e0 & 7) != 0 || (local_10e0 != DAT_001e0978 + 0x11ad208U))))
              goto LAB_00176208;
              local_e0 = (char *)((ulong)local_e0 & 0xffffffff00000000);
              local_10e0 = 1;
              uVar13 = FUN_001428fc(uVar13,uVar17 + 0x38,&local_10e0,8);
              if (((int)uVar13 == 0) || (local_10e0 != 0)) goto LAB_00176208;
              iVar11 = FUN_001428fc(uVar13,uVar17 + 0x40,&local_e0,4);
              if ((iVar11 == 0) || ((int)local_e0 != -1)) goto LAB_00176208;
              local_10e8 = CONCAT44(local_10e8._4_4_,0xffffd700);
              local_10f0 = CONCAT62(local_10f0._2_6_,0x10);
              local_10f8 = CONCAT71(local_10f8._1_7_,1);
              local_1100 = local_1100 & 0xffffffffffffff00;
              lVar19 = FUN_0014bef8(DAT_001cfb4c,&local_10e8,4,uVar16 + 0x80);
              if ((((lVar19 == 4) &&
                   (lVar19 = FUN_0014bef8(DAT_001cfb4c,&local_10f0,2,uVar16 + 0xb0), lVar19 == 2))
                  && (lVar19 = FUN_0014bef8(DAT_001cfb4c,&local_10f8,1,uVar16 + 0x90), lVar19 == 1))
                 && (lVar19 = FUN_0014bef8(DAT_001cfb4c,&local_1100,1,uVar16 + 0x96), lVar19 == 1))
              {
                local_10e0 = local_10e0 & 0xffffffffffffff00;
                lVar19 = FUN_0014bef8(DAT_001cfb4c,&local_10e0,1,uVar16 + 8);
                if (lVar19 != 1) goto LAB_00176898;
                local_10e0 = local_10e0 & 0xffffffffffffff00;
                lVar19 = FUN_0014bef8(DAT_001cfb4c,&local_10e0,1,uVar17 + 8);
                if (lVar19 != 1) goto LAB_00176898;
                (*(code *)(DAT_001e0978 + 0x595330))(0x3e19999a,uVar17);
                (*(code *)(DAT_001e0978 + 0x594138))(uVar8,uVar16,0);
                iVar11 = FUN_00164418(uVar16,uVar8);
                if ((iVar11 == 0) || (iVar11 = FUN_001646e8(uVar15,uVar16), iVar11 == 0)) {
                  iVar9 = FUN_00164418(uVar16,uVar8);
                  if (iVar9 != 0) {
                    (*(code *)(DAT_001e0978 + 0x59567c))(uVar16);
                    (*(code *)(DAT_001e0978 + 0x594138))(uVar15,uVar16,0);
                  }
                  FUN_001648a4(uVar15,uVar16,uVar8);
                  (*(code *)(DAT_001e0978 + 0x5d48e8))(uVar17);
                  uVar3 = DAT_0022b934 + 1;
                  bVar6 = DAT_0022b934 < 0x20;
                  DAT_0022b934 = uVar3;
                  if (bVar6) {
                    snprintf((char *)&local_10e0,0x50,",\"detail\":%u",0);
                    pcVar18 = "donor_handoff_refused";
                    goto LAB_00176978;
                  }
                  goto LAB_00176a60;
                }
                (*(code *)(DAT_001e0978 + 0x5d48e8))(uVar15);
                (*(code *)(DAT_001e0978 + 0x594138))(uVar8,uVar17,0);
                uVar13 = FUN_00164418(uVar17,uVar8);
                if ((int)uVar13 != 0) {
                  puVar32[7] = uVar14;
                  *(undefined4 *)(puVar32 + 9) = 0;
                  *puVar32 = uVar28;
                  puVar32[1] = uVar20;
                  puVar32[4] = uVar17;
                  puVar32[2] = uVar8;
                  puVar32[3] = uVar16;
                  puVar32[8] = 0xffffffffffffffff;
                  puVar32[5] = uVar24;
                  *(int *)(puVar32 + 6) = iVar10;
                  DAT_0022cf58 = DAT_0022cf58 + 1;
                  uVar3 = DAT_0022b934 + 1;
                  bVar1 = DAT_0022b934 < 0x20;
                  DAT_0022b934 = uVar3;
                  local_1190 = puVar32;
                  if (bVar1) {
                    snprintf((char *)&local_10e0,0x50,",\"detail\":%u");
                    uVar13 = FUN_001417c8("trophies_hud","native_field_icon_created",&local_10e0);
                  }
                  goto LAB_00176490;
                }
                uVar13 = (*(code *)(DAT_001e0978 + 0x59567c))(uVar16);
                local_e0 = (char *)((ulong)local_e0 & 0xffffffff00000000);
                local_10e0 = 1;
                uVar13 = FUN_001428fc(uVar13,uVar16 + 0x38,&local_10e0,8);
                if ((((int)uVar13 != 0) && (local_10e0 == 0)) &&
                   ((uVar13 = FUN_001428fc(uVar13,uVar16 + 0x40,&local_e0,4), (int)uVar13 != 0 &&
                    ((int)local_e0 == -1)))) {
                  uVar13 = (*(code *)(DAT_001e0978 + 0x58d1c4))(uVar16);
                }
                local_e0 = (char *)((ulong)local_e0 & 0xffffffff00000000);
                local_10e0 = 1;
                uVar13 = FUN_001428fc(uVar13,uVar17 + 0x38,&local_10e0,8);
                if (((((int)uVar13 == 0) || (local_10e0 != 0)) ||
                    (iVar9 = FUN_001428fc(uVar13,uVar17 + 0x40,&local_e0,4), iVar9 == 0)) ||
                   ((int)local_e0 != -1)) goto LAB_00176a60;
                pcVar21 = (code *)(DAT_001e0978 + 0x5d48e8);
                uVar15 = uVar17;
              }
              else {
LAB_00176898:
                (*(code *)(DAT_001e0978 + 0x5d48e8))(uVar17);
                pcVar21 = (code *)(DAT_001e0978 + 0x5d48e8);
              }
            }
            (*pcVar21)(uVar15);
          }
        }
LAB_00176a60:
        lVar19 = 0x17;
      }
      lVar22 = 0;
      do {
        if (*(long *)((long)&DAT_0022cd38 + lVar22) == 0) {
          uVar20 = 0;
          *(ulong *)((long)&DAT_0022cd38 + lVar22) = uVar28;
          goto LAB_00175804;
        }
        lVar22 = lVar22 + 8;
      } while (lVar22 != 0x200);
      uVar20 = 0;
      goto LAB_00175804;
    }
LAB_00176490:
    if ((int)local_1190[6] != local_1178) {
      uVar20 = 0;
      lVar19 = 0x18;
      goto LAB_00175804;
    }
    local_e0 = (char *)((ulong)local_e0 & 0xffffffff00000000);
    uVar20 = local_1190[3];
    local_1190[7] = uVar14;
    local_1190[5] = uVar24;
    local_10e0 = 1;
    uVar13 = FUN_001428fc(uVar13,uVar20 + 0x38,&local_10e0,8);
    if ((((int)uVar13 != 0) && (local_10e0 == 0)) &&
       ((uVar13 = FUN_001428fc(uVar13,uVar20 + 0x40,&local_e0,4), (int)uVar13 != 0 &&
        ((int)local_e0 == -1)))) {
      uVar13 = (*(code *)(DAT_001e0978 + 0x594138))(local_1118,local_1190[3],0);
    }
    local_e0 = (char *)((ulong)local_e0 & 0xffffffff00000000);
    uVar20 = local_1190[4];
    local_10e0 = 1;
    uVar13 = FUN_001428fc(uVar13,uVar20 + 0x38,&local_10e0,8);
    if ((((int)uVar13 != 0) && (local_10e0 == 0)) &&
       ((iVar10 = FUN_001428fc(uVar13,uVar20 + 0x40,&local_e0,4), iVar10 != 0 &&
        ((int)local_e0 == -1)))) {
      (*(code *)(DAT_001e0978 + 0x594138))(local_1118,local_1190[4],0);
    }
    iVar10 = FUN_00164418(local_1190[3],local_1118);
    if ((iVar10 == 0) || (iVar10 = FUN_00164418(local_1190[4],local_1118), iVar10 == 0)) {
      uVar20 = 0;
      lVar19 = 0x19;
      goto LAB_00175804;
    }
    if (((int)local_1190[8] != iVar9) || (*(int *)((long)local_1190 + 0x44) != -1)) {
      iVar10 = FUN_0014b1d0(0x5921a0,0xb0,
                            "6b5853b7737f55c2722ea9a461730d5a0274ec3f756ad12c93cede1764df931a");
      if (iVar10 == 0) {
        if (DAT_0021c9f8 == (code *)0x0) {
          local_10e0 = 0;
          pcStack_10d8 = (code *)0x0;
          dl_iterate_phdr(FUN_00163228,&local_10e0);
          if (((int)local_10e0 == 1) && (pcStack_10d8 != (code *)0x0)) {
            DAT_0021c9f8 = pcStack_10d8;
            goto LAB_00176640;
          }
        }
        else {
LAB_00176640:
          iVar10 = (*DAT_0021c9f8)(DAT_001e0978);
          if (iVar10 == 1) goto LAB_00176650;
        }
        lVar19 = 0x1a;
        uVar20 = 0x5921a0;
        goto LAB_00175804;
      }
LAB_00176650:
      snprintf((char *)&local_10e0,0x20,"%d",uVar27 & 0xffffffff);
      (*(code *)(DAT_001e0978 + 0x66ae58))(&local_e0,&local_10e0);
      (*(code *)(DAT_001e0978 + 0x5921a0))(local_1190[3],&local_e0);
      (*(code *)(DAT_001e0978 + 0x66ad48))(&local_e0);
      *(int *)(local_1190 + 8) = iVar9;
      *(undefined4 *)((long)local_1190 + 0x44) = 0xffffffff;
    }
    fVar33 = (float)local_90;
    if ((((DAT_0021c828 != 0) && (DAT_0021c7a8 == uVar28)) && (DAT_0021c7b8 == lVar22)) &&
       (ABS((float)local_90 - DAT_0021c820) < 0.5)) {
      fVar33 = DAT_0021c81c;
    }
    fVar36 = -20.0;
    if (bVar6) {
      fVar36 = -45.0;
    }
    fVar36 = fVar36 + local_90._4_4_;
    (*(code *)(DAT_001e0978 + 0x595314))(fVar33 + -6.0,fVar36,local_1190[3]);
    (*(code *)(DAT_001e0978 + 0x595314))(fVar33 + -6.0 + -15.0,fVar36 + 7.0,local_1190[4]);
    local_10e8 = 0;
    uVar28 = FUN_00139fcc();
    if (uVar28 != uVar14) {
LAB_00176ab4:
      uVar20 = 0;
      lVar19 = 0x1b;
      goto LAB_00175804;
    }
    local_10f0 = CONCAT44(local_10f0._4_4_,0xffffffff);
    local_e0 = "trophiesAboveHead";
    if (((((int)DAT_00214938 != 1) || (DAT_00214930 == (code *)0x0)) ||
        ((iVar9 = (*DAT_00214930)(&local_e0,&local_10e0,1,&local_10e8,&local_10f0), iVar9 != 1 ||
         ((local_10e8 == 0 || ((int)local_10f0 != 0)))))) || ((int)pcStack_10d8 != 2))
    goto LAB_00176ab4;
    uVar20 = 0;
    lVar19 = 0x1b;
    if ((local_10e0._4_4_ != 1) || (local_10e8 != local_1148)) goto LAB_00175804;
    local_10e0 = CONCAT71(local_10e0._1_7_,1);
    lVar19 = FUN_0014bef8(DAT_001cfb4c,&local_10e0,1,local_1190[3] + 8,0);
    if (lVar19 != 1) goto LAB_00176ab4;
    local_10e0 = CONCAT71(local_10e0._1_7_,1);
    lVar19 = FUN_0014bef8(DAT_001cfb4c,&local_10e0,1,local_1190[4] + 8);
    if (lVar19 != 1) goto LAB_00176ab4;
    DAT_0022cf50 = DAT_0022cf50 + 1;
    *(undefined4 *)(local_1190 + 9) = 1;
  }
  else {
    uVar20 = 0;
    lVar19 = 1;
LAB_00175804:
    plVar25 = (long *)(lVar19 * 8 + 0x22cf60);
    do {
      cVar5 = '\x01';
      bVar6 = (bool)ExclusiveMonitorPass(plVar25,0x10);
      if (bVar6) {
        *plVar25 = *plVar25 + 1;
        cVar5 = ExclusiveMonitorsStatus();
      }
    } while (cVar5 != '\0');
    do {
      uVar3 = DAT_0022d060;
      cVar5 = '\x01';
      bVar6 = (bool)ExclusiveMonitorPass(0x22d060,0x10);
      if (bVar6) {
        cVar5 = ExclusiveMonitorsStatus();
        DAT_0022d060 = DAT_0022d060 | 1 << lVar19;
      }
    } while (cVar5 != '\0');
    if ((uVar3 & 1 << lVar19) == 0) {
      snprintf((char *)&local_10e0,0x50,",\"gate\":%u,\"detail\":%u",lVar19,uVar20);
      FUN_001417c8("trophies_hud","update_gate_refused",&local_10e0);
    }
  }
  *puVar12 = uVar4;
  if ((((uint)_DAT_0022cf40 >> 2 & 1) != 0) && ((int)DAT_00214938 == 1)) {
    lVar19 = 0;
    ppuVar30 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar10 = strcmp("ShowEnemyAmmoStatus",*ppuVar30);
      iVar9 = DAT_00209cd8;
      if (iVar10 == 0) {
        if ((((-1 < (int)lVar19) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar19 * 4) != 0))
            && (DAT_00209cd8 != 0)) &&
           (((uVar13 = gettid(), iVar9 == (int)uVar13 &&
             (iVar9 = FUN_001428fc(uVar13,*(long *)(param_1 + 0x98) + 0xb48,&local_10e0,8),
             iVar9 != 0)) && ((0x11fff < local_10e0 + 0x2000 && ((local_10e0 & 7) == 0)))))) {
          local_e0 = (char *)CONCAT71(local_e0._1_7_,1);
          FUN_0014bef8(DAT_001cfb4c,&local_e0,1,local_10e0 + 8);
        }
        break;
      }
      lVar19 = lVar19 + 1;
      ppuVar30 = ppuVar30 + 3;
    } while (lVar19 != 0x37);
  }
LAB_00175954:
  *puVar12 = uVar4;
  if (*(long *)(lVar7 + 0x28) == local_88) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0019cb5c @ 0019cb5c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_0019cb5c(long *param_1)

{
  byte *pbVar1;
  char cVar2;
  byte bVar3;
  long lVar4;
  undefined8 uVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  char *__s;
  char *pcVar9;
  ssize_t sVar10;
  char *pcVar11;
  char *pcVar12;
  int *piVar13;
  char *pcVar14;
  uint uVar15;
  long lVar16;
  ulong uVar17;
  byte *pbVar18;
  ulong uVar19;
  uint *puVar20;
  int *piVar21;
  byte bVar22;
  byte bVar23;
  bool bVar24;
  long lVar25;
  long *plVar26;
  long *plVar27;
  long *plVar28;
  size_t sVar29;
  long lVar30;
  ulong uVar31;
  byte *pbVar32;
  undefined4 uStack_10084;
  sockaddr sStack_10080;
  byte abStack_10070 [8];
  undefined8 uStack_10068;
  
  lVar4 = tpidr_el0;
  lVar16 = *(long *)(lVar4 + 0x28);
  abStack_10070 = (byte  [8])s___data____0011cc08._0_8_;
  uStack_10068 = CONCAT62(uStack_10068._2_6_,0x7b);
  if ((int)param_1[1] == 0) {
    lVar30 = 9;
    plVar27 = (long *)&DAT_00117c20;
  }
  else {
    uVar31 = 0;
    plVar28 = param_1 + 3;
    lVar30 = 9;
    plVar26 = (long *)&DAT_00117c20;
    do {
      plVar27 = (long *)&DAT_00117c20;
      if (uVar31 != 0) {
        plVar27 = (long *)&DAT_00120242;
      }
      uVar6 = snprintf((char *)(abStack_10070 + lVar30),0x1000U - lVar30,"%s\"#%s\":%u",plVar27,
                       plVar28,(ulong)*(uint *)((long)plVar28 + -4));
      if (((int)uVar6 < 0) || (0x1000U - lVar30 <= (ulong)uVar6)) {
        __s = (char *)0x0;
        goto LAB_0019cd28;
      }
      lVar30 = lVar30 + (ulong)uVar6;
      uVar31 = uVar31 + 1;
      plVar27 = plVar28;
      if ((int)plVar28[-1] != *(int *)((long)param_1 + 0xc)) {
        plVar27 = plVar26;
      }
      plVar28 = plVar28 + 5;
      plVar26 = plVar27;
    } while (uVar31 < *(uint *)(param_1 + 1));
  }
  uVar6 = snprintf((char *)(abStack_10070 + lVar30),0x1000U - lVar30,
                   "},\"magic_number\":0,\"signature\":\"0\",\"tag\":\"#%s\",\"gameMode\":\"NULL\"}"
                   ,plVar27);
  if (((int)uVar6 < 0) || (0x1000U - lVar30 <= (ulong)uVar6)) {
    __s = (char *)0x0;
  }
  else {
    lVar30 = lVar30 + (ulong)uVar6;
    FUN_0019d76c(abStack_10070,lVar30);
    __s = (char *)malloc(lVar30 * 5 + 0xc);
    uVar5 = s___data____0011b68b._0_8_;
    if (__s != (char *)0x0) {
      __s[8] = '\"';
      *(undefined8 *)__s = uVar5;
      if (lVar30 == 0) {
        lVar25 = 9;
      }
      else {
        sVar29 = lVar30 * 5 + 3;
        lVar25 = 9;
        pbVar18 = abStack_10070;
        do {
          snprintf(__s + lVar25,sVar29,"\\\\x%02x",(ulong)*pbVar18);
          lVar30 = lVar30 + -1;
          lVar25 = lVar25 + 5;
          sVar29 = sVar29 - 5;
          pbVar18 = pbVar18 + 1;
        } while (lVar30 != 0);
      }
      pcVar9 = __s + lVar25;
      pcVar9[2] = '\0';
      pcVar9[0] = '\"';
      pcVar9[1] = '}';
    }
  }
LAB_0019cd28:
  pcVar9 = (char *)malloc(0x20000);
  uVar6 = 0;
  if ((__s != (char *)0x0) && (pcVar9 != (char *)0x0)) {
    sStack_10080.sa_data[10] = '\0';
    sStack_10080.sa_data[0xb] = '\0';
    sStack_10080.sa_data[0xc] = '\0';
    sStack_10080.sa_data[0xd] = '\0';
    sStack_10080.sa_data[2] = '\0';
    sStack_10080.sa_data[3] = '\0';
    sStack_10080.sa_data[4] = '\0';
    sStack_10080.sa_data[5] = '\0';
    sStack_10080.sa_data[6] = '\0';
    sStack_10080.sa_data[7] = '\0';
    sStack_10080.sa_data[8] = '\0';
    sStack_10080.sa_data[9] = '\0';
    sStack_10080.sa_family = 2;
    sStack_10080.sa_data[0] = '\0';
    sStack_10080.sa_data[1] = 'P';
    iVar7 = inet_pton(2,"87.228.126.116",(void *)((ulong)&sStack_10080 | 4));
    if ((iVar7 == 1) && (iVar7 = socket(2,1,0), -1 < iVar7)) {
      uStack_10068 = _UNK_00112908;
      abStack_10070[0] = DAT_00112900;
      abStack_10070[1] = UNK_00112901;
      abStack_10070[2] = UNK_00112902;
      abStack_10070[3] = UNK_00112903;
      abStack_10070[4] = UNK_00112904;
      abStack_10070[5] = UNK_00112905;
      abStack_10070[6] = UNK_00112906;
      abStack_10070[7] = UNK_00112907;
      setsockopt(iVar7,1,0x14,abStack_10070,0x10);
      setsockopt(iVar7,1,0x15,abStack_10070,0x10);
      iVar8 = connect(iVar7,&sStack_10080,0x10);
      if (iVar8 == 0) {
        sVar29 = strlen(__s);
        uVar6 = snprintf((char *)abStack_10070,0x300,
                         "POST /bsd/api/v1/get_bsd_users_by_mask HTTP/1.1\r\nHost: plusapi.bsd.meowfox.net\r\nContent-Type: application/json\r\nContent-Length: %zu\r\nConnection: close\r\n\r\n"
                         ,sVar29);
        if (uVar6 - 1 < 0x2ff) {
          uVar31 = 0;
          do {
            if (uVar6 <= uVar31) {
              uVar31 = 0;
              goto LAB_0019ce54;
            }
            sVar10 = send(iVar7,abStack_10070 + uVar31,uVar6 - uVar31,0x4000);
            uVar31 = sVar10 + uVar31;
          } while (0 < sVar10);
        }
        goto LAB_0019ce78;
      }
      close(iVar7);
    }
    goto LAB_0019cf64;
  }
  goto LAB_0019cf68;
  while( true ) {
    sVar10 = send(iVar7,__s + uVar31,sVar29 - uVar31,0x4000);
    uVar31 = sVar10 + uVar31;
    if (sVar10 < 1) break;
LAB_0019ce54:
    if (sVar29 < uVar31 || sVar29 - uVar31 == 0) {
      lVar30 = 0;
      goto LAB_0019d13c;
    }
  }
LAB_0019ce78:
  lVar30 = 0;
LAB_0019ce7c:
  bVar24 = true;
  goto LAB_0019ce80;
LAB_0019d188:
  bVar23 = *pbVar18;
  if (0x22 < bVar23) goto LAB_0019d640;
  if ((1L << ((ulong)bVar23 & 0x3f) & 0x100002600U) == 0) {
    if ((((ulong)bVar23 == 0x22) && (sVar29 = strlen((char *)pbVar18), sVar29 < 0x1fff9)) &&
       (pcVar12 = (char *)malloc(sVar29 + 9), pcVar12 != (char *)0x0)) {
      builtin_strncpy(pcVar12,"{\"data\":",8);
      memcpy(pcVar12 + 8,pbVar18,sVar29 + 1);
      pcVar14 = strstr(pcVar12,"\"data\":\"");
      if (pcVar14 == (char *)0x0) goto LAB_0019d310;
      uVar31 = 0;
      pbVar18 = (byte *)(pcVar14 + 8);
      goto LAB_0019d244;
    }
    goto LAB_0019d640;
  }
  pbVar18 = pbVar18 + 1;
  goto LAB_0019d188;
LAB_0019d244:
  do {
    bVar23 = *pbVar18;
    uVar19 = uVar31;
    if (bVar23 == 0x5c) {
      bVar22 = pbVar18[1];
      if (bVar22 == 0) break;
      pbVar18 = pbVar18 + 2;
      bVar23 = 10;
      switch((uint)(CONCAT14(bVar22,(uint)bVar22 * 0x1000000 + -0x62000000) >> 0x19) & 0xff) {
      case 0:
        bVar23 = 8;
        break;
      default:
        bVar23 = 0xc;
        if (bVar22 != 0x66) {
          bVar23 = bVar22;
        }
        break;
      case 6:
        break;
      case 8:
        bVar23 = 0xd;
        break;
      case 9:
        bVar23 = 9;
      }
    }
    else {
      if ((bVar23 == 0) || (bVar23 == 0x22)) break;
      pbVar18 = pbVar18 + 1;
    }
    abStack_10070[uVar31] = bVar23;
    uVar31 = uVar31 + 1;
    uVar19 = 0xffff;
  } while (uVar31 != 0xffff);
  pbVar18 = abStack_10070;
  pbVar18[uVar19] = 0;
  bVar23 = abStack_10070[0];
  if (abStack_10070[0] != 0x62) {
LAB_0019d30c:
    if (bVar23 != 0) goto LAB_0019d324;
LAB_0019d310:
    free(pcVar12);
    goto LAB_0019d640;
  }
  if ((abStack_10070[1] == '\"') || (abStack_10070[1] == '\'')) {
    pbVar18 = (byte *)((ulong)abStack_10070 | 2);
    if (2 < uVar19) {
      cVar2 = '\0';
      if (sStack_10080.sa_data[uVar19 + 0xd] != abStack_10070[1]) {
        cVar2 = sStack_10080.sa_data[uVar19 + 0xd];
      }
      sStack_10080.sa_data[uVar19 + 0xd] = cVar2;
    }
    bVar23 = abStack_10070[2];
    goto LAB_0019d30c;
  }
  pbVar18 = abStack_10070;
  bVar23 = 0x62;
LAB_0019d324:
  uVar31 = 0;
  do {
    uVar19 = uVar31;
    pbVar32 = pbVar18 + 1;
    if (bVar23 != 0x5c) {
      pcVar11[uVar19] = bVar23;
      pbVar18 = pbVar32;
      goto LAB_0019d3d0;
    }
    bVar23 = *pbVar32;
    bVar22 = 10;
    switch((uint)(CONCAT14(bVar23,(uint)bVar23 * 0x1000000 + -0x6e000000) >> 0x19) & 0xff) {
    case 0:
      break;
    default:
      goto switchD_0019d374_caseD_1;
    case 2:
      bVar22 = 0xd;
      break;
    case 3:
      bVar22 = 9;
      break;
    case 5:
      bVar22 = pbVar18[2];
      if (bVar22 != 0) {
        bVar3 = pbVar18[3];
        if (bVar3 != 0) {
          uVar6 = (uint)bVar22;
          if (bVar22 - 0x30 < 10) {
            iVar7 = uVar6 - 0x30;
          }
          else if (uVar6 - 0x61 < 6) {
            iVar7 = uVar6 - 0x57;
          }
          else {
            iVar7 = uVar6 - 0x37;
            if (5 < uVar6 - 0x41) {
              iVar7 = -1;
            }
          }
          uVar6 = (uint)bVar3;
          if (bVar3 - 0x30 < 10) {
            iVar8 = uVar6 - 0x30;
          }
          else if (uVar6 - 0x61 < 6) {
            iVar8 = uVar6 - 0x57;
          }
          else {
            iVar8 = uVar6 - 0x37;
            if (5 < uVar6 - 0x41) {
              iVar8 = -1;
            }
          }
          if ((-1 < iVar7) && (-1 < iVar8)) {
            pcVar11[uVar19] = (byte)iVar8 | (byte)(iVar7 << 4);
            pbVar18 = pbVar18 + 4;
            goto LAB_0019d3d0;
          }
          goto LAB_0019d310;
        }
      }
switchD_0019d374_caseD_1:
      bVar22 = bVar23;
    }
    bVar23 = pbVar18[1];
    pbVar1 = pbVar18 + 2;
    pcVar11[uVar19] = bVar22;
    pbVar18 = pbVar32;
    if (bVar23 != 0) {
      pbVar18 = pbVar1;
    }
LAB_0019d3d0:
    bVar23 = *pbVar18;
    uVar31 = uVar19 + 1;
  } while ((bVar23 != 0) && (uVar19 < 0xfffe));
  free(pcVar12);
  if (0xfffd < uVar19) goto LAB_0019d640;
  FUN_0019d76c(pcVar11,uVar31);
  pcVar11[uVar31] = '\0';
  pcVar12 = strstr(pcVar11,"\"bsd_users\"");
  uVar15 = *(uint *)(param_1 + 1);
  if (pcVar12 != (char *)0x0) {
    if (uVar15 == 0) goto LAB_0019d640;
    uVar31 = 0;
    do {
      snprintf((char *)abStack_10070,0x20,"\"%s\"",param_1 + uVar31 * 5 + 3);
      pcVar14 = strstr(pcVar12,(char *)abStack_10070);
      if (pcVar14 == (char *)0x0) {
        snprintf((char *)abStack_10070,0x20,"\"#%s\"",param_1 + uVar31 * 5 + 3);
        pcVar14 = strstr(pcVar12,(char *)abStack_10070);
        if (pcVar14 != (char *)0x0) goto LAB_0019d538;
      }
      else {
LAB_0019d538:
        sVar29 = strlen((char *)abStack_10070);
        pbVar18 = (byte *)(pcVar14 + sVar29);
        pbVar32 = pbVar18 + 2;
        for (; bVar23 = *pbVar18, bVar23 < 0x3b; pbVar18 = pbVar18 + 1) {
          if ((1L << ((ulong)bVar23 & 0x3f) & 0x100002600U) == 0) {
            if ((ulong)bVar23 == 0x3a) {
              for (; bVar23 = pbVar32[-1],
                  bVar23 < 0x21 && (1L << ((ulong)bVar23 & 0x3f) & 0x100002600U) != 0;
                  pbVar32 = pbVar32 + 1) {
              }
              if ((bVar23 == 0x7b) &&
                 (pcVar14 = strchr((char *)pbVar32,0x7d), pcVar14 != (char *)0x0)) {
                uStack_10084 = 0xffffffff;
                sStack_10080.sa_family = 0xffff;
                sStack_10080.sa_data[0] = -1;
                sStack_10080.sa_data[1] = -1;
                iVar7 = FUN_0019da24(pbVar32,pcVar14,"trophies",&sStack_10080);
                iVar8 = FUN_0019da24(pbVar32,pcVar14,"character",&uStack_10084);
                if (iVar7 != 0) {
                  *(undefined4 *)(param_1 + uVar31 * 5 + 6) = sStack_10080._0_4_;
                }
                if (iVar8 != 0) {
                  *(undefined4 *)((long)param_1 + uVar31 * 0x28 + 0x34) = uStack_10084;
                }
              }
            }
            break;
          }
          pbVar32 = pbVar32 + 1;
        }
      }
      uVar31 = uVar31 + 1;
      uVar15 = *(uint *)(param_1 + 1);
    } while (uVar31 < uVar15);
  }
  if (uVar15 == 0) goto LAB_0019d640;
  uVar6 = 0;
  uVar31 = (ulong)uVar15;
  puVar20 = (uint *)((long)param_1 + 0x34);
  do {
    if ((int)puVar20[-1] < 0) {
      uVar15 = 0;
    }
    else {
      uVar15 = ~*puVar20 >> 0x1f;
    }
    uVar6 = uVar6 + uVar15;
    puVar20 = puVar20 + 10;
    uVar31 = uVar31 - 1;
  } while (uVar31 != 0);
  goto LAB_0019d644;
  while( true ) {
    if (sVar10 < 0) {
      piVar13 = (int *)__errno();
      if (*piVar13 != 4) goto LAB_0019ce7c;
    }
    else {
      lVar30 = sVar10 + lVar30;
    }
    if (0x1ffff < lVar30 + 1U) break;
LAB_0019d13c:
    sVar10 = recv(iVar7,pcVar9 + lVar30,0x1ffff - lVar30,0);
    if (sVar10 == 0) break;
  }
  bVar24 = false;
LAB_0019ce80:
  close(iVar7);
  pcVar9[lVar30] = '\0';
  if ((((bVar24) || (lVar30 - 0x1ffffU < 0xfffffffffffe0000)) ||
      ((iVar7 = strncmp(pcVar9,"HTTP/1.1 200 ",0xd), iVar7 != 0 &&
       (iVar7 = strncmp(pcVar9,"HTTP/1.0 200 ",0xd), iVar7 != 0)))) ||
     (pcVar11 = strstr(pcVar9,"\r\n\r\n"), pcVar11 == (char *)0x0)) {
LAB_0019cf64:
    uVar6 = 0;
  }
  else {
    pcVar11 = pcVar11 + 4;
    if (pcVar11 != pcVar9) {
      sVar29 = strlen(pcVar11);
      memmove(pcVar9,pcVar11,sVar29 + 1);
    }
    pcVar11 = (char *)malloc(0x10000);
    if (pcVar11 == (char *)0x0) goto LAB_0019cf64;
    pcVar12 = strstr(pcVar9,"\"data\"");
    if (pcVar12 != (char *)0x0) {
      for (pbVar18 = (byte *)(pcVar12 + 7); bVar23 = pbVar18[-1], bVar23 < 0x3b;
          pbVar18 = pbVar18 + 1) {
        if ((1L << ((ulong)bVar23 & 0x3f) & 0x100002600U) == 0) {
          if ((ulong)bVar23 == 0x3a) goto LAB_0019d188;
          break;
        }
      }
    }
LAB_0019d640:
    uVar6 = 0;
LAB_0019d644:
    free(pcVar11);
  }
LAB_0019cf68:
  pthread_mutex_lock((pthread_mutex_t *)&DAT_0031ae50);
  uVar15 = DAT_0031ae80;
  if ((((DAT_0031b018 != param_1[0x34]) || (DAT_0031ae78 != *param_1)) ||
      (uVar31 = (ulong)DAT_0031ae80, DAT_0031ae80 != *(uint *)(param_1 + 1))) ||
     (DAT_0031ae84 != *(int *)((long)param_1 + 0xc))) goto LAB_0019d0b8;
  if (DAT_0031ae80 != 0) {
    if (((DAT_0031ae88 != (int)param_1[2]) || (DAT_0031ae8c != *(int *)((long)param_1 + 0x14))) ||
       (iVar7 = strcmp(&DAT_0031ae90,(char *)(param_1 + 3)), iVar7 != 0)) goto LAB_0019d0b8;
    plVar27 = param_1 + 8;
    pcVar11 = &DAT_0031aeb8;
    uVar19 = 1;
    do {
      uVar17 = uVar19;
      if (((uVar31 == uVar17) || (*(int *)(pcVar11 + -8) != (int)plVar27[-1])) ||
         (*(int *)(pcVar11 + -4) != *(int *)((long)plVar27 + -4))) break;
      iVar7 = strcmp(pcVar11,(char *)plVar27);
      plVar27 = plVar27 + 5;
      pcVar11 = pcVar11 + 0x28;
      uVar19 = uVar17 + 1;
    } while (iVar7 == 0);
    if (uVar17 < uVar31) goto LAB_0019d0b8;
    piVar13 = (int *)((long)param_1 + 0x34);
    piVar21 = (int *)((long)&DAT_0031aea8 + 4);
    do {
      if (-1 < piVar13[-1]) {
        piVar21[-1] = piVar13[-1];
      }
      if (-1 < *piVar13) {
        *piVar21 = *piVar13;
      }
      piVar21 = piVar21 + 10;
      uVar31 = uVar31 - 1;
      piVar13 = piVar13 + 10;
    } while (uVar31 != 0);
  }
  DAT_0031b020 = (uint)(uVar15 == uVar6);
LAB_0019d0b8:
  DAT_0031b030 = 0;
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_0031ae50);
  free(__s);
  free(pcVar9);
  free(param_1);
  if (*(long *)(lVar4 + 0x28) == lVar16) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

