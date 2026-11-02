// t13: the four CSWCProjectile motion functions ("motion4").
//   006D33C0 launch / motion setup    object_projectile_006d33c0  void  fastcall @12  RET 4   (no FuncInfo)
//   006D7590 per-frame motion step    object_projectile_006d7590  void  fastcall @8   RET 0   (no FuncInfo)
//   006D2E00 homing velocity update   object_projectile_006d2e00  void  fastcall @8   RET 0   (no FuncInfo)
//   006D30C0 curving velocity update  object_projectile_006d30c0  void  fastcall @8   RET 0   (no FuncInfo)
// Ownership: reassigned by user from the sol projectile lanes, 2026-10-15, PENDING-REGISTRATION.
// Written from my own static disassembly of swkotor.exe. The paused sol-projectile-motion t2 source (four naked
// whole-body asm transcriptions) was read and checked line by line; none of its asm is used (see INTEGRATION.md).
// Declarations of the four exports: shared/object_projectile_internal.h (unchanged pinned copy). Every callee is a
// selected provider, another projectile export under its exact name (t9 006D4250, t12 006D70D0, t11b 006FC0E0), or one
// of the four functions here.
//
// Projectile fields used (as read and written by the four functions):
//   +024 Vector position (SetPosition slot +08Ch writes it)   +030 Vector orientation   +044 elapsed ms
//   +100 Vector previous position   +114 Vector destination   +120 remaining ms   +124 target id   +128 CExoString node
//   +158 BYTE attach mode   +164 done flag   +178 Vector velocity   +184 Vector acceleration   +198 current path event
//   +19C Gob   +1A0 BYTE curve side   +1A1 BYTE orientation mode
// Path event (34h bytes, see t12): +00 point  +0C second point  +18 target id  +24 lead time  +28 float radius
//   +2D BYTE type (1..13)
//
// x87: see x87_arith.h (t12, unchanged). Register values are doubles, DWORD memory operands M32, with the x87 NaN
// selection. The minimal x87 helpers below exist only where C++ has no operation: fcos, fsin, fldln2/fyl2x, and the
// 80-bit ST0 transport around the CRT acos 006FC0E0 (t11b export).
#include "8b103f2541-object_projectile_internal.h"
#include "74c2dbe6b5-object_projectile_providers.h"
#include "ed1c25ba9a-object_projectile_literals.h"
#include "fd26c081f1-crt_acos_006fc0e0.h"
#include "62caa84031-m15e.h"
#include "f58c269829-scene3d.h"
#include "9963eab06a-vtables_gen.h"
#include "b6906bba0c-startup_string.h"
#include "7f8fd7121f-x87_arith.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <type_traits>
// L3 MATH-TRIG-CPP t1 (2026-10-24): LD unit (recipe + -mno-sse -ffp-model=strict -mlong-double-80); math declarations.
#include "../../math/kotor_math.h"

using U = std::uint32_t;
using B = std::uint8_t;
using S = kotor_reconstruction::StartupString;
using kotor_scene3d::Quaternion;
using kotor_scene3d::Vector;
using x87::M32;

namespace menu_tables_m41 { namespace cells { extern const unsigned int invalid_object_00746644; } }
namespace base_ini_reconstruction { extern const char warning_empty_0073d71c[]; }
extern "C" {
// Selected providers (scene3d.h declares 004AB100 / 004AB180 / 00423B90).
float* __cdecl kotor_cdecl_00423bc0(float* out, const float* a, const float* b);               // out = a - b
float* __cdecl vector_component_mult_00423bf0(float* out, const float* v, float scale);        // out = scale * v
float* __cdecl float_times_vector_00437a60(float* out, float scale, const float* v);           // out = scale * v
float* __fastcall vector_add_assign_004a9740(float* self, void*, const float* other);          // self += other, RET 4
aurora_model::Gob* __cdecl aurora_new_aur_object_00449cc0(const char* name, const char* gob_name, std::FILE* mdl, std::FILE* mdx);
}

// The exports are the shared-header declarations (t9, t11b and t12 import them by these names).
static_assert(std::is_same_v<decltype(&object_projectile_006d33c0), void (__fastcall*)(void*, void*, Vector*)>, "33C0");
static_assert(std::is_same_v<decltype(&object_projectile_006d7590), void (__fastcall*)(void*, void*)>, "7590");
static_assert(std::is_same_v<decltype(&object_projectile_006d2e00), void (__fastcall*)(void*, void*)>, "2E00");
static_assert(std::is_same_v<decltype(&object_projectile_006d30c0), void (__fastcall*)(void*, void*)>, "30C0");

namespace {
// Original immediates (instruction bytes, not image data).
constexpr U kInvalidId = 0x7f000000u;    // cmp dword [esi+124h], 7F000000h (006D3422)
constexpr U kGravity = 0xc11cf5c3u;      // -9.81f, mov [esp+18h], 0C11CF5C3h (006D378F)
constexpr U kLobSpeed = 0x40f00000u;     // 7.5f, push 40F00000h (006D36A9)
constexpr U kTwo = 0x40000000u;          // 2.0f, push 40000000h (006D390A / 006D3AD6 / 006D3B66 / 006D3F85)
constexpr U kOne = 0x3f800000u;          // 1.0f, the identity quaternion w (006D34A3 / 006D3CE2 / 006D7618 / 006D7897)
constexpr U kPart = 0xffu, kForce = 1u;  // GetModel(FFh, 1)

template<class T> T rd(const void* p, U n) { T v; std::memcpy(&v, static_cast<const B*>(p) + n, sizeof v); return v; }
template<class T> void wr(void* p, U n, T v) { std::memcpy(static_cast<B*>(p) + n, &v, sizeof v); }
void* at(void* p, U n) { return static_cast<B*>(p) + n; }
void* ptr(const void* p, U n) { return rd<void*>(p, n); }
void* client() { return ptr(kotor_menu_window_cells::manager_007a39fc, 4); }   // [[007A39FC]+4]
U word_of(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
void* pointer_of(U word) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word)); }
M32 f(const void* p, U n) { return x87::m32_at(p, n); }                         // DWORD float field (raw bits)
const KotorM15ObjectMethods_007516c8* object_table(const void* o) { return rd<const KotorM15ObjectMethods_007516c8*>(o, 0); }
const aurora_model::GobVTable* gob_table(const void* g) { return rd<const aurora_model::GobVTable*>(g, 0); }
aurora_model::Gob* as_gob(void* g) { return static_cast<aurora_model::Gob*>(g); }
S* string_at(void* p, U n) { return static_cast<S*>(at(p, n)); }

