// Pilot 4 arm A: CSWGuiStatusSummary::HandleInputEvent.
#include "pilot4/a/p4a_gui.h"

// FUNCTION: SWKOTOR 0x00625AC0
void CSWGuiStatusSummary::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x27:
        case 0x28:
        case 0x2d:
        case 0x2e:
            SetToDestroy(0);
            g_pAppManager->GetClientExoApp()->SetInputClass(0, 1);
            m_pManager->PlayGuiSound(0);
            if (!(m_nFlags & 1)) {
                g_pAppManager->GetClientExoApp()->SetPausedByCombat(0, 0, 0);
            }
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// Not in the function table: 0x00625AC0's listed extent runs into this one.
// FUNCTION: SWKOTOR 0x00625B50
void CSWGuiStatusSummary::UpdateStatusFlashes()
{
    if (m_bSound0C) {
        m_pManager->PlayGuiSound(0xc);
    }
    if (m_bSound0D) {
        m_pManager->PlayGuiSound(0xd);
    }
    if (m_bSound0E) {
        m_pManager->PlayGuiSound(0xe);
    }
    for (int i = 0; i < 9; i++) {
        int bFlash;
        switch (i) {
        case 0:
            bFlash = m_bFlash0b;
            break;
        case 1:
            bFlash = m_n6C != 0;
            break;
        case 2:
            bFlash = m_n70 != 0;
            break;
        case 3:
            bFlash = m_n74 != 0;
            break;
        case 4:
            bFlash = m_nNew > m_nOld;
            break;
        case 5:
            bFlash = m_nOld > m_nNew;
            break;
        case 7:
            bFlash = m_bFlash7;
            break;
        case 8:
            bFlash = m_bFlash8;
            break;
        default:
            bFlash = 0;
            break;
        }
        if (bFlash) {
            g_pAppManager->GetClientExoApp()->GetGuiInGame()->FlashStatus(i);
        }
    }
    m_bFlash0 = 0;
    m_bFlash1 = 0;
    m_bFlash7 = 0;
    m_bFlash8 = 0;
    m_bFlash0b = 0;
    m_bSound0C = 0;
    m_bSound0D = 0;
    m_bSound0E = 0;
    m_bBit8 = 0;
    m_n6C = 0;
    m_nOld = 0;
    m_nNew = 0;
    m_n70 = 0;
    m_n74 = 0;
}
