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
// L3 MATH-TRIG-CPP t1 (2026-10-24): 0045D220 Animate, 0045CF30 MouseDir and 0045D040 MapToScreen are written in the
// PLAN-v3 writing rule (long double register values, explicit (float) stores, math through kotor_math.h). The unit then
// needs the clang LD recipe; that migration of this MSVC unit is NOT ruled (HELD, see the tier INTEGRATION.md).
#include "../../math/kotor_math.h"

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


// L3 t1: field access in the converted functions as direct memory operands (recipe /Ob0: kotor_scene3d::field and fl
// are calls). __forceinline is honoured under /Ob0.
template<class T> __forceinline T& ref(void* o, U off) { return *reinterpret_cast<T*>(static_cast<unsigned char*>(o) + off); }

// L3 t1 a2: Scene::HitCheck (vtbl+38) receives its two by-value Vectors as six stack words, which the original fills
// with integer moves from its fstp dword results (0045CD6C..0045CD97, 0045CEE4..0045CF0F). The slot is called with
// the same stack image typed as words, so each float result is stored once (fstp dword) and copied by mov; a
// float-typed Vector argument made clang hold the six results on the x87 stack and spill them through 4 bytes (S1).
struct VectorWords { U x, y, z; };
using SceneHitCheckWords = I (__fastcall*)(void*, void*, U, U, U, U, U, U, Vector*, void*);
template<class F> __forceinline SceneHitCheckWords slot_as_words(F slot) {   // the slot word itself, retyped
    return reinterpret_cast<SceneHitCheckWords>(reinterpret_cast<std::uintptr_t>(slot));
}
__forceinline U float_bits(float f) { U u; std::memcpy(&u, &f, 4); return u; }
// L3 t1 a2: one clip distance of 0045CC90's perspective ray, as the original computes it for the far clip
// (0045CCCF..0045CD27) and again for the near clip (0045CD2B..0045CD8F):
//   fld clip ; fld dir.x ; fmul st(1) (kept) ; fld dir.y ; fmul st(2) ; fstp dword ty ; fld dir.z ; fmul st(2) ;
//   fstp dword tz ; fadd [esi+7Ch] ; fstp dword p.x ; fstp st(0) ; fld ty ; fadd [esi+80h] ; fstp dword p.y ;
//   fld tz ; fadd [esi+84h] ; fstp dword p.z
// The five float results are written to memory here; held in the caller until the Scene::HitCheck argument stores,
// clang kept six of them on the x87 stack at once and spilled two through 4 bytes (S1).
__declspec(noinline) void ray_point(VectorWords* p, const Vector& dir, long double clip, const Vector& position) {
    const long double tx = static_cast<long double>(dir.x) * clip;
    const long double ty = static_cast<float>(static_cast<long double>(dir.y) * clip);   // 0045CCF0 / 0045CD3D
    const long double tz = static_cast<float>(static_cast<long double>(dir.z) * clip);   // 0045CD02 / 0045CD47
    p->x = float_bits(static_cast<float>(tx + position.x));    // 0045CD09 / 0045CD4E
    p->y = float_bits(static_cast<float>(ty + position.y));    // 0045CD19 / 0045CD5E
    p->z = float_bits(static_cast<float>(tz + position.z));    // 0045CD27 / 0045CD8F
}

// L3 t1 a2: a Vector built from three raw argument words with integer moves (the original copies by-value vector
// arguments with mov, 0045BFE7..0045C019); never through an x87 load/store, which would quiet a signalling NaN.
__forceinline Vector vector_of_words(U x, U y, U z) {
    Vector v;
    std::memcpy(&v.x, &x, 4); std::memcpy(&v.y, &y, 4); std::memcpy(&v.z, &z, 4);
    return v;
}

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

