// CSWCModule follow-camera updates for the chase (option 0) and EQ
// (option 2) camera styles.

#include "m/sw_c_module_camera.h"

extern "C" double __cdecl fabs(double x);
#pragma intrinsic(fabs)

extern float g_fCameraTurnThreshold;                // 0x007A2460
extern float g_fCameraTurnScale;                    // 0x007A2464

// FUNCTION: SWKOTOR 0x00640700
void CSWCModule::UpdateCameraModeChaseCam()
{
    CAurBehavior* pBehavior = m_pCamera->GetBehavior(-1);
    CSWCameraTiltTarget* pStick = pBehavior ? pBehavior->GetTiltTarget() : 0;
    Scene* pScene = 0;
    if (m_pArea)
        pScene = m_pArea->m_pScene;
    CSWCCreature* pPlayer = g_pAppManager->m_pClientExoApp->GetPlayerCreature();
    if (!pScene || !pPlayer)
        return;

    if (m_bResetCameraParams) {
        CAurBehavior* pCurrent = m_pCamera->GetBehavior(-1);
        CSWCameraTiltTarget* pTarget = pCurrent ? pCurrent->GetTiltTarget() : 0;
        if (pTarget) {
            pTarget->m_f68 = 5.0f;
            pTarget->m_f6C = 20.0f;
            pTarget->m_f5C = 1.0f;
            pTarget->m_f60 = 50.0f;
            g_fCameraFollowA = 5.0f;
            g_fCameraFollowB = 20.0f;
            g_fCameraFollowC = 1.0f;
            g_fCameraFollowPitch = 50.0f;
            pTarget->m_vUp = m_vCameraUp = Vector(0.0f, 0.0f, 1.0f);

            float fNear;
            float fFar;
            m_pCamera->GetClipPlanes(&fNear, &fFar);
            m_pCamera->SetClipPlanes(fNear, 45.0f);
            pTarget->m_f44 = 50.0f;
            pTarget->m_f30 = 20.0f;
            m_bResetCameraParams = 0;
        }
    }

    if (pPlayer->GetCurrentAnimation() != 10002 && pPlayer->GetCurrentAnimation() != 10003
        && pPlayer->GetCurrentAnimation() != 10004 && pPlayer->GetCurrentAnimation() != 10078
        && pPlayer->GetCurrentAnimation() != 10079 && pPlayer->GetCurrentAnimation() != 10084
        && pPlayer->GetCurrentAnimation() != 10085 && pPlayer->GetCurrentAnimation() != 10086
        && pPlayer->GetCurrentAnimation() != 10087) {
        m_vFollowPosition = pPlayer->m_vPosition;
        m_bFollowValid = 1;
    }
    else if (m_bFollowValid) {
        m_n88 = 0;
        m_bFollowValid = 0;
    }

    unsigned long nFlags = m_nCameraFlags;
    if (nFlags) {
        m_n88 = 1;
        m_n84 = 0;
    }
    if (m_n88 || nFlags)
        return;

    float fCameraYaw = GetPitch(m_pCamera->GetOrientation());
    float fAngle = GetFacing(&pPlayer->m_vOrientation) - fCameraYaw;
    while (fAngle < -180.0f)
        fAngle += 360.0f;
    while (fAngle > 180.0f)
        fAngle -= 360.0f;
    if (!pPlayer->m_bIgnoreCameraClamp) {
        if (fAngle > 90.0f && m_n90 == 2)
            fAngle = -90.0f;
        else if (fAngle < -90.0f && m_n90 == 1)
            fAngle = 90.0f;
    }
    if (fabs(fAngle) > 1.0) {
        if ((fabs(fAngle) > g_fCameraTurnThreshold && !m_n84) || m_n84) {
            if (fAngle > 90.0f)
                fAngle = 90.0f;
            else if (fAngle < -90.0f)
                fAngle = -90.0f;
            if (pStick)
                pStick->m_fTurn = g_fCameraTurnScale * fAngle;
            m_nCameraFlags = 1;
            m_n84 = 1;
        }
    }
    else {
        m_n84 = 0;
    }
}

