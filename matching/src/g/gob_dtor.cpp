// Gob destructor and scalar deleting destructor (vtable 0x741268).
// Gob derives from CAurObject (vtable 0x741108, SafePointer target at +0x04).
#define G_EXO_ARRAY_LIST_REMOVE
#define G_SAFE_POINTER_LIST_DECL_ONLY
#include "g/aur_texture.h"

class CAurObject : public SafePointerTarget<CAurObject> {
public:
    virtual ~CAurObject() {}
};

// vtable 0x00740a94
class CAurMeshBuffer {
public:
    virtual ~CAurMeshBuffer() {}
    int m_n04;
    int m_n08;
};

class Gob;
class Part;
class Scene;
class AnimRun;
class GobEventHandler {
public:
    char szEvent[0x20];
};

class Scene {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9c();
    virtual void va0(); virtual void va4(); virtual void va8(); virtual void vac();
    virtual void AddGob(Gob* pGob);                     // +0xb0
    virtual void RemoveGob(Gob* pGob);                  // +0xb4
};

class Deletable {
public:
    virtual ~Deletable();
};

extern int g_bKeepModels;                   // 0x007a8a04
extern int g_nGobCount;                     // 0x007a8a18

class MaxTree {
public:
    char pad00[0x54];
    int m_nRefCount;                        // +0x54

    static void operator delete(void* p);   // 0x0044aec0
};

class Model : public MaxTree {
};

void __cdecl AurTextureRelease(CAurTexture* p);                     // 0x00420d70
void __cdecl SceneRemoveGobLights(Gob* pGob);                       // 0x00455540

class Gob : public CAurObject {
public:
    virtual ~Gob();                                     // +0x000
    virtual void v004(); virtual void v008(); virtual void v00c();
    virtual void v010(); virtual void v014();
    virtual int PlayAnimation(char* szName, float fSpeed, int nFlags, float fStart);  // +0x018
    virtual void v01c();
    virtual void v020(); virtual void v024(); virtual void v028(); virtual void v02c();
    virtual void v030(); virtual void v034(); virtual void v038(); virtual void v03c();
    virtual void v040(); virtual void v044(); virtual void v048(); virtual void v04c();
    virtual void v050(); virtual void v054(); virtual void v058(); virtual void v05c();
    virtual void v060(); virtual void v064(); virtual void v068(); virtual void v06c();
    virtual void v070(); virtual void v074(); virtual void v078(); virtual void v07c();
    virtual void v080(); virtual void v084(); virtual void v088(); virtual void v08c();
    virtual void v090(); virtual void v094(); virtual void v098(); virtual void v09c();
    virtual void v0a0(); virtual void v0a4(); virtual void v0a8(); virtual void v0ac();
    virtual void v0b0(); virtual void v0b4(); virtual void v0b8(); virtual void v0bc();
    virtual void v0c0(); virtual void v0c4(); virtual void v0c8(); virtual void v0cc();
    virtual void v0d0(); virtual void v0d4(); virtual void v0d8(); virtual void v0dc();
    virtual void v0e0(); virtual void v0e4(); virtual void v0e8(); virtual void v0ec();
    virtual void v0f0(); virtual void v0f4(); virtual void v0f8(); virtual void v0fc();
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10c();
    virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11c();
    virtual void v120(); virtual void v124(); virtual void v128(); virtual void v12c();
    virtual void v130(); virtual void v134(); virtual void v138(); virtual void v13c();
    virtual void v140(); virtual void v144(); virtual void v148(); virtual void v14c();
    virtual void v150(); virtual void v154(); virtual void v158(); virtual void v15c();
    virtual void v160(); virtual void v164(); virtual void v168();
    virtual void DoEvent(char* szEvent, void* pData);  // +0x16c

    char pad10[0x20 - 0x10];
    CExoArrayList<int> m_l20;                           // +0x20
    char pad2c[0x58 - 0x2c];
    Model* m_pModel;                                    // +0x58
    Model* m_pModel5c;                                  // +0x5c
    Model* m_pModel60;                                  // +0x60
    Model* m_pAnimModel;                                // +0x64
    Scene* m_pScene;                                    // +0x68
    Deletable* m_pRoot;                                 // +0x6c
    char pad70[0xb0 - 0x70];
    CExoArrayList<AnimRun*> m_lRuns;                    // +0xb0
    char padbc[0xec - 0xbc];
    CExoArrayList<Model*> m_lAttachedModels;            // +0xec
    CExoArrayList<Part*> m_lAttachedParts;              // +0xf8
    char pad104[0x110 - 0x104];
    CAurMeshBuffer m_MeshBuffer;                        // +0x110
    char pad11c[0x174 - 0x11c];
    Deletable* m_p174;                                  // +0x174
    Deletable* m_p178;                                  // +0x178
    char pad17c[0x188 - 0x17c];
    Deletable* m_pAttachment;                           // +0x188
    char pad18c[0x190 - 0x18c];
    CExoArrayList<GobEventHandler*> m_lHandlers;        // +0x190
    CExoArrayList<int> m_l19c;                          // +0x19c
    SafePointer<CAurTexture> m_pEnvironment;            // +0x1a8
    char pad1ac[0x1bc - 0x1ac];
    CExoArrayList<Gob*> m_lAttachments;                 // +0x1bc

    void SetScene(Scene* pScene)
    {
        if (m_pScene == pScene)
            return;
        if (m_pScene)
            m_pScene->RemoveGob(this);
        m_pScene = pScene;
        if (pScene)
            pScene->AddGob(this);
        DoEvent("SetScene", 0);
    }
    void SetAttachment(Deletable* p)
    {
        Deletable* pOld = m_pAttachment;
        m_pAttachment = p;
        if (pOld)
            delete pOld;
    }
};

extern CExoArrayList<Gob*> g_lGobs;         // 0x007a8a58

static inline void ReleaseModel(Model* pModel)
{
    if (!g_bKeepModels && --pModel->m_nRefCount == 0)
        MaxTree::operator delete(pModel);
}

// FUNCTION: SWKOTOR 0x0044b2f0
Gob::~Gob()
{
    g_nGobCount--;
    DoEvent("Destroy", 0);
    PlayAnimation(0, 1.0f, 0, 0);
    SetScene(0);
    SetAttachment(0);
    if (m_pRoot) {
        delete m_pRoot;
        m_pRoot = 0;
    }
    int i;
    for (i = 0; i < m_lAttachedModels.num; i++)
        ReleaseModel(m_lAttachedModels.element[i]);
    m_lAttachedModels.num = 0;
    m_lAttachedParts.num = 0;
    if (m_pModel)
        ReleaseModel(m_pModel);
    if (m_pModel5c)
        ReleaseModel(m_pModel5c);
    if (m_pModel60)
        ReleaseModel(m_pModel60);
    for (i = 0; i < m_lHandlers.num; i++) {
        delete m_lHandlers.element[i];
        m_lHandlers.element[i] = 0;
    }
    m_lHandlers.num = 0;
    CAurTexture* pEnvironment = m_pEnvironment.m_p;
    m_pEnvironment = 0;
    AurTextureRelease(pEnvironment);
    g_lGobs.Remove(this);
    SceneRemoveGobLights(this);
    if (m_p174) {
        delete m_p174;
        m_p174 = 0;
    }
    if (m_p178) {
        delete m_p178;
        m_p178 = 0;
    }
}

// FUNCTION: SWKOTOR 0x0044b9e0 SYMBOL: ??_GGob@@UAEPAXI@Z
