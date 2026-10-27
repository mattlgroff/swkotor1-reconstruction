// CSWGuiStore (layout "store", vtable 0x00756E38): the merchant screen with
// the shop and inventory item lists, buy/sell confirmation and the price
// and stock read-outs. DeleteItemEntries and the panel callbacks OnPanelAdded and
// OnPanelRemoved are in sw_gui_store_panel_lifecycle.cpp.
#include "l/sw_gui_store.h"

inline CSWSStore* CSWGuiStore::GetStore()
{
    return g_pAppManager->GetServerExoApp()->GetStoreByGameObjectID(m_oidStore);
}

// FUNCTION: SWKOTOR 0x006C0610
void CSWGuiStore::PopulateStore()
{
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidCustomer);
    if (pCreature) {
        m_nGold = pCreature->GetGold();
        m_lblCreditsValue.m_cText.SetText(CExoString((int)m_nGold));
    }
}

// FUNCTION: SWKOTOR 0x006C0690
void CSWGuiStore::SetDescription(const CExoString& sText)
{
    m_lblDescription.m_cText.SetText(sText);
    m_lbDescription.ClearControls();
    CSWGuiExtent extent = m_lblDescription.m_extent;
    if (sText.GetLength() > 0) {
        extent.height = m_lblDescription.m_cTextBox.GetHeight();
    } else {
        extent.height = m_lblDescription.m_cTextBox.GetLineHeight();
    }
    m_lblDescription.SetExtent(extent);
    CSWGuiControl* pControl = &m_lblDescription;
    m_lbDescription.AddControls(&pControl, 1, 0, 0, 0);
    m_lbDescription.SetActiveControl(&m_lblDescription, 0);
    m_lblScrollBar.SetVisibleBit(m_lbDescription.IsOversized());
}

