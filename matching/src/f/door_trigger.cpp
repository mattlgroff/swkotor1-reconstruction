// CSWSDoor / CSWSTrigger area glue (b0058_f2_door_trigger).

class Vector {
public:
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    float x, y, z;
};

class Quaternion {
public:
    float w, x, y, z;
};

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    void Allocate(int s);
    void Remove(T t);
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
    char m_resRef[16];
};

class CResStruct {
public:
    unsigned long m_nStructIndex;
};

class CResGFF {
public:
    CResGFF(unsigned short nType, char *sFileType, const CResRef &cResRef);
    virtual ~CResGFF();
    void GetTopLevelStruct(CResStruct *pStruct);
    char pad_0x4[0x90];
    int m_bLoaded;                   // +0x94
    char pad_0x98[0x8];
};

class CSWSObject;
class CSWSWaypoint;
class CSWSArea;
class CSWSCreature;

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual CSWSObject *AsSWSObject();         // +0x10
    virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54();
    virtual CSWSWaypoint *AsSWSWaypoint();     // +0x58

    unsigned long m_idSelf;          // +0x4
    unsigned char m_nObjectType;     // +0x8
    char pad_0x9[0x3];
};

class CGameObjectArray {
public:
    char GetGameObject(unsigned long id, CGameObject **ppObject);
};

class CSWSModule {
public:
    unsigned long GetWaypoint(const CExoString &sTag);
    unsigned long FindObjectByTagOrdinal(const CExoString &sTag, int nNth);
};

class CSWSDoor;

class CServerExoApp {
public:
    CSWSModule *GetModule();
    CGameObject *GetGameObject(unsigned long id);
    CSWSCreature *GetCreatureByGameObjectID(unsigned long id);
    CSWSDoor *GetDoorByGameObjectID(unsigned long id);
    unsigned long GetPlayerCreatureId();
    CGameObjectArray *GetObjectArray();
    CGameObject *GetObjectByGameObjectID(unsigned long id);
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;
extern char g_cObjectArraySuccess;
extern int g_nDoorStateClosed;
extern int g_nDoorStateOpen1;
extern int g_nDoorStateOpen2;
extern unsigned char g_nFeatMines;

class CSWCollisionMesh {
public:
    int GetMeshOutline(Vector *p1, Vector *p2, Vector *p3, Vector *p4);
    Vector GetWorldPoint(const Vector &v);
    char pad_0x0[0x20];
    Vector m_vPosition;              // +0x20
    char pad_0x2c[0xc];
    Quaternion m_qOrientation;       // +0x38
    void SetOrientation(Quaternion q) { m_qOrientation = q; }
    char pad_0x48[0x28];
    Vector m_avActionPoints[2];      // +0x70
};

struct CSWSArea11C {
    unsigned long m_n0;
    unsigned long m_n4;
};

class CSWArea {
public:
    virtual ~CSWArea();
    char pad_0x4[0x118];
};

class CGameObjectBase {
public:
    virtual void v00();
    unsigned long GetGameObjectID() { return m_idSelf; }
    unsigned long m_idSelf;
};

class CSWSArea : public CSWArea, public CGameObjectBase {
public:
    void RemoveObjectFromArea(unsigned long oid);
    void AddObjectToArea(unsigned long oid, int bFlag);
    void RemoveDoor(unsigned long oid);
    int CountDoorId(unsigned long oid)
    {
        int nCount = 0;
        for (int i = 0; i < m_aDoorIds.num; i++)
            if (m_aDoorIds.element[i] == oid)
                nCount++;
        return nCount;
    }
    void RemoveDoorId(unsigned long oid) { m_aDoorIds.Remove(oid); }
    void AddObjectId(unsigned long oid) { m_aObjectIds.Add(oid); }
    void AddStaticBoundingBox(unsigned long oid, Vector *pMin, Vector *pMax);
    int LineSegmentsIntersect(float x1, float y1, float x2, float y2, float x3, float y3,
                              float x4, float y4, float *pX, float *pY);
    char pad_0x124[0x8];
    CExoArrayList<unsigned long> m_aDoorIds;   // +0x12c
    char pad_0x138[0x68];
    CExoArrayList<unsigned long> m_aObjectIds; // +0x1a0
};

class CSWSObject : public CGameObject {
public:
    CSWSArea *GetArea();
    void SetArea(CSWSArea *pArea);
    void SetPosition(const Vector &v, int a, int b, int c);

