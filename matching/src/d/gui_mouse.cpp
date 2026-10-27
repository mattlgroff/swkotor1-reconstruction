// CSWGuiManager::ActivateSoftwareMouse: builds the GUI scene that holds the
// "gui_mouse" model. Kept out of gui_manager.cpp so it is not inlined into
// ShowSoftwareMouse.
#include "d/gui.h"

// NONMATCH: 17 insns, only the camera call: the original loads the camera's
// vtable after the Vector argument is built (sub esp before the fild).
// FUNCTION: SWKOTOR 0x0040b060
int CSWGuiManager::ActivateSoftwareMouse()
{
    m_pMouseObject = new CSWGuiScene;
    if (!m_pMouseObject) {
        return 0;
    }
    {
        CSWGuiExtent extent;
        extent.height = m_nHeight;
        extent.left = 0;
        extent.top = 0;
        extent.width = m_nWidth;
        m_pMouseObject->SetExtent(extent);
    }
    {
        CExoString sModel("gui_mouse");
        m_pMouse = m_pMouseObject->AddModel(sModel, -1);
    }
    if (!m_pMouse) {
        return 0;
    }
    m_pMouse->SetTexture("", m_cCursor.GetResRefStr());
    CAurCamera* pCamera = m_pMouseObject->m_pCamera;
    pCamera->SetViewSize(Vector(m_nWidth * 0.005f, m_nHeight * 0.005f, m_nWidth * 0.0090625f));
    m_pMouse->SetPosition(Vector(m_nMouseX * 0.01f, (m_nHeight - m_nMouseY) * 0.01f, 0.0f));
    if (m_bCentred) {
        m_pMouse->PlayAnimation("center", 1.0f, 0, 0);
        m_pMouse->SetScale(1.0f);
        m_pMouse->PlayAnimation("", 0, 0x80, 0);
    }
    m_pMouse->SetIllumination(Vector(1.0f, 1.0f, 1.0f), 1);
    return 1;
}
