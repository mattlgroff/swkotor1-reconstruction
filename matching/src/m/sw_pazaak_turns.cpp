// Pazaak rules object CSWPazaak: the two turn helpers the game panel calls
// (0x0067CAE0, 0x0067CB10); the rest of the class lives at 0x006E4xxx.

#include "m/sw_gui_pazaak.h"

// FUNCTION: SWKOTOR 0x0067CAE0
void CSWPazaak::DrawPlayerCard()
{
    m_cPlayer.AddGameCard(DrawCard());
}

// FUNCTION: SWKOTOR 0x0067CB10
int CSWPazaak::DoAITurn()
{
    m_nAIState = PlayAITurn(&m_cAI, &m_cPlayer, m_nAIState);
    return m_nAIState;
}
