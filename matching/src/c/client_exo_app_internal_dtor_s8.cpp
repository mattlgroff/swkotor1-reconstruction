// CClientExoAppInternal constructor and destructor, worker C s8
// (translation-unit-local layout: the shared header's list templates have no
// constructors or destructors).

#include <string.h>

#define OBJECT_INVALID 0x7f000000

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const char* s);           // 0x005E5140
};

extern const unsigned long OBJECT_INVALID_ID;      // 0x00746644

class CExoTimers {
public:
    unsigned __int64 GetHighResolutionTimer();      // 0x005E9260
};

class CExoBase {
public:
    void* m_pUnknown00;
    CExoTimers* m_pcExoTimers;                      // 0x04
};

extern CExoBase* g_pExoBase;                        // 0x007A39E0

class CExoLinkedListInternal {
public:
    struct CNode {
        CNode* pPrev;
        CNode* pNext;
        void* pObject;
    };
    CNode* pHead;                                   // 0x00
    void* pTail;                                    // 0x04
    unsigned long nCount;                           // 0x08

    ~CExoLinkedListInternal();                      // 0x005E9AB0
    void* RemoveHead();                             // 0x005E9C80
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;   // 0x00

    CExoLinkedList();                               // out of line per instantiation
    int IsEmpty() { return m_pcExoLinkedListInternal->pHead == 0; }
    T* RemoveHead() { return (T*)m_pcExoLinkedListInternal->RemoveHead(); }
    T* GetHead()
    {
        CExoLinkedListInternal::CNode* p = m_pcExoLinkedListInternal->pHead;
        return p ? (T*)p->pObject : 0;
    }
    ~CExoLinkedList() { delete m_pcExoLinkedListInternal; }
};

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    CExoArrayList()
    {
        num = 0;
        array_size = 0;
        element = 0;
    }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
    __forceinline void Allocate(int n)
    {
        T* pOld = element;
        array_size = n;
        element = new T[n];
        for (int i = 0; i < num; i++) {
            element[i] = pOld[i];
        }
        if (pOld) {
            delete[] pOld;
        }
    }
    __forceinline void SetSize(int n)
    {
        if (n == 0) {
            if (element) {
                delete[] element;
                element = 0;
                array_size = 0;
            }
        } else {
            Allocate(n);
        }
        num = n;
    }
    __forceinline CExoArrayList& operator=(const CExoArrayList& o)
    {
        num = 0;
        SetSize(o.num);
        for (int i = 0; i < o.num; i++) {
            element[i] = o.element[i];
        }
        return *this;
    }
};

class CResRef {
public:
    ~CResRef();                                     // 0x004015A0 (empty)
    char m_resRef[16];
};

class CClientPlayer {
public:
    char pad_0x0[0xc];
    CExoString m_sPlayerName;                       // 0x0c
    CResRef m_cResRef;                              // 0x14 (descriptive)
    char pad_0x24[0x30 - 0x24];
    CExoString m_sCommunityName;                    // 0x30 (descriptive)
};

class CServerInfo {
public:
    ~CServerInfo();                                 // 0x004B1360
};

class CClientAIMaster {
public:
    ~CClientAIMaster();                             // 0x00604B40
};

class CNetLayer {
public:
    int ShutDown();                                 // 0x005D5600
    ~CNetLayer();                                   // 0x005D5720
};

class CGameObjectArray {
public:
    ~CGameObjectArray();                            // 0x004D7E10
};

class CSWCModule {
public:
    virtual ~CSWCModule();
};

class CWorldTimer {
public:
    ~CWorldTimer();                                 // 0x004015A0 (empty)
};

class CSWCQuestJournal {
public:
    ~CSWCQuestJournal();                            // 0x00602C10
};

class CSWCWorldJournal {
public:
    ~CSWCWorldJournal();                            // 0x00676DE0
};

class CGuiInGame {
public:
    void ShutDown();                                // 0x0062C310
    ~CGuiInGame();                                  // 0x006303B0
};

class CExoImeUI {
public:
    ~CExoImeUI();                                   // 0x005F1020
};

class CSWCSaveLoad {
public:
    virtual ~CSWCSaveLoad();
};

struct CClientMessageHandler1c0 {
    int m_n00;
};

class CSoundSetList {
public:
    ~CSoundSetList();                               // 0x00678880
};

struct CItemDropSounds {
    char m_resRef[16];
};

class CSWGuiPanel {
public:
    virtual ~CSWGuiPanel();
};

class CSWGuiManager {
public:
    int PanelExists(CSWGuiPanel* pPanel);           // 0x0040BD70
    ~CSWGuiManager();                               // 0x0040AAA0
};

class CExoInput {
public:
    virtual ~CExoInput();
};

