// Scene core helpers, CHitInfo and the SafePointer host
// (0x00451C80 .. 0x00458ED0).

#include "h/scene.h"

__int64 AurGetTime();                                   // 0x0044F320
Scene* FindScene(char* szName);                         // 0x0044FD60
void SceneAddSpecialPart(Scene* pScene, ScenePart* pPart);      // 0x0046C420
void SceneRemoveSpecialPart(Scene* pScene, ScenePart* pPart);   // 0x0046C4A0

extern __int64 g_nAnimStartTime;                        // 0x007B9298
extern Scene* g_pAnimatingScene;                        // 0x007B92A0
extern __int64 g_nAnimEndTime;                          // 0x007B9288
extern float g_fAnimSeconds;                            // 0x007B92D0
extern int g_bAnimCulling;                              // 0x0078E3B8
extern unsigned char g_bAnimVisibleOnly;                // 0x007B92F4
extern CExoArrayList<void*> g_lSceneLights;             // 0x007FBF78
extern CExoArrayList<void*> g_lSceneEmitters;           // 0x0080AA74
extern CExoArrayList<void*> g_lSceneObjects;            // 0x007FBFCC

// FUNCTION: SWKOTOR 0x00451C80
void Scene::Animate(int n)
{
    g_nAnimStartTime = AurGetTime();
    g_pAnimatingScene = this;
    if (g_bAnimCulling && g_bAnimVisibleOnly) {
        int i = m_lAnimated.num;
        while (i) {
            i--;
            if (i < m_lAnimated.num)
                m_lAnimated[i]->Animate(n);
        }
        i = m_lChildren.num;
        while (i) {
            i--;
            if (i < m_lChildren.num && !m_lChildren[i]->m_b16)
                m_lChildren[i]->Animate(n);
        }
    }
    else {
        int i = m_lChildren.num;
        while (i) {
            i--;
            if (i < m_lChildren.num)
                m_lChildren[i]->Animate(n);
        }
    }
    g_pAnimatingScene = 0;
    g_nAnimEndTime = AurGetTime();
    g_fAnimSeconds = (float)(int)(g_nAnimEndTime - g_nAnimStartTime) * 0.001f;
}

static inline Scene* GetMainScene()
{
    Scene* pScene = FindScene("mainscene");
    if (!pScene)
        pScene = FindScene("scene");
    return pScene;
}

// FUNCTION: SWKOTOR 0x004530D0
bool AurSetGlobalWind(float x, float y, float z)
{
    Scene* pScene = GetMainScene();
    if (pScene) {
        Vector vWind(x, y, z);
        pScene->m_pWind->SetWind(vWind);
        return true;
    }
    return false;
}

// FUNCTION: SWKOTOR 0x00453140
bool AurSetGlobalWindVariation(float a, float b)
{
    Scene* pScene = GetMainScene();
    if (pScene) {
        pScene->m_pWind->SetVariation(a * 0.017453292f, b * 0.017453292f);
        return true;
    }
    return false;
}

// FUNCTION: SWKOTOR 0x00453900
void Scene::ChildRemove(ScenePart* pPart)
{
    if (pPart->m_n58) {
        SceneRemoveSpecialPart(this, pPart);
        UnlistSpecialParts(pPart->m_p6C);
    }
    m_lChildren.Remove(pPart);
    if (pPart->IsAnimated())
        m_lAnimated.Remove(pPart);
}

// FUNCTION: SWKOTOR 0x004554B0 SYMBOL: ?RemoveFromSceneManager@@YAXPAVScenePart@@@Z
void RemoveFromSceneManager(ScenePart* pPart)
{
    void* pLight = pPart->GetLight();
    void* pEmitter = pPart->GetEmitter();
    if (pLight)
        g_lSceneLights.RemoveIfPresent(pLight);
    if (pEmitter)
        g_lSceneEmitters.RemoveIfPresent(pEmitter);
}

// FUNCTION: SWKOTOR 0x00455540 SYMBOL: ?RemoveFromSceneManager@@YAXPAX@Z
void RemoveFromSceneManager(void* p)
{
    g_lSceneObjects.RemoveIfPresent(p);
}

