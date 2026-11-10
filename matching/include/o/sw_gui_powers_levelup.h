// Worker O matching headers: CSWGuiPowersLevelUp (swguipowerslvlup.o, layout
// "pwrlvlup", vtable 0x00759780, 0x1A10 bytes), the Force power picker of
// the level-up panel, 0x006F0EE0-0x006F2910. The twin of
// CSWGuiFeatsCharGen (o/sw_gui_feats_chargen.h).
#ifndef O_SW_GUI_POWERS_LEVELUP_H
#define O_SW_GUI_POWERS_LEVELUP_H

#include "o/sw_gui_feats_chargen.h"

// The level-up stats calls of the power picker.
class CSWCLevelUpStatsP : public CSWCLevelUpStats {
public:
    int CanChooseSpell(unsigned long nSpell, unsigned char nLevel,
                       CExoArrayListM<unsigned long>* pChosen);     // 0x00649B50
    int HasSpellPrereq(CSWSpell* pSpell, CExoArrayListM<unsigned long>* pChosen); // 0x006497A0
    void AddDefaultPackageSpells(CExoArrayListM<unsigned long>* pList);   // 0x00649C30
};

class CSWGuiPowersLevelUp : public CSWGuiPanel {
public:
    CSWGuiPowersLevelUp(CSWGuiManager* pManager, unsigned long oidCreature);  // 0x006F2180
    ~CSWGuiPowersLevelUp();                         // 0x006F0EE0
    virtual void Draw(float fDelta);                // 0x34 (0x006F1210)
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006F28C0)
    virtual void OnPanelAdded();                    // 0x48 (0x006F10C0)
    void HandleCancelButton();                      // 0x006F10F0
    void OnAcceptButton();                          // 0x006F1130
    // 0 available, 1 chosen, 2 known, 3 none.
    void DeterminePower(unsigned long nSpell, unsigned char* pStatus);  // 0x006F1280
    void SetDescription(unsigned long nStrRef);     // 0x006F1320
    void OnEnterPower(unsigned long nSpell);        // 0x006F1460
    void BuildButtons();                            // 0x006F15A0
    void BuildAvailableList();                      // 0x006F1730
    void OnPowerSelectionChanged(CSWGuiControl* pControl);  // 0x006F1940
    void InitiatePowers();                          // 0x006F1990
    void AddChosenPower(unsigned long nSpell);      // 0x006F1C90
    void RemoveChosenPower(unsigned long nSpell);   // 0x006F1E40
    void OnRecommendButton();                       // 0x006F1F30
    void OnPowerPicked(unsigned long nSpell);       // 0x006F2030
    void OnDoubleClick(CSWGuiControl* pControl);    // 0x006F2110
    CSWCLevelUpStatsP* GetStats() { return (CSWCLevelUpStatsP*)m_pCreature->m_pStats; }
    // Spells 6 and 14 are not offered to party members other than the player.
    int IsLocked(unsigned long nSpell) { return !m_bIsPC && (nSpell == 6 || nSpell == 14); }

    CSWCCreature* m_pCreature;                      // 0x64
    CSWGuiLevelUpPanel* m_pParent;                  // 0x68
    CSWGuiLabel m_lblTitle;                         // 0x6c MAIN_TITLE_LBL
    CSWGuiLabel m_lblSubTitle;                      // 0x1ac SUB_TITLE_LBL
    CSWGuiLabel m_lblRemainingBox;                  // 0x2ec REMAINING_BOX_LBL
    CSWGuiLabel m_lblRemaining;                     // 0x42c REMAINING_SELECTIONS_LBL
    CSWGuiLabel m_lblSelectionsRemaining;           // 0x56c SELECTIONS_REMAINING_LBL
    CSWGuiLabel m_lblDesc;                          // 0x6ac DESC_LBL
    CSWGuiLabel m_lbl7ec;                           // 0x7ec (not loaded)
    CSWGuiLabel m_lbl92c;                           // 0x92c (not loaded)
    CSWGuiLabel m_lblDescItem;                      // 0xa6c LB_DESC item
    CSWGuiLabel m_lblName;                          // 0xbac LBL_POWER
    CSWGuiListBox m_lbPowers;                       // 0xcec LB_POWERS
    CSWGuiListBox m_lbDesc;                         // 0xfcc LB_DESC
    CSWGuiButton m_btnRecommended;                  // 0x12ac RECOMMENDED_BTN
    CSWGuiButton m_btnSelect;                       // 0x1470 SELECT_BTN
    CSWGuiButton m_btnAccept;                       // 0x1634 ACCEPT_BTN
    CSWGuiButton m_btnBack;                         // 0x17f8 BACK_BTN
    unsigned char m_nRemaining;                     // 0x19bc
    unsigned char m_nSpellLevel;                    // 0x19bd CanChooseSpell level (remaining + 1)
    unsigned long m_bIsPC : 1;                      // 0x19c0 bit 0
    unsigned long m_nCurrentSpell;                  // 0x19c4
    CExoString m_sSelectText;                       // 0x19c8
    CExoString m_sRemoveText;                       // 0x19d0
    CExoArrayListM<unsigned long> m_lstKnown;       // 0x19d8
    CExoArrayListM<unsigned long> m_lstAvailable;   // 0x19e4
    CExoArrayListM<unsigned long> m_lstChosen;      // 0x19f0
    CSWGuiAbilityList m_cChart;                     // 0x19fc
    unsigned char m_nStatus;                        // 0x1a0c DeterminePower of the current spell
};

#endif
