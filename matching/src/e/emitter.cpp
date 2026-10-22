// PartEmitter / LightningEmitter / PartTrigger
// (0x0048D770, 0x00490700, 0x00494B30, 0x00494D40, 0x0049B230 .. 0x0049DB30).
// Self-contained: the Part / Gob
// shapes here differ from mdlanim.h (virtual dtor at slot 0, zeroing
// Vector default ctor).

#include <string.h>
#include <stdlib.h>
#include <math.h>

extern "C" int __cdecl _stricmp(const char* a, const char* b);

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList() { if (element) delete[] element; }
    void Allocate(int n);           // 0x005E03C0 (pointers), 0x00421310 (Vector)
    T& operator[](int i) { return element[i]; }
    void DelIndex(int i)
    {
        num--;
        for (; i < num; i++)
            element[i] = element[i + 1];
    }
    void Remove(T value)            // 0x00445080
    {
        int i;
        for (i = num - 1; i >= 0; i--)
            if (element[i] == value)
                break;
        DelIndex(i);
    }
    void Append(T value);           // 0x00444FE0 (pointer lists, out of line)
    void RemoveAt(int i);           // 0x006857A0 (pointer lists, out of line)
    void Add(T value)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = value;
    }
    void SetSize(int n)
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
};

class Vector {
public:
    float x, y, z;

    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }
    Vector operator*(float f) const { return Vector(x * f, y * f, z * f); }
    float operator*(const Vector& v) const { return x * v.x + y * v.y + z * v.z; }
};

class Quaternion {
public:
    float w, x, y, z;

    Quaternion() { z = 0.0f; y = 0.0f; x = 0.0f; w = 1.0f; }
    Quaternion(Vector vAxis, float fAngle);         // 0x004AC960

    Vector GetYAxis() const;                        // 0x0044F420 (out of line)
    // Rotated +Y axis (inline copy of GetYAxis).
    Vector YAxis() const
    {
        return Vector(2.0f * (x * y - z * w),
                      1.0f - 2.0f * (x * x + z * z),
                      2.0f * (y * z + x * w));
    }

    // Rotated +Z axis.
    Vector GetZAxis() const
    {
        return Vector(2.0f * (x * z + y * w),
                      2.0f * (y * z - w * x),
                      1.0f - 2.0f * (y * y + x * x));
    }
};

float Magnitude(const Vector& v);                   // 0x004AB100
Vector Normalize(const Vector& v);                  // 0x004AB180
Vector CrossProduct(const Vector& a, const Vector& b);   // 0x004AB210
Quaternion operator*(const Quaternion& a, const Quaternion& b);     // 0x004A9AC0

class RenderCamera {
public:
    char pad_0x0[0x7c];
    Vector m_vPosition;                             // +0x7c
};
extern RenderCamera* g_pRenderCamera;               // 0x007BB4E8
int BoxIntersect(Vector* pStart, Vector* pEnd, Vector* pMin, Vector* pMax);    // 0x004AA610
int PolyHit(Vector* pVerts, int nVerts, Vector* pStart, Vector* pEnd,
            Vector* pHit, Vector* pNormal);         // 0x004AB9D0

class Gob;
typedef void (*GobCallback)(Gob* pGob, char* szEvent, void* pData);

class Gob {
public:
    Gob(char* szName);                              // 0x00449440
    virtual ~Gob();
    virtual void g04(); virtual void g08(); virtual void g0c();
    virtual void g10(); virtual void g14(); virtual void g18(); virtual void g1c();
    virtual void g20(); virtual void g24();
    virtual int AddCallback(GobCallback pfn, char* szEvent, void* pData, float fTime, int bAlways);  // +0x28
    virtual void RemoveCallback(int nId);           // +0x2c
    virtual void g30(); virtual void g34(); virtual void g38(); virtual void g3c();
    virtual void g40(); virtual void g44(); virtual void g48(); virtual void g4c();
    virtual void g50(); virtual void g54(); virtual void g58(); virtual void g5c();
    virtual void g60();
    virtual Vector GetPosition();                   // +0x64

    CExoArrayList<Gob**> m_lWatchers;               // +0x04
    char pad_0x10[0x58 - 0x10];
    class GobModel* m_pModel;                       // +0x58
    char pad_0x5c[0x170 - 0x5c];
    class Camera* m_pCamera;                        // +0x170 (scenes)
    char pad_0x174[0x1cc - 0x174];
};

class Camera {
public:
    char pad_0x0[0x30];
    float m_fFadeStart;                             // +0x30
    char pad_0x34[0x1c];
    float m_fFade;                                  // +0x50
    char pad_0x54[0x40];
    float m_fAlpha;                                 // +0x94
};

// Safe pointer: registers its slot in the target's watcher list.
template <class T>
class SafePointer {
public:
    T* m_p;

    SafePointer() { m_p = 0; }
    ~SafePointer()
    {
        if (m_p) {
            CExoArrayList<T**>& lWatchers = m_p->m_lWatchers;
            lWatchers.Remove(&m_p);
        }
    }
    void Clear()
    {
        if (m_p) {
            CExoArrayList<T**>& lWatchers = m_p->m_lWatchers;
            lWatchers.Remove(&m_p);
        }
        m_p = 0;
    }
    SafePointer<T>& operator=(T* p)
    {
        if (m_p)
            m_p->m_lWatchers.Remove(&m_p);
        m_p = p;
        if (p)
            p->m_lWatchers.Add(&m_p);
        return *this;
    }
    operator T*() const { return m_p; }
    T* operator->() const { return m_p; }
};

class PartEmitter;

class WindManager {
public:
    CExoArrayList<WindManager**> m_lWatchers;       // +0x00
    char pad_0xc[0xc];
    Vector m_vWind;                                 // +0x18
    void DeregisterWindyEmitter(PartEmitter* pEmitter);     // 0x004A6480
};

class CHitInfo {
public:
    CHitInfo(int nType, int nMask, int nFlags);     // 0x004578F0
    int m_n00;
    int m_nFlags;                                   // +0x04
    CExoArrayList<int> m_l08;                       // +0x08
    char pad_0x14[0x20];
    int m_nHitType;                                 // +0x34
    SafePointer<Gob> m_pGob;                        // +0x38
    char pad_0x3c[4];
    Vector m_vNormal;                               // +0x40
    int m_n4c;
    int m_nSurface;                                 // +0x50
    char pad_0x54[0x10];
};

class Material {
public:
    Material();                                     // 0x0047B290
    ~Material();                                    // 0x0047B980
    void LoadTextures(PartEmitter* pPart);          // 0x0047B770
    char pad_0x0[0x4c];
};

class Part;

class MdlNode {
public:
    MdlNode(char* szName);                          // 0x00445210
    unsigned short m_nType;                         // +0x00
};

class MdlNodeEmitter : public MdlNode {
public:
    char pad_0x2[0x4e];
    float m_fSpread;                                // +0x50
    char pad_0x54[8];
    int m_nBranchCount;                             // +0x5c
    char pad_0x60[0x10];
    char m_szUpdate[0x20];                          // +0x70
    char m_szRender[0x20];                          // +0x90
    char m_szBlend[0x20];                           // +0xb0
    char pad_0xd0[0x20];
    char m_szChunk[0x14];                           // +0xf0
    int m_bLoop;                                    // +0x104
    char pad_0x108[0x24];
    unsigned int m_nFlags;                          // +0x12c

