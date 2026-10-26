// 0886-gates (Claude): faithful bodies for former required(0x...) gates of the 0886 cohort unit on the local-variable
// load path (local_variables_native.inc NativeLocalVariables ports).
// Transcribed from the original swkotor.exe (read-only); addresses in comments are original instructions.
// ABI: original __thiscall members are exported as extern "C" __fastcall(self, unused_edx, stack args...):
//   0059A6F0 CSWSScriptVarTable::SetInt(CExoString* name, int value, int flag)          RET 0Ch
//   0059A920 CSWSScriptVarTable::SetLocation(CExoString* name, CScriptLocation value)   RET 1Ch (location by value)
//   004CA800 CScriptLocation::LoadLocation(CResGFF* gff, CResStruct* structure)         RET 8
// .rdata bound to proven values: [0074A110] = "NW_JOURNAL", [00745DF0/E0/D0] = "OrientationX/Y/Z",
//   [0074423C/30/24] = "PositionX/Y/Z" (C string literals).
// EH: SetInt / SetLocation register unwind frames that only destroy their CExoString temporary / by-value location;
// the bound callees do not throw in normal operation, so that cleanup is not reproduced (normal path keeps it).
// Quirk kept (004CA800): the GFF status out-parameter is the stack slot of the structure argument (dead after load).
#include <cstdint>
#include <cstring>

namespace {
using U = std::uint32_t;
inline void* ptr(U a) { return reinterpret_cast<void*>(a); }
inline U word(const void* p) { return reinterpret_cast<U>(p); }
inline U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
inline void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
inline void wf(U a, float v) { std::memcpy(ptr(a), &v, 4); }
}

namespace kotor_reconstruction {
struct StartupString { int __thiscall BorrowedEquals005e5290(const char* text); };     // 005E5290 CExoString::operator==(char const*)
}
namespace base_ini_reconstruction {
kotor_reconstruction::StartupString* __fastcall left_005e5f70(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out, int count);                             // 005E5F70 CExoString::Left
}
namespace startup_rand {
kotor_reconstruction::StartupString* __fastcall SubString005e6270(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out, int start, int count);                  // 005E6270 CExoString::SubString
}

extern "C" {
// ---------------- existing providers (c14 map, exact decorated arity) ----------------
void* __fastcall recovered_var_match_0059a390(void* table, void*, void* name, U type, U create); // 0059A390 MatchIndex (w2_entry_native)
int __fastcall async_string_length_005e5790(void* string, void*);               // 005E5790 CExoString::GetLength (0188)
char __fastcall c2da_current_string_index_005e5600(void* string, void*, int index); // 005E5600 CExoString::operator[] (0376)
void __fastcall frame_LogStringDtor_005e5c20(void* string, void*);              // 005E5C20 ~CExoString (0459)
void* __fastcall kotor_fastcall_004edb00(void* creature, void*);                // 004EDB00 CSWSCreature::GetJournal
void __fastcall kotor_fastcall_005c5a40(void* journal, void*, void* tag, U state, U flag); // 005C5A40 CSWSJournal::SetState RET 0Ch
void __fastcall kotor_fastcall_005c63f0(void* journal, void*, void* tag, U date, U flag);  // 005C63F0 CSWSJournal::SetDate RET 0Ch
void __fastcall kotor_fastcall_005c6650(void* journal, void*, void* tag, U time, U flag);  // 005C6650 CSWSJournal::SetTime RET 0Ch
void __fastcall gui_scene_void_return_0c_005c5630(void*, void*, unsigned, unsigned, unsigned); // 005C5630 void_return RET 0Ch (0464)
void __fastcall kotor_m15e_slot090_void0_004015a0(void*, void*);                // 004015A0 ~CScriptLocation (empty, 0832)
float __fastcall gui_load_gff_float_00411d00(void* gff, void*, void* structure, const char* label, unsigned* status, float fallback); // 00411D00 (0292)

// ---------------- bodies defined here ----------------
void __fastcall kotor_fastcall_0059a6f0(void* table, void*, void* name, U value, U flag);
struct Claude0886Location { U d[6]; };                                            // CScriptLocation (position, orientation)
void __fastcall kotor_fastcall_0059a920(void* table, void*, void* name, Claude0886Location value);
void __fastcall kotor_fastcall_004ca800(void* location, void*, void* gff, void* structure);
}

