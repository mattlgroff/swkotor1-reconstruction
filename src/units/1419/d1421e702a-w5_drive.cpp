// W5 G16 keyboard drive closure, reconstructed from swkotor.exe (GOG) read-only disassembly.
// CSWSCreature::AIActionDrive 0051E6A0, DriveUpdateLocation 0051CD20, GetRunRate 004F1BE0, GetWalkRate 004F1B20,
// TerminateClientSidePath 005110B0, CSWSPlayer::GetLastUpdateObject 00561820, CServerExoApp::GetDoorByGameObjectID
// 004AE7C0 / 004B1920, CSWSDoor::GetIsLinked 005890D0, CSWSArea::GetDoorCrossed 005064D0,
// CSWSCreature::AutoCloseGUIPanels 004EDED0. BumpFriends 004F6390 is the t2 body (tiers/t2/source/w5_precalc.cpp; trap fallback in w5_foreign_traps.cpp). Explicit traps: CloseInventory 0055D800/00587560.
#include "ab3fc15780-w5_drive.hpp"
#include "e5e3766b7c-w5_subarea.hpp"
#include "71303376b8-w5_action_queue.hpp"
#include "294ff121b7-w5_server_env.hpp"
#include <cmath>

using namespace w5;

namespace {
constexpr float k_rate_min_0073f400 = 0.125f;
constexpr float k_rate_max_0073f414 = 1.5f;
constexpr float k_thousand_0073d6fc = 1000.0f;
constexpr float k_walk_floor_00741838 = 100.0f;
constexpr float k_zero_0073d700 = 0.0f;
constexpr float k_strafe_rate_1600_00745548 = 1600.0f;
constexpr float k_micro_007439bc = 1.0e-6f;          // .rdata 007439BC (9.9999997e-07)
constexpr float k_unsigned_bias_0073d770 = 4294967296.0f;
constexpr float k_half_0073e9ac = 0.5f;
constexpr double k_tenth_00747e08 = 0.1;
constexpr double k_deg2rad_00747e00 = 0.017453292519943295;
constexpr float k_rate_scale_00747df8 = 0.0004f;     // .rdata 00747DF8 (3.9999999e-04)
constexpr float k_quarter_0073edf0 = 0.25f;
constexpr float k_eighth_creep_0073dff4 = 0.005f;    // .rdata 0073DFF4 (4.9999999e-03)
constexpr float k_minus_one_00740cbc = -1.0f;
constexpr double k_rad2deg_00743888 = 57.29577951308232;
constexpr double k_full_turn_00747df0 = 360.0;
constexpr U k_one_bits = 0x3f800000u;

// x87 fsin / fcos of the wide heading (0051E7D1 / 0051E7EF), stored as dwords.
void x87_cos_sin(double angle, float* c, float* s) {
#if defined(_M_IX86)
    float cv, sv;
    __asm {
        fld angle
        fld st(0)
        fcos
        fstp cv
        fsin
        fstp sv
    }
    *c = cv; *s = sv;
#else
    *c = static_cast<float>(std::cos(angle)); *s = static_cast<float>(std::sin(angle));
#endif
}
// (0051F46E: see x87trig_0051f46e)
// lane x87-trig (2026-10-24): 0051F462..0051F489 as one x87 sequence. The fpatan result (64-bit significand)
// feeds the fmul qword [00743888] directly; the former x87_atan2 rounded it to double first. The product, the
// compare with [0073EDF8] and the conditional fadd qword [00747DF0] are PC-53 results; the double returned is
// the value 006FAE8C (__ftol) receives. Precondition as in the original: PC = 53 bits, RC = nearest.
const double kX87Zero_0073edf8 = 0.0;
double clamp_rate(float factor) {                       // 004F1BEC..004F1C1E (fcom 0.125 / 1.5)
    if (factor < k_rate_min_0073f400) return k_rate_min_0073f400;           // test ah,5; jp -> below
    if (factor <= k_rate_max_0073f414 || factor != factor) return factor;   // test ah,41h; jne keep (<=, unordered)
    return k_rate_max_0073f414;
}
void* stats(void* creature) { return at<void*>(creature, 0xa74); }
void* pathfind(void* creature) { return at<void*>(creature, 0x340); }
float w5_height_bits(U bits) { return flt(bits); }        // W1 wrapper returns IEEE single bits
void set_animation(void* creature, U anim) { vcall<void>(creature, 0x7c, anim); }   // vtbl+7C (thiscall RET 4)
}  // namespace

