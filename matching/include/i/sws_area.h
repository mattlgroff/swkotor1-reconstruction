// CSWSArea room / DFS-table / transition helpers
// (0x004BB600-0x004BC57F). Self-contained declarations; only the members
// these functions use are placed.
#ifndef I_SWS_AREA_H
#define I_SWS_AREA_H

#include <stddef.h>

struct Vector {
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }
    Vector operator+(const Vector& v) const { return Vector(x + v.x, y + v.y, z + v.z); }
    float MagnitudeSquared() const { return x * x + y * y + z * z; }
    float x, y, z;
};

Vector NormalizeVector(const Vector& v);                    // 0x004AB180
int operator==(const Vector& a, const Vector& b);          // 0x004AA980 (cdecl)
// The out-of-line free operator-(const Vector&, const Vector&); declared
// under another name because Vector here carries the inline member form.
Vector VectorSubtract(const Vector& a, const Vector& b);    // 0x00423BC0
int IntersectCircle2d(const Vector& vCenter, float fRadius, const Vector& vFrom, const Vector& vTo, Vector* pNear, Vector* pFar, int bSegment);  // 0x004AAB50

template <class T> class CExoArrayList {
public:
    void Allocate(int n);                           // <unsigned long>: 0x005E03C0
    void Add(T t);                                  // <unsigned long>: 0x00671C00
    // Add expanded in place: the original inlines the first Add of
    // CSWSArea::IntersectingLineSegment and calls 0x00671C00 elsewhere.
    void AddInline(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num++] = t;
    }
    void DelIndex(int nIndex)
    {
        num--;
        for (; nIndex < num; nIndex++) {
            element[nIndex] = element[nIndex + 1];
        }
    }
    void Remove(T t)
    {
        int i;
        for (i = 0; i < num; i++) {
            if (element[i] == t) {
                break;
            }
        }
        DelIndex(i);
    }

    T* element;
    int num;
    int array_size;
};

// Collision query result (0x20 bytes).
class CSWCollisionResult {
public:
    CSWCollisionResult(unsigned long nMask);            // 0x00581320

    unsigned long m_nMask;      // +0x00
    char pad_0x4[0x4];
    int m_bHit;                 // +0x08
    int m_nFace;                // +0x0C
    Vector m_vHit;              // +0x10
    char pad_0x1c[0x4];
};

// 0x48 bytes.
class CWalkHitInfo {
public:
    void Initialize(int nType, const Vector& v1, const Vector& v2, const Vector& v3,
                    const Vector& v4, int nIndex, const Vector& v5);    // 0x004BB570

    char pad_0x0[0x48];
};

// 0x4c bytes, an array of them at CSWSArea +0x230.
class CSWRoomWalkMesh {
public:
    int ClipSegmentToRoom(Vector vStart, Vector vEnd, Vector* pClipStart, Vector* pClipEnd,
                          int* pEdge);              // 0x00583E30 (placeholder name)
};

class CSWSRoom {
public:
    virtual ~CSWSRoom();                                                // 0x005794B0
    float ComputeHeight(Vector vPosition, int nArg);                    // 0x00579430
    float ComputeHeightSecondary(int nFace, Vector vPosition);          // 0x00579470
    int CheckAABBWalkable(Vector vStart, Vector vEnd,
                          CSWCollisionResult* pResult);                 // 0x00579560
    int CheckAABBAll(Vector vStart, Vector vEnd,
                     CSWCollisionResult* pResult);                      // 0x00579610
    int GetAdjacentRoom(int nEdge);                                     // 0x00579420
    int ClipSegmentWalkable(Vector* pStart, const Vector& vEnd, Vector* pClipStart,
                            Vector* pClipEnd, float* pZLow, float* pZHigh,
                            int* pEdge);                                // 0x00579670 (placeholder name)
    int NoNonWalkPolysOnRoom(Vector* pStart, Vector* pEnd, float* pZLow, float* pZHigh,
                             const float& fRadius, CWalkHitInfo* pHit); // 0x00579680

