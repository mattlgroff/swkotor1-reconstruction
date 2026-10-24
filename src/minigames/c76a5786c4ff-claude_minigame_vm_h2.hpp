#pragma once
// T5 tier h2: SWMG completeness handlers (not used by vanilla minigame scripts).
// Original CSWVirtualMachineCommands handlers, int __thiscall (int routine, int argc), RET 8 -> __fastcall @16.
extern "C" {
// claude_minigame_vm_h2_string.cpp (isolated /EHs), except 005CB650 -> claude_minigame_vm_h2_f14.cpp
int __fastcall kotor_fastcall_005cb560(void* commands, void*, int routine, int argc); // 584 SWMG_GetLastEventModelName
int __fastcall kotor_fastcall_005cb650(void* commands, void*, int routine, int argc); // 585 SWMG_GetObjectByName
int __fastcall kotor_fastcall_005cbb80(void* commands, void*, int routine, int argc); // 597 SWMG_GetObjectName
int __fastcall kotor_fastcall_005cc450(void* commands, void*, int routine, int argc); // 625/626 SWMG_GetGunBank{Bullet,Gun}Model
int __fastcall kotor_fastcall_005cc7f0(void* commands, void*, int routine, int argc); // 632/633 SWMG_SetGunBank{Bullet,Gun}Model
// claude_minigame_vm_h2.cpp (default W5), except 005CD160 -> claude_minigame_vm_h2_f14.cpp
int __fastcall kotor_fastcall_005cb930(void* commands, void*, int routine, int argc); // 589 SWMG_GetLastBulletHitShooter
int __fastcall kotor_fastcall_005cbf00(void* commands, void*, int routine, int argc); // 610 SWMG_SetCameraClip
int __fastcall kotor_fastcall_005cc330(void* commands, void*, int routine, int argc); // 618 SWMG_SetMaxHitPoints / 622 SWMG_SetNumLoops
int __fastcall kotor_fastcall_005cc580(void* commands, void*, int routine, int argc); // 627 Damage / 631 Target / 640 IsGunBankTargetting
int __fastcall kotor_fastcall_005cc640(void* commands, void*, int routine, int argc); // 628 TimeBetweenShots / 629 Lifespan / 630 Speed (get)
int __fastcall kotor_fastcall_005cc700(void* commands, void*, int routine, int argc); // 657..660 Get{Horizontal,Vertical}Spread/SensingRadius/Inaccuracy
int __fastcall kotor_fastcall_005cc920(void* commands, void*, int routine, int argc); // 634 SetGunBankDamage / 638 SetGunBankTarget
int __fastcall kotor_fastcall_005cc9d0(void* commands, void*, int routine, int argc); // 635/636/637 SetGunBank{TimeBetweenShots,Lifespan,Speed}
int __fastcall kotor_fastcall_005ccaa0(void* commands, void*, int routine, int argc); // 661..664 Set{Horizontal,Vertical}Spread/SensingRadius/Inaccuracy
int __fastcall kotor_fastcall_005cd160(void* commands, void*, int routine, int argc); // 684/686/688 SWMG_SetSound{Frequency,FrequencyIsRandom,Volume}
}
