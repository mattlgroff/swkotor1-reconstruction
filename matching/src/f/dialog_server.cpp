// CSWSDialog: loading and server-side conversation flow (nwsdialog.o),
// 0x0059EAA0..0x005A27B0. Own copy of the CSWSDialog layout (dialog.cpp keeps
// its own), extended with the camera and stunt members the loaders touch.

class CExoString {
public:
    CExoString();
    CExoString(const char *s);
    CExoString(const CExoString &s);
    ~CExoString();
    CExoString &operator=(const CExoString &s);
    int operator==(const char *s) const;
    int operator!=(const char *s) const;
    int operator==(const CExoString &s) const;
    int GetLength() const;
    int IsEmpty() const;
    int CompareNoCase(const char *s) const;
    int CompareNoCase(const CExoString &s) const;
    CExoString operator+(const CExoString &s) const;
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef();
    CResRef(const char *s);
    CResRef(const CResRef &r);
    CResRef operator=(const CResRef &r);
    CResRef operator=(const char *s);
    int operator==(const char *s) const;
    int IsValid() const;
    char m_resRef[16];
};

class CExoLocString {
public:
    float GetStringReadTime(int nLanguage, unsigned char nGender);
    CExoLocString();
    ~CExoLocString();
    CExoLocString &operator=(const CExoLocString &s);
    int GetString(unsigned long nLanguage, CExoString *pString, unsigned char nGender) const;
    void *m_pList;
    unsigned long m_nStrRef;
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList {
    unsigned long m_nStructIndex;
    char m_pLabel[16];
};

class Vector;

class CResGFF {
public:
    CExoLocString ReadFieldCExoLocString(CResStruct *pStruct, char *szLabel, int &bSuccess,
                                         const CExoLocString &sDefault);
    CResRef ReadFieldCResRef(CResStruct *pStruct, char *szLabel, int &bSuccess,
                             const CResRef &cDefault);
    unsigned char ReadFieldBYTE(CResStruct *pStruct, char *szLabel, int &bSuccess,
                                unsigned char nDefault);
    Vector ReadFieldVector(CResStruct *pStruct, char *szLabel, int &bSuccess,
                           const Vector &vDefault);
    int GetList(CResList *pList, CResStruct *pStruct, char *szLabel);
    unsigned long GetListCount(CResList *pList);
    int GetListElement(CResStruct *pElement, CResList *pList, unsigned long nIndex);
    void GetTopLevelStruct(CResStruct *pStruct);
    CExoString ReadFieldCExoString(CResStruct *pStruct, char *szLabel, int &bSuccess,
                                   const CExoString &sDefault);
    unsigned short ReadFieldWORD(CResStruct *pStruct, char *szLabel, int &bSuccess,
                                 unsigned short nDefault);
    unsigned long ReadFieldDWORD(CResStruct *pStruct, char *szLabel, int &bSuccess,
                                 unsigned long nDefault);
    int ReadFieldINT(CResStruct *pStruct, char *szLabel, int &bSuccess, int nDefault);
    float ReadFieldFLOAT(CResStruct *pStruct, char *szLabel, int &bSuccess, float fDefault);
};

class Vector {
public:
    Vector() {}
    Vector(float fX, float fY, float fZ) : x(fX), y(fY), z(fZ) {}
    float x, y, z;
    Vector operator-(const Vector &v) const
    {
        Vector r;
        r.x = x - v.x;
        r.y = y - v.y;
        r.z = z - v.z;
        return r;
    }
};

class CSWSArea;
class CSWSCreature;
class CSWSObject;

class CGameObject {
public:
    virtual ~CGameObject();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual CSWSObject *AsSWSObject();      // +0x10
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual CSWSCreature *AsSWSCreature();   // +0x30

    CSWSArea *GetArea();
    unsigned long GetNearestObjectByTag(const CExoString &sTag, float fRange);
    unsigned long m_idSelf;          // +0x4
    char pad_0x8[0x10];
    CExoString m_sTag;               // +0x18
};

class CSWSObject : public CGameObject {
public:
    void SetDialogDelay(float fDelay);
    void SetDialogOwner(unsigned long oidOwner);
    void StopSoundPlayingInDialog();
    int StopDialog();
    int SendDialogEntry(int nEntry);
    char pad_0x20[0x34];
    unsigned long m_oidDialogOwner;  // +0x54
    char pad_0x58[0x4];
    unsigned long m_nDialogWaitFlags; // +0x5c (placeholder name)
    char pad_0x60[0x18];
    int m_nAILevel;                  // +0x78
    char pad_0x7c[0x14];
    Vector m_vPosition;              // +0x90
};

class CSWSJournal {
public:
    int GetState(const CExoString &sTag);
    void SetDate(const CExoString &sTag, unsigned long nDate, int bFlag);
    void SetTime(const CExoString &sTag, unsigned long nTime, int bFlag);
    void SetState(const CExoString &sTag, int nState, int bFlag);
    int GetQuestEntryXP(const CExoString &sTag);
};

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;
};

class CSWSFaction {
public:
    CExoArrayList<unsigned long> m_aMembers;  // +0x0
};

class CSWSCreature : public CSWSObject {
public:
    void ClearActivityFlag(int nFlag);
    void SetActivity(int nActivity, int bOn, int bFlag);
    CSWSJournal *GetJournal();
    CSWSFaction *GetFaction();
};

class CSWSPlayer {
public:
    CGameObject *GetGameObject();
};

class CSWSClient {
public:
    virtual ~CSWSClient();
    virtual CSWSPlayer *AsSWSPlayer();       // +0x4
};

class CGameObjectArray {
public:
    char GetGameObject(unsigned long id, CGameObject **ppObject);
};

extern char g_cObjectArraySuccess;

class CWorldTimer {
public:
    void GetWorldTime(unsigned long *pnDay, unsigned long *pnTime);
};

class CSWPartyTable {
public:
    void UnstealthParty(int bFlag);
    void DistributeExperience(int nXP, int bFlag);
};

class CServerExoApp {
public:
    CGameObjectArray *GetObjectArray();
    CSWSClient *GetClientObjectByPlayerID(unsigned long nPlayerId, int bFlag);
    CWorldTimer *GetWorldTimer();
    CSWPartyTable *GetPartyTable();
    CSWSCreature *GetCreatureByGameObjectID(unsigned long oid);
    CGameObject *GetGameObject(unsigned long oid);
    unsigned long GetPlayerCharacterID();
    int GetPlayerLanguage(unsigned long nPlayer);
    int GetIgnoreConversationStartRange();
};

extern float g_fDialogSpeakerRange;  // 0x0074744C (placeholder name, 1000.0)

