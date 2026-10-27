// The in-game quest journal panel CSWGuiInGameJournal (vtable 0x00751960).
// Handler names follow the playable reference (src/units/1407 panels_menus,
// src/units/1746 claude_gui_journal) and the Mac debug map where it agrees.

#include "m/sw_gui_in_game_journal.h"

// FUNCTION: SWKOTOR 0x00644850
CSWGuiInGameJournal::~CSWGuiInGameJournal()
{
    m_lbDescription.ClearControls();
    m_lbItems.ClearControls();
    if (m_pQuestItem) {
        delete m_pQuestItem;
    }
}

// FUNCTION: SWKOTOR 0x00644A40
CSWGuiInGameJournal::CSWGuiInGameJournal(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    m_pFB8 = 0;
    m_pQuestItem = new CSWGuiQuestItem(m_pManager);
    {
        CResRef cLayout("journal");
        StartLoadFromLayout(cLayout);
    }
    {
        CExoString sName("LBL_TITLE");
        InitControl(&m_lblTitle, sName, 1);
    }
    {
        CExoString sName("LB_ITEMS");
        InitControl(&m_lbItems, sName, 1);
    }
    {
        CExoString sName("LBL_ITEM_DESCRIPTION");
        InitControl(&m_lbDescription, sName, 1);
    }
    {
        CExoString sName("BTN_QUESTITEMS");
        InitControl(&m_btnQuestItems, sName, 1);
    }
    {
        CExoString sName("BTN_SWAPTEXT");
        InitControl(&m_btnSwapText, sName, 1);
    }
    {
        CExoString sName("BTN_SORT");
        InitControl(&m_btnSort, sName, 1);
    }
    {
        CExoString sName("BTN_EXIT");
        InitControl(&m_btnExit, sName, 1);
    }
    UnloadResource();

    CSWGuiControl* pProto = m_lbDescription.m_pProtoItem;
    m_lblDescription.CopyStyle(pProto->m_extent, pProto, 1.0f);
    CSWGuiButton* pItemProto = (CSWGuiButton*)m_lbItems.m_pProtoItem;
    pItemProto->m_cBorder.m_cParams.m_vColor = g_vDifficultyExitColor;
    pItemProto->m_cHilight.m_cParams.m_vColor = g_vAbilityColorGranted;
    pItemProto->m_cHilight.m_cParams.SetFlags(1, 1, 0);
    CSWGuiListBox* pItems = &m_lbItems;
    pItems->m_nListFlags |= 0x20;

    CSWGuiButton* pButton = &m_btnQuestItems;
    pButton->m_bSelectable = 0;
    pButton->AddEvent(0x27, this, &CSWGuiPanel::OnXButtonPressed);
    pButton = &m_btnSwapText;
    pButton->m_bSelectable = 0;
    pButton->AddEvent(0x27, this, &CSWGuiPanel::OnYButtonPressed);
    pButton = &m_btnSort;
    pButton->m_bSelectable = 0;
    pButton->AddEvent(0x27, this, &CSWGuiPanel::OnBlackButtonPressed);
    pButton = &m_btnExit;
    pButton->m_bSelectable = 0;
    pButton->AddEvent(0x27, this, &CSWGuiPanel::OnBButtonPressed);
    pItems->SetHilightable(1);
    CSWGuiPanel::SetActiveControl(pItems, 0);

    int nNext = g_nJournalSortOrder + 1;
    if (nNext >= 4) {
        nNext = 0;
    }
    CExoString sText = g_pTlkTable->GetSimpleString(0xa646) + " " +
                       g_pTlkTable->GetSimpleString(g_anJournalSortStrRefs[nNext]);
    m_btnSort.m_cText.m_cParams.SetText(sText);
    if (g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bJournalDone) {
        m_btnSwapText.m_cText.m_cParams.SetStrRef(0x7db2);
        sText = g_pTlkTable->GetSimpleString(0x7db1) + " - " +
                g_pTlkTable->GetSimpleString(g_anJournalSortStrRefs[g_nJournalSortOrder]);
    }
    else {
        m_btnSwapText.m_cText.m_cParams.SetStrRef(0x7db1);
        sText = g_pTlkTable->GetSimpleString(0x7db2) + " - " +
                g_pTlkTable->GetSimpleString(g_anJournalSortStrRefs[g_nJournalSortOrder]);
    }
    m_lblTitle.m_cText.m_cParams.SetText(sText);
    m_lblDescription.m_cText.m_cParams.SetText("");
}

