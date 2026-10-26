// Lane-sound batch 3: CExoSoundSourceInternal::Play (005DB4D0) and its lowers:
// Demand/Release (005DAE50/005DAE90), CRes::Demand/Release (00409B10/00409B80),
// DoObstructionWork (005DAEB0), InterruptPlayingSound (005D7FE0),
// GetFree3DVoice/GetFree2DVoice (005D8EB0/005D8FF0), pitch/fixed variance
// (005DB3D0/005DB450). Offsets observed; src = CExoSoundSourceInternal.
#include "3a10bcdf21-sound_internal.h"
#include <cstdlib>
#include <cstring>
#include <new>
// [M47 compose] D52: single original-RNG provider (lane-3d-model crt_rand_006fc447 over compat_getptd; original _rand 006FC447)
extern "C" std::int32_t __cdecl crt_rand_006fc447();

namespace menu_resources { struct Manager; }
extern "C" {
menu_resources::Manager* __cdecl c2da_manager_007a39e8();                                         // resource-borrowed-accessors.obj
std::uint32_t __fastcall c2da_manager_demand_004089f0(menu_resources::Manager*, void*, void*);     // task10-c2da-manager-demand.obj
std::uint32_t __fastcall c2da_manager_release_demand_body_00408c90(menu_resources::Manager*, void*, void*);
char* __fastcall async_string_cstr_005e5670(kotor_reconstruction::StartupString*, void*);          // task10-canonical-ref-string-carriers.obj
__declspec(dllimport) void __stdcall AIL_set_3D_sample_obstruction(void*, float);
__declspec(dllimport) void __stdcall AIL_stop_3D_sample(void*);
__declspec(dllimport) void __stdcall AIL_set_3D_sample_loop_count(void*, std::int32_t);
__declspec(dllimport) void __stdcall AIL_set_3D_sample_offset(void*, std::uint32_t);
__declspec(dllimport) void __stdcall AIL_resume_3D_sample(void*);
__declspec(dllimport) void __stdcall AIL_stop_sample(void*);
__declspec(dllimport) void __stdcall AIL_set_sample_loop_count(void*, std::int32_t);
__declspec(dllimport) void __stdcall AIL_set_sample_position(void*, std::uint32_t);
__declspec(dllimport) void __stdcall AIL_resume_sample(void*);
__declspec(dllimport) void __stdcall AIL_WAV_info(void*, void*);
}
namespace sound_lower29_required {
void __fastcall start3d_005d71c0(void*, void*, void*, void*);
void __fastcall start2d_005d7480(void*, void*, void*, void*);
}
namespace sound_pause29_required {
std::uint32_t __fastcall actor_pause_005db150(void*, void*);
extern volatile std::uint32_t* const word_00832890;
}
namespace kotor_sound_internal { std::uint32_t __fastcall SourcePauseable005d6b70(void*, void*, void*); }
namespace kotor_sound_path_required {
std::uint32_t __fastcall remove_nonplaying_looping_005d79c0(void*, void*, void*);
void __fastcall add_playing_005d7b40(void*, void*, void*);
void __fastcall add_nonplaying_looping_005d7b80(void*, void*, void*);
std::uint32_t __fastcall source_in_range_005db050(void*, void*);
}

namespace {
using U = std::uint32_t;
unsigned char* at(void* p, unsigned off) { return static_cast<unsigned char*>(p) + off; }
U rd(void* p, unsigned off) { U v; std::memcpy(&v, at(p, off), 4); return v; }
void wr(void* p, unsigned off, U v) { std::memcpy(at(p, off), &v, 4); }
void* rp(void* p, unsigned off) { void* v; std::memcpy(&v, at(p, off), 4); return v; }
void wp(void* p, unsigned off, void* v) { std::memcpy(at(p, off), &v, 4); }
float rf(void* p, unsigned off) { float v; std::memcpy(&v, at(p, off), 4); return v; }
unsigned char rb(void* p, unsigned off) { return *at(p, off); }
void* bp(U v) { void* p; std::memcpy(&p, &v, 4); return p; }
kotor_list::Internal* L(void* sys, unsigned off) { return static_cast<kotor_list::Internal*>(rp(sys, off)); }
void* next(kotor_list::Internal* l, void** pos) { return bp(unresolved_list_next_005e9b90(l, nullptr, pos)); }
unsigned char* group(void* sys, unsigned char index) { return static_cast<unsigned char*>(bp(rd(sys, 0x4c) + static_cast<U>(index) * 24u)); }
void destroy_source(void* source) {                                  // vtable slot 0, push 1
    using Deleting = void(__fastcall*)(void*, void*, U);
    Deleting del; std::memcpy(&del, rp(source, 0), 4); del(source, nullptr, 1);
}
double u32d(U v) { return static_cast<double>(v); }                  // FILD + [0073E290]/[0073D770] 2^32 fix-up
long long ftol(double x) { return static_cast<long long>(x); }       // __ftol2 006FAE8C (truncation)
}