    char pad_0x4[0x38 - 0x4];
    int m_bBlocked;                                                     // +0x38 (placeholder name)
    CSWRoomWalkMesh* m_pWalkMesh;                                       // +0x3c (placeholder name)
    char pad_0x40[0x4c - 0x40];
};

class CSWSObject;
class CSWSCreature;
class CSWSDoor;
class CSWSTrigger;
class CSWSPlaceable;
class CSWSArea;
class CSWSAreaOfEffectObject;
class CSWSEncounter;

class CGameObject {
public:
    virtual ~CGameObject() {}                       // 0x00
    virtual void v04(); virtual void v08(); virtual void v0c();
    virtual CSWSObject* AsSWSObject();              // 0x10
    virtual void v14();
    virtual CSWSDoor* AsSWSDoor();                  // 0x18
    virtual void v1c(); virtual void v20(); virtual void v24();
    virtual CSWSArea* AsSWSArea();                  // 0x28
    virtual void v2c();
    virtual CSWSCreature* AsSWSCreature();          // 0x30
    virtual void v34(); virtual void v38(); virtual void v3c();
    virtual CSWSTrigger* AsSWSTrigger();            // 0x40
    virtual void v44();
    virtual CSWSPlaceable* AsSWSPlaceable();        // 0x48
    virtual void v4c();
    virtual CSWSAreaOfEffectObject* AsSWSAreaOfEffectObject();  // 0x50
    virtual void v54(); virtual void v58();
    virtual CSWSEncounter* AsSWSEncounter();        // 0x5c

    unsigned long m_idSelf;                         // 0x04
    unsigned char m_nObjectType;                    // 0x08
    char pad_0x9[0x3];
};

class CPathfindInformation {
public:
    char pad_0x0[0x4];
    float m_fRadius;                                // +0x04 (placeholder name)
    float m_fPersonalSpace;                         // +0x08 (placeholder name)
    char pad_0xc[0x14 - 0xc];
    float m_fHeight;                                // +0x14 (placeholder name)
    char pad_0x18[0x2c - 0x18];
    unsigned long m_oidCreature;                    // +0x2c
    unsigned long m_oidTarget;                      // +0x30 (placeholder name)
    char pad_0x34[0x6c - 0x34];
    int m_nGridStepTolerance;                       // +0x6c (placeholder name)
    char pad_0x70[0xbc - 0x70];
    unsigned long m_oidIgnore;                      // +0xbc (placeholder name)
    char pad_0xc0[0x15c - 0xc0];
    int* m_pGridPath;                               // +0x15c (placeholder name, x/y pairs)
    int m_nGridPathLength;                          // +0x160 (placeholder name)
    char pad_0x164[0x188 - 0x164];
    int m_nBestGridPathLength;                      // +0x188 (placeholder name)
    int* m_pBestGridPath;                           // +0x18c (placeholder name)
    int m_nBestGridDistance;                        // +0x190 (placeholder name)
    char pad_0x194[0x210 - 0x194];
    int m_bIgnoreCreatures;                         // +0x210 (placeholder name)
};

class CSWSObject : public CGameObject {
public:
    CSWSArea* GetArea();                            // 0x004CB120

    char pad_0xc[0x90 - 0xc];
    Vector m_vPosition;                             // 0x90
};

class CSWSCreature : public CSWSObject {
public:
    char pad_0x9c[0x340 - 0x9c];
    CPathfindInformation* m_pPathfindInformation;   // 0x340
};

class CSWSDoor : public CSWSObject {
public:
    int GetIsLinkedToDoorOrTrigger();               // 0x005890D0
    int InDoorArea(Vector vPosition);               // 0x005897D0 (placeholder name)
    CSWSObject* GetLinkedObject();                  // 0x00589580
    int NoNonWalkPolys(float fX1, float fY1, float* pX2, float* pY2, float* pZ,
                       int nArg);                   // 0x005890F0
};

class CSWSPlaceable : public CSWSObject {
public:
    int NoNonWalkPolys(float fX1, float fY1, float* pX2, float* pY2, float* pZ,
                       int nArg);                   // 0x00585350
};

