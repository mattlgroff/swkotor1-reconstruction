// CSWGuiQuestItem activation path. The original vtable has OnPanelAdded at
// 0x006D2C20, which repopulates the plot-item list before forwarding to panel.
#include "l/sw_gui_dialog.h"

class CSWGuiQuestItem : public CSWGuiPanel {
public:
    __declspec(noinline) virtual ~CSWGuiQuestItem();
    virtual void OnPanelAdded();
    void PopulateItemListBox();
};

// FUNCTION: SWKOTOR 0x006D2C20
void CSWGuiQuestItem::OnPanelAdded()
{
    PopulateItemListBox();
    CSWGuiPanel::OnPanelAdded();
}

// FUNCTION: SWKOTOR 0x006D2C00 SYMBOL: ??_GCSWGuiQuestItem@@UAEPAXI@Z
CSWGuiQuestItem::~CSWGuiQuestItem()
{
}
