// lane-m48-b fork GUI: verbatim single-function extractions (D9 precedent) from lane-input r3 sources
//   lane-input/src/gui_manager_input.cpp (sha256 in FORK-PACKET.json): 0040B500, 0040C1E0, 0040B690, 00418A20, 005D5930
//   lane-input/src/input_system.cpp: 005E0D80 IsEventInClass
//   lane-input/src/gui_vtables.cpp + input_system.cpp:88: raw event-desc vtable 0074D5FC and its 0063E7F0 slot body.
// Not extracted (already single-owned in M47 under other spellings; joined in m48_gui_joins.cpp):
//   0040C150 Manager::RemoveToolTip -> core47 @kotor_fastcall_0040c150@8; 0040AA00 -> gui-load @gui_load_panel_offset_extent_0040aa00@12;
//   00418960 -> gui-load @gui_load_control_focus_change_00418960@12; 0041A5E0 -> gui-load @gui_load_listbox_hit_local_0041a5e0@12.
// Function bodies are byte-for-byte the lane-input text (instruction citations are lane-input's, verified against
// swkotor.exe by that lane); only the surrounding includes/declarations are assembled here.
#include "deeac869ec-hover_path.h"
#include "0e7d509bda-input_system.h"
#include "36201186c4-window_shared_cells.h"
#include "eddb76278d-allocator_live.h"
#include <cstring>

using namespace kotor_input_hover;

namespace kotor_sound_path_required {                      // lane-sound declaration (sound_internal.h)
std::uint32_t __fastcall source_play_005db4d0(void*, void*, std::uint32_t, std::uint32_t);  // RET 8, EAX = [src+58h]
}


// ---- 0040B500 CSWGuiManager::SetTemporarilyDisableToolTip(int) ----
void Manager::SetTemporarilyDisableToolTip(int disable) {
    if (!disable) {                                                        // 0040B506..0A
        ResetToolTipTimer();                                               // 0040B54D..4F
        tooltip_mode = 0;                                                  // 0040B554
        tooltip_item = 0;                                                  // 0040B558
        return;
    }
    Control* over = mouse_over;                                            // 0040B50C
    tooltip_timer = 0xBF800000u;                                           // 0040B511 (-1.0f bits)
    if (!over) { tooltip_mode = disable; tooltip_item = 0; return; }       // 0040B518 -> 0040B554
    if ((over->*(over->cvt()->as_listbox))()) {                            // 0040B51A..21
        Control* again = mouse_over;                                       // 0040B523 reload
        ListBox* lb = (again->*(again->cvt()->as_listbox))();              // 0040B526..2A
        const int item = lb->HitCheckMouseLocal(0);                        // 0040B528 push 0 / 0040B52F
        tooltip_mode = disable;                                            // 0040B534
        tooltip_item = static_cast<U32>(item);                             // 0040B538
        return;
    }
    tooltip_mode = disable;                                                // 0040B542
    tooltip_item = static_cast<U32>(reinterpret_cast<std::uintptr_t>(mouse_over));  // 0040B53F/46 (control word)
}

