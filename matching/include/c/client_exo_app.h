// Worker C matching headers: CClientExoApp facade and CClientExoAppInternal.
// Layout from b005e_client_exo_app_accessors (internal block is 0x4d4 bytes).
#ifndef C_CLIENT_EXO_APP_H
#define C_CLIENT_EXO_APP_H

#include "c/exo_base.h"

#define OBJECT_INVALID 0x7f000000

class CClientPlayer;
class CWorldTimer;
class CSWCQuestJournal;
class CSWCWorldJournal;
class CSWGuiPanel;
class CClientInput;
class CImeUI;
class CSoundSetList;
class CSWCModule;
class CSWCSaveLoad;
class CSWGuiManager;
class CSWGuiLoadScreen;
class CMoviePlayer;
class CExoSoundSource;
class CSWCCreature;
class CGameObject;
class CSWCObject;
class CGameObjectArray;
class CClientExoApp;
class CServerExoApp;
class CSWParty;
class CGuiInGame;
class CSWSCreature;
class CSWCCombatAttackData;
class CSWCMessage;
class CSWCAreaOfEffectObject;
class CExoStreamingSoundSource;
class CSWCMiniGame;
class CSWCItem;
class CSWCArea;
class CSWCTrigger;
class CSWCProjectile;
class CSWCDoor;
class CSWCSoundObject;
class CSWCPlaceable;
class CGob;
class CSWGuiInterfaceAction;

struct CHostileHilite {
    unsigned long m_oidTarget;                      // 0x00
    float m_fTimeLeft;                              // 0x04
};

struct CRumblePattern {
    char pad_0x0[0x20];
    int m_bExclusive;                               // 0x20
};

struct CActiveRumble {
    int m_nPattern;                                 // 0x00
    int m_nTime;                                    // 0x04
    int m_bActive;                                  // 0x08
};

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);   // 0x004ADE40
    unsigned __int64 GetSnapshotTime();             // 0x004ADC70
    void SetSnapshotScale(float fScale);            // 0x004AE190
    void SetFixedSnapshotRate(int bEnable, unsigned long nRate);   // 0x004AE170
};

class CExoTimers {
public:
    unsigned __int64 GetHighResolutionTimer();      // 0x005E9260
};

class CExoBase {
public:
    void* m_pUnknown00;
    CExoTimers* m_pcExoTimers;                      // 0x04
};

class CExoSound {
public:
    void Update();                                  // 0x005D5DD0
    void SetMusicState(int n);                      // 0x005D5E80
};

class CExoSoundSource {
public:
    ~CExoSoundSource();                             // 0x005D6200
    void Stop();                                    // 0x005D5B80
    void SetFadeTime(unsigned long nMs);            // 0x005D5B90
    int IsPlaying();                                // 0x005D5AF0
};

class CMoviePlayer {
public:
    CMoviePlayer();                                 // 0x0070C170
    ~CMoviePlayer();                                // 0x0070C250
    void Stop(int a, int b);                        // 0x0070C1E0
    int GetState();                                 // 0x0070C1F0
    void HideFadePanel();                           // 0x0070C220
    int Initialize(int n);                          // 0x0063E7C0
    unsigned long GetFrameCount();                  // 0x0070C230
    void Play();                                    // 0x0070C240
    void* m_pInternal;                              // 0x00
};

class CSWCQuestJournal {
public:
    CSWCQuestJournal();                             // 0x006770C0
    char pad_0x0[0x50];
    void JournalUpdated(int a, int b, int c);       // 0x00676C20
};

class CSWCWorldJournal {
public:
    CSWCWorldJournal();                             // 0x00676DA0
    char pad_0x0[0x20];
};

class CSWCModulePlayer {
public:
    char pad_0x0[0x104];
    unsigned long m_idPlayerObject;                 // 0x104

    void Unknown609410();                           // 0x00609410
};

class CSWCModuleScene {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual int TestCameraFlag(int nFlag);          // 0x80
};

class CSWCModule {
public:
    CSWCModule();                                   // 0x00643F40
    void InitializeShotCamera(unsigned long n);     // 0x006412F0
    void EnterCombatMode();                         // 0x00641540
    void ExitCombatMode();                          // 0x00641DC0

    CSWCMiniGame* GetMiniGame();                    // 0x00641980
    CSWCModuleScene* GetScene() { return m_pScene; }
    void SetMapCharacter(unsigned long oid, int b); // 0x0063F9D0

    char pad_0x0[0x40];
    CSWCModuleScene* m_pScene;                      // 0x40
    char pad_0x44[0x4];
    CSWCModulePlayer* m_pPlayer;                    // 0x48
    char pad_0x4c[0xec];
};

class CSWCSaveLoad {
public:
    int SendSaveGameRequest(int a, int b, int c);   // 0x006774E0
    int SendLoadGameRequest(int a, int b, int c);   // 0x00677560
    void HandleMessage(unsigned char* pData, unsigned long nSize);   // 0x0066A640
};

class CClientMessageHandler1c0 {
public:
    void HandleMessage(unsigned char* pData, unsigned long nSize);   // 0x00675710
};

class CSWGuiManager {
public:
    int PanelExists(CSWGuiLoadScreen* pPanel);      // 0x0040BD70
    void PopModalPanel();                           // 0x0040BE00
    int RemovePanel(CSWGuiPanel* pPanel);           // 0x0040C830
    void PlayGuiSound(int nSound);                  // 0x0040A140
    int HandleRMouseUp();                           // 0x0040A1A0
    int HandleRMouseDown();                         // 0x0040C610
};

class CSWGuiPanel {
public:
    virtual ~CSWGuiPanel();
};

class CSWGuiLoadScreen : public CSWGuiPanel {
public:
    void SetHint(int nHint);                        // 0x0067A9C0
};

class CGob {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94();
    virtual CGob* FindPart(unsigned long nPart, int bRecurse);  // 0x98
    virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8();
    virtual void Pause();                           // 0xdc
    virtual void Resume();                          // 0xe0
};

