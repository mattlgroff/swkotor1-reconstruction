// Lane W4 G12: 006097F0 CSWCArea::Update and the per-frame CSWCAmbientSound leaves it reaches.
// Transcribed from dis/006097F0.asm and dis/0068EF80.asm (+ leaves). x87 policy: see w4_native.hpp. Every original
// FPU-stack intermediate is a `double`, every `fstp dword` an explicit float store, in the original order.
#include "2a64b89258-w4_frame.hpp"

namespace kotor_reconstruction { struct StartupString; }
namespace menu_resources { struct Ref; }
namespace menu_tables_required {
std::uint32_t __fastcall StreamPlay_005d5b10(void*, void*, menu_resources::Ref*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t); // CExoStreamingSoundSource::Play
std::uint32_t __fastcall StreamIsPlaying_005d5af0(void*, void*);                    // CExoStreamingSoundSource::IsPlaying
}
namespace sound_fade29 { void __fastcall Begin005d5b90(void* source, void*, std::uint32_t duration); }  // FadeAndStop
namespace kotor_sound_internal { std::uint32_t __fastcall SoundGetMode005d6490(void* sound, void*); }  // CExoSound::GetSoundMode
extern "C" {
void __fastcall kotor_fastcall_006735d0(void*,void*,float delta) noexcept(false); // CSWMiniGame::Update, RET4
void __fastcall object_ambient_0068db30(void*, void*, kotor_reconstruction::StartupString*); // PlayStinger, RET4
void __fastcall kotor_fastcall_00606490(void*, void*, float); // complete day/night cycle, RET4
void* __fastcall client_get_player_creature_005ed540(void* app, void*);              // CClientExoApp::GetPlayerCreature
W4Vector* __fastcall quaternion_zdir_0043e250(const void* q, void*, W4Vector* out);   // Quaternion::zdir (RET 4)
W4Vector* __cdecl vector_normalized_004ab180(W4Vector* out, const W4Vector* v);       // normalize
W4Vector* __cdecl vector_cross_004ab210(W4Vector* out, const W4Vector* a, const W4Vector* b); // vectorCrossProduct
W4Vector* __fastcall vector_subtract_assign_004a9770(W4Vector* self, void*, const W4Vector* other); // Vector::operator-=
double __cdecl kotor_m15a_yaw_004aa0f0(const W4Vector* v);                            // Yaw (naked; ST0 at PC53 -> read as double)
void* __cdecl yaw_pitch_roll_004acac0(void* out, float yaw, float pitch, float roll); // YawPitchRoll (returns out)
std::int32_t __cdecl crt_rand_006fc447();                                             // rand
int __cdecl format_006fadb0(char*, const char*, ...);                                 // sprintf
void* __fastcall kotor_fastcall_005ed510(void* app, void*);                           // CClientExoApp::GetAnimationTimer
void* __fastcall kotor_fastcall_005ed500(void* app, void*);                           // CClientExoApp::GetPauseTimer
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, std::uint32_t* day, std::uint32_t* time); // GetWorldTime
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, void* string); // CResRef::CResRef(CExoString const&)
// Cross-lane pending (W1 cross-cutting leaf; donor: server-runtime-source item_ai_recharge_native.cpp).
std::uint32_t __fastcall recovered_world_time_difference_004ae460(void* timer, void*, std::uint32_t day1, std::uint32_t time1,
                                                                   std::uint32_t day2, std::uint32_t time2,
                                                                   std::uint32_t* out_day, std::uint32_t* out_time);
// W4-defined in this unit.
void __fastcall kotor_fastcall_0068ef80(void* ambient, void*);                       // CSWCAmbientSound::Update
std::uint32_t __fastcall kotor_fastcall_005d64e0(void* sound, void*);                // CExoSound::GetLostFocusPaused
std::uint32_t __fastcall kotor_fastcall_0068dcc0(void* ambient, void*);              // CSWCAmbientSound::GetTimeDiff
void __fastcall kotor_fastcall_0068dc90(void* ambient, void*);                       // CSWCAmbientSound::ResetLastUpdate
void __fastcall kotor_fastcall_0068dd30(void* ambient, void*, std::uint32_t enable, std::uint32_t kind); // PlayTruncated
void __fastcall kotor_fastcall_006053c0(void* area, void*);                          // CSWCArea::SetLightningDone
}

