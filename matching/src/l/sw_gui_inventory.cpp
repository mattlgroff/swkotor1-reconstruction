// CSWGuiInGameInventory (layout "inventory", 0x006B2A80-0x006B4C90): the
// item list screen with its filter button and the party portraits; plus the
// container panel's small callbacks.
#include "l/sw_gui_inventory.h"

inline CSWSCreature* GetServerPlayerCreature()
{
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    return pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureID());
}

// FUNCTION: SWKOTOR 0x006B2A80
void CSWGuiInGameInventory::SetNextFilter()
{
    int nFilter = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_nInventoryFilter + 1;
    if (nFilter >= 6) {
        nFilter = 0;
    }
    g_pAppManager->m_pClientExoApp->GetInGameGui()->m_nInventoryFilter = nFilter;
    int nNext = nFilter + 1;
    if (nNext >= 6) {
        nNext = 0;
    }
    CExoString sText = g_pTlkTable->GetSimpleString(0xa577) + " " +
                       g_pTlkTable->GetSimpleString(g_aInventoryFilterNames[nNext]);
    m_button14ec.m_cTextParams.SetText(sText);
    sText = g_pTlkTable->GetSimpleString(0x7dab) + " - " +
            g_pTlkTable->GetSimpleString(g_aInventoryFilterNames[nFilter]);
    m_label1a4.m_cText.SetText(sText);
}

