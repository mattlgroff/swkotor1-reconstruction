// lane-m48-b fork CLIENT, unit client-movie: CExoMoviePlayer PlayMoviesAsync / shutdown bodies, completing the M47 movie
// player (lane-m47-core rev2 core47/movie_player.cpp owns the constructor chain, worker, 00405080/00405920/00404BB0).
// Consumers: lane_config_tables.cpp (intro queue binding PlayMoviesAsync_0070c200, movies-enabled path 00602B7D) and
// routes-shutdown (ShutDownMoviePlayer 005F7190 at 00604154). Under the documented first-frame route (Disable Movies=1)
// neither runs; both are link-required.
// Evidence: dumpbin /disasm swkotor.exe (tools/dis); IAT names from dumpbin /imports (0073D1F4 SetThreadPriority,
// 0073D1F8 SetEvent, 0073D1FC GetExitCodeThread, 0073D200 CloseHandle, 0073D1E4 Sleep); .rdata 0073D774/0073D810 read
// with dumpbin /rawdata and reproduced as literals of identical bytes.
#include "e6446a7354-r4_lowers.h"
#include "96c5832fd3-startup_string.h"

using namespace r4_lowers;
using kotor_reconstruction::StartupString;

namespace kotor_reconstruction { struct ArrayMutations;
    __declspec(noinline) StartupString* __fastcall StartupAssign005e5c50(StartupString*, void*, StartupString*); }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, std::uint32_t); }
namespace kotor_menu20 { struct Debug; }
namespace base_ini_reconstruction { void __fastcall warning_005e9400(kotor_menu20::Debug*, void*, int, const char*, const char*); }
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }
namespace kotor_startup { struct CriticalSection { CriticalSection* __thiscall Construct(); void __thiscall Destroy(); }; }
namespace gui_load { using Word = std::uint32_t; }
extern "C" {
__declspec(dllimport) int __stdcall SetThreadPriority(void* thread, int priority);           // [0073D1F4]
__declspec(dllimport) int __stdcall SetEvent(void* event);                                  // [0073D1F8]
__declspec(dllimport) int __stdcall GetExitCodeThread(void* thread, std::uint32_t* code);   // [0073D1FC]
std::uint32_t __stdcall manager_close_handle_0073d200(std::uint32_t);                        // [0073D200] adapter
void __stdcall c2da_sleep_0073d1e4(std::uint32_t);                                          // [0073D1E4] adapter
StartupString* __fastcall WindowCopy005e5b00(StartupString*, void*, const StartupString* volatile);
void __fastcall gui_load_array_add_00671c00(kotor_reconstruction::ArrayMutations*, void*, gui_load::Word);
void __fastcall exo_array_list_destruct_0056b6f0(void*, void*);
void __fastcall kotor_fastcall_00405080(void* list, void*, I32 count) noexcept(false);     // movie_player.cpp
void __fastcall kotor_fastcall_00405920(void* list, void*);                                // movie_player.cpp
void __fastcall kotor_fastcall_00404bb0(void* internal, void*) noexcept(false);            // movie_player.cpp
int __fastcall kotor_fastcall_0070c1f0(void* player, void*);                               // IsMoviePlaying (routes-client-wrappers)
void __fastcall kotor_fastcall_0070c1e0(void* player, void*, U32, U32);                   // CancelMovie (core47 teardown)
// closed / new here
void __fastcall kotor_fastcall_00405150(void* list, void*, I32 count);
void __fastcall kotor_fastcall_00405d50(void* list, void*, U32 reallocate);
void* __fastcall kotor_fastcall_00405a20(void* list, void*, void* source);
// abi-a1: 004059A0 CExoArrayList<CExoString>::Add(CExoString) uses claude_0886_bodies_vars here, with the
// original ABI: thiscall, the 8-byte CExoString by value in the two stack dwords, RET 8, callee destroys it.
struct ExoStringArg { U32 data, length; };                                                 // CExoString by value
void __fastcall claude_exo_string_list_add_004059a0(void* list, void*, ExoStringArg value) noexcept(false);
void* __fastcall kotor_fastcall_00405d10(void* list, void*, void* source) noexcept(false);
void __fastcall kotor_fastcall_00405e50(void* internal, void*, void* movies, U32 flags, void* skippable) noexcept(false);
void __fastcall kotor_fastcall_00405a50(void* internal, void*) noexcept(false);
void __fastcall kotor_fastcall_0070c250(void* player, void*) noexcept(false);
void __fastcall kotor_fastcall_005f7190(void* internal, void*);
}
namespace lane_config_required { void __fastcall PlayMoviesAsync_0070c200(void*, void*, void*, std::uint32_t, void*); }

