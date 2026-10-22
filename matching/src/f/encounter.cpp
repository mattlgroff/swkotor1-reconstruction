// CSWArea ctor / static bounding boxes, CSWSEncounter lifetime and
// activate list, CSWSAreaOfEffectObject duration (0x0058F6B0..0x00595560).

class CExoString {
public:
    CExoString();
    CExoString(const char *s);
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
    CResRef operator=(const CResRef &r);
    char m_resRef[16];
};

class Vector {
public:
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    float x, y, z;
};

class Vector0 {
public:
    Vector0() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector0(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    float x, y, z;
};

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;
    CExoArrayList() { element = 0; num = 0; array_size = 0; }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
    void Allocate(int s);
    void Remove(T t);
    void DelIndex(int i)
    {
        num = num - 1;
        for (; i < num; i++)
            element[i] = element[i + 1];
    }
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num] = t;
        num++;
    }
};

class C2DA {
public:
    int GetFLOATEntry(int nRow, const CExoString &sColumn, float *pValue);
    char pad_0x0[0x24];
    int m_nNumRows;                  // +0x24
    int GetNumRows() { return m_nNumRows; }
};

class C2DArrays {
public:
    char pad_0x0[0xa8];
    C2DA *m_pEncDifficultyTable;     // +0xa8
    C2DA *GetEncDifficultyTable() { return m_pEncDifficultyTable; }
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    C2DArrays *m_p2DArrays;          // +0xb8
    C2DArrays *Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules *g_pRules;

class CGameObject {
public:
    virtual ~CGameObject();
    unsigned long m_idSelf;          // +0x4
    unsigned char m_nObjectType;     // +0x8
    char pad_0x9[0x3];
    unsigned long GetID() { return m_idSelf; }
};

class CServerAIMaster {
public:
    void RemoveObject(CGameObject *pObject);
};

class CSWSModule {
public:
    void RemoveObjectFromLookupTable(CExoString *pTag, unsigned long oid);
};

class CServerExoApp {
public:
    CSWSModule *GetModule();
    CServerAIMaster *GetServerAIMaster();
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;

// ---------------------------------------------------------------------------

class CSWArea {
public:
    CSWArea();
    virtual ~CSWArea();
    int RemoveStaticBoundingBox(unsigned long oid);

