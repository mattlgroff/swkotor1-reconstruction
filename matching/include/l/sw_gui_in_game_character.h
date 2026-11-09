// Worker L matching headers: the party-member portrait buttons of the
// in-game character screen CSWGuiInGameCharacter (0x006AF350-0x006AF950).
// Only the members these callbacks use are declared.
#ifndef L_SW_GUI_IN_GAME_CHARACTER_H
#define L_SW_GUI_IN_GAME_CHARACTER_H

#include "l/sw_gui_in_game_abilities.h"

extern Vector g_vDifficultyExitColor;               // 0x007A23B4

class CSWGuiInGameCharacter : public CSWGuiPanel {
public:
    void UpdatePortraits();                         // 0x006AEE30
    void OnChangeCharacter(CSWGuiControl* pControl);    // 0x006AF350
    void DoAutoLevelup(CSWGuiControl* pControl);    // 0x006AEC80
    void SetLabelBonusPenaltyColour(CSWGuiLabel* pLabel, int nValue);   // 0x006AECD0
    void OnSwitchLeft(CSWGuiControl* pControl);     // 0x006AF450
    void OnSwitchRight(CSWGuiControl* pControl);    // 0x006AF6D0

    char pad_0x64[0x4b2c - 0x64];
    CSWGuiButton m_btn4B2C;                         // 0x4b2c
    CSWGuiButton m_btn4CF0;                         // 0x4cf0
    char pad_0x4eb4[0x5078 - 0x4eb4];
    CSWGuiButton m_btn5078;                         // 0x5078
    char pad_0x523c[0x59f0 - 0x523c];
    char m_nPartyMember;                            // 0x59f0
};

#endif // L_SW_GUI_IN_GAME_CHARACTER_H
