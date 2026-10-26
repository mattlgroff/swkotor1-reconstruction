// CSWCModule camera mode switches: shot (dialogue), free-look, combat and
// navigate cameras. Each saves the on-a-stick camera first when the options
// select it, then installs a newly allocated behaviour.

#include "m/sw_c_module_camera.h"

// FUNCTION: SWKOTOR 0x006412F0
void CSWCModule::InitializeShotCamera(unsigned long nShot)
{
    unsigned char nMode = g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nCameraMode;
    if (nMode == 3) {
        CSWCameraOnAStick* pStick = (CSWCameraOnAStick*)m_pCamera->GetBehavior(0x106a);
        if (pStick)
            pStick->SaveCameraSettings(&m_cSavedCamera);
    }
    CSWBehaviorCameraDialog* pBehavior = new CSWBehaviorCameraDialog(m_pCamera->GetGob(), nShot);
    g_pAppManager->m_pClientExoApp->GetClientOptions()->SetCameraMode(4);
    m_pCamera->SetBehavior(pBehavior);
}

// Horizontal direction from one point to another (z dropped).
static inline Vector GroundDirection(const Vector& vFrom, const Vector& vTo)
{
    return Vector(vTo.x - vFrom.x, vTo.y - vFrom.y, 0.0f);
}

// FUNCTION: SWKOTOR 0x006413C0
int CSWCModule::SetFreeLookCamera(unsigned long oidTarget)
{
    CClientExoApp* pApp = g_pAppManager->m_pClientExoApp;
    CGameObject* pObject = pApp->GetGameObject(oidTarget);
    if (!pObject)
        return 0;
    CSWCCreature* pCreature = pObject->AsSWCCreature();
    if (!pCreature)
        return 0;

    unsigned char nMode = g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nCameraMode;
    if (nMode == 3) {
        CSWCameraOnAStick* pStick = (CSWCameraOnAStick*)m_pCamera->GetBehavior(0x106a);
        if (pStick) {
            pStick->SaveCameraSettings(&m_cSavedCamera);
            Vector vDirection = GroundDirection(pStick->GetCameraPosition(), pCreature->m_vPosition);
            pCreature->SetOrientation(Normalize(vDirection));
        }
    }

    CSWCameraFreeLook* pFreeLook = new CSWCameraFreeLook(m_pCamera->GetGob(), pCreature->GetGob(),
                                                         pCreature->GetFreeLookVideoEffect());
    g_pAppManager->m_pClientExoApp->GetClientOptions()->SetCameraMode(5);
    m_pCamera->SetBehavior(pFreeLook);
    m_nCameraMode = 5;
    return 1;
}

// FUNCTION: SWKOTOR 0x00641540
int CSWCModule::EnterCombatMode()
{
    unsigned char nMode = g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nCameraMode;
    if (nMode == 3) {
        CSWCameraOnAStick* pStick = (CSWCameraOnAStick*)m_pCamera->GetBehavior(0x106a);
        if (pStick)
            pStick->SaveCameraSettings(&m_cSavedCamera);
    }
    CSWBehaviorCameraCombatMode* pCombat = new CSWBehaviorCameraCombatMode(m_pCamera->GetGob());
    g_pAppManager->m_pClientExoApp->GetClientOptions()->SetCameraMode(6);
    m_pCamera->SetBehavior(pCombat);
    return 1;
}

// FUNCTION: SWKOTOR 0x00641610
int CSWCModule::SetNavigateCamera()
{
    unsigned char nMode = g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nCameraMode;
    if (nMode == 3) {
        CSWCameraOnAStick* pStick = (CSWCameraOnAStick*)m_pCamera->GetBehavior(0x106a);
        if (pStick)
            pStick->SaveCameraSettings(&m_cSavedCamera);
        else
            return 0;
    }
    CSWCameraNavigateMode* pNavigate = new CSWCameraNavigateMode(m_pCamera->GetGob());
    g_pAppManager->m_pClientExoApp->GetClientOptions()->SetCameraMode(7);
    m_pCamera->SetBehavior(pNavigate);
    return 1;
}

// FUNCTION: SWKOTOR 0x006419A0
void CSWCModule::UpdateCameraFollow(float fDelta)
{
    int nMode = g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nCameraMode;
    if (nMode == 4 || m_bClearViewRequest || !m_pCamera)
        return;

    if (nMode != m_nCameraMode) {
        CClientExoApp* pApp = g_pAppManager->m_pClientExoApp;
        CGameObject* pObject = pApp->GetGameObject(m_oidFollow);
        if (!pObject)
            return;
        CSWCObject* pClientObject = pObject->AsSWCObject();
        if (!pClientObject)
            return;
        Gob* pModel = pClientObject->GetModel(0xff, 1);
        if (!pModel)
            return;

        if (m_nCameraMode == 3)
            m_pCamera->SetBehavior(0);

        switch (nMode) {
        case 3: {
            CSWCCreature* pCreature = pObject->AsSWCCreature();
            CSWCameraOnAStick* pStick = new CSWCameraOnAStick(m_pCamera->GetGob(), pCreature->GetGob());
            pStick->m_nFollowBody = pCreature->GetFollowBody();
            pStick->m_fFollowHeight = pCreature->GetHeadHeight() + pCreature->GetCameraOffset();
            m_pCamera->SetBehavior(pStick);
            break;
        }
        case 5: {
            CSWCCreature* pCreature = pObject->AsSWCCreature();
            CSWCameraFreeLook* pFreeLook = new CSWCameraFreeLook(m_pCamera->GetGob(), pCreature->GetGob(),
                                                                 pCreature->GetFreeLookVideoEffect());
            m_pCamera->SetBehavior(pFreeLook);
            break;
        }
        case 7: {
            CSWCameraNavigateMode* pNavigate = new CSWCameraNavigateMode(m_pCamera->GetGob());
            m_pCamera->SetBehavior(pNavigate);
            break;
        }
        default:
            m_pCamera->SetFollow(pModel);
            break;
        }
        m_nCameraMode = nMode;
    }

    if (m_nLastCameraOption != g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nCameraMode)
        m_bResetCameraParams = 1;

    switch (g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nCameraMode) {
    case 0:
        UpdateCameraModeChaseCam();
        break;
    case 1:
        UpdateCameraModeBG();
        break;
    case 2:
        UpdateCameraModeEQ(fDelta);
        break;
    case 3:
    case 4:
        return;
    }

    if (g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nCameraMode == 0
        || g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nCameraMode == 1) {
        CAurBehavior* pBehavior = m_pCamera->GetBehavior(-1);
        if (pBehavior)
            pBehavior->GetTiltTarget();
    }
    m_fSavedDistance = m_pCamera->GetDistance();
    m_fSavedYaw = m_pCamera->GetYaw();
    m_fSavedPitch = m_pCamera->GetPitch();
    m_nLastCameraOption = g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nCameraMode;
}
