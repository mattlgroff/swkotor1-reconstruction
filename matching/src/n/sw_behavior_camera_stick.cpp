// CSWCameraOnAStick (swbehaviorcamera.o, vtable 0x0075165C).
// Member-function names without a debug-map record are descriptive.

#include "n/sw_behavior_camera.h"

#include <math.h>

// FUNCTION: SWKOTOR 0x00637A30
void CSWCameraOnAStick::Message(char* szMsg)
{
    if (_stricmp("m_fDist", MsgGetWord(szMsg)) == 0)
        MsgParseFloat(szMsg + 7, &m_fDist);
    if (_stricmp("m_fSpeed", MsgGetWord(szMsg)) == 0)
        MsgParseFloat(szMsg + 8, &m_fSpeed);
    if (_stricmp("m_fPresetPitch", MsgGetWord(szMsg)) == 0)
        MsgParseFloat(szMsg + 14, &m_fPresetPitch);
    if (_stricmp("m_fHeight", MsgGetWord(szMsg)) == 0)
        MsgParseFloat(szMsg + 9, &m_fHeight);
}

// FUNCTION: SWKOTOR 0x00637AF0
Vector CSWCameraOnAStick::ComputeDesiredPosition(Vector vHead)
{
    Vector vOffset = Vector(0.0f, 1.0f, 0.0f) * m_fDist;
    vOffset = m_qOrientation * vOffset;
    Vector vResult = vHead - vOffset;
    return vResult;
}

// FUNCTION: SWKOTOR 0x00637B80
float CSWCameraOnAStick::AccelerateInterp(float fCurrent, float fMin, float fMax, Vector vFrom, Vector vTo,
                                          float fDelta)
{
    float fValue = fCurrent;
    Vector vDiff;
    vDiff.x = vTo.x - vFrom.x;
    vDiff.y = vTo.y - vFrom.y;
    vDiff.z = vTo.z - vFrom.z;
    float fLengthSq = vDiff.z * vDiff.z + vDiff.y * vDiff.y + vDiff.x * vDiff.x;
    if (fLengthSq > 1.0f)
        fValue = fDelta * 3.0f + fCurrent;
    if (fLengthSq < 1.0f)
        fValue = fValue - fDelta * 5.0f;
    fValue = fValue > fMin ? fValue : fMin;
    fValue = fValue < fMax ? fValue : fMax;
    return fValue;
}

// FUNCTION: SWKOTOR 0x00637C20
void CSWCameraOnAStick::ResetLastPosition(Vector vPosition, Quaternion qOrientation)
{
    m_vLastPosition = vPosition;
    m_vPosition = vPosition;
    m_qOrientation = qOrientation;
    m_pGob->SetPosition(vPosition);
    m_pGob->SetOrientation(qOrientation);
}

