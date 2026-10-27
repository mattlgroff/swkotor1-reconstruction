// CSWCModule::Update (nwcmodule.o): applies a pending clear-view request,
// advances the game clock when not paused, updates the area and renders.

#include "m/sw_c_module_camera.h"

// FUNCTION: SWKOTOR 0x00644650
// NONMATCH (3 diff insns): the original loads both clear-view outputs before
// storing them into the stick target; ours stores the first before loading
// the second (possible alias through the escaped local). Local copies, an
// inline setter and an array local were tried.
void CSWCModule::Update(float fDelta)
{
    if (!m_pArea || !m_pCamera) {
        return;
    }
    if (m_bClearViewRequest) {
        float fTurn;
        float fZoom;
        if (FindClearView(fDelta, &fTurn, &fZoom)) {
            CAurBehavior* pBehavior = m_pCamera->GetBehavior(-1);
            if (pBehavior) {
                CSWCameraTiltTarget* pTarget = pBehavior->GetTiltTarget();
                if (pTarget) {
                    pTarget->m_f3C = fTurn;
                    pTarget->m_f44 = fZoom;
                }
            }
        }
        m_bClearViewRequest = 0;
    }
    if (!g_pAppManager->m_pClientExoApp->m_pInternal->GetActivePauseState()) {
        m_fGameTime += fDelta;
    }
    m_pArea->Update(fDelta);
    if (g_pAppManager->m_pClientExoApp->GetGuiManager()->m_bModalPanel <= 0) {
        m_pCamera->m08();
    }
}