class CExoSound {
public:
    ~CExoSound();                                   // 0x005D62F0
};

class CTlkTable {
public:
    ~CTlkTable();                                   // 0x0041E100
};

class CAuroraInterface {
public:
    ~CAuroraInterface();                            // 0x0070D340 (descriptive)
};

class CAurInternal {
public:
    static void DestroyCAuroraInstance();           // 0x0044ED80
};

class CResRefName {
public:
    CResRefName(const char* s);                     // 0x00406D80 (CResRef(const char*))
    char m_resRef[16];
};

class CExoResMan {
public:
    ~CExoResMan();                                  // 0x00408670
    int RemoveResourceImageFile(const CExoString& sName);   // 0x00408830
    int RemoveFixedKeyTableFile(const CExoString& sName);   // 0x004088C0
    int RemoveResourceDirectory(const CExoString& sName);   // 0x004088D0
    int Exists(const CResRefName& cRes, unsigned short nType, int b);   // 0x00408BC0
};

extern CSWGuiManager* g_pGuiManager;                // 0x007A39F4
extern CExoInput* g_pExoInput;                      // 0x007A39E4
extern CExoSound* g_pExoSound;                      // 0x007A39EC
extern CTlkTable* g_pTlkTable;                      // 0x007A3A08
extern CAuroraInterface* g_pAuroraInterface;        // 0x007A39F0 (descriptive)
extern void* g_pRender;                             // 0x007A39F8
extern CExoResMan* g_pExoResMan;                    // 0x007A39E8
class CSWParty {
public:
    int IsPartyMember(unsigned long oid);           // 0x006349C0 (returns the NPC index)
    void SetLeader(int nIndex);                     // 0x00635480
    void SetPlayerCreature(unsigned long oid);      // 0x006364C0
    CSWParty();                                     // 0x00636170
    ~CSWParty();                                    // 0x004015A0 (empty)
    char pad_0x0[0x1290];
};

class CClientOptions {
public:
    CClientOptions();                               // 0x0061DB60
    ~CClientOptions();                              // 0x004015A0 (empty)
    char pad_0x0[0xa0];
};

class CExoStreamingSoundSource {
public:
    ~CExoStreamingSoundSource();                    // 0x005D6200
};

class CGameObject;

class CSWSCreature {
public:
    char pad_0x0[0x920];
    CExoArrayList<unsigned long> m_lstVisibleList;  // 0x920 (descriptive)
};

class CSWSPlayer {
public:
    void SetGameObject(CGameObject* pObject);       // 0x00561C40
};

class CServerExoApp {
public:
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid);     // 0x004AE770
    unsigned long ClientToServerObjectID(unsigned long oid);        // 0x004AEA30
    CExoLinkedList<CSWSPlayer>* GetPlayerList();    // 0x004AED90
};

class CAppManager {
public:
    void* m_pUnknown00;
    void* m_pClientExoApp;                          // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWPlayerControl {
public:
    int m_n00;
    void SetCreature(unsigned long oid);            // 0x00679340
};

class CSWMiniGameObjectArray {
public:
    CSWMiniGameObjectArray();                       // 0x0066BF10
    char pad_0x0[0x400];
};

// Three zero-initialised words (descriptive).
struct CClientZeroTriple {
    int m_n0, m_n4, m_n8;

    CClientZeroTriple()
    {
        m_n0 = 0;
        m_n4 = 0;
        m_n8 = 0;
    }
};

struct CClientPazaakState {
    int m_bPazaakIsRunning;                         // 0x00
    CExoString m_sPazaakScript;                     // 0x04
    int m_nPazaakWager;                             // 0x0c
    int m_bPazaakWon;                               // 0x10
    int m_nPazaakDeck;                              // 0x14

    CClientPazaakState()
    {
        m_bPazaakIsRunning = 0;
        m_bPazaakWon = 0;
        m_nPazaakWager = 0;
        m_nPazaakDeck = 0;
    }
};

struct CHostileHilite {
    unsigned long m_oidTarget;
    int m_nType;
};

struct CRumblePattern {
    int m_nPattern;
};

struct CActiveRumble {
    int m_nPattern;
};

struct CGuiStringStruct {
    unsigned long m_nStrRef;                        // 0x00
    CExoString m_sString;                           // 0x04
};

// 0x10-byte entry of the +0x418 table (descriptive).
struct CClientNamedSlot {
    CExoString m_sName;                             // 0x00
    char pad_0x8[0x8];
};

// 0x38-byte entry of the +0x4c8 list (descriptive).
struct CClientNamedEntry {
    CExoString m_sName;                             // 0x00
    char pad_0x8[0x30];
};

