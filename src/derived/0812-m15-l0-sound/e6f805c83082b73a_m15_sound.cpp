// lane-m15-0 unit m15-l0-sound: 3D one-shot sound path, sound-source leaves, CSoundSet / CResHelper<CResSSF,2060> /
// CResSSF family (with their method tables 00752CA0 / 00752CA4 / 00758000). Original: swkotor.exe GOG K1,
// static disassembly only. Layout facts are cited at the instruction that establishes them.
//   CExoSoundSource        +4 internal (005D5910)
//   CExoStreamingSoundSource +0 internal (005D6290)
//   CExoSoundSourceInternal +1C 3D flag, +24 looping, +2C delete-after-play, +34 state byte, +38/+3C 2D/3D voice,
//                           +44 CExoSoundInternal*, +50/+54 rates, +5C delay, +64 delay start, +68 fixed variance,
//                           +6C/+70 distances, +74 pitch variance, +78 position (005DB2C0..005DB9B7)
//   CExoSoundInternal      +34 delayed one-shot list, +4C priority-group records (24 bytes), +148 streaming VO source
//   CResHelper<CResSSF,2060> (0x1C): +0 vtbl, +4 requested, +8 CRes*, +0C CResRef (006789C1..00678AC6)
#include "f155d76cd432eed7_m15_l0.h"
#include <cctype>
#include <cstring>

