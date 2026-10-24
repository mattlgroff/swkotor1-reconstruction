#pragma once
// Lane gui-load: CResGFF typed field readers on the main-menu layout path.
// Native x86 ABI veneers: ECX = CResGFF receiver, EDX unused, original stack
// arguments in original order, callee cleanup (fastcall @N == thiscall RET N-8).
// Class-returning originals take the MSVC hidden return pointer as the first
// stack argument; it is spelled explicitly here and returned in EAX.
#include "0088952307f9-gff_menu_lists.h"   // task17 GetFieldByLabel/GetList/count/element over q05 views
#include "75d9d34323f7-startup_string.h"   // M40 stage 8-byte CExoString storage
#include "d8f703239b61-resources.h"        // M40 stage menu_resources::Ref (16-byte CResRef) / Manager
#include <cstdint>

namespace gui_load {
using Word = std::uint32_t;
using Gff = kotor_q05::CResGFF;
using Struct = kotor_q05::StructHandle;
using Field = kotor_q05::FieldRecord;
using String = kotor_reconstruction::StartupString;
// Observed 16-byte CResRef storage (00405EF0 writes exactly 16 bytes); M40 stage type.
using ResRef = menu_resources::Ref;
// Observed 12-byte Vector (004122A0 copies three dwords).
struct Vector { float x, y, z; };
static_assert(sizeof(ResRef) == 16 && sizeof(Vector) == 12, "observed x86 value sizes");
// Original GFF field type codes compared at the cited CMP instructions.
enum : Word { kByte = 0, kDword = 4, kInt = 5, kFloat = 8, kString = 10, kResRef = 11,
              kStruct = 14, kVector = 17 };
}

extern "C" {
// 00405EF0 CResRef::CResRef(char const*, unsigned char) RET 8.
__declspec(noinline) gui_load::ResRef* __fastcall gui_load_resref_bytes_00405ef0(
    gui_load::ResRef* self, void* unused_edx, const char* source, gui_load::Word length_word);
// 00405FB0 CResRef::CopyToString(char* out) RET 4: 4 dwords then NUL at out+16.
__declspec(noinline) void __fastcall gui_load_resref_copy_to_string_00405fb0(
    const gui_load::ResRef* self, void* unused_edx, char* out);
// 00411A10 CResGFF::GetStructFromStruct(CResStruct* out, CResStruct* in, char* label) RET 0Ch.
__declspec(noinline) gui_load::Word __fastcall gui_load_gff_struct_00411a10(
    gui_load::Gff* self, void* unused_edx, gui_load::Struct* out, gui_load::Struct* in, const char* label);
// 00411A60 ReadFieldBYTE(CResStruct*, char*, int* success, BYTE default) RET 10h.
__declspec(noinline) unsigned char __fastcall gui_load_gff_byte_00411a60(
    gui_load::Gff* self, void* unused_edx, gui_load::Struct* s, const char* label,
    gui_load::Word* success, unsigned char fallback);
// 00411C20 ReadFieldDWORD RET 10h.
__declspec(noinline) gui_load::Word __fastcall gui_load_gff_dword_00411c20(
    gui_load::Gff* self, void* unused_edx, gui_load::Struct* s, const char* label,
    gui_load::Word* success, gui_load::Word fallback);
// 00411C90 ReadFieldINT RET 10h.
__declspec(noinline) std::int32_t __fastcall gui_load_gff_int_00411c90(
    gui_load::Gff* self, void* unused_edx, gui_load::Struct* s, const char* label,
    gui_load::Word* success, std::int32_t fallback);
// 00411D00 ReadFieldFLOAT RET 10h; result in ST0.
__declspec(noinline) float __fastcall gui_load_gff_float_00411d00(
    gui_load::Gff* self, void* unused_edx, gui_load::Struct* s, const char* label,
    gui_load::Word* success, float fallback);
// 00411E10 ReadFieldCResRef(hidden CResRef*, CResStruct*, char*, int*, CResRef const&) RET 14h.
__declspec(noinline) gui_load::ResRef* __fastcall gui_load_gff_resref_00411e10(
    gui_load::Gff* self, void* unused_edx, gui_load::ResRef* out, gui_load::Struct* s,
    const char* label, gui_load::Word* success, const gui_load::ResRef* fallback);
// 00411EC0 ReadFieldCExoString(hidden CExoString*, CResStruct*, char*, int*, CExoString const&) RET 14h.
__declspec(noinline) gui_load::String* __fastcall gui_load_gff_string_00411ec0(
    gui_load::Gff* self, void* unused_edx, gui_load::String* out, gui_load::Struct* s,
    const char* label, gui_load::Word* success, const gui_load::String* fallback);
// 004122A0 ReadFieldVector(hidden Vector*, CResStruct*, char*, int*, Vector const&) RET 14h.
__declspec(noinline) gui_load::Vector* __fastcall gui_load_gff_vector_004122a0(
    gui_load::Gff* self, void* unused_edx, gui_load::Vector* out, gui_load::Struct* s,
    const char* label, gui_load::Word* success, const gui_load::Vector* fallback);

// ---- external dependencies (not authored here) ----
// 005E5B00 CExoString copy constructor into raw storage (M40: window-native-strings.obj).
gui_load::String* __fastcall WindowCopy005e5b00(gui_load::String*, void*, const gui_load::String* volatile);
// 005E5B70 CExoString::CExoString(char const*, int): single provider lane-resman (D18).
gui_load::String* __fastcall resman_string_counted_005e5b70(gui_load::String*, void*, const char*, int);
}
