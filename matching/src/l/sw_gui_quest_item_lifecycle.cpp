// The plot-item screen CSWGuiQuestItem (layout "QuestItem", vtable
// 0x00757C20): lists the player's plot items and shows the property
// description of the highlighted one.
#include "l/sw_gui_quest_item.h"

// FUNCTION: SWKOTOR 0x006D2460 SYMBOL: ??0?$CExoLinkedList@VCSWGuiInGameItemEntry@@@@QAE@XZ
// Out-of-line list constructor instantiated for the entry list.

// FUNCTION: SWKOTOR 0x006D24D0
void CSWGuiQuestItem::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x28:
        case 0x2e:
            m_pManager->PlayGuiSoundI(0);
            m_pManager->PopModalPanel();
            SetToDestroy(0);
            break;
        case 0x39:
            (&m_lbDescription)->HandleInputEvent(0x31, 1);
            break;
        case 0x3a:
            (&m_lbDescription)->HandleInputEvent(0x32, 1);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006D2580
void CSWGuiQuestItem::OnControlEntered(CSWGuiControl* pControl)
{
    if (pControl && pControl->m_n4c) {
        m_pSelected = pControl;
        CSWSItem* pItem = g_pAppManager->m_pServerExoApp->GetItemByGameObjectID(
            ((CSWGuiInGameItemEntry*)pControl)->m_oidItem);
        if (pItem) {
            m_lblDescription.m_cText.SetText(pItem->GetPropertyDescription());
        } else {
            m_lblDescription.m_cText.SetText("Error: Invalid item");
        }
    }
    CSWGuiExtent extent = m_lblDescription.m_extent;
    if (m_lblDescription.m_cText.m_sText.GetLength() > 0) {
        extent.height = m_lblDescription.m_cTextBox.GetHeight();
    } else {
        extent.height = m_lblDescription.m_cTextBox.GetLineHeight();
    }
    (&m_lblDescription)->SetExtent(extent);
    CSWGuiControl* pLabel = &m_lblDescription;
    m_lbDescription.AddControls(&pLabel, 1, 0, 0, 0);
    m_lbDescription.SetActiveControl(&m_lblDescription, 0);
}

// FUNCTION: SWKOTOR 0x006D26C0
CSWGuiQuestItem::CSWGuiQuestItem(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    m_pSelected = 0;
    StartLoadFromLayout("QuestItem");
    CSWGuiListBox* pItems = &m_lbItems;
    InitControl(pItems, "LB_ITEMS", 1);
    InitControl(&m_lbDescription, "LB_ITEM_DESCRIPTION", 1);
    InitControl(&m_lblTitle, "LBL_TITLE", 1);
    CSWGuiButton* pBack = &m_btnBack;
    InitControl(pBack, "BTN_BACK", 1);
    m_lblDescription.CopyStyle(m_lbDescription.m_pProtoItem->m_extent, m_lbDescription.m_pProtoItem, 1.0f);
    UnloadResource();
    pItems->m_nListFlags |= 0x20;
    m_lblDescription.m_cText.SetText("");
    pBack->m_bNavigable = 0;
    pBack->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    SetActiveControl(pItems, 0);
}

// FUNCTION: SWKOTOR 0x006D28F0
CSWGuiQuestItem::~CSWGuiQuestItem()
{
    m_lbItems.ClearControls();
    m_lbDescription.ClearControls();
    while (!m_lstEntries.IsEmpty()) {
        CSWGuiInGameItemEntry* pEntry = m_lstEntries.RemoveHead();
        if (pEntry) {
            delete pEntry;
        }
    }
}

// FUNCTION: SWKOTOR 0x006D29F0
// NONMATCH: the entry-list append loads the array before the count and the
// following temporaries rotate eax/ecx/edx (14 insns).
void CSWGuiQuestItem::PopulateItemListBox()
{
    CSWSCreature* pCreature = g_pAppManager->m_pClientExoApp->GetParty()->GetPlayerCharacter()->AsServerCreature();
    CItemRepository* pRepository = pCreature->GetItemRepository(1);
    CExoArrayList<CSWGuiControl*> lstEntries;
    int nWidth = m_lbItems.m_nAreaWidth - m_lbItems.m_nPadding * 2;
    CSWGuiButton* pProto = (CSWGuiButton*)m_lbItems.m_pProtoItem;
    for (unsigned long i = 0; i < pRepository->m_nItems; i++) {
        CSWSItem* pItem = pRepository->GetItemAt(i);
        if (pItem && pItem->m_bPlotItem) {
            CSWGuiInGameItemEntry* pEntry = new CSWGuiInGameItemEntry;
            if (pEntry) {
                pEntry->Initialize((CSWGuiText*)&pProto->m_cTextBox, &pProto->GetBorder(), &pProto->GetHilight(),
                                   nWidth, 0);
                pEntry->AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiQuestItem::OnControlEntered);
                pEntry->SetItem(pItem->m_idSelf, 0, 0);
                m_lstEntries.AddTail(pEntry);
                lstEntries.Add(pEntry);
            }
        }
    }
    m_lbItems.AddControls(lstEntries, 0, 1, 0);
    if (m_pSelected) {
        m_pSelected->SetFlag0(0);
    }
    if (lstEntries.m_nCount > 0) {
        m_lbItems.SetSelectedControl(0, 0);
    } else {
        m_lbItems.SetSelectedControl(-1, 0);
        m_lblDescription.m_cText.SetText("");
        OnControlEntered(0);
    }
}

// FUNCTION: SWKOTOR 0x006D2C00 SYMBOL: ??_GCSWGuiQuestItem@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006D2C20
void CSWGuiQuestItem::OnPanelAdded()
{
    PopulateItemListBox();
    CSWGuiPanel::OnPanelAdded();
}