// FUNCTION: SWKOTOR 0x006B2C90
int CSWGuiInGameInventory::CheckFilter(CSWSItem* pItem, CSWSCreature* pCreature, int bEquipped)
{
    int bResult = 0;
    if (!pItem) {
        return 0;
    }
    switch (g_pAppManager->m_pClientExoApp->GetInGameGui()->m_nInventoryFilter) {
    case 0:
        bResult = 1;
        break;
    case 1:
        bResult = pItem->m_bItemFlag7;
        break;
    case 2:
        bResult = pItem->m_bPlotItem;
        break;
    case 3:
        if (((CSWItem*)pItem)->GetBaseItem()) {
            bResult = ((CSWItem*)pItem)->GetBaseItem()->m_nEquipableSlots != 0;
        }
        break;
    case 4:
        if (((CSWItem*)pItem)->GetBaseItem()) {
            if (!pItem->m_bPlotItem && !((CSWItem*)pItem)->GetBaseItem()->m_nEquipableSlots) {
                bResult = 1;
            } else {
                bResult = 0;
            }
        }
        break;
    case 5:
        if (pCreature->GetClientCreature()) {
            if (pCreature->GetClientCreature()->CanEquipItem(pItem, bEquipped, 0xff) &&
                pCreature->CanUseItem(pItem, 0)) {
                bResult = 1;
            } else {
                bResult = 0;
            }
        }
        break;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x006B2DC0
void CSWGuiInGameInventory::UpdatePortraits()
{
    CSWParty* pParty = g_pAppManager->m_pClientExoApp->GetParty();
    if (pParty->m_nMembers > 1) {
        m_button16b0.m_bVisible = 1;
        m_button16b0.SetFillImage(((CSWCCreaturePortraitVirtuals*)pParty->GetPartyMember(1))->GetPortrait(1));
        m_button16b0.SetHilightFillImage(
            ((CSWCCreaturePortraitVirtuals*)pParty->GetPartyMember(1))->GetPortrait(1));
    } else {
        m_button16b0.m_bVisible = 0;
    }
    if (pParty->m_nMembers > 2) {
        m_button1874.m_bVisible = 1;
        m_button1874.SetFillImage(((CSWCCreaturePortraitVirtuals*)pParty->GetPartyMember(2))->GetPortrait(1));
        m_button1874.SetHilightFillImage(
            ((CSWCCreaturePortraitVirtuals*)pParty->GetPartyMember(2))->GetPortrait(1));
    } else {
        m_button1874.m_bVisible = 0;
    }
}

// FUNCTION: SWKOTOR 0x006B4430
void CSWGuiInGameInventory::OnPanelRemoved()
{
    m_items.ClearControls();
    for (int i = 0; i < m_entries.m_nCount; i++) {
        if (m_entries.m_pArray[i]) {
            delete m_entries.m_pArray[i];
        }
        m_entries.m_pArray[i] = 0;
    }
    m_entries.SetSize(0);
    g_pAppManager->m_pServerExoApp->GetPartyTable()->GetPartyItems()->ClearNewItems(&m_newItems);
}

// FUNCTION: SWKOTOR 0x006B4BB0
void CSWGuiInGameInventory::Draw(float fDelta)
{
    if (m_nInventoryFlags & 1) {
        PopulateItemListBox();
    }
    UpdatePlayerStats();
    if (m_nInventoryFlags & 4) {
        if (!GetServerPlayerCreature()->m_nAB0) {
            m_button1328.m_cTextParams.SetColor(g_vDifficultyExitColor);
        } else {
            m_button1328.m_cTextParams.SetColor(g_vKeyMapLockedTextColor);
        }
    } else {
        m_button1328.m_cTextParams.SetColor(g_vKeyMapLockedTextColor);
    }
    CSWGuiPanel::Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x006B5250
void CSWGuiContainer::OnPanelRemoved()
{
    g_pGuiManager->RestoreOldMousePosition();
    CSWGuiPanel::OnPanelRemoved();
}

// FUNCTION: SWKOTOR 0x006B3ED0
void CSWGuiInGameInventory::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x3a:
            (&m_description)->HandleInputEvent(0x32, 1);
            break;
        case 0x39:
            (&m_description)->HandleInputEvent(0x31, 1);
            break;
        case 0xce:
            m_pManager->PlayGuiSound(1);
            g_pAppManager->m_pClientExoApp->ChangeCharacterToNextLivingPartyMember(0, 0, 1);
            m_nInventoryFlags |= 1;
            UpdatePortraits();
            break;
        case 0x28:
        case 0x2d:
        case 0x2e:
        case 0xdf:
            if (g_pAppManager->m_pClientExoApp->GetInGameGui()->HideSWInGameGui(0)) {
                g_pAppManager->m_pClientExoApp->SetInputClass(0, 1);
            }
            break;
        case 0x29:
            SetNextFilter();
            m_pManager->PlayGuiSound(1);
            m_nInventoryFlags |= 1;
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006B51D0
void CSWGuiContainer::OnPanelAdded()
{
    CSWGuiExtent cPanel = m_extent;
    CSWGuiExtent cButton = m_ok.m_extent;
    g_pGuiManager->SaveMousePosition();
    g_pGuiManager->MoveMouseToPosition(cButton.width / 2 + cButton.left + cPanel.left,
                                       cButton.height / 2 + cPanel.top + cButton.top);
    CSWGuiPanel::OnPanelAdded();
}

// FUNCTION: SWKOTOR 0x006B92F0
void CSWGuiContainer::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x2d:
            if (!(m_nContainerFlags & 1)) {
                m_pManager->PlayGuiSound(0);
            }
        case 0x27:
            if (m_nContainerFlags & 1) {
                int bGive = 0;
                if (m_nMode == 1) {
                    bGive = 1;
                }
                m_pManager->PlayGuiSound(0);
                g_pAppManager->GetClientExoApp()->GetMessage()->SendPlayerToServerCloseContainer(m_oidContainer,
                                                                                               bGive);
                g_pAppManager->m_pClientExoApp->GetInGameGui()->HideContainerGui();
            }
            break;
        case 0x28:
        case 0x2e:
            m_pManager->PlayGuiSound(0);
            g_pAppManager->GetClientExoApp()->GetMessage()->SendPlayerToServerCloseContainer(m_oidContainer, 0);
            g_pAppManager->m_pClientExoApp->GetInGameGui()->HideContainerGui();
            break;
        case 0x29:
            m_pManager->PlayGuiSound(0);
            if (m_nContainerFlags & 1) {
                SetupGiveMode(0);
            } else {
                SetContainer(m_oidContainer, m_nMode);
            }
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006B2EC0
void CSWGuiInGameInventory::OnChangeCharacter(CSWGuiControl* pControl)
{
    int nMember = 1;
    if (pControl->m_n4c) {
        if (g_pAppManager->m_pServerExoApp->GetServerInfo()->m_bPartyMode) {
            char nNPC = (char)pControl->m_n58;
            CSWCCreature* pCreature;
            if (nNPC == -1) {
                pCreature = g_pAppManager->m_pClientExoApp->GetPlayerCreature();
            } else {
                CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
                pCreature = pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(
                    g_pAppManager->m_pServerExoApp->GetPartyTable()->GetNPCObject(nNPC, 0, 1)));
            }
            if (pCreature) {
                if (m_button1bfc.m_bVisible) {
                    if (pControl == &m_button16b0) {
                        m_button16b0.m_n58 = m_button1874.m_n58;
                    }
                } else {
                    m_button16b0.m_n58 = 0xfffffffe;
                }
                m_nSelected = nNPC;
            }
        } else {
            if (pControl == &m_button1874) {
                nMember = 2;
            }
            if (!g_pAppManager->m_pClientExoApp->ChangeCharacterToNextLivingPartyMember(0, 0, nMember)) {
                return;
            }
            m_pManager->PlayGuiSound(1);
        }
        m_nInventoryFlags |= 1;
        UpdatePortraits();
    }
}

// FUNCTION: SWKOTOR 0x006B7170
void CSWGuiContainer::GiveItem(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        if (pControl->m_n48) {
            unsigned char nSound = pControl->m_nSound;
            m_pManager->PlayGuiSoundI(nSound);
        }
        int nCount = 1;
        if (g_bClearTargetOnAction) {
            nCount = ((CSWGuiInGameItemEntry*)pControl)->GetNumItems();
        }
        unsigned long oidContainer = m_oidContainer;
        unsigned long oidItem = ((CSWGuiInGameItemEntry*)pControl)->m_oidItem;
        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
        pClient->GetParty()->GetPlayerCharacter()->PerformGiveItem(oidItem, oidContainer, nCount);
        m_nContainerFlags |= 6;
    }
}

// FUNCTION: SWKOTOR 0x006B26E0
void CSWGuiInGameInventory::OnCantEquip(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
        pBox->SetAllowCancel(0);
        pBox->SetMessageStrRef(0xa5f5);
        pBox->SetCloseCallback(0, 0, 0);
        m_pManager->AddPanel(pBox, 1, 1);
    }
}

