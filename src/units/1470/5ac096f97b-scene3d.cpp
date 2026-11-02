// lane-3d-render: faithful bodies for the main-menu 3D scene path (construction + per-frame top).
// Evidence: dumpbin /disasm of swkotor.exe (GOG K1, sha256 9c10e045...); instruction addresses cited per behavior.
// opengl32 entries are called through the real imports (original `call dword ptr [IAT]`); glEnable reuses the
// M40 adapter kotor_renderer_init::glEnable_0073d390; gluPerspective comes from glu32 (DECISIONS D10).
// Floating point: the original evaluates on the x87 stack; values the original rounds through a `fstp dword`
// are rounded to float here; register intermediates use long double. Byte equality is not claimed.
// kotor_math.h comes first: MSVC <new> and <cstdlib> can include <math.h>,
// which kotor_math.h rejects when it comes first; <math.h> itself declares no long double overload of the standard
// names (those are in <cmath>, never included), so the later inclusion is harmless (ldgate R2/R3 check the object).
#include "../../math/kotor_math.h"
#include "../../math/kotor_x87.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
// L3 MATH-TRIG-CPP t1 (2026-10-24): 0045C540 RenderScene, 00423C40 Quaternion::axis and 00456300
// DoHighlightTriggers are written in the PLAN-v3 writing rule. kotor_math.h replaces <cmath> (an LD unit must not
// include it); kotor_x87.h provides FloatToInt for the original's frndint helper (rule 6.12). The unit then needs
// the reviewed clang long-double recipe recorded in the build manifest.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "6ce15fd005-crt_acos_scene_adapter.h"
#include <utility>
#include <intrin.h>   // _AddressOfReturnAddress (L3 t1 a2: by-value Vector arguments read as raw stack words)
#include <string.h>
#include "aac3dc8237-scene3d.h"
#include "16ea3f255d-render_pool.h"   // lane-renderer-bodies: RequestPoolManager (007A68A4 slot+10 GetPool), D32

namespace {
using namespace kotor_scene3d;

template<class Fn> inline Fn slot(void* object, unsigned offset) {
    void** table = *static_cast<void** volatile*>(object);
    return reinterpret_cast<Fn>(table[offset / 4]);
}
using SlotVoid = void (__fastcall*)(void*, void*);
using SlotU = void (__fastcall*)(void*, void*, U);
using SlotPtr = void (__fastcall*)(void*, void*, void*);
using SlotFive = void (__fastcall*)(void*, void*, U, U, U, U, U);
using SlotClip = void (__fastcall*)(void*, void*, float, float);
using SlotAttachScene = I (__fastcall*)(void*, void*, void*);
using SlotDoEvent = void (__fastcall*)(void*, void*, const char*, void*);   // Gob vtbl+16C DoEvent(name, data)
using SlotGetPart = void* (__fastcall*)(void*, void*, const char*);
using SlotPartTransform = void (__fastcall*)(void*, void*, void*, void*);
using SlotSetPosition = Vector* (__fastcall*)(void*, void*, Vector*, Vector);
using SlotBool = bool (__fastcall*)(void*, void*);
using SlotHitCheck = I (__fastcall*)(void*, void*, Vector, Vector, Vector*, void*);   // Scene vtbl+38 HitCheck (RET 20h)
constexpr U kFogModes_0073f25c[3] = {0x2601, 0x0800, 0x0801};          // GL_LINEAR, GL_EXP, GL_EXP2
constexpr U kMeshResetValue_00740a70 = 4;                            // .rdata 00740A70
using SlotI = I (__fastcall*)(void*, void*);
using SlotDelete = U (__fastcall*)(void*, void*, U);   // scalar deleting destructor slot +00
constexpr U kRadToDeg_00742ef8 = 0x42652EE1;                         // 57.29578f
constexpr U kGlOne_0073f278 = 1;                                     // GL_ONE
constexpr U kGlModelview_0073f294 = 0x1700;                          // .rdata 0073F294 (same value as 0073F298)
constexpr U kByte_0073ede4 = 0x437F0000;                             // 255.0f
constexpr U kFadeOff_00741568 = 1, kFadeOn_0074156c = 2;            // .rdata words stored to Scene+B8
constexpr double kFadeMinimum_00742ef0 = 0.4;                         // qword 3FD999999999999A
constexpr U kPrimitiveModes_0073f334[10] = {0, 1, 3, 4, 5, 6, 7, 8, 9, 2};   // .rdata 0073F334 (glBegin modes)
constexpr U kSphereA_0073f4ec = 0x3F378D50, kSphereB_0073f4e8 = 0xBF378D50;   // +-0.717f
constexpr U kShadowLift_007415c4 = 0x3FE00000;                       // 1.75f
constexpr U kGlCw_0073f2c0 = 0x0900, kGlCcw_0073f2c4 = 0x0901;
constexpr U kGlIncr_0073f2d4 = 0x1E02, kGlDecr_0073f2d8 = 0x1E03;
// (00740C88 6.2831855f: kRsTwoPi_00740c88 below, L3 t1)
constexpr U kGlPolygon_0073f34c = 7;                                  // .rdata 0073F34C (GL_QUADS)
using BehaviorCtor = void* (__fastcall*)(void*, void*, void*, void*, void*);

// .rdata constants read by the original through memory operands (values from the pinned image).
constexpr U kGlModelview_0073f298 = 0x1700;           // GL_MODELVIEW
constexpr U kGlProjection_0073f290 = 0x1701;          // GL_PROJECTION
constexpr U kGlFrontAndBack_0073f2f4 = 0x0408;        // GL_FRONT_AND_BACK
constexpr U kPolygonModes_0073f2e4[2] = {0x1B01, 0x1B02}; // GL_LINE, GL_FILL
constexpr U kGlLequal_0073f2ac = 0x0203;              // glDepthFunc arg
constexpr U kGlGreater_0073f2b0 = 0x0204;             // glAlphaFunc func
constexpr U kGlNormalArray_0073f39c = 0x8075;
constexpr U kGlTexCoordArray_0073f398 = 0x8078;
constexpr U kGlVertexArray_0073f3a4 = 0x8074;
// (0073DFEC 0.01f: kRsOrthoScale_0073dfec below, L3 t1)
constexpr U kHalf_0073e9ac = 0x3F000000;              // 0.5f
constexpr U kDegToRad_007415f8 = 0x3C8EFA35;          // 0.0174533f
constexpr U kMilli_0073d708 = 0x3A83126F;             // 0.001f
constexpr U kRadToNegDeg_0073f3f8 = 0xC2652EE1;       // -57.29578f
constexpr U kMinusOne_00740cbc = 0xBF800000;          // -1.0f
constexpr U kOne = 0x3F800000;                        // 1.0f (0073D764 and immediates)
constexpr U kMinusOneImm = 0xBF800000;
constexpr U kFloatZero_0073d700 = 0u;                 // .rdata 0073D700 0.0f
constexpr double kAxisEpsilon_0073f3d8 = 1e-07;       // qword 3E7AD7F29ABCAF48

// L3 t1 a2: the D30-FINAL helpers as the PLAN-v3 writing rule (every FP function of this unit is converted by the LD
// recipe): W = the value on the x87 stack (long double, 80-bit register), S = an original fstp dword point, f = an
// image constant by its bit pattern. Force-inlined: under the /Ob0 recipe a plain inline helper is a call, which
// forces the caller's live x87 values out of the registers.
__forceinline float f(U bits) { float v; std::memcpy(&v, &bits, 4); return v; }
__forceinline long double W(float v) { return v; }
// L3 t1 a2: a float cell or raw float word read as the original does (fld dword [cell]); kotor_scene3d::bits_to_float is a
// call returning ST0 under the /Ob0 recipe.
__forceinline long double Wcell(const volatile U* cell) { return *reinterpret_cast<const volatile float*>(cell); }
__forceinline float fbits(U bits) { float v; std::memcpy(&v, &bits, 4); return v; }
__forceinline float S(long double v) { return static_cast<float>(v); }
// L3 t1: field access in the converted functions as direct memory operands (/Ob0 makes field/f calls), and the image
// constants they use as float / double values of the original's operand width.
template<class T> __forceinline T& ref(void* o, U off) { return *reinterpret_cast<T*>(static_cast<unsigned char*>(o) + off); }
// L3 t1 a2: every function of the unit reads its fields through ref / atf (force-inlined; kotor_scene3d::field / at are
// calls under the /Ob0 recipe and forced the callers' live x87 values out of the registers: S1).
__forceinline void* atf(void* o, U off) { return static_cast<unsigned char*>(o) + off; }
constexpr float kRsOrthoScale_0073dfec = 0.00999999977648258209228515625f;   // 3C23D70A
constexpr float kRsHalf_0073e9ac = 0.5f;                                     // 3F000000
constexpr float kRsOne_0073d764 = 1.0f;                                      // 3F800000
constexpr float kRsTwoPi_00740c88 = 6.283185482025146484375f;                // 40C90FDB
constexpr double kRsHalfD_0073f3d0 = 0.5;                                    // qword 3FE0000000000000
constexpr double kRsOneD_0073ede8 = 1.0;                                     // qword 3FF0000000000000
inline void set_bits(void* object, U offset, U bits) { field<U>(object, offset) = bits; }
// D41/D42: Scene and Gob dispatch goes through the published typed tables (vtable pointer reloaded per call).
inline const SceneVTable* scene_vt(void* scene) { return *static_cast<const SceneVTable* const volatile*>(scene); }
inline const aurora_model::GobVTable* gob_vt(void* gob) { return *static_cast<const aurora_model::GobVTable* const volatile*>(gob); }
inline aurora_model::Gob* as_gob(void* gob) { return static_cast<aurora_model::Gob*>(gob); }
inline const CameraVTable* camera_vt(void* camera) { return *static_cast<const CameraVTable* const volatile*>(camera); }
inline const GuiSceneVTable* gui_scene_vt(void* gui_scene) { return *static_cast<const GuiSceneVTable* const volatile*>(gui_scene); }
inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
// L3 t1 a2: by-value Vector arguments. The original copies them with integer moves (mov). clang's __fastcall /
// __thiscall ABI expands a by-value Vector into three float parameters, loaded with fld dword (quiets a signalling NaN)
// and held on the x87 stack across calls (4-byte spills, S1). A callee therefore reads the words from its own stack
// argument slots (A2_STACK_ARGS()[0] is the first stack argument, the word above the return address), and a caller
// calls the same entry through a word-typed pointer (same stack image, same `ret N`), as anim_contested 00438AF0 /
// core46_camera 0045CC90.
#define A2_STACK_ARGS() (reinterpret_cast<U*>(static_cast<unsigned char*>(_AddressOfReturnAddress()) + 4))
// The pointer is read back through a volatile local: a callee of THIS object whose parameters are unnamed (it reads its
// argument words itself) would otherwise let the optimizer see unused parameters and pass no argument words at all
// (measured: 0044F470 -> 00437BA0 and 0045C3F0 -> 0045C030 pushed nothing). The call is then an indirect call.
template<class To, class From> __forceinline To words_form(From f) { To volatile p = reinterpret_cast<To>(reinterpret_cast<std::uintptr_t>(f)); return p; }
template<class F> __forceinline F opaque(F f) { F volatile p = f; return p; }
// raw(): a word read that the compiler cannot merge with a float load of the same address (GVN forwarded such a value
// and stored it with fld/fstp: 00450FA0's +108h radius).
__forceinline U raw(const void* p) { return *static_cast<const volatile U*>(p); }
__forceinline void copy_words3(void* dst, const void* src) {
    const volatile U* s = static_cast<const volatile U*>(src); U* d = static_cast<U*>(dst);
    const U x = s[0], y = s[1], z = s[2];
    d[0] = x; d[1] = y; d[2] = z;
}
// One original `fstp dword` of a register value whose float is then only copied with integer moves.
__forceinline U stored_bits(long double v) { const float s = static_cast<float>(v); U u; std::memcpy(&u, &s, 4); return u; }
using MatrixWords = float* (__fastcall*)(float*, void*, U, U, U, U, U, U, U, U, U);          // 00437BA0, RET 24h
using SetPositionWords = Vector* (__fastcall*)(void*, void*, Vector*, U, U, U);             // 0043F0C0 / 0045C030, RET 10h
using HitCheckWords = I (__fastcall*)(void*, void*, U, U, U, U, U, U, Vector*, void*);      // 0043EB80 / 00456B60 / 0044FE00, RET 20h
using BspHitWords = U (__cdecl*)(void*, U, U, U, U, U, U, Vector*, U (__cdecl*)(void*, Vector, Vector, Vector*));   // 004AD260
using PartHitWords = U (__cdecl*)(void*, U, U, U, U, U, U, Vector*);                         // 0046B4C0
// OpenGL entries whose float arguments the original pushes as raw words (mov / push): called through word-typed
// pointers to the same import, so no fld/fstp copy quiets a signalling NaN.
using GlWords3 = void (__stdcall*)(U, U, U);                 // glVertex3f / glColor3f / glTranslatef / glScalef / glNormal3f
using GlWords2 = void (__stdcall*)(U, U);                    // glTexCoord2f
using GlWords4 = void (__stdcall*)(U, U, U, U);              // glRotatef / glColor4f / glVertex4f
// D52/D30-FINAL: x87 transcendental instructions of the original, executed on the x87 unit (D21 inline asm).
// lane x87-trig (2026-10-24): the fptan (0045C72B/0045C74D/0045C76D/0045C78B) and fsin (004567B3) results are
// consumed by further x87 instructions in the original with their full 64-bit significand; the former x87_tan /
// x87_sin returned them rounded to double. Those sites now use the x87trig_* sequences below (after this
// namespace). 00423C9F (fsin) is the scene_acos::axis_scale sequence, unchanged.
// L3 t1: the x87trig_* helpers and their memory-operand constants are replaced by plain C++ (RenderScene, DoHighlightTriggers).
// frndint; fistp dword under the original CW (RC nearest-even) - D30-FINAL explicit round-to-nearest port.
// L3 t1: rule 6.12 - the original's frndint sites are the one FloatToInt helper (kotor_x87.h, the original asm body).
__forceinline I x87_round(float value) {
    int result;
    FloatToInt(&result, value);
    return result;
}
inline void scene_child_detach_attach(void* gob, void* scene) {
    // Shared tail of Gob::SetScene 0043F203..0043F239 and Gob::AttachToObject 00443934..00443968.
    void* previous = ref<void*>(gob, 0x68);
    if (previous == scene) return;
    if (previous != nullptr) scene_vt(previous)->s0b4_scene_child_remove(previous, nullptr, gob);   // Scene::ChildRemove
    ref<void*>(gob, 0x68) = scene;
    if (scene != nullptr) scene_vt(scene)->s0b0_scene_child_add(scene, nullptr, gob);             // Scene::ChildAdd
    gob_vt(gob)->do_event(as_gob(gob), nullptr, "UpdateScenes", 0);                     // 00740AD8, Gob::DoEvent
}
} // namespace


// 0045F640 AurATIValidate: provided by lane-upload (D52); declaration in scene3d.h.

extern "C" {

// ================================================================ GUI glue
// 0067ACB0 CSWGui3DSceneView vtbl+38: mov eax,[ecx+5C]; add ecx,5Ch; jmp [eax+0Ch] (CSWGuiScene::Draw, RET 4).
void __fastcall gui_3d_scene_view_draw_0067acb0(void* view, void*, U delta_bits) {
    void* gui_scene = atf(view, 0x5C);
    gui_scene_vt(gui_scene)->s00c_gui_scene_draw(gui_scene, nullptr, delta_bits);
}

// 00414FB0 CSWGuiScene::Draw (CSWGuiScene vtable 0073E3F0 slot+0C)
void __fastcall gui_scene_draw_00414fb0(void* gui_scene, void*, U delta_bits) {
    void* scene = ref<void*>(gui_scene, 0x14);
    scene_vt(scene)->s01c_scene_animate(scene, nullptr, delta_bits);              // 00414FBD Scene::Animate(delta)
    const U accepted = kotor_AurGUISetupViewport_004592F0(             // 00414FDB
        ref<U>(gui_scene, 0x04), ref<U>(gui_scene, 0x08), ref<U>(gui_scene, 0x0C), ref<U>(gui_scene, 0x10),
        atf(gui_scene, 0x28), 1u, kOne);                                // push 3F800000h; push 1; lea eax,[esi+28h]
    if (accepted == 0) return;                                         // 00414FE5
    void* camera = ref<void*>(gui_scene, 0x18);
    camera_vt(camera)->s008_camera_render_scene(camera, nullptr);       // 00414FEC Camera::RenderScene
    kotor_AurGUICloseViewport_00459580();                              // 00414FEF
}

// 00414F80 CSWGuiDrawable::~CSWGuiDrawable (reached here only as CSWGuiScene ctor unwind state 0)
void __fastcall gui_drawable_destruct_00414f80(void* object, void*) {
    ref<void* const*>(object, 0) = gui_drawable_vtable_0073e324;     // mov [ecx],73E324h
}

// 00416770 CExoArrayList::CExoArrayList(size)
PointerList* __fastcall exo_array_list_construct_00416770(PointerList* list, void*, U size) {
    list->size = 0;                                                    // 0041677B [esi+4]
    list->capacity = 0;                                                // 0041677E [esi+8]
    list->data = 0;                                                    // 00416781 [esi]
    if (size != 0) menu_list_growth::allocate_005e03c0(list, nullptr, size); // 00416788
    return list;
}

// 0056B6F0 CExoArrayList::~CExoArrayList: free(data) unconditionally, then data=0.
void __fastcall exo_array_list_destruct_0056b6f0(PointerList* list, void*) {
    live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(list->data)));   // 0056B6F6
    list->data = 0;                                                    // 0056B6FE
}

// 0047ABC0 CExoArrayList::~CExoArrayList variant: free(data) only when non-null; data left as is.
void __fastcall exo_array_list_free_0047abc0(void* list, void*) {
    void* data = ref<void*>(list, 0);
    if (data != nullptr) live_free(data);                             // 0047ABC4 / 0047ABC7
}

// 004174B0 CSWGuiScene::CSWGuiScene (member of CSWGuiMainMenu at +3BC via 0067C548; embedded at view+5C)
void* __fastcall gui_scene_construct_004174b0(void* gui_scene, void*) {
    ref<U>(gui_scene, 0x10) = 0;                                     // 004174D0
    ref<U>(gui_scene, 0x0C) = 0;
    ref<U>(gui_scene, 0x08) = 0;
    ref<U>(gui_scene, 0x04) = 0;
    ref<const GuiSceneVTable*>(gui_scene, 0) = &gui_scene_vtable_0073e3f0;    // 004174E4 (EH state 0 already set at 004174E0)
    PointerList* models = &ref<PointerList>(gui_scene, 0x1C);
    // EH (FuncInfo 00761454): state 0 -> 00414F80(this), state 1 -> 0056B6F0(this+1C). See EH note in NOTES.md.
    {
        exo_array_list_construct_00416770(models, nullptr, 0);         // 004174EA
        {                                                              // EH state 1 (004174F4)
            ref<U>(gui_scene, 0x28) = 0;                             // 004174F9..004174FF
            ref<U>(gui_scene, 0x2C) = 0;
            ref<U>(gui_scene, 0x30) = 0;
            ref<void*>(gui_scene, 0x14) = new_aur_scene_00458e70("scene");      // 00417502 (0073E460)
            ref<void*>(gui_scene, 0x18) = new_aur_camera_0045d930("camera");    // 0041750F (0073E458)
            void* scene = ref<void*>(gui_scene, 0x14);
            if (scene == nullptr || ref<void*>(gui_scene, 0x18) == nullptr) return gui_scene; // 0041751F / 00417523
            scene_vt(scene)->s020_void_return_14(scene, nullptr, 0x0Au, 0x0Au, 5u, 1u, 0x41200000u);     // 00417534 (void_return_14)
            scene = ref<void*>(gui_scene, 0x14);
            scene_vt(scene)->s08c_scene_disable_visibility_graph(scene, nullptr);               // 0041753C DisableVisibilityGraph
            scene = ref<void*>(gui_scene, 0x14);
            scene_vt(scene)->s004_scene_set_fog(scene, nullptr, 0u);              // 00417548 SetFog(0)
            void* camera = ref<void*>(gui_scene, 0x18);
            camera_vt(camera)->s040_camera_set_clip_dist(camera, nullptr, f(0x3DCCCCCDu), f(0x461C4000u)); // 0041755A SetClipDist(0.1,10000)
            camera = ref<void*>(gui_scene, 0x18);
            (void)camera_vt(camera)->s014_camera_attach_to_scene(camera, nullptr, ref<void*>(gui_scene, 0x14)); // 00417566 AttachToScene
            ref<U>(gui_scene, 0x28) = cells.gui_scene_color_0078d3d8[0];  // 00417569
            ref<U>(gui_scene, 0x2C) = cells.gui_scene_color_0078d3d8[1];  // 00417571
            ref<U>(gui_scene, 0x30) = cells.gui_scene_color_0078d3d8[2];  // 0041757A
        }
    }
    return gui_scene;                                                  // 00417588 mov eax,esi
}

// ================================================================ Scene
// 00458E70 NewCAurScene(name): new(2C4h) + Scene::Scene; unwind 00716930 frees the block.
void* __cdecl new_aur_scene_00458e70(const char* name) {
    void* memory = live_operator_new(kSceneSize);                      // 00458E8B
    if (memory == nullptr) return nullptr;                             // 00458EA1 -> 00458EC2 xor eax,eax
    return scene_construct_00458790(memory, nullptr, name);           // 00458EAA (unwind 00716930: free(memory))
}

// 00458790 Scene::Scene(name)
void* __fastcall scene_construct_00458790(void* scene, void*, const char* name) {
    ref<U>(scene, 0x08) = 0; ref<U>(scene, 0x0C) = 0; ref<U>(scene, 0x04) = 0;  // 004587B4..004587BA
    ref<const SceneVTable*>(scene, 0) = &scene_vtable_00741708;            // 004587BD
    // members zeroed in original order (004587CA..00458848)
    static const U zeroed[] = {0x58, 0x5C, 0x54, 0x74, 0x78, 0x7C, 0x88, 0x8C, 0x90, 0x98, 0x9C, 0x94, 0xA4, 0xA8,
                               0xAC, 0xB0, 0xC0, 0xC4, 0xBC, 0xCC, 0xD0, 0xC8, 0xF0, 0xF4, 0xEC};
    for (U offset : zeroed) ref<U>(scene, offset) = 0;
    // EH (FuncInfo 00763434): states 6..0 free lists EC/C8/BC, SafePointer A4, lists 94/54, base 00457990;
    // state 7 adds Gob dtor 0044B2F0(this+F8); 8 = MaxTree delete 0044AEC0(model); 9/10 = free. See NOTES.md.
    gob_construct_00449440(static_cast<aurora_model::Gob*>(atf(scene, 0xF8)), nullptr, "c_FocusGob");   // 00458861 (007416FC), EH state 6
    {                                                                  // EH state 7 (00458872)
        char unique[64];                                               // [esp+44h], 64 bytes up to the frame top
        char* scene_name = &ref<char>(scene, 0x10);
        scene_name[0] = 0;                                             // 0045887A mov byte [ebp],bl
        if (name != nullptr) {                                         // 0045886D
            std::sprintf(unique, "%s", name);                          // 0045888A (0073D5D0)
            I suffix = 0;                                              // 00458892
            while (find_scene_0044fd60(unique) != nullptr || find_gob_00440460(unique) != nullptr) { // 0045889B / 004588AC
                ++suffix;                                              // 004588BC
                std::sprintf(unique, "%s%d", name, suffix);           // 004588CD (0073EDD0)
            }
            std::strcpy(scene_name, unique);                           // 004588D7..004588E9
        }
        char dummy[32];                                                // [esp+24h]
        std::sprintf(dummy, "c_FocusGobDummyModel%d", *cells.focus_counter_007b92d8); // 004588FC (007416E4)
        *cells.focus_counter_007b92d8 = *cells.focus_counter_007b92d8 + 1;          // 0045890D
        void* model_memory = live_operator_new(0xC4);                  // 00458913 -> 0043E1B0 jmp 006FA7E6
        void* model = nullptr;
        if (model_memory != nullptr) {
            model = model_construct_0044aa70(static_cast<aurora_model::MaxTree*>(model_memory), nullptr, dummy);         // 00458932, EH state 8 (00716877)
        }
        ref<void*>(scene, 0x150) = model;                            // 0045894C (Gob+58)
        (void)model_add_ref_0043e240(static_cast<aurora_model::MaxTree*>(model), nullptr, atf(scene, 0xF8));       // 00458952 (ECX may be null, as original)
        set_bits(scene, 0x88, 0); set_bits(scene, 0x8C, 0); set_bits(scene, 0x90, 0);      // 0045896F..00458993
        set_bits(scene, 0x74, kOne); set_bits(scene, 0x78, kOne); set_bits(scene, 0x7C, kOne); // 004589A5..004589C2
        ref<U>(scene, 0x60) = 0;                                     // 004589BC
        ref<U>(scene, 0x6C) = 0;                                     // 004589BF
        set_bits(scene, 0x64, 0x40A00000u);                            // 5.0f
        set_bits(scene, 0x68, 0x42200000u);                            // 40.0f
        set_bits(scene, 0x70, 0x3DCCCCCDu);                            // 0.1f
        ref<U>(scene, 0x50) = 0;                                     // 004589DA
        void* lights_memory = live_operator_new(0xF8);                 // 004589DD
        void* lights = nullptr;
        if (lights_memory != nullptr) {
            lights = light_manager_construct_004a38b0(lights_memory, nullptr);       // 004589F7, EH state 9 (00716882)
        }
        ref<void*>(scene, 0x80) = lights;                            // 00458A0D
        void* wind_memory = live_operator_new(0x858);                  // 00458A13
        void* wind = nullptr;
        if (wind_memory != nullptr) {
            wind = wind_manager_construct_004a6640(wind_memory, nullptr);            // 00458A2D, EH state 10 (0071688D)
        }
        ref<void*>(scene, 0x84) = wind;                              // 00458A3E
        safe_pointer_assign_00447fa0(atf(scene, 0xA4), nullptr, aur_texture_get_normalization_cube_map_004233e0()); // 00458A44/00458A50
        ref<U16>(scene, 0xD8) = 1;                                   // 00458A55
        ref<U>(scene, 0xB4) = 0;                                     // 00458A5E
        PointerList* scenes = cells.scene_list_007b9398;
        if (scenes->size == scenes->capacity) {                       // 00458A64..00458A6F
            const I capacity = scenes->capacity;
            menu_list_growth::allocate_005e03c0(scenes, nullptr, static_cast<U>(capacity != 0 ? capacity + capacity : 8)); // 00458A84
        }
        reinterpret_cast<void**>(static_cast<std::uintptr_t>(scenes->data))[scenes->size] = scene; // 00458A94
        scenes->size = scenes->size + 1;                              // 00458A97
        ref<U>(scene, 0xA0) = 0;                                     // 00458AA0
        ref<U8>(scene, 0xDA) = 1;                                    // 00458AA6
        ref<U8>(scene, 0xDB) = 0;                                    // 00458AAD
        ref<U>(scene, 0xDC) = 0;                                     // 00458AB3
        set_bits(scene, 0xE0, kOne);                                   // 00458AB9
        ref<U>(scene, 0xC0) = 0;                                     // 00458AC3
        gob_set_scene_0043f200(atf(scene, 0xF8), nullptr, scene);       // 00458AC9
        ref<U>(scene, 0xD4) = 0;                                     // 00458AD4
        ref<void*>(scene, 0xE4) = aur_texture_get_reference_00423a60("filmnoisetex", 0);  // 00458ADA (007416D4)
        ref<void*>(scene, 0xE8) = aur_texture_get_reference_00423a60("distortiontex", 0); // 00458AEB (007416C4)
        if (aur_ati_fragment_shader_available_0045f560() != 0)       // 00458AF9
            vertex_program_manager_reset_004a2470();                   // 00458B02
    }
    return scene;                                                      // 00458B0F
}

// 00458B30 Scene::SetFog (vtbl+04)
void __fastcall scene_set_fog_00458b30(void* scene, void*, U fog) { field<U>(scene, 0x60) = fog; }

// 0044F800 Scene::DisableVisibilityGraph (vtbl+8C): mov word [ecx+D8h],1; ret (EAX untouched)
void __fastcall scene_disable_visibility_graph_0044f800(void* scene, void*) { field<U16>(scene, 0xD8) = 1; }

// 005C8650 void_return_14 (Scene vtbl+20 among others): ret 14h
void __fastcall void_return_14_005c8650(void*, void*, U, U, U, U, U) {}

// 00451C80 Scene::Animate (vtbl+1C)
void __fastcall scene_animate_00451c80(void* scene, void*, U delta_bits) {
    *cells.animate_start_007b9298 = query_timer_0044f320();            // 00451C85..00451C96
    *cells.active_scene_007b92a0 = scene;                              // 00451C9C
    if (*cells.animations_0078e3b8 != 0 && *cells.interleaved_007b92f4 != 0) {   // 00451CA2 / 00451CAB
        for (I index = ref<I>(scene, 0xF0); index != 0;) {          // 00451CAD..00451CDC
            --index;
            if (index < ref<I>(scene, 0xF0)) {                       // cmp edi,eax; jge (signed)
                void* object = ref<void**>(scene, 0xEC)[index];
                slot<SlotU>(object, 0x14)(object, nullptr, delta_bits);
            }
        }
        for (I index = ref<I>(scene, 0x58); index != 0;) {          // 00451CDE..00451D02
            --index;
            if (index < ref<I>(scene, 0x58)) {
                void* object = ref<void**>(scene, 0x54)[index];
                if (ref<U8>(object, 0x16) == 0) (void)gob_vt(object)->animate(as_gob(object), nullptr, delta_bits); // 00451CF3
            }
        }
    } else {
        for (I index = ref<I>(scene, 0x58); index != 0;) {          // 00451D06..00451D27
            --index;
            if (index < ref<I>(scene, 0x58)) {
                void* object = ref<void**>(scene, 0x54)[index];
                (void)gob_vt(object)->animate(as_gob(object), nullptr, delta_bits);
            }
        }
    }
    *cells.active_scene_007b92a0 = nullptr;                            // 00451D29
    const I64 end = query_timer_0044f320();                            // 00451D33
    const U start_low = static_cast<U>(*cells.animate_start_007b9298); // 00451D38 mov ecx,[007B9298]
    *cells.animate_end_007b9288 = end;                                 // 00451D3E / 00451D4F
    const I elapsed = static_cast<I>(static_cast<U>(end) - start_low); // sub eax,ecx; fild dword
    *cells.animate_ms_007b92d0 = static_cast<float>(static_cast<long double>(elapsed) * static_cast<long double>(f(kMilli_0073d708)));
}

// 004512D0 Scene::Render (vtbl+18)
void __fastcall scene_render_004512d0(void* scene, void*) {
    (void)kotor_renderer_init::ATIValidate_0045f640();                // 004512D9 (AL discarded)
    *cells.render_scene_007b935c = scene;                              // 004512DE
    *cells.render_start_007b9280 = query_timer_0044f320();             // 004512E4..004512F5
    if (ref<U>(scene, 0x50) == 0) scene_update_emitters_004511f0(scene, nullptr);   // 004512FB / 004512FF
    if (*cells.animations_0078e3b8 == 0 || *cells.interleaved_007b92f4 == 0)          // 00451304..00451312
        ref<U>(scene, 0x50) = ref<U>(scene, 0x50) + 1;            // 00451314
    *cells.frame_mod3_0078e3f8 = (*cells.frame_mod3_0078e3f8 + 1) % 3;  // 00451317..0045132B (idiv, remainder)
    if (*cells.render_enabled_0078e3f4 != 0) {                         // 00451325 / 00451331
        *cells.counter_007b92e0 = 0;                                   // 00451337
        *cells.counter_007b9310 = 0;                                   // 0045133D
        void* focus = atf(scene, 0xF8);
        Vector returned;
        const U* at_a8 = &ref<U>(scene, 0xA8);                         // 0045134C..00451361 (mov copies to the argument)
        words_form<SetPositionWords>(gob_vt(focus)->s05c_gob_set_position)(focus, nullptr, &returned, at_a8[0], at_a8[1], at_a8[2]); // 0045136F Gob::SetPosition
        *cells.texture_flag_0078e330 = 1;                              // 00451372
        (void)texture_make_current_00421ee0();                         // 00451379
        aur_texture_force_controller_update_00420e50();                // 0045137E
        *cells.active_scene_007b92a0 = scene;                          // 00451384
        manage_scene_bsp_0046e5c0(scene);                              // 0045138A
        glPushAttrib(0x6080);                                          // 00451397
        glDepthFunc(kGlLequal_0073f2ac);                               // 004513A4
        glEnableClientState(kGlNormalArray_0073f39c);                  // 004513B7
        glEnableClientState(kGlTexCoordArray_0073f398);                // 004513BF
        glEnableClientState(kGlVertexArray_0073f3a4);                  // 004513C8
        kotor_renderer_init::glEnable_0073d390(*cells.gl_alpha_test_0078d44c);       // 004513D7
        glAlphaFunc(kGlGreater_0073f2b0, *cells.alpha_ref_007b92e8);   // 004513E6
        kotor_renderer_init::glEnable_0073d390(*cells.gl_lighting_0078d45c);         // 004513F3
        if (ref<U>(scene, 0xA0) == 0 && ref<U>(scene, 0xCC) == 0) { // 004513F5 / 004513FD
            scene_vt(scene)->s120_scene_render_only_gobs(scene, nullptr);              // 00451409 Scene::RenderOnlyGobs
            *cells.active_scene_007b92a0 = nullptr;                    // 0045140F
            glPopAttrib();                                             // 00451415
        } else {
            wind_manager_update_timer_004a5e30(ref<void*>(scene, 0x84), nullptr, *cells.frame_time_0078e574); // 0045142D
            scene_vt(scene)->s0b8_scene_render_single_pass(scene, nullptr);               // 00451436 Scene::RenderSinglePass
            glPopAttrib();                                             // 0045143C
            const I a = *cells.stat_007b9344;                          // ecx
            const I b = *cells.stat_007b9340;                          // edx
            const I total = a + b + *cells.stat_007a46b0;              // lea eax,[ecx+edx]; add eax,edi
            if (*cells.stat_007b9348 < total) *cells.stat_007b9348 = total;   // 00451459 jge
            *cells.active_scene_007b92a0 = nullptr;                    // 0045145F
            if (*cells.stat_007b9338 < b) *cells.stat_007b9338 = b;    // 0045146C
            if (*cells.stat_007b933c < a) *cells.stat_007b933c = a;    // 0045147A
            *cells.stat_007b9340 = 0;                                  // 00451480
            *cells.stat_007b9344 = 0;                                  // 0045148E
            (void)scene_vt(scene)->s0a8_scene_is_animating(scene, nullptr);               // 00451498 (vtbl+A8; AL result unused)
            const I64 end = query_timer_0044f320();                    // 0045149E
            const U start_low = static_cast<U>(*cells.render_start_007b9280);
            *cells.render_end_007b9290 = end;
            const I elapsed = static_cast<I>(static_cast<U>(end) - start_low);
            *cells.render_ms_007b92d4 = static_cast<float>(static_cast<long double>(elapsed) * static_cast<long double>(f(kMilli_0073d708)));
        }
    }
    if (*cells.animations_0078e3b8 != 0 && *cells.interleaved_007b92f4 != 0)          // 004514CA..004514D8
        ref<U>(scene, 0x50) = ref<U>(scene, 0x50) + 1;            // 004514DA
}

