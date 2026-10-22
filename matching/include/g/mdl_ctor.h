// MdlNode family constructors (0x00445210, 0x0044a300-0x0044ae00)
// with the member layouts their stores show. Behavioural reference:
// src/batches/b0044_mdlnode_lifecycle.cpp. The +0x50 / +0x54 slots are
// per-type function pointers (not vtables); lists are CExoArrayList
// (element, num, array_size; constructed num, array_size, element).
#ifndef G_MDL_CTOR_H
#define G_MDL_CTOR_H

typedef unsigned int size_t;

void* __cdecl operator new(size_t n);           // 0x006fa7e6
void __cdecl operator delete(void* p);          // 0x006fa390

// The sized constructor's dead `new` (nSize is always 0 here) is what
// leaves the original's empty-state SEH frame in the owners' ctors.
template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList(int nSize = 0)
    {
        num = 0;
        array_size = nSize;
        element = 0;
        if (array_size)
            element = new T[array_size];
    }
    ~CExoArrayList() { if (element) delete[] element; }
    T& operator[](int i) { return element[i]; }
    void Allocate(int n);
    void SetSize(int n);
    void Remove(T t)
    {
        int i;
        for (i = num - 1; i >= 0; i--) {
            if (element[i] == t)
                break;
        }
        num--;
        for (; i < num; i++)
            element[i] = element[i + 1];
    }
};

template <class T>
void CExoArrayList<T>::SetSize(int n)
{
    if (n == 0) {
        if (element) {
            delete[] element;
            element = 0;
        }
        array_size = 0;
    } else {
        Allocate(n);
    }
    num = n;
}

// A back-referenced pointer; the target keeps the list of SafePointers
// aimed at it (+0x04). The destructor is out of line (0x00422380).
template <class T>
class SafePointer {
public:
    T* m_p;

    SafePointer() { m_p = 0; }
    ~SafePointer();
    SafePointer<T>& operator=(T* p)
    {
        if (m_p)
            m_p->m_lRefs.Remove(this);
        m_p = p;
        return *this;
    }
    operator T*() { return m_p; }
};

class CAurTexture {
public:
    void* m_pVtbl;                                      // +0x00
    CExoArrayList<SafePointer<CAurTexture>*> m_lRefs;   // +0x04
};

void AurTextureRelease(CAurTexture* p);     // 0x00420d70
void PoolRelease(int nId);                  // 0x00423f50

// Triangle face (0x20 bytes) with an out-of-line destructor (0x0043e140).
class TriFace {
public:
    char pad00[0x20];

    ~TriFace();
};

extern "C" void* __cdecl memset(void* p, int c, size_t n);
#pragma intrinsic(memset)

class Vector {
public:
    float x, y, z;