// FUNCTION: SWKOTOR 0x00638070
void CSWCameraOnAStick::SetCameraStyle(int nStyle)
{
    if (m_nStyle == nStyle)
        return;
    CTwoDimArrays* p2DArrays = g_pRules->m_p2DArrays;
    C2DA* pTable = p2DArrays->m_pCameraStyles;
    if (!pTable) {
        m_nStyle = -1;
        m_fDist = 6.25f;
        m_fSpeed = 20.0f;
        m_fPresetPitch = 80.0f;
        m_fHeight = 3.5f;
        return;
    }
    if (nStyle > pTable->m_nNumRows)
        return;
    m_nStyle = nStyle;
    float fViewAngle;
    {
        CExoString sColumn("DISTANCE");
        pTable->GetFLOATEntry(m_nStyle, sColumn, &m_fDist);
    }
    {
        CExoString sColumn("SPEED");
        pTable->GetFLOATEntry(m_nStyle, sColumn, &m_fSpeed);
    }
    {
        CExoString sColumn("PITCH");
        pTable->GetFLOATEntry(m_nStyle, sColumn, &m_fPresetPitch);
    }
    {
        CExoString sColumn("HEIGHT");
        pTable->GetFLOATEntry(m_nStyle, sColumn, &m_fHeight);
    }
    {
        CExoString sColumn("TILTUP");
        pTable->GetFLOATEntry(m_nStyle, sColumn, &g_fCameraTiltUpLimit);
    }
    {
        CExoString sColumn("TILTDOWN");
        pTable->GetFLOATEntry(m_nStyle, sColumn, &g_fCameraTiltDownLimit);
    }
    {
        CExoString sColumn("TILTSPEED");
        pTable->GetFLOATEntry(m_nStyle, sColumn, &m_fTiltRate);
    }
    {
        CExoString sColumn("ROTATION");
        pTable->GetFLOATEntry(m_nStyle, sColumn, &g_fCameraRotation);
    }
    {
        CExoString sColumn("VIEWANGLE");
        pTable->GetFLOATEntry(m_nStyle, sColumn, &fViewAngle);
    }
    m_cClip.m_fFarClip = g_pAppManager->m_pClientExoApp->GetClientOptions()->GetFarClip();
    m_cClip.m_fFarClip2 = g_pAppManager->m_pClientExoApp->GetClientOptions()->GetFarClip2();
    m_cClip.m_fFarClip3 = g_pAppManager->m_pClientExoApp->GetClientOptions()->GetFarClip3();
    if (fViewAngle == 0.0f)
        fViewAngle = 55.0f;
    m_pCamera->GetCameraView()->SetViewAngle(fViewAngle);
    m_f88 = m_fDist;
    m_f8C = fViewAngle;
}

// FUNCTION: SWKOTOR 0x006383B0
void CSWCameraOnAStick::ClearTarget()
{
    m_oidTarget = 0x7f000000;
}

// FUNCTION: SWKOTOR 0x00638E80
void CSWCameraOnAStick::OrientTowardTarget()
{
    Gob* pTarget = m_rTarget;
    Vector vDir = pTarget->GetPosition() - m_pGob->GetPosition();
    Pitch(vDir);
    vDir.z = 0.0f;
    if (vDir.y * vDir.y + vDir.x * vDir.x > 0.0f) {
        m_pGob->SetOrientation(EulerToQuaternion(GetFacing(&Normalize(vDir)), m_fPresetPitch, 0.0f));
    }
}

// FUNCTION: SWKOTOR 0x00638F60
void CSWCameraOnAStick::UpdateClipDistances()
{
    m_cClip.m_fFarClip = g_pAppManager->m_pClientExoApp->GetClientOptions()->GetFarClip();
    m_cClip.m_fFarClip2 = g_pAppManager->m_pClientExoApp->GetClientOptions()->GetFarClip2();
    m_cClip.m_fFarClip3 = g_pAppManager->m_pClientExoApp->GetClientOptions()->GetFarClip3();
    if (g_nPendingCameraStyle != -1) {
        SetCameraStyle(g_nPendingCameraStyle);
        g_nPendingCameraStyle = -1;
    }
}

// FUNCTION: SWKOTOR 0x00638FE0
void CSWCameraOnAStick::SetCameraInitialState()
{
    Quaternion qTarget = m_rTarget->GetOrientation();
    m_qOrientation = EulerToQuaternion(GetFacing(&Normalize(qTarget.ZAxis())) + m_fYawOffset,
                                       m_fPresetPitch + m_fYawOffset, 0.0f);
    m_pGob->SetOrientation(m_qOrientation);
    Vector vPosition = m_rTarget->GetPosition();
    vPosition.z += 1.5f;
    vPosition -= qTarget.YAxis();
    m_pGob->SetPosition(vPosition);
    m_vPosition = vPosition;
    m_vDesiredPosition = vPosition;
    m_vLastPosition = vPosition;
}

