// Worker L matching headers: the tutorial message box CSWGuiTutorialBox
// (vtable 0x00755CD0, 0x006AA040-0x006AA8A0), a CSWGuiMessageBox that shows
// the "Message%i" columns of tutorial.2da and then runs the deferred action.
#ifndef L_SW_GUI_TUTORIAL_BOX_H
#define L_SW_GUI_TUTORIAL_BOX_H

#include "l/sw_gui_bark_bubble.h"

class C2DA {
public:
    C2DA(CResRef cResRef, int bLoad);               // 0x00413CC0
    virtual ~C2DA();
    int Load2DArray();                              // 0x004143B0
    void Unload2DArray();                           // 0x004139E0
    int GetCExoStringEntry(int nRow, const CExoString& sColumn, CExoString* pValue);  // 0x00413270
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);   // 0x00413660
    int GetINTEntry(const CExoString& sRow, const CExoString& sColumn, int* pValue);   // 0x00414110
    char pad_0x4[0x20];
    int m_nNumRows;                                 // 0x24
    char pad_0x28[0x2c];
};

class CTwoDimArrays {
public:
    char pad_0x0[0xc];
    C2DA* m_pAppearanceTable;                       // 0x0c
    C2DA* GetAppearanceTable() { return m_pAppearanceTable; }
    char pad_0x10[0x68 - 0x10];
    C2DA* m_pKeymapTable;                           // 0x68
    C2DA* m_pBindableKeysTable;                     // 0x6c
    char pad_0x70[0xc];
    C2DA* m_pDifficultyTable;                       // 0x7c (difficultyopt)
    char pad_0x80[0x98];
    C2DA* m_pTutorialTable;                         // 0x118
    C2DA* GetTutorialTable() { return m_pTutorialTable; }
    C2DA* GetKeymapTable() { return m_pKeymapTable; }
    C2DA* GetBindableKeysTable() { return m_pBindableKeysTable; }
    C2DA* GetDifficultyTable() { return m_pDifficultyTable; }
};

class CSWSpellArray;
class CSWFeat;
class CSWSkill;
class CSWClass;

class CSWRules {
public:
    CSWFeat* GetFeat(unsigned short nFeat);         // 0x00550C00
    char pad_0x0[0x34];
    class CSWBaseItemArrayL* m_pBaseItems;          // 0x34
    char pad_0x38[0x54];
    CSWSpellArray* m_pSpellArray;                   // 0x8c
    char pad_0x90[0x10];
    unsigned short* m_pFeatIds;                     // 0xa0 (sorted feat list)
    unsigned short m_nNumFeats;                     // 0xa4
    unsigned short m_nNumFeatIds;                   // 0xa6
    char pad_0xa8[0x3];
    unsigned char m_nNumSkills;                     // 0xab
    CSWClass* m_pClasses;                           // 0xac
    char pad_0xb0[0x4];
    CSWSkill* m_pSkills;                            // 0xb4
    CTwoDimArrays* m_p2DArrays;                     // 0xb8
    CTwoDimArrays* Get2DArrays() { return m_p2DArrays; }
    unsigned short GetFeatId(unsigned short nIndex) { return m_pFeatIds[nIndex]; }
    unsigned char GetNumSkills() { return m_nNumSkills; }
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CSWSCreature;
class CSWCCreatureStats;

class CSWCCreature : public CSWCObject {
public:
    CSWCCreature();                                 // 0x00616A20
    CSWSCreature* AsServerCreature();               // 0x0060FB20
    class CSWCAnimBase* GetAnimBase();              // 0x0063C070
    void CGPauseCycle(float fDelta, int bReset);    // 0x0060F7C0
    void SetAppearance(struct CSWCCreatureAppearance* pAppearance, int nPart, int bLoad);  // 0x006134C0
    char pad_0x30[0x21c - 0x30];
    struct CSWCCreatureAppearance* m_pAppearance;   // 0x21c (0x3c-byte appearance block)
    char pad_0x220[0x2d4 - 0x220];
    unsigned long m_bSoloFlag : 1;                  // 0x2d4 bit 0 (descriptive)
    char pad_0x2d8[0x2f8 - 0x2d8];
    CSWCCreatureStats* m_pStats;                    // 0x2f8
    CSWCCreatureStats* GetClientStats() { return m_pStats; }
    char pad_0x2fc[0x144];
    unsigned long m_n440;                           // 0x440 (placeholder name)
    char pad_0x444[0x8];

    void SetUp3DScene(class CSWGui3DSceneView* pView, CSWCCreature* pSource, int a, int b);  // 0x006100F0 (descriptive)

