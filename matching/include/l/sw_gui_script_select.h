// Worker L matching headers: the party AI script picker CSWGuiScriptSelect
// (SWGuiScriptSelect.o, layout "ScriptSelect", vtable 0x007590A8, 0xC40
// bytes), opened from the character screen.
#ifndef L_SW_GUI_SCRIPT_SELECT_H
#define L_SW_GUI_SCRIPT_SELECT_H

#include "l/sw_gui_levelup_chargen.h"

// One "aiscripts" row: description strref and AI state.
struct CSWGuiScriptEntry {
    unsigned long m_nDescription;                   // 0x0
    int m_nAIState;                                 // 0x4
};

// The panel's script table: constructor, zeroing destructor and the
// releasing SetSize (Allocate 0x004C4F50 for 8-byte entries).
template <class T>
class CExoArrayListZ {
public:
    T* element;
    int num;
    int array_size;
    CExoArrayListZ()
    {
        num = 0;
        array_size = 0;
        element = 0;
    }
    ~CExoArrayListZ()
    {
        delete[] element;
        element = 0;
    }
    void Allocate(int nSize);                       // 0x004C4F50
    void SetSizeFree(int nSize)
    {
        if (nSize == 0) {
            if (element) {
                delete[] element;
                element = 0;
                array_size = 0;
            }
        } else {
            Allocate(nSize);
        }
        num = nSize;
    }
};

class CSWGuiScriptSelect : public CSWGuiPanel {
public:
    CSWGuiScriptSelect(CSWGuiManager* pManager);    // 0x006EA000
    ~CSWGuiScriptSelect();                          // 0x006E9AC0
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006E9BC0)

    void SetCreatureID(unsigned long oidCreature);  // 0x006E9CA0
    void SetDescription(unsigned long nStrRef);     // 0x006E9D50
    void OnScriptSelected(CSWGuiControl* pControl); // 0x006E9E70 (descriptive)
    CSWGuiControl* CreateOption(unsigned long nStrRef, PFN_GUI_CALLBACK pfnCallback,
                                CSWGuiControl* pProto); // 0x006E9F10 (descriptive)
    void OnEnterOption(CSWGuiControl* pControl);    // 0x006E9FE0 (descriptive)

    CExoArrayListZ<CSWGuiScriptEntry> m_lstScripts; // 0x64
    CSWGuiListBox m_lbStates;                       // 0x70 LST_AIState
    CSWGuiListBox m_lbDesc;                         // 0x350 LB_DESC
    CSWGuiLabel m_lblDesc;                          // 0x630 (description item)
    CSWGuiLabel m_lblTitle;                         // 0x770 LBL_TITLE
    CSWGuiButton m_btnBack;                         // 0x8b0 BTN_Back
    CSWGuiButton m_btnAccept;                       // 0xa74 BTN_Accept
    int m_nC38;                                     // 0xc38 (placeholder name)
    unsigned long m_oidCreature;                    // 0xc3c
};

#endif
