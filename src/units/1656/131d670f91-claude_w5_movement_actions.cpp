// W5 queued-action bodies, movement group (lane worker w5-movement-actions).
// CSWSCreature::AIAction* bodies dispatched by the W1 runner (RunActions 0057F4A0), transcribed from the original
// swkotor.exe (read-only). Addresses in the comments are the original instructions.
//   Contract: original thiscall(CSWSObjectActionNode* node), RET 4, EAX = disposition
//             (.rdata 00745DBC = 1 in progress, 00745DC0 = 2 complete, 00745DC4 = 3 failed).
// Not reconstructed here: 0051F4F0 AIActionMoveToPoint (pathfinder / walk-update cascade far above the brief's
// limit; see the report). Callees without any provider are reconstructed in the "shared helpers" section at the end
// under kotor_fastcall_<addr>.
#include "842d6b1618-w5_t3.hpp"
#include "5889fde5f1-w5_action_bodies.hpp"
#include "4f622be2f2-w5_area_direct_line.hpp"
#include "ab3fc15780-w5_drive.hpp"
#include "73b9db80fe-w5_pathfind_reset.hpp"
#include "e5e3766b7c-w5_subarea.hpp"
#include <initializer_list>

using namespace w5;

namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction { void* __fastcall remove_head_005e9c80(kotor_list::Internal*, void*); }  // [0091]

extern "C" {
// ---------------- existing providers (c03 map), exact C-linkage names ----------------
void* __fastcall kotor_fastcall_004ae780(void* server, void*, U id);                    // GetAreaByGameObjectID [0530]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);             // GetSWSMessage [creature_destructor_r2]
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U size, U field4c, U field48); // CreateWriteMessage [w2_message]
void __fastcall kotor_fastcall_004d62c0(void* message, void*, U value, U bits);         // WriteBYTE [w2_message]
void __fastcall kotor_fastcall_004d6920(void* message, void*, U value_bits, U scale, U bits); // WriteFLOAT [w2_message]
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** output, U* size);     // GetWriteMessage [w2_message]
U __fastcall kotor_fastcall_00567e60(void* message, void*, U recipient, U major, U minor, void* data, U size); // SendServerToPlayerMessage
U __fastcall recovered_update_subareas_0051b940(void* creature, void*, void* pos, U area_id, U flag);       // UpdateSubareasOnJumpPosition [W2]
void __fastcall recovered_creature_placement_004fa100(void* creature, void*, U area, U x, U y, U z, U saved, U flag); // AddToArea [1060]
void __fastcall recovered_reset_all_follow_data_00637890(void* party, void*, void* pos, float yaw, U flag); // ResetAllFollowData [W2]
double __fastcall kotor_fastcall_00634a70(void* party, void*);                           // CSWParty::GetFollowRange [W4 t1b], ST0
void __fastcall recovered_trigger_bounds_0058cc60(void* trigger, void*, void* x0, void* y0, void* x1, void* y1); // ComputeBoundingBox [1128]
Vector* __cdecl float_times_vector_00437a60(Vector* out, float scale, const Vector* v);  // operator*(float, Vector) [0555]
int __cdecl crt_rand_006fc447();                                                         // rand [0479]
void __fastcall c2da_005e9c10(void* list, void*, void* position);                        // CExoLinkedListInternal::Remove [0191] (no EAX)
void __cdecl unresolved_ftol2_006fae8c();                                                // __ftol2 (ST0 -> EAX) [0064]

// ---------------- reconstructed in this file (shared helpers, defined at the end) ----------------
U __fastcall kotor_fastcall_004eb5a0(void* creature, void*, U group);                    // AddPathfindingWaitActionToFront RET 4
void __fastcall kotor_fastcall_004edf80(void* creature, void*);                          // CutWaypointPath RET 0
void __fastcall kotor_fastcall_005d0e70(void* pf, void*);                                // ResetInterAreaSearchData RET 0
U __fastcall kotor_fastcall_004c6450(void* module, void*, U id);                         // IsObjectInLimbo RET 4
void __fastcall kotor_fastcall_004c63f0(void* module, void*, U id);                      // RemoveObjectFromLimbo RET 4
void __fastcall kotor_fastcall_004c3ea0(void* module, void*, void* pf);                  // SetIntraAreaGoal RET 4
U __fastcall kotor_fastcall_0058d8c0(void* trigger, void*, const Vector* p, Vector* out); // GetClosestPointWithinTrigger RET 8
void __fastcall kotor_fastcall_005cec90(void* pf, void*);                                // ComputeStepTolerance RET 0
Vector* __fastcall kotor_fastcall_004bdf80(void* area, void*, Vector* out, const Vector* from, const Vector* target,
                                           float dist, void* pf);                         // ComputeAwayVector RET 14h
U __fastcall kotor_fastcall_004f1ef0(void* creature, void*, U unused, U group);          // CancelAction RET 8
U __fastcall kotor_fastcall_0056c610(void* message, void*, void* player, U flags, U x, U y, U z, U byte_value); // RET 18h
}

namespace {
constexpr U k_in_progress_00745dbc = 1;           // .rdata 00745DBC
constexpr U k_complete_00745dc0 = 2;              // .rdata 00745DC0
constexpr U k_failed_00745dc4 = 3;                // .rdata 00745DC4
constexpr U k_type_bound_00746633 = 4;            // .rdata 00746633 (byte; also the area type in JumpToPoint)
constexpr U k_type_trigger_00746636 = 7;          // .rdata 00746636 (byte)
constexpr U k_type_door_00746639 = 10;            // .rdata 00746639 (byte)
const float k_tiny_00747504 = 1e-05f;             // .rdata 00747504
const float k_radius_slack_0073dfec = 0.01f;      // .rdata 0073DFEC
const double k_follow_close_00747c48 = 0x1.47ae147ae147cp-7; // .rdata 00747C48 qword 3F847AE147AE147C (one ULP above 0.01)
const float k_zero_0073d700 = 0.0f;               // .rdata 0073D700
const float k_five_00743264 = 5.0f;               // .rdata 00743264
const float k_half_0073e9ac = 0.5f;               // .rdata 0073E9AC
const float k_fifth_0073e46c = 0.2f;              // .rdata 0073E46C
const float k_far_0074325c = 1e10f;               // .rdata 0074325C
const float k_seven_00747c6c = 7.0f;              // .rdata 00747C6C
const float k_three_tenths_00746d38 = 0.3f;       // .rdata 00746D38
const float k_three_quarters_0073e31c = 0.75f;    // .rdata 0073E31C
const float k_one_0073d764 = 1.0f;                // .rdata 0073D764
const float k_quarter_pi_00741c3c = 0.785398185253143310546875f; // .rdata 00741C3C (3F490FDB)
const float k_w256_007455d4 = 256.0f, k_w64_007455cc = 64.0f, k_w16_007455d0 = 16.0f, k_w4_0073f404 = 4.0f;
const float k_min_scale_0073dff4 = 0.005f;        // .rdata 0073DFF4

inline void* as_object(void* o) { return vcall<void*>(o, 0x10); }      // vtbl+10 AsSWSObject
inline void* as_door(void* o) { return vcall<void*>(o, 0x18); }        // vtbl+18 AsSWSDoor
inline void* as_area(void* o) { return vcall<void*>(o, 0x28); }        // vtbl+28 AsSWSArea
inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }    // vtbl+30 AsSWSCreature
inline void* as_trigger(void* o) { return vcall<void*>(o, 0x40); }     // vtbl+40 AsSWSTrigger
inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline Vector vec(U x, U y, U z) { Vector v; std::memcpy(&v.x, &x, 4); std::memcpy(&v.y, &y, 4); std::memcpy(&v.z, &z, 4); return v; }
inline unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
inline Vector* vat(void* p, U off) { return reinterpret_cast<Vector*>(bytes(p) + off); }
// CGameObjectArray::GetGameObject through the server object array (inline view of 004AED70); AL 0 = found
inline bool lookup(U id, void** out) { return static_cast<unsigned char>(kotor_fastcall_004d8230(object_array(), nullptr, id, out)) == 0; }
inline void* pathfind(void* c) { return at<void*>(c, 0x340); }
inline void* server_internal() { return at<void*>(server_app(), 4); }
inline U party_leader_server_id() {                                       // ClientToServerObjectId(GetSWParty()+24)
    return kotor_fastcall_004aea30(server_app(), nullptr, at<U>(kotor_fastcall_005ed8b0(client_app(), nullptr), 0x24));
}
// the three entry checks shared by the bodies: vtbl+94, return_zero (0063E7F0), GetDeadTemp, each compared with 1
inline bool blocked(void* self) {
    if (vcall<U>(self, 0x94) == 1) return true;
    if (kotor_m15e_slot010_zero_0063e7f0(self, nullptr) == 1) return true;
    return recovered_creature_incapacitated_004ef890(self, nullptr) == 1;
}
// area of self != node area id (area +120 = the CGameObject id of the area, 0 when self has no area)
inline bool other_area(void* self, U area_id) {
    if (recovered_object_area_004cb120(self, nullptr) == nullptr) return false;
    return area_id != at<U>(recovered_object_area_004cb120(self, nullptr), 0x120);
}
inline double sq(float f) { return static_cast<double>(f) * f; }
// x87 transcendental and __ftol2 exactly as the original executes them (ST0 in/out)
// lane x87-trig (2026-10-24): the former x87_fcos / x87_fsin rounded the 64-bit-significand results to double;
// 004BE05A/004BE060 feed them to fmul/fadd/fsub on the stack. Replaced by x87trig_004be041 (below).
}  // namespace
// 004BE041..004BE091 (one probe direction of the 8-way search): a = i * [00741C3C] (PC 53), [esp+20h] = (float)a,
// c = cos(a) (the extended a), s = sin((float)a); rx = (float)(n.y*s + n.x*c) ([esp+A4h]); ry = n.y*c - n.x*s
// stays on the stack in the original (a PC-53 result, carried exactly by the double). n = {[esp+80h], [esp+84h]}.
// Precondition as in the original: PC = 53 bits, RC = nearest. No CALL inside the asm.
extern "C" __declspec(noinline) void __cdecl x87trig_004be041(const int* i, const float* n, float* angle_f, float* rx,
                                                             double* ry) {
    __asm {
        mov eax, i
        mov ecx, n
        fild dword ptr [eax]                   // 004BE041 [esp+24h]
        fmul dword ptr k_quarter_pi_00741c3c       // 004BE050
        mov eax, angle_f
        fst dword ptr [eax]                    // 004BE056 [esp+20h]
        fcos                                   // 004BE05A
        fld dword ptr [eax]                    // 004BE05C
        fsin                                   // 004BE060
        fld dword ptr [ecx + 4]                // 004BE062 [esp+84h]
        fmul st, st(1)                         // 004BE069 D8 C9
        fld dword ptr [ecx]                    // 004BE06B [esp+80h]
        fmul st, st(3)                         // 004BE072 D8 CB
        faddp st(1), st                        // 004BE074 DE C1
        mov eax, rx
        fstp dword ptr [eax]                   // 004BE076 [esp+A4h]
        fld dword ptr [ecx + 4]                // 004BE07D
        fmulp st(2), st                        // 004BE084 DE CA
        fld dword ptr [ecx]                    // 004BE086
        fmul st, st(1)                         // 004BE08D
        fsubp st(2), st                        // 004BE08F DE EA
        fstp st(0)                             // 004BE091
        mov eax, ry
        fstp qword ptr [eax]                   // (ry stays on the stack in the original; exact under PC 53)
    }
}
namespace {
inline U ftol2(double v) { U r; __asm { fld qword ptr v } __asm { call unresolved_ftol2_006fae8c } __asm { mov r, eax } return r; }
// AddActionToFront(type, group, pairs[13]) through W1's 004CCB00 export (shape helper from w5_open_door.cpp)
inline void front(void* self, U type, U group, std::initializer_list<U> list) {
    U pairs[26] = {};
    U i = 0;
    for (U v : list) pairs[i++] = v;
    w5_add_action_to_front_body(self, type, group, pairs);
}
inline U group_of(void* node) { return at<U16>(node, 0x6c); }
}  // namespace

