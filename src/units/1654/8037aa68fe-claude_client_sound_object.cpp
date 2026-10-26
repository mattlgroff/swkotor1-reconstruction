// Lane W4 successor (client-sound-object): the client placed-sound object CSWCSoundObject (0x16C bytes, a
// CSWCObject of type 10h) and its method table 007522A8, transcribed from swkotor.exe (read-only static
// disassembly; nothing original executed). Each statement cites its original instruction range.
//   0066B200 CSWCSoundObject::CSWCSoundObject              0066B370 ~CSWCSoundObject   0066BAB0 scalar deleting dtor
//   0066B420 LoadFromMessage(CSWCMessage*)                 0066B8A0 AddToArea(CSWCArea*, CGameObjectArray*)
//   0066AE90 UpdateTimeOfDay  0066B130 Stop  0066B190 FadeAndStop(int)  0066B050 Play
//   0066BBD0 AIUpdate (+74h)  0066BAD0 GetNextSound  0066AF30 GetTimeDiff  0066AFA0 UpdateVolumeVariation
//   0066B000 UpdateIntervalVariation  0066B980 ShuffleList
// plus the sound/array leaves they reach that have no callable rebuilt provider:
//   005D5920 CExoSoundSource::IsHardwarePlayingSoundPleaseDontUseThis  005D61C0 CExoSoundSource::SetResRef
//   005D5980 CExoSoundSource::SetPitchVariance  005D61A0 CExoSoundSource::SetDistance
//   00648100 CExoArrayList<CResRef>::DelIndex
// 005D5930 CExoSoundSource::Play is called through its single owner (w4-creature_ai), which returns the play length.
//
// Field map (this): +000 vptr, +004 id (CGameObject), CSWCObject base to +0FC;
//   +0FC CExoSoundSource* (positional/non-stream) +100 CExoStreamingSoundSource* +104 active +108 volume
//   +10C volume variation +110 times (0 = hour mask, 1 = day, 2 = night) +114 audible now +118 position z
//   +11C pitch variation +120 hour mask +124 fixed variance +128 restart pending +12C priority byte
//   +130 continuous +134/+138 floats +13C CExoArrayList<CResRef>* sounds +140 current index +144 interval
//   +148 interval variation +14C time to next +150 time left in current +154 random +158 random position
//   +15C restart list +160/+164 world time (day, time of day) +168 uses stream source.
#include "d746fcdd94-w4_providers.hpp"

using namespace w4;

namespace menu_resources { struct Ref { unsigned char bytes[16]; }; }   // CResRef (16 bytes, trivial)
static_assert(sizeof(menu_resources::Ref) == 16, "CResRef");
using Ref = menu_resources::Ref;

// ------------------------------------------------------------------------------------------- providers
namespace kotor_reconstruction { struct CExoRand { std::uint32_t Rand(); }; }                // 005EA490 (0148-rand-state)
namespace resource_ref_assignment {                                                            // 00406120 CResRef::operator=(const&)
Ref* __fastcall assign_00406120(Ref* self, void*, Ref* out, const Ref* source);
}
namespace kotor_item_drop_sounds { void* __fastcall ref_default_element_00405ed0(void* ref, void*); }   // 00405ED0 CResRef()
namespace menu_tables_required {
void* __fastcall StreamConstruct_005d5a50(void* self, void*);                                  // CExoStreamingSoundSource()
void __fastcall StreamSetLooping_005d5b40(void* self, void*, U32 looping);                     // SetLooping RET 4
void __fastcall StreamSetPriority_005d5ba0(void* self, void*, U32 group);                      // SetPriorityGroup RET 4
U32 __fastcall StreamIsPlaying_005d5af0(void* self, void*);                                    // IsPlaying
U32 __fastcall StreamPlay_005d5b10(void* self, void*, Ref* ref, U32, U32, U32, U32);            // Play RET 14h
}
namespace table29_required { void __fastcall gui_volume_005d5b50(void* self, void*, U32 volume); }  // stream SetVolume
namespace sound_fade29 { void __fastcall Begin005d5b90(void* self, void*, U32 time); }          // stream FadeAndStop RET 4
namespace kotor_sound_rev3 {
void __fastcall SourceSetPriorityGroup005d5900(void* self, void*, U32 group);                  // 005D5900 RET 4
void __fastcall InternalSetPitchVariance005dba40(void* self, void*, float variance);          // 005DBA40 RET 4
}
namespace kotor_sound_rev3_required {                                                          // 005D5E90 CResHelper::SetResRef RET 8
void __fastcall res_helper_set_ref_005d5e90(void* helper, void*, const void* ref, U32 mode);
}
namespace sound_lower29 { U32 __fastcall ActorPlaying005db0e0(void* internal, void*); }        // 005DB0E0 internal IsPlaying
namespace kotor_sound_internal { void __fastcall Render005d9590(void* sound, void*); }         // 005D9590 CExoSoundInternal::Render
extern "C" U32 __fastcall kotor_fastcall_005d5930(void* source, void*) noexcept(false);      // 005D5930 CExoSoundSource::Play RET 0