// FUNCTION: SWKOTOR 0x00644720 SYMBOL: ??_9@$BFM@AE
// vcall thunk for &CSWGuiPanel::OnYButtonPressed (+0x5c), emitted by the ctor.
// FUNCTION: SWKOTOR 0x00644730 SYMBOL: ??_9@$BGA@AE
// vcall thunk for &CSWGuiPanel::OnBlackButtonPressed (+0x60), emitted by the ctor.

// FUNCTION: SWKOTOR 0x00645100
void CSWGuiInGameJournal::OnControlEntered(CSWGuiControl* pControl)
{
    if (pControl) {
        int nIndex = ((CSWGuiJournalItemEntry*)pControl)->m_nIndex;
        m_lbDescription.ClearControls();
        SJournalEntry* pEntry = GetQuestEntryAt(nIndex);
        if (pEntry) {
            m_lstViewed.AddUnique(nIndex);
            CExoString sText;
            unsigned long nLanguage = g_pAppManager->m_pClientExoApp->GetClientLanguage();
            pEntry->m_sText.GetString(nLanguage, &sText, 0);
            if (pEntry->m_nPlanet != -1) {
                int nName;
                g_pRules->Get2DArrays()->GetPlanets()->GetINTEntry(pEntry->m_nPlanet, "Name", &nName);
                sText = g_pTlkTable->GetSimpleString(nName) + ":\n" + sText;
            }
            m_lblDescription.m_cText.m_cParams.SetText(sText);
        }
    }
    CSWGuiExtent extent = m_lblDescription.m_extent;
    if (m_lblDescription.m_cText.m_cParams.m_sText.GetLength() > 0) {
        extent.height = m_lblDescription.m_cText.GetHeight();
    }
    else {
        extent.height = m_lblDescription.m_cText.GetFontHeight();
    }
    CSWGuiLabel* pLabel = &m_lblDescription;
    pLabel->SetExtent(extent);
    CSWGuiControl* apControls[1];
    apControls[0] = pLabel;
    CSWGuiListBox* pList = &m_lbDescription;
    pList->AddControls(apControls, 1, 0, 0, 0);
    pList->SelectControl(pLabel, 0);
}

typedef void (CSWGuiPanel::*PFN_GUI_CALLBACK)(CSWGuiControl*);

// FUNCTION: SWKOTOR 0x00645330
void CSWGuiInGameJournal::PopulateItemListBox()
{
    CSWGuiControl* pProto;
    unsigned long nLanguage;
    CSWCJournal* pJournal = g_pAppManager->m_pClientExoApp->GetQuestJournal();
    CExoString sText;
    nLanguage = g_pAppManager->m_pClientExoApp->GetClientLanguage();
    CExoArrayList<CSWGuiControl*> lstItems;
    pProto = m_lbItems.m_pProtoItem;
    CSWGuiExtent extent = pProto->m_extent;
    extent.width = m_lbItems.m_cItemExtent.width - m_lbItems.m_nBorder * 2;
    int nIndex = 0;
    SJournalEntry* pEntry = GetQuestEntryAt(nIndex);
    while (pEntry) {
        CSWGuiJournalItemEntry* pItem = new CSWGuiJournalItemEntry;
        if (pItem) {
            pItem->m_nIndex = nIndex;
            pItem->CopyStyle(extent, pProto);
            pItem->AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiInGameJournal::OnControlEntered);
            pEntry->m_sName.GetString(nLanguage, &sText, 0);
            pItem->m_cText.m_cParams.SetText(sText);
            if (pEntry->m_bUpdated) {
                pItem->SetEncouraged(1);
            }
            lstItems.AddInline(pItem);
        }
        nIndex++;
        pEntry = GetQuestEntryAt(nIndex);
    }
    m_lbItems.AddControls(lstItems, 1, 1, 0);
    if (m_pFB8) {
        m_pFB8->SetActive(0);
    }
    if (lstItems.num > 0) {
        m_lbItems.SetSelectedControl(0, 0);
    }
    else {
        m_lbItems.SetSelectedControl(-1, 0);
        m_lblDescription.m_cText.m_cParams.SetText("");
        OnControlEntered(0);
    }
    (pJournal->*(g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bJournalDone
                     ? &CSWCJournal::ClearDoneListChanged
                     : &CSWCJournal::ClearActiveListChanged))();
}