class CClientExoAppInternal {
public:
    CClientExoAppInternal();
    ~CClientExoAppInternal();
    int CrownFutureLeader();
    void StopServices();
    void ShutDownMoviePlayer();                     // 0x005F7190

    CSWMiniGameObjectArray* m_pUnknown00;           // 0x00
    CClientOptions* m_pClientOptions;               // 0x04
    CServerInfo* m_pServerInfo;                     // 0x08
    CClientAIMaster* m_pClientAIMaster;             // 0x0c
    CNetLayer* m_pNetLayer;                         // 0x10
    CGameObjectArray* m_pGameObjectArray;           // 0x14
    CSWCModule* m_pModule;                          // 0x18
    CExoLinkedList<CClientPlayer> m_lstPlayerList;  // 0x1c
    unsigned long m_oidPlayer;                      // 0x20
    CWorldTimer* m_pWorldTimer;                     // 0x24
    CWorldTimer* m_pExclusionTimer;                 // 0x28
    CWorldTimer* m_pPauseTimer;                     // 0x2c
    CWorldTimer* m_pAnimationTimer;                 // 0x30
    char pad_0x34[0x38 - 0x34];
    CSWCQuestJournal* m_pQuestJournal;              // 0x38
    CSWCWorldJournal* m_pWorldJournal;              // 0x3c
    CGuiInGame* m_pGuiInGame;                       // 0x40
    int m_n44;                                      // 0x44
    int m_n48;                                      // 0x48
    int m_n4c;                                      // 0x4c
    void* m_pPlayModulePanel;                       // 0x50
    int m_n54;                                      // 0x54
    unsigned long m_n58;                            // 0x58
    char pad_0x5c[0x60 - 0x5c];
    int m_n60;                                      // 0x60
    char pad_0x64[0x68 - 0x64];
    int m_n68;                                      // 0x68
    int m_n6c;                                      // 0x6c
    CClientPazaakState m_cPazaak;                   // 0x70
    void* m_pCaptureFile;                           // 0x88
    int m_bCaptureEvents;                           // 0x8c
    int m_bCaptureEventPlayBack;                    // 0x90
    int m_nUnknown94;                               // 0x94
    CExoLinkedList<void> m_lstPlayBackEvents;       // 0x98
    void* m_pInputClass;                            // 0x9c
    char pad_0xa0[0xa4 - 0xa0];
    unsigned long m_nHitCheckMaterialMask;          // 0xa4
    unsigned char m_nA8;                            // 0xa8
    char pad_0xa9[0xac - 0xa9];
    int m_nAC;                                      // 0xac
    int m_nB0;                                      // 0xb0
    int m_nB4;                                      // 0xb4
    CClientZeroTriple m_cUnknownB8;                 // 0xb8
    CClientZeroTriple m_cUnknownC4;                 // 0xc4
    int m_nD0;                                      // 0xd0
    int m_nD4;                                      // 0xd4
    char pad_0xd8[0xdc - 0xd8];
    int m_bClientImeUI;                             // 0xdc
    CExoImeUI* m_pImeUI;                            // 0xe0
    int m_nE4;                                      // 0xe4
    int m_nE8;                                      // 0xe8
    int m_nEC;                                      // 0xec
    int m_nF0;                                      // 0xf0
    int m_nUnknownF4;                               // 0xf4
    int m_nUnknownF8;                               // 0xf8
    char pad_0xfc[0x100 - 0xfc];
    int m_anKeyboardModal[7];                       // 0x100
    int m_n11C;                                     // 0x11c
    int m_n120;                                     // 0x120
    int m_n124;                                     // 0x124
    int m_n128;                                     // 0x128
    void* m_pMoviePlayer;                           // 0x12c
    char pad_0x130[0x13c - 0x130];
    CExoString m_sClientName;                       // 0x13c
    char pad_0x144[0x14c - 0x144];
    CSWCSaveLoad* m_pSaveLoad;                      // 0x14c
    int m_n150;                                     // 0x150
    int m_n154;                                     // 0x154
    char pad_0x158[0x168 - 0x158];
    CExoLinkedList<CExoString> m_lstUnknown168;     // 0x168
    int m_n16C;                                     // 0x16c
    unsigned long m_n170;                           // 0x170
    int m_n174;                                     // 0x174
    int m_n178;                                     // 0x178
    unsigned char m_n17C;                           // 0x17c
    char pad_0x17d[0x188 - 0x17d];
    int m_n188;                                     // 0x188
    int m_nLoadBarState;                            // 0x18c
    CExoLinkedList<unsigned long> m_lstHitcheckIgnore;  // 0x190
    int m_nUnknown194;                              // 0x194
    char pad_0x198[0x19c - 0x198];
    int m_n19C;                                     // 0x19c
    unsigned long m_nVideoMode;                     // 0x1a0
    unsigned char m_nTexturePack;                   // 0x1a4
    char pad_0x1a5[0x1a8 - 0x1a5];
    int m_nUnknown1a8;                              // 0x1a8
    int m_nUnknown1ac;                              // 0x1ac
    int m_n1B0;                                     // 0x1b0
    char* m_pCharacterFile;                         // 0x1b4
    unsigned long m_nCharacterFileSize;             // 0x1b8
    unsigned long m_nCharacterFileValue;            // 0x1bc
    CClientMessageHandler1c0* m_pMessageHandler1c0; // 0x1c0
    CSoundSetList* m_pSoundSetList;                 // 0x1c4
    CItemDropSounds* m_pItemDropSounds;             // 0x1c8
    unsigned char m_nPauseFlags;                    // 0x1cc
    char pad_0x1cd[0x1d0 - 0x1cd];
    CExoLinkedList<unsigned long>* m_pPauseExclusionList;   // 0x1d0
    int m_n1D4;                                     // 0x1d4
    CExoArrayList<CExoString> m_lstUnknown1d8;      // 0x1d8
    CExoString m_sUnknown1e4;                       // 0x1e4
    int m_bStartMenuMusicOnMovieEnd;                // 0x1ec
    int m_n1F0;                                     // 0x1f0
    char pad_0x1f4[0x200 - 0x1f4];
    int m_bUnknown200;                              // 0x200
    int m_bClientInitialized;                       // 0x204
    int m_bApplicationIdsMatch;                     // 0x208
    CClientZeroTriple m_cUnknown20C;                // 0x20c
    char pad_0x218[0x220 - 0x218];
    int m_bInStartNewModule;                        // 0x220
    unsigned long m_n224;                           // 0x224
    int m_bCanSendDialog;                           // 0x228
    int m_nCanSendDialogTime;                       // 0x22c
    int m_bDialogSent;                              // 0x230
    int m_bDialogModelsSent;                        // 0x234
    CExoStreamingSoundSource* m_pMenuMusic;         // 0x238
    CExoStreamingSoundSource* m_pLoadMusic;         // 0x23c
    int m_n240;                                     // 0x240
    int m_n244;                                     // 0x244
    int m_nUnknown248;                              // 0x248
    int m_n24C;                                     // 0x24c
    int m_bUnknown250;                              // 0x250
    int m_bUnknown254;                              // 0x254
    int m_n258;                                     // 0x258
    int m_n25C;                                     // 0x25c
    int m_n260;                                     // 0x260
    int m_bConnectionStalled;                       // 0x264
    int m_n268;                                     // 0x268
    int m_n26C;                                     // 0x26c
    CSWParty* m_pParty;                             // 0x270
    CSWGuiManager* m_pGuiManager;                   // 0x274
    void* m_pLoadScreen;                            // 0x278
    void* m_pUnknownPanel27c;                       // 0x27c
    CSWGuiPanel* m_pMainMenu;                       // 0x280
    CSWGuiPanel* m_pPreviousMainMenu;               // 0x284
    int m_bAreaNotReadyToDisplay;                   // 0x288
    unsigned long m_nUnknown28c;                    // 0x28c
    char pad_0x290[0x294 - 0x290];
    CExoString m_sPlayerCharacterName;              // 0x294
    int m_n29C;                                     // 0x29c
    CSWPlayerControl* m_pPlayerControl;             // 0x2a0
    float m_f2A4;                                   // 0x2a4
    CExoArrayList<unsigned long> m_lstUnknown2a8;   // 0x2a8
    unsigned long m_oidLastTarget;                  // 0x2b4
    unsigned long m_oidLastClickedOnTarget;         // 0x2b8
    int m_n2BC;                                     // 0x2bc
    int m_bGameOver;                                // 0x2c0
    int m_bGameOverSet;                             // 0x2c4
    unsigned long m_nDeathLastTime;                 // 0x2c8
    float m_fDeathElapsed;                          // 0x2cc
    float m_fDeathSpeedScale;                       // 0x2d0
    unsigned long m_oidPortraitCreature;            // 0x2d4
    CExoArrayList<CExoString> m_lstMoveToModuleMovies;  // 0x2d8
    CExoArrayList<CExoString> m_lstQueuedMovies;    // 0x2e4
    CExoArrayList<int> m_lstQueuedMovieFlags;       // 0x2f0
    CExoString m_sLoadScreenImage;                  // 0x2fc
    CExoString m_sLoadMusic;                        // 0x304
    int m_bLoadScreenEnabled;                       // 0x30c
    int m_bLoadFromIFO;                             // 0x310
    CExoArrayList<CHostileHilite> m_lstHostileHilites;  // 0x314
    int m_bCombatMode;                              // 0x320
    int m_bEnemySighted;                            // 0x324
    int m_bUnknown328;                              // 0x328
    int m_bRunScript;                               // 0x32c
    CExoString m_sRunScript;                        // 0x330
    unsigned long m_oidFutureLeader;                // 0x338
    int m_bLoadingSaveGame;                         // 0x33c
    CExoArrayList<CRumblePattern> m_lstRumblePatterns;  // 0x340
    CExoArrayList<CActiveRumble> m_lstActiveRumbles;    // 0x34c
    CExoArrayList<CGuiStringStruct> m_lstGuiStrings;    // 0x358
    unsigned long m_n364;                           // 0x364
    int m_n368;                                     // 0x368
    int m_n36C;                                     // 0x36c
    int m_n370;                                     // 0x370
    int m_nUnknown374;                              // 0x374
    float m_fUnknown378;                            // 0x378
    unsigned long m_bUnknown37c_0 : 1;              // 0x37c
    unsigned long m_bUnknown37c_1 : 1;
    unsigned long m_bPausedByCombatSet : 1;
    int m_bPausedByCombat;                          // 0x380
    unsigned long m_bAutoPaused : 1;                // 0x384
    char pad_0x388[0x38c - 0x388];
    float m_fUnknown38c;                            // 0x38c
    float m_fUnknown390;                            // 0x390
    int m_nUnknown394;                              // 0x394
    unsigned char m_nAutoPauseFlag;                 // 0x398
    char pad_0x399[0x39c - 0x399];
    float m_fUnknown39c;                            // 0x39c
    int m_nUnknown3a0;                              // 0x3a0
    int m_n3A4;                                     // 0x3a4
    int m_nVideoEffect;                             // 0x3a8
    unsigned long m_nTutorialWindowA;               // 0x3ac
    unsigned long m_nTutorialWindowB;               // 0x3b0
    int m_nTutorialWindowC;                         // 0x3b4
    unsigned char m_nTutorialWindow;                // 0x3b8
    char pad_0x3b9[0x3bc - 0x3b9];
    unsigned long m_oidCachedPlayer;                // 0x3bc
    void* m_pCachedPlayerCreature;                  // 0x3c0
    CExoStreamingSoundSource* m_pEvilEndingSound;   // 0x3c4
    unsigned long m_nMousePositionRestoreX;         // 0x3c8
    unsigned long m_nMousePositionRestoreY;         // 0x3cc
    int m_n3D0;                                     // 0x3d0
    int m_n3D4;                                     // 0x3d4
    int m_n3D8;                                     // 0x3d8
    float m_f3DC;                                   // 0x3dc
    int m_nAttackMashTutorialTimer;                 // 0x3e0
    unsigned char m_pTutorialFlags[0x30];           // 0x3e4
    unsigned char m_n414;                           // 0x414
    char pad_0x415[0x418 - 0x415];
    CClientNamedSlot m_aUnknown418[7];              // 0x418
    unsigned long m_nLoadScreenHintUpdateTimer;     // 0x488
    unsigned long m_nLoadScreenHintLastTime;        // 0x48c
    unsigned char m_bGamePlayHint;                  // 0x490
    unsigned char m_bStoryHint;                     // 0x491
    char pad_0x492[0x494 - 0x492];
    int m_nUnknown494;                              // 0x494
    unsigned char m_bModuleResourcesLoaded;         // 0x498
    char pad_0x499[0x49c - 0x499];
    int m_n49C;                                     // 0x49c
    int m_n4A0;                                     // 0x4a0
    unsigned long m_n4A4;                           // 0x4a4
    CClientZeroTriple m_cUnknown4A8;                // 0x4a8
    int m_bCharacterChangeInProgress;               // 0x4b4
    int m_bCreditSequenceInProgress;                // 0x4b8
    int m_bEvilEndingHackInProgress;                // 0x4bc
    int m_bAllowHighMonitorFrequency;               // 0x4c0
    int m_bSoundPausedByCombat;                     // 0x4c4
    CExoArrayList<CClientNamedEntry> m_lstUnknown4c8;   // 0x4c8
};

