#pragma once
// Original VM member ABI: ECX=commands, stack=(routine,argc), RET8, EAX=status.
extern "C" {
int __fastcall kotor_fastcall_005cd230(void* commands, void*, int routine, int argc);
int __fastcall kotor_fastcall_005cd280(void* commands, void*, int routine, int argc);
int __fastcall kotor_fastcall_005cbf70(void* commands, void*, int routine, int argc);
int __fastcall kotor_fastcall_005cbfb0(void* commands, void*, int routine, int argc);
}
