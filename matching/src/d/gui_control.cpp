// CSWGuiControl and the control classes.
// Semantics from src/batches/b0041_gui_controls.cpp (x86 confirmed).
#include "d/gui_control.h"

void operator delete(void* p);
void* operator new(unsigned int n);

inline int IsPointInExtent(const CSWGuiExtent& ext, int x, int y)
{
    return x >= ext.left && x <= ext.width + ext.left && y >= ext.top && y <= ext.height + ext.top;
}

inline int IsPointInExtentCopy(CSWGuiExtent ext, int x, int y)
{
    return x >= ext.left && x <= ext.width + ext.left && y >= ext.top && y <= ext.height + ext.top;
}

#pragma auto_inline(off)

// The two element types the original instantiates here are explicit
// specialisations (one body, GUI_ARRAY_LIST); CExoArrayList<CSWGuiControl*>
// stays out of line (0x00416770 / 0x00671c00).
#define GUI_ARRAY_LIST_ALLOCATE(T)                                   \
    template <>                                             \
    void CExoArrayList<T>::Allocate(int nSize)              \
    {                                                       \
        T* pOld = element;                                  \
        array_size = nSize;                                 \
        element = new T[nSize];                             \
        for (int i = 0; i < num; i++) {                     \
            element[i] = pOld[i];                           \
        }                                                   \
        if (pOld) {                                         \
            delete[] pOld;                                  \
        }                                                   \
    }
#define GUI_ARRAY_LIST_CTOR_ADD(T)                          \
    template <>                                             \
    CExoArrayList<T>::CExoArrayList(int nSize)              \
    {                                                       \
        num = 0;                                            \
        array_size = 0;                                     \
        element = 0;                                        \
        if (nSize) {                                        \
            Allocate(nSize);                                \
        }                                                   \
    }                                                       \
    template <>                                             \
    void CExoArrayList<T>::Add(T t)                         \
    {                                                       \
        if (num == array_size) {                            \
            Allocate(array_size ? array_size * 2 : 16);     \
        }                                                   \
        element[num++] = t;                                 \
    }

// FUNCTION: SWKOTOR 0x004186e0 SYMBOL: ?Allocate@?$CExoArrayList@UCSWGuiControlEvent@@@@QAEXH@Z
GUI_ARRAY_LIST_ALLOCATE(CSWGuiControlEvent)
// FUNCTION: SWKOTOR 0x0041a870 SYMBOL: ??0?$CExoArrayList@UCSWGuiControlEvent@@@@QAE@H@Z
// FUNCTION: SWKOTOR 0x0041a8a0 SYMBOL: ?Add@?$CExoArrayList@UCSWGuiControlEvent@@@@QAEXUCSWGuiControlEvent@@@Z
GUI_ARRAY_LIST_CTOR_ADD(CSWGuiControlEvent)

// FUNCTION: SWKOTOR 0x0041b830 SYMBOL: ??0?$CExoArrayList@UCSWGuiExtent@@@@QAE@H@Z
// FUNCTION: SWKOTOR 0x0041b860 SYMBOL: ?Add@?$CExoArrayList@UCSWGuiExtent@@@@QAEXUCSWGuiExtent@@@Z
GUI_ARRAY_LIST_CTOR_ADD(CSWGuiExtent)

// ---------------------------------------------------------------------------
// CSWGuiScrollBar leaves