// FUNCTION: SWKOTOR 0x00455670
void Scene::ChildAdd(ScenePart* pPart)
{
    if (pPart->m_n58) {
        SceneAddSpecialPart(this, pPart);
        ListSpecialPartsR(pPart->m_p6C);
    }
    m_lChildren.Add(pPart);
    if (pPart->IsAnimated())
        m_lAnimated.Add(pPart);
}

// FUNCTION: SWKOTOR 0x00456A60
void CHitInfo::ClearOutput()
{
    Vector0 vZero(0.0f, 0.0f, 0.0f);
    m_n18 = 0;
    m_pHit.Clear();
    m_v40 = vZero;
    m_v1C = vZero;
    m_v28 = vZero;
    m_n34 = 0;
    m_n4C = 0;
    m_n50 = 0;
    m_n54 = 0;
    m_n3C = -1;
}

// FUNCTION: SWKOTOR 0x00456AF0
void CHitInfo::Reset(void* p0, void* p4, int n14)
{
    m_p0 = p0;
    m_p4 = p4;
    m_n14 = n14;
    m_lHits.num = 0;
    m_n58 = 0;
    m_b5C = 0;
    m_b5D = 1;
    m_b5E = 0;
    m_b5F = 0;
    m_b60 = 0;
    ClearOutput();
}

// FUNCTION: SWKOTOR 0x00457820
SafePointerHost::~SafePointerHost()
{
    for (int i = num - 1; i >= 0; i--) {
        SafePointerBase* p = element[i];
        if (p->m_p)
            p->m_p->RemoveInline(p);
        p->m_p = 0;
    }
    if (element) {
        delete[] element;
        element = 0;
    }
    array_size = 0;
    num = 0;
}

// FUNCTION: SWKOTOR 0x004578F0
CHitInfo::CHitInfo(void* p0, void* p4, int n14)
{
    Reset(p0, p4, n14);
}

// FUNCTION: SWKOTOR 0x00457990
CAurScene::~CAurScene()
{
}

// FUNCTION: SWKOTOR 0x004579A0 SYMBOL: ??_GCAurScene@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00458E70
CAurScene* NewCAurScene(char* szName)
{
    return new Scene(szName);
}

void AurClearActiveBins();                              // 0x0046A610
void AurTextureRelease(void* p);                        // 0x00420D70
void BSPTraverse(BSPNodeBase* pNode, void (*pfn)(BSPNodeBase*), void* pUser);    // 0x004ACEA0
void SceneBSPDeleteRoomData(BSPNodeBase* pNode);        // 0x0046C850

struct SceneTileSlot {
    void* m_p;
    int m_n4;
    int m_n8;
};

extern CExoArrayList<Scene*> g_lScenes;                 // 0x007B9398
extern SceneTileSlot g_aSceneTileSlots[5000];           // 0x008194E4

// FUNCTION: SWKOTOR 0x00458BE0
Scene::~Scene()
{
    while (m_lChildren.num > 0)
        ((SceneGob*)m_lChildren.element[m_lChildren.num - 1])->AttachToScene(0);
    m_gob.AttachToScene(0);
    g_lScenes.Remove(this);
    if (m_pLights)
        delete m_pLights;
    if (m_pBSP)
        delete m_pBSP;
    AurClearActiveBins();
    while (m_lRooms.num > 0) {
        BSPTraverse(m_lRooms.element[m_lRooms.num - 1], SceneBSPDeleteRoomData, 0);
        if (m_lRooms.element[m_lRooms.num - 1])
            delete m_lRooms.element[m_lRooms.num - 1];
        m_lRooms.num--;
    }
    if (m_pWind)
        delete m_pWind;
    CAurObject* pTexture = m_pTexture.m_p;
    if (pTexture) {
        m_pTexture.Reset();
        AurTextureRelease(pTexture);
    }
    if (m_pE4) {
        void* p = m_pE4;
        m_pE4 = 0;
        AurTextureRelease(p);
    }
    if (m_pE8) {
        void* p = m_pE8;
        m_pE8 = 0;
        AurTextureRelease(p);
    }
    for (int i = 0; i < 5000; i++)
        g_aSceneTileSlots[i].m_p = 0;
}

// FUNCTION: SWKOTOR 0x00458ED0 SYMBOL: ??_GScene@@UAEPAXI@Z
