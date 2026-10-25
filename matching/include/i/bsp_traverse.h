#ifndef I_BSP_TRAVERSE_H
#define I_BSP_TRAVERSE_H

class LightScene;

class BSPNode {
public:
    char pad_0x00[0x10];
    BSPNode* m_pRightChild; // +0x10
    BSPNode* m_pLeftChild;  // +0x14
};

void __cdecl BSPTraverse(BSPNode* pNode,
                         void (__cdecl* pVisit)(BSPNode*, LightScene*),
                         LightScene* pScene);

#endif
