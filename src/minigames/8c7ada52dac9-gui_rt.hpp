#pragma once
// Lane W4 G15 in-game GUI construction: shared runtime for CGuiInGame::Initialize and the panel constructors.
// Generic CSWGui* widget layer = existing runtime1115 providers (exact map symbols; ABI per gui-load headers:
// D1 __fastcall(self, unused EDX, stack words), callee cleanup). Pointer types are erased to void* here; the
// C-linkage decorated names (@N) carry the stack size and are verified by tools/check_undefined.py.
//
// Vtables: an original panel table is reproduced as a writable 4-byte-slot table built once from the EXISTING
// runtime1115 base table of its direct base class (byte copy; the base tables are the generated typed tables of the
// gui-load / m48 owners) plus the derived class's override slots. A slot is overridden exactly where the original
// derived table differs from the original base table (build/hudtools/vt2.py). Override functions are W4 bodies or
// explicit kotor_unreachable_trap stubs at the original slot function's address.
//
// Exceptions: provider constructors are noexcept(false). Constructors here keep the original EH unwind order with a
// fixed-capacity Unwind guard (no heap use): members are destroyed in reverse order of construction, then the base.
#include "3d9f1bbdade6-w4_providers.hpp"
#include <initializer_list>

namespace menu_resources {
struct Ref { unsigned char bytes[16]; Ref* __thiscall construct_00406d80(const char* text); };  // CResRef(const char*)
}
static_assert(sizeof(menu_resources::Ref) == 16, "CResRef");
namespace kotor_menu_10_19 { struct Tlk; }
namespace kotor_input_hover { struct Control; }
namespace kotor_m1_shared {
struct PanelEventThunks {                                                                            // CSWGuiPanel event members
    void __thiscall AcceptButtonCallback_00624ba0(kotor_input_hover::Control*);                       // 00624BA0
    void __thiscall OnBButtonPressed_00624bb0(kotor_input_hover::Control*);                           // 00624BB0
    void __thiscall OnXButtonPressed_00624bc0(kotor_input_hover::Control*);                           // 00624BC0
};
}
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }   // CExoString()
namespace kotor_reconstruction { struct StartupString; }
namespace tlk_core {
void __fastcall ParseStr0041dd30(kotor_menu_10_19::Tlk*, void*, kotor_reconstruction::StartupString*);  // CTlkTable::ParseStr
}

