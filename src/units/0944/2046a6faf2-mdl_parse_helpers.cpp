// mdl_parse_helpers.cpp - lane-m48-asciimdl fork B: ASCII .mdl parse helpers of the GOG swkotor.exe (image base
// 0x400000): Parse<T> list readers, element parsers, controller builders (AddGenericController*, AddBezierController),
// key culling / quaternion compression, FindMdlNode and the CExoArrayList helpers they use.
// Evidence: complete per-function listings tools/dB/<ADDR>.txt (dumpbin over every Ghidra ADDRESS_RANGE; NOTE the
// lane's d/<ADDR>.txt listings drop instructions after some calls and were not used here); constants via tools/pe.py.
// Conventions: see asciimdl.hpp (D1 __fastcall for __thiscall, raw observed-offset storage, live_operator_new /
// live_free, D30-FINAL double intermediates, constant-initialised cells only).
// EH: 0043F5E0 / 004450D0 (state 0: free the new block if an element constructor throws), 00464B50 / 00465090 /
// 00465320 (state 0: free the local key list), 004647F0 (state 0: destroy the by-value argument) only run when a
// callee throws; no callee here throws (live_* , the list helpers, sscanf/sprintf providers, element constructors),
// so the unwind funclets are cited, not emitted (D30-FINAL /EHsc rule).
// x87 float copies (fld dword / fstp dword of the same float, e.g. 00462A19..00462A1F, 004651A6..004651AC) are
// reproduced as float copies: value-preserving except that x87 would quiet a signalling NaN (recorded, as f3_mdl_node).
#include "d51e09c5f8-asciimdl.hpp"
#include "f078693989-scene3d.h"   // kotor_scene3d::Vector {x,y,z} / Quaternion {w,x,y,z} (lane-3d-render canonical header)
#include <cmath>
#include <climits>
#include "../../math/kotor_x87.h"   // FloatToInt: the original's inline-asm frndint/fistp helper (L1, PLAN-v3 6.12)

using namespace aurora_model;
using kotor_scene3d::Vector;
using kotor_scene3d::Quaternion;

namespace asciimdl_b {
// MaxFace_PostProcess (20-byte element, 0x14 stride at 00468A82 / 004647F0 rep movs 5): opaque words; passed by
// value to 004647F0 (RET 14h) - D1 by-value struct argument (D1-amendment: permitted for arguments).
struct MaxFacePostProcess { U words[5]; };
static_assert(sizeof(MaxFacePostProcess) == 0x14, "observed element size");
}

extern "C" {
// ===================================================================== existing providers (M47 objects)
void __fastcall exo_array_list_allocate16_00437c50(List* self, void* unused_edx, I capacity);              // m47-anim-lookat (RET 4)
void __fastcall quaternion_list_allocate_00437cc0(List* list, void* unused_edx, I capacity);               // 3d-anim-anim-contested (RET 4)
void __fastcall aurora_skin_weight_list_allocate_0043f6b0(List* self, void* unused_edx, I capacity);       // model46-f3-mdl-node (RET 4)
Quaternion* __fastcall quaternion_construct_004ac960(Quaternion* self, void* unused_edx, Vector axis, float angle); // model46-f2-math (RET 10h)
Quaternion* __cdecl quaternion_scalar_multiply_004a9b80(Quaternion* out, const Quaternion* q, float s);     // 3d-anim-anim-contested
void __fastcall aurora_maxface_destruct_0043e140(void* self, void* unused_edx);                             // 3d-model-model-destroy
void __fastcall aurora_maxface_postprocess_destroy_0043e0f0(void* self, void* unused_edx);                  // 3d-model-model-lifetime
extern volatile I aurora_face_count_007a8a00;                                                               // 3d-model-model-destroy
int __cdecl format_006fadb0(char* buffer, const char* format, ...);                                         // tpc-joins (sprintf 006FADB0)

// ===================================================================== cells owned here (no other reference in the image;
// raw-image absolute-reference scan: 0078E58C x8 in 00463F80, 0078E590 004650FD, 0078E588 00465121, 007FBEF8..007FBF04
// in 00463B30, 007FBF08..007FBF14 in 00464E40, 007FBAE8 in 0046507E/0046528C/004654DB)
volatile float mdl_key_epsilon_0078e58c = 0x1.A36E2Ep-14f;   // .data 38D1B717 (1e-4f) - CullUnnecessaryKeys threshold
volatile I mdl_cull_keys_0078e590 = 1;                       // .data 00000001 - AddGenericController cull switch
volatile I mdl_compress_quaternions_0078e588 = 1;            // .data 00000001 - AddGenericController compress switch
volatile I mdl_controller_values_007fbae8 = 0;               // .bss  - running count of parsed controller rows/keys
volatile U mdl_key_axis_007fbef8[3] = {};                    // .bss  - function-static Vector of 00463B30
volatile U mdl_key_axis_guard_007fbf04 = 0;                  // .bss  - its init guard (bit 0)
volatile U mdl_single_axis_007fbf08[3] = {};                 // .bss  - function-static Vector of 00464E40
volatile U mdl_single_axis_guard_007fbf14 = 0;               // .bss  - its init guard (bit 0)

// ===================================================================== cluster-internal bodies authored here
void* __fastcall maxface_postprocess_construct_0043e0d0(void* self, void* unused_edx);                      // RET 0, EAX=this
void* __fastcall maxface_construct_0043f710(void* self, void* unused_edx);                                  // RET 0, EAX=this
void __fastcall maxface_postprocess_list_allocate_0043f5e0(List* self, void* unused_edx, I capacity);      // RET 4
void __fastcall maxface_postprocess_list_set_size_00444f10(List* self, void* unused_edx, I size);          // RET 4
void __fastcall maxface_list_allocate_004450d0(List* self, void* unused_edx, I capacity);                  // RET 4
void __cdecl mdl_parse_maxface_postprocess_00463710(char* text, void* face);
void __cdecl mdl_parse_skin_weight_004637a0(char* text, void* weight);
void __cdecl mdl_parse_uint_array_00463850(char* text, U* out);
void __fastcall aur_generic_key_list_delete_at_004638b0(List* self, void* unused_edx, I index);           // RET 4
void __fastcall aur_generic_key_list_allocate_00463950(List* self, void* unused_edx, I capacity);         // RET 4
void __cdecl mdl_parse_aur_generic_key_00463b30(char* text, void* key);
void __cdecl mdl_cull_unnecessary_keys_00463f80(List* keys);
List* __fastcall aur_generic_key_list_assign_004643b0(List* self, void* unused_edx, const List* source);   // RET 4
void __cdecl mdl_parse_aur_generic_key_list_00464440(char* text, List* list);
void __fastcall maxface_postprocess_list_add_004647f0(List* self, void* unused_edx, asciimdl_b::MaxFacePostProcess value); // RET 14h
void __cdecl mdl_compress_quaternion_keys_00464b50(List* keys);
}

namespace menu_tables_m41 {
// menu-tables-d1-bodies: `eh vector constructor iterator' 006FB55F (identical declaration -> identical mangled name)
void __stdcall VectorConstruct_006fb55f(void*, unsigned, unsigned, void* (__fastcall*)(void*, void*), void (__fastcall*)(void*, void*));
}

