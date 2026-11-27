// Small CSWGuiStore lifecycle leaves. Field offsets and call order follow the
// original executable and the linear-sweep reconstruction in panels_menus.cpp.

#include "l/sw_gui_store.h"

// FUNCTION: SWKOTOR 0x006C1330
void CSWGuiStore::DeleteItemEntries()
{
    m_lbInvItems.ClearControls();
    m_lbShopItems.ClearControls();

    CExoArrayList<CSWGuiStoreItemEntry*>& lstStore = m_lstStoreEntries;
    for (int i = 0; i < lstStore.m_nCount; ++i) {
        delete lstStore.m_pArray[i];
        lstStore.m_pArray[i] = 0;
    }
    lstStore.m_nCount = 0;

    CExoArrayList<CSWGuiStoreItemEntry*>& lstInv = m_lstInvEntries;
    for (int j = 0; j < lstInv.m_nCount; ++j) {
        delete lstInv.m_pArray[j];
        lstInv.m_pArray[j] = 0;
    }
    lstInv.m_nCount = 0;
}

// FUNCTION: SWKOTOR 0x006C1650
void CSWGuiStore::OnPanelRemoved()
{
    DeleteItemEntries();
    CSWGuiPanel::OnPanelRemoved();
}

// FUNCTION: SWKOTOR 0x006C2170
void CSWGuiStore::OnPanelAdded()
{
    InitializeStoreType();
    PopulateStore();
    CSWGuiPanel::OnPanelAdded();
    ShowBuyGUI();
}
