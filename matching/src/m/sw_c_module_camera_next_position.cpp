// CSWCModule::GetNextCameraPosition: predicts where the follow camera will
// be after the given time step.

#include "m/sw_c_module_camera.h"

extern "C" double __cdecl pow(double x, double y);
#pragma intrinsic(pow)

#define CAMERA_MIN(a, b) ((a) < (b) ? (a) : (b))
#define CAMERA_MAX(a, b) ((a) > (b) ? (a) : (b))

// FUNCTION: SWKOTOR 0x006402F0
// NONMATCH: 56 diff insns. Control flow, clamps and pow calls agree; the
// original copies the normalized quaternion into a named local before
// ToDirection (4 bytes more frame) and adds m_vCameraUp first for y/z.
int CSWCModule::GetNextCameraPosition(float fDelta, Vector* pOut)
{
    Quaternion qDesired;
    if (!m_pCamera)
        return 0;
    CClientExoApp* pApp = g_pAppManager->m_pClientExoApp;
    CGameObject* pObject = pApp->GetGameObject(m_oidFollow);
    if (!pObject)
        return 0;
    CSWCObject* pClientObject = pObject->AsSWCObject();
    if (!pClientObject)
        return 0;
    Gob* pModel = pClientObject->GetModel(0xff, 1);
    if (!pModel)
        return 0;

    float fYaw = m_pCamera->GetYaw() + fDelta * m_fCameraTurn;
    float fPitch = m_pCamera->GetPitch() + fDelta * m_fCameraTilt;
    float fScale = (float)pow(2.0, fDelta * m_fCameraZoom * -0.5f);
    float fZoom = m_pCamera->GetDistance() * fScale;
    fPitch = CAMERA_MAX(g_fCameraFollowC, CAMERA_MIN(fPitch, g_fCameraFollowPitch));
    fZoom = CAMERA_MAX(g_fCameraFollowA, CAMERA_MIN(fZoom, g_fCameraFollowB));
    float fT = 1.0f - (float)pow(0.5, fDelta * 750.0f);

    Vector vTarget = m_vCameraUp + pModel->GetPosition();
    Vector vBase = m_vCameraTarget * (1.0f - fT) + vTarget * fT;
    qDesired = EulerToQuaternion(fYaw, fPitch, 0.0f);
    Quaternion qOrientation = Normalize(Slerp(m_pCamera->GetOrientation(), qDesired, fT));
    Vector vDirection = qOrientation.ToDirection();
    m_vCameraTarget = vBase;
    *pOut = vBase + vDirection * fZoom;
    return 1;
}
