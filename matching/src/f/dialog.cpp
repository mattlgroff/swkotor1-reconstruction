// CSWSDialog helpers (b0059_f2_scriptvar_dialog,
// b005a_f2_dialog_stats).

extern "C" double pow(double x, double y);
extern "C" double ceil(double x);

class CExoString {
public:
    CExoString();
    CExoString(const CExoString &s);
    ~CExoString();
    int operator==(const char *s) const;
    int operator!=(const char *s) const;
    int CompareNoCase(const CExoString &s) const;
    int GetLength() const;
    char operator[](int n) const;
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef();
    CResRef(const char *s);
    CResRef(const CResRef &r);
    CResRef operator=(const CResRef &r);
    int operator==(const char *s) const;
    void CopyToString(CExoString &s) const;
    char m_resRef[16];
};

class CExoLocString {
public:
    int GetString(unsigned long nLanguage, CExoString *pString, unsigned char nGender) const;
    CExoLocString &operator=(const CExoLocString &s);
    void *m_pList;
    unsigned long m_nStrRef;
};

class CRes {
public:
    void GetResRef(CResRef *pResRef, unsigned short *pType);
};

class CResGFFStruct;

class CResGFF {
public:
    CResRef ReadFieldCResRef(CResGFFStruct *pStruct, char *szLabel, int *bSuccess,
                             const CResRef &cDefault);
    unsigned long ReadFieldDWORD(CResGFFStruct *pStruct, char *szLabel, int *bSuccess,
                                 unsigned long nDefault);
};

class CSWSCreature;
class CSWSPlaceable;
class CSWSArea;

class CGameObject {
public:
    virtual ~CGameObject();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual CGameObject *AsSWSObject(); // +0x10
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual CSWSCreature *AsSWSCreature();   // +0x30
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual void v40();
    virtual void v44();
    virtual CSWSPlaceable *AsSWSPlaceable(); // +0x48

    CSWSArea *GetArea();
    unsigned long GetNearestObjectByTag(const CExoString &sTag, int n);
    unsigned long m_idSelf;          // +0x4
    char pad_0x8[0x10];
    CExoString m_sTag;               // +0x18
    unsigned long GetID() { return m_idSelf; }
};

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;
};

class CSWSArea {
public:
    char pad_0x0[0x190];
    CExoArrayList<unsigned long> m_aGameObjects;  // +0x190
};

class CSWSPlaceable {
public:
    void ClearAllHostileActionsVs(CGameObject *pObject);
};

class CSWSCombatRound {
public:
    void ClearAllHostileActions();
};

class CSWSObject : public CGameObject {
public:
    void SetDialogOwner(unsigned long oidOwner);
    char pad_0x20[0x34];
    unsigned long m_oidDialogOwner;  // +0x54
    char pad_0x58[0x20];
    int m_nAILevel;                  // +0x78
};

class CSWSCreature : public CGameObject {
public:
    void SetDialogState(int nState, int a, int b);
    int GetTimeSinceLastHostile(unsigned long nTime, int b);
    void SetCombatMode(int b);
    CSWSCombatRound *GetCombatRound();
    void ClearAllHostileActionsVs(CGameObject *pObject);

    char pad_0x20[0xa68];
    int m_bPlayerControlled;         // +0xa88
};

class CGameObjectArray {
public:
    char GetGameObject(unsigned long id, CGameObject **ppObject);
};

extern char g_cObjectArraySuccess;

class CSWParty {
public:
    char pad_0x0[0x190];
    CExoArrayList<unsigned long> m_aidMembers;  // +0x190
};

class CSWPartyTable {
public:
    void GivePlotXP(int nPlotIndex, int nXP);
    CSWParty *GetParty();
};

class CSWSModuleLang {
public:
    char pad_0x0[0x38];
    unsigned long m_nWorldTime;      // +0x38
};

class CVirtualMachine {
public:
    int RunScript(CExoString *psFileName, unsigned long oid, int bOid);
    int GetRunScriptReturnValue(int *pnType, void **ppValue);
};

extern CVirtualMachine *g_pVirtualMachine;