// FUNCTION: SWKOTOR 0x006B2740
void CSWGuiInGameInventory::OnNotUseable(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
        pBox->SetAllowCancel(0);
        pBox->SetMessageStrRef(0xa5f6);
        pBox->SetCloseCallback(0, 0, 0);
        m_pManager->AddPanel(pBox, 1, 1);
    }
}

// FUNCTION: SWKOTOR 0x006B27A0
void CSWGuiInGameInventory::OnNotUseableOnHawk(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
        pBox->SetAllowCancel(0);
        pBox->SetMessageStrRef(0xbff9);
        pBox->SetCloseCallback(0, 0, 0);
        m_pManager->AddPanel(pBox, 1, 1);
    }
}

// FUNCTION: SWKOTOR 0x006B2800
void CSWGuiInGameInventory::OnFullHealth(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
        pBox->SetAllowCancel(0);
        pBox->SetMessageStrRef(0xa603);
        pBox->SetCloseCallback(0, 0, 0);
        m_pManager->AddPanel(pBox, 1, 1);
    }
}

// FUNCTION: SWKOTOR 0x006B2860
void CSWGuiInGameInventory::OnSquadFullHealth(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
        pBox->SetAllowCancel(0);
        pBox->SetMessageStrRef(0xbd6e);
        pBox->SetCloseCallback(0, 0, 0);
        m_pManager->AddPanel(pBox, 1, 1);
    }
}

// FUNCTION: SWKOTOR 0x006B25E0
void CSWGuiInGameInventory::OnControlSelected(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        CSWGuiInGameItemEntry* pEntry = (CSWGuiInGameItemEntry*)m_items.GetSelectedControl();
        if (pEntry) {
            unsigned long oidItem = pEntry->m_oidItem;
            if (oidItem != 0x7f000000) {
                CSWSCreature* pPlayer = GetServerPlayerCreature();
                if (pPlayer->m_bCombatState && pPlayer->m_nAC0 == 1 && pPlayer->m_nAB0) {
                    CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
                    pBox->SetAllowCancel(0);
                    pBox->SetMessageStrRef(0xa5a9);
                    pBox->SetCloseCallback(0, 0, 0);
                    m_pManager->AddPanel(pBox, 1, 1);
                    return;
                }
                pPlayer->m_n1CC = 0;
                pPlayer->InstantUseItem(oidItem);
                m_nInventoryFlags |= 3;
                m_oidUsedItem = oidItem;
                m_nSavedTop = m_items.m_nTop;
                m_nSavedSelected = m_items.m_nSelected;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006B3D10
void CSWGuiInGameInventory::OnControlEntered(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        unsigned long oidItem = ((CSWGuiInGameItemEntry*)pControl)->m_oidItem;
        if (oidItem != 0x7f000000) {
            CSWSItem* pItem = g_pAppManager->m_pServerExoApp->GetItemByGameObjectID(oidItem);
            CExoString sDescription = pItem->GetPropertyDescription();
            m_label64.m_cText.SetText(sDescription);
            if (m_nSelected != 0xff && g_pAppManager->m_pServerExoApp->GetServerInfo()->m_bPartyMode) {
                m_bInvFlag2 = 0;
            } else {
                m_bInvFlag2 = pControl->m_n58;
            }
            if (pItem->m_bItemFlag7) {
                m_newItems.AddUnique(oidItem);
            }
        } else {
            m_bInvFlag2 = 0;
            CExoString sError("Error: Invalid item");
            m_label64.m_cText.SetText(sError);
        }
        m_description.ClearControls();
        CSWGuiExtent extent = m_label64.m_extent;
        if (m_label64.m_cText.m_sText.GetLength() > 0) {
            extent.height = m_label64.m_cTextBox.GetHeight();
        } else {
            extent.height = m_label64.m_cTextBox.GetLineHeight();
        }
        m_label64.SetExtent(extent);
        CSWGuiControl* pLabel = &m_label64;
        m_description.AddControls(&pLabel, 1, 0, 0, 0);
        m_description.SetActiveControl(&m_label64, 0);
    }
}