// Three DWORDs as the original moves them (integer copies; never through the FPU).
struct Point { M32 x, y, z; };
static_assert(sizeof(Point) == 12, "Vector-sized");
constexpr Point kZero = {M32{0}, M32{0}, M32{0}};
Point point_at(const void* p, U n) { return {f(p, n), f(p, n + 4), f(p, n + 8)}; }
void put_point(void* p, U n, Point v) { x87::put_at(p, n, v.x); x87::put_at(p, n + 4, v.y); x87::put_at(p, n + 8, v.z); }
Vector vector_of(Point v) { Vector r; std::memcpy(&r, &v, sizeof r); return r; }
Quaternion identity() { Quaternion q; const U w[4] = {kOne, 0, 0, 0}; std::memcpy(&q, w, sizeof q); return q; }
float float_of(U bits) { float v; std::memcpy(&v, &bits, sizeof v); return v; }

// L3 t1 a2: register values are long double (PLAN-v3 writing rule); a float field or a raw DWORD operand is read
// with a force-inlined accessor (the recipe is /Ob0), so it is a direct `fld dword` / memory operand; every original
// fstp dword is one (float). (The t12 x87_arith.h double emulation of the x87 NaN selection is not needed: the
// operations run on the x87 itself.)
__forceinline float l2f(const void* p, U n) { float v; std::memcpy(&v, static_cast<const B*>(p) + n, sizeof v); return v; }
__forceinline long double ldm(M32 m) { float v; std::memcpy(&v, &m.bits, sizeof v); return v; }   // fld dword of a raw DWORD
__forceinline M32 fstm(long double v) { const float r = static_cast<float>(v); M32 m; std::memcpy(&m.bits, &r, sizeof m.bits); return m; }   // fstp dword
__forceinline void put_f(void* p, U n, long double v) { const float r = static_cast<float>(v); std::memcpy(static_cast<B*>(p) + n, &r, sizeof r); }   // fstp dword [p+n]
__forceinline float lit_f(const unsigned char* bytes) { float v; std::memcpy(&v, bytes, sizeof v); return v; }   // dword image literal
// fild dword [field] ; test ; jge ; fadd dword [0073D770] (2^32) ; fmul dword [0073D708] (0.001f): the field as an
// unsigned count of milliseconds (exact in the register), times 0.001f at PC 53.
__forceinline long double seconds_of(const void* p, U n) {
    U v; std::memcpy(&v, static_cast<const B*>(p) + n, sizeof v);
    return static_cast<long double>(v) * lit_f(object_projectile_literal_0073d708);
}

void* get_model(void* object) { return object_table(object)->s098_get_model(object, nullptr, kPart, kForce); }   // slot +098 (FFh, 1)
// Gob::GetPartPosition (Gob slot +098)([self+128h] node name, &position, &orientation)
U part_position(void* gob, void* self, Point* position, Quaternion* orientation) {
    const char* const name = WindowCStr005e5670(string_at(self, 0x128), nullptr);
    return gob_table(gob)->s098_gob_get_part_position(as_gob(gob), nullptr, word_of(name), word_of(position), word_of(orientation));
}
// L3 t1 a2: Gob::SetPosition (Gob slot +05C)(Vector* out, Vector v by value) called with v as the three raw words
// the original pushes with integer moves (006D3DCA..006D3DF1, 006D4033..006D404A, 006D4065..006D407D): a float-typed
// Vector argument is copied with fld/fstp dword by clang, which quiets a signalling NaN.
using GobSetPositionWords = Vector* (__fastcall*)(aurora_model::Gob*, void*, Vector*, U, U, U);
Vector* set_position_words(aurora_model::Gob* gob, Vector* out, Point v) {
    const GobSetPositionWords slot = reinterpret_cast<GobSetPositionWords>(reinterpret_cast<std::uintptr_t>(gob_table(gob)->s05c_gob_set_position));
    return slot(gob, nullptr, out, v.x.bits, v.y.bits, v.z.bits);
}
// Gob::ReattachReference (Gob slot +0A4)(other, name)
void reattach(void* gob, void* other, const char* name) {
    (void)gob_table(gob)->s0a4_gob_reattach_reference(as_gob(gob), nullptr, word_of(other), word_of(name));
}

// ======================================================================================== 006D7590 helpers
// 006D764A..006D7744 / 006D78C9..006D79DB: the target object's node [self+128h], else its position raised by 1.25
// (attach modes 2 and 3 also move it by -0.5 x the projectile's orientation). The second GetModel result is used
// unchecked by the original.
void target_point(void* self, void* object, Point& p, Quaternion& q) {
    if (get_model(object) == nullptr) {                                                  // 006D7659..006D7661
        p = point_at(self, 0x114);                                                       // 006D7746..006D775C
        return;
    }
    void* const gob = get_model(object);                                                 // 006D7672 / 006D78F1
    if (part_position(gob, self, &p, &q) != 0) return;                                   // 006D7694..006D769C
    const B mode = rd<B>(self, 0x158);                                                   // 006D76A2
    p = point_at(object, 0x24);                                                          // 006D76B0..006D76C3
    p.z = fstm(ldm(p.z) + lit_f(object_projectile_literal_00757ca8));                    // 006D76C7..006D76D1 fadd [00757CA8] (1.25f) ; fstp dword
    if (mode == 3 || mode == 2) {                                                         // 006D76A8..006D76AE
        // 006D7708..006D773B: fld or.x ; fchs ; fld or.y ; fchs ; fld or.z ; fchs ; fstp dword [esp+3Ch] ;
        // fxch ; fmul 0.5 ; fstp dword ; fmul 0.5 ; fstp dword ; fld [esp+3Ch] ; fmul 0.5 ; fstp dword
        const long double negative_z = static_cast<float>(-static_cast<long double>(l2f(self, 0x38)));   // 006D7717
        const float half = lit_f(object_projectile_literal_0073e9ac);
        float offset[3];                                                                 // [esp+24h]
        offset[0] = static_cast<float>(-static_cast<long double>(l2f(self, 0x30)) * half);   // 006D7723
        offset[1] = static_cast<float>(-static_cast<long double>(l2f(self, 0x34)) * half);   // 006D772D
        offset[2] = static_cast<float>(negative_z * half);                                   // 006D773B
        (void)vector_add_assign_004a9740(reinterpret_cast<float*>(&p), nullptr, offset);  // 006D773F
    }
}
// 006D7760..006D77C0 / 006D79DF..006D7A35: event +0Ch = p; destination = p + event point
void place_event(void* self, Point p) {
    put_point(ptr(self, 0x198), 0xc, p);                                                 // 006D7760..006D777A
    void* const event = ptr(self, 0x198);                                                // 006D7781
    const M32 x = fstm(ldm(p.x) + l2f(event, 0));                                       // 006D777D..006D7793
    const M32 y = fstm(ldm(p.y) + l2f(event, 4));                                       // 006D779B..006D77A2
    const M32 z = fstm(ldm(p.z) + l2f(event, 8));                                       // 006D77A6..006D77B8
    put_point(self, 0x114, {x, y, z});
}
// 006D7A93..006D7AB5: take the next path event, keeping the remaining time (006D70D0's result is not tested)
void next_event_keep_time(void* self) {
    const U remaining = rd<U>(self, 0x120);
    (void)object_projectile_006d70d0(self, nullptr);
    wr<U>(self, 0x120, rd<U>(self, 0x120) + remaining);
}

