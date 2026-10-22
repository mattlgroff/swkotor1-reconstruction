// SceneBSPGetObjectTextures (own TU: SceneBSPGetTileTextures in
// scenebsp.cpp calls it).

#include "h/scenebsp.h"

struct TexName {
    void* m_pName;                      // +0x00
};

class TexSource {
public:
    char pad_0x0[0x44];
    TexName* m_pHolder;                 // +0x44
};

class TexPart {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual TexSource* GetTexInfo();    // +0x2c
    char pad_0x4[0x2c];
    CExoArrayList<TexPart*> m_lChildren;    // +0x30
};

#pragma inline_recursion(on)

// FUNCTION: SWKOTOR 0x0046C060
void SceneBSPGetObjectTextures(TexPart* pPart, CExoArrayList<void*>* pList)
{
    if (!pPart)
        return;
    TexSource* pTex = pPart->GetTexInfo();
    if (pTex)
        pList->Add(pTex->m_pHolder->m_pName);
    for (int i = 0; i < pPart->m_lChildren.num; i++)
        SceneBSPGetObjectTextures(pPart->m_lChildren.element[i], pList);
}
