// CSWCModule::RestoreCamera: reinstalls the camera saved before a shot,
// free-look, combat or navigate camera took over.

#include "m/sw_c_module_camera.h"

// FUNCTION: SWKOTOR 0x00641DC0
// NONMATCH: the original keeps a separate EH epilogue for the combat-mode
// return; MSVC7 merges it into the final return here (50 diff insns, mostly
// the resulting 15-byte shift and ebx/ebp swap).
int CSWCModule::RestoreCamera()
{
    if (g_pAppManager->m_pClientExoApp->GetCombatMode()) {
        EnterCombatMode();
        CSWBehaviorCameraCombat* pCombat = (CSWBehaviorCameraCombat*)m_pCamera->GetBehavior(0x1070);
        if (pCombat)
            pCombat->RestoreSettings();
        return 1;
    }
    if (!m_cSavedCamera.m_nValid) {
        g_pAppManager->m_pClientExoApp->GetClientOptions()->SetCameraMode(3);
        return 0;
    }

    Vector vPosition(0.0f, 0.0f, 0.0f);
    Quaternion qOrientation;
    if (m_pCamera->GetBehavior(-1)) {
        vPosition = m_pCamera->GetBehavior(-1)->m_pGob->GetPosition();
        qOrientation = m_pCamera->GetBehavior(-1)->m_pGob->GetOrientation();
    }
    int bKeepOrientation = 0;
    int bResetPosition = 0;
    if (m_pCamera->GetBehavior(-1) && m_pCamera->GetBehavior(-1)->GetType() == 0x106d)
        bKeepOrientation = 1;
    if (m_pCamera->GetBehavior(-1) && m_pCamera->GetBehavior(-1)->GetType() == 0x106e)
        bKeepOrientation = 1;
    if (m_pCamera->GetBehavior(-1) && m_pCamera->GetBehavior(-1)->GetType() == 0x1070)
        bResetPosition = 1;

    if ((m_nCameraMode != 3 && m_nCameraMode != 5 && m_nCameraMode != 7) || !m_cSavedCamera.m_nValid)
        return 0;

    if (!m_cSavedCamera.m_rTarget.m_p) {
        CSWCCreature* pCreature = g_pAppManager->m_pClientExoApp->GetParty()->GetCharacter(0);
        if (pCreature) {
            if (m_cSavedCamera.m_nType == 1) {
                CSWCameraFreeLook* pFreeLook = new CSWCameraFreeLook(m_pCamera->GetGob(), pCreature->GetGob(),
                                                                     pCreature->GetFreeLookVideoEffect());
                m_pCamera->SetBehavior(pFreeLook);
            }
            else if (m_cSavedCamera.m_nType == 0) {
                CSWCameraOnAStick* pStick = new CSWCameraOnAStick(m_pCamera->GetGob(), pCreature->GetGob());
                pStick->m_nFollowBody = pCreature->GetFollowBody();
                pStick->m_fFollowHeight = pCreature->GetHeadHeight() + pCreature->GetCameraOffset();
                if (bResetPosition)
                    pStick->ResetLastPosition(vPosition, qOrientation);
                pStick->m_bKeepOrientation = bKeepOrientation;
                m_pCamera->SetBehavior(pStick);
            }
        }
    }
    else if (m_cSavedCamera.m_nType == 1) {
        CSWCameraFreeLook* pFreeLook = new CSWCameraFreeLook(&m_cSavedCamera);
        m_pCamera->SetBehavior(pFreeLook);
        m_nCameraMode = 5;
    }
    else if (m_cSavedCamera.m_nType == 0) {
        CSWCCreature* pCreature = g_pAppManager->m_pClientExoApp->GetParty()->GetCharacter(0);
        if (pCreature) {
            m_cSavedCamera.m_rTarget = pCreature->GetGob();
            m_cSavedCamera.m_nFollowBody = pCreature->GetFollowBody();
            m_cSavedCamera.m_fFollowHeight = pCreature->GetHeadHeight() + pCreature->GetCameraOffset();
        }
        CSWCameraOnAStick* pStick = new CSWCameraOnAStick(&m_cSavedCamera);
        if (bResetPosition)
            pStick->ResetLastPosition(vPosition, qOrientation);
        pStick->m_bKeepOrientation = bKeepOrientation;
        m_pCamera->SetBehavior(pStick);
        m_nCameraMode = 3;
    }

    if (m_pCameraModel) {
        delete m_pCameraModel;
        m_pCameraModel = 0;
    }
    g_pAppManager->m_pClientExoApp->GetClientOptions()->SetCameraMode((unsigned char)m_nCameraMode);
    return 1;
}
