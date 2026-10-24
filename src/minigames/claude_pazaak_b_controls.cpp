// Pazaak lane PZ-B tier a1, part 1: the Pazaak GUI controls. Contract: claude_pazaak_decls.hpp (pz-b h1).
//   CSWGuiActivatedButton (0x1D4, table 00753120): 0067CA50 setColor, 0067CB70 Draw (x87 fsin), 0067CC90 SetEnabled
//   CSWGuiPazaakCard (0x31C, table 007531C0): 0067CA80 Draw, 0067CAC0 HandleInputEvent, 0067CD00 Load, 0067CD30 SetCard,
//     0067CF00 DrawDragDrop, 0067D040 HandleLMouseDown, 0067D060 HandleMouseCapturedMovement, 0067D0D0 HandleLMouseUp,
//     0067DDD0 ctor, 0067DE60 GetIsSelectable, 0067DE80 dtor, 0067EFC0 scalar deleting dtor
//   CSWGuiTutorial (0x99C, CSWGuiMessageBox base, table 00753260): 0067D110 PlaceRightOf, 0067D1C0 PlaceAbove,
//     0067D260 HandleInputEvent, 0067DEF0 ctor, 0067DF60 dtor, 0067DFC0 ShowHelp, 0067E0B0 ShowMultipleHelp,
//     0067EFE0 scalar deleting dtor
// Transcribed from the original swkotor.exe (GOG 1.03) disassembly; each statement carries its original PCs.
// ABI: original thiscall = __fastcall(self, unused EDX, stack args), callee cleanup; float args as raw bits.
// x87: PC=53 (static init); every FPU-stack intermediate is carried as double, every `fstp dword` is a (float) store,
// fsin is the x87 instruction (x87_sin_plus_one below: fsin + fadd as one sequence), comparisons use the exact fnstsw/test/jcc predicate (w4::jcc_*).
// Data owned here: the card-text table [007A2600] (28 x const char*, strings byte-exact from .rdata 00753048..007530AC).
#include "0fa9be42e331-claude_gui_char.hpp"
#include "602f99a6ff9a-claude_pazaak_decls.hpp"

namespace kotor_menu_window_cells { extern void* volatile gui_manager_007a39f4; }                 // [007A39F4] CSWGuiManager*
namespace kotor_input_hover {
struct Control { void HandleLMouseDown(); void HandleLMouseUp(); };      // 0041AB90 / 0041ABD0 (0391, body-aligned)
struct Navigable { void HandleInputEvent(int event, int down); };        // 0041A9D0 (0391)
struct Button { void HandleInputEvent(int event, int down); };           // 0041AD40 (0391)
}
namespace kotor_reconstruction { struct ArrayMutations; }
namespace kotor_renderer_gl {                                            // 00437D60 CExoArrayList<>::Allocate(int) (0609)
void __fastcall allocate_00437d60(kotor_reconstruction::ArrayMutations* list, void*, unsigned capacity);
}

extern "C" {
void __fastcall kotor_fastcall_00417ab0(void* button, void*, w4::U32 delta_bits);                 // CSWGuiButton::Draw(float)
void __fastcall gui_load_button_set_enabled_00418db0(void* button, void*, w4::I32 enabled);       // CSWGuiButton::SetEnabled
void __fastcall gui_load_button_load_0041c930(void* button, void*, void* a, void* b) noexcept(false); // CSWGuiButton::Load
void* __fastcall c2da_default_ref_00405ed0(void* self, void*);                                   // CResRef::CResRef()
menu_resources::Ref* __fastcall unresolved_ref_assign_00406290(menu_resources::Ref*, void*, menu_resources::Ref* out,
                                                               const char* text);                // CResRef::operator=(const char*)
void __fastcall kotor_fastcall_0040a200(void* manager, void*, void* control);                     // CSWGuiManager::ReleaseMouse
void __fastcall gui_load_panel_offset_extent_0040aa00(void* panel, void*, void* extent);          // CSWGuiPanel::GetExtentAccountingForPanelOffset
void __fastcall gui_m48_message_box_handle_input_006250f0(void* box, void*, w4::I32 event, w4::I32 down); // CSWGuiMessageBox::HandleInputEvent
void* __fastcall kotor_m15e_slot00c_self_00641db0(void* self, void*);                            // 00641DB0 self_return
void __fastcall kotor_fastcall_0067efa0(void*, void*, std::uint32_t);                            // CSWGuiSpeedButton scalar deleting dtor (existing)
}

using namespace gchar;