namespace {
// .rdata strings (read from the pinned image)
constexpr char kCountChar_0073ee54[] = "%d %c";
constexpr char kEndlist_0073ee4c[] = "endlist";
constexpr char kD_0073d720[] = "%d";
constexpr char kS_0073d5d0[] = "%s";
constexpr char kD8_00741e30[] = "%d %d %d %d %d %d %d %d";
constexpr char kSkin_00741e48[] = "%s %f %s %f %s %f %s %f";
constexpr char kRoot_00741e60[] = "ROOT";
constexpr char kF4_0073d5dc[] = "%f %f %f %f";
constexpr char kF10_00741eb4[] = "%f %f %f %f %f %f %f %f %f %f";
constexpr char kAabb_00741ed4[] = "%f %f %f %f %f %f %d %d";
// .rdata floats
constexpr float kZero_0073d700 = 0.0f;
constexpr float kOne_0073d764 = 1.0f;
constexpr float kAabbPad_0073dfec = 0x1.47AE14p-7f;     // 3C23D70A (0.01f) - only feeds dead locals, see 00463C20
constexpr float kZRange_00741f10 = 1022.0f, kZScale_00741f14 = 511.0f;
constexpr float kXYRange_00741f18 = 2046.0f, kXYScale_00741f1c = 1023.0f;
constexpr U kXYClamp = 0x44FFC000u;                     // 2046.0f immediate (00464C9A / 00464CF7)
constexpr U kZClamp = 0x447F8000u;                      // 1022.0f immediate (00464D54)

void* vp(U a) { return ptr<void>(a); }
U elem(const List* l, I i, U size) { return l->data + static_cast<U>(i) * size; }
I grow_or(I capacity, I first) { return capacity != 0 ? capacity * 2 : first; }   // test; je; add eax,eax / mov eax,N
float fval(U a, U off) { return get<float>(vp(a), off); }

// Inline blank skip (e.g. 00464567..00464572): `if (p) while (*p == ' ' || *p == '\t') ++p;`
char* skip_inline(char* p) {
    if (p != nullptr)
        while (*reinterpret_cast<volatile char*>(p) == ' ' || *reinterpret_cast<volatile char*>(p) == '\t') ++p;
    return p;
}
// Parse<T> tail (e.g. 004645F4..00464621): skip blanks of the last line, then of (that + 7); results discarded.
// When the stream ended without "endlist" the line is NULL and the second skip reads address 7: the original faults
// there (004645FA..00464605); the same volatile read is reproduced (recorded review item, malformed input only).
void tail_inline(char* line) {
    U q = addr(skip_inline(line)) + 7u;                                  // add ecx,7
    if (q != 0)                                                          // test ecx,ecx; je
        while (*ptr<volatile char>(q) == ' ' || *ptr<volatile char>(q) == '\t') ++q;
}
// Same tail through the called provider (004628C0 form 00462A54..00462A5E): skip_blanks_00463510 twice.
void tail_called(char* line) {
    skip_blanks_00463510(ptr<char>(addr(skip_blanks_00463510(line)) + 7u));
}
// %s result used as a copy source when sscanf returns 0 (00464910..00464918, 004649E8..004649F0): unreachable
// (%s yields 1 or EOF); kept for fidelity exactly as lane-m48-a anim_parse.cpp copy_from_result.
void copy_from_result(char* destination, int result) {
    const char* source = reinterpret_cast<const char*>(static_cast<std::intptr_t>(result));
    for (I i = 0;; ++i) { const char c = source[i]; destination[i] = c; if (c == '\0') break; }
}
// Binary payload of a Parse<T> list ("%d %c" matched both): count*size bytes from AurResGetData copied over the
// list data (rep movs dword + byte), then *text = 0 (e.g. 0046296C..00462993).
void copy_binary(char* text, List* list, U bytes) {
    const void* const source = resource_read_0044bfd0(bytes);
    std::memcpy(vp(list->data), source, bytes);
    *text = '\0';
}
// Inline count-header resize (e.g. 004628F5..00462930): n == 0 -> free data (data = 0) and capacity = 0, else
// Allocate(n); then size = n.
template <class Alloc> void header_resize(List* list, I n, Alloc allocate) {
    if (n == 0) {
        if (list->data != 0) { live_free(vp(list->data)); list->data = static_cast<U>(n); }
        list->capacity = 0;
    } else {
        allocate(list, n);
    }
    list->size = n;
}
// Inline CExoArrayList<float/DWORD>::Add growth (e.g. 00464F4F..00464F92): capacity = cap ? 2*cap : 8; new block;
// copy `size` dwords (live size); free the old block when non-null.
void grow_dwords_inline(List* list) {
    const I capacity = grow_or(list->capacity, 8);
    const U old = list->data;                                           // mov edi,[esi]
    list->capacity = capacity;                                          // mov [esi+8],eax
    list->data = addr(live_operator_new(static_cast<U>(capacity) * 4u)); // shl eax,2; call 006FA7E6; mov [esi],eax
    for (I i = 0; i < list->size; ++i) put<U>(vp(list->data), static_cast<U>(i) * 4u, get<U>(vp(old), static_cast<U>(i) * 4u));
    if (old != 0) live_free(vp(old));
}
}

