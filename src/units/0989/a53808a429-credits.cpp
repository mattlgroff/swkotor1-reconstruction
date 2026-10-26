// lane-m1-movies-play / credits sub-fork (Milestone 1): the "credits" entry of the Movies panel.
//   005EE1C0 CClientExoApp::StartCreditSequence        005F4C60 CClientExoAppInternal::StartCreditSequence
//   005EE1D0 CClientExoApp::StopCreditSequence         005F4D50 CClientExoAppInternal::StopCreditSequence
//   005EE1B0 CClientExoApp::FadeAndStopStreamingSoundSource (-> 005F1B20, sound-donor-fade owner)
//   005F4E50 CClientExoAppInternal::SetEvilEndingHackInProgress
//   0062FB90 CGuiInGame::FadeAndStopDialogAmbientTrack
//   CSWGuiInGameCredits (0x5F8; operator new at 005F4C88): ctor 0068F8D0, dtor 0068F0E0, deleting dtor 0068F6D0,
//   OnPanelAdded 0068F260, HandleInputEvent 0068F350, SetText 0068F3A0, UpdateScrolling 0068F630,
//   SetNextText 0068F6F0, UpdateFading 0068F790, Draw 0068F880, method table 007541F0 (27 slots).
// Original: swkotor.exe GOG K1 (image base 0x400000), static disassembly only; nothing executed.
// Conventions: D1 fastcall veneers for thiscall entries (callee cleanup identical), D21 typed method table,
// D24 object views of method tables, D47 member event callback (AcceptButtonCallback 00624BA0 via m1-shared),
// D30-FINAL: x87 wide intermediates mirrored as doubles, float only at the original fstp/fst dword stores,
// __ftol2 through the original CRT port genuine_x87_ftol_006fae8c; D3 raw float<->word via memcpy.
// credits-a1: the former D83 branch trap at 005F4D1D is replaced by the original branch: [[[this+18h]+48h]+1DCh]
// CSWCAmbientSound -> StopSounds(0) 0068E1B0 (owner lane-w3 recovered_client_ambient_sound_stop_sounds_0068e1b0).
#include "9bac6f8338-gui_objects.h"               // gui_load views, gui_lb::ListBox, kotor_input_hover, tlk/c2da/text/listbox owners
#include "95913ca8e0-mainmenu.h"                  // kotor_fastcall_0040bc70 (AddPanel), kotor_fastcall_005ee180 (ShowMouse)
#include "b976ed802d-m1_shared.h"                 // kotor_m1_shared::PanelEventThunks::AcceptButtonCallback_00624ba0 (L0-shared)
#include "015caeaf6d-allocator_live.h"            // live_operator_new (006FA7E6) / live_free (006FA390)
#include "cda8452790-gui_size.h"                  // gui_size::Gui::set_size_0040be70 (CSWGuiManager::SetSize owner)
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>

using kotor_reconstruction::StartupString;
using gui_load::Word;
using gui_load::I32;
namespace kih = kotor_input_hover;

// ---------------------------------------------------------------- existing owners (declarations repeated exactly)
namespace kotor_input_required { void* __fastcall get_in_game_gui_005ed690(void* app, void* unused_edx); }  // r4-lowers 005ED690
namespace kotor_game_prepare { void __fastcall SetGlobalUnpauseableState0062b410(void* self, void* edx, std::uint32_t); } // 0062B410
namespace menu_normal_40_49 { struct MusicInternal; }
namespace table29_required {                                         // sound/sound_path.h (verbatim)
std::uint32_t __fastcall gui_play_005d5b10(void*, void*, menu_resources::Ref*,
    std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);    // RET 14h
void* __fastcall gui_sound_construct_005d5a50(void*, void*);
void __fastcall gui_sound_destroy_005d6200(void*, void*);
std::uint32_t __fastcall gui_playing_005d5af0(void*, void*);
void __fastcall gui_priority_005d5ba0(void*, void*, std::uint32_t);
void __fastcall gui_fade_005f1b20(menu_normal_40_49::MusicInternal*, void*, void*, std::uint32_t); // sound-fade-r8
}
extern "C" {
void __fastcall kotor_fastcall_005ee170(void* outer, void*, std::uint32_t);    // routes-window-leaves: CClientExoApp::HideMouse
void __fastcall kotor_fastcall_005d5b80(void* source, void*);                 // m48b client-dtors: CExoStreamingSoundSource::Stop
void __cdecl kotor_cdecl_00403de0();                                          // m48b client-resolution: RestoreResolutionFromMovie
unsigned __fastcall frame_GetPauseState_004ae980(void*, void*, unsigned);     // r4-lowers: CServerExoApp::GetPauseState
void __fastcall gui_m48_client_set_input_class_005eda60(void* app, void*, std::uint32_t input_class, std::uint32_t show) noexcept(false);
Word __fastcall gui_m48_client_get_input_class_005eda70(void* app, void*);   // joined to lane-input 005EDA70
void __fastcall gui_m48_panel_add_control_0040b9a0(gui_load::Panel* panel, void*, gui_load::Control* control) noexcept(false);
void __fastcall gui_m48_listbox_add_controls_0041c2c0(gui_lb::ListBox*, void*, gui_load::Control** controls, I32 count,
    Word suppress, Word restore, Word variable) noexcept(false);              // m48b-gui-controls 0041C2C0
// lane-startup-misc C2DA owners (extern "C" spellings as the reviewed gui_sounds consumer declares them)
void* __fastcall c2da_current_construct_00413cc0(void* table, void*, menu_resources::Ref name, std::int32_t mode);
int __fastcall c2da_current_load_004143b0(void* table, void*);
std::int32_t __fastcall c2da_current_get_int_00413660(void* table, void*, std::int32_t row, StartupString* column, std::int32_t* value);
StartupString* __fastcall GuiAssign005e5140(StartupString*, void*, const char*);   // ini-string-priority 005E5140
void __cdecl genuine_x87_ftol_006fae8c();                                     // renderer-x87-ftol-alias owner (__ftol2)
// lane-w3 sound unit (w3_client_area.h, verbatim): 0068E1B0 CSWCAmbientSound::StopSounds(BOOL), thiscall RET 4.
void __fastcall recovered_client_ambient_sound_stop_sounds_0068e1b0(void* self, void*, std::uint32_t) noexcept(false);
}

