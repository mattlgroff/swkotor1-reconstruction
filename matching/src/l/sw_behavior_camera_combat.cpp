// The combat camera behaviour CSWBehaviorCameraCombat (vtable 0x00757BDC):
// a camera-style table driven orbit around the player in combat mode.
#include "l/sw_behavior_camera_combat.h"
#include <math.h>

// FUNCTION: SWKOTOR 0x006D12B0
CSWBehaviorCameraCombat::CSWBehaviorCameraCombat(Gob* pGob)
    : CAurBehavior(pGob), m_v1C(0.0f, 0.0f, 0.0f)
{
    m_f18 = 0.0f;
    m_f14 = 0.0f;
    m_fTurn = 0.0f;
    m_fTurnAccel = 0.0f;
    CTwoDimArrays* p2DArrays = g_pRules->m_p2DArrays;
    C2DA* pTable = p2DArrays->m_pCameraStyles;
    float fViewAngle;
    {
        CExoString sColumn("DISTANCE");
        pTable->GetFLOATEntry(8, sColumn, &m_fDistance);
    }
    {
        CExoString sColumn("HEIGHT");
        pTable->GetFLOATEntry(8, sColumn, &m_fHeight);
    }
    {
        CExoString sColumn("PITCH");
        pTable->GetFLOATEntry(8, sColumn, &m_fPitch);
    }
    {
        CExoString sColumn("ROTATION");
        pTable->GetFLOATEntry(8, sColumn, &m_fRotation);
    }
    {
        CExoString sColumn("SPEED");
        pTable->GetFLOATEntry(8, sColumn, &m_fSpeed);
    }
    {
        CExoString sColumn("VIEWANGLE");
        pTable->GetFLOATEntry(8, sColumn, &fViewAngle);
    }
    m_pGob->GetCameraView()->SetViewAngle(fViewAngle);
    m_cClip.m_fMass = g_pAppManager->m_pClientExoApp->GetClientOptions()->GetFarClip();
    m_cClip.m_fGain = g_pAppManager->m_pClientExoApp->GetClientOptions()->GetFarClip2();
    float fReleaseGain = g_pAppManager->m_pClientExoApp->GetClientOptions()->GetFarClip3();
    m_cClip.m_fReleaseGain = fReleaseGain;
    m_bSwapMouseButtons = g_bSwapMouseButtons;
}

// FUNCTION: SWKOTOR 0x006D14D0
int CSWBehaviorCameraCombat::GetType()
{
    return 0x1070;
}

// The camera's forward (Y) axis; inlined copy of Quaternion::GetYAxis
// (0x0044F420).
static inline Vector L_GetYAxis(const Quaternion& q)
{
    return Vector((q.x * q.y - q.w * q.z) * 2.0f,
                  1.0f - (q.x * q.x + q.z * q.z) * 2.0f,
                  (q.y * q.z + q.w * q.x) * 2.0f);
}

// FUNCTION: SWKOTOR 0x006D14E0
Vector CSWBehaviorCameraCombat::CalculateDesiredLocation(CSWCCreature* pCreature, CSWCObject* pTarget,
                                                         float fDelta, float* pfYaw)
{
    CSWCModuleCamera* pCamera = g_pAppManager->m_pClientExoApp->GetModuleCamera();
    Vector vDirection(0.0f, 0.0f, 0.0f);
    Vector vStart = pCreature->m_vPosition;
    float fTurn;
    if (m_fTurn == 0.0f) {
        fTurn = m_cClip.Update(Vector(0.0f, m_fTurnAccel, 0.0f), fDelta, 1, 0, 0, 0).m_vVelocity.y;
    } else {
        fTurn = m_fTurn;
        m_cClip.SetVelocity(fTurn);
        m_fTurnAccel = 0.0f;
    }
    Quaternion qOrientation = EulerToQuaternion(fTurn, 0.0f, 0.0f) * pCamera->GetOrientation();
    *pfYaw = GetPitch(qOrientation);
    vDirection = L_GetYAxis(qOrientation);
    vDirection.z = 0.0f;
    vDirection.Normalize();
    vStart.z = 0.0f;
    Vector vResult = vDirection * -m_fDistance + vStart;
    Vector vOut = vResult;
    vOut.z = pCreature->m_pAppearance->GetHeadHeightOffset() + pCreature->m_vPosition.z + m_fHeight;
    return vOut;
}

// FUNCTION: SWKOTOR 0x006D16F0
float CSWBehaviorCameraCombat::GetNextYaw(const Vector& vTarget, float fYaw, float fCurrent,
                                          float* pfTurn)
{
    Quaternion qOrientation = m_pGob->GetOrientation();
    if (GetPitch(qOrientation) != fYaw) {
        g_pAppManager->m_pClientExoApp->GetClientOptions()->GetBrightness();
    }
    g_pAppManager->m_pClientExoApp->GetClientOptions()->GetBrightness();
    *pfTurn = 0.0f;
    return fYaw;
}

// FUNCTION: SWKOTOR 0x006D1760
Vector CSWBehaviorCameraCombat::GetNextPosition(const Vector& vDesired, float fDuration,
                                                float fDelta)
{
    Vector vDiff = vDesired - m_pGob->GetPosition();
    Vector vResult = vDesired;
    if (fDuration > 0.001f) {
        float fLengthSq = vDiff.MagnitudeSquared();
        if (fLengthSq > 0.01f) {
            fDuration = sqrt(fLengthSq) / fDuration * fDelta;
            vResult = Normalize(vDiff) * fDuration + m_pGob->GetPosition();
        }
    }
    return vResult;
}