extern "C" {
// ===================================================================== small constructors
// 0043E0D0 MaxFace_PostProcess::MaxFace_PostProcess: [0]=[4]=[10]=[8]=[C]=0; EAX = this (0043E0D0..0043E0E2).
void* __fastcall maxface_postprocess_construct_0043e0d0(void* self, void*) {
    put<U>(self, 0x0, 0); put<U>(self, 0x4, 0); put<U>(self, 0x10, 0); put<U>(self, 0x8, 0); put<U>(self, 0xC, 0);
    return self;
}
// 0043F710 MaxFace::MaxFace: [0]=[4]=[8]=0, ++face count 007A8A00, [10]=0; EAX = this (0043F710..0043F725).
void* __fastcall maxface_construct_0043f710(void* self, void*) {
    put<U>(self, 0x0, 0); put<U>(self, 0x4, 0); put<U>(self, 0x8, 0);
    aurora_face_count_007a8a00 = aurora_face_count_007a8a00 + 1;                     // 0043F71C inc dword [007A8A00]
    put<U>(self, 0x10, 0);
    return self;
}

// ===================================================================== CExoArrayList<MaxFace_PostProcess> (0x14)
// 0043F5E0 allocate(capacity): capacity stored; block = new(cap*14h+4); block[0] = cap and the eh vector
// constructor iterator (006FB55F: 14h, cap, ctor 0043E0D0, dtor 0043E0F0) builds the elements; data = block+4 (0 when
// new returned NULL); copy `size` old elements (5 dwords); old block: eh vector destructor iterator (006FA852,
// 14h, old[-1], 0043E0F0) then free(old-4). NOTE (original behaviour): the destructor frees the buffers the copied
// elements still point at.
void __fastcall maxface_postprocess_list_allocate_0043f5e0(List* self, void*, I capacity) {
    const U old = self->data;                                                       // 0043F5FE ebp = [ebx]
    self->capacity = capacity;                                                      // 0043F60C
    void* const block = live_operator_new(static_cast<U>(capacity) * 0x14u + 4u);  // 0043F600..0043F60F
    U data = 0;                                                                     // 0043F61B edi = 0
    if (block != nullptr) {                                                         // 0043F61D..0043F623
        data = addr(block) + 4u;                                                    // 0043F630
        put<I>(block, 0, capacity);                                                 // 0043F636
        menu_tables_m41::VectorConstruct_006fb55f(vp(data), 0x14u, static_cast<unsigned>(capacity),
            &maxface_postprocess_construct_0043e0d0, &aurora_maxface_postprocess_destroy_0043e0f0);   // 0043F625..0043F638
    }
    self->data = data;                                                              // 0043F64C
    for (I i = 0; i < self->size; ++i)                                              // 0043F640..0043F669 (live size)
        std::memcpy(vp(self->data + static_cast<U>(i) * 0x14u), vp(old + static_cast<U>(i) * 0x14u), 0x14);
    if (old != 0) {                                                                 // 0043F66B..0043F66D
        c2da_destroy_array_006fa852(vp(old), 0x14, get<I>(vp(old - 4u), 0), &aurora_maxface_postprocess_destroy_0043e0f0); // 0043F66F..0043F67E
        live_free(vp(old - 4u));                                                    // 0043F677 / 0043F683..0043F684
    }
}
// 00444F10 SetSize(n): n == 0 -> destroy + free the block (data = 0), size = 0, capacity = 0; else allocate(n) and
// size = n.
void __fastcall maxface_postprocess_list_set_size_00444f10(List* self, void*, I size) {
    if (size == 0) {                                                                // 00444F15..00444F1A
        const U data = self->data;
        if (data != 0) {                                                            // 00444F1C..00444F20
            c2da_destroy_array_006fa852(vp(data), 0x14, get<I>(vp(data - 4u), 0), &aurora_maxface_postprocess_destroy_0043e0f0); // 00444F22..00444F32
            live_free(vp(data - 4u));                                               // 00444F37..00444F38
            self->data = static_cast<U>(size);                                      // 00444F40 mov [esi],ebx (0)
        }
        self->size = size;                                                          // 00444F43
        self->capacity = 0;                                                         // 00444F46
        return;
    }
    maxface_postprocess_list_allocate_0043f5e0(self, nullptr, size);                // 00444F52..00444F55
    self->size = size;                                                              // 00444F5A
}
// 004647F0 Add(MaxFace_PostProcess by value) RET 14h: grow to cap ? 2*cap : 8 (0043F5E0) when full; data[size] =
// argument (5 dwords); ++size; then the callee-owned argument is destroyed (0043E0F0 on [esp+1Ch]).
void __fastcall maxface_postprocess_list_add_004647f0(List* self, void*, asciimdl_b::MaxFacePostProcess value) {
    if (self->size == self->capacity)                                               // 0046480A..00464818
        maxface_postprocess_list_allocate_0043f5e0(self, nullptr, grow_or(self->capacity, 8)); // 0046481A..0046482A
    std::memcpy(vp(elem(self, self->size, 0x14)), &value, 0x14);                    // 0046482F..00464843
    self->size = self->size + 1;                                                    // 00464845..0046484D
    aurora_maxface_postprocess_destroy_0043e0f0(&value, nullptr);                   // 00464849 / 00464858
}

// ===================================================================== CExoArrayList<MaxFace> (0x20)
// 004450D0 allocate(capacity): as 0043F5E0 with 20h elements, ctor 0043F710, dtor 0043E140 (8-dword copies).
void __fastcall maxface_list_allocate_004450d0(List* self, void*, I capacity) {
    const U old = self->data;                                                       // 004450F3
    self->capacity = capacity;                                                      // 004450FA
    void* const block = live_operator_new(static_cast<U>(capacity) * 0x20u + 4u);  // 004450EC..004450FD
    U data = 0;                                                                     // 00445109
    if (block != nullptr) {                                                         // 0044510B..00445111
        data = addr(block) + 4u;                                                    // 0044511E
        put<I>(block, 0, capacity);                                                 // 00445124
        menu_tables_m41::VectorConstruct_006fb55f(vp(data), 0x20u, static_cast<unsigned>(capacity),
            &maxface_construct_0043f710, &aurora_maxface_destruct_0043e140);       // 00445113..00445126
    }
    self->data = data;                                                              // 0044513A
    for (I i = 0; i < self->size; ++i)                                              // 0044512B..00445157
        std::memcpy(vp(self->data + static_cast<U>(i) * 0x20u), vp(old + static_cast<U>(i) * 0x20u), 0x20);
    if (old != 0) {                                                                 // 00445159..0044515B
        c2da_destroy_array_006fa852(vp(old), 0x20, get<I>(vp(old - 4u), 0), &aurora_maxface_destruct_0043e140); // 0044515D..0044516C
        live_free(vp(old - 4u));                                                    // 00445171..00445172
    }
}
// 00464350 SetSize(n) of CExoArrayList<MaxFace> (shared header asciimdl.hpp).
void __fastcall mdl_maxface_list_set_size_00464350(List* self, void*, I size) {
    if (size == 0) {                                                                // 00464355..0046435A
        const U data = self->data;
        if (data != 0) {                                                            // 0046435C..00464360
            c2da_destroy_array_006fa852(vp(data), 0x20, get<I>(vp(data - 4u), 0), &aurora_maxface_destruct_0043e140); // 00464362..00464372
            live_free(vp(data - 4u));                                               // 00464377..00464378
            self->data = static_cast<U>(size);                                      // 00464380
        }
        self->size = size;                                                          // 00464383
        self->capacity = 0;                                                         // 00464386
        return;
    }
    maxface_list_allocate_004450d0(self, nullptr, size);                            // 00464392..00464395
    self->size = size;                                                              // 0046439A
}

// ===================================================================== CExoArrayList<AurGenericKey> (0x2C)
// 004638B0 deleteAt(index): --size; for i = index .. size-1 (live size): data[i] = data[i+1] (11 dwords).
void __fastcall aur_generic_key_list_delete_at_004638b0(List* self, void*, I index) {
    self->size = self->size - 1;                                                    // 004638B8..004638C0
    for (I i = index; i < self->size; ++i)                                          // 004638BE..004638E8
        std::memcpy(vp(elem(self, i, 0x2C)), vp(elem(self, i + 1, 0x2C)), 0x2C);   // rep movs 0Bh
}
// 00463950 allocate(capacity): capacity stored; data = new(cap*2Ch) (no NULL test); copy `size` old elements; free
// the old block when non-null.
void __fastcall aur_generic_key_list_allocate_00463950(List* self, void*, I capacity) {
    self->capacity = capacity;                                                      // 00463957
    const U old = self->data;                                                       // 0046395E
    self->data = addr(live_operator_new(static_cast<U>(capacity) * 0x2Cu));         // 0046395A..00463966
    for (I i = 0; i < self->size; ++i)                                              // 00463968..0046398F (live size)
        std::memcpy(vp(self->data + static_cast<U>(i) * 0x2Cu), vp(old + static_cast<U>(i) * 0x2Cu), 0x2C);
    if (old != 0) live_free(vp(old));                                               // 00463993..00463998
}
// 004643B0 operator=(source): size = 0; for each source element (live source size): copy to a local, grow to
// cap ? 2*cap : 8 (00463950) when full, append. EAX = this.
List* __fastcall aur_generic_key_list_assign_004643b0(List* self, void*, const List* source) {
    self->size = 0;                                                                 // 004643BD
    for (I i = 0; i < source->size; ++i) {                                          // 004643C0..00464432
        unsigned char key[0x2C];
        std::memcpy(key, vp(elem(source, i, 0x2C)), 0x2C);                          // 004643D0..004643E5
        if (self->size == self->capacity)                                           // 004643E7..004643EA
            aur_generic_key_list_allocate_00463950(self, nullptr, grow_or(self->capacity, 8)); // 004643EC..004643FC
        std::memcpy(vp(elem(self, self->size, 0x2C)), key, 0x2C);                   // 00464401..0046441C
        self->size = self->size + 1;                                                // 0046441E
    }
    return self;                                                                    // 00464437
}

// ===================================================================== CExoArrayList<NewController> (0x10)
// 004642B0 operator=(source): as 004643B0 with 16-byte records and growth 00437C50. EAX = this.
List* __fastcall controller_list_assign_004642b0(List* self, void*, const List* source) {
    self->size = 0;                                                                 // 004642BE
    for (I i = 0; i < source->size; ++i) {                                          // 004642C1..0046433F
        U record[4];
        std::memcpy(record, vp(elem(source, i, 0x10)), 0x10);                       // 004642D0..004642EF
        if (self->size == self->capacity)                                           // 004642E3..004642F3
            exo_array_list_allocate16_00437c50(self, nullptr, grow_or(self->capacity, 8)); // 004642F5..00464305
        std::memcpy(vp(elem(self, self->size, 0x10)), record, 0x10);                // 0046430A..00464328
        self->size = self->size + 1;                                                // 0046432B
    }
    return self;                                                                    // 00464343
}

// ===================================================================== element parsers
// 00463710 Parse(MaxFace_PostProcess): free [0], [10], [4] when non-null; [4] = new(30h), [0] = new(0Ch),
// [10] = new(0Ch); sscanf "%d x8" -> [0][0..2], [8], [4][0..2], [C].
void __cdecl mdl_parse_maxface_postprocess_00463710(char* text, void* face) {
    if (get<U>(face, 0x0) != 0) live_free(vp(get<U>(face, 0x0)));                  // 00463715..0046371C
    if (get<U>(face, 0x10) != 0) live_free(vp(get<U>(face, 0x10)));                 // 00463724..0046372C
    if (get<U>(face, 0x4) != 0) live_free(vp(get<U>(face, 0x4)));                   // 00463734..0046373C
    put<U>(face, 0x4, addr(live_operator_new(0x30)));                               // 00463744..0046374D
    put<U>(face, 0x0, addr(live_operator_new(0x0C)));                               // 0046374B..00463757
    put<U>(face, 0x10, addr(live_operator_new(0x0C)));                              // 00463755..00463760
    const U a = get<U>(face, 0x0), b = get<U>(face, 0x4);                           // 0046375E / 00463763
    kotor_sscanf_006fae08(text, kD8_00741e30, ptr<I>(a), ptr<I>(a + 4u), ptr<I>(a + 8u), static_cast<I*>(vp(addr(face) + 8u)),
                          ptr<I>(b), ptr<I>(b + 4u), ptr<I>(b + 8u), static_cast<I*>(vp(addr(face) + 0xCu))); // 00463766..0046378A
}
// 004637A0 Parse_SkinVertexWeight: [0],[4],[8],[C] = new(20h) each; sprintf([4]/[8]/[C], "ROOT") (format string
// "ROOT" itself); [14] = [18] = [1C] = 0; sscanf "%s %f %s %f %s %f %s %f" -> [0],[10],[4],[14],[8],[18],[C],[1C];
// [30] = result / 2 (signed, cdq/sub/sar); [24] = [20] = -1.
void __cdecl mdl_parse_skin_weight_004637a0(char* text, void* weight) {
    put<U>(weight, 0x0, addr(live_operator_new(0x20)));                             // 004637A3..004637B0
    put<U>(weight, 0x4, addr(live_operator_new(0x20)));                             // 004637AE..004637B9
    put<U>(weight, 0x8, addr(live_operator_new(0x20)));                             // 004637B7..004637C3
    put<U>(weight, 0xC, addr(live_operator_new(0x20)));                             // 004637C1..004637CB
    format_006fadb0(ptr<char>(get<U>(weight, 0x4)), kRoot_00741e60);                // 004637CE..004637D7
    put<U>(weight, 0x14, 0);                                                        // 004637E8
    format_006fadb0(ptr<char>(get<U>(weight, 0x8)), kRoot_00741e60);                // 004637DC..004637EE
    put<U>(weight, 0x18, 0);                                                        // 004637FF
    format_006fadb0(ptr<char>(get<U>(weight, 0xC)), kRoot_00741e60);                // 004637F3..00463805
    put<U>(weight, 0x1C, 0);                                                        // 00463814
    const U w = addr(weight);
    const int result = kotor_sscanf_006fae08(text, kSkin_00741e48,
        ptr<char>(get<U>(weight, 0x0)), ptr<float>(w + 0x10u), ptr<char>(get<U>(weight, 0x4)), ptr<float>(w + 0x14u),
        ptr<char>(get<U>(weight, 0x8)), ptr<float>(w + 0x18u), ptr<char>(get<U>(weight, 0xC)), ptr<float>(w + 0x1Cu)); // 0046380A..00463833
    put<I>(weight, 0x24, -1);                                                       // 0046383E / 00463844
    put<I>(weight, 0x20, -1);                                                       // 00463847
    put<I>(weight, 0x30, result / 2);                                               // 00463838..0046384A (cdq; sub; sar 1)
}
// 00463850 Parse_uint(text, out): count from "%d" (local = 0 first); for i < count: next line, "%d" -> out[i].
void __cdecl mdl_parse_uint_array_00463850(char* text, U* out) {
    I count = 0;                                                                    // 00463863 [esp+10h] = 0
    kotor_sscanf_006fae08(text, kD_0073d720, &count);                               // 00463851..00463867
    U* target = out;                                                                // 00463878 edi
    for (I i = 0; i < count; ++i) {                                                 // 00463873..0046389E
        char* const line = resource_next_0044bfa0();                                // 00463880 (no NULL test)
        kotor_sscanf_006fae08(line, kD_0073d720, target);                           // 00463885..0046388C
        ++target;                                                                   // 00463899 add edi,4
    }
}
// 00463A30 Parse_Quaternion(text, out): axis (x,y,z) = 0 then "%f %f %f %f" -> axis, angle. Zero axis -> out =
// identity {1,0,0,0} (raw dwords); otherwise Quaternion(axis, angle) (004AC960) copied to out.
void __cdecl mdl_parse_quaternion_00463a30(char* text, void* out) {
    float axis_x = 0.0f, axis_y = 0.0f, axis_z = 0.0f;                              // 00463A51..00463A61
    float angle = 0.0f;   // D2 (00463A33 [esp+0]): original has no initializing store; read raw at 00463AE7
    kotor_sscanf_006fae08(text, kF4_0073d5dc, &axis_x, &axis_y, &axis_z, &angle);   // 00463A33..00463A69
    if (static_cast<double>(axis_x) == kZero_0073d700 &&                            // 00463A6E..00463A80 (jp: unequal/NaN)
        static_cast<double>(axis_y) == kZero_0073d700 &&                            // 00463A82..00463A91
        static_cast<double>(axis_z) == kZero_0073d700) {                            // 00463A93..00463AA2
        put<U>(out, 0x0, 0x3F800000u);                                              // 00463AA8..00463AB4
        put<U>(out, 0x4, 0);                                                        // 00463AB6..00463AC2
        put<U>(out, 0x8, 0);                                                        // 00463AC5..00463ADD
        put<U>(out, 0xC, 0);                                                        // 00463AD1..00463AE0
        return;
    }
    const Vector axis{axis_x, axis_y, axis_z};                                      // 00463AE7..00463B02 (raw dwords)
    Quaternion q;                                                                   // [esp+14h]
    const Quaternion* const r = quaternion_construct_004ac960(&q, nullptr, axis, angle); // 00463B05..00463B09
    std::memcpy(out, r, 0x10);                                                      // 00463B0E..00463B25 (through EAX)
}
// 00463B30 Parse_AurGenericKey(text, key): "%f x10" -> key[0..24]; key[28] = result. Result 5 (time + axis-angle):
// the function-static Vector 007FBEF8 (guard 007FBF04 bit 0: first use zeroes it) takes key[4..C], Quaternion(V,
// key[10]) (004AC960) and key[4],[8],[C],[10] = q.x, q.y, q.z, q.w.
void __cdecl mdl_parse_aur_generic_key_00463b30(char* text, void* key) {
    const U k = addr(key);
    const int result = kotor_sscanf_006fae08(text, kF10_00741eb4, ptr<float>(k), ptr<float>(k + 4u), ptr<float>(k + 8u),
        ptr<float>(k + 0xCu), ptr<float>(k + 0x10u), ptr<float>(k + 0x14u), ptr<float>(k + 0x18u), ptr<float>(k + 0x1Cu),
        ptr<float>(k + 0x20u), ptr<float>(k + 0x24u));                              // 00463B36..00463B6E
    put<I>(key, 0x28, result);                                                      // 00463B79
    if (result != 5) return;                                                        // 00463B76..00463B7C
    if ((mdl_key_axis_guard_007fbf04 & 1u) == 0) {                                  // 00463B82..00463B8F
        mdl_key_axis_guard_007fbf04 = mdl_key_axis_guard_007fbf04 | 1u;             // 00463B91
        mdl_key_axis_007fbef8[0] = 0; mdl_key_axis_007fbef8[1] = 0; mdl_key_axis_007fbef8[2] = 0; // 00463B97..00463BAB
    }
    mdl_key_axis_007fbef8[0] = get<U>(key, 0x4);                                    // 00463BB9..00463BBB
    mdl_key_axis_007fbef8[1] = get<U>(key, 0x8);                                    // 00463BC0..00463BC3
    mdl_key_axis_007fbef8[2] = get<U>(key, 0xC);                                    // 00463BC9..00463BCB
    const float angle = get<float>(key, 0x10);                                      // 00463BD1 (raw dword push)
    Vector axis;
    const U words[3] = {mdl_key_axis_007fbef8[0], mdl_key_axis_007fbef8[1], mdl_key_axis_007fbef8[2]}; // 00463BD3..00463BEF
    std::memcpy(&axis, words, sizeof(axis));
    Quaternion q;                                                                   // [esp+10h]
    quaternion_construct_004ac960(&q, nullptr, axis, angle);                        // 00463BF2..00463BF6
    put<float>(key, 0x4, q.x);                                                      // 00463BFB / 00463C07
    put<float>(key, 0x8, q.y);                                                      // 00463BFF / 00463C0D
    put<float>(key, 0xC, q.z);                                                      // 00463C03 / 00463C10
    put<float>(key, 0x10, q.w);                                                     // 00463C09 / 00463C12
}
// 00463C20 Parse_AABB(text, slot): recursive AABB tree node. Blanks skipped (text may be NULL, then sscanf gets
// NULL as the original); "%f x6 %d %d" -> min, max, the second %d lands in the TEXT ARGUMENT SLOT (index), the
// last in a local; node = new(28h) {0.., [20] = -1}; *slot = node; [20] = index (the original text pointer when
// fewer than 7 fields converted); [0..14] = min/max. Index -1 (interior): two child lines parsed into [18]/[1C];
// then with exactly 7 fields the split axis bit is computed from the children's centres into [24], else
// [24] = the 8th field.
void __cdecl mdl_parse_aabb_00463c20(char* text, void* out_slot) {
    char* p = text;                                                                 // 00463C20 ecx = text
    if (p != nullptr)                                                               // 00463C2A..00463C2E
        while (*p == ' ' || *p == '\t') ++p;                                        // 00463CD0..00463CDF
    I index = static_cast<I>(addr(text));                                           // [esp+44h] = the text argument slot
    I last = 0;   // D2 (00463C36 [esp+0Ch]): no initializing store; read at 00463F72 when fewer than 8 fields
    float bounds[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};                         // 00463C64..00463C8C ([esp+10h..24h])
    const int fields = kotor_sscanf_006fae08(p, kAabb_00741ed4, &bounds[0], &bounds[1], &bounds[2], &bounds[3],
                                             &bounds[4], &bounds[5], &index, &last); // 00463C36..00463C94
    void* node = live_operator_new(0x28);                                           // 00463C99..00463C9D
    if (node != nullptr) {                                                          // 00463CA8..00463CAA
        put<U>(node, 0x0, 0); put<U>(node, 0x4, 0); put<U>(node, 0x8, 0); put<U>(node, 0xC, 0);    // 00463CAC..00463CB4
        put<U>(node, 0x10, 0); put<U>(node, 0x14, 0); put<U>(node, 0x1C, 0); put<U>(node, 0x18, 0); // 00463CB7..00463CC0
        put<I>(node, 0x20, -1); put<U>(node, 0x24, 0);                              // 00463CC3..00463CC6
    }
    put<U>(out_slot, 0, addr(node));                                                // 00463CEB (NULL node: the
    // original then faults storing [eax+20h] at 00463CED - allocation failure only; same store reproduced)
    node = ptr<void>(get<U>(out_slot, 0));
    put<I>(node, 0x20, index);                                                      // 00463CED
    put<float>(node, 0x0, bounds[0]); put<float>(node, 0x4, bounds[1]); put<float>(node, 0x8, bounds[2]); // 00463CF0..00463D03 (raw)
    put<float>(node, 0xC, bounds[3]); put<float>(node, 0x10, bounds[4]); put<float>(node, 0x14, bounds[5]); // 00463D06..00463D1F (raw)
    // 00463D28..00463DA8 (fields == 7): the locals min -= 0.01, max += 0.01 (0073DFEC) are dead stores - the node
    // already holds the raw values and every later read of [esp+10h]/[esp+18h] is preceded by a new store; no
    // observable effect, not emitted.
    (void)kAabbPad_0073dfec;
    if (get<I>(ptr<void>(get<U>(out_slot, 0)), 0x20) != -1) return;                // 00463DAC..00463DB1
    mdl_parse_aabb_00463c20(resource_next_0044bfa0(), at(ptr<void>(get<U>(out_slot, 0)), 0x18)); // 00463DB7..00463DC3
    mdl_parse_aabb_00463c20(resource_next_0044bfa0(), at(ptr<void>(get<U>(out_slot, 0)), 0x1C)); // 00463DC8..00463DD4
    if (fields != 7) {                                                              // 00463DDC..00463DDF
        put<I>(ptr<void>(get<U>(out_slot, 0)), 0x24, last);                         // 00463F70..00463F76
        return;
    }
    void* const self = ptr<void>(get<U>(out_slot, 0));                              // 00463DE5
    const U a = get<U>(self, 0x18), b = get<U>(self, 0x1C);                         // 00463DE7 / 00463DFB
    const double s0 = static_cast<double>(fval(a, 0xC)) + fval(a, 0x0);             // 00463DEA..00463DED
    const double s1 = static_cast<double>(fval(a, 0x4)) + fval(a, 0x10);            // 00463DEF..00463DF2
    const float s2 = static_cast<float>(static_cast<double>(fval(a, 0x8)) + fval(a, 0x14)); // 00463DF5..00463DFE fstp [esp+3Ch]
    const double t0 = static_cast<double>(fval(b, 0x0)) + fval(b, 0xC);             // 00463E02..00463E04
    const float t1 = static_cast<float>(static_cast<double>(fval(b, 0x4)) + fval(b, 0x10)); // 00463E07..00463E0D fstp [esp+2Ch]
    const float t2 = static_cast<float>(static_cast<double>(fval(b, 0x8)) + fval(b, 0x14)); // 00463E11..00463E17 fstp [esp+30h]
    const float dx = static_cast<float>(t0 - s0);                                   // 00463E1B..00463E1D fstp [esp+10h]
    const double dy = static_cast<double>(t1) - s1;                                 // 00463E21..00463E29 (kept wide in ST0)
    const float dz = static_cast<float>(static_cast<double>(t2) - static_cast<double>(s2)); // 00463E2B..00463E33 fstp [esp+18h]
    // |v| as compiled: fcomp 0; test ah,5; jp -> fchs only when v < 0 (NaN and -0 unchanged)
    const double ax = dx < kZero_0073d700 ? -static_cast<double>(dx) : static_cast<double>(dx);
    const double ay = dy < kZero_0073d700 ? -dy : dy;
    const double az = dz < kZero_0073d700 ? -static_cast<double>(dz) : static_cast<double>(dz);
    I shift;
    if (ax > ay && ax > az)                                                         // 00463E37..00463E6C / 00463E6E..00463EA7
        shift = dx > kZero_0073d700 ? 0 : 3;                                        // 00463EAB..00463ED9 (test 41h)
    else if (ay > az)                                                               // 00463EE5..00463F18
        shift = dy > kZero_0073d700 ? 1 : 4;                                        // 00463F1A..00463F36
    else
        shift = dz > kZero_0073d700 ? 2 : 5;                                        // 00463F42..00463F64
    put<I>(self, 0x24, 1 << shift);                                                 // shl; mov [esi+24h]
}
} // extern "C" (definitions below were declared extern "C" above / in asciimdl.hpp and keep C linkage)