extern "C" {

// =========================================================================================================
// 00511080 CSWSCreature::AIActionAreaWait
U __fastcall w5_action_area_wait_00511080(void* self, void*, void* node) {
    (void)node;
    if (at<U>(self, 0x9d4) == 0) return k_failed_00745dc4;                         // 00511080..8F
    return recovered_object_area_004cb120(self, nullptr) == nullptr ? k_in_progress_00745dbc
                                                                    : k_complete_00745dc0; // 00511092..A5
}

// =========================================================================================================
// 005137C0 CSWSCreature::AIActionOrientCamera: node +38 flags, +3C/+40/+44 floats, +48 byte -> camera message
U __fastcall w5_action_orient_camera_005137c0(void* self, void*, void* node) {
    const U flags = at<U>(node, 0x38), x = at<U>(node, 0x3c), y = at<U>(node, 0x40);  // 005137C1..D8
    const U z = at<U>(node, 0x44), b = at<U>(node, 0x48);
    void* player = kotor_fastcall_004ae910(server_app(), nullptr, at<U>(self, 4));   // 005137DF..EF
    if (player != nullptr) {
        void* message = recovered_server_sws_message_004aedb0(server_app(), nullptr); // 005137FA..03
        if (message != nullptr) kotor_fastcall_0056c610(message, nullptr, player, flags, x, y, z, b); // 0051380C..1C
    }
    return k_complete_00745dc0;                                                    // 00513821
}

// =========================================================================================================
// 0050FC90 CSWSCreature::AIActionChangeFacingPoint: node +38 x, +3C y
U __fastcall w5_action_change_facing_point_0050fc90(void* self, void*, void* node) {
    if (blocked(self)) return k_failed_00745dc4;                                   // 0050FC96..C1
    Vector d;                                                                       // [esp+8]
    d.x = static_cast<float>(static_cast<double>(at<float>(node, 0x38)) - at<float>(self, 0x90)); // 0050FCD1..E7
    d.y = static_cast<float>(static_cast<double>(at<float>(node, 0x3c)) - at<float>(self, 0x94)); // 0050FCEB..F5
    d.z = 0.0f;                                                                     // 0050FCDF
    if (sq(d.y) + sq(d.x) >= static_cast<double>(k_tiny_00747504)) {               // 0050FCF9..16 (test ah,1: C0 skips)
        Vector n;                                                                   // [esp+14]
        w1x_object_set_orientation_004cd170(self, nullptr, normalize_004ab180(&n, &d)); // 0050FD18..2D
    }
    return k_complete_00745dc0;
}

// =========================================================================================================
// 0050FB60 CSWSCreature::AIActionChangeFacingObject: node +38 object id
U __fastcall w5_action_change_facing_object_0050fb60(void* self, void*, void* node) {
    if (blocked(self)) return k_failed_00745dc4;                                   // 0050FB66..91
    void* obj = nullptr;                                                            // [esp+4]
    if (!lookup(at<U>(node, 0x38), &obj)) return k_failed_00745dc4;                 // 0050FB97..BF
    if (at<U8>(obj, 8) <= k_type_bound_00746633) return k_failed_00745dc4;          // 0050FBC5..D2 (jbe)
    if (recovered_object_area_004cb120(obj, nullptr) != recovered_object_area_004cb120(self, nullptr))
        return k_failed_00745dc4;                                                   // 0050FBD8..EA
    Vector target;                                                                  // [esp+14]
    copy_vector(&target, bytes(as_object(obj)) + 0x90);                             // 0050FBF0..19
    Vector d;                                                                       // [esp+8]
    kotor_cdecl_00423bc0(&d, &target, vat(self, 0x90));                             // 0050FC14..23
    d.z = 0.0f;                                                                     // 0050FC37
    if (sq(d.y) + sq(d.x) >= static_cast<double>(k_tiny_00747504)) {               // 0050FC28..50
        Vector n;                                                                   // [esp+14]
        w1x_object_set_orientation_004cd170(self, nullptr, normalize_004ab180(&n, &d)); // 0050FC52..67
    }
    return k_complete_00745dc0;
}

// =========================================================================================================
// 00510670 CSWSCreature::AIActionCheckMoveToPoint: node +38/+3C/+40 point, +44 area id, +48 target id, +4C run
U __fastcall w5_action_check_move_to_point_00510670(void* self, void*, void* node) {
    if (blocked(self)) return k_failed_00745dc4;                                   // 00510676..A3
    if ((at<U8>(self, 0x9f0) & 2) == 0) return k_failed_00745dc4;                   // 005106A9..B0
    U x = at<U>(node, 0x38), y = at<U>(node, 0x3c), z = at<U>(node, 0x40);           // [esp+C..14]
    U area_id = at<U>(node, 0x44), target = at<U>(node, 0x48);                      // [esp+18], [esp+1C]
    const U run = at<U>(node, 0x4c);                                                // EBX
    void* obj = nullptr;                                                            // [esp+20] (unused afterwards)
    if (!lookup(area_id, &obj)) return k_failed_00745dc4;                           // 005106E6..01
    const float range = at<float>(pathfind(self), 4);                               // 00510707..10 (over the node slot)
    const bool far_area = other_area(self, area_id);                                // 00510714..43 (EBP)
    Vector point = vec(x, y, z);                                                    // [esp+30]
    Vector d;                                                                       // [esp+24]
    kotor_cdecl_00423bc0(&d, vat(self, 0x90), &point);                              // 00510743..6C
    if (!far_area) {                                                                // 00510774..77
        const double two_r = static_cast<double>(range) + range;                    // 00510779..7D
        const double sum = (sq(d.z) + sq(d.y)) + sq(d.x);                           // 0051077F..99
        if (!(two_r * two_r <= sum)) return k_complete_00745dc0;                    // 0051079B..A8 (test ah,41h; jp)
    }
    // 005107AE..FB: re-queue itself. Original quirk: the type-1 run parameter is pushed by value (EBX) where
    // AddActionNodeParameter 004CAC30 dereferences a pointer (its +4C slot read from the node) - kept as is.
    front(self, 0xa, group_of(node), {2, addr(&x), 2, addr(&y), 2, addr(&z), 3, addr(&area_id), 3, addr(&target), 1, run});
    Vector to = vec(x, y, z);                                                       // [esp+24] (reused)
    Vector zero = vec(0, 0, 0);                                                     // [esp+30]
    w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, group_of(node), &to, area_id, target, run,
        0.0f, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0);                              // 00510800..64
    kotor_fastcall_004eb5a0(self, nullptr, group_of(node));                          // 00510869..72
    return k_complete_00745dc0;
}

// =========================================================================================================
// 005108A0 CSWSCreature::AIActionCheckMoveToPointRadius: +38..40 point, +44 area, +48 target, +4C run,
// +50 radius, +54 flag
U __fastcall w5_action_check_move_to_point_radius_005108a0(void* self, void*, void* node) {
    if (blocked(self)) return k_failed_00745dc4;                                   // 005108A5..D2
    U x = at<U>(node, 0x38), y = at<U>(node, 0x3c), z = at<U>(node, 0x40);           // [esp+8], [esp+C], [esp+18]
    U area_id = at<U>(node, 0x44), target = at<U>(node, 0x48), run = at<U>(node, 0x4c); // [14], [24], [20]
    U radius = at<U>(node, 0x50), flag = at<U>(node, 0x54);                         // [10], [1C]
    void* obj = nullptr;                                                            // [esp+28]
    if (!lookup(area_id, &obj)) return k_failed_00745dc4;                           // 00510910..35
    const bool far_area = other_area(self, area_id);                                // 0051093B..69
    const float dxf = static_cast<float>(static_cast<double>(at<float>(self, 0x90)) - flt(x)); // 0051096B..79 [esp+38]
    const double dy = static_cast<double>(at<float>(self, 0x94)) - flt(y);          // 0051097D (ST0, not stored)
    if (!far_area) {
        const double r = static_cast<double>(flt(radius)) + k_radius_slack_0073dfec; // 00510985..89
        const double sum = dy * dy + sq(dxf);                                       // 0051098F..9B
        if (!(r * r < sum)) return k_complete_00745dc0;                             // 0051099D..AC (test ah,5; jp)
    }
    front(self, 0x12, group_of(node), {2, addr(&x), 2, addr(&y), 2, addr(&z), 3, addr(&area_id), 3, addr(&target),
                                       1, addr(&run), 2, addr(&radius), 1, addr(&flag)}); // 005109B6..A0D
    Vector to = vec(x, y, z);                                                       // [esp+38]
    Vector zero = vec(0, 0, 0);                                                     // [esp+2C]
    w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, group_of(node), &to, area_id, target, run,
        flt(radius), 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, flag);                    // 00510A12..80
    kotor_fastcall_004eb5a0(self, nullptr, group_of(node));                          // 00510A85..8E
    return k_complete_00745dc0;
}