namespace {
void* gui_manager() { return kotor_menu_window_cells::gui_manager_007a39f4; }                    // [007A39F4]
constexpr U32 kZeroF = 0x00000000u;        // [0073D700] 0.0f
constexpr U32 kOneF = 0x3F800000u;         // [0073D764] 1.0f
constexpr U32 kHalfF = 0x3F000000u;        // [0073E9AC] 0.5f
constexpr U32 kPulseF = 0x41BC7EDDu;       // [00753044] 23.5619449f (7.5 * pi)
constexpr U32 kActivateTimeF = 0x3F4CCCCDu;  // 0.8f (setColor, 0067CA54)

// [007A2600] card-text table (.data, file-initialized pointers into .rdata). Index = card value 0..27.
const char k_p1[] = "+1", k_p2[] = "+2", k_p3[] = "+3", k_p4[] = "+4", k_p5[] = "+5", k_p6[] = "+6";   // 007530AC..00753098
const char k_m1[] = "-1", k_m2[] = "-2", k_m3[] = "-3", k_m4[] = "-4", k_m5[] = "-5", k_m6[] = "-6";   // 00753094..00753080
const char k_f1[] = "\xB1" "1", k_f2[] = "\xB1" "2", k_f3[] = "\xB1" "3",                           // 0075307C..00753074
           k_f4[] = "\xB1" "4", k_f5[] = "\xB1" "5", k_f6[] = "\xB1" "6";                           // 00753070..00753068
const char k_d1[] = "1", k_d2[] = "2", k_d3[] = "3", k_d4[] = "4", k_d5[] = "5",                    // 00741E1C, 007455B8, 00753064..
           k_d6[] = "6", k_d7[] = "7", k_d8[] = "8", k_d9[] = "9", k_d10[] = "10";                   // ..00753048
const char k_empty_0073d71c[] = "";                                                              // 0073D71C
}  // namespace
extern "C" const char* const kotor_pazaak_card_text_007a2600[28] = {
    k_p1, k_p2, k_p3, k_p4, k_p5, k_p6, k_m1, k_m2, k_m3, k_m4, k_m5, k_m6,
    k_f1, k_f2, k_f3, k_f4, k_f5, k_f6, k_d1, k_d2, k_d3, k_d4, k_d5, k_d6, k_d7, k_d8, k_d9, k_d10};

namespace {
const void* color_7a2408() { return kotor_m1_color_007a2408; }     // [007A2408] (0, .33, .49)
const void* color_7a23b4() { return borrowed_exit_color_007a23b4; } // [007A23B4] (0, .66, .98)
// 0067CBED fsin + 0067CBEF fadd [0073D764]: x87 precision control does NOT apply to fsin, so the original adds 1.0
// to the 64-bit-mantissa fsin result and rounds once (PC=53) at the fadd. Rounding fsin to double first (w4::x87_fsin
// followed by a double add) loses those bits near sin = -1 (caught by the replica test). Kept as one x87 sequence; no
// CALL inside the asm.
const float k_one_0073d764 = 1.0f;
__declspec(noinline) double x87_sin_plus_one(double x) {
    double r;
    __asm { fld x }
    __asm { fsin }
    __asm { fadd k_one_0073d764 }
    __asm { fstp r }
    return r;
}
inline void* mgr_of(void* panel) { return lp(panel, 0x18); }        // CSWGuiPanel +18h manager

// Inlined CExoArrayList<unsigned>::Add (0067E00D..0067E03A / 0067E0F0..0067E124): grow to capacity*2 (0x10 when
// empty) when full, then append.
void list_add(void* list, U32 value) {
    if (lw(list, 4) == lw(list, 8)) {
        const U32 cap = lw(list, 8);
        kotor_renderer_gl::allocate_00437d60(static_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
                                             cap != 0 ? cap + cap : 0x10u);
    }
    sw(lp(list, 0), lw(list, 4) * 4u, value);
    sw(list, 4, lw(list, 4) + 1u);
}
}  // namespace