// FUNCTION: SWKOTOR 0x006D1870
void CSWBehaviorCameraCombat::SetTurn(float fTurn)
{
    int bTurn = g_bSwapMouseButtons;
    if (g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nMouseFlags & 2) {
        bTurn = bTurn == 0;
    }
    if (bTurn) {
        m_fTurn = fTurn;
    } else {
        m_fTurn = 0.0f;
    }
}

// FUNCTION: SWKOTOR 0x006D18C0 SYMBOL: ??_GCSWBehaviorCameraCombat@@UAEPAXI@Z

// Debug output of the camera position (compiled out; its arguments are
// still evaluated).
static inline void L_DebugCameraPosition(float fX, float fY, float fZ, float fDistance)
{
}

// FUNCTION: SWKOTOR 0x006D1FC0
int CSWBehaviorCameraCombat::Control(float fDelta)
{
    if (!CAurBehavior::Control(fDelta)) {
        return 0;
    }
    m_cClip.m_fMass = g_pAppManager->m_pClientExoApp->GetClientOptions()->GetFarClip();
    m_cClip.m_fGain = g_pAppManager->m_pClientExoApp->GetClientOptions()->GetFarClip2();
    m_cClip.m_fReleaseGain = g_pAppManager->m_pClientExoApp->GetClientOptions()->GetFarClip3();
    CSWCCreature* pPC = g_pAppManager->m_pClientExoApp->GetParty()->GetPlayerCharacter();
    CClientExoApp* pApp = g_pAppManager->m_pClientExoApp;
    CGameObject* pTarget = pApp->GetGameObject(pApp->GetLastTarget());
    if (g_bRenderQA && m_pGob && pPC) {
        Vector vA = m_pGob->GetPosition();
        Vector vB = m_pGob->GetPosition();
        Vector vC = m_pGob->GetPosition();
        Magnitude(m_pGob->GetPosition() - pPC->m_vPosition);
    }
    int bSwap = g_bSwapMouseButtons;
    if (g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nMouseFlags & 2) {
        bSwap = bSwap == 0;
    }
    if (bSwap) {
        if (!m_bSwapMouseButtons) {
            g_pGuiManager->SaveMousePosition();
            m_bSwapMouseButtons = bSwap;
            g_pAppManager->m_pClientExoApp->HideMouse(0x20);
        }
    } else if (m_bSwapMouseButtons) {
        g_pGuiManager->RestoreOldMousePosition();
        m_bSwapMouseButtons = 0;
        g_pAppManager->m_pClientExoApp->ShowMouse(0x20);
    }
    if (g_pAppManager->m_pClientExoApp->GetInputClass() == 2) {
        m_bSwapMouseButtons = 0;
        g_pAppManager->m_pClientExoApp->ShowMouse(0x20);
    }
    if (!pTarget) {
        pTarget = (CGameObject*)pPC;
    }
    if (pPC && pTarget && pTarget->AsSWCObject()) {
        float fYaw;
        Vector vSafe;
        {
            Vector vHead = pPC->m_vPosition;
            vHead.z += pPC->m_pAppearance->GetHeadNodeHeight();
            Vector vDesired = CalculateDesiredLocation(pPC, pTarget->AsSWCObject(), fDelta, &fYaw);
            vSafe = CheckPosition(pPC, vDesired, vHead);
        }
        float fTurn = 0.0f;
        fYaw = GetNextYaw(vSafe, fYaw, fDelta, &fTurn);
        Vector vPosition = GetNextPosition(vSafe, fTurn, fDelta);
        Quaternion qOrientation = EulerToQuaternion(fYaw + m_f14, m_fPitch + m_f18, 0.0f);
        m_pGob->SetOrientation(qOrientation);
        m_pGob->SetPosition(vPosition);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x006D22C0
// NONMATCH: pPC / app pointer in edi / ebx swapped, and the original shares
// the target-position slot with the checked position (19 insns).
void CSWBehaviorCameraCombat::RestoreSettings()
{
    CSWCObject* pPC = g_pAppManager->m_pClientExoApp->GetParty()->GetPlayerCharacter();
    CClientExoApp* pApp = g_pAppManager->m_pClientExoApp;
    CGameObject* pTarget = pApp->GetGameObject(pApp->GetLastTarget());
    g_pAppManager->m_pClientExoApp->GetModuleCamera();
    if (pPC && pTarget && pTarget->AsSWCObject()) {
        Vector vStart = pPC->m_vPosition;
        Vector vTarget = pTarget->AsSWCObject()->m_vPosition;
        Vector vDir = vTarget - vStart;
        float fYaw = GetFacing(&vDir);
        vDir.Normalize();
        Vector vDesired = vDir * -m_fDistance + vStart;
        Vector vPosition = CheckPosition(pPC, vDesired, vStart);
        Quaternion qOrientation = EulerToQuaternion(fYaw, m_fPitch, 0.0f);
        m_pGob->SetOrientation(qOrientation);
        m_pGob->SetPosition(vPosition);
    }
}
