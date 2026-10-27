// LightManager (aurlightmanager.o, 0x004A2A00-0x004A5710). Minimal
// layout for this bucket's members; vtable has 25 slots (0x00743958).
#ifndef I_LIGHT_MANAGER_H
#define I_LIGHT_MANAGER_H

__declspec(dllimport) void __stdcall glEnable(unsigned int cap);
__declspec(dllimport) void __stdcall glDisable(unsigned int cap);
__declspec(dllimport) void __stdcall glLightfv(unsigned int light, unsigned int pname,
                                               const float* params);

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
    CExoArrayList(const CExoArrayList<T>& o)                    // 0x0046D250
    {
        num = 0;
        array_size = 0;
        element = 0;
        for (int i = 0; i < o.num; i++)
            AddCopy(o.element[i]);
    }
    __forceinline void AddCopy(T value)
    {
        if (num == array_size) {
            int n = array_size ? array_size * 2 : 8;
            T* old = element;
            array_size = n;
            element = new T[n];
            for (int i = 0; i < num; i++)
                element[i] = old[i];
            if (old)
                delete[] old;
        }
        element[num++] = value;
    }
    ~CExoArrayList() { if (element) delete[] element; }
    CExoArrayList<T>& operator=(const CExoArrayList<T>& o)      // 0x0046BC80
    {
        num = 0;
        for (int i = 0; i < o.num; i++)
            AddCopy(o.element[i]);
        return *this;
    }
    void Allocate(int nSize);                                   // <Light*>: 0x005E03C0
    void Remove(T t);                                           // <Light*>: 0x00445080
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 8);
        }
        element[num] = t;
        num++;
    }
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
};

template <class T>
void CExoArrayList<T>::Allocate(int nSize)
{
    T* pOld = element;
    array_size = nSize;
    element = new T[nSize];
    for (int i = 0; i < num; i++) {
        element[i] = pOld[i];
    }
    if (pOld) {
        delete[] pOld;
    }
}

class Vector;
int VectorEqual(const Vector& a, const Vector& b);      // 0x004AA980 (placeholder name)