// FUNCTION: SWKOTOR 0x00639760
Quaternion CSWCameraOnAStick::ComputeOrientation(float fDelta)
{
    Gob* pTarget = m_rTarget;
    Vector vDir = pTarget->GetPosition() - m_pGob->GetPosition();
    vDir.z = 0.0f;
    float fCurrentPitch = GetRoll(m_pGob->GetOrientation());
    if (fabs(m_fTiltSpeed) > 1.0)
        m_fTiltSpeed *= 0.013333334f;
    if (m_fTiltSpeed > 0.0f) {
        float fLimit = g_fCameraTiltUpLimit + m_fPresetPitch;
        float fPitch = m_fTiltRate * m_fTiltSpeed * fDelta + m_fPitch;
        m_fPitch = fLimit < fPitch ? fLimit : fPitch;
    }
    if (m_fTiltSpeed < 0.0f) {
        float fLimit = m_fPresetPitch - g_fCameraTiltDownLimit;
        float fPitch = m_fTiltRate * m_fTiltSpeed * fDelta + m_fPitch;
        m_fPitch = fLimit > fPitch ? fLimit : fPitch;
    }
    float fStep = (m_fPitch - fCurrentPitch) * fDelta * 0.5f;
    m_fTiltSpeed = 0.0f;
    return EulerToQuaternion(GetFacing(&Normalize(vDir)) + m_fYawOffset, m_fPitch - fStep + m_f94, 0.0f);
}

// FUNCTION: SWKOTOR 0x00639C30
void CSWCameraOnAStick::SetLookAtTarget(CSWCObject* pObject)
{
    if (!m_rTarget || !pObject)
        return;
    m_oidTarget = pObject->m_oidSelf;
    m_v74 = Normalize(m_rTarget->GetPosition() - pObject->m_vPosition) * m_fDist;
    m_v74 += m_rTarget->GetPosition();
    m_v74.z = m_fHeight + m_f6C + m_v74.z;
}

// FUNCTION: SWKOTOR 0x00639FC0 SYMBOL: ??0CSWCameraOnAStick@@QAE@PAVGob@@0@Z
CSWCameraOnAStick::CSWCameraOnAStick(Gob* pGob, Gob* pTarget)
    : CAurBehavior(pGob), m_vLastPosition(0.0f, 0.0f, 0.0f), m_vDesiredPosition(0.0f, 0.0f, 0.0f),
      m_vPosition(0.0f, 0.0f, 0.0f), m_v74(0.0f, 0.0f, 0.0f), m_v128(0.0f, 0.0f, 0.0f)
{
    m_pCamera = pGob;
    AddDependency(pTarget);
    m_rTarget = pTarget;
    m_rCombatTarget.Clear();
    m_nStyle = -1;
    SetCameraStyle(g_pAppManager->m_pClientExoApp->GetModule()->m_pArea->m_nCameraStyle);
    m_v128 = Vector(0.0f, 0.0f, 0.0f);
    m_fPitch = m_fPresetPitch;
    m_f124 = 0.0f;
    m_f118 = 100.0f;
    m_fTiltSpeed = 0.0f;
    m_f100 = 0.0f;
    m_f10C = 0.0f;
    Quaternion qTarget = m_rTarget->GetOrientation();
    m_qOrientation = EulerToQuaternion(GetFacing(&Normalize(qTarget.ZAxis())), m_fPresetPitch, 0.0f);
    m_pGob->SetOrientation(m_qOrientation);
    Vector vPosition = m_rTarget->GetPosition();
    vPosition.z += 1.5f;
    vPosition -= qTarget.YAxis();
    m_pGob->SetPosition(vPosition);
    m_vPosition = vPosition;
    m_vDesiredPosition = vPosition;
    m_vLastPosition = vPosition;
    m_n64 = 0;
    m_f84 = 0.0f;
    m_fYawOffset = 0.0f;
    m_f94 = 0.0f;
    m_fA4 = 0.0f;
    m_fAC = 0.0f;
    m_n60 = 1;
    m_vDesiredPosition.x = 3.4028235e38f;
    m_oidTarget = 0x7f000000;
    m_f98 = -1.0f;
    m_bSwapMouseButtons = g_bSwapMouseButtons;
}

// FUNCTION: SWKOTOR 0x0063A310
int CSWCameraOnAStick::GetType()
{
    return 0x106a;
}

// FUNCTION: SWKOTOR 0x0063A320
CSWCameraOnAStick::~CSWCameraOnAStick()
{
}

