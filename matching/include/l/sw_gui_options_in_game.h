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

struct CSWGuiSliderThumbParams {
    char pad_0x0[0x24];
    union {
        unsigned long m_nFlags;
        struct {
            unsigned long m_bHighlighted : 1;
            unsigned long m_nOtherFlags : 31;
        };
    };
    char pad_0x28[4];
    void SetHighlighted(int b) { m_bHighlighted = b; }
};

// Slider control, size 0x1B8; lifecycle calls 0x0041BAA0/0x0041BB40.
class CSWGuiSlider : public CSWGuiControl {
public:
    CSWGuiSlider();
    virtual ~CSWGuiSlider();
    char pad_0x5c[0x18];
    int m_nValue; // 0x74
    char pad_0x78[0x18c - 0x78];
    CSWGuiSliderThumbParams m_cThumbParams; // 0x18c
};

// Options panels; ordered members follow constructor and destructor evidence.
class CSWGuiSaveLoad : public CSWGuiPanel {
public:
    CSWGuiSaveLoad(CSWGuiManager* pManager, int bSave, int nArg);   // 0x006CC680
    char pad_0x64[0x1160 - 0x64];
};

#ifdef L_OPTIONS_FAMILY_NOVTABLE
class __declspec(novtable) CSWGuiInGameGameplay : public CSWGuiPanel {
#else
class CSWGuiInGameGameplay : public CSWGuiPanel {
#endif
public:
    CSWGuiInGameGameplay(CSWGuiManager* pManager); // 0x006E69A0
    __declspec(noinline) virtual ~CSWGuiInGameGameplay(); // 0x006E7130
    virtual void HandleInputEvent(int nEvent, int bDown); // panel slot 0x3c
    void OnDifficultyLeft(CSWGuiControl* pControl);
    void OnEnterDiff(CSWGuiControl* pControl);
    void OnExitDiff(CSWGuiControl* pControl);
    void OnCombatMovement(CSWGuiControl* pControl);
    void OnReverse(CSWGuiControl* pControl);
    void OnInvertCam(CSWGuiControl* pControl);
    void OnAutoSave(CSWGuiControl* pControl);
    void OnAutoLevelUp(CSWGuiControl* pControl);
    void OnKeyMap(CSWGuiControl* pControl);
    void OnMouse(CSWGuiControl* pControl);
    void OnPanelAdded();
    void OnEnterSlot(CSWGuiControl* pControl);
    void OnDefault(CSWGuiControl* pControl);
    void SetUpOptions();
    void SetDescription(unsigned long nStrRef);
    CSWGuiLabel m_control_64; // 0x64
    CSWGuiLabel m_control_1A4; // 0x1A4
    CSWGuiListBox m_control_2E4; // 0x2E4
    CSWGuiButton m_control_5C4; // 0x5C4
    CSWGuiButton m_control_788; // 0x788
    CSWGuiButton m_control_94C; // 0x94C
    CSWGuiButton m_control_B10; // 0xB10
    CSWGuiButton m_control_CD4; // 0xCD4
    CSWGuiButton m_control_E98; // 0xE98
    CSWGuiButton m_control_105C; // 0x105C
    CSWGuiButtonToggle m_control_1220; // 0x1220
    CSWGuiButtonToggle m_control_14D4; // 0x14D4
    CSWGuiButtonToggle m_control_1788; // 0x1788
    CSWGuiButtonToggle m_control_1A3C; // 0x1A3C
    CSWGuiButtonToggle m_control_1CF0; // 0x1CF0
    CSWGuiSlider m_control_1FA4; // 0x1FA4
    CSWGuiLabel m_control_215C; // 0x215C
    void SetDifficultyText(); // 0x006E6520
    int m_nDifficulty; // 0x229c
};

class CSWGuiOptionsFeedback : public CSWGuiPanel {
public:
    CSWGuiOptionsFeedback(CSWGuiManager* pManager); // 0x006E2A70
    __declspec(noinline) virtual ~CSWGuiOptionsFeedback(); // 0x006E0020
    virtual void HandleInputEvent(int nEvent, int bDown); // panel slot 0x3c
    void OnUseSmallFont(CSWGuiControl* pControl);
    void OnEnableToolTips(CSWGuiControl* pControl);
    void OnHideInGameGUI(CSWGuiControl* pControl);
    void OnSummary(CSWGuiControl* pControl);
    void OnFloaty(CSWGuiControl* pControl);
    void OnMiniMap(CSWGuiControl* pControl);
    void OnSubtitles(CSWGuiControl* pControl);
    void OnTutorial(CSWGuiControl* pControl);
    void OnHide(CSWGuiControl* pControl);
    void OnEnterSlot(CSWGuiControl* pControl);
    void OnDefault(CSWGuiControl* pControl);
    void SetUpOptions();
    void SetDescription(unsigned long nStrRef);
    CSWGuiListBox m_control_64; // 0x64
    CSWGuiLabel m_control_344; // 0x344
    CSWGuiLabel m_control_484; // 0x484
    CSWGuiListBox m_control_5C4; // 0x5C4
    CSWGuiButton m_control_8A4; // 0x8A4
    CSWGuiButton m_control_A68; // 0xA68
};

#ifdef L_SWGUI_MATCHING_DESTRUCTOR_TU
class __declspec(novtable) CSWGuiInGameAutoPause : public CSWGuiPanel {
#else
class CSWGuiInGameAutoPause : public CSWGuiPanel {
#endif
public:
    CSWGuiInGameAutoPause(CSWGuiManager* pManager);    // 0x006E76A0
    __declspec(noinline) virtual ~CSWGuiInGameAutoPause(); // 0x006E7A90
    virtual void HandleInputEvent(int nEvent, int bDown); // panel slot 0x3c
    void OnDefault(CSWGuiControl* pControl);
    void OnEnterSlot(CSWGuiControl* pControl);
    void SetUpOptions(); // 0x006E7280
    void SetDescription(unsigned long nStrRef); // 0x006E75A0
    CSWGuiOptionsCheckbox m_aCheckboxes[6];              // 0x64
    CSWGuiLabel m_lblDescription;                        // 0x109C
    CSWGuiLabel m_lblTitle;                              // 0x11DC
    CSWGuiButton m_btnBack;                              // 0x131C
    CSWGuiButton m_btnDefault;                           // 0x14E0
    CSWGuiListBox m_lbDetails;                            // 0x16A4
};

#ifdef L_OPTIONS_FAMILY_NOVTABLE
class __declspec(novtable) CSWGuiOptionsGraphics : public CSWGuiPanel {
#else
class CSWGuiOptionsGraphics : public CSWGuiPanel {
#endif
public:
    CSWGuiOptionsGraphics(CSWGuiManager* pManager); // 0x006E2E70
    __declspec(noinline) virtual ~CSWGuiOptionsGraphics(); // 0x006E3440
    virtual void HandleInputEvent(int nEvent, int bDown); // panel slot 0x3c
    void OnShadows(CSWGuiControl* pControl);
    void OnGrass(CSWGuiControl* pControl);
    void OnExitSlider(CSWGuiControl* pControl);
    void OnEnterSlider(CSWGuiControl* pControl);
    void OnResolution(CSWGuiControl* pControl);
    void OnAdvanced(CSWGuiControl* pControl);
    void OnEnterSlot(CSWGuiControl* pControl);
    void OnDefault(CSWGuiControl* pControl);
    void SetUpOptions();
    void SetDescription(unsigned long nStrRef);
    CSWGuiSlider m_control_64; // 0x64
    CSWGuiLabel m_control_21C; // 0x21C
    CSWGuiLabel m_control_35C; // 0x35C
    CSWGuiLabel m_control_49C; // 0x49C
    CSWGuiListBox m_control_5DC; // 0x5DC
    CSWGuiButton m_control_8BC; // 0x8BC
    CSWGuiButton m_control_A80; // 0xA80
    CSWGuiButton m_control_C44; // 0xC44
    CSWGuiButtonToggle m_control_E08; // 0xE08
    CSWGuiButtonToggle m_control_10BC; // 0x10BC
    CSWGuiButton m_control_1370; // 0x1370
    CSWGuiButton m_control_1534; // 0x1534
    CSWGuiButton m_control_16F8; // 0x16F8
    char pad_0x18bc[0x4];
};

#ifdef L_OPTIONS_FAMILY_NOVTABLE
class __declspec(novtable) CSWGuiOptionsSound : public CSWGuiPanel {
#else
class CSWGuiOptionsSound : public CSWGuiPanel {
#endif
public:
    CSWGuiOptionsSound(CSWGuiManager* pManager); // 0x006E3550
    __declspec(noinline) virtual ~CSWGuiOptionsSound(); // 0x006E3C60
    virtual void HandleInputEvent(int nEvent, int bDown); // panel slot 0x3c
    void OnEnterSlider(CSWGuiControl* pControl);
    void OnAdvanced(CSWGuiControl* pControl);
    void OnDefault(CSWGuiControl* pControl);
    void SetUpOptions();
    void SetDescription(unsigned long nStrRef);
    CSWGuiSlider m_control_64; // 0x64
    CSWGuiSlider m_control_21C; // 0x21C
    CSWGuiSlider m_control_3D4; // 0x3D4
    CSWGuiSlider m_control_58C; // 0x58C
    CSWGuiLabel m_control_744; // 0x744
    CSWGuiLabel m_control_884; // 0x884
    CSWGuiLabel m_control_9C4; // 0x9C4
    CSWGuiLabel m_control_B04; // 0xB04
    CSWGuiLabel m_control_C44; // 0xC44
    CSWGuiLabel m_control_D84; // 0xD84
    CSWGuiListBox m_control_EC4; // 0xEC4
    CSWGuiButton m_control_11A4; // 0x11A4
    CSWGuiButton m_control_1368; // 0x1368
    CSWGuiButton m_control_152C; // 0x152C
};

#ifdef L_OPTIONS_FAMILY_NOVTABLE
class __declspec(novtable) CSWGuiOptionsResolution : public CSWGuiPanel {
#else
class CSWGuiOptionsResolution : public CSWGuiPanel {
#endif
public:
    CSWGuiOptionsResolution(CSWGuiManager* pManager); // 0x006E0710
    __declspec(noinline) virtual ~CSWGuiOptionsResolution(); // 0x006E0C70
    virtual void HandleInputEvent(int nEvent, int bDown); // panel slot 0x3c
    CSWGuiLabel m_control_64; // 0x64
    CSWGuiListBox m_control_1A4; // 0x1A4
    CSWGuiButton m_control_484; // 0x484
    CSWGuiButton m_control_648; // 0x648
    char pad_0x80c[0x4];
};

#ifdef L_OPTIONS_FAMILY_NOVTABLE
class __declspec(novtable) CSWGuiOptionsGraphicsAdvanced : public CSWGuiPanel {
#else
class CSWGuiOptionsGraphicsAdvanced : public CSWGuiPanel {
#endif
public:
    CSWGuiOptionsGraphicsAdvanced(CSWGuiManager* pManager, CSWGuiOptionsGraphics* pParent); // 0x006E16E0
    __declspec(noinline) virtual ~CSWGuiOptionsGraphicsAdvanced(); // 0x006E1F40
    virtual void HandleInputEvent(int nEvent, int bDown); // panel slot 0x3c
    void OnAntiAliasLeft(CSWGuiControl* pControl);
    void OnAntiAliasRight(CSWGuiControl* pControl);
    void OnFrameBuffer(CSWGuiControl* pControl);
    void OnSoftShadows(CSWGuiControl* pControl);
    void OnVSync(CSWGuiControl* pControl);
    void OnDiscardChanges(CSWGuiControl* pControl);
    void OnEnterSlot(CSWGuiControl* pControl);
    void OnDefault(CSWGuiControl* pControl);
    void OnTexQualLeft(CSWGuiControl* pControl);
    void OnTexQualRight(CSWGuiControl* pControl);
    void OnAnisotropyLeft(CSWGuiControl* pControl);
    void OnAnisotropyRight(CSWGuiControl* pControl);
    void SetUpOptions();
    void SetDescription(unsigned long nStrRef);
    void SetAntiAliasText();
    void SetTexQualText();
    void SetAnisotropyText();
    CSWGuiLabel m_control_64; // 0x64
    CSWGuiLabel m_control_1A4; // 0x1A4
    CSWGuiListBox m_control_2E4; // 0x2E4
    CSWGuiButtonToggle m_control_5C4; // 0x5C4
    CSWGuiButtonToggle m_control_878; // 0x878
    CSWGuiButtonToggle m_control_B2C; // 0xB2C
    CSWGuiButton m_control_DE0; // 0xDE0
    CSWGuiButton m_control_FA4; // 0xFA4
    CSWGuiButton m_control_1168; // 0x1168
    CSWGuiButton m_control_132C; // 0x132C
    CSWGuiButton m_control_14F0; // 0x14F0
    CSWGuiButton m_control_16B4; // 0x16B4
    CSWGuiButton m_control_1878; // 0x1878
    CSWGuiButton m_control_1A3C; // 0x1A3C
    CSWGuiButton m_control_1C00; // 0x1C00
    CSWGuiButton m_control_1DC4; // 0x1DC4
    CSWGuiButton m_control_1F88; // 0x1F88
    CSWGuiButton m_control_214C; // 0x214C
    int m_nTexQuality; // 0x2310
    int m_nMaxTexQuality;
    int m_nAntiAlias;
    int m_nAnisotropy;
    unsigned long m_bFrameBuffer;
    unsigned long m_bVSync;
    unsigned long m_bSoftShadows;
    unsigned long m_bDiscard;
    unsigned long m_bChanged;
    CSWGuiOptionsGraphics* m_pParent;
};

#ifdef L_OPTIONS_FAMILY_NOVTABLE
class __declspec(novtable) CSWGuiOptionsSoundAdvanced : public CSWGuiPanel {
#else
class CSWGuiOptionsSoundAdvanced : public CSWGuiPanel {
#endif
public:
    CSWGuiOptionsSoundAdvanced(CSWGuiManager* pManager); // 0x006E20A0
    __declspec(noinline) virtual ~CSWGuiOptionsSoundAdvanced(); // 0x006E2520
    virtual void HandleInputEvent(int nEvent, int bDown); // panel slot 0x3c
    void OnEAXRight(CSWGuiControl* pControl);
    void OnEnterOption(CSWGuiControl* pControl);
    void OnDiscardChanges(CSWGuiControl* pControl);
    void OnEAXLeft(CSWGuiControl* pControl);
    void OnForceSoftware(CSWGuiControl* pControl);
    void OnDefault(CSWGuiControl* pControl);
    void SetUpOptions();
    void SetDescription(unsigned long nStrRef);
    void SetEAXText();
    void EnableEAXControls(unsigned long bEnabled);
    CSWGuiButtonToggle m_control_64; // 0x64
    int m_nEAX; // 0x318
    CSWGuiButton m_control_31C; // 0x31C
    CSWGuiButton m_control_4E0; // 0x4E0
    CSWGuiButton m_control_6A4; // 0x6A4
    CSWGuiLabel m_control_868; // 0x868
    CSWGuiLabel m_control_9A8; // 0x9A8
    CSWGuiListBox m_control_AE8; // 0xAE8
    CSWGuiButton m_control_DC8; // 0xDC8
    CSWGuiButton m_control_F8C; // 0xF8C
    CSWGuiButton m_control_1150; // 0x1150
    unsigned long m_bForceSoftware; // 0x1314
    unsigned long m_bChanged;
    unsigned long m_bDiscard;
};

#ifdef L_OPTIONS_FAMILY_NOVTABLE
class __declspec(novtable) CSWGuiOptionsMouse : public CSWGuiPanel {
#else
class CSWGuiOptionsMouse : public CSWGuiPanel {
#endif
public:
    CSWGuiOptionsMouse(CSWGuiManager* pManager); // 0x006E2600
    __declspec(noinline) virtual ~CSWGuiOptionsMouse(); // 0x006E29B0
    virtual void HandleInputEvent(int nEvent, int bDown); // panel slot 0x3c
    void OnEnterSlider(CSWGuiControl* pControl);
    void OnEnterRevMouseButtons(CSWGuiControl* pControl);
    void OnMoveMouseSenSlider(CSWGuiControl* pControl);
    void OnDefault(CSWGuiControl* pControl);
    void SetUpOptions();
    void SetDescription(unsigned long nStrRef);
    CSWGuiLabel m_control_64; // 0x64
    CSWGuiLabel m_control_1A4; // 0x1A4
    CSWGuiListBox m_control_2E4; // 0x2E4
    CSWGuiButton m_control_5C4; // 0x5C4
    CSWGuiButton m_control_788; // 0x788
    CSWGuiSlider m_control_94C; // 0x94C
    CSWGuiLabel m_control_B04; // 0xB04
    CSWGuiButtonToggle m_control_C44; // 0xC44
};

#ifdef L_OPTIONS_FAMILY_NOVTABLE
class __declspec(novtable) CSWGuiOptionsMain : public CSWGuiPanel {
#else
class CSWGuiOptionsMain : public CSWGuiPanel {
#endif
public:
    CSWGuiOptionsMain(CSWGuiManager* pManager); // 0x006E3E80
    __declspec(noinline) virtual ~CSWGuiOptionsMain(); // 0x006E42C0
    virtual void HandleInputEvent(int nEvent, int bDown); // panel slot 0x3c
    void OnSoundOpt(CSWGuiControl* pControl);
    void OnGraphicsOpt(CSWGuiControl* pControl);
    void OnFeedbackOpt(CSWGuiControl* pControl);
    void OnAutopauseOpt(CSWGuiControl* pControl);
    void OnGameplayOpt(CSWGuiControl* pControl);
    void OnEnterSlot(CSWGuiControl* pControl);
    void SetDescription(unsigned long nStrRef);
    CSWGuiButton m_control_64[6]; // 0x64
    CSWGuiLabel m_control_AFC; // 0xAFC
    CSWGuiLabel m_control_C3C; // 0xC3C
    CSWGuiButton m_control_D7C; // 0xD7C
    CSWGuiListBox m_control_F40; // 0xF40
};

// Keymapping panel layout used by Gameplay::OnKeyMap, ctor 0x006EDC90.
class CSWGuiInGameOptKeyMappings : public CSWGuiPanel {
public:
    CSWGuiInGameOptKeyMappings(CSWGuiManager* pManager);
    char pad_0x64[0xf34 - 0x64];
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
