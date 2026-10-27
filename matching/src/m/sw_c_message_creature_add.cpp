// CSWCMessage::HandleServerToPlayerCreatureUpdate_Add (nwchandlemessage.o).

struct Vector {
    float x, y, z;
    Vector() { x = 0; y = 0; z = 0; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

void* operator new(unsigned int nSize);
void operator delete(void* p);

class CSWCArea {
public:
    int GetObjectsAtPositionVisible(const Vector& vPosition);   // 0x006054E0
    char pad_0x0[0x184];
    void* m_pScene;                                 // 0x184
};

class CSWCModule {
public:
    char pad_0x0[0x48];
    CSWCArea* m_pArea;                              // 0x48
};

class CSWCCreature {
public:
    CSWCCreature();                                 // 0x00616A20
    virtual ~CSWCCreature();
    virtual void SetObjectID(unsigned long nId);    // 0x04
    virtual void v08(); virtual void v0C(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1C(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2C(); virtual void v30(); virtual void v34();
    virtual void v38(); virtual void v3C(); virtual void v40(); virtual void v44();
    virtual void v48(); virtual void v4C(); virtual void v50(); virtual void v54();
    virtual void v58(); virtual void v5C(); virtual void v60(); virtual void v64();
    virtual void v68(); virtual void v6C(); virtual void v70(); virtual void v74();
    virtual void v78(); virtual void v7C(); virtual void v80();
    virtual void SetArea(CSWCArea* pArea);          // 0x84
    virtual void SetOrientation(const Vector& vOrientation);    // 0x88
    virtual void SetPosition(const Vector& vPosition);  // 0x8c
    virtual void v90();
    virtual void AttachToScene(void* pScene);       // 0x94

    void SetPartyMember(int bPartyMember);          // 0x0060EF50
    void SetDesiredDirection(const Vector& v);      // 0x0060B380
    void SetDesiredOrientation(const Vector& v);    // 0x0060B3C0
    void ForgetObject(unsigned long nId);           // 0x00613A70 (descriptive)
    void SetDesiredFadeState(unsigned char nState, int a, unsigned long b, unsigned long c);  // 0x0063CE60
    void SetFadeState(unsigned char nState);        // 0x0063E220

    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0x20 - 0x8];
    CSWCArea* m_pArea;                              // 0x20
    Vector m_vPosition;                             // 0x24
    CSWCArea* GetArea() { return m_pArea; }
    char pad_0x30[0x9c - 0x30];
    int m_bFadeIn;                                  // 0x9c (descriptive)
    char pad_0xa0[0xe4 - 0xa0];
    int m_nE4;                                      // 0xe4
    char pad_0xe8[0x1dc - 0xe8];
    Vector m_vLastPosition;                         // 0x1dc (descriptive)
    char pad_0x1e8[0x3d4 - 0x1e8];
    unsigned short m_nAppearance;                   // 0x3d4 (descriptive)
    char pad_0x3d6[0x44c - 0x3d6];
};

class CGameObjectArray {
public:
    unsigned char AddExternalObject(unsigned long* pId, CSWCCreature* pObject, int bClient);   // 0x004D7FF0
};

class CClientAIMaster {
public:
    int AddObject(CSWCCreature* pObject);           // 0x00604A20
};

class CClientExoApp {
public:
    CSWCModule* GetModule();                        // 0x005ED530
    CSWCCreature* GetPlayerCreature();              // 0x005ED540
    CGameObjectArray* GetGameObjectArray();         // 0x005ED570
    CSWCCreature* GetCreatureByGameObjectID(unsigned long nId);  // 0x005ED5B0
    CClientAIMaster* GetClientAIMaster();           // 0x005ED6B0
};

class CAppManager {
public:
    void* m_p00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC
extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C

class CSWCMessage {
public:
    int MessageReadOverflow(int bError);            // 0x004D6230
    unsigned char ReadBYTE(int nBits);              // 0x004D6720
    unsigned short ReadWORD(int nBits);             // 0x004D6750
    float ReadFLOAT(float fScale, int nBits);       // 0x004D67B0
    unsigned long ReadOBJECTIDClient();             // 0x00692630
    int HandleServerToPlayerCreatureUpdate_Add();
};

// FUNCTION: SWKOTOR 0x0064C0A0
int CSWCMessage::HandleServerToPlayerCreatureUpdate_Add()
{
    Vector vOrientation;
    CSWCCreature* pPlayer = g_pAppManager->GetClientExoApp()->GetPlayerCreature();
    int bExisting = 0;
    unsigned long nId = ReadOBJECTIDClient();
    unsigned char nPartyType = ReadBYTE(8);
    float fX = ReadFLOAT(1.0f, 0x20);
    float fY = ReadFLOAT(1.0f, 0x20);
    float fZ = ReadFLOAT(1.0f, 0x20);
    vOrientation.x = ReadFLOAT(1.0f, 0x20);
    vOrientation.y = ReadFLOAT(1.0f, 0x20);
    vOrientation.z = ReadFLOAT(1.0f, 0x20);
    unsigned short nAppearance = ReadWORD(16);
    if (MessageReadOverflow(1)) {
        return 0;
    }
    CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(nId);
    if (pCreature) {
        bExisting = 1;
        pCreature->m_nE4 = 0;
        if (pPlayer) {
            pPlayer->ForgetObject(nId);
        }
        if (pCreature->GetArea()->GetObjectsAtPositionVisible(pCreature->m_vPosition) == 1) {
            pCreature->SetDesiredFadeState(1, 0, 0, 0);
        }
        else {
            pCreature->SetDesiredFadeState(0, 0, 0, 0);
        }
        pCreature->m_bFadeIn = 1;
    }
    CSWCArea* pArea = g_pAppManager->GetClientExoApp()->GetModule()->m_pArea;
    if (!bExisting) {
        pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(nId);
        if (!pCreature) {
            pCreature = new CSWCCreature;
            if (!pCreature) {
                return 0;
            }
            pCreature->SetObjectID(nId);
            pCreature->SetArea(pArea);
            pCreature->SetPartyMember(nPartyType == 1);
            Vector vPosition(fX, fY, fZ);
            pCreature->SetPosition(vPosition);
            pCreature->SetOrientation(vOrientation);
            pCreature->SetDesiredDirection(vOrientation);
            pCreature->SetDesiredOrientation(vOrientation);
            pCreature->m_vLastPosition = vPosition;
            if (g_pAppManager->GetClientExoApp()->GetGameObjectArray()->AddExternalObject(&nId, pCreature, 1) !=
                GAME_OBJECT_VALID) {
                return 0;
            }
            CClientAIMaster* pAI = g_pAppManager->GetClientExoApp()->GetClientAIMaster();
            pAI->AddObject(pCreature);
        }
        if (pPlayer) {
            pPlayer->ForgetObject(nId);
        }
        pCreature->SetFadeState(1);
        pCreature->SetDesiredFadeState(1, 0, 0, 0);
    }
    pCreature->AttachToScene(pArea->m_pScene);
    pCreature->m_nAppearance = nAppearance;
    pCreature->m_nE4 = 0;
    return 1;
}