class CItemModelBase {
public:
    virtual void v00();
    char pad_0x4[0xc];
};

class CItemModel : public CItemModelBase, public CGob {
};

class CGameObject {
public:
    virtual void v00();
    virtual void v04();
    virtual void SetWorldTime(unsigned long nDay, unsigned long nTime);  // 0x08
    virtual CSWCObject* AsSWCObject();              // 0x0c
    virtual void v10(); virtual CSWCDoor* AsSWCDoor(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual CSWCArea* AsSWCArea(); virtual void v28();
    virtual CSWCCreature* AsSWCCreature();          // 0x2c
    virtual CSWSCreature* AsSWSCreature();          // 0x30
    virtual CItemModel* GetItemModel();             // 0x34

    unsigned long m_idSelf;                         // 0x04
    unsigned char m_nObjectType;                    // 0x08
};

class CSWCObject : public CGameObject {
public:
    virtual void v38(); virtual CSWCTrigger* AsSWCTrigger(); virtual void v40();
    virtual CSWCProjectile* AsSWCProjectile();      // 0x44
    virtual void v48(); virtual CSWCPlaceable* AsSWCPlaceable(); virtual void v50();
    virtual CSWCAreaOfEffectObject* AsSWCAreaOfEffectObject();   // 0x54
    virtual void v58(); virtual void v5C(); virtual void v60(); virtual void v64();
    virtual void v68(); virtual CSWCSoundObject* AsSWCSoundObject(); virtual void v70(); virtual void v74();
    virtual void v78(); virtual void v7C(); virtual void v80(); virtual void v84();
    virtual void v88(); virtual void v8C(); virtual void v90(); virtual void v94();
    virtual CGob* FindPart(unsigned long nPart, int bRecurse);  // 0x98
    virtual void v9C(); virtual void vA0(); virtual void vA4();
    virtual void SetHostileHilite(int b);           // 0xa8
    virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual void vF0(); virtual void vF4(); virtual void vF8(); virtual void vFC();
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10C();
    virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11C();
    virtual int BarkString(unsigned long oid, int a, int b, int c);   // 0x120

    void PlayAnimation(int a, int b, int c, int d); // 0x0063CE60
};

struct CSWCPlaceableAppearance {
    char pad_0x0[0x444];
    int m_bCorpseFaded;                             // 0x444
};

class CSWCPlaceable : public CSWCObject {
public:
    CSWCPlaceableAppearance* GetAppearance();       // 0x00682E00
};

class CSWCCreature : public CSWCObject {
public:
    char pad_0xc[0x3c];
    unsigned short m_nAnimation;                    // 0x48
    char pad_0x4a[0x9a];
    int m_bCorpse;                                  // 0xe4
    char pad_0xe8[0x20c];
    unsigned long m_oidLockOrientation;             // 0x2f4
    char pad_0x2f8[0xe4];
    unsigned long m_oidCorpseContainer;             // 0x3dc

    int IsAnimationPlaying(unsigned short nAnim);   // 0x0063C4A0
    void SetCorpseState(int b);                     // 0x0063E220
    void GetCreatureLight(int b);                   // 0x00614AA0
    int IsDead();                                   // 0x004EF890
    int IsInjured();                                // 0x00614B60
    void SetLockOrientation(unsigned long oid, int b);   // 0x00610950
    void SetAttackTarget(int b);                    // 0x00610A10
    void ForceMenuAction(unsigned long nAction, CSWCCreature* pCreature); // 0x0060F330
    void ForceMenuHostileAction(unsigned long nAction, CSWCCreature* pCreature); // 0x00616200
    void GetSelfForcePowers(CExoArrayList<CSWGuiInterfaceAction>& actions); // 0x00616230
    CGameObject* GetServerObject();                 // 0x0063D4B0
    void PlaySwingSound(const CExoString& sEvent, CSWCCombatAttackData* pData, int nCount); // 0x0060DC50
    int ShowLightSaberContactVisual(int nUnused);   // 0x0060DE20
    void PartyMenuUseItem(unsigned long oidItem, CGameObject* pTarget); // 0x0060F590
    void PerformFeat(int nFeat, CSWCCreature* pTarget); // 0x006167D0
    void PerformSkill(unsigned short nSkill, CSWCCreature* pTarget); // 0x00616900
};

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long id, CGameObject** ppObject);  // 0x004D8230
    unsigned char Delete(unsigned long id, CGameObject** ppObject);         // 0x004D81A0
};

class CSWPartyMember;

class CSWParty {
public:
    CSWPartyMember* GetMember(int n);               // 0x006346C0
};

class CSWPartyMember {
public:
    CSWCCreature* GetCreature();                    // 0x0060FB20
};

class CClientOptions {
public:
    int IsLoaded();                                 // 0x0061DBE0
    void SetDefaults();                             // 0x0061DA60
    int SetOption(int nOption, int nValue);         // 0x004AE170

    void SetCameraMode(unsigned char nMode);        // 0x0061B6F0 (reference name)
    void SetFullScreenEnabled(int bEnabled);         // 0x0061B710 (reference name; ownership conflict noted)
    void SetAutoLevelUpNPCs(int bEnabled);           // 0x0061B740 (Mac name)

    unsigned long m_nFlags;                         // +0x00
    char pad_0x04[0x04];
    unsigned long m_nGameplayFlags;                 // +0x08
    char pad_0x0c[0x61];
    unsigned char m_nCameraMode;                    // +0x6d
    unsigned char m_nCameraModeCurrent;              // +0x6e
};

class CClientUnknown700 {
public:
    int Unknown61b780();                            // 0x0061B780
    char pad_0x0[0xc];
    float m_fToolTipDelay;                          // 0x0c

    float GetToolTipDelay() { return m_fToolTipDelay; }
    float GetGamma(int b);                          // 0x0061D590
};

void AurDisableSaturation();                        // 0x0044F0D0
void AurDisableScanNoise();                         // 0x0044F0F0

