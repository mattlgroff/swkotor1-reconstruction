// CSWCameraDeath (swbehaviorcamera.o, vtable 0x0075169C).

#include "n/sw_behavior_camera.h"

// FUNCTION: SWKOTOR 0x00638540
void CSWCameraDeath::Message(char* szMsg)
{
    if (_stricmp("m_yaw", MsgGetWord(szMsg)) == 0)
        MsgParseFloat(szMsg + 5, &m_yaw);
    if (_stricmp("m_speedYaw", MsgGetWord(szMsg)) == 0)
        MsgParseFloat(szMsg + 10, &m_speedYaw);
    if (_stricmp("m_desiredPitch", MsgGetWord(szMsg)) == 0)
        MsgParseFloat(szMsg + 14, &m_desiredPitch);
    if (_stricmp("m_desiredDistance", MsgGetWord(szMsg)) == 0)
        MsgParseFloat(szMsg + 17, &m_desiredDistance);
    if (_stricmp("m_logYaw", MsgGetWord(szMsg)) == 0)
        MsgParseFloat(szMsg + 8, &m_logYaw);
    if (_stricmp("m_logPitch", MsgGetWord(szMsg)) == 0)
        MsgParseFloat(szMsg + 10, &m_logPitch);
    if (_stricmp("m_logDistance", MsgGetWord(szMsg)) == 0)
        MsgParseFloat(szMsg + 13, &m_logDistance);
}

// FUNCTION: SWKOTOR 0x0063A730
CSWCameraDeath::~CSWCameraDeath()
{
}

// FUNCTION: SWKOTOR 0x0063A760
int CSWCameraDeath::GetType()
{
    return 0x106f;
}

// FUNCTION: SWKOTOR 0x0063A770
void CSWCameraDeath::SetTarget(Gob* pTarget)
{
    if (m_rTarget)
        RemoveDependency(m_rTarget);
    if (pTarget) {
        AddDependency(pTarget);
        Vector vDir = pTarget->GetPosition() - m_pGob->GetPosition();
        Vector vNormal = Normalize(vDir);
        m_yaw = GetFacing(&vNormal);
        m_f34 = 0.75f;
    }
    m_rTarget = pTarget;
}

// FUNCTION: SWKOTOR 0x0063BBD0
CSWCameraDeath::CSWCameraDeath(Gob* pGob, Gob* pTarget)
    : CAurBehavior(pGob)
{
    m_yaw = 0.0f;
    m_logYaw = 0.5f;
    m_speedYaw = 30.0f;
    m_logPitch = 0.01f;
    m_desiredPitch = 90.0f;
    m_logDistance = 0.5f;
    m_desiredDistance = 3.0f;
    m_f34 = 0.75f;
    m_f38 = 0.0f;
    m_f3C = 0.01f;
    m_rTarget.Clear();
    SetTarget(pTarget);
}

// FUNCTION: SWKOTOR 0x0063BC90 SYMBOL: ??_GCSWCameraDeath@@UAEPAXI@Z
