// CSWCModule camera input controls: turn acceleration, zoom, scroll, shake
// and bump requests, and the collision dispatcher.

#include "m/sw_c_module_camera.h"

// FUNCTION: SWKOTOR 0x00640090
void CSWCModule::AcclTurnCamera(float fAmount, float fUnused)
{
    if (g_pAppManager->m_pClientExoApp->GetCombatMode()) {
        CSWBehaviorCameraCombat* pCombat = (CSWBehaviorCameraCombat*)m_pCamera->GetBehavior(0x1070);
        if (pCombat) {
            pCombat->SetTurn(0.0f);
            pCombat->m_fTurnAccel = -fAmount;
        }
    }
    else if (m_nCameraMode == 3) {
        CSWCameraOnAStick* pStick = (CSWCameraOnAStick*)m_pCamera->GetBehavior(0x106a);
        if (pStick) {
            pStick->m_f100 = 0.0f;
            pStick->m_f10C = fAmount;
        }
    }
    else if (m_nCameraMode == 5) {
        if (m_pCamera->GetBehavior(0x106e)) {
            CSWCCreature* pCreature = g_pAppManager->m_pClientExoApp->GetPlayerCreature();
            if (pCreature)
                pCreature->m_fCameraTurnAccel = -fAmount;
        }
    }
    else if (m_nCameraMode == 7) {
        CSWCameraNavigate* pNavigate = (CSWCameraNavigate*)m_pCamera->GetBehavior(0x1071);
        if (pNavigate)
            pNavigate->m_fTurn = fAmount * -60.0f;
    }
    else {
        if (m_pCamera) {
            CAurBehavior* pBehavior = m_pCamera->GetBehavior(-1);
            if (pBehavior) {
                CSWCameraTiltTarget* pTarget = pBehavior->GetTiltTarget();
                if (pTarget)
                    m_fCameraTurn = pTarget->m_fTurn += fAmount;
            }
        }
        if (fAmount != 0.0f) {
            m_bTurnChanged = 1;
            m_nCameraFlags |= 1;
        }
    }
}

// FUNCTION: SWKOTOR 0x006401D0
void CSWCModule::ZoomCamera(float fAmount)
{
    if (m_pCamera) {
        CAurBehavior* pBehavior = m_pCamera->GetBehavior(-1);
        if (pBehavior) {
            CSWCameraTiltTarget* pTarget = pBehavior->GetTiltTarget();
            if (pTarget)
                m_fCameraZoom = pTarget->m_fZoom += fAmount;
        }
    }
    if (fAmount != 0.0f) {
        m_bZoomChanged = 1;
        m_nCameraFlags |= 1;
    }
}

// FUNCTION: SWKOTOR 0x00640240
void CSWCModule::ScrollCamera(Vector vDelta)
{
    Vector vPosition(0.0f, 0.0f, 0.0f);
    if (vDelta.x * vDelta.x + vDelta.y * vDelta.y + vDelta.z * vDelta.z <= 1e-5f)
        return;
    vPosition = m_pCamera->GetPosition();
    vPosition += vDelta;
    m_pCamera->SetPosition(vPosition);
    m_nCameraFlags = 1;
}

// FUNCTION: SWKOTOR 0x00640A80
void CSWCModule::UpdateCameraModeBG()
{
    if (!m_bResetCameraParams)
        return;
    CAurBehavior* pBehavior = m_pCamera->GetBehavior(-1);
    if (!pBehavior)
        return;
    CSWCameraTiltTarget* pTarget = pBehavior->GetTiltTarget();
    if (!pTarget)
        return;

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
    pTarget->m_f30 = 20.0f;
    pTarget->m_f44 = 50.0f;
    m_bResetCameraParams = 0;
}

// FUNCTION: SWKOTOR 0x006416F0
void CSWCModule::ShakeCamera(int nDelay, int nDuration)
{
    if (nDuration && nDuration >= m_nShakeRemaining) {
        m_nShakeRemaining = nDuration;
        m_nShakeDuration = nDuration;
        m_nShakeDelay = nDelay;
        m_nShakeE0 = 70;
        m_bShakeChanged = 1;
    }
}