// ---- 0040C1E0 CSWGuiManager::HandleMouseMove(x, y) ----
void Manager::HandleMouseMove(int x, int y) {
    mouse_x = x;                                                           // 0040C1F6
    mouse_y = y;                                                           // 0040C1F8
    CursorModel* cursor = cursor_model;                                    // 0040C1EC
    if (cursor) {
        const float k = 0.01f;                                             // 0073DFEC (.rdata 0x3C23D70A)
        Vector3 v;
        v.x = static_cast<float>(static_cast<double>(x) * static_cast<double>(k));                    // 0040C1FD..21E (FILD/FMUL/FSTP)
        v.y = static_cast<float>(static_cast<double>(static_cast<int>(height) - y) * static_cast<double>(k));  // 0040C201..232
        v.z = 0.0f;                                                        // 0040C216
        (cursor->*(cursor->vtable->set_position))(&v, v);                  // 0040C23A..4B
    }
    Control* captured_control = captured;                                  // 0040C24E
    if (captured_control && (captured_control->*(captured_control->vtable->captured_movement))(x, y)) return;  // 0040C251..5E
    Panel* hit_panel; Control* hit;                                        // [esp+8] / [esp+4] outs
    HitCheckMouse(x, y, &hit_panel, &hit, 0);                              // 0040C264..74
    if (hit != mouse_over) {                                               // 0040C279..80
        if (flags & 8u) RemoveToolTip();                                   // 0040C282..8A
        SetTemporarilyDisableToolTip(0);                                   // 0040C28F..93
    }
    if (!hit) {                                                            // 0040C298..9A
        Control* old = mouse_over;                                         // 0040C29C
        if (old) {
            const U32 f = old->flags;                                      // 0040C2A3
            if ((f & 1u) && !(f & 4u)) {                                   // 0040C2A6..AC
                (old->*(old->vtable->handle_mouse_over))(0);               // 0040C2AE..B1 (EDI = 0)
                Control* p = mouse_over->GetSelectableParent();            // 0040C2B4..B7
                if (p) (p->*(p->vtable->handle_mouse_over))(0);            // 0040C2BC..E0
            } else {
                Control* p = old->GetSelectableParent();                   // 0040C2C2
                if (p && (p->flags & 1u) && !(mouse_over->flags & 4u))     // 0040C2C7..D8
                    (p->*(p->vtable->handle_mouse_over))(0);               // 0040C2DA..E0
            }
            mouse_over = nullptr;                                          // 0040C2E3
        }
        SetTemporarilyDisableToolTip(0);                                   // 0040C2EA..EE
        return;
    }
    if (tooltip_mode == 1 && !(hit->*(hit->cvt()->as_listbox))() &&        // 0040C2FD..0C
        hit != reinterpret_cast<Control*>(static_cast<std::uintptr_t>(tooltip_item)))  // 0040C30E (word compare)
        SetTemporarilyDisableToolTip(0);                                   // 0040C313..17
    Control* old = mouse_over;                                             // 0040C31C (EBX)
    mouse_over = hit;                                                      // 0040C31F
    if (!(hit->*(hit->cvt()->slot60))() &&                                 // 0040C322..2B
        !(mouse_over->*(mouse_over->cvt()->slot78))()) {                   // 0040C331..3B
        Panel* panel = hit_panel;                                          // 0040C341 (EBP)
        Control* cur = mouse_over;                                         // 0040C348
        if (cur != (panel->*(panel->pvt()->get_active_control))()) {       // 0040C345..52
            if ((cur->*(cur->cvt()->is_selectable))() &&                   // 0040C358..64
                (mouse_over->flags & 4u) && (mouse_over->flags & 8u)) {    // 0040C366..72
                if ((panel->*(panel->pvt()->get_active_control))()) {     // 0040C374..7E
                    Control* active = (panel->*(panel->pvt()->get_active_control))();  // 0040C380..85
                    (active->*(active->cvt()->handle_focus_change))(0);    // 0040C388..8E
                }
                (mouse_over->*(mouse_over->cvt()->handle_focus_change))(1);  // 0040C391..98
            } else {
                Control* p = mouse_over->GetSelectableParent();            // 0040C39D..A0
                if (p && !(p->*(p->cvt()->slot60))() && !(p->*(p->cvt()->slot78))()) {  // 0040C3A7..BF
                    const U32 f = p->flags;                                // 0040C3C1
                    const U32 b4 = (f >> 2) & 1u;                          // 0040C3C4..C9
                    if (b4 && (f & 8u)) {                                  // 0040C3CC..D1
                        if (p != (panel->*(panel->pvt()->get_active_control))())  // 0040C3D3..DD
                            (p->*(p->cvt()->handle_focus_change))(1);      // 0040C3DF..E5
                    } else if (!b4 && (f & 8u)) {                          // 0040C3EA..F1
                        (p->*(p->vtable->handle_mouse_over))(1);           // 0040C3F3..F9
                    }
                }
            }
        }
    }
    Control* cur = mouse_over;                                             // 0040C400
    int entered = 0;                                                       // 0040C403 (EBP)
    if (old != cur) {                                                      // 0040C405..07
        if ((cur->*(cur->cvt()->is_selectable))()) {                       // 0040C40D..17
            (mouse_over->*(mouse_over->vtable->handle_mouse_over))(1);     // 0040C419..20
            const U32 f = mouse_over->flags;                               // 0040C423..26
            if ((f & 4u) && (f & 8u)) entered = 1;                         // 0040C429..31
        }
        for (Control* p = mouse_over->parent_control; p; p = p->parent_control) {  // 0040C436..45
            if (p == old) continue;                                        // 0040C447..49
            if (!(p->*(p->cvt()->is_selectable))()) continue;              // 0040C44B..57
            (p->*(p->vtable->handle_mouse_over))(1);                       // 0040C459..5F
            if ((p->flags & 4u) && (p->flags & 8u)) entered = 1;           // 0040C462..6D
        }
        if (entered) {                                                     // 0040C474..76
            if (old) {                                                     // 0040C478..7A
                if (!old->parent_control && !old->GetIsChild(mouse_over))  // 0040C480..94
                    (old->*(old->vtable->handle_mouse_over))(0);           // 0040C496..9B
                if (old->GetSelectableParent())                            // 0040C49E..A7
                    (old->*(old->vtable->handle_mouse_over))(0);           // 0040C4A9..AF (receiver is OLD, observed)
            }
        } else if (old) {                                                  // 0040C4B4..B6
            const U32 f = old->flags;                                      // 0040C4B8
            if ((f & 1u) && !(f & 4u)) {                                   // 0040C4BB..C1
                (old->*(old->vtable->handle_mouse_over))(0);               // 0040C4C3..C9
                Control* p = old->GetSelectableParent();                   // 0040C4CC..CE
                if (p && p != mouse_over && p != mouse_over->GetSelectableParent())  // 0040C4D5..E7
                    (p->*(p->vtable->handle_mouse_over))(0);               // 0040C4E9..EF
            } else {
                Control* p = old->GetSelectableParent();                   // 0040C4F4..F6
                if (p && (p->flags & 1u) && !(p->flags & 4u) &&            // 0040C4FD..0A
                    p != mouse_over && p != mouse_over->GetSelectableParent())  // 0040C50C..1A
                    (p->*(p->vtable->handle_mouse_over))(0);               // 0040C51C..22
            }
        }
    }
    ListBox* lb = (hit->*(hit->cvt()->as_listbox))();                      // 0040C525..30 (hit reloaded)
    if (!lb) return;                                                       // 0040C532..34
    if (tooltip_mode == 1 &&                                               // 0040C536..3A
        static_cast<U32>(lb->HitCheckMouseLocal(0)) != tooltip_item)       // 0040C53C..48
        SetTemporarilyDisableToolTip(0);                                   // 0040C54A..4E
    lb->HandleMouseMove();                                                 // 0040C553..55
}

