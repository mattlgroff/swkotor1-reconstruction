// CSWGuiClassSelection (swguiclassselection.o): the class selection screen
// shown after "New Game" (layout "ClassSel", vtable 0x00758020) and its six
// class slots CSWGuiClassSelChar (descriptive name).
#include "l/sw_gui_class_selection.h"

int __cdecl rand();

// FUNCTION: SWKOTOR 0x006DB6D0
CSWGuiClassSelChar::~CSWGuiClassSelChar()
{
}

// FUNCTION: SWKOTOR 0x006DB720
CSWGuiClassSelection::~CSWGuiClassSelection()
{
    for (int i = 0; i < 6; i++) {
        delete m_aChars[i].m_pCreature;
        m_aChars[i].m_pCreature = 0;
    }
    CResRef cRim("CHARGEN");
    if (g_pExoResMan->Exists(cRim, 0xbba, 0)) {
        g_pExoResMan->m_bFlag0 = 1;
    }
}

// FUNCTION: SWKOTOR 0x006DB8F0
void CSWGuiClassSelection::OnPanelAdded()
{
    g_pExoResMan->CleanDirectory("GAMEINPROGRESS:", 0, 0);
    g_pExoInput->CoolDownEvent(0x28, 500);
    g_pExoInput->CoolDownEvent(0x2e, 500);
    g_pAppManager->m_pClientExoApp->HideLoadScreen(1, 1);
    CSWGuiPanel::OnPanelAdded();
    g_pAppManager->m_pClientExoApp->StartMenuMusic(0, 0x7f, 2);
}

