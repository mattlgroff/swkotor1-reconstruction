// CSWSCreature::SetCombatMode (nwscreatureai.cpp). SetActivity stays out of line here,
// so this lives apart from sws_creature_activity.cpp.
#include "a/sws_creature.h"

// NONMATCH: 116 diff: the activity if-chain blocks and feedback tails are laid out/cross-jumped differently.
// FUNCTION: SWKOTOR 0x0050EE80
void CSWSCreature::SetCombatMode(unsigned char nMode, int bCheckWeapon)
{
    unsigned char nOld;
    if (bCheckWeapon == 1) {
        nOld = m_nCombatMode;
        if (nMode == 5) {
            if (m_pInventory->GetItemInSlot(0x10)) {
                SendFeedbackMessage(0xbc, 0);
                return;
            }
        } else if (nMode == 6) {
            CSWSItem* pWeapon = m_pInventory->GetItemInSlot(0x10);
            if (!pWeapon || g_pRules->GetBaseItem(pWeapon->m_nBaseItem)->m_n1A <= 0) {
                SendFeedbackMessage(0xbc, 0);
                return;
            }
        } else if (nMode == 2 || nMode == 3 || nMode == 1) {
            CSWSItem* pWeapon = m_pInventory->GetItemInSlot(0x10);
            if (pWeapon && g_pRules->GetBaseItem(pWeapon->m_nBaseItem)->m_n1A > 0) {
                SendFeedbackMessage(0xbc, 0);
                return;
            }
        }
    } else if (nMode == 4 && m_nCombatMode != 4) {
        nOld = m_nCombatMode;
    } else {
        if (nMode == m_nCombatMode) return;
        if (m_pCombatRound->m_bRoundStarted == 1) {
            m_nPendingCombatMode = nMode;
            return;
        }
        if (nMode == 5) {
            if (m_pInventory->GetItemInSlot(0x10)) {
                SendFeedbackMessage(0xbc, 0);
                return;
            }
        } else if (nMode == 6) {
            CSWSItem* pWeapon = m_pInventory->GetItemInSlot(0x10);
            if (!pWeapon || g_pRules->GetBaseItem(pWeapon->m_nBaseItem)->m_n1A <= 0) {
                SendFeedbackMessage(0xbc, 0);
                return;
            }
        }
        nOld = m_nCombatMode;
    }
    m_nCombatMode = nMode;
    if (nMode == 1) {
        SetActivity(0x400, 1, 0);
        return;
    }
    if (nOld == 1) {
        SetActivity(0x400, 0, 0);
        return;
    }
    if (nMode == 2) {
        SetActivity(0x100, 1, 0);
        return;
    }
    if (nOld == 2) {
        SetActivity(0x100, 0, 0);
        return;
    }
    if (nMode == 3) {
        SetActivity(0x200, 1, 0);
        return;
    }
    if (nOld == 3) {
        SetActivity(0x200, 0, 0);
        return;
    }
    if (nMode == 4) {
        SetActivity(0x800, 1, 0);
        return;
    }
    if (nOld == 4) {
        SetActivity(0x800, 0, 0);
        return;
    }
    if (nMode == 5) {
        SetActivity(0x1000, 1, 0);
        return;
    }
    if (nOld == 5) {
        SetActivity(0x1000, 0, 0);
        return;
    }
    if (nMode == 6) {
        SetActivity(0x2000, 1, 0);
        return;
    }
    if (nOld == 6) {
        SetActivity(0x2000, 0, 0);
        return;
    }
}