    char pad_0xc[0x84];
    Vector m_vPosition;              // +0x90
    Vector m_vOrientation;           // +0x9c
};

class CSWSWaypoint : public CSWSObject {
};

class CSWSDoor : public CSWSObject {
public:
    int GetMeshOutline(Vector vPosition, Vector vOrientation, Vector *p1, Vector *p2,
                       Vector *p3, Vector *p4);
    Vector GetActionPoint(int nPoint, unsigned char nIndex);
    CSWSObject *GetLinkedObject();
    void MoveToNextOpenState();
    int LoadFromTemplate(const CResRef &cResRef);
    int LoadDoor(CResGFF *pGFF, CResStruct *pStruct, int bFlag);
    void RemoveFromArea();
    void AddToArea(CSWSArea *pArea, float x, float y, float z, int bFlag);
    int CheckPlacement(Vector vPosition, Vector vOrientation, Vector *pMin, Vector *pMax);

    char pad_0xa8[0x224];
    unsigned char m_nOpenState;      // +0x2cc
    unsigned char m_nNextOpenState;  // +0x2cd
    char pad_0x2ce[0x5e];
    CSWCollisionMesh *m_apWalkMeshes[3]; // +0x32c
    Vector m_vBBMin;                 // +0x338
    Vector m_vBBMax;                 // +0x344
    Vector m_avOutline[4];           // +0x350
    int m_n380;                      // +0x380
    unsigned char m_nLinkedToFlags;  // +0x384
    char pad_0x385[0x3];
    CExoString m_sLinkedTo;          // +0x388
    char pad_0x390[0x8];
    unsigned long m_oidLinkedTo;     // +0x398
    char pad_0x39c[0x14];
    Quaternion m_qOrientation;       // +0x3b0
};

class CSWSAreaDoorState {
public:
    void SetDoorState(Vector *pPosition, Vector *pMin, Vector *pMax, unsigned long oid, int bClosed);
};

// FUNCTION: SWKOTOR 0x00588940
int CSWSDoor::GetMeshOutline(Vector vPosition, Vector vOrientation, Vector *p1, Vector *p2,
                             Vector *p3, Vector *p4)
{
    m_apWalkMeshes[g_nDoorStateClosed]->m_vPosition = vPosition;
    m_apWalkMeshes[g_nDoorStateClosed]->SetOrientation(m_qOrientation);
    return m_apWalkMeshes[g_nDoorStateClosed]->GetMeshOutline(p1, p2, p3, p4);
}

// FUNCTION: SWKOTOR 0x005891B0
Vector CSWSDoor::GetActionPoint(int nPoint, unsigned char nIndex)
{
    switch (nPoint) {
    case 0x2726:
        return m_apWalkMeshes[g_nDoorStateClosed]->GetWorldPoint(
            m_apWalkMeshes[g_nDoorStateClosed]->m_avActionPoints[nIndex]);
    case 0x2742:
        return m_apWalkMeshes[g_nDoorStateOpen1]->GetWorldPoint(
            m_apWalkMeshes[g_nDoorStateOpen1]->m_avActionPoints[nIndex]);
    case 0x2743:
        return m_apWalkMeshes[g_nDoorStateOpen2]->GetWorldPoint(
            m_apWalkMeshes[g_nDoorStateOpen2]->m_avActionPoints[nIndex]);
    }
    return Vector(-9999999.0f, -9999999.0f, -9999999.0f);
}

// FUNCTION: SWKOTOR 0x00589580
CSWSObject *CSWSDoor::GetLinkedObject()
{
    CSWSObject *pObject = 0;
    if (m_nLinkedToFlags == 1 || m_nLinkedToFlags == 2) {
        CSWSDoor *pDoor = g_pAppManager->GetServerExoApp()->GetDoorByGameObjectID(m_oidLinkedTo);
        if (!pDoor) {
            m_oidLinkedTo = g_pAppManager->GetServerExoApp()->GetModule()->FindObjectByTagOrdinal(m_sLinkedTo, 0);
            CGameObject *pGameObject = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidLinkedTo);
            if (pGameObject)
                return pGameObject->AsSWSObject();
        }
        pObject = pDoor;
    }
    return pObject;
}

class CSWSAreaGlue {
public:
    void SetDoorState(Vector *pPosition, Vector *pMin, Vector *pMax, unsigned long oid, int bClosed);
};

// FUNCTION: SWKOTOR 0x00589680
void CSWSDoor::MoveToNextOpenState()
{
    unsigned char nOld = m_nOpenState;
    m_nOpenState = m_nNextOpenState;
    if (m_nOpenState != nOld && m_nOpenState != 3) {
        CSWSAreaGlue *pArea = (CSWSAreaGlue *)GetArea();
        if (pArea)
            pArea->SetDoorState(&m_vPosition, &m_vBBMin, &m_vBBMax, m_idSelf, m_nOpenState == 0);
    }
}

