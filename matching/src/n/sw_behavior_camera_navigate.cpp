// CSWCameraNavigate (swbehaviorcamera.o, vtable 0x007515F4).

#include "n/sw_behavior_camera.h"

// FUNCTION: SWKOTOR 0x00638670
CSWCameraNavigate::CSWCameraNavigate(Gob* pGob)
    : CAurBehavior(pGob)
{
    m_fTurn = 0.0f;
    m_fTilt = 0.0f;
    m_fSpeed = 0.0f;
    m_fRaise = 0.0f;
    m_fStrafe = 0.0f;
    m_fRoll = 0.0f;
    m_f2C = 0.0f;
}

// FUNCTION: SWKOTOR 0x006386A0
int CSWCameraNavigate::GetType()
{
    return 0x1071;
}

// FUNCTION: SWKOTOR 0x006386B0 SYMBOL: ??_GCSWCameraNavigate@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006386D0
CSWCameraNavigate::~CSWCameraNavigate()
{
}

// FUNCTION: SWKOTOR 0x006386E0
void CSWCameraNavigate::Message(char* szMsg)
{
    if (_stricmp("turn", MsgGetWord(szMsg)) == 0)
        MsgParseFloat(szMsg + 4, &m_fTurn);
    if (_stricmp("tilt", MsgGetWord(szMsg)) == 0)
        MsgParseFloat(szMsg + 4, &m_fTilt);
    if (_stricmp("speed", MsgGetWord(szMsg)) == 0)
        MsgParseFloat(szMsg + 5, &m_fSpeed);
    if (_stricmp("raise", MsgGetWord(szMsg)) == 0)
        MsgParseFloat(szMsg + 5, &m_fRaise);
    if (_stricmp("strafe", MsgGetWord(szMsg)) == 0)
        MsgParseFloat(szMsg + 6, &m_fStrafe);
    if (_stricmp("roll", MsgGetWord(szMsg)) == 0)
        MsgParseFloat(szMsg + 4, &m_fRoll);
}
