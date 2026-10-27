// CSWGuiStore constructor (layout "store") and destructor.
#include "l/sw_gui_store.h"

// FUNCTION: SWKOTOR 0x006C1820 SYMBOL: ??_GCSWGuiStore@@UAEPAXI@Z
// Emitted with the destructor below.

// FUNCTION: SWKOTOR 0x006C1470
CSWGuiStore::~CSWGuiStore()
{
    DeleteItemEntries();
}

// FUNCTION: SWKOTOR 0x006C1C00
CSWGuiStore::CSWGuiStore(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    CSWGuiListBox* pShopItems = &m_lbShopItems;
    CSWGuiListBox* pInvItems = &m_lbInvItems;
    m_n226c = 0;
    StartLoadFromLayout("store");
    InitControl(&m_lblBuySell, "LBL_BUYSELL", 1);
    InitControl(pShopItems, "LB_SHOPITEMS", 1);
    InitControl(pInvItems, "LB_INVITEMS", 1);
    InitControl(&m_lblCredits, "LBL_CREDITS", 1);
    InitControl(&m_lblCreditsValue, "LBL_CREDITS_VALUE", 1);
    InitControl(&m_lbDescription, "LB_DESCRIPTION", 1);
    m_lbDescription.m_n2c1 = 1;
    InitControl(&m_lblCost, "LBL_COST", 1);
    InitControl(&m_lblCostValue, "LBL_COST_VALUE", 1);
    InitControl(&m_lblStock, "LBL_STOCK", 1);
    InitControl(&m_lblStockValue, "LBL_STOCK_VALUE", 1);
    InitControl(&m_btnCancel, "BTN_Cancel", 1);
    InitControl(&m_btnExamine, "BTN_Examine", 1);
    InitControl(&m_btnAccept, "BTN_Accept", 1);
    UnloadResource();
    m_lblDescription.CopyStyle(m_lbDescription.m_pProtoItem->m_extent, m_lbDescription.m_pProtoItem, 1.0f);
    pShopItems->m_nListFlags |= 0x20;
    pInvItems->m_nListFlags |= 0x20;
    CSWGuiPanel::SetActiveControl(pShopItems, 0);
    pInvItems->m_nFlags &= ~2;
    m_n64 = 0;
    m_n2274 = 0;
    m_nGold = 0;
    CSWGuiButton* pButton = &m_btnCancel;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    pButton = &m_btnExamine;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiStore::OnControlXButton);
    m_btnAccept.m_nFlags &= ~4;
    pInvItems->SetListFlag0(1);
    pShopItems->SetListFlag0(1);
    m_nResolution = 1;
}
