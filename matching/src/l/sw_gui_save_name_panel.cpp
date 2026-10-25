// Save-name dialog callbacks and the save-game editbox destructor thunk.
// Layout/call evidence: the original SaveNamePanel is 0x690 bytes, owns its
// SaveLoad object at +0x64, and embeds the editbox value at +0x548.
#include "d/gui_control.h"

class CSWGuiSaveLoad {
public:
    void WriteGame(const CExoString& name);
};

class CSWGuiSaveNamePanel : public CSWGuiPanel {
public:
    void HandleOkButton(CSWGuiControl* pControl);
};

// FUNCTION: SWKOTOR 0x006C9D50
void CSWGuiSaveNamePanel::HandleOkButton(CSWGuiControl*)
{
    {
        CExoString name(*(CExoString*)((char*)this + 0x548));
        CSWGuiSaveLoad* saveLoad = (CSWGuiSaveLoad*)*(void**)((char*)this + 0x64);
        saveLoad->WriteGame(name);
    }
    unsigned long flags = *(unsigned long*)((char*)this + 0x44);
    CSWGuiManager* manager = m_pManager;
    *(unsigned long*)((char*)this + 0x44) = (flags & 0xfffffcffu) | 0x400u;
    manager->PopModalPanel();
}

// Inferred subclass name from the savegame editbox vtable at 0x007575B0.
class CSWGuiSaveGameEditBox : public CSWGuiEditbox {
public:
    void DestroyBase();
};

// FUNCTION: SWKOTOR 0x006C9DD0
void CSWGuiSaveGameEditBox::DestroyBase()
{
    CSWGuiEditbox::~CSWGuiEditbox();
}