// =========================================================================================================
// 005101A0 CSWSCreature::AIActionCheckMoveToObject: node +38 object id, +3C run
U __fastcall w5_action_check_move_to_object_005101a0(void* self, void*, void* node) {
    if (blocked(self)) return k_failed_00745dc4;                                   // 005101A5..D3
    U id = at<U>(node, 0x38), run = at<U>(node, 0x3c);                              // [esp+C], [esp+18]
    Vector pos = vec(0, 0, 0);                                                      // [esp+1C]
    float range = 0.0f;                                                             // [esp+14]
    void* obj = nullptr;                                                            // [esp+10]
    if (!lookup(id, &obj)) return k_failed_00745dc4;                                // 005101E3..2C
    void* area = recovered_object_area_004cb120(obj, nullptr);                      // 00510232..41 (EDI)
    w5_creature_get_use_range_004ee440(self, nullptr, id, &pos, &range, 0);          // 0051023B..50
    if (area == nullptr) {                                                          // 00510255..57
        if (as_creature(obj) == nullptr) return k_failed_00745dc4;                  // 00510259..64
        area = module_get_area_004c30f0(server_get_module_004ae6b0(server_app(), nullptr), nullptr); // 0051026A..7A
        copy_vector(&pos, bytes(as_creature(obj)) + 0x314);                         // 0051027F..A1
        if (area == nullptr) return k_failed_00745dc4;                              // 005102A5
    }
    if (area != recovered_object_area_004cb120(self, nullptr) ||                     // 005102AB..B4
        w5_creature_get_is_in_use_range_004f6000(self, nullptr, id, 0.0f, 0) == 0) { // 005102B6..C8
        front(self, 2, group_of(node), {3, addr(&id), 1, addr(&run)});              // 005102CE..19
        const U area_id = at<U>(area, 0x120);                                       // 005102F0
        Vector zero = vec(0, 0, 0);                                                 // [esp+28]
        w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, group_of(node), &pos, area_id, id, run,
            range, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0);                         // 0051031E..6D
        kotor_fastcall_004eb5a0(self, nullptr, group_of(node));                      // 00510372..7B
    }
    w5_creature_set_lock_orientation_to_object_004f34a0(self, nullptr, k_object_invalid, 0); // 00510380..8A
    return k_complete_00745dc0;
}

// =========================================================================================================
// 005103B0 CSWSCreature::AIActionCheckMoveToObjectRadius: +38 id, +3C run, +40 range, +44 radius, +48, +4C flag, +50
U __fastcall w5_action_check_move_to_object_radius_005103b0(void* self, void*, void* node) {
    if (blocked(self)) return k_failed_00745dc4;                                   // 005103B5..E3
    U id = at<U>(node, 0x38), run = at<U>(node, 0x3c), range_bits = at<U>(node, 0x40); // [10], [24], [20]
    U radius = at<U>(node, 0x44), p48 = at<U>(node, 0x48);                          // [18], [2C]
    U flag = 0;                                                                     // [14]
    if (at<U>(node, 0x4c) != 0) flag = at<U>(node, 0x4c);                            // 00510401..1F
    U p50 = at<U>(node, 0x50);                                                      // [1C]
    Vector pos = vec(0, 0, 0);                                                      // [esp+30]
    float use_range;                                                                // [esp+28] (GetUseRange writes it first)
    void* obj = nullptr;                                                            // [esp+C]
    if (!lookup(id, &obj)) return k_failed_00745dc4;                                // 00510426..62
    if (obj == nullptr || as_object(obj) == nullptr) return k_failed_00745dc4;     // 00510468..7B
    w5_creature_get_use_range_004ee440(self, nullptr, id, &pos, &use_range, 0);      // 00510481..94
    void* area = recovered_object_area_004cb120(as_object(obj), nullptr);           // 00510499..AF (EBX)
    if (recovered_object_area_004cb120(as_object(obj), nullptr) == nullptr) {       // 005104A9..BD
        if (as_creature(obj) == nullptr) return k_failed_00745dc4;                  // 005104BF..CA
        area = module_get_area_004c30f0(server_get_module_004ae6b0(server_app(), nullptr), nullptr); // 005104D0..EB
        copy_vector(&pos, bytes(as_creature(obj)) + 0x314);                         // 005104E5..05
    }
    if (area == nullptr) return k_failed_00745dc4;                                  // 00510509..0B
    if (recovered_object_area_004cb120(self, nullptr) == nullptr) return k_failed_00745dc4; // 00510511..1A
    const bool far_area = area != recovered_object_area_004cb120(self, nullptr);    // 00510521..3A (EBP)
    const double slack = static_cast<double>(flt(radius)) - use_range;              // 00510528..2C
    float extra;                                                                    // node argument slot [E+4]
    if (k_zero_0073d700 > slack) extra = 0.0f;                                      // 00510532..52 (test ah,41h)
    else extra = static_cast<float>(slack);                                         // 00510554
    const U in_range = w5_creature_get_is_in_use_range_004f6000(self, nullptr, id, extra, flag); // 00510558..69
    if (!far_area && in_range != 0) return k_complete_00745dc0;                     // 0051056E..75
    front(self, 0x11, group_of(node), {3, addr(&id), 1, addr(&run), 2, addr(&range_bits), 2, addr(&radius),
                                       1, addr(&p48), 1, addr(&flag), 1, addr(&p50)}); // 0051057B..D5
    const U area_id = at<U>(area, 0x120);                                           // 0051059A
    Vector zero = vec(0, 0, 0);                                                     // [esp+3C]
    w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, group_of(node), &pos, area_id, id, run,
        flt(range_bits), 0.0f, 0, 0, 0, 0, 0, 1, &zero, 0, 0, p50);                 // 005105DA..32
    kotor_fastcall_004eb5a0(self, nullptr, group_of(node));                          // 00510637..40
    return k_complete_00745dc0;
}

// =========================================================================================================
// 00510AB0 CSWSCreature::AIActionCheckMoveToFollowRadius: +38..40 point, +44 area id, +50 radius
U __fastcall w5_action_check_move_to_follow_radius_00510ab0(void* self, void*, void* node) {
    if (blocked(self)) return k_failed_00745dc4;                                   // 00510AB4..E2
    const float x = at<float>(node, 0x38), y = at<float>(node, 0x3c), z = at<float>(node, 0x40);
    const U area_id = at<U>(node, 0x44);                                            // EDI
    const float radius = at<float>(node, 0x50);                                     // [esp+10]
    void* obj = nullptr;                                                            // [esp+14]
    if (!lookup(area_id, &obj)) return k_failed_00745dc4;                           // 00510B03..2B
    const bool far_area = other_area(self, area_id);                                // 00510B31..5B
    const double dx = static_cast<double>(at<float>(self, 0x90)) - x;               // 00510B5D..63
    const double dy = static_cast<double>(at<float>(self, 0x94)) - y;               // 00510B67..6D
    const double dz = static_cast<double>(at<float>(self, 0x98)) - z;               // 00510B71..77
    if (!far_area) {
        const double r = static_cast<double>(radius) + k_radius_slack_0073dfec;     // 00510B7D..81
        const double sum = (dz * dz + dy * dy) + dx * dx;                           // 00510B87..95
        if (!(r * r < sum)) return k_complete_00745dc0;                             // 00510B97..AA (test ah,5; jp)
    }
    const U client_id = kotor_fastcall_005eda50(client_app(), nullptr, at<U>(self, 4)); // 00510BB4..C0
    const I index = static_cast<I>(recovered_party_index_006349c0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr,
                                                                  client_id));     // 00510BC5..DB
    if (index >= 0)                                                                 // 00510BDD..DF (jl)
        at<U>(kotor_fastcall_005ed8b0(client_app(), nullptr), static_cast<U>(index) * 0x88 + 0x28) = 5; // 00510BE1..F5
    return k_complete_00745dc0;
}

// =========================================================================================================
// 005136B0 CSWSCreature::AIActionCheckForceFollowObject: +38 id, +3C, +40, +44
U __fastcall w5_action_check_force_follow_object_005136b0(void* self, void*, void* node) {
    Vector here;                                                                    // [esp+18]
    copy_vector(&here, bytes(self) + 0x90);                                         // 005136D4..F7
    U id = at<U>(node, 0x38), p3c = at<U>(node, 0x3c), p40 = at<U>(node, 0x40), p44 = at<U>(node, 0x44);
    U zero_a = 0;                                                                   // [esp+8]
    front(self, 0x3a, 0xffff, {3, addr(&id), 1, addr(&p3c), 2, addr(&p40), 2, addr(&p44)}); // 005136B4..4F
    front(self, 0x37, 0xffff, {3, addr(&id), 1, addr(&p3c), 2, addr(&p40), 2, addr(&p44), 1, addr(&zero_a),
                               2, addr(&here.x), 2, addr(&here.y)});                // 00513754..A9
    return k_complete_00745dc0;
}