// FUNCTION: SWKOTOR 0x006456E0
void CSWGuiInGameJournal::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        CSWCJournal* pJournal = g_pAppManager->m_pClientExoApp->GetQuestJournal();
        switch (nEvent) {
        case 0x2b: {
            g_nJournalSortOrder = (pJournal->*(g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bJournalDone
                                                   ? &CSWCJournal::GetDoneSortBy
                                                   : &CSWCJournal::GetActiveSortBy))() + 1;
            if (g_nJournalSortOrder >= 4) {
                g_nJournalSortOrder = 0;
            }
            (pJournal->*(g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bJournalDone
                             ? &CSWCJournal::SetDoneSortBy
                             : &CSWCJournal::SetActiveSortBy))(g_nJournalSortOrder);
            int nNext = g_nJournalSortOrder + 1;
            if (nNext >= 4) {
                nNext = 0;
            }
            CExoString sText = g_pTlkTable->GetSimpleString(0xa646) + " " +
                               g_pTlkTable->GetSimpleString(g_anJournalSortStrRefs[nNext]);
            m_btnSort.m_cText.m_cParams.SetText(sText);
            sText = g_pTlkTable->GetSimpleString(
                        g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bJournalDone ? 0x7db1 : 0x7db2) +
                    " - " + g_pTlkTable->GetSimpleString(g_anJournalSortStrRefs[g_nJournalSortOrder]);
            m_lblTitle.m_cText.m_cParams.SetText(sText);
            m_pManager->PlayGuiSound(1);
            break;
        }
        case 0x2a: {
            MarkNewQuestsObserved();
            g_nJournalSortOrder = (pJournal->*(g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bJournalDone
                                                   ? &CSWCJournal::GetDoneSortBy
                                                   : &CSWCJournal::GetActiveSortBy))();
            g_pAppManager->m_pClientExoApp->GetInGameGui()->SetJournalOption(
                !g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bJournalDone);
            PopulateItemListBox();
            CExoString sText;
            if (g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bJournalDone) {
                m_btnSwapText.m_cText.m_cParams.SetStrRef(0x7db2);
                sText = g_pTlkTable->GetSimpleString(0x7db1) + " - " +
                        g_pTlkTable->GetSimpleString(g_anJournalSortStrRefs[g_nJournalSortOrder]);
            }
            else {
                m_btnSwapText.m_cText.m_cParams.SetStrRef(0x7db1);
                sText = g_pTlkTable->GetSimpleString(0x7db2) + " - " +
                        g_pTlkTable->GetSimpleString(g_anJournalSortStrRefs[g_nJournalSortOrder]);
            }
            m_lblTitle.m_cText.m_cParams.SetText(sText);
            m_pManager->PlayGuiSound(0);
            break;
        }
        case 0x29:
            m_pManager->PlayGuiSound(0);
            m_pManager->AddPanel(m_pQuestItem, 3, 1);
            break;
        case 0x28:
        case 0x2d:
        case 0x2e:
        case 0xdf:
            if (g_pAppManager->m_pClientExoApp->GetInGameGui()->HideSWInGameGui(0)) {
                g_pAppManager->m_pClientExoApp->SetInputClass(0, 1);
            }
            break;
        case 0x39:
            m_lbDescription.HandleInputEvent(0x31, 1);
            break;
        case 0x3a:
            m_lbDescription.HandleInputEvent(0x32, 1);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x00645E00
void CSWGuiInGameJournal::OnPanelAdded()
{
    CSWCJournal* pJournal = g_pAppManager->m_pClientExoApp->GetQuestJournal();
    int nSort = g_nJournalSortOrder;
    if (g_bJournalUpdated) {
        nSort = 0;
        g_bJournalUpdated = 0;
    }
    (pJournal->*(g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bJournalDone
                     ? &CSWCJournal::SetDoneSortBy
                     : &CSWCJournal::SetActiveSortBy))(nSort);
    int nNext = nSort + 1;
    if (nNext >= 4) {
        nNext = 0;
    }
    CExoString sText = g_pTlkTable->GetSimpleString(0xa646) + " " +
                       g_pTlkTable->GetSimpleString(g_anJournalSortStrRefs[nNext]);
    m_btnSort.m_cText.m_cParams.SetText(sText);
    sText = g_pTlkTable->GetSimpleString(
                g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bJournalDone ? 0x7db1 : 0x7db2) +
            " - " + g_pTlkTable->GetSimpleString(g_anJournalSortStrRefs[nSort]);
    m_lblTitle.m_cText.m_cParams.SetText(sText);
    PopulateItemListBox();
    m_btnExit.HandleMouseOver(0);
    CSWGuiPanel::OnPanelAdded();
}