using namespace w4;

namespace {
struct Quat { float x, y, z, w; };
static_assert(sizeof(Quat) == 16, "Quaternion");
const float k15_00747f6c = 15.0f, k10_007455c8 = 10.0f, k5_00743264 = 5.0f, kmilli_0073d708 = 0.001f;
const float ktenth_0073e798 = 0.1f, k04_0073e320 = 0.4f, k2p32_0073d770 = 4294967296.0f;
const float k1_6000_0074efc8 = 1.0f / 6000.0f, k100_00741838 = 100.0f, kzero_0073d700 = 0.0f;
static_assert(sizeof(float) == 4, "float");
const char flash_format_0074efcc[] = "flash0%i";                                      // .rdata 0074EFCC
// fild dword (signed) [+ 2^32 when negative] => unsigned DWORD as double (exact; < 2^33).
double fild_unsigned(U32 v) {
    double d = static_cast<double>(static_cast<I32>(v));

    if (static_cast<I32>(v) < 0) d += static_cast<double>(k2p32_0073d770);



    return d;
}
}
// Cells (single owner here; C linkage so W3's CSWCArea::SetAsDay/SetAsNight/SetAsDusk/SetAsDawn and
// UpdateDayNightCycle, the only other writers of [0078E408], bind to the same storage).
extern "C" {
std::uint32_t kotor_cell_00832c70 = 0;   // [00832C70]: written 00609EEA, read 006053C1 (SetLightningDone)
float kotor_cell_0078e408 = 0.5f;        // [0078E408]: .data initial 3F000000
}

