// Part instance constructors (0x004457a0 Part, 0x00445a40
// PartLight, 0x004478d0 PartLightsaber) and the MdlNodeTriMesh post-process
// block constructor (0x00445720), with the member layouts their stores
// show. Behavioural reference: src/batches/b0044_part_ctors.cpp.
// Self-contained; uses the sized CExoArrayList constructor whose dead `new`
// reproduces the original's empty-state SEH frames.
#ifndef G_PART_CTOR_H
#define G_PART_CTOR_H

typedef unsigned int size_t;

void* __cdecl operator new(size_t n);           // 0x006fa7e6
void __cdecl operator delete(void* p);          // 0x006fa390

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
    int Contains(T t)
    {
        int n = 0;
        for (int i = 0; i < num; i++)
            if (element[i] == t)
                n++;
        return n;
    }
    int IndexOf(T t)
    {
        for (int i = 0; i < num; i++)
            if (element[i] == t)
                return i;
        return -1;
    }
    void DelIndex(int i)
    {
        num--;
        for (; i < num; i++)
            element[i] = element[i + 1];
    }
    void Allocate(int n);
    void SetSize(int n);
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

// One face of the MDL post-process face list (0x14 bytes, dtor 0x0043e0f0).
class MaxFace_PostProcess {
public:
    char pad00[0x14];

    ~MaxFace_PostProcess();
};

class Vector {
public:
    float x, y, z;

    Vector() { x = 0; y = 0; z = 0; }
};

class Quaternion {
public:
    float w, x, y, z;

    Quaternion() { z = 0; y = 0; x = 0; w = 1.0f; }
};

class Material;
class MdlNodeTriMesh;
class MdlNodeDanglyMesh;

extern "C" void* __cdecl memcpy(void* d, const void* s, size_t n);
#pragma intrinsic(memcpy)

void PoolRequest(int nType, int nSize, int nFlags, int nUnused,
                 int* pId);                     // 0x00423ee0
void* PoolLock(int nId, int nUnused1, int nUnused2, int nFlags); // 0x00423f10
void PoolUnlock(int nId);                       // 0x00423f30

class MaxTree {
public:
    char pad00[0x4c];
    unsigned char m_nType;                  // +0x4c (low 7 bits)
    char pad4d[0x53 - 0x4d];
    unsigned char m_bRenderEnvMap;          // +0x53
    char pad54[0xac - 0x54];
    int m_nVertexBuffer;                    // +0xac

    MaxTree* AsModel() { return (m_nType & 0x7f) == 2 ? this : 0; }
};

class MdlNode {
public:
    unsigned short m_nType;                 // +0x00
    char pad02[0x08 - 0x02];
    MaxTree* m_pTree;                       // +0x08

    MdlNodeTriMesh* AsMdlNodeTriMesh()
    {
        if ((m_nType & 0x021) == 0x021)
            return (MdlNodeTriMesh*)this;
        return 0;
    }
    MdlNodeDanglyMesh* AsMdlNodeDanglyMesh()
    {
        short f = m_nType;
        f &= 0x121;
        if (f == 0x121)
            return (MdlNodeDanglyMesh*)this;
        return 0;
    }
};

class MdlNodeTriMesh : public MdlNode {
public:
    char pad0c[0x138 - 0x0c];
    int m_n138;                             // +0x138
    char pad13c[0x14c - 0x13c];
    int m_nVertexStride;                    // +0x14c
    char pad150[0x160 - 0x150];
    int m_nTVertOffset;                     // +0x160
    char pad164[0x180 - 0x164];
    short m_nVerts;                         // +0x180
    char pad182[0x194 - 0x182];
    int m_nVertexOffset;                    // +0x194
};

class MdlNodeDanglyMesh : public MdlNodeTriMesh {
public:
    char pad198[0x1b4 - 0x198];
    Vector* m_pConstraints;                 // +0x1b4
};

extern int g_nPartCount;            // 0x007a8a10

// Per-mesh scratch lists built while post-processing a trimesh (0x6c).
class MdlNodeTriMesh_PostProcess {
public:
    CExoArrayList<int> m_lVerts;            // +0x00
    CExoArrayList<int> m_lTVerts0;          // +0x0c
    CExoArrayList<int> m_lTVerts1;          // +0x18
    CExoArrayList<int> m_lTVerts2;          // +0x24
    CExoArrayList<int> m_lTVerts3;          // +0x30
    CExoArrayList<int> m_lNormals;          // +0x3c
    CExoArrayList<MaxFace_PostProcess> m_lFaces;    // +0x48
    CExoArrayList<int> m_lColors;           // +0x54
    CExoArrayList<char*> m_lStrings;        // +0x60