// FUNCTION: SWKOTOR 0x006C0790
unsigned long CSWGuiStore::GetItemBuyValue(CSWSItem* pItem)
{
    CSWSStore* pStore = GetStore();
    if (pStore && pItem) {
        unsigned long nMarkUp = pStore->m_nMarkUpBonus + pStore->m_nMarkUp;
        return pItem->GetCost() * nMarkUp / 100;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006C07F0
unsigned long CSWGuiStore::GetItemSellValue(CSWSItem* pItem)
{
    CSWSStore* pStore = GetStore();
    if (pStore && pItem) {
        unsigned long nMarkDown = pStore->m_nMarkDownBonus + pStore->m_nMarkDown;
        return pItem->GetCost() * nMarkDown / 100;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006C0850
void CSWGuiStore::PopulateInventoryItemListBox()
{
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidCustomer);
    CItemRepository* pRepository;
    if (pCreature) {
        pRepository = pCreature->GetItemRepository(1);
    }
    CExoArrayList<CSWGuiControl*> lstEntries;
    int nIndex = 0;
    int bDone = 0;
    unsigned long nTotal = 0;
    for (unsigned long i = 0; i < pRepository->m_nItems; i++) {
        CSWSItem* pItem = pRepository->GetItemAt(i);
        if (pItem && !pItem->m_bPlotItem) {
            nTotal++;
        }
    }
    unsigned long nMade = 0;
    int nNext = 0;
    int nCurrent = 0;
    if (nTotal > 0) {
        do {
            for (unsigned long j = 0; j < pRepository->m_nItems; j++) {
                if (bDone) {
                    break;
                }
                CSWSItem* pItem = pRepository->GetItemAt(j);
                if (pItem && !pItem->m_bPlotItem) {
                    int nSort = ((CSWItem*)pItem)->GetBaseItem()->m_nInventorySortOrder;
                    if (nSort == nCurrent) {
                        bDone = CreateItemEntry(&nIndex, &lstEntries, pItem->m_idSelf, 1);
                        nMade++;
                    } else if (nSort > nCurrent) {
                        if (nNext == nCurrent || nSort < nNext) {
                            nNext = nSort;
                        }
                    }
                }
            }
            nCurrent = nNext;
        } while (nMade < nTotal);
    }
    m_lbInvItems.AddControls(lstEntries, 0, 1, 0);
    if (m_lbInvItems.m_bVisible) {
        if (lstEntries.m_nCount > 0) {
            m_lbInvItems.SetSelectedControl(0, 0);
        } else {
            m_lbInvItems.SetSelectedControl(-1, 0);
            m_lblCostValue.m_cText.SetText("");
            m_lblStockValue.m_cText.SetText("");
            m_lblDescription.m_cText.SetText("");
        }
    }
    m_n2274 = 0;
}

// FUNCTION: SWKOTOR 0x006C0AA0
void CSWGuiStore::OnControlEntered(CSWGuiControl* pControl)
{
    if (!pControl->IsActivated()) {
        return;
    }
    unsigned long oidItem = ((CSWGuiStoreItemEntry*)pControl)->m_oidItem;
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSItem* pItem = pServer->GetItemByGameObjectID(pServer->ClientToServerObjectID(oidItem));
    if (!pItem) {
        return;
    }
    if (m_lbShopItems.m_bVisible) {
        m_lblCostValue.m_cText.SetText(CExoString((int)GetItemBuyValue(pItem)));
    } else {
        m_lblCostValue.m_cText.SetText(CExoString((int)GetItemSellValue(pItem)));
    }
    if (pItem->m_bInfinite) {
        m_lblStockValue.m_cText.SetStrRef(0xa3df);
    } else {
        m_lblStockValue.m_cText.SetText(CExoString(pItem->GetStackSize()));
    }
    SetDescription(pItem->GetPropertyDescription());
}

// FUNCTION: SWKOTOR 0x006C0BE0
void CSWGuiStore::BuyItem(CSWSItem* pItem)
{
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CSWSCreature* pPlayer = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureID());
    CSWSStore* pStore = GetStore();
    if (!pPlayer || !pStore) {
        return;
    }
    if (!pPlayer->GetItemRepository(1) || !pItem) {
        return;
    }
    m_pManager->PlayGuiSound(9);
    m_nGold -= GetItemBuyValue(pItem);
    pPlayer->SetGold(m_nGold);
    pStore->SellItem(pItem, pPlayer);
    int nSelected = m_lbShopItems.m_nSelected;
    int nTop = m_lbShopItems.m_nTop;
    PopulateStoreItemListBox();
    if (nSelected >= m_lbShopItems.m_nControls) {
        nSelected = m_lbShopItems.m_nControls - 1;
    }
    CSWGuiListBox* pListBox = &m_lbShopItems;
    pListBox->SetTopVisible(nTop);
    pListBox->SetSelectedControl(nSelected, 0);
    m_lblCreditsValue.m_cText.SetText(CExoString((int)m_nGold));
}

// FUNCTION: SWKOTOR 0x006C0D20
void CSWGuiStore::BuyItemCallback(CSWGuiControl* pControl)
{
    if (!g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox->m_bAccepted) {
        return;
    }
    unsigned long oidItem = ((CSWGuiStoreItemEntry*)pControl)->m_oidItem;
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSItem* pItem = pServer->GetItemByGameObjectID(pServer->ClientToServerObjectID(oidItem));
    if (pItem) {
        BuyItem(pItem);
    }
}

// FUNCTION: SWKOTOR 0x006C0D80
void CSWGuiStore::SellItem(CSWSItem* pItem)
{
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CSWSCreature* pPlayer = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureID());
    CSWSStore* pStore = GetStore();
    if (pItem && pPlayer && pStore) {
        CItemRepository* pRepository = pPlayer->GetItemRepository(1);
        if (pRepository) {
            m_pManager->PlayGuiSound(0xb);
            m_nGold += GetItemSellValue(pItem);
            pPlayer->SetGold(m_nGold);
            if (pItem->m_nStackSize > 1) {
                pStore->AcquireItem(pItem->SplitItem(1));
            } else {
                pRepository->RemoveItem(pItem);
                pStore->AcquireItem(pItem);
            }
            int nSelected = m_lbInvItems.m_nSelected;
            int nTop = m_lbInvItems.m_nTop;
            PopulateInventoryItemListBox();
            if (nSelected >= m_lbInvItems.m_nControls) {
                nSelected = m_lbInvItems.m_nControls - 1;
            }
            CSWGuiListBox* pListBox = &m_lbInvItems;
            pListBox->SetTopVisible(nTop);
            pListBox->SetSelectedControl(nSelected, 0);
            m_lblCreditsValue.m_cText.SetText(CExoString((int)m_nGold));
        }
    }
}

// FUNCTION: SWKOTOR 0x006C0EE0
void CSWGuiStore::SellItemCallback(CSWGuiControl* pControl)
{
    if (!g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox->m_bAccepted) {
        return;
    }
    unsigned long oidItem = ((CSWGuiStoreItemEntry*)pControl)->m_oidItem;
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSItem* pItem = pServer->GetItemByGameObjectID(pServer->ClientToServerObjectID(oidItem));
    if (pItem) {
        SellItem(pItem);
    }
}

// FUNCTION: SWKOTOR 0x006C0F40
void CSWGuiStore::OnControlInvAButton(CSWGuiControl* pControl)
{
    if (!pControl->IsActivated()) {
        return;
    }
    if (pControl->m_n48 == 0x2d) {
        m_pManager->PlayGuiSound(0);
    }
    if (pControl == &m_btnAccept) {
        pControl = m_lbInvItems.GetSelectedControl();
        if (!pControl) {
            return;
        }
    }
    unsigned long oidItem = ((CSWGuiStoreItemEntry*)pControl)->m_oidItem;
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSItem* pItem = pServer->GetItemByGameObjectID(pServer->ClientToServerObjectID(oidItem));
    if (!pItem) {
        return;
    }
    CServerExoApp* pServerB = g_pAppManager->GetServerExoApp();
    CSWSCreature* pPlayer = pServerB->GetCreatureByGameObjectID(pServerB->GetPlayerCreatureID());
    unsigned long nLimit;
    if (pPlayer && pPlayer->m_pStats->GetLevel(0) * 50 <= 250) {
        nLimit = pPlayer->m_pStats->GetLevel(0) * 50;
    } else {
        nLimit = 250;
    }
    if (GetItemSellValue(pItem) > nLimit) {
        CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
        CExoString sMessage;
        g_pTlkTable->SetCustomToken(0, CExoString((int)nLimit));
        sMessage = ((CSWTlkTable*)g_pTlkTableAlt)->Fetch(0xa401, 0, 0, 0);
        pBox->SetAllowCancel(1);
        pBox->SetMessage(sMessage);
        pBox->SetCloseCallback(this, (PFN_GUI_CALLBACK)&CSWGuiStore::SellItemCallback, (int)pControl);
        g_pAppManager->m_pClientExoApp->GetGuiManager()->AddPanel(pBox, 1, 1);
    } else {
        SellItem(pItem);
    }
}

// FUNCTION: SWKOTOR 0x006C1130
void CSWGuiStore::OnControlStoreAButton(CSWGuiControl* pControl)
{
    if (!pControl->IsActivated()) {
        return;
    }
    if (pControl->m_n48) {
        m_pManager->PlayGuiSound(0);
    }
    if (pControl == &m_btnAccept) {
        pControl = m_lbShopItems.GetSelectedControl();
        if (!pControl) {
            return;
        }
    }
    unsigned long oidItem = ((CSWGuiStoreItemEntry*)pControl)->m_oidItem;
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSItem* pItem = pServer->GetItemByGameObjectID(pServer->ClientToServerObjectID(oidItem));
    if (!pItem) {
        return;
    }
    if (m_nGold >= GetItemBuyValue(pItem)) {
        CServerExoApp* pServerB = g_pAppManager->GetServerExoApp();
        CSWSCreature* pPlayer = pServerB->GetCreatureByGameObjectID(pServerB->GetPlayerCreatureID());
        unsigned long nLimit;
        if (pPlayer) {
            nLimit = pPlayer->GetStats()->GetLevel(0) * 50;
        } else {
            nLimit = 50;
        }
        if (GetItemBuyValue(pItem) > nLimit) {
            CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
            CExoString sMessage;
            g_pTlkTable->SetCustomToken(0, CExoString((int)nLimit));
            sMessage = ((CSWTlkTable*)g_pTlkTableAlt)->Fetch(0xa424, 0, 0, 0);
            pBox->SetAllowCancel(1);
            pBox->SetMessage(sMessage);
            pBox->SetCloseCallback(this, (PFN_GUI_CALLBACK)&CSWGuiStore::BuyItemCallback, (int)pControl);
            g_pAppManager->m_pClientExoApp->GetGuiManager()->AddPanel(pBox, 1, 1);
        } else {
            BuyItem(pItem);
        }
    } else {
        g_pAppManager->m_pClientExoApp->GetInGameGui()->ShowExamineBox(0xa3de, 1);
    }
}

// FUNCTION: SWKOTOR 0x006C13C0
void CSWGuiStore::ShowSellGUI()
{
    CSWGuiListBox* pInvItems = &m_lbInvItems;
    pInvItems->m_bVisible = 1;
    m_lbShopItems.m_bVisible = 0;
    m_lblTitle.m_cText.SetStrRef(0x7d82);
    m_btnAccept.m_cTextParams.SetStrRef(0x7d82);
    m_btnAccept.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiStore::OnControlInvAButton);
    m_lblBuySell.m_cText.SetStrRef(0x7d82);
    m_lblShopTitle.m_cText.SetStrRef(0xa3d1);
    m_btnExamine.m_cTextParams.SetStrRef(0xa3d1);
    m_lblCost.m_cText.SetStrRef(0xa3d9);
    SetActiveControl(pInvItems, 0);
    PopulateInventoryItemListBox();
}

