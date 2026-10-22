// CGuiInGame leaves in bucket n.

#include "n/gui_in_game_n.h"

// FUNCTION: SWKOTOR 0x0062B050
void CGuiInGame::UpdateCombatMode()
{
    if (m_pMainInterface) {
        m_pMainInterface->UpdateActions();
    }
}