// ======================================================================================== 006D33C0 helpers
// ---- L3 t1 (2026-10-24): 006D33C0 (with its inlined launch / lob / acceleration code below) is plain C++ in the
// PLAN-v3 writing rule; this unit is an LD unit. It replaces the inline-asm helpers x87_cos_to_m32, x87_sin_to_m32,
// x87_ln_times, x87_load_m32 and x87_store_m32 (acos, cos, sin and log are the kotor_math.h long double overloads).
// Field and literal reads are __forceinline (the recipe is /Ob0), so each is a direct memory operand.
__forceinline float l3f(const void* p, U n) { float v; std::memcpy(&v, static_cast<const B*>(p) + n, sizeof v); return v; }
__forceinline float l3lit(const unsigned char* bytes) { float v; std::memcpy(&v, bytes, sizeof v); return v; }
__forceinline double l3lit64(const unsigned char* bytes) { double v; std::memcpy(&v, bytes, sizeof v); return v; }
__forceinline float l3mf(M32 m) { float v; std::memcpy(&v, &m.bits, sizeof v); return v; }
__forceinline M32 l3m(float f) { M32 m; std::memcpy(&m.bits, &f, sizeof m.bits); return m; }
// The original's `fsub dword ptr [field]` with a NaN register operand and a signalling-NaN DWORD memory operand
// returns the register operand (Intel SDM vol. 1, 4.8.3.5, table 4-7). Under -ffp-model=strict clang emits
// fld ; fsubp (the load quiets the SNaN, then the larger significand wins); this keeps the original's result for that
// one case, every other operand pair is the plain long double operation.
__forceinline bool l3snan_at(const void* p, U n) {
    U b; std::memcpy(&b, static_cast<const B*>(p) + n, sizeof b);
    return (b & 0x7f800000u) == 0x7f800000u && (b & 0x007fffffu) != 0 && (b & 0x00400000u) == 0;
}
__forceinline long double l3sub_m(long double reg, const void* p, U n) { return (reg != reg && l3snan_at(p, n)) ? reg : reg - l3f(p, n); }
// fild dword [field] ; (fadd dword [0073D770] 2^32 when negative) ; fmul dword [0073D708] (0.001f): ms as seconds.
__forceinline long double l3seconds(const void* p, U n) { return static_cast<long double>(rd<U>(p, n)) * l3lit(object_projectile_literal_0073d708); }

// 006D3B8F tail: acceleration = (1 / t^2) * (2 * (toward - t * velocity)); every vector op is the selected provider.
// 006D3915..006D392F: fld t ; fmul t ; fdivr dword [0073D764] (1.0f) ; fstp dword [esp] (the factor argument).
// L3 t1 a2: `seconds` is the float word the original stores at 006D367E ([esp+20h]) and pushes as a raw word
// (006D38DA..006D38E1); it is passed as its bits (M32), so no second (float) conversion is made (the checkpoint held
// it as a long double and re-rounded it: a round trip that is no original store) and it is never held on the x87
// stack across the calls (a float reference parameter is promoted to a float value by clang: 4-byte spill, S1).
void l3_accelerate(void* self, const Point& toward, M32 seconds) {
    float scaled[3];
    const float* const travel = float_times_vector_00437a60(scaled, l3mf(seconds), static_cast<const float*>(at(self, 0x178)));
    float difference[3];
    const Vector to = vector_of(toward);
    const float* const rest = kotor_cdecl_00423bc0(difference, &to.x, travel);
    float doubled[3];
    const float* const twice = float_times_vector_00437a60(doubled, float_of(kTwo), rest);
    const float inverse = static_cast<float>(l3lit(object_projectile_literal_0073d764) / (static_cast<long double>(l3mf(seconds)) * l3mf(seconds)));
    float accel[3];
    const float* const a = float_times_vector_00437a60(accel, inverse, twice);
    put_point(self, 0x184, point_at(a, 0));                                              // 006D3B94..006D3BAA
}

enum class Spread { hundredths, coin, hundredths_gob };