extern "C" {
extern const unsigned char kotor_m15e_object_methods_007516c8[];                               // CSWCObject table (82 slots)
void* __fastcall kotor_m15a_object_construct_0063e530(void* self, void*, U32 object_type) noexcept(false);  // RET 4
void __fastcall kotor_m15e_object_destruct_0063e800(void* self, void*) noexcept(false);
// method-table slot bodies (+06C / +078 / +07C / +080)
void* __fastcall kotor_m15e_slot00c_self_00641db0(void* self, void*);                         // 00641DB0 return this
U32 __fastcall mp_slot30_ai_zero_0066b360(void*, void*, void*, void*, U32);                     // 0066B360 xor eax,eax; ret 0Ch
U32 __fastcall slot_void_return_4_0060e760_a1(void*, void*, U32);                               // 0060E760 ret 4
void __fastcall kotor_m15e_slot090_void0_004015a0(void*, void*);                                // 004015A0 ret
void __fastcall kotor_sound_object_set_position_0066b310(void* self, void*, const void* position) noexcept(false);  // +08C
// message readers (CSWCMessage / CSWMessage)
U32 __fastcall kotor_fastcall_00692630(void* message, void*);                                   // ReadOBJECTIDClient
U32 __fastcall kotor_fastcall_004d66f0(void* message, void*);                                   // ReadBOOL
unsigned char __fastcall kotor_fastcall_004d6720(void* message, void*, U32 bits);              // ReadBYTE
unsigned short __fastcall kotor_fastcall_004d6750(void* message, void*, U32 bits);             // ReadWORD
U32 __fastcall kotor_fastcall_004d6780(void* message, void*, U32 bits);                         // ReadDWORD
float __fastcall kotor_fastcall_004d67b0(void* message, void*, U32 scale_bits, U32 bits);      // ReadFLOAT
void* __fastcall kotor_fastcall_004d6180(void* message, void*, void* result, U32 size);        // ReadCResRef (by value)
// sound sources
void* __fastcall kotor_sound_source_construct_005d5870(void* self, void*) noexcept(false);    // CExoSoundSource()
void __fastcall kotor_fastcall_005d6200(void* self, void*);                                     // ~CExoStreamingSoundSource
void __fastcall kotor_m15_sound_source_set_3d_005d5910(void* source, void*, U32 is3d);
void __fastcall kotor_m15_stream_source_set_3d_005d6290(void* source, void*, U32 is3d);
void __fastcall kotor_m15_sound_source_set_looping_005d59d0(void* source, void*, U32 looping);
void __fastcall kotor_m15_stream_source_set_distance_005d62a0(void* source, void*, float a, float b);
U32 __fastcall kotor_m15_sound_source_get_looping_005d6190(void* source, void*);
U32 __fastcall kotor_m15_stream_source_get_looping_005d6280(void* source, void*);
void __fastcall kotor_m15_sound_source_stop_005d5a20(void* source, void*);
void __fastcall kotor_fastcall_005d5b80(void* source, void*);                                   // stream Stop
void __fastcall kotor_fastcall_005d5950(void* source, void*, U32 volume);                       // SetVolume RET 4
void __fastcall kotor_fastcall_005d5a30(void* source, void*, float variance);                   // SetFixedVariance RET 4
void __fastcall kotor_m15_source_internal_shut_down_005db2b0(void* internal, void*);           // 005DB2B0
// object / area / client
void __fastcall recovered_client_sound_object_set_sound_id_0066ae30(void* self, void*, U32 active);   // 0066AE30 RET 4
void* __fastcall kotor_m15e_client_get_client_ai_master_005ed6b0(void* app, void*);
void __fastcall recovered_bridge_dword_array_add_unique_006449d0(void* list, void*, U32 value);
std::uint8_t __fastcall recovered_object_array_add_external_004d7ff0(void* array, void*, U32* id, void* object, U32) noexcept(false);
U32 __fastcall recovered_client_ai_master_add_object_00604a20(void* master, void*, void* object) noexcept(false);
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, void* day, void* time);        // RET 8
U32 __fastcall recovered_world_time_difference_004ae460(void* timer, void*, U32 day1, U32 time1, U32 day2, U32 time2,
                                                        U32* out_day, U32* out_time);           // RET 18h
// CResRef / arrays / CRT
Ref* __fastcall unresolved_ref_assign_00406290(Ref* self, void*, Ref* out, const char* text);   // operator=(const char*)
int __fastcall c2da_equal_text_004060d0(Ref* self, void*, const char* text);                    // operator==(const char*)
void __fastcall recovered_ref_array_grow_00506e20(void* array, void*, U32 size);                // CExoArrayList<CResRef>::Allocate
int __cdecl crt_rand_006fc447();                                                                 // CRT rand

// ------------------------------------------------------------------------------------------- exports
void* __fastcall recovered_client_sound_object_construct_0066b200(void* self, void*) noexcept(false);
void __fastcall recovered_client_sound_object_destruct_0066b370(void* self, void*) noexcept(false);
void* __fastcall recovered_client_sound_object_scalar_deleting_destructor_0066bab0(void* self, void*, U32 flags) noexcept(false);
void __fastcall recovered_client_sound_object_load_from_message_0066b420(void* self, void*, void* message) noexcept(false);
U32 __fastcall recovered_client_sound_object_add_to_area_0066b8a0(void* self, void*, void* area, void* objects) noexcept(false);
void __fastcall recovered_client_sound_object_update_time_of_day_0066ae90(void* self, void*);
void __fastcall recovered_client_sound_object_stop_0066b130(void* self, void*) noexcept(false);
void __fastcall recovered_client_sound_object_fade_and_stop_0066b190(void* self, void*, U32 time) noexcept(false);
void __fastcall recovered_client_sound_object_play_0066b050(void* self, void*) noexcept(false);
void __fastcall recovered_client_sound_object_ai_update_0066bbd0(void* self, void*) noexcept(false);
Ref* __fastcall recovered_client_sound_object_get_next_sound_0066bad0(void* self, void*, Ref* out) noexcept(false);
U32 __fastcall recovered_client_sound_object_get_time_diff_0066af30(void* self, void*) noexcept(false);
void __fastcall recovered_client_sound_object_update_volume_variation_0066afa0(void* self, void*) noexcept(false);
void __fastcall recovered_client_sound_object_update_interval_variation_0066b000(void* self, void*) noexcept(false);
void __fastcall recovered_client_sound_object_shuffle_list_0066b980(void* self, void*) noexcept(false);
U32 __fastcall kotor_fastcall_005d5920(void* source, void*);
void __fastcall kotor_fastcall_005d61c0(void* source, void*, const void* ref, U32 mode) noexcept(false);
void __fastcall kotor_fastcall_005d5980(void* source, void*, float variance);
void __fastcall kotor_fastcall_005d61a0(void* source, void*, U32 a_bits, U32 b_bits);
void __fastcall kotor_fastcall_00648100(void* list, void*, int index) noexcept(false);
}

namespace {
constexpr U32 kSoundObjectType_0074663e = 0x10;     // .rdata 0074663E (byte 10h) CSWCObject type
constexpr std::uint8_t kFalse_0074666c = 0;         // .rdata 0074666C (byte 00h)
const char kZero_00741e18[] = "0";                  // .rdata 00741E18 ("0", 2 bytes)
constexpr float kOne_0073d764 = 1.0f;               // .rdata 0073D764 (3F800000h)
constexpr float kZero_0073d700 = 0.0f;              // .rdata 0073D700 (00000000h)
constexpr U32 kOneBits = 0x3F800000u;               // immediate 1.0f message scale

template <class F> struct OnThrow {                 // runs the original EH-state cleanup only while unwinding
    F f; bool armed;
    ~OnThrow() { if (armed) f(); }
};
template <class F> OnThrow<F> on_throw(F f) { return OnThrow<F>{f, true}; }

void* client_app_() { return lp(kotor_menu_window_cells::manager_007a39fc, 4); }               // [[007A39FC]+4]
void* world_timer() { return frame_GetWorldTimer_005ed4f0(client_app_(), nullptr); }          // 005ED4F0
U32 exo_rand() {                                                                                 // [[007A39E0]+10h]->Rand()
    return static_cast<kotor_reconstruction::CExoRand*>(lp(menu_continuation::exo_base_007a39e0, 0x10))->Rand();
}
void* src(void* self) { return lp(self, 0xFC); }
void* stream(void* self) { return lp(self, 0x100); }
// Recurring "if (!+168 && +FC) source op; else if (+100) stream op" selection, inline in the original.
bool use_source(void* self) { return lw(self, 0x168) == 0 && src(self) != nullptr; }
U32 is_looping(void* self, bool& any) {
    any = true;
    if (use_source(self)) return kotor_m15_sound_source_get_looping_005d6190(src(self), nullptr);
    if (stream(self) != nullptr) return kotor_m15_stream_source_get_looping_005d6280(stream(self), nullptr);
    any = false;
    return 0;
}

// 005D5930 CExoSoundSource::Play(): EAX = [[this+4]+58h] (the play length in ms) or 0; AIUpdate stores it in +150h.
U32 play_source(void* source) { return kotor_fastcall_005d5930(source, nullptr); }             // 0066BCE4 / 0066BE6B

// CExoArrayList<CResRef> { CResRef* data; int count; int capacity; } append (inline at 0066B79B..7DA, 0066BA0C..4B).
void append_ref(void* list, const Ref* value) {
    Ref discarded;
    U32 capacity = lw(list, 8);
    if (lw(list, 4) == capacity) {                                                               // cmp [list+4],eax
        recovered_ref_array_grow_00506e20(list, nullptr, capacity != 0 ? capacity + capacity : 0x10);
    }
    const U32 count = lw(list, 4);
    Ref* const slot = static_cast<Ref*>(lp(list, 0)) + count;                                    // shl 4; add data
    sw(list, 4, count + 1);
    resource_ref_assignment::assign_00406120(slot, nullptr, &discarded, value);                 // operator=
}

void* sound_object_methods();
}  // namespace

