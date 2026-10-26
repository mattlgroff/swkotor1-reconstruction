// CSWCameraFreeLook (swbehaviorcamera.o, vtable 0x0075167C).

#include "n/sw_behavior_camera.h"

// FUNCTION: SWKOTOR 0x006383C0
void CSWCameraFreeLook::UpdateCamraStyle()
{
    CTwoDimArrays* p2DArrays = g_pRules->m_p2DArrays;
    C2DA* pTable = p2DArrays->m_pCameraStyles;
    if (!pTable) {
        m_fRotateSpeed = 60.0f;
        m_fTiltSpeed = 60.0f;
    }
    CSWCArea* pArea = g_pAppManager->m_pClientExoApp->GetModule()->m_pArea;
    int nRow = pArea->m_nCameraStyle;
    {
        CExoString sColumn("FL_TiltSpeed");
        pTable->GetFLOATEntry(nRow, sColumn, &m_fTiltSpeed);
    }
    {
        CExoString sColumn("FL_RotateSpeed");
        pTable->GetFLOATEntry(nRow, sColumn, &m_fRotateSpeed);
    }
    {
        CExoString sColumn("FL_LOOKUP");
        pTable->GetFLOATEntry(nRow, sColumn, &m_fLookUp);
    }
    {
        CExoString sColumn("FL_LOOKDOWN");
        pTable->GetFLOATEntry(nRow, sColumn, &m_fLookDown);
    }
    if (m_fLookUp > m_fLookDown)
        m_fLookLimit = m_fLookUp;
    else
        m_fLookLimit = m_fLookDown;
}

// FUNCTION: SWKOTOR 0x0063A5D0 SYMBOL: ??0CSWCameraFreeLook@@QAE@PAVGob@@0H@Z
CSWCameraFreeLook::CSWCameraFreeLook(Gob* pGob, Gob* pTarget, int nVideoEffect)
    : CAurBehavior(pGob), m_v4C(0.0f, 0.0f, 0.0f), m_v58(0.0f, 0.0f, 0.0f), m_v64(0.0f, 0.0f, 0.0f)
{
    m_pCamera = pGob;
    AddDependency(pTarget);
    m_rTarget = pTarget;
    m_f38 = 0.0f;
    m_f24 = 0.0f;
    m_f20 = 0.0f;
    m_f70 = 0.0f;
    m_f74 = 0.0f;
    UpdateCamraStyle();
    m_nVideoEffect = nVideoEffect;
    g_pAppManager->m_pClientExoApp->EnableVideoEffect(nVideoEffect);
    m_fLookLimit = 200.0f;
    m_fC0 = 500.0f;
}

// FUNCTION: SWKOTOR 0x0063A6B0
int CSWCameraFreeLook::GetType()
{
    return 0x106e;
}

// FUNCTION: SWKOTOR 0x0063A6C0
CSWCameraFreeLook::~CSWCameraFreeLook()
{
    g_pAppManager->m_pClientExoApp->DisableVideoEffect();
}

// FUNCTION: SWKOTOR 0x0063BAD0 SYMBOL: ??_GCSWCameraFreeLook@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0063BAF0 SYMBOL: ??0CSWCameraFreeLook@@QAE@PAVCSWCameraSettings@@@Z
CSWCameraFreeLook::CSWCameraFreeLook(CSWCameraSettings* pSettings)
    : CAurBehavior(pSettings->m_pCamera), m_v4C(0.0f, 0.0f, 0.0f), m_v58(0.0f, 0.0f, 0.0f),
      m_v64(0.0f, 0.0f, 0.0f)
{
    m_rTarget = pSettings->m_rTarget;
    m_pCamera = pSettings->m_pCamera;
    m_f20 = 0.0f;
    m_f38 = 0.0f;
    m_f24 = 0.0f;
    UpdateCamraStyle();
    m_nVideoEffect = pSettings->m_nVideoEffect;
    g_pAppManager->m_pClientExoApp->EnableVideoEffect(m_nVideoEffect);
    m_fLookLimit = 200.0f;
    m_fC0 = 500.0f;
}