class CServerExoApp {
public:
    int GetLanguage();
    unsigned long GetLanguageID(int nLanguage);
    CSWSModuleLang *GetModuleByIndex(int n);
    CGameObjectArray *GetObjectArray();
    CSWPartyTable *GetPartyTable();
    CGameObject *GetGameObject(unsigned long oid);
};

class CNetLayer {
public:
    int SendDialogModels(unsigned long nPlayer, unsigned long nSpeakers, unsigned long *pIds,
                         CResRef *pModels, void *p1, void *p2);
    int GetPlayerModelIndex(const CExoString &sTag);
};

class CServerMessage {
public:
    unsigned long ServerToClientObjectId(unsigned long oid);
    CNetLayer *GetNetLayer();
    void SetDialogModelsDirty(int b);
};

class CAppManager {
public:
    char pad_0x0[0x4];
    CServerMessage *m_pServerMessage; // +0x4
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;

class CDialogLink {
public:
    CResRef m_cActive;               // +0x0
    unsigned long m_nIndex;          // +0x10
};

struct CExoLinkedListNode {
    CExoLinkedListNode *pPrev;
    CExoLinkedListNode *pNext;
    void *pObject;
};

class CExoLinkedListInternal {
public:
    ~CExoLinkedListInternal();
    void *RemoveHead();
    void *GetNext(CExoLinkedListNode *&pPos);
    void *Remove(CExoLinkedListNode *pPos);
    void *m_pHead;
    void *m_pTail;
    int m_nCount;
};

template <class T>
class CExoLinkedList {
public:
    ~CExoLinkedList() { delete m_pcExoLinkedListInternal; }
    void *GetHeadPos() { return m_pcExoLinkedListInternal->m_pHead; }
    int IsEmpty() { return m_pcExoLinkedListInternal->m_pHead == 0; }
    T *RemoveHead() { return (T *)m_pcExoLinkedListInternal->RemoveHead(); }
    CExoLinkedListNode *GetHead()
    {
        return (CExoLinkedListNode *)m_pcExoLinkedListInternal->m_pHead;
    }
    T *GetHeadObject()
    {
        void *pObject = m_pcExoLinkedListInternal->m_pHead;
        if (pObject)
            pObject = ((CExoLinkedListNode *)pObject)->pObject;
        return (T *)pObject;
    }
    T *GetNext(CExoLinkedListNode *&pPos) { return (T *)m_pcExoLinkedListInternal->GetNext(pPos); }
    void Remove(CExoLinkedListNode *pPos) { m_pcExoLinkedListInternal->Remove(pPos); }
    int Count() { return m_pcExoLinkedListInternal->m_nCount; }
    CExoLinkedListInternal *m_pcExoLinkedListInternal;
};

class CDialogEntry {
public:
    CExoString m_sSpeaker;           // +0x0
    CExoLocString m_sText;           // +0x8
    CResRef m_cScript;               // +0x10
    char pad_0x20[0x14];
    CResRef m_cSound;                // +0x34
    char pad_0x44[0x64];
    unsigned long m_nReplies;        // +0xa8
    CDialogLink *m_pReplies;         // +0xac
};

class CDialogReply {
public:
    char pad_0x0[0x10];
    CResRef m_cScript;               // +0x10
    char pad_0x20[0x88];
    unsigned long m_nEntries;        // +0xa8
    CDialogLink *m_pEntries;         // +0xac
    char pad_0xb0[0x4];
};

class CPlayerEntry {
public:
    unsigned long m_n;
    unsigned long m_oidPlayer;       // +0x4
    unsigned char m_nPlayerType;     // +0x8
};

class CDialogSpeaker {
public:
    CExoString m_sTag;               // +0x0
    CResRef m_cModel;                // +0x8
};

class CSWSDialog {
public:
    unsigned long GetPlayerCharacterId();
    unsigned long GetDialogModel(int nIndex, int *pnModel);
    int SendDialogModels(CGameObject *pObject, unsigned long nPlayer);
    int GetSpeaker(CGameObject *pObject, const CExoString &sSpeaker);
    void BarkOneLine(unsigned long nEntry);
    unsigned long GetStartEntry(CGameObject *pObject);
    virtual ~CSWSDialog();
    void ClearDialogEntries();
    void ClearDialogReplies();
    void Cleanup();
    void ClearAllPlayerHostileActions();
    void RunScript(CGameObject *pObject, const CResRef &cScript);
    void RunEndConversationScript(CGameObject *pObject, int bOwnerOnly);
    void LoadDialogLinkedNode(CDialogLink *pLink, CResGFF *pGFF, CResGFFStruct *pStruct,
                                     int *bSuccess, unsigned long nMax);
    int CheckScript(CGameObject *pObject, const CResRef &cScript);
    CExoString GetExoStringFromLocString(const CExoLocString &cLoc, unsigned char nGender);
    void ClearDialogOwnerInObject(unsigned long oidObject);
    int GetStartEntryOneLiner(CGameObject *pObject, CExoLocString &sText, CResRef &cSound,
                              CResRef &cScript);
    void InitializePsuedoRandomCameraAngles(CRes *pRes);
    void GiveDialogPlotXP(int nPlotIndex, float fXPPercentage);
    void SetOwnerAILevel(CSWSObject *pObject);
    int RemovePlayer(unsigned long oidPlayer);

