#pragma once
// T5 tier h1: swoop-used SWMG VM handlers. Original CSWVirtualMachineCommands handlers,
// int __thiscall (int routine, int argc), RET 8 -> exported __fastcall @16.
extern "C" {
int __fastcall kotor_fastcall_005cbb00(void* commands, void*, int routine, int argc); // SWMG_GetLastFollowerHit 593 / SWMG_GetLastObstacleHit 594
int __fastcall kotor_fastcall_005cbdb0(void* commands, void*, int routine, int argc); // SWMG_RemoveAnimation 607
int __fastcall kotor_fastcall_005cce70(void* commands, void*, int routine, int argc); // SWMG_SetPlayer{Speed,MinSpeed,AccelerationPerSecond,MaxSpeed} 649/650/651/668
int __fastcall kotor_fastcall_005ccf30(void* commands, void*, int routine, int argc); // SWMG_SetPlayer{Offset,TunnelPos,TunnelNeg,Origin,TunnelInfinite} 647/652/654/656/718
}
