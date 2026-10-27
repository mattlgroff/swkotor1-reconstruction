// Implicit destructors of small in-game panels and the party selection
// screen. The original bodies do not restore the derived vptr, so the
// classes are declared novtable in this isolated TU; their scalar deleting
// destructors are in sw_gui_generated_delete.cpp.
#define L_PARTY_SELECTION_DTORS
#include "l/sw_gui_party_selection.h"

// Pause menu panel.
class __declspec(novtable) CSWGuiInGamePause : public CSWGuiPanel {
public:
    virtual ~CSWGuiInGamePause();
    CSWGuiLabel m_lblTitle;                         // 0x64
    CSWGuiLabel m_lblMessage;                       // 0x1a4
    CSWGuiButton m_btnResume;                       // 0x2e4
};

// FUNCTION: SWKOTOR 0x006C05A0
CSWGuiInGamePause::~CSWGuiInGamePause()
{
}

// Solo-mode question box (0xFC8 bytes) built on the message box.
class __declspec(novtable) CSWGuiInGameSoloModeQuery : public CSWGuiMessageBox {
public:
    virtual ~CSWGuiInGameSoloModeQuery();
    CSWGuiLabel m_aLabels984;                       // 0x984
    CSWGuiLabel m_aLabelsAC4;                       // 0xac4
    CSWGuiLabel m_aLabelsC04;                       // 0xc04
    CSWGuiLabel m_aLabelsD44;                       // 0xd44
    CSWGuiLabel m_aLabelsE84;                       // 0xe84
    int m_nFC4;                                     // 0xfc4
};

// FUNCTION: SWKOTOR 0x006C2560
CSWGuiInGameSoloModeQuery::~CSWGuiInGameSoloModeQuery()
{
}

class __declspec(novtable) CSWGuiInGameAreaTransition : public CSWGuiPanel {
public:
    virtual ~CSWGuiInGameAreaTransition();
    CSWGuiLabel m_lbl64;                            // 0x64
    CSWGuiLabel m_lbl1A4;                           // 0x1a4
    CSWGuiLabel m_lbl2E4;                           // 0x2e4
};

// FUNCTION: SWKOTOR 0x006C8190
CSWGuiInGameAreaTransition::~CSWGuiInGameAreaTransition()
{
}

// The radio button base destructor is inlined into the party slot.
CSWGuiRadioButton::~CSWGuiRadioButton() {}

// FUNCTION: SWKOTOR 0x006BE420
CSWGuiPartySelectionSlot::~CSWGuiPartySelectionSlot()
{
}

// FUNCTION: SWKOTOR 0x006BFFB0
CSWGuiPartySelection::~CSWGuiPartySelection()
{
}
