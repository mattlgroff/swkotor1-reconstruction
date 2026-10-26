// CSWGuiLevelUpCharGen (vtable 0x00758F50, 0x2560 bytes, layout "MAINCG")
// and the step panel CSWGuiLevelUpPanel (vtable 0x00759568, 0x1CEC bytes).
#ifndef L_SW_GUI_LEVELUP_CHARGEN_H
#define L_SW_GUI_LEVELUP_CHARGEN_H

#include "l/sw_gui_skill_info.h"

class CSWGuiInGameCharacter : public CSWGuiPanel {
public:
    void AttachModel();                             // inferred, 0x006AEBF0
};

// The level-up preview writes through the level-up statistics interface of
// the live creature's client stats.
class CSWCLevelUpStats : public CSWCCreatureStats {
public:
    void SetSkillRankBase(unsigned short nSkill, char nRank);  // 0x006488E0
    void SetFortSavingThrow();                      // 0x00648B30
    void SetWillSavingThrow();                      // 0x00648BE0
    void SetReflexSavingThrow();                    // 0x00648C90
    void SetDEXBase(unsigned char nDEX, int bApply);    // 0x0064A100
    void SetClassLevel(unsigned char nIndex, unsigned char nLevel);   // 0x0064A2A0
    void SetSTRBase(unsigned char nSTR);            // 0x0064A0D0
    void SetCONBase(unsigned char nCON);            // 0x006496D0
    void SetWISBase(unsigned char nWIS);            // 0x00649770
    void SetINTBase(unsigned char nINT);            // 0x006478A0
    void SetCHABase(unsigned char nCHA);            // 0x006478D0
    void SetClass(unsigned char nIndex, unsigned char nClass);    // 0x0064A1F0
    int GetBaseAttackBonus();                       // 0x006479F0
    void GetPackageFeatsLeft(unsigned char* pnNormal, unsigned char* pnBonus);   // 0x00648FE0
    void AddSkillPointsPerLevel();                  // 0x00648A00
    unsigned char ComputeNumberKnownSpellsLeft();   // 0x00649070
    void AddGrantedFeats(CExoArrayListL<int>* pList);  // 0x00649950
};

// Virtual destructor of the client creature (slot 0); the shared
// CSWCCreature declaration carries no vtable.
class CSWCCreatureDeletable {
public:
    virtual ~CSWCCreatureDeletable();
};

class Quaternion {
public:
    Quaternion(float fX, float fY, float fZ, float fW)
    {
        x = fX;
        y = fY;
        z = fZ;
        w = fW;
    }
    float x, y, z, w;
};

class CAurScene {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void SetRoom(const char* sRoom, const Vector& vPosition,
                         const Quaternion& qOrientation);   // 0x70 (descriptive)
};

// A camera hook part: +0x3C position, +0x48 enabled.
class CAurCameraHook {
public:
    char pad_0x0[0x3c];
    Vector m_vPosition;                             // 0x3c
    int m_bEnabled;                                 // 0x48
};

class CAurCamera {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual CAurCameraHook* GetHook(int nHook);     // 0x80 (descriptive)
};

class CAurObject;

// Model scene drawable (vtable 0x0073E3F0), 0x34 bytes.
class CSWGuiScene {
public:
    CSWGuiScene();                                  // 0x004174B0
    ~CSWGuiScene();
    CAurObject* AddModel(const CExoString& sModel, int nIndex);    // 0x00417620
    void* m_pVtable;
    char pad_0x4[0x10];
    CAurScene* m_pScene;                            // 0x14
    CAurCamera* m_pCamera;                          // 0x18
    char pad_0x1c[0x34 - 0x1c];
};

extern int g_bDo3DGui;                              // 0x0078D1E4 (descriptive)

// Inline 3D model view control (vtable 0x00752E30); dtor 0x0067AC50, 0x90 bytes.
class CSWGui3DSceneView : public CSWGuiControl {
public:
    CSWGui3DSceneView() {}
    ~CSWGui3DSceneView();                           // 0x0067AC50
    CSWGuiScene m_cScene;                           // 0x5c
};

class CSWCMessage {
public:
    // Sends the finished level-up (old and new stats) to the server.
    void SendPlayerToServerLevelUp(CSWCCreatureStats* pOld,
                                   CSWCCreatureStats* pNew);  // 0x00678280 (descriptive)
};