class CSWSDialogSpeaker {
public:
    CSWSDialogSpeaker();
    ~CSWSDialogSpeaker();
    unsigned long m_oidSpeaker;      // +0x0
    CExoString m_sTag;               // +0x4
};

class CSWSMessage {
public:
    static int GetLocStringServerTime(unsigned long nPlayer, CExoLocString *pText,
                                      CExoString *pString, float *pfSeconds,
                                      unsigned char nGender);
};

class Vector;

class CGuiInGame {
public:
    void StartGlobalFade(int nType, float fDelay, float fDuration, const Vector &vColor);
    void AppendToMsgBuffer(const CExoString &sText, unsigned long nType, unsigned char nFlag);
    int HandleDialogReplyChosen(unsigned long nPlayer, unsigned long nReply, unsigned long nEntry,
                                const CExoString &sText, unsigned long oidSpeaker,
                                unsigned char nGender, int bStarted);
    void UpdateStatus(int nType, int nValue);
    void CloseDialog();
    void SetConversationType(unsigned long nType, unsigned char nComputerType);
    void AdjustHideCount(int nDelta);
    char pad_0x0[0xc0];
    int m_bC0;                       // +0xc0 (placeholder name: AnimatedCut)
    int m_bOldHitCheck;              // +0xc4
    char pad_0xc8[0xad0];
    int m_bFadeOnBark;               // +0xb98 (placeholder name)
    int GetFadeOnBark() { return m_bFadeOnBark; }
};

struct STR_RES {
    CExoString sString;              // +0x0
    CResRef cSoundResRef;            // +0x8
    float fSoundLength;              // +0x18
};

class CTlkTable {
public:
    int Fetch(unsigned long nStrRef, STR_RES &strRes, unsigned char nGender);
};

extern CTlkTable *g_pTlkTable;       // 0x007A3A08
extern int g_bLogConversations;      // 0x00833A9C (placeholder name)

class CSWCCreatureStats {
public:
    CExoString GetFullName();
};

class CSWCCreature {
public:
    char pad_0x0[0x2f8];
    CSWCCreatureStats *m_pStats;     // +0x2f8
};

class CClientGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual CSWCCreature *AsSWCCreature();   // +0x2c
};

class CClientExoApp {
public:
    CGuiInGame *GetInGameGui();
    unsigned long ServerToClientObjectId(unsigned long oid);
    void BarkString(unsigned long oid, const CExoString &sText, const CResRef &cSound, int bFlag);
    CClientGameObject *GetGameObject(unsigned long oid);
};


class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp *m_pClientExoApp;  // +0x4
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
    CClientExoApp *GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager *g_pAppManager;

struct CExoLinkedListNode {
    CExoLinkedListNode *pPrev;
    CExoLinkedListNode *pNext;
    void *pObject;
};

class CExoLinkedListInternal {
public:
    void *RemoveHead();
    void *GetNext(CExoLinkedListNode *&pPos);
    CExoLinkedListNode *m_pHead;
    CExoLinkedListNode *m_pTail;
    int m_nCount;
};

class CSWSDialogPlayer {
public:
    unsigned long m_nPlayerId;       // +0x0
    unsigned long m_oidPlayer;       // +0x4
    unsigned char m_nPlayerType;     // +0x8
};

class CSWSDialogAnimation {
public:
    CSWSDialogAnimation();
    ~CSWSDialogAnimation();
    CExoString m_sParticipant;       // +0x0
    unsigned short m_nAnimation;     // +0x8
};

class CSWSDialogReplyLink {
public:
    CSWSDialogReplyLink();
    CResRef m_cActive;               // +0x0
    unsigned long m_nIndex;          // +0x10
    int m_bDisplayInactive;          // +0x14
};

class CSWSDialogEntryLink {
public:
    CSWSDialogEntryLink() : m_nIndex(0) {}
    CResRef m_cActive;               // +0x0
    unsigned long m_nIndex;          // +0x10
};

class CSWSDialogStuntModel {
public:
    CSWSDialogStuntModel();
    ~CSWSDialogStuntModel();
    CExoString m_sParticipant;       // +0x0
    CResRef m_cStuntModel;           // +0x8
};

class CSWSDialogBase {
public:
    CExoString m_sSpeaker;           // +0x0
    CExoLocString m_sText;           // +0x8
    CResRef m_cScript;               // +0x10
    unsigned long m_nWaitFlags;      // +0x20
    unsigned long m_nDelay;          // +0x24
    int m_n28;                       // +0x28
    int m_n2C;                       // +0x2c
    char pad_0x30[0x4];
    CResRef m_cSound;                // +0x34
    CExoString m_s44;                // +0x44
    CExoString m_sQuest;             // +0x4c
    unsigned long m_nQuestEntry;     // +0x54
    int m_nPlotIndex;                // +0x58
    float m_fPlotXPPercentage;       // +0x5c
    unsigned long m_nAnimations;     // +0x60
    CSWSDialogAnimation *m_pAnimations; // +0x64
    unsigned char m_nFadeType;       // +0x68
    Vector m_vFadeColor;             // +0x6c
    float m_fFadeLength;             // +0x78
    float m_fFadeDelay;              // +0x7c
    unsigned char m_nSoundExists;    // +0x80
};

class CSWSDialogCamera {
public:
    CExoString m_sListener;          // +0x0
    unsigned long m_nCameraAngle;    // +0x8
    int m_nCameraID;                 // +0xc
    float m_fCamHeightOffset;        // +0x10
    float m_fTarHeightOffset;        // +0x14
    unsigned short m_nCameraAnimation; // +0x18
    char pad_0x1a[0x2];
    float m_fCamFieldOfView;         // +0x1c
    int m_nCamVidEffect;             // +0x20
};

class CSWSDialogNode : public CSWSDialogBase, public CSWSDialogCamera {
public:
    ~CSWSDialogNode();
};

class CSWSDialogEntry : public CSWSDialogNode {
public:
    CSWSDialogEntry();
    ~CSWSDialogEntry();
    unsigned long m_nReplies;        // +0xa8
    CSWSDialogReplyLink *m_pReplies; // +0xac
};

