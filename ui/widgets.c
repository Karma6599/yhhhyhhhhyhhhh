/*
 * widgets — UI subsystem
 * Decompiled with Ghidra 11.3.2 (arm64 pseudocode) from: libNexusUI69252.so
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

/* ===== FUN_0014ec44 @ 0014ec44 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0014ec44(char *param_1,undefined8 *param_2)

{
  long lVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  bool bVar4;
  bool bVar5;
  int iVar6;
  int iVar7;
  undefined8 uVar8;
  long lVar9;
  pthread_t __target_thread;
  uint uVar10;
  ulong uVar11;
  undefined8 local_b0;
  undefined8 local_a8;
  long local_a0;
  long local_98;
  timespec local_90;
  long local_80;
  long local_78;
  int local_70;
  int local_64;
  ulong local_58;
  undefined8 local_50;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  iVar6 = strcmp(param_1,"ShowFastPlayAgainButton");
  if ((iVar6 != 0) && (iVar7 = strcmp(param_1,"BattleEndInstantExit"), iVar7 != 0)) {
    uVar8 = 0;
    goto LAB_0014ee3c;
  }
  DAT_0022d8e8 = 1;
  iVar7 = clock_gettime(1,&local_90);
  if (iVar7 == 0) {
    lVar9 = local_90.tv_sec * 1000 + (ulong)local_90.tv_nsec / 1000000;
  }
  else {
    lVar9 = 0;
  }
  FUN_0014ef90(&local_90,lVar9);
  if (iVar6 == 0) {
    local_a0 = 0;
    local_98 = 0;
    local_a8 = 0;
    local_b0 = DAT_0010f720;
    bVar5 = false;
    if ((local_70 != 0) && (local_78 != 0)) {
      if ((DAT_0022f1c0 != 0) && ((DAT_0022f1b0 == local_80 && (DAT_0022f1b8 == local_90.tv_nsec))))
      goto LAB_0014ecf4;
      iVar6 = FUN_00192138(&local_b0);
      bVar5 = false;
      if ((iVar6 != 0) && ((local_a0 != 0 && (local_98 != 0)))) {
        bVar5 = (int)local_a8 != 0;
      }
    }
  }
  else {
LAB_0014ecf4:
    bVar5 = false;
  }
  iVar6 = DAT_0022f038;
  bVar4 = false;
  if ((DAT_0022da74._4_4_ != 0) && ((int)DAT_0022da7c == 1)) {
    if (DAT_0022d900 == local_80) {
      bVar4 = DAT_0022d8f8 == local_90.tv_nsec;
    }
    else {
      bVar4 = false;
    }
  }
  if (DAT_0022d8d8 == '\x01') {
    local_58 = 0;
    local_50 = 0;
    if ((DAT_0022f038 < 1) || (iVar7 = gettid(), iVar7 != iVar6)) goto LAB_0014ee14;
    __target_thread = pthread_self();
    iVar6 = pthread_getname_np(__target_thread,(char *)&local_58,0x10);
    if (iVar6 != 0) goto LAB_0014ee14;
    if (local_58 != 0x706f6f6c6e69614d || (char)local_50 != '\0') {
      bVar4 = true;
    }
    uVar10 = 0;
    if (((!bVar4) && (uVar10 = 0, local_64 == 0)) && (DAT_0022dad8 == 0)) {
      uVar11 = DAT_0022f1c0;
      if (DAT_0022f1c0 != 0) {
        if (DAT_0022f1b0 == local_80) {
          uVar11 = (ulong)(DAT_0022f1b8 == local_90.tv_nsec);
        }
        else {
          uVar11 = 0;
        }
      }
      if ((uVar11 & 1) == 0 && !bVar5) {
        if ((int)DAT_0022db38 != 0) goto LAB_0014ee14;
        uVar10 = 0;
        if ((local_70 != 0) && (DAT_0022db18 != 0)) {
          if ((local_80 != DAT_0022db20) ||
             ((local_90.tv_nsec != DAT_0022db30 || (uVar8 = FUN_00154eb4(), (int)uVar8 == 0))))
          goto LAB_0014ee14;
          local_58 = 0;
          iVar6 = FUN_0014e7c4(uVar8,DAT_0022db18 + 0x248,&local_58,8);
          uVar11 = local_58;
          if (((local_58 & 7) != 0 || local_58 < 0x1000) || iVar6 == 0) {
            uVar11 = 0;
          }
          uVar10 = (uint)(uVar11 == DAT_0022db28);
        }
      }
      else {
        uVar10 = (uint)uVar11 ^ 1;
      }
    }
  }
  else {
LAB_0014ee14:
    uVar10 = 0;
  }
  uVar8 = 1;
  *(char *)((long)param_2 + 0x14) = (char)uVar10;
  *(undefined1 *)((long)param_2 + 0x15) = 0;
  uVar3 = _UNK_0010f968;
  uVar2 = _DAT_0010f960;
  *(uint *)(param_2 + 2) = uVar10 ^ 1;
  *(undefined2 *)((long)param_2 + 0x16) = 0;
  param_2[1] = uVar3;
  *param_2 = uVar2;
LAB_0014ee3c:
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar8);
}

/* ===== FUN_0014f5b8 @ 0014f5b8 ===== */

void FUN_0014f5b8(char *param_1)

{
  long lVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  ulong uVar6;
  long lVar7;
  undefined8 local_d0;
  undefined8 local_c8;
  long local_c0;
  long local_b8;
  timespec local_b0;
  long local_a0;
  long local_98;
  int local_90;
  int local_84;
  ulong local_78;
  undefined1 auStack_70 [20];
  char local_5c;
  long local_58;
  
  lVar1 = tpidr_el0;
  local_58 = *(long *)(lVar1 + 0x28);
  iVar2 = strcmp(param_1,"ShowFastPlayAgainButton");
  if (iVar2 != 0) {
    iVar3 = strcmp(param_1,"BattleEndInstantExit");
    if (iVar3 == 0) {
      iVar3 = 2;
      goto LAB_0014f620;
    }
    goto LAB_0014f60c;
  }
  iVar3 = 1;
LAB_0014f620:
  iVar4 = FUN_0014ec44(param_1,auStack_70);
  lVar5 = 0;
  if ((iVar4 == 0) || (local_5c == '\0')) goto LAB_0014f800;
  iVar4 = clock_gettime(1,&local_b0);
  if (iVar4 == 0) {
    lVar7 = local_b0.tv_sec * 1000 + (ulong)local_b0.tv_nsec / 1000000;
  }
  else {
    lVar7 = 0;
  }
  FUN_0014ef90(&local_b0,lVar7);
  lVar5 = 0;
  if (((local_90 == 0) || (local_84 != 0)) || (DAT_0022dad8 != 0)) goto LAB_0014f800;
  if (iVar2 == 0) {
    local_c0 = 0;
    local_b8 = 0;
    local_c8 = 0;
    local_d0 = DAT_0010f720;
    if (((local_98 == 0) ||
        ((((DAT_0022f1c0 != 0 && (DAT_0022f1b0 == local_a0)) && (DAT_0022f1b8 == local_b0.tv_nsec))
         || ((iVar2 = FUN_00192138(&local_d0), iVar2 == 0 || (local_c0 == 0)))))) ||
       ((local_b8 == 0 || ((int)local_c8 == 0)))) goto LAB_0014f6ac;
    DAT_0022dae0 = 0;
    uVar6 = 0;
    DAT_0022db08 = local_c0;
    DAT_0022db10 = local_b8;
LAB_0014f7c0:
    lVar5 = 2;
    DAT_0022dad8 = iVar3;
    DAT_0022dae8 = local_a0;
    DAT_0022daf0 = uVar6;
    DAT_0022daf8 = local_b0.tv_nsec;
    DAT_0022db00 = lVar7;
  }
  else {
LAB_0014f6ac:
    lVar5 = DAT_0022db18;
    if (DAT_0022db18 == 0) goto LAB_0014f800;
    if ((local_a0 == DAT_0022db20) && (local_b0.tv_nsec == DAT_0022db30)) {
      lVar5 = FUN_00154eb4(DAT_0022db18,local_a0);
      if ((int)lVar5 == 0) goto LAB_0014f800;
      local_78 = 0;
      iVar2 = FUN_0014e7c4(lVar5,DAT_0022db18 + 0x248,&local_78,8);
      uVar6 = local_78;
      if (((local_78 & 7) != 0 || local_78 < 0x1000) || iVar2 == 0) {
        uVar6 = 0;
      }
      if (uVar6 == DAT_0022db28) {
        DAT_0022db08 = 0;
        DAT_0022db10 = 0;
        DAT_0022dae0 = DAT_0022db18;
        goto LAB_0014f7c0;
      }
    }
LAB_0014f60c:
    lVar5 = 0;
  }
LAB_0014f800:
  if (*(long *)(lVar1 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar5);
}

/* ===== nexus_native_trophy_text_guard @ 0014fcec ===== */

void nexus_native_trophy_text_guard(long param_1)

{
  undefined4 uVar1;
  long lVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined8 uVar5;
  int local_24c;
  undefined1 auStack_248 [512];
  long local_48;
  
  lVar2 = tpidr_el0;
  local_48 = *(long *)(lVar2 + 0x28);
  puVar4 = (undefined4 *)__errno();
  uVar5 = 0;
  uVar1 = *puVar4;
  local_24c = 0;
  if ((((DAT_0022d8e0 != param_1) || (DAT_0022d8d8 == '\0')) || (DAT_0022dd98 == 0)) ||
     (((DAT_0022db80 == 0 || (DAT_0022db90 == 0)) ||
      ((0x200 < DAT_0022db90 ||
       (uVar5 = FUN_0014e7c4(0,DAT_0022d8e0 + 0x5921a0,&local_24c,4), (int)uVar5 == 0))))))
  goto LAB_0014fdcc;
  if (local_24c == DAT_0022db88) {
    uVar5 = FUN_0014e7c4(uVar5,DAT_0022db80,auStack_248,DAT_0022db90);
    if ((int)uVar5 == 0) goto LAB_0014fdcc;
    iVar3 = memcmp(auStack_248,&DAT_0022db98,DAT_0022db90);
    if (iVar3 == 0) {
      uVar5 = FUN_0014fe14(0x5921a4,0xac,
                           "53397ef450e30da2b8d4a3670db34ea376776454fa87062980a88ff69c879891");
      goto LAB_0014fdcc;
    }
  }
  uVar5 = 0;
LAB_0014fdcc:
  *puVar4 = uVar1;
  if (*(long *)(lVar2 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== JNI_OnLoad @ 00150c18 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 JNI_OnLoad(long *param_1)

{
  long lVar1;
  __dev_t _Var2;
  __uid_t _Var3;
  bool bVar4;
  char cVar5;
  char cVar6;
  int iVar7;
  int iVar8;
  __uid_t _Var9;
  ssize_t sVar10;
  long lVar11;
  undefined8 uVar12;
  long lVar13;
  undefined8 uVar14;
  long lVar15;
  long lVar16;
  int *piVar17;
  long lVar18;
  char *pcVar19;
  char *pcVar20;
  undefined4 uVar21;
  long lVar22;
  undefined8 local_4230;
  long local_4220;
  long local_4218;
  stat local_4210;
  long local_4120;
  long lStack_4118;
  long local_4110;
  long lStack_4108;
  undefined1 local_4100 [24];
  undefined *local_40e8;
  undefined *puStack_40e0;
  undefined *puStack_40d8;
  undefined *local_40d0;
  undefined *puStack_40c8;
  undefined *local_40c0;
  __time_t _Stack_40b8;
  long lStack_40b0;
  __time_t _Stack_40a8;
  undefined1 local_100 [40];
  __dev_t local_d8;
  long local_d0;
  long local_a8;
  long local_a0;
  long local_78;
  
  lVar1 = tpidr_el0;
  local_78 = *(long *)(lVar1 + 0x28);
  uVar21 = 0x10006;
  DAT_0022e010 = param_1;
  FUN_001524e8("initialize_enter","a10_lab_only");
  local_4100._8_4_ = 0;
  local_4100._12_4_ = 0;
  local_4100._0_8_ = (undefined *)0x0;
  local_40e8 = (undefined *)0x0;
  local_4100[0x10] = 0;
  local_4100._17_4_ = 0;
  local_4100._21_3_ = 0;
  puStack_40d8 = (undefined *)0x0;
  puStack_40e0 = (undefined *)0x0;
  puStack_40c8 = (undefined *)0x0;
  local_40d0 = (undefined *)0x0;
  _Stack_40b8 = 0;
  local_40c0 = (undefined *)0x0;
  _Stack_40a8 = 0;
  lStack_40b0 = 0;
  iVar7 = __open_2("/proc/self/cmdline",0x80000);
  if (iVar7 < 0) {
LAB_00150d04:
    pcVar19 = "package_identity";
    FUN_001524e8("disabled","package_identity");
  }
  else {
    sVar10 = read(iVar7,local_4100,0x5f);
    close(iVar7);
    if ((sVar10 < 0x15) ||
       (((undefined *)local_4100._0_8_ != (undefined *)0x746975732e647362 ||
        CONCAT44(local_4100._12_4_,local_4100._8_4_) != 0x78656e2e65736163) ||
        CONCAT44(local_4100._17_4_,CONCAT13(local_4100[0x10],local_4100._13_3_)) != 0x3276737578656e
       )) goto LAB_00150d04;
    dl_iterate_phdr(FUN_001529bc,0);
    local_4100._8_4_ = 0;
    local_4100._0_8_ = DAT_0010f790;
    dl_iterate_phdr(FUN_00155cf0,local_4100);
    DAT_001a7c38 = local_4100._4_4_;
    if (local_4100._8_4_ != 1) {
      DAT_001a7c38 = -1;
    }
    lVar11 = sysconf(0x27);
    PTR_s_ui_initialize_001a7c40 = s_game_mapping_0013b998;
    if ((((DAT_0022e018 == 1) && (DAT_0022d8e0 != 0)) &&
        (PTR_s_ui_initialize_001a7c40 = s_process_memory_0013938d, -1 < DAT_001a7c38)) &&
       ((PTR_s_ui_initialize_001a7c40 = s_page_size_0013939c, lVar11 == 0x4000 || (lVar11 == 0x1000)
        ))) {
      PTR_s_ui_initialize_001a7c40 = s_game_file_identity_001382f9;
      iVar7 = __open_2(&DAT_0022e01c,0x88000);
      if (-1 < iVar7) {
        iVar8 = fstat(iVar7,(stat *)local_100);
        if (((iVar8 == 0) && ((local_100._16_4_ & 0xf000) == 0x8000)) && (local_d0 == 0x1340c90)) {
          FUN_00191314(local_4210.__unused + 1);
          sVar10 = read(iVar7,local_4100,0x4000);
          if (sVar10 < 1) {
            lVar22 = 0;
          }
          else {
            lVar22 = 0;
            do {
              lVar22 = lVar22 + sVar10;
              FUN_00191338(local_4210.__unused + 1,local_4100,sVar10);
              sVar10 = read(iVar7,local_4100,0x4000);
            } while (0 < sVar10);
          }
          iVar8 = fstat(iVar7,&local_4210);
          if ((iVar8 == 0) && (local_100._0_8_ == local_4210.st_dev)) {
            bVar4 = false;
            if ((local_100._8_8_ == local_4210.st_ino) && (local_4210.st_size == 0x1340c90)) {
              if (local_a8 != local_4210.st_mtim.tv_sec) goto LAB_00150f70;
              bVar4 = local_a0 == local_4210.st_mtim.tv_nsec;
            }
          }
          else {
LAB_00150f70:
            bVar4 = false;
          }
          close(iVar7);
          FUN_00191678(local_4210.__unused + 1,&local_4120);
          if ((((sVar10 == 0) && (bVar4)) && (lVar22 == 0x1340c90)) &&
             (((local_4120 == 0x2afb85406beb0aa1 && lStack_4118 == 0x896cd30a169d915) &&
              local_4110 == -0x51bf65d94a67e6dd) && lStack_4108 == -0x1c12db6926d6ee2c)) {
            PTR_s_ui_initialize_001a7c40 = s_original_code_guards_001360e9;
            iVar7 = FUN_00191058(DAT_0022d8e0,FUN_0014e7c4,0,&DAT_0010fbf8,0x13);
            if (((iVar7 != 0) && (iVar7 = FUN_00191170(DAT_0022d8e0,FUN_0014e7c4,0), iVar7 != 0)) &&
               (iVar7 = FUN_00152b7c(), iVar7 != 0)) {
              PTR_s_ui_initialize_001a7c40 = s_menu_environment_0013361b;
              local_4210.st_dev = 0;
              DAT_0022f020 = lVar11;
              iVar7 = (**(code **)(*param_1 + 0x30))(param_1,&local_4210,0x10006);
              if ((iVar7 == 0) && ((long *)local_4210.st_dev != (long *)0x0)) {
                PTR_s_ui_initialize_001a7c40 = s_menu_initialize_00131e93;
                iVar7 = nexus_menu_init();
                _Var2 = local_4210.st_dev;
                if (iVar7 == 1) {
                  PTR_s_ui_initialize_001a7c40 = s_storage_context_00139da4;
                  lVar11 = (**(code **)(*(long *)local_4210.st_dev + 0x30))
                                     (local_4210.st_dev,"nexus/loader/NexusLoader");
                  if ((((lVar11 != 0) &&
                       (lVar22 = (**(code **)(*(long *)_Var2 + 0x388))
                                           (_Var2,lVar11,"applicationContext",
                                            "()Landroid/content/Context;"), lVar22 != 0)) &&
                      (lVar22 = (**(code **)(*(long *)_Var2 + 0x390))(_Var2,lVar11,lVar22),
                      lVar22 != 0)) &&
                     (cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2), cVar5 == '\0')) {
                    uVar12 = (**(code **)(*(long *)_Var2 + 0xf8))(_Var2,lVar22);
                    lVar13 = (**(code **)(*(long *)_Var2 + 0x108))
                                       (_Var2,uVar12,"getFilesDir","()Ljava/io/File;");
                    if (((lVar13 != 0) &&
                        (lVar13 = (**(code **)(*(long *)_Var2 + 0x110))(_Var2,lVar22,lVar13),
                        lVar13 != 0)) &&
                       (cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2), cVar5 == '\0')) {
                      uVar14 = (**(code **)(*(long *)_Var2 + 0xf8))(_Var2,lVar13);
                      lVar15 = (**(code **)(*(long *)_Var2 + 0x108))
                                         (_Var2,uVar14,"getCanonicalPath","()Ljava/lang/String;");
                      if (((lVar15 != 0) &&
                          (lVar15 = (**(code **)(*(long *)_Var2 + 0x110))(_Var2,lVar13,lVar15),
                          lVar15 != 0)) &&
                         ((cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2), cVar5 == '\0' &&
                          (lVar16 = (**(code **)(*(long *)_Var2 + 0x548))(_Var2,lVar15,0),
                          lVar16 != 0)))) {
                        PTR_s_ui_initialize_001a7c40 = s_storage_private_files_00132f3e;
                        iVar7 = FUN_00155e94();
                        (**(code **)(*(long *)_Var2 + 0x550))(_Var2,lVar15,lVar16);
                        (**(code **)(*(long *)_Var2 + 0xb8))(_Var2,lVar15);
                        (**(code **)(*(long *)_Var2 + 0xb8))(_Var2,uVar14);
                        (**(code **)(*(long *)_Var2 + 0xb8))(_Var2,lVar13);
                        (**(code **)(*(long *)_Var2 + 0xb8))(_Var2,uVar12);
                        (**(code **)(*(long *)_Var2 + 0xb8))(_Var2,lVar22);
                        (**(code **)(*(long *)_Var2 + 0xb8))(_Var2,lVar11);
                        if (-1 < iVar7) {
                          PTR_s_ui_initialize_001a7c40 = s_storage_menu_directory_00131ea3;
                          iVar8 = mkdirat(iVar7,"nexus-menu",0x1c0);
                          if ((iVar8 == 0) || (piVar17 = (int *)__errno(), *piVar17 == 0x11)) {
                            DAT_001a7c50 = __openat_2(iVar7,"nexus-menu",0x8c000);
                            close(iVar7);
                            if ((-1 < DAT_001a7c50) &&
                               (iVar7 = fstat(DAT_001a7c50,(stat *)local_4100), iVar7 == 0)) {
                              _Var3 = (__uid_t)local_40e8;
                              _Var9 = getuid();
                              if ((_Var3 == _Var9) && ((local_4100[0x10] & 0x3f) == 0)) {
                                PTR_s_ui_initialize_001a7c40 = s_storage_registration_0013771a;
                                FUN_001560d8(DAT_001a7c50);
                                local_100._8_8_ = _UNK_00198bf8;
                                local_100._0_8_ = _DAT_00198bf0;
                                local_100._24_8_ = PTR_FUN_00198c08;
                                local_100._16_8_ = PTR_FUN_00198c00;
                                iVar7 = nexus_menu_register_storage(local_100);
                                if (iVar7 == 1) {
                                  local_100._8_8_ = _UNK_00198c18;
                                  local_100._0_8_ = _DAT_00198c10;
                                  local_100._32_4_ = _UNK_00110000;
                                  local_100._36_4_ = _UNK_00110004;
                                  local_100._24_8_ = _DAT_0010fff8;
                                  PTR_s_ui_initialize_001a7c40 = s_backend_binding_context_0013aef9;
                                  local_4100._8_4_ = (undefined4)_UNK_00198c48;
                                  local_4100._12_4_ = (undefined4)((ulong)_UNK_00198c48 >> 0x20);
                                  local_4100._0_8_ = _DAT_00198c40;
                                  local_40e8 = PTR_FUN_00198c58;
                                  local_4100[0x10] = (byte)PTR_FUN_00198c50;
                                  local_4100._17_4_ = (undefined4)((ulong)PTR_FUN_00198c50 >> 8);
                                  local_4100._21_3_ = (undefined3)((ulong)PTR_FUN_00198c50 >> 0x28);
                                  local_100._16_8_ = PTR_s_fdf834103d333f9f8a1947b3a405b32d_00198c20
                                  ;
                                  local_d8 = 0xfc1f05fff0;
                                  puStack_40d8 = PTR_FUN_00198c68;
                                  puStack_40e0 = PTR_FUN_00198c60;
                                  local_40d0 = PTR_FUN_00198c70;
                                  iVar7 = FUN_00169420(&DAT_0022f040,local_4100,local_100);
                                  _Var2 = local_4210.st_dev;
                                  if (iVar7 == 1) {
                                    PTR_s_ui_initialize_001a7c40 = s_ui_assets_001366bf;
                                    if (DAT_0022d868 == 0) {
                                      local_4100._0_8_ = (undefined *)0x0;
                                      iVar7 = (**(code **)(*(long *)local_4210.st_dev + 0x6d8))
                                                        (local_4210.st_dev,local_4100);
                                      if ((iVar7 == 0) &&
                                         ((undefined *)local_4100._0_8_ != (undefined *)0x0)) {
                                        lVar11 = (**(code **)(*(long *)_Var2 + 0x30))
                                                           (_Var2,"nexus/loader/h");
                                        if ((lVar11 == 0) ||
                                           (cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2),
                                           cVar5 != '\0')) {
                                          (**(code **)(*(long *)_Var2 + 0x88))(_Var2);
                                          __android_log_print(4,"NexusOnlineProbe","class_missing");
                                        }
                                        else {
                                          lVar22 = (**(code **)(*(long *)_Var2 + 0x388))
                                                             (_Var2,lVar11,"snapshot",&DAT_00135b2c)
                                          ;
                                          cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2);
                                          if (cVar5 == '\0') {
                                            if (lVar22 == 0) goto LAB_001516ec;
                                            lVar13 = (**(code **)(*(long *)_Var2 + 0xa8))
                                                               (_Var2,lVar11);
                                            local_4218 = 1;
                                          }
                                          else {
                                            (**(code **)(*(long *)_Var2 + 0x88))(_Var2);
                                            lVar22 = 0;
LAB_001516ec:
                                            local_4218 = 0;
                                            lVar13 = 0;
                                          }
                                          (**(code **)(*(long *)_Var2 + 0xb8))(_Var2,lVar11);
                                          cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2);
                                          if (cVar5 == '\0') {
                                            if (lVar13 == 0) goto LAB_00151ab0;
                                            uVar12 = 1;
                                            DAT_0022d860 = (undefined *)local_4100._0_8_;
                                            DAT_0022d868 = lVar13;
                                            DAT_0022d870 = lVar22;
                                          }
                                          else {
                                            (**(code **)(*(long *)_Var2 + 0x88))(_Var2);
LAB_00151ab0:
                                            uVar12 = 0;
                                          }
                                          __android_log_print(4,"NexusOnlineProbe",
                                                              "bound=%d method=%d",uVar12,local_4218
                                                             );
                                        }
                                      }
                                    }
                                    if (DAT_0022d838 == 0) {
                                      local_4100._0_8_ = (undefined *)0x0;
                                      iVar7 = (**(code **)(*(long *)_Var2 + 0x6d8))
                                                        (_Var2,local_4100);
                                      if ((iVar7 == 0) &&
                                         ((undefined *)local_4100._0_8_ != (undefined *)0x0)) {
                                        lVar11 = (**(code **)(*(long *)_Var2 + 0x30))
                                                           (_Var2,"nexus/bsd/ClientInputController")
                                        ;
                                        if ((lVar11 != 0) &&
                                           (cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2),
                                           cVar5 == '\0')) {
                                          local_4218 = (**(code **)(*(long *)_Var2 + 0x388))
                                                                 (_Var2,lVar11,"requestUtf8",
                                                                  "(I[B[BII)Z");
                                          cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2);
                                          if (cVar5 != '\0') {
                                            (**(code **)(*(long *)_Var2 + 0x88))(_Var2);
                                            local_4218 = 0;
                                          }
                                          local_4220 = (**(code **)(*(long *)_Var2 + 0x388))
                                                                 (_Var2,lVar11,&DAT_001348a4,"(I)[B"
                                                                 );
                                          cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2);
                                          if (cVar5 != '\0') {
                                            (**(code **)(*(long *)_Var2 + 0x88))(_Var2);
                                            local_4220 = 0;
                                          }
                                          lVar22 = (**(code **)(*(long *)_Var2 + 0x388))
                                                             (_Var2,lVar11,"dismiss",&DAT_0013725d);
                                          cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2);
                                          if (cVar5 == '\0') {
                                            lVar13 = 0;
                                            if (((local_4218 != 0) && (local_4220 != 0)) &&
                                               (lVar22 != 0)) {
                                              lVar13 = (**(code **)(*(long *)_Var2 + 0xa8))
                                                                 (_Var2,lVar11);
                                            }
                                          }
                                          else {
                                            (**(code **)(*(long *)_Var2 + 0x88))(_Var2);
                                            lVar22 = 0;
                                            lVar13 = 0;
                                          }
                                          (**(code **)(*(long *)_Var2 + 0xb8))(_Var2,lVar11);
                                          cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2);
                                          if (cVar5 == '\0') {
                                            if (lVar13 != 0) {
                                              DAT_0022f028 = (undefined *)local_4100._0_8_;
                                              DAT_0022d840 = local_4218;
                                              DAT_0022d848 = local_4220;
                                              DAT_0022d838 = lVar13;
                                              DAT_0022d850 = lVar22;
                                            }
                                            goto LAB_00151760;
                                          }
                                        }
                                        (**(code **)(*(long *)_Var2 + 0x88))(_Var2);
                                      }
                                    }
LAB_00151760:
                                    lVar11 = (**(code **)(*(long *)_Var2 + 0x30))
                                                       (_Var2,"nexus/loader/NexusLoader");
                                    if (((lVar11 != 0) &&
                                        (lVar22 = (**(code **)(*(long *)_Var2 + 0x388))
                                                            (_Var2,lVar11,"applicationContext",
                                                             "()Landroid/content/Context;"),
                                        lVar22 != 0)) &&
                                       ((lVar22 = (**(code **)(*(long *)_Var2 + 0x390))
                                                            (_Var2,lVar11,lVar22), lVar22 != 0 &&
                                        ((cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2),
                                         cVar5 == '\0' &&
                                         (lVar13 = (**(code **)(*(long *)_Var2 + 0x30))
                                                             (_Var2,"nexus/ui/UiAssetProof"),
                                         lVar13 != 0)))))) {
                                      lVar15 = (**(code **)(*(long *)_Var2 + 0x388))
                                                         (_Var2,lVar13,"verify",
                                                          "(Landroid/content/Context;)Z");
                                      if (lVar15 == 0) {
                                        cVar5 = '\0';
                                      }
                                      else {
                                        cVar5 = (**(code **)(*(long *)_Var2 + 0x3a8))
                                                          (_Var2,lVar13,lVar15,lVar22);
                                      }
                                      cVar6 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2);
                                      (**(code **)(*(long *)_Var2 + 0xb8))(_Var2,lVar13);
                                      (**(code **)(*(long *)_Var2 + 0xb8))(_Var2,lVar22);
                                      (**(code **)(*(long *)_Var2 + 0xb8))(_Var2,lVar11);
                                      DAT_0022f4c0 = (uint)(cVar6 == '\0' && cVar5 == '\x01');
                                      if (cVar6 == '\0' && cVar5 == '\x01') {
                                        iVar7 = (**(code **)(*(long *)_Var2 + 0x6d8))
                                                          (_Var2,&DAT_0022f4c8);
                                        if (iVar7 == 0) {
                                          lVar11 = (**(code **)(*(long *)_Var2 + 0x30))
                                                             (_Var2,
                                                  "nexus/brawl/activation/ActivationController");
                                          if (lVar11 == 0) {
                                            (**(code **)(*(long *)_Var2 + 0x88))(_Var2);
                                          }
                                          else {
                                            DAT_0022f4d0 = (**(code **)(*(long *)_Var2 + 0x388))
                                                                     (_Var2,lVar11,
                                                                      "nativeUiSnapshot",
                                                                      "()[Ljava/lang/String;");
                                            cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2);
                                            if (cVar5 != '\0') {
                                              (**(code **)(*(long *)_Var2 + 0x88))(_Var2);
                                              DAT_0022f4d0 = 0;
                                            }
                                            DAT_0022f4d8 = (**(code **)(*(long *)_Var2 + 0x388))
                                                                     (_Var2,lVar11,
                                                                      "dispatchNativeUi",
                                                                      &DAT_00133632);
                                            cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2);
                                            if (cVar5 != '\0') {
                                              (**(code **)(*(long *)_Var2 + 0x88))(_Var2);
                                              DAT_0022f4d8 = 0;
                                            }
                                            DAT_0022f4e0 = (**(code **)(*(long *)_Var2 + 0x388))
                                                                     (_Var2,lVar11,"dismissNativeUi"
                                                                      ,&DAT_0013ba46);
                                            cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2);
                                            if (cVar5 != '\0') {
                                              (**(code **)(*(long *)_Var2 + 0x88))(_Var2);
                                              DAT_0022f4e0 = 0;
                                            }
                                            if ((DAT_0022f4d0 != 0) && (DAT_0022f4d8 != 0)) {
                                              DAT_0022f4e8 = (**(code **)(*(long *)_Var2 + 0xa8))
                                                                       (_Var2,lVar11);
                                            }
                                            (**(code **)(*(long *)_Var2 + 0xb8))(_Var2,lVar11);
                                          }
                                        }
                                        if ((DAT_0022d8b0 & 1) == 0) {
                                          iVar7 = (**(code **)(*(long *)_Var2 + 0x6d8))
                                                            (_Var2,&DAT_0022d8d0);
                                          if (iVar7 == 0) {
                                            lVar11 = (**(code **)(*(long *)_Var2 + 0x30))
                                                               (_Var2,
                                                  "nexus/overlay/NexusOverlayBridge");
                                            if ((lVar11 == 0) ||
                                               (cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2)
                                               , cVar5 != '\0')) {
LAB_00151f28:
                                              lVar13 = 0;
                                              lVar22 = 0;
LAB_00151f30:
                                              cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2);
                                              if (cVar5 != '\0') {
                                                (**(code **)(*(long *)_Var2 + 0x88))(_Var2);
                                              }
                                              if (lVar13 != 0) {
                                                (**(code **)(*(long *)_Var2 + 0xb8))(_Var2,lVar13);
                                              }
                                              if (lVar22 != 0) {
                                                (**(code **)(*(long *)_Var2 + 0xb8))(_Var2,lVar22);
                                              }
                                              if (lVar11 != 0) {
                                                (**(code **)(*(long *)_Var2 + 0xb8))(_Var2,lVar11);
                                              }
                                            }
                                            else {
                                              memcpy(local_4100,&PTR_s_nativeSnapshot_0019acb8,0x108
                                                    );
                                              iVar7 = (**(code **)(*(long *)_Var2 + 0x6b8))
                                                                (_Var2,lVar11,local_4100,0xb);
                                              if ((iVar7 != 0) ||
                                                 (cVar5 = (**(code **)(*(long *)_Var2 + 0x720))
                                                                    (_Var2), cVar5 != '\0'))
                                              goto LAB_00151f28;
                                              lVar22 = (**(code **)(*(long *)_Var2 + 0x30))
                                                                 (_Var2,
                                                  "nexus/overlay/NexusOverlayHost");
                                              if ((((lVar22 == 0) ||
                                                   (cVar5 = (**(code **)(*(long *)_Var2 + 0x720))
                                                                      (_Var2), cVar5 != '\0')) ||
                                                  (lVar15 = (**(code **)(*(long *)_Var2 + 0x388))
                                                                      (_Var2,lVar22,"openFromNative"
                                                                       ,&DAT_001361b3), lVar15 == 0)
                                                  ) || (cVar5 = (**(code **)(*(long *)_Var2 + 0x720)
                                                                )(_Var2), cVar5 != '\0')) {
                                                lVar13 = 0;
                                                goto LAB_00151f30;
                                              }
                                              lVar13 = (**(code **)(*(long *)_Var2 + 0x30))
                                                                 (_Var2,
                                                  "nexus/overlay/NexusBattleMenuHost");
                                              if (((lVar13 == 0) ||
                                                  (cVar5 = (**(code **)(*(long *)_Var2 + 0x720))
                                                                     (_Var2), cVar5 != '\0')) ||
                                                 ((lVar16 = (**(code **)(*(long *)_Var2 + 0x388))
                                                                      (_Var2,lVar13,
                                                                       "onNativeBattleState","(ZJ)Z"
                                                                      ), lVar16 == 0 ||
                                                  (cVar5 = (**(code **)(*(long *)_Var2 + 0x720))
                                                                     (_Var2), cVar5 != '\0'))))
                                              goto LAB_00151f30;
                                              local_4230 = (**(code **)(*(long *)_Var2 + 0x388))
                                                                     (_Var2,lVar13,
                                                                      "togglePanelFromNative",
                                                                      &DAT_00133bbb);
                                              cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2);
                                              if (cVar5 != '\0') {
                                                (**(code **)(*(long *)_Var2 + 0x88))(_Var2);
                                                local_4230 = 0;
                                              }
                                              DAT_0022f030 = (**(code **)(*(long *)_Var2 + 0xa8))
                                                                       (_Var2,lVar22);
                                              DAT_0022f4f0 = lVar15;
                                              DAT_0022d8b8 = (**(code **)(*(long *)_Var2 + 0xa8))
                                                                       (_Var2,lVar13);
                                              DAT_0022d8c0 = local_4230;
                                              DAT_0022f4f8 = lVar16;
                                              (**(code **)(*(long *)_Var2 + 0xb8))(_Var2,lVar13);
                                              (**(code **)(*(long *)_Var2 + 0xb8))(_Var2,lVar22);
                                              (**(code **)(*(long *)_Var2 + 0xb8))(_Var2,lVar11);
                                              if (((DAT_0022f030 != 0) && (DAT_0022d8b8 != 0)) &&
                                                 (cVar5 = (**(code **)(*(long *)_Var2 + 0x720))
                                                                    (_Var2), cVar5 == '\0')) {
                                                DAT_0022d8c8._0_4_ = 1;
                                                DAT_0022d8b0 = 1;
                                                goto LAB_00151b50;
                                              }
                                            }
                                            cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2);
                                            if (cVar5 != '\0') {
                                              (**(code **)(*(long *)_Var2 + 0x88))(_Var2);
                                            }
                                          }
                                          DAT_0022f4c0 = 0;
                                        }
                                        else {
LAB_00151b50:
                                          _Var2 = local_4210.st_dev;
                                          PTR_s_ui_initialize_001a7c40 =
                                               s_social_registration_00139877;
                                          DAT_0022f4c0 = 1;
                                          iVar7 = (**(code **)(*(long *)local_4210.st_dev + 0x98))
                                                            (local_4210.st_dev,0xc);
                                          if (iVar7 < 0) {
                                            (**(code **)(*(long *)_Var2 + 0x88))(_Var2);
                                          }
                                          else {
                                            lVar11 = (**(code **)(*(long *)_Var2 + 0x30))
                                                               (_Var2,
                                                  "com/supercell/titan/NativeDialogManager");
                                            lVar22 = (**(code **)(*(long *)_Var2 + 0x30))
                                                               (_Var2,"android/app/DialogFragment");
                                            lVar13 = (**(code **)(*(long *)_Var2 + 0x30))
                                                               (_Var2,"android/app/Dialog");
                                            lVar15 = (**(code **)(*(long *)_Var2 + 0x30))
                                                               (_Var2,"android/widget/TextView");
                                            lVar16 = (**(code **)(*(long *)_Var2 + 0x30))
                                                               (_Var2,"java/lang/Object");
                                            lVar18 = (**(code **)(*(long *)_Var2 + 0x30))
                                                               (_Var2,"android/view/View");
                                            cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2);
                                            if ((((((cVar5 == '\0') && (lVar11 != 0)) &&
                                                  (lVar22 != 0)) && ((lVar13 != 0 && (lVar15 != 0)))
                                                 ) && (lVar16 != 0)) && (lVar18 != 0)) {
                                              DAT_0025c8e0 = (**(code **)(*(long *)_Var2 + 0x480))
                                                                       (_Var2,lVar11,&DAT_00137c33,
                                                                                                                                                
                                                  "Lcom/supercell/titan/NativeDialogManager;");
                                              DAT_0025c8e8 = (**(code **)(*(long *)_Var2 + 0x108))
                                                                       (_Var2,lVar22,"getDialog",
                                                                        "()Landroid/app/Dialog;");
                                              DAT_0025c8f0 = (**(code **)(*(long *)_Var2 + 0x108))
                                                                       (_Var2,lVar13,"findViewById",
                                                                        "(I)Landroid/view/View;");
                                              DAT_0025c8f8 = (**(code **)(*(long *)_Var2 + 0x108))
                                                                       (_Var2,lVar15,"getText",
                                                                        "()Ljava/lang/CharSequence;"
                                                                       );
                                              DAT_0025c900 = (**(code **)(*(long *)_Var2 + 0x108))
                                                                       (_Var2,lVar16,"toString",
                                                                        "()Ljava/lang/String;");
                                              DAT_0025c908 = (**(code **)(*(long *)_Var2 + 0x108))
                                                                       (_Var2,lVar18,"performClick",
                                                                        &DAT_001361b3);
                                              DAT_0025c910 = (**(code **)(*(long *)_Var2 + 0x108))
                                                                       (_Var2,lVar18,"isShown",
                                                                        &DAT_001361b3);
                                              DAT_0025c918 = (**(code **)(*(long *)_Var2 + 0x108))
                                                                       (_Var2,lVar18,"isEnabled",
                                                                        &DAT_001361b3);
                                              cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2);
                                              if (((((cVar5 == '\0') && (DAT_0025c8e0 != 0)) &&
                                                   ((DAT_0025c8e8 != 0 &&
                                                    (((DAT_0025c8f0 != 0 && (DAT_0025c8f8 != 0)) &&
                                                     (DAT_0025c900 != 0)))))) &&
                                                  ((DAT_0025c908 != 0 && (DAT_0025c910 != 0)))) &&
                                                 (DAT_0025c918 != 0)) {
                                                DAT_0025c920 = (**(code **)(*(long *)_Var2 + 0xa8))
                                                                         (_Var2,lVar11);
                                              }
                                            }
                                            cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2);
                                            if (cVar5 != '\0') {
                                              (**(code **)(*(long *)_Var2 + 0x88))(_Var2);
                                            }
                                            (**(code **)(*(long *)_Var2 + 0xa0))(_Var2,0);
                                          }
                                          lVar11 = (**(code **)(*(long *)_Var2 + 0x30))
                                                             (_Var2,"com/supercell/titan/GameApp");
                                          if (lVar11 != 0) {
                                            DAT_0025c8d0 = (**(code **)(*(long *)_Var2 + 0x388))
                                                                     (_Var2,lVar11,"getInstance",
                                                                                                                                            
                                                  "()Lcom/supercell/titan/GameApp;");
                                            if ((DAT_0025c8d0 != 0) &&
                                               (cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2)
                                               , cVar5 == '\0')) {
                                              DAT_0025c8d8 = (**(code **)(*(long *)_Var2 + 0xa8))
                                                                       (_Var2,lVar11);
                                            }
                                            (**(code **)(*(long *)_Var2 + 0xb8))(_Var2,lVar11);
                                          }
                                          cVar5 = (**(code **)(*(long *)_Var2 + 0x720))(_Var2);
                                          if (cVar5 != '\0') {
                                            (**(code **)(*(long *)_Var2 + 0x88))(_Var2);
                                          }
                                          local_4100._8_4_ = (undefined4)_UNK_0019adc8;
                                          local_4100._12_4_ =
                                               (undefined4)((ulong)_UNK_0019adc8 >> 0x20);
                                          local_4100._0_8_ = _DAT_0019adc0;
                                          local_4100[0x10] = (byte)_DAT_0019add0;
                                          local_4100._17_4_ =
                                               (undefined4)((ulong)_DAT_0019add0 >> 8);
                                          local_4100._21_3_ =
                                               (undefined3)((ulong)_DAT_0019add0 >> 0x28);
                                          puStack_40d8 = _UNK_0019ade8;
                                          puStack_40c8 = _UNK_0019adf8;
                                          local_40d0 = PTR_FUN_0019adf0;
                                          puStack_40e0 = _UNK_0010fa18;
                                          local_40e8 = _DAT_0010fa10;
                                          iVar7 = nexus_menu_register_backend(1,local_4100);
                                          if (iVar7 == 1) {
                                            PTR_s_ui_initialize_001a7c40 =
                                                 s_menu_facade_class_00133b58;
                                            lVar11 = (**(code **)(*(long *)local_4210.st_dev + 0x30)
                                                     )(local_4210.st_dev,
                                                                                                              
                                                  "nexus/brawl/evasion/AdvancedEvasionController");
                                            if (lVar11 != 0) {
                                              puStack_40d8 = PTR_FUN_00198bd0;
                                              puStack_40e0 = PTR_DAT_00198bc8;
                                              puStack_40c8 = PTR_s___Ljava_lang_String__00198be0;
                                              local_40d0 = PTR_s_getNativeMenuDiagnostics_00198bd8;
                                              local_40c0 = PTR_FUN_00198be8;
                                              PTR_s_ui_initialize_001a7c40 =
                                                   s_menu_method_signature_001341a5;
                                              local_4100._8_4_ = SUB84(PTR_DAT_00198bb0,0);
                                              local_4100._12_4_ =
                                                   (undefined4)((ulong)PTR_DAT_00198bb0 >> 0x20);
                                              local_4100._0_8_ = PTR_s_startNativeUiHooks_00198ba8;
                                              local_40e8 = PTR_s_getNativeMenuLoadState_00198bc0;
                                              local_4100[0x10] = (byte)PTR_FUN_00198bb8;
                                              local_4100._17_4_ =
                                                   (undefined4)((ulong)PTR_FUN_00198bb8 >> 8);
                                              local_4100._21_3_ =
                                                   (undefined3)((ulong)PTR_FUN_00198bb8 >> 0x28);
                                              lVar22 = (**(code **)(*(long *)local_4210.st_dev +
                                                                   0x388))(local_4210.st_dev,lVar11,
                                                                           "startNativeUiHooks",
                                                                           &DAT_0013ba46);
                                              if (((lVar22 == 0) ||
                                                  (lVar22 = (**(code **)(*(long *)local_4210.st_dev
                                                                        + 0x388))
                                                                      (local_4210.st_dev,lVar11,
                                                                       "getNativeMenuLoadState",
                                                                       &DAT_00135b2c), lVar22 == 0))
                                                 || (lVar22 = (**(code **)(*(long *)local_4210.
                                                                                    st_dev + 0x388))
                                                                        (local_4210.st_dev,lVar11,
                                                                         "getNativeMenuDiagnostics",
                                                                         "()Ljava/lang/String;"),
                                                    lVar22 == 0)) {
                                                (**(code **)(*(long *)local_4210.st_dev + 0xb8))
                                                          (local_4210.st_dev,lVar11);
                                              }
                                              else {
                                                PTR_s_ui_initialize_001a7c40 =
                                                     s_menu_native_registration_00137c08;
                                                iVar7 = (**(code **)(*(long *)local_4210.st_dev +
                                                                    0x6b8))(local_4210.st_dev,lVar11
                                                                            ,local_4100,3);
                                                (**(code **)(*(long *)local_4210.st_dev + 0xb8))
                                                          (local_4210.st_dev,lVar11);
                                                if (iVar7 == 0) {
                                                  iVar7 = FUN_00153060();
                                                  if (iVar7 == 1) {
                                                    iVar8 = FUN_00153670();
                                                    if (iVar8 == 0) {
                                                      FUN_001524e8("social_unavailable",
                                                                   "native_guard_or_install");
                                                    }
                                                    pcVar19 = "hooks_installed";
                                                    pcVar20 = "four_preserving_observers";
                                                  }
                                                  else {
                                                    if (iVar7 < 0) {
                                                      FUN_001524e8("fatal",
                                                  "target_write_and_restore_unverified");
                                                  FUN_001525f4(param_1,"target_restore_unverified",
                                                               "native_link_failed");
                    /* WARNING: Subroutine does not return */
                                                  abort();
                                                  }
                                                  pcVar19 = "disabled";
                                                  pcVar20 = "publication_failed_passthrough";
                                                  }
                                                  DAT_0022db78._0_4_ = (uint)(iVar7 == 1);
                                                  FUN_001524e8(pcVar19,pcVar20);
                                                  FUN_00153a10(param_1);
                                                  goto LAB_00150d30;
                                                }
                                              }
                                            }
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                          else {
                            close(iVar7);
                          }
                        }
                      }
                    }
                  }
                }
              }
              FUN_001524e8("disabled","menu_host_or_JNI_registration");
              FUN_001525f4(param_1,PTR_s_ui_initialize_001a7c40,"native_link_failed");
              uVar21 = 0xffffffff;
              goto LAB_00150d30;
            }
          }
        }
        else {
          close(iVar7);
        }
      }
    }
    FUN_001524e8("disabled",PTR_s_ui_initialize_001a7c40);
    pcVar19 = PTR_s_ui_initialize_001a7c40;
  }
  FUN_001525f4(param_1,pcVar19,"required_synchronous_callback_absent");
LAB_00150d30:
  if (*(long *)(lVar1 + 0x28) == local_78) {
    return uVar21;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001524e8 @ 001524e8 ===== */

void FUN_001524e8(undefined8 param_1,undefined8 param_2)

{
  long lVar1;
  __pid_t _Var2;
  undefined4 uVar3;
  int iVar4;
  long lVar5;
  timespec local_58;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  if (DAT_0022f428 < 0x40) {
    DAT_0022f428 = DAT_0022f428 + 1;
    _Var2 = getpid();
    uVar3 = gettid(_Var2);
    iVar4 = clock_gettime(0,&local_58);
    if (iVar4 == 0) {
      lVar5 = local_58.tv_sec * 1000 + (ulong)local_58.tv_nsec / 1000000;
    }
    else {
      lVar5 = 0;
    }
    __android_log_print(4,"NexusLab69252",
                        "{\"stage\":\"runtime_ui_%s\",\"reason\":\"%s\",\"pid\":%d,\"tid\":%d,\"time_ms\":%llu,\"game_sha256\":\"%s\",\"stage_ptr\":\"%p\",\"root\":\"%p\",\"launcher\":\"%p\",\"panel\":\"%p\",\"open\":%d,\"scheduling_rva\":\"0x5921a0\",\"observed_text\":\"%p\",\"observed_movie\":\"%p\",\"observed_movie_vptr\":\"%p\",\"movie_be_s16\":%d}"
                        ,param_1,param_2,_Var2,uVar3,lVar5,
                        "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3",0,0,0,0,0
                        ,0,0,0,0);
  }
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001554b0 @ 001554b0 ===== */

void FUN_001554b0(undefined8 param_1)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  ulong uVar4;
  ulong uVar5;
  long lVar6;
  ulong local_70;
  ulong local_68;
  byte local_5c [4];
  char local_58 [4];
  char local_54 [4];
  ulong local_50 [3];
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  uVar3 = FUN_0014e7c4(param_1,DAT_0022d8e0 + 0x12eb0e8,local_54,1);
  if ((int)uVar3 != 0 && local_54[0] == '\0') {
    iVar2 = FUN_0014e7c4(uVar3,DAT_0022d8e0 + 0x12eb170,local_58,1);
    uVar5 = 0;
    if ((((iVar2 == 0) || (local_58[0] != '\0')) ||
        (uVar4 = (*(code *)(DAT_0022d8e0 + 0x51eacc))("sc/ui.sc",0), uVar5 = uVar4, uVar4 == 0)) ||
       (uVar5 = FUN_0014e7c4(uVar4,uVar4 + 0x80,local_5c,1), (int)uVar5 == 0)) goto LAB_00155504;
    if ((local_5c[0] & 1) != 0) {
      local_50[0] = 0;
      uVar3 = FUN_0014e7c4(uVar5,uVar4 + 0x88,local_50,8);
      if ((((int)uVar3 != 0) && (0xfff < local_50[0])) && ((local_50[0] & 7) == 0)) {
        lVar6 = local_50[0] + 8;
        local_50[0] = 0;
        uVar3 = FUN_0014e7c4(uVar3,lVar6,local_50,8);
        uVar4 = local_50[0];
        if ((((int)uVar3 != 0) && (0xfff < local_50[0])) && ((local_50[0] & 7) == 0)) {
          lVar6 = local_50[0] + 0xf8;
          local_50[0] = 0;
          iVar2 = FUN_0014e7c4(uVar3,lVar6,local_50,8);
          uVar5 = 0;
          local_70 = 0;
          local_68 = 0;
          if (((iVar2 == 0) || (local_50[0] < 0x1000)) || ((local_50[0] & 7) != 0))
          goto LAB_00155504;
          iVar2 = FUN_0014e7c4(0,uVar4 + 0x100,&local_68,8);
          uVar5 = 0;
          if (((iVar2 == 0) || (local_68 == 0)) || (0x10000 < local_68)) goto LAB_00155504;
          iVar2 = FUN_0014e7c4(0,uVar4 + 0x110,&local_70,8);
          uVar5 = 0;
          if (((iVar2 == 0) || (local_70 == 0)) || (0x10000 < local_70)) goto LAB_00155504;
          iVar2 = FUN_00190fd0("popover_button_blue",local_50);
          if (iVar2 != 0) {
            uVar5 = (*(code *)(DAT_0022d8e0 + 0x5dfbf8))(lVar6,local_50);
            if (uVar5 == 0) goto LAB_00155504;
            iVar2 = FUN_00190fd0("popover_button_spectate",local_50);
            if (iVar2 != 0) {
              lVar6 = (*(code *)(DAT_0022d8e0 + 0x5dfbf8))(lVar6,local_50);
              uVar5 = (ulong)(lVar6 != 0);
              goto LAB_00155504;
            }
          }
        }
      }
    }
  }
  uVar5 = 0;
LAB_00155504:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_00158f5c @ 00158f5c ===== */

void FUN_00158f5c(long *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x00158f6c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*param_1 + 0x538))
            (param_1,
             "{\"schema\":\"nexus-overlay-wire/v1\",\"categories\":[{\"id\":\"main\",\"label\":\"Main\",\"icon\":\"sliders\"},{\"id\":\"combat\",\"label\":\"Combat\",\"icon\":\"crosshair\"},{\"id\":\"visual\",\"label\":\"Visual\",\"icon\":\"eye\"},{\"id\":\"utility\",\"label\":\"Utility\",\"icon\":\"wrench\"},{\"id\":\"settings\",\"label\":\"Settings\",\"icon\":\"gear\"},{\"id\":\"plus\",\"label\":\"Nexus+\",\"icon\":\"star\"}],\"entries\":[{\"key\":\"menu.killaura\",\"label\":\"Kill aura\",\"description\":\"\\u041d\\u0430\\u0441\\u0442\\u0440\\u043e\\u0439\\u043a\\u0430 \\u0430\\u0432\\u0442\\u043e\\u043c\\u0430\\u0442\\u0438\\u0447\\u0435\\u0441\\u043a\\u043e\\u0433\\u043e \\u0432\\u044b\\u0431\\u043e\\u0440\\u0430 \\u0446\\u0435\\u043b\\u0438 \\u0438 \\u0430\\u0442\\u0430\\u043a\\u0438.\",\"icon\":\"crosshair\",\"category\":\"combat\",\"parentKey\":\"\",\"control\":4,\"actionId\":7,\"queryActionId\":17,\"min\":0,\"max\":0,\"step\":0,\"incrementActionId\":-1,\"decrementActionId\":-1,\"screen\":75,\"paid\":false,\"topLevel\":true,\"options\":[],\"aliases\":[7]},{\"key\":\"menu.autododge\",\"label\":\"Auto dodge\",\"description\":\"\\u041d\\u0430\\u0441\\u0442\\u0440\\u043e\\u0439\\u043a\\u0430 \\u0443\\u043a\\u043b\\u043e\\u043d\\u0435\\u043d\\u0438\\u044f \\u043e\\u0442 \\u0441\\u043d\\u0430\\u0440\\u044f\\u0434\\u043e\\u0432.\",\"icon\":\"shield\",\"category\":\"combat\",\"parentKey\":\"\",\"control\":4,\"actionId\":8,\"queryActionId\":22,\"min\":0,\"max\":0,\"step\":0,\"incrementActionId\":-1,\"decrementActionId\":-1,\"screen\":68,\"paid\":false,\"topLevel\":true,\"options\":[],\"aliases\":[8]},{\"key\":\"menu.follow\",\"label\":\"Follow\",\"description\":\"\\u041d\\u0430\\u0441\\u0442\\u0440\\u043e\\u0439\\u043a\\u0430 \\u0441\\u043b\\u0435\\u0434\\u043e\\u0432\\u0430\\u043d\\u0438\\u044f \\u0437\\u0430 \\u0441\\u043e\\u044e\\u0437\\u043d\\u0438\\u043a\\u043e\\u043c.\",\"icon\":\"user\",\"category\":\"utility\",\"parentKey\":\"\",\"control\":4,\"actionId\":10,\"queryActionId\":26,\"min\":0,\"max\":0,\"step\":0,\"incrementActionId\":-1,\"decrementActionId\":-1,\"screen\":70,\"paid\":true,\"topLevel\":true,\"options\":[],\"aliases\":[10]},{\"key\":\"menu.aim\",\"label\":\"Smart aim\",\"description\":\"\\u041d\\u0430\\u0441\\u0442\\u0440\\u043e\\u0439\\u043a\\u0430 \\u043f\\u0440\\u0438\\u0446\\u0435\\u043b\\u0438\\u0432\\u0430\\u043d\\u0438\\u044f \\u0441 \\u0443\\u043f\\u0440\\u04..." /* TRUNCATED STRING LITERAL */
            );
  return;
}

/* ===== FUN_0015a108 @ 0015a108 ===== */

undefined8 FUN_0015a108(int *param_1)

{
  char *pcVar1;
  char *pcVar2;
  undefined *puVar3;
  long lVar4;
  long *plVar5;
  bool bVar6;
  char cVar7;
  char cVar8;
  char cVar9;
  int iVar10;
  int iVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  long lVar18;
  long lVar19;
  long lVar20;
  long lVar21;
  long lVar22;
  long lVar23;
  long lVar24;
  long lVar25;
  long lVar26;
  long lVar27;
  long lVar28;
  long lVar29;
  long lVar30;
  long lVar31;
  long lVar32;
  long lVar33;
  long lVar34;
  long lVar35;
  long lVar36;
  long lVar37;
  undefined8 uVar38;
  undefined8 uVar39;
  undefined8 uVar40;
  undefined4 uVar41;
  long lVar42;
  bool bVar43;
  long *local_78;
  long local_70;
  
  lVar4 = tpidr_el0;
  local_70 = *(long *)(lVar4 + 0x28);
  local_78 = (long *)0x0;
  if (DAT_0022f4c8 == (long *)0x0 || DAT_0022f4e8 == 0) {
LAB_0015a1a0:
    iVar10 = 0;
  }
  else {
    iVar10 = (**(code **)(*DAT_0022f4c8 + 0x30))(DAT_0022f4c8,&local_78,0x10006);
    if (iVar10 != 0) {
      if ((iVar10 != -2) ||
         (iVar10 = (**(code **)(*DAT_0022f4c8 + 0x20))(DAT_0022f4c8,&local_78,0), iVar10 != 0))
      goto LAB_0015a1a0;
      iVar10 = 1;
    }
    plVar5 = local_78;
    if (local_78 != (long *)0x0) {
      iVar11 = (**(code **)(*local_78 + 0x98))(local_78,0x30);
      bVar6 = iVar11 == 0;
      if (iVar11 != 0) {
LAB_0015a238:
        lVar29 = 0;
        goto LAB_0015a23c;
      }
      lVar12 = (**(code **)(*plVar5 + 0x30))(plVar5,"android/os/Looper");
      lVar29 = lVar12;
      if (lVar12 == 0) goto LAB_0015a23c;
      cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
      if (((cVar7 != '\0') ||
          (lVar13 = (**(code **)(*plVar5 + 0x30))(plVar5,"android/app/AlertDialog$Builder"),
          lVar13 == 0)) || (cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0'))
      goto LAB_0015a238;
      lVar42 = (**(code **)(*plVar5 + 0x30))(plVar5,"android/app/AlertDialog");
      if (lVar42 == 0) {
        lVar29 = 0;
        lVar42 = 0;
        goto LAB_0015a240;
      }
      cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
      if ((((((cVar7 != '\0') ||
             (lVar14 = (**(code **)(*plVar5 + 0x30))(plVar5,"android/widget/EditText"), lVar14 == 0)
             ) || ((cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0' ||
                   ((lVar15 = (**(code **)(*plVar5 + 0x30))(plVar5,"android/app/Activity"),
                    lVar15 == 0 || (cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0')))
                   ))) ||
           (lVar16 = (**(code **)(*plVar5 + 0x30))(plVar5,"java/lang/Object"), lVar16 == 0)) ||
          ((((((cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0' ||
               (lVar17 = (**(code **)(*plVar5 + 0x30))(plVar5,"android/view/Window"), lVar17 == 0))
              || (cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0')) ||
             ((lVar18 = (**(code **)(*plVar5 + 0x388))(plVar5,lVar12,"prepare",&DAT_0013ba46),
              lVar18 == 0 || (cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0')))) ||
            (lVar19 = (**(code **)(*plVar5 + 0x388))
                                (plVar5,lVar12,"myLooper","()Landroid/os/Looper;"), lVar19 == 0)) ||
           (((cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0' ||
             (lVar20 = (**(code **)(*plVar5 + 0x388))(plVar5,lVar12,&DAT_0013af3f,&DAT_0013ba46),
             lVar20 == 0)) ||
            ((cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0' ||
             (((lVar21 = (**(code **)(*plVar5 + 0x108))
                                   (plVar5,lVar13,"<init>","(Landroid/content/Context;)V"),
               lVar21 == 0 || (cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0')) ||
              (lVar22 = (**(code **)(*plVar5 + 0x108))
                                  (plVar5,lVar14,"<init>","(Landroid/content/Context;)V"),
              lVar22 == 0)))))))))) ||
         (((((cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0' ||
             (lVar23 = (**(code **)(*plVar5 + 0x108))
                                 (plVar5,lVar13,"setTitle",
                                  "(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;"),
             lVar23 == 0)) || (cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0')) ||
           ((lVar24 = (**(code **)(*plVar5 + 0x108))
                                (plVar5,lVar13,"setView",
                                 "(Landroid/view/View;)Landroid/app/AlertDialog$Builder;"),
            lVar24 == 0 || (cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0')))) ||
          (((((lVar25 = (**(code **)(*plVar5 + 0x108))
                                  (plVar5,lVar13,"setPositiveButton",
                                   "(Ljava/lang/CharSequence;Landroid/content/DialogInterface$OnClickListener;)Landroid/app/AlertDialog$Builder;"
                                  ), lVar25 == 0 ||
              (((cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0' ||
                (lVar26 = (**(code **)(*plVar5 + 0x108))
                                    (plVar5,lVar13,"setCancelable",
                                     "(Z)Landroid/app/AlertDialog$Builder;"), lVar26 == 0)) ||
               (cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0')))) ||
             ((lVar27 = (**(code **)(*plVar5 + 0x108))
                                  (plVar5,lVar13,"create","()Landroid/app/AlertDialog;"),
              lVar27 == 0 || (cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0')))) ||
            (lVar28 = (**(code **)(*plVar5 + 0x108))(plVar5,lVar42,&DAT_00137c35,&DAT_0013ba46),
            lVar28 == 0)) || (cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0'))))))
      goto LAB_0015a238;
      lVar29 = (**(code **)(*plVar5 + 0x108))(plVar5,lVar42,"dismiss",&DAT_0013ba46);
      if (((((lVar29 == 0) ||
            (lVar30 = (**(code **)(*plVar5 + 0x108))
                                (plVar5,lVar42,"setCanceledOnTouchOutside",&DAT_0013b4c4),
            lVar30 == 0)) ||
           ((cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0' ||
            ((((lVar31 = (**(code **)(*plVar5 + 0x108))(plVar5,lVar14,"setSingleLine",&DAT_0013b4c4)
               , lVar31 == 0 || (cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0')) ||
              (lVar32 = (**(code **)(*plVar5 + 0x108))
                                  (plVar5,lVar14,"setHint","(Ljava/lang/CharSequence;)V"),
              lVar32 == 0)) ||
             ((cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0' ||
              (lVar33 = (**(code **)(*plVar5 + 0x108))(plVar5,lVar14,"setInputType",&DAT_0013725d),
              lVar33 == 0)))))))) ||
          ((((cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0' ||
             ((lVar34 = (**(code **)(*plVar5 + 0x108))
                                  (plVar5,lVar14,"getText","()Landroid/text/Editable;"), lVar34 == 0
              || (cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0')))) ||
            ((lVar16 = (**(code **)(*plVar5 + 0x108))
                                 (plVar5,lVar16,"toString","()Ljava/lang/String;"), lVar16 == 0 ||
             (((cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0' ||
               (lVar35 = (**(code **)(*plVar5 + 0x108))(plVar5,lVar14,"requestFocus",&DAT_001361b3),
               lVar35 == 0)) || (cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0'))))))
           || ((((lVar36 = (**(code **)(*plVar5 + 0x108))
                                     (plVar5,lVar42,"getWindow","()Landroid/view/Window;"),
                 lVar36 == 0 || (cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0')) ||
                (lVar17 = (**(code **)(*plVar5 + 0x108))
                                    (plVar5,lVar17,"setSoftInputMode",&DAT_0013725d), lVar17 == 0))
               || ((cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0' ||
                   (lVar37 = (**(code **)(*plVar5 + 0x108))
                                       (plVar5,lVar15,"isFinishing",&DAT_001361b3), lVar37 == 0)))))
           ))) || ((cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0' ||
                   ((lVar15 = (**(code **)(*plVar5 + 0x108))
                                        (plVar5,lVar15,"isDestroyed",&DAT_001361b3), lVar15 == 0 ||
                    (cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5), cVar7 != '\0')))))) {
LAB_0015a23c:
        lVar42 = 0;
LAB_0015a240:
        bVar43 = true;
LAB_0015a244:
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') {
          (**(code **)(*plVar5 + 0x88))(plVar5);
        }
        if ((lVar42 != 0) && (lVar29 != 0)) {
          (**(code **)(*plVar5 + 0x1e8))(plVar5,lVar42,lVar29);
          cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
          if (cVar7 != '\0') {
            (**(code **)(*plVar5 + 0x88))(plVar5);
          }
        }
        FUN_0015b620(&DAT_0013a33f);
        param_1[0xd] = 1;
        if (!bVar43) {
          pthread_join((pthread_t)local_78,(void **)0x0);
        }
      }
      else {
        (**(code **)(*plVar5 + 0x468))(plVar5,lVar12,lVar18);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015a23c;
        lVar18 = (**(code **)(*plVar5 + 0x390))(plVar5,lVar12,lVar19);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015a23c;
        lVar19 = (**(code **)(*plVar5 + 0x390))(plVar5,DAT_0025c8d8,DAT_0025c8d0);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015a23c;
        lVar42 = 0;
        bVar43 = true;
        if ((lVar18 == 0) || (lVar19 == 0)) goto LAB_0015a244;
        lVar13 = (**(code **)(*plVar5 + 0xe0))(plVar5,lVar13,lVar21,lVar19);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') {
LAB_0015ae6c:
          lVar42 = 0;
          goto LAB_0015a244;
        }
        lVar14 = (**(code **)(*plVar5 + 0xe0))(plVar5,lVar14,lVar22,lVar19);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015ae6c;
        lVar42 = 0;
        if ((lVar13 == 0) || (lVar14 == 0)) goto LAB_0015a244;
        pcVar1 = "Invite By Tag";
        if (*param_1 != 0x80) {
          pcVar1 = "Visual BrawlTV";
        }
        pcVar2 = "Spectate With Tag";
        if (*param_1 != 0x7d) {
          pcVar2 = pcVar1;
        }
        uVar38 = (**(code **)(*plVar5 + 0x538))(plVar5,pcVar2);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015ae6c;
        puVar3 = &UNK_001317d0;
        if (*param_1 != 0x7f) {
          puVar3 = &DAT_00139425;
        }
        uVar39 = (**(code **)(*plVar5 + 0x538))(plVar5,puVar3);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015ae6c;
        uVar40 = (**(code **)(*plVar5 + 0x538))(plVar5,&DAT_0013af51);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015ae6c;
        (**(code **)(*plVar5 + 0x1e8))(plVar5,lVar14,lVar31,1);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015ae6c;
        (**(code **)(*plVar5 + 0x1e8))(plVar5,lVar14,lVar32,uVar39);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015ae6c;
        uVar41 = 2;
        if (*param_1 != 0x7f) {
          uVar41 = 0x1091;
        }
        (**(code **)(*plVar5 + 0x1e8))(plVar5,lVar14,lVar33,uVar41);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015ae6c;
        (**(code **)(*plVar5 + 0x110))(plVar5,lVar13,lVar23,uVar38);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015ae6c;
        (**(code **)(*plVar5 + 0x110))(plVar5,lVar13,lVar24,lVar14);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015ae6c;
        (**(code **)(*plVar5 + 0x110))(plVar5,lVar13,lVar25,uVar40,0);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015ae6c;
        (**(code **)(*plVar5 + 0x110))(plVar5,lVar13,lVar26,0);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015ae6c;
        lVar42 = (**(code **)(*plVar5 + 0x110))(plVar5,lVar13,lVar27);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if ((cVar7 != '\0') || (lVar42 == 0)) goto LAB_0015a244;
        (**(code **)(*plVar5 + 0x1e8))(plVar5,lVar42,lVar30,0);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015a244;
        (**(code **)(*plVar5 + 0x1e8))(plVar5,lVar42,lVar28);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015a244;
        (**(code **)(*plVar5 + 0x128))(plVar5,lVar14,lVar35);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015a244;
        lVar13 = (**(code **)(*plVar5 + 0x110))(plVar5,lVar42,lVar36);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015a244;
        if (lVar13 != 0) {
          (**(code **)(*plVar5 + 0x1e8))(plVar5,lVar13,lVar17,0x15);
          cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
          if (cVar7 != '\0') goto LAB_0015a244;
        }
        uVar38 = (**(code **)(*plVar5 + 0xa8))(plVar5,lVar42);
        *(undefined8 *)(param_1 + 6) = uVar38;
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015a244;
        uVar38 = (**(code **)(*plVar5 + 0xa8))(plVar5,lVar18);
        *(undefined8 *)(param_1 + 8) = uVar38;
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if ((((cVar7 != '\0') || (*(long *)(param_1 + 6) == 0)) || (*(long *)(param_1 + 8) == 0)) ||
           (iVar11 = pthread_create((pthread_t *)&local_78,(pthread_attr_t *)0x0,FUN_0015b33c,
                                    param_1), iVar11 != 0)) goto LAB_0015a244;
        iVar11 = 100;
        do {
          if (((int)*(undefined8 *)(param_1 + 10) != 0) ||
             ((int)*(undefined8 *)(param_1 + 0xb) != 0)) break;
          usleep(10000);
          iVar11 = iVar11 + -1;
        } while (iVar11 != 0);
        if ((int)*(undefined8 *)(param_1 + 10) == 0) {
LAB_0015b0a8:
          bVar43 = false;
          goto LAB_0015a244;
        }
        (**(code **)(*plVar5 + 0x468))(plVar5,lVar12,lVar20);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015b0a8;
        param_1[0xd] = 1;
        pthread_join((pthread_t)local_78,(void **)0x0);
        if ((int)*(undefined8 *)(param_1 + 0xc) == 0) goto LAB_0015b328;
        lVar12 = (**(code **)(*plVar5 + 0x390))(plVar5,DAT_0025c8d8,DAT_0025c8d0);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015a240;
        cVar7 = (**(code **)(*plVar5 + 0x128))(plVar5,lVar19,lVar37);
        cVar8 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar8 != '\0') goto LAB_0015a240;
        cVar8 = (**(code **)(*plVar5 + 0x128))(plVar5,lVar19,lVar15);
        cVar9 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar9 != '\0') goto LAB_0015a240;
        if (((lVar12 == 0) ||
            (cVar9 = (**(code **)(*plVar5 + 0xc0))(plVar5,lVar19,lVar12), cVar9 == '\0')) ||
           ((cVar7 != '\0' || (cVar8 != '\0')))) goto LAB_0015b328;
        lVar12 = (**(code **)(*plVar5 + 0x110))(plVar5,lVar14,lVar34);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015a240;
        if (lVar12 == 0) goto LAB_0015b328;
        lVar12 = (**(code **)(*plVar5 + 0x110))(plVar5,lVar12,lVar16);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015a240;
        if (lVar12 == 0) goto LAB_0015b328;
        iVar11 = (**(code **)(*plVar5 + 0x540))(plVar5,lVar12);
        cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
        if (cVar7 != '\0') goto LAB_0015a240;
        if (iVar11 - 1U < 0x60) {
          lVar13 = (**(code **)(*plVar5 + 0x548))(plVar5,lVar12,0);
          cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
          if (cVar7 != '\0') goto LAB_0015a240;
          if (lVar13 != 0) {
            iVar11 = FUN_00159bbc(*param_1,lVar13,iVar11,*(undefined8 *)(param_1 + 2),
                                  *(undefined8 *)(param_1 + 4));
            (**(code **)(*plVar5 + 0x550))(plVar5,lVar12,lVar13);
            if (iVar11 == 4) {
              puVar3 = &UNK_0013729c;
              if (*param_1 != 0x7f) {
                puVar3 = &DAT_0013b4da;
              }
              FUN_0015b620(puVar3);
              bVar6 = true;
              param_1[0xd] = 1;
              goto LAB_0015a2dc;
            }
          }
LAB_0015b328:
          bVar6 = true;
          param_1[0xd] = 1;
        }
        else {
          if (iVar11 < 0x61) goto LAB_0015b328;
          FUN_0015b620(&DAT_001311c6);
          bVar6 = true;
          param_1[0xd] = 1;
        }
      }
LAB_0015a2dc:
      cVar7 = (**(code **)(*plVar5 + 0x720))(plVar5);
      if (cVar7 != '\0') {
        (**(code **)(*plVar5 + 0x88))(plVar5);
      }
      if (*(long *)(param_1 + 6) != 0) {
        (**(code **)(*plVar5 + 0xb0))(plVar5);
      }
      if (*(long *)(param_1 + 8) != 0) {
        (**(code **)(*plVar5 + 0xb0))(plVar5);
      }
      if (bVar6) {
        (**(code **)(*plVar5 + 0xa0))(plVar5,0);
      }
      goto LAB_0015a1b0;
    }
  }
  param_1[0xd] = 1;
LAB_0015a1b0:
  free(param_1);
  DAT_0025c9d8._0_4_ = 0;
  if (iVar10 != 0) {
    (**(code **)(*DAT_0022f4c8 + 0x28))();
  }
  if (*(long *)(lVar4 + 0x28) == local_70) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0015b620 @ 0015b620 ===== */

void FUN_0015b620(undefined8 param_1)

{
  long lVar1;
  bool bVar2;
  long *plVar3;
  char cVar4;
  int iVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long *local_70;
  long local_68;
  
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  local_70 = (long *)0x0;
  if (DAT_0022f4c8 != (long *)0x0 && DAT_0022f4e8 != 0) {
    iVar5 = (**(code **)(*DAT_0022f4c8 + 0x30))(DAT_0022f4c8,&local_70,0x10006);
    if (iVar5 == 0) {
      bVar2 = true;
      plVar3 = local_70;
    }
    else {
      if ((iVar5 != -2) ||
         (iVar5 = (**(code **)(*DAT_0022f4c8 + 0x20))(DAT_0022f4c8,&local_70,0), iVar5 != 0))
      goto LAB_0015b724;
      bVar2 = false;
      plVar3 = local_70;
    }
    local_70 = plVar3;
    if (plVar3 != (long *)0x0) {
      iVar5 = (**(code **)(*plVar3 + 0x98))(plVar3,8);
      if (iVar5 == 0) {
        if (DAT_0025c8d8 == 0) {
          lVar6 = 0;
        }
        else {
          lVar6 = (**(code **)(*plVar3 + 0x390))(plVar3,DAT_0025c8d8,DAT_0025c8d0);
        }
        cVar4 = (**(code **)(*plVar3 + 0x720))(plVar3);
        if ((((((cVar4 == '\0') &&
               (lVar7 = (**(code **)(*plVar3 + 0x30))(plVar3,"android/widget/Toast"), lVar7 != 0))
              && (lVar8 = (**(code **)(*plVar3 + 0x388))
                                    (plVar3,lVar7,"makeText",
                                     "(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;"
                                    ), lVar8 != 0)) &&
             ((lVar9 = (**(code **)(*plVar3 + 0x108))(plVar3,lVar7,&DAT_00137c35,&DAT_0013ba46),
              lVar9 != 0 && (lVar10 = (**(code **)(*plVar3 + 0x538))(plVar3,param_1), lVar6 != 0))))
            && ((lVar10 != 0 &&
                ((cVar4 = (**(code **)(*plVar3 + 0x720))(plVar3), cVar4 == '\0' &&
                 (lVar6 = (**(code **)(*plVar3 + 0x390))(plVar3,lVar7,lVar8,lVar6,lVar10,0),
                 lVar6 != 0)))))) && (cVar4 = (**(code **)(*plVar3 + 0x720))(plVar3), cVar4 == '\0')
           ) {
          (**(code **)(*plVar3 + 0x1e8))(plVar3,lVar6,lVar9);
        }
        cVar4 = (**(code **)(*plVar3 + 0x720))(plVar3);
        if (cVar4 != '\0') {
          (**(code **)(*plVar3 + 0x88))(plVar3);
        }
        (**(code **)(*plVar3 + 0xa0))(plVar3,0);
      }
      else {
        cVar4 = (**(code **)(*plVar3 + 0x720))(plVar3);
        if (cVar4 != '\0') {
          (**(code **)(*plVar3 + 0x88))(plVar3);
        }
      }
      if (!bVar2) {
        (**(code **)(*DAT_0022f4c8 + 0x28))();
      }
    }
  }
LAB_0015b724:
  if (*(long *)(lVar1 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_0015b8f4 @ 0015b8f4 ===== */

void * FUN_0015b8f4(ulong param_1,int param_2,undefined4 param_3)

{
  ulong uVar1;
  undefined *puVar2;
  undefined4 uVar3;
  byte bVar4;
  uint uVar5;
  long lVar6;
  bool bVar7;
  undefined *puVar8;
  bool bVar9;
  int iVar10;
  int iVar11;
  undefined8 *puVar12;
  undefined4 *puVar13;
  FILE *__stream;
  int *piVar14;
  char *pcVar15;
  size_t sVar16;
  void *pvVar17;
  uint uVar18;
  ulong uVar19;
  ulong uVar20;
  ulong uVar21;
  ulong __n;
  ulong uVar22;
  long lVar23;
  ulong uVar24;
  long lVar25;
  undefined8 *puVar26;
  void *__dest;
  ulong uVar27;
  ulong local_2308;
  long local_2300;
  uint local_22f4;
  char *local_22f0;
  undefined4 local_229c;
  undefined1 auStack_2298 [4];
  undefined1 auStack_2294 [4];
  ulong local_2290;
  ulong uStack_2288;
  undefined8 local_2280 [64];
  uint local_2080;
  undefined8 local_2078;
  byte local_2070 [8192];
  long local_70;
  
  lVar6 = tpidr_el0;
  local_70 = *(long *)(lVar6 + 0x28);
  puVar12 = (undefined8 *)__emutls_get_address(&DAT_001a7c88);
  puVar2 = PTR_DAT_001a36c0;
  *puVar12 = &DAT_00132a0b;
  puVar8 = PTR_nexus_script_port_ui_server_thread_001a36c8;
  *(undefined4 *)(puVar12 + 1) = 0;
  __n = (long)puVar8 - (long)puVar2;
  if (DAT_0022f020 < __n) {
    __dest = (void *)0x0;
    goto LAB_0015bdbc;
  }
  uVar27 = -DAT_0022f020 & param_1;
  uVar5 = param_2 << 5 | 0x52800002;
  *puVar12 = &DAT_0013673d;
  uVar1 = param_1 + 4;
  uVar24 = (long)PTR_DAT_001a36d0 - (long)puVar2;
  uVar19 = (long)PTR_DAT_001a36d8 - (long)puVar2;
  uVar20 = (long)PTR_DAT_001a36e0 - (long)puVar2;
  uVar21 = (long)PTR_DAT_001a36e8 - (long)puVar2;
  uVar22 = 0x100000;
  do {
    pvVar17 = (void *)0x0;
    if (uVar22 <= uVar27) {
      pvVar17 = (void *)(uVar27 - uVar22);
    }
    if ((void *)(uVar27 + uVar22) != (void *)0x0) {
      __dest = mmap((void *)(uVar27 + uVar22),DAT_0022f020,3,0x22,-1,0);
      if (__dest == (void *)0xffffffffffffffff) {
        puVar13 = (undefined4 *)__errno();
        uVar3 = *puVar13;
        *puVar12 = &DAT_0013a39a;
        *(undefined4 *)(puVar12 + 1) = uVar3;
        goto joined_r0x0015bb20;
      }
      iVar10 = FUN_00190d68(param_1,__dest,local_2070);
      if (iVar10 == 0) {
        pcVar15 = "enter";
LAB_0015bb3c:
        *puVar12 = pcVar15;
        sVar16 = DAT_0022f020;
        *(undefined4 *)(puVar12 + 1) = 0;
LAB_0015bb48:
        munmap(__dest,sVar16);
        goto joined_r0x0015bb20;
      }
      iVar10 = FUN_00190d68((long)__dest + uVar24,uVar1,&local_2290);
      if (iVar10 == 0) {
        pcVar15 = "resume";
        goto LAB_0015bb3c;
      }
      memcpy(__dest,PTR_DAT_001a36c0,__n);
      *(uint *)((long)__dest + uVar19) = uVar5;
      *(undefined4 *)((long)__dest + uVar20) = param_3;
      *(undefined4 *)((long)__dest + uVar24) = (undefined4)local_2290;
      *(code **)((long)__dest + uVar21) = FUN_0015d224;
      FUN_00193944(__dest,(long)__dest + __n);
      iVar10 = mprotect(__dest,DAT_0022f020,5);
      if (iVar10 != 0) {
        puVar13 = (undefined4 *)__errno();
        uVar3 = *puVar13;
        *puVar12 = &DAT_0013715b;
        sVar16 = DAT_0022f020;
        *(undefined4 *)(puVar12 + 1) = uVar3;
        goto LAB_0015bb48;
      }
LAB_0015bda8:
      pcVar15 = "ready";
      goto LAB_0015bdb0;
    }
joined_r0x0015bb20:
    if (uVar22 < uVar27) {
      __dest = mmap(pvVar17,DAT_0022f020,3,0x22,-1,0);
      if (__dest == (void *)0xffffffffffffffff) {
        puVar13 = (undefined4 *)__errno();
        uVar3 = *puVar13;
        *puVar12 = &DAT_0013a39a;
        *(undefined4 *)(puVar12 + 1) = uVar3;
      }
      else {
        iVar10 = FUN_00190d68(param_1,__dest,local_2070);
        if (iVar10 == 0) {
          pcVar15 = "enter";
LAB_0015bc48:
          *puVar12 = pcVar15;
          sVar16 = DAT_0022f020;
          *(undefined4 *)(puVar12 + 1) = 0;
        }
        else {
          iVar10 = FUN_00190d68((long)__dest + uVar24,uVar1,&local_2290);
          if (iVar10 == 0) {
            pcVar15 = "resume";
            goto LAB_0015bc48;
          }
          memcpy(__dest,PTR_DAT_001a36c0,__n);
          *(uint *)((long)__dest + uVar19) = uVar5;
          *(undefined4 *)((long)__dest + uVar20) = param_3;
          *(undefined4 *)((long)__dest + uVar24) = (undefined4)local_2290;
          *(code **)((long)__dest + uVar21) = FUN_0015d224;
          FUN_00193944(__dest,(long)__dest + __n);
          iVar10 = mprotect(__dest,DAT_0022f020,5);
          if (iVar10 == 0) goto LAB_0015bda8;
          puVar13 = (undefined4 *)__errno();
          uVar3 = *puVar13;
          *puVar12 = &DAT_0013715b;
          sVar16 = DAT_0022f020;
          *(undefined4 *)(puVar12 + 1) = uVar3;
        }
        munmap(__dest,sVar16);
      }
    }
    bVar9 = uVar22 < 0x6f00001;
    uVar22 = uVar22 + 0x100000;
  } while (bVar9);
  memset(local_2280,0,0x208);
  if ((((DAT_0022f020 == 0x4000) || (DAT_0022f020 == 0x1000)) && (__n <= DAT_0022f020)) &&
     (uVar22 = __n - 4, bVar9 = (uVar24 & 3) == 0,
     ((((uVar20 <= uVar22 && uVar19 <= uVar22) && bVar9) && uVar24 < uVar22 ||
      ((uVar20 <= uVar22 && uVar19 <= uVar22) && bVar9) && uVar24 == uVar22) && 3 < uVar24) &&
     0xb < __n)) {
    if ((((param_1 & 3) == 0) && (uVar21 <= __n - 8 && DAT_0022d8e0 < 0xfffffffffec00000)) &&
       ((DAT_0022d8e0 <= param_1 && (param_1 - DAT_0022d8e0 >> 0x16 < 5)))) {
      uVar22 = param_1 - 0x8000000;
      if (uVar22 < 0x10001) {
        uVar22 = 0x10000;
      }
      puVar2 = PTR_DAT_001a36c0 + (0x8000004 - (long)PTR_DAT_001a36d0) + param_1;
      if (~(ulong)(PTR_DAT_001a36c0 + (0x8000004 - (long)PTR_DAT_001a36d0)) < param_1) {
        puVar2 = (undefined *)0xffffffffffffffff;
      }
      if ((undefined *)~DAT_0022f020 <= puVar2) {
        puVar2 = (undefined *)~DAT_0022f020;
      }
      lVar23 = uVar22 - 1;
      if (param_1 >> 0x1b == 0) {
        lVar23 = 0xffff;
      }
      uStack_2288 = (ulong)puVar2 & -DAT_0022f020;
      local_2290 = DAT_0022f020 + lVar23 & -DAT_0022f020;
      local_2078 = DAT_0022f020;
      if (local_2290 <= uStack_2288) {
        __stream = fopen("/proc/self/maps","r");
        if (__stream != (FILE *)0x0) {
LAB_0015be10:
          piVar14 = (int *)__errno();
          local_2308 = 0;
          local_2300 = 0;
          local_22f4 = 0;
          bVar9 = false;
LAB_0015be34:
          bVar7 = bVar9;
          *piVar14 = 0;
          pcVar15 = fgets((char *)local_2070,0x2000,__stream);
          if (pcVar15 != (char *)0x0) {
LAB_0015bff8:
            uVar22 = __strlen_chk(local_2070,0x2000);
            if ((uVar22 != 0) && (local_2070[uVar22 - 1] == 10)) {
              iVar10 = 0;
              local_22f4 = local_22f4 + 1;
              if (local_22f4 < 0x8001) {
                pcVar15 = "gap_bounds";
                if (uVar22 <= 0x800000U - local_2300) {
                  lVar23 = 0;
                  uVar27 = 0;
                  local_2300 = uVar22 + local_2300;
                  do {
                    bVar4 = local_2070[lVar23];
                    uVar18 = (uint)bVar4;
                    if (bVar4 - 0x30 < 10) {
                      iVar10 = -0x30;
                    }
                    else {
                      iVar10 = -0x57;
                      if (5 < bVar4 - 0x61) {
                        if (5 < uVar18 - 0x41) goto LAB_0015c0ac;
                        iVar10 = -0x37;
                      }
                    }
                    if (uVar27 >> 0x3c != 0) goto LAB_0015c244;
                    lVar23 = lVar23 + 1;
                    uVar27 = (ulong)(iVar10 + uVar18) + uVar27 * 0x10;
                  } while( true );
                }
                goto LAB_0015c248;
              }
            }
            iVar10 = 0;
            pcVar15 = "gap_bounds";
            goto LAB_0015c248;
          }
          iVar10 = *piVar14;
          iVar11 = ferror(__stream);
          if ((iVar11 == 0) || (iVar10 != 4)) {
LAB_0015c3c4:
            if (iVar11 == 0) {
              iVar10 = feof(__stream);
              if (iVar10 != 0) {
                iVar10 = 0;
                pcVar15 = "gap_maps";
                if ((local_22f4 == 0) || (!bVar7)) goto LAB_0015c248;
                FUN_00168b00(&local_2290,param_1,local_2308,0xffffffffffffffff);
                iVar10 = fclose(__stream);
                if (iVar10 != 0) {
                  iVar10 = *piVar14;
                  pcVar15 = "gap_close";
                  goto LAB_0015c460;
                }
                *(undefined4 *)(puVar12 + 1) = 0;
                *puVar12 = "gap_empty";
                uVar22 = (ulong)local_2080;
                if (local_2080 == 0) {
                  __dest = (void *)0x0;
                  goto LAB_0015bdbc;
                }
                puVar26 = local_2280;
                local_22f0 = "gap_range";
                goto LAB_0015c4c8;
              }
              iVar10 = 0;
            }
          }
          else {
            clearerr(__stream);
            *piVar14 = 0;
            pcVar15 = fgets((char *)local_2070,0x2000,__stream);
            if (pcVar15 != (char *)0x0) goto LAB_0015bff8;
            iVar10 = *piVar14;
            iVar11 = ferror(__stream);
            if ((iVar11 == 0) || (iVar10 != 4)) goto LAB_0015c3c4;
            clearerr(__stream);
            *piVar14 = 0;
            pcVar15 = fgets((char *)local_2070,0x2000,__stream);
            if (pcVar15 != (char *)0x0) goto LAB_0015bff8;
            iVar10 = *piVar14;
            iVar11 = ferror(__stream);
            if ((iVar11 == 0) || (iVar10 != 4)) goto LAB_0015c3c4;
            clearerr(__stream);
            *piVar14 = 0;
            pcVar15 = fgets((char *)local_2070,0x2000,__stream);
            if (pcVar15 != (char *)0x0) goto LAB_0015bff8;
            iVar10 = *piVar14;
            iVar11 = ferror(__stream);
            if ((iVar11 == 0) || (iVar10 != 4)) goto LAB_0015c3c4;
            clearerr(__stream);
            *piVar14 = 0;
            pcVar15 = fgets((char *)local_2070,0x2000,__stream);
            if (pcVar15 != (char *)0x0) goto LAB_0015bff8;
            iVar10 = *piVar14;
            iVar11 = ferror(__stream);
            if ((iVar11 == 0) || (iVar10 != 4)) goto LAB_0015c3c4;
            clearerr(__stream);
            *piVar14 = 0;
            pcVar15 = fgets((char *)local_2070,0x2000,__stream);
            if (pcVar15 != (char *)0x0) goto LAB_0015bff8;
            iVar10 = *piVar14;
            iVar11 = ferror(__stream);
            if ((iVar11 == 0) || (iVar10 != 4)) goto LAB_0015c3c4;
            clearerr(__stream);
            *piVar14 = 0;
            pcVar15 = fgets((char *)local_2070,0x2000,__stream);
            if (pcVar15 != (char *)0x0) goto LAB_0015bff8;
            iVar10 = *piVar14;
            iVar11 = ferror(__stream);
            if ((iVar11 == 0) || (iVar10 != 4)) goto LAB_0015c3c4;
            clearerr(__stream);
            *piVar14 = 0;
            pcVar15 = fgets((char *)local_2070,0x2000,__stream);
            if (pcVar15 != (char *)0x0) goto LAB_0015bff8;
            iVar10 = *piVar14;
            iVar11 = ferror(__stream);
            if ((iVar11 == 0) || (iVar10 != 4)) goto LAB_0015c3c4;
          }
          pcVar15 = "gap_read";
          goto LAB_0015c248;
        }
        piVar14 = (int *)__errno();
        iVar10 = *piVar14;
        if (iVar10 == 4) {
          __stream = fopen("/proc/self/maps","r");
          if (__stream != (FILE *)0x0) goto LAB_0015be10;
          piVar14 = (int *)__errno();
          iVar10 = *piVar14;
          if (iVar10 == 4) {
            __stream = fopen("/proc/self/maps","r");
            if (__stream != (FILE *)0x0) goto LAB_0015be10;
            piVar14 = (int *)__errno();
            iVar10 = *piVar14;
            if (iVar10 == 4) {
              __stream = fopen("/proc/self/maps","r");
              if (__stream != (FILE *)0x0) goto LAB_0015be10;
              piVar14 = (int *)__errno();
              iVar10 = *piVar14;
              if (iVar10 == 4) {
                __stream = fopen("/proc/self/maps","r");
                if (__stream != (FILE *)0x0) goto LAB_0015be10;
                piVar14 = (int *)__errno();
                iVar10 = *piVar14;
                if (iVar10 == 4) {
                  __stream = fopen("/proc/self/maps","r");
                  if (__stream != (FILE *)0x0) goto LAB_0015be10;
                  piVar14 = (int *)__errno();
                  iVar10 = *piVar14;
                  if (iVar10 == 4) {
                    __stream = fopen("/proc/self/maps","r");
                    if (__stream != (FILE *)0x0) goto LAB_0015be10;
                    piVar14 = (int *)__errno();
                    iVar10 = *piVar14;
                    if (iVar10 == 4) {
                      __stream = fopen("/proc/self/maps","r");
                      if (__stream != (FILE *)0x0) goto LAB_0015be10;
                      piVar14 = (int *)__errno();
                      iVar10 = *piVar14;
                    }
                  }
                }
              }
            }
          }
        }
        pcVar15 = "gap_open";
        goto LAB_0015c460;
      }
    }
  }
  pcVar15 = "gap_window";
  __dest = (void *)0x0;
LAB_0015bdb0:
  *(undefined4 *)(puVar12 + 1) = 0;
  *puVar12 = pcVar15;
  goto LAB_0015bdbc;
LAB_0015c0ac:
  iVar10 = 0;
  if ((int)lVar23 != 0) {
    pcVar15 = "gap_parse";
    if (uVar18 == 0x2d) {
      lVar25 = 0;
      uVar22 = 0;
      do {
        bVar4 = local_2070[lVar25 + lVar23 + 1];
        uVar18 = (uint)bVar4;
        if (bVar4 - 0x30 < 10) {
          iVar10 = -0x30;
        }
        else {
          iVar10 = -0x57;
          if (5 < bVar4 - 0x61) {
            if (5 < uVar18 - 0x41) goto LAB_0015c120;
            iVar10 = -0x37;
          }
        }
        if (uVar22 >> 0x3c != 0) goto LAB_0015c244;
        lVar25 = lVar25 + 1;
        uVar22 = (ulong)(iVar10 + uVar18) + uVar22 * 0x10;
      } while( true );
    }
    goto LAB_0015c248;
  }
LAB_0015c244:
  iVar10 = 0;
  pcVar15 = "gap_parse";
  goto LAB_0015c248;
  while( true ) {
    iVar10 = FUN_00190d68((long)__dest + uVar24,uVar1,auStack_2294);
    if (iVar10 == 0) {
      iVar10 = 0;
      __dest = (void *)0x0;
      goto LAB_0015c68c;
    }
    pvVar17 = mmap(__dest,DAT_0022f020,3,0x100022,-1,0);
    if (pvVar17 == (void *)0xffffffffffffffff) {
      *(int *)(puVar12 + 1) = *piVar14;
      *puVar12 = "gap_mmap";
    }
    else {
      if (__dest == pvVar17) {
        iVar10 = FUN_00190d68(param_1,__dest,auStack_2298);
        if (iVar10 == 0) {
          iVar10 = 0;
          pcVar15 = "enter";
        }
        else {
          iVar10 = FUN_00190d68((long)__dest + uVar24,uVar1,&local_229c);
          if (iVar10 == 0) {
            iVar10 = 0;
            pcVar15 = "resume";
          }
          else {
            memcpy(pvVar17,PTR_DAT_001a36c0,__n);
            *(uint *)((long)pvVar17 + uVar19) = uVar5;
            *(undefined4 *)((long)pvVar17 + uVar20) = param_3;
            *(undefined4 *)((long)pvVar17 + uVar24) = local_229c;
            *(code **)((long)pvVar17 + uVar21) = FUN_0015d224;
            FUN_00193944(pvVar17,(long)pvVar17 + __n);
            iVar10 = mprotect(pvVar17,DAT_0022f020,5);
            if (iVar10 == 0) {
              iVar10 = 0;
              local_22f0 = "ready";
              goto LAB_0015c68c;
            }
            iVar10 = *piVar14;
            pcVar15 = "seal";
          }
        }
      }
      else {
        iVar10 = 0;
        pcVar15 = "gap_return";
      }
      *puVar12 = pcVar15;
      sVar16 = DAT_0022f020;
      *(int *)(puVar12 + 1) = iVar10;
      iVar10 = munmap(pvVar17,sVar16);
      if (iVar10 != 0) {
        __dest = (void *)0x0;
        iVar10 = *piVar14;
        local_22f0 = "gap_unmap";
        goto LAB_0015c68c;
      }
    }
    __dest = (void *)0x0;
    uVar22 = uVar22 - 1;
    puVar26 = puVar26 + 1;
    if (uVar22 == 0) break;
LAB_0015c4c8:
    __dest = (void *)*puVar26;
    if ((((__dest < (void *)0x10000) ||
         (((DAT_0022f020 - 1 & (ulong)__dest) != 0 || 0xfffffffffffffffb < param_1) ||
          (void *)~DAT_0022f020 < __dest)) ||
        ((__dest < (void *)(DAT_0022d8e0 + 0x1400000) &&
         (DAT_0022d8e0 < DAT_0022f020 + (long)__dest)))) ||
       (iVar10 = FUN_00190d68(param_1,__dest,local_2070), iVar10 == 0)) {
      iVar10 = 0;
      __dest = (void *)0x0;
      local_22f0 = "gap_range";
LAB_0015c68c:
      *puVar12 = local_22f0;
      goto LAB_0015c468;
    }
  }
  goto LAB_0015bdbc;
LAB_0015c120:
  if ((int)lVar25 == 0) goto LAB_0015c244;
  if (uVar18 == 0x20) {
    sVar16 = strlen((char *)(local_2070 + lVar23 + lVar25 + 2));
    if ((sVar16 < 5) ||
       ((bVar4 = local_2070[lVar23 + lVar25 + 2], bVar4 != 0x72 && (bVar4 != 0x2d))))
    goto LAB_0015c430;
    if (((local_2070[lVar23 + lVar25 + 3] == 0x77) || (local_2070[lVar23 + lVar25 + 3] == 0x2d)) &&
       ((local_2070[lVar23 + lVar25 + 4] == 0x78 || (local_2070[lVar23 + lVar25 + 4] == 0x2d)))) {
      if (((local_2070[lVar23 + lVar25 + 5] == 0x73) || (local_2070[lVar23 + lVar25 + 5] == 0x70))
         && (local_2070[lVar23 + lVar25 + 6] == 0x20)) {
        iVar10 = 0;
        pcVar15 = "gap_parse";
        if ((uVar22 <= uVar27) || (uVar27 < local_2308)) goto LAB_0015c248;
        if ((DAT_0022f020 - 1 & (uVar22 | uVar27)) != 0) goto LAB_0015c430;
        FUN_00168b00(&local_2290,param_1,local_2308,uVar27);
        bVar9 = bVar7;
        if (uVar1 <= uVar22) {
          bVar9 = true;
        }
        local_2308 = uVar22;
        if (param_1 < uVar27 || 0xfffffffffffffffb < param_1) {
          bVar9 = bVar7;
        }
        goto LAB_0015be34;
      }
    }
    iVar10 = 0;
    pcVar15 = "gap_parse";
    goto LAB_0015c248;
  }
LAB_0015c430:
  iVar10 = 0;
  pcVar15 = "gap_parse";
LAB_0015c248:
  fclose(__stream);
LAB_0015c460:
  __dest = (void *)0x0;
  *puVar12 = pcVar15;
LAB_0015c468:
  *(int *)(puVar12 + 1) = iVar10;
LAB_0015bdbc:
  if (*(long *)(lVar6 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return __dest;
}

/* ===== FUN_0015cd20 @ 0015cd20 ===== */

void FUN_0015cd20(ulong param_1,uint *param_2)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  long lVar7;
  long lVar8;
  char *pcVar9;
  undefined1 auStack_68 [8];
  undefined1 auStack_60 [8];
  long local_58;
  
  lVar3 = tpidr_el0;
  local_58 = *(long *)(lVar3 + 0x28);
  lVar7 = *(long *)(param_2 + 2);
  if ((((0xffffffffff88d78f < param_1) || (lVar8 = 0x772870, param_1 + 0x772870 != lVar7)) &&
      ((0xffffffffffa6de5f < param_1 || (lVar8 = 0x5921a0, param_1 + 0x5921a0 != lVar7)))) &&
     (((0xffffffffff777c93 < param_1 || (lVar8 = 0x88836c, param_1 + 0x88836c != lVar7)) &&
      (lVar8 = 0x678fe4, param_1 + 0x678fe4 != lVar7 || 0xffffffffff98701b < param_1)))) {
    lVar8 = 0;
  }
  pcVar9 = "unknown";
  if (*(char **)(param_2 + 4) != (char *)0x0) {
    pcVar9 = *(char **)(param_2 + 4);
  }
  iVar4 = strcmp(pcVar9,"write");
  if (iVar4 == 0) {
    iVar4 = 0x77;
  }
  else {
    iVar4 = strcmp(pcVar9,"read");
    if (iVar4 == 0) {
      iVar4 = 0x72;
    }
    else {
      iVar5 = strcmp(pcVar9,"mismatch");
      iVar4 = 0x6d;
      if (iVar5 != 0) {
        iVar4 = 0x75;
      }
    }
  }
  pcVar9 = *(char **)(param_2 + 8);
  if (pcVar9 == (char *)0x0) {
    uVar6 = 0x75;
  }
  else {
    iVar5 = strcmp(pcVar9,"proc");
    if (iVar5 == 0) {
      uVar6 = 0x70;
    }
    else {
      iVar5 = strcmp(pcVar9,"vm");
      uVar6 = 0x75;
      if (iVar5 == 0) {
        uVar6 = 0x76;
      }
    }
  }
  pcVar9 = "write";
  if (iVar4 != 0x77) {
    pcVar9 = "read";
  }
  if (((lVar8 == 0) || (3 < *param_2)) ||
     (((iVar4 == 0x75 ||
       ((*(char **)(param_2 + 10) == (char *)0x0 ||
        (iVar5 = strcmp(*(char **)(param_2 + 10),pcVar9), iVar5 != 0)))) ||
      (*(long *)(param_2 + 0xe) != 4)))) {
    if (*(long *)(lVar3 + 0x28) == local_58) {
      FUN_0015540c(0x25c9dc,0x37,0x37,"pb_%lx_%c_context_unknown",lVar8,iVar4);
      return;
    }
    goto LAB_0015d074;
  }
  if (iVar4 == 0x6d) {
    if (*(long *)(lVar3 + 0x28) == local_58) {
      FUN_0015540c(0x25c9dc,0x37,0x37,"pb_%lx_m_%c_g%08x",lVar8,uVar6,param_2[6]);
      return;
    }
    goto LAB_0015d074;
  }
  pcVar9 = (char *)FUN_00168d18(*(undefined8 *)(param_2 + 0xc));
  FUN_00168dfc(auStack_60,*(undefined8 *)(param_2 + 0x10));
  iVar5 = strcmp(pcVar9,"alloc");
  if ((iVar5 == 0) || (iVar5 = strcmp(pcVar9,"remap"), iVar5 == 0)) {
    lVar7 = *(long *)(param_2 + 0x12);
    if (lVar7 < 0) goto LAB_0015cfe8;
    FUN_0015540c(auStack_68,8,8,"other");
  }
  else {
    lVar7 = *(long *)(param_2 + 0x12);
LAB_0015cfe8:
    FUN_00168dfc(auStack_68,lVar7);
  }
  uVar1 = param_2[0x14];
  uVar2 = param_2[0x15];
  if (0xffe < uVar1 - 1) {
    uVar1 = 0;
  }
  if (0xffe < uVar2 - 1) {
    uVar2 = 0;
  }
  FUN_0015540c(0x25c9dc,0x37,0x37,"pb_%lx_%c_%c_%s_e%u_%s_%s_e%u",lVar8,iVar4,uVar6,auStack_60,uVar1
               ,pcVar9,auStack_68,uVar2);
  if (*(long *)(lVar3 + 0x28) == local_58) {
    return;
  }
LAB_0015d074:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0015f334 @ 0015f334 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0015f334(uint param_1,ulong param_2,undefined8 param_3,ulong param_4)

{
  long lVar1;
  bool bVar2;
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  undefined1 auVar6 [16];
  code *pcVar7;
  bool bVar8;
  bool bVar9;
  bool bVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  __pid_t _Var14;
  pthread_t __target_thread;
  undefined8 uVar15;
  undefined8 uVar16;
  char *pcVar17;
  long lVar18;
  uint uVar19;
  ulong uVar20;
  long lVar21;
  undefined1 auVar22 [16];
  ulong local_1d0;
  undefined8 local_1c8;
  code *local_1c0;
  long local_1b8;
  long lStack_1b0;
  long local_1a8;
  ulong local_1a0;
  undefined8 local_198;
  ulong local_190;
  long local_188;
  int local_180;
  int local_17c;
  int iStack_178;
  int local_174;
  ulong local_168;
  long lStack_160;
  ulong local_158;
  long lStack_150;
  int local_148;
  int iStack_144;
  int iStack_140;
  int iStack_13c;
  undefined4 local_138;
  uint uStack_134;
  undefined8 local_130;
  undefined8 local_128;
  long local_120;
  long local_118;
  timespec local_110;
  undefined8 local_100;
  long local_f8;
  long lStack_f0;
  code *local_e8;
  int local_d0;
  long local_70;
  
  iVar13 = DAT_0022f038;
  lVar1 = tpidr_el0;
  local_70 = *(long *)(lVar1 + 0x28);
  if (DAT_0022d8d8 != '\x01') goto LAB_0015f3b4;
  local_110.tv_sec = 0;
  local_110.tv_nsec = 0;
  if ((DAT_0022f038 < 1) || (iVar11 = gettid(), iVar11 != iVar13)) goto LAB_0015f3b4;
  __target_thread = pthread_self();
  iVar11 = pthread_getname_np(__target_thread,(char *)&local_110,0x10);
  if ((iVar11 != 0) ||
     ((local_110.tv_sec != 0x706f6f6c6e69614d || (char)local_110.tv_nsec != '\0' ||
      (DAT_0022f03c < 1)))) goto LAB_0015f3b4;
  uVar15 = FUN_0014ef90(&local_168,param_4);
  if (DAT_0022da7c._4_4_ == 0) {
    if ((local_148 == 0) || (iStack_144 == 0 && iStack_140 == 0)) {
      DAT_0022da58 = 0;
      DAT_0022da50 = 0;
      DAT_0022da6c = 0;
      DAT_0022da64 = 0;
      uRam000000000022d9e0 = 0;
      DAT_0022d9d8 = 0;
      DAT_0022d9f0 = 0;
      DAT_0022d9e8 = 0;
      uRam000000000022da00 = 0;
      _DAT_0022d9f8 = 0;
      uRam000000000022da10 = 0;
      DAT_0022da08 = 0;
      DAT_0022da18 = 0;
      DAT_0022d930 = 0;
      DAT_0022d928 = 0;
      uRam000000000022d940 = 0;
      _DAT_0022d938 = 0;
      uRam000000000022d950 = 0;
      _DAT_0022d948 = 0;
      uRam000000000022d960 = 0;
      DAT_0022d958 = 0;
      DAT_0022d970 = 0;
      _DAT_0022d968 = 0;
      uRam000000000022d980 = 0;
      DAT_0022d978 = 0;
      uRam000000000022d990 = 0;
      _DAT_0022d988 = 0;
      uRam000000000022d9a0 = 0;
      _DAT_0022d998 = 0;
      uRam000000000022d9b0 = 0;
      _DAT_0022d9a8 = 0;
      uRam000000000022d9c0 = 0;
      _DAT_0022d9b8 = 0;
      DAT_0022d9c8 = 0;
      DAT_0022dad4 = 0;
      DAT_0022da7c._0_4_ = 0;
      DAT_0022da7c._4_4_ = 0;
      DAT_0022da74._0_4_ = 0;
      DAT_0022da74._4_4_ = 0;
      uRam000000000022da8c = 0;
      uRam000000000022da90 = 0;
      DAT_0022da84 = 0;
      _DAT_0022da88 = 0;
      uRam000000000022da9c = 0;
      uRam000000000022daa0 = 0;
      _DAT_0022da94 = 0;
      _DAT_0022da98 = 0;
      uRam000000000022daac = 0;
      uRam000000000022dab0 = 0;
      _DAT_0022daa4 = 0;
      _DAT_0022daa8 = 0;
      uRam000000000022dabc = 0;
      uRam000000000022dac0 = 0;
      _DAT_0022dab4 = 0;
      _DAT_0022dab8 = 0;
      uRam000000000022dacc = 0;
      uRam000000000022dad0 = 0;
      _DAT_0022dac4 = 0;
      DAT_0022dac8 = 0;
    }
    else {
      if ((((DAT_0022da84 == 0) || (local_168 < DAT_0022d8f0)) || (local_158 != DAT_0022d900)) ||
         (lStack_160 != DAT_0022d8f8)) {
        DAT_0022da58 = 0;
        DAT_0022da7c._0_4_ = 0;
        DAT_0022da7c._4_4_ = 0;
        DAT_0022da50 = 0;
        uRam000000000022da90 = 0;
        _DAT_0022da94 = 0;
        _DAT_0022da88 = 0;
        uRam000000000022da8c = 0;
        DAT_0022d930 = 0;
        DAT_0022d928 = 0;
        uRam000000000022d940 = 0;
        _DAT_0022d938 = 0;
        uRam000000000022d950 = 0;
        _DAT_0022d948 = 0;
        uRam000000000022d960 = 0;
        DAT_0022d958 = 0;
        DAT_0022d970 = 0;
        _DAT_0022d968 = 0;
        uRam000000000022d980 = 0;
        DAT_0022d978 = 0;
        uRam000000000022d990 = 0;
        _DAT_0022d988 = 0;
        uRam000000000022d9a0 = 0;
        _DAT_0022d998 = 0;
        uRam000000000022d9b0 = 0;
        _DAT_0022d9a8 = 0;
        uRam000000000022d9c0 = 0;
        _DAT_0022d9b8 = 0;
        uRam000000000022d9e0 = 0;
        DAT_0022d9d8 = 0;
        DAT_0022d9f0 = 0;
        DAT_0022d9e8 = 0;
        uRam000000000022da00 = 0;
        _DAT_0022d9f8 = 0;
        uRam000000000022da10 = 0;
        DAT_0022da08 = 0;
        uRam000000000022daa0 = 0;
        _DAT_0022daa4 = 0;
        _DAT_0022da98 = 0;
        uRam000000000022da9c = 0;
        uRam000000000022dab0 = 0;
        _DAT_0022dab4 = 0;
        _DAT_0022daa8 = 0;
        uRam000000000022daac = 0;
        uRam000000000022dac0 = 0;
        _DAT_0022dac4 = 0;
        _DAT_0022dab8 = 0;
        uRam000000000022dabc = 0;
        uRam000000000022dad0 = 0;
        DAT_0022dad4 = 0;
        DAT_0022dac8 = 0;
        uRam000000000022dacc = 0;
        DAT_0022da6c = 0;
        DAT_0022da64 = 0;
        DAT_0022da18 = 0;
        DAT_0022da74._0_4_ = 0;
        DAT_0022da74._4_4_ = 0;
        DAT_0022d9c8 = 0;
      }
      DAT_0022da84 = 1;
      if ((lStack_150 != 0) && (lStack_160 != 0)) {
        DAT_0022d9d0 = local_168;
        DAT_0022d9c8 = lStack_160;
      }
    }
    uRam000000000022d918 = CONCAT44(iStack_13c,iStack_140);
    _DAT_0022d910 = CONCAT44(iStack_144,local_148);
    DAT_0022d920 = CONCAT44(uStack_134,local_138);
    DAT_0022d8f8 = lStack_160;
    DAT_0022d8f0 = local_168;
    lRam000000000022d908 = lStack_150;
    DAT_0022d900 = local_158;
  }
  if (param_1 == 3) {
    lVar21 = 0;
    do {
      uVar20 = (&DAT_00281918)[lVar21 * 4];
      if (uVar20 != 0) {
        local_110.tv_sec = 0;
        uVar16 = FUN_0014e7c4(uVar15,uVar20,&local_110,8);
        bVar9 = local_110.tv_sec == DAT_0022d8e0 + 0x11c7b88;
        uVar15 = FUN_0014e7c4(uVar16,uVar20 + 0x248,&local_110,8);
        if ((((int)uVar15 == 0) || (local_110.tv_sec == (&DAT_00281920)[lVar21 * 4])) &&
           ((int)uVar16 == 0 || bVar9)) {
          iVar11 = 0x20;
          do {
            if (((uVar20 == local_158) || (uVar20 == (&DAT_00281928)[lVar21 * 4])) ||
               (uVar15 = FUN_0014e7c4(uVar15,uVar20 + 0x38,&local_110,8), (int)uVar15 == 0)) break;
            if (local_110.tv_sec == 0) goto LAB_0015f4e8;
            iVar11 = iVar11 + -1;
            uVar20 = local_110.tv_sec;
          } while (iVar11 != 0);
        }
        else {
LAB_0015f4e8:
          (&DAT_00281920)[lVar21 * 4] = 0;
          (&DAT_00281918)[lVar21 * 4] = 0;
          (&DAT_00281930)[lVar21 * 4] = 0;
          (&DAT_00281928)[lVar21 * 4] = 0;
        }
      }
      lVar21 = lVar21 + 1;
    } while (lVar21 != 8);
  }
  uVar20 = DAT_0022db18;
  local_110.tv_sec = 0;
  if (DAT_0022db18 != 0) {
    uVar15 = FUN_0014e7c4(uVar15,DAT_0022db18,&local_110,8);
    bVar9 = local_110.tv_sec == DAT_0022d8e0 + 0x11c7b88;
    uVar16 = FUN_0014e7c4(uVar15,uVar20 + 0x248,&local_110,8);
    if ((local_110.tv_sec == DAT_0022db28 || (int)uVar16 == 0) && ((int)uVar15 == 0 || bVar9)) {
      iVar11 = 0x20;
      do {
        if ((uVar20 == local_158 || uVar20 == DAT_0022db20) ||
           (uVar16 = FUN_0014e7c4(uVar16,uVar20 + 0x38,&local_110,8), (int)uVar16 == 0)) break;
        if (local_110.tv_sec == 0) goto LAB_0015f664;
        iVar11 = iVar11 + -1;
        uVar20 = local_110.tv_sec;
      } while (iVar11 != 0);
    }
    else {
LAB_0015f664:
      DAT_0022db38 = 0;
      DAT_0022db20 = 0;
      DAT_0022db18 = 0;
      DAT_0022db30 = 0;
      DAT_0022db28 = 0;
    }
  }
  if (DAT_0022f1c0 == 0) {
    bVar9 = true;
  }
  else if ((DAT_0022f1b0 == local_158) && (DAT_0022f1b8 == lStack_160)) {
    bVar9 = false;
  }
  else {
    bVar9 = true;
    DAT_0022f1b8 = 0;
    DAT_0022f1b0 = 0;
    DAT_0022f1c8 = 0;
    DAT_0022f1c0 = 0;
  }
  if (iStack_144 != 0) {
    local_190 = 0;
    local_188 = 0;
    local_198 = 0;
    local_1a0 = DAT_0010f720;
    if (((((local_148 != 0) && (lStack_150 != 0)) &&
         ((bVar9 || ((DAT_0022f1b0 != local_158 || (DAT_0022f1b8 != lStack_160)))))) &&
        (iVar11 = FUN_00192138(&local_1a0), iVar11 != 0)) &&
       ((((local_190 != 0 && (local_188 != 0)) && ((int)local_198 == 0)) && (local_198._4_4_ != 0)))
       ) {
      auVar22._8_8_ = local_158;
      auVar22._0_8_ = lStack_160;
      auVar22 = NEON_ext(auVar22,auVar22,8,1);
      uRam000000000022da90 = 0;
      _DAT_0022da94 = 0;
      _DAT_0022da88 = 0;
      uRam000000000022da8c = 0;
      uRam000000000022daa0 = 0;
      _DAT_0022daa4 = 0;
      _DAT_0022da98 = 0;
      uRam000000000022da9c = 0;
      uRam000000000022dab0 = 0;
      _DAT_0022dab4 = 0;
      _DAT_0022daa8 = 0;
      uRam000000000022daac = 0;
      uRam000000000022dac0 = 0;
      _DAT_0022dac4 = 0;
      _DAT_0022dab8 = 0;
      uRam000000000022dabc = 0;
      uRam000000000022dad0 = 0;
      DAT_0022dad4 = 0;
      DAT_0022dac8 = 0;
      uRam000000000022dacc = 0;
      DAT_0022f1b8 = auVar22._8_8_;
      DAT_0022f1b0 = auVar22._0_8_;
      DAT_0022f1c0 = local_188;
      DAT_0022f1c8 = local_168;
      DAT_0022dad8 = 0;
      DAT_0022da7c._4_4_ = 0;
      DAT_0022da74._4_4_ = 1;
      DAT_0022da7c._0_4_ = 1;
      DAT_0022d9f0 = local_168;
    }
  }
  if (((DAT_0022da74._4_4_ != 0) && ((int)DAT_0022da7c == 1)) &&
     ((DAT_0022d900 == local_158 && (DAT_0022d8f8 == lStack_160)))) {
    local_100 = (code *)0x0;
    local_f8 = 0;
    local_110.tv_nsec = 0;
    local_110.tv_sec = DAT_0010f720;
    if (((local_148 != 0) && (lStack_150 != 0)) &&
       (((DAT_0022f1c0 == 0 || ((DAT_0022f1b0 != DAT_0022d900 || (DAT_0022f1b8 != DAT_0022d8f8))))
        && ((iVar11 = FUN_00192138(&local_110), iVar11 != 0 &&
            ((((local_100 != (code *)0x0 && (local_f8 != 0)) && ((int)local_110.tv_nsec != 0)) &&
             (iVar11 = FUN_00192178(local_100,local_f8,1), iVar11 != 0)))))))) {
      auVar3._8_8_ = local_158;
      auVar3._0_8_ = lStack_160;
      auVar22 = NEON_ext(auVar3,auVar3,8,1);
      DAT_0022f1c0 = local_f8;
      DAT_0022f1c8 = local_168;
      DAT_0022f1b8 = auVar22._8_8_;
      DAT_0022f1b0 = auVar22._0_8_;
    }
  }
  if (4999 < uStack_134) {
    uStack_134 = 5000;
  }
  if (uStack_134 < 0x1f5) {
    uStack_134 = 500;
  }
  if ((DAT_0022d9f0 == 0) || (local_168 < DAT_0022d9f0)) {
    bVar9 = true;
  }
  else {
    bVar9 = (ulong)uStack_134 <= local_168 - DAT_0022d9f0;
  }
  if (iStack_13c == 0) {
    if (iStack_144 == 0) {
      bVar10 = false;
      if (iStack_140 != 0) {
        bVar10 = bVar9;
      }
      if (bVar10) goto LAB_0015fb80;
    }
    else if (bVar9) {
LAB_0015fb80:
      local_1c0 = (code *)0x0;
      local_1b8 = 0;
      local_1c8 = 0;
      local_1d0 = DAT_0010f720;
      if (((((local_148 != 0) && (lStack_150 != 0)) &&
           ((DAT_0022f1c0 == 0 || ((DAT_0022f1b0 != local_158 || (DAT_0022f1b8 != lStack_160))))))
          && ((iVar11 = FUN_00192138(&local_1d0), iVar11 != 0 &&
              (((local_1c0 != (code *)0x0 && (local_1b8 != 0)) && ((int)local_1c8 != 0)))))) &&
         ((local_1c8._4_4_ != 0 && (iVar11 = FUN_00192178(local_1c0,local_1b8,1), iVar11 != 0)))) {
        DAT_0022d9f0 = local_168;
        local_110.tv_sec = 0;
        local_110.tv_nsec = DAT_0022d8e0;
        local_e8 = (code *)(DAT_0022d8e0 + 0xac319c);
        local_100 = FUN_0014e7c4;
        local_f8 = DAT_0022d8e0 + _DAT_0010fa30;
        lStack_f0 = DAT_0022d8e0 + _UNK_0010fa38;
        iVar11 = FUN_00165768(&local_110);
        if (iVar11 != 0) {
          auVar5._8_8_ = local_158;
          auVar5._0_8_ = lStack_160;
          DAT_0022f1c0 = local_1b8;
          auVar22 = NEON_ext(auVar5,auVar5,8,1);
          DAT_0022f1c8 = local_168;
          uRam000000000022da90 = 0;
          _DAT_0022da94 = 0;
          _DAT_0022da88 = 0;
          uRam000000000022da8c = 0;
          uRam000000000022daa0 = 0;
          _DAT_0022daa4 = 0;
          _DAT_0022da98 = 0;
          uRam000000000022da9c = 0;
          uRam000000000022dab0 = 0;
          _DAT_0022dab4 = 0;
          _DAT_0022daa8 = 0;
          uRam000000000022daac = 0;
          uRam000000000022dac0 = 0;
          _DAT_0022dac4 = 0;
          _DAT_0022dab8 = 0;
          uRam000000000022dabc = 0;
          uRam000000000022dad0 = 0;
          DAT_0022dad4 = 0;
          DAT_0022dac8 = 0;
          uRam000000000022dacc = 0;
          DAT_0022f1b8 = auVar22._8_8_;
          DAT_0022f1b0 = auVar22._0_8_;
          DAT_0022dad8 = 0;
          DAT_0022da7c._4_4_ = 0;
          DAT_0022da74._4_4_ = 1;
          DAT_0022da7c._0_4_ = 1;
          goto LAB_0015f3b4;
        }
        FUN_00192178(local_1c0,local_1b8,0);
      }
    }
  }
  if (((param_1 == 1) && (local_148 != 0)) &&
     ((DAT_0022db18 != 0 &&
      (((local_158 == DAT_0022db20 && (lStack_160 == DAT_0022db30)) &&
       (uVar15 = FUN_00154eb4(), (int)uVar15 != 0)))))) {
    local_110.tv_sec = 0;
    iVar11 = FUN_0014e7c4(uVar15,DAT_0022db18 + 0x248,&local_110,8);
    uVar20 = local_110.tv_sec;
    if (((local_110.tv_sec & 7U) != 0 || (ulong)local_110.tv_sec < 0x1000) || iVar11 == 0) {
      uVar20 = 0;
    }
    if (((uVar20 == DAT_0022db28) &&
        (iVar11 = FUN_00165944(DAT_0022db18,local_158,&local_110), iVar11 != 0)) &&
       (local_110.tv_sec == param_2)) {
      local_120 = 0;
      local_118 = 0;
      DAT_0022dad8 = 0;
      DAT_0022db38 = CONCAT44(DAT_0022db38._4_4_,1);
      local_128 = 0;
      local_130 = DAT_0010f720;
      if (((((local_148 != 0) && (lStack_150 != 0)) &&
           ((DAT_0022f1c0 == 0 || ((DAT_0022f1b0 != local_158 || (DAT_0022f1b8 != lStack_160))))))
          && (iVar11 = FUN_00192138(&local_130), iVar11 != 0)) &&
         ((((local_120 != 0 && (local_118 != 0)) && ((int)local_128 != 0)) &&
          (iVar11 = FUN_00192178(local_120,local_118,1), iVar11 != 0)))) {
        auVar4._8_8_ = local_158;
        auVar4._0_8_ = lStack_160;
        auVar22 = NEON_ext(auVar4,auVar4,8,1);
        DAT_0022f1c0 = local_118;
        DAT_0022f1c8 = local_168;
        DAT_0022f1b8 = auVar22._8_8_;
        DAT_0022f1b0 = auVar22._0_8_;
      }
      uRam000000000022dad0 = 0;
      DAT_0022dad4 = 0;
      DAT_0022dac8 = 0;
      uRam000000000022dacc = 0;
      uRam000000000022dac0 = 0;
      _DAT_0022dac4 = 0;
      _DAT_0022dab8 = 0;
      uRam000000000022dabc = 0;
      uRam000000000022dab0 = 0;
      _DAT_0022dab4 = 0;
      _DAT_0022daa8 = 0;
      uRam000000000022daac = 0;
      uRam000000000022daa0 = 0;
      _DAT_0022daa4 = 0;
      _DAT_0022da98 = 0;
      uRam000000000022da9c = 0;
      uRam000000000022da90 = 0;
      _DAT_0022da94 = 0;
      _DAT_0022da88 = 0;
      uRam000000000022da8c = 0;
      DAT_0022da7c._4_4_ = 0;
      DAT_0022da74._4_4_ = 1;
      DAT_0022da7c._0_4_ = 1;
      DAT_0022d9f0 = local_168;
    }
  }
  iVar11 = nexus_menu_ui_state(&local_130,iVar13);
  bVar8 = local_130._4_1_ != '\0';
  bVar9 = iVar11 == 1 && bVar8;
  iVar12 = FUN_00192340("ShowFastPlayAgainButton",&local_110);
  bVar10 = (iVar12 == 1 && local_100._4_1_ != '\0') && local_110.tv_nsec._4_4_ != 0;
  iVar12 = FUN_00192340("BattleEndInstantExit",&local_110);
  uVar19 = bVar10 | 2;
  bVar2 = (iVar12 == 1 && local_100._4_1_ != '\0') && local_110.tv_nsec._4_4_ != 0 || bVar10;
  if ((iVar12 != 1 || local_100._4_1_ == '\0') || local_110.tv_nsec._4_4_ == 0) {
    uVar19 = (uint)bVar10;
  }
  if ((bVar2) || ((iVar11 != 1 || !bVar8 && (DAT_0022dad8 == 0)))) {
    DAT_0022d8e8 = bVar2;
  }
  if (local_148 == 0) {
    DAT_0022dad8 = 0;
LAB_0015ff3c:
    FUN_00165ab8(0,&local_168,bVar9);
  }
  else {
    if (((DAT_0022f1c0 != 0) && (DAT_0022f1b0 == local_158)) && (DAT_0022f1b8 == lStack_160)) {
      if (lStack_150 == 0) {
        if ((((DAT_0022db18 != 0) && (DAT_0022f1b0 == DAT_0022db20)) &&
            (DAT_0022f1b8 == DAT_0022db30)) && (uVar15 = FUN_00154eb4(), (int)uVar15 != 0)) {
          local_110.tv_sec = 0;
          iVar11 = FUN_0014e7c4(uVar15,DAT_0022db18 + 0x248,&local_110,8);
          uVar20 = local_110.tv_sec;
          if (((local_110.tv_sec & 7U) != 0 || (ulong)local_110.tv_sec < 0x1000) || iVar11 == 0) {
            uVar20 = 0;
          }
          if (uVar20 == DAT_0022db28) goto LAB_0015fb3c;
        }
        goto LAB_0015fddc;
      }
LAB_0015fb3c:
      DAT_0022dad8 = 0;
LAB_0015ff18:
      DAT_0022da7c._4_4_ = 0;
      uRam000000000022daa0 = 0;
      _DAT_0022daa4 = 0;
      _DAT_0022da98 = 0;
      uRam000000000022da9c = 0;
      uRam000000000022dab0 = 0;
      _DAT_0022dab4 = 0;
      _DAT_0022daa8 = 0;
      uRam000000000022daac = 0;
      uRam000000000022dac0 = 0;
      _DAT_0022dac4 = 0;
      _DAT_0022dab8 = 0;
      uRam000000000022dabc = 0;
      uRam000000000022dad0 = 0;
      DAT_0022dad4 = 0;
      DAT_0022dac8 = 0;
      uRam000000000022dacc = 0;
      uRam000000000022da90 = 0;
      _DAT_0022da94 = 0;
      _DAT_0022da88 = 0;
      uRam000000000022da8c = 0;
      DAT_0022da74._4_4_ = 1;
      DAT_0022da7c._0_4_ = 1;
      FUN_00165ab8(0,&local_168,bVar9);
      goto LAB_0015f3b4;
    }
LAB_0015fddc:
    if (((((int)DAT_0022db38 != 0) && (local_148 != 0)) &&
        ((DAT_0022db18 != 0 && ((local_158 == DAT_0022db20 && (lStack_160 == DAT_0022db30)))))) &&
       (uVar15 = FUN_00154eb4(), (int)uVar15 != 0)) {
      local_110.tv_sec = 0;
      iVar11 = FUN_0014e7c4(uVar15,DAT_0022db18 + 0x248,&local_110,8);
      uVar20 = local_110.tv_sec;
      if (((local_110.tv_sec & 7U) != 0 || (ulong)local_110.tv_sec < 0x1000) || iVar11 == 0) {
        uVar20 = 0;
      }
      if (uVar20 == DAT_0022db28) goto LAB_0015ff18;
    }
    if (DAT_0022d8e8 != 1) goto LAB_0015ff3c;
    FUN_00164978(&local_168);
    if ((((((int)DAT_0022db38 != 0) && (local_148 != 0)) && (DAT_0022db18 != 0)) &&
        ((local_158 == DAT_0022db20 && (lStack_160 == DAT_0022db30)))) &&
       (uVar15 = FUN_00154eb4(), (int)uVar15 != 0)) {
      local_110.tv_sec = 0;
      iVar11 = FUN_0014e7c4(uVar15,DAT_0022db18 + 0x248,&local_110,8);
      uVar20 = local_110.tv_sec;
      if (((local_110.tv_sec & 7U) != 0 || (ulong)local_110.tv_sec < 0x1000) || iVar11 == 0) {
        uVar20 = 0;
      }
      if (uVar20 == DAT_0022db28) goto LAB_0015ff18;
    }
    uVar15 = FUN_00165ab8(uVar19,&local_168,bVar9);
    lVar21 = DAT_0022db10;
    pcVar7 = DAT_0022db08;
    uVar20 = DAT_0022dae0;
    iVar11 = DAT_0022dad8;
    if (param_1 == 1) {
      if ((((uVar19 & DAT_0022db5c & 1) != 0) && (DAT_0022db40 == DAT_0022db60)) &&
         (DAT_0022db48 != 0)) {
        local_110.tv_sec = 0;
        uVar15 = FUN_0014e7c4(uVar15,DAT_0022db48,&local_110,8);
        uVar20 = local_110.tv_sec;
        if (((local_110.tv_sec & 7U) != 0 || (ulong)local_110.tv_sec < 0x1000) || (int)uVar15 == 0)
        {
          uVar20 = 0;
        }
        if (((uVar20 != DAT_0022d8e0 + 0x11c0a48) ||
            (uVar15 = FUN_00154d5c(DAT_0022db48,DAT_0022db60), (int)uVar15 == 0)) ||
           (DAT_0022db48 != param_2)) goto LAB_00160204;
        uVar15 = 1;
        pcVar17 = "ShowFastPlayAgainButton";
LAB_0016029c:
        iVar11 = FUN_0014f5b8(pcVar17);
        pcVar17 = "queued";
        if (iVar11 != 2) {
          pcVar17 = "unavailable";
        }
        uVar19 = DAT_0022f4bc + 1;
        bVar9 = DAT_0022f4bc < 0x80;
        DAT_0022f4bc = uVar19;
        if (bVar9) {
          _Var14 = getpid();
          iVar11 = clock_gettime(0,&local_110);
          if (iVar11 == 0) {
            lVar21 = local_110.tv_sec * 1000 + (ulong)local_110.tv_nsec / 1000000;
          }
          else {
            lVar21 = 0;
          }
          __android_log_print(4,"NexusLab69252",
                              "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}"
                              ,"script_port_result_click",pcVar17,uVar15,_Var14,iVar13,lVar21);
        }
        goto LAB_0015f3b4;
      }
LAB_00160204:
      if ((((uVar19 & DAT_0022db5c) >> 1 != 0) && (DAT_0022db40 == DAT_0022db60)) &&
         (DAT_0022db50 != 0)) {
        local_110.tv_sec = 0;
        iVar11 = FUN_0014e7c4(uVar15,DAT_0022db50,&local_110,8);
        uVar20 = local_110.tv_sec;
        if (((local_110.tv_sec & 7U) != 0 || (ulong)local_110.tv_sec < 0x1000) || iVar11 == 0) {
          uVar20 = 0;
        }
        if (((uVar20 == DAT_0022d8e0 + 0x11c0a48) &&
            (iVar11 = FUN_00154d5c(DAT_0022db50,DAT_0022db60), iVar11 != 0)) &&
           (DAT_0022db50 == param_2)) {
          uVar15 = 2;
          pcVar17 = "BattleEndInstantExit";
          goto LAB_0016029c;
        }
      }
      uVar19 = (uint)(DAT_0022dad8 != 0);
    }
    else {
      if (DAT_0022dad8 == 0) goto LAB_0015ff54;
      if ((param_1 != 3) || (local_168 <= DAT_0022db00)) goto LAB_0015f3b4;
      if ((DAT_0022dad8 == 1) && (DAT_0022db10 != 0)) {
        iVar13 = FUN_0014f434(&local_168,&local_110);
        bVar9 = false;
        bVar10 = bVar9;
        if ((iVar13 != 0) && (bVar10 = false, local_100 == pcVar7)) {
          bVar10 = local_f8 == lVar21;
          bVar9 = bVar10;
        }
      }
      else if (DAT_0022db10 == 0) {
        iVar13 = FUN_0014f4dc(&local_168);
        bVar10 = false;
        bVar9 = bVar10;
        if ((iVar13 != 0) && (bVar10 = false, bVar9 = false, uVar20 == DAT_0022db18)) {
          lVar18 = FUN_00154cf0(uVar20 + 0x248);
          bVar10 = lVar18 == DAT_0022daf0;
          bVar9 = false;
        }
      }
      else {
        bVar10 = false;
        bVar9 = false;
      }
      if ((((DAT_0022db00 < local_168) && (local_168 - DAT_0022db00 < 0x7d1)) && (iStack_13c == 0))
         && (local_158 == DAT_0022dae8)) {
        uVar19 = 0;
        DAT_0022dad8 = 0;
        bVar2 = false;
        if (lStack_160 == DAT_0022daf8) {
          bVar2 = bVar10;
        }
        if (bVar2) {
          if ((!bVar9) || (iVar13 = FUN_00192178(pcVar7,lVar21,1), iVar13 != 0)) {
            if (iVar11 == 1) {
              uVar19 = FUN_001658c4();
              if (uVar19 != 0) {
LAB_001604c8:
                if (bVar9) {
                  iVar13 = FUN_0014f4dc(&local_168);
                  if (iVar13 != 0) {
                    DAT_0022db38 = CONCAT44(DAT_0022db38._4_4_,1);
                  }
                  auVar6._8_8_ = local_158;
                  auVar6._0_8_ = lStack_160;
                  auVar22 = NEON_ext(auVar6,auVar6,8,1);
                  DAT_0022f1c0 = lVar21;
                  DAT_0022f1c8 = local_168;
                  DAT_0022f1b8 = auVar22._8_8_;
                  DAT_0022f1b0 = auVar22._0_8_;
                }
                else {
                  DAT_0022db38 = CONCAT44(DAT_0022db38._4_4_,1);
                }
                uRam000000000022dad0 = 0;
                DAT_0022dad4 = 0;
                DAT_0022dac8 = 0;
                uRam000000000022dacc = 0;
                uRam000000000022dac0 = 0;
                _DAT_0022dac4 = 0;
                _DAT_0022dab8 = 0;
                uRam000000000022dabc = 0;
                uRam000000000022dab0 = 0;
                _DAT_0022dab4 = 0;
                _DAT_0022daa8 = 0;
                uRam000000000022daac = 0;
                uRam000000000022daa0 = 0;
                _DAT_0022daa4 = 0;
                _DAT_0022da98 = 0;
                uRam000000000022da9c = 0;
                uRam000000000022da90 = 0;
                _DAT_0022da94 = 0;
                _DAT_0022da88 = 0;
                uRam000000000022da8c = 0;
                DAT_0022da7c._4_4_ = 0;
                DAT_0022da74._4_4_ = 1;
                DAT_0022da7c._0_4_ = 1;
                DAT_0022d9f0 = local_168;
                goto LAB_00160304;
              }
            }
            else {
              iVar13 = FUN_0014d168(0x91aae0,0x1dc,
                                    "ffc8b3034ec3cc494046376cfe663e164e4e4e6210f7a4ba0eb1d68c3346715f"
                                   );
              if (iVar13 == 0) goto LAB_001606dc;
              iVar13 = FUN_00154eb4(uVar20,local_158);
              if (iVar13 != 0) {
                lVar18 = FUN_00154cf0(uVar20 + 0x248);
                if (lVar18 == DAT_0022daf0) {
                  uVar19 = 1;
                  (*(code *)(DAT_0022d8e0 + 0x91aae0))(uVar20,1);
                  iVar13 = FUN_00154eb4(uVar20,local_158);
                  if ((iVar13 != 0) &&
                     (lVar18 = FUN_00154cf0(uVar20 + 0x248), lVar18 == DAT_0022daf0)) {
                    (*(code *)(DAT_0022d8e0 + 0x91aae0))(uVar20,1);
                  }
                  goto LAB_001604c8;
                }
              }
            }
            if (bVar9) {
              FUN_00192178(pcVar7,lVar21,0);
            }
          }
LAB_001606dc:
          uVar19 = 0;
        }
      }
      else {
        uVar19 = 0;
        DAT_0022dad8 = 0;
      }
    }
LAB_00160304:
    if (uVar19 != 0) goto LAB_0015f3b4;
  }
LAB_0015ff54:
  uVar19 = 0x32;
  if ((int)DAT_0022d880 != 0) {
    uVar19 = 0xfa;
  }
  if (((DAT_002815d0 != '\x01') || (DAT_002815c8 != uVar19)) ||
     ((param_4 < DAT_002815c0 || ((ulong)uVar19 <= param_4 - DAT_002815c0)))) {
    bVar9 = false;
    DAT_002815d0 = '\x01';
    local_1a0 = local_1a0 & 0xffffffff00000000;
    DAT_002815c0 = param_4;
    DAT_002815c8 = uVar19;
    if ((local_148 != 0) && (iStack_144 != 0)) {
      iVar13 = nexus_menu_setting_value(0x27,&local_1a0);
      if (iVar13 == 1) {
        bVar9 = (int)local_1a0 == 1;
      }
      else {
        bVar9 = false;
      }
    }
    FUN_0015540c(&local_110,0x80,0x80,"BATTLES %llu  W %llu  L %llu  D %llu\nTROPHIES %+lld",
                 DAT_0022da20,DAT_0022da28,DAT_0022da30,DAT_0022da38,DAT_0022da48);
    FUN_0016d020(&local_110,bVar9);
  }
  if ((local_148 == 0) || (iStack_144 == 0 && iStack_140 == 0)) {
    if ((DAT_0022d8e8 & 1) == 0) {
      memset(&DAT_002815d8,0,0x338);
    }
    goto LAB_0015f3b4;
  }
  FUN_00164978(&local_168);
  if (param_1 == 1) {
    if ((DAT_0022d928 == param_2) || (DAT_0022d978 == param_2)) {
      DAT_0022da74._4_4_ = 1;
    }
    goto LAB_0015f3b4;
  }
  if ((param_1 | 2) == 2) {
    iVar13 = FUN_0016515c(param_3,&local_110);
    if (iVar13 != 0) {
      if (param_1 == 2) {
        FUN_00165224();
      }
      else {
        FUN_00165360(&local_168,param_2,&local_110);
      }
    }
    goto LAB_0015f3b4;
  }
  if ((param_1 != 3) || (param_4 < DAT_00281910)) goto LAB_0015f3b4;
  DAT_00281910 = param_4 + 0x32;
  iVar13 = FUN_0014e030(&DAT_0022d8f0,&local_168,&local_110);
  if (iVar13 == 0) goto LAB_0015f3b4;
  if ((((((local_110.tv_sec == param_2) || (iVar13 = FUN_00155084(), iVar13 != 0)) ||
        (iVar13 = FUN_0016563c(&local_110), iVar13 == 0)) ||
       ((FUN_0014ef90(&local_1a0,param_4), local_180 == 0 || (local_174 != 0)))) ||
      (local_198 != lStack_160)) ||
     ((local_190 != local_158 || (local_17c == 0 && iStack_178 == 0)))) {
LAB_0016039c:
    if (DAT_0022da7c._4_4_ != 0) {
      DAT_0022da7c._4_4_ = 0;
      uRam000000000022da90 = 0;
      _DAT_0022da94 = 0;
      _DAT_0022da88 = 0;
      uRam000000000022da8c = 0;
      uRam000000000022daa0 = 0;
      _DAT_0022daa4 = 0;
      _DAT_0022da98 = 0;
      uRam000000000022da9c = 0;
      uRam000000000022dab0 = 0;
      _DAT_0022dab4 = 0;
      _DAT_0022daa8 = 0;
      uRam000000000022daac = 0;
      uRam000000000022dac0 = 0;
      _DAT_0022dac4 = 0;
      _DAT_0022dab8 = 0;
      uRam000000000022dabc = 0;
      uRam000000000022dad0 = 0;
      DAT_0022dad4 = 0;
      DAT_0022dac8 = 0;
      uRam000000000022dacc = 0;
    }
  }
  else {
    if (local_d0 == 1) {
      local_1d0 = 0;
      local_1a8 = DAT_0022d8e0 + 0xac319c;
      local_1c8 = DAT_0022d8e0;
      local_1c0 = FUN_0014e7c4;
      local_1b8 = DAT_0022d8e0 + _DAT_0010fa30;
      lStack_1b0 = DAT_0022d8e0 + _UNK_0010fa38;
      iVar13 = FUN_00165768(&local_1d0);
      if (DAT_0022da7c._4_4_ != 0) {
        if (iVar13 != 0) goto LAB_0016077c;
LAB_00160788:
        DAT_0022da7c._4_4_ = 0;
        uRam000000000022daa0 = 0;
        _DAT_0022daa4 = 0;
        _DAT_0022da98 = 0;
        uRam000000000022da9c = 0;
        uRam000000000022dab0 = 0;
        _DAT_0022dab4 = 0;
        _DAT_0022daa8 = 0;
        uRam000000000022daac = 0;
        uRam000000000022dac0 = 0;
        _DAT_0022dac4 = 0;
        _DAT_0022dab8 = 0;
        uRam000000000022dabc = 0;
        uRam000000000022dad0 = 0;
        DAT_0022dad4 = 0;
        DAT_0022dac8 = 0;
        uRam000000000022dacc = 0;
        uRam000000000022da90 = 0;
        _DAT_0022da94 = 0;
        _DAT_0022da88 = 0;
        uRam000000000022da8c = 0;
      }
      if (iVar13 == 0) goto LAB_0015f3b4;
    }
    else {
      if ((local_d0 == 2) && ((local_17c == 0 || (local_188 != 0)))) goto LAB_0016039c;
      (*local_e8)(lStack_f0,local_110.tv_sec);
      if (DAT_0022da7c._4_4_ != 0) {
        iVar13 = 1;
LAB_0016077c:
        DAT_0022da74._4_4_ = 1;
        DAT_0022da7c._0_4_ = DAT_0022dac8;
        goto LAB_00160788;
      }
    }
    __android_log_print(4,"NexusRelease69252",
                        "{\"stage\":\"postbattle_dispatch\",\"kind\":%u,\"epoch\":%llu,\"matches\":%llu,\"trophies\":%lld,\"server_acceptance_proven\":false}"
                        ,local_d0,lStack_160,DAT_0022da20,DAT_0022da48);
  }
LAB_0015f3b4:
  if (*(long *)(lVar1 + 0x28) == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00162118 @ 00162118 ===== */

void FUN_00162118(long param_1)

{
  long lVar1;
  char *pcVar2;
  undefined8 uVar3;
  byte bVar4;
  byte bVar5;
  int iVar6;
  int iVar7;
  char *pcVar8;
  
  bVar4 = DAT_0022d888;
  uVar3 = DAT_0022d880;
  if ((((*(char *)(param_1 + 9) != '\0') && (*(char *)(param_1 + 10) == '\0')) &&
      (*(char *)(param_1 + 8) == 'O')) && (*(uint *)(param_1 + 4) < 0x1a)) {
    lVar1 = param_1 + (ulong)*(uint *)(param_1 + 4) * 0x40;
    *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) + 1;
    iVar6 = DAT_0025cb38;
    bVar5 = DAT_0022dfb8;
    if ((bVar4 & 1) == 0) {
      if (DAT_0022dfbc == 0) {
        if ((DAT_0022dfb8 & 1) == 0) {
          if (DAT_0022d894 == 0) {
            pcVar2 = "LOW-RES TEXTURES / GAME RELOAD";
            if (DAT_0022f4b8 != 0) {
              pcVar2 = "RELOADING TEXTURES";
            }
            pcVar8 = "RELOADING TEXTURES";
            if (((DAT_0022d88c | DAT_0022d890) & 1) == 0) {
              pcVar8 = pcVar2;
            }
          }
          else {
            pcVar8 = "RELOAD FAILED / TAP TO RETRY";
          }
        }
        else {
          pcVar8 = "TEXTURES FAILED / TAP TO RETRY";
        }
      }
      else {
        pcVar8 = "SAVE FAILED / TAP TO RETRY";
      }
    }
    else {
      pcVar8 = "TEXTURES FAILED / RESTART GAME";
    }
    iVar7 = (int)uVar3;
    *(char **)(lVar1 + 0x18) = "MAX OPTIMIZATION";
    *(char **)(lVar1 + 0x20) = pcVar8;
    uVar3 = DAT_0010f770;
    *(undefined4 *)(lVar1 + 0x10) = 0x20002;
    *(undefined8 *)(lVar1 + 0x40) = 0x10100000000;
    *(undefined4 *)(lVar1 + 0x30) = 0x18;
    *(int *)(lVar1 + 0x34) = iVar7;
    *(undefined8 *)(lVar1 + 0x28) = uVar3;
    *(uint *)(lVar1 + 0x38) = (uint)(iVar7 != 0 && iVar6 != 0) & (bVar5 ^ 1);
    *(int *)(lVar1 + 0x3c) = iVar7;
    *(undefined2 *)(lVar1 + 0x48) = 5;
    return;
  }
  return;
}

/* ===== FUN_00165ab8 @ 00165ab8 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00165ab8(long param_1,long param_2,int param_3)

{
  bool bVar1;
  ulong uVar2;
  char *pcVar3;
  long lVar4;
  bool bVar5;
  int iVar6;
  __pid_t _Var7;
  undefined4 uVar8;
  ulong uVar9;
  ulong uVar10;
  undefined8 uVar11;
  uint uVar12;
  ulong uVar13;
  long lVar14;
  uint uVar15;
  uint uVar16;
  long lVar17;
  long *plVar18;
  ulong *puVar19;
  float fVar20;
  float local_c4;
  undefined8 local_c0;
  ulong local_b8;
  undefined8 local_b0;
  long local_a8;
  long local_a0;
  timespec local_98;
  undefined8 local_88;
  undefined8 local_80;
  undefined1 auStack_78 [16];
  long local_68;
  
  lVar4 = tpidr_el0;
  local_68 = *(long *)(lVar4 + 0x28);
  uVar12 = (uint)param_1;
  if (DAT_0022db40 == 0 || DAT_0022db40 == DAT_0022db60) {
    if (DAT_0022db48 != 0) {
      if (DAT_0022db40 == DAT_0022db60) {
        local_b8 = 0;
        param_1 = FUN_0014e7c4(param_1,DAT_0022db48,&local_b8,8);
        uVar13 = local_b8;
        if (((local_b8 & 7) != 0 || local_b8 < 0x1000) || (int)param_1 == 0) {
          uVar13 = 0;
        }
        if ((uVar13 == DAT_0022d8e0 + 0x11c0a48U) &&
           (param_1 = FUN_00154d5c(DAT_0022db48,DAT_0022db60), (int)param_1 != 0))
        goto LAB_00165ba8;
      }
      DAT_0022db48 = 0;
      _DAT_0022db58 = _DAT_0022db58 & 0xfffffffeffffffff;
    }
  }
  else {
    DAT_0022db48 = 0;
    DAT_0022db40 = 0;
    _DAT_0022db58 = 0;
    DAT_0022db50 = 0;
  }
LAB_00165ba8:
  if (DAT_0022db50 != 0) {
    if (DAT_0022db40 == DAT_0022db60) {
      local_b8 = 0;
      param_1 = FUN_0014e7c4(param_1,DAT_0022db50,&local_b8,8);
      uVar13 = local_b8;
      if (((local_b8 & 7) != 0 || local_b8 < 0x1000) || (int)param_1 == 0) {
        uVar13 = 0;
      }
      if ((uVar13 == DAT_0022d8e0 + 0x11c0a48U) &&
         (param_1 = FUN_00154d5c(DAT_0022db50,DAT_0022db60), (int)param_1 != 0)) goto LAB_00165c28;
    }
    DAT_0022db50 = 0;
    _DAT_0022db58 = _DAT_0022db58 & 0xfffffffdffffffff;
  }
LAB_00165c28:
  bVar5 = false;
  if ((*(int *)(param_2 + 0x20) != 0) && (param_1 = DAT_0022db18, DAT_0022db18 != 0)) {
    if ((*(long *)(param_2 + 0x10) == DAT_0022db20) &&
       ((*(long *)(param_2 + 8) == DAT_0022db30 && (param_1 = FUN_00154eb4(), (int)param_1 != 0))))
    {
      local_b8 = 0;
      param_1 = FUN_0014e7c4(param_1,DAT_0022db18 + 0x248,&local_b8,8);
      if (((local_b8 & 7) != 0 || local_b8 < 0x1000) || (int)param_1 == 0) {
        local_b8 = 0;
      }
      bVar5 = local_b8 == DAT_0022db28;
    }
    else {
      bVar5 = false;
    }
  }
  local_a8 = 0;
  local_a0 = 0;
  local_b0 = 0;
  local_b8 = DAT_0010f720;
  uVar13 = (ulong)*(uint *)(param_2 + 0x20);
  uVar15 = 3;
  if (!(bool)(bVar5 & (int)DAT_0022db38 == 0)) {
    uVar15 = 0;
  }
  if ((*(uint *)(param_2 + 0x20) != 0) && (uVar13 = *(ulong *)(param_2 + 0x18), uVar13 != 0)) {
    if ((DAT_0022f1c0 == 0) ||
       ((DAT_0022f1b0 != *(long *)(param_2 + 0x10) || (DAT_0022f1b8 != *(long *)(param_2 + 8))))) {
      param_1 = FUN_00192138(&local_b8);
      uVar13 = 0;
      if (((int)param_1 != 0) && ((local_a8 != 0 && (local_a0 != 0)))) {
        uVar13 = (ulong)((int)local_b0 != 0);
      }
    }
    else {
      uVar13 = 0;
    }
  }
  uVar16 = 0;
  if ((uVar12 != 0) && (param_3 == 0)) {
    if (((*(int *)(param_2 + 0x20) == 0) || (*(int *)(param_2 + 0x2c) != 0 || DAT_0022dad8 != 0)) ||
       (((DAT_0022f1c0 != 0 && (DAT_0022f1b0 == *(long *)(param_2 + 0x10))) &&
        (DAT_0022f1b8 == *(long *)(param_2 + 8))))) {
      uVar16 = 0;
    }
    else {
      uVar16 = ((uint)uVar13 | uVar15) & uVar12;
    }
  }
  local_c0 = 0;
  local_c4 = 0.0;
  local_88 = 0;
  local_80 = 0;
  uVar13 = FUN_0014e7c4(param_1,DAT_0022f178 + 0x178,(long)&local_c0 + 4,4);
  if ((int)uVar13 != 0) {
    if ((((ABS(local_c0._4_4_) != INFINITY) && (!NAN(ABS(local_c0._4_4_)))) &&
        ((0.0 < local_c0._4_4_ &&
         ((local_c0._4_4_ == 16.0 || local_c0._4_4_ < 16.0 != NAN(local_c0._4_4_) &&
          (uVar13 = FUN_0014e7c4(uVar13,DAT_0022f178 + 0x4c,&local_c0,4), (int)uVar13 != 0)))))) &&
       (uVar13 = FUN_0014e7c4(uVar13,DAT_0022f178 + 0x54,&local_c4,4), (int)uVar13 != 0)) {
      if ((((ABS((float)local_c0) != INFINITY) && (!NAN(ABS((float)local_c0)))) &&
          (ABS(local_c4) != INFINITY)) &&
         ((!NAN(ABS(local_c4)) &&
          (uVar13 = FUN_0014e7c4(uVar13,DAT_0022f178 + 0x3c,&local_88,0x10), (int)uVar13 != 0)))) {
        if ((ABS((float)local_80) != INFINITY) &&
           ((!NAN(ABS((float)local_80)) && (300.0 <= (float)local_80)))) goto LAB_00165ed8;
      }
    }
  }
  uVar16 = 0;
LAB_00165ed8:
  lVar17 = 0;
  bVar5 = true;
  do {
    uVar12 = 1 << lVar17;
    if ((uVar12 & uVar16) == 0) {
LAB_00165fe8:
      if (DAT_0022db40 == DAT_0022db60) {
        plVar18 = &DAT_0022db48 + lVar17;
        if (*plVar18 != 0) {
          local_98.tv_sec = 0;
          uVar13 = FUN_0014e7c4(uVar13,*plVar18,&local_98,8);
          uVar9 = local_98.tv_sec;
          if (((local_98.tv_sec & 7U) != 0 || (ulong)local_98.tv_sec < 0x1000) || (int)uVar13 == 0)
          {
            uVar9 = 0;
          }
          if ((uVar9 == DAT_0022d8e0 + 0x11c0a48U) &&
             (uVar13 = FUN_00154d5c(*plVar18,DAT_0022db60), (int)uVar13 != 0)) {
            uVar13 = (*(code *)(DAT_0022d8e0 + 0x595314))(0xc61c3c00,0xc61c3c00,*plVar18);
            uVar12 = DAT_0022db5c & (uVar12 ^ 0xffffffff);
            goto LAB_00165f64;
          }
        }
      }
    }
    else {
      if (DAT_0022db40 != DAT_0022db60) {
LAB_00165fdc:
        if ((DAT_0022db58 == 0) &&
           (uVar13 = FUN_00163eb8(uVar13,"map_editor_big_exit_button"), (int)uVar13 != 0)) {
          uVar9 = (*(code *)(DAT_0022d8e0 + 0x11a2840))(0x260);
          uVar13 = uVar9;
          if (uVar9 != 0) {
            (*(code *)(DAT_0022d8e0 + 0x887c14))();
            uVar13 = FUN_0014e5ac(uVar9);
            if ((((int)uVar13 != 0) && (uVar13 = FUN_00164090(uVar9), (int)uVar13 != 0)) &&
               (uVar10 = (*(code *)(DAT_0022d8e0 + 0x51ecec))
                                   ("sc/ui.sc","map_editor_big_exit_button"), uVar13 = uVar10,
               uVar10 != 0)) {
              local_98.tv_sec = 0;
              uVar13 = FUN_0014e7c4(uVar10,uVar10,&local_98,8);
              uVar2 = local_98.tv_sec;
              if (((local_98.tv_sec & 7U) != 0 || (ulong)local_98.tv_sec < 0x1000) ||
                  (int)uVar13 == 0) {
                uVar2 = 0;
              }
              if (uVar2 == DAT_0022d8e0 + 0x11ad208U) {
                (*(code *)(DAT_0022d8e0 + 0x772a30))(uVar9,uVar10,1);
                uVar11 = (*(code *)(DAT_0022d8e0 + 0x5d7c30))(uVar10,0);
                local_98.tv_sec = 0;
                uVar13 = FUN_0014e7c4(uVar11,uVar9 + 0x80,&local_98,8);
                uVar2 = local_98.tv_sec;
                if (((local_98.tv_sec & 7U) != 0 || (ulong)local_98.tv_sec < 0x1000) ||
                    (int)uVar13 == 0) {
                  uVar2 = 0;
                }
                if (uVar2 == uVar10) {
                  local_98.tv_sec = 0;
                  uVar13 = FUN_0014e7c4(uVar13,uVar9 + 0xb0,&local_98,8);
                  uVar2 = local_98.tv_sec;
                  if (((local_98.tv_sec & 7U) != 0 || (ulong)local_98.tv_sec < 0x1000) ||
                      (int)uVar13 == 0) {
                    uVar2 = 0;
                  }
                  if (uVar2 == uVar10) {
                    local_98.tv_sec = 0;
                    iVar6 = FUN_0014e7c4(uVar13,uVar9 + 0x88,&local_98,8);
                    uVar13 = local_98.tv_sec;
                    if (((local_98.tv_sec & 7U) != 0 || (ulong)local_98.tv_sec < 0x1000) ||
                        iVar6 == 0) {
                      uVar13 = 0;
                    }
                    uVar13 = FUN_00154d5c(uVar13,uVar9);
                    if ((int)uVar13 != 0) {
                      pcVar3 = "PLAY AGAIN";
                      if (!bVar5) {
                        pcVar3 = "EXIT";
                      }
                      (*(code *)(DAT_0022d8e0 + 0x66ae58))(auStack_78,pcVar3);
                      (*(code *)(DAT_0022d8e0 + 0x88836c))(uVar9,auStack_78,1);
                      (*(code *)(DAT_0022d8e0 + 0x66ad48))(auStack_78);
                      FUN_0016cf00(0x3fa00000,0x3fa00000,uVar9);
                      (*(code *)(DAT_0022d8e0 + 0x595314))(0xc61c3c00,0xc61c3c00,uVar9);
                      (*(code *)(DAT_0022d8e0 + 0x5988ec))(DAT_0022f178,uVar9);
                      uVar13 = FUN_00154d5c(uVar9,DAT_0022db60);
                      if ((int)uVar13 != 0) {
                        DAT_0022db40 = DAT_0022db60;
                        (&DAT_0022db48)[lVar17] = uVar9;
                        goto LAB_00165efc;
                      }
                    }
                  }
                }
              }
            }
          }
          _DAT_0022db58 = CONCAT44(DAT_0022db5c,1);
          uVar15 = DAT_0022f4bc + 1;
          bVar1 = DAT_0022f4bc < 0x80;
          DAT_0022f4bc = uVar15;
          if (bVar1) {
            _Var7 = getpid();
            uVar8 = gettid(_Var7);
            iVar6 = clock_gettime(0,&local_98);
            if (iVar6 == 0) {
              lVar14 = local_98.tv_sec * 1000 + (ulong)local_98.tv_nsec / 1000000;
            }
            else {
              lVar14 = 0;
            }
            uVar13 = __android_log_print(4,"NexusLab69252",
                                         "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}"
                                         ,"script_port_result_button_unavailable",
                                         "owned_gamebutton_contract",lVar17,_Var7,uVar8,lVar14);
          }
        }
        goto LAB_00165fe8;
      }
      puVar19 = (ulong *)(&DAT_0022db48 + lVar17);
      if (*puVar19 == 0) goto LAB_00165fdc;
      local_98.tv_sec = 0;
      uVar13 = FUN_0014e7c4(uVar13,*puVar19,&local_98,8);
      uVar9 = local_98.tv_sec;
      if (((local_98.tv_sec & 7U) != 0 || (ulong)local_98.tv_sec < 0x1000) || (int)uVar13 == 0) {
        uVar9 = 0;
      }
      if ((uVar9 != DAT_0022d8e0 + 0x11c0a48U) ||
         (uVar13 = FUN_00154d5c(*puVar19,DAT_0022db60), (int)uVar13 == 0)) goto LAB_00165fdc;
      uVar9 = *puVar19;
LAB_00165efc:
      fVar20 = (float)local_80 + -95.0;
      if (!bVar5) {
        fVar20 = 95.0;
      }
      uVar13 = (*(code *)(DAT_0022d8e0 + 0x595314))
                         ((fVar20 - (float)local_c0) / local_c0._4_4_,
                          (88.0 - local_c4) / local_c0._4_4_,uVar9);
      uVar12 = DAT_0022db5c | uVar12;
LAB_00165f64:
      _DAT_0022db58 = CONCAT44(uVar12,DAT_0022db58);
    }
    lVar17 = 1;
    bVar1 = !bVar5;
    bVar5 = false;
    if (bVar1) {
      if (*(long *)(lVar4 + 0x28) == local_68) {
        return;
      }
                    /* WARNING: Subroutine does not return */
      __stack_chk_fail();
    }
  } while( true );
}

/* ===== nexus_rich_plan @ 0016a3ec ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nexus_rich_plan(float param_1,float param_2,long param_3,undefined8 param_4,uint *param_5)

{
  int iVar1;
  float *pfVar2;
  char cVar3;
  long lVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  bool bVar15;
  int iVar16;
  uint uVar17;
  undefined8 uVar18;
  long lVar19;
  size_t sVar20;
  undefined1 *puVar21;
  bool bVar22;
  ulong uVar23;
  float *pfVar24;
  long lVar25;
  ulong uVar26;
  ulong uVar27;
  uint *puVar28;
  ulong uVar29;
  char *__s;
  uint *puVar30;
  ulong uVar31;
  ulong uVar32;
  float fVar33;
  undefined4 uVar34;
  uint uVar35;
  float fVar36;
  float fVar37;
  float fVar38;
  float fVar39;
  undefined8 uVar40;
  float fVar41;
  undefined8 local_260;
  undefined8 uStack_258;
  undefined8 uStack_250;
  undefined8 uStack_248;
  undefined8 local_240;
  undefined8 uStack_238;
  undefined8 uStack_230;
  undefined8 uStack_228;
  undefined8 local_220;
  undefined8 uStack_218;
  undefined8 uStack_210;
  undefined8 uStack_208;
  undefined8 local_200;
  undefined8 uStack_1f8;
  undefined8 uStack_1f0;
  undefined8 uStack_1e8;
  undefined8 local_1e0;
  undefined8 uStack_1d8;
  undefined8 local_1d0;
  undefined8 uStack_1c8;
  undefined8 local_1c0;
  undefined8 uStack_1b8;
  undefined8 uStack_1b0;
  undefined8 uStack_1a8;
  undefined8 local_1a0;
  undefined8 uStack_198;
  undefined8 uStack_190;
  undefined8 uStack_188;
  undefined8 local_180;
  undefined8 uStack_178;
  undefined8 uStack_170;
  undefined8 local_168;
  undefined8 local_160 [21];
  long local_b8;
  
  lVar4 = tpidr_el0;
  local_b8 = *(long *)(lVar4 + 0x28);
  uVar18 = 4;
  if ((((((param_3 == 0) || (param_5 == (uint *)0x0)) || (ABS(param_1) == INFINITY)) ||
       ((NAN(ABS(param_1)) || (0x1a < *(uint *)(param_3 + 4))))) ||
      (param_2 != 8192.0 && param_2 < 8192.0 == NAN(param_2))) ||
     (((param_1 != 8192.0 && param_1 < 8192.0 == NAN(param_1) || (param_2 < 540.0)) ||
      ((param_1 < 960.0 || ((ABS(param_2) == INFINITY || (NAN(ABS(param_2))))))))))
  goto LAB_0016aa44;
  memset(param_5,0,0x1b810);
  uVar40 = DAT_0010f910;
  uVar9 = DAT_0010f880;
  uVar8 = DAT_0010f860;
  uVar17 = *param_5;
  uVar31 = 0;
  *param_5 = uVar17 + 2;
  uVar6 = DAT_0010f7b0;
  uVar18 = DAT_0010f7a0;
  fVar33 = (float)NEON_fminnm(param_1 * 0.0009765625,param_2 / 576.0);
  *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 4) = uVar8;
  *(undefined1 *)(param_5 + (ulong)uVar17 * 0x2c + 0xf) = 1;
  uVar10 = _UNK_0010f958;
  uVar7 = _DAT_0010f950;
  *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 7) = uVar40;
  param_5[1] = (uint)fVar33;
  uVar14 = _UNK_0010fb28;
  uVar11 = _DAT_0010fb20;
  uVar5 = DAT_0010f7a8;
  *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 0xb) = uVar10;
  *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 9) = uVar7;
  uVar13 = _UNK_0010f998;
  uVar12 = _DAT_0010f990;
  *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 0x37) = uVar14;
  *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 0x35) = uVar11;
  uVar10 = DAT_0010f8e0;
  uVar7 = DAT_0010f848;
  *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 0xd) = uVar5;
  uVar11 = DAT_0010f920;
  *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 0x39) = uVar5;
  uVar5 = DAT_0010f840;
  *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 0x32) = uVar13;
  *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 0x30) = uVar12;
  *(ulong *)(param_5 + 2) =
       CONCAT44((float)((ulong)uVar18 >> 0x20) * fVar33 +
                (param_2 + (float)((ulong)uVar9 >> 0x20) * fVar33) * 0.5,
                (float)uVar18 * fVar33 + (param_1 + (float)uVar9 * fVar33) * 0.5);
  param_5[(ulong)uVar17 * 0x2c + 0x34] = 0;
  *(undefined1 *)(param_5 + (ulong)uVar17 * 0x2c + 0x3b) = 1;
  do {
    uVar17 = *param_5;
    if (uVar17 < 0x280) {
      uVar35 = NEON_fmadd((float)(uVar31 & 0xffffffff),0x43120000,0xc3e40000);
      *param_5 = uVar17 + 1;
      puVar30 = param_5 + (ulong)uVar17 * 0x2c + 4;
      *(undefined8 *)puVar30 = uVar6;
      *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 10) = uVar5;
      param_5[(ulong)uVar17 * 0x2c + 0xc] = 0x3f8f5c29;
      *(undefined1 *)(param_5 + (ulong)uVar17 * 0x2c + 0xf) = 1;
      param_5[(ulong)uVar17 * 0x2c + 9] = uVar35;
      *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 7) = uVar40;
    }
    else {
      puVar30 = (uint *)0x0;
    }
    uVar17 = (&DAT_0013cdb0)[uVar31];
    *(undefined8 *)(puVar30 + 9) = uVar10;
    puVar30[2] = uVar17;
    cVar3 = *(char *)(param_3 + 8);
    lVar19 = __strchr_chk("CASFBTX",cVar3,8);
    if (lVar19 == 0) {
      lVar19 = __strchr_chk(&DAT_00138fa4,cVar3,4);
      if (lVar19 == 0) {
        if (cVar3 == 'U') {
          uVar23 = 3;
        }
        else {
          lVar19 = __strchr_chk("OQKDZGNPJL",cVar3,0xb);
          if (lVar19 == 0) {
            uVar23 = 5;
            if (cVar3 != 'Y') {
              uVar23 = 0;
            }
          }
          else {
            uVar23 = 4;
          }
        }
      }
      else {
        uVar23 = 2;
      }
    }
    else {
      uVar23 = 1;
    }
    if (uVar31 == uVar23) {
      *(undefined8 *)(puVar30 + 9) = uVar11;
      *(ulong *)(puVar30 + 5) =
           CONCAT44((float)((ulong)*(undefined8 *)(puVar30 + 5) >> 0x20) +
                    (float)((ulong)uVar7 >> 0x20),(float)*(undefined8 *)(puVar30 + 5) + (float)uVar7
                   );
    }
    uVar9 = _UNK_0010faa8;
    uVar18 = _DAT_0010faa0;
    uVar31 = uVar31 + 1;
  } while (uVar31 != 6);
  uVar17 = *param_5;
  *param_5 = uVar17 + 1;
  *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 0xb) = uVar9;
  *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 9) = uVar18;
  uVar18 = DAT_0010f878;
  *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 4) = uVar8;
  *(undefined1 *)(param_5 + (ulong)uVar17 * 0x2c + 0xf) = 1;
  *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 7) = uVar40;
  *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 0xd) = uVar18;
  cVar3 = *(char *)(param_3 + 8);
  lVar19 = __strchr_chk("CASFBTX",cVar3,8);
  uVar34 = 0x3f800000;
  if (lVar19 == 0) {
    lVar19 = __strchr_chk(&DAT_00138fa4,cVar3,4);
    uVar34 = 0x40000000;
    if ((lVar19 == 0) && (uVar34 = 0x40400000, cVar3 != 'U')) {
      lVar19 = __strchr_chk("OQKDZGNPJL",cVar3,0xb);
      uVar34 = 0x40800000;
      if ((lVar19 == 0) && (uVar34 = 0x40a00000, cVar3 != 'Y')) {
        uVar34 = 0;
      }
    }
  }
  uVar17 = *param_5;
  fVar33 = (float)NEON_fmadd(uVar34,0x43120000,0xc3e40000);
  uVar31 = 0;
  *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 4) = DAT_0010f728;
  uVar6 = _UNK_0010fb78;
  uVar5 = _DAT_0010fb70;
  *param_5 = uVar17 + 1;
  uVar18 = DAT_0010f7e0;
  *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 0xc) = uVar6;
  *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 10) = uVar5;
  *(undefined1 *)(param_5 + (ulong)uVar17 * 0x2c + 0xf) = 1;
  param_5[(ulong)uVar17 * 0x2c + 9] = (uint)(fVar33 + 0.02057143);
  param_5[(ulong)uVar17 * 0x2c + 0xe] = 0x4299d89e;
  (param_5 + (ulong)uVar17 * 0x2c + 7)[0] = 0xffffffff;
  (param_5 + (ulong)uVar17 * 0x2c + 7)[1] = 0;
  do {
    __s = (&PTR_s_MAIN_PAGE_0019b450)[uVar31];
    sVar20 = strlen(__s);
    if (sVar20 < 7) {
      uVar34 = 0x3f866666;
    }
    else if (sVar20 == 7) {
      uVar34 = 0x3f8a3d71;
    }
    else {
      uVar34 = 0x3f8f5c29;
      if (sVar20 != 8) {
        uVar34 = 0x3f970a3d;
      }
    }
    sVar20 = strlen(__s);
    if ((0x7f < sVar20) || (uVar17 = *param_5, 0x27f < uVar17)) goto LAB_0016aa3c;
    *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 4) = uVar18;
    fVar33 = (float)NEON_fmadd((float)(uVar31 & 0xffffffff),0x43120000,0xc3e40000);
    *param_5 = uVar17 + 1;
    param_5[(ulong)uVar17 * 0x2c + 10] = 0x43f68000;
    *(undefined1 *)(param_5 + (ulong)uVar17 * 0x2c + 0xf) = 1;
    (param_5 + (ulong)uVar17 * 0x2c + 7)[0] = 0xffffffff;
    (param_5 + (ulong)uVar17 * 0x2c + 7)[1] = 0;
    *(ulong *)(param_5 + (ulong)uVar17 * 0x2c + 0xb) = CONCAT44(uVar34,uVar34);
    param_5[(ulong)uVar17 * 0x2c + 9] = (uint)(fVar33 + -25.0);
    sVar20 = strlen(__s);
    memcpy((void *)((long)param_5 + (ulong)uVar17 * 0xb0 + 0x3e),__s,sVar20 + 1);
    uVar31 = uVar31 + 1;
    param_5[(ulong)uVar17 * 0x2c + 7] = 0xffffffff;
  } while (uVar31 != 6);
  iVar16 = FUN_0016b76c(0xc4094000,0x42800000,0x40100000,param_5,"NEXUS MENU",1,0,9,param_4);
  if (((iVar16 != 0) &&
      (iVar16 = FUN_0016b76c(0xc3990000,0x42800000,0x40100000,param_5,&DAT_001384c3,1,0,1,param_4),
      iVar16 != 0)) &&
     (iVar16 = FUN_0016b76c(0xc4040000,0x42de0000,0x3f59999a,param_5,"t.me/NexusBrawl",1,1,0xf,
                            param_4), uVar5 = _DAT_0010faf0, iVar16 != 0)) {
    uVar17 = *param_5;
    uVar31 = (ulong)uVar17;
    *(undefined8 *)(param_5 + uVar31 * 0x2c + 0xb) = _UNK_0010faf8;
    *(undefined8 *)(param_5 + uVar31 * 0x2c + 9) = uVar5;
    uVar6 = _UNK_0010fab8;
    uVar5 = _DAT_0010fab0;
    lVar19 = uVar31 + 1;
    *(undefined1 *)(param_5 + 0x45b) = 0;
    param_5[uVar31 * 0x2c + 8] = 0;
    *(undefined8 *)(param_5 + uVar31 * 0x2c + 6) = uVar6;
    *(undefined8 *)(param_5 + uVar31 * 0x2c + 4) = uVar5;
    uVar5 = DAT_0010f778;
    *param_5 = (uint)lVar19;
    *(undefined1 *)(param_5 + uVar31 * 0x2c + 0xf) = 1;
    *(undefined8 *)(param_5 + uVar31 * 0x2c + 0xd) = uVar5;
    uVar6 = _UNK_0010f988;
    uVar5 = _DAT_0010f980;
    if (uVar17 < 0x27f) {
      *param_5 = uVar17 + 2;
      *(undefined8 *)(param_5 + lVar19 * 0x2c + 0xb) = uVar6;
      *(undefined8 *)(param_5 + lVar19 * 0x2c + 9) = uVar5;
      *(undefined8 *)(param_5 + lVar19 * 0x2c + 4) = uVar18;
      *(undefined1 *)(param_5 + lVar19 * 0x2c + 0xf) = 1;
      *(undefined2 *)((long)param_5 + lVar19 * 0xb0 + 0x3e) = 0x58;
      (param_5 + lVar19 * 0x2c + 7)[0] = 0xffffffff;
      uVar10 = _UNK_0010fb98;
      uVar9 = _DAT_0010fb90;
      uVar8 = _UNK_0010f9b8;
      uVar7 = _DAT_0010f9b0;
      uVar6 = DAT_0010f7e8;
      uVar5 = DAT_0010f7c0;
      (param_5 + lVar19 * 0x2c + 7)[1] = 0;
      uVar31 = (ulong)*(uint *)(param_3 + 4);
      local_160[1] = 0;
      local_160[0] = 0;
      local_160[3] = 0;
      local_160[2] = 0;
      local_160[5] = 0;
      local_160[4] = 0;
      local_160[7] = 0;
      local_160[6] = 0;
      local_160[9] = 0;
      local_160[8] = 0;
      local_160[0xb] = 0;
      local_160[10] = 0;
      local_160[0xd] = 0;
      local_160[0xc] = 0;
      local_160[0xf] = 0;
      local_160[0xe] = 0;
      local_160[0x11] = 0;
      local_160[0x10] = 0;
      local_160[0x13] = 0;
      local_160[0x12] = 0;
      if (*(uint *)(param_3 + 4) != 0) {
        uVar17 = 0;
        pfVar24 = (float *)(param_3 + 0x28);
        do {
          if ((*(char *)(pfVar24 + 8) != '\x01') || (pfVar24[1] != 516.0)) {
            fVar33 = pfVar24[-6];
            if ((fVar33 != 1.82169e-43) || ((*pfVar24 != 315.0 || (pfVar24[1] != 100.0)))) {
              if (uVar17 == 0x14) goto LAB_0016aa3c;
              pfVar2 = (float *)0x0;
              if (fVar33 != 0.0) {
                pfVar2 = pfVar24 + -6;
              }
              local_160[uVar17] = pfVar2;
              uVar17 = uVar17 + 1;
            }
          }
          pfVar24 = pfVar24 + 0x10;
          uVar31 = uVar31 - 1;
        } while (uVar31 != 0);
      }
      uVar31 = 0;
      do {
        puVar30 = (uint *)local_160[uVar31];
        if (puVar30 == (uint *)0x0) {
          uVar23 = (ulong)*param_5;
          lVar19 = 0;
          uVar17 = 0;
          uVar35 = ((uint)uVar31 & 0xff) / 5;
          bVar15 = true;
          fVar33 = (float)NEON_fmadd((float)((uint)uVar31 + uVar35 * -5 & 0xff),0x432f0000,
                                     0xc3dc0000);
          fVar41 = (float)NEON_fmadd((float)uVar35,0x42dc0000,0x43320000);
        }
        else {
          if (*(char *)(param_3 + 10) == '\0') {
            lVar25 = 0;
            do {
              if ((*(uint *)((long)&DAT_0019b5e8 + lVar25) == (uint)*(byte *)(param_3 + 8)) &&
                 (lVar19 = (long)&DAT_0019b5e8 + lVar25,
                 *(uint *)((long)&DAT_0019b5ec + lVar25) == *puVar30)) goto LAB_0016ac0c;
              lVar25 = lVar25 + 0x20;
            } while (lVar25 != 0xa60);
          }
          lVar19 = 0;
LAB_0016ac0c:
          if ((puVar30[10] == 0) || ((char)puVar30[0xd] == '\0')) {
            uVar17 = 0;
          }
          else {
            uVar17 = (uint)(puVar30[0xc] == 0) << 1;
          }
          bVar15 = lVar19 == 0;
          if ((lVar19 != 0) && (*(int *)(lVar19 + 0x10) == 0)) {
            uVar17 = *(uint *)(lVar19 + 0xc);
          }
          fVar33 = (float)puVar30[6];
          fVar41 = (float)puVar30[7];
          uVar23 = (ulong)*param_5;
          if ((char)puVar30[0xe] == '\0') {
            uVar35 = *puVar30;
            if (uVar35 == 0x30) {
              uVar17 = 2;
            }
            else if (uVar35 == 0x32) {
              uVar17 = 3;
            }
            else if (uVar35 == 0x31) {
              uVar17 = 1;
            }
          }
        }
        if (uVar23 < 0x280) {
          *param_5 = (int)uVar23 + 1;
          puVar28 = param_5 + uVar23 * 0x2c + 4;
          *puVar28 = 2;
          param_5[uVar23 * 0x2c + 5] = uVar17;
          uVar35 = NEON_fmadd((float)((uVar31 & 0xffffffff) / 5),0x3f4ccccd,fVar41 + -2.4);
          param_5[uVar23 * 0x2c + 9] = (uint)fVar33;
          *(undefined8 *)(param_5 + uVar23 * 0x2c + 0xb) = uVar6;
          param_5[uVar23 * 0x2c + 8] = (uint)(uVar17 == 0);
          *(undefined1 *)(param_5 + uVar23 * 0x2c + 0xf) = 1;
          param_5[uVar23 * 0x2c + 10] = uVar35;
          param_5[uVar23 * 0x2c + 7] = 0xffffffff;
        }
        else {
          puVar28 = (uint *)0x0;
        }
        if (puVar30 == (uint *)0x0) {
          uVar17 = 0;
        }
        else {
          uVar17 = *puVar30;
        }
        puVar28[2] = uVar17;
        *(undefined8 *)(puVar28 + 9) = uVar5;
        if (bVar15) {
          uStack_258 = 0;
          local_260 = 0;
          uStack_248 = 0;
          uStack_250 = 0;
          uStack_238 = 0;
          local_240 = 0;
          uStack_228 = 0;
          uStack_230 = 0;
          uStack_218 = 0;
          local_220 = 0;
          uStack_208 = 0;
          uStack_210 = 0;
          uStack_1f8 = 0;
          local_200 = 0;
          uStack_1e8 = 0;
          uStack_1f0 = 0;
          uStack_1d8 = 0;
          local_1e0 = 0;
          uStack_1c8 = 0;
          local_1d0 = 0;
          uStack_1b8 = 0;
          local_1c0 = 0;
          uStack_1a8 = 0;
          uStack_1b0 = 0;
          uStack_198 = 0;
          local_1a0 = 0;
          uStack_188 = 0;
          uStack_190 = 0;
          uStack_178 = 0;
          local_180 = 0;
          local_168 = 0;
          uStack_170 = 0;
          if (puVar30 == (uint *)0x0) {
            bVar15 = false;
            uVar32 = 0xffffffff;
          }
          else {
            if ((*(char **)(puVar30 + 2) == (char *)0x0) ||
               (sVar20 = strlen(*(char **)(puVar30 + 2)), 0x7f < sVar20)) goto LAB_0016aa3c;
            FUN_0016ba38(&local_1e0,0x80);
            puVar21 = (undefined1 *)__strchr_chk(&local_1e0,10,0x80);
            if (puVar21 != (undefined1 *)0x0) {
              *puVar21 = 0;
            }
            FUN_0016ba38(&local_260,0x80);
            if (((*puVar30 - 0x21037 < 0xffffffc9) && ((byte)puVar30[0xe] < 8)) &&
               ((1 << (ulong)((byte)puVar30[0xe] & 0x1f) & 0xd8U) != 0)) {
              FUN_0016ba38(&local_260,0x80);
            }
            if (*(char *)(param_3 + 8) == 'R') {
              if (*(char *)(param_3 + 10) == '\0') {
                uVar17 = FUN_0016bae0(*(undefined8 *)(puVar30 + 2));
                uVar32 = (ulong)uVar17;
                if ((int)uVar17 < 0) {
                  bVar15 = false;
                }
                else {
                  FUN_0016ba38(&local_260,0x80);
                  bVar15 = true;
                }
              }
              else {
                bVar15 = false;
                uVar32 = 0xffffffff;
              }
            }
            else {
              bVar15 = false;
              uVar32 = 0xffffffff;
            }
          }
          fVar38 = 0.0;
          bVar22 = false;
          uVar17 = 0;
          fVar37 = 0.0;
          while( true ) {
            for (; *(char *)((long)&local_1e0 + (ulong)uVar17) == ' '; uVar17 = uVar17 + 1) {
              fVar38 = fVar38 + 7.0;
            }
            if (*(char *)((long)&local_1e0 + (ulong)uVar17) == '\0') break;
            fVar39 = -0.0;
            if (bVar22) {
              fVar39 = fVar38 + 4.2;
            }
            fVar36 = (float)FUN_0016cca0(0x3fb33333);
            fVar38 = 0.0;
            fVar37 = fVar37 + fVar39 + fVar36;
            bVar22 = true;
            uVar17 = uVar17 + 1;
          }
          fVar38 = 207.2 / fVar37;
          if (fVar37 == 148.0 || fVar37 < 148.0 != NAN(fVar37)) {
            fVar38 = 1.4;
          }
          iVar16 = FUN_0016b76c(0x3f47ae14,fVar33 + -55.0,fVar41 + -27.0,fVar38,param_5,&local_1e0,0
                                ,uVar31 & 0xffffffff,0x1a,param_4);
          if (iVar16 != 0) {
            uVar40 = 0;
            if (bVar15) {
              uVar40 = *(undefined8 *)(&UNK_0019b490 + uVar32 * 0x18);
            }
            uVar32 = __strlen_chk(&local_260,0x80);
            if (uVar32 < 0xc) {
              uVar34 = 0x3f933333;
            }
            else if (uVar32 < 0xf) {
              uVar34 = 0x3fa66666;
            }
            else {
              uVar34 = 0x3fc00000;
              if (0x12 < uVar32) {
                uVar34 = 0x3fd9999a;
              }
            }
            uVar32 = __strlen_chk(&local_260,0x80);
            if ((uVar32 < 0x80) && (uVar17 = *param_5, uVar17 < 0x280)) {
              *param_5 = uVar17 + 1;
              *(undefined8 *)(param_5 + (ulong)uVar17 * 0x2c + 4) = uVar18;
              *(undefined1 *)(param_5 + (ulong)uVar17 * 0x2c + 0xf) = 1;
              (param_5 + (ulong)uVar17 * 0x2c + 7)[0] = 0xffffffff;
              (param_5 + (ulong)uVar17 * 0x2c + 7)[1] = 0;
              *(ulong *)(param_5 + (ulong)uVar17 * 0x2c + 0xb) = CONCAT44(uVar34,uVar34);
              *(ulong *)(param_5 + (ulong)uVar17 * 0x2c + 9) =
                   CONCAT44(fVar41 + -10.0 + (float)((ulong)uVar40 >> 0x20),
                            fVar33 + -55.0 + (float)uVar40);
              lVar19 = __strlen_chk(&local_260,0x80);
              memcpy((void *)((long)param_5 + (ulong)uVar17 * 0xb0 + 0x3e),&local_260,lVar19 + 1);
              param_5[(ulong)uVar17 * 0x2c + 7] = 0xffa9adb6;
              if ((puVar30 == (uint *)0x0) && (uVar32 = (ulong)*param_5, uVar23 < uVar32)) {
                uVar26 = uVar32 - uVar23;
                if (1 < uVar26) {
                  uVar27 = uVar26 & 0xfffffffffffffffe;
                  puVar30 = param_5 + uVar23 * 0x2c + 0x3b;
                  uVar23 = uVar27 + uVar23;
                  uVar29 = uVar27;
                  do {
                    *(undefined1 *)(puVar30 + -0x2c) = 0;
                    uVar29 = uVar29 - 2;
                    *(undefined1 *)puVar30 = 0;
                    puVar30 = puVar30 + 0x58;
                  } while (uVar29 != 0);
                  if (uVar26 == uVar27) goto LAB_0016abec;
                }
                lVar19 = uVar32 - uVar23;
                puVar30 = param_5 + uVar23 * 0x2c + 0xf;
                do {
                  lVar19 = lVar19 + -1;
                  *(undefined1 *)puVar30 = 0;
                  puVar30 = puVar30 + 0x2c;
                } while (lVar19 != 0);
              }
              goto LAB_0016abec;
            }
          }
          goto LAB_0016aa3c;
        }
        puVar28[5] = (uint)fVar33;
        puVar28[6] = (uint)fVar41;
        *(undefined8 *)(puVar28 + 9) = uVar10;
        *(undefined8 *)(puVar28 + 7) = uVar9;
        if ((*(int *)(lVar19 + 8) == 0) || (*(char *)((long)puVar30 + 0x35) == '\0'))
        goto LAB_0016af10;
        puVar21 = (undefined1 *)((long)puVar28 + 0x2e);
        switch(*puVar30) {
        case 0x37:
          goto LAB_0016af10;
        case 0x38:
          break;
        case 0x39:
          break;
        case 0x3a:
          break;
        case 0x3b:
          break;
        case 0x3c:
          break;
        case 0x3d:
          break;
        case 0x3e:
          goto LAB_0016b5f4;
        case 0x3f:
          goto LAB_0016b5f4;
        default:
          *puVar21 = 0;
          goto LAB_0016b618;
        case 0x42:
          goto LAB_0016af10;
        case 0x45:
          goto LAB_0016b454;
        case 0x46:
          goto LAB_0016b454;
        case 0x47:
          goto LAB_0016b454;
        case 0x48:
LAB_0016b454:
          FUN_0016ba38((double)(int)puVar30[0xb] / 1000.0,puVar21,0xffffffffffffffff);
          goto LAB_0016b618;
        case 0x4b:
          break;
        case 0x4c:
          break;
        case 0x4d:
          break;
        case 0x4e:
          break;
        case 0x4f:
          break;
        case 0x50:
          break;
        case 0x51:
          break;
        case 0x52:
          break;
        case 0x53:
          break;
        case 0x54:
          break;
        case 0x55:
          break;
        case 0x56:
          break;
        case 0x57:
          break;
        case 0x58:
          break;
        case 0x59:
          break;
        case 0x5a:
          break;
        case 0x5b:
          break;
        case 0x6a:
        case 0x92:
          goto LAB_0016af10;
        case 0x6e:
          break;
        case 0x6f:
          break;
        case 0x70:
          goto LAB_0016af10;
        case 0x73:
          break;
        case 0x74:
LAB_0016af10:
          FUN_0016ba38((long)puVar28 + 0x2e,0xffffffffffffffff);
          goto LAB_0016b618;
        case 0x75:
        case 0x76:
          goto LAB_0016b344;
        case 0x77:
        case 0x78:
          goto LAB_0016b344;
        case 0x89:
        case 0x8a:
          goto LAB_0016b344;
        case 0x8b:
        case 0x8c:
          goto LAB_0016b304;
        case 0x8d:
        case 0x8e:
LAB_0016b304:
          FUN_0016ba38(puVar21,0xffffffffffffffff);
          goto LAB_0016b618;
        case 0x8f:
        case 0x90:
LAB_0016b344:
          FUN_0016ba38(puVar21,0xffffffffffffffff);
          goto LAB_0016b618;
        case 0x98:
          goto LAB_0016b5f4;
        case 0x99:
LAB_0016b5f4:
          FUN_0016ba38(puVar21,0xffffffffffffffff);
          goto LAB_0016b618;
        case 0x9b:
        }
        FUN_0016ba38(puVar21,0xffffffffffffffff);
LAB_0016b618:
        iVar16 = 0;
        uVar17 = *param_5;
        iVar1 = 0;
        if (uVar17 < 0x281) {
          iVar1 = 0x280 - uVar17;
        }
        lVar19 = (ulong)uVar17 * 0xb0;
        do {
          if (iVar1 == iVar16) goto LAB_0016aa3c;
          uVar35 = uVar17 + 1 + iVar16;
          iVar16 = iVar16 + 1;
          *param_5 = uVar35;
          *(undefined8 *)((long)param_5 + lVar19 + 0x10) = uVar18;
          *(undefined8 *)((long)param_5 + lVar19 + 0x2c) = uVar8;
          *(undefined8 *)((long)param_5 + lVar19 + 0x24) = uVar7;
          *(undefined1 *)((long)param_5 + lVar19 + 0x3e) = 0;
          *(undefined8 *)((long)param_5 + lVar19 + 0x1c) = 0xffffffff;
          *(undefined1 *)((long)param_5 + lVar19 + 0x3c) = 0;
          lVar19 = lVar19 + 0xb0;
        } while (iVar16 != 0x1b);
LAB_0016abec:
        uVar31 = uVar31 + 1;
      } while (uVar31 != 0x14);
      if ((*(char *)(param_3 + 8) == 'Y') && (*(char *)(param_3 + 10) == '\0')) {
        local_168 = 0;
        uStack_1d8 = 0;
        uStack_170 = 0;
        uStack_178 = 0;
        local_180 = 0;
        uStack_188 = 0;
        uStack_190 = 0;
        uStack_198 = 0;
        local_1a0 = 0;
        uStack_1a8 = 0;
        uStack_1b0 = 0;
        uStack_1b8 = 0;
        local_1c0 = 0;
        uStack_1c8 = 0;
        local_1e0 = DAT_0010f8a0;
        local_1d0 = 0xffffffffffffffff;
        iVar16 = nexus_rich_plus_layout(param_5,&local_1e0,param_4);
        if (iVar16 != 1) goto LAB_0016aa3c;
      }
      if (*(char *)(param_3 + 9) == '\0') {
        uVar17 = *param_5;
        uVar31 = (ulong)uVar17;
        if (uVar17 != 0) {
          if (uVar17 == 1) {
            uVar32 = 0;
          }
          else {
            uVar32 = uVar31 & 0xfffffffe;
            puVar30 = param_5 + 0x3b;
            uVar23 = uVar32;
            do {
              *(undefined1 *)(puVar30 + -0x2c) = 0;
              uVar23 = uVar23 - 2;
              *(undefined1 *)puVar30 = 0;
              puVar30 = puVar30 + 0x58;
            } while (uVar23 != 0);
            if (uVar32 == uVar31) goto LAB_0016b6f4;
          }
          lVar19 = uVar31 - uVar32;
          uVar18 = 1;
          puVar30 = param_5 + uVar32 * 0x2c + 0xf;
          do {
            lVar19 = lVar19 + -1;
            *(undefined1 *)puVar30 = 0;
            puVar30 = puVar30 + 0x2c;
          } while (lVar19 != 0);
          goto LAB_0016aa44;
        }
      }
LAB_0016b6f4:
      uVar18 = 1;
      goto LAB_0016aa44;
    }
  }
LAB_0016aa3c:
  uVar18 = 4;
LAB_0016aa44:
  if (*(long *)(lVar4 + 0x28) == local_b8) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar18);
}

/* ===== FUN_0016fc60 @ 0016fc60 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: stack */

void FUN_0016fc60(float param_1,float param_2,long param_3,ulong param_4)

{
  bool bVar1;
  uint uVar2;
  int *piVar3;
  code *pcVar4;
  ulong uVar5;
  long lVar6;
  bool bVar7;
  undefined8 uVar8;
  int iVar9;
  int iVar10;
  undefined8 uVar11;
  char *pcVar12;
  uint uVar13;
  ulong uVar14;
  long lVar15;
  ulong uVar16;
  undefined4 uVar17;
  float fVar18;
  float fVar19;
  undefined4 uVar20;
  float fVar21;
  float fVar22;
  float fVar23;
  int local_144;
  int local_140;
  byte local_13c [4];
  undefined4 local_138;
  undefined4 uStack_134;
  undefined4 local_130;
  undefined4 uStack_12c;
  undefined8 local_d8;
  undefined8 uStack_d0;
  undefined8 uStack_c8;
  undefined8 uStack_c0;
  undefined8 uStack_b8;
  long local_b0;
  undefined8 uStack_a8;
  long local_a0;
  
  lVar6 = tpidr_el0;
  local_a0 = *(long *)(lVar6 + 0x28);
  if ((DAT_00283338 != 0) && (DAT_00283338 != DAT_001a7d30)) {
    memset(&DAT_00283338,0,0x6b8);
  }
  uStack_a8 = 0;
  local_b0 = 0;
  uStack_b8 = 0;
  uStack_c0 = 0;
  uStack_c8 = 0;
  uStack_d0 = 0;
  local_d8 = DAT_0010f8e8;
  iVar9 = FUN_00191bf4(&local_d8);
  uVar13 = (uint)(iVar9 != 0) | (uint)((int)uStack_d0 != 0) << 1 | (uint)(uStack_d0._4_4_ != 0) << 2
           | (uint)(*(char *)(param_3 + 9) != '\0') << 3 | (uint)(DAT_00283984 != 0) << 4;
  if (((uVar13 != DAT_0022d81c) &&
      (uVar2 = DAT_002839f4 + 1, bVar1 = DAT_002839f4 < 0x30, DAT_0022d81c = uVar13,
      DAT_002839f4 = uVar2, bVar1)) && (DAT_001a7d00 != (code *)0x0)) {
    (*DAT_001a7d00)(DAT_001a7cc8,"script_port_camera","scope_api_enabled_live_menu_failed");
  }
  if (((iVar9 == 0) || ((int)uStack_d0 == 0)) ||
     ((uStack_d0._4_4_ == 0 || (*(char *)(param_3 + 9) != '\0')))) {
    if (DAT_00283988 != 0) {
      FUN_00191c34(5,0,0);
    }
    _DAT_00283988 = 0;
    uVar5 = DAT_00283948;
    if ((DAT_00283980 != 0) &&
       (iVar9 = FUN_0016d5cc(DAT_00283340,DAT_001a7d30), uVar5 = DAT_00283948, iVar9 != 0)) {
      FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_00283340);
      uVar5 = DAT_00283948;
    }
    goto LAB_0016fe20;
  }
  uVar5 = DAT_00283948;
  if ((DAT_00283984 != 0) || (iVar9 = FUN_0016d910(), uVar5 = DAT_00283948, iVar9 == 0))
  goto LAB_0016fe20;
  local_138 = CONCAT31(local_138._1_3_,1);
  if ((DAT_001a7d28 + 0x19dU < 0x1001) ||
     ((iVar9 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x19c,&local_138,1), uVar5 = DAT_00283948
      , iVar9 != 1 || ((char)local_138 != '\0')))) goto LAB_0016fe20;
  if (DAT_00283980 == 0) {
    iVar9 = (*DAT_001a7cf8)(DAT_001a7cc8,"edit_controls_ui");
    iVar10 = (*DAT_001a7cf8)(DAT_001a7cc8,"popup_generic");
    uVar13 = (uint)(iVar9 != 0) | (uint)(iVar10 != 0) << 1;
    if (uVar13 == 3) {
      iVar9 = FUN_0017a478();
      if (iVar9 != 0) goto LAB_0016febc;
      DAT_00283984 = 1;
      uVar13 = DAT_002839f4 + 1;
      bVar1 = 0x2f < DAT_002839f4;
      uVar5 = DAT_00283948;
      DAT_002839f4 = uVar13;
      if ((bVar1) || (DAT_001a7d00 == (code *)0x0)) goto LAB_0016fe20;
      pcVar12 = "native_ui_contract_failed";
      uVar13 = DAT_002839a8;
    }
    else {
      iVar9 = uVar13 + 1;
      uVar5 = DAT_00283948;
      if (((DAT_002839f0 == iVar9) ||
          (uVar2 = DAT_002839f4 + 1, bVar1 = 0x2f < DAT_002839f4, DAT_002839f0 = iVar9,
          DAT_002839f4 = uVar2, bVar1)) || (DAT_001a7d00 == (code *)0x0)) goto LAB_0016fe20;
      pcVar12 = "waiting_assets_slider_popup";
    }
    (*DAT_001a7d00)(DAT_001a7cc8,"script_port_camera",pcVar12,uVar13);
    uVar5 = DAT_00283948;
  }
  else {
LAB_0016febc:
    iVar9 = FUN_0016d5cc(DAT_00283340,DAT_001a7d30);
    uVar5 = DAT_00283948;
    if (iVar9 != 0) {
      if (DAT_00283940 != local_b0) {
        _DAT_00283988 = 0;
        DAT_00283940 = local_b0;
        DAT_00283970 = 0;
        uRam0000000000283958 = 0;
        DAT_00283950 = 0;
        uRam0000000000283968 = 0;
        _DAT_00283960 = 0;
        _DAT_00283990 = 0;
        DAT_00283998 = 0;
        DAT_002839a0 = 0;
      }
      DAT_002839e8 = uStack_a8;
      uRam00000000002839d0 = uStack_c0;
      _DAT_002839c8 = uStack_c8;
      fVar22 = param_1 / DAT_001e53fc;
      fVar23 = param_2 / DAT_001e53fc;
      uVar17 = NEON_fminnm(fVar22 / 620.0,fVar23 / 440.0);
      fVar21 = (float)NEON_fminnm(uVar17,0x3f800000);
      uRam00000000002839c0 = uStack_d0;
      _DAT_002839b8 = local_d8;
      lRam00000000002839e0 = local_b0;
      _DAT_002839d8 = uStack_b8;
      _DAT_00283988 = CONCAT44(1,DAT_00283988);
      DAT_002839ac = param_1;
      DAT_002839b0 = param_2;
      if (((ABS(fVar21) != INFINITY) && (!NAN(ABS(fVar21)))) && (0.0 < fVar21)) {
        DAT_002839b4 = fVar21;
        FUN_0016cf00(fVar21,fVar21,DAT_00283340);
        fVar18 = (float)NEON_fnmsub(param_1,0x3f000000,DAT_001e5400);
        fVar19 = (float)NEON_fnmsub(param_2,0x3f000000,DAT_001e5404);
        FUN_0016cf5c(fVar18 / DAT_001e53fc,fVar19 / DAT_001e53fc,DAT_00283340);
        if (DAT_00283988 == 0) {
          FUN_0017b3f0();
          iVar9 = FUN_0017b530(param_1,param_2,DAT_001e53fc,fVar21,&local_138);
          uVar5 = param_4;
          if (iVar9 != 0) {
            FUN_0017b5e8(local_138,uStack_134,local_130,uStack_12c,DAT_002833a0);
            uVar5 = param_4;
          }
        }
        else {
          FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_002833a0);
          fVar18 = 0.0;
          FUN_0017b5e8(0,0,fVar22 / fVar21,fVar23 / fVar21,DAT_00283418);
          FUN_0016cf70(0x3f266666,DAT_00283418);
          FUN_0017b5e8(0,0,0x44160000,0x43c80000,DAT_00283348);
          FUN_0017b5e8(0x43840000,0xc3280000,0x42400000,0x42280000,DAT_00283490);
          FUN_0017b5e8(0xc32f0000,0x43210000,0x43160000,0x42280000,DAT_00283508);
          FUN_0017b5e8(0,0x43210000,0x43160000,0x42280000,DAT_00283580);
          FUN_0017b5e8(0x432f0000,0x43210000,0x43160000,0x42280000,DAT_002835f8);
          uVar8 = DAT_00283508;
          uVar11 = FUN_0017b858(&DAT_0013ce4c + *(int *)(&DAT_0013ce4c + (long)(int)uStack_c8 * 4));
          FUN_0016cec0(uVar8,uVar11);
          FUN_0017b920(0xc2fa0000,0xc3290000,0x41c00000,&DAT_00283670,"CAMERA SETTINGS");
          if (DAT_00283948 - 1 < param_4) {
            fVar18 = (float)(param_4 - DAT_00283948) / 1000.0;
          }
          uVar14 = 0;
          bVar7 = false;
          lVar15 = 0x3b0;
          bVar1 = false;
          uVar17 = NEON_fmin(fVar18,0x3dcccccd);
          DAT_00283948 = param_4;
          do {
            iVar9 = FUN_0017bb9c(uVar14 & 0xffffffff);
            uVar5 = DAT_00283948;
            if (iVar9 == 0) goto LAB_0016fe20;
            local_13c[0] = 0;
            local_140 = 0;
            uVar16 = (&DAT_00283350)[uVar14];
            if (uVar16 + 0x141 < 0x1001) goto LAB_0016fe20;
            iVar9 = (*DAT_001a7ce8)(DAT_001a7cc8,(undefined1 *)(uVar16 + 0x140),local_13c,1);
            uVar5 = DAT_00283948;
            if (((iVar9 != 1) || (1 < local_13c[0])) || (uVar16 < 0xf18)) goto LAB_0016fe20;
            piVar3 = (int *)(uVar16 + 0xe8);
            iVar9 = (*DAT_001a7ce8)(DAT_001a7cc8,piVar3,&local_140,4);
            uVar5 = DAT_00283948;
            if (iVar9 != 1) goto LAB_0016fe20;
            if (local_13c[0] == 0) {
              iVar9 = *(int *)((long)&uStack_c8 + uVar14 * 4 + 4);
              if (local_140 != iVar9) {
                *piVar3 = iVar9;
                local_140 = iVar9;
              }
LAB_001702cc:
              uVar13 = 0;
              if ((&DAT_00283990)[uVar14] != 0) {
                bVar1 = true;
              }
            }
            else {
              uVar13 = (uint)local_13c[0];
              if ((&DAT_00283990)[uVar14] == 0) {
                if (((&DAT_00283950)[uVar14] - 1 < param_4) &&
                   (param_4 - (&DAT_00283950)[uVar14] < 200)) {
                  local_140 = 100;
                  if (uVar14 != 0) {
                    local_140 = 0;
                  }
                  *piVar3 = local_140;
                  local_13c[0] = 0;
                  *(undefined1 *)(uVar16 + 0x140) = 0;
                  bVar1 = true;
                  FUN_00191c34(1,uVar14 & 0xffffffff,local_140);
                  uVar13 = (uint)local_13c[0];
                  (&DAT_00283950)[uVar14] = 0;
                  *(int *)((long)&uStack_c8 + uVar14 * 4 + 4) = local_140;
                  if (local_13c[0] == 0) goto LAB_001702cc;
                }
                else {
                  (&DAT_00283950)[uVar14] = param_4;
                }
              }
            }
            pcVar4 = (code *)(DAT_001a7cd0 + 0x889940);
            (&DAT_00283990)[uVar14] = uVar13;
            (*pcVar4)(uVar17,uVar16);
            local_144 = 0;
            iVar9 = (*DAT_001a7ce8)(DAT_001a7cc8,piVar3,&local_144,4);
            uVar5 = DAT_00283948;
            if (iVar9 != 1) goto LAB_0016fe20;
            if ((local_144 != *(int *)((long)&uStack_c8 + uVar14 * 4 + 4)) &&
               (iVar9 = FUN_00191c34(1,uVar14 & 0xffffffff), iVar9 != 0)) {
              bVar7 = true;
              *(int *)((long)&uStack_c8 + uVar14 * 4 + 4) = local_144;
            }
            uVar5 = uVar14 & 0xffffffff;
            uVar20 = NEON_fmadd((float)uVar5,0x42340000,0xc2b40000);
            FUN_0016cf5c(0x43020000,uVar20,uVar16);
            if (uVar14 == 0) {
              FUN_00176a24((double)((float)uStack_c8._4_4_ / 100.0),&local_138,0x60,0x60,"%s: %.2fx"
                           ,&DAT_00139a93);
            }
            else {
              FUN_00176a24(&local_138,0x60,0x60,"%s: %d",
                           (long)&DAT_0013ce5c + (long)(int)(&DAT_0013ce5c)[uVar14],
                           *(undefined4 *)((long)&uStack_c8 + uVar14 * 4 + 4));
            }
            uVar14 = uVar14 + 1;
            uVar20 = NEON_fmadd((float)uVar5,0x42340000,0xc2d20000);
            FUN_0017b920(0xc37f0000,uVar20,0x41b00000,(long)&DAT_00283338 + lVar15,&local_138);
            lVar15 = lVar15 + 0x78;
          } while (uVar14 != 5);
          if (bVar1) {
            FUN_00191c34(5,0,0);
          }
          if ((((bVar7) &&
               (uVar13 = DAT_002839a4 + 1, bVar1 = DAT_002839a4 < 8, DAT_002839a4 = uVar13, bVar1))
              && (uVar13 = DAT_002839f4 + 1, bVar1 = DAT_002839f4 < 0x30, DAT_002839f4 = uVar13,
                 bVar1)) && (DAT_001a7d00 != (code *)0x0)) {
            (*DAT_001a7d00)(DAT_001a7cc8,"script_port_camera","slider_changed",0);
          }
          uRam00000000002839c0 = uStack_d0;
          _DAT_002839b8 = local_d8;
          uRam00000000002839d0 = uStack_c0;
          _DAT_002839c8 = uStack_c8;
          lRam00000000002839e0 = local_b0;
          _DAT_002839d8 = uStack_b8;
          DAT_002839e8 = uStack_a8;
          uVar5 = DAT_00283948;
        }
      }
    }
  }
LAB_0016fe20:
  DAT_00283948 = uVar5;
  if (*(long *)(lVar6 + 0x28) != local_a0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_00170db4 @ 00170db4 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00170db4(long param_1)

{
  uint uVar1;
  ulong uVar2;
  char cVar3;
  long lVar4;
  bool bVar5;
  code *pcVar6;
  int iVar7;
  long lVar8;
  char cVar9;
  bool bVar10;
  char cVar11;
  int iVar12;
  int iVar13;
  undefined8 uVar14;
  ulong uVar15;
  ulong uVar16;
  undefined8 uVar17;
  bool bVar18;
  undefined4 uVar19;
  ulong *puVar20;
  long lVar21;
  undefined4 uVar22;
  float fVar23;
  float fVar24;
  long local_190;
  float local_188;
  float fStack_184;
  float local_180;
  undefined8 local_178;
  undefined8 uStack_170;
  undefined8 uStack_168;
  undefined8 uStack_160;
  undefined8 uStack_158;
  undefined8 local_150;
  undefined8 uStack_148;
  undefined8 uStack_140;
  undefined8 uStack_138;
  undefined8 local_130;
  undefined8 uStack_128;
  undefined8 uStack_120;
  undefined8 uStack_118;
  undefined8 local_110;
  undefined8 uStack_108;
  undefined8 local_100;
  undefined8 uStack_f8;
  undefined8 local_f0;
  undefined8 uStack_e8;
  undefined8 local_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  undefined8 uStack_c8;
  undefined8 local_c0;
  undefined8 uStack_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  long local_a0;
  
  lVar4 = tpidr_el0;
  local_a0 = *(long *)(lVar4 + 0x28);
  if (DAT_002844b8 != 0 && DAT_002844b8 != DAT_001a7d30) {
    DAT_002844d0 = 0;
    _DAT_002844d4 = 0;
    uRam00000000002843f8 = 0;
    _DAT_002843f0 = 0;
    uRam0000000000284408 = 0;
    _DAT_00284400 = 0;
    uRam0000000000284418 = 0;
    _DAT_00284410 = 0;
    uRam0000000000284428 = 0;
    _DAT_00284420 = 0;
    uRam0000000000284438 = 0;
    _DAT_00284430 = 0;
    uRam0000000000284448 = 0;
    _DAT_00284440 = 0;
    uRam0000000000284458 = 0;
    _DAT_00284450 = 0;
    uRam0000000000284468 = 0;
    _DAT_00284460 = 0;
    uRam0000000000284478 = 0;
    _DAT_00284470 = 0;
    uRam0000000000284488 = 0;
    _DAT_00284480 = 0;
    uRam0000000000284498 = 0;
    _DAT_00284490 = 0;
    uRam00000000002844a8 = 0;
    _DAT_002844a0 = 0;
    DAT_002844b8 = 0;
    DAT_002844b0 = 0;
    DAT_002844c8 = 0;
    DAT_002844cc = 0;
    DAT_002844c0 = 0;
    uRam00000000002843e8 = 0;
    DAT_002843e0 = 0;
  }
  local_190 = 0;
  iVar12 = FUN_0014d288(&local_190);
  cVar9 = false;
  cVar3 = *(char *)(param_1 + 9);
  if (((DAT_0028398c != 0) && (DAT_00283980 != 0)) && (DAT_00283988 == 0)) {
    if ((DAT_00283338 == DAT_001a7d30) && (iVar13 = FUN_0017c598(DAT_002833a0), iVar13 != 0)) {
      local_178 = CONCAT71(local_178._1_7_,0xff);
      if (0x1000 < DAT_002833a0 + 0x49U) {
        iVar13 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_002833a0 + 0x48,&local_178,1);
        cVar9 = iVar13 == 1 && (char)local_178 == '\x01';
        goto LAB_00170eb4;
      }
    }
    cVar9 = false;
  }
LAB_00170eb4:
  iVar7 = DAT_0028398c;
  iVar13 = DAT_00283988;
  cVar11 = FUN_0017ce78();
  lVar8 = local_190;
  bVar10 = false;
  if (cVar11 != '\0') {
    cVar9 = cVar9 + '\x01';
  }
  if ((iVar12 != 0) && (cVar3 == '\0')) {
    bVar10 = (iVar7 == 0 || iVar13 == 0) && local_190 != 0;
  }
  iVar12 = FUN_0017ce78();
  if (iVar12 != 0) {
    if ((((DAT_0028398c == 0) || (DAT_00283980 == 0)) || (DAT_00283988 != 0)) ||
       ((DAT_00283338 != DAT_001a7d30 || (iVar12 = FUN_0017c598(DAT_002833a0), iVar12 == 0)))) {
LAB_00170f88:
      uVar19 = 0x42c00000;
    }
    else {
      local_178 = CONCAT71(local_178._1_7_,0xff);
      if (DAT_002833a0 + 0x49U < 0x1001) goto LAB_00170f88;
      iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_002833a0 + 0x48,&local_178,1);
      if ((char)local_178 != '\x01' || iVar12 != 1) goto LAB_00170f88;
      uVar19 = 0x43120000;
    }
    lVar21 = 0;
    bVar18 = true;
    while( true ) {
      puVar20 = &DAT_00284220 + lVar21 * 0x1b;
      FUN_0016cf5c(0,0,*puVar20);
      FUN_0016cf00(0x3f800000,0x3f800000,*puVar20);
      FUN_0016cf84(*puVar20,&local_188);
      if (((ABS(local_180 - local_188) == INFINITY) || (NAN(ABS(local_180 - local_188)))) ||
         (fVar24 = local_180 - local_188, fVar24 <= 0.0)) break;
      uVar15 = *puVar20 + 0x10;
      if (((uVar15 < 0x1000) || ((*puVar20 & 0xfffffffffffffff0) == 0xffffffffffffffe0)) ||
         (iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar15,&local_178,0x10), iVar12 != 1)) break;
      if ((ABS((float)local_178) == INFINITY) || (NAN(ABS((float)local_178)))) break;
      if ((ABS(local_178._4_4_) == INFINITY) || (NAN(ABS(local_178._4_4_)))) break;
      if ((ABS((float)uStack_170) == INFINITY) || (NAN(ABS((float)uStack_170)))) break;
      if ((ABS(uStack_170._4_4_) == INFINITY) ||
         ((((NAN(ABS(uStack_170._4_4_)) || (local_178._4_4_ != 0.0)) || ((float)uStack_170 != 0.0))
          || ((((float)local_178 <= 0.0 || (uStack_170._4_4_ <= 0.0)) ||
              (((float)local_178 != 128.0 && (float)local_178 < 128.0 == NAN((float)local_178) ||
               (uStack_170._4_4_ != 128.0 && uStack_170._4_4_ < 128.0 == NAN(uStack_170._4_4_)))))))
         )) break;
      fVar24 = ((float)local_178 * 76.0 * DAT_0022d818) / (fVar24 * DAT_001e53fc);
      if ((((ABS(fVar24) == INFINITY) || (NAN(ABS(fVar24)))) || (fVar24 <= 0.0)) ||
         (fVar24 != 8.0 && fVar24 < 8.0 == NAN(fVar24))) break;
      FUN_0016cf00(fVar24,fVar24,*puVar20);
      FUN_0016cf84(*puVar20,&local_188);
      uVar22 = NEON_fmadd((float)lVar21,0x42a80000,0x41a00000);
      fVar24 = (float)NEON_fnmsub(uVar22,DAT_0022d818,DAT_001e5400);
      fVar23 = (float)NEON_fnmsub(uVar19,DAT_0022d818,DAT_001e5404);
      FUN_0016cf5c(fVar24 / DAT_001e53fc - local_188,fVar23 / DAT_001e53fc - fStack_184,*puVar20);
      lVar21 = 1;
      bVar5 = !bVar18;
      bVar18 = false;
      if (bVar5) break;
    }
  }
  if ((bVar10) && (iVar12 = FUN_0016d910(), iVar12 != 0)) {
    local_178 = CONCAT71(local_178._1_7_,1);
    if ((DAT_001a7d28 + 0x19dU < 0x1001) ||
       (((iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x19c,&local_178,1),
         pcVar6 = DAT_001a7cf8, uVar17 = DAT_001a7cc8, iVar12 != 1 || ((char)local_178 != '\0')) ||
        (DAT_002844cc != 0)))) goto LAB_001711fc;
    if (DAT_002844c8 == 0) {
      _DAT_002844d4 = 1;
      uVar14 = nexus_rich_asset(5);
      iVar12 = (*pcVar6)(uVar17,uVar14);
      if ((iVar12 != 0) && (iVar12 = FUN_00172da8(), iVar12 != 0)) {
        _DAT_002844d4 = 2;
        nexus_rich_asset(5);
        uVar15 = FUN_0016ce48();
        iVar12 = FUN_0016dc20(uVar15,5,0);
        if (iVar12 != 0) {
          local_188 = 0.0;
          local_178 = 1;
          if ((((((0xfff < uVar15 + 0x38) && ((uVar15 & 0xfffffffffffffff8) != 0xffffffffffffffc0))
                && (iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar15 + 0x38,&local_178,8), iVar12 == 1))
               && ((local_178 == 0 && (0xfff < uVar15 + 0x40)))) &&
              ((uVar15 & 0xfffffffffffffffc) != 0xffffffffffffffbc)) &&
             ((iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar15 + 0x40,&local_188,4), iVar12 == 1 &&
              (local_188 == -NAN)))) {
            FUN_0016ce84(uVar15,0);
            uVar16 = FUN_0016cdf0(0x260);
            if (uVar16 != 0) {
              FUN_0016ce34();
              local_178 = 0;
              if (uVar16 + 8 >> 3 < 0x201) {
                bVar10 = false;
              }
              else {
                iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar16,&local_178,8);
                bVar10 = iVar12 == 1;
              }
              bVar18 = false;
              if (0xfff < local_178) {
                bVar18 = bVar10;
              }
              uVar2 = local_178;
              if (!(bool)(bVar18 & (local_178 & 7) == 0)) {
                uVar2 = 0;
              }
              if (uVar2 == DAT_001a7cd0 + 0x11c0a48U) {
                local_188 = 0.0;
                local_178 = 1;
                if ((((0xfff < uVar16 + 0x38) &&
                     ((uVar16 & 0xfffffffffffffff8) != 0xffffffffffffffc0)) &&
                    (iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar16 + 0x38,&local_178,8), iVar12 == 1)
                    ) && ((((local_178 == 0 && (0xfff < uVar16 + 0x40)) &&
                           (((uVar16 & 0xfffffffffffffffc) != 0xffffffffffffffbc &&
                            ((iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar16 + 0x40,&local_188,4),
                             iVar12 == 1 && (local_188 == -NAN)))))) &&
                          (iVar12 = FUN_0014e5ac(uVar16), iVar12 != 0)))) {
                  uStack_a8 = 0;
                  local_b0 = 0;
                  uStack_b8 = 0;
                  local_c0 = 0;
                  uStack_c8 = 0;
                  local_d0 = 0;
                  uStack_d8 = 0;
                  local_e0 = 0;
                  uStack_e8 = 0;
                  local_f0 = 0;
                  uStack_f8 = 0;
                  local_100 = 0;
                  uStack_108 = 0;
                  local_110 = 0;
                  uStack_168 = 0;
                  uStack_170 = 0;
                  uStack_158 = 0;
                  uStack_160 = 0;
                  uStack_148 = 0;
                  local_150 = 0;
                  uStack_138 = 0;
                  uStack_140 = 0;
                  uStack_128 = 0;
                  local_130 = 0;
                  uStack_118 = 0;
                  uStack_120 = 0;
                  local_178 = uVar16;
                  iVar12 = FUN_0017c42c(&local_178,uVar15,0x3d4);
                  if (iVar12 != 0) {
                    uVar17 = FUN_0017b858("BATTLE");
                    FUN_0016cec0(uVar16,uVar17);
                    FUN_0016cf5c(0xc61c3c00,0xc61c3c00,uVar16);
                    FUN_0016cfb8(DAT_001a7d28,uVar16);
                    iVar12 = FUN_0016d5cc(uVar16,DAT_001a7d30);
                    if ((iVar12 != 0) && (iVar12 = FUN_0016e464(uVar16,1), iVar12 != 0)) {
                      uRam0000000000284488 = local_d0;
                      _DAT_00284480 = uStack_d8;
                      uRam0000000000284498 = local_c0;
                      _DAT_00284490 = uStack_c8;
                      DAT_002844c8 = 1;
                      uRam00000000002844a8 = local_b0;
                      _DAT_002844a0 = uStack_b8;
                      uRam0000000000284448 = local_110;
                      _DAT_00284440 = uStack_118;
                      uRam0000000000284458 = local_100;
                      _DAT_00284450 = uStack_108;
                      uRam0000000000284468 = local_f0;
                      _DAT_00284460 = uStack_f8;
                      uRam0000000000284478 = local_e0;
                      _DAT_00284470 = uStack_e8;
                      uRam0000000000284408 = local_150;
                      _DAT_00284400 = uStack_158;
                      uRam0000000000284418 = uStack_140;
                      _DAT_00284410 = uStack_148;
                      DAT_002844b0 = uStack_a8;
                      DAT_002844b8 = DAT_001a7d30;
                      uRam0000000000284428 = local_130;
                      _DAT_00284420 = uStack_138;
                      uRam0000000000284438 = uStack_120;
                      _DAT_00284430 = uStack_128;
                      uRam00000000002843e8 = uStack_170;
                      DAT_002843e0 = local_178;
                      uRam00000000002843f8 = uStack_160;
                      _DAT_002843f0 = uStack_168;
                      _DAT_002844d4 = 3;
                      uVar1 = DAT_001a7d20 + 1;
                      bVar10 = DAT_001a7d20 < 0x40;
                      DAT_001a7d20 = uVar1;
                      if ((bVar10) && (DAT_001a7d00 != (code *)0x0)) {
                        (*DAT_001a7d00)(DAT_001a7cc8,"battle_menu_launcher","native_button_attached"
                                        ,0);
                      }
                      goto LAB_001712c0;
                    }
                  }
                }
              }
            }
          }
        }
      }
      pcVar6 = DAT_001a7cf8;
      uVar17 = DAT_001a7cc8;
      uVar14 = nexus_rich_asset(5);
      iVar12 = (*pcVar6)(uVar17,uVar14);
      if (iVar12 != 0) {
        DAT_002844cc = 1;
        uVar1 = DAT_001a7d20 + 1;
        bVar10 = DAT_001a7d20 < 0x40;
        DAT_001a7d20 = uVar1;
        if ((bVar10) && (DAT_001a7d00 != (code *)0x0)) {
          (*DAT_001a7d00)(DAT_001a7cc8,"battle_menu_launcher","native_button_contract_failed",
                          _DAT_002844d4);
        }
      }
LAB_00171558:
      FUN_0017cfbc();
      goto LAB_00171244;
    }
LAB_001712c0:
    iVar12 = FUN_0016d5cc(DAT_002843e0,DAT_001a7d30);
    if (iVar12 != 0) {
      local_178 = CONCAT71(local_178._1_7_,0xff);
      if (((0x1000 < DAT_002843e0 + 0x49) &&
          (iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_002843e0 + 0x48,&local_178,1), iVar12 == 1)) &&
         ((char)local_178 == '\x01')) {
        fVar24 = (float)NEON_fnmsub(DAT_0022d818,0x42960000,DAT_001e5400);
        fVar23 = (float)NEON_fnmsub((float)(byte)(cVar9 * '2' + 0x60) + 21.0,DAT_0022d818,
                                    DAT_001e5404);
        iVar12 = FUN_0017b5e8(fVar24 / DAT_001e53fc,fVar23 / DAT_001e53fc,
                              (DAT_0022d818 * 110.0) / DAT_001e53fc,
                              (DAT_0022d818 * 42.0) / DAT_001e53fc,DAT_002843e0);
        if (iVar12 != 0) {
          DAT_002844d0 = 1;
          DAT_002844c0 = lVar8;
          goto LAB_00171244;
        }
        DAT_002844cc = 1;
        goto LAB_00171558;
      }
    }
    DAT_002844c0 = 0;
    DAT_002844cc = (int)DAT_0010f860;
    DAT_002844d0 = (undefined4)((ulong)DAT_0010f860 >> 0x20);
    if ((DAT_002844c8 == 0) || (DAT_002843e0 == 0)) goto LAB_00171244;
    iVar12 = FUN_0016d5cc(DAT_002843e0,DAT_001a7d30);
  }
  else {
LAB_001711fc:
    DAT_002844d0 = 0;
    DAT_002844c0 = 0;
    if ((DAT_002844c8 == 0) || (DAT_002843e0 == 0)) goto LAB_00171244;
    iVar12 = FUN_0016d5cc(DAT_002843e0,DAT_001a7d30);
  }
  if (iVar12 != 0) {
    FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_002843e0);
  }
LAB_00171244:
  if (*(long *)(lVar4 + 0x28) == local_a0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00171d68 @ 00171d68 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00171d68(float param_1,float param_2,ulong param_3)

{
  ulong uVar1;
  char *pcVar2;
  long lVar3;
  bool bVar4;
  undefined8 uVar5;
  ulong uVar6;
  bool bVar7;
  int iVar8;
  undefined4 uVar9;
  ulong uVar10;
  undefined8 uVar11;
  ulong uVar12;
  uint uVar13;
  undefined4 uVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  short local_254 [2];
  undefined8 local_250;
  undefined8 uStack_248;
  undefined8 uStack_240;
  undefined8 local_238;
  undefined8 uStack_230;
  undefined8 local_228;
  undefined8 uStack_220;
  undefined8 local_218;
  undefined8 uStack_210;
  undefined8 local_208;
  undefined8 uStack_200;
  undefined8 local_1f8;
  float local_1f0;
  float local_1ec;
  ulong local_1e0 [40];
  long local_a0;
  
  lVar3 = tpidr_el0;
  local_a0 = *(long *)(lVar3 + 0x28);
  iVar8 = FUN_0016d910();
  if (iVar8 == 0) goto LAB_00171dd0;
  local_1e0[0] = CONCAT71(local_1e0[0]._1_7_,1);
  if (((DAT_001a7d28 + 0x19dU < 0x1001) ||
      (iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x19c,local_1e0,1), iVar8 != 1)) ||
     ((char)local_1e0[0] != '\0')) goto LAB_00171dd0;
  local_1f8 = 0;
  uStack_200 = 0;
  local_208 = 0;
  uStack_210 = 0;
  local_218 = 0;
  uStack_220 = 0;
  local_228 = 0;
  uStack_230 = 0;
  local_238 = 0;
  uStack_240 = 0;
  uStack_248 = 0;
  local_250 = DAT_0010f850;
  iVar8 = FUN_00191c90(&local_250);
  if (iVar8 == 0) {
    uStack_248 = 0;
    local_250 = 0;
    local_238 = 0;
    uStack_240 = 0;
    local_228 = 0;
    uStack_230 = 0;
    local_218 = 0;
    uStack_220 = 0;
    local_208 = 0;
    uStack_210 = 0;
    local_1f8 = 0;
    uStack_200 = 0;
  }
  uVar10 = FUN_0017ec28();
  uVar12 = DAT_001a7d30;
  if ((DAT_002844e8 != 0) && (DAT_002844e8 != DAT_001a7d30)) {
    DAT_00284648 = 0;
    DAT_002844f0 = 0;
    DAT_002844e8 = 0;
    DAT_00284500 = 0;
    DAT_002844f8 = 0;
    uRam0000000000284510 = 0;
    _DAT_00284508 = 0;
    uRam0000000000284520 = 0;
    _DAT_00284518 = 0;
    uRam0000000000284530 = 0;
    _DAT_00284528 = 0;
    uRam0000000000284540 = 0;
    _DAT_00284538 = 0;
    uRam0000000000284550 = 0;
    _DAT_00284548 = 0;
    uRam0000000000284560 = 0;
    _DAT_00284558 = 0;
    uRam0000000000284570 = 0;
    _DAT_00284568 = 0;
    uRam0000000000284580 = 0;
    _DAT_00284578 = 0;
    uRam0000000000284590 = 0;
    _DAT_00284588 = 0;
    uRam00000000002845a0 = 0;
    _DAT_00284598 = 0;
    uRam00000000002845b0 = 0;
    _DAT_002845a8 = 0;
    uRam00000000002845c0 = 0;
    _DAT_002845b8 = 0;
    uRam00000000002845d0 = 0;
    _DAT_002845c8 = 0;
    uRam00000000002845e0 = 0;
    _DAT_002845d8 = 0;
    uRam00000000002845f0 = 0;
    _DAT_002845e8 = 0;
    uRam0000000000284600 = 0;
    _DAT_002845f8 = 0;
    uRam0000000000284610 = 0;
    _DAT_00284608 = 0;
    uRam0000000000284620 = 0;
    _DAT_00284618 = 0;
    uRam0000000000284630 = 0;
    _DAT_00284628 = 0;
    uRam0000000000284640 = 0;
    _DAT_00284638 = 0;
  }
  if (uVar10 == 0) {
    if (DAT_002844f8 != 0) {
      uVar10 = DAT_002844f0;
      if (DAT_002844f0 != 0) {
        uVar13 = 0;
        do {
          if ((uVar10 == uVar12) || (0xf < uVar13)) goto LAB_0017222c;
          local_1e0[0] = 0;
          if (uVar10 + 0x40 >> 3 < 0x201) {
            bVar7 = false;
          }
          else {
            iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar10 + 0x38,local_1e0,8);
            bVar7 = iVar8 == 1;
          }
          uVar6 = local_1e0[0];
          bVar4 = false;
          if (0xfff < local_1e0[0]) {
            bVar4 = bVar7;
          }
          bVar4 = (bool)(bVar4 & (local_1e0[0] & 7) == 0);
          uVar1 = local_1e0[0];
          if (!bVar4) {
            uVar1 = 0;
          }
          iVar8 = FUN_0016d5cc(uVar10,uVar1);
          if (iVar8 == 0) goto LAB_001724f4;
          uVar13 = uVar13 + 1;
          uVar10 = uVar6;
        } while (bVar4);
        uVar10 = 0;
      }
LAB_0017222c:
      uVar6 = DAT_002844f0;
      if (uVar10 == uVar12) {
        uVar12 = DAT_002844f8;
        if (DAT_002844f8 != 0) {
          uVar13 = 0;
          do {
            if ((uVar12 == uVar6) || (0xf < uVar13)) goto LAB_001722d0;
            local_1e0[0] = 0;
            if (uVar12 + 0x40 >> 3 < 0x201) {
              bVar7 = false;
            }
            else {
              iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar12 + 0x38,local_1e0,8);
              bVar7 = iVar8 == 1;
            }
            uVar10 = local_1e0[0];
            bVar4 = false;
            if (0xfff < local_1e0[0]) {
              bVar4 = bVar7;
            }
            bVar4 = (bool)(bVar4 & (local_1e0[0] & 7) == 0);
            uVar1 = local_1e0[0];
            if (!bVar4) {
              uVar1 = 0;
            }
            iVar8 = FUN_0016d5cc(uVar12,uVar1);
            if (iVar8 == 0) goto LAB_001724f4;
            uVar13 = uVar13 + 1;
            uVar12 = uVar10;
          } while (bVar4);
          uVar12 = 0;
        }
LAB_001722d0:
        if (uVar12 == uVar6) goto LAB_0017238c;
      }
    }
  }
  else {
    if ((DAT_002844f0 != 0) && (DAT_002844f0 != uVar10)) {
      DAT_00284648 = 0;
      DAT_002844f0 = 0;
      DAT_002844e8 = 0;
      DAT_00284500 = 0;
      DAT_002844f8 = 0;
      uRam0000000000284510 = 0;
      _DAT_00284508 = 0;
      uRam0000000000284520 = 0;
      _DAT_00284518 = 0;
      uRam0000000000284530 = 0;
      _DAT_00284528 = 0;
      uRam0000000000284540 = 0;
      _DAT_00284538 = 0;
      uRam0000000000284550 = 0;
      _DAT_00284548 = 0;
      uRam0000000000284560 = 0;
      _DAT_00284558 = 0;
      uRam0000000000284570 = 0;
      _DAT_00284568 = 0;
      uRam0000000000284580 = 0;
      _DAT_00284578 = 0;
      uRam0000000000284590 = 0;
      _DAT_00284588 = 0;
      uRam00000000002845a0 = 0;
      _DAT_00284598 = 0;
      uRam00000000002845b0 = 0;
      _DAT_002845a8 = 0;
      uRam00000000002845c0 = 0;
      _DAT_002845b8 = 0;
      uRam00000000002845d0 = 0;
      _DAT_002845c8 = 0;
      uRam00000000002845e0 = 0;
      _DAT_002845d8 = 0;
      uRam00000000002845f0 = 0;
      _DAT_002845e8 = 0;
      uRam0000000000284600 = 0;
      _DAT_002845f8 = 0;
      uRam0000000000284610 = 0;
      _DAT_00284608 = 0;
      uRam0000000000284620 = 0;
      _DAT_00284618 = 0;
      uRam0000000000284630 = 0;
      _DAT_00284628 = 0;
      uRam0000000000284640 = 0;
      _DAT_00284638 = 0;
    }
    if ((((byte)uStack_248 >> 1 & 1) == 0) || (uStack_248._4_4_ != 0)) {
      if ((DAT_002844f8 != 0) && (uVar10 == DAT_002844f0)) {
        uVar13 = 0;
        uVar12 = DAT_002844f8;
        do {
          if ((uVar12 == uVar10) || (0xf < uVar13)) goto LAB_00172380;
          local_1e0[0] = 0;
          if (uVar12 + 0x40 >> 3 < 0x201) {
            bVar7 = false;
          }
          else {
            iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar12 + 0x38,local_1e0,8);
            bVar7 = iVar8 == 1;
          }
          uVar6 = local_1e0[0];
          bVar4 = false;
          if (0xfff < local_1e0[0]) {
            bVar4 = bVar7;
          }
          bVar4 = (bool)(bVar4 & (local_1e0[0] & 7) == 0);
          uVar1 = local_1e0[0];
          if (!bVar4) {
            uVar1 = 0;
          }
          iVar8 = FUN_0016d5cc(uVar12,uVar1);
          if (iVar8 == 0) goto LAB_001724f4;
          uVar13 = uVar13 + 1;
          uVar12 = uVar6;
        } while (bVar4);
        uVar12 = 0;
LAB_00172380:
        if (uVar12 == uVar10) {
LAB_0017238c:
          FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_002844f8);
        }
      }
    }
    else if ((int)DAT_00284648 == 0) {
      if (DAT_002844f8 == 0) {
        iVar8 = (*DAT_001a7cf8)(DAT_001a7cc8,"popover_text_left");
        if ((iVar8 != 0) && (iVar8 = FUN_00172da8(), iVar8 != 0)) {
          DAT_002844e8 = DAT_001a7d30;
          DAT_002844f0 = uVar10;
          DAT_002844f8 = FUN_0016ce48("popover_text_left");
          iVar8 = FUN_0016debc(DAT_002844f8,0x11ad208);
          if ((iVar8 != 0) && (iVar8 = FUN_0016ddc8(DAT_002844f8), iVar8 != 0)) {
            FUN_0016ce84(DAT_002844f8,0);
            DAT_00284500 = FUN_0016ce98(DAT_002844f8,&DAT_0013784a);
            iVar8 = FUN_0016debc(DAT_00284500,0x11abad8);
            if ((iVar8 != 0) && (iVar8 = FUN_0016e020(DAT_002844f8,DAT_00284500), iVar8 != 0)) {
              local_1e0[0] = local_1e0[0] & 0xffffffffffff0000;
              local_1f0 = (float)((uint)local_1f0 & 0xffffff00);
              if (((0xfff < DAT_00284500 + 0xb0) &&
                  (((((DAT_00284500 & 0xfffffffffffffffe) != 0xffffffffffffff4e &&
                     (iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00284500 + 0xb0,local_1e0,2),
                     iVar8 == 1)) && (0 < (short)local_1e0[0])) &&
                   (((short)local_1e0[0] < 0x101 && (0x1000 < DAT_00284500 + 0x91)))))) &&
                 ((iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00284500 + 0x90,&local_1f0,1),
                  iVar8 == 1 && (local_1f0._0_1_ < 2)))) {
                local_1e0[0] = CONCAT62(local_1e0[0]._2_6_,0xe);
                local_1f0 = (float)CONCAT31(local_1f0._1_3_,1);
                *(undefined2 *)(DAT_00284500 + 0xb0) = 0xe;
                *(undefined1 *)(DAT_00284500 + 0x90) = 1;
                FUN_0016cf00(0x3f800000,0x3f800000,DAT_002844f8);
                FUN_0016cf00(0x3f800000,0x3f800000,DAT_00284500);
                FUN_0016ce20(DAT_002844f8,0);
                FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_002844f8);
                FUN_0016cfa4(uVar10,DAT_002844f8);
                iVar8 = FUN_0016d800(DAT_002844f8,uVar10);
                if (iVar8 != 0) {
                  uVar12 = DAT_002844f8;
                  if (DAT_002844f8 != 0) goto LAB_00171f60;
                  goto LAB_00171fec;
                }
              }
            }
          }
          DAT_00284648 = CONCAT44(DAT_00284648._4_4_,1);
        }
      }
      else {
LAB_00171f60:
        uVar13 = 0;
        uVar12 = DAT_002844f8;
        do {
          if ((uVar12 == uVar10) || (0xf < uVar13)) goto LAB_00171fec;
          local_1e0[0] = 0;
          if (uVar12 + 0x40 >> 3 < 0x201) {
            bVar7 = false;
          }
          else {
            iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar12 + 0x38,local_1e0,8);
            bVar7 = iVar8 == 1;
          }
          uVar6 = local_1e0[0];
          bVar4 = false;
          if (0xfff < local_1e0[0]) {
            bVar4 = bVar7;
          }
          bVar4 = (bool)(bVar4 & (local_1e0[0] & 7) == 0);
          uVar1 = local_1e0[0];
          if (!bVar4) {
            uVar1 = 0;
          }
          iVar8 = FUN_0016d5cc(uVar12,uVar1);
          if (iVar8 == 0) goto LAB_001723a4;
          uVar13 = uVar13 + 1;
          uVar12 = uVar6;
        } while (bVar4);
        uVar12 = 0;
LAB_00171fec:
        if ((uVar12 == uVar10) && (iVar8 = FUN_0016d800(DAT_00284500,DAT_002844f8), iVar8 != 0)) {
          uVar14 = uStack_240._4_4_;
          uVar9 = FUN_0014cd5c();
          FUN_0017f1e4(local_1e0,uVar14,&uStack_230,uVar9);
          iVar8 = strcmp((char *)local_1e0,&DAT_00284508);
          if (iVar8 != 0) {
            FUN_0016ced8(&local_1f0,local_1e0);
            FUN_0016ceac(DAT_00284500,&local_1f0);
            (*(code *)(DAT_001a7cd0 + 0x66ad48))(&local_1f0);
            FUN_00176a24(&DAT_00284508,0x140,0x140,&DAT_001343dc,local_1e0);
            FUN_0016ceec(DAT_00284500,0xffffffff);
          }
          local_254[0] = 0;
          if ((((0xfff < DAT_00284500 + 0xb0) &&
               ((DAT_00284500 & 0xfffffffffffffffe) != 0xffffffffffffff4e)) &&
              (iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00284500 + 0xb0,local_254,2), iVar8 == 1))
             && ((0 < local_254[0] && (local_254[0] < 0x101)))) {
            FUN_0016cf00(0x3f800000,0x3f800000,DAT_00284500);
            FUN_0016cf5c(0,0,DAT_002844f8);
            FUN_0016cf5c(0,0,DAT_00284500);
            FUN_0016cf84(DAT_00284500,&local_1f0);
            if (((ABS(local_1f0) != INFINITY) &&
                ((!NAN(ABS(local_1f0)) && (ABS(local_1ec) != INFINITY)))) && (!NAN(ABS(local_1ec))))
            {
              FUN_0016cf5c(120.0 - local_1f0,90.0 - local_1ec,DAT_00284500);
            }
          }
        }
        else {
LAB_001723a4:
          DAT_00284648 = 0;
          DAT_002844f0 = 0;
          DAT_002844e8 = 0;
          DAT_00284500 = 0;
          DAT_002844f8 = 0;
          uRam0000000000284510 = 0;
          _DAT_00284508 = 0;
          uRam0000000000284520 = 0;
          _DAT_00284518 = 0;
          uRam0000000000284530 = 0;
          _DAT_00284528 = 0;
          uRam0000000000284540 = 0;
          _DAT_00284538 = 0;
          uRam0000000000284550 = 0;
          _DAT_00284548 = 0;
          uRam0000000000284560 = 0;
          _DAT_00284558 = 0;
          uRam0000000000284570 = 0;
          _DAT_00284568 = 0;
          uRam0000000000284580 = 0;
          _DAT_00284578 = 0;
          uRam0000000000284590 = 0;
          _DAT_00284588 = 0;
          uRam00000000002845a0 = 0;
          _DAT_00284598 = 0;
          uRam00000000002845b0 = 0;
          _DAT_002845a8 = 0;
          uRam00000000002845c0 = 0;
          _DAT_002845b8 = 0;
          uRam00000000002845d0 = 0;
          _DAT_002845c8 = 0;
          uRam00000000002845e0 = 0;
          _DAT_002845d8 = 0;
          uRam00000000002845f0 = 0;
          _DAT_002845e8 = 0;
          uRam0000000000284600 = 0;
          _DAT_002845f8 = 0;
          uRam0000000000284610 = 0;
          _DAT_00284608 = 0;
          uRam0000000000284620 = 0;
          _DAT_00284618 = 0;
          uRam0000000000284630 = 0;
          _DAT_00284628 = 0;
          uRam0000000000284640 = 0;
          _DAT_00284638 = 0;
        }
      }
    }
  }
LAB_001724f4:
  uVar12 = DAT_001a7d30;
  if ((DAT_00282150 != 0) && (DAT_00282150 != DAT_001a7d30)) {
    memset(&DAT_00282150,0,0x9b8);
  }
  if ((uStack_248 & 1) == 0) {
    DAT_00282af8 = 0;
  }
  else {
    DAT_00282ae8 = DAT_00282af8;
    if (DAT_00282af8 != 0) {
      if (DAT_00282ae4 != 0) goto LAB_00171dd0;
      if (DAT_00282ae0 == 0) {
        iVar8 = (*DAT_001a7cf8)(DAT_001a7cc8,"edit_controls_ui");
        if (((iVar8 == 0) || (iVar8 = (*DAT_001a7cf8)(DAT_001a7cc8,"popup_generic"), iVar8 == 0)) ||
           (iVar8 = (*DAT_001a7cf8)(DAT_001a7cc8,"popover_text_left"), iVar8 == 0))
        goto LAB_00171dd0;
        iVar8 = FUN_0017e0b0();
        if (iVar8 == 0) {
          uVar13 = DAT_00282b0c + 1;
          DAT_00282ae4 = 1;
          bVar7 = DAT_00282b0c < 0x18;
          DAT_00282b0c = uVar13;
          if ((bVar7) && (DAT_001a7d00 != (code *)0x0)) {
            (*DAT_001a7d00)(DAT_001a7cc8,"script_port_fps","native_ui_contract_failed",0);
          }
          goto LAB_00171dd0;
        }
        uVar13 = DAT_00282b0c + 1;
        bVar7 = DAT_00282b0c < 0x18;
        DAT_00282b0c = uVar13;
        if ((bVar7) && (DAT_001a7d00 != (code *)0x0)) {
          (*DAT_001a7d00)(DAT_001a7cc8,"script_port_fps","native_slider_created",1);
        }
      }
      iVar8 = FUN_0016d5cc(DAT_00282158,DAT_001a7d30);
      if ((iVar8 != 0) && (iVar8 = FUN_0016e7c0(), iVar8 != 0)) {
        fVar18 = param_1 / DAT_001e53fc;
        fVar19 = param_2 / DAT_001e53fc;
        uVar14 = NEON_fminnm(fVar18 / 620.0,fVar19 / 420.0);
        fVar17 = (float)NEON_fminnm(uVar14,0x3f800000);
        if ((ABS(fVar17) != INFINITY) && ((!NAN(ABS(fVar17)) && (0.0 < fVar17)))) {
          FUN_0016cf00(fVar17,fVar17,DAT_00282158);
          fVar15 = (float)NEON_fnmsub(param_1,0x3f000000,DAT_001e5400);
          fVar16 = (float)NEON_fnmsub(param_2,0x3f000000,DAT_001e5404);
          FUN_0016cf5c(fVar15 / DAT_001e53fc,fVar16 / DAT_001e53fc,DAT_00282158);
          fVar15 = 0.0;
          FUN_0017b5e8(0,0,fVar18 / fVar17,fVar19 / fVar17,DAT_00282178);
          FUN_0016cf70(0x3f266666,DAT_00282178);
          FUN_0017b5e8(0,0,0x44160000,0x43b90000,DAT_00282160);
          FUN_0017b5e8(0x43838000,0xc31b0000,0x42400000,0x42280000,DAT_002822d0);
          FUN_0017b5e8(0xc2dc0000,0x43070000,0x43160000,0x42300000,DAT_00282428);
          FUN_0017b5e8(0x42dc0000,0x43070000,0x43160000,0x42300000,DAT_00282580);
          if (DAT_00282af0 - 1 < param_3) {
            fVar15 = (float)(param_3 - DAT_00282af0) / 1000.0;
          }
          uVar14 = NEON_fmin(fVar15,0x3dcccccd);
          DAT_00282af0 = param_3;
          (*(code *)(DAT_001a7cd0 + 0x889940))(uVar14,DAT_00282168);
          local_1f0 = 0.0;
          if (((((0xfff < DAT_00282168 + 0xe8) &&
                ((DAT_00282168 & 0xfffffffffffffffc) != 0xffffffffffffff14)) &&
               (iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00282168 + 0xe8,&local_1f0,4), iVar8 == 1))
              && ((0xffffff6e < (int)local_1f0 - 0x92U && (DAT_00282af8 != 0)))) &&
             (DAT_00282b04 == 0)) {
            DAT_00282afc = local_1f0;
            FUN_0016cf5c(0,0xc1f00000,DAT_00282168);
            FUN_0017e998(0xc2aa0000,0xc3190000,0x41d00000,&DAT_002826d8,"FPS LIMIT");
            if (local_1f0 == 2.03188e-43) {
              FUN_00176a24(local_1e0,0x60,0x60,&DAT_0013aa70);
            }
            else {
              FUN_00176a24(local_1e0,0x60,0x60,"Limit: %d FPS");
            }
            FUN_0017e998(0xc2dc0000,0xc2b80000,0x41c00000,&DAT_00282830,local_1e0);
            FUN_0017e998(0xc3520000,0,0x41800000,&DAT_00282988,
                         "Actual FPS depends on your device.\nPress SAVE to apply.");
            uVar5 = DAT_00282428;
            pcVar2 = "SAVED";
            if (DAT_00282afc != DAT_00282b00) {
              pcVar2 = "SAVE";
            }
            uVar11 = FUN_0017b858(pcVar2);
            FUN_0016cec0(uVar5,uVar11);
            uVar5 = DAT_00282580;
            pcVar2 = "DEFAULT";
            if (DAT_00282b00 != 2.03188e-43) {
              pcVar2 = "RESET";
            }
            uVar11 = FUN_0017b858(pcVar2);
            FUN_0016cec0(uVar5,uVar11);
          }
        }
      }
      goto LAB_00171dd0;
    }
  }
  DAT_00282ae8 = DAT_00282af8;
  if ((DAT_00282ae0 != 0) && (iVar8 = FUN_0016d5cc(DAT_00282158,uVar12), iVar8 != 0)) {
    FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_00282158);
  }
LAB_00171dd0:
  if (*(long *)(lVar3 + 0x28) == local_a0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00172fd4 @ 00172fd4 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00172fd4(uint param_1)

{
  uint uVar1;
  ulong *puVar2;
  ulong *puVar3;
  undefined8 *puVar4;
  long lVar5;
  long lVar6;
  bool bVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  bool bVar11;
  int iVar12;
  int iVar13;
  undefined4 *puVar14;
  undefined8 uVar15;
  ulong uVar16;
  ulong uVar17;
  long lVar18;
  ulong uVar19;
  undefined4 uVar20;
  char *pcVar21;
  int *piVar22;
  undefined8 *puVar23;
  ushort uVar24;
  undefined1 auVar25 [16];
  undefined1 auVar26 [16];
  undefined8 uVar27;
  undefined8 uVar28;
  undefined8 uVar29;
  undefined8 uVar30;
  undefined8 uVar31;
  undefined8 uVar32;
  ulong local_110;
  char local_104 [4];
  ulong local_100;
  undefined4 local_f4;
  undefined8 local_f0;
  undefined8 local_e8;
  code *local_e0;
  code *local_d8;
  code *local_d0;
  code *local_c8;
  code *local_c0;
  code *local_b8;
  code *local_b0;
  code *local_a8;
  code *local_a0;
  code *pcStack_98;
  code *local_90;
  code *local_88;
  code *local_80;
  code *local_78;
  long local_70;
  
  lVar6 = tpidr_el0;
  uVar19 = (ulong)param_1;
  local_70 = *(long *)(lVar6 + 0x28);
  if (param_1 == 0x2b) {
    iVar12 = FUN_0016d910();
    if (iVar12 == 0) {
      puVar14 = (undefined4 *)__errno();
      uVar20 = *puVar14;
      uVar1 = DAT_001a7d20 + 1;
      bVar11 = 0x3f < DAT_001a7d20;
      DAT_001a7d20 = uVar1;
      if ((bVar11) || (DAT_001a7d00 == (code *)0x0)) goto LAB_00173430;
      pcVar21 = "clip_stage_changed";
LAB_00173424:
      (*DAT_001a7d00)(DAT_001a7cc8,"nexus_menu_probe",pcVar21,0);
LAB_00173430:
      *puVar14 = uVar20;
    }
    else {
      local_f0 = CONCAT71(local_f0._1_7_,1);
      if (((DAT_001a7d28 + 0x19d < 0x1001) ||
          (iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x19c,&local_f0,1), iVar12 != 1)) ||
         ((char)local_f0 != '\0')) {
        puVar14 = (undefined4 *)__errno();
        uVar20 = *puVar14;
        uVar1 = DAT_001a7d20 + 1;
        bVar11 = DAT_001a7d20 < 0x40;
        DAT_001a7d20 = uVar1;
        if ((bVar11) && (DAT_001a7d00 != (code *)0x0)) {
          pcVar21 = "clip_stage_busy";
          goto LAB_00173424;
        }
        goto LAB_00173430;
      }
      auVar26._8_8_ = DAT_0022d4e8;
      auVar26._0_8_ = DAT_0022d4e0;
      auVar25._8_8_ = DAT_0022d4f8;
      auVar25._0_8_ = DAT_0022d4f0;
      local_e0 = FUN_0017fe9c;
      local_d8 = FUN_0017fee0;
      auVar25 = NEON_cmeq(auVar25,0,8);
      auVar26 = NEON_cmeq(auVar26,0,8);
      local_d0 = FUN_00180034;
      local_c8 = FUN_001802bc;
      local_c0 = FUN_00180540;
      local_b8 = FUN_00180548;
      local_b0 = FUN_00180564;
      local_a8 = FUN_00180580;
      uVar24 = NEON_umaxv(CONCAT26(CONCAT11(~auVar25[9],~auVar25[8]),
                                   CONCAT24(CONCAT11(~auVar25[1],~auVar25[0]),
                                            CONCAT22(CONCAT11(~auVar26[9],~auVar26[8]),
                                                     CONCAT11(~auVar26[1],~auVar26[0])))),2);
      local_a0 = FUN_0018058c;
      pcStack_98 = FUN_001805a4;
      local_90 = FUN_00180724;
      local_88 = FUN_0018073c;
      local_f0 = DAT_001a7cd0;
      local_e8 = 0;
      local_80 = FUN_00180758;
      local_78 = FUN_0017fbc8;
      if ((((uVar24 & 1) != 0) || (DAT_0022d510 != 0)) ||
         ((0xfffffffffecfefff < DAT_001a7cd0 - 0x1000 || (_DAT_0022d514 != 0)))) {
        puVar14 = (undefined4 *)__errno();
        uVar20 = *puVar14;
        uVar1 = DAT_001a7d20 + 1;
        bVar11 = DAT_001a7d20 < 0x40;
        DAT_001a7d20 = uVar1;
        if ((bVar11) && (DAT_001a7d00 != (code *)0x0)) {
          (*DAT_001a7d00)(DAT_001a7cc8,"nexus_menu_probe","clip_create_contract",0);
        }
LAB_001733e4:
        *puVar14 = uVar20;
        pcVar21 = "clip_create_refused";
LAB_001733f0:
        uVar20 = *puVar14;
        uVar1 = DAT_001a7d20 + 1;
        bVar11 = DAT_001a7d20 < 0x40;
        DAT_001a7d20 = uVar1;
        if ((bVar11) && (DAT_001a7d00 != (code *)0x0)) goto LAB_00173424;
        goto LAB_00173430;
      }
      iVar12 = FUN_001802bc();
      if (iVar12 == 0) {
        puVar14 = (undefined4 *)__errno();
        uVar20 = *puVar14;
        if (local_78 != (code *)0x0) {
          pcVar21 = "clip_create_pins";
LAB_001733e0:
          (*local_78)(local_e8,pcVar21);
        }
        goto LAB_001733e4;
      }
      DAT_0022d508 = DAT_0010f8c8;
      DAT_0022d500 = local_f0;
      DAT_0022d4e0 = (*local_c0)(local_e8,0x2c8);
      if (DAT_0022d4e0 == 0) {
        puVar14 = (undefined4 *)__errno();
        uVar20 = *puVar14;
        if (local_78 != (code *)0x0) {
          pcVar21 = "clip_view_allocate";
          goto LAB_001733e0;
        }
        goto LAB_001733e4;
      }
      pcVar21 = "clip_view_address";
      if ((DAT_0022d4e0 + 0x2c8 >> 3 < 0x259) || ((DAT_0022d4e0 & 7) != 0)) {
LAB_00173498:
        _DAT_0022d510 = DAT_0010f740;
        puVar14 = (undefined4 *)__errno();
        uVar20 = *puVar14;
        if (local_78 != (code *)0x0) {
          (*local_78)(local_e8,pcVar21);
        }
        pcVar21 = "clip_create_partial";
        *puVar14 = uVar20;
        goto LAB_001733f0;
      }
      iVar12 = (*local_d0)(local_e8,DAT_0022d4e0,0x2c8);
      if (iVar12 == 0) {
        pcVar21 = "clip_view_writable";
        goto LAB_00173498;
      }
      (*local_b8)(local_e8,DAT_0022d4e0,1);
      local_100 = 0;
      if ((((0xffffffffffffed37 < DAT_0022d4e0 - 0x1000) || ((DAT_0022d4e0 & 7) != 0)) ||
          (iVar12 = (*local_e0)(local_e8,DAT_0022d4e0,&local_100,8), uVar16 = DAT_0022d4e0,
          iVar12 != 1)) || (local_100 != local_f0 + 0x11c11e8)) {
        pcVar21 = "clip_view_type";
        goto LAB_00173498;
      }
      local_110 = local_110 & 0xffffffff00000000;
      uVar17 = DAT_0022d4e0 + 0x40;
      local_100 = 1;
      if (((uVar17 >> 3 < 0x201) ||
          (iVar12 = (*local_e0)(local_e8,DAT_0022d4e0 + 0x38,&local_100,8),
          0xffffffffffffffbb < uVar16)) ||
         ((iVar12 != 1 ||
          ((local_100 != 0 || (iVar12 = (*local_e0)(local_e8,uVar17,&local_110,4), iVar12 != 1))))))
      {
        pcVar21 = "clip_view_detached";
        goto LAB_00173498;
      }
      if ((int)local_110 != -1) {
        pcVar21 = "clip_view_detached";
        goto LAB_00173498;
      }
      if (((DAT_0022d4e0 + 0xd8 >> 3 < 0x201) ||
          (iVar12 = (*local_e0)(local_e8,DAT_0022d4e0 + 0xd0,&DAT_0022d4e8,8), iVar12 != 1)) ||
         (DAT_0022d4e8 == DAT_0022d4e0)) {
        pcVar21 = "clip_inner_pointer";
        goto LAB_00173498;
      }
      iVar12 = FUN_00180764(&local_f0,DAT_0022d4e8,0x11c1360,0x80);
      if (iVar12 == 0) {
        pcVar21 = "clip_inner_type";
        goto LAB_00173498;
      }
      iVar12 = FUN_001808ec(&local_f0,DAT_0022d4e8,DAT_0022d4e0);
      if (iVar12 == 0) {
        pcVar21 = "clip_inner_membership";
        goto LAB_00173498;
      }
      local_100 = local_100 & 0xffffffffffff0000;
      if (DAT_0022d4e0 + 0x50 >> 1 < 0x801) {
        pcVar21 = "clip_view_count";
        goto LAB_00173498;
      }
      iVar12 = (*local_e0)(local_e8,DAT_0022d4e0 + 0x4e,&local_100,2);
      pcVar21 = "clip_view_count";
      if ((iVar12 != 1) || (pcVar21 = "clip_view_count", (short)local_100 != 1)) goto LAB_00173498;
      local_100 = local_100 & 0xffffffffffff0000;
      if (DAT_0022d4e8 + 0x50 >> 1 < 0x801) {
        pcVar21 = "clip_inner_count";
        goto LAB_00173498;
      }
      iVar12 = (*local_e0)(local_e8,DAT_0022d4e8 + 0x4e,&local_100,2);
      pcVar21 = "clip_inner_count";
      if ((iVar12 != 1) || ((short)local_100 != 0)) goto LAB_00173498;
      DAT_0022d4f0 = (*local_c0)(local_e8,0x80);
      if (DAT_0022d4f0 == 0) {
        pcVar21 = "clip_content_allocate";
        goto LAB_00173498;
      }
      pcVar21 = "clip_content_address";
      if ((((DAT_0022d4f0 + 0x80 >> 7 < 0x21) || ((DAT_0022d4f0 & 7) != 0)) ||
          (pcVar21 = "clip_content_alias", DAT_0022d4f0 == DAT_0022d4e0)) ||
         (DAT_0022d4f0 == DAT_0022d4e8)) goto LAB_00173498;
      iVar12 = (*local_d0)(local_e8,DAT_0022d4f0,0x80);
      if (iVar12 == 0) {
        pcVar21 = "clip_content_writable";
        goto LAB_00173498;
      }
      (*local_b0)(local_e8,DAT_0022d4f0,0);
      iVar12 = FUN_00180764(&local_f0,DAT_0022d4f0,0x11abcf0,0x80);
      if (iVar12 == 0) {
        pcVar21 = "clip_content_type";
        goto LAB_00173498;
      }
      iVar12 = FUN_00180814(&local_f0,DAT_0022d4f0);
      if (iVar12 == 0) {
        pcVar21 = "clip_content_detached";
        goto LAB_00173498;
      }
      local_100 = local_100 & 0xffffffffffff0000;
      if (DAT_0022d4f0 + 0x50 >> 1 < 0x801) {
        pcVar21 = "clip_content_count";
        goto LAB_00173498;
      }
      iVar12 = (*local_e0)(local_e8,DAT_0022d4f0 + 0x4e,&local_100,2);
      pcVar21 = "clip_content_count";
      if ((iVar12 != 1) || ((short)local_100 != 0)) goto LAB_00173498;
      (*local_a8)(local_e8,DAT_0022d4e8,DAT_0022d4f0);
      iVar12 = FUN_00180b44(&local_f0,&DAT_0022d4e0);
      if (iVar12 == 0) {
        pcVar21 = "clip_content_parenting";
        goto LAB_00173498;
      }
      (*local_80)(local_e8,DAT_0022d4e0,1);
      (*local_80)(local_e8,DAT_0022d4e8,1);
      (*local_80)(local_e8,DAT_0022d4f0,1);
      local_104[0] = '\x01';
      local_f4 = (uint)local_f4._2_2_ << 0x10;
      iVar12 = (*local_d0)(local_e8,DAT_0022d4e0 + 0xe0,1);
      if (((iVar12 == 0) || (iVar12 = (*local_d0)(local_e8,DAT_0022d4e0 + 0xe7,2), iVar12 == 0)) ||
         ((iVar12 = (*local_d0)(local_e8,DAT_0022d4e0 + 0x280,2), iVar12 == 0 ||
          (iVar12 = (*local_d0)(local_e8,DAT_0022d4e0 + 0x27e,1), iVar12 == 0)))) {
        pcVar21 = "clip_policy_writable";
        goto LAB_00173498;
      }
      iVar12 = (*local_d8)(local_e8,DAT_0022d4e0 + 0xe0,local_104,1);
      if (iVar12 == 0) {
        pcVar21 = "clip_policy_clip";
        goto LAB_00173498;
      }
      iVar12 = (*local_d8)(local_e8,DAT_0022d4e0 + 0xe7,&local_f4,2);
      if (iVar12 == 0) {
        pcVar21 = "clip_policy_axes";
        goto LAB_00173498;
      }
      iVar12 = (*local_d8)(local_e8,DAT_0022d4e0 + 0x280,&local_f4,2);
      if (iVar12 == 0) {
        pcVar21 = "clip_policy_controller";
        goto LAB_00173498;
      }
      (*local_88)(local_e8,DAT_0022d4e0 + 0xf0,0);
      _DAT_0022d510 = CONCAT44(_DAT_0022d514,1);
      iVar12 = FUN_00180d80(&local_f0,&DAT_0022d4e0,0,0);
      uVar17 = DAT_001a7d38;
      uVar16 = DAT_001a7d28;
      if (iVar12 == 0) {
        pcVar21 = "clip_create_postcondition";
        goto LAB_00173498;
      }
      iVar12 = FUN_00180d80(&local_f0,&DAT_0022d4e0,0,0);
      if ((iVar12 == 0) || (DAT_0022d4f8 != 0)) {
        puVar14 = (undefined4 *)__errno();
        uVar20 = *puVar14;
        if (local_78 != (code *)0x0) {
          pcVar21 = "clip_attach_contract";
LAB_001745dc:
          (*local_78)(local_e8,pcVar21);
        }
LAB_001745e0:
        *puVar14 = uVar20;
        pcVar21 = "clip_attach_refused";
LAB_001745ec:
        uVar1 = DAT_001a7d20 + 1;
        bVar11 = DAT_001a7d20 < 0x40;
        DAT_001a7d20 = uVar1;
        if ((bVar11) && (DAT_001a7d00 != (code *)0x0)) {
          (*DAT_001a7d00)(DAT_001a7cc8,"nexus_menu_probe",pcVar21,0);
        }
        *puVar14 = uVar20;
      }
      else {
        if ((((uVar17 + 0x80 >> 7 < 0x21) || ((uVar17 & 7) != 0)) || (DAT_0022d4e0 == uVar17)) ||
           ((DAT_0022d4e8 == uVar17 || (DAT_0022d4f0 == uVar17)))) {
          puVar14 = (undefined4 *)__errno();
          uVar20 = *puVar14;
          if (local_78 != (code *)0x0) {
            pcVar21 = "clip_attach_parent_address";
            goto LAB_001745dc;
          }
          goto LAB_001745e0;
        }
        local_100 = 0;
        iVar12 = (*local_e0)(local_e8,uVar17,&local_100,8);
        if ((iVar12 != 1) || (local_100 != local_f0 + 0x11abcf0)) {
          puVar14 = (undefined4 *)__errno();
          uVar20 = *puVar14;
          if (local_78 != (code *)0x0) {
            pcVar21 = "clip_attach_parent_type";
            goto LAB_001745dc;
          }
          goto LAB_001745e0;
        }
        local_100 = 0;
        iVar12 = (*local_e0)(local_e8,uVar17 + 0x30,&local_100,8);
        if ((iVar12 != 1) || (local_100 != uVar16)) {
          puVar14 = (undefined4 *)__errno();
          uVar20 = *puVar14;
          if (local_78 != (code *)0x0) {
            pcVar21 = "clip_attach_parent_context";
            goto LAB_001745dc;
          }
          goto LAB_001745e0;
        }
        iVar12 = (*local_c8)(local_e8);
        if (iVar12 == 0) {
          puVar14 = (undefined4 *)__errno();
          uVar20 = *puVar14;
          if (local_78 != (code *)0x0) {
            pcVar21 = "clip_attach_pins";
            goto LAB_001745dc;
          }
          goto LAB_001745e0;
        }
        (*local_a8)(local_e8,uVar17,DAT_0022d4e0);
        DAT_0022d4f8 = uVar17;
        iVar12 = FUN_00180d80(&local_f0,&DAT_0022d4e0,uVar16,0);
        if (iVar12 == 0) {
          _DAT_0022d510 = DAT_0010f740;
          puVar14 = (undefined4 *)__errno();
          uVar20 = *puVar14;
          if (local_78 != (code *)0x0) {
            (*local_78)(local_e8,"clip_attach_postcondition");
          }
          pcVar21 = "clip_attach_partial";
          *puVar14 = uVar20;
          goto LAB_001745ec;
        }
        iVar12 = FUN_0017fc3c(&local_f0,DAT_001a7d28,0,0);
        if (iVar12 == 1) {
          uVar15 = FUN_0017fc3c(&local_f0,DAT_001a7d28,0,1);
          if ((int)uVar15 == 1) goto LAB_00173aa0;
          pcVar21 = "clip_content_move_refused";
          if ((int)uVar15 != 0) {
            pcVar21 = "clip_content_move_partial";
          }
          FUN_0017fbc8(uVar15,pcVar21);
        }
        else {
          puVar14 = (undefined4 *)__errno();
          uVar20 = *puVar14;
          uVar1 = DAT_001a7d20 + 1;
          bVar11 = DAT_001a7d20 < 0x40;
          DAT_001a7d20 = uVar1;
          if ((bVar11) && (DAT_001a7d00 != (code *)0x0)) {
            pcVar21 = "clip_view_move_refused";
            if (iVar12 != 0) {
              pcVar21 = "clip_view_move_partial";
            }
            (*DAT_001a7d00)(DAT_001a7cc8,"nexus_menu_probe",pcVar21,0);
          }
          *puVar14 = uVar20;
        }
      }
    }
    uVar15 = 0xffffffff;
    DAT_001a7d10 = 0xffffffff;
    PTR_s_not_initialized_001a7d50 = s_main_page_content_construction_00138b2e;
    uVar1 = DAT_001a7d20 + 1;
    bVar11 = 0x3f < DAT_001a7d20;
    DAT_001a7d20 = uVar1;
    if ((bVar11) || (DAT_001a7d00 == (code *)0x0)) goto LAB_00173bd4;
    pcVar21 = "main_page_content_construction";
LAB_00173bcc:
    DAT_001a7d10 = 0xffffffff;
    (*DAT_001a7d00)(DAT_001a7cc8,"nexus_rich_stopped",pcVar21,uVar19);
  }
  else {
    if ((0x2a < param_1) && (DAT_0022d510 == 0)) {
      puVar14 = (undefined4 *)__errno();
      uVar20 = *puVar14;
      uVar1 = DAT_001a7d20 + 1;
      bVar11 = DAT_001a7d20 < 0x40;
      DAT_001a7d20 = uVar1;
      if ((bVar11) && (DAT_001a7d00 != (code *)0x0)) {
        pcVar21 = "clip_missing_live";
        goto LAB_00173424;
      }
      goto LAB_00173430;
    }
LAB_00173aa0:
    lVar5 = uVar19 * 0xb0;
    puVar2 = &DAT_001c3568 + uVar19 * 0x1b;
    nexus_rich_asset(*(undefined4 *)(&DAT_001a7d6c + lVar5));
    uVar16 = FUN_0016ce48();
    uVar20 = *(undefined4 *)(&DAT_001a7d6c + lVar5);
    (&DAT_001c3570)[uVar19 * 0x1b] = uVar16;
    puVar23 = (undefined8 *)(&DAT_001a7d78 + lVar5);
    iVar12 = FUN_0016dc20(uVar16,uVar20,*(undefined4 *)puVar23);
    if (iVar12 == 0) {
LAB_00173b78:
      uVar15 = 0xffffffff;
      DAT_001a7d10 = 0xffffffff;
      PTR_s_not_initialized_001a7d50 = s_asset_movie_contract_00133d1c;
      uVar1 = DAT_001a7d20 + 1;
      bVar11 = 0x3f < DAT_001a7d20;
      DAT_001a7d20 = uVar1;
      if ((bVar11) || (DAT_001a7d00 == (code *)0x0)) goto LAB_00173bd4;
      pcVar21 = "asset_movie_contract";
      goto LAB_00173bcc;
    }
    local_100 = local_100 & 0xffffffff00000000;
    local_f0 = 1;
    if ((((uVar16 + 0x38 < 0x1000) || ((uVar16 & 0xfffffffffffffff8) == 0xffffffffffffffc0)) ||
        (iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar16 + 0x38,&local_f0,8), iVar12 != 1)) ||
       (((local_f0 != 0 || (uVar16 + 0x40 < 0x1000)) ||
        (((uVar16 & 0xfffffffffffffffc) == 0xffffffffffffffbc ||
         ((iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar16 + 0x40,&local_100,4), iVar12 != 1 ||
          ((int)local_100 != -1)))))))) goto LAB_00173b78;
    piVar22 = &DAT_001a7d68 + uVar19 * 0x2c;
    if (*piVar22 != 2) {
      uVar20 = *(undefined4 *)puVar23;
      *puVar2 = uVar16;
      FUN_0016ce84(uVar16,uVar20);
      iVar12 = *piVar22;
      if (iVar12 == 3) {
        uVar17 = FUN_0016ce98(uVar16,&DAT_00134a97);
        local_f0 = 0;
        puVar3 = &DAT_001c3578 + uVar19 * 0x1b;
        *puVar3 = uVar17;
        if (uVar17 + 8 >> 3 < 0x201) {
          bVar11 = false;
        }
        else {
          iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar17,&local_f0,8);
          bVar11 = iVar12 == 1;
        }
        bVar7 = false;
        if (0xfff < local_f0) {
          bVar7 = bVar11;
        }
        uVar17 = local_f0;
        if (!(bool)(bVar7 & (local_f0 & 7) == 0)) {
          uVar17 = 0;
        }
        if (uVar17 == DAT_001a7cd0 + 0x11abad8) {
          local_f0 = 0;
          uVar17 = *puVar3 + 0x30;
          if ((((0xfff < uVar17) && ((*puVar3 & 0xfffffffffffffff8) != 0xffffffffffffffc8)) &&
              (iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar17,&local_f0,8), iVar12 == 1)) &&
             (((local_f0 == 0 || (local_f0 == DAT_001a7d28)) &&
              (iVar12 = FUN_0016e020(uVar16,*puVar3), iVar12 != 0)))) {
            local_f0 = local_f0 & 0xffffffffffff0000;
            uVar16 = *puVar3 + 0xb0;
            if (((uVar16 < 0x1000) || ((*puVar3 & 0xfffffffffffffffe) == 0xffffffffffffff4e)) ||
               ((iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar16,&local_f0,2), iVar12 != 1 ||
                (((short)local_f0 < 1 || (0x100 < (short)local_f0)))))) {
              DAT_001a7d10 = 0xffffffff;
              PTR_s_not_initialized_001a7d50 = s_font_export_size_0013b662;
              uVar1 = DAT_001a7d20 + 1;
              bVar11 = 0x3f < DAT_001a7d20;
              DAT_001a7d20 = uVar1;
              if ((bVar11) || (DAT_001a7d00 == (code *)0x0)) goto LAB_00173bd0;
              pcVar21 = "font_export_size";
              goto LAB_00173bcc;
            }
LAB_00173e30:
            iVar12 = *piVar22;
            goto LAB_00173e34;
          }
        }
        uVar15 = 0xffffffff;
        DAT_001a7d10 = 0xffffffff;
        PTR_s_not_initialized_001a7d50 = s_font_export_owned_txt_001326dc;
        uVar1 = DAT_001a7d20 + 1;
        bVar11 = 0x3f < DAT_001a7d20;
        DAT_001a7d20 = uVar1;
        if ((bVar11) || (DAT_001a7d00 == (code *)0x0)) goto LAB_00173bd4;
        pcVar21 = "font_export_owned_txt";
      }
      else {
LAB_00173e34:
        if (iVar12 != 2) {
          FUN_0016ce20(*puVar2,0);
          iVar12 = *piVar22;
        }
        local_f0 = CONCAT71(local_f0._1_7_,0xff);
        if (0x1000 < *puVar2 + 0x49) {
          iVar13 = (*DAT_001a7ce8)(DAT_001a7cc8,*puVar2 + 0x48,&local_f0,1);
          if ((iVar13 == 1) && ((bool)(char)local_f0 == (iVar12 == 2))) {
            uVar16 = *puVar2 + 0x10;
            if ((0xfff < uVar16) &&
               (((*puVar2 & 0xfffffffffffffff0) != 0xffffffffffffffe0 &&
                (iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar16,&local_f0,0x10), iVar12 == 1)))) {
              if ((ABS((float)local_f0) != INFINITY) && (!NAN(ABS((float)local_f0)))) {
                if ((ABS(local_f0._4_4_) != INFINITY) && (!NAN(ABS(local_f0._4_4_)))) {
                  if ((ABS((float)local_e8) != INFINITY) && (!NAN(ABS((float)local_e8)))) {
                    if (((((ABS(local_e8._4_4_) != INFINITY) && (!NAN(ABS(local_e8._4_4_)))) &&
                         (local_f0._4_4_ == 0.0)) &&
                        ((((float)local_e8 == 0.0 && (0.0 < (float)local_f0)) &&
                         (0.0 < local_e8._4_4_)))) &&
                       (((float)local_f0 == 128.0 || (float)local_f0 < 128.0 != NAN((float)local_f0)
                        && (local_e8._4_4_ == 128.0 || local_e8._4_4_ < 128.0 != NAN(local_e8._4_4_)
                           )))) {
                      local_f0 = 0;
                      uVar16 = *puVar2 + 0x30;
                      if (((0xfff < uVar16) &&
                          (((*puVar2 & 0xfffffffffffffff8) != 0xffffffffffffffc8 &&
                           (iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar16,&local_f0,8), iVar12 == 1))
                          )) && ((local_f0 == 0 || (local_f0 == DAT_001a7d28)))) {
                        FUN_0016cf5c(*puVar2);
                        puVar4 = &DAT_001a7d38;
                        if (0x2a < param_1) {
                          puVar4 = &DAT_0022d4f0;
                        }
                        FUN_0016cfa4(*puVar4,*puVar2);
                        iVar12 = FUN_0016d5cc(*puVar2,*puVar4);
                        if (iVar12 != 0) {
                          uVar28 = *(undefined8 *)(lVar5 + 0x1a7e10);
                          uVar27 = *(undefined8 *)(&DAT_001a7e08 + lVar5);
                          uVar8 = *(undefined8 *)(&DAT_001a7de8 + lVar5);
                          uVar9 = *(undefined8 *)(&DAT_001a7df8 + lVar5);
                          uVar10 = *(undefined8 *)(lVar5 + 0x1a7e00);
                          lVar18 = uVar19 * 0xd8;
                          uVar15 = 1;
                          uVar30 = *(undefined8 *)(lVar5 + 0x1a7db0);
                          uVar29 = *(undefined8 *)(&DAT_001a7da8 + lVar5);
                          uVar32 = *(undefined8 *)(lVar5 + 0x1a7dc0);
                          uVar31 = *(undefined8 *)(&DAT_001a7db8 + lVar5);
                          *(undefined8 *)(lVar18 + 0x1c3610) = *(undefined8 *)(lVar5 + 0x1a7df0);
                          *(undefined8 *)(&DAT_001c3608 + lVar18) = uVar8;
                          *(undefined8 *)(lVar18 + 0x1c3620) = uVar10;
                          *(undefined8 *)(&DAT_001c3618 + lVar18) = uVar9;
                          *(undefined8 *)(lVar18 + 0x1c3630) = uVar28;
                          *(undefined8 *)(&DAT_001c3628 + lVar18) = uVar27;
                          uVar10 = *(undefined8 *)(&DAT_001a7dc8 + lVar5);
                          uVar27 = *(undefined8 *)(lVar5 + 0x1a7dd0);
                          uVar8 = *(undefined8 *)(&DAT_001a7dd8 + lVar5);
                          uVar9 = *(undefined8 *)(lVar5 + 0x1a7de0);
                          *(undefined8 *)(lVar18 + 0x1c35d0) = uVar30;
                          *(undefined8 *)(&DAT_001c35c8 + lVar18) = uVar29;
                          *(undefined8 *)(lVar18 + 0x1c35e0) = uVar32;
                          *(undefined8 *)(&DAT_001c35d8 + lVar18) = uVar31;
                          uVar29 = *(undefined8 *)(lVar5 + 0x1a7d70);
                          uVar28 = *(undefined8 *)piVar22;
                          uVar31 = *(undefined8 *)(&DAT_001a7d80 + lVar5);
                          uVar30 = *puVar23;
                          *(undefined8 *)(lVar18 + 0x1c35f0) = uVar27;
                          *(undefined8 *)(&DAT_001c35e8 + lVar18) = uVar10;
                          *(undefined8 *)(lVar18 + 0x1c3600) = uVar9;
                          *(undefined8 *)(&DAT_001c35f8 + lVar18) = uVar8;
                          uVar8 = *(undefined8 *)(&DAT_001a7d88 + lVar5);
                          uVar9 = *(undefined8 *)(&DAT_001a7d90 + lVar5);
                          auVar26 = *(undefined1 (*) [16])(&DAT_001a7d98 + lVar5);
                          *(undefined8 *)(&DAT_001c3590 + uVar19 * 0x36) = uVar29;
                          *(undefined8 *)(&DAT_001c3588 + uVar19 * 0x36) = uVar28;
                          *(undefined8 *)(&DAT_001c35a0 + lVar18) = uVar31;
                          *(undefined8 *)(&DAT_001c3598 + lVar18) = uVar30;
                          uVar10 = DAT_0010f860;
                          *(undefined8 *)(&DAT_001c35b0 + lVar18) = uVar9;
                          *(undefined8 *)(&DAT_001c35a8 + lVar18) = uVar8;
                          *(long *)(lVar18 + 0x1c35c0) = auVar26._8_8_;
                          *(long *)(&DAT_001c35b8 + lVar18) = auVar26._0_8_;
                          *(undefined8 *)(&DAT_001c3638 + uVar19 * 0x36) = uVar10;
                          goto LAB_00173bd4;
                        }
                        uVar15 = 0xffffffff;
                        DAT_001a7d10 = 0xffffffff;
                        PTR_s_not_initialized_001a7d50 = s_carrier_membership_001373c8;
                        uVar1 = DAT_001a7d20 + 1;
                        bVar11 = 0x3f < DAT_001a7d20;
                        DAT_001a7d20 = uVar1;
                        if ((bVar11) || (DAT_001a7d00 == (code *)0x0)) goto LAB_00173bd4;
                        pcVar21 = "carrier_membership";
                        goto LAB_00173bcc;
                      }
                    }
                  }
                }
              }
            }
            uVar15 = 0xffffffff;
            DAT_001a7d10 = 0xffffffff;
            PTR_s_not_initialized_001a7d50 = s_display_matrix_context_0013b13d;
            uVar1 = DAT_001a7d20 + 1;
            bVar11 = 0x3f < DAT_001a7d20;
            DAT_001a7d20 = uVar1;
            if ((bVar11) || (DAT_001a7d00 == (code *)0x0)) goto LAB_00173bd4;
            pcVar21 = "display_matrix_context";
            goto LAB_00173bcc;
          }
        }
        uVar15 = 0xffffffff;
        DAT_001a7d10 = 0xffffffff;
        PTR_s_not_initialized_001a7d50 = s_display_input_flag_00134480;
        uVar1 = DAT_001a7d20 + 1;
        bVar11 = 0x3f < DAT_001a7d20;
        DAT_001a7d20 = uVar1;
        if ((bVar11) || (DAT_001a7d00 == (code *)0x0)) goto LAB_00173bd4;
        pcVar21 = "display_input_flag";
      }
      goto LAB_00173bcc;
    }
    uVar17 = FUN_0016cdf0(0x260);
    *puVar2 = uVar17;
    if (uVar17 == 0) {
      uVar15 = 0xffffffff;
      DAT_001a7d10 = 0xffffffff;
      PTR_s_not_initialized_001a7d50 = s_button_allocation_0013685f;
      uVar1 = DAT_001a7d20 + 1;
      bVar11 = 0x3f < DAT_001a7d20;
      DAT_001a7d20 = uVar1;
      if ((bVar11) || (DAT_001a7d00 == (code *)0x0)) goto LAB_00173bd4;
      pcVar21 = "button_allocation";
      goto LAB_00173bcc;
    }
    FUN_0016ce34();
    iVar12 = FUN_0014e5ac(*puVar2);
    if (iVar12 == 0) {
      uVar15 = 0xffffffff;
      DAT_001a7d10 = 0xffffffff;
      PTR_s_not_initialized_001a7d50 = s_button_sound_cache_contract_00136d39;
      uVar1 = DAT_001a7d20 + 1;
      bVar11 = 0x3f < DAT_001a7d20;
      DAT_001a7d20 = uVar1;
      if ((bVar11) || (DAT_001a7d00 == (code *)0x0)) goto LAB_00173bd4;
      pcVar21 = "button_sound_cache_contract";
      goto LAB_00173bcc;
    }
    local_f0 = 0;
    local_100 = 1;
    local_110 = 1;
    local_104[0] = '\x01';
    if (*puVar2 + 8 >> 3 < 0x201) {
      bVar11 = false;
    }
    else {
      iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,*puVar2,&local_f0,8);
      bVar11 = iVar12 == 1;
    }
    bVar7 = false;
    if (0xfff < local_f0) {
      bVar7 = bVar11;
    }
    uVar17 = local_f0;
    if (!(bool)(bVar7 & (local_f0 & 7) == 0)) {
      uVar17 = 0;
    }
    if (uVar17 == DAT_001a7cd0 + 0x11c0a48) {
      uVar17 = *puVar2;
      local_f4 = 0;
      local_f0 = 1;
      if ((((((uVar17 + 0x38 < 0x1000) || ((uVar17 & 0xfffffffffffffff8) == 0xffffffffffffffc0)) ||
            (iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar17 + 0x38,&local_f0,8), iVar12 != 1)) ||
           ((local_f0 != 0 || (uVar17 + 0x40 < 0x1000)))) ||
          ((uVar17 & 0xfffffffffffffffc) == 0xffffffffffffffbc)) ||
         ((iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar17 + 0x40,&local_f4,4), iVar12 != 1 ||
          (local_f4 != -1)))) goto LAB_00174240;
      uVar17 = *puVar2 + 0xa8;
      if ((uVar17 < 0x1000) ||
         ((((*puVar2 & 0xfffffffffffffff8) == 0xffffffffffffff50 ||
           (iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar17,&local_100,8), iVar12 != 1)) ||
          (local_100 != 0)))) goto LAB_00174240;
      uVar17 = *puVar2 + 0x120;
      if (((((uVar17 < 0x1000) || ((*puVar2 & 0xfffffffffffffff8) == 0xfffffffffffffed8)) ||
           (iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar17,&local_110,8), iVar12 != 1)) ||
          ((local_110 != 0 || (*puVar2 + 0x1f1 < 0x1001)))) ||
         ((iVar12 = (*DAT_001a7ce8)(DAT_001a7cc8,*puVar2 + 0x1f0,local_104,1), iVar12 != 1 ||
          (local_104[0] != '\0')))) goto LAB_00174240;
      iVar12 = FUN_0017c42c(puVar2,uVar16,uVar19);
      if (iVar12 == 0) {
        DAT_001a7d10 = 0xffffffff;
        PTR_s_not_initialized_001a7d50 = s_button_movie_binding_00137df9;
        uVar1 = DAT_001a7d20 + 1;
        bVar11 = DAT_001a7d20 < 0x40;
        DAT_001a7d20 = uVar1;
        if ((bVar11) && (DAT_001a7d00 != (code *)0x0)) {
          pcVar21 = "button_movie_binding";
          goto LAB_00173bcc;
        }
      }
      else {
        FUN_0016ce84(uVar16,*(undefined4 *)puVar23);
        lVar18 = FUN_0017b858(&DAT_00134f22);
        if (lVar18 != 0) {
          FUN_0016cec0(*puVar2,lVar18);
          goto LAB_00173e30;
        }
        DAT_001a7d10 = 0xffffffff;
        PTR_s_not_initialized_001a7d50 = s_string_budget_00136871;
        uVar1 = DAT_001a7d20 + 1;
        bVar11 = DAT_001a7d20 < 0x40;
        DAT_001a7d20 = uVar1;
        if ((bVar11) && (DAT_001a7d00 != (code *)0x0)) {
          pcVar21 = "string_budget";
          goto LAB_00173bcc;
        }
      }
    }
    else {
LAB_00174240:
      DAT_001a7d10 = 0xffffffff;
      PTR_s_not_initialized_001a7d50 = s_button_constructor_contract_00133d31;
      uVar1 = DAT_001a7d20 + 1;
      bVar11 = DAT_001a7d20 < 0x40;
      DAT_001a7d20 = uVar1;
      if ((bVar11) && (DAT_001a7d00 != (code *)0x0)) {
        pcVar21 = "button_constructor_contract";
        goto LAB_00173bcc;
      }
    }
  }
LAB_00173bd0:
  uVar15 = 0xffffffff;
LAB_00173bd4:
  if (*(long *)(lVar6 + 0x28) == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar15);
}

/* ===== FUN_0017474c @ 0017474c ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0017474c(uint param_1)

{
  bool bVar1;
  uint uVar2;
  int *piVar3;
  int *piVar4;
  ulong *puVar5;
  long lVar6;
  long lVar7;
  undefined8 uVar8;
  int iVar9;
  long lVar10;
  ulong uVar11;
  undefined8 uVar12;
  char *pcVar13;
  ulong *puVar14;
  float *pfVar15;
  ulong uVar16;
  float fVar17;
  float fVar18;
  undefined4 uVar19;
  undefined8 uVar20;
  undefined8 uVar21;
  undefined8 uVar22;
  undefined8 uVar23;
  ushort uVar24;
  undefined4 uVar25;
  undefined1 auVar26 [16];
  undefined8 uVar27;
  undefined8 uVar28;
  undefined8 uVar29;
  undefined8 uVar30;
  undefined1 auVar31 [16];
  undefined8 uVar32;
  float local_a8;
  float local_a4;
  float local_a0;
  float local_9c;
  float local_98;
  float fStack_94;
  undefined8 uStack_90;
  undefined8 local_88;
  float local_80;
  float local_7c;
  long local_78;
  
  lVar7 = tpidr_el0;
  lVar6 = (ulong)param_1 * 0xb0;
  local_78 = *(long *)(lVar7 + 0x28);
  puVar14 = &DAT_001c3568 + (ulong)param_1 * 0x1b;
  piVar3 = &DAT_001a7d68 + (ulong)param_1 * 0x2c;
  iVar9 = FUN_0017609c(puVar14,piVar3);
  if (iVar9 == 0) {
    uVar12 = 0xffffffff;
    DAT_001a7d10 = 0xffffffff;
    PTR_s_not_initialized_001a7d50 = s_live_ownership_or_type_0013b70b;
    uVar2 = DAT_001a7d20 + 1;
    bVar1 = 0x3f < DAT_001a7d20;
    DAT_001a7d20 = uVar2;
    if ((bVar1) || (DAT_001a7d00 == (code *)0x0)) goto LAB_00175378;
    pcVar13 = "live_ownership_or_type";
LAB_00175368:
    DAT_001a7d10 = 0xffffffff;
    (*DAT_001a7d00)(DAT_001a7cc8,"nexus_rich_stopped",pcVar13,param_1);
  }
  else {
    uVar16 = (ulong)param_1;
    if ((*piVar3 == 2) &&
       (*(int *)(&DAT_001a7d6c + uVar16 * 0xb0) != *(int *)(&DAT_001c358c + uVar16 * 0xd8))) {
      nexus_rich_asset();
      uVar11 = FUN_0016ce48();
      iVar9 = FUN_0016dc20(uVar11,*(int *)(&DAT_001a7d6c + uVar16 * 0xb0),
                           *(undefined4 *)(&DAT_001a7d78 + uVar16 * 0xb0));
      if (iVar9 != 0) {
        local_98 = 0.0;
        local_88 = 1;
        if (((((0xfff < uVar11 + 0x38) && ((uVar11 & 0xfffffffffffffff8) != 0xffffffffffffffc0)) &&
             (iVar9 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar11 + 0x38,&local_88,8), iVar9 == 1)) &&
            ((local_88 == 0 && (0xfff < uVar11 + 0x40)))) &&
           (((uVar11 & 0xfffffffffffffffc) != 0xffffffffffffffbc &&
            ((iVar9 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar11 + 0x40,&local_98,4), iVar9 == 1 &&
             (local_98 == -NAN)))))) {
          iVar9 = FUN_0017c42c(puVar14,uVar11,param_1);
          if (iVar9 == 0) {
            uVar12 = 0xffffffff;
            DAT_001a7d10 = 0xffffffff;
            PTR_s_not_initialized_001a7d50 = s_palette_binding_contract_001363c8;
            uVar2 = DAT_001a7d20 + 1;
            bVar1 = 0x3f < DAT_001a7d20;
            DAT_001a7d20 = uVar2;
            if ((bVar1) || (DAT_001a7d00 == (code *)0x0)) goto LAB_00175378;
            pcVar13 = "palette_binding_contract";
            goto LAB_00175368;
          }
          FUN_0016ce84(uVar11,*(undefined4 *)(&DAT_001a7d78 + uVar16 * 0xb0));
          uVar11 = *puVar14;
          uVar12 = FUN_0017b858(&DAT_00134f22);
          FUN_0016cec0(uVar11,uVar12);
          (&DAT_001c363c)[uVar16 * 0x36] = 0;
          goto LAB_001747ec;
        }
      }
      uVar12 = 0xffffffff;
      DAT_001a7d10 = 0xffffffff;
      PTR_s_not_initialized_001a7d50 = s_palette_movie_contract_001351e4;
      uVar2 = DAT_001a7d20 + 1;
      bVar1 = 0x3f < DAT_001a7d20;
      DAT_001a7d20 = uVar2;
      if ((bVar1) || (DAT_001a7d00 == (code *)0x0)) goto LAB_00175378;
      pcVar13 = "palette_movie_contract";
      goto LAB_00175368;
    }
LAB_001747ec:
    if ((&DAT_001a7d94)[uVar16 * 0xb0] == '\0') {
      if (((&DAT_001c363c)[uVar16 * 0x36] == 0) || ((&DAT_001c35b4)[uVar16 * 0xd8] != '\0')) {
        FUN_0016cf5c(*puVar14);
      }
      uVar27 = *(undefined8 *)(lVar6 + 0x1a7e10);
      uVar12 = *(undefined8 *)(&DAT_001a7e08 + lVar6);
      uVar22 = *(undefined8 *)(lVar6 + 0x1a7df0);
      uVar20 = *(undefined8 *)(&DAT_001a7de8 + lVar6);
      uVar30 = *(undefined8 *)(&DAT_001a7df8 + lVar6);
      uVar32 = *(undefined8 *)(lVar6 + 0x1a7e00);
      lVar10 = uVar16 * 0xd8;
      (&DAT_001c363c)[uVar16 * 0x36] = 1;
      *(undefined8 *)(lVar10 + 0x1c3610) = uVar22;
      *(undefined8 *)(&DAT_001c3608 + lVar10) = uVar20;
      *(undefined8 *)(lVar10 + 0x1c3620) = uVar32;
      *(undefined8 *)(&DAT_001c3618 + lVar10) = uVar30;
      uVar22 = *(undefined8 *)(lVar6 + 0x1a7db0);
      uVar20 = *(undefined8 *)(&DAT_001a7da8 + lVar6);
      uVar30 = *(undefined8 *)(&DAT_001a7db8 + lVar6);
      uVar32 = *(undefined8 *)(lVar6 + 0x1a7dc0);
      *(undefined8 *)(lVar10 + 0x1c3630) = uVar27;
      *(undefined8 *)(&DAT_001c3628 + lVar10) = uVar12;
LAB_00174fa8:
      lVar10 = uVar16 * 0xd8;
      uVar12 = 1;
      uVar28 = *(undefined8 *)(lVar6 + 0x1a7dd0);
      uVar27 = *(undefined8 *)(&DAT_001a7dc8 + lVar6);
      uVar29 = *(undefined8 *)(&DAT_001a7dd8 + lVar6);
      uVar8 = *(undefined8 *)(lVar6 + 0x1a7de0);
      *(undefined8 *)(lVar10 + 0x1c35d0) = uVar22;
      *(undefined8 *)(&DAT_001c35c8 + lVar10) = uVar20;
      *(undefined8 *)(lVar10 + 0x1c35e0) = uVar32;
      *(undefined8 *)(&DAT_001c35d8 + lVar10) = uVar30;
      uVar23 = *(undefined8 *)(lVar6 + 0x1a7d70);
      uVar21 = *(undefined8 *)piVar3;
      uVar32 = *(undefined8 *)(&DAT_001a7d78 + lVar6);
      uVar20 = *(undefined8 *)(&DAT_001a7d80 + lVar6);
      *(undefined8 *)(lVar10 + 0x1c35f0) = uVar28;
      *(undefined8 *)(&DAT_001c35e8 + lVar10) = uVar27;
      *(undefined8 *)(lVar10 + 0x1c3600) = uVar8;
      *(undefined8 *)(&DAT_001c35f8 + lVar10) = uVar29;
      uVar29 = *(undefined8 *)(&DAT_001a7d90 + lVar6);
      uVar30 = *(undefined8 *)(&DAT_001a7d88 + lVar6);
      uVar22 = *(undefined8 *)(&DAT_001a7d98 + lVar6);
      uVar27 = *(undefined8 *)(lVar6 + 0x1a7da0);
      *(undefined8 *)(&DAT_001c3590 + uVar16 * 0x36) = uVar23;
      *(undefined8 *)(&DAT_001c3588 + uVar16 * 0x36) = uVar21;
      *(undefined8 *)(&DAT_001c35a0 + lVar10) = uVar20;
      *(undefined8 *)(&DAT_001c3598 + lVar10) = uVar32;
      *(undefined8 *)(&DAT_001c35b0 + lVar10) = uVar29;
      *(undefined8 *)(&DAT_001c35a8 + lVar10) = uVar30;
      *(undefined8 *)(lVar10 + 0x1c35c0) = uVar27;
      *(undefined8 *)(&DAT_001c35b8 + lVar10) = uVar22;
      goto LAB_00175378;
    }
    iVar9 = *piVar3;
    if (iVar9 == 2) {
      if (((&DAT_001c363c)[uVar16 * 0x36] == 0) ||
         (iVar9 = strcmp((char *)(uVar16 * 0xb0 + 0x1a7d96),(char *)(uVar16 * 0xd8 + 0x1c35b6)),
         iVar9 != 0)) {
        lVar10 = FUN_0017b858(uVar16 * 0xb0 + 0x1a7d96);
        if (lVar10 == 0) {
          uVar12 = 0xffffffff;
          DAT_001a7d10 = 0xffffffff;
          PTR_s_not_initialized_001a7d50 = s_persistent_button_caption_budget_0013b722;
          uVar2 = DAT_001a7d20 + 1;
          bVar1 = 0x3f < DAT_001a7d20;
          DAT_001a7d20 = uVar2;
          if ((bVar1) || (DAT_001a7d00 == (code *)0x0)) goto LAB_00175378;
          pcVar13 = "persistent_button_caption_budget";
        }
        else {
          FUN_0016cec0(*puVar14,lVar10);
          iVar9 = FUN_0017609c(puVar14,piVar3);
          if (iVar9 != 0) {
            iVar9 = *piVar3;
            goto LAB_00174878;
          }
          uVar12 = 0xffffffff;
          DAT_001a7d10 = 0xffffffff;
          PTR_s_not_initialized_001a7d50 = s_button_caption_ownership_0013a5d3;
          uVar2 = DAT_001a7d20 + 1;
          bVar1 = 0x3f < DAT_001a7d20;
          DAT_001a7d20 = uVar2;
          if ((bVar1) || (DAT_001a7d00 == (code *)0x0)) goto LAB_00175378;
          pcVar13 = "button_caption_ownership";
        }
        goto LAB_00175368;
      }
LAB_00174ba0:
      if (((((&DAT_001c363c)[uVar16 * 0x36] == 0) || ((&DAT_001c35b4)[uVar16 * 0xd8] == '\0')) ||
          (*(float *)(&DAT_001a7d7c + uVar16 * 0xb0) != *(float *)(&DAT_001c359c + uVar16 * 0xd8)))
         || (((*(float *)(&DAT_001a7d80 + uVar16 * 0xb0) !=
               *(float *)(&DAT_001c35a0 + uVar16 * 0xd8) ||
              (*(float *)(&DAT_001a7d84 + uVar16 * 0xb0) !=
               *(float *)(&DAT_001c35a4 + uVar16 * 0xd8))) ||
             ((*(float *)(&DAT_001a7d88 + uVar16 * 0xb0) !=
               *(float *)(&DAT_001c35a8 + uVar16 * 0xd8) ||
              ((*(float *)(&DAT_001a7d8c + uVar16 * 0xb0) !=
                *(float *)(&DAT_001c35ac + uVar16 * 0xd8) ||
               (*(float *)(&DAT_001a7d90 + uVar16 * 0xb0) !=
                *(float *)(&DAT_001c35b0 + uVar16 * 0xd8))))))))) {
        pfVar15 = (float *)(&DAT_001a7d8c + uVar16 * 0xb0);
        fVar17 = *pfVar15;
        if (fVar17 != 0.0 && fVar17 < 0.0 == NAN(fVar17)) {
          iVar9 = FUN_0016d910();
          if (iVar9 != 0) {
            uVar11 = *puVar14 + 0x10;
            if (((0xfff < uVar11) && ((*puVar14 & 0xfffffffffffffff0) != 0xffffffffffffffe0)) &&
               (iVar9 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar11,&local_88,0x10), uVar11 = local_88,
               iVar9 == 1)) {
              fVar17 = (float)local_88;
              if ((ABS((float)local_88) != INFINITY) && (!NAN(ABS((float)local_88)))) {
                if (((((ABS(local_88._4_4_) != INFINITY) &&
                      ((!NAN(ABS(local_88._4_4_)) && (ABS(local_80) != INFINITY)))) &&
                     (!NAN(ABS(local_80)))) &&
                    ((((ABS(local_7c) != INFINITY && (!NAN(ABS(local_7c)))) &&
                      (local_88._4_4_ == 0.0)) && ((local_80 == 0.0 && (0.0 < (float)local_88))))))
                   && ((0.0 < local_7c &&
                       (((float)local_88 == 128.0 || (float)local_88 < 128.0 != NAN((float)local_88)
                        && (local_7c == 128.0 || local_7c < 128.0 != NAN(local_7c))))))) {
                  FUN_0016cf84(*puVar14,&local_98);
                  fVar18 = (float)uStack_90 - local_98;
                  fStack_94 = (float)((ulong)uStack_90 >> 0x20) - fStack_94;
                  if ((ABS(fVar18) != INFINITY) && (!NAN(ABS(fVar18)))) {
                    auVar26._8_8_ = CONCAT44(fStack_94,fVar18);
                    auVar26._0_8_ = CONCAT44(fStack_94,fVar18);
                    auVar31 = NEON_fcmgt(auVar26,_DAT_0010f9f0,4);
                    auVar26 = NEON_fcmge(_DAT_0010f9f0,auVar26,4);
                    uVar24 = NEON_umaxv(CONCAT26(auVar26._12_2_,
                                                 CONCAT24(auVar26._8_2_,
                                                          CONCAT22(auVar31._4_2_,auVar31._0_2_))),2)
                    ;
                    if ((((uVar24 & 1) == 0) && (ABS(fStack_94) != INFINITY)) &&
                       (!NAN(ABS(fStack_94)))) {
                      fVar18 = (fVar17 * *pfVar15) / fVar18;
                      if ((ABS(fVar18) != INFINITY) && (!NAN(ABS(fVar18)))) {
                        fStack_94 = (local_7c * *(float *)(&DAT_001a7d90 + uVar16 * 0xb0)) /
                                    fStack_94;
                        if ((((ABS(fStack_94) != INFINITY) &&
                             ((!NAN(ABS(fStack_94)) && (0.0001 <= fVar18)))) &&
                            (0.0001 <= fStack_94)) &&
                           ((fVar18 == 128.0 || fVar18 < 128.0 != NAN(fVar18) &&
                            (fStack_94 == 128.0 || fStack_94 < 128.0 != NAN(fStack_94))))) {
                          FUN_0016cf00(*puVar14);
                          FUN_0016cf84(*puVar14,&local_a8);
                          fVar17 = ABS((local_a0 - local_a8) - *pfVar15);
                          if (((fVar17 != 0.5 && fVar17 < 0.5 == NAN(fVar17)) ||
                              (((fVar17 == INFINITY || (NAN(fVar17))) ||
                               (fVar17 = ABS((local_9c - local_a4) -
                                             *(float *)(&DAT_001a7d90 + uVar16 * 0xb0)),
                               fVar17 == INFINITY)))) ||
                             (fVar17 != 0.5 && fVar17 < 0.5 == NAN(fVar17))) {
                            FUN_0016cf00(uVar11 & 0xffffffff,*puVar14);
                          }
                          else {
                            uVar19 = NEON_fmsub(local_a0 + local_a8,0x3f000000,
                                                *(undefined4 *)(&DAT_001a7d7c + uVar16 * 0xb0));
                            uVar25 = NEON_fmsub(local_9c + local_a4,0x3f000000,
                                                *(undefined4 *)(&DAT_001a7d80 + uVar16 * 0xb0));
                            FUN_0016cf5c(uVar19,uVar25);
                            iVar9 = FUN_0016d910();
                            if (iVar9 != 0) goto LAB_00174f78;
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          uVar12 = 0xffffffff;
          DAT_001a7d10 = 0xffffffff;
          PTR_s_not_initialized_001a7d50 = s_native_bounds_sizing_0013b743;
          uVar2 = DAT_001a7d20 + 1;
          bVar1 = 0x3f < DAT_001a7d20;
          DAT_001a7d20 = uVar2;
          if ((bVar1) || (DAT_001a7d00 == (code *)0x0)) goto LAB_00175378;
          pcVar13 = "native_bounds_sizing";
          goto LAB_00175368;
        }
LAB_00174f54:
        FUN_0016cf00(*(undefined4 *)(&DAT_001a7d84 + uVar16 * 0xb0),*puVar14);
        FUN_0016cf5c(*(undefined4 *)(&DAT_001a7d7c + uVar16 * 0xb0),*puVar14);
      }
LAB_00174f78:
      uVar27 = *(undefined8 *)(lVar6 + 0x1a7e10);
      uVar12 = *(undefined8 *)(&DAT_001a7e08 + lVar6);
      uVar20 = *(undefined8 *)(&DAT_001a7de8 + lVar6);
      uVar30 = *(undefined8 *)(&DAT_001a7df8 + lVar6);
      uVar32 = *(undefined8 *)(lVar6 + 0x1a7e00);
      lVar10 = uVar16 * 0xd8;
      *(undefined8 *)(lVar10 + 0x1c3610) = *(undefined8 *)(lVar6 + 0x1a7df0);
      *(undefined8 *)(&DAT_001c3608 + lVar10) = uVar20;
      *(undefined8 *)(lVar10 + 0x1c3620) = uVar32;
      *(undefined8 *)(&DAT_001c3618 + lVar10) = uVar30;
      uVar22 = *(undefined8 *)(lVar6 + 0x1a7db0);
      uVar20 = *(undefined8 *)(&DAT_001a7da8 + lVar6);
      uVar30 = *(undefined8 *)(&DAT_001a7db8 + lVar6);
      uVar32 = *(undefined8 *)(lVar6 + 0x1a7dc0);
      *(undefined8 *)(lVar10 + 0x1c3630) = uVar27;
      *(undefined8 *)(&DAT_001c3628 + lVar10) = uVar12;
      (&DAT_001c363c)[uVar16 * 0x36] = 1;
      goto LAB_00174fa8;
    }
LAB_00174878:
    if (iVar9 != 3) goto LAB_00174ba0;
    piVar4 = &DAT_001c363c + uVar16 * 0x36;
    if ((*piVar4 != 0) &&
       (iVar9 = strcmp((char *)(uVar16 * 0xb0 + 0x1a7d96),(char *)(uVar16 * 0xd8 + 0x1c35b6)),
       iVar9 == 0)) {
LAB_001748f4:
      if (*(int *)(&DAT_001a7d74 + uVar16 * 0xb0) != *(int *)(&DAT_001c3594 + uVar16 * 0xd8))
      goto LAB_00174fc8;
LAB_0017491c:
      if ((((&DAT_001c35b4)[uVar16 * 0xd8] == '\0') ||
          (*(float *)(&DAT_001a7d7c + uVar16 * 0xb0) != *(float *)(&DAT_001c359c + uVar16 * 0xd8)))
         || ((*(float *)(&DAT_001a7d80 + uVar16 * 0xb0) != *(float *)(&DAT_001c35a0 + uVar16 * 0xd8)
             || (*(float *)(&DAT_001a7d84 + uVar16 * 0xb0) !=
                 *(float *)(&DAT_001c35a4 + uVar16 * 0xd8))))) {
LAB_001749ac:
        FUN_0016cf5c(0,ZEXT816(0),*puVar14);
        puVar14 = &DAT_001c3578 + uVar16 * 0x1b;
        goto LAB_00174f54;
      }
      goto LAB_00174f78;
    }
    lVar10 = FUN_0017b858(uVar16 * 0xb0 + 0x1a7d96);
    if (lVar10 == 0) {
      uVar12 = 0xffffffff;
      DAT_001a7d10 = 0xffffffff;
      PTR_s_not_initialized_001a7d50 = s_persistent_string_budget_00139568;
      uVar2 = DAT_001a7d20 + 1;
      bVar1 = 0x3f < DAT_001a7d20;
      DAT_001a7d20 = uVar2;
      if ((bVar1) || (DAT_001a7d00 == (code *)0x0)) goto LAB_00175378;
      pcVar13 = "persistent_string_budget";
      goto LAB_00175368;
    }
    FUN_0016ceac((&DAT_001c3578)[uVar16 * 0x1b],lVar10);
    if (*piVar4 != 0) goto LAB_001748f4;
LAB_00174fc8:
    local_88 = local_88 & 0xffffffff00000000;
    local_98 = 0.0;
    local_a8 = 0.0;
    puVar5 = &DAT_001c3578 + uVar16 * 0x1b;
    uVar11 = *puVar5 + 0x84;
    if (((uVar11 < 0x1000) || ((*puVar5 & 0xfffffffffffffffc) == 0xffffffffffffff78)) ||
       (iVar9 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar11,&local_88,4), iVar9 != 1)) {
      DAT_001a7d10 = 0xffffffff;
      PTR_s_not_initialized_001a7d50 = s_outline_read_00138be0;
      uVar2 = DAT_001a7d20 + 1;
      bVar1 = DAT_001a7d20 < 0x40;
      DAT_001a7d20 = uVar2;
      if ((bVar1) && (DAT_001a7d00 != (code *)0x0)) {
        pcVar13 = "outline_read";
        goto LAB_00175368;
      }
    }
    else {
      FUN_0016ceec(*puVar5,*(float *)(&DAT_001a7d74 + uVar16 * 0xb0));
      uVar11 = *puVar5 + 0x84;
      if (((0xfff < uVar11) && ((*puVar5 & 0xfffffffffffffffc) != 0xffffffffffffff78)) &&
         ((iVar9 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar11,&local_98,4), iVar9 == 1 &&
          (local_98 == (float)local_88)))) {
        uVar11 = *puVar5 + 0x80;
        if ((((0xfff < uVar11) && ((*puVar5 & 0xfffffffffffffffc) != 0xffffffffffffff7c)) &&
            (iVar9 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar11,&local_a8,4), iVar9 == 1)) &&
           (local_a8 == *(float *)(&DAT_001a7d74 + uVar16 * 0xb0))) {
          if (*piVar4 != 0) goto LAB_0017491c;
          goto LAB_001749ac;
        }
      }
      DAT_001a7d10 = 0xffffffff;
      PTR_s_not_initialized_001a7d50 = s_text_color_or_outline_0013c897;
      uVar2 = DAT_001a7d20 + 1;
      bVar1 = DAT_001a7d20 < 0x40;
      DAT_001a7d20 = uVar2;
      if ((bVar1) && (DAT_001a7d00 != (code *)0x0)) {
        pcVar13 = "text_color_or_outline";
        goto LAB_00175368;
      }
    }
  }
  uVar12 = 0xffffffff;
LAB_00175378:
  if (*(long *)(lVar7 + 0x28) != local_78) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar12);
  }
  return;
}

/* ===== nexus_rich_action @ 00175738 ===== */

void nexus_rich_action(long param_1,undefined4 *param_2)

{
  uint uVar1;
  long lVar2;
  bool bVar3;
  int iVar4;
  int iVar5;
  ulong uVar6;
  undefined8 uVar7;
  char *pcVar8;
  uint uVar9;
  char *pcVar10;
  undefined4 uVar11;
  int iVar12;
  long *plVar13;
  ulong uVar14;
  long local_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  if (((((param_2 != (undefined4 *)0x0) && (DAT_001a7d0c != 0)) &&
       (iVar4 = (*DAT_001a7cf0)(DAT_001a7cc8), iVar4 != 0)) &&
      ((DAT_002844d0 != 0 && (DAT_002843e0 == param_1)))) &&
     (iVar4 = FUN_0016d5cc(param_1,DAT_001a7d30), iVar4 != 0)) {
    local_60 = CONCAT71(local_60._1_7_,0xff);
    if (((param_1 + 0x49U < 0x1001) ||
        (iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x48,&local_60,1), iVar4 != 1)) ||
       ((char)local_60 != '\x01')) goto LAB_0017587c;
    local_60 = 0;
    iVar4 = FUN_0014d288(&local_60);
    pcVar8 = "android_toggle_rejected";
    pcVar10 = pcVar8;
    if (((iVar4 != 0) && (local_60 != 0)) &&
       (pcVar10 = "android_toggle_rejected", local_60 == DAT_002844c0)) {
      iVar4 = FUN_0014d350();
      pcVar10 = pcVar8;
      if (iVar4 != 0) {
        pcVar10 = "android_toggle_queued";
      }
    }
    uVar1 = DAT_001a7d20 + 1;
    bVar3 = 0x3f < DAT_001a7d20;
    DAT_001a7d20 = uVar1;
    uVar1 = DAT_00282b08;
    if ((bVar3) || (DAT_001a7d00 == (code *)0x0)) goto switchD_00175df0_default;
    pcVar8 = "battle_menu_launcher";
LAB_00175d08:
    uVar11 = 0;
LAB_00175f2c:
    (*DAT_001a7d00)(DAT_001a7cc8,pcVar8,pcVar10,uVar11);
    uVar1 = DAT_00282b08;
    goto switchD_00175df0_default;
  }
LAB_0017587c:
  if ((param_2 == (undefined4 *)0x0) || (DAT_001a7d0c == 0)) {
    if (param_2 != (undefined4 *)0x0) goto LAB_00175990;
LAB_00175e40:
    uVar7 = 0;
    goto LAB_00176044;
  }
  iVar4 = (*DAT_001a7cf0)(DAT_001a7cc8);
  if (((iVar4 != 0) && (DAT_002839fc == '\x01')) && (iVar4 = FUN_0016d910(), iVar4 != 0)) {
    if ((DAT_00284220 == param_1) && (iVar4 = FUN_0016d5cc(param_1,DAT_001a7d30), iVar4 != 0)) {
      local_60 = CONCAT71(local_60._1_7_,0xff);
      if (param_1 + 0x49U < 0x1001) goto LAB_0017591c;
      uVar7 = 1;
      iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x48,&local_60,1);
      if ((iVar4 != 1) || ((char)local_60 != '\x01')) goto LAB_0017591c;
LAB_00175980:
      FUN_00192300(uVar7);
      uVar1 = DAT_00282b08;
      goto switchD_00175df0_default;
    }
LAB_0017591c:
    if ((DAT_002842f8 == param_1) && (iVar4 = FUN_0016d5cc(param_1,DAT_001a7d30), iVar4 != 0)) {
      local_60 = CONCAT71(local_60._1_7_,0xff);
      if (((0x1000 < param_1 + 0x49U) &&
          (iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x48,&local_60,1), iVar4 == 1)) &&
         ((char)local_60 == '\x01')) {
        uVar7 = 2;
        goto LAB_00175980;
      }
    }
  }
LAB_00175990:
  uVar7 = (*DAT_001a7cf0)(DAT_001a7cc8);
  if ((int)uVar7 == 0) goto LAB_00176044;
  if (((DAT_00282ae8 == 0) || (DAT_00282ae0 == 0)) ||
     ((iVar4 = FUN_0016d910(), iVar4 == 0 ||
      (iVar4 = FUN_0016d5cc(DAT_00282158,DAT_001a7d30), iVar4 == 0)))) {
LAB_00175abc:
    if ((((DAT_0028211c != 0) && (DAT_00282110 != 0)) && (iVar4 = FUN_0016d910(), iVar4 != 0)) &&
       (iVar4 = FUN_0016d5cc(DAT_00281b10,DAT_001a7d30), iVar4 != 0)) {
      if ((DAT_00281b20 != param_1) ||
         (iVar4 = FUN_0017df44(param_1), uVar1 = DAT_00282b08, iVar4 == 0)) {
        if ((DAT_00281bb8 == param_1) && (iVar4 = FUN_0017df44(param_1), iVar4 != 0)) {
          DAT_00282118 = 0;
          uVar1 = DAT_00282b08;
        }
        else {
          if ((DAT_00281c50 == param_1) && (iVar4 = FUN_0017df44(param_1), iVar4 != 0)) {
            uVar11 = 0;
          }
          else if ((DAT_00281ce8 == param_1) && (iVar4 = FUN_0017df44(param_1), iVar4 != 0)) {
            uVar11 = 1;
          }
          else if ((DAT_00281d80 == param_1) && (iVar4 = FUN_0017df44(param_1), iVar4 != 0)) {
            uVar11 = 2;
          }
          else if ((DAT_00281e18 == param_1) && (iVar4 = FUN_0017df44(param_1), iVar4 != 0)) {
            uVar11 = 3;
          }
          else if ((DAT_00281eb0 == param_1) && (iVar4 = FUN_0017df44(param_1), iVar4 != 0)) {
            uVar11 = 4;
          }
          else {
            if ((DAT_00281f48 != param_1) || (iVar4 = FUN_0017df44(param_1), iVar4 == 0))
            goto LAB_00175c68;
            uVar11 = 5;
          }
          uVar1 = DAT_00282b08;
          if (DAT_0028212c != 0) {
            iVar4 = FUN_00191e38(uVar11);
            if (iVar4 == 0) {
              uVar1 = DAT_00282b08 + 1;
              if ((DAT_00282b08 < 0x20) && (DAT_001a7d00 != (code *)0x0)) {
                pcVar10 = "save_failed";
                goto LAB_00175f24;
              }
            }
            else {
              DAT_00282118 = 0;
              uVar1 = DAT_00282b08 + 1;
              if ((DAT_00282b08 < 0x20) && (DAT_001a7d00 != (code *)0x0)) {
                pcVar10 = "saved";
LAB_00175f24:
                DAT_00282b08 = DAT_00282b08 + 1;
                pcVar8 = "script_port_font_ui";
                goto LAB_00175f2c;
              }
            }
          }
        }
      }
      goto switchD_00175df0_default;
    }
LAB_00175c68:
    if ((((DAT_0028398c == 0) || (DAT_00283980 == 0)) || (iVar4 = FUN_0016d910(), iVar4 == 0)) ||
       (iVar4 = FUN_0016d5cc(DAT_00283340,DAT_001a7d30), iVar4 == 0)) {
LAB_00175e00:
      if ((DAT_001a7d10 != 2) || (uVar6 = FUN_00193f80(1,&DAT_001a7d08), (uVar6 & 1) != 0))
      goto LAB_00175e40;
      if (DAT_001a7d14 == 0) {
        uVar7 = FUN_0016d910();
        if (((int)uVar7 != 0) && (uVar7 = FUN_0016d5cc(DAT_001a7d38,DAT_001a7d30), (int)uVar7 != 0))
        {
          iVar4 = FUN_0016e464(DAT_001a7d38,1);
          uVar7 = 0;
          if ((iVar4 != 0) && (uVar6 = (ulong)DAT_001a7d18, uVar6 != 0)) {
            uVar14 = 0;
            plVar13 = &DAT_001c3568;
            do {
              if ((((*(int *)((long)plVar13 + 0xd4) != 0) && ((int)plVar13[4] == 2)) &&
                  (*(char *)((long)plVar13 + 0x4c) != '\0')) && (*plVar13 == param_1)) {
                iVar4 = FUN_0017609c(plVar13);
                if (iVar4 != 0) {
                  iVar4 = FUN_00176418(param_1,(int)plVar13[5]);
                  if (iVar4 == 0) {
                    uVar11 = 0;
                  }
                  else {
                    uVar11 = (undefined4)plVar13[5];
                  }
                  uVar7 = 1;
                  *param_2 = uVar11;
                  goto LAB_00175e38;
                }
                uVar6 = (ulong)DAT_001a7d18;
              }
              uVar14 = uVar14 + 1;
              plVar13 = plVar13 + 0x1b;
            } while (uVar14 < uVar6);
            goto LAB_00175e34;
          }
        }
      }
      else {
LAB_00175e34:
        uVar7 = 0;
      }
LAB_00175e38:
      DAT_001a7d08 = 0;
      goto LAB_00176044;
    }
    if ((DAT_002833a0 == param_1) && (iVar4 = FUN_0017c598(param_1), iVar4 != 0)) {
      uVar1 = DAT_00282b08;
      if (DAT_00283988 == 0) {
        DAT_00283988 = 1;
        uVar9 = DAT_002839f4 + 1;
        bVar3 = DAT_002839f4 < 0x30;
        DAT_002839f4 = uVar9;
        if ((bVar3) && (DAT_001a7d00 != (code *)0x0)) {
          pcVar8 = "script_port_camera";
          pcVar10 = "popup_open";
          goto LAB_00175d08;
        }
      }
      goto switchD_00175df0_default;
    }
    if ((DAT_00283418 == param_1) && (iVar4 = FUN_0017c598(param_1), iVar4 != 0)) {
      uVar11 = 1;
    }
    else if ((DAT_00283490 == param_1) && (iVar4 = FUN_0017c598(param_1), iVar4 != 0)) {
      uVar11 = 2;
    }
    else if ((DAT_00283508 == param_1) && (iVar4 = FUN_0017c598(param_1), iVar4 != 0)) {
      uVar11 = 3;
    }
    else if ((DAT_00283580 == param_1) && (iVar4 = FUN_0017c598(param_1), iVar4 != 0)) {
      uVar11 = 4;
    }
    else {
      if ((DAT_002835f8 != param_1) || (iVar4 = FUN_0017c598(param_1), iVar4 == 0))
      goto LAB_00175e00;
      uVar11 = 5;
    }
    uVar1 = DAT_00282b08;
    if (DAT_00283988 == 0) goto switchD_00175df0_default;
    switch(uVar11) {
    case 2:
      DAT_00283988 = 0;
      uVar7 = 5;
      break;
    case 3:
      uVar7 = 2;
      uVar1 = DAT_002839c8 + 1;
      uVar9 = uVar1 & 3;
      if ((int)uVar1 < 1) {
        uVar9 = -(-uVar1 & 3);
      }
      goto LAB_00176038;
    case 4:
      uVar7 = 3;
      break;
    case 5:
      uVar7 = 4;
      break;
    default:
      goto switchD_00175df0_default;
    }
    uVar9 = 0;
LAB_00176038:
    FUN_00191c34(uVar7,0,uVar9);
    uVar1 = DAT_00282b08;
  }
  else if ((DAT_00282178 != param_1) ||
          (iVar4 = FUN_00179dfc(param_1), uVar1 = DAT_00282b08, iVar4 == 0)) {
    if ((DAT_002822d0 == param_1) && (iVar4 = FUN_00179dfc(param_1), iVar4 != 0)) {
      DAT_00282af8 = 0;
      uVar1 = DAT_00282b08;
    }
    else if ((DAT_00282428 == param_1) && (iVar4 = FUN_00179dfc(param_1), iVar4 != 0)) {
      uVar1 = DAT_00282b08;
      if (DAT_00282afc != DAT_00282b00) {
        bVar3 = DAT_00282afc - 1U < 0x91;
        iVar4 = DAT_00282afc;
        iVar5 = DAT_00282afc;
        iVar12 = DAT_00282afc;
        if (DAT_00282afc == 0x91) {
LAB_00175f44:
          iVar5 = 0;
          iVar12 = 0x91;
        }
        DAT_00282b04 = 1;
        if (!bVar3) {
          iVar5 = -1;
        }
        iVar5 = FUN_00191cd0(iVar5);
        if ((0xffffff6e < iVar12 - 0x92U) && (DAT_00282af8 != 0)) {
          if (iVar5 != 0) {
            DAT_00282afc = iVar12;
            DAT_00282b00 = iVar12;
          }
          DAT_00282b04 = 0;
        }
        uVar1 = DAT_00282b0c + 1;
        bVar3 = DAT_00282b0c < 0x18;
        DAT_00282b0c = uVar1;
        if ((bVar3) && (DAT_001a7d00 != (code *)0x0)) {
          pcVar8 = "save_failed";
          if (iVar5 != 0) {
            pcVar8 = "saved";
          }
          (*DAT_001a7d00)(DAT_001a7cc8,"script_port_fps",pcVar8,iVar12);
        }
        uVar1 = DAT_00282b08;
        if ((iVar5 != 0) && (iVar5 = FUN_0016e7c0(), uVar1 = DAT_00282b08, iVar5 != 0))
        goto LAB_00175fec;
      }
    }
    else {
      if ((DAT_00282580 != param_1) || (iVar4 = FUN_00179dfc(param_1), iVar4 == 0))
      goto LAB_00175abc;
      if (DAT_00282b00 != 0x91) {
        bVar3 = true;
        iVar4 = 0x91;
        goto LAB_00175f44;
      }
      iVar4 = FUN_0016e7c0();
      uVar1 = DAT_00282b08;
      if (iVar4 == 0) goto switchD_00175df0_default;
      iVar4 = 0x91;
      DAT_00282afc = 0x91;
LAB_00175fec:
      *(int *)(DAT_00282168 + 0xe8) = iVar4;
      uVar1 = DAT_00282b08;
    }
  }
switchD_00175df0_default:
  DAT_00282b08 = uVar1;
  uVar7 = 1;
  *param_2 = 0;
LAB_00176044:
  if (*(long *)(lVar2 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar7);
  }
  return;
}

/* ===== FUN_0017a478 @ 0017a478 ===== */

void FUN_0017a478(void)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  float fVar6;
  float fVar7;
  ulong uVar8;
  bool bVar9;
  bool bVar10;
  int iVar11;
  ulong uVar12;
  ulong uVar13;
  long lVar14;
  ulong uVar15;
  float *pfVar16;
  char *pcVar17;
  undefined4 uVar18;
  uint uVar19;
  ulong uVar20;
  ulong *puVar21;
  undefined8 uVar22;
  float fVar23;
  float fVar24;
  long local_e8;
  ulong local_d0;
  ulong uStack_c8;
  ulong local_c0;
  float fStack_b8;
  ulong local_b0;
  long local_a8;
  
  lVar2 = tpidr_el0;
  local_a8 = *(long *)(lVar2 + 0x28);
  DAT_002839a8 = 1;
  uVar12 = FUN_0017bd68();
  if ((int)uVar12 != 0) {
    DAT_002839a8 = 2;
    uVar13 = FUN_0016cdf0(0x80);
    uVar12 = uVar13;
    if (uVar13 != 0) {
      FUN_0016ce08();
      local_c0 = 0;
      if (uVar13 + 8 >> 3 < 0x201) {
        bVar10 = false;
      }
      else {
        iVar11 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar13,&local_c0,8);
        bVar10 = iVar11 == 1;
      }
      bVar9 = false;
      if (0xfff < local_c0) {
        bVar9 = bVar10;
      }
      uVar12 = local_c0;
      if (!(bool)(bVar9 & (local_c0 & 7) == 0)) {
        uVar12 = 0;
      }
      if (uVar12 == DAT_001a7cd0 + 0x11abcf0U) {
        local_d0 = local_d0 & 0xffffffff00000000;
        local_c0 = 1;
        if (((((0xfff < uVar13 + 0x38) && ((uVar13 & 0xfffffffffffffff8) != 0xffffffffffffffc0)) &&
             (iVar11 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar13 + 0x38,&local_c0,8), iVar11 == 1)) &&
            ((local_c0 == 0 && (0xfff < uVar13 + 0x40)))) &&
           (((uVar13 & 0xfffffffffffffffc) != 0xffffffffffffffbc &&
            ((iVar11 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar13 + 0x40,&local_d0,4), iVar11 == 1 &&
             ((float)local_d0 == -NAN)))))) {
          DAT_00283338 = DAT_001a7d30;
          DAT_00283340 = uVar13;
          FUN_0016ce20(uVar13,1);
          FUN_0016cf5c(0xc61c3c00,0xc61c3c00,uVar13);
          FUN_0016cfb8(DAT_001a7d28,uVar13);
          uVar12 = FUN_0016d5cc(uVar13,DAT_001a7d30);
          if ((int)uVar12 == 0) goto LAB_0017a5fc;
          DAT_002839a8 = 3;
          uVar12 = FUN_0017bf48(&DAT_002833a0,5,"CAMERA");
          if (((int)uVar12 == 0) ||
             (uVar12 = FUN_0017bf48(&DAT_00283418,6,&DAT_00134f22), (int)uVar12 == 0))
          goto LAB_0017a5fc;
          DAT_002839a8 = 4;
          DAT_00283348 = FUN_0016ce48("popup_generic");
          local_c0 = 0;
          if (DAT_00283348 + 8 >> 3 < 0x201) {
            bVar10 = false;
          }
          else {
            iVar11 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00283348,&local_c0,8);
            bVar10 = iVar11 == 1;
          }
          uVar12 = DAT_00283348;
          bVar9 = false;
          if (0xfff < local_c0) {
            bVar9 = bVar10;
          }
          uVar15 = local_c0;
          if (!(bool)(bVar9 & (local_c0 & 7) == 0)) {
            uVar15 = 0;
          }
          if (uVar15 == DAT_001a7cd0 + 0x11ad208U) {
            local_d0 = local_d0 & 0xffffffff00000000;
            local_c0 = 1;
            if ((((0xfff < DAT_00283348 + 0x38) &&
                 ((DAT_00283348 & 0xfffffffffffffff8) != 0xffffffffffffffc0)) &&
                (iVar11 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00283348 + 0x38,&local_c0,8), iVar11 == 1
                )) && (((local_c0 == 0 && (uVar15 = uVar12 + 0x40, 0xfff < uVar15)) &&
                       (((uVar12 & 0xfffffffffffffffc) != 0xffffffffffffffbc &&
                        ((iVar11 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar15,&local_d0,4), iVar11 == 1 &&
                         ((float)local_d0 == -NAN)))))))) {
              FUN_0016ce84(DAT_00283348,0);
              lVar14 = FUN_0017c284(DAT_00283348,"button_negative");
              if (lVar14 != 0) {
                *(undefined1 *)(lVar14 + 8) = 0;
              }
              lVar14 = FUN_0017c284(DAT_00283348,"button_no");
              if (lVar14 != 0) {
                *(undefined1 *)(lVar14 + 8) = 0;
              }
              lVar14 = FUN_0017c284(DAT_00283348,"button_yes");
              if (lVar14 != 0) {
                *(undefined1 *)(lVar14 + 8) = 0;
              }
              lVar14 = FUN_0017c284(DAT_00283348,"button_ok");
              if (lVar14 != 0) {
                *(undefined1 *)(lVar14 + 8) = 0;
              }
              lVar14 = FUN_0017c284(DAT_00283348,"button_close");
              if (lVar14 != 0) {
                *(undefined1 *)(lVar14 + 8) = 0;
              }
              uVar15 = FUN_0016ce98(DAT_00283348,&DAT_00134a97);
              uVar12 = DAT_00283348;
              if (uVar15 != 0) {
                uVar19 = 0;
                uVar20 = uVar15;
                do {
                  if ((uVar20 == uVar12) || (0xf < uVar19)) goto LAB_0017a94c;
                  local_c0 = 0;
                  if (uVar20 + 0x40 >> 3 < 0x201) {
                    bVar10 = false;
                  }
                  else {
                    iVar11 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar20 + 0x38,&local_c0,8);
                    bVar10 = iVar11 == 1;
                  }
                  uVar8 = local_c0;
                  bVar9 = false;
                  if (0xfff < local_c0) {
                    bVar9 = bVar10;
                  }
                  bVar9 = (bool)(bVar9 & (local_c0 & 7) == 0);
                  uVar1 = local_c0;
                  if (!bVar9) {
                    uVar1 = 0;
                  }
                  iVar11 = FUN_0016d5cc(uVar20,uVar1);
                  if (iVar11 == 0) goto LAB_0017a960;
                  uVar19 = uVar19 + 1;
                  uVar20 = uVar8;
                } while (bVar9);
                uVar20 = 0;
LAB_0017a94c:
                if (uVar20 == uVar12) {
                  *(undefined1 *)(uVar15 + 8) = 0;
                }
              }
LAB_0017a960:
              uVar15 = FUN_0016ce98(DAT_00283348,"title");
              uVar12 = DAT_00283348;
              if (uVar15 != 0) {
                uVar19 = 0;
                uVar20 = uVar15;
                do {
                  if ((uVar20 == uVar12) || (0xf < uVar19)) goto LAB_0017aa1c;
                  local_c0 = 0;
                  if (uVar20 + 0x40 >> 3 < 0x201) {
                    bVar10 = false;
                  }
                  else {
                    iVar11 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar20 + 0x38,&local_c0,8);
                    bVar10 = iVar11 == 1;
                  }
                  uVar8 = local_c0;
                  bVar9 = false;
                  if (0xfff < local_c0) {
                    bVar9 = bVar10;
                  }
                  bVar9 = (bool)(bVar9 & (local_c0 & 7) == 0);
                  uVar1 = local_c0;
                  if (!bVar9) {
                    uVar1 = 0;
                  }
                  iVar11 = FUN_0016d5cc(uVar20,uVar1);
                  if (iVar11 == 0) goto LAB_0017aa30;
                  uVar19 = uVar19 + 1;
                  uVar20 = uVar8;
                } while (bVar9);
                uVar20 = 0;
LAB_0017aa1c:
                if (uVar20 == uVar12) {
                  *(undefined1 *)(uVar15 + 8) = 0;
                }
              }
LAB_0017aa30:
              FUN_0016ce20(DAT_00283348,0);
              FUN_0016cfa4(uVar13,DAT_00283348);
              DAT_002839a8 = 5;
              uVar12 = FUN_0017bf48(&DAT_00283490,3,&DAT_00136344);
              if (((((int)uVar12 != 0) &&
                   (uVar12 = FUN_0017bf48(&DAT_00283508,1,"MODE 1"), (int)uVar12 != 0)) &&
                  (uVar12 = FUN_0017bf48(&DAT_00283580,1,"ZOOM 0.60x"), (int)uVar12 != 0)) &&
                 (uVar12 = FUN_0017bf48(&DAT_002835f8,1,"RESET"), uVar5 = DAT_0010f8d0,
                 uVar4 = DAT_0010f8b0, (int)uVar12 != 0)) {
                local_e8 = 0;
                do {
                  DAT_002839a8 = (int)local_e8 + 8;
                  uVar12 = FUN_0016ce48("edit_controls_ui");
                  local_c0 = 0;
                  if (uVar12 + 8 >> 3 < 0x201) {
                    bVar10 = false;
                  }
                  else {
                    iVar11 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar12,&local_c0,8);
                    bVar10 = iVar11 == 1;
                  }
                  bVar9 = false;
                  if (0xfff < local_c0) {
                    bVar9 = bVar10;
                  }
                  uVar13 = local_c0;
                  if (!(bool)(bVar9 & (local_c0 & 7) == 0)) {
                    uVar13 = 0;
                  }
                  if (uVar13 != DAT_001a7cd0 + 0x11ad208U) {
LAB_0017b2ac:
                    uVar12 = 0;
                    uVar19 = DAT_002839f4 + 1;
                    bVar10 = 0x2f < DAT_002839f4;
                    DAT_002839f4 = uVar19;
                    if ((bVar10) || (DAT_001a7d00 == (code *)0x0)) goto LAB_0017a5fc;
                    pcVar17 = "slider_asset_contract";
LAB_0017b2e8:
                    (*DAT_001a7d00)(DAT_001a7cc8,"script_port_camera",pcVar17,local_e8);
                    goto LAB_0017a5f8;
                  }
                  local_d0 = local_d0 & 0xffffffff00000000;
                  local_c0 = 1;
                  if (((((uVar12 + 0x38 < 0x1000) ||
                        ((uVar12 & 0xfffffffffffffff8) == 0xffffffffffffffc0)) ||
                       ((iVar11 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar12 + 0x38,&local_c0,8),
                        iVar11 != 1 || ((local_c0 != 0 || (uVar12 + 0x40 < 0x1000)))))) ||
                      ((uVar12 & 0xfffffffffffffffc) == 0xffffffffffffffbc)) ||
                     ((iVar11 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar12 + 0x40,&local_d0,4), iVar11 != 1
                      || ((float)local_d0 != -NAN)))) goto LAB_0017b2ac;
                  *(ulong *)(&DAT_00283378 + local_e8 * 8) = uVar12;
                  FUN_0016ce84(uVar12,0);
                  uVar12 = FUN_0017c284(uVar12,"slider_scale");
                  if (uVar12 == 0) {
                    uVar19 = DAT_002839f4 + 1;
                    bVar10 = DAT_002839f4 < 0x30;
                    uVar12 = 0;
                    DAT_002839f4 = uVar19;
                    if ((bVar10) && (DAT_001a7d00 != (code *)0x0)) {
                      pcVar17 = "slider_scale_contract";
                      goto LAB_0017b2e8;
                    }
                    goto LAB_0017a5fc;
                  }
                  lVar14 = FUN_0017c284(uVar12,"slider_bg");
                  uVar13 = FUN_0017c284(uVar12,"slider_button");
                  if ((lVar14 == 0) || (uVar13 == 0)) {
                    uVar12 = 0;
                    uVar19 = DAT_002839f4 + 1;
                    bVar10 = DAT_002839f4 < 0x30;
                    DAT_002839f4 = uVar19;
                    if ((bVar10) && (DAT_001a7d00 != (code *)0x0)) {
                      pcVar17 = "slider_children_contract";
                      goto LAB_0017b2e8;
                    }
                    goto LAB_0017a5fc;
                  }
                  FUN_0016cf5c(0,0,lVar14);
                  FUN_0016cf5c(0,0,uVar13);
                  FUN_0016cf84(uVar13,&local_c0);
                  uVar15 = (*(code *)(DAT_001a7cd0 + 0x5d76d4))(uVar13,"bounds");
                  if (uVar15 == 0) {
LAB_0017ad74:
                    uStack_c8 = (ulong)(uint)fStack_b8;
                    bVar10 = false;
                    local_d0 = local_c0;
                  }
                  else {
                    uVar19 = 0;
                    uVar20 = uVar15;
                    do {
                      if ((uVar20 == uVar13) || (0xf < uVar19)) goto LAB_0017ad50;
                      local_b0 = 0;
                      if (uVar20 + 0x40 >> 3 < 0x201) {
                        bVar10 = false;
                      }
                      else {
                        iVar11 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar20 + 0x38,&local_b0,8);
                        bVar10 = iVar11 == 1;
                      }
                      uVar8 = local_b0;
                      bVar9 = false;
                      if (0xfff < local_b0) {
                        bVar9 = bVar10;
                      }
                      bVar9 = (bool)(bVar9 & (local_b0 & 7) == 0);
                      uVar1 = local_b0;
                      if (!bVar9) {
                        uVar1 = 0;
                      }
                      iVar11 = FUN_0016d5cc(uVar20,uVar1);
                      if (iVar11 == 0) goto LAB_0017ad74;
                      uVar19 = uVar19 + 1;
                      uVar20 = uVar8;
                    } while (bVar9);
                    uVar20 = 0;
LAB_0017ad50:
                    if (uVar20 != uVar13) goto LAB_0017ad74;
                    FUN_0016cf84(uVar15,&local_d0);
                    bVar10 = true;
                  }
                  uVar15 = FUN_0016cdf0(0x178);
                  if (uVar15 == 0) goto LAB_0017a5f8;
                  (*(code *)(DAT_001a7cd0 + 0x889288))(uVar15,lVar14,uVar13,0,1);
                  local_b0 = 0;
                  if (uVar15 + 8 >> 3 < 0x201) {
                    bVar9 = false;
                  }
                  else {
                    iVar11 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar15,&local_b0,8);
                    bVar9 = iVar11 == 1;
                  }
                  bVar3 = false;
                  if (0xfff < local_b0) {
                    bVar3 = bVar9;
                  }
                  uVar20 = local_b0;
                  if (!(bool)(bVar3 & (local_b0 & 7) == 0)) {
                    uVar20 = 0;
                  }
                  if (uVar20 != DAT_001a7cd0 + 0x11c0c58U) {
LAB_0017b270:
                    uVar19 = DAT_002839f4 + 1;
                    bVar10 = DAT_002839f4 < 0x30;
                    DAT_002839f4 = uVar19;
                    if ((bVar10) && (DAT_001a7d00 != (code *)0x0)) {
                      pcVar17 = "slider_constructor_contract";
                      goto LAB_0017b2e8;
                    }
                    goto LAB_0017a5f8;
                  }
                  uVar19 = 0;
                  uVar20 = uVar15;
                  do {
                    if ((uVar20 == uVar12) || (0xf < uVar19)) goto LAB_0017aebc;
                    local_b0 = 0;
                    if (uVar20 + 0x40 >> 3 < 0x201) {
                      bVar9 = false;
                    }
                    else {
                      iVar11 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar20 + 0x38,&local_b0,8);
                      bVar9 = iVar11 == 1;
                    }
                    uVar8 = local_b0;
                    bVar3 = false;
                    if (0xfff < local_b0) {
                      bVar3 = bVar9;
                    }
                    bVar3 = (bool)(bVar3 & (local_b0 & 7) == 0);
                    uVar1 = local_b0;
                    if (!bVar3) {
                      uVar1 = 0;
                    }
                    iVar11 = FUN_0016d5cc(uVar20,uVar1);
                    if (iVar11 == 0) goto LAB_0017b270;
                    uVar19 = uVar19 + 1;
                    uVar20 = uVar8;
                  } while (bVar3);
                  uVar20 = 0;
LAB_0017aebc:
                  if (uVar20 != uVar12) goto LAB_0017b270;
                  local_b0 = 0;
                  if (uVar15 + 0x130 >> 3 < 0x201) {
                    bVar9 = false;
                  }
                  else {
                    iVar11 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar15 + 0x128,&local_b0,8);
                    bVar9 = iVar11 == 1;
                  }
                  bVar3 = false;
                  if (0xfff < local_b0) {
                    bVar3 = bVar9;
                  }
                  uVar12 = local_b0;
                  if (!(bool)(bVar3 & (local_b0 & 7) == 0)) {
                    uVar12 = 0;
                  }
                  if (uVar12 != uVar13) goto LAB_0017b270;
                  FUN_0016cfa4(DAT_00283340,uVar15);
                  bVar9 = local_e8 == 0;
                  uVar18 = 200;
                  if (!bVar9) {
                    uVar18 = 10000;
                  }
                  (&DAT_00283350)[local_e8] = uVar15;
                  *(undefined4 *)(uVar15 + 0xf0) = uVar18;
                  uVar22 = NEON_cmlt(CONCAT44((uint)bVar9 << 0x1f,(uint)bVar9 << 0x1f),0,4);
                  uVar22 = NEON_bsl(uVar22,uVar5,uVar4,1);
                  *(undefined8 *)(uVar15 + 0xe8) = uVar22;
                  if (bVar10) {
                    pfVar16 = (float *)FUN_0016f79c(uVar15 + 0x138);
                    fVar23 = fStack_b8;
                    local_b0 = local_b0 & 0xffffffff00000000;
                    if (0x400 < (ulong)(pfVar16 + 1) >> 2) {
                      fVar7 = (float)local_c0;
                      fVar6 = (float)uStack_c8;
                      fVar24 = (float)local_d0;
                      iVar11 = (*DAT_001a7ce8)(DAT_001a7cc8,pfVar16,&local_b0,4);
                      if (iVar11 == 1) {
                        if ((ABS((float)local_b0) != INFINITY) && (!NAN(ABS((float)local_b0)))) {
                          fVar23 = ((fVar23 - fVar7) - (fVar6 - fVar24)) * 0.5;
                          fVar24 = ABS(fVar23);
                          if ((fVar24 != INFINITY) &&
                             (fVar24 == 512.0 || fVar24 < 512.0 != NAN(fVar24))) {
                            fVar23 = (float)local_b0 - fVar23;
                            local_b0 = CONCAT44(local_b0._4_4_,fVar23);
                            *pfVar16 = fVar23;
                            goto LAB_0017b034;
                          }
                        }
                      }
                    }
                    uVar19 = DAT_002839f4 + 1;
                    bVar10 = DAT_002839f4 < 0x30;
                    DAT_002839f4 = uVar19;
                    if ((bVar10) && (DAT_001a7d00 != (code *)0x0)) {
                      pcVar17 = "slider_bounds_contract";
                      goto LAB_0017b2e8;
                    }
                    goto LAB_0017a5f8;
                  }
LAB_0017b034:
                  FUN_0016cf5c(0xc61c3c00,0xc61c3c00,uVar15);
                  iVar11 = FUN_0017bb9c(local_e8);
                  if (iVar11 == 0) {
                    uVar19 = DAT_002839f4 + 1;
                    bVar10 = DAT_002839f4 < 0x30;
                    DAT_002839f4 = uVar19;
                    if ((bVar10) && (DAT_001a7d00 != (code *)0x0)) {
                      pcVar17 = "slider_ownership_contract";
                      goto LAB_0017b2e8;
                    }
                    goto LAB_0017a5f8;
                  }
                  local_e8 = local_e8 + 1;
                } while (local_e8 != 5);
                lVar14 = 0;
                puVar21 = &DAT_00283678;
                do {
                  DAT_002839a8 = (uint)lVar14 | 0x10;
                  nexus_rich_asset(5);
                  uVar12 = FUN_0016ce48();
                  *puVar21 = uVar12;
                  uVar12 = FUN_0016dc20(uVar12,5,0);
                  if ((int)uVar12 == 0) goto LAB_0017a5fc;
                  uVar12 = *puVar21;
                  local_c0 = 1;
                  local_d0 = local_d0 & 0xffffffff00000000;
                  if (((((uVar12 + 0x38 < 0x1000) ||
                        ((uVar12 & 0xfffffffffffffff8) == 0xffffffffffffffc0)) ||
                       (iVar11 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar12 + 0x38,&local_c0,8),
                       iVar11 != 1)) || ((local_c0 != 0 || (uVar12 + 0x40 < 0x1000)))) ||
                     (((uVar12 & 0xfffffffffffffffc) == 0xffffffffffffffbc ||
                      ((iVar11 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar12 + 0x40,&local_d0,4),
                       iVar11 != 1 || ((float)local_d0 != -NAN)))))) goto LAB_0017a5f8;
                  FUN_0016ce84(*puVar21,0);
                  uVar12 = FUN_0016ce98(*puVar21,&DAT_00134a97);
                  local_c0 = 0;
                  puVar21[1] = uVar12;
                  if (uVar12 + 8 >> 3 < 0x201) {
                    bVar10 = false;
                  }
                  else {
                    iVar11 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar12,&local_c0,8);
                    bVar10 = iVar11 == 1;
                  }
                  bVar9 = false;
                  if (0xfff < local_c0) {
                    bVar9 = bVar10;
                  }
                  uVar12 = local_c0;
                  if (!(bool)(bVar9 & (local_c0 & 7) == 0)) {
                    uVar12 = 0;
                  }
                  if (uVar12 != DAT_001a7cd0 + 0x11abad8U) goto LAB_0017a5f8;
                  uVar12 = FUN_0016e020(*puVar21,puVar21[1]);
                  if ((int)uVar12 == 0) goto LAB_0017a5fc;
                  FUN_0016ce20(*puVar21,0);
                  FUN_0016cf5c(0xc61c3c00,0xc61c3c00,*puVar21);
                  FUN_0016cfa4(DAT_00283340,*puVar21);
                  uVar12 = FUN_0017c598(*puVar21);
                  if ((int)uVar12 == 0) goto LAB_0017a5fc;
                  lVar14 = lVar14 + 1;
                  puVar21 = puVar21 + 0xf;
                } while (lVar14 != 6);
                uVar12 = 1;
                DAT_00283980 = 1;
                uVar19 = DAT_002839f4 + 1;
                bVar10 = DAT_002839f4 < 0x30;
                DAT_002839f4 = uVar19;
                if ((bVar10) && (DAT_001a7d00 != (code *)0x0)) {
                  (*DAT_001a7d00)(DAT_001a7cc8,"script_port_camera","native_sliders_created",5);
                  uVar12 = 1;
                }
              }
              goto LAB_0017a5fc;
            }
          }
        }
      }
LAB_0017a5f8:
      uVar12 = 0;
    }
  }
LAB_0017a5fc:
  if (*(long *)(lVar2 + 0x28) != local_a8) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar12);
  }
  return;
}

/* ===== FUN_0017d01c @ 0017d01c ===== */

void FUN_0017d01c(void)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  ulong uVar4;
  bool bVar5;
  int iVar6;
  ulong uVar7;
  long lVar8;
  ulong uVar9;
  ulong uVar10;
  uint uVar11;
  int local_64;
  ulong local_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  uVar7 = FUN_0017bd68();
  if (((int)uVar7 == 0) || (uVar7 = FUN_0016cdf0(0x80), DAT_00281b10 = uVar7, uVar7 == 0))
  goto LAB_0017d17c;
  FUN_0016ce08();
  local_60 = 0;
  if (DAT_00281b10 + 8 >> 3 < 0x201) {
    bVar5 = false;
  }
  else {
    iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00281b10,&local_60,8);
    bVar5 = iVar6 == 1;
  }
  uVar7 = DAT_00281b10;
  bVar3 = false;
  if (0xfff < local_60) {
    bVar3 = bVar5;
  }
  uVar9 = local_60;
  if (!(bool)(bVar3 & (local_60 & 7) == 0)) {
    uVar9 = 0;
  }
  if (uVar9 == DAT_001a7cd0 + 0x11abcf0U) {
    local_64 = 0;
    local_60 = 1;
    if ((((((0xfff < DAT_00281b10 + 0x38) &&
           ((DAT_00281b10 & 0xfffffffffffffff8) != 0xffffffffffffffc0)) &&
          (iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00281b10 + 0x38,&local_60,8), iVar6 == 1)) &&
         ((local_60 == 0 && (uVar9 = uVar7 + 0x40, 0xfff < uVar9)))) &&
        ((uVar7 & 0xfffffffffffffffc) != 0xffffffffffffffbc)) &&
       ((iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar9,&local_64,4), iVar6 == 1 && (local_64 == -1))))
    {
      DAT_00281b08 = DAT_001a7d30;
      FUN_0016ce20(DAT_00281b10,1);
      FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_00281b10);
      FUN_0016cfb8(DAT_001a7d28,DAT_00281b10);
      uVar7 = FUN_0016d5cc(DAT_00281b10,DAT_001a7d30);
      if (((int)uVar7 == 0) ||
         (uVar7 = FUN_0017d8c4(&DAT_00281b20,6,&DAT_00134f22), (int)uVar7 == 0)) goto LAB_0017d17c;
      DAT_00281b18 = FUN_0016ce48("popup_generic");
      local_60 = 0;
      if (DAT_00281b18 + 8 >> 3 < 0x201) {
        bVar5 = false;
      }
      else {
        iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00281b18,&local_60,8);
        bVar5 = iVar6 == 1;
      }
      uVar7 = DAT_00281b18;
      bVar3 = false;
      if (0xfff < local_60) {
        bVar3 = bVar5;
      }
      uVar9 = local_60;
      if (!(bool)(bVar3 & (local_60 & 7) == 0)) {
        uVar9 = 0;
      }
      if (uVar9 == DAT_001a7cd0 + 0x11ad208U) {
        local_64 = 0;
        local_60 = 1;
        if ((((0xfff < DAT_00281b18 + 0x38) &&
             ((DAT_00281b18 & 0xfffffffffffffff8) != 0xffffffffffffffc0)) &&
            (iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00281b18 + 0x38,&local_60,8), iVar6 == 1)) &&
           (((local_60 == 0 && (uVar9 = uVar7 + 0x40, 0xfff < uVar9)) &&
            (((uVar7 & 0xfffffffffffffffc) != 0xffffffffffffffbc &&
             ((iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar9,&local_64,4), iVar6 == 1 &&
              (local_64 == -1)))))))) {
          FUN_0016ce84(DAT_00281b18,0);
          lVar8 = FUN_0017c284(DAT_00281b18,"button_negative");
          if (lVar8 != 0) {
            *(undefined1 *)(lVar8 + 8) = 0;
          }
          lVar8 = FUN_0017c284(DAT_00281b18,"button_no");
          if (lVar8 != 0) {
            *(undefined1 *)(lVar8 + 8) = 0;
          }
          lVar8 = FUN_0017c284(DAT_00281b18,"button_yes");
          if (lVar8 != 0) {
            *(undefined1 *)(lVar8 + 8) = 0;
          }
          lVar8 = FUN_0017c284(DAT_00281b18,"button_ok");
          if (lVar8 != 0) {
            *(undefined1 *)(lVar8 + 8) = 0;
          }
          lVar8 = FUN_0017c284(DAT_00281b18,"button_close");
          if (lVar8 != 0) {
            *(undefined1 *)(lVar8 + 8) = 0;
          }
          uVar9 = FUN_0016ce98(DAT_00281b18,&DAT_00134a97);
          uVar7 = DAT_00281b18;
          if (uVar9 != 0) {
            uVar11 = 0;
            uVar10 = uVar9;
            do {
              if ((uVar10 == uVar7) || (0xf < uVar11)) goto LAB_0017d478;
              local_60 = 0;
              if (uVar10 + 0x40 >> 3 < 0x201) {
                bVar5 = false;
              }
              else {
                iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar10 + 0x38,&local_60,8);
                bVar5 = iVar6 == 1;
              }
              uVar4 = local_60;
              bVar3 = false;
              if (0xfff < local_60) {
                bVar3 = bVar5;
              }
              bVar3 = (bool)(bVar3 & (local_60 & 7) == 0);
              uVar1 = local_60;
              if (!bVar3) {
                uVar1 = 0;
              }
              iVar6 = FUN_0016d5cc(uVar10,uVar1);
              if (iVar6 == 0) goto LAB_0017d484;
              uVar11 = uVar11 + 1;
              uVar10 = uVar4;
            } while (bVar3);
            uVar10 = 0;
LAB_0017d478:
            if (uVar10 == uVar7) {
              *(undefined1 *)(uVar9 + 8) = 0;
            }
          }
LAB_0017d484:
          uVar9 = FUN_0016ce98(DAT_00281b18,"title");
          uVar7 = DAT_00281b18;
          if (uVar9 != 0) {
            uVar11 = 0;
            uVar10 = uVar9;
            do {
              if ((uVar10 == uVar7) || (0xf < uVar11)) goto LAB_0017d538;
              local_60 = 0;
              if (uVar10 + 0x40 >> 3 < 0x201) {
                bVar5 = false;
              }
              else {
                iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar10 + 0x38,&local_60,8);
                bVar5 = iVar6 == 1;
              }
              uVar4 = local_60;
              bVar3 = false;
              if (0xfff < local_60) {
                bVar3 = bVar5;
              }
              bVar3 = (bool)(bVar3 & (local_60 & 7) == 0);
              uVar1 = local_60;
              if (!bVar3) {
                uVar1 = 0;
              }
              iVar6 = FUN_0016d5cc(uVar10,uVar1);
              if (iVar6 == 0) goto LAB_0017d544;
              uVar11 = uVar11 + 1;
              uVar10 = uVar4;
            } while (bVar3);
            uVar10 = 0;
LAB_0017d538:
            if (uVar10 == uVar7) {
              *(undefined1 *)(uVar9 + 8) = 0;
            }
          }
LAB_0017d544:
          FUN_0016ce20(DAT_00281b18,0);
          FUN_0016cfa4(DAT_00281b10,DAT_00281b18);
          uVar7 = FUN_0017d8c4(&DAT_00281bb8,3,&DAT_00136344);
          if ((((((int)uVar7 != 0) &&
                (uVar7 = FUN_0017d8c4(&DAT_00281c50,1,"DEFAULT"), (int)uVar7 != 0)) &&
               (uVar7 = FUN_0017d8c4(&DAT_00281ce8,1,"PUSIA BOLD"), (int)uVar7 != 0)) &&
              (((uVar7 = FUN_0017d8c4(&DAT_00281d80,1,"NICE BRAWL"), (int)uVar7 != 0 &&
                (uVar7 = FUN_0017d8c4(&DAT_00281e18,1,"GHOUL STARS"), (int)uVar7 != 0)) &&
               ((uVar7 = FUN_0017d8c4(&DAT_00281eb0,1,"IMPACT"), (int)uVar7 != 0 &&
                ((uVar7 = FUN_0017d8c4(&DAT_00281f48,1,"GENSHIN IMPACT"), (int)uVar7 != 0 &&
                 (uVar7 = FUN_0017dc00(&DAT_00281fe0,0x18), (int)uVar7 != 0)))))))) &&
             (uVar7 = FUN_0017dc00(&DAT_00282078,0x10), (int)uVar7 != 0)) {
            uVar7 = 1;
            DAT_00282110 = 1;
            uVar11 = DAT_00282b08 + 1;
            bVar5 = DAT_00282b08 < 0x20;
            DAT_00282b08 = uVar11;
            if ((bVar5) && (DAT_001a7d00 != (code *)0x0)) {
              (*DAT_001a7d00)(DAT_001a7cc8,"script_port_font_ui","chooser_built",6);
              uVar7 = 1;
            }
          }
          goto LAB_0017d17c;
        }
      }
    }
  }
  uVar7 = 0;
LAB_0017d17c:
  if (*(long *)(lVar2 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar7);
  }
  return;
}

/* ===== FUN_0017dc00 @ 0017dc00 ===== */

void FUN_0017dc00(long param_1,undefined2 param_2)

{
  long lVar1;
  bool bVar2;
  bool bVar3;
  int iVar4;
  long lVar5;
  undefined8 uVar6;
  ulong uVar7;
  uint local_54;
  ulong local_50;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  lVar5 = FUN_0016ce48("popover_text_left");
  local_50 = 0;
  *(long *)(param_1 + 8) = lVar5;
  if (lVar5 + 8U >> 3 < 0x201) {
    bVar3 = false;
  }
  else {
    iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,lVar5,&local_50,8);
    bVar3 = iVar4 == 1;
  }
  bVar2 = false;
  if (0xfff < local_50) {
    bVar2 = bVar3;
  }
  uVar7 = local_50;
  if (!(bool)(bVar2 & (local_50 & 7) == 0)) {
    uVar7 = 0;
  }
  if (uVar7 == DAT_001a7cd0 + 0x11ad208U) {
    uVar7 = *(ulong *)(param_1 + 8);
    local_54 = 0;
    local_50 = 1;
    if (((((0xfff < uVar7 + 0x38) && ((uVar7 & 0xfffffffffffffff8) != 0xffffffffffffffc0)) &&
         (iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar7 + 0x38,&local_50,8), iVar4 == 1)) &&
        ((local_50 == 0 && (0xfff < uVar7 + 0x40)))) &&
       (((uVar7 & 0xfffffffffffffffc) != 0xffffffffffffffbc &&
        ((iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar7 + 0x40,&local_54,4), iVar4 == 1 &&
         (local_54 == 0xffffffff)))))) {
      FUN_0016ce84(*(undefined8 *)(param_1 + 8),0);
      lVar5 = FUN_0016ce98(*(undefined8 *)(param_1 + 8),&DAT_0013784a);
      local_50 = 0;
      *(long *)(param_1 + 0x10) = lVar5;
      if (lVar5 + 8U >> 3 < 0x201) {
        bVar3 = false;
      }
      else {
        iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,lVar5,&local_50,8);
        bVar3 = iVar4 == 1;
      }
      bVar2 = false;
      if (0xfff < local_50) {
        bVar2 = bVar3;
      }
      uVar7 = local_50;
      if (!(bool)(bVar2 & (local_50 & 7) == 0)) {
        uVar7 = 0;
      }
      if (uVar7 == DAT_001a7cd0 + 0x11abad8U) {
        uVar6 = FUN_0016e020(*(undefined8 *)(param_1 + 8),*(undefined8 *)(param_1 + 0x10));
        if ((int)uVar6 == 0) goto LAB_0017de64;
        uVar6 = 0;
        local_50 = local_50 & 0xffffffffffff0000;
        local_54 = local_54 & 0xffffff00;
        uVar7 = *(ulong *)(param_1 + 0x10) + 0xb0;
        if ((uVar7 < 0x1000) ||
           ((*(ulong *)(param_1 + 0x10) & 0xfffffffffffffffe) == 0xffffffffffffff4e))
        goto LAB_0017de64;
        iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar7,&local_50,2);
        uVar6 = 0;
        if ((iVar4 != 1) || (((short)local_50 < 1 || (0x100 < (short)local_50)))) goto LAB_0017de64;
        if (0x1000 < *(long *)(param_1 + 0x10) + 0x91U) {
          iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,*(long *)(param_1 + 0x10) + 0x90,&local_54,1);
          uVar6 = 0;
          if ((iVar4 == 1) && ((byte)local_54 < 2)) {
            local_50 = CONCAT62(local_50._2_6_,param_2);
            local_54 = CONCAT31(local_54._1_3_,1);
            *(undefined2 *)(*(long *)(param_1 + 0x10) + 0xb0) = param_2;
            *(undefined1 *)(*(long *)(param_1 + 0x10) + 0x90) = 1;
            FUN_0016cf00(0x3f800000,0x3f800000,*(undefined8 *)(param_1 + 8));
            FUN_0016cf00(0x3f800000,0x3f800000,*(undefined8 *)(param_1 + 0x10));
            FUN_0016ce20(*(undefined8 *)(param_1 + 8),0);
            FUN_0016cf5c(0xc61c3c00,0xc61c3c00,*(undefined8 *)(param_1 + 8));
            FUN_0016cfa4(DAT_00281b10,*(undefined8 *)(param_1 + 8));
            uVar6 = FUN_0017df44(*(undefined8 *)(param_1 + 8));
          }
          goto LAB_0017de64;
        }
      }
    }
  }
  uVar6 = 0;
LAB_0017de64:
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar6);
}

/* ===== FUN_0017e0b0 @ 0017e0b0 ===== */

void FUN_0017e0b0(void)

{
  undefined *puVar1;
  ulong uVar2;
  long lVar3;
  bool bVar4;
  ulong uVar5;
  bool bVar6;
  int iVar7;
  ulong uVar8;
  long lVar9;
  ulong uVar10;
  ulong uVar11;
  undefined8 uVar12;
  long lVar13;
  ulong *puVar14;
  uint uVar15;
  uint local_74;
  ulong local_70;
  long local_68;
  
  lVar3 = tpidr_el0;
  local_68 = *(long *)(lVar3 + 0x28);
  uVar8 = FUN_0017bd68();
  if ((int)uVar8 != 0) {
    uVar8 = FUN_0016cdf0(0x80);
    DAT_00282158 = uVar8;
    if (uVar8 != 0) {
      FUN_0016ce08();
      local_70 = 0;
      if (DAT_00282158 + 8 >> 3 < 0x201) {
        bVar6 = false;
      }
      else {
        iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00282158,&local_70,8);
        bVar6 = iVar7 == 1;
      }
      uVar8 = DAT_00282158;
      bVar4 = false;
      if (0xfff < local_70) {
        bVar4 = bVar6;
      }
      uVar10 = local_70;
      if (!(bool)(bVar4 & (local_70 & 7) == 0)) {
        uVar10 = 0;
      }
      if (uVar10 == DAT_001a7cd0 + 0x11abcf0U) {
        local_74 = 0;
        local_70 = 1;
        if (((((0xfff < DAT_00282158 + 0x38) &&
              ((DAT_00282158 & 0xfffffffffffffff8) != 0xffffffffffffffc0)) &&
             (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00282158 + 0x38,&local_70,8), iVar7 == 1)) &&
            ((local_70 == 0 && (uVar10 = uVar8 + 0x40, 0xfff < uVar10)))) &&
           (((uVar8 & 0xfffffffffffffffc) != 0xffffffffffffffbc &&
            ((iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar10,&local_74,4), iVar7 == 1 &&
             (local_74 == 0xffffffff)))))) {
          DAT_00282150 = DAT_001a7d30;
          FUN_0016ce20(DAT_00282158,1);
          FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_00282158);
          FUN_0016cfb8(DAT_001a7d28,DAT_00282158);
          uVar8 = FUN_0016d5cc(DAT_00282158,DAT_001a7d30);
          if (((int)uVar8 == 0) ||
             (uVar8 = FUN_0017f2f8(&DAT_00282178,6,&DAT_00134f22), (int)uVar8 == 0))
          goto LAB_0017e218;
          DAT_00282160 = FUN_0016ce48("popup_generic");
          local_70 = 0;
          if (DAT_00282160 + 8 >> 3 < 0x201) {
            bVar6 = false;
          }
          else {
            iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00282160,&local_70,8);
            bVar6 = iVar7 == 1;
          }
          uVar8 = DAT_00282160;
          bVar4 = false;
          if (0xfff < local_70) {
            bVar4 = bVar6;
          }
          uVar10 = local_70;
          if (!(bool)(bVar4 & (local_70 & 7) == 0)) {
            uVar10 = 0;
          }
          if (uVar10 == DAT_001a7cd0 + 0x11ad208U) {
            local_74 = 0;
            local_70 = 1;
            if ((((0xfff < DAT_00282160 + 0x38) &&
                 ((DAT_00282160 & 0xfffffffffffffff8) != 0xffffffffffffffc0)) &&
                (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00282160 + 0x38,&local_70,8), iVar7 == 1))
               && (((local_70 == 0 && (uVar10 = uVar8 + 0x40, 0xfff < uVar10)) &&
                   (((uVar8 & 0xfffffffffffffffc) != 0xffffffffffffffbc &&
                    ((iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar10,&local_74,4), iVar7 == 1 &&
                     (local_74 == 0xffffffff)))))))) {
              FUN_0016ce84(DAT_00282160,0);
              lVar9 = FUN_0017c284(DAT_00282160,"button_negative");
              if (lVar9 != 0) {
                *(undefined1 *)(lVar9 + 8) = 0;
              }
              lVar9 = FUN_0017c284(DAT_00282160,"button_no");
              if (lVar9 != 0) {
                *(undefined1 *)(lVar9 + 8) = 0;
              }
              lVar9 = FUN_0017c284(DAT_00282160,"button_yes");
              if (lVar9 != 0) {
                *(undefined1 *)(lVar9 + 8) = 0;
              }
              lVar9 = FUN_0017c284(DAT_00282160,"button_ok");
              if (lVar9 != 0) {
                *(undefined1 *)(lVar9 + 8) = 0;
              }
              lVar9 = FUN_0017c284(DAT_00282160,"button_close");
              if (lVar9 != 0) {
                *(undefined1 *)(lVar9 + 8) = 0;
              }
              uVar10 = FUN_0016ce98(DAT_00282160,&DAT_00134a97);
              uVar8 = DAT_00282160;
              if (uVar10 != 0) {
                uVar15 = 0;
                uVar11 = uVar10;
                do {
                  if ((uVar11 == uVar8) || (0xf < uVar15)) goto LAB_0017e508;
                  local_70 = 0;
                  if (uVar11 + 0x40 >> 3 < 0x201) {
                    bVar6 = false;
                  }
                  else {
                    iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar11 + 0x38,&local_70,8);
                    bVar6 = iVar7 == 1;
                  }
                  uVar5 = local_70;
                  bVar4 = false;
                  if (0xfff < local_70) {
                    bVar4 = bVar6;
                  }
                  bVar4 = (bool)(bVar4 & (local_70 & 7) == 0);
                  uVar2 = local_70;
                  if (!bVar4) {
                    uVar2 = 0;
                  }
                  iVar7 = FUN_0016d5cc(uVar11,uVar2);
                  if (iVar7 == 0) goto LAB_0017e514;
                  uVar15 = uVar15 + 1;
                  uVar11 = uVar5;
                } while (bVar4);
                uVar11 = 0;
LAB_0017e508:
                if (uVar11 == uVar8) {
                  *(undefined1 *)(uVar10 + 8) = 0;
                }
              }
LAB_0017e514:
              uVar10 = FUN_0016ce98(DAT_00282160,"title");
              uVar8 = DAT_00282160;
              if (uVar10 != 0) {
                uVar15 = 0;
                uVar11 = uVar10;
                do {
                  if ((uVar11 == uVar8) || (0xf < uVar15)) goto LAB_0017e5c8;
                  local_70 = 0;
                  if (uVar11 + 0x40 >> 3 < 0x201) {
                    bVar6 = false;
                  }
                  else {
                    iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar11 + 0x38,&local_70,8);
                    bVar6 = iVar7 == 1;
                  }
                  uVar5 = local_70;
                  bVar4 = false;
                  if (0xfff < local_70) {
                    bVar4 = bVar6;
                  }
                  bVar4 = (bool)(bVar4 & (local_70 & 7) == 0);
                  uVar2 = local_70;
                  if (!bVar4) {
                    uVar2 = 0;
                  }
                  iVar7 = FUN_0016d5cc(uVar11,uVar2);
                  if (iVar7 == 0) goto LAB_0017e5d4;
                  uVar15 = uVar15 + 1;
                  uVar11 = uVar5;
                } while (bVar4);
                uVar11 = 0;
LAB_0017e5c8:
                if (uVar11 == uVar8) {
                  *(undefined1 *)(uVar10 + 8) = 0;
                }
              }
LAB_0017e5d4:
              FUN_0016ce20(DAT_00282160,0);
              FUN_0016cfa4(DAT_00282158,DAT_00282160);
              uVar8 = FUN_0017f2f8(&DAT_002822d0,3,&DAT_00136344);
              if (((((int)uVar8 != 0) &&
                   (uVar8 = FUN_0017f2f8(&DAT_00282428,1,&DAT_00132558), (int)uVar8 != 0)) &&
                  (uVar8 = FUN_0017f2f8(&DAT_00282580,1,"RESET"), (int)uVar8 != 0)) &&
                 (uVar8 = FUN_0017f634(), (int)uVar8 != 0)) {
                lVar9 = 0;
                do {
                  if (lVar9 == 0x2b0) {
                    DAT_00282990 = FUN_0016ce48("popover_text_left");
                    local_70 = 0;
                    if (DAT_00282990 + 8U >> 3 < 0x201) {
                      bVar6 = false;
                    }
                    else {
                      iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00282990,&local_70,8);
                      bVar6 = iVar7 == 1;
                    }
                    bVar4 = false;
                    if (0xfff < local_70) {
                      bVar4 = bVar6;
                    }
                    puVar14 = (ulong *)&DAT_00282990;
                    uVar8 = local_70;
                    if (!(bool)(bVar4 & (local_70 & 7) == 0)) {
                      uVar8 = 0;
                    }
                    if (uVar8 != DAT_001a7cd0 + 0x11ad208U) goto LAB_0017e214;
                  }
                  else {
                    nexus_rich_asset(5);
                    uVar12 = FUN_0016ce48();
                    *(undefined8 *)((long)&DAT_002826e0 + lVar9) = uVar12;
                    uVar8 = FUN_0016dc20(uVar12,5,0);
                    if ((int)uVar8 == 0) goto LAB_0017e218;
                    puVar14 = (ulong *)((long)&DAT_002826e0 + lVar9);
                  }
                  uVar8 = *puVar14;
                  local_74 = 0;
                  local_70 = 1;
                  if (((((uVar8 + 0x38 < 0x1000) ||
                        ((uVar8 & 0xfffffffffffffff8) == 0xffffffffffffffc0)) ||
                       ((iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar8 + 0x38,&local_70,8), iVar7 != 1
                        || ((local_70 != 0 || (uVar8 + 0x40 < 0x1000)))))) ||
                      ((uVar8 & 0xfffffffffffffffc) == 0xffffffffffffffbc)) ||
                     ((iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar8 + 0x40,&local_74,4), iVar7 != 1 ||
                      (local_74 != 0xffffffff)))) goto LAB_0017e214;
                  FUN_0016ce84(*puVar14,0);
                  puVar1 = &DAT_0013784a;
                  if (lVar9 != 0x2b0) {
                    puVar1 = &DAT_00134a97;
                  }
                  lVar13 = FUN_0016ce98(*puVar14,puVar1);
                  local_70 = 0;
                  *(long *)((long)&DAT_002826e8 + lVar9) = lVar13;
                  if (lVar13 + 8U >> 3 < 0x201) {
                    bVar6 = false;
                  }
                  else {
                    iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,lVar13,&local_70,8);
                    bVar6 = iVar7 == 1;
                  }
                  bVar4 = false;
                  if (0xfff < local_70) {
                    bVar4 = bVar6;
                  }
                  uVar8 = local_70;
                  if (!(bool)(bVar4 & (local_70 & 7) == 0)) {
                    uVar8 = 0;
                  }
                  if (uVar8 != DAT_001a7cd0 + 0x11abad8U) goto LAB_0017e214;
                  uVar8 = FUN_0016e020(*puVar14,*(undefined8 *)((long)&DAT_002826e8 + lVar9));
                  if ((int)uVar8 == 0) goto LAB_0017e218;
                  if (lVar9 == 0x2b0) {
                    local_70 = local_70 & 0xffffffffffff0000;
                    local_74 = local_74 & 0xffffff00;
                    if (((((DAT_00282998 + 0xb0 < 0x1000) ||
                          ((DAT_00282998 & 0xfffffffffffffffe) == 0xffffffffffffff4e)) ||
                         (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00282998 + 0xb0,&local_70,2),
                         iVar7 != 1)) || (((short)local_70 < 1 || (0x100 < (short)local_70)))) ||
                       ((DAT_00282998 + 0x91 < 0x1001 ||
                        ((iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00282998 + 0x90,&local_74,1),
                         iVar7 != 1 || (1 < (byte)local_74)))))) goto LAB_0017e214;
                    local_70 = CONCAT62(local_70._2_6_,0x10);
                    local_74 = CONCAT31(local_74._1_3_,1);
                    *(undefined2 *)(DAT_00282998 + 0xb0) = 0x10;
                    *(undefined1 *)(DAT_00282998 + 0x90) = 1;
                    FUN_0016cf00(0x3f800000,0x3f800000,*puVar14);
                    FUN_0016cf00(0x3f800000,0x3f800000,DAT_00282998);
                  }
                  FUN_0016ce20(*puVar14,0);
                  FUN_0016cf5c(0xc61c3c00,0xc61c3c00,*puVar14);
                  FUN_0016cfa4(DAT_00282158,*puVar14);
                  uVar8 = FUN_00179dfc(*puVar14);
                  if ((int)uVar8 == 0) goto LAB_0017e218;
                  lVar9 = lVar9 + 0x158;
                } while (lVar9 != 0x408);
                uVar8 = 1;
                DAT_00282ae0 = 1;
              }
              goto LAB_0017e218;
            }
          }
        }
      }
LAB_0017e214:
      uVar8 = 0;
    }
  }
LAB_0017e218:
  if (*(long *)(lVar3 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar8);
}

/* ===== FUN_0017f634 @ 0017f634 ===== */

void FUN_0017f634(void)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  undefined8 uVar4;
  ulong uVar5;
  bool bVar6;
  bool bVar7;
  int iVar8;
  ulong uVar9;
  ulong uVar10;
  long lVar11;
  ulong uVar12;
  float *pfVar13;
  ulong uVar14;
  uint uVar15;
  float fVar16;
  float fVar17;
  ulong local_b0;
  ulong uStack_a8;
  ulong local_a0;
  float fStack_98;
  ulong local_90;
  long local_88;
  
  lVar2 = tpidr_el0;
  local_88 = *(long *)(lVar2 + 0x28);
  uVar9 = FUN_0016ce48("edit_controls_ui");
  local_a0 = 0;
  if (uVar9 + 8 >> 3 < 0x201) {
    bVar6 = false;
  }
  else {
    iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar9,&local_a0,8);
    bVar6 = iVar8 == 1;
  }
  bVar7 = false;
  if (0xfff < local_a0) {
    bVar7 = bVar6;
  }
  uVar10 = local_a0;
  if (!(bool)(bVar7 & (local_a0 & 7) == 0)) {
    uVar10 = 0;
  }
  if (uVar10 == DAT_001a7cd0 + 0x11ad208U) {
    local_b0 = local_b0 & 0xffffffff00000000;
    local_a0 = 1;
    if (((((0xfff < uVar9 + 0x38) && ((uVar9 & 0xfffffffffffffff8) != 0xffffffffffffffc0)) &&
         (iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar9 + 0x38,&local_a0,8), iVar8 == 1)) &&
        ((local_a0 == 0 && (0xfff < uVar9 + 0x40)))) &&
       (((uVar9 & 0xfffffffffffffffc) != 0xffffffffffffffbc &&
        ((iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar9 + 0x40,&local_b0,4), iVar8 == 1 &&
         ((float)local_b0 == -NAN)))))) {
      DAT_00282170 = uVar9;
      FUN_0016ce84(uVar9,0);
      uVar10 = FUN_0017c284(uVar9,"slider_scale");
      uVar9 = uVar10;
      if (uVar10 == 0) goto LAB_0017f78c;
      lVar11 = FUN_0017c284(uVar10,"slider_bg");
      uVar12 = FUN_0017c284(uVar10,"slider_button");
      uVar9 = 0;
      if ((lVar11 == 0) || (uVar12 == 0)) goto LAB_0017f78c;
      FUN_0016cf5c(0,0,lVar11);
      FUN_0016cf5c(0,0,uVar12);
      FUN_0016cf84(uVar12,&local_a0);
      uVar9 = (*(code *)(DAT_001a7cd0 + 0x5d76d4))(uVar12,"bounds");
      if (uVar9 == 0) {
LAB_0017f91c:
        uStack_a8 = (ulong)(uint)fStack_98;
        bVar6 = false;
        local_b0 = local_a0;
      }
      else {
        uVar15 = 0;
        uVar14 = uVar9;
        do {
          if ((uVar14 == uVar12) || (0xf < uVar15)) goto LAB_0017f900;
          local_b0 = 0;
          if (uVar14 + 0x40 >> 3 < 0x201) {
            bVar6 = false;
          }
          else {
            iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar14 + 0x38,&local_b0,8);
            bVar6 = iVar8 == 1;
          }
          uVar5 = local_b0;
          bVar7 = false;
          if (0xfff < local_b0) {
            bVar7 = bVar6;
          }
          bVar7 = (bool)(bVar7 & (local_b0 & 7) == 0);
          uVar1 = local_b0;
          if (!bVar7) {
            uVar1 = 0;
          }
          iVar8 = FUN_0016d5cc(uVar14,uVar1);
          if (iVar8 == 0) goto LAB_0017f91c;
          uVar15 = uVar15 + 1;
          uVar14 = uVar5;
        } while (bVar7);
        uVar14 = 0;
LAB_0017f900:
        if (uVar14 != uVar12) goto LAB_0017f91c;
        FUN_0016cf84(uVar9,&local_b0);
        bVar6 = true;
      }
      uVar14 = FUN_0016cdf0(0x178);
      uVar9 = uVar14;
      if (uVar14 == 0) goto LAB_0017f78c;
      (*(code *)(DAT_001a7cd0 + 0x889288))(uVar14,lVar11,uVar12,0,1);
      local_90 = 0;
      if (uVar14 + 8 >> 3 < 0x201) {
        bVar7 = false;
      }
      else {
        iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar14,&local_90,8);
        bVar7 = iVar8 == 1;
      }
      bVar3 = false;
      if (0xfff < local_90) {
        bVar3 = bVar7;
      }
      uVar9 = local_90;
      if (!(bool)(bVar3 & (local_90 & 7) == 0)) {
        uVar9 = 0;
      }
      if (uVar9 == DAT_001a7cd0 + 0x11c0c58U) {
        uVar15 = 0;
        uVar9 = uVar14;
        do {
          if ((uVar9 == uVar10) || (0xf < uVar15)) goto LAB_0017fa5c;
          local_90 = 0;
          if (uVar9 + 0x40 >> 3 < 0x201) {
            bVar7 = false;
          }
          else {
            iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar9 + 0x38,&local_90,8);
            bVar7 = iVar8 == 1;
          }
          uVar5 = local_90;
          bVar3 = false;
          if (0xfff < local_90) {
            bVar3 = bVar7;
          }
          bVar3 = (bool)(bVar3 & (local_90 & 7) == 0);
          uVar1 = local_90;
          if (!bVar3) {
            uVar1 = 0;
          }
          uVar9 = FUN_0016d5cc(uVar9,uVar1);
          if ((int)uVar9 == 0) goto LAB_0017f78c;
          uVar15 = uVar15 + 1;
          uVar9 = uVar5;
        } while (bVar3);
        uVar9 = 0;
LAB_0017fa5c:
        if (uVar9 == uVar10) {
          local_90 = 0;
          if (uVar14 + 0x130 >> 3 < 0x201) {
            bVar7 = false;
          }
          else {
            iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar14 + 0x128,&local_90,8);
            bVar7 = iVar8 == 1;
          }
          bVar3 = false;
          if (0xfff < local_90) {
            bVar3 = bVar7;
          }
          uVar9 = local_90;
          if (!(bool)(bVar3 & (local_90 & 7) == 0)) {
            uVar9 = 0;
          }
          if (uVar9 == uVar12) {
            FUN_0016cfa4(DAT_00282158,uVar14);
            uVar4 = DAT_0010f810;
            DAT_00282168 = uVar14;
            *(undefined4 *)(uVar14 + 0xe8) = DAT_00282afc;
            *(undefined8 *)(uVar14 + 0xec) = uVar4;
            if (!bVar6) {
LAB_0017fba4:
              FUN_0016cf5c(0xc61c3c00,0xc61c3c00,uVar14);
              uVar9 = FUN_0016e7c0();
              goto LAB_0017f78c;
            }
            pfVar13 = (float *)FUN_0016f79c(uVar14 + 0x138);
            local_90 = local_90 & 0xffffffff00000000;
            if (0x400 < (ulong)(pfVar13 + 1) >> 2) {
              fVar17 = (float)local_a0;
              fVar16 = (float)local_b0;
              iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,pfVar13,&local_90,4);
              if (iVar8 == 1) {
                if ((ABS((float)local_90) != INFINITY) && (!NAN(ABS((float)local_90)))) {
                  fVar16 = ((fStack_98 - fVar17) - ((float)uStack_a8 - fVar16)) * 0.5;
                  fVar17 = ABS(fVar16);
                  if ((fVar17 != INFINITY) && (fVar17 == 512.0 || fVar17 < 512.0 != NAN(fVar17))) {
                    fVar16 = (float)local_90 - fVar16;
                    local_90 = CONCAT44(local_90._4_4_,fVar16);
                    *pfVar13 = fVar16;
                    goto LAB_0017fba4;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  uVar9 = 0;
LAB_0017f78c:
  if (*(long *)(lVar2 + 0x28) != local_88) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar9);
  }
  return;
}

/* ===== FUN_00184e28 @ 00184e28 ===== */

void FUN_00184e28(undefined8 *param_1,uint *param_2)

{
  uint uVar1;
  char cVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  uint uVar11;
  int iVar12;
  uint uVar13;
  long lVar14;
  ulong uVar15;
  int local_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  long local_38;
  
  lVar3 = tpidr_el0;
  local_38 = *(long *)(lVar3 + 0x28);
  *(undefined8 *)((long)param_1 + 0xc) = 0;
  *(undefined8 *)((long)param_1 + 4) = 0;
  *(undefined4 *)param_1 = 0x18;
  *(undefined4 *)(param_1 + 2) = 1;
  *(undefined4 *)((long)param_1 + 0x14) = 0;
  uVar11 = FUN_0018d15c();
  if ((int)uVar11 < 0) {
    uVar11 = 0;
    cVar2 = *(char *)((long)param_2 + 0x47);
    lVar14 = DAT_002848c0;
    lVar4 = DAT_00284900;
    lVar5 = DAT_00284940;
    lVar6 = DAT_00284980;
    lVar7 = DAT_002849c0;
    lVar8 = DAT_00284a00;
    lVar9 = DAT_00284a40;
    lVar10 = DAT_00284a80;
  }
  else {
    uVar1 = param_2[0x10];
    *(undefined1 *)((long)param_1 + 0x15) = 1;
    uVar11 = *(uint *)((long)&DAT_002846b0 + (ulong)uVar11 * 4);
    *(uint *)((long)param_1 + 4) = uVar11;
    *(uint *)((long)param_1 + 0xc) = uVar11;
    if (-1 < (int)uVar1) {
      uVar11 = uVar11 >> (ulong)(uVar1 & 0x1f) & 1;
      *(uint *)((long)param_1 + 4) = uVar11;
    }
    cVar2 = *(char *)((long)param_2 + 0x47);
    lVar14 = DAT_002848c0;
    lVar4 = DAT_00284900;
    lVar5 = DAT_00284940;
    lVar6 = DAT_00284980;
    lVar7 = DAT_002849c0;
    lVar8 = DAT_00284a00;
    lVar9 = DAT_00284a40;
    lVar10 = DAT_00284a80;
  }
  DAT_002848c0 = lVar14;
  DAT_00284900 = lVar4;
  DAT_00284940 = lVar5;
  DAT_00284980 = lVar6;
  DAT_002849c0 = lVar7;
  DAT_00284a00 = lVar8;
  DAT_00284a40 = lVar9;
  DAT_00284a80 = lVar10;
  if (cVar2 != '\0') {
    *(undefined4 *)(param_1 + 2) = 3;
    goto LAB_00184fa8;
  }
  uVar1 = *param_2;
  uVar13 = 0x11;
  switch(uVar1) {
  case 7:
    break;
  case 8:
    uVar13 = 0x16;
    break;
  case 9:
  case 10:
  case 0xd:
switchD_00184ee8_caseD_9:
    uVar13 = uVar1;
    break;
  case 0xb:
    uVar13 = 0x17;
    break;
  case 0xc:
    uVar13 = 0x1e;
    break;
  case 0xe:
    uVar13 = 0x19;
    break;
  default:
    if (uVar1 == 0x43) {
      uVar13 = 0x44;
    }
    else {
      if (uVar1 != 0x83) goto switchD_00184ee8_caseD_9;
      uVar13 = 0x84;
    }
  }
  if ((byte)param_2[0x11] == 1) {
    if (uVar13 != uVar1) {
      FUN_00184e28(param_1,PTR_DAT_001a36f0 + (ulong)uVar13 * 0x48);
      *(undefined1 *)((long)param_1 + 0x14) = 1;
      goto LAB_00184fa8;
    }
  }
  else if ((byte)param_2[0x11] < 5) {
    switch(uVar1) {
    case 0x2f:
    case 0x30:
    case 0x31:
    case 0x32:
    case 0x45:
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x6c:
    case 0x82:
      goto switchD_00184f94_caseD_2f;
    default:
      uVar11 = uVar1 >> 6;
      uVar15 = 1L << ((ulong)uVar1 & 0x3f);
      if (((&DAT_00284898)[uVar11] & uVar15) == 0) {
        if (((&DAT_002848d8)[uVar11] & uVar15) == 0) {
          if (((&DAT_00284918)[uVar11] & uVar15) == 0) {
            if (((&DAT_00284958)[uVar11] & uVar15) == 0) {
              if (((&DAT_00284998)[uVar11] & uVar15) == 0) {
                if (((&DAT_002849d8)[uVar11] & uVar15) == 0) {
                  if (((&DAT_00284a18)[uVar11] & uVar15) == 0) {
                    if (((&DAT_00284a58)[uVar11] & uVar15) == 0) break;
                    lVar14 = 7;
                  }
                  else {
                    lVar14 = 6;
                  }
                }
                else {
                  lVar14 = 5;
                }
              }
              else {
                lVar14 = 4;
              }
            }
            else {
              lVar14 = 3;
            }
          }
          else {
            lVar14 = 2;
          }
        }
        else {
          lVar14 = 1;
        }
      }
      else {
        lVar14 = 0;
      }
      if ((code *)(&DAT_002848b0)[lVar14 * 8] != (code *)0x0) {
        uStack_44 = 0;
        uStack_40 = 0;
        uStack_4c = 0;
        uStack_48 = 0;
        uStack_3c = 0;
        local_50 = 0x18;
        iVar12 = (*(code *)(&DAT_002848b0)[lVar14 * 8])
                           ((&DAT_00284890)[lVar14 * 8],(ulong)uVar1,&local_50);
        if ((iVar12 == 1) && (local_50 == 0x18)) {
          param_1[1] = CONCAT44(uStack_44,uStack_48);
          *param_1 = CONCAT44(uStack_4c,0x18);
          param_1[2] = CONCAT44(uStack_3c,uStack_40);
        }
      }
      break;
    case 0x40:
    case 0x4a:
    case 0x5c:
    case 0x70:
    case 0x91:
      *(bool *)((long)param_1 + 0x14) =
           (((lVar14 != 0 || lVar4 != 0) || (lVar5 != 0 || lVar6 != 0)) ||
           ((lVar7 != 0 || lVar8 != 0) || lVar9 != 0)) || lVar10 != 0;
      *(uint *)(param_1 + 2) =
           (uint)((((lVar14 == 0 && lVar4 == 0) && (lVar5 == 0 && lVar6 == 0)) &&
                  ((lVar7 == 0 && lVar8 == 0) && lVar9 == 0)) && lVar10 == 0);
      if (((((lVar14 != 0 || lVar4 != 0) || (lVar5 != 0 || lVar6 != 0)) ||
           ((lVar7 != 0 || lVar8 != 0) || lVar9 != 0)) || lVar10 != 0) &&
         ((uVar1 == 0x70 || (uVar1 == 0x4a)))) {
        *(uint *)(param_1 + 1) = uVar11;
      }
    }
    goto LAB_00184fa8;
  }
switchD_00184f94_caseD_2f:
  *(undefined4 *)(param_1 + 2) = 0;
  *(uint *)(param_1 + 1) = uVar11;
  *(undefined1 *)((long)param_1 + 0x14) = 1;
LAB_00184fa8:
  if (*(long *)(lVar3 + 0x28) != local_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== nexus_menu_dispatch @ 00185158 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nexus_menu_dispatch(uint param_1,long param_2,ulong param_3,int param_4)

{
  undefined4 *puVar1;
  long lVar2;
  undefined8 *puVar3;
  long lVar4;
  long lVar5;
  byte bVar6;
  long lVar7;
  undefined *puVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  ulong uVar12;
  long lVar13;
  uint uVar14;
  undefined4 *puVar15;
  undefined *puVar16;
  ulong uVar17;
  uint uVar18;
  uint uVar19;
  undefined4 uVar20;
  undefined8 local_610;
  undefined1 auStack_608 [16];
  undefined4 local_5f8;
  char local_5f4;
  undefined8 local_430;
  uint local_428;
  uint local_424;
  undefined8 local_420;
  undefined8 local_418;
  undefined8 uStack_410;
  long local_408;
  ulong local_400;
  undefined4 local_3f8;
  undefined4 local_260;
  undefined4 local_25c;
  long local_68;
  
  lVar7 = tpidr_el0;
  local_68 = *(long *)(lVar7 + 0x28);
  uVar12 = 4;
  if (((0xa7 < param_1) || (0x100 < param_3)) || ((param_2 == 0 && (param_3 != 0))))
  goto LAB_00185530;
  uVar12 = FUN_00193f80(1,&DAT_00284688);
  if ((uVar12 & 1) != 0) {
    uVar12 = 3;
    goto LAB_00185530;
  }
  uVar12 = 4;
  if ((param_4 < 1) || (DAT_00284694 != param_4)) goto LAB_0018552c;
  DAT_002846a4 = param_1;
  iVar9 = FUN_0018d0fc(param_1);
  if ((iVar9 == 0) &&
     ((iVar9 = pthread_once((pthread_once_t *)&DAT_0028dcfc,FUN_0018d500),
      DAT_0028dd00 == (code *)0x0 || (iVar9 = (*DAT_0028dd00)(iVar9), iVar9 != 1)))) {
    uVar12 = 1;
    DAT_0028469f = 0;
    _DAT_0028469c = 0x5901;
    goto LAB_00185514;
  }
  FUN_00185d10();
  puVar8 = PTR_DAT_001a36f0;
  puVar16 = PTR_DAT_001a36f0 + (ulong)param_1 * 0x48;
  if (puVar16[0x47] != '\0') goto LAB_00185218;
  if (param_1 == 0x82) {
    uVar12 = 1;
    _DAT_0028469c = _DAT_0028469c & 0xff00;
    goto LAB_00185514;
  }
  uVar17 = (ulong)param_1;
  bVar6 = PTR_DAT_001a36f0[uVar17 * 0x48 + 0x44];
  if (bVar6 == 1) {
    uVar12 = 1;
    DAT_0028469f = 0;
    _DAT_0028469c = CONCAT11(PTR_DAT_001a36f0[uVar17 * 0x48 + 0x45],1);
    goto LAB_00185514;
  }
  switch(param_1) {
  case 0x30:
    if (DAT_00284a98 == (code *)0x0) {
LAB_00185218:
      uVar12 = 0;
    }
    else {
      FUN_001842d0(&local_430);
      iVar9 = (*DAT_00284a98)(DAT_00284a90,&local_430,0x3c4);
      uVar10 = 1;
      if (iVar9 == 0) {
        uVar10 = 0xffffffff;
      }
      uVar12 = (ulong)uVar10;
    }
    break;
  case 0x31:
    local_610 = 0;
    if (DAT_00284aa0 == (code *)0x0) goto LAB_00185218;
    iVar9 = (*DAT_00284aa0)(DAT_00284a90,&local_430,0x3c4,&local_610);
    if (iVar9 == 0) {
      uVar12 = 0xffffffff;
    }
    else {
      uVar12 = FUN_0018447c(&local_430,local_610,auStack_608);
      if ((int)uVar12 == 1) {
        uVar12 = FUN_001846b8(auStack_608);
      }
    }
    break;
  case 0x32:
    lVar13 = 0;
    puVar15 = (undefined4 *)(PTR_PTR_s_nexus_sx_spin_001a36f8 + 0x28);
    do {
      puVar1 = puVar15 + -6;
      uVar20 = *puVar15;
      puVar15 = puVar15 + 0xc;
      *(ulong *)((long)&local_430 + lVar13) = CONCAT44(uVar20,*puVar1);
      lVar13 = lVar13 + 8;
    } while (lVar13 != 0x1d0);
    local_260 = *(undefined4 *)(PTR_PTR_s_nexus_sx_spin_001a36f8 + 0xaf0);
    local_25c = *(undefined4 *)(PTR_PTR_s_nexus_sx_spin_001a36f8 + 0xb08);
    uVar12 = FUN_001846b8(&local_430);
    goto LAB_001852f4;
  case 0x33:
  case 0x34:
  case 0x35:
  case 0x36:
  case 0x37:
  case 0x38:
  case 0x39:
  case 0x3a:
  case 0x3b:
  case 0x3c:
  case 0x3d:
  case 0x3e:
  case 0x3f:
  case 0x41:
  case 0x42:
  case 0x43:
  case 0x44:
  case 0x45:
  case 0x46:
  case 0x47:
  case 0x48:
  case 0x49:
  case 0x4b:
  case 0x4c:
  case 0x4d:
  case 0x4e:
  case 0x4f:
  case 0x50:
  case 0x51:
  case 0x52:
  case 0x53:
  case 0x54:
  case 0x55:
  case 0x56:
  case 0x57:
  case 0x58:
  case 0x59:
  case 0x5a:
  case 0x5b:
  case 0x5d:
  case 0x5e:
  case 0x5f:
  case 0x60:
  case 0x61:
  case 0x62:
  case 99:
  case 100:
  case 0x65:
  case 0x66:
  case 0x67:
  case 0x68:
  case 0x69:
  case 0x6a:
  case 0x6b:
  case 0x6c:
  case 0x6d:
  case 0x6e:
  case 0x6f:
    goto switchD_001852e8_caseD_33;
  case 0x40:
  case 0x4a:
  case 0x5c:
  case 0x70:
switchD_001852e8_caseD_40:
    uVar12 = FUN_0018d64c(param_1);
LAB_001852f4:
    if ((int)uVar12 - 1U < 2) {
      uVar12 = FUN_0018d334();
    }
    break;
  default:
    if (param_1 == 0x91) goto switchD_001852e8_caseD_40;
switchD_001852e8_caseD_33:
    uVar10 = FUN_0018d15c(puVar16);
    if ((int)uVar10 < 0) {
      uVar11 = 0;
    }
    else {
      uVar11 = *(uint *)((long)&DAT_002846b0 + (ulong)uVar10 * 4);
    }
    uVar18 = (uint)bVar6;
    if ((bVar6 == 5) || (uVar19 = (uint)bVar6, uVar19 == 2)) {
      uVar14 = (uint)(uVar11 == 0);
      if (-1 < (int)*(uint *)(puVar8 + uVar17 * 0x48 + 0x40)) {
        uVar14 = 1 << (ulong)(*(uint *)(puVar8 + uVar17 * 0x48 + 0x40) & 0x1f) ^ uVar11;
      }
    }
    else {
      uVar14 = uVar11;
      if (((-1 < (int)uVar10) && (uVar19 < 8)) && ((1 << (ulong)(uVar19 & 0x1f) & 0xd8U) != 0)) {
        lVar13 = (long)*(int *)(puVar8 + uVar17 * 0x48 + 0x3c) + (long)(int)uVar11;
        lVar4 = (long)*(int *)(PTR_PTR_s_nexus_sx_spin_001a36f8 + (ulong)uVar10 * 0x18 + 8);
        lVar5 = (long)*(int *)(PTR_PTR_s_nexus_sx_spin_001a36f8 + (ulong)uVar10 * 0x18 + 0x10);
        if ((uVar18 == 7) || (uVar18 == 4)) {
          lVar2 = lVar4;
          if (lVar13 <= lVar5) {
            lVar2 = lVar13;
          }
          if (lVar4 <= lVar2) {
            lVar5 = lVar2;
          }
          uVar14 = (uint)lVar5;
        }
        else {
          if (lVar5 <= lVar13) {
            lVar13 = lVar5;
          }
          if (lVar13 <= lVar4) {
            lVar13 = lVar4;
          }
          uVar14 = (uint)lVar13;
        }
      }
    }
    if ((uVar18 < 5) &&
       ((0x3d < param_1 - 0x2f ||
        ((1L << ((ulong)(param_1 - 0x2f) & 0x3f) & 0x2000000003c00001U) == 0)))) {
      lVar13 = FUN_0018d3b4(param_1);
      FUN_00184e28(auStack_608,puVar16);
      DAT_002846ac = local_5f8;
      if ((lVar13 == 0) || (local_5f4 == '\0')) {
        uVar12 = 0;
        break;
      }
      uStack_410 = *(undefined8 *)(puVar8 + uVar17 * 0x48 + 0x30);
      local_418 = *(undefined8 *)(puVar8 + uVar17 * 0x48 + 0x28);
      puVar3 = (undefined8 *)(PTR_PTR_s_nexus_sx_spin_001a36f8 + (ulong)uVar10 * 0x18);
      if ((int)uVar10 < 0) {
        puVar3 = (undefined8 *)(puVar8 + uVar17 * 0x48 + 0x20);
      }
      local_420 = *puVar3;
      local_430._0_4_ = 0x40;
      local_430._4_4_ = param_1;
      local_428 = uVar18;
      local_424 = uVar14;
      local_408 = param_2;
      local_400 = param_3;
      local_3f8 = FUN_0018d48c();
      uVar11 = (**(code **)(lVar13 + 0x30))(*(undefined8 *)(lVar13 + 8),&local_430);
    }
    else {
      uVar11 = 1;
    }
    if ((-1 < (int)uVar10) && (uVar11 - 1 < 2)) {
      *(uint *)((long)&DAT_002846b0 + (ulong)uVar10 * 4) = uVar14;
      uVar12 = FUN_0018d334();
      if ((int)uVar12 != 1) break;
    }
    uVar12 = (ulong)uVar11;
  }
LAB_00185514:
  DAT_002846a8 = (undefined4)uVar12;
  DAT_002846a0 = DAT_002846a0 + 1;
LAB_0018552c:
  DAT_00284688 = 0;
LAB_00185530:
  if (*(long *)(lVar7 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar12);
}

/* ===== FUN_0018d0fc @ 0018d0fc ===== */

undefined4 FUN_0018d0fc(uint param_1)

{
  undefined4 uVar1;
  
  uVar1 = 0;
  switch(param_1) {
  case 0x20:
  case 0x2d:
  case 0x2f:
  case 0x6d:
  case 0x6e:
  case 0x6f:
  case 0x70:
    goto switchD_0018d120_caseD_20;
  case 0x21:
  case 0x25:
  case 0x27:
  case 0x28:
  case 0x29:
  case 0x2a:
  case 0x2b:
  case 0x2c:
  case 0x2e:
  case 0x30:
  case 0x31:
  case 0x32:
  case 0x33:
  case 0x34:
  case 0x35:
  case 0x36:
  case 0x37:
  case 0x38:
  case 0x39:
  case 0x3a:
  case 0x3b:
  case 0x3c:
  case 0x3d:
  case 0x3e:
  case 0x3f:
  case 0x40:
  case 0x41:
  case 0x42:
  case 0x43:
  case 0x44:
  case 0x45:
  case 0x46:
  case 0x47:
  case 0x48:
  case 0x49:
  case 0x4a:
  case 0x4b:
  case 0x4c:
  case 0x4d:
  case 0x4e:
  case 0x4f:
  case 0x50:
  case 0x51:
  case 0x52:
  case 0x53:
  case 0x54:
  case 0x55:
  case 0x56:
  case 0x57:
  case 0x58:
  case 0x59:
  case 0x5a:
  case 0x5b:
  case 0x5c:
  case 0x5d:
  case 0x5e:
  case 0x5f:
  case 0x60:
  case 0x61:
  case 0x62:
  case 99:
  case 100:
  case 0x65:
  case 0x66:
  case 0x67:
  case 0x68:
  case 0x69:
  case 0x6a:
  case 0x6b:
  case 0x6c:
  case 0x71:
  case 0x72:
  case 0x73:
  case 0x74:
  case 0x75:
  case 0x76:
  case 0x77:
  case 0x78:
  case 0x7a:
  case 0x7b:
  case 0x7c:
  case 0x7d:
  case 0x7e:
  case 0x7f:
  case 0x80:
  case 0x81:
  case 0x82:
  case 0x83:
  case 0x84:
  case 0x85:
  case 0x86:
  case 0x87:
  case 0x88:
  case 0x89:
  case 0x8a:
  case 0x8b:
  case 0x8c:
  case 0x8d:
  case 0x8e:
  case 0x8f:
  case 0x90:
  case 0x91:
  case 0x92:
  case 0x93:
  case 0x94:
  case 0x95:
  case 0x96:
  case 0x97:
  case 0x98:
  case 0x99:
switchD_0018d120_caseD_21:
    if (0x9c < param_1) {
      return 0;
    }
    uVar1 = *(undefined4 *)(&DAT_00140830 + (long)(int)param_1 * 4);
  case 0x22:
  case 0x23:
  case 0x24:
  case 0x26:
  case 0x79:
  case 0x9a:
    return uVar1;
  default:
    if (param_1 != 0x20002) goto switchD_0018d120_caseD_21;
switchD_0018d120_caseD_20:
    return 1;
  }
}

/* ===== FUN_0018f988 @ 0018f988 ===== */

undefined8 FUN_0018f988(uint param_1,char *param_2)

{
  bool bVar1;
  uint uVar2;
  int iVar3;
  size_t sVar4;
  undefined8 uVar5;
  long lVar6;
  ulong uVar7;
  ulong uVar8;
  
  sVar4 = strlen(param_2);
  uVar2 = DAT_0028fb58;
  if (sVar4 < 0x80) {
    uVar7 = (ulong)DAT_0028fb58;
    if (DAT_0028fb58 == 0) {
      uVar7 = 0;
    }
    else {
      lVar6 = 0x28fed8;
      uVar8 = uVar7;
      do {
        iVar3 = strcmp((char *)(lVar6 + -0x80),param_2);
        if (iVar3 == 0) goto LAB_0018faac;
        lVar6 = lVar6 + 0x90;
        uVar8 = uVar8 - 1;
      } while (uVar8 != 0);
      if (uVar2 == 0x200) goto LAB_0018f9b4;
    }
    DAT_0028fb58 = uVar2 + 1;
    sVar4 = strlen(param_2);
    memcpy(&DAT_0028fe58 + uVar7 * 0x90,param_2,sVar4 + 1);
    lVar6 = uVar7 * 0x90 + 0x28fed8;
    (*DAT_0028fb18)(DAT_0028fac8,lVar6,&DAT_0028fe58 + uVar7 * 0x90);
LAB_0018faac:
    if (*(long *)(&DAT_0028fcc0 + (ulong)param_1 * 8) == lVar6) {
      uVar5 = 1;
    }
    else {
      (*DAT_0028fb20)(DAT_0028fac8,(&DAT_0028fb80)[param_1],lVar6);
      uVar5 = 1;
      *(long *)(&DAT_0028fcc0 + (ulong)param_1 * 8) = lVar6;
    }
  }
  else {
LAB_0018f9b4:
    uVar5 = 0xffffffff;
    DAT_0028fb48 = 0xffffffff;
    FUN_00183cdc(0xffffffff,DAT_0028fb4c,"persistent_label_budget");
    uVar2 = DAT_0028fb54 + 1;
    bVar1 = DAT_0028fb54 < 0x80;
    DAT_0028fb54 = uVar2;
    if ((bVar1) && (DAT_0028fb38 != (code *)0x0)) {
      (*DAT_0028fb38)(DAT_0028fac8,"nexus_menu_stopped","persistent_label_budget",0);
    }
  }
  return uVar5;
}

/* ===== FUN_0018fe44 @ 0018fe44 ===== */

void FUN_0018fe44(long *param_1,undefined8 *param_2,uint param_3,long param_4,int param_5,
                 char *param_6,ulong param_7)

{
  long lVar1;
  bool bVar2;
  bool bVar3;
  int iVar4;
  long lVar5;
  ulong uVar6;
  undefined8 uVar7;
  char *pcVar8;
  char *pcVar9;
  undefined4 uVar10;
  uint uVar11;
  code *pcVar12;
  ulong uVar13;
  ulong uVar14;
  undefined4 uVar15;
  ulong local_70;
  long local_68;
  
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  if (((((int)param_1[8] == 3) || (param_6 == (char *)0x0)) || (1 < param_3)) ||
     ((((int)param_1[10] != 0 || (iVar4 = strcmp(param_6,"Mainloop"), param_5 < 1)) ||
      ((iVar4 != 0 ||
       ((*(int *)((long)param_1 + 0x44) != 0 && (*(int *)((long)param_1 + 0x44) != param_5))))))))
  goto LAB_0018fe7c;
  *(undefined4 *)(param_1 + 10) = 1;
  if (param_3 == 0) {
    param_1[5] = param_4;
    if ((*(uint *)(param_1 + 0xb) & 1) == 0) {
      *(uint *)(param_1 + 0xb) = *(uint *)(param_1 + 0xb) | 1;
      (*(code *)param_2[0xc])(*param_2,"scheduling_seen","Mainloop_TextField_setText",param_1);
    }
  }
  local_70 = 0;
  if (*param_1 + 0x12eb9f8U >> 3 < 0x201) {
LAB_0018ffc4:
    uVar13 = 0;
    bVar3 = false;
    uVar14 = 0;
  }
  else {
    iVar4 = (*(code *)param_2[1])(*param_2,*param_1 + 0x12eb9f0,&local_70,8);
    uVar13 = local_70;
    bVar3 = (local_70 & 7) != 0;
    uVar14 = local_70;
    if ((iVar4 == 0 || local_70 < 0x1000) || bVar3) {
      uVar14 = 0;
    }
    if ((iVar4 == 0 || local_70 < 0x1000) || bVar3) goto LAB_0018ffc4;
    local_70 = 0;
    if (uVar14 + 0x98 >> 3 < 0x201) {
      bVar3 = false;
    }
    else {
      iVar4 = (*(code *)param_2[1])(*param_2,uVar14 + 0x90,&local_70,8);
      bVar3 = iVar4 != 0;
    }
    bVar2 = false;
    if (0xfff < local_70) {
      bVar2 = bVar3;
    }
    uVar14 = local_70;
    if (!(bool)(bVar2 & (local_70 & 7) == 0)) {
      uVar14 = 0;
    }
    bVar3 = true;
  }
  if ((int)param_1[8] == 2) {
    if ((((param_1[1] != uVar13) || (param_1[2] != uVar14)) ||
        (iVar4 = FUN_001905a4(param_2,param_1[3],uVar14), iVar4 == 0)) ||
       (iVar4 = FUN_001905a4(param_2,param_1[4],uVar14), iVar4 == 0)) {
      uVar10 = 3;
      pcVar8 = "stopped";
      pcVar9 = "stage_generation_or_attachment_changed";
      goto LAB_001903d0;
    }
    if ((param_3 == 1) && (param_1[3] == param_4)) {
      (*(code *)param_2[0xc])(*param_2,"click","own_launcher",param_1);
      bVar3 = (int)param_1[9] == 0;
      *(uint *)(param_1 + 9) = (uint)bVar3;
      uVar15 = 0x43200000;
      uVar10 = 0x433e0000;
      if (!bVar3) {
        uVar15 = 0xc61c3c00;
        uVar10 = 0xc61c3c00;
      }
      (*(code *)param_2[10])(uVar10,uVar15,*param_2,param_1[4]);
      pcVar8 = "close";
      if ((int)param_1[9] != 0) {
        pcVar8 = "open";
      }
      (*(code *)param_2[0xc])(*param_2,pcVar8,"empty_panel",param_1);
    }
  }
  else if (param_3 == 0) {
    bVar2 = false;
    if (uVar14 != 0) {
      bVar2 = bVar3;
    }
    if (bVar2) {
      lVar5 = FUN_00190510(param_2,param_4);
      if (lVar5 == *param_1 + 0x11abad8) {
        iVar4 = FUN_001907b0(param_2,param_4,uVar14);
        if (iVar4 == 0) {
          if ((*(uint *)(param_1 + 0xb) >> 4 & 1) == 0) {
            uVar11 = *(uint *)(param_1 + 0xb) | 0x10;
            pcVar8 = "waiting_rooted_text";
            pcVar9 = "TextField_not_attached_to_current_root";
            goto LAB_00190448;
          }
        }
        else {
          uVar6 = param_1[6];
          if (param_1[6] == 0) {
            param_1[6] = param_7;
            uVar6 = param_7;
          }
          if ((ulong)param_1[7] <= param_7) {
            if ((0x1f < *(int *)((long)param_1 + 0x4c)) || (0x752 < param_7 - uVar6 >> 5)) {
              uVar10 = 3;
              pcVar8 = "stopped";
              pcVar9 = "asset_readiness_budget";
LAB_001903d0:
              *(undefined4 *)(param_1 + 8) = uVar10;
              goto LAB_001903d4;
            }
            param_1[7] = param_7 + 500;
            *(int *)((long)param_1 + 0x4c) = *(int *)((long)param_1 + 0x4c) + 1;
            iVar4 = (*(code *)param_2[2])(*param_2);
            if (iVar4 != 0) {
              param_1[1] = uVar13;
              param_1[2] = uVar14;
              *(undefined4 *)(param_1 + 8) = 1;
              *(int *)((long)param_1 + 0x44) = param_5;
              (*(code *)param_2[0xc])(*param_2,"engine_thread","rooted_TextField_setText",param_1);
              lVar5 = (*(code *)param_2[3])(*param_2,0x260);
              param_1[3] = lVar5;
              lVar5 = (*(code *)param_2[3])(*param_2,0x260);
              param_1[4] = lVar5;
              if ((param_1[3] == 0) || (lVar5 == 0)) {
                uVar10 = 3;
                pcVar8 = "stopped";
                pcVar9 = "allocation_failed";
              }
              else {
                (*(code *)param_2[4])(*param_2);
                (*(code *)param_2[4])(*param_2,param_1[4]);
                iVar4 = FUN_001908c0(param_1,param_2,param_1[3]);
                if ((iVar4 == 0) || (iVar4 = FUN_001908c0(param_1,param_2,param_1[4]), iVar4 == 0))
                {
                  uVar10 = 3;
                  pcVar8 = "stopped";
                  pcVar9 = "initial_button_state";
                }
                else {
                  iVar4 = FUN_00190ad0(param_1,param_2,param_1[3],"popover_button_blue",
                                       param_1 + 0xc,"NEXUS");
                  if ((iVar4 == 0) ||
                     (iVar4 = FUN_00190ad0(param_1,param_2,param_1[4],"popover_button_spectate",
                                           param_1 + 0xe,&DAT_00134f22), iVar4 == 0))
                  goto LAB_001903e4;
                  iVar4 = FUN_001908c0(param_1,param_2,param_1[3]);
                  if ((iVar4 == 0) || (iVar4 = FUN_001908c0(param_1,param_2,param_1[4]), iVar4 == 0)
                     ) {
                    uVar10 = 3;
                    pcVar8 = "stopped";
                    pcVar9 = "configured_button_state";
                  }
                  else {
                    (*(code *)param_2[0xc])
                              (*param_2,"constructed","two_0x260_buttons_no_features",param_1);
                    uVar6 = FUN_00190510(param_2,*param_1 + 0x12eb9f0);
                    if ((uVar6 == uVar13) &&
                       (uVar6 = FUN_00190510(param_2,uVar13 + 0x90), uVar6 == uVar14)) {
                      (*(code *)param_2[10])(0xc61c3c00,0xc61c3c00,*param_2,param_1[4]);
                      (*(code *)param_2[10])(0x42b40000,0x42200000,*param_2,param_1[3]);
                      (*(code *)param_2[0xb])(*param_2,uVar13,param_1[4]);
                      (*(code *)param_2[0xb])(*param_2,uVar13,param_1[3]);
                      iVar4 = FUN_001905a4(param_2,param_1[4],uVar14);
                      if ((iVar4 == 0) ||
                         (iVar4 = FUN_001905a4(param_2,param_1[3],uVar14), iVar4 == 0)) {
                        uVar10 = 3;
                        pcVar8 = "stopped";
                        pcVar9 = "attachment_postcondition";
                      }
                      else {
                        uVar10 = 2;
                        pcVar8 = "attached";
                        pcVar9 = "exact_root_parent_and_vector_membership";
                      }
                    }
                    else {
                      uVar10 = 3;
                      pcVar8 = "stopped";
                      pcVar9 = "stage_changed_during_construction";
                    }
                  }
                }
              }
              goto LAB_001903d0;
            }
            iVar4 = *(int *)((long)param_1 + 0x54);
            *(int *)((long)param_1 + 0x54) = iVar4 + 1;
            if (iVar4 != 0) goto LAB_001903e4;
            pcVar12 = (code *)param_2[0xc];
            pcVar8 = "waiting_assets";
            uVar7 = *param_2;
            pcVar9 = "ui_sc_exports";
            goto LAB_001903dc;
          }
        }
      }
      else if ((*(uint *)(param_1 + 0xb) >> 3 & 1) == 0) {
        uVar11 = *(uint *)(param_1 + 0xb) | 8;
        pcVar8 = "waiting_receiver";
        pcVar9 = "base_TextField_vtable_required";
        goto LAB_00190448;
      }
    }
    else {
      if ((*(uint *)(param_1 + 0xb) >> 1 & 1) != 0) goto LAB_001903e4;
      uVar11 = *(uint *)(param_1 + 0xb) | 2;
      pcVar8 = "waiting_stage";
      pcVar9 = "stage_or_root_null";
LAB_00190448:
      *(uint *)(param_1 + 0xb) = uVar11;
LAB_001903d4:
      pcVar12 = (code *)param_2[0xc];
      uVar7 = *param_2;
LAB_001903dc:
      (*pcVar12)(uVar7,pcVar8,pcVar9,param_1);
    }
  }
LAB_001903e4:
  *(undefined4 *)(param_1 + 10) = 0;
LAB_0018fe7c:
  if (*(long *)(lVar1 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_00192340 @ 00192340 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_00192340(char *param_1,undefined8 *param_2)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  uint uVar4;
  undefined4 uVar5;
  undefined8 uVar6;
  undefined4 local_40;
  undefined4 local_3c;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  if (param_2 == (undefined8 *)0x0) {
    uVar6 = 4;
  }
  else {
    iVar3 = strcmp(param_1,"BattleServersPopup");
    if ((((iVar3 == 0) || (iVar3 = strcmp(param_1,"ThemesPopup"), iVar3 == 0)) ||
        (iVar3 = strcmp(param_1,"BSDDebugPopup"), iVar3 == 0)) ||
       ((iVar3 = strcmp(param_1,"ProfilePopup"), iVar3 == 0 ||
        (iVar3 = strcmp(param_1,"MapEditorPopup"), iVar3 == 0)))) {
      uVar6 = 1;
      *(undefined8 *)((long)param_2 + 0xc) = 0;
      *(undefined8 *)((long)param_2 + 4) = 0;
      *(undefined4 *)param_2 = 0x18;
      *(undefined4 *)((long)param_2 + 0x14) = 1;
    }
    else {
      iVar3 = strcmp(param_1,"CameraReset");
      FUN_0019198c();
      if (iVar3 == 0) {
        *(undefined1 *)((long)param_2 + 0x15) = 0;
        uVar6 = _DAT_0010f960;
        bVar2 = DAT_002a1f10 == 0;
        param_2[1] = _UNK_0010f968;
        *param_2 = uVar6;
        *(uint *)(param_2 + 2) = (uint)bVar2;
        *(bool *)((long)param_2 + 0x14) = !bVar2;
      }
      else {
        local_40 = 1;
        local_3c = 0;
        if (DAT_002a1f28 == (code *)0x0) {
          uVar4 = 0;
          uVar5 = 0;
        }
        else {
          iVar3 = (*DAT_002a1f28)(param_1,&local_3c,&local_40);
          uVar4 = (uint)(iVar3 != 0);
          uVar5 = local_3c;
          if (iVar3 == 0) {
            uVar5 = 0;
          }
        }
        *(undefined4 *)(param_2 + 1) = uVar5;
        *(undefined4 *)((long)param_2 + 0xc) = local_3c;
        *(char *)((long)param_2 + 0x14) = (char)uVar4;
        *(undefined1 *)((long)param_2 + 0x15) = 1;
        *(undefined4 *)param_2 = 0x18;
        *(undefined4 *)((long)param_2 + 4) = local_3c;
        *(uint *)(param_2 + 2) = uVar4 ^ 1;
      }
      uVar6 = 1;
      *(undefined2 *)((long)param_2 + 0x16) = 0;
    }
  }
  if (*(long *)(lVar1 + 0x28) != local_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return uVar6;
}

/* ===== FUN_001924cc @ 001924cc ===== */

void FUN_001924cc(int param_1)

{
  int iVar1;
  long lVar2;
  uint uVar3;
  int iVar4;
  ulong uVar5;
  char *__s1;
  undefined8 local_40;
  long local_38;
  
  lVar2 = tpidr_el0;
  local_38 = *(long *)(lVar2 + 0x28);
  if (param_1 - 0x21037U < 0xffffffc9) {
    uVar5 = 4;
  }
  else {
    FUN_0019198c();
    __s1 = (&PTR_s_DisablePinAnimation_001a2fb8)[(ulong)(param_1 - 0x21000) * 3];
    uVar3 = strcmp(__s1,"CameraReset");
    uVar5 = (ulong)uVar3;
    if (uVar3 == 0) {
      FUN_0019198c();
      if (DAT_002a1f10 == (code *)0x0) {
        uVar5 = 0;
      }
      else {
        uVar5 = (*DAT_002a1f10)(1);
        if (((int)uVar5 != 0) &&
           ((DAT_002a1e70 == (code *)0x0 || (uVar5 = (*DAT_002a1e70)(4,0,0), (int)uVar5 != 0)))) {
          uVar5 = 1;
        }
      }
    }
    else {
      if ((0x29811000000000U >> ((ulong)(param_1 - 0x21000) & 0x3f) & 1) != 0) {
        if (*(long *)(lVar2 + 0x28) == local_38) {
          FUN_0014f5b8(__s1);
          return;
        }
        goto LAB_00192658;
      }
      uVar5 = 0;
      local_40 = 0;
      if ((DAT_002a1f28 != (code *)0x0) && (DAT_002a1f30 != (code *)0x0)) {
        iVar4 = (*DAT_002a1f28)(__s1,(long)&local_40 + 4,&local_40);
        uVar5 = 0;
        if ((iVar4 != 0) && (0 < (int)local_40)) {
          iVar4 = (int)local_40 + 1;
          iVar1 = 0;
          if (iVar4 != 0) {
            iVar1 = (local_40._4_4_ + 1) / iVar4;
          }
          iVar4 = (*DAT_002a1f30)(__s1,(local_40._4_4_ + 1) - iVar1 * iVar4);
          uVar5 = (ulong)(iVar4 != 0);
        }
      }
    }
  }
  if (*(long *)(lVar2 + 0x28) == local_38) {
    return;
  }
LAB_00192658:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== nexus_rich_plan @ 00194890 ===== */

void nexus_rich_plan(void)

{
  (*(code *)PTR_nexus_rich_plan_001a3b50)();
  return;
}

