// model / animation / controller classes shared by
// matching/src/e/newcontroller.cpp, anim.cpp and skin.cpp.
#ifndef MATCHING_E_MDLANIM_H
#define MATCHING_E_MDLANIM_H

#include <string.h>
#include <stdio.h>

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList() { if (element) delete[] element; }
    void Allocate(int n);           // 0x005E03C0 for pointer lists
    void AllocateOutOfLine(int n);  // the non-inlined instances (0x0043F6B0, 0x00488160)
    void Remove(T value) throw();   // 0x00445080
    int Contains(T value);          // 0x004AF240
    void Insert(T value, int nPos);  // 0x00445020
    T& operator[](int i) { return element[i]; }
    void DelIndex(int i)
    {
        num--;
        for (; i < num; i++)
            element[i] = element[i + 1];
    }
    void Add(T value)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = value;
    }
    CExoArrayList<T>& operator=(const CExoArrayList<T>& o);
};

// Out of line everywhere except MdlNodeSkin::InternalGenVertices' TU, which
// defines E_INLINE_ALLOCATE before including this header.
template <class T>
#ifdef E_INLINE_ALLOCATE
inline
#endif
void CExoArrayList<T>::Allocate(int n)
{
    T* old = element;
    array_size = n;
    element = new T[n];
    for (int i = 0; i < num; i++)
        element[i] = old[i];
    if (old)
        delete[] old;
}

template <class T>
CExoArrayList<T>& CExoArrayList<T>::operator=(const CExoArrayList<T>& o)
{
    num = 0;
    for (int i = 0; i < o.num; i++)
        Add(o.element[i]);
    return *this;
}

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector(const Vector& v) { x = v.x; y = v.y; z = v.z; }
    Vector& operator=(const Vector& v) { x = v.x; y = v.y; z = v.z; return *this; }
    Vector operator+(const Vector& v) const { return Vector(x + v.x, y + v.y, z + v.z); }
    Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }
    Vector operator*(float f) const { return Vector(x * f, y * f, z * f); }
    void operator+=(const Vector& v);           // 0x004A9740
};

// Plain RGB triple (implicit copy: the by-value pass is a block copy).
struct AurColor {
    float r, g, b;
};

class Quaternion {
public:
    float w, x, y, z;

    Quaternion() { z = 0.0f; y = 0.0f; x = 0.0f; w = 1.0f; }
};

Vector Lerp(const Vector& a, const Vector& b, float t);         // 0x0046EEA0
Vector BezierInterpolate(const Vector& p0, const Vector& p1,
                         const Vector& c0, const Vector& c1, float t);  // 0x004AC110
float BezierInterpolate(float p0, float p1, float c0, float c1, float t);  // 0x004AAAE0
Quaternion* Interpolate(Quaternion* pOut, const Quaternion* pA, const Quaternion* pB, float t);  // 0x00481AA0
Quaternion Slerp(Quaternion a, const Quaternion& b, float t);   // 0x004A9E00

class Matrix {
public:
    float m[9];
    Matrix() throw();                           // 0x00437AF0
};

class SkinVertexWeight {
public:
    char pad_0x0[0x34];
};

class Part;
class MdlNodeInstance;

class NodeVertex {
public:
    NodeVertex(Vector* pPos, Vector* pTV0, Vector* pTV1, Vector* pTV2, Vector* pTV3,
               Vector* pNormal, int n);     // 0x00478680
    unsigned char operator==(NodeVertex& o);    // 0x00478820
    char pad_0x0[0x58];
};

// Per-face vertex index record of the trimesh build data (0x14 bytes).
struct MdlFaceIndices {
    int* pPos;          // +0x00
    int* pTex;          // +0x04 [layer * 3 + corner]
    int* p08;
    int* p0c;
    int* pNormal;       // +0x10
};

class MdlNodeTriMeshData {
public:
    CExoArrayList<Vector> m_lVerts;     // +0x00
    CExoArrayList<Vector> m_lTVerts0;   // +0x0c
    CExoArrayList<Vector> m_lTVerts1;   // +0x18
    CExoArrayList<Vector> m_lTVerts2;   // +0x24
    CExoArrayList<Vector> m_lTVerts3;   // +0x30
    char pad_0x3c[0xc];
    CExoArrayList<MdlFaceIndices> m_lFaceIndices;   // +0x48
    CExoArrayList<Vector> m_lNormals;   // +0x54
    CExoArrayList<NodeVertex*> m_lNodeVerts;    // +0x60
};

struct MdlFace {
    Vector normal;          // +0x00
    float fDistance;        // +0x0c
    int nSurface;           // +0x10
    short adj[3];           // +0x14
    short verts[3];         // +0x1a
};

class MdlNode;
class Gob;

// Event data handed to Gob::DoEvent("ReportKey", ...).
struct ReportKeyEvent {
    float fTime;            // +0x00
    Vector* pPosition;      // +0x04
    Quaternion* pOrientation;   // +0x08
    unsigned char bPosition;    // +0x0c