extern "C" {
// ---- panel (CSWGuiPanel)
void* __fastcall gui_load_panel_construct_0040b570(void* self, void*, void* manager) noexcept(false);   // 0040B570
void __fastcall gui_load_panel_destruct_0040cf70(void* self, void*);                                  // 0040CF70
void __fastcall gui_load_panel_start_layout_0040a680(void* self, void*, void* resref);                // 0040A680
void __fastcall gui_load_panel_init_control_0040b930(void* self, void*, void* control, void* name, std::int32_t flag); // 0040B930
void __fastcall gui_load_panel_stop_layout_0040b8f0(void* self, void*);                               // 0040B8F0
void __fastcall gui_load_panel_set_extent_0040a5a0(void* self, void*, const void* extent);            // 0040A5A0
void __fastcall gui_m48_panel_center_0040a600(void* self, void*);                                     // 0040A600
void __fastcall gui_m48_panel_add_control_0040b9a0(void* self, void*, void* control);                 // 0040B9A0
void __fastcall gui_load_panel_set_alpha_00404b00(void* self, void*, float alpha);                    // 00404B00 CSWGuiPanel::SetPanelAlpha
void __fastcall gui_load_panel_set_active_control_0040a630(void* self, void*, void* control, std::uint32_t); // 0040A630
// ---- controls
void* __fastcall gui_load_label_construct_0041acd0(void* self, void*) noexcept(false);               // 0041ACD0
void __fastcall gui_load_label_destruct_00419d10(void* self, void*);                                  // 00419D10
void* __fastcall gui_load_button_construct_0041c0f0(void* self, void*) noexcept(false);              // 0041C0F0
void __fastcall gui_load_button_destruct_00419da0(void* self, void*);                                 // 00419DA0
void* __fastcall gui_load_listbox_construct_0041be40(void* self, void*) noexcept(false);             // 0041BE40
void __fastcall gui_load_listbox_destruct_0041b060(void* self, void*);                                // 0041B060
void __fastcall gui_load_control_add_event_0041ab20(void* control, void*, std::int32_t id, void* object, const void* func) noexcept(false); // 0041AB20
void __fastcall gui_load_text_params_set_text_00415e00(void* params, void*, void* string) noexcept(false);   // 00415E00
void __fastcall gui_load_text_params_set_strref_00415e50(void* params, void*, std::uint32_t strref) noexcept(false); // 00415E50
void __fastcall gui_load_text_params_set_color_00414e10(void* params, void*, const void* color);      // 00414E10
// ---- message box family (m48b owner)
void __fastcall gui_m48_message_box_destruct_006270a0(void* self, void*);                             // 006270A0
void __fastcall kotor_fastcall_00627130(void* self, void*, std::uint32_t allow);                     // CSWGuiMessageBox::SetAllowCancel
// ---- strings / misc
void __fastcall frame_LogStringCtor_005e5a90(void* self, void*, const char* text);                   // CExoString(const char*)
void __fastcall frame_LogStringDtor_005e5c20(void* self, void*);                                      // ~CExoString
void* __fastcall WindowCopy005e5b00(void* self, void*, void* source);                                 // CExoString(const&)
void* __fastcall kotor_fastcall_005eda40(void* app, void*);                                           // GetSWGuiManager
void __fastcall kotor_m15_client_set_load_bar_progress_005edd40(void* app, void*, std::int32_t progress, std::uint32_t flag); // SetLoadBarProgress
void __cdecl kotor_cdecl_00401c10(float, unsigned, unsigned);                                         // UpdateScreen
extern void* volatile kotor_cell_tlk_007a3a08;                                                        // [007A3A08] CTlkTable*
void* __fastcall GuiAssign005e5140(void* self, void*, const char* text);                             // CExoString::operator=(const char*)
void* __fastcall async_assign_string_005e5c50(void* self, void*, void* source);                     // CExoString::operator=(const&)
void __cdecl frame_LogFormat_005e5680(void* string, const char* format, ...);                        // CExoString::Format
const char* __fastcall frame_LogCStr_005e5670(void* string, void*);                                  // CExoString::CStr
void __fastcall kotor_fastcall_00409ee0(void* manager, void*, std::int32_t* x, std::int32_t* y);   // CSWGuiManager::GetSafeMargin (W4)
// existing real panel constructors (m48b / fade owners)
void* __fastcall gui_fade_construct_00624810(void* self, void*, void* manager);                      // CSWGuiFade
}
namespace kotor_original {
void* __fastcall kotor_fastcall_00626df0(void* self, void*, void* manager);                           // CSWGuiMessageBox
void* __fastcall kotor_fastcall_006ce7c0(void* self, void*, void* manager);                           // CSWGuiSkillInfoBox
void* __fastcall kotor_fastcall_006aa100(void* self, void*, void* manager);                           // CSWGuiTutorialBox
}
// existing runtime1115 base method tables (byte-copied by vt_build)
extern "C" const unsigned char gui_load_panel_methods_0073e010[];         // CSWGuiPanel (27 slots)
extern "C" const unsigned char gui_m48_message_box_methods_0074fdb0[];    // CSWGuiMessageBox (33 slots)
extern "C" const unsigned char gui_load_label_methods_0073e5b8[];         // CSWGuiLabel (40 slots)
extern "C" const unsigned char gui_load_button_methods_0073e658[];        // CSWGuiButton (40 slots)
extern "C" const unsigned char gui_load_listbox_methods_0073e840[];       // CSWGuiListBox
extern "C" const unsigned char kotor_vtable_007513f8[];                   // CSWGuiControllerLossBox (m48b)

// Dismissible scope guard: runs its cleanup unless dismissed at the normal end of the scope (= only while an
// exception unwinds, the original EH state's cleanup), without an explicit rethrow in extern "C" bodies.
namespace w4gui {
template <class F> struct OnThrow {
    F f; bool armed = true;
    ~OnThrow() { if (armed) f(); }
};
template <class F> OnThrow<F> on_throw(F f) { return OnThrow<F>{f}; }
}
// Event handler referenced only by address (thiscall(CSWGuiControl*) RET 4) and not reconstructed: explicit trap.
#define W4_GUI_TRAP_HANDLER(addr, name) \
    void __fastcall kotor_fastcall_##addr(void*, void*, void*) { kotor_unreachable_trap(0x##addr##u, name); }

namespace w4gui {
using namespace w4;

// One-time built override table.
struct Override { U32 offset; const void* fn; };
inline void vt_build(void** table, U32 slots, const void* base, U32 base_slots, const Override* ovr, U32 n) {
    std::memcpy(table, base, base_slots * 4u);
    for (U32 i = base_slots; i < slots; ++i) table[i] = nullptr;
    for (U32 i = 0; i < n; ++i) table[ovr[i].offset / 4u] = const_cast<void*>(ovr[i].fn);
}
template <class F> inline const void* fn(F f) { return reinterpret_cast<const void*>(f); }
template <class PMF> inline const void* pmf_code(PMF p) { static_assert(sizeof(PMF) == 4, "single-inheritance PMF"); const void* v; std::memcpy(&v, &p, 4); return v; }
inline const void* on_accept() { return pmf_code(&kotor_m1_shared::PanelEventThunks::AcceptButtonCallback_00624ba0); }
inline const void* on_b() { return pmf_code(&kotor_m1_shared::PanelEventThunks::OnBButtonPressed_00624bb0); }
inline const void* on_x() { return pmf_code(&kotor_m1_shared::PanelEventThunks::OnXButtonPressed_00624bc0); }
inline void exo_default(void* s) { kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(s), nullptr); }
inline void set_vptr(void* self, const void* table) { sw(self, 0, ad(table)); }

