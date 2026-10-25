#include "i/bsp_traverse.h"

// FUNCTION: SWKOTOR 0x004ACEA0
void __cdecl BSPTraverse(BSPNode* pNode,
                        void (__cdecl* pVisit)(BSPNode*, LightScene*),
                        LightScene* pScene)
{
    if (pNode == 0) {
        return;
    }

    do {
        pVisit(pNode, pScene);
        BSPTraverse(pNode->m_pRightChild, pVisit, pScene);
        pNode = pNode->m_pLeftChild;
    } while (pNode != 0);
}