// ================================================================ Camera
// 0045D930 NewCAurCamera(name): new(234h) + Camera::Camera; unwind 00716AD0 frees the block.
void* __cdecl new_aur_camera_0045d930(const char* name) {
    void* memory = live_operator_new(kCameraSize);                     // 0045D94B
    if (memory == nullptr) return nullptr;
    return camera_construct_0045d800(memory, nullptr, name);          // 0045D96A (unwind 00716AD0: free(memory))
}

// 0045C240 (unnamed) CAurCamera interface destructor: mov [ecx],7418F8h
void __fastcall camera_base_destruct_0045c240(void* camera, void*) {
    ref<const CameraBaseVTable*>(camera, 0) = &camera_base_vtable_007418f8;
}

// 0045D800 Camera::Camera(name)
void* __fastcall camera_construct_0045d800(void* camera, void*, const char* name) {
    ref<const CameraBaseVTable*>(camera, 0) = &camera_base_vtable_007418f8;   // 0045D81F
    // EH (FuncInfo 007636B8): state 0 -> 0045C240(this); 1 -> Gob dtor 0044B2F0(this+4); 2 -> 0047ABC0(this+1F8).
    gob_construct_00449440(static_cast<aurora_model::Gob*>(atf(camera, 4)), nullptr, name);              // 0045D835 (EH state 0 set at 0045D831)
    ref<const aurora_model::GobVTable*>(camera, 4) = &camera_gob_vtable_00741988;      // 0045D83A
    ref<const CameraVTable*>(camera, 0) = &camera_vtable_00741b20;          // 0045D846
    PlaneList* planes = &ref<PlaneList>(camera, 0x1F8);
    planes->count = 0;                                                 // 0045D84C [ebx+4]
    planes->capacity = 0;                                              // 0045D84F [ebx+8]
    planes->data = nullptr;                                            // 0045D852 [ebx]
    for (U offset = 0x204; offset <= 0x230; offset += 4) ref<U>(camera, offset) = 0; // 0045D854..0045D896
    set_bits(camera, 0x1D0, 0x42340000u);                              // 0045D8A5 view angle 45.0
    set_bits(camera, 0x1DC, 0x3DCCCCCDu);                              // 0045D8AF near 0.1
    set_bits(camera, 0x1E0, 0x42C80000u);                              // 0045D8B9 far 100.0
    set_bits(camera, 0x1D8, kOne);                                     // 0045D8C3
    ref<U>(camera, 0x1D4) = 0;                                       // 0045D8CD
    ref<U>(camera, 0x1F0) = 0;                                       // 0045D8D3
    ref<U>(camera, 0x1EC) = 0;
    ref<U>(camera, 0x1E8) = 0;
    ref<U>(camera, 0x1E4) = 0;                                       // 0045D8E5
    plane_list_allocate_0045c270(planes, nullptr, 6);                  // 0045D8EB (EH state 2 set at 0045D8A0)
    planes->count = 6;                                                 // 0045D8F7
    return camera;
}

// 0045C270 CExoArrayList<Plane>::allocate(size)
void __fastcall plane_list_allocate_0045c270(PlaneList* list, void*, I size) {
    Plane* previous = list->data;                                      // 0045C27C
    list->capacity = size;                                             // 0045C282
    Plane* fresh = static_cast<Plane*>(live_operator_new(static_cast<U>(size) << 4)); // 0045C27E shl 4
    if (fresh != nullptr && size - 1 >= 0) {                           // 0045C293 lea edx,[edi-1]; cmp; jl
        for (I n = 0; n < size; ++n) {                                 // 0045C2A0..0045C2AF (edx = size rows)
            U* row = reinterpret_cast<U*>(&fresh[n]);
            row[0] = 0; row[1] = 0; row[2] = 0; row[3] = 0;
        }
    }
    list->data = fresh;                                                // 0045C2B5
    for (I index = 0; index < list->count; ++index) {                  // 0045C2BC..0045C2E8 (count re-read each pass)
        const U* source = reinterpret_cast<const U*>(&previous[index]);
        U* target = reinterpret_cast<U*>(&list->data[index]);
        target[0] = source[0]; target[1] = source[1]; target[2] = source[2]; target[3] = source[3];
    }
    if (previous != nullptr) live_free(previous);                      // 0045C2EE / 0045C2F1
}

// 0045BEC0 Camera::SetClipDist (vtbl+40): raw DWORD stores
// L3 t1 a2: the argument words are copied with mov (0045BEC0..0045BECE); the float parameters were loaded with fld (S1-free
// but an x87 copy, quieting a signalling NaN).
void __fastcall camera_set_clip_dist_0045bec0(void* camera, void*, float, float) {
    const U* a = A2_STACK_ARGS();
    const U near_bits = a[0], far_bits = a[1];
    ref<U>(camera, 0x1DC) = near_bits;                                 // 0045BEC8
    ref<U>(camera, 0x1E0) = far_bits;                                  // 0045BECE
}

// 0045BEE0 Camera::SetViewAngle (vtbl+44)
void __fastcall camera_set_view_angle_0045bee0(void* camera, void*, float) {
    ref<U>(camera, 0x1D0) = A2_STACK_ARGS()[0];                        // 0045BEE0..0045BEE4 (mov)
}

// 0045C0A0 Camera::AttachToScene (vtbl+14)
I __fastcall camera_attach_to_scene_0045c0a0(void* camera, void*, void* scene) {
    gob_set_scene_0043f200(atf(camera, 4), nullptr, scene);             // 0045C0A8
    return 1;                                                          // 0045C0AD
}

// 0045C200 Camera::AttachToObject: add ecx,4; jmp 00443820 (same three stack words, RET 0Ch, EAX passthrough)
void* __fastcall camera_attach_to_object_0045c200(void* camera, void*, void* object, const char* part, I mode) {
    return gob_attach_to_object_00443820(atf(camera, 4), nullptr, object, part, mode);
}

// 0043F200 Gob::SetScene
void __fastcall gob_set_scene_0043f200(void* gob, void*, void* scene) { scene_child_detach_attach(gob, scene); }

// 00443820 Gob::AttachToObject(object, part name, mode) -> behavior
void* __fastcall gob_attach_to_object_00443820(void* gob, void*, void* object, const char* part_name, I mode) {
    void* behavior = nullptr;                                          // 0044383C xor ebp,ebp
    if (object == nullptr) {                                           // 00443843
        gob_vt(gob)->s058_gob_set_behavior(as_gob(gob), nullptr, nullptr);               // 00443976 Gob::SetBehavior(0)
        return nullptr;
    }
    void* part = gob_vt(object)->s10c_gob_get_part(as_gob(object), nullptr, part_name);   // 00443852 Gob::GetPart
    if (part == nullptr) {                                             // 0044385C
        gob_vt(gob)->s058_gob_set_behavior(as_gob(gob), nullptr, nullptr);
        return nullptr;
    }
    slot<SlotPartTransform>(part, 0x0C)(part, nullptr, atf(gob, 0x78), atf(gob, 0x84));  // 00443871
    U size = 0;
    BehaviorCtor construct = nullptr;
    switch (static_cast<U>(mode)) {                                    // 00443878 cmp eax,3; ja (unsigned); table 00443990
    case 0: size = 0x3C; construct = behavior_attach_0049fb10; break;             // 00443888
    case 1: size = 0x4C; construct = behavior_attach_translate_0049fdc0; break;   // 004438AE
    case 2: size = 0x48; construct = behavior_attach_rotate_0049fe00; break;      // 004438D4
    case 3: size = 0x3C; construct = behavior_attach_no_orientation_0049fe40; break; // 004438FA
    default: break;                                                    // 0044392C with ebp=0
    }
    if (construct != nullptr) {
        void* memory = live_operator_new(size);
        if (memory != nullptr) {
            behavior = construct(memory, nullptr, gob, object, part);  // push edi(part),ebx(object),esi(gob); unwind 00715BD0..: free
        }
    }
    gob_vt(gob)->s058_gob_set_behavior(as_gob(gob), nullptr, behavior);                  // 00443931 Gob::SetBehavior
    scene_child_detach_attach(gob, ref<void*>(object, 0x68));        // 00443934..00443968
    return behavior;                                                   // 0044397F mov eax,ebp
}

// 0045C540 Camera::RenderScene (vtbl+08)
// L3 t1 (2026-10-24): PLAN-v3 writing rule (replaces the a1 helpers x87trig_0045c72b/74d/76d/78b). Register values
// are long double, each original fstp dword an explicit (float), each original fstp qword (the gluPerspective /
// glOrtho double arguments) an explicit (double); DegToRad 004A96D0's ST0 feeds tan unrounded (ret80: ARITH x4).
void __fastcall camera_render_scene_0045c540(void* camera, void*) {
    if (ref<void*>(camera, 0x6C) == nullptr) return;                 // 0045C554 (Gob+68 scene)
    const U mode_index = (*cells.wireframe_007bb4f0 == 0) ? 1u : 0u;   // 0045C568 sete al
    glPolygonMode(kGlFrontAndBack_0073f2f4, kPolygonModes_0073f2e4[mode_index]); // 0045C577
    glMatrixMode(kGlModelview_0073f298); glPushMatrix();               // 0045C589 / 0045C591
    glMatrixMode(kGlProjection_0073f290); glPushMatrix();              // 0045C59A / 0045C59C
    GLint viewport[4];                                                 // [esp+58h]..[esp+64h]
    glGetIntegerv(0x0BA2, viewport);                                   // 0045C5A8 GL_VIEWPORT
    long double width, height;                                         // fild (exact)
    if (ref<I>(camera, 0x1EC) == 0 && ref<I>(camera, 0x1F0) == 0) {    // 0045C5B6 / 0045C5C0
        height = static_cast<long double>(viewport[3]);                // fild [esp+64h]
        width = static_cast<long double>(viewport[2]);                 // fild [esp+60h]
    } else {
        const I x = ref<I>(camera, 0x1E4);
        const I y = ref<I>(camera, 0x1E8);
        const I w = ref<I>(camera, 0x1EC);
        const I h = ref<I>(camera, 0x1F0);
        if (x >= static_cast<I>(*cells.width_0078d1d4) || x + w < 0 ||           // 0045C5D2 / 0045C5E3
            y >= static_cast<I>(*cells.height_0078d1d8) || y + h < 0) {          // 0045C5EF / 0045C606
            void* scene = ref<void*>(camera, 0x6C);                  // 0045CC7B (matrices stay pushed, as original)
            ref<U>(scene, 0x50) = ref<U>(scene, 0x50) + 1;        // 0045CC7E inc [esi+50h]
            return;
        }
        aurora_viewport_00459170(static_cast<U>(x), static_cast<U>(y), static_cast<U>(w), static_cast<U>(h)); // 0045C610
        height = static_cast<long double>(ref<I>(camera, 0x1F0));      // 0045C615
        width = static_cast<long double>(ref<I>(camera, 0x1EC));       // 0045C61B
    }
    // 0045C624..0045C630: fdiv st,st(1) ; fstp dword [esp+14h] (aspect) ; fmul dword [0073DFEC] ; fstp dword [esi+1D8h]
    const long double aspect = S(width / height);                      // 0045C626 (held as its rounded value: [esp+14h])
    ref<float>(camera, 0x1D8) = S(height * kRsOrthoScale_0073dfec);    // 0045C630
    glMatrixMode(kGlProjection_0073f290);                              // 0045C63D
    glLoadIdentity();                                                  // 0045C63F
    const U near_bits = ref<U>(camera, 0x1DC);                       // 0045C645
    Plane* planes = ref<PlaneList>(camera, 0x1F8).data;
    {   U* row = reinterpret_cast<U*>(&planes[0]);                     // 0045C661..0045C685
        row[0] = 0; row[1] = 0; row[2] = kOne; row[3] = near_bits; }
    {   U* row = reinterpret_cast<U*>(&planes[1]);                     // 0045C6A5..0045C6CD
        row[0] = 0; row[1] = 0; row[2] = kMinusOneImm;
        ref<float>(row, 0xC) = S(-W(ref<float>(camera, 0x1E0))); }    // 0045C688 fld far ; fchs ; 0045C6C2 fstp dword
    if (!(ref<float>(camera, 0x1D0) > 0.0f)) {                         // 0045C6D6 fcomp 0 ; test 41h ; jne (<= 0 or unordered)
        // 0045C9CB..0045C9FD: glOrtho(0, aspect * [1D8h], 0, [1D8h], near, far) - every argument an fstp qword; the float
        // fields widen exactly (fld dword ; fstp qword); right = aspect * [1D8h] is one (double) conversion (0045C9F6).
        opaque(&gl_ortho_00404b60)(0.0, static_cast<double>(aspect * ref<float>(camera, 0x1D8)), 0.0,
                          static_cast<double>(ref<float>(camera, 0x1D8)), static_cast<double>(ref<float>(camera, 0x1DC)),
                          static_cast<double>(ref<float>(camera, 0x1E0)));
        U* row = reinterpret_cast<U*>(ref<PlaneList>(camera, 0x1F8).data + 2);    // 0045CA06 (plane list re-read per row)
        row[0] = kMinusOneImm; row[1] = 0; row[2] = 0; row[3] = 0;     // 0045CA0F..0045CA4D
        const long double right_d = -(aspect * ref<float>(camera, 0x1D8));   // 0045CA02 / 0045CA50 fmul ; 0045CA67 fchs
        row = reinterpret_cast<U*>(ref<PlaneList>(camera, 0x1F8).data + 3);       // 0045CA56
        row[0] = kOne; row[1] = 0; row[2] = 0;                         // 0045CA5F..0045CA8A
        ref<float>(row, 0xC) = S(right_d);                             // 0045CA8D fstp
        row = reinterpret_cast<U*>(ref<PlaneList>(camera, 0x1F8).data + 4);       // 0045CA9E
        row[0] = 0; row[1] = kMinusOneImm; row[2] = 0; row[3] = 0;     // 0045CAA7..0045CAE5
        const long double top_d = -W(ref<float>(camera, 0x1D8));       // 0045CAE8 fld ; 0045CAF4 fchs
        row = reinterpret_cast<U*>(ref<PlaneList>(camera, 0x1F8).data + 5);       // 0045CAEE
        row[0] = 0; row[1] = kOne; row[2] = 0;                         // 0045CAFC..0045CB2D
        ref<float>(row, 0xC) = S(top_d);                               // 0045CB25 fstp
    } else {
        // 0045C6ED..0045C70F: gluPerspective(fov, aspect, near, far) - every argument an fstp qword; the float fields widen
        // exactly; aspect (the [esp+14h] float) is one (double) conversion of its register value (0045C702).
        opaque(&glu_perspective_0045c300)(static_cast<double>(ref<float>(camera, 0x1D0)), static_cast<double>(aspect),
                                 static_cast<double>(ref<float>(camera, 0x1DC)), static_cast<double>(ref<float>(camera, 0x1E0)));
        // Four times: fld [esi+1D0h] ; fmul dword [0073E9AC] ; fstp dword [esp] (one (float) each) ; call DegToRad (ST0) ;
        // fptan ; fstp st(0). The four results are stored as the original's a / b / c / d floats (a = -(tan * aspect),
        // b = tan * aspect, c = -tan, d = tan stored twice) and copied with mov into the NormalOf argument vectors.
        // The four NormalOf argument vectors are frame words (volatile: each float is stored at once, as the original's
        // stack slots; a float held across the next DegToRad / tan call was spilled through 4 bytes, S1).
        volatile U ad[3], ac[3], bc[3], bd[3];
        const long double t_a = tan(static_cast<long double>(deg_to_rad_004a96d0(S(W(ref<float>(camera, 0x1D0)) * kRsHalf_0073e9ac))));   // 0045C714..0045C72D
        const U a = stored_bits(-(t_a * aspect));                      // 0045C72F fmul [esp+18h] ; fchs ; 0045C735 fstp [esp+1Ch]
        ad[0] = a; ac[0] = a;
        const long double t_b = tan(static_cast<long double>(deg_to_rad_004a96d0(S(W(ref<float>(camera, 0x1D0)) * kRsHalf_0073e9ac))));   // 0045C739..0045C74F
        const U b = stored_bits(t_b * aspect);                         // 0045C751 fmul [esp+18h] ; 0045C755 fstp [esp+18h]
        bc[0] = b; bd[0] = b;
        const long double t_c = tan(static_cast<long double>(deg_to_rad_004a96d0(S(W(ref<float>(camera, 0x1D0)) * kRsHalf_0073e9ac))));   // 0045C759..0045C76F
        const U c = stored_bits(-t_c);                                 // 0045C771 fchs ; 0045C773 fstp [esp+20h]
        ac[1] = c; bc[1] = c;
        const long double t_d = tan(static_cast<long double>(deg_to_rad_004a96d0(S(W(ref<float>(camera, 0x1D0)) * kRsHalf_0073e9ac))));   // 0045C777..0045C78B
        ad[1] = stored_bits(t_d);                                      // 0045C7E1 fst [esp+34h]
        bd[1] = stored_bits(t_d);                                      // 0045C7E5 fstp [esp+48h] (the unrounded ST0 again)
        ad[2] = kMinusOneImm; ac[2] = kMinusOneImm; bc[2] = kMinusOneImm; bd[2] = kMinusOneImm;   // 0045C7B7.. (mov -1.0)
        U origin[3] = {0, 0, 0};                                       // 0045C7C7..0045C7D7
        U normal[3];
        const auto plane_row = [camera](U index, const Vector* n) {    // 0045C7EE..0045C821 (list re-read; d = 0)
            U* row = reinterpret_cast<U*>(ref<PlaneList>(camera, 0x1F8).data + index);
            const U* w = reinterpret_cast<const U*>(n);
            const U x = w[0], y = w[1], z = w[2];
            row[0] = x; row[1] = y; row[2] = z; row[3] = 0;
        };
        const auto vec = [](volatile U* w) { return reinterpret_cast<Vector*>(const_cast<U*>(w)); };
        plane_row(2, normal_of_004aca00(vec(normal), vec(origin), vec(ad), vec(ac)));   // 0045C7E9 left
        plane_row(3, normal_of_004aca00(vec(normal), vec(origin), vec(bc), vec(bd)));   // 0045C876 right
        plane_row(4, normal_of_004aca00(vec(normal), vec(origin), vec(bd), vec(ad)));   // 0045C903 top
        plane_row(5, normal_of_004aca00(vec(normal), vec(origin), vec(ac), vec(bc)));   // 0045C996 bottom
    }
    if (*cells.alt_view_007bb4f4 != 0)                                 // 0045CB33
        gl_set_view_transform_00425ca0(cells.alt_view_position_007bb504, cells.alt_view_orientation_0078e440);
    else
        gl_set_view_transform_00425ca0(&ref<Vector>(camera, 0x7C), &ref<Quaternion>(camera, 0x88)); // 0045CB53
#pragma clang loop unroll(disable)
    for (U offset = 0; offset < 0x60; offset += 0x10) {                // 0045CB63..0045CBBC (kept a loop: one in-loop fstp)
        Plane* plane = reinterpret_cast<Plane*>(reinterpret_cast<unsigned char*>(ref<PlaneList>(camera, 0x1F8).data) + offset);
        Vector rotated;
        quaternion_rotate_004a9bb0(&rotated, &ref<Quaternion>(camera, 0x88), reinterpret_cast<const Vector*>(plane));
        copy_words3(plane, &rotated);                                  // 0045CB78..0045CB85 (raw dword moves)
        plane = reinterpret_cast<Plane*>(reinterpret_cast<unsigned char*>(ref<PlaneList>(camera, 0x1F8).data) + offset);
        // 0045CB8E..0045CBB9: ((d - c*pz) - b*py) - a*px ; fstp dword [plane+0Ch]
        const long double distance = (static_cast<long double>(plane->d) - static_cast<long double>(plane->c) * ref<float>(camera, 0x84))
                                     - static_cast<long double>(plane->b) * ref<float>(camera, 0x80);
        plane->d = S(distance - static_cast<long double>(plane->a) * ref<float>(camera, 0x7C));   // 0045CBB9 fstp
    }
    const Plane* chosen = ref<PlaneList>(camera, 0x1F8).data + *cells.plane_index_007bb4ec;   // 0045CBC4 shl 4
    std::memcpy(cells.plane_normal_007bb4f8, chosen, 12);              // 0045CBD6..0045CBEE
    *cells.plane_distance_007bb4e4 = S(W(chosen->d));                  // 0045CBF4..0045CBF8 fld ; fstp dword (an x87 copy)
    ref<U>(camera, 0x1F4) = 1;                                       // 0045CBFE
    *cells.current_camera_007bb4e8 = camera;                           // 0045CC0F
    glMatrixMode(kGlModelview_0073f298);                               // 0045CC15
    void* scene = ref<void*>(camera, 0x6C);
    scene_vt(scene)->s018_scene_render(scene, nullptr);                       // 0045CC1C Scene::Render
    glMatrixMode(kGlProjection_0073f290); glPopMatrix();               // 0045CC26 / 0045CC2E
    glMatrixMode(kGlModelview_0073f298); glPopMatrix();                // 0045CC37 / 0045CC39
    *cells.current_camera_007bb4e8 = nullptr;                          // 0045CC3B
    if (ref<U>(camera, 0x1EC) != 0 || ref<U>(camera, 0x1F0) != 0) // 0045CC4B / 0045CC55
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]); // 0045CC6D
}

// ================================================================ math / GL wrappers
// 004A96D0 DegToRad: fld [esp+4]; fmul [007415F8]; ret (ST0)
// L3 t1 a2: the product of two floats is exact in 53 bits, so the double-typed product IS the original's ST0 (no
// long double -> double conversion, which was a qword round trip with no original store: S3).
double __cdecl deg_to_rad_004a96d0(float degrees) { return static_cast<double>(degrees) * static_cast<double>(f(kDegToRad_007415f8)); }

// 0045C300 _gluPerspective: re-pushes four doubles to GLU32 gluPerspective (thunk 0070C126)
// L3 t1 a2: the eight argument words are re-pushed with mov (0045C300..0045C327); double parameters were re-pushed
// with fld/fstp qword (quiets a signalling NaN).
void __cdecl glu_perspective_0045c300(double, double, double, double) {
    using GluPerspectiveWords = void (__stdcall*)(U, U, U, U, U, U, U, U);
    const U* a = A2_STACK_ARGS();
    words_form<GluPerspectiveWords>(&gluPerspective)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);   // 0045C328 (thunk 0070C126)
}

// 00404B60 _glOrtho: re-pushes six doubles to opengl32 glOrtho (IAT 0073D234)
// L3 t1 a2: the twelve argument words are re-pushed with mov (00404B60..00404B9B), as 0045C300.
void __cdecl gl_ortho_00404b60(double, double, double, double, double, double) {
    using GlOrthoWords = void (__stdcall*)(U, U, U, U, U, U, U, U, U, U, U, U);
    const U* a = A2_STACK_ARGS();
    words_form<GlOrthoWords>(&glOrtho)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11]);   // 00404B9C
}

// 004AB210 vectorCrossProduct(out, p, q) = p x q
Vector* __cdecl vector_cross_004ab210(Vector* out, const Vector* p, const Vector* q) {
    const long double z = static_cast<long double>(p->x) * q->y - static_cast<long double>(p->y) * q->x;
    const long double y = static_cast<long double>(p->z) * q->x - static_cast<long double>(q->z) * p->x;
    const long double x = static_cast<long double>(p->y) * q->z - static_cast<long double>(p->z) * q->y;
    out->x = static_cast<float>(x); out->y = static_cast<float>(y); out->z = static_cast<float>(z);   // 004AB242..004AB247
    return out;                                                        // EAX = out (004AB23C)
}

// 004AB100 vector_magnitude: sqrt((x*x + y*y) + z*z) in ST0
// L3 t1 a2: the reviewed asm body (unchanged instructions) as a naked function ending in the original's `ret`, so ST0 is
// returned as the original leaves it; the former `fstp qword [magnitude]` + `return magnitude` was a qword round trip
// with no original store (S3). The asm has no CALL. Extern callers keep the `double` declaration (ST0 return).
__declspec(naked) double __cdecl vector_magnitude_004ab100(const Vector*) {
    __asm {
        mov eax, dword ptr [esp+4]
        fld dword ptr [eax+8]
        fld dword ptr [eax+4]
        fld dword ptr [eax]
        fld st(0)
        fmul st(0), st(1)
        fld st(2)
        fmul st(0), st(3)
        faddp st(1), st(0)
        fld st(3)
        fmul st(0), st(4)
        faddp st(1), st(0)
        fsqrt
        fstp st(3)
        fstp st(0)
        fstp st(0)
        ret
    }
}

// 004ACA00 NormalOf(out, a, b, c) = normalize((b-a) x (c-b)); zero length -> (1,0,0)
Vector* __cdecl normal_of_004aca00(Vector* out, const Vector* a, const Vector* b, const Vector* c) {
    const Vector e1{S(W(c->x) - W(b->x)), S(W(c->y) - W(b->y)), S(W(c->z) - W(b->z))};   // 004ACA0B..004ACA2B
    const Vector e2{S(W(b->x) - W(a->x)), S(W(b->y) - W(a->y)), S(W(b->z) - W(a->z))};   // 004ACA2F..004ACA51
    Vector cross;
    vector_cross_004ab210(&cross, &e2, &e1);                           // 004ACA56
    const long double magnitude = vector_magnitude_004ab100(&cross);        // 004ACA60
    if (magnitude == 0.0) {                                            // fcom 0; test ah,44h; jp (equal only)
        out->x = f(kOne); out->y = 0.0f; out->z = 0.0f;                // 004ACA7D..004ACA86
        return out;
    }
    const long double inverse = 1.0 / magnitude;                            // 004ACA8D fdivr [0073D764]
    out->x = static_cast<float>(static_cast<long double>(cross.x) * inverse);
    out->y = static_cast<float>(static_cast<long double>(cross.y) * inverse);
    out->z = static_cast<float>(static_cast<long double>(cross.z) * inverse);
    return out;
}

// 004A9BB0 operator*(Quaternion, Vector) -> rotated vector
Vector* __cdecl quaternion_rotate_004a9bb0(Vector* out, const Quaternion* q, const Vector* v) {
    const U w_bits = float_to_bits(q->w);
    if (*cells.quaternion_shortcut_007a1980 != 0 && (w_bits == 0x3F800000u || w_bits == 0xBF800000u)) { // 004A9BB8..004A9BD0
        const Vector copy = *v;                                        // 004A9BDB..004A9BEA raw DWORD copy
        *out = copy;
        return out;
    }
    const float xx = S(W(q->x) * W(q->x)), yy = S(W(q->y) * W(q->y)), zz = S(W(q->z) * W(q->z));  // [esp] [esp+10h] [esp+0Ch]
    const float yx = S(W(q->y) * W(q->x)), zx = S(W(q->z) * W(q->x)), xw = S(W(q->x) * W(q->w));  // [esp+14h] [esp+20h] [esp+8]
    const float zy = S(W(q->z) * W(q->y)), yw = S(W(q->y) * W(q->w)), zw = S(W(q->z) * W(q->w));  // [esp+4] [esp+1Ch] [esp+18h]
    const long double vx = v->x, vy = v->y, vz = v->z;
    const long double t1 = (1.0 - 2.0 * (static_cast<long double>(yy) + xx)) * vz + 2.0 * (static_cast<long double>(zx) - yw) * vx +
                      2.0 * (static_cast<long double>(zy) + xw) * vy;      // 004A9C56..004A9C84
    const long double t2 = (1.0 - 2.0 * (static_cast<long double>(zz) + xx)) * vy + 2.0 * (static_cast<long double>(zw) + yx) * vx +
                      2.0 * (static_cast<long double>(zy) - xw) * vz;      // 004A9C86..004A9CB4
    const long double t3 = (1.0 - 2.0 * (static_cast<long double>(zz) + yy)) * vx + 2.0 * (static_cast<long double>(yx) - zw) * vy +
                      2.0 * (static_cast<long double>(yw) + zx) * vz;      // 004A9CB6..004A9CE8
    out->x = static_cast<float>(t3); out->y = static_cast<float>(t2); out->z = static_cast<float>(t1); // 004A9CEA..004A9CEF
    return out;
}

// 004A9B50 operator-(Quaternion): (w * -1.0f, x, y, z)
Quaternion* __cdecl quaternion_negate_w_004a9b50(Quaternion* out, const Quaternion* q) {
    const float w = q->w, x = q->x, y = q->y, z = q->z;               // all four loaded before any store
    out->w = S(W(w) * W(f(kMinusOne_00740cbc)));                         // 004A9B63 fmul; 004A9B69 fstp
    out->x = x; out->y = y; out->z = z;
    return out;
}

// 004AA9C0 MakeMatrix4x4(out[16], translation, quaternion)
float* __cdecl make_matrix4x4_004aa9c0(float* m, const Vector* t, const Quaternion* q) {
    const long double xx = static_cast<long double>(q->x) * q->x, yy = static_cast<long double>(q->y) * q->y;   // FPU stack
    const long double zz = static_cast<long double>(q->z) * q->z, yx = static_cast<long double>(q->y) * q->x;
    const float zx = S(W(q->z) * W(q->x)), wx = S(W(q->w) * W(q->x)), zy = S(W(q->z) * W(q->y)), wy = S(W(q->w) * W(q->y)); // fstp dword
    const long double wz = static_cast<long double>(q->w) * q->z;
    U* bits = reinterpret_cast<U*>(m);
    bits[3] = 0; bits[7] = 0; bits[11] = 0;                            // 004AAA15..004AAA1D
    m[0] = static_cast<float>(1.0 - 2.0 * (zz + yy));
    m[1] = static_cast<float>(2.0 * (wz + yx));
    m[2] = static_cast<float>(2.0 * (static_cast<long double>(zx) - wy));
    m[4] = static_cast<float>(2.0 * (yx - wz));
    m[5] = static_cast<float>(1.0 - 2.0 * (zz + xx));
    m[6] = static_cast<float>(2.0 * (static_cast<long double>(zy) + wx));
    m[8] = static_cast<float>(2.0 * (static_cast<long double>(wy) + zx));
    m[9] = static_cast<float>(2.0 * (static_cast<long double>(zy) - wx));
    m[10] = static_cast<float>(1.0 - 2.0 * (yy + xx));
    const U* translation = reinterpret_cast<const U*>(t);
    bits[12] = translation[0]; bits[13] = translation[1]; bits[14] = translation[2];   // 004AAA90..004AAAA0
    bits[15] = kOne;                                                   // 004AAAA3
    return m;
}

// 00423C40 Quaternion::axis(out) (thiscall, RET 4)
// L3 t1 (2026-10-24): PLAN-v3 writing rule (replaces the a2-verified scene_acos helper sequence for this function; the
// helpers stay for 00425D19 / 0046F169). acos is the kotor_math.h long double overload (_CIacos semantics: the 80-bit
// argument, the 80-bit result; L1, until then the reviewed t11b replica).
Vector* __fastcall quaternion_axis_00423c40(const Quaternion* q, void*, Vector* out) {
    // 00423C43..00423C54: the three components are copied before out is written (out may alias q): x by MOV, y and z by
    // fld/fstp dword. Each is only ever multiplied later, so they are held as register values here.
    const long double x = ref<float>(const_cast<Quaternion*>(q), 4);
    const long double y = ref<float>(const_cast<Quaternion*>(q), 8);
    const long double z = ref<float>(const_cast<Quaternion*>(q), 0xC);
    // 00423C58..00423C5F: fld w ; call _CIacos ; fadd st,st
    const long double r = acos(static_cast<long double>(ref<float>(const_cast<Quaternion*>(q), 0)));
    const long double angle = r + r;
    // 00423C61..00423C81: fld st(0) ; fcom 0 ; test 5 ; jp (skip fchs unless less) ; fcomp qword 1e-7 ; test 5 ; jp
    const long double magnitude = (angle < 0.0f) ? -angle : angle;
    if (magnitude < kAxisEpsilon_0073f3d8) {
        out->x = f(kOne); out->y = 0.0f; out->z = 0.0f;                // 00423C83..00423C90 (raw dwords)
        return out;
    }
    // 00423C99..00423CA1: fmul qword 0.5 ; fsin ; fdivr qword 1.0
    const long double scale = kRsOneD_0073ede8 / sin(angle * kRsHalfD_0073f3d0);
    out->x = static_cast<float>(x * scale);                            // 00423CA7..00423CAD
    out->y = static_cast<float>(y * scale);                            // 00423CAF..00423CB5
    out->z = static_cast<float>(z * scale);                            // 00423CB8..00423CBE
    return out;
}

// 00425CA0 GLRender::SetViewTransform(position, orientation)
void __cdecl gl_set_view_transform_00425ca0(const Vector* position, const Quaternion* orientation) {
    glMatrixMode(0x1700);                                              // 00425CAF
    glLoadIdentity();                                                  // 00425CB1
    glMatrixMode(0x1700);                                              // 00425CBC
    if (*cells.matrix_view_0078d494 != 0) {                            // 00425CBE
        Vector zero;
        reinterpret_cast<U*>(&zero)[0] = 0; reinterpret_cast<U*>(&zero)[1] = 0; reinterpret_cast<U*>(&zero)[2] = 0;
        Quaternion inverse;
        quaternion_negate_w_004a9b50(&inverse, orientation);           // 00425CE9
        float matrix[16];
        make_matrix4x4_004aa9c0(matrix, &zero, &inverse);              // 00425CF9
        glMultMatrixf(matrix);                                         // 00425D06
    } else {
        Vector axis;
        quaternion_axis_00423c40(orientation, nullptr, &axis);         // 00425D19
        // L3 t1 a2: real L1 acos(long double) (_CIacos 006FC0E0 on the 80-bit operand loaded from w), then
        // 00425D34 fadd st,st ; 00425D37 fmul dword [0073F3F8] ; 00425D3D fstp dword (the angle argument).
        const long double arc = acos(W(orientation->w));                 // 00425D1E fld [esi] ; 00425D2F call 006FC0E0
        const U degrees = stored_bits((arc + arc) * f(kRadToNegDeg_0073f3f8));   // 00425D34..00425D3D fstp [esp]
        const U* ax = reinterpret_cast<const U*>(&axis);
        words_form<GlWords4>(&glRotatef)(degrees, ax[0], ax[1], ax[2]);   // 00425D20..00425D40 (axis pushed as dwords)
    }
    // L3 t1 a2 (2026-11-01, value gate): fld ; fchs ; fstp dword per word (00425D4A..00425D63, z first). The float negation
    // compiled to an integer sign flip here, which keeps a signalling NaN the original quiets.
    const U tz = stored_bits(-W(position->z));                         // 00425D4A..00425D52
    const U ty = stored_bits(-W(position->y));                         // 00425D56..00425D5B
    const U tx = stored_bits(-W(position->x));                         // 00425D5F..00425D63
    words_form<GlWords3>(&glTranslatef)(tx, ty, tz);                   // 00425D66
}