// ---------------------------------------------------------------- cells owned here (no other original reader/writer)
extern "C" {
// .data 007A2670 = 43988000h (305.0f); its only reference in the image is the read at 0068F775 (orig.asm scan).
std::uint32_t kotor_cell_credits_scroll_span_007a2670 = 0x43988000u;
// .bss 00833BB8..00833BC4: saved 0078D1D8 / manager height / manager width / saved 0078D1D4 for the evil-ending
// 640x480 credits; written only at 0068F9C9..9EF, read only at 0068F19B..1B2.
std::uint32_t kotor_cell_credits_saved_height_00833bb8 = 0;
std::uint32_t kotor_cell_credits_gui_height_00833bbc = 0;
std::uint32_t kotor_cell_credits_gui_width_00833bc0 = 0;
std::uint32_t kotor_cell_credits_saved_width_00833bc4 = 0;
}

namespace kotor_m1_credits {
// CSWGuiInGameCredits (0x5F8: operator new(5F8h) at 005F4C88). Offsets from the ctor 0068F8D0 / dtor 0068F0E0.
struct Credits : gui_load::Panel {
    gui_lb::ListBox lb_credits;      // +064 "LB_CREDITS" (ctor 0041BE40 at 0068F90F, InitControl 0068F98E)
    gui_load::Label text_item;       // +344 list item label (0068F91F; Initialize from LB proto 0068FAC3)
    gui_load::Label click_area;      // +484 full-screen label (0068F931; event 27h -> 00624BA0 at 0068FB11)
    unsigned char music[8];          // +5C4 CExoString (005B3190 at 0068F941) = the sequence name (0068FC42)
    void* source;                    // +5CC CExoStreamingSoundSource* (0068FC4E = 0; 0068F2FF)
    std::int32_t* strrefs;           // +5D0 int[count] (0068FB3C = 0; 0068FBAB)
    float scroll_time;               // +5D4 (0068FC54; 0068F670)
    float fade_time;                 // +5D8 (0068FC5A; 0068F7A3)
    Word saved_input_class;          // +5DC (0068FC60 = 2; 0068F2B4)
    unsigned char index;             // +5E0 (0068FC6A; 0068F781)
    unsigned char u5e1[3];
    Word fading;                     // +5E4 (0068FC70 = 1; 0068F713)
    Word fade_in;                    // +5E8 (0068FC76 = 1; 0068F7EE / 0068F83A)
    unsigned char count;             // +5EC (0068FB36 = 0; 0068FBA0)
    unsigned char u5ed[3];
    float scroll_interval;           // +5F0 (0068FC3C / 0068FC7C = 0; 0068F77B)
    Word u5f4;                       // +5F4 (no reference found in the class code)
};
static_assert(offsetof(Credits, lb_credits) == 0x64 && offsetof(Credits, text_item) == 0x344 &&
              offsetof(Credits, click_area) == 0x484 && offsetof(Credits, music) == 0x5C4 &&
              offsetof(Credits, source) == 0x5CC && offsetof(Credits, strrefs) == 0x5D0 &&
              offsetof(Credits, scroll_time) == 0x5D4 && offsetof(Credits, fade_time) == 0x5D8 &&
              offsetof(Credits, saved_input_class) == 0x5DC && offsetof(Credits, index) == 0x5E0 &&
              offsetof(Credits, fading) == 0x5E4 && offsetof(Credits, fade_in) == 0x5E8 &&
              offsetof(Credits, count) == 0x5EC && offsetof(Credits, scroll_interval) == 0x5F0 &&
              sizeof(Credits) == 0x5F8, "CSWGuiInGameCredits 0x5F8");
}
using kotor_m1_credits::Credits;

extern "C" {
void __fastcall kotor_fastcall_005ee1c0(void* client_app, void* unused_edx, void* callback12, StartupString* name) noexcept(false);
void __fastcall kotor_fastcall_005f4c60(void* internal, void* unused_edx, void* callback12, StartupString* name) noexcept(false);
void __fastcall kotor_fastcall_005ee1d0(void* client_app, void* unused_edx);
void __fastcall kotor_fastcall_005f4d50(void* internal, void* unused_edx);
void __fastcall kotor_fastcall_005ee1b0(void* client_app, void* unused_edx, void* source, std::uint32_t duration);
void __fastcall kotor_fastcall_005f4e50(void* internal, void* unused_edx, std::uint32_t value);
void __fastcall kotor_fastcall_0062fb90(void* in_game, void* unused_edx);
Credits* __fastcall kotor_fastcall_0068f8d0(Credits* self, void* unused_edx, gui_load::Object* manager, StartupString* name) noexcept(false);
void __fastcall kotor_fastcall_0068f0e0(Credits* self, void* unused_edx);
void* __fastcall kotor_fastcall_0068f6d0(Credits* self, void* unused_edx, Word flags);
void __fastcall kotor_fastcall_0068f260(gui_load::Panel* self, void* unused_edx) noexcept(false);
void __fastcall kotor_fastcall_0068f350(gui_load::Object* self, void* unused_edx, I32 event, I32 value);
void __fastcall kotor_fastcall_0068f3a0(Credits* self, void* unused_edx, Word strref, Word resize) noexcept(false);
void __fastcall kotor_fastcall_0068f630(Credits* self, void* unused_edx, float delta);
void __fastcall kotor_fastcall_0068f6f0(Credits* self, void* unused_edx) noexcept(false);
void __fastcall kotor_fastcall_0068f790(Credits* self, void* unused_edx, float delta) noexcept(false);
void __fastcall kotor_fastcall_0068f880(gui_load::Panel* self, void* unused_edx, Word delta);
extern const struct KotorM1CreditsMethods kotor_m1_credits_methods_007541f0;
}