// Fixed-capacity reverse-order unwinder (destructor runs only while unwinding unless dismissed).
struct Unwind {
    using Dtor = void(__fastcall*)(void*, void*);
    Dtor dtor[128]; void* object[128]; U32 count = 0; bool armed = true;
    void push(Dtor d, void* o) { dtor[count] = d; object[count] = o; ++count; }
    void dismiss() { armed = false; }
    ~Unwind() { if (armed) while (count) { --count; dtor[count](object[count], nullptr); } }
};

// Construction helpers (each mirrors the original call shape; see call sites for PCs).
inline void* label(void* self, U32 off, Unwind& u) {
    void* c = static_cast<U8*>(self) + off; gui_load_label_construct_0041acd0(c, nullptr); u.push(&gui_load_label_destruct_00419d10, c); return c; }
inline void* button(void* self, U32 off, Unwind& u) {
    void* c = static_cast<U8*>(self) + off; gui_load_button_construct_0041c0f0(c, nullptr); u.push(&gui_load_button_destruct_00419da0, c); return c; }
inline void* listbox(void* self, U32 off, Unwind& u) {
    void* c = static_cast<U8*>(self) + off; gui_load_listbox_construct_0041be40(c, nullptr); u.push(&gui_load_listbox_destruct_0041b060, c); return c; }
inline void start_layout(void* self, const char* resref) {
    menu_resources::Ref ref; ref.construct_00406d80(resref);                                     // CResRef(const char*)
    gui_load_panel_start_layout_0040a680(self, nullptr, &ref);                                   // StartLoadFromLayout
}
struct ExoString {
    U32 storage[2];
    explicit ExoString(const char* text) { frame_LogStringCtor_005e5a90(storage, nullptr, text); }
    ~ExoString() { frame_LogStringDtor_005e5c20(storage, nullptr); }
};
inline void init_control(void* self, U32 off, const char* name, std::int32_t flag = 1) {
    ExoString s(name);
    gui_load_panel_init_control_0040b930(self, nullptr, static_cast<U8*>(self) + off, s.storage, flag); // InitControl
}
inline void stop_layout(void* self) { gui_load_panel_stop_layout_0040b8f0(self, nullptr); }
inline void add_event(void* self, U32 off, std::int32_t id, void* target, const void* func) {
    gui_load_control_add_event_0041ab20(static_cast<U8*>(self) + off, nullptr, id, target, func);
}
inline void zero(void* self, U32 off) { sw(self, off, 0); }
// eh_vector_constructor_iterator (006FB55F) / eh_vector_destructor_iterator (006FA852, reverse order).
using Ctor0 = void* (__fastcall*)(void*, void*);
using Dtor0 = void (__fastcall*)(void*, void*);
inline void vector_construct(void* first, U32 stride, U32 count, Ctor0 ctor, Dtor0 dtor) {
    U32 built = 0;
    try { for (; built < count; ++built) ctor(static_cast<U8*>(first) + built * stride, nullptr); }
    catch (...) { while (built) { --built; dtor(static_cast<U8*>(first) + built * stride, nullptr); } throw; }
}
inline void vector_destruct(void* first, U32 stride, U32 count, Dtor0 dtor) {
    for (U32 i = count; i-- > 0;) dtor(static_cast<U8*>(first) + i * stride, nullptr);
}
// Embedded member array: construct, then register each element for reverse-order unwinding.
inline void vector_member(void* first, U32 stride, U32 count, Ctor0 ctor, Dtor0 dtor, Unwind& u) {
    vector_construct(first, stride, count, ctor, dtor);
    for (U32 i = 0; i < count; ++i) u.push(dtor, static_cast<U8*>(first) + i * stride);
}
// new T[count] with a count prefix word (operator new + eh_vector_constructor_iterator; raw block freed on throw).
inline void* new_vector(U32 count, U32 stride, Ctor0 ctor, Dtor0 dtor) {
    U8* raw = static_cast<U8*>(live_operator_new(count * stride + 4u));
    if (raw == nullptr) return nullptr;
    sw(raw, 0, count);
    try { vector_construct(raw + 4, stride, count, ctor, dtor); } catch (...) { live_free(raw); throw; }
    return raw + 4;
}
// delete[] of a polymorphic array: count != 0 -> element vector deleting dtor (slot 0, flags 3), else free(p-4).
inline void delete_vector(void* first) {
    if (first == nullptr) return;
    if (lw(static_cast<U8*>(first) - 4, 0) != 0) vcall<void*, U32>(first, 0x00, 3);
    else live_free(static_cast<U8*>(first) - 4);
}
inline void* at(void* self, U32 off) { return static_cast<U8*>(self) + off; }
inline void copy_extent(void* e, void* control) { std::memcpy(e, static_cast<U8*>(control) + 4, 16); }
inline void set_extent(void* control, const void* e) { vcall<void, const void*>(control, 0x04, e); }   // CSWGuiControl::SetExtent
// Scalar deleting destructor shape shared by every panel class (thiscall(flags) RET 4).
inline void* deleting(void* self, void (__fastcall* dtor)(void*, void*), U32 flags) {
    dtor(self, nullptr);
    if (flags & 1u) live_free(self);
    return self;
}
}  // namespace w4gui