    ReportKeyEvent() { pPosition = 0; pOrientation = 0; bPosition = 1; }
};

#define VSLOTS4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define VSLOTS16(n) VSLOTS4(n##a) VSLOTS4(n##b) VSLOTS4(n##c) VSLOTS4(n##d)

class Model;
class AnimRun;
class LookAtAnimation;

typedef void (*GobCallback)(Gob* pGob, char* szEvent, void* pData);

struct GobEventHandler {
    char szEvent[0x20];                                 // +0x00
    void (*pfnCallback)(Gob* pGob, char* szEvent, void* pData);  // +0x20
    void* pDefaultData;                                 // +0x24
    float fTime;                                        // +0x28
    unsigned char bAlways;                              // +0x2c

    GobEventHandler(GobCallback pfn, char* szName, void* pData, float fTime, unsigned char bAlways) throw();
};

class Scene;
class CAurBehavior;
class CAurBehaviorAttach;

// Attachment record behind Gob +0x188.
class GobAttachment {
public:
    virtual void v00(); virtual void v04();
    virtual void Render(int nFlags);                    // +0x08
    virtual void v0c(); virtual void v10();
    virtual CAurBehaviorAttach* GetAttachBehavior();   // +0x14
};

class Gob {
public:
    virtual ~Gob();
    virtual void g0a1(); virtual void g0a2(); virtual void g0a3();
    virtual void Render(int nFlags);                    // +0x10
    virtual void Control(float fDelta);                 // +0x14
    virtual int PlayAnimation(char* szName, float fSpeed, int nFlags, float fStart);   // +0x18
    virtual AnimRun* FindRun(char* szName, float* pTime, float* pLength);    // +0x1c
    virtual void g0c0(); virtual void g0c1();
    virtual int AddCallback(GobCallback pfn, char* szEvent, void* pData, float fTime, int bAlways);  // +0x28
    virtual void RemoveCallback(int nId);               // +0x2c
    VSLOTS4(g0d)
    VSLOTS4(g1a)
    virtual void g1b0(); virtual void g1b1();
    virtual void Detach(int nFlags);                    // +0x58
    virtual Vector SetPosition(Vector vPosition);       // +0x5c
    virtual Quaternion SetOrientation(Quaternion qOrientation);   // +0x60
    virtual Vector GetPosition();                       // +0x64
    virtual Quaternion GetOrientation();                // +0x68
    virtual void g1c3();
    VSLOTS4(g1d)
    VSLOTS4(g2a) VSLOTS4(g2b) VSLOTS4(g2c)
    virtual void g2d0();
    virtual void SetColorShift(AurColor cColor, int nMode, int bPropagate);  // +0xb4
    virtual void SetIllumination(AurColor cColor, int bPropagate);           // +0xb8
    virtual void g2d3();
    virtual void g3a0(); virtual void g3a1(); virtual void g3a2();
    virtual void SetScale(float fScale, int nFlags);    // +0xcc
    VSLOTS4(g3b) VSLOTS4(g3c) VSLOTS4(g3d)
    virtual void g4a0(); virtual void g4a1();
    virtual CAurBehavior* GetBehavior(int nIndex);      // +0x108
    virtual void g4a3();
    virtual void g4b0(); virtual void g4b1();
    virtual void SetLookAt(Gob* pTarget, char* szTargetPart, char* szPart, float fYaw, float fPitch,
                           float fX, float fY);        // +0x118
    virtual void StartLookAt();                         // +0x11c
    virtual void PlayOverlayAnimation(char* szName, char** pNames, float* pTimes, int nCount, int nFlags);  // +0x120
    virtual void ReplayOverlay(char* szName);           // +0x124
    virtual void AddLastRunFlags(int nFlags);           // +0x128
    virtual void RemoveLastRunFlags(int nFlags);        // +0x12c
    VSLOTS4(g4d)
    VSLOTS4(g5a) VSLOTS4(g5b)
    virtual void v160(); virtual void v164(); virtual void v168();
    virtual void DoEvent(char* szEvent, void* pData);  // +0x16c
    virtual void v170(); virtual void v174();
    virtual void UpdateViewVolume();                   // +0x178
    virtual void v17c();
    virtual unsigned char CanHandleEvents();           // +0x180

    CExoArrayList<Gob**> m_lWatchers;       // +0x04
    char pad_0x10[0x1c];
    float m_fTime;                          // +0x2c
    char pad_0x30[0x28];
    Model* m_pModel;                        // +0x58
    char pad_0x5c[8];
    Model* m_pAnimModel;                    // +0x64
    Scene* m_pScene;                        // +0x68
    Part* m_pRootPart;                      // +0x6c
    char pad_0x70[8];
    Vector m_vPosition;                     // +0x78
    Quaternion m_qOrientation;              // +0x84
    char pad_0x94[0x1c];
    CExoArrayList<AnimRun*> m_lRuns;        // +0xb0
    char pad_0xbc[0x10];
    AurColor m_cColorShift;                 // +0xcc
    int m_nColorShiftMode;                  // +0xd8
    char pad_0xdc[4];
    AurColor m_cIllumination;               // +0xe0
    char pad_0xec[0x88];
    LookAtAnimation* m_pLookAt;             // +0x174
    char pad_0x178[0x10];
    GobAttachment* m_pAttachment;           // +0x188
    char pad_0x18c[4];
    CExoArrayList<GobEventHandler*> m_lHandlers;    // +0x190
    char pad_0x19c[0x18];
    float m_fScale;                         // +0x1b4
    float m_fBaseScale;                     // +0x1b8