// 0044F320 QueryTimer: microseconds as signed 64-bit (EDX:EAX)
I64 __cdecl query_timer_0044f320() {
    const U guard = *cells.timer_guard_007b9418;
    if ((guard & 1u) == 0) {                                           // 0044F32E test al,cl
        *cells.timer_guard_007b9418 = guard | 1u;                      // 0044F33F
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);                         // 0044F345 (result ignored)
        *cells.timer_frequency_007b9410 = frequency.QuadPart;          // 0044F353 / 0044F359
    }
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);                                 // 0044F364 (result ignored)
    return (counter.QuadPart * 1000000) / *cells.timer_frequency_007b9410;   // __allmul / __alldiv (signed)
}

// ================================================================ per-frame core: RenderSinglePass
}   // extern "C" (L3 t1 a2: closed around the C++-linkage helpers below; LD1)
namespace {
// Shared timing tail: end = QueryTimer(); [7B9288]=end; ms = (I)(end.lo - [7B9298].lo) * 0.001f; then restart [7B9298].
// L3 t1 a2: force-inlined and long double (it was a /Ob0 call returning double: a qword round trip, S3); C++ linkage
// (outside the file's extern "C" block; LD1).
__forceinline long double pass_elapsed_ms() {
    const I64 end = query_timer_0044f320();
    const U start_low = static_cast<U>(*cells.animate_start_007b9298);
    *cells.animate_end_007b9288 = end;
    const I elapsed = static_cast<I>(static_cast<U>(end) - start_low);
    return static_cast<long double>(elapsed) * static_cast<long double>(f(kMilli_0073d708));
}
inline void pass_restart() { *cells.animate_start_007b9298 = query_timer_0044f320(); }
// 004516A1..004516C1 (also 0045171E.., 0045184F..): clear stencil to (1<<(bits-1))-1, mask 0xFF, glClear(STENCIL)
inline void clear_stencil() {
    const I bits = aur_get_available_stencil_bits_0045fd00();
    if (bits != 0) glClearStencil(static_cast<GLint>((1u << static_cast<U>(bits - 1)) - 1u));
    glStencilMask(0xFF);
    glClear(0x400);
}
using SlotFour = void (__fastcall*)(void*, void*, U, U, U, U);
constexpr U kGlColorArray_0073f3a0 = 0x8076;           // GL_COLOR_ARRAY
constexpr U kBlendSrc_0073f270 = 0x0302, kBlendDst_0073f274 = 0x0303;
constexpr U kGlFront_0073f2ec = 0x0404, kGlAmbient_0073f2fc = 0x1200, kGlDiffuse_0073f300 = 0x1201, kGlEmission_0073f304 = 0x1600;
constexpr U kGlAlways_0073f2bc = 0x0207, kGlKeep_0073f2c8 = 0x1E00, kGlReplace_0073f2d0 = 0x1E01;
}
extern "C" {

// 004AAAB0 Homogenize(v, &w): copies v.xyz and *w into the static float[4] at 00831F18, returns it.
float* __cdecl homogenize_004aaab0(const Vector* v, const float* w) {
    U* out = reinterpret_cast<U*>(cells.homogenize_00831f18);
    const U* in = reinterpret_cast<const U*>(v);
    out[0] = in[0]; out[1] = in[1]; out[2] = in[2];                    // 004AAAB6..004AAACC
    out[3] = *reinterpret_cast<const U*>(w);                           // 004AAAD3
    return cells.homogenize_00831f18;                                  // mov eax,831F18h
}

// 0045FD00 AurGetAvailableStencilBits: cached glGetIntegerv(GL_STENCIL_BITS) into [0078E54C]
I __cdecl aur_get_available_stencil_bits_0045fd00() {
    if (*cells.stencil_bits_0078e54c == -1)                            // 0045FD05 cmp eax,-1
        glGetIntegerv(0x0D57, const_cast<GLint*>(reinterpret_cast<volatile GLint*>(cells.stencil_bits_0078e54c))); // 0045FD14
    return *cells.stencil_bits_0078e54c;                               // 0045FD1A reload
}

// 00437BA0 Matrix::Matrix(Vector r0, Vector r1, Vector r2) by value, RET 24h: zero 9 then rows.
float* __fastcall matrix_construct_00437ba0(float* out, void*, Vector, Vector, Vector) {
    const U* r = A2_STACK_ARGS();                                      // r0, r1, r2 words [esp+4]..[esp+24h]
    U* m = reinterpret_cast<U*>(out);
    for (int i = 0; i < 9; ++i) m[i] = 0;                              // 00437BA8..00437BBF
    for (int i = 0; i < 9; ++i) m[i] = r[i];                           // 00437BC4..00437BFB (mov)
    return out;
}

// 0044F470 Quaternion::getmatrix(out 3x3) thiscall RET 4
// L3 t1 a2: the thirteen original fstp dword stores are the thirteen (float) conversions; the nine matrix words are passed
// to Matrix::Matrix as the stack words the original fills with mov (0044F538..0044F58A); y2, yx, wz, z2 are held as
// their rounded values (the original reloads them from [esp+8] / [esp+10h] / [esp+0Ch] / [esp+4]).
float* __fastcall quaternion_get_matrix_0044f470(const Quaternion* q, void*, float* out) {
    const long double xz = static_cast<long double>(q->x) * q->z, wy = static_cast<long double>(q->y) * q->w;   // 0044F473..0044F47D
    const U a = stored_bits(2.0 * (wy + xz));                                   // 0044F485 fstp [esp+14h]
    const long double yz = static_cast<long double>(q->y) * q->z, wx = static_cast<long double>(q->x) * q->w;   // 0044F489..0044F492
    const U b = stored_bits(2.0 * (yz - wx));                                   // 0044F49A fstp [esp+18h]
    const long double y2 = S(W(q->y) * W(q->y));                                // 0044F4A5 fstp [esp+8]
    const long double xx = static_cast<long double>(q->x) * q->x;               // stays on the stack
    const U c = stored_bits(1.0 - 2.0 * (y2 + xx));                             // 0044F4C0 fstp [esp+1Ch]
    const long double yx = S(W(q->y) * W(q->x));                                // 0044F4CA fstp [esp+10h]
    const long double wz = S(W(q->w) * W(q->z));                                // 0044F4D3 fstp [esp+0Ch]
    const U d = stored_bits(2.0 * (yx - wz));                                   // 0044F4E1 fstp [esp+20h]
    const long double z2 = S(W(q->z) * W(q->z));                                // 0044F4EC fstp [esp+4]
    const U e = stored_bits(1.0 - 2.0 * (z2 + xx));                             // 0044F500 fstp [esp+24h]
    const U ff = stored_bits(2.0 * (wx + yz));                                  // 0044F50A fstp [esp+28h]
    const U g = stored_bits(1.0 - 2.0 * (z2 + y2));                             // 0044F520 fstp [esp+2Ch]
    const U h = stored_bits(2.0 * (wz + yx));                                   // 0044F52E fstp [esp+30h]
    const U i = stored_bits(2.0 * (xz - wy));                                   // 0044F532..0044F57D fstp [esp+58h]
    volatile U w[9];                                                            // the words as frame words (stored once,
    w[0] = g; w[1] = h; w[2] = i; w[3] = d; w[4] = e; w[5] = ff; w[6] = a; w[7] = b; w[8] = c;   // copied with mov)
    return words_form<MatrixWords>(&matrix_construct_00437ba0)(out, nullptr, w[0], w[1], w[2], w[3], w[4], w[5], w[6], w[7], w[8]); // 0044F58F
}

// 004514F0 Scene::RenderSinglePass (Scene vtbl+B8)
void __fastcall scene_render_single_pass_004514f0(void* scene, void*) {
    using kotor_renderer_init::glEnable_0073d390;
    using kotor_renderer_init::glDisable_0073d22c;
    if (aur_nv2x_level_shaders_available_0045fbd0()) {                 // 004514F9
        void* camera = *cells.current_camera_007bb4e8;
        float m[9];
        quaternion_get_matrix_0044f470(&ref<Quaternion>(camera, 0x88), nullptr, m);   // 00451517
        camera = *cells.current_camera_007bb4e8;                       // 0045151C reload
        // L3 t1 a2: every argument is a raw word pushed from a register loaded with mov (00451522..0045156B); the
        // float-typed call loaded them with fld dword (quiets a signalling NaN) and held them across the calls (S1).
        using ProgramEnvWords = void (__cdecl*)(U, U, U, U, U);
        const U* mw = reinterpret_cast<const U*>(m);
        const U* position = &ref<U>(camera, 0x7C);
        const U px = position[0], py = position[1], pz = position[2];  // 00451529..0045152E (eax, edi, ebx)
        words_form<ProgramEnvWords>(&gl_program_env_parameter_0044f820)(0x5C, mw[0], mw[3], mw[6], px);   // 0045153F
        words_form<ProgramEnvWords>(&gl_program_env_parameter_0044f820)(0x5B, mw[1], mw[4], mw[7], py);   // 00451556
        words_form<ProgramEnvWords>(&gl_program_env_parameter_0044f820)(0x5A, mw[2], mw[5], mw[8], pz);   // 0045156D
        const ProgramParameter4f set = (*cells.programs_0078e5ec != 0)                 // 00451572
            ? *cells.program_env_parameter_007bb850 : *cells.program_parameter_nv_007bb7ac;
        set(0x8620, 0x59, 0.0f, 0.0f, 0.0f, f(kOne));                 // 00451590 / 00451598
    }
    scene_vt(scene)->s124_scene_do_fog(scene, nullptr);                      // 004515A2 Scene::DoFog
    glEnable_0073d390(*cells.gl_blend_0078d448);                       // 004515AE
    glBlendFunc(kBlendSrc_0073f270, kBlendDst_0073f274);               // 004515C2
    glDisable_0073d22c(*cells.gl_stencil_test_0078d470);               // 004515D4
    glDisableClientState(kGlColorArray_0073f3a0);                      // 004515E3
    glDisable_0073d22c(0x0B57);                                        // 004515EA GL_COLOR_MATERIAL
    pass_restart();                                                    // 004515EC
    {
        float w = f(kOne);                                             // [esp+10h]=1.0
        const Vector zero{0.0f, 0.0f, 0.0f};                           // [esp+14h..1Ch]
        glLightModelfv(0x0B53, homogenize_004aaab0(&zero, &w));        // 00451626 / 0045163A GL_LIGHT_MODEL_AMBIENT
    }
    glDepthMask(1);                                                    // 0045163E
    if (*cells.static_geometry_0078e400 != 0) scene_vt(scene)->s0c4_scene_render_static_geometry(scene, nullptr);  // 00451651 RenderStaticGeometry
    {
        float w = f(kOne);                                             // 00451663
        glLightModelfv(0x0B53, homogenize_004aaab0(&ref<Vector>(scene, 0x88), &w));    // 0045166B / 00451679
    }
    glDisableClientState(kGlColorArray_0073f3a0);                      // 00451682
    glDisable_0073d22c(0x0B57);                                        // 00451689
    if (*cells.stencil_pass_007b92e4 != 0) {                           // 00451690
        clear_stencil();
        (void)scene_vt(scene)->s114_scene_do_trigger_bucket(scene, nullptr);                  // 004516CB DoTriggerBucket
        (void)scene_vt(scene)->s118_scene_do_highlight_triggers(scene, nullptr, 0);                  // 004516D7 DoHighlightTriggers(0)
    }
    *cells.pass_ms_007b92b8 = static_cast<float>(pass_elapsed_ms());   // 004516DD..00451703
    pass_restart();                                                    // 00451709
    clear_stencil();                                                   // 00451719..00451740 (unconditional)
    if (ref<U>(scene, 0xD4) != 0 && *cells.shadows_0078e3a8 != 0)    // 00451746 / 00451750
        (void)scene_vt(scene)->s0bc_scene_render_shadows(scene, nullptr, 0, 1, 1, 0);       // 00451765 RenderShadows
    *cells.pass_ms_007b92bc = static_cast<float>(pass_elapsed_ms());   // 0045176B..00451791
    pass_restart();
    if (*cells.dynamic_geometry_0078e3fc != 0) scene_vt(scene)->s0c0_scene_render_dynamic_geometry(scene, nullptr); // 004517B4 RenderDynamicGeometry
    *cells.pass_ms_007b92c4 = static_cast<float>(pass_elapsed_ms());   // 004517BA..004517E0
    pass_restart();
    render_personal_space_00450fa0(scene);                             // 004517F7
    render_paths_004510e0(scene);                                      // 004517FD
    *cells.pass_ms_007b92c0 = static_cast<float>(pass_elapsed_ms());   // 00451805..0045182B
    pass_restart();
    if (*cells.stencil_pass_007b92e4 != 0) {                           // 0045183B
        clear_stencil();
        (void)scene_vt(scene)->s118_scene_do_highlight_triggers(scene, nullptr, 1);                  // 0045187D DoHighlightTriggers(1)
    }
    *cells.pass_ms_007b92b8 = static_cast<float>(pass_elapsed_ms() + static_cast<long double>(*cells.pass_ms_007b92b8)); // 004518A9 fadd
    pass_restart();
    (void)scene_vt(scene)->s110_scene_do_emitter_bucket(scene, nullptr);                      // 004518C9 DoEmitterBucket
    scene_vt(scene)->s0e8_scene_do_light_saber_bucket(scene, nullptr);                       // 004518D3 DoLightSaberBucket
    gl_render_drawable_squares_00432720();                             // 004518D9
    *cells.pass_ms_007b92c8 = static_cast<float>(pass_elapsed_ms());   // 004518DE..00451904
    pass_restart();
    (void)scene_vt(scene)->s0cc_scene_render_motion_blur(scene, nullptr);                       // 0045191E RenderMotionBlur
    (void)scene_vt(scene)->s0e4_scene_do_distortion_buckets(scene, nullptr);                       // 00451928 DoDistortionBuckets
    gl_set_film_noise_texture_00426650(ref<void*>(scene, 0xE4));     // 00451935
    *cells.distortion_texture_007a68d8 = ref<void*>(scene, 0xE8);    // 00451940
    if (ref<U>(scene, 0xD4) != 0)                                    // 0045194F
        gl_frame_buffer_modifications_00437520(*cells.framebuffer_mode_007fbfc4 > 0 ? 0x3Fu : 0x1Fu); // 00451953..00451968
    scene_vt(scene)->s0c8_scene_render_lens_flares(scene, nullptr);                       // 00451974 RenderLensFlares
    if (ref<U>(scene, 0xD4) != 0) {                                  // 0045197A
        glDisable_0073d22c(*cells.gl_depth_test_0078d474);             // 0045198A
        glDisable_0073d22c(*cells.gl_lighting_0078d45c);               // 00451993
        gl_frame_buffer_modifications2_004361f0(0x1F);                 // 00451997
    }
    glDisable_0073d22c(*cells.gl_stencil_test_0078d470);               // 004519A6
    glDisable_0073d22c(*cells.gl_lighting_0078d45c);                   // 004519AE
    glEnable_0073d390(*cells.gl_alpha_test_0078d44c);                  // 004519BD
    glEnable_0073d390(*cells.gl_depth_test_0078d474);                  // 004519C6
    glEnable_0073d390(*cells.gl_blend_0078d448);                       // 004519CE
    glDisable_0073d22c(*cells.gl_fog_0078d450);                        // 004519D7
    glDisableClientState(kGlNormalArray_0073f39c);                     // 004519E0
    glDisableClientState(kGlTexCoordArray_0073f398);                   // 004519E8
    glDisableClientState(kGlVertexArray_0073f3a4);                     // 004519F1
    glDisableClientState(kGlColorArray_0073f3a0);                      // 004519FA
    glDisable_0073d22c(0x0B57);                                        // 00451A01
    *cells.pass_ms_007b92cc = static_cast<float>(pass_elapsed_ms());   // 00451A03..00451A29
    pass_restart();                                                    // 00451A2F
}

// 004A23B0 AurVertexProgramMakeCurrent: inc dword [00830540]; ret
void __cdecl aur_vertex_program_make_current_004a23b0() {
    *cells.vertex_program_epoch_00830540 = *cells.vertex_program_epoch_00830540 + 1;
}

// 0045FD20 AurGetStencilClearValue: cached (1<<(bits-1))-1 in [0078E550]
I __cdecl aur_get_stencil_clear_value_0045fd20() {
    I bits = *cells.stencil_bits_0078e54c;                             // 0045FD20
    if (bits == -1) {                                                  // 0045FD26
        glGetIntegerv(0x0D57, const_cast<GLint*>(reinterpret_cast<volatile GLint*>(cells.stencil_bits_0078e54c))); // 0045FD35
        bits = *cells.stencil_bits_0078e54c;                           // 0045FD3B
    }
    if (bits == 0) return 0;                                           // 0045FD43
    I value = *cells.stencil_clear_0078e550;                           // 0045FD48
    if (value == -1) {                                                 // 0045FD4D
        value = static_cast<I>((1u << static_cast<U>(bits - 1)) - 1u);  // dec ecx; shl; dec
        *cells.stencil_clear_0078e550 = value;                         // 0045FD5B
    }
    return value;
}

// 00450C30 Scene::RenderStaticGeometry (Scene vtbl+C4)
void __fastcall scene_render_static_geometry_00450c30(void* scene, void*) {
    aur_vertex_program_make_current_004a23b0();                        // 00450C37
    void* lights = ref<void*>(scene, 0x80);
    slot<SlotVoid>(lights, 0x08)(lights, nullptr);                     // 00450C44 LightManager vtbl+08
    float material[8];                                                 // [esp+0Ch]..[esp+28h]
    U* bits = reinterpret_cast<U*>(material);
    bits[0] = kOne; bits[1] = kOne; bits[2] = kOne; bits[3] = kOne;    // 00450C5F..00450C77
    bits[4] = 0; bits[5] = 0; bits[6] = 0; bits[7] = kOne;             // 00450C7F..00450C97
    glMaterialfv(kGlFront_0073f2ec, kGlAmbient_0073f2fc, material);    // 00450C9F
    glMaterialfv(kGlFront_0073f2ec, kGlDiffuse_0073f300, material);    // 00450CB3
    glMaterialfv(kGlFront_0073f2ec, kGlEmission_0073f304, material + 4); // 00450CC7
    scene_vt(scene)->s0d8_scene_do_mesh_buckets(scene, nullptr);                       // 00450CCD Scene::DoMeshBuckets
    *cells.pass_ms_007b92b0 = static_cast<float>(pass_elapsed_ms());   // 00450CD3..00450CF9
    pass_restart();                                                    // 00450CFF
    scene_vt(scene)->s0f0_scene_do_bin_bucket(scene, nullptr);                       // 00450D13 Scene::DoBinBucket
    *cells.pass_ms_007b92b4 = static_cast<float>(pass_elapsed_ms());   // 00450D19..00450D3F
    pass_restart();                                                    // 00450D45
}

// 00450B50 Scene::RenderDynamicGeometry (Scene vtbl+C0): stencil setup, then tail jmp vtbl+E0 DoGobBuckets
void __fastcall scene_render_dynamic_geometry_00450b50(void* scene, void*) {
    if (*cells.shadows_0078e3a8 != 0) {                                // 00450B50 / 00450B5A
        const I clear = aur_get_stencil_clear_value_0045fd20();        // 00450B5D
        kotor_renderer_init::glEnable_0073d390(*cells.gl_stencil_test_0078d470);   // 00450B6A
        glStencilMask(0xFF);                                           // 00450B75
        glStencilFunc(kGlAlways_0073f2bc, clear, 0xFFFFFFFFu);         // 00450B85
        glStencilOp(kGlKeep_0073f2c8, kGlKeep_0073f2c8, kGlReplace_0073f2d0); // 00450B99
    }
    aur_vertex_program_make_current_004a23b0();                        // 00450BA0
    scene_vt(scene)->s0e0_scene_do_gob_buckets(scene, nullptr);                       // 00450BAA jmp [eax+0E0h]
}

// 00450D60 Scene::RenderOnlyGobs (Scene vtbl+120)
void __fastcall scene_render_only_gobs_00450d60(void* scene, void*) {
    void* lights = ref<void*>(scene, 0x80);
    slot<SlotVoid>(lights, 0x04)(lights, nullptr);                     // 00450D6C LightManager vtbl+04
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_alpha_test_0078d44c); // 00450D76
    aur_vertex_program_make_current_004a23b0();                        // 00450D7C
    for (I index = 0; index < ref<I>(scene, 0x58); ++index) {        // 00450D81..00450DD4 (count re-read, signed)
        void* gob = ref<void**>(scene, 0x54)[index];
        using NearestLightsWords = void (__fastcall*)(void*, void*, void*, U, U, U, U, U);   // 004A3770, RET 18h
        const U* p = &ref<U>(gob, 0x78);                               // 00450D9D..00450DB2 (mov copies)
        words_form<NearestLightsWords>(&light_manager_get_nearest_lights_004a3770)(ref<void*>(scene, 0x80), nullptr, scene,
            p[0], p[1], p[2], 0x40000000u, 0);                         // 00450DBC (2.0f, 0)
        gob = ref<void**>(scene, 0x54)[index];                       // 00450DC4 reload
        gob_vt(gob)->s010_gob_render(gob, nullptr, 0);                       // 00450DCB Gob::Render(0)
    }
}

// 00457B90 Scene::DoGobBuckets (Scene vtbl+E0)
void __fastcall scene_do_gob_buckets_00457b90(void* scene, void*) {
    using SlotVecU = void (__fastcall*)(void*, void*, const Vector*, U);
    using SlotUU = void (__fastcall*)(void*, void*, U, U);
    using SlotUVecWords = void (__fastcall*)(void*, void*, U, U, U, U);   // lights vtbl+28 (U, Vector by value)
    Vector position;                                                   // [esp+14h]..[esp+1Ch]
    *cells.gob_bucket_flag_007b92fc = 0;                               // 00457BAD
    reinterpret_cast<U*>(&position)[0] = 0; reinterpret_cast<U*>(&position)[1] = 0; reinterpret_cast<U*>(&position)[2] = 0; // 00457BB7..00457BC7
    qsort_006fbe00(*cells.gob_bucket_007fbfcc, static_cast<U>(*cells.gob_bucket_count_007fbfd0), 4, gob_order_cmp_00450270); // 00457BCF (006FBE00, D34)
    *cells.gob_bucket_snapshot_007b92ec = *cells.gob_bucket_count_007fbfd0;   // 00457BDA
    void* lights = ref<void*>(scene, 0x80);
    ref<U>(lights, 0xE0) = 0;                                        // 00457BE6
    ref<U>(lights, 0xEC) = 0;                                        // 00457BF0
    for (I index = *cells.gob_bucket_count_007fbfd0; index != 0;) {    // 00457BFA..00457E77
        --index;                                                       // 00457C10
        (*cells.callback_0078e39c)();                                  // 00457C15 call [0078E39C]
        void* gob = (*cells.gob_bucket_007fbfcc)[index];               // 00457C20
        if (*cells.animations_0078e3b8 != 0 && *cells.interleaved_007b92f4 != 0) {  // 00457C23..00457C33
            if (index >= *cells.gob_bucket_count_007fbfd0) continue;   // 00457C35
            (void)gob_vt(gob)->animate(as_gob(gob), nullptr, *cells.frame_time_0078e574);       // 00457C4B Gob::Animate
            if (index >= *cells.gob_bucket_count_007fbfd0) continue;   // 00457C4E
        }
        if (!gob_visibility_check_004579c0(gob, nullptr)) continue;   // 00457C5C
        if (*cells.depth_toggle_007b92f0 != 0) {                       // 00457C69
            void* model = ref<void*>(gob, 0x58);
            if (model != nullptr && ((ref<U8>(model, 0x50) >> 2) & 1) != 0)       // 00457C72..00457C82
                kotor_renderer_init::glDisable_0073d22c(*cells.gl_depth_test_0078d474);  // 00457C8B
        }
        Vector* gob_position = &ref<Vector>(gob, 0x78);              // ebp
        position = *gob_position;                                      // 00457C94..00457CA8
        lights = ref<void*>(scene, 0x80);
        if (gob_is_in_cutscene_0043e8a0(gob, nullptr)) {               // 00457CAC
            void* part = find_part_0043fe00(static_cast<aurora_model::Part*>(ref<void*>(gob, 0x6C)), "cutscenedummy");  // 00457CC2 (00740C8C)
            if (part == nullptr) {
                slot<SlotVecU>(lights, 0x38)(lights, nullptr, gob_position, ref<U>(gob, 0x138)); // 00457CDE
            } else {
                U guard = *cells.cutscene_guard_007b9440;              // 00457CE6
                if ((guard & 1u) == 0) {                               // static Vector 7B9434 = 0
                    guard |= 1u; *cells.cutscene_guard_007b9440 = guard;
                    U* v = reinterpret_cast<U*>(cells.cutscene_position_007b9434); v[0] = 0; v[1] = 0; v[2] = 0;
                }
                if ((guard & 2u) == 0) {                               // static Quaternion 7B9424 = (1,0,0,0)
                    guard |= 2u; *cells.cutscene_guard_007b9440 = guard;
                    U* q = reinterpret_cast<U*>(cells.cutscene_orientation_007b9424);
                    q[3] = 0; q[2] = 0; q[1] = 0; q[0] = kOne;         // 00457D26..00457D44 (stores 7B9430,742C,7428,7424)
                }
                slot<SlotPartTransform>(part, 0x0C)(part, nullptr, cells.cutscene_position_007b9434, cells.cutscene_orientation_007b9424); // 00457D5C
                lights = ref<void*>(scene, 0x80);
                slot<SlotVecU>(lights, 0x38)(lights, nullptr, cells.cutscene_position_007b9434, ref<U>(gob, 0x138)); // 00457D73
                position = *cells.cutscene_position_007b9434;          // 00457D76..00457D8F
            }
        } else {
            slot<SlotVecU>(lights, 0x38)(lights, nullptr, gob_position, ref<U>(gob, 0x138)); // 00457DA5
        }
        lights = ref<void*>(scene, 0x80);
        slot<SlotVecU>(lights, 0x00)(lights, nullptr, &position, 0);   // 00457DB7
        lights = ref<void*>(scene, 0x80);
        if (ref<U>(gob, 0x178) != 0)                                 // 00457DB9
            slot<SlotUU>(lights, 0x24)(lights, nullptr, 3, ref<U>(gob, 0x178)); // 00457DCE
        else
            slot<SlotU>(lights, 0x20)(lights, nullptr, 3);             // 00457DD7
        if (*cells.focus_lights_007b9300 != 0 &&                       // 00457DDA
            _strnicmp(&ref<char>(gob, 0x38), "c_focusgob", 10) == 0) {   // 00457DEE (007416B8)
            lights = ref<void*>(scene, 0x80);
            const U* gp = reinterpret_cast<const U*>(gob_position);    // the position words, mov-copied (00457DF7..00457E17)
            slot<SlotUVecWords>(lights, 0x28)(lights, nullptr, 3, gp[0], gp[1], gp[2]); // 00457E1A
        }
        kotor_renderer_init::glEnable_0073d390(*cells.gl_blend_0078d448);  // 00457E2E
        glBlendFunc(kBlendSrc_0073f270, kBlendDst_0073f274);           // 00457E3D
        if (*cells.render_gobs_0078e3c4 != 0) gob_vt(gob)->s010_gob_render(gob, nullptr, 0);  // 00457E52 Gob::Render(0)
        if (*cells.depth_toggle_007b92f0 != 0 &&                       // 00457E55 (no null check of gob+58, as original)
            ((ref<U8>(ref<void*>(gob, 0x58), 0x50) >> 2) & 1) != 0)
            kotor_renderer_init::glEnable_0073d390(*cells.gl_depth_test_0078d474);  // 00457E73
    }
    if (*cells.animations_0078e3b8 != 0 && *cells.interleaved_007b92f4 != 0) {  // 00457E7D..00457E8D
        for (I index = ref<I>(scene, 0x58); index != 0;) {           // 00457E8F..00457EB1
            --index;
            if (index < ref<I>(scene, 0x58)) {
                void* object = ref<void**>(scene, 0x54)[index];
                (void)gob_vt(object)->animate(as_gob(object), nullptr, *cells.frame_time_0078e574); // 00457EAC
            }
        }
    }
}

// ================================================================ CRT qsort 006FBE00 / _shortsort 006FBD90 (DECISIONS D34)
namespace {
// Byte swap used inline at 006FBEAE/006FBEE0/006FBF12/006FBF8E/006FBDD3: skipped when a == b.
inline void qsort_swap(char* a, char* b, U width) {
    if (a == b) return;
    for (U n = width; n != 0; --n, ++a, ++b) { const char t = *a; *a = *b; *b = t; }
}
// 006FBD90 _shortsort: hi arrives in EAX, (lo, width, comp) on the stack (cdecl cleanup by caller).
void qsort_shortsort_006fbd90(char* lo, char* hi, U width, int (__cdecl* comp)(const void*, const void*)) {
    while (hi > lo) {                                                  // 006FBD9C jbe
        char* max = lo;                                                // 006FBDA7
        for (char* p = lo + width; p <= hi; p += width)                // 006FBDA2..006FBDC3
            if (comp(p, max) > 0) max = p;                             // 006FBDB2 / 006FBDBD
        if (max != hi && width != 0) qsort_swap(max, hi, width);       // 006FBDC9..006FBDEC
        hi -= width;                                                   // 006FBDF2
    }
}
}

void __cdecl qsort_006fbe00(void* base, U num, U width, int (__cdecl* comp)(const void*, const void*)) {
    char* lostk[30]; char* histk[30];                                  // [esp+20h] / [esp+98h]
    if (num < 2 || width == 0) return;                                 // 006FBE0D jb / 006FBE20
    I stkptr = 0;                                                      // 006FBE35
    char* lo = static_cast<char*>(base);
    char* hi = lo + width * (num - 1);                                 // 006FBE2D..006FBE33
    for (;;) {
        const U size = static_cast<U>(hi - lo) / width + 1;           // 006FBE45..006FBE4D (unsigned div)
        bool pushed_recurse = false;
        if (size <= 8) {                                               // 006FBE4E ja
            qsort_shortsort_006fbd90(lo, hi, width, comp);             // 006FBE5F
        } else {
            char* mid = lo + (size >> 1) * width;                      // 006FBE8F..006FBE94
            if (comp(lo, mid) > 0) qsort_swap(lo, mid, width);        // 006FBE9A
            if (comp(lo, hi) > 0) qsort_swap(lo, hi, width);          // 006FBECC
            if (comp(mid, hi) > 0) qsort_swap(mid, hi, width);        // 006FBEFE
            char* loguy = lo;
            char* higuy = hi;
            for (;;) {                                                 // 006FBF30
                if (mid > loguy) {
                    do { loguy += width; } while (loguy < mid && comp(loguy, mid) <= 0);   // 006FBF34..006FBF48
                }
                if (mid <= loguy) {
                    do { loguy += width; } while (loguy <= hi && comp(loguy, mid) <= 0);  // 006FBF50..006FBF68
                }
                do { higuy -= width; } while (higuy > mid && comp(higuy, mid) > 0);       // 006FBF70..006FBF84
                if (higuy < loguy) break;                              // 006FBF88 ja
                qsort_swap(loguy, higuy, width);                       // 006FBF8C..006FBFB4
                if (mid == higuy) mid = loguy;                         // 006FBFBD..006FBFC5
            }
            higuy += width;                                            // 006FBFCC
            if (mid < higuy) {
                do { higuy -= width; } while (higuy > mid && comp(higuy, mid) == 0);      // 006FBFD2..006FBFE6
            }
            if (mid >= higuy) {
                do { higuy -= width; } while (higuy > lo && comp(higuy, mid) == 0);       // 006FBFF0..006FC008
            }
            if (higuy - lo >= hi - loguy) {                            // 006FC01A jl (signed)
                if (lo < higuy) { lostk[stkptr] = lo; histk[stkptr] = higuy; ++stkptr; }  // 006FC01E..006FC032
                if (loguy < hi) { lo = loguy; pushed_recurse = true; }                    // 006FC036..006FC046
            } else {
                if (loguy < hi) { lostk[stkptr] = loguy; histk[stkptr] = hi; ++stkptr; }  // 006FC04B..006FC05F
                if (lo < higuy) { hi = higuy; pushed_recurse = true; }                    // 006FC063..006FC073
            }
        }
        if (pushed_recurse) continue;
        --stkptr;                                                      // 006FBE67..006FBE70 js
        if (stkptr < 0) return;
        lo = lostk[stkptr]; hi = histk[stkptr];                        // 006FBE76..006FBE8B
    }
}

// ================================================================ core traversal helpers (batch 5)
// 00450270 GobOrderCmp(const Gob** a, const Gob** b): qsort comparator; never returns 0.
int __cdecl gob_order_cmp_00450270(const void* pa, const void* pb) {
    void* a = *static_cast<void* const*>(pa);
    void* b = *static_cast<void* const*>(pb);
    const float da = ref<float>(a, 0xD8), db = ref<float>(b, 0xD8);
    if (da < db) return -1;                                            // 0045027B..00450297 (test ah,5; jp)
    if (da > db) return 1;                                             // 00450299..004502B3 (test ah,41h)
    const U8 fa = ref<U8>(ref<void*>(a, 0x58), 0x50);             // 004502B4 (model+50)
    const U8 fb = ref<U8>(ref<void*>(b, 0x58), 0x50);
    if ((fa & 1) != 0) { if ((fb & 1) == 0) return -1; }               // 004502C2..004502D8
    else if ((fb & 1) != 0) return 1;                                  // 004502D9
    if (*cells.order_by_flag_0078e3b4 != 0) {                          // 004502DD
        const U8 ca = ref<U8>(a, 0x17F), cb = ref<U8>(b, 0x17F);
        if (ca != 0) { if (cb == 0) return -1; }                       // 004502E6..004502F8
        else if (cb != 0) return 1;                                    // 004502FE
    }
    const U8 a2 = (fa >> 2) & 1, b2 = (fb >> 2) & 1;                   // 00450308..0045031D (word load, low byte used)
    if (a2 != 0) { if (b2 == 0) return 1; }                            // 00450320..00450328
    else if (b2 != 0) return -1;                                       // 00450332
    const U8 a4 = (fa >> 4) & 1, b4 = (fb >> 4) & 1;                   // 00450336..00450343
    if (a4 != 0) { if (b4 == 0) return -1; }
    else if (b4 != 0) return 1;
    const U8 a3 = (fa >> 3) & 1, b3 = (fb >> 3) & 1;                   // 0045035A..00450362
    if (a3 != 0) { if (b3 == 0) return -1; }
    else if (b3 != 0) return 1;
    return (ref<float>(a, 0x104) > ref<float>(b, 0x104)) ? 1 : -1; // 0045037B..00450391 (test ah,41h; je -> 1)
}

