// CSWGuiLevelUpCharGen constructor (layout "MAINCG"); the scalar deleting
// destructor is emitted with the vtable here.
#include "l/sw_gui_levelup_chargen.h"

// FUNCTION: SWKOTOR 0x006E8EF0
CSWGuiLevelUpCharGen::CSWGuiLevelUpCharGen(CSWGuiManager* pManager, unsigned long oidCreature,
                                           CSWCCreature* pPreview)
    : CSWGuiPanel(pManager)
{
    CClientExoApp* pClient = g_pAppManager->GetClientExoApp();
    CSWCCreature* pSource = pClient->GetCreatureByGameObjectID(oidCreature);
    m_pManager = pClient->GetGuiManager();
    CSWCCreature* pSnapshot = new CSWCCreature();
    m_oidCreature = oidCreature;
    m_pSnapshot = pSnapshot;
    CopyCreatureStats(pSource, pSnapshot);
    CSWCCreature* pCreature = GetLiveCreature();
    if (pCreature) {
        CSWCCreatureStats* pStats = pCreature->m_pStats;
        unsigned char nIndex = pStats->m_nNumClasses - 1;
        ((CSWCLevelUpStats*)pStats)->SetClassLevel(nIndex, pStats->GetClassLevel(nIndex) + 1);
    }
    DetermineClass();
    StartLoadFromLayout("MAINCG");
    InitControl(&m_lblTitle, "MAIN_TITLE_LBL", 1);
    InitControl(&m_lblName, "LBL_NAME", 1);
    InitControl(&m_lblClass, "LBL_CLASS", 1);
    InitControl(&m_lblLevel, "LBL_LEVEL", 1);
    InitControl(&m_lblLevelVal, "LBL_LEVEL_VAL", 1);
    InitControl(&m_lblStr, "STR_LBL", 1);
    InitControl(&m_lblDex, "DEX_LBL", 1);
    InitControl(&m_lblCon, "CON_LBL", 1);
    InitControl(&m_lblInt, "INT_LBL", 1);
    InitControl(&m_lblWis, "WIS_LBL", 1);
    InitControl(&m_lblCha, "CHA_LBL", 1);
    InitControl(&m_lblStrAb, "STR_AB_LBL", 1);
    InitControl(&m_lblDexAb, "DEX_AB_LBL", 1);
    InitControl(&m_lblConAb, "CON_AB_LBL", 1);
    InitControl(&m_lblIntAb, "INT_AB_LBL", 1);
    InitControl(&m_lblWisAb, "WIS_AB_LBL", 1);
    InitControl(&m_lblChaAb, "CHA_AB_LBL", 1);
    InitControl(&m_lblVit, "LBL_VIT", 1);
    InitControl(&m_lblDef, "LBL_DEF", 1);
    InitControl(&m_lblNewWill, "NEW_WILL_LBL", 1);
    InitControl(&m_lblNewRefl, "NEW_REFL_LBL", 1);
    InitControl(&m_lblNewFort, "NEW_FORT_LBL", 1);
    InitControl(&m_lblBevelL, "LBL_BEVEL_L", 1);
    InitControl(&m_lblBevelM, "LBL_BEVEL_M", 1);
    InitControl(&m_lblBevelR, "LBL_BEVEL_R", 1);
    InitControl(&m_lblFortitude, "LBL_FORTITUDE", 1);
    InitControl(&m_lblReflex, "LBL_REFLEX", 1);
    InitControl(&m_lblWill, "LBL_WILL", 1);
    InitControl(&m_cModel, "MODEL_LBL", 1);
    InitControl(&m_lblPortrait, "PORTRAIT_LBL", 1);
    UnloadResource();
    m_lblTitle.m_cText.SetStrRef(0x42F);
    m_pLevelUpPanel = new CSWGuiLevelUpPanel(m_pManager, m_oidCreature);
    m_pLevelUpPanel->m_pParent = this;
    SetNameString(((CSWCCreatureVirtuals*)m_pSnapshot)->GetDisplayName());
    AbilitiesChanged();
    m_lblPortrait.m_cBorder.SetFillImage(GetCreaturePortrait(pSource, 1), 0);
    CAurCamera* pCamera = m_cModel.m_cScene.m_pCamera;
    CAurScene* pRoom = m_cModel.m_cScene.m_pScene;
    if (g_bDo3DGui) {
        pRoom->SetRoom("gui3D_room", Vector(0.0f, 0.0f, 0.0f), Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
        m_cModel.m_cScene.AddModel(CExoString("charrec_light"), -1);
        pPreview->SetUp3DScene(&m_cModel, pSource, 0, 1);
        ((CSWCCreatureVirtuals*)pPreview)->SetGuiScene(pRoom);
        ((CSWCCreatureVirtuals*)pPreview)->SetGuiPosition(Vector(0.0f, 0.0f, 0.0f));
        CAurCameraHook* pHook = pCamera->GetHook(0x3EA);
        if (pHook) {
            pHook->m_bEnabled = 1;
            pHook->m_vPosition = Vector(0.0f, 0.0f, ((CSWCCreatureVirtuals*)pPreview)->GetModelHeight());
        }
    }
    m_nInputClass = g_pAppManager->GetClientExoApp()->GetInputClass();
}

// FUNCTION: SWKOTOR 0x006E89D0 SYMBOL: ??_GCSWGuiLevelUpCharGen@@UAEPAXI@Z
// Emitted by the constructor above; the regular destructor is defined in
// sw_gui_levelup_chargen.cpp.
