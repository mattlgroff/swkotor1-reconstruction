// Worker O matching headers: CSWGuiFeatsCharGen (swguifeatschargen.o,
// layout "FTCHRGEN", vtable 0x007598B0, 0x1A1C bytes), the feat picker of
// the custom character panel and the level-up panel, 0x006F2A20-0x006F47D0.
#ifndef O_SW_GUI_FEATS_CHARGEN_H
#define O_SW_GUI_FEATS_CHARGEN_H

#include "o/sw_gui_main_chargen.h"

extern Vector g_vGuiDisabledTextColor;              // 0x007A2408
extern Vector g_vGuiDisabledBorderColor;            // 0x007A2384
extern Vector g_vUpgradeAvailableColor;             // 0x007A2378

// The level-up stats calls of the feat picker.
class CSWCLevelUpStatsF : public CSWCLevelUpStats {
public:
    int CanChooseFeat(unsigned short nFeat, unsigned char nNormal, unsigned char nBonus,
                      CExoArrayListM<unsigned short>* pChosen);     // 0x0064A4C0
    int HasFeatPrereq(CSWFeat* pFeat, CExoArrayListM<unsigned short>* pChosen); // 0x006498A0
    int AddDefaultPackageFeats(unsigned char nClass, CExoArrayListM<unsigned short>* pList,
                               int bNormal, int bBonus);            // 0x0064A770
    void AddGrantedFeats(CExoArrayListM<int>* pList);  // 0x00649950
    void AddFeat(unsigned short nFeat) { m_lstFeats.AddUnique(nFeat); }
};

#ifdef O_FEATS_CHARGEN_DTOR
class __declspec(novtable) CSWGuiFeatsCharGen : public CSWGuiPanel {
#else
class CSWGuiFeatsCharGen : public CSWGuiPanel {
#endif
public:
    CSWGuiFeatsCharGen(CSWGuiManager* pManager, CSWCCreature* pCreature, int bLevelUp,
                       int bBonusOnly);             // 0x006F3D60
    ~CSWGuiFeatsCharGen();                          // 0x006F2A50
    virtual void Draw(float fDelta);                // 0x34 (0x006F2CC0)
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006F4680)
    virtual void OnPanelAdded();                    // 0x48 (0x006F2F30)
    void ShowGranted(CSWGuiControl* pControl);      // 0x006F2C40
    void HandleCancelButton();                      // 0x006F2C70
    // 0 available, 1 chosen, 2 granted at this level, 3 known, 4 none.
    void DetermineFeat(unsigned short nFeat, unsigned char* pStatus);  // 0x006F2D30
    void SetDescription(unsigned long nStrRef);     // 0x006F2DF0
    void OnEnterFeat(unsigned short nFeat);         // 0x006F2FB0
    void BuildButtons();                            // 0x006F30F0
    void BuildAvailableList();                      // 0x006F3290
    void OnFeatSelectionChanged(CSWGuiControl* pControl);   // 0x006F3420
    void InitiateFeats();                           // 0x006F3460
    void AddChosenFeat(unsigned short nFeat);       // 0x006F37E0
    void RemoveChosenFeat(unsigned short nFeat);    // 0x006F3990
    void OnRecommendButton();                       // 0x006F3A80
    void OnFeatPicked(unsigned short nFeat);        // 0x006F3C20
    void OnDoubleClick(CSWGuiControl* pControl);    // 0x006F3CF0
    void OnAcceptButton();                          // 0x006F44C0
    CSWCLevelUpStatsF* GetStats() { return (CSWCLevelUpStatsF*)m_pCreature->m_pStats; }

    CSWCCreature* m_pCreature;                      // 0x64
    CSWGuiPanel* m_pParent;                         // 0x68 level-up or custom panel
    CSWGuiLabel m_lblTitle;                         // 0x6c MAIN_TITLE_LBL
    CSWGuiLabel m_lblSubTitle;                      // 0x1ac SUB_TITLE_LBL
    CSWGuiLabel m_lblRemainingBox;                  // 0x2ec REMAINING_BOX_LBL
    CSWGuiLabel m_lblRemaining;                     // 0x42c STD_REMAINING_SELECTIONS_LBL
    CSWGuiLabel m_lblSelectionsRemaining;           // 0x56c STD_SELECTIONS_REMAINING_LBL
    CSWGuiLabel m_lblDesc;                          // 0x6ac DESC_LBL
    CSWGuiLabel m_lbl7ec;                           // 0x7ec (not loaded)
    CSWGuiLabel m_lbl92c;                           // 0x92c (not loaded)
    CSWGuiLabel m_lblDescItem;                      // 0xa6c LB_DESC item
    CSWGuiLabel m_lblName;                          // 0xbac LBL_NAME
    CSWGuiButton m_btnAccept;                       // 0xcec BTN_ACCEPT
    CSWGuiButton m_btnBack;                         // 0xeb0 BTN_BACK
    CSWGuiButton m_btnRecommended;                  // 0x1074 BTN_RECOMMENDED
    CSWGuiButton m_btnSelect;                       // 0x1238 BTN_SELECT
    CSWGuiListBox m_lbFeats;                        // 0x13fc LB_FEATS
    CSWGuiListBox m_lbDesc;                         // 0x16dc LB_DESC
    CExoArrayListM<unsigned short> m_lstKnown;      // 0x19bc feats the creature has
    CExoArrayListM<unsigned short> m_lstGranted;    // 0x19c8 granted at this level
    CExoArrayListM<unsigned short> m_lstAvailable;  // 0x19d4
    CExoArrayListM<unsigned short> m_lstChosen;     // 0x19e0
    unsigned short m_nCurrentFeat;                  // 0x19ec
    unsigned char m_nNormalFeats;                   // 0x19ee
    unsigned char m_nBonusFeats;                    // 0x19ef
    unsigned char m_nNormalLeft;                    // 0x19f0
    unsigned char m_nBonusLeft;                     // 0x19f1
    unsigned char m_nRemaining;                     // 0x19f2
    unsigned long m_bLevelUp : 1;                   // 0x19f4 bit 0
    unsigned long m_bBonusOnly : 1;                 //        bit 1
    unsigned long m_bShowGranted : 1;               //        bit 2
    CExoString m_sSelectText;                       // 0x19f8
    CExoString m_sRemoveText;                       // 0x1a00
    CSWGuiAbilityList m_cChart;                     // 0x1a08
    unsigned char m_nStatus;                        // 0x1a18 DetermineFeat of the current feat
};

#endif
