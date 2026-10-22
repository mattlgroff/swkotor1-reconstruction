// area bounding boxes, CSWSEncounter activate area,
// CSWSAreaOfEffectObject and CSWCollisionMesh (b0059_f2_aoe_collision).

#include <string.h>

class Vector {
public:
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    float x, y, z;
};

class Quaternion {
public:
    Quaternion() { x = y = z = 0.0f; w = 1.0f; }
    float w, x, y, z;
};

Vector operator*(const Quaternion &q, const Vector &v);

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    void Allocate(int s);
    void Remove(T t);
    void AddOutOfLine(T t);
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num] = t;
        num++;
    }
};

class CExoString {
public:
    CExoString();
    ~CExoString();
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef();
    char m_resRef[16];
};

class CSWSCreature;
class CSWSArea;
class CGameObject;

class CGameObject {
public:
    virtual ~CGameObject();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual CSWSCreature *AsSWSCreature();     // +0x30
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4c();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5c();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6c();
    virtual void v70();
    virtual void v74();
    virtual void EventHandler(unsigned long nEvent, unsigned long oidCaller, void *pData,
                              unsigned long a, unsigned long b); // +0x78
    virtual void SetAnimation(int nAnimation); // +0x7c
    virtual void v80();
    virtual void v84();
    virtual void v88();
    virtual void v8c();
    virtual void v90();
    virtual void v94();
    virtual void v98();
    virtual void v9c();
    virtual void va0();
    virtual void va4();
    virtual void va8();
    virtual void vac();
    virtual void vb0();
    virtual void vb4();
    virtual int GetLastSpellId();              // +0xb8

    unsigned long m_idSelf;          // +0x4
    unsigned char m_nObjectType;     // +0x8
    char pad_0x9[0x3];
};

class CGameObjectArray {
public:
    char GetGameObject(unsigned long id, CGameObject **ppObject);
};

class CWorldTimer {
public:
    void GetWorldTime(unsigned long *pDay, unsigned long *pTime);
};

class CServerAIMaster {
public:
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long oidCaller,
                          unsigned long oidObject, unsigned long nEvent, void *pData);
    void AddObject(CGameObject *pObject, int bFlag);
    void RemoveObject(CGameObject *pObject);
};

class CSWSModule {
public:
    void RemoveObjectFromLookupTable(CExoString *pTag, unsigned long oid);
};

class CServerExoApp {
public:
    CSWSModule *GetModule();
    CSWSCreature *GetCreatureByGameObjectID(unsigned long id);
    CGameObjectArray *GetObjectArray();
    CServerAIMaster *GetServerAIMaster();
    CWorldTimer *GetWorldTimer();
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;
extern char g_cObjectArraySuccess;
extern unsigned long g_oidInvalid;
extern unsigned long EVENT_SIGNAL_EVENT;

class CScriptEvent {
public:
    CScriptEvent();
    void SetObjectID(int nIndex, unsigned long oid);
    unsigned short m_nType;
    char pad_0x2[0x32];
};

// ---------------------------------------------------------------------------

class CSWArea {
public:
    void AddStaticBoundingBox(unsigned long oid, Vector *pMin, Vector *pMax);