    char pad_0x4[0x4];
    CDialogEntry *m_pEntries;                // +0x8
    unsigned long m_nReplies;                // +0xc
    CDialogReply *m_pReplies;                // +0x10
    unsigned long m_nStartingEntries;        // +0x14
    CDialogLink *m_pStartingEntries;         // +0x18
    CResRef m_cEndConversation;              // +0x1c
    CResRef m_cEndConversationAbort;         // +0x2c
    char pad_0x3c[0xc];
    unsigned long m_nSpeakers;               // +0x48
    CDialogSpeaker *m_pSpeakers;             // +0x4c
    char m_a50[0x14];                        // +0x50
    CExoLinkedList<CPlayerEntry> m_lPlayers; // +0x64
    char pad_0x68[0x4];
    int m_bConversationStarted;              // +0x6c
    char pad_0x70[0x8];
    unsigned char m_nPlayerType;             // +0x78
    char pad_0x79[0x3];
    unsigned long m_oidPlayer;               // +0x7c
    unsigned long m_oidOwner;                // +0x80
    char pad_0x84[0x8];
    int m_bLockFirstPlayer;                  // +0x8c
    char pad_0x90[0xc];
    unsigned long m_nCameraAngle;            // +0x9c
    unsigned long m_nCameraId;               // +0xa0
    unsigned long m_bCameraSide;             // +0xa4
    char pad_0xa8[0x54];
    char m_aFC[0x10];                        // +0xfc
    int m_bEndScriptRun;                     // +0x10c
    int m_bOneLiner;                         // +0x110
    CExoLinkedList<void> m_lList114;         // +0x114
    CExoLinkedList<void> m_lList118;         // +0x118
    int m_nOldAILevel;                       // +0x11c
};

// FUNCTION: SWKOTOR 0x0059E9B0
void CSWSDialog::ClearDialogOwnerInObject(unsigned long oidObject)
{
    CGameObject *pGameObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(
            oidObject, &pGameObject) == g_cObjectArraySuccess) {
        CSWSObject *pObject = (CSWSObject *)pGameObject;
        if (pObject->m_oidDialogOwner == m_oidOwner) {
            pObject->SetDialogOwner(0x7f000000);
            if (pObject->AsSWSCreature())
                pObject->AsSWSCreature()->SetDialogState(4, 0, 0);
        }
    }
}

// FUNCTION: SWKOTOR 0x0059ED70
void CSWSDialog::RunScript(CGameObject *pObject, const CResRef &cScript)
{
    if (cScript == "")
        return;
    CExoString sScript;
    cScript.CopyToString(sScript);
    g_pVirtualMachine->RunScript(&sScript, pObject->GetID(), 1);
}

// FUNCTION: SWKOTOR 0x0059EE00
int CSWSDialog::GetStartEntryOneLiner(CGameObject *pObject, CExoLocString &sText, CResRef &cSound,
                                      CResRef &cScript)
{
    for (unsigned long i = 0; i < m_nStartingEntries; i++) {
        if (!(m_pEntries[m_pStartingEntries[i].m_nIndex].m_sSpeaker != "") &&
            CheckScript(pObject, m_pStartingEntries[i].m_cActive) &&
            m_pEntries[m_pStartingEntries[i].m_nIndex].m_nReplies == 0) {
            sText = m_pEntries[m_pStartingEntries[i].m_nIndex].m_sText;
            cSound = m_pEntries[m_pStartingEntries[i].m_nIndex].m_cSound;
            cScript = m_pEntries[m_pStartingEntries[i].m_nIndex].m_cScript;
            return 1;
        }
    }
    return 0;
}

