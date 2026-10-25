// Small CSWGuiStore lifecycle leaves. Field offsets and call order follow the
// original executable and the linear-sweep reconstruction in panels_menus.cpp.

#include "l/sw_gui_dialog.h"

class CSWGuiDeletingEntry {
public:
    virtual void* Destroy(unsigned int flags) = 0;
};

class CSWGuiStore : public CSWGuiPanel {
public:
    virtual void OnPanelAdded();
    virtual void OnPanelRemoved();
    void DeleteItemEntries();
    void InitializeStoreType();
    void PopulateStore();
    void ShowBuyGUI();
};

// FUNCTION: SWKOTOR 0x006C1330
void CSWGuiStore::DeleteItemEntries()
{
    ((CSWGuiListBox*)((char*)this + 0x1760))->ClearControls();
    ((CSWGuiListBox*)((char*)this + 0x1480))->ClearControls();

    for (int i = 0; i < *(volatile int*)((char*)this + 0x6c); ++i) {
        void* entry = ((void**) *(void***)((char*)this + 0x68))[i];
        if (entry != 0) {
            ((CSWGuiDeletingEntry*)entry)->Destroy(1);
        }
        ((void**) *(void***)((char*)this + 0x68))[i] = 0;
    }
    *(volatile int*)((char*)this + 0x6c) = 0;

    for (int j = 0; j < *(volatile int*)((char*)this + 0x78); ++j) {
        void* entry = ((void**) *(void***)((char*)this + 0x74))[j];
        if (entry != 0) {
            ((CSWGuiDeletingEntry*)entry)->Destroy(1);
        }
        ((void**) *(void***)((char*)this + 0x74))[j] = 0;
    }
    *(int*)((char*)this + 0x78) = 0;
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