namespace {
void* word_ptr(const void* p, unsigned off) { void* v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
Word word_at(const void* p, unsigned off) { Word v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
void put_word(void* p, unsigned off, Word v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, 4); }
Word word_of(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
Word float_bits(float f) { Word w; std::memcpy(&w, &f, 4); return w; }
float bits_float(Word w) { float f; std::memcpy(&f, &w, 4); return f; }
void* app_manager() { return kotor_menu_window_cells::manager_007a39fc; }                // [007A39FC]
void* client_app() { return word_ptr(app_manager(), 4); }                                 // [[007A39FC]+4]
void* client_internal() { return word_ptr(client_app(), 4); }                             // [[[007A39FC]+4]+4]
gui_load::Object* gui_manager() { return static_cast<gui_load::Object*>(kotor_input_cells::gui_manager_007a39f4); } // [007A39F4]
gui_size::Gui* gui_size_view(gui_load::Object* m) { return reinterpret_cast<gui_size::Gui*>(m); }
std::int32_t manager_width() { std::int16_t v; std::memcpy(&v, reinterpret_cast<unsigned char*>(gui_manager()) + 0x6C, 2); return v; } // movsx [+6Ch]
std::int32_t manager_height() { std::int16_t v; std::memcpy(&v, reinterpret_cast<unsigned char*>(gui_manager()) + 0x6E, 2); return v; } // movsx [+6Eh]
StartupString* music(Credits* self) { return reinterpret_cast<StartupString*>(self->music); }
unsigned char* bytes(Credits* self) { return reinterpret_cast<unsigned char*>(self); }
// CSWGuiInGameCredits+42C = text_item.text params (+344 +D0 +18); +454 = its alpha float (params +28);
// +414 = text_item.text (CSWGuiText).
void* text_params(Credits* self) { return bytes(self) + 0x42C; }
void set_text_alpha(Credits* self, Word bits) { put_word(self, 0x454, bits); }

// CSWGuiLabel method table 0073E5B8 slot +98 (00418CD0 Initialize; 3 stack words, RET 0Ch), as typed in gui_vtables.
using LabelInit98 = void (__fastcall*)(gui_load::Object*, void*, Word, Word, Word);
struct LabelInitView { const void* s00_94[0x26]; LabelInit98 initialize98; };
static_assert(offsetof(LabelInitView, initialize98) == 0x98, "label +98");
// CSWGuiText method table 0073E3A4 slot +08 (00414EB0 GetIdealHeight).
using TextIdeal08 = Word (__fastcall*)(gui_load::Text*, void*);
struct TextIdealView { const void* s00; const void* s04; TextIdeal08 ideal_height; };
static_assert(offsetof(TextIdealView, ideal_height) == 0x08, "text +08");

// CTlkTable STR_RES {CExoString text; CResRef sound; tail} (0x1C; 0068F3F1..0068F402 locals [esp+44h]/[esp+4Ch]).
struct StrRes { unsigned char text[8]; menu_resources::Ref sound; unsigned char tail[4]; };
static_assert(sizeof(StrRes) == 0x1C, "STR_RES");
// Raw storage for a CExoString produced into an unconstructed return slot (005E5D10 outputs).
struct Slot { alignas(StartupString) unsigned char bytes[sizeof(StartupString)]; StartupString* p() { return reinterpret_cast<StartupString*>(bytes); } };

// Pinned PE literals (identical bytes).
const char credits_layout_0074eddc[] = "credits";      // .rdata 0074EDDC (layout resref and 2DA name)
const char lb_credits_0075426c[] = "LB_CREDITS";        // .rdata 0075426C
const char fnt_credits_00754260[] = "fnt_credits";      // .rdata 00754260
const char name_column_00745eec[] = "Name";             // .rdata 00745EEC
const char newline_0073d5cc[] = "\n";                    // .rdata 0073D5CC
const char empty_0073d71c[] = "";                        // .rdata 0073D71C
const Word k_one_0073d764 = 0x3F800000u;                 // .rdata 0073D764 1.0f
const Word k_two_00741be0 = 0x40000000u;                 // .rdata 00741BE0 2.0f
const Word k_three_00740c84 = 0x40400000u;               // .rdata 00740C84 3.0f
const Word k_third_007415f4 = 0x3EAAAAABu;               // .rdata 007415F4 0.33333334f
const Word k_half_0073e9ac = 0x3F000000u;                // .rdata 0073E9AC 0.5f
const Word k_inv480_0075425c = 0x3B088889u;              // .rdata 0075425C 1/480 (0.0020833334f)
const unsigned char k_tlk_flag_0073ecd0 = 0x00;          // .rdata byte 0073ECD0 (image value 00; as gui-load gui_params)

// __ftol2 (006FAE8C) on a wide value: the original loads the x87 value and calls the CRT port; EAX = low word.
__declspec(noinline) std::int32_t ftol2(double value) {
    std::int32_t result;
    __asm {
        fld qword ptr [value]
        call genuine_x87_ftol_006fae8c
        mov result, eax
    }
    return result;
}

// Constructed-member stages of 0068F8D0 (EH states 0..4 of FuncInfo 00730018; 5..7 are temporaries).
enum Stage { kPanel = 0, kListBox, kTextItem, kClickArea, kMusic };
void unwind_credits(Credits* self, int built) {
    if (built >= kMusic) music(self)->~StartupString();
    if (built >= kClickArea) gui_load_label_destruct_00419d10(&self->click_area, nullptr);
    if (built >= kTextItem) gui_load_label_destruct_00419d10(&self->text_item, nullptr);
    if (built >= kListBox) gui_load_listbox_destruct_0041b060(&self->lb_credits, nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
}

// ================================================================ CClientExoApp / CClientExoAppInternal leaves
extern "C" {
// 005EE1C0 CClientExoApp::StartCreditSequence: mov ecx,[ecx+4]; jmp 005F4C60 (RET 8 in the target).
void __fastcall kotor_fastcall_005ee1c0(void* client_app, void*, void* callback12, StartupString* name) noexcept(false) {
    kotor_fastcall_005f4c60(word_ptr(client_app, 4), nullptr, callback12, name);     // 005EE1C0..C3
}
// 005F4C60 CClientExoAppInternal::StartCreditSequence(Vector const* color, CExoString const* name), RET 8.
void __fastcall kotor_fastcall_005f4c60(void* internal, void*, void* callback12, StartupString* name) noexcept(false) {
    if (word_at(internal, 0x4B8) != 0) return;                                       // 005F4C79..81 -> 005F4D3B
    void* const memory = live_operator_new(0x5F8);                                   // 005F4C88..8D
    Credits* panel = nullptr;                                                        // 005F4CBC esi = 0
    if (memory != nullptr) {                                                         // 005F4C99..A3 (EH state 0)
        try {
            panel = kotor_fastcall_0068f8d0(static_cast<Credits*>(memory), nullptr, gui_manager(), name);  // 005F4CA5..B3
        } catch (...) { live_free(memory); throw; }
    }
    if (panel == nullptr) return;                                                    // 005F4CBE..C8 -> 005F4D3A
    if (frame_ContainsPanel_0040bd70(gui_manager(), nullptr, panel) == 0) {          // 005F4CCA..D8 PanelExists
        const std::uint32_t mode = word_at(internal, 0x4BC) != 0 ? 1u : 3u;          // 005F4CDA..F0 ([+4BC] ? 1 : 3)
        kotor_fastcall_0040bc70(gui_manager(), nullptr, panel, mode, 1);             // 005F4CE2..F3 AddPanel(panel, mode, 1)
    }
    std::memcpy(&panel->color, callback12, 12);                                      // 005F4CF8..0C panel+50 = *arg1 (12 bytes)
    if (void* const module = word_ptr(internal, 0x18)) {                             // 005F4D0F..14
        if (void* const area = word_ptr(module, 0x48)) {                             // 005F4D16..1B
            if (void* const ambient = word_ptr(area, 0x1DC)) {                        // 005F4D1D..25 CSWCAmbientSound
                recovered_client_ambient_sound_stop_sounds_0068e1b0(ambient, nullptr, 0);  // 005F4D27..2B StopSounds(0)
            }
        }
    }
    put_word(internal, 0x4B8, 1);                                                    // 005F4D30
}
// 005EE1D0 CClientExoApp::StopCreditSequence: mov ecx,[ecx+4]; jmp 005F4D50.
void __fastcall kotor_fastcall_005ee1d0(void* client_app, void*) {
    kotor_fastcall_005f4d50(word_ptr(client_app, 4), nullptr);                       // 005EE1D0..D3
}
// 005F4D50 CClientExoAppInternal::StopCreditSequence: [this+4B8h] = 0.
void __fastcall kotor_fastcall_005f4d50(void* internal, void*) { put_word(internal, 0x4B8, 0); }   // 005F4D50
// 005EE1B0 CClientExoApp::FadeAndStopStreamingSoundSource(source, ms): mov ecx,[ecx+4]; jmp 005F1B20 (RET 8).
void __fastcall kotor_fastcall_005ee1b0(void* client_app, void*, void* source, std::uint32_t duration) {
    table29_required::gui_fade_005f1b20(static_cast<menu_normal_40_49::MusicInternal*>(word_ptr(client_app, 4)), nullptr,
                                        source, duration);                           // 005EE1B0..B3
}
// 005F4E50 CClientExoAppInternal::SetEvilEndingHackInProgress(int), RET 4.
void __fastcall kotor_fastcall_005f4e50(void* internal, void*, std::uint32_t value) {
    put_word(internal, 0x4BC, value);                                                // 005F4E59
    if (value != 0) return;                                                          // 005F4E5F
    if (word_ptr(internal, 0x3C4) == nullptr) return;                                // 005F4E61..69
    kotor_fastcall_005d5b80(word_ptr(internal, 0x3C4), nullptr);                     // 005F4E6C Stop
    if (void* const source = word_ptr(internal, 0x3C4)) {                            // 005F4E71..79
        table29_required::gui_sound_destroy_005d6200(source, nullptr);               // 005F4E7D
        live_free(source);                                                           // 005F4E82..88
    }
    put_word(internal, 0x3C4, 0);                                                    // 005F4E8B
}
// 0062FB90 CGuiInGame::FadeAndStopDialogAmbientTrack.
void __fastcall kotor_fastcall_0062fb90(void* in_game, void*) {
    void* const track = word_ptr(in_game, 0xBBC);                                    // 0062FB93..9B
    if (track == nullptr) return;
    kotor_fastcall_005ee1b0(client_app(), nullptr, track, 0x1F4);                    // 0062FB9E..AC (500 ms)
    if (void* const source = word_ptr(in_game, 0xBBC)) {                             // 0062FBB1..B9
        table29_required::gui_sound_destroy_005d6200(source, nullptr);               // 0062FBBD
        live_free(source);                                                           // 0062FBC2..C8
    }
    put_word(in_game, 0xBBC, 0);                                                     // 0062FBCB
}

// ================================================================ CSWGuiInGameCredits
// 0068F8D0 CSWGuiInGameCredits::CSWGuiInGameCredits(CSWGuiManager*, CExoString const&), RET 8.
Credits* __fastcall kotor_fastcall_0068f8d0(Credits* self, void*, gui_load::Object* manager, StartupString* name) noexcept(false) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                       // 0068F8F6..FB
    int built = kPanel;
    try {
        self->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&kotor_m1_credits_methods_007541f0);  // 0068F909
        gui_load_listbox_construct_0041be40(&self->lb_credits, nullptr);             // 0068F902..0F (+64)
        built = kListBox;
        gui_load_label_construct_0041acd0(&self->text_item, nullptr);                // 0068F914..1F (+344)
        built = kTextItem;
        gui_load_label_construct_0041acd0(&self->click_area, nullptr);               // 0068F924..31 (+484)
        built = kClickArea;
        ::new (static_cast<void*>(self->music)) StartupString();                     // 0068F936..41 (005B3190)
        built = kMusic;
        self->flags |= 1u;                                                           // 0068F946..5A [+44] |= 1
        {
            gui_load::ResRef layout;                                                 // [esp+28h], no destructor
            layout.construct_00406d80(credits_layout_0074eddc);                      // 0068F94C..5D "credits"
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);            // 0068F962..69
        }
        {
            StartupString tag(lb_credits_0075426c);                                  // 0068F96E..77 (state 5)
            gui_load_panel_init_control_0040b930(self, nullptr, &self->lb_credits, &tag, 1);   // 0068F97C..8E
        }                                                                            // 0068F993..9C
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                          // 0068F9A1..A3
        if (word_at(client_internal(), 0x4BC) != 0) {                                // 0068F9A8..B9 (evil-ending hack)
            kotor_cell_credits_gui_width_00833bc0 = static_cast<std::uint32_t>(manager_width());     // 0068F9C1..C9
            kotor_cell_credits_gui_height_00833bbc = static_cast<std::uint32_t>(manager_height());   // 0068F9C5..D5
            kotor_cell_credits_saved_width_00833bc4 = kotor_renderer_gl_cells::width_0078d1d4;      // 0068F9CF..E9
            kotor_cell_credits_saved_height_00833bb8 = kotor_menu_window_cells::height_0078d1d8;    // 0068F9DA..EF
            kotor_renderer_gl_cells::width_0078d1d4 = 0x280;                         // 0068F9F4
            kotor_menu_window_cells::height_0078d1d8 = 0x1E0;                        // 0068F9FE
            gui_size_view(gui_manager())->set_size_0040be70(0x280, 0x1E0);           // 0068F9DF..A08
        }
        gui_load::Extent extent{};                                                   // [esp+38h..44h]
        extent.left = 0; extent.top = 0;                                             // 0068FA12..1E
        extent.width = manager_width();                                              // 0068FA22..26
        extent.height = manager_height();                                            // 0068FA2A..35
        gui_load_panel_set_extent_0040a5a0(self, nullptr, &extent);                  // 0068FA2E..39 (direct)
        extent.width = self->extent.width - 0x50;                                    // 0068FA3E..4F [esi+0Ch]-50h
        extent.height = self->extent.height - 0x50;                                  // 0068FA41..53 [esi+10h]-50h
        extent.left = 0x28; extent.top = 0x28;                                       // 0068FA4A..62
        {
            gui_load::Object* const lb = &self->lb_credits;
            lb->vtbl->set_extent(lb, nullptr, &extent);                              // 0068FA57..69 vslot +04
        }
        // 0068FA70..7C fild [esi+10h]; fmul dword [0075425C] -> fstp dword (third Initialize word).
        const float scale = static_cast<float>(static_cast<double>(self->extent.height) *
                                               static_cast<double>(bits_float(k_inv480_0075425c)));
        extent.left += 4; extent.top += 4;                                           // 0068FA82..95
        extent.width -= 8; extent.height -= 8;                                       // 0068FA8A..A1
        gui_load::Control* const proto = self->lb_credits.proto;                     // 0068FAA3 [esi+330h]
        {
            gui_load::Object* const item = &self->text_item;
            gui_lb::vt<LabelInitView>(item)->initialize98(item, nullptr, word_of(&extent), word_of(proto),
                                                          float_bits(scale));       // 0068FAA9..C3 vslot +98
        }
        extent.left = 0; extent.top = 0;                                             // 0068FACE..D2
        extent.width = manager_width();                                              // 0068FAD6..DA
        extent.height = manager_height();                                            // 0068FADE..ED
        {
            gui_load::Object* const area = &self->click_area;
            gui_lb::vt<LabelInitView>(area)->initialize98(area, nullptr, word_of(&extent), word_of(proto),
                                                          k_one_0073d764);          // 0068FAE2..F9 vslot +98 (1.0f)
        }
        gui_m48_panel_add_control_0040b9a0(self, nullptr, &self->click_area);        // 0068FAFF..B02
        gui_load_control_add_event_0041ab20(&self->click_area, nullptr, 0x27, reinterpret_cast<kih::GuiObject*>(self),
            kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::AcceptButtonCallback_00624ba0)); // 0068FB07..11
        {
            gui_load::Ref font;                                                      // [esp+28h], no destructor
            font.construct_00406d80(fnt_credits_00754260);                           // 0068FB16..1F "fnt_credits"
            gui_load_text_params_set_base_font_00415dd0(text_params(self), nullptr, &font);   // 0068FB24..2F
        }
        self->count = 0;                                                             // 0068FB36
        self->strrefs = nullptr;                                                     // 0068FB3C
        void* table = nullptr;                                                       // [esp+14h]
        {
            void* const memory = live_operator_new(0x54);                            // 0068FB34..42
            if (memory != nullptr) {                                                 // 0068FB50..57 (state 6: free on throw)
                try {
                    menu_resources::Ref ref;                                         // by-value argument area 0068FB5A..68
                    ref.construct_00406d80(credits_layout_0074eddc);                 // "credits"
                    table = c2da_current_construct_00413cc0(memory, nullptr, ref, 0); // 0068FB59..6F C2DA(ref, 0)
                } catch (...) { live_free(memory); throw; }
            }
        }
        // 0068FB82..90: Load2DArray on the table pointer (also when operator new failed, as the original).
        if (c2da_current_load_004143b0(table, nullptr) == 0) return self;            // 0068FB89..90 -> 0068FC9A
        const unsigned char rows = static_cast<unsigned char>(word_at(table, 0x24));  // 0068FB96..99 (AL = rows)
        self->count = rows;                                                          // 0068FBA0
        self->strrefs = static_cast<std::int32_t*>(live_operator_new(static_cast<std::uint32_t>(rows) * 4u)); // 0068FB99..AB
        for (unsigned char i = 0; i < self->count; ++i) {                            // 0068FBBA..C0 / 0068FC1E..30 (byte)
            std::int32_t value;                                                      // [esp+20h]
            std::int32_t found;
            {
                StartupString column(name_column_00745eec);                          // 0068FBC2..CB "Name" (state 7)
                found = c2da_current_get_int_00413660(table, nullptr, i, &column, &value);   // 0068FBD0..E9
            }                                                                        // 0068FBEE..FB
            self->strrefs[i] = found != 0 ? value : 0;                               // 0068FC00..1B
        }
        self->scroll_interval = 0.0f;                                                // 0068FC3C
        music(self)->Assign005e5c50(name);                                           // 0068FC32..42
        self->source = nullptr;                                                      // 0068FC4E
        self->scroll_time = 0.0f;                                                    // 0068FC54
        self->fade_time = 0.0f;                                                      // 0068FC5A
        self->saved_input_class = 2;                                                 // 0068FC60
        self->index = 0;                                                             // 0068FC6A
        self->fading = 1;                                                            // 0068FC70
        self->fade_in = 1;                                                           // 0068FC76
        self->scroll_interval = 0.0f;                                                // 0068FC7C
        kotor_fastcall_0068f6f0(self, nullptr);                                      // 0068FC82 SetNextText
        gui_load_text_params_set_alignment_00415f20(text_params(self), nullptr, 0x12);   // 0068FC87..8F
        set_text_alpha(self, 0);                                                     // 0068FC94 [+454] = 0
    } catch (...) {
        unwind_credits(self, built);
        throw;
    }
    return self;                                                                     // 0068FC9F eax = this; RET 8
}

// 0068F0E0 CSWGuiInGameCredits::~CSWGuiInGameCredits().
void __fastcall kotor_fastcall_0068f0e0(Credits* self, void*) {
    self->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&kotor_m1_credits_methods_007541f0);  // 0068F0FE
    if (void* const source = self->source) {                                         // 0068F104..14
        table29_required::gui_sound_destroy_005d6200(source, nullptr);               // 0068F118
        live_free(source);                                                           // 0068F11D..23
    }
    if (self->strrefs != nullptr) live_free(self->strrefs);                          // 0068F126..36
    void* const in_game = kotor_input_required::get_in_game_gui_005ed690(client_app(), nullptr);   // 0068F139..46
    if (in_game != nullptr) kotor_fastcall_0062fb90(in_game, nullptr);               // 0068F148..4E
    kotor_fastcall_005ee1d0(client_app(), nullptr);                                  // 0068F153..5C StopCreditSequence
    gui_m48_client_set_input_class_005eda60(client_app(), nullptr, self->saved_input_class, 1);   // 0068F161..73
    if (in_game != nullptr)
        kotor_game_prepare::SetGlobalUnpauseableState0062b410(in_game, nullptr, 0);  // 0068F178..80
    if (word_at(client_internal(), 0x4BC) != 0) {                                    // 0068F185..99
        kotor_renderer_gl_cells::width_0078d1d4 = kotor_cell_credits_saved_width_00833bc4;   // 0068F19B / 1AC
        kotor_menu_window_cells::height_0078d1d8 = kotor_cell_credits_saved_height_00833bb8; // 0068F1A7 / 1C0
        gui_size_view(gui_manager())->set_size_0040be70(
            static_cast<std::int32_t>(kotor_cell_credits_gui_width_00833bc0),
            static_cast<std::int32_t>(kotor_cell_credits_gui_height_00833bbc));     // 0068F1A1..C5 SetSize(BC0, BBC)
        kotor_fastcall_005f4e50(client_internal(), nullptr, 0);                      // 0068F1CA..D9
        kotor_cdecl_00403de0();                                                      // 0068F1DE RestoreResolutionFromMovie
    }
    kotor_fastcall_005ee180(client_app(), nullptr, 0x80);                            // 0068F1E3..F1 ShowMouse(80h)
    music(self)->~StartupString();                                                   // 0068F1F6..201
    gui_load_label_destruct_00419d10(&self->click_area, nullptr);                    // 0068F206..211
    gui_load_label_destruct_00419d10(&self->text_item, nullptr);                     // 0068F216..221
    gui_load_listbox_destruct_0041b060(&self->lb_credits, nullptr);                  // 0068F226..22E
    gui_load_panel_destruct_0040cf70(self, nullptr);                                 // 0068F233..23D
}
// 0068F6D0 scalar deleting destructor (flags), RET 4.
void* __fastcall kotor_fastcall_0068f6d0(Credits* self, void*, Word flags) {
    kotor_fastcall_0068f0e0(self, nullptr);                                          // 0068F6D3
    if (flags & 1u) live_free(self);                                                 // 0068F6D8..E5
    return self;                                                                     // 0068F6E8
}