// =========================================================================================================
// 005132E0 CSWSCreature::AIActionForceFollowObject: +38 id, +3C run, +40/+44 offset (x,y), +48 mode,
// +4C/+50 last goal (x,y)
U __fastcall w5_action_force_follow_object_005132e0(void* self, void*, void* node) {
    if (vcall<U>(self, 0x94) != 0) return k_failed_00745dc4;                        // 005132E6..F2
    if (kotor_m15e_slot010_zero_0063e7f0(self, nullptr) != 0) return k_failed_00745dc4; // 005132F8..01
    if (recovered_creature_incapacitated_004ef890(self, nullptr) != 0) return k_failed_00745dc4; // 00513307..10
    U id = at<U>(node, 0x38), run = at<U>(node, 0x3c);                              // [esp+18], [esp+14]
    Vector offset = vec(at<U>(node, 0x40), at<U>(node, 0x44), 0);                   // [esp+28]: z = EAX (0 after the checks)
    U mode = at<U>(node, 0x48);                                                     // [esp+C]
    const float last_x = at<float>(node, 0x4c), last_y = at<float>(node, 0x50);     // [esp+40], [esp+44]
    U wait = 0x3e800000u;                                                           // [esp+10] 0.25
    void* obj = kotor_fastcall_004ae750(server_app(), nullptr, id);                 // 00513340..66 (EBX)
    if (obj == nullptr || as_object(obj) == nullptr) return k_failed_00745dc4;     // 00513368..79
    Vector leader;                                                                  // [esp+1C]
    copy_vector(&leader, bytes(kotor_fastcall_005ed8b0(client_app(), nullptr)) + 0x2c0); // 0051337F..AA
    Vector rotated;                                                                 // [esp+4C]
    const Vector* r = quaternion_rotate_004a9bb0(&rotated, bytes(kotor_fastcall_005ed8b0(client_app(), nullptr)) + 0x2cc,
                                                 &offset);                          // 005133AE..C3
    Vector goal;                                                                    // [esp+34]
    vector_sum_00423b90(&goal, &leader, r);                                         // 005133C8..D3
    const U area_id = at<U>(as_object(obj), 0x8c);                                  // 005133D8..EA
    Vector d;                                                                       // [esp+1C] (reused)
    d.x = static_cast<float>(static_cast<double>(goal.x) - last_x);                 // 005133E2..F6
    d.y = static_cast<float>(static_cast<double>(goal.y) - last_y);                 // 005133FA..02
    d.z = goal.z;                                                                   // 00513406..0A
    const double sum = (sq(d.z) + sq(d.y)) + sq(d.x);
    if (mode == 0) {                                                                // 005133F0..0E
        mode = 1;                                                                   // 00513418
        if (!(sum <= k_follow_close_00747c48)) mode = 0;                            // 00513414..45 (test ah,41h; jnp)
    } else {
        const double range = kotor_fastcall_00634a70(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr); // 00513544..53
        if (!(range * range <= sum)) return k_in_progress_00745dbc;                 // 00513558..81 (test ah,41h; jp)
        mode = 0;                                                                   // 005135D9
    }
    front(self, 0x37, 0xffff, {3, addr(&id), 1, addr(&run), 2, addr(&offset.x), 2, addr(&offset.y), 1, addr(&mode),
                               2, addr(&goal.x), 2, addr(&goal.y)});                // 0051344D..9F / 00513587..E1
    if (at<U>(self, 0xa88) != 0) wait = 0x3ccccccdu;                                // 005134A4..AE 0.025
    front(self, 0x1e, group_of(node), {2, addr(&wait)});                            // 005134B6..F8
    w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, group_of(node), &goal, area_id, id, run,
        0.0f, 0.0f, 0, 0, 0, 0, 0, 0, &offset, 0, 0, 0);                            // 005134FD..31 (a13 = &offset)
    return k_complete_00745dc0;
}

// =========================================================================================================
// 0050FD50 CSWSCreature::AIActionCheckMoveAwayFromLocation: +38..40 location, +44 run, +48 distance, +4C
U __fastcall w5_action_check_move_away_from_location_0050fd50(void* self, void*, void* node) {
    if (blocked(self)) return k_failed_00745dc4;                                   // 0050FD54..81
    void* area = recovered_object_area_004cb120(self, nullptr);                     // 0050FDA3 (EBX)
    Vector from_loc = vec(at<U>(node, 0x38), at<U>(node, 0x3c), at<U>(node, 0x40)); // [esp+18]
    U run = at<U>(node, 0x44), dist = at<U>(node, 0x48), p4c = at<U>(node, 0x4c);   // [10], [C], [14]
    const double dx = static_cast<double>(at<float>(self, 0x90)) - from_loc.x;      // 0050FDA8..BC
    const double dy = static_cast<double>(at<float>(self, 0x94)) - from_loc.y;      // 0050FDC3..DC
    const double dz = static_cast<double>(at<float>(self, 0x98)) - from_loc.z;      // 0050FDE0..EA
    const double sum = (dz * dz + dy * dy) + dx * dx;                               // 0050FDEE..07
    if (!(sq(flt(dist)) >= sum)) return k_complete_00745dc0;                        // 0050FE09..1E (test ah,1: C0)
    Vector away;                                                                    // [esp+30]
    kotor_fastcall_004bdf80(area, nullptr, &away, vat(self, 0x90), &from_loc, flt(dist), pathfind(self)); // 0050FE24..39
    const U area_id = at<U>(area, 0x120);                                           // 0050FE5A
    front(self, 0x2c, group_of(node), {2, addr(&from_loc.x), 2, addr(&from_loc.y), 2, addr(&from_loc.z),
                                       1, addr(&run), 2, addr(&dist), 1, addr(&p4c)}); // 0050FE3E..95
    Vector zero = vec(0, 0, 0);                                                     // [esp+24]
    w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, group_of(node), &away, area_id,
        k_object_invalid, run, 0.0f, 0.0f, 0, 0, 0, 0, 1, 0, &zero, 0, 0, 0);       // 0050FE9A..E6
    kotor_fastcall_004eb5a0(self, nullptr, group_of(node));                          // 0050FEEB..F4
    return k_complete_00745dc0;
}

// =========================================================================================================
// 005155B0 CSWSCreature::AIActionCheckMoveAwayFromObject: +38 id, +3C run, +40 distance, +44 retries
U __fastcall w5_action_check_move_away_from_object_005155b0(void* self, void*, void* node) {
    if (blocked(self)) return k_failed_00745dc4;                                   // 005155B4..E1
    U id = at<U>(node, 0x38), run = at<U>(node, 0x3c), dist = at<U>(node, 0x40);    // [10], [14], [8]
    I retries = at<I>(node, 0x44);                                                  // [4]
    Vector target = vec(0, 0, 0);                                                   // [esp+20]
    void* obj = nullptr;                                                            // [esp+18]
    if (!lookup(id, &obj)) return k_failed_00745dc4;                                // 005155FF..40
    copy_vector(&target, bytes(obj) + 0x90);                                        // 00515646..63
    void* obj_area = recovered_object_area_004cb120(obj, nullptr);                  // 00515667..6C [esp+28]
    // 00515670..DB: cancel every queued type-3 action aimed at the same object; restart the scan after each cancel
    // (original quirk: loops forever if CancelAction removes nothing for a matching node)
    void** list = reinterpret_cast<void**>(bytes(self) + 0xfc);
    for (;;) {
        U cancelled = 0;                                                            // EBX
        void* internal = *list;
        void* head = *static_cast<void**>(internal);
        void* action = head != nullptr ? at<void*>(head, 8) : nullptr;              // ESI
        void* position = *static_cast<void**>(internal);                            // [esp+18]
        while (action != nullptr) {
            if (cancelled != 0) break;                                              // 00515696..98
            if (at<U>(action, 0) == 3 && at<U>(action, 0x38) == id) {               // 0051569A..A6
                kotor_fastcall_004f1ef0(self, nullptr, 0, group_of(action));         // 005156A8..B2
                cancelled = 1;
                continue;                                                           // 005156BC -> 005156D4
            }
            action = reinterpret_cast<void*>(static_cast<std::uintptr_t>(
                unresolved_list_next_005e9b90(*list, nullptr, &position)));         // 005156BE..D2
        }
        if (cancelled != 1) break;                                                  // 005156D8..DB
    }
    Vector d;                                                                       // [esp+38]
    d.x = static_cast<float>(static_cast<double>(at<float>(self, 0x90)) - target.x); // 005156DD..EF
    d.y = static_cast<float>(static_cast<double>(at<float>(self, 0x94)) - target.y); // 005156F3..FA
    d.z = static_cast<float>(static_cast<double>(at<float>(self, 0x98)) - target.z); // 005156FE..05
    if (obj_area != recovered_object_area_004cb120(self, nullptr)) return k_complete_00745dc0; // 00515709..14
    const double sum = (sq(d.z) + sq(d.y)) + sq(d.x);                               // 0051571A..34
    if (!(sq(flt(dist)) >= sum)) return k_complete_00745dc0;                        // 00515736..45 (test ah,1)
    if (retries < 1) return k_complete_00745dc0;                                    // 0051574B..52 (jl)
    retries = retries - 1;                                                          // 0051575E..60
    Vector away;                                                                    // [esp+44]
    kotor_fastcall_004bdf80(obj_area, nullptr, &away, vat(self, 0x90), &target, flt(dist), pathfind(self)); // 00515758..76
    const U area_id = at<U>(obj_area, 0x120);                                       // 00515793
    front(self, 3, group_of(node), {3, addr(&id), 1, addr(&run), 2, addr(&dist), 1, addr(&retries)}); // 0051577B..D3
    Vector zero = vec(0, 0, 0);                                                     // [esp+38]
    w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, group_of(node), &away, area_id,
        k_object_invalid, run, 0.0f, 0.0f, 0, 0, 0, 0, 1, 0, &zero, 0, 0, 0);       // 005157D8..2D
    kotor_fastcall_004eb5a0(self, nullptr, group_of(node));                          // 00515832..3B
    return k_complete_00745dc0;
}

// =========================================================================================================
// 0050FF20 CSWSCreature::AIActionCheckInterAreaPathfinding
U __fastcall w5_action_check_inter_area_pathfinding_0050ff20(void* self, void*, void* node) {
    if (blocked(self)) return k_failed_00745dc4;                                   // 0050FF24..51
    if (recovered_object_area_004cb120(self, nullptr) == nullptr) return k_in_progress_00745dbc; // 0050FF57..6B
    I count = at<I>(pathfind(self), 0xa8);                                          // 0050FF6E..75 (EDI)
    if (count <= 1) {                                                               // 0050FF7B..7E
        kotor_fastcall_005d0e70(pathfind(self), nullptr);                            // 00510121
    } else {
        // 0050FF84..4B: drop the first area of the inter-area path. Original quirk: both new arrays are filled
        // from +AC (the +B0 array is not copied).
        U* areas = static_cast<U*>(live_operator_new(static_cast<U>(count) * 4 - 4)); // EBP
        U* second = nullptr;                                                        // EBX
        if (count > 2) {
            second = static_cast<U*>(live_operator_new(static_cast<U>(count) * 4 - 8));
            for (I i = 0; i < count - 2; ++i) second[i] = at<U*>(pathfind(self), 0xac)[i + 1]; // 0050FFC0..D6
        }
        --count;                                                                    // 0050FFDA
        for (I i = 0; i < count; ++i) areas[i] = at<U*>(pathfind(self), 0xac)[i + 1]; // 0050FFE0..F7
        if (at<void*>(pathfind(self), 0xac) != nullptr) live_free(at<void*>(pathfind(self), 0xac)); // 0050FFF9..0F
        if (at<void*>(pathfind(self), 0xb0) != nullptr) live_free(at<void*>(pathfind(self), 0xb0)); // 00510012..28
        at<U*>(pathfind(self), 0xb0) = second;                                      // 0051002B..31
        at<U*>(pathfind(self), 0xac) = areas;                                       // 00510037..3D
        at<I>(pathfind(self), 0xa8) = count;                                        // 00510043..4B
        if (recovered_object_area_004cb120(self, nullptr) != nullptr) {             // 00510051..5A
            kotor_fastcall_004c3ea0(server_get_module_004ae6b0(server_app(), nullptr), nullptr, pathfind(self)); // 00510060..77
            void* pf = pathfind(self);
            const float gx = at<float>(pf, 0x5c), gy = at<float>(pf, 0x60);         // 00510082..88
            const U gz = at<U>(pf, 0x64), goal_area = at<U>(pf, 0x70);
            at<float>(pf, 0x5c) = static_cast<float>(static_cast<double>(gx) - k_five_00743264); // 0051008F..A0
            at<U>(pathfind(self), 0xa4) = 1;                                        // 005100A3..B7
            Vector goal = {gx, gy, flt(gz)};                                        // [esp+28]
            Vector zero = vec(0, 0, 0);                                             // [esp+1C]
            w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, group_of(node), &goal, goal_area,
                k_object_invalid, 0, 0.0f, 0.0f, 0, 0, 0, 0, 0, 0, &zero, 0, 0, 0); // 0051008B..10C
            kotor_fastcall_004eb5a0(self, nullptr, group_of(node));                  // 00510111..1A
        }
    }
    U wait = 0x3f400000u;                                                           // [esp+C] 0.75
    front(self, 0x1e, group_of(node), {2, addr(&wait)});                            // 00510126..74
    return k_complete_00745dc0;
}