class CSWSDialogReply : public CSWSDialogNode {
public:
    CSWSDialogReply();
    ~CSWSDialogReply();
    unsigned long m_nEntries;        // +0xa8
    CSWSDialogEntryLink *m_pEntries; // +0xac
    int m_nB0;                       // +0xb0
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedList();
    ~CExoLinkedList();
    int IsEmpty() { return m_pcExoLinkedListInternal->m_pHead == 0; }
    T *RemoveHead() { return (T *)m_pcExoLinkedListInternal->RemoveHead(); }
    CExoLinkedListNode *GetHeadPos() { return m_pcExoLinkedListInternal->m_pHead; }
    T *GetHeadObject()
    {
        void *pObject = m_pcExoLinkedListInternal->m_pHead;
        if (pObject)
            pObject = ((CExoLinkedListNode *)pObject)->pObject;
        return (T *)pObject;
    }
    T *GetNext(CExoLinkedListNode *&pPos) { return (T *)m_pcExoLinkedListInternal->GetNext(pPos); }
    T *GetFirst()
    {
        CExoLinkedListNode *pNode = m_pcExoLinkedListInternal->m_pHead;
        return pNode ? (T *)pNode->pObject : 0;
    }
    CExoLinkedListInternal *m_pcExoLinkedListInternal;
};


extern unsigned char g_nDefaultGender;  // 0x0073ECD0 (placeholder name)

class CSWSDialog {
public:
    CSWSDialog();
    virtual ~CSWSDialog();
    void LoadDialogCamera(CSWSDialogCamera *pCamera, CResGFF *pGFF, CResStruct *pStruct,
                          int *bSuccess);
    float SetDialogDelay(CSWSObject *pObject, CExoLocString *pText, unsigned long nMinDelay,
                         int bHasReplies, int bNoMinimum);
    void AddJournalEntry(const CExoString &sQuest, int nEntry, unsigned long nPlayerId);
    CSWSObject *GetSpeaker(CSWSObject *pObject, const CExoString &sTag);
    void ClearDialogOwnerInObject(unsigned long oidObject);
    void Cleanup();
    int LoadDialog(CResGFF *pGFF, int bLoadAll);
    int HandleReply(unsigned long nPlayerId, CSWSObject *pObject, unsigned long nReply, int bForce,
                    unsigned long nEntry, int bSkipDelay);
    CExoString GetExoStringFromLocString(const CExoLocString &cLoc, unsigned char nGender);
    void GiveDialogPlotXP(int nPlotIndex, float fXPPercentage);
    int RunScript(CGameObject *pObject, const CResRef &cScript);
    int CheckScript(CGameObject *pObject, const CResRef &cScript);
    void InitializePsuedoRandomCameraAngles(CResGFF *pGFF);
    void LoadDialogLinkedNode(CSWSDialogReplyLink *pLink, CResGFF *pGFF, CResStruct *pStruct,
                              int *bSuccess, unsigned long nMax);
    void LoadDialogLinkedNode(CSWSDialogEntryLink *pLink, CResGFF *pGFF, CResStruct *pStruct,
                              int *bSuccess, unsigned long nMax)
    {
        pLink->m_cActive = pGFF->ReadFieldCResRef(pStruct, "Active", *bSuccess, "");
        pLink->m_nIndex = pGFF->ReadFieldDWORD(pStruct, "Index", *bSuccess, 0);
        if (pLink->m_nIndex > nMax)
            *bSuccess = 0;
    }
    void LoadDialogBase(CSWSDialogBase *pNode, CResGFF *pGFF, CResStruct *pStruct,
                        unsigned long *pnDefaultDelay, int *bSuccess);
    void BarkOneLine(unsigned long nEntry);
    void RunEndConversationScript(CSWSObject *pObject, int bOwnerOnly);

    unsigned long m_nEntries;                // +0x4
    CSWSDialogEntry *m_pEntries;             // +0x8
    unsigned long m_nReplies;                // +0xc
    CSWSDialogReply *m_pReplies;             // +0x10
    unsigned long m_nStartingEntries;        // +0x14
    CSWSDialogEntryLink *m_pStartingEntries; // +0x18
    CResRef m_cEndConversation;              // +0x1c
    CResRef m_cEndConversationAbort;         // +0x2c
    int m_bSkippable;                        // +0x3c
    int m_bUnequipItems;                     // +0x40
    int m_bUnequipHItem;                     // +0x44
    unsigned long m_nStuntModels;            // +0x48
    CSWSDialogStuntModel *m_pStuntModels;    // +0x4c
    CResRef m_cCameraModel;                  // +0x50
    int m_n60;                               // +0x60
    CExoLinkedList<CSWSDialogPlayer> m_lPlayers; // +0x64
    int m_n68;                               // +0x68
    int m_bConversationStarted;              // +0x6c
    int m_n70;                               // +0x70
    int m_n74;                               // +0x74
    unsigned char m_nPlayerType;             // +0x78
    unsigned long m_oidPlayer;               // +0x7c
    unsigned long m_oidOwner;                // +0x80
    CSWSDialogSpeaker *m_pSpeakerCache;      // +0x84
    unsigned long m_nSpeakerCache;           // +0x88
    int m_bLockFirstPlayer;                  // +0x8c
    int m_n90;                               // +0x90
    unsigned long m_oid94;                   // +0x94
    unsigned long m_oid98;                   // +0x98
    unsigned long m_aCameraAngles[24];       // +0x9c
    CResRef m_cAmbientTrack;                 // +0xfc
    int m_bEndScriptRun;                     // +0x10c
    int m_bOneLiner;                         // +0x110
    CExoLinkedList<unsigned long> m_lList114; // +0x114
    CExoLinkedList<unsigned long> m_lList118; // +0x118
    int m_nOldAILevel;                       // +0x11c
    int m_n120;                              // +0x120
    int m_n124;                              // +0x124
    int m_n128;                              // +0x128
};

// FUNCTION: SWKOTOR 0x0059EAA0
void CSWSDialog::LoadDialogCamera(CSWSDialogCamera *pCamera, CResGFF *pGFF, CResStruct *pStruct,
                                  int *bSuccess)
{
    int bOk;
    pCamera->m_sListener = pGFF->ReadFieldCExoString(pStruct, "Listener", bOk, "");
    pCamera->m_nCameraAngle = pGFF->ReadFieldDWORD(pStruct, "CameraAngle", bOk, 0);
    pCamera->m_nCameraID = pGFF->ReadFieldINT(pStruct, "CameraID", bOk, 0);
    pCamera->m_fCamHeightOffset = pGFF->ReadFieldFLOAT(pStruct, "CamHeightOffset", bOk, 0.0f);
    pCamera->m_fTarHeightOffset = pGFF->ReadFieldFLOAT(pStruct, "TarHeightOffset", bOk, 0.0f);
    pCamera->m_nCameraAnimation = pGFF->ReadFieldWORD(pStruct, "CameraAnimation", bOk, 0);
    pCamera->m_nCamVidEffect = pGFF->ReadFieldINT(pStruct, "CamVidEffect", bOk, -1);
    pCamera->m_fCamFieldOfView = pGFF->ReadFieldFLOAT(pStruct, "CamFieldOfView", bOk, 0.0f);
    if (!bOk || pCamera->m_fCamFieldOfView <= 0.0f)
        pCamera->m_fCamFieldOfView = -1.0f;
    if (pCamera->m_nCameraAngle != 6)
        pCamera->m_nCameraID = -1;
}

