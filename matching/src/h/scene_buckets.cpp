// Scene render buckets drawn from the global gob lists (Scene vtable
// +0xDC .. +0xE8). The lists are filled by ManageSceneBSP.

#include <math.h>

extern "C" __declspec(dllimport) void __stdcall glEnable(unsigned int cap);
extern "C" __declspec(dllimport) void __stdcall glDisable(unsigned int cap);
extern "C" __declspec(dllimport) void __stdcall glColorMask(unsigned char r, unsigned char g,
                                                            unsigned char b, unsigned char a);
extern "C" __declspec(dllimport) void __stdcall glStencilOp(unsigned int fail, unsigned int zfail,
                                                            unsigned int zpass);
extern "C" __declspec(dllimport) void __stdcall glStencilMask(unsigned int mask);
extern "C" __declspec(dllimport) void __stdcall glStencilFunc(unsigned int func, int ref,
                                                              unsigned int mask);

extern "C" __declspec(dllimport) void __stdcall glBlendFunc(unsigned int sfactor, unsigned int dfactor);
extern "C" __declspec(dllimport) void __stdcall glEnableClientState(unsigned int array);
extern "C" __declspec(dllimport) void __stdcall glDisableClientState(unsigned int array);
extern "C" __declspec(dllimport) void __stdcall glColor4f(float r, float g, float b, float a);
extern "C" __declspec(dllimport) void __stdcall glMaterialfv(unsigned int face, unsigned int pname,
                                                             const float* params);
extern "C" __declspec(dllimport) void __stdcall glClearStencil(int s);
extern "C" __declspec(dllimport) void __stdcall glClear(unsigned int mask);
extern "C" __declspec(dllimport) void __stdcall glPushAttrib(unsigned int mask);
extern "C" __declspec(dllimport) void __stdcall glPopAttrib();
extern "C" __declspec(dllimport) void __stdcall glDepthFunc(unsigned int func);
extern "C" __declspec(dllimport) void __stdcall glLightModelfv(unsigned int pname, const float* params);
extern "C" __declspec(dllimport) void __stdcall glDepthMask(unsigned char flag);
extern "C" __declspec(dllimport) void __stdcall glPushMatrix();
extern "C" __declspec(dllimport) void __stdcall glPopMatrix();
extern "C" __declspec(dllimport) void __stdcall glMultMatrixf(const float* m);
extern "C" __declspec(dllimport) void __stdcall glAlphaFunc(unsigned int func, float ref);
extern "C" __declspec(dllimport) void __stdcall glFogf(unsigned int pname, float param);
extern "C" __declspec(dllimport) void __stdcall glFogfv(unsigned int pname, const float* params);
extern "C" __declspec(dllimport) void __stdcall glColorMaterial(unsigned int face, unsigned int mode);

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList() { if (element) delete[] element; }
    CExoArrayList(const CExoArrayList<T>& other);               // 0x0046D250 (int)
    CExoArrayList<T>& operator=(const CExoArrayList<T>& other); // 0x0046BC80

    int Contains(T t)
    {
        int nCount = 0;
        for (int i = 0; i < num; i++) {
            if (element[i] == t)
                nCount++;
        }
        return nCount;
    }

    void Allocate(int n);
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = t;
    }
};

template <class T>
void CExoArrayList<T>::Allocate(int n)
{
    T* pOld = element;
    array_size = n;
    element = new T[n];
    for (int i = 0; i < num; i++)
        element[i] = pOld[i];
    if (pOld)
        delete[] pOld;
}

struct Vector {
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};