namespace {
// ===================================================================== Parse<T> (count header "%d %c", binary payload,
// or an "endlist"-terminated line list). One template, one instantiation per original body; Traits cite the
// per-body differences (element size/parser, allocator, first growth, tail form).
template <class T> void parse_list(char* text, List* list, typename T::State& state) {
    I count = 0;                                                                    // [esp+14h] = 0 (e.g. 004628DB)
    char separator;                                                                 // %c target, never read
    const int fields = kotor_sscanf_006fae08(text, kCountChar_0073ee54, &count, &separator); // e.g. 004628CB..004628E3
    if (fields > 0) {                                                               // test; jle
        T::header(list, count);
        if (fields == 1) {                                                          // one element per following line
            if (list->size > 0) {                                                   // (n > 0)
                I i = 0;
                do {
                    char* const line = resource_next_0044bfa0();
                    if (line == nullptr) return;
                    T::parse(state, line, elem(list, i, T::kSize));
                    ++i;
                } while (i < list->size);                                           // live size
            }
            return;
        }
        copy_binary(text, list, static_cast<U>(count) * T::kSize);                  // "%d %c": binary payload
        return;
    }
    char* line = resource_next_0044bfa0();
    if (line != nullptr) {
        do {
            if (kotor_strncmp_006faff0(T::skip(line), kEndlist_0073ee4c, 7) == 0) break;
            if (list->size == 0) {
                T::allocate(list, 1);
                list->size = 1;
            } else if (list->size == count) {
                T::append_last(list, count);                                         // copy of data[count-1] appended
            }
            T::parse(state, line, elem(list, count, T::kSize));
            ++count;
            line = resource_next_0044bfa0();
        } while (line != nullptr);
    }
    T::tail(line);
}
struct NoState {};
// generic "copy last element to a local, grow when full, store, ++size" (rep movs / mov forms)
template <U Size> void append_copy(List* list, I count, void (*allocate)(List*, I)) {
    unsigned char last[Size];
    std::memcpy(last, vp(elem(list, count - 1, Size)), Size);
    if (list->size == list->capacity) allocate(list, grow_or(list->capacity, 8));
    std::memcpy(vp(elem(list, list->size, Size)), last, Size);
    list->size = list->size + 1;
}
void alloc_00437d60(List* l, I n) { kotor_renderer_gl::allocate_00437d60(l, nullptr, static_cast<U>(n)); }
void alloc_005e03c0(List* l, I n) { menu_list_growth::allocate_005e03c0(l, nullptr, static_cast<U>(n)); }
void alloc_00463950(List* l, I n) { aur_generic_key_list_allocate_00463950(l, nullptr, n); }
void alloc_0043f6b0(List* l, I n) { aurora_skin_weight_list_allocate_0043f6b0(l, nullptr, n); }
void alloc_00437cc0(List* l, I n) { quaternion_list_allocate_00437cc0(l, nullptr, n); }
void alloc_0043f5e0(List* l, I n) { maxface_postprocess_list_allocate_0043f5e0(l, nullptr, n); }

// 004628C0 Parse<float>: header 004628F5..00462930 (Allocate 00437D60); per-line 00462935..00462962 (004635C0);
// binary 0046296C..00462993; endlist 0046299B..00462A4E with skip_blanks_00463510 CALLED (004629BB); first-use
// Allocate(1) 004629DB..004629E3; last copy fld/fstp float 004629F0..00462A1F with first growth 10h (00462A09);
// tail 00462A54..00462A63 (called). Donor: lane-texture-epoch controller_base.cpp read_float_list_004628c0
// (anonymous-namespace; covered only the header/per-line/binary forms) - logic reused, endlist form added.
struct FloatList {
    using State = NoState;
    static constexpr U kSize = 4;
    static void header(List* l, I n) { header_resize(l, n, alloc_00437d60); }
    static void allocate(List* l, I n) { alloc_00437d60(l, n); }
    static void parse(State&, char* line, U e) { parse_float_004635c0(line, ptr<volatile float>(e)); }
    static char* skip(char* line) { return skip_blanks_00463510(line); }
    static void append_last(List* l, I count) {
        const float last = get<float>(vp(l->data), static_cast<U>(count) * 4u - 4u); // 004629F2..004629FB
        if (l->size == l->capacity) alloc_00437d60(l, grow_or(l->capacity, 0x10));  // 004629F6..00462A11
        put<float>(vp(l->data), static_cast<U>(l->size) * 4u, last);                 // 00462A16..00462A1F
        l->size = l->size + 1;                                                       // 00462A22..00462A26
    }
    static void tail(char* line) { tail_called(line); }
};
// 00464630 Parse<int>: header 00464665..004646A0 (Allocate 005E03C0); per-line 004646A5..004646CE (004635E0);
// binary 004646D8..004646FF; endlist 00464707..004647B9 (inline skip 00464752); Allocate(1) 00464745..0046474D;
// last copy (mov) 0046475F..00464791 first growth 8; tail 004647BF..004647EC.
struct IntList {
    using State = NoState;
    static constexpr U kSize = 4;
    static void header(List* l, I n) { header_resize(l, n, alloc_005e03c0); }
    static void allocate(List* l, I n) { alloc_005e03c0(l, n); }
    static void parse(State&, char* line, U e) { parse_int_004635e0(line, ptr<volatile std::int32_t>(e)); }
    static char* skip(char* line) { return skip_inline(line); }
    static void append_last(List* l, I count) { append_copy<4>(l, count, alloc_005e03c0); }
    static void tail(char* line) { tail_inline(line); }
};
// 00464440 Parse<AurGenericKey>: header 00464475..004644B0 (Allocate 00463950); per-line 004644B5..004644E0
// (00463B30, stride 2Ch); binary 004644EA..0046450F; endlist 00464517..004645EE; Allocate(1) 00464555..0046455E;
// last copy rep movs 0Bh 00464574..004645C3 first growth 8; tail 004645F4..00464621.
struct KeyList {
    using State = NoState;
    static constexpr U kSize = 0x2C;
    static void header(List* l, I n) { header_resize(l, n, alloc_00463950); }
    static void allocate(List* l, I n) { alloc_00463950(l, n); }
    static void parse(State&, char* line, U e) { mdl_parse_aur_generic_key_00463b30(line, vp(e)); }
    static char* skip(char* line) { return skip_inline(line); }
    static void append_last(List* l, I count) { append_copy<0x2C>(l, count, alloc_00463950); }
    static void tail(char* line) { tail_inline(line); }
};
// 00468A00 Parse<MaxFace_PostProcess>: header = SetSize 00444F10 (00468A31..00468A3C); per-line 00468A46..00468A71
// (00463710, stride 14h); binary 00468A7B..00468AA4; endlist 00468AAE..00468B4F; Allocate(1) 00468AE2..00468AEB
// (0043F5E0); last copy = Add(by value copy of data[count-1]) 004647F0 (00468B01..00468B21: the Add destroys its
// argument, freeing the buffers shared with data[count-1] - original behaviour); tail 00468B55..00468B82.
struct MaxFacePostProcessList {
    using State = NoState;
    static constexpr U kSize = 0x14;
    static void header(List* l, I n) { maxface_postprocess_list_set_size_00444f10(l, nullptr, n); }
    static void allocate(List* l, I n) { alloc_0043f5e0(l, n); }
    static void parse(State&, char* line, U e) { mdl_parse_maxface_postprocess_00463710(line, vp(e)); }
    static char* skip(char* line) { return skip_inline(line); }
    static void append_last(List* l, I count) {
        asciimdl_b::MaxFacePostProcess copy;
        std::memcpy(&copy, vp(elem(l, count - 1, 0x14)), 0x14);                      // 00468B09..00468B1D
        maxface_postprocess_list_add_004647f0(l, nullptr, copy);                     // 00468B1F..00468B21
    }
    static void tail(char* line) { tail_inline(line); }
};
// 00468B90 Parse<uint>: header 00468BC5..00468C00 (Allocate 00437D60); per-line 00468C05..00468C39 - 004635E0
// into the local [esp+18h] then stored to data[i]; binary 00468C43..00468C6A; endlist 00468C72..00468D33 (same
// local, 00468D04..00468D1C); Allocate(1) 00468CB5..00468CBD; last copy (mov) 00468CCF..00468D01 growth 8;
// tail 00468D39..00468D6C.
struct UintList {
    struct State { std::int32_t value = 0; };   // D2 ([esp+18h]): no initializing store; stored even when 004635E0 converts nothing
    static constexpr U kSize = 4;
    static void header(List* l, I n) { header_resize(l, n, alloc_00437d60); }
    static void allocate(List* l, I n) { alloc_00437d60(l, n); }
    static void parse(State& s, char* line, U e) {
        parse_int_004635e0(line, &s.value);
        put<std::int32_t>(vp(e), 0, s.value);
    }
    static char* skip(char* line) { return skip_inline(line); }
    static void append_last(List* l, I count) { append_copy<4>(l, count, alloc_00437d60); }
    static void tail(char* line) { tail_inline(line); }
};
// 00468D70 Parse<SkinVertexWeight>: header 00468DA5..00468DE0 (Allocate 0043F6B0); per-line 00468DE5..00468E10
// (004637A0, stride 34h); binary 00468E1A..00468E3F; endlist 00468E47..00468F1E; Allocate(1) 00468E85..00468E8E;
// last copy rep movs 0Dh 00468EA4..00468EF3 growth 8; tail 00468F24..00468F51.
struct SkinWeightList {
    using State = NoState;
    static constexpr U kSize = 0x34;
    static void header(List* l, I n) { header_resize(l, n, alloc_0043f6b0); }
    static void allocate(List* l, I n) { alloc_0043f6b0(l, n); }
    static void parse(State&, char* line, U e) { mdl_parse_skin_weight_004637a0(line, vp(e)); }
    static char* skip(char* line) { return skip_inline(line); }
    static void append_last(List* l, I count) { append_copy<0x34>(l, count, alloc_0043f6b0); }
    static void tail(char* line) { tail_inline(line); }
};
// 00468F60 Parse<Quaternion>: header 00468F95..00468FD0 (Allocate 00437CC0); per-line 00468FD5..00469000
// (00463A30, stride 10h); binary 0046900A..00469030; endlist 00469038..0046911B; Allocate(1) 00469075..0046907E;
// last copy (4 dwords) 00469094..004690ED growth 8; tail 00469121..0046914E.
struct QuaternionList {
    using State = NoState;
    static constexpr U kSize = 0x10;
    static void header(List* l, I n) { header_resize(l, n, alloc_00437cc0); }
    static void allocate(List* l, I n) { alloc_00437cc0(l, n); }
    static void parse(State&, char* line, U e) { mdl_parse_quaternion_00463a30(line, vp(e)); }
    static char* skip(char* line) { return skip_inline(line); }
    static void append_last(List* l, I count) { append_copy<0x10>(l, count, alloc_00437cc0); }
    static void tail(char* line) { tail_inline(line); }
};

} // namespace

