// lane-m48-b fork CLIENT, unit client-sound-engine: CClientExoAppInternal::UpdateSoundEngine 005F5370 (R4 suffix
// frame_UpdateSound_005f5370, every frame) and its leaves 00641980, 0066D5D0, 005F5180, 005D5DE0, 005D5DF0.
// On the main-menu frame [this+18h] (module) is null, so only CExoSound::Render 005D5DD0 runs (005F5376..005F537B).
// Evidence: dumpbin /disasm swkotor.exe 005F5370..005F56A7, 00641980..00641990, 0066D5D0..0066D62E, 005F5180..005F51BC,
// 005D5DE0..005D5DFB. Data: 007A22A0 .data float 3F400000h (0.75f, written here; no other owner -> single storage
// here), 0073D700 = 0.0f, 0073D764 = 1.0f, 00740CBC = BF800000h (-1.0f).
// D30-FINAL: x87 intermediates as double, float only at the original fstp dword stores (cited per line).
#include "e6446a7354-r4_lowers.h"

using namespace r4_lowers;

namespace {
struct Vec { float x, y, z; };
struct Quat { float w[4]; };
}
namespace sound_fade29_required { void __fastcall update_005d5dd0(void*, void*); }                     // CExoSound::Render (sound-path)
namespace kotor_sound_path_required {
void __fastcall listener_orientation_005d6540(void* internal, void*, void* front, void* up);          // sound-volume
void __fastcall listener_position_005d6600(void* internal, void*, void* position);                   // sound-volume
}
extern "C" {
extern void* kotor_cell_sound_007a39ec;
Vec* __fastcall vector_add_assign_004a9740(Vec* self, void*, const Vec* other);           // Vector::operator+= (3d model)
Vec* __fastcall vector_subtract_assign_004a9770(Vec* self, void*, const Vec* other);      // Vector::operator-= (core46-math)
Vec* __cdecl vector_component_mult_00423bf0(Vec* out, const Vec* v, float scale);         // vectorComponentMult (world-los)
Vec* __fastcall quaternion_zdir_0043e250(const Quat* q, void*, Vec* out);                 // Quaternion::zdir (model46-f2-math)
Vec* __fastcall quaternion_ydir_0044f420(const Quat* q, void*, Vec* out);                 // Quaternion::ydir (model46-f2-math)
void* __fastcall frame_GetCharacter_006346c0(void*, void*, int);                         // CSWParty::GetCharacter
void* __fastcall kotor_fastcall_00641980(void* module, void*);
Vec* __fastcall kotor_fastcall_0066d5d0(void* follower, void*, Vec* out);
Vec* __fastcall kotor_fastcall_005f5180(void* camera, void*, Vec* out);
void __fastcall kotor_fastcall_005d5de0(void* sound, void*, Vec* front, Vec* up);
void __fastcall kotor_fastcall_005d5df0(void* sound, void*, Vec* position);
void __fastcall frame_UpdateSound_005f5370(menu_continuation::Client*, void*);
}
namespace kotor_sound_listener_cells {
extern volatile float listener_blend_007a22a0;
volatile float listener_blend_007a22a0 = 0.75f;   // .data 007A22A0 = 3F400000h
}

namespace {
constexpr float k_zero_0073d700 = 0.0f, k_one_0073d764 = 1.0f, k_minus_one_00740cbc = -1.0f;
using VecSlot = Vec* (__thiscall*)(void*, Vec*);
using QuatSlot = Quat* (__thiscall*)(void*, Quat*);   // 0045C470 copies 16 bytes (cam+88h..97h) into the out buffer
using ObjectSlot = void* (__thiscall*)(void*, U32);
using FloatSlot = double (__thiscall*)(void*);     // ST0 result kept at full width (fadd at 005F5566 consumes ST0)
template <class F> F slot(void* object, U32 offset) { return reinterpret_cast<F>(w(pw(object, 0), offset)); } // r4_lowers vcall precedent
}

