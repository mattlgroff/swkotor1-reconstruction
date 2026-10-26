// Lane W4 G13: 0061AD60 CSWCCreature::AIUpdate and its small client leaves.
// Transcribed from the original swkotor.exe (dis/0061AD60.asm and per-callee listings); statements cite PCs.
#include "a5e2cda8f5-creature.hpp"

using namespace w4;

// W4-owned .data cells (never written by the original; see creature.hpp).
extern "C" {
float kotor_cell_drive_run_ratio_007a22f4 = 0.6f;
float kotor_cell_wind_strength_007a231c = 2.5f;
float kotor_cell_turn_rate_007a25f4 = 900.0f;
}

namespace w4c {
// Original callers keep Yaw/Pitch's ST0 (PC53) result unrounded; capture it exactly as double.
double yaw_st0(const void* vector) {
    double r; void* fn = reinterpret_cast<void*>(&kotor_m15a_yaw_004aa0f0);
    __asm { mov eax, vector }
    __asm { push eax }
    __asm { call fn }
    __asm { add esp, 4 }
    __asm { fstp r }
    return r;
}
double pitch_st0(const void* vector) {
    double r; void* fn = reinterpret_cast<void*>(&kotor_m15a_pitch_004aa130);
    __asm { mov eax, vector }
    __asm { push eax }
    __asm { call fn }
    __asm { add esp, 4 }
    __asm { fstp r }
    return r;
}
double anim_length_st0(void* object, U32 anim) {
    double r; void* fn = reinterpret_cast<void*>(&kotor_fastcall_0063c0e0);
    __asm { mov eax, anim }
    __asm { push eax }
    __asm { mov ecx, object }
    __asm { call fn }
    __asm { fstp r }
    return r;
}
}  // namespace w4c
using namespace w4c;

namespace {
const float one_0073d764 = 1.0f;
}

// 0063C0E0 CSWCObject::GetAnimationLength(anim) (thiscall, RET 4): naked so the anim base's ST0 result is
// passed through untouched (tail jump), exactly like the original.
extern "C" __declspec(naked) float __fastcall kotor_fastcall_0063c0e0(void*, void*, U32) {
    __asm {
        mov ecx, dword ptr [ecx + 0x68]      // 0063C0E0
        test ecx, ecx                        // 0063C0E3
        jne have_base                        // 0063C0E5
        fld dword ptr [one_0073d764]         // 0063C0E7
        ret 4                                // 0063C0ED
    have_base:
        mov eax, dword ptr [ecx]             // 0063C0F0
        jmp dword ptr [eax + 0x38]           // 0063C0F2
    }
}

// 005ED660 CClientExoApp::GetActiveTimer(id): mov ecx,[ecx+4]; jmp 005F7DC0.
extern "C" void* __fastcall kotor_fastcall_005ed660(void* app, void*, U32 id) {
    return kotor_fastcall_005f7dc0(lp(app, 4), nullptr, id);
}
// 005ED570 CClientExoApp::GetGameObjectArray().
extern "C" void* __fastcall kotor_fastcall_005ed570(void* app, void*) {
    return lp(lp(app, 4), 0x14);                                                             // 005ED570..005ED573
}
// 005ED9F0 CClientExoApp::GetActivePauseState(): mov ecx,[ecx+4]; jmp 005EE910 (W4 main).
extern "C" U8 __fastcall kotor_fastcall_005ed9f0(void* app, void*) {
    return kotor_fastcall_005ee910(lp(app, 4), nullptr);
}
// 00604BC0 CSWCArea::GetIsObjectCorpse(id) (thiscall, RET 4).
extern "C" U32 __fastcall kotor_fastcall_00604bc0(void* area, void*, U32 id) {
    for (U32 i = 0; i < 4; ++i)                                                              // 00604BC4..00604BDB
        if (lw(area, 0x268 + 4 * i) == id) return 1;                                         // 00604BD0..00604BE2
    return 0;                                                                                // 00604BDD
}
// 005D5930 CExoSoundSource::Play() (thiscall, RET 0): EAX = the internal Play's EAX, [internal+58h] (the play
// length in ms), or 0 when [this+4] is null. CSWCSoundObject::AIUpdate stores it (0066BD1E / 0066BEA4); the W4
// callers here discard it (0061B179, 0061081B).
extern "C" unsigned __fastcall kotor_fastcall_005d5930(void* source, void*);
namespace kotor_sound_path_required { unsigned __fastcall source_play_005db4d0(void*, void*, unsigned, unsigned); }
extern "C" unsigned __fastcall kotor_fastcall_005d5930(void* source, void*) {
    void* internal = lp(source, 4);                                                          // 005D5930
    if (internal == nullptr) return 0;                                                       // 005D5933..005D5935, 005D5941
    return kotor_sound_path_required::source_play_005db4d0(internal, nullptr, 0, 0);         // 005D5937..005D5940
}
// 005D5A30 CExoSoundSource::SetFixedVariance(float) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_005d5a30(void* source, void*, float variance) {
    void* internal = lp(source, 4);                                                          // 005D5A30
    if (internal == nullptr) return;                                                         // 005D5A33..005D5A35
    kotor_m15_source_internal_set_fixed_variance_005db980(internal, nullptr, variance);      // 005D5A37 jmp
}

