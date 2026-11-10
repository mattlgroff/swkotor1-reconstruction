// Worker O matching headers (copied from bucket l): the character creation frame
// CSWGuiMainCharGen (swguimainchargen.o, layout "MAINCG", vtable 0x007592A8,
// 0x22DC bytes) and its step sub-panels CSWGuiQuickOrCustomPanel (0xD98,
// vtable 0x00759710), CSWGuiQuickPanel (0x1360, vtable 0x00759668) and
// CSWGuiCustomPanel (0x2188, vtable 0x007595E0). Builds on the level-up
// summary header (same "MAINCG" layout, same creature/stat interfaces).
#ifndef O_SW_GUI_MAIN_CHARGEN_H
#define O_SW_GUI_MAIN_CHARGEN_H

#include "o/sw_gui_levelup_chargen.h"

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
    virtual void v108(); virtual void v10c();
    virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11c();
    virtual void v120(); virtual void v124();
    virtual void SetAnimationBlend(int b);          // 0x128 (descriptive)
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

#ifdef O_MAIN_CHARGEN_DTOR
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

// A CExoArrayList member with the inline constructor and destructor (the
// shared o CExoArrayList declares neither).
template <class T>
class CExoArrayListM {
public:
    T* element;
    int num;
    int array_size;
    CExoArrayListM()
    {
        num = 0;
        array_size = 0;
        element = 0;
    }
    ~CExoArrayListM()
    {
        delete[] element;
        element = 0;
    }
    // Out-of-line copies: <int> 0x005E03C0, <unsigned short> 0x004D2FA0.
    void Allocate(int nSize)
    {
        T* pOld = element;
        array_size = nSize;
        element = new T[nSize];
        for (int i = 0; i < num; i++) {
            element[i] = pOld[i];
        }
        if (pOld) {
            delete[] pOld;
        }
    }
    T& operator[](int i) { return element[i]; }
    // SetSize that also releases the buffer for a zero size.
    void SetSize(int nSize)
    {
        if (nSize == 0) {
            if (element) {
                delete[] element;
                element = 0;
                array_size = 0;
            }
        } else {
            Allocate(nSize);
        }
        num = nSize;
    }
    // Out-of-line copies: <unsigned long> 0x00616940, <unsigned short> 0x006169A0.
    void Insert(T t, int nPos)
    {
        int i = num;
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 0x10);
        }
        element[num] = t;
        num++;
        for (; i > nPos; i--) {
            element[i] = element[i - 1];
        }
        element[nPos] = t;
    }
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 0x10);
        }
        element[num] = t;
        num++;
    }
    int IndexOf(T t)
    {
        for (int i = 0; i < num; i++) {
            if (element[i] == t) {
                return i;
            }
        }
        return -1;
    }
    void DelIndex(int nIndex)
    {
        num--;
        for (; nIndex < num; nIndex++) {
            element[nIndex] = element[nIndex + 1];
        }
    }
};

// A control whose hilight (SetFlag0) is switched off when the time runs out.
struct CSWGuiControlTimer {
    CSWGuiControl* m_pControl;
    float m_fTime;
};

