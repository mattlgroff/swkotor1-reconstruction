// CSWGuiScene destructor / Draw and its model list helpers
// (0x00414fb0, 0x004165e0, 0x004174b0, 0x00417620, ??_G 0x004175a0). The
// rest of the class lives in gui_parts.cpp; the destructor is the key
// function, so the vtable and ??_G are emitted here.
#include "d/gui_parts.h"

// Only the image-file flags of CExoResMan are needed here.
class CExoResMan {
public:
    int RemoveResourceImageFile(const CExoString& sName);   // 0x00408830

    char pad_0x0[0x34];
    unsigned char m_bChargenLoaded : 1;     // 0x34 bit 0 (RIMS:CHARGEN)
    unsigned char m_bMainMenuLoaded : 1;    // 0x34 bit 1 (RIMS:MAINMENU)
};

extern CExoResMan* g_pExoResMan;            // 0x007a39e8

extern "C" int __cdecl AurGUISetupViewport(int x, int y, int w, int h, Vector* pAmbient, int a, float f); // 0x004592f0
extern "C" void __cdecl AurGUICloseViewport();  // 0x00459580


// FUNCTION: SWKOTOR 0x00414fb0
void CSWGuiScene::Draw(float fDelta)
{
    m_pScene->Update(fDelta);
    if (AurGUISetupViewport(m_extent.left, m_extent.top, m_extent.width, m_extent.height, &m_vAmbient, 1, 1.0f)) {
        m_pCamera->Render();
        AurGUICloseViewport();
    }
}

// FUNCTION: SWKOTOR 0x004165e0
CSWGuiScene::~CSWGuiScene()
{
    for (int i = 0; i < m_lstModels.num; i++) {
        if (m_lstModels[i]) {
            m_lstModels[i]->Destroy(1);
        }
    }
    m_lstModels.num = 0;
    if (m_pCamera) {
        m_pCamera->Destroy(1);
        m_pCamera = 0;
    }
    if (m_pScene) {
        m_pScene->Destroy(1);
        m_pScene = 0;
    }
    if (g_pExoResMan->m_bMainMenuLoaded) {
        g_pExoResMan->RemoveResourceImageFile("RIMS:MAINMENU");
        g_pExoResMan->m_bMainMenuLoaded = 0;
    }
    if (g_pExoResMan->m_bChargenLoaded) {
        g_pExoResMan->RemoveResourceImageFile("RIMS:CHARGEN");
        g_pExoResMan->m_bChargenLoaded = 0;
    }
}

// FUNCTION: SWKOTOR 0x004174b0
CSWGuiScene::CSWGuiScene()
{
    m_pScene = NewCAurScene("scene");
    m_pCamera = NewCAurCamera("camera");
    if (m_pScene && m_pCamera) {
        m_pScene->SetLighting(10, 10, 5, 1, 10.0f);
        m_pScene->Initialize();
        m_pScene->SetActive(0);
        m_pCamera->SetClipPlanes(0.1f, 10000.0f);
        m_pCamera->SetScene(m_pScene);
        m_vAmbient = g_vGuiSceneAmbient;
    }
}

// FUNCTION: SWKOTOR 0x00417620
CAurObject* CSWGuiScene::AddModel(const CExoString& sModel, int nIndex)
{
    CAurObject* pModel = NewCAurObject(sModel.CStr(), "", 0, 0);
    return AddModel(pModel, nIndex, 1) ? pModel : 0;
}

// FUNCTION: SWKOTOR 0x004175a0 SYMBOL: ??_GCSWGuiScene@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00414f80 SYMBOL: ??1CSWGuiDrawable@@UAE@XZ

// FUNCTION: SWKOTOR 0x00414f90 SYMBOL: ??_GCSWGuiDrawable@@UAEPAXI@Z
