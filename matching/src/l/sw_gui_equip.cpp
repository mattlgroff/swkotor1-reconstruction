// CSWGuiInGameEquip (layout "equip", 0x006B5600-0x006BB530): slot icons,
// the item description box and the equip/unequip actions.
#include "l/sw_gui_inventory.h"

// FUNCTION: SWKOTOR 0x006B5600
CExoString CSWGuiInGameEquip::GetIcon(int nSlot)
{
    CExoString sIcon;
    sIcon.Format("i%s%s", m_pCreature->GetClientStats()->GetRace() == RACIAL_TYPE_DROID ? "d" : "",
                 g_aEquipSlots[nSlot].m_sIcon);
    return sIcon;
}

// FUNCTION: SWKOTOR 0x006B56B0
void CSWGuiInGameEquip::SetDescription(const CExoString& sText)
{
    m_label2318.m_cText.SetText(sText);
    m_description.ClearControls();
    CSWGuiExtent extent = m_label2318.m_extent;
    if (sText.GetLength() > 0) {
        extent.height = m_label2318.m_cTextBox.GetHeight();
    } else {
        extent.height = m_label2318.m_cTextBox.GetLineHeight();
    }
    m_label2318.SetExtent(extent);
    CSWGuiControl* pControl = &m_label2318;
    m_description.AddControls(&pControl, 1, 0, 0, 0);
    m_description.SetActiveControl(&m_label2318, 0);
}

inline CSWBaseItem* GetItemBaseItem(CSWSItem* pItem)
{
    return g_pRules->GetBaseItemArray()->GetBaseItem(pItem->GetBaseItemType());
}