// FUNCTION: SWKOTOR 0x005FBFE0
CClientExoAppInternal::CClientExoAppInternal()
{
    m_bStartMenuMusicOnMovieEnd = 1;
    m_n1D4 = 1;
    m_bAllowHighMonitorFrequency = 0;
    m_n26C = 0;
    m_bClientInitialized = 0;
    m_nUnknown194 = 0;
    m_n19C = 0;
    m_oidPlayer = OBJECT_INVALID;
    m_nUnknown28c = OBJECT_INVALID;
    m_n44 = 0;
    m_pMainMenu = 0;
    m_pPreviousMainMenu = 0;
    m_n48 = 0;
    m_n4c = 0;
    m_pPlayModulePanel = 0;
    m_pModule = 0;
    m_n54 = 0;
    m_n68 = 0;
    m_pSaveLoad = 0;
    m_pQuestJournal = 0;
    m_pWorldJournal = 0;
    m_n6c = 0;
    m_n188 = 0;
    m_nLoadBarState = 0;
    m_pGuiManager = 0;
    m_pLoadScreen = 0;
    m_pUnknownPanel27c = 0;
    m_pServerInfo = 0;
    m_pInputClass = 0;
    m_nA8 = 0;
    m_n2BC = 0;
    m_bCombatMode = 0;
    m_bEnemySighted = 0;
    m_bUnknown328 = 0;
    m_n414 = 0;
    m_nHitCheckMaterialMask = 0;
    memset(m_anKeyboardModal, 0, sizeof(m_anKeyboardModal));
    m_nEC = 0;
    m_nF0 = 0;
    m_nE4 = 0;
    m_nE8 = 0;
    m_nUnknownF4 = 0;
    m_nUnknownF8 = 0;
    m_n11C = 0;
    m_n120 = 0;
    m_n124 = 0;
    m_n128 = 0;
    m_nD0 = 0;
    m_nD4 = 0;
    m_bClientImeUI = 0;
    m_pImeUI = 0;
    m_n150 = 0;
    m_n154 = 0;
    m_pCaptureFile = 0;
    m_bCaptureEvents = 0;
    m_bCaptureEventPlayBack = 0;
    m_nUnknown94 = 30;
    m_pGuiInGame = 0;
    m_n58 = OBJECT_INVALID_ID;
    m_n170 = OBJECT_INVALID_ID;
    m_n174 = 0;
    m_n178 = 0;
    m_n17C = 0;
    m_n16C = 0;
    m_nVideoMode = 0xffffffff;
    m_nTexturePack = 0xff;
    m_pCharacterFile = 0;
    m_nCharacterFileSize = 0;
    m_nCharacterFileValue = 0;
    m_pSoundSetList = 0;
    m_pItemDropSounds = 0;
    m_nPauseFlags = 0;
    m_pPauseExclusionList = new CExoLinkedList<unsigned long>;
    m_pWorldTimer = 0;
    m_pExclusionTimer = 0;
    m_pPauseTimer = 0;
    m_pAnimationTimer = 0;
    m_pMoviePlayer = 0;
    m_bUnknown200 = 0;
    m_n60 = 0;
    m_bApplicationIdsMatch = 0;
    m_bInStartNewModule = 0;
    m_n1F0 = 0;
    m_nAC = 0;
    m_nB0 = 0;
    m_nB4 = 0;
    m_n224 = OBJECT_INVALID_ID;
    m_pMenuMusic = 0;
    m_pLoadMusic = 0;
    m_n244 = 0;
    m_n240 = 0;
    m_nUnknown248 = 0;
    m_n24C = 0;
    m_bUnknown250 = 0;
    m_bUnknown254 = 0;
    m_n258 = 0;
    m_n25C = 0;
    m_nUnknown1a8 = 0;
    m_nUnknown1ac = 1;
    m_n260 = 0;
    m_bConnectionStalled = 0;
    m_n268 = 0;
    m_pUnknown00 = new CSWMiniGameObjectArray;
    m_pParty = new CSWParty;
    m_bAreaNotReadyToDisplay = 1;
    m_f2A4 = -1.0f;
    m_pPlayerControl = 0;
    m_n29C = 1;
    m_bGameOver = 0;
    m_bGameOverSet = 1;
    m_nDeathLastTime = (unsigned long)(g_pExoBase->m_pcExoTimers->GetHighResolutionTimer() / 1000);
    m_fDeathElapsed = 0;
    m_fDeathSpeedScale = 0;
    m_oidPortraitCreature = OBJECT_INVALID;
    m_oidLastTarget = OBJECT_INVALID;
    m_oidLastClickedOnTarget = OBJECT_INVALID;
    m_bCanSendDialog = 0;
    m_nCanSendDialogTime = 0;
    m_bDialogSent = 0;
    m_bDialogModelsSent = 0;
    m_sLoadScreenImage = "";
    m_bLoadScreenEnabled = 0;
    m_bLoadFromIFO = 0;
    m_bRunScript = 0;
    m_n1B0 = 0;
    m_bLoadingSaveGame = 0;
    m_oidFutureLeader = OBJECT_INVALID;
    m_n364 = OBJECT_INVALID;
    m_n368 = 0;
    m_n36C = 0;
    m_n370 = 0;
    m_nUnknown374 = 0;
    m_bPausedByCombatSet = 0;
    m_bPausedByCombat = 0;
    m_bAutoPaused = 0;
    m_fUnknown38c = 0;
    m_fUnknown390 = 0;
    m_nUnknown394 = 0;
    m_nUnknown3a0 = 0;
    m_nAutoPauseFlag = 0xff;
    m_fUnknown39c = 0;
    m_n3A4 = 0;
    m_nVideoEffect = -2;
    m_nTutorialWindowA = OBJECT_INVALID;
    m_nTutorialWindowB = OBJECT_INVALID;
    m_nTutorialWindowC = 0;
    m_nTutorialWindow = 0xff;
    m_oidCachedPlayer = OBJECT_INVALID;
    m_pCachedPlayerCreature = 0;
    m_n3D4 = 0;
    m_n3D0 = 0;
    m_nMousePositionRestoreY = 0;
    m_nMousePositionRestoreX = 0;
    m_n3D8 = 0;
    m_f3DC = -1.0f;
    m_nAttackMashTutorialTimer = 0;
    memset(m_pTutorialFlags, 0, 5);
    memset(m_aUnknown418, 0, sizeof(m_aUnknown418));
    m_nLoadScreenHintUpdateTimer = 0;
    m_nLoadScreenHintLastTime = 0;
    m_bGamePlayHint = 0;
    m_bStoryHint = 0;
    m_nUnknown494 = 0;
    m_bUnknown37c_0 = 0;
    m_bUnknown37c_1 = 0;
    m_fUnknown378 = 0;
    m_pClientOptions = new CClientOptions;
    m_bModuleResourcesLoaded = 0;
    m_n49C = 0;
    m_n4A0 = 0;
    m_pEvilEndingSound = 0;
    m_n4A4 = OBJECT_INVALID;
    m_bCharacterChangeInProgress = 0;
    m_bCreditSequenceInProgress = 0;
    m_bEvilEndingHackInProgress = 0;
    m_bSoundPausedByCombat = 0;
}