namespace kotor_resource_required {
// 00409B80 CRes::Release: CExoResMan::Release(res) on [007A39E8] (00408C90), EAX returned.
// [M47 compose] 00409B80 CRes::Release: single body = lane-resman @c2da_release_demand_00409b80@8 (D18); name joined (definition removed; declaration kept)
std::uint32_t __fastcall res_release_00409b80(void* res, void*);
// 00409B10 CRes::Demand: CExoResMan::Demand(res) on [007A39E8] (004089F0).
// [M47 compose] 00409B10 CRes::Demand: single body = lane-resman @c2da_current_resource_demand_00409b10@8 (D18); name joined (definition removed; declaration kept)
std::uint32_t __fastcall res_demand_00409b10(void* res, void*);
}

namespace kotor_sound_internal {
// 005DAE50 CExoSoundSourceInternal::Demand, RET 0.
U __fastcall SourceDemand005dae50(void* src, void*) {
    void* res = rp(src, 8);
    if (!res) return 0;                                              // 005DAE53-005DAE5D
    if (rd(src, 0x30) && *reinterpret_cast<std::uint16_t*>(at(res, 4)) != 0) return 1; // 005DAE5E-005DAE74 (004064B0 GetDemands)
    const U ok = kotor_resource_required::res_demand_00409b10(res, nullptr) != 0 ? 1u : 0u; // 005DAE75-005DAE81
    wr(src, 0x30, ok);                                               // 005DAE83
    return ok;
}
// 005DAE90 CExoSoundSourceInternal::Release, RET 0.
U __fastcall SourceRelease005dae90(void* src, void*) {
    if (!rd(src, 0x30)) return 0;                                    // 005DAE90-005DAE95 / 005DAEAD
    wr(src, 0x30, 0);                                                // 005DAE97
    return kotor_resource_required::res_release_00409b80(rp(src, 8), nullptr) != 0 ? 1u : 0u; // 005DAE9E-005DAEAA
}
// 005DB3D0 CalculatePitchVarianceFrequency, RET 0.
void __fastcall PitchVariance005db3d0(void* src, void*) {
    const int r = crt_rand_006fc447();                                       // 005DB3D5 (_rand 006FC447 -> CRT rand)
    const double base = static_cast<double>(rf(src, 0x74)) * u32d(rd(src, 0x48)); // 005DB3E6-005DB3FB
    if (r & 1) {                                                     // 005DB3DA-005DB3EC
        const U t = static_cast<U>(ftol(base));                      // 005DB3FD
        if (t) wr(src, 0x54, rd(src, 0x54) + static_cast<U>(crt_rand_006fc447()) % t); // 005DB404-005DB417
    } else {
        const U t = static_cast<U>(ftol(base * 0.5));                // 005DB41D-005DB432 ([0073E9AC]=0.5)
        if (t) wr(src, 0x54, rd(src, 0x54) - static_cast<U>(crt_rand_006fc447()) % t); // 005DB439-005DB446
    }
}
// 005DB450 CalculateFixedVarianceFrequency, RET 0.
void __fastcall FixedVariance005db450(void* src, void*) {
    wr(src, 0x50, static_cast<U>(ftol(static_cast<double>(rf(src, 0x68)) * u32d(rd(src, 0x48))))); // 005DB454-005DB46E
}
// 005D7FE0 CExoSoundInternal::InterruptPlayingSound(group byte), RET 4.
void __fastcall InterruptPlayingSound005d7fe0(void* sys, void*, U group_word) {
    const unsigned char g = static_cast<unsigned char>(group_word);  // 005D8002 (BL)
    void* pos = rp(L(sys, 0x40), 0);
    void* src = pos ? rp(pos, 8) : nullptr;
    if (!pos) return;                                                // 005D7FF7-005D7FFB
    while (rb(src, 0x35) != g) {                                     // 005D8006
        src = next(L(sys, 0x40), &pos);                              // 005D800B-005D8018
        if (!pos) return;                                            // 005D801A-005D8026
    }
    if (rd(src, 0x28)) {                                             // 005D8029-005D8030
        if (sound_lower29::ActorPlaying005db0e0(src, nullptr)) {      // 005D8032-005D803D
            if (!ListContains005e9d40(L(sys, 0x48), nullptr, src)) ini_addhead::AddHead005e9d70(L(sys, 0x48), nullptr, src); // 005D803F-005D804C
        } else if (!ListContains005e9d40(L(sys, 0x48), nullptr, src)) {
            destroy_source(src);                                     // 005D8058-005D8067
        }
        return;
    }
    kotor_sound_path_required::shutdown_source_005db210(src, nullptr); // 005D8070
    if (rd(src, 0x24)) {                                             // 005D8075-005D807A
        *at(src, 0x34) = 2;                                          // 005D807C
        wp(src, 0x40, ini_addhead::AddHead005e9d70(L(sys, 0x38), nullptr, src)); // 005D8080-005D8089
    }
}
// 005D8EB0 / 005D8FF0 GetFree3DVoice / GetFree2DVoice(group byte), RET 4.
static void* free_voice(void* sys, U group_word, bool three_d) {
    const U groups = rd(sys, 0x4c);                                  // EDX (loaded once)
    unsigned char* mine = static_cast<unsigned char*>(bp(groups + static_cast<U>(static_cast<unsigned char>(group_word)) * 24u));
    if (!mine) return nullptr;                                       // 005D8EC7 / 005D9007
    const unsigned arr_off = three_d ? 0x30u : 0x2cu;
    const std::int32_t fixed_n = three_d ? 0 : static_cast<std::int32_t>(rb(sys, 0x28) >> 1); // 005D9018-005D901C (once)
    std::int32_t n = three_d ? rb(sys, 0x29) : fixed_n;
    if (n <= 0) return nullptr;                                      // 005D8EE0 / 005D9026
    unsigned char* arr = static_cast<unsigned char*>(rp(sys, arr_off)); // [esp+1C]/[esp+20]
    U best = 0; unsigned char best_rank = 0;
    for (std::int32_t i = 0; i < n; ) {
        unsigned char* e = arr + static_cast<U>(i) * 16u;
        if (rd(e, 8) == 0) {                                         // 005D8EF7-005D8EFC
            void* owner = rp(e, 0);
            if (!owner) return e;                                    // 005D8EFE-005D8F02 -> 005D8F7F
            unsigned char* other = static_cast<unsigned char*>(bp(groups + static_cast<U>(rb(owner, 0x35)) * 24u));
            if (other && other[6] > mine[6] && other[6] > best_rank) { best = static_cast<U>(i); best_rank = other[6]; } // 005D8F0E-005D8F22
        }
        ++i;
        if (three_d) n = rb(sys, 0x29);                              // 005D8F26 (reloaded each pass)
    }
    if (best_rank == 0) return nullptr;                              // 005D8F32-005D8F38
    void* victim = rp(arr + best * 16u, 0);                          // 005D8F3E-005D8F45
    if (rd(victim, 0x28)) {                                          // 005D8F48-005D8F4F
        if (sound_lower29::ActorPlaying005db0e0(victim, nullptr)) {
            if (!ListContains005e9d40(L(sys, 0x48), nullptr, victim)) ini_addhead::AddHead005e9d70(L(sys, 0x48), nullptr, victim);
        } else if (!ListContains005e9d40(L(sys, 0x48), nullptr, victim)) {
            destroy_source(victim);                                  // 005D8F9B-005D8FA1
        }
    } else {
        kotor_sound_path_required::shutdown_source_005db210(victim, nullptr); // 005D8FB2
        if (rd(victim, 0x24)) {
            *at(victim, 0x34) = 2;                                   // 005D8FBE
            wp(victim, 0x40, ini_addhead::AddHead005e9d70(L(sys, 0x38), nullptr, victim)); // 005D8FC2-005D8FCB
        }
    }
    return static_cast<unsigned char*>(rp(sys, arr_off)) + best * 16u; // 005D8FCE-005D8FD1 ([30]/[2C] reloaded)
}
void* __fastcall Free3DVoice005d8eb0(void* sys, void*, U g) { return free_voice(sys, g, true); }
void* __fastcall Free2DVoice005d8ff0(void* sys, void*, U g) { return free_voice(sys, g, false); }
}

