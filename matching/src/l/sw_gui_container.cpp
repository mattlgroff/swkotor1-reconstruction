// CSWGuiContainer (swguicontainer.o): filling the container screen's item
// list from a placeable's inventory.
#include "l/sw_gui_inventory_equip.h"

#define __min(a, b) (((a) < (b)) ? (a) : (b))

class CItemRepository {
public:
    CSWSItem* GetItemAt(int nIndex);                // 0x005560B0
    unsigned long GetItemId(int nIndex);            // 0x005560E0
    char pad_0x0[0x10];
    unsigned long m_nItems;                         // 0x10
};

class CSWSItem {
public:
    char pad_0x0[0x108];
    int m_bPlotItem;                                // 0x108
};

class CSWSPlaceable {
public:
    char pad_0x0[0x36c];
    CItemRepository* m_pItemRepository;             // 0x36c
};

// The list box's prototype item: a label with a hilight border (0x140).
class CSWGuiLabelHilight : public CSWGuiLabel {
public:
    CSWGuiBorder m_cHilight;                        // 0x140
};

// FUNCTION: SWKOTOR 0x006B8130
void CSWGuiContainer::SetContainer(unsigned long oidContainer, int nMode)
{
    m_oidContainer = oidContainer;
    m_nMode = (unsigned char)nMode;
    CExoArrayList<CSWGuiControl*> lstEntries;
    CSWGuiLabelHilight* pProto = (CSWGuiLabelHilight*)m_items.m_pProtoItem;
    CSWGuiText* pText = (CSWGuiText*)&pProto->m_cTextBox;
    CSWGuiBorder* pBorder = &pProto->GetBorder();
    CSWGuiBorder* pHilight = &pProto->m_cHilight;
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
    CItemRepository* pItems =
        pServer->GetPlaceableByGameObjectID(pServer->ClientToServerObjectID(oidContainer))->m_pItemRepository;
    lstEntries.SetSize(pItems->m_nItems);
    CSWGuiListBox* pList = &m_items;
    int nWidth = pList->m_nAreaWidth - pList->m_nPadding * 2;
    for (unsigned long i = 0; i < pItems->m_nItems; i++) {
        unsigned long oidItem = pClient->ServerToClientObjectId(pItems->GetItemId(i));
        lstEntries.m_pArray[i] = new CSWGuiInGameItemEntry();
        ((CSWGuiInGameItemEntry*)lstEntries.m_pArray[i])->Initialize(pText, pBorder, pHilight, nWidth, 0);
        ((CSWGuiInGameItemEntry*)lstEntries.m_pArray[i])->SetItem(oidItem, 0, 0);
    }
    pList->AddControls(lstEntries, 1, 0, 0);
    int nTitle = m_nMode ? 0x189 : 0x18a;
    CExoString sGive = g_pAppManager->m_pClientExoApp->GetGUIString(0xbb0c) + CExoString(" ") +
                       g_pAppManager->m_pClientExoApp->GetGUIString(0x968f);
    m_label70.m_cText.SetStrRef(nTitle);
    m_label2f0.m_cText.SetStrRef(0x968e);
    m_ok.m_cTextParams.SetStrRef(0x968e);
    m_give.m_cTextParams.SetText(sGive);
    m_nContainerFlags |= 1;
    pList->m_nListFlags |= 0x200;
    pList->SetEnabled(0);
    SetActiveControl(0, 0);
    SetActiveControl(pList, 0);
}

// FUNCTION: SWKOTOR 0x006B8410
void CSWGuiContainer::SetupGiveMode(int bKeepSelection)
{
    CExoArrayList<CSWGuiControl*> lstEntries;
    CSWGuiLabelHilight* pProto = (CSWGuiLabelHilight*)m_items.m_pProtoItem;
    CSWGuiText* pText = (CSWGuiText*)&pProto->m_cTextBox;
    CSWGuiBorder* pBorder = &pProto->GetBorder();
    CSWGuiBorder* pHilight = &pProto->m_cHilight;
    CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
    CItemRepository* pItems = g_pAppManager->m_pServerExoApp->GetPartyTable()->GetPartyItems();
    int nEntry = 0;
    CSWGuiListBox* pList = &m_items;
    int nWidth = pList->m_nAreaWidth - pList->m_nPadding * 2;
    for (unsigned long i = 0; i < pItems->m_nItems; i++) {
        unsigned long oidItem = pClient->ServerToClientObjectId(pItems->GetItemId(i));
        CSWSItem* pItem = pItems->GetItemAt(i);
        if (pItem && !pItem->m_bPlotItem) {
            CSWGuiInGameItemEntry* pEntry = new CSWGuiInGameItemEntry();
            lstEntries.Add(pEntry);
            ((CSWGuiInGameItemEntry*)lstEntries.m_pArray[nEntry])->Initialize(pText, pBorder, pHilight, nWidth, 1);
            ((CSWGuiInGameItemEntry*)lstEntries.m_pArray[nEntry])->SetItem(oidItem, 0, 0);
            lstEntries.m_pArray[nEntry]->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiContainer::GiveItem);
            lstEntries.m_pArray[nEntry]->AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiContainer::GiveItem);
            nEntry++;
        }
    }
    CExoString sGive = g_pAppManager->m_pClientExoApp->GetGUIString(0xbb0c) + " " +
                       g_pAppManager->m_pClientExoApp->GetGUIString(0x968e);
    m_label70.m_cText.SetStrRef(0x188);
    m_label2f0.m_cText.SetStrRef(0x968f);
    m_ok.m_cTextParams.SetStrRef(0x968f);
    m_give.m_cTextParams.SetText(sGive);
    m_nContainerFlags &= ~1;
    pList->m_nListFlags &= ~0x200ul;
    pList->SetEnabled(1);
    int nSelected = 0;
    if (bKeepSelection && lstEntries.m_nCount > 0) {
        nSelected = __min(pList->m_nSelected, lstEntries.m_nCount - 1);
    }
    pList->AddControls(lstEntries, 1, 0, 0);
    SetActiveControl(0, 0);
    SetActiveControl(pList, 0);
    if (lstEntries.m_nCount > 0) {
        pList->SetSelectedControl(nSelected, 0);
    }
}
