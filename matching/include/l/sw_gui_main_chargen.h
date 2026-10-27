// Worker L matching headers: the character creation frame
// CSWGuiMainCharGen (swguimainchargen.o, layout "MAINCG", vtable 0x007592A8,
// 0x22DC bytes) and its step sub-panels CSWGuiQuickOrCustomPanel (0xD98,
// vtable 0x00759710), CSWGuiQuickPanel (0x1360, vtable 0x00759668) and
// CSWGuiCustomPanel (0x2188, vtable 0x007595E0). Builds on the level-up
// summary header (same "MAINCG" layout, same creature/stat interfaces).
#ifndef L_SW_GUI_MAIN_CHARGEN_H
#define L_SW_GUI_MAIN_CHARGEN_H

#include "l/sw_gui_levelup_chargen.h"

class CAurObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14();
    virtual int PlayAnimation(const char* sName, float fSpeed, int a, int b);  // 0x18 (descriptive)
    virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9c();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual void vF0(); virtual void vF4(); virtual void vF8(); virtual void vFC();
    virtual void v100();
    virtual CAurScene* GetScene();                  // 0x104 (descriptive)
};

class CSWCAnimBase {
public:
    virtual void v00(); virtual void v04();
    virtual CAurObject* GetModel(unsigned char nPart);  // 0x08 (descriptive)
};

// Class armor-class bonus per level (descriptive name of 0x005BE770).
class CSWClassL : public CSWClass {
public:
    char GetACBonus(int nLevel);          // 0x005BE770
};

class CSWCLevelUpStatsL : public CSWCLevelUpStats {
public:
    void ClearPackage();                            // 0x00649A70 (descriptive)
    int AddDefaultPackageFeats(unsigned char nClass, CExoArrayListL<unsigned short>* pList,
                               int a, int b);       // 0x0064A770 (descriptive)
    void AddDefaultPackageSkills(unsigned char nClass, int a);  // 0x006491A0 (descriptive)
    void UpdateAbilityModifiedSkillRanks();         // 0x00649380 (descriptive)
};

// Creature appearance block (0x3C bytes, CSWCCreature +0x21C).
struct CSWCCreatureAppearance {
    char pad_0x0[0x18];
    unsigned short m_nAppearance;                   // 0x18
    char pad_0x1a[0x3c - 0x1a];
};

float FUN_004aa0f0(const Vector& v);                // 0x004AA0F0 (yaw)
float FUN_004aa130(const Vector& v);                // 0x004AA130 (pitch)
Quaternion YawPitchRoll(float fYaw, float fPitch, float fRoll);     // 0x004ACAC0

class CSWGuiQuickOrCustomPanel;
class CSWGuiQuickPanel;
class CSWGuiCustomPanel;

class CSWGuiClassSelection {
public:
    void BeginPlayGame();                           // 0x006DBDF0
};

#ifdef L_MAIN_CHARGEN_DTOR
class __declspec(novtable) CSWGuiMainCharGen : public CSWGuiPanel {
#else
class CSWGuiMainCharGen : public CSWGuiPanel {
#endif
public:
    CSWGuiMainCharGen(CSWGuiManager* pManager, CSWCCreature* pCreature,
                      CSWGuiClassSelection* pClassSel);   // 0x006EB420
    ~CSWGuiMainCharGen();                           // 0x006EBF00
    virtual void Draw(float fDelta);                // 0x34 (0x006EB340)
    virtual void OnPanelAdded();                    // 0x48 (0x006EA810)

    void SetVisiblePanel(int nPanel);               // 0x006EA760
    void RemoveSubPanels();                         // 0x006EA830 (descriptive)
    void DetermineClass();                          // 0x006EA910
    void SetNameString(const CExoString& sName);    // 0x006EA9B0
    void PortraitChanged();                         // 0x006EA9F0
    void AbilitiesChanged();                        // 0x006EAC30
    void ClearAbilities();                          // 0x006EB010
    void BeginPlayGame();                           // 0x006EB320
    void NameChanged();                             // 0x006EEF70