namespace sound_voice_required {
// 005DAEB0 CExoSoundSourceInternal::DoObstructionWork, RET 0.
void __fastcall actor_obstruction_005daeb0(void* src, void*) {
    void* voice = rp(src, 0x3c);
    if (!voice) return;                                              // 005DAEB0-005DAEB5
    AIL_set_3D_sample_obstruction(rp(voice, 4), rf(src, 0x9c));      // 005DAEB7-005DAEC2
}
}

namespace kotor_sound_path_required {
// 005DB4D0 CExoSoundSourceInternal::Play(force, keep_restart), RET 8, EAX = [src+58] (the play length in ms that
// 005DB64F stores): every path joins 005DB7D3 `mov esi,[esi+58h]`, then ~CExoString(local), `mov eax,esi`.
U __fastcall source_play_005db4d0(void* src, void*, U force, U keep_restart) {
    using kotor_sound_internal::SourceDemand005dae50; using kotor_sound_internal::SourceRelease005dae90;
    kotor_reconstruction::StartupString local;                       // 005DB4F3-005DB4F7 (EH state 0, ~ at 005DB7E2)
    void* sys = rp(src, 0x44);
    wr(src, 0x2c, 0); wr(src, 0x5c, 0); wr(src, 0x64, 0);            // 005DB501-005DB507
    enum { End, Fail, Release, Loopers } exit_path = End;
    if (rd(sys, 8) == 0) exit_path = Fail;                           // 005DB50A-005DB511
    else {
        const std::int32_t loops = rd(src, 0x24) == 0 ? 1 : 0;       // 005DB517-005DB526
        if (keep_restart == 0) wr(src, 0x88, 0);                     // 005DB51E-005DB52A
        void* v3 = rp(src, 0x3c); void* v2 = rp(src, 0x38);
        if (v3 && rp(v3, 4)) {                                       // 005DB530-005DB53C
            AIL_stop_3D_sample(rp(v3, 4));                           // 005DB53E
            AIL_set_3D_sample_loop_count(rp(rp(src, 0x3c), 4), loops); // 005DB545-005DB54D
            AIL_set_3D_sample_offset(rp(rp(src, 0x3c), 4), 0);       // 005DB553-005DB55B
            AIL_resume_3D_sample(rp(rp(src, 0x3c), 4));              // 005DB561-005DB568
        } else if (v2 && rp(v2, 4)) {                                // 005DB573-005DB57F
            AIL_stop_sample(rp(v2, 4));
            AIL_set_sample_loop_count(rp(rp(src, 0x38), 4), loops);
            AIL_set_sample_position(rp(rp(src, 0x38), 4), 0);
            AIL_resume_sample(rp(rp(src, 0x38), 4));                 // 005DB582-005DB5AB
        } else {
            bool proceed = true;
            if (rd(src, 0x58) == 0 && rd(src, 0x30) == 0) {          // 005DB5B6-005DB5C2
                if (!SourceDemand005dae50(src, nullptr)) { exit_path = Fail; proceed = false; } // 005DB5C8-005DB5D1
                else {
                    if (void* res = rp(src, 8)) {                    // 005DB5D7-005DB5E8
                        (void)async_string_cstr_005e5670(&local, nullptr);
                        (void)*reinterpret_cast<volatile std::uint16_t*>(at(res, 4)); // 004064B0 (result unused)
                    }
                    void* res = rp(src, 8);
                    using GetData = void*(__fastcall*)(void*, void*);  // vtable +14h, thiscall, no stack args
                    GetData get; std::memcpy(&get, at(rp(res, 0), 0x14), 4);
                    alignas(4) unsigned char info[0x24];               // AILSOUNDINFO at [esp+1Ch]
                    std::memset(info, 0, sizeof info);
                    AIL_WAV_info(get(res, nullptr), info);            // 005DB5F2-005DB5FB (info pushed before the vcall)
                    const U rate = rd(info, 0x0c), bits = rd(info, 0x10), channels = rd(info, 0x14), length = rd(info, 0x08);
                    const double per_byte = 8.0 / u32d(bits * rate);  // 005DB601-005DB622 ([0074D2F8]=8.0)
                    const U frames = length / channels;              // 005DB61E-005DB62E (DIV)
                    wr(src, 0x58, static_cast<U>(ftol(per_byte * u32d(frames) * 1000.0))); // 005DB636-005DB64F ([0074D2F0]=1000.0)
                    if (rd(src, 0x1c) && channels != 1) {            // 005DB652-005DB65C
                        (void)SourceRelease005dae90(src, nullptr);   // 005DB660
                        if (rd(src, 0x28)) wr(src, 0x2c, 1);         // 005DB665-005DB670
                        proceed = false;
                    } else {
                        wr(src, 0x48, rate); wr(src, 0x54, rate);    // 005DB67F / 005DB688
                        if (!(rf(src, 0x74) == 0.0f)) kotor_sound_internal::PitchVariance005db3d0(src, nullptr); // 005DB67C-005DB694
                        if (!(rf(src, 0x68) == 0.0f)) kotor_sound_internal::FixedVariance005db450(src, nullptr); // 005DB699-005DB6AB
                    }
                }
            }
            if (proceed) {
                if (!source_in_range_005db050(src, nullptr)) {       // 005DB6B2-005DB6BB
                    if (rd(src, 0x30)) { wr(src, 0x30, 0); (void)kotor_resource_required::res_release_00409b80(rp(src, 8), nullptr); } // 005DB6BD-005DB6CC
                    exit_path = Loopers;
                } else {
                    const unsigned char gi = rb(src, 0x35);          // 005DB6D6
                    unsigned char* g = group(sys, gi);
                    bool fail_release = false;
                    if (g[5] >= g[4]) {                              // 005DB6E8-005DB6EE
                        if (force != 0 || rd(g, 0) == 0) fail_release = true; // 005DB6F0-005DB6F7
                        else kotor_sound_internal::InterruptPlayingSound005d7fe0(sys, nullptr, gi); // 005DB6F9-005DB704
                    }
                    void* voice = nullptr;
                    if (!fail_release) {
                        const bool three_d = rd(src, 0x1c) != 0;     // 005DB709-005DB71D
                        voice = three_d ? kotor_sound_internal::Free3DVoice005d8eb0(sys, nullptr, rb(src, 0x35))
                                        : kotor_sound_internal::Free2DVoice005d8ff0(sys, nullptr, rb(src, 0x35)); // 005DB71F / 005DB746
                        if (!voice) fail_release = true;             // 005DB726 / 005DB74D
                        else if (rd(src, 0x30) == 0 && !SourceDemand005dae50(src, nullptr)) exit_path = Fail; // 005DB72A-005DB738 / 005DB76F-005DB77D
                        else {
                            if (three_d) sound_lower29_required::start3d_005d71c0(sys, nullptr, src, voice); // 005DB73A-005DB73F
                            else sound_lower29_required::start2d_005d7480(sys, nullptr, src, voice);          // 005DB78D-005DB792
                            if (rb(src, 0x34) == 2) (void)remove_nonplaying_looping_005d79c0(sys, nullptr, src); // 005DB797-005DB7A1
                            add_playing_005d7b40(sys, nullptr, src);  // 005DB7A6-005DB7AA
                            if (rd(sys, 0x144) != 0) (void)sound_pause29_required::actor_pause_005db150(src, nullptr); // 005DB7AF-005DB7B8, 005DB7CC
                            else if (*sound_pause29_required::word_00832890 != 0 &&
                                     kotor_sound_internal::SourcePauseable005d6b70(sys, nullptr, src)) // 005DB7BA-005DB7CA
                                (void)sound_pause29_required::actor_pause_005db150(src, nullptr);
                        }
                    }
                    if (fail_release) { (void)SourceRelease005dae90(src, nullptr); exit_path = Loopers; } // 005DB751-005DB753
                }
            }
        }
    }
    if (exit_path == Loopers) {                                      // 005DB758
        if (rd(src, 0x24) == 0) exit_path = Fail;
        else if (rb(src, 0x34) == 0) add_nonplaying_looping_005d7b80(sys, nullptr, src); // 005DB75D-005DB768
    }
    if (exit_path == Fail && rd(src, 0x28)) wr(src, 0x2c, 1);        // 005DB77F-005DB784
    return rd(src, 0x58);                                            // 005DB7D3 (read before ~local), 005DB7EC
}
}
