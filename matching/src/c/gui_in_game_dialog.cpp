// CGuiInGame dialog / pop-up cluster (b0062c_gui_in_game_dialog).

#include "c/client_exo_app.h"

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fx, float fy, float fz) { x = fx; y = fy; z = fz; }
    Vector operator-() const { return Vector(-x, -y, -z); }
    Vector operator*(float f) const { return Vector(x * f, y * f, z * f); }
    Vector operator+(const Vector& v) const { return Vector(x + v.x, y + v.y, z + v.z); }
};

class Quaternion {
public:
    float w, x, y, z;

    Vector GetDirection();                              // 0x0043E250
};

Vector Normalize(const Vector& v);                      // 0x004AB180
extern float g_fDialogCameraOffset;                     // 0x00743264

class CCameraX {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual Vector GetPosition();                       // 0x20
    virtual Quaternion GetOrientation();                // 0x24
};

class CSWGuiPanelX : public CSWGuiPanel {
public:
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38();
    virtual void SetActive(int a, int b);               // 0x3c
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void Refresh();                             // 0x70
    virtual void v74();
    virtual void SetEntry(int b);                       // 0x78
    virtual void SetReplies(int a);                     // 0x7c

    char pad_0x4[0x40];
    unsigned long m_nFlags;                             // 0x44
    char pad_0x48[0x1c];
    unsigned long m_bEnabled : 1;                       // 0x64
    char pad_0x68[0x14];
    unsigned long m_bPaused : 1;                        // 0x7c

    void ResetInput();                                  // 0x006A8A60
    void SetReason(unsigned char n);                    // 0x006C00C0
    void SetText(const CExoString& s);                  // 0x006C24A0
    void SetTransitionObject(unsigned long oid, const Vector& v);   // 0x006C7EC0
    void SetVisible(int b);                             // 0x0040B9D0
    void StartFade(int a, int b, float f, const Vector& v);    // 0x006244E0
    void SetTutorial(int a, int b, int c);              // 0x006AA0D0
    void HideSummary();                                 // 0x00625B50
    void ResetA();                                      // 0x006A74B0
    void ResetB();                                      // 0x006A7540
    void SetToDestroy(int b);                           // 0x00624A00
};

class CSWGuiAreaTransitionX : public CSWGuiPanelX {
public:
    char pad_0x80[0x3a4];
    Vector m_vPosition;                                 // 0x424
    unsigned long m_oidTarget;                          // 0x430
};

Vector operator-(const Vector& a, const Vector& b);    // 0x00423BC0
float Magnitude(const Vector& v);                       // 0x004AB100
extern float g_fAreaTransitionDistance;                 // 0x00741870

class CSWGuiBarkBubbleX : public CSWGuiPanelX {
public:
    void Hide();                                        // 0x006A9C10
    void SetDialogActive(int b);                        // 0x006A9C60
};

class CSWGuiManagerPanels {
public:
    void PlayGuiSound(int n);                           // 0x0040A140
    void AddPanel(CSWGuiPanel* pPanel, int a, int b);   // 0x0040BC70
    void SendToBack(CSWGuiPanel* pPanel);               // 0x0040BD20
    int PanelExists(CSWGuiPanel* pPanel);               // 0x0040BD70
    void PopModalPanel();                               // 0x0040BE00
    void RemovePanel(CSWGuiPanel* pPanel);              // 0x0040C830

    char pad_0x0[0x98];
    int m_bLocked;                                      // 0x98
};

class CSWCMessageX {
public:
    void TogglePause();                                 // 0x00677800
};

class CSWSAreaX {
public:
    char pad_0x0[0x2c4];
    int m_bNoTransitions;                               // 0x2c4
};

class CSWCAreaX {
public:
    CSWSAreaX* GetServerArea();                         // 0x00605640
};

class CResRefX;

class CSWCModuleX {
public:
    char pad_0x0[0x40];
    CCameraX* m_pCamera;                                // 0x40
    char pad_0x44[0x4];
    CSWCAreaX* m_pArea;                                 // 0x48
    void SetCameraModel(CResRefX* p);                   // 0x00640F70
    float SetAnimatedCamera(unsigned short n, int a, unsigned long oid);  // 0x00641010
    CSWSAreaX* GetServerArea()
    {
        if (m_pArea) {
            return m_pArea->GetServerArea();
        }
        return 0;
    }
};

class CSWCCreatureX {
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
    virtual void v90();
    virtual int GetIsInConversation();                  // 0x94
    int IsDead();                                       // 0x004EF890
};

class CSWCPlayerX {
public:
    char pad_0x0[0x1dc];
    Vector m_vPosition;                                 // 0x1dc

    CSWCCreatureX* GetCreature();                       // 0x0060FB20
};

class CSWPartyX {
public:
    int m_nMembers;                                     // 0x00
};

class CGuiStateX {
public:
    char pad_0x0[0xa4];
    CSWGuiPanelX* m_pControllerPause;                   // 0xa4
    char pad_0xa8[0xc];
    int m_bBusy;                                        // 0xb4
    char pad_0xb8[0x20];
    unsigned long m_nDialogDepth;                       // 0xd8
};

class CClientOptionsX {
public:
    char pad_0x0[0x14];
    unsigned char m_nFlags;                             // 0x14
};

class CSWSObjectX {
public:
    void ClearCamera();                                 // 0x004CC360
};

class CServerObjectX {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual CSWSObjectX* AsServerObject();              // 0x10
};

class CServerExoAppX {
public:
    CServerObjectX* GetGameObject(unsigned long oid);   // 0x004AE750
    void SetDialogState(int n);                         // 0x004AF030
};

class CClientExoAppX : public CClientExoApp {
public:
    CSWPartyX* GetPartyX();                             // 0x005ED8B0
    CSWCPlayerX* GetPlayerX();                          // 0x005ED540
    CSWCModuleX* GetModuleX();                          // 0x005ED530
    CGuiStateX* GetGuiState();                          // 0x005ED690
    int IsUnknownAd0();                                 // 0x005EDAD0
    CClientOptionsX* GetOptionsX();                     // 0x005ED700
    int IsUnknown110();                                 // 0x005EE110
    CSWCMessageX* GetMessage();                         // 0x005ED6F0
    CSWGuiManagerPanels* GetGuiManager();               // 0x005EDA40
    void UnknownEdba0();                                // 0x005EDBA0
    int GetPausedByCombatX();                           // 0x005EDC10
};

class CMsgBufferEntry {
public:
    CExoString m_sText;                                 // 0x00
    unsigned long m_nValue;                             // 0x08
    unsigned char m_nFlag;                              // 0x0c
};

