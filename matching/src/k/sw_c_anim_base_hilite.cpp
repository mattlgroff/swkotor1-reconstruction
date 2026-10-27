// CSWCAnimBase alpha and hilite colour (nwcanimbase.o): the first active
// hilite slot gives the model's illumination and colour shift.
#include "k/sw_c_anim_base.h"

// The area's hilite bookkeeping is compiled out; only the calls that
// produce its arguments remain.
class CSWCArea {
public:
    void UpdateHilite(void* pProperty) {}
    void ClearHilite() {}
};

class CSWCModule {
public:
    char pad_0x0[0x48];
    CSWCArea* m_pArea;                              // 0x48
};

class CClientExoApp {
public:
    CSWCModule* GetModule();                        // 0x005ED530
};

class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp* m_pClientExoApp;                 // 0x04
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

// Not in the function table: starts inside 0x0069E8D0's range.
// FUNCTION: SWKOTOR 0x0069EB20
int CSWCAnimBase::SetAlpha(float fAlpha, unsigned char nPart)
{
    if (!m_pModel || nPart != 0xff) {
        return 0;
    }
    m_fC0 = fAlpha;
    int i;
    for (i = 0; i < 5; i++) {
        if (m_a90[i]) {
            m_pModel->SetColorShifting(m_aVectors[i], fAlpha, 1);
            break;
        }
    }
    if (i == 5) {
        m_pModel->SetColorShifting(Vector(1.0f, 1.0f, 1.0f), fAlpha, 1);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0069EC00
void CSWCAnimBase::RefreshHilite()
{
    if (!m_pModel) {
        return;
    }
    int i;
    for (i = 0; i < 5; i++) {
        if (m_a90[i]) {
            m_pModel->SetIllumination(m_aVectors[i], 1);
            m_pModel->SetColorShifting(m_aVectors[i], m_fC0, 1);
            if (m_aA4[i] && g_pAppManager->m_pClientExoApp->GetModule() &&
                g_pAppManager->m_pClientExoApp->GetModule()->m_pArea) {
                CSWCArea* pArea = g_pAppManager->m_pClientExoApp->GetModule()->m_pArea;
                pArea->UpdateHilite(m_pModel->GetGobProperty());
            }
            break;
        }
    }
    if (i == 5) {
        m_pModel->SetIllumination(Vector(0.0f, 0.0f, 0.0f), 1);
        m_pModel->SetColorShifting(Vector(1.0f, 1.0f, 1.0f), m_fC0, 1);
        if (g_pAppManager->m_pClientExoApp->GetModule() && g_pAppManager->m_pClientExoApp->GetModule()->m_pArea) {
            g_pAppManager->m_pClientExoApp->GetModule()->m_pArea->ClearHilite();
        }
    }
}