// 00641980 CSWCModule::GetMiniGame: [this+48h] ? [[this+48h]+264h] : 0.
extern "C" void* __fastcall kotor_fastcall_00641980(void* module, void*) {
    void* area = pw(module, 0x48);                                      // 00641980
    return area ? pw(area, 0x264) : nullptr;                            // 00641983..0064198E
}
// 0066D5D0 CSWTrackFollower::GetPosition(out), RET 4: zero vector unless [this+6Ch]; then [[this+68h]]->slot64(local).
extern "C" Vec* __fastcall kotor_fastcall_0066d5d0(void* follower, void*, Vec* out) {
    Vec local{0.0f, 0.0f, 0.0f};                                        // 0066D5D8..0066D5E8
    Vec value = local;                                                  // 0066D60B..0066D613
    if (w(follower, 0x6c) != 0) {                                       // 0066D5D3..0066D5F0
        void* object = pw(pw(follower, 0x68), 0);                       // 0066D5F2..0066D5F5
        value = *slot<VecSlot>(object, 0x64)(object, &local);           // 0066D5F7..0066D609
    }
    *out = value;                                                       // 0066D618..0066D624
    return out;                                                         // 0066D628
}
// 005F5180 CSWCameraOnAStick::GetTargetHeadPosition(out), RET 4: target [this+14h] slot64 position, z += [this+6Ch].
extern "C" Vec* __fastcall kotor_fastcall_005f5180(void* camera, void*, Vec* out) {
    Vec local;
    void* target = pw(camera, 0x14);                                    // 005F5186
    slot<VecSlot>(target, 0x64)(target, &local);                        // 005F5189..005F5190 (result read from local)
    const float z = static_cast<float>(static_cast<double>(local.z) + f(camera, 0x6c)); // 005F5193..005F51A6 fstp dword
    out->x = local.x; out->y = local.y; out->z = z;                     // 005F519E..005F51B5
    return out;
}
// 005D5DE0 CExoSound::SetListenerOrientation(front, up), RET 8 / 005D5DF0 SetListenerPosition(pos), RET 4.
extern "C" void __fastcall kotor_fastcall_005d5de0(void* sound, void*, Vec* front, Vec* up) {
    if (void* internal = pw(sound, 0)) kotor_sound_path_required::listener_orientation_005d6540(internal, nullptr, front, up); // 005D5DE0..005D5DE6
}
extern "C" void __fastcall kotor_fastcall_005d5df0(void* sound, void*, Vec* position) {
    if (void* internal = pw(sound, 0)) kotor_sound_path_required::listener_position_005d6600(internal, nullptr, position); // 005D5DF0..005D5DF6
}

