#pragma once
// Lane W4 G12: in-game continuations of 00602EB0 CClientExoAppInternal::MainLoop and the client per-frame update
// chain. Entry points are extern "C" __fastcall (ECX = original `this`, EDX unused) so the existing owner files
// (frame_driver_loading.cpp, menu_r2.cpp, frame_mainloop_loading.cpp, frame_menu_path_client.cpp) call them
// without new headers. Each continuation ends exactly where the existing owner resumes.
#include "3d9f1bbdade6-w4_providers.hpp"

extern "C" {
// 00602F51..00603253 (area-entry setup). Resumes the owner at 00603254. *stack20 receives the original
// [esp+20h] write at 00603248 (1) only when the [7A3A38] block ran to completion.
void __fastcall w4_mainloop_area_entry_00602f51(void* client, void*, std::uint32_t* stack20);
// 006035A6..00603892 (player-model animation test queue). Entered after vtable+98h returned non-zero.
// Resumes the owner at 00603893.
void __fastcall w4_mainloop_anim_queue_006035a6(void* client, void*);
// 006038D6..00603AA3 (in-game world tick). character = ebp (CSWParty::GetCharacter(0) at 0060389A),
// area = module+48h (non-null). Resumes the owner at 00603AA4.
void __fastcall w4_mainloop_world_tick_006038d6(void* client, void*, void* character, void* area);
// 00603AC5..00603B15 (server dialog tick). dialog_id = in_game+188h (!= 7F000000), server non-null.
// Resumes the owner at 00603B16.
void __fastcall w4_mainloop_server_dialog_00603ac5(void* client, void*, std::uint32_t dialog_id);

// W4-owned callees (defined in W4 units).
void __fastcall kotor_fastcall_006048c0(void* ai_master, void*);                     // CClientAIMaster::UpdateState
void* __fastcall kotor_fastcall_00604870(void* ai_list, void*);                     // CClientAIList::GetNextObject
void __fastcall kotor_fastcall_00644650(void* module, void*, float delta);          // CSWCModule::Update
void __fastcall kotor_fastcall_005ee860(void* client, void*, void* position);       // CClientExoAppInternal::UpdateRoom
std::uint8_t __fastcall kotor_fastcall_005ee910(void* client, void*);              // GetActivePauseState
void* __fastcall kotor_fastcall_005ed560(void* app, void*);                         // CClientExoApp::GetModuleCamera
void* __fastcall kotor_fastcall_005ee6f0(void* client, void*);                      // CClientExoAppInternal::GetModuleCamera
void* __fastcall kotor_fastcall_005ee3e0(void* module, void*, void* out);           // CSWCModule::GetModuleName
std::uint32_t __fastcall kotor_fastcall_004aed20(void* server, void*);             // CServerExoApp::GetAutoSavePending
void __fastcall kotor_fastcall_005f3030(void* client, void*, std::uint32_t value); // SetCanSendDialog
void __fastcall kotor_fastcall_005f2bb0(void* client, void*, std::uint32_t id);    // SetPlayerCreature
std::int32_t __fastcall kotor_fastcall_006364c0(void* party, void*, std::uint32_t id); // CSWParty::AddCharacter
const char* __cdecl kotor_cdecl_00401430(std::uint32_t);                            // ClearLogCounters
void __fastcall kotor_fastcall_00604d70(void* area, void*);                         // CSWCArea::StartSounds
void __fastcall kotor_fastcall_005fb6e0(void* client, void*);                       // CrownFutureLeader
void __fastcall kotor_fastcall_005f5e10(void* client, void*, float delta);          // UpdateCamera (G14)
void __fastcall kotor_fastcall_0063fe10(void* module, void*, std::uint32_t, std::uint32_t); // TurnCamera (G14)
void __fastcall kotor_fastcall_00640090(void* module, void*, std::uint32_t, std::uint32_t); // AcclTurnCamera (G14)
W4Vector* __fastcall kotor_fastcall_0062ed30(void* in_game, void*, W4Vector* out, int flag); // GetDialogCameraPosition (G19 client)
void __fastcall kotor_fastcall_006097f0(void* area, void*, float delta);            // CSWCArea::Update
std::uint32_t __fastcall kotor_fastcall_006441d0(void* module, void*, float delta, std::uint32_t* out_a, std::uint32_t* out_b); // FindClearView (G14)
// Cross-lane callees (owner W5; see INTEGRATION.md: pending trap stubs in w4_pending_traps.cpp).
void __fastcall kotor_fastcall_004cd580(void* object, void*);                       // CSWSObject::UpdateDialog (W5)
void __fastcall recovered_object_delete_dialog_004cc330(void* object, void*);                       // CSWSObject::DeleteDialog (W5)
}

namespace w4 {
// W4-owned write-only profiling cells (no other original reader/writer; see MAINLOOP-PATHS.md).
extern float profile_ai_update_00832908;
extern U32 anim_queue_flag_00832c50;
}
