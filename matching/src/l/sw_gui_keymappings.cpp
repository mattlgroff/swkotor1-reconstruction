// CSWGuiInGameOptKeyMappings (vtable 0x00759358, swguiinOPTKeyMapping.o) and
// the CSWGuiKeyMapButton methods that read keymap.2da / write swkotor.ini.
#include "l/sw_gui_keymappings.h"

// FUNCTION: SWKOTOR 0x006EC120
CExoString CSWGuiKeyMapButton::GetActionId(int nId, int nType)
{
    CExoString sAction;
    if (nType == 0) {
        CExoString sNumber(nId);
        sAction = CExoString("Action") + sNumber;
    } else if (nType == 1) {
        sAction.Format("%s%dB", "Action", nId);
    } else if (nType == 2) {
        sAction.Format("%s%dA", "Action", nId);
    }
    return sAction;
}

// FUNCTION: SWKOTOR 0x006EC250
void CSWGuiKeyMapButton::SetExtent(const CSWGuiExtent& extent)
{
    CSWGuiExtent cLocal = extent;
    cLocal.width -= 0xC5;
    m_extent = cLocal;
    GetBorder().SetExtent(cLocal);
    GetHilight().SetExtent(cLocal);
    m_cTextBox.SetExtent(IntersectExtents(GetBorder().GetInnerExtent(), GetHilight().GetInnerExtent()));
    CSWGuiExtent cEvent = extent;
    cEvent.left = cLocal.left + cLocal.width + 2;
    cEvent.width = 0xC3;
    m_cEventButton.SetExtent(cEvent);
}

// FUNCTION: SWKOTOR 0x006EC7D0
void CSWGuiKeyMapButton::SetDescription(const CExoString& sAction)
{
    int nActionStrRef;
    g_pRules->Get2DArrays()->GetKeymapTable()->GetINTEntry(sAction, "ActionStrRef", &nActionStrRef);
    g_pRules->Get2DArrays()->GetKeymapTable()->GetINTEntry(sAction, "DescStrRef", &m_nDescriptionStrRef);
    m_nToolTipStrRef = m_nDescriptionStrRef;
    m_cEventButton.m_nToolTipStrRef = m_nDescriptionStrRef;
    g_pRules->Get2DArrays()->GetKeymapTable()->GetINTEntry(sAction, "Page", &m_nPage);
    int nValue = 0;
    m_nClassFlags = 0;
    if (g_pRules->Get2DArrays()->GetKeymapTable()->GetINTEntry(sAction, "ICPC", &nValue) && nValue == 1) {
        m_nClassFlags |= 1;
    }
    if (g_pRules->Get2DArrays()->GetKeymapTable()->GetINTEntry(sAction, "ICMiniGame", &nValue) && nValue == 1) {
        m_nClassFlags |= 2;
    }
    if (g_pRules->Get2DArrays()->GetKeymapTable()->GetINTEntry(sAction, "ICPCGUI", &nValue) && nValue == 1) {
        m_nClassFlags |= 4;
    }
    if (g_pRules->Get2DArrays()->GetKeymapTable()->GetINTEntry(sAction, "ICDialog", &nValue) && nValue == 1) {
        m_nClassFlags |= 8;
    }
    if (g_pRules->Get2DArrays()->GetKeymapTable()->GetINTEntry(sAction, "ICFreeLook", &nValue) && nValue == 1) {
        m_nClassFlags |= 0x10;
    }
    if (g_pRules->Get2DArrays()->GetKeymapTable()->GetINTEntry(sAction, "ICMovie", &nValue) && nValue == 1) {
        m_nClassFlags |= 0x20;
    }
    CExoString sText = g_pTlkTable->GetSimpleString(nActionStrRef);
    m_cTextParams.SetText(sText);
    sText = g_pTlkTable->GetSimpleString(0xBC10);
    CExoInputEvent* pEvent = g_pExoInput->GetEvent(m_nId, -1);
    if (pEvent) {
        if (pEvent->HasAlternate() && m_nType > 1) {
            unsigned long nKey = pEvent->m_nAltKey;
            if (m_bChanged) {
                nKey = m_nNewKey;
            } else {
                m_nOriginalKey = nKey;
            }
            if (nKey) {
                sText = g_pAppManager->GetClientExoApp()->GetDisplayCharacterForKey(nKey);
            }
        } else {
            unsigned long nKey = pEvent->m_nKey;
            if (m_bChanged) {
                nKey = m_nNewKey;
            } else {
                m_nOriginalKey = nKey;
            }
            if (nKey) {
                sText = g_pAppManager->GetClientExoApp()->GetDisplayCharacterForKey(nKey);
            }
        }
    }
    m_cEventButton.m_cTextParams.SetText(sText);
}