// ---- 0040B690 CSWGuiPanel::HitCheckMouse(x, y) ----
Control* Panel::HitCheckMouse(int px, int py) {
    Extent e = extent;                                                     // 0040B699..B7
    int lx, ly;
    if (!(flags & 1u)) {                                                   // 0040B6A7..BB
        const int w = static_cast<int>(kotor_renderer_gl_cells::width_0078d1d4); // 0040B6BD
        const int h = static_cast<int>(kotor_menu_window_cells::height_0078d1d8);  // 0040B6D4
        lx = static_cast<int>(static_cast<U32>(px) + static_cast<U32>(-((w - 0x280) / 2)));  // 0040B6C2..D2 (CDQ/SUB/SAR)
        ly = static_cast<int>(static_cast<U32>(py) + static_cast<U32>(-((h - 0x1E0) / 2)));  // 0040B6D9..EB
    } else {
        GetExtentAccountingForPanelOffset(&e);                             // 0040B6EF..F6
        lx = px; ly = py;                                                  // 0040B6FB..FF
    }
    if (lx < e.x) return nullptr;                                          // 0040B70B..0D
    if (lx > static_cast<int>(static_cast<U32>(e.width) + static_cast<U32>(e.x))) return nullptr;   // 0040B70F..17
    if (ly < e.y) return nullptr;                                          // 0040B719..1B
    if (ly > static_cast<int>(static_cast<U32>(e.height) + static_cast<U32>(e.y))) return nullptr;  // 0040B71D..25
    const int cx = static_cast<int>(static_cast<U32>(lx) - static_cast<U32>(e.x));  // 0040B72C
    const int cy = static_cast<int>(static_cast<U32>(ly) - static_cast<U32>(e.y));  // 0040B727
    for (int i = control_count - 1; i >= 0; --i) {                         // 0040B729..4B
        Control* c = controls[i];                                          // 0040B735..38
        if (!c) continue;                                                  // 0040B73B..3D
        Control* r = (c->*(c->cvt()->hit_check))(cx, cy);                  // 0040B73F..43
        if (r) return r;                                                   // 0040B746..48
    }
    return nullptr;                                                        // 0040B74D
}