// 0068F260 CSWGuiInGameCredits::OnPanelAdded() (method table +48).
void __fastcall kotor_fastcall_0068f260(gui_load::Panel* panel, void*) noexcept(false) {
    Credits* const self = static_cast<Credits*>(panel);
    kotor_fastcall_005ee170(client_app(), nullptr, 0x80);                            // 0068F278..88 HideMouse(80h)
    kotor_game_prepare::SetGlobalUnpauseableState0062b410(
        kotor_input_required::get_in_game_gui_005ed690(client_app(), nullptr), nullptr, 1);  // 0068F28D..9F (no null test)
    self->saved_input_class = gui_m48_client_get_input_class_005eda70(client_app(), nullptr);  // 0068F2A4..B4
    gui_load_panel_on_added_0040b870(self, nullptr);                                 // 0068F2B2..BA CSWGuiPanel::OnPanelAdded
    gui_m48_client_set_input_class_005eda60(client_app(), nullptr, 2, 1);            // 0068F2BF..CB
    void* const memory = live_operator_new(4);                                       // 0068F2D0..D2
    void* source = nullptr;                                                          // 0068F2F3
    if (memory != nullptr) {                                                         // 0068F2DE..E8 (EH state 0)
        try { source = table29_required::gui_sound_construct_005d5a50(memory, nullptr); }   // 0068F2EA..EC
        catch (...) { live_free(memory); throw; }
    }
    self->source = source;                                                           // 0068F2FF
    if (source == nullptr) return;                                                   // 0068F305
    table29_required::gui_priority_005d5ba0(source, nullptr, 4);                     // 0068F307..0B
    menu_resources::Ref track;                                                       // [esp+8], no destructor
    track.construct_00406d80(WindowCStr005e5670(music(self), nullptr));             // 0068F310..20
    (void)table29_required::gui_play_005d5b10(self->source, nullptr, &track, 8, 0, 1, 0);   // 0068F325..38
}