// FUNCTION: SWKOTOR 0x006ECCC0
void CSWGuiKeyMapButton::ResetToDefault()
{
    int nKey;
    bool bFailed;
    {
        CExoString sColumn("Language0");
        bFailed = g_pRules->Get2DArrays()->m_pKeymapTable->GetINTEntry(GetActionId(m_nId, m_nType),
                                                                      sColumn, &nKey) == 0;
    }
    if (!bFailed) {
        m_bChanged = m_nOriginalKey != (unsigned long)nKey;
        m_nNewKey = nKey;
        SetDescription(GetActionId(m_nId, m_nType));
        if (m_bChanged) {
            m_cBorderParams.m_vColor = g_vKeyMapChangedColor;
            m_cEventButton.m_cBorderParams.m_vColor = g_vKeyMapChangedColor;
        } else {
            m_cBorderParams.m_vColor = g_vKeyMapNormalColor;
            m_cEventButton.m_cBorderParams.m_vColor = g_vKeyMapNormalColor;
        }
    }
}

// FUNCTION: SWKOTOR 0x006ECE60
void CSWGuiKeyMapButton::PostChangeToEvent()
{
    CExoInputEvent* pEvent = g_pExoInput->GetEvent(m_nId, -1);
    if (pEvent) {
        if (pEvent->HasAlternate() && m_nType > 1) {
            pEvent->m_nAltKey = m_nNewKey;
        } else {
            pEvent->m_nKey = m_nNewKey;
        }
    }
    CExoIni cIni;
    CExoString sValue;
    sValue.Format("%d", m_nNewKey);
    CExoString sSection("Keymapping");
    CExoString sFile(".\\swkotor.ini");
    cIni.WriteIniEntry(sValue, sFile, sSection, GetActionId(m_nId, m_nType));
}