    void PerformUseItem(unsigned long oidItem, int a, unsigned long oidTarget,
                        const Vector& vTarget);     // 0x0060EE60
    void PerformCastSpell(int nSpell, unsigned long oidTarget, const Vector& vTarget,
                          unsigned char nMeta, int a, int b, int c);   // 0x0060EF10
    void PerformFeat(int nFeat, CSWCCreature* pTarget);         // 0x006167D0
    void ActionMenuAttack(int a, CSWCCreature* pAttacker);      // 0x00616800
    void PerformSkill(unsigned short nSkill, CSWCCreature* pTarget);   // 0x00616900
    void PerformSkill(int nSkill, int a, unsigned long oidTarget);   // 0x0060EE00
};

class CSWCDoor {
public:
    void ActionMenuOpen(int a, CSWCCreature* pCreature);        // 0x00683E90
    CExoString GetTransitionName();                 // 0x006C7D20 (descriptive)
    char pad_0x0[0x13c];
    CExoString m_sTransitionName;                   // 0x13c (descriptive)
};

class CSWCPlaceable {
public:
    void ActionMenuOpen(int a, CSWCCreature* pCreature);        // 0x006826A0
};

class CSWGuiInGameMap {
public:
    void OnXButton();                               // 0x00692BC0
};

class CSWGuiInGamePartySelection {
public:
    void Show();                                    // 0x006880C0
};

// vtable 0x00754A38 family; ctor 0x00626DF0, dtor 0x006270A0.
class CSWGuiMessageBox : public CSWGuiPanel {
public:
    CSWGuiMessageBox(CSWGuiManager* pManager);      // 0x00626DF0
    ~CSWGuiMessageBox();                            // 0x006270A0
    void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006250F0)
    void OnPanelAdded();                            // 0x48 (0x006258F0)
    virtual void SetMessageStrRef(unsigned long nStrRef);   // 0x6c
    virtual void SetMessage(CExoString sMessage);   // 0x70 (0x006271A0)
    virtual void SetOkStrRef(unsigned long nStrRef);        // 0x74 (0x00625260)
    virtual void SetCancelStrRef(unsigned long nStrRef);    // 0x78 (0x00625270)
    virtual void v7c();                             // 0x7c (0x00625280)
    virtual void v80();                             // 0x80 (0x006252A0)

    void SetAllowCancel(int bCancel);               // 0x00627130
    void SetImage(const CResRef& cImage);           // 0x00627160

    unsigned long m_bAccepted : 1;                  // 0x64 bit 0
    char pad_0x68[0xc];
    CSWGuiLabel m_lblMessage;                       // 0x74
    char pad_0x1b4[0x7d0];
};

// vtable 0x00755CD0; deleting dtor 0x006AA830, implicit dtor 0x006AA850.
class CSWGuiTutorialBox : public CSWGuiMessageBox {
public:
    CSWGuiTutorialBox(CSWGuiManager* pManager);     // 0x006AA100
    void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006AA8A0)
    void OnPanelAdded();                            // 0x48 (0x006AA860)
    void SetMessageStrRef(unsigned long nStrRef);   // 0x6c (0x006AA040)

    void SetActionData(unsigned long a, unsigned long b, unsigned long c);  // 0x006AA0D0
    int HasTutorial(unsigned char nReason);  // 0x006AA1A0
    int ShowNextMessage();                          // 0x006AA670
    int ShowTutorial(unsigned char nReason);        // 0x006AA900

    // Close callbacks (one per deferred action).
    void OnCloseNothing(CSWGuiControl* pControl);   // 0x006AA230
    void OnCloseSkill(CSWGuiControl* pControl);     // 0x006AA260
    void OnCloseSpell(CSWGuiControl* pControl);     // 0x006AA2C0
    void OnCloseFeat(CSWGuiControl* pControl);      // 0x006AA350
    void OnCloseUseItem(CSWGuiControl* pControl);   // 0x006AA3D0
    void OnCloseFlourish(CSWGuiControl* pControl);  // 0x006AA440
    void OnCloseOpen(CSWGuiControl* pControl);      // 0x006AA480
    void OnCloseAttack(CSWGuiControl* pControl);    // 0x006AA520
    void OnCloseGui(CSWGuiControl* pControl);       // 0x006AA590
    void OnCloseGuiB(CSWGuiControl* pControl);      // folded into 0x006AA590
    void OnClosePartySelection(CSWGuiControl* pControl);  // 0x006AA5D0
    void OnCloseMap(CSWGuiControl* pControl);       // 0x006AA620

    void RestorePause()
    {
        if (!m_bWasPaused) {
            g_pAppManager->GetClientExoApp()->SetPausedByCombat(0, 2, 0);
        }
        m_bWasPaused = 0;
    }

    unsigned long m_oidCreature;                    // 0x984
    unsigned long m_nData;                          // 0x988
    unsigned long m_nAction;                        // 0x98c
    int m_bWasPaused;                               // 0x990
    unsigned char m_nReason;                        // 0x994
    unsigned char m_nMessage;                       // 0x995
};

#endif