    CSWCLevelUpStatsL* GetStats() { return (CSWCLevelUpStatsL*)m_pCreature->m_pStats; }
    CSWCCreatureVirtuals* GetCreatureV() { return (CSWCCreatureVirtuals*)m_pCreature; }

    CSWCCreature* m_pCreature;                      // 0x64
    CSWGuiClassSelection* m_pClassSelection;        // 0x68
    CSWGuiQuickOrCustomPanel* m_pQuickOrCustom;     // 0x6c
    CSWGuiQuickPanel* m_pQuick;                     // 0x70
    CSWGuiCustomPanel* m_pCustom;                   // 0x74
    CSWGuiLabel m_lblTitle;                         // 0x78 MAIN_TITLE_LBL
    CSWGuiLabel m_lblName;                          // 0x1B8
    CSWGuiLabel m_lblClass;                         // 0x2F8
    CSWGuiLabel m_lblStr;                           // 0x438
    CSWGuiLabel m_lblDex;                           // 0x578
    CSWGuiLabel m_lblCon;                           // 0x6B8
    CSWGuiLabel m_lblInt;                           // 0x7F8
    CSWGuiLabel m_lblWis;                           // 0x938
    CSWGuiLabel m_lblCha;                           // 0xA78
    CSWGuiLabel m_lblStrAb;                         // 0xBB8
    CSWGuiLabel m_lblDexAb;                         // 0xCF8
    CSWGuiLabel m_lblConAb;                         // 0xE38
    CSWGuiLabel m_lblIntAb;                         // 0xF78
    CSWGuiLabel m_lblWisAb;                         // 0x10B8
    CSWGuiLabel m_lblChaAb;                         // 0x11F8
    CSWGuiLabel m_lblVit;                           // 0x1338
    CSWGuiLabel m_lblDef;                           // 0x1478
    CSWGuiLabel m_lblNewWill;                       // 0x15B8
    CSWGuiLabel m_lblNewRefl;                       // 0x16F8
    CSWGuiLabel m_lblNewFort;                       // 0x1838
    CSWGuiLabel m_lblPortrait;                      // 0x1978
    CSWGuiLabel m_lblBevelL;                        // 0x1AB8
    CSWGuiLabel m_lblBevelM;                        // 0x1BF8
    CSWGuiLabel m_lblBevelR;                        // 0x1D38
    CSWGuiLabel m_lblFortitude;                     // 0x1E78
    CSWGuiLabel m_lblReflex;                        // 0x1FB8
    CSWGuiLabel m_lblWill;                          // 0x20F8
    CSWGui3DSceneView m_cModel;                     // 0x2238 MODEL_LBL
    int m_nVisiblePanel;                            // 0x22C8 1 quick-or-custom, 2 quick, 3 custom, 4 none
    CExoString m_sClassName;                        // 0x22CC
    CExoString m_sName;                             // 0x22D4
};

extern Vector g_vAbilityColorGranted;               // 0x007A23C0

// Portrait and name pickers opened by the quick/custom panels; the second
// SetParentPanel argument is 1 for the quick panel, 2 for the custom panel.
class CSWGuiPortraitCharGen : public CSWGuiPanel {
public:
    CSWGuiPortraitCharGen(CSWGuiManager* pManager, CSWCCreature* pCreature);  // 0x006F9430
    void SetParentPanel(CSWGuiPanel* pParent, unsigned char nType);         // 0x006F8A00
    char pad_0x64[0x1240 - 0x64];
};

class CSWGuiNameChargen : public CSWGuiPanel {
public:
    CSWGuiNameChargen(CSWGuiManager* pManager, CSWCCreature* pCreature);  // 0x006F9E70
    void SetParentPanel(CSWGuiPanel* pParent, unsigned char nType);         // 0x006F9B80
    char pad_0x64[0x9c4 - 0x64];
};

// Step panels (layouts "QUICKORCUSTOM", "QUICKPNL", "CUSTPNL").
class CSWGuiQuickOrCustomPanel : public CSWGuiPanel {
public:
    CSWGuiQuickOrCustomPanel(CSWGuiManager* pManager, CSWCCreature* pCreature);   // 0x006F09F0
    CSWCCreature* m_pCreature;                      // 0x64
    CSWGuiMainCharGen* m_pParent;                   // 0x68
    char pad_0x6c[0xd98 - 0x6c];
};

class CSWGuiQuickPanel : public CSWGuiPanel {
public:
    CSWGuiQuickPanel(CSWGuiManager* pManager, CSWCCreature* pCreature);   // 0x006F0390
    void SetState(int nState);                      // 0x006EFC10
    void OnSelectPlay(CSWGuiControl* pControl);     // 0x006EFD60
    void OnSelectName(CSWGuiControl* pControl);     // 0x006EFD80
    void OnSelectPortrait(CSWGuiControl* pControl); // 0x006EFE10
    void CancelledByChild();                        // 0x006EFF10
    void DoQuickStats();                            // 0x006EFFD0
    CSWCLevelUpStatsL* GetStats() { return (CSWCLevelUpStatsL*)m_pCreature->m_pStats; }
    CSWCCreature* m_pCreature;                      // 0x64
    CSWGuiMainCharGen* m_pParent;                   // 0x68
    CSWGuiLabel m_lblDecoration;                    // 0x6c LBL_DECORATION
    CSWGuiLabelHilight m_aHilights[3];              // 0x1ac LBL_%d
    CSWGuiLabel m_aNumbers[3];                      // 0x6c8 LBL_NUM%d
    CSWGuiButton m_aSteps[3];                       // 0xa88 BTN_STEPNAME%d
    CSWGuiButton m_btnBack;                         // 0xfd4
    CSWGuiButton m_btnCancel;                       // 0x1198
    int m_nState;                                   // 0x135c
};

#ifdef L_CUSTOM_PANEL_DTOR
class __declspec(novtable) CSWGuiCustomPanel : public CSWGuiPanel {
#else
class CSWGuiCustomPanel : public CSWGuiPanel {
#endif
public:
    CSWGuiCustomPanel(CSWGuiManager* pManager, CSWCCreature* pCreature);  // 0x006EF730
    ~CSWGuiCustomPanel();                           // 0x006EFB40
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006EF610)
    void SetParentPanel(CSWGuiMainCharGen* pParent);    // 0x006EEF60
    void SetState(int nState);                      // 0x006EEFD0
    void OnSelectPortrait(CSWGuiControl* pControl); // 0x006EF120
    void OnSelectPlay(CSWGuiControl* pControl);     // 0x006EF220
    void OnSelectName(CSWGuiControl* pControl);     // 0x006EF240
    void OnSelectFeats(CSWGuiControl* pControl);    // 0x006EF2D0
    void OnSelectSkills(CSWGuiControl* pControl);   // 0x006EF360
    void OnSelectAbilities(CSWGuiControl* pControl);    // 0x006EF3F0
    void CancelledByChild();                        // 0x006EF480
    void ClearAbilities();                          // 0x006EF540
    void CancelPanel();                             // 0x006EF570
    void ConfirmCancel(CSWGuiControl* pControl);    // 0x006EF5B0 (descriptive)
    void OnCancelPressed(CSWGuiControl* pControl);  // 0x006EF6A0
    CSWCCreature* m_pCreature;                      // 0x64
    CSWGuiMainCharGen* m_pParent;                   // 0x68
    CSWGuiLabelHilight m_aHilights[6];              // 0x6c LBL_%d
    CSWGuiLabel m_aNumbers[6];                      // 0xaa4 LBL_NUM%d
    CSWGuiButton m_aSteps[6];                       // 0x1224 BTN_STEPNAME%d
    CSWGuiLabel m_lblBG;                            // 0x1cbc
    CSWGuiButton m_btnBack;                         // 0x1dfc
    CSWGuiButton m_btnCancel;                       // 0x1fc0
    int m_nState;                                   // 0x2184
};

#endif