    MdlNodeTriMesh_PostProcess();           // 0x00445720
    ~MdlNodeTriMesh_PostProcess();          // 0x004453b0
};

class CAurPart {
public:
    virtual ~CAurPart() {}
};

// vtable 0x00740ae8
class Part : public CAurPart {
public:
    MdlNode* m_pNode;                       // +0x04
    Vector m_vPosition;                     // +0x08
    Quaternion m_qOrientation;              // +0x14
    float m_fScale;                         // +0x24
    int m_n28;                              // +0x28
    unsigned char m_b2c;                    // +0x2c
    char pad2d[0x30 - 0x2d];
    CExoArrayList<Part*> m_lChildren;       // +0x30
    int m_n3c;                              // +0x3c
    void* m_p40;                            // +0x40
    Material* m_pMaterial;                  // +0x44
    int m_n48;                              // +0x48

    Part(MdlNode* pNode);                   // 0x004457a0
    virtual ~Part();                        // 0x00440140
};

class CAurObjectLightContainer {
public:
    void* m_pVtbl;                          // +0x00
    CExoArrayList<Part*> m_lLights;         // +0x04
    CExoArrayList<int> m_lPriorities;       // +0x10
    Part* m_pMainLight;                     // +0x1c
    int m_nMainPriority;                    // +0x20
};

class Gob {
public:
    char pad00[0x178];
    CAurObjectLightContainer* m_pLights;    // +0x178
};

extern CExoArrayList<Gob*> g_lGobs;         // 0x007a8a58

// vtable 0x00740c08
class PartLight : public Part {
public:
    Vector m_vColor;                        // +0x4c
    int m_n58;                              // +0x58
    int m_n5c;                              // +0x5c
    float m_f60;                            // +0x60
    int m_n64;                              // +0x64
    float m_f68;                            // +0x68
    float m_f6c;                            // +0x6c
    float m_f70;                            // +0x70
    int m_n74;                              // +0x74
    float m_f78;                            // +0x78
    int m_n7c;                              // +0x7c
    int m_n80;                              // +0x80
    int m_n84;                              // +0x84
    int m_n88;                              // +0x88
    float m_f8c;                            // +0x8c
    int m_n90;                              // +0x90
    Vector m_vDirection;                    // +0x94

    PartLight(MdlNode* pNode);              // 0x00445a40
    virtual ~PartLight();                   // 0x00440320
};

class Material {
public:
    char pad00[0x4c];

    Material();                             // 0x0047b290
};

extern int g_nNextTriMeshId;                // 0x0078e328
extern int g_nPartTriMeshCount;             // 0x007a8a14

struct TriMeshTVert {
    int u;
    int v;
};

// vtable 0x00740b68
class PartTriMesh : public Part {
public:
    Vector m_v4c;                           // +0x4c
    Vector m_v58;                           // +0x58
    Vector m_v64;                           // +0x64
    int m_n70;                              // +0x70
    int m_nTexture0;                        // +0x74
    int m_nTexture1;                        // +0x78
    unsigned char m_b7c;                    // +0x7c
    unsigned char m_bRenderEnvMap;          // +0x7d
    char pad7e[0x80 - 0x7e];
    int m_n80;                              // +0x80
    float m_fAlpha;                         // +0x84
    char pad88[0x8c - 0x88];
    int m_nId;                              // +0x8c
    TriMeshTVert* m_pTVerts;                // +0x90
    int m_n94;                              // +0x94

    PartTriMesh(MdlNode* pNode);            // 0x00445840
    virtual ~PartTriMesh();
};

// vtable 0x00740d78
class PartDanglyMesh : public PartTriMesh {
public:
    Vector m_v98;                           // +0x98
    Quaternion m_qA4;                       // +0xa4
    Vector* m_pConstraints;                 // +0xb4
    int m_nConstraintBuffer;                // +0xb8

    PartDanglyMesh(MdlNode* pNode);         // 0x00447980
    virtual ~PartDanglyMesh();
};

// vtable 0x00740ce0
class PartLightsaber : public PartTriMesh {
public:
    char pad98[0xfc - 0x98];
    Vector m_aVerts[25];                    // +0xfc
    Quaternion m_aOrientations[25];         // +0x228
    short m_n3b8;                           // +0x3b8
    short m_n3ba;                           // +0x3ba
    short m_n3bc;                           // +0x3bc
    char pad3be[0x3c0 - 0x3be];
    int m_n3c0;                             // +0x3c0
    int m_n3c4;                             // +0x3c4
    int m_n3c8;                             // +0x3c8

    PartLightsaber(MdlNode* pNode);         // 0x004478d0
    virtual ~PartLightsaber();
};

#endif
