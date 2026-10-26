// CSWGuiUpgrade / CSWGuiUpgradeItemSelect (swguiupgrade.o): building the
// upgradable item lists and moving upgrade items between the party
// inventory and the working item.

#include <string.h>
#include "l/sw_gui_upgrade.h"

// FUNCTION: SWKOTOR 0x006C6120
void CSWGuiUpgrade::VerifyUpgrade(CSWSItem* pItem)
{
    int nSlot = m_pControl1c->m_n58;
    if (g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox->m_bAccepted) {
        InsertUpgrade(pItem, nSlot);
        UpdateItems();
        OnEnterSlot(m_pControl1c);
        return;
    }
    pItem->m_nUpgrades &= ~(1 << m_aSlotRows[nSlot]);
}

// FUNCTION: SWKOTOR 0x006C58C0
CSWUpgradeItemEntry* CSWGuiUpgrade::CreateItemEntry(unsigned long oidItem, unsigned char nSlot)
{
    int nWidth = m_lbItems.m_nAreaWidth - m_lbItems.m_nPadding * 2;
    CSWGuiButton* pProto = (CSWGuiButton*)m_lbItems.m_pProtoItem;
    CSWUpgradeItemEntry* pEntry = new CSWUpgradeItemEntry;
    pEntry->Initialize((CSWGuiText*)&pProto->m_cTextBox, &pProto->GetBorder(), &pProto->GetHilight(), nWidth);
    pEntry->AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiUpgrade::OnControlEntered);
    pEntry->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiUpgrade::OnUpgradeSelected);
    pEntry->AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiUpgrade::OnUpgradeSelected);
    pEntry->SetItem(oidItem, 0, 0x7f000000, 0);
    pEntry->m_nType = nSlot;
    return pEntry;
}

// FUNCTION: SWKOTOR 0x006C59A0
void CSWGuiUpgrade::InsertUpgrade(CSWSItem* pItem, int nSlot)
{
    if (!pItem) {
        return;
    }
    int nIndex = m_lstRemoved.IndexOf(pItem);
    if (nIndex != -1) {
        m_lstRemoved.DelIndex(nIndex);
        if (pItem->m_nStackSize > 1) {
            pItem = pItem->SplitItem(1);
        } else {
            g_pAppManager->m_pServerExoApp->GetPartyTable()->GetPartyItems()->RemoveItem(pItem);
        }
    } else {
        if (pItem->m_nStackSize > 1) {
            pItem = pItem->SplitItem(1);
        } else {
            g_pAppManager->m_pServerExoApp->GetPartyTable()->GetPartyItems()->RemoveItem(pItem);
        }
        m_lstAdded.Add(pItem);
    }
    m_aSlotItems[nSlot] = pItem;
}

// FUNCTION: SWKOTOR 0x006C4960
void CSWGuiUpgradeItemSelect::BuildItemList(CSWSCreature* pCreature, CExoArrayList<CSWGuiControl*>* pList, C2DA* p2DA)
{
    CSWGuiButton* pProto = (CSWGuiButton*)m_lbItems.m_pProtoItem;
    int nWidth = m_lbItems.m_nAreaWidth - m_lbItems.m_nPadding * 2;
    for (int i = 0; i < 18; i++) {
        unsigned long nSlot = 1 << i;
        CSWSItem* pItem = pCreature->GetInventory()->GetItemInSlot(nSlot);
        if (pItem && pItem->GetUpgradableType(p2DA) == m_nUpgradeType) {
            CSWUpgradeItemEntry* pEntry = new CSWUpgradeItemEntry;
            pEntry->Initialize((CSWGuiText*)&pProto->m_cTextBox, &pProto->GetBorder(), &pProto->GetHilight(), nWidth);
            pEntry->AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiUpgradeItemSelect::OnControlEntered);
            pEntry->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiUpgradeItemSelect::OnUpgradableSelected);
            pEntry->AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiUpgradeItemSelect::OnUpgradableSelected);
            pEntry->SetItem(pItem->m_idSelf, 1, pCreature->m_idSelf, nSlot);
            pList->Add(pEntry);
        }
    }
}