extern "C" {
void* __cdecl live_operator_new(std::uint32_t);                                     // 006FA7E6
void __cdecl live_free(void*);                                                       // 006FA390
__declspec(dllimport) void __stdcall AIL_set_3D_position(void*, float, float, float);           // [0073D4B0]
__declspec(dllimport) void __stdcall AIL_set_3D_sample_playback_rate(void*, std::int32_t);      // [0073D4E8]
__declspec(dllimport) void __stdcall AIL_set_sample_playback_rate(void*, std::int32_t);         // [0073D500]
// CResRef / CRes / CExoResMan (M46 resman-object-bodies & co.)
int __fastcall c2da_equal_text_004060d0(void* ref, void*, const char*);              // CResRef::operator==(char const*)
int __fastcall c2da_equal_ref_004060b0(void* ref, void*, void* other);               // CResRef::operator==(CResRef const&)
int __fastcall c2da_not_equal_text_00406060(void* ref, void*, const char*);          // CResRef::operator!=(char const*)
char* __fastcall c2da_ref_00405fe0(void* ref, void*) noexcept;                       // CResRef::GetResRefStr
void* __fastcall c2da_default_ref_00405ed0(void* ref, void*);                        // CResRef::CResRef()
void* __fastcall unresolved_ref_assign_00406290(void* ref, void*, void* out, const char*); // CResRef::operator=(char const*) RET 8
void* __fastcall c2da_copy_00406120(void* ref, void*, void* out, const void* src);   // CResRef::operator=(CResRef const&) RET 8
void __fastcall c2da_cancel_00409b00(void* res, void*);                              // CRes::CancelRequest
void __fastcall c2da_request_00408620(void* res, void*);                             // CRes::Request
void __fastcall c2da_release_demand_00409b80(void* res, void*) noexcept(false);      // CRes::Release
int __fastcall aurora_get_demands_004064b0(void* res, void*) noexcept(false);        // CRes::GetDemands
void* __fastcall c2da_resource_base_ctor_00406460(void* self, void*);                // CRes::CRes
void __fastcall c2da_resource_base_destroy_00409d40(void* self, void*);              // CRes::~CRes
std::uint32_t __stdcall c2da_resource_constant_word_00406490();                      // CRes slot 1 leaf (M46)
std::uint32_t __stdcall c2da_resource_constant_word_0063e7f0();                      // CRes slot 2 leaf (M46)
struct KotorM15ResMan;
KotorM15ResMan* __cdecl c2da_manager_007a39e8();                                      // [007A39E8]
int __fastcall c2da_release_00409cf0(KotorM15ResMan*, void*, void* res);             // CExoResMan::ReleaseResObject
void* __fastcall c2da_lookup_004074d0(KotorM15ResMan*, void*, const void* ref, std::uint32_t type);   // GetResObject
void __fastcall c2da_register_00407680(KotorM15ResMan*, void*, const void* ref, std::uint32_t type, void* res); // SetResObject
// lists
void* __fastcall c2da_005e9d70(void* list, void*, void* item);                       // CExoLinkedListInternal::AddHead
// streaming source (lane-sound donors)
void __fastcall kotor_fastcall_005d5b80(void* source, void*);                        // CExoStreamingSoundSource::Stop
void __fastcall kotor_fastcall_005d6200(void* source, void*);                        // ~CExoStreamingSoundSource
}
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } } // [007A39E0] CExoBase*
namespace table29_required {
void* __fastcall gui_sound_construct_005d5a50(void*, void*);                          // CExoStreamingSoundSource ctor
void __fastcall gui_priority_005d5ba0(void*, void*, std::uint32_t);                   // SetPriorityGroup
void __fastcall gui_volume_005d5b50(void*, void*, std::uint32_t);                     // SetVolume
}
namespace menu_resources { struct Ref; }
namespace table29_required {
std::uint32_t __fastcall gui_play_005d5b10(void*, void*, menu_resources::Ref*, std::uint32_t, std::uint32_t,
                                           std::uint32_t, std::uint32_t);              // CExoStreamingSoundSource::Play RET 14h
}
namespace kotor_sound_rev3 {
void* __fastcall InternalConstruct005daf50(void* self, void*, void* system);          // CExoSoundSourceInternal ctor
void __fastcall InternalSetPriorityGroup005dbb20(void* self, void*, std::uint32_t);   // SetPriorityGroup
void __fastcall InternalSetPitchVariance005dba40(void* self, void*, float);           // SetPitchVariance (bound, M60)
}
namespace kotor_sound_rev3_required { void __fastcall res_helper_set_ref_005d5e90(void* helper, void*, const void* ref, std::uint32_t mode); }
namespace kotor_sound_path_required {
void __fastcall shutdown_source_005db210(void*, void*);                               // CExoSoundSourceInternal::ShutDownSource
void __fastcall source_set_volume_005db800(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall source_play_005db4d0(void*, void*, std::uint32_t, std::uint32_t); // CExoSoundSourceInternal::Play RET 8, EAX = [src+58h]
}
namespace kotor_sound_internal { void __fastcall FixedVariance005db450(void*, void*); } // CalculateFixedVarianceFrequency (bound)
namespace sound_lower29 { std::uint32_t __fastcall ActorPlaying005db0e0(void*, void*); } // IsPlaying
namespace sound_pause29_required {
std::uint32_t __fastcall actor_pause_005db150(void*, void*);
std::uint32_t __fastcall actor_resume_005db1c0(void*, void*);
}
namespace kotor_input_required { std::uint32_t __fastcall low_resolution_timer_005e9250(void* timers, void*); }

namespace {
using U = std::uint32_t;
unsigned char* at(void* p, unsigned off) { return static_cast<unsigned char*>(p) + off; }
U rd(void* p, unsigned off) { U v; std::memcpy(&v, at(p, off), 4); return v; }
void wr(void* p, unsigned off, U v) { std::memcpy(at(p, off), &v, 4); }
void* rp(void* p, unsigned off) { void* v; std::memcpy(&v, at(p, off), 4); return v; }
void wp(void* p, unsigned off, const void* v) { std::memcpy(at(p, off), &v, 4); }
float rf(void* p, unsigned off) { float v; std::memcpy(&v, at(p, off), 4); return v; }
void wf(void* p, unsigned off, float v) { std::memcpy(at(p, off), &v, 4); }
const char kEmpty[] = "";                                                    // 0073D71C
using Deleting = void* (__fastcall*)(void*, void*, U);
// MOV EAX,[obj]; PUSH flags; CALL [EAX]: slot word read FROM the table (one load of the vptr, one of the slot).
void* virtual_delete(void* object, U flags) { Deleting d; std::memcpy(&d, rp(object, 0), 4); return d(object, nullptr, flags); } // 005D78CE..005D78D4
}

// ------------------------------------------------------------------ method tables (typed constant arrays, D21)
namespace kotor_m15_sound {
struct Methods00758000 {                                                      // CResSSF (00758000..00758013, 5 cells)
    void* (__fastcall* deleting_destruct_006db6b0)(void*, void*, U);
    U (__stdcall* constant_word_00406490)();
    U (__stdcall* constant_word_0063e7f0)();
    U (__fastcall* freed_006db680)(void*, void*);
    U (__fastcall* serviced_006db690)(void*, void*);
};
static_assert(sizeof(Methods00758000) == 20, "five x86 cells");
extern const Methods00758000 method_table_00758000;
const Methods00758000 method_table_00758000 = {
    &kotor_m15_res_ssf_deleting_destruct_006db6b0, &c2da_resource_constant_word_00406490,
    &c2da_resource_constant_word_0063e7f0, &kotor_m15_res_ssf_freed_006db680, &kotor_m15_res_ssf_serviced_006db690};
// 00752CA0 {00678B00} CResHelper<CResSSF,2060>; 00752CA4 {00678EA0} CSoundSet (one cell each; 00752CA8 = "RESR...").
extern const Deleting method_table_00752ca0[1];
extern const Deleting method_table_00752ca4[1];
const Deleting method_table_00752ca0[1] = {&kotor_m15_ssf_helper_deleting_destruct_00678b00};
const Deleting method_table_00752ca4[1] = {&kotor_m15_sound_set_deleting_destruct_00678ea0};
}

extern "C" {
// ================================================================== CExoSoundSource (outer)
// 005D5910: ECX=[ECX+4]; null -> RET 4; else tail JMP 005DB320 (same stack word).
void __fastcall kotor_m15_sound_source_set_3d_005d5910(void* source, void*, U is3d) {
    void* const internal = rp(source, 4);                                     // 005D5910
    if (internal != nullptr) kotor_m15_source_internal_set_3d_005db320(internal, nullptr, is3d); // 005D5913..005D5917
}
// 005D59D0: ECX=[ECX+4]; null -> RET 4; else tail JMP 005DB130.
void __fastcall kotor_m15_sound_source_set_looping_005d59d0(void* source, void*, U looping) {
    void* const internal = rp(source, 4);                                     // 005D59D0
    if (internal != nullptr) kotor_m15_source_internal_set_looping_005db130(internal, nullptr, looping); // 005D59D3..005D59D7
}
// 005D59E0 SetPosition(const Vector*, float z_offset), RET 8: local {x, y, (float)(z + offset)} -> 005DB2C0.
void __fastcall kotor_m15_sound_source_set_position_005d59e0(void* source, void*, const void* position, float z_offset) {
    void* const internal = rp(source, 4);                                     // 005D59E0
    if (internal == nullptr) return;                                          // 005D59E6..005D59E8
    float local[3];                                                           // [esp] 12 bytes
    std::memcpy(local, position, 12);                                         // 005D59EA..005D59FA
    local[2] = static_cast<float>(static_cast<double>(local[2]) + static_cast<double>(z_offset)); // 005D59FE..005D5A0F (FLD/FADD/FSTP dword)
    kotor_m15_source_internal_set_position_005db2c0(internal, nullptr, local); // 005D5A13
}
// 005D5A20 Stop: ECX=[ECX+4]; null -> RET; else tail JMP 005DB2B0.
void __fastcall kotor_m15_sound_source_stop_005d5a20(void* source, void*) {
    void* const internal = rp(source, 4);                                     // 005D5A20
    if (internal != nullptr) kotor_m15_source_internal_shut_down_005db2b0(internal, nullptr); // 005D5A23..005D5A27
}

// 005D5E10 CExoSound::Play3DOneShotSound, RET 28h: ECX=[ECX] (CExoSoundInternal*); null -> EAX=0.
U __fastcall kotor_m15_sound_play_3d_one_shot_005d5e10(void* sound, void*, void* ref, KotorM15VectorWords position,
                                                       float z_offset, U group, U delay, U volume, float fixed_variance,
                                                       float pitch_variance) noexcept(false) {
    void* const internal = rp(sound, 0);                                      // 005D5E10
    if (internal == nullptr) return 0;                                        // 005D5E12..005D5E14 / 005D5E61
    KotorM15VectorWords moved = position;                                     // 005D5E3F..005D5E51 (3 words copied)
    moved.z = static_cast<float>(static_cast<double>(position.z) + static_cast<double>(z_offset)); // 005D5E1A..005D5E2C
    return kotor_m15_sound_internal_play_3d_one_shot_005d7680(internal, nullptr, ref, moved, group, delay, volume,
                                                              fixed_variance, pitch_variance); // 005D5E54..005D5E59
}

// ================================================================== CExoStreamingSoundSource leaves
void __fastcall kotor_m15_stream_source_set_3d_005d6290(void* source, void*, U is3d) {
    void* const internal = rp(source, 0);                                     // 005D6290
    if (internal != nullptr) wr(internal, 0x58, is3d);                        // 005D6292..005D629A
}
void __fastcall kotor_m15_stream_source_set_distance_005d62a0(void* source, void*, float min_d, float max_d) {
    void* const internal = rp(source, 0);                                     // 005D62A0
    if (internal == nullptr) return;                                          // 005D62A2..005D62A4
    wf(internal, 0x48, min_d);                                                // 005D62A6 / 005D62AE (raw dword copy)
    wf(internal, 0x4C, max_d);                                                // 005D62AA / 005D62B1
}
void __fastcall kotor_m15_stream_source_set_position_005d62c0(void* source, void*, const void* position) {
    void* const internal = rp(source, 0);                                     // 005D62C1
    if (internal != nullptr) std::memcpy(at(internal, 0x5C), position, 12);   // 005D62C7..005D62DB
}

// ================================================================== CExoSoundInternal
// 005D7680 Play3DOneShotSound(CResRef* ref, Vector pos, group, delay, volume, fixed, pitch), RET 24h.
U __fastcall kotor_m15_sound_internal_play_3d_one_shot_005d7680(void* internal, void*, void* ref, KotorM15VectorWords position,
                                                                U group, U delay, U volume, float fixed_variance,
                                                                float pitch_variance) noexcept(false) {
    U result = 0;                                                             // [esp+10h] = EBX = 0 (005D769F..005D76A8)
    if (c2da_equal_text_004060d0(ref, nullptr, kEmpty) != 0) return 0;       // 005D76A1..005D76C8
    const char* const name = c2da_ref_00405fe0(ref, nullptr);                // 005D76CC..005D76CE
    // 006FD268 (statically linked CRT toupper(int)); the modern CRT toupper is used (C locale: identical mapping).
    const int first = std::toupper(static_cast<int>(static_cast<signed char>(name[0]))); // 005D76D5..005D76DE (MOVSX)
    const unsigned char low = static_cast<unsigned char>(first);              // compared as AL (005D76E1..005D76E9)
    if ((low == 'C' || low == 'P' || low == 'N') && name[1] == '_') {         // 005D76E1..005D76F5
        // ---- voice-over: one streaming source at internal+148h
        if (void* old = rp(internal, 0x148)) {                                // 005D76FB..005D7703
            kotor_fastcall_005d5b80(old, nullptr);                            // 005D7705 Stop
            if (void* again = rp(internal, 0x148)) {                          // 005D770A..005D7712
                kotor_fastcall_005d6200(again, nullptr);                      // 005D7716 ~CExoStreamingSoundSource
                live_free(again);                                             // 005D771B..005D771C
            }
        }
        void* const raw = live_operator_new(4);                               // 005D7724..005D7726
        void* const stream = raw != nullptr ? table29_required::gui_sound_construct_005d5a50(raw, nullptr) : nullptr; // 005D7732..005D7743
        wp(internal, 0x148, stream);                                          // 005D774F
        if (stream == nullptr) return result;                                 // 005D7755 -> 005D77D3 (0)
        table29_required::gui_priority_005d5ba0(stream, nullptr, group);      // 005D7757..005D775E
        kotor_m15_stream_source_set_3d_005d6290(rp(internal, 0x148), nullptr, 1); // 005D7763..005D776B
        kotor_m15_stream_source_set_position_005d62c0(rp(internal, 0x148), nullptr, &position); // 005D7770..005D777B
        if (static_cast<unsigned char>(volume) != 0)                          // 005D7780..005D7786 (TEST AL,AL)
            table29_required::gui_volume_005d5b50(rp(internal, 0x148), nullptr, volume); // 005D7788..005D778F (whole word)
        void* const record = at(rp(internal, 0x4C), (group & 0xFFu) * 24u);   // 005D779A..005D77A3 (MOVZX EAX,BL)
        kotor_m15_stream_source_set_distance_005d62a0(rp(internal, 0x148), nullptr, rf(record, 0x0C), rf(record, 8)); // 005D77A6..005D77B6
        result = table29_required::gui_play_005d5b10(rp(internal, 0x148), nullptr, static_cast<menu_resources::Ref*>(ref),
                                                     4, 0, 0, 0);             // 005D77BB..005D77CF
        return result;                                                        // 005D77D3..005D77D7
    }
    // ---- one-shot CExoSoundSourceInternal (0xA0)
    void* const raw = live_operator_new(0xA0);                                // 005D77DC..005D77E1
    void* const source = raw != nullptr ? kotor_sound_rev3::InternalConstruct005daf50(raw, nullptr, internal) : nullptr; // 005D77ED..005D7806
    kotor_sound_rev3_required::res_helper_set_ref_005d5e90(source, nullptr, ref, 0); // 005D7808..005D7815 (no null check)
    wr(source, 0x28, 1);                                                      // 005D781D
    kotor_m15_source_internal_set_3d_005db320(source, nullptr, 1);            // 005D781A / 005D7820
    kotor_sound_rev3::InternalSetPriorityGroup005dbb20(source, nullptr, group); // 005D7825..005D782C
    kotor_m15_source_internal_set_position_005db2c0(source, nullptr, &position); // 005D7831..005D7838
    if (static_cast<unsigned char>(volume) != 0)                              // 005D783D..005D7843
        kotor_sound_path_required::source_set_volume_005db800(source, nullptr, volume, 0, 0x3F800000u); // 005D7845..005D784F
    if (!(fixed_variance == 0.0f))                                            // 005D7854..005D7863 (FCOMP 0073D700, TEST AH,44h / JNP)
        (void)kotor_m15_source_internal_set_fixed_variance_005db980(source, nullptr, fixed_variance); // 005D7865..005D786C
    if (!(pitch_variance == 0.0f))                                            // 005D7871..005D7880
        kotor_sound_rev3::InternalSetPitchVariance005dba40(source, nullptr, pitch_variance); // 005D7882..005D7889
    void* const record = at(rp(internal, 0x4C), (group & 0xFFu) * 24u);       // 005D788E..005D7897
    wf(source, 0x70, rf(record, 0x0C));                                       // 005D789F / 005D78A8
    wf(source, 0x6C, rf(record, 8));                                          // 005D789C / 005D78AB
    if (delay != 0) {                                                         // 005D78A2..005D78AE
        kotor_m15_source_internal_set_one_shot_delay_005db340(source, nullptr, delay); // 005D78B0..005D78B1
        return result;                                                        // 005D78B6 EDI = [esp+10h] = 0
    }
    result = kotor_sound_path_required::source_play_005db4d0(source, nullptr, 0, 0); // 005D78BC..005D78C5 mov edi,eax
    if (rd(source, 0x2C) != 0) virtual_delete(source, 1);                     // 005D78C7..005D78D4
    return result;                                                            // 005D78D6
}

// 005D7BB0 AddToDelayedOneShotList(source), RET 4: source+34h = 3; source+40h = AddHead(this+34h list, source).
void __fastcall kotor_m15_sound_internal_add_delayed_one_shot_005d7bb0(void* internal, void*, void* source) {
    *at(source, 0x34) = 3;                                                    // 005D7BB5
    wp(source, 0x40, c2da_005e9d70(rp(internal, 0x34), nullptr, source));     // 005D7BB9..005D7BC2
}

// ================================================================== CExoSoundSourceInternal
// 005DB130 (no symbol; tail target of 005D59D7) SetLooping: if state byte +34h == 0, +24h = value. RET 4.
void __fastcall kotor_m15_source_internal_set_looping_005db130(void* self, void*, U looping) {
    if (*at(self, 0x34) == 0) wr(self, 0x24, looping);                        // 005DB130..005DB13B
}
// 005DB2B0 ShutDownSource thunk: JMP 005DB210.
void __fastcall kotor_m15_source_internal_shut_down_005db2b0(void* self, void*) {
    kotor_sound_path_required::shutdown_source_005db210(self, nullptr);       // 005DB2B0
}
// 005DB2C0 SetPosition(const Vector*), RET 4.
void __fastcall kotor_m15_source_internal_set_position_005db2c0(void* self, void*, const void* position) {
    std::memcpy(at(self, 0x78), position, 12);                                // 005DB2C4..005DB2DA
    if (rd(self, 0x1C) == 0) return;                                          // 005DB2DD..005DB2E2
    if (sound_lower29::ActorPlaying005db0e0(self, nullptr) == 0) return;      // 005DB2E4..005DB2ED
    float v[3]; std::memcpy(v, position, 12);
    const float z = static_cast<float>(static_cast<double>(v[2]) * -1.0);     // 005DB2EF..005DB301 (FMUL [00740CBC] = -1.0f; FSTP dword)
    AIL_set_3D_position(rp(rp(self, 0x3C), 4), v[0], v[1], z);                // 005DB2F2..005DB30A
}
// 005DB320 Set3D, RET 4: only when state byte != 1 and no 2D/3D voice is attached.
void __fastcall kotor_m15_source_internal_set_3d_005db320(void* self, void*, U is3d) {
    if (*at(self, 0x34) == 1) return;                                         // 005DB320..005DB324
    if (rd(self, 0x3C) != 0) return;                                          // 005DB326..005DB32B
    if (rd(self, 0x38) != 0) return;                                          // 005DB32D..005DB332
    wr(self, 0x1C, is3d);                                                     // 005DB334..005DB338
}
// 005DB340 SetOneShotDelay(delay), RET 4.
void __fastcall kotor_m15_source_internal_set_one_shot_delay_005db340(void* self, void*, U delay) {
    void* const system = rp(self, 0x44);                                      // 005DB343
    if (rd(system, 8) == 0) return;                                           // 005DB346..005DB34B
    wr(self, 0x5C, delay);                                                    // 005DB34D..005DB351
    wr(self, 0x64, kotor_input_required::low_resolution_timer_005e9250(
                       rp(kotor_menu_10_19::dependencies::base_007a39e0, 4), nullptr)); // 005DB354..005DB366
    kotor_m15_sound_internal_add_delayed_one_shot_005d7bb0(rp(self, 0x44), nullptr, self); // 005DB362..005DB369
}
// 005DB980 SetFixedVariance(float), RET 4 -> 0 when outside [0.5, 2.0] (or NaN), else 1.
U __fastcall kotor_m15_source_internal_set_fixed_variance_005db980(void* self, void*, float variance) {
    if (!(variance <= 2.0f)) return 0;                                        // 005DB980..005DB992 (FCOMP [00741BE0]; TEST AH,41h; JP)
    if (variance < 0.5f) return 0;                                            // 005DB998..005DB9A7 (FCOMP [0073E9AC]; TEST AH,1)
    wf(self, 0x68, variance);                                                 // 005DB9AD..005DB9B4
    wr(self, 0x74, 0);                                                        // 005DB9B7
    if (rd(rp(self, 0x44), 8) == 0) return 1;                                 // 005DB9B1..005DB9C3 -> 005DBA2F
    (void)sound_pause29_required::actor_pause_005db150(self, nullptr);        // 005DB9C5..005DB9C7
    void* const v3 = rp(self, 0x3C);                                          // 005DB9CC
    if (v3 != nullptr && rp(v3, 4) != nullptr) {                              // 005DB9CF..005DB9D8
        kotor_sound_internal::FixedVariance005db450(self, nullptr);           // 005DB9DA..005DB9DC
        AIL_set_3D_sample_playback_rate(rp(rp(self, 0x3C), 4), static_cast<std::int32_t>(rd(self, 0x50))); // 005DB9E1..005DB9EC
        (void)sound_pause29_required::actor_resume_005db1c0(self, nullptr);   // 005DB9F2..005DB9F4
        return 1;                                                             // 005DB9F9
    }
    void* const v2 = rp(self, 0x38);                                          // 005DBA02
    if (v2 != nullptr && rp(v2, 4) != nullptr) {                              // 005DBA05..005DBA0E
        kotor_sound_internal::FixedVariance005db450(self, nullptr);           // 005DBA10..005DBA12
        AIL_set_sample_playback_rate(rp(rp(self, 0x38), 4), static_cast<std::int32_t>(rd(self, 0x50))); // 005DBA17..005DBA22
    }
    (void)sound_pause29_required::actor_resume_005db1c0(self, nullptr);       // 005DBA28..005DBA2A
    return 1;                                                                 // 005DBA2F
}

// ================================================================== sound sets
// 006788D0 CSoundSetList::GetSoundSet(index) -> CResRef by value (hidden out = first stack word), RET 8, EAX=out.
// List: +0 WORD count, +4 CResRef[count] (16 bytes each).
void* __fastcall kotor_m15_sound_set_list_get_006788d0(void* list, void*, void* out_ref, U index) {
    const std::uint16_t i = static_cast<std::uint16_t>(index);                // 006788D0 MOV AX,[ESP+8]
    std::uint16_t count; std::memcpy(&count, list, 2);
    if (i < count) {                                                          // 006788D8..006788DB (JB)
        std::memcpy(out_ref, at(rp(list, 4), static_cast<unsigned>(i) << 4), 16); // 0067890D..00678934
        return out_ref;                                                       // 0067891D EAX = out
    }
    alignas(4) unsigned char local[16];                                       // [esp] CResRef
    (void)c2da_default_ref_00405ed0(local, nullptr);                          // 006788DD..006788E1
    std::memcpy(out_ref, local, 16);                                          // 006788E6..00678904
    return out_ref;                                                           // 006788E6 EAX = out
}

// 006789A0 CResHelper<CResSSF,2060>::SetResRef(const CResRef* ref, int request), RET 8.
void __fastcall kotor_m15_ssf_helper_set_ref_006789a0(void* helper, void*, const void* ref, U request) noexcept(false) {
    void* const ref_mut = const_cast<void*>(ref);
    void* const current = at(helper, 0x0C);                                   // 006789C1
    if (c2da_equal_ref_004060b0(ref_mut, nullptr, current) != 0) return;      // 006789C4..006789D2
    if (rd(helper, 8) != 0 && c2da_not_equal_text_00406060(current, nullptr, kEmpty) != 0) { // 006789D8..006789ED
        if (rd(helper, 4) != 0) c2da_cancel_00409b00(rp(helper, 8), nullptr); // 006789EF..006789F9
        if (c2da_release_00409cf0(c2da_manager_007a39e8(), nullptr, rp(helper, 8)) == 0) { // 006789FE..00678A0F
            if (void* res = rp(helper, 8)) virtual_delete(res, 1);            // 00678A11..00678A1C
            wr(helper, 8, 0);                                                 // 00678A1E
        }
    }
    alignas(4) unsigned char discarded[16];                                   // [esp+10h] returned-by-value temporary
    if (c2da_equal_text_004060d0(ref_mut, nullptr, kEmpty) != 0) {            // 00678A25..00678A33
        wr(helper, 8, 0);                                                     // 00678A41
        (void)unresolved_ref_assign_00406290(current, nullptr, discarded, kEmpty); // 00678A35..00678A48
        return;
    }
    void* res = c2da_lookup_004074d0(c2da_manager_007a39e8(), nullptr, ref, 0x80C); // 00678A61..00678A6D
    if (res == nullptr) {                                                     // 00678A74..00678A76
        void* const raw = live_operator_new(0x2C);                            // 00678A78..00678A7A
        res = raw != nullptr ? kotor_m15_res_ssf_construct_006db650(raw, nullptr) : nullptr; // 00678A86..00678A99
        c2da_register_00407680(c2da_manager_007a39e8(), nullptr, ref, 0x80C, res); // 00678A9B..00678AB0
        if (res == nullptr) return;                                           // 00678AB5..00678AB7
    }
    wp(helper, 8, res);                                                       // 00678ABF
    if (request != 0) {                                                       // 00678AB9..00678AC2
        wr(helper, 4, 1);                                                     // 00678AC6
        c2da_request_00408620(res, nullptr);                                  // 00678AC4..00678ACD
    }
    (void)c2da_copy_00406120(current, nullptr, discarded, ref);               // 00678AD2..00678ADC
}

// 00678940 CResHelper<CResSSF,2060>::~CResHelper, RET 0.
void __fastcall kotor_m15_ssf_helper_destruct_00678940(void* helper, void*) {
    wp(helper, 0, &kotor_m15_sound::method_table_00752ca0);                   // 00678948
    if (rd(helper, 8) == 0) return;                                           // 00678943..0067894E
    if (c2da_not_equal_text_00406060(at(helper, 0x0C), nullptr, kEmpty) == 0) return; // 00678950..0067895F
    if (rd(helper, 4) != 0) {                                                 // 00678961..00678966
        c2da_cancel_00409b00(rp(helper, 8), nullptr);                         // 00678968..0067896B
        wr(helper, 4, 0);                                                     // 00678970
    }
    if (c2da_release_00409cf0(c2da_manager_007a39e8(), nullptr, rp(helper, 8)) != 0) return; // 00678977..00678988
    if (void* res = rp(helper, 8)) virtual_delete(res, 1);                    // 0067898A..00678995
    wr(helper, 8, 0);                                                         // 00678997
}
// 00678B00 scalar deleting dtor (slot 0 of 00752CA0), RET 4, EAX=this.
void* __fastcall kotor_m15_ssf_helper_deleting_destruct_00678b00(void* helper, void*, U flags) {
    kotor_m15_ssf_helper_destruct_00678940(helper, nullptr);                  // 00678B03
    if (flags & 1) live_free(helper);                                         // 00678B08..00678B10
    return helper;                                                            // 00678B18
}
// 00678B20 CSoundSet::CSoundSet(const CResRef*), RET 4, EAX=this. EH 0072DFD8 state 0 = helper base.
void* __fastcall kotor_m15_sound_set_construct_00678b20(void* self, void*, const void* ref) noexcept(false) {
    wp(self, 0, &kotor_m15_sound::method_table_00752ca0);                     // 00678B45
    (void)c2da_default_ref_00405ed0(at(self, 0x0C), nullptr);                 // 00678B3C..00678B4B
    wr(self, 8, 0);                                                           // 00678B5C
    alignas(4) unsigned char discarded[16];
    (void)unresolved_ref_assign_00406290(at(self, 0x0C), nullptr, discarded, kEmpty); // 00678B50..00678B63
    wr(self, 4, 0);                                                           // 00678B68
    wp(self, 0, &kotor_m15_sound::method_table_00752ca4);                     // 00678B80
    try {
        kotor_m15_ssf_helper_set_ref_006789a0(self, nullptr, ref, 0);         // 00678B6F..00678B86
    } catch (...) {
        kotor_m15_ssf_helper_destruct_00678940(self, nullptr);                // unwind state 0 (0072DFD8)
        throw;
    }
    return self;                                                              // 00678B90
}
// 00678BA0 CSoundSet::~CSoundSet, RET 0.
void __fastcall kotor_m15_sound_set_destruct_00678ba0(void* self, void*) noexcept(false) {
    wp(self, 0, &kotor_m15_sound::method_table_00752ca4);                     // 00678BBD
    try {                                                                     // EH 0072DFF8 state 0 (00678BC8): unwind = helper dtor
        if (void* res = rp(self, 8)) {                                        // 00678BC3..00678BD0
            if (aurora_get_demands_004064b0(res, nullptr) != 0)               // 00678BD2..00678BD9
                c2da_release_demand_00409b80(rp(self, 8), nullptr);           // 00678BDB..00678BDE
        }
    } catch (...) {
        kotor_m15_ssf_helper_destruct_00678940(self, nullptr);
        throw;
    }
    kotor_m15_ssf_helper_destruct_00678940(self, nullptr);                    // 00678BE3..00678BED
}
// 00678EA0 scalar deleting dtor (slot 0 of 00752CA4), RET 4, EAX=this.
void* __fastcall kotor_m15_sound_set_deleting_destruct_00678ea0(void* self, void*, U flags) noexcept(false) {
    kotor_m15_sound_set_destruct_00678ba0(self, nullptr);                     // 00678EA3
    if (flags & 1) live_free(self);                                           // 00678EA8..00678EB0
    return self;                                                              // 00678EB8
}

// ================================================================== CResSSF (0x2C)
// 006DB650 CResSSF::CResSSF, RET 0, EAX=this.
void* __fastcall kotor_m15_res_ssf_construct_006db650(void* self, void*) noexcept(false) {
    (void)c2da_resource_base_ctor_00406460(self, nullptr);                    // 006DB653
    wp(self, 0, &kotor_m15_sound::method_table_00758000);                     // 006DB658
    wr(self, 0x28, 0);                                                        // 006DB65E
    return self;                                                              // 006DB665
}
// 006DB670 CResSSF::~CResSSF: vtable 00758000, tail JMP CRes::~CRes 00409D40.
void __fastcall kotor_m15_res_ssf_destruct_006db670(void* self, void*) {
    wp(self, 0, &kotor_m15_sound::method_table_00758000);                     // 006DB670
    c2da_resource_base_destroy_00409d40(self, nullptr);                       // 006DB676
}
// 006DB680 OnResourceFreed (slot 3): +28h = 0; EAX = 1.
U __fastcall kotor_m15_res_ssf_freed_006db680(void* self, void*) {
    wr(self, 0x28, 0);                                                        // 006DB680
    return 1;                                                                 // 006DB687
}
// 006DB690 OnResourceServiced (slot 4): no data (+10h) -> 0; else +28h = 1 if it was 0; EAX = 1.
U __fastcall kotor_m15_res_ssf_serviced_006db690(void* self, void*) {
    if (rd(self, 0x10) == 0) return 0;                                        // 006DB690..006DB699
    if (rd(self, 0x28) == 0) wr(self, 0x28, 1);                               // 006DB69A..006DB6A6
    return 1;                                                                 // 006DB69F
}
// 006DB6B0 scalar deleting dtor (slot 0 of 00758000), RET 4, EAX=this.
void* __fastcall kotor_m15_res_ssf_deleting_destruct_006db6b0(void* self, void*, U flags) {
    kotor_m15_res_ssf_destruct_006db670(self, nullptr);                       // 006DB6B3
    if (flags & 1) live_free(self);                                           // 006DB6B8..006DB6C0
    return self;                                                              // 006DB6C8
}
}