// FUNCTION: SWKOTOR 0x0059FF50
float CSWSDialog::SetDialogDelay(CSWSObject *pObject, CExoLocString *pText, unsigned long nMinDelay,
                                 int bHasReplies, int bNoMinimum)
{
    CExoString sText;
    float fDelay = 0.0f;
    if (m_lPlayers.IsEmpty() && bHasReplies) {
        CSWSMessage::GetLocStringServerTime(0xffffffff, pText, &sText, &fDelay, m_nPlayerType);
        if (fDelay < 0.15f)
            fDelay = sText.GetLength() * 0.1125f;
    }
    if (!g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bC0 && !bNoMinimum) {
        if (fDelay < 0.01f)
            fDelay = 0.01f;
    }
    if ((unsigned long)(int)fDelay < nMinDelay)
        fDelay = (float)nMinDelay;
    pObject->SetDialogDelay(fDelay);
    return fDelay;
}

// FUNCTION: SWKOTOR 0x005A0E80
CSWSDialog::CSWSDialog()
{
    m_oidOwner = 0x7f000000;
    m_oidPlayer = 0x7f000000;
    m_pEntries = 0;
    m_nEntries = 0;
    m_pReplies = 0;
    m_nReplies = 0;
    m_pStartingEntries = 0;
    m_nStartingEntries = 0;
    m_bConversationStarted = 0;
    m_n70 = 0;
    m_n68 = 0;
    m_n74 = -1;
    m_nPlayerType = g_nDefaultGender;
    m_oid94 = 0x7f000000;
    m_oid98 = 0x7f000000;
    m_pSpeakerCache = 0;
    m_nSpeakerCache = 0;
    m_bLockFirstPlayer = 0;
    m_n90 = 0;
    m_n60 = 0;
    m_bUnequipItems = 0;
    m_bUnequipHItem = 0;
    m_bSkippable = 0;
    m_aCameraAngles[0] = 0;
    m_aCameraAngles[1] = 1;
    m_aCameraAngles[2] = 0;
    m_aCameraAngles[3] = 1;
    m_aCameraAngles[4] = 3;
    m_aCameraAngles[5] = 1;
    m_aCameraAngles[6] = 3;
    m_aCameraAngles[7] = 2;
    m_aCameraAngles[8] = 2;
    m_aCameraAngles[9] = 1;
    m_aCameraAngles[10] = 3;
    m_aCameraAngles[11] = 1;
    m_aCameraAngles[12] = 2;
    m_aCameraAngles[13] = 1;
    m_aCameraAngles[14] = 2;
    m_aCameraAngles[15] = 1;
    m_aCameraAngles[16] = 3;
    m_aCameraAngles[17] = 2;
    m_aCameraAngles[18] = 3;
    m_aCameraAngles[19] = 1;
    m_aCameraAngles[20] = 1;
    m_aCameraAngles[21] = 1;
    m_aCameraAngles[22] = 0;
    m_aCameraAngles[23] = 0;
    m_cAmbientTrack = "";
    m_bEndScriptRun = 0;
    m_bOneLiner = 0;
    m_n120 = 0;
    m_n124 = 0;
    m_n128 = 0;
    m_nOldAILevel = 2;
}

// FUNCTION: SWKOTOR 0x0059EF00
void CSWSDialog::AddJournalEntry(const CExoString &sQuest, int nEntry, unsigned long nPlayerId)
{
    if (sQuest.IsEmpty())
        return;
    CSWSClient *pClient = g_pAppManager->GetServerExoApp()->GetClientObjectByPlayerID(nPlayerId, 0);
    if (!pClient || !pClient->AsSWSPlayer())
        return;
    CGameObject *pObject = pClient->AsSWSPlayer()->GetGameObject();
    if (!pObject || !pObject->AsSWSCreature())
        return;
    CSWSCreature *pCreature = pObject->AsSWSCreature();
    unsigned long nDay;
    unsigned long nTime;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
    if (pCreature && pCreature->GetJournal()->GetState(sQuest) < nEntry) {
        pCreature->GetJournal()->SetDate(sQuest, nDay, 0);
        pCreature->GetJournal()->SetTime(sQuest, nTime, 0);
        pCreature->GetJournal()->SetState(sQuest, nEntry, 0);
        int nXP = pCreature->GetJournal()->GetQuestEntryXP(sQuest);
        if (nXP) {
            if (g_pAppManager->GetServerExoApp()->GetPartyTable())
                g_pAppManager->m_pServerExoApp->GetPartyTable()->DistributeExperience(nXP, 1);
            g_pAppManager->m_pClientExoApp->GetInGameGui()->UpdateStatus(2, nXP);
        }
    }
    CSWSFaction *pFaction = pCreature->GetFaction();
    if (!pFaction)
        return;
    int nCount = pFaction->m_aMembers.num;
    unsigned long *pMembers = pFaction->m_aMembers.element;
    if (!pMembers)
        return;
    for (int i = 0; i < nCount; i++) {
        CSWSCreature *pMember =
            g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(pMembers[i]);
        if (pMember && pMember != pCreature && pMember->GetJournal()->GetState(sQuest) < nEntry) {
            pMember->GetJournal()->SetDate(sQuest, nDay, 0);
            pMember->GetJournal()->SetTime(sQuest, nTime, 0);
            pMember->GetJournal()->SetState(sQuest, nEntry, 0);
            int nXP = pMember->GetJournal()->GetQuestEntryXP(sQuest);
            if (nXP)
                g_pAppManager->GetClientExoApp()->GetInGameGui()->UpdateStatus(2, nXP);
        }
    }
}

