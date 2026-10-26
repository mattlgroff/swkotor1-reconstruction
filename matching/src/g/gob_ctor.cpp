// Gob::Gob (0x00449440): member construction, a unique instance name and
// registration in the global Gob list. Layout from gob.h / gob_dtor.cpp.
typedef unsigned int size_t;

extern "C" int __cdecl sprintf(char* s, const char* fmt, ...);
extern "C" int __cdecl _stricmp(const char* a, const char* b);
extern "C" char* __cdecl strcpy(char* d, const char* s);
#pragma intrinsic(strcpy)

void* __cdecl operator new(size_t n);
void __cdecl operator delete(void* p);
void* __cdecl operator new[](size_t n);
void __cdecl operator delete[](void* p);

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList()
    {
        if (element)
            delete[] element;
    }
    T& operator[](int i) { return element[i]; }
    void Insert(T t, int nIndex);               // 0x00445020
    void Remove(T t);                           // 0x00445080
    void Add(T t);
};

template <class T>
class SafePointer {
public:
    T* m_p;

    SafePointer() { m_p = 0; }
    ~SafePointer();
    SafePointer<T>& operator=(T* p)
    {
        if (m_p)
            m_p->RemoveRef(this);
        m_p = p;
        if (p)
            p->AddRef(this);
        return *this;
    }
};

template <class T>
class SafePointerTarget {
public:
    CExoArrayList<SafePointer<T>*> m_lRefs;

    void AddRef(SafePointer<T>* p) { m_lRefs.Add(p); }
    void RemoveRef(SafePointer<T>* p) { m_lRefs.Remove(p); }
};

class CAurTexture : public SafePointerTarget<CAurTexture> {
public:
    virtual ~CAurTexture();
};

class CAurObject : public SafePointerTarget<CAurObject> {
public:
    virtual ~CAurObject();
};

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
};

class AurColor {
public:
    float r, g, b;

    AurColor() { r = 0; g = 0; b = 0; }
    AurColor(float fR, float fG, float fB) { r = fR; g = fG; b = fB; }
};

// vtable 0x00740a94
class CAurMeshBuffer {
public:
    CAurMeshBuffer();                           // 0x0044ee00
    virtual ~CAurMeshBuffer();
    int m_n04;
    int m_n08;
};

// The sized list constructor with a dead `new` (leaves the owner's
// empty-state SEH store; see mdl_ctor.h).
template <class T>
class CExoSizedList : public CExoArrayList<T> {
public:
    CExoSizedList(int nSize = 0)
    {
        this->num = 0;
        this->array_size = nSize;
        this->element = 0;
        if (this->array_size)
            this->element = new T[this->array_size];
    }
};

// vtable 0x00740cdc, 0x24 bytes.
class CAurObjectLightContainer {
public:
    CAurObjectLightContainer() { m_n1c = 0; m_n20 = 0; }
    virtual ~CAurObjectLightContainer();

    CExoSizedList<void*> m_l04;                 // +0x04
    CExoSizedList<void*> m_l10;                 // +0x10
    int m_n1c;                                  // +0x1c
    int m_n20;                                  // +0x20
};

class Model;
class Part;
class Scene;
class AnimRun;
class GobAttachment;
class GobEventHandler;

class Gob;

Scene* __cdecl FindScene(char* szName);         // 0x0044fd60
int __cdecl GobListFindIndex(char* szName);     // 0x00440410

extern CExoArrayList<Gob*> g_lGobs;             // 0x007a8a58
extern int g_nGobsCreated;                      // 0x007a8a1c
extern int g_nGobCount;                         // 0x007a8a18

class Gob : public CAurObject {
public:
    Gob(char* szName);
    virtual ~Gob();

