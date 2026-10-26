#ifndef F_ROOM_SURFACE_MESH_H
#define F_ROOM_SURFACE_MESH_H

// CSWCollisionMesh and CSWRoomSurfaceMesh (walkmesh) layouts for the
// 0x00580340-0x00582D70 and 0x00596670-0x0059A040 clusters.

#include <stdio.h>

class Vector {
public:
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator+(const Vector &v) const { return Vector(x + v.x, y + v.y, z + v.z); }
    Vector operator-(const Vector &v) const { return Vector(x - v.x, y - v.y, z - v.z); }
    Vector &operator*=(float f);
    Vector &operator+=(const Vector &v);
    float x, y, z;
};

Vector Normalize(const Vector &v);
Vector NormalOf(const Vector &v0, const Vector &v1, const Vector &v2);
int BoxIntersect(Vector *pStart, Vector *pEnd, Vector *pMin, Vector *pMax);
int PolyHit(Vector *pPoly, int nVerts, Vector *pStart, Vector *pEnd, Vector *pEnd2, Vector *pHit);
double PlaneHeightAtPoint(const Vector &vNormal, float fDistance, const Vector &vPoint);

class Quaternion {
public:
    float w, x, y, z;
};

Quaternion operator~(const Quaternion &q);
Vector operator*(const Quaternion &q, const Vector &v);

class CExoString {
public:
    CExoString();
    CExoString(const char *s);
    ~CExoString();
    char *CStr() const;
    void Format(const char *szFormat, ...);
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef();
    CResRef operator=(const CResRef &r);
    char m_resRef[16];
};



class CRes {
public:
    virtual ~CRes();
    void *Demand();
    int Release();
    int Dump(int bForce);
    char pad_0x4[0x2c];
};

class CExoResMan {
public:
    int Exists(const CResRef &cResRef, unsigned short nType, unsigned long *pTable);
    CRes *GetResObject(const CResRef &cResRef, unsigned short nType);
    void SetResObject(const CResRef &cResRef, unsigned short nType, CRes *pRes);
};

extern CExoResMan *g_pExoResMan;

class C2DA {
public:
    int GetCExoStringEntry(int nRow, const CExoString &sColumn, CExoString *pValue);
    int GetINTEntry(int nRow, const CExoString &sColumn, int *pValue);
};

class C2DArrays {
public:
    char pad_0x0[0x14];
    C2DA *m_pSurfaceMat;             // +0x14
    C2DA *GetSurfaceMat() { return m_pSurfaceMat; }
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    C2DArrays *m_p2DArrays;          // +0xb8
    C2DArrays *Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules *g_pRules;

// The BWM file header (0x88 bytes).
struct CSWWalkMeshHeader {
    CSWWalkMeshHeader();
    char m_sFileType[8];             // +0x00
    unsigned long m_nType;           // +0x08
    Vector m_avUse[2];               // +0x0c
    Vector m_avDock[2];              // +0x24
    Vector m_vPosition;              // +0x3c
    int m_nVertices;                 // +0x48
    unsigned long m_nVertexOffset;   // +0x4c
    int m_nFaces;                    // +0x50
    unsigned long m_nFaceOffset;     // +0x54
    unsigned long m_nMaterialOffset; // +0x58
    unsigned long m_nNormalOffset;   // +0x5c
    unsigned long m_nPlaneOffset;    // +0x60
    int m_nAABBs;                    // +0x64
    unsigned long m_nAABBOffset;     // +0x68
    int m_nAABBRoot;                 // +0x6c
    int m_nWalkableFaces;            // +0x70
    unsigned long m_nAdjacencyOffset; // +0x74
    int m_nPerimeterEdges;           // +0x78
    unsigned long m_nPerimeterOffset; // +0x7c
    int m_nRooms;                    // +0x80
    unsigned long m_nRoomOffset;     // +0x84
};

class CResMDL : public CRes {
public:
    CResMDL();
    virtual ~CResMDL();
    char *GetMDLDataPtr();
    int GetMDLSize();
    int m_n30;                       // +0x30
};

class CResBWM : public CRes {
public:
    CResBWM();
    virtual ~CResBWM();
    int IsBinary();
    char *GetBWMDataPtr();
    int GetBWMSize();
    char *m_pData;                   // +0x30
    CSWWalkMeshHeader *m_pHeader;    // +0x34
};

template <class T>
class CExoArrayList {
public:
    CExoArrayList() { num = 0; array_size = 0; element = NULL; }
    ~CExoArrayList() { delete[] element; element = NULL; }
    CExoArrayList<T> &operator=(const CExoArrayList<T> &other);
    void Allocate(int s)
    {
        T *old = element;
        array_size = s;
        element = new T[s];
        for (int i = 0; i < num; i++)
            element[i] = old[i];
        if (old)
            delete[] old;
    }
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num++] = t;
    }
    void Insert(T t, int pos)
    {
        int i = num;
        Add(t);
        for (; i > pos; i--)
            element[i] = element[i - 1];
        element[pos] = t;
    }
    T *element;
    int num;
    int array_size;
};

