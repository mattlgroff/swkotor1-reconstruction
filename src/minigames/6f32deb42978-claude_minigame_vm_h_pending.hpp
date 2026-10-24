#pragma once
// Callable contracts for h1 callees that have no shared header.
// 0066C160 / 0066CB10: t1 tier c1 bodies (exports @kotor_fastcall_0066c160@8 / @kotor_fastcall_0066cb10@8),
//   original: mov esi,ecx ... ret (thiscall, no stack args, pointer result in EAX).
// 0066C870: claude_gui_char_minigame.obj (c63 @kotor_fastcall_0066c870@12), original RET 4, result in ST0.
// The t1 c1 header itself includes "resources.h", so the two declarations are restated here verbatim.
extern "C" {
void* __fastcall kotor_fastcall_0066c160(void*, void*);
void* __fastcall kotor_fastcall_0066cb10(void*, void*);
float __fastcall kotor_fastcall_0066c870(void* follower, void*, float speed);
}