// L3 t1: the x87-trig a2 helpers x87trig_0045d288/0045d2a9/0045d2be/0045cf92/0045d179 are replaced by the plain C++
// of Animate, MouseDir and MapToScreen below.

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
    // L3 t1 a2: raw word copies (was bits_to_float, a call returning ST0 under /Ob0: six 4-byte x87 spills, S1, and a
    // signalling NaN quieted by fld/fstp).
    const Vector start = vector_of_words(sx, sy, sz);      // 0045C000..0045C019 mov (lower pushed vector)
    const Vector end = vector_of_words(ex, ey, ez);        // 0045BFE7..0045BFFD mov (higher pushed vector)
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
    return static_cast<double>(ref<float>(self, 0x1D0));  // 0045C4D0 fld dword [ecx+1D0h] ; ret
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
// L3 t1: PLAN-v3 writing rule (replaces the a2 helpers x87trig_0045d288/2a9/2be).
U __fastcall camera_animate_0045d220(void* self, void*, U delta_bits) {
    aurora_model::Gob* gob = static_cast<aurora_model::Gob*>(at(self, 4));   // 0045D25B / 0045D2D5 / 0045D2F0
    void* rec = ref<void*>(self, 0x1D4);                   // 0045D223
    if (rec == nullptr) return gob_animate_00486670(gob, nullptr, delta_bits);   // 0045D22B -> 0045D2EB
    float delta;                                           // the argument word [esp+8]
    std::memcpy(&delta, &delta_bits, sizeof delta);
    ref<float>(rec, 4) = static_cast<float>(static_cast<long double>(delta) + ref<float>(rec, 4));   // 0045D231..0045D238
    rec = ref<void*>(self, 0x1D4);                         // 0045D23B reload
    if (ref<float>(rec, 4) > ref<float>(rec, 0)) {         // 0045D241..0045D24B fcomp ; test 41h ; jne (not greater: animate)
        live_free(rec);                                    // 0045D24D
        ref<void*>(self, 0x1D4) = nullptr;                 // 0045D25E
        return gob_animate_00486670(gob, nullptr, delta_bits);   // 0045D268
    }
    const U mode = ref<U>(rec, 0x10);                      // 0045D271
    long double t = static_cast<long double>(ref<float>(rec, 4)) / ref<float>(rec, 0);   // 0045D274 / 0045D279 fdiv
    if (mode == 1u) {                                      // 0045D27D..0045D296: 1 - (cos(t*pi) + 1) * 0.5
        t = kOne - (cos(t * kPi) + kOne) * kHalf;
    } else if (mode == 2u) {                               // 0045D29E..0045D2AB: 1 - cos(t*pi/2)
        t = kOne - cos(t * kHalfPi);
    } else if (mode == 3u) {                               // 0045D2B3..0045D2BE: sin(t*pi/2)
        t = sin(t * kHalfPi);
    } else if (mode != 0u) {                               // 0045D2E9 fstp st(0)
        return gob_animate_00486670(gob, nullptr, delta_bits);
    }
    // 0045D2C0..0045D2DA: fld 1 ; fsub st,st(1) ; fmul [rec+8] ; fxch ; fmul [rec+0Ch] ; faddp ; fstp dword [self+1D0h]
    ref<float>(self, 0x1D0) = static_cast<float>((kOne - t) * ref<float>(rec, 8) + t * ref<float>(rec, 0x0C));
    return gob_animate_00486670(gob, nullptr, delta_bits); // 0045D2E0
}

