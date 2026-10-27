// CSWGuiMainInterface::GetActionIcon: icon / alternate icon resrefs for a
// queued action (attack, equip, power, item or feat).
#include "k/sw_gui_main_interface.h"

// FUNCTION: SWKOTOR 0x00686FB0
// NONMATCH: cl cross-jumps both "power" arms into the node-0x0f tail (lea
// hoisted above the "i_powerm" push), 32 bytes short; the original keeps
// every arm's own tail. Switch tables, temporaries and the other arms
// agree (the spell arm also picks ecx for the spell id where the original
// reuses eax).
void CSWGuiMainInterface::GetActionIcon(CSWSCombatRound* pRound, CSWSObjectActionNode* pNode,
                                        CSWSCombatRoundAction* pAction, CExoLinkedListPosition& pos,
                                        CResRef& cIcon, CResRef& cAlt)
{
    cIcon = "";
    cAlt = "";
    int nActionId = 0xc;
    if (pNode) {
        nActionId = pNode->m_nActionId;
    }
    switch (nActionId) {
    case 0xc: {
        unsigned char nType = pAction->m_nActionType;
        int nFeat = pAction->m_nFeat;
        switch (nType) {
        case 1:
            cIcon = "i_attack";
            cAlt = "i_attackm";
            return;
        case 6:
            cIcon = "i_equip";
            cAlt = "i_equipm";
            return;
        case 7:
            cIcon = "i_unequip";
            cAlt = "i_equipm";
            return;
        case 10: {
            CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(pAction->m_oidItem);
            if (pItem) {
                cIcon = pItem->GetIcon();
                cAlt = "i_useitemm";
            }
            return;
        }
        case 9: {
            CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpell(pAction->m_nSpellId);
            if (pSpell) {
                cIcon = pSpell->m_cIcon;
                cAlt = "i_powerm";
            }
            return;
        }
        case 11: {
            CSWFeat* pFeat = g_pRules->GetFeat(nFeat);
            if (pFeat) {
                cIcon = pFeat->m_cIcon;
                cAlt = "i_featm";
            }
            return;
        }
        }
        break;
    }
    case 0x3f:
        if (pAction) {
            switch (pAction->m_nActionType) {
            case 1:
                cIcon = "i_attack";
                cAlt = "i_attackm";
                return;
            case 6:
                cIcon = "i_equip";
                cAlt = "i_equipm";
                return;
            case 7:
                cIcon = "i_unequip";
                cAlt = "i_equipm";
                return;
            case 10: {
                CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(pAction->m_oidItem);
                if (pItem) {
                    cIcon = pItem->GetIcon();
                    cAlt = "i_useitemm";
                }
                return;
            }
            case 9: {
                CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpell(pAction->m_nSpellId);
                if (pSpell) {
                    cIcon = pSpell->m_cIcon;
                    cAlt = "i_powerm";
                }
                return;
            }
            case 11: {
                CSWFeat* pFeat = g_pRules->GetFeat(pAction->m_nFeatId);
                if (pFeat) {
                    cIcon = pFeat->m_cIcon;
                    cAlt = "i_featm";
                }
                return;
            }
            }
        }
        else {
            cIcon = "i_attack";
            cAlt = "i_attackm";
        }
        break;
    case 0x2e: {
        CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(pNode->m_nParam0);
        if (pItem) {
            cIcon = pItem->GetIcon();
            cAlt = "i_useitemm";
        }
        break;
    }
    case 0xf:
        cIcon = g_pRules->m_pSpellArray->GetSpell(pNode->m_nParam0)->m_cIcon;
        cAlt = "i_powerm";
        return;
    }
}
