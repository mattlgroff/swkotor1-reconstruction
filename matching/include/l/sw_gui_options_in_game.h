// Worker L matching headers: the in-game options menu CSWGuiOptionsInGame
// (layout "optionsingame", vtable 0x00755DE0, 0x006AAAD0-0x006AB7B0).
#ifndef L_SW_GUI_OPTIONS_IN_GAME_H
#define L_SW_GUI_OPTIONS_IN_GAME_H

#include "l/sw_gui_tutorial_box.h"

// vtable 0x0073E8C8 family; ctor 0x0041C0F0, dtor 0x00419DA0 (0x1c4 bytes).
class CSWGuiButton : public CSWGuiControl {
public:
    CSWGuiButton();                                 // 0x0041C0F0
    ~CSWGuiButton();                                // 0x00419DA0
    virtual void CopyStyle(const CSWGuiExtent& extent, CSWGuiButton* pFrom);   // 0x98
    char pad_0x5c[0x24];
    CSWGuiBorderParams m_cBorderParams;             // 0x80 (m_cBorder + 0x14)
    char pad_0xe0[0x14];
    CSWGuiBorderParams m_cHilightParams;            // 0xf4 (m_cHilight + 0x14)
    CSWGuiLabelText m_cTextBox;                     // 0x154 (m_cText)
    CSWGuiTextParams m_cTextParams;                 // 0x16c (m_cText + 0x18)
};

// Toggle control family used by the options checkboxes (base size 0x2B4).
class CSWGuiButtonToggle : public CSWGuiButton {
public:
    CSWGuiButtonToggle();                           // 0x0041CA30
    virtual ~CSWGuiButtonToggle();                  // 0x0041CAD0
    char pad_0x1c4[0xf0];
};

// Inferred CSWGuiOptionsCheckbox, vtable 0x00758258, size 0x2B4.
#ifdef L_SWGUI_MATCHING_DESTRUCTOR_TU
class __declspec(novtable) CSWGuiOptionsCheckbox : public CSWGuiButtonToggle {
#else
class CSWGuiOptionsCheckbox : public CSWGuiButtonToggle {
#endif
public:
    CSWGuiOptionsCheckbox();                        // 0x006E00D0
    __declspec(noinline) virtual ~CSWGuiOptionsCheckbox(); // 0x006E00F0
};

// Panels opened from the options menu (constructors only).
class CSWGuiSaveLoad : public CSWGuiPanel {
public:
    CSWGuiSaveLoad(CSWGuiManager* pManager, int bSave, int nArg);   // 0x006CC680
    char pad_0x64[0x1160 - 0x64];
};

class CSWGuiOptionsGameplay : public CSWGuiPanel {
public:
    CSWGuiOptionsGameplay(CSWGuiManager* pManager); // 0x006E69A0
    char pad_0x64[0x22a0 - 0x64];
};

class CSWGuiOptionsFeedback : public CSWGuiPanel {
public:
    CSWGuiOptionsFeedback(CSWGuiManager* pManager); // 0x006E2A70
    char pad_0x64[0xc2c - 0x64];
};

#ifdef L_SWGUI_MATCHING_DESTRUCTOR_TU
class __declspec(novtable) CSWGuiOptionsAutoPause : public CSWGuiPanel {
#else
class CSWGuiOptionsAutoPause : public CSWGuiPanel {
#endif
public:
    CSWGuiOptionsAutoPause(CSWGuiManager* pManager);    // 0x006E76A0
    __declspec(noinline) virtual ~CSWGuiOptionsAutoPause(); // 0x006E7A90
    CSWGuiOptionsCheckbox m_aCheckboxes[6];              // 0x64
    CSWGuiLabel m_lblDescription;                        // 0x109C
    CSWGuiLabel m_lblTitle;                              // 0x11DC
    CSWGuiButton m_btnBack;                              // 0x131C
    CSWGuiButton m_btnDefault;                           // 0x14E0
    CSWGuiListBox m_lbDetails;                            // 0x16A4
};

class CSWGuiOptionsGraphics : public CSWGuiPanel {
public:
    CSWGuiOptionsGraphics(CSWGuiManager* pManager); // 0x006E2E70
    char pad_0x64[0x18c0 - 0x64];
};

class CSWGuiOptionsSound : public CSWGuiPanel {
public:
    CSWGuiOptionsSound(CSWGuiManager* pManager);    // 0x006E3550
    char pad_0x64[0x16f0 - 0x64];
};

typedef void (CSWGuiPanel::*PFN_GUI_CALLBACK)(CSWGuiControl* pControl);

// vtable 0x00755DE0; deleting dtor 0x006AB790, implicit dtor 0x006AB7B0.
class CSWGuiOptionsInGame : public CSWGuiPanel {
public:
    CSWGuiOptionsInGame(CSWGuiManager* pManager);   // 0x006AB2F0
    void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006AAEC0)
    void OnPanelAdded();                            // 0x48 (0x006AB140)
    void OnPanelRemoved();                          // 0x4c (0x006AAAD0)

    void OnLoadGame(CSWGuiControl* pControl);       // 0x006AAAF0
    void OnSaveGame(CSWGuiControl* pControl);       // 0x006AAB80
    void OnGameplay(CSWGuiControl* pControl);       // 0x006AAC10
    void OnFeedback(CSWGuiControl* pControl);       // 0x006AAC90
    void OnAutoPause(CSWGuiControl* pControl);      // 0x006AAD10
    void OnGraphics(CSWGuiControl* pControl);       // 0x006AAD90
    void OnSound(CSWGuiControl* pControl);          // 0x006AAE10
    void OnQuitConfirmed(CSWGuiControl* pControl);  // 0x006AAE90
    void OnQuit(CSWGuiControl* pControl);           // 0x006AB1B0
    void OnButtonSelected(CSWGuiControl* pControl); // 0x006AB220
    void SetDescription(int nStrRef);               // 0x006AB000
    int GetButtonIndex(CSWGuiControl* pControl)
    {
        for (int i = 0; i < 8; i++) {
            if (pControl == &m_aButtons[i]) {
                return i;
            }
        }
        return -1;
    }

    CSWGuiButton m_aButtons[8];                     // 0x64 (LOADGAME..QUIT)
    CSWGuiButton m_btnE84;                          // 0xe84
    CSWGuiLabel m_lblTitle;                         // 0x1048
    CSWGuiLabel m_lblDesc;                          // 0x1188
    CSWGuiLabel m_lbl12C8;                          // 0x12c8
    CSWGuiLabel m_lbl1408;                          // 0x1408
    CSWGuiLabel m_lbl1548;                          // 0x1548
    CSWGuiLabel m_lbl1688;                          // 0x1688
    CSWGuiLabel m_lblScroll;                        // 0x17c8
    CSWGuiListBox m_lbDesc;                         // 0x1908
    CSWGuiButton m_btnExit;                         // 0x1be8
};

#endif