// FUNCTION: SWKOTOR 0x00418010
int CSWGuiScrollBar::CanScroll()
{
    if (m_nVisibleValue < m_nMaxValue && m_nCurValue >= 1) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00418030
int CSWGuiScrollBar::IsVisible()
{
    int nMax = m_nMaxValue;
    if (!m_bAlwaysShow && m_nVisibleValue <= 1 && m_nCurValue != m_nMaxValue) {
        nMax++;
    }
    if (m_nVisibleValue < m_nMaxValue && m_nCurValue + m_nVisibleValue < nMax) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00418070
void CSWGuiScrollBar::SetMaxValue(int nMax, unsigned char bFlag)
{
    if (m_nMaxValue != nMax && nMax >= 0) {
        m_nMaxValue = nMax;
        m_bAlwaysShow = bFlag;
        SetExtent(m_extent);
    }
}

// FUNCTION: SWKOTOR 0x004180a0
void CSWGuiScrollBar::SetCurValue(int nValue)
{
    if (nValue < 0) {
        nValue = 0;
    } else if (nValue > m_nMaxValue) {
        nValue = m_nMaxValue;
    }
    m_nCurValue = nValue;
    SetExtent(m_extent);
}

// FUNCTION: SWKOTOR 0x004180d0
void CSWGuiScrollBar::SetVisibleValue(int nValue)
{
    if (m_nVisibleValue != nValue) {
        if (nValue >= 0 && (nValue != 0 || m_nMaxValue == 0)) {
            m_nVisibleValue = nValue;
        } else {
            m_nVisibleValue = 1;
        }
    }
    SetExtent(m_extent);
}

// FUNCTION: SWKOTOR 0x004181d0
void CSWGuiControl::SetFlag0(int b)
{
    m_bFlag0 = b;
}

// FUNCTION: SWKOTOR 0x00418660
void CSWGuiEditbox::SetFocus()
{
    HandleFocusChange(1);
}

// FUNCTION: SWKOTOR 0x00418670
int CSWGuiEditbox::IsSelectable()
{
    // Read the flag byte itself; the bitfield accessor compiles as a dword load.
    __asm {
        movzx eax, byte ptr [ecx + 0x44]
        shr eax, 3
        and eax, 1
    }
}

// FUNCTION: SWKOTOR 0x00418680
void CSWGuiRadioButton::HandleLMouseDown()
{
    if (m_bSelectable) {
        HandleInputEvent(0, 1);
        HandleFocusChange(1);
        g_pGuiManager->SetCapture(this, 1);
    }
}

// FUNCTION: SWKOTOR 0x004186b0
void CSWGuiEditbox::HandleLMouseUp()
{
    g_pGuiManager->ReleaseCapture(this);
    OnCaptureLost();
}

// FUNCTION: SWKOTOR 0x004186d0
void CSWGuiEditbox::UpdateFont()
{
    m_cText.m_cParams.UpdateFont();
}

// FUNCTION: SWKOTOR 0x00418750
void CSWGuiControl::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0:
            SetFlag0(1);
            break;
        case 1:
            if (m_bFlag0) {
                SetFlag0(0);
            }
            break;
        }
    }
    CSWGuiControlEvent* pEvent = m_lstEvents.element;
    for (int i = 0; i < m_lstEvents.num; i++, pEvent++) {
        if (pEvent->m_nEvent == nEvent && pEvent->m_pfnCallback) {
            m_n48 = nEvent;
            m_n4c = bDown;
            (pEvent->m_pObject->*pEvent->m_pfnCallback)(this);
            return;
        }
    }
}

