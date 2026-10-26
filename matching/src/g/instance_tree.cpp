// CreateInstanceTreeR: instantiate a model node tree as a part tree for a
// Gob. The original inlines its recursion two levels deep.
// Behavioural reference: src/units/0665/b313627324-model_instance.cpp.
#include "g/exo_list.h"

class Gob;
class Part;

extern float g_fControlWeight;                  // 0x00798b38

class MdlNode {
public:
    short m_nType;                              // +0x00
    short m_nIndex;                             // +0x02
    char pad04[0x2c - 0x04];
    MdlNode** m_pChildren;                      // +0x2c
    int m_nChildren;                            // +0x30

    void InternalControl(Part* pPart, float f1, int n, float f2);  // 0x0043f890
    Part* InternalCreateInstance(Gob* pGob);                        // 0x00448a70
};

class Part {
public:
    char pad00[0x28];
    int m_nIndex;                               // +0x28
    char pad2c[0x30 - 0x2c];
    CExoArrayList<Part*> m_lChildren;           // +0x30
    Part* m_pParent;                            // +0x3c
    Gob* m_pGob;                                // +0x40
};

#pragma inline_recursion(on)
#pragma inline_depth(3)

// FUNCTION: SWKOTOR 0x00449200
Part* __cdecl CreateInstanceTreeR(MdlNode* pNode, Gob* pGob)
{
    if (!pNode)
        return 0;
    Part* pPart = pNode->InternalCreateInstance(pGob);
    pPart->m_pGob = pGob;
    g_fControlWeight = 1.0f;
    pNode->InternalControl(pPart, -1.0f, 0, 1.0f);
    pPart->m_lChildren.SetSize(pNode->m_nChildren);
    for (int i = 0; i < pNode->m_nChildren; i++) {
        pPart->m_lChildren[i] = CreateInstanceTreeR(pNode->m_pChildren[i], pGob);
        pPart->m_lChildren[i]->m_pParent = pPart;
    }
    pPart->m_nIndex = pNode->m_nIndex;
    return pPart;
}
