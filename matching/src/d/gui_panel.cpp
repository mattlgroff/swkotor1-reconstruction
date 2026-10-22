// CSWGuiObject / CSWGuiExtent / CSWGuiPanel.
// Semantics from src/batches/b0040_gui_manager_panel.cpp (x86 confirmed).
#include "d/gui.h"

// Members the original calls out of line are wrapped in
// #pragma auto_inline(off) (MSVC inlines across the whole TU otherwise).

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x00409dc0
void CSWGuiExtent::Load(CResGFF* pGFF, CResStruct* pStruct)
{
    CResStruct cExtent;
    if (pGFF->GetStructFromStruct(&cExtent, pStruct, "EXTENT")) {
        int bSuccess;
        left = pGFF->ReadFieldINT(&cExtent, "LEFT", bSuccess, 0);
        top = pGFF->ReadFieldINT(&cExtent, "TOP", bSuccess, 0);
        width = pGFF->ReadFieldINT(&cExtent, "WIDTH", bSuccess, 0);
        height = pGFF->ReadFieldINT(&cExtent, "HEIGHT", bSuccess, 0);
    }
}
#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x00409e60
void CSWGuiPanel::HandleInputEvent(int nEvent, int bDown)
{
    if (m_pActiveControl) {
        m_pActiveControl->HandleInputEvent(nEvent, bDown);
    }
}

// FUNCTION: SWKOTOR 0x00409e80
CSWGuiObject::~CSWGuiObject()
{
}

// FUNCTION: SWKOTOR 0x00409e90
void CSWGuiObject::SetExtent(const CSWGuiExtent& extent)
{
    m_extent = extent;
}

// FUNCTION: SWKOTOR 0x00409eb0
int CSWGuiObject::v10(int a, int b)
{
    return 0;
}

// FUNCTION: SWKOTOR 0x00409ec0 SYMBOL: ??_GCSWGuiObject@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0040a4a0
CSWGuiExtent IntersectExtents(const CSWGuiExtent& a, const CSWGuiExtent& b)
{
    CSWGuiExtent r;
    r.left = a.left > b.left ? a.left : b.left;
    r.top = a.top > b.top ? a.top : b.top;
    r.width = (a.left + a.width < b.left + b.width ? a.left + a.width : b.left + b.width) - r.left;
    r.height = (a.top + a.height < b.top + b.height ? a.top + a.height : b.top + b.height) - r.top;
    return r;
}

// FUNCTION: SWKOTOR 0x0040a520
CSWGuiExtent UnionExtents(const CSWGuiExtent& a, const CSWGuiExtent& b)
{
    CSWGuiExtent r;
    r.left = a.left < b.left ? a.left : b.left;
    r.top = a.top < b.top ? a.top : b.top;
    r.width = (a.left + a.width > b.left + b.width ? a.left + a.width : b.left + b.width) - r.left;
    r.height = (a.top + a.height > b.top + b.height ? a.top + a.height : b.top + b.height) - r.top;
    return r;
}

// FUNCTION: SWKOTOR 0x0040a5a0
void CSWGuiPanel::SetExtent(const CSWGuiExtent& extent)
{
    m_extent = extent;
    CSWGuiExtent cInner;
    cInner.left = 0;
    cInner.top = 0;
    cInner.width = extent.width;
    cInner.height = extent.height;
    m_pBorder->SetExtent(&cInner);
}

// FUNCTION: SWKOTOR 0x0040a600
void CSWGuiPanel::CenterPanel()
{
    m_extent.left = (m_pManager->m_nWidth - m_extent.width) / 2;
    m_extent.top = (m_pManager->m_nHeight - m_extent.height) / 2;
    m_nFlags |= 1;
}