extern "C" {
unsigned __fastcall recovered_door_is_linked_005890d0(void*, void*);   // W1 tier6b

// 004F1BE0 CSWSCreature::GetRunRate: max(stats+198 * clamp(+A08, 0.125, 1.5) * 1000, 1000).
double __fastcall w5_creature_get_run_rate_004f1be0(void* creature, void*) {
    const double rate = (clamp_rate(at<float>(creature, 0xa08)) * at<float>(stats(creature), 0x198)) * k_thousand_0073d6fc;
    if (static_cast<double>(k_thousand_0073d6fc) <= rate || rate != rate) return rate;   // fcomp; test ah,41h; jne keep
    return k_thousand_0073d6fc;
}

// 004F1B20 CSWSCreature::GetWalkRate: stats+19C * clamp(+A08) * 1000 (stored as float); <= 100 -> 0.0;
// a party creature (+9FC bit 0) with a client counterpart walks at client creature [+21C]+60 * 1000.
double __fastcall w5_creature_get_walk_rate_004f1b20(void* creature, void*) {
    const double wide = (clamp_rate(at<float>(creature, 0xa08)) * at<float>(stats(creature), 0x19c)) * k_thousand_0073d6fc;
    const float stored = static_cast<float>(wide);                        // 004F1B6A fst [esp+4]
    if (wide <= static_cast<double>(k_walk_floor_00741838)) return k_zero_0073d700;   // test ah,41h; jp -> party path
    void* client = client_app();
    const U client_id = kotor_fastcall_005eda50(client, nullptr, at<U>(creature, 4));
    void* client_creature = reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_R4_005ed5b0(client, nullptr, client_id)));
    (void)kotor_fastcall_005ed8b0(client, nullptr);                       // 004F1BB0 (result unused)
    if ((at<U8>(creature, 0x9fc) & 1) != 0 && client_creature != nullptr)
        return static_cast<double>(at<float>(at<void*>(client_creature, 0x21c), 0x60)) * k_thousand_0073d6fc;
    return stored;
}

// 00561820 CSWSPlayer::GetLastUpdateObject: W2-owned (coordinator ruling) -> recovered_last_update_object_00561820.

// 005110B0 CSWSCreature::TerminateClientSidePath(byte n): clears the player's last-update path for this creature.
void __fastcall w5_creature_terminate_client_side_path_005110b0(void* creature, void*, U arg) {
    void* player = kotor_fastcall_004b2a50(at<void*>(server_app(), 4), nullptr, at<U>(creature, 4));
    if (player == nullptr) return;
    void* update = recovered_last_update_object_00561820(player, nullptr, at<U>(creature, 4));
    if (update == nullptr) return;
    at<U>(update, 0x40) = 0; at<U>(update, 0x44) = 0; at<U>(update, 0x48) = 0;   // 005110DD..00511109
    at<U>(update, 0x4c) = 0xffff;                                         // 00511112
    at<U8>(update, 0x60) = static_cast<U8>(static_cast<U8>(arg) - 1);     // 0051110C..00511119
}

// 004AE7C0 -> 004B1920 GetDoorByGameObjectID: object type 0Ah (.rdata 00746639) -> vtbl+18 (AsSWSDoor).
void* __fastcall w5_server_internal_get_door_by_id_004b1920(void* internal, void*, U id) {
    void* object = nullptr;
    const unsigned char status = static_cast<unsigned char>(kotor_fastcall_004d8230(at<void*>(internal, 0x1005c), nullptr, id, &object));
    if (status != 0 || object == nullptr) return nullptr;
    if (at<U8>(object, 8) != 0x0a) return nullptr;
    return vcall<void*>(object, 0x18);
}
void* __fastcall w5_server_get_door_by_id_004ae7c0(void* server, void*, U id) {
    return w5_server_internal_get_door_by_id_004b1920(at<void*>(server, 4), nullptr, id);
}
// 005890D0 CSWSDoor::GetIsLinked: byte +384 is 1 or 2.
U __fastcall w5_door_get_is_linked_005890d0(void* door, void*) {   // 1127: W1 owns 005890D0
    return recovered_door_is_linked_005890d0(door, nullptr);
}