// A list that can also point into a loaded resource (m_bExternal).
template <class T>
class CSWMeshList : public CExoArrayList<T> {
public:
    CSWMeshList() { m_bExternal = 0; }
    void SetExternal(T *pData, int nCount)
    {
        if (!this->num) {
            this->element = pData;
            this->num = nCount;
            this->array_size = nCount;
            m_bExternal = 1;
        }
    }
    void Clear()
    {
        if (!m_bExternal) {
            if (this->element) {
                delete[] this->element;
                this->element = NULL;
                this->array_size = 0;
            }
            this->num = 0;
        }
    }
    int m_bExternal;
};

struct CSWCollisionResult {
    CSWCollisionResult(unsigned long nMask)
    {
        m_nFace = -1;
        m_nMask = nMask;
        m_n04 = 0;
        m_bHit = 0;
        m_nBit = 0;
        m_vHit = Vector(0.0f, 0.0f, 0.0f);
    }
    unsigned long m_nMask;           // +0x00
    int m_n04;                       // +0x04
    int m_bHit;                      // +0x08
    int m_nFace;                     // +0x0c
    Vector m_vHit;                   // +0x10
    unsigned long m_nBit;            // +0x1c
};

class CSWRoomSurfaceMeshAABBNode {
public:
    CSWRoomSurfaceMeshAABBNode();
    ~CSWRoomSurfaceMeshAABBNode() {}
    Vector m_vMin;                   // +0x00
    Vector m_vMax;                   // +0x0c
    int m_nFace;                     // +0x18
    int m_nPlane;                    // +0x1c
    unsigned long m_nSplitMask;      // +0x20
    int m_nLeft;                     // +0x24
    int m_nRight;                    // +0x28
};

struct CSWCollisionFace {
    int m_anVertex[3];
};

struct CSWRoomEdge {
    int m_nVertexSlot;               // face * 3 + corner
    int m_nTransition;
};

class CSWCollisionMesh {
public:
    CSWCollisionMesh(unsigned short nA, unsigned short nB);
    virtual int v00();
    virtual int SaveBinaryMesh(const CExoString &sFile);
    virtual void TransformToWorld(const Vector &vPosition);
    virtual void GetResRef(CResRef *pResRef);
    virtual void GetResRef2(CResRef *pResRef);
    virtual unsigned long GetWalkCheckMask();
    virtual unsigned long GetLineOfSightMask();
    virtual unsigned long GetWalkMask();
    virtual unsigned long GetAllMask();
    virtual int LoadMeshBinary(CResBWM *pRes);
    virtual int LoadMeshText(char *pData, int nSize);
    ~CSWCollisionMesh();