    char pad_0x0[0x44];
    int m_nStaticBBCapacity;         // +0x44
    int m_nStaticBBCount;            // +0x48
    Vector *m_pvStaticBB;            // +0x4c
    unsigned long *m_poidStaticBB;   // +0x50
};

// FUNCTION: SWKOTOR 0x0058FD50
void CSWArea::AddStaticBoundingBox(unsigned long oid, Vector *pMin, Vector *pMax)
{
    if (m_nStaticBBCount == m_nStaticBBCapacity) {
        int nNewSize = m_nStaticBBCapacity * 2;
        if (nNewSize <= 8)
            nNewSize = 8;
        Vector *pNewBB = new Vector[nNewSize * 2];
        unsigned long *pNewIds = new unsigned long[nNewSize];
        if (m_pvStaticBB) {
            memcpy(pNewBB, m_pvStaticBB, m_nStaticBBCount * 2 * sizeof(Vector));
            delete[] m_pvStaticBB;
        }
        if (m_poidStaticBB) {
            memcpy(pNewIds, m_poidStaticBB, m_nStaticBBCount * sizeof(unsigned long));
            delete[] m_poidStaticBB;
        }
        m_nStaticBBCapacity = nNewSize;
        m_pvStaticBB = pNewBB;
        m_poidStaticBB = pNewIds;
    }
    m_pvStaticBB[m_nStaticBBCount * 2] = *pMin;
    m_pvStaticBB[m_nStaticBBCount * 2 + 1] = *pMax;
    m_poidStaticBB[m_nStaticBBCount] = oid;
    m_nStaticBBCount++;
}

// ---------------------------------------------------------------------------

class CSWSArea {
public:
    void RemoveObjectFromArea(unsigned long oid);
    void AddObjectToArea(unsigned long oid, int bFlag);
    int LineSegmentsIntersect(float x1, float y1, float x2, float y2, float x3, float y3,
                              float x4, float y4, float *pX, float *pY);
    void AddObjectId(unsigned long oid) { m_aObjectIds.Add(oid); }

    char pad_0x0[0x190];
    CExoArrayList<unsigned long> m_aGameObjects; // +0x190
    char pad_0x19c[0x4];
    CExoArrayList<unsigned long> m_aObjectIds;   // +0x1a0
};

class CSWSObject : public CGameObject {
public:
    CSWSObject(char nType, unsigned long oid, int bFlag);
    virtual ~CSWSObject();
    CSWSArea *GetArea();
    void SetArea(CSWSArea *pArea);
    void SetPosition(const Vector &v, int a, int b, int c);

    char pad_0xc[0xc];
    CExoString m_sTag;               // +0x18
    char pad_0x20[0x70];
    Vector m_vPosition;              // +0x90
    Vector m_vOrientation;           // +0x9c
    unsigned long m_nCreatedDay;     // +0xa8
    unsigned long m_nCreatedTime;    // +0xac
    char pad_0xb0[0x124];
    unsigned char m_n1D4;            // +0x1d4
    unsigned char m_n1D5;            // +0x1d5
    char pad_0x1d6[0x5a];
};

class CSWSEncounter : public CSWSObject {
public:
    void RemoveFromArea();
    void AddToArea(CSWSArea *pArea, float x, float y, float z, int bFlag);
    int LineSegmentIntersectActivateArea(Vector *pStart, Vector *pEnd);
    void AddToActivateAreaList(unsigned long oid);

