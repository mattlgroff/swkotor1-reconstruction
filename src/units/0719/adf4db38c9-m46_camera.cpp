// lane-m46-3dcore fork D: CAurCamera (vtable 00741B20) slot bodies + CAurBehaviorCameraFollow::GetTargetWorldPosition.
// Original: swkotor.exe GOG K1 (image base 00400000). Every behaviour cites the original instruction addresses.
// Conventions: D1 (thiscall slots as __fastcall(self, edx_unused, ...)), raw-word slot contract of vtables2_gen.h
// (float arguments arrive as raw DWORDs, converted with bits_to_float, never through an FPU load/store),
// D30-FINAL (wide x87 intermediates are doubles; float only at the original fstp dword stores; x87 FPTAN/FSIN/FCOS
// executed on the x87 unit via D21 inline asm), D41 (virtual calls through published typed tables only).
// Camera layout: byte-offset views of the 0x234-byte object (0045D946); the embedded Gob starts at +4 (0045D25B).
// The four slots whose original result is ST0 but whose generated declaration is EAX (0045C170, 0045C1A0,
// 0045C1D0, 0045D300) are authored in m46_camera_st0.cpp (header diff in parts/D.json).
#include "6c81d8874d-scene3d.h"
#include "647ed12135-m46_camera_externs.h"

namespace kotor_renderer_init {   // aurora-windows-gl-imports (M45 defined ?glGetIntegerv_0073d244@kotor_renderer_init@@YGXIPAH@Z)
void __stdcall glGetIntegerv_0073d244(std::uint32_t, std::int32_t*);
}

namespace {
using namespace kotor_scene3d;

// .rdata constants (read from the pinned image)
constexpr U kAngle45 = 0x42340000u;           // 45.0f immediate (0045BF06 / 0045BF84)
constexpr float kHalfDegToRad = 0.008726646192371845f;   // [00741BB4] 0x3C0EFA35 (pi/360)
constexpr float kHalfPi = 1.5707963705062866f;           // [00741BB8] 0x3FC90FDB
constexpr float kPi = 3.1415927410125732f;               // [00741BBC] 0x40490FDB
constexpr float kOne = 1.0f;                             // [0073D764]
constexpr float kHalf = 0.5f;                            // [0073E9AC]
constexpr float kZero = 0.0f;                            // [0073D700]
constexpr std::uint32_t kGlViewport = 0x0BA2u;           // push 0BA2h (0045CF6D / 0045D07D / 0045CDDA)

inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
template<class T> inline T* ptr(U w) { return reinterpret_cast<T*>(static_cast<std::uintptr_t>(w)); }
inline float fl(void* o, U off) { return field<float>(o, off); }

// D21 / lane x87-trig (2026-10-24): the former x87_tan / x87_cos / x87_sin returned the 64-bit-significand results
// rounded to double although the original keeps using them on the x87 stack (0045D288/0045D2A9/0045D2BE, 0045D179,
// 0045CF92). Each site now runs one x87 sequence (x87trig_* after this namespace). Memory operands (image bits):
const U kX87HalfDegToRad_00741bb4 = 0x3C0EFA35u;   // pi/360
const U kX87HalfPi_00741bb8 = 0x3FC90FDBu;         // 1.5707964f
const U kX87Pi_00741bbc = 0x40490FDBu;             // 3.1415927f
const U kX87One_0073d764 = 0x3F800000u;            // 1.0f
const U kX87Half_0073e9ac = 0x3F000000u;           // 0.5f

// Viewport words at Camera+1E4..+1F0 copied to a local; if width or height is 0 the GL viewport is queried
// (0045CF36..0045CF72 / 0045D046..0045D082 / 0045CDA5..0045CDDF).
struct Viewport { I x, y, w, h; };
inline Viewport camera_viewport(void* camera) {
    Viewport v{field<I>(camera, 0x1E4), field<I>(camera, 0x1E8), field<I>(camera, 0x1EC), field<I>(camera, 0x1F0)};
    if (v.w == 0 || v.h == 0) kotor_renderer_init::glGetIntegerv_0073d244(kGlViewport, &v.x);
    return v;
}

// Typed table views (D41): reload [object] at every call (original `mov reg,[obj]; call [reg+slot]`).
inline const CameraVTable& camera_slots(void* camera) {
    return *static_cast<const CameraVTable*>(*static_cast<void* const volatile*>(camera));
}
inline const SceneVTable& scene_slots_of(void* scene) {
    return *static_cast<const SceneVTable*>(*static_cast<void* const volatile*>(scene));
}
} // namespace

