// CGuiInGame dialog-state helpers (guiingame.o): participant lock lists,
// non-dependant list, conversation panels and dialog camera reset.
// Member offsets come from the original accessors at 0x0062A970..0x00632300.
#ifndef MATCHING_N_GUI_IN_GAME_DIALOG_H
#define MATCHING_N_GUI_IN_GAME_DIALOG_H

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;

    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    CExoString(const CExoString& s);                // 0x005E5B00
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    int CompareNoCase(const char* s) const;         // 0x005E58C0
    int IsEmpty() const;                            // 0x005E58A0
};

typedef void* CExoLinkedListPosition;

struct CExoLinkedListNode {
    CExoLinkedListNode* pPrev;                      // 0x00
    CExoLinkedListNode* pNext;                      // 0x04
    void* pObject;                                  // 0x08
};

class CExoLinkedListInternal {
public:
    CExoLinkedListNode* pHead;                      // 0x00
    CExoLinkedListNode* pTail;                      // 0x04
    unsigned long nCount;                           // 0x08

    void* GetNext(CExoLinkedListPosition& pos);     // 0x005E9B90
    void* Remove(CExoLinkedListPosition pos);       // 0x005E9C10
    CExoLinkedListPosition AddHead(void* p);        // 0x005E9D70
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;   // 0x00

    CExoLinkedListPosition GetHeadPos() { return m_pcExoLinkedListInternal->pHead; }
    T* GetHead()
    {
        void* p = m_pcExoLinkedListInternal->pHead;
        if (p) {
            p = ((CExoLinkedListNode*)p)->pObject;
        }
        return (T*)p;
    }
    T* GetNext(CExoLinkedListPosition& pos) { return (T*)m_pcExoLinkedListInternal->GetNext(pos); }
    T* Remove(CExoLinkedListPosition pos) { return (T*)m_pcExoLinkedListInternal->Remove(pos); }
    CExoLinkedListPosition AddHead(T* p) { return m_pcExoLinkedListInternal->AddHead(p); }
};

class C2DA {
public:
    char pad_0x0[0x24];
    int m_nNumRows;                                 // +0x24

    int GetINTEntry(int nRow, int nColumn, int* pValue);   // 0x00413510
};

class CTwoDimArrays {
public:
    char pad_0x0[0x3c];
    C2DA* m_pAnimations;                            // +0x3C
    C2DA* m_pDialogAnimations;                      // +0x40
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    CTwoDimArrays* m_p2DArrays;                     // +0xB8
};

extern CSWRules* g_pRules;                          // 0x007A3A28
extern int g_nAnimFireForgetColumn;                 // 0x007A214C
extern int g_nAnimLoopingColumn;                    // 0x007A2154
extern int g_nAnimDialogColumn;                     // 0x007A2170
extern int g_nDialogAnimFireForgetColumn;           // 0x007A218C
extern int g_nDialogAnimDialogColumn;               // 0x007A2190
extern int g_nDialogAnimLoopingColumn;              // 0x007A2194
extern int g_nDialogAnimOverlayColumn;              // 0x007A2198
extern int g_bGuiFlag;                              // 0x007A2370

class CVirtualMachine {
public:
    int RunScript(CExoString* psFileName, unsigned long oid, int bOid); // 0x005D0FC0
};

extern CVirtualMachine* g_pVirtualMachine;          // 0x007A3A00

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

class CResRef {
public:
    char m_resRef[16];

    CResRef(const char* s);                         // 0x00406D80
    int IsValid() const;                            // 0x00406030
};

class CExoSound {
public:
    void SetSoundMode(int nMode);                   // 0x005D5E80
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC
extern int g_n74C5FC;                               // 0x0074C5FC, sound mode for modal panels

class CSWPartyTable {
public:
    char pad_0x0[0xf0];
    int m_nControlledNPC;                           // +0xF0

    void SetSelectedPlanet(int nPlanet);            // 0x00563B60
};

class CSWSArea {
public:
    char pad_0x0[0x2ac];
    int m_bUnescapable;                             // +0x2AC
};

class CSWSCreature {
public:
    CSWSArea* GetArea();                            // 0x004CB120
};

class CSWSItem;

class CSWCMessage {
public:
    int SendPlayerToServerInput_TogglePauseRequest();   // 0x00677800
};

class CSWGuiManager;
class CGuiInGame;
class CSWCObject;
class CSWCCreature;
class CSWCPlaceable;

class CGameObject {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual CSWCObject* AsSWCObject();              // +0x0C
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1C();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual CSWCCreature* AsSWCCreature();          // +0x2C
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3C();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual CSWCPlaceable* AsSWCPlaceable();        // +0x4C