class CServerExoApp {
public:
    int CreatureAcquireItem(unsigned long oidCreature, unsigned long oidItem);   // 0x004AEB40
    int GetPauseState(int n);                       // 0x004AE980
    CGameObject* GetGameObject(unsigned long oid);  // 0x004AE750
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid);  // 0x004AE770
    unsigned long ClientToServerObjectID(unsigned long oid);     // 0x004AEA30
    unsigned long GetPlayerCreatureID();            // 0x004AEA40
    CWorldTimer* GetWorldTimer();                   // 0x004AEDE0
    CWorldTimer* GetPauseTimer();                   // 0x004AEE00
    class CSWPartyTable* GetPartyTable();            // 0x004AEE70
};

class CSWPartyTable {
public:
    void AutoLevelUpNPCs();                          // 0x00563B90
};

struct CAppPad {
    int m_n00;                                      // 0x00
    int m_n04;                                      // 0x04
    char pad_0x8[0xc];
    int m_n14;                                      // 0x14
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08
    char pad_0xc[0x8];
    CAppPad* m_pPad;                                // 0x14

    void Unknown401280();                           // 0x00401280
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
    CServerExoApp* GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC
extern CExoBase* g_pExoBase;                        // 0x007A39E0
extern CExoSound* g_pExoSound;                      // 0x007A39EC
extern CSWGuiManager* g_pGuiManager;                // 0x007A39F4
extern int g_bSwapMouseButtons;                     // 0x008338F0
extern unsigned long g_bReinitializeVideoMode;      // 0x007A3A2C

struct CClientResRefSlot {
    char m_resRef[16];
};

class CClientExoAppInternal {
public:
    void Unknown5FCD10(int a, int b, const CExoString& s);         // 0x005FCD10
    CClientExoAppInternal();                        // 0x005FBFE0
    ~CClientExoAppInternal();                       // 0x005FC6F0

