#include "c/client_exo_app.h"

// FUNCTION: SWKOTOR 0x0061F940
int CClientExoAppInternal::PerformRButtonDownAction()
{
    CSWGuiManager* pGuiManager = g_pGuiManager;
    g_bSwapMouseButtons = 1;
    return pGuiManager->HandleRMouseDown();
}

// FUNCTION: SWKOTOR 0x0061F960
int CClientExoAppInternal::PerformRButtonUpAction()
{
    CSWGuiManager* pGuiManager = g_pGuiManager;
    g_bSwapMouseButtons = 0;
    return pGuiManager->HandleRMouseUp();
}