// 0045CF30 Camera::MouseDir(Vector* out, int x, int y) -> out: thiscall RET 0Ch.
// L3 t1: PLAN-v3 writing rule (replaces the a2 helper x87trig_0045cf92). tan(view * pi/360) stays a register value
// for both components, as at 0045CF92..0045CFEF.
U __fastcall camera_mouse_dir_0045cf30(void* self, void*, U out, U x, U y) {
    const Viewport vp = camera_viewport(self);             // 0045CF36..0045CF72
    const I xa = static_cast<I>(x - static_cast<U>(vp.x)); // 0045CF7E..0045CF8C
    const I ya = static_cast<I>(y - static_cast<U>(vp.y)); // 0045CF94..0045CF9C
    // 0045CF78..0045CF92: fld [self+1D0h] ; fmul dword [00741BB4] ; fptan ; (0045CFB7) fstp st(0)
    const long double t = tan(static_cast<long double>(ref<float>(self, 0x1D0)) * kHalfDegToRad);
    const long double half_h = static_cast<long double>(vp.h) * kHalf;                 // 0045CFB9..0045CFBD fild h ; fmul 0.5
    Vector v;
    // 0045CFC3..0045CFDB: fild xa ; fild w ; fmul 0.5 ; fsubp ; fmul st(2) ; fdiv st(1) ; fstp dword [esp+10h]
    v.x = static_cast<float>((static_cast<long double>(xa) - static_cast<long double>(vp.w) * kHalf) * t / half_h);
    // 0045CFDF..0045CFE9: fild ya ; fsub st(1) ; fmul st(2) ; fdiv st(1) ; fstp dword [esp+14h]
    v.y = static_cast<float>((static_cast<long double>(ya) - half_h) * t / half_h);
    v.z = -1.0f;                                           // 0045CFAF mov [esp+18h],0BF800000h
    Vector rotated;
    const Vector* r = quaternion_rotate_004a9bb0(&rotated, &field<Quaternion>(self, 0x88), &v);   // 0045CFF1
    v = *r;                                                // 0045CFF6..0045D00D
    vector_normalize_004ab130(&v, nullptr);                // 0045D011
    *ptr<Vector>(out) = v;                                 // 0045D016..0045D02D
    return out;                                            // EAX = out (0045D016)
}

// 0045D040 Camera::MapToScreen(Vector* out, Vector world, bool normalized) -> out: thiscall RET 14h.
// L3 t1: PLAN-v3 writing rule (replaces the a2 helper x87trig_0045d179).
U __fastcall camera_map_to_screen_0045d040(void* self, void*, U out, U px, U py, U pz, U normalized) {
    const Viewport vp = camera_viewport(self);             // 0045D046..0045D082
    Vector* o = ptr<Vector>(out);
    if (!(ref<float>(self, 0x1D0) > kZero)) {              // 0045D088..0045D099 fcomp ; test 41h ; jne
        o->x = 0.0f; o->y = 0.0f; o->z = 0.0f;             // 0045D1F4..0045D205 (raw zero DWORDs)
        return out;
    }
    float wx, wy, wz;                                      // the argument words [esp+50h..58h]
    std::memcpy(&wx, &px, sizeof wx); std::memcpy(&wy, &py, sizeof wy); std::memcpy(&wz, &pz, sizeof wz);
    Vector d;                                              // 0045D09F..0045D0DE: fld world.N ; fsub [self+7Ch+4N] ; fstp dword
    d.x = static_cast<float>(static_cast<long double>(wx) - ref<float>(self, 0x7C));
    d.y = static_cast<float>(static_cast<long double>(wy) - ref<float>(self, 0x80));
    d.z = static_cast<float>(static_cast<long double>(wz) - ref<float>(self, 0x84));
    Quaternion conj;
    const Quaternion* c = quaternion_negate_w_004a9b50(&conj, &field<Quaternion>(self, 0x88));   // 0045D0EF
    Vector rotated_tmp;
    const Vector* rr = quaternion_rotate_004a9bb0(&rotated_tmp, c, &d);   // 0045D0FD
    const Vector r = *rr;                                  // 0045D102..0045D11C
    const Vector origin{0.0f, 0.0f, 0.0f};                 // 0045D12F..0045D13F
    const Vector normal{0.0f, 0.0f, 1.0f};                 // 0045D147..0045D157
    Vector p;
    plane_line_intersection_004ab250(&p, &normal, 1.0f, &origin, &r);   // 0045D120 push 3F800000h / 0045D15F
    // 0045D164..0045D195: fld [self+1D0h] ; fmul [00741BB4] ; fptan ; fstp st(0) ; fild h ; fst dword [esp+0Ch] (h_f) ;
    // fmul 0.5 ; fst dword [esp+4] (half_h) ; fdiv st,st(1) ; fstp dword [esp+50h] (k) ; fstp st(0)
    const long double t = tan(static_cast<long double>(ref<float>(self, 0x1D0)) * kHalfDegToRad);
    const long double h = static_cast<long double>(vp.h);
    const float h_f = static_cast<float>(h);
    const long double hh = h * kHalf;
    const float half_h = static_cast<float>(hh);
    const float k = static_cast<float>(hh / t);
    const float w_f = static_cast<float>(static_cast<long double>(vp.w));                    // 0045D197..0045D19B fild ; fstp dword
    // 0045D19F..0045D1B3: fld k ; fmul p.x ; fld w_f ; fmul 0.5 ; faddp ; fiadd vp.x
    long double sx = static_cast<long double>(k) * p.x + static_cast<long double>(w_f) * kHalf;
    sx = sx + static_cast<long double>(vp.x);
    // 0045D1B7..0045D1C3: fld k ; fmul p.y ; fiadd vp.y ; fadd half_h
    long double sy = static_cast<long double>(k) * p.y + static_cast<long double>(vp.y);
    sy = sy + half_h;
    if ((normalized & 0xFFu) != 0) {                       // 0045D170 mov al / 0045D177 test / 0045D1C7 je
        sx = sx / w_f;                                     // 0045D1CB fdiv [esp+8]
        sy = kOne - sy / h_f;                              // 0045D1D1..0045D1D5 fdiv [esp+0Ch] ; fsubr 1
    }
    o->x = static_cast<float>(sx);                         // 0045D1E1
    o->z = 0.0f;                                           // 0045D1E3
    o->y = static_cast<float>(sy);                         // 0045D1EB
    return out;
}