// FUNCTION: SWKOTOR 0x006B5760 SYMBOL: ?EquipItem@CSWGuiInGameEquip@@QAEHKKH@Z
int CSWGuiInGameEquip::EquipItem(unsigned long oidItem, unsigned long nSlot, int bClearActions)
{
    CGameObject* pObject = 0;
    unsigned long oidServer = g_pAppManager->GetServerExoApp()->ClientToServerObjectID(oidItem);
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    if (pServer->GetObjectArray()->GetGameObject(oidServer, &pObject) != GAME_OBJECT_VALID ||
        !pObject->AsSWSItem()) {
        return 0;
    }
    CSWSCreature* pCreature = m_pCreature->AsServerCreature();
    if (!pCreature) {
        return 0;
    }
    if (!(GetItemBaseItem(pObject->AsSWSItem())->m_nEquipableSlots & nSlot)) {
        return 0;
    }
    if (bClearActions) {
        pCreature->SetCommandable(1);
        pCreature->m_pCombatRound->RemoveAllActions();
    }
    pCreature->AddEquipItemActions(pObject->AsSWSItem(), nSlot, 0, 1, 1, 0, bClearActions == 0);
    if (pCreature->m_bCombatState && pCreature->m_pCombatRound->m_oidAttackTarget != 0x7f000000) {
        pCreature->AddAttackActions(pCreature->m_pCombatRound->m_oidAttackTarget, 0, 0, 0, 0, 0, 0, 1, 0x2719,
                                    0x5dc, 1, 0, 0, 4, 0);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x006B5890 SYMBOL: ?EquipItem@CSWGuiInGameEquip@@QAEHPAVCSWSItem@@KH@Z
int CSWGuiInGameEquip::EquipItem(CSWSItem* pCopy, unsigned long nSlot, int bClearActions)
{
    CSWSCreature* pCreature = m_pCreature->AsServerCreature();
    if (pCreature) {
        CItemRepository* pRepository = pCreature->GetItemRepository(1);
        for (unsigned long i = 0; i < pRepository->m_nItems; i++) {
            CSWSItem* pItem = pRepository->GetItemAt(i);
            if (pCopy->CompareItem(pItem)) {
                unsigned long oidItem = pItem->m_idSelf;
                return EquipItem(oidItem, nSlot, bClearActions);
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006B5910
void CSWGuiInGameEquip::UnequipItem(unsigned long oidItem, int bClearActions)
{
    CSWSItem* pItem = g_pAppManager->m_pServerExoApp->GetItemByGameObjectID(
        g_pAppManager->m_pServerExoApp->ClientToServerObjectID(oidItem));
    if (pItem) {
        CSWSCreature* pCreature = m_pCreature->AsServerCreature();
        if (pCreature) {
            if (bClearActions) {
                pCreature->SetCommandable(1);
                pCreature->m_pCombatRound->RemoveAllActions();
            }
            pCreature->AddUnequipActions(pItem, 0x7f000000, 0, 1, 1);
            CSWCItem* pClientItem = g_pAppManager->m_pClientExoApp->GetItemByGameObjectID(oidItem);
            if (pClientItem) {
                pClientItem->SetPowered(0, 0, 0, 0);
            }
            if (pCreature->m_bCombatState && pCreature->m_pCombatRound->m_oidAttackTarget != 0x7f000000) {
                pCreature->AddAttackActions(pCreature->m_pCombatRound->m_oidAttackTarget, 0, 0, 0, 0, 0, 0,
                                            1, 0x2719, 0x5dc, 1, 0, 0, 4, 0);
            }
        }
    }
}

inline void SetControlVisible(CSWGuiControl* pControl, int bVisible) { pControl->m_bVisible = bVisible; }

// FUNCTION: SWKOTOR 0x006B59F0
void CSWGuiInGameEquip::ShowCantEquipMessage(int bShow, int nStrRef)
{
    int bHide = bShow == 0;
    SetControlVisible(&m_label1b98, bHide);
    SetControlVisible(&m_label1e18, bHide);
    SetControlVisible(&m_label2098, bHide);
    SetControlVisible(&m_label1cd8, bHide);
    SetControlVisible(&m_label1f58, bHide);
    SetControlVisible(&m_label21d8, bHide);
    SetControlVisible(&m_label2598, bHide);
    SetControlVisible(&m_label26d8, bHide);
    SetControlVisible(&m_label2a98, bHide);
    SetControlVisible(&m_label2bd8, bHide);
    if (bShow) {
        m_label4130.m_cText.SetText(g_pAppManager->GetClientExoApp()->GetGUIString(nStrRef));
    }
    SetControlVisible(&m_label4130, bShow);
    (&m_button3698)->SetEnabled(bHide);
    m_button3698.m_cTextParams.SetColor(bShow ? g_vKeyMapLockedTextColor : g_vDifficultyExitColor);
}

// FUNCTION: SWKOTOR 0x006B65E0
CSWSItem* CSWGuiInGameEquip::CopyItem(unsigned long oidItem)
{
    CSWSItem* pCopy = 0;
    CGameObject* pObject = 0;
    unsigned long oidServer = g_pAppManager->GetServerExoApp()->ClientToServerObjectID(oidItem);
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    if (pServer->GetObjectArray()->GetGameObject(oidServer, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSItem()) {
        pCopy = new CSWSItem(0x7f000000);
        pCopy->CopyItem(pObject->AsSWSItem());
    }
    return pCopy;
}

// FUNCTION: SWKOTOR 0x006B66A0
int CSWGuiInGameEquip::ItemExists(unsigned long oidItem)
{
    CGameObject* pObject;
    unsigned long oidServer = g_pAppManager->GetServerExoApp()->ClientToServerObjectID(oidItem);
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    if (pServer->GetObjectArray()->GetGameObject(oidServer, &pObject) == GAME_OBJECT_VALID) {
        return pObject->AsSWSItem() != 0;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006B7590
void CSWGuiInGameEquip::SetCharacter(CSWCCreature* pCreature)
{
    if (pCreature != m_pCreature) {
        m_pCreature = pCreature;
        if (pCreature) {
            for (int i = 0; i < 9; i++) {
                m_slotLabels[i].SetFillImage(GetIcon(i));
            }
        } else {
            m_label2458.SetFillImage("");
        }
    }
    if (m_pCreature) {
        m_label2458.SetFillImage(((CSWCCreaturePortraitVirtuals*)m_pCreature)->GetPortrait(1));
    }
}

// FUNCTION: SWKOTOR 0x006B7E30
void CSWGuiInGameEquip::CloseDescription()
{
    ShowDescription(0);
    m_nEquipFlags &= ~1UL;
    SetActiveControl(m_pSavedControl, 0);
    OnEnterSlot(m_pControl1c);
    m_label2f98.m_cText.SetText("");
    m_items.SetEnabled(0);
}

// FUNCTION: SWKOTOR 0x006B91C0
CSWGuiInGameItemEntry* CSWGuiInGameEquip::AddItemEntryToList(int nIndex)
{
    if (nIndex == m_entries.m_nCount) {
        CSWGuiInGameItemEntry* pEntry = new CSWGuiInGameItemEntry;
        m_entries.Add(pEntry);
        CSWGuiButton* pProto = (CSWGuiButton*)m_items.m_pProtoItem;
        pEntry->Initialize((CSWGuiText*)&pProto->m_cTextBox, &pProto->GetBorder(), &pProto->GetHilight(),
                           m_items.m_nAreaWidth - m_items.m_nPadding * 2, 1);
        pEntry->AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiInGameEquip::OnItemSelected);
        pEntry->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiInGameEquip::OnOKPressed);
        pEntry->m_cBorderParams.m_fAlpha = 0.0f;
        return pEntry;
    }
    CSWGuiInGameItemEntry* pEntry = m_entries.m_pArray[nIndex];
    pEntry->m_cBorderParams.m_fAlpha = 0.0f;
    return pEntry;
}

// FUNCTION: SWKOTOR 0x006B4C90
int CSWGuiInGameEquip::GetSlotItemIdIndex(int nSlotMask)
{
    switch (nSlotMask) {
    case 0x20:
        return 0;
    case 0x10:
        return 1;
    case 0x01:
        return 2;
    case 0x80:
        return 3;
    case 0x100:
        return 4;
    case 0x02:
        return 5;
    case 0x08:
        return 6;
    case 0x200:
        return 7;
    case 0x400:
        return 8;
    }
    return 10;
}

// FUNCTION: SWKOTOR 0x006B9160
void CSWGuiInGameEquip::OnOKPressed(CSWGuiControl* pControl)
{
    if (pControl->m_n4c && (m_nEquipFlags & 1)) {
        m_n42ac = 0;
        m_n42b4 = 0;
        if (m_pControl42b0) {
            delete m_pControl42b0;
            m_pControl42b0 = 0;
        }
        if (m_pControl42b8) {
            delete m_pControl42b8;
            m_pControl42b8 = 0;
        }
        CloseDescription();
    }
}

// FUNCTION: SWKOTOR 0x006B7680
void CSWGuiInGameEquip::ShowDescription(int bShow)
{
    ShowCantEquipMessage(0, -1);
    UpdatePortraits(bShow == 0);
    if (bShow) {
        ShowControl(&m_description);
        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
        m_button385c.m_cTextParams.SetText(pClient->GetGUIString(0x62d));
        HideControl(&m_slots[0]);
        HideControl(&m_slotLabels[0]);
        HideControl(&m_slots[1]);
        HideControl(&m_slotLabels[1]);
        HideControl(&m_slots[2]);
        HideControl(&m_slotLabels[2]);
        HideControl(&m_slots[3]);
        HideControl(&m_slotLabels[3]);
        HideControl(&m_slots[4]);
        HideControl(&m_slotLabels[4]);
        HideControl(&m_slots[5]);
        HideControl(&m_slotLabels[5]);
        HideControl(&m_slots[6]);
        HideControl(&m_slotLabels[6]);
        HideControl(&m_slots[7]);
        HideControl(&m_slotLabels[7]);
        HideControl(&m_slots[8]);
        HideControl(&m_slotLabels[8]);
        HideControl(&m_label2818);
        HideControl(&m_label2958);
        HideControl(&m_label2458);
        HideControl(&m_label2d18);
        ShowControl(&m_button3698);
        for (int i = 0; i < m_items.m_nControls; i++) {
            ((CSWGuiButton*)m_items.GetControl(i)->IsSelectable())->m_cBorderParams.m_fAlpha = 1.0f;
        }
    } else {
        HideControl(&m_description);
        m_button385c.m_cTextParams.SetText(g_pAppManager->GetClientExoApp()->GetGUIString(0x62e));
        ShowControl(&m_slots[0]);
        ShowControl(&m_slotLabels[0]);
        ShowControl(&m_slots[1]);
        ShowControl(&m_slotLabels[1]);
        ShowControl(&m_slots[2]);
        ShowControl(&m_slotLabels[2]);
        ShowControl(&m_slots[3]);
        ShowControl(&m_slotLabels[3]);
        ShowControl(&m_slots[4]);
        ShowControl(&m_slotLabels[4]);
        ShowControl(&m_slots[5]);
        ShowControl(&m_slotLabels[5]);
        ShowControl(&m_slots[6]);
        ShowControl(&m_slotLabels[6]);
        ShowControl(&m_slots[7]);
        ShowControl(&m_slotLabels[7]);
        ShowControl(&m_slots[8]);
        ShowControl(&m_slotLabels[8]);
        ShowControl(&m_label2818);
        ShowControl(&m_label2958);
        ShowControl(&m_label2458);
        ShowControl(&m_label2d18);
        HideControl(&m_button3698);
        for (int i = 0; i < m_items.m_nControls; i++) {
            ((CSWGuiButton*)m_items.GetControl(i)->IsSelectable())->m_cBorderParams.m_fAlpha = 0.0f;
        }
    }
}

// FUNCTION: SWKOTOR 0x006B8E10
void CSWGuiInGameEquip::OnPanelRemoved()
{
    if (m_nEquipFlags & 1) {
        CloseDescription();
    }
    m_items.ClearControls();
    for (int i = 0; i < m_entries.m_nCount; i++) {
        if (m_entries.m_pArray[i]) {
            delete m_entries.m_pArray[i];
        }
        m_entries.m_pArray[i] = 0;
    }
    m_entries.SetSize(0);
    m_nCompareSlot = 0;
    m_oidCompareItem = 0x7f000000;
    CSWGuiPanel::OnPanelRemoved();
}

// FUNCTION: SWKOTOR 0x006B8EB0
void CSWGuiInGameEquip::OnSlotSelected(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        if (m_pCreature->m_pArea->GetServerArea()->m_bNoEquipChange) {
            CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
            pBox->SetAllowCancel(0);
            pBox->SetCloseCallback(0, 0, 0);
            pBox->SetMessageStrRef(0xa56a);
            m_pManager->AddPanel(pBox, 1, 1);
            return;
        }
        if (m_nSlot == 2 && m_pCreature->AsServerCreature()->m_bCombatState == 1 &&
            m_pCreature->AsServerCreature()->m_nAC0 == 1) {
            CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
            pBox->SetAllowCancel(0);
            pBox->SetCloseCallback(0, 0, 0);
            pBox->SetMessageStrRef(0x5e2);
            m_pManager->AddPanel(pBox, 1, 1);
            return;
        }
        if (m_nSlot == 0x20 && m_oidEquipped[1] != 0x7f000000) {
            CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
            CSWSItem* pItem = pServer->GetItemByGameObjectID(pServer->ClientToServerObjectID(m_oidEquipped[1]));
            if (pItem && ((CSWItem*)pItem)->GetBaseItem()->m_n1B == 4) {
                CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
                pBox->SetAllowCancel(0);
                pBox->SetCloseCallback(0, 0, 0);
                pBox->SetMessageStrRef(0xa568);
                m_pManager->AddPanel(pBox, 1, 1);
                return;
            }
        }
        if (m_items.m_nControls == 1) {
            CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
            pBox->SetAllowCancel(0);
            pBox->SetCloseCallback(0, 0, 0);
            pBox->SetMessageStrRef(0xa569);
            m_pManager->AddPanel(pBox, 1, 1);
            return;
        }
        m_pSavedControl = pControl;
        ShowDescription(1);
        SetActiveControl(&m_items, 0);
        if (m_oidEquipped[pControl->m_n58] != 0x7f000000) {
            m_items.SetSelectedControl(1, 0);
        }
        m_nEquipFlags |= 1;
        m_label2f98.m_cText.SetText(g_pAppManager->m_pClientExoApp->GetGUIString(0x950a));
        g_pAppManager->m_pClientExoApp->ShowTutorialWindow(0xb, 0x7f000000, 0x7f000000, 0);
        (&m_items)->SetEnabled(1);
    }
}

// FUNCTION: SWKOTOR 0x006BA820
void CSWGuiInGameEquip::OnChangeCharacter(CSWGuiControl* pControl)
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
                if (m_button3f6c.m_bVisible) {
                    if (pControl == &m_button3a20) {
                        m_button3a20.m_n58 = m_button3be4.m_n58;
                    }
                } else {
                    m_button3a20.m_n58 = 0xfffffffe;
                }
                m_nSelected = nNPC;
                SetCharacter(pCreature);
            }
        } else {
            if (pControl == &m_button3be4) {
                nMember = 2;
            }
            if (!g_pAppManager->m_pClientExoApp->ChangeCharacterToNextLivingPartyMember(0, 0, nMember)) {
                return;
            }
            m_pManager->PlayGuiSound(1);
            SetCharacter(g_pAppManager->m_pClientExoApp->GetParty()->GetPlayerCharacter());
        }
        for (int i = 0; i < 9; i++) {
            if (g_aEquipSlots[i].m_nSlotMask == m_nSlot) {
                SetActiveControl(&m_slots[i], 0);
                break;
            }
        }
        OnEnterSlot(m_pControl1c);
        UpdatePortraits(1);
    }
}
