#pragma once
// Lane W4: exact link-time providers that already exist in runtime1115 (kotor-vm-cell1115.map symbol in each
// comment). No definitions here. Every declaration below is checked by tools/check_undefined.py against the map.
#include "554bccf21783-w4_native.hpp"

struct W4Vector { float x, y, z; };
static_assert(sizeof(W4Vector) == 12, "Vector");

// ---- cells (existing canonical owners) ----
namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;   // ?manager_007a39fc@kotor_menu_window_cells@@3RAXA (0053-window-shared-cells)
extern void* volatile input_007a39e4;     // ?input_007a39e4@kotor_menu_window_cells@@3RAXA
}
namespace menu_continuation {
extern float frame_delta_0078e574;        // ?frame_delta_0078e574@menu_continuation@@3MA
extern std::uint32_t high_resolution_low_008328f0;  // 0456-r4-lowers-r4-cells
extern std::uint32_t high_resolution_high_008328f4;
extern const float scale_007439bc;        // 1e-6f
struct ExoBase;
extern ExoBase* exo_base_007a39e0;        // ?exo_base_007a39e0@menu_continuation@@3PAUExoBase@1@A
}
namespace frame_control {
extern std::uint32_t sound_manager_007a39ec;    // ?sound_manager_007a39ec@frame_control@@3IA
extern std::uint32_t message_manager_007a3a00;  // ?message_manager_007a3a00@frame_control@@3IA
extern std::uint32_t end_timer_low_008328e8, end_timer_high_008328ec;
extern const float profile_scale_0073d708;      // 0.001f
}
extern "C" {
extern volatile std::uint32_t kotor_cell_render_active_007a3a38;  // _kotor_cell_render_active_007a3a38
extern const std::uint32_t kotor_rdata_sound_mode_0074c5ec;      // _kotor_rdata_sound_mode_0074c5ec (.rdata)
}