// 00423B90 vectorSum(out, a, b)
Vector* __cdecl vector_sum_00423b90(Vector* out, const Vector* a, const Vector* b) {
    auto* const resultPointer = out;
    // Keep the original x87 register/m32 operand roles and load every input
    // before the first store: early widening changes signalling-NaN selection.
    __asm {
        mov eax, a
        mov ecx, b
        fld dword ptr [eax+8]
        fadd dword ptr [ecx+8]
        fld dword ptr [eax+4]
        fadd dword ptr [ecx+4]
        fld dword ptr [eax]
        fadd dword ptr [ecx]
        mov eax, resultPointer
        fstp dword ptr [eax]
        fstp dword ptr [eax+4]
        fstp dword ptr [eax+8]
    }
    return out;
}

// 004579C0 Gob::VisibilityCheck -> AL = byte [gob+18]
bool __fastcall gob_visibility_check_004579c0(void* gob, void*) {
    if (*cells.visibility_test_007b92f8 == 0) return true;            // 004579CE / 004579E7
    void* model = ref<void*>(gob, 0x58);
    if (model != nullptr && ((ref<U8>(model, 0x50) >> 6) & 1) != 0) return true;   // 004579E9..004579FA
    if (gob_vt(gob)->s190_gob_is_always_rendering(as_gob(gob), nullptr) != 0) return true;        // 00457A00 Gob::IsAlwaysRendering
    void* camera = *cells.current_camera_007bb4e8;
    Vector delta;                                                      // [esp+0Ch]
    {
        // L3 t1 a2: the three differences stay on the x87 stack and are stored x, y, z (one (float) each, 00457A2E..00457A36).
        const Vector& p = ref<Vector>(gob, 0x78);
        const long double z = W(ref<float>(camera, 0x84)) - W(p.z);   // 00457A0F..00457A18
        const long double y = W(ref<float>(camera, 0x80)) - W(p.y);   // 00457A1F..00457A26
        const long double x = W(ref<float>(camera, 0x7C)) - W(p.x);   // 00457A29..00457A2C
        delta.x = static_cast<float>(x);                               // 00457A2E fstp [esp+10h]
        delta.y = static_cast<float>(y);                               // 00457A32 fstp [esp+14h]
        delta.z = static_cast<float>(z);                               // 00457A36 fstp [esp+18h]
    }
    const long double distance = vector_magnitude_004ab100(&delta);         // 00457A3A
    if (static_cast<long double>(ref<float>(gob, 0x138)) / distance * 2.0 > static_cast<long double>(*cells.visibility_ratio_0078e3c0)) { // 00457A48..00457A59
        ref<U>(gob, 0x1C) = 0;                                       // 00457A5B
        ref<U8>(gob, 0x18) = 1;                                      // 00457A62
        return true;                                                   // 00457A66
    }
    const long double elapsed = Wcell(cells.frame_time_0078e574) + W(ref<float>(gob, 0x1C));   // 00457A80..00457A86
    ref<float>(gob, 0x1C) = S(elapsed);                              // 00457A89 fst (ST0 stays wide)
    if (elapsed > W(*cells.visibility_interval_0078e3bc)) {            // 00457A8C fcomp on the wide value
        alignas(4) unsigned char hit[0x64];                            // [esp+30h]..[esp+93h] (stack bound, inferred)
        ref<U>(gob, 0x1C) = 0;                                       // 00457AAA
        aurora_hit_info_construct_004578f0(hit, nullptr, 0, 0xBFFFFFFFu, 0); // 00457AB1 (0, raw -1.9999999f, 0)
        ref<U>(hit, 0) = 1;                                          // 00457AD4 [esp+3Ch]=1
        Vector zero;                                                   // [esp+18h]..[esp+20h]
        reinterpret_cast<U*>(&zero)[0] = 0; reinterpret_cast<U*>(&zero)[1] = 0; reinterpret_cast<U*>(&zero)[2] = 0;
        Vector offset;                                                 // [esp+24h]
        quaternion_rotate_004a9bb0(&offset, &ref<Quaternion>(gob, 0x84), &ref<Vector>(gob, 0x13C)); // 00457AF4
        vector_sum_00423b90(&delta, &ref<Vector>(gob, 0x78), &offset);   // 00457B00
        void* scene = ref<void*>(gob, 0x68);
        // L3 t1 a2: start = the camera position words, end = the delta words, both copied with mov (00457B1B..00457B4B).
        const U* eye = &ref<U>(*cells.current_camera_007bb4e8, 0x7C);
        const U* end = reinterpret_cast<const U*>(&delta);
        const I blocked = words_form<HitCheckWords>(scene_vt(scene)->s038_scene_hit_check)(scene, nullptr, eye[0], eye[1], eye[2],
            end[0], end[1], end[2], &zero, hit);                       // 00457B4E Scene::HitCheck
        ref<U8>(gob, 0x18) = (blocked == 0) ? 1 : 0;                 // 00457B51 sete
        chit_info_destruct_00456b30(hit, nullptr);                     // 00457B68
    }
    if (ref<U8>(gob, 0x18) == 0) *cells.gob_bucket_flag_007b92fc = *cells.gob_bucket_flag_007b92fc + 1;  // 00457B74 inc [007B92FC]
    return ref<U8>(gob, 0x18) != 0;                                  // 00457B7A mov al,[esi+18h]
}

// 0045F710 AurMultiTextureAvailable: cached (caps & mask 0078E484) == mask in [0078E514]
I __cdecl aur_multi_texture_available_0045f710() {
    U cache = *cells.multitexture_cache_0078e514;                      // 0045F710
    if (cache == 0xFFFFFFFFu) {                                        // 0045F715
        const U mask = *cells.mask_0078e484;
        cache = ((*cells.capabilities_007bb85c & mask) == mask) ? 1u : 0u;  // and/sub/neg/sbb/inc
        *cells.multitexture_cache_0078e514 = cache;                    // 0045F72E
    }
    return static_cast<I>(cache);
}

// 00450DE0 Scene::DoFog (Scene vtbl+124)
void __fastcall scene_do_fog_00450de0(void* scene, void*) {
    using kotor_renderer_init::glEnable_0073d390;
    auto fog_mode = [scene]() {                                        // 00450E0B..00450E2A / 00450ED1..00450EF0
        const U mode = ref<U>(scene, 0x6C);
        const U index = (mode == 0) ? 0u : ((mode != 1) ? 2u : 1u);    // setne cl; inc ecx
        glFogi(0x0B65, static_cast<GLint>(kFogModes_0073f25c[index]));
    };
    if (*cells.force_fog_007b930c == 0) {                              // 00450DE0 / 00450DEE
        if (ref<U>(scene, 0x60) == 0) {                              // 00450DF4
            kotor_renderer_init::glDisable_0073d22c(*cells.gl_fog_0078d450);  // 00450EB1
            return;
        }
        glEnable_0073d390(*cells.gl_fog_0078d450);                     // 00450E05
        fog_mode();
        if (*cells.black_fog_008305e0 != 0) {                          // 00450E30
            // glFogfv reads 4 floats from a 3-float local: the 4th is the caller return address in the
            // original ([esp+14h]); no faithful value exists -> 0 (documented deviation, DECISIONS D2 style).
            float color[4] = {0.0f, 0.0f, 0.0f, 0.0f};                // 00450E44..00450E54
            glFogfv(*cells.gl_fog_color_0078d490, color);              // 00450E69
        } else {
            glFogfv(*cells.gl_fog_color_0078d490, &ref<float>(scene, 0x74));  // 00450E64 (reads scene+74..+80, as original)
        }
        glFogf(*cells.gl_fog_start_0078d484, ref<float>(scene, 0x64));  // 00450E83
        glFogf(*cells.gl_fog_end_0078d488, ref<float>(scene, 0x68));    // 00450E93
        glFogf(*cells.gl_fog_density_0078d48c, ref<float>(scene, 0x70)); // 00450EA2
        return;
    }
    ref<U>(scene, 0x60) = 1;                                         // 00450EBD
    glEnable_0073d390(*cells.gl_fog_0078d450);                         // 00450ECB
    fog_mode();
    {
        float color[4] = {f(kOne), 0.0f, 0.0f, 0.0f};                  // 00450F02..00450F12; 4th = return address in original (deviation)
        glFogfv(*cells.gl_fog_color_0078d490, color);                  // 00450F1A
    }
    set_bits(scene, 0x74, kOne); set_bits(scene, 0x78, 0); set_bits(scene, 0x7C, 0);   // 00450F26..00450F52
    glFogf(*cells.gl_fog_start_0078d484, f(0x3DCCCCCDu));              // 00450F61 (0.1)
    set_bits(scene, 0x64, 0); set_bits(scene, 0x68, 0x41A00000u); set_bits(scene, 0x70, 0x3DCCCCCDu); // 00450F63..00450F71
    glFogf(*cells.gl_fog_end_0078d488, f(0x41A00000u));                // 00450F84 (20.0)
    glFogf(*cells.gl_fog_density_0078d48c, f(0x3DCCCCCDu));            // 00450F91 (0.1)
}

// 00450150 Scene::DoBinBucket (Scene vtbl+F0)
void __fastcall scene_do_bin_bucket_00450150(void* scene, void*) {
    using kotor_renderer_init::glEnable_0073d390;
    if (*cells.bin_bucket_0078e3ac == 0) return;                       // 00450150 / 0045015D
    glEnable_0073d390(*cells.gl_blend_0078d448);                       // 00450169
    glBlendFunc(kBlendSrc_0073f270, kBlendDst_0073f274);               // 0045017D
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_cull_face_0078d454);  // 00450189
    if (aur_multi_texture_available_0045f710() != 0)                   // 0045018F
        (*cells.client_active_texture_007bb6b0)(0x84C0);               // 0045019D
    *cells.current_stage_007a6898 = 0;                                 // 004501A9
    if (*cells.black_fog_008305e0 != 0) glEnableClientState(kGlColorArray_0073f3a0);  // 004501B8
    glDisableClientState(kGlColorArray_0073f3a0);                      // 004501C6
    glColor4f(0.0f, 0.0f, 0.0f, f(kOne));                              // 004501D4
    glAlphaFunc(kGlGreater_0073f2b0, *cells.bin_alpha_ref_0078e3b0);   // 004501ED
    for (I material = 0; material < *cells.grass_materials_00831e48; ++material) {   // 004501EF..00450241
        if (!aur_grass_material_in_use_004a6840(static_cast<U>(material))) continue; // 004501F9
        for (I bin = 0; bin < *cells.bin_count_0080aa6c; ++bin) {      // 00450205..00450237
            void* wind = ref<void*>(scene, 0x84);
            void* camera = *cells.current_camera_007bb4e8;
            void* triangle_bin = (*cells.bins_0080aa68)[bin];
            triangle_bin_render_grass_polys_004a6d30(triangle_bin, nullptr, static_cast<U>(material), &ref<Vector>(camera, 0x7C), wind); // 0045022A
        }
    }
    glAlphaFunc(kGlGreater_0073f2b0, *cells.alpha_ref_007b92e8);       // 00450251
    *cells.texture_flag_0078e330 = 1;                                  // 0045025A
    glEnable_0073d390(*cells.gl_cull_face_0078d454);                   // 00450261
}

// 0046D250 CExoArrayList<int>::List(const CExoArrayList<int>& source): element-wise append copy, RET 4
PointerList* __fastcall int_list_copy_construct_0046d250(PointerList* list, void*, const PointerList* source) {
    list->size = 0; list->capacity = 0; list->data = 0;               // 0046D25B..0046D261
    for (I index = 0; index < source->size; ++index) {                // 0046D263..0046D2E2 (source size re-read)
        const U value = reinterpret_cast<const U*>(static_cast<std::uintptr_t>(source->data))[index]; // 0046D270
        if (list->size == list->capacity) {                            // 0046D27D
            const I capacity = list->capacity != 0 ? list->capacity + list->capacity : 8;
            U* previous = reinterpret_cast<U*>(static_cast<std::uintptr_t>(list->data));
            list->capacity = capacity;                                 // 0046D299 (before new)
            list->data = static_cast<U>(reinterpret_cast<std::uintptr_t>(live_operator_new(static_cast<U>(capacity) * 4u))); // 0046D29C
            for (I n = 0; n < list->size; ++n)                         // 0046D2A9..0046D2BE
                reinterpret_cast<U*>(static_cast<std::uintptr_t>(list->data))[n] = previous[n];
            if (previous != nullptr) live_free(previous);              // 0046D2C5
        }
        reinterpret_cast<U*>(static_cast<std::uintptr_t>(list->data))[list->size] = value;   // 0046D2D6
        list->size = list->size + 1;                                   // 0046D2D9
    }
    return list;
}

namespace {
// DoMeshBuckets local animated-object list (data [esp+20h], size [esp+24h], capacity [esp+28h]).
struct LocalList { U* data; I size; I capacity; };
// 00455852..004558B0 / 0045594C..004559AB: growth (capacity before new, no null check), copy, free old, append.
inline void local_append(LocalList& list, U value) {
    if (list.size == list.capacity) {
        const I capacity = list.capacity != 0 ? list.capacity + list.capacity : 8;
        list.capacity = capacity;
        U* previous = list.data;
        U* fresh = static_cast<U*>(live_operator_new(static_cast<U>(capacity) * 4u));
        list.data = fresh;
        for (I n = 0; n < list.size; ++n) fresh[n] = previous[n];
        if (previous != nullptr) live_free(previous);
    }
    list.data[list.size] = value;
    list.size = list.size + 1;
}
inline I local_count(const LocalList& list, U value) {               // 0045583B..0045584C / 00455925..0045593C
    I matches = 0;
    for (I n = 0; n < list.size; ++n) if (list.data[n] == value) ++matches;
    return matches;
}
inline void part_prepare(void* part) {                               // part vtbl+60(), vtbl+64(0)
    slot<SlotVoid>(part, 0x60)(part, nullptr);
    slot<SlotU>(part, 0x64)(part, nullptr, 0);
}
}

// 00455710 Scene::DoMeshBuckets (Scene vtbl+D8)
void __fastcall scene_do_mesh_buckets_00455710(void*, void*) {
    LocalList animated{nullptr, 0, 0};                                 // 0045572C..00455738 (EH state 0)
    PointerList* parts = cells.part_list_00827f40;
    for (I index = 0; index < parts->size; ++index) {                  // 0045573C..00455787 (size re-read)
        void* part = reinterpret_cast<void**>(static_cast<std::uintptr_t>(parts->data))[index];
        void* material = ref<void*>(part, 0x44);
        material_bind_texture0_0047adf0(material, nullptr);            // 0045575F
        material_set_blending_mode_0047af70(material, nullptr, 0);     // 0045576A
        part_prepare(part);                                            // 00455773 / 0045577C
    }
    PointerList ordering;                                              // [esp+2Ch] (EH state 1)
    int_list_copy_construct_0046d250(&ordering, nullptr, aur_texture_get_ordering_00421970()); // 00455789 / 00455793
    PointerList* buckets = cells.mesh_buckets_008194e0;                // 12-byte {data,size,capacity} entries
    for (I k = 0; k < ordering.size; ++k) {                            // 00455798..004558DE
        const U bucket_index = reinterpret_cast<U*>(static_cast<std::uintptr_t>(ordering.data))[k];  // 004557B4
        PointerList* bucket = &buckets[bucket_index];                  // lea ebx,[eax+eax*2]; shl 2
        if (bucket->size == 0) continue;                               // 004557C3
        void* first = reinterpret_cast<void**>(static_cast<std::uintptr_t>(bucket->data))[0];
        void* material = ref<void*>(first, 0x44);
        material_bind_texture0_0047adf0(material, nullptr);            // 004557DC
        material_set_blending_mode_0047af70(material, nullptr, 0);     // 004557E5
        for (I j = 0; j < bucket->size; ++j) {                         // 004557F2..004558C9 (size re-read)
            void* part = reinterpret_cast<void**>(static_cast<std::uintptr_t>(bucket->data))[j];
            part_prepare(part);                                        // 00455821 / 0045582A
            const U owner = ref<U>(part, 0x80);                      // 0045582D
            if (owner == 0) continue;                                  // 00455839
            if (local_count(animated, owner) != 0) continue;           // 0045584E
            local_append(animated, owner);
        }
    }
    PointerList* bucket0 = &buckets[0];                                // 008194E0 / 008194E4
    for (I j = 0; j < bucket0->size; ++j) {                            // 004558E4..004559BB
        void* part = reinterpret_cast<void**>(static_cast<std::uintptr_t>(bucket0->data))[j];
        if (j == 0) material_bind_texture0_0047adf0(ref<void*>(part, 0x44), nullptr);    // 0045590B / 00455910
        part_prepare(part);                                            // 00455919 / 00455922
        if (local_count(animated, ref<U>(part, 0x80)) != 0) continue;   // 00455925..00455940
        local_append(animated, ref<U>(part, 0x80));                  // 00455942..004559AB (no null test here)
    }
    glEnableClientState(kGlTexCoordArray_0073f398);                    // 004559C7
    for (I n = 0; n < animated.size; ++n) {                            // 004559CD..004559F2
        void* object = reinterpret_cast<void*>(static_cast<std::uintptr_t>(animated.data[n]));
        if (object == nullptr) continue;                               // 004559D6
        ref<U>(object, 0x50) = ref<U>(object, 0x54);               // 004559DA..004559DD
        ref<U>(object, 0x8C) = kMeshResetValue_00740a70;             // 004559E3..004559E9 (.rdata 00740A70 = 4)
    }
    if (ordering.data != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(ordering.data))); // 00455A02
    if (animated.data != nullptr) live_free(animated.data);            // 00455A17
}

// 00444FE0 CExoArrayList<int>::Add(value), RET 4
// [M45 compose] 00444FE0 body removed: existing M44 provider gui_viewport_list::add_00444fe0 (viewport-add) wins; name joined in m45-joins (definition removed; declaration kept)
void __fastcall int_list_add_00444fe0(PointerList* list, void*, U value);

// 0046F060 GLRender::Transform(position, orientation, force)
void __cdecl gl_transform_0046f060(const Vector* position, const Quaternion* orientation, U force) {
    const I mode = static_cast<I>(*cells.matrix_view_0078d494);        // 0046F060
    if (mode == 0 && force == 0) return;                               // 0046F06A / 0046F072
    *cells.transform_count_00827fa8 = *cells.transform_count_00827fa8 + 1;   // 0046F078..0046F081
    if (mode > 0) {                                                    // 0046F087 jle
        make_matrix4x4_004aa9c0(cells.transform_matrix_008280a0, position, orientation);   // 0046F098
        glMultMatrixf(cells.transform_matrix_008280a0);                // 0046F0A5
        return;
    }
    U guard = *cells.transform_guard_0082809c;
    if ((guard & 1u) == 0) {                                           // 0046F0B4 static Vector 828090 = 0
        guard |= 1u; *cells.transform_guard_0082809c = guard;
        U* v = reinterpret_cast<U*>(cells.transform_position_00828090); v[0] = 0; v[1] = 0; v[2] = 0;
    }
    if ((guard & 2u) == 0) {                                           // 0046F0DE static Quaternion 828080 = (1,0,0,0)
        guard |= 2u; *cells.transform_guard_0082809c = guard;
        U* q = reinterpret_cast<U*>(cells.transform_orientation_00828080); q[3] = 0; q[2] = 0; q[1] = 0; q[0] = kOne;
    }
    const U* pw = reinterpret_cast<const U*>(position);
    const U px = pw[0], py = pw[1], pz = pw[2];                        // 0046F112..0046F126 (mov)
    U* cp = reinterpret_cast<U*>(cells.transform_position_00828090); cp[0] = px; cp[1] = py; cp[2] = pz;   // 0046F118..0046F12C
    words_form<GlWords3>(&glTranslatef)(px, py, pz);                   // 0046F129..0046F132 (push edx/ecx/eax)
    {   const U* ow = reinterpret_cast<const U*>(orientation);         // 0046F138..0046F163 (mov)
        U* co = reinterpret_cast<U*>(cells.transform_orientation_00828080);
        const U w = ow[0], x = ow[1], y = ow[2], z = ow[3];
        co[0] = w; co[1] = x; co[2] = y; co[3] = z; }
    Vector axis;
    quaternion_axis_00423c40(cells.transform_orientation_00828080, nullptr, &axis);   // 0046F169
    // L3 t1 a2: real L1 acos(long double), then 0046F188 fadd st,st ; 0046F18B fmul dword [00742EF8] ; fstp dword.
    const long double arc = acos(W(cells.transform_orientation_00828080->w));   // 0046F16E fld ; 0046F183 call 006FC0E0
    const U degrees = stored_bits((arc + arc) * f(kRadToDeg_00742ef8));     // 0046F188..0046F191 fstp [esp]
    const U* ax = reinterpret_cast<const U*>(&axis);
    words_form<GlWords4>(&glRotatef)(degrees, ax[0], ax[1], ax[2]);         // 0046F174..0046F194 (axis pushed as dwords)
}

// 00473EA0 Gob::PartDraw(part, flag), RET 8. The original inlines two recursion levels (00473F74.., 00474031..)
// and recurses at 004740D6; this is the recursive source form with the same order of effects.
void __fastcall gob_part_draw_00473ea0(void* gob, void*, void* part, U flag) {
    if (ref<U8>(part, 0x2C) == 0) return;                            // 00473EA8
    // L3 t1 a2: saved and restored as three dwords (the original's mov copies; a Vector copy is three float
    // registers spilled as 4-byte values across the draw calls, S1, and quiets a signalling NaN).
    U saved[3];
    std::memcpy(saved, cells.part_draw_vector_0078e8ac, 12);           // 00473EB7..00473ED0
    glPushMatrix();                                                    // 00473ED4
    if (*cells.transform_enabled_0078e5f0 != 0)                        // 00473EDA
        gl_transform_0046f060(&ref<Vector>(part, 0x08), &ref<Quaternion>(part, 0x14), 0);   // 00473EED
    const U scale = ref<U>(part, 0x24);                              // 00473EF5 mov edx,[ebp+24h]
    words_form<GlWords3>(&glScalef)(scale, scale, scale);              // 00473EFA..00473F01 (nested levels: _glScalef 00459150)
    const bool skip = slot<SlotI>(part, 0x2C)(part, nullptr) != 0 &&   // 00473F0C
                      ref<void*>(part, 0x04) != nullptr &&           // 00473F13
                      ref<U8>(ref<void*>(part, 0x04), 0x189) == 0; // 00473F1A
    if (!skip) {
        slot<SlotVoid>(part, 0x60)(part, nullptr);                     // 00473F29
        slot<SlotU>(part, 0x64)(part, nullptr, flag);                  // 00473F36
    }
    if (*cells.part_depth_0078e5e0 != 0) {                             // 00473F39
        *cells.part_depth_0078e5e0 = *cells.part_depth_0078e5e0 - 1;   // 00473F49
        for (I index = 0; index < ref<I>(part, 0x34); ++index)       // 00473F4F..00474160 (count re-read)
            gob_part_draw_00473ea0(gob, nullptr, ref<void**>(part, 0x30)[index], flag);
        *cells.part_depth_0078e5e0 = *cells.part_depth_0078e5e0 + 1;   // 00474168
    }
    glPopMatrix();                                                     // 0047416F
    std::memcpy(cells.part_draw_vector_0078e8ac, saved, 12);           // 00474175..0047418D
}

// 00474C90 Gob::ProxyPartDraw(part_ignored, flag), RET 8: always draws from gob+6C.
void __fastcall gob_proxy_part_draw_00474c90(void* gob, void*, void*, U flag) {
    using kotor_renderer_init::glEnable_0073d390;
    void* proxy = ref<void*>(gob, 0x170);
    if (proxy == nullptr || !(ref<float>(proxy, 0x94) < f(kOne))) {  // 00474C9E / 00474CAA (test ah,5; jp)
        gob_part_draw_00473ea0(gob, nullptr, ref<void*>(gob, 0x6C), flag);   // 00474DF9
        return;
    }
    glEnable_0073d390(*cells.gl_fog_0078d450);                         // 00474CC3
    const float fade = ref<float>(proxy, 0x94);
    const float square = S(W(fade) * W(fade));                         // 00474CE0 fstp [esp+10h]
    void* scene = ref<void*>(gob, 0x68);
    // 4th component read by glFogfv is the return address slot in the original ([esp+1Ch]): deviation -> 0.
    float color[4] = {S(W(square) * W(ref<float>(scene, 0x74))), S(W(square) * W(ref<float>(scene, 0x78))),
                      S(W(square) * W(ref<float>(scene, 0x7C))), 0.0f};   // 00474D0A..00474D12; 4th: D43
    glFogfv(*cells.gl_fog_color_0078d490, color);                      // 00474D16
    glFogf(*cells.gl_fog_density_0078d48c, S(W(ref<float>(ref<void*>(gob, 0x68), 0x70)) * 2.0));   // 00474D2A fadd st,st; fstp
    glFogf(*cells.gl_fog_start_0078d484, S(W(ref<float>(ref<void*>(gob, 0x68), 0x64)) * W(ref<float>(ref<void*>(gob, 0x170), 0x94)))); // 00474D3F..00474D4C
    glFogf(*cells.gl_fog_end_0078d488, static_cast<float>(static_cast<long double>(ref<float>(ref<void*>(gob, 0x68), 0x68)) *
        static_cast<long double>(ref<float>(ref<void*>(gob, 0x170), 0x94)) + static_cast<long double>(f(0x3DCCCCCDu))));   // 00474D6B (+[0073E798] 0.1)
    gob_part_draw_00473ea0(gob, nullptr, ref<void*>(gob, 0x6C), flag);   // 00474D82
    if (ref<U>(ref<void*>(gob, 0x68), 0x60) == 0)                  // 00474D8F
        kotor_renderer_init::glDisable_0073d22c(*cells.gl_fog_0078d450);    // 00474D97
    glFogf(*cells.gl_fog_density_0078d48c, ref<float>(ref<void*>(gob, 0x68), 0x70));  // 00474DAE
    glFogf(*cells.gl_fog_start_0078d484, ref<float>(ref<void*>(gob, 0x68), 0x64));    // 00474DC1
    glFogf(*cells.gl_fog_end_0078d488, ref<float>(ref<void*>(gob, 0x68), 0x68));      // 00474DD3
    glFogfv(*cells.gl_fog_color_0078d490, &ref<float>(ref<void*>(gob, 0x68), 0x74));  // 00474DE3
}

// 00478260 Gob::Render(propagate), RET 4 (Gob vtable 00741268 slot+10)
void __fastcall gob_render_00478260(void* gob, void*, U propagate) {
    auto keep_transform = [gob]() {                                    // 00478300..00478338 / 0047855E..00478596
        ref<Vector>(gob, 0x94) = ref<Vector>(gob, 0x78);
        ref<Quaternion>(gob, 0xA0) = ref<Quaternion>(gob, 0x84);
    };
    if (*cells.render_enabled_0078e860 == 0) return;                   // 0047826E / 00478285
    if (ref<void*>(gob, 0x6C) == nullptr) { keep_transform(); return; }   // 0047828E
    if (*cells.render_flagged_0078e85c == 0) {                         // 00478292
        void* model = ref<void*>(gob, 0x58);
        if (model != nullptr && ((ref<U8>(model, 0x50) >> 2) & 1) != 0) { keep_transform(); return; }  // 0047829B..004782E8
    }
    if (ref<U8>(gob, 0x166) == 0 && *cells.render_all_00828024 == 0) { keep_transform(); return; }   // 004782ED..004782FE
    if (*cells.fade_test_0082806c != 0 &&                              // 00478340
        ((ref<U8>(ref<void*>(gob, 0x58), 0x50) >> 3) & 1) != 0 &&  // 0047834D..00478359
        *cells.active_scene_007b92a0 != nullptr) {                     // 0047835F
        // L3 t1 a2: eye and target are raw words (the original's mov copies 0047836C..0047839F); Gob::HitCheck receives
        // them as stack words (004783B4..004783D7) and shortens the target local in place (&target, 004783AF).
        U eye[3];
        copy_words3(eye, atf(*cells.current_camera_007bb4e8, 0x7C));   // 0047836C..0047837A (edi, ebp, ebx)
        U target[3];
        copy_words3(target, atf(*cells.active_scene_007b92a0, 0xA8)); // 0047837D..0047839F ([esp+10h]..[esp+18h])
        alignas(4) unsigned char hit[0x64];                            // [esp+28h] (stack bound, inferred)
        aurora_hit_info_construct_004578f0(hit, nullptr, 0, 0xBFFFFFFFu, 0); // 004783A3
        ref<U8>(hit, 0x5D) = 0;                                      // 004783E7
        const I blocked = words_form<HitCheckWords>(gob_vt(gob)->s08c_gob_hit_check)(gob, nullptr, eye[0], eye[1], eye[2],
            target[0], target[1], target[2], reinterpret_cast<Vector*>(target), hit);  // 004783EF Gob::HitCheck
        const long double step = Wcell(cells.frame_time_0078e574) / static_cast<long double>(*cells.fade_time_0078e5a0); // 004783F5..004783FD
        const long double faded = (blocked != 0) ? static_cast<long double>(ref<float>(gob, 0xDC)) - step   // 00478405 fsubr [esi+DCh]
                                                 : step + static_cast<long double>(ref<float>(gob, 0xDC));  // 0047840D fadd [esi+DCh]
        ref<float>(gob, 0xDC) = static_cast<float>(faded);           // 00478413 fstp
        // 00478419..00478478: upper = (1 < v) ? 1 : v (test 5; jp: NaN keeps v); lower > upper (test 41h; jne) ? lower :
        // the same choice re-read from [esi+DCh]; every candidate is a float loaded with fld and stored with fstp.
        const long double one = W(f(kOne));
        const long double value = W(ref<float>(gob, 0xDC));
        const long double upper = (one < value) ? one : value;          // 00478419..0047843A
        const long double lower = W(*cells.fade_minimum_0078e858);      // 0047843A fld [0078E858]
        const long double again = W(ref<float>(gob, 0xDC));
        ref<float>(gob, 0xDC) = static_cast<float>((lower > upper) ? lower : ((one < again) ? one : again));   // 00478478 fstp
        chit_info_destruct_00456b30(hit, nullptr);                     // 00478489
    }
    (void)gob_vt(gob)->do_event(as_gob(gob), nullptr, "GetPersonalRadius", word(atf(gob, 0x108)));   // 0047849E (00742F30)
    (void)gob_vt(gob)->do_event(as_gob(gob), nullptr, "GetCreatureRadius", word(atf(gob, 0x10C)));   // 004784B4 (00742F1C)
    (void)gob_vt(gob)->do_event(as_gob(gob), nullptr, "GetPath", word(atf(gob, 0x110)));             // 004784CA (00742F14)
    *cells.gob_render_count_00827fac = *cells.gob_render_count_00827fac + 1;            // 004784D0
    ref<U8>(gob, 0x13) = 1;                                          // 004784D6
    glPushMatrix();                                                    // 004784DA
    if (*cells.transform_enabled_0078e5f0 != 0)                        // 004784E0
        gl_transform_0046f060(&ref<Vector>(gob, 0x78), &ref<Quaternion>(gob, 0x84), 1);   // 004784F6
    const U scale_bits = ref<U>(gob, 0x1B4);
    if (scale_bits != kOne) {                                          // 00478504 (bitwise compare with 3F800000h)
        const float scale = fbits(scale_bits);
        glScalef(scale, scale, scale);                                 // 00478512
    }
    if (ref<U8>(gob, 0x164) != 0) {                                  // 00478518
        material_bind_texture0_0047adf0(ref<void*>(gob, 0x168), nullptr);   // 00478528
        material_set_blending_mode_0047af70(ref<void*>(gob, 0x168), nullptr, 0);   // 00478535
    }
    gob_proxy_part_draw_00474c90(gob, nullptr, ref<void*>(gob, 0x6C), 0);  // 00478542
    if (ref<U8>(gob, 0x155) != 0)                                    // 00478547
        int_list_add_00444fe0(cells.deferred_gobs_007fbf90, nullptr, static_cast<U>(reinterpret_cast<std::uintptr_t>(gob)));  // 00478557
    else
        keep_transform();                                              // 0047855E..00478596
    if (*cells.debug_spheres_00828060 == 1) {                          // 00478599
        float color[3] = {f(kOne), f(kOne), 0.0f};                     // [esp+1Ch..24h] after pushes
        // L3 t1 a2: the radius argument is fld [esi+138h] ; fstp dword [esp] (004785A2 / 004785BC), an x87 copy.
        gl_draw_sphere_bound_00431a50(S(W(ref<float>(gob, 0x138))), atf(gob, 0x13C), color, f(kOne), 1);  // 004785D7
    }
    if (*cells.debug_frames_0082805c != 0 || ref<U16>(gob, 0xC8) != 0)   // 004785DF..004785F0
        gl_draw_axis_aligned_frame_0046ef10(atf(gob, 0x120), atf(gob, 0x12C), atf(gob, 0x148));   // 00478607
    glPopMatrix();                                                     // 0047860F
    if (static_cast<U8>(propagate) != 0)                               // 00478615 byte test of the argument
        (void)gob_vt(gob)->do_event(as_gob(gob), nullptr, "propagaterender", 0);   // 0047862B (00742F04)
}

// ================================================================ core batch 8 (menu-frame reached Gob/Scene slots)
// 0043F0C0 Gob::SetPosition(Vector by value) -> hidden return, Gob vtbl+5C, RET 10h
Vector* __fastcall gob_set_position_0043f0c0(aurora_model::Gob* gob, void*, Vector* out, Vector) {
    const U* p = A2_STACK_ARGS() + 1;                                  // position words [esp+8]..[esp+10h]
    copy_words3(atf(gob, 0x78), p);                                    // 0043F0C4..0043F0D7 (mov)
    copy_words3(out, p);                                               // 0043F0DF..0043F0E6 (mov)
    return out;                                                        // 0043F0DA mov eax,[esp+8]
}

// 0043F1D0 Gob::SetBehavior(behavior), Gob vtbl+58, RET 4: replace, then tail-call old behavior's deleting dtor(1)
U __fastcall gob_set_behavior_0043f1d0(aurora_model::Gob* gob, void*, void* behavior) {
    void* previous = ref<void*>(gob, 0x188);                         // 0043F1D6
    ref<void*>(gob, 0x188) = behavior;                               // 0043F1DE
    if (previous == nullptr) return word(gob);                         // 0043F1E4 (EAX = this from 0043F1D4)
    return slot<SlotDelete>(previous, 0x00)(previous, nullptr, 1);     // 0043F1E8 [esp+4]=1; jmp [eax]
}

