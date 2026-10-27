// CSWSPlaceable constructor (round s8, 0x005877E0).

class CExoString {
public:
    CExoString();
    ~CExoString();
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CExoLocString {
public:
    CExoLocString();
    ~CExoLocString();
    void *m_pList;
    void *m_pPad;
};

class CResRef {
public:
    CResRef();
    CResRef operator=(const char *s);
    char m_resRef[16];
};

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;
    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
};

class Quaternion {
public:
    Quaternion() { x = y = z = 0.0f; w = 1.0f; }
    float w, x, y, z;
};

extern const unsigned char g_nObjectTypePlaceable; // 0x00746638
extern const unsigned long g_oidInvalid;           // 0x00746644

class CSWSObject;

class CServerAIMaster {
public:
    int AddObject(CSWSObject *pObject, int nList);
};

class CServerExoApp {
public:
    CServerAIMaster *GetServerAIMaster();
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;

class CItemRepository {
public:
    CItemRepository(unsigned long oidParent, int bFlag);
    ~CItemRepository();
    char pad_0x0[0x18];
};

class CGameObject {
public:
    virtual ~CGameObject();
    unsigned long m_idSelf;          // +0x4
    unsigned char m_nObjectType;     // +0x8
    char pad_0x9[0x3];
};

class CSWSObject : public CGameObject {
public:
    CSWSObject(unsigned char nType, unsigned long oid, int bFlag);
    virtual ~CSWSObject();

    char pad_0xc[0x21c];
};

class CSWPlaceableSurfaceMesh {
public:
    CSWPlaceableSurfaceMesh();
    ~CSWPlaceableSurfaceMesh();
    char pad_0x0[0x88];
};

class CSWSPlaceable : public CSWSObject {
public:
    CSWSPlaceable(unsigned long oid);
    virtual ~CSWSPlaceable();

    CExoLocString m_sLocName;        // +0x228
    unsigned char m_nAppearance;     // +0x230
    char pad_0x231[0x3];
    CExoLocString m_sDescription;    // +0x234
    char pad_0x23c[0x4];
    CResRef m_cConversation;         // +0x240
    char pad_0x250[0x4];
    int m_n254;                      // +0x254
    char pad_0x258[0x4];
    int m_n25C;                      // +0x25c
    char pad_0x260[0x4];
    CExoString m_sPortrait;          // +0x264
    char pad_0x26c[0x8];
    unsigned char m_n274;            // +0x274
    char pad_0x275[0xb];
    int m_n280;                      // +0x280
    int m_n284;                      // +0x284
    int m_n288;                      // +0x288
    int m_n28C;                      // +0x28c
    unsigned char m_n290;            // +0x290
    char pad_0x291[0x3];
    CExoString m_sScripts[16];       // +0x294
    char pad_0x314[0x4];
    CExoArrayList<unsigned long> m_aList; // +0x318
    char pad_0x324[0x8];
    int m_n32C;                      // +0x32c
    char pad_0x330[0x4];
    int m_n334;                      // +0x334
    int m_n338;                      // +0x338
    int m_n33C;                      // +0x33c
    char pad_0x340[0x4];
    int m_nBoundingBoxIndex;         // +0x344
    unsigned long m_nLastHeartbeatDay;  // +0x348
    unsigned long m_nLastHeartbeatTime; // +0x34c
    unsigned long m_oid350;          // +0x350
    unsigned long m_oid354;          // +0x354
    unsigned long m_oid358;          // +0x358
    unsigned long m_oid35C;          // +0x35c
    unsigned long m_oid360;          // +0x360
    unsigned long m_oid364;          // +0x364
    unsigned long m_oid368;          // +0x368
    CItemRepository *m_pItemRepository; // +0x36c
    unsigned short m_n370;           // +0x370
    unsigned short m_n372;           // +0x372
    unsigned long m_oid374;          // +0x374
    CResRef m_cResRef378;            // +0x378
    unsigned long m_oidLastUsedBy;   // +0x388
    int m_bLightIsOn;                // +0x38c
    int m_bLightStateChanged;        // +0x390
    unsigned char m_nBodyBag;        // +0x394
    char pad_0x395[0x7];
    int m_bInvalidId;                // +0x39c
    CSWPlaceableSurfaceMesh m_cWalkMesh; // +0x3a0
    Quaternion m_qOrientation;       // +0x428
    int m_n438;                      // +0x438
    int m_bInConversation;           // +0x43c
    int m_bHasClientCreature;        // +0x440
    int m_n444;                      // +0x444
    unsigned long m_oidClientCreature;  // +0x448
    int m_n44C;                      // +0x44c
    int m_n450;                      // +0x450
};

// FUNCTION: SWKOTOR 0x005877E0
CSWSPlaceable::CSWSPlaceable(unsigned long oid)
    : CSWSObject(g_nObjectTypePlaceable, oid, 0)
{
    g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddObject(this, 0);
    m_n32C = 0;
    m_n338 = 0;
    m_nLastHeartbeatDay = 0;
    m_nLastHeartbeatTime = 0;
    m_oid360 = 0x7f000000;
    m_oid364 = 0x7f000000;
    m_oid368 = 0x7f000000;
    m_oid35C = 0x7f000000;
    m_oid350 = 0x7f000000;
    m_oid354 = 0x7f000000;
    m_oid358 = 0x7f000000;
    m_pItemRepository = new CItemRepository(m_idSelf, 0);
    m_n284 = 1;
    m_n280 = 1;
    m_n288 = 1;
    m_n28C = 0;
    m_n274 = 0;
    m_n290 = 0xff;
    m_n254 = 0;
    m_n334 = 0;
    m_n370 = 0;
    m_n372 = 0;
    m_oid374 = g_oidInvalid;
    m_nBoundingBoxIndex = -1;
    m_cResRef378 = "";
    m_oidLastUsedBy = 0xffffffff;
    m_n25C = 0;
    m_bLightIsOn = 1;
    m_bLightStateChanged = 0;
    m_bInvalidId = 0;
    if (oid == 0x7f000000)
        m_bInvalidId = 1;
    m_oidClientCreature = 0x7f000000;
    m_n444 = 1;
    m_nBodyBag = 0;
    m_n438 = 0;
    m_bInConversation = 0;
    m_n33C = 0;
    m_bHasClientCreature = 0;
    m_n44C = 0;
    m_n450 = 0;
}