// FUNCTION: SWKOTOR 0x0058B3D0
int CSWSDoor::LoadFromTemplate(const CResRef &cResRef)
{
    CResGFF *pGFF = new CResGFF(0x7fa, "UTD ", cResRef);
    if (!pGFF->m_bLoaded) {
        delete pGFF;
        return 0;
    }
    CResStruct cTop;
    pGFF->GetTopLevelStruct(&cTop);
    int bResult = LoadDoor(pGFF, &cTop, 1);
    delete pGFF;
    return bResult;
}

// FUNCTION: SWKOTOR 0x0058B490
void CSWSDoor::RemoveFromArea()
{
    if (GetArea()) {
        GetArea()->RemoveObjectFromArea(m_idSelf);
        unsigned long oidSelf = m_idSelf;
        CExoArrayList<unsigned long> *pList = &GetArea()->m_aDoorIds;
        int nCount = 0;
        for (int i = 0; i < pList->num; i++)
            if (pList->element[i] == oidSelf)
                nCount++;
        if (nCount == 1)
            GetArea()->RemoveDoorId(m_idSelf);
        GetArea()->RemoveDoor(m_idSelf);
    }
    SetArea(0);
    SetPosition(Vector(0.0f, 0.0f, 0.0f), 1, 1, 0);
}

// FUNCTION: SWKOTOR 0x0058B6D0
void CSWSDoor::AddToArea(CSWSArea *pArea, float x, float y, float z, int bFlag)
{
    RemoveFromArea();
    if (!pArea)
        return;
    SetArea(pArea);
    SetPosition(Vector(x, y, z), 0, 1, 0);
    pArea->AddObjectToArea(m_idSelf, bFlag);
    pArea->m_aObjectIds.Add(m_idSelf);
    if (m_apWalkMeshes) {
        if (CheckPlacement(m_vPosition, m_vOrientation, &m_vBBMin, &m_vBBMax)) {
            pArea->AddStaticBoundingBox(m_idSelf, &m_vBBMin, &m_vBBMax);
            GetMeshOutline(m_vPosition, m_vOrientation, &m_avOutline[0], &m_avOutline[1],
                           &m_avOutline[2], &m_avOutline[3]);
            m_n380 = 4;
        }
    }
}

// ---------------------------------------------------------------------------

class CSWSCreatureStats {
public:
    int HasFeat(int nFeat, int bFlag);
    char pad_0x0[0x78];
    int m_nFactionId;                // +0x78
};

class CSWSCreature : public CSWSObject {
public:
    int GetIsPC();
    int GetReputation(unsigned long oid, int bFlag);
    char pad_0xa8[0x9cc];
    CSWSCreatureStats *m_pStats;     // +0xa74
};

class CSWSPlayerCreature {
public:
    int GetIsActive();
};

class CSWSTrigger : public CSWSObject {
public:
    unsigned long GetTargetArea();
    int LoadFromTemplate(const CResRef &cResRef);
    int LoadTrigger(CResGFF *pGFF, CResStruct *pStruct);
    void RemoveFromArea();
    void AddToArea(CSWSArea *pArea, float x, float y, float z, int bFlag);
    void GetBoundingBox(float *pMinX, float *pMaxX, float *pMinY, float *pMaxY);
    void UpdateBounds() { float a, b, c, d; GetBoundingBox(&a, &b, &c, &d); }
    int LineSegmentIntersectTrigger(Vector *pStart, Vector *pEnd);
    int GetCanFireMineOnObject(unsigned long oidTarget, int bFlag);
    void SetCreator(unsigned long oidCreator);

    char pad_0xa8[0x188];
    CExoString m_sTransitionDestination; // +0x230
    char pad_0x238[0x50];
    Vector *m_pvVertices;            // +0x288
    char pad_0x28c[0x8];
    int m_nNumPoints;                // +0x294
    int *m_pnPointIndices;           // +0x298
    char pad_0x29c[0x18];
    int m_bIsTransition;             // +0x2b4
    int m_nFactionId;                // +0x2b8
    char pad_0x2bc[0x28];
    unsigned long m_oidCreator;      // +0x2e4
    char pad_0x2e8[0x2c];
    int m_bCreatorIsPlayer;          // +0x314
};