    void SetScene(Scene* pScene);           // 0x0043F200
    void AttachChild(Gob* pChild);          // 0x00447310
    void DetachChild(Gob* pChild);          // 0x00447370
    unsigned char GetModelFlag2();          // 0x00481D60
    unsigned char GetModelFlag4();          // 0x00481D80
};

// Clears itself when the watched Gob dies (the Gob nulls every registered slot).
class GobWatch {
public:
    Gob* m_pGob;
    GobWatch(Gob* pGob) { m_pGob = pGob; pGob->m_lWatchers.Add(&m_pGob); }
    ~GobWatch() { if (m_pGob) m_pGob->m_lWatchers.Remove(&m_pGob); }
};

class Part {
public:
    virtual void v00();
    virtual char* GetName();                // +0x04
    virtual void v08(); virtual void v0c();
    VSLOTS4(p1)
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual void UpdateTransform();         // +0x2c
    virtual Part* GetRoot();                // +0x30
    virtual class PartEmitter* AsEmitter(); // +0x34
    virtual Part* AsReference();            // +0x38
    virtual void p3c();
    VSLOTS4(p4) VSLOTS4(p5)
    virtual void p60(); virtual void p64();
    virtual void SetChildState(int nState);  // +0x68
    char* m_pBase;                  // +0x04
    Vector m_vPosition;             // +0x08
    Quaternion m_qOrientation;      // +0x14
    char pad_0x24[4];
    unsigned short m_nNodeId;       // +0x28
    char pad_0x2a[6];
    CExoArrayList<Part*> m_lChildren;   // +0x30
    Part* m_pParent;                // +0x3c
    Gob* m_pGob;                    // +0x40
};

// Controller header (0x10 bytes) stored in MdlNode::m_lControllers.
class NewController {
public:
    int m_nType;                    // +0x00: offset of the controlled value in the part
    short m_nAddOffset;             // +0x04: -1, or offset of an additive base value
    unsigned short m_nNumKeys;      // +0x06
    unsigned short m_nTimeIndex;    // +0x08
    unsigned short m_nDataIndex;    // +0x0a
    unsigned char m_nColumns : 4;   // +0x0c
    unsigned char m_nFlags : 4;     // 1 = bezier
    char pad_0xd[3];

    void GetKeyIndex(float fTime, MdlNode* pNode, int* pIndex, float* pFrac);  // 0x00482B70
    int GetFloatValue(float fTime, MdlNode* pNode, float* pOut);
    int GetVectorValue(float fTime, MdlNode* pNode, Vector* pOut);
    void GetQuaternionFromIndexLoc(int nIndex, MdlNode* pNode, Quaternion* pOut);
    int GetQuaternionValue(float fTime, MdlNode* pNode, Quaternion* pOut);
    void Control(Part* pPart, MdlNode* pNode, float fPrevTime, float fTime, float fBlend);
    void ReportKey(Part* pPart, MdlNode* pNode, int nKey, float fDelta);
    void ReportKeys(Part* pPart, MdlNode* pNode, float fStart, float fEnd);
};

class Animation;

class MdlNode {
public:
    unsigned short m_nType;                     // +0x00
    unsigned short m_nId;                       // +0x02
    char pad_0x4[4];
    Animation* m_pAnimation;                    // +0x08
    MdlNode* m_pParent;                         // +0x0c
    char pad_0x10[0x1c];
    CExoArrayList<MdlNode*> m_lChildren;        // +0x2c
    CExoArrayList<NewController> m_lControllers;    // +0x38
    CExoArrayList<float> m_lControllerData;         // +0x44

    MdlNode(char* szName);                      // 0x00445210
    void InternalControl(Part* pPart, float fPrevTime, float fTime, float fBlend);
};

class MdlNodeTriMesh : public MdlNode {
public:
    char pad_0x50[8];
    CExoArrayList<MdlFace> m_lFaces;            // +0x58
    char pad_0x64[0x134 - 0x64];
    MdlNodeTriMeshData* m_pData;                // +0x134
    char pad_0x138[0x180 - 0x138];
    short m_nVertexCount;                       // +0x180
    char pad_0x182[0x19c - 0x182];

    void InternalControl(Part* pPart, float fPrevTime, float fTime, float fBlend);
};

class MdlNodeSkin : public MdlNodeTriMesh {
public:
    CExoArrayList<SkinVertexWeight> m_lWeights; // +0x19c

    MdlNodeInstance* InternalCreateInstance(Part* pPart);
    void InternalGenVertices();
};

#endif
