// Worker A round s11: CSWSCreature::PostLoad (behavioural reference:
// src/derived/1028-creature_template_finish/386865deba9f1ee1_creature_template_finish.cpp).
#include "a/sws_creature.h"

class CSWSFaction {
public:
    void AddMember(unsigned long oid, int bArg);    // 0x005BFA70
};

extern const unsigned short EFFECT_TYPE_074678C;    // 0x0074678C (68, placeholder name)

static inline int AddObjectToLookupTable(const CExoString& sTag, unsigned long oid)
{
    return GetServerExoApp()->GetModule6B0()->AddObjectToLookupTable(sTag, oid);
}

// FUNCTION: SWKOTOR 0x004F1C40
// NONMATCH (49): cl caches the constant 1 in ebx from the first block on (the original keeps
// immediates until m_bPostLoading = 1 and caches 0 in ebx instead); otherwise same shape.
void CSWSCreature::PostLoad()
{
    CServerAIMaster* pAIMaster = GetServerExoApp()->GetServerAIMaster();
    if (m_pStats->m_bIsPC == 1) {
        pAIMaster->SetAILevel((CSWSObject*)this, 4);
        unsigned long nSlot = 1;
        for (int i = 0; i < 18; i++, nSlot += nSlot) {
            CSWSItem* pItem = m_pInventory->GetItemInSlot(nSlot);
            if (pItem) {
                pItem->m_nFlags288 |= 8;
            }
        }
        for (unsigned long n = 0; n < GetItemRepository(1)->m_nItems; n++) {
            CSWSItem* pItem = GetItemRepository(1)->ItemListGetItem(n);
            pItem->m_nFlags288 |= 8;
        }
    }
    AddObjectToLookupTable(m_sTag, m_idSelf);
    m_b9D4 = m_pStats->m_bIsPC;
    CFactionManager* pFactionManager = GetServerExoApp()->GetFactionManager();
    if (m_b9D4 == 1) {
        if (pFactionManager && pFactionManager->m_pDefaultFaction) {
            pFactionManager->m_pDefaultFaction->AddMember(m_idSelf, 0);
        }
        m_pStats->m_nStartingFaction = 0;
    }
    else {
        int nFaction = m_pStats->m_nFactionId;
        m_pStats->m_nFactionId = -1;
        CSWSFaction* pFaction = pFactionManager->GetFaction(nFaction);
        if (pFaction) {
            pFaction->AddMember(m_idSelf, 0);
            m_pStats->m_nStartingFaction = nFaction;
        }
        else {
            CExoString sError;
            sError.Format("Cannot set creature %s to faction %d", m_sTag.CStr(), nFaction);
            pFaction = pFactionManager->GetFaction(1);
            pFaction->AddMember(m_idSelf, 0);
            m_pStats->m_nStartingFaction = 1;
        }
    }
    UpdatePersonalSpace();
    m_bPostLoading = 1;
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_074678C;
    pEffect->SetDurationType(g_nDurationEquipped);
    pEffect->SetCreator(m_idSelf);
    pEffect->SetInteger(0, m_pStats->m_nRace);
    ApplyEffect(pEffect, 0, 0);
    m_bPostLoading = 0;
    if (GetCurrentHitPoints(0) < 1) {
        SetAnimation(0x2718);
        m_bE8 = 0;
        if (!m_b9D4 || GetCurrentHitPoints(0) <= -10) {
            m_nAIStateFlags = 0;
        }
    }
    if (m_b9D4 == 1) {
        m_bF0 = 1;
    }
    UpdateAppearanceDependantInfo(0);
}