// 006097F0 CSWCArea::Update(float) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_006097f0(void* area, void*, float delta_arg) {
    // ---- 006097F0..0060988E: elapsed world time since the previous update ----
    U32 day = 0, time = 0;                                                                   // [esp+68h], [esp+6Ch]
    frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr, &day, &time); // 00609801..00609815
    U32 diff_day = 0, diff_ms = 0;                                                           // [esp+B8h], [esp+58h]
    recovered_world_time_difference_004ae460(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr, day, time,
                                             lw(area, 0x1FC), lw(area, 0x200), &diff_day, &diff_ms); // 0060981A..0060984F
    sw(area, 0x1FC, day);                                                                    // 00609866
    sw(area, 0x200, time);                                                                   // 0060986C
    const float dt = static_cast<float>(fild_unsigned(diff_ms) * kmilli_0073d708);           // 00609854..0060988A [esp+2Ch]
    if (lw(area, 0x184) == 0) return;                                                        // 00609880..0060988E

    // ---- 00609894..00609C54: weather emitters around the camera ----
    void* player = client_get_player_creature_005ed540(client_app(), nullptr);              // 00609894..0060989D
    void* camera = kotor_fastcall_005ed560(client_app(), nullptr);                           // 006098A2..006098AD
    if (player != nullptr && camera != nullptr) {                                            // 006098B2..006098BC
        const U8* player_position = static_cast<const U8*>(player) + 0x24;                   // [esp+10h]
        W4Vector anchor;                                                                     // [esp+18h]
        std::memcpy(&anchor, player_position, 12);                                           // 006098C2..006098E3
        anchor.z = static_cast<float>(static_cast<double>(anchor.z) + k15_00747f6c);         // 006098D9..006098EE
        Quat orientation;                                                                    // [esp+CCh]
        W4Vector forward_raw;                                                                // [esp+7Ch]
        vcall<void*, Quat*>(camera, 0x24, &orientation);                                     // 006098F9..00609914 GetOrientation
        const W4Vector* z = quaternion_zdir_0043e250(&orientation, nullptr, &forward_raw);  // 00609917..00609919 (arg pushed at 006098EB)
        W4Vector flat{z->x, z->y, 0.0f};                                                     // [esp+40h]: 0060991E..0060993F (z := 0)
        W4Vector tmp;                                                                        // [esp+7Ch]
        const W4Vector* n = vector_normalized_004ab180(&tmp, &flat);                         // 00609943
        W4Vector forward{n->x, n->y, n->z};                                                  // [esp+40h] 00609948..00609961
        W4Vector up{0.0f, 0.0f, 1.0f};                                                       // [esp+5Ch] 0060996E..00609976
        const W4Vector* c = vector_cross_004ab210(&tmp, &forward, &up);                      // 0060997E
        W4Vector cross{c->x, c->y, c->z};                                                    // [esp+30h] 00609983..006099A0
        const W4Vector* m = vector_normalized_004ab180(&tmp, &cross);                        // 006099A4
        const float side_x = static_cast<float>(static_cast<double>(m->x) * k10_007455c8);   // 006099A9..006099B4 [esp+4Ch]
        const float side_y = static_cast<float>(static_cast<double>(m->y) * k10_007455c8);   // 006099B8..006099C1 [esp+50h]
        const float side_z = static_cast<float>(static_cast<double>(m->z) * k10_007455c8);   // 006099C5..006099DE [esp+54h]
        (void)side_z;                                                                        // stored to [esp+38h], then 0 at 00609A05
        const float fwd10_x = static_cast<float>(static_cast<double>(forward.x) * k10_007455c8); // 006099E2..006099F9 [esp+70h]
        const float fwd10_y = static_cast<float>(static_cast<double>(forward.y) * k10_007455c8); // 00609A01..00609A0F [esp+74h]
        W4Vector side{side_x, side_y, 0.0f};                                                 // [esp+30h]
        vector_subtract_assign_004a9770(&anchor, nullptr, &side);                            // 006099FD..00609A13
        U32 index = 0;                                                                       // [esp+28h]
        for (I32 row = 0; static_cast<short>(row) < 2; ++row) {                              // [esp+3Ch] 00609A18..00609C4E
            for (I32 col = 0; static_cast<short>(col) < 3; ++col, ++index) {                 // bx 00609A20..00609C3B
                void** slot = static_cast<void**>(static_cast<void*>(static_cast<U8*>(area) + 0x18C)) +
                              static_cast<short>(static_cast<U16>(index));                    // 00609A22..00609A30
                if (*slot == nullptr) continue;                                              // 00609A2E..00609A37
                const float yaw = static_cast<float>(kotor_m15a_yaw_004aa0f0(&forward));    // 00609A3D..00609A4B (fstp dword)
                Quat rotation;                                                               // [esp+E8h]
                const Quat* q = static_cast<const Quat*>(yaw_pitch_roll_004acac0(&rotation, yaw, 0.0f, 0.0f)); // 00609A4E..00609A56
                Quat returned;                                                               // [esp+F8h]
                vcall<void*, Quat*, Quat>(*slot, 0x60, &returned, *q);                       // 00609A5B..00609A80 SetOrientation
                W4Vector position, returned_position;
                if (static_cast<short>(col) == 1 && static_cast<short>(row) == 0) {          // 00609A83..00609A93
                    const double a0 = static_cast<double>(fwd10_x) * kzero_0073d700;         // 00609A99..00609A9F
                    const double a1 = static_cast<double>(fwd10_y) * kzero_0073d700;         // 00609AA7..00609AAE
                    const float x = static_cast<float>(static_cast<double>(side.x) + anchor.x); // 00609AB6..00609ACF [esp+A0h]
                    const double y = static_cast<double>(side.y) + anchor.y;                  // 00609ABA..00609AD6 (stays on the FPU)
                    const float zf = static_cast<float>(static_cast<double>(side.z) + anchor.z); // 00609ABE..00609AE5 [esp+A8h]
                    position.x = static_cast<float>(static_cast<double>(x) + a0);             // 00609AEC..00609AF5
                    position.y = static_cast<float>(y + a1);                                  // 00609AFF..00609B01
                    position.z = zf;                                                          // 00609B10..00609B17
                    vcall<void*, W4Vector*, W4Vector>(*slot, 0x5C, &returned_position, position); // 00609B22..00609B2C SetPosition
                } else {
                    const double j = static_cast<double>(static_cast<short>(row));           // 00609B34..00609B40 fild
                    const float r0 = static_cast<float>(static_cast<double>(fwd10_x) * j);   // 00609B44..00609B55 [esp+94h]
                    const float r1 = static_cast<float>(static_cast<double>(fwd10_y) * j);   // 00609B5E..00609B67 [esp+98h]
                    const float r2 = static_cast<float>(j * kzero_0073d700);                 // 00609B6E..00609B74 [esp+9Ch]
                    const double i = static_cast<double>(static_cast<short>(col));           // 00609B7B fild
                    const double cx = static_cast<double>(side.x) * i;                       // 00609B7F..00609B83 (FPU)
                    const float cy = static_cast<float>(static_cast<double>(side.y) * i);    // 00609B85..00609B8B [esp+B0h]
                    const float cz = static_cast<float>(i * static_cast<double>(side.z));    // 00609B92..00609B98 [esp+B4h]



                    const double px = cx + anchor.x;                                          // 00609B9F

                    const float py = static_cast<float>(static_cast<double>(cy) + anchor.y);  // 00609BA3..00609BAE [esp+BCh]
                    const float pz = static_cast<float>(static_cast<double>(cz) + anchor.z);  // 00609BB5..00609BC0 [esp+C0h]
                    const double qx = px + r0;                                                // 00609BC7
                    const double qy = static_cast<double>(py) + r1;                           // 00609BCE..00609BD5
                    const float qz = static_cast<float>(static_cast<double>(pz) + r2);        // 00609BDC..00609BEA [esp+90h]
                    position.x = static_cast<float>(qx);                                      // 00609BF3..00609BFB
                    position.y = static_cast<float>(qy);                                      // 00609BFD..00609C12
                    position.z = static_cast<float>(static_cast<double>(qz) - k5_00743264);   // 00609C01..00609C1D
                    vcall<void*, W4Vector*, W4Vector>(*slot, 0x5C, &returned_position, position); // 00609C20..00609C2A SetPosition
                }
            }
        }

        // ---- 00609C54..00609EFE: lightning ----
        void* lightning = lp(area, 0x1A4);
        if (lightning != nullptr) {                                                          // 00609C54..00609C5C
            bool strike = true;                                                              // edi = 1
            const double flash = lf(area, 0x1E4);
            if (!jcc_jnp(flash, kzero_0073d700, 0x44)) {                                     // 00609C62..00609C78 (flash != 0)
                if (jcc_jp(flash, dt, 0x41)) {                                               // 00609C7A..00609C89
                    strike = false;                                                          // 00609CA4
                    sf(area, 0x1E4, static_cast<float>(flash - dt));                         // 00609C9E..00609CAA
                } else {
                    sw(area, 0x1E4, 0);                                                      // 00609C8D
                    kotor_fastcall_006053c0(area, nullptr);                                  // 00609C8B..00609C97 SetLightningDone
                }
            }
            const double next = lf(area, 0x1E0);
            bool go = true;
            if (!jcc_jnp(next, kzero_0073d700, 0x44)) {                                      // 00609CB0..00609CC1
                if (jcc_jp(next, dt, 0x41)) {                                                // 00609CC3..00609CD2
                    sf(area, 0x1E0, static_cast<float>(next - dt));                          // 00609D9E..00609DA8
                    go = false;                                                              // 00609DAE jmp 00609EFE
                } else {
                    sw(area, 0x1E0, 0);                                                      // 00609CD8
                }
            }
            if (go && strike) {                                                              // 00609CE2..00609CE4
                const I32 r_delay = crt_rand_006fc447() % 5000;                              // 00609CEA..00609CF5
                sf(area, 0x1E0, static_cast<float>(static_cast<double>(r_delay) * kmilli_0073d708 + k5_00743264)); // 00609CFB..00609D0B
                if (crt_rand_006fc447() % 100 < static_cast<I32>(lb(area, 0xAA)) &&           // 00609D11..00609D27
                    crt_rand_006fc447() % 2 != 0) {                                          // 00609D2D..00609D3E
                    W4Vector at;                                                             // [esp+18h]
                    std::memcpy(&at, player_position, 12);                                   // 00609D44..00609D62
                    at.z = static_cast<float>(static_cast<double>(at.z) + k10_007455c8);     // 00609D54..00609D66
                    if (crt_rand_006fc447() % 2 != 0)                                        // 00609D6A..00609D7B
                        at.x = static_cast<float>(static_cast<double>(crt_rand_006fc447() % 100) * ktenth_0073e798 + at.x); // 00609D7D..00609DD2
                    else
                        at.x = static_cast<float>(static_cast<double>(at.x) - static_cast<double>(crt_rand_006fc447() % 100) * ktenth_0073e798); // 00609DB3..00609DD2
                    if (crt_rand_006fc447() % 2 != 0)                                        // 00609DD6..00609DE7
                        at.y = static_cast<float>(static_cast<double>(crt_rand_006fc447() % 100) * ktenth_0073e798 + at.y); // 00609DE9..00609E2F
                    else
                        at.y = static_cast<float>(static_cast<double>(at.y) - static_cast<double>(crt_rand_006fc447() % 100) * ktenth_0073e798); // 00609E0A..00609E2F
                    vcall<void, void*>(lp(area, 0x1A4), 0x4C, lp(area, 0x184));             // 00609E29..00609E3C AttachToScene
                    W4Vector returned_position;
                    vcall<void*, W4Vector*, W4Vector>(lp(area, 0x1A4), 0x5C, &returned_position, at); // 00609E3F..00609E68 SetPosition
                    char name[32];                                                           // [esp+BCh]
                    format_006fadb0(name, flash_format_0074efcc, crt_rand_006fc447() % 3 + 1); // 00609E6B..00609E87
                    U32 length_out = 0, flash_out = 0;                                       // [esp+28h], [esp+24h]
                    vcall<U32, char*, U32*, U32*>(lp(area, 0x1A4), 0x1C, name, &length_out, &flash_out); // 00609E8C..00609EA9 GetAnimation
                    sw(area, 0x1E4, flash_out);                                              // 00609EAC..00609EBA
                    vcall<U32, char*, float, U32, U32>(lp(area, 0x1A4), 0x18, name, 1.0f, 3, 0); // 00609EB0..00609ECF PlayAnimation
                    kotor_cell_00832c70 = f_bits(kotor_cell_0078e408);          // 00609ED8..00609EEA
                    kotor_cell_0078e408 = static_cast<float>(static_cast<double>(kotor_cell_0078e408) * k04_0073e320); // 00609ED2..00609EF0
                    (void)frame_R4_005ed690(client_app(), nullptr);                          // 00609EE4..00609EF9 (result unused)
                }
            }
        }
    }

    // ---- 00609EFE..00609F5A: weather sound timer ----
    const U8 weather = lb(area, 0xAB);
    if (weather == 1 || weather == 2) {                                                      // 00609EFE..00609F0C
        const double t = lf(area, 0x1F8);



        if (jcc_jp(t, dt, 0x41)) {                                                           // 00609F0E..00609F1D

            sf(area, 0x1F8, static_cast<float>(t - dt));                                     // 00609F50..00609F5A
        } else {
            (void)crt_rand_006fc447();                                                       // 00609F28 (result discarded)
            const I32 r = crt_rand_006fc447() % 5000;                                        // 00609F2D..00609F38
            sf(area, 0x1F8, static_cast<float>(static_cast<double>(r) * kmilli_0073d708 + k5_00743264)); // 00609F3E..00609F5A
        }
    }
    // ---- 00609F60..00609F8E: day/night ----
    if (lw(area, 0xA0) != 0)                                                                 // 00609F60..00609F6A
        kotor_fastcall_00606490(area, nullptr, dt);
    if (lw(area, 0xA4) != 0 && lw(area, 0x230) != 0) sw(area, 0x230, 0);                     // 00609F78..00609F88
    // ---- 00609F8E..0060A067: weather transition ----
    const U32 remaining = lw(area, 0x1EC);
    if (remaining != 0) {                                                                    // 00609F94..00609F96 (jbe)
        if (remaining <= diff_ms) {                                                          // 00609F9C..00609FA0 (jbe)
            sw(area, 0x1EC, 0);                                                              // 0060A05B
            vcall<void, U32, float>(area, 0x1C, lb(area, 0x1F1), 100.0f);                   // 0060A04C..0060A064 SetWeather
        } else {
            const double progress = fild_unsigned(6000u - remaining) * k1_6000_0074efc8 * k100_00741838; // 00609FA6..00609FCE
            const U8 target = lb(area, 0x1F1);
            const float percent = static_cast<float>(progress);                              // 00609FD4 fst [esp+28h]
            sf(area, 0x1F4, percent);                                                        // 00609FD8
            if (target == 1) vcall<void, U32, float>(area, 0x1C, 1, percent);                // 00609FDE..0060A035
            else if (target == 2) vcall<void, U32, float>(area, 0x1C, 2, percent);           // 00609FE9..0060A035
            else if (target == 0) {                                                          // 00609FF7..00609FF9
                const float back = static_cast<float>(fild_unsigned(remaining) * k1_6000_0074efc8 * k100_00741838); // 00609FFB..0060A021
                sf(area, 0x1F4, back);                                                       // 0060A029
                vcall<void, U32, float>(area, 0x1C, lb(area, 0x1F0), back);                  // 0060A02F..0060A035
            }
            sw(area, 0x1EC, lw(area, 0x1EC) - diff_ms);                                      // 0060A038..0060A044
        }
    }
    // ---- 0060A067..0060A0DB: ambient sound, scene animation, minigame ----
    kotor_fastcall_0068ef80(lp(area, 0x1DC), nullptr);                                       // 0060A067..0060A06D
    frame_R4_004adbd0(kotor_fastcall_005ed510(client_app(), nullptr), nullptr);             // 0060A072..0060A082

    auto* animation_timer = static_cast<menu_continuation::WorldTimer*>(kotor_fastcall_005ed510(client_app(), nullptr)); // 0060A097
    const I64 ticks = animation_timer->GetSnapshotTimeDifference_004adc80();                 // 0060A09E
    const float scene_delta = static_cast<float>(static_cast<double>(ticks) * menu_continuation::scale_007439bc); // 0060A0AB..0060A0BC
    vcall<void, float>(lp(area, 0x184), 0x1C, scene_delta);                                  // 0060A0B0..0060A0BF Scene vtable +1Ch

    void* mini = lp(area, 0x264);                                           // 0060A0C2 single load
    if (mini != nullptr)                                                     // 0060A0C8..0060A0CA
        kotor_fastcall_006735d0(mini, nullptr, delta_arg);                     // 0060A0CC..0060A0D6
}