// FUNCTION: SWKOTOR 0x0059FC30
CSWSObject *CSWSDialog::GetSpeaker(CSWSObject *pObject, const CExoString &sTag)
{
    unsigned long oidSpeaker;
    unsigned long i;
    for (i = 0; i < m_nSpeakerCache; i++) {
        if (m_pSpeakerCache[i].m_sTag == sTag) {
            oidSpeaker = m_pSpeakerCache[i].m_oidSpeaker;
            break;
        }
    }
    if (i == m_nSpeakerCache) {
        if (pObject->m_sTag == sTag)
            oidSpeaker = pObject->m_idSelf;
        else
            oidSpeaker = pObject->GetNearestObjectByTag(sTag, g_fDialogSpeakerRange);
        CSWSDialogSpeaker *pOld = m_pSpeakerCache;
        m_pSpeakerCache = new CSWSDialogSpeaker[m_nSpeakerCache + 1];
        for (unsigned long k = 0; k < m_nSpeakerCache; k++) {
            m_pSpeakerCache[k].m_sTag = pOld[k].m_sTag;
            m_pSpeakerCache[k].m_oidSpeaker = pOld[k].m_oidSpeaker;
        }
        m_pSpeakerCache[m_nSpeakerCache].m_sTag = sTag;
        m_pSpeakerCache[m_nSpeakerCache].m_oidSpeaker = oidSpeaker;
        m_nSpeakerCache++;
        delete[] pOld;
    }
    if (oidSpeaker == 0x7f000000)
        return 0;
    CGameObject *pFound;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidSpeaker, &pFound) !=
        g_cObjectArraySuccess)
        return 0;
    CSWSObject *pSpeaker = (CSWSObject *)pFound;
    Vector vDiff = pSpeaker->m_vPosition - pObject->m_vPosition;
    if (pSpeaker->GetArea() == pObject->GetArea() &&
        !(vDiff.x * vDiff.x + vDiff.y * vDiff.y + vDiff.z * vDiff.z >
          g_fDialogSpeakerRange * g_fDialogSpeakerRange)) {
        if (pSpeaker->m_oidDialogOwner != 0x7f000000 && pSpeaker->m_oidDialogOwner != m_oidOwner) {
            if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(
                    pSpeaker->m_oidDialogOwner, &pFound) == g_cObjectArraySuccess &&
                ((CSWSObject *)pFound)->m_oidDialogOwner == pSpeaker->m_oidDialogOwner)
                return 0;
            pSpeaker->SetDialogOwner(0x7f000000);
        }
        if (pSpeaker->m_oidDialogOwner == 0x7f000000) {
            pSpeaker->SetDialogOwner(m_oidOwner);
            if (pSpeaker->AsSWSCreature()) {
                pSpeaker->AsSWSCreature()->ClearActivityFlag(2);
                pSpeaker->AsSWSCreature()->SetActivity(4, 1, 0);
            }
        }
        return pSpeaker;
    }
    if (pSpeaker->m_oidDialogOwner == m_oidOwner)
        ClearDialogOwnerInObject(oidSpeaker);
    return 0;
}

// NONMATCH (5): the reply loop addresses [edi+reg] where cl emits [reg+edi]
// (the identical entry loop above matches); counter type / pointer-arith /
// inline-helper shapes tried.
// FUNCTION: SWKOTOR 0x005A27B0
void CSWSDialog::Cleanup()
{
    CGameObject *pOwner = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidOwner);
    if (pOwner && pOwner->AsSWSObject()) {
        int nAILevel = m_nOldAILevel;
        pOwner->AsSWSObject()->m_nAILevel = nAILevel;
        pOwner->AsSWSObject()->StopSoundPlayingInDialog();
    }
    ClearDialogOwnerInObject(m_oidOwner);
    if (m_lPlayers.IsEmpty())
        ClearDialogOwnerInObject(m_oidPlayer);
    while (!m_lPlayers.IsEmpty()) {
        CSWSDialogPlayer *pPlayer = m_lPlayers.RemoveHead();
        g_pAppManager->GetClientExoApp()->GetInGameGui()->CloseDialog();
        CGameObject *pFound;
        unsigned long oidPlayer = pPlayer->m_oidPlayer;
        if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(
                oidPlayer, &pFound) == g_cObjectArraySuccess) {
            CSWSObject *pObject = (CSWSObject *)pFound;
            if (pObject->m_oidDialogOwner == m_oidOwner) {
                pObject->SetDialogOwner(0x7f000000);
                if (pObject->AsSWSCreature())
                    pObject->AsSWSCreature()->SetActivity(4, 0, 0);
            }
        }
        delete pPlayer;
    }
    unsigned long i;
    if (m_pSpeakerCache) {
        for (i = 0; i < m_nSpeakerCache; i++)
            ClearDialogOwnerInObject(m_pSpeakerCache[i].m_oidSpeaker);
        delete[] m_pSpeakerCache;
        m_pSpeakerCache = 0;
        m_nSpeakerCache = 0;
    }
    if (m_pEntries) {
        for (i = 0; i < m_nEntries; i++) {
            if (m_pEntries[i].m_pReplies) {
                delete[] m_pEntries[i].m_pReplies;
                m_pEntries[i].m_pReplies = 0;
            }
            if (m_pEntries[i].m_pAnimations) {
                delete[] m_pEntries[i].m_pAnimations;
                m_pEntries[i].m_pAnimations = 0;
                m_pEntries[i].m_nAnimations = 0;
            }
        }
        delete[] m_pEntries;
        m_pEntries = 0;
        m_nEntries = 0;
    }
    if (m_pReplies) {
        for (unsigned long j = 0; j < m_nReplies; j++) {
            if (m_pReplies[j].m_pEntries) {
                delete[] m_pReplies[j].m_pEntries;
                m_pReplies[j].m_pEntries = 0;
            }
            if (m_pReplies[j].m_pAnimations) {
                delete[] m_pReplies[j].m_pAnimations;
                m_pReplies[j].m_pAnimations = 0;
                m_pReplies[j].m_nAnimations = 0;
            }
        }
        delete[] m_pReplies;
        m_pReplies = 0;
        m_nReplies = 0;
    }
    if (m_pStartingEntries) {
        delete[] m_pStartingEntries;
        m_pStartingEntries = 0;
        m_nStartingEntries = 0;
    }
    m_bConversationStarted = 0;
    m_n70 = 0;
    m_n68 = 0;
    m_n74 = -1;
    m_oidOwner = 0x7f000000;
    m_oidPlayer = 0x7f000000;
    m_nPlayerType = g_nDefaultGender;
    m_bLockFirstPlayer = 0;
    m_n90 = 0;
}