// 0068F350 CSWGuiInGameCredits::HandleInputEvent(event, value), RET 8: events 27h/28h/2Dh/2Eh with value != 0
// mark the panel for removal (jump table 0068F388/index bytes 0068F390 = 00 00 01 01 01 01 00 00); no base call.
void __fastcall kotor_fastcall_0068f350(gui_load::Object* object, void*, I32 event, I32 value) {
    if (value == 0) return;                                                          // 0068F350..56
    const std::uint32_t index = static_cast<std::uint32_t>(event - 0x27);            // 0068F358..5C
    if (index > 7u) return;                                                          // 0068F35F..62 (unsigned ja)
    static const unsigned char cases_0068f390[8] = { 0, 0, 1, 1, 1, 1, 0, 0 };       // .text 0068F390
    if (cases_0068f390[index] != 0) return;                                          // -> 0068F384
    gui_load::Panel* const self = static_cast<gui_load::Panel*>(object);
    self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                              // 0068F372..81
}

// 0068F3A0 CSWGuiInGameCredits::SetText(strref, resize), RET 8.
void __fastcall kotor_fastcall_0068f3a0(Credits* self, void*, Word strref, Word resize) noexcept(false) {
    gui_load::Extent extent{};                                                       // [esp+34h..40h] zeroed 0068F3CB..D7
    if (self->fading != 0) {                                                         // 0068F3C4..DB
        gui_load_text_params_set_strref_00415e50(text_params(self), nullptr, strref); // 0068F3DD..E7
    } else {
        StrRes res;                                                                  // [esp+44h] text, [esp+4Ch] sound
        ::new (static_cast<void*>(res.text)) StartupString();                        // 0068F3F1..F5 (state 0)
        StartupString* const text = reinterpret_cast<StartupString*>(res.text);
        try {
            c2da_default_ref_00405ed0(&res.sound, nullptr);                          // 0068F3FA..402 (state 1)
            StartupString combined;                                                  // [esp+2Ch] 0068F407..13 (state 2)
            StartupString padding;                                                   // [esp+24h] 0068F418..21 (state 3)
            tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, strref, &res, k_tlk_flag_0073ecd0);   // 0068F426..43
            // 0068F448..477: lines = __ftol2(h_item / (font * h_panel * (1/480))) + 1, x87 wide (D30-FINAL).
            const float item_height = static_cast<float>(self->text_item.extent.height);   // 0068F448..54 fstp dword
            const I32 font = gui_load_text_font_height_00414ee0(&self->text_item.text, nullptr);   // 0068F44E..58
            const double divisor = static_cast<double>(font) * static_cast<double>(self->extent.height) *
                                   static_cast<double>(bits_float(k_inv480_0075425c));   // 0068F45D..68
            std::int32_t lines = ftol2(static_cast<double>(item_height) / divisor) + 1;   // 0068F46E..82
            GuiAssign005e5140(&padding, nullptr, empty_0073d71c);                    // 0068F479..83
            if (lines > 0) {                                                         // 0068F488..8A
                do {
                    StartupString newline(newline_0073d5cc);                         // 0068F490..99 [esp+18h] (state 4)
                    Slot sum;                                                        // [esp+10h]
                    StartupString* const joined = GuiConcat005e5d10(&padding, nullptr, sum.p(), &newline);   // 0068F49E..B0
                    try { padding.Assign005e5c50(joined); }                          // 0068F4B5..BF (state 5)
                    catch (...) { joined->~StartupString(); throw; }
                    joined->~StartupString();                                        // 0068F4C4..CC
                } while (--lines != 0);                                              // 0068F4D1..E0
            }
            Slot first;                                                              // [esp+10h]
            StartupString* const lead = GuiConcat005e5d10(&padding, nullptr, first.p(), text);   // 0068F4E2..F0
            try {
                Slot second;                                                         // [esp+18h]
                StartupString* const full = GuiConcat005e5d10(lead, nullptr, second.p(), &padding);  // 0068F4F5..507
                try { combined.Assign005e5c50(full); }                               // 0068F50C..516
                catch (...) { full->~StartupString(); throw; }
                full->~StartupString();                                              // 0068F51B..523
            } catch (...) { lead->~StartupString(); throw; }
            lead->~StartupString();                                                  // 0068F528..531
            gui_load_text_params_set_text_00415e00(text_params(self), nullptr, &combined);   // 0068F536..541
        } catch (...) { text->~StartupString(); throw; }                             // padding/combined by C++ order
        text->~StartupString();                                                      // 0068F546..56E (padding, combined, text)
    }
    gui_load_listbox_clear_items_00419a90(&self->lb_credits, nullptr);               // 0068F573..578
    if (resize != 0) {                                                               // 0068F57D..82
        extent = self->text_item.extent;                                             // 0068F584..AD (+348)
        gui_load::Text* const text_object = &self->text_item.text;                   // +414
        extent.height = static_cast<I32>(gui_lb::vt<TextIdealView>(text_object)->ideal_height(text_object, nullptr)); // 0068F5A1..BA vslot +08
        gui_load::Object* const item = &self->text_item;
        item->vtbl->set_extent(item, nullptr, &extent);                              // 0068F5B4..C9 vslot +04
    }
    gui_load::Control* items[1] = { &self->text_item };                              // 0068F5D9..E1 (argument slot reused)
    gui_m48_listbox_add_controls_0041c2c0(&self->lb_credits, nullptr, items, 1, 0, 0, 0);   // 0068F5CC..E5
    gui_load_listbox_set_active_control_0041c160(&self->lb_credits, nullptr, &self->text_item, 0);  // 0068F5EA..EF
    self->lb_credits.scale = static_cast<float>(self->lb_credits.extent.height);     // 0068F5F4..617 (+31C = lb+2B8)
}

