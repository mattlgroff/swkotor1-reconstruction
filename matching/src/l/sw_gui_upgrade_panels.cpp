// Workbench upgrade selection panels (swguiupgrade.o): CSWGuiUpgradeSelection
// and CSWGuiUpgradeItemSelect lifecycle and small handlers.

#include "l/sw_gui_upgrade.h"

// FUNCTION: SWKOTOR 0x006C4450
CSWGuiUpgradeSelection::~CSWGuiUpgradeSelection()
{
    if (m_pItemSelect) {
        delete m_pItemSelect;
        m_pItemSelect = 0;
    }
}

// FUNCTION: SWKOTOR 0x006C5B50 SYMBOL: ??_GCSWGuiUpgradeSelection@@UAEPAXI@Z
// Emitted with the destructor above.

// FUNCTION: SWKOTOR 0x006C47A0
CSWGuiUpgradeItemSelect::~CSWGuiUpgradeItemSelect()
{
    m_lbItems.ClearControls();
    m_lbDescription.ClearControls();
    if (m_pUpgrade) {
        delete m_pUpgrade;
        m_pUpgrade = 0;
    }
}

// FUNCTION: SWKOTOR 0x006C5B70 SYMBOL: ??_GCSWGuiUpgradeItemSelect@@UAEPAXI@Z
// Emitted with the destructor above.

// FUNCTION: SWKOTOR 0x006C2A70
unsigned long CSWGuiUpgradeItemSelect::GetSelectedOwner()
{
    if (m_pSelectedEntry) {
        return m_pSelectedEntry->m_oidOwner;
    }
    return 0x7f000000;
}

// FUNCTION: SWKOTOR 0x006C2A90
unsigned long CSWGuiUpgradeItemSelect::GetSelectedSlot()
{
    if (m_pSelectedEntry) {
        return m_pSelectedEntry->m_nSlot;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006C2AB0
void CSWGuiUpgradeSelection::GetUpgradableItems(CSWSCreature* pCreature, int* pAvailable, C2DA* p2DA)
{
    for (int i = 0; i < 18; i++) {
        CSWSItem* pItem = pCreature->GetInventory()->GetItemInSlot(1 << i);
        if (pItem) {
            int nType = pItem->GetUpgradableType(p2DA);
            if (nType > 0) {
                pAvailable[nType - 1] = 1;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006C2B10
void CSWGuiUpgradeSelection::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown && (nEvent == 0x28 || nEvent == 0x2e)) {
        m_pManager->PlayGuiSound(0);
        ((CGuiInGameUpgrade*)g_pAppManager->m_pClientExoApp->GetInGameGui())->HideUpgradeScreen();
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006C2B60
void CSWGuiUpgradeSelection::OnTypeSelected(CSWGuiControl* pControl)
{
    if (!pControl->m_n4c) {
        return;
    }
    if (pControl->m_n48 == 0x2d) {
        m_pManager->PlayGuiSound(0);
    }
    if (pControl == &m_btnUpgradeItems) {
        pControl = m_pEntered;
        if (!pControl) {
            return;
        }
    }
    int nType = pControl->m_n58;
    if (m_aAvailable[nType]) {
        m_pItemSelect->m_nUpgradeType = nType + 1;
        m_pManager->AddPanel(m_pItemSelect, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006C2BD0
void CSWGuiUpgradeSelection::OnEnter(CSWGuiControl* pControl)
{
    int nType = pControl->m_n58;
    if (!pControl->m_n4c) {
        return;
    }
    CSWGuiRadioButton* pEntered = pControl->GetRadioButton();
    if (!pEntered) {
        return;
    }
    if (m_aAvailable[nType]) {
        m_btnUpgradeItems.m_cTextParams.SetColor(g_vDifficultyExitColor);
        m_btnUpgradeItems.m_cBorderParams.m_vColor = g_vUpgradeAvailableColor;
        m_pEntered = pEntered;
    } else {
        m_btnUpgradeItems.m_cTextParams.SetColor(g_vKeyMapLockedTextColor);
        m_btnUpgradeItems.m_cBorderParams.m_vColor = g_vKeyMapLockedColor;
        m_pEntered = 0;
    }
}

// FUNCTION: SWKOTOR 0x006C2C80
void CSWGuiUpgradeItemSelect::SetDescription(const CExoString& sText)
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
}

// FUNCTION: SWKOTOR 0x006C2D30
void CSWGuiUpgradeItemSelect::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x28:
        case 0x2e:
            m_pManager->PlayGuiSound(0);
            SetToDestroy(0);
            m_pManager->PopModalPanel();
            break;
        case 0x3a:
            m_lbDescription.HandleInputEvent(0x32, 1);
            break;
        case 0x39:
            m_lbDescription.HandleInputEvent(0x31, 1);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006C4880
void CSWGuiUpgradeItemSelect::OnControlEntered(CSWGuiControl* pControl)
{
    if (pControl->IsActivated()) {
        unsigned long oidItem = ((CSWUpgradeItemEntry*)pControl)->m_oidItem;
        if (oidItem != 0x7f000000) {
            CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem);
            if (pItem) {
                SetDescription(pItem->GetPropertyDescription());
            } else {
                SetDescription("");
            }
        } else {
            SetDescription("");
        }
    }
}