// ---- 00418A20 CSWGuiControl::GetIsChild(control) ----
int Control::GetIsChild(Control* control) {
    const int n = child_count;                                             // 00418A20
    for (int i = 0; i < n; ++i)                                            // 00418A2A..3B
        if (children[i] == control) return 1;                              // 00418A31..33 / 00418A43
    return 0;                                                              // 00418A3D
}

// ---- 005D5930 CExoSoundSource::Play ----
// abi-a3: single owner of 005D5930 is @kotor_fastcall_005d5930@8 (w4-creature_ai), which returns the play length.
// This member is the C++ spelling the hover/object/anim callers (0040A165, 00647340, 0069A0D4) use; every one of
// those original sites discards EAX, so the member stays void and forwards to the owner.
extern "C" std::uint32_t __fastcall kotor_fastcall_005d5930(void* source, void*) noexcept(false);
void SoundSource::Play() {
    (void)kotor_fastcall_005d5930(this, nullptr);                         // 005D5930..005D5943 (owner)
}


// ======== input_system.cpp extractions ========
// ---- 005E0D80 IsEventInClass(desc_index, class) ----
int InputInternal::IsEventInClass(U32 desc_index, int input_class) {
    if (input_class < 0 || input_class >= 6) return 0;                     // 005E0D84..8F
    InputClass& c = classes[input_class];                                  // 005E0D95..9D (this+class*0x30; words at +0x2C)
    const int word = static_cast<int>(desc_index) / 32;                    // 005E0DA0..B0 (CDQ/AND/ADD/SAR)
    const int old_count = c.words.count;                                   // 005E0DAD
    if (word >= old_count) {                                               // 005E0DB3
        WordArraySetSize00405150(&c.words, nullptr, word * 2 + 1);         // 005E0DB8..C2
        for (int i = old_count; i < c.words.count; ++i) c.words.data[i] = 0;  // 005E0DC7..DD
    }
    const int bit = static_cast<int>(desc_index) % 32;                     // 005E0DE0..F3 (signed remainder)
    return (c.words.data[word] & (1u << (bit & 31))) ? 1 : 0;              // 005E0DF4..05
}

// 00405150 CExoArrayList<unsigned long>::SetSize: NOT extracted - single body = sibling fork CLIENT
// (lane-m48-b/client/src/client_movie.cpp @kotor_fastcall_00405150@12); lane-input's spelling is joined to it
// in m48_gui_joins.cpp.
// ---- 0074D5FC raw event-desc table (lane-input gui_vtables.cpp:140) and slot body 0063E7F0 (input_system.cpp:88) ----
int RawEventDesc::ReturnZero0063e7f0() { return 0; }                       // 0063E7F0 xor eax,eax
const RawEventDescVtable raw_event_desc_vtable_0074d5fc = { &RawEventDesc::ReturnZero0063e7f0, &RawEventDesc::ReturnZero0063e7f0 };