// FUNCTION: SWKOTOR 0x006C1660
int CSWGuiStore::CreateItemEntry(int* pIndex, CExoArrayList<CSWGuiControl*>* pList, unsigned long oidItem,
                                 int bInventory)
{
    CExoArrayList<CSWGuiStoreItemEntry*>* pPool = bInventory ? &m_lstInvEntries : &m_lstStoreEntries;
    CSWGuiStoreItemEntry* pEntry;
    if (*pIndex < pPool->m_nCount) {
        pEntry = pPool->m_pArray[*pIndex];
    } else {
        CSWGuiStoreItemEntry* pNew = new CSWGuiStoreItemEntry;
        pEntry = pNew;
        pPool->Add(pNew);
        pNew->m_nSound = 0xff;
        if (bInventory) {
            CSWGuiButton* pProto = (CSWGuiButton*)m_lbInvItems.m_pProtoItem->IsSelectable();
            pNew->Initialize((CSWGuiText*)&pProto->m_cTextBox, &pProto->GetBorder(), &pProto->GetHilight(),
                             m_lbInvItems.m_nAreaWidth - m_lbInvItems.m_nPadding * 2);
            pNew->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiStore::OnControlInvAButton);
            pNew->AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiStore::OnControlInvAButton);
        } else {
            CSWGuiButton* pProto = (CSWGuiButton*)m_lbShopItems.m_pProtoItem->IsSelectable();
            pNew->Initialize((CSWGuiText*)&pProto->m_cTextBox, &pProto->GetBorder(), &pProto->GetHilight(),
                             m_lbShopItems.m_nAreaWidth - m_lbShopItems.m_nPadding * 2);
            pNew->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiStore::OnControlStoreAButton);
            pNew->AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiStore::OnControlStoreAButton);
        }
        pNew->AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiStore::OnControlEntered);
    }
    pEntry->SetItem(oidItem, 0);
    pEntry->SetFlag0(0);
    pList->Add(pEntry);
    (*pIndex)++;
    return 0;
}

