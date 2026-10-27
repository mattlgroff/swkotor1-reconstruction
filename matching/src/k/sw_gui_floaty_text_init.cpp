// CSWGuiFloatyText::Init and Update (floating text over an object).
#include "k/sw_gui_floaty_text.h"

// FUNCTION: SWKOTOR 0x00688E10
void CSWGuiFloatyText::Init(unsigned long oid, const CExoString& sText, const Vector& vColor,
                            float fDuration)
{
    m_fDuration = m_fTimeLeft = fDuration;
    m_nStack = 1;
    m_oidObject = oid;
    m_cText.SetAlignment(0x12);
    m_cText.SetText(sText);
    m_cText.SetColor(vColor);
    m_cText.m_fAlpha = 1.0f;
    m_cText.SetBaseFont("fnt_d16x16");
    Vector vAnchor(0.0f, 0.0f, 0.0f);
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidObject);
    if (!pGameObject) {
        return;
    }
    CSWCObject* pObject = pGameObject->AsSWCObject();
    if (!pObject) {
        return;
    }
    pObject->GetTextAnchor(&vAnchor);
    m_extent.width = 200;
    int nHeight = m_cTextBox.GetLineHeight();
    m_extent.height = nHeight;
    m_extent.top = (int)(vAnchor.y - 32.0 - nHeight);
    m_extent.left = (int)(vAnchor.x - 100.0f);
    SetExtent(m_extent);
    if (!pObject->AsSWCCreature()) {
        return;
    }
    CAurObject* pModel = pObject->GetPartModel(0xff, 1);
    CSWCModuleCamera* pCamera = g_pAppManager->m_pClientExoApp->GetModuleCamera();
    Vector vScreen = pCamera->WorldToScreen(pModel->GetPosition(), 1);
    m_nScreenX = (short)(int)(g_pAppManager->m_pClientExoApp->GetGuiManager()->m_nWidth * vScreen.x);
    m_nScreenY = (short)(int)(g_pAppManager->m_pClientExoApp->GetGuiManager()->m_nHeight * vScreen.y);
}

// FUNCTION: SWKOTOR 0x00688FF0
// NONMATCH: register/peephole only; the original adds the y delta to
// m_extent.top in memory and swaps ecx/edx there. Compound, reversed and
// local-hoisted extent updates tried. Not a separate functions.csv entry.
void CSWGuiFloatyText::Update(float fDelta)
{
    if ((m_fTimeLeft -= fDelta) <= 0.0f) {
        g_pMainInterface->RemoveControl(this);
        return;
    }
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidObject);
    if (pGameObject) {
        CSWCObject* pObject = pGameObject->AsSWCObject();
        if (pObject && pObject->AsSWCCreature()) {
            CAurObject* pModel = pObject->GetPartModel(0xff, 1);
            CSWCModuleCamera* pCamera = g_pAppManager->m_pClientExoApp->GetModuleCamera();
            Vector vDir = -pCamera->GetOrientation().ToDirection();
            if (vDir * pModel->GetPosition() > vDir * pCamera->GetPosition()) {
                Vector vScreen = pCamera->WorldToScreen(pModel->GetPosition(), 1);
                short nX = (short)(int)(g_pAppManager->m_pClientExoApp->GetGuiManager()->m_nWidth * vScreen.x);
                short nY = (short)(int)(g_pAppManager->m_pClientExoApp->GetGuiManager()->m_nHeight * vScreen.y);
                m_extent.top += nY - m_nScreenY;
                m_extent.left += nX - m_nScreenX;
                m_nScreenY = nY;
                m_nScreenX = nX;
                SetExtent(m_extent);
                m_bVisible = 1;
            } else {
                m_bVisible = 0;
            }
        }
    }
    m_cText.m_fAlpha = m_fTimeLeft / m_fDuration;
}