    char pad_0x230[0x50];
    int m_nActivateListCapacity;     // +0x280
    unsigned long *m_poidActivateList; // +0x284
    int m_nActivateListCount;        // +0x288
    char pad_0x28c[0x4];
    Vector *m_pvVertices;            // +0x290
    char pad_0x294[0x4];
    int *m_pnPointIndices;           // +0x298
    int m_nNumPoints;                // +0x29c
};

// FUNCTION: SWKOTOR 0x0058FFD0
int CSWSEncounter::LineSegmentIntersectActivateArea(Vector *pStart, Vector *pEnd)
{
    float fX, fY;
    CSWSArea *pArea = GetArea();
    if (pArea) {
        for (int i = 0; i < m_nNumPoints; i++) {
            int nFirst = m_pnPointIndices[i];
            int nSecond = m_pnPointIndices[i != m_nNumPoints - 1 ? i + 1 : 0];
            if (pArea->LineSegmentsIntersect(pStart->x, pStart->y, pEnd->x, pEnd->y,
                                             m_pvVertices[nFirst].x, m_pvVertices[nFirst].y,
                                             m_pvVertices[nSecond].x, m_pvVertices[nSecond].y,
                                             &fX, &fY) == 1)
                return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005902D0
void CSWSEncounter::AddToActivateAreaList(unsigned long oid)
{
    for (int i = 0; i < m_nActivateListCount; i++)
        if (m_poidActivateList[i] == oid)
            return;
    if (m_nActivateListCount < m_nActivateListCapacity) {
        m_poidActivateList[m_nActivateListCount] = oid;
        m_nActivateListCount++;
    } else {
        unsigned long *pOld = m_poidActivateList;
        m_nActivateListCapacity += 16;
        unsigned long *pNew = new unsigned long[m_nActivateListCapacity];
        for (int j = 0; j <= m_nActivateListCount; j++)
            pNew[j] = pOld[j];
        pNew[m_nActivateListCount] = oid;
        m_nActivateListCount++;
        m_poidActivateList = pNew;
        delete[] pOld;
    }
}

// FUNCTION: SWKOTOR 0x00593520
void CSWSEncounter::AddToArea(CSWSArea *pArea, float x, float y, float z, int bFlag)
{
    RemoveFromArea();
    if (pArea) {
        SetArea(pArea);
        SetPosition(Vector(x, y, z), 0, 1, 0);
        GetArea()->AddObjectToArea(m_idSelf, bFlag);
        CSWSArea *pCurArea = GetArea();
        pCurArea->AddObjectId(m_idSelf);
    }
}

// ---------------------------------------------------------------------------

class CSWSCreatureStats {
public:
    unsigned char GetSpellCasterLevel(unsigned char nClass, int bFlag);
};

class CSWSCreature : public CSWSObject {
public:
    int GetSpellDC(int nSpell);
    void AddAreaOfEffect(unsigned long oid);
    void RemoveTargetingAOE(unsigned long oid) { m_aTargetingAOEs.Remove(oid); }

    char pad_0x230[0xf4];
    CExoArrayList<unsigned long> m_aTargetingAOEs;  // +0x324
    CExoArrayList<unsigned long> m_aInsideAOEs;     // +0x330
    char pad_0x33c[0x738];
    CSWSCreatureStats *m_pStats;     // +0xa74
};

class CSWSpell {
public:
    char pad_0x0[0x3f];
    unsigned char m_nInnateLevel;    // +0x3f
};

class CSWSpellArray {
public:
    CSWSpell *GetSpell(int nSpell);
};

class CSWRules {
public:
    char pad_0x0[0x8c];
    CSWSpellArray *m_pSpellArray;    // +0x8c
};

extern CSWRules *g_pRules;

class CSWSAreaOfEffectObject : public CSWSObject {
public:
    CSWSAreaOfEffectObject(unsigned long oid);
    virtual ~CSWSAreaOfEffectObject();
    void SetCreator(unsigned long oidCreator);
    void SetTargetObjID(unsigned long oidTarget);
    void RemoveFromSubAreas(int bRunScripts);
    void RemoveFromArea();
    void JumpToPoint(CSWSArea *pArea, const Vector &vPosition);
    int InSubAreas(Vector vPosition);
    void UpdateSubAreas(int bFlag);

    unsigned char m_n230;            // +0x230
    char pad_0x231[0x3];
    int m_n234;                      // +0x234
    int m_n238;                      // +0x238
    char pad_0x23c[0x8];
    void *m_p244;                    // +0x244
    unsigned long m_oidCreator;      // +0x248
    unsigned long m_oidTarget;       // +0x24c
    unsigned long m_oid250;          // +0x250
    unsigned long m_oid254;          // +0x254
    int m_nSpellId;                  // +0x258
    int m_nCasterLevel;              // +0x25c
    CExoString m_sScripts[4];        // +0x260
    int m_n280;                      // +0x280
    int m_n284;                      // +0x284
    int m_n288;                      // +0x288
    unsigned char m_n28C;            // +0x28c
};

// FUNCTION: SWKOTOR 0x00594480
CSWSAreaOfEffectObject::CSWSAreaOfEffectObject(unsigned long oid)
    : CSWSObject(0xb, oid, 0)
{
    m_oidTarget = g_oidInvalid;
    m_oid250 = g_oidInvalid;
    m_oid254 = g_oidInvalid;
    m_oidCreator = g_oidInvalid;
    SetPosition(Vector(0.0f, 0.0f, 0.0f), 1, 1, 0);
    m_n238 = 0;
    m_n284 = 0;
    m_n280 = 0;
    m_n230 = 0;
    m_p244 = 0;
    m_n234 = -1;
    unsigned long nTime;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTime(&oid, &nTime);
    m_nCreatedDay = oid;
    m_nCreatedTime = nTime;
    m_n288 = 0;
    m_n28C = 2;
    g_pAppManager->m_pServerExoApp->GetServerAIMaster()->AddObject(this, 0);
    m_nCasterLevel = 0;
    m_nSpellId = 0xe;
}

// NONMATCH (4 insns): the class byte is spilled to the dead arg slot right
// before the push in the original; here the spill is scheduled before the
// 0xff/0xfe compares.
// FUNCTION: SWKOTOR 0x00594FF0
void CSWSAreaOfEffectObject::SetCreator(unsigned long oidCreator)
{
    m_oidCreator = oidCreator;
    CSWSCreature *pCreator = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreator);
    if (pCreator && pCreator->GetLastSpellId() != -1) {
        m_n1D5 = pCreator->m_n1D5;
        m_nSpellId = pCreator->GetSpellDC(pCreator->GetLastSpellId());
        unsigned char nClass = pCreator->m_n1D4;
        if (nClass != 0xff && nClass != 0xfe) {
            m_nCasterLevel = pCreator->m_pStats->GetSpellCasterLevel(nClass, 1);
        } else {
            CSWSpell *pSpell = g_pRules->m_pSpellArray->GetSpell(pCreator->GetLastSpellId());
            if (pSpell)
                m_nCasterLevel = pSpell->m_nInnateLevel * 2 - 1;
        }
    }
}

// FUNCTION: SWKOTOR 0x005955A0
void CSWSAreaOfEffectObject::SetTargetObjID(unsigned long oidTarget)
{
    CGameObject *pObject;
    if (m_oidTarget != g_oidInvalid) {
        if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidTarget, &pObject) ==
                g_cObjectArraySuccess &&
            pObject)
            pObject->AsSWSCreature()->RemoveTargetingAOE(m_idSelf);
    }
    m_oidTarget = oidTarget;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidTarget, &pObject) ==
            g_cObjectArraySuccess &&
        pObject) {
        unsigned long oid = m_idSelf;
        pObject->AsSWSCreature()->AddAreaOfEffect(oid);
    }
}

