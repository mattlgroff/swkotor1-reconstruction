// CSWGuiPortraitCharGen constructor (layout "PORTCUST"); the scalar deleting
// destructor is emitted with the vtable here.
#include "o/sw_gui_main_chargen.h"

// FUNCTION: SWKOTOR 0x006F9430
CSWGuiPortraitCharGen::CSWGuiPortraitCharGen(CSWGuiManager* pManager, CSWCCreature* pCreature)
    : CSWGuiPanel(pManager)
{
    m_pCreature = pCreature;
    StartLoadFromLayout("PORTCUST");
    InitControl(&m_lblTitle, "MAIN_TITLE_LBL", 1);
    InitControl(&m_lblSubTitle, "SUB_TITLE_LBL", 1);
    InitControl(&m_lblPortrait, "LBL_PORTRAIT", 1);
    InitControl(&m_cHead, "LBL_HEAD", 1);
    InitControl(&m_aBevels[0], "LBL_BEVEL_L", 1);
    InitControl(&m_aBevels[1], "LBL_BEVEL_M", 1);
    InitControl(&m_aBevels[2], "LBL_BEVEL_R", 1);
    InitControl(&m_aBevels[3], "LBL_BEVEL_T", 1);
    InitControl(&m_aBevels[4], "LBL_BEVEL_B", 1);
    InitControl(&m_btnBack, "BTN_BACK", 1);
    InitControl(&m_btnAccept, "BTN_ACCEPT", 1);
    InitControl(&m_btnLeft, "BTN_ARRL", 1);
    InitControl(&m_btnRight, "BTN_ARRR", 1);
    UnloadResource();
    m_nOriginalPortraitId = GetStats()->m_nPortraitId;
    m_nCurrent = 0;
    GetPortraitsList(GetStats()->m_nGender);
    CAurScene* pScene = m_cHead.m_cScene.m_pScene;
    CAurCamera* pCamera = m_cHead.m_cScene.m_pCamera;
    if (g_bDo3DGui) {
        CExoString sHook;
        sHook.Format("camerahook%c", GetStats()->m_nGender == 0 ? 'm' : 'f');
        pScene->SetRoom("gui3D_room", Vector(0.0f, 0.0f, 0.0f), Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
        CAurObject* pModel;
        {
            CExoString sLight("cghead_light");
            pModel = m_cHead.m_cScene.AddModel(sLight, -1);
        }
        if (pModel) {
            pModel->PlayAnimation("default", 1.0f, 0, 0);
            pCamera->AttachToHook(pModel, sHook.CStr(), 1);
            pCamera->SetFieldOfView(22.726f);
        }
    }
    CSWGuiButton* pButton = &m_btnBack;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    pButton = &m_btnAccept;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent27);
    pButton = &m_btnRight;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPortraitCharGen::OnRightArrow);
    pButton = &m_btnLeft;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPortraitCharGen::OnLeftArrow);
    UpdatePortraitButton();
    m_fIdleTimer = 0.0f;
}

// FUNCTION: SWKOTOR 0x006F9A30 SYMBOL: ??_GCSWGuiPortraitCharGen@@UAEPAXI@Z
// Emitted by the constructor above.