class CSWSAreaOfEffectObject : public CSWSObject {
public:
    int LineSegmentIntersectAreaOfEffect(Vector vStart, Vector vEnd);  // 0x005950C0 (placeholder name)
    int InAreaOfEffect(Vector vPosition);           // 0x00595450 (placeholder name)
};

class CSWSEncounter : public CSWSObject {
public:
    int LineSegmentIntersectActivateArea(Vector* pStart, Vector* pEnd);  // 0x0058FFD0
    int InActivateArea(Vector* pPosition);          // 0x00590090 (placeholder name)
};

class CSWSTrigger : public CSWSObject {
public:
    int LineSegmentIntersectTrigger(Vector* pStart, Vector* pEnd);  // 0x0058CD80
    int InTrigger(Vector* pPosition);               // 0x0058CE40 (placeholder name)
    unsigned long GetTargetArea();                  // 0x0058C830

    char pad_0x9c[0x2b4 - 0x9c];
    int m_bAreaTransition;                          // 0x2b4 (placeholder name)
};

extern const unsigned char g_nGameObjectFound;      // 0x0074666C
extern const unsigned char g_nObjectTypeTrigger;    // 0x00746636 (placeholder name)
extern int g_bDebugOverlap;                         // 0x00832838 (placeholder name)
extern int g_bDebugDirectLine;                      // 0x008327C4 (placeholder name)
extern int g_bDebugDirectLineHeights;               // 0x008327C8 (placeholder name)
int VectorEqual(const Vector& a, const Vector& b);  // 0x004AA980 (placeholder name)

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long oid, CGameObject** ppObject);  // 0x004D8230
};

class CSWSPlayer;

class CServerExoApp {
public:
    CGameObject* GetGameObject(unsigned long oid);                  // 0x004AE750
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid);     // 0x004AE770
    CSWSPlayer* GetClientObjectByObjectId(unsigned long oid);       // 0x004AE910
    CGameObjectArray* GetObjectArray();                             // 0x004AED70
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp* m_pServerExoApp;                 // 0x8
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC
inline CServerExoApp* GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }

// DFS transposition-table entry (0x10 bytes, 0x200 entries).
struct CGridDFSTransEntry {
    int nX;
    int nY;
    int nCost;
    int nDepth;
};

struct CPathPointDFSTransEntry {
    float fX;
    float fY;
    float fCost;
    float fDepth;
};

#define OBJECT_INVALID 0x7f000000