namespace {
using Str = kotor_reconstruction::StartupString;
}

// ===================================================================================== 0059A6F0
// SetInt: stores the value in the (created) integer entry; for an owner creature (+0C) with a journal, names
// "NW_JOURNAL" + 'E'/'D'/'T'/'P' at [11] forward the value to the journal entry named by the tail.
void __fastcall kotor_fastcall_0059a6f0(void* table, void*, void* name, U value, U flag) {
    const U self = word(table);
    wr(word(recovered_var_match_0059a390(table, nullptr, name, 1, 1)) + 0xc, value); // 0059A70A..0059A71F
    if (rd(self + 0xc) == 0) return;                                             // 0059A722..0059A727
    if (async_string_length_005e5790(name, nullptr) <= 10) return;               // 0059A72D..0059A737 (jle)
    if (kotor_fastcall_004edb00(ptr(rd(self + 0xc)), nullptr) == nullptr) return; // 0059A73D..0059A747
    unsigned char left[8];                                                       // [esp+10h] CExoString
    Str* const prefix = base_ini_reconstruction::left_005e5f70(static_cast<Str*>(name), nullptr,
                                                               reinterpret_cast<Str*>(left), 10); // 0059A74E..0059A757
    const int same = prefix->BorrowedEquals005e5290("NW_JOURNAL");              // 0059A75C..0059A76B ([0074A110])
    frame_LogStringDtor_005e5c20(left, nullptr);                                 // 0059A770..0059A77E
    if (same == 0) return;                                                       // 0059A783..0059A786
    unsigned char tail[8];                                                       // [esp+0Ch] / [esp+14h] CExoString
    const char kind = c2da_current_string_index_005e5600(name, nullptr, 11);     // 0059A78C..0059A795
    if (kind == 'E') {
        void* const tag = startup_rand::SubString005e6270(static_cast<Str*>(name), nullptr, reinterpret_cast<Str*>(tail), 16, 0x80); // 0059A799..0059A7A7
        kotor_fastcall_005c5a40(kotor_fastcall_004edb00(ptr(rd(self + 0xc)), nullptr), nullptr, tag, value, flag); // 0059A7AC..0059A7C5
    } else if (c2da_current_string_index_005e5600(name, nullptr, 11) == 'D') {    // 0059A7D3..0059A7E0
        void* const tag = startup_rand::SubString005e6270(static_cast<Str*>(name), nullptr, reinterpret_cast<Str*>(tail), 15, 0x80); // 0059A7E2..0059A7EE
        kotor_fastcall_005c63f0(kotor_fastcall_004edb00(ptr(rd(self + 0xc)), nullptr), nullptr, tag, value, flag); // 0059A7F3..0059A80C
    } else if (c2da_current_string_index_005e5600(name, nullptr, 11) == 'T') {    // 0059A81A..0059A825
        void* const tag = startup_rand::SubString005e6270(static_cast<Str*>(name), nullptr, reinterpret_cast<Str*>(tail), 15, 0x80); // 0059A827..0059A833
        kotor_fastcall_005c6650(kotor_fastcall_004edb00(ptr(rd(self + 0xc)), nullptr), nullptr, tag, value, flag); // 0059A838..0059A851
    } else if (c2da_current_string_index_005e5600(name, nullptr, 11) == 'P') {    // 0059A85C..0059A865
        void* const tag = startup_rand::SubString005e6270(static_cast<Str*>(name), nullptr, reinterpret_cast<Str*>(tail), 18, 0x80); // 0059A867..0059A875
        gui_scene_void_return_0c_005c5630(kotor_fastcall_004edb00(ptr(rd(self + 0xc)), nullptr), nullptr,
                                          word(tag), value, flag);               // 0059A87A..0059A893
    } else {
        return;                                                                  // 0059A865 -> 0059A8A9
    }
    frame_LogStringDtor_005e5c20(tail, nullptr);                                 // 0059A89C..0059A8A4
}