// 006053C0 CSWCArea::SetLightningDone() (thiscall, RET 0).
extern "C" void __fastcall kotor_fastcall_006053c0(void* area, void*) {
    std::memcpy(&kotor_cell_0078e408, &kotor_cell_00832c70, 4);                              // 006053C1..006053C9 (raw DWORD copy)
    const I32 r = crt_rand_006fc447() % 5000;                                                // 006053CE..006053D9
    void* lightning = lp(area, 0x1A4);                                                       // 006053DB
    sf(area, 0x1E0, static_cast<float>(static_cast<double>(r) * kmilli_0073d708 + k5_00743264)); // 006053E7..006053F7
    if (lightning != nullptr) vcall<void, void*>(lightning, 0x4C, nullptr);                  // 006053FE..00605404 AttachToScene(0)
}

// 005D64E0 CExoSound::GetLostFocusPaused() (thiscall, RET 0).
extern "C" U32 __fastcall kotor_fastcall_005d64e0(void* sound, void*) {
    void* internal = lp(sound, 0);                                                           // 005D64E0
    return internal ? lw(internal, 0x144) : 0;                                               // 005D64E2..005D64EF
}

// 0068DC90 CSWCAmbientSound::ResetLastUpdate() (thiscall, RET 0).
extern "C" void __fastcall kotor_fastcall_0068dc90(void* ambient, void*) {
    frame_GetWorldTime_004ade40(kotor_fastcall_005ed500(client_app(), nullptr), nullptr,
                                static_cast<U32*>(static_cast<void*>(static_cast<U8*>(ambient) + 0xD4)),
                                static_cast<U32*>(static_cast<void*>(static_cast<U8*>(ambient) + 0xD8))); // 0068DC90..0068DCAF
}