namespace {
const char k_source_file_0073d774[] = "exomovieplayerinternal.cpp";
const char k_more_movies_0073d810[] = "CExoMoviePlayerInternal::PlayMoviesAsync(): More movies specified than skippable flags.";
}

// 00405150 CExoArrayList<unsigned long>::SetSize(n), RET 4.
extern "C" void __fastcall kotor_fastcall_00405150(void* list, void*, I32 count) {
    if (count == 0) {                                                   // 00405152..0040515A
        if (void* data = pw(list, 0)) {                                 // 0040515C..00405160
            live_free(data);                                            // 00405162..00405163
            sw(list, 0, 0); sw(list, 8, 0); sw(list, 4, 0);             // 0040516B..00405170
            return;
        }
    } else {
        menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
                                            static_cast<U32>(count));   // 00405178..0040517B
    }
    sw(list, 4, static_cast<U32>(count));                               // 00405180
}
// 00405D50 CExoArrayList<int>::clear(reallocate), RET 4: size = 0; SetSize(0) only when reallocate != 0.
extern "C" void __fastcall kotor_fastcall_00405d50(void* list, void*, U32 reallocate) {
    sw(list, 4, 0);                                                     // 00405D56
    if (reallocate != 0) kotor_fastcall_00405150(list, nullptr, 0);     // 00405D54..00405D67 (tail jump, arg rewritten 0)
}
// 00405A20 CExoArrayList<int> assignment from *source (size reset, Add each), RET 4, returns this.
extern "C" void* __fastcall kotor_fastcall_00405a20(void* list, void*, void* source) {
    sw(list, 4, 0);                                                     // 00405A2B
    for (I32 i = 0; i < static_cast<I32>(w(source, 4)); ++i)            // 00405A2E..00405A46
        gui_load_array_add_00671c00(static_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
                                    w(pw(source, 0), static_cast<U32>(i) * 4)); // 00405A33..00405A3B
    return list;                                                        // 00405A48
}
// 00405D10 CExoArrayList<CExoString> assignment from *source, RET 4, returns this.
extern "C" void* __fastcall kotor_fastcall_00405d10(void* list, void*, void* source) noexcept(false) {
    sw(list, 4, 0);                                                     // 00405D1B
    for (I32 i = 0; i < static_cast<I32>(w(source, 4)); ++i) {          // 00405D1E..00405D44
        const StartupString* element = reinterpret_cast<const StartupString*>(bytes(pw(source, 0)) + static_cast<U32>(i) * 8); // 00405D23..00405D28
        ExoStringArg value;                                             // 00405D25 sub esp,8: Add's by-value slot
        WindowCopy005e5b00(reinterpret_cast<StartupString*>(&value), nullptr, element); // 00405D2B..00405D32 copy into the slot
        claude_exo_string_list_add_004059a0(list, nullptr, value);      // 00405D37..00405D39 (Add destroys the slot)
    }
    return list;                                                        // 00405D48
}