// FUNCTION: SWKOTOR 0x00641730
void CSWCModule::BumpCamera(int nDuration)
{
    if (!m_nBump) {
        m_nBump = 50;
        m_nBumpDuration = nDuration;
        m_bBumpChanged = 1;
    }
}

// FUNCTION: SWKOTOR 0x00641CB0
// Only the GobRef member at +0x0c needs destruction.
CSWCameraSettings::~CSWCameraSettings()
{
}

// FUNCTION: SWKOTOR 0x00644170
float CSWCModule::UpdateCameraCollision(float fDelta, float fZoom)
{
    if (m_bClearViewRequest)
        return fZoom;
    if (g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nCameraMode == 2)
        return UpdateCameraCollisionEQ(fDelta, fZoom);
    return UpdateCameraCollisionDefault(fDelta, fZoom);
}

extern "C" double __cdecl sin(double x);
extern "C" double __cdecl cos(double x);
extern "C" int __cdecl rand();
#pragma intrinsic(sin, cos)

// FUNCTION: SWKOTOR 0x00641760
void CSWCModule::UpdateCameraBumpAndShake(float fDelta)
{
    CSWCameraOnAStick* pStick = (CSWCameraOnAStick*)m_pCamera->GetBehavior(0x106a);
    CSWCameraShake2C* pDialog = (CSWCameraShake2C*)m_pCamera->GetBehavior(0x106d);
    CSWCameraShake70* pFreeLook = (CSWCameraShake70*)m_pCamera->GetBehavior(0x106e);
    CSWBehaviorCameraCombat* pCombat = (CSWBehaviorCameraCombat*)m_pCamera->GetBehavior(0x1070);
    if (!pStick && !pDialog && !pFreeLook && !pCombat)
        return;

    if (!(g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nFlags & 4)) {
        if (m_nBump > 0) {
            m_nBump = 0;
            m_nBumpDuration = 0;
            m_bBumpChanged = 1;
        }
        else if (m_nShakeRemaining > 0) {
            m_nShakeRemaining = 0;
            m_nShakeDelay = 0;
            m_nShakeE0 = 0;
            m_bShakeChanged = 1;
        }
    }

    int nElapsed = (int)(fDelta * 1000.0f);
    int nRemaining = m_nShakeRemaining;
    if (nRemaining <= 0)
        return;
    if (m_nShakeDelay > 0) {
        if (m_nShakeDelay <= nElapsed) {
            nElapsed -= m_nShakeDelay;
            m_nShakeDelay = 0;
        }
        else {
            m_nShakeDelay -= nElapsed;
        }
    }
    if (m_nShakeDelay)
        return;

    float fYaw;
    float fPitch;
    if (nRemaining <= nElapsed) {
        fPitch = 0.0f;
        m_nShakeRemaining = 0;
        fYaw = 0.0f;
    }
    else {
        double fPhase = nRemaining * 6.2831852;
        float fEnvelope = (float)cos(fPhase / m_nShakeDuration) + 1.0f;
        float fWave = (float)sin(fPhase / g_fCameraShakePeriod);
        float fAmplitude = fWave * fEnvelope * g_fCameraShakeDegrees * 0.0174532924f;
        fYaw = ((rand() + 1) % 2 - 1) * fAmplitude;
        fPitch = ((rand() + 1) % 2 - 1) * fAmplitude;
    }

    if (pStick) {
        pStick->m_fShakePitch = fPitch;
        pStick->m_fShakeYaw = fYaw;
    }
    else if (pCombat) {
        pCombat->m_f18 = fPitch;
        pCombat->m_f14 = fYaw;
    }
    else if (pDialog) {
        pDialog->m_fShakePitch = fPitch;
        pDialog->m_fShakeYaw = fYaw;
    }
    else if (pFreeLook) {
        pFreeLook->m_fShakePitch = fPitch;
        pFreeLook->m_fShakeYaw = fYaw;
    }
    m_nShakeRemaining -= nElapsed;
}