// 005064D0 CSWSArea::GetDoorCrossed(from, to): first static object box crossed by the segment (both ends
// raised 0.5) whose door plane separates the ends; INVALID when none.
U __fastcall w5_area_get_door_crossed_005064d0(void* area, void*, const Vector* from, const Vector* to) {
    Vector a = *from;                                                     // [esp+C]
    a.z = static_cast<float>(static_cast<double>(a.z) + k_half_0073e9ac);
    Vector b = *to;                                                       // [esp+18]
    b.z = static_cast<float>(static_cast<double>(b.z) + k_half_0073e9ac);
    for (I i = 0; i < at<I>(area, 0x48); ++i) {
        const unsigned char* box = at<unsigned char*>(area, 0x4c) + i * 0x18;
        if (box_intersect_004aa610(&a, &b, reinterpret_cast<const Vector*>(box), reinterpret_cast<const Vector*>(box + 0xc)) == 0)
            continue;
        const U id = at<U*>(area, 0x50)[i];
        void* door = w5_server_get_door_by_id_004ae7c0(server_app(), nullptr, id);
        if (door == nullptr) continue;
        const Vector p = at<Vector>(door, 0x90);
        const float nx = at<float>(door, 0x9c), ny = at<float>(door, 0xa0), nz = at<float>(door, 0xa4);
        const double s1 = ((static_cast<double>(a.z) - p.z) * nz + (static_cast<double>(a.y) - p.y) * ny) +
                          (static_cast<double>(a.x) - p.x) * nx;          // 00506578..005065C2
        const double s2 = ((static_cast<double>(b.z) - p.z) * nz + (static_cast<double>(b.y) - p.y) * ny) +
                          (static_cast<double>(b.x) - p.x) * nx;          // 005065DA..0050660A
        const U side1 = s1 > k_zero_0073d700 ? 1u : 0u, side2 = s2 > k_zero_0073d700 ? 1u : 0u;  // test ah,41h; jne
        if (side1 != side2) return id;
    }
    return k_object_invalid;
}

// explicit traps (W5-owned, not reconstructed in this checkpoint)
// CloseInventory (thiscall RET 8; EAX unused by AutoCloseGUIPanels): real bodies kotor_fastcall_0055d800
// (claude_w5_creature_actions.cpp) and kotor_fastcall_00587560 (claude_provider_fixes.cpp).
void __fastcall kotor_fastcall_0055d800(void* item, void*, U id, U take);
void __fastcall kotor_fastcall_00587560(void* placeable, void*, U id, U take);

// 004EDED0 CSWSCreature::AutoCloseGUIPanels(flag): close an open item (type 6) or placeable (type 9) inventory.
void __fastcall w5_creature_auto_close_gui_panels_004eded0(void* creature, void*, U) {
    void* client = kotor_fastcall_004ae910(server_app(), nullptr, at<U>(creature, 4));
    if (client == nullptr) return;
    const U open = **static_cast<U**>(static_cast<void*>(static_cast<unsigned char*>(client) + 0x58)); // [[client+58]]
    if (open == k_object_invalid) return;                                 // .rdata 00746644
    void* object = nullptr;
    if (static_cast<unsigned char>(kotor_fastcall_004d8230(object_array(), nullptr, open, &object)) != 0) return;
    const U8 type = at<U8>(object, 8);
    if (type == 6) {                                                      // .rdata 00746635
        void* item = vcall<void*>(object, 0x38);
        if (item != nullptr) kotor_fastcall_0055d800(item, nullptr, at<U>(creature, 4), 1);
    } else if (type == 9) {                                               // .rdata 00746638
        void* placeable = vcall<void*>(object, 0x48);
        if (placeable != nullptr) kotor_fastcall_00587560(placeable, nullptr, at<U>(creature, 4), 1);
    }
}