// FUNCTION: SWKOTOR 0x006C1840
void CSWGuiStore::PopulateStoreItemListBox()
{
    CSWSStore* pStore = GetStore();
    CExoArrayList<CSWGuiControl*> lstEntries;
    CItemRepository* pRepository = pStore->m_pItems;
    int nIndex = 0;
    int bDone = 0;
    unsigned long nMade = 0;
    int nNext = 0;
    int nCurrent = 0;
    if (pRepository->m_nItems > 0) {
        do {
            for (unsigned long i = 0; i < pRepository->m_nItems; i++) {
                if (bDone) {
                    break;
                }
                CSWSItem* pItem = pRepository->GetItemAt(i);
                if (pItem && !pItem->m_bPlotItem) {
                    int nSort = ((CSWItem*)pItem)->GetBaseItem()->m_nInventorySortOrder;
                    if (nSort == nCurrent) {
                        bDone = CreateItemEntry(&nIndex, &lstEntries, pItem->m_idSelf, 0);
                        nMade++;
                    } else if (nSort > nCurrent) {
                        if (nNext == nCurrent || nSort < nNext) {
                            nNext = nSort;
                        }
                    }
                }
            }
            nCurrent = nNext;
        } while (nMade < pRepository->m_nItems);
    }
    m_lbShopItems.AddControls(lstEntries, 0, 1, 0);
    if (m_lbShopItems.m_bVisible) {
        if (lstEntries.m_nCount > 0) {
            m_lbShopItems.SetSelectedControl(0, 0);
        } else {
            m_lbShopItems.SetSelectedControl(-1, 0);
            m_lblCostValue.m_cText.SetText("");
            m_lblStockValue.m_cText.SetText("");
            m_lblDescription.m_cText.SetText("");
        }
    }
    m_n2274 = 0;
}