extern "C" {
// ================================================================================== 005D5xxx / 00648100 leaves
// 005D5920 CExoSoundSource::IsHardwarePlayingSoundPleaseDontUseThis() RET 0.
U32 __fastcall kotor_fastcall_005d5920(void* source, void*) {
    void* const internal = lp(source, 4);                                                        // 005D5920
    if (internal == nullptr) return 0;                                                           // 005D5923..5925, 592C
    return sound_lower29::ActorPlaying005db0e0(internal, nullptr);                               // 005D5927 jmp 005DB0E0
}
// 005D61C0 CExoSoundSource::SetResRef(const CResRef&, int mode) RET 8.
void __fastcall kotor_fastcall_005d61c0(void* source, void*, const void* ref, U32 mode) noexcept(false) {
    if (lp(source, 4) == nullptr) return;                                                        // 005D61C3..61C8
    kotor_m15_source_internal_shut_down_005db2b0(lp(source, 4), nullptr);                       // 005D61CA
    sw(lp(source, 4), 0x58, 0);                                                                  // 005D61CF..61D2
    kotor_sound_internal::Render005d9590(lp(lp(source, 4), 0x44), nullptr);                      // 005D61D9..61DF
    kotor_sound_rev3_required::res_helper_set_ref_005d5e90(lp(source, 4), nullptr, ref, mode);   // 005D61E4..61F1
}
// 005D5980 CExoSoundSource::SetPitchVariance(float) RET 4: clamp to [0, 1] with the original x87 predicates.
void __fastcall kotor_fastcall_005d5980(void* source, void*, float variance) {
    if (!jcc_jne(variance, kOne_0073d764, 0x41)) {                                               // 005D5980..598F (> 1.0)
        variance = bits_f(0x3F800000u);                                                          // 005D5991..5999
    } else if (!jcc_jp(variance, kZero_0073d700, 0x05)) {                                        // 005D599B..59AA (< 0.0)
        variance = bits_f(0x00000000u);                                                          // 005D59AC
    }
    void* const internal = lp(source, 4);                                                        // 005D59B4
    if (internal == nullptr) return;                                                             // 005D59B7..59B9, 59C0
    kotor_sound_rev3::InternalSetPitchVariance005dba40(internal, nullptr, variance);            // 005D59BB jmp 005DBA40
}
// 005D61A0 CExoSoundSource::SetDistance(float, float) RET 8: dword copies into internal +70h / +6Ch.
void __fastcall kotor_fastcall_005d61a0(void* source, void*, U32 a_bits, U32 b_bits) {
    void* const internal = lp(source, 4);                                                        // 005D61A0
    if (internal == nullptr) return;                                                             // 005D61A3..61A5
    sw(internal, 0x70, a_bits);                                                                  // 005D61A7, 61AF
    sw(internal, 0x6C, b_bits);                                                                  // 005D61AB, 61B2
}
// 00648100 CExoArrayList<CResRef>::DelIndex(int) RET 4: count-- first, then shift the tail down by one element
// with CResRef::operator= (data and count re-read every iteration, as in the original).
void __fastcall kotor_fastcall_00648100(void* list, void*, int index) noexcept(false) {
    Ref discarded;                                                                               // {esp+10h}
    const I32 count = li(list, 4) - 1;                                                           // 0064810B..810E
    sw(list, 4, static_cast<U32>(count));                                                        // 00648113
    if (index >= count) return;                                                                  // 00648111, 8116 (jge)
    U32 offset = static_cast<U32>(index) << 4;                                                   // 00648119..811E
    do {
        Ref* const dst = reinterpret_cast<Ref*>(static_cast<U8*>(lp(list, 0)) + offset);         // 00648120..8122
        resource_ref_assignment::assign_00406120(dst, nullptr, &discarded, dst + 1);            // 00648125..812E
        ++index;                                                                                 // 00648136
        offset += 0x10;                                                                          // 00648137
    } while (index < li(list, 4));                                                               // 00648133, 813A..813C
}

// ================================================================================== CSWCSoundObject
// 0066B200 CSWCSoundObject::CSWCSoundObject() RET 0, EAX this.
void* __fastcall recovered_client_sound_object_construct_0066b200(void* self, void*) noexcept(false) {
    kotor_m15a_object_construct_0063e530(self, nullptr, kSoundObjectType_0074663e);             // 0066B216..B226 (byte 0074663E)
    auto unwind = on_throw([self] { kotor_m15e_object_destruct_0063e800(self, nullptr); });     // EH state 0 (0072D683)
    sw(self, 0, ad(sound_object_methods()));                                                     // 0066B238 vptr 007522A8
    sw(self, 0x168, 0); sw(self, 0xFC, 0); sw(self, 0x100, 0); sw(self, 0x14C, 0);              // 0066B23E..B250
    sw(self, 0x104, 1); sw(self, 0x108, 0x7F); sw(self, 0x10C, 0); sw(self, 0x110, 3);          // 0066B256..B26C
    sw(self, 0x11C, 0); sw(self, 0x124, 0x3F800000u); sw(self, 0x120, 0); sw(self, 0x114, 1);   // 0066B276..B28C
    sb(self, 0x12C, 0); sw(self, 0x144, 0); sw(self, 0x148, 0); sw(self, 0x154, 0);             // 0066B292..B2A4
    sw(self, 0x158, 0);                                                                          // 0066B2AA
    void* list = live_operator_new(0xC);                                                         // 0066B232, B2B0
    if (list != nullptr) {                                                                       // 0066B2B8..B2BA
        sw(list, 4, 0); sw(list, 8, 0); sw(list, 0, 0);                                          // 0066B2BC..B2C2
    } else {
        list = nullptr;                                                                          // 0066B2C6
    }
    sw(self, 0x13C, ad(list));                                                                   // 0066B2CC
    sw(self, 0x140, 0); sw(self, 0x15C, 0); sw(self, 0x130, 0); sw(self, 0x134, 0);             // 0066B2D2..B2E4
    sw(self, 0x138, 0); sw(self, 0x128, 0);                                                      // 0066B2EA..B2F0
    unwind.armed = false;
    return self;                                                                                 // 0066B2F6
}

// 0066B370 CSWCSoundObject::~CSWCSoundObject() RET 0.
void __fastcall recovered_client_sound_object_destruct_0066b370(void* self, void*) noexcept(false) {
    sw(self, 0, ad(sound_object_methods()));                                                     // 0066B38F
    auto unwind = on_throw([self] { kotor_m15e_object_destruct_0063e800(self, nullptr); });     // EH state 0 (0072D698)
    if (void* const source = src(self)) {                                                        // 0066B395..B3A3
        reinterpret_cast<void*(__fastcall*)(void*, void*, U32)>(vslot(source, 0))(source, nullptr, 1);  // 0066B3A5..B3A9
        sw(self, 0xFC, 0);                                                                       // 0066B3AB
    }
    if (void* const s = stream(self)) {                                                          // 0066B3B1..B3B9
        kotor_fastcall_005d6200(s, nullptr);                                                     // 0066B3BB..B3BD
        live_free(s);                                                                            // 0066B3C2..B3C3
        sw(self, 0x100, 0);                                                                      // 0066B3CB
    }
    if (void* const list = lp(self, 0x13C)) {                                                    // 0066B3D1..B3D9
        live_free(lp(list, 0));                                                                  // 0066B3DB..B3DE
        sw(list, 0, 0);                                                                          // 0066B3E4
        live_free(list);                                                                         // 0066B3E3..B3E6
        sw(self, 0x13C, 0);                                                                      // 0066B3EE
    }
    unwind.armed = false;                                                                        // 0066B3F6 state -1
    kotor_m15e_object_destruct_0063e800(self, nullptr);                                          // 0066B3F4..B3FE
}

// 0066BAB0 CSWCSoundObject scalar deleting destructor (slot +000) RET 4.
void* __fastcall recovered_client_sound_object_scalar_deleting_destructor_0066bab0(void* self, void*, U32 flags) noexcept(false) {
    recovered_client_sound_object_destruct_0066b370(self, nullptr);                              // 0066BAB1..BAB3
    if ((flags & 1u) != 0) live_free(self);                                                      // 0066BAB8..BAC5
    return self;                                                                                 // 0066BAC8
}

// 0066B420 CSWCSoundObject::LoadFromMessage(CSWCMessage*) RET 4 (EAX not set: void).
void __fastcall recovered_client_sound_object_load_from_message_0066b420(void* self, void*, void* message) noexcept(false) {
    float vector[3] = {0.0f, 0.0f, 0.0f};                                                        // 0066B446..B456 {esp+14h} Vector()
    const U32 id = kotor_fastcall_00692630(message, nullptr);                                    // 0066B444..B45E
    reinterpret_cast<void(__fastcall*)(void*, void*, U32)>(vslot(self, 0x04))(self, nullptr, id); // 0066B463..B466 SetId
    const U32 active = kotor_fastcall_004d66f0(message, nullptr);                                // 0066B469..B46B (ebp)
    const U32 positional = kotor_fastcall_004d66f0(message, nullptr);                            // 0066B470..B47B ({esp+60h})
    const U32 is_stream = kotor_fastcall_004d66f0(message, nullptr);                             // 0066B47F..B484 (ebx)
    if (is_stream != 0) {                                                                        // 0066B486..B488
        void* const raw = live_operator_new(4);                                                  // 0066B48A..B48C
        auto unwind = on_throw([raw] { live_free(raw); });                                       // EH state 0
        void* const made = raw != nullptr ? menu_tables_required::StreamConstruct_005d5a50(raw, nullptr) : nullptr;  // 0066B498..B4AD
        unwind.armed = false;
        sw(self, 0x100, ad(made));                                                               // 0066B4AF
        sw(self, 0x168, 1);                                                                      // 0066B4B5
    } else {
        void* const raw = live_operator_new(8);                                                  // 0066B4C1..B4C3
        auto unwind = on_throw([raw] { live_free(raw); });                                       // EH state 1
        void* const made = raw != nullptr ? kotor_sound_source_construct_005d5870(raw, nullptr) : nullptr;  // 0066B4CF..B4E4
        unwind.armed = false;
        sw(self, 0xFC, ad(made));                                                                // 0066B4E6
    }
    recovered_client_sound_object_set_sound_id_0066ae30(self, nullptr, active);                 // 0066B4EC..B4F7
    if (use_source(self)) kotor_m15_sound_source_set_3d_005d5910(src(self), nullptr, positional);         // 0066B4FC..B515
    else if (stream(self) != nullptr) kotor_m15_stream_source_set_3d_005d6290(stream(self), nullptr, positional);  // 0066B51C..B52B
    if (use_source(self)) kotor_m15_sound_source_set_looping_005d59d0(src(self), nullptr, is_stream);     // 0066B530..B545
    else if (stream(self) != nullptr) menu_tables_required::StreamSetLooping_005d5b40(stream(self), nullptr, is_stream);  // 0066B54C..B557
    if (is_stream != 0) {                                                                        // 0066B55C..B562
        if (positional != 0) {                                                                   // 0066B564..B56E
            menu_tables_required::StreamSetLooping_005d5b40(stream(self), nullptr, 1);           // 0066B566..B570
            kotor_m15_stream_source_set_3d_005d6290(stream(self), nullptr, 1);                  // 0066B575..B57D
            menu_tables_required::StreamSetPriority_005d5ba0(stream(self), nullptr, 5);          // 0066B582..B58A
        } else {
            menu_tables_required::StreamSetLooping_005d5b40(stream(self), nullptr, 1);           // 0066B566..B591
            kotor_m15_stream_source_set_3d_005d6290(stream(self), nullptr, 0);                  // 0066B596..B59E
            menu_tables_required::StreamSetPriority_005d5ba0(stream(self), nullptr, 4);          // 0066B5A3..B5AB
        }
    } else {
        kotor_sound_rev3::SourceSetPriorityGroup005d5900(src(self), nullptr, positional != 0 ? 0x16u : 0x15u);  // 0066B5B2..B5C2
    }
    sw(self, 0x108, kotor_fastcall_004d6720(message, nullptr, 8));                               // 0066B5C7..B5D7 movzx
    sw(self, 0x10C, kotor_fastcall_004d6720(message, nullptr, 8));                               // 0066B5D3..B5E5
    sw(self, 0x110, kotor_fastcall_004d6720(message, nullptr, 8));                               // 0066B5EB..B600
    sf(self, 0x11C, kotor_fastcall_004d67b0(message, nullptr, kOneBits, 0x20));                  // 0066B5F7..B612 fstp dword
    sf(self, 0x124, kotor_fastcall_004d67b0(message, nullptr, kOneBits, 0x20));                  // 0066B60B..B61F
    sw(self, 0x120, kotor_fastcall_004d6780(message, nullptr, 0x20));                            // 0066B625..B632
    sb(self, 0x12C, kotor_fastcall_004d6720(message, nullptr, 8));                               // 0066B62E..B641
    sw(self, 0x144, kotor_fastcall_004d6780(message, nullptr, 0x20));                            // 0066B63D..B650
    sw(self, 0x148, kotor_fastcall_004d6780(message, nullptr, 0x20));                            // 0066B64C..B664
    const float distance_a = kotor_fastcall_004d67b0(message, nullptr, kOneBits, 0x20);          // 0066B65B..B66F {esp+10h}
    const float distance_b = kotor_fastcall_004d67b0(message, nullptr, kOneBits, 0x20);          // 0066B673..B681 {esp+60h}
    if (use_source(self)) kotor_fastcall_005d61a0(src(self), nullptr, f_bits(distance_a), f_bits(distance_b));  // 0066B685..B6A3
    else if (stream(self) != nullptr)
        kotor_m15_stream_source_set_distance_005d62a0(stream(self), nullptr, distance_a, distance_b);           // 0066B6AA..B6BE
    sw(self, 0x154, kotor_fastcall_004d66f0(message, nullptr));                                  // 0066B6C3..B6CC
    sw(self, 0x158, kotor_fastcall_004d66f0(message, nullptr));                                  // 0066B6CA..B6D9
    sw(self, 0x130, kotor_fastcall_004d66f0(message, nullptr));                                  // 0066B6D7..B6ED
    sf(self, 0x134, kotor_fastcall_004d67b0(message, nullptr, kOneBits, 0x20));                  // 0066B6E4..B6FA
    sf(self, 0x138, kotor_fastcall_004d67b0(message, nullptr, kOneBits, 0x20));                  // 0066B6F8..B70E
    vector[0] = kotor_fastcall_004d67b0(message, nullptr, kOneBits, 0x20);                       // 0066B70C..B720
    vector[1] = kotor_fastcall_004d67b0(message, nullptr, kOneBits, 0x20);                       // 0066B724..B732
    vector[2] = kotor_fastcall_004d67b0(message, nullptr, kOneBits, 0x20);                       // 0066B736..B744 fst
    sf(self, 0x118, vector[2]);                                                                  // 0066B74A fstp
    reinterpret_cast<void(__fastcall*)(void*, void*, const void*)>(vslot(self, 0x8C))(self, nullptr, vector);  // 0066B748..B757
    const unsigned short sounds = kotor_fastcall_004d6750(message, nullptr, 0x10);               // 0066B75D..B766 (bp)
    if (sounds != 0) {                                                                           // 0066B768..B76B (jbe)
        U32 remaining = sounds;                                                                  // 0066B76D..B770 {esp+60h}
        do {
            Ref read;                                                                            // {esp+30h}
            const Ref* const got = static_cast<const Ref*>(kotor_fastcall_004d6180(message, nullptr, &read, 0x10));  // 0066B774..B77D
            Ref local;                                                                           // {esp+20h}
            std::memcpy(&local, got, sizeof local);                                              // 0066B782..B7A5
            void* const list = lp(self, 0x13C);                                                  // 0066B787
            Ref discarded;                                                                       // {esp+40h}
            U32 capacity = lw(list, 8);                                                          // 0066B79B
            if (lw(list, 4) == capacity)                                                         // 0066B7A2..B7A9
                recovered_ref_array_grow_00506e20(list, nullptr, capacity != 0 ? capacity + capacity : 0x10);  // 0066B7AB..B7BB
            const U32 count = lw(list, 4);                                                       // 0066B7C0
            Ref* const slot = static_cast<Ref*>(lp(list, 0)) + count;                            // 0066B7C3..B7CA
            sw(list, 4, count + 1);                                                              // 0066B7CC..B7CD
            resource_ref_assignment::assign_00406120(slot, nullptr, &discarded, &local);        // 0066B7D0..B7DA
        } while (--remaining != 0);                                                              // 0066B7DF..B7E3
    }
    sw(self, 0x15C, 1);                                                                          // 0066B7ED
    sw(self, 0x150, 0);                                                                          // 0066B7F7
    if (lw(self, 0x154) != 0 && sounds == 1 && lw(self, 0x130) == 0 && lw(self, 0x144) == 0) {    // 0066B7E5..B81B
        if (use_source(self)) kotor_m15_sound_source_set_looping_005d59d0(src(self), nullptr, 1);          // 0066B81D..B833
        else if (stream(self) != nullptr) menu_tables_required::StreamSetLooping_005d5b40(stream(self), nullptr, 1);  // 0066B83A..B846
    }
    // 0066B84B..B882: UpdateIntervalVariation 0066B000, inlined.
    U32 next = lw(self, 0x144);                                                                  // 0066B84B
    if (next != 0) {                                                                             // 0066B851..B853
        const U32 variation = lw(self, 0x148);                                                   // 0066B855
        if (variation != 0) {                                                                    // 0066B85B..B85D (jbe)
            const U32 r = exo_rand();                                                            // 0066B85F..B867
            next += r % (variation + variation) - variation;                                     // 0066B86C..B875 (div unsigned)
            if (static_cast<I32>(next) < 0) next = 0;                                            // 0066B877..B880 (sets/dec/and)
        }
        sw(self, 0x14C, next);                                                                   // 0066B882
    }
}

// 0066B8A0 CSWCSoundObject::AddToArea(CSWCArea*, CGameObjectArray*) RET 8, EAX BOOL.
U32 __fastcall recovered_client_sound_object_add_to_area_0066b8a0(void* self, void*, void* area, void* objects) noexcept(false) {
    void* const ai_master = kotor_m15e_client_get_client_ai_master_005ed6b0(client_app_(), nullptr);   // 0066B8A0..B8AE (ebp)
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(vslot(self, 0x84))(self, nullptr, area);  // 0066B8B3..B8BE SetArea
    recovered_bridge_dword_array_add_unique_006449d0(static_cast<U8*>(area) + 0x1D0, nullptr, lw(self, 4));  // 0066B8C4..B8D1
    const std::uint8_t added = recovered_object_array_add_external_004d7ff0(objects, nullptr,
                                                                            reinterpret_cast<U32*>(static_cast<U8*>(self) + 4), self, 0);  // 0066B8C7, B8D6..B8DE
    if (added != kFalse_0074666c) return 0;                                                      // 0066B8E3..B8E9, B8F7..B8FD
    if (recovered_client_ai_master_add_object_00604a20(ai_master, nullptr, self) == 0) return 0; // 0066B8EB..B8F5
    frame_GetWorldTime_004ade40(world_timer(), nullptr, static_cast<U8*>(self) + 0x160,
                                static_cast<U8*>(self) + 0x164);                                 // 0066B900..B91E
    bool any = false;
    const U32 looping = is_looping(self, any);                                                   // 0066B923..B948
    if (any && looping != 0 && lw(self, 0x104) != 0) sw(self, 0x128, 1);                        // 0066B94D..B95B
    return 1;                                                                                    // 0066B968
}

// 0066AE90 CSWCSoundObject::UpdateTimeOfDay() RET 0.
void __fastcall recovered_client_sound_object_update_time_of_day_0066ae90(void* self, void*) {
    if (lw(self, 0x110) != 0) {                                                                  // 0066AE93..AE99, AEA3
        void* const module = kotor_fastcall_005ed530(client_app_(), nullptr);                    // 0066AE9B..AEA5 GetModule
        const U32 phase = lb(module, 0xB4);                                                      // 0066AEAA
        if (phase > 4) return;                                                                   // 0066AEB1..AEB4 (ja)
        switch (phase) {                                                                         // 0066AEB6 table 0066AF14
        case 0: case 1: case 3:                                                                  // -> 0066AEBD
            sw(self, 0x114, lw(self, 0x110) != 2 ? 1u : 0u);                                     // 0066AEBD..AECB
            return;
        default:                                                                                 // 2, 4 -> 0066AED3
            sw(self, 0x114, lw(self, 0x110) != 1 ? 1u : 0u);                                     // 0066AED3..AEE1
            return;
        }
    }
    const U32 mask = lw(self, 0x120);                                                            // 0066AEEA
    void* const module = kotor_fastcall_005ed530(client_app_(), nullptr);                        // 0066AE9B..AEF0
    const U32 hour = lw(module, 0xB0);                                                           // 0066AEF5
    sw(self, 0x114, ((1u << (hour & 31u)) & mask) != 0 ? 1u : 0u);                               // 0066AEFB..AF0B (shl uses cl & 31)
}

// 0066B130 CSWCSoundObject::Stop() RET 0.
void __fastcall recovered_client_sound_object_stop_0066b130(void* self, void*) noexcept(false) {
    if (lw(self, 0x104) != 0) { sw(self, 0x14C, 0); sw(self, 0x104, 0); }                        // 0066B133..B146
    if (lw(self, 0x168) == 0) kotor_m15_sound_source_stop_005d5a20(src(self), nullptr);          // 0066B14C..B15A (no null check)
    else kotor_fastcall_005d5b80(stream(self), nullptr);                                         // 0066B16E..B174 (no null check)
    sw(self, 0x150, 0); sw(self, 0x14C, 0);                                                      // 0066B15F..B165 / B179..B17F
}

// 0066B190 CSWCSoundObject::FadeAndStop(int time) RET 4.
void __fastcall recovered_client_sound_object_fade_and_stop_0066b190(void* self, void*, U32 time) noexcept(false) {
    if (lw(self, 0x104) != 0) { sw(self, 0x14C, 0); sw(self, 0x104, 0); }                        // 0066B193..B1A6
    if (lw(self, 0x168) == 0) kotor_m15_sound_source_stop_005d5a20(src(self), nullptr);          // 0066B1AC..B1BA
    else sound_fade29::Begin005d5b90(stream(self), nullptr, time);                               // 0066B1D0..B1DB
    sw(self, 0x150, 0); sw(self, 0x14C, 0);                                                      // 0066B1BF..B1C5 / B1E0..B1E6
}

// 0066B050 CSWCSoundObject::Play() RET 0.
void __fastcall recovered_client_sound_object_play_0066b050(void* self, void*) noexcept(false) {
    Ref unused;                                                                                  // {esp+8}
    kotor_item_drop_sounds::ref_default_element_00405ed0(&unused, nullptr);                      // 0066B057..B05B
    bool restart;
    if (lw(self, 0x168) == 0) {                                                                  // 0066B060..B06D
        if (kotor_fastcall_005d5920(src(self), nullptr) == 0) restart = true;                    // 0066B06F..B07C
        else if (lw(self, 0x168) == 0) restart = false;                                          // 0066B07E..B086
        else restart = menu_tables_required::StreamIsPlaying_005d5af0(stream(self), nullptr) == 0;   // 0066B088..B095
    } else {
        restart = menu_tables_required::StreamIsPlaying_005d5af0(stream(self), nullptr) == 0;    // 0066B088..B095
    }
    if (restart) {
        sw(self, 0x150, 0); sw(self, 0x14C, 0); sw(self, 0x15C, 1);                              // 0066B097..B0AB
        frame_GetWorldTime_004ade40(world_timer(), nullptr, static_cast<U8*>(self) + 0x160,
                                    static_cast<U8*>(self) + 0x164);                             // 0066B0B1..B0CE
    }
    if (lw(self, 0x104) == 1) return;                                                            // 0066B0D3..B0D9
    sw(self, 0x14C, 0); sw(self, 0x104, 1);                                                      // 0066B0E3..B0ED
    bool any = false;
    const U32 looping = is_looping(self, any);                                                   // 0066B0DB..B110
    if (any && looping != 0) sw(self, 0x128, 1);                                                 // 0066B115..B119
}

// 0066AF30 CSWCSoundObject::GetTimeDiff() RET 0: time of day since the stored start (+160h day, +164h time).
U32 __fastcall recovered_client_sound_object_get_time_diff_0066af30(void* self, void*) noexcept(false) {
    U32 now_time = 0, now_day = 0, out_time = 0, out_day = 0;                                    // {esp+4} {esp+8} {esp+0Ch} {esp+10h}
    frame_GetWorldTime_004ade40(world_timer(), nullptr, &now_day, &now_time);                    // 0066AF30..AF4F
    recovered_world_time_difference_004ae460(world_timer(), nullptr, now_day, now_time, lw(self, 0x160), lw(self, 0x164),
                                             &out_day, &out_time);                               // 0066AF54..AF86
    return out_time;                                                                             // 0066AF8B
}

// 0066AFA0 CSWCSoundObject::UpdateVolumeVariation() RET 0.
void __fastcall recovered_client_sound_object_update_volume_variation_0066afa0(void* self, void*) noexcept(false) {
    const I32 variation = li(self, 0x10C);                                                       // 0066AFA4
    if (variation <= 0) return;                                                                  // 0066AFAA..AFAC (jle)
    const I32 base = li(self, 0x108);                                                            // 0066AFAF
    const I32 r = crt_rand_006fc447();                                                           // 0066AFB5
    I32 volume = r % static_cast<I32>(static_cast<U32>(variation) + static_cast<U32>(variation)); // 0066AFBA..AFBE (cdq/idiv)
    volume = static_cast<I32>(static_cast<U32>(volume) - static_cast<U32>(variation) + static_cast<U32>(base));  // 0066AFC0..AFC2
    if (volume > 0x7F) volume = 0x7F;                                                            // 0066AFC4..AFCA
    const U32 clamped = volume < 0 ? 0u : static_cast<U32>(volume);                              // 0066AFD5..AFDD
    if (lw(self, 0x168) == 0) kotor_fastcall_005d5950(src(self), nullptr, clamped);              // 0066AFDF..AFEA
    else table29_required::gui_volume_005d5b50(stream(self), nullptr, clamped);                  // 0066AFF2..AFF8
}

// 0066B000 CSWCSoundObject::UpdateIntervalVariation() RET 0.
void __fastcall recovered_client_sound_object_update_interval_variation_0066b000(void* self, void*) noexcept(false) {
    U32 next = lw(self, 0x144);                                                                  // 0066B004
    if (next == 0) return;                                                                       // 0066B00A..B00C
    const U32 variation = lw(self, 0x148);                                                       // 0066B00F
    if (variation != 0) {                                                                        // 0066B015..B017 (jbe)
        const U32 r = exo_rand();                                                                // 0066B019..B021
        next += r % (variation + variation) - variation;                                         // 0066B026..B02F (div unsigned)
        if (static_cast<I32>(next) < 0) next = 0;                                                // 0066B031..B03A
    }
    sw(self, 0x14C, next);                                                                       // 0066B03C
}

// 0066B980 CSWCSoundObject::ShuffleList() RET 0: move entries in CRT-random order into a fresh list.
void __fastcall recovered_client_sound_object_shuffle_list_0066b980(void* self, void*) noexcept(false) {
    void* shuffled = live_operator_new(0xC);                                                     // 0066B99C..B9A0
    if (shuffled != nullptr) { sw(shuffled, 4, 0); sw(shuffled, 8, 0); sw(shuffled, 0, 0); }     // 0066B9AA..B9B6
    else shuffled = nullptr;                                                                     // 0066B9BA
    I32 left = li(lp(self, 0x13C), 4);                                                           // 0066B9BC..B9C2 (ebp)
    if (left > 0) {                                                                              // 0066B9C5..B9CF (jle)
        I32 counter = left;                                                                      // 0066B9D5 {esp+10h}
        do {
            const I32 index = crt_rand_006fc447() % left;                                        // 0066B9E0..B9E8 (cdq/idiv)
            Ref local;                                                                           // {esp+14h}
            std::memcpy(&local, static_cast<const Ref*>(lp(lp(self, 0x13C), 0)) + index, sizeof local);   // 0066B9EA..BA16
            append_ref(shuffled, &local);                                                        // 0066BA0C..BA4B
            kotor_fastcall_00648100(lp(self, 0x13C), nullptr, index);                            // 0066BA50..BA57
            --left;                                                                              // 0066BA60
        } while (--counter != 0);                                                                // 0066BA5C..BA66
    }
    if (void* const old = lp(self, 0x13C)) {                                                     // 0066BA6C..BA74
        live_free(lp(old, 0));                                                                   // 0066BA76..BA79
        sw(old, 0, 0);                                                                           // 0066BA7F
        live_free(old);                                                                          // 0066BA7E..BA85
    }
    sw(self, 0x13C, ad(shuffled));                                                               // 0066BA91
}

// 0066BAD0 CSWCSoundObject::GetNextSound() RET 4: CResRef by value (hidden result pointer), EAX = result.
Ref* __fastcall recovered_client_sound_object_get_next_sound_0066bad0(void* self, void*, Ref* out) noexcept(false) {
    Ref ref;                                                                                     // {esp+4}
    Ref discarded;                                                                               // {esp+14h}
    kotor_item_drop_sounds::ref_default_element_00405ed0(&ref, nullptr);                         // 0066BAD6..BADA
    if (lw(self, 0x15C) != 0) {                                                                  // 0066BADF..BAE7
        void* const list = lp(self, 0x13C);                                                      // 0066BAE9
        sw(self, 0x15C, 0); sw(self, 0x140, 0);                                                  // 0066BAEF..BAF9
        resource_ref_assignment::assign_00406120(&ref, nullptr, &discarded, static_cast<const Ref*>(lp(list, 0)));  // 0066BB03..BB0F
    } else {
        void* const list = lp(self, 0x13C);                                                      // 0066BB19
        const I32 last = li(list, 4) - 1;                                                        // 0066BB1F..BB28
        I32 index = li(self, 0x140);                                                             // 0066BB22
        if (index < last) {                                                                      // 0066BB29..BB2B (jge)
            ++index;                                                                             // 0066BB2D
            sw(self, 0x140, static_cast<U32>(index));                                            // 0066BB2E
            resource_ref_assignment::assign_00406120(&ref, nullptr, &discarded,
                                                     static_cast<const Ref*>(lp(list, 0)) + index);   // 0066BB34..BB45
        } else if (lw(self, 0x154) != 0) {                                                       // 0066BB4C..BB54
            if (lw(self, 0x158) != 0) recovered_client_sound_object_shuffle_list_0066b980(self, nullptr);  // 0066BB56..BB62
            void* const fresh = lp(self, 0x13C);                                                 // 0066BB67
            sw(self, 0x140, 0);                                                                  // 0066BB6D
            resource_ref_assignment::assign_00406120(&ref, nullptr, &discarded, static_cast<const Ref*>(lp(fresh, 0)));  // 0066BB77..BB83
        } else {
            unresolved_ref_assign_00406290(&ref, nullptr, &discarded, kZero_00741e18);            // 0066BB8A..BB98
        }
    }
    std::memcpy(out, &ref, sizeof ref);                                                          // 0066BB9D..BBBB
    return out;                                                                                  // 0066BBA5 (eax)
}

// 0066BBD0 CSWCSoundObject::AIUpdate() (slot +074) RET 0.
void __fastcall recovered_client_sound_object_ai_update_0066bbd0(void* self, void*) noexcept(false) {
    Ref ref;                                                                                     // {esp+8}
    Ref first;                                                                                   // {esp+18h}
    Ref second;                                                                                  // {esp+28h}
    kotor_item_drop_sounds::ref_default_element_00405ed0(&ref, nullptr);                         // 0066BBD7..BBDB
    bool stop_stream = false;
    if (lw(self, 0x104) == 0) {                                                                  // 0066BBE0..BBEA
        if (lw(self, 0x168) != 0) {                                                              // 0066BBEC..BBF2
            if (menu_tables_required::StreamIsPlaying_005d5af0(stream(self), nullptr) == 0) return;   // 0066BBF4..BC01
            stop_stream = true;
        }
    } else {
        if (lw(self, 0x110) == 0) recovered_client_sound_object_update_time_of_day_0066ae90(self, nullptr);  // 0066BC18..BC22
        if (lw(self, 0x114) == 0) {                                                              // 0066BC27..BC33
            if (lw(self, 0x168) != 0) stop_stream = true;                                        // 0066BC35..BC37
        } else {
            goto audible;                                                                        // 0066BC5D
        }
    }
    if (stop_stream) {
        kotor_fastcall_005d5b80(stream(self), nullptr);                                          // 0066BC07..BC0D
        return;
    }
    if (kotor_fastcall_005d5920(src(self), nullptr) == 0) return;                                // 0066BC39..BC46
    kotor_m15_sound_source_stop_005d5a20(src(self), nullptr);                                    // 0066BC4C..BC52
    return;

audible: {
    bool any = false;
    const U32 looping = is_looping(self, any);                                                   // 0066BC5D..BC80
    if (any && looping != 0) {                                                                   // 0066BC85..BC87
        if (lw(self, 0x128) == 0) return;                                                        // 0066BC8D..BC93
        sw(self, 0x128, 0); sw(self, 0x15C, 1);                                                  // 0066BC9F..BCA5
        U32 played;
        if (lw(self, 0x168) == 0) {                                                              // 0066BC99, BCAF
            Ref* const next = recovered_client_sound_object_get_next_sound_0066bad0(self, nullptr, &first);   // 0066BCB1..BCB9
            kotor_fastcall_005d61c0(src(self), nullptr, next, 0);                                // 0066BCBE..BCC5 (push 0 at BCB1)
            kotor_fastcall_005d5950(src(self), nullptr, lb(self, 0x108));                        // 0066BCCA..BCD9
            played = play_source(src(self));                                                     // 0066BCDE..BCE4
        } else {
            table29_required::gui_volume_005d5b50(stream(self), nullptr, lb(self, 0x108));       // 0066BCEB..BCFA
            Ref* const next = recovered_client_sound_object_get_next_sound_0066bad0(self, nullptr, &first);   // 0066BCFF..BD0B
            played = menu_tables_required::StreamPlay_005d5b10(stream(self), nullptr, next, 4, 0, 0, 0);    // 0066BD10..BD17
        }
        sw(self, 0x150, played);                                                                 // 0066BD1E
        if (played != 0) return;                                                                 // 0066BD1C, BD24
        if (lw(self, 0x104) == 0) return;                                                        // 0066BD2A..BD30
        sw(self, 0x14C, 0); sw(self, 0x104, 0);                                                  // 0066BD36..BD3C
        return;
    }
    // 0066BD48: one-shot sounds.
    U32 elapsed = recovered_client_sound_object_get_time_diff_0066af30(self, nullptr);           // 0066BD49..BD4B (edi)
    frame_GetWorldTime_004ade40(world_timer(), nullptr, static_cast<U8*>(self) + 0x160,
                                static_cast<U8*>(self) + 0x164);                                 // 0066BD50..BD70
    const U32 left = lw(self, 0x150);                                                            // 0066BD75
    if (left != 0) {                                                                             // 0066BD7B..BD7D
        if (elapsed < left) { sw(self, 0x150, left - elapsed); return; }                         // 0066BD7F..BD81, BDE9..BDF7
        sw(self, 0x150, 0);                                                                      // 0066BD83
        elapsed = 0;                                                                             // 0066BD89
    }
    const U32 wait = lw(self, 0x14C);                                                            // 0066BD8B
    if (wait != 0) {                                                                             // 0066BD91..BD93
        if (elapsed < wait) { sw(self, 0x14C, wait - elapsed); return; }                         // 0066BD95..BD97, BDF8..BE06
        sw(self, 0x14C, 0);                                                                      // 0066BD99
    }
    Ref* const next = recovered_client_sound_object_get_next_sound_0066bad0(self, nullptr, &first);   // 0066BD9F..BDAB
    Ref* const copy = resource_ref_assignment::assign_00406120(&ref, nullptr, &second, next);   // 0066BDB0..BDBA
    bool deactivate;
    if (c2da_equal_text_004060d0(copy, nullptr, kZero_00741e18) != 0) {                         // 0066BD9F (push), BDBF..BDC8
        deactivate = true;
    } else {
        U32 played;
        if (lw(self, 0x168) == 0) {                                                              // 0066BE07..BE0D
            kotor_fastcall_005d61c0(src(self), nullptr, &ref, 0);                                // 0066BE0F..BE1B
            kotor_fastcall_005d5950(src(self), nullptr, lb(self, 0x108));                        // 0066BE20..BE2F
            recovered_client_sound_object_update_volume_variation_0066afa0(self, nullptr);       // 0066BE34..BE36
            kotor_fastcall_005d5980(src(self), nullptr, lf(self, 0x11C));                        // 0066BE3B..BE4B
            kotor_fastcall_005d5a30(src(self), nullptr, lf(self, 0x124));                        // 0066BE50..BE60
            played = play_source(src(self));                                                     // 0066BE65..BE6B
        } else {
            table29_required::gui_volume_005d5b50(stream(self), nullptr, lb(self, 0x108));       // 0066BE72..BE81
            recovered_client_sound_object_update_volume_variation_0066afa0(self, nullptr);       // 0066BE86..BE88
            played = menu_tables_required::StreamPlay_005d5b10(stream(self), nullptr, &ref, 4, 0, 0, 0);    // 0066BE8D..BE9D
        }
        sw(self, 0x150, played);                                                                 // 0066BEA4
        deactivate = false;
        if (played == 0) {                                                                       // 0066BEA2..BEAA
            void* const list = lp(self, 0x13C);                                                  // 0066BEAC
            if (li(list, 4) <= 1) {                                                              // 0066BEB2..BEB6 (jle)
                deactivate = true;
            } else {
                kotor_fastcall_00648100(list, nullptr, li(self, 0x140));                         // 0066BEBC..BEC3
                const I32 index = li(self, 0x140);                                               // 0066BEC8
                if (index > 0) sw(self, 0x140, static_cast<U32>(index - 1));                     // 0066BECE..BED3
            }
        }
        if (!deactivate) {
            if (lw(self, 0x154) == 0 && li(self, 0x140) >= li(lp(self, 0x13C), 4) - 1) return;  // 0066BED9..BEF3
            recovered_client_sound_object_update_interval_variation_0066b000(self, nullptr);     // 0066BEF5..BEF7
            return;
        }
    }
    // 0066BDCA
    if (lw(self, 0x104) == 0) return;                                                            // 0066BDCA..BDD0
    sw(self, 0x14C, 0); sw(self, 0x104, 0);                                                      // 0066BDD7..BDDD
    }
}
}  // extern "C"