    unsigned long m_idSelf;                         // +0x04
};

class CSWCAnimBase {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void SetFlag138(int bFlag);             // +0x70
};

class CSWCObject : public CGameObject {
public:
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98();
    virtual void SetAnimation(unsigned short nAnimation, int nArg1, int nArg2);  // +0x9C
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual void vF0(); virtual void vF4(); virtual void vF8(); virtual void vFC();
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10C();
    virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11C();
    virtual void v120(); virtual void v124();
    // Descriptive: the speaker's own sound/lip-sync fallback.
    virtual void PlayDialogSound(const CResRef& rSound);    // +0x128

    char pad_0x8[0x54 - 0x8];
    int m_n54;                                      // +0x54
    char pad_0x58[0x68 - 0x58];
    CSWCAnimBase* m_pAnimationBase;                 // +0x68

    unsigned short GetCurrentAnimation();           // 0x0063C060
};

class CSWCCreature : public CSWCObject {
public:
    void LookAt(unsigned long oidTarget, float fDuration);  // 0x006146E0
};

class CSWSPlaceable {
public:
    char pad_0x0[0xd4];
    unsigned short m_nAnimation;                    // +0xD4, descriptive
};

class CSWCPlaceable : public CSWCObject {
public:
    CSWSPlaceable* GetServerPlaceable();            // 0x00682E00
};

class CSWSPlayer {
public:
    char pad_0x0[0x38];
    unsigned long m_oidPC;                          // +0x38, descriptive
};

class CServerExoApp {
public:
    CSWSItem* GetItemByGameObjectID(unsigned long oid);    // 0x004AE760
    CSWPartyTable* GetPartyTable();                 // 0x004AEE70
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid);    // 0x004AE770
    unsigned long GetPlayerCreatureId();            // 0x004AEA40
    CSWSPlayer* GetClientObjectByObjectId(unsigned long oid);  // 0x004AE910
};

class CTlkTable {
public:
    char pad_0x0[0x3c];
    unsigned char m_nGender;                        // +0x3C
    char pad_0x3d[0x58 - 0x3d];
    CGameObject* m_pTokenObject;                    // +0x58, descriptive
    unsigned long m_nFlags;                         // +0x5C

    unsigned char GetGender();                      // 0x00674B40
    void ParseTokens(CExoString* pString);          // 0x0041DD30
};

extern CTlkTable* g_pTlkTableAlt;                   // 0x007A3A0C

class CExoInput {
public:
    void CoolDownEvent(unsigned long nEvent, unsigned long nTime);  // 0x005DF4B0
};

extern CExoInput* g_pExoInput;                      // 0x007A39E4

class CExoStreamingSoundSource {
public:
    CExoStreamingSoundSource();                     // 0x005D5A50
    ~CExoStreamingSoundSource();                    // 0x005D6200
    void Stop();                                    // 0x005D5B80
    void SetPriorityGroup(unsigned char nGroup);    // 0x005D5BA0
    int Play(const CResRef& rSound, int a, int b, int c, int d);   // 0x005D5B10

    void* m_pInternal;
};

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);   // 0x004ADE40
    int SubtractWorldTimes(unsigned long nDay1, unsigned long nTime1,
                           unsigned long nDay2, unsigned long nTime2,
                           unsigned long* pDays, unsigned long* pTime);  // 0x004AE460
};

class CSWCModule {
public:
    void RestoreCamera();                           // 0x00641DC0
};

class CClientExoApp {
public:
    CWorldTimer* GetWorldTimer();                   // 0x005ED4F0
    CSWCModule* GetModule();                        // 0x005ED530
    CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
    CSWGuiManager* GetGuiManager();                 // 0x005EDA40
    unsigned long ServerToClientObjectId(unsigned long oid);  // 0x005EDA50
    void DisableVideoEffect();                      // 0x005EDF20
    CGuiInGame* GetInGameGui();                     // 0x005ED690
    CSWCMessage* GetSWCMessage();                   // 0x005ED6F0
    void IndicateObject(CSWCObject* pObject, int bArg);   // 0x005EDB70
    int GetPausedByCombat();                        // 0x005EDC10
    int SetPausedByCombat(int bPaused, unsigned char nReason, int bArg);   // 0x005EDC20
    int SetInputClass(int nClass, int bArg);        // 0x005EDA60
    int GetCharacterChangeInProgress();             // 0x005EE190
};

class CAppManager {
public:
    void* m_pUnknown00;                             // +0x00
    CClientExoApp* m_pClientExoApp;                 // +0x04
    CServerExoApp* m_pServerExoApp;                 // +0x08

