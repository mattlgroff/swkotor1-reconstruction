// Lane W4: 00617AD0 CSWCCreature::CanSee(CSWCObject* target) (thiscall, RET 4; returns 0/1 in EAX).
// Transcribed from dis/00617AD0.asm. The line-of-sight probe SceneHitCheck 0060F1E0 belongs to W5 (G17 family);
// it is bound as a cross-lane symbol (see INTEGRATION.md) and the scene hit info is the existing static CHitInfo
// [007B93A8] owned by core46 (_scene_hit_info_007b93a8).
#include "27e3c46f56-w4_frame.hpp"

namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth {
void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations* list, void*, unsigned count); // CExoArrayList::Allocate
}
extern "C" {
extern unsigned char scene_hit_info_007b93a8[];                                   // [007B93A8] static CHitInfo (core46)
void __fastcall aurora_hit_info_clear_output_00456a60(void* hit, void*);          // CHitInfo::ClearOutput
void* __fastcall kotor_fastcall_00683ee0(void* door, void*);                      // CSWCDoor::GetServerDoor (W4 party unit)
// Cross-lane pending (W5): SceneHitCheck(scene, Vector from, Vector to, int, CHitInfo*) cdecl, returns 2 on a hit.
std::int32_t __cdecl kotor_cdecl_0060f1e0(void* scene, W4Vector from, W4Vector to, std::int32_t flags, void* hit);
std::uint32_t __fastcall kotor_fastcall_00617ad0(void* self, void*, void* target);
}
using namespace w4;

namespace {
// a2 (L3 t1): the vectors are copied as words (the original moves them with `mov`); only the two z components are
// floats the original computes (fld / fadd / fstp dword). A word-typed form of SceneHitCheck passes the two vectors as
// the same 12 stack bytes each (clang would expand a by-value float struct into float parameters).
struct V3W { U32 x, y, z; };
template <class F, class G> __forceinline F word_form(G* callee) {
    void* volatile address = reinterpret_cast<void*>(callee);
    return reinterpret_cast<F>(address);
}
__forceinline long double ld_word(U32 w) { float f; std::memcpy(&f, &w, 4); return f; }   // fld dword
__forceinline void store_float(U32& w, long double v) { const float f = static_cast<float>(v); std::memcpy(&w, &f, 4); } // fstp dword to the slot
void append_ignore(void* item) {                                                  // inlined CExoArrayList<Gob*>::Add
    U8* hit = scene_hit_info_007b93a8;
    const U32 count = lw(hit, 0x0C), alloc = lw(hit, 0x10);                       // [007B93B4], [007B93B8]
    if (count == alloc)
        menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(hit + 8), nullptr,
                                            alloc != 0 ? alloc + alloc : 0x10u);  // 00617B74..00617B8B
    sw(lp(hit, 8), lw(hit, 0x0C) * 4, ad(item));                                  // 00617B90..00617B9C
    sw(hit, 0x0C, lw(hit, 0x0C) + 1);                                             // 00617B9F..00617BA8
}
}

extern "C" U32 __fastcall kotor_fastcall_00617ad0(void* self, void*, void* target) {
    if (target == nullptr) return 0;                                              // 00617AD6..00617AEA
    V3W from, to;
    std::memcpy(&from, static_cast<U8*>(self) + 0x24, 12);                        // 00617AED..00617B05 [esp+1Ch..24h]
    store_float(from.z, ld_word(from.z) + 1.5f);                              // 00617B09..00617B1F (0073F414): fld, fadd, fstp dword
    std::memcpy(&to, static_cast<U8*>(target) + 0x24, 12);                        // 00617B00..00617B29 [esp+10h..18h]
    const long double height = vcall<float>(target, 0x140);                       // 00617B2D (ST0, kept unrounded)
    store_float(to.z, height + ld_word(to.z));                                // 00617B33..00617B3C: fadd dword [esp+18h], fstp dword
    U8* hit = scene_hit_info_007b93a8;
    aurora_hit_info_clear_output_00456a60(hit, nullptr);                          // 00617B37..00617B40
    hit[0x5C] = 1;                                                                // 00617B47 [007B9404]
    sw(hit, 0x0C, 0);                                                             // 00617B4E [007B93B4]
    append_ignore(vcall<void*, U32, U32>(self, 0x98, 0xFF, 1));                   // 00617B58..00617BA8 GetModel(-1 part,1)
    if (void* m = vcall<void*, U32, U32>(self, 0x98, 0xFE, 1)) append_ignore(m);  // 00617BAE..00617BF7
    append_ignore(vcall<void*, U32, U32>(target, 0x98, 0xFF, 1));                 // 00617BFD..00617C4F
    if (void* m = vcall<void*, U32, U32>(target, 0x98, 0xFE, 1)) append_ignore(m); // 00617C55..00617C9E
    I32 result;
    for (;;) {
        aurora_hit_info_clear_output_00456a60(hit, nullptr);                      // 00617CB0..00617CB5
        void* scene = lp(lp(self, 0x20), 0x184);                                  // 00617CBE..00617CC1
        result = word_form<std::int32_t(__cdecl*)(void*, V3W, V3W, std::int32_t, void*)>(&kotor_cdecl_0060f1e0)(scene, from, to, 0, hit); // 00617CC7..00617CF9
        if (result != 2) break;                                                   // 00617D03..00617D06
        void* object = vcall<void*>(lp(hit, 0x38), 0x70);                         // 00617D0C..00617D14 [007B93E0]
        if (object == nullptr) { result = 1; break; }                             // 00617D19..00617D1B -> 00617DB4
        void* model;
        if (vcall<void*>(object, 0x14) != nullptr) {                              // 00617D21..00617D2C (door)
            void* server_door = kotor_fastcall_00683ee0(vcall<void*>(object, 0x14), nullptr); // 00617D2E..00617D35
            if (server_door != nullptr && lb(server_door, 0x2CC) == 0) { result = 1; break; } // 00617D3A..00617D46
            model = vcall<void*, U32, U32>(vcall<void*>(object, 0x14), 0x98, 0xFF, 1); // 00617D48..00617D5A
        } else {
            if (vcall<void*>(object, 0x4C) == nullptr) break;                     // 00617D79..00617D80 (result stays 2)
            model = vcall<void*, U32, U32>(vcall<void*>(object, 0x4C), 0x98, 0xFF, 1); // 00617D82..00617D5A
        }
        append_ignore(model);                                                     // 00617D60..00617DA9
    }
    return result != 1 ? 1u : 0u;                                                 // 00617DB9..00617DC8
}
