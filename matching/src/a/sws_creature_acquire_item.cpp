// CSWSCreature::AcquireItem (nwscreatureai.cpp).
#include "a/sws_creature.h"

// NONMATCH: 27 diff: the m_idSelf branch picks edx/eax the other way round, so the SetPossessor tails are not cross-jumped.
// FUNCTION: SWKOTOR 0x005158E0
int CSWSCreature::AcquireItem(CSWSItem** ppItem, unsigned long oidFrom, unsigned long oidRepository, int bArg)
{
    GetServerExoApp()->GetObjectArray();
    CItemRepository* pRepository = 0;
    CSWSItem* pContainer = 0;
    if ((*ppItem)->m_oidPossessor != oidFrom) {
        pContainer = GetServerExoApp()->GetItemByGameObjectID((*ppItem)->m_oidPossessor);
        if (!pContainer || pContainer->m_oidPossessor != oidFrom) return 0;
        oidFrom = (*ppItem)->m_oidPossessor;
    }
    if (oidRepository == 0x7f000000) {
        pRepository = GetItemRepository(1);
    } else {
        pContainer = GetServerExoApp()->GetItemByGameObjectID(oidRepository);
        if (pContainer) pRepository = pContainer->m_pRepository;
    }
    CGameObject* pPrevious = GetServerExoApp()->GetGameObject(oidFrom);
    if (pPrevious) {
        unsigned char nType = pPrevious->m_nObjectType;
        if (nType == 5) {
            pPrevious->AsSWSCreature()->RemoveItem(*ppItem, 1, 1, 0);
        } else if (nType == 9) {
            pPrevious->AsSWSPlaceable()->RemoveItem(*ppItem);
        } else if (nType == 6 && pPrevious->AsSWSItem()->m_pRepository) {
            pPrevious->AsSWSItem()->m_pRepository->RemoveItem(*ppItem);
        }
    } else if (pContainer && g_pRules->GetBaseItem((*ppItem)->m_nBaseItem)->m_bContainer &&
               g_pRules->GetBaseItem(pContainer->m_nBaseItem)->m_bContainer) {
        return 0;
    }
    int nResult = pRepository->AddItem(ppItem, 1, bArg, 1);
    if (nResult == 1) {
        if (!pPrevious && *ppItem) (*ppItem)->RemoveFromArea();
        if (oidRepository == OBJECT_INVALID) {
            if (*ppItem) (*ppItem)->SetPossessor(m_idSelf, 1, bArg, 0x7f000000);
        } else if (*ppItem) {
            (*ppItem)->SetPossessor(oidRepository, 1, bArg, 0x7f000000);
        }
    }
    return nResult;
}