    void*                              m_pUnknown00; // 0x00
    CClientOptions*                    m_pClientOptions; // 0x04
    void*                              m_pUnknown08; // 0x08
    void*                              m_pUnknown0c; // 0x0c
    void*                              m_pUnknown10; // 0x10
    CGameObjectArray*                  m_pGameObjectArray; // 0x14
    CSWCModule*                        m_pModule; // 0x18
    CExoLinkedList<CClientPlayer>      m_lstPlayerList; // 0x1c
    unsigned long                      m_oidPlayer; // 0x20
    CWorldTimer*                       m_pWorldTimer; // 0x24
    CWorldTimer*                       m_pExclusionTimer; // 0x28
    CWorldTimer*                       m_pPauseTimer; // 0x2c
    CWorldTimer*                       m_pAnimationTimer; // 0x30
    void*                              m_pUnknown34; // 0x34
    CSWCQuestJournal*                  m_pQuestJournal; // 0x38
    CSWCWorldJournal*                  m_pWorldJournal; // 0x3c
    CGuiInGame*                        m_pGuiInGame; // 0x40
    char pad_0x44[0xc];
    CSWGuiPanel*                       m_pPlayModulePanel; // 0x50
    char pad_0x54[0x1c];
    int                                m_bPazaakIsRunning; // 0x70
    CExoString                         m_sPazaakScript; // 0x74
    int                                m_nPazaakWager; // 0x7c
    int                                m_bPazaakWon; // 0x80
    int                                m_nPazaakDeck; // 0x84
    int                                m_nUnknown88; // 0x88
    int                                m_bCaptureEvents; // 0x8c
    int                                m_bCaptureEventPlayBack; // 0x90
    int                                m_nUnknown94; // 0x94
    char pad_0x98[0x4];
    CClientInput*                      m_pInputClass; // 0x9c
    char pad_0xa0[0x4];
    unsigned long                      m_nHitCheckMaterialMask; // 0xa4
    char pad_0xa8[0x34];
    int                                m_bClientImeUI; // 0xdc
    CImeUI*                            m_pImeUI; // 0xe0
    char pad_0xe4[0x48];
    CMoviePlayer*                      m_pMoviePlayer; // 0x12c
    char pad_0x130[0xc];
    CExoString                         m_sClientName; // 0x13c
    int                                m_nUnknown144; // 0x144
    unsigned short                     m_nServerMode; // 0x148
    char pad_0x14a[0x2];
    CSWCSaveLoad*                      m_pSaveLoad; // 0x14c
    char pad_0x150[0x18];
    CExoLinkedList<CExoString>         m_lstUnknown168; // 0x168
    char pad_0x16c[0x20];
    int                                m_nLoadBarState; // 0x18c
    CExoLinkedList<unsigned long>      m_lstHitcheckIgnore; // 0x190
    int                                m_nUnknown194; // 0x194
    char pad_0x198[0x10];
    int                                m_nUnknown1a8; // 0x1a8
    int                                m_nUnknown1ac; // 0x1ac
    char pad_0x1b0[0x4];
    char*                              m_pCharacterFile; // 0x1b4
    unsigned long                      m_nCharacterFileSize; // 0x1b8
    unsigned long                      m_nCharacterFileValue; // 0x1bc
    CClientMessageHandler1c0*          m_pMessageHandler1c0; // 0x1c0
    CSoundSetList*                     m_pSoundSetList; // 0x1c4
    CResRef*                           m_pItemDropSounds; // 0x1c8
    unsigned char                      m_nPauseFlags; // 0x1cc
    char pad_0x1cd[0x3];
    CExoLinkedList<unsigned long>*     m_pPauseExclusionList; // 0x1d0
    char pad_0x1d4[0x18];
    int                                m_bStartMenuMusicOnMovieEnd; // 0x1ec
    char pad_0x1f0[0x8];
    int                                m_bMovieQueueActive; // 0x1f8
    char pad_0x1fc[0x4];
    int                                m_bUnknown200; // 0x200
    int                                m_bClientInitialized; // 0x204
    char pad_0x208[0x18];
    int                                m_bInStartNewModule; // 0x220
    char pad_0x224[0x4];
    int                                m_bCanSendDialog; // 0x228
    int                                m_nCanSendDialogTime; // 0x22c
    int                                m_bDialogSent; // 0x230
    int                                m_bDialogModelsSent; // 0x234
    CExoSoundSource*                   m_pMenuMusic; // 0x238
    CExoSoundSource*                   m_pLoadMusic; // 0x23c
    char pad_0x240[0x8];
    int                                m_nUnknown248; // 0x248
    char pad_0x24c[0x4];
    int                                m_bUnknown250; // 0x250
    int                                m_bUnknown254; // 0x254
    char pad_0x258[0x18];
    CSWParty*                          m_pParty; // 0x270
    CSWGuiManager*                     m_pGuiManager; // 0x274
    CSWGuiLoadScreen*                  m_pLoadScreen; // 0x278
    CSWGuiPanel*                       m_pUnknownPanel27c; // 0x27c
    char pad_0x280[0x8];
    int                                m_bAreaNotReadyToDisplay; // 0x288
    int                                m_nUnknown28c; // 0x28c
    int                                m_nPlayerCharacterSet; // 0x290
    CExoString                         m_sPlayerCharacterName; // 0x294
    char pad_0x29c[0x4];
    CSWGuiPanel*                       m_pCharacterPanel; // 0x2a0
    char pad_0x2a4[0x10];
    unsigned long                      m_oidLastTarget; // 0x2b4
    unsigned long                      m_oidLastClickedOnTarget; // 0x2b8
    char pad_0x2bc[0x4];
    int                                m_bGameOver; // 0x2c0
    int                                m_bGameOverSet; // 0x2c4
    char pad_0x2c8[0xc];
    unsigned long                      m_oidPortraitCreature; // 0x2d4
    CExoArrayList<CResRef>             m_lstMoveToModuleMovies; // 0x2d8
    CExoArrayList<CResRef>             m_lstQueuedMovies; // 0x2e4
    CExoArrayList<int>                 m_lstQueuedMovieFlags; // 0x2f0
    char pad_0x2fc[0x8];
    CExoString                         m_sLoadMusic; // 0x304
    char pad_0x30c[0x4];
    int                                m_bLoadFromIFO; // 0x310
    CExoArrayList<CHostileHilite>      m_lstHostileHilites; // 0x314
    int                                m_bCombatMode; // 0x320
    int                                m_bEnemySighted; // 0x324
    char pad_0x328[0x4];
    int                                m_bRunScript; // 0x32c
    CExoString                         m_sRunScript; // 0x330
    unsigned long                      m_oidFutureLeader; // 0x338
    int                                m_bLoadingSaveGame; // 0x33c
    CExoArrayList<CRumblePattern>      m_lstRumblePatterns; // 0x340
    CExoArrayList<CActiveRumble>       m_lstActiveRumbles; // 0x34c
    char pad_0x358[0x1c];
    int                                m_nUnknown374; // 0x374
    float                              m_fUnknown378; // 0x378
    unsigned long                      m_bUnknown37c_0 : 1; // 0x37c
    unsigned long                      m_bUnknown37c_1 : 1;
    unsigned long                      m_bPausedByCombatSet : 1;
    int                                m_bPausedByCombat; // 0x380
    unsigned long                      m_bAutoPaused : 1; // 0x384
    unsigned char                      m_nPausedByCombatFlag; // 0x388
    char pad_0x389[0x3];
    float m_fUnknown38c;
    float m_fUnknown390;
    char pad_0x394[0x14];
    int                                m_nVideoEffect; // 0x3a8
    int                                m_nTutorialWindowA; // 0x3ac
    int                                m_nTutorialWindowB; // 0x3b0
    int                                m_nTutorialWindowC; // 0x3b4
    unsigned char                      m_nTutorialWindow; // 0x3b8
    char pad_0x3b9[0x3];
    unsigned long                      m_oidCachedPlayer; // 0x3bc
    CSWCCreature*                      m_pCachedPlayerCreature; // 0x3c0
    CExoStreamingSoundSource*          m_pEvilEndingSound; // 0x3c4
    unsigned long m_nMousePositionRestoreX;    // 0x3c8
    unsigned long m_nMousePositionRestoreY;    // 0x3cc
    char pad_0x3d0[0x10];
    int                                m_nAttackMashTutorialTimer; // 0x3e0
    unsigned char                      m_pTutorialFlags[0x34]; // 0x3e4
    CClientResRefSlot                  m_aUnknown418[7]; // 0x418
    int                                m_nLoadScreenHintUpdateTimer; // 0x488
    char pad_0x48c[0x4];
    unsigned char                      m_bGamePlayHint; // 0x490
    unsigned char                      m_bStoryHint; // 0x491
    char pad_0x492[0x6];
    unsigned char                      m_bModuleResourcesLoaded; // 0x498
    char pad_0x499[0x1b];
    int                                m_bCharacterChangeInProgress; // 0x4b4
    int                                m_bCreditSequenceInProgress; // 0x4b8
    int                                m_bEvilEndingHackInProgress; // 0x4bc
    int                                m_bAllowHighMonitorFrequency; // 0x4c0
    int                                m_bSoundPausedByCombat; // 0x4c4
    char pad_0x4c8[0xc];