    CServerExoApp* GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWGuiPanel {
public:
    virtual ~CSWGuiPanel();
};

class CSWGuiManager {
public:
    void AddPanel(CSWGuiPanel* pPanel, int nMode, int bArg);   // 0x0040BC70
    int PanelExists(CSWGuiPanel* pPanel);           // 0x0040BD70
    void PlayGuiSound(char nSound);                 // 0x0040A140
    void SendPanelToBack(CSWGuiPanel* pPanel);      // 0x0040BD20
    int RemovePanel(CSWGuiPanel* pPanel);           // 0x0040C830
};

class CSWGuiInGameMenu : public CSWGuiPanel {
public:
    void SetActiveControlID(int nIndex, int bArg);  // 0x00624BD0
};

class CSWGuiBarkBubble : public CSWGuiPanel {
public:
    void SetBark(unsigned long oid, const CExoString& sText, const CResRef& rSound, int bArg);   // 0x006A9920
};

class CSWGuiInGameGalaxyMap : public CSWGuiPanel {
public:
    CSWGuiInGameGalaxyMap(CSWGuiManager* pManager); // 0x00695180
    char pad_0x4[0x2550 - 0x4];
};

class CSWGuiInGameCharacter : public CSWGuiPanel {
public:
    CSWGuiInGameCharacter(CSWGuiManager* pManager); // 0x006B0E40
    int ShowLevelUpGUI(int bFlag);                  // 0x006B0BB0
    char pad_0x4[0x59f8 - 0x4];
};

class CSWGuiPartySelection : public CSWGuiPanel {
public:
    CSWGuiPartySelection(CSWGuiManager* pManager);  // 0x006BFA40
    void SetForcedNPC(int nForced1, int nForced2);  // 0x006BE310
    char pad_0x4[0x64 - 0x4];
    int m_nMode;                                    // +0x64, descriptive
    char pad_0x68[0x70 - 0x68];
    int m_bFromScript;                              // +0x70, descriptive
    CExoString m_sExitScript;                       // +0x74
    char pad_0x7c[0x3a80 - 0x7c];
};

class CSWGuiStore : public CSWGuiPanel {
public:
    CSWGuiStore(CSWGuiManager* pManager);           // 0x006C1C00
    char pad_0x4[0x2278 - 0x4];
    unsigned long m_oidStore;                       // +0x2278
    unsigned long m_oidCustomer;                    // +0x227C
};

class CSWGuiUpgradeSelection : public CSWGuiPanel {
public:
    CSWGuiUpgradeSelection(CSWGuiManager* pManager);    // 0x006C78D0
    char pad_0x4[0x113c - 0x4];
    unsigned long m_oidItem;                        // +0x113C
    char pad_0x1140[0x1158 - 0x1140];
};

class CSWGuiLetterbox : public CSWGuiPanel {
public:
    void ResetFade();                               // 0x006A75E0
};

class CSWGuiAreaTransition : public CSWGuiPanel {
public:
    char pad_0x4[0x424 - 0x4];
    Vector m_vPosition;                             // +0x424
};

class CSWGuiDialogComputer : public CSWGuiPanel {
public:
    CSWGuiDialogComputer(CSWGuiManager* pManager);  // 0x006A8EB0
    void SetType(unsigned char nType);              // 0x006A8400