class CExoSound {
public:
    void SetSoundMode(int nMode);                   // 0x005D5E80
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC
extern int g_nSoundModeZero;                        // 0x0074C5EC
extern int g_bGuiFlag;                              // 0x007A2370
extern Vector g_vDifficultyExitColor;               // 0x007A23B4
extern Vector g_vKeyMapLockedTextColor;             // 0x007A2408

class CSWGuiLevelUpPanel;

class CSWGuiLevelUpCharGen : public CSWGuiPanel {
public:
    CSWGuiLevelUpCharGen(CSWGuiManager* pManager, unsigned long oidCreature,
                         CSWCCreature* pPreview);   // 0x006E8EF0
    ~CSWGuiLevelUpCharGen();                        // 0x006E7B70
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006E9A50)
    virtual void OnPanelAdded();                    // 0x48 (0x006E7B40)

    CSWCCreature* m_pSnapshot;                      // 0x64
    CSWGuiInGameCharacter* m_pCharacter;            // 0x68
    CSWGuiLevelUpPanel* m_pLevelUpPanel;            // 0x6C
    CSWGuiLabel m_lblTitle;                         // 0x70 MAIN_TITLE_LBL
    CSWGuiLabel m_lblName;                          // 0x1B0
    CSWGuiLabel m_lblClass;                         // 0x2F0
    CSWGuiLabel m_lblLevel;                         // 0x430
    CSWGuiLabel m_lblLevelVal;                      // 0x570
    CSWGuiLabel m_lblStr;                           // 0x6B0
    CSWGuiLabel m_lblDex;                           // 0x7F0
    CSWGuiLabel m_lblCon;                           // 0x930
    CSWGuiLabel m_lblInt;                           // 0xA70
    CSWGuiLabel m_lblWis;                           // 0xBB0
    CSWGuiLabel m_lblCha;                           // 0xCF0
    CSWGuiLabel m_lblStrAb;                         // 0xE30
    CSWGuiLabel m_lblDexAb;                         // 0xF70
    CSWGuiLabel m_lblConAb;                         // 0x10B0
    CSWGuiLabel m_lblIntAb;                         // 0x11F0
    CSWGuiLabel m_lblWisAb;                         // 0x1330
    CSWGuiLabel m_lblChaAb;                         // 0x1470
    CSWGuiLabel m_lblVit;                           // 0x15B0
    CSWGuiLabel m_lblDef;                           // 0x16F0
    CSWGuiLabel m_lblNewWill;                       // 0x1830
    CSWGuiLabel m_lblNewRefl;                       // 0x1970
    CSWGuiLabel m_lblNewFort;                       // 0x1AB0
    CSWGuiLabel m_lblBevelL;                        // 0x1BF0
    CSWGuiLabel m_lblBevelM;                        // 0x1D30
    CSWGuiLabel m_lblBevelR;                        // 0x1E70
    CSWGuiLabel m_lblFortitude;                     // 0x1FB0
    CSWGuiLabel m_lblReflex;                        // 0x20F0
    CSWGuiLabel m_lblWill;                          // 0x2230
    CSWGui3DSceneView m_cModel;                     // 0x2370 MODEL_LBL
    CSWGuiLabel m_lblPortrait;                      // 0x2400
    char pad_0x2540[4];
    int m_nInputClass;                              // 0x2544
    CExoString m_sClassName;                        // 0x2548
    CExoString m_sName;                             // 0x2550
    char pad_0x2558[4];
    unsigned long m_oidCreature;                    // 0x255C

    CSWCCreatureStats* GetSnapshotStats() { return m_pSnapshot->m_pStats; }
    CSWCCreature* GetLiveCreature()
    {
        return g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_oidCreature);
    }

    void DetermineClass();                          // 0x006E7DD0
    void SetNameString(const CExoString& sName);    // 0x006E7EC0
    void AbilitiesChanged();                        // 0x006E7F70
    void NewAbilityScores(unsigned int nStr, unsigned int nCha, unsigned int nInt,
                          unsigned int nWis, unsigned int nCon, unsigned int nDex,
                          unsigned int nPoints);    // 0x006E8500
    void ClearAbilities();                          // 0x006E85A0
    void ClearSkills();                             // 0x006E8730
    void ClearPowers();                             // 0x006E87E0
    void BeginPlayGame();                           // 0x006E88A0
    void ClearFeats();                              // 0x006E8DB0
    void CancelLevelUp();                           // 0x006E8E20
    int CopyCreatureStats(CSWCCreature* pFrom, CSWCCreature* pTo); // 0x006E89F0
};

class CSWGuiLevelUpPanel;

// The four step screens opened by the level-up panel; each keeps the
// panel that opened it at +0x68.
class CSWGuiLevelUpChild : public CSWGuiPanel {
public:
    char pad_0x64[4];
    CSWGuiLevelUpPanel* m_pLevelUpPanel;            // 0x68
};

