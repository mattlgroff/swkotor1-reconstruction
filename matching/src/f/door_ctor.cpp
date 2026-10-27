// CSWSDoor constructor and small accessors (round s8, 0x00589EE0-0x0058A1E0).

class CExoString {
public:
    CExoString();
    ~CExoString();
    CExoString &operator=(const char *s);
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

class Vector {
public:
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    float x, y, z;
};

class Quaternion {
public:
    Quaternion() { x = y = z = 0.0f; w = 1.0f; }
    float w, x, y, z;
};

extern const unsigned char g_nObjectTypeDoor;      // 0x00746639
extern int g_nDoorStateClosed;
extern int g_nDoorStateOpen1;
extern int g_nDoorStateOpen2;

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

    char pad_0xc[0xc8];
    int m_nAnimation;                // +0xd4
    char pad_0xd8[0x150];
};

class CSWDoorSurfaceMesh;

class CSWSDoor : public CSWSObject {
public:
    CSWSDoor(unsigned long oid);
    virtual ~CSWSDoor();
    CResRef GetConversation();
    CExoLocString *GetLocName();

    CExoString m_sScripts[15];       // +0x228
    unsigned char m_nAppearance;     // +0x2a0
    unsigned char m_nGenericType;    // +0x2a1
    CResRef m_cConversation;         // +0x2a2
    char pad_0x2b2[0x2];
    int m_n2B4;                      // +0x2b4
    int m_n2B8;                      // +0x2b8
    unsigned char m_n2BC;            // +0x2bc
    unsigned char m_n2BD;            // +0x2bd
    unsigned char m_n2BE;            // +0x2be
    unsigned char m_n2BF;            // +0x2bf
    unsigned char m_n2C0;            // +0x2c0
    unsigned char m_n2C1;            // +0x2c1
    unsigned char m_nVisualEffect;   // +0x2c2
    char pad_0x2c3[0x1];
    int m_n2C4;                      // +0x2c4
    int m_n2C8;                      // +0x2c8
    unsigned char m_nOpenState;      // +0x2cc
    unsigned char m_nNextOpenState;  // +0x2cd
    char pad_0x2ce[0x2];
    CExoString m_s2D0;               // +0x2d0
    int m_n2D8;                      // +0x2d8
    CExoArrayList<unsigned long> m_aList; // +0x2dc
    int m_n2E8;                      // +0x2e8
    unsigned char m_n2EC;            // +0x2ec
    unsigned char m_n2ED;            // +0x2ed
    char pad_0x2ee[0x2];
    int m_n2F0;                      // +0x2f0
    int m_n2F4;                      // +0x2f4
    unsigned char m_n2F8;            // +0x2f8
    char pad_0x2f9[0x3];
    int m_n2FC;                      // +0x2fc
    int m_n300;                      // +0x300
    int m_n304;                      // +0x304
    int m_bStatic;                   // +0x308
    int m_n30C;                      // +0x30c
    int m_n310;                      // +0x310
    unsigned long m_oid314;          // +0x314
    unsigned long m_oid318;          // +0x318
    unsigned long m_oidLastOpened;   // +0x31c
    unsigned long m_oid320;          // +0x320
    unsigned long m_oid324;          // +0x324
    unsigned long m_oid328;          // +0x328
    CSWDoorSurfaceMesh *m_apWalkMeshes[3]; // +0x32c
    Vector m_vBBMin;                 // +0x338
    Vector m_vBBMax;                 // +0x344
    Vector m_avAreaPoints[4];        // +0x350
    int m_nAreaPoints;               // +0x380
    unsigned char m_nLinkedToFlags;  // +0x384
    char pad_0x385[0x3];
    CExoString m_sLinkedTo;          // +0x388
    CExoString m_sLinkedToModule;    // +0x390
    unsigned long m_oidLinkedTo;     // +0x398
    CExoLocString m_sLocName;        // +0x39c
    CExoLocString m_sDescription;    // +0x3a4
    unsigned short m_n3AC;           // +0x3ac
    char pad_0x3ae[0x2];
    Quaternion m_qOrientation;       // +0x3b0
    int m_n3C0;                      // +0x3c0
    int m_n3C4;                      // +0x3c4
    CExoLocString m_sTransitionDestination; // +0x3c8
};

// FUNCTION: SWKOTOR 0x00589EE0
CSWSDoor::CSWSDoor(unsigned long oid)
    : CSWSObject(g_nObjectTypeDoor, oid, 0)
{
    g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddObject(this, 0);
    m_nAnimation = 0x2726;
    m_oidLastOpened = 0x7f000000;
    m_oid320 = 0x7f000000;
    m_oid314 = 0x7f000000;
    m_oid318 = 0x7f000000;
    m_oid328 = 0x7f000000;
    m_oid324 = 0x7f000000;
    for (int i = 0; i < 15; i++)
        m_sScripts[i] = "default";
    m_nAppearance = 0;
    m_nGenericType = 0;
    m_n2F0 = 0;
    m_n2EC = 0;
    m_n2ED = 0;
    m_n2E8 = 0;
    m_n2F4 = 0;
    m_n2F8 = 0xff;
    m_n300 = 1;
    m_n2FC = 1;
    m_n304 = 1;
    m_n2B4 = 0;
    m_n2B8 = 0;
    m_n2BC = 0;
    m_n2BD = 0;
    m_n2BE = 0;
    m_s2D0 = "";
    m_n2D8 = 0;
    m_n2BF = 0;
    m_n2C0 = 0;
    m_n2C1 = 0;
    m_nVisualEffect = 0;
    m_n2C4 = 0;
    m_n2C8 = 0;
    m_nOpenState = 0;
    m_nNextOpenState = 0;
    m_bStatic = 1;
    m_n30C = 0;
    m_n310 = 0;
    m_nLinkedToFlags = 0;
    m_oidLinkedTo = 0x7f000000;
    m_apWalkMeshes[g_nDoorStateClosed] = 0;
    m_apWalkMeshes[g_nDoorStateOpen1] = 0;
    m_apWalkMeshes[g_nDoorStateOpen2] = 0;
    m_nAreaPoints = 0;
    m_n3AC = 0;
    m_n3C0 = 0;
    m_n3C4 = 0;
}

// FUNCTION: SWKOTOR 0x0058A1B0
CResRef CSWSDoor::GetConversation()
{
    return m_cConversation;
}

// FUNCTION: SWKOTOR 0x0058A1E0
CExoLocString *CSWSDoor::GetLocName()
{
    return &m_sLocName;
}