// FUNCTION: SWKOTOR 0x006C2DF0
void CSWGuiUpgradeItemSelect::OnUpgradableSelected(CSWGuiControl* pControl)
{
    if (!pControl->m_n4c) {
        return;
    }
    if (pControl->m_n48 == 0x2d) {
        m_pManager->PlayGuiSound(0);
    }
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    if (pControl == &m_btnUpgradeItem) {
        m_pSelectedEntry = (CSWUpgradeItemEntry*)m_lbItems.GetSelectedControl();
        if (!m_pSelectedEntry) {
            return;
        }
    } else {
        m_pSelectedEntry = (CSWUpgradeItemEntry*)pControl;
    }
    CSWSItem* pItem = pServer->GetItemByGameObjectID(m_pSelectedEntry->m_oidItem);
    m_bUnequipped = 0;
    if (m_pSelectedEntry->m_bEquipped) {
        CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(m_pSelectedEntry->m_oidOwner);
        if (m_pSelectedEntry->GetSlot() == 0x10 || m_pSelectedEntry->GetSlot() == 0x20) {
            CSWSItem* pRight = pCreature->GetInventory()->GetItemInSlot(0x10);
            CSWSItem* pLeft = pCreature->GetInventory()->GetItemInSlot(0x20);
            if (pLeft) {
                if (pRight == pItem) {
                    m_bUnequipped = 1;
                    m_bOtherUnequipped = 0;
                    m_oidOtherItem = pLeft->m_idSelf;
                    pCreature->UnequipItem(pLeft);
                    pCreature->UnequipItem(pItem);
                } else if (pLeft == pItem) {
                    m_bUnequipped = 1;
                    m_bOtherUnequipped = 1;
                    m_oidOtherItem = pRight->m_idSelf;
                    pCreature->UnequipItem(pItem);
                    pCreature->UnequipItem(pRight);
                }
            } else {
                pCreature->UnequipItem(pItem);
            }
        } else {
            pCreature->UnequipItem(pItem);
        }
    } else if (pItem->m_nStackSize > 1) {
        pItem = pItem->SplitItem(1);
    } else {
        pServer->GetPartyTable()->GetPartyItems()->RemoveItem(pItem);
    }
    m_pUpgrade->m_nType = m_nUpgradeType;
    m_pUpgrade->m_pItem = pItem;
    m_pManager->AddPanel(m_pUpgrade, 3, 1);
}

// FUNCTION: SWKOTOR 0x006C6330
void CSWGuiUpgradeItemSelect::OnPanelAdded()
{
    if (m_nUpgradeType > 0 && m_nUpgradeType <= 4) {
        if (m_pItem) {
            unsigned long oidPossessor = m_pItem->m_oidPossessor;
            CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
            CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(oidPossessor);
            unsigned long nSlot = pCreature->GetInventory()->GetSlotFromItem(m_pItem);
            m_pSelectedEntry = new CSWUpgradeItemEntry;
            if ((int)nSlot > 0) {
                m_pSelectedEntry->SetItem(m_pItem->m_idSelf, 1, pCreature->m_idSelf, nSlot);
                pCreature->UnequipItem(m_pItem);
            } else if (m_pItem->m_nStackSize > 1) {
                m_pItem = m_pItem->SplitItem(1);
            } else {
                pServer->GetPartyTable()->GetPartyItems()->RemoveItem(m_pItem);
            }
            m_pUpgrade->m_nType = m_nUpgradeType;
            m_pUpgrade->m_pItem = m_pItem;
            m_pManager->AddPanel(m_pUpgrade, 3, 1);
            return;
        }
        BuildUpgradeList();
        m_lbItems.SetSelectedControl(0, 0);
        SetActiveControl(&m_lbItems, 0);
        CSWGuiPanel::OnPanelAdded();
        return;
    }
    HidePanel();
}

