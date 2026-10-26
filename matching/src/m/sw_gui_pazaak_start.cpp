// Pazaak side-deck selection panel CSWGuiPazaakStart (vtable 0x007532E8,
// 0x0067D3B0-0x0067FED0); its constructor and input handlers follow at
// 0x006807E0-0x00681A90.

#include "m/sw_gui_pazaak.h"

// FUNCTION: SWKOTOR 0x0067D3B0
void CSWGuiPazaakStart::HandleWagerExit(int nWager)
{
    m_pPazaak->m_nWager = nWager;
    SetActiveControl(&m_aAvail[0], 0);
}

// FUNCTION: SWKOTOR 0x0067D3D0
void CSWGuiPazaakStart::HandleQuitDialog(CSWGuiControl* pControl)
{
    if (!g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox->m_bAccepted) {
        return;
    }
    if (m_pManager->PanelExists(m_pWagerPopup)) {
        m_pManager->RemovePanel(m_pWagerPopup);
    }
    MarkForDelete();
    g_pAppManager->m_pClientExoApp->EndPazaakGame(0, 0, 0, m_pPazaak->m_oidOpponent);
}

// FUNCTION: SWKOTOR 0x0067D450
int CSWGuiPazaakStart::IsOtherCardActiveControl()
{
    CSWGuiControl* pActive = m_pActiveControl;
    int nId = pActive->m_nId;
    if (nId < 0 || nId >= 18) {
        return 0;
    }
    return pActive == &m_aAvail[nId];
}

// FUNCTION: SWKOTOR 0x0067D480
int CSWGuiPazaakStart::IsSidedeckCardActiveControl()
{
    CSWGuiControl* pActive = m_pActiveControl;
    int nId = pActive->m_nId;
    if (nId < 0 || nId >= 10) {
        return 0;
    }
    return pActive == &m_aChosen[nId];
}

// FUNCTION: SWKOTOR 0x0067E2B0
CSWGuiPazaakStart::~CSWGuiPazaakStart()
{
    if (m_pWagerPopup) {
        delete m_pWagerPopup;
        m_pWagerPopup = 0;
    }
    if (m_pPazaak) {
        delete m_pPazaak;
        m_pPazaak = 0;
    }
}

// FUNCTION: SWKOTOR 0x0067E400
void CSWGuiPazaakStart::ShowDisabledButtons()
{
    m_btnAccept.SetEnabled(m_bAllChosen);
    CSWGuiControl* pActive = m_pActiveControl;
    if (pActive) {
        int nId = pActive->m_nId;
        if (IsAvailCard(pActive, nId)) {
            if (&m_aAvail[nId] == pActive && m_aAvailCount[nId] < 1) {
                m_btnSwitch.SetEnabled(0);
            }
            else {
                m_btnSwitch.SetEnabled(!m_bAllChosen);
            }
        }
        else if (IsChosenCard(pActive, nId)) {
            int bEnable = 1;
            if (&m_aChosen[nId] == pActive && m_aChosenCards[nId].m_nValue < 0) {
                bEnable = 0;
            }
            m_btnSwitch.SetEnabled(bEnable);
        }
        else {
            m_btnSwitch.SetEnabled(0);
        }
    }
    else {
        m_btnSwitch.SetEnabled(0);
    }
}

// FUNCTION: SWKOTOR 0x0067E530
void CSWGuiPazaakStart::OnChosenCardEntered(CSWGuiControl* pControl)
{
    ShowDisabledButtons();
}

// FUNCTION: SWKOTOR 0x0067F730 SYMBOL: ??_GCSWGuiPazaakStart@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0067F750
void CSWGuiPazaakStart::OnPanelAdded()
{
    if (m_pWagerPopup) {
        SetActiveControl(0, 0);
        m_pManager->AddPanel(m_pWagerPopup, 1, 1);
    }
    CSWPartyTable* pTable = g_pAppManager->m_pServerExoApp->GetPartyTable();
    int i;
    for (i = 0; i < 18; i++) {
        m_aAvailCount[i] = pTable->m_aPazaakCards[i];
        if (m_aAvailCount[i] == 0) {
            m_aAvailCount[i] = -1;
            m_aAvail[i].SetCard(CPazaakCard(-1, 0));
        }
    }
    m_bAllChosen = 1;
    for (i = 0; i < 10; i++) {
        m_aChosenCards[i] = CPazaakCard(pTable->m_aPazaakSideList[i], 0);
        if (m_aChosenCards[i].m_nValue >= 0 && m_aAvailCount[m_aChosenCards[i].m_nValue] > 0) {
            m_aAvailCount[m_aChosenCards[i].m_nValue]--;
        }
        else {
            m_aChosenCards[i] = CPazaakCard(-1, 0);
        }
        m_aChosen[i].SetCard(m_aChosenCards[i]);
        if (m_aChosenCards[i].m_nValue < 0) {
            m_bAllChosen = 0;
        }
    }
    for (i = 0; i < 18; i++) {
        if (m_aAvailCount[i] >= 0) {
            m_aAvailNum[i].m_cText.m_cParams.SetText(CExoString(m_aAvailCount[i]));
        }
        if (m_aAvailCount[i] == 0) {
            m_aAvail[i].m_cBorder.m_cParams.m_vColor = Vector(0.667f, 0.667f, 0.667f);
            m_aAvail[i].m_bDraggable = 0;
        }
    }
    for (i = 0; i < 10; i++) {
        m_aChosen[i].SetNavigation(1, &m_aChosen[(i + 9) % 10]);
        m_aChosen[i].SetNavigation(3, &m_aChosen[(i + 1) % 10]);
        m_aChosen[i].SetNavigation(0, &m_aChosen[(i + 8) % 10]);
        m_aChosen[i].SetNavigation(2, &m_aChosen[(i + 2) % 10]);
    }
    for (i = 0; i < 18; i++) {
        m_aAvail[i].SetNavigation(1, &m_aAvail[(i + 12) % 18]);
        m_aAvail[i].SetNavigation(3, &m_aAvail[(i + 6) % 18]);
        m_aAvail[i].SetNavigation(0, &m_aAvail[(i + 5) % 6 + i / 6 * 6]);
        m_aAvail[i].SetNavigation(2, &m_aAvail[(i + 1) % 6 + i / 6 * 6]);
    }
    if (!m_pWagerPopup) {
        SetActiveControl(&m_aAvail[0], 0);
    }
    ShowDisabledButtons();
    unsigned int aHelp[3];
    aHelp[0] = 0x96e3;
    aHelp[1] = 0x96e4;
    aHelp[2] = 0x96e5;
    m_cTutorial.ShowMultipleHelp(aHelp, 3);
    CSWGuiPanel::OnPanelAdded();
}