    unsigned char IsP2P() { return (unsigned char)m_nFlags & 1; }
    unsigned char IsP2PBezier() { return (unsigned char)(m_nFlags >> 1) & 1; }
    unsigned char IsAffectedByWind();
    unsigned char IsRandom() { return (unsigned char)(m_nFlags >> 5) & 1; }
    unsigned char IsInheritingVelocity() { return (unsigned char)(m_nFlags >> 7) & 1; }
    unsigned char IsRandomOrder(){ return (unsigned char)(m_nFlags >> 12) & 1; }
    void EnablePointToPoint();
    void EnablePointToPointSel();
    void EnableAffectedByWind();
    void EnableTinted();
    void EnableBouncing();
    void EnableRandom();
    void EnableRandomOrder();
    void EnableInheriting();
    void EnableInheritingVelocity();
    void EnableInheritingLocal();
    void EnableSplat();
    void EnableInheritingPart();
    void EnableDepthTexture();

    MdlNodeEmitter* GetEmitterNode();               // 0x0043E3C0
    Part* InternalCreateInstance(Gob* pScene);
    void ControlDispatch(Part* pPart, float fA, float fB, float fC);  // 0x0043F890
};

class Part {
public:
    Part(MdlNode* pNode);                           // 0x004457A0
    virtual ~Part();                                // 0x00440140
    virtual void p04(); virtual void p08();
    virtual void GetWorldTransform(Vector* pPosition, Quaternion* pOrientation);  // +0x0c
    virtual void p10(); virtual void p14();
    virtual int PlayAnimation(char* szName, float fSpeed, int nFlags, float fStart);  // +0x18
    virtual void p1c();
    virtual void p20(); virtual void p24(); virtual void p28(); virtual void p2c();
    virtual void p30(); virtual void p34();
    virtual Part* GetGobPart();                     // +0x38
    virtual void p3c();
    virtual void p40(); virtual void p44(); virtual void p48();
    virtual void SetVisible(int bVisible);          // +0x4c
    virtual void p50(); virtual void p54(); virtual void p58();
    virtual Vector SetPosition(Vector vPosition);   // +0x5c
    virtual Quaternion SetOrientation(Quaternion qOrientation);    // +0x60
    virtual void Render(int nFlags);                // +0x64
    virtual void p68(); virtual void p6c();
    virtual void p70(); virtual void p74(); virtual void p78();
    virtual void Initialize();                      // +0x7c
    virtual void Update(float fDelta);              // +0x80
    virtual int IsLocal();                          // +0x84
    virtual void p88(); virtual void p8c();
    virtual void p90(); virtual void p94(); virtual void p98(); virtual void p9c();
    virtual void pa0(); virtual void pa4(); virtual void pa8(); virtual void pac();
    virtual void pb0(); virtual void pb4(); virtual void pb8(); virtual void pbc();
    virtual void pc0(); virtual void pc4(); virtual void pc8();
    virtual void SetScale(float fScale, int nFlags);    // +0xcc

    MdlNodeEmitter* m_pNode;                        // +0x04
    char pad_0x8[0x28];
    CExoArrayList<Part*> m_lChildren;               // +0x30
    Part* m_pParent;                                // +0x3c
    Gob* m_pGob;                                    // +0x40
    char pad_0x44[8];
    union {
        int m_n4c;                                  // +0x4c: blend mode / gob part target
        float m_fRed;                               // +0x4c: PartTrigger colour
    };
};

void RemoveFromSceneManager(Part* pPart);           // 0x004554B0

extern int g_nParticleCount;                        // 0x0083046C
extern int g_nChunkVisible;                         // 0x007B92A0
Part* NewPartFromModel(char* szModel, char* szName, int a, int b);  // 0x00449CC0 (cdecl)

class Particle {
public:
    Particle(PartEmitter* pEmitter, float fArg);
    virtual ~Particle() { g_nParticleCount--; }

    virtual class ChunkyParticle* AsChunky();       // +0x04 (0x0063E7F0: return 0)
    virtual void Init(float fArg, float fScale);      // +0x08, 0x004921D0

    PartEmitter* m_pEmitter;                        // +0x04
    Vector m_vPosition;                             // +0x08
    float m_fAngle;                                 // +0x14
    float m_f18;
    Vector m_v1c;                                   // +0x1c
    float m_f28;
    Quaternion m_q2c;                               // +0x2c
    int m_n3c;                                      // +0x3c
    float m_f40;                                    // +0x40
    float m_f44;
    Vector m_vAxis;                                 // +0x48
    Vector m_v54;
    Vector m_v60;
    unsigned char m_b6c;                            // +0x6c
    float m_fAge;                                   // +0x70

    Part* ChunkPart();                              // ChunkyParticle::m_pPart
    void randomPosition(int nWidth, int nHeight);   // 0x0048D870
    void inheritVelocity(float fDelta);             // 0x0048D9E0
    void randomDirection(int nWidth, int nHeight);  // 0x0048DE60
};

class ChunkyParticle : public Particle {
public:
    ChunkyParticle(PartEmitter* pEmitter, float fArg);
    virtual ~ChunkyParticle();
    virtual void Init(float fArg, float fScale);

    Part* m_pPart;                                  // +0x74
};

inline Part* Particle::ChunkPart() { return ((ChunkyParticle*)this)->m_pPart; }

// Chunk models are Gobs: position +0x78, bounding radius +0x138.
class ChunkGob : public Part {
public:
    char pad_0x50[0x28];
    Vector m_vPosition;                             // +0x78
    char pad_0x84[0x138 - 0x84];
    float m_fRadius;                                // +0x138
};

class GobModel {
public:
    char pad_0x0[0x50];
    int m_nFlags;                                   // +0x50
};

extern float g_fLastEmitterF58;                     // 0x00830494

extern int g_nEmitterCount;                         // 0x00830470
extern int g_bDrawEmitters;                         // 0x00830498
extern int g_nRenderNormal;                         // 0x00798B6C
extern int g_nRenderBillboardToLocalZ;              // 0x00798B70
extern int g_nRenderBillboardToWorldZ;              // 0x00798B74
extern int g_nRenderLinked;                         // 0x00798B78
extern int g_nRenderAlignedToWorldZ;                // 0x00798B7C
extern short g_nLightningRenderFlags;               // 0x00798B80
extern short g_nEmitterRenderFlags;                 // 0x00798B84
extern int g_nRenderAlignedToParticleDir;           // 0x00798B88
extern int g_nRenderMotionBlur;                     // 0x00798B8C