    CSWCQuestJournal* CreateQuestJournal();
    CSWCWorldJournal* CreateWorldJournal();
    CSWCModule* CreateModule();
    void JournalUpdated(int a, int b, int c);
    int SendSaveGameRequest(int a, int b, int c);
    int SendLoadGameRequest(int a, int b, int c);
    int SetUnknown94(int n);
    CSWCCreature* GetPlayerCreature();
    void SetUnknown194(int n);
    int LoadScreenExists();
    CExoString* GetPlayerCharacterName();
    int InitializeMoviePlayer();
    int IsMoviePlaying();
    int GetPausedByCombat();
    int GetControlledCharacterDead();
    void SetGameOver(int b);
    void RestoreMousePositionTo(int x, int y);
    void SetLoadScreenHint(int nHint);
    void DisableVideoEffect();
    int ForwardUnknown46f0();
    void CancelMovie();
    void WaitSoundFade750();
    void WaitSound500();
    void HideMoviePlayerFadePanel();
    void StopCreditSequence();
    void UpdateMovieState();
    void AddObjectToHitcheckIgnoreList(unsigned long oid);
    void RemoveObjectFromHitcheckIgnoreList(unsigned long oid);
    void EmptyHitcheckIgnoreList();                 // 0x005F6410
    void StopMenuMusic();
    void ShutDownMoviePlayer();
    void UpdateHostileHilite(float fDelta);
    void ClearPauseExclusionList();
    void AddToPauseExclusionList(unsigned long oid);
    void Uninitialize();
    void AddItemModelDirectlyToIgnoreList(unsigned long oid, CExoArrayList<CGob*>* pList);
    int PlayRumblePattern(int nPattern);
    void ShutDownToMainMenu();
    void AddObjectToIgnoreList(unsigned long oid, CExoArrayList<CGob*>* pList);
    void SetCharacterFile(unsigned long nSize, const char* pData, unsigned long nValue);
    void StopLoadMusic();
    int BarkString(unsigned long oid, int a, int b, int c);
    int CanPlayBarkSound();
    void StopBarkSound();
    void SetPausedByCombat(int bPaused, unsigned char nFlag, int bForce);
    int LoadMoveToModuleMovies(int n);
    int LoadMovieList(int n, CExoArrayList<CResRef>* pList);   // 0x005FBBF0

    // callees
    int IsOnExclusionList(unsigned long oid);       // 0x005F56B0
    CWorldTimer* GetActiveTimer(unsigned long oid); // 0x005F7DC0
    void ClearInputState(int nMask);                // 0x0061FA00
    void SetInputState(int nMask);                  // 0x0061F9C0
    void ResetInput();                              // 0x005F2980
    void ShowLoadScreen(int b);                     // 0x005F6F20
    void SetPortraitCreature(unsigned long oid);
    void SetCombatMode(int bCombat);
    void SetInputActive(int bActive, int bClear);
    void RestartMoviesIfNecessary();
    static void __stdcall FadeOutMusic(CExoSoundSource* p, int nMs);  // 0x005F1B20
    void ClearClientState();                        // 0x00690910
    void SetCursorState(int a, int b);              // 0x005F14A0
    void UnloadModule();                            // 0x005F8290
    void ResetGuis(int n);                          // 0x005FCA30
    void SetMusic(int a, int b, int c);             // 0x005F9AF0
    void AddCreatureModelToIgnoreList(unsigned long oid, CExoArrayList<CGob*>* pList);  // 0x005F97E0
    void AddOtherModelToIgnoreList(unsigned long oid, CExoArrayList<CGob*>* pList);     // 0x005F9AA0
    void SetVideoMode(int nMode, int b);            // 0x005F1830
    CResRef GetItemDropSound(unsigned char nType);  // inline in client_exo_app.cpp
    void SetTexturePack(int nPack, int b);          // 0x005F14A0
    CExoString GetLoadMusicByModuleName(const CExoString& sModule);   // 0x005F3650
    CExoString GetGUIString(int nId);               // 0x005FB540
    CExoString GetDisplayCharacterForKey(int nKey); // 0x005F4DF0
    int GetClientRangedAnimation(unsigned long a, unsigned long b, int c, int d);  // 0x005F33F0