// M48 composer: 004628C0 single owner = texture-epoch controller_base.cpp (extern "C" mdl_parse_float_list_004628c0); this copy dropped.
void __cdecl mdl_parse_int_list_00464630(char* text, List* list) { NoState s; parse_list<IntList>(text, list, s); }
void __cdecl mdl_parse_aur_generic_key_list_00464440(char* text, List* list) { NoState s; parse_list<KeyList>(text, list, s); }
void __cdecl mdl_parse_maxface_list_00468a00(char* text, List* list) { NoState s; parse_list<MaxFacePostProcessList>(text, list, s); }
void __cdecl mdl_parse_uint_list_00468b90(char* text, List* list) { UintList::State s; parse_list<UintList>(text, list, s); }
void __cdecl mdl_parse_skin_weight_list_00468d70(char* text, List* list) { NoState s; parse_list<SkinWeightList>(text, list, s); }
void __cdecl mdl_parse_quaternion_list_00468f60(char* text, List* list) { NoState s; parse_list<QuaternionList>(text, list, s); }

// 00464880 Parse<char*>: "%d" count (local = 0). count > 0 form: inline resize (005E03C0) and, for each element,
// data[i] = new(80h), next line (no NULL test), sscanf "%s" into it (copy-from-result when 0). Otherwise the endlist
// form: Allocate(1) on an empty list, last pointer appended when size == i, then data[i] = new(80h) + "%s".
void __cdecl mdl_parse_cstr_list_00464880(char* text, List* list) {
    I count = 0;                                                                    // 00464893 [esp+0Ch] = 0
    if (kotor_sscanf_006fae08(text, kD_0073d720, &count) > 0) {                     // 00464881..004648A5
        header_resize(list, count, alloc_005e03c0);                                 // 004648AB..004648DF
        for (I i = 0; i < list->size; ++i) {                                        // 004648DB..00464920 (n > 0, live size)
            put<U>(vp(list->data), static_cast<U>(i) * 4u, addr(live_operator_new(0x80))); // 004648E4..004648F0
            char* const name = ptr<char>(get<U>(vp(list->data), static_cast<U>(i) * 4u));  // 004648F3..004648F5
            char* const line = resource_next_0044bfa0();                            // 004648F8
            const int result = kotor_sscanf_006fae08(line, kS_0073d5d0, name);       // 004648FD..00464904
            if (result == 0) copy_from_result(name, result);                        // 0046490C..00464918
        }
        return;
    }
    char* line = resource_next_0044bfa0();                                          // 00464927
    if (line != nullptr) {                                                          // 0046492E..00464930
        do {
            if (kotor_strncmp_006faff0(skip_inline(line), kEndlist_0073ee4c, 7) == 0) break; // 00464940..00464958
            if (list->size == 0) {                                                  // 0046495E..00464963
                alloc_005e03c0(list, 1);                                            // 00464965..00464969
                list->size = 1;                                                     // 0046496E
            } else if (list->size == count) {                                       // 00464984..0046498A
                append_copy<4>(list, count, alloc_005e03c0);                        // 0046498C..004649B6
            }
            put<U>(vp(list->data), static_cast<U>(count) * 4u, addr(live_operator_new(0x80))); // 004649B9..004649C9
            char* const name = ptr<char>(get<U>(vp(list->data), static_cast<U>(count) * 4u)); // 004649CC..004649D2
            const int result = kotor_sscanf_006fae08(line, kS_0073d5d0, name);       // 004649D5..004649DC
            if (result == 0) copy_from_result(name, result);                        // 004649E4..004649F0
            ++count;                                                                // 004649F2
            line = resource_next_0044bfa0();                                        // 004649F6
        } while (line != nullptr);                                                  // 004649FD..004649FF
    }
    tail_inline(line);                                                              // 00464A05..00464A32
}
// 00464A40 Parse<uint*>: "%d" count (local = 0; no field test); count == 0 -> free data (data = 0), capacity = 0,
// else Allocate 00437D60; size = count; per element: next line, "%d" -> row length (local), data[i] =
// new(length*4), Parse_uint(line, data[i]) (00463850).
void __cdecl mdl_parse_uint_ptr_list_00464a40(char* text, List* list) {
    I count = 0;                                                                    // 00464A57 [esp+0Ch] = 0
    I length = 0;   // D2 (00464A9B [esp+10h]): no initializing store; read at 00464AAD when "%d" converts nothing
    kotor_sscanf_006fae08(text, kD_0073d720, &count);                               // 00464A43..00464A5B
    if (count == 0) {                                                               // 00464A6B..00464A6F
        if (list->data != 0) { live_free(vp(list->data)); list->data = 0; }         // 00464A71..00464A80
        list->capacity = 0;                                                         // 00464A82
    } else {
        alloc_00437d60(list, count);                                                // 00464A87..00464A8A
    }
    list->size = count;                                                             // 00464A91
    for (I i = 0; i < list->size; ++i) {                                            // 00464A8F..00464AD8 (n > 0, live size)
        char* const line = resource_next_0044bfa0();                                // 00464A96 (no NULL test)
        kotor_sscanf_006fae08(line, kD_0073d720, &length);                          // 00464A9B..00464AA8
        put<U>(vp(list->data), static_cast<U>(i) * 4u, addr(live_operator_new(static_cast<U>(length) * 4u))); // 00464AAD..00464AC0
        mdl_parse_uint_array_00463850(line, ptr<U>(get<U>(vp(list->data), static_cast<U>(i) * 4u))); // 00464AC3..00464ACA
    }
}