class Vector {
public:
    int IsEqual(const Vector& v) const { return VectorEqual(*this, v); }
    Vector() {}
    Vector(float fX, float fY, float fZ)
    {
        x = fX;
        y = fY;
        z = fZ;
    }
    Vector& operator-=(const Vector& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    float x;
    float y;
    float z;
};

inline Vector operator-(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vector operator*(const Vector& a, float f)
{
    return Vector(f * a.x, f * a.y, f * a.z);
}

class Scene;

class Quaternion {
public:
    Quaternion()
    {
        z = 0.0f;
        y = 0.0f;
        x = 0.0f;
        w = 1.0f;
    }

    float w, x, y, z;
};

// The light's model node; only the shadow priority is read here.
class LightNode {
public:
    char pad_0x0[0x90];
    int m_nShadowPriority;                              // +0x90 (placeholder name)
};

// PartLight (a model Part).
class Light {
public:
    virtual void v00();
    virtual void v04();
    virtual void GetLocalTransform(Vector* pPos, Quaternion* pOrient);  // +0x08
    virtual void GetWorldTransform(Vector* pPos, Quaternion* pOrient);  // +0x0c

    LightNode* m_pNode;                                 // +0x04
    char pad_0x08[0x40 - 0x8];
    int m_bDynamicType;                                 // +0x40 (placeholder name)
    char pad_0x44[0x4c - 0x44];
    Vector m_vColor;                                    // +0x4c
    float m_fRadius;                                    // +0x58
    int m_bCastShadow;                                  // +0x5c (placeholder name)
    char pad_0x60[0x68 - 0x60];
    int m_bAmbientOnly;                                 // +0x68
    int m_bAffectDynamic;                               // +0x6c
    int m_bAffectStatic;                                // +0x70
    char pad_0x74[0x80 - 0x74];
    float m_fPriority;                                  // +0x80 (placeholder name)
    char pad_0x84[0x8c - 0x84];
    float m_fMultiplier;                                // +0x8c
    float m_fShadowFade;                                // +0x90 (placeholder name)
    Vector m_vPosition;                                 // +0x94
};

class LightManager {
public:
    virtual void PrioritizeLights(Vector* pPos, int bFlag);             // +0x00
    virtual void EnableLights();                                        // +0x04
    virtual void EnableLightsDynamicOnly();                             // +0x08
    virtual void AddLight(Light* pLight);                               // +0x0c
    virtual void RemoveLight(Light* pLight, unsigned char bAll);        // +0x10
    virtual void AddRenderLight(Light* pLight);                         // +0x14
    virtual void Reset();                                               // +0x18
    virtual void UpdateLights();                                        // +0x1c
    virtual void EnableHighestPriorityLights(int n);                    // +0x20
    virtual void EnableHighestPriorityLightsFade(int n, void* pFade);   // +0x24
    virtual void V28(int a, int b, int c, int d);                       // +0x28
    virtual void DisableBumpMapLights();                                // +0x2c 0x004A5680
    virtual void EnableBumpMapLights();                                 // +0x30 0x004A5710
    virtual void CollectLightsForGivenRoom(void* pRoom);                // +0x34
    virtual void SetupLightsForThisGob(Vector* pPos, float fRadius);    // +0x38
    virtual void SetupLightsForThisRoom(Vector* pPos, float fRadius);   // +0x3c
    virtual void SetupLightsForThisPositionAndRadius(Vector* pPos, float fRadius);  // +0x40 0x004A36B0
    virtual void SetupLightsForThisPositionAndRadiusIn(Vector* pPos, float fRadius,
                                                       CExoArrayList<Light*>* pLights);  // +0x44 0x004A35B0
    virtual void V48(int a, int b);                                     // +0x48
    virtual void PrioritizeIllumination(unsigned char bForce);          // +0x4c 0x004A3CF0
    virtual void PrioritizeShadow();                                    // +0x50 0x004A3E30
    virtual void PrioritizeBump();                                      // +0x54 0x004A3EE0
    virtual void V58();                                                 // +0x58
    virtual void ControlLightsFade(CExoArrayList<Light*>* pActive, CExoArrayList<Light*>* pPrevious,
                                   CExoArrayList<Light*>* pDesired, int bDynamicOnly);  // +0x5c
    virtual void EnableHeadLight();                                     // +0x60 0x004A2FF0

    LightManager();                                                     // 0x004A38B0
    ~LightManager();                                                    // 0x004A2BC0

    int DisableThisLight(Light* pLight);                                // 0x004A4F10
    int EnableThisLight(Light* pLight);                                 // 0x004A4FF0
    CExoArrayList<Light*>* GetShadowLights(int nMode);                  // 0x004A4B10
    int GetNearestLights(Scene* pScene, Vector vPos, float fRadius, int bDynamicOnly);  // 0x004A3770

    int m_bUseList74;                                   // +0x04
    CExoArrayList<Light*> m_lLights;                    // +0x08
    CExoArrayList<Light*> m_lDynamicLights;             // +0x14 (placeholder name)
    CExoArrayList<Light*> m_lStaticLights;              // +0x20 (placeholder name)
    CExoArrayList<Light*> m_lRenderLights;              // +0x2c
    CExoArrayList<Light*> m_l38;                        // +0x38
    CExoArrayList<Light*> m_lEnabled44;                 // +0x44
    CExoArrayList<Light*> m_lShadowLights;              // +0x50 (placeholder name)
    CExoArrayList<Light*> m_lBumpMapLights;             // +0x5c
    CExoArrayList<Light*> m_l68;                        // +0x68
    CExoArrayList<Light*> m_lEnabled74;                 // +0x74
    CExoArrayList<Light*> m_l80;                        // +0x80
    CExoArrayList<Light*> m_l8c;                        // +0x8c
    CExoArrayList<Light*> m_l98;                        // +0x98
    Light* m_pShadowLight;                              // +0xa4 (placeholder name)
    int m_nA8;                                          // +0xa8
    CExoArrayList<Light*> m_lAC;                        // +0xac
    CExoArrayList<Light*> m_lB8;                        // +0xb8
    CExoArrayList<Light*> m_lC4;                        // +0xc4
    CExoArrayList<Light*> m_lD0;                        // +0xd0
    CExoArrayList<Light*> m_lDC;                        // +0xdc
    CExoArrayList<Light*> m_lE8;                        // +0xe8
    unsigned char m_bBumpMapLightsDisabled;             // +0xf4
};

extern int g_bVertexProgramRefresh;                     // 0x007A193C
extern int g_bDynamicLights;                            // 0x007B92DC
extern int g_nActiveLights;                             // 0x008305E8 (placeholder name)
extern int g_nMaxShadowLights;                          // 0x007A1938 (placeholder name)
extern float g_fFrameTime;                              // 0x0078E574 (placeholder name)
extern float g_fShadowLightFadeTime;                    // 0x007A1948 (placeholder name)
extern int g_nDynamicActiveLights;                      // 0x008305EC (placeholder name)
extern short g_nHeadLight;                              // 0x0078E388 (placeholder name)
void AurEnableHeadLight();                              // 0x0044DA60
float* AurMakeColor(const Vector& vColor, const float& fAlpha);     // 0x004AAAB0
extern const unsigned int g_aGLLightParams[];           // 0x0073F320 (placeholder name)
extern Vector g_vHeadLightAmbient;                      // 0x007A1958 (placeholder name)
extern Vector g_vHeadLightDiffuse;                      // 0x007A194C (placeholder name)
class RenderCamera {
public:
    char pad_0x0[0x7c];
    Vector m_vPosition;                                 // +0x7c
};
extern RenderCamera* g_pRenderCamera;                   // 0x007BB4E8
void AurDisableHeadLight();                             // 0x0044DA80

int __cdecl LightCompare(const void* a, const void* b); // 0x004A28C0 (placeholder name)
extern "C" void __cdecl qsort(void* base, unsigned int num, unsigned int width,
                              int(__cdecl* compare)(const void*, const void*));
float FUN_004ab100(const Vector* v);                    // 0x004AB100 magnitude
void SetLightGL(int nIndex, const Vector& vPosition, const Vector& vColor, const float& fRadius,
                const int& bAmbient);                   // 0x004A2A00

#endif