// FUNCTION: SWKOTOR 0x0058C830
unsigned long CSWSTrigger::GetTargetArea()
{
    if (m_bIsTransition) {
        unsigned long oidWaypoint =
            g_pAppManager->GetServerExoApp()->GetModule()->GetWaypoint(m_sTransitionDestination);
        CGameObject *pObject;
        if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidWaypoint, &pObject) ==
            g_cObjectArraySuccess)
            return pObject->AsSWSWaypoint()->GetArea()->GetGameObjectID();
    }
    return 0x7f000000;
}

// FUNCTION: SWKOTOR 0x0058CD80
int CSWSTrigger::LineSegmentIntersectTrigger(Vector *pStart, Vector *pEnd)
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

// NONMATCH (9 insns): the second faction compare loads both operands instead
// of cmp reg,[mem], and the feat byte is zero-extended with movzx instead of
// xor edx,edx / mov dl.
// FUNCTION: SWKOTOR 0x0058D4A0
int CSWSTrigger::GetCanFireMineOnObject(unsigned long oidTarget, int bFlag)
{
    int bCanFire = 1;
    CSWSCreature *pCreator = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(m_oidCreator);
    CSWSCreature *pTarget = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
    if (pTarget) {
        if (!bFlag) {
            if (pCreator) {
                if (pCreator->GetIsPC()) {
                    unsigned long oidCreator = pCreator->m_idSelf;
                    if (pTarget->GetReputation(oidCreator, 0) > 10 ||
                        m_nFactionId == pTarget->m_pStats->m_nFactionId)
                        bCanFire = 0;
                }
            } else {
                if (pTarget->GetReputation(m_idSelf, 0) > 10 ||
                    pTarget->m_pStats->m_nFactionId == m_nFactionId)
                    bCanFire = 0;
            }
        }
        if (pTarget->m_pStats->HasFeat(g_nFeatMines, 0))
            return 0;
    }
    return bCanFire;
}

class CSWSPlayerObj {
public:
    int GetIsPlayer();
};

class CServerExoAppEx {
public:
    unsigned long GetPlayerCreatureId();
    CSWSPlayerObj *GetPlayerByCreatureId(unsigned long oid);
};

// FUNCTION: SWKOTOR 0x0058D700
void CSWSTrigger::SetCreator(unsigned long oidCreator)
{
    m_oidCreator = oidCreator;
    if (oidCreator != 0x7f000000 &&
        (oidCreator == ((CServerExoAppEx *)g_pAppManager->m_pServerExoApp)->GetPlayerCreatureId() ||
         ((CServerExoAppEx *)g_pAppManager->m_pServerExoApp)->GetPlayerByCreatureId(oidCreator)->GetIsPlayer()))
        m_bCreatorIsPlayer = 1;
    else
        m_bCreatorIsPlayer = 0;
}

// FUNCTION: SWKOTOR 0x0058ED70
int CSWSTrigger::LoadFromTemplate(const CResRef &cResRef)
{
    CResGFF *pGFF = new CResGFF(0x7f0, "UTT ", cResRef);
    if (!pGFF->m_bLoaded) {
        delete pGFF;
        return 0;
    }
    CResStruct cTop;
    pGFF->GetTopLevelStruct(&cTop);
    int bResult = LoadTrigger(pGFF, &cTop);
    delete pGFF;
    return bResult;
}

// FUNCTION: SWKOTOR 0x0058EE30
void CSWSTrigger::RemoveFromArea()
{
    if (GetArea()) {
        GetArea()->RemoveObjectFromArea(m_idSelf);
        CSWSArea *pCurArea = GetArea();
        pCurArea->m_aObjectIds.Remove(m_idSelf);
        unsigned long oidSelf = m_idSelf;
        CExoArrayList<unsigned long> *pList = &GetArea()->m_aDoorIds;
        int nCount = 0;
        for (int i = 0; i < pList->num; i++)
            if (pList->element[i] == oidSelf)
                nCount++;
        if (nCount == 1)
            GetArea()->RemoveDoorId(m_idSelf);
    }
    SetArea(0);
    SetPosition(Vector(0.0f, 0.0f, 0.0f), 1, 1, 0);
}

// FUNCTION: SWKOTOR 0x0058F030
void CSWSTrigger::AddToArea(CSWSArea *pArea, float x, float y, float z, int bFlag)
{
    RemoveFromArea();
    if (pArea) {
        CExoString sUnused;
        SetArea(pArea);
        SetPosition(Vector(x, y, z), 0, 1, 0);
        GetArea()->AddObjectToArea(m_idSelf, bFlag);
        CSWSArea *pCurArea = GetArea();
        pCurArea->AddObjectId(m_idSelf);
        UpdateBounds();
    }
}
