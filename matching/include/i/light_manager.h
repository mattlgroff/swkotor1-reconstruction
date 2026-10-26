// LightManager (aurlightmanager.o, 0x004A2A00-0x004A5710). Minimal
// layout for this bucket's members; vtable has 25 slots (0x00743958).
#ifndef I_LIGHT_MANAGER_H
#define I_LIGHT_MANAGER_H

__declspec(dllimport) void __stdcall glEnable(unsigned int cap);
__declspec(dllimport) void __stdcall glDisable(unsigned int cap);

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    CExoArrayList(const CExoArrayList<T>& other);               // 0x0046D250
    ~CExoArrayList() { if (element) delete[] element; }
    CExoArrayList<T>& operator=(const CExoArrayList<T>& other); // 0x0046BC80
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

class Vector {
public:
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

class Light {
public:
    char pad_0x00[0x4c];
    Vector m_vColor;                                    // +0x4c
    float m_fRadius;                                    // +0x58
    char pad_0x5c[0x68 - 0x5c];
    int m_bAmbientOnly;                                 // +0x68
    int m_bAffectDynamic;                               // +0x6c
    int m_bAffectStatic;                                // +0x70
    char pad_0x74[0x8c - 0x74];
    float m_fMultiplier;                                // +0x8c
    char pad_0x90[0x94 - 0x90];
    Vector m_vPosition;                                 // +0x94
};

class LightManager {
public:
    virtual void PrioritizeLights(Vector* pPos, int bFlag);             // +0x00
    virtual void EnableLights();                                        // +0x04
    virtual void EnableLightsDynamicOnly();                             // +0x08
    virtual void AddLight(Light* pLight);                               // +0x0c
    virtual void RemoveLight(Light* pLight, int bFlag);                 // +0x10
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

    int DisableThisLight(Light* pLight);                                // 0x004A4F10
    int EnableThisLight(Light* pLight);                                 // 0x004A4FF0
    int GetNearestLights(Scene* pScene, Vector vPos, float fRadius, int bDynamicOnly);  // 0x004A3770

    int m_bUseList74;                                   // +0x04
    CExoArrayList<Light*> m_lLights;                    // +0x08
    char pad_0x14[0x30 - 0x14];
    int m_nRenderLights;                                // +0x30
    char pad_0x34[0x44 - 0x34];
    CExoArrayList<Light*> m_lEnabled44;                 // +0x44
    char pad_0x50[0x5c - 0x50];
    CExoArrayList<Light*> m_lBumpMapLights;             // +0x5c
    char pad_0x68[0x74 - 0x68];
    CExoArrayList<Light*> m_lEnabled74;                 // +0x74
    char pad_0x80[0xf4 - 0x80];
    unsigned char m_bBumpMapLightsDisabled;             // +0xf4
};

extern int g_bVertexProgramRefresh;                     // 0x007A193C
extern int g_bDynamicLights;                            // 0x007B92DC

float FUN_004ab100(const Vector* v);                    // 0x004AB100 magnitude
void SetLightGL(int nIndex, const Vector& vPosition, const Vector& vColor, const float& fRadius,
                const int& bAmbient);                   // 0x004A2A00

#endif
