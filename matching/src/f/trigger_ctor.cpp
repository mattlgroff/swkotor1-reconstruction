// CSWSTrigger constructor and name accessor (round s8, 0x0058EAE0, 0x0058ED60).

class CExoString {
public:
    CExoString();
    CExoString(const char *s);
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
    float x, y, z;
};

class C2DA {
public:
    int GetFLOATEntry(int nRow, const CExoString &sColumn, float *pValue);
};

class C2DArrays {
public:
    char pad_0x0[0xc4];
    C2DA *m_pTrapTable;              // +0xc4
    C2DA *GetTrapTable() { return m_pTrapTable; }
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    C2DArrays *m_p2DArrays;          // +0xb8
    C2DArrays *Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules *g_pRules;
extern float g_fTriggerRadius;
extern const unsigned char g_nObjectTypeTrigger;   // 0x00746636

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
    void SetAnimation(int nAnimation);

    char pad_0xc[0xc8];
    int m_nAnimation;                // +0xd4
    char pad_0xd8[0x150];
};

class CSWSTrigger : public CSWSObject {
public:
    CSWSTrigger(unsigned long oid);
    virtual ~CSWSTrigger();
    CExoLocString *GetLocName();

    CExoLocString m_sLocName;        // +0x228
    CExoString m_sLinkedTo;          // +0x230
    CExoString m_sLinkedToModule;    // +0x238
    unsigned char m_nLinkedToFlags;  // +0x240
    char pad_0x241[0x3];
    CExoString m_sScripts[7];        // +0x244
    CExoString m_sKeyName;           // +0x27c
    int m_nVertices;                 // +0x284
    Vector *m_pvVertices;            // +0x288
    int m_n28C;                      // +0x28c
    int m_n290;                      // +0x290
    int m_nOutlineVertices;          // +0x294
    int *m_pnOutlineVertices;        // +0x298
    unsigned long m_oidLastEntered;  // +0x29c
    unsigned long m_oidLastLeft;     // +0x2a0
    unsigned long m_oidLastDisarmed; // +0x2a4
    CExoArrayList<unsigned long> m_aObjectsInside; // +0x2a8
    int m_bIsTransition;             // +0x2b4
    int m_nFactionId;                // +0x2b8
    int m_bCircular;                 // +0x2bc
    int m_bTrapDetectable;           // +0x2c0
    int m_bTrapDisarmable;           // +0x2c4
    int m_bTrapFlag;                 // +0x2c8
    int m_bAutoRemoveKey;            // +0x2cc
    int m_bTrapOneShot;              // +0x2d0
    unsigned char m_nTrapType;       // +0x2d4
    char pad_0x2d5[0x3];
    int m_n2D8;                      // +0x2d8
    int m_n2DC;                      // +0x2dc
    float m_fHighlightHeight;        // +0x2e0
    unsigned long m_oidCreator;      // +0x2e4
    float m_fMinX;                   // +0x2e8
    float m_fMaxX;                   // +0x2ec
    float m_fMinY;                   // +0x2f0
    float m_fMaxY;                   // +0x2f4
    int m_n2F8;                      // +0x2f8
    unsigned char m_nCursor;         // +0x2fc
    char pad_0x2fd[0x3];
    int m_n300;                      // +0x300
    int m_n304;                      // +0x304
    unsigned short m_nLoadScreenId;  // +0x308
    unsigned short m_n30A;           // +0x30a
    CExoLocString m_sTransitionDestination; // +0x30c
    int m_bSetByPlayerParty;         // +0x314
};

// FUNCTION: SWKOTOR 0x0058EAE0
CSWSTrigger::CSWSTrigger(unsigned long oid)
    : CSWSObject(g_nObjectTypeTrigger, oid, 0)
{
    g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddObject(this, 0);
    m_nAnimation = 10001;
    if (!g_pRules->Get2DArrays()->GetTrapTable()->GetFLOATEntry(17, "PrimaryRange", &g_fTriggerRadius))
        g_fTriggerRadius = 1.0f;
    m_sLinkedTo = "";
    m_nVertices = 0;
    m_pvVertices = 0;
    m_n28C = 0;
    m_n290 = 0;
    m_nOutlineVertices = 0;
    m_pnOutlineVertices = 0;
    m_n2F8 = 0;
    m_oidLastEntered = 0x7f000000;
    m_oidLastLeft = 0x7f000000;
    m_oidLastDisarmed = 0x7f000000;
    m_bIsTransition = 0;
    m_bCircular = 0;
    m_bTrapDetectable = 1;
    m_bTrapDisarmable = 1;
    m_bTrapFlag = 0;
    m_sKeyName = "";
    m_bTrapOneShot = 1;
    m_bAutoRemoveKey = 0;
    m_nTrapType = 0xff;
    m_nFactionId = 0;
    m_n2D8 = 0;
    m_n2DC = 0;
    for (int i = 0; i < 7; i++)
        m_sScripts[i] = "default";
    m_fMinX = 0.0f;
    m_fMaxX = 0.0f;
    m_fMinY = 0.0f;
    m_fMaxY = 0.0f;
    m_nCursor = 0;
    m_n300 = 0;
    m_n304 = 0;
    m_oidCreator = 0x7f000000;
    m_fHighlightHeight = 0.1f;
    m_nLoadScreenId = 0;
    m_n30A = 0;
    m_bSetByPlayerParty = 0;
    SetAnimation(10143);
}

// FUNCTION: SWKOTOR 0x0058ED60
CExoLocString *CSWSTrigger::GetLocName()
{
    return &m_sLocName;
}