// FUNCTION: SWKOTOR 0x006C3AA0
void CSWGuiUpgrade::UpdateItems()
{
    CResRef cIcon;
    CExoString sUnused;
    CSWGuiLabel* pLabels;
    int nFirst = 0;
    switch (m_nType) {
    case 1:
    case 3:
        pLabels = m_aSlotLabels3;
        break;
    case 4:
        nFirst = 1;
    case 2:
        pLabels = m_aSlotLabels4;
        break;
    }
    unsigned char nCount = 0;
    switch (m_nType) {
    case 1:
    case 3:
        nCount = 3;
        break;
    case 2:
        nCount = 4;
        break;
    case 4:
        nCount = 2;
        break;
    }
    int nEnd = nCount + nFirst;
    for (int i = nFirst; i < nEnd; i++) {
        if (m_aSlotItems[i]) {
            CSWBaseItem* pBaseItem = g_pRules->m_pBaseItems->GetBaseItem(m_aSlotItems[i]->GetBaseItemType());
            cIcon = m_aSlotItems[i]->GetIcon();
        } else {
            cIcon = g_aUpgradeSlots[m_nType - 1][i].m_sIcon;
        }
        pLabels[i].m_cBorder.SetFillImage(cIcon, 0);
    }
    SetDescription(m_pItem->GetPropertyDescription());
}

// FUNCTION: SWKOTOR 0x006C3C30
void CSWGuiUpgrade::OnEnterSlot(CSWGuiControl* pControl)
{
    if (!pControl->m_n4c) {
        return;
    }
    int nSlot = pControl->m_n58;
    int nStrRef = g_aUpgradeSlots[m_nType - 1][nSlot].m_nNameStrRef;
    if (m_nType == 1) {
        if (m_aSlotItems[nSlot]) {
            CExoString sName;
            CExoLocString sLocName(m_aSlotItems[nSlot]->m_sLocName);
            sLocName.GetString(g_pAppManager->GetClientExoApp()->GetClientLanguage(), &sName, 0);
            m_lblLSSlotName.m_cText.SetText(sName);
        } else {
            m_lblLSSlotName.m_cText.SetStrRef(nStrRef);
        }
        return;
    }
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    int nCount = 0;
    CSWSItem* pUpgrade = pServer->GetItemByGameObjectID(
        pServer->GetPartyTable()->GetPartyItems()->FindItemWithTag(m_aSlotTags[nSlot]));
    if (pUpgrade) {
        nCount = pUpgrade->m_nStackSize;
    }
    m_lblSlotName.m_cText.SetStrRef(nStrRef);
    {
        CExoString sCount(nCount);
        m_lblUpgradeCount.m_cText.SetText(sCount);
    }
    m_lblUpgrades.m_cText.SetStrRef(42026);
    m_lblProperty.m_cText.SetText(m_pItem->GetKeyedPropertyString(m_aSlotRows[nSlot]));
}

// FUNCTION: SWKOTOR 0x006C3460
void CSWGuiUpgrade::GetArmorModelAndTextureResRef(int nRow, int nModelVariation, int nTextureVariation,
                                                  CExoString* pModel, CExoString* pTexture)
{
    char szModel[] = "ModelA";
    char szTexture[] = "TEXA";
    if (nModelVariation <= 10) {
        if (nModelVariation < 1) {
            nModelVariation = 1;
        }
    } else {
        nModelVariation = 10;
    }
    int nTexture = 1;
    if (nTextureVariation >= 1) {
        nTexture = nTextureVariation;
    }
    szTexture[3] = szModel[5] = 'A' - 1 + nModelVariation;
    bool bFailed = g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(nRow, szModel, pModel) == 0;
    if (bFailed) {
        *pModel = "";
    }
    if (g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(nRow, szTexture, pTexture)) {
        pTexture->Format("%s%02d", pTexture->CStr(), nTexture);
        {
            CResRef cTexture(*pTexture);
            if (g_pExoResMan->Exists(cTexture, 3, 0)) {
                return;
            }
        }
        {
            CResRef cTexture(*pTexture);
            if (g_pExoResMan->Exists(cTexture, 0xbbf, 0)) {
                return;
            }
        }
        pTexture->Format("%s%02d", pModel->CStr(), 1);
    } else {
        *pTexture = "";
    }
}