    unsigned long m_n4;              // +0x4
    unsigned long m_n8;              // +0x8
    unsigned long m_nC;              // +0xc
    char pad_0x10[0x8];
    void *m_p18;                     // +0x18
    void *m_p1C;                     // +0x1c
    void *m_p20;                     // +0x20
    void *m_p24;                     // +0x24
    void *m_p28;                     // +0x28
    char pad_0x2c[0x8];
    void *m_p34;                     // +0x34
    char pad_0x38[0x8];
    void *m_p40;                     // +0x40
    int m_nStaticBBCapacity;         // +0x44
    int m_nStaticBBCount;            // +0x48
    Vector *m_pvStaticBB;            // +0x4c
    unsigned long *m_poidStaticBB;   // +0x50
    Vector0 m_vMapOffset;            // +0x54
    char pad_0x60[0x8];
    unsigned long m_n68;             // +0x68
    unsigned long m_n6C;             // +0x6c
    unsigned long m_n70;             // +0x70
    unsigned long m_n74;             // +0x74
    unsigned long m_n78;             // +0x78
    char pad_0x7c[0x4];
    unsigned long m_n80;             // +0x80
    unsigned long m_n84;             // +0x84
    unsigned long m_n88;             // +0x88
    unsigned long m_n8C;             // +0x8c
    unsigned long m_n90;             // +0x90
    unsigned long m_n94;             // +0x94
    char pad_0x98[0x4];
    unsigned long m_n9C;             // +0x9c
    unsigned long m_nA0;             // +0xa0
    unsigned long m_nA4;             // +0xa4
    unsigned char m_nA8;             // +0xa8
    unsigned char m_nA9;             // +0xa9
    unsigned char m_nAA;             // +0xaa
    unsigned char m_nAB;             // +0xab
    unsigned long m_nAC;             // +0xac
    unsigned long m_nB0;             // +0xb0
    unsigned char m_nB4;             // +0xb4
    char pad_0xb5[0x3];
    unsigned long m_nB8;             // +0xb8
    CResRef m_cResRef;               // +0xbc
    char pad_0xcc[0x20];
    CResRef m_cResRef2;              // +0xec
};

// FUNCTION: SWKOTOR 0x0058F6B0
CSWArea::CSWArea()
{
    m_vMapOffset = Vector0(0.0f, 0.0f, 0.0f);
    m_n4 = 0;
    m_nC = 0;
    m_n8 = 0;
    m_n68 = 0;
    m_n6C = 0;
    m_n70 = 0;
    m_n74 = 0;
    m_n78 = 0;
    m_n80 = 0;
    m_n84 = 0;
    m_n88 = 0;
    m_n8C = 0;
    m_n90 = 0;
    m_n94 = 0;
    m_n9C = 0;
    m_nA4 = 0;
    m_nA8 = 0;
    m_nA9 = 0;
    m_nAA = 0;
    m_nAB = 0;
    m_nAC = 0;
    m_nB0 = 0;
    m_p1C = 0;
    m_p20 = 0;
    m_p24 = 0;
    m_p28 = 0;
    m_p18 = 0;
    m_p34 = 0;
    m_p40 = 0;
    m_pvStaticBB = 0;
    m_poidStaticBB = 0;
    m_nB8 = 0;
    m_nA0 = 1;
    m_nB4 = 0x32;
}

struct CSWAreaBlock58F800 {
    void Reset();
    char pad_0x0[0x10];
    unsigned long m_n10, m_n14, m_n18, m_n1C, m_n20, m_n24, m_n28, m_n2C;
    unsigned long m_n30, m_n34, m_n38, m_n3C, m_n40, m_n44, m_n48, m_n4C, m_n50;
};

// FUNCTION: SWKOTOR 0x0058F800
void CSWAreaBlock58F800::Reset()
{
    m_n3C = 0;
    m_n34 = 0;
    m_n30 = 0;
    m_n40 = 0;
    m_n10 = 0;
    m_n14 = 0;
    m_n1C = 0;
    m_n20 = 0;
    m_n24 = 0;
    m_n28 = 0;
    m_n18 = 0;
    m_n38 = 0;
    m_n2C = 0;
    m_n44 = 0;
    m_n48 = 0;
    m_n4C = 0;
    m_n50 = 0;
}

// NONMATCH (8): block layout only; the original exits the search loop straight
// to the shared `return 0` (merged return-0 placement), ours falls through to
// the `nIndex >= count` test first.
// FUNCTION: SWKOTOR 0x0058FEC0
int CSWArea::RemoveStaticBoundingBox(unsigned long oid)
{
    int nIndex = 0;
    while (nIndex < m_nStaticBBCount) {
        if (m_poidStaticBB[nIndex] == oid)
            break;
        nIndex++;
    }
    if (nIndex >= m_nStaticBBCount)
        return 0;
    if (nIndex < 0 || nIndex >= m_nStaticBBCount)
        return 0;
    m_nStaticBBCount--;
    for (; nIndex < m_nStaticBBCount; nIndex++) {
        m_poidStaticBB[nIndex] = m_poidStaticBB[nIndex + 1];
        m_pvStaticBB[nIndex * 2] = m_pvStaticBB[nIndex * 2 + 2];
        m_pvStaticBB[nIndex * 2 + 1] = m_pvStaticBB[nIndex * 2 + 3];
    }
    return 1;
}

// ---------------------------------------------------------------------------

class CSWSArea : public CSWArea {
public:
    void RemoveObjectFromArea(unsigned long oid);
    char pad_0x0fc[0xa4];
    CExoArrayList<unsigned long> m_aObjectIds;   // +0x1a0
};

class CSWSObject : public CGameObject {
public:
    virtual ~CSWSObject();
    CSWSArea *GetArea();
    void SetArea(CSWSArea *pArea);
    void SetPosition(const Vector &v, int a, int b, int c);

