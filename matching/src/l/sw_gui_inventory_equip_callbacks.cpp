#include "l/sw_gui_inventory_equip.h"


// FUNCTION: SWKOTOR 0x006BB510
void CSWGuiInGameEquip::Draw(float fDelta)
{
    UpdateInventory();
    CSWGuiPanel::Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x006B8770
void CSWGuiContainer::Update(int)
{
    if (m_nContainerFlags & 4) { m_nContainerFlags &= ~4UL; return; }
    if ((m_nContainerFlags & 2) && !(m_nContainerFlags & 1)) {
        SetupGiveMode(1);
        m_nContainerFlags &= ~2UL;
    }
}

// The following constructors only emit vtables and wrappers; no constructor
// reconstruction is claimed. The real regular destructors remain external.
// FUNCTION: SWKOTOR 0x006B4B90 SYMBOL: ??_GCSWGuiInGameInventory@@UAEPAXI@Z
CSWGuiInGameInventory::CSWGuiInGameInventory() {}
// FUNCTION: SWKOTOR 0x006B9450 SYMBOL: ??_GCSWGuiInGameEquip@@UAEPAXI@Z
CSWGuiInGameEquip::CSWGuiInGameEquip() {}
// FUNCTION: SWKOTOR 0x006B8030 SYMBOL: ??_GCSWGuiContainer@@UAEPAXI@Z
CSWGuiContainer::CSWGuiContainer() {}

// FUNCTION: SWKOTOR 0x006BB530
void CSWGuiInGameEquip::OnPanelAdded()
{
    m_nSelected = 0xff;
    m_button3a20.m_n58 = 0xfffffffe;
    m_button3be4.m_n58 = 0xfffffffe;
    SetCharacter(g_pAppManager->m_pClientExoApp->GetParty()->GetPlayerCharacter());
    UpdatePortraits(1);
    OnEnterSlot(m_pControl1c);
    CSWGuiPanel::OnPanelAdded();
}
// FUNCTION: SWKOTOR 0x006B4C30
void CSWGuiInGameInventory::OnPanelAdded()
{
    m_nSelected = 0xff;
    m_button16b0.m_n58 = 0xfffffffe;
    m_button1874.m_n58 = 0xfffffffe;
    UpdatePortraits();
    PopulateItemListBox();
    m_button1164.HandleMouseOver(0);
    CSWGuiPanel::OnPanelAdded();
    g_pAppManager->m_pClientExoApp->ShowTutorialWindow(20, 0x7f000000, 0x7f000000, 0);
}
// FUNCTION: SWKOTOR 0x006B4FE0
int CSWGuiInGameItemEntry::GetNumItems()
{
    int count = 1;
    CExoString text(m_cTextParams.m_sText);
    if (!text.IsEmpty()) count = text.AsINT();
    return count;
}