class CDialogBufferEntry {
public:
    CExoString m_sSpeaker;                              // 0x00
    CExoString m_sText;                                 // 0x08
};

class CExoStringX : public CExoString {
public:
    int operator!=(const char* s) const;                // 0x005E5390
};

extern int g_bGuiFlag;                                  // 0x007A2370
extern int g_bLetterbox;                                // 0x0083390C
extern int g_bPauseFlag;                                // 0x007B92AC
extern int g_nSoundModeZero;                            // 0x0074C5EC

class CGob2 {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void LoadStuntModel(CResRef* pResRef);      // 0x20
};

class CGobModel {
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
    virtual CGob2* FindPart(unsigned long nPart, int bRecurse);  // 0x98
    virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual void vF0(); virtual void vF4(); virtual void vF8(); virtual void vFC();
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10C();
    virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11C();
    virtual void v120(); virtual void v124();
    virtual void PlayLipSync(CResRefX* p);              // 0x128
};

class CGameObjectX {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual CGobModel* GetModel();                      // 0x0c
};

class CResRefX {
public:
    char m_resRef[16];
    CResRefX();                                         // 0x00405ED0
    void CopyToString(char* p);                         // 0x00405FB0
    int IsValid();                                      // 0x00406030
    CResRefX operator=(const CResRefX& r);              // 0x00406120
    CResRefX operator=(const char* s);                  // 0x00406290
};

class CExoStreamingSoundSourceX {
public:
    CExoStreamingSoundSourceX();                        // 0x005D5A50
    ~CExoStreamingSoundSourceX();                       // 0x005D6200
    void SetPriority(int n);                            // 0x005D5BA0
    void Play(CResRefX* pResRef, int a, int b, int c, int d);   // 0x005D5B10

    void* m_pInternal;
};

class CSWGuiInGamePanel0 : public CSWGuiPanelX { public: CSWGuiInGamePanel0(CSWGuiManagerPanels* p); char pad[0x42bc - 0x80]; };
class CSWGuiInGamePanel1 : public CSWGuiPanelX { public: CSWGuiInGamePanel1(CSWGuiManagerPanels* p); char pad[0x1de8 - 0x80]; };
class CSWGuiInGamePanel2 : public CSWGuiPanelX { public: CSWGuiInGamePanel2(CSWGuiManagerPanels* p); char pad[0x59f8 - 0x80]; };
class CSWGuiInGamePanel3 : public CSWGuiPanelX { public: CSWGuiInGamePanel3(CSWGuiManagerPanels* p); char pad[0x3f98 - 0x80]; };
class CSWGuiInGamePanel4 : public CSWGuiPanelX { public: CSWGuiInGamePanel4(CSWGuiManagerPanels* p); char pad[0xaf4 - 0x80]; };
class CSWGuiInGamePanel5 : public CSWGuiPanelX { public: CSWGuiInGamePanel5(CSWGuiManagerPanels* p); char pad[0xfc8 - 0x80]; };
class CSWGuiInGamePanel6 : public CSWGuiPanelX { public: CSWGuiInGamePanel6(CSWGuiManagerPanels* p); char pad[0x10dc - 0x80]; };
class CSWGuiInGamePanel7 : public CSWGuiPanelX { public: CSWGuiInGamePanel7(CSWGuiManagerPanels* p); char pad[0x1dac - 0x80]; };

class CExoSoundSourceX {
public:
    int IsPlaying();                                    // 0x005D5AF0
    int IsLipSyncReady();                               // 0x0063E7A0
};

class CTutorialBits {
public:
    unsigned char m_ab[8];

    int Get(unsigned char n)
    {
        unsigned char nByte = n >> 3;
        unsigned char nBit = n - (nByte << 3);
        return m_ab[nByte] & (1 << nBit);
    }
    void Set(unsigned char n)
    {
        unsigned char nByte = n >> 3;
        unsigned char nBit = n - (nByte << 3);
        m_ab[nByte] |= 1 << nBit;
    }
};

class CGuiInGame {
public:
    void RemoveGUIPopUps();
    void AppendToMsgBuffer(const CExoStringX& s, unsigned long n, unsigned char b);
    void AppendToDialogBuffer(const CExoStringX& sSpeaker, const CExoString& sText);
    void ShowDialogEntry(int a, int b, int c);
    void StoreLastDialogAnimationData();
    void ClearRepliesData();
    void ShutDown();
    int LoadStuntModelsForDialog();
    int HideSWInGameGui(int bKeepPause);
    void HideGalaxyMapGui();
    void SetPaused(int bPaused, unsigned char nReason);
    int HandleDialogModels(int nUnused, unsigned long nCount, unsigned long* pIds, CResRefX* pResRefs, CResRefX* pCamera, CResRefX* pAmbient);
    void HideStoreGui();
    void ShowSoloModeQuery(const CExoString& sText);
    void HideSoloMode();
    void HideUpgradeScreen();
    void HideAreaTransition();
    void SetGlobalDialogState(int nState);
    void ShowStatusSummary();
    int ShowTutorialWindow(unsigned char nTutorial, int a, int b, int c);
    int IsOnDialogNonDependantList(const CExoString& sName);
    void UpdateDialogLipSync();
    int SetAnimatedCamera(unsigned short nCamera, int nParam);
    void EnterControllerPauseState();
    void UpdateAreaTransitionGUI();
    Vector GetDialogCameraPosition(int bOffset);