// =========================================================================================================
// 0051D600 CSWSCreature::AIActionJumpToPoint: +38..40 point, +44 area id, [+48 safe-search flag (5+ params)],
// [+4C search radius (6+)], [+50/+54 facing (7+)]
U __fastcall w5_action_jump_to_point_0051d600(void* self, void*, void* node) {
    if (blocked(self)) return k_failed_00745dc4;                                   // 0051D606..34
    if (pathfind(self) == nullptr) return k_failed_00745dc4;                        // 0051D63A..42
    if (recovered_object_area_004cb120(self, nullptr) == nullptr &&                  // 0051D648..51
        kotor_fastcall_004c6450(server_get_module_004ae6b0(server_app(), nullptr), nullptr, at<U>(self, 4)) != 0) // 0051D653..6E
        kotor_fastcall_004c63f0(server_get_module_004ae6b0(server_app(), nullptr), nullptr, at<U>(self, 4)); // 0051D670..84
    U face_x = at<U>(self, 0x9c), face_y = at<U>(self, 0xa0);                       // [esp+10], [esp+14]
    const U x = at<U>(node, 0x38), y = at<U>(node, 0x3c), z = at<U>(node, 0x40);     // [E+4], [esp+20], [esp+24]
    const U area_id = at<U>(node, 0x44);                                            // EDI
    const U16 count = at<U16>(node, 0x6e);
    U safe = 1;                                                                     // EBP
    U radius = 0x41a00000u;                                                         // [esp+18] 20.0
    if (count >= 5) safe = at<U>(node, 0x48);                                       // 0051D6AE..D0 (jb)
    if (count >= 6) radius = at<U>(node, 0x4c);                                     // 0051D6D3..DC
    if (count >= 7) { face_x = at<U>(node, 0x50); face_y = at<U>(node, 0x54); }     // 0051D6E0..F0
    (void)recovered_object_area_004cb120(self, nullptr);                            // 0051D6F4..F6 (result unused)
    void* obj = nullptr;                                                            // [esp+1C]
    if (!lookup(area_id, &obj)) return k_failed_00745dc4;                           // 0051D6FB..1C
    if (obj == nullptr || at<U8>(obj, 8) != k_type_bound_00746633) return k_failed_00745dc4; // 0051D722..37
    void* area = as_area(obj);                                                      // 0051D73D..48 (EDI)
    w5_pathfind_reset_inter_tile_search_005d00a0(pathfind(self), nullptr);          // 0051D742..4A
    w5_pathfind_reset_grid_search_005cefa0(pathfind(self), nullptr);                // 0051D74F..55
    Vector safe_pos = vec(0, 0, 0);                                                 // [esp+28]
    at<U>(pathfind(self), 0x2c) = at<U>(self, 4);                                   // 0051D75A..7F
    Vector want = vec(x, y, z);                                                     // [esp+34]
    if (w1x_area_safe_location_004be860(area, nullptr, addr(&want), radius, addr(pathfind(self)), safe != 0 ? 1u : 0u,
                                        addr(&safe_pos)) != 1) {                    // 0051D782..BE
        kotor_fastcall_005d0e70(pathfind(self), nullptr);                            // 0051D9A1..A7
        return k_failed_00745dc4;
    }
    w5_creature_auto_close_gui_panels_004eded0(self, nullptr, 0);                   // 0051D7C4..C8
    kotor_fastcall_004edf80(self, nullptr);                                          // 0051D7CD..CF
    if (at<U>(self, 4) == party_leader_server_id()) {                               // 0051D7D4..F9
        Vector facing = vec(face_x, face_y, 0);                                     // [esp+34]
        Vector n;                                                                   // [esp+40]
        const float yaw = static_cast<float>(kotor_m15a_yaw_004aa0f0(normalize_004ab180(&n, &facing))); // 0051D811..33
        recovered_reset_all_follow_data_00637890(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, &safe_pos,
                                                 yaw, 1);                           // 0051D83A..48
    }
    void* player = kotor_fastcall_004b2a50(server_internal(), nullptr, at<U>(self, 4)); // 0051D84D..64 (EBP)
    if (area == recovered_object_area_004cb120(self, nullptr)) {                    // 0051D862..6D
        recovered_object_position_004cd1a0(self, nullptr, &safe_pos, 1, 1, 0);      // 0051D86F..7C
        recovered_update_subareas_0051b940(self, nullptr, &safe_pos, at<U>(area, 0x120), 1); // 0051D881..91
    } else {
        if (player != nullptr) recovered_update_subareas_0051b940(self, nullptr, &safe_pos, k_object_invalid, 1); // 0051D898..AA
        recovered_creature_placement_004fa100(self, nullptr, addr(area), bits(safe_pos.x), bits(safe_pos.y),
                                              bits(safe_pos.z), 0, 0);              // 0051D8AF..C5
        if (player == nullptr)                                                      // 0051D8CA..CC
            recovered_update_subareas_0051b940(self, nullptr, &safe_pos, at<U>(area, 0x120), 1); // -> 0051D881
        else
            front(self, 0x3c, group_of(node), {});                                  // 0051D8CE..0D
    }
    Vector facing = vec(face_x, face_y, 0);                                         // [esp+34]
    Vector n;                                                                       // [esp+40]
    w1x_object_set_orientation_004cd170(self, nullptr, normalize_004ab180(&n, &facing)); // 0051D912..3F
    front(self, 4, group_of(node), {});                                             // 0051D944..83
    at<U>(self, 0xa18) = 1;                                                         // 0051D989
    return k_complete_00745dc0;
}

