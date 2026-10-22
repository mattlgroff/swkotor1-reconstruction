// NodeVertex (model vertices) and the VertexPrimitive hierarchy
// (per-mesh vertex arrays), 0x00478650-0x004794C0.
#ifndef MATCHING_H_VERTEX_H
#define MATCHING_H_VERTEX_H

class Vector {
public:
    float x, y, z;

    Vector() { x = 0; y = 0; z = 0; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

int operator==(const Vector& a, const Vector& b);      // 0x004AA980

struct Vector2 {
    float u, v;
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList() { if (element) delete[] element; }
    void Allocate(int n);               // 0x00421310 (Vector)
    void Add(T value)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = value;
    }
    CExoArrayList<T>& operator=(const CExoArrayList<T>& o)
    {
        num = 0;
        for (int i = 0; i < o.num; i++)
            Add(o.element[i]);
        return *this;
    }
};

class NodeVertexAnim;
class NodeVertexDangly;

extern int g_nNodeVertexBytes;          // 0x00830238

// 0x56 bytes (0x58 with padding).
class NodeVertex {
public:
    NodeVertex(Vector* pPosition, Vector* pNormal, Vector* pV18, Vector* pV24,
               Vector* pV30, Vector* pV3C, Vector* pColor);
    bool operator==(NodeVertex& o);
    NodeVertexAnim* AsAnim();
    NodeVertexDangly* AsDangly();

    Vector m_vPosition;         // +0x00
    Vector m_vNormal;           // +0x0c
    Vector m_v18;               // +0x18
    Vector m_v24;               // +0x24
    Vector m_v30;               // +0x30
    Vector m_v3C;               // +0x3c
    Vector m_vColor;            // +0x48
    unsigned char m_nFlags;     // +0x54
    unsigned char m_nType;      // +0x55 (1 anim, 2 dangly)
};

class NodeVertexDangly : public NodeVertex {
public:
    NodeVertexDangly(Vector* pPosition, Vector* pNormal, Vector* pV18,
                     Vector* pV24, Vector* pV30, Vector* pV3C, Vector* pColor,
                     float fDisplacement);
    bool operator==(NodeVertexDangly& o);

    float m_fDisplacement;      // +0x58
};

class NodeVertexAnim : public NodeVertex {
public:
    NodeVertexAnim(Vector* pPosition, Vector* pNormal, Vector* pV18,
                   Vector* pV24, Vector* pV30, Vector* pV3C, Vector* pColor,
                   CExoArrayList<Vector>* pA, CExoArrayList<Vector>* pB,
                   CExoArrayList<Vector>* pC);

    CExoArrayList<Vector> m_l58;    // +0x58
    CExoArrayList<Vector> m_l64;    // +0x64
    CExoArrayList<Vector> m_l70;    // +0x70
};

// ---------------------------------------------------------------------------

class Material {
public:
    bool HasBumpMap();              // 0x0047B8F0
};

class MdlNodeTriMeshData {
public:
    char pad00[0xac];
    int m_hVertexBuffer;            // +0xac
};

class MdlNodeTriMeshOwner {
public:
    MdlNodeTriMeshData* GetData();  // 0x0043E1C0
};

class MdlNodeTriMesh {
public:
    bool HasBumpmap();              // 0x0043E410
    int GetVertexBuffer() { return m_pOwner->GetData()->m_hVertexBuffer; }

    char pad00[8];
    MdlNodeTriMeshOwner* m_pOwner;  // +0x08
    char pad0c[0x138 - 0x0c];
    int m_n138;                     // +0x138
    char pad13c[0x14c - 0x13c];
    int m_nVertexStride;            // +0x14c
    int m_n150;                     // +0x150
    char pad154[0x160 - 0x154];
    int m_nUVOffset;                // +0x160
    char pad164[0x180 - 0x164];
    short m_nNumVertices;           // +0x180
    short m_n182;                   // +0x182
    char pad184[0x194 - 0x184];
    int m_n194;                     // +0x194
};

class PartTriMesh {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual PartTriMesh* GetSource();   // +0x2c