class CSWGuiPowersLevelUp : public CSWGuiLevelUpChild {
public:
    CSWGuiPowersLevelUp(CSWGuiManager* pManager, unsigned long oidCreature);  // 0x006F2180
    char pad_0x6c[0x1A10 - 0x6C];
};

class CSWGuiFeatsCharGen : public CSWGuiLevelUpChild {
public:
    CSWGuiFeatsCharGen(CSWGuiManager* pManager, CSWCCreature* pCreature, int bLevelUp,
                       int bBonusOnly);             // 0x006F3D60
    char pad_0x6c[0x1A1C - 0x6C];
};

class CSWGuiSkillsCharGen : public CSWGuiLevelUpChild {
public:
    CSWGuiSkillsCharGen(CSWGuiManager* pManager, CSWCCreature* pCreature, int bLevelUp);  // 0x006F51D0
    char pad_0x6c[0x49D0 - 0x6C];
};

class CSWGuiAbilitiesCharGen : public CSWGuiLevelUpChild {
public:
    CSWGuiAbilitiesCharGen(CSWGuiManager* pManager, CSWCCreature* pCreature, int bLevelUp);  // 0x006F7600
    char pad_0x6c[0x3DF4 - 0x6C];
};

// The level-up step list (layout "LEVELUPPNL"): abilities, skills, feats,
// powers, accept. The parent at +0x68 and current step at +0x1CCC are
// documented in the original GUI reconstruction.
#ifdef L_LEVELUP_PANEL_DTOR
class __declspec(novtable) CSWGuiLevelUpPanel : public CSWGuiPanel {
#else
class CSWGuiLevelUpPanel : public CSWGuiPanel {
#endif
public:
    CSWGuiLevelUpPanel(CSWGuiManager* pManager, unsigned long oidCreature);    // 0x006EE7D0
    ~CSWGuiLevelUpPanel();                          // 0x006EEE90

    CSWCCreature* m_pCreature;                      // 0x64
    CSWGuiLevelUpCharGen* m_pParent;                // 0x68
    CSWGuiLabel m_lblBG;                            // 0x6C
    CSWGuiLabelHilight m_aHilights[5];              // 0x1AC
    CSWGuiLabel m_aNumbers[5];                      // 0xA30
    CSWGuiButton m_aSteps[5];                       // 0x1070
    CSWGuiButton m_btnBack;                         // 0x1944
    CSWGuiButton m_btnCancel;                       // 0x1B08
    int m_nState;                                   // 0x1CCC
    int m_nLastState;                               // 0x1CD0
    int m_aStepEnabled[5];                          // 0x1CD4
    int m_bCanCancel;                               // 0x1CE8

    void NewAbilityScores(unsigned int strength, unsigned int charisma,
                          unsigned int intelligence, unsigned int wisdom,
                          unsigned int constitution, unsigned int dexterity,
                          unsigned int points);     // 0x006EE0D0
    void ChangeState(int bBack);                    // 0x006EE0E0
    void OnSelectPowersButton(CSWGuiControl* pControl);     // 0x006EE350
    void OnSelectFeatsButton(CSWGuiControl* pControl);      // 0x006EE3D0
    void OnSelectSkillsButton(CSWGuiControl* pControl);     // 0x006EE480
    void OnSelectAbilitiesButton(CSWGuiControl* pControl);  // 0x006EE500
    void CancelledByChild();                        // 0x006EE580
    void AbilitySelectionCompleted();               // 0x006EE5B0
    void PowersSelectionCompleted();                // 0x006EE5D0
    void OnCancelPressed(CSWGuiControl* pControl);  // 0x006EE5F0
    void SetCanCancel(int bCanCancel);              // 0x006EE640
    void OnSelectAcceptButton(CSWGuiControl* pControl);     // 0x006EE780
    virtual void OnPanelAdded();                    // 0x48 (0x006EE6C0)

    CSWCLevelUpStats* GetLevelUpStats() { return (CSWCLevelUpStats*)m_pCreature->m_pStats; }
    // Shows step nStep with the next step number.
    __forceinline void EnableStep(int nStep, void (CSWGuiPanel::*pfnCallback)(CSWGuiControl*), int& nNumber)
    {
        m_aSteps[nStep].AddEvent(0x27, this, pfnCallback);
        m_aSteps[nStep].AddEvent(0x2D, this, pfnCallback);
        m_aStepEnabled[nStep] = 1;
        CExoString sNumber(++nNumber);
        m_aNumbers[nStep].m_cText.SetText(sNumber);
    }
    __forceinline void DisableStep(int nStep)
    {
        HideControl(&m_aSteps[nStep]);
        HideControl(&m_aHilights[nStep]);
        HideControl(&m_aNumbers[nStep]);
        m_aStepEnabled[nStep] = 0;
    }
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006EE720)
};

#endif