// 00610A70 CSWCCreature::PlayDrawAnimation(anim) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_00610a70(void* c, void*, U32 anim) {
    if (kotor_fastcall_005edc10(client_app(), nullptr) != 0) {                               // 00610A70..00610A82
        std::memcpy(static_cast<U8*>(c) + 0x444, &anim, 2);                                 // 00610A84..00610A89
        return;
    }
    kotor_fastcall_0063c760(c, nullptr, anim, 0x5DC, 1.0f, 0, 0);                           // 00610A94..00610AA9
    std::uint16_t none = 0xFFFF; std::memcpy(static_cast<U8*>(c) + 0x444, &none, 2);         // 00610AAE
}

// 0060F0F0 CSWCCreature::IsDriveAnimationBlocked() (thiscall, RET 0).
extern "C" U32 __fastcall kotor_fastcall_0060f0f0(void* c, void*) {
    U32 blocked = lw(c, 0x13C);                                                              // 0060F100
    const U32 sid = kotor_fastcall_004aea30(server_app(), nullptr, lw(c, 4));                // 0060F0F3..0060F107
    if (kotor_fastcall_004aeae0(server_app(), nullptr, sid) != 0) blocked = 1;               // 0060F10C..0060F11F
    if (kotor_fastcall_0063c4a0(c, nullptr, lh(c, 0x48)) != 0) return 1;                     // 0060F124..0060F139
    return blocked;                                                                          // 0060F13B
}

// 00614B60 CSWCCreature::IsInjured() (thiscall, RET 0): tail jumps to CSWSCreature::IsInjured.
extern "C" U32 __fastcall kotor_fastcall_00614b60(void* c, void*) {
    if (kotor_m15a_object_get_server_object_0063d4b0(c, nullptr) == nullptr)                 // 00614B63..00614B6A
        return recovered_creature_is_injured_004eff30(nullptr, nullptr);                                    // 00614B82..00614B87
    void* s = vcall<void*>(kotor_m15a_object_get_server_object_0063d4b0(c, nullptr), 0x30);  // 00614B6C..00614B77
    return recovered_creature_is_injured_004eff30(s, nullptr);                                              // 00614B7D
}

// 00614A30 CSWCCreature::ForceAppearanceUpdate() (thiscall, RET 0).
extern "C" void __fastcall kotor_fastcall_00614a30(void* c, void*) {
    sw(c, 0x310, 0);                                                                         // 00614A33
    void* module = kotor_fastcall_005ed530(client_app(), nullptr);                           // 00614A3D..00614A46
    std::memcpy(static_cast<U8*>(lp(c, 0x21C)) + 0x18, static_cast<U8*>(c) + 0x38C, 2);     // 00614A4B..00614A67
    kotor_m15a_creature_set_appearance_006134c0(c, nullptr, static_cast<U8*>(c) + 0x31C, 0xEF, 1); // 00614A58..00614A6E
    if (module == nullptr) return;                                                           // 00614A73..00614A75
    void* area = lp(module, 0x48);                                                           // 00614A77
    if (area == nullptr) return;                                                             // 00614A7A..00614A7C
    vcall<void, U32>(c, 0x94, lw(area, 0x184));                                              // 00614A7E..00614A89 AttachToScene(scene)
}