    CSWSAreaX* GetCurrentServerArea()
    {
        CSWSAreaX* pArea = 0;
        if (((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetModuleX()) {
            pArea = ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetModuleX()->GetServerArea();
        }
        return pArea;
    }
    void TogglePauseRequest()
    {
        ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetMessage()->TogglePause();
    }

    int SetStuntModelCount(unsigned long nCount);
    void StartDialogAmbientTrack(CResRefX* pResRef);
    void SetLipSyncData(unsigned long oid, const CResRefX& res);
    void SetReplyData(unsigned long i, const CExoString& sText, int a2, int a3, int a4, int a5,
                      float f6, float f7, int a8, unsigned short a9, float f10, int a11,
                      unsigned char a12, const Vector& v13, float f14, float f15, int nCount,
                      unsigned long* pIds, unsigned short* pAnims);

    void SetSWGuiStatus(int nStatus, int bDestroyPause);
    void SetCurrentPanel(int nOld, int nNew);

    char pad_0x0[0x8];
    CSWGuiPanelX* m_pPanel08;                           // 0x08
    CSWGuiPanelX* m_apPanels[8];                        // 0x0c
    int m_nCurrentPanel;                                // 0x2c
    int m_n30;                                          // 0x30
    int m_n34;                                          // 0x34
    CSWGuiManagerPanels* m_pGuiManager;                 // 0x38
    CSWGuiPanelX* m_pDialog;                            // 0x3c
    CSWGuiPanelX* m_pDialogA;                           // 0x40
    CSWGuiPanelX* m_pDialogB;                           // 0x44
    CSWGuiPanelX* m_p48;                                // 0x48
    CSWGuiBarkBubbleX* m_pBarkBubble;                   // 0x4c
    CSWGuiPanelX* m_p50;                                // 0x50
    CSWGuiPanelX* m_pContainerGui;                      // 0x54
    CSWGuiPanelX* m_p58;                                // 0x58
    CSWGuiPanelX* m_p5c;                                // 0x5c
    CSWGuiPanelX* m_p60;                                // 0x60
    CSWGuiPanelX* m_p64;                                // 0x64
    CSWGuiPanelX* m_p68;                                // 0x68
    CSWGuiPanelX* m_pGlobalFade;                        // 0x6c
    CSWGuiPanelX* m_p70;                                // 0x70
    CSWGuiPanelX* m_p74;                                // 0x74
    CSWGuiPanelX* m_p78;                                // 0x78
    CSWGuiPanelX* m_pPausePanel;                        // 0x7c
    CSWGuiPanelX* m_pGalaxyMap;                         // 0x80
    CSWGuiPanelX* m_pStore;                             // 0x84
    CSWGuiPanelX* m_pUpgrade;                           // 0x88
    CSWGuiPanelX* m_pSoloMode;                          // 0x8c
    CSWGuiPanelX* m_p90;                                // 0x90
    CSWGuiAreaTransitionX* m_pAreaTransition;           // 0x94
    CSWGuiPanelX* m_p98;                                // 0x98
    CSWGuiPanelX* m_p9c;                                // 0x9c
    CSWGuiPanelX* m_pTutorial;                          // 0xa0
    CSWGuiPanelX* m_pa4;                                // 0xa4
    CSWGuiPanelX* m_pStatusPanel;                       // 0xa8
    void* m_pPendingStatusSummary;                      // 0xac
    int m_bDialogActive;                                // 0xb0
    int m_nGlobalDialogState;                           // 0xb4
    char pad_0xb8[0x8];
    int m_nc0;                                          // 0xc0
    char pad_0xc4[0x1c];
    int m_bWasPausedByCombat;                           // 0xe0
    char pad_0xe4[0x14];
    CMsgBufferEntry* m_pMsgBuffer;                      // 0xf8
    CDialogBufferEntry* m_pDialogBuffer;                // 0xfc
    unsigned long m_nMsgBufferCount;                    // 0x100
    unsigned long m_nDialogBufferCount;                 // 0x104
    int m_bInitialized;                                 // 0x108
    char pad_0x10c[0x8];
    unsigned long m_nReplies;                           // 0x114
    CExoString* m_pReplyStrings;                        // 0x118
    void* m_apReplyData[14];                            // 0x11c
    int* m_ppReplyFlags;                                // 0x154
    unsigned long** m_ppReplyA;                         // 0x158
    unsigned short** m_ppReplyB;                        // 0x15c
    char pad_0x160[0x4];
    unsigned short m_nAnimatedCamera;                   // 0x164
    char pad_0x166[0xa];
    unsigned long m_oidDialogOwner;                     // 0x170
    char pad_0x174[0x14];
    unsigned long m_oidCameraObject;                    // 0x188
    unsigned long m_nStuntModels;                       // 0x18c
    unsigned long* m_pStuntModelIds;                    // 0x190
    CResRefX* m_pStuntModelResRefs;                     // 0x194
    unsigned long m_nDialogParticipants;                // 0x198
    unsigned long m_nLastDialogParticipants;            // 0x19c
    unsigned long* m_pDialogParticipants;               // 0x1a0
    unsigned short* m_pDialogAnimations;                // 0x1a4
    unsigned long* m_pLastDialogParticipants;           // 0x1a8
    unsigned short* m_pLastDialogAnimations;            // 0x1ac
    char pad_0x1b0[0x970];
    int m_bShotCameraInitialized;                       // 0xb20
    int m_bAnimatedCamera;                              // 0xb24
    int m_bCameraModelSet;                              // 0xb28
    float m_fAnimatedCameraTime;                        // 0xb2c
    char pad_0xb30[0x8];
    int m_bPaused;                                      // 0xb38
    int m_bPausePanelShown;                             // 0xb3c
    CExoSoundSourceX* m_pLipSyncSound;                  // 0xb40
    char pad_0xb44[0x8];
    CExoLinkedList<CExoString> m_lstDialogNonDependant; // 0xb4c
    char pad_0xb50[0x48];
    int m_bPendingFade;                                 // 0xb98
    char pad_0xb9c[0xc];
    CTutorialBits m_abTutorialSeen;                     // 0xba8
    char pad_0xbb0[0xc];
    CExoStreamingSoundSourceX* m_pDialogAmbientTrack;   // 0xbbc
    char pad_0xbc0[0x1c];
    unsigned long m_oidLipSync;                         // 0xbdc
    CResRefX m_resLipSync;                              // 0xbe0
    char pad_0xbf0[0x1c];
    Vector m_vAreaTransition;                           // 0xc0c
    unsigned long m_oidAreaTransition;                  // 0xc18
};

// FUNCTION: SWKOTOR 0x0062B150
void CGuiInGame::RemoveGUIPopUps()
{
    if (m_pGuiManager->PanelExists(m_p98)) {
        m_p98->SetActive(0x28, 1);
    }
    if (m_pGuiManager->PanelExists(m_pTutorial)) {
        m_pTutorial->SetActive(0x28, 1);
    }
    if (m_pGuiManager->PanelExists(m_pSoloMode)) {
        m_pSoloMode->SetActive(0x28, 1);
    }
    if (m_pGuiManager->PanelExists(m_pContainerGui)) {
        m_pContainerGui->SetActive(0x28, 1);
    }
}

// FUNCTION: SWKOTOR 0x0062B5C0
void CGuiInGame::AppendToMsgBuffer(const CExoStringX& s, unsigned long n, unsigned char b)
{
    if (s != "") {
        if (m_nMsgBufferCount >= 0x40) {
            m_nMsgBufferCount = 0x3f;
            for (unsigned short i = 0; i < m_nMsgBufferCount; i++) {
                m_pMsgBuffer[i] = m_pMsgBuffer[i + 1];
            }
        }
        m_pMsgBuffer[m_nMsgBufferCount].m_sText = s;
        m_pMsgBuffer[m_nMsgBufferCount].m_nValue = n;
        m_pMsgBuffer[m_nMsgBufferCount].m_nFlag = b;
        m_nMsgBufferCount++;
    }
}

// FUNCTION: SWKOTOR 0x0062B680
void CGuiInGame::AppendToDialogBuffer(const CExoStringX& sSpeaker, const CExoString& sText)
{
    if (sSpeaker != "") {
        if (m_nDialogBufferCount >= 0x40) {
            m_nDialogBufferCount = 0x3f;
            for (unsigned short i = 0; i < m_nDialogBufferCount; i++) {
                m_pDialogBuffer[i] = m_pDialogBuffer[i + 1];
            }
        }
        m_pDialogBuffer[m_nDialogBufferCount].m_sSpeaker = sSpeaker;
        m_pDialogBuffer[m_nDialogBufferCount].m_sText = sText;
        m_nDialogBufferCount++;
    }
}

// FUNCTION: SWKOTOR 0x0062B730
void CGuiInGame::ShowDialogEntry(int a, int b, int c)
{
    if (m_nReplies <= 1) {
        g_pAppManager->m_pClientExoApp->m_pcInternal->SetInputState(0x10);
    }
    if (m_bDialogActive) {
        m_pDialog->Refresh();
    } else {
        m_bDialogActive = 1;
        g_pAppManager->m_pClientExoApp->SetInputClass(3, 1);
        ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->UnknownEdba0();
        m_pBarkBubble->SetDialogActive(1);
    }
    m_pDialog->SetEntry(b);
    if (!m_pGuiManager->PanelExists(m_pDialog)) {
        int nLayer = 0;
        if (m_pDialog == m_pDialogA) {
            if (!g_bLetterbox) {
                m_p68->ResetInput();
                m_pGuiManager->AddPanel(m_p68, nLayer, 1);
                m_p60->ResetA();
                m_pGuiManager->AddPanel(m_p60, nLayer, 1);
                m_p64->ResetB();
                m_pGuiManager->AddPanel(m_p64, nLayer, 1);
            }
        } else if (m_pDialog == m_pDialogB) {
            nLayer = 2;
        }
        m_pGuiManager->AddPanel(m_pDialog, nLayer, 1);
        SetSWGuiStatus(3, 1);
    }
    m_pDialog->SetReplies(a);
}

// FUNCTION: SWKOTOR 0x0062B960
void CGuiInGame::StoreLastDialogAnimationData()
{
    m_nLastDialogParticipants = 0;
    if (m_pLastDialogParticipants) {
        delete m_pLastDialogParticipants;
        m_pLastDialogParticipants = 0;
    }
    if (m_pLastDialogAnimations) {
        delete m_pLastDialogAnimations;
        m_pLastDialogAnimations = 0;
    }
    m_nLastDialogParticipants = m_nDialogParticipants;
    if (m_nDialogParticipants) {
        m_pLastDialogParticipants = new unsigned long[m_nDialogParticipants];
        m_pLastDialogAnimations = new unsigned short[m_nDialogParticipants];
    }
    for (int i = 0; i < (int)m_nDialogParticipants; i++) {
        m_pLastDialogParticipants[i] = m_pDialogParticipants[i];
        m_pLastDialogAnimations[i] = m_pDialogAnimations[i];
    }
}

// FUNCTION: SWKOTOR 0x0062BA30
void CGuiInGame::ClearRepliesData()
{
    if (m_pReplyStrings) {
        delete[] m_pReplyStrings;
        m_pReplyStrings = 0;
    }
    if (m_apReplyData[0]) { delete m_apReplyData[0]; m_apReplyData[0] = 0; }
    if (m_apReplyData[1]) { delete m_apReplyData[1]; m_apReplyData[1] = 0; }
    if (m_apReplyData[2]) { delete m_apReplyData[2]; m_apReplyData[2] = 0; }
    if (m_apReplyData[3]) { delete m_apReplyData[3]; m_apReplyData[3] = 0; }
    if (m_apReplyData[4]) { delete m_apReplyData[4]; m_apReplyData[4] = 0; }
    if (m_apReplyData[5]) { delete m_apReplyData[5]; m_apReplyData[5] = 0; }
    if (m_apReplyData[6]) { delete m_apReplyData[6]; m_apReplyData[6] = 0; }
    if (m_apReplyData[7]) { delete m_apReplyData[7]; m_apReplyData[7] = 0; }
    if (m_apReplyData[8]) { delete m_apReplyData[8]; m_apReplyData[8] = 0; }
    if (m_apReplyData[9]) { delete m_apReplyData[9]; m_apReplyData[9] = 0; }
    if (m_apReplyData[10]) { delete m_apReplyData[10]; m_apReplyData[10] = 0; }
    if (m_apReplyData[11]) { delete m_apReplyData[11]; m_apReplyData[11] = 0; }
    if (m_apReplyData[12]) { delete m_apReplyData[12]; m_apReplyData[12] = 0; }
    if (m_apReplyData[13]) { delete m_apReplyData[13]; m_apReplyData[13] = 0; }
    for (int i = 0; i < (int)m_nReplies; i++) {
        if (m_ppReplyFlags[i]) {
            void* pA = m_ppReplyA[i];
            void* pB = m_ppReplyB[i];
            if (pA) {
                delete pA;
            }
            if (pB) {
                delete pB;
            }
        }
    }
    if (m_ppReplyA) {
        delete m_ppReplyA;
        m_ppReplyA = 0;
    }
    if (m_ppReplyB) {
        delete m_ppReplyB;
        m_ppReplyB = 0;
    }
    if (m_ppReplyFlags) {
        delete m_ppReplyFlags;
        m_ppReplyFlags = 0;
    }
    m_nReplies = 0;
}

// FUNCTION: SWKOTOR 0x0062C310
void CGuiInGame::ShutDown()
{
    if (m_bInitialized) {
        m_pGuiManager = 0;
        if (m_pBarkBubble) { delete m_pBarkBubble; m_pBarkBubble = 0; }
        if (m_p50) { delete m_p50; m_p50 = 0; }
        if (m_pContainerGui) { delete m_pContainerGui; m_pContainerGui = 0; }
        if (m_p58) { delete m_p58; m_p58 = 0; }
        if (m_p70) { delete m_p70; m_p70 = 0; }
        if (m_p74) { delete m_p74; m_p74 = 0; }
        if (m_p5c) { delete m_p5c; m_p5c = 0; }
        if (m_pPanel08) { delete m_pPanel08; m_pPanel08 = 0; }
        if (m_p60) { delete m_p60; m_p60 = 0; }
        if (m_p64) { delete m_p64; m_p64 = 0; }
        if (m_p68) { delete m_p68; m_p68 = 0; }
        if (m_pDialogA) { delete m_pDialogA; m_pDialogA = 0; }
        if (m_pDialogB) { delete m_pDialogB; m_pDialogB = 0; }
        if (m_p48) { delete m_p48; m_p48 = 0; }
        if (m_pGlobalFade) { delete m_pGlobalFade; m_pGlobalFade = 0; }
        if (m_p78) { delete m_p78; m_p78 = 0; }
        if (m_pPausePanel) { delete m_pPausePanel; m_pPausePanel = 0; }
        if (m_pSoloMode) { delete m_pSoloMode; m_pSoloMode = 0; }
        if (m_pAreaTransition) { delete m_pAreaTransition; m_pAreaTransition = 0; }
        if (m_pStatusPanel) { delete m_pStatusPanel; m_pStatusPanel = 0; }
        if (m_pPendingStatusSummary) { delete m_pPendingStatusSummary; m_pPendingStatusSummary = 0; }
        if (m_pStore) { delete m_pStore; m_pStore = 0; }
        if (m_p90) { delete m_p90; m_p90 = 0; }
        for (int i = 0; i < 8; i++) {
            if (m_apPanels[i]) { delete m_apPanels[i]; m_apPanels[i] = 0; }
        }
        if (m_pGalaxyMap) { delete m_pGalaxyMap; m_pGalaxyMap = 0; }
        if (m_pStuntModelIds) { delete m_pStuntModelIds; m_pStuntModelIds = 0; }
        if (m_pStuntModelResRefs) { delete m_pStuntModelResRefs; m_pStuntModelResRefs = 0; }
        if (m_pDialogParticipants) { delete m_pDialogParticipants; m_pDialogParticipants = 0; }
        if (m_pDialogAnimations) { delete m_pDialogAnimations; m_pDialogAnimations = 0; }
        if (m_pLastDialogParticipants) { delete m_pDialogParticipants; m_pDialogParticipants = 0; }
        if (m_pLastDialogAnimations) { delete m_pLastDialogAnimations; m_pLastDialogAnimations = 0; }
        m_nStuntModels = 0;
        ClearRepliesData();
        m_nDialogParticipants = 0;
        if (m_pDialogParticipants) { delete m_pDialogParticipants; m_pDialogParticipants = 0; }
        if (m_pDialogAnimations) { delete m_pDialogAnimations; m_pDialogAnimations = 0; }
        m_nLastDialogParticipants = 0;
        if (m_pLastDialogParticipants) { delete m_pLastDialogParticipants; m_pLastDialogParticipants = 0; }
        if (m_pLastDialogAnimations) { delete m_pLastDialogAnimations; m_pLastDialogAnimations = 0; }
        m_bPaused = 0;
        m_bPausePanelShown = 0;
        m_bInitialized = 0;
    }
}

// FUNCTION: SWKOTOR 0x0062C620
int CGuiInGame::LoadStuntModelsForDialog()
{
    for (unsigned long i = 0; i < m_nStuntModels; i++) {
        CGameObjectX* pObject = (CGameObjectX*)g_pAppManager->GetClientExoApp()->GetGameObject(m_pStuntModelIds[i]);
        if (pObject) {
            CGobModel* pModel = pObject->GetModel();
            if (pModel) {
                char sName[17];
                ((CResRefX*)m_pStuntModelResRefs)[i].CopyToString(sName);
                if (sName[0]) {
                    CGob2* pPart = pModel->FindPart(0xff, 0);
                    if (!pPart) {
                        return 0;
                    }
                    pPart->LoadStuntModel((CResRef*)sName);
                    pPart = pModel->FindPart(0xfe, 0);
                    if (pPart) {
                        pPart->LoadStuntModel((CResRef*)sName);
                    }
                }
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0062CBA0
int CGuiInGame::HideSWInGameGui(int bKeepPause)
{
    if (m_bInitialized) {
        CSWGuiManagerPanels* pGui = ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetGuiManager();
        if (pGui && !pGui->m_bLocked) {
            SetSWGuiStatus(4, 1);
            pGui->RemovePanel(m_pPanel08);
            if (g_bGuiFlag) {
                pGui->RemovePanel(m_apPanels[m_nCurrentPanel]);
            }
            m_n30 = 0;
            SetCurrentPanel(m_nCurrentPanel, -1);
            if (m_bPaused) {
                if (bKeepPause) {
                    goto destroy;
                }
                if (!g_bPauseFlag && !m_pGuiManager->PanelExists(m_pPausePanel)) {
                    m_pGuiManager->AddPanel(m_pPausePanel, 4, 1);
                    m_pGuiManager->SendToBack(m_pPausePanel);
                }
            } else {
                ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetMessage()->TogglePause();
                if (bKeepPause) {
destroy:
                    if (m_pGuiManager->PanelExists(m_pPausePanel)) {
                        m_pPausePanel->SetToDestroy(0);
                    }
                }
            }
            m_pBarkBubble->Hide();
            m_pGuiManager->PlayGuiSound(5);
            g_pExoSound->SetMusicState(g_nSoundModeZero);
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0062D1D0
void CGuiInGame::HideGalaxyMapGui()
{
    if (m_bInitialized) {
        if (((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetPausedByCombatX()) {
            g_pAppManager->m_pClientExoApp->SetPausedByCombat(0, 0, 0);
        }
        SetSWGuiStatus(4, 1);
        g_pAppManager->m_pClientExoApp->SetInputClass(0, 1);
        m_pGuiManager->RemovePanel(m_pGalaxyMap);
        if (!g_bGuiFlag) {
            if (m_pGalaxyMap) {
                delete m_pGalaxyMap;
            }
            m_pGalaxyMap = 0;
        }
        m_pGuiManager->PlayGuiSound(5);
        g_pExoSound->SetMusicState(g_nSoundModeZero);
    }
}

// FUNCTION: SWKOTOR 0x0062DEF0
void CGuiInGame::SetPaused(int bPaused, unsigned char nReason)
{
    if (m_bInitialized) {
        if (bPaused) {
            int bShown = 0;
            switch (nReason) {
            case 1: case 4: case 5: case 7: case 8: case 9: case 10: case 11:
                bShown = 1;
                if (m_pPausePanel) {
                    m_pPausePanel->SetReason(nReason);
                }
                if (m_n34 != 1 && !g_pAppManager->m_pClientExoApp->GetMiniGame()) {
                    if (m_pPausePanel && m_pGuiManager->PanelExists(m_pPausePanel)) {
                        m_pPausePanel->SetToDestroy(0);
                    }
                } else if (m_pPausePanel && !m_pGuiManager->PanelExists(m_pPausePanel) && !g_bPauseFlag) {
                    m_pGuiManager->AddPanel(m_pPausePanel, 4, bShown);
                    m_pGuiManager->SendToBack(m_pPausePanel);
                }
                break;
            }
            m_bPausePanelShown = bShown;
            m_bPaused = bPaused;
        } else {
            if (m_pPausePanel && m_pGuiManager->PanelExists(m_pPausePanel)) {
                m_pPausePanel->SetToDestroy(0);
            }
            m_bPaused = bPaused;
            m_bPausePanelShown = 0;
        }
    }
}

// FUNCTION: SWKOTOR 0x0062E020
int CGuiInGame::HandleDialogModels(int nUnused, unsigned long nCount, unsigned long* pIds, CResRefX* pResRefs, CResRefX* pCamera, CResRefX* pAmbient)
{
    SetStuntModelCount(nCount);
    for (unsigned long i = 0; i < nCount; i++) {
        if (i < m_nStuntModels && m_pStuntModelIds && m_pStuntModelResRefs) {
            m_pStuntModelIds[i] = pIds[i];
            m_pStuntModelResRefs[i] = pResRefs[i];
        }
    }
    if (!LoadStuntModelsForDialog()) {
        return 0;
    }
    if (pCamera->IsValid() && !m_bCameraModelSet) {
        CSWCModuleX* pModule = ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetModuleX();
        if (pModule) {
            pModule->SetCameraModel(pCamera);
            m_bCameraModelSet = 1;
        }
    }
    StartDialogAmbientTrack(pAmbient);
    return 1;
}

// FUNCTION: SWKOTOR 0x0062E4A0
void CGuiInGame::HideStoreGui()
{
    if (m_bInitialized) {
        if (((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetPausedByCombatX()) {
            g_pAppManager->m_pClientExoApp->SetPausedByCombat(0, 0, 0);
        }
        SetSWGuiStatus(4, 1);
        g_pAppManager->m_pClientExoApp->SetInputClass(0, 1);
        if (!g_bGuiFlag) {
            m_pStore->m_nFlags = (m_pStore->m_nFlags & ~0x200) | 0x500;
            m_pStore = 0;
        } else {
            m_pGuiManager->RemovePanel(m_pStore);
        }
        m_pGuiManager->PlayGuiSound(5);
        g_pExoSound->SetMusicState(g_nSoundModeZero);
    }
}

// FUNCTION: SWKOTOR 0x0062E550
void CGuiInGame::ShowSoloModeQuery(const CExoString& sText)
{
    if (m_bInitialized && m_pGuiManager && m_pSoloMode) {
        CSWPartyX* pParty = ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetPartyX();
        int bSeveral = 0;
        if (pParty) {
            bSeveral = pParty->m_nMembers > 1;
        }
        CSWCPlayerX* pPlayer = ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetPlayerX();
        CSWCCreatureX* pCreature = 0;
        if (pPlayer) {
            pCreature = pPlayer->GetCreature();
        }
        CSWSAreaX* pArea = 0;
        if (((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetModuleX()) {
            CSWCModuleX* pModule = ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetModuleX();
            if (pModule->m_pArea) {
                pArea = pModule->m_pArea->GetServerArea();
            }
        }
        if (((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetGuiState()->m_bBusy ||
            (pArea && pArea->m_bNoTransitions) ||
            ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->IsUnknownAd0() ||
            (pCreature && (pCreature->GetIsInConversation() || pCreature->IsDead())) ||
            !bSeveral) {
            m_pGuiManager->PlayGuiSound(2);
            return;
        }
        if (!m_bPaused) {
            ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetMessage()->TogglePause();
        }
        m_pSoloMode->SetText(sText);
        if (!m_pGuiManager->PanelExists(m_pSoloMode)) {
            m_pGuiManager->AddPanel(m_pSoloMode, 1, 1);
        }
        g_pAppManager->m_pClientExoApp->SetInputClass(2, 1);
    }
}

// FUNCTION: SWKOTOR 0x0062E6D0
void CGuiInGame::HideSoloMode()
{
    if (m_pGuiManager->PanelExists(m_pSoloMode)) {
        if (!m_bPaused) {
            ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetMessage()->TogglePause();
        }
        m_pGuiManager->PopModalPanel();
        CSWGuiPanelX* pPanel = m_pSoloMode;
        pPanel->m_nFlags &= ~0x100;
        unsigned long nFlags = pPanel->m_nFlags;
        if ((nFlags & 0x600) != 0x400) {
            pPanel->m_nFlags = (nFlags & ~0x400) | 0x200;
        }
        g_pAppManager->m_pClientExoApp->SetInputClass(0, 1);
    }
}

// FUNCTION: SWKOTOR 0x0062E870
void CGuiInGame::HideUpgradeScreen()
{
    if (m_bInitialized && m_pGuiManager && m_pUpgrade) {
        if (!m_bPaused) {
            ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetMessage()->TogglePause();
        }
        g_pAppManager->m_pClientExoApp->SetInputClass(0, 1);
        g_pExoSound->SetMusicState(g_nSoundModeZero);
        m_pUpgrade->m_nFlags = (m_pUpgrade->m_nFlags & ~0x300) | 0x400;
        m_pUpgrade = 0;
    }
}

// FUNCTION: SWKOTOR 0x0062EB10
void CGuiInGame::HideAreaTransition()
{
    CSWSAreaX* pArea = 0;
    if (((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetModuleX()) {
        pArea = ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetModuleX()->GetServerArea();
    }
    if (pArea && pArea->m_bNoTransitions) {
        if (m_pGuiManager->PanelExists(m_pAreaTransition)) {
            m_pGuiManager->SendToBack(m_pAreaTransition);
        }
        return;
    }
    if (m_bInitialized && m_pGuiManager && m_pAreaTransition) {
        Vector v;
        v.x = 0.0f;
        v.y = 0.0f;
        v.z = 0.0f;
        m_pAreaTransition->SetTransitionObject(0x7f000000, v);
        if (m_pGuiManager->PanelExists(m_pAreaTransition)) {
            m_pGuiManager->RemovePanel(m_pAreaTransition);
        }
    }
}

// FUNCTION: SWKOTOR 0x0062EC60
void CGuiInGame::SetGlobalDialogState(int nState)
{
    m_nGlobalDialogState = nState;
    if (!nState) {
        m_nc0 = nState;
        ((CServerExoAppX*)g_pAppManager->m_pServerExoApp)->SetDialogState(nState);
        CGuiStateX* pState = ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetGuiState();
        if (pState->m_nDialogDepth > 0) {
            pState->m_nDialogDepth--;
        }
        if (m_bPendingFade) {
            m_bPendingFade = 0;
            Vector v;
            v.x = 0.0f;
            v.y = 0.0f;
            v.z = 0.0f;
            if (m_pGlobalFade) {
                m_pGuiManager->AddPanel(m_pGlobalFade, 4, 1);
                m_pGlobalFade->SetVisible(1);
                m_pGlobalFade->StartFade(1, 0, 1.0f, v);
            }
        }
        g_pAppManager->m_pClientExoApp->m_pcInternal->ClearInputState(0x10);
    }
}

// FUNCTION: SWKOTOR 0x0062EF90
void CGuiInGame::ShowStatusSummary()
{
    if (m_pStatusPanel) {
        if (m_pStatusPanel->m_bEnabled &&
            (((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetOptionsX()->m_nFlags & 0x20)) {
            m_pStatusPanel->m_bPaused = (m_bPaused || ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->IsUnknown110()) ? 1 : 0;
            g_pAppManager->m_pClientExoApp->SetPausedByCombat(1, 6, 0);
            m_pGuiManager->AddPanel(m_pStatusPanel, 1, 1);
            g_pAppManager->m_pClientExoApp->SetInputClass(2, 0);
        } else {
            m_pStatusPanel->HideSummary();
        }
    }
}

// FUNCTION: SWKOTOR 0x0062F4A0
int CGuiInGame::ShowTutorialWindow(unsigned char nTutorial, int a, int b, int c)
{
    if (((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetOptionsX()->m_nFlags & 2) {
        if (nTutorial < 0x2b) {
            int bSeen = m_abTutorialSeen.Get(nTutorial);
            if (!bSeen) {
                m_abTutorialSeen.Set(nTutorial);
                m_pTutorial->SetTutorial(a, b, c);
                if (!((CClientExoAppX*)g_pAppManager->GetClientExoApp())->GetGuiManager()->PanelExists(m_pTutorial)) {
                    ((CClientExoAppX*)g_pAppManager->GetClientExoApp())->GetGuiManager()->AddPanel(m_pTutorial, 1, 1);
                }
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0062F840
int CGuiInGame::IsOnDialogNonDependantList(const CExoString& sName)
{
    if (!sName.CompareNoCase("")) {
        CExoLinkedListPosition pos = m_lstDialogNonDependant.GetHeadPos();
        CExoString* p = m_lstDialogNonDependant.GetHead();
        while (pos) {
            if (sName.CompareNoCase(p->CStr())) {
                return 1;
            }
            p = m_lstDialogNonDependant.GetNext(pos);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0062FA30
void CGuiInGame::UpdateDialogLipSync()
{
    if (m_oidLipSync != 0x7f000000 && m_pLipSyncSound && m_pLipSyncSound->IsPlaying() &&
        m_pLipSyncSound->IsLipSyncReady()) {
        CGameObjectX* pObject = (CGameObjectX*)g_pAppManager->GetClientExoApp()->GetGameObject(m_oidLipSync);
        if (pObject && pObject->GetModel()) {
            pObject->GetModel()->PlayLipSync(&m_resLipSync);
            m_resLipSync = "";
            m_oidLipSync = 0x7f000000;
        }
    }
}

// FUNCTION: SWKOTOR 0x0062BF70
int CGuiInGame::SetAnimatedCamera(unsigned short nCamera, int nParam)
{
    if (m_bCameraModelSet && nCamera >= 1000 && nCamera <= 1727) {
        CSWCModuleX* pModule = ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetModuleX();
        if (pModule) {
            m_bAnimatedCamera = 1;
            m_bShotCameraInitialized = 0;
            m_nAnimatedCamera = nCamera;
            CServerObjectX* pObject = ((CServerExoAppX*)g_pAppManager->GetServerExoApp())->GetGameObject(m_oidCameraObject);
            if (pObject && pObject->AsServerObject()) {
                pObject->AsServerObject()->ClearCamera();
            }
            m_fAnimatedCameraTime = pModule->SetAnimatedCamera(nCamera, nParam, m_oidDialogOwner);
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0062F590
void CGuiInGame::EnterControllerPauseState()
{
    CSWGuiPanelX* pPanel = ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetGuiState()->m_pControllerPause;
    if (pPanel) {
        m_pGuiManager->AddPanel(pPanel, 1, 1);
    }
    if (g_pAppManager && g_pAppManager->m_pServerExoApp) {
        m_bWasPausedByCombat = ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetPausedByCombatX();
        if (!m_bWasPausedByCombat) {
            g_pAppManager->m_pClientExoApp->SetPausedByCombat(1, 0, 1);
        }
    }
}

// FUNCTION: SWKOTOR 0x00632180
void CGuiInGame::UpdateAreaTransitionGUI()
{
    if (m_bInitialized && m_pGuiManager && m_pAreaTransition) {
        CSWSAreaX* pArea = 0;
        if (((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetModuleX()) {
            pArea = ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetModuleX()->GetServerArea();
        }
        if (pArea && m_n34 == 1) {
            Vector v = m_pAreaTransition->m_vPosition;
            if (m_pAreaTransition->m_oidTarget == m_oidAreaTransition &&
                Magnitude(v - m_vAreaTransition) < g_fAreaTransitionDistance) {
                return;
            }
            if (m_oidAreaTransition != 0x7f000000) {
                if (pArea->m_bNoTransitions) {
                    if (m_pGuiManager->PanelExists(m_pAreaTransition)) {
                        m_pGuiManager->SendToBack(m_pAreaTransition);
                    }
                    return;
                }
                m_pAreaTransition->SetTransitionObject(m_oidAreaTransition, m_vAreaTransition);
                if (!m_pGuiManager->PanelExists(m_pAreaTransition)) {
                    m_pGuiManager->AddPanel(m_pAreaTransition, 4, 1);
                    m_pGuiManager->SendToBack(m_pAreaTransition);
                }
                return;
            }
        }
        HideAreaTransition();
    }
}

// FUNCTION: SWKOTOR 0x0062B440
void CGuiInGame::SetLipSyncData(unsigned long oid, const CResRefX& res)
{
    m_oidLipSync = oid;
    m_resLipSync = res;
}

// FUNCTION: SWKOTOR 0x0062AA00
void CGuiInGame::SetSWGuiStatus(int nStatus, int bDestroyPause)
{
    switch (nStatus) {
    case 1:
        if (m_n34 != 3) {
            m_n34 = 1;
        }
        break;
    case 2:
        m_n34 = 2;
        break;
    case 3:
        if (m_n34 != 2) {
            m_n34 = 3;
        }
        break;
    case 4:
        if (m_n34 != 2) {
            m_n34 = 1;
        }
        break;
    default:
        return;
    }
    if (m_bInitialized && m_pGuiManager) {
        if (m_n34 == 1) {
            if (!m_pGuiManager->PanelExists(m_p90)) {
                m_pGuiManager->AddPanel(m_p90, 4, 1);
                m_pGuiManager->SendToBack(m_p90);
            }
            if (m_bPausePanelShown && m_pPausePanel && !m_pGuiManager->PanelExists(m_pPausePanel)) {
                m_pGuiManager->AddPanel(m_pPausePanel, 4, 1);
                m_pGuiManager->SendToBack(m_pPausePanel);
            }
        } else {
            if (m_pGuiManager->PanelExists(m_p90)) {
                m_pGuiManager->RemovePanel(m_p90);
            }
            if (bDestroyPause && m_pGuiManager->PanelExists(m_pPausePanel)) {
                m_pPausePanel->SetToDestroy(0);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0062B850
int CGuiInGame::SetStuntModelCount(unsigned long nCount)
{
    int bResult = 0;
    if (nCount) {
        if (m_pStuntModelIds) {
            delete m_pStuntModelIds;
            m_pStuntModelIds = 0;
        }
        if (m_pStuntModelResRefs) {
            delete m_pStuntModelResRefs;
            m_pStuntModelResRefs = 0;
        }
        m_nStuntModels = nCount;
        m_pStuntModelIds = new unsigned long[nCount];
        m_pStuntModelResRefs = new CResRefX[m_nStuntModels];
        if (m_pStuntModelIds && m_pStuntModelResRefs) {
            bResult = 1;
        } else {
            delete m_pStuntModelIds;
            m_pStuntModelIds = 0;
            delete m_pStuntModelResRefs;
            m_pStuntModelResRefs = 0;
            m_nStuntModels = 0;
            bResult = 0;
        }
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x0062A8B0
void CGuiInGame::StartDialogAmbientTrack(CResRefX* pResRef)
{
    if (m_pDialogAmbientTrack) {
        delete m_pDialogAmbientTrack;
        m_pDialogAmbientTrack = 0;
    }
    if (pResRef->IsValid()) {
        m_pDialogAmbientTrack = new CExoStreamingSoundSourceX();
        if (m_pDialogAmbientTrack) {
            m_pDialogAmbientTrack->SetPriority(4);
            m_pDialogAmbientTrack->Play(pResRef, 8, 0, 1, 0);
        }
    }
}

// FUNCTION: SWKOTOR 0x0062B1E0
void CGuiInGame::SetCurrentPanel(int nOld, int nNew)
{
    if (!g_bGuiFlag && nOld != nNew) {
        if (nOld != -1 && m_apPanels[nOld]) {
            m_apPanels[nOld]->m_nFlags = (m_apPanels[nOld]->m_nFlags & ~0x300) | 0x400;
            m_apPanels[nOld] = 0;
        }
        if (!m_apPanels[nNew]) {
            CSWGuiPanelX* pPanel;
            switch (nNew) {
            case 0: pPanel = new CSWGuiInGamePanel0(m_pGuiManager); break;
            case 1: pPanel = new CSWGuiInGamePanel1(m_pGuiManager); break;
            case 2: pPanel = new CSWGuiInGamePanel2(m_pGuiManager); break;
            case 3: pPanel = new CSWGuiInGamePanel3(m_pGuiManager); break;
            case 4: pPanel = new CSWGuiInGamePanel4(m_pGuiManager); break;
            case 5: pPanel = new CSWGuiInGamePanel5(m_pGuiManager); break;
            case 6: pPanel = new CSWGuiInGamePanel6(m_pGuiManager); break;
            case 7: pPanel = new CSWGuiInGamePanel7(m_pGuiManager); break;
            }
            m_apPanels[nNew] = pPanel;
        }
    }
}

// FUNCTION: SWKOTOR 0x00628750
void CGuiInGame::SetReplyData(unsigned long i, const CExoString& sText, int a2, int a3, int a4, int a5,
                              float f6, float f7, int a8, unsigned short a9, float f10, int a11,
                              unsigned char a12, const Vector& v13, float f14, float f15, int nCount,
                              unsigned long* pIds, unsigned short* pAnims)
{
    if (i < m_nReplies) {
        m_pReplyStrings[i] = sText;
        ((int*)m_apReplyData[0])[i] = a2;
        ((int*)m_apReplyData[1])[i] = a3;
        ((int*)m_apReplyData[2])[i] = a4;
        ((int*)m_apReplyData[3])[i] = a5;
        ((float*)m_apReplyData[4])[i] = f6;
        ((float*)m_apReplyData[5])[i] = f7;
        ((int*)m_apReplyData[6])[i] = a8;
        ((unsigned short*)m_apReplyData[7])[i] = a9;
        ((float*)m_apReplyData[8])[i] = f10;
        ((int*)m_apReplyData[9])[i] = a11;
        ((unsigned char*)m_apReplyData[10])[i] = a12;
        ((Vector*)m_apReplyData[11])[i] = v13;
        ((float*)m_apReplyData[12])[i] = f14;
        ((float*)m_apReplyData[13])[i] = f15;
        m_ppReplyFlags[i] = nCount;
        if (nCount) {
            m_ppReplyA[i] = new unsigned long[nCount];
            m_ppReplyB[i] = new unsigned short[nCount];
        } else {
            m_ppReplyA[i] = 0;
            m_ppReplyB[i] = 0;
        }
        for (int k = 0; k < nCount; k++) {
            m_ppReplyA[i][k] = pIds[k];
            m_ppReplyB[i][k] = pAnims[k];
        }
    }
}

// FUNCTION: SWKOTOR 0x0062ED30
Vector CGuiInGame::GetDialogCameraPosition(int bOffset)
{
    Vector vZero(0.0f, 0.0f, 0.0f);
    CSWCPlayerX* pPlayer = ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetPlayerX();
    Vector vPosition;
    if (pPlayer) {
        vPosition = pPlayer->m_vPosition;
    } else {
        vPosition = vZero;
    }
    if ((m_bShotCameraInitialized || m_bAnimatedCamera) &&
        ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetModuleX()) {
        CCameraX* pCamera = ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetModuleX()->m_pCamera;
        ((CClientExoAppX*)g_pAppManager->m_pClientExoApp)->GetPlayerX();
        if (pCamera) {
            Vector vCamera = pCamera->GetPosition();
            if (bOffset) {
                Quaternion q = pCamera->GetOrientation();
                vPosition = vCamera + Normalize(-q.GetDirection()) * 5.0f;
            } else {
                vPosition = vCamera;
            }
        }
    }
    return vPosition;
}
