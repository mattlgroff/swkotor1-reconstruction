// CSWGuiManager. Semantics from src/batches/b0040_gui_manager_panel.cpp.
#include "d/gui.h"

// Members the original calls out of line are wrapped in
// #pragma auto_inline(off) (MSVC inlines across the whole TU otherwise).

// FUNCTION: SWKOTOR 0x0040aaa0
CSWGuiManager::~CSWGuiManager()
{
    if (m_p84) {
        delete m_p84;
        m_p84 = 0;
    }
    if (m_p80) {
        delete m_p80;
        m_p80 = 0;
    }
    if (m_pBackground) {
        delete m_pBackground;
        m_pBackground = 0;
    }
    DeactivateSoftwareMouse();
    if (m_pToolTip) {
        delete m_pToolTip;
        m_pToolTip = 0;
    }
    UnloadGuiSounds();
    g_pGuiManager = 0;
}

// FUNCTION: SWKOTOR 0x0040a260
void CSWGuiManager::HideSoftwareMouse()
{
    DeactivateSoftwareMouse();
}

// FUNCTION: SWKOTOR 0x0040acc0
void CSWGuiManager::DetermineNeedToDraw()
{
    int nCount = m_lstModalPanels.num;
    int bCovered = 0;
    for (int i = nCount - 1; i >= 0; i--) {
        CSWGuiPanel* pPanel = m_lstModalPanels[i];
        if (pPanel && (pPanel->m_nFlags & 4)) {
            if (bCovered) {
                pPanel->m_nFlags &= ~0x80;
            }
            else {
                pPanel->m_nFlags |= 0x80;
                if (pPanel->m_nFlags & 8) {
                    bCovered = 1;
                    m_sResolution = pPanel->GetFullScreenBG();
                }
            }
        }
    }
    nCount = m_lstPanels.num;
    for (int j = nCount - 1; j >= 0; j--) {
        CSWGuiPanel* pPanel = m_lstPanels[j];
        if (pPanel && (pPanel->m_nFlags & 4) && GetModalIndex(pPanel) == -1) {
            if (bCovered) {
                pPanel->m_nFlags &= ~0x80;
            }
            else {
                pPanel->m_nFlags |= 0x80;
                if (pPanel->m_nFlags & 8) {
                    bCovered = 1;
                    m_sResolution = pPanel->GetFullScreenBG();
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00409f00
int CSWGuiManager::LoadGuiSounds()
{
    C2DA* p2DA = new C2DA("guisounds", 0);
    if (!p2DA) {
        return 0;
    }
    if (!p2DA->Load2DArray()) {
        delete p2DA;
        return 0;
    }
    if (p2DA->m_nNumRows > 0) {
        CExoString sResRef;
        int nRows = p2DA->m_nNumRows;
        m_nGuiSounds = nRows;
        m_pGuiSounds = new CExoSoundSource*[(char)nRows];
        if (m_pGuiSounds) {
            for (int i = 0; i < m_nGuiSounds; i++) {
                if (p2DA->GetCExoStringEntry(i, "soundresref", &sResRef)) {
                    m_pGuiSounds[i] = new CExoSoundSource(sResRef);
                    if (m_pGuiSounds[i]) {
                        m_pGuiSounds[i]->SetPriorityGroup(11);
                    }
                }
            }
        }
    }
    delete p2DA;
    return 1;
}

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x0040a0d0
void CSWGuiManager::UnloadGuiSounds()
{
    if (m_pGuiSounds) {
        for (int i = 0; i < m_nGuiSounds; i++) {
            if (m_pGuiSounds[i]) {
                delete m_pGuiSounds[i];
                m_pGuiSounds[i] = 0;
            }
        }
        delete[] m_pGuiSounds;
        m_pGuiSounds = 0;
    }
    m_nGuiSounds = 0;
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x0040a140
void CSWGuiManager::PlayGuiSound(char nSound)
{
    if (nSound != -1 && nSound < m_nGuiSounds && m_pGuiSounds && m_pGuiSounds[nSound]) {
        m_pGuiSounds[nSound]->Play();
    }
}
#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x0040a170
int CSWGuiManager::HandleLMouseUp()
{
    int bHandled = 0;
    if (m_pCaptureControl) {
        if (m_nCaptureType == 1) {
            m_pCaptureControl->HandleLMouseUp();
            bHandled = 1;
        }
        else {
            bHandled = 0;
        }
    }
    m_bLeftDown = 0;
    return bHandled;
}

// FUNCTION: SWKOTOR 0x0040a1a0
int CSWGuiManager::HandleRMouseUp()
{
    int bHandled = 0;
    if (m_pCaptureControl) {
        if (m_nCaptureType == 4) {
            m_pCaptureControl->HandleRMouseUp();
            bHandled = 1;
        }
        else {
            bHandled = 0;
        }
    }
    return bHandled;
}

// FUNCTION: SWKOTOR 0x0040a1c0
void CSWGuiManager::SetCapture(CSWGuiControl* pControl, int nType)
{
    if (m_pCaptureControl) {
        m_pCaptureControl->OnCaptureLost();
    }
    m_pCaptureControl = pControl;
    m_nCaptureType = nType;
}

// FUNCTION: SWKOTOR 0x0040a200
void CSWGuiManager::ReleaseCapture(CSWGuiControl* pControl)
{
    if (pControl && m_pCaptureControl == pControl) {
        m_pCaptureControl = 0;
        pControl->OnCaptureLost();
    }
    m_nCaptureType = 0;
}

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x0040a230
void CSWGuiManager::DeactivateSoftwareMouse()
{
    if (m_pMouseObject) {
        delete m_pMouseObject;
        m_pMouseObject = 0;
    }
    if (m_pMouse) {
        m_pMouse = 0;
    }
}
#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x0040a270
void CSWGuiManager::SetSoftwareMouseCursor(int nCursor, int bCentred, int bPressed)
{
    m_nCursor = nCursor;
    if (bPressed) {
        nCursor += nCursor % 2;
    }
    m_cCursor = g_aCursorNames[nCursor];
    if (m_pMouse) {
        m_pMouse->SetTexture("", m_cCursor.GetResRefStr());
        if (bCentred != m_bCentred) {
            m_pMouse->PlayAnimation(bCentred ? "center" : "default", 1.0f, 0, 0);
            m_pMouse->SetScale(1.0f);
            m_pMouse->PlayAnimation("", 0, 0x80, 0);
        }
    }
    m_bCentred = bCentred;
}

// FUNCTION: SWKOTOR 0x0040a340
void CSWGuiManager::ChangeToolTipText(int nText)
{
    if (m_pToolTip) {
        m_pToolTip->SetText(nText);
    }
}

// FUNCTION: SWKOTOR 0x0040a350
void CSWGuiManager::ReloadToolTipPanel()
{
    if (m_pToolTip) {
        delete m_pToolTip;
    }
    m_pToolTip = new CSWGuiToolTip(this);
}

// FUNCTION: SWKOTOR 0x0040a3d0
void CSWGuiManager::SaveMousePosition()
{
    m_nSavedMouseX = m_nMouseX;
    m_nSavedMouseY = m_nMouseY;
}

// FUNCTION: SWKOTOR 0x0040a3e0
CExoString CSWGuiManager::GetScreenResolutionString(int nWidth, int nHeight)
{
    if (nWidth == 1024) {
        if (nHeight == 768) {
            return CExoString("1024x768");
        }
    }
    else if (nWidth == 1280) {
        if (nHeight == 960) {
            return CExoString("1280x960");
        }
        if (nHeight == 1024) {
            return CExoString("1280x1024");
        }
    }
    else if (nWidth == 1600 && nHeight == 1200) {
        return CExoString("1600x1200");
    }
    return CExoString("800x600");
}

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x0040ab70
int CSWGuiManager::GetModalIndex(CSWGuiPanel* pPanel)
{
    for (int i = 0; i < m_lstModalPanels.num; i++) {
        if (m_lstModalPanels[i] == pPanel) {
            return i;
        }
    }
    return -1;
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x0040aba0
int CSWGuiManager::GetPanelIndex(CSWGuiPanel* pPanel)
{
    for (int i = 0; i < m_lstPanels.num; i++) {
        if (m_lstPanels[i] && m_lstPanels[i] == pPanel) {
            return i;
        }
    }
    return -1;
}
#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x0040abe0
int CSWGuiManager::HitCheckMouse(int x, int y, CSWGuiPanel** ppPanel, CSWGuiControl** ppControl, int nUnused)
{
    int nModal = m_lstModalPanels.num;
    if (nModal) {
        CSWGuiControl* pControl = m_lstModalPanels[nModal - 1]->HitCheckMouse(x, y);
        if (ppPanel) {
            if (pControl) {
                *ppPanel = m_lstModalPanels[nModal - 1];
            }
            else {
                *ppPanel = 0;
            }
        }
        if (ppControl) {
            *ppControl = pControl;
        }
        return (int)pControl;
    }
    for (int i = m_lstPanels.num - 1; i >= 0; i--) {
        CSWGuiControl* pControl = m_lstPanels[i]->HitCheckMouse(x, y);
        if (pControl) {
            if (ppPanel) {
                *ppPanel = m_lstPanels[i];
            }
            if (ppControl) {
                *ppControl = pControl;
            }
            return 1;
        }
    }
    if (ppPanel) {
        *ppPanel = 0;
    }
    if (ppControl) {
        *ppControl = 0;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0040adf0
int CSWGuiManager::IsPanelOnTop(CSWGuiPanel* pPanel)
{
    int i;
    for (i = m_lstModalPanels.num - 1; i >= 0; i--) {
        if (!(m_lstModalPanels[i]->m_nFlags & 0x600)) {
            return m_lstModalPanels[i] == pPanel;
        }
    }
    for (i = m_lstPanels.num - 1; i >= 0; i--) {
        if (!(m_lstPanels[i]->m_nFlags & 0x600)) {
            return m_lstPanels[i] == pPanel;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0040b290
CSWGuiMouse* CSWGuiManager::ShowSoftwareMouse()
{
    if (m_pMouse) {
        return m_pMouse;
    }
    return ActivateSoftwareMouse();
}

// FUNCTION: SWKOTOR 0x0040b2a0
int CSWGuiManager::SetFocusControl(int nArg)
{
    if (m_lstModalPanels.num) {
        CSWGuiPanel* pTop = m_lstModalPanels[m_lstModalPanels.num - 1];
        CSWGuiControl* pFocus = m_pFocusControl;
        if (pTop->GetFocusControl() == pFocus) {
            pFocus->HandleFocusEvent(nArg);
        }
        return 0;
    }
    if (m_pFocusControl) {
        m_pFocusControl->HandleFocusEvent(nArg);
        return 1;
    }
    return 0;
}

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x0040b300
void CSWGuiManager::UpdateMouseOverControl()
{
    CSWGuiPanel* pPanel;
    CSWGuiControl* pControl;
    HitCheckMouse(m_nMouseX, m_nMouseY, &pPanel, &pControl, 0);
    if (pControl != m_pMouseOverControl) {
        if (m_pMouseOverControl) {
            m_pMouseOverControl->SetHighlight(0);
        }
        m_pMouseOverControl = pControl;
    }
    if (m_pMouseOverControl != m_pLastMouseOverControl) {
        m_pLastMouseOverControl = pControl;
        m_bLeftDown = 0;
    }
    else {
        m_pLastMouseOverControl = pControl;
    }
}
#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x0040b360
void CSWGuiManager::GetUpdatedFontName(char* sName)
{
    if (strcmp("fnt_console", sName) == 0) {
        return;
    }
    unsigned char nLength = (unsigned char)strlen(sName);
    switch (g_pAppManager->m_pClientExoApp->GetOptions()->m_nFontSize) {
    case 1:
        sName[nLength] = 'a';
        sName[nLength + 1] = 0;
        break;
    case 2:
        sName[nLength] = 'b';
        sName[nLength + 1] = 0;
        break;
    default:
        if (g_nScreenWidth < 1280) {
            sName[nLength] = 'a';
            sName[nLength + 1] = 0;
        }
        else if (g_nScreenWidth < 2500) {
            sName[nLength] = 'b';
            sName[nLength + 1] = 0;
        }
        else {
            sName[nLength] = 'b';
            sName[nLength + 1] = 0;
        }
        break;
    }
}

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x0040b420
void CSWGuiManager::UpdateAllFonts()
{
    for (int i = 0; i < m_lstPanels.num; i++) {
        if (m_lstPanels[i]->m_bUpdateFonts) {
            m_lstPanels[i]->UpdateFonts();
        }
    }
    if (m_pToolTip) {
        m_pToolTip->UpdateFonts();
    }
    if (g_pAppManager->m_pClientExoApp->GetFontManager()) {
        g_pAppManager->m_pClientExoApp->GetFontManager()->UpdateFonts();
    }
}
#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x0040b490
void CSWGuiManager::DisplayToolTip(int nText)
{
    if (!m_bToolTipLocked && (g_pAppManager->m_pClientExoApp->GetOptions()->m_nFlags & 0x400)) {
        m_bToolTipShown = 1;
        m_pToolTip->SetText(nText);
        m_pToolTip->Show();
    }
}

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x0040b4d0
void CSWGuiManager::ResetToolTipTimer()
{
    if (!m_bToolTipLocked && (g_pAppManager->m_pClientExoApp->GetOptions()->m_nFlags & 0x400)) {
        m_fToolTipTimer = 0.0f;
    }
}

// FUNCTION: SWKOTOR 0x0040b500
void CSWGuiManager::SetTemporarilyDisableToolTip(int bDisable)
{
    if (bDisable) {
        m_fToolTipTimer = -1.0f;
        if (m_pMouseOverControl) {
            CSWGuiControl* pControl;
            if (m_pMouseOverControl->GetScrollBar()) {
                pControl = m_pMouseOverControl->GetScrollBar()->GetToolTipControl(0);
            }
            else {
                pControl = m_pMouseOverControl;
            }
            m_bToolTipLocked = bDisable;
            m_pToolTipControl = pControl;
            return;
        }
    }
    else {
        ResetToolTipTimer();
    }
    m_bToolTipLocked = bDisable;
    m_pToolTipControl = 0;
}
#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x0040bad0
CSWGuiManager::CSWGuiManager()
{
    m_nWidth = 0;
    m_nHeight = 0;
    m_nOpaquePanels = 0;
    m_nTransparentPanels = 0;
    m_n64 = 0;
    m_n72 = 0;
    m_p80 = 0;
    m_p84 = 0;
    m_pBackground = 0;
    m_sResolution = "800x600";
    m_sResolution = m_sResolution + g_aResolutionSuffixes[0];
    m_pMouseOverControl = 0;
    m_pCaptureControl = 0;
    m_nCaptureType = 0;
    m_bLeftDown = 0;
    m_bRightDown = 0;
    m_pLastMouseOverControl = 0;
    m_pFocusControl = 0;
    m_pMouse = 0;
    m_pMouseObject = 0;
    m_cCursor = "";
    m_bCentred = 0;
    m_nCursor = 1;
    m_pToolTip = new CSWGuiToolTip(this);
    m_fToolTipTimer = 0.0f;
    m_bToolTipShown = 0;
    m_pToolTipOwner = 0;
    m_bToolTipLocked = 0;
    m_pToolTipControl = 0;
    m_pDragControl = 0;
    m_pGuiSounds = 0;
    m_nGuiSounds = 0;
    g_pGuiManager = this;
    m_n68 = 0;
    if (g_pAppManager->m_pClientExoApp->GetOptions()) {
        m_fGuiScale = g_pAppManager->m_pClientExoApp->GetOptions()->GetGuiScale();
    }
    m_nSavedMouseX = -1;
    m_nSavedMouseY = -1;
}

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x0040bc70
void CSWGuiManager::AddPanel(CSWGuiPanel* pPanel, int nFlags, int bPlaySound)
{
    if (!pPanel) {
        return;
    }
    if (bPlaySound) {
        PlayGuiSound(pPanel->m_nOpenSound);
    }
    if (GetPanelIndex(pPanel) != -1) {
        unsigned long nFade = (pPanel->m_nFlags >> 9) & 3;
        if (nFade != 2 && nFade == 0) {
            return;
        }
        pPanel->m_nFlags &= ~0x700;
        if (nFlags & 1) {
            PushModalPanel(pPanel);
        }
        pPanel->OnPanelAdded();
        DetermineNeedToDraw();
    }
    else {
        pPanel->m_nDrawMode = (unsigned int)nFlags >> 1;
        if (pPanel->m_nFlags & 8) {
            m_nOpaquePanels++;
        }
        if (!(pPanel->m_nFlags & 0x10)) {
            m_nTransparentPanels++;
        }
        m_lstPanels.Add(pPanel);
        if (nFlags & 1) {
            PushModalPanel(pPanel);
        }
        pPanel->OnPanelAdded();
        DetermineNeedToDraw();
    }
}
#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x0040bd20
void CSWGuiManager::SendPanelToBack(CSWGuiPanel* pPanel)
{
    if (m_lstModalPanels.IndexOf(pPanel) == -1) {
        int nIndex = m_lstPanels.IndexOf(pPanel);
        if (nIndex != -1) {
            m_lstPanels.DelIndex(nIndex);
            m_lstPanels.Insert(pPanel, 0);
        }
        DetermineNeedToDraw();
    }
}

// FUNCTION: SWKOTOR 0x0040bd70
int CSWGuiManager::PanelExists(CSWGuiPanel* pPanel)
{
    return GetPanelIndex(pPanel) != -1;
}

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x0040bd90
int CSWGuiManager::PushModalPanel(CSWGuiPanel* pPanel)
{
    if (!pPanel) {
        return 0;
    }
    if (m_pMouseOverControl) {
        m_pMouseOverControl->SetHighlight(0);
    }
    if (GetPanelIndex(pPanel) == -1) {
        AddPanel(pPanel, 0, 1);
    }
    if (GetModalIndex(pPanel) != -1) {
        return 0;
    }
    if (m_pFocusControl) {
        m_pFocusControl->SetFocus(0);
    }
    m_lstModalPanels.Add(pPanel);
    return 1;
}

// FUNCTION: SWKOTOR 0x0040be00
CSWGuiPanel* CSWGuiManager::PopModalPanel()
{
    if (!m_lstModalPanels.num) {
        return 0;
    }
    CSWGuiPanel* pPanel = m_lstModalPanels[m_lstModalPanels.num - 1];
    m_lstModalPanels.DelIndex(m_lstModalPanels.num - 1);
    DetermineNeedToDraw();
    if (m_lstModalPanels.num > 0 && m_lstModalPanels[m_lstModalPanels.num - 1]->GetFocusControl()) {
        m_lstModalPanels[m_lstModalPanels.num - 1]->GetFocusControl()->SetFocus(1);
    }
    return pPanel;
}
#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x0040be70
void CSWGuiManager::SetSize(int nWidth, int nHeight)
{
    if (m_nWidth != nWidth || m_nHeight != nHeight) {
        m_nWidth = (short)nWidth;
        m_nHeight = (short)nHeight;
        UpdateAllFonts();
        if (m_pBackground) {
            delete m_pBackground;
            m_pBackground = 0;
            DetermineNeedToDraw();
        }
    }
}

// FUNCTION: SWKOTOR 0x0040c120
void CSWGuiManager::HideAllPanels()
{
    for (int i = 0; i < m_lstPanels.num; i++) {
        m_lstPanels[i]->SetVisible(0);
    }
}

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x0040c150
void CSWGuiManager::RemoveToolTip()
{
    m_bToolTipShown = 0;
    ResetToolTipTimer();
    if (m_pToolTipOwner) {
        m_pToolTipOwner->m_nFlags &= ~0x40;
        m_pToolTipOwner = 0;
    }
}
#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x0040c570
int CSWGuiManager::HandleLMouseDown(int bDown)
{
    m_bLeftDown = bDown;
    if (m_bToolTipShown) {
        RemoveToolTip();
    }
    if (m_pCaptureControl && m_nCaptureType == 1) {
        return 0;
    }
    if (g_pAppManager->m_pClientExoApp->GetInputMode() == 3) {
        for (int i = m_lstPanels.num - 1; i >= 0; i--) {
            m_lstPanels[i]->HandleInputEvent(0x1f9, 1);
        }
    }
    UpdateMouseOverControl();
    SetTemporarilyDisableToolTip(1);
    if (m_pMouseOverControl) {
        m_pMouseOverControl->HandleLMouseDown();
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0040c610
int CSWGuiManager::HandleRMouseDown()
{
    if (m_bToolTipShown) {
        RemoveToolTip();
    }
    if (m_pCaptureControl) {
        return 0;
    }
    UpdateMouseOverControl();
    SetTemporarilyDisableToolTip(1);
    if (m_pMouseOverControl) {
        m_pMouseOverControl->HandleRMouseDown();
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0040c650
int CSWGuiManager::HandleZMouse(int nDelta)
{
    if (m_bToolTipShown) {
        RemoveToolTip();
    }
    CSWGuiPanel* pPanel;
    CSWGuiControl* pControl;
    HitCheckMouse(m_nMouseX, m_nMouseY, &pPanel, &pControl, 0);
    if (pControl != m_pMouseOverControl) {
        m_pMouseOverControl = pControl;
    }
    if (!pControl) {
        return 0;
    }
    if (pControl->GetScrollBar()) {
        unsigned int nCount;
        if (nDelta > 0) {
            nCount = (unsigned int)(nDelta - 1) / 120 + 1;
            do {
                pControl->GetScrollBar()->HandleInputEvent(0x1f4, 1);
            } while (--nCount);
            if (m_bToolTipLocked == 1) {
                SetTemporarilyDisableToolTip(0);
            }
            pControl->GetScrollBar()->RefreshScroll();
        }
        else if (nDelta < 0) {
            nCount = (unsigned int)(-nDelta - 1) / 120 + 1;
            do {
                pControl->GetScrollBar()->HandleInputEvent(0x1f5, 1);
            } while (--nCount);
            if (m_bToolTipLocked == 1) {
                SetTemporarilyDisableToolTip(0);
            }
            pControl->GetScrollBar()->RefreshScroll();
        }
    }
    else {
        if (!pControl->IsControl()) {
            pControl = pControl->GetOwner();
        }
        if (pControl && pControl->IsSelectable() && nDelta) {
            pControl->HandleInputEvent(0x1f4 + (nDelta <= 0), 1);
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0040c790
void CSWGuiManager::MoveMouseToPosition(int x, int y)
{
    m_nMouseX = x;
    m_nMouseY = y;
    g_pExoInput->SetMousePosition(x, g_nScreenHeight - y - 1);
    HandleMouseMove(x, y);
}

// FUNCTION: SWKOTOR 0x0040c7d0
void CSWGuiManager::RestoreOldMousePosition()
{
    if (m_nSavedMouseX >= 0) {
        m_nMouseX = m_nSavedMouseX;
        m_nMouseY = m_nSavedMouseY;
        g_pExoInput->SetMousePosition(m_nMouseX, g_nScreenHeight - m_nMouseY - 1);
        g_pAppManager->m_pClientExoApp->m_pInternal->SetMousePosition(m_nMouseX, m_nMouseY);
        m_nSavedMouseX = -1;
        m_nSavedMouseY = -1;
        HandleMouseMove(m_nMouseX, m_nMouseY);
    }
}

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x0040c830
int CSWGuiManager::RemovePanel(CSWGuiPanel* pPanel)
{
    if (!pPanel) {
        return 0;
    }
    if (GetPanelIndex(pPanel) == -1) {
        return 0;
    }
    int nModal = GetModalIndex(pPanel);
    if (nModal != -1) {
        if (nModal == m_lstModalPanels.num - 1) {
            PopModalPanel();
        }
        else {
            m_lstModalPanels.DelIndex(nModal);
        }
    }
    pPanel->OnPanelRemoved();
    int nIndex = GetPanelIndex(pPanel);
    if (nIndex == -1) {
        return 0;
    }
    m_lstPanels.DelIndex(nIndex);
    if (pPanel->m_nFlags & 8) {
        m_nOpaquePanels--;
    }
    if (!(pPanel->m_nFlags & 0x10)) {
        m_nTransparentPanels--;
    }
    DetermineNeedToDraw();
    m_pMouseOverControl = 0;
    HandleMouseMove(m_nMouseX, m_nMouseY);
    m_pCaptureControl = 0;
    return 1;
}
#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x00409ee0
void CSWGuiManager::GetSafeMargins(int* pX, int* pY)
{
    *pX = 0x30;
    *pY = 0x24;
}

// FUNCTION: SWKOTOR 0x0040cc50
void CSWGuiManager::Draw(float fDelta)
{
    int nPanels = m_lstPanels.num;
    GuiBeginFrame();
    GuiUpdateFrame(fDelta);
    GuiUpdateBorders(fDelta);
    if (m_nOpaquePanels > 0) {
        DrawPCBG(fDelta);
    }
    for (int i = 0; i < nPanels; i++) {
        CSWGuiPanel* pPanel = m_lstPanels[i];
        if (pPanel) {
            if ((pPanel->m_nFlags & 4) && GetModalIndex(pPanel) == -1) {
                pPanel->Draw(fDelta);
            }
        }
        else {
            m_lstPanels.DelIndex(i);
            i--;
        }
    }
    int nModal = m_lstModalPanels.num;
    for (int j = 0; j < nModal; j++) {
        CSWGuiPanel* pPanel = m_lstModalPanels[j];
        if (pPanel) {
            if (pPanel->m_nFlags & 4) {
                pPanel->Draw(fDelta);
            }
        }
        else {
            m_lstModalPanels.DelIndex(j);
            j--;
        }
    }
    if (m_pDragControl) {
        AurGUIStartLayer();
        Vector vColor;
        vColor.x = -1.0f;
        vColor.y = -1.0f;
        vColor.z = -1.0f;
        if (AurGUISetupViewport(0, 0, m_nWidth, m_nHeight, &vColor, 0, 1.0f)) {
            m_pDragControl->DrawAt(m_nMouseX, m_nMouseY);
            AurGUICloseViewport();
        }
        AurGUIStopLayer();
    }
    if (!g_pAppManager->m_pClientExoApp->GetInFreeLook()) {
        if (m_bToolTipShown) {
            m_pToolTip->Draw(fDelta);
        }
        if (m_fToolTipTimer >= 0.0f) {
            m_fToolTipTimer += fDelta;
            if (m_fToolTipTimer > m_fGuiScale) {
                if (m_pMouseOverControl && m_pMouseOverControl->HasToolTip()) {
                    m_pToolTipOwner = m_pMouseOverControl;
                }
                m_fToolTipTimer = -1.0f;
            }
        }
    }
    if (m_pMouseObject) {
        AurGUIStartLayer();
        m_pMouseObject->Draw(fDelta);
        AurGUIStopLayer();
    }
    for (int k = m_lstPanels.num - 1; k >= 0; k--) {
        CSWGuiPanel* pPanel = m_lstPanels[k];
        unsigned long nFade = pPanel->m_nFadeState;
        if (nFade == 2) {
            RemovePanel(pPanel);
            delete pPanel;
        }
        else if (nFade) {
            pPanel->m_nFlags &= ~0x700;
            RemovePanel(pPanel);
        }
    }
    if (g_bDrawSafeRegion) {
        DrawSafeRegion(fDelta);
    }
}

// FUNCTION: SWKOTOR 0x0040ce70
void CSWGuiManager::Update(float fDelta)
{
    int nPanels = m_lstPanels.num;
    for (int i = 0; i < nPanels; i++) {
        CSWGuiPanel* pPanel = m_lstPanels[i];
        if (pPanel) {
            if ((pPanel->m_nFlags & 4) && GetModalIndex(pPanel) == -1) {
                pPanel->Update(fDelta);
            }
        }
        else {
            m_lstPanels.DelIndex(i);
            i--;
        }
    }
    int nModal = m_lstModalPanels.num;
    for (int j = 0; j < nModal; j++) {
        CSWGuiPanel* pPanel = m_lstModalPanels[j];
        if (pPanel) {
            if (pPanel->m_nFlags & 4) {
                pPanel->Update(fDelta);
            }
        }
        else {
            m_lstModalPanels.DelIndex(j);
            j--;
        }
    }
    for (int k = m_lstPanels.num - 1; k >= 0; k--) {
        CSWGuiPanel* pPanel = m_lstPanels[k];
        if (!(pPanel->m_nFlags & 0x100)) {
            unsigned long nFade = pPanel->m_nFadeState;
            if (nFade == 2) {
                RemovePanel(pPanel);
                delete pPanel;
            }
            else if (nFade) {
                pPanel->m_nFlags &= ~0x700;
                RemovePanel(pPanel);
            }
        }
    }
}