// ===================================================================== FindMdlNode / key processing
// 00464AF0 FindMdlNode(node, name): NULL -> 0; stricmp(name, node name 00438A00) == 0 -> node; else depth-first over
// the children (+2C data, +30 count read once) returning the first hit; 0.
MdlNode* __cdecl mdl_find_node_00464af0(MdlNode* root, const char* name) {
    if (root == nullptr) return nullptr;                                            // 00464AF5..00464AFC
    if (kotor_stricmp_0070acaf(name, aurora_node_translate_name_00438a00(root, nullptr)) == 0) // 00464B00..00464B17
        return root;                                                                // 00464B19..00464B1F
    const I count = get<I>(root, 0x30);                                             // 00464B20
    for (I i = 0; i < count; ++i) {                                                 // 00464B23..00464B47
        MdlNode* const hit = mdl_find_node_00464af0(ptr<MdlNode>(get<U>(ptr<void>(get<U>(root, 0x2C)), static_cast<U>(i) * 4u)), name); // 00464B30..00464B38
        if (hit != nullptr) return hit;                                             // 00464B40..00464B42
    }
    return nullptr;                                                                 // 00464B49
}
// 00463F80 CullUnnecessaryKeys(keys): for i = size-2 down to 2: when key[i-1]'s four values are within 0078E58C of
// both key[i] and key[i-2] (fld eps; fcompp; test ah,41h: eps > |a-b|), deleteAt(i-1) (004638B0).
void __cdecl mdl_cull_unnecessary_keys_00463f80(List* keys) {
    auto close = [](U a, U b) {   // fld [a]; fsub [b]; fabs; fld [0078E58C]; fcompp; test ah,41h; jne -> keep
        return static_cast<double>(mdl_key_epsilon_0078e58c) > std::fabs(static_cast<double>(fval(a, 0)) - fval(b, 0));
    };
    for (I i = keys->size - 2; i > 1; --i) {                                        // 00463F86..00463F8F / 00464071..00464078
        const U k = keys->data + static_cast<U>(i) * 0x2Cu;                         // 00463FA0..00463FA6 (data reloaded)
        if (close(k + 0x04u, k - 0x28u) && close(k + 0x08u, k - 0x24u) &&          // 00463FA2..00463FD6
            close(k + 0x0Cu, k - 0x20u) && close(k + 0x10u, k - 0x1Cu) &&          // 00463FDC..00464008
            close(k - 0x54u, k - 0x28u) && close(k - 0x50u, k - 0x24u) &&          // 0046400A..00464036
            close(k - 0x4Cu, k - 0x20u) && close(k - 0x48u, k - 0x1Cu))            // 00464038..00464064
            aur_generic_key_list_delete_at_004638b0(keys, nullptr, i - 1);          // 00464066..0046406C
    }
}
// 00464B50 CompressQuaternionKey(keys): every key (time, x, y, z, w) becomes (time, packed, ..., count 2) in a local
// list: q negated when w < 0 (004A9B80 by -1); x, y -> clamp((v+1)*1023, 0, 2046), z -> clamp((v+1)*511, 0, 1022),
// rounded (frndint/fistp), packed = z<<22 | y<<11 | x; then keys = local (004643B0) and the local data freed.
void __cdecl mdl_compress_quaternion_keys_00464b50(List* keys) {
    List local;                                                                     // [esp+3Ch] {data, size, capacity}
    local.data = 0; local.size = 0; local.capacity = 0;                             // 00464B6F..00464B7D
    Quaternion q;                                                                   // [esp+1Ch] {w, x, y, z}
    q.x = 0.0f; q.y = 0.0f; q.z = 0.0f; q.w = 1.0f;                                 // 00464B92..00464B9E (overwritten per key)
    unsigned char key[0x2C] = {};   // D2 ([esp+58h]): words 2..9 have no initializing store and are copied out
    for (I i = 0; i < keys->size; ++i) {                                            // 00464B88..00464BAA / 00464DE0..00464DF2
        const U k = elem(keys, i, 0x2C);                                            // 00464BD2..00464BDC
        put<U>(key, 0x0, get<U>(vp(k), 0x0));                                       // 00464BD9 / 00464BE5
        q.x = fval(k, 0x4); q.y = fval(k, 0x8); q.z = fval(k, 0xC);                 // 00464BDE..00464BF3 (raw)
        q.w = fval(k, 0x10);                                                        // 00464BF7..00464BFA fld/fst
        put<I>(key, 0x28, 2);                                                       // 00464BFE
        if (q.w < kZero_0073d700) {                                                 // 00464C09..00464C14 (test ah,5; jp)
            Quaternion negated;                                                     // [esp+84h]
            const Quaternion* const r = quaternion_scalar_multiply_004a9b80(&negated, &q, -1.0f); // 00464C16..00464C28 (BF800000h)
            q = *r;                                                                 // 00464C2D..00464C47 (through EAX)
        }
        auto quantise = [](float v, float scale, float range, U clamp) {           // 00464C4B..00464CA4 shape
            const double wide = (static_cast<double>(v) + kOne_0073d764) * scale;   // fld; fadd 1.0; fmul
            float stored;
            if (range < wide) { std::memcpy(&stored, &clamp, 4); }                  // fcomp; jnp / jp: clamp
            else if (kZero_0073d700 > wide) stored = 0.0f;                          // 0 > v -> 0
            else stored = static_cast<float>(wide);                                 // fstp dword
            return stored;
        };
        const float xs = quantise(q.x, kXYScale_00741f1c, kXYRange_00741f18, kXYClamp); // 00464C4B..00464CA4 -> [esp+34h]
        const float ys = quantise(q.y, kXYScale_00741f1c, kXYRange_00741f18, kXYClamp); // 00464CA8..00464D01 -> [esp+2Ch]
        const float zs = quantise(q.z, kZScale_00741f14, kZRange_00741f10, kZClamp);    // 00464D05..00464D5E -> [esp+38h]
        I xi, yi, zi;                                                               // [esp+14h], [esp+10h], [esp+18h] (pointers [54h], [4Ch], [50h])
        FloatToInt(&xi, xs);                                                        // 00464D62..00464D6C fld [esp+34h]; mov edx,[esp+54h]; frndint; fistp [edx]
        FloatToInt(&yi, ys);                                                        // 00464D6E..00464D78 fld [esp+2Ch]; mov edx,[esp+4Ch]; frndint; fistp [edx]
        FloatToInt(&zi, zs);                                                        // 00464D7A..00464D84 fld [esp+38h]; mov edx,[esp+50h]; frndint; fistp [edx]
        put<U>(key, 0x4, ((static_cast<U>(zi) << 11 | static_cast<U>(yi)) << 11) | static_cast<U>(xi)); // 00464D86..00464D9E
        if (local.size == local.capacity)                                           // 00464D9C..00464DA2
            aur_generic_key_list_allocate_00463950(&local, nullptr, grow_or(local.capacity, 8)); // 00464DA4..00464DB5
        std::memcpy(vp(elem(&local, local.size, 0x2C)), key, 0x2C);                 // 00464DBE..00464DDB
        local.size = local.size + 1;                                                // 00464DDA / 00464DE6
    }
    const U data = local.data;                                                      // 00464DF8 edi
    aur_generic_key_list_assign_004643b0(keys, nullptr, &local);                    // 00464DFC..00464E03
    if (data != 0) live_free(vp(data));                                             // 00464E08..00464E18 (state -1)
}