// 0068F630 CSWGuiInGameCredits::UpdateScrolling(float), RET 4.
void __fastcall kotor_fastcall_0068f630(Credits* self, void*, float delta) {
    // +1C4/+1C8 = list box +160/+164 (scroll bar +5C/+60: position limit / position).
    if (word_at(self, 0x1C8) == word_at(self, 0x1C4)) {                              // 0068F633..3F
        if (table29_required::gui_playing_005d5af0(self->source, nullptr) != 0) return;   // 0068F641..4E
        self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                          // 0068F650..5F
        return;
    }
    const double sum = static_cast<double>(delta) + static_cast<double>(self->scroll_time);   // 0068F666..6A
    self->scroll_time = static_cast<float>(sum);                                     // 0068F670 fst dword
    if (!(sum >= static_cast<double>(self->scroll_interval))) return;                // 0068F676..81 (C0: less/unordered)
    gui_load::Object* const lb = &self->lb_credits;                                  // 0068F684 edi = +64
    for (;;) {
        lb->vtbl->handle_input_event(lb, nullptr, 0x32, 1);                          // 0068F687..8F vslot +3C(32h, 1)
        self->scroll_time = static_cast<float>(static_cast<double>(self->scroll_time) -
                                               static_cast<double>(self->scroll_interval));   // 0068F692..AA
        if (word_at(self, 0x1C8) == word_at(self, 0x1C4)) return;                    // 0068F69E..B0
        if (!(static_cast<double>(self->scroll_time) >= static_cast<double>(self->scroll_interval))) return;  // 0068F6B2..C3
    }
}