// 0068DCC0 CSWCAmbientSound::GetTimeDiff() (thiscall, RET 0; returns the millisecond difference).
extern "C" U32 __fastcall kotor_fastcall_0068dcc0(void* ambient, void*) {
    U32 day = 0, time = 0;                                                                   // [esp+8h], [esp+4h]
    frame_GetWorldTime_004ade40(kotor_fastcall_005ed500(client_app(), nullptr), nullptr, &day, &time); // 0068DCC0..0068DCDF
    U32 out_day = 0, out_time = 0;                                                           // [esp+10h], [esp+0Ch]
    recovered_world_time_difference_004ae460(kotor_fastcall_005ed500(client_app(), nullptr), nullptr, day, time,
                                             lw(ambient, 0xD4), lw(ambient, 0xD8), &out_day, &out_time); // 0068DCE4..0068DD16
    return out_time;                                                                         // 0068DD1B
}

// 0068DD30 CSWCAmbientSound::PlayTruncated(int enable, int kind) (thiscall, RET 8).
extern "C" void __fastcall kotor_fastcall_0068dd30(void* ambient, void*, U32 enable, U32 kind) {
    if (kind == 1) sw(ambient, 0x04, enable);                                                // 0068DD40..0068DD57
    else if (kind == 2) sw(ambient, 0x1C, enable);                                           // 0068DD45..0068DD52
    else if (kind == 4) sw(ambient, 0x14, enable);                                           // 0068DD48..0068DD4D
    void* stream = lp(ambient, 0xE0);
    sw(ambient, 0xD0, 10);                                                                   // 0068DD60
    if (menu_tables_required::StreamIsPlaying_005d5af0(stream, nullptr) != 0) {              // 0068DD5A..0068DD71
        const I32 playing = li(ambient, 0x100);
        if (static_cast<I32>(kind) > playing) {                                              // 0068DD73..0068DD7B
            if (enable == 0) return;                                                         // 0068DD7D..0068DD7F
            sound_fade29::Begin005d5b90(lp(ambient, 0xE0), nullptr, 1000);                   // 0068DD85..0068DD90
            return;
        }
        if (static_cast<I32>(kind) != playing) return;                                       // 0068DD9E
        if (enable != 0) return;                                                             // 0068DDA4..0068DDA6
        sound_fade29::Begin005d5b90(lp(ambient, 0xE0), nullptr, 1000);                       // 0068DDAC..0068DDB7
        if (kind != 4) return;                                                               // 0068DDBC..0068DDBF
        object_ambient_0068db30(ambient, nullptr, reinterpret_cast<kotor_reconstruction::StartupString*>(static_cast<U8*>(ambient) + 0x6C));
    }
    if (enable == 0) return;                                                                 // 0068DDD9..0068DDDB
    U32 ref[4];                                                                              // [esp+0Ch] CResRef (16 bytes)
    U32 source_offset, mode, state;
    if (lw(ambient, 0x14) != 0) { source_offset = 0x6C; mode = 8; state = 4; }               // 0068DDE1..0068DE1C
    else if (lw(ambient, 0x1C) != 0) {                                                       // 0068DE2E..0068DE33
        source_offset = lw(ambient, 0xF8) != 0 ? 0x8C : 0xAC; mode = 4; state = 2;           // 0068DE35..0068DEA1
    } else if (lw(ambient, 0x04) != 0) {                                                     // 0068DEA3..0068DEA8
        source_offset = lw(ambient, 0xF8) != 0 ? 0x2C : 0x4C; mode = 8; state = 1;           // 0068DEAA..0068DF14
    } else {
        sw(ambient, 0xD0, 10000);                                                            // 0068DF16
        sw(ambient, 0x100, 0);                                                               // 0068DF20
        return;
    }
    async_ref_construct_string_00406d60(ref, nullptr, static_cast<U8*>(ambient) + source_offset); // CResRef::CResRef
    if (menu_tables_required::StreamPlay_005d5b10(lp(ambient, 0xE0), nullptr,
                                                  reinterpret_cast<menu_resources::Ref*>(ref), mode, 0, 1, 0) == 0)
        sw(ambient, 0xD0, 10000);                                                            // 0068DE11 / 0068DE6B / 0068DEDD
    sw(ambient, 0x100, state);                                                               // 0068DE1C / 0068DE76 / 0068DEE8
}

