// CSWGuiMainCharGen constructor and destructors (layout "MAINCG").
#include "l/sw_gui_main_chargen.h"

// FUNCTION: SWKOTOR 0x006EB420
CSWGuiMainCharGen::CSWGuiMainCharGen(CSWGuiManager* pManager, CSWCCreature* pCreature,
                                     CSWGuiClassSelection* pClassSel)
    : CSWGuiPanel(pManager)
{
    m_pClassSelection = pClassSel;
    m_pManager = g_pAppManager->GetClientExoApp()->GetGuiManager();
    m_pCreature = pCreature;
    DetermineClass();
    StartLoadFromLayout("MAINCG");
    InitControl(&m_lblTitle, "MAIN_TITLE_LBL", 1);
    InitControl(&m_lblName, "LBL_NAME", 1);
    InitControl(&m_lblClass, "LBL_CLASS", 1);
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
    InitControl(&m_lblNewWill, "NEW_WILL_LBL", 1);
    InitControl(&m_lblNewRefl, "NEW_REFL_LBL", 1);
    InitControl(&m_lblNewFort, "NEW_FORT_LBL", 1);
    InitControl(&m_lblBevelL, "LBL_BEVEL_L", 1);
    InitControl(&m_lblBevelM, "LBL_BEVEL_M", 1);
    InitControl(&m_lblBevelR, "LBL_BEVEL_R", 1);
    InitControl(&m_lblFortitude, "LBL_FORTITUDE", 1);
    InitControl(&m_lblReflex, "LBL_REFLEX", 1);
    InitControl(&m_lblWill, "LBL_WILL", 1);
    InitControl(&m_lblVit, "LBL_VIT", 1);
    InitControl(&m_lblDef, "LBL_DEF", 1);
    InitControl(&m_cModel, "MODEL_LBL", 1);
    InitControl(&m_lblPortrait, "PORTRAIT_LBL", 1);
    UnloadResource();
    m_pQuickOrCustom = new CSWGuiQuickOrCustomPanel(m_pManager, m_pCreature);
    m_pQuickOrCustom->m_pParent = this;
    m_pQuick = new CSWGuiQuickPanel(m_pManager, m_pCreature);
    m_pQuick->m_pParent = this;
    m_pCustom = new CSWGuiCustomPanel(m_pManager, m_pCreature);
    m_pCustom->SetParentPanel(this);
    m_nVisiblePanel = 1;
    {
        CExoString sEmpty("");
        m_sName = sEmpty;
        m_lblName.m_cText.SetText(m_sName);
        m_lblClass.m_cText.SetText(m_sClassName);
    }
    m_lblPortrait.SetFillImage(((CSWCCreatureVirtuals*)m_pCreature)->GetPortrait(0));
    CAurScene* pRoom = m_cModel.m_cScene.m_pScene;
    CAurCamera* pCamera = m_cModel.m_cScene.m_pCamera;
    if (g_bDo3DGui) {
        pRoom->SetRoom("gui3D_room", Vector(0.0f, 0.0f, 0.0f), Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
        {
            CExoString sLight("charrec_light");
            m_cModel.m_cScene.AddModel(sLight, -1);
        }
        m_pCreature->SetUp3DScene(&m_cModel, 0, 0, 0);
        GetCreatureV()->SetGuiScene(pRoom);
        GetCreatureV()->SetGuiPosition(Vector(0.0f, 0.0f, 0.0f));
        CAurCameraHook* pHook = pCamera->GetHook(0x3EA);
        if (pHook) {
            pHook->m_bEnabled = 1;
            pHook->m_vPosition = Vector(0.0f, 0.0f, GetCreatureV()->GetModelHeight());
        }
    }
}

// FUNCTION: SWKOTOR 0x006EBEE0 SYMBOL: ??_GCSWGuiMainCharGen@@UAEPAXI@Z
// Emitted with the vtable by the constructor above.