class CSWSArea {
public:
    CSWSRoom* GetRoom(const Vector& vPosition, int* pFace);                 // 0x004BB600
    CSWSRoom* GetRoomByIndex(int nIndex);                                   // 0x004BB700 (placeholder name)
    int GetRoomIndex(const Vector& vPosition, int bAll);                    // 0x004BB730
    float ComputePathLength(int nCount, float* pPoints);                    // 0x004BB860 (placeholder name)
    int CountVisibleToPlayers(const Vector& vPosition, int nMax);           // 0x004BB8B0
    int EvaluateOverlappingTargets(CPathfindInformation* pInfo, Vector* pA,
                                   unsigned long oidTarget, Vector* pB, float fRange,
                                   int a6, int bCheck, unsigned long* pOut);  // 0x004BBA40
    void GridDFSTransTableInitialize();                                     // 0x004BBC50
    int GridDFSTransTableGet(int nX, int nY, int* pCost, int* pDepth);      // 0x004BBCC0
    void GridDFSTransTablePut(int nX, int nY, int nCost, int nDepth);       // 0x004BBD80
    int GridDFSGenerateSuccessors(int nCost, int nLimit, int nX, int nY, int nZ,
                                  int** ppSuccessors);                      // 0x004BCEE0 (placeholder name)
    int GridDFSearch(int nDepth, int nCost, int nLimit, int nX, int nY, int nZ);  // 0x004C0930
    int NoNoneWalkPolysInStaticObject(unsigned long oid, float fX1, float fY1, float fX2,
                                      float fY2, float fZ, int a7, int a8,
                                      int nArg);                            // 0x004BBE20
    void PathPointDFSTransTableInitialize();                                // 0x004BBEB0
    int PathPointDFSTransTableGet(float fX, float fY, float* pCost, float* pDepth);  // 0x004BBF20
    void PathPointDFSTransTablePut(float fX, float fY, float fCost, float fDepth);   // 0x004BC010
    int SetHitInfoForCreature(const Vector& vCenter, float fRadius, const Vector& vFrom, const Vector& vTo,
                              CWalkHitInfo* pHit);                          // 0x004BC0E0
    float ComputeHeight(const Vector& vPosition, int bUseFace, CSWSRoom** ppRoom);  // 0x004BC380
    int CountAreaTransitionTriggers(unsigned long* pList, int nExcluded,
                                    unsigned long* pExcluded);              // 0x004BC400
    int IntersectingLineSegment(Vector* pStart, Vector* pEnd,
                                CExoArrayList<unsigned long>* pList);       // 0x004BF2C0
    int InSubAreas(Vector* pPosition, CExoArrayList<unsigned long>* pList); // 0x004BF470
    int GetFirstObjectIdInArea(unsigned long* pOid);                        // 0x005071B0 (placeholder name)
    int GetNextObjectIdInArea(unsigned long* pOid);                         // 0x005071E0 (placeholder name)
    int ClearLineOfSight(Vector* pFrom, Vector* pTo, Vector* pHit, int* pOut,
                         unsigned long oidExclude1, unsigned long oidExclude2,
                         int a7, int a8);                                   // 0x0050C330
    int NoNonWalkPolysDetailed(const Vector& vStart, const Vector& vEnd, float fZLow,
                               float fZHigh, float fRadius, unsigned long oidIgnore,
                               unsigned long* pBlocker, int a8, int a9, int a10,
                               CWalkHitInfo* pHit, int a12);                // 0x00506650
    int NoCreaturesOnLine(const Vector& vStart, const Vector& vEnd, CPathfindInformation* pInfo,
                          int a4, int a5, unsigned long* pBlocker, int a7,
                          CWalkHitInfo* pHit);                              // 0x004BC590
    int TestDirectLine(const Vector& vFrom, const Vector& vTo, const float& fRadius,
                       float fHeight, int nUnused, CWalkHitInfo* pHit);     // 0x004BCB70
    int ComputeBestCorner(float fX0, float fY0, float fX1, float fY1, float fX2, float fY2,
                          float fRadius, float fHeight, float* pX1, float* pY1,
                          float* pX2, float* pY2);                  // 0x004BE3A0
    int NoNonWalkPolys(const Vector& vStart, const Vector& vEnd, float fZLow, float fZHigh,
                       float fRadius, unsigned long oidIgnore, CWalkHitInfo* pHit,
                       int nArg);                   // 0x005086D0
    int ValidateSafePosition(const Vector& vPosition, CPathfindInformation* pInfo);  // 0x004BE5E0 (placeholder name)

    char pad_0x0[0x120];
    unsigned long m_idSelf;                         // 0x120 (CGameObject at 0x11c)
    char pad_0x124[0x190 - 0x124];
    CExoArrayList<unsigned long> m_aGameObjects;    // 0x190
    char pad_0x19c[0x1a0 - 0x19c];
    CExoArrayList<unsigned long> m_aSubAreas;       // 0x1a0 (placeholder name)
    CPathfindInformation* m_pPathfindInformation;   // 0x1ac
    char pad_0x1b0[0x1b4 - 0x1b0];
    CGridDFSTransEntry* m_pGridDFSTransTable;       // 0x1b4
    char pad_0x1b8[0x1cc - 0x1b8];
    int m_bGridDFSAbort;                            // 0x1cc (placeholder name)
    int m_nGridDFSNodes;                            // 0x1d0 (placeholder name)
    char pad_0x1d4[0x1e0 - 0x1d4];
    int m_bTransitionCounted;                       // 0x1e0 (placeholder name)
    char pad_0x1e4[0x22c - 0x1e4];
    int m_nRooms;                                   // 0x22c
    CSWSRoom* m_pRooms;                             // 0x230
    char pad_0x234[0x250 - 0x234];
    CPathPointDFSTransEntry* m_pPathPointDFSTransTable;  // 0x250
};

#endif