// 0051CD20 CSWSCreature::DriveUpdateLocation(run): one frame of movement toward pathfind+90 (x, y).
U __fastcall w5_creature_drive_update_location_0051cd20(void* creature, void*, U run) {
    if (at<U>(creature, 0xcc) == 0) return 1;                             // 0051CD3B..0051CD59
    if (at<void*>(pathfind(creature), 0x90) == nullptr) return 3;         // 0051CD5C..0051CD80
    U result = 1;                                                         // EBP
    Vector next{0, 0, 0};                                                 // [E+14]
    double rate = run ? w5_creature_get_run_rate_004f1be0(creature, nullptr) : w5_creature_get_walk_rate_004f1b20(creature, nullptr);
    const U anim = at<U>(creature, 0xd4);
    if (anim == 0x275e || anim == 0x275f) rate = k_strafe_rate_1600_00745548;   // 0051CDBA..0051CDD0
    const float speed = static_cast<float>(at<float>(creature, 0xa0c) * rate);  // 0051CDD6..0051CDEE (arg slot)
    const float* target = at<float*>(pathfind(creature), 0x90);
    Vector t;                                                             // [E+20]
    std::memcpy(&t.x, &target[0], 4);
    t.y = target[1];                                                      // fld/fstp
    t.z = 0.0f;
    t.z = w5_height_bits(w1x_area_height_bits_004bc380(recovered_object_area_004cb120(creature, nullptr), nullptr, &t, 0, nullptr)); // 0051CE17..0051CE1C
    Vector* p = &at<Vector>(creature, 0x90);
    Vector d;                                                             // [E+2C]
    d.z = static_cast<float>(static_cast<double>(t.z) - p->z);
    d.y = static_cast<float>(static_cast<double>(t.y) - p->y);
    d.x = static_cast<float>(static_cast<double>(t.x) - p->x);
    Vector n;                                                             // [E+44]
    normalize_004ab180(&n, &d);
    double elapsed = static_cast<double>(static_cast<I>(at<U>(creature, 0xcc)));   // fild dword
    if (static_cast<I>(at<U>(creature, 0xcc)) < 0) elapsed += k_unsigned_bias_0073d770;
    const double step = (elapsed * speed) * k_micro_007439bc;            // 0051CE6E..0051CE72
    const double distance2 = (static_cast<double>(d.z) * d.z + static_cast<double>(d.y) * d.y) + static_cast<double>(d.x) * d.x;
    if (distance2 <= step * step) {                                       // fcompp; test ah,41h; jp -> partial (>, unordered)
        next = t;                                                         // 0051CEA1..0051CEAB
        result = 2;                                                       // [00745DC0]
    } else {
        const double zs = static_cast<double>(n.z) * step;
        const float ys = static_cast<float>(static_cast<double>(n.y) * step);   // 0051CEC3
        const float xs = static_cast<float>(static_cast<double>(n.x) * step);   // 0051CECD
        const double z = zs + p->z;
        next.x = static_cast<float>(static_cast<double>(xs) + p->x);
        next.y = static_cast<float>(static_cast<double>(ys) + p->y);
        next.z = static_cast<float>(z);
    }
    if (w5x_area_no_creatures_on_line_004bc590(recovered_object_area_004cb120(creature, nullptr), nullptr, p, &next,
            pathfind(creature), 1, 0, 0, 0, nullptr) == 0)                // 0051CF27 (8 args: ..., 1, 0, 0, 0, 0)
        return 3;
    w5_creature_auto_close_gui_panels_004eded0(creature, nullptr, 1);     // 0051CF38
    w5_creature_update_subareas_on_move_to_0051b7b0(creature, nullptr, p, &next, 0, nullptr, 1);  // 0051CF4B
    const U door_id = w5_area_get_door_crossed_005064d0(recovered_object_area_004cb120(creature, nullptr), nullptr, p, &next);
    if (door_id != k_object_invalid) {
        void* door = w5_server_get_door_by_id_004ae7c0(server_app(), nullptr, door_id);
        if (door != nullptr && w5_door_get_is_linked_005890d0(door, nullptr) != 0 && at<U8>(door, 0x2cc) != 0) {
            queue_script_event(0x1e, at<U>(creature, 4), door_id, at<U>(creature, 4));   // 0051CFA0..0051D001
            return 3;
        }
    }
    next.z = w5_height_bits(w1x_area_height_bits_004bc380(recovered_object_area_004cb120(creature, nullptr), nullptr, &next, 0, nullptr)); // 0051D01F
    recovered_object_position_004cd1a0(creature, nullptr, &next, 1, 1, 0);   // 0051D035
    return result;
}

