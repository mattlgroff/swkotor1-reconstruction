// lane-prepare: CClientExoAppInternal::SetEventDescriptions 005EEB10..005F0ACD
// (swkotor.exe GOG K1, sha256 9c10e045...1435). Native thiscall, RET, no stack
// arguments; D1 form __fastcall(self, unused EDX). EH frame 0072575F: the four
// frame locals (CExoIni +40h, CExoStrings +18h/+44h/+B0h; states -1..3) and every
// temporary CExoString are C++ automatic objects whose destruction order equals
// the original state unwinding. Statement comments cite instruction addresses.
// The prelude (005EEB3D..005EF1E7) is a mechanical, count-checked transcription
// of the CreateNewEvent/AddEvent/ScaleEvent call stream.
#include "0c319006bf-event_descriptions.h"
#include "689940810d-string_priority.h"     // GuiConcat005e5d10 (CExoString::operator+), GuiAssign005e5140
#include "9281d0a58c-warning_runtime.h"     // kotor_reconstruction::StartupFormat005e5680 (CExoString::Format)
#include "9e32108632-base_ini.h"            // base_ini_reconstruction::PublicIni (CExoIni 005E6750/67C0/67D0/67E0)
#include "369787b919-window_shared_cells.h" // kotor_menu_window_cells::input_007a39e4 / manager_007a39fc
#include <windows.h>
#include <intrin.h>
#include <cstring>
#include <new>
#include "66a5366142-menu_targets.h"   // M41 kotor_menu20::String::as_int_005e5630: sole CExoString::AsINT owner
namespace { inline std::int32_t as_int_005e5630(kotor_reconstruction::StartupString* s) { return reinterpret_cast<kotor_menu20::String*>(s)->as_int_005e5630(); } }

namespace kotor_game_prepare {
namespace {
using Word = std::uint32_t;
using SS = kotor_reconstruction::StartupString;

// CExoInput singleton cell 007A39E4, re-read before every call ('mov ecx,[007A39E4h]').
void* input() noexcept { return kotor_menu_window_cells::input_007a39e4; }
Word load32(const void* base, unsigned off) noexcept {
    Word v; std::memcpy(&v, static_cast<const unsigned char*>(base) + off, 4); return v;
}
void* ptr(Word w) noexcept { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(w)); }
// C2DA* = [[007A3A28]+B8h]+68h, re-read at every use (e.g. 005EF371..005EF37C).
// No null test exists on the reads that feed a call (except where cited).
void* table() noexcept {
    return ptr(load32(ptr(load32(kotor_cell_rules_007a3a28, 0xB8)), 0x68));
}
// Storage for a CExoString constructed by a callee (005E5BC0 / 005E5D10 result).
class Slot {
    alignas(4) unsigned char bytes_[sizeof(SS)];
    SS* object_ = nullptr;
public:
    Slot() noexcept = default;
    Slot(const Slot&) = delete;
    Slot& operator=(const Slot&) = delete;
    SS* raw() noexcept { return static_cast<SS*>(static_cast<void*>(bytes_)); }
    SS* adopt(SS* p) noexcept { object_ = p; return p; }
    SS* get() const noexcept { return object_; }
    ~Slot() { if (object_) object_->~SS(); }   // 005E5C20
};
// "<prefix><n>": CExoString(n) (005E5BC0), CExoString(prefix) (005E5A90),
// operator+ (005E5D10), target = result (005E5C50), then destroy result,
// prefix, number (005E5C20 x3) - e.g. 005EF2E5..005EF35E.
void label(SS* target, std::int32_t n, const char* prefix) {
    Slot number; number.adopt(kotor_fastcall_005e5bc0(number.raw(), nullptr, n));
    SS head(prefix);
    Slot joined; joined.adopt(GuiConcat005e5d10(&head, nullptr, joined.raw(), number.get()));
    target->Assign005e5c50(joined.get());
}
// "Action<n><suffix>": 005EF860..005EF93D / 005F03E4..005F04C4 construct
// CExoString(n), CExoString(suffix), CExoString("Action"); t = "Action"+n;
// r = t + suffix; target = r; destroy r, t, "Action", suffix, n.
void label_suffix(SS* target, std::int32_t n, const char* suffix) {
    Slot number; number.adopt(kotor_fastcall_005e5bc0(number.raw(), nullptr, n));
    SS tail(suffix);
    SS head("Action");                                                      // 0074E7D8
    Slot first; first.adopt(GuiConcat005e5d10(&head, nullptr, first.raw(), number.get()));
    Slot second; second.adopt(GuiConcat005e5d10(first.get(), nullptr, second.raw(), &tail));
    target->Assign005e5c50(second.get());
}
// C2DA::GetINTEntry(row, CExoString(column), out) with the column temporary
// constructed before the table read and destroyed after the call.
int int_entry(SS* row, const char* column, std::int32_t* out) {
    SS col(column);
    return kotor_fastcall_00414110(table(), nullptr, row, &col, out);
}
int float_entry(SS* row, const char* column, float* out) {
    SS col(column);
    return kotor_fastcall_00413fa0(table(), nullptr, row, &col, out);
}
// The zero-filled 256-DWORD table at frame+E0h (005EF23A rep stosd 100h) is
// only ever read (005EF5FC, 005EF715, 005EFD65, 005EFD6E); no store exists.
// Each read is 'cmp dword ptr [esp+reg*4+0E0h],ebp(0)': a scaled-index load with
// no upper bound. Only the jl tests (005EF70F, 005EFD5F, 005EFD63) exclude a
// negative index, and 005EF5F7 masks the INI key to 8 bits. An index >= 256
// reads whatever lies past the table (the original's saved-register/EH words
// and beyond) or faults if that address is unmapped. Kept as the same unchecked
// read: table + index*4 in 32-bit address arithmetic (wrapping like the SIB
// form), loaded through an integer address in a noinline helper so it is a real
// load and not a C++ out-of-bounds subscript. The words past this build's table
// are this frame's, not the original's.
__declspec(noinline) Word frame_word(std::uintptr_t table, std::int32_t index) noexcept {
    const std::uint32_t address = static_cast<std::uint32_t>(table) + static_cast<std::uint32_t>(index) * 4u;
    return *reinterpret_cast<const volatile Word*>(static_cast<std::uintptr_t>(address));
}
Word used_at(const Word (&used)[256], std::int32_t index) noexcept {
    return frame_word(reinterpret_cast<std::uintptr_t>(&used[0]), index);
}
}