// 0043E8A0 Gob::IsInCutscene: mov al,[ecx+14h]
bool __fastcall gob_is_in_cutscene_0043e8a0(void* gob, void*) { return field<U8>(gob, 0x14) != 0; }

// 0043E8B0 Gob::IsAlwaysRendering (Gob vtbl+190): mov al,[ecx+12h]
U8 __fastcall gob_is_always_rendering_0043e8b0(aurora_model::Gob* gob, void*) { return field<U8>(gob, 0x12); }

// 004402F0 Gob::GetPart(name) (Gob vtbl+10C), RET 4: "root" -> root part, else FindPart(root, name)
void* __fastcall gob_get_part_004402f0(aurora_model::Gob* gob, void*, const char* name) {
    const int differs = _stricmp("root", name);                        // 004402FE (00740C00)
    void* root = ref<void*>(gob, 0x6C);                              // 00440308
    if (differs == 0) return root;                                     // 0044030B
    return find_part_0043fe00(static_cast<aurora_model::Part*>(root), name);   // 0044030F
}

// 00486560 Gob::DoEvent: provided by lane-3d-model (D52), @gob_do_event_00486560@16 (Gob vtbl+16C).

// 00455670 Scene::ChildAdd(gob) (Scene vtbl+B0), RET 4
U __fastcall scene_child_add_00455670(void* scene, void*, void* gob) {
    if (ref<void*>(gob, 0x58) != nullptr) {                          // 00455675
        scene_bsp_add_gob_0046c420(scene, gob);                        // 00455682
        scene_list_special_parts_r_004531a0(scene, nullptr, ref<void*>(gob, 0x6C));  // 00455690
    }
    int_list_add_00444fe0(&ref<PointerList>(scene, 0x54), nullptr, word(gob));   // 00455695..004556BF (same growth/store sequence)
    const U animated = gob_vt(gob)->s160_gob_return_zero(as_gob(gob), nullptr);    // 004556C6 vtbl+160
    if (animated != 0) int_list_add_00444fe0(&ref<PointerList>(scene, 0xEC), nullptr, word(gob)); // 004556D0..00455703
    return animated;                                                   // EAX = vtbl+160 result
}

// 00453900 Scene::ChildRemove(gob) (Scene vtbl+B4), RET 4
U __fastcall scene_child_remove_00453900(void* scene, void*, void* gob) {
    if (ref<void*>(gob, 0x58) != nullptr) {                          // 00453905
        scene_bsp_remove_gob_0046c4a0(scene, gob);                     // 00453911
        unlist_special_parts_00453550(scene, nullptr, ref<void*>(gob, 0x6C));   // 0045391F
    }
    ref<PointerList>(scene, 0x54).RemoveReverse32(word(gob));        // 00453928 (lane-aurres 00445080)
    const U animated = gob_vt(gob)->s160_gob_return_zero(as_gob(gob), nullptr);    // 00453931
    if (animated != 0) ref<PointerList>(scene, 0xEC).RemoveReverse32(word(gob));  // 00453942
    return animated;
}

// 00432720 GLRender::RenderDrawableSquares
void __cdecl gl_render_drawable_squares_00432720() {
    kotor_renderer_init::glDisable_0073d22c(0x0B50);                   // 0043272C
    kotor_renderer_init::glDisable_0073d22c(0x0DE1);                   // 00432733
    kotor_renderer_init::glDisable_0073d22c(0x0B71);                   // 0043273A
    for (I index = 0; index < cells.drawable_squares_007a693c->size; ++index)   // 0043273C..00432760 (size re-read)
        render_squares_00426000(reinterpret_cast<U*>(static_cast<std::uintptr_t>(cells.drawable_squares_007a693c->data))[index]);  // 00432750
    kotor_renderer_init::glEnable_0073d390(0x0B71);                    // 0043276D
    kotor_renderer_init::glEnable_0073d390(0x0B50);                    // 00432774
}

// 00450440 Scene::DoLightSaberBucket (Scene vtbl+E8)
void __fastcall scene_do_light_saber_bucket_00450440(void* scene, void*) {
    using SlotVecU = void (__fastcall*)(void*, void*, const Vector*, U);
    using SlotUU = void (__fastcall*)(void*, void*, U, U);
    kotor_renderer_init::glEnable_0073d390(*cells.gl_blend_0078d448);  // 0045044A
    for (I index = 0; index < cells.saber_bucket_007fbff0->size; ++index) {   // 00450450..004504BE
        void* gob = reinterpret_cast<void**>(static_cast<std::uintptr_t>(cells.saber_bucket_007fbff0->data))[index];
        Vector* position = &ref<Vector>(gob, 0x78);                  // 00450478
        void* lights = ref<void*>(scene, 0x80);
        slot<SlotVecU>(lights, 0x38)(lights, nullptr, position, ref<U>(gob, 0x138));   // 0045047C
        lights = ref<void*>(scene, 0x80);
        slot<SlotVecU>(lights, 0x00)(lights, nullptr, position, 0);   // 0045048A
        lights = ref<void*>(scene, 0x80);
        if (ref<U>(gob, 0x178) != 0) slot<SlotUU>(lights, 0x24)(lights, nullptr, 3, ref<U>(gob, 0x178));  // 004504A1
        else slot<SlotU>(lights, 0x20)(lights, nullptr, 3);            // 004504AA
        gob_vt(gob)->s010_gob_render(gob, nullptr, 0);                 // 004504B3 Gob::Render(0)
    }
}

// ================================================================ WindManager (per frame on the menu: Scene::Render 0045142D)
// 004A57C0 WindManager::fillNoiseField(float* field16x16), RET 4. Random values come from the one shared original
// CRT rand port crt_rand_006fc447 (lane-3d-model, D52).
void __fastcall wind_manager_fill_noise_field_004a57c0(void* wind, void*, float* field) {
    (void)wind;
    std::memset(field, 0, 0x100 * 4);                                  // 004A57C7..004A57D7 rep stos 100h dwords
    I step = 0x10;                                                     // 004A57D2 / [esp+14h]
#pragma clang loop unroll(disable)
    for (;;) {
        if (step <= 0x10) {                                            // 004A57E5 jg
            I row_start = 0, row_offset = 0;                           // [esp+10h] / [esp+18h]
            const I divisor = 500 / (0x10 / step);                     // 004A57F4..004A580A (idiv twice)
#pragma clang loop unroll(disable)
            for (I row_end = step; ; ) {                               // ebp
                I col_start = 0;                                       // esi
#pragma clang loop unroll(disable)
                for (I col_end = step; ; ) {                           // edi
                    const I r = crt_rand_006fc447() % divisor;         // 004A5814 call 006FC447 (D52: lane-3d-model port); idiv remainder
                    const float value = S(static_cast<long double>(r) * W(f(kMilli_0073d708)));  // fild; fmul; fstp [esp+20h]
                    if (row_start < row_end) {                         // 004A581E jge
                        float* row = field + row_offset + col_start;   // 004A583E lea edx,[ecx+eax*4]
                        // L3 t1 a2: loops kept loops (the original's 004A5847..004A5874 / 004A5850..004A586E), so the one
                        // in-loop fstp appears once (clang unrolled the inner loop into extra round trips, S3).
#pragma clang loop unroll(disable)
                        for (I rows = row_end - row_start; rows != 0; --rows, row += 0x10) {   // 004A5847..004A5874
#pragma clang loop unroll(disable)
                            for (I c = 0; c < col_end - col_start; ++c)                        // 004A5850..004A586E
                                row[c] = S(W(value) + W(row[c]));     // fld value; fadd [eax-4]; fstp
                        }
                    }
                    col_start = col_end;                               // 004A587A
                    col_end += step;                                   // 004A587C
                    if (col_end > 0x10) break;                         // 004A587E jle
                }
                row_start = row_end;                                   // 004A5885
                row_offset = row_end << 4;                             // 004A5889
                row_end += step;                                       // 004A588C
                if (row_end > 0x10) break;                             // 004A588E jle
            }
        }
        step = step / 2;                                               // 004A589B..004A58A0 (cdq; sub; sar 1)
        if (step < 1) return;                                          // 004A58A2 jge
    }
}

// 004A5E30 WindManager::updateTimer(raw float delta), RET 4
void __fastcall wind_manager_update_timer_004a5e30(void* wind, void*, U delta_bits) {
    const long double delta = W(fbits(delta_bits));                 // 004A5E30 fld [esp+4]
    const long double timer = delta + W(ref<float>(wind, 0x844));         // 004A5E3A
    ref<float>(wind, 0x844) = S(timer);                              // 004A5E43 fst (ST stays wide)
    if (timer > W(ref<float>(wind, 0x848))) {                        // 004A5E49 fcomp; test ah,41h
        const U next = ref<U>(wind, 0x834);                          // 004A5E56
        const U current = ref<U>(wind, 0x830);
        ref<U>(wind, 0x830) = next;                                  // 004A5E62
        ref<U>(wind, 0x834) = current;                               // 004A5E6B
        wind_manager_fill_noise_field_004a57c0(wind, nullptr, reinterpret_cast<float*>(static_cast<std::uintptr_t>(current))); // 004A5E71
        ref<U>(wind, 0x844) = 0;                                     // 004A5E76
    }
    ref<float>(wind, 0x840) = S(W(ref<float>(wind, 0x844)) / W(ref<float>(wind, 0x848)));   // 004A5E7C..004A5E8B
    const float vx = S(delta * W(ref<float>(wind, 0x18)));           // 004A5E91..004A5E98
    const float vy = S(delta * W(ref<float>(wind, 0x1C)));           // 004A5EA0..004A5EA7
    const float vz = S(delta * W(ref<float>(wind, 0x20)));           // 004A5EAB..004A5EBB
    ref<float>(wind, 0x24) = vx; ref<float>(wind, 0x28) = vy; ref<float>(wind, 0x2C) = vz;  // 004A5EB6..004A5EC3
    for (I index = 0; index < ref<I>(wind, 0x10); ++index) {         // 004A5EC6..004A5F37 (count re-read)
        unsigned char* entries = ref<unsigned char*>(wind, 0x0C);
        float& remaining = *reinterpret_cast<float*>(entries + index * 0x18 + 0x10);
        remaining = S(W(remaining) - delta);                           // 004A5ED6..004A5EE2
        if (ref<float>(ref<unsigned char*>(wind, 0x0C) + index * 0x18, 0x10) < 0.0f) {   // 004A5EE7..004A5EF6 (remove when < 0)
            const I count = ref<I>(wind, 0x10) - 1;                  // 004A5EF8..004A5F02
            ref<I>(wind, 0x10) = count;
            for (I j = index; j < count; ++j) {                        // 004A5F05..004A5F20
                unsigned char* base = ref<unsigned char*>(wind, 0x0C) + j * 0x18;
                std::memcpy(base, base + 0x18, 0x18);                  // rep movs 6 dwords (forward)
            }
            --index;                                                   // 004A5F26 dec ebp (revisit slot)
        }
    }
}

// 00421970 AurTextureGetOrdering: rebuild the id list 007A478C from the ordered texture list, return it.
PointerList* __cdecl aur_texture_get_ordering_00421970() {
    PointerList* ids = cells.texture_ids_007a478c;                     // data 7A478C / size 7A4790 / capacity 7A4794
    ids->size = 0;                                                     // 0042197B
    *cells.texture_max_id_007a46bc = 0;                                // 00421980
    PointerList* ordered = cells.texture_ordered_007a47a4;             // data 7A47A4 / size 7A47A8
    for (I i = 0; i < ordered->size; ++i) {                            // 00421971..00421AAF (size re-read)
        void* texture = reinterpret_cast<void**>(static_cast<std::uintptr_t>(ordered->data))[i];   // 004219A6
        if (ref<U8>(texture, 0xE1) != 0) continue;                   // 004219A9
        for (I k = 0; k < static_cast<I>(ref<std::int16_t>(texture, 0xC4)) * static_cast<I>(ref<std::int16_t>(texture, 0xC2)); ++k) {  // movsx; imul (re-evaluated)
            const I id = ref<I*>(texture, 0x40)[k];                  // 004219D5
            if (id >= 0x1388 || id <= 0) continue;                     // 004219D8 jge / 004219E6 jle (signed)
            if (ids->size == ids->capacity) {                          // 004219F1
                const I capacity = ids->capacity != 0 ? ids->capacity + ids->capacity : 8;
                U* previous = reinterpret_cast<U*>(static_cast<std::uintptr_t>(ids->data));
                ids->capacity = capacity;                              // 00421A0C (before new)
                ids->data = word(live_operator_new(static_cast<U>(capacity) * 4u));   // 00421A15 / 00421A20
                for (I n = 0; n < ids->size; ++n)                      // 00421A2A..00421A45
                    reinterpret_cast<U*>(static_cast<std::uintptr_t>(ids->data))[n] = previous[n];
                if (previous != nullptr) live_free(previous);          // 00421A4C
            }
            reinterpret_cast<U*>(static_cast<std::uintptr_t>(ids->data))[ids->size] = static_cast<U>(id);   // 00421A5F
            ids->size = ids->size + 1;                                 // 00421A6F
            const U reread = static_cast<U>(ref<I*>(texture, 0x40)[k]);  // 00421A75..00421A78
            if (reread > *cells.texture_max_id_007a46bc) *cells.texture_max_id_007a46bc = reread;   // 00421A7B jbe (unsigned)
        }
    }
    return ids;                                                        // 00421AB9 mov eax,7A478Ch
}

// 0046F6B0 Transform(pos, quat, force): if [0078E5F0] tail-jump GLRender::Transform, else return (cdecl)
void __cdecl transform_0046f6b0(const Vector* position, const Quaternion* orientation, U force) {
    if (*cells.transform_enabled_0078e5f0 != 0)                        // 0046F6B0..0046F6B7
        gl_transform_0046f060(position, orientation, force);           // 0046F6B9 jmp
}

// 00450FA0 RenderPersonalSpace(scene) (cdecl): debug spheres for flagged objects; depth test off around the loop
void __cdecl render_personal_space_00450fa0(void* scene) {
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_depth_test_0078d474);   // 00450FAA
    for (I index = 0; index < ref<I>(scene, 0x58); ++index) {        // 00450FB4..004510BA (count re-read)
        void* gob = ref<void**>(scene, 0x54)[index];
        if (*cells.personal_space_all_007b9314 == 0 && ref<U8>(gob, 0x154) == 0) continue;   // 00450FD0..00450FE7
        if (!(ref<float>(gob, 0x108) > 0.0f)) continue;              // 00450FED..00450FFE (test ah,41h)
        glPushMatrix();                                                // 00451004
        transform_0046f6b0(&ref<Vector>(gob, 0x78), &ref<Quaternion>(gob, 0x84), 1);   // 00451013
        float color[3]; Vector center;                                 // [esp+10h] / [esp+1Ch]
        reinterpret_cast<U*>(&center)[0] = 0; reinterpret_cast<U*>(&center)[1] = 0; reinterpret_cast<U*>(&center)[2] = 0;
        color[0] = f(kOne); color[1] = f(0x3F19999Au); color[2] = f(kOne); // 00451046..00451056 (1, 0.6, 1)
        gl_draw_sphere_bound_00431a50(f(kMilli_0073d708), &center, color, f(kOne), 0);       // 0045105E (radius raw 3A83126F)
        using SphereWords = void (__cdecl*)(U, const void*, const float*, U, U);
        words_form<SphereWords>(&gl_draw_sphere_bound_00431a50)(raw(atf(gob, 0x108)), &center, color, kOne, 0);   // 00451063 mov ; 0045107B
        // (the third radius, +10Ch, is fld/fstp-copied by the original: 00451080 / 0045109B)
        color[1] = 0.0f;                                               // 0045109E [esp+28h]=0
        gl_draw_sphere_bound_00431a50(S(W(ref<float>(gob, 0x10C))), &center, color, f(kOne), 0); // 00451080..004510A6 (x87 copy)
        glPopMatrix();                                                 // 004510AE
    }
    kotor_renderer_init::glEnable_0073d390(*cells.gl_depth_test_0078d474);    // 004510C9
}

// 004510E0 RenderPaths(scene) (cdecl): debug line strips of each object's path (+114 coord count, +118 points)
void __cdecl render_paths_004510e0(void* scene) {
    if (*cells.render_paths_007b9318 == 0) return;                     // 004510E1..004510E8
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_depth_test_0078d474);   // 004510FC
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_lighting_0078d45c);     // 00451105
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_texture_2d_0078d458);   // 0045110E
    glColor3f(f(0x3F19999Au), f(kOne), f(kOne));                       // 0045111F (0.6, 1, 1)
    for (I index = 0; index < ref<I>(scene, 0x58); ++index) {        // 00451129..004511BB
        void* gob = ref<void**>(scene, 0x54)[index];
        glBegin(3);                                                    // 0045114E GL_LINE_STRIP
        for (I point = 0; point < ref<I>(gob, 0x114) / 3; ++point) { // 00451154..004511A1 (signed /3 via 55555556h, re-evaluated)
            const float* v = reinterpret_cast<const float*>(ref<unsigned char*>(gob, 0x118) + point * 0x0C);
            const U* vw = reinterpret_cast<const U*>(v);
            words_form<GlWords3>(&glVertex3f)(vw[0], vw[1], vw[2]);    // 00451176..00451185 (mov pushes)
        }
        glEnd();                                                       // 004511A7
    }
    kotor_renderer_init::glEnable_0073d390(*cells.gl_lighting_0078d45c);      // 004511CC
    kotor_renderer_init::glEnable_0073d390(*cells.gl_texture_2d_0078d458);    // 004511D4
    kotor_renderer_init::glEnable_0073d390(*cells.gl_depth_test_0078d474);    // 004511DD
}

// 00456B30 CHitInfo::~CHitInfo: unregister from the owner list (+38), free the result buffer (+8)
void __fastcall chit_info_destruct_00456b30(void* hit, void*) {
    void* owner = ref<void*>(hit, 0x38);                             // 00456B33
    if (owner != nullptr)                                              // 00456B3B
        ref<PointerList>(owner, 0x04).RemoveReverse32(word(atf(hit, 0x38)));   // 00456B41 Remove([+38]+4, &hit+38)
    void* data = ref<void*>(hit, 0x08);                              // 00456B46
    if (data != nullptr) live_free(data);                              // 00456B4E
}

// 00457EC0 Scene::RenderLensFlares (Scene vtbl+C8). Locals are named by their entry-relative slot (E-xx, espmap.py).
void __fastcall scene_render_lens_flares_00457ec0(void* scene, void*) {
    using kotor_renderer_init::glEnable_0073d390;
    using kotor_renderer_init::glDisable_0073d22c;
    if (*cells.lens_flares_0078e3f0 == 0) return;                      // 00457EDB / 00457EEC
    PointerList lights;                                                // E-7C (data) / E-78 (size)
    int_list_copy_construct_0046d250(&lights, nullptr,
        light_manager_get_lens_flare_lights_004a28b0(ref<void*>(scene, 0x80), nullptr));   // 00457EF8 / 00457F05
    U guard = *cells.flare_guard_007b9458;
    if ((guard & 1u) == 0) {                                           // 00457F0A
        *cells.flare_guard_007b9458 = guard | 1u;                      // 00457F1A
        *cells.flare_pool_a_007b9454 = aurora_pool_request_00423ee0(0, 0x60, 0x42, 0x1011, cells.flare_pool_a_007b9454); // 00457F38 / 00457F40
    }
    glDisable_0073d22c(*cells.gl_lighting_0078d45c);                   // 00457F59
    glDisable_0073d22c(*cells.gl_depth_test_0078d474);                 // 00457F62
    glEnable_0073d390(*cells.gl_blend_0078d448);                       // 00457F71
    glBlendFunc(kGlOne_0073f278, kGlOne_0073f278);                     // 00457F7A
    glEnable_0073d390(*cells.gl_texture_2d_0078d458);                  // 00457F86
    if (ref<U>(scene, 0x60) != 0) glDisable_0073d22c(*cells.gl_fog_0078d450);   // 00457F8C..00457F98
    glMatrixMode(kGlModelview_0073f294); glPushMatrix(); glLoadIdentity();          // 00457FA6..00457FB4
    glMatrixMode(kGlModelview_0073f298); glPushMatrix(); glLoadIdentity();          // 00457FBD..00457FC5
    glTranslatef(0.0f, 0.0f, f(0xC0000000u));                          // 00457FCE (0, 0, -2)
    // L3 t1 a2 (writing rule, re-derived from 00457FD4..004586F3): register values are long double; each original
    // fstp/fst dword is one (float) conversion at that point; values the original only copies with mov (the HitCheck
    // vectors, the divisor word, tx / ty, the colour words, the size) are held as raw words, so no float value is kept
    // in an x87 register across the GL / pool calls (was: 4-byte spills, S1) and no copy goes through fld/fstp.
    using DivideWords = Vector* (__cdecl*)(Vector*, const Vector*, U);   // 00437A90 operator/(Vector, float)
    using GlTranslateWords = void (__stdcall*)(U, U, U);
    for (I i = 0; i < lights.size; ++i) {                              // 00457FD4..004586F3 (E-AC)
        void* light = reinterpret_cast<void**>(static_cast<std::uintptr_t>(lights.data))[i];  // ebx (E-D8)
        void* camera = *cells.current_camera_007bb4e8;
        const Vector* light_position = &ref<Vector>(light, 0x94);    // edi
        Vector delta;                                                  // E-BC..E-B4 ([esp+50h])
        {
            const long double z = W(ref<float>(light, 0x9C)) - W(ref<float>(camera, 0x84));   // 00457FFF..00458005
            const long double y = W(light_position->y) - W(ref<float>(camera, 0x80));          // 00458011..00458018
            const long double x = W(light_position->x) - W(ref<float>(camera, 0x7C));          // 0045801E..00458020
            delta.x = static_cast<float>(x);                           // 00458028 fstp
            delta.y = static_cast<float>(y);                           // 0045802C fstp
            delta.z = static_cast<float>(z);                           // 00458030 fstp
        }
        // 00458034..00458047: fcomp [flare+50h] ; test ah,41h ; je -> next light when the magnitude is greater (ordered)
        if (static_cast<long double>(vector_magnitude_004ab100(&delta)) > W(ref<float>(ref<void*>(light, 0x04), 0x50))) continue;
        if (*cells.flare_hit_test_0078e3e4 != 0) {                     // 0045804D
            U g = *cells.flare_guard_007b9458;
            if ((g & 2u) == 0) {                                       // 00458059 static Vector 7B9448 = 0
                *cells.flare_guard_007b9458 = g | 2u;
                U* v = reinterpret_cast<U*>(cells.flare_hit_vector_007b9448); v[0] = 0; v[1] = 0; v[2] = 0;
            }
            void* memory = live_operator_new(0x64);                    // 00458089
            void* hit = (memory != nullptr) ? aurora_hit_info_construct_004578f0(memory, nullptr, 0, 0xBFFFFFFFu, 0) : nullptr;  // 004580AA
            ref<U>(hit, 0x14) = 0;                                   // 004580BE (null hit faults, as original)
            ref<U>(hit, 0x58) = 1;                                   // 004580C1
            const U* lp = reinterpret_cast<const U*>(light_position);
            I blocked;
            if (*cells.flare_hit_order_0078e3ec != 0) {                // 004580D8 (camera -> light): end = light words
                const U ex = lp[0], ey = lp[1], ez = lp[2];            // 004580E4..004580F3
                const U* eye = &ref<U>(*cells.current_camera_007bb4e8, 0x7C);   // 004580F6..0045812C
                blocked = words_form<HitCheckWords>(scene_vt(scene)->s038_scene_hit_check)(scene, nullptr, eye[0], eye[1], eye[2],
                    ex, ey, ez, cells.flare_hit_vector_007b9448, hit);
            } else {                                                   // 00458100 (light -> camera): end = camera words
                const U* eye = &ref<U>(*cells.current_camera_007bb4e8, 0x7C);
                const U ex = eye[0], ey = eye[1], ez = eye[2];         // 00458100..00458115
                blocked = words_form<HitCheckWords>(scene_vt(scene)->s038_scene_hit_check)(scene, nullptr, lp[0], lp[1], lp[2],
                    ex, ey, ez, cells.flare_hit_vector_007b9448, hit);  // 0045812C
            }
            chit_info_destruct_00456b30(hit, nullptr);                 // 0045812F..00458155 (inlined dtor)
            live_free(hit);                                            // 00458159
            const long double step = W(*cells.flare_fade_rate_0078e3e8) * Wcell(cells.frame_time_0078e574);  // 0045815E..00458164
            if (blocked != 0) {                                        // 0045816D
                long double faded = W(ref<float>(light, 0x88)) - step;    // 00458171 fsubr
                if (!(faded > 0.0)) faded = W(f(kFloatZero_0073d700));      // 00458177..00458186 (test 41h; je keeps > 0)
                ref<float>(light, 0x88) = static_cast<float>(faded);  // 0045818C fst
                if (faded == 0.0) continue;                            // 00458194 fcomp 0; test ah,44h; jnp -> next light
            } else {
                long double faded = step + W(ref<float>(light, 0x88));    // 004581A7 fadd
                if (!(faded < 1.0)) faded = W(f(kOne));                // 004581AD..004581BC (test 5; jnp keeps < 1)
                ref<float>(light, 0x88) = static_cast<float>(faded);  // 004581C2 fstp
            }
        }
        // 004581CA: light direction in camera space -> screen-plane projection point
        camera = *cells.current_camera_007bb4e8;
        void* flare = ref<void*>(light, 0x04);                       // esi
        Vector to_light;                                               // E-64..E-5C
        {
            const long double z = W(light_position->z) - W(ref<float>(camera, 0x84));   // 004581CF..004581D2
            const long double y = W(light_position->y) - W(ref<float>(camera, 0x80));   // 004581DB..004581E5
            const long double x = W(light_position->x) - W(ref<float>(camera, 0x7C));   // 004581EC..004581F3
            to_light.x = static_cast<float>(x);                        // 004581FF fstp
            to_light.y = static_cast<float>(y);                        // 00458206 fstp
            to_light.z = static_cast<float>(z);                        // 0045820D fstp
        }
        Quaternion inverse;                                            // E-1C
        quaternion_negate_w_004a9b50(&inverse, &ref<Quaternion>(camera, 0x88));   // 00458214
        Vector view;                                                   // E-F4
        quaternion_rotate_004a9bb0(&view, &inverse, &to_light);        // 00458222
        const U distance_bits = stored_bits(vector_magnitude_004ab100(&view));   // 0045822C / 00458231 fstp E-B0
        const long double distance = fbits(distance_bits);
        Vector direction;                                              // E-34 -> copied to E-F4
        const Vector* divided = words_form<DivideWords>(&vector_divide_00437a90)(&direction, &view, distance_bits);   // 00458235..00458247 (mov push)
        copy_words3(&view, divided);                                   // 0045824C..00458264
        // 00458259..0045827B: (y + x) * 0.0 - z * 1.0 (fmul [0073D700] ; fmul [0073D764] ; fsubp)
        long double facing = (W(view.y) + W(view.x)) * W(f(kFloatZero_0073d700)) - W(view.z) * W(f(kOne));
        if (!(facing > 0.0)) facing = W(f(kFloatZero_0073d700));            // 0045827D..0045828C (test 41h; je keeps > 0)
        const long double facing_squared = S(facing * facing);         // 00458296..004582A8 fstp (E-C4)
        Vector origin; Vector plane_normal;                            // E-70 / E-58
        reinterpret_cast<U*>(&origin)[0] = 0; reinterpret_cast<U*>(&origin)[1] = 0; reinterpret_cast<U*>(&origin)[2] = 0;
        reinterpret_cast<U*>(&plane_normal)[0] = 0; reinterpret_cast<U*>(&plane_normal)[1] = 0; reinterpret_cast<U*>(&plane_normal)[2] = kMinusOneImm;
        Vector hit_point;                                              // E-28
        const Vector* projected = plane_line_intersection_004ab250(&hit_point, &plane_normal, f(0x40000000u), &origin, &view);  // 00458300 (d = 2.0)
        copy_words3(&view, projected);                                 // 00458305..00458315
        Vector screen;                                                 // E-F4 after 0045832E..00458360
        {
            const long double nz = -W(view.z);                         // 00458319..0045831D
            const long double ny = -W(view.y);                         // 00458322..00458326
            const long double nx = -W(view.x);                         // 00458328..0045832C
            screen.x = static_cast<float>(nx);                         // 0045832E fstp [esp+80h] (-> [esp+18h] by mov)
            screen.y = static_cast<float>(ny);                         // 00458340 fstp [esp+84h]
            screen.z = static_cast<float>(nz);                         // 00458352 fstp [esp+88h]
        }
        if (ref<I>(flare, 0x58) == 0) continue;                      // 00458364 / 00458369
        for (I j = 0; j < ref<I>(flare, 0x58); ++j) {                // 0045836F..004586DB (count re-read)
            void* texture = ref<void**>(flare, 0x54)[j];
            slot<SlotVoid>(texture, 0xB4)(texture, nullptr);           // 0045839D CAurTexture::Bind (lane-upload texture_bind_004214b0)
            const long double position = W(ref<float*>(flare, 0x6C)[j]);  // 004583A6
            const float* color = reinterpret_cast<const float*>(ref<unsigned char*>(flare, 0x78) + j * 0x0C);   // 004583B1..004583B6
            const long double fade = W(ref<float>(light, 0x88));          // E-C0 ([esp+4Ch], a mov copy of +88)
            const U ty = stored_bits(W(screen.y) * position);          // 004583AD..004583C2 fstp [esp+28h]
            const U tx = stored_bits(position * W(screen.x));          // 004583C6..004583CA fstp [esp+0C0h]
            const long double cz = W(color[2]) + W(ref<float>(light, 0x54));   // 004583D1..004583D4
            const long double cy = W(color[1]) + W(ref<float>(light, 0x50));   // 004583D7..004583DA
            const long double cx = S(W(color[0]) + W(ref<float>(light, 0x4C)));   // 004583DD..004583E2 fstp [esp+0CCh]
            const long double bz = S(cz * fade);                       // 004583EB..004583EF fstp [esp+38h]
            const long double gy = cy * fade;                          // 004583F3
            const long double rx = cx * fade;                          // 004583F7..004583FE
            const long double blue_k = S(bz * facing_squared);         // 00458402..0045840A fstp [esp+38h]
            const long double green_k = S(gy * facing_squared);        // 0045840E..00458414 fstp [esp+10h]
            const long double red_k = rx * facing_squared;             // 00458418
            const long double base_reg = W(ref<float>(flare, 0x50));
            const long double spread = base_reg - distance;            // 0045841C..0045841F
            const long double spread_stored = S(spread);               // 00458423 fst [esp+8Ch]
            const U blue = stored_bits(blue_k * spread);               // 0045842A..00458430 fstp [esp+64h]
            const U green = stored_bits(green_k * spread);             // 00458434..00458442 fstp [esp+38h]
            const U red = stored_bits(red_k * spread);                 // 0045844A..00458452 fstp [esp+6Ch]
            volatile U rgb[3];                                         // [esp+3Ch] r / [esp+40h] g / [esp+44h] b (frame words: stored once, re-read per use)
            const long double base = W(ref<float>(flare, 0x50));       // 00458458
            if (base == 0.0) {                                         // 0045845B fcom 0; test ah,44h; jnp (equal)
                rgb[0] = red; rgb[1] = green; rgb[2] = blue;           // 0045849C..004584B2 mov copies
            } else {
                const long double inverse_base = S(W(f(kOne)) / base); // 00458468..00458470 fstp [esp+10h]
                const long double nb = inverse_base * fbits(blue);     // 00458476..0045847A
                const long double ng = inverse_base * fbits(green);    // 0045847E..00458482
                const long double nr = inverse_base * fbits(red);      // 00458486..0045848A
                rgb[0] = stored_bits(nr);                              // 0045848E fstp [esp+3Ch]
                rgb[1] = stored_bits(ng);                              // 00458492 fstp [esp+40h]
                rgb[2] = stored_bits(nb);                              // 00458496 fstp [esp+44h]
            }
#pragma clang loop unroll(disable)
            for (int c = 0; c < 3; ++c) {                              // 004584B6..0045854A clamp to [0,1]
                if (W(fbits(rgb[c])) < 0.0) rgb[c] = 0;                // fcomp 0; test 5; jp (less, ordered) -> mov 0
                else if (W(fbits(rgb[c])) > 1.0) rgb[c] = kOne;        // fcomp 1; test 41h; jne (greater, ordered) -> mov 1.0
            }
            const U size = stored_bits(W(ref<float*>(flare, 0x60)[j]) / W(ref<float>(flare, 0x50)) * spread_stored
                                       * W(*cells.flare_size_scale_0078e3e0));   // 00458552..00458568 fstp [esp+10h]
            glPushMatrix();                                            // 0045856C
            words_form<GlTranslateWords>(&glTranslatef)(tx, ty, 0u);   // 00458572..00458580 (mov pushes)
            U g = *cells.flare_guard_007b9458;
            if ((g & 4u) == 0) {                                       // 00458586
                *cells.flare_guard_007b9458 = g | 4u;
                *cells.flare_pool_b_007b9444 = aurora_pool_request_00423ee0(0, 0x60, 0x42, 0x1011, cells.flare_pool_b_007b9444); // 004585AD
            }
            unsigned char* vertex = static_cast<unsigned char*>(aurora_pool_lock_00423f10(*cells.flare_pool_b_007b9444, 0, 0, 1));  // 004585CD
            const I r = x87_round(S(W(fbits(rgb[0])) * W(f(kByte_0073ede4))));   // 004585D2..004585ED (fstp; fld; frndint; fistp)
            const I gr = x87_round(S(W(fbits(rgb[1])) * W(f(kByte_0073ede4))));  // 004585EF..00458607
            const I b = x87_round(S(W(fbits(rgb[2])) * W(f(kByte_0073ede4))));   // 00458609..00458621
            const U packed = (((static_cast<U>(b) | 0xFFFFFF00u) << 8 | static_cast<U>(gr)) << 8) | static_cast<U>(r);   // 00458623..0045863D
            const long double size_reg = fbits(size);
            vertex += 0x10;                                            // 00458641
#pragma clang loop unroll(disable)
            for (U corner = 0; corner < 4; ++corner, vertex += 0x18) { // 00458644..004586A6
                *reinterpret_cast<U*>(vertex - 4) = packed;            // 00458646 colour
                *reinterpret_cast<U*>(vertex) = (corner == 0 || corner == 3) ? 0u : kOne;               // 00458649..00458661 u (fld 0 / 1 ; fstp)
                *reinterpret_cast<U*>(vertex + 4) = (corner < 2) ? 0u : kOne;                           // 0045865E..00458675 v
                *reinterpret_cast<float*>(vertex - 0x10) = static_cast<float>((corner == 0 || corner == 3) ? -size_reg : size_reg);   // 00458673..0045868E x
                *reinterpret_cast<float*>(vertex - 0x0C) = static_cast<float>((corner < 2) ? -size_reg : size_reg);                   // 00458691..00458699 y
                *reinterpret_cast<U*>(vertex - 8) = 0;                 // 0045869C z
            }
            (void)aurora_pool_unlock_00423f30(*cells.flare_pool_b_007b9444);   // 004586AF
            gl_render_objects_vtc_quad_00431db0(*cells.flare_pool_b_007b9444, 4); // 004586BC
            glPopMatrix();                                             // 004586C4
        }
    }
    glBlendFunc(kBlendSrc_0073f270, kBlendDst_0073f274);               // 00458713
    glEnable_0073d390(*cells.gl_depth_test_0078d474);                  // 0045871F
    glEnable_0073d390(*cells.gl_lighting_0078d45c);                    // 00458728
    if (ref<U>(scene, 0x60) != 0) glEnable_0073d390(*cells.gl_fog_0078d450);    // 0045872A..00458739
    glMatrixMode(kGlModelview_0073f298); glPopMatrix();                // 00458742 / 0045874A
    glMatrixMode(kGlModelview_0073f294); glPopMatrix();                // 00458753 / 00458755
    if (lights.data != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(lights.data)));   // 0045875E..0045876E
}

