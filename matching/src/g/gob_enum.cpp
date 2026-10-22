// Gob::EnumParts, the filtered depth-first part collector (the
// original inlines its own recursion seven levels deep).
#include "g/exo_list.h"

class Part {
public:
    char pad00[0x30];
    CExoArrayList<Part*> m_lChildren;       // +0x30
};

typedef int (*PartFilter)(Part* pPart);

class Gob {
public:
    void EnumParts(Part* pPart, PartFilter pfnFilter, CExoArrayList<Part*>* pList);
};

#pragma inline_recursion(on)
#pragma inline_depth(7)

// FUNCTION: SWKOTOR 0x004474d0
void Gob::EnumParts(Part* pPart, PartFilter pfnFilter, CExoArrayList<Part*>* pList)
{
    if (!pPart)
        return;
    if (!pfnFilter)
        pList->Add(pPart);
    else if (pfnFilter(pPart))
        pList->Add(pPart);
    for (int i = 0; i < pPart->m_lChildren.num; i++)
        EnumParts(pPart->m_lChildren.element[i], pfnFilter, pList);
}