// 0045CC90 Camera::HitCheck(int x, int y, Vector* out, CHitInfo* hit): thiscall RET 10h; EAX from Scene vtbl+38.
// L3 t1 a2: PLAN-v3 writing rule. The products and sums are long double register values; every original fstp dword
// is one (float); the position, clip and scale fields are read where the original reads them (after the MouseDir
// call). Was: double locals with the position read before the virtual call (qword/dword spills across it, S1).
U __fastcall camera_hit_check_0045cc90(void* self, void*, U x, U y, U out, U hit) {
    if (ref<void*>(self, 0x6C) == nullptr) return 0u;      // 0045CC96..0045CC9C -> 0045CF1D xor eax,eax
    if (ref<float>(self, 0x1D0) > kZero) {                 // 0045CCA2..0045CCB3 fcomp 0 ; test ah,41h ; jne (not > or NaN: ortho)
        Vector dir;                                        // [esp+8] after the two pushes
        camera_slots(self).s02c_camera_mouse_dir(self, nullptr, word(&dir), x, y);   // 0045CCB9..0045CCCC [vtbl+2C]
        // 0045CCD9..0045CCEF: hit, scene, its vtable and out are fetched before the arithmetic (no call may separate
        // the float results from the Scene::HitCheck call: they would spill through 4 bytes, S1)
        void* scene = ref<void*>(self, 0x6C);              // 0045CCDF reload
        const SceneHitCheckWords scene_hit_check = slot_as_words(scene_slots_of(scene).s038_scene_hit_check);   // 0045CCED mov edx,[ecx]
        Vector* const out_v = ptr<Vector>(out);            // 0045CCE9
        void* const hit_p = ptr<void>(hit);                // 0045CCD9
        // 0045CCCF..0045CD27 (far clip: end) and 0045CD2B..0045CD8F (near clip: start)
        const Vector& position = ref<Vector>(self, 0x7C);
        VectorWords end;
        ray_point(&end, dir, ref<float>(self, 0x1E0), position);
        VectorWords start;
        ray_point(&start, dir, ref<float>(self, 0x1DC), position);
        return static_cast<U>(scene_hit_check(scene, nullptr, start.x, start.y, start.z, end.x, end.y, end.z, out_v, hit_p));   // 0045CD9A call [edx+38h]
    }
    // orthographic path 0045CDA5
    const Viewport vp = camera_viewport(self);             // 0045CDA5..0045CDE5
    const I xa = static_cast<I>(x - static_cast<U>(vp.x)); // 0045CDE9..0045CDF5
    const I ya = static_cast<I>(y - static_cast<U>(vp.y)); // 0045CDF7..0045CDFB
    if (xa < 0 || ya < 0 || xa >= vp.w || ya >= vp.h) return 0u;   // 0045CE07..0045CE21 (signed jl / jge)
    void* scene = ref<void*>(self, 0x6C);                  // 0045CE98
    const SceneHitCheckWords scene_hit_check = slot_as_words(scene_slots_of(scene).s038_scene_hit_check);   // 0045CE9B mov edx,[ecx]
    Vector* const out_v = ptr<Vector>(out);                // 0045CE7E
    void* const hit_p = ptr<void>(hit);                    // 0045CE6F
    // 0045CE27..0045CE63: fild h ; fild xa ; fadd 0.5 ; fdiv st(1) ; fmul [esi+1D8h] ; fstp dword ox ; (ya) ... ; fstp st(0)
    const long double h = static_cast<long double>(vp.h);
    const long double ox = static_cast<float>((static_cast<long double>(xa) + kHalf) / h * ref<float>(self, 0x1D8));   // 0045CE3D
    const long double oy = static_cast<float>((static_cast<long double>(ya) + kHalf) / h * ref<float>(self, 0x1D8));   // 0045CE5B
    const long double neg_near = static_cast<float>(-static_cast<long double>(ref<float>(self, 0x1DC)));   // 0045CE69..0045CE76 fchs ; fstp dword
    const long double nf = neg_near - ref<float>(self, 0x1E0);   // 0045CE7A..0045CE82 (kept)
    VectorWords end;
    end.x = float_bits(static_cast<float>(ox + ref<float>(self, 0x7C)));   // 0045CE8C..0045CE9D
    end.y = float_bits(static_cast<float>(oy + ref<float>(self, 0x80)));   // 0045CEA4..0045CEB3
    end.z = float_bits(static_cast<float>(nf + ref<float>(self, 0x84)));                             // 0045CEB7..0045CEBD
    VectorWords start;
    start.x = float_bits(static_cast<float>(ox + ref<float>(self, 0x7C)));   // 0045CEC1..0045CEC8
    start.y = float_bits(static_cast<float>(oy + ref<float>(self, 0x80)));   // 0045CECC..0045CED6
    start.z = float_bits(static_cast<float>(neg_near + ref<float>(self, 0x84)));   // 0045CEDA..0045CF07
    return static_cast<U>(scene_hit_check(scene, nullptr, start.x, start.y, start.z, end.x, end.y, end.z, out_v, hit_p));   // 0045CF12 call [edx+38h]
}