// ===================================================================================== 0059A920
// SetLocation: copies the six location dwords into the (created) type-5 entry's location (+0C pointer), then runs
// the empty by-value CScriptLocation destructor.
void __fastcall kotor_fastcall_0059a920(void* table, void*, void* name, Claude0886Location value) {
    const U entry = word(recovered_var_match_0059a390(table, nullptr, name, 5, 1)); // 0059A937..0059A948
    const U target = rd(entry + 0xc);                                            // 0059A94D
    for (U i = 0; i < 6; ++i) wr(target + 4 * i, value.d[i]);                   // 0059A950..0059A959 rep movsd
    kotor_m15e_slot090_void0_004015a0(&value, nullptr);                          // 0059A95B..0059A967
}

// ===================================================================================== 004CA800
// LoadLocation: orientation (+0C..+14) then position (+0..+8), each ReadFieldFLOAT with default 0.0f.
void __fastcall kotor_fastcall_004ca800(void* location, void*, void* gff, void* structure) {
    const U self = word(location);
    unsigned status = 0;                                                         // [esp+24h] (structure argument slot)
    const float ox = gui_load_gff_float_00411d00(gff, nullptr, structure, "OrientationX", &status, 0.0f); // 004CA80E..004CA824
    const float oy = gui_load_gff_float_00411d00(gff, nullptr, structure, "OrientationY", &status, 0.0f); // 004CA828..004CA83C
    const float oz = gui_load_gff_float_00411d00(gff, nullptr, structure, "OrientationZ", &status, 0.0f); // 004CA840..004CA854
    wf(self + 0xc, ox);                                                          // 004CA858..004CA863
    wf(self + 0x10, oy);                                                         // 004CA869
    wf(self + 0x14, oz);                                                         // 004CA878
    const float px = gui_load_gff_float_00411d00(gff, nullptr, structure, "PositionX", &status, 0.0f); // 004CA86C..004CA883
    const float py = gui_load_gff_float_00411d00(gff, nullptr, structure, "PositionY", &status, 0.0f); // 004CA887..004CA89B
    const float pz = gui_load_gff_float_00411d00(gff, nullptr, structure, "PositionZ", &status, 0.0f); // 004CA89F..004CA8B3
    wf(self, px);                                                                // 004CA8C3
    wf(self + 4, py);                                                            // 004CA8C6
    wf(self + 8, pz);                                                            // 004CA8CA
}

// =====================================================================================================
//   004059A0 CExoArrayList<CExoString>::Add(CExoString value) RET 8 -- exported with the original by-value ABI
//   (@16: the 8-byte CExoString occupies the two argument dwords; the callee owns and destroys it).
//   abi-a1: this is the selected by-value export for these 004059A0 callers. The former 0705 twin @kotor_fastcall_004059a0@12 (pointer
//   argument, RET 4) is removed; its two callers (00405D10, 004B71E0) call this export with the original ABI.
extern "C" {
void __fastcall kotor_fastcall_00405080(void* list, void*, int capacity);       // 00405080 CExoArrayList<CExoString>::Allocate (0705)
void* __fastcall async_assign_string_005e5c50(void* self, void*, const void* other); // 005E5C50 CExoString::operator= (0199)
struct Claude0886ExoString { U data, length; };
void __fastcall claude_exo_string_list_add_004059a0(void* list, void*, Claude0886ExoString value);
}
void __fastcall claude_exo_string_list_add_004059a0(void* list, void*, Claude0886ExoString value) {
    const U s = word(list);
    const U capacity = rd(s + 8);                                                // 004059B8
    if (rd(s + 4) == capacity)                                                   // 004059BB..004059C6
        kotor_fastcall_00405080(list, nullptr, capacity != 0 ? static_cast<int>(capacity + capacity) : 0x10); // 004059C8..004059D8
    const U index = rd(s + 4);                                                   // 004059DD
    const U slot = rd(s) + index * 8;                                            // 004059E0..004059E2
    wr(s + 4, index + 1);                                                        // 004059E9..004059EB
    (void)async_assign_string_005e5c50(ptr(slot), nullptr, &value);              // 004059E5..004059EE
    frame_LogStringDtor_005e5c20(&value, nullptr);                               // 004059F3..004059FF (by-value argument)
}