// 0068EF80 CSWCAmbientSound::Update() (thiscall, RET 0).
extern "C" void __fastcall kotor_fastcall_0068ef80(void* ambient, void*) {
    void* sound = pt(frame_control::sound_manager_007a39ec);
    const U32 mode_paused_0074c5f8 = 3, mode_cutscene_0074c5fc = 4;                          // .rdata 0074C5F8 / 0074C5FC
    if (menu_tables_required::StreamIsPlaying_005d5af0(lp(ambient, 0xE0), nullptr) == 0 &&   // 0068EF83..0068EF90
        kotor_sound_internal::SoundGetMode005d6490(sound, nullptr) != mode_paused_0074c5f8 &&  // 0068EF96..0068EFA7
        kotor_fastcall_005d64e0(sound, nullptr) == 0) {                                      // 0068EFAD..0068EFBA
        const U32 wait = lw(ambient, 0xD0);
        bool reset = false;
        if (wait != 0) {                                                                     // 0068EFC0..0068EFC8 (jbe)
            const U32 elapsed = kotor_fastcall_0068dcc0(ambient, nullptr);                   // 0068EFCA..0068EFCC
            const U32 current = lw(ambient, 0xD0);
            if (elapsed >= current) {                                                        // 0068EFD7..0068EFD9 (jb)
                const U32 truncated = lw(ambient, 0xFC);
                sw(ambient, 0xD0, 0);                                                        // 0068EFE3
                if (truncated != 0) kotor_fastcall_0068dd30(ambient, nullptr, 1, 0);        // 0068F007..0068F00B
                else if (lw(ambient, 0x14) != 0) vcall<void, U32>(ambient, 0x10, 1);         // 0068EFF1..0068EFFC
                else vcall<void, U32>(ambient, 0x00, 1);                                     // 0068F001..0068F003
            } else {
                sw(ambient, 0xD0, current - elapsed);                                        // 0068F012..0068F014
                reset = true;                                                                // 0068F01A jmp 0068F069
            }
        } else if (lw(ambient, 0x04) != 0 || lw(ambient, 0x14) != 0) {                      // 0068F01C..0068F028
            if (lw(ambient, 0xFC) != 0) sw(ambient, 0xD0, 10);                               // 0068F02A..0068F05F
            else if (lw(ambient, 0x14) != 0) sw(ambient, 0xD0, 1000);                        // 0068F034..0068F03B
            else {
                const U32 base = lw(ambient, 0xCC);                                          // 0068F047
                sw(ambient, 0xD0, base > 1 ? base : 1);                                      // 0068F04D..0068F057
            }
            reset = true;
        }
        if (reset) kotor_fastcall_0068dc90(ambient, nullptr);                                // 0068F069..0068F06B
    }
    if (lw(ambient, 0xFC) != 0) return;                                                      // 0068F070..0068F078
    if (menu_tables_required::StreamIsPlaying_005d5af0(lp(ambient, 0xE4), nullptr) != 0) return; // 0068F07A..0068F087
    if (kotor_sound_internal::SoundGetMode005d6490(sound, nullptr) == mode_paused_0074c5f8) return; // 0068F089..0068F09A
    if (kotor_sound_internal::SoundGetMode005d6490(sound, nullptr) == mode_cutscene_0074c5fc) return; // 0068F09C..0068F0AD
    if (lw(ambient, 0x1C) == 0) return;                                                      // 0068F0AF..0068F0B4
    if (lw(ambient, 0x108) != 0) return;                                                     // 0068F0B6..0068F0BE
    if (kotor_fastcall_005d64e0(sound, nullptr) != 0) return;                                // 0068F0C0..0068F0CD
    vcall<void, U32>(ambient, 0x18, 1);                                                      // 0068F0CF..0068F0D5
}