extern "C" {
// ======================= CSWGuiActivatedButton =======================
// 0067CA50 setColor(const Vector&) RET 4
void __fastcall kotor_fastcall_0067ca50(void* self, void*, const void* color) {
    sw(self, 0x1C4, kActivateTimeF);                                                           // 0067CA54
    std::memcpy(at(self, 0x1C8), color, 12);                                                   // 0067CA5E..0067CA71
}

// 0067CB70 Draw(float) RET 4
void __fastcall kotor_fastcall_0067cb70(void* self, void*, U32 delta_bits) {
    const double timer = static_cast<double>(lf(self, 0x1C4));                                 // 0067CB76 fld [1C4]
    if (!jcc_jne(timer, static_cast<double>(bits_f(kZeroF)), 0x01)) {                          // 0067CB7C..0067CB87 fcomp/test ah,1/jne
        const double t = static_cast<double>(lf(self, 0x1C4)) - static_cast<double>(bits_f(delta_bits)); // 0067CB8D..0067CB98
        U32 c[3] = {lw(color_7a2408(), 0), lw(color_7a2408(), 4), lw(color_7a2408(), 8)};       // 0067CB93..0067CBBC [esp+4..C]
        sf(self, 0x1C4, static_cast<float>(t));                                                 // 0067CBB6 fst [1C4]
        if (jcc_jp(t, static_cast<double>(bits_f(kZeroF)), 0x05)) {                             // 0067CBAC fcom; 0067CBC0..0067CBC5 jp
            const double s = x87_sin_plus_one(t * static_cast<double>(bits_f(kPulseF)))         // 0067CBE7..0067CBEF
                             * static_cast<double>(bits_f(kHalfF));                            // 0067CBF5
            const double rest = static_cast<double>(bits_f(kOneF)) - s;                        // 0067CBFB..0067CC01
            const float a = static_cast<float>(static_cast<double>(bits_f(c[0])) * rest);       // 0067CC03..0067CC09 [1C]
            const float b = static_cast<float>(static_cast<double>(bits_f(c[1])) * rest);       // 0067CC0D..0067CC13 [20]
            const double zc = rest * static_cast<double>(bits_f(c[2]));                         // 0067CC17
            const double px = s * static_cast<double>(lf(self, 0x1C8));                         // 0067CC1B..0067CC1D
            const double py = s * static_cast<double>(lf(self, 0x1CC));                         // 0067CC23..0067CC25
            const float tz = static_cast<float>(s * static_cast<double>(lf(self, 0x1D0)));      // 0067CC2B..0067CC33 [30]
            const float rx = static_cast<float>(px + static_cast<double>(a));                   // 0067CC37..0067CC3B
            const float ry = static_cast<float>(py + static_cast<double>(b));                   // 0067CC43..0067CC49
            const float rz = static_cast<float>(static_cast<double>(tz) + zc);                  // 0067CC4D..0067CC57
            c[0] = f_bits(rx); c[1] = f_bits(ry); c[2] = f_bits(rz);                            // 0067CC61..0067CC69
        } else if ((lb(self, 0x44) & 8u) != 0) {                                                // 0067CBC7..0067CBCE
            c[0] = lw(color_7a23b4(), 0); c[1] = lw(color_7a23b4(), 4); c[2] = lw(color_7a23b4(), 8); // 0067CBD4..0067CBE5, CC61
        }
        gui_load_text_params_set_color_00414e10(at(self, 0x16C), nullptr, c);                   // 0067CC6D..0067CC78
    }
    kotor_fastcall_00417ab0(self, nullptr, delta_bits);                                         // 0067CC7D..0067CC84 CSWGuiButton::Draw
}

// 0067CC90 SetEnabled(int) RET 4
void __fastcall kotor_fastcall_0067cc90(void* self, void*, I32 enabled) {
    gui_load_button_set_enabled_00418db0(self, nullptr, enabled);                               // 0067CC90..0067CC98
    if (jcc_jp(static_cast<double>(lf(self, 0x1C4)), static_cast<double>(bits_f(kZeroF)), 0x05)) return; // 0067CC9D..0067CCAE
    const void* color = (lb(self, 0x44) & 8u) != 0 ? color_7a23b4() : color_7a2408();           // 0067CCB0..0067CCBB
    gui_load_text_params_set_color_00414e10(at(self, 0x16C), nullptr, color);                   // 0067CCC0..0067CCC7
    if ((lb(self, 0x44) & 8u) != 0 && lp(gui_manager(), 0x08) == self)                          // 0067CCCC..0067CCDB
        vcall<void, U32>(self, 0x40, 1u);                                                      // 0067CCDD..0067CCE7 SetActive(1)
    else
        vcall<void, U32>(self, 0x40, 0u);                                                      // 0067CCEE..0067CCF5 SetActive(0)
}

// ======================= CSWGuiPazaakCard =======================
// 0067CA80 Draw(float) RET 4
void __fastcall kotor_fastcall_0067ca80(void* self, void*, U32 delta_bits) {
    if ((lb(self, 0x44) & 1u) != 0) {                                                           // 0067CA83..0067CA87
        vcall<void, U32>(at(self, 0x6C), 0x0C, delta_bits);                                     // 0067CA89..0067CA95 border Draw
        vcall<void, U32>(at(self, 0xE0), 0x0C, delta_bits);                                     // 0067CA98..0067CAA5 text Draw
        return;
    }
    kotor_fastcall_00417ab0(self, nullptr, delta_bits);                                         // 0067CAAD..0067CAB4 CSWGuiButton::Draw
}

// 0067CAC0 HandleInputEvent(int, int) RET 8 (tail jumps keep the caller's arguments)
void __fastcall kotor_fastcall_0067cac0(void* self, void*, I32 event, I32 down) {
    if (event == 0x27) {                                                                        // 0067CAC4..0067CAC7
        static_cast<kotor_input_hover::Navigable*>(self)->HandleInputEvent(event, down);        // 0067CACD jmp 0041A9D0
        return;
    }
    static_cast<kotor_input_hover::Button*>(self)->HandleInputEvent(event, down);               // 0067CAD6 jmp 0041AD40
}

// 0067CD00 Load(a, b) RET 8
void __fastcall kotor_fastcall_0067cd00(void* self, void*, void* a, void* b) {
    gui_load_button_load_0041c930(self, nullptr, a, b);                                         // 0067CD00..0067CD0F
    gui_load_border_params_set_pulsing_00414d00(at(self, 0xF4), nullptr, 1u, 0u, 1u);           // 0067CD14..0067CD20
}

// 0067CD30 SetCard(card by value {value, flag}) RET 8
void __fastcall kotor_fastcall_0067cd30(void* self, void*, I32 value, I32 flag) {
    sw(self, 0x304, static_cast<U32>(value));                                                   // 0067CD60
    sw(self, 0x308, static_cast<U32>(flag));                                                    // 0067CD66
    {
        const char* text;
        if (value < 0 || value >= 0x1C) text = k_empty_0073d71c;                                // 0067CD5B..0067CD71 -> 0067CDE1
        else if (value >= 0x0C && value < 0x12)                                                 // 0067CD73..0067CD7B
            text = flag != 0 ? kotor_pazaak_card_text_007a2600[value - 6]                       // 0067CD85 [esi*4+007A25E8]
                             : kotor_pazaak_card_text_007a2600[value - 12];                     // 0067CDA4 [esi*4+007A25D0]
        else text = kotor_pazaak_card_text_007a2600[value];                                     // 0067CDBE [esi*4+007A2600]
        Str s(text);                                                                            // 0067CD8D/CDAC/CDCA/CDEA CExoString
        gui_load_text_params_set_text_00415e00(at(self, 0x2AC), nullptr, s.p());                // 0067CDFF..0067CE05
    }                                                                                           // 0067CE19 ~CExoString
    menu_resources::Ref ref;
    c2da_default_ref_00405ed0(&ref, nullptr);                                                   // 0067CE1E..0067CE22 CResRef()
    // Jump table 0067CECC, index bytes 0067CEE0 (29), index = value + 1 (unsigned, > 0x1C -> default 0067CE87):
    //   byte 0 (value -1) -> 0067CE7B ""; 1..6 (0..5) -> 0067CE3D; 7..12 (6..11) -> 0067CE48; 13..18 (12..17) -> 0067CE54;
    //   19..28 (18..27) -> 0067CE6F.
    static constexpr U8 k_index_0067cee0[29] = {0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
    const U32 index = static_cast<U32>(value) + 1u;                                             // 0067CE27
    const char* name;
    if (index > 0x1Cu) name = "lbl_cardback";                                                   // 0067CE2D ja -> 0067CE87 (007530B0)
    else switch (k_index_0067cee0[index]) {                                                     // 0067CE2F..0067CE36
        case 0: name = k_empty_0073d71c; break;                                                 // 0067CE7B (0073D71C)
        case 1: name = "lbl_cardmpos"; break;                                                   // 0067CE3D (00753100)
        case 2: name = "lbl_cardmneg"; break;                                                   // 0067CE48 (007530F0)
        case 3: name = flag != 0 ? "lbl_cardraref" : "lbl_cardrarem"; break;                    // 0067CE54..0067CE64 (007530E0 / 007530D0)
        default: name = "lbl_cardstand"; break;                                                 // 0067CE6F (007530C0)
    }
    menu_resources::Ref out;                                                                    // by-value return slot
    unresolved_ref_assign_00406290(&ref, nullptr, &out, name);                                  // 0067CE93..0067CE98 CResRef::operator=
    gui_load_border_params_set_fill_00414c00(at(self, 0x80), nullptr, &ref, 0);                 // 0067CE9D..0067CEAA SetFillImage(ref, 0)
}

// 0067CF00 DrawDragDrop(int x, int y) RET 8
void __fastcall kotor_fastcall_0067cf00(void* self, void*, I32 x, I32 y) {
    void* m = gui_manager();                                                                    // 0067CF00
    if (lp(m, 0x10) != self || lw(m, 0x0C) != 1u) {                                             // 0067CF0B..0067CF18
        sw(m, 0x54, 0);                                                                         // 0067D027
        return;
    }
    I32 saved[4]; std::memcpy(saved, at(self, 0x70), 16);                                      // 0067CF1E..0067CF36 [esp+14..20]
    I32 now[4]; std::memcpy(now, at(self, 0x70), 16);                                          // 0067CF3A..0067CF6C [esp+4..10]
    now[0] = static_cast<I32>(static_cast<U32>(x) - static_cast<U32>(now[2] / 2));             // 0067CF55..0067CF62 (cdq/sub/sar)
    now[1] = static_cast<I32>(static_cast<U32>(y) - static_cast<U32>(now[3] / 2));             // 0067CF66..0067CF81
    void* border = at(self, 0x6C);
    vcall<void, const void*>(border, 0x04, now);                                                // 0067CF7A..0067CF88 SetExtent
    vcall<void, U32>(border, 0x0C, kZeroF);                                                     // 0067CF8B..0067CF91 Draw(0)
    I32 lsaved[4]; std::memcpy(lsaved, at(self, 0x1C8), 16);                                   // 0067CF94..0067CFC5 [esp+34..40]
    I32 lnow[4] = {lsaved[0], lsaved[1], lsaved[2], lsaved[3]};
    lnow[0] = static_cast<I32>(static_cast<U32>(lsaved[0]) + (static_cast<U32>(now[0]) - static_cast<U32>(saved[0]))); // 0067CFC9..0067CFDB
    lnow[1] = static_cast<I32>(static_cast<U32>(lsaved[1]) + (static_cast<U32>(now[1]) - static_cast<U32>(saved[1]))); // 0067CFDF..0067CFF1
    void* label = at(self, 0x1C4);
    vcall<void, const void*>(label, 0x04, lnow);                                                // 0067CFED..0067CFFA SetExtent
    vcall<void, U32>(label, 0x38, kZeroF);                                                      // 0067CFFD..0067D003 Draw(0)
    vcall<void, const void*>(label, 0x04, lsaved);                                              // 0067D006..0067D00F
    vcall<void, const void*>(border, 0x04, saved);                                              // 0067D012..0067D01B
}

// 0067D040 HandleLMouseDown() RET (tail jump to CSWGuiControl::HandleLMouseDown)
void __fastcall kotor_fastcall_0067d040(void* self, void*) {
    sw(self, 0x30C, lw(gui_manager(), 0x00));                                                   // 0067D040..0067D047
    sw(self, 0x310, lw(gui_manager(), 0x04));                                                   // 0067D04D..0067D055
    static_cast<kotor_input_hover::Control*>(self)->HandleLMouseDown();                         // 0067D05B jmp 0041AB90
}

// 0067D060 HandleMouseCapturedMovement(int x, int y) RET 8 -> 0
I32 __fastcall kotor_fastcall_0067d060(void* self, void*, I32 x, I32 y) {
    if (lw(self, 0x314) != 0 && lw(gui_manager(), 0x0C) == 1u && lp(gui_manager(), 0x54) != self) { // 0067D063..0067D07B
        const U8* resref = static_cast<const U8*>(kotor_m15e_slot00c_self_00641db0(at(self, 0xC0), nullptr)); // 0067D07D..0067D083
        if (*resref != 0) {                                                                     // 0067D088..0067D08B
            const U32 dx = static_cast<U32>(x) - lw(self, 0x30C);                               // 0067D08D..0067D097
            const U32 sx = static_cast<U32>(static_cast<I32>(dx) >> 31);                        // cdq
            const U32 dy = static_cast<U32>(y) - lw(self, 0x310);                               // 0067D0A3..0067D0AD
            const U32 sy = static_cast<U32>(static_cast<I32>(dy) >> 31);
            if (static_cast<I32>((dx ^ sx) - sx) > 0x0C || static_cast<I32>((dy ^ sy) - sy) > 0x0C) // 0067D099..0067D0B7 (jg / jle)
                sw(gui_manager(), 0x54, ad(self));                                              // 0067D0B9..0067D0BE
        }
    }
    return 0;                                                                                   // 0067D0C1
}

// 0067D0D0 HandleLMouseUp() RET
void __fastcall kotor_fastcall_0067d0d0(void* self, void*) {
    void* m = gui_manager();
    if (lp(m, 0x54) != self) {                                                                  // 0067D0D8..0067D0DD
        static_cast<kotor_input_hover::Control*>(self)->HandleLMouseUp();                       // 0067D107 jmp 0041ABD0
        return;
    }
    sw(m, 0x54, 0);                                                                             // 0067D0E1
    vcall<void, U32, U32>(self, 0x3C, pazaak::card::kEventDrop, 1u);                            // 0067D0DF..0067D0EF HandleInputEvent(0x17FEB, 1)
    kotor_fastcall_0040a200(gui_manager(), nullptr, self);                                      // 0067D0F2..0067D0F9 ReleaseMouse
    vcall<void>(self, 0x14);                                                                    // 0067D0FE..0067D103 jmp [vtbl+14]
}

// 0067DDD0 CSWGuiPazaakCard() RET
void* __fastcall kotor_fastcall_0067ddd0(void* self, void*) noexcept(false) {
    gui_load_button_construct_0041c0f0(self, nullptr);                                          // 0067DDEE
    auto unwind = on_throw([&]() { gui_load_button_destruct_00419da0(self, nullptr); });        // state 0
    set_vptr(self, kotor_pazaak_vtable_007531c0());                                             // 0067DE03 (007531C0)
    gui_load_label_construct_0041acd0(at(self, 0x1C4), nullptr);                                // 0067DDF3..0067DE09
    unwind.armed = false;
    sw(self, 0x308, 0);                                                                         // 0067DE0E
    sw(self, 0x314, 1); sw(self, 0x318, 1);                                                     // 0067DE18..0067DE23
    sw(self, 0x304, 0xFFFFFFFFu);                                                               // 0067DE29..0067DE2C
    sw(self, 0x308, 0);                                                                         // 0067DE32..0067DE34
    sw(self, 0x1C4 + 0x44, lw(self, 0x1C4 + 0x44) | 0x20u);                                     // 0067DE3A..0067DE44
    return self;
}

// 0067DE60 GetIsSelectable() RET
I32 __fastcall kotor_fastcall_0067de60(void* self, void*) {
    return static_cast<I32>((lw(self, 0x44) >> 3) & lw(self, 0x318) & 1u);                      // 0067DE60..0067DE6E
}

// 0067DE80 ~CSWGuiPazaakCard() RET
void __fastcall kotor_fastcall_0067de80(void* self, void*) {
    set_vptr(self, kotor_pazaak_vtable_007531c0());                                             // 0067DE9D
    void* m = gui_manager();                                                                    // 0067DEA3
    if (lp(m, 0x54) == self) sw(m, 0x54, 0);                                                    // 0067DEA8..0067DEB5
    gui_load_label_destruct_00419d10(at(self, 0x1C4), nullptr);                                 // 0067DEB8..0067DEBE
    gui_load_button_destruct_00419da0(self, nullptr);                                           // 0067DEC3..0067DECD
}

// 0067EFC0 scalar deleting destructor RET 4
void* __fastcall kotor_fastcall_0067efc0(void* self, void*, U32 flags) {
    return deleting(self, &kotor_fastcall_0067de80, flags);                                     // 0067EFC3..0067EFD8
}

// ======================= CSWGuiTutorial =======================
// 0067D110 PlaceRightOf(CSWGuiControl*) RET 4
void __fastcall kotor_fastcall_0067d110(void* self, void*, void* control) {
    I32 e[4]; std::memcpy(e, at(control, 4), 16);                                              // 0067D11C..0067D12B [esp+10..1C]
    I32 right = static_cast<I32>(static_cast<U32>(e[2]) + static_cast<U32>(e[0]));             // 0067D12E lea ebp,[esi+edx]
    I32 mid = static_cast<I32>(static_cast<U32>(e[3] / 2) + static_cast<U32>(e[1]));           // 0067D139..0067D144
    void* parent = lp(control, 0x34);                                                           // 0067D121
    if (parent != nullptr) {                                                                    // 0067D146..0067D14C
        void* panel = vcall<void*>(parent, 0x28);                                               // 0067D14E..0067D150
        if (panel != nullptr) gui_load_panel_offset_extent_0040aa00(panel, nullptr, e);         // 0067D153..0067D15E
        right = static_cast<I32>(static_cast<U32>(right) + static_cast<U32>(e[0]));             // 0067D163..0067D16B
        mid = static_cast<I32>(static_cast<U32>(mid) + static_cast<U32>(e[1]));                 // 0067D167..0067D16D
    }
    std::memcpy(e, at(self, 4), 16);                                                           // 0067D16F..0067D185
    mid = static_cast<I32>(static_cast<U32>(mid) - static_cast<U32>(e[3] / 2));                // 0067D189..0067D190
    e[0] = right; e[1] = mid;                                                                   // 0067D19D..0067D1A1
    vcall<void, const void*>(self, 0x04, e);                                                    // 0067D196..0067D1A5 SetExtent
}

// 0067D1C0 PlaceAbove(CSWGuiControl*) RET 4
void __fastcall kotor_fastcall_0067d1c0(void* self, void*, void* control) {
    sw(self, 0x998, ad(control));                                                               // 0067D1CE
    I32 e[4]; std::memcpy(e, at(control, 4), 16);                                              // 0067D1CB..0067D1F3
    I32 left = e[0], top = e[1];                                                                // ebx / edi
    void* parent = lp(control, 0x34);                                                           // 0067D1D4
    if (parent != nullptr) {                                                                    // 0067D1D7..0067D1F9
        void* panel = vcall<void*>(parent, 0x28);                                               // 0067D1FB..0067D1FF
        if (panel != nullptr) gui_load_panel_offset_extent_0040aa00(panel, nullptr, e);         // 0067D202..0067D20D
        left = static_cast<I32>(static_cast<U32>(left) + static_cast<U32>(e[0]));               // 0067D212..0067D21A
        top = static_cast<I32>(static_cast<U32>(top) + static_cast<U32>(e[1]));                 // 0067D216..0067D21C
    }
    std::memcpy(e, at(self, 4), 16);                                                           // 0067D21E..0067D23C
    top = static_cast<I32>(static_cast<U32>(top) - static_cast<U32>(e[3]));                     // 0067D236
    e[0] = left; e[1] = top;                                                                    // 0067D247..0067D24B
    vcall<void, const void*>(self, 0x04, e);                                                    // 0067D240..0067D24F SetExtent
}

// 0067D260 HandleInputEvent(int event, int down) RET 8
void __fastcall kotor_fastcall_0067d260(void* self, void*, I32 event, I32 down) {
    enum { Base, Prev, Next } path = Base;
    if (down != 0) {                                                                            // 0067D264..0067D26D
        if (event > 0x2E) {                                                                     // 0067D26F..0067D272 (jg)
            if (event == 0x1F6) path = Next;                                                    // 0067D28D..0067D295
            else if (event == 0x1F7) path = Prev;                                               // 0067D297..0067D298
        } else if (event == 0x2E) path = Prev;                                                  // 0067D274
        else if (event == 0x27) path = Next;                                                    // 0067D276..0067D27B
        else if (event == 0x28) path = Prev;                                                    // 0067D27D..0067D27E
    }
    if (path == Prev && li(self, 0x990) > 0) {                                                  // 0067D29A..0067D2A2 (jle -> base)
        kotor_fastcall_0040a140(mgr_of(self), nullptr, 0);                                      // 0067D2A4..0067D2A9
        sw(self, 0x990, lw(self, 0x990) - 1u);                                                  // 0067D2AE
        if (lw(self, 0x990) == 0) kotor_fastcall_00627130(self, nullptr, 0);                    // 0067D2B4..0067D2BA SetAllowCancel(0)
        vcall<void, U32>(self, 0x6C, lw(lp(self, 0x984), lw(self, 0x990) * 4u));                // 0067D2BF..0067D2D3 SetMessage(strref)
    } else if (path == Next && li(self, 0x990) < static_cast<I32>(lw(self, 0x988) - 1u)) {     // 0067D2EC..0067D2FD (jge -> base)
        kotor_fastcall_0040a140(mgr_of(self), nullptr, 0);                                      // 0067D2FF..0067D304
        sw(self, 0x990, lw(self, 0x990) + 1u);                                                  // 0067D309
        kotor_fastcall_00627130(self, nullptr, 1);                                              // 0067D30F..0067D313 SetAllowCancel(1)
        vcall<void, U32>(self, 0x6C, lw(lp(self, 0x984), lw(self, 0x990) * 4u));                // 0067D318..0067D32C
    } else {
        gui_m48_message_box_handle_input_006250f0(self, nullptr, event, down);                  // 0067D280..0067D284
        return;
    }
    void* anchor = lp(self, 0x998);                                                             // 0067D2D6
    if (anchor != nullptr) kotor_fastcall_0067d1c0(self, nullptr, anchor);                      // 0067D2DC..0067D2E3 PlaceAbove
}

// 0067DEF0 CSWGuiTutorial(CSWGuiManager*) RET 4
void* __fastcall kotor_fastcall_0067def0(void* self, void*, void* manager) noexcept(false) {
    kotor_original::kotor_fastcall_00626df0(self, nullptr, manager);                            // 0067DF0A..0067DF0D
    set_vptr(self, kotor_pazaak_vtable_00753260());                                             // 0067DF18 (00753260)
    sw(self, 0x988, 0); sw(self, 0x98C, 0); sw(self, 0x984, 0);                                 // 0067DF1E..0067DF2A
    sw(self, 0x990, 0); sw(self, 0x998, 0);                                                     // 0067DF30..0067DF36
    sw(self, 0x994, 1);                                                                         // 0067DF3C
    return self;
}

// 0067DF60 ~CSWGuiTutorial() RET (no vptr store in the original)
void __fastcall kotor_fastcall_0067df60(void* self, void*) {
    void* data = lp(self, 0x984);                                                               // 0067DF7D
    if (data != nullptr) live_free(data);                                                       // 0067DF83..0067DF90 _free
    gui_m48_message_box_destruct_006270a0(self, nullptr);                                       // 0067DF98..0067DFA2
}

// 0067DFC0 ShowHelp(message strref, button A strref, button B strref) RET 0Ch
void __fastcall kotor_fastcall_0067dfc0(void* self, void*, U32 message, I32 button_a, U32 button_b) {
    const U32 flags = lw(self, 0x44);                                                           // 0067DFC4
    sw(self, 0x988, 0); sw(self, 0x990, 0); sw(self, 0x998, 0);                                 // 0067DFCC..0067DFD8
    if ((flags & 0x600u) != 0) {                                                                // 0067DFC9 test ah,6
        sw(self, 0x44, flags & 0xFFFFF8FFu);                                                    // 0067DFE3..0067DFE9
        frame_RemovePanel_0040c830(mgr_of(self), nullptr, self);                                // 0067DFE0..0067DFEC
    }
    if (lw(self, 0x994) == 0) return;                                                           // 0067DFF1..0067DFF7
    list_add(at(self, 0x984), message);                                                         // 0067DFFD..0067E03A
    if (button_a == -1 && button_b == 0x96E2u) {                                                // 0067E041..0067E04C
        kotor_fastcall_00627130(self, nullptr, 0);                                              // 0067E04E..0067E052
        gui_load_text_params_set_strref_00415e50(at(self, 0x460), nullptr, 0x96DFu);            // 0067E057..0067E062 -> 0067E080
    } else {
        kotor_fastcall_00627130(self, nullptr, 1);                                              // 0067E064..0067E068
        gui_load_text_params_set_strref_00415e50(at(self, 0x460), nullptr, static_cast<U32>(button_a)); // 0067E06D..0067E074
        gui_load_text_params_set_strref_00415e50(at(self, 0x624), nullptr, button_b);           // 0067E079..0067E080
    }
    vcall<void, U32>(self, 0x6C, message);                                                      // 0067E085..0067E08A SetMessage(strref)
    kotor_fastcall_0040bc70(mgr_of(self), nullptr, self, 1, 1);                                 // 0067E08D..0067E095 AddPanel(this,1,1)
}

// 0067E0B0 ShowMultipleHelp(const unsigned* strrefs, int count) RET 8
void __fastcall kotor_fastcall_0067e0b0(void* self, void*, const U32* strrefs, I32 count) {
    if (lw(self, 0x994) == 0) return;                                                           // 0067E0B4..0067E0C0
    kotor_fastcall_0067dfc0(self, nullptr, strrefs[0], -1, 0x96E2u);                            // 0067E0C7..0067E0D3
    for (I32 i = 1; i < count; ++i) list_add(at(self, 0x984), strrefs[i]);                      // 0067E0D8..0067E127
    gui_load_text_params_set_strref_00415e50(at(self, 0x624), nullptr, 0x96E0u);                // 0067E12E..0067E139
}

// 0067EFE0 scalar deleting destructor RET 4
void* __fastcall kotor_fastcall_0067efe0(void* self, void*, U32 flags) {
    return deleting(self, &kotor_fastcall_0067df60, flags);                                     // 0067EFE3..0067EFF8
}

// ======================= method tables (original diff against the base table; gui_vtables.cpp pattern) ===========
}  // extern "C"
namespace {
const Override ovr_activated[] = {
    {0x00, fn(&kotor_fastcall_0067efa0)},   // 0067EFA0 CSWGuiSpeedButton::~CSWGuiSpeedButton (shared, existing)
    {0x38, fn(&kotor_fastcall_0067cb70)},   // 0067CB70 Draw
    {0x88, fn(&kotor_fastcall_0067cc90)},   // 0067CC90 SetEnabled
};
const Override ovr_card[] = {
    {0x00, fn(&kotor_fastcall_0067efc0)}, {0x10, fn(&kotor_fastcall_0067d060)}, {0x18, fn(&kotor_fastcall_0067d040)},
    {0x1C, fn(&kotor_fastcall_0067d0d0)}, {0x38, fn(&kotor_fastcall_0067ca80)}, {0x3C, fn(&kotor_fastcall_0067cac0)},
    {0x48, fn(&kotor_fastcall_0067cd00)}, {0x80, fn(&kotor_fastcall_0067de60)}, {0x94, fn(&kotor_fastcall_0067cf00)},
};
const Override ovr_tutorial[] = {
    {0x00, fn(&kotor_fastcall_0067efe0)},   // 0067EFE0 scalar deleting dtor
    {0x3C, fn(&kotor_fastcall_0067d260)},   // 0067D260 HandleInputEvent
};
void* table_activated[40]; bool built_activated = false;
void* table_card[40]; bool built_card = false;
void* table_tutorial[33]; bool built_tutorial = false;
}  // namespace
extern "C" {
const void* __cdecl kotor_pazaak_vtable_00753120() {        // CSWGuiActivatedButton (40 slots; base 0073E658)
    if (!built_activated) { vt_build(table_activated, 40, gui_load_button_methods_0073e658, 40, ovr_activated, 3); built_activated = true; }
    return table_activated;
}
const void* __cdecl kotor_pazaak_vtable_007531c0() {        // CSWGuiPazaakCard (40 slots; base 0073E658)
    if (!built_card) { vt_build(table_card, 40, gui_load_button_methods_0073e658, 40, ovr_card, 9); built_card = true; }
    return table_card;
}
const void* __cdecl kotor_pazaak_vtable_00753260() {        // CSWGuiTutorial (33 slots; base 0074FDB0)
    if (!built_tutorial) { vt_build(table_tutorial, 33, gui_m48_message_box_methods_0074fdb0, 33, ovr_tutorial, 2); built_tutorial = true; }
    return table_tutorial;
}
}  // extern "C"
