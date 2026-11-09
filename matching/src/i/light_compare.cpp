// LightCompare (aurlightmanager.o, 0x004A28C0): qsort comparator of the
// prioritised light lists. Hidden start inside functions.csv's 0x004A28B0
// entry.
#include "i/light_manager.h"

// FUNCTION: SWKOTOR 0x004A28C0
int __cdecl LightCompare(const void* a, const void* b)
{
    Light* pA = (*(Light**)a)->AsLight();
    Light* pB = (*(Light**)b)->AsLight();
    LightNode* pNodeB = pB->m_pNode;
    int nPriorityA = pA->m_nSortPriority ? pA->m_nSortPriority : pA->m_pNode->m_nShadowPriority;
    int nPriorityB = pB->m_nSortPriority ? pB->m_nSortPriority : pNodeB->m_nShadowPriority;
    if (pA->m_vColor.IsEqual(Vector(0.0f, 0.0f, 0.0f))) {
        return 1;
    }
    if (pB->m_vColor.IsEqual(Vector(0.0f, 0.0f, 0.0f))) {
        return -1;
    }
    if (nPriorityA < nPriorityB) {
        return -1;
    }
    if (nPriorityA > nPriorityB) {
        return 1;
    }
    if (pA->m_bAffectStatic == 0 && pB->m_bAffectStatic > 0) {
        return 1;
    }
    if (pA->m_bAffectStatic == 1) {
        if (pB->m_bAffectStatic == 0 || pB->m_bAffectStatic == 2) {
            return -1;
        }
    } else if (pA->m_bAffectStatic == 2) {
        if (pB->m_bAffectStatic == 0) {
            return -1;
        }
    } else if (pB->m_bAffectStatic == 1) {
        return 1;
    }
    if (pA->m_fPriority < pB->m_fPriority) {
        return -1;
    }
    if (pA->m_fPriority > pB->m_fPriority) {
        return 1;
    }
    return pB < pA ? 1 : -1;
}
