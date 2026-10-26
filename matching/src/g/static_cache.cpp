// Functions built on function-scope statics: the shadow scratch block
// (0x00426940) and the microsecond timer (0x0044f320); plus the small
// Scene virtuals next to the timer.
typedef unsigned int size_t;

void* __cdecl operator new[](size_t n);
void __cdecl operator delete[](void* p);

extern int g_nShadowMemoryPeak;                 // 0x00828050

// FUNCTION: SWKOTOR 0x00426940
// Name not recorded; one scratch block that only ever grows.
void* AllocateShadowMemory(int nSize)
{
    static char* s_pBlock = new char[nSize];
    static int s_nCapacity = nSize;
    if (nSize > g_nShadowMemoryPeak)
        g_nShadowMemoryPeak = nSize;
    if (nSize > s_nCapacity) {
        delete[] s_pBlock;
        s_pBlock = new char[nSize];
        s_nCapacity = nSize;
    }
    return s_pBlock;
}

union _LARGE_INTEGER {
    struct {
        unsigned long LowPart;
        long HighPart;
    };
    __int64 QuadPart;
};

extern "C" __declspec(dllimport) int __stdcall QueryPerformanceFrequency(_LARGE_INTEGER* p);
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceCounter(_LARGE_INTEGER* p);

inline __int64 GetTimerFrequency()
{
    _LARGE_INTEGER nFrequency;
    QueryPerformanceFrequency(&nFrequency);
    return nFrequency.QuadPart;
}

// FUNCTION: SWKOTOR 0x0044f320
__int64 AurGetTime()
{
    static __int64 s_nFrequency = GetTimerFrequency();
    _LARGE_INTEGER nCounter;
    QueryPerformanceCounter(&nCounter);
    return nCounter.QuadPart * 1000000 / s_nFrequency;
}

class Vector {
public:
    float x, y, z;
};

class WindManager {
public:
    Vector GetWind(Vector vPosition, float fScale);    // 0x004a6050
};

class Gob {
public:
    char pad00[0x1b4];
    float m_fScale;                             // +0x1b4
};

extern int g_bBlackFog;                         // 0x008305e0

// Scene virtuals; 0x0044f3a0-0x0044f3c0, 0x0044f780, 0x0044f800,
// 0x0044f810, 0x0044f8c0, 0x0044fc80, 0x0044fca0, 0x0044fd40 and
// 0x0044fd50 are missing from functions.csv.
class Scene {
public:
    char pad00[0x84];
    WindManager* m_pWind;                       // +0x84
    char pad88[0xd4 - 0x88];
    int m_nCurrentRoom;                         // +0xd4
    short m_bIgnoreVisibilityGraph;             // +0xd8
    unsigned char m_bAnimating;                 // +0xda

    float GetObjectScale(Gob* pGob);            // +0x14
    void EnableAnimations();                    // +0x60
    void DisableAnimations();                   // +0x64
    void SetBlindLights(int b);                 // +0x68
    unsigned char IsAnimating();                // +0xa8
    Vector GetWindVector(Vector vPosition, float fScale);  // +0xd0
    int ReplaceTileTexture(char* szOld, char* szNew, int n);   // +0x30
    void DeleteTile(Vector* pPosition);         // +0x34
    void SetCurrentRoom(int nRoom);             // +0x80
    int GetIgnoreVisibilityGraph();             // +0x84
    void DisableVisibilityGraph();              // +0x8c
};

int AurTextureReplaceTileReferences(Scene* pScene, char* szOld, char* szNew, int n);  // 0x004237e0
void SceneBSPRemoveTileData(Scene* pScene, Vector* pPosition);    // 0x0046d2f0

// FUNCTION: SWKOTOR 0x0044f3a0
void Scene::EnableAnimations()
{
    m_bAnimating = 1;
}

// FUNCTION: SWKOTOR 0x0044f3b0
void Scene::DisableAnimations()
{
    m_bAnimating = 0;
}

// FUNCTION: SWKOTOR 0x0044f3c0
unsigned char Scene::IsAnimating()
{
    return m_bAnimating;
}

// FUNCTION: SWKOTOR 0x0044f780
Vector Scene::GetWindVector(Vector vPosition, float fScale)
{
    return m_pWind->GetWind(vPosition, fScale);
}

// FUNCTION: SWKOTOR 0x0044f800
void Scene::DisableVisibilityGraph()
{
    m_bIgnoreVisibilityGraph = 1;
}

// FUNCTION: SWKOTOR 0x0044f810
int Scene::GetIgnoreVisibilityGraph()
{
    return m_bIgnoreVisibilityGraph;
}

// FUNCTION: SWKOTOR 0x0044f8c0
void Scene::SetCurrentRoom(int nRoom)
{
    m_nCurrentRoom = nRoom;
}

// FUNCTION: SWKOTOR 0x0044fc80
int Scene::ReplaceTileTexture(char* szOld, char* szNew, int n)
{
    return AurTextureReplaceTileReferences(this, szOld, szNew, n);
}

// FUNCTION: SWKOTOR 0x0044fca0
void Scene::DeleteTile(Vector* pPosition)
{
    SceneBSPRemoveTileData(this, pPosition);
}

// FUNCTION: SWKOTOR 0x0044fd40
float Scene::GetObjectScale(Gob* pGob)
{
    return pGob->m_fScale;
}

// FUNCTION: SWKOTOR 0x0044fd50
void Scene::SetBlindLights(int b)
{
    g_bBlackFog = b;
}
