// CSWGuiKeyMapButton (vtable 0x007593C8, size 0x3B8). Layout follows the
// keymap.2da consumers and constructor stores in the original executable.
#ifndef L_SW_GUI_KEYMAP_BUTTON_H
#define L_SW_GUI_KEYMAP_BUTTON_H

#include <stddef.h>
#include "l/sw_gui_options_in_game.h"

class CSWGuiInGameOptKeyMappings;

class CSWGuiKeyMapButton : public CSWGuiButton {
public:
    CSWGuiKeyMapButton(CSWGuiInGameOptKeyMappings* pOwner);
    virtual ~CSWGuiKeyMapButton();
    virtual void SetExtent(const CSWGuiExtent& extent);    // 0x006EC250
    virtual void Draw(float fDelta);                       // 0x006EC350
    virtual CSWGuiControl* HitCheckMouse(int x, int y);    // 0x006EC380
    void UpdateColor();                                    // 0x006EC6E0
    void SetUnchangable();                                 // 0x006EC770

    unsigned short m_nDescriptionStrRef;                   // 0x1C4
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