// ======== gui_manager_input.cpp extraction (verbatim)
// ---- 005EDA70 CClientExoApp::GetInputClass ----
U32 ClientApp::GetInputClass() { return internal->input_class; }           // 005EDA70..73


// ======== authored (lane-m48-b GUI fork): CExoInput::GetEvent 005DF4C0 / CExoInputInternal::GetEvent 005E2360
extern "C" {
void* __fastcall gui_m48_input_internal_get_event_005e2360(InputInternal* self, void*, U32 index, int input_class);
// ---- 005E2360 CExoInputInternal::GetEvent(ulong index, int class) (RET 8): description pointer or 0.
void* __fastcall gui_m48_input_internal_get_event_005e2360(InputInternal* self, void*, U32 index, int input_class) {
    unsigned char* const p = reinterpret_cast<unsigned char*>(self);
    U32 descs; std::memcpy(&descs, p + 0x128, 4);                                 // [esi+128h]
    if (input_class >= 0 && input_class < 6) {                                     // 005E236C..71
        if (!self->IsEventInClass(index, input_class)) return nullptr;             // 005E2374..81 (005E0D80)
        void* d; std::memcpy(&d, reinterpret_cast<const unsigned char*>(static_cast<std::uintptr_t>(descs)) + index * 4, 4);  // 005E2383..89
        return d;
    }
    U32 count; std::memcpy(&count, p + 0x12C, 4);                                 // [esi+12Ch]
    if (input_class == -1 && index < count) {                                      // 005E2392..A1 (unsigned JAE)
        void* d; std::memcpy(&d, reinterpret_cast<const unsigned char*>(static_cast<std::uintptr_t>(descs)) + index * 4, 4);  // 005E23A3..A9
        return d;
    }
    return nullptr;                                                                // 005E23B1..BD (EBX = 0)
}
// ---- 005DF4C0 CExoInput::GetEvent(ulong, int): ECX = [ECX+4]; JMP 005E2360.
void* __fastcall gui_m48_input_get_event_005df4c0(void* input, void*, U32 index, int input_class) {
    InputInternal* internal; std::memcpy(&internal, static_cast<const unsigned char*>(input) + 4, 4);
    return gui_m48_input_internal_get_event_005e2360(internal, nullptr, index, input_class);
}
}