// =========================================================================================================
// 0051D110 CSWSCreature::AIActionJumpToObject: +38 object id, [+3C safe-search flag (2+ params, default 1)]
U __fastcall w5_action_jump_to_object_0051d110(void* self, void*, void* node) {
    if (blocked(self)) return k_failed_00745dc4;                                   // 0051D119..49
    if (pathfind(self) == nullptr) return k_failed_00745dc4;                        // 0051D14F..57
    U safe = 1;                                                                     // [esp+20]
    if (at<U16>(node, 0x6e) > 1) safe = at<U>(node, 0x3c);                          // 0051D164..74 (jbe)
    void* obj = kotor_fastcall_004ae750(server_app(), nullptr, at<U>(node, 0x38));  // 0051D178..87 (EDI)
    if (obj == nullptr || as_object(obj) == nullptr) return k_failed_00745dc4;     // 0051D189..9A
    void* area = recovered_object_area_004cb120(as_object(obj), nullptr);           // 0051D1A0..AE (EBX, [esp+1C])
    Vector target = vec(0, 0, 0);                                                   // [esp+10]
    Vector orientation = vec(0, 0, 0);                                              // [esp+24]
    Vector safe_pos = vec(0, 0, 0);                                                 // [esp+30]
    const U type = at<U8>(obj, 8);                                                  // 0051D1F8
    if (type == 0xa) {                                                              // 0051D1FC..03 door
        void* door = as_door(obj);                                                  // 0051D294..9B (EBX)
        const Vector* door_face = vat(as_door(obj), 0x9c);                          // 0051D29D..A4
        Vector back;                                                                // [esp+40]
        kotor_cdecl_00423bc0(&back, vat(door, 0x90), door_face);                    // 0051D2A9..B6
        Vector point;                                                               // [esp+70]
        copy_vector(&target, w5_door_get_nearest_action_point_00589240(door, nullptr, &point, &back, 1)); // 0051D2BE..E7
        const float twice = static_cast<float>(static_cast<double>(at<float>(pathfind(self), 8)) + at<float>(pathfind(self), 8)); // 0051D2EB..FF
        Vector scaled;                                                              // [esp+88]
        const Vector* s = float_times_vector_00437a60(&scaled, twice, vat(door, 0x9c)); // 0051D2F6..03
        Vector moved;                                                               // [esp+64]
        copy_vector(&target, kotor_cdecl_00423bc0(&moved, &target, s));             // 0051D308..36
        Vector to_door;                                                             // [esp+7C]
        const Vector* t = kotor_cdecl_00423bc0(&to_door, &target, vat(door, 0x90));  // 0051D328..3A
        Vector n;                                                                   // [esp+4C]
        copy_vector(&orientation, normalize_004ab180(&n, t));                       // 0051D33F..6A
    } else if (type == 0xc) {                                                       // 0051D209..0C
        copy_vector(&target, bytes(as_object(obj)) + 0x90);                          // 0051D25A..76
        copy_vector(&orientation, bytes(as_object(obj)) + 0x9c);                     // 0051D27A..6A
    } else {
        copy_vector(&target, bytes(obj) + 0x90);                                    // 0051D20E..24
        Vector d;                                                                   // [esp+58]
        const Vector* p = kotor_cdecl_00423bc0(&d, vat(as_object(obj), 0x90), &target); // 0051D228..3F
        Vector n;                                                                   // [esp+94]
        copy_vector(&orientation, normalize_004ab180(&n, p));                       // 0051D244..6A
    }
    if (area == nullptr) {                                                          // 0051D36E
        if (as_creature(obj) == nullptr) return k_failed_00745dc4;                  // 0051D370..79
        void* creature = as_creature(obj);                                          // 0051D37F..8F (EDI)
        void* found = kotor_fastcall_004ae780(server_app(), nullptr, at<U>(creature, 0x310)); // 0051D386..98
        copy_vector(&target, bytes(creature) + 0x314);                               // 0051D39D..B9
        if (found == nullptr) return k_failed_00745dc4;                             // 0051D3BD
        area = found;                                                               // 0051D3C3
    }
    w5_pathfind_reset_inter_tile_search_005d00a0(pathfind(self), nullptr);          // 0051D3C5..CB
    w5_pathfind_reset_grid_search_005cefa0(pathfind(self), nullptr);                // 0051D3D0..D6
    at<U>(pathfind(self), 0x2c) = at<U>(self, 4);                                   // 0051D3DB..E4
    if (w1x_area_safe_location_004be860(area, nullptr, addr(&target), 0x41a00000u, addr(pathfind(self)), safe,
                                        addr(&safe_pos)) != 1) {                    // 0051D3E7..0C
        kotor_fastcall_005d0e70(pathfind(self), nullptr);                            // 0051D5E0..E6
        return k_failed_00745dc4;
    }
    w5_creature_auto_close_gui_panels_004eded0(self, nullptr, 0);                   // 0051D412..16
    kotor_fastcall_004edf80(self, nullptr);                                          // 0051D41B..1D
    const U face_y = at<U>(self, 0xa0), face_x = at<U>(self, 0x9c);                 // 0051D422..3B
    if (at<U>(self, 4) == party_leader_server_id()) {                               // 0051D43F..5B
        Vector facing = vec(face_x, face_y, 0);                                     // [esp+3C]
        Vector n;                                                                   // [esp+48]
        const float yaw = static_cast<float>(kotor_m15a_yaw_004aa0f0(normalize_004ab180(&n, &facing))); // 0051D473..95
        recovered_reset_all_follow_data_00637890(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, &safe_pos,
                                                 yaw, 1);                           // 0051D49C..AA
    }
    void* player = kotor_fastcall_004b2a50(server_internal(), nullptr, at<U>(self, 4)); // 0051D4AF..C4 (EDI)
    w1x_object_set_orientation_004cd170(self, nullptr, &orientation);              // 0051D4C6..CD
    if (area == recovered_object_area_004cb120(self, nullptr)) {                    // 0051D4D2..DB
        recovered_object_position_004cd1a0(self, nullptr, &safe_pos, 1, 1, 0);      // 0051D4DD..EA
        recovered_update_subareas_0051b940(self, nullptr, &safe_pos, at<U>(area, 0x120), 1); // 0051D4EF..FF
    } else {
        if (player != nullptr) recovered_update_subareas_0051b940(self, nullptr, &safe_pos, k_object_invalid, 1); // 0051D506..18
        recovered_creature_placement_004fa100(self, nullptr, addr(area), bits(safe_pos.x), bits(safe_pos.y),
                                              bits(safe_pos.z), 0, 0);              // 0051D51D..33
        if (player == nullptr)                                                      // 0051D538..3A
            recovered_update_subareas_0051b940(self, nullptr, &safe_pos, at<U>(area, 0x120), 1); // -> 0051D4EF
        else
            front(self, 0x3c, group_of(node), {});                                  // 0051D53C..7B
    }
    front(self, 4, group_of(node), {});                                             // 0051D580..BF
    at<U>(self, 0xa18) = 1;                                                         // 0051D5C5
    return k_complete_00745dc0;
}

// =========================================================================================================
// 00515AC0 CSWSCreature::AIActionRandomWalk: +38/+3C/+40 centre, +44 area id
U __fastcall w5_action_random_walk_00515ac0(void* self, void*, void* node) {
    U cx = at<U>(node, 0x38), cy = at<U>(node, 0x3c), cz = at<U>(node, 0x40), area_id = at<U>(node, 0x44);
    U wait = 0x40400000u;                                                           // [esp+2C] 3.0
    void* area = kotor_fastcall_004ae780(server_app(), nullptr, area_id);           // 00515AE6..05 (EBX)
    if (area == nullptr) return k_failed_00745dc4;                                  // 00515B07..19
    while (*static_cast<void**>(at<void*>(self, 0xfc)) != nullptr) {                 // 00515B1C..4C: clear the queue
        void* removed = base_ini_reconstruction::remove_head_005e9c80(
            static_cast<kotor_list::Internal*>(at<void*>(self, 0xfc)), nullptr);    // 00515B28
        if (removed != nullptr) {
            recovered_action_node_destroy_004cc4d0(removed, nullptr);               // 00515B33..35
            live_free(removed);                                                     // 00515B3A..40
        }
    }
    if (at<I>(self, 0x78) <= 0) {                                                   // 00515B58..77 (jle)
        wait = 0x41700000u;                                                         // 00515FA4 15.0
        front(self, 0x2d, group_of(node), {2, addr(&cx), 2, addr(&cy), 2, addr(&cz), 3, addr(&area_id)}); // 00515F74..B2
        front(self, 0x1e, group_of(node), {2, addr(&wait)});                        // 00515FB7..FC
        return k_complete_00745dc0;
    }
    front(self, 0x2d, group_of(node), {2, addr(&cx), 2, addr(&cy), 2, addr(&cz), 3, addr(&area_id)}); // 00515B4E..B0
    front(self, 0x1e, group_of(node), {2, addr(&wait)});                            // 00515BB5..FA
    Vector goal = vec(cx, cy, 0);                                                   // [esp+24]
    Vector dir = vec(0, 0, 0);                                                      // [esp+14]
    I tries = crt_rand_006fc447() % 15;                                             // 00515C2F..3C ([esp+20])
    goal.x = static_cast<float>((static_cast<double>(tries) - k_seven_00747c6c) + goal.x); // 00515C40..4E
    tries = crt_rand_006fc447() % 15;                                               // 00515C52..63
    goal.y = static_cast<float>((static_cast<double>(tries) - k_seven_00747c6c) + goal.y); // 00515C67..7A
    at<U>(&goal, 8) = w1x_area_height_bits_004bc380(area, nullptr, &goal, 1, nullptr); // 00515C5F..85
    Vector* here = vat(self, 0x90);                                                 // EDI
    dir.x = static_cast<float>(static_cast<double>(goal.x) - here->x);              // 00515C89..A1
    dir.y = static_cast<float>(static_cast<double>(goal.y) - here->y);              // 00515C9D..BC
    dir.z = static_cast<float>(static_cast<double>(goal.z) - here->z);              // 00515CB1..C8
    float length = static_cast<float>(kotor_cdecl_004ab100(&dir));                  // 00515CCC..D1 ([esp+10])
    const float full_length = length;                                               // [esp+68]
    const float min_length = static_cast<float>(static_cast<double>(length) * k_three_tenths_00746d38); // 00515CD5..F0 [esp+64]
    Vector n;                                                                       // [esp+84]
    copy_vector(&dir, normalize_004ab180(&n, &dir));                                // 00515CE3..19
    at<void*>(area, 0x1ac) = pathfind(self);                                        // 00515D06..13
    tries = 0x14;                                                                   // 00515D48
    float radius = at<float>(pathfind(self), 4);                                    // [esp+3C]
    U band = at<U>(pathfind(self), 0x14);                                           // [esp+40]
    I hit = w5_area_test_direct_line_004bcb70(area, nullptr, here, &goal, &radius, flt(band), 0, nullptr); // 00515D1D..50
    while (hit != 1) {                                                              // 00515D55..58
        const double shorter = static_cast<double>(length) * k_three_quarters_0073e31c; // 00515D60..64
        length = static_cast<float>(shorter);                                       // 00515D6A (fst)
        if (shorter < static_cast<double>(min_length)) {                            // 00515D6E..77 (unrounded ST0; test ah,5; jp)
            (void)crt_rand_006fc447();                                              // 00515D7D (result unused)
            Vector up = vec(0, 0, 0x3f800000u);                                     // [esp+78]
            Vector c1;                                                              // [esp+90]
            copy_vector(&dir, vector_cross_004ab210(&c1, &up, &dir));                // 00515D82..C7
            Vector up2 = vec(0, 0, 0x3f800000u);                                    // [esp+58]
            Vector c2;                                                              // [esp+9C]
            const Vector* r = vector_cross_004ab210(&c2, &up2, &dir);                // 00515DCA..F8
            dir.x = static_cast<float>(-static_cast<double>(r->x));                 // 00515DFD..10
            dir.y = static_cast<float>(-static_cast<double>(r->y));                 // 00515E14..25
            dir.z = static_cast<float>(-static_cast<double>(r->z));                 // 00515E29..3E
            length = full_length;                                                   // 00515E37..51
        }
        const double ax = static_cast<double>(dir.x) * length, ay = static_cast<double>(dir.y) * length; // 00515E55..61
        const float az = static_cast<float>(static_cast<double>(dir.z) * length);   // 00515E65..6D
        goal.x = static_cast<float>(ax + here->x);                                  // 00515E74..80
        goal.y = static_cast<float>(ay + here->y);                                  // 00515E84..99
        goal.z = static_cast<float>(static_cast<double>(az) + here->z);             // 00515E8B..AF
        if (static_cast<double>(length) < k_one_0073d764) return k_complete_00745dc0; // 00515EA1..B8 (test ah,5; jnp)
        const I before = tries;                                                     // 00515EBE..CB
        tries = tries - 1;
        if (before < 0) return k_complete_00745dc0;
        radius = at<float>(pathfind(self), 4);                                      // 00515ED1..DC
        band = at<U>(pathfind(self), 0x14);
        hit = w5_area_test_direct_line_004bcb70(area, nullptr, here, &goal, &radius, flt(band), 0, nullptr); // 00515EDA..F9
    }
    Vector zero = vec(0, 0, 0);                                                     // [esp+54]
    w5_creature_add_move_to_point_action_to_front_004f8a50(self, nullptr, group_of(node), &goal, area_id,
        k_object_invalid, 0, 0.0f, 0.0f, 0, 0, 0, 0, 1, 0, &zero, 0, 0, 0);         // 00515F07..5D
    return k_complete_00745dc0;
}