// NONMATCH (14): esi / edi swapped for the counter and the sum (original
// shares the EH-state zero register with nSum); four declaration orders tried.
// FUNCTION: SWKOTOR 0x0059F220
void CSWSDialog::InitializePsuedoRandomCameraAngles(CRes *pRes)
{
    CResRef cResRef;
    CExoString sResRef;
    unsigned short nType;
    unsigned long nSum = 0;
    int i;
    pRes->GetResRef(&cResRef, &nType);
    cResRef.CopyToString(sResRef);
    for (i = 0; i < sResRef.GetLength(); i++)
        nSum += ((int)pow(10.0, i) + 1) * sResRef[i];
    m_nCameraAngle = nSum % 19;
    m_nCameraId = nSum % 5 + 1;
    m_bCameraSide = nSum & 1;
}

// FUNCTION: SWKOTOR 0x0059F3C0
void CSWSDialog::GiveDialogPlotXP(int nPlotIndex, float fXPPercentage)
{
    if (nPlotIndex != -1 && fXPPercentage > 0.0f &&
        g_pAppManager->m_pServerExoApp->GetPartyTable())
        g_pAppManager->GetServerExoApp()->GetPartyTable()->GivePlotXP(
            nPlotIndex, (int)ceil(fXPPercentage * 100.0f));
}

// FUNCTION: SWKOTOR 0x0059F430
void CSWSDialog::SetOwnerAILevel(CSWSObject *pObject)
{
    int nAILevel = pObject->m_nAILevel;
    m_nOldAILevel = nAILevel;
    if (nAILevel < 3)
        pObject->m_nAILevel = 3;
    m_oidOwner = pObject->m_idSelf;
}

// FUNCTION: SWKOTOR 0x0059EC10
void CSWSDialog::LoadDialogLinkedNode(CDialogLink *pLink, CResGFF *pGFF, CResGFFStruct *pStruct,
                                      int *bSuccess, unsigned long nMax)
{
    CResRef cDefault("");
    pLink->m_cActive = pGFF->ReadFieldCResRef(pStruct, "Active", bSuccess, cDefault);
    pLink->m_nIndex = pGFF->ReadFieldDWORD(pStruct, "Index", bSuccess, 0);
    if (pLink->m_nIndex > nMax)
        *bSuccess = 0;
}