    char pad_0xc[0xc];
    CExoString m_sTag;               // +0x18
    char pad_0x20[0x208];
};

class CEncounterListEntry {
public:
    CEncounterListEntry &operator=(const CEncounterListEntry &o);
    ~CEncounterListEntry() {}
    CResRef m_cResRef;               // +0x0
    float m_fCR;                     // +0x10
    unsigned long m_n14;             // +0x14
    unsigned long m_n18;             // +0x18
    unsigned long m_n1C;             // +0x1c
    unsigned long m_n20;             // +0x20
};

// FUNCTION: SWKOTOR 0x00590250
CEncounterListEntry &CEncounterListEntry::operator=(const CEncounterListEntry &o)
{
    m_cResRef = o.m_cResRef;
    m_fCR = o.m_fCR;
    m_n14 = o.m_n14;
    m_n18 = o.m_n18;
    m_n1C = o.m_n1C;
    m_n20 = o.m_n20;
    return *this;
}

class CEncounterSpawnPoint {
public:
    char pad_0x0[0x10];
};

class CEncounterBuffer {
public:
    ~CEncounterBuffer()
    {
        delete[] m_pData;
        m_pData = 0;
    }
    unsigned char *m_pData;
};

class CSWSEncounter : public CSWSObject {
public:
    virtual ~CSWSEncounter();
    int GetInActivateAreaList(unsigned long oid);
    void RemoveFromActivateAreaList(unsigned long oid);
    void SetDifficulty(int nDifficulty);
    void ClearSpawnList();
    void RemoveFromArea();
    CExoLocString *GetLocalizedName();
    void SpawnIfAppropriate(unsigned long oid, float fDifficulty);
    void SetActive(int bActive);
    void UpdateActivateArea(unsigned long oid);
    void SpawnEncounterCreatures(float fDifficulty);
    void SetExhausted();

