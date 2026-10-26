// MdlNodeBuildMap (0x00439a10): numbers an animation tree like its model. The
// original inlines the recursion three levels deep; MdlNode::GetName is
// expanded only at the top level.
#include "g/mdl_ctor.h"

extern "C" int __cdecl _stricmp(const char* a, const char* b);

inline char* MdlNode::GetName()
{
    if (m_pTree->AsModel())
        return m_pTree->AsModel()->m_lStrings[m_n04];
    return m_pTree->AsAnimation()->m_pModel->m_lStrings[m_n04];
}

#pragma inline_recursion(on)
#pragma inline_depth(5)

// FUNCTION: SWKOTOR 0x00439a10
void __cdecl MdlNodeBuildMap(MdlNode* pDest, MdlNode* pSource, int* pNextId)
{
    pDest->m_nNodeNumber = pSource->m_nNodeNumber;
    for (int i = 0; i < pDest->m_lChildren.num; i++) {
        MdlNode* pChild = pDest->m_lChildren[i];
        int bFound = 0;
        for (int j = 0; j < pSource->m_lChildren.num; j++) {
            MdlNode* pOther = pSource->m_lChildren[j];
            if (!_stricmp(pChild->GetName(), pOther->GetName())) {
                MdlNodeBuildMap(pChild, pOther, pNextId);
                bFound = 1;
                break;
            }
        }
        if (!bFound) {
            pChild->m_nNodeNumber = *pNextId;
            (*pNextId)++;
        }
    }
}
