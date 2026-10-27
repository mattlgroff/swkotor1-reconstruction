// Worker A round s7: CSWSCreature::SetActivity (nwscreature.cpp). Kept apart
// from ClearActivities: in one translation unit cl changes the caller.
#include "a/sws_creature.h"

// FUNCTION: SWKOTOR 0x004F00A0
void CSWSCreature::SetActivity(int nActivity, int bOn, int bArg)
{
    if ((m_nA00 & nActivity) == (unsigned long)nActivity) {
        return;
    }
    if (((nActivity & 0xf8) && bOn == 1) || (nActivity == 0xf8 && bOn == 0)) {
        if ((m_n9FC & 0x8) && !(m_nA00 & 0x8)) {
            SetActivity(0x8, 0, 0);
        }
        if ((m_n9FC & 0x10) && !(m_nA00 & 0x10)) {
            SetActivity(0x10, 0, 0);
        }
        if ((m_n9FC & 0x20) && !(m_nA00 & 0x20)) {
            SetActivity(0x20, 0, 0);
        }
        if ((m_n9FC & 0x40) && !(m_nA00 & 0x40)) {
            SetActivity(0x40, 0, 0);
        }
        if ((m_n9FC & 0x80) && !(m_nA00 & 0x80)) {
            SetActivity(0x80, 0, 0);
        }
    }
    if (((nActivity & 0x3f00) && bOn == 1) || (nActivity == 0x3f00 && bOn == 0)) {
        if ((m_n9FC & 0x100) && !(m_nA00 & 0x100)) {
            SetActivity(0x100, 0, 0);
        }
        if ((m_n9FC & 0x200) && !(m_nA00 & 0x200)) {
            SetActivity(0x200, 0, 0);
        }
        if ((m_n9FC & 0x400) && !(m_nA00 & 0x400)) {
            SetActivity(0x400, 0, 0);
        }
        if ((m_n9FC & 0x800) && !(m_nA00 & 0x800)) {
            SetActivity(0x800, 0, 0);
        }
        if ((m_n9FC & 0x1000) && !(m_nA00 & 0x1000)) {
            SetActivity(0x1000, 0, 0);
        }
        if ((m_n9FC & 0x2000) && !(m_nA00 & 0x2000)) {
            SetActivity(0x2000, 0, 0);
        }
    }
    int bChanged;
    if (bOn == 1) {
        bChanged = !(m_n9FC & nActivity);
        m_n9FC |= nActivity;
    }
    else {
        bChanged = m_n9FC & nActivity;
        m_n9FC &= ~nActivity;
    }
    if (!bChanged) {
        return;
    }
    if (nActivity & 1) {
        if (bOn == 1 && !(m_n9FC & 0xc)) {
            SetStealthMode((unsigned char)bOn, bArg);
        }
        else if (bOn == 0) {
            SetStealthMode((unsigned char)bOn, bArg);
        }
        else {
            m_n9FC &= ~nActivity;
        }
    }
    if (nActivity & 2) {
        SetDetectMode(bOn != 0);
    }
    if ((nActivity & 4) && bOn == 0) {
        StopDialog();
    }
}

// FUNCTION: SWKOTOR 0x004F2A50
int CSWSCreature::ToggleMode(unsigned char nMode)
{
    if (GetDead()) {
        return 0;
    }
    if (GetIsPossessed()) {
        return 0;
    }
    if (GetDeadTemp()) {
        return 0;
    }
    if (nMode == 1) {
        if (!GetStats()->GetCanUseSkill(2)) {
            return 1;
        }
        if (m_nStealthMode == 1) {
            SetActivity(1, 0, 0);
        }
        else if (m_b4E0) {
            SendFeedbackMessage(0x3c, NULL);
        }
        else {
            SetActivity(1, 1, 0);
        }
    }
    else if (nMode == 2) {
        if (m_nCombatMode == 1) {
            SetCombatMode(0, 1);
        }
        else {
            SetCombatMode(1, 0);
        }
    }
    else if (nMode == 3) {
        if (m_nCombatMode == 2) {
            SetCombatMode(0, 1);
        }
        else {
            SetCombatMode(2, 0);
        }
    }
    else if (nMode == 4) {
        if (m_nCombatMode == 3) {
            SetCombatMode(0, 1);
        }
        else {
            SetCombatMode(3, 0);
        }
    }
    else if (nMode == 5) {
        if (m_nCombatMode == 4) {
            SetCombatMode(0, 1);
        }
        else {
            SetCombatMode(4, 0);
        }
    }
    else if (nMode == 6) {
        if (m_nCombatMode == 5) {
            SetCombatMode(0, 1);
        }
        else {
            SetCombatMode(5, 0);
        }
    }
    else if (nMode == 7) {
        if (m_nCombatMode == 6) {
            SetCombatMode(0, 1);
        }
        else {
            SetCombatMode(6, 0);
        }
    }
    return 1;
}