class PartEmitter : public Part {
public:
    float m_f50;                                    // +0x50
    float m_f54;
    float m_f58;
    float m_f5c;
    float m_f60;
    float m_f64;
    float m_f68;
    float m_f6c;
    float m_f70;
    float m_fBirthrate;                             // +0x74
    float m_fLifeExp;                               // +0x78
    float m_f7c;
    float m_f80;
    float m_f84;
    float m_f88;
    float m_f8c;
    float m_f90;
    float m_f94;
    float m_f98;
    float m_f9c;
    float m_fa0;
    float m_fa4;
    float m_fa8;
    float m_fac;
    float m_fb0;
    float m_fb4;
    float m_fb8;
    float m_fbc;
    float m_fc0;
    float m_fc4;
    float m_fc8;
    float m_fcc;
    float m_fTargetDist;                            // +0xd0
    float m_fd4;
    float m_fd8;
    float m_fdc;
    float m_fe0;
    float m_fe4;
    float m_fe8;
    float m_fec;
    float m_ff0;
    float m_fFarClip;                               // +0xf4
    float m_ff8;
    float m_ffc;
    float m_f100;
    float m_f104;
    float m_f108;
    float m_f10c;
    float m_f110;
    int m_nRenderMode;                              // +0x114
    int m_n118;
    Vector m_v11c;
    Vector m_vXAxis;                                // +0x128
    Vector m_vYAxis;                                // +0x134
    Vector m_vZAxis;                                // +0x140
    Vector m_v14c;
    Vector m_v158;
    Vector m_vPosition;                             // +0x164
    Vector m_vLastPosition;                         // +0x170
    Vector m_v17c;
    Vector m_v188;
    Quaternion m_q194;
    Quaternion m_qOrientation;                      // +0x1a4
    Quaternion m_qLastOrientation;                  // +0x1b4
    SafePointer<WindManager> m_pWind;               // +0x1c4
    CExoArrayList<Particle*> m_lParticles;          // +0x1c8
    CExoArrayList<Particle*> m_lDeadParticles;      // +0x1d4
    Material* m_pMaterial;                          // +0x1e0
    SafePointer<Gob> m_pTarget;                     // +0x1e4
    CHitInfo* m_pHitInfo;                           // +0x1e8
    Camera* m_pCamera1ec;                           // +0x1ec
    int m_nCallbackId;                              // +0x1f0
    short m_nRenderFlags;                           // +0x1f4
    unsigned char m_b1f6;
    unsigned char m_b1f7;

    PartEmitter(MdlNodeEmitter* pNode);
    virtual ~PartEmitter();
    virtual void Initialize();

    void MoveFunc(float fDelta);
    void CalculateRadius();
    void updateParticleFrame(float fDelta);
    void MoveNormal(float fDelta);                  // 0x00492450
    void MoveP2PBezier(float fDelta);               // 0x00492EA0
    void MoveP2PGravity(float fDelta);              // 0x00493AE0
    void RenderNothing();
    void DrawDebug();
    void chkParticleLife(float fDelta);
    float GetCameraFade();
    void chkRandom(Particle* pParticle);
    int NextFrame(int nFrame);
    static void StartEmitterLayer();
    static void EndEmitterLayer();
    float GetRadius() { return m_fTargetDist; }

    Camera* GetSceneCamera()
    {
        if (m_pGob && m_pGob->m_pCamera)
            return m_pGob->m_pCamera;
        return 0;
    }
};

class FountainEmitter : public PartEmitter {
public:
    FountainEmitter(MdlNodeEmitter* pNode) : PartEmitter(pNode) {}
    virtual void p04();
};

class ExplosionEmitter : public PartEmitter {
public:
    ExplosionEmitter(MdlNodeEmitter* pNode) : PartEmitter(pNode) {}
    virtual void p08();
};

class SingleEmitter : public PartEmitter {
public:
    SingleEmitter(MdlNodeEmitter* pNode) : PartEmitter(pNode) {}
    virtual void p10();
    virtual void Update(float fDelta);
};

class LightningEmitter : public PartEmitter {
public:
    Vector m_v1f8;
    Vector m_v204;
    CExoArrayList<Vector> m_l210;
    CExoArrayList<Vector> m_l21c;
    CExoArrayList<Vector> m_l228;
    CExoArrayList<Vector> m_l234;
    CExoArrayList<Vector> m_l240;
    CExoArrayList<Vector> m_l24c;
    CExoArrayList<LightningEmitter*> m_lBranches;   // +0x258
    CExoArrayList<float*> m_l264;
    CExoArrayList<float*> m_l270;
    Vector m_v27c;
    int m_n288;
    unsigned char m_bBranch;                        // +0x28c

    LightningEmitter(MdlNodeEmitter* pNode);
    virtual ~LightningEmitter();
    virtual void Initialize();
    void InitializeBranch();
};

void EmitterTargetCallback(Gob* pGob, char* szEvent, void* pData);
void ExplosionCallback(Gob* pGob, char* szEvent, void* pData);  // 0x0048D770

// FUNCTION: SWKOTOR 0x0048D770
void ExplosionCallback(Gob* pGob, char* szEvent, void* pData)
{
    ((PartEmitter*)pData)->m_b1f6 = 1;
}

// FUNCTION: SWKOTOR 0x00490700
void PartEmitter::RenderNothing()
{
    if (m_pGob && m_pGob->m_pCamera && m_pGob->m_pCamera->m_fAlpha < 1.0f)
        return;
    if (g_bDrawEmitters)
        DrawDebug();
    for (int i = 0; i < m_lParticles.num; i++) {
        Particle* pParticle = m_lParticles[i];
        if (((ChunkyParticle*)pParticle)->m_pPart) {
            ((ChunkyParticle*)pParticle)->m_pPart->SetPosition(pParticle->m_vPosition);
            if (m_f88 != 0.0f && !pParticle->m_b6c)
                ((ChunkyParticle*)pParticle)->m_pPart->SetOrientation(Quaternion(pParticle->m_vAxis, pParticle->m_fAngle));
        }
    }
}

