// Gob attachment list (+0x1bc) add / remove.
#define G_EXO_LIST_CONTAINS
#include "g/exo_list.h"

class Gob {
public:
    char pad00[0x1bc];
    CExoArrayList<Gob*> m_lAttachments;     // +0x1bc

    void AddAttachment(Gob* pGob);          // 0x00447310
    void RemoveAttachment(Gob* pGob);       // 0x00447370
};

// FUNCTION: SWKOTOR 0x00447310
void Gob::AddAttachment(Gob* pGob)
{
    if (m_lAttachments.Contains(pGob))
        return;
    m_lAttachments.Add(pGob);
}

// FUNCTION: SWKOTOR 0x00447370
void Gob::RemoveAttachment(Gob* pGob)
{
    if (m_lAttachments.Contains(pGob))
        m_lAttachments.Remove(pGob);
}