    // CClientExoApp forwarder targets
    void BeginIntro();                                              // 0x00602DC0
    int GetObjectName(unsigned long oid, CExoString* pName);        // 0x005F6640
    void ExitProgram();                                             // 0x005EEA10
    CSWCModule* GetModule();                                        // 0x00644760
    void* GetModuleCamera();                                        // 0x005EE6F0
    CGameObject* GetGameObject(unsigned long oid);                  // 0x005F10F0
    CClientPlayer* GetPlayerByGameObjectID(unsigned long oid);      // 0x005F5CB0
    CSWCItem* GetItemByGameObjectID(unsigned long oid);             // 0x005F1130
    CSWCCreature* GetCreatureByGameObjectID(unsigned long oid);     // 0x005F1180
    CSWCArea* GetAreaByGameObjectID(unsigned long oid);             // 0x005F11D0
    CSWCTrigger* GetTriggerByGameObjectID(unsigned long oid);       // 0x005F1220
    CSWCProjectile* GetProjectileByGameObjectID(unsigned long oid); // 0x005F1270
    CSWCPlaceable* GetPlaceableByGameObjectID(unsigned long oid);   // 0x005F12C0
    CSWCDoor* GetDoorByGameObjectID(unsigned long oid);             // 0x005F1310
    CSWCAreaOfEffectObject* GetAreaOfEffectByGameObjectID(unsigned long oid); // 0x005F1360
    CSWCSoundObject* GetSoundObjectByGameObjectID(unsigned long oid); // 0x005F13B0
    int GetPauseState(unsigned char nType);                         // 0x005EE8F0
    int PauseState(unsigned char nType) { return m_nPauseFlags & nType; }
    void SetPauseState(int nType, int bPause, int bForce);          // 0x005F7FA0
    CClientPlayer* GetPlayerByPlayerID(unsigned long nPlayerId);    // 0x005F5C70
    void AttachPlayerGameObjectToArea();                            // 0x005F5CF0
    void InitializeInGameGUI();                                     // 0x005F1410
    void ServerModeChange(unsigned char nMode);                     // 0x005EEAD0
    void SetPlayerCreature(unsigned long oid);                      // 0x005F2BB0
    int Initialize();                                               // 0x005F51C0
    int MainLoop();                                                 // 0x00602EB0
    int CreateCaptureEventFile(const CExoString& sFile);            // 0x005F5720
    int setUpEventPlayBack(const CExoString& sFile);                // 0x005F5910
    void setCaptureEventRate(int nRate);                            // 0x005EEAE0
    void SetDebugMode(int nMode);                                   // 0x005F1490
    int StartServices(int a, int b, int c, int d, int e);           // 0x005F8550
    void SetEventDescriptions();                                    // 0x005EEB10
    void StopServices();                                            // 0x00604130
    unsigned char GetActivePauseState();                                      // 0x005EE910
    CSWCModuleScene* GetModuleScene();                              // 0x005EE6F0
    int HandleMessage(unsigned long nPlayerId, unsigned char* pData, unsigned long nSize, int bRaw);   // 0x005EE700
    void QueryExitProgram();                                        // 0x005F6960
    int IsPlayerCreature(CGameObject* pObject);                     // 0x005F2B50
    unsigned long ServerToClientObjectId(unsigned long oid);        // 0x005F2AC0
    int SetInputClass(int nClass, int bForce);                      // 0x006200E0
    void ShowLoadScreen(int a, int b, int c);                       // 0x005F6C60
    void HideLoadScreen(int a, int b);                              // 0x005F6E20
    void SetPlayerCharacterName(const CExoString& sName);           // 0x005F2B00
    void PlayLoadModuleMovies();                                    // 0x00602650
    int PlayMovie(int a, int b, int c, int d);                      // 0x00602CF0
    void AddMoveToModuleMovie(const CResRef& cMovie);               // 0x006027A0
    void IndicateObject(CGameObject* pObject);                      // 0x005F2C60
    void ResetDriveAcceleration();                                  // 0x005F1400
    int IsSoundPlayingInDialog();                                   // 0x005F2D00
    void StopSoundPlayingInDialog();                                // 0x005F2D80
    void DisableInput();                                            // 0x0061F7C0
    void EnableInput();                                             // 0x0061F7D0
    void DismissInGameGUI();                                        // 0x00620320
    void ChangeCharacter(int a, int b);                             // 0x005F6B60
    void RunDeathSequence();                                        // 0x005F7200
    void SetLastPartyMemberTempKilled(int b);                       // 0x005F2FE0
    void SetCanSendDialog(int b);                                   // 0x005F3030
    int GetCanSendDialog(int a, int b, int c);                      // 0x005F3070
    int GetClientMeleeAnimation(unsigned long a, unsigned long b, int c, int d, int e, int f); // 0x005F32E0
    void SetLoadScreenByModuleName(const CExoString& sModule);      // 0x005F3480
    void SetLoadBarProgress(int a, int b);                          // 0x005FB3F0
    void SetLastTarget(unsigned long oid, int b);                   // 0x005F4A20
    void UpdateRoom(int n);                                         // 0x005EE860
    void StartPazaakGame(int a, int b, int c, int d, int e);        // 0x005F3810
    void EndPazaakGame(int a, int b, int c, int d);                 // 0x005F3950
    int StopRumblePattern(int nPattern);                            // 0x005F74B0
    void TogglePartyFollow();                                       // 0x005F2A20
    void PlayerFlourishWeapons();                                   // 0x005F4460
    void AddFloatyText(int a, int b, int c);                        // 0x006027C0
    void SetAutoPaused(int a, int b);                               // 0x005F3F10
    void EnableVideoEffect(int nEffect);                            // 0x005F7710
    void DelayNextEnemyAutoPause();                                 // 0x005F4100
    int ShowTutorialWindow(int nWindow, int a, int b, int c);       // 0x005F4120
    void ClearCachedCreatures(int b);                               // 0x005F1460
    void LookUpAndPerformRumbleWithCutOff(int a, int b, int c);     // 0x005FB960
    void ChangeCharacterToNextLivingPartyMember(int a, int b, int c); // 0x005F7960
    int GetNextLoadScreenHintSTRREF(int a, int b);                  // 0x005F4760
    void SetMouseMode(unsigned char nMode);                         // 0x005F49B0
    void FadeAndStopStreamingSoundSource(CExoStreamingSoundSource* p, unsigned long nMs); // 0x005F1B20
    void StartCreditSequence(int a, int b);                         // 0x005F4C60
    void EnableMouseAtEndOfLoadModuleMovie();                       // 0x005F4DB0
    void AddMovieToMovieQueue(int a, int b);                        // 0x005FBF50
    void PlayMovieQueue(int b);                                     // 0x00602D40
    int PlayMoviesInExoArrayList(CExoArrayList<CExoString>* pList, int* pResult,
                                 int bWait, unsigned long nFlags, CExoArrayList<int>* pMovieFlags); // 0x00602AF0
    int IsValidResolution(int a, int b);                            // 0x005F0C60
    int DontUseThisSetPlayerCreature(unsigned long oid);            // 0x005F4EC0

    // worker C r5
    void HurryDeathSequence();                                      // 0x005F2F20
    void SetGameSpeed(float fSpeed);                                // 0x005F2F60
    void SetToolTipInstantMode(int bInstant);                       // 0x005F4BE0
    void PlayCharacterSelectSound(CSWCCreature* pCreature);         // 0x005F4D60
    void SetEvilEndingHackInProgress(int b);                        // 0x005F4E50
    int LoadOptions();                                              // 0x005F4710
    void EmptyMethod();                                             // 0x004015A0 (shared empty body)
    void PerformLButtonDownAction(int bDouble);                     // 0x0061F880
    void PerformLButtonUpAction();                                  // 0x00620530
    int PerformRButtonDownAction();                                  // 0x0061F940
    int PerformRButtonUpAction();                                    // 0x0061F960
    void HandleMouseClickInWorld();                                 // 0x00620350
    CSWCCreature* LookUpCreature(unsigned long oid);                // inline in client_exo_app_internal_r5.cpp
    int PlayMovieList(CExoArrayList<CResRef>* pList, int a, int b, int c, int d);   // 0x00602AF0
};

class CClientExoApp {
public:
    // vtable 0x0074E6D8 (7 slots; names unknown yet)
    virtual void v00(int a, int b, int c);          // 0x005C5630 (empty, ret 0xc)
    virtual void v04();                             // 0x005ED790
    virtual float v08();                            // 0x005EE350 (fld [0x007A22B4])
    virtual void v0C();                             // 0x005ED7A0
    virtual void* v10();                            // 0x005ED6E0
    virtual void v14();                             // 0x005ED890
    virtual void* v18();                            // 0x005ED520