// ===================================================================== controller builders
// Controller record (16 bytes, appended to node+38 through 00437C50): {type dword; u16 index; u16 rows;
// u16 time offset; u16 value offset; u8 columns; 3 bytes never stored}.
namespace {
struct Record { U16 index, rows, time_offset, value_offset; U8 columns; U8 rest[3]; };
static_assert(sizeof(Record) == 12, "controller record tail");
void append_record(List* controllers, I type, const Record& r) {
    if (controllers->size == controllers->capacity)
        exo_array_list_allocate16_00437c50(controllers, nullptr, grow_or(controllers->capacity, 8));
    const U e = elem(controllers, controllers->size, 0x10);
    put<I>(vp(e), 0x0, type);
    std::memcpy(vp(e + 4u), &r, 12);
    controllers->size = controllers->size + 1;
}
void push_float(List* data, float value) {   // inline Add with new/copy/free growth
    if (data->size == data->capacity) grow_dwords_inline(data);
    put<float>(vp(data->data), static_cast<U>(data->size) * 4u, value);
    data->size = data->size + 1;
}
void push_float_00437d60(List* data, float value) {   // Add through Allocate 00437D60
    if (data->size == data->capacity) alloc_00437d60(data, grow_or(data->capacity, 8));
    put<float>(vp(data->data), static_cast<U>(data->size) * 4u, value);
    data->size = data->size + 1;
}
}