// FUNCTION: SWKOTOR 0x00595B60
void CSWSAreaOfEffectObject::RemoveFromSubAreas(int bRunScripts)
{
    CSWSArea *pArea = GetArea();
    CGameObjectArray *pArray = g_pAppManager->m_pServerExoApp->GetObjectArray();
    for (int i = 0; i < pArea->m_aGameObjects.num; i++) {
        CGameObject *pObject;
        if (pArray->GetGameObject(pArea->m_aGameObjects.element[i], &pObject) == g_cObjectArraySuccess &&
            pObject->m_nObjectType == 5) {
            CSWSCreature *pCreature = pObject->AsSWSCreature();
            if (InSubAreas(pCreature->m_vPosition)) {
                unsigned long oidSelf = m_idSelf;
                CExoArrayList<unsigned long> *pList = &pCreature->m_aInsideAOEs;
                int nCount = 0;
                for (int j = 0; j < pList->num; j++)
                    if (pList->element[j] == oidSelf)
                        nCount++;
                if (nCount) {
                    pList->Remove(oidSelf);
                    CScriptEvent *pEvent = new CScriptEvent;
                    pEvent->m_nType = 0xd;
                    pEvent->SetObjectID(0, pCreature->m_idSelf);
                    if (!bRunScripts) {
                        unsigned long oidCreature = pCreature->m_idSelf;
                        g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                            0, 0, oidCreature, m_idSelf, EVENT_SIGNAL_EVENT, pEvent);
                    } else
                        EventHandler(EVENT_SIGNAL_EVENT, pCreature->m_idSelf, pEvent, 0, 0);
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x005962D0
void CSWSAreaOfEffectObject::RemoveFromArea()
{
    if (GetArea()) {
        RemoveFromSubAreas(1);
        GetArea()->RemoveObjectFromArea(m_idSelf);
        CSWSArea *pCurArea = GetArea();
        pCurArea->m_aObjectIds.Remove(m_idSelf);
    }
    SetArea(0);
    SetPosition(Vector(0.0f, 0.0f, 0.0f), 1, 1, 0);
    SetAnimation(0);
}

// FUNCTION: SWKOTOR 0x00596360
void CSWSAreaOfEffectObject::JumpToPoint(CSWSArea *pArea, const Vector &vPosition)
{
    CSWSCreature *pTarget = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidTarget);
    if (pTarget && GetArea() == pArea) {
        CExoArrayList<unsigned long> *pList = &pTarget->m_aInsideAOEs;
        pList->Remove(m_idSelf);
        RemoveFromSubAreas(0);
        SetPosition(vPosition, 1, 1, 0);
        pList->AddOutOfLine(m_idSelf);
        UpdateSubAreas(0);
    }
}

// FUNCTION: SWKOTOR 0x005963D0
CSWSAreaOfEffectObject::~CSWSAreaOfEffectObject()
{
    if (m_oidTarget != g_oidInvalid) {
        CGameObject *pObject;
        if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidTarget, &pObject) ==
            g_cObjectArraySuccess)
            pObject->AsSWSCreature()->RemoveTargetingAOE(m_idSelf);
    }
    RemoveFromArea();
    if (m_p244)
        delete m_p244;
    g_pAppManager->GetServerExoApp()->GetServerAIMaster()->RemoveObject(this);
    CSWSModule *pModule = g_pAppManager->GetServerExoApp()->GetModule();
    if (pModule) {
        unsigned long oid = m_idSelf;
        pModule->RemoveObjectFromLookupTable(&m_sTag, oid);
    }
}

// FUNCTION: SWKOTOR 0x005965B0 SYMBOL: ??_GCSWSAreaOfEffectObject@@UAEPAXI@Z

// ---------------------------------------------------------------------------

class CRes {
public:
    virtual ~CRes();
    void Release();
};

class CExoResMan {
public:
    int ReleaseResObject(CRes *pRes);
};

extern CExoResMan *g_pExoResMan;

struct CSWCollisionFace {
    int m_anVertex[3];
};

struct CResWOKHeader {
    char pad_0x0[0x8];
    unsigned long m_nType;           // +0x08
    Vector m_vUse1;                  // +0x0c
    Vector m_vUse2;                  // +0x18
    char pad_0x24[0x18];
    Vector m_vPosition;              // +0x3c
    int m_nVertices;                 // +0x48
    unsigned long m_nVertexOffset;   // +0x4c
    int m_nFaces;                    // +0x50
    unsigned long m_nFaceOffset;     // +0x54
    unsigned long m_nMaterialOffset; // +0x58
    unsigned long m_nNormalOffset;   // +0x5c
    unsigned long m_nPlaneOffset;    // +0x60
    char pad_0x64[0xc];
    int m_nAABBs;                    // +0x70
};

class CResWOK : public CRes {
public:
    unsigned long GetType() { return m_pHeader ? m_pHeader->m_nType : 0; }
    Vector GetUse1() { return m_pHeader ? m_pHeader->m_vUse1 : Vector(0.0f, 0.0f, 0.0f); }
    char pad_0x4[0x2c];
    char *m_pData;                   // +0x30
    CResWOKHeader *m_pHeader;        // +0x34
};

class CSWCollisionMesh {
public:
    CSWCollisionMesh(unsigned short nA, unsigned short nB);
    virtual ~CSWCollisionMesh();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void PrepareRender();    // +0x1c
    void RenderMesh(float fR, float fG, float fB);
    int GetTriangle(int i, int &n0, int &n1, int &n2)
    {
        if (i >= m_nFaces)
            return 0;
        CSWCollisionFace *pFace = &m_pFaces[i];
        n0 = pFace->m_anVertex[0];
        n1 = pFace->m_anVertex[1];
        n2 = pFace->m_anVertex[2];
        return 1;
    }
    int GetTriangle2(int i, int *n0, int *n1, int *n2)
    {
        if (i < m_nFaces) {
            *n0 = m_pFaces[i].m_anVertex[0];
            *n1 = m_pFaces[i].m_anVertex[1];
            *n2 = m_pFaces[i].m_anVertex[2];
            return 1;
        }
        return 0;
    }
    void SetTriangleCount(int n);
    void SetVertexCount(int n);
    Vector LocalToWorldRotation(const Vector &v);
    Vector GetWorldPoint(const Vector &v);
    void GetVertex(int n, Vector *pOut);
    int LoadMeshBinary(CResWOK *pRes);

    int m_bTransformed;              // +0x04
    char pad_0x8[0x4];
    CResRef m_cResRef;               // +0x0c
    CRes *m_pRes;                    // +0x1c
    Vector m_vPosition;              // +0x20
    Vector m_vOffset;                // +0x2c
    Quaternion m_qOrientation;       // +0x38
    unsigned short m_n48;            // +0x48
    unsigned short m_n4A;            // +0x4a
    int m_bLoaded;                   // +0x4c
    int m_nVertices;                 // +0x50
    Vector *m_pvVertices;            // +0x54
    int m_nFaces;                    // +0x58
    int m_nAABBs;                    // +0x5c
    CSWCollisionFace *m_pFaces;      // +0x60
    int *m_pnMaterials;              // +0x64
    Vector *m_pvNormals;             // +0x68
    float *m_pfPlaneDistances;       // +0x6c
    Vector m_avUse[2];               // +0x70
};

// FUNCTION: SWKOTOR 0x005965D0
CSWCollisionMesh::~CSWCollisionMesh()
{
    if (m_bLoaded) {
        m_pvVertices = 0;
        m_pFaces = 0;
        m_pnMaterials = 0;
        m_pfPlaneDistances = 0;
        m_pvNormals = 0;
        m_pRes->Release();
        if (!g_pExoResMan->ReleaseResObject(m_pRes) && m_pRes)
            delete m_pRes;
        m_pRes = 0;
    } else {
        delete[] m_pvVertices;
        m_pvVertices = 0;
        delete[] m_pFaces;
        m_pFaces = 0;
        delete[] m_pnMaterials;
        m_pnMaterials = 0;
        delete[] m_pvNormals;
        m_pvNormals = 0;
        delete[] m_pfPlaneDistances;
        m_pfPlaneDistances = 0;
    }
}

// FUNCTION: SWKOTOR 0x00596900
void CSWCollisionMesh::SetTriangleCount(int n)
{
    m_nFaces = n;
    if (!m_bLoaded) {
        m_pFaces = new CSWCollisionFace[n];
        m_pnMaterials = new int[n];
        m_pfPlaneDistances = new float[n];
        m_pvNormals = new Vector[n];
    }
}

// FUNCTION: SWKOTOR 0x005969A0
void CSWCollisionMesh::SetVertexCount(int n)
{
    m_nVertices = n;
    m_pvVertices = new Vector[n];
}

// FUNCTION: SWKOTOR 0x00596B30
Vector CSWCollisionMesh::LocalToWorldRotation(const Vector &v)
{
    if (m_bTransformed)
        return v;
    return m_qOrientation * v;
}

// FUNCTION: SWKOTOR 0x00596B80
CSWCollisionMesh::CSWCollisionMesh(unsigned short nA, unsigned short nB)
{
    m_n48 = nB;
    m_n4A = nA;
    m_bLoaded = 0;
    m_bTransformed = 0;
    m_pRes = 0;
    m_nVertices = 0;
    m_pvVertices = 0;
    m_nFaces = 0;
    m_nAABBs = 0;
    m_pFaces = 0;
    m_pnMaterials = 0;
    m_pvNormals = 0;
    m_pfPlaneDistances = 0;
}

// FUNCTION: SWKOTOR 0x005970E0
void CSWCollisionMesh::GetVertex(int n, Vector *pOut)
{
    if (m_pvVertices && n < m_nVertices)
        *pOut = GetWorldPoint(m_pvVertices[n]);
}

// FUNCTION: SWKOTOR 0x00597120
int CSWCollisionMesh::LoadMeshBinary(CResWOK *pRes)
{
    m_bLoaded = 1;
    m_bTransformed = pRes->m_pHeader ? pRes->m_pHeader->m_nType : 0;
    m_avUse[0] = pRes->m_pHeader ? pRes->m_pHeader->m_vUse1 : Vector(0.0f, 0.0f, 0.0f);
    m_avUse[1] = pRes->m_pHeader ? pRes->m_pHeader->m_vUse2 : Vector(0.0f, 0.0f, 0.0f);
    m_nVertices = pRes->m_pHeader ? pRes->m_pHeader->m_nVertices : 0;
    m_vOffset = pRes->m_pHeader ? pRes->m_pHeader->m_vPosition : Vector(0.0f, 0.0f, 0.0f);
    m_pvVertices = pRes->m_pHeader ? (Vector *)(pRes->m_pHeader->m_nVertexOffset + pRes->m_pData) : 0;
    m_nFaces = pRes->m_pHeader ? pRes->m_pHeader->m_nFaces : 0;
    m_nAABBs = pRes->m_pHeader ? pRes->m_pHeader->m_nAABBs : 0;
    m_pFaces = pRes->m_pHeader ? (CSWCollisionFace *)(pRes->m_pHeader->m_nFaceOffset + pRes->m_pData) : 0;
    m_pnMaterials = pRes->m_pHeader ? (int *)(pRes->m_pHeader->m_nMaterialOffset + pRes->m_pData) : 0;
    m_pvNormals = pRes->m_pHeader ? (Vector *)(pRes->m_pHeader->m_nNormalOffset + pRes->m_pData) : 0;
    m_pfPlaneDistances = pRes->m_pHeader ? (float *)(pRes->m_pHeader->m_nPlaneOffset + pRes->m_pData) : 0;
    return 1;
}

// NONMATCH (21): original holds the face offset in edi and the three vertex
// ids in ecx/ebx/ebp, with no pFace pointer add; shapes tried: direct
// m_pFaces[i].m_anVertex[k], face copy, inline GetTriangle helpers.
// FUNCTION: SWKOTOR 0x00599F70
void CSWCollisionMesh::RenderMesh(float fR, float fG, float fB)
{
    Vector v0, v1, v2;
    PrepareRender();
    if (!m_nFaces)
        return;
    for (int i = 0; i < m_nFaces; i++) {
        if (i < m_nFaces) {
            CSWCollisionFace *pFace = &m_pFaces[i];
            int n0 = pFace->m_anVertex[0];
            int n1 = pFace->m_anVertex[1];
            int n2 = pFace->m_anVertex[2];
            if (n0 != -1 && n1 != -1 && n2 != -1) {
                if (m_pvVertices && n0 < m_nVertices)
                    v0 = GetWorldPoint(m_pvVertices[n0]);
                if (m_pvVertices && n1 < m_nVertices)
                    v1 = GetWorldPoint(m_pvVertices[n1]);
                if (m_pvVertices && n2 < m_nVertices)
                    v2 = GetWorldPoint(m_pvVertices[n2]);
            }
        }
    }
}