// 00405E50 CExoMoviePlayerInternal::PlayMoviesAsync(movies, flags, skippable), RET 0Ch.
extern "C" void __fastcall kotor_fastcall_00405e50(void* internal, void*, void* movies, U32 flags, void* skippable) noexcept(false) {
    SetThreadPriority(pw(internal, 0x38), 0);                           // 00405E53..00405E59
    kotor_fastcall_00405d10(bytes(internal) + 8, nullptr, movies);      // 00405E5F..00405E67
    sw(internal, 0x8c, flags);                                          // 00405E72..00405E76
    void* skip = bytes(internal) + 0x90;                                // 00405E7C
    if (skippable) {                                                    // 00405E6C..00405E82
        kotor_fastcall_00405a20(skip, nullptr, skippable);              // 00405E84..00405E85
        if (static_cast<I32>(w(internal, 0x0c)) > static_cast<I32>(w(internal, 0x94))) // 00405E8A..00405E93
            base_ini_reconstruction::warning_005e9400(static_cast<kotor_menu20::Debug*>(pw(kotor_menu_10_19::dependencies::base_007a39e0, 8)),
                                                      nullptr, 0x653, k_source_file_0073d774, k_more_movies_0073d810); // 00405E95..00405EAD
    } else {
        kotor_fastcall_00405d50(skip, nullptr, 0);                      // 00405EB4..00405EB6
    }
    sw(internal, 0x30, 1);                                              // 00405EBF
    SetEvent(pw(internal, 4));                                          // 00405EBB..00405EC6
}
// 0070C200 CExoMoviePlayer::PlayMoviesAsync: mov ecx,[ecx]; jmp 00405E50 (lane-config consumer spelling).
void __fastcall lane_config_required::PlayMoviesAsync_0070c200(void* player, void*, void* movies, std::uint32_t flags, void* skippable) {
    kotor_fastcall_00405e50(pw(player, 0), nullptr, movies, flags, skippable); // 0070C200..0070C202
}

// 00405A50 CExoMoviePlayerInternal::~CExoMoviePlayerInternal.
extern "C" void __fastcall kotor_fastcall_00405a50(void* internal, void*) noexcept(false) {
    sw(internal, 0, 1);                                                 // 00405A7C
    SetEvent(pw(internal, 4));                                          // 00405A70..00405A82
    std::uint32_t code = 0;
    do { GetExitCodeThread(pw(internal, 0x38), &code); } while (code == 0x103u); // 00405A90..00405AA3 (STILL_ACTIVE)
    manager_close_handle_0073d200(w(internal, 0x38));                   // 00405AA5..00405AAF
    manager_close_handle_0073d200(w(internal, 4));                      // 00405AB1..00405AB5
    if (void* object = pw(internal, 0x44)) {                            // 00405AB7..00405ABC
        vcall1(object, 0, 1);                                           // 00405ABE..00405AC2 (scalar deleting destructor)
        sw(internal, 0x44, 0);                                          // 00405AC4
    }
    kotor_fastcall_00404bb0(internal, nullptr);                         // 00405ACB..00405ACD
    exo_array_list_destruct_0056b6f0(bytes(internal) + 0x90, nullptr); // 00405AD2..00405ADD
    frame_LogStringDtor_005e5c20(reinterpret_cast<frame_log::String*>(bytes(internal) + 0x68), nullptr); // 00405AE2..00405AEA
    reinterpret_cast<kotor_startup::CriticalSection*>(bytes(internal) + 0x40)->Destroy(); // 00405AEF..00405AF7 (005E9320 -> 005EC260 DeleteCriticalSection; task-31 owner)
    frame_LogStringDtor_005e5c20(reinterpret_cast<frame_log::String*>(bytes(internal) + 0x28), nullptr); // 00405AFC..00405B04
    kotor_fastcall_00405920(bytes(internal) + 8, nullptr);              // 00405B09..00405B14
}
// 0070C250 CExoMoviePlayer::~CExoMoviePlayer: internal [this] -> ~Internal, free.
extern "C" void __fastcall kotor_fastcall_0070c250(void* player, void*) noexcept(false) {
    if (void* internal = pw(player, 0)) {                               // 0070C252..0070C258
        kotor_fastcall_00405a50(internal, nullptr);                     // 0070C25A..0070C25C
        live_free(internal);                                            // 0070C261..0070C262
    }
}
// 005F7190 CClientExoAppInternal::ShutDownMoviePlayer (player at [this+12Ch]).
extern "C" void __fastcall kotor_fastcall_005f7190(void* internal, void*) {
    if (!pw(internal, 0x12c)) return;                                   // 005F7193..005F719B
    if (kotor_fastcall_0070c1f0(pw(internal, 0x12c), nullptr) == 1) {   // 005F719E..005F71A6
        do {
            kotor_fastcall_0070c1e0(pw(internal, 0x12c), nullptr, 1, 1); // 005F71B0..005F71BA
            c2da_sleep_0073d1e4(5);                                     // 005F71BF..005F71C1
        } while (kotor_fastcall_0070c1f0(pw(internal, 0x12c), nullptr) == 1); // 005F71C3..005F71D1
    }
    if (void* player = pw(internal, 0x12c)) {                           // 005F71D3..005F71DB
        kotor_fastcall_0070c250(player, nullptr);                       // 005F71DD..005F71DF
        live_free(player);                                              // 005F71E4..005F71E5
    }
    sw(internal, 0x12c, 0);                                             // 005F71ED
}