// FUNCTION: SWKOTOR 0x0059EC90
int CSWSDialog::CheckScript(CGameObject *pObject, const CResRef &cScript)
{
    if (cScript == "")
        return 1;
    CExoString sScript;
    cScript.CopyToString(sScript);
    int nResult = g_pVirtualMachine->RunScript(&sScript, pObject->GetID(), 1);
    if (nResult == 1) {
        int nType;
        void *pValue;
        if (g_pVirtualMachine->GetRunScriptReturnValue(&nType, &pValue) && nType == 3)
            nResult = pValue != 0;
        else
            nResult = 0;
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x0059F320
CExoString CSWSDialog::GetExoStringFromLocString(const CExoLocString &cLoc, unsigned char nGender)
{
    int nLanguage = g_pAppManager->m_pServerExoApp->GetLanguage();
    CExoString sText;
    cLoc.GetString(g_pAppManager->GetServerExoApp()->GetLanguageID(nLanguage), &sText, nGender);
    return sText;
}

// FUNCTION: SWKOTOR 0x005A4860 SYMBOL: ??_GCSWSDialog@@UAEPAXI@Z
// FUNCTION: SWKOTOR 0x005A4880
CSWSDialog::~CSWSDialog()
{
    ClearDialogEntries();
    ClearDialogReplies();
    Cleanup();
}

// FUNCTION: SWKOTOR 0x005A0940
void CSWSDialog::ClearAllPlayerHostileActions()
{
    int nLang = g_pAppManager->GetServerExoApp()->GetLanguage();
    unsigned long nTime = g_pAppManager->GetServerExoApp()->GetModuleByIndex(nLang)->m_nWorldTime;
    CGameObjectArray *pArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
    CExoArrayList<unsigned long> &aMembers =
        g_pAppManager->GetServerExoApp()->GetPartyTable()->GetParty()->m_aidMembers;
    for (int i = 0; i < aMembers.num; i++) {
        CGameObject *pObject;
        unsigned long id = aMembers.element[i];
        if (pArray->GetGameObject(id, &pObject) == g_cObjectArraySuccess &&
            pObject && pObject->AsSWSCreature() &&
            !pObject->AsSWSCreature()->m_bPlayerControlled) {
            CSWSCreature *pCreature = pObject->AsSWSCreature();
            if (pCreature->GetTimeSinceLastHostile(nTime, 0) <= 10) {
                pCreature->SetCombatMode(1);
                if (pCreature->GetCombatRound())
                    pCreature->GetCombatRound()->ClearAllHostileActions();
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x005A0A40
void CSWSDialog::RunEndConversationScript(CGameObject *pObject, int bOwnerOnly)
{
    if (m_bConversationStarted && !m_bEndScriptRun) {
        m_bEndScriptRun = 1;
        RunScript(pObject, m_cEndConversation);
        CGameObjectArray *pArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
        CGameObject *pFound;
        pArray->GetGameObject(m_oidOwner, &pFound);
        if (pFound) {
            CGameObject *pOwner = pFound->AsSWSObject();
            if (pOwner) {
                CSWSArea *pArea = pOwner->GetArea();
                if (!bOwnerOnly) {
                    for (int i = 0; i < pArea->m_aGameObjects.num;) {
                        unsigned long id = pArea->m_aGameObjects.element[i];
                        i++;
                        pArray->GetGameObject(id, &pFound);
                        if (pFound) {
                            if (pFound->AsSWSCreature())
                                pFound->AsSWSCreature()->ClearAllHostileActionsVs(pOwner);
                            else if (pFound->AsSWSPlaceable())
                                pFound->AsSWSPlaceable()->ClearAllHostileActionsVs(pOwner);
                        }
                    }
                } else {
                    if (pOwner->AsSWSCreature())
                        pOwner->AsSWSCreature()->ClearAllHostileActionsVs(pOwner);
                    else if (pOwner->AsSWSPlaceable())
                        pOwner->AsSWSPlaceable()->ClearAllHostileActionsVs(pOwner);
                }
            }
        }
    } else {
        RunScript(pObject, m_cEndConversationAbort);
    }
}

// FUNCTION: SWKOTOR 0x005A0240
unsigned long CSWSDialog::GetDialogModel(int nIndex, int *pnModel)
{
    *pnModel = 0;
    if (nIndex >= 0 && nIndex < (int)m_nSpeakers) {
        CGameObject *pObject = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidOwner);
        if (pObject && pObject->AsSWSObject()) {
            CGameObject *pOwner = pObject->AsSWSObject();
            CServerMessage *pMessage = g_pAppManager->m_pServerMessage;
            unsigned long oid;
            if (m_pSpeakers[nIndex].m_sTag == "OWNER") {
                oid = pMessage->ServerToClientObjectId(GetPlayerCharacterId());
            } else if (m_pSpeakers[nIndex].m_sTag == "PLAYER" || m_pSpeakers[nIndex].m_sTag == "") {
                oid = pMessage->ServerToClientObjectId(m_oidOwner);
            } else {
                oid = pMessage->ServerToClientObjectId(
                    pOwner->GetNearestObjectByTag(m_pSpeakers[nIndex].m_sTag, 0));
                *pnModel = pMessage->GetNetLayer()->GetPlayerModelIndex(m_pSpeakers[nIndex].m_sTag);
            }
            return oid;
        }
    }
    return 0x7f000000;
}

// FUNCTION: SWKOTOR 0x005A0080
int CSWSDialog::SendDialogModels(CGameObject *pObject, unsigned long nPlayer)
{
    CServerMessage *pMessage = g_pAppManager->m_pServerMessage;
    CResRef *pModels = new CResRef[m_nSpeakers];
    unsigned long *pIds = new unsigned long[m_nSpeakers];
    for (unsigned long i = 0; i < m_nSpeakers; i++) {
        pIds[i] = 0x7f000000;
        if (m_pSpeakers[i].m_sTag == "OWNER")
            pIds[i] = pMessage->ServerToClientObjectId(m_lPlayers.GetHeadPos() ? m_oidPlayer : 0x7f000000);
        else if (m_pSpeakers[i].m_sTag == "PLAYER" || m_pSpeakers[i].m_sTag == "")
            pIds[i] = pMessage->ServerToClientObjectId(m_oidOwner);
        else
            pIds[i] = pMessage->ServerToClientObjectId(
                pObject->GetNearestObjectByTag(m_pSpeakers[i].m_sTag, 0));
        pModels[i] = m_pSpeakers[i].m_cModel;
    }
    if (pMessage->GetNetLayer()->SendDialogModels(nPlayer, m_nSpeakers, pIds, pModels, m_a50, m_aFC))
        g_pAppManager->m_pServerMessage->SetDialogModelsDirty(1);
    delete[] pIds;
    delete[] pModels;
    return 1;
}

// FUNCTION: SWKOTOR 0x005A3640
unsigned long CSWSDialog::GetStartEntry(CGameObject *pObject)
{
    for (unsigned long i = 0; i < m_nStartingEntries; i++) {
        if ((!(m_pEntries[m_pStartingEntries[i].m_nIndex].m_sSpeaker != "") ||
             pObject->m_sTag.CompareNoCase(m_pEntries[m_pStartingEntries[i].m_nIndex].m_sSpeaker) ||
             GetSpeaker(pObject, m_pEntries[m_pStartingEntries[i].m_nIndex].m_sSpeaker)) &&
            CheckScript(pObject, m_pStartingEntries[i].m_cActive)) {
            unsigned long nEntry = m_pStartingEntries[i].m_nIndex;
            if (m_pEntries[nEntry].m_nReplies == 0 && !m_bOneLiner) {
                BarkOneLine(nEntry);
                RunScript(pObject, m_pEntries[m_pStartingEntries[i].m_nIndex].m_cScript);
                while (!m_lPlayers.IsEmpty())
                    delete m_lPlayers.RemoveHead();
                return 0xffffffff;
            }
            if (m_pEntries[nEntry].m_nReplies == 1 &&
                !m_pReplies[m_pEntries[nEntry].m_pReplies->m_nIndex].m_nEntries && !m_bOneLiner) {
                BarkOneLine(nEntry);
                RunScript(pObject, m_pEntries[m_pStartingEntries[i].m_nIndex].m_cScript);
                RunScript(pObject,
                          m_pReplies[m_pEntries[m_pStartingEntries[i].m_nIndex].m_pReplies->m_nIndex].m_cScript);
                while (!m_lPlayers.IsEmpty())
                    delete m_lPlayers.RemoveHead();
                return 0xffffffff;
            }
            return nEntry;
        }
    }
    return 0xffffffff;
}

// NONMATCH (17): the head-object getter result goes through eax into edi in
// the original, the final IsEmpty test uses ecx, and the new-head tail keeps
// its own return; three loop / getter shapes tried.
// FUNCTION: SWKOTOR 0x0059FB60
int CSWSDialog::RemovePlayer(unsigned long oidPlayer)
{
    CExoLinkedListNode *pPos = m_lPlayers.GetHead();
    int bFirst = 1;
    for (CPlayerEntry *pPlayer = m_lPlayers.GetHeadObject(); pPlayer;
         pPlayer = m_lPlayers.GetNext(pPos)) {
        if (pPlayer->m_oidPlayer == oidPlayer) {
            if (m_lPlayers.Count() != 1 && (!m_bLockFirstPlayer || !bFirst)) {
                m_lPlayers.Remove(pPos);
                if (pPlayer->m_oidPlayer == m_oidPlayer) {
                    delete pPlayer;
                    CPlayerEntry *pNew = m_lPlayers.GetHeadObject();
                    if (pNew) {
                        m_oidPlayer = pNew->m_oidPlayer;
                        m_nPlayerType = pNew->m_nPlayerType;
                    }
                    return 1;
                }
                delete pPlayer;
                return 1;
            }
            return 0;
        }
        bFirst = 0;
    }
    return !m_lPlayers.IsEmpty();
}