// 0046B7C0 ConstructCameraFadePlane(scene). Original convention: scene in EAX (0046B7C7 mov esi,eax), RET 0.
// Exported as cdecl(scene) per lane-3d-bsp (its naked construct_camera_fade_plane_eax_entry_0046b7c0 adapts EAX callers).
void __cdecl construct_camera_fade_plane_0046b7c0(void* scene) {
    // L3 t1 a2 (writing rule, re-derived from 0046B7C0..0046BBBC; B = esp after `push esi`): register values are long
    // double; each of the original's 28 fstp/fst dword stores is one (float) conversion at that point. A stored value
    // the original reloads later (fld / fsub / fmul from its slot) is held as its stored word (stored_bits) and read
    // back with fbits, so the conversion is the only x87 round trip of that store (a float-typed stack local added a
    // second, store-to-slot round trip); values only passed by pointer are stored as floats.
    void* camera = *cells.current_camera_007bb4e8;                     // 0046B7C9
    void* behavior = (camera != nullptr) ? ref<void*>(camera, 0x18C) : nullptr;      // 0046B7D2
    // L3 t1 a2: the behaviour slot +1C answers the follow behaviour (0 = none); the original passes that answer, not the
    // behaviour, to GetTargetWorldPosition (0046B7E1 test eax,eax ; 0046B7FE mov ecx,eax).
    void* follow = (behavior != nullptr)
        ? reinterpret_cast<void*>(static_cast<std::uintptr_t>(static_cast<U>(slot<SlotI>(behavior, 0x1C)(behavior, nullptr)))) : nullptr;  // 0046B7DE
    if (follow == nullptr) {
        ref<U>(scene, 0xB8) = kFadeOff_00741568;                     // 0046B7E5..0046B7EB
        return;
    }
    Vector target;                                                     // B+68h
    behavior_camera_follow_get_target_world_position_0049f0e0(follow, nullptr, &target);   // 0046B7FE..0046B800
    camera = *cells.current_camera_007bb4e8;
    Vector eye;                                                        // B+40h
    camera_vt(camera)->s020_camera_get_position(camera, nullptr, &eye);               // 0046B812 Camera::GetPosition
    volatile U delta[3];                                               // B+28h..B+30h (frame words)
    delta[0] = stored_bits(W(target.x) - W(eye.x));                    // 0046B815..0046B826 fstp
    delta[1] = stored_bits(W(target.y) - W(eye.y));                    // 0046B82B..0046B833 fstp
    delta[2] = stored_bits(W(target.z) - W(eye.z));                    // 0046B837..0046B83F fstp
    Vector unit;                                                       // B+1Ch..B+24h
    normalize_004ab180(&unit, reinterpret_cast<const Vector*>(const_cast<U*>(delta))); // 0046B843
    // 0046B848..0046B85D: uy*uy + ux*ux ; fsqrt (L1 sqrt on the register value)
    const long double length = sqrt(W(unit.y) * W(unit.y) + W(unit.x) * W(unit.x));
    const U length_stored = stored_bits(length);                       // 0046B85F fst [B+80h] (reloaded 0046BADA)
    if (length < kFadeMinimum_00742ef0) {                              // 0046B866 fcomp qword 0.4 (the unrounded ST0); test ah,5; jp
        ref<U>(scene, 0xB8) = kFadeOff_00741568;                     // 0046B873
        return;
    }
    reinterpret_cast<U*>(&unit)[2] = 0;                                // 0046B894 B+24h = 0
    delta[2] = 0;                                                      // 0046B89C B+30h = 0
    U guard = *cells.fade_plane_guard_00827f74;
    if ((guard & 1u) == 0) {                                           // 0046B887..0046B8A4 static Vector 827F68 = (0,0,1)
        *cells.fade_plane_guard_00827f74 = guard | 1u;
        U* v = reinterpret_cast<U*>(cells.fade_plane_up_00827f68); v[0] = 0; v[1] = 0; v[2] = kOne;
    }
    U side[3];                                                         // B+4h..B+0Ch
    side[0] = stored_bits(-W(unit.y) * W(*cells.fade_plane_width_0078e5c0));          // 0046B8CA..0046B8EC fchs ; fmul ; fstp
    side[1] = stored_bits(W(unit.x) * W(*cells.fade_plane_width_0078e5c0));           // 0046B8D5..0046B8F6
    side[2] = stored_bits(W(*cells.fade_plane_width_0078e5c0) * W(f(kFloatZero_0073d700)));   // 0046B8FA..0046B906
    U other[3];                                                        // B+10h..B+18h
    other[0] = stored_bits(-W(fbits(side[0])));                        // 0046B90A..0046B910
    other[1] = stored_bits(-W(fbits(side[1])));                        // 0046B914..0046B91A
    other[2] = stored_bits(-W(fbits(side[2])));                        // 0046B91E..0046B924
    Vector cross_in;                                                   // B+4Ch..B+54h
    {
        const long double keep = W(f(kOne)) - W(*cells.fade_plane_fraction_0078e5bc);   // 0046B928..0046B92E
        const long double dx = W(fbits(delta[0])) * keep;              // 0046B934..0046B938
        const U dy = stored_bits(W(fbits(delta[1])) * keep);           // 0046B93A..0046B940 fstp [B+38h]
        const U dz = stored_bits(keep * W(f(kFloatZero_0073d700)));    // 0046B944..0046B94C fstp [B+3Ch] (constant 0.0)
        cross_in.x = S(dx - W(fbits(side[0])));                        // 0046B950..0046B954
        cross_in.y = S(W(fbits(dy)) - W(fbits(side[1])));              // 0046B958..0046B960
        cross_in.z = S(W(fbits(dz)) - W(fbits(side[2])));              // 0046B964..0046B96C
    }
    Vector crossed, normal1;                                           // B+58h / B+74h
    vector_cross_004ab210(&crossed, cells.fade_plane_up_00827f68, &cross_in);    // 0046B970
    normalize_004ab180(&normal1, &crossed);                            // 0046B97E
    Plane plane1;                                                      // B+94h..B+A0h
    {
        const long double px = W(fbits(side[0])) + W(eye.x);          // 0046B983..0046B987
        const long double py = W(fbits(side[1])) + W(eye.y);          // 0046B992..0046B99D
        const long double pz = W(fbits(side[2])) + W(eye.z);          // 0046B9A8..0046B9B3
        copy_words3(&plane1, &normal1);                                // 0046B98B..0046B9C7 (mov)
        plane1.d = S(-((W(normal1.z) * pz + W(normal1.y) * py) + px * W(normal1.x)));   // 0046B9BE..0046B9E8
    }
    Vector cross_in2;                                                  // B+4Ch (copied by mov from B+4h..B+0Ch)
    {
        const long double keep = W(f(kOne)) - W(*cells.fade_plane_fraction_0078e5bc);   // 0046B9F8..0046B9FE
        const long double dx = W(fbits(delta[0])) * keep;              // 0046BA04..0046BA08
        const U dy = stored_bits(W(fbits(delta[1])) * keep);           // 0046BA0A..0046BA10 fstp [B+5Ch]
        const U dz = stored_bits(keep * W(fbits(delta[2])));           // 0046BA14..0046BA1A fstp [B+60h] (the zeroed delta.z)
        U in[3];
        in[0] = stored_bits(dx - W(fbits(other[0])));                  // 0046BA1E..0046BA22 fstp [B+4h] -> mov
        in[1] = stored_bits(W(fbits(dy)) - W(fbits(other[1])));        // 0046BA2A..0046BA3B fstp [B+8h] -> mov
        in[2] = stored_bits(W(fbits(dz)) - W(fbits(other[2])));        // 0046BA3F..0046BA57 fstp [B+0Ch] -> mov
        copy_words3(&cross_in2, in);                                   // 0046BA26..0046BA5F
    }
    Vector normal2;                                                    // B+34h
    // L3 t1 a2: the operands are (cross_in2, up) here (0046B9EF push 827F68h last argument; 0046BA3A push &cross_in2),
    // the reverse of the first plane's (up, cross_in) at 0046B8E6..0046B8EB.
    vector_cross_004ab210(&crossed, &cross_in2, cells.fade_plane_up_00827f68);   // 0046BA63
    normalize_004ab180(&normal2, &crossed);                            // 0046BA6E
    Plane plane2;                                                      // B+84h..B+90h
    {
        const long double px = W(fbits(other[0])) + W(eye.x);         // 0046BA73..0046BA77
        const long double py = W(fbits(other[1])) + W(eye.y);         // 0046BA7F..0046BA87
        const long double pz = W(fbits(other[2])) + W(eye.z);         // 0046BA8F..0046BA96
        copy_words3(&plane2, &normal2);                                // 0046BA7B..0046BAB4 (mov)
        plane2.d = S(-((W(normal2.z) * pz + W(normal2.y) * py) + px * W(normal2.x)));   // 0046BAA1..0046BAC9
    }
    const long double inverse = W(f(kOne)) / W(fbits(length_stored));  // 0046BAD4..0046BADA fdiv [B+80h]
    const U nx = stored_bits(W(unit.x) * inverse);                     // 0046BAE1..0046BAE7 fstp [B+10h]
    const U ny = stored_bits(W(unit.y) * inverse);                     // 0046BAEB..0046BB07 fstp [B+14h]
    ref<U>(scene, 0xB8) = kFadeOn_0074156c;                          // 0046BAEF..0046BAF7
    const U nz = stored_bits(inverse * W(unit.z));                     // 0046BB0F..0046BB21 fstp [B+18h] (unit.z == 0)
    camera = *cells.current_camera_007bb4e8;                           // 0046BAFD
    U* plane3 = &ref<U>(camera, 0x204);                              // 0046BB1B
    const long double sigma = W(*cells.fade_plane_offset_007fbf64);
    const long double a = W(fbits(delta[0])) * sigma;                  // 0046BB25..0046BB2D
    const long double b = W(fbits(delta[1])) * sigma;                  // 0046BB37..0046BB3B
    const U c = stored_bits(sigma * W(fbits(delta[2])));               // 0046BB41..0046BB4B fstp [B+60h]
    const U tx = stored_bits(W(target.x) - a);                         // 0046BB4F..0046BB55 fstp [B+4h]
    const long double ty = W(target.y) - b;                            // 0046BB59..0046BB5F (kept wide)
    plane3[0] = nx; plane3[1] = ny; plane3[2] = nz;                    // 0046BB67..0046BB74 (mov)
    const long double tz_term = (W(target.z) - W(fbits(c))) * W(fbits(nz));   // 0046BB63..0046BB70
    const long double sum = (tz_term + ty * W(fbits(ny))) + W(fbits(tx)) * W(fbits(nx));   // 0046BB77..0046BB87
    plane3[3] = stored_bits(-sum);                                     // 0046BB89..0046BB93 fstp [B+64h] -> mov
    camera = *cells.current_camera_007bb4e8;
    camera_vt(camera)->s090_camera_set_fade_planes(camera, nullptr, &plane1, &plane2); // 0046BBAE Camera::SetFadePlanes
}

// ================================================================ core batch 14 (small per-frame/consumer helpers)
// 00459150 _glScalef(x, y, z): cdecl re-push to opengl32 glScalef
// L3 t1 a2: the three argument words are re-pushed with mov (00459150..0045915F).
void __cdecl gl_scalef_00459150(float, float, float) {
    const U* a = A2_STACK_ARGS();
    words_form<GlWords3>(&glScalef)(a[0], a[1], a[2]);
}

// 0044FCF0 Scene::SetObjectScale(gob, scale) (Scene vtbl+10), RET 8: gob vtbl+CC(scale, 1)
U __fastcall scene_set_object_scale_0044fcf0(void*, void*, void* gob, U scale_bits) {
    return gob_vt(gob)->s0cc_gob_set_object_scale(as_gob(gob), nullptr, scale_bits, 1);   // 0044FCFA..0044FCFD
}

// 0044FD40 Scene::GetObjectScale(gob) (Scene vtbl+14), RET 4: fld [gob+1B4] (ST0)
// L3 t1 a2: returned as the float widened to double (exact: fld dword ; ret, as the original); the long double -> double
// conversion was a qword round trip with no original store (S3).
double __fastcall scene_get_object_scale_0044fd40(void*, void*, void* gob) { return static_cast<double>(ref<float>(gob, 0x1B4)); }

// 0045C030 Camera::SetPosition(Vector) (Camera vtbl+18), RET 10h -> out
Vector* __fastcall camera_set_position_0045c030(void* camera, void*, Vector* out, Vector) {
    const U* p = A2_STACK_ARGS() + 1;                                  // position words [esp+8]..[esp+10h]
    copy_words3(atf(camera, 0x7C), p);                                 // 0045C034..0045C047 (mov)
    copy_words3(out, p);                                               // 0045C04F..0045C056 (mov)
    return out;                                                        // 0045C04A
}

// 004714B0 Part::Draw(flag) (Part vtables +64), RET 4: debug sphere when [00828024]
U __fastcall part_draw_004714b0(void* part, void*, U) {
    if (*cells.render_all_00828024 == 0) return 0;                     // 004714B0..004714BA (EAX = the zero cell value)
    float color[3] = {f(kOne), 0.0f, 0.0f};                            // 004714D1..004714E1 (1,0,0)
    gl_draw_sphere_bound_00431a50(f(0x3DCCCCCDu), atf(part, 0x08), color, f(kOne), 1);   // 004714E9 (0.1, part+8)
    return 0;   // EAX = DrawSphereBound's leftover in the original (no defined value; callers ignore it)
}

// 00451D70 GLRender::DrawFromVerts(mode, CExoArrayList<Vector>*): glBegin(table[mode]); glVertex3fv each; glEnd
void __cdecl gl_draw_from_verts_00451d70(U mode, const PointerList* vertices) {
    glBegin(kPrimitiveModes_0073f334[mode]);                           // 00451D74..00451D7E
    for (I i = 0; i < vertices->size; ++i)                             // 00451D88..00451DB0 (size re-read)
        glVertex3fv(reinterpret_cast<const float*>(static_cast<std::uintptr_t>(vertices->data) + static_cast<U>(i) * 0x0Cu)); // 00451DA5
    glEnd();                                                           // 00451DB6 jmp
}

// 00431DB0 GLRender::RenderObjectsVTCQuad(pool, count): VBO arrays or immediate quads, 0x18-byte V/C/T vertices
void __cdecl gl_render_objects_vtc_quad_00431db0(U pool, U count) {
    if (kotor_renderer_init::VertexBufferAvailable_0045fc90() != 0) { // 00431DB2
        void* buffer = kotor_renderer_gl_cells::pool_007a68a4->GetPool(static_cast<I>(pool));   // 00431DCC slot+10
        glEnableClientState(0x8074);                                   // 00431DDC
        glEnableClientState(0x8076);                                   // 00431DE3
        glEnableClientState(0x8078);                                   // 00431DEA
        glDisableClientState(0x8075);                                  // 00431DF1
        (*cells.bind_buffer_007bb7b4)(0x8892, word(buffer));           // 00431DFD
        glColorPointer(4, 0x1401, 0x18, reinterpret_cast<const void*>(static_cast<std::uintptr_t>(0x0C)));    // 00431E0E
        glTexCoordPointer(2, 0x1406, 0x18, reinterpret_cast<const void*>(static_cast<std::uintptr_t>(0x10))); // 00431E1F
        glVertexPointer(3, 0x1406, 0x18, nullptr);                     // 00431E30
        glDrawArrays(7, 0, static_cast<GLsizei>(count));               // 00431E3F GL_QUADS
        (*cells.bind_buffer_007bb7b4)(0x8892, 0);                      // 00431E4C
        return;
    }
    glBegin(7);                                                        // 00431E57
    const unsigned char* vertex = static_cast<const unsigned char*>(kotor_renderer_gl_cells::pool_007a68a4->GetPool(static_cast<I>(pool)));  // 00431E6A
    for (I n = static_cast<I>(count); n > 0; --n, vertex += 0x18) {    // 00431E71..00431EA3
        glColor4ubv(vertex + 0x0C);                                    // 00431E8A
        words_form<GlWords2>(&glTexCoord2f)(*reinterpret_cast<const U*>(vertex + 0x10), *reinterpret_cast<const U*>(vertex + 0x14));   // 00431E8C..00431E93 (mov pushes)
        glVertex3fv(reinterpret_cast<const float*>(vertex));           // 00431E99
    }
    glEnd();                                                           // 00431EA9 jmp
}

// 00426000 RenderSquares(pool index): 4 V/C vertices (0x10 bytes), line loop
void __cdecl render_squares_00426000(U pool_index) {
    const I pool = static_cast<I>(pool_index);
    if (kotor_renderer_init::VertexBufferAvailable_0045fc90() != 0) { // 00426000
        void* buffer = kotor_renderer_gl_cells::pool_007a68a4->GetPool(pool);   // 00426016
        (*cells.bind_buffer_007bb7b4)(0x8892, word(buffer));           // 0042601F
        glVertexPointer(3, 0x1406, 0x10, nullptr);                     // 00426030
        glColorPointer(4, 0x1401, 0x10, reinterpret_cast<const void*>(static_cast<std::uintptr_t>(0x0C)));   // 00426041
        glDrawArrays(2, 0, 4);                                         // 0042604D GL_LINE_LOOP
        return;
    }
    const unsigned char* vertex = static_cast<const unsigned char*>(kotor_renderer_gl_cells::pool_007a68a4->GetPool(pool));   // 00426058
    glBegin(2);                                                        // 0042605F
    glColor4ubv(vertex + 0x0C);                                        // 00426069 (first vertex colour only)
    for (int n = 0; n < 4; ++n, vertex += 0x10) glVertex3fv(reinterpret_cast<const float*>(vertex));   // 00426080..00426087
    glEnd();                                                           // 0042608C jmp
}

// 004A9AC0 operator*(Quaternion a, Quaternion b) -> out (cdecl, EAX = out)
Quaternion* __cdecl quaternion_multiply_004a9ac0(Quaternion* out, const Quaternion* a, const Quaternion* b) {
    const long double z = ((W(b->y) * W(a->x) + W(b->z) * W(a->w)) - W(b->x) * W(a->y)) + W(a->z) * W(b->w);   // 004A9AC8..004A9AE2
    const long double y = ((W(b->y) * W(a->w) - W(b->z) * W(a->x)) + W(a->y) * W(b->w)) + W(b->x) * W(a->z);   // 004A9AE4..004A9AFE
    const long double x = ((W(b->z) * W(a->y) + W(b->x) * W(a->w)) + W(a->x) * W(b->w)) - W(a->z) * W(b->y);   // 004A9B00..004A9B1A
    const long double w = ((W(a->w) * W(b->w) - W(b->x) * W(a->x)) - W(a->y) * W(b->y)) - W(b->z) * W(a->z);   // 004A9B1C..004A9B3A
    out->w = S(w); out->x = S(x); out->y = S(y); out->z = S(z);       // 004A9B3C..004A9B44
    return out;
}

// ================================================================ core batch 15 (gated-off / debug draws)
// 004A6840 AurGrassMaterialInUse(index): index > count-1 -> false, else byte [materials + index*54h + 50h]
bool __cdecl aur_grass_material_in_use_004a6840(U material) {
    if (static_cast<I>(material) > *cells.grass_materials_00831e48 - 1) return false;   // 004A6840..004A684E (signed jle)
    return ref<U8>(*cells.grass_material_table_00831e44, material * 0x54u + 0x50u) != 0;   // 004A6853..004A685C (AL)
}

// 0049DA00 PartTrigger::DrawVolume (thiscall, RET 0): three DrawFromVerts lists (+80 mode 4, +8C mode 8, +98 mode 8)
void __fastcall part_trigger_draw_volume_0049da00(void* part, void*) {
    gl_draw_from_verts_00451d70(4, &ref<PointerList>(part, 0x80));   // 0049DA0C
    gl_draw_from_verts_00451d70(8, &ref<PointerList>(part, 0x8C));   // 0049DA1A
    gl_draw_from_verts_00451d70(8, &ref<PointerList>(part, 0x98));   // 0049DA28
}

// 0049DA40 PartTrigger::Draw(flag) (PartTrigger table +64), RET 4
void __fastcall part_trigger_draw_0049da40(void* part, void*, U) {
    const long double alpha = W(*cells.trigger_alpha_008304b8) + W(ref<float>(part, 0x58));   // 0049DA43..0049DA4C
    float color[4];                                                    // [esp+4]..[esp+10h]
    {   U* cw = reinterpret_cast<U*>(color);                           // 0049DA4F..0049DA6A (mov copies)
        const U r = ref<U>(part, 0x4C), g = ref<U>(part, 0x50), b = ref<U>(part, 0x54);
        cw[0] = r; cw[1] = g; cw[2] = b; }
    color[3] = S(alpha);                                               // 0049DA58 fst
    if (alpha == 0.0) return;                                        // 0049DA60 fcomp 0; test ah,44h; jnp (EAX = fnstsw word)
    glPushAttrib(0x40040);                                             // 0049DA7F
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_blend_0078d448); // 0049DA91
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_texture_2d_0078d458);   // 0049DA9A
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_lighting_0078d45c);     // 0049DAA3
    glColor4fv(color);                                                 // 0049DAAA
    gl_draw_from_verts_00451d70(9, &ref<PointerList>(part, 0x74));   // 0049DAB6 (LINE_LOOP)
    kotor_renderer_init::glEnable_0073d390(*cells.gl_blend_0078d448); // 0049DAC5
    if (ref<U8>(part, 0xCC) != 0) glBlendFunc(kGlOne_0073f278, kGlOne_0073f278);   // 0049DAD6..0049DAEC
    else glBlendFunc(kBlendSrc_0073f270, kBlendDst_0073f274);         // 0049DADF..0049DAEC
    gl_draw_from_verts_00451d70(3, &ref<PointerList>(part, 0xD0));   // 0049DAFB (LINE_STRIP)
    glPopAttrib();                                                     // 0049DB03
}

namespace {
// 0046F200 / 0046F300 shared body: static 32-entry packed colour table from the .data float rows 0078E6A8.
U* walk_mesh_colors(volatile U* guard, U** storage, bool inverse) {
    U* table;
    if ((*guard & 1u) == 0) {                                          // 0046F223 / 0046F323
        *guard = *guard | 1u;
        table = static_cast<U*>(live_operator_new(0x80));              // 0046F23C (no null test)
        *storage = table;                                              // 0046F244
    } else {
        table = *storage;                                              // 0046F24B
    }
    const float* row = cells.walkmesh_colors_0078e6a8;                 // ecx = 78E6AC - 4
    // L3 t1 a2: the three channel conversions written in line (0046F270..0046F2B8 / 0046F370..0046F3C8: fld [row] (fld 1 ;
    // fsub [row]) ; fmul 255 ; fstp dword [esp+18h] ; FloatToInt), so each is one (float) at its original store; the loop
    // is kept a loop (one static store each).
#pragma clang loop unroll(disable)
    for (int n = 0; n < 32; ++n, row += 3) {                           // 0046F270..0046F2E4 (until 78E82C)
        const I r = x87_round(S((inverse ? (W(f(kOne)) - W(row[0])) : W(row[0])) * W(f(kByte_0073ede4))));   // 0046F279 / 0046F37F
        const I g = x87_round(S((inverse ? (W(f(kOne)) - W(row[1])) : W(row[1])) * W(f(kByte_0073ede4))));   // 0046F291 / 0046F39D
        const I b = x87_round(S((inverse ? (W(f(kOne)) - W(row[2])) : W(row[2])) * W(f(kByte_0073ede4))));   // 0046F2AA / 0046F3BC
        table[n] = (((static_cast<U>(b) | 0xFFFFFF00u) << 8 | static_cast<U>(g)) << 8) | static_cast<U>(r);  // 0046F2BA..0046F2D6
    }
    return table;                                                      // EAX = table (set before the loop)
}
}
// 0046F200 GetWalkMeshColors
U* __cdecl get_walk_mesh_colors_0046f200() { return walk_mesh_colors(cells.walkmesh_guard_008280e4, cells.walkmesh_table_008280e0, false); }
// 0046F300 GetInverseWalkMeshColors
U* __cdecl get_inverse_walk_mesh_colors_0046f300() { return walk_mesh_colors(cells.walkmesh_inverse_guard_008280ec, cells.walkmesh_inverse_table_008280e8, true); }

// 004AB630 RotationArc(out, Vector from, Vector to) -> out (static result 00831F64)
// L3 t1 a2: `from` and `to` are normalized in place in their own stack argument slots (004AB670 lea ecx,[esp+14h] /
// 004AB679 lea ecx,[esp+20h]) and read from there; clang's by-value Vector parameters were copied into locals with
// fld/fstp dword (quiets a signalling NaN; four extra dword round trips, S3).
Quaternion* __cdecl rotation_arc_004ab630(Quaternion* out, Vector, Vector) {
    U* const args = A2_STACK_ARGS();                                   // out, from [1..3], to [4..6]
    Vector* const from = reinterpret_cast<Vector*>(args + 1);
    Vector* const to = reinterpret_cast<Vector*>(args + 4);
    U guard = *cells.rotation_arc_guard_00831f74;
    if ((guard & 1u) == 0) {                                           // 004AB630..004AB666 static Quaternion = (1,0,0,0)
        *cells.rotation_arc_guard_00831f74 = guard | 1u;
        U* q = reinterpret_cast<U*>(cells.rotation_arc_00831f64); q[3] = 0; q[2] = 0; q[1] = 0; q[0] = kOne;
    }
    vector_normalize_004ab130(from, nullptr);                          // 004AB674 (lane-3d-model)
    vector_normalize_004ab130(to, nullptr);                            // 004AB67D
    Vector axis;                                                       // [esp] (E-C)
    vector_cross_004ab210(&axis, from, to);                            // 004AB691
    const long double d = (W(to->z) * W(from->z) + W(to->x) * W(from->x)) + W(to->y) * W(from->y);   // 004AB696..004AB6B3
    if (d < W(f(kMinusOne_00740cbc)) || d == W(f(kMinusOne_00740cbc))) {   // 004AB6B5 fcom -1; test ah,41h; jp (> or NaN computes)
        U* o = reinterpret_cast<U*>(out); o[0] = 0; o[1] = kOne; o[2] = 0; o[3] = 0;   // 004AB6CA..004AB6D6
        return out;
    }
    const long double s = static_cast<long double>(sqrt(static_cast<long double>((d + W(f(kOne))) * 2.0)));   // 004AB6DD..004AB6E7 (L3: as above)
    const long double inverse = W(f(kOne)) / s;                             // 004AB6E9..004AB6EF
    Quaternion* q = cells.rotation_arc_00831f64;
    q->x = S(W(axis.x) * inverse);                                     // 004AB6F1..004AB6F7
    q->y = S(W(axis.y) * inverse);                                     // 004AB6FD..004AB703
    q->z = S(inverse * W(axis.z));                                     // 004AB709..004AB70D
    q->w = S(s * W(f(kHalf_0073e9ac)));                                // 004AB713..004AB719
    const U* qw = reinterpret_cast<const U*>(q);
    U* o = reinterpret_cast<U*>(out);
    o[0] = qw[0]; o[1] = qw[1]; o[2] = qw[2]; o[3] = qw[3];            // 004AB71F..004AB73F (mov, w x y z)
    return out;
}

// 004318F0 GLRender::DrawTriangles(face list*, vertices, colours): flat-normal triangles (32-byte faces)
void __cdecl gl_draw_triangles_004318f0(const PointerList* faces, const Vector* vertices, const U* colors) {
    glBegin(4);                                                        // 004318F7 GL_TRIANGLES
    Vector v0{}, v1{}, v2{};                                           // [esp+14h..34h] zeroed (00431908..00431948)
    for (I i = 0; i < faces->size; ++i) {                              // 00431901..00431A2F (size re-read)
        const unsigned char* face = reinterpret_cast<const unsigned char*>(static_cast<std::uintptr_t>(faces->data)) + i * 0x20;
        glColor4ubv(reinterpret_cast<const GLubyte*>(colors + *reinterpret_cast<const U*>(face + 0x10)));   // 0043197F
        v0 = vertices[*reinterpret_cast<const std::int16_t*>(face + 0x1A)];   // 0043198B..004319A5
        v1 = vertices[*reinterpret_cast<const std::int16_t*>(face + 0x1C)];   // 004319A9..004319C3
        v2 = vertices[*reinterpret_cast<const std::int16_t*>(face + 0x1E)];   // 004319C7..004319EB
        Vector normal;
        glNormal3fv(&normal_of_004aca00(&normal, &v0, &v1, &v2)->x);   // 004319FA / 00431A03
        glVertex3fv(&v0.x); glVertex3fv(&v1.x); glVertex3fv(&v2.x);    // 00431A0E..00431A1C
    }
    glEnd();                                                           // 00431A3C jmp
}

// 00431A50 GLRender::DrawSphereBound(radius, center, colour, scale, billboard)
void __cdecl gl_draw_sphere_bound_00431a50(float radius, const void* center, const float* color, float scale, U billboard) {
    kotor_renderer_init::glDisable_0073d22c(0x0DE1);                   // 00431A5F
    kotor_renderer_init::glDisable_0073d22c(0x0B50);                   // 00431A66
    if (static_cast<U8>(billboard) != 0) {                             // 00431A68 byte test
        float m[16];                                                   // [esp+4]
        glGetFloatv(0x0BA6, m);                                        // 00431A7A GL_MODELVIEW_MATRIX
        // 00431A80..00431AC4: each swap is an x87 copy one way (fld m[1] ; fstp m[4]) and a mov the other way (m[1] =
        // the old m[4] word), as the original.
        U* mw = reinterpret_cast<U*>(m);
        {   const U old4 = mw[4]; mw[4] = stored_bits(W(m[1])); mw[1] = old4; }    // 00431A80 / 00431A94..00431A98 / 00431ABC
        {   const U old8 = mw[8]; mw[8] = stored_bits(W(m[2])); mw[2] = old8; }    // 00431A9C / 00431AA4..00431AA8 / 00431AC0
        {   const U old9 = mw[9]; mw[9] = stored_bits(W(m[6])); mw[6] = old9; }    // 00431AAC / 00431AB4..00431AB8 / 00431AC4
        copy_words3(&mw[12], center);                                  // 00431A84..00431AB0
        glPushMatrix();                                                // 00431AC8
        glMatrixMode(0x1700);                                          // 00431AD3
        glMultMatrixf(m);                                              // 00431ADE
    }
    {   const U* cw = reinterpret_cast<const U*>(color);               // 00431AE4..00431AF5 (mov pushes)
        words_form<GlWords3>(&glColor3f)(cw[0], cw[1], cw[2]); }       // 00431AF6
    // L3 t1 a2: r, a, b and -r live in the original's frame (fstp dword, reloaded for every glVertex3f push); they are
    // frame variables here too (volatile: one store each, reloaded per use) instead of float registers spilled as
    // 4-byte values across the GL calls (S1).
    volatile float r = S(W(radius) * W(scale));                        // 00431AFC..00431B06
    volatile float a = S(W(r) * W(f(kSphereA_0073f4ec)));              // 00431B21..00431B2D (0.717)
    volatile float b = S(W(r) * W(f(kSphereB_0073f4e8)));              // 00431B40..00431B4C (-0.717)
    volatile float nr = S(-W(r));                                      // 00431B58..00431B60
    glBegin(2);                                                        // 00431B0A GL_LINE_LOOP
    glVertex3f(0.0f, r, 0.0f);                                         // 00431B1F
    glVertex3f(a, a, 0.0f);                                            // 00431B37
    glVertex3f(r, 0.0f, 0.0f);                                         // 00431B3E
    glVertex3f(a, b, 0.0f);                                            // 00431B56
    glVertex3f(0.0f, nr, 0.0f);                                        // 00431B6B
    glVertex3f(b, b, 0.0f);                                            // 00431B71
    glVertex3f(nr, 0.0f, 0.0f);                                        // 00431B78
    glVertex3f(b, a, 0.0f);                                            // 00431B7E
    glEnd();                                                           // 00431B80
    if (static_cast<U8>(billboard) != 0) glPopMatrix();                // 00431B86..00431B91
    kotor_renderer_init::glEnable_0073d390(0x0DE1);                    // 00431BA2
    kotor_renderer_init::glEnable_0073d390(0x0B50);                    // 00431BA9
}

// 0046EF10 GLRender::DrawAxisAlignedFrame(min, max, colour): two loops and four edges
void __cdecl gl_draw_axis_aligned_frame_0046ef10(const void* minimum, const void* maximum, const void* colour) {
    // L3 t1 a2: every glColor3f / glVertex3f argument is a word the original pushes with mov (0046EF28..0046F037): word-typed
    // calls (the float calls loaded each with fld dword, quieting a signalling NaN).
    const U* a = static_cast<const U*>(minimum);
    const U* b = static_cast<const U*>(maximum);
    const U* c = static_cast<const U*>(colour);
    const GlWords3 vertex = words_form<GlWords3>(&glVertex3f);
    kotor_renderer_init::glDisable_0073d22c(0x0DE1);                   // 0046EF1F
    kotor_renderer_init::glDisable_0073d22c(0x0B50);                   // 0046EF26
    words_form<GlWords3>(&glColor3f)(c[0], c[1], c[2]);                // 0046EF37
    glBegin(2);                                                        // 0046EF45
    vertex(a[0], a[1], a[2]); vertex(a[0], b[1], a[2]); vertex(b[0], b[1], a[2]); vertex(b[0], a[1], a[2]);
    glEnd();                                                           // 0046EF89
    glBegin(2);                                                        // 0046EF91
    vertex(a[0], a[1], b[2]); vertex(b[0], a[1], b[2]); vertex(b[0], b[1], b[2]); vertex(a[0], b[1], b[2]);
    glEnd();                                                           // 0046EFC7
    glBegin(1);                                                        // 0046EFCF GL_LINES
    vertex(a[0], a[1], a[2]); vertex(a[0], a[1], b[2]);
    vertex(b[0], a[1], a[2]); vertex(b[0], a[1], b[2]);
    vertex(b[0], b[1], a[2]); vertex(b[0], b[1], b[2]);
    vertex(a[0], b[1], a[2]); vertex(a[0], b[1], b[2]);
    glEnd();                                                           // 0046F039
    kotor_renderer_init::glEnable_0073d390(0x0B50);                    // 0046F04A
    kotor_renderer_init::glEnable_0073d390(0x0DE1);                    // 0046F051
}