    char pad_0x228[0x8];
    CExoLocString m_sLocName;        // +0x230
    int m_bActive;                   // +0x238
    char pad_0x23c[0xc];
    int m_nDifficultyValue;          // +0x248
    char pad_0x24c[0xc];
    CExoArrayList<CEncounterSpawnPoint *> m_aSpawnList; // +0x258
    char pad_0x264[0x14];
    int m_bExhausted;                // +0x278
    char pad_0x27c[0x8];
    unsigned long *m_poidActivateList; // +0x284
    int m_nActivateListCount;        // +0x288
    int m_bTriggered;                // +0x28c
    Vector *m_pvVertices;            // +0x290
    char pad_0x294[0x4];
    int *m_pnPointIndices;           // +0x298
    char pad_0x29c[0x14];
    CEncounterListEntry *m_pCreatureList; // +0x2b0
    char pad_0x2b4[0x4];
    unsigned long *m_pSpawnPointIds; // +0x2b8
    char pad_0x2bc[0x1c];
    float m_fRespawnTime;            // +0x2d8
    char pad_0x2dc[0xc];
    CExoString m_sScripts[5];        // +0x2e8
    CEncounterBuffer *m_pBuffer;     // +0x310
};

// FUNCTION: SWKOTOR 0x00590290
int CSWSEncounter::GetInActivateAreaList(unsigned long oid)
{
    for (int i = 0; i < m_nActivateListCount; i++)
        if (m_poidActivateList[i] == oid)
            return 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x005903B0
void CSWSEncounter::RemoveFromActivateAreaList(unsigned long oid)
{
    int nRemoved = 0;
    for (int i = 0; i < m_nActivateListCount; i++) {
        unsigned long oidEntry = m_poidActivateList[i];
        if (oidEntry == oid)
            nRemoved++;
        else if (nRemoved)
            m_poidActivateList[i - nRemoved] = oidEntry;
    }
    m_nActivateListCount -= nRemoved;
}

// FUNCTION: SWKOTOR 0x00590A60
void CSWSEncounter::SetDifficulty(int nDifficulty)
{
    float fValue = 0.0f;
    C2DA *pTable = g_pRules->Get2DArrays()->GetEncDifficultyTable();
    if (pTable) {
        if (nDifficulty >= g_pRules->Get2DArrays()->GetEncDifficultyTable()->GetNumRows() ||
            nDifficulty < 0)
            return;
        pTable->GetFLOATEntry(nDifficulty, "VALUE", &fValue);
    }
    m_nDifficultyValue = (int)fValue;
}

// FUNCTION: SWKOTOR 0x005921A0
void CSWSEncounter::ClearSpawnList()
{
    if (m_aSpawnList.num > 0) {
        for (int i = m_aSpawnList.num - 1; i >= 0; i--) {
            CEncounterSpawnPoint *pPoint = m_aSpawnList.element[i];
            m_aSpawnList.DelIndex(i);
            if (pPoint)
                delete pPoint;
        }
    }
}

// FUNCTION: SWKOTOR 0x00592210
void CSWSEncounter::RemoveFromArea()
{
    if (GetArea()) {
        GetArea()->RemoveObjectFromArea(m_idSelf);
        CSWSArea *pArea = GetArea();
        pArea->m_aObjectIds.Remove(m_idSelf);
    }
    SetArea(0);
    SetPosition(Vector(0.0f, 0.0f, 0.0f), 1, 1, 0);
}

// FUNCTION: SWKOTOR 0x00593380
CSWSEncounter::~CSWSEncounter()
{
    RemoveFromArea();
    g_pAppManager->m_pServerExoApp->GetServerAIMaster()->RemoveObject(this);
    CSWSModule *pModule = g_pAppManager->m_pServerExoApp->GetModule();
    if (pModule) {
        unsigned long oid = m_idSelf;
        pModule->RemoveObjectFromLookupTable(&m_sTag, oid);
    }
    if (m_pCreatureList) {
        delete[] m_pCreatureList;
        m_pCreatureList = 0;
    }
    if (m_poidActivateList) {
        delete[] m_poidActivateList;
        m_poidActivateList = 0;
    }
    if (m_pvVertices) {
        delete[] m_pvVertices;
        m_pvVertices = 0;
    }
    if (m_pnPointIndices) {
        delete[] m_pnPointIndices;
        m_pnPointIndices = 0;
    }
    if (m_pSpawnPointIds) {
        delete[] m_pSpawnPointIds;
        m_pSpawnPointIds = 0;
    }
    ClearSpawnList();
    if (m_pBuffer)
        delete m_pBuffer;
}

// FUNCTION: SWKOTOR 0x00593510
CExoLocString *CSWSEncounter::GetLocalizedName()
{
    return &m_sLocName;
}

// FUNCTION: SWKOTOR 0x00593BE0
void CSWSEncounter::SpawnIfAppropriate(unsigned long oid, float fDifficulty)
{
    if (m_bActive && m_bExhausted != 1) {
        m_bTriggered = 0;
        UpdateActivateArea(oid);
        if (m_bTriggered == 1) {
            SpawnEncounterCreatures(fDifficulty);
            if (m_fRespawnTime > 0.0f) {
                SetExhausted();
                m_bExhausted = 1;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00593C50
void CSWSEncounter::SetActive(int bActive)
{
    m_bActive = bActive;
    if (bActive)
        SpawnIfAppropriate(0x7f000000, -1.0f);
}

// FUNCTION: SWKOTOR 0x00593F90 SYMBOL: ??_GCSWSEncounter@@UAEPAXI@Z

// ---------------------------------------------------------------------------

class CSWSAreaOfEffectObject : public CSWSObject {
public:
    void SetDuration(unsigned char nDurationType, float fDuration);
    void AddObjectInside(unsigned long oid);

    char pad_0x228[0x60];
    int m_nDuration;                 // +0x288
    unsigned char m_nDurationType;   // +0x28c
    char pad_0x28d[0x97];
    CExoArrayList<unsigned long> m_aObjectsInside; // +0x324
};

// FUNCTION: SWKOTOR 0x00594770
void CSWSAreaOfEffectObject::SetDuration(unsigned char nDurationType, float fDuration)
{
    m_nDurationType = nDurationType;
    if (fDuration > 0.0f)
        m_nDuration = (int)(fDuration * 1000.0f);
}

// FUNCTION: SWKOTOR 0x00595560
void CSWSAreaOfEffectObject::AddObjectInside(unsigned long oid)
{
    m_aObjectsInside.Add(oid);
}
