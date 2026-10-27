// CSWGuiTargetActionMenu constructor (CSWGuiMainInterface +0xbc).
#include "k/sw_gui_main_interface.h"

// FUNCTION: SWKOTOR 0x0068BE40
CSWGuiTargetActionMenu::CSWGuiTargetActionMenu()
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            m_anSelected[i][j] = -1;
        }
    }
    m_nSelectedRow = -1;
    m_bShown = 0;
    m_bMouseOver = 0;
    m_fNameTimer = -1.0f;
    m_fHiliteTimer = -1.0f;
    m_nSelectedMenu = 0;
}
