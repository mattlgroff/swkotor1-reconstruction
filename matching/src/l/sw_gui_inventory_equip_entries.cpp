#include "l/sw_gui_inventory_equip.h"

// FUNCTION: SWKOTOR 0x006B66F0
CSWGuiRadioButton::CSWGuiRadioButton() {}

// FUNCTION: SWKOTOR 0x006BE360
CSWGuiRadioButton::~CSWGuiRadioButton() {}

// FUNCTION: SWKOTOR 0x006B7EE0
CSWGuiInGameItemEntry::CSWGuiInGameItemEntry()
{
    m_nItemFlags &= ~7UL;
    m_oidItem = 0x7f000000;
    m_n398 = 0;
}
// FUNCTION: SWKOTOR 0x006B71E0
CSWGuiStoreItemEntry::CSWGuiStoreItemEntry()
{
    m_oidItem = 0x7f000000;
}

// FUNCTION: SWKOTOR 0x006B7F80 SYMBOL: ??_GCSWGuiInGameItemEntry@@UAEPAXI@Z
// Emitted by the real constructor above; its regular destructor is external.

// FUNCTION: SWKOTOR 0x006B87A0 SYMBOL: ??_GCSWGuiStoreItemEntry@@UAEPAXI@Z
// Emitted by the real constructor above; its regular destructor is external.
