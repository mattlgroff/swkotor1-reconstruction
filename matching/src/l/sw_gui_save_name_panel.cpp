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

    // d/gui_control.h's CSWGuiPanel stops at +0x1c; its flags word is at +0x44.
    char pad_0x1c[0x44 - 0x1c];
    unsigned long m_nFlags;                         // +0x44
    char pad_0x48[0x64 - 0x48];
    CSWGuiSaveLoad* m_pSaveLoad;                    // +0x64
    char pad_0x68[0x548 - 0x68];
    CExoString m_sSaveName;                         // +0x548, the editbox value
};

// FUNCTION: SWKOTOR 0x006C9D50
void CSWGuiSaveNamePanel::HandleOkButton(CSWGuiControl*)
{
    {
        CExoString name(m_sSaveName);
        CSWGuiSaveLoad* saveLoad = m_pSaveLoad;
        saveLoad->WriteGame(name);
    }
    unsigned long flags = m_nFlags;
    CSWGuiManager* manager = m_pManager;
    m_nFlags = (flags & 0xfffffcffu) | 0x400u;
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