// 005F5370 CClientExoAppInternal::UpdateSoundEngine.
extern "C" void __fastcall frame_UpdateSound_005f5370(menu_continuation::Client* client_internal, void*) {
    void* const self = client_internal;
    using kotor_sound_listener_cells::listener_blend_007a22a0;
    void* module = pw(self, 0x18);                                      // 005F5376
    if (module) {                                                       // 005F5379..005F537B
        void* camera = pw(module, 0x40);                                // 005F5389
        void* creature;
        if (w(self, 0x3bc) == w(self, 0x20)) {                          // 005F5381..005F5391
            creature = pw(self, 0x3c0);                                 // 005F5393
        } else {
            const U32 found = frame_R4_005ed5b0(pw(menu_continuation::app_manager_007a39fc, 4), nullptr, w(self, 0x20)); // 005F539B..005F53A5
            sw(self, 0x3c0, found);                                     // 005F53AD
            sw(self, 0x3bc, w(self, 0x20));                             // 005F53AA / 005F53B3
            creature = at(found);
        }
        if (camera) {                                                   // 005F53BB..005F53BD
            union { Vec v; Quat q; } scratch;                           // [esp+30h..3Fh], 16 bytes (next local at [esp+40h]);
                                                                        // camera slot +24h writes a 16-byte quaternion here
            void* minigame = kotor_fastcall_00641980(module, nullptr);  // 005F53C3..005F53C6
            void* follower = minigame ? pw(minigame, 0x24) : nullptr;   // 005F53CB..005F53D4
            bool proceed = true;
            if (follower) kotor_fastcall_0066d5d0(follower, nullptr, &scratch.v); // 005F53D6..005F53DD (result overwritten below)
            else if (!creature) proceed = false;                        // 005F53E4..005F53E6
            if (proceed) {
                Vec position{0.0f, 0.0f, 0.0f};                         // 005F53F5..005F5405 ([esp+0C..14])
                Vec front{0.0f, 0.0f, 0.0f};                            // 005F540D..005F541D ([esp+18..20])
                position = *slot<VecSlot>(camera, 0x20)(camera, &scratch.v); // 005F53EC..005F5435
                const Quat orientation = *slot<QuatSlot>(camera, 0x24)(camera, &scratch.q); // 005F5438..005F5470 ([esp+40..4C])
                const float blend = listener_blend_007a22a0;            // 005F5448
                if (static_cast<double>(blend) < k_zero_0073d700) {     // 005F5450..005F5474 (jp not taken: below, ordered)
                    listener_blend_007a22a0 = 0.0f;                     // 005F5476
                } else if (static_cast<double>(listener_blend_007a22a0) > k_one_0073d764) { // 005F5482..005F5493
                    listener_blend_007a22a0 = 1.0f;                     // 005F5495
                }
                Vec offset;                                             // [esp+24..2C]
                if (void* stick = slot<ObjectSlot>(camera, 0x80)(camera, 0x106a)) { // 005F549F..005F54B0
                    const Vec head = *kotor_fastcall_005f5180(stick, nullptr, &scratch.v); // 005F54B2..005F54E2
                    const double dx = static_cast<double>(head.x) - position.x;  // 005F54C7..005F54CB
                    const double dy = static_cast<double>(head.y) - position.y;  // 005F54D6..005F54DA
                    const float dz = static_cast<float>(static_cast<double>(head.z) - position.z); // 005F54E2..005F54F3 fstp dword
                    offset.x = static_cast<float>(dx * listener_blend_007a22a0); // 005F54F9..005F54FF
                    offset.y = static_cast<float>(dy * listener_blend_007a22a0); // 005F5503..005F5509
                    offset.z = static_cast<float>(static_cast<double>(dz) * listener_blend_007a22a0); // 005F550D..005F5517
                    vector_add_assign_004a9740(&position, nullptr, &offset); // 005F54E6..005F551B
                } else if (slot<ObjectSlot>(camera, 0x80)(camera, 0x1070) != nullptr) { // 005F5525..005F5536
                    void* character = frame_GetCharacter_006346c0(pw(self, 0x270), nullptr, 0); // 005F5538..005F5540
                    Vec base;                                           // [esp+24..2C]
                    std::memcpy(&base, bytes(character) + 0x24, sizeof(Vec)); // 005F5545..005F555A
                    const double height = slot<FloatSlot>(character, 0xd8)(character); // 005F555E..005F5560
                    const double top = height + base.z;                 // 005F5566
                    Vec delta;                                          // [esp+30..38]
                    delta.x = static_cast<float>(static_cast<double>(base.x) - position.x); // 005F556A..005F557D
                    delta.y = static_cast<float>(static_cast<double>(base.y) - position.y); // 005F5585..005F558E
                    delta.z = static_cast<float>(top - position.z);     // 005F5592..005F5596
                    const Vec* scaled = vector_component_mult_00423bf0(&base, &delta, listener_blend_007a22a0); // 005F556E..005F559F
                    vector_add_assign_004a9740(&position, nullptr, scaled); // 005F55A2..005F55A7
                } else {
                    void* app = pw(menu_continuation::app_manager_007a39fc, 4);
                    if (w(at(frame_R4_005ed690(app, nullptr)), 0xb4) != 0 &&                 // 005F55AE..005F55C3
                        w(at(frame_R4_005ed690(pw(menu_continuation::app_manager_007a39fc, 4), nullptr)), 0xe4) != 0) { // 005F55C5..005F55DB
                        const float distance = f(at(frame_R4_005ed690(pw(menu_continuation::app_manager_007a39fc, 4), nullptr)), 0xf4); // 005F55DD..005F55F6
                        const Vec* forward = quaternion_zdir_0043e250(&orientation, nullptr, &scratch.v); // 005F55F9..005F55FE
                        const Vec* scaled = vector_component_mult_00423bf0(&offset, forward, distance); // 005F5603..005F560E
                        vector_subtract_assign_004a9770(&position, nullptr, scaled); // 005F5611..005F5616
                    }
                }
                kotor_fastcall_005d5df0(kotor_cell_sound_007a39ec, nullptr, &position); // 005F561B..005F5626
                const Vec dir = *quaternion_zdir_0043e250(&orientation, nullptr, &scratch.v); // 005F562B..005F5650
                front.x = static_cast<float>(static_cast<double>(dir.x) * k_minus_one_00740cbc); // 005F563F..005F5653
                front.y = static_cast<float>(static_cast<double>(dir.y) * k_minus_one_00740cbc); // 005F565B..005F566E
                front.z = static_cast<float>(static_cast<double>(dir.z) * k_minus_one_00740cbc); // 005F5672..005F567C
                Vec* up = quaternion_ydir_0044f420(&orientation, nullptr, &scratch.v); // 005F565F..005F5680
                kotor_fastcall_005d5de0(kotor_cell_sound_007a39ec, nullptr, &front, up); // 005F5685..005F5691
            }
        }
    }
    sound_fade29_required::update_005d5dd0(kotor_cell_sound_007a39ec, nullptr); // 005F5698..005F569E
}