    unsigned char m_b10;                        // +0x10
    unsigned char m_b11;                        // +0x11
    unsigned char m_b12;                        // +0x12
    unsigned char m_b13;                        // +0x13
    unsigned char m_b14;                        // +0x14
    unsigned char m_bRenderBumped;              // +0x15
    unsigned char m_b16;                        // +0x16
    unsigned char m_b17;                        // +0x17
    unsigned char m_b18;                        // +0x18
    char pad19[0x1c - 0x19];
    int m_n1c;                                  // +0x1c
    CExoArrayList<int> m_l20;                   // +0x20
    int m_n2c;                                  // +0x2c
    int m_n30;                                  // +0x30
    int m_n34;                                  // +0x34
    char m_szName[0x20];                        // +0x38
    Model* m_pModel;                            // +0x58
    Model* m_pModelX;                           // +0x5c
    Model* m_pModelZ;                           // +0x60
    Model* m_pAnimModel;                        // +0x64
    Scene* m_pScene;                            // +0x68
    Part* m_pRoot;                              // +0x6c
    Part* m_pRootX;                             // +0x70
    Part* m_pRootZ;                             // +0x74
    Vector m_vPosition;                         // +0x78
    Quaternion m_qOrientation;                  // +0x84
    Vector m_vVelocity;                         // +0x94
    Quaternion m_qSpin;                         // +0xa0
    CExoArrayList<AnimRun*> m_lRuns;            // +0xb0
    int m_nbc;                                  // +0xbc
    unsigned char m_bCanDownSample;             // +0xc0
    char padc1;
    short m_nFade;                              // +0xc2
    short m_bColorShift;                        // +0xc4
    short m_bIllumination;                      // +0xc6
    short m_bSelfIllum;                         // +0xc8
    char padca[0xcc - 0xca];
    AurColor m_cColorShift;                     // +0xcc
    float m_fAlphaD8;                           // +0xd8
    float m_fAlphaDC;                           // +0xdc
    AurColor m_cIllumination;                   // +0xe0
    CExoArrayList<Model*> m_lAttachedModels;    // +0xec
    CExoArrayList<Part*> m_lAttachedParts;      // +0xf8
    int m_n104;                                 // +0x104
    int m_n108;                                 // +0x108
    int m_n10c;                                 // +0x10c
    CAurMeshBuffer m_MeshBuffer;                // +0x110
    int m_n11c;                                 // +0x11c
    Vector m_vBoxMin;                           // +0x120
    Vector m_vBoxMax;                           // +0x12c
    float m_fRadius;                            // +0x138
    Vector m_vCenter;                           // +0x13c
    Vector m_vSelfIllum;                        // +0x148
    unsigned char m_b154;                       // +0x154
    unsigned char m_b155;                       // +0x155
    unsigned char m_bFog;                       // +0x156
    char pad157;
    int m_nFog158;                              // +0x158
    int m_nFog15c;                              // +0x15c
    int m_nFog160;                              // +0x160
    unsigned char m_b164;                       // +0x164
    unsigned char m_b165;                       // +0x165
    unsigned char m_b166;                       // +0x166
    unsigned char m_bDirty;                     // +0x167
    int m_n168;                                 // +0x168
    int m_n16c;                                 // +0x16c
    void* m_pTileData;                          // +0x170
    void* m_p174;                               // +0x174
    CAurObjectLightContainer* m_pLights;        // +0x178
    unsigned char m_bHidden;                    // +0x17c
    unsigned char m_bNoClip;                    // +0x17d
    unsigned char m_b17e;                       // +0x17e
    unsigned char m_b17f;                       // +0x17f
    float m_f180;                               // +0x180
    unsigned char m_b184;                       // +0x184
    char pad185[0x188 - 0x185];
    GobAttachment* m_pAttachment;               // +0x188
    int m_n18c;                                 // +0x18c
    CExoArrayList<GobEventHandler*> m_lHandlers;    // +0x190
    CExoArrayList<int> m_l19c;                  // +0x19c
    SafePointer<CAurTexture> m_pEnvironment;    // +0x1a8
    int m_nUserData;                            // +0x1ac
    int m_nUserData2;                           // +0x1b0
    float m_fScale;                             // +0x1b4
    float m_fBaseScale;                         // +0x1b8
    CExoArrayList<Gob*> m_lAttachments;         // +0x1bc
    int m_n1c8;                                 // +0x1c8
};

// An existing Gob with this name, if any.
inline Gob* GobListFind(char* szName)
{
    int i = GobListFindIndex(szName);
    if (i < g_lGobs.num && !_stricmp(g_lGobs[i]->m_szName, szName))
        return g_lGobs[i];
    return 0;
}

// FUNCTION: SWKOTOR 0x00449440
Gob::Gob(char* szName)
{
    m_b17 = 1;
    m_b184 = 0;
    g_nGobsCreated++;
    m_b154 = 0;
    m_b18 = 1;
    m_n1c = 0;
    g_nGobCount++;
    m_szName[0] = 0;
    if (szName) {
        char szBuffer[64];
        sprintf(szBuffer, "%s", szName);
        int n = 0;
        while (FindScene(szBuffer) || GobListFind(szBuffer)) {
            n++;
            sprintf(szBuffer, "%s%d", szName, n);
        }
        strcpy(m_szName, szBuffer);
    }
    m_n2c = 0;
    m_pRoot = 0;
    m_pRootX = 0;
    m_pRootZ = 0;
    m_pScene = 0;
    m_pAttachment = 0;
    m_pEnvironment = 0;
    m_cColorShift = AurColor(1.0f, 1.0f, 1.0f);
    m_vBoxMin = Vector(0, 0, 0);
    m_vBoxMax = Vector(1.0f, 1.0f, 1.0f);
    m_vCenter = Vector(0, 0, 0);
    m_nbc = 0;
    m_bCanDownSample = 1;
    m_n18c = -1;
    m_pModel = 0;
    m_pModelX = 0;
    m_pModelZ = 0;
    m_pAnimModel = 0;
    m_bColorShift = 0;
    m_bIllumination = 0;
    m_fAlphaD8 = 1.0f;
    m_fAlphaDC = 1.0f;
    m_nFade = 1;
    m_bSelfIllum = 0;
    m_n104 = 0;
    m_b155 = 0;
    m_f180 = 1.5f;
    m_n11c = 0;
    m_n108 = 0;
    m_n10c = 0;
    m_b164 = 0;
    m_b165 = 0;
    m_b166 = 0;
    m_bHidden = 0;
    m_n168 = 0;
    m_n16c = 0;
    m_bRenderBumped = 0;
    m_b17f = 0;
    m_bNoClip = 0;
    m_bFog = 0;
    m_nFog158 = 0;
    m_nFog15c = 0;
    m_nFog160 = 0;
    m_fRadius = 1.0f;
    m_vSelfIllum = Vector(0, 0, 1.0f);
    m_nUserData = 0;
    m_pTileData = 0;
    m_nUserData2 = 0;
    m_bDirty = 0;
    m_b10 = 0;
    m_b11 = 1;
    m_b17e = 0;
    m_p174 = 0;
    m_n30 = 0;
    m_fScale = 1.0f;
    m_fBaseScale = 1.0f;
    m_n34 = 0;
    m_b12 = 0;
    m_b14 = 0;
    m_b16 = 0;
    m_pLights = new CAurObjectLightContainer;
    m_n1c8 = 1;
    g_lGobs.Insert(this, GobListFindIndex(m_szName));
}
