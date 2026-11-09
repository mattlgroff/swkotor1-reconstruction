// Load-bar progress callback (0x004B30D0, placeholder name): a cdecl
// (current, total, base, span) callback in the CServerExoAppInternal object
// file. Hidden start inside functions.csv's 0x004B3070 entry.
#include "i/sws_module.h"

// FUNCTION: SWKOTOR 0x004B30D0
int LoadBarProgressCallback(int nCurrent, int nTotal, int nBase, int nSpan)
{
    g_pAppManager->m_pClientExoApp->SetLoadBarProgress(
        (int)((float)nCurrent / nTotal * nSpan) + nBase, 0);
    RunFrame(1.0f / 30.0f, 0, 0);
    return 0;
}
