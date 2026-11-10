// CSWGuiMainInterface::UpdatePortraits: refreshes the three party portrait
// slots (and the sliding leader slot) from the party's server creatures.
#include <stdlib.h>
#include "k/sw_gui_main_interface.h"

// Health, force, level and experience of one slot.
__forceinline void CSWGuiMainInterfaceChar::UpdateStats(CSWSCreature* pServer)
{
    SetHealth(pServer->GetCurrentHitPoints(0), pServer->GetMaxHitPoints(1),
              pServer->GetPoisoned());
    CSWSCreatureStats* pStats = pServer->m_pStats;
    int nMaxFP = pServer->GetMaxForcePoints();
    int nCurFP = (short)(pStats->m_nCurrentForcePoints + pStats->m_nForcePointsBonus);
    if (nMaxFP == 0) {
        m_aBars[1].SetMaxValue(1);
        m_aBars[1].SetCurValue(0);
    }
    else {
        m_aBars[1].SetMaxValue(nMaxFP);
        m_aBars[1].SetCurValue(nCurFP);
    }
    m_nCurFP = nCurFP;
    m_nMaxFP = nMaxFP;
    m_bDirty = 1;
    m_nDefense = pServer->m_pStats->GetLevel(0);
    m_bDirty = 1;
    m_nLevel = __max((int)(pServer->m_pStats->GetExpNeededForLevelUp() -
                           pServer->m_pStats->GetExperience()), 0);
    m_bDirty = 1;
}

// NONMATCH (50): same size and flow; scratch-register rotation (vtable /
// app loads eax vs ecx/edx) and the effect-icon loop (ours counts down in
// edx, the original keeps the count in ebx and j in ecx).
// FUNCTION: SWKOTOR 0x00687860
void CSWGuiMainInterface::UpdatePortraits()
{
    CSWParty* pParty = g_pAppManager->m_pClientExoApp->GetParty();
    CExoString sUnused;
    for (int i = 0; i < 3; i++) {
        CSWCCreature* pMember = pParty->GetCharacter(i);
        CSWSCreature* pServer = pMember ? pMember->GetServerCreature() : 0;
        int bShow = pServer != 0;
        unsigned char nState = 0;
        unsigned char nStackB = 0;
        unsigned char nStackA = 0;
        int nSlot = i;
        if (m_fSlideTime > 0.0f) {
            switch (i) {
            case 0:
                nSlot = 1;
                break;
            case 1:
                if (pParty->m_nCount == 2) {
                    nSlot = 0;
                    bShow = 0;
                }
                else {
                    nSlot = 2;
                }
                break;
            case 2:
                if (pParty->m_nCount == 2) {
                    bShow = 1;
                }
                nSlot = 0;
                break;
            }
        }
        else if (pParty->m_nCount == 2) {
            if (i == 1) {
                bShow = 0;
                nSlot = 2;
            }
            else if (i == 2) {
                bShow = 1;
            }
        }
        if (pServer) {
            m_aParty[nSlot].m_sName = pMember->GetClientStats()->m_sFirstName;
            m_aParty[nSlot].m_sClass = pMember->GetClientStats()->m_sClassName;
            m_aParty[nSlot].m_aLabels[5].SetFillImage(pMember->GetPortrait(1));
            m_aParty[nSlot].UpdateStats(pServer);
            if (pServer->GetDead() || pServer->GetDeadTemp()) {
                nState = 1;
            }
            else if (pServer->m_pStats->CanLevelUp()) {
                nState = 4;
                if (pMember->m_bLevelUpPending) {
                    g_pAppManager->m_pClientExoApp->GetInGameGui()->UpdateStatus(9, 0);
                    pMember->m_bLevelUpPending = 0;
                }
            }
            else if (pServer->IsDebilitated()) {
                nState = 2;
                if (i == 0) {
                    SetCombatMessage(0xbb2b);
                }
            }
            else {
                for (int j = 0; j < pServer->m_aEffectIcons.num; j++) {
                    CSWSEffectIcon* pIcon = pServer->m_aEffectIcons[j];
                    if (pIcon->m_bVisible) {
                        if (pIcon->m_bBeneficial) {
                            nStackB++;
                        }
                        else {
                            nStackA++;
                        }
                    }
                }
            }
            if (!pServer->IsDebilitated()) {
                if (m_nCombatMessage == 0xbb2b && m_fQueueTime == -1.0f) {
                    m_lblDescription.m_cText.SetColor(g_vGuiColor23D8);
                    m_nCombatMessage = 0xbc50;
                    CExoString sText = g_pAppManager->GetClientExoApp()->GetGUIString(0xbc50);
                    m_lblDescription.m_cText.SetText(sText);
                    m_lblDescription.m_cText.m_fAlpha = 1.0f;
                }
            }
            m_aParty[nSlot].Show(1, nState, nStackB, nStackA);
            if (m_fSlideTime > 0.0f && nSlot == 0) {
                m_cLeader.m_sName = pMember->GetClientStats()->m_sFirstName;
                m_cLeader.m_sClass = pMember->GetClientStats()->m_sClassName;
                m_cLeader.m_aLabels[5].SetFillImage(pMember->GetPortrait(1));
                m_cLeader.UpdateStats(pServer);
                m_cLeader.Show(1, nState, nStackB, nStackA);
            }
        }
        if (!bShow) {
            m_aParty[i].Hide();
        }
    }
}