// FUNCTION: SWKOTOR 0x00640B90
// NONMATCH: stack-slot tie only (5 shown diff insns): the original packs the
// spilled player pointer between the far and near clip outputs.
void CSWCModule::UpdateCameraModeEQ(float fDelta)
{
    Scene* pScene = 0;
    if (m_pArea)
        pScene = m_pArea->m_pScene;
    CSWCCreature* pPlayer = g_pAppManager->m_pClientExoApp->GetPlayerCreature();
    if (!pScene || !pPlayer)
        return;

    CAurBehavior* pBehavior = m_pCamera->GetBehavior(-1);
    CSWCameraTiltTarget* pStick = pBehavior ? pBehavior->GetTiltTarget() : 0;
    if (m_bResetCameraParams && pStick) {
        pStick->m_f5C = 1.0f;
        pStick->m_f60 = 62.0f;
        pStick->m_f68 = 6.0f;
        pStick->m_f6C = 6.0f;
        g_fCameraFollowA = 6.0f;
        g_fCameraFollowB = 6.0f;
        g_fCameraFollowC = 1.0f;
        g_fCameraFollowPitch = 62.0f;
        pStick->m_vUp = m_vCameraUp = Vector(0.0f, 0.0f, 2.0f);

        float fNear;
        float fFar;
        m_pCamera->GetClipPlanes(&fNear, &fFar);
        m_pCamera->SetClipPlanes(fNear, 45.0f);
        pStick->m_f30 = 6.0f;
        pStick->m_f44 = 62.0f;
        m_bResetCameraParams = 0;
    }

    if (pPlayer->GetCurrentAnimation() != 10002 && pPlayer->GetCurrentAnimation() != 10003
        && pPlayer->GetCurrentAnimation() != 10004 && pPlayer->GetCurrentAnimation() != 10078
        && pPlayer->GetCurrentAnimation() != 10079 && pPlayer->GetCurrentAnimation() != 10084
        && pPlayer->GetCurrentAnimation() != 10085 && pPlayer->GetCurrentAnimation() != 10086
        && pPlayer->GetCurrentAnimation() != 10087) {
        m_vFollowPosition = pPlayer->m_vPosition;
        m_bFollowValid = 1;
    }
    else if (m_bFollowValid) {
        m_n88 = 0;
        m_bFollowValid = 0;
    }

    unsigned long nFlags = m_nCameraFlags;
    if (nFlags) {
        m_n88 = 1;
        m_n84 = 0;
    }
    if (m_n88 || nFlags)
        return;

    float fCameraYaw = m_pCamera->GetYaw();
    float fPlayerYaw = GetFacing(&pPlayer->m_vOrientation);
    float fAngle = fPlayerYaw - fCameraYaw;
    float fRawAngle = fAngle;
    while (fAngle < -180.0f)
        fAngle += 362.0f;
    while (fAngle > 180.0f)
        fAngle -= 362.0f;
    if (!pPlayer->m_bIgnoreCameraClamp) {
        if (fAngle > 90.0f && m_n90 == 2)
            fAngle = -90.0f;
        else if (fAngle < -90.0f && m_n90 == 1)
            fAngle = 90.0f;
    }
    if (fabs(fAngle) > 1.0) {
        float fTurn = fAngle * 5.0f;
        if (fTurn > 180.0f)
            fTurn = 180.0f;
        else if (fTurn < -180.0f)
            fTurn = -180.0f;
        float fRemaining = fPlayerYaw - (fCameraYaw + fTurn * fDelta);
        while (fRemaining < -180.0f)
            fRemaining += 362.0f;
        while (fRemaining > 180.0f)
            fRemaining -= 362.0f;
        if ((fTurn > 0.0001f && fRemaining < 0.0001f) || (fTurn < 0.0001f && fRemaining > 0.0001f))
            fTurn = fRawAngle / fDelta;
        if (pStick)
            pStick->m_fTurn = g_fCameraTurnScale * fTurn;
        m_nCameraFlags = 1;
        m_n84 = 1;
    }
    else {
        m_n84 = 0;
    }
}
