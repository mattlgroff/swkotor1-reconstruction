// CSWCModule camera controls (nwcmodule.o) that drive the camera behaviours.

#include "n/sw_behavior_camera.h"

// FUNCTION: SWKOTOR 0x0063FCC0
void CSWCModule::TiltCamera(float fAmount)
{
    if (m_nCameraMode == 3) {
        CSWCameraOnAStick* pStick = (CSWCameraOnAStick*)m_pCamera->GetBehavior(0x106a);
        if (pStick)
            pStick->m_fTiltSpeed = fAmount;
    }
    else if (m_nCameraMode == 5) {
        CSWCameraFreeLook* pFreeLook = (CSWCameraFreeLook*)m_pCamera->GetBehavior(0x106e);
        if (pFreeLook)
            pFreeLook->m_f38 = fAmount;
    }
    else if (m_nCameraMode == 7) {
        CSWCameraNavigate* pNavigate = (CSWCameraNavigate*)m_pCamera->GetBehavior(0x1071);
        if (pNavigate)
            pNavigate->m_fTilt = fAmount * -60.0f;
    }
    else {
        if (m_pCamera) {
            CAurBehavior* pBehavior = m_pCamera->GetBehavior(-1);
            if (pBehavior) {
                CSWCameraTiltTarget* pTarget = pBehavior->GetTiltTarget();
                if (pTarget)
                    m_fCameraTilt = pTarget->m_fTilt += fAmount;
            }
        }
        m_nCameraFlags |= fAmount != 0.0f ? 1 : 0;
    }
}

// FUNCTION: SWKOTOR 0x0063FDB0
void CSWCModule::AcclTiltCamera(float fAmount)
{
    if (m_nCameraMode == 3) {
        m_pCamera->GetBehavior(0x106a);
    }
    else if (m_nCameraMode == 5) {
        CSWCameraFreeLook* pFreeLook = (CSWCameraFreeLook*)m_pCamera->GetBehavior(0x106e);
        if (pFreeLook)
            pFreeLook->m_f24 = -fAmount;
    }
    else if (m_nCameraMode == 7) {
        m_pCamera->GetBehavior(0x1071);
    }
}