// FUNCTION: SWKOTOR 0x0063A360
void CSWCameraOnAStick::ClearCombatTarget()
{
    m_rCombatTarget.Clear();
}

// FUNCTION: SWKOTOR 0x0063AD40 SYMBOL: ??_GCSWCameraOnAStick@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0063AD60 SYMBOL: ??0CSWCameraOnAStick@@QAE@PAVCSWCameraSettings@@@Z
CSWCameraOnAStick::CSWCameraOnAStick(CSWCameraSettings* pSettings)
    : CAurBehavior(pSettings->m_pCamera), m_vLastPosition(0.0f, 0.0f, 0.0f), m_vDesiredPosition(0.0f, 0.0f, 0.0f),
      m_vPosition(0.0f, 0.0f, 0.0f), m_v74(0.0f, 0.0f, 0.0f), m_v128(0.0f, 0.0f, 0.0f)
{
    m_rTarget = pSettings->m_rTarget;
    m_pCamera = pSettings->m_pCamera;
    m_n68 = pSettings->m_n14;
    m_f6C = pSettings->m_f18;
    m_nStyle = -1;
    SetCameraStyle(pSettings->m_nStyle);
    AddDependency(m_rTarget);
    m_v128 = Vector(0.0f, 0.0f, 0.0f);
    m_fPitch = m_fPresetPitch;
    m_f124 = 0.0f;
    m_f118 = 100.0f;
    m_fTiltSpeed = 0.0f;
    m_f100 = 0.0f;
    m_f10C = 0.0f;
    Quaternion qTarget = m_rTarget->GetOrientation();
    m_qOrientation = EulerToQuaternion(GetFacing(&Normalize(qTarget.ZAxis())), m_fPresetPitch, 0.0f);
    m_pGob->SetOrientation(m_qOrientation);
    Vector vPosition = m_rTarget->GetPosition();
    vPosition.z += 1.5f;
    vPosition -= qTarget.YAxis();
    m_vPosition = vPosition;
    m_vDesiredPosition = vPosition;
    m_vLastPosition = vPosition;
    m_n60 = 1;
    m_n64 = 1;
    m_fYawOffset = 0.0f;
    m_f94 = 0.0f;
    m_fA4 = 0.0f;
    m_fAC = 0.0f;
    m_oidTarget = 0x7f000000;
    m_f98 = -1.0f;
    m_bSwapMouseButtons = g_bSwapMouseButtons;
}

// FUNCTION: SWKOTOR 0x0063A380
void CSWCameraOnAStick::ChangeTarget(Gob* pTarget, int bUnused)
{
    m_f100 = 0.0f;
    if (pTarget != m_rTarget) {
        RemoveDependency(m_rTarget);
        AddDependency(pTarget);
    }
    m_rTarget = pTarget;
    Quaternion qTarget;
    Quaternion qOwn;
    Vector vBack(0.0f, 0.0f, 0.0f);
    qTarget = m_rTarget->GetOrientation();
    qOwn = m_pGob->GetOrientation();
    m_pGob->SetOrientation(EulerToQuaternion(GetPitch(qTarget), GetRoll(qOwn), GetEulerRoll(qOwn)));
    vBack = -qTarget.YAxis();
    vBack.z = 0.0f;
    vBack.Normalize();
    Vector vPosition = vBack * m_fDist + m_rTarget->GetPosition();
    vPosition.z = m_fHeight;
    m_vDesiredPosition = vPosition;
    m_vPosition = vPosition;
}

// FUNCTION: SWKOTOR 0x0063BA80
void CSWCameraOnAStick::SaveCameraSettings(CSWCameraSettings* pSettings)
{
    pSettings->m_nValid = 1;
    pSettings->m_nType = g_nCameraSettingsTypeOnAStick;
    pSettings->m_pCamera = m_pCamera;
    pSettings->m_rTarget = m_rTarget.m_p;
    pSettings->m_n14 = m_n68;
    pSettings->m_nStyle = m_nStyle;
    pSettings->m_f18 = m_f6C;
    pSettings->m_nVideoEffect = -1;
}