// =========================================================================================================
// ============================== shared helpers (no provider anywhere) ====================================
// Candidates for dedupe: other action groups use AddPathfindingWaitActionToFront, CutWaypointPath,
// ResetInterAreaSearchData, CancelAction and the module limbo queries too.

// 004EB5A0 CSWSCreature::AddPathfindingWaitActionToFront(group): wait 0.3 s (0.1 s for a player creature)
U __fastcall kotor_fastcall_004eb5a0(void* creature, void*, U group) {
    U wait = 0x3e99999au;                                                           // 004EB5AC 0.3
    if (kotor_fastcall_004ae910(server_app(), nullptr, at<U>(creature, 4)) != nullptr) wait = 0x3dcccccdu; // 004EB5A9..C1 0.1
    front(creature, 0x1e, group, {2, addr(&wait)});                                 // 004EB5C9..09
    return 1;
}

// 004EDF80 CSWSCreature::CutWaypointPath
void __fastcall kotor_fastcall_004edf80(void* creature, void*) {
    void* pf = pathfind(creature);
    const U8 serial = at<U8>(pf, 0x100);                                            // 004EDF86
    at<U>(pf, 0xf8) = 1;                                                            // 004EDF8C
    at<U8>(pf, 0x100) = static_cast<U8>(serial + 1);                                // 004EDF96..98
    at<U>(pathfind(creature), 0xfc) = 0;                                            // 004EDF9E..A4
    pf = pathfind(creature);
    at<I>(pf, 0x104) = (at<I>(pf, 0x8c) - at<I>(pf, 0x9c)) >> 1;                    // 004EDFAE..C3 (sar)
    copy_vector(bytes(pathfind(creature)) + 0x108, bytes(creature) + 0x90);          // 004EDFC9..E8
    copy_vector(bytes(pathfind(creature)) + 0x114, bytes(creature) + 0x9c);          // 004EDFEB..09
}

// 005D0E70 CPathfindInformation::ResetInterAreaSearchData
void __fastcall kotor_fastcall_005d0e70(void* pf, void*) {
    if (at<void*>(pf, 0xb8) != nullptr) live_free(at<void*>(pf, 0xb8));             // 005D0E73..86
    at<U>(pf, 0x84) = 0; at<U>(pf, 0x1c) = 0; at<U>(pf, 0x20) = 0; at<U>(pf, 0x24) = 0; // 005D0E89..95
    at<U>(pf, 0xb4) = 0; at<U>(pf, 0xb8) = 0;                                       // 005D0E98..9E
    at<U>(pf, 0x88) = 1;                                                            // 005D0EA5
    w5_pathfind_reset_inter_area_search_005d0e00(pf, nullptr);                      // 005D0EB2 (tail jump)
}

// 004C6450 CSWSModule::IsObjectInLimbo(id): +1DC ids, +1E0 count, scanned from the end
U __fastcall kotor_fastcall_004c6450(void* module, void*, U id) {
    I i = at<I>(module, 0x1e0) - 1;                                                 // 004C6450..57
    if (i < 0) return 0;
    const U* ids = at<const U*>(module, 0x1dc) + i;
    for (;;) {
        if (id == *ids) return 1;                                                   // 004C6466..68
        --i; --ids;                                                                 // 004C646A..6B
        if (i < 0) return 0;                                                        // 004C646E..70
    }
}

// 004C63F0 CSWSModule::RemoveObjectFromLimbo(id)
void __fastcall kotor_fastcall_004c63f0(void* module, void*, U id) {
    I i = at<I>(module, 0x1e0) - 1;                                                 // 004C63F0..F7
    if (i < 0) return;
    const U* ids = at<const U*>(module, 0x1dc) + i;
    for (;;) {
        if (id == *ids) break;                                                      // 004C6407..09
        --i; --ids;
        if (i < 0) return;                                                          // 004C640F..14
    }
    const I count = at<I>(module, 0x1e0) - 1;                                       // 004C6417..22
    at<I>(module, 0x1e0) = count;
    if (!(i < count)) return;                                                       // 004C6420..28 (jge)
    do {                                                                            // 004C6430..48 (array and count re-read)
        U* array = at<U*>(module, 0x1dc);
        array[i] = array[i + 1];
        ++i;
    } while (i < at<I>(module, 0x1e0));
}

// 005CEC90 CPathfindInformation::ComputeStepTolerance: +6C = ftol(+68 / +4), +68 itself when +4 == 0
void __fastcall kotor_fastcall_005cec90(void* pf, void*) {
    double v = at<float>(pf, 0x68);                                                 // 005CEC9C
    if (!(at<float>(pf, 4) == k_zero_0073d700)) v = v / at<float>(pf, 4);           // 005CEC93..A6 (test ah,44h; jnp)
    at<U>(pf, 0x6c) = ftol2(v);                                                     // 005CECA9..AE
}

// 0058D8C0 CSWSTrigger::GetClosestPointWithinTrigger(point, out): 5x5 grid over the trigger bounds, nearest
// (in xy, z of the probe = 0) grid point inside the trigger
U __fastcall kotor_fastcall_0058d8c0(void* trigger, void*, const Vector* p, Vector* out) {
    float min_x, min_y, max_x, max_y;                                               // [esp+4], [C], [14], [10]
    float best = flt(0x51ba43b7u);                                                  // [esp+28] 1e11
    U best_x = 0, best_y = 0, best_z = 0;                                           // EBP, [esp+44], [esp+48]
    recovered_trigger_bounds_0058cc60(trigger, nullptr, &min_x, &min_y, &max_x, &max_y); // 0058D8C4..FC
    float x = min_x;                                                                // [esp+8]
    const float step_x = static_cast<float>((static_cast<double>(max_x) - min_x) * k_fifth_0073e46c); // 0058D901..17
    const float step_y = static_cast<float>((static_cast<double>(max_y) - min_y) * k_fifth_0073e46c); // 0058D91B..29
    if (!(min_x <= max_x)) return 0;                                                // 0058D92D..3A (test ah,41h; jp)
    double x_sum;                                                                   // unrounded x87 x + step (compared, 0058DA30)
    do {
        float y = min_y;                                                            // 0058D950..5C [esp+4]
        if (min_y <= max_y) {                                                       // 0058D958..65
            const double dx = static_cast<double>(p->x) - x;                        // 0058D96F..75
            const double dz = p->z;                                                 // 0058D981 (probe z = 0)
            const float dz2 = static_cast<float>(dz * dz);                          // 0058D98C..92 [esp+30]
            const float dx2 = static_cast<float>(dx * dx);                          // 0058D998..9C [esp+2C]
            double y_sum;                                                           // unrounded x87 y + step (compared, 0058DA19)
            do {
                const double dy = static_cast<double>(p->y) - y;                    // 0058D9B0..B7
                Vector probe = {x, y, 0.0f};                                        // [esp+58]
                const double d = (dy * dy + dx2) + dz2;                             // 0058D9BD..D5
                const float df = static_cast<float>(d);                             // 0058D9D9 [esp+34]
                if (d < static_cast<double>(best) &&                                // 0058D9DD..E6 (test ah,5; jp)
                    w5_trigger_in_trigger_0058ce40(trigger, nullptr, &probe) == 1) { // 0058D9E8..F9
                    best_x = bits(x); best_y = bits(y); best_z = 0; best = df;      // 0058D9FB..09
                }
                y_sum = static_cast<double>(y) + step_y;               // 0058DA0D (fst keeps the unrounded sum)
                y = static_cast<float>(y_sum);                                      // 0058DA15
            } while (y_sum <= max_y);                                                  // 0058DA19..22 (test ah,41h; jnp)
        }
        x_sum = static_cast<double>(x) + step_x;                       // 0058DA24 (fst keeps the unrounded sum)
        x = static_cast<float>(x_sum);                                              // 0058DA2C
    } while (x_sum <= max_x);                                                          // 0058DA30..39
    if (!(best < k_far_0074325c)) return 0;                                         // 0058DA3F..51 (test ah,5; jp)
    *out = vec(best_x, best_y, best_z);                                             // 0058DA53..64
    return 1;
}

// 004C3EA0 CSWSModule::SetIntraAreaGoal(pathfind): goal = nearest point of the next transition object
void __fastcall kotor_fastcall_004c3ea0(void* module, void*, void* pf) {
    (void)module;
    if (at<I>(pf, 0xa8) <= 1) return;                                               // 004C3EA4..AF (jle)
    at<U>(pf, 0x44) = at<U>(pf, 0x5c); at<U>(pf, 0x48) = at<U>(pf, 0x60); at<U>(pf, 0x4c) = at<U>(pf, 0x64); // 004C3EB5..CA
    at<U>(pf, 0x54) = at<U>(pf, 0x70); at<U>(pf, 0x50) = at<U>(pf, 0x68);           // 004C3EC1..D6
    void* obj = nullptr;                                                            // over the argument slot
    if (!lookup(*at<U*>(pf, 0xb0), &obj)) return;                                   // 004C3ED0..FC
    const U type = at<U8>(obj, 8);
    Vector goal;                                                                    // [esp+8]
    Vector from = vec(at<U>(pf, 0x74), at<U>(pf, 0x78), at<U>(pf, 0x7c));           // [esp+14] (fld/fstp copies)
    void* target;                                                                   // EDI
    if (type == k_type_trigger_00746636) {                                          // 004C3F02..10
        target = as_trigger(obj);                                                   // 004C3F12..14
        goal = vec(0, 0, 0);                                                        // 004C3F3A..4A
        if (kotor_fastcall_0058d8c0(target, nullptr, &from, &goal) != 1) return;     // 004C3F52..5A
        at<U>(pf, 0x68) = 0;                                                        // 004C3F5C
    } else if (type == k_type_door_00746639) {                                      // 004C3F65..6B
        target = as_door(obj);                                                      // 004C3F6D..6F
        w5_door_get_nearest_action_point_00589240(target, nullptr, &goal, &from, 1); // 004C3F72..97
        at<float>(pf, 0x68) = static_cast<float>(static_cast<double>(at<float>(pf, 4)) + k_half_0073e9ac); // 004C3F9C..A5
    } else {
        return;                                                                     // 004C3F6B
    }
    at<float>(pf, 0x60) = goal.y; at<float>(pf, 0x64) = goal.z; at<float>(pf, 0x5c) = goal.x; // 004C3FA8..BC
    kotor_fastcall_005cec90(pf, nullptr);                                            // 004C3FBF
    at<U>(pf, 0x70) = at<U>(pf, 0x80);                                              // 004C3FC4..CA
    at<U>(pf, 0x30) = at<U>(target, 4);                                             // 004C3FCD..D0
}