void __fastcall SetEventDescriptions005eeb10(void* self, void*) {
    const float one_f = 1.0f;                                               // push 3F800000h
    kotor_fastcall_005df600(input(), nullptr);                              // 005EEB36/005EEB3D
    // .rdata constant words read by the prelude (load-image values):
    const Word k_0074d3c8 = 0u; // [0074D3C8]
    const Word k_0074d3cc = 1u; // [0074D3CC]
    const Word k_0074d3d0 = 2u; // [0074D3D0]
    const Word k_0074d3e0 = 0u; // [0074D3E0]
    const Word k_0074d3e4 = 1u; // [0074D3E4]
    const Word k_0074d3e8 = 2u; // [0074D3E8]
    const Word k_0074d3ec = 3u; // [0074D3EC]
    const Word k_0074d3f0 = 4u; // [0074D3F0]
    const Word k_0074d3f4 = 5u; // [0074D3F4]
    const Word k_0074d3f8 = 6u; // [0074D3F8]
    const Word k_0074d3fc = 7u; // [0074D3FC]
    const Word k_0074d400 = 8u; // [0074D400]
    const Word k_0074d404 = 9u; // [0074D404]
    const Word k_0074d408 = 10u; // [0074D408]
    const Word k_0074d478 = 38u; // [0074D478]
    const Word k_0074d540 = 88u; // [0074D540]
    const Word k_0074d594 = 110u; // [0074D594]
    const Word k_0074d598 = 111u; // [0074D598]
    const Word k_0074d5a0 = 113u; // [0074D5A0]
    const Word k_0074d5a4 = 114u; // [0074D5A4]
    const Word k_0074d5cc = 124u; // [0074D5CC]
    kotor_fastcall_005df460(input(), nullptr, 0xB5u, 0x1u, k_0074d3c8 /*[0074D3C8]=0*/, k_0074d3f8 /*[0074D3F8]=6*/, 0x84u); // 005EEB61
    kotor_fastcall_005df470(input(), nullptr, 0xB5u, 0x2u); // 005EEB73
    kotor_fastcall_005df470(input(), nullptr, 0xB5u, 0x3u); // 005EEB85
    kotor_fastcall_005df470(input(), nullptr, 0xB5u, 0x4u); // 005EEB97
    kotor_fastcall_005df460(input(), nullptr, 0xB6u, 0x1u, k_0074d3c8 /*[0074D3C8]=0*/, k_0074d404 /*[0074D404]=9*/, 0x84u); // 005EEBBB
    kotor_fastcall_005df470(input(), nullptr, 0xB6u, 0x2u); // 005EEBCD
    kotor_fastcall_005df470(input(), nullptr, 0xB6u, 0x3u); // 005EEBDF
    kotor_fastcall_005df460(input(), nullptr, 0xB7u, 0x1u, k_0074d3c8 /*[0074D3C8]=0*/, k_0074d408 /*[0074D408]=10*/, 0x84u); // 005EEC04
    kotor_fastcall_005df470(input(), nullptr, 0xB7u, 0x2u); // 005EEC16
    kotor_fastcall_005df470(input(), nullptr, 0xB7u, 0x3u); // 005EEC28
    kotor_fastcall_005df460(input(), nullptr, 0xB8u, 0x1u, k_0074d3c8 /*[0074D3C8]=0*/, k_0074d3fc /*[0074D3FC]=7*/, 0x84u); // 005EEC4C
    kotor_fastcall_005df470(input(), nullptr, 0xB8u, 0x2u); // 005EEC5E
    kotor_fastcall_005df470(input(), nullptr, 0xB8u, 0x3u); // 005EEC70
    kotor_fastcall_005df460(input(), nullptr, 0xB9u, 0x1u, k_0074d3c8 /*[0074D3C8]=0*/, k_0074d400 /*[0074D400]=8*/, 0x84u); // 005EEC94
    kotor_fastcall_005df470(input(), nullptr, 0xB9u, 0x2u); // 005EECA6
    kotor_fastcall_005df470(input(), nullptr, 0xB9u, 0x3u); // 005EECB8
    kotor_fastcall_005df460(input(), nullptr, 0xBBu, 0x1u, k_0074d3c8 /*[0074D3C8]=0*/, k_0074d540 /*[0074D540]=88*/, 0x84u); // 005EECDD
    kotor_fastcall_005df470(input(), nullptr, 0xBBu, 0x2u); // 005EECEF
    kotor_fastcall_005df470(input(), nullptr, 0xBBu, 0x3u); // 005EED01
    {   // 005EED06..005EED1A: CClientExoApp::GetClientOptions(manager->client) byte +58h
        void* client = ptr(load32(kotor_menu_window_cells::manager_007a39fc, 4)); // 005EED06/005EED0B
        const void* options = kotor_fastcall_005ed700(client, nullptr);          // 005EED0E
        if (static_cast<const unsigned char*>(options)[0x58] != 0) {              // 005EED13..005EED1A
        kotor_fastcall_005df460(input(), nullptr, 0x50u, 0x1u, k_0074d3c8 /*[0074D3C8]=0*/, k_0074d478 /*[0074D478]=38*/, 0x84u); // 005EED39
        kotor_fastcall_005df470(input(), nullptr, 0x50u, 0x0u); // 005EED47
        kotor_fastcall_005df470(input(), nullptr, 0x50u, 0x2u); // 005EED56
        kotor_fastcall_005df470(input(), nullptr, 0x50u, 0x4u); // 005EED65
        }
    }
    kotor_fastcall_005df460(input(), nullptr, 0x7u, 0x3u, k_0074d3d0 /*[0074D3D0]=2*/, k_0074d598 /*[0074D598]=111*/, 0x84u); // 005EED86
    kotor_fastcall_005df470(input(), nullptr, 0x7u, 0x0u); // 005EED94
    kotor_fastcall_005df480(input(), nullptr, 0x7u, one_f /*3F800000h*/, one_f /*3F800000h*/); // 005EEDAB
    kotor_fastcall_005df460(input(), nullptr, 0x4u, 0x1u, k_0074d3d0 /*[0074D3D0]=2*/, k_0074d598 /*[0074D598]=111*/, 0x84u); // 005EEDCC
    kotor_fastcall_005df470(input(), nullptr, 0x4u, 0x4u); // 005EEDDB
    kotor_fastcall_005df460(input(), nullptr, 0x8u, 0x3u, k_0074d3d0 /*[0074D3D0]=2*/, k_0074d594 /*[0074D594]=110*/, 0x84u); // 005EEDFD
    kotor_fastcall_005df470(input(), nullptr, 0x8u, 0x0u); // 005EEE0B
    kotor_fastcall_005df480(input(), nullptr, 0x8u, one_f /*3F800000h*/, one_f /*3F800000h*/); // 005EEE22
    kotor_fastcall_005df460(input(), nullptr, 0x3u, 0x1u, k_0074d3d0 /*[0074D3D0]=2*/, k_0074d594 /*[0074D594]=110*/, 0x84u); // 005EEE43
    kotor_fastcall_005df470(input(), nullptr, 0x3u, 0x4u); // 005EEE52
    kotor_fastcall_005df460(input(), nullptr, 0xCu, 0x3u, k_0074d3d0 /*[0074D3D0]=2*/, k_0074d5a0 /*[0074D5A0]=113*/, 0x84u); // 005EEE73
    kotor_fastcall_005df470(input(), nullptr, 0xCu, 0x0u); // 005EEE81
    kotor_fastcall_005df470(input(), nullptr, 0xCu, 0x4u); // 005EEE90
    kotor_fastcall_005df480(input(), nullptr, 0xCu, one_f /*3F800000h*/, one_f /*3F800000h*/); // 005EEEA7
    kotor_fastcall_005df460(input(), nullptr, 0xDu, 0x3u, k_0074d3d0 /*[0074D3D0]=2*/, k_0074d5a4 /*[0074D5A4]=114*/, 0x84u); // 005EEEC9
    kotor_fastcall_005df470(input(), nullptr, 0xDu, 0x0u); // 005EEED7
    kotor_fastcall_005df470(input(), nullptr, 0xDu, 0x4u); // 005EEEE6
    kotor_fastcall_005df480(input(), nullptr, 0xDu, one_f /*3F800000h*/, one_f /*3F800000h*/); // 005EEEFD
    kotor_fastcall_005df460(input(), nullptr, 0xBu, 0x1u, k_0074d3d0 /*[0074D3D0]=2*/, k_0074d5cc /*[0074D5CC]=124*/, 0x84u); // 005EEF1E
    kotor_fastcall_005df470(input(), nullptr, 0xBu, 0x0u); // 005EEF2C
    kotor_fastcall_005df470(input(), nullptr, 0xBu, 0x4u); // 005EEF3B
    kotor_fastcall_005df460(input(), nullptr, 0x17u, 0x3u, k_0074d3cc /*[0074D3CC]=1*/, k_0074d3ec /*[0074D3EC]=3*/, 0x84u); // 005EEF5C
    kotor_fastcall_005df470(input(), nullptr, 0x17u, 0x1u); // 005EEF6B
    kotor_fastcall_005df460(input(), nullptr, 0x18u, 0x3u, k_0074d3cc /*[0074D3CC]=1*/, k_0074d3f0 /*[0074D3F0]=4*/, 0x84u); // 005EEF8D
    kotor_fastcall_005df470(input(), nullptr, 0x18u, 0x1u); // 005EEF9C
    kotor_fastcall_005df460(input(), nullptr, 0x19u, 0x1u, k_0074d3cc /*[0074D3CC]=1*/, k_0074d3e0 /*[0074D3E0]=0*/, 0x84u); // 005EEFBD
    kotor_fastcall_005df470(input(), nullptr, 0x19u, 0x1u); // 005EEFCC
    kotor_fastcall_005df460(input(), nullptr, 0x41u, 0x3u, k_0074d3cc /*[0074D3CC]=1*/, k_0074d3ec /*[0074D3EC]=3*/, 0x84u); // 005EEFED
    kotor_fastcall_005df470(input(), nullptr, 0x41u, 0x2u); // 005EEFFC
    kotor_fastcall_005df470(input(), nullptr, 0x41u, 0x0u); // 005EF00A
    kotor_fastcall_005df470(input(), nullptr, 0x41u, 0x3u); // 005EF019
    kotor_fastcall_005df470(input(), nullptr, 0x41u, 0x4u); // 005EF028
    kotor_fastcall_005df460(input(), nullptr, 0x42u, 0x3u, k_0074d3cc /*[0074D3CC]=1*/, k_0074d3f0 /*[0074D3F0]=4*/, 0x84u); // 005EF04A
    kotor_fastcall_005df470(input(), nullptr, 0x42u, 0x2u); // 005EF059
    kotor_fastcall_005df470(input(), nullptr, 0x42u, 0x0u); // 005EF067
    kotor_fastcall_005df470(input(), nullptr, 0x42u, 0x3u); // 005EF076
    kotor_fastcall_005df470(input(), nullptr, 0x42u, 0x4u); // 005EF085
    kotor_fastcall_005df460(input(), nullptr, 0x43u, 0x1u, k_0074d3cc /*[0074D3CC]=1*/, k_0074d3e0 /*[0074D3E0]=0*/, 0x84u); // 005EF0A6
    kotor_fastcall_005df470(input(), nullptr, 0x43u, 0x0u); // 005EF0B4
    kotor_fastcall_005df470(input(), nullptr, 0x43u, 0x2u); // 005EF0C3
    kotor_fastcall_005df470(input(), nullptr, 0x43u, 0x3u); // 005EF0D2
    kotor_fastcall_005df470(input(), nullptr, 0x43u, 0x4u); // 005EF0E1
    kotor_fastcall_005df460(input(), nullptr, 0x44u, 0x1u, k_0074d3cc /*[0074D3CC]=1*/, k_0074d3e4 /*[0074D3E4]=1*/, 0x84u); // 005EF102
    kotor_fastcall_005df470(input(), nullptr, 0x44u, 0x0u); // 005EF110
    kotor_fastcall_005df470(input(), nullptr, 0x44u, 0x2u); // 005EF11F
    kotor_fastcall_005df470(input(), nullptr, 0x44u, 0x3u); // 005EF12E
    kotor_fastcall_005df470(input(), nullptr, 0x44u, 0x4u); // 005EF13D
    kotor_fastcall_005df460(input(), nullptr, 0x45u, 0x1u, k_0074d3cc /*[0074D3CC]=1*/, k_0074d3e8 /*[0074D3E8]=2*/, 0x84u); // 005EF15F
    kotor_fastcall_005df470(input(), nullptr, 0x45u, 0x2u); // 005EF16E
    kotor_fastcall_005df470(input(), nullptr, 0x45u, 0x4u); // 005EF17D
    kotor_fastcall_005df470(input(), nullptr, 0x45u, 0x0u); // 005EF18B
    kotor_fastcall_005df460(input(), nullptr, 0x46u, 0x1u, k_0074d3cc /*[0074D3CC]=1*/, k_0074d3f4 /*[0074D3F4]=5*/, 0x84u); // 005EF1AC
    kotor_fastcall_005df470(input(), nullptr, 0x46u, 0x2u); // 005EF1BB
    kotor_fastcall_005df470(input(), nullptr, 0x46u, 0x0u); // 005EF1C9
    kotor_fastcall_005df470(input(), nullptr, 0x46u, 0x3u); // 005EF1D8
    kotor_fastcall_005df470(input(), nullptr, 0x46u, 0x4u); // 005EF1E7

    // ---- 005EF1EC..005EF22A: frame locals (EH states 0..3) -----------------
    base_ini_reconstruction::PublicIni ini;
    ini.construct_005e6750();                                               // 005EF1EC/005EF1F0 (state 0)
    struct IniScope {                                                       // state -1 unwind: 005E67E0
        base_ini_reconstruction::PublicIni& ini;
        ~IniScope() { ini.destroy_005e67e0(); }                             // 005F0AB3
    } ini_scope{ini};
    SS row;                                                                 // +18h 005EF200 (state 1)
    SS value;                                                               // +44h 005EF211 (state 2)
    SS language;                                                            // +B0h 005EF225 (state 3)
    Word used[256];
    std::memset(used, 0, sizeof(used));                                     // 005EF238..005EF246
    std::int32_t out14 = 0;                                                 // +14h: written only by callees below
    std::int32_t t20 = 0;                                                   // +20h
    // ---- 005EF248..005EF487: language column "Language<lang>" --------------
    if (table()) {                                                          // 005EF248..005EF253
        label(&language, static_cast<std::int32_t>(load32(self, 0x144)), "Language"); // 005EF259..005EF2DB ([esi+144h], 0074E7E0)
        std::int32_t n = 0xC8;                                              // 005EF2E0
        bool found = false;
        for (;;) {
            label(&row, n, "Action");                                       // 005EF2E5..005EF35E (0074E7D8)
            const bool disabled = int_entry(&row, "Disabled", &out14) != 0 && out14 == 1; // 005EF363..005EF3BF (0074E7CC)
            if (!disabled) {                                                // 005EF3C4/005EF3C6
                if (kotor_fastcall_00414110(table(), nullptr, &row, &language, &out14) != 0 && // 005EF3C8..005EF3F2
                    out14 >= 0) { found = true; break; }                    // 005EF3F4/005EF3F8 jge 005EF48C
            }
            if (++n > 0x109) break;                                         // 005EF3FE..005EF405 (signed jle)
        }
        if (!found) label(&language, 0, "Language");                        // 005EF40B..005EF487 (push ebp=0)
    }
    // ---- 005EF48C..005EF853: Action 0C8h..109h key bindings -----------------
    for (Word id = 0xC8; id <= 0x109; ++id) {                               // 005EF48C / 005EF84C..005EF853 (jbe)
        label(&row, static_cast<std::int32_t>(id), "Action");               // 005EF491..005EF50A
        if (int_entry(&row, "Disabled", &out14) != 0 && out14 == 1) continue; // 005EF50F..005EF573 (no null test on table)
        int read;
        {
            SS section("Keymapping");                                       // 005EF579 (0074E7C0)
            SS file(".\\swkotor.ini");                                      // 005EF587 (0073D648)
            read = ini.read_005e67c0(&value, &file, &section, &row);       // 005EF59D..005EF5BD
        }                                                                   // 005EF5C2..005EF5E1 destroy file, section
        if (read != 0) {                                                    // 005EF5E6/005EF5E8
            const std::int32_t key = as_int_005e5630(&value) & 0xFF; // 005EF5EE..005EF5F7
            out14 = key;                                                    // 005EF603
            if (used_at(used, key) != 0 && key != 0) continue;              // 005EF5FC..005EF60B
            t20 = 0;                                                        // 005EF617
            std::int32_t key_arg = key;                                     // eax at 005EF686 when table is null
            if (table()) {                                                  // 005EF611..005EF624
                bool missing;
                {
                    SS column("EventType");                                 // 005EF626 (00746624)
                    missing = kotor_fastcall_00414110(table(), nullptr, &row, &column, &t20) == 0; // 005EF634..005EF66B
                }                                                           // 005EF675
                if (!missing && t20 == 1) {                                 // 005EF67A/005EF6A6/005EF6AB
                    kotor_fastcall_005df460(input(), nullptr, id, 0, 0u /*[0074D3C8]*/, // 005EF6AD..005EF6C5
                                            static_cast<Word>(out14), 0x84u);
                    continue;                                               // 005EF6CA
                }
                if (missing) t20 = 0;                                       // 005EF67E
                key_arg = out14;                                            // 005EF682
            }
            kotor_fastcall_005df460(input(), nullptr, id, 1, 0u /*[0074D3C8]*/, // 005EF686..005EF69C
                                    static_cast<Word>(key_arg), 0x84u);
            continue;                                                       // 005EF6A1
        }
        // Not in swkotor.ini: default from the language column.
        if (!table()) continue;                                             // 005EF6CF..005EF6E0
        if (kotor_fastcall_00414110(table(), nullptr, &row, &language, &out14) == 0) continue; // 005EF6E6..005EF703
        std::int32_t key = out14;                                           // 005EF709
        if (key < 0) continue;                                              // 005EF70D/005EF70F
        if (!(used_at(used, key) != 0 && key != 0)) {                       // 005EF715..005EF720 (else 005EF7C5)
            t20 = 0;                                                        // 005EF72C
            std::int32_t key_arg = key;                                     // eax at 005EF79E when table is null
            Word type = 1;
            if (table()) {                                                  // 005EF726..005EF739
                bool missing;
                {
                    SS column("EventType");                                 // 005EF73B (00746624)
                    missing = kotor_fastcall_00414110(table(), nullptr, &row, &column, &t20) == 0; // 005EF749..005EF77F
                }                                                           // 005EF789
                if (!missing && t20 == 1) type = 0;                         // 005EF790 -> 005EF991..005EF9AD (push ebp)
                else if (missing) t20 = 0;                                  // 005EF796
                key_arg = out14;                                            // 005EF79A / 005EF99C
            }
            kotor_fastcall_005df460(input(), nullptr, id, type, 0u /*[0074D3C8]*/, // 005EF79E..005EF7B4 / 005EF9A0..005EF7B4
                                    static_cast<Word>(key_arg), 0x84u);
            key = out14;                                                    // 005EF7B9
            if (key == 0) continue;                                         // 005EF7BD/005EF7BF
        }
        kotor_reconstruction::StartupFormat005e5680(&value, "%d", key);     // 005EF7C5..005EF7D5 (0073D720)
        {
            SS section("Keymapping");                                       // 005EF7D8 (0074E7C0)
            SS file(".\\swkotor.ini");                                      // 005EF7E9 (0073D648)
            (void)ini.write_005e67d0(&value, &file, &section, &row);        // 005EF7FF..005EF822 (result unused)
        }                                                                   // 005EF827..005EF847
    }
    // ---- 005EF859..005EFDA1: Action 118h..11Eh two-key bindings "A"/"B" ------
    for (Word id = 0x118; id <= 0x11E; ++id) {                              // 005EF859 / 005EFD9A..005EFDA1 (jbe)
        label_suffix(&row, static_cast<std::int32_t>(id), "A");             // 005EF860..005EF93D (0074E7BC)
        if (int_entry(&row, "Disabled", &out14) != 0 && out14 == 1) continue; // 005EF942..005EF9C7
        int read;
        {
            SS section("Keymapping");                                       // 005EF9CD
            SS file(".\\swkotor.ini");                                      // 005EF9DB
            read = ini.read_005e67c0(&value, &file, &section, &row);       // 005EF9F1..005EFA11
        }                                                                   // 005EFA16..005EFA35
        const std::int32_t unset = -1;                                      // 005EFA3A or ebx,-1
        out14 = unset;                                                      // 005EFA3F
        if (read != 0) {
            out14 = as_int_005e5630(&value) & 0xFF;       // 005EFA45..005EFA53
        } else if (table()) {                                               // 005EFA5C..005EFA6C
            (void)kotor_fastcall_00414110(table(), nullptr, &row, &language, &out14); // 005EFA72..005EFA88 (result unused)
            if (out14 != 0) {                                               // 005EFA8D..005EFA93
                kotor_reconstruction::StartupFormat005e5680(&value, "%d", out14); // 005EFA99..005EFAA9
                SS section("Keymapping");                                   // 005EFAAC
                SS file(".\\swkotor.ini");                                  // 005EFABA
                (void)ini.write_005e67d0(&value, &file, &section, &row);    // 005EFAD0..005EFAF0
            }                                                               // 005EFAF5..005EFB12
        }
        std::int32_t second = unset;                                        // 005EFB1C mov [esp+5Ch],ebx
        label_suffix(&row, static_cast<std::int32_t>(id), "B");             // 005EFB17..005EFBFB (00747C28)
        {
            SS section("Keymapping");                                       // 005EFC00
            SS file(".\\swkotor.ini");                                      // 005EFC0E
            read = ini.read_005e67c0(&value, &file, &section, &row);       // 005EFC24..005EFC44
        }                                                                   // 005EFC49..005EFC68
        if (read != 0) {
            second = as_int_005e5630(&value) & 0xFF;      // 005EFC71..005EFC7F
        } else if (table()) {                                               // 005EFC88..005EFC98
            (void)kotor_fastcall_00414110(table(), nullptr, &row, &language, &second); // 005EFC9E..005EFCB4
            if (second != 0) {                                              // 005EFCB9..005EFCBF
                kotor_reconstruction::StartupFormat005e5680(&value, "%d", second); // 005EFCC5..005EFCD5
                SS section("Keymapping");                                   // 005EFCD8
                SS file(".\\swkotor.ini");                                  // 005EFCE9
                (void)ini.write_005e67d0(&value, &file, &section, &row);    // 005EFD02..005EFD28
            }                                                               // 005EFD2D..005EFD50
        }
        const std::int32_t first = out14;                                   // 005EFD59
        if (first < 0 || second < 0) continue;                              // 005EFD5D..005EFD63
        const bool distinct_unused = used_at(used, first) == 0 && used_at(used, second) == 0 && first != second; // 005EFD65..005EFD79
        if (!distinct_unused && (first != 0 || second != 0)) continue;      // 005EFD7B..005EFD81
        kotor_fastcall_005df460(input(), nullptr, id, 4, 0u /*[0074D3C8]*/, // 005EFD83..005EFD95
                                static_cast<Word>(second), static_cast<Word>(first));
    }
    // ---- 005EFDA7..005F03D9: Action 0C8h..109h input contexts / repeat / scale
    for (Word id = 0xC8; id <= 0x109; ++id) {                               // 005EFDA7 / 005F03D2..005F03D9 (jbe)
        label(&row, static_cast<std::int32_t>(id), "Action");               // 005EFDB0..005EFE3E
        if (int_entry(&row, "Disabled", &out14) != 0 && out14 == 1) continue; // 005EFE43..005EFEB0
        if (kotor_fastcall_005df6f0(input(), nullptr, id) == 0) continue;    // 005EFEB6..005EFEC4
        std::int32_t v10 = 0;                                               // +10h: written only by callees
        static const char* const contexts[6] = {"ICPC", "ICMiniGame", "ICPCGUI", "ICDialog", "ICFreeLook", "ICMovie"};
        // 0074E7B4/0074E7A8/0074E7A0/0074E794/0074E788/0074E780 -> AddEvent(id,0..5)
        // at 005EFF3B/005EFFA9/005F0017/005F0085/005F00F3/005F0161.
        for (Word c = 0; c < 6; ++c)
            if (int_entry(&row, contexts[c], &v10) != 0 && v10 != 0)        // 005EFECA..005F0156
                kotor_fastcall_005df470(input(), nullptr, id, c);
        if (int_entry(&row, "Repeatable", &v10) != 0 && v10 != 0) {         // 005F0166..005F01C4 (0074E774)
            std::int32_t wait = 0, rate = 0;                                // +98h / +ACh: written only by callees
            bool ok;
            {
                SS wait_column("RepeatWait");                               // 005F01CA (0074E768)
                ok = kotor_fastcall_00414110(table(), nullptr, &row, &wait_column, &wait) != 0; // 005F01D8..005F020A
                if (ok) ok = int_entry(&row, "RepeatRate", &rate) != 0;     // 005F020C..005F025F (0074E75C)
            }                                                               // 005F0267..005F0273
            if (ok) kotor_fastcall_005df490(input(), nullptr, id, wait, rate); // 005F027C..005F0293
        }
        if (!(int_entry(&row, "Scale", &v10) != 0 && v10 != 0)) continue;   // 005F0298..005F02FF (0074E754)
        float magnitude = 0.0f, exponent = 0.0f;                            // +A8h / +9Ch: written only by callees
        bool ok;
        {
            SS mag_column("ScaleMag");                                      // 005F0305 (0074E748)
            ok = kotor_fastcall_00413fa0(table(), nullptr, &row, &mag_column, &magnitude) != 0; // 005F0313..005F0344
            if (ok) ok = float_entry(&row, "ScaleExp", &exponent) != 0;     // 005F0346..005F0399 (0074E73C)
        }                                                                   // 005F03A1..005F03AD
        if (ok) kotor_fastcall_005df480(input(), nullptr, id, magnitude, exponent); // 005F03B6..005F03CD
    }
    // ---- 005F03DF..005F0A68: Action 118h..11Eh ("A" label) contexts / repeat / scale
    for (Word id = 0x118; id <= 0x11E; ++id) {                              // 005F03DF / 005F0A61..005F0A68 (jbe)
        label_suffix(&row, static_cast<std::int32_t>(id), "A");             // 005F03E4..005F04C4 (0074E7BC)
        if (int_entry(&row, "Disabled", &out14) != 0 && out14 == 1) continue; // 005F04C9..005F052D
        if (kotor_fastcall_005df6f0(input(), nullptr, id) == 0) continue;    // 005F0533..005F0541
        std::int32_t v10 = 0;
        static const char* const contexts[6] = {"ICPC", "ICMiniGame", "ICPCGUI", "ICDialog", "ICFreeLook", "ICMovie"};
        for (Word c = 0; c < 6; ++c)                                        // 005F0547..005F07F9
            if (int_entry(&row, contexts[c], &v10) != 0 && v10 != 0)
                kotor_fastcall_005df470(input(), nullptr, id, c);
        if (int_entry(&row, "Repeatable", &v10) != 0 && v10 != 0) {         // 005F07FE..005F085C
            std::int32_t wait = 0, rate = 0;
            bool ok;
            {
                SS wait_column("RepeatWait");                               // 005F0862
                ok = kotor_fastcall_00414110(table(), nullptr, &row, &wait_column, &wait) != 0; // 005F086B..005F08A2
                if (ok) ok = int_entry(&row, "RepeatRate", &rate) != 0;     // 005F08A4..005F08F7
            }                                                               // 005F08FF..005F090B
            if (ok) kotor_fastcall_005df490(input(), nullptr, id, wait, rate); // 005F0914..005F092B
        }
        if (!(int_entry(&row, "Scale", &v10) != 0 && v10 != 0)) continue;   // 005F0930..005F098E
        float magnitude = 0.0f, exponent = 0.0f;
        bool ok;
        {
            SS mag_column("ScaleMag");                                      // 005F0994
            ok = kotor_fastcall_00413fa0(table(), nullptr, &row, &mag_column, &magnitude) != 0; // 005F099D..005F09D3
            if (ok) ok = float_entry(&row, "ScaleExp", &exponent) != 0;     // 005F09D5..005F0A28
        }                                                                   // 005F0A30..005F0A3C
        if (ok) kotor_fastcall_005df480(input(), nullptr, id, magnitude, exponent); // 005F0A45..005F0A5C
    }
    // 005F0A6E..005F0AB3: destroy +B0h, +44h, +18h, then CExoIni (C++ scope exit).
}
}