// Portrait and name pickers opened by the quick/custom panels; the second
// SetParentPanel argument is 1 for the quick panel, 2 for the custom panel.
// CSWGuiPortraitCharGen: swguiportraitchargen.o, layout "PORTCUST", vtable
// 0x00759EA8, 0x1240 bytes.
#ifdef O_PORTRAIT_CHARGEN_DTOR
class __declspec(novtable) CSWGuiPortraitCharGen : public CSWGuiPanel {
#else
class CSWGuiPortraitCharGen : public CSWGuiPanel {
#endif
public:
    CSWGuiPortraitCharGen(CSWGuiManager* pManager, CSWCCreature* pCreature);  // 0x006F9430
    ~CSWGuiPortraitCharGen();                       // 0x006F9A50
    virtual void Draw(float fDelta);                // 0x34 (0x006F8C60)
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006F8FF0)
    virtual void OnPanelAdded();                    // 0x48 (0x006F89D0)
    void SetParentPanel(CSWGuiPanel* pParent, unsigned char nType);         // 0x006F8A00
    void OnSelectDisplayButton();                   // 0x006F8A20
    void UpdatePortraitButton();                    // 0x006F8AD0
    void HandleCancelButton();                      // 0x006F8F40
    void HandleRightButton();                       // 0x006F8FC0
    void GetPortraitsList(int nGender);             // 0x006F90F0
    // Button events; identical code folded with other panels' handlers.
    void OnLeftArrow(CSWGuiControl* pControl);      // 0x0067CB40 (HandleInputEvent(0x2f, 1))
    void OnRightArrow(CSWGuiControl* pControl);     // 0x0067CB50 (HandleInputEvent(0x30, 1))
    CSWCLevelUpStatsL* GetStats() { return (CSWCLevelUpStatsL*)m_pCreature->m_pStats; }
    CSWCCreatureVirtuals* GetCreatureV() { return (CSWCCreatureVirtuals*)m_pCreature; }
    CSWCCreature* m_pCreature;                      // 0x64
    CSWGuiPanel* m_pParent;                         // 0x68 quick or custom panel
    CSWGuiLabel m_lblTitle;                         // 0x6c MAIN_TITLE_LBL
    CSWGuiLabel m_lblSubTitle;                      // 0x1ac SUB_TITLE_LBL
    CSWGuiLabel m_lblPortrait;                      // 0x2ec LBL_PORTRAIT
    CSWGuiLabel m_aBevels[5];                       // 0x42c LBL_BEVEL_L/M/R/T/B
    CSWGui3DSceneView m_cHead;                      // 0xa6c LBL_HEAD
    CSWGuiButton m_btnAccept;                       // 0xafc BTN_ACCEPT
    CSWGuiButton m_btnBack;                         // 0xcc0 BTN_BACK
    CSWGuiButton m_btnRight;                        // 0xe84 BTN_ARRR
    CSWGuiButton m_btnLeft;                         // 0x1048 BTN_ARRL
    CExoArrayListM<int> m_lstPortraits;             // 0x120c portraits.2da rows
    CExoArrayListM<int> m_lstAppearances;           // 0x1218 head appearance per row
    CExoArrayListM<CSWGuiControlTimer> m_lstTimers; // 0x1224 (descriptive)
    float m_fIdleTimer;                             // 0x1230 time to the next idle animation
    int m_nCurrent;                                 // 0x1234 index into m_lstPortraits
    int m_nOriginalPortraitId;                      // 0x1238 restored on cancel
    unsigned char m_nParentType;                    // 0x123c 1 quick, 2 custom
};

class CSWGuiEditText {
public:
    void SetText(const CExoString& sText);          // 0x00416490
    char pad_0x0[0x70];
    short m_nMaxLength;                             // 0x70
    unsigned long m_bFlag0 : 1;                     // 0x74
    CExoString m_sText;                             // 0x78
};

// vtable 0x0073E9A0 family; 0x160 bytes.
class CSWGuiEditbox : public CSWGuiControl {
public:
    CSWGuiEditbox();                                // 0x0041C6B0
    ~CSWGuiEditbox();                               // 0x0041C740
    void SetFocus();                                // 0x00418660
    char pad_0x5c[0xe0 - 0x5c];
    CSWGuiEditText m_cText;                         // 0xe0
};

// The random name generator (a CResHelper<CResLTR, 2036>, see
// o/sw_name_gen.h); only its interface is needed here.
class CSWNameGen {
public:
    CSWNameGen();                                   // 0x00711290
    virtual ~CSWNameGen();                          // 0x007112E0
    // nNameParts: 1 male first name, 2 female first name, 4 last name.
    CExoString GetRandomName(unsigned short nRace, unsigned char nNameParts,
                             unsigned char nMaxLength);    // 0x007118C0
    char pad_0x4[0x24 - 0x4];
};

extern const unsigned short RACIAL_TYPE_HUMAN;     // 0x0074BF14

#ifdef O_NAME_CHARGEN_DTOR
class __declspec(novtable) CSWGuiNameChargen : public CSWGuiPanel {
#else
class CSWGuiNameChargen : public CSWGuiPanel {
#endif
public:
    CSWGuiNameChargen(CSWGuiManager* pManager, CSWCCreature* pCreature);  // 0x006F9E70
    ~CSWGuiNameChargen();                           // 0x006FA2E0
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006FA220)
    virtual void OnPanelAdded();                    // 0x48 (0x006FA140)
    void SetParentPanel(CSWGuiPanel* pParent, unsigned char nType);         // 0x006F9B80
    void HandleCancelButton();                      // 0x006F9BA0
    void HandleRandomName();                        // 0x006F9BF0
    void HandleDoneButton(CSWGuiControl* pControl); // 0x006F9CD0
    CSWCCreature* m_pCreature;                      // 0x64
    CSWGuiPanel* m_pParent;                         // 0x68 quick or custom panel
    CSWGuiButton m_btnDone;                         // 0x6c END_BTN
    CSWGuiEditbox m_editName;                       // 0x230 NAME_BOX_EDIT
    CSWGuiLabel m_lblTitle;                         // 0x390 MAIN_TITLE_LBL
    CSWGuiLabel m_lblSubTitle;                      // 0x4d0 SUB_TITLE_LBL
    CSWGuiButton m_btnBack;                         // 0x610 BTN_BACK
    CSWGuiButton m_btnRandom;                       // 0x7d4 BTN_RANDOM
    unsigned char m_nParentType;                    // 0x998 1 quick, 2 custom
    unsigned long m_bRandomName : 1;                // 0x99c (descriptive)
    CSWNameGen m_cNameGen;                          // 0x9a0
};