// FUNCTION: SWKOTOR 0x0067FB10
int CSWGuiPazaakStart::AddChosenCard(int nKind, int nSlot)
{
    if (m_aAvailCount[nKind] < 1) {
        return 0;
    }
    if (nSlot < 0) {
        for (nSlot = 0; nSlot < 10; nSlot++) {
            if (m_aChosenCards[nSlot].m_nValue < 0) {
                break;
            }
        }
    }
    if (nSlot >= 10) {
        return 0;
    }
    CPazaakCard cCard;
    cCard.m_nValue = nKind;
    m_aChosenCards[nSlot] = cCard;
    int nLeft = --m_aAvailCount[nKind];
    {
        CExoString sCount(nLeft);
        m_aAvailNum[nKind].m_cText.m_cParams.SetText(sCount);
    }
    CResRef cFill = m_aAvail[nKind].m_cBorder.m_cParams.m_cFill;
    m_aChosen[nSlot].m_cBorder.m_cParams.SetFillImage(cFill, 0);
    if (m_aAvailCount[nKind] == 0) {
        m_aAvail[nKind].m_cBorder.m_cParams.m_vColor = Vector(0.667f, 0.667f, 0.667f);
        m_aAvail[nKind].m_bDraggable = 0;
    }
    CExoString sText(m_aAvail[nKind].m_lblFace.m_cText.m_cParams.m_sText);
    m_aChosen[nSlot].m_lblFace.m_cText.m_cParams.SetText(sText);
    m_bAllChosen = 1;
    for (int i = 0; i < 10; i++) {
        if (m_aChosenCards[i].m_nValue < 0) {
            m_bAllChosen = 0;
            break;
        }
    }
    ShowDisabledButtons();
    return 1;
}

// FUNCTION: SWKOTOR 0x0067FD10
int CSWGuiPazaakStart::RemoveChosenCard(int nSlot)
{
    int nKind = m_aChosenCards[nSlot].m_nValue;
    if (nKind < 0) {
        return 0;
    }
    m_bAllChosen = 0;
    ShowDisabledButtons();
    CPazaakCard cEmpty;
    cEmpty.m_nValue = -1;
    cEmpty.m_bFlag = 0;
    m_aChosenCards[nSlot] = cEmpty;
    m_aAvailCount[nKind]++;
    {
        CExoString sCount(m_aAvailCount[nKind]);
        m_aAvailNum[nKind].m_cText.m_cParams.SetText(sCount);
    }
    m_aAvail[nKind].m_cBorder.m_cParams.m_vColor = Vector(1.0f, 1.0f, 1.0f);
    m_aAvail[nKind].m_bDraggable = 1;
    {
        CResRef cEmptyFill("");
        m_aChosen[nSlot].m_cBorder.m_cParams.SetFillImage(cEmptyFill, 0);
    }
    {
        CExoString sText("");
        m_aChosen[nSlot].m_lblFace.m_cText.m_cParams.SetText(sText);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0067FE80
void CSWGuiPazaakStart::HandleAvailCardDrop(CSWGuiControl* pControl)
{
    int nKind = pControl->m_nId;
    CSWGuiControl* pTarget = g_pGuiManager->m_pMouseOverControl;
    if (!pTarget) {
        return;
    }
    int nId = pTarget->m_nId;
    if (nId >= 0 && nId < 10 && pTarget == &m_aChosen[nId]) {
        RemoveChosenCard(nId);
        AddChosenCard(nKind, nId);
    }
}

// FUNCTION: SWKOTOR 0x0067FED0
void CSWGuiPazaakStart::HandleChosenCardDrop(CSWGuiControl* pControl)
{
    int nFrom = pControl->m_nId;
    int nMoved = m_aChosenCards[nFrom].m_nValue;
    CSWGuiControl* pTarget = g_pGuiManager->m_pMouseOverControl;
    if (!pTarget) {
        return;
    }
    int nId = pTarget->m_nId;
    if (nId < 0) {
        return;
    }
    int nMode;
    if (nId < 10 && pTarget == &m_aChosen[nId]) {
        nMode = 2;
    }
    else if (nId < 18 && pTarget == &m_aAvail[nId]) {
        nMode = 1;
    }
    else {
        return;
    }
    RemoveChosenCard(nFrom);
    if (nMode != 2) {
        return;
    }
    int nOther = m_aChosenCards[nId].m_nValue;
    RemoveChosenCard(nId);
    if (nOther >= 0) {
        AddChosenCard(nOther, nFrom);
    }
    if (nMoved >= 0) {
        AddChosenCard(nMoved, nId);
    }
}
