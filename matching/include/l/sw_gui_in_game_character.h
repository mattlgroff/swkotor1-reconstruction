// Worker L matching headers: the in-game character screen
// CSWGuiInGameCharacter (vtable 0x00756100, 0x59F8 bytes, 0x006AE990-
// 0x006B2250): stat labels, party-member portrait buttons, the 3D preview
// and the level-up entry points. Member order follows the destructor.
#ifndef L_SW_GUI_IN_GAME_CHARACTER_H
#define L_SW_GUI_IN_GAME_CHARACTER_H

#include "l/sw_gui_script_select.h"

extern Vector g_vDifficultyExitColor;               // 0x007A23B4

Vector operator*(const Quaternion& q, const Vector& v);     // 0x004A9BB0
Quaternion YawPitchRoll(float fYaw, float fPitch, float fRoll);     // 0x004ACAC0

// vtable 0x00756060: the alignment bar, a slider the player cannot drag.
// The destructor TU declares it novtable: 0x006AF940 is a bare jmp to
// ~CSWGuiSlider with no vptr restoration.
#if defined(L_SLIDER_READONLY_DTOR_TU) || defined(L_CHARACTER_VIEW_TU)
class __declspec(novtable) CSWGuiSliderReadOnly : public CSWGuiSlider {
#else
class CSWGuiSliderReadOnly : public CSWGuiSlider {
#endif
public:
    CSWGuiSliderReadOnly();                         // inlined into 0x006B0E40
#ifdef L_SLIDER_READONLY_DTOR_TU
    ~CSWGuiSliderReadOnly();                        // 0x006AF940
#else
    // Implicit (inline): ~CSWGuiInGameCharacter's body calls ~CSWGuiSlider
    // directly (novtable there: no vptr restore); only unwind funclets and
    // ??_G call the copy at 0x006AF940.
    ~CSWGuiSliderReadOnly() {}
#endif
};

// vtable 0x007533C8: auto-repeat button (full declaration in k/m pazaak).
#ifdef L_CHARACTER_VIEW_TU
class __declspec(novtable) CSWGuiSpeedButton : public CSWGuiButton {
public:
    ~CSWGuiSpeedButton() {}                         // folded 0x00644A30
#else
class CSWGuiSpeedButton : public CSWGuiButton {
public:
#endif
    float m_fRepeatDelay;                           // 0x1c4
    float m_fInitialDelay;                          // 0x1c8
    float m_fTimer;                                 // 0x1cc
    unsigned long m_bLeftButton : 1;                // 0x1d0
};

class CSWGuiInGameCharacter : public CSWGuiPanel {
public:
    ~CSWGuiInGameCharacter();                      // 0x006AF970
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006B2250)
    void SetModText(CSWGuiLabel* pLabel, int nMod);  // 0x006AE990
    void AttachModel();                             // 0x006AEBF0
    void DoAutoLevelup(CSWGuiControl* pControl);    // 0x006AEC80
    void SetLabelBonusPenaltyColour(CSWGuiLabel* pLabel, int nValue);   // 0x006AECD0
    void RotateCharacter(int bLeft);                // 0x006AED20
    void RotateCharacterLeft(CSWGuiControl* pControl);      // 0x006AED90
    void RotateCharacterRight(CSWGuiControl* pControl);     // 0x006AEDE0
    void UpdatePortraits();                         // 0x006AEE30
    void OnChangeCharacter(CSWGuiControl* pControl);    // 0x006AF350
    void OnSwitchLeft(CSWGuiControl* pControl);     // 0x006AF450
    void OnSwitchRight(CSWGuiControl* pControl);    // 0x006AF6D0
    void SetStats();                                // 0x006AFDA0
    int ShowLevelUpGUI(int bLevelUp);               // 0x006B0BB0
    void StartAutoLevelup();                        // 0x006B0CB0

    // The preview creature seen through its virtual interface.
    CSWCCreatureVirtuals* GetPreviewV() { return (CSWCCreatureVirtuals*)m_pPreview; }