    char pad_0x4[0x3620 - 0x4];
};

class CGuiInGame {
public:
    char pad_0x0[0x8];
    CSWGuiInGameMenu* m_pInGameMenu;                // +0x08
    union {
        CSWGuiPanel* m_apPanels[8];                 // +0x0C
        struct {
            CSWGuiPanel* m_pPanel0C;                // +0x0C
            CSWGuiPanel* m_pPanel10;                // +0x10
            CSWGuiInGameCharacter* m_pCharacter;    // +0x14
        };
    };
    int m_nCurrentPanel;                            // +0x2C
    int m_bShowPanels;                              // +0x30
    char pad_0x34[0x38 - 0x34];
    CSWGuiManager* m_pGuiManager;                   // +0x38
    CSWGuiPanel* m_pDialog;                         // +0x3C
    CSWGuiPanel* m_pDialogNormal;                   // +0x40
    CSWGuiDialogComputer* m_pDialogComputer;        // +0x44
    CSWGuiPanel* m_pDialogCamera;                   // +0x48
    CSWGuiBarkBubble* m_pBarkBubble;                // +0x4C
    char pad_0x50[0x68 - 0x50];
    CSWGuiLetterbox* m_pLetterbox;                  // +0x68
    char pad_0x6c[0x78 - 0x6c];
    CSWGuiPartySelection* m_pPartySelection;        // +0x78
    char pad_0x7c[0x80 - 0x7c];
    CSWGuiInGameGalaxyMap* m_pGalaxyMap;            // +0x80
    CSWGuiStore* m_pStore;                          // +0x84
    CSWGuiUpgradeSelection* m_pUpgrade;             // +0x88
    char pad_0x8c[0x94 - 0x8c];
    CSWGuiAreaTransition* m_pAreaTransition;        // +0x94
    CSWGuiPanel* m_pGlobalPanel98;                  // +0x98
    CSWGuiPanel* m_pGlobalPanel9C;                  // +0x9C
    CSWGuiPanel* m_pGlobalPanelA0;                  // +0xA0
    CSWGuiPanel* m_pGlobalPanelA4;                  // +0xA4
    char pad_0xa8[0xb4 - 0xa8];
    int m_nGlobalDialogState;                       // +0xB4
    char pad_0xb8[0xe4 - 0xb8];
    int m_nE4;                                      // +0xE4
    Vector m_vE8;                                   // +0xE8
    int m_nF4;                                      // +0xF4
    char pad_0xf8[0x108 - 0xf8];
    int m_bInitialized;                             // +0x108
    int m_bLevelUpAvailable;                        // +0x10C, descriptive
    char pad_0x110[0x164 - 0x110];
    unsigned short m_nAnimatedCamera;               // +0x164
    char pad_0x166[0x170 - 0x166];
    unsigned long m_oidDialogOwner;                 // +0x170
    unsigned long m_oidDialogSpeaker;               // +0x174
    unsigned long m_oid178;                         // +0x178
    unsigned long m_oid17C;                         // +0x17C
    unsigned long m_oidLastSpeaker;                 // +0x180, descriptive
    unsigned long m_oid184;                         // +0x184
    char pad_0x188[0x19c - 0x188];
    unsigned long m_nLastDialogParticipants;        // +0x19C
    char pad_0x1a0[0x1a8 - 0x1a0];
    unsigned long* m_pLastDialogParticipants;       // +0x1A8
    char pad_0x1ac[0xb20 - 0x1ac];
    int m_bShotCameraInitialized;                   // +0xB20
    int m_bAnimatedCamera;                          // +0xB24
    int m_bCameraModelSet;                          // +0xB28
    float m_fAnimatedCameraTime;                    // +0xB2C
    char pad_0xb30[0xb34 - 0xb30];
    int m_nB34;                                     // +0xB34
    int m_bPaused;                                  // +0xB38
    char pad_0xb3c[0xb40 - 0xb3c];
    CExoStreamingSoundSource* m_pDialogSound;       // +0xB40
    CExoLinkedList<unsigned long> m_lstLockedOrientation;   // +0xB44
    CExoLinkedList<unsigned long> m_lstLockedHeadFollow;    // +0xB48
    CExoLinkedList<CExoString> m_lstDialogNonDependant;     // +0xB4C
    char pad_0xb50[0xbcc - 0xb50];
    unsigned long m_nActionDay;                     // +0xBCC
    unsigned long m_nActionTime;                    // +0xBD0
    unsigned long m_nActionDelay;                   // +0xBD4

    void SetConversationType(unsigned long nType, unsigned char nComputerType);
    void ShutDownGlobal();
    void EndDialogCamera();
    int IsOverlayDialogAnimation(unsigned short nAnimation);
    int IsLoopingDialogAnimation(unsigned short nAnimation);
    int IsFireForgetDialogAnimation(unsigned short nAnimation);
    Vector GetAreaTransitionPosition();
    void UpdateCreatedInGameGUI(int nOldPanel, int nNewPanel);   // 0x0062B1E0
    void NextSWInGameGui();
    void ClearRepliesData();                        // 0x0062BA30
    int CanPlayDialogAnimations(unsigned long oid); // 0x0062F0E0
    void SetLipSyncData(unsigned long oid, const CResRef& rSound);   // 0x0062B440
    void CleanUpDialogNode();
    void CleanUpDialogReplies();
    int HandleDialogReplyChosen(unsigned long nArg1, unsigned long nArg2, unsigned long nArg3,
                                const CExoString& sText, unsigned long nArg5,
                                unsigned char nArg6, int nArg7);
    int HandleDialogSound(unsigned long oid, const CResRef& rSound);
    void ResetDialogAnimations();
    void ResetTalkAnimations();
    void SwitchToSWInGameGui(int nPanel);
    void SetSWGuiStatus(int nStatus, int bArg);     // 0x0062AA00
    void ShowGalaxyMapGui(int nPlanet);
    void ShowStoreGui(unsigned long oidStore, unsigned long oidCustomer);
    void ShowUpgradeScreen(unsigned long oidItem);
    int ShowLevelUpGUI(int bFlag);
    void ShowPartySelection(const CExoString& sExitScript, int bFromScript, int nForced1, int nForced2);
    void PrevSWInGameGui();
    void SetLockObjectOrientationInDialog(unsigned long oid, int bLock);
    int GetCanOrientInDialog(unsigned long oid);
    void SetLockHeadFollowInDialog(unsigned long oid, int bLock);
    int GetCanHeadFollowInDialog(unsigned long oid);
    int IsOnDialogNonDependantList(const CExoString& sName);   // 0x0062F840
    int CanPerformAction();
    void AddToDialogNonDependantList(const CExoString& sName);
};

#endif
