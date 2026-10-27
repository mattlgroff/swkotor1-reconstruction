// Worker A round s7: CSWSCreature::ActivityManager (nwscreature.cpp). Kept
// apart from SetActivity, which cl would otherwise inline here; Rest too.
#include "a/sws_creature.h"

extern const unsigned long EVENT_0x7441DC;      // 0x007441DC (placeholder name)

// FUNCTION: SWKOTOR 0x004F87D0
void CSWSCreature::ActivityManager(int nAction)
{
    if (nAction != 4) {
        if ((m_nA00 & 0x3f00) != 0x3f00) {
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
            m_n9FC &= ~0x3f00;
        }
        if (nAction == 1) {
            if ((m_n9FC & 1) && !(m_nA00 & 1)) {
                unsigned long bChanged = m_n9FC & 1;
                m_n9FC &= ~1;
                if (bChanged) {
                    SetStealthMode(0, 0);
                }
            }
            if ((m_n9FC & 4) && !(m_nA00 & 4)) {
                unsigned long bChanged = m_n9FC & 4;
                m_n9FC &= ~4;
                if (bChanged) {
                    StopDialog();
                }
            }
            if ((m_n9FC & 0x8) && !(m_nA00 & 0x8)) {
                m_n9FC &= ~0x8;
            }
            if ((m_n9FC & 0x10) && !(m_nA00 & 0x10)) {
                m_n9FC &= ~0x10;
            }
            if ((m_n9FC & 0x20) && !(m_nA00 & 0x20)) {
                m_n9FC &= ~0x20;
            }
            if ((m_n9FC & 0x40) && !(m_nA00 & 0x40)) {
                m_n9FC &= ~0x40;
            }
            if ((m_n9FC & 0x80) && !(m_nA00 & 0x80)) {
                m_n9FC &= ~0x80;
            }
        }
    }
    else {
        if ((m_n9FC & 1) && !(m_nA00 & 1)) {
            unsigned long bChanged = m_n9FC & 1;
            m_n9FC &= ~1;
            if (bChanged) {
                SetStealthMode(0, 0);
            }
        }
        if ((m_n9FC & 4) && !(m_nA00 & 4)) {
            unsigned long bChanged = m_n9FC & 4;
            m_n9FC &= ~4;
            if (bChanged) {
                StopDialog();
            }
        }
        if ((m_n9FC & 0x8) && !(m_nA00 & 0x8)) {
            m_n9FC &= ~0x8;
        }
    }
}

// FUNCTION: SWKOTOR 0x004FD1E0
void CSWSCreature::Rest(int)
{
    CExoString sUnused;
    int nZero = 0;
    if (GetDead()) {
        return;
    }
    if (GetIsPossessed()) {
        return;
    }
    int bParty = m_b9D4;
    if (bParty && GetArea() && GetArea()->m_bNoRest) {
        SendFeedbackMessage(0x36, NULL);
        return;
    }
    if (GetNearestEnemy(30.0f, 0x7f000000, 0x7f000000) != 0x7f000000) {
        SendFeedbackMessage(0xba, NULL);
        return;
    }
    ClearAllActions(1);
    if (bParty && m_nExcitedTimer > 0) {
        SendFeedbackMessage(0x11, NULL);
        return;
    }
    m_n994 = 0;
    m_n998 = 0;
    m_n99C = 0;
    m_n9A0 = 0;
    ClearActivities(0xf);
    if (!(m_nA00 & 8)) {
        if (m_n9FC & 8) {
            SetActivity(8, 0, 0);
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
        m_n9FC |= 8;
    }
    if (GetServerExoApp()->GetClientObjectByObjectId(m_idSelf)) {
        Fn4F1460();
        ClearAllActions(1);
    }
    if (bParty) {
        SendFeedbackMessage(0x12, NULL);
        CSWSModule* pModule = GetServerExoApp()->GetModule6B0();
        CScriptEvent* pEvent = new CScriptEvent;
        pEvent->m_nType = 0x23;
        pEvent->SetInteger(0, 1);
        GetServerExoApp()->GetServerAIMaster()->AddEventAbsoluteTime(0, 0, m_idSelf, pModule->m_idSelf,
                                                                     EVENT_0x7441DC, pEvent);
    }
    AddAction(0x2a, 0xffff, 1, &bParty, 1, &nZero);
}