    Vector WorldToLocal(const Vector &v);
    Vector GetWorldPoint(const Vector &v);
    void GetVertex(int n, Vector *pOut);
    void GetVertex(int n, float *pX, float *pY, float *pZ);
    int GetLine(char **ppData, unsigned long *pnSize, char *pLine, unsigned long nMaxLength);
    void ComputeNormals();
    int GetMeshBoundingBox(Vector &vMin, Vector &vMax);
    int GetMeshOutline(Vector *pCorner1, Vector *pCorner2, Vector *pCorner3, Vector *pCorner4);
    int LoadMesh(int bBinary);

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
    int m_nWalkableFaces;            // +0x5c
    CSWCollisionFace *m_pFaces;      // +0x60
    int *m_pnMaterials;              // +0x64
    Vector *m_pvNormals;             // +0x68
    float *m_pfPlaneDistances;       // +0x6c
    Vector m_avUse[2];               // +0x70
};

class CSWRoomSurfaceMesh : public CSWCollisionMesh {
public:
    CSWRoomSurfaceMesh();
    ~CSWRoomSurfaceMesh();
    virtual int SaveBinaryMesh(const CExoString &sFile);
    virtual void TransformToWorld(const Vector &vPosition);
    virtual void GetResRef(CResRef *pResRef);
    virtual unsigned long GetWalkCheckMask();
    virtual unsigned long GetLineOfSightMask();
    virtual unsigned long GetWalkMask();
    virtual unsigned long GetAllMask();
    virtual int LoadMeshBinary(CResBWM *pRes);
    virtual int LoadMeshText(char *pData, int nSize);

    void GetAdjacentFaces(int nUnused, int nFace, int *pA, int *pB, int *pC);
    int CheckAABBNode(int nNode, Vector *pStart, Vector *pEnd, CSWCollisionResult *pResult);
    void TransformAABBNode(int nNode, const Vector &vPosition);
    int GetPerimeterVertex(int nEdge, Vector &vOut);
    int GetEdgeVertices(int nEdge, Vector &vStart, Vector &vEnd);
    int WeldEdgeVertices(CSWRoomSurfaceMesh *pOther, float fTolerance, Vector *pUnused1, Vector *pUnused2);
    int CheckAABBAll(Vector vStart, Vector vEnd, CSWCollisionResult *pResult);
    int CheckAABBWalkable(Vector vStart, Vector vEnd, CSWCollisionResult *pResult);
    int FindTriangle(const Vector &vPosition, int bWalkCheck, Vector vNudge);
    int CheckAABBLineOfSight(Vector vStart, Vector vEnd, CSWCollisionResult *pResult);
    int SetAdjacency(CSWRoomSurfaceMesh *pOther, int nThisRoom, int nOtherRoom, Vector *pUnused1, Vector *pUnused2);
    void AddCrossing(const Vector &vUnused, const Vector &vDirection, const Vector &vPoint, int &nCrossings, int nData);
    int GetSurfaceMaterial(Vector vPosition);
    int GetSurfaceMaterialWalkCheckOnly(Vector vPosition);
    float ComputeHeightAABB(Vector vPosition, int bWalkCheck);

    CSWCollisionFace *m_pAdjacency;                  // +0x88
    CSWMeshList<CSWRoomEdge> m_lPerimeter;           // +0x8c
    CSWMeshList<int> m_lRooms;                       // +0x9c
    CSWMeshList<CSWRoomSurfaceMeshAABBNode> m_lAABB; // +0xac
    CExoArrayList<Vector> m_lCrossings;              // +0xbc
    CExoArrayList<int> m_lCrossingData;              // +0xc8
    int m_nAABBRoot;                                 // +0xd4
    unsigned long m_nLineOfSightMask;                // +0xd8
    unsigned long m_nWalkMask;                       // +0xdc
    unsigned long m_nWalkCheckMask;                  // +0xe0
    unsigned long m_nAllMask;                        // +0xe4
};

#endif