// 004BDF80 CSWSArea::ComputeAwayVector(out, from, away-from point, distance, pathfind): best of 8 directions
Vector* __fastcall kotor_fastcall_004bdf80(void* area, void*, Vector* out, const Vector* from, const Vector* target,
                                           float dist, void* pf) {
    Vector d;                                                                       // [esp+44]
    d.x = static_cast<float>(static_cast<double>(target->x) - from->x);             // 004BDF8F..A5
    d.y = static_cast<float>(static_cast<double>(target->y) - from->y);             // 004BDFAA..C0
    d.z = 0.0f;                                                                     // 004BDFB8
    const double tz = static_cast<double>(target->z) - from->z;                     // 004BDFC4..C7
    const float base_sq = static_cast<float>((sq(d.y) + sq(d.x)) + tz * tz);        // 004BDFCA..E2 [esp+58]
    Vector n;                                                                       // [esp+80]
    normalize_004ab180(&n, &d);                                                     // 004BDFE8
    const float dist_sq = static_cast<float>(static_cast<double>(dist) * dist);     // 004BDFED..0C [esp+80]
    const float dz = static_cast<float>(static_cast<double>(dist) * k_zero_0073d700); // 004BE013..31 [esp+28]
    float best = 0.0f;                                                              // [esp+40]
    Vector result;                                                                  // [esp+34]
    copy_vector(&result, from);                                                     // 004BE002..39
    Vector cand = vec(0, 0, 0);                                                     // [esp+10] (written before every read)
    for (I i = 0; i < 8; ++i) {                                                     // 004BE041..6A
        float angle_f, rx;                                                          // [esp+20] [esp+A4]
        double ry;                                                                  // ST0 after 004BE091
        x87trig_004be041(&i, &n.x, &angle_f, &rx, &ry);                             // 004BE041..91 (x87)
        const double rxd = static_cast<double>(rx) * dist;                          // 004BE093..9A
        const float rxd_f = static_cast<float>(rxd);                                // 004BE0A1 [esp+2C]
        const float ryd_f = static_cast<float>(ry * dist);                          // 004BE0A5..AE [esp+20]
        cand.x = static_cast<float>(rxd + from->x);                                 // 004BE0B6..BF
        cand.y = static_cast<float>(static_cast<double>(ryd_f) + from->y);          // 004BE0C3..CD
        cand.z = static_cast<float>(static_cast<double>(dz) + from->z);             // 004BE0D1..DB
        {
            const double ex = static_cast<double>(target->x) - cand.x, ey = static_cast<double>(target->y) - cand.y;
            const double ez = static_cast<double>(target->z) - cand.z;              // 004BE0DF..F0
            if ((ez * ez + ey * ey) + ex * ex < static_cast<double>(dist_sq)) {     // 004BE0F4..13 (test ah,5; jp)
                cand.x = static_cast<float>(static_cast<double>(rxd_f) + cand.x);   // 004BE115..34
                cand.y = static_cast<float>(static_cast<double>(ryd_f) + cand.y);   // 004BE138..4F
                cand.z = static_cast<float>(static_cast<double>(dz) + cand.z);      // 004BE140..5B
            }
        }
        if (pf == nullptr) continue;                                                // 004BE15F..61
        at<void*>(area, 0x1ac) = pf;                                                // 004BE167..6D
        float radius = at<float>(pf, 4);                                            // [esp+64]
        bool score = w5_area_test_direct_line_004bcb70(area, nullptr, from, &cand, &radius, at<float>(pf, 0x14), 1,
                                                       nullptr) == 1;               // 004BE16B..99
        if (!score) {
            U found = 0;                                                            // EBX
            float scale = 1.0f;                                                     // [esp+1C]
            for (;;) {
                if (found != 0) break;                                              // 004BE1B0..B2
                const float sx = static_cast<float>(static_cast<double>(rxd_f) * scale); // 004BE1B8..D4 [esp+C8]
                const double sy = static_cast<double>(ryd_f) * scale;               // 004BE1DB
                const double sz = static_cast<double>(dz) * scale;                  // 004BE1DF..E6
                cand.x = static_cast<float>(static_cast<double>(sx) + from->x);     // 004BE1EA..FD
                cand.y = static_cast<float>(sy + from->y);                          // 004BE1FB..20
                cand.z = static_cast<float>(sz + from->z);                          // 004BE21D..33
                float radius2 = at<float>(pf, 4);                                   // [esp+84]
                if (w5_area_test_direct_line_004bcb70(area, nullptr, from, &cand, &radius2, at<float>(pf, 0x14), 1,
                                                      nullptr) == 1)                // 004BE1CD..4D
                    found = 1;                                                      // 004BE24F
                scale = static_cast<float>(static_cast<double>(scale) * k_half_0073e9ac); // 004BE251..5B
                if (!(static_cast<double>(scale) >= k_min_scale_0073dff4)) break;   // 004BE25F..6A (test ah,1; je)
            }
            if (found != 1) continue;                                               // 004BE270..73
            const double ex = static_cast<double>(target->x) - cand.x, ey = static_cast<double>(target->y) - cand.y;
            const double ez = static_cast<double>(target->z) - cand.z;              // 004BE279..8A
            const double back = (ez * ez + ey * ey) + ex * ex;                      // 004BE28E..9C
            if (!(back >= static_cast<double>(base_sq))) continue;                  // 004BE29E..AC (test bl,ah: C0)
            score = true;
        }
        Vector diff;                                                                // [esp+98]
        diff.x = static_cast<float>(static_cast<double>(cand.x) - target->x);       // 004BE2B6..C5
        diff.y = static_cast<float>(static_cast<double>(cand.y) - target->y);       // 004BE2CC..D3
        diff.z = static_cast<float>(static_cast<double>(cand.z) - target->z);       // 004BE2DA..E1
        double m = kotor_cdecl_004ab100(&diff);                                     // 004BE2E8
        if (i == 4) m = m * k_w256_007455d4;                                        // 004BE2F0..F5
        else if (i == 3 || i == 5) m = m * k_w64_007455cc;                          // 004BE2FD..05 / 004BE32B
        else if (i == 2 || i == 6) m = m * k_w16_007455d0;                          // 004BE307..0F / 004BE323
        else if (i == 1 || i == 7) m = m * k_w4_0073f404;                           // 004BE311..1B
        if (!(m >= static_cast<double>(best))) continue;                          // 004BE331..3A (test ah,1)
        best = static_cast<float>(m);                                               // 004BE340
        result = cand;                                                              // 004BE33C..54
    }
    *out = result;                                                                  // 004BE370..8D
    return out;
}

// 004F1EF0 CSWSCreature::CancelAction(unused, group): removes every queued node of the group that vtbl+74 accepts
U __fastcall kotor_fastcall_004f1ef0(void* creature, void*, U unused, U group) {
    (void)unused;   // read only on the unreachable null-list branch (004F1F21)
    void** list = reinterpret_cast<void**>(bytes(creature) + 0xfc);                 // ESI
    void* head = *static_cast<void**>(*list);
    void* position = head;                                                          // [esp+C]
    void* action = head != nullptr ? at<void*>(head, 8) : nullptr;                  // EBX
    U result = 0;                                                                   // [esp+14]
    while (action != nullptr) {                                                     // 004F1F27 / 004F1FB5
        if (at<U16>(action, 0x6c) == static_cast<U16>(group)) {                     // 004F1F34..44
            void* current = position;                                               // EDI
            action = reinterpret_cast<void*>(static_cast<std::uintptr_t>(
                unresolved_list_next_005e9b90(*list, nullptr, &position)));         // 004F1F46..52
            void* node = unresolved_list_at_pos_005e9b60(*list, nullptr, current);   // 004F1F4F..54
            void* first_head = *static_cast<void**>(*list);
            void* first = first_head != nullptr ? at<void*>(first_head, 8) : nullptr; // 004F1F59..6A
            if (vcall<U>(creature, 0x74, node, node == first ? 1u : 0u) == 1) {     // 004F1F6E..84
                // 004F1F86..89 Remove(position) returns the node's data (0 for a null position); the selected
                // provider returns no EAX, so the data word is read first (same node, before the unlink)
                void* removed = current != nullptr ? at<void*>(current, 8) : nullptr;
                c2da_005e9c10(*list, nullptr, current);
                if (removed != nullptr) {                                           // 004F1F8E..92
                    recovered_action_node_destroy_004cc4d0(removed, nullptr);       // 004F1F94..96
                    live_free(removed);                                             // 004F1F9B..A1
                }
                result = 1;                                                         // 004F1FA4
            }
        } else {
            action = reinterpret_cast<void*>(static_cast<std::uintptr_t>(
                unresolved_list_next_005e9b90(*list, nullptr, &position)));         // 004F1FAE..B3
        }
    }
    return result;
}

// 0056C610 CSWSMessage::SendServerToPlayerCamera_ChangeLocation(player, flags, x, y, z, byte)
U __fastcall kotor_fastcall_0056c610(void* message, void*, void* player, U flags, U x, U y, U z, U byte_value) {
    kotor_fastcall_004d68a0(message, nullptr, 0x100, 0xffffffffu, 1);              // 0056C615..20
    kotor_fastcall_004d62c0(message, nullptr, flags, 8);                            // 0056C625..2E
    if ((flags & 1) != 0) kotor_fastcall_004d6920(message, nullptr, x, 0x3f800000u, 0x20); // 0056C633..46
    if ((flags & 2) != 0) kotor_fastcall_004d6920(message, nullptr, y, 0x3f800000u, 0x20); // 0056C64B..5E
    if ((flags & 4) != 0) kotor_fastcall_004d6920(message, nullptr, z, 0x3f800000u, 0x20); // 0056C663..76
    if ((flags & 8) != 0) kotor_fastcall_004d62c0(message, nullptr, byte_value, 8); // 0056C67B..89
    void* data;                                                                     // [esp+C]
    U size;                                                                         // [esp+8]
    const U ok = kotor_fastcall_004d6440(message, nullptr, &data, &size);            // 0056C68E..9A
    if (ok == 0) return ok;                                                         // 0056C69F..A8
    return kotor_fastcall_00567e60(message, nullptr, at<U>(player, 4), 0x10, 1, data, size); // 0056C6AB..C3
}

}  // extern "C"
