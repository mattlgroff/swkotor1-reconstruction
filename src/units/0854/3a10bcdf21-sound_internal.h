#pragma once
// Lane-sound: CExoSoundInternal constructor 005D86E0 and Initialize 005DA8A0
// plus the small container helpers they call. __fastcall(ECX receiver, EDX
// ignored, stack DWORDs) == original __thiscall ABI (D1). No storage here.
#include <cstdint>
#include "f9f7ab0530-startup_string.h"
#include "65c9c926f6-constructors.h" // kotor_list::Internal (005E9AA0)

namespace kotor_sound_internal {
using U = std::uint32_t;
void* __fastcall Construct005d86e0(void* self, void*);                                   // RET 0, EAX=this
U __fastcall Initialize005da8a0(void* self, void*, U voices2d, U voices3d, U eax_mode, U hardware); // RET 10h
void* __fastcall ListConstruct005d6c40(void* list, void*);                               // RET 0, EAX=list
void __fastcall ListUnwind005d86c0(void* list, void*);                                   // RET 0
U __fastcall ListContains005e9d40(void* internal, void*, void* data);                    // RET 4
void* __fastcall StringListConstruct005e9530(void* self, void*);                         // RET 0
void* __fastcall StringListGet005e95e0(void* self, void*, U index);                      // RET 4
void __fastcall StringListDestroy005e9860(void* self, void*);                            // RET 0
void __fastcall ArrayListDestroy0056b6f0(void* self, void*);                             // RET 0
void __fastcall Render005d9590(void* self, void*);                                       // RET 0 (sound_render.cpp)
void __fastcall SetMusicVolume005d6940(void* self, void*, float v, U keep_base, U apply_group); // RET 0Ch
}

// Existing M41 providers (declarations matching their defining objects).
namespace base_ini_reconstruction {
struct Node;
void __fastcall destroy_list_005e9ab0(kotor_list::Internal*, void*);   // ini-list-cleanup.obj
void* __fastcall remove_head_005e9c80(kotor_list::Internal*, void*);   // ini-list-cleanup.obj
extern const char warning_empty_0073d71c[];                            // ini-local-warning-literals.obj
}
namespace ini_addhead {
base_ini_reconstruction::Node* __fastcall AddHead005e9d70(kotor_list::Internal*, void*, void*); // ini-add-head.obj
}
extern "C" {
std::uint32_t __fastcall unresolved_list_next_005e9b90(void*, void*, void**);   // resource-list-accessor-exports.obj
std::uint32_t __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);  // resource-list-accessor-exports.obj
kotor_reconstruction::StartupString* __fastcall GuiAssign005e5140(
    kotor_reconstruction::StartupString*, void*, const char*);                  // ini-string-priority.obj
}
namespace kotor_menu_10_19 {
using Wide = unsigned __int64;
struct TimerDependency { Wide __thiscall high_resolution_005e9260(); };          // world-timer-high-resolution.obj
namespace dependencies {
extern void* base_007a39e0;                                                     // modern-game-storage.obj
Wide __stdcall unsigned_divide_006fb030(Wide, Wide);                            // unsigned-divide-helpers.obj
}
}

// Existing sound donors (names as declared by those packets).
namespace sound_lower29 { std::uint32_t __fastcall ActorPlaying005db0e0(void*, void*); }
namespace sound_streaming29 { std::uint32_t __fastcall InternalIsPlaying005dc300(void*, void*); }
namespace sound_pause29_required {
std::uint32_t __fastcall stream_pause_005dc3c0(void*, void*);
std::uint32_t __fastcall stream_resume_005dcdb0(void*, void*);
extern volatile std::uint32_t* const word_00832890;
}
namespace sound_stream_play29_required { void __fastcall open_005dbd30(void*, void*); }
namespace sound_lifecycle29_required { extern volatile std::uint32_t* const word_00832894; }
namespace sound_mode29_required { extern const volatile std::uint32_t* const word_0074c5ec; }

// Genuinely missing CExoSoundInternal / CExoSoundSourceInternal lowers.
namespace kotor_sound_path_required {
void __fastcall load_priority_groups_005d7bd0(void*, void*);                    // RET 0 (1022 bytes)
void __fastcall enumerate_providers_005da6d0(void*, void*);                     // RET 0 (463 bytes)
void __fastcall provider_preferences_005d66d0(void*, void*, std::uint32_t);     // RET 4 (616 bytes)
std::uint32_t __fastcall remove_playing_005d78f0(void*, void*, void*);          // RET 4, EAX = next position
void __fastcall shutdown_source_005db210(void*, void*);                         // RET 0
void __fastcall store_restart_005db380(void*, void*);                           // RET 0
std::uint32_t __fastcall source_play_005db4d0(void*, void*, std::uint32_t, std::uint32_t); // RET 8, EAX = [src+58h] (ms)
}

// Original Miles imports (mss32.dll; import library generated locally per D17).
extern "C" {
__declspec(dllimport) std::int32_t __stdcall AIL_set_preference(std::uint32_t, std::int32_t);
__declspec(dllimport) char* __stdcall AIL_set_redist_directory(const char*);
__declspec(dllimport) std::int32_t __stdcall AIL_quick_startup(std::int32_t, std::int32_t, std::uint32_t, std::int32_t, std::int32_t);
__declspec(dllimport) void __stdcall AIL_quick_shutdown(void);
__declspec(dllimport) void __stdcall AIL_quick_handles(void**, void**, void**);
__declspec(dllimport) std::uint32_t __stdcall AIL_open_3D_provider(void*);
__declspec(dllimport) void __stdcall AIL_close_3D_provider(void*);
__declspec(dllimport) void* __stdcall AIL_allocate_3D_sample_handle(void*);
__declspec(dllimport) void* __stdcall AIL_allocate_sample_handle(void*);
__declspec(dllimport) void __stdcall AIL_init_sample(void*);
__declspec(dllimport) void __stdcall AIL_set_3D_rolloff_factor(void*, float);
__declspec(dllimport) void __stdcall AIL_end_3D_sample(void*);
__declspec(dllimport) void __stdcall AIL_release_3D_sample_handle(void*);
__declspec(dllimport) void __stdcall AIL_end_sample(void*);
__declspec(dllimport) void __stdcall AIL_release_sample_handle(void*);
__declspec(dllimport) void __stdcall AIL_close_stream(void*);
}