// Step panels (layouts "QUICKORCUSTOM", "QUICKPNL", "CUSTPNL").
#ifdef O_QUICK_OR_CUSTOM_DTOR
class __declspec(novtable) CSWGuiQuickOrCustomPanel : public CSWGuiPanel {
#else
class CSWGuiQuickOrCustomPanel : public CSWGuiPanel {
#endif
public:
    CSWGuiQuickOrCustomPanel(CSWGuiManager* pManager, CSWCCreature* pCreature);   // 0x006F09F0
    ~CSWGuiQuickOrCustomPanel();                    // 0x006F0D50
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006F0E10)
    void HandleCancelButton();                      // 0x006F07D0
    void OnSelectQuickCharacterButton(CSWGuiControl* pControl);     // 0x006F0800
    void OnSelectCustomCharacterButton(CSWGuiControl* pControl);    // 0x006F0830
    void SetDescription(unsigned long nStrRef);     // 0x006F0860
    void OnEnterQuickCharacterButton(CSWGuiControl* pControl);      // 0x006F09D0
    void OnEnterCustomCharacterButton(CSWGuiControl* pControl);     // 0x006F09E0
    CSWCCreature* m_pCreature;                      // 0x64
    CSWGuiMainCharGen* m_pParent;                   // 0x68
    CSWGuiButton m_btnQuick;                        // 0x6c QUICK_CHAR_BTN
    CSWGuiButton m_btnCustom;                       // 0x230 CUST_CHAR_BTN
    CSWGuiLabel m_lblDesc;                          // 0x3f4 (description item)
    CSWGuiLabel m_lblDecoration;                    // 0x534 LBL_DECORATION
    CSWGuiLabel m_lblRAnalog;                       // 0x674 LBL_RANALOG
    CSWGuiLabel m_lblRBG;                           // 0x7b4 LBL_RBG
    CSWGuiListBox m_lbDesc;                         // 0x8f4 LB_DESC
    CSWGuiButton m_btnBack;                         // 0xbd4 BTN_BACK
};

#ifdef O_QUICK_PANEL_DTOR
class __declspec(novtable) CSWGuiQuickPanel : public CSWGuiPanel {
#else
class CSWGuiQuickPanel : public CSWGuiPanel {
#endif
public:
    CSWGuiQuickPanel(CSWGuiManager* pManager, CSWCCreature* pCreature);   // 0x006F0390
    ~CSWGuiQuickPanel();                            // 0x006F0700
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006F0280)
    virtual void OnPanelAdded();                    // 0x48 (0x006F0260)
    void ConfirmCancel(CSWGuiControl* pControl);    // 0x006F0220 (descriptive)
    void OnCancelPressed(CSWGuiControl* pControl);  // 0x006F0310
    void SetState(int nState);                      // 0x006EFC10
    void OnSelectPlay(CSWGuiControl* pControl);     // 0x006EFD60
    void OnSelectName(CSWGuiControl* pControl);     // 0x006EFD80
    void OnSelectPortrait(CSWGuiControl* pControl); // 0x006EFE10
    void CancelledByChild();                        // 0x006EFF10
    void PortraitCreationCompleted();               // 0x006EFF60
    void NameCreationCompleted();                   // 0x006EFF80
    CExoString GetCurrentName();                    // 0x006EFFA0
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

#ifdef O_CUSTOM_PANEL_DTOR
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
    void PortraitCreationCompleted();               // 0x006EF4C0
    void NameCreationCompleted();                   // 0x006EF4E0
    void AbilityCreationCompleted();                // 0x006EF500
    void SkillCreationCompleted();                  // 0x006EF520
    void FeatCreationCompleted();                   // 0x006EF530
    CExoString GetCurrentName();                    // folded into 0x006EFFA0
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
