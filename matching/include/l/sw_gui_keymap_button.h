// CSWGuiKeyMapButton (vtable 0x007593C8, size 0x3B8). Layout follows the
// keymap.2da consumers and constructor stores in the original executable.
#ifndef L_SW_GUI_KEYMAP_BUTTON_H
#define L_SW_GUI_KEYMAP_BUTTON_H

#include <stddef.h>
#include "l/sw_gui_options_in_game.h"

class CSWGuiInGameOptKeyMappings;

extern Vector g_vKeyMapLockedColor;                 // 0x007A2384
extern Vector g_vKeyMapNormalColor;                 // 0x007A2390
extern Vector g_vKeyMapChangedColor;                // 0x007A23FC
extern Vector g_vKeyMapLockedTextColor;             // 0x007A2408

#ifdef L_KEYMAP_BUTTON_NOVTABLE
class __declspec(novtable) CSWGuiKeyMapButton : public CSWGuiButton {
#else
class CSWGuiKeyMapButton : public CSWGuiButton {
#endif
public:
    CSWGuiKeyMapButton(CSWGuiInGameOptKeyMappings* pOwner);
    virtual ~CSWGuiKeyMapButton();                         // 0x006ED570
    virtual void SetExtent(const CSWGuiExtent& extent);    // 0x006EC250
    virtual void Draw(float fDelta);                       // 0x006EC350
    virtual CSWGuiControl* HitCheckMouse(int x, int y);    // 0x006EC380
    void UpdateColor();                                    // 0x006EC6E0
    void SetUnchangable();                                 // 0x006EC770
    CExoString GetActionId(int nId, int nType);            // 0x006EC120
    void SetDescription(const CExoString& sAction);        // 0x006EC7D0
    void ResetToDefault();                                 // 0x006ECCC0
    void PostChangeToEvent();                              // 0x006ECE60
    int UpdateEvent(unsigned long nKey);                   // 0x006EDBD0
    void OnControlEntered(CSWGuiControl* pControl);        // 0x006ED5C0
    void OnEventClicked(CSWGuiControl* pControl);          // 0x006ED5E0
    void OnNothing(CSWGuiControl* pControl);               // folded into 0x0060E760

    int m_nDescriptionStrRef;                              // 0x1C4
    CSWGuiButton m_cEventButton;                           // 0x1C8
    CSWGuiInGameOptKeyMappings* m_pOwner;                  // 0x38C
    int m_nId;                                             // 0x390
    int m_nType;                                           // 0x394
    unsigned long m_bChanged;                              // 0x398
    unsigned long m_nNewKey;                               // 0x39C
    unsigned long m_nOriginalKey;                          // 0x3A0
    unsigned long m_bUnchangeable;                         // 0x3A4
    unsigned long m_nClassFlags;                            // 0x3A8
    int m_nPage;                                           // 0x3AC
    unsigned long m_n3B0;                                  // 0x3B0
    int m_nSortPosition;                                   // 0x3B4
};

typedef char CSWGuiKeyMapButton_layout_check[
    (offsetof(CSWGuiKeyMapButton, m_nDescriptionStrRef) == 0x1C4 &&
     offsetof(CSWGuiKeyMapButton, m_cEventButton) == 0x1C8 &&
     offsetof(CSWGuiKeyMapButton, m_pOwner) == 0x38C &&
     offsetof(CSWGuiKeyMapButton, m_bUnchangeable) == 0x3A4 &&
     offsetof(CSWGuiKeyMapButton, m_nPage) == 0x3AC &&
     offsetof(CSWGuiKeyMapButton, m_nSortPosition) == 0x3B4 &&
     sizeof(CSWGuiKeyMapButton) == 0x3B8) ? 1 : -1];

#endif
