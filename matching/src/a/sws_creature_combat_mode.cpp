// CSWSCreature::SetCombatMode (nwscreatureai.cpp). SetActivity stays out of line here,
// so this lives apart from sws_creature_activity.cpp.
#include "a/sws_creature.h"

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
            if (!pWeapon) {
                SendFeedbackMessage(0xbc, 0);
                return;
            }
            if (g_pRules->GetBaseItem(pWeapon->m_nBaseItem)->m_n1A <= 0) {
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
        m_nCombatMode = nMode;
        goto Activity;
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
Activity:
    if (nMode != 1 && nOld != 1) {
        if (nMode != 2 && nOld != 2) {
            if (nMode != 3 && nOld != 3) {
                if (nMode != 4 && nOld != 4) {
                    if (nMode != 5 && nOld != 5) {
                        if (nMode != 6) {
                            if (nOld == 6) {
                                SetActivity(0x2000, 0, 0);
                            }
                        } else {
                            SetActivity(0x2000, 1, 0);
                        }
                    } else if (nMode == 5) {
                        SetActivity(0x1000, 1, 0);
                    } else {
                        SetActivity(0x1000, 0, 0);
                    }
                } else if (nMode == 4) {
                    SetActivity(0x800, 1, 0);
                } else {
                    SetActivity(0x800, 0, 0);
                }
            } else if (nMode == 3) {
                SetActivity(0x200, 1, 0);
            } else {
                SetActivity(0x200, 0, 0);
            }
        } else if (nMode == 2) {
            SetActivity(0x100, 1, 0);
        } else {
            SetActivity(0x100, 0, 0);
        }
    } else if (nMode == 1) {
        SetActivity(0x400, 1, 0);
    } else {
        SetActivity(0x400, 0, 0);
    }
}