// 006D379C..006D3933 (mode 4), 006D3938..006D3AFF (mode 9), 006D3E17..006D3FAE (mode 12): a random launch.
void l3_random_launch(void* self, Point& dir, const Point& toward, M32 seconds, Spread spread) {
    Vector unit;                                                                         // [esp+1CCh]
    const Vector original = vector_of(dir);
    dir = point_at(normalize_004ab180(&unit, &original), 0);                             // [esp+28h..30h], DWORD copies
    // 006D37B4..006D37CE: fld dword dir.x ; call 006FC0E0 _CIacos (80-bit in / out) ; fstp dword [esp+24h].
    long double heading = static_cast<float>(acos(static_cast<long double>(l3mf(dir.x))));
    // 006D37D2..006D37ED: fld dir.y ; fcomp 0 ; test 5 ; jp (not less) ; else fld [00740C88] (2pi) ; fsub heading ; fstp.
    if (l3mf(dir.y) < l3lit(object_projectile_literal_0073d700))
        heading = static_cast<float>(l3lit(object_projectile_literal_00740c88) - heading);
    // 006D37F1..006D3825: turn = rand() % 900 ; side = [00833BD8] ; fild turn ; fmul [0073E798] (0.1f) ;
    // fmul [007415F8] (deg->rad) ; side ? fsubr heading (heading - st) : fadd heading.
    const std::int32_t turn = crt_rand_006fc447() % 0x384;
    const U side = object_projectile::toggle_00833bd8;
    const long double degrees = (static_cast<long double>(turn) * l3lit(object_projectile_literal_0073e798)) * l3lit(object_projectile_literal_007415f8);
    const long double angle = side != 0 ? heading - degrees : degrees + heading;
    // 006D3825..006D382F: fld st(0) ; fcos ; fstp dword [esp+34h] ; fsin ; fstp dword [esp+38h].
    Vector aim;                                                                          // [esp+34h..3Ch] (in memory)
    aim.x = static_cast<float>(cos(angle));
    aim.y = static_cast<float>(sin(angle));
    long double lift;
    if (spread == Spread::coin) {                                                        // 006D39CF..006D3A14
        if (crt_rand_006fc447() % 2 != 0)                                                // and eax,80000001h; jns; dec; or; inc
            lift = static_cast<long double>(crt_rand_006fc447() % 0x64) * l3lit(object_projectile_literal_0073dfec);
        else
            lift = static_cast<long double>(crt_rand_006fc447() % 0x32) * l3lit(object_projectile_literal_00757ca4);
    } else {
        lift = static_cast<long double>(crt_rand_006fc447() % 0x64) * l3lit(object_projectile_literal_0073dfec);   // 006D3833..006D385A
    }
    const U flip = object_projectile::toggle_00833bd8;                                   // 006D3840 (re-read)
    aim.z = static_cast<float>(lift);                                                    // fstp dword [esp+3Ch]
    object_projectile::toggle_00833bd8 = flip == 0 ? 1u : 0u;                            // xor edx,edx; test edi,edi; sete dl
    // 006D386A..006D387B: |toward| (ST0) ; fldln2 ; fxch ; fyl2x ; fmul qword [00753D98] (5.0) ; call __ftol2.
    const Vector to = vector_of(toward);
    const long double distance = vector_magnitude_004ab100(&to);
    const U scaled = static_cast<U>(static_cast<long long>(log(distance) * l3lit64(object_projectile_literal_00753d98)));
    const U sign = static_cast<U>(static_cast<std::int32_t>(scaled) >> 31);             // cdq
    const U range = ((scaled ^ sign) - sign) + 1u;                                       // xor; sub; inc
    const std::int32_t pick = crt_rand_006fc447() % static_cast<std::int32_t>(range);   // 006D3888..006D388E idiv
    Vector direction;                                                                    // [esp+A0h]
    const Vector* const n = normalize_004ab180(&direction, &aim);
    // 006D3898..006D38A9: fild pick ; fadd [00740C84] (3.0f) ; fstp dword [esp] (the speed argument; the original
    // stores it before the normalize call, which reads no float state: the order is not observable).
    float velocity[3];                                                                   // [esp+1A8h]
    const float* const v = vector_component_mult_00423bf0(velocity, &n->x,
        static_cast<float>(static_cast<long double>(pick) + l3lit(object_projectile_literal_00740c84)));
    put_point(self, 0x178, point_at(v, 0));                                              // 006D38C4..006D38DE
    l3_accelerate(self, toward, seconds);
}

// 006D36A9..006D378C (mode 10): a lob. acceleration = 7.5 * unit(dir.x, dir.y, 0); velocity = (2 / t) * toward -
// acceleration; then acceleration x and y become (a^2 - v^2) / (2 toward) per axis.
void l3_lob(void* self, Point dir, const Point& toward, M32 seconds) {
    dir.z = M32{0};                                                                      // 006D36BB
    Vector unit;                                                                         // [esp+7Ch]
    const Vector flat = vector_of(dir);
    const Vector* const n = normalize_004ab180(&unit, &flat);
    float pushed[3];                                                                     // [esp+4Ch]
    const float* const a = vector_component_mult_00423bf0(pushed, &n->x, float_of(kLobSpeed));
    // 006D36D6..006D3709: fld [00741BE0] (2.0f) ; fdiv t ; fstp dword [esp] (the factor argument).
    const long double rate = static_cast<long double>(l3lit(object_projectile_literal_00741be0)) / l3mf(seconds);
    put_point(self, 0x184, point_at(a, 0));                                              // 006D36E0..006D36FF
    float scaled[3];                                                                     // [esp+100h]
    const Vector to = vector_of(toward);
    const float* const s = float_times_vector_00437a60(scaled, static_cast<float>(rate), &to.x);
    float velocity[3];                                                                   // [esp+1B4h]
    const float* const v = kotor_cdecl_00423bc0(velocity, s, static_cast<const float*>(at(self, 0x184)));
    put_point(self, 0x178, point_at(v, 0));                                              // 006D3725..006D373A
    for (U axis = 0; axis < 8; axis += 4) {                                              // 006D3723..006D3754, 006D375A..006D3778
        // fld a ; fld v ; fld st(1) ; fmul st(2) ; fld st(1) ; fmul st(2) ; fsubp ; fld to ; fadd st,st ; fdivp ; fstp dword
        const long double ac = l3f(self, 0x184 + axis), ve = l3f(self, 0x178 + axis);
        const long double top = ac * ac - ve * ve;
        const long double t = l3f(&to, axis);
        wr<float>(self, 0x184 + axis, static_cast<float>(top / (t + t)));
    }
}

// 006D3BF9..006D3DA3 (mode 7): attach the projectile's Gob to the target's node, or to a "dummytarget" reference
// Gob placed at the path event when the target is not an object.
void attach_to_target(void* self) {
    void* const model = get_model(self);                                                 // 006D3C5C..006D3C6E
    if (model == nullptr) return;
    void* const array = kotor_fastcall_005ed570(client(), nullptr);                      // 006D3C78..006D3C80
    if (array == nullptr) return;
    void* object = nullptr;                                                              // [esp+58h]
    if (kotor_fastcall_004d8230(array, nullptr, rd<U>(self, 0x124), &object) == object_projectile_literal_0074666c[0]) {
        Point part = kZero;                                                              // [esp+10h]
        Quaternion q = identity();                                                       // [esp+6Ch]
        if (get_model(object) == nullptr) return;                                        // 006D3CEC..006D3CFB
        void* const gob = get_model(object);                                             // 006D3D0C
        if (part_position(gob, self, &part, &q) != 0) {                                  // 006D3D31..006D3D47
            const char* const name = WindowCStr005e5670(string_at(self, 0x128), nullptr);   // 006D3D4D
            void* const other = get_model(object);                                       // 006D3D5E
            reattach(model, other, name);                                                // 006D3D67
        } else {
            reattach(model, get_model(object), object_projectile_literal_00740c00);       // 006D3D7A..006D3D93 "root"
        }
        return;
    }
    aurora_model::Gob* const reference =
        aurora_new_aur_object_00449cc0(object_projectile_literal_00757c94, object_projectile_literal_00757c98, nullptr, nullptr);   // "ref", "dummytarget"
    wr(self, 0x19c, reference);                                                          // 006D3DBE
    if (reference == nullptr) return;
    Vector placed;                                                                       // [esp+C4h]
    (void)set_position_words(reference, &placed, point_at(ptr(self, 0x198), 0));          // 006D3DCA..006D3DF1
    reattach(model, ptr(self, 0x19c), object_projectile_literal_00740c00);               // 006D3DF4..006D3E04 "root"
}

