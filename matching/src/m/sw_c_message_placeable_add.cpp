// CSWCMessage::HandleServerToPlayerPlaceableUpdate_Add (nwchandlemessage.o).

class CExoString {
public:
    char* m_sString;
    int m_nBufferLength;
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
};

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    CResRef(const CExoString& s);                   // 0x00406D60
    CResRef(const char* s);                         // 0x00406D80
    char m_resRef[16];
};

void* operator new(unsigned int nSize);
void operator delete(void* p);

class C2DA {
public:
    int GetCExoStringEntry(int nRow, const CExoString& sColumn, CExoString* pValue);  // 0x00413270
};

class CTwoDimArrays {
public:
    char pad_0x0[0x2c];
    C2DA* m_pPlaceables;                            // 0x2c
    C2DA* GetPlaceables() { return m_pPlaceables; }
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    CTwoDimArrays* m_p2DArrays;                     // 0xb8
    CTwoDimArrays* Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CSWCArea;
class CAnimationBase;

class CSWCObject {
public:
    virtual ~CSWCObject();
    virtual void SetObjectID(unsigned long nId);    // 0x04
    virtual void v08(); virtual void v0C(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1C(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2C(); virtual void v30(); virtual void v34();
    virtual void v38(); virtual void v3C(); virtual void v40(); virtual void v44();
    virtual void v48(); virtual void v4C(); virtual void v50(); virtual void v54();
    virtual void v58(); virtual void v5C(); virtual void v60(); virtual void v64();
    virtual void v68(); virtual void v6C(); virtual void v70(); virtual void v74();
    virtual int LoadModel(const CResRef& cModel, unsigned char nAlpha, int nFlags);  // 0x78
    virtual void v7C(); virtual void v80();
    virtual void SetArea(CSWCArea* pArea);          // 0x84
    virtual void v88(); virtual void v8C(); virtual void v90();
    virtual void AttachToScene(void* pScene);       // 0x94
    virtual void v98();
    virtual void SetAnimation(unsigned short nAnimation, int a, int b);   // 0x9c
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void SetEnvironmentMap(int a, int b, const CResRef& cMap);   // 0xb0

    CAnimationBase* GetAnimationBase();             // 0x0063C070
    unsigned short GetLoopingAnimation();           // 0x0063C100
    void AcquireAnimationBase(CSWCObject* pFrom);   // 0x0063D700 (descriptive)
    void SetLoopingAnimation(unsigned short nAnimation);    // 0x0063D830
    void SetDesiredFadeState(unsigned char nState, int a, unsigned long b, unsigned long c);  // 0x0063CE60
    void SetFadeState(unsigned char nState);        // 0x0063E220
    void SetObjectVisibilityOnSetArea(CSWCArea* pArea); // 0x0063E350

    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0xe4 - 0x8];
    int m_nE4;                                      // 0xe4
    char pad_0xe8[0xfc - 0xe8];
    CExoString m_sName;                             // 0xfc
};

class CSWCCreature : public CSWCObject {
};

class CSWCPlaceable : public CSWCObject {
public:
    CSWCPlaceable();                                // 0x00682270
    void SetAppearance(unsigned char nAppearance);  // 0x00682E20
    void LoadLight();                               // 0x006826E0