// FUNCTION: SWKOTOR 0x005FC6F0
CClientExoAppInternal::~CClientExoAppInternal()
{
    if (m_pParty) {
        delete m_pParty;
        m_pParty = 0;
    }
    delete m_pPauseExclusionList;
    if (m_pUnknown00) {
        delete m_pUnknown00;
        m_pUnknown00 = 0;
    }
    if (m_pEvilEndingSound) {
        delete m_pEvilEndingSound;
        m_pEvilEndingSound = 0;
    }
}

// NONMATCH (94 diff insns): inline budget; the original expands Allocate
// only in the last of the three list assignments (the first two call the
// out-of-line 0x005E03C0). Plain inline expands none, __forceinline all
// three; nothing in between was found.
// FUNCTION: SWKOTOR 0x005FB6E0
int CClientExoAppInternal::CrownFutureLeader()
{
    if (m_oidFutureLeader != OBJECT_INVALID) {
    int nIndex = m_pParty->IsPartyMember(m_oidFutureLeader);
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSCreature* pNewLeader = pServer->GetCreatureByGameObjectID(pServer->ClientToServerObjectID(m_oidFutureLeader));
    unsigned long oidOldLeader = m_oidPlayer;
    if (nIndex > 0) {
        m_pParty->SetLeader(nIndex);
    } else if (nIndex == 0) {
        CSWSPlayer* pPlayer = pServer->GetPlayerList()->GetHead();
        m_oidPlayer = m_oidFutureLeader;
        if (m_pParty) {
            m_pParty->SetPlayerCreature(m_oidFutureLeader);
        }
        pPlayer->SetGameObject((CGameObject*)pNewLeader);
    }
    if (oidOldLeader != m_oidFutureLeader) {
        CSWSCreature* pOldLeader = pServer->GetCreatureByGameObjectID(pServer->ClientToServerObjectID(oidOldLeader));
        CExoArrayList<unsigned long> lstVisible;
        lstVisible = pOldLeader->m_lstVisibleList;
        pOldLeader->m_lstVisibleList = pNewLeader->m_lstVisibleList;
        pNewLeader->m_lstVisibleList = lstVisible;
    }
    m_pPlayerControl->SetCreature(m_oidFutureLeader);
    m_oidFutureLeader = OBJECT_INVALID;
    return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00604130
void CClientExoAppInternal::StopServices()
{
    ShutDownMoviePlayer();
    if (m_pGuiManager->PanelExists(m_pMainMenu) && m_pMainMenu) {
        delete m_pMainMenu;
    }
    m_pMainMenu = 0;
    if (m_pPreviousMainMenu) {
        delete m_pPreviousMainMenu;
        m_pPreviousMainMenu = 0;
    }
    if (m_pSoundSetList) {
        delete m_pSoundSetList;
        m_pSoundSetList = 0;
    }
    if (m_pItemDropSounds) {
        delete m_pItemDropSounds;
        m_pItemDropSounds = 0;
    }
    if (m_pModule) {
        delete m_pModule;
        m_pModule = 0;
    }
    if (m_pGameObjectArray) {
        delete m_pGameObjectArray;
        m_pGameObjectArray = 0;
    }
    if (m_pWorldJournal) {
        delete m_pWorldJournal;
        m_pWorldJournal = 0;
    }
    if (m_pImeUI) {
        delete m_pImeUI;
        m_pImeUI = 0;
    }
    if (m_pGuiInGame) {
        m_pGuiInGame->ShutDown();
        delete m_pGuiInGame;
        m_pGuiInGame = 0;
    }
    if (m_pGuiManager) {
        delete m_pGuiManager;
        m_pGuiManager = 0;
        g_pGuiManager = 0;
    }
    if (m_pClientAIMaster) {
        delete m_pClientAIMaster;
        m_pClientAIMaster = 0;
    }
    while (!m_lstPlayerList.IsEmpty()) {
        CClientPlayer* pPlayer = m_lstPlayerList.RemoveHead();
        if (pPlayer) {
            delete pPlayer;
        }
    }
    if (m_pServerInfo) {
        delete m_pServerInfo;
        m_pServerInfo = 0;
    }
    if (m_pNetLayer) {
        m_pNetLayer->ShutDown();
        delete m_pNetLayer;
        m_pNetLayer = 0;
    }
    if (g_pExoInput) {
        delete g_pExoInput;
        g_pExoInput = 0;
    }
    if (m_pMenuMusic) {
        delete m_pMenuMusic;
        m_pMenuMusic = 0;
    }
    if (m_pLoadMusic) {
        delete m_pLoadMusic;
        m_pLoadMusic = 0;
    }
    if (g_pExoSound) {
        delete g_pExoSound;
        g_pExoSound = 0;
    }
    if (m_pSaveLoad) {
        delete m_pSaveLoad;
        m_pSaveLoad = 0;
    }
    if (g_pTlkTable) {
        delete g_pTlkTable;
        g_pTlkTable = 0;
    }
    if (m_pSoundSetList) {
        delete m_pSoundSetList;
        m_pSoundSetList = 0;
    }
    if (g_pAuroraInterface) {
        delete g_pAuroraInterface;
        g_pAuroraInterface = 0;
    }
    if (g_pRender) {
        CAurInternal::DestroyCAuroraInstance();
        g_pRender = 0;
    }
    if (g_pExoResMan) {
        g_pExoResMan->RemoveResourceDirectory("PORTRAITS:");
        g_pExoResMan->RemoveResourceDirectory("SERVERVAULT:");
        g_pExoResMan->RemoveFixedKeyTableFile("HD0:CHITIN");
        g_pExoResMan->RemoveResourceDirectory("OVERRIDE:");
        g_pExoResMan->RemoveResourceDirectory("HD0:MOVIES");
        g_pExoResMan->RemoveResourceDirectory("HD0:STREAMMUSIC");
        g_pExoResMan->RemoveResourceDirectory("HD0:STREAMWAVES");
        g_pExoResMan->RemoveResourceDirectory("TEMPCLIENT:");
        if (g_pExoResMan->Exists("GLOBAL", 0xbba, 0)) {
            g_pExoResMan->RemoveResourceImageFile("RIMS:GLOBAL");
        }
        g_pExoResMan->RemoveResourceDirectory("RIMS:");
        if (g_pExoResMan) {
            delete g_pExoResMan;
            g_pExoResMan = 0;
        }
    }
    if (m_pWorldTimer) {
        delete m_pWorldTimer;
        m_pWorldTimer = 0;
    }
    if (m_pExclusionTimer) {
        delete m_pExclusionTimer;
        m_pExclusionTimer = 0;
    }
    if (m_pPauseTimer) {
        delete m_pPauseTimer;
        m_pPauseTimer = 0;
    }
    if (m_pAnimationTimer) {
        delete m_pAnimationTimer;
        m_pAnimationTimer = 0;
    }
    if (m_pClientOptions) {
        delete m_pClientOptions;
        m_pClientOptions = 0;
    }
    if (m_pMessageHandler1c0) {
        delete m_pMessageHandler1c0;
        m_pMessageHandler1c0 = 0;
    }
    if (m_pPlayerControl) {
        delete m_pPlayerControl;
    }
    if (m_pQuestJournal) {
        delete m_pQuestJournal;
        m_pQuestJournal = 0;
    }
    if (m_pCharacterFile) {
        delete[] m_pCharacterFile;
        m_pCharacterFile = 0;
    }
    m_nCharacterFileSize = 0;
    m_nCharacterFileValue = 0;
}