// 006D3FB3..006D40FF (mode 12, after the launch): a reference Gob at the own model's position, the model raised by
// 0.01, the reference oriented as the model, attached to the model's scene and reattached as "root".
void own_reference(void* self) {
    aurora_model::Gob* const reference =
        aurora_new_aur_object_00449cc0(object_projectile_literal_00757c94, base_ini_reconstruction::warning_empty_0073d71c, nullptr, nullptr);   // "ref", ""
    wr(self, 0x19c, reference);                                                          // 006D3FE1
    if (reference == nullptr) return;
    void* const model = get_model(self);                                                 // 006D3FED..006D3FFE
    Vector position;                                                                     // [esp+184h]
    const Point here = point_at(pointer_of(gob_table(model)->s064_gob_get_position(as_gob(model), nullptr, word_of(&position))), 0);   // [esp+10h..18h]
    Vector placed;                                                                       // [esp+19Ch]
    (void)set_position_words(as_gob(ptr(self, 0x19c)), &placed, here);                   // 006D4033..006D404A
    Point raised = here;
    raised.z = l3m(static_cast<float>(static_cast<long double>(l3mf(here.z)) + l3lit(object_projectile_literal_0073dfec)));   // fld [esp+18h]; fadd [0073DFEC]; fstp (L3 t1)
    Vector moved;                                                                        // [esp+5Ch]
    (void)set_position_words(as_gob(model), &moved, raised);                             // 006D4065..006D407D
    Quaternion orientation;                                                              // [esp+1F0h]
    U q[4];
    std::memcpy(q, pointer_of(gob_table(model)->s068_gob_get_orientation(as_gob(model), nullptr, word_of(&orientation))), sizeof q);   // 006D408C..006D40AF
    Quaternion turned;                                                                   // [esp+200h]
    (void)gob_table(ptr(self, 0x19c))->s060_gob_set_orientation(as_gob(ptr(self, 0x19c)), nullptr, word_of(&turned), q[0], q[1], q[2], q[3]);   // 006D40C0
    const aurora_model::GobVTable* const table = gob_table(ptr(self, 0x19c));          // 006D40C3..006D40CB (table read first)
    const U scene = gob_table(model)->s104_gob_get_property_0x68(as_gob(model), nullptr);   // 006D40CF
    (void)table->attach_to_scene(as_gob(ptr(self, 0x19c)), nullptr, static_cast<aurora_model::Scene*>(pointer_of(scene)));   // 006D40D5..006D40DC
    reattach(model, ptr(self, 0x19c), object_projectile_literal_00740c00);               // 006D40DF..006D40EF "root"
}

void stop(void* self) {   // velocity and acceleration zero (the original stores x, y, z of +178h, then of +184h)
    put_point(self, 0x178, kZero);
    put_point(self, 0x184, kZero);
}

// ======================================================================================== 006D2E00 / 006D30C0 head
// L3 t1 a2: the difference destination - position, stored as three floats (006D2E06..006D2E41 /
// 006D30C6..006D3119: fld [esi+114h+4i] ; fsub [esi+24h+4i] ; fstp dword). Written to memory by a separate function:
// the floats are reloaded after calls (006D30C0's normalize) and must not stay on the x87 stack across them (S1).
__declspec(noinline) void difference_of(void* self, Point* d) {
    d->x = fstm(static_cast<long double>(l2f(self, 0x114)) - l2f(self, 0x24));
    d->y = fstm(static_cast<long double>(l2f(self, 0x118)) - l2f(self, 0x28));
    d->z = fstm(static_cast<long double>(l2f(self, 0x11c)) - l2f(self, 0x2c));
}
// 006D3121..006D319F (006D30C0): T = [+120h] / 1000 ; fdivr 1.0 (kept) ; u = d * (1 / T), three fstp dword. The
// three floats are reloaded after the normalize call (fadd dword [esp+38h..40h]): written to memory here.
__declspec(noinline) void spiral_rate(void* self, const Point& d, float* u) {
    const long double inverse = lit_f(object_projectile_literal_0073d764) / seconds_of(self, 0x120);   // 006D3121..006D3171
    u[0] = static_cast<float>(ldm(d.x) * inverse);                                      // 006D3187
    u[1] = static_cast<float>(ldm(d.y) * inverse);                                      // 006D3191
    u[2] = static_cast<float>(ldm(d.z) * inverse);                                      // 006D319B
}

