// Level-up preview state restoration and cancellation leaves.
#include "l/sw_gui_levelup_chargen.h"

namespace {
inline CSWCCreature* GetLiveCreature(CSWGuiLevelUpCharGen* pPanel)
{
    return g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(pPanel->m_oidCreature);
}

inline void HideFlags(void* pControl)
{
    unsigned long* pFlags = (unsigned long*)((char*)pControl + 0x44);
    *pFlags = (*pFlags & 0xFFFFFCFFul) | 0x400ul;
}
}

// FUNCTION: SWKOTOR 0x006E87E0
void CSWGuiLevelUpCharGen::ClearPowers()
{
    CSWCCreature* pCreature = GetLiveCreature(this);
    if (pCreature == 0) {
        return;
    }
    unsigned long nClass = (unsigned long)m_pSnapshot->m_pStats->m_nNumClasses - 1ul;
    pCreature->m_pStats->ClearSpells();
    unsigned char nIndex = 0;
    if (m_pSnapshot->m_pStats->GetNumberKnownSpells(nClass, 0) > 0) {
        do {
            CSWCCreatureStats* pTarget = pCreature->m_pStats;
            unsigned long nSpell = m_pSnapshot->m_pStats->GetKnownSpell(nClass, 0, nIndex);
            pTarget->AddKnownSpell(nClass, nSpell);
            ++nIndex;
        } while ((unsigned short)nIndex < m_pSnapshot->m_pStats->GetNumberKnownSpells(nClass, 0));
    }
}

// FUNCTION: SWKOTOR 0x006E8DB0
void CSWGuiLevelUpCharGen::ClearFeats()
{
    CSWCCreature* pCreature = GetLiveCreature(this);
    if (pCreature == 0) {
        return;
    }
    pCreature->m_pStats->ClearFeats();
    unsigned long nIndex = 0;
    if (m_pSnapshot->m_pStats->m_nNumFeats > 0) {
        do {
            CSWCCreatureStats* pTarget = pCreature->m_pStats;
            unsigned short nFeat = m_pSnapshot->m_pStats->GetFeat((unsigned short)nIndex);
            CExoArrayList<unsigned short>* pLiveFeats = (CExoArrayList<unsigned short>*)&pTarget->m_pFeats;
            pLiveFeats->AddUnique(nFeat);
            ++nIndex;
        } while ((unsigned short)nIndex < m_pSnapshot->m_pStats->m_nNumFeats);
    } else {
        return;
    }
}

// FUNCTION: SWKOTOR 0x006E8E20
void CSWGuiLevelUpCharGen::CancelLevelUp()
{
    if (m_nInputClass == 0) {
        return;
    }
    CSWCCreature* pCreature = GetLiveCreature(this);
    CopyCreatureStats(m_pSnapshot, pCreature);
    m_pManager->PopModalPanel();
    m_pManager->PopModalPanel();
    m_pLevelUpPanel->m_nFlags = (m_pLevelUpPanel->m_nFlags & 0xFFFFFCFFul) | 0x400ul;
    CSWGuiInGameCharacter* pCharacter = m_pCharacter;
    HideFlags(this);
    pCharacter->AttachModel();
    if (*(int*)0x007A2370 == 0 && m_nInputClass != 2) {
        m_pCharacter->m_nFlags = (m_pCharacter->m_nFlags & 0xFFFFFCFFul) | 0x400ul;
        CGuiInGame* pInGame = g_pAppManager->GetClientExoApp()->GetInGameGui();
        *(unsigned long*)((char*)pInGame + 0x14) = 0;
    }
    int nInputClass = m_nInputClass;
    g_pAppManager->GetClientExoApp()->SetInputClass(nInputClass, 1);
}