// 0060E6C0 CSWCCreature::ReAttachCamera() (thiscall, RET 0).
extern "C" void __fastcall kotor_fastcall_0060e6c0(void* c, void*) {
    void* module = kotor_fastcall_005ed530(client_app(), nullptr);                           // 0060E6C0..0060E6CC
    if (lw(c, 0x310) != 0) return;                                                           // 0060E6D3..0060E6DB
    if (client_get_player_creature_005ed540(client_app(), nullptr) != c) return;             // 0060E6DD..0060E6ED
    if (lw(c, 0x314) == 0 || module == nullptr) return;                                      // 0060E6EF..0060E6FB
    void* camera = lp(module, 0x40);                                                         // 0060E6FD
    if (camera == nullptr) return;                                                           // 0060E700..0060E702
    if (vcall<U32, U32>(camera, 0x80, 0x106D) != 0) return;                                  // 0060E704..0060E713 GetBehavior
    if (vcall<U32, U32>(lp(module, 0x40), 0x80, 0x106E) != 0) return;                        // 0060E715..0060E727
    kotor_fastcall_0063f9d0(module, nullptr, lw(c, 4), lw(c, 0x318));                        // 0060E729..0060E736
    sw(c, 0x314, 0);                                                                         // 0060E73B
    sw(c, 0x318, 0);                                                                         // 0060E745
}

// 00611700 CSWCCreature::ComputeWeaponOverlays(int, int) (thiscall, RET 8). Returns 1.
extern "C" U32 __fastcall kotor_fastcall_00611700(void* c, void*, U32 recompute, U32) {
    if (gob_get_property_0x68_0063c070(c, nullptr) == nullptr) return 0;                     // 00611703..0061170D (eax = 0)
    if (recompute != 1) return 1;                                                            // 00611710..00611715
    static const U32 overlay_anims[8] = {0x272E, 0x27AB, 0x2736, 0x27AA, 0x2737, 0x2738, 0x2739, 0x273A};
    for (U32 anim : overlay_anims) {                                                         // 0061171B..006117F1
        void* base = gob_get_property_0x68_0063c070(c, nullptr);
        if (vcall<U32, U32>(base, 0x4C, anim) == 1) return 1;
    }
    if (lw(c, 0x398) != 0) return 1;                                                         // 006117F3..006117FB
    if (lh(c, 0x2D0) == 0) return 1;                                                         // 006117FD..00611805
    (void)kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr,
        kotor_m15a_creature_get_equipped_item_id_0060ca80(c, nullptr, 0x20));               // 00611807..00611819
    if (lw(c, 0x138) == 1) return 1;                                                         // 0061181E..00611825
    (void)gob_get_property_0x68_0063c070(c, nullptr);                                        // 00611827..00611829
    (void)kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr,
        kotor_m15a_creature_get_equipped_item_id_0060ca80(c, nullptr, 0x10));               // 0061182E..00611841
    return 1;                                                                                // 00611846
}

// 0063D270 CSWCObject::SetWind(int on, float strength) (thiscall, RET 8).
extern "C" void __fastcall kotor_fastcall_0063d270(void* o, void*, U32 on, U32 strength_bits) {
    void* model = vcall<void*, U32, U32>(o, 0x98, 0xFF, 0);                                  // 0063D270..0063D27A GetModel(255,0)
    if (model == nullptr) return;                                                            // 0063D282..0063D284
    if (kotor_fastcall_005ed9f0(client_app(), nullptr) != 2 && on != 0) {                    // 0063D286..0063D29E
        vcall<void, U32, U32, U32, U32>(model, 0xD8, 1, strength_bits, 0, 0);                // 0063D2A0..0063D2AF
        return;
    }
    vcall<void, U32, U32, U32, U32>(model, 0xD8, 0, 0, 0, 0);                                // 0063D2B9..0063D2C5
}