// ---- lane x87-trig (2026-10-24): original x87 sequences around fcos/fsin/fptan kept whole ----
// fsin/fcos/fptan deliver a 64-bit significand whatever the precision control; each helper keeps the
// transcendental with the original's consuming x87 instructions up to the original's dword stores, with the
// original operand order. Precondition as in the original: PC = 53 bits, RC = nearest. No CALL inside the asm.
extern "C" {
// Camera::Animate view-angle record {+0 duration, +4 elapsed, +8 from, +C to, +10 mode}; result to Camera+1D0.
// Common tail 0045D2C0..0045D2DA: (1 - t) * from + t * to.
// mode 1: 0045D274..0045D296 t = 1 - (cos(e/d * pi) + 1) * 0.5
__declspec(noinline) void __cdecl x87trig_0045d288(const void* rec, void* camera) {
    __asm {
        mov ecx, rec
        mov eax, camera
        fld dword ptr [ecx + 4]                // 0045D274
        fdiv dword ptr [ecx]                   // 0045D279
        fmul dword ptr kX87Pi_00741bbc         // 0045D282
        fcos                                   // 0045D288
        fadd dword ptr kX87One_0073d764        // 0045D28A
        fmul dword ptr kX87Half_0073e9ac       // 0045D290
        fsubr dword ptr kX87One_0073d764       // 0045D296
        fld dword ptr kX87One_0073d764         // 0045D2C0
        fsub st, st(1)                         // 0045D2CA D8 E1
        fmul dword ptr [ecx + 8]               // 0045D2CD
        fxch st(1)                             // 0045D2D0
        fmul dword ptr [ecx + 0Ch]             // 0045D2D2
        faddp st(1), st                        // 0045D2D8 DE C1
        fstp dword ptr [eax + 1D0h]            // 0045D2DA
    }
}
// mode 2: 0045D274..0045D2AB t = 1 - cos(e/d * pi/2)
__declspec(noinline) void __cdecl x87trig_0045d2a9(const void* rec, void* camera) {
    __asm {
        mov ecx, rec
        mov eax, camera
        fld dword ptr [ecx + 4]                // 0045D274
        fdiv dword ptr [ecx]                   // 0045D279
        fmul dword ptr kX87HalfPi_00741bb8     // 0045D2A3
        fcos                                   // 0045D2A9
        fsubr dword ptr kX87One_0073d764       // 0045D2AB
        fld dword ptr kX87One_0073d764         // 0045D2C0
        fsub st, st(1)                         // 0045D2CA
        fmul dword ptr [ecx + 8]               // 0045D2CD
        fxch st(1)                             // 0045D2D0
        fmul dword ptr [ecx + 0Ch]             // 0045D2D2
        faddp st(1), st                        // 0045D2D8
        fstp dword ptr [eax + 1D0h]            // 0045D2DA
    }
}
// mode 3: 0045D274..0045D2BE t = sin(e/d * pi/2)
__declspec(noinline) void __cdecl x87trig_0045d2be(const void* rec, void* camera) {
    __asm {
        mov ecx, rec
        mov eax, camera
        fld dword ptr [ecx + 4]                // 0045D274
        fdiv dword ptr [ecx]                   // 0045D279
        fmul dword ptr kX87HalfPi_00741bb8     // 0045D2B8
        fsin                                   // 0045D2BE
        fld dword ptr kX87One_0073d764         // 0045D2C0
        fsub st, st(1)                         // 0045D2CA
        fmul dword ptr [ecx + 8]               // 0045D2CD
        fxch st(1)                             // 0045D2D0
        fmul dword ptr [ecx + 0Ch]             // 0045D2D2
        faddp st(1), st                        // 0045D2D8
        fstp dword ptr [eax + 1D0h]            // 0045D2DA
    }
}
// Camera::MouseDir 0045CF78..0045CFEF: t = tan(view * pi/360) stays on the stack for both components.
// vp = the viewport words {x, y, w, h}; xa = x - vp.x, ya = y - vp.y ([esp+34h] / EDX).
__declspec(noinline) void __cdecl x87trig_0045cf92(const void* camera, const I* vp, const I* xa, const I* ya,
                                                   float* vx, float* vy) {
    __asm {
        mov eax, camera
        mov ecx, vp
        fld dword ptr [eax + 1D0h]             // 0045CF78
        fmul dword ptr kX87HalfDegToRad_00741bb4  // 0045CF82
        fptan                                  // 0045CF92
        fstp st(0)                             // 0045CFB7
        fild dword ptr [ecx + 0Ch]             // 0045CFB9 h
        fmul dword ptr kX87Half_0073e9ac       // 0045CFBD
        mov eax, xa
        fild dword ptr [eax]                   // 0045CFC3
        fild dword ptr [ecx + 8]               // 0045CFCB w
        fmul dword ptr kX87Half_0073e9ac       // 0045CFCF
        fsubp st(1), st                        // 0045CFD5 DE E9
        fmul st, st(2)                         // 0045CFD7 D8 CA
        fdiv st, st(1)                         // 0045CFD9 D8 F1
        mov eax, vx
        fstp dword ptr [eax]                   // 0045CFDB [esp+10h]
        mov eax, ya
        fild dword ptr [eax]                   // 0045CFDF
        fsub st, st(1)                         // 0045CFE3 D8 E1
        fmul st, st(2)                         // 0045CFE5
        fdiv st, st(1)                         // 0045CFE7
        mov eax, vy
        fstp dword ptr [eax]                   // 0045CFE9 [esp+14h]
        fstp st(0)                             // 0045CFED
        fstp st(0)                             // 0045CFEF
    }
}
// Camera::MapToScreen 0045D164..0045D195: h_f = (float)h ([esp+0Ch]), half_h = (float)(h * 0.5) ([esp+4]),
// k = (float)(h * 0.5 / tan(view * pi/360)) ([esp+50h]).
__declspec(noinline) void __cdecl x87trig_0045d179(const void* camera, const I* vp, float* h_f, float* half_h, float* k) {
    __asm {
        mov eax, camera
        mov ecx, vp
        fld dword ptr [eax + 1D0h]             // 0045D164
        fmul dword ptr kX87HalfDegToRad_00741bb4  // 0045D16A
        fptan                                  // 0045D179
        fstp st(0)                             // 0045D17B
        fild dword ptr [ecx + 0Ch]             // 0045D17D h
        mov eax, h_f
        fst dword ptr [eax]                    // 0045D181
        fmul dword ptr kX87Half_0073e9ac       // 0045D185
        mov eax, half_h
        fst dword ptr [eax]                    // 0045D18B
        fdiv st, st(1)                         // 0045D18F D8 F1
        mov eax, k
        fstp dword ptr [eax]                   // 0045D191
        fstp st(0)                             // 0045D195
    }
}
}  // extern "C"