// 0068F6F0 CSWGuiInGameCredits::SetNextText().
void __fastcall kotor_fastcall_0068f6f0(Credits* self, void*) noexcept(false) {
    const std::int32_t last = static_cast<std::int32_t>(self->count) - 1;           // 0068F6F6..706
    Word resize = 0;                                                                 // 0068F704
    if (!(static_cast<std::int32_t>(self->index) < last)) {                          // 0068F707..09 (signed jl)
        self->fading = 0;                                                            // 0068F713
        gui_load_text_params_set_alignment_00415f20(text_params(self), nullptr, 0x0A);   // 0068F70B..19
        set_text_alpha(self, k_one_0073d764);                                        // 0068F71E [+454] = 1.0f
        resize = 1;                                                                  // 0068F728
    }
    kotor_fastcall_0068f3a0(self, nullptr, static_cast<Word>(self->strrefs[self->index]), resize);   // 0068F72D..741
    if (self->fading == 0) {                                                         // 0068F746..4E
        // 0068F76D..77B fild [+354] (text item height); fdivr dword [007A2670]; fstp dword [+5F0].
        self->scroll_interval = static_cast<float>(static_cast<double>(bits_float(kotor_cell_credits_scroll_span_007a2670)) /
                                                   static_cast<double>(self->text_item.extent.height));
    }
    ++self->index;                                                                   // 0068F781 inc byte
}

// 0068F790 CSWGuiInGameCredits::UpdateFading(float), RET 4.
void __fastcall kotor_fastcall_0068f790(Credits* self, void*, float delta) noexcept(false) {
    const double t = static_cast<double>(delta) + static_cast<double>(self->fade_time);   // 0068F790..97
    self->fade_time = static_cast<float>(t);                                         // 0068F7A3 fst dword
    const Word fade_in = self->fade_in;                                              // 0068F7AE
    if (!(t >= static_cast<double>(bits_float(k_one_0073d764)))) {                   // 0068F79D..B4 (C0 -> 0068F859)
        if (fade_in != 0) {                                                          // 0068F859..5B
            set_text_alpha(self, float_bits(static_cast<float>(t)));                 // 0068F85D fstp dword [+454]
            return;
        }
        set_text_alpha(self, float_bits(static_cast<float>(static_cast<double>(bits_float(k_one_0073d764)) - t)));  // 0068F867..6F
        return;
    }
    if (fade_in != 0) {                                                              // 0068F7BA..BC
        set_text_alpha(self, k_one_0073d764);                                        // 0068F7C4 [+454] = 1.0f
        if (!(t >= static_cast<double>(bits_float(k_three_00740c84)))) return;       // 0068F7BE..D3
        const std::int32_t n = ftol2(static_cast<double>(bits_float(k_third_007415f4)) * t);   // 0068F7D9..E1
        self->fade_in = 0;                                                           // 0068F7EE
        self->fade_time = static_cast<float>(t - static_cast<double>(n) * static_cast<double>(bits_float(k_three_00740c84)));  // 0068F7EA..800
        return;
    }
    set_text_alpha(self, 0);                                                         // 0068F812 [+454] = 0
    if (!(t >= static_cast<double>(bits_float(k_two_00741be0)))) return;             // 0068F80C..21
    const std::int32_t n = ftol2(static_cast<double>(bits_float(k_half_0073e9ac)) * t);   // 0068F823..2B
    self->fade_in = 1;                                                               // 0068F83A
    self->fade_time = static_cast<float>(t - (static_cast<double>(n) + static_cast<double>(n)));   // 0068F834..848
    kotor_fastcall_0068f6f0(self, nullptr);                                          // 0068F850 SetNextText
}