// 0062D700 CGuiInGame::IsFireForgetDialogAnimation(anim) / 0062D5E0 IsLoopingDialogAnimation(anim)
// (thiscall, RET 4). Same shape; they differ in the 2DA columns and the fallback id ranges.
namespace {
U32 dialog_animation_check(U32 anim_arg, U32 col_a_cell, U32 col_b_cell, U32 lo, U32 hi) {
    const std::uint16_t anim = static_cast<std::uint16_t>(anim_arg);
    void* rules = kotor_cell_rules_007a3a28;
    if (anim >= 0x2710 && static_cast<I32>(anim) < li(lp(lp(rules, 0xB8), 0x40), 0x24) + 0x2710) {
        // 0062D716..0062D799: rows of the 2DA at [rules+B8h]+40h, row = anim - 2710h.
        const int row = static_cast<std::uint16_t>(anim - 0x2710);
        int a = 0, b = 0;
        if (c2da_current_get_int_entry_00413510(lp(lp(rules, 0xB8), 0x40), nullptr, row, static_cast<int>(col_a_cell), &a) == 0) return 0;
        if (c2da_current_get_int_entry_00413510(lp(lp(kotor_cell_rules_007a3a28, 0xB8), 0x40), nullptr, row,
                                                static_cast<int>(kotor_rules_cell32_007a2190), &b) == 0) return 0;
        return (a != 0 && b != 0) ? 1 : 0;
    }
    // 0062D79C..0062D811: animations 2DA at [rules+B8h]+3Ch, row = anim.
    int a = 0, b = 0;
    if (c2da_current_get_int_entry_00413510(lp(lp(rules, 0xB8), 0x3C), nullptr, anim, static_cast<int>(col_b_cell), &a) != 0 &&
        c2da_current_get_int_entry_00413510(lp(lp(kotor_cell_rules_007a3a28, 0xB8), 0x3C), nullptr, anim,
                                            static_cast<int>(kotor_rules_cell32_007a2170), &b) != 0 &&
        a != 0 && b != 0)
        return 1;
    return (anim >= lo && anim <= hi) ? 1 : 0;
}
}
extern "C" U32 __fastcall kotor_fastcall_0062d700(void*, void*, U32 anim) {
    return dialog_animation_check(anim, kotor_rules_cell32_007a218c, kotor_rules_cell32_007a214c, 0x3E8, 0x52F);
}
extern "C" U32 __fastcall kotor_fastcall_0062d5e0(void*, void*, U32 anim) {
    return dialog_animation_check(anim, kotor_rules_cell32_007a2194, kotor_rules_cell32_007a2154, 0x578, 0x6BF);
}

// 0063C4A0 CSWCObject::AnimationDialog(anim) (thiscall, RET 4).
extern "C" U32 __fastcall kotor_fastcall_0063c4a0(void*, void*, U32 anim) {
    if (kotor_fastcall_0062d700(pt(frame_R4_005ed690(client_app(), nullptr)), nullptr, anim) != 0) return 1; // 0063C4A0..0063C4BC
    if (kotor_fastcall_0062d5e0(pt(frame_R4_005ed690(client_app(), nullptr)), nullptr, anim) != 0) return 1; // 0063C4BE..0063C4D6
    return 0;
}