// 0051E6A0 CSWSCreature::AIActionDrive(node). Node parameters (AddDriveAction 004EB510): +38 client x,
// +3C client y, +40 heading*10, +44 a, +48 key flags (1 fwd, 2 back, 4/8 strafe, 10h/20h turn), +4C repeat,
// +50 first-run (cleared here).
__declspec(noinline) double __cdecl x87trig_0051f46e(const void* creature) {
    double r;
    __asm {
        mov eax, creature
        fld dword ptr [eax + 0A0h]             // 0051F462
        fld dword ptr [eax + 9Ch]              // 0051F468
        fpatan                                 // 0051F46E
        fmul qword ptr k_rad2deg_00743888      // 0051F470
        fcom qword ptr kX87Zero_0073edf8       // 0051F476
        fnstsw ax                              // 0051F47C
        test ah, 5                             // 0051F47E
        jp x87trig_keep                        // 0051F481
        fadd qword ptr k_full_turn_00747df0    // 0051F483
    x87trig_keep:
        fstp qword ptr r                       // (ST0 is __ftol's operand at 0051F498; exact under PC 53)
    }
    return r;
}
U __fastcall w5_action_drive_0051e6a0(void* creature, void*, void* node) {
    if (vcall<U>(creature, 0x94) == 1) return 3;                          // 0051E6AE..0051E6B7
    if (kotor_m15e_slot010_zero_0063e7f0(creature, nullptr) == 1) return 3;
    if (recovered_creature_incapacitated_004ef890(creature, nullptr) == 1) return 3;
    const Vector client{at<float>(node, 0x38), at<float>(node, 0x3c), 0.0f};   // [E+F0]
    const U first_run = at<U>(node, 0x50);
    U flags = at<U>(node, 0x48);                                          // [E+44]
    const U arg_a = at<U>(node, 0x44);                                    // [E+BC]
    const I repeat = at<I>(node, 0x4c);                                   // [E+B8]
    void* area = recovered_object_area_004cb120(creature, nullptr);       // [E+C0]
    if (area == nullptr) return 3;
    at<U>(creature, 0xa14) = 1;
    if (at<U>(creature, 0x8e8) == 1 && (flags & 3) == 3) flags = (flags & ~2u) | 1u;   // 0051E740..0051E764
    Vector* p = &at<Vector>(creature, 0x90);                              // EBX
    Vector final_pos = *p;                                                // [E+48]
    U scale_bits = k_one_bits;                                            // [E+24] after the first-run setup
    if (first_run != 0) {
        const double heading = (static_cast<double>(static_cast<I>(at<U>(node, 0x40))) * k_tenth_00747e08) * k_deg2rad_00747e00;
        at<U>(creature, 0xa0c) = k_one_bits;
        at<U>(node, 0x50) = 0;
        Vector dir;                                                       // [E+60]
        dir.z = 0.0f;
        float rate = 0.0f;                                                // [E+40]
        Vector move{0, 0, 0};                                             // [E+28]
        x87_cos_sin(heading, &dir.x, &dir.y);
        if (flags & 4) {                                                  // strafe left
            const Vector side{static_cast<float>(-static_cast<double>(dir.y)), dir.x, 0.0f};
            vector_add_assign_004a9740(&move, nullptr, &side);
            rate = 0.8f;                                                  // 3F4CCCCD
        } else if (flags & 8) {                                           // strafe right
            const Vector side{dir.y, static_cast<float>(-static_cast<double>(dir.x)), 0.0f};
            vector_add_assign_004a9740(&move, nullptr, &side);
            rate = 0.8f;
        }
        bool have_rate = true;
        double wide_rate = 0;
        if ((flags & 3) == 3) { vector_add_assign_004a9740(&move, nullptr, &dir); wide_rate = w5_creature_get_run_rate_004f1be0(creature, nullptr); }
        else if (flags & 1) { vector_add_assign_004a9740(&move, nullptr, &dir); wide_rate = w5_creature_get_walk_rate_004f1b20(creature, nullptr); }
        else if (flags & 2) { vector_subtract_assign_004a9770(&move, nullptr, &dir); wide_rate = w5_creature_get_walk_rate_004f1b20(creature, nullptr); }
        else have_rate = false;
        if (have_rate) rate = static_cast<float>(wide_rate * k_rate_scale_00747df8);   // 0051E896..0051E8A0
        if (flags & 0x30) {                                               // turn: rotate move and dir by +-30
            alignas(4) float q[4];                                        // [E+E0]
            yaw_pitch_roll_004acac0(q, (flags & 0x10) ? 30.0f : -30.0f, 0.0f, 0.0f);
            Vector r;
            move = *quaternion_rotate_004a9bb0(&r, q, &move);
            dir = *quaternion_rotate_004a9bb0(&r, q, &dir);
        }
        vector_normalize_004ab130(&dir, nullptr);
        w1x_object_set_orientation_004cd170(creature, nullptr, const_cast<Vector*>(&dir));    // 0051E992
        vector_normalize_004ab130(&move, nullptr);
        Vector delta;                                                     // [E+10]
        kotor_cdecl_00423bc0(&delta, &client, p);
        Vector want{0, 0, 0};                                             // [E+54]
        const double off2 = (static_cast<double>(delta.z) * delta.z + static_cast<double>(delta.y) * delta.y) + static_cast<double>(delta.x) * delta.x;
        bool repositioned = false;
        if (off2 > k_quarter_0073edf0) {                                  // fcomp 0.25; test ah,41h; jne -> keep
            if (w1x_area_safe_location_004be860(area, nullptr, static_cast<U>(reinterpret_cast<std::uintptr_t>(&client)), 0x3f000000u, static_cast<U>(reinterpret_cast<std::uintptr_t>(pathfind(creature))), 0, static_cast<U>(reinterpret_cast<std::uintptr_t>(&want))) != 0) {
                recovered_object_position_004cd1a0(creature, nullptr, &want, 1, 1, 0);
                repositioned = true;
            }
        }
        if (!repositioned) want = *p;                                     // 0051EA31..0051EA43
        const Vector pos_xy{p->x, p->y, 0.0f};                            // [E+84] P.x, [E+8C] P.y
        float radius = at<float>(pathfind(creature), 4);                  // [E+88]
        const float band = at<float>(pathfind(creature), 0x14);           // [E+90]
        at<U>(creature, 0xa0c) = k_one_bits;                              // 0051EA86
        Vector scaled;
        vector_add_assign_004a9740(&want, nullptr, vector_component_mult_00423bf0(&scaled, &move, rate));
        want.z = w5_height_bits(w1x_area_height_bits_004bc380(recovered_object_area_004cb120(creature, nullptr), nullptr, &want, 0, nullptr));
        U blocker = 0;                                                    // [E+C4]
        if (w5x_area_no_creatures_on_line_004bc590(area, nullptr, p, &want, pathfind(creature), 1, 0,
                static_cast<I>(reinterpret_cast<std::uintptr_t>(&blocker)), 0, nullptr) == 0 && blocker != k_object_invalid) {
            void* other = kotor_fastcall_004ae770(server_app(), nullptr, blocker);
            if (other != nullptr) w5_creature_bump_friends_004f6390(creature, nullptr, other, p, &want);
        }
        Vector from = pos_xy;                                             // [E+10] (z = 0)
        U ok = 0;                                                         // EDI
        if (w5_area_test_direct_line_004bcb70(area, nullptr, &from, &want, &radius, band, 0, nullptr) == 1) {
            final_pos = want;
            ok = 1;
        } else {
            float t = 0.5f, dt = 0.25f;                                   // [E+1C] [E+20]
            Vector probe;                                                 // [E+A0]
            for (;;) {                                                    // 0051EB84..0051EC9F binary search on the step
                const float dz = static_cast<float>(static_cast<double>(want.z) - p->z);
                const float xs = static_cast<float>((static_cast<double>(want.x) - p->x) * t);
                const double ys = (static_cast<double>(want.y) - p->y) * t;
                const double zs = static_cast<double>(dz) * t;
                probe.x = static_cast<float>(static_cast<double>(xs) + p->x);
                probe.y = static_cast<float>(ys + p->y);
                probe.z = static_cast<float>(zs + p->z);
                Vector origin = pos_xy;                                   // [E+AC] (z = 0)
                double next_t;
                if (w5_area_test_direct_line_004bcb70(area, nullptr, &origin, &probe, &radius, band, 0, nullptr) == 1) {
                    final_pos = probe; ok = 1;
                    next_t = static_cast<double>(dt) + t;
                } else {
                    next_t = static_cast<double>(t) - dt;
                }
                t = static_cast<float>(next_t);
                const double half = static_cast<double>(dt) * k_half_0073e9ac;
                dt = static_cast<float>(half);
                if (!(half > k_eighth_creep_0073dff4)) break;             // fcomp 0.005; test ah,41h; je loop
            }
            if (ok == 1) {
                if (repeat > 1) at<I>(node, 0x4c) = repeat - 1;           // 0051ECAA..0051ECC1
            } else {
                // 0051ECD1..0051F248: slide along walls, refining two side directions for 15 rounds.
                Vector q1, q2;                                            // [E+78] [E+94]
                const Vector perp{move.y, static_cast<float>(-static_cast<double>(move.x)), 0.0f};
                normalize_004ab180(&q1, &perp);
                const Vector neg{static_cast<float>(static_cast<double>(q1.x) * k_minus_one_00740cbc),
                                 static_cast<float>(static_cast<double>(q1.y) * k_minus_one_00740cbc),
                                 static_cast<float>(static_cast<double>(q1.z) * k_minus_one_00740cbc)};
                normalize_004ab180(&q2, &neg);
                Vector dir_l = move, dir_r = move;                        // [E+AC] [E+10]
                U left = 1, right = 1;                                    // [E+20] [E+1C]
                U left_ok = blocker, right_ok = blocker;                  // EBP / EDI start as [E+C4]
                for (I round = 0; round < 15; ++round) {
                    if (left == 0 && right == 0) break;
                    Vector sum_l{static_cast<float>(static_cast<double>(dir_l.x) + q2.x), static_cast<float>(static_cast<double>(dir_l.y) + q2.y),
                                 static_cast<float>(static_cast<double>(dir_l.z) + q2.z)};
                    Vector a, b, tmp;
                    a = *normalize_004ab180(&tmp, &sum_l);                // [E+34]
                    Vector sum_r{static_cast<float>(static_cast<double>(dir_r.x) + q1.x), static_cast<float>(static_cast<double>(dir_r.y) + q1.y),
                                 static_cast<float>(static_cast<double>(dir_r.z) + q1.z)};
                    b = *normalize_004ab180(&tmp, &sum_r);                // [E+C8]
                    if (left != 0) {
                        Vector s, probe_l;
                        probe_l = *vector_sum_00423b90(&tmp, p, vector_component_mult_00423bf0(&s, &a, rate));
                        Vector origin = pos_xy;
                        left_ok = w5_area_test_direct_line_004bcb70(area, nullptr, &origin, &probe_l, &radius, band, 0, nullptr) == 1 ? 1u : 0u;
                    }
                    if (right != 0) {
                        Vector s, probe_r;
                        probe_r = *vector_sum_00423b90(&tmp, p, vector_component_mult_00423bf0(&s, &b, rate));
                        Vector origin = pos_xy;
                        right_ok = w5_area_test_direct_line_004bcb70(area, nullptr, &origin, &probe_r, &radius, band, 0, nullptr) == 1 ? 1u : 0u;
                    }
                    if (left != 0) {
                        if (left_ok != 0) q2 = a;                         // 0051EFE5..0051EFFF
                        else { dir_l = a; left = right_ok == 0 ? 1u : 0u; }   // 0051F008..0051F02D
                    }
                    if (right != 0) {
                        if (right_ok != 0) q1 = b;                        // 0051F040..0051F05D
                        else { dir_r = b; right = left_ok == 0 ? 1u : 0u; }   // 0051F066..0051F08E
                    }
                }
                Vector chosen{0, 0, 0};                                   // [E+34]
                if (left == right) {                                      // 0051F0C8..0051F113
                    const double d2 = (static_cast<double>(q2.y) * move.y + static_cast<double>(q2.x) * move.x) + static_cast<double>(q2.z) * move.z;
                    const double d1 = (static_cast<double>(q1.y) * move.y + static_cast<double>(q1.x) * move.x) + static_cast<double>(q1.z) * move.z;
                    chosen = (d1 < d2) ? q2 : q1;                         // fcompp; test ah,5; jp -> q1
                } else if (left != 0) chosen = q2;
                else if (right != 0) chosen = q1;
                const double along = (static_cast<double>(chosen.y) * move.y + static_cast<double>(chosen.x) * move.x) + static_cast<double>(chosen.z) * move.z;
                float along_f = static_cast<float>(along);                // [E+24]
                if (!(along > k_quarter_0073edf0)) along_f = k_quarter_0073edf0;   // test ah,41h; je keep
                std::memcpy(&scale_bits, &along_f, 4);
                const double s = static_cast<double>(rate) * along_f;
                const float ys2 = static_cast<float>(static_cast<double>(chosen.y) * s);
                const float zs2 = static_cast<float>(static_cast<double>(chosen.z) * s);
                final_pos.x = static_cast<float>(static_cast<double>(chosen.x) * s + p->x);
                final_pos.y = static_cast<float>(static_cast<double>(ys2) + p->y);
                final_pos.z = static_cast<float>(static_cast<double>(zs2) + p->z);
                Vector origin = pos_xy;
                ok = w5_area_test_direct_line_004bcb70(area, nullptr, &origin, &final_pos, &radius, band, 0, nullptr) == 1 ? 1u : 0u;
            }
        }
        if (ok != 1) {                                                    // 0051F387..0051F3C4
            at<U>(pathfind(creature), 0x28) = arg_a;
            w5_creature_terminate_client_side_path_005110b0(creature, nullptr, arg_a);
            at<U>(creature, 0xa14) = 0;
            set_animation(creature, 0x2710);
            return 3;
        }
        // 0051F252..0051F33A: commit the target into the pathfind record (+340).
        at<U>(creature, 0xa0c) = scale_bits;
        float* points = static_cast<float*>(live_operator_new(8));
        points[0] = final_pos.x; points[1] = final_pos.y;
        void* pf = pathfind(creature);
        at<U>(pf, 0x8c) = 2;
        at<float*>(pathfind(creature), 0x90) = points;
        at<U>(pathfind(creature), 0) = 1;
        at<U>(pathfind(creature), 0x2c) = at<U>(creature, 4);
        at<U>(pathfind(creature), 0x30) = k_object_invalid;
        at<U>(pathfind(creature), 0x84) = 0;
        at<U>(pathfind(creature), 0x70) = at<U>(creature, 0x8c);
        at<Vector>(pathfind(creature), 0x5c) = final_pos;
        at<float>(pathfind(creature), 0x74) = p->x;
        at<U>(pathfind(creature), 0x78) = at<U>(creature, 0x94);
        at<float>(pathfind(creature), 0x7c) = at<float>(creature, 0x98);
        at<U>(pathfind(creature), 0x9c) = 0;
        at<U>(pathfind(creature), 0x28) = arg_a;
    }
    // 0051F33D..0051F4ED: animate and advance (every frame).
    void* pf = pathfind(creature);
    final_pos.x = at<float>(pf, 0x5c); final_pos.y = at<float>(pf, 0x60); final_pos.z = at<float>(pf, 0x64);
    U anim = 0x2714;
    U run = 0;
    if (flags & 4) anim = 0x275e;
    else if (flags & 8) anim = 0x275f;
    if ((flags & 3) == 3) {
        run = 1;
        anim = (flags & 4) ? 0x2766u : ((flags & 8) ? 0x2767u : 0x2714u);
    } else if (flags & 1) {
        anim = (flags & 4) ? 0x2764u : ((flags & 8) ? 0x2765u : 0x2712u);
    } else if (flags & 2) {
        anim = 0x2713;
    }
    set_animation(creature, static_cast<U16>(anim));
    const U result = w5_creature_drive_update_location_0051cd20(creature, nullptr, run);
    if (result == 1) return 1;
    if (repeat > 1 && result == 2) {                                      // 0051F44E..0051F4C8: chain the next step
        const double angle = x87trig_0051f46e(creature);                  // 0051F462..0051F489 (x87, incl. fcom 0 / +360)
        const I degrees = static_cast<I>(static_cast<std::int64_t>(angle));   // __ftol2 (truncate)
        const U heading = static_cast<U16>(static_cast<U>(degrees) * 10u);
        w5_add_drive_action_004eb510(creature, nullptr, at<U16>(node, 0x6c), p, heading, arg_a, flags, static_cast<U>(repeat - 1));
        return result;
    }
    at<U>(creature, 0xa14) = 0;                                           // 0051F4CB..0051F4DE
    set_animation(creature, 0x2710);
    return result;
}
}