// 00464E40 AddGenericControllerSingle(keyword - not read, text, controllers, data, type, index): one key from the
// line (00463B30); 4 values (axis-angle) -> quaternion through the function-static Vector 007FBF08 (guard 007FBF14:
// bit set, no zeroing); values shifted up one slot, time = 0, count + 1; columns 1/3/4 for counts 2/4/5; data gets
// 0 then the values; record appended (00437C50); ++007FBAE8.
void __cdecl mdl_add_generic_controller_single_00464e40(char*, char* text, List* controllers, List* data, I type, I index) {
    Record r;
    std::memset(&r, 0, sizeof(r));   // D2 ([esp+1Ch..1Fh]): columns (counts other than 2/4/5) and the 3 tail bytes
    r.index = static_cast<U16>(index);                                              // 00464E47 / 00464E5F
    r.rows = 1;                                                                     // 00464E59 / 00464E64
    unsigned char key[0x2C];                                                        // [esp+30h]
    mdl_parse_aur_generic_key_00463b30(text, key);                                  // 00464E54..00464E69
    if (get<I>(key, 0x28) == 4) {                                                   // 00464E6E..00464E78
        if ((mdl_single_axis_guard_007fbf14 & 1u) == 0)                             // 00464E7A..00464E80
            mdl_single_axis_guard_007fbf14 = mdl_single_axis_guard_007fbf14 | 1u;   // 00464E82
        mdl_single_axis_007fbf08[0] = get<U>(key, 0x0);                             // 00464E88 / 00464E94
        mdl_single_axis_007fbf08[1] = get<U>(key, 0x4);                             // 00464E8C / 00464E9D
        mdl_single_axis_007fbf08[2] = get<U>(key, 0x8);                             // 00464E90 / 00464EA9
        const float angle = get<float>(key, 0xC);                                   // 00464E99..00464EA3
        const U words[3] = {mdl_single_axis_007fbf08[0], mdl_single_axis_007fbf08[1], mdl_single_axis_007fbf08[2]}; // 00464EA4..00464EC5
        Vector axis;
        std::memcpy(&axis, words, sizeof(axis));
        Quaternion q;                                                               // [esp+20h]
        quaternion_construct_004ac960(&q, nullptr, axis, angle);                    // 00464EC8..00464ECC
        put<float>(key, 0x0, q.x); put<float>(key, 0x4, q.y);                       // 00464ED1..00464EE5
        put<float>(key, 0x8, q.z); put<float>(key, 0xC, q.w);                       // 00464ED9..00464EED
    }
    for (I m = get<I>(key, 0x28) - 1; m >= 0; --m)                                  // 00464EF1..00464F09: key[m+1] = key[m]
        put<U>(key, static_cast<U>(m + 1) * 4u, get<U>(key, static_cast<U>(m) * 4u));
    const I count = get<I>(key, 0x28) + 1;                                          // 00464F0B..00464F10 (re-read after the shift)
    put<I>(key, 0x28, count);
    put<U>(key, 0x0, 0);                                                            // 00464F16 time = 0
    if (count == 2) r.columns = 1;                                                  // 00464F14..00464F1E / 00464F36
    else if (count == 4) r.columns = 3;                                             // 00464F20..00464F23 / 00464F2F
    else if (count == 5) r.columns = 4;                                             // 00464F25..00464F28
    r.time_offset = static_cast<U16>(data->size);                                   // 00464F44..00464F48
    if (data->size == data->capacity) grow_dwords_inline(data);                     // 00464F3E..00464F92
    put<U>(vp(data->data), static_cast<U>(data->size) * 4u, 0);                     // 00464F95..00464F9A
    data->size = data->size + 1;                                                    // 00464FA1..00464FAE
    r.value_offset = static_cast<U16>(data->size);                                  // 00464FAB / 00464FB1
    for (I e = 1; e < get<I>(key, 0x28); ++e)                                       // 00464FA9..00464FB6 / 00465024..00465031
        push_float(data, get<float>(key, static_cast<U>(e) * 4u));                  // 00464FC0..0046502C (fld/fstp via [esp+70h])
    append_record(controllers, type, r);                                            // 00465033..0046507A
    mdl_controller_values_007fbae8 = mdl_controller_values_007fbae8 + 1;            // 0046507D..00465085
}

// 00465090 AddGenericController(keyword - not read, text, controllers, data, type, index): key list (00464440);
// empty -> nothing. Columns 1/3/4 by the first key's count (2/4/5); count 5: cull (0078E590) and, unless the
// current model 007FBAE4 has word+50 bit 9, compress (0078E588) with columns 2. Times then per-key values appended
// to data; record appended; 007FBAE8 += rows.
void __cdecl mdl_add_generic_controller_00465090(char*, char* text, List* controllers, List* data, I type, I index) {
    List keys;                                                                      // [esp+0Ch]
    keys.data = 0; keys.size = 0; keys.capacity = 0;                                // 004650A9..004650B3
    Record r;
    std::memset(&r, 0, sizeof(r));   // D2 ([esp+24h..27h]): columns (other counts) and the 3 tail bytes
    r.index = static_cast<U16>(index);                                              // 004650BB / 004650CA
    mdl_parse_aur_generic_key_list_00464440(text, &keys);                           // 004650B7..004650CF
    r.rows = static_cast<U16>(keys.size);                                           // 004650D4..004650DB
    if (keys.size != 0) {                                                           // 004650E0..004650E6
        const I count = get<I>(vp(keys.data), 0x28);                                // 004650EC
        if (count == 2) r.columns = 1;                                              // 004650EF..004650F1 / 00465155
        else if (count == 4) r.columns = 3;                                         // 004650F3..004650F6 / 0046514E
        else if (count == 5) {                                                      // 004650F8..004650F9
            r.columns = 4;                                                          // 00465101
            if (mdl_cull_keys_0078e590 != 0) {                                      // 004650FB..00465106
                mdl_cull_unnecessary_keys_00463f80(&keys);                          // 00465108..0046510D
                r.rows = static_cast<U16>(keys.size);                               // 00465112..0046511A
            }
            if (mdl_compress_quaternions_0078e588 != 0 &&                           // 0046511F..00465125
                ((get<U16>(aurora_current_model_007fbae4, 0x50) >> 9) & 1u) == 0) { // 00465127..00465138
                mdl_compress_quaternion_keys_00464b50(&keys);                       // 0046513A..0046513F
                r.columns = 2;                                                      // 00465147
            }
        }
        r.time_offset = static_cast<U16>(data->size);                               // 00465166..0046516B
        for (I k = 0; k < keys.size; ++k)                                           // 0046515E..004651BA (L.size times)
            push_float_00437d60(data, fval(elem(&keys, k, 0x2C), 0x0));             // 00465180..004651B7
        r.value_offset = static_cast<U16>(data->size);                              // 004651C2..004651C6
        for (I k = 0; k < keys.size; ++k) {                                         // 004651CB..00465284
            const U key = elem(&keys, k, 0x2C);
            for (I e = 1; e < get<I>(vp(key), 0x28); ++e)                           // 004651E0..004651E9 / 00465260..0046526C (live)
                push_float(data, fval(key, static_cast<U>(e) * 4u));               // 004651F2..0046525D
        }
        mdl_controller_values_007fbae8 = mdl_controller_values_007fbae8 + keys.size; // 0046528A..00465296
        append_record(controllers, type, r);                                        // 00465290..004652E7
    }
    if (keys.data != 0) live_free(vp(keys.data));                                   // 004652EC..004652F9 (state -1)
}

// 00465320 AddBezierController(keyword - not read, text, controllers, data, type, index): as 00465090 without the
// cull/compress step; columns = ((first count - 1) / 3) | 10h (signed /3 via 55555556h); data list freed always.
void __cdecl mdl_add_bezier_controller_00465320(char*, char* text, List* controllers, List* data, I type, I index) {
    List keys;                                                                      // [esp+10h]
    keys.data = 0; keys.size = 0; keys.capacity = 0;                                // 0046533A..00465344
    Record r;
    std::memset(&r, 0, sizeof(r));   // D2 ([esp+29h..2Bh]): the 3 tail bytes are never stored
    r.index = static_cast<U16>(index);                                              // 0046534C / 0046535B
    mdl_parse_aur_generic_key_list_00464440(text, &keys);                           // 00465348..00465360
    r.rows = static_cast<U16>(keys.size);                                           // 00465365..0046536E
    if (keys.size == 0) {                                                           // 0046536C..00465373
        if (keys.data != 0) live_free(vp(keys.data));                               // 00465375..0046538A
        return;
    }
    r.columns = static_cast<U8>(static_cast<U8>((get<I>(vp(keys.data), 0x28) - 1) / 3) | 0x10u); // 004653A9..004653CB
    r.time_offset = static_cast<U16>(data->size);                                   // 004653BB / 004653CF
    for (I k = 0; k < keys.size; ++k)                                               // 004653C9..00465410
        push_float_00437d60(data, fval(elem(&keys, k, 0x2C), 0x0));                 // 004653D6..0046540D
    r.value_offset = static_cast<U16>(data->size);                                  // 00465418..0046541C
    for (I k = 0; k < keys.size; ++k) {                                             // 00465421..004654D3
        const U key = elem(&keys, k, 0x2C);
        for (I e = 1; e < get<I>(vp(key), 0x28); ++e)                               // 00465431..0046543A / 004654B1..004654BD
            push_float(data, fval(key, static_cast<U>(e) * 4u));                   // 00465440..004654AE
    }
    mdl_controller_values_007fbae8 = mdl_controller_values_007fbae8 + keys.size;    // 004654D9..004654E5
    append_record(controllers, type, r);                                            // 004654DF..00465536
    live_free(vp(keys.data));                                                       // 0046552A..00465541 (no NULL test)
}