// FUNCTION: SWKOTOR 0x005A1050
void CSWSDialog::BarkOneLine(unsigned long nEntry)
{
    CExoString sText;
    CResRef cSound;
    unsigned long nPlayer = g_pAppManager->GetServerExoApp()->GetPlayerCharacterID();
    m_pEntries[nEntry].m_sText.GetString(
        g_pAppManager->GetServerExoApp()->GetPlayerLanguage(nPlayer), &sText, m_nPlayerType);
    cSound = m_pEntries[nEntry].m_cSound;
    if (g_pAppManager->m_pClientExoApp->GetInGameGui()->GetFadeOnBark()) {
        g_pAppManager->GetClientExoApp()->GetInGameGui()->m_bFadeOnBark = 0;
        g_pAppManager->GetClientExoApp()->GetInGameGui()->StartGlobalFade(1, 0.0f, 1.0f,
                                                                        Vector(0.0f, 0.0f, 0.0f));
    }
    if (!cSound.IsValid()) {
        STR_RES strRes;
        unsigned long nStrRef = m_pEntries[nEntry].m_sText.m_nStrRef;
        g_pTlkTable->Fetch(nStrRef, strRes, g_nDefaultGender);
        cSound = strRes.cSoundResRef;
    }
    if (!sText.CompareNoCase("")) {
        CGameObject *pPlayer = g_pAppManager->m_pServerExoApp->GetGameObject(nPlayer);
        if (pPlayer) {
            unsigned long oidSpeaker;
            if (m_pEntries->m_sSpeaker == "")
                oidSpeaker = m_oidOwner;
            else
                oidSpeaker = pPlayer->AsSWSObject()->GetNearestObjectByTag(m_pEntries->m_sSpeaker, 0.0f);
            unsigned long oidClient = g_pAppManager->GetClientExoApp()->ServerToClientObjectId(oidSpeaker);
            int bIgnoreRange = g_pAppManager->m_pServerExoApp->GetIgnoreConversationStartRange();
            g_pAppManager->GetClientExoApp()->BarkString(oidClient, sText, cSound, bIgnoreRange);
            if (g_bLogConversations && nPlayer != 0x7f000000) {
                CClientGameObject *pObject = g_pAppManager->GetClientExoApp()->GetGameObject(oidClient);
                CExoString sName;
                if (pObject && pObject->AsSWCCreature()) {
                    CSWCCreature *pCreature = pObject->AsSWCCreature();
                    if (pCreature) {
                        CSWCCreatureStats *pStats = pCreature->m_pStats;
                        sName = pStats->GetFullName();
                        {
                            CExoString sSeparator(": ");
                            g_pAppManager->m_pClientExoApp->GetInGameGui()->AppendToMsgBuffer(
                                sName + sSeparator + sText, 1, 0);
                        }
                    }
                }
            }
            CGameObject *pOwner = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidOwner);
            if (pOwner) {
                CSWSObject *pOwnerObject = pOwner->AsSWSObject();
                if (pOwnerObject) {
                    m_bConversationStarted = 1;
                    RunEndConversationScript(pOwnerObject, 1);
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0059F5F0
void CSWSDialog::LoadDialogBase(CSWSDialogBase *pNode, CResGFF *pGFF, CResStruct *pStruct,
                                unsigned long *pnDefaultDelay, int *bSuccess)
{
    int bLocal = 0;
    int bFadeType = 0;
    CResStruct cElement;
    CResList cList;
    pNode->m_sText = pGFF->ReadFieldCExoLocString(pStruct, "Text", *bSuccess, CExoLocString());
    pNode->m_cScript = pGFF->ReadFieldCResRef(pStruct, "Script", bLocal, "");
    pNode->m_sSpeaker = pGFF->ReadFieldCExoString(pStruct, "Speaker", bLocal, "");
    pNode->m_nWaitFlags = pGFF->ReadFieldDWORD(pStruct, "WaitFlags", bLocal, 0);
    pNode->m_sQuest = pGFF->ReadFieldCExoString(pStruct, "Quest", bLocal, "");
    pNode->m_nQuestEntry = pGFF->ReadFieldDWORD(pStruct, "QuestEntry", bLocal, 0);
    pNode->m_nPlotIndex = pGFF->ReadFieldINT(pStruct, "PlotIndex", bLocal, 0);
    pNode->m_fPlotXPPercentage = pGFF->ReadFieldFLOAT(pStruct, "PlotXPPercentage", bLocal, 0.0f);
    pNode->m_nDelay = pGFF->ReadFieldDWORD(pStruct, "Delay", bLocal, 0);
    pNode->m_nFadeType = pGFF->ReadFieldBYTE(pStruct, "FadeType", bFadeType, 0);
    pNode->m_vFadeColor = pGFF->ReadFieldVector(pStruct, "FadeColor", bLocal, Vector(0.0f, 0.0f, 0.0f));
    pNode->m_fFadeDelay = pGFF->ReadFieldFLOAT(pStruct, "FadeDelay", bLocal, 0.0f);
    pNode->m_fFadeLength = pGFF->ReadFieldFLOAT(pStruct, "FadeLength", bLocal, 0.0f);
    pNode->m_n28 = -1;
    pNode->m_cSound = pGFF->ReadFieldCResRef(pStruct, "Sound", bLocal, "");
    CResRef cVO = pGFF->ReadFieldCResRef(pStruct, "VO_ResRef", bLocal, "");
    pNode->m_nSoundExists = pGFF->ReadFieldBYTE(pStruct, "SoundExists", bLocal, 0x80);
    if (!pNode->m_cSound.IsValid()) {
        pNode->m_cSound = cVO;
        if (!pNode->m_cSound.IsValid()) {
            pNode->m_nSoundExists = 0;
            pNode->m_n2C = 1;
        }
    } else {
        pNode->m_n2C = 1;
    }
    if (pNode->m_nDelay == 0xffffffff) {
        if (!pNode->m_cSound.IsValid()) {
            if (!pNode->m_nWaitFlags) {
                pNode->m_cSound = "";
            } else {
                pNode->m_n28 = 0;
                pNode->m_nDelay = *pnDefaultDelay;
            }
        } else {
            pNode->m_nDelay = *pnDefaultDelay;
            if (!pNode->m_nWaitFlags)
                pNode->m_nWaitFlags = 2;
        }
    } else {
        pNode->m_nWaitFlags |= 0x10;
    }
    if (pNode->m_nFadeType == 1 || pNode->m_nFadeType == 2)
        pNode->m_fFadeLength = 0.0f;
    if (!pNode->m_nFadeType || !bFadeType) {
        pNode->m_fFadeDelay = 0.0f;
        pNode->m_fFadeLength = 0.0f;
        pNode->m_nFadeType = 0;
        pNode->m_vFadeColor = Vector(0.0f, 0.0f, 0.0f);
    }
    pGFF->GetList(&cList, pStruct, "AnimList");
    unsigned long nCount = pGFF->GetListCount(&cList);
    pNode->m_nAnimations = nCount;
    if (nCount > 0)
        pNode->m_pAnimations = new CSWSDialogAnimation[nCount];
    for (unsigned long i = 0; i < pNode->m_nAnimations; i++) {
        pGFF->GetListElement(&cElement, &cList, i);
        pNode->m_pAnimations[i].m_sParticipant =
            pGFF->ReadFieldCExoString(&cElement, "Participant", *bSuccess, "");
        pNode->m_pAnimations[i].m_nAnimation =
            pGFF->ReadFieldWORD(&cElement, "Animation", *bSuccess, 0);
    }
}

// NONMATCH (56): the StuntList allocation keeps the null result in ebp,
// zeroed before the count test, and its loop top is not aligned; ternary,
// local-pointer and local-count shapes tried. Everything before it matches.
// FUNCTION: SWKOTOR 0x005A2AE0
int CSWSDialog::LoadDialog(CResGFF *pGFF, int bLoadAll)
{
    int bSuccess = 1;
    CResStruct cTop;
    int bOptional = 1;
    unsigned long i;
    unsigned long j;
    unsigned char nComputerType;
    CResStruct cLink;
    CResStruct cElement;
    CResStruct cStunt;
    InitializePsuedoRandomCameraAngles(pGFF);
    m_aCameraAngles[22] = 1;
    m_aCameraAngles[23] = 1;
    Cleanup();
    pGFF->GetTopLevelStruct(&cTop);
    m_cCameraModel = pGFF->ReadFieldCResRef(&cTop, "CameraModel", bOptional, "");
    unsigned long nDelayEntry = pGFF->ReadFieldDWORD(&cTop, "DelayEntry", bSuccess, 0);
    pGFF->ReadFieldDWORD(&cTop, "DelayReply", bSuccess, 0);
    m_cEndConversation = pGFF->ReadFieldCResRef(&cTop, "EndConversation", bOptional, "");
    m_cEndConversationAbort = pGFF->ReadFieldCResRef(&cTop, "EndConverAbort", bOptional, "");
    m_bSkippable = pGFF->ReadFieldBYTE(&cTop, "Skippable", bOptional, 1);
    int nConversationType = pGFF->ReadFieldINT(&cTop, "ConversationType", bOptional, 0);
    nComputerType = pGFF->ReadFieldBYTE(&cTop, "ComputerType", bOptional, 0);
    m_cAmbientTrack = pGFF->ReadFieldCResRef(&cTop, "AmbientTrack", bOptional, "");
    m_bUnequipItems = pGFF->ReadFieldBYTE(&cTop, "UnequipItems", bOptional, 0);
    m_bUnequipHItem = pGFF->ReadFieldBYTE(&cTop, "UnequipHItem", bOptional, 0);
    int bAnimatedCut = pGFF->ReadFieldBYTE(&cTop, "AnimatedCut", bOptional, 0);
    int bOldHitCheck = pGFF->ReadFieldBYTE(&cTop, "OldHitCheck", bOptional, 0);
    g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bC0 = bAnimatedCut;
    g_pAppManager->GetClientExoApp()->GetInGameGui()->m_bOldHitCheck = bOldHitCheck;
    if (nConversationType == 2)
        m_bOneLiner = 1;
    if (nConversationType == 1) {
        g_pAppManager->GetClientExoApp()->GetInGameGui()->SetConversationType(1, nComputerType);
    } else {
        g_pAppManager->GetClientExoApp()->GetInGameGui()->SetConversationType(0, 0);
        g_pAppManager->m_pServerExoApp->GetPartyTable()->UnstealthParty(1);
    }
    if (bAnimatedCut)
        g_pAppManager->m_pClientExoApp->GetInGameGui()->AdjustHideCount(1);

    CResList cList;
    pGFF->GetList(&cList, &cTop, "ReplyList");
    m_nReplies = pGFF->GetListCount(&cList);
    pGFF->GetList(&cList, &cTop, "EntryList");
    m_nEntries = pGFF->GetListCount(&cList);
    m_pEntries = new CSWSDialogEntry[m_nEntries];

    CResList cLinks;
    for (i = 0; bSuccess && i < m_nEntries; i++) {
        pGFF->GetListElement(&cElement, &cList, i);
        LoadDialogBase(&m_pEntries[i], pGFF, &cElement, &nDelayEntry, &bSuccess);
        LoadDialogCamera(&m_pEntries[i], pGFF, &cElement, &bSuccess);
        pGFF->GetList(&cLinks, &cElement, "RepliesList");
        m_pEntries[i].m_nReplies = pGFF->GetListCount(&cLinks);
        if (bLoadAll) {
            if (m_pEntries[i].m_nReplies)
                m_pEntries[i].m_pReplies = new CSWSDialogReplyLink[m_pEntries[i].m_nReplies];
            for (j = 0; bSuccess && j < m_pEntries[i].m_nReplies; j++) {
                pGFF->GetListElement(&cLink, &cLinks, j);
                LoadDialogLinkedNode(&m_pEntries[i].m_pReplies[j], pGFF, &cLink, &bSuccess,
                                     m_nReplies);
                m_pEntries[i].m_pReplies[j].m_bDisplayInactive =
                    pGFF->ReadFieldBYTE(&cLink, "DisplayInactive", bOptional, 0) != 0;
            }
        }
    }
    if (!bSuccess) {
        Cleanup();
        return bSuccess;
    }
    if (bLoadAll) {
        pGFF->GetList(&cList, &cTop, "ReplyList");
        m_pReplies = new CSWSDialogReply[m_nReplies];
        for (i = 0; bSuccess && i < m_nReplies; i++) {
            pGFF->GetListElement(&cElement, &cList, i);
            LoadDialogBase(&m_pReplies[i], pGFF, &cElement, &nDelayEntry, &bSuccess);
            LoadDialogCamera(&m_pReplies[i], pGFF, &cElement, &bSuccess);
            pGFF->GetList(&cLinks, &cElement, "EntriesList");
            m_pReplies[i].m_nEntries = pGFF->GetListCount(&cLinks);
            if (m_pReplies[i].m_nEntries)
                m_pReplies[i].m_pEntries = new CSWSDialogEntryLink[m_pReplies[i].m_nEntries];
            for (j = 0; bSuccess && j < m_pReplies[i].m_nEntries; j++) {
                pGFF->GetListElement(&cLink, &cLinks, j);
                LoadDialogLinkedNode(&m_pReplies[i].m_pEntries[j], pGFF, &cLink, &bSuccess,
                                     m_nEntries);
            }
        }
        if (!bSuccess) {
            Cleanup();
            return bSuccess;
        }
    } else {
        m_nReplies = 0;
    }

    pGFF->GetList(&cLinks, &cTop, "StartingList");
    m_nStartingEntries = pGFF->GetListCount(&cLinks);
    if (!m_nStartingEntries)
        m_pStartingEntries = 0;
    else
        m_pStartingEntries = new CSWSDialogEntryLink[m_nStartingEntries];
    for (i = 0; bSuccess && i < m_nStartingEntries; i++) {
        pGFF->GetListElement(&cLink, &cLinks, i);
        LoadDialogLinkedNode(&m_pStartingEntries[i], pGFF, &cLink, &bSuccess, m_nEntries);
    }
    if (!bSuccess)
        Cleanup();

    CResList cStunts;
    pGFF->GetList(&cStunts, &cTop, "StuntList");
    m_nStuntModels = pGFF->GetListCount(&cStunts);
    m_pStuntModels = m_nStuntModels ? new CSWSDialogStuntModel[m_nStuntModels] : 0;
    for (i = 0; i < m_nStuntModels && bSuccess; i++) {
        pGFF->GetListElement(&cStunt, &cStunts, i);
        m_pStuntModels[i].m_sParticipant =
            pGFF->ReadFieldCExoString(&cStunt, "Participant", bSuccess, "");
        m_pStuntModels[i].m_cStuntModel =
            pGFF->ReadFieldCResRef(&cStunt, "StuntModel", bSuccess, "");
    }
    return bSuccess;
}

// Descriptive name (not in the debug map): applies the reply a player chose.
// NONMATCH (81): stack slots of pPos / pClient / pFound are rotated, the
// found object is not cached in esi, and the second player-list head test is
// folded; declaration-order and getter-shape variants tried.
// FUNCTION: SWKOTOR 0x005A03C0
int CSWSDialog::HandleReply(unsigned long nPlayerId, CSWSObject *pObject, unsigned long nReply,
                            int bForce, unsigned long nEntry, int bSkipDelay)
{
    CExoLinkedListNode *pPos;
    CClientExoApp *pClient = g_pAppManager->GetClientExoApp();
    CGameObject *pFound;
    if (!bForce && (m_n74 != -1 || nEntry != (unsigned long)m_n70))
        return 1;
    pPos = m_lPlayers.GetHeadPos();
    CSWSDialogPlayer *pPlayer = m_lPlayers.GetHeadObject();
    while (pPlayer) {
        if (pPlayer->m_nPlayerId == nPlayerId)
            break;
        pPlayer = m_lPlayers.GetNext(pPos);
    }
    if (pPlayer) {
        if (bForce) {
            if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(
                    pPlayer->m_oidPlayer, &pFound) != g_cObjectArraySuccess)
                return 1;
            if (pFound->AsSWSCreature()) {
                pFound->AsSWSCreature()->SetActivity(4, 0, 0);
                return 1;
            }
            ((CSWSObject *)pFound)->StopDialog();
            return 1;
        }
        m_oidPlayer = pPlayer->m_oidPlayer;
        m_nPlayerType = pPlayer->m_nPlayerType;
    }
    if (m_lPlayers.GetHeadPos() && !pPlayer)
        return 1;
    if (nReply >= m_pEntries[m_n70].m_nReplies)
        return 0;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidPlayer, &pFound) !=
        g_cObjectArraySuccess)
        return 0;
    m_n74 = nReply;
    unsigned long nReplyIndex = m_pEntries[m_n70].m_pReplies[nReply].m_nIndex;
    if (m_lPlayers.GetHeadPos()) {
        pPos = m_lPlayers.GetHeadPos();
        CSWSDialogPlayer *pOther = m_lPlayers.GetFirst();
        while (pOther) {
            if (pOther->m_nPlayerId != nPlayerId) {
                CExoString sListener(
                    m_pReplies[m_pEntries[m_n70].m_pReplies[m_n74].m_nIndex].m_sListener);
                if (sListener != "") {
                    if (!sListener.CompareNoCase("OWNER")) {
                        if (sListener.CompareNoCase("PLAYER"))
                            g_pAppManager->m_pServerExoApp->GetPlayerCharacterID();
                        else
                            GetSpeaker(pObject, sListener);
                    }
                }
                CExoString sText =
                    GetExoStringFromLocString(m_pReplies[nReplyIndex].m_sText, m_nPlayerType);
                pClient->GetInGameGui()->HandleDialogReplyChosen(
                    pOther->m_nPlayerId, nReply, nEntry, sText,
                    pClient->ServerToClientObjectId(m_oidPlayer), m_nPlayerType,
                    m_bConversationStarted);
            }
            AddJournalEntry(m_pReplies[nReplyIndex].m_sQuest, m_pReplies[nReplyIndex].m_nQuestEntry,
                            pOther->m_nPlayerId);
            GiveDialogPlotXP(m_pReplies[nReplyIndex].m_nPlotIndex,
                             m_pReplies[nReplyIndex].m_fPlotXPPercentage);
            pOther = m_lPlayers.GetNext(pPos);
        }
    }
    if (m_lPlayers.GetHeadPos())
        m_oid98 = m_oidPlayer;
    if (!bSkipDelay) {
        CSWSDialogReply *pReply = &m_pReplies[nReplyIndex];
        if (pReply->m_nWaitFlags & 2) {
            SetDialogDelay(pObject, &pReply->m_sText, pReply->m_nDelay, pReply->m_nEntries != 0, 1);
        } else if (pReply->m_n28 != -1) {
            SetDialogDelay(pObject, &pReply->m_sText, 0, pReply->m_nEntries != 0, 1);
        } else if (g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bC0) {
            pObject->SetDialogDelay(0.0f);
        } else {
            SetDialogDelay(pObject, &m_pReplies[nReplyIndex].m_sText,
                           (unsigned long)m_pReplies[nReplyIndex].m_sText.GetStringReadTime(
                               g_pAppManager->m_pServerExoApp->GetPlayerLanguage(m_oidPlayer),
                               m_nPlayerType),
                           m_pReplies[nReplyIndex].m_nEntries != 0, 1);
        }
        if (!g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bC0)
            pObject->m_nDialogWaitFlags = m_pReplies[nReplyIndex].m_nWaitFlags;
        else
            pObject->m_nDialogWaitFlags = 0;
    }
    RunScript(pObject, m_pReplies[nReplyIndex].m_cScript);
    for (unsigned long k = 0; k < m_pReplies[nReplyIndex].m_nEntries; k++) {
        if ((!(m_pEntries[m_pReplies[nReplyIndex].m_pEntries[k].m_nIndex].m_sSpeaker != "") ||
             pObject->m_sTag.CompareNoCase(
                 m_pEntries[m_pReplies[nReplyIndex].m_pEntries[k].m_nIndex].m_sSpeaker) ||
             GetSpeaker(pObject,
                        m_pEntries[m_pReplies[nReplyIndex].m_pEntries[k].m_nIndex].m_sSpeaker)) &&
            CheckScript(pObject, m_pReplies[nReplyIndex].m_pEntries[k].m_cActive))
            return pObject->SendDialogEntry(m_pReplies[nReplyIndex].m_pEntries[k].m_nIndex);
    }
    m_bConversationStarted = 1;
    m_n60 = 1;
    return 0;
}