// 0068F880 CSWGuiInGameCredits::Draw(float), RET 4 (method table +34).
void __fastcall kotor_fastcall_0068f880(gui_load::Panel* panel, void*, Word delta) {
    Credits* const self = static_cast<Credits*>(panel);
    void* const server = word_ptr(app_manager(), 8);                                 // 0068F880..88 [[007A39FC]+8]
    if (server == nullptr || frame_GetPauseState_004ae980(server, nullptr, 2) == 0) {   // 0068F88B..9D
        if (self->fading != 0) kotor_fastcall_0068f790(self, nullptr, bits_float(delta));   // 0068F89F..AC
        else kotor_fastcall_0068f630(self, nullptr, bits_float(delta));              // 0068F8BE
    }
    kotor_panel_draw_0040B760(self, nullptr, delta);                                 // 0068F8B1..B4 / 0068F8C3..C6
}

// ================================================================ 007541F0 CSWGuiInGameCredits method table (27 slots)
// .rdata 007541F0..0075425B (dumped from the PE): +00 0068F6D0, +34 0068F880, +3C 0068F350, +48 0068F260; all
// other slots equal CSWGuiPanel's 0073E010 (same owners as the M56 title-movies / options tables).
struct KotorM1CreditsMethods {
    void* (__fastcall* s00)(Credits*, void*, Word);                                  // +00 0068F6D0 deleting dtor
    void (__fastcall* s04)(gui_load::Object*, void*, const gui_load::Extent*);       // +04 0040A5A0 SetExtent
    void (__fastcall* s08)(gui_load::Object*, void*, gui_load::Object*, Word);       // +08 0040A630 SetActiveControl
    void (__fastcall* s0c)(gui_load::Object*, void*, Word);                          // +0C 0060E760 void_return [RET 4]
    Word (__fastcall* s10)(gui_load::Object*, void*, Word, Word);                    // +10 00409EB0 [RET 8]
    void (__fastcall* s14)(gui_load::Object*, void*);                                // +14 004015A0
    void (__fastcall* s18)(gui_load::Object*, void*);                                // +18 004015A0
    void (__fastcall* s1c)(gui_load::Object*, void*);                                // +1C 004015A0
    void (__fastcall* s20)(gui_load::Object*, void*);                                // +20 004015A0
    void (__fastcall* s24)(gui_load::Object*, void*);                                // +24 004015A0
    gui_load::Object* (__fastcall* s28)(gui_load::Object*, void*);                   // +28 00641DB0 self_return
    Word (__fastcall* s2c)(gui_load::Object*, void*);                                // +2C 0063E7F0 return_zero
    Word (__fastcall* s30)(gui_load::Object*, void*);                                // +30 0063E7F0 return_zero
    void (__fastcall* s34)(gui_load::Panel*, void*, Word);                           // +34 0068F880 Draw
    void (__fastcall* s38)(gui_load::Object*, void*, Word);                          // +38 0060E760 void_return [RET 4]
    void (__fastcall* s3c)(gui_load::Object*, void*, I32, I32);                      // +3C 0068F350 HandleInputEvent
    gui_load::Object* (__fastcall* s40)(gui_load::Object*, void*, I32, I32);         // +40 0040B690 HitCheckMouse
    gui_load::Object* (__fastcall* s44)(gui_load::Object*, void*);                   // +44 0040B630 GetActiveControl
    void (__fastcall* s48)(gui_load::Panel*, void*);                                 // +48 0068F260 OnPanelAdded
    void (__fastcall* s4c)(gui_load::Panel*, void*);                                 // +4C 0040C170 OnPanelRemoved
    void (__fastcall* s50)(gui_load::Object*, void*, Word);                          // +50 0040B640 OnAButtonPressed
    void (__fastcall* s54)(gui_load::Object*, void*, Word);                          // +54 0040B650 OnBButtonPressed
    void (__fastcall* s58)(gui_load::Object*, void*, Word);                          // +58 0040B660 OnXButtonPressed
    void (__fastcall* s5c)(gui_load::Object*, void*, Word);                          // +5C 0040B670 OnYButtonPressed
    void (__fastcall* s60)(gui_load::Object*, void*, Word);                          // +60 0040B680 OnBlackButtonPressed
    gui_load::String* (__fastcall* s64)(gui_load::Panel*, void*, gui_load::String*); // +64 0040A900 GetFullScreenBG
    void (__fastcall* s68)(gui_load::Panel*, void*);                                 // +68 0040A9C0 ResetFont
};
static_assert(sizeof(KotorM1CreditsMethods) == 0x6C, "007541F0 slot count (27)");
extern const KotorM1CreditsMethods kotor_m1_credits_methods_007541f0 = {
    &kotor_fastcall_0068f6d0,                       // +00
    &gui_load_panel_set_extent_0040a5a0,            // +04
    &gui_load_panel_set_active_control_0040a630,    // +08
    &gui_load_slot_ret4_0060e760,                   // +0C
    &gui_load_slot_zero8_00409eb0,                  // +10
    &gui_load_slot_ret_004015a0,                    // +14
    &gui_load_slot_ret_004015a0,                    // +18
    &gui_load_slot_ret_004015a0,                    // +1C
    &gui_load_slot_ret_004015a0,                    // +20
    &gui_load_slot_ret_004015a0,                    // +24
    &gui_load_slot_self_00641db0,                   // +28
    &gui_load_slot_zero_0063e7f0,                   // +2C
    &gui_load_slot_zero_0063e7f0,                   // +30
    &kotor_fastcall_0068f880,                       // +34
    &gui_load_slot_ret4_0060e760,                   // +38
    &kotor_fastcall_0068f350,                       // +3C
    &gui_load_panel_hit_check_0040b690,             // +40
    &gui_load_panel_active_control_0040b630,        // +44
    &kotor_fastcall_0068f260,                       // +48
    &gui_load_panel_on_removed_0040c170,            // +4C
    &gui_load_panel_button_a_0040b640,              // +50
    &gui_load_panel_button_b_0040b650,              // +54
    &gui_load_panel_button_x_0040b660,              // +58
    &gui_load_panel_button_y_0040b670,              // +5C
    &gui_load_panel_button_black_0040b680,          // +60
    &gui_load_panel_full_screen_bg_0040a900,        // +64
    &gui_load_panel_reset_font_0040a9c0,            // +68
};
} // extern "C"