namespace {
// 007522A8 CSWCSoundObject method table (82 slots): the CSWCObject table 007516C8 with seven class slots.
// The shared slots are the L15-E table's own entries (copied once, never the raw original addresses).
void* sound_object_methods() {
    static void* table[82];
    static bool built = false;
    if (!built) {
        std::memcpy(table, kotor_m15e_object_methods_007516c8, sizeof table);
        table[0x000 / 4] = reinterpret_cast<void*>(&recovered_client_sound_object_scalar_deleting_destructor_0066bab0);   // 0066BAB0
        table[0x06C / 4] = reinterpret_cast<void*>(&kotor_m15e_slot00c_self_00641db0);                                    // 00641DB0
        table[0x074 / 4] = reinterpret_cast<void*>(&recovered_client_sound_object_ai_update_0066bbd0);                    // 0066BBD0 AIUpdate
        table[0x078 / 4] = reinterpret_cast<void*>(&mp_slot30_ai_zero_0066b360);                                          // 0066B360 LoadModel -> 0
        table[0x07C / 4] = reinterpret_cast<void*>(&slot_void_return_4_0060e760_a1);                                      // 0060E760 ret 4
        table[0x080 / 4] = reinterpret_cast<void*>(&kotor_m15e_slot090_void0_004015a0);                                   // 004015A0 ret
        table[0x08C / 4] = reinterpret_cast<void*>(&kotor_sound_object_set_position_0066b310);                            // 0066B310 SetPosition
        built = true;
    }
    return table;
}
}  // namespace