    MdlNodeTriMesh* m_pNode;        // +0x04
    char pad08[0x44 - 0x08];
    Material* m_pMaterial;          // +0x44
    char pad48[0x74 - 0x48];
    int m_h74;                      // +0x74
    int m_hVertexArray;             // +0x78
    unsigned char m_b7c;            // +0x7c
};

// Vertex array range helpers (cdecl).
void AurVARCreate(int a, int nSize, int nType, int b, int* pHandle);    // 0x00423EE0
void* AurVARLock(int hArray, int a, int b, int nMode);                  // 0x00423F10
void AurVARUnlock(int hArray);                                          // 0x00423F30
void AurVARRelease(int hArray);                                         // 0x00423F50
void AurVARBindIndices(int a, int b, int h, int n);                     // 0x00425970
void AurVARBindVertices(int nStride, int a, int h, int n,
                        MdlNodeTriMesh* pNode, int b);                  // 0x00425520
void AurVARBindTexCoords(int nStride, int a, int h, int n);             // 0x00425A40
void AurVARBindNormals(int nStride, int a, int h, int n);               // 0x00425900
int AurBumpProgramCreate();                                             // 0x00425C60
int AurBumpMappingAvailable();                                          // 0x0045FB10
int AurVertexProgramsAvailable();                                       // 0x0045F770

extern int g_bDisableTexCoordArray;     // 0x008305E0
extern unsigned int g_eTexCoordArray;   // 0x0073F3A0
extern int g_bARBVertexProgram;         // 0x0078E5EC
extern int g_bNVVertexProgram;          // 0x0078E8C0
typedef void (__stdcall* PFNVERTEXATTRIBPTR)(unsigned int index, int size,
    unsigned int type, unsigned char normalized, int stride, const void* p);
extern PFNVERTEXATTRIBPTR g_pglVertexAttribPointerARB;  // 0x007BB7D8
typedef void (__stdcall* PFNVERTEXATTRIBPTRNV)(unsigned int index, int size,
    unsigned int type, int stride, const void* p);
extern PFNVERTEXATTRIBPTRNV g_pglVertexAttribPointerNV; // 0x007BB854

extern "C" __declspec(dllimport) void __stdcall glEnableClientState(unsigned int);
extern "C" __declspec(dllimport) void __stdcall glDisableClientState(unsigned int);

class VertexPrimitiveFlat;
class VertexPrimitiveSkinned;
class VertexPrimitiveAnim;
class VertexPrimitiveDangly;

// vtable 0x00742F48.
class VertexPrimitive {
public:
    VertexPrimitive(PartTriMesh* pOwner)
    {
        m_pOwner = pOwner;
        m_nNumArrays = 0;
        m_n10 = 1;
        m_bArraysSetup = 0;
    }
    virtual ~VertexPrimitive() {}                                   // 0x00478670
    virtual VertexPrimitiveFlat* AsFlat() { return 0; }             // +0x04
    virtual VertexPrimitiveSkinned* AsSkinned() { return 0; }       // +0x08
    virtual VertexPrimitiveAnim* AsAnim() { return 0; }             // +0x0c
    virtual VertexPrimitiveDangly* AsDangly() { return 0; }         // +0x10
    virtual void BuildVertexArrays() = 0;                           // +0x14
    virtual void SetupVertexArraysStandard();                       // +0x18
    virtual void SetupVertexArrays() = 0;                           // +0x1c
    virtual int v20(int) = 0;                                       // +0x20
    virtual char* ReconnectArraysVAR(char* p) = 0;                  // +0x24
    virtual void InvalidateArrays() { m_bArraysSetup = 1; }         // +0x28

    MdlNodeTriMesh* m_pNode;        // +0x04
    PartTriMesh* m_pOwner;          // +0x08
    int m_nNumArrays;               // +0x0c
    int m_n10;                      // +0x10
    unsigned char m_bArraysSetup;   // +0x14
};

// vtable 0x00742F74.
class VertexPrimitiveFlat : public VertexPrimitive {
public:
    VertexPrimitiveFlat(PartTriMesh* pOwner);
    virtual ~VertexPrimitiveFlat();
    virtual VertexPrimitiveFlat* AsFlat() { return this; }
    virtual void BuildVertexArrays();
    virtual void SetupVertexArrays();
    virtual int v20(int) { return 0; }
    virtual char* ReconnectArraysVAR(char* p);
    virtual void v2c(int) {}
    virtual void v30(int, int) {}
    virtual void v34(int, int) {}

    int m_hBumpProgram;             // +0x18
};

// vtable 0x00742FAC.
class VertexPrimitiveSkinned : public VertexPrimitiveFlat {
public:
    VertexPrimitiveSkinned(PartTriMesh* pOwner);
    virtual ~VertexPrimitiveSkinned() {}
    virtual VertexPrimitiveSkinned* AsSkinned() { return this; }
    virtual void BuildVertexArrays() {}
    virtual char* ReconnectArraysVAR(char* p);
};

// vtable 0x00742FE4.
class VertexPrimitiveDangly : public VertexPrimitiveFlat {
public:
    VertexPrimitiveDangly(PartTriMesh* pOwner);
    virtual ~VertexPrimitiveDangly();
    virtual VertexPrimitiveDangly* AsDangly() { return this; }
    virtual void BuildVertexArrays() {}

    Vector* m_pVertices;            // +0x1c
};

// vtable 0x0074301C.
class VertexPrimitiveAnim : public VertexPrimitiveFlat {
public:
    VertexPrimitiveAnim(PartTriMesh* pOwner);
    virtual ~VertexPrimitiveAnim();
    virtual VertexPrimitiveAnim* AsAnim() { return this; }
    virtual void BuildVertexArrays() {}

    int m_hNormals;                 // +0x1c
    int m_hTexCoords;               // +0x20
};

#endif