// 0045D490 Camera::SceneBoundaryCheck(const Vector* a, const Vector* b, const Plane* bound, Vector* out):
// thiscall RET 10h, EAX 0/1.
// L3 t1 a2: PLAN-v3 writing rule. Sums and products are long double register values, each original fstp dword is
// one (float); the running maximum is the float the original stores at 0045D5F0, held as a long double so it never
// spills through 4 bytes across the calls (was a float local: 4-byte x87 spill, S1; and double locals: qword spills).
U __fastcall camera_scene_boundary_check_0045d490(void* self, void*, U a_word, U b_word, U bound_word, U out_word) {
    const Plane ground{0.0f, 0.0f, 1.0f, 0.0f};            // 0045D496..0045D4D1 locals (0,0,1) + d 0
    const I partner[4] = {4, 5, 3, 2};                     // 0045D4D9..0045D4ED
    Vector points[4] = {};                                 // 0045D4F5..0045D50C zeroed
    const Plane* bound = ptr<const Plane>(bound_word);     // 0045D50F ebx = arg3
    const Vector* position = &ref<Vector>(self, 0x7C);     // 0045D527 esi = this+7C
    I best_index = 0;                                      // 0045D517
    long double best = kZero;                              // 0045D51F mov dword [esp+20h],0
    // L3 t1 a2: the original is a loop (0045D538..0045D60E); unrolled, its four in-loop stores would each appear
    // four times in the listing instead of once per original store site.
#pragma clang loop unroll(disable)
    for (I i = 0; i < 4; ++i) {                            // 0045D531 ebp = 20h .. 60h step 10h
        const Plane* planes = ref<PlaneList>(self, 0x1F8).data;   // 0045D540 reload
        Vector cross_tmp;
        const Vector* c = vector_cross_004ab210(&cross_tmp, reinterpret_cast<const Vector*>(&planes[2 + i]),
                                                reinterpret_cast<const Vector*>(&planes[partner[i]]));   // 0045D546..0045D55F
        const Vector cr = *c;                              // 0045D564..0045D58E (mov)
        Vector end;
        end.x = static_cast<float>(static_cast<long double>(cr.x) + position->x);   // 0045D56D..0045D57E
        end.y = static_cast<float>(static_cast<long double>(cr.y) + position->y);   // 0045D586..0045D594
        end.z = static_cast<float>(static_cast<long double>(cr.z) + position->z);   // 0045D598..0045D5AC
        Vector hit;
        const Vector* h = plane_line_intersection_004ab250(&hit, reinterpret_cast<const Vector*>(&ground), ground.d,
                                                           position, &end);   // 0045D5B0
        points[i] = *h;                                    // 0045D5B5..0045D5C4 (mov)
        // 0045D5C7..0045D5DE: fld p.z ; fmul c ; fld a ; fmul p.x ; faddp ; fld p.y ; fmul b ; faddp ; fadd d
        const long double dist = (static_cast<long double>(points[i].z) * bound->c +
                                  static_cast<long double>(bound->a) * points[i].x) +
                                 static_cast<long double>(points[i].y) * bound->b + bound->d;
        if (dist > best) {                                 // 0045D5E1..0045D5EA fcom dword ; test ah,41h ; jne (not > or NaN)
            best = static_cast<float>(dist);               // 0045D5F0 fstp dword [esp+20h]
            best_index = i;                                // 0045D5F4
        }
    }
    if (best == kZero) return 0u;                          // 0045D614..0045D631 fcomp 0 ; test ah,44h ; jp (equal: 0)
    const Vector* a = ptr<const Vector>(a_word);           // 0045D63A edi = arg1
    const Vector* b = ptr<const Vector>(b_word);           // 0045D66F eax = arg2
    const Vector& p = points[best_index];                  // 0045D641..0045D64B
    Vector d;
    d.x = static_cast<float>(static_cast<long double>(position->x) - p.x);   // 0045D638..0045D657
    d.y = static_cast<float>(static_cast<long double>(position->y) - p.y);   // 0045D65F..0045D665
    d.z = static_cast<float>(static_cast<long double>(position->z) - p.z);   // 0045D669..0045D676
    Vector w;
    w.x = static_cast<float>(static_cast<long double>(b->x) - a->x);         // 0045D67A..0045D67E
    w.y = static_cast<float>(static_cast<long double>(b->y) - a->y);         // 0045D682..0045D688
    w.z = static_cast<float>(static_cast<long double>(b->z) - a->z);         // 0045D68C..0045D698
    Vector cross_tmp;
    const Vector* c = vector_cross_004ab210(&cross_tmp, &d, &w);   // 0045D69C
    Vector norm_tmp;
    const Vector* n = normalize_004ab180(&norm_tmp, c);    // 0045D6A1..0045D6AA
    const Vector nn = *n;                                  // 0045D6AF..0045D6EE (mov)
    // 0045D6BB..0045D703: fld n.x ; fmul a.x ; fld n.z ; fmul a.z ; faddp ; fld n.y ; fmul a.y ; faddp ; fchs ; fstp dword
    const float plane_d = static_cast<float>(-((static_cast<long double>(nn.x) * a->x +
                                                static_cast<long double>(nn.z) * a->z) +
                                               static_cast<long double>(nn.y) * a->y));
    const Plane* planes = ref<PlaneList>(self, 0x1F8).data;   // 0045D6D4..0045D6E8 plane 1
    Vector end;
    end.z = static_cast<float>(static_cast<long double>(position->z) + planes[1].c);   // 0045D707..0045D70E
    end.y = static_cast<float>(static_cast<long double>(position->y) + planes[1].b);   // 0045D712..0045D715
    end.x = static_cast<float>(static_cast<long double>(planes[1].a) + position->x);   // 0045D718..0045D71E
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
