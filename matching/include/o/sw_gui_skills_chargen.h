// Worker O matching headers: CSWGuiSkillsCharGen (swguiskillschargen.o,
// layout "SKCHRGEN", vtable 0x00759990, 0x49D0 bytes), the skill point
// screen of the custom character panel and the level-up panel,
// 0x006F47E0-0x006F6A60.
#ifndef O_SW_GUI_SKILLS_CHARGEN_H
#define O_SW_GUI_SKILLS_CHARGEN_H

#include "o/sw_gui_main_chargen.h"

extern Vector g_vGuiDisabledTextColor;              // 0x007A2408
extern Vector g_vAbilityColorGranted;               // 0x007A23C0
extern const unsigned short RACIAL_TYPE_DROID;      // 0x0074BF10

// The level-up stats calls of the skill screen.
class CSWCLevelUpStatsS : public CSWCLevelUpStatsL {
public:
    int GetSkillPointCostForSkillRank(unsigned short nSkill, int bArg);    // 0x00647900
    int IncreaseSkillRank(unsigned short nSkill);   // 0x00648D50
    void DecreaseSkillRank(unsigned short nSkill, unsigned char nMin);  // 0x00648EB0
    char GetSkillRankOOL(unsigned short nSkill);    // 0x0060B350 (an out-of-line GetSkillRank)
    CSWClass* GetClassObject(unsigned char nIndex) { return &g_pRules->m_pClasses[GetClass(nIndex)]; }
    unsigned char GetClassId(unsigned char nIndex) { return GetClass(nIndex); }
    void SetModifiedSkillRank(unsigned short nSkill, char nRank)
    {
        if (nSkill < g_pRules->m_nNumSkills) {
            m_pModifiedSkillRanks[nSkill] = nRank;
        }
    }
};

class CSWGuiSkillsCharGen : public CSWGuiPanel {
public:
    CSWGuiSkillsCharGen(CSWGuiManager* pManager, CSWCCreature* pCreature, int bLevelUp);  // 0x006F51D0
    ~CSWGuiSkillsCharGen();                         // 0x006F47E0
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006F6A10)
    virtual void OnPanelAdded();                    // 0x48 (0x006F49A0)
    void HandleCancelButton();                      // 0x006F49D0
    void OnExitPointsButton(CSWGuiControl* pControl);       // 0x006F4A80
    void SetDescription(const CExoString& sText);   // 0x006F4AB0 (descriptive)
    int IsClassSkill(unsigned short nSkill);        // 0x006F4B60
    void OnEnterPointsButton(CSWGuiControl* pControl);      // 0x006F4BF0
    void ConfirmAccept(CSWGuiControl* pControl);    // 0x006F4DC0
    void OnRecommendButton();                       // 0x006F4E20
    void OnMinusButton();                           // 0x006F6370
    void OnPlusButton();                            // 0x006F6570
    void OnAcceptButton();                          // 0x006F6980
    // Button events; identical code is folded with other panels' handlers.
    void OnMinusButtonPushed(CSWGuiControl* pControl);  // 0x0067CB40 (HandleInputEvent(0x2f, 1))
    void OnPlusButtonPushed(CSWGuiControl* pControl);   // 0x0067CB50 (HandleInputEvent(0x30, 1))
    CSWCLevelUpStatsS* GetStats() { return (CSWCLevelUpStatsS*)m_pCreature->m_pStats; }
    // Greyed skills: not selectable, or not usable by droids.
    int IsUnusable(unsigned short nSkill)
    {
        CSWSkill* pSkill = &g_pRules->m_pSkills[nSkill];
        return !(pSkill->m_nFlags & 0x10) || (m_bDroid && !(pSkill->m_nFlags & 0x20));
    }

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
    CSWGuiLabel m_lblClassSkill;                    // 0xd4c CLASSSKL_LBL
    CSWGuiLabel m_lblDescItem;                      // 0xe8c LB_DESC item
    CSWGuiLabel m_aLabels[8];                       // 0xfcc <skill>_LBL
    CSWGuiButton m_aPoints[8];                      // 0x19cc <skill>_POINTS_BTN
    CSWGuiButton m_btnAccept;                       // 0x27ec BTN_ACCEPT
    CSWGuiButton m_btnBack;                         // 0x29b0 BTN_BACK
    CSWGuiButton m_btnRecommended;                  // 0x2b74 BTN_RECOMMENDED
    CSWGuiButton m_aPlus[8];                        // 0x2d38 <abbr>_PLUS_BTN
    CSWGuiButton m_aMinus[8];                       // 0x3b58 <abbr>_MINUS_BTN
    CExoString m_aDescriptions[8];                  // 0x4978
    int m_nPoints;                                  // 0x49b8 skill points left
    int m_nCurrent;                                 // 0x49bc current skill
    unsigned char m_aBaseRanks[8];                  // 0x49c0 ranks when the screen opened
    unsigned long m_bLevelUp : 1;                   // 0x49c8 bit 0
    unsigned long m_bAllSkills : 1;                 //        bit 1 (not a party member)
    unsigned long m_bDroid : 1;                     //        bit 2
    int m_nInitialPoints;                           // 0x49cc
};

#endif