// ======== authored (lane-m48-b GUI fork): CExoInputInternal::ClearEvents 005E0710, CExoRawInputInternal::SetActive
// 005E3C20 and the two DirectInput buffer flushes 005E3010 / 005E3080 (IDirectInputDevice8A slots +1C Acquire,
// +20 Unacquire, +28 GetDeviceData; DIERR_NOTACQUIRED 8007000Ch, DIERR_INPUTLOST 8007001Eh).
namespace kotor_menu_10_19 {
struct TimerDependency { unsigned __int64 __thiscall high_resolution_005e9260(); };
namespace dependencies {
extern void* base_007a39e0;
unsigned __int64 __stdcall unsigned_divide_006fb030(unsigned __int64, unsigned __int64);
}
}
namespace base_ini_reconstruction { void* __fastcall remove_head_005e9c80(kotor_list::Internal*, void*); }   // ini-list-cleanup
extern "C" {
std::uint32_t __fastcall unresolved_list_next_005e9b90(void*, void*, void**);
std::uint32_t __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);
void __fastcall gui_m48_raw_clear_keyboard_buffer_005e3010(RawInput* self, void*);
void __fastcall gui_m48_raw_clear_mouse_buffer_005e3080(RawInput* self, void*);
void __fastcall kotor_fastcall_005e0710(void* internal, void*);
void __fastcall kotor_fastcall_005e3c20(void* object, void*, int active);
}
namespace {
// 005E3010 / 005E3080 shared loop: GetDeviceData(0x14, NULL, &count = INFINITE, 0) until it succeeds; on NOTACQUIRED
// Acquire and retry only while Acquire reports INPUTLOST; on INPUTLOST Acquire once.
void flush_device(RawInput* self, IDirectInputDevice8A* RawInput::*device) {
    for (;;) {
        IDirectInputDevice8A* d = self->*device;                                  // 005E3030 / 005E3092
        DWORD count = 0xFFFFFFFFu;                                                 // 005E303E / 005E30A0
        const HRESULT hr = d->GetDeviceData(0x14, nullptr, &count, 0);             // 005E3033..49 / 005E3095..AB
        if (hr == 0) return;                                                       // 005E304C..4E
        if (hr == static_cast<HRESULT>(0x8007000Cu)) {                             // 005E3050..55
            const HRESULT again = (self->*device)->Acquire();                      // 005E3057..5D
            if (again == static_cast<HRESULT>(0x8007001Eu)) continue;              // 005E3060..65
            return;
        }
        if (hr == static_cast<HRESULT>(0x8007001Eu)) (self->*device)->Acquire();   // 005E306A..77
        return;
    }
}
}
extern "C" {
// ---- 005E3010 CExoRawInputInternal::ClearKeyboardBuffer (RET) ----
void __fastcall gui_m48_raw_clear_keyboard_buffer_005e3010(RawInput* self, void*) {
    if (self->keyboard_mode != 0) return;                                          // 005E3014..19
    if (!self->keyboard) return;                                                   // 005E301B..20
    if (!self->active) return;                                                     // 005E3022..27
    flush_device(self, &RawInput::keyboard);                                       // 005E3030..77
}
// ---- 005E3080 CExoRawInputInternal::ClearMouseBuffer (RET) ----
void __fastcall gui_m48_raw_clear_mouse_buffer_005e3080(RawInput* self, void*) {
    if (!self->mouse) return;                                                      // 005E3084..89
    if (!self->active) return;                                                     // 005E308B..90
    flush_device(self, &RawInput::mouse);                                          // 005E3092..D9
}
// ---- 005E0710 CExoInputInternal::ClearEvents (RET) ----
void __fastcall kotor_fastcall_005e0710(void* internal, void*) {
    InputInternal* self = static_cast<InputInternal*>(internal);
    if (!self->raw) return;                                                        // 005E0715..1F
    gui_m48_raw_clear_mouse_buffer_005e3080(self->raw, nullptr);                   // 005E0725
    gui_m48_raw_clear_keyboard_buffer_005e3010(self->raw, nullptr);                // 005E072A..30
    for (int i = 0; i < self->descs.count; ++i) {                                  // 005E0735..6F (count reloaded)
        U32 word; std::memcpy(&word, reinterpret_cast<const unsigned char*>(self->descs.data) + i * 4, 4);
        EventDesc* d = reinterpret_cast<EventDesc*>(static_cast<std::uintptr_t>(word));   // 005E0741..47
        if (!d) continue;                                                          // 005E074A..4C
        switch (d->type) {                                                         // 005E074E..56 (table 005E07EC)
        case 4: d->alt_accum = 0; d->accum = 0; d->state = 0; break;               // 005E075D..63
        case 3: d->accum = 0; d->state = 0; break;                                 // 005E0760..63
        case 0: case 1: case 2: case 5: d->state = 0; break;                       // 005E0763
        default: break;                                                            // > 5 (JA 005E0766)
        }
    }
    kotor_list::Internal* repeats = self->repeats.internal;                        // 005E0771
    void* pos; std::memcpy(&pos, repeats, sizeof pos);                             // 005E0777..7B (head)
    while (pos) {                                                                  // 005E077F / 005E07A9
        repeats = self->repeats.internal;                                          // 005E0781
        unsigned char* entry = reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(
            unresolved_list_at_pos_005e9b60(repeats, nullptr, pos)));             // 005E0787..88
        const U32 zero = 0;
        std::memcpy(entry + 0x0C, &zero, 4);                                       // 005E0791 (no NULL check in the original)
        std::memcpy(entry + 0x18, &zero, 4);                                       // 005E0794
        unresolved_list_next_005e9b90(self->repeats.internal, nullptr, &pos);      // 005E0797..9E
    }
    for (;;) {                                                                     // 005E07AB..E5
        kotor_list::Internal* buffered = self->buffered.internal;
        void* head; std::memcpy(&head, buffered, sizeof head);
        if (!head) break;                                                          // 005E07B1..B3 / 005E07E3..E5
        unsigned char* node = static_cast<unsigned char*>(base_ini_reconstruction::remove_head_005e9c80(buffered, nullptr));   // 005E07B6
        if (node) {                                                                // 005E07BD..BF
            void* desc; std::memcpy(&desc, node + 0x0C, sizeof desc);              // 005E07C1
            if (desc) live_free(desc);                                             // 005E07C4..C9 (006FA390)
            live_free(node);                                                       // 005E07D4..D5
        }
    }
}
// ---- 005E3C20 CExoRawInputInternal::SetActive(int) (RET 4) ----
void __fastcall kotor_fastcall_005e3c20(void* object, void*, int active) {
    RawInput* self = static_cast<RawInput*>(object);
    if (active == self->active) return;                                            // 005E3C28..2B
    if (IDirectInputDevice8A* kb = self->keyboard) {                               // 005E3C31..36
        if (active == 1) { if (self->keyboard_mode == 0) kb->Acquire(); }          // 005E3C38..47
        else kb->Unacquire();                                                      // 005E3C4C..4F
    }
    if (IDirectInputDevice8A* mouse = self->mouse) {                               // 005E3C52..57
        if (active == 1) mouse->Acquire(); else mouse->Unacquire();                // 005E3C59..68
    }
    for (int i = 0; i < self->joystick_count; ++i) {                               // 005E3C6B..9C (count reloaded)
        IDirectInputDevice8A** sticks = self->joysticks;                           // 005E3C76
        if (!sticks || !sticks[i]) continue;                                       // 005E3C79..82
        if (active == 1) sticks[i]->Acquire(); else sticks[i]->Unacquire();        // 005E3C84..93
    }
    self->active = active;                                                         // 005E3CA0
    if (!active) return;                                                           // 005E3C9E..A3
    gui_m48_raw_clear_keyboard_buffer_005e3010(self, nullptr);                     // 005E3CA5..A7
    gui_m48_raw_clear_mouse_buffer_005e3080(self, nullptr);                        // 005E3CAC..AE
    for (int i = 0; i < self->joystick_count; ++i) {                               // 005E3CB3..F9
        unsigned char* state = static_cast<unsigned char*>(self->word2c) + i * 0x74;   // 005E3CC0..CA ([esi+2Ch] + i*74h)
        std::memset(state, 0, 0x50);                                               // 005E3CC3..CC (REP STOSD x14h)
        struct BaseView { void* word00; kotor_menu_10_19::TimerDependency* timers; };
        const unsigned __int64 now = static_cast<BaseView*>(kotor_menu_10_19::dependencies::base_007a39e0)->timers->high_resolution_005e9260();   // 005E3CCE..D6
        const U32 ms = static_cast<U32>(kotor_menu_10_19::dependencies::unsigned_divide_006fb030(now, 1000u));   // 005E3CDB..E4
        std::memcpy(static_cast<unsigned char*>(self->word2c) + i * 0x74 + 0x50, &ms, 4);   // 005E3CE9..EC (array reloaded)
    }
}
}

// ======== owner patch (lane W5 t2b, coordinator ruling): extern "C" export of 005E0D80 for cross-lane callers.
// No behaviour change: forwards to the existing member body above. Thiscall RET 8 -> fastcall(self, edx, a, b).
extern "C" int __fastcall gui_m48_input_internal_is_event_in_class_005e0d80(InputInternal* self, void*, U32 desc_index, int input_class) {
    return self->IsEventInClass(desc_index, input_class);
}