    Vector() { x = 0; y = 0; z = 0; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

class Quaternion {
public:
    float w, x, y, z;

    Quaternion() { z = 0; y = 0; x = 0; w = 1.0f; }
    Quaternion(float fW, float fX, float fY, float fZ)
    {
        z = fZ;
        y = fY;
        x = fX;
        w = fW;
    }
};

class MdlNode;
class MaxTree;
class Model;
class Animation;
class MdlNodeLight;
class MdlNodeEmitter;
class MdlNodeCamera;
class MdlNodeReference;
class MdlNodeTriMesh;
class MdlNodeSkin;
class MdlNodeAnimMesh;
class MdlNodeDanglyMesh;
class MdlNodeAABB;
class MdlNodeTrigger;
class MdlNodeLightsaber;
typedef void (*MdlNodeFn)(MdlNode*);

extern int g_nMdlNodesAlive;    // 0x007a8a08
extern int g_nMdlNodesTotal;    // 0x007a8a0c

class MdlNode {
public:
    unsigned short m_nType;                 // +0x00
    short m_nNodeNumber;                    // +0x02
    int m_n04;                              // +0x04
    MaxTree* m_pTree;                       // +0x08
    MdlNode* m_pParent;                     // +0x0c
    Vector m_vPosition;                     // +0x10
    Quaternion m_qOrientation;              // +0x1c
    CExoArrayList<MdlNode*> m_lChildren;    // +0x2c
    CExoArrayList<int> m_lControllerKeys;   // +0x38
    CExoArrayList<float> m_lControllerData; // +0x44

    MdlNode(char* szName);                  // 0x00445210
    ~MdlNode();                             // 0x0044a040
    void InternalDestructorDispatch();      // 0x004488f0
    void InternalDestructor();              // 0x0043fdd0
    char* GetName();                        // 0x00438a00

    MdlNodeLight* AsMdlNodeLight();
    MdlNodeEmitter* AsMdlNodeEmitter();
    MdlNodeCamera* AsMdlNodeCamera();
    MdlNodeReference* AsMdlNodeReference();
    MdlNodeTriMesh* AsMdlNodeTriMesh();
    MdlNodeSkin* AsMdlNodeSkin();
    MdlNodeAnimMesh* AsMdlNodeAnimMesh();
    MdlNodeDanglyMesh* AsMdlNodeDanglyMesh();
    MdlNodeAABB* AsMdlNodeAABB();
    MdlNodeTrigger* AsMdlNodeTrigger();
    MdlNodeLightsaber* AsMdlNodeLightsaber();
};

class MdlNodeCamera : public MdlNode {
};

class MdlNodeTrigger : public MdlNode {
};

class MdlNodeEmitter : public MdlNode {
public:
    int m_n50;                  // +0x50
    int m_n54;                  // +0x54
    int m_n58;                  // +0x58
    int m_n5c;                  // +0x5c
    int m_n60;                  // +0x60
    int m_n64;                  // +0x64
    int m_n68;                  // +0x68
    int m_n6c;                  // +0x6c
    char m_szUpdate[32];        // +0x70
    char m_szRender[32];        // +0x90
    char m_szBlend[32];         // +0xb0
    char m_szTexture[32];       // +0xd0
    char m_szChunkName[16];     // +0xf0
    int m_n100;                 // +0x100
    int m_n104;                 // +0x104
    short m_n108;               // +0x108
    char m_n10a;                // +0x10a
    char m_sz10b[33];           // +0x10b
    int m_n12c;                 // +0x12c

    MdlNodeEmitter(char* szName);           // 0x0044a300
};

class MdlNodeLight : public MdlNode {
public:
    int m_n50;                              // +0x50
    CExoArrayList<SafePointer<CAurTexture> > m_lFlares; // +0x54
    CExoArrayList<float> m_lFlareSizes;     // +0x60
    CExoArrayList<float> m_lFlarePositions; // +0x6c
    CExoArrayList<Vector> m_lFlareColors;   // +0x78
    CExoArrayList<char*> m_lFlareTextures;  // +0x84
    int m_n90;                  // +0x90
    int m_n94;                  // +0x94
    int m_n98;                  // +0x98
    int m_n9c;                  // +0x9c
    int m_na0;                  // +0xa0
    int m_na4;                  // +0xa4
    int m_na8;                  // +0xa8

    MdlNodeLight(char* szName);             // 0x0044a3f0
    ~MdlNodeLight();                        // 0x00448f80
    void InternalDestructor();              // 0x00448000
    void BinaryInternalDestructor();        // 0x0044a500
};

class MdlNodeReference : public MdlNode {
public:
    char pad50[0x74 - 0x50];

    MdlNodeReference(char* szName);         // 0x0044a5c0
};

class MdlNodeTriMesh_PostProcess {
public:
    ~MdlNodeTriMesh_PostProcess();          // 0x004453b0
};

class MdlNodeTriMesh : public MdlNode {
public:
    MdlNodeFn m_pfn50;                      // +0x50
    MdlNodeFn m_pfn54;                      // +0x54
    CExoArrayList<TriFace> m_lFaces;        // +0x58
    Vector m_vBBoxMin;                      // +0x64
    Vector m_vBBoxMax;                      // +0x70
    float m_fRadius;                        // +0x7c
    Vector m_vAverage;                      // +0x80
    Vector m_vDiffuse;                      // +0x8c
    Vector m_vAmbient;                      // +0x98
    int m_na4;                              // +0xa4
    char m_szTexture0[32];                  // +0xa8
    char m_szTexture1[32];                  // +0xc8
    CExoArrayList<char*> m_lVerts;          // +0xe8
    CExoArrayList<int> m_lTVerts0;          // +0xf4
    CExoArrayList<int> m_lTVerts1;          // +0x100
    CExoArrayList<int> m_lTVerts2;          // +0x10c
    CExoArrayList<int> m_lTextures;         // +0x118
    char pad124[0x128 - 0x124];
    int m_nTexture;                         // +0x128
    int m_n12c;                             // +0x12c
    char pad130[0x134 - 0x130];
    MdlNodeTriMesh_PostProcess* m_pPostProcess; // +0x134
    int m_n138;                             // +0x138
    char pad13c[0x14c - 0x13c];
    int m_n14c;                             // +0x14c
    int m_n150;                             // +0x150
    int m_a154[11];                         // +0x154
    short m_n180;                           // +0x180
    short m_n182;                           // +0x182
    char m_b184;                            // +0x184
    char m_b185;                            // +0x185
    char m_b186;                            // +0x186
    char m_bRender;                         // +0x187
    char m_b188;                            // +0x188
    char m_bShadow;                         // +0x189
    char pad18a[0x18c - 0x18a];
    int m_n18c;                             // +0x18c
    int m_n190;                             // +0x190
    int m_n194;                             // +0x194
    void* m_p198;                           // +0x198

    MdlNodeTriMesh(char* szName);           // 0x0044a640
    ~MdlNodeTriMesh();                      // 0x00448bf0
};

// Axis-aligned bounding box tree node; deleting a child goes through the
// out-of-line scalar deleting destructor (0x0043e2a0).
class AABB {
public:
    char pad00[0x18];
    AABB* m_pLeft;              // +0x18
    AABB* m_pRight;             // +0x1c

    ~AABB()
    {
        if (m_pLeft)
            delete m_pLeft;
        if (m_pRight)
            delete m_pRight;
    }
};

class MdlNodeAABB : public MdlNodeTriMesh {
public:
    AABB* m_pTree;              // +0x19c

    MdlNodeAABB(char* szName);              // 0x0044a8b0
    ~MdlNodeAABB();                         // 0x00449170
};

class MdlNodeAnimMesh : public MdlNodeTriMesh {
public:
    float m_fSampleRate;                    // +0x19c
    CExoArrayList<Vector> m_lAnimVerts;     // +0x1a0
    CExoArrayList<Vector> m_lAnimTVerts;    // +0x1ac
    CExoArrayList<Vector> m_lAnimNormals;   // +0x1b8
    void* m_p1c4;                           // +0x1c4
    void* m_p1c8;                           // +0x1c8
    int m_n1cc;                             // +0x1cc
    int m_n1d0;                             // +0x1d0

    MdlNodeAnimMesh(char* szName);          // 0x0044abc0
    ~MdlNodeAnimMesh();                     // 0x00448da0
};

class MdlNodeSkin : public MdlNodeTriMesh {
public:
    CExoArrayList<int> m_lWeights;          // +0x19c
    int m_n1a8;                             // +0x1a8
    int m_n1ac;                             // +0x1ac
    void* m_p1b0;                           // +0x1b0
    int m_n1b4;                             // +0x1b4
    CExoArrayList<int> m_lBoneMap;          // +0x1b8
    CExoArrayList<int> m_lQBones;           // +0x1c4
    CExoArrayList<int> m_lTBones;           // +0x1d0
    char pad1dc[0x200 - 0x1dc];

    MdlNodeSkin(char* szName);              // 0x0044ac70
    ~MdlNodeSkin();                         // 0x00448e60
    void InternalPostProcess();             // 0x0043b350
    void BuildBoneData();                   // 0x0043a120
};

class MdlNodeDanglyMesh : public MdlNodeTriMesh {
public:
    CExoArrayList<float> m_lConstraints;    // +0x19c
    char pad1a8[0x1b4 - 0x1a8];
    void* m_p1b4;                           // +0x1b4

    MdlNodeDanglyMesh(char* szName);        // 0x0044ae00
    ~MdlNodeDanglyMesh();                   // 0x004490f0
};

class MdlNodeLightsaber : public MdlNodeTriMesh {
public:
    char pad19c[0x1a8 - 0x19c];
    int m_nVertexBuffer;                    // +0x1a8
    int m_nIndexBuffer;                     // +0x1ac

    MdlNodeLightsaber(char* szName);        // 0x0044ad80
    ~MdlNodeLightsaber();                   // 0x00449090
    void InternalDestructor();              // 0x0043fd10
};

// Checked downcasts on the exact type flags.
#define G_MDL_AS(T, M) \
    inline T* MdlNode::As##T() \
    { \
        if ((m_nType & M) == M) \
            return (T*)this; \
        return 0; \
    }
G_MDL_AS(MdlNodeLight, 0x003)
G_MDL_AS(MdlNodeEmitter, 0x005)
G_MDL_AS(MdlNodeCamera, 0x009)
G_MDL_AS(MdlNodeReference, 0x011)
G_MDL_AS(MdlNodeTriMesh, 0x021)
G_MDL_AS(MdlNodeSkin, 0x061)
G_MDL_AS(MdlNodeAnimMesh, 0x0a1)
G_MDL_AS(MdlNodeDanglyMesh, 0x121)
G_MDL_AS(MdlNodeAABB, 0x221)
G_MDL_AS(MdlNodeTrigger, 0x401)
G_MDL_AS(MdlNodeLightsaber, 0x821)
#undef G_MDL_AS

// Deletes a node through its most-derived destructor (the type tests run
// from the most specific flag sets down to the plain node).
__forceinline void MdlNodeDelete(MdlNode* p)
{
    if (p->AsMdlNodeLight())
        delete p->AsMdlNodeLight();
    else if (p->AsMdlNodeCamera())
        delete p->AsMdlNodeCamera();
    else if (p->AsMdlNodeDanglyMesh())
        delete p->AsMdlNodeDanglyMesh();
    else if (p->AsMdlNodeAnimMesh())
        delete p->AsMdlNodeAnimMesh();
    else if (p->AsMdlNodeEmitter())
        delete p->AsMdlNodeEmitter();
    else if (p->AsMdlNodeReference())
        delete p->AsMdlNodeReference();
    else if (p->AsMdlNodeAABB())
        delete p->AsMdlNodeAABB();
    else if (p->AsMdlNodeSkin())
        delete p->AsMdlNodeSkin();
    else if (p->AsMdlNodeTrigger())
        delete p->AsMdlNodeTrigger();
    else if (p->AsMdlNodeLightsaber())
        delete p->AsMdlNodeLightsaber();
    else if (p->AsMdlNodeTriMesh())
        delete p->AsMdlNodeTriMesh();
    else
        delete p;
}

// ---------------------------------------------------------------- MaxTree
extern "C" char* __cdecl strcpy(char* d, const char* s);
#pragma intrinsic(strcpy)

class MaxTree;
typedef void (*MaxTreeFn)(MaxTree*);

class MaxTree {
public:
    MaxTreeFn m_pfnDestroy;                 // +0x00
    MaxTreeFn m_pfn04;                      // +0x04
    char m_szName[32];                      // +0x08
    MdlNode* m_pRoot;                       // +0x28
    int m_n2c;                              // +0x2c
    CExoArrayList<MdlNode*> m_lFreeList;    // +0x30
    CExoArrayList<int> m_lTextures;         // +0x3c
    int m_n48;                              // +0x48
    unsigned char m_nType;                  // +0x4c
    char pad4d[0x50 - 0x4d];

    MaxTree(char* szName);                  // 0x0044a900
    ~MaxTree() {}                           // 0x0043f730
    void FreeTheFreeList();                 // 0x0044a9c0
    Model* AsModel();                       // 0x0043e1c0
    Animation* AsAnimation();               // 0x0043e1d0
    static void operator delete(void* p);   // 0x0044aec0
};

class Animation;
class Model;

class Model : public MaxTree {
public:
    short m_n50;                            // +0x50
    char m_b52;                             // +0x52
    char m_b53;                             // +0x53
    int m_n54;                              // +0x54
    CExoArrayList<Animation*> m_lAnimations; // +0x58
    Model* m_pSuperModel;                   // +0x64
    Vector m_vBBoxMin;                      // +0x68
    Vector m_vBBoxMax;                      // +0x74
    float m_fRadius;                        // +0x80
    float m_fScale;                         // +0x84
    char m_b88;                             // +0x88
    char pad89[0xac - 0x89];
    int m_nTexture;                         // +0xac
    int m_nb0;                              // +0xb0
    void* m_pFont;                          // +0xb4
    CExoArrayList<char*> m_lStrings;        // +0xb8

    Model(char* szName);                    // 0x0044aa70
    ~Model();                               // 0x0043f790
};

class Animation : public MaxTree {
public:
    char pad50[0x84 - 0x50];
    Model* m_pModel;                        // +0x84
};

#endif