// FUNCTION: SWKOTOR 0x006EDBD0
int CSWGuiKeyMapButton::UpdateEvent(unsigned long nKey)
{
    int nResult = m_pOwner->UpdateEvent(m_nId, m_nType, nKey, m_nClassFlags);
    if (nResult == 0) {
        m_nNewKey = nKey;
        m_bChanged = m_nOriginalKey != m_nNewKey;
        SetDescription(GetActionId(m_nId, m_nType));
        UpdateColor();
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x006EC3B0
void CSWGuiInGameOptKeyMappings::SetEditing(int bEditing)
{
    m_bEditing = bEditing;
    g_pExoInput->SetCaptureKeyboardKeys(bEditing);
}

// FUNCTION: SWKOTOR 0x006ECFB0 SYMBOL: ??_GCSWGuiInGameOptKeyMappings@@UAEPAXI@Z
// FUNCTION: SWKOTOR 0x006EC3D0
CSWGuiInGameOptKeyMappings::~CSWGuiInGameOptKeyMappings()
{
    m_lstEvents.ClearControls();
    while (m_lstButtons.num) {
        int i = m_lstButtons.num - 1;
        CSWGuiKeyMapButton* pButton = m_lstButtons[i];
        if (pButton) {
            delete pButton;
        }
        m_lstButtons[i] = 0;
        m_lstButtons.DelIndex(i);
    }
}

// FUNCTION: SWKOTOR 0x006EC580
void CSWGuiInGameOptKeyMappings::FixFilterColors(CSWGuiControl* pControl)
{
    switch (m_nFilter) {
    case 0:
        m_btnFilterMove.m_cTextParams.SetColor(g_vDefaultReplyColor);
        if (!pControl) {
            m_btnFilterGame.m_cTextParams.SetColor(g_vKeyMapNormalColor);
            m_btnFilterMini.m_cTextParams.SetColor(g_vKeyMapNormalColor);
        }
        break;
    case 1:
        m_btnFilterGame.m_cTextParams.SetColor(g_vDefaultReplyColor);
        if (!pControl) {
            m_btnFilterMove.m_cTextParams.SetColor(g_vKeyMapNormalColor);
            m_btnFilterMini.m_cTextParams.SetColor(g_vKeyMapNormalColor);
        }
        break;
    case 2:
        m_btnFilterMini.m_cTextParams.SetColor(g_vDefaultReplyColor);
        if (!pControl) {
            m_btnFilterMove.m_cTextParams.SetColor(g_vKeyMapNormalColor);
            m_btnFilterGame.m_cTextParams.SetColor(g_vKeyMapNormalColor);
        }
        break;
    }
}

// FUNCTION: SWKOTOR 0x006ECFD0
void CSWGuiInGameOptKeyMappings::FillListBox(int nFilter)
{
    CExoArrayListL<CSWGuiControl*> lstSorted;
    for (int i = 0; i < m_lstButtons.num; i++) {
        CSWGuiKeyMapButton* pButton = m_lstButtons[i];
        if (pButton->m_nPage != nFilter) {
            continue;
        }
        int nSortPos = pButton->m_nSortPosition;
        int nPos = -1;
        for (int j = 0; j < lstSorted.num; j++) {
            if (((CSWGuiKeyMapButton*)lstSorted[j])->m_nSortPosition > nSortPos) {
                nPos = j;
                break;
            }
            nPos = j + 1;
        }
        if (nPos == -1) {
            lstSorted.Add(pButton);
        } else {
            int nOld = lstSorted.num;
            lstSorted.Add(pButton);
            for (int k = nOld; k > nPos; k--) {
                lstSorted[k] = lstSorted[k - 1];
            }
            lstSorted[nPos] = pButton;
        }
    }
    CSWGuiListBox* pList = &m_lstEvents;
    pList->ClearControls();
    pList->AddControls(lstSorted, 0, 1, 0);
    pList->m_nListFlags &= ~0x40;
}

// FUNCTION: SWKOTOR 0x006ED1E0
int CSWGuiInGameOptKeyMappings::IsKeyUsedAlready(unsigned long nKey, int nId, int nType,
                                                 unsigned long nClassFlags)
{
    for (int i = 0; i < m_lstButtons.num; i++) {
        CSWGuiKeyMapButton* pButton = m_lstButtons[i];
        if ((pButton->m_nId != nId || pButton->m_nType != nType) && (pButton->m_nClassFlags & nClassFlags)) {
            unsigned long nBound = pButton->m_bChanged ? pButton->m_nNewKey : pButton->m_nOriginalKey;
            if (nBound == nKey) {
                pButton->m_bChanged = 1;
                pButton->m_nNewKey = 0;
                pButton->SetDescription(pButton->GetActionId(pButton->m_nId, pButton->m_nType));
                pButton->UpdateColor();
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006ED2D0
void CSWGuiInGameOptKeyMappings::OnDefaultClick(CSWGuiControl* pControl)
{
    for (int i = 0; i < m_lstButtons.num; i++) {
        if (!m_lstButtons[i]->m_bUnchangeable) {
            m_lstButtons[i]->ResetToDefault();
        }
    }
}

// FUNCTION: SWKOTOR 0x006ED310
void CSWGuiInGameOptKeyMappings::OnAcceptClick(CSWGuiControl* pControl)
{
    for (int i = 0; i < m_lstButtons.num; i++) {
        CSWGuiKeyMapButton* pButton = m_lstButtons[i];
        if (!pButton->m_bUnchangeable && pButton->m_bChanged) {
            pButton->PostChangeToEvent();
        }
    }
    m_bEditing = 0;
    g_pExoInput->SetCaptureKeyboardKeys(0);
    m_pManager->PlayGuiSound(0);
    m_pManager->PopModalPanel();
    m_nFlags = (m_nFlags & 0xFFFFFCFFul) | 0x400ul;
}

// FUNCTION: SWKOTOR 0x006ED390
void CSWGuiInGameOptKeyMappings::OnFilterMove(CSWGuiControl* pControl)
{
    m_nFilter = 0;
    m_btnFilterMove.m_cTextParams.SetColor(g_vDefaultReplyColor);
    m_btnFilterGame.m_cTextParams.SetColor(g_vKeyMapNormalColor);
    m_btnFilterMini.m_cTextParams.SetColor(g_vKeyMapNormalColor);
    FillListBox(m_nFilter);
}

// FUNCTION: SWKOTOR 0x006ED3E0
void CSWGuiInGameOptKeyMappings::OnFilterGame(CSWGuiControl* pControl)
{
    m_nFilter = 1;
    m_btnFilterGame.m_cTextParams.SetColor(g_vDefaultReplyColor);
    m_btnFilterMove.m_cTextParams.SetColor(g_vKeyMapNormalColor);
    m_btnFilterMini.m_cTextParams.SetColor(g_vKeyMapNormalColor);
    FillListBox(m_nFilter);
}

// FUNCTION: SWKOTOR 0x006ED430
void CSWGuiInGameOptKeyMappings::OnFilterMini(CSWGuiControl* pControl)
{
    m_nFilter = 2;
    m_btnFilterMini.m_cTextParams.SetColor(g_vDefaultReplyColor);
    m_btnFilterMove.m_cTextParams.SetColor(g_vKeyMapNormalColor);
    m_btnFilterGame.m_cTextParams.SetColor(g_vKeyMapNormalColor);
    FillListBox(m_nFilter);
}

// FUNCTION: SWKOTOR 0x006ED480
void CSWGuiInGameOptKeyMappings::SetCaptureEvent(CSWGuiKeyMapButton* pButton)
{
    CSWGuiKeyMapButton* pPrevious = m_pCaptureButton;
    if (pButton != pPrevious) {
        if (pPrevious) {
            pPrevious->UpdateColor();
            pPrevious->m_cEventButton.m_cBorderParams.SetFlags(0, 0, 0);
        }
        m_lstEvents.SetListFlag0(0);
        m_bEditing = 0;
        g_pExoInput->SetCaptureKeyboardKeys(0);
    }
    if (pButton) {
        m_pCaptureButton = pButton;
        pButton->m_cEventButton.m_cBorderParams.m_vColor = g_vDefaultReplyColor;
        pButton->m_cEventButton.m_cBorderParams.SetFlags(1, 1, 1);
        m_lstEvents.SetListFlag0(1);
        m_bEditing = 1;
        g_pExoInput->SetCaptureKeyboardKeys(1);
    }
}

// FUNCTION: SWKOTOR 0x006ED610
CSWGuiKeyMapButton* CSWGuiInGameOptKeyMappings::CreateOption(int nId, CSWGuiButton* pProto, int nType)
{
    CSWGuiZeroExtent extent;
    CSWGuiKeyMapButton* pButton = new CSWGuiKeyMapButton(this);
    pButton->m_nId = nId;
    pButton->m_nType = nType;
    pButton->CopyStyle(extent, pProto);
    pButton->m_cEventButton.CopyStyle(extent, pProto);
    pButton->m_cEventButton.m_cTextParams.SetText(CExoString(nId));
    CExoString sAction = pButton->GetActionId(nId, nType);
    int bForceDisplay = 0;
    int bDisabled = -1;
    int bReMappable;
    int nSortPos;
    g_pRules->Get2DArrays()->GetKeymapTable()->GetINTEntry(sAction, "ReMappable", &bReMappable);
    g_pRules->Get2DArrays()->GetKeymapTable()->GetINTEntry(sAction, "ForceDisplay", &bForceDisplay);
    g_pRules->Get2DArrays()->GetKeymapTable()->GetINTEntry(sAction, "Disabled", &bDisabled);
    g_pRules->Get2DArrays()->GetKeymapTable()->GetINTEntry(sAction, "SortPos", &nSortPos);
    pButton->m_nSortPosition = nSortPos;
    if ((!bReMappable && !bForceDisplay) || bDisabled == 1) {
        delete pButton;
        return 0;
    }
    pButton->SetDescription(sAction);
    int nCorner = (pButton->m_cHilightParams.GetCornerSize() > pButton->m_cBorderParams.GetCornerSize()
                       ? &pButton->m_cHilightParams : &pButton->m_cBorderParams)->GetCornerSize();
    extent.height = (pButton->m_cTextBox.GetHeight() + nCorner) * 2;
    pButton->SetExtent(extent);
    pButton->AddEvent(0x27, (CSWGuiPanel*)pButton, (PFN_GUI_CALLBACK)&CSWGuiKeyMapButton::OnEventClicked);
    pButton->AddEvent(0, (CSWGuiPanel*)pButton, (PFN_GUI_CALLBACK)&CSWGuiKeyMapButton::OnControlEntered);
    pButton->AddEvent(1, (CSWGuiPanel*)pButton, (PFN_GUI_CALLBACK)&CSWGuiKeyMapButton::OnNothing);
    if (bForceDisplay) {
        pButton->SetUnchangable();
    }
    return pButton;
}

// FUNCTION: SWKOTOR 0x006ED930
void CSWGuiInGameOptKeyMappings::FillListWithEvents()
{
    m_lstButtons.num = 0;
    CSWGuiControl* pProto = m_lstEvents.m_pProtoItem;
    for (int i = 0; i < 7; i++) {
        CSWGuiKeyMapButton* pButton = CreateOption(i + 0x118, (CSWGuiButton*)pProto, 2);
        if (pButton) {
            m_lstButtons.Add(pButton);
        }
        pButton = CreateOption(i + 0x118, (CSWGuiButton*)pProto, 1);
        if (pButton) {
            m_lstButtons.Add(pButton);
        }
    }
    for (int i = 0; i < 0x42; i++) {
        CSWGuiKeyMapButton* pButton = CreateOption(i + 0xC8, (CSWGuiButton*)pProto, 0);
        if (pButton) {
            m_lstButtons.AddInline(pButton);
        }
    }
}

// FUNCTION: SWKOTOR 0x006EDA80
int CSWGuiInGameOptKeyMappings::UpdateEvent(int nId, int nType, unsigned long nKey, unsigned long nClassFlags)
{
    CExoString sRow;
    sRow.Format("KEY%d", nKey - g_nKeyMapKeyBase);
    int bBindable = 0;
    if (g_pRules->Get2DArrays()->GetBindableKeysTable()->GetINTEntry(sRow, "Bindable", &bBindable) &&
        bBindable != 0) {
        if (IsKeyUsedAlready(nKey, nId, nType, nClassFlags)) {
            return 1;
        }
        return 0;
    }
    return 2;
}

// FUNCTION: SWKOTOR 0x006EDC90
CSWGuiInGameOptKeyMappings::CSWGuiInGameOptKeyMappings(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    m_bEditing = 0;
    g_pExoInput->SetCaptureKeyboardKeys(0);
    m_nFilter = 0;
    m_pCaptureButton = 0;
    StartLoadFromLayout("OPTKeyMapping");
    InitControl(&m_lblTitle, "LBL_Title", 1);
    InitControl(&m_btnDefault, "BTN_Default", 1);
    InitControl(&m_btnAccept, "BTN_Accept", 1);
    InitControl(&m_btnCancel, "BTN_Cancel", 1);
    InitControl(&m_btnFilterMove, "BTN_Filter_Move", 1);
    InitControl(&m_btnFilterGame, "BTN_Filter_Game", 1);
    InitControl(&m_btnFilterMini, "BTN_Filter_Mini", 1);
    InitControl(&m_lstEvents, "LST_EventList", 1);
    UnloadResource();
    m_btnFilterMove.m_cTextParams.SetColor(g_vDefaultReplyColor);
    FillListWithEvents();
    FillListBox(m_nFilter);
    m_btnDefault.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiInGameOptKeyMappings::OnDefaultClick);
    m_btnAccept.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiInGameOptKeyMappings::OnAcceptClick);
    m_btnCancel.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    CSWGuiButton* pMove = &m_btnFilterMove;
    pMove->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiInGameOptKeyMappings::OnFilterMove);
    pMove->AddEvent(1, this, (PFN_GUI_CALLBACK)&CSWGuiInGameOptKeyMappings::FixFilterColors);
    pMove->m_bNavigable = 0;
    CSWGuiButton* pGame = &m_btnFilterGame;
    pGame->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiInGameOptKeyMappings::OnFilterGame);
    pGame->AddEvent(1, this, (PFN_GUI_CALLBACK)&CSWGuiInGameOptKeyMappings::FixFilterColors);
    pGame->m_bNavigable = 0;
    CSWGuiButton* pMini = &m_btnFilterMini;
    pMini->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiInGameOptKeyMappings::OnFilterMini);
    pMini->AddEvent(1, this, (PFN_GUI_CALLBACK)&CSWGuiInGameOptKeyMappings::FixFilterColors);
    pMini->m_bNavigable = 0;
    SetActiveControl(&m_lstEvents, 0);
}

// FUNCTION: SWKOTOR 0x006EE040
void CSWGuiInGameOptKeyMappings::Update(float fDelta)
{
    if (m_bEditing != 1) {
        return;
    }
    unsigned long nKey = g_pExoInput->GetLastCapturedKeyboardKey();
    if (!nKey || !m_pCaptureButton) {
        return;
    }
    if (nKey == g_nKeyMapEscapeKey) {
        SetCaptureEvent(0);
        m_pManager->PlayGuiSound(0);
        return;
    }
    switch (m_pCaptureButton->UpdateEvent(nKey)) {
    case 0:
        SetEditing(0);
        m_pManager->PlayGuiSound(0);
        SetCaptureEvent(0);
        break;
    case 1:
        SetEditing(1);
        break;
    case 2:
        SetEditing(1);
        break;
    }
}

// FUNCTION: SWKOTOR 0x006EC510
void CSWGuiInGameOptKeyMappings::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown && (nEvent == 0x28 || nEvent == 0x2E)) {
        m_bEditing = 0;
        g_pExoInput->SetCaptureKeyboardKeys(0);
        m_pManager->PlayGuiSound(0);
        m_pManager->PopModalPanel();
        m_nFlags = (m_nFlags & 0xFFFFFCFFul) | 0x400ul;
        return;
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006ED5C0
void CSWGuiKeyMapButton::OnControlEntered(CSWGuiControl* pControl)
{
    m_pOwner->SetCaptureEvent(0);
}

// FUNCTION: SWKOTOR 0x006ED5E0
void CSWGuiKeyMapButton::OnEventClicked(CSWGuiControl* pControl)
{
    if (!m_bUnchangeable && pControl->m_n4c) {
        m_pOwner->SetCaptureEvent(this);
    }
}
