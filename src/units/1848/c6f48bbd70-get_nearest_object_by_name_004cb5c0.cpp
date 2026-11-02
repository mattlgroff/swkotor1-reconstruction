// Original 004CB5C0 CSWSObject::GetNearestObjectByName.
// thiscall ECX=this (CSWSObject*), stack (const CExoString* name, float radius),
// RET 8, EAX = candidate server object id (candidate+4) or 0x7F000000.
// The return is an object id, never a packed color; callers map it via 005EDA50.
//
// Full entry-to-return behavior is in PLAN.md. Search origin and candidate
// position are the same three floats (this+0x90/94/98 and candidate+0x90/94/98):
// 004CB64F is add esi,0x90, not +0x144.
#include "65b8eb6bac-dialog_target_lookup_contracts.hpp"

#include "../../math/kotor_math.h"   // L1: long double sqrt (raw fsqrt on the 80-bit register value)

static_assert(sizeof(void*) == 4, "004CB5C0 requires the original x86 pointer ABI");

namespace {

// a2 (L3 t1): register values are long double; the original keeps `best` as a float in a stack slot
// ([esp+10h]: stored by fstp dword at 004CB602 / 004CB763, compared by fcom dword at 004CB74A): a volatile
// StoredFloat, so there is exactly one (float) conversion per original store and no 4-byte x87 spill.
struct StoredFloat {
    volatile float f;
    __forceinline long double value() const { return f; }   // fld / fcom dword [slot]
};
__forceinline long double ld_word(dtl::U32 w) { float f; std::memcpy(&f, &w, 4); return f; }   // fld dword

}  // namespace

extern "C" dtl::U32 __fastcall kotor_fastcall_004cb5c0(
    void* self, void*, const void* name, float radius) {
    using namespace dtl;

    // 004CB5D2 / 004CB5DF / 004CB602: sentinel best id, radius!=0 flag, radius^2.
    dtl::U32 best_id = 0x7F000000u;
    bool radius_nonzero = (static_cast<long double>(radius) != 0.0L);   // 004CB5C3..004CB5E6: fld ; fcomp 0 ; test ah,44h (equal -> 0; NaN -> 1)
    StoredFloat best;
    best.f = static_cast<float>(static_cast<long double>(radius) * radius);   // 004CB5EA..004CB602: fld ; fmul dword ; fstp dword [esp+10h]

    // 004CB5F7 / 004CB5EE / 004CB5FF / 004CB606: area-out slot, app/server read,
    // object array, then [this+0x8C].
    void* area_slot = nullptr;
    void* const app1 = kotor_menu_window_cells::manager_007a39fc;
    void* const array1 = recovered_object_array_004aed70(lp(app1, 8), nullptr);
    const dtl::U32 area_key = lw(self, 0x8C);
    (void)kotor_fastcall_004d8230(array1, nullptr, area_key, &area_slot);

    // 004CB61E / 004CB62A / 004CB631: out-null and AsArea-null gates.
    if (area_slot == nullptr) {
        return 0x7F000000u;
    }
    void* const area = as_area_slot_28(area_slot);
    if (area == nullptr) {
        return 0x7F000000u;
    }

    // 004CB639 / 004CB642 / 004CB649: empty name short-circuits to sentinel.
    // Exact selected symbol is ?BorrowedEquals005e5290@... (0061FBC0).
    if (reinterpret_cast<kotor_reconstruction::StartupString*>(
            const_cast<void*>(name))->BorrowedEquals005e5290("") != 0) {
        return 0x7F000000u;
    }

    // 004CB64F / 004CB655 / 004CB657 / 004CB65A: add esi,0x90 then copy
    // this+0x90/94/98. These are the object's own position and search origin.
    const dtl::U32 search_x = lw(self, 0x90);
    const dtl::U32 search_y = lw(self, 0x94);
    const dtl::U32 search_z = lw(self, 0x98);

    // 004CB661..004CB678: first area object; failure goes to RET 004CB780
    // with best_id still the 0x7F000000 sentinel.
    dtl::U32 cursor = 0;
    if (recovered_area_first_object_005071b0(area, nullptr, &cursor) != 1) {
        return best_id;
    }

    for (;;) {
        // 004CB680..004CB6A5: re-read app/server, object array, then the
        // iterator id; compare AL against the constant byte 0 at 0074666C.
        const dtl::U32 id = cursor;
        void* candidate = nullptr;
        void* const app2 = kotor_menu_window_cells::manager_007a39fc;
        void* const array2 = recovered_object_array_004aed70(lp(app2, 8), nullptr);
        const unsigned char status =
            kotor_fastcall_004d8230(array2, nullptr, id, &candidate);
        if (status == 0) {
            // 004CB6AB..004CB6D6: candidate position copy, then a second
            // app/server read and candidate+0x8C area lookup.
            const dtl::U32 cand_x = lw(candidate, 0x90);
            const dtl::U32 cand_y = lw(candidate, 0x94);
            const dtl::U32 cand_z = lw(candidate, 0x98);

            void* cand_area_slot = nullptr;
            void* const app3 = kotor_menu_window_cells::manager_007a39fc;
            void* const array3 = recovered_object_array_004aed70(lp(app3, 8), nullptr);
            const dtl::U32 cand_area_key = lw(candidate, 0x8C);
            (void)kotor_fastcall_004d8230(
                array3, nullptr, cand_area_key, &cand_area_slot);

            // 004CB6F2..004CB705: null out -> EAX 0; otherwise AsArea;
            // candidate area must equal the search area.
            void* cand_area = nullptr;
            if (cand_area_slot != nullptr) {
                cand_area = as_area_slot_28(cand_area_slot);
            }
            if (cand_area == area) {
                // 004CB707..004CB716: case-insensitive candidate tag compare.
                if (exostring_compare_no_case_005e6450(
                        atv(candidate, 0x18), nullptr, name) != 0) {
                    // 004CB718..004CB746: dx/dy/dz and (dz^2+dy^2)+dx^2 in
                    // exactly this order. Products are exact in double; each
                    // add rounds to 53 bits like x87 PC=53.
                    const long double dx = ld_word(cand_x) - ld_word(search_x);   // 004CB71A..004CB71E fld ; fsub dword
                    const long double dy = ld_word(cand_y) - ld_word(search_y);   // 004CB722..004CB726
                    const long double dz = ld_word(cand_z) - ld_word(search_z);   // 004CB72A..004CB72E
                    const long double dist2 = (dz * dz + dy * dy) + dx * dx;      // 004CB732..004CB740

                    // 004CB748..004CB753: radius==0 skips the comparison;
                    // otherwise accept only ordered dist2 <= best (NaN skips).
                    if (!radius_nonzero ||
                        dist2 <= best.value()) {           // 004CB74A fcom dword [esp+10h] ; test ah,41h ; jp
                        // 004CB755..004CB763: fsqrt, candidate+4, flag=1,
                        // float best store. The original loads the id after
                        // fsqrt and before the best store.
                        const long double root = sqrt(dist2);   // 004CB755 fsqrt
                        const dtl::U32 accepted_id = lw(candidate, 4);
                        best.f = static_cast<float>(root);      // 004CB763 fstp dword [esp+10h]
                        best_id = accepted_id;
                        radius_nonzero = true;
                    }
                }
            }
        }

        // 004CB76B..004CB77A: every candidate advances the area iterator;
        // return only when it stops.
        if (recovered_area_next_object_005071e0(area, nullptr, &cursor) != 1) {
            break;
        }
    }

    // 004CB780..004CB78A: return best id (sentinel when nothing accepted).
    return best_id;
}