// FUNCTION: SWKOTOR 0x00494B30
void PartEmitter::chkParticleLife(float fDelta)
{
    unsigned char bNotify = 0;
    if (m_pNode->m_szChunk[0])
        bNotify = 1;
    for (int i = m_lParticles.num - 1; i >= 0; i--) {
        Particle* pParticle = m_lParticles[i];
        if (m_fLifeExp >= 0.0f && pParticle->m_fAge >= m_fLifeExp) {
            m_lParticles.DelIndex(i);
            m_lDeadParticles.Add(pParticle);
            if (bNotify && pParticle->ChunkPart())
                pParticle->ChunkPart()->SetVisible(0);
        } else {
            pParticle->m_fAge = fDelta + pParticle->m_fAge;
            if (pParticle->m_fAge < 0.0f) {
                pParticle->m_fAge = 0.0f;
                pParticle->m_f40 = 0.0f;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00494D40
void PartEmitter::MoveFunc(float fDelta)
{
    MdlNodeEmitter* pNode = m_pNode->GetEmitterNode();
    if (pNode->IsP2P()) {
        if (m_pTarget) {
            if (pNode->IsP2PBezier())
                MoveP2PBezier(fDelta);
            else
                MoveP2PGravity(fDelta);
        }
    } else {
        MoveNormal(fDelta);
    }
}

// FUNCTION: SWKOTOR 0x0048D780
unsigned char MdlNodeEmitter::IsAffectedByWind()
{
    unsigned char b = (unsigned char)(m_nFlags >> 2);
    b &= 1;
    return b;
}
// FUNCTION: SWKOTOR 0x0048D790
void MdlNodeEmitter::EnablePointToPoint() { m_nFlags |= 0x1; }
// FUNCTION: SWKOTOR 0x0048D7A0
void MdlNodeEmitter::EnablePointToPointSel() { m_nFlags |= 0x2; }
// FUNCTION: SWKOTOR 0x0048D7B0
void MdlNodeEmitter::EnableAffectedByWind() { m_nFlags |= 0x4; }
// FUNCTION: SWKOTOR 0x0048D7C0
void MdlNodeEmitter::EnableTinted() { m_nFlags |= 0x8; }
// FUNCTION: SWKOTOR 0x0048D7D0
void MdlNodeEmitter::EnableBouncing() { m_nFlags |= 0x10; }
// FUNCTION: SWKOTOR 0x0048D7E0
void MdlNodeEmitter::EnableRandom() { m_nFlags |= 0x20; }
// FUNCTION: SWKOTOR 0x0048D7F0
void MdlNodeEmitter::EnableRandomOrder() { m_nFlags |= 0x1000; }
// FUNCTION: SWKOTOR 0x0048D800
void MdlNodeEmitter::EnableInheriting() { m_nFlags |= 0x40; }
// FUNCTION: SWKOTOR 0x0048D810
void MdlNodeEmitter::EnableInheritingVelocity() { m_nFlags |= 0x80; }
// FUNCTION: SWKOTOR 0x0048D820
void MdlNodeEmitter::EnableInheritingLocal() { m_nFlags |= 0x100; }
// FUNCTION: SWKOTOR 0x0048D830
void MdlNodeEmitter::EnableSplat() { m_nFlags |= 0x200; }
// FUNCTION: SWKOTOR 0x0048D840
void MdlNodeEmitter::EnableInheritingPart() { m_nFlags |= 0x400; }
// FUNCTION: SWKOTOR 0x0048D850
void MdlNodeEmitter::EnableDepthTexture() { m_nFlags |= 0x800; }

// FUNCTION: SWKOTOR 0x0048D860 SYMBOL: ??1Particle@@UAE@XZ

// FUNCTION: SWKOTOR 0x0048DE30 SYMBOL: ??_GParticle@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0048DC30
ChunkyParticle::~ChunkyParticle()
{
    if (m_pPart) {
        delete m_pPart;
        m_pPart = 0;
    }
}

// FUNCTION: SWKOTOR 0x0048DC90
float PartEmitter::GetCameraFade()
{
    float fFade = 1.0f;
    Camera* pCamera;
    if (m_pCamera1ec)
        pCamera = m_pCamera1ec;
    else
        pCamera = GetSceneCamera();
    if (pCamera && m_vPosition.z - pCamera->m_fFadeStart > 5.0f)
        fFade = pCamera->m_fFade;
    return fFade;
}

// FUNCTION: SWKOTOR 0x0048DCF0
void PartEmitter::chkRandom(Particle* pParticle)
{
    if (m_pNode->IsRandom()) {
        int nRange = (int)(m_f6c - m_f70) + 1;
        if (nRange < 0)
            nRange = -nRange;
        if (nRange)
            pParticle->m_n3c = (int)(rand() % nRange + m_f70);
    }
}

// FUNCTION: SWKOTOR 0x0048DD50
int PartEmitter::NextFrame(int nFrame)
{
    if (m_f70 != m_f6c) {
        int n = nFrame;
        if (m_f6c < m_f70) {
            n++;
            if (n <= m_f70)
                return n;
        } else {
            n--;
            if (n >= m_f70)
                return n;
        }
    }
    return -1;
}

struct CEventEntry {
    int a, b, c;
};

// FUNCTION: SWKOTOR 0x0048DDE0 SYMBOL: ?DelIndex@?$CExoArrayList@UCEventEntry@@@@QAEXH@Z
template void CExoArrayList<CEventEntry>::DelIndex(int);

// FUNCTION: SWKOTOR 0x0048E190 SYMBOL: ??_GChunkyParticle@@UAEPAXI@Z

extern "C" {
__declspec(dllimport) void __stdcall glDepthMask(unsigned char flag);
__declspec(dllimport) void __stdcall glDisable(unsigned int cap);
__declspec(dllimport) void __stdcall glEnable(unsigned int cap);
__declspec(dllimport) void __stdcall glEnableClientState(unsigned int array);
__declspec(dllimport) void __stdcall glDisableClientState(unsigned int array);
}

extern unsigned int g_nCapLighting;                 // 0x0078D45C
extern unsigned int g_nCapBlend;                    // 0x0078D448
extern unsigned int g_nCapAlphaTest;                // 0x0078D44C
extern const unsigned int g_nVertexArray;           // 0x0073F3A4
extern const unsigned int g_nColorArray;            // 0x0073F3A0
extern const unsigned int g_nTexCoordArray;         // 0x0073F398
extern const unsigned int g_nNormalArray;           // 0x0073F39C

void DrawSphere(float fRadius, Vector* pPosition, Vector* pColor, float fAlpha, int nFlags);  // 0x00431A50

// FUNCTION: SWKOTOR 0x0048EF10
void PartEmitter::DrawDebug()
{
    Vector vPosition;
    Quaternion qOrientation;
    GetWorldTransform(&vPosition, &qOrientation);
    DrawSphere(GetRadius(), &vPosition, &Vector(1.0f, 0.5f, 1.0f), 1.0f, 1);
}

// FUNCTION: SWKOTOR 0x0048EFA0
void PartEmitter::StartEmitterLayer()
{
    glDepthMask(0);
    glDisable(g_nCapLighting);
    glEnable(g_nCapBlend);
    glDisable(g_nCapAlphaTest);
    glEnableClientState(g_nVertexArray);
    glEnableClientState(g_nColorArray);
    glEnableClientState(g_nTexCoordArray);
    glDisableClientState(g_nNormalArray);
}

// FUNCTION: SWKOTOR 0x00494C40
Particle::Particle(PartEmitter* pEmitter, float fArg)
{
    m_pEmitter = pEmitter;
    Particle::Init(fArg, 1.0f);
    g_nParticleCount++;
}

// FUNCTION: SWKOTOR 0x00494CB0
ChunkyParticle::ChunkyParticle(PartEmitter* pEmitter, float fArg)
    : Particle(pEmitter, fArg)
{
    if (pEmitter) {
        m_pPart = NewPartFromModel(pEmitter->m_pNode->m_szChunk, "", 0, 0);
        if (m_pPart)
            m_pPart->SetVisible(g_nChunkVisible);
    }
}

// FUNCTION: SWKOTOR 0x0049B230 SYMBOL: ??1?$SafePointer@VWindManager@@@@QAE@XZ

// FUNCTION: SWKOTOR 0x0049B240
void EmitterTargetCallback(Gob* pGob, char* szEvent, void* pData)
{
    PartEmitter* pEmitter = (PartEmitter*)pData;
    for (int i = 0; i < pEmitter->m_lChildren.num; i++) {
        Part* pChild = pEmitter->m_lChildren[i];
        if (pChild->GetGobPart()) {
            pEmitter->m_pTarget = (Gob*)pChild->GetGobPart()->m_n4c;
            Vector vPosition;
            Quaternion qOrientation;
            pEmitter->GetWorldTransform(&vPosition, &qOrientation);
            if (pEmitter->m_pTarget)
                pEmitter->m_fTargetDist = Magnitude(pEmitter->m_pTarget->GetPosition() - vPosition);
            else
                pEmitter->m_fTargetDist = 0.0f;
            return;
        }
    }
    pEmitter->m_pGob->RemoveCallback(pEmitter->m_nCallbackId);
}

// FUNCTION: SWKOTOR 0x0049B380
PartEmitter::PartEmitter(MdlNodeEmitter* pNode)
    : Part(pNode)
{
    m_f50 = 0.0f;
    m_fd8 = 0.0f;
    m_f54 = 0.0f;
    m_fdc = 255.0f;
    m_fe0 = 255.0f;
    m_fe4 = 255.0f;
    m_f58 = 0.0f;
    m_ff0 = 0.0f;
    m_f5c = 0.0f;
    m_f60 = 0.0f;
    m_f64 = 0.0f;
    m_f68 = 0.0f;
    m_f6c = 0.0f;
    m_f70 = 0.0f;
    m_fBirthrate = 0.0f;
    m_fLifeExp = 0.0f;
    m_f7c = 0.0f;
    m_f80 = 0.0f;
    m_f84 = 0.0f;
    m_f88 = 0.0f;
    m_f8c = 0.0f;
    m_f90 = 0.0f;
    m_fe8 = 0.0f;
    m_f94 = 0.0f;
    m_f98 = 0.0f;
    m_fec = 0.0f;
    m_f9c = 0.0f;
    m_fa0 = 0.0f;
    m_fa4 = 0.0f;
    m_fa8 = 0.0f;
    m_fac = 0.0f;
    m_fb0 = 0.0f;
    m_fb4 = 1.0f;
    m_fb8 = 0.0f;
    m_fbc = 0.0f;
    m_fc0 = 0.0f;
    m_fc4 = 0.0f;
    m_fc8 = 0.0f;
    m_f104 = 0.0f;
    m_f108 = 0.0f;
    m_f10c = 0.0f;
    m_f110 = 0.0f;
    m_ffc = 0.0f;
    m_fFarClip = 10000.0f;
    m_ff8 = 10000.0f;
    m_b1f6 = 0;
    m_pMaterial = 0;
    m_fTargetDist = 1.0f;
    m_fd4 = 0.0f;
    m_pCamera1ec = 0;
    m_pTarget.Clear();
    m_pWind.Clear();
    m_pHitInfo = new CHitInfo(1, ~0x40000000, 0);
    g_nEmitterCount++;
}

// NONMATCH (45 insns): everything up to the quaternion -> axes block matches
// (except the word / byte store order at +0x1f4 / +0x1f7); MSVC
// canonicalises the operand order of every product (fld w; fmul z whatever
// the source says), while the original keeps a different order per row.
// Source order, local copies, a pointer, a Quaternion member function and
// 2.0f * (...) all give the same 45.
// FUNCTION: SWKOTOR 0x0049CAA0
void PartEmitter::Initialize()
{
    MdlNodeEmitter* pNode = m_pNode;
    if (!strcmp(pNode->m_szRender, "Normal"))
        m_nRenderMode = g_nRenderNormal;
    else if (!strcmp(pNode->m_szRender, "Billboard_to_World_Z"))
        m_nRenderMode = g_nRenderBillboardToWorldZ;
    else if (!strcmp(pNode->m_szRender, "Billboard_to_Local_Z"))
        m_nRenderMode = g_nRenderBillboardToLocalZ;
    else if (!strcmp(pNode->m_szRender, "Linked"))
        m_nRenderMode = g_nRenderLinked;
    else if (!strcmp(pNode->m_szRender, "Aligned_to_World_Z"))
        m_nRenderMode = g_nRenderAlignedToWorldZ;
    else if (!strcmp(pNode->m_szRender, "Aligned_to_Particle_Dir"))
        m_nRenderMode = g_nRenderAlignedToParticleDir;
    else if (!strcmp(pNode->m_szRender, "Motion_Blur"))
        m_nRenderMode = g_nRenderMotionBlur;
    m_nRenderFlags = g_nEmitterRenderFlags;
    m_b1f7 = 0;
    if (pNode->IsP2P()) {
        m_nCallbackId = m_pGob->AddCallback(EmitterTargetCallback, "_EmitterTarget", this, 9999.0f, 0);
        if (!pNode->IsP2PBezier())
            m_fd4 = m_fBirthrate;
    }
    if (!_stricmp(pNode->m_szBlend, "Normal"))
        m_n4c = 0;
    else if (!_stricmp(pNode->m_szBlend, "PunchThrough") || !_stricmp(pNode->m_szBlend, "Punch-Through"))
        m_n4c = 1;
    else
        m_n4c = 2;
    m_pMaterial = new Material();
    m_pMaterial->LoadTextures(this);
    if (IsLocal())
        m_fFarClip = 10000.0f;
    else
        m_fFarClip = 0.0f;
    m_ff8 = 10000.0f;
    m_fcc = 0.0f;
    m_v14c = Vector(0.0f, 0.0f, 0.0f);
    m_vLastPosition = m_vPosition;
    m_qLastOrientation = m_qOrientation;
    Quaternion& q = m_qOrientation;
    m_vXAxis = Vector(1.0f - (q.y * q.y + q.z * q.z) * 2.0f,
                      (q.y * q.x + q.w * q.z) * 2.0f,
                      (q.z * q.x - q.w * q.y) * 2.0f);
    m_vYAxis = Vector((q.y * q.x - q.w * q.z) * 2.0f,
                      1.0f - (q.x * q.x + q.z * q.z) * 2.0f,
                      (q.z * q.y + q.x * q.w) * 2.0f);
    m_vZAxis = Vector((q.z * q.x + q.y * q.w) * 2.0f,
                      (q.z * q.y - q.w * q.x) * 2.0f,
                      1.0f - (q.x * q.x + q.y * q.y) * 2.0f);
}

// FUNCTION: SWKOTOR 0x0049CE30
void LightningEmitter::InitializeBranch()
{
    PartEmitter::Initialize();
    m_pNode->ControlDispatch(this, -1.0f, 0.0f, 1.0f);
    m_l210.SetSize(2);
    m_l21c.SetSize(2);
    m_l228.SetSize(2);
    m_l234.SetSize(2);
    m_l240.SetSize(2);
    m_l24c.SetSize(2);
    m_lBranches.SetSize(0);
    m_l264.SetSize(0);
    m_bBranch = 1;
    m_pTarget = new Gob("_AUR_lightningbranch");
    m_fb0 = 0.0f;
    m_fac = 0.0f;
    m_ffc = 0.0f;
    m_fFarClip = m_fb8;
    m_ff8 = m_f108;
}

// FUNCTION: SWKOTOR 0x0049CFA0
PartEmitter::~PartEmitter()
{
    if (m_pHitInfo)
        delete m_pHitInfo;
    int i;
    for (i = m_lParticles.num - 1; i >= 0; i--) {
        Particle* pParticle = m_lParticles.element[i];
        m_lParticles.DelIndex(i);
        if (pParticle)
            delete pParticle;
    }
    for (i = m_lDeadParticles.num - 1; i >= 0; i--) {
        Particle* pParticle = m_lDeadParticles.element[i];
        m_lDeadParticles.DelIndex(i);
        if (pParticle)
            delete pParticle;
    }
    if (m_pWind)
        m_pWind->DeregisterWindyEmitter(this);
    if (m_pGob)
        RemoveFromSceneManager(this);
    if (m_pMaterial)
        delete m_pMaterial;
    g_nEmitterCount--;
}

// NONMATCH (15 insns): the original keeps an SEH frame without any state
// store (empty unwind table); ours drops the frame. Body stores match.
// FUNCTION: SWKOTOR 0x0049D1D0
LightningEmitter::LightningEmitter(MdlNodeEmitter* pNode)
    : PartEmitter(pNode)
{
    m_fc0 = 0.0f;
    m_n288 = 0;
    m_bBranch = 0;
}

// FUNCTION: SWKOTOR 0x0049D300
LightningEmitter::~LightningEmitter()
{
    MdlNodeEmitter* pNode = m_pNode->GetEmitterNode();
    if (m_lBranches.num) {
        for (int i = 0; i < pNode->m_nBranchCount; i++) {
            if (m_lBranches[i]->m_pTarget)
                delete (Gob*)m_lBranches[i]->m_pTarget;
            if (m_lBranches[i])
                delete m_lBranches[i];
        }
    }
}

// FUNCTION: SWKOTOR 0x0049D480
void LightningEmitter::Initialize()
{
    PartEmitter::Initialize();
    m_fLifeExp = 1.0f;
    int nId = m_pGob->AddCallback(EmitterTargetCallback, "_EmitterTarget", this, 9999.0f, 0);
    m_nRenderFlags = g_nLightningRenderFlags;
    m_nCallbackId = nId;
    MdlNodeEmitter* pNode = m_pNode->GetEmitterNode();
    m_lBranches.SetSize(pNode->m_nBranchCount);
    m_l264.SetSize(pNode->m_nBranchCount);
    for (int i = 0; i < pNode->m_nBranchCount; i++) {
        m_lBranches[i] = new LightningEmitter(m_pNode);
        m_lBranches[i]->InitializeBranch();
    }
}

// FUNCTION: SWKOTOR 0x0049D5C0
Part* MdlNodeEmitter::InternalCreateInstance(Gob* pScene)
{
    PartEmitter* pEmitter;
    if (!strcmp(m_szUpdate, "Fountain")) {
        pEmitter = new FountainEmitter(this);
    } else if (!strcmp(m_szUpdate, "Explosion")) {
        pEmitter = new ExplosionEmitter(this);
        if (pScene)
            pScene->AddCallback(ExplosionCallback, "Detonate", pEmitter, 9999.0f, 0);
    } else if (!strcmp(m_szUpdate, "Single")) {
        pEmitter = new SingleEmitter(this);
    } else {
        pEmitter = new LightningEmitter(this);
    }
    pEmitter->m_pGob = pScene;
    pEmitter->Initialize();
    return pEmitter;
}

// FUNCTION: SWKOTOR 0x0049D740 SYMBOL: ??_GPartEmitter@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0049D760 SYMBOL: ??_GFountainEmitter@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0049D780 SYMBOL: ??1FountainEmitter@@UAE@XZ

// FUNCTION: SWKOTOR 0x0049D790 SYMBOL: ??_GLightningEmitter@@UAEPAXI@Z

// ---------------------------------------------------------------------------
// PartTrigger

extern const unsigned short g_nTriggerNodeType;     // 0x00740A40

class MdlNodeTrigger : public MdlNode {
public:
    MdlNodeTrigger(char* szName);
};

// FUNCTION: SWKOTOR 0x0049D7B0
MdlNodeTrigger::MdlNodeTrigger(char* szName)
    : MdlNode(szName)
{
    m_nType = g_nTriggerNodeType;
}

class Scene {
public:
    char pad_0x0[0xa0];
    void* m_pBSP;                                   // +0xa0
};

void SceneBSPRemovePart(Scene* pScene, Part* pPart);   // 0x0046C510
void SceneRemovePart(Scene* pScene, Part* pPart);      // 0x0046D1E0

class PartTrigger : public Part {
public:
    float m_fGreen;                                 // +0x50
    float m_fBlue;                                  // +0x54
    float m_fAlphaBase;                             // +0x58
    Vector m_vMin;                                  // +0x5c
    Vector m_vMax;                                  // +0x68
    CExoArrayList<Vector> m_l74;                    // +0x74 outline
    CExoArrayList<Vector> m_lPoints;                // +0x80
    CExoArrayList<Vector> m_lLines1;                // +0x8c
    CExoArrayList<Vector> m_lLines2;                // +0x98
    int m_nSurface;                                 // +0xa4
    unsigned char m_ba8;
    float m_fHighlightWidth;                        // +0xac
    unsigned char m_bb0;
    Vector m_vColor;                                // +0xb4
    float m_fAlpha;                                 // +0xc0
    unsigned char m_bc4;
    float m_fc8;
    unsigned char m_bcc;
    CExoArrayList<Vector> m_lVerts;                 // +0xd0

    PartTrigger(MdlNode* pNode);
    virtual ~PartTrigger();
    int HitCheckGeom(Vector* pStart, Vector* pEnd, Vector* pHit, CHitInfo* pInfo, int nUnused);
    void DrawVolume();
    virtual void Render(int nFlags);
    void SetHighlight(unsigned char bHighlight, float fWidth, unsigned char bFlag);  // 0x0049DC10
};

// FUNCTION: SWKOTOR 0x0049E610
void AurPartTriggerSetHighlight(PartTrigger* pPart, unsigned char bHighlight, float fWidth,
                                unsigned char bFlag)
{
    pPart->SetHighlight(bHighlight, fWidth, bFlag);
}

void DrawFromVerts(int nMode, CExoArrayList<Vector>* pVerts);  // 0x00451D70

// FUNCTION: SWKOTOR 0x0049D7D0
void AurPartTriggerDelete(Scene* pScene, PartTrigger* pPart, unsigned char bDelete)
{
    if (pScene->m_pBSP)
        SceneBSPRemovePart(pScene, pPart);
    else
        SceneRemovePart(pScene, pPart);
    if (bDelete && pPart)
        delete pPart;
}

// FUNCTION: SWKOTOR 0x0049D810
int PartTrigger::HitCheckGeom(Vector* pStart, Vector* pEnd, Vector* pHit, CHitInfo* pInfo, int nUnused)
{
    if (!(pInfo->m_nFlags & 0x40000000))
        return 0;
    if (!BoxIntersect(pStart, pEnd, &m_vMin, &m_vMax))
        return 0;
    for (int i = 0; i < m_lVerts.num; i += 3) {
        int nResult = PolyHit(&m_lVerts[i], 3, pStart, pEnd, pEnd, &pInfo->m_vNormal);
        if (nResult) {
            if (pHit) {
                *pHit = *pEnd;
                pInfo->m_nSurface = m_nSurface;
                pInfo->m_nHitType = 0x40000000;
            }
            return nResult;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0049D8E0
PartTrigger::~PartTrigger()
{
    RemoveFromSceneManager(this);
}

// FUNCTION: SWKOTOR 0x0049D9B0
void AurPartTriggerSetColor(PartTrigger* pPart, Vector* pColor, float fAlpha, unsigned char bC4,
                            float fC8, unsigned char bCC)
{
    pPart->m_vColor.x = pColor->x;
    pPart->m_vColor.y = pColor->y;
    pPart->m_vColor.z = pColor->z;
    pPart->m_fAlpha = fAlpha;
    pPart->m_bc4 = bC4;
    pPart->m_fc8 = fC8;
    pPart->m_bcc = bCC;
}

// FUNCTION: SWKOTOR 0x0049DA00
void PartTrigger::DrawVolume()
{
    DrawFromVerts(4, &m_lPoints);
    DrawFromVerts(8, &m_lLines1);
    DrawFromVerts(8, &m_lLines2);
}

// FUNCTION: SWKOTOR 0x0049DB10 SYMBOL: ??_GPartTrigger@@UAEPAXI@Z

// NONMATCH (12 insns): same frame-only difference as the LightningEmitter
// ctor (also tried Vector arrays, an out-of-class list ctor and an inline
// helper defined after the ctor).
// FUNCTION: SWKOTOR 0x0049DB30
PartTrigger::PartTrigger(MdlNode* pNode)
    : Part(pNode)
{
    m_vColor.x = 1.0f;
    m_vColor.y = 1.0f;
    m_vColor.z = 1.0f;
    m_fc8 = 1.0f;
    m_ba8 = 0;
    m_bb0 = 0;
    m_bc4 = 0;
    m_bcc = 0;
    m_fAlpha = 0.5f;
}

extern float g_fTriggerAlpha;                       // 0x008304B8
extern unsigned int g_nCapDepthTest;                // 0x0078D458
extern "C" {
__declspec(dllimport) void __stdcall glPushAttrib(unsigned int mask);
__declspec(dllimport) void __stdcall glPopAttrib();
__declspec(dllimport) void __stdcall glColor4fv(const float* v);
__declspec(dllimport) void __stdcall glBlendFunc(unsigned int sfactor, unsigned int dfactor);
}
extern const unsigned int g_nBlendSrcAlpha;         // 0x0073F270
extern const unsigned int g_nBlendOneMinusSrcAlpha; // 0x0073F274
extern const unsigned int g_nBlendOne;              // 0x0073F278

// FUNCTION: SWKOTOR 0x0049DA40
void PartTrigger::Render(int nFlags)
{
    float color[4];
    color[3] = g_fTriggerAlpha + m_fAlphaBase;
    color[0] = m_fRed;
    color[1] = m_fGreen;
    color[2] = m_fBlue;
    if (color[3] != 0.0f) {
        glPushAttrib(0x40040);
        glDisable(g_nCapBlend);
        glDisable(g_nCapDepthTest);
        glDisable(g_nCapLighting);
        glColor4fv(color);
        DrawFromVerts(9, &m_l74);
        glEnable(g_nCapBlend);
        if (m_bcc)
            glBlendFunc(g_nBlendOne, g_nBlendOne);
        else
            glBlendFunc(g_nBlendSrcAlpha, g_nBlendOneMinusSrcAlpha);
        DrawFromVerts(3, &m_lVerts);
        glPopAttrib();
    }
}

Vector MinVector(const CExoArrayList<Vector>& l);   // 0x004AB060
Vector MaxVector(const CExoArrayList<Vector>& l);   // 0x004AAFC0

// NONMATCH (5 insns): loop 1 swaps the stack slots of the counter and the
// byte offset, and loop 2 loads -m_fHighlightWidth before e.y.
// FUNCTION: SWKOTOR 0x0049DC10
void PartTrigger::SetHighlight(unsigned char bHighlight, float fWidth, unsigned char bFlag)
{
    m_bb0 = bFlag;
    m_ba8 = bHighlight;
    m_fHighlightWidth = (fWidth < 0.0f) ? -fWidth : fWidth;
    if (m_l74.num < 2 || !bHighlight)
        return;
    m_lPoints.num = 0;
    m_lLines1.num = 0;
    m_lLines2.num = 0;
    for (int i = 0; i < m_l74.num; i++) {
        Vector v = m_l74[i];
        v.z += m_fHighlightWidth;
        m_lPoints.Add(v);
        m_lLines1.Add(v);
        v.z -= m_fHighlightWidth + m_fHighlightWidth;
        m_lPoints.Add(v);
    }
    for (int j = m_l74.num; j--; ) {
        Vector& e = m_l74[j];
        float fZ = -m_fHighlightWidth;
        m_lLines2.Add(Vector(e.x, e.y, fZ + e.z));
    }
    m_lPoints.Add(m_lPoints[0]);
    m_lPoints.Add(m_lPoints[1]);
    m_vMin = MinVector(m_lPoints);
    m_vMax = MaxVector(m_lPoints);
}

extern "C" __declspec(dllimport) void __stdcall glEnableClientState(unsigned int array);

// FUNCTION: SWKOTOR 0x0048F000
void PartEmitter::EndEmitterLayer()
{
    glDepthMask(1);
    glEnable(g_nCapLighting);
    glBlendFunc(g_nBlendSrcAlpha, g_nBlendOneMinusSrcAlpha);
    glEnableClientState(g_nNormalArray);
}

#define g_fTwoPi 6.28318548f                        // 0x00740C88
#define g_fMinusTwoPi -6.28318548f                  // 0x00743338

// NONMATCH (2 insns, reported 11): the original sets ecx for Append before
// `push eax`; ours pushes first (tried a list reference / pointer and a ?:).
// FUNCTION: SWKOTOR 0x004979C0
void SingleEmitter::Update(float fDelta)
{
    MdlNodeEmitter* pNode = m_pNode;
    if ((int)m_f58) {
        if (m_lParticles.num == 0) {
            Particle* pParticle;
            if (pNode->m_szChunk[0])
                pParticle = new ChunkyParticle(this, fDelta);
            else
                pParticle = new Particle(this, fDelta);
            m_lParticles.Append(pParticle);
            chkRandom(m_lParticles[0]);
            return;
        }
        m_lParticles[0]->m_fAge += fDelta;
        if (m_fLifeExp > 0.0f) {
            if (m_lParticles[0]->m_fAge > m_fLifeExp && pNode->m_bLoop)
                m_lParticles[0]->m_fAge = 0.0f;
        }
        else if (m_lParticles[0]->m_fAge < 0.0f) {
            if (m_fLifeExp > 0.0f)
                m_lParticles[0]->m_fAge = 0.0f;
            else
                m_lParticles[0]->m_fAge = fDelta;
        }
        m_lParticles[0]->m_fAngle += fDelta * m_f88;
        if (m_lParticles[0]->m_fAngle > g_fTwoPi)
            m_lParticles[0]->m_fAngle -= g_fTwoPi;
        else if (m_lParticles[0]->m_fAngle < g_fMinusTwoPi)
            m_lParticles[0]->m_fAngle += g_fTwoPi;
        if (m_nRenderMode == g_nRenderMotionBlur) {
            float t = (fDelta < m_fb4 ? fDelta : m_fb4) / m_fb4;
            Particle* pParticle = m_lParticles[0];
            Vector* pTrail = &pParticle->m_v60;
            float s = 1.0f - t;
            Vector a(pTrail->x * s, pTrail->y * s, pTrail->z * s);
            Vector b(pParticle->m_vPosition.x * t, pParticle->m_vPosition.y * t, pParticle->m_vPosition.z * t);
            *pTrail = Vector(a.x + b.x, a.y + b.y, a.z + b.z);
        }
        MoveFunc(fDelta);
    }
    else if (m_lParticles.num) {
        Particle* pParticle = m_lParticles[0];
        m_lParticles.RemoveAt(0);
        if (pParticle)
            delete pParticle;
    }
}

// FUNCTION: SWKOTOR 0x00492400
void ChunkyParticle::Init(float fArg, float fScale)
{
    Particle::Init(fArg, 1.0f);
    if (m_pPart) {
        m_pPart->SetVisible(g_nChunkVisible);
        m_pPart->SetScale(fScale, 1);
        m_pPart->PlayAnimation("impact", 1.0f, 1, 0);
    }
}

// NONMATCH (2 real insns): the original loads m_fLifeExp before m_fd4 for
// their product; MSVC reorders the commutative operands whatever the source
// order (tried 9 spellings, inline getters, a fGravity local).
// FUNCTION: SWKOTOR 0x0048E1B0
void PartEmitter::CalculateRadius()
{
    if (m_pGob && m_pGob->m_pModel && (m_pGob->m_pModel->m_nFlags & 1)) {
        m_fTargetDist = 10000.0f;
        return;
    }
    Vector vPosition;
    Quaternion qOrientation;
    GetWorldTransform(&vPosition, &qOrientation);
    if (m_pTarget) {
        Vector v = m_pTarget->GetPosition() - vPosition;
        m_fTargetDist = Magnitude(v);
        return;
    }
    float fGravity = m_f7c * -9.81f;
    if (fGravity < 0.0f)
        fGravity = -fGravity;
    m_fd4 = fGravity;
    float fWind = 0.0f;
    if (m_pWind) {
        Vector v = m_pWind->m_vWind;
        fWind = Magnitude(v);
    }
    float fRadius = fWind + (m_fa8 + m_f8c);
    fRadius = (fRadius + m_fd4 * m_fLifeExp * 0.5f) * m_fLifeExp;
    m_fTargetDist = fRadius;
    float fX = m_f90 > m_f94 ? m_f90 : m_f94;
    if (!(fX > m_fe8))
        fX = m_fe8;
    fX = fX * 0.5f;
    float fY = m_f98 > m_f9c ? m_f98 : m_f9c;
    if (!(fY > m_fec))
        fY = m_fec;
    fY = fY * 0.5f;
    if (fY <= 0.0f)
        fY = fX;
    float fA = m_fb0 * 0.01f;
    float fB = m_fac * 0.01f;
    m_fTargetDist = ((float)sqrt(fY * fY + fX * fX) + (float)sqrt(fB * fB + fA * fA) + fRadius) * 1.15f;
    for (int i = 0; i < m_lParticles.num; i++) {
        ChunkyParticle* pChunky = m_lParticles[i]->AsChunky();
        if (!pChunky)
            break;
        ChunkGob* pPart = (ChunkGob*)pChunky->m_pPart;
        if (pPart) {
            Vector v;
            v.x = pPart->m_vPosition.x - vPosition.x;
            v.y = pPart->m_vPosition.y - vPosition.y;
            v.z = pPart->m_vPosition.z - vPosition.z;
            float f = Magnitude(v) + pPart->m_fRadius;
            if (f < m_fTargetDist)
                f = m_fTargetDist;
            m_fTargetDist = f;
        }
    }
    g_fLastEmitterF58 = m_f58;
}

static inline void FloatToInt(int* pOut, float f)
{
    __asm {
        fld f
        mov edx, pOut
        frndint
        fistp dword ptr [edx]
    }
}

// FUNCTION: SWKOTOR 0x0048E460
void PartEmitter::updateParticleFrame(float fDelta)
{
    unsigned char bRandom = 0;
    unsigned char bRandomOrder = 0;
    int nRange, nSteps;
    float fSpan = m_f6c - m_f70;
    FloatToInt(&nRange, fSpan);
    if (nRange < 0)
        nRange = -nRange;
    nRange++;
    if (m_pNode->IsRandomOrder())
        bRandomOrder = 1;
    if (m_pNode->IsRandom())
        bRandom = 1;
    for (int i = 0; i < m_lParticles.num; i++) {
        Particle* pParticle = m_lParticles[i];
        float fElapsed;
        if (bRandom)
            fElapsed = pParticle->m_fAge - pParticle->m_f40;
        else
            fElapsed = pParticle->m_fAge - pParticle->m_f44;
        if (fDelta != 0.0f && fElapsed >= fDelta) {
            int nLo, nHi;
            FloatToInt(&nLo, m_f70);
            FloatToInt(&nHi, m_f6c);
            if (m_f70 != m_f6c) {
                if (bRandomOrder) {
                    pParticle->m_n3c = rand() % nRange;
                } else if (m_f6c < m_f70) {
                    FloatToInt(&nSteps, (float)-floor(fElapsed / fDelta));
                    pParticle->m_n3c += nSteps;
                    if (bRandom) {
                        pParticle->m_f44 = nSteps * fDelta + pParticle->m_f44;
                        while (pParticle->m_n3c < m_f6c)
                            pParticle->m_n3c += nLo - nHi + 1;
                    } else {
                        pParticle->m_n3c = nLo > pParticle->m_n3c ? nLo : pParticle->m_n3c;
                    }
                } else {
                    FloatToInt(&nSteps, (float)floor(fElapsed / fDelta));
                    pParticle->m_n3c += nSteps;
                    if (bRandom) {
                        pParticle->m_f44 = nSteps * fDelta + pParticle->m_f44;
                        while (pParticle->m_n3c > nHi)
                            pParticle->m_n3c += nLo - nHi - 1;
                    } else {
                        pParticle->m_n3c = nHi < pParticle->m_n3c ? nHi : pParticle->m_n3c;
                    }
                }
                if (!bRandom)
                    pParticle->m_f44 = pParticle->m_n3c * fDelta;
                if (m_fLifeExp >= 0.0f)
                    pParticle->m_f40 = pParticle->m_fAge;
                else {
                    pParticle->m_f40 = 0.0f;
                    pParticle->m_fAge = 0.0f;
                }
            }
        }
    }
}

// NONMATCH (2 real insns): GetZAxis y*w loads w first in ours (same operand
// canonicalisation as Particle::inheritVelocity / CalculateRadius); a
// `Quaternion* q` spelling fixes y*w but swaps the sum instead.
// FUNCTION: SWKOTOR 0x004921D0 SYMBOL: ?Init@Particle@@UAEXMM@Z
void Particle::Init(float fArg, float fScale)
{
    PartEmitter* pEmitter = m_pEmitter;
    m_b6c = 0;
    m_fAngle = 0.0f;
    m_f40 = 0.0f;
    m_fAge = 0.0f;
    m_f44 = 0.0f;
    m_q2c = pEmitter->m_qOrientation;
    m_f18 = 0.0f;
    m_v1c.z = 0.0f;
    m_v1c.y = 0.0f;
    m_v1c.x = 0.0f;
    int nSpeed = 0;
    m_n3c = (int)pEmitter->m_f70;
    float fSpread = pEmitter->m_f8c;
    if (fSpread > 0.01) {
        nSpeed = rand() % (int)(fSpread * 100.0f);
        if (rand() % 2 > 0)
            nSpeed = -nSpeed;
    }
    int nWidth = (int)(fScale * m_pEmitter->m_fac * 100.0f);
    int nHeight = (int)(fScale * m_pEmitter->m_fb0 * 100.0f);
    if (nWidth == 0 && nHeight == 0)
        m_vPosition = m_pEmitter->m_vPosition;
    else
        randomPosition(nWidth, nHeight);
    m_v60 = m_vPosition;
    if (m_pEmitter->m_pNode->IsInheritingVelocity())
        inheritVelocity(fArg);
    if (m_pEmitter->m_fa0 != 0.0f)
        randomDirection(nWidth, nHeight);
    m_vAxis = m_q2c.GetZAxis();
    m_v54 = m_vAxis * (nSpeed * 0.01f + m_pEmitter->m_fa8);
}

// NONMATCH (~62): the original keeps `this` in ebx and copies the loop
// quaternion from the operator* temp (edi) after the GetYAxis call; ours
// reloads it from m_q2c (esi) and uses edi for this. Tried q as named result,
// assigned, chained and declared before the if.
// FUNCTION: SWKOTOR 0x0048DE60
void Particle::randomDirection(int nWidth, int nHeight)
{
    float fHalf = m_pEmitter->m_fa0 * 0.5f;
    Vector vDir = Normalize(m_vPosition - g_pRenderCamera->m_vPosition);
    int n = rand() % (1 - (int)(fHalf * -100.0f));
    m_q2c = Quaternion(m_pEmitter->m_vXAxis, n * 0.01f) * m_q2c;
    n = rand() % 628;
    m_q2c = Quaternion(m_pEmitter->m_vZAxis, n * 0.01f) * m_q2c;
    if (m_pEmitter->m_pNode->m_fSpread != 0.0f) {
        float fDot = vDir * m_q2c.GetYAxis();
        Quaternion q = m_q2c;
        while ((fDot < 0.0f ? -fDot : fDot) < cos(1.5707963267948966 - m_pEmitter->m_pNode->m_fSpread)) {
            float fAngle;
            if (fDot < 0.0f)
                fAngle = m_pEmitter->m_pNode->m_fSpread + 1.5707964f - fDot;
            else
                fAngle = fDot - 1.5707964f + m_pEmitter->m_pNode->m_fSpread;
            q = Quaternion(CrossProduct(vDir, m_q2c.YAxis()), fAngle) * m_q2c;
            fDot = q.YAxis() * vDir;
        }
        m_q2c = q;
    }
}