// FUNCTION: SWKOTOR 0x0040a630
void CSWGuiPanel::SetActiveControl(CSWGuiControl* pControl, int bSound)
{
    if (m_pActiveControl != pControl) {
        if (m_pActiveControl) {
            m_pActiveControl->HandleInputEvent(1, 1);
        }
        m_pActiveControl = pControl;
        if (pControl) {
            pControl->HandleInputEvent(0, 1);
            if (bSound && m_pManager) {
                m_pManager->PlayGuiSound(pControl->m_nClickSound);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0040a680
void CSWGuiPanel::StartLoadFromLayout(const CResRef& cResRef)
{
    if (m_nFlags & 2) {
        return;
    }
    CResGFF* pGFF = new CResGFF(0x7ff, "GUI ", cResRef);
    m_pGFF = pGFF;
    if (pGFF->Demand()) {
        CResStruct cTop;
        pGFF->GetTopLevelStruct(&cTop);
        m_extent.Load(pGFF, &cTop);
        int bSuccess;
        {
            Vector vDefault;
            vDefault.x = -1.0f;
            vDefault.y = -1.0f;
            vDefault.z = -1.0f;
            m_vColor = pGFF->ReadFieldVector(&cTop, "COLOR", bSuccess, vDefault);
        }
        CResStruct cBorder;
        if (pGFF->GetStructFromStruct(&cBorder, &cTop, "BORDER")) {
            m_pBorder->Load(pGFF, &cTop, "BORDER");
            CSWGuiExtent cInner;
            cInner.left = 0;
            cInner.top = 0;
            cInner.width = m_extent.width;
            cInner.height = m_extent.height;
            m_pBorder->SetExtent(&cInner);
        }
        else {
            CResRef cDefault("");
            CSWGuiBorderParams* pParams = &m_pBorder->m_cParams;
            pParams->SetFillImage(pGFF->ReadFieldCResRef(&cTop, "BACKGROUND", bSuccess, cDefault));
        }
        SetAlpha(pGFF->ReadFieldFLOAT(&cTop, "ALPHA", bSuccess, 1.0f));
        pGFF->GetList(&m_cControls, &cTop, "CONTROLS");
        m_nFlags |= 2;
    }
}

// FUNCTION: SWKOTOR 0x0040a870
void CSWGuiPanel::SetBackground(const CResRef& cResRef)
{
    m_pBorder->m_cParams.SetFillImage(cResRef, 0);
}

// FUNCTION: SWKOTOR 0x0040a900
CExoString CSWGuiPanel::GetFullScreenBG()
{
    CExoString sName = CSWGuiManager::GetScreenResolutionString(g_nScreenWidth, g_nScreenHeight);
    sName = sName + g_aResolutionSuffixes[m_nResolution];
    return sName;
}

// FUNCTION: SWKOTOR 0x0040a9c0
void CSWGuiPanel::UpdateFonts()
{
    if (m_bUpdateFonts) {
        for (int i = 0; i < m_lstControls.num; i++) {
            if (m_lstControls[i]) {
                m_lstControls[i]->OnPanelLoaded();
            }
        }
    }
}

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x0040aa00
void CSWGuiPanel::GetAbsoluteExtent(CSWGuiExtent* pExtent)
{
    *pExtent = m_extent;
    if (m_nFlags & 8) {
        pExtent->left += (m_pManager->m_nWidth - pExtent->width) / 2;
        pExtent->top += (m_pManager->m_nHeight - pExtent->height) / 2;
        return;
    }
    if (m_nFlags & 0x20) {
        pExtent->left += (m_pManager->m_nWidth - 640) / 2;
    }
    if (m_nFlags & 0x40) {
        pExtent->top += (m_pManager->m_nHeight - 480) / 2;
    }
}
#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x0040b570
CSWGuiPanel::CSWGuiPanel(CSWGuiManager* pManager)
    : m_lstControls(0)
{
    m_vColor.x = 0;
    m_vColor.y = 0;
    m_vColor.z = 0;
    m_pBorder = new CSWGuiBorder;
    m_pManager = pManager;
    m_pActiveControl = 0;
    m_pGFF = 0;
    m_nResolution = 0;
    m_bCentred = 0;
    m_bLoaded = 0;
    m_bVisible = 1;
    m_bCentreX = 0;
    m_bCentreY = 0;
    m_bDraw = 1;
    m_bFlag8 = 0;
    m_nFadeState = 0;
    m_fAlpha = 1.0f;
    m_nOpenSound = 0xff;
    m_bUpdateFonts = 1;
}

// FUNCTION: SWKOTOR 0x0040b630
CSWGuiControl* CSWGuiPanel::GetFocusControl()
{
    return m_pActiveControl;
}

// FUNCTION: SWKOTOR 0x0040b640
void CSWGuiPanel::HandleInputEvent27(int n)
{
    HandleInputEvent(0x27, 1);
}

// FUNCTION: SWKOTOR 0x0040b650
void CSWGuiPanel::HandleInputEvent28(int n)
{
    HandleInputEvent(0x28, 1);
}

// FUNCTION: SWKOTOR 0x0040b660
void CSWGuiPanel::HandleInputEvent29(int n)
{
    HandleInputEvent(0x29, 1);
}

// FUNCTION: SWKOTOR 0x0040b670
void CSWGuiPanel::HandleInputEvent2A(int n)
{
    HandleInputEvent(0x2a, 1);
}

// FUNCTION: SWKOTOR 0x0040b680
void CSWGuiPanel::HandleInputEvent2B(int n)
{
    HandleInputEvent(0x2b, 1);
}

// FUNCTION: SWKOTOR 0x0040b690
CSWGuiControl* CSWGuiPanel::HitCheckMouse(int x, int y)
{
    CSWGuiExtent cExtent = m_extent;
    if (!(m_nFlags & 1)) {
        x += -((g_nScreenWidth - 640) / 2);
        y += -((g_nScreenHeight - 480) / 2);
    }
    else {
        GetAbsoluteExtent(&cExtent);
    }
    if (x >= cExtent.left && x <= cExtent.left + cExtent.width &&
        y >= cExtent.top && y <= cExtent.top + cExtent.height) {
        int nX = x - cExtent.left;
        int nY = y - cExtent.top;
        for (int i = m_lstControls.num - 1; i >= 0; i--) {
            CSWGuiControl* pControl = m_lstControls[i];
            if (pControl) {
                CSWGuiControl* pHit = pControl->HitCheckMouse(nX, nY);
                if (pHit) {
                    return pHit;
                }
            }
        }
    }
    return 0;
}

inline int IsValidExtent(const CSWGuiExtent& e)
{
    return e.left >= 0 && e.top >= 0 && e.height > 0 && e.width > 0;
}

// FUNCTION: SWKOTOR 0x0040b760
void CSWGuiPanel::Draw(float fDelta)
{
    if (m_bDraw && IsValidExtent(m_extent)) {
        if (m_extent.width + m_extent.left <= m_pManager->m_nWidth &&
            m_extent.height + m_extent.top <= m_pManager->m_nHeight) {
            CSWGuiExtent cExtent = m_extent;
            int nControls = m_lstControls.num;
            GetAbsoluteExtent(&cExtent);
            AurGUIStartLayer();
            if (AurGUISetupViewport(cExtent.left, cExtent.top, cExtent.width, cExtent.height, &m_vColor, 0, m_fAlpha)) {
                m_pBorder->Draw(fDelta);
                for (int i = 0; i < nControls; i++) {
                    CSWGuiControl* pControl = m_lstControls[i];
                    if (pControl && (pControl->m_nFlags & 2)) {
                        pControl->Draw(fDelta);
                    }
                }
                AurGUICloseViewport();
            }
            AurGUIStopLayer();
        }
    }
}

// FUNCTION: SWKOTOR 0x0040b870
void CSWGuiPanel::OnPanelAdded()
{
    UpdateFonts();
    g_pGuiManager->UpdateMouseOverControl();
    CSWGuiControl* pControl = m_pManager->m_pMouseOverControl;
    if (pControl) {
        int nControls = m_lstControls.num;
        for (int i = 0; i < nControls; i++) {
            if (m_lstControls[i] == pControl) {
                if (pControl->m_nFlags & 8) {
                    m_pManager->m_pMouseOverControl = pControl;
                    pControl->SetHighlight(1);
                    if (m_pActiveControl && pControl != m_pActiveControl &&
                        pControl->IsSelectable() && (pControl->m_nFlags & 4)) {
                        m_pActiveControl->HandleInputEvent(1, 1);
                    }
                }
                return;
            }
        }
    }
}

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x0040b8f0
void CSWGuiPanel::UnloadResource()
{
    if (m_nFlags & 2) {
        m_pGFF->Release();
        delete m_pGFF;
        m_pGFF = 0;
        m_nFlags &= ~2;
        SetupNavigation();
    }
}
#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x0040b930
void CSWGuiPanel::InitControl(CSWGuiControl* pControl, int nArg, int bAdd)
{
    pControl->Initialize(this, m_pGFF, &m_cControls, nArg);
    if (bAdd) {
        int nIndex = pControl->m_nIndex;
        if (nIndex >= 0) {
            if (nIndex < m_lstControls.num) {
                m_lstControls[nIndex] = pControl;
                return;
            }
            while (nIndex > m_lstControls.num) {
                m_lstControls.Add(0);
            }
            m_lstControls.Add(pControl);
        }
    }
}

// FUNCTION: SWKOTOR 0x0040b9a0
void CSWGuiPanel::AddControl(CSWGuiControl* pControl)
{
    if (m_lstControls.IndexOf(pControl) == -1) {
        pControl->m_pParent = this;
        m_lstControls.Add(pControl);
    }
}

// FUNCTION: SWKOTOR 0x0040b9d0
void CSWGuiPanel::SetVisible(int bVisible)
{
    m_bVisible = bVisible;
    if (m_pManager) {
        m_pManager->DetermineNeedToDraw();
    }
}

// FUNCTION: SWKOTOR 0x0040ba00
CSWGuiControl* CSWGuiPanel::GetControl(int nIndex)
{
    if (nIndex < m_lstControls.num) {
        return m_lstControls[nIndex];
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0040c170
void CSWGuiPanel::OnPanelRemoved()
{
    g_pGuiManager->RemoveToolTip();
    CSWGuiControl* pFocus = g_pGuiManager->m_pFocusControl;
    if (pFocus && pFocus == GetFocusControl()) {
        g_pGuiManager->m_pFocusControl->SetFocus(0);
    }
    CSWGuiManager* pManager = m_pManager;
    CSWGuiControl* pControl = pManager->m_pMouseOverControl;
    if (pControl) {
        int nControls = m_lstControls.num;
        for (int i = 0; i < nControls; i++) {
            if (m_lstControls[i] == pControl) {
                pManager->m_pMouseOverControl = 0;
                pControl->SetHighlight(0);
                return;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0040cf70
CSWGuiPanel::~CSWGuiPanel()
{
    delete m_pBorder;
    m_pBorder = 0;
    UnloadResource();
    delete m_pGFF;
    m_pGFF = 0;
    CSWGuiManager* pManager = m_pManager;
    if (pManager && pManager->PanelExists(this)) {
        pManager->RemovePanel(this);
    }
}

// FUNCTION: SWKOTOR 0x0040d010 SYMBOL: ??_GCSWGuiPanel@@UAEPAXI@Z