// FUNCTION: SWKOTOR 0x004187c0
CSWGuiControl* CSWGuiControl::HitCheckMouse(int x, int y)
{
    if (m_bFlag1 && !m_bFlag5 && IsPointInExtent(m_extent, x, y)) {
        return this;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00418800 SYMBOL: ?Load@CSWGuiControl@@QAEXPAVCSWGuiObject@@PAVCResGFF@@PAUCResStruct@@AAVCExoString@@@Z
void CSWGuiControl::Load(CSWGuiObject* pOwner, CResGFF* pGFF, CResStruct* pStruct, CExoString& sName)
{
    m_pOwner = pOwner;
    CResStruct cStruct;
    if (pGFF->GetStructFromStruct(&cStruct, pStruct, sName.CStr())) {
        Load(pGFF, &cStruct);
    }
}

// FUNCTION: SWKOTOR 0x00418960
void CSWGuiControl::HandleFocusChange(int bFocus)
{
    if (bFocus && m_pOwner) {
        CSWGuiPanel* pPanel = m_pOwner->GetPanel();
        if (pPanel) {
            if (!m_bFlag0) {
                g_pGuiManager->PlayGuiSound(1);
            }
            pPanel->SetActiveControl(this, 0);
        }
    }
}

// FUNCTION: SWKOTOR 0x004189a0
void CSWGuiControl::HandleMouseOver(int bOver)
{
    if (bOver) {
        g_pGuiManager->ResetToolTipTimer();
    } else if (m_bToolTipShown) {
        HideToolTip();
    }
}

// FUNCTION: SWKOTOR 0x004189d0
int CSWGuiControl::IsSelectable()
{
    CSWGuiObject* pOwner = m_pOwner;
    if (pOwner && pOwner->GetListBox()) {
        if (pOwner->GetListBox()->m_bSelectable) {
            return GetNavigation() != 0;
        }
    } else if (m_bSelectable) {
        return GetNavigation() != 0;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00418a20
int CSWGuiControl::HasChildControl(CSWGuiControl* pControl)
{
    for (int i = 0; i < m_lstChildren.num; i++) {
        if (m_lstChildren[i] == pControl) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00418a50
CSWGuiControl* CSWGuiControl::GetOwner()
{
    CSWGuiControl* pControl = this;
    while (pControl->m_pParentControl) {
        pControl = pControl->m_pParentControl;
        if (pControl->IsSelectable()) {
            return pControl;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00418cd0
void CSWGuiLabel::CopyStyle(const CSWGuiExtent& extent, CSWGuiLabel* pFrom, float fScale)
{
    SetStyle(extent, &pFrom->m_cText.m_cParams, &pFrom->m_cBorder.m_cParams, fScale);
}

// FUNCTION: SWKOTOR 0x00418d00
void CSWGuiLabel::SetEnabled(int b)
{
    CSWGuiControl::SetEnabled(b);
    if (b) {
        m_cText.m_cParams.ResetColor();
    } else {
        m_cText.m_cParams.SetColor(&g_vDisabledColor);
    }
}

// FUNCTION: SWKOTOR 0x00418d40
void CSWGuiLabel::UpdateFont()
{
    m_cText.m_cParams.UpdateFont();
}

// FUNCTION: SWKOTOR 0x00418d50
void CSWGuiLabelHilight::CopyStyle(const CSWGuiExtent& extent, CSWGuiLabelHilight* pFrom)
{
    SetStyle(extent, &pFrom->m_cText.m_cParams, &pFrom->m_cBorder.m_cParams, &pFrom->m_cHilight.m_cParams);
}

// FUNCTION: SWKOTOR 0x00418d80
void CSWGuiButton::CopyStyle(const CSWGuiExtent& extent, CSWGuiButton* pFrom)
{
    SetStyle(extent, &pFrom->m_cText.m_cParams, &pFrom->m_cBorder.m_cParams, &pFrom->m_cHilight.m_cParams);
}

// FUNCTION: SWKOTOR 0x00418db0
void CSWGuiButton::SetEnabled(int b)
{
    CSWGuiControl::SetEnabled(b);
    if (b) {
        m_cText.m_cParams.ResetColor();
    } else {
        m_cText.m_cParams.SetColor(&g_vDisabledColor);
        SetFlag0(0);
    }
}

// FUNCTION: SWKOTOR 0x00418df0
void CSWGuiButton::UpdateFont()
{
    m_cText.m_cParams.UpdateFont();
}

// FUNCTION: SWKOTOR 0x00418e00
void CSWGuiButton::SetFlag0(int b)
{
    m_bFlag0 = b;
    CSWGuiTextParams* pParams = &m_cText.m_cParams;
    if (b) {
        pParams->m_bHilight = 1;
        pParams->SetColor(&g_vHilightColor);
    } else {
        pParams->m_bHilight = 0;
        pParams->ResetColor();
    }
}

// ---------------------------------------------------------------------------
// CSWGuiControl

// FUNCTION: SWKOTOR 0x00419a20
CSWGuiControl::~CSWGuiControl()
{
}

// FUNCTION: SWKOTOR 0x00419cc0 SYMBOL: ??_GCSWGuiControl@@UAEPAXI@Z
// FUNCTION: SWKOTOR 0x00419ce0 SYMBOL: ??1CSWGuiNavigable@@UAE@XZ
// FUNCTION: SWKOTOR 0x00419cf0 SYMBOL: ??_GCSWGuiNavigable@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00419d10
CSWGuiLabel::~CSWGuiLabel()
{
}

// FUNCTION: SWKOTOR 0x00419d80 SYMBOL: ??_GCSWGuiLabel@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00419da0
CSWGuiButton::~CSWGuiButton()
{
}

// FUNCTION: SWKOTOR 0x00419e20 SYMBOL: ??_GCSWGuiButton@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00419e40
CSWGuiScrollBar::~CSWGuiScrollBar()
{
}

// FUNCTION: SWKOTOR 0x00419ec0 SYMBOL: ??_GCSWGuiScrollBar@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0041aa80
CSWGuiControl::CSWGuiControl()
{
    m_bFlag0 = 0;
    m_bFlag1 = 1;
    m_bSelectable = 1;
    m_bFlag4 = 0;
    m_bFlag5 = 0;
    m_bToolTipShown = 0;
    m_pOwner = 0;
    m_n58 = 0;
    m_nSound = 0;
    m_pParentControl = 0;
    m_nToolTipStrRef = 0;
    m_nKeyBinding = 0;
    m_nId = -1;
    m_nClickSound = 1;
}

// FUNCTION: SWKOTOR 0x0041a980
void CSWGuiNavigable::HandleMouseOver(int bOver)
{
    if (bOver) {
        if (!m_bFlag0) {
            HandleInputEvent(0, 1);
            g_pGuiManager->PlayGuiSound(1);
        }
    } else {
        HandleInputEvent(1, 1);
    }
    CSWGuiControl::HandleMouseOver(bOver);
}

// FUNCTION: SWKOTOR 0x0041ab20
void CSWGuiControl::AddEvent(int nEvent, CSWGuiPanel* pObject, PFN_GUI_CALLBACK pfnCallback)
{
    CSWGuiControlEvent* pEvent = m_lstEvents.element;
    for (int i = 0; i < m_lstEvents.num; i++, pEvent++) {
        if (pEvent->m_nEvent == nEvent) {
            if (pfnCallback) {
                pEvent->m_pObject = pObject;
                pEvent->m_pfnCallback = pfnCallback;
                return;
            }
            m_lstEvents.DelIndex(i);
            return;
        }
    }
    if (pfnCallback) {
        CSWGuiControlEvent cEvent;
        cEvent.m_pObject = pObject;
        cEvent.m_pfnCallback = pfnCallback;
        cEvent.m_nEvent = nEvent;
        m_lstEvents.Add(cEvent);
    }
}

// FUNCTION: SWKOTOR 0x0041ab90
void CSWGuiControl::HandleLMouseDown()
{
    if (!IsSelectable()) {
        CSWGuiControl* pOwner = GetOwner();
        if (pOwner) {
            pOwner->HandleLMouseDown();
            return;
        }
    }
    g_pGuiManager->SetCapture(this, 1);
}

// FUNCTION: SWKOTOR 0x0041abd0
void CSWGuiControl::HandleLMouseUp()
{
    CSWGuiControl* pOver = g_pGuiManager->m_pMouseOverControl;
    if (pOver && (pOver == this || pOver->GetOwner() == this)) {
        if (g_pGuiManager->m_bLeftDown) {
            HandleInputEvent(0x1f9, 1);
        }
        if (m_bSelectable) {
            HandleInputEvent(0x27, 1);
        }
    }
    g_pGuiManager->ReleaseCapture(this);
    OnCaptureLost();
}

// FUNCTION: SWKOTOR 0x0041ac30
void CSWGuiControl::HandleRMouseDown()
{
    if (!IsSelectable()) {
        CSWGuiControl* pOwner = GetOwner();
        if (pOwner) {
            pOwner->HandleRMouseDown();
            return;
        }
    }
    g_pGuiManager->SetCapture(this, 4);
}

// FUNCTION: SWKOTOR 0x0041ac70
void CSWGuiControl::HandleRMouseUp()
{
    CSWGuiControl* pOver = g_pGuiManager->m_pMouseOverControl;
    if (pOver && (pOver == this || pOver->GetOwner() == this) && m_bSelectable) {
        HandleInputEvent(0x44, 1);
    }
    g_pGuiManager->ReleaseCapture(this);
    OnCaptureLost();
}

// FUNCTION: SWKOTOR 0x0041acc0
void CSWGuiControl::AddChildControl(CSWGuiControl* pControl)
{
    m_lstChildren.Add(pControl);
}

// FUNCTION: SWKOTOR 0x0041acd0
CSWGuiLabel::CSWGuiLabel()
{
    m_bNavigable = 0;
}

// FUNCTION: SWKOTOR 0x0041aed0
CSWGuiProgressBar::CSWGuiProgressBar()
{
    m_bStartFromLeft = 1;
    m_bNavigable = 0;
    m_nMaxValue = 0;
    m_nCurValue = 0;
}

// FUNCTION: SWKOTOR 0x0041af50 SYMBOL: ??_GCSWGuiProgressBar@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0041af70
CSWGuiProgressBar::~CSWGuiProgressBar()
{
}

// FUNCTION: SWKOTOR 0x0041afe0
CSWGuiScrollBar::CSWGuiScrollBar()
{
    m_nMaxValue = 0;
    m_nCurValue = 0;
    m_nVisibleValue = 0;
    m_bDrawMode = 0;
}

// FUNCTION: SWKOTOR 0x0041b5c0
CSWGuiLabelHilight::CSWGuiLabelHilight()
{
}

// FUNCTION: SWKOTOR 0x0041b610
CSWGuiLabelHilight::~CSWGuiLabelHilight()
{
}

// FUNCTION: SWKOTOR 0x0041b8b0
CSWGuiNavigable::CSWGuiNavigable()
{
    for (int i = 0; i < 4; i++) {
        m_aNavigation[i] = 0;
    }
    m_bNavigable = 1;
}

// FUNCTION: SWKOTOR 0x0041b8e0 SYMBOL: ?Load@CSWGuiControl@@UAEXPAVCResGFF@@PAUCResStruct@@@Z
void CSWGuiControl::Load(CResGFF* pGFF, CResStruct* pStruct)
{
    int bSuccess;
    m_extent.Load(pGFF, pStruct);
    SetExtent(m_extent);
    m_nId = pGFF->ReadFieldINT(pStruct, "ID", bSuccess, -1);
    int nParent = pGFF->ReadFieldINT(pStruct, "Obj_ParentID", bSuccess, -1);
    if (nParent != -1) {
        CSWGuiPanel* pPanel = m_pOwner->GetPanel();
        if (pPanel) {
            m_pParentControl = pPanel->GetControl(nParent);
            m_pParentControl->AddChildControl(this);
        }
    }
}

// FUNCTION: SWKOTOR 0x0041b960
void CSWGuiLabel::Load(CResGFF* pGFF, CResStruct* pStruct)
{
    {
        CExoString sText("TEXT");
        m_cText.Load(pGFF, pStruct, sText);
    }
    {
        CExoString sBorder("BORDER");
        m_cBorder.Load(pGFF, pStruct, sBorder);
    }
    CSWGuiControl::Load(pGFF, pStruct);
}

// FUNCTION: SWKOTOR 0x0041ba20
void CSWGuiLabelHilight::Load(CResGFF* pGFF, CResStruct* pStruct)
{
    {
        CExoString sHilight("HILIGHT");
        m_cHilight.Load(pGFF, pStruct, sHilight);
    }
    CSWGuiLabel::Load(pGFF, pStruct);
}

// FUNCTION: SWKOTOR 0x0041baa0
CSWGuiSlider::CSWGuiSlider()
{
    m_nMaxValue = 0;
    m_nValue = 0;
    m_nSound = 1;
}

// FUNCTION: SWKOTOR 0x0041bb20 SYMBOL: ??_GCSWGuiSlider@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0041bb40
CSWGuiSlider::~CSWGuiSlider()
{
}

// FUNCTION: SWKOTOR 0x0041bbc0
void CSWGuiProgressBar::Load(CResGFF* pGFF, CResStruct* pStruct)
{
    int bSuccess;
    {
        CExoString sBorder("BORDER");
        m_cBorder.Load(pGFF, pStruct, sBorder);
    }
    {
        CExoString sProgress("PROGRESS");
        m_cProgress.Load(pGFF, pStruct, sProgress);
    }
    SetMaxValue(pGFF->ReadFieldINT(pStruct, "MAXVALUE", bSuccess, 0));
    SetCurValue(pGFF->ReadFieldINT(pStruct, "CURVALUE", bSuccess, 0));
    SetStartFromLeft(pGFF->ReadFieldBYTE(pStruct, "STARTFROMLEFT", bSuccess, 1));
    CSWGuiControl::Load(pGFF, pStruct);
}

// FUNCTION: SWKOTOR 0x0041bcd0
void CSWGuiScrollBar::Load(CResGFF* pGFF, CResStruct* pStruct)
{
    int bSuccess;
    {
        CExoString sBorder("BORDER");
        m_cBorder.Load(pGFF, pStruct, sBorder);
    }
    {
        CExoString sDir("DIR");
        m_cDir.Load(pGFF, pStruct, sDir);
    }
    {
        CExoString sThumb("THUMB");
        m_cThumb.Load(pGFF, pStruct, sThumb);
    }
    SetMaxValue(pGFF->ReadFieldINT(pStruct, "MAXVALUE", bSuccess, 0), 0);
    SetCurValue(pGFF->ReadFieldINT(pStruct, "CURVALUE", bSuccess, 0));
    SetVisibleValue(pGFF->ReadFieldINT(pStruct, "VISIBLEVALUE", bSuccess, 0));
    m_bDrawMode = pGFF->ReadFieldBYTE(pStruct, "DRAWMODE", bSuccess, 0);
    CSWGuiControl::Load(pGFF, pStruct);
}

// FUNCTION: SWKOTOR 0x0041c0f0
CSWGuiButton::CSWGuiButton()
{
}

// FUNCTION: SWKOTOR 0x0041c6b0
CSWGuiEditbox::CSWGuiEditbox()
{
    m_bNavigable = 1;
}

// FUNCTION: SWKOTOR 0x0041c720 SYMBOL: ??_GCSWGuiEditbox@@UAEPAXI@Z


// FUNCTION: SWKOTOR 0x0041c7d0
void CSWGuiEditbox::Load(CResGFF* pGFF, CResStruct* pStruct)
{
    {
        CExoString sText("TEXT");
        m_cText.Load(pGFF, pStruct, sText);
    }
    {
        CExoString sBorder("BORDER");
        m_cBorder.Load(pGFF, pStruct, sBorder);
    }
    CSWGuiControl::Load(pGFF, pStruct);
}

// FUNCTION: SWKOTOR 0x0041c890
void CSWGuiNavigable::Load(CResGFF* pGFF, CResStruct* pStruct)
{
    CResStruct cMoveTo;
    if (pGFF->GetStructFromStruct(&cMoveTo, pStruct, "MOVETO")) {
        int bSuccess;
        m_aNavigation[1] = pGFF->ReadFieldINT(&cMoveTo, "LEFT", bSuccess, 0);
        m_aNavigation[3] = pGFF->ReadFieldINT(&cMoveTo, "RIGHT", bSuccess, 0);
        m_aNavigation[0] = pGFF->ReadFieldINT(&cMoveTo, "UP", bSuccess, 0);
        m_aNavigation[2] = pGFF->ReadFieldINT(&cMoveTo, "DOWN", bSuccess, 0);
    }
    CSWGuiControl::Load(pGFF, pStruct);
}

// FUNCTION: SWKOTOR 0x0041c930
void CSWGuiButton::Load(CResGFF* pGFF, CResStruct* pStruct)
{
    {
        CExoString sBorder("BORDER");
        m_cBorder.Load(pGFF, pStruct, sBorder);
    }
    {
        CExoString sHilight("HILIGHT");
        m_cHilight.Load(pGFF, pStruct, sHilight);
    }
    {
        CExoString sText("TEXT");
        m_cText.Load(pGFF, pStruct, sText);
    }
    CSWGuiNavigable::Load(pGFF, pStruct);
}

// FUNCTION: SWKOTOR 0x0041ca30
CSWGuiButtonToggle::CSWGuiButtonToggle()
{
    m_bSelected = 0;
    m_nToggleEvent = 0x27;
    m_nSound = 3;
}

// FUNCTION: SWKOTOR 0x0041cab0 SYMBOL: ??_GCSWGuiButtonToggle@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0041cad0
CSWGuiButtonToggle::~CSWGuiButtonToggle()
{
}

// FUNCTION: SWKOTOR 0x0041cb40
void CSWGuiButtonToggle::Load(CResGFF* pGFF, CResStruct* pStruct)
{
    int bSuccess;
    {
        CExoString sSelected("SELECTED");
        m_cSelected.Load(pGFF, pStruct, sSelected);
    }
    {
        CExoString sHilightselected("HILIGHTSELECTED");
        m_cHilightSelected.Load(pGFF, pStruct, sHilightselected);
    }
    m_bSelected = pGFF->ReadFieldBYTE(pStruct, "ISSELECTED", bSuccess, 0);
    CSWGuiButton::Load(pGFF, pStruct);
}

// ---------------------------------------------------------------------------
// CSWGuiListBox

// FUNCTION: SWKOTOR 0x004181f0
void CSWGuiListBox::UpdateScrollBar(int nWidth, int bSetExtent)
{
    if (m_extent.width) {
        CSWGuiExtent cExtent;
        if (nWidth > m_extent.width) {
            nWidth = m_extent.width;
        }
        cExtent.top = m_extent.top;
        cExtent.height = m_extent.height;
        cExtent.width = nWidth;
        if (m_bScrollBarOnLeft) {
            cExtent.left = m_extent.left;
        } else {
            cExtent.left = m_extent.left - nWidth + m_extent.width;
        }
        m_cScrollBar.SetExtent(cExtent);
        if (bSetExtent) {
            SetExtent(m_extent);
        }
    }
}

// FUNCTION: SWKOTOR 0x00418270
void CSWGuiListBox::SetScrollBarOnLeft(int bLeft)
{
    if (m_bScrollBarOnLeft != bLeft) {
        m_bScrollBarOnLeft = bLeft;
        UpdateScrollBar(m_cScrollBar.m_extent.width, 1);
    }
}

// FUNCTION: SWKOTOR 0x0041b060
CSWGuiListBox::~CSWGuiListBox()
{
    if (m_pProtoItem) {
        delete m_pProtoItem;
        m_pProtoItem = 0;
    }
    ClearControls();
}

// FUNCTION: SWKOTOR 0x0041b120
void CSWGuiListBox::SetListFlag0(int b)
{
    m_bListFlag0 = b;
}

// FUNCTION: SWKOTOR 0x0041be40
CSWGuiListBox::CSWGuiListBox()
    : m_lstControls(0), m_lstExtents(0)
{
    m_nSelected = -1;
    m_n2c1 = -1;
    m_n6c = -1;
    m_n2c0 = 0;
    m_pProtoItem = 0;
    m_nFlags = (m_nFlags & 0xfffff8ba) | 0x183a;
    m_nFirstVisible = 0;
    m_nLastVisible = 0;
    m_n88 = 0;
    m_n8c = 0;
    m_n7c = 0;
    m_pLastControl = 0;
    m_nPages = 1;
    m_fScale = 1.0f;
}

// FUNCTION: SWKOTOR 0x0041bf60 SYMBOL: ??_GCSWGuiListBox@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0041bf80
void CSWGuiListBox::SetExtent(const CSWGuiExtent& extent)
{
    int nWidth = m_cScrollBar.m_extent.width;
    m_extent = extent;
    CSWGuiExtent cExtent = extent;
    if (m_bScrollBarOnLeft) {
        cExtent.left += nWidth;
    }
    cExtent.width -= nWidth;
    m_cBorder.SetExtent(cExtent);
    m_cArea = m_cBorder.GetInnerExtent();
    UpdateScrollBar(nWidth, 0);
    m_bDirty = 1;
    OrganizeControls();
}

// FUNCTION: SWKOTOR 0x00419a90
void CSWGuiListBox::ClearControls()
{
    int i;
    if (m_bFlag1) {
        if (m_pLastControl && !m_pLastControl->v6c()) {
            m_pLastControl = 0;
        }
        for (i = 0; i < m_lstControls.num; i++) {
            if (m_lstControls[i]) {
                delete m_lstControls[i];
            }
            m_lstControls[i] = 0;
        }
    } else {
        for (i = 0; i < m_lstControls.num; i++) {
            if (m_bFlag2) {
                m_lstControls[i]->SetExtent(m_lstExtents[i]);
            }
            m_lstControls[i]->m_pOwner = 0;
        }
    }
    m_pLastControl = 0;
    m_lstControls.num = 0;
    m_lstExtents.num = 0;
    m_nSelected = -1;
    m_nPages = 1;
}

// FUNCTION: SWKOTOR 0x0041a290
int CSWGuiListBox::IsOversized()
{
    if (m_n2c0 + m_nItemSize > m_cArea.height && m_lstControls.num > 0 && !m_bUnequal) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0041a5a0
CSWGuiControl* CSWGuiListBox::GetControl(int nIndex)
{
    if (nIndex >= 0 && nIndex < m_lstControls.num) {
        return m_lstControls[nIndex];
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0041a5d0
CSWGuiControl* CSWGuiListBox::GetSelectedControl()
{
    return GetControl(m_nSelected);
}

// FUNCTION: SWKOTOR 0x0041c040
void CSWGuiListBox::SetSelectedControl(int nIndex, int bSound)
{
    if (m_nSelected != -1) {
        m_lstControls[m_nSelected]->HandleInputEvent(1, 1);
    }
    if (nIndex >= 0 && nIndex < m_lstControls.num) {
        m_nSelected = nIndex;
        m_lstControls[m_nSelected]->HandleInputEvent(0, 1);
        if (bSound) {
            ((CSWGuiPanel*)m_pOwner)->m_pManager->PlayGuiSound(m_lstControls[m_nSelected]->m_nClickSound);
        }
    } else {
        m_nSelected = -1;
    }
    m_bKeepSelectionVisible = 1;
    OrganizeControls();
    m_cScrollBar.SetCurValue(m_nFirstVisible);
}

// FUNCTION: SWKOTOR 0x0041c160
void CSWGuiListBox::SelectControl(CSWGuiControl* pControl, int bSound)
{
    SetSelectedControl(m_lstControls.IndexOf(pControl), bSound);
}

// FUNCTION: SWKOTOR 0x0041c190
void CSWGuiListBox::SetPadding(unsigned char nPadding)
{
    if (m_n2c0 <= m_cArea.width / 2 && m_n2c0 <= m_cArea.height / 2) {
        m_n2c0 = nPadding;
        m_bDirty = 1;
        OrganizeControls();
    }
}

// FUNCTION: SWKOTOR 0x004183b0
void CSWGuiEditbox::Initialize(const CSWGuiExtent& extent, CSWGuiTextParams* pText, CSWGuiBorderParams* pBorder)
{
    m_extent = extent;
    m_cBorder.Initialize(m_extent, pBorder);
    CSWGuiExtent cInner = m_cBorder.GetInnerExtent();
    m_cText.Initialize(cInner, pText, 1.0f);
}

// FUNCTION: SWKOTOR 0x00418420
void CSWGuiEditbox::Draw(float fDelta)
{
    m_cBorder.Draw(fDelta);
    m_cText.Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x00418450
void CSWGuiEditbox::SetExtent(const CSWGuiExtent& extent)
{
    m_extent = extent;
    m_cBorder.SetExtent(m_extent);
    CSWGuiExtent cInner = m_cBorder.GetInnerExtent();
    m_cText.SetExtent(cInner);
}

// FUNCTION: SWKOTOR 0x0041a7b0
void CSWGuiListBox::UpdateFont()
{
    for (int i = 0; i < m_lstControls.num; i++) {
        m_lstControls[i]->UpdateFont();
    }
    if (m_pProtoItem) {
        m_pProtoItem->UpdateFont();
    }
}

// FUNCTION: SWKOTOR 0x0041a800
int CSWGuiListBox::DisplayToolTip()
{
    CSWGuiControl* pControl = HitCheckMouseLocal(0);
    if (pControl) {
        return pControl->DisplayToolTip();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0041a820
void CSWGuiEditbox::HandleFocusChange(int bFocus)
{
    if (bFocus) {
        g_pExoInput->StartTextInput();
        g_pGuiManager->m_pFocusControl = this;
        m_cText.SetFocus(1);
    } else {
        g_pExoInput->StopTextInput();
        g_pGuiManager->m_pFocusControl = 0;
        m_cText.SetFocus(0);
    }
    CSWGuiControl::HandleFocusChange(bFocus);
}

// FUNCTION: SWKOTOR 0x0041a9d0
void CSWGuiNavigable::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        CSWGuiControl* pControl = this;
        while (pControl) {
            if (pControl->GetNavigation()) {
                int* pNavigation = pControl->GetNavigation()->m_aNavigation;
                switch (nEvent) {
                case 0x32:
                case 0x3e:
                    pControl = (CSWGuiControl*)pNavigation[2];
                    break;
                case 0x31:
                case 0x3d:
                    pControl = (CSWGuiControl*)pNavigation[0];
                    break;
                case 0x2f:
                case 0x3f:
                    pControl = (CSWGuiControl*)pNavigation[1];
                    break;
                case 0x30:
                case 0x40:
                    pControl = (CSWGuiControl*)pNavigation[3];
                    break;
                }
                if (!pControl) {
                    break;
                }
                if (!pControl->m_bSelectable && pControl != this) {
                    continue;
                }
            }
            if (m_pOwner && pControl != this) {
                m_pOwner->SetActiveControl(pControl, 1);
            }
            break;
        }
    }
    CSWGuiControl::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x0041ad40
void CSWGuiButton::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown && nEvent == 0x27 && m_pOwner) {
        CSWGuiManager* pManager;
        if (m_pOwner->GetPanel()) {
            pManager = m_pOwner->GetPanel()->m_pManager;
        } else {
            if (!m_pOwner->GetListBox()) {
                goto done;
            }
            pManager = m_pOwner->GetListBox()->m_pOwner->GetPanel()->m_pManager;
        }
        if (pManager) {
            pManager->PlayGuiSound(m_nSound);
        }
    }
done:
    CSWGuiNavigable::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x0041adb0
void CSWGuiButtonToggle::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown && nEvent == m_nToggleEvent) {
        m_bSelected = !m_bSelected;
    }
    CSWGuiButton::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x0041c1d0 SYMBOL: ?AddControls@CSWGuiListBox@@QAEXABV?$CExoArrayList@PAVCSWGuiControl@@@@HHH@Z
void CSWGuiListBox::AddControls(const CExoArrayList<CSWGuiControl*>& lstControls, int bOwned, int bRestoreExtents,
                                int bUnequal)
{
    int nId = 0;
    int nCount = lstControls.num;
    ClearControls();
    m_bFlag1 = bOwned;
    m_bFlag2 = bRestoreExtents;
    for (int i = 0; i < nCount; i++) {
        CSWGuiControl* pControl = lstControls.element[i];
        if (m_lstControls.IndexOf(pControl) == -1) {
            pControl->m_nId = nId;
            pControl->m_pOwner = this;
            nId++;
            m_lstControls.Add(pControl);
            m_lstExtents.Add(pControl->m_extent);
        }
    }
    m_bUnequal = bUnequal;
    m_bDirty = 1;
    OrganizeControls();
}

// FUNCTION: SWKOTOR 0x0041c2c0 SYMBOL: ?AddControls@CSWGuiListBox@@QAEXPAPAVCSWGuiControl@@HHHH@Z
void CSWGuiListBox::AddControls(CSWGuiControl** pControls, int nCount, int bOwned, int bRestoreExtents,
                                int bUnequal)
{
    int nId = 0;
    ClearControls();
    m_bFlag1 = bOwned;
    m_bFlag2 = bRestoreExtents;
    for (int i = 0; i < nCount; i++) {
        CSWGuiControl* pControl = pControls[i];
        if (m_lstControls.IndexOf(pControl) == -1) {
            pControl->m_nId = nId;
            pControl->m_pOwner = this;
            nId++;
            m_lstControls.Add(pControl);
            m_lstExtents.Add(pControl->m_extent);
        }
    }
    m_bUnequal = bUnequal;
    m_bDirty = 1;
    OrganizeControls();
}

// FUNCTION: SWKOTOR 0x0041a5e0
CSWGuiControl* CSWGuiListBox::HitCheckMouseLocal(int* pIndex)
{
    int x;
    int y;
    ((CSWGuiPanel*)m_pOwner)->GetLocalMouseCoords(&x, &y);
    if (IsPointInExtentCopy(m_cScrollBar.m_extent, x, y)) {
        if (pIndex) {
            *pIndex = -1;
        }
        return &m_cScrollBar;
    }
    int nX = x - m_cArea.left;
    int nY = y - m_cArea.top;
    m_nLocalMouseX = nX;
    m_nLocalMouseY = nY;
    if (m_lstControls.num > 0) {
        for (int i = 0; i < m_lstControls.num; i++) {
            CSWGuiControl* pControl = m_lstControls[i]->HitCheckMouse(nX, nY);
            if (pControl) {
                if (pIndex) {
                    *pIndex = i;
                }
                return pControl;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0041a6c0
int CSWGuiListBox::IsSelectable()
{
    int bResult = 0;
    if (!m_bSelectable) {
        return 0;
    }
    CSWGuiListBox* pList = GetListBox();
    if (pList) {
        CSWGuiControl* pControl = pList->HitCheckMouseLocal(0);
        if (pControl && pControl->IsSelectable()) {
            bResult = 1;
        }
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x0041a700
void CSWGuiListBox::HandleLMouseUp()
{
    int nIndex;
    m_n88 = 0;
    CSWGuiControl* pControl = HitCheckMouseLocal(&nIndex);
    if (pControl && m_bSelectable) {
        if (m_n6c != -1) {
            if (m_n6c == nIndex) {
                HandleInputEvent(0x27, 1);
            }
        } else if (m_bListFlag0 || (m_lstControls.num > 0 && m_lstControls[0]->v60())) {
            if (g_pGuiManager->m_bLeftDown && !pControl->v6c()) {
                HandleInputEvent(0x27, 1);
            }
        }
    }
    m_n6c = -1;
    g_pGuiManager->ReleaseCapture(this);
    m_n70 = 0;
}

// FUNCTION: SWKOTOR 0x0041c3a0
void CSWGuiListBox::SetActiveControl(CSWGuiControl* pControl, int bSound)
{
    SelectControl(pControl, bSound);
}

// FUNCTION: SWKOTOR 0x0041c3b0
void CSWGuiListBox::SetFlag0(int b)
{
    m_bFlag0 = b;
    if (m_bFlag0) {
        if (m_nSelected == -1 && !m_bFlag9) {
            SetSelectedControl(0, 0);
        } else {
            OrganizeControls();
        }
    }
}

// FUNCTION: SWKOTOR 0x004182b0
int CSWGuiListBox::GetPagesToScroll()
{
    int nArea = m_cArea.height;
    if (nArea) {
        return (int)(m_nItemSize * m_fScale / nArea + 0.9999f) - (int)m_fScale + 1;
    }
    return 1;
}

#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x0041c740
CSWGuiEditbox::~CSWGuiEditbox()
{
}

// Out of the auto_inline(off) region so new[] inlines the extent ctor loop.
// FUNCTION: SWKOTOR 0x0041a8f0 SYMBOL: ?Allocate@?$CExoArrayList@UCSWGuiExtent@@@@QAEXH@Z
GUI_ARRAY_LIST_ALLOCATE(CSWGuiExtent)