// ======================================================================================== 006D2E00 / 006D30C0 tail
// 006D2F5C..006D30BE / 006D325A..006D33BC: the new position p = position + t * velocity is clamped to the destination
// when it would pass it; then the orientation (mode [+1A1h]: 1 toward the event's second point, 2 along the step,
// otherwise the initial difference) and the position are set. `order` is the function's own order of the squared
// sums (006D2E00: (z^2 + y^2) + x^2; 006D30C0: (z^2 + x^2) + y^2).
struct Squares { int a, b, c; };   // component indices of ((a^2 + b^2) + c^2)
// fld dword v[a] ; fmul dword v[a] ; fld v[b] ; fmul v[b] ; faddp ; fld v[c] ; fmul v[c] ; faddp
__forceinline long double sum_squares(const Point& v, Squares o) {
    const M32 c[3] = {v.x, v.y, v.z};
    const long double a = ldm(c[o.a]), b = ldm(c[o.b]), cc = ldm(c[o.c]);
    return (a * a + b * b) + cc * cc;
}
// 006D2F00..006D2FA3 / 006D31FE..006D32A1: the step point p = position + T * velocity (T = [+44h] / 1000), stored as
// three floats, and the squared length of position - p (registers). L3 t1 a2: p is written to the caller's memory
// here, so the destination copy that may replace it stays an integer copy (the original moves the DWORDs,
// 006D2FB4..006D2FCE); held as floats in the caller, clang loaded the copy with fld dword (quieting a signalling NaN).
__declspec(noinline) long double step_point(void* self, Point* p) {
    const long double elapsed = seconds_of(self, 0x44);                                  // fild [esi+44h]; (+2^32); fmul [0073D708]
    const long double px = elapsed * l2f(self, 0x178);                                   // fld st(0); fmul [eax] (kept)
    const long double py = static_cast<float>(elapsed * l2f(self, 0x17c));               // 006D2F22 / 006D3220 fstp dword
    const long double pz = static_cast<float>(elapsed * l2f(self, 0x180));               // 006D2F2B / 006D3229 fstp dword
    p->x = fstm(px + l2f(self, 0x24));                                                   // 006D2F32 / 006D3230
    p->y = fstm(py + l2f(self, 0x28));                                                   // 006D2F45 / 006D3243
    p->z = fstm(pz + l2f(self, 0x2c));                                                   // 006D2F58 / 006D3256
    const long double a = static_cast<long double>(l2f(self, 0x24)) - ldm(p->x);        // fld pos.x ; fsub p.x
    const long double b = static_cast<long double>(l2f(self, 0x28)) - ldm(p->y);
    const long double c = static_cast<long double>(l2f(self, 0x2c)) - ldm(p->z);
    return (c * c + b * b) + a * a;                                                      // ((c^2 + b^2) + a^2), registers
}
// 006D2F00..006D30BE / 006D31FE..006D33BC (the two originals' shared tail)
void step_and_face(void* self, Point difference, Squares order) {
    Point p;                                                                             // [esp+20h] / [esp+14h]
    const long double moved = step_point(self, &p);
    const long double whole = sum_squares(difference, order);
    if (whole < moved) p = point_at(self, 0x114);                                        // fcompp; test ah,5; jp (copy only when less)
    Point dir = difference;                                                              // [esp+8]
    const B mode = rd<B>(self, 0x1a1);
    if (mode == 1) {                                                                     // event second point - p
        void* const event = ptr(self, 0x198);
        dir = {fstm(static_cast<long double>(l2f(event, 0xc)) - ldm(p.x)), fstm(static_cast<long double>(l2f(event, 0x10)) - ldm(p.y)),
               fstm(static_cast<long double>(l2f(event, 0x14)) - ldm(p.z))};             // 006D2FED/2FFC/3013, 006D32EB/32F6/3311
    } else if (mode == 2) {                                                              // p - position
        dir = {fstm(ldm(p.x) - l2f(self, 0x24)), fstm(ldm(p.y) - l2f(self, 0x28)),
               fstm(ldm(p.z) - l2f(self, 0x2c))};                                         // 006D302D/3040/3053, 006D332B/333E/3351
    }
    if (mode != 0 && sum_squares(dir, order) > lit_f(object_projectile_literal_00747504)) {   // fcomp; test ah,41h; jne
        Vector unit;                                                                     // [esp+3Ch] / [esp+44h]
        const Vector raw = vector_of(dir);
        const Vector* const n = normalize_004ab180(&unit, &raw);
        object_table(self)->s088_set_orientation(self, nullptr, &n->x);                  // slot +088
    }
    const Vector position = vector_of(p);                                                // [esp+20h] / [esp+14h]
    object_table(self)->s08c_set_position(self, nullptr, &position.x);                   // slot +08C
}
}  // namespace