// ================================================================ core batch 16 (gated-off, link-required)
// 0044FE00 Scene::HitCheckGob(start, end, Vector* out, CHitInfo* hit) (thiscall, RET 20h): nearest gob hit
// L3 t1 a2: start / end are the raw stack words (A2_STACK_ARGS()[0..2] / [3..5]); Gob::HitCheck receives them as the
// words the original copies with mov (0044FEC4..0044FEFB) and shortens `end` in place through its own argument slot
// (0044FEC7 lea ecx,[esp+28h] = &end); `*out = end` is a mov copy (0044FF2A..0044FF3B).
I __fastcall scene_hit_check_gob_0044fe00(void* scene, void*, Vector, Vector, Vector* out, void* hit) {
    U* const args = A2_STACK_ARGS();
    const U* start = args;                                             // [esp+18h]..[esp+20h] after the five pushes
    U* end = args + 3;                                                 // [esp+24h]..[esp+2Ch]
    I result = 0;                                                      // [esp+10h]
    for (I index = 0; index < ref<I>(scene, 0x58); ++index) {        // 0044FE14..0044FF14 (count re-read)
        void* gob = ref<void**>(scene, 0x54)[index];
        void* model = ref<void*>(gob, 0x58);
        if (model != nullptr) {                                        // 0044FE29
            if ((ref<U8>(model, 0x50) & 1u) != 0 && ref<U8>(hit, 0x60) == 0) continue;   // 0044FE2D..0044FE38
            if (ref<U8>(gob, 0x17C) != 0) continue;                  // 0044FE3E
            const U8 flags = ref<U8>(model, 0x50);                   // 0044FE4C (word load, low byte tested)
            if (((flags >> 1) & 1u) != 0) continue;                    // 0044FE52
            if (((flags >> 7) & 1u) != 0) continue;                    // 0044FE5D..0044FE67 (bit stored into the hit-arg slot byte)
        }
        if (ref<U8>(hit, 0x5C) != 0 && model != nullptr) {           // 0044FE6D..0044FE76
            const U16 word = ref<U16>(model, 0x50);                  // 0044FE78
            if (((word >> 2) & 1u) != 0) continue;
            if (((word >> 4) & 1u) != 0) continue;
            if (((word >> 5) & 1u) != 0 && ((word >> 10) & 1u) == 0) continue;   // 0044FE94..0044FEA7
        }
        I ignored = 0;                                                 // 0044FEA9..0044FEC2 hit+8/+C ignore list
        const PointerList& ignore = ref<PointerList>(hit, 0x08);
        for (I n = 0; n < ignore.size; ++n)
            if (reinterpret_cast<void**>(static_cast<std::uintptr_t>(ignore.data))[n] == gob) ++ignored;
        if (ignored != 0) continue;
        const I r = words_form<HitCheckWords>(gob_vt(gob)->s08c_gob_hit_check)(gob, nullptr, start[0], start[1], start[2],
            end[0], end[1], end[2], reinterpret_cast<Vector*>(end), hit);   // 0044FF00 (shortens end)
        if (r != 0) result = r;                                        // 0044FF0A
    }
    if (result != 0 && out != nullptr) copy_words3(out, end);          // 0044FF1A..0044FF3B
    return result;                                                     // 0044FF3E mov eax,ecx
}

// 00456B60 Scene::HitCheck(start, end, Vector* out, CHitInfo* hit) (Scene vtbl+38, RET 20h)
// L3 t1 a2: start / end are the raw stack words (A2_STACK_ARGS()[0..2] / [3..5]); every callee receives them as the
// words the original copies with mov, and `end` is shortened in place through its own argument slot (&end = lea
// [esp+24h]-relative, 00456BAF / 00456C6B / 00456D07); the result copies are mov copies.
I __fastcall scene_hit_check_00456b60(void* scene, void*, Vector, Vector, Vector* out, void* hit) {
    U* const args = A2_STACK_ARGS();
    U* start = args;                                                   // [esp+18h]..[esp+20h] after the five pushes
    U* end = args + 3;                                                 // [esp+24h]..[esp+2Ch]
    Vector* const end_slot = reinterpret_cast<Vector*>(end);
    if (hit == nullptr) {                                              // 00456B68
        hit = cells.scene_hit_info_007b93a8;                           // 00456B73 static CHitInfo
        aurora_hit_info_clear_output_00456a60(hit, nullptr);           // 00456B7A (lane-3d-model)
    }
    *cells.bsp_hit_info_007fbf48 = hit;                                // 00456B83
    void* hit_gob = nullptr;                                           // [esp+34h] reused as the gob result
    if (ref<U8>(hit, 0x5F) == 0) {                                   // 00456B89..00456B96
        if (ref<I>(scene, 0xCC) == 0) {                              // 00456B9C (no rooms: whole-scene BSP)
            if (words_form<BspHitWords>(&bsp_hit_check_004ad260)(ref<void*>(scene, 0xA0), start[0], start[1], start[2],
                    end[0], end[1], end[2], end_slot, hit_part_on_bsp_node_0046b4c0) != 0) {  // 00456BE9
                copy_words3(atf(hit, 0x1C), end);                      // 00456BFF..00456C15
                ref<U>(hit, 0x18) = 1;
                if (out != nullptr) copy_words3(out, end);             // 00456C18..00456C22
            }
        }
        for (I index = 0; index < ref<I>(scene, 0xCC); ++index) {    // 00456C25..00456CED
            void* room = ref<void**>(scene, 0xC8)[index];
            if (box_intersect_004aa610(reinterpret_cast<const Vector*>(start), end_slot, &ref<Vector>(room, 0x1C), &ref<Vector>(room, 0x28)) == 0) continue;  // 00456C5B
            if (words_form<PartHitWords>(&hit_part_on_bsp_node_0046b4c0)(room, start[0], start[1], start[2],
                    end[0], end[1], end[2], end_slot) == 0) continue;   // 00456C9F
            copy_words3(atf(hit, 0x1C), end);                          // 00456CAD..00456CC8
            ref<U>(hit, 0x18) = 1;
            if (out != nullptr) copy_words3(out, end);                 // 00456CCD..00456CD8
            ref<void*>(hit, 0x54) = room;                            // 00456CDB
        }
    }
    *cells.bsp_hit_info_007fbf48 = nullptr;                            // 00456CF3
    if (ref<U>(hit, 0x00) == 0) {                                    // 00456CFD
        const I r = words_form<HitCheckWords>(&scene_hit_check_gob_0044fe00)(scene, nullptr, start[0], start[1], start[2],
            end[0], end[1], end[2], end_slot, hit);                     // 00456D3C
        hit_gob = reinterpret_cast<void*>(static_cast<std::uintptr_t>(static_cast<U>(r)));
        if (r != 0) {
            copy_words3(atf(hit, 0x1C), end);                          // 00456D49..00456D68
            ref<U>(hit, 0x18) = 2;
            if (out != nullptr) copy_words3(out, end);                 // 00456D6D..00456D72
        }
    }
    void* owner = ref<void*>(hit, 0x38);                             // 00456D75 unregister old SafePointer
    if (owner != nullptr) ref<PointerList>(owner, 0x04).RemoveReverse32(word(atf(hit, 0x38)));   // 00456D7F..00456DB6 (inlined Remove)
    ref<void*>(hit, 0x38) = hit_gob;                                 // 00456DBE
    if (hit_gob != nullptr)                                            // 00456DC0..00456DEC register &hit+38 with the gob
        int_list_add_00444fe0(&ref<PointerList>(hit_gob, 0x04), nullptr, word(atf(hit, 0x38)));
    return ref<I>(hit, 0x18);                                        // 00456DEF
}

// 00451A50 Scene::RenderShadows(a1, a2, cast, clear) (Scene vtbl+BC, RET 10h)
U __fastcall scene_render_shadows_00451a50(void* scene, void*, U, U, U cast, U clear) {
    using kotor_renderer_init::glEnable_0073d390;
    using kotor_renderer_init::glDisable_0073d22c;
    const I bits = aur_get_available_stencil_bits_0045fd00();          // 00451A54
    if (bits == 0) return 0;                                           // 00451A5F (EAX = 0)
    glDisable_0073d22c(*cells.gl_texture_2d_0078d458);                 // 00451A74
    glDisable_0073d22c(*cells.gl_alpha_test_0078d44c);                 // 00451A7D
    glDisable_0073d22c(*cells.gl_lighting_0078d45c);                   // 00451A86
    glStencilMask(0xFF);                                               // 00451A8D
    glEnable_0073d390(*cells.gl_stencil_test_0078d470);                // 00451A9F
    if (clear != 0) glClear(0x400);                                    // 00451AA1..00451AAE
    const auto soft = []() {                                           // 00451AF4..00451B1F / 00451BD7..00451C02
        return *cells.soft_requested_0078e420 != 0 && aur_nv2x_level_shaders_available_0045fbd0() &&
               (kotor_soft_shadow::SoftAvailable_0045f7b0() != 0 || kotor_soft_shadow::ATISoftAvailable_0045f7e0() != 0) &&
               *cells.soft_enabled_0078dae9 != 0;
    };
    if (cast != 0) {                                                   // 00451AB4
        void* light = ref<void*>(ref<void*>(scene, 0x270), 0x1C);  // 00451AC6..00451ACC
        if (light != nullptr &&
            W(ref<float>(scene, 0xB0)) + W(f(kShadowLift_007415c4)) < W(ref<float>(light, 0x9C))) {   // 00451AD7..00451AEE
            if (soft()) gl_set_up_soft_shadows_0042d1c0();             // 00451B21
            glStencilMask(0xFF);                                       // 00451B2B
            glEnable_0073d390(*cells.gl_stencil_test_0078d470);        // 00451B38
            glDepthMask(0);                                            // 00451B3C
            glColorMask(0, 0, 0, 0);                                   // 00451B4A
            glStencilFunc(kGlAlways_0073f2bc, 0x0F, 0x0F);             // 00451B5A
            glDisableClientState(kGlNormalArray_0073f39c);             // 00451B6D
            glDisableClientState(kGlTexCoordArray_0073f398);           // 00451B76
            glDisableClientState(kGlColorArray_0073f3a0);              // 00451B7E
            (void)scene_vt(scene)->s0fc_scene_do_gob_shadows_for_this_light(scene, nullptr, word(light));   // 00451B85
            glEnableClientState(kGlNormalArray_0073f39c);              // 00451B91
            glEnableClientState(kGlTexCoordArray_0073f398);            // 00451B9A
            glEnableClientState(kGlColorArray_0073f3a0);               // 00451BA3
            glColorMask(1, 1, 1, 1);                                   // 00451BAD
            glStencilMask(0);                                          // 00451BBB
            (void)scene_vt(scene)->s100_scene_render_stencil_shadows_for_this_light(scene, nullptr, word(light),
                ref<U>(ref<void*>(scene, 0x270), 0x20), static_cast<U>(bits));   // 00451BD1
            if (soft()) gl_finish_soft_shadows_0042d9a0();             // 00451C04
            glStencilMask(0xFF);                                       // 00451C0E
        }
    }
    glEnableClientState(kGlNormalArray_0073f39c);                      // 00451C23
    glEnableClientState(kGlTexCoordArray_0073f398);                    // 00451C2B
    if (*cells.black_fog_008305e0 != 0) glEnableClientState(kGlColorArray_0073f3a0);   // 00451C2D..00451C3D
    glDepthMask(1);                                                    // 00451C41
    glEnable_0073d390(*cells.gl_texture_2d_0078d458);                  // 00451C4E
    glEnable_0073d390(*cells.gl_alpha_test_0078d44c);                  // 00451C56
    glEnable_0073d390(*cells.gl_lighting_0078d45c);                    // 00451C5F
    glDisable_0073d22c(*cells.gl_stencil_test_0078d470);               // 00451C68
    return 0;   // EAX: leftover of the last GL call in the original (no defined value)
}

// 00471560 PartAABB::Draw(flag) (PartAABB table +64, RET 4): walkmesh debug draw
U __fastcall part_aabb_draw_00471560(void* part, void*, U) {
    using kotor_renderer_init::glEnable_0073d390;
    using kotor_renderer_init::glDisable_0073d22c;
    if (*cells.walkmesh_debug_007fbf5c == 0) return 0;                 // 00471578..00471586 (EAX = 0)
    U guard = *cells.aabb_guard_00828118;
    if ((guard & 1u) == 0) { *cells.aabb_guard_00828118 = guard | 1u; *cells.aabb_colors_00828114 = get_walk_mesh_colors_0046f200(); }          // 0047158C..004715AE
    guard = *cells.aabb_guard_00828118;
    if ((guard & 2u) == 0) { *cells.aabb_guard_00828118 = guard | 2u; *cells.aabb_inverse_colors_00828110 = get_inverse_walk_mesh_colors_0046f300(); } // 004715B7..004715D4
    void* node = ref<void*>(part, 0x04);                             // 004715E8
    glDisable_0073d22c(*cells.gl_texture_2d_0078d458);                 // 004715EC
    glDisable_0073d22c(*cells.gl_lighting_0078d45c);                   // 004715F5
    glColor4f(0.0f, 0.0f, 0.0f, 0.0f);                                 // 004715FF
    if (node == nullptr) return 0;                                     // 00471605
    if (ref<U>(part, 0x40) == 0) {                                   // 0047160D
        glPushMatrix();                                                // 00471618
        transform_0046f6b0(&ref<Vector>(part, 0x08), &ref<Quaternion>(part, 0x14), 0);   // 0047161E..00471751 (inlined)
    }
    *cells.aabb_face_count_00827fb0 = *cells.aabb_face_count_00827fb0 + ref<U>(node, 0x5C);   // 00471757..00471768
    const PointerList* faces = &ref<PointerList>(node, 0x58);
    const Vector* vertices = ref<const Vector*>(node, 0x198);
    gl_draw_triangles_004318f0(faces, vertices, *cells.aabb_colors_00828114);            // 00471779
    glFrontFace(kGlCw_0073f2c0);                                       // 0047178E
    gl_draw_triangles_004318f0(faces, ref<const Vector*>(node, 0x198), *cells.aabb_inverse_colors_00828110);   // 0047179F
    glFrontFace(kGlCcw_0073f2c4);                                      // 004717AE
    glPolygonMode(kGlFront_0073f2ec, kPolygonModes_0073f2e4[0]);       // 004717C3 (FRONT, LINE)
    gl_draw_triangles_004318f0(faces, ref<const Vector*>(node, 0x198), *cells.aabb_inverse_colors_00828110);   // 004717D4
    glFrontFace(kGlCw_0073f2c0);                                       // 004717E2
    gl_draw_triangles_004318f0(faces, ref<const Vector*>(node, 0x198), *cells.aabb_colors_00828114);           // 004717F3
    glFrontFace(kGlCcw_0073f2c4);                                      // 00471801
    glPolygonMode(kGlFront_0073f2ec, kPolygonModes_0073f2e4[1]);       // 00471811 (FRONT, FILL)
    if (ref<U>(part, 0x40) == 0) glPopMatrix();                      // 00471813..0047181F
    glEnable_0073d390(*cells.gl_texture_2d_0078d458);                  // 00471832
    glEnable_0073d390(*cells.gl_lighting_0078d45c);                    // 0047183B
    return 0;   // EAX = glEnable leftover in the original (no defined value)
}

// 00456300 Scene::DoHighlightTriggers(flag byte) (Scene vtbl+118, RET 4): stencil-masked trigger volumes
U __fastcall scene_do_highlight_triggers_00456300(void* scene, void*, U flag) {
    using kotor_renderer_init::glEnable_0073d390;
    using kotor_renderer_init::glDisable_0073d22c;
    using SlotVoid0 = void (__fastcall*)(void*, void*);
    PointerList triggers{};                                            // [esp+20h]: data/size/capacity = 0 (00456328..00456330)
    const I bits = aur_get_available_stencil_bits_0045fd00();          // 0045633B
    if (bits == 0) return 0;                                           // 00456346
    // L3 t1: 0045634C..0045635E fld [0078E574] ; fmul dword [00740C88] ; fadd [scene+B4h] ; fstp dword [scene+B4h]
    ref<float>(scene, 0xB4) = static_cast<float>(Wcell(cells.frame_time_0078e574) * kRsTwoPi_00740c88 + ref<float>(scene, 0xB4));
    if (*cells.trigger_highlight_0078e614 != 0) {                      // 00456364
        glDisable_0073d22c(*cells.gl_blend_0078d448);                  // 0045637E
        glDisable_0073d22c(*cells.gl_texture_2d_0078d458);             // 00456387
        glDisable_0073d22c(*cells.gl_lighting_0078d45c);               // 00456390
        glDisable_0073d22c(*cells.gl_alpha_test_0078d44c);             // 00456398
        for (I i = 0; i < cells.trigger_list_0080aa74->size; ++i) {    // 0045639A..0045640A
            void* trigger = reinterpret_cast<void**>(static_cast<std::uintptr_t>(cells.trigger_list_0080aa74->data))[i];
            if (ref<U8>(trigger, 0xA8) == 0) continue;               // 004563B9
            if (static_cast<U8>(flag) != ref<U8>(trigger, 0xB0)) continue;   // 004563C3..004563D0
            int_list_add_00444fe0(&triggers, nullptr, word(trigger));  // 004563D2..004563FE (same growth sequence)
        }
        const U reference = 1u << static_cast<U>(bits - 1);            // 0045641C..00456428 (E-A0)
        for (I k = 0; k < triggers.size; ++k) {                        // 0045640C..00456866
            const I stencil = aur_get_available_stencil_bits_0045fd00();   // 00456430
            if (stencil != 0) glClearStencil(static_cast<GLint>((1u << static_cast<U>(stencil - 1)) - 1u));   // 00456439..00456445
            glStencilMask(0xFF); glClear(0x400); glStencilMask(0xFF);   // 00456451..00456468
            glEnable_0073d390(*cells.gl_stencil_test_0078d470);        // 00456470
            glDepthMask(0);                                            // 00456478
            glColorMask(0, 0, 0, 0);                                   // 0045648C
            glStencilFunc(kGlAlways_0073f2bc, 0x0F, 0x0F);             // 00456499
            glStencilOp(kGlKeep_0073f2c8, kGlIncr_0073f2d4, kGlKeep_0073f2c8);   // 004564B3
            glFrontFace(kGlCw_0073f2c0);                               // 004564C1
            void* trigger = reinterpret_cast<void**>(static_cast<std::uintptr_t>(triggers.data))[k];
            slot<SlotVoid0>(trigger, 0x7C)(trigger, nullptr);          // 004564CD (trigger volume draw)
            glStencilOp(kGlKeep_0073f2c8, kGlDecr_0073f2d8, kGlKeep_0073f2c8);   // 004564DE
            glFrontFace(kGlCcw_0073f2c4);                              // 004564E7
            trigger = reinterpret_cast<void**>(static_cast<std::uintptr_t>(triggers.data))[k];
            slot<SlotVoid0>(trigger, 0x7C)(trigger, nullptr);          // 004564F3
            glFrontFace(kGlCcw_0073f2c4);                              // 004564FC
            glColorMask(1, 1, 1, 1);                                   // 00456506
            PointerList quad{};                                        // [esp+10h]: data/size/capacity = 0 (00456508..00456512)
            const auto corner = [&quad](U plane_a, U plane_b) {        // cross(plane[a], plane[b]) -> normalize -> append
                Plane* planes = ref<PlaneList>(*cells.current_camera_007bb4e8, 0x1F8).data;
                Vector crossed, direction;
                vector_cross_004ab210(&crossed, reinterpret_cast<const Vector*>(&planes[plane_a]), reinterpret_cast<const Vector*>(&planes[plane_b]));
                // L3 t1 a2: the normalized vector is copied as three words (mov, 0045654D..00456555 / 004565B4..004565BE ..),
                // held across the allocate call and stored with mov (00456579..0045657E ..); a float copy was three x87
                // values spilled through 4 bytes (S1) and quieted a signalling NaN.
                const U* n = reinterpret_cast<const U*>(normalize_004ab180(&direction, &crossed));
                const U vx = n[0], vy = n[1], vz = n[2];
                if (quad.size == quad.capacity)                        // (first append: capacity 0 == size 0 -> 8)
                    vector_list_allocate_00421310(&quad, nullptr, quad.capacity != 0 ? quad.capacity + quad.capacity : 8);
                U* slot3 = reinterpret_cast<U*>(static_cast<std::uintptr_t>(quad.data)) + 3 * static_cast<U>(quad.size);
                slot3[0] = vx; slot3[1] = vy; slot3[2] = vz;
                quad.size = quad.size + 1;
            };
            corner(5, 2);                                              // 0045653A / 00456562 (plane+50 x plane+20)
            corner(3, 5);                                              // 004565A1 (plane+30 x plane+50)
            corner(4, 3);                                              // 00456629 (plane+40 x plane+30)
            corner(2, 4);                                              // 004566AD (plane+20 x plane+40)
            glPushMatrix();                                            // 0045670D
            // L3 t1 a2: the camera position words are pushed with mov (00456713..00456729): a word-typed call (a float
            // call loaded them with fld dword, which quiets a signalling NaN).
            using GlTranslateWords = void (__stdcall*)(U, U, U);
            const U* eye = &ref<U>(*cells.current_camera_007bb4e8, 0x7C);
            const U ex = eye[0], ey = eye[1], ez = eye[2];
            words_form<GlTranslateWords>(&glTranslatef)(ex, ey, ez);   // 00456736
            glEnable_0073d390(*cells.gl_blend_0078d448);               // 00456743
            glBlendFunc(kBlendSrc_0073f270, kBlendDst_0073f274);       // 00456756
            glStencilFunc(kGlLequal_0073f2ac, static_cast<GLint>(reference), 0xFFFFFFFFu);   // 0045676A
            glStencilOp(kGlKeep_0073f2c8, kGlKeep_0073f2c8, kGlKeep_0073f2c8);              // 00456778
            void* t = reinterpret_cast<void**>(static_cast<std::uintptr_t>(triggers.data))[k];
            if (ref<U8>(t, 0xC4) != 0) {                             // 00456789 pulsing alpha
                // L3 t1: 00456793..004567DD fld [t+C8h] ; fmul [scene+B4h] ; fsin ; fadd 1.0f ; fmul [t+C0h] ; fmul 0.5f ;
                // fstp dword [esp+1Ch] (replaces the a1 helper x87trig_004567b3)
                // L3 t1 a2: the colour words +B4/+B8/+BC are pushed with mov (004567A3..004567EC) and pulse as the stored
                // word ([esp+1Ch], 004567E1): a word-typed call.
                using GlColor4Words = void (__stdcall*)(U, U, U, U);
                const U r = ref<U>(t, 0xB4), g = ref<U>(t, 0xB8), b = ref<U>(t, 0xBC);
                const U pulse = stored_bits((sin(static_cast<long double>(ref<float>(t, 0xC8)) * ref<float>(scene, 0xB4)) + kRsOne_0073d764)
                                            * ref<float>(t, 0xC0) * kRsHalf_0073e9ac);
                words_form<GlColor4Words>(&glColor4f)(r, g, b, pulse);   // 004567ED
            } else {
                glColor4fv(&ref<float>(t, 0xB4));                    // 004567FB
            }
            glBegin(kGlPolygon_0073f34c);                              // 00456808
            for (I n = 0; n < quad.size; ++n)                          // 0045680E..0045681F (size snapshot in esi)
                glVertex3fv(&reinterpret_cast<const Vector*>(static_cast<std::uintptr_t>(quad.data))[n].x);
            glEnd();                                                   // 00456821
            glPopMatrix();                                             // 00456827
            glDisable_0073d22c(*cells.gl_stencil_test_0078d470);       // 00456834
            glDepthMask(1);                                            // 0045683C
            if (quad.data != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(quad.data)));   // 00456842..0045684F
        }
        glEnable_0073d390(*cells.gl_alpha_test_0078d44c);              // 00456878
        glEnable_0073d390(*cells.gl_blend_0078d448);                   // 00456881
        glEnable_0073d390(*cells.gl_texture_2d_0078d458);              // 0045688A
        glEnable_0073d390(*cells.gl_lighting_0078d45c);                // 00456892
    }
    if (triggers.data != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(triggers.data)));   // 00456895..004568A5
    return 0;   // EAX: leftover in the original (no defined value)
}

// ================================================================ typed CSWGuiScene / Camera tables: authored slot bodies
// 004175A0 CSWGuiScene scalar deleting destructor (CSWGuiScene vtbl+00), RET 4
U __fastcall gui_scene_destruct_deleting_004175a0(void* gui_scene, void*, U flags) {
    gui_scene_destruct_004165e0(gui_scene, nullptr);                   // 004175A3
    if ((static_cast<U8>(flags) & 1u) != 0) live_free(gui_scene);      // 004175A8..004175B0
    return word(gui_scene);                                            // 004175B8
}
// 004064A0 GetProperty0x10 (ICF; CSWGuiScene vtbl+08): mov eax,[ecx+10h]
// [M45 compose] 004064A0 body removed: existing M44 provider gui_load_slot_prop10_004064a0 wins (definition removed; declaration kept)
U __fastcall gui_scene_get_property_0x10_004064a0(void* self, void*);
// 005C5630 void_return_0xc (ICF; CSWGuiScene vtbl+10): ret 0Ch
void __fastcall gui_scene_void_return_0c_005c5630(void*, void*, U, U, U) {}

// 0045C330 Camera::~Camera (body): restore vtables, free the plane list, destroy the Gob, base vtable
void __fastcall camera_destruct_0045c330(void* camera, void*) {
    ref<const CameraVTable*>(camera, 0) = &camera_vtable_00741b20;  // 0045C353
    ref<const aurora_model::GobVTable*>(camera, 4) = &camera_gob_vtable_00741988;   // 0045C359
    Plane* planes = ref<PlaneList>(camera, 0x1F8).data;              // 0045C35F
    if (planes != nullptr) live_free(planes);                          // 0045C367..0045C372
    gob_destruct_0044b2f0(as_gob(atf(camera, 4)), nullptr);             // 0045C381
    ref<const CameraBaseVTable*>(camera, 0) = &camera_base_vtable_007418f8;   // 0045C38A
}
// 0045D910 Camera scalar deleting destructor (Camera vtbl+00), RET 4
U __fastcall camera_destruct_deleting_0045d910(void* camera, void*, U flags) {
    camera_destruct_0045c330(camera, nullptr);                         // 0045D913
    if ((static_cast<U8>(flags) & 1u) != 0) live_free(camera);         // 0045D918..0045D920
    return word(camera);                                               // 0045D928
}
// 0045C250 CAurCamera scalar deleting destructor (interface vtbl+00), RET 4
U __fastcall camera_base_destruct_deleting_0045c250(void* camera, void*, U flags) {
    ref<const CameraBaseVTable*>(camera, 0) = &camera_base_vtable_007418f8;   // 0045C258
    if ((static_cast<U8>(flags) & 1u) != 0) live_free(camera);         // 0045C25E..0045C261
    return word(camera);                                               // 0045C269
}
// 0045C440 Camera::AsCAurObject (Camera vtbl+04): this ? this+4 : 0
void* __fastcall camera_as_aur_object_0045c440(void* camera, void*) { return camera != nullptr ? at(camera, 4) : nullptr; }
// 00401590 return_zero_0x10 (ICF; Camera vtbl+10): xor eax,eax; ret 10h
U __fastcall camera_return_zero_00401590(void*, void*, U, U, U, U) { return 0; }
// 0045C450 Camera::GetPosition(out) (Camera vtbl+20), RET 4
Vector* __fastcall camera_get_position_0045c450(void* camera, void*, Vector* out) {
    *out = ref<Vector>(camera, 0x7C);                                // 0045C454..0045C467
    return out;                                                        // 0045C450 mov eax,[esp+4]
}
// 0045C160 Camera::BehaviorMessage -> Gob::BehaviorMessage(this+4) (tail jmp)
U __fastcall camera_behavior_message_0045c160(void* camera, void*, U message) {
    return gob_behavior_message_0043f5a0(as_gob(atf(camera, 4)), nullptr, message);
}
// 00641DB0 self_return (ICF; Camera vtbl+70): mov eax,ecx
void* __fastcall camera_self_return_00641db0(void* self, void*) { return self; }
// 0045C210 Camera::AttachToPart -> Gob::AttachToPart(this+4) (tail jmp, RET 0Ch)
U __fastcall camera_attach_to_part_0045c210(void* camera, void*, U a, U b, U c) {
    return gob_attach_to_part_004439a0(as_gob(atf(camera, 4)), nullptr, a, b, c);
}
// 0045C230 Camera::SetBehavior -> Gob::SetBehavior(this+4)
U __fastcall camera_set_behavior_0045c230(void* camera, void*, void* behavior) {
    return gob_set_behavior_0043f1d0(as_gob(atf(camera, 4)), nullptr, behavior);
}
// 0045C220 Camera::GetBehavior -> Gob::GetBehavior(this+4)
U __fastcall camera_get_behavior_0045c220(void* camera, void*, U a) {
    return gob_get_behavior_0043f130(as_gob(atf(camera, 4)), nullptr, a);
}
// 0045C530 Camera::ViewVolumeSet: mov eax,[ecx+1F4h]
U __fastcall camera_view_volume_set_0045c530(void* camera, void*) { return field<U>(camera, 0x1F4); }
// 0045C4E0 Camera::SetFadePlanes(p1, p2) (Camera vtbl+90), RET 8: raw copies to +214 / +224
void __fastcall camera_set_fade_planes_0045c4e0(void* camera, void*, const Plane* first, const Plane* second) {
    std::memcpy(atf(camera, 0x214), first, 16);                         // 0045C4E4..0045C4FE
    std::memcpy(atf(camera, 0x224), second, 16);                        // 0045C501..0045C51E
}

// ---- Camera-embedded Gob table 00741988 thunks (this = camera+4)
namespace { inline void* camera_of(aurora_model::Gob* gob) { return static_cast<unsigned char*>(static_cast<void*>(gob)) - 4; } }
U __fastcall camera_gob_destructor_0045c3a0(aurora_model::Gob* gob, void*, U flags) { return camera_destruct_deleting_0045d910(camera_of(gob), nullptr, flags); }  // sub ecx,4; jmp
U __fastcall camera_gob_as_aur_camera_0045c430(aurora_model::Gob* gob, void*) { return word(camera_of(gob)); }   // lea eax,[ecx-4]
// 0045BEA0: Render only when this camera is not the one currently rendering (and one is), then Gob::Render
void __fastcall camera_gob_render_0045bea0(void* gob, void*, U flag) {
    void* current = *cells.current_camera_007bb4e8;                    // 0045BEA0
    if (current == static_cast<unsigned char*>(gob) - 4 || current == nullptr) return;   // 0045BEA8..0045BEAE (RET 4)
    gob_render_00478260(gob, nullptr, flag);                           // 0045BEB0 jmp
}
U __fastcall camera_gob_animate_0045d7c0(aurora_model::Gob* gob, void*, U dt) { return camera_animate_0045d220(camera_of(gob), nullptr, dt); }
aurora_model::I __fastcall camera_gob_attach_to_scene_0045c3b0(aurora_model::Gob* gob, void*, aurora_model::Scene* scene) {
    return camera_attach_to_scene_0045c0a0(camera_of(gob), nullptr, scene);
}
void* __fastcall camera_gob_attach_to_object_0045c3c0(void* gob, void*, void* object, const char* part, I mode) {
    return camera_attach_to_object_0045c200(static_cast<unsigned char*>(gob) - 4, nullptr, object, part, mode);
}
U __fastcall camera_gob_attach_to_part_0045c3d0(aurora_model::Gob* gob, void*, U a, U b, U c) { return camera_attach_to_part_0045c210(camera_of(gob), nullptr, a, b, c); }
U __fastcall camera_gob_set_behavior_0045c3e0(aurora_model::Gob* gob, void*, void* behavior) { return camera_set_behavior_0045c230(camera_of(gob), nullptr, behavior); }
Vector* __fastcall camera_gob_set_position_0045c3f0(aurora_model::Gob* gob, void*, Vector* out, Vector) {
    const U* p = A2_STACK_ARGS() + 1;                                  // sub ecx,4 ; jmp 0045C030 (the words passed on)
    return words_form<SetPositionWords>(&camera_set_position_0045c030)(camera_of(gob), nullptr, out, p[0], p[1], p[2]);
}
U __fastcall camera_gob_set_orientation_0045c400(aurora_model::Gob* gob, void*, U a, U b, U c, U d, U e) {
    return camera_set_orientation_0045c060(camera_of(gob), nullptr, a, b, c, d, e);
}
U __fastcall camera_gob_get_position_0045d7d0(aurora_model::Gob* gob, void*, U out) {
    return word(camera_get_position_0045c450(camera_of(gob), nullptr, reinterpret_cast<Vector*>(static_cast<std::uintptr_t>(out))));
}
U __fastcall camera_gob_get_orientation_0045d7e0(aurora_model::Gob* gob, void*, U out) { return camera_get_orientation_0045c470(camera_of(gob), nullptr, out); }
U __fastcall camera_gob_behavior_message_0045c410(aurora_model::Gob* gob, void*, U message) { return camera_behavior_message_0045c160(camera_of(gob), nullptr, message); }
// [M45 compose] 005B5E90 (ret 8) body removed: D68 earliest-staged owner lane-gui-load gui_load_slot_ret8_005b5e90 (M44) (definition removed; declaration kept)
U __fastcall camera_gob_void_return_8_005b5e90(aurora_model::Gob*, void*, U, U);   // 005B5E90 ret 8 (EAX unset: no defined value)
U __fastcall camera_gob_get_behavior_0045c420(aurora_model::Gob* gob, void*, U a) { return camera_get_behavior_0045c220(camera_of(gob), nullptr, a); }
void* __fastcall camera_gob_as_aur_object_0045d7f0(aurora_model::Gob* gob, void*) { return camera_as_aur_object_0045c440(camera_of(gob), nullptr); }