// FUNCTION: SWKOTOR 0x006C5E90
void CSWGuiUpgradeItemSelect::FinishUpgrading()
{
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSItem* pItem = m_pUpgrade->m_pItem;
    if (m_pSelectedEntry->m_bEquipped) {
        unsigned long nSlot = m_pSelectedEntry->m_nSlot;
        CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(m_pSelectedEntry->GetOwner());
        int bEquip = 1;
        if (!pCreature->CanEquipItem(pItem, &nSlot, 0, 0, 0)) {
            pServer->GetPartyTable()->GetPartyItems()->AddItem(&pItem, 1, 0, 0);
            m_pSelectedEntry->SetItem(pItem->m_idSelf, 0, 0x7f000000, 0);
            bEquip = 0;
        }
        if (m_bUnequipped) {
            if (m_bOtherUnequipped) {
                pCreature->EquipItem(0x10, pServer->GetItemByGameObjectID(m_oidOtherItem), 1, 0, 0);
                if (bEquip) {
                    pCreature->EquipItem(0x20, pItem, 1, 0, 0);
                }
            } else if (bEquip) {
                pCreature->EquipItem(0x10, pItem, 1, 0, 0);
                pCreature->EquipItem(0x20, pServer->GetItemByGameObjectID(m_oidOtherItem), 1, 0, 0);
            } else {
                pCreature->EquipItem(0x10, pServer->GetItemByGameObjectID(m_oidOtherItem), 1, 0, 0);
            }
        } else if (bEquip) {
            pCreature->EquipItem(nSlot, pItem, 1, 0, 0);
        }
    } else {
        pServer->GetPartyTable()->GetPartyItems()->AddItem(&pItem, 1, 0, 0);
    }
    if (m_pItem) {
        HidePanel();
        m_pManager->PopModalPanel();
        ((CGuiInGameUpgrade*)g_pAppManager->m_pClientExoApp->GetInGameGui())->HideUpgradeScreen();
        m_pItem = 0;
        if (m_pSelectedEntry) {
            delete m_pSelectedEntry;
        }
        return;
    }
    if (m_pSelectedEntry->m_oidItem != pItem->m_idSelf) {
        BuildUpgradeList();
    }
    for (int i = 0; m_lbItems.m_nControls; i++) {
        CSWUpgradeItemEntry* pEntry = (CSWUpgradeItemEntry*)m_lbItems.GetControl(i);
        if (pEntry->m_oidItem == pItem->m_idSelf) {
            m_pSelectedEntry = pEntry;
            break;
        }
    }
    m_lbItems.SetSelectedControl(-1, 0);
    m_lbItems.SetActiveControl(m_pSelectedEntry, 0);
}