extern "C" {

// 0045BEF0 Camera::BeginViewAngleAnimation(to_angle, duration, mode): thiscall RET 0Ch.
U __fastcall camera_begin_view_angle_animation_0045bef0(void* self, void*, U to_angle, U duration, U mode) {
    void* old = field<void*>(self, 0x1D4);                 // 0045BEF3
    if (old != nullptr) {                                  // 0045BEF9
        live_free(old);                                    // 0045BEFE call 006FA390
        field<U>(self, 0x1D0) = kAngle45;                  // 0045BF06 view angle reset to 45.0f
    }
    void* rec = live_operator_new(0x14);                   // 0045BF10..0045BF12 (no null test in the original)
    field<void*>(self, 0x1D4) = rec;                       // 0045BF1B
    field<U>(rec, 0x00) = duration;                        // 0045BF21 [rec+0] = arg2
    field<U>(rec, 0x04) = 0u;                              // 0045BF2D elapsed
    field<U>(rec, 0x0C) = to_angle;                        // 0045BF3A [rec+C] = arg1
    const U from = field<U>(self, 0x1D0);                  // 0045BF43
    field<U>(rec, 0x08) = from;                            // 0045BF49 [rec+8] = current angle
    field<U>(rec, 0x10) = mode;                            // 0045BF59 [rec+10] = arg3
    return from;                                           // EAX = [esi+1D0] loaded at 0045BF43
}

// 0045BF60 Camera::EndViewAngleAnimation(bool reset): thiscall RET 4.
U __fastcall camera_end_view_angle_animation_0045bf60(void* self, void*, U reset) {
    live_free(field<void*>(self, 0x1D4));                  // 0045BF63..0045BF6A (free(NULL) allowed)
    const U al = reset & 0xFFu;                            // 0045BF6F mov al,[esp+0Ch]
    field<void*>(self, 0x1D4) = nullptr;                   // 0045BF78
    if (al != 0) field<U>(self, 0x1D0) = kAngle45;         // 0045BF76 / 0045BF84
    // Original EAX = free()'s EAX with AL replaced by the argument byte (0045BF6F); no caller reads it.
    // Labeled deviation (D2-style): return the observed AL value zero-extended.
    return al;
}

// 0045BFA0 Camera::SetViewPort(x, y, width, height): thiscall RET 10h.
U __fastcall camera_set_view_port_0045bfa0(void* self, void*, U x, U y, U width, U height) {
    field<U>(self, 0x1E4) = x;                             // 0045BFA8
    field<U>(self, 0x1E8) = y;                             // 0045BFB4
    I w = static_cast<I>(width);
    if (!(w > 0)) w = 1;                                   // 0045BFB2 test / jg / mov eax,1
    field<I>(self, 0x1EC) = w;                             // 0045BFC1
    I h = static_cast<I>(height);
    if (!(h > 0)) h = 1;                                   // 0045BFCB / 0045BFCF
    field<I>(self, 0x1F0) = h;                             // 0045BFD4
    return static_cast<U>(h);                              // EAX = stored height (0045BFD4)
}

// 0045BFE0 Camera::HitCheck(Vector start, Vector end, Vector* out): thiscall RET 1Ch; forwards to the embedded
// Gob::HitCheck 0043EB80 with the CHitInfo* argument pushed as 0 (0045BFE4 push 0, 0045C016 add ecx,4).
U __fastcall camera_hit_check_0045bfe0(void* self, void*, U sx, U sy, U sz, U ex, U ey, U ez, U out) {
    const Vector start{bits_to_float(sx), bits_to_float(sy), bits_to_float(sz)};   // 0045C000..0045C019
    const Vector end{bits_to_float(ex), bits_to_float(ey), bits_to_float(ez)};     // 0045BFE7..0045BFFD
    return static_cast<U>(gob_hit_check_0043eb80(static_cast<aurora_model::Gob*>(at(self, 4)), nullptr,
                                                 start, end, ptr<Vector>(out), nullptr));   // 0045C01C
}

// 0045C060 Camera::SetOrientation(Quaternion q) -> Quaternion by value (hidden pointer first): thiscall RET 14h.
U __fastcall camera_set_orientation_0045c060(void* self, void*, U result, U w, U x, U y, U z) {
    U* q = &field<U>(self, 0x88);                          // 0045C065 add ecx,88h
    q[0] = w; q[1] = x; q[2] = y; q[3] = z;                // 0045C071..0045C083 raw DWORD copies
    U* r = ptr<U>(result);                                 // 0045C086 [esp+10h] = hidden return pointer
    r[0] = w; r[1] = x; r[2] = y; r[3] = z;                // 0045C08C..0045C094
    return result;                                         // EAX = hidden pointer
}

// 0045C0C0 Camera::SetFollow(CAurObject* target): thiscall RET 4. EH frame 00716A4B (state 0 = free the new'd
// behaviour if its constructor throws) is cited, not emitted: the constructor is an extern "C" no-throw entry and
// operator new 006FA7E6 returns NULL (D30-FINAL unwind rule).
U __fastcall camera_set_follow_0045c0c0(void* self, void*, U target) {
    if (target != 0u) {                                    // 0045C0DB test edi,edi
        void* behavior = live_operator_new(0x70);          // 0045C0E1..0045C0E3
        if (behavior != nullptr) {                         // 0045C0EF
            void* gob = (self != nullptr) ? at(self, 4) : nullptr;   // 0045C0FB..0045C10D (this->Gob base)
            behavior = behavior_camera_follow_construct_0049fe80(behavior, nullptr, gob, ptr<void>(target));   // 0045C106 / 0045C113
        }                                                  // 0045C11A xor eax,eax when allocation failed
        return camera_slots(self).s07c_camera_set_behavior(self, nullptr, behavior);   // 0045C11C..0045C129 [vtbl+7C]
    }
    return camera_slots(self).s07c_camera_set_behavior(self, nullptr, nullptr);       // 0045C13F..0045C145
}

// 0045C470 Camera::GetOrientation -> Quaternion by value (hidden pointer): thiscall RET 4.
U __fastcall camera_get_orientation_0045c470(void* self, void*, U result) {
    const U* q = &field<U>(self, 0x88);                    // 0045C474
    U* r = ptr<U>(result);
    r[0] = q[0]; r[1] = q[1]; r[2] = q[2]; r[3] = q[3];    // 0045C47B..0045C490
    return result;                                         // EAX = hidden pointer (0045C470)
}

// 0045C4A0 Camera::GetViewPortHeight: EAX = [ecx+1F0].
U __fastcall camera_get_view_port_height_0045c4a0(void* self, void*) {
    return field<U>(self, 0x1F0);                          // 0045C4A0
}

// 0045C4B0 Camera::GetClipDist(float* near, float* far): thiscall RET 8; EAX = far bits (0045C4BC).
U __fastcall camera_get_clip_dist_0045c4b0(void* self, void*, U near_out, U far_out) {
    *ptr<U>(near_out) = field<U>(self, 0x1DC);             // 0045C4B0..0045C4BA
    const U far_bits = field<U>(self, 0x1E0);              // 0045C4BC
    *ptr<U>(far_out) = far_bits;                           // 0045C4C6
    return far_bits;
}

// 0045C4D0 Camera::GetViewAngle: fld dword [ecx+1D0] (ST0).
double __fastcall camera_get_view_angle_0045c4d0(void* self, void*) {
    return static_cast<double>(fl(self, 0x1D0));          // 0045C4D0
}

// 0045D760 Camera::GetViewVolume(index) -> Plane by value (hidden pointer): thiscall RET 8.
U __fastcall camera_get_view_volume_0045d760(void* self, void*, U result, U index) {
    const U* plane = reinterpret_cast<const U*>(field<PlaneList>(self, 0x1F8).data) + (index << 4) / 4;   // 0045D760..0045D76D shl 4
    U* r = ptr<U>(result);
    r[0] = plane[0]; r[1] = plane[1]; r[2] = plane[2]; r[3] = plane[3];   // 0045D774..0045D789
    return result;
}

// 0045D220 Camera::Animate(float delta): thiscall RET 4; view-angle animation record at +1D4
// {+0 duration, +4 elapsed, +8 from, +C to, +10 mode}, then Gob::Animate 00486670 on the embedded Gob.
U __fastcall camera_animate_0045d220(void* self, void*, U delta_bits) {
    aurora_model::Gob* gob = static_cast<aurora_model::Gob*>(at(self, 4));   // 0045D25B / 0045D2D5 / 0045D2F0
    void* rec = field<void*>(self, 0x1D4);                 // 0045D223
    if (rec == nullptr) return gob_animate_00486670(gob, nullptr, delta_bits);   // 0045D22B -> 0045D2EB
    field<float>(rec, 4) = static_cast<float>(static_cast<double>(bits_to_float(delta_bits)) +
                                              static_cast<double>(fl(rec, 4)));  // 0045D231..0045D238 fstp
    rec = field<void*>(self, 0x1D4);                       // 0045D23B reload
    const double elapsed = static_cast<double>(fl(rec, 4));                  // 0045D241
    const double duration = static_cast<double>(fl(rec, 0));
    if (elapsed > duration) {                              // 0045D244..0045D24B fcomp; test ah,41h; jne (<, = or unordered keep animating)
        live_free(rec);                                    // 0045D24D
        field<void*>(self, 0x1D4) = nullptr;               // 0045D25E
        return gob_animate_00486670(gob, nullptr, delta_bits);   // 0045D268
    }
    const U mode = field<U>(rec, 0x10);                    // 0045D271
    double t = static_cast<double>(fl(rec, 4)) / static_cast<double>(fl(rec, 0));   // 0045D274 / 0045D279 fdiv
    if (mode == 0u) {
        // 0045D27B je 0045D2C0: linear
    } else if (mode == 1u) {                               // 0045D27D
        x87trig_0045d288(rec, self);                       // 0045D274..0045D296 + 0045D2C0..0045D2DA (x87)
        return gob_animate_00486670(gob, nullptr, delta_bits);   // 0045D2E0
    } else if (mode == 2u) {                               // 0045D29E
        x87trig_0045d2a9(rec, self);                       // 0045D274..0045D2AB + 0045D2C0..0045D2DA (x87)
        return gob_animate_00486670(gob, nullptr, delta_bits);   // 0045D2E0
    } else if (mode == 3u) {                               // 0045D2B3
        x87trig_0045d2be(rec, self);                       // 0045D274..0045D2BE + 0045D2C0..0045D2DA (x87)
        return gob_animate_00486670(gob, nullptr, delta_bits);   // 0045D2E0
    } else {
        return gob_animate_00486670(gob, nullptr, delta_bits);   // 0045D2E9 fstp st(0) -> 0045D2EB
    }
    const double one_minus = static_cast<double>(kOne) - t;   // 0045D2C0..0045D2CA
    field<float>(self, 0x1D0) = static_cast<float>(one_minus * static_cast<double>(fl(rec, 8)) +
                                                   t * static_cast<double>(fl(rec, 0x0C)));   // 0045D2CD..0045D2DA
    return gob_animate_00486670(gob, nullptr, delta_bits); // 0045D2E0
}

// 0045CF30 Camera::MouseDir(Vector* out, int x, int y) -> out: thiscall RET 0Ch.
U __fastcall camera_mouse_dir_0045cf30(void* self, void*, U out, U x, U y) {
    const Viewport vp = camera_viewport(self);             // 0045CF36..0045CF72
    const I xa = static_cast<I>(x - static_cast<U>(vp.x)); // 0045CF7E..0045CF8C
    const I ya = static_cast<I>(y - static_cast<U>(vp.y)); // 0045CF94..0045CF9C
    Vector v;
    x87trig_0045cf92(self, &vp.x, &xa, &ya, &v.x, &v.y);   // 0045CF78..0045CFEF (x87: tan kept on the stack)
    v.z = -1.0f;                                           // 0045CFAF mov [esp+18h],0BF800000h
    Vector rotated;
    const Vector* r = quaternion_rotate_004a9bb0(&rotated, &field<Quaternion>(self, 0x88), &v);   // 0045CFF1
    v = *r;                                                // 0045CFF6..0045D00D
    vector_normalize_004ab130(&v, nullptr);                // 0045D011
    *ptr<Vector>(out) = v;                                 // 0045D016..0045D02D
    return out;                                            // EAX = out (0045D016)
}

// 0045D040 Camera::MapToScreen(Vector* out, Vector world, bool normalized) -> out: thiscall RET 14h.
U __fastcall camera_map_to_screen_0045d040(void* self, void*, U out, U px, U py, U pz, U normalized) {
    const Viewport vp = camera_viewport(self);             // 0045D046..0045D082
    Vector* o = ptr<Vector>(out);
    if (!(static_cast<double>(fl(self, 0x1D0)) > static_cast<double>(kZero))) {   // 0045D088..0045D099 test ah,41h jne
        o->x = 0.0f; o->y = 0.0f; o->z = 0.0f;             // 0045D1F4..0045D205 (raw zero DWORDs)
        return out;
    }
    Vector d;
    d.x = static_cast<float>(static_cast<double>(bits_to_float(px)) - static_cast<double>(fl(self, 0x7C)));   // 0045D09F..0045D0A6
    d.y = static_cast<float>(static_cast<double>(bits_to_float(py)) - static_cast<double>(fl(self, 0x80)));   // 0045D0AE..0045D0C1
    d.z = static_cast<float>(static_cast<double>(bits_to_float(pz)) - static_cast<double>(fl(self, 0x84)));   // 0045D0C5..0045D0DE
    Quaternion conj;
    const Quaternion* c = quaternion_negate_w_004a9b50(&conj, &field<Quaternion>(self, 0x88));   // 0045D0EF
    Vector rotated_tmp;
    const Vector* rr = quaternion_rotate_004a9bb0(&rotated_tmp, c, &d);   // 0045D0FD
    const Vector r = *rr;                                  // 0045D102..0045D11C
    const Vector origin{0.0f, 0.0f, 0.0f};                 // 0045D12F..0045D13F
    const Vector normal{0.0f, 0.0f, 1.0f};                 // 0045D147..0045D157
    Vector p;
    plane_line_intersection_004ab250(&p, &normal, 1.0f, &origin, &r);   // 0045D120 push 3F800000h / 0045D15F
    float h_f, half_h, k;                                                                   // [esp+0Ch] [esp+4] [esp+50h]
    x87trig_0045d179(self, &vp.x, &h_f, &half_h, &k);                                       // 0045D164..0045D195 (x87)
    const float w_f = static_cast<float>(static_cast<double>(vp.w));                        // 0045D197..0045D19B
    double sx = static_cast<double>(k) * static_cast<double>(p.x) +
                static_cast<double>(w_f) * static_cast<double>(kHalf);                      // 0045D19F..0045D1B1
    sx = sx + static_cast<double>(vp.x);                                                    // 0045D1B3 fiadd
    double sy = static_cast<double>(k) * static_cast<double>(p.y) + static_cast<double>(vp.y);   // 0045D1B7..0045D1BF
    sy = sy + static_cast<double>(half_h);                                                  // 0045D1C3
    if ((normalized & 0xFFu) != 0) {                       // 0045D170 mov al / 0045D177 test / 0045D1C7 je
        sx = sx / static_cast<double>(w_f);                // 0045D1CB
        sy = static_cast<double>(kOne) - sy / static_cast<double>(h_f);   // 0045D1D1..0045D1D5
    }
    o->x = static_cast<float>(sx);                         // 0045D1E1
    o->z = 0.0f;                                           // 0045D1E3
    o->y = static_cast<float>(sy);                         // 0045D1EB
    return out;
}

// 0045CC90 Camera::HitCheck(int x, int y, Vector* out, CHitInfo* hit): thiscall RET 10h; EAX from Scene vtbl+38.
U __fastcall camera_hit_check_0045cc90(void* self, void*, U x, U y, U out, U hit) {
    void* scene = field<void*>(self, 0x6C);                // 0045CC96
    if (scene == nullptr) return 0u;                       // 0045CC9C -> 0045CF1D xor eax,eax
    const double px = static_cast<double>(fl(self, 0x7C)), py = static_cast<double>(fl(self, 0x80)),
                 pz = static_cast<double>(fl(self, 0x84));
    if (static_cast<double>(fl(self, 0x1D0)) > static_cast<double>(kZero)) {   // 0045CCA2..0045CCB3
        Vector dir;
        camera_slots(self).s02c_camera_mouse_dir(self, nullptr, word(&dir), x, y);   // 0045CCB9..0045CCCC [vtbl+2C]
        const double far_clip = static_cast<double>(fl(self, 0x1E0));             // 0045CCCF
        Vector end;
        end.x = static_cast<float>(static_cast<double>(dir.x) * far_clip + px);   // 0045CCD5 / 0045CD06 wide product
        const float fy = static_cast<float>(static_cast<double>(dir.y) * far_clip);   // 0045CCE7 / 0045CCF0 fstp
        const float fz = static_cast<float>(static_cast<double>(dir.z) * far_clip);   // 0045CCFB / 0045CD02 fstp
        end.y = static_cast<float>(static_cast<double>(fy) + py);                 // 0045CD0F..0045CD19
        end.z = static_cast<float>(static_cast<double>(fz) + pz);                 // 0045CD1D..0045CD27
        const double near_clip = static_cast<double>(fl(self, 0x1DC));            // 0045CD2B
        Vector start;
        start.x = static_cast<float>(static_cast<double>(dir.x) * near_clip + px);   // 0045CD31 / 0045CD4B wide product
        const float ny = static_cast<float>(static_cast<double>(dir.y) * near_clip);   // 0045CD37..0045CD3D
        const float nz = static_cast<float>(static_cast<double>(dir.z) * near_clip);   // 0045CD41..0045CD47
        start.y = static_cast<float>(static_cast<double>(ny) + py);               // 0045CD54..0045CD5E
        start.z = static_cast<float>(static_cast<double>(nz) + pz);               // 0045CD62..0045CD8F
        return static_cast<U>(scene_slots_of(scene).s038_scene_hit_check(scene, nullptr, start, end,
                                                                          ptr<Vector>(out), ptr<void>(hit)));   // 0045CD9A [vtbl+38]
    }
    // orthographic path 0045CDA5
    const Viewport vp = camera_viewport(self);             // 0045CDA5..0045CDE5
    const I xa = static_cast<I>(x - static_cast<U>(vp.x)); // 0045CDE9..0045CDF5
    const I ya = static_cast<I>(y - static_cast<U>(vp.y)); // 0045CDF7..0045CDFB
    if (xa < 0 || ya < 0 || xa >= vp.w || ya >= vp.h) return 0u;   // 0045CE07..0045CE21
    const double h = static_cast<double>(vp.h);            // 0045CE27 fild
    const float ox = static_cast<float>((static_cast<double>(xa) + static_cast<double>(kHalf)) / h *
                                        static_cast<double>(fl(self, 0x1D8)));   // 0045CE2B..0045CE3D
    const float oy = static_cast<float>((static_cast<double>(ya) + static_cast<double>(kHalf)) / h *
                                        static_cast<double>(fl(self, 0x1D8)));   // 0045CE45..0045CE5B
    const float neg_near = -fl(self, 0x1DC);               // 0045CE69..0045CE76 fchs / fstp (exact negation)
    Vector end;
    end.x = static_cast<float>(static_cast<double>(ox) + px);   // 0045CE8C..0045CE9D
    end.y = static_cast<float>(static_cast<double>(oy) + py);   // 0045CEA4..0045CEB3
    end.z = static_cast<float>(static_cast<double>(neg_near) - static_cast<double>(fl(self, 0x1E0)) + pz);   // 0045CE7A..0045CEBD
    Vector start;
    start.x = static_cast<float>(static_cast<double>(ox) + px);   // 0045CEC1..0045CEC8
    start.y = static_cast<float>(static_cast<double>(oy) + py);   // 0045CECC..0045CED6
    start.z = static_cast<float>(static_cast<double>(neg_near) + pz);   // 0045CEDA..0045CF07
    return static_cast<U>(scene_slots_of(scene).s038_scene_hit_check(scene, nullptr, start, end,
                                                                      ptr<Vector>(out), ptr<void>(hit)));   // 0045CF12
}

// 0045D490 Camera::SceneBoundaryCheck(const Vector* a, const Vector* b, const Plane* bound, Vector* out):
// thiscall RET 10h, EAX 0/1.
U __fastcall camera_scene_boundary_check_0045d490(void* self, void*, U a_word, U b_word, U bound_word, U out_word) {
    const Plane ground{0.0f, 0.0f, 1.0f, 0.0f};            // 0045D496..0045D4D1 locals (0,0,1) + d 0
    const I partner[4] = {4, 5, 3, 2};                     // 0045D4D9..0045D4ED
    Vector points[4] = {};                                 // 0045D4F5..0045D50C zeroed
    const Plane* bound = ptr<const Plane>(bound_word);     // 0045D50F ebx = arg3
    const Vector* position = &field<Vector>(self, 0x7C);   // 0045D527 esi = this+7C
    I best_index = 0;                                      // 0045D517
    float best = 0.0f;                                     // 0045D51F
    for (I i = 0; i < 4; ++i) {                            // 0045D531 ebp = 20h .. 60h step 10h
        const Plane* planes = field<PlaneList>(self, 0x1F8).data;   // 0045D540 reload
        Vector cross_tmp;
        const Vector* c = vector_cross_004ab210(&cross_tmp, reinterpret_cast<const Vector*>(&planes[2 + i]),
                                                reinterpret_cast<const Vector*>(&planes[partner[i]]));   // 0045D546..0045D55F
        const Vector cr = *c;                              // 0045D564..0045D58E
        Vector end;
        end.x = static_cast<float>(static_cast<double>(cr.x) + static_cast<double>(position->x));   // 0045D56D..0045D57E
        end.y = static_cast<float>(static_cast<double>(cr.y) + static_cast<double>(position->y));   // 0045D586..0045D594
        end.z = static_cast<float>(static_cast<double>(cr.z) + static_cast<double>(position->z));   // 0045D598..0045D5AC
        Vector hit;
        const Vector* h = plane_line_intersection_004ab250(&hit, reinterpret_cast<const Vector*>(&ground), ground.d,
                                                           position, &end);   // 0045D5B0
        points[i] = *h;                                    // 0045D5B5..0045D5C4
        const double dist = static_cast<double>(points[i].z) * static_cast<double>(bound->c) +
                            static_cast<double>(bound->a) * static_cast<double>(points[i].x) +
                            static_cast<double>(points[i].y) * static_cast<double>(bound->b) +
                            static_cast<double>(bound->d);  // 0045D5C7..0045D5DE
        if (dist > static_cast<double>(best)) {            // 0045D5E1..0045D5EA fcom / test ah,41h jne
            best = static_cast<float>(dist);               // 0045D5F0
            best_index = i;                                // 0045D5F4
        }
    }
    if (static_cast<double>(best) == static_cast<double>(kZero)) return 0u;   // 0045D614..0045D631
    const Vector* a = ptr<const Vector>(a_word);           // 0045D63A edi = arg1
    const Vector* b = ptr<const Vector>(b_word);           // 0045D66F eax = arg2
    const Vector& p = points[best_index];                  // 0045D641..0045D64B
    Vector d;
    d.x = static_cast<float>(static_cast<double>(position->x) - static_cast<double>(p.x));   // 0045D638..0045D657
    d.y = static_cast<float>(static_cast<double>(position->y) - static_cast<double>(p.y));   // 0045D65F..0045D665
    d.z = static_cast<float>(static_cast<double>(position->z) - static_cast<double>(p.z));   // 0045D669..0045D676
    Vector w;
    w.x = static_cast<float>(static_cast<double>(b->x) - static_cast<double>(a->x));         // 0045D67A..0045D67E
    w.y = static_cast<float>(static_cast<double>(b->y) - static_cast<double>(a->y));         // 0045D682..0045D688
    w.z = static_cast<float>(static_cast<double>(b->z) - static_cast<double>(a->z));         // 0045D68C..0045D698
    Vector cross_tmp;
    const Vector* c = vector_cross_004ab210(&cross_tmp, &d, &w);   // 0045D69C
    Vector norm_tmp;
    const Vector* n = normalize_004ab180(&norm_tmp, c);    // 0045D6A1..0045D6AA
    const Vector nn = *n;                                  // 0045D6AF..0045D6B7
    const float plane_d = static_cast<float>(-(static_cast<double>(nn.x) * static_cast<double>(a->x) +
                                               static_cast<double>(nn.z) * static_cast<double>(a->z) +
                                               static_cast<double>(nn.y) * static_cast<double>(a->y)));   // 0045D6BB..0045D703
    const Plane* planes = field<PlaneList>(self, 0x1F8).data;   // 0045D6D4..0045D6E8 plane 1
    Vector end;
    end.z = static_cast<float>(static_cast<double>(position->z) + static_cast<double>(planes[1].c));   // 0045D707..0045D70E
    end.y = static_cast<float>(static_cast<double>(position->y) + static_cast<double>(planes[1].b));   // 0045D712..0045D715
    end.x = static_cast<float>(static_cast<double>(planes[1].a) + static_cast<double>(position->x));   // 0045D718..0045D71E
    Vector hit;
    const Vector* h = plane_line_intersection_004ab250(&hit, &nn, plane_d, position, &end);   // 0045D72E
    *ptr<Vector>(out_word) = *h;                           // 0045D736..0045D74B
    return 1u;                                             // 0045D74E
}

// 0049F0E0 CAurBehaviorCameraFollow::GetTargetWorldPosition(Vector* out): thiscall RET 4; copies the target
// object's position words [[this+14]+78..+80].
Vector* __fastcall behavior_camera_follow_get_target_world_position_0049f0e0(void* behavior, void*, Vector* out) {
    const U* src = &field<U>(field<void*>(behavior, 0x14), 0x78);   // 0049F0E0..0049F0E7
    U* dst = reinterpret_cast<U*>(out);
    dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];     // 0049F0EB..0049F0FA raw DWORD copies
    return out;                                            // EAX = out (0049F0E3)
}

} // extern "C"
