// Worker L matching headers: the party selection screen CSWGuiPartySelection
// (vtable 0x00756D28, 0x3A80 bytes) and its nine member buttons
// (CSWGuiPartySelectionSlot, descriptive name, 0x454 bytes).
#ifndef L_SW_GUI_PARTY_SELECTION_H
#define L_SW_GUI_PARTY_SELECTION_H

#include "l/sw_gui_inventory_equip.h"

#ifdef L_PARTY_SELECTION_DTORS
#define L_PARTY_VTABLE __declspec(novtable)
#else
#define L_PARTY_VTABLE
#endif

class L_PARTY_VTABLE CSWGuiPartySelectionSlot : public CSWGuiRadioButton {
public:
    CSWGuiPartySelectionSlot();                     // 0x006BED70
    virtual ~CSWGuiPartySelectionSlot();            // 0x006BE420
    void SetSelected(int bSelected);                // 0x006BE370

    int m_bSelected;                                // 0x1c4
    CSWGuiLabel m_lblPortrait;                      // 0x1c8 (descriptive)
    CSWGuiLabel m_lblBorder;                        // 0x308 (descriptive)
    union {
        unsigned long m_nSlotFlags;                 // 0x448
        struct {
            unsigned long m_bAvailable : 1;         //      bit 0 (descriptive)
            unsigned long m_bMember : 1;            //      bit 1 (descriptive)
            unsigned long m_bForced : 1;            //      bit 2 (descriptive)
            unsigned long m_nSlotFlagsRest : 29;
        };
    };
    int m_nPartyIndex;                              // 0x44c
    int m_nNPC;                                     // 0x450
};

extern Vector g_vAbilityColorGranted;               // 0x007A23C0
extern Vector g_vAbilityColorLocked;                // 0x007A23E4
extern Vector g_vDifficultyExitColor;               // 0x007A23B4
extern Vector g_vUpgradeAvailableColor;             // 0x007A2378 (descriptive)
extern Vector g_vKeyMapLockedColor;                 // 0x007A2384
extern Vector g_vKeyMapLockedTextColor;             // 0x007A2408

class L_PARTY_VTABLE CSWGuiPartySelection : public CSWGuiPanel {
public:
    CSWGuiPartySelection(CSWGuiManager* pManager);  // 0x006BFA40
    virtual ~CSWGuiPartySelection();                // 0x006BFFB0
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006BEDE0)
    virtual void OnPanelRemoved();                  // 0x4c (0x006BE490)
    void SetForcedNPC(int nForced1, int nForced2);  // 0x006BE310
    void UpdateCount();                             // 0x006BE4E0
    void AcceptParty();                             // 0x006BE560
    void ConfirmPartySelection(CSWGuiControl* pControl);    // 0x006BEC90
    void SetAddRemoveLabel(int bAdd);               // 0x006BECC0
    void OnToggled(CSWGuiControl* pControl);        // 0x006BF2A0
    void OnDone(CSWGuiControl* pControl);           // 0x006BF3B0
    void OnEnter(CSWGuiControl* pControl);          // 0x006BF5B0
    void OnLeave(CSWGuiControl* pControl);          // shared empty handler 0x0060E760 (descriptive)

    int m_nMode;                                    // 0x64
    int m_nSelected;                                // 0x68
    int m_bForced;                                  // 0x6c
    int m_bFromScript;                              // 0x70
    CExoString m_sExitScript;                       // 0x74
    CSWGuiPartySelectionSlot m_aSlots[9];           // 0x7c
    CSWGuiLabel m_lblCount;                         // 0x2770
    CSWGuiButton m_btnDone;                         // 0x28b0
    CSWGuiLabel m_lblNPCName;                       // 0x2a74
    CSWGuiLabel m_lblNPCLevel;                      // 0x2bb4
    CSWGuiLabel m_lbl3D;                            // 0x2cf4
    CSWGuiLabel m_aLabels[7];                       // 0x2e34 (0 title, 3 available, 4-6 bevels)
    CSWGuiButton m_btnBack;                         // 0x36f4
    CSWGuiButton m_btnAccept;                       // 0x38b8
    CSWGuiPartySelectionSlot* m_pEntered;           // 0x3a7c
};

#endif // L_SW_GUI_PARTY_SELECTION_H