// ---- functions (fastcall = original thiscall with dummy EDX) ----
extern "C" {
unsigned __fastcall kotor_fastcall_00641980(void* module, void*);                 // CSWCModule::GetMiniGame
void __fastcall kotor_fastcall_006200e0(void* client, void*, int, int);            // CClientExoAppInternal::SetInputClass
void __fastcall kotor_fastcall_006792e0(void* control, void*, int);                // CSWPlayerControl::SetEnabled
void __fastcall frame_R4_0062aa00(void* in_game, void*, unsigned, unsigned);       // CGuiInGame::SetSWGuiStatus
void __fastcall kotor_m1_opt_in_game_initialize_map_0062b0a0(void* in_game, void*); // CGuiInGame::InitializeMap
void __fastcall kotor_fastcall_0061f7d0(void* client, void*);                      // CClientExoAppInternal::EnableInput
void* __fastcall WindowCopy005e5b00(void* self, void*, void* source);              // CExoString::CExoString(const&)
void __fastcall frame_LogStringDtor_005e5c20(void* self, void*);                   // CExoString::~CExoString
unsigned __fastcall kotor_fastcall_0070c1f0(void* movie, void*);                   // CExoMoviePlayer::IsMoviePlaying
void __fastcall kotor_m15_client_internal_hide_load_screen_005f6e20(void* client, void*, int, int); // HideLoadScreen
void __fastcall kotor_fastcall_0061fa00(void* client, void*, int);                 // CClientExoAppInternal::ShowMouse
void __cdecl kotor_cdecl_00403de0();                                              // RestoreResolutionFromMovie
void __fastcall kotor_fastcall_0070c220(void* movie, void*);                       // CExoMoviePlayer::HideFadePanel
void __fastcall kotor_fastcall_004adff0(void* timer, void*);                       // CWorldTimer::PauseWorldTimer
void __fastcall kotor_fastcall_004adef0(void* timer, void*, void* source);         // CWorldTimer::ResetTimer
void __fastcall frame_R4_004adbd0(void* timer, void*);                             // CWorldTimer::SetSnapshotTime
unsigned long long __fastcall kotor_fastcall_004adc70(void* timer, void*);         // CWorldTimer::GetSnapshotTime
void __fastcall kotor_fastcall_004ae030(void* timer, void*);                       // CWorldTimer::UnpauseWorldTimer
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                     // CServerExoApp::GetWorldTimer
unsigned __fastcall frame_R4_005ed690(void* app, void*);                           // CClientExoApp::GetInGameGui
void __fastcall kotor_m15_ingame_start_global_fade_0062abf0(void* in_game, void*, int, float, float, void* colour); // StartGlobalFade
void __fastcall kotor_fastcall_005ed310(void* object, void*);                      // 005ED310 (mov ecx,[ecx+4]; jmp void_return)
void __fastcall frame_R4_005d5e80(void* sound, void*, unsigned);                   // CExoSoundInternal::SetSoundMode
void* __fastcall kotor_fastcall_005f1420(void* client, void*);                     // CClientExoAppInternal::GetPlayerCreature
unsigned __fastcall frame_R4_005ed5b0(void* app, void*, unsigned id);               // CClientExoApp::GetCreatureByGameObjectID
unsigned __fastcall frame_R4_005ed480(void* app, void*);                           // CClientExoApp::GetCaptureEventPlayBack
unsigned __fastcall frame_ContainsPanel_0040bd70(void* gui, void*, void* panel);   // CSWGuiManager::PanelExists
void* __fastcall kotor_fastcall_005ed700(void* app, void*);                        // CClientExoApp::GetClientOptions
void* __fastcall kotor_fastcall_005ed530(void* app, void*);                        // CClientExoApp::GetModule
unsigned __fastcall kotor_fastcall_004aea30(void* server, void*, unsigned id);      // CServerExoApp::ClientToServerObjectId
void* __fastcall kotor_fastcall_004ae750(void* server, void*, unsigned id);         // CServerExoApp::GetGameObject
void* __fastcall kotor_fastcall_005f10f0(void* client, void*, unsigned id);         // CClientExoAppInternal::GetGameObject
void __fastcall unresolved_list_next_005e9b90(void* list, void*, void* position);  // CExoLinkedListInternal::GetNext (returns data; unused)
unsigned __fastcall unresolved_list_at_pos_005e9b60(void* list, void*, void* pos);  // CExoLinkedListInternal::GetAtPos
void* __fastcall kotor_fastcall_005eda40(void* app, void*);                        // CClientExoApp::GetSWGuiManager
void* __fastcall frame_GetWorldTimer_005ed4f0(void* app, void*);                   // CClientExoApp::GetWorldTimer
}
namespace menu_continuation {
struct WorldTimer { long long __thiscall GetSnapshotTimeDifference_004adc80(); };   // 0457 r4 lowers
struct ExoTimers { long long __thiscall GetHighResolutionTimer_005e9260(); };       // 0457 r4 lowers
}
namespace options_video { struct Client; }
namespace lane_config_required {
int __fastcall WaitForMovie_005f6f20(options_video::Client*, void*, int);          // CClientExoAppInternal::WaitForMovie
}
namespace kotor_game_prepare {
void __fastcall SetGlobalUnpauseableState0062b410(void* in_game, void*, unsigned);  // CGuiInGame::SetGlobalUnpauseableState
}

namespace w4 {
inline void* app_manager() { return kotor_menu_window_cells::manager_007a39fc; }   // [007A39FC]
inline void* client_app() { return lp(app_manager(), 4); }                         // [[007A39FC]+4] CClientExoApp
inline void* server_app() { return lp(app_manager(), 8); }                         // [[007A39FC]+8] CServerExoApp
inline float frame_delta() { return menu_continuation::frame_delta_0078e574; }
inline U64 high_resolution_timer() {                                                 // [[007A39E0]+4]->GetHighResolutionTimer
    void* timers = lp(menu_continuation::exo_base_007a39e0, 4);
    return static_cast<U64>(static_cast<menu_continuation::ExoTimers*>(timers)->GetHighResolutionTimer_005e9260());
}
template <class R, class... A> inline R vcall(void* object, U32 slot, A... args) {
    return reinterpret_cast<R(__thiscall*)(void*, A...)>(vslot(object, slot))(object, args...);
}
}
