// CSWCMiniGame constructor (vtable 0x00752914); the CResHelper base
// constructor is inlined here.

#define M_INLINE_RES_HELPER_CTOR
#include "m/sw_mini_game_object.h"

// The client area: only its resource name is read here.
class CSWCArea {
public:
    char pad_0x0[0x10c];
    CResRef m_cResRef;                              // 0x10c
};

// FUNCTION: SWKOTOR 0x00671C40
CSWCMiniGame::CSWCMiniGame(CSWCArea* pArea)
    : m_sC0(Vector(), Vector()), m_sD8(Vector(), Vector())
{
    m_pScene = (CSWMiniScene*)pArea;
    m_pPlayer = 0;
    m_pCamera = 0;
    m_n8C = 0;
    m_fNear = 0.1f;
    m_f70 = 65.0f;
    m_fFar = 100.0f;
    m_nAxisType = 0;
    m_n7C = 0;
    m_nMode = 0;
    m_nLateralAxis = 0;
    m_nVerticalAxis = 0;
    m_nAxis = 1;
    m_pRoom = 0;
    m_pMusic = new CExoStreamingSoundSource;
    m_pMusic->SetLooping(1);
    SetResRef(pArea->m_cResRef, 0);
}
