// CSWGuiPazaakGame::Draw and the hand-card callback (swguipazaak.o). Draw
// counts the end-of-round timer down or runs the game sequence while the
// panel is on top.
#include "k/sw_gui_pazaak.h"

// FUNCTION: SWKOTOR 0x00680710
void CSWGuiPazaakGame::Draw(float fDelta)
{
    if (m_fTimer >= 0.0f) {
        m_fTimer -= fDelta;
        if (m_fTimer < 0.0f) {
            RefreshDisplay();
        }
    }
    else {
        while (m_pManager->IsPanelOnTop(this) && DoGameSequence()) {
        }
    }
    CSWGuiPanel::Draw(fDelta);
}

// Not in the function table: event 0x27 callback of the hand cards; plays
// or flips the card the hand mode selected.
// FUNCTION: SWKOTOR 0x00680790
void CSWGuiPazaakGame::HandleHandCardAction(CSWGuiControl* pControl)
{
    if (pControl && !pControl->m_n4c) {
        return;
    }
    if (m_nHandMode == 1) {
        HandlePlayHandCard(pControl->m_nId);
    }
    else if (m_nHandMode == 2) {
        HandleFlipHandCard();
    }
    m_nHandMode = 0;
    SetHandEnabled(0);
}
