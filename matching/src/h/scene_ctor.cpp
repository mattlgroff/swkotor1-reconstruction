// Scene constructor. Uses a TU-local Scene layout whose member types
// zero-construct in the original order; h/scene.h keeps the padded layout
// used by the other Scene files.
#include "h/scenebsp.h"

extern "C" int __cdecl sprintf(char*, const char*, ...);
extern "C" char* __cdecl strcpy(char*, const char*);
#pragma intrinsic(strcpy)

class Scene;
class Gob;
class CAurTexture;

class Model {
public:
    Model(char* szName);                    // 0x0044AA70
    ~Model();
    void* operator new(unsigned int n);     // 0x0043E1B0
    void operator delete(void* p);          // 0x0044AEC0
    int AddRef(Gob* pGob);                  // 0x0043E240
    char pad[0xc4];
};

class Gob {
public:
    Gob(char* szName);                      // 0x00449440
    ~Gob();                                 // 0x0044B2F0
    void AttachToScene(Scene* pScene);      // 0x0043F200
    char pad_0x0[0x58];
    Model* m_pModel;                        // +0x58
    char pad_0x5c[0x1cc - 0x5c];
};

class LightManager {
public:
    LightManager();                         // 0x004A38B0
    ~LightManager();
    char pad[0xf8];
};

class WindManager {
public:
    WindManager();                          // 0x004A6640
    ~WindManager();
    char pad[0x858];
};

class SafePointerHost : public CExoArrayList<void*> {
public:
    ~SafePointerHost();                     // 0x00457820
};

class CAurScene : public SafePointerHost {
public:
    virtual ~CAurScene();                   // 0x00457990
};

class TextureRef {
public:
    CAurTexture* m_p;
    TextureRef() { m_p = 0; }
    ~TextureRef();
    TextureRef& operator=(CAurTexture* p);  // 0x00447FA0
};

class Vector0 : public Vector {
public:
    Vector0() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector0(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

class Scene : public CAurScene {
public:
    char m_szName[0x40];                    // +0x10
    int m_n50;                              // +0x50
    CExoArrayList<void*> m_lChildren;       // +0x54
    int m_bFogEnabled;                      // +0x60
    float m_fFogStart;                      // +0x64
    float m_fFogEnd;                        // +0x68
    int m_nFogMode;                         // +0x6c
    float m_fFogDensity;                    // +0x70
    Vector0 m_vFogColor;                    // +0x74
    LightManager* m_pLights;                // +0x80
    WindManager* m_pWind;                   // +0x84
    Vector0 m_v88;                          // +0x88
    CExoArrayList<void*> m_l94;             // +0x94
    void* m_pBSP;                           // +0xa0
    TextureRef m_pTexture;                  // +0xa4
    Vector0 m_vA8;                          // +0xa8
    int m_nB4;                              // +0xb4
    int m_nB8;                              // +0xb8
    CExoArrayList<void*> m_lBC;             // +0xbc
    CExoArrayList<void*> m_lRooms;          // +0xc8
    void* m_pCurrentRoom;                   // +0xd4
    unsigned short m_wD8;                   // +0xd8
    unsigned char m_bDA;                    // +0xda
    unsigned char m_bDB;                    // +0xdb
    int m_nDC;                              // +0xdc
    float m_fE0;                            // +0xe0
    CAurTexture* m_pFilmNoise;              // +0xe4
    CAurTexture* m_pDistortion;             // +0xe8
    CExoArrayList<void*> m_lAnimated;       // +0xec
    Gob m_gob;                              // +0xf8

    Scene(char* szName);                    // 0x00458790
    virtual ~Scene();
};

Scene* FindScene(char* szName);             // 0x0044FD60
Gob* GobListFind(char* szName);             // 0x00440460
CAurTexture* AurTextureGetNormalizationCubeMap();       // 0x004233E0
CAurTexture* AurTextureGetReference(char* szName, int n);   // 0x00423A60
int AurGLCaps560();                         // 0x0045F560
void ResetVertexPrograms_004a2470();        // 0x004A2470

extern int g_nFocusGobCounter;              // 0x007B92D8, descriptive
extern CExoArrayList<Scene*> g_lScenes;     // 0x007B9398

// FUNCTION: SWKOTOR 0x00458790
Scene::Scene(char* szName) : m_gob("c_FocusGob")
{
    m_szName[0] = 0;
    if (szName) {
        char szUnique[0x40];
        sprintf(szUnique, "%s", szName);
        int i = 0;
        while (FindScene(szUnique) || GobListFind(szUnique)) {
            i++;
            sprintf(szUnique, "%s%d", szName, i);
        }
        strcpy(m_szName, szUnique);
    }
    char szModel[0x20];
    sprintf(szModel, "c_FocusGobDummyModel%d", g_nFocusGobCounter);
    g_nFocusGobCounter++;
    m_gob.m_pModel = new Model(szModel);
    m_gob.m_pModel->AddRef(&m_gob);
    m_v88 = Vector0(0.0f, 0.0f, 0.0f);
    m_bFogEnabled = 0;
    m_nFogMode = 0;
    m_vFogColor = Vector0(1.0f, 1.0f, 1.0f);
    m_fFogStart = 5.0f;
    m_fFogEnd = 40.0f;
    m_fFogDensity = 0.1f;
    m_n50 = 0;
    m_pLights = new LightManager;
    m_pWind = new WindManager;
    m_pTexture = AurTextureGetNormalizationCubeMap();
    m_wD8 = 1;
    m_nB4 = 0;
    g_lScenes.Add(this);
    m_pBSP = 0;
    m_bDA = 1;
    m_bDB = 0;
    m_nDC = 0;
    m_fE0 = 1.0f;
    m_lBC.num = 0;
    m_gob.AttachToScene(this);
    m_pCurrentRoom = 0;
    m_pFilmNoise = AurTextureGetReference("filmnoisetex", 0);
    m_pDistortion = AurTextureGetReference("distortiontex", 0);
    if (AurGLCaps560())
        ResetVertexPrograms_004a2470();
}