// ---- RestartMoviesIfNecessary chain (routes-window-leaves consumer 005ED980 -> 005F9780) ----
namespace options_video { struct Client; }
namespace lane_config_required { std::int32_t __fastcall WaitForMovie_005f6f20(options_video::Client*, void*, std::int32_t); } // client-wait-movie
namespace kotor_game_prepare { void __fastcall ShowMouse0061fa00(void* self, void* edx, int); }
extern "C" {
void __fastcall kotor_fastcall_0061f9c0(void* internal, void*, std::uint32_t);   // HideMouse (routes-window-leaves)
void __cdecl kotor_cdecl_00403cf0();                                            // client-resolution
void __cdecl kotor_cdecl_00403de0();                                            // client-resolution
void __fastcall kotor_fastcall_0070c220(void* player, void*);                   // HideFadePanel (startup46)
U32 __fastcall kotor_fastcall_00405040(void* internal, void*);
U32 __fastcall kotor_fastcall_0070c230(void* player, void*);
void __fastcall kotor_fastcall_00405050(void* internal, void*);
void __fastcall kotor_fastcall_0070c240(void* player, void*);
void __fastcall kotor_fastcall_005f9780(void* internal, void*);
}
// 00405040 CExoMoviePlayerInternal::GetNumberOfMoviesToPlay: [this+0Ch] (movie list size).
extern "C" U32 __fastcall kotor_fastcall_00405040(void* internal, void*) { return w(internal, 0x0c); } // 00405040
// 0070C230 CExoMoviePlayer::GetNumberOfMoviesToPlay: internal ? 00405040 : 0.
extern "C" U32 __fastcall kotor_fastcall_0070c230(void* player, void*) {
    void* internal = pw(player, 0);                                     // 0070C230
    return internal ? kotor_fastcall_00405040(internal, nullptr) : 0;   // 0070C232..0070C23B
}
// 00405050 CExoMoviePlayerInternal::RestartMoviesAsync: SetThreadPriority([+38],0) twice, [+30]=1, SetEvent([+4]).
extern "C" void __fastcall kotor_fastcall_00405050(void* internal, void*) {
    SetThreadPriority(pw(internal, 0x38), 0);                           // 0040505A..00405060
    SetThreadPriority(pw(internal, 0x38), 0);                           // 00405062..00405068
    sw(internal, 0x30, 1);                                              // 0040506E
    SetEvent(pw(internal, 4));                                          // 0040506A..00405075
}
// 0070C240 CExoMoviePlayer::RestartMoviesAsync: mov ecx,[ecx]; jmp 00405050 (no null test).
extern "C" void __fastcall kotor_fastcall_0070c240(void* player, void*) { kotor_fastcall_00405050(pw(player, 0), nullptr); } // 0070C240..0070C242
// 005F9780 CClientExoAppInternal::RestartMoviesIfNecessary.
extern "C" void __fastcall kotor_fastcall_005f9780(void* internal, void*) {
    if (!pw(internal, 0x12c)) return;                                   // 005F9783..005F978B
    if (kotor_fastcall_0070c230(pw(internal, 0x12c), nullptr) == 0) return; // 005F978D..005F9794 (jbe)
    kotor_fastcall_0061f9c0(internal, nullptr, 8);                      // 005F9796..005F979A
    kotor_cdecl_00403cf0();                                             // 005F979F
    kotor_fastcall_0070c240(pw(internal, 0x12c), nullptr);              // 005F97A4..005F97AA
    lane_config_required::WaitForMovie_005f6f20(static_cast<options_video::Client*>(internal), nullptr, 1); // 005F97AF..005F97B3
    kotor_cdecl_00403de0();                                             // 005F97B8
    kotor_game_prepare::ShowMouse0061fa00(internal, nullptr, 8);        // 005F97BD..005F97C1
    kotor_fastcall_0070c220(pw(internal, 0x12c), nullptr);              // 005F97C6..005F97CD (tail jump)
}