    CClientExoAppInternal* m_pcInternal;            // 0x04

    CClientExoApp();
    ~CClientExoApp();

    CSWCQuestJournal* GetQuestJournal();
    CSWCWorldJournal* GetWorldJournal();
    int GetCaptureEvents();
    int GetCaptureEventPlayBack();
    int IsClientImeUI();
    CImeUI* GetImeUI();
    CWorldTimer* GetPauseTimer();
    CWorldTimer* GetAnimationTimer();
    CSWGuiPanel* GetPlayModulePanel();
    CExoLinkedList<CClientPlayer>* GetPlayerList();
    int IsClientInitialized();
    unsigned long GetHitCheckMaterialMask();
    CSoundSetList* GetSoundSetList();
    int GetLoadBarState();
    CClientInput* GetInputClass();
    int GetPlayerCharacterSet();
    int GetAreaNotReadyToDisplay();
    CExoArrayList<CResRef>* GetMoveToModuleMovies();
    int GetGameOver();
    int GetLoadFromIFO();
    unsigned long GetLastTarget();
    void SetRunScript(const CExoString& sScript);
    int GetCombatMode();
    int GetAutoPaused();
    int GetAttackMashTutorialTimer();
    int GetLoadingSaveGame();
    void ClearEnemySighted();
    unsigned char GetGamePlayHint();
    unsigned char GetStoryHint();
    void ResetModuleResourcesLoaded();
    int GetPazaakIsRunning();
    int GetCharacterChangeInProgress();
    int GetCreditSequenceInProgress();
    int GetAllowHighMonitorFrequency();
    int GetSoundPausedByCombat();
    void RemoveMoveToModuleMovies();

    // forwarders (b005e_client_exo_app_accessors)
    void BeginIntro();
    void JournalUpdated(int a, int b, int c);
    int GetObjectName(unsigned long oid, CExoString* pName);
    void SetCharacterFile(unsigned long nSize, const char* pData, unsigned long nValue);
    void DisplayMainMenu(int n);
    void UnloadModule();
    CSWCModule* CreateModule();
    void ExitProgram();
    CSWCModule* GetModule();
    CSWCCreature* GetPlayerCreature();
    void* GetModuleCamera();
    CGameObject* GetGameObject(unsigned long oid);
    CClientPlayer* GetPlayerByGameObjectID(unsigned long oid);
    CSWCItem* GetItemByGameObjectID(unsigned long oid);
    CSWCCreature* GetCreatureByGameObjectID(unsigned long oid);
    CSWCMessage* GetSWCMessage();                  // 0x005ED6F0
    CSWCArea* GetAreaByGameObjectID(unsigned long oid);
    CSWCTrigger* GetTriggerByGameObjectID(unsigned long oid);
    CSWCProjectile* GetProjectileByGameObjectID(unsigned long oid);
    CSWCPlaceable* GetPlaceableByGameObjectID(unsigned long oid);
    CSWCDoor* GetDoorByGameObjectID(unsigned long oid);
    CSWCAreaOfEffectObject* GetAreaOfEffectByGameObjectID(unsigned long oid);
    CSWCSoundObject* GetSoundObjectByGameObjectID(unsigned long oid);
    int GetPauseState(unsigned char nType);
    void SetPauseState(int nType, int bPause, int bForce);
    int SendSaveGameRequest(int a, int b, int c);
    int SendLoadGameRequest(int a, int b, int c);
    CClientPlayer* GetPlayerByPlayerID(unsigned long nPlayerId);
    void AttachPlayerGameObjectToArea();
    void InitializeInGameGUI();
    void ServerModeChange(unsigned char nMode);
    void AddObjectToHitcheckIgnoreList(unsigned long oid);
    void RemoveObjectFromHitcheckIgnoreList(unsigned long oid);
    void EmptyHitcheckIgnoreList();
    void SetPlayerCreature(unsigned long oid);
    int Initialize();
    int MainLoop();
    void Uninitialize();
    int CreateCaptureEventFile(const CExoString& sFile);
    int setUpEventPlayBack(const CExoString& sFile);
    void setCaptureEventRate(int nRate);
    void SetDebugMode(int nMode);
    int StartServices(int a, int b, int c, int d, int e);
    void SetEventDescriptions();
    void StopServices();
    void RestartMoviesIfNecessary();
    void AddObjectModelDirectlyToIgnoreList(unsigned long oid, CExoArrayList<CGob*>* pList);
    void StartMenuMusic(int a, int b, int c);
    void StopMenuMusic();
    void StopLoadMusic();
    unsigned char GetActivePauseState();
    void QueryExitProgram();
    void SetInputActive(int bActive, int bClear);
    void TurnOffKeyboardModal();
    int IsPlayerCreature(CGameObject* pObject);
    unsigned long ServerToClientObjectId(unsigned long oid);
    int SetInputClass(int nClass, int bForce);
    void ShowLoadScreen(int a, int b, int c);
    void HideLoadScreen(int a, int b);
    void SetPlayerCharacterName(const CExoString& sName);
    CExoString* GetPlayerCharacterName();
    void PlayLoadModuleMovies();
    int PlayMovie(int a, int b, int c, int d);
    int IsMoviePlaying();
    void AddMoveToModuleMovie(const CResRef& cMovie);
    void IndicateObject(CGameObject* pObject);
    void ResetDriveAcceleration();
    int BarkString(unsigned long oid, int a, int b, int c);
    int IsSoundPlayingInDialog();
    void StopSoundPlayingInDialog();
    void DisableInput();
    void EnableInput();
    void DismissInGameGUI();
    int GetPausedByCombat();
    void ChangeCharacter(int a, int b);
    void RunDeathSequence();
    void SetGameOver(int b);
    void SetLastPartyMemberTempKilled(int b);
    void SetCanSendDialog(int b);
    int GetCanSendDialog(int a, int b, int c);
    int GetClientMeleeAnimation(unsigned long a, unsigned long b, int c, int d, int e, int f);
    void SetLoadScreenByModuleName(const CExoString& sModule);
    void SetLoadBarProgress(int a, int b);
    void SetLastTarget(unsigned long oid, int b);
    void UpdateRoom(int n);
    void StartPazaakGame(int a, int b, int c, int d, int e);
    void EndPazaakGame(int a, int b, int c, int d);
    int PlayRumblePattern(int nPattern);
    void StopRumblePattern(int nPattern);
    void TogglePartyFollow();
    void SetCombatMode(int bCombat);
    void PlayerFlourishWeapons();
    void AddFloatyText(int a, int b, int c);
    void SetAutoPaused(int a, int b);
    void SetLoadScreenHint(int nHint);
    void EnableVideoEffect(int nEffect);
    void DisableVideoEffect();
    void DelayNextEnemyAutoPause();
    void ShowTutorialWindow(int a, int b, int c, int d);
    void ClearCachedCreatures(int b);
    void LookUpAndPerformRumbleWithCutOff(int a, int b, int c);
    int SaveOptions();
    void ChangeCharacterToNextLivingPartyMember(int a, int b, int c);
    int GetNextLoadScreenHintSTRREF(int a, int b);
    void BusyWaitMenuMusic();
    void BusyWaitMuteSound();
    void SetMouseMode(unsigned char nMode);
    void CancelMovie();
    void HideMouse(int nMask);
    void ShowMouse(int nMask);
    void FadeAndStopStreamingSoundSource(CExoStreamingSoundSource* p, unsigned long nMs);
    void StartCreditSequence(int a, int b);
    void StopCreditSequence();
    void EnableMouseAtEndOfLoadModuleMovie();
    void AddMovieToMovieQueue(int a, int b);
    void PlayMovieQueue(int b);
    CWorldTimer* GetActiveTimer(unsigned long oid);
    int IsValidResolution(int a, int b);
    void DontUseThisSetPlayerCreature(unsigned long oid);

