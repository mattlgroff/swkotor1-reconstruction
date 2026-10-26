// CSWGuiUpgrade (swguiupgrade.o): the workbench upgrade panel's lifecycle
// and simple handlers.

#include "l/sw_gui_upgrade.h"

// FUNCTION: SWKOTOR 0x006C4AD0
CSWGuiUpgrade::~CSWGuiUpgrade()
{
    m_lbItems.ClearControls();
    m_lbDesc.ClearControls();
    if (m_pUpgrade2DA) {
        delete m_pUpgrade2DA;
        m_pUpgrade2DA = 0;
    }
    if (m_pCrystals2DA) {
        delete m_pCrystals2DA;
        m_pCrystals2DA = 0;
    }
}

// FUNCTION: SWKOTOR 0x006C6100 SYMBOL: ??_GCSWGuiUpgrade@@UAEPAXI@Z
// Emitted with the destructor above.

// FUNCTION: SWKOTOR 0x006C25F0
void CSWGuiUpgrade::OnPanelRemoved()
{
    CSWSItem* pOriginal = m_pOriginalItem;
    m_nType = 0;
    m_pItem = 0;
    if (pOriginal) {
        delete pOriginal;
        m_pOriginalItem = 0;
    }
    for (int i = 0; i < 4; i++) {
        if (m_aSlotItems[i]) {
            delete m_aSlotItems[i];
            m_aSlotItems[i] = 0;
        }
    }
    m_lstRemoved.m_nCount = 0;
    m_lstAdded.m_nCount = 0;
    CSWGuiPanel::OnPanelRemoved();
}

// FUNCTION: SWKOTOR 0x006C32E0
void CSWGuiUpgrade::SetDescription(const CExoString& sText)
{
    m_lblDescription.m_cText.SetText(sText);
    CSWGuiListBox* pListBox = m_nType != 1 ? &m_lbDesc : &m_lbDescLS;
    pListBox->ClearControls();
    CSWGuiExtent extent = m_lblDescription.m_extent;
    if (sText.GetLength() > 0) {
        extent.height = m_lblDescription.m_cTextBox.GetHeight();
    } else {
        extent.height = m_lblDescription.m_cTextBox.GetLineHeight();
    }
    m_lblDescription.SetExtent(extent);
    CSWGuiControl* pControl = &m_lblDescription;
    pListBox->AddControls(&pControl, 1, 0, 0, 0);
    pListBox->SetActiveControl(&m_lblDescription, 0);
}

// FUNCTION: SWKOTOR 0x006C4D30
void CSWGuiUpgrade::CloseItems()
{
    ShowItems(0);
    m_bItemsOpen = 0;
    SetActiveControl(m_pSelectedControl, 0);
    UpdateItems();
    OnEnterSlot(m_pControl1c);
}

// FUNCTION: SWKOTOR 0x006C6190
void CSWGuiUpgrade::OnAssemble(CSWGuiControl* pControl)
{
    if (!pControl->m_n4c) {
        return;
    }
    if (pControl->m_n48 == 0x2d) {
        m_pManager->PlayGuiSound(pControl->m_nSound);
    }
    m_pItemSelect->FinishUpgrading();
    HidePanel();
    m_pManager->PopModalPanel();
}

// FUNCTION: SWKOTOR 0x006C6A80
void CSWGuiUpgrade::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x28:
        case 0x2e:
            m_pManager->PlayGuiSound(0);
            if (m_bItemsOpen) {
                CloseItems();
            } else {
                CancelPanel();
            }
            break;
        case 0x3a:
            if (m_nType == 1) {
                m_lbDescLS.HandleInputEvent(0x32, 1);
            } else {
                m_lbDesc.HandleInputEvent(0x32, 1);
            }
            break;
        case 0x39:
            if (m_nType == 1) {
                m_lbDescLS.HandleInputEvent(0x31, 1);
            } else {
                m_lbDesc.HandleInputEvent(0x31, 1);
            }
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006C61F0
void CSWGuiUpgrade::CancelPanel()
{
    if (m_pItem) {
        delete m_pItem;
    }
    m_pItem = m_pOriginalItem;
    m_pOriginalItem = 0;
    CItemRepository* pRepository = g_pAppManager->m_pServerExoApp->GetPartyTable()->GetPartyItems();
    CSWSItem* pItem;
    int i;
    for (i = 0; i < m_lstAdded.m_nCount; i++) {
        pItem = m_lstAdded.m_pArray[i];
        for (int j = 0; j < 4; j++) {
            if (pItem == m_aSlotItems[j]) {
                m_aSlotItems[j] = 0;
                break;
            }
        }
        pRepository->AddItem(&pItem, 1, 0, 0);
    }
    for (i = 0; i < m_lstRemoved.m_nCount; i++) {
        pItem = m_lstRemoved.m_pArray[i];
        if (pItem->m_nStackSize > 1) {
            pItem->m_nStackSize--;
        } else {
            pRepository->RemoveItem(pItem);
            if (pItem) {
                delete pItem;
            }
        }
    }
    m_pItemSelect->FinishUpgrading();
    HidePanel();
    m_pManager->PopModalPanel();
}