// FUNCTION: SWKOTOR 0x006DB9B0
void CSWGuiClassSelection::OnSelectButton(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        if (pControl->m_nEvent == 0x2d) {
            unsigned char nSound = pControl->m_nSound;
            m_pManager->PlayGuiSound(nSound);
        }
        int i;
        for (i = 0; i < 6; i++) {
            if (pControl == &m_aChars[i].m_btnSelect) {
                break;
            }
        }
        m_pSelected = m_aChars[i].m_pCreature;
        m_pManager->AddPanel(new CSWGuiMainCharGen(m_pManager, m_pSelected, this), 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006DBA70
void CSWGuiClassSelection::OnEnterButton(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        int i;
        for (i = 0; i < 6; i++) {
            if (pControl == &m_aChars[i].m_btnSelect) {
                break;
            }
        }
        CExoString sText;
        if (g_aClassChoices[i].m_nGender == 0) {
            sText = g_pTlkTable->GetSimpleString(0x286);
        } else {
            sText = g_pTlkTable->GetSimpleString(0x287);
        }
        if (g_aClassChoices[i].m_nClass == g_nClassScoundrel) {
            CExoString sSpace(" ");
            sText = sText + sSpace + g_pTlkTable->GetSimpleString(0x87);
        } else if (g_aClassChoices[i].m_nClass == g_nClassScout) {
            CExoString sSpace(" ");
            sText = sText + sSpace + g_pTlkTable->GetSimpleString(0x85);
        } else {
            CExoString sSpace(" ");
            sText = sText + sSpace + g_pTlkTable->GetSimpleString(0x86);
        }
        m_lblDesc.m_cText.m_cParams.SetStrRef(g_aClassChoices[i].m_nDescription);
        m_lblClass.m_cText.m_cParams.SetText(sText);
    }
}

// FUNCTION: SWKOTOR 0x006DBD30
void CSWGuiClassSelection::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x28:
        case 0x2e:
            m_pManager->PlayGuiSound(0);
            MarkForDelete();
            g_pAppManager->m_pClientExoApp->DisplayMainMenu(0);
            break;
        case 0x35:
            HandleInputEvent(0x2f, bDown);
            break;
        case 0x36:
            HandleInputEvent(0x30, bDown);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006DBDF0
void CSWGuiClassSelection::BeginPlayGame()
{
    g_pAppManager->m_pClientExoApp->BusyWaitMenuMusic();
    g_pAppManager->m_pClientExoApp->GetGuiInGame()->AdjustHideCount(1);
    CExoString sCommand;
    CClientExoApp* pApp = g_pAppManager->m_pClientExoApp;
    m_pSelected->m_pStats->m_n5c = *(volatile unsigned char*)&m_pSelected->m_pStats->m_n5c;
    m_pSelected->m_pStats->m_n5e = *(volatile unsigned char*)&m_pSelected->m_pStats->m_n5e;
    m_pSelected->m_pStats->m_n5d = *(volatile unsigned char*)&m_pSelected->m_pStats->m_n5d;
    m_pSelected->m_pStats->m_n120 = 0;
    g_pAppManager->CreateServer(0);
    m_pSelected->SaveCharGenCreature();
    pApp->GetNetLayer()->StartConnectToSession(0, pApp->GetClientName(), "", 10, g_nConnectionType);
    pApp->GetMessage()->SendPlayerToServer_01_00();
    sCommand.Format("%c%s.%s %s", 's', "Module", "Load", m_sModule);
    pApp->GetNetLayer()->SendMessageToPlayer(PLAYERID_SERVER, (unsigned char*)sCommand.CStr(), sCommand.GetLength(), 0);
    pApp->SetPlayerCharacterName("temp");
    MarkForDelete();
    pApp->SetInputClass(0, 1);
    pApp->SetLoadScreenByModuleName(m_sModule);
    pApp->SetLoadMusic(pApp->GetLoadMusicByModuleName(m_sModule));
    pApp->ShowLoadScreen(1, 1, 0);
    pApp->SetLoadScreenHint(pApp->GetNextLoadScreenHintSTRREF(m_sModule, 0));
}

// FUNCTION: SWKOTOR 0x006DC030
void CSWGuiClassSelection::Draw(float fDelta)
{
    CSWGuiZeroExtent extButton;
    CSWGuiZeroExtent extModel;
    for (int i = 0; i < 6; i++) {
        CSWGuiClassSelChar* pChar = &m_aChars[i];
        if (g_bDo3DGui && m_bFlag7) {
            CAurScene* pScene = pChar->m_cModel.m_cScene.m_pScene;
            if (pChar->m_pCreature->GetAnimBase()->GetModel(0xff)->GetScene() != pScene) {
                m_pSelected->AttachToScene(pScene);
            }
            pChar->m_pCreature->CGPauseCycle(fDelta, 0);
        }
        static_cast<CSWGuiExtent&>(extButton) = pChar->m_btnSelect.m_extent;
        static_cast<CSWGuiExtent&>(extModel) = pChar->m_cModel.m_extent;
        int nTop = extButton.top;
        int nDelta;
        if (pChar->m_btnSelect.m_bFlag0) {
            int nSpan = nTop - 0x76;
            if (nSpan == 0) {
                pChar->m_fTimer = -1.0f;
                continue;
            }
            if (pChar->m_fTimer == -1.0f) {
                pChar->m_fTimer = (nSpan / 10) * 0.25f;
                continue;
            }
            float fLeft = pChar->m_fTimer - fDelta;
            pChar->m_fTimer = fLeft;
            int nTarget;
            if (fLeft > 0.0f) {
                nTarget = 0x80 - (int)((0.25f - fLeft) / 0.25f * 10.0f);
                if (nTarget <= 0x76) {
                    pChar->m_fTimer = -1.0f;
                    nTarget = 0x76;
                }
            } else {
                pChar->m_fTimer = -1.0f;
                nTarget = 0x76;
            }
            nDelta = nTop - nTarget;
            if (nDelta == 0) {
                continue;
            }
            extButton.top = nTop - nDelta;
            extButton.left -= nDelta;
            extButton.width += nDelta * 2;
            extButton.height += nDelta * 2;
            pChar->m_btnSelect.SetExtent(extButton);
            extModel.top -= nDelta;
            extModel.left -= nDelta;
            extModel.width += nDelta * 2;
            extModel.height += nDelta * 2;
        } else {
            int nSpan = 0x80 - nTop;
            if (nSpan == 0) {
                pChar->m_fTimer = -1.0f;
                continue;
            }
            if (pChar->m_fTimer == -1.0f) {
                pChar->m_fTimer = (nSpan / 10) * 0.25f;
                continue;
            }
            float fLeft = pChar->m_fTimer - fDelta;
            pChar->m_fTimer = fLeft;
            int nTarget;
            if (fLeft > 0.0f) {
                nTarget = 0x76 - (int)((0.25f - fLeft) / 0.25f * -10.0f);
                if (nTarget >= 0x80) {
                    pChar->m_fTimer = -1.0f;
                    nTarget = 0x80;
                }
            } else {
                pChar->m_fTimer = -1.0f;
                nTarget = 0x80;
            }
            nDelta = nTarget - nTop;
            if (nDelta == 0) {
                continue;
            }
            extButton.top = nTop + nDelta;
            extButton.left += nDelta;
            extButton.width -= nDelta * 2;
            extButton.height -= nDelta * 2;
            pChar->m_btnSelect.SetExtent(extButton);
            extModel.top += nDelta;
            extModel.left += nDelta;
            extModel.width -= nDelta * 2;
            extModel.height -= nDelta * 2;
        }
        pChar->m_cModel.SetExtent(extModel);
    }
    CSWGuiPanel::Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x006DC350
CSWGuiClassSelChar::CSWGuiClassSelChar()
{
}

// FUNCTION: SWKOTOR 0x006DC330 SYMBOL: ??_GCSWGuiClassSelection@@UAEPAXI@Z
// Emitted with the destructor above.

// FUNCTION: SWKOTOR 0x006DC3C0
CSWGuiClassSelection::CSWGuiClassSelection(CSWGuiManager* pManager, const CExoString& sModule)
    : CSWGuiPanel(pManager)
{
    CExoString sName;
    Vector vPosition(0.0f, 0.0f, 0.0f);
    Quaternion qOrientation(0.0f, 0.0f, 0.0f, 1.0f);
    g_pAppManager->m_pClientExoApp->SetLoadScreenByModuleName("classsel");
    g_pAppManager->m_pClientExoApp->SetLoadScreenHint(0);
    g_pAppManager->m_pClientExoApp->ShowLoadScreen(1, 0, 0);
    RunFrame(1.0f / 30.0f, 0, 0);
    {
        CResRef cRim("CHARGEN");
        if (g_pExoResMan->Exists(cRim, 0xbba, 0)) {
            g_pExoResMan->AddResourceImageFile("RIMS:CHARGEN");
        }
    }
    g_pAppManager->m_pClientExoApp->SetLoadBarProgress(6, 1);
    RunFrame(1.0f / 30.0f, 0, 0);
    g_pAppManager->m_pClientExoApp->SetLoadBarProgress(12, 1);
    RunFrame(1.0f / 30.0f, 0, 0);
    {
        CResRef cLayout("ClassSel");
        StartLoadFromLayout(cLayout);
    }
    C2DA* pPortraits = new C2DA("PORTRAITS", 0);
    pPortraits->Load2DArray();
    int nRows = pPortraits->m_nNumRows;
    CExoArrayList<int> lstMale;
    CExoArrayList<int> lstFemale;
    for (int nRow = 0; nRow < nRows; nRow++) {
        int bForPC;
        pPortraits->GetINTEntry(nRow, "ForPC", &bForPC);
        if (bForPC) {
            int nSex;
            pPortraits->GetINTEntry(nRow, "Sex", &nSex);
            if (nSex == 0) {
                lstMale.Add(nRow);
            } else if (nSex == 1) {
                lstFemale.Add(nRow);
            }
        }
    }
    g_pAppManager->m_pClientExoApp->SetLoadBarProgress(18, 1);
    RunFrame(1.0f / 30.0f, 0, 0);
    int nProgress = 3;
    int nPortrait;
    for (int i = 0; i < 6; i++) {
        CSWGuiClassChoice* pChoice = &g_aClassChoices[i];
        sName.Format("BTN_SEL%d", i + 1);
        InitControl(&m_aChars[i].m_btnSelect, sName, 1);
        sName.Format("3D_MODEL%d", i + 1);
        InitControl(&m_aChars[i].m_cModel, sName, 1);
        m_aChars[i].m_btnSelect.m_cBorder.m_cParams.m_vColor = g_vDifficultyExitColor;
        m_aChars[i].m_btnSelect.m_cHilight.m_cParams.m_vColor = g_vAbilityColorGranted;
        m_aChars[i].m_btnSelect.m_cHilight.m_cParams.SetFlags(1, 1, 0);
        m_aChars[i].m_btnSelect.AddEvent(0, this,
            (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiClassSelection::OnEnterButton);
        m_aChars[i].m_btnSelect.AddEvent(0x27, this,
            (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiClassSelection::OnSelectButton);
        m_aChars[i].m_btnSelect.AddEvent(0x2d, this,
            (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiClassSelection::OnSelectButton);
        CAurScene* pScene = m_aChars[i].m_cModel.m_cScene.m_pScene;
        CAurCamera* pCamera = m_aChars[i].m_cModel.m_cScene.m_pCamera;
        if (g_bDo3DGui) {
            pScene->SetRoom("gui3D_room", vPosition, qOrientation);
            CAurObject* pLight = m_aChars[i].m_cModel.m_cScene.AddModel("cgbody_light", -1);
            if (pLight) {
                pCamera->AttachToHook(pLight, "camerahook", 0);
                pCamera->SetFieldOfView(22.726f);
            }
        }
        g_pAppManager->m_pClientExoApp->SetLoadBarProgress((int)(++nProgress * 0.0625f * 100.0f), 1);
        RunFrame(1.0f / 30.0f, 0, 0);
        CSWCCreature* pCreature = new CSWCCreature();
        pCreature->m_pStats->m_nGender = pChoice->m_nGender;
        pCreature->m_pStats->SetClass(0, pChoice->m_nClass);
        pCreature->SetSoundSet(pChoice->m_nSoundSet);
        CSWCCreatureAppearance cAppearance = *pCreature->m_pAppearance;
        cAppearance.m_nGender = pCreature->m_pStats->m_nGender;
        if (cAppearance.m_nGender == 0) {
            int n = rand() % lstMale.num;
            nPortrait = lstMale.element[n];
            lstMale.DelIndex(n);
        } else if (cAppearance.m_nGender == 1) {
            int n = rand() % lstFemale.num;
            nPortrait = lstFemale.element[n];
            lstFemale.DelIndex(n);
        }
        int nAppearance;
        switch (pChoice->m_nClass) {
        case 2:
            pPortraits->GetINTEntry(nPortrait, "Appearance_S", &nAppearance);
            break;
        case 1:
            pPortraits->GetINTEntry(nPortrait, "AppearanceNumber", &nAppearance);
            break;
        case 0:
            pPortraits->GetINTEntry(nPortrait, "Appearance_L", &nAppearance);
            break;
        default:
            pPortraits->GetINTEntry(nPortrait, "AppearanceNumber", &nAppearance);
            break;
        }
        pCreature->SetPortraitId(nPortrait);
        pCreature->m_pStats->m_nAppearance = nAppearance;
        cAppearance.m_nAppearance = nAppearance;
        cAppearance.m_n14 = 1;
        cAppearance.m_nType = 2;
        cAppearance.m_n01 = 1;
        if (g_bDo3DGui) {
            pCreature->SetAppearance(&cAppearance, 3, 1);
            pCreature->AttachToScene(pScene);
            pCreature->SetPosition(vPosition);
            pCreature->CGPauseCycle(0.0f, 1);
        }
        m_aChars[i].m_pCreature = pCreature;
        m_aChars[i].m_fTimer = -1.0f;
        g_pAppManager->m_pClientExoApp->SetLoadBarProgress((int)(++nProgress * 0.0625f * 100.0f), 1);
        RunFrame(1.0f / 30.0f, 0, 0);
    }
    InitControl(&m_lblInstruction, "LBL_INSTRUCTION", 1);
    InitControl(&m_lblCharGen, "LBL_CHAR_GEN", 1);
    InitControl(&m_lblDesc, "LBL_DESC", 1);
    InitControl(&m_lblClass, "LBL_CLASS", 1);
    InitControl(&m_btnBack, "BTN_BACK", 1);
    UnloadResource();
    m_btnBack.m_bNavigable = 0;
    m_btnBack.AddEvent(0x27, this, &CSWGuiPanel::OnBButtonPressed);
    SetActiveControl(&m_aChars[0].m_btnSelect, 0);
    m_sModule = sModule;
    g_pAppManager->m_pClientExoApp->SetLoadBarProgress((int)(++nProgress * 0.0625f * 100.0f), 1);
    RunFrame(1.0f / 30.0f, 0, 0);
    delete pPortraits;
}