// ================================================================ 00471860 PartLightsaber::Draw (Part vtbl+64, RET 4)
// Frame 3B4h. Lightsaber node arrays (node = MdlNode::AsMdlNodeLightsaber(part+4), 00471883):
//   +19C vertices (Vector*), +1A0 texcoords (float[2]*), +1A4 normals (Vector*).
// Part history ring (length .rdata 00740A4C = 20): +98 float time[], +FC Vector position[], +228 Quaternion
//   orientation[], +3B8 u16 current index, +3BC i16 history count.
// .rdata i16 constants (movsx reads): 00740A48 = 20, 00740A4C = 20, 00740A50 = 8, 00740A54 = 4.
namespace {
using I16 = std::int16_t;
constexpr I kSaberRing_00740a4c = 20, kSaberRows_00740a48 = 20, kSaberBlade_00740a50 = 8, kSaberStrip_00740a54 = 4;
constexpr U kGlQuadStrip_0073f350 = 8;                                   // GL_QUAD_STRIP
constexpr U kOne_0073d764 = 0x3F800000u, kZero_0073d700 = 0u;   // kMinusOne_00740cbc defined above
inline I16 ring_back(I16 index) { return index < 0 ? static_cast<I16>(index + kSaberRing_00740a4c) : index; }   // add cx/ax,[00740A4C]
// L3 t1 a2 (writing rule, re-derived from 00471860..004738B5): the original copies every glTexCoord2f / glNormal3f /
// glVertex4f argument either with mov (raw words) or with fld ; fstp dword (an x87 copy, which quiets a signalling NaN),
// differently per call site. The helpers below are the patterns; each call site uses the one its original code has.
// saber_texcoord_x87: both texcoords fld/fstp-copied (00471A12..00471A32, 00471C3C..00471C62, 004730F3..0047310D,
// 00473210..0047322A, 004735F1..0047360B): two (float) conversions, one per original store, at five original sites.
__declspec(noinline) void saber_texcoord_x87(const float* tc) {
    const U v = stored_bits(W(tc[1]));                                 // fld [tc+4] ; fstp dword
    const U u = stored_bits(W(tc[0]));                                 // fld [tc] ; fstp dword
    words_form<GlWords2>(&glTexCoord2f)(u, v);
}
__forceinline void saber_texcoord_raw(const float* tc) {               // mov copies (e.g. 00471A96..00471AB3)
    const U u = raw(&tc[0]), v = raw(&tc[1]);
    words_form<GlWords2>(&glTexCoord2f)(u, v);
}
__forceinline void saber_normal_raw(const Vector* n) {                 // mov copies (00471A38..00471A55 and the other three)
    const U x = raw(&n->x), y = raw(&n->y), z = raw(&n->z);
    words_form<GlWords3>(&glNormal3f)(x, y, z);
}
__forceinline void saber_vertex_raw(const U* v) {                      // mov copies of a local result, w = 1.0
    words_form<GlWords4>(&glVertex4f)(v[0], v[1], v[2], kOne_0073d764);
}
// The simple path's vertices: x and z by mov, y through fld ; fstp dword (00471A5B..00471A90, 00471AD9..00471B05,
// 00471C00..00471C32, 00471C8B..00471CBC): one (float) conversion at four original sites.
__declspec(noinline) void saber_vertex_y87(const Vector* v) {
    const U x = raw(&v->x), z = raw(&v->z);
    const U y = stored_bits(W(v->y));
    words_form<GlWords4>(&glVertex4f)(x, y, z, kOne_0073d764);
}
// The original's inlined part-local transform (twelve copies, e.g. 00472162..00472236): the vertex is copied with mov,
// rotated by q (004A9BB0), the position p added (x, y kept on the x87 stack, z stored first), the part world
// position subtracted, and the result rotated by the conjugate (004A9B50) of the part world orientation. Four (float)
// conversions, one per original store, at each of the twelve sites.
__declspec(noinline) void saber_to_local(U* out, const void* q, const volatile U* p, const Vector* v,
                                         const Vector* world_position, const Quaternion* world_orientation) {
    U vw[3];
    copy_words3(vw, v);
    Vector rotated;
    quaternion_rotate_004a9bb0(&rotated, static_cast<const Quaternion*>(q), reinterpret_cast<const Vector*>(vw));
    const long double sx = W(fbits(p[0])) + W(rotated.x);             // fld [p.x] ; fadd [r.x]
    const long double sy = W(fbits(p[1])) + W(rotated.y);             // fld [p.y] ; fadd [r.y]
    const long double sz = S(W(fbits(p[2])) + W(rotated.z));          // fld [p.z] ; fadd [r.z] ; fstp dword (stored first)
    U wr[3];
    wr[0] = stored_bits(sx - W(world_position->x));                    // fsub [wp.x] ; fstp dword
    wr[1] = stored_bits(sy - W(world_position->y));                    // fsub [wp.y] ; fstp dword
    wr[2] = stored_bits(sz - W(world_position->z));                    // fld ; fsub [wp.z] ; fstp dword
    Quaternion inverse;
    const Quaternion* conj = quaternion_negate_w_004a9b50(&inverse, world_orientation);
    Vector local;
    quaternion_rotate_004a9bb0(&local, conj, reinterpret_cast<const Vector*>(wr));
    copy_words3(out, &local);                                          // mov copies of the result
}
using SlerpWords = Quaternion* (__cdecl*)(Quaternion*, U, U, U, U, const void*, U);   // 004A9E00 (out, Quaternion by value, to*, t)
using ArcWords = Quaternion* (__cdecl*)(Quaternion*, U, U, U, U, U, U);                // 004AB630 (out, Vector, Vector)
__forceinline void copy_words4(volatile U* d, const volatile U* s) { const U a = s[0], b = s[1], c = s[2], e = s[3]; d[0] = a; d[1] = b; d[2] = c; d[3] = e; }
__forceinline void copy_w3(volatile U* d, const volatile U* s) { const U a = s[0], b = s[1], c = s[2]; d[0] = a; d[1] = b; d[2] = c; }
}

U __fastcall part_lightsaber_draw_00471860(void* part, void*, U) {
    using kotor_renderer_init::glEnable_0073d390;
    using kotor_renderer_init::glDisable_0073d22c;
    if (*cells.saber_enabled_0078e840 == 0) return 0;                  // 00471866..0047187A (EAX = 0)
    aurora_model::MdlNode* node = aurora_node_as_saber_0043e3a0(ref<aurora_model::MdlNode*>(part, 0x04), nullptr);   // 00471883
    Vector world_position; Quaternion world_orientation;               // E-388 / E-2C8
    {   U* p = reinterpret_cast<U*>(&world_position); p[0] = 0; p[1] = 0; p[2] = 0;          // 0047189B..004718C3
        U* q = reinterpret_cast<U*>(&world_orientation); q[3] = 0; q[2] = 0; q[1] = 0; q[0] = kOne_0073d764; }   // 004718CB..004718EC
    slot<SlotPartTransform>(part, 0x0C)(part, nullptr, &world_position, &world_orientation);   // 004718F7
    glDisable_0073d22c(*cells.gl_cull_face_0078d454);                  // 00471906
    if (*cells.saber_additive_00828028 != 0) {                         // 00471908..0047190F
        glBlendFunc(kGlOne_0073f278, kGlOne_0073f278);                 // 00471918
        glDepthMask(0);                                                // 00471920
        glStencilMask(0);                                              // 00471928
    }
    if (*cells.saber_lit_00828038 == 0) glDisable_0073d22c(*cells.gl_lighting_0078d45c);   // 0047192E..0047193E
    words_form<GlWords4>(&glColor4f)(kOne_0073d764, kOne_0073d764, kOne_0073d764, kOne_0073d764);   // 00471940..00471954
    glPolygonMode(kGlFrontAndBack_0073f2f4, kPolygonModes_0073f2e4[*cells.saber_wireframe_00828034 == 0 ? 1 : 0]);   // 0047195A..00471976 (sete)
    const Vector* vertices = ref<const Vector*>(node, 0x19C);        // 0047197C
    const float* texcoords = ref<const float*>(node, 0x1A0);         // 00471982
    const Vector* normals = ref<const Vector*>(node, 0x1A4);         // 0047198D
    const float* times = &ref<float>(part, 0x98);
    const Vector* positions = &ref<Vector>(part, 0xFC);
    const Quaternion* orientations = &ref<Quaternion>(part, 0x228);
    const I half = kSaberBlade_00740a50 / 2;                           // movsx; cdq; sub; sar 1
    const I first = kSaberStrip_00740a54 * kSaberRows_00740a48 + kSaberBlade_00740a50;   // 00471B60..00471B78 / E-314

    if (*cells.saber_simple_0078e828 != 0 && ref<I16>(part, 0x3BC) < 2) {   // 00471988..004719AD
        glBegin(kGlQuadStrip_0073f350);                                // 004719CC
#pragma clang loop unroll(disable)
        for (I i = 0; i < half; ++i) {                                 // 004719D2..00471B42
            saber_texcoord_x87(&texcoords[2 * i]);                     // 00471A12..00471A32
            saber_normal_raw(&normals[i]);                             // 00471A38..00471A55
            saber_vertex_y87(&vertices[i]);                            // 00471A5B..00471A90
            saber_texcoord_raw(&texcoords[2 * (i + half)]);            // 00471A96..00471AB3
            saber_normal_raw(&normals[i + half]);                      // 00471AB9..00471AD3
            saber_vertex_y87(&vertices[i + half]);                     // 00471AD9..00471B05
        }
        glEnd();                                                       // 00471B4E
        glBegin(kGlQuadStrip_0073f350);                                // 00471B5A
#pragma clang loop unroll(disable)
        for (I i = first; i < first + half; ++i) {                     // 00471B7A..00471D0B (bound re-read each pass)
            saber_texcoord_raw(&texcoords[2 * (i + half)]);            // 00471BC7..00471BDA
            saber_normal_raw(&normals[i + half]);                      // 00471BE0..00471BFA
            saber_vertex_y87(&vertices[i + half]);                     // 00471C00..00471C32
            saber_texcoord_x87(&texcoords[2 * i]);                     // 00471C3C..00471C62
            saber_normal_raw(&normals[i]);                             // 00471C68..00471C85
            saber_vertex_y87(&vertices[i]);                            // 00471C8B..00471CBC
        }
        glEnd();                                                       // 00471D11
        glPolygonMode(kGlFrontAndBack_0073f2f4, kPolygonModes_0073f2e4[1]);   // 00471D24
        glEnable_0073d390(*cells.gl_cull_face_0078d454);               // 00471D37
        glBlendFunc(kBlendSrc_0073f270, kBlendDst_0073f274);           // 00471D46
        glEnable_0073d390(*cells.gl_lighting_0078d45c);                // 00473896 (00471D52 jmp)
        glDepthMask(1);                                                // 0047389A
        glStencilMask(0xFF);                                           // 004738A5
        return 0;   // EAX = glStencilMask leftover in the original (no defined value)
    }

    // ---- trail path (00471D57). Frame words (volatile U: stored once, re-read per use, as the original's slots).
    *cells.saber_strip_count_00828030 = 0;                             // 00471D57
    long double accumulated = W(f(kZero_0073d700));                    // [esp+20h] = 0 (00471D5D), held as its stored value
    volatile U offsets[4][3];                                          // E-1D4 [esp+1F0h]
    volatile U blade[3];                                               // [esp+118h]
    blade[0] = stored_bits(W(vertices[half].x) - W(vertices[0].x));    // 00471D95..00471D99 fstp
    blade[1] = stored_bits(W(vertices[half].y) - W(vertices[0].y));    // 00471DA0..00471DA6 fstp
    blade[2] = stored_bits(W(vertices[half].z) - W(vertices[0].z));    // 00471DAD..00471DCF fstp
#pragma clang loop unroll(disable)
    for (int j = 0; j < 4; ++j) copy_w3(offsets[j], blade);            // 00471DB0..00471E24 (mov)
    const long double length = S(vector_magnitude_004ab100(reinterpret_cast<const Vector*>(const_cast<U*>(offsets[0]))));   // 00471E2B / 00471E30 fstp [esp+48h]
    const U16 current = ref<U16>(part, 0x3B8);                       // 00471E36
    I16 newer = ring_back(static_cast<I16>(current - 1));              // 00471E40..00471E4F (cx)
    I16 older = ring_back(static_cast<I16>(current - 2));              // 00471E56..00471E68 (word [esp+28h])
    volatile U position[3];                                            // [esp+74h]
    volatile U orientation[4];                                         // [esp+8Ch]
    volatile U previous_position[3];                                   // [esp+10Ch]
    volatile U previous_orientation[4];                                // [esp+174h]
    previous_orientation[3] = 0; previous_orientation[2] = 0; previous_orientation[1] = 0; previous_orientation[0] = kOne_0073d764;   // 00471EA3..00471EC4
    copy_w3(position, reinterpret_cast<const U*>(&positions[newer]));  // 00471ECF..00471EE5 (mov)
    copy_words4(orientation, reinterpret_cast<const U*>(&orientations[newer]));   // 00471EE9..00471F10 (mov)
    const auto to_local = [&](volatile U* out, const volatile U* q, const volatile U* p, const Vector* v) {
        saber_to_local(const_cast<U*>(out), const_cast<const U*>(q), p, v, &world_position, &world_orientation);
    };
#pragma clang loop unroll(disable)
    for (I k = 1; k < ref<I16>(part, 0x3BC); ++k) {                  // 00471F31..00471F45 / 0047242D..0047243F
        const I a = older, b = newer;                                  // 00471F60 / 00471F63
        long double fraction_wide = W(*cells.saber_step_0078e838) / (W(times[a]) - W(times[b]));   // 00471F66..00471F7F (fsub ; fdivr)
        volatile U fraction; fraction = stored_bits(fraction_wide);    // 00471F85 fst [esp+10h] (a frame word)
        if (fraction_wide < W(f(kOne_0073d764))) {                     // 00471F89..00471F94 (test 5; jp)
            for (;;) {
                const long double accumulated_wide = W(*cells.saber_step_0078e838) + accumulated;   // 00471FA0..00471FA6
                accumulated = S(accumulated_wide);                     // 00471FAA fst [esp+20h]
                if (accumulated_wide > W(*cells.saber_max_accum_00798b30)) break;   // 00471FAE..00471FB9 (test 41h; je)
                copy_w3(previous_position, position);                  // 00471FBF..00472015 (mov)
                copy_words4(previous_orientation, orientation);
                const long double fr = W(fbits(fraction));
                const Vector& to = positions[a];
                const Vector& from = positions[b];
                const long double ftx = fr * W(to.x);                  // 00471FC3 / 00472020 fmul [to.x]
                const long double fty = fr * W(to.y);                  // 00472026..0047202E
                const long double ftz = S(fr * W(to.z));               // 00472034..00472045 fstp [esp+154h]
                const long double inverse_fraction = W(f(kOne_0073d764)) - fr;   // 0047204C..00472052
                const long double ifx = inverse_fraction * W(from.x);  // 00472056..00472058
                const long double ify = S(inverse_fraction * W(from.y));   // 0047205A..0047205F fstp [esp+0A4h]
                const long double ifz = S(inverse_fraction * W(from.z));   // 00472066..0047206B fstp [esp+0A8h]
                position[0] = stored_bits(ifx + ftx);                  // 00472072..00472083 fadd st,st(2) ; fstp [esp+68h] -> mov
                position[1] = stored_bits(ify + fty);                  // 0047207C..00472093 fadd st,st(1) ; fstp [esp+6Ch] -> mov
                position[2] = stored_bits(ifz + ftz);                  // 0047209D..004720B4 fadd [esp+154h] ; fstp -> mov
                const U* ob = reinterpret_cast<const U*>(&orientations[b]);
                Quaternion slerped;
                const Quaternion* s = words_form<SlerpWords>(&quaternion_slerp_004a9e00)(&slerped, raw(&ob[0]), raw(&ob[1]), raw(&ob[2]), raw(&ob[3]),
                                                                        &orientations[a], fraction);   // 004720A4..004720EB
                copy_words4(orientation, reinterpret_cast<const U*>(s));   // 004720F0..00472118 (mov)
                *cells.saber_strip_count_00828030 = *cells.saber_strip_count_00828030 + 1;   // 0047211F..00472127
                glBegin(kGlQuadStrip_0073f350);                        // 0047212D
#pragma clang loop unroll(disable)
                for (I j = 0; j < kSaberStrip_00740a54; ++j) {         // 00472133..004723C0
                    saber_texcoord_raw(&texcoords[2 * (first + j)]);   // 00472162..00472175
                    volatile U near_vertex[3];
                    to_local(near_vertex, orientation, position, &vertices[first + j]);   // 0047217B..00472236
                    saber_vertex_raw(const_cast<const U*>(near_vertex));                   // 00472240..00472253
                    copy_w3(offsets[j], near_vertex);                  // 00472259..0047226C (mov)
                    saber_texcoord_raw(&texcoords[2 * (first + j)]);   // 0047226F..00472282
                    volatile U far_vertex[3];
                    to_local(far_vertex, previous_orientation, previous_position, &vertices[first + j]);   // 00472288..0047234C
                    saber_vertex_raw(const_cast<const U*>(far_vertex));                    // 00472356..00472369
                    offsets[j][0] = stored_bits(W(fbits(offsets[j][0])) - W(fbits(far_vertex[0])));   // 0047236F..00472377 fstp [esp+5Ch] -> mov
                    offsets[j][1] = stored_bits(W(fbits(offsets[j][1])) - W(fbits(far_vertex[1])));   // 0047237F..00472386 fstp [esp+60h] -> mov
                    offsets[j][2] = stored_bits(W(fbits(offsets[j][2])) - W(fbits(far_vertex[2])));   // 0047238A..0047239A fstp [esp+64h] -> mov
                }
                glEnd();                                               // 004723D8
                fraction_wide = W(*cells.saber_step_0078e838) / (W(times[a]) - W(times[b])) + W(fbits(fraction));   // 004723DE..004723F6
                fraction = stored_bits(fraction_wide);                 // 004723FA fst [esp+10h]
                if (!(fraction_wide < W(f(kOne_0073d764)))) break;     // 004723FE..00472409 (test 5; jnp loops)
            }
        }
        newer = older;                                                 // 0047240F / 00471F52 (word [esp+10h])
        older = ring_back(static_cast<I16>(older - 1));                // 00472417..00472428
    }
#pragma clang loop unroll(disable)
    for (I j = 0; j < kSaberStrip_00740a54; ++j) {                     // 00472445..00472542
        const long double magnitude_wide = vector_magnitude_004ab100(reinterpret_cast<const Vector*>(const_cast<U*>(offsets[j])));   // 00472463
        const U magnitude = stored_bits(magnitude_wide);               // 00472468 fst [esp+10h]
        if (magnitude_wide == W(f(kZero_0073d700))) continue;          // 0047246C..0047247A (test ah,44h; jnp)
        if (W(*cells.saber_min_ratio_0078e83c) * length > W(fbits(magnitude))) {   // 00472480..00472493 (test 41h; jne)
            copy_w3(offsets[j], blade);                                // 00472495..004724B1 (mov)
        } else {
            const long double inverse = W(f(kOne_0073d764)) / W(fbits(magnitude)); // 004724B9..004724C1
            const long double x = inverse * W(fbits(offsets[j][0]));   // 004724C5..004724C7 (ST)
            const long double y = S(inverse * W(fbits(offsets[j][1])));   // 004724C9..004724CE fstp [esp+0A4h]
            const long double z = S(inverse * W(fbits(offsets[j][2])));   // 004724D5..004724DA fstp [esp+0A8h]
            offsets[j][0] = stored_bits(x * length);                   // 004724E1..004724FA fstp [esp+80h] -> mov
            offsets[j][1] = stored_bits(y * length);                   // 004724F3..00472519 fstp [esp+84h] -> mov
            offsets[j][2] = stored_bits(z * length);                   // 00472507..0047252A fstp [esp+88h] -> mov
        }
    }
    glBegin(kGlQuadStrip_0073f350);                                    // 0047254F
#pragma clang loop unroll(disable)
    for (I j = 0; j < half; ++j) {                                     // 00472555..0047280D
        saber_texcoord_raw(&texcoords[2 * (first + half + j)]);        // 004725A7..004725BA
        volatile U tip[3];
        to_local(tip, orientation, position, &vertices[first + j]);    // 004725C0..0047267C
        U sum[3];
        sum[0] = stored_bits(W(fbits(offsets[j][0])) + W(fbits(tip[0])));   // 00472680..0047268D fstp [esp+0B4h]
        sum[1] = stored_bits(W(fbits(tip[1])) + W(fbits(offsets[j][1])));   // 00472694..0047269A fstp [esp+0B8h]
        sum[2] = stored_bits(W(fbits(tip[2])) + W(fbits(offsets[j][2])));   // 004726A1..004726BC fstp [esp+0BCh]
        saber_vertex_raw(sum);                                         // 004726C3..004726D1
        saber_texcoord_raw(&texcoords[2 * (first + j)]);               // 004726D7..004726EE
        volatile U base_vertex[3];
        to_local(base_vertex, orientation, position, &vertices[first + j]);   // 004726F4..004727AC
        saber_vertex_raw(const_cast<const U*>(base_vertex));           // 004727AF..004727C9
    }
    glEnd();                                                           // 00472813

    if (*cells.saber_glow_0078e834 != 0) {                             // 00472819..00472820
        accumulated = W(f(kZero_0073d700));                            // 00472836 [esp+20h] = 0
        const U16 now = ref<U16>(part, 0x3B8);                       // 0047282F
        I16 p1 = ring_back(static_cast<I16>(now - 1));                 // 0047283E..0047284D (word [esp+10h])
        I16 p2 = ring_back(static_cast<I16>(now - 2));                 // 00472852..00472864 (di / word [esp+28h])
        volatile U qb[4], qa[4], qc[4];                                // [esp+124h] / [esp+0C0h] / [esp+0ECh]
        qb[3] = 0; qb[2] = 0; qb[1] = 0; qb[0] = kOne_0073d764;        // 00472870..00472891
        qa[3] = 0; qa[2] = 0; qa[1] = 0; qa[0] = kOne_0073d764;        // 0047289C..004728BD
        qc[3] = 0; qc[2] = 0; qc[1] = 0; qc[0] = kOne_0073d764;        // 004728C8..004728E9
        volatile U pa[3], pb[3];                                       // [esp+5Ch] / [esp+68h]
        copy_w3(pa, reinterpret_cast<const U*>(&positions[p1]));       // 004728F4..0047291A
        copy_words4(qa, reinterpret_cast<const U*>(&orientations[p1]));   // 00472906..00472935
        copy_w3(pb, reinterpret_cast<const U*>(&positions[p2]));       // 0047293C..0047296D
        copy_words4(qb, reinterpret_cast<const U*>(&orientations[p2]));   // 00472955..004729A5
        copy_w3(position, pa);                                         // 00472984..004729CE
        copy_words4(orientation, qa);                                  // 00472990..004729E0
#pragma clang loop unroll(disable)
        for (I k = 1; k < ref<I16>(part, 0x3BC); ++k) {              // 004729B7..004729EB / 0047347F..00473491
            // extrapolated target T = 2*pa - pb: doubling in ST, -pb via fmul -1.0f (00740CBC)
            const long double two_x = W(fbits(pa[0])) + W(fbits(pa[0]));   // 004729F1..004729F7 (fadd st,st)
            const long double two_y = W(fbits(pa[1])) + W(fbits(pa[1]));   // 004729FC..00472A03
            const long double two_z = S(W(fbits(pa[2])) + W(fbits(pa[2])));   // 00472A0C..00472A25 fstp [esp+27Ch]
            const long double minus_x = W(fbits(pb[0])) * W(f(kMinusOne_00740cbc));   // 00472A2C..00472A31
            const long double minus_y = S(W(fbits(pb[1])) * W(f(kMinusOne_00740cbc)));   // 00472A3E..00472A5B fstp [esp+254h]
            const long double minus_z = S(W(fbits(pb[2])) * W(f(kMinusOne_00740cbc)));   // 00472A62..00472A6D fstp [esp+258h]
            volatile U target[3];                                      // [esp+80h]
            target[0] = stored_bits(minus_x + two_x);                  // 00472A74..00472A76 fadd st,st(2) ; fstp
            target[1] = stored_bits(minus_y + two_y);                  // 00472A7D..00472A86 fadd st,st(1) ; fstp
            target[2] = stored_bits(minus_z + two_z);                  // 00472A91..00472A9F fadd [esp+27Ch] ; fstp
            volatile U b0[3], b3[3], a0[3], a3[3];
            to_local(b0, qb, pb, &vertices[0]);                        // 004729F5..00472B21 ([esp+1E4h])
            to_local(b3, qb, pb, &vertices[3]);                        // 00472B30..00472BFB
            volatile U from_edge[3];
            from_edge[0] = stored_bits(W(fbits(b3[0])) - W(fbits(b0[0])));   // 00472C01..00472C16 fstp [esp+14Ch]
            from_edge[1] = stored_bits(W(fbits(b3[1])) - W(fbits(b0[1])));   // 00472C21..00472C3B fstp [esp+150h]
            from_edge[2] = stored_bits(W(fbits(b3[2])) - W(fbits(b0[2])));   // 00472C42..00472C60 fstp [esp+154h]
            to_local(a0, qa, pa, &vertices[0]);                        // 00472C25..00472CFE ([esp+1F0h])
            to_local(a3, qa, pa, &vertices[3]);                        // 00472D0D..00472DD5
            volatile U to_edge[3];
            to_edge[0] = stored_bits(W(fbits(a3[0])) - W(fbits(a0[0])));   // 00472DDB..00472DF0 fstp [esp+174h]
            to_edge[1] = stored_bits(W(fbits(a3[1])) - W(fbits(a0[1])));   // 00472DFE..00472E14 fstp [esp+184h]
            to_edge[2] = stored_bits(W(fbits(a3[2])) - W(fbits(a0[2])));   // 00472E1B..00472E32 fstp [esp+188h]
            Quaternion arc;
            const Quaternion* arc_result = words_form<ArcWords>(&rotation_arc_004ab630)(&arc, from_edge[0], from_edge[1], from_edge[2],
                                                                                        to_edge[0], to_edge[1], to_edge[2]);   // 00472E0D..00472E68 (mov)
            copy_words4(qc, reinterpret_cast<const U*>(arc_result));   // 00472E6D..00472E9C
            Quaternion product;
            const Quaternion* product_result = quaternion_multiply_004a9ac0(&product, reinterpret_cast<const Quaternion*>(const_cast<U*>(qa)),
                                                                            reinterpret_cast<const Quaternion*>(const_cast<U*>(qc)));   // 00472E94..00472EAC
            copy_words4(qc, reinterpret_cast<const U*>(product_result));   // 00472EB1..00472EE0
            const float* time_older = &times[p2];                      // 00472ED6..00472EF3
            const float* time_newer = &times[p1];                      // 00472ED1..00472EF9
            long double fraction_wide = W(*cells.saber_step_0078e838) / (W(*time_older) - W(*time_newer));   // 00472EEE..00472EFD
            volatile U fraction; fraction = stored_bits(fraction_wide);   // 00472F03 fst [esp+10h] (a frame word)
            if (fraction_wide < W(f(kOne_0073d764))) {                 // 00472F07..00472F12 (test 5; jp)
                for (;;) {
                    const long double accumulated_wide = W(*cells.saber_step_0078e838) + accumulated;   // 00472F20..00472F26
                    accumulated = S(accumulated_wide);                 // 00472F2A fst [esp+20h]
                    if (accumulated_wide > W(*cells.saber_max_accum_00798b30)) break;   // 00472F2E..00472F39
                    copy_w3(previous_position, position);              // 00472F46..00472F88 (mov)
                    copy_words4(previous_orientation, orientation);    // 00472F6C..00472FCF (mov)
                    const long double fr = W(fbits(fraction));
                    const long double scaled_x = W(fbits(target[0])) * fr;     // 00472F3F..00472F4A
                    const long double scaled_y = fr * W(fbits(target[1]));     // 00472F52..00472F5A
                    const long double scaled_z = S(fr * W(fbits(target[2])));  // 00472F68..00472F8F fstp [esp+240h]
                    const long double inverse_fraction = W(f(kOne_0073d764)) - fr;   // 00472F96..00472FA3
                    const long double pax = W(fbits(pa[0])) * inverse_fraction;   // 00472FAE..00472FC0
                    const long double pay = S(inverse_fraction * W(fbits(pa[1])));   // 00472FC2..00472FD6 fstp [esp+188h]
                    const long double paz = S(inverse_fraction * W(fbits(pa[2])));   // 00472FDD..00472FE3 fstp [esp+18Ch]
                    position[0] = stored_bits(pax + scaled_x);         // 00472FEA..00473001 fstp [esp+0B4h] -> mov
                    position[1] = stored_bits(pay + scaled_y);         // 00472FFA..00473023 fstp [esp+0BCh] -> mov
                    position[2] = stored_bits(paz + scaled_z);         // 00473030..00473057 fstp [esp+0D4h] -> mov
                    Quaternion slerped;
                    const Quaternion* s = words_form<SlerpWords>(&quaternion_slerp_004a9e00)(&slerped, qa[0], qa[1], qa[2], qa[3],
                                                                            const_cast<const U*>(qc), fraction);   // 00473005..00473081
                    copy_words4(orientation, reinterpret_cast<const U*>(s));   // 00473086..004730A6
                    *cells.saber_strip_count_00828030 = *cells.saber_strip_count_00828030 + 1;   // 004730AD..004730BD
                    glBegin(kGlQuadStrip_0073f350);                    // 004730C3
#pragma clang loop unroll(disable)
                    for (I j = 0; j < kSaberStrip_00740a54; ++j) {     // 004730C9..00473373
                        saber_texcoord_x87(&texcoords[2 * j]);         // 004730F3..0047310D
                        volatile U far_vertex[3];
                        to_local(far_vertex, previous_orientation, previous_position, &vertices[j]);   // 00473113..004731D5
                        saber_vertex_raw(const_cast<const U*>(far_vertex));                            // 004731DA..004731F4
                        copy_w3(offsets[j], far_vertex);               // 004731FA..0047320D (mov)
                        saber_texcoord_x87(&texcoords[2 * j]);         // 00473210..0047322A
                        volatile U near_vertex[3];
                        to_local(near_vertex, orientation, position, &vertices[j]);   // 00473230..004732F2
                        saber_vertex_raw(const_cast<const U*>(near_vertex));         // 004732F7..00473311
                        offsets[j][0] = stored_bits(W(fbits(offsets[j][0])) - W(fbits(near_vertex[0])));   // 00473317..00473323 fstp [esp+0A0h] -> mov
                        offsets[j][1] = stored_bits(W(fbits(offsets[j][1])) - W(fbits(near_vertex[1])));   // 00473331..00473338 fstp [esp+0A4h] -> mov
                        offsets[j][2] = stored_bits(W(fbits(offsets[j][2])) - W(fbits(near_vertex[2])));   // 0047333F..00473352 fstp [esp+0A8h] -> mov
                    }
                    glEnd();                                           // 00473384
                    fraction_wide = W(*cells.saber_step_0078e838) / (W(*time_older) - W(*time_newer)) + W(fbits(fraction));   // 0047338A..0047339C
                    fraction = stored_bits(fraction_wide);             // 004733A0 fst [esp+10h]
                    if (!(fraction_wide < W(f(kOne_0073d764)))) break; // 004733A4..004733AF
                }
            }
            copy_w3(pb, pa);                                           // 004733B5..004733FE
            copy_words4(qb, qa);                                       // 004733C5..00473409
            copy_w3(pa, target);                                       // 004733E1..00473440
            copy_words4(qa, qc);                                       // 00473434..00473466
            p1 = p2;                                                   // 0047341E..0047343B (word [esp+10h])
            p2 = ring_back(static_cast<I16>(p2 - 1));                  // 0047343F..0047347A
        }
        glBegin(kGlQuadStrip_0073f350);                                // 0047349D
#pragma clang loop unroll(disable)
        for (I j = 0; j < half; ++j) {                                 // 004734A3..0047384E
            const long double magnitude_wide = vector_magnitude_004ab100(reinterpret_cast<const Vector*>(const_cast<U*>(offsets[j])));   // 004734E5
            const U magnitude = stored_bits(magnitude_wide);           // 004734EA fst [esp+10h]
            if (magnitude_wide != W(f(kZero_0073d700))) {              // 004734EE..004734FC (test 44h; jnp skips when equal)
                if (W(*cells.saber_min_ratio_0078e83c) * length > W(fbits(magnitude))) {   // 00473502..00473515
                    offsets[j][0] = stored_bits(-W(fbits(blade[0])));  // 00473517..00473524 fld ; fchs ; fstp [esp+0A0h] -> mov
                    offsets[j][1] = stored_bits(-W(fbits(blade[1])));  // 00473532..0047353D
                    offsets[j][2] = stored_bits(-W(fbits(blade[2])));  // 00473544..00473557
                } else {
                    const long double inverse = W(f(kOne_0073d764)) / W(fbits(magnitude));   // 0047356D..00473577
                    const long double x = inverse * W(fbits(offsets[j][0]));    // 0047357F..00473581 (ST)
                    const long double y = S(inverse * W(fbits(offsets[j][1])));   // 00473583..00473588 fstp [esp+150h]
                    const long double z = S(inverse * W(fbits(offsets[j][2])));   // 0047358F..00473594 fstp [esp+154h]
                    const long double minus_length = S(-length);       // 0047359B..004735A1 fchs ; fst [esp+54h]
                    offsets[j][0] = stored_bits(minus_length * x);     // 004735A5..004735B5 fmul st(1) ; fstp -> mov
                    offsets[j][1] = stored_bits(minus_length * y);     // 004735B9..004735DD
                    offsets[j][2] = stored_bits(minus_length * z);     // 004735CB..004735EE
                }
            }
            saber_texcoord_x87(&texcoords[2 * j]);                     // 004735F1..0047360B
            volatile U base_vertex[3];
            to_local(base_vertex, orientation, position, &vertices[j]);   // 00473611..004736D0
            saber_vertex_raw(const_cast<const U*>(base_vertex));       // 004736D5..004736EF
            saber_texcoord_raw(&texcoords[2 * (half + j)]);            // 004736F5..0047370C
            volatile U tip[3];
            to_local(tip, orientation, position, &vertices[j]);        // 00473712..004737CC
            U sum[3];
            sum[0] = stored_bits(W(fbits(offsets[j][0])) + W(fbits(tip[0])));   // 004737D1..004737E5 fstp [esp+16Ch]
            sum[1] = stored_bits(W(fbits(offsets[j][1])) + W(fbits(tip[1])));   // 004737EC..004737F2 fstp [esp+170h]
            sum[2] = stored_bits(W(fbits(offsets[j][2])) + W(fbits(tip[2])));   // 004737F9..00473815 fstp [esp+174h]
            saber_vertex_raw(sum);                                     // 0047381C..0047382A
        }
        glEnd();                                                       // 00473854
    }
    glPolygonMode(kGlFrontAndBack_0073f2f4, kPolygonModes_0073f2e4[1]);   // 00473867
    glBlendFunc(kBlendSrc_0073f270, kBlendDst_0073f274);               // 0047387A
    glEnable_0073d390(*cells.gl_lighting_0078d45c);                    // 0047388D
    glEnable_0073d390(*cells.gl_cull_face_0078d454);                   // 00473896
    glDepthMask(1);                                                    // 0047389A
    glStencilMask(0xFF);                                               // 004738A5
    return 0;   // EAX = glStencilMask leftover in the original (no defined value)
}

} // extern "C"