    char pad_0x104[0x110 - 0x104];
    int m_bUseable;                                 // 0x110 (descriptive)
    int m_n114;                                     // 0x114
    int m_n118;                                     // 0x118
    char pad_0x11c[4];
    int m_n120;                                     // 0x120
    int m_n124;                                     // 0x124
    int m_bHitCheck;                                // 0x128 (descriptive)
    int m_n12C;                                     // 0x12c
    int m_n130;                                     // 0x130
    char pad_0x134[0x150 - 0x134];
};

class CSWCArea {
public:
    int ParentCorpseExists(unsigned long nId);      // 0x006057B0
    char pad_0x0[0x184];
    void* m_pScene;                                 // 0x184
};

class CSWCModule {
public:
    char pad_0x0[0x48];
    CSWCArea* m_pArea;                              // 0x48
};

class CSWSPlaceable {
public:
    char pad_0x0[0x440];
    int m_bBodyBag;                                 // 0x440 (descriptive)
    int m_n444;                                     // 0x444
    unsigned long m_oidCorpse;                      // 0x448 (descriptive)
    int m_bCorpse;                                  // 0x44c (descriptive)
};

class CServerExoApp {
public:
    CSWSPlaceable* GetPlaceableByGameObjectID(unsigned long nId);   // 0x004AE7A0
    unsigned long ClientToServerObjectID(unsigned long nId);    // 0x004AEA30
};

class CGameObjectArray {
public:
    unsigned char AddExternalObject(unsigned long* pId, CSWCObject* pObject, int bClient);  // 0x004D7FF0
    unsigned char Delete(unsigned long nId, CSWCObject** ppObject);   // 0x004D81A0
};

class CClientAIList {
public:
    int RemoveObject(CSWCObject* pObject);          // 0x00604810
};

class CClientAIMaster {
public:
    int AddObject(CSWCObject* pObject);             // 0x00604A20
    char pad_0x0[4];
    CClientAIList m_lstObjects;                     // 0x04
};

struct CClientOptions {
    char pad_0x0[0x72];
    unsigned char m_bGore;                          // 0x72 (descriptive)
};

class CClientExoApp {
public:
    CSWCModule* GetModule();                        // 0x005ED530
    CGameObjectArray* GetGameObjectArray();         // 0x005ED570
    CSWCCreature* GetCreatureByGameObjectID(unsigned long nId);  // 0x005ED5B0
    CSWCPlaceable* GetPlaceableByGameObjectID(unsigned long nId);   // 0x005ED5F0
    CClientAIMaster* GetClientAIMaster();           // 0x005ED6B0
    CClientOptions* GetClientOptions();             // 0x005ED700
    void AddObjectToHitcheckIgnoreList(unsigned long nId);  // 0x005ED750
    void RemoveObjectFromHitcheckIgnoreList(unsigned long nId);  // 0x005ED760
    unsigned long ServerToClientObjectId(unsigned long nId);    // 0x005EDA50
};

class CAppManager {
public:
    void* m_p00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC
extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C

class CSWCMessage {
public:
    int MessageReadOverflow(int bError);            // 0x004D6230
    int ReadBOOL();                                 // 0x004D66F0
    unsigned char ReadBYTE(int nBits);              // 0x004D6720
    unsigned short ReadWORD(int nBits);             // 0x004D6750
    unsigned long ReadOBJECTIDClient();             // 0x00692630
    CExoString ReadCExoLocStringClient(CResRef& cRes, unsigned long* pStrRef);  // 0x00692660
    int HandleServerToPlayerPlaceableUpdate_Add();
};

// FUNCTION: SWKOTOR 0x0064D500
// NONMATCH (52 diff insns): structure, calls and stack slots agree; remaining
// differences are register choice (eax/ecx/edx rotations), the scheduling of
// the eight flag stores, and the hitcheck branch whose shared id push the
// original hoists above the test.
int CSWCMessage::HandleServerToPlayerPlaceableUpdate_Add()
{
    CGameObjectArray* pObjects = g_pAppManager->GetClientExoApp()->GetGameObjectArray();
    unsigned long nId = ReadOBJECTIDClient();
    CExoString sName = ReadCExoLocStringClient(CResRef(), 0);
    int b124 = ReadBOOL();
    unsigned char nAppearance = ReadBYTE(8);
    unsigned short nAnimation = ReadWORD(16);
    int bUseable = ReadBOOL();
    int bHitCheck = ReadBOOL();
    int b130 = ReadBOOL();
    int b114 = ReadBOOL();
    int b118 = ReadBOOL();
    int b12C = ReadBOOL();
    int b120 = ReadBOOL();
    if (MessageReadOverflow(1)) {
        return 0;
    }
    CSWCArea* pArea = g_pAppManager->GetClientExoApp()->GetModule()->m_pArea;
    CSWCPlaceable* pPlaceable = g_pAppManager->GetClientExoApp()->GetPlaceableByGameObjectID(nId);
    if (!pPlaceable) {
        CExoString sModel;
        if (!g_pAppManager->GetClientExoApp()->GetClientOptions()->m_bGore) {
            unsigned char bMissing;
            {
                CExoString sColumn("LowGore");
                C2DA* pTable = g_pRules->Get2DArrays()->GetPlaceables();
                bMissing = !pTable->GetCExoStringEntry(nAppearance, sColumn, &sModel);
            }
            if (bMissing) {
                CExoString sColumn("ModelName");
                C2DA* pTable = g_pRules->Get2DArrays()->GetPlaceables();
                pTable->GetCExoStringEntry(nAppearance, sColumn, &sModel);
            }
        }
        else {
            CExoString sColumn("ModelName");
            C2DA* pTable = g_pRules->Get2DArrays()->GetPlaceables();
            pTable->GetCExoStringEntry(nAppearance, sColumn, &sModel);
        }
        if (!g_pAppManager->m_pServerExoApp) {
            return 0;
        }
        pPlaceable = new CSWCPlaceable;
        if (!pPlaceable) {
            return 0;
        }
        CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
        CSWSPlaceable* pServerPlaceable = pServer->GetPlaceableByGameObjectID(pServer->ClientToServerObjectID(nId));
        int bLoadModel = 1;
        if (pServerPlaceable && pServerPlaceable->m_bCorpse) {
            CSWCCreature* pCorpse = g_pAppManager->m_pClientExoApp->GetCreatureByGameObjectID(
                g_pAppManager->m_pClientExoApp->ServerToClientObjectId(pServerPlaceable->m_oidCorpse));
            if (pCorpse && pCorpse->GetAnimationBase()) {
                bLoadModel = 0;
                pPlaceable->AcquireAnimationBase(pCorpse);
                pPlaceable->SetLoopingAnimation(pCorpse->GetLoopingAnimation());
                CClientAIMaster* pAIMaster = g_pAppManager->GetClientExoApp()->GetClientAIMaster();
                pAIMaster->m_lstObjects.RemoveObject(pCorpse);
                CSWCObject* pRemoved;
                if (g_pAppManager->m_pClientExoApp->GetGameObjectArray()->Delete(pCorpse->m_idSelf, &pRemoved) ==
                        GAME_OBJECT_VALID &&
                    pRemoved) {
                    delete pCorpse;
                }
            }
        }
        if (bLoadModel) {
            pPlaceable->LoadModel(sModel, 0xff, 10);
        }
        pPlaceable->SetAppearance(nAppearance);
        pPlaceable->SetObjectID(nId);
        pPlaceable->SetArea(pArea);
        if (pObjects->AddExternalObject(&nId, pPlaceable, 1) != GAME_OBJECT_VALID) {
            return 0;
        }
        CClientAIMaster* pAI = g_pAppManager->GetClientExoApp()->GetClientAIMaster();
        pAI->AddObject(pPlaceable);
        pPlaceable->LoadLight();
        if (pServerPlaceable && pServerPlaceable->m_bBodyBag && !pServerPlaceable->m_n444 && pArea &&
            pArea->ParentCorpseExists(pPlaceable->m_idSelf)) {
            pPlaceable->SetDesiredFadeState(0, 0, 0, 0);
            pPlaceable->SetFadeState(0);
        }
    }
    else {
        pPlaceable->m_nE4 = 0;
        pPlaceable->SetDesiredFadeState(1, 0, 0, 0);
        pPlaceable->SetObjectVisibilityOnSetArea(pArea);
    }
    pPlaceable->m_sName = sName;
    pPlaceable->AttachToScene(pArea->m_pScene);
    pPlaceable->m_bUseable = bUseable;
    pPlaceable->m_bHitCheck = bHitCheck;
    pPlaceable->m_n130 = b130;
    pPlaceable->m_n114 = b114;
    pPlaceable->m_n118 = b118;
    pPlaceable->m_n12C = b12C;
    pPlaceable->m_n120 = b120;
    pPlaceable->m_n124 = b124;
    pPlaceable->SetAnimation(nAnimation, 0, 0);
    if (pPlaceable->m_bHitCheck) {
        g_pAppManager->m_pClientExoApp->RemoveObjectFromHitcheckIgnoreList(pPlaceable->m_idSelf);
    }
    else {
        g_pAppManager->m_pClientExoApp->AddObjectToHitcheckIgnoreList(pPlaceable->m_idSelf);
    }
    pPlaceable->SetEnvironmentMap(1, 1, "DEFAULT");
    return 1;
}
