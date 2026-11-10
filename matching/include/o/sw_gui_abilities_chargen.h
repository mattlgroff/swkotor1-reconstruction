// Worker O matching headers: CSWGuiAbilitiesCharGen (swguiabilitieschargen.o,
// layout "ABCHRGEN", vtable 0x00759C68, 0x3DF4 bytes), the ability score
// screen of the custom character panel and the level-up panel,
// 0x006F6B60-0x006F89D0. Scores are kept in STR, DEX, CON, WIS, INT, CHA
// order.
#ifndef O_SW_GUI_ABILITIES_CHARGEN_H
#define O_SW_GUI_ABILITIES_CHARGEN_H

#include "o/sw_gui_main_chargen.h"

extern Vector g_vAbilityColorGranted;               // 0x007A23C0

// The level-up stats calls of the ability screen.
class CSWCLevelUpStatsA : public CSWCLevelUpStats {
public:
    CSWClass* GetClassObject(unsigned char nIndex) { return &g_pRules->m_pClasses[GetClass(nIndex)]; }
    unsigned char GetSTRBase() { return m_nSTRBase; }
    unsigned char GetDEXBase() { return m_nDEXBase; }
    unsigned char GetCONBase() { return m_nCONBase; }
    unsigned char GetWISBase() { return m_nWISBase; }
    unsigned char GetINTBase() { return m_nINTBase; }
    unsigned char GetCHABase() { return m_nCHABase; }
};

class CSWGuiAbilitiesCharGen : public CSWGuiPanel {
public:
    CSWGuiAbilitiesCharGen(CSWGuiManager* pManager, CSWCCreature* pCreature, int bLevelUp);  // 0x006F7600
    ~CSWGuiAbilitiesCharGen();                      // 0x006F6BF0
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006F8880)
    virtual void OnPanelAdded();                    // 0x48 (0x006F6DB0)
    // Points given back by lowering a score (negative): -1 on level-up.
    int GetAbilityPointRefund(int nScore);          // 0x006F6B60 (descriptive)
    // Points needed to raise a score: 1 on level-up.
    int GetAbilityPointCost(int nScore);            // 0x006F6BB0
    void HandleCancelButton();                      // 0x006F6DE0
    void OnExitPointsButton(CSWGuiControl* pControl);       // 0x006F6E40
    void CompletePanel();                           // 0x006F6E70
    void SetDescription(unsigned long nStrRef);     // 0x006F6FA0
    void OnEnterPointsButton(CSWGuiControl* pControl);      // 0x006F70E0
    void OnAcceptButton();                          // 0x006F7330
    void OnRecommendButton();                       // 0x006F7390
    void OnMinusButton();                           // 0x006F8480
    void OnPlusButton();                            // 0x006F8670
    // Button events; identical code is folded with other panels' handlers.
    void OnMinusButtonPushed(CSWGuiControl* pControl);  // 0x0067CB40 (HandleInputEvent(0x2f, 1))
    void OnPlusButtonPushed(CSWGuiControl* pControl);   // 0x0067CB50 (HandleInputEvent(0x30, 1))
    CSWCLevelUpStatsA* GetStats() { return (CSWCLevelUpStatsA*)m_pCreature->m_pStats; }

    CSWCCreature* m_pCreature;                      // 0x64
    CSWGuiPanel* m_pParent;                         // 0x68 level-up or custom panel
    CSWGuiListBox m_lbDesc;                         // 0x6c LB_DESC
    CSWGuiLabel m_lblTitle;                         // 0x34c MAIN_TITLE_LBL
    CSWGuiLabel m_lblSubTitle;                      // 0x48c SUB_TITLE_LBL
    CSWGuiLabel m_lblRemainingBox;                  // 0x5cc REMAINING_BOX_LBL
    CSWGuiLabel m_lblRemaining;                     // 0x70c REMAINING_SELECTIONS_LBL
    CSWGuiLabel m_lblSelectionsRemaining;           // 0x84c SELECTIONS_REMAINING_LBL
    CSWGuiLabel m_lblDesc;                          // 0x98c DESC_LBL
    CSWGuiLabel m_lblCost;                          // 0xacc COST_LBL
    CSWGuiLabel m_lblCostPoints;                    // 0xc0c COST_POINTS_LBL
    CSWGuiLabel m_lblDescItem;                      // 0xd4c LB_DESC item
    CSWGuiLabel m_lblAbilityMod;                    // 0xe8c LBL_ABILITY_MOD
    CSWGuiLabel m_lblModifier;                      // 0xfcc LBL_MODIFIER
    CSWGuiLabel m_aLabels[6];                       // 0x110c <ability>_LBL
    CSWGuiButton m_aPoints[6];                      // 0x188c <ability>_POINTS_BTN
    CSWGuiButton m_btnAccept;                       // 0x2324 BTN_ACCEPT
    CSWGuiButton m_btnBack;                         // 0x24e8 BTN_BACK
    CSWGuiButton m_btnRecommended;                  // 0x26ac BTN_RECOMMENDED
    CSWGuiButton m_aPlus[6];                        // 0x2870 <ability>_PLUS_BTN
    CSWGuiButton m_aMinus[6];                       // 0x3308 <ability>_MINUS_BTN
    unsigned long m_anDescriptions[6];              // 0x3da0 description strrefs
    int m_nPoints;                                  // 0x3db8 points left
    int m_anScores[6];                              // 0x3dbc
    int m_anBaseScores[6];                          // 0x3dd4 scores when a level-up opened
    int m_nCurrent;                                 // 0x3dec current ability
    int m_bLevelUp;                                 // 0x3df0
};

#endif