// FUNCTION: SWKOTOR 0x006C4520
void CSWGuiUpgradeSelection::OnPanelAdded()
{
    C2DA* p2DA = new C2DA(CResRef("upgrade"), 0);
    p2DA->Load2DArray();
    CSWSItem* pItem;
    if (m_oidItem != 0x7f000000 &&
        (pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(m_oidItem)) != 0) {
        m_pItemSelect->SetUpgradeType(pItem->GetUpgradableType(p2DA));
        m_pItemSelect->m_pItem = pItem;
        m_pManager->AddPanel(m_pItemSelect, 3, 1);
    } else {
        CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
        CSWPartyTable* pParty = pServer->GetPartyTable();
        CItemRepository* pRepository = pParty->GetPartyItems();
        memset(m_aAvailable, 0, sizeof(m_aAvailable));
        GetUpgradableItems(pServer->GetCreatureByGameObjectID(
                               pServer->GetPlayerList()->GetHead()->AsSWSPlayer()->m_oidServer),
                           m_aAvailable, p2DA);
        unsigned int i;
        for (i = 0; i < 9; i++) {
            if (pParty->GetNPCAvailable(i) && pParty->GetNPCSelectable(i)) {
                CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(pParty->GetNPCObject(i, 0, 1));
                if (pCreature) {
                    GetUpgradableItems(pCreature, m_aAvailable, p2DA);
                }
            }
        }
        for (i = 0; i < pRepository->m_nItems; i++) {
            CSWSItem* pRepositoryItem = pRepository->GetItemAt(i);
            if (pRepositoryItem) {
                int nType = pRepositoryItem->GetUpgradableType(p2DA);
                if (nType > 0) {
                    m_aAvailable[nType - 1] = 1;
                }
            }
        }
        for (int j = 0; j < 4; j++) {
            m_aTypeLabels[j].SetVisibleBit(m_aAvailable[j]);
            if (m_aAvailable[j]) {
                m_aTypes[j].m_cTextParams.SetColor(g_vDifficultyExitColor);
                m_aTypes[j].SetEnabled(1);
            } else {
                m_aTypes[j].m_cTextParams.SetColor(g_vKeyMapLockedTextColor);
                m_aTypes[j].SetEnabled(0);
            }
        }
        SetActiveControl(&m_aTypes[0], 0);
    }
    if (p2DA) {
        delete p2DA;
    }
    CSWGuiPanel::OnPanelAdded();
}

// FUNCTION: SWKOTOR 0x006C5B90
void CSWGuiUpgradeItemSelect::BuildUpgradeList()
{
    CExoArrayList<CSWGuiControl*> lstControls;
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CSWPartyTable* pParty = pServer->GetPartyTable();
    CItemRepository* pRepository = pParty->GetPartyItems();
    CSWGuiListBox* pListBox = &m_lbItems;
    CSWGuiButton* pProto = (CSWGuiButton*)m_lbItems.m_pProtoItem;
    int nWidth = pListBox->m_nAreaWidth - m_lbItems.m_nPadding * 2;
    C2DA* p2DA = new C2DA(CResRef("upgrade"), 0);
    p2DA->Load2DArray();
    CSWSCreature* pPlayer = pServer->GetCreatureByGameObjectID(
        pServer->GetPlayerList()->GetHead()->AsSWSPlayer()->m_oidServer);
    BuildItemList(pPlayer, &lstControls, p2DA);
    unsigned int i;
    for (i = 0; i < 9; i++) {
        if (pParty->GetNPCAvailable(i) && pParty->GetNPCSelectable(i)) {
            CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(pParty->GetNPCObject(i, 0, 1));
            if (pCreature) {
                BuildItemList(pCreature, &lstControls, p2DA);
            }
        }
    }
    for (i = 0; i < pRepository->m_nItems; i++) {
        CSWSItem* pItem = pRepository->GetItemAt(i);
        if (pItem && pItem->GetUpgradableType(p2DA) == m_nUpgradeType) {
            CSWUpgradeItemEntry* pEntry = new CSWUpgradeItemEntry;
            pEntry->Initialize((CSWGuiText*)&pProto->m_cTextBox, &pProto->GetBorder(), &pProto->GetHilight(), nWidth);
            pEntry->AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiUpgradeItemSelect::OnControlEntered);
            pEntry->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiUpgradeItemSelect::OnUpgradableSelected);
            pEntry->AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiUpgradeItemSelect::OnUpgradableSelected);
            pEntry->SetItem(pItem->m_idSelf, 0, 0x7f000000, 0);
            lstControls.AddInline(pEntry);
        }
    }
    if (p2DA) {
        delete p2DA;
    }
    pListBox->AddControls(lstControls, 1, 0, 0);
}