extern "C" {

// 006D2E00..006D30BE: homing. velocity = (2 d - a T^2) / (2 T) for d = destination - position, T = [+120h] / 1000
// and a = [+184h] (x and y and z each in the original's own operand form), then step_and_face. RET 0.
void __fastcall object_projectile_006d2e00(void* self, void*) {
    Point d;                                                                             // [esp+14h..1Ch] (copied to [esp+8])
    difference_of(self, &d);                                                             // 006D2E06..006D2E41
    const long double t = seconds_of(self, 0x120);                                       // 006D2E49..006D2E5B
    const long double tt = t * t;                                                        // fld st(0); fmul st(1)
    const long double ax = tt * l2f(self, 0x184);                                        // fld st(0); fmul [esi+184h] (kept)
    const long double ay = static_cast<float>(tt * l2f(self, 0x188));                    // 006D2E7D fstp dword [esp+24h]
    const long double az = static_cast<float>(tt * l2f(self, 0x18c));                    // 006D2E89 fstp dword [esp+28h]
    const long double twice_x = ldm(d.x) + ldm(d.x);                                     // fld [esp+14h] ; fadd st(0),st(0) (kept)
    const long double twice_y = static_cast<float>(ldm(d.y) + ldm(d.y));                 // 006D2E99 fstp dword [esp+30h]
    const long double twice_z = static_cast<float>(ldm(d.z) + ldm(d.z));                 // 006D2EA3 fstp dword [esp+34h]
    const long double vx = twice_x - ax;                                                 // fsubrp st(1): ST1 = ST0 - ST1
    const long double vy = static_cast<float>(twice_y - ay);                             // 006D2EB1 fstp dword [esp+3Ch]
    const long double vz = static_cast<float>(twice_z - az);                             // 006D2EBD fstp dword [esp+40h]
    const long double inverse = static_cast<float>(lit_f(object_projectile_literal_0073d764) / (t + t));   // fadd st,st ; fdivr [0073D764] ; 006D2ECB fstp dword [esp+4]
    put_f(self, 0x178, vx * inverse);                                                    // 006D2ED3
    put_f(self, 0x17c, vy * inverse);                                                    // 006D2EE5
    put_f(self, 0x180, vz * inverse);                                                    // 006D2EF8
    step_and_face(self, d, Squares{2, 1, 0});                                            // S2 and the facing length: (z^2 + y^2) + x^2
}

// 006D30C0..006D33BC: curving. velocity = (destination - position) / T + 7.5 * unit(side), side = (-dy, dx, 0) when
// [+1A0h] is set, else (dy, -dx, 0); then step_and_face. RET 0.
void __fastcall object_projectile_006d30c0(void* self, void*) {
    Point d;                                                                             // [esp+20h..28h] (copied to [esp+8])
    difference_of(self, &d);                                                             // 006D30C6..006D3119
    Point side;                                                                          // [esp+2Ch..34h]
    if (rd<B>(self, 0x1a0) != 0) {                                                        // 006D3133..006D3149
        side.x = fstm(-ldm(d.y));                                                        // 006D314B..006D3159 fld dy ; fchs ; fstp dword
        side.y = d.x;                                                                    // mov
    } else {
        side.x = d.y;                                                                    // mov
        side.y = fstm(-ldm(d.x));                                                        // 006D315F..006D316D fld dx ; fchs ; fstp dword
    }
    side.z = M32{0};                                                                     // 006D3141
    float u[3];                                                                          // [esp+38h..40h]
    spiral_rate(self, d, u);                                                             // 006D3121..006D319F
    Vector unit;                                                                         // [esp+44h]
    const Vector raw = vector_of(side);
    const Vector* const n = normalize_004ab180(&unit, &raw);                             // 006D31A1
    // 006D31A6..006D3201: fld n.x ; fmul 7.5 ; fld n.y ; fmul 7.5 ; fld n.z ; fmul 7.5 ; fstp dword kz ;
    // fxch ; fadd ux ; fstp dword ; fadd uy ; fstp dword ; fld kz ; fadd uz ; fstp dword
    const float k = lit_f(object_projectile_literal_00749d80);                          // 7.5f
    const long double kx = static_cast<long double>(l2f(n, 0)) * k;
    const long double ky = static_cast<long double>(l2f(n, 4)) * k;
    const long double kz = static_cast<float>(static_cast<long double>(l2f(n, 8)) * k);  // 006D31CB
    put_f(self, 0x178, kx + u[0]);                                                       // 006D31D5
    put_f(self, 0x17c, ky + u[1]);                                                       // 006D31E3
    put_f(self, 0x180, kz + u[2]);                                                       // 006D31F6
    step_and_face(self, d, Squares{2, 0, 1});                                            // S2 and the facing length: (z^2 + x^2) + y^2
}

// 006D33C0..006D4170: motion setup for the current path event. The aim point is the event point (target id
// 7F000000h), the target object's node [+128h] plus the event point (object type > 4), else (0, 0, 0). A zero
// distance stops the projectile; otherwise the event type [+2Dh] (1..13) selects the launch. `output` is read only
// (its three DWORDs are copied). RET 4.
void __fastcall object_projectile_006d33c0(void* self, void*, Vector* output) {
    Point toward = kZero;                                                                // [esp+40h]
    Point target = kZero;                                                                // [esp+4Ch]
    if (ptr(self, 0x198) == nullptr) return;                                             // 006D33CB..006D341C
    if (rd<U>(self, 0x124) == kInvalidId) {                                              // 006D3422
        target = point_at(ptr(self, 0x198), 0);                                          // 006D356C..006D357C
    } else {
        void* const array = kotor_fastcall_005ed570(client(), nullptr);                  // 006D3432..006D343A
        void* object = nullptr;                                                          // [esp+20h]
        if (kotor_fastcall_004d8230(array, nullptr, rd<U>(self, 0x124), &object) == object_projectile_literal_0074666c[0] &&
            rd<B>(object, 8) > object_projectile_literal_00746633[0]) {                 // 006D3452..006D346B (jbe: unsigned)
            Point part = kZero;                                                          // [esp+10h]
            Quaternion q = identity();                                                   // [esp+5Ch]
            if (get_model(object) == nullptr) {                                          // 006D34AB..006D34BC
                target = point_at(self, 0x114);                                          // 006D3550..006D3566
            } else {
                void* const gob = get_model(object);                                     // 006D34CD
                if (part_position(gob, self, &part, &q) == 0) {                          // 006D34E1..006D34F7
                    part = point_at(object, 0x24);                                       // 006D34F9..006D350C
                    part.z = l3m(static_cast<float>(static_cast<long double>(l3mf(part.z)) + l3lit(object_projectile_literal_00757ca8)));   // 006D350C..006D351A fadd [00757CA8] (1.25f) ; fstp dword (L3 t1)
                }
                Vector sum;                                                              // [esp+7Ch]
                const Vector node = vector_of(part);
                target = point_at(vector_sum_00423b90(&sum, &node, static_cast<const Vector*>(ptr(self, 0x198))), 0);   // 006D351E..006D354A
            }
        }
    }
    // L3 t1: 006D3580..006D35AD fld target ; fsub dword [esi+24h..2Ch] ; fstp dword (toward x, y, z).
    toward = {l3m(static_cast<float>(l3sub_m(l3mf(target.x), self, 0x24))), l3m(static_cast<float>(l3sub_m(l3mf(target.y), self, 0x28))),
              l3m(static_cast<float>(l3sub_m(l3mf(target.z), self, 0x2c)))};
    // 006D35B1..006D35E0: (z*z + y*y) + x*x ; fcomp [00747504] ; test 41h ; jp (greater or unordered goes on).
    const long double tz = l3mf(toward.z), ty = l3mf(toward.y), tx = l3mf(toward.x);
    const long double length = (tz * tz + ty * ty) + tx * tx;
    if (length <= l3lit(object_projectile_literal_00747504)) {
        stop(self);                                                                      // 006D35E2..006D3643
        return;
    }
    const M32 seconds = l3m(static_cast<float>(l3seconds(self, 0x120)));                 // 006D3653..006D367E fstp dword [esp+20h] (the word is passed on)
    Point dir = point_at(output, 0);                                                     // [esp+28h..30h], DWORD copies
    switch (static_cast<U>(rd<B>(ptr(self, 0x198), 0x2d)) - 1u) {                        // 006D3694..006D36A2, table 006D4174
    case 0:    // type 1
    case 5:    // type 6
    case 12:   // type 13
        put_point(self, 0x184, kZero);                                                   // 006D3BBA..006D3BE9
        return;
    case 1:    // type 2: gravity
        put_point(self, 0x184, {M32{0}, M32{0}, M32{kGravity}});                         // 006D378F..006D3BE9
        return;
    case 3:    // type 4
        l3_random_launch(self, dir, toward, seconds, Spread::hundredths);
        return;
    case 4:    // type 5: from rest
        put_point(self, 0x178, kZero);                                                   // 006D3B04..006D3B3A
        l3_accelerate(self, toward, seconds);
        return;
    case 6:    // type 7
        stop(self);                                                                      // 006D3BF9..006D3C5E
        attach_to_target(self);
        return;
    case 8:    // type 9
        l3_random_launch(self, dir, toward, seconds, Spread::coin);
        return;
    case 9:    // type 10
        l3_lob(self, dir, toward, seconds);
        return;
    case 11:   // type 12
        l3_random_launch(self, dir, toward, seconds, Spread::hundredths_gob);
        own_reference(self);
        return;
    default:   // types 3, 8, 11 and every value outside 1..13 (ja 006D4102)
        stop(self);                                                                      // 006D4102..006D4163
        return;
    }
}

// 006D7590..006D7B93: one frame of path motion. Re-aims a tracked target, advances the path event when it is used up
// (006D70D0), stops at the destination or within the event radius, otherwise records the previous position and
// runs the velocity update the event type selects (table 006D7B94 by index bytes 006D7BA4). RET 0.
void __fastcall object_projectile_006d7590(void* self, void*) {
    Point p = kZero;                                                                     // [esp+18h] (offsets after the four pushes)
    if (ptr(self, 0x198) == nullptr) {                                                   // 006D7596..006D75B6
        wr<U>(self, 0x164, 1);                                                           // 006D75B8
        return;
    }
    bool advanced = false;                                                               // EDI
    Quaternion q;                                                                        // [esp+3Ch]
    if (rd<U>(ptr(self, 0x198), 0x18) == menu_tables_m41::cells::invalid_object_00746644) {   // 006D75C7..006D75DB
        put_point(self, 0x114, point_at(ptr(self, 0x198), 0));                           // 006D75DD..006D75F2
    } else {
        q = identity();                                                                  // 006D7600..006D7618
        void* const array = kotor_fastcall_005ed570(client(), nullptr);                  // 006D75FA..006D7623
        void* object = nullptr;                                                          // [esp+10h]
        if (kotor_fastcall_004d8230(array, nullptr, rd<U>(ptr(self, 0x198), 0x18), &object) == object_projectile_literal_0074666c[0]) {
            target_point(self, object, p, q);
            place_event(self, p);
        } else {
            put_point(self, 0x114, point_at(ptr(self, 0x198), 0));                       // 006D77C5..006D77E0
        }
    }
    const U elapsed = rd<U>(self, 0x44), duration = rd<U>(self, 0x120);                 // 006D77E3..006D77EE
    if (elapsed >= duration) {                                                           // jb (unsigned)
        wr<U>(self, 0x44, elapsed - duration);
        wr<U>(self, 0x120, 0);
        object_table(self)->s08c_set_position(self, nullptr, static_cast<const float*>(at(self, 0x114)));   // 006D7804
        advanced = true;
    }
    const U left = rd<U>(self, 0x120) - rd<U>(self, 0x44);                               // 006D780F..006D7818
    if (left <= rd<U>(ptr(self, 0x198), 0x24) || advanced) {                             // jbe / test edi,edi
        if (object_projectile_006d70d0(self, nullptr) == 0) return;                      // 006D7829..006D7832
    }
    wr<U>(self, 0x120, rd<U>(self, 0x120) - rd<U>(self, 0x44));                          // 006D7838..006D784B
    if (ptr(self, 0x198) == nullptr) return;                                             // 006D7849..006D7851
    if (rd<U>(ptr(self, 0x198), 0x18) == menu_tables_m41::cells::invalid_object_00746644) {   // 006D7857..006D7860
        put_point(self, 0x114, point_at(ptr(self, 0x198), 0));                           // 006D7862..006D7871
    } else {
        q = identity();                                                                  // 006D787F..006D7897
        void* const array = kotor_fastcall_005ed570(client(), nullptr);
        void* object = nullptr;                                                          // [esp+14h]
        if (kotor_fastcall_004d8230(array, nullptr, rd<U>(ptr(self, 0x198), 0x18), &object) == object_projectile_literal_0074666c[0]) {
            target_point(self, object, p, q);              // p keeps its value from above
            place_event(self, p);
        }                                                                                // not found: the destination is kept
    }
    // 006D7A38..006D7A6C: at the destination (|destination - position|^2 <= eps)?
    const long double dx = static_cast<long double>(l2f(self, 0x114)) - l2f(self, 0x24);   // 006D7A38..006D7A48 (registers)
    const long double dy = static_cast<long double>(l2f(self, 0x118)) - l2f(self, 0x28);
    const long double dz = static_cast<long double>(l2f(self, 0x11c)) - l2f(self, 0x2c);
    const long double rest = (dz * dz + dy * dy) + dx * dx;
    if (rest <= lit_f(object_projectile_literal_00747504)) {                             // fcomp ; test ah,41h ; jp: > or unordered goes to 006D7AB6
        object_table(self)->s08c_set_position(self, nullptr, static_cast<const float*>(at(self, 0x114)));   // 006D7A73
        if (!(l2f(ptr(self, 0x198), 0x28) >= lit_f(object_projectile_literal_0073d700))) return;   // test ah,1; jne (< or unordered)
        next_event_keep_time(self);
        return;
    }
    void* const event = ptr(self, 0x198);                                                // EBX (006D7AB6)
    if (l2f(event, 0x28) >= lit_f(object_projectile_literal_0073d700) &&                // 006D7ABC..006D7ACA fcomp ; test ah,1 ; jne
        rd<U>(event, 0x18) != menu_tables_m41::cells::invalid_object_00746644) {         // 006D7ACC..006D7AD5
        float away[3];                                                                   // [esp+30h]
        const float* const d = kotor_cdecl_00423bc0(away, static_cast<const float*>(at(self, 0x24)), reinterpret_cast<const float*>(&p));   // position - p
        const Point gap = point_at(d, 0);                                                // [esp+24h..2Ch] (mov)
        const long double radius = l2f(event, 0x28);                                     // 006D7AE7
        const long double distance = sum_squares(gap, Squares{2, 1, 0});                 // (z^2 + y^2) + x^2
        if (radius * radius >= distance) {                                               // fld st(1) ; fmul st(2) ; fcompp ; test ah,1 ; je
            next_event_keep_time(self);
            return;
        }
    }
    put_point(self, 0x100, point_at(self, 0x24));                                        // 006D7B30..006D7B43 previous position
    const U index = static_cast<U>(rd<B>(event, 0x2d)) - 1u;                             // movzx; dec
    if (index > 12u) return;                                                             // ja 006D7B8C
    static constexpr B kCase[13] = {0, 0, 3, 0, 0, 1, 3, 3, 0, 0, 3, 0, 2};              // index bytes 006D7BA4
    switch (kCase[index]) {                                                              // targets 006D7B94
    case 0:                                                                              // 006D7B85
        object_projectile_006d2e00(self, nullptr);
        return;
    case 1:                                                                              // 006D7B76
        object_projectile_006d30c0(self, nullptr);
        return;
    case 2:                                                                              // 006D7B5E
        object_projectile_006d2e00(self, nullptr);
        (void)object_projectile_006d4250(self, nullptr, 4u);                             // PlayAnimation(4)
        return;
    default:                                                                             // 006D7B8C
        return;
    }
}

}  // extern "C"