// FUNCTION: SWKOTOR 0x006C1A50
void CSWGuiStore::ShowBuyGUI()
{
    m_lbInvItems.m_bVisible = 0;
    CSWGuiListBox* pShopItems = &m_lbShopItems;
    pShopItems->m_bVisible = 1;
    m_lblTitle.m_cText.SetStrRef(0x7d84);
    m_btnAccept.m_cTextParams.SetStrRef(0x7d84);
    m_btnAccept.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiStore::OnControlStoreAButton);
    m_lblBuySell.m_cText.SetStrRef(0x7d84);
    m_lblShopTitle.m_cText.SetStrRef(0xa3d2);
    m_btnExamine.m_cTextParams.SetStrRef(0xa3d2);
    m_lblCost.m_cText.SetStrRef(0xa3d7);
    SetActiveControl(pShopItems, 0);
    PopulateStoreItemListBox();
}

// FUNCTION: SWKOTOR 0x006C1B00
void CSWGuiStore::OnControlXButton(CSWGuiControl* pControl)
{
    if (pControl && !pControl->IsActivated()) {
        return;
    }
    CSWSStore* pStore = GetStore();
    if (!pStore || pStore->m_nBuySellFlag != 3) {
        return;
    }
    if (m_lbShopItems.m_bVisible) {
        ShowSellGUI();
    } else {
        ShowBuyGUI();
    }
}

// FUNCTION: SWKOTOR 0x006C1B50
void CSWGuiStore::InitializeStoreType()
{
    CSWSStore* pStore = GetStore();
    if (!pStore) {
        return;
    }
    switch (pStore->m_nBuySellFlag) {
    case 3:
        ShowControl(&m_lblInvTitle);
        ShowControl(&m_lblShopTitle);
        ShowControl(&m_btnExamine);
        ShowBuyGUI();
        break;
    case 2:
        HideControl(&m_lblInvTitle);
        HideControl(&m_lblShopTitle);
        HideControl(&m_btnExamine);
        ShowSellGUI();
        break;
    case 1:
        HideControl(&m_lblInvTitle);
        HideControl(&m_lblShopTitle);
        HideControl(&m_btnExamine);
        ShowBuyGUI();
        break;
    }
}

// FUNCTION: SWKOTOR 0x006C2190
void CSWGuiStore::HandleInputEvent(int nEvent, int bDown)
{
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
    if (bDown) {
        switch (nEvent) {
        case 0x28:
        case 0x2e:
            m_pManager->PlayGuiSound(0);
            g_pAppManager->m_pClientExoApp->GetInGameGui()->HideStoreGui();
            break;
        case 0x3a:
            m_lbDescription.HandleInputEvent(0x32, 1);
            break;
        case 0x39:
            m_lbDescription.HandleInputEvent(0x31, 1);
            break;
        case 0x29:
            m_pManager->PlayGuiSound(0);
            OnControlXButton(0);
            break;
        }
    }
}