inline Vector operator-(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

struct Quaternion {
    float w, x, y, z;

    Quaternion() { z = 0.0f; y = 0.0f; x = 0.0f; w = 1.0f; }
    void GetMatrix(float* m);                   // 0x0044F470
};

class Plane {
public:
    void Transform(const Vector& vTranslation, const Quaternion& qRotation);  // 0x004AB4C0

    Vector n;
    float d;
};

Quaternion operator-(const Quaternion& q);                                 // 0x004A9B50
Vector operator*(const Quaternion& q, const Vector& v);                    // 0x004A9BB0
void MakeMatrix4x4(float* m, const Vector& v, const Quaternion& q);        // 0x004AA9C0

class Material {
public:
    void BindTexture0();                        // 0x0047ADF0
    void SetBlendingMode(int nMode);            // 0x0047AF70
};

// Per-part lightmap state reset after the mesh pass (Part +0x80).
struct MeshLightmap {
    char pad_0x0[0x50];
    float m_fFade;                              // +0x50, descriptive
    float m_fBaseFade;                          // +0x54, descriptive
    char pad_0x58[0x8c - 0x58];
    int m_n8C;                                  // +0x8c, descriptive
    char pad_0x90[4];
    float m_fFogFade;                           // +0x94, descriptive
};

class CHitInfo {
public:
    CHitInfo(int nType, int nMask, int nFlags); // 0x004578F0
    ~CHitInfo();                                // 0x00456B30
    int m_n00;                                  // +0x00
    char pad_0x4[0x64 - 0x4];
};

Vector operator+(const Vector& a, const Vector& b);                        // 0x00423B90

class Part {
public:
    virtual void V00(); virtual void V04(); virtual void V08();
    virtual void GetWorldTransform(Vector* pPosition, Quaternion* pOrientation); // +0x0c, descriptive
    virtual void V10(); virtual void V14(); virtual void V18(); virtual void V1C();
    virtual void V20(); virtual void V24(); virtual void V28(); virtual void V2C();
    virtual void V30(); virtual void V34(); virtual void V38(); virtual void V3C();
    virtual void V40(); virtual void V44(); virtual void V48(); virtual void V4C();
    virtual void V50(); virtual void V54(); virtual void V58();
    virtual void ProjectShadow(Vector* pLightPos, float fRadius, Plane* pPlane, int nFlags); // +0x5c, descriptive
    virtual void Prepare();                     // +0x60
    virtual void Draw(int);                     // +0x64
    virtual void V68(); virtual void V6C(); virtual void V70(); virtual void V74();
    virtual void V78();
    virtual void DrawVolume();                  // +0x7c, descriptive
    virtual int InRange(Vector vPosition, float fRadius); // +0x80, descriptive
    char pad_0x4[4];
    Vector m_vPosition;                         // +0x08
    Quaternion m_qOrientation;                  // +0x14
    char pad_0x24[0x44 - 0x24];
    Material* m_pMaterial;                      // +0x44
    char pad_0x48[0x80 - 0x48];
    MeshLightmap* m_pLightmap;                  // +0x80, descriptive
    float m_fAlpha;                             // +0x84
    char pad_0x88[0xa8 - 0x88];
    unsigned char m_bA8;                        // +0xa8
    char pad_0xa9[7];
    unsigned char m_bHighlightGroup;            // +0xb0, descriptive
    char pad_0xb1[3];
    float m_aColor[3];                          // +0xb4
    float m_fPulseAmplitude;                    // +0xc0, descriptive
    unsigned char m_bPulse;                     // +0xc4, descriptive
    char pad_0xc5[3];
    float m_fPulseRate;                         // +0xc8, descriptive
};

class Gob;

class PartEmitter {
public:
    char pad_0x0[0x40];
    Gob* m_pGob;                                // +0x40

    void AnimateParticles(float dt);            // 0x00494DA0
    void RenderFunc();                          // 0x0049B680
    static void StartEmitterLayer();            // 0x0048EFA0
    static void EndEmitterLayer();              // 0x0048F000
};

extern "C" void __cdecl qsort(void* base, unsigned int num, unsigned int width,
                              int (__cdecl* compare)(const void*, const void*));
int __cdecl EmitterOrderCmp(const void* a, const void* b);   // 0x0044F5A0
int __cdecl GobOrderCmp(const void* a, const void* b);       // 0x00450270
extern "C" int __cdecl _strnicmp(const char* a, const char* b, unsigned int n);

// Room emitter list (room +0x58, list at +0x24).
struct RoomEmitters {
    char pad_0x0[0x24];
    CExoArrayList<PartEmitter*> m_lEmitters;    // +0x24
};

class SceneRoom {
public:
    char pad_0x0[0x58];
    RoomEmitters* m_pEmitters;                  // +0x58, descriptive
};

class TriangleBin {
public:
    void RenderGrassPolys(unsigned int material, Vector* eye, void* wind); // 0x004A6D30
};

class RenderCamera {
public:
    char pad_0x0[0x7c];
    Vector m_vPosition;                         // +0x7c
    Quaternion m_qOrientation;                  // +0x88
    char pad_0x98[0x1f8 - 0x98];
    CExoArrayList<Plane> m_lViewVolume;         // +0x1f8
};

class Scene;
class Gob;
class Light;

__int64 AurGetTime();                           // 0x0044F320
bool AurGLCaps640();                            // 0x0045F640
void AurSetVertexProgramParameter(unsigned int nIndex, float x, float y, float z, float w); // 0x0044F820
float* AurMakeColor(const Vector& v, const float& a);   // 0x004AAAB0
void RenderPersonalSpace(Scene* pScene);        // 0x00450FA0
void RenderPaths(Scene* pScene);                // 0x004510E0
void RenderAllSquares();                        // 0x00432720
void SetFilmNoiseTexture(void* pTexture);       // 0x00426650
void FrameBufferModifications(int nFlags);      // 0x00437520, descriptive
void FrameBufferModifications2(int nFlags);     // 0x004361F0, descriptive
void AurTextureMakeCurrent();                   // 0x00421EE0
void AurTextureUpdateProcedural();              // 0x00420E50
void ManageSceneBSP(Scene* pScene);             // 0x0046E5C0
int AurGetStencilClearValue();                  // 0x0045FD20
int AurGetAvailableStencilBits();               // 0x0045FD00
bool AurNV2xLevelShadersAvailable();            // 0x0045FBD0
int AurGLCaps7B0();                             // 0x0045F7B0
int AurGLCaps7E0();                             // 0x0045F7E0
void SetUpSoftShadows();                        // 0x0042D1C0
void FinishSoftShadows();                       // 0x0042D9A0
void AurVertexProgramMakeCurrent();             // 0x004A23B0
int AurGLCaps710();                             // 0x0045F710, multitexture available
Part* FindPart(Part* pRoot, char* szName);      // 0x0043FE00
float Magnitude(const Vector& v);               // 0x004AB100
void DoGobShadow(Gob* pGob, Light* pLight);     // 0x0044FF50, descriptive
void _glBlendFunc(int nSrc, int nDst);          // 0x00401670

struct VertexPC {
    float x, y, z;
    unsigned int color;
};

void DrawVerticesPC(int nMode, VertexPC* pVertices, int nCount);   // 0x00431F70
unsigned char AurGrassMaterialInUse(unsigned int material); // 0x004A6840


struct GobModel {
    char pad_0x0[0x50];
    unsigned short m_bb0 : 1;                   // +0x50
    unsigned short m_bb1 : 1;
    unsigned short m_bNoDepthTest : 1;          // bit 2
    unsigned short m_bb3 : 1;
    unsigned short m_bb4 : 1;
    unsigned short m_bb5 : 1;
    unsigned short m_bb6 : 1;

    unsigned char NoDepthTest() { return m_bNoDepthTest; }
    unsigned char GetB5() { return m_bb5; }
    bool GetB6() { return m_bb6; }
};

// Object at Gob +0x188; only its +0x14 query is used here.
class GobController {
public:
    virtual void V00(); virtual void V04(); virtual void V08(); virtual void V0C();
    virtual void V10();
    virtual int IsActive();                     // +0x14, descriptive
};

class Gob {
public:
    virtual void V000();
    virtual void V004();
    virtual void V008();
    virtual void V00C();
    virtual void Render(int bPropagate);        // +0x10
    virtual void Animate(float fDeltaT);        // +0x14
    virtual void V018(); virtual void V01C(); virtual void V020();
    virtual void V024(); virtual void V028(); virtual void V02C(); virtual void V030();
    virtual void V034(); virtual void V038(); virtual void V03C(); virtual void V040();
    virtual void V044(); virtual void V048(); virtual void V04C(); virtual void V050();
    virtual void V054(); virtual void V058(); virtual void V05C(); virtual void V060();
    virtual void V064(); virtual void V068(); virtual void V06C(); virtual void V070();
    virtual void V074(); virtual void V078(); virtual void V07C(); virtual void V080();
    virtual void V084(); virtual void V088(); virtual void V08C(); virtual void V090();
    virtual void V094(); virtual void V098(); virtual void V09C(); virtual void V0A0();
    virtual void V0A4(); virtual void V0A8(); virtual void V0AC(); virtual void V0B0();
    virtual void V0B4(); virtual void V0B8(); virtual void V0BC(); virtual void V0C0();
    virtual void V0C4(); virtual void V0C8(); virtual void V0CC(); virtual void V0D0();
    virtual void V0D4(); virtual void V0D8(); virtual void V0DC(); virtual void V0E0();
    virtual void V0E4(); virtual void V0E8(); virtual void V0EC(); virtual void V0F0();
    virtual void V0F4(); virtual void V0F8(); virtual void V0FC(); virtual void V100();
    virtual void V104(); virtual void V108(); virtual void V10C(); virtual void V110();
    virtual void V114(); virtual void V118(); virtual void V11C(); virtual void V120();
    virtual void V124(); virtual void V128(); virtual void V12C(); virtual void V130();
    virtual void V134(); virtual void V138(); virtual void V13C(); virtual void V140();
    virtual void V144(); virtual void V148();
    virtual void RenderBlur();                  // +0x14c
    virtual void V150(); virtual void V154(); virtual void V158(); virtual void V15C();
    virtual void V160(); virtual void V164(); virtual void V168(); virtual void V16C();
    virtual bool IsForceAnimated();             // +0x170, descriptive
    virtual void V174(); virtual void V178(); virtual void V17C(); virtual void V180();
    virtual void V184(); virtual void V188(); virtual void V18C();
    virtual unsigned char GetFlag12();          // +0x190

    unsigned char VisibilityCheck();            // 0x004579C0
    unsigned char IsInCutscene();               // 0x0043E8A0
    char pad_0x4[0x18 - 0x4];
    bool m_bVisible;                            // +0x18, descriptive
    char pad_0x19[3];
    float m_fVisibilityTime;                    // +0x1c, descriptive
    char pad_0x20[0x38 - 0x20];
    char m_szName[0x20];                        // +0x38
    GobModel* m_pModel;                         // +0x58
    char pad_0x5c[0x68 - 0x5c];
    Scene* m_pScene;                            // +0x68
    Part* m_pRootPart;                          // +0x6c
    char pad_0x70[8];
    Vector m_vPosition;                         // +0x78
    Quaternion m_qOrientation;                  // +0x84
    char pad_0x94[0xc2 - 0x94];
    unsigned short m_nC2;                       // +0xc2, descriptive
    char pad_0xc4[0x138 - 0xc4];
    float m_fRadius;                            // +0x138
    Vector m_vCenter;                           // +0x13c
    char pad_0x148[0x166 - 0x148];
    unsigned char m_b166;                       // +0x166, descriptive
    char pad_0x167[0x178 - 0x167];
    int m_nLightMask;                           // +0x178
    char pad_0x17c[0x188 - 0x17c];
    GobController* m_p188;                      // +0x188, descriptive
};

// Scene light manager interface (Scene +0x80).
class SceneLights {
public:
    virtual void PrioritizeLights(Vector* pos, int n);          // +0x00
    virtual void V04();
    virtual void ResetLights();                                 // +0x08, descriptive
    virtual void V0C();
    virtual void V10(); virtual void V14(); virtual void V18(); virtual void V1C();
    // MSVC places overloaded virtuals in reverse declaration order.
    virtual void EnableHighestPriorityLights(int n, int mask);  // +0x24
    virtual void EnableHighestPriorityLights(int n);            // +0x20
    virtual void AddFocusLight(int n, Vector pos);             // +0x28, descriptive
    virtual void V2C(); virtual void V30(); virtual void V34();
    virtual void SetupLightsForThisGob(Vector* pos, float radius); // +0x38
    CExoArrayList<Light*>* GetLights(int nType);                // 0x004A4B10
    char pad_0x4[0xdc];
    int m_nE0;                                  // +0xe0
    char pad_0xe4[8];
    int m_nEC;                                  // +0xec
};

class Scene;
class Gob;
class Light;

class WindManager {
public:
    void Update(float fDeltaT);                 // 0x004A5E30, descriptive
};

// Object embedded at Scene +0xf8.
class SceneView {
public:
    virtual void V00(); virtual void V04(); virtual void V08(); virtual void V0C();
    virtual void V10(); virtual void V14(); virtual void V18(); virtual void V1C();
    virtual void V20(); virtual void V24(); virtual void V28(); virtual void V2C();
    virtual void V30(); virtual void V34(); virtual void V38(); virtual void V3C();
    virtual void V40(); virtual void V44(); virtual void V48(); virtual void V4C();
    virtual void V50(); virtual void V54(); virtual void V58();
    virtual Vector SetFocus(Vector v);          // +0x5c, descriptive
};

class Light {
public:
    char pad_0x0[0x58];
    float m_fRadius;                            // +0x58
    char pad_0x5c[4];
    float m_fShadowRadius;                      // +0x60
    char pad_0x64[0x30];
    Vector m_vPosition;                         // +0x94

    float GetShadowRadius()
    {
        float fRadius = m_fRadius;
        if (m_fShadowRadius > 0.0f)
            fRadius = m_fShadowRadius;
        return fRadius;
    }
};

struct ShadowSource {
    char pad_0x0[0x1c];
    Light* m_pLight;                            // +0x1c
    float m_fAlpha;                             // +0x20
};

// Minimal Scene: the vtable slots used here (vtable 0x00741708).
class Scene {
public:
    virtual void V000(); virtual void V004(); virtual void V008(); virtual void V00C();
    virtual void V010(); virtual void V014();
    virtual void Render();                      // +0x18
    virtual void V01C();
    virtual void V020(); virtual void V024(); virtual void V028(); virtual void V02C();
    virtual void V030(); virtual void V034(); virtual int HitCheck(Vector vStart, Vector vEnd, Vector* pHit, CHitInfo* pInfo); // +0x38
    virtual void V03C();
    virtual void V040(); virtual void V044(); virtual void V048(); virtual void V04C();
    virtual void V050(); virtual void V054(); virtual void V058(); virtual void V05C();
    virtual void V060(); virtual void V064(); virtual void V068(); virtual void V06C();
    virtual void V070(); virtual void V074(); virtual void V078(); virtual void V07C();
    virtual void V080(); virtual void V084(); virtual void V088(); virtual void V08C();
    virtual void V090(); virtual void V094(); virtual void V098(); virtual void V09C();
    virtual void V0A0(); virtual void V0A4();
    virtual bool IsAnimating();                 // +0xa8
    virtual void V0AC();
    virtual void V0B0(); virtual void V0B4();
    virtual void RenderSinglePass();            // +0xb8
    virtual void RenderShadows(int a, int b, int bCast, int bClear); // +0xbc
    virtual void RenderDynamicGeometry();       // +0xc0
    virtual void RenderStaticGeometry();        // +0xc4
    virtual void RenderLensFlares();            // +0xc8
    virtual void RenderTransparentGeometry();   // +0xcc
    virtual void V0D0(); virtual void V0D4(); virtual void DoMeshBuckets();      // +0xd8
    virtual void DoBlurBuckets();               // +0xdc
    virtual void DoGobBuckets();                // +0xe0
    virtual void DoDistortionBuckets();         // +0xe4
    virtual void DoLightSaberBucket();          // +0xe8
    virtual void V0EC();
    virtual void DoBinBucket();                 // +0xf0
    virtual void DoFadeBuckets();               // +0xf4
    virtual void DoGobShadows();                // +0xf8
    virtual void DoGobShadowsForThisLight(Light* pLight);              // +0xfc
    virtual void RenderStencilShadowsForThisLight(Light* pLight, float fAlpha, int nStencilBit); // +0x100
    virtual void V104(); virtual void V108();
    virtual void DoDynamicMeshProjections();    // +0x10c
    virtual void DoEmitterBucket();             // +0x110
    virtual void DoTriggerBucket();             // +0x114
    virtual void DoHighlightTriggers(unsigned char bFlag);    // +0x118
    virtual void V11C();
    virtual void RenderOnlyGobs();              // +0x120
    virtual void DoFog();                       // +0x124
    char pad_0x4[0x4c];
    int m_nFrames;                              // +0x50
    CExoArrayList<Gob*> m_lChildren;            // +0x54
    int m_bFogEnabled;                          // +0x60
    float m_fFogStart;                          // +0x64
    float m_fFogEnd;                            // +0x68
    char pad_0x6c[4];
    float m_fFogDensity;                        // +0x70
    Vector m_vFogColor;                         // +0x74
    SceneLights* m_pLights;                     // +0x80
    WindManager* m_pWind;                       // +0x84
    Vector m_vAmbient;                          // +0x88
    CExoArrayList<PartEmitter*> m_lEmitters;    // +0x94
    int m_nA0;                                  // +0xa0, descriptive
    char pad_0xa4[4];
    Vector m_vFocus;                            // +0xa8, descriptive
    float m_fHighlightTime;                     // +0xb4, descriptive
    char pad_0xb8[0xc8 - 0xb8];
    CExoArrayList<SceneRoom*> m_lRooms;         // +0xc8
    int m_bFrameBufferEffects;                  // +0xd4, descriptive
    char pad_0xd8[0xc];
    void* m_pFilmNoiseTexture;                  // +0xe4
    void* m_pDistortionTexture;                 // +0xe8
    char pad_0xec[0xc];
    SceneView m_View;                           // +0xf8, descriptive
    char pad_0xfc[0x270 - 0xfc];
    ShadowSource* m_pShadow;                    // +0x270, descriptive

    void PreWarm();                             // 0x004511F0, descriptive
    float GetFogStart() { return m_fFogStart; }
    float GetFogEnd() { return m_fFogEnd; }
    float GetFogDensity() { return m_fFogDensity; }
    __forceinline void DoFadePart(Part* pPart, CExoArrayList<MeshLightmap*>& lLightmaps, float& fAlpha);
};

extern unsigned int g_nGLStencilTest;           // 0x0078D470
extern unsigned int g_nGLBlend;                 // 0x0078D448
extern unsigned int g_nGLKeep;                  // 0x0073F2C8
extern unsigned int g_nGLReplace;               // 0x0073F2D0
extern unsigned int g_nGLAlways;                // 0x0073F2BC
extern void (*g_pfnBlurSetup)();                // 0x0078E39C, descriptive
extern int g_nBlurBucketCount;                  // 0x007B92EC, descriptive
extern CExoArrayList<Gob*> g_l7FBFC0;           // 0x007FBFC0 distortion bucket
extern CExoArrayList<Gob*> g_l7FBFF0;           // 0x007FBFF0 light saber bucket
extern CExoArrayList<Gob*> g_l7FBF90;           // 0x007FBF90 blur bucket

typedef void (__stdcall* PFNCLIENTACTIVETEXTURE)(unsigned int texture);

extern int g_bRenderBins;                       // 0x0078E3AC, descriptive
extern unsigned int g_nGLCullFace;              // 0x0078D454
extern unsigned int g_aBlendFactors[];          // 0x0073F270
extern PFNCLIENTACTIVETEXTURE g_pfnClientActiveTexture; // 0x007BB6B0
extern unsigned int g_nActiveTextureUnit;       // 0x007A6898
extern int g_bBlackFog;                         // 0x008305E0
extern unsigned int g_eColorArray;              // 0x0073F3A0
extern unsigned int g_eAlphaFunc;               // 0x0073F2B0
extern float g_fBinAlphaRef;                    // 0x0078E3B0, descriptive
extern float g_fAlphaRef;                       // 0x007B92E8, descriptive
extern int g_nGrassMaterials;                   // 0x00831E48, descriptive
extern CExoArrayList<TriangleBin*> g_l80AA68;   // 0x0080AA68 triangle bins
extern RenderCamera* g_pRenderCamera;
// File-scope in the original scene unit: MSVC knows no pointer aliases
// them, so it delays the high-dword stores past member loads.
static __int64 g_nAnimStartTime;               // 0x007B9298
static __int64 g_nAnimEndTime;                  // 0x007B9288
extern float g_fMeshBucketSeconds;              // 0x007B92B0, descriptive
extern float g_fBinBucketSeconds;               // 0x007B92B4, descriptive
extern unsigned int g_eGLFront;                 // 0x0073F2EC
extern unsigned int g_eGLAmbient;               // 0x0073F2FC
extern unsigned int g_eGLDiffuse;               // 0x0073F300
extern unsigned int g_eGLEmission;              // 0x0073F304
extern int g_bRenderEmitters;                   // 0x0078E3D8, descriptive
extern CExoArrayList<PartEmitter*> g_l7FBF78;   // 0x007FBF78 emitter bucket
extern float g_fFrameTime;                      // 0x0078E574, descriptive
extern Scene* g_pRenderScene;                   // 0x007B935C, descriptive
static __int64 g_nRenderStartTime;              // 0x007B9280, descriptive
static __int64 g_nRenderEndTime;                // 0x007B9290, descriptive
extern float g_fRenderSeconds;                  // 0x007B92D4, descriptive
extern int g_bAnimCulling;                      // 0x0078E3B8
extern unsigned char g_bAnimVisibleOnly;        // 0x007B92F4
extern int g_nRenderFrame3;                     // 0x0078E3F8, descriptive
extern int g_bRenderScenes;                     // 0x0078E3F4, descriptive
extern int g_n7B92E0;                           // 0x007B92E0
extern int g_n7B9310;                           // 0x007B9310
extern Scene* g_pAnimatingScene;                // 0x007B92A0
extern unsigned int g_eGLLequal;                // 0x0073F2AC
extern unsigned int g_nGLAlphaTest;             // 0x0078D44C
extern int g_nStatA;                            // 0x007B9340, descriptive
extern int g_nStatB;                            // 0x007B9344, descriptive
extern int g_nBindCountPeak;                    // 0x007A46B0
extern int g_nStatTotalPeak;                    // 0x007B9348, descriptive
extern int g_nStatAPeak;                        // 0x007B9338, descriptive
extern int g_nStatBPeak;                        // 0x007B933C, descriptive
typedef void (__stdcall* PFNPROGRAMPARAMETER)(unsigned int target, unsigned int index,
                                              float x, float y, float z, float w);
extern int g_bARBVertexProgram;                 // 0x0078E5EC
extern PFNPROGRAMPARAMETER g_pfnProgramEnvParameter4fARB;  // 0x007BB850
extern PFNPROGRAMPARAMETER g_pfnProgramParameter4fNV;      // 0x007BB7AC
extern int g_bRenderStatic;                     // 0x0078E400, descriptive
extern int g_bRenderDynamic;                    // 0x0078E3FC, descriptive
extern int g_bStencilTriggers;                  // 0x007B92E4, descriptive
extern float g_fTriggerSeconds;                 // 0x007B92B8, descriptive
extern float g_fShadowSeconds;                  // 0x007B92BC, descriptive
extern float g_fDynamicSeconds;                 // 0x007B92C4, descriptive
extern float g_fPathSeconds;                    // 0x007B92C0, descriptive
extern float g_fEmitterSeconds;                 // 0x007B92C8, descriptive
extern float g_fPostSeconds;                    // 0x007B92CC, descriptive
extern void* g_pDistortionTexture;              // 0x007A68D8
extern unsigned int g_nGLFog;                   // 0x0078D450
extern int g_bSoftShadows;                      // 0x0078E420
extern unsigned char g_bSoftShadowsAllowed;     // 0x0078DAE9
extern int g_bGobBucketFlag;                    // 0x007B92FC, descriptive
extern CExoArrayList<Gob*> g_l7FBFCC;           // 0x007FBFCC gob bucket
extern int g_bGobDepthToggle;                   // 0x007B92F0, descriptive
extern int g_bFocusLights;                      // 0x007B9300, descriptive
extern int g_bRenderGobs;                       // 0x0078E3C4, descriptive
extern int g_bShadowDistanceCheck;              // 0x0078E3CC, descriptive
extern float g_fShadowDistance;                 // 0x0078E3C8, descriptive
extern int g_bStencilDynamic;                   // 0x0078E3A8, descriptive
extern int g_bRenderFlat;                       // 0x0078E614
extern unsigned int g_eTexCoordArray;           // 0x0073F398
extern unsigned int g_eVertexArray;             // 0x0073F3A4
extern unsigned int g_eNormalArray;             // 0x0073F39C
extern unsigned int g_nGLLighting;              // 0x0078D474
extern unsigned int g_nGLTexture2D;             // 0x0078D458
extern unsigned int g_nGLDepthTest;             // 0x0078D45C
extern CExoArrayList<Part*> g_lAllMeshParts;     // 0x007FBFB4
extern CExoArrayList<Part*> g_lTransparentParts; // 0x00827F40
extern CExoArrayList<Part*> g_aMeshBuckets[];    // 0x008194E0
extern CExoArrayList<Part*> g_aFadeBuckets[];    // 0x0080AA80

extern unsigned int g_eGLFogStart;              // 0x0078D484
extern unsigned int g_eGLFogEnd;                // 0x0078D488
extern unsigned int g_eGLFogDensity;            // 0x0078D48C
extern unsigned int g_eGLFogColor;              // 0x0078D490
extern unsigned int g_eGLEmissionMode;          // 0x0073F30C
extern const int g_nMeshLightmapReset;          // 0x00740A70, descriptive
CExoArrayList<int>* AurTextureGetOrdering();     // 0x00421970
extern int g_bVisibilityTest;                   // 0x007B92F8, descriptive
extern float g_fVisibilityRatio;                // 0x0078E3C0, descriptive
extern float g_fVisibilityInterval;             // 0x0078E3BC, descriptive
extern float g_fMinProjectionRadius;            // 0x0078E3D4, descriptive
extern CExoArrayList<Part*> g_l80AA74;          // 0x0080AA74 trigger parts           // 0x007BB4E8
extern unsigned char g_bTextureStateDirty;      // 0x0078E330, descriptive

// FUNCTION: SWKOTOR 0x00450150
void Scene::DoBinBucket()
{
    if (!g_bRenderBins)
        return;
    glEnable(g_nGLBlend);
    glBlendFunc(g_aBlendFactors[0], g_aBlendFactors[1]);
    glDisable(g_nGLCullFace);
    if (AurGLCaps710())
        g_pfnClientActiveTexture(0x84c0);
    g_nActiveTextureUnit = 0;
    if (g_bBlackFog)
        glEnableClientState(g_eColorArray);
    glDisableClientState(g_eColorArray);
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    glAlphaFunc(g_eAlphaFunc, g_fBinAlphaRef);
    for (int material = 0; material < g_nGrassMaterials; material++) {
        if (!AurGrassMaterialInUse(material))
            continue;
        for (int i = 0; i < g_l80AA68.num; i++)
            g_l80AA68.element[i]->RenderGrassPolys(material, &g_pRenderCamera->m_vPosition, m_pWind);
    }
    glAlphaFunc(g_eAlphaFunc, g_fAlphaRef);
    g_bTextureStateDirty = 1;
    glEnable(g_nGLCullFace);
}

static __forceinline void ClearStencil()
{
    int nBits = AurGetAvailableStencilBits();
    if (nBits)
        glClearStencil((1 << (nBits - 1)) - 1);
    glStencilMask(0xff);
    glClear(0x400);
}

static __forceinline float LapSeconds()
{
    g_nAnimEndTime = AurGetTime();
    return (float)(int)(g_nAnimEndTime - g_nAnimStartTime) * 0.001f;
}

// FUNCTION: SWKOTOR 0x004514F0
void Scene::RenderSinglePass()
{
    if (AurNV2xLevelShadersAvailable()) {
        float m[9];
        g_pRenderCamera->m_qOrientation.GetMatrix(m);
        Vector pos = g_pRenderCamera->m_vPosition;
        AurSetVertexProgramParameter(0x5c, m[0], m[3], m[6], pos.x);
        AurSetVertexProgramParameter(0x5b, m[1], m[4], m[7], pos.y);
        AurSetVertexProgramParameter(0x5a, m[2], m[5], m[8], pos.z);
        if (g_bARBVertexProgram)
            g_pfnProgramEnvParameter4fARB(0x8620, 0x59, 0.0f, 0.0f, 0.0f, 1.0f);
        else
            g_pfnProgramParameter4fNV(0x8620, 0x59, 0.0f, 0.0f, 0.0f, 1.0f);
    }
    DoFog();
    glEnable(g_nGLBlend);
    glBlendFunc(g_aBlendFactors[0], g_aBlendFactors[1]);
    glDisable(g_nGLStencilTest);
    glDisableClientState(g_eColorArray);
    glDisable(0xb57);
    g_nAnimStartTime = AurGetTime();
    {
        float a = 1.0f;
        Vector zero(0.0f, 0.0f, 0.0f);
        glLightModelfv(0xb53, AurMakeColor(zero, a));
    }
    glDepthMask(1);
    if (g_bRenderStatic)
        RenderStaticGeometry();
    {
        float a = 1.0f;
        glLightModelfv(0xb53, AurMakeColor(m_vAmbient, a));
    }
    glDisableClientState(g_eColorArray);
    glDisable(0xb57);
    if (g_bStencilTriggers) {
        ClearStencil();
        DoTriggerBucket();
        DoHighlightTriggers(0);
    }
    g_fTriggerSeconds = LapSeconds();
    g_nAnimStartTime = AurGetTime();
    ClearStencil();
    if (m_bFrameBufferEffects && g_bStencilDynamic)
        RenderShadows(0, 1, 1, 0);
    g_fShadowSeconds = LapSeconds();
    g_nAnimStartTime = AurGetTime();
    if (g_bRenderDynamic)
        RenderDynamicGeometry();
    g_fDynamicSeconds = LapSeconds();
    g_nAnimStartTime = AurGetTime();
    RenderPersonalSpace(this);
    RenderPaths(this);
    g_fPathSeconds = LapSeconds();
    g_nAnimStartTime = AurGetTime();
    if (g_bStencilTriggers) {
        ClearStencil();
        DoHighlightTriggers(1);
    }
    g_fTriggerSeconds = LapSeconds() + g_fTriggerSeconds;
    g_nAnimStartTime = AurGetTime();
    DoEmitterBucket();
    DoLightSaberBucket();
    RenderAllSquares();
    g_fEmitterSeconds = LapSeconds();
    g_nAnimStartTime = AurGetTime();
    RenderTransparentGeometry();
    DoDistortionBuckets();
    SetFilmNoiseTexture(m_pFilmNoiseTexture);
    g_pDistortionTexture = m_pDistortionTexture;
    if (m_bFrameBufferEffects) {
        int nFlags = 0x1f;
        if (g_l7FBFC0.num > 0)
            nFlags = 0x3f;
        FrameBufferModifications(nFlags);
    }
    RenderLensFlares();
    if (m_bFrameBufferEffects) {
        glDisable(g_nGLLighting);
        glDisable(g_nGLDepthTest);
        FrameBufferModifications2(0x1f);
    }
    glDisable(g_nGLStencilTest);
    glDisable(g_nGLDepthTest);
    glEnable(g_nGLAlphaTest);
    glEnable(g_nGLLighting);
    glEnable(g_nGLBlend);
    glDisable(g_nGLFog);
    glDisableClientState(g_eNormalArray);
    glDisableClientState(g_eTexCoordArray);
    glDisableClientState(g_eVertexArray);
    glDisableClientState(g_eColorArray);
    glDisable(0xb57);
    g_fPostSeconds = LapSeconds();
    g_nAnimStartTime = AurGetTime();
}

// FUNCTION: SWKOTOR 0x004512D0
void Scene::Render()
{
    AurGLCaps640();
    g_pRenderScene = this;
    g_nRenderStartTime = AurGetTime();
    if (!m_nFrames)
        PreWarm();
    if (!g_bAnimCulling || !g_bAnimVisibleOnly)
        m_nFrames++;
    g_nRenderFrame3 = (g_nRenderFrame3 + 1) % 3;
    if (g_bRenderScenes) {
        g_n7B92E0 = 0;
        g_n7B9310 = 0;
        m_View.SetFocus(m_vFocus);
        g_bTextureStateDirty = 1;
        AurTextureMakeCurrent();
        AurTextureUpdateProcedural();
        g_pAnimatingScene = this;
        ManageSceneBSP(this);
        glPushAttrib(0x6080);
        glDepthFunc(g_eGLLequal);
        glEnableClientState(g_eNormalArray);
        glEnableClientState(g_eTexCoordArray);
        glEnableClientState(g_eVertexArray);
        glEnable(g_nGLAlphaTest);
        glAlphaFunc(g_eAlphaFunc, g_fAlphaRef);
        glEnable(g_nGLDepthTest);
        if (!m_nA0 && !m_lRooms.num) {
            RenderOnlyGobs();
            g_pAnimatingScene = 0;
            glPopAttrib();
        } else {
            m_pWind->Update(g_fFrameTime);
            RenderSinglePass();
            glPopAttrib();
            g_pAnimatingScene = 0;
            int nTotal = g_nStatB + g_nStatA + g_nBindCountPeak;
            if (g_nStatTotalPeak < nTotal)
                g_nStatTotalPeak = nTotal;
            if (g_nStatAPeak < g_nStatA)
                g_nStatAPeak = g_nStatA;
            g_nStatA = 0;
            if (g_nStatBPeak < g_nStatB)
                g_nStatBPeak = g_nStatB;
            g_nStatB = 0;
            IsAnimating();
            g_nRenderEndTime = AurGetTime();
            g_fRenderSeconds = (float)(int)(g_nRenderEndTime - g_nRenderStartTime) * 0.001f;
        }
    }
    if (g_bAnimCulling && g_bAnimVisibleOnly)
        m_nFrames++;
}

static __forceinline bool UseSoftShadows()
{
    return g_bSoftShadows && AurNV2xLevelShadersAvailable()
        && (AurGLCaps7B0() || AurGLCaps7E0()) && g_bSoftShadowsAllowed;
}

// FUNCTION: SWKOTOR 0x00451A50
void Scene::RenderShadows(int a, int b, int bCast, int bClear)
{
    int nStencilBits = AurGetAvailableStencilBits();
    if (!nStencilBits)
        return;
    glDisable(g_nGLTexture2D);
    glDisable(g_nGLAlphaTest);
    glDisable(g_nGLDepthTest);
    glStencilMask(0xff);
    glEnable(g_nGLStencilTest);
    if (bClear)
        glClear(0x400);
    if (bCast) {
        Light* pLight = m_pShadow->m_pLight;
        if (pLight && m_vFocus.z + 1.75f < pLight->m_vPosition.z) {
            if (UseSoftShadows())
                SetUpSoftShadows();
            glStencilMask(0xff);
            glEnable(g_nGLStencilTest);
            glDepthMask(0);
            glColorMask(0, 0, 0, 0);
            glStencilFunc(g_nGLAlways, 0xf, 0xf);
            glDisableClientState(g_eNormalArray);
            glDisableClientState(g_eTexCoordArray);
            glDisableClientState(g_eColorArray);
            DoGobShadowsForThisLight(pLight);
            glEnableClientState(g_eNormalArray);
            glEnableClientState(g_eTexCoordArray);
            glEnableClientState(g_eColorArray);
            glColorMask(1, 1, 1, 1);
            glStencilMask(0);
            RenderStencilShadowsForThisLight(pLight, m_pShadow->m_fAlpha, nStencilBits);
            if (UseSoftShadows())
                FinishSoftShadows();
            glStencilMask(0xff);
        }
    }
    glEnableClientState(g_eNormalArray);
    glEnableClientState(g_eTexCoordArray);
    if (g_bBlackFog)
        glEnableClientState(g_eColorArray);
    glDepthMask(1);
    glEnable(g_nGLTexture2D);
    glEnable(g_nGLAlphaTest);
    glEnable(g_nGLDepthTest);
    glDisable(g_nGLStencilTest);
}

// FUNCTION: SWKOTOR 0x00450C30
void Scene::RenderStaticGeometry()
{
    AurVertexProgramMakeCurrent();
    m_pLights->ResetLights();
    float material[8];
    material[0] = 1.0f;
    material[1] = 1.0f;
    material[2] = 1.0f;
    material[3] = 1.0f;
    material[4] = 0.0f;
    material[5] = 0.0f;
    material[6] = 0.0f;
    material[7] = 1.0f;
    glMaterialfv(g_eGLFront, g_eGLAmbient, material);
    glMaterialfv(g_eGLFront, g_eGLDiffuse, material);
    glMaterialfv(g_eGLFront, g_eGLEmission, material + 4);
    DoMeshBuckets();
    g_nAnimEndTime = AurGetTime();
    g_fMeshBucketSeconds = (float)(int)(g_nAnimEndTime - g_nAnimStartTime) * 0.001f;
    g_nAnimStartTime = AurGetTime();
    DoBinBucket();
    g_nAnimEndTime = AurGetTime();
    g_fBinBucketSeconds = (float)(int)(g_nAnimEndTime - g_nAnimStartTime) * 0.001f;
    g_nAnimStartTime = AurGetTime();
}

// FUNCTION: SWKOTOR 0x00450B50
void Scene::RenderDynamicGeometry()
{
    if (g_bStencilDynamic) {
        int ref = AurGetStencilClearValue();
        glEnable(g_nGLStencilTest);
        glStencilMask(0xff);
        glStencilFunc(g_nGLAlways, ref, 0xffffffff);
        glStencilOp(g_nGLKeep, g_nGLKeep, g_nGLReplace);
    }
    AurVertexProgramMakeCurrent();
    DoGobBuckets();
}

// FUNCTION: SWKOTOR 0x00450BB0
void Scene::RenderTransparentGeometry()
{
    glEnableClientState(g_eTexCoordArray);
    glEnableClientState(g_eVertexArray);
    glEnableClientState(g_eNormalArray);
    glEnableClientState(g_eColorArray);
    glEnable(g_nGLBlend);
    glEnable(g_nGLLighting);
    glDepthMask(0);
    glBlendFunc(g_aBlendFactors[0], g_aBlendFactors[1]);
    glEnable(g_nGLDepthTest);
    DoBlurBuckets();
    glDepthMask(1);
}

// FUNCTION: SWKOTOR 0x004509B0
void Scene::DoEmitterBucket()
{
    glClearStencil(0);
    glStencilMask(0xff);
    glClear(0x400);
    if (!g_bRenderEmitters)
        return;
    glEnable(g_nGLStencilTest);
    glDisable(g_nGLStencilTest);
    PartEmitter::StartEmitterLayer();
    qsort(g_l7FBF78.element, g_l7FBF78.num, 4, EmitterOrderCmp);
    bool bAnimating = IsAnimating();
    if (g_bRenderFlat) {
        for (int i = 0; i < g_l7FBF78.num; i++) {
            Gob* gob = g_l7FBF78.element[i]->m_pGob;
            if (bAnimating || (gob && gob->IsForceAnimated())) {
                float dt = g_fFrameTime < 0.1f ? g_fFrameTime : 0.1f;
                g_l7FBF78.element[i]->AnimateParticles(dt);
            }
            g_l7FBF78.element[i]->RenderFunc();
        }
    }
    PartEmitter::EndEmitterLayer();
}

// FUNCTION: SWKOTOR 0x00450AC0
void Scene::DoTriggerBucket()
{
    if (!g_bRenderFlat)
        return;
    glDisable(g_nGLBlend);
    glDisable(g_nGLTexture2D);
    glDisable(g_nGLDepthTest);
    for (int i = 0; i < g_l80AA74.num; i++) {
        if (!g_l80AA74.element[i]->m_bA8)
            g_l80AA74.element[i]->Draw(0);
    }
    glEnable(g_nGLBlend);
    glEnable(g_nGLTexture2D);
    glEnable(g_nGLDepthTest);
}

// Stencil-masked ground fan under one shadow-casting light.
// FUNCTION: SWKOTOR 0x00450730
void Scene::RenderStencilShadowsForThisLight(Light* pLight, float fAlpha, int nStencilBit)
{
    float fRadius = pLight->m_fRadius;
    if (pLight->m_fShadowRadius > 0.0f)
        fRadius = pLight->m_fShadowRadius;
    if (fRadius == 0.0f)
        return;
    static VertexPC* s_pFan = new VertexPC[11];
    if (fRadius > 50.0f) {
        s_pFan[0].x = m_vFocus.x;
        s_pFan[0].y = m_vFocus.y;
        fRadius = 50.0f;
        s_pFan[0].z = m_vFocus.z;
    } else {
        float fHeight = (float)fabs(pLight->m_vPosition.z - m_vFocus.z);
        float fSquared = fRadius * fRadius - fHeight * fHeight;
        if (fSquared < 0.0f)
            fSquared = 0.0f;
        float fRoot = (float)sqrt(fSquared);
        s_pFan[0].x = pLight->m_vPosition.x;
        s_pFan[0].y = pLight->m_vPosition.y;
        s_pFan[0].z = m_vFocus.z;
        fRadius = fRoot;
    }
    glStencilFunc(g_eGLLequal, 1 << (nStencilBit - 1), 0xffffffff);
    glStencilOp(g_nGLKeep, g_nGLKeep, g_nGLKeep);
    glDisable(g_nGLLighting);
    glEnable(g_nGLBlend);
    if (UseSoftShadows())
        _glBlendFunc(2, 3);
    else
        glBlendFunc(g_aBlendFactors[0], g_aBlendFactors[1]);
    s_pFan[0].color = (int)(255.0f * (fAlpha * 0.5f)) << 24;
    for (int i = 0; i < 10; i++) {
        float fAngle = i * 0.69813168f;
        s_pFan[i + 1].x = (float)cos(fAngle) * fRadius + s_pFan[0].x;
        s_pFan[i + 1].y = (float)sin(fAngle) * fRadius + s_pFan[0].y;
        s_pFan[i + 1].z = m_vFocus.z;
        s_pFan[i + 1].color = 0;
    }
    DrawVerticesPC(5, s_pFan, 11);
    glEnable(g_nGLLighting);
}

// FUNCTION: SWKOTOR 0x00450550
void Scene::DoGobShadowsForThisLight(Light* pLight)
{
    float fRadius = pLight->m_fRadius;
    if (pLight->m_fShadowRadius > 0.0f)
        fRadius = pLight->m_fShadowRadius;
    int i = g_l7FBFCC.num;
    while (i) {
        i--;
        if (g_bAnimCulling && g_bAnimVisibleOnly) {
            if (i >= g_l7FBFCC.num)
                continue;
            Gob* pAnimated = g_l7FBFCC.element[i];
            if (pAnimated->m_p188)
                pAnimated->Animate(g_fFrameTime);
            if (i >= g_l7FBFCC.num)
                continue;
        }
        Gob* gob = g_l7FBFCC.element[i];
        if (!gob->m_nC2)
            continue;
        if (!gob->m_b166)
            continue;
        if (!gob->m_pModel->NoDepthTest() && !gob->m_pModel->GetB5())
            continue;
        Vector vPosition = gob->m_vPosition;
        if (gob->IsInCutscene()) {
            Part* pDummy = FindPart(gob->m_pRootPart, "cutscenedummy");
            Quaternion qOrientation;
            if (pDummy)
                pDummy->GetWorldTransform(&vPosition, &qOrientation);
        }
        if (g_bShadowDistanceCheck
            && Magnitude(g_pAnimatingScene->m_vFocus - vPosition) > g_fShadowDistance)
            continue;
        Vector vToLight;
        vToLight.x = pLight->m_vPosition.x - vPosition.x;
        vToLight.y = pLight->m_vPosition.y - vPosition.y;
        vToLight.z = pLight->m_vPosition.z - vPosition.z;
        if (Magnitude(vToLight) < fRadius)
            DoGobShadow(gob, pLight);
    }
}

// FUNCTION: SWKOTOR 0x00457B90
void Scene::DoGobBuckets()
{
    g_bGobBucketFlag = 0;
    Vector vPosition(0.0f, 0.0f, 0.0f);
    qsort(g_l7FBFCC.element, g_l7FBFCC.num, 4, GobOrderCmp);
    g_nBlurBucketCount = g_l7FBFCC.num;
    SceneLights* pLights = m_pLights;
    pLights->m_nE0 = 0;
    pLights->m_nEC = 0;
    int i = g_l7FBFCC.num;
    while (i) {
        i--;
        g_pfnBlurSetup();
        Gob* gob = g_l7FBFCC.element[i];
        if (g_bAnimCulling && g_bAnimVisibleOnly) {
            if (i >= g_l7FBFCC.num)
                continue;
            gob->Animate(g_fFrameTime);
            if (i >= g_l7FBFCC.num)
                continue;
        }
        if (!gob->VisibilityCheck())
            continue;
        if (g_bGobDepthToggle && gob->m_pModel && gob->m_pModel->NoDepthTest())
            glDisable(g_nGLLighting);
        Vector* pPosition = &gob->m_vPosition;
        vPosition = *pPosition;
        if (gob->IsInCutscene()) {
            Part* pDummy = FindPart(gob->m_pRootPart, "cutscenedummy");
            if (!pDummy) {
                m_pLights->SetupLightsForThisGob(pPosition, gob->m_fRadius);
            } else {
                static Vector s_vDummyPosition(0.0f, 0.0f, 0.0f);
                static Quaternion s_qDummyOrientation;
                pDummy->GetWorldTransform(&s_vDummyPosition, &s_qDummyOrientation);
                m_pLights->SetupLightsForThisGob(&s_vDummyPosition, gob->m_fRadius);
                vPosition = s_vDummyPosition;
            }
        } else {
            m_pLights->SetupLightsForThisGob(pPosition, gob->m_fRadius);
        }
        m_pLights->PrioritizeLights(&vPosition, 0);
        if (gob->m_nLightMask)
            m_pLights->EnableHighestPriorityLights(3, gob->m_nLightMask);
        else
            m_pLights->EnableHighestPriorityLights(3);
        if (g_bFocusLights && !_strnicmp(gob->m_szName, "c_focusgob", 10))
            m_pLights->AddFocusLight(3, *pPosition);
        glEnable(g_nGLBlend);
        glBlendFunc(g_aBlendFactors[0], g_aBlendFactors[1]);
        if (g_bRenderGobs)
            gob->Render(0);
        if (g_bGobDepthToggle && gob->m_pModel->NoDepthTest())
            glEnable(g_nGLLighting);
    }
    if (g_bAnimCulling && g_bAnimVisibleOnly) {
        int n = m_lChildren.num;
        while (n) {
            n--;
            if (n < m_lChildren.num)
                m_lChildren.element[n]->Animate(g_fFrameTime);
        }
    }
}

// FUNCTION: SWKOTOR 0x004503A0
void Scene::DoDistortionBuckets()
{
    glEnable(g_nGLStencilTest);
    glColorMask(0, 0, 0, 0);
    glStencilOp(g_nGLKeep, g_nGLKeep, g_nGLReplace);
    glStencilMask(1);
    glStencilFunc(g_nGLAlways, 1, 0xff);
    for (int i = 0; i < g_l7FBFC0.num; i++)
        g_l7FBFC0.element[i]->Render(0);
    glColorMask(1, 1, 1, 1);
    glStencilMask(0xff);
    glDisable(g_nGLStencilTest);
}

// FUNCTION: SWKOTOR 0x00450440
void Scene::DoLightSaberBucket()
{
    glEnable(g_nGLBlend);
    for (int i = 0; i < g_l7FBFF0.num; i++) {
        Gob* gob = g_l7FBFF0.element[i];
        m_pLights->SetupLightsForThisGob(&gob->m_vPosition, gob->m_fRadius);
        m_pLights->PrioritizeLights(&gob->m_vPosition, 0);
        if (gob->m_nLightMask)
            m_pLights->EnableHighestPriorityLights(3, gob->m_nLightMask);
        else
            m_pLights->EnableHighestPriorityLights(3);
        gob->Render(0);
    }
}

// FUNCTION: SWKOTOR 0x004504D0
void Scene::DoBlurBuckets()
{
    g_nBlurBucketCount = g_l7FBF90.num;
    for (int i = 0; i < g_l7FBF90.num; i++) {
        g_pfnBlurSetup();
        m_pLights->SetupLightsForThisGob(&g_l7FBF90.element[i]->m_vPosition,
                                         g_l7FBF90.element[i]->m_fRadius);
        m_pLights->PrioritizeLights(&g_l7FBF90.element[i]->m_vPosition, 0);
        m_pLights->EnableHighestPriorityLights(3);
        g_l7FBF90.element[i]->RenderBlur();
    }
}

// NONMATCH: register allocation only. The original keeps the light list
// buffer in ebp and reloads lights[j] after the radius select (ecx reused);
// cl keeps it live and folds the light position offsets.
// FUNCTION: SWKOTOR 0x00453950
void Scene::DoGobShadows()
{
    CExoArrayList<Light*> lights;
    lights = *m_pLights->GetLights(0);
    if (lights.num == 0)
        return;
    Light** ppLights = lights.element;
    int i = g_l7FBFCC.num;
    while (i) {
        i--;
        if (g_bAnimCulling && g_bAnimVisibleOnly) {
            if (i >= g_l7FBFCC.num)
                continue;
            Gob* pAnimated = g_l7FBFCC.element[i];
            if (pAnimated->m_p188)
                pAnimated->Animate(g_fFrameTime);
            if (i >= g_l7FBFCC.num)
                continue;
        }
        if (!g_l7FBFCC.element[i]->m_nC2)
            continue;
        if (!g_l7FBFCC.element[i]->m_b166)
            continue;
        if (g_bShadowDistanceCheck
            && Magnitude(g_pAnimatingScene->m_vFocus - g_l7FBFCC.element[i]->m_vPosition) > g_fShadowDistance)
            continue;
        GobModel* pModel = g_l7FBFCC.element[i]->m_pModel;
        if (pModel && !pModel->NoDepthTest()) {
            GobController* pController = g_l7FBFCC.element[i]->m_p188;
            if (!pController)
                continue;
            if (!pController->IsActive())
                continue;
        }
        for (int j = 0; j < lights.num; j++) {
            float fRadius = ppLights[j]->GetShadowRadius();
            if (Magnitude(ppLights[j]->m_vPosition - g_l7FBFCC.element[i]->m_vPosition) < fRadius)
                DoGobShadow(g_l7FBFCC.element[i], ppLights[j]);
        }
    }
}

// NONMATCH: the original keeps the light list buffer in edi (reloaded from
// the list after the part loop) and copies the floor plane in a different
// order; the radius clamp is `!(r > min)`.
// FUNCTION: SWKOTOR 0x00453BB0
void Scene::DoDynamicMeshProjections()
{
    CExoArrayList<Light*> lights;
    Plane* pFloor = &g_pRenderCamera->m_lViewVolume.element[1];
    Vector vNormal = pFloor->n;
    float fDistance = pFloor->d + 0.05f;
    lights = *m_pLights->GetLights(0);
    for (int j = 0; j < lights.num; j++) {
        Light* pLight = lights.element[j];
        float fRadius = pLight->m_fRadius;
        if (pLight->m_fShadowRadius > 0.0f)
            fRadius = pLight->m_fShadowRadius;
        if (!(fRadius > g_fMinProjectionRadius))
            fRadius = g_fMinProjectionRadius;
        for (int k = 0; k < g_lAllMeshParts.num; k++) {
            if (!g_lAllMeshParts.element[k]->InRange(pLight->m_vPosition, fRadius))
                continue;
            glPushMatrix();
            float m[16];
            MakeMatrix4x4(m, g_lAllMeshParts.element[k]->m_vPosition,
                          g_lAllMeshParts.element[k]->m_qOrientation);
            glMultMatrixf(m);
            Plane plane;
            plane.n = vNormal;
            plane.d = fDistance;
            plane.Transform(g_lAllMeshParts.element[k]->m_vPosition,
                            g_lAllMeshParts.element[k]->m_qOrientation);
            Vector vLocal = -g_lAllMeshParts.element[k]->m_qOrientation
                * (pLight->m_vPosition - g_lAllMeshParts.element[k]->m_vPosition);
            g_lAllMeshParts.element[k]->ProjectShadow(&vLocal, fRadius, &plane, 0);
            glPopMatrix();
        }
    }
}

// FUNCTION: SWKOTOR 0x00455710
void Scene::DoMeshBuckets()
{
    CExoArrayList<MeshLightmap*> lLightmaps;
    int j;
    for (int i = 0; i < g_lTransparentParts.num; i++) {
        Part* pPart = g_lTransparentParts.element[i];
        Material* pMaterial = pPart->m_pMaterial;
        pMaterial->BindTexture0();
        pMaterial->SetBlendingMode(0);
        pPart->Prepare();
        pPart->Draw(0);
    }
    CExoArrayList<int> lOrder(*AurTextureGetOrdering());
    for (int k = 0; k < lOrder.num; k++) {
        int n = lOrder.element[k];
        if (g_aMeshBuckets[n].num) {
            Material* pMaterial = g_aMeshBuckets[n].element[0]->m_pMaterial;
            pMaterial->BindTexture0();
            pMaterial->SetBlendingMode(0);
            for (j = 0; j < g_aMeshBuckets[n].num; j++) {
                Part* pPart = g_aMeshBuckets[n].element[j];
                pPart->Prepare();
                pPart->Draw(0);
                MeshLightmap* pLightmap = pPart->m_pLightmap;
                if (pLightmap && !lLightmaps.Contains(pLightmap))
                    lLightmaps.Add(pLightmap);
            }
        }
    }
    for (j = 0; j < g_aMeshBuckets[0].num; j++) {
        Part* pPart = g_aMeshBuckets[0].element[j];
        if (j == 0)
            pPart->m_pMaterial->BindTexture0();
        pPart->Prepare();
        pPart->Draw(0);
        if (!lLightmaps.Contains(pPart->m_pLightmap))
            lLightmaps.Add(pPart->m_pLightmap);
    }
    glEnableClientState(g_eTexCoordArray);
    for (int m = 0; m < lLightmaps.num; m++) {
        if (lLightmaps.element[m]) {
            lLightmaps.element[m]->m_fFade = lLightmaps.element[m]->m_fBaseFade;
            lLightmaps.element[m]->m_n8C = g_nMeshLightmapReset;
        }
    }
}

// NONMATCH: the original builds the hit-check end point in the slot of
// the camera delta (0x88-byte frame, ebp used for the argument copy) and
// duplicates the epilogue after the early `return 1`; reassigning the
// delta, block scopes and temporaries all keep separate slots.
// FUNCTION: SWKOTOR 0x004579C0
unsigned char Gob::VisibilityCheck()
{
    if (!g_bVisibilityTest)
        return 1;
    if (m_pModel && m_pModel->GetB6())
        return 1;
    if (GetFlag12())
        return 1;
    {
        Vector vDelta = g_pRenderCamera->m_vPosition - m_vPosition;
        float fDistance = Magnitude(vDelta);
        if (m_fRadius / fDistance * 2.0f > g_fVisibilityRatio) {
            m_fVisibilityTime = 0.0f;
            m_bVisible = true;
            return 1;
        }
    }
    m_fVisibilityTime = g_fFrameTime + m_fVisibilityTime;
    if (m_fVisibilityTime > g_fVisibilityInterval) {
        m_fVisibilityTime = 0.0f;
        CHitInfo hit(0, 0xBFFFFFFF, 0);
        hit.m_n00 = 1;
        Vector vHit(0.0f, 0.0f, 0.0f);
        Vector vEnd = m_vPosition + m_qOrientation * m_vCenter;
        int bHit = m_pScene->HitCheck(g_pRenderCamera->m_vPosition, vEnd, &vHit, &hit);
        m_bVisible = bHit == 0;
    }
    if (!m_bVisible)
        g_bGobBucketFlag++;
    return m_bVisible;
}

// FUNCTION: SWKOTOR 0x004511F0
void Scene::PreWarm()
{
    g_pAnimatingScene = this;
    for (int i = 0; i < m_lRooms.num; i++) {
        RoomEmitters* pRoom = m_lRooms.element[i]->m_pEmitters;
        for (int j = 0; j < pRoom->m_lEmitters.num; j++) {
            PartEmitter* pEmitter = pRoom->m_lEmitters.element[j];
            for (int n = 0; n < 100; n++)
                pEmitter->AnimateParticles(0.1f);
        }
    }
    for (int k = 0; k < m_lEmitters.num; k++) {
        PartEmitter* pEmitter = m_lEmitters.element[k];
        for (int n = 0; n < 100; n++)
            pEmitter->AnimateParticles(0.1f);
    }
    g_pAnimatingScene = 0;
}

static inline Vector operator*(const Vector& v, float f)
{
    return Vector(v.x * f, v.y * f, v.z * f);
}

static __forceinline void SetFog(unsigned int ePName, float fParam)
{
    glFogf(ePName, fParam);
}

__forceinline void Scene::DoFadePart(Part* pPart, CExoArrayList<MeshLightmap*>& lLightmaps, float& fAlpha)
{
    fAlpha = pPart->m_fAlpha;
    pPart->m_fAlpha = pPart->m_pLightmap->m_fFade * pPart->m_fAlpha;
    if (pPart->m_fAlpha > 0.0f) {
        pPart->Prepare();
        if (pPart->m_pLightmap && pPart->m_pLightmap->m_fFogFade < 1.0f) {
            glEnable(g_nGLFog);
            float fFade = pPart->m_pLightmap->m_fFogFade;
            float fFade2 = fFade * fFade;
            Vector vColor = m_vFogColor * fFade2;
            glFogfv(g_eGLFogColor, &vColor.x);
            SetFog(g_eGLFogDensity, m_fFogDensity * 2.0f);
            SetFog(g_eGLFogStart, pPart->m_pLightmap->m_fFogFade * m_fFogStart);
            SetFog(g_eGLFogEnd, pPart->m_pLightmap->m_fFogFade * m_fFogEnd + 0.1f);
            pPart->Draw(0);
            if (!m_bFogEnabled)
                glDisable(g_nGLFog);
            glFogf(g_eGLFogDensity, GetFogDensity());
            glFogf(g_eGLFogStart, GetFogStart());
            glFogf(g_eGLFogEnd, GetFogEnd());
            glFogfv(g_eGLFogColor, &m_vFogColor.x);
        } else {
            pPart->Draw(0);
        }
    }
    pPart->m_fAlpha = fAlpha;
    if (!lLightmaps.Contains(pPart->m_pLightmap))
        lLightmaps.Add(pPart->m_pLightmap);
}

// NONMATCH (1 insn): in the first inlined part copy the original addresses
// the lightmap list's grow copy as [dst + delta]; the second copy and the
// rest match.
// FUNCTION: SWKOTOR 0x00455B60
void Scene::DoFadeBuckets()
{
    CExoArrayList<MeshLightmap*> lLightmaps;
    CExoArrayList<int> lOrder(*AurTextureGetOrdering());
    float fAlpha;
    m_pLights->ResetLights();
    glEnable(g_nGLBlend);
    glColorMaterial(g_eGLFront, g_eGLEmissionMode);
    glEnable(0xb57);
    if (g_bBlackFog)
        glEnableClientState(g_eColorArray);
    float material[8];
    material[0] = 1.0f;
    material[1] = 1.0f;
    material[2] = 1.0f;
    material[3] = 1.0f;
    material[4] = 0.0f;
    material[5] = 0.0f;
    material[6] = 0.0f;
    material[7] = 1.0f;
    glMaterialfv(g_eGLFront, g_eGLAmbient, material);
    glMaterialfv(g_eGLFront, g_eGLDiffuse, material);
    glMaterialfv(g_eGLFront, g_eGLEmission, material + 4);
    int j;
    for (int k = 0; k < lOrder.num; k++) {
        int n = lOrder.element[k];
        if (g_aFadeBuckets[n].num) {
            Material* pMaterial = g_aFadeBuckets[n].element[0]->m_pMaterial;
            pMaterial->BindTexture0();
            pMaterial->SetBlendingMode(0);
            for (j = 0; j < g_aFadeBuckets[n].num; j++)
                DoFadePart(g_aFadeBuckets[n].element[j], lLightmaps, fAlpha);
        }
    }
    for (j = 0; j < g_aFadeBuckets[0].num; j++) {
        Part* pPart = g_aFadeBuckets[0].element[j];
        if (j == 0)
            pPart->m_pMaterial->BindTexture0();
        DoFadePart(pPart, lLightmaps, fAlpha);
    }
    for (int m = 0; m < lLightmaps.num; m++) {
        lLightmaps.element[m]->m_fFade = lLightmaps.element[m]->m_fBaseFade;
        lLightmaps.element[m]->m_n8C = g_nMeshLightmapReset;
    }
    glDisableClientState(g_eColorArray);
    glDisable(0xb57);
    glDisable(g_nGLBlend);
}