// 0061AD60 CSWCCreature::AIUpdate() (client creature vtable 0074F5F8 +74h; thiscall, RET 0).
extern "C" void __fastcall kotor_fastcall_0061ad60(void* c, void*) {
    void* app = client_app();                                                                // 0061AD60..0061AD6C (edi)
    U32 day = 0, tick = 0;                                                                   // [esp+14h], [esp+18h]
    frame_GetWorldTime_004ade40(kotor_fastcall_005ed660(app, nullptr, lw(c, 4)), nullptr, &day, &tick); // 0061AD71..0061AD88
    U32 diff_day = 0, diff_tick = 0;                                                         // [esp+20h], [esp+1Ch]
    recovered_world_time_difference_004ae460(frame_GetWorldTimer_005ed4f0(app, nullptr), nullptr, day, tick,
                                             lw(c, 0x3C), lw(c, 0x40), &diff_day, &diff_tick); // 0061AD8D..0061ADB2
    const U32 hold = lw(c, 0xF0);                                                            // 0061ADB7
    sw(c, 0x44, diff_tick);                                                                  // 0061ADC5
    if (hold != 0) sw(c, 0xF0, hold > diff_tick ? hold - diff_tick : 0);                     // 0061ADC8..0061ADD8
    if (lh(c, 0x444) != 0xFFFF && kotor_fastcall_005edc10(client_app(), nullptr) == 0)      // 0061ADDE..0061ADF8
        kotor_fastcall_00610a70(c, nullptr, lh(c, 0x444));                                   // 0061ADFA..0061AE06
    if (lw(c, 0x68) != 0) {                                                                  // 0061AE0B..0061AE13



        if (lh(c, 0x48) == 0x2710) {                                                         // 0061AE19..0061AE1F

            const U32 elapsed = lw(c, 0x390) + lw(c, 0x44);                                  // 0061AE25..0061AE34
            sw(c, 0x390, elapsed);                                                           // 0061AE3A
            if (elapsed >= lw(c, 0x394) &&
                lw(pt(frame_R4_005ed690(client_app(), nullptr)), 0xB4) == 0) {               // 0061AE38..0061AE59
                U32 anim;
                if (lb(lp(c, 0x68), 0xC6) == 1) {                                            // 0061AE5F..0061AE68
                    const int r = crt_rand_006fc447() % 7;                                   // 0061AE6A..0061AE75
                    static const U32 fidget[4] = {0x2745, 0x2746, 0x2747, 0x2748};           // jump table 0061B36C
                    anim = (r >= 0 && r <= 3) ? fidget[r] : 0x27A3;                          // 0061AE77..0061AEB3
                } else {
                    const int r = crt_rand_006fc447() % 5;                                   // 0061AE98..0061AEA3
                    anim = r == 0 ? 0x2745u : r == 1 ? 0x2746u : 0x27A3u;                    // 0061AEA5..0061AEB3
                }
                U32 slot = anim;                                                             // [esp+10h] (0061AEC2)
                vcall<void, U32, U32*>(c, 0xA0, anim, &slot);                                // 0061AEB8..0061AEC6 AddFireForgetAnimation
                sw(c, 0x390, 0);                                                             // 0061AECC
                const int r = crt_rand_006fc447() % 4;                                       // 0061AED2..0061AEE2 (signed remainder)
                sw(c, 0x394, static_cast<U32>((r + 3) * 5000));                              // 0061AEE3..0061AEEC
            }
        } else {
            sw(c, 0x390, 0);                                                                 // 0061AEF4
        }
    }
    vcall<void>(c, 0xE8);                                                                    // 0061AEFA..0061AEFE UpdatePopUpBubble
    bool animation_update = true;
    if (lw(c, 0x3A8) == 1 && kotor_fastcall_0060f0f0(c, nullptr) == 0) {                     // 0061AF04..0061AF15
        void* app2 = client_app();                                                           // 0061AF17..0061AF1C
        const U32 pid = kotor_m15_client_get_player_creature_id_005ed550(app2, nullptr);     // 0061AF21
        if (kotor_fastcall_0062dbd0(pt(frame_R4_005ed690(app2, nullptr)), nullptr, pid) == 0) { // 0061AF26..0061AF39
            kotor_fastcall_0061a670(c, nullptr);                                             // 0061AF3F
            kotor_fastcall_00615960(c, nullptr);                                             // 0061AF46
            animation_update = false;
        }
    } else if (lw(c, 0x3A4) != 0) {                                                          // 0061AF50..0061AF56
        const I32 index = recovered_party_index_006349c0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, lw(c, 4)); // 0061AF58..0061AF6C
        if (index == 1 || index == 2) (void)kotor_fastcall_005ed8b0(client_app(), nullptr);  // 0061AF71..0061AF83
        if (lw(c, 0x3A4) != 0 && kotor_m15a_object_get_server_object_0063d4b0(c, nullptr) != nullptr) { // 0061AF88..0061AF99
            void* s = vcall<void*>(kotor_m15a_object_get_server_object_0063d4b0(c, nullptr), 0x30); // 0061AF9B..0061AFA6
            if (s != nullptr && recovered_creature_following_leader_004f7750(s, nullptr) != 0) {                  // 0061AFA9..0061AFB6
                kotor_fastcall_0061a670(c, nullptr);                                         // 0061AFBA
                kotor_fastcall_00615e80(c, nullptr);                                         // 0061AFC1
                kotor_fastcall_0060bdf0(c, nullptr, reinterpret_cast<W4Vector*>(static_cast<U8*>(c) + 0x1DC)); // 0061AFC6..0061AFCF
                animation_update = false;
            }
        }
    }
    if (animation_update) kotor_fastcall_0061a980(c, nullptr);                               // 0061AFD6..0061AFD8
    const U32 id = lw(c, 4);                                                                 // 0061AFDD
    if (id != 0x7F000000u && lw(c, 0x68) != 0 &&
        id == kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr))       // 0061AFE0..0061AFFD
        vcall<void>(lp(c, 0x68), 0xD4);                                                      // 0061AFFF..0061B004
    if (lw(c, 0x50) == 0) {                                                                  // 0061B00A..0061B00D
        enum { USE_UP, USE_NORMAL } choice = USE_UP;
        W4Vector normal;                                                                     // [esp+24h]
        bool take_server_path = true;                                                        // 0061B027 vs 0061B0A3
        void* area = lp(c, 0x20);                                                            // 0061B013
        if (area != nullptr && kotor_fastcall_00604bc0(area, nullptr, lw(c, 4)) == 1) take_server_path = false; // 0061B016..0061B025
        bool steep_path = false;                                                             // 0061B20F
        if (take_server_path) {
            if (kotor_m15a_object_get_server_object_0063d4b0(c, nullptr) == nullptr ||
                vcall<void*>(kotor_m15a_object_get_server_object_0063d4b0(c, nullptr), 0x30) == nullptr) { // 0061B027..0061B046
                steep_path = true;
            } else {
                void* s = kotor_m15a_object_get_server_object_0063d4b0(c, nullptr) ?
                          vcall<void*>(kotor_m15a_object_get_server_object_0063d4b0(c, nullptr), 0x30) : nullptr; // 0061B04C..0061B069
                if (vcall<U32>(s, 0x94) != 1) {                                              // 0061B069..0061B075
                    void* s2 = kotor_m15a_object_get_server_object_0063d4b0(c, nullptr) ?
                               vcall<void*>(kotor_m15a_object_get_server_object_0063d4b0(c, nullptr), 0x30) : nullptr; // 0061B077..0061B094
                    if (recovered_creature_incapacitated_004ef890(s2, nullptr) != 1) steep_path = true; // 0061B094..0061B09D
                }
            }
        }
        if (steep_path) {
            // 0061B20F..0061B261
            if (lw(lp(c, 0x21C), 0x38) != 0) {                                               // 0061B20F..0061B218
                kotor_fastcall_004aeb70(server_app(), nullptr, &normal, reinterpret_cast<W4Vector*>(static_cast<U8*>(c) + 0x24)); // 0061B21E..0061B230
                const double p = pitch_st0(&normal);                                         // 0061B235..0061B23A
                if (!jcc_jne(p, 40.0f, 0x41)) choice = USE_NORMAL;                           // 0061B23F..0061B24D
            }
        } else {
            // 0061B0A3..0061B0EE
            kotor_fastcall_004aeb70(server_app(), nullptr, &normal, reinterpret_cast<W4Vector*>(static_cast<U8*>(c) + 0x24)); // 0061B0A3..0061B0B5
            const double p = pitch_st0(&normal);                                             // 0061B0BA..0061B0BF
            if (!jcc_jp(p, 80.0f, 0x41) && !jcc_jne(p, 65.0f, 0x01)) choice = USE_NORMAL;   // 0061B0C4..0061B0DF
        }
        W4Vector* tilt = choice == USE_NORMAL ? &normal
                                              : reinterpret_cast<W4Vector*>(&kotor_world_los::up_007a19b0); // 0061B0E1 / 0061B0EE..0061B0F2
        kotor_fastcall_0069de90(gob_get_property_0x68_0063c070(c, nullptr), nullptr, tilt,
                                reinterpret_cast<W4Vector*>(static_cast<U8*>(c) + 0x30)); // 0061B0F7..0061B100
    }
    kotor_fastcall_0060bb20(c, nullptr);                                                     // 0061B105..0061B107
    vcall<void>(c, 0xC4);                                                                    // 0061B10C..0061B110 UpdateVisualEffectsList
    {
        U32 d = 0, t = 0;                                                                    // [esp+14h], [esp+18h]
        frame_GetWorldTime_004ade40(kotor_fastcall_005ed660(client_app(), nullptr, lw(c, 4)), nullptr, &d, &t); // 0061B116..0061B134
        sw(c, 0x40, t);                                                                      // 0061B141
        sw(c, 0x3C, d);                                                                      // 0061B14C
    }
    if (lw(c, 0x17C) != 0 && lw(c, 0x180) == 0) {                                            // 0061B144..0061B157
        kotor_m15_sound_source_set_position_005d59e0(lp(c, 0x17C), nullptr, static_cast<U8*>(c) + 0x24, 1.5f); // 0061B159..0061B162
        kotor_m15_sound_source_set_3d_005d5910(lp(c, 0x17C), nullptr, 1);                    // 0061B167..0061B16E
        kotor_fastcall_005d5930(lp(c, 0x17C), nullptr);                                      // 0061B173..0061B179
        sw(c, 0x180, 1);                                                                     // 0061B17E
    }
    const U32 delta = lw(c, 0x44);                                                           // 0061B18A
    const I32 t154 = static_cast<I32>(lw(c, 0x154) - delta);                                 // 0061B184..0061B193
    sw(c, 0x154, static_cast<U32>(t154));
    if (t154 <= 0) sw(c, 0x154, 0x1770);                                                     // 0061B199..0061B19B
    const I32 t158 = static_cast<I32>(lw(c, 0x158) - delta);                                 // 0061B1A5..0061B1B1
    sw(c, 0x158, static_cast<U32>(t158));
    if (t158 <= 0) {                                                                         // 0061B1B7
        sw(c, 0x158, 0x7D0);                                                                 // 0061B1BD
        (void)kotor_fastcall_00611700(c, nullptr, 1, 1);                                     // 0061B1B9..0061B1C7
    }
    if (kotor_cell_007a2484 != 0) {                                        // 0061B1CC..0061B1D2
        (void)kotor_fastcall_005ed530(client_app(), nullptr);                                // 0061B1D8..0061B1E0 (result unused)
        U32 pending = lw(c, 0x310);                                                          // 0061B1E5
        if (pending != 0) {                                                                  // 0061B1EB..0061B1ED
            if (pending == 1) sw(c, 0x310, 0);                                               // 0061B1EF..0061B1F3
            pending = lw(c, 0x310);                                                          // 0061B1F9
            if (pending > lw(c, 0x44)) sw(c, 0x310, pending - lw(c, 0x44));                  // 0061B1FF..0061B268
            else kotor_fastcall_00614a30(c, nullptr);                                        // 0061B206..0061B208
        }
        kotor_fastcall_0060e6c0(c, nullptr);                                                 // 0061B26E..0061B270
    }
    if (kotor_fastcall_0063df60(c, nullptr) != 0) {                                          // 0061B275..0061B27E
        kotor_m15e_client_ai_list_remove_object_00604810(
            static_cast<U8*>(kotor_m15e_client_get_client_ai_master_005ed6b0(client_app(), nullptr)) + 4, nullptr, c); // 0061B280..0061B292
        U32 removed = 0;                                                                     // [esp+10h]
        const U8 status = recovered_object_array_remove_004d81a0(
            kotor_fastcall_005ed570(client_app(), nullptr), nullptr, lw(c, 4), &removed);    // 0061B297..0061B2B0
        if (status == 0 /* byte [0074666C] */ && removed != 0) {                             // 0061B2B5..0061B2C1
            vcall<void, U32>(c, 0x00, 1);                                                    // 0061B2C3..0061B2C8 scalar deleting dtor
            return;
        }
    }
    // 0061B2D2..0061B2D4: call 004015A0 (void_return) with ECX=this: no effect.
    bool wind = false;
    if (lb(c, 0x90) == 1 && lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x71) == 2) { // 0061B2D9..0061B2F3
        wind = true;
    } else if (lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x71) == 1) {             // 0061B2F5..0061B306
        const U32 self_id = lw(c, 4);                                                        // 0061B310
        wind = kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr) == self_id; // 0061B308..0061B31A
    }
    if (wind) kotor_fastcall_0063d270(c, nullptr, 1, f_bits(kotor_cell_wind_strength_007a231c)); // 0061B31C..0061B32A
    else kotor_fastcall_0063d270(c, nullptr, 0, 0);                                          // 0061B326..0061B32A
    if (lp(c, 0x68) != nullptr) {                                                            // 0061B32F..0061B334
        const U32 self_id = lw(c, 4);                                                        // 0061B33F
        const U32 pid = kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr); // 0061B336..0061B344
        vcall<void, U32>(lp(c, 0x68), 0xC4, self_id == pid ? 1u : 0u);                       // 0061B349..0061B354
    }
    kotor_fastcall_00610840(c, nullptr);                                                     // 0061B35A..0061B35C
}