    CExoString GetClientName();
    void SetInStartNewModule(int b);
    void SetVideoMode(int nMode);
    void SetTexturePack(int nPack);
    CResRef GetItemDropSound(unsigned char nType);
    int CreatureAcquireItem(unsigned long oidCreature, unsigned long oidItem);
    void SetStartMenuMusicOnMovieEnd(int b);
    void SetAreaNotReadyToDisplay(int b);
    void SetDialogSent(int b);
    void SetDialogModelsSent(int b);
    int GetClientRangedAnimation(unsigned long a, unsigned long b, int c, int d);
    CExoString GetLoadMusicByModuleName(const CExoString& sModule);
    void SetLoadMusic(const CExoString& sMusic);
    void SetLoadFromIFO(int b);
    void SetFutureLeader(unsigned long oid);
    CExoString GetGUIString(int nId);
    void SetAttackMashTutorialTimer(int n);
    void SetLoadingSaveGame(int b);
    void SetLoadScreenHintUpdateTimer(int n);
    void SetGamePlayHint(unsigned char b);
    void SetStoryHint(unsigned char b);
    void SetCharacterChangeInProgress(int b);
    void SetLastClickedOnTarget(unsigned long oid);
    int GetInFreeLook();
    CExoString GetDisplayCharacterForKey(int nKey);

    // callees (not matched here)
    CClientOptions* GetClientOptions();             // 0x005ED4F0
    CWorldTimer* GetWorldTimer();                   // 0x005ED4F0 (same slot as GetClientOptions)
    CClientUnknown700* GetUnknown700();             // 0x005ED700
    CSWParty* GetParty();                           // 0x005ED8B0
    CGuiInGame* GetInGameGui();                     // 0x005ED690

    // worker C r5: unnamed facade accessors
    void EmptyForwarder();                          // 0x005ED310
    int GetUnknown88();                             // 0x005ED460
    int GetUnknown144();                            // 0x005ED4C0
    unsigned long GetPlayerCreatureId();            // 0x005ED550
    CGameObjectArray* GetGameObjectArray();         // 0x005ED570
    void* GetUnknown0c();                           // 0x005ED6B0
    CSWCSaveLoad* GetSaveLoad();                    // 0x005ED6F0
    void* GetUnknown08();                           // 0x005ED710
    CSWGuiManager* GetGuiManager();                 // 0x005EDA40
    void* GetUnknown00();                           // 0x005EDAF0
    int IsNotUnknown28c(int n);                     // 0x005EDB80
    void GetPazaakResult(int* pWon, int* pWager);   // 0x005EDDE0
    void SetUnknown1a8(int a, int b, int c);        // 0x005EDE10
    void SetTutorialFlag(int n, unsigned char b);   // 0x005EDFD0
    unsigned char GetTutorialFlag(int n);           // 0x005EDFF0
    CSWCMiniGame* GetMiniGame();                    // 0x005EDB00 (OK)
    int SetPausedByCombat(int bPaused, unsigned char nFlag, int bForce);   // 0x005EDC20
    void CallUnknown5FCD10(int a, int b, const CExoString& s);    // 0x005ED360
    CClientResRefSlot* GetUnknown418(int n);        // 0x005EE020
    void SetModuleResourcesLoadedFlag(unsigned char n);   // 0x005EE0A0
    int GetModuleResourcesLoadedFlag(unsigned char n);    // 0x005EE0D0
    int IsUnknown378Positive();                     // 0x005EE0F0
    int GetPausedByCombatSet();                     // 0x005EE110
};

#endif