    CSWGuiLabel m_lblName;                          // 0x64
    CSWGuiLabel m_lblClass;                         // 0x1a4
    CSWGuiLabel m_lblClass1;                        // 0x2e4
    CSWGuiLabel m_lblClass2;                        // 0x424
    CSWGuiLabel m_lblLevel;                         // 0x564
    CSWGuiLabel m_lblLevel1;                        // 0x6a4
    CSWGuiLabel m_lblLevel2;                        // 0x7e4
    CSWGuiLabel m_lblFort;                          // 0x924
    CSWGuiLabel m_lblRef;                           // 0xa64
    CSWGuiLabel m_lblWill;                          // 0xba4
    CSWGuiLabel m_lblFortStat;                      // 0xce4
    CSWGuiLabel m_lblRefStat;                       // 0xe24
    CSWGuiLabel m_lblWillStat;                      // 0xf64
    CSWGuiLabel m_lblExp;                           // 0x10a4
    CSWGuiLabel m_lblExpStat;                       // 0x11e4
    CSWGuiLabel m_lblNextLevel;                     // 0x1324
    CSWGuiLabel m_lblNeededXP;                      // 0x1464
    CSWGuiLabel m_lblDefenseStat;                   // 0x15a4
    CSWGuiLabel m_lblForceStat;                     // 0x16e4
    CSWGuiLabel m_lblVitalityStat;                  // 0x1824
    CSWGuiLabel m_lblDefense;                       // 0x1964
    CSWGuiLabel m_lblForce;                         // 0x1aa4
    CSWGuiLabel m_lblVitality;                      // 0x1be4
    CSWGuiLabel m_lblStr;                           // 0x1d24
    CSWGuiLabel m_lblStrength;                      // 0x1e64
    CSWGuiLabel m_lblStrMod;                        // 0x1fa4
    CSWGuiLabel m_lblWis;                           // 0x20e4
    CSWGuiLabel m_lblWisdom;                        // 0x2224
    CSWGuiLabel m_lblWisMod;                        // 0x2364
    CSWGuiLabel m_lblCha;                           // 0x24a4
    CSWGuiLabel m_lblCharisma;                      // 0x25e4
    CSWGuiLabel m_lblChaMod;                        // 0x2724
    CSWGuiLabel m_lblInt;                           // 0x2864
    CSWGuiLabel m_lblIntelligence;                  // 0x29a4
    CSWGuiLabel m_lblIntMod;                        // 0x2ae4
    CSWGuiLabel m_lblCon;                           // 0x2c24
    CSWGuiLabel m_lblConstitution;                  // 0x2d64
    CSWGuiLabel m_lblConMod;                        // 0x2ea4
    CSWGuiLabel m_lblDex;                           // 0x2fe4
    CSWGuiLabel m_lblDexterity;                     // 0x3124
    CSWGuiLabel m_lblDexMod;                        // 0x3264
    CSWGuiLabel m_lblBevel;                         // 0x33a4
    CSWGuiLabel m_lblBevel2;                        // 0x34e4
    CSWGuiLabel m_alblGoods[10];                    // 0x3624
    CSWGuiLabel m_lblMore;                          // 0x42a4
    CSWGuiLabel m_lblAdorn;                         // 0x43e4
    CSWGuiBlackenedLabel m_lblLight;                // 0x4524 (vtable 0x00753CC0)
    CSWGuiBlackenedLabel m_lblDark;                 // 0x4664 (vtable 0x00753CC0)
    CSWGuiButton m_btnAutoLevel;                    // 0x47a4
    CSWGuiButton m_btnLevelUp;                      // 0x4968
    CSWGuiButton m_btnChange1;                      // 0x4b2c
    CSWGuiButton m_btnChange2;                      // 0x4cf0
    CSWGuiButton m_btnCharLeft;                     // 0x4eb4
    CSWGuiButton m_btnCharRight;                    // 0x5078
    CSWGuiButton m_btnExit;                         // 0x523c
    CSWGuiButton m_btnScripts;                      // 0x5400
    CSWGuiSliderReadOnly m_sldAlign;                // 0x55c4 (vtable 0x00756060)
    CSWGui3DSceneView m_view3DChar;                 // 0x577c
    CSWGuiSpeedButton m_btn3DChar;                  // 0x580c (vtable 0x007533C8)
    char pad_0x59e0[0x59e4 - 0x59e0];
    CSWCCreature* m_pPreview;                       // 0x59e4
    char pad_0x59e8[0x59f0 - 0x59e8];
    char m_nPartyMember;                            // 0x59f0
    char pad_0x59f1[0x59f4 - 0x59f1];
    CSWGuiScriptSelect* m_pScriptSelect;            // 0x59f4
};

#endif // L_SW_GUI_IN_GAME_CHARACTER_H
