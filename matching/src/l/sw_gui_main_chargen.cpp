// CSWGuiMainCharGen (swguimainchargen.o): the character creation frame
// (layout "MAINCG") hosting the quick-or-custom, quick and custom panels.
#include "l/sw_gui_main_chargen.h"

// FUNCTION: SWKOTOR 0x006EA760
void CSWGuiMainCharGen::SetVisiblePanel(int nPanel)
{
    if (nPanel == m_nVisiblePanel) {
        return;
    }
    m_pManager->PopModalPanel();
    switch (m_nVisiblePanel) {
    case 1:
        m_pManager->RemovePanel(m_pQuickOrCustom);
        break;
    case 2:
        m_pManager->RemovePanel(m_pQuick);
        break;
    case 3:
        m_pManager->RemovePanel(m_pCustom);
        break;
    default:
        m_nVisiblePanel = 4;
        break;
    }
    if (m_nVisiblePanel == 4) {
        return;
    }
    switch (nPanel) {
    case 1:
        m_pManager->AddPanel(m_pQuickOrCustom, 1, 1);
        break;
    case 2:
        m_pManager->AddPanel(m_pQuick, 1, 1);
        break;
    case 3:
        m_pManager->AddPanel(m_pCustom, 1, 1);
        break;
    default:
        m_nVisiblePanel = 4;
        break;
    }
    if (m_nVisiblePanel != 4) {
        m_nVisiblePanel = nPanel;
    }
}

// FUNCTION: SWKOTOR 0x006EA810
void CSWGuiMainCharGen::OnPanelAdded()
{
    m_pManager->AddPanel(m_pQuickOrCustom, 1, 1);
    CSWGuiPanel::OnPanelAdded();
}

// FUNCTION: SWKOTOR 0x006EA830
void CSWGuiMainCharGen::RemoveSubPanels()
{
    m_pManager->PopModalPanel();
    if (m_pManager->PanelExists(m_pCustom)) {
        m_pCustom->m_nFlags = (m_pCustom->m_nFlags & 0xFFFFFCFFul) | 0x400ul;
    } else {
        delete m_pCustom;
        m_pCustom = 0;
    }
    if (m_pManager->PanelExists(m_pQuickOrCustom)) {
        m_pQuickOrCustom->m_nFlags = (m_pQuickOrCustom->m_nFlags & 0xFFFFFCFFul) | 0x400ul;
    } else {
        delete m_pQuickOrCustom;
        m_pQuickOrCustom = 0;
    }
    if (m_pManager->PanelExists(m_pQuick)) {
        m_pQuick->m_nFlags = (m_pQuick->m_nFlags & 0xFFFFFCFFul) | 0x400ul;
    } else {
        delete m_pQuick;
        m_pQuick = 0;
    }
    m_pManager->PopModalPanel();
    m_nFlags = (m_nFlags & 0xFFFFFCFFul) | 0x400ul;
}

// FUNCTION: SWKOTOR 0x006EA910
void CSWGuiMainCharGen::DetermineClass()
{
    m_sClassName = GetStats()->GetClassDescription(' ');
    m_sClassName = m_sClassName.Left(m_sClassName.Find('(', 0));
}

// FUNCTION: SWKOTOR 0x006EA9B0
void CSWGuiMainCharGen::SetNameString(const CExoString& sName)
{
    m_sName = sName;
    m_lblName.m_cText.SetText(m_sName);
    m_lblClass.m_cText.SetText(m_sClassName);
}

// FUNCTION: SWKOTOR 0x006EA9F0
void CSWGuiMainCharGen::PortraitChanged()
{
    m_lblPortrait.SetFillImage(((CSWCCreatureVirtuals*)m_pCreature)->GetPortrait(0));
    {
        CExoString sEmpty("");
        m_lblPortrait.m_cText.SetText(sEmpty);
    }
    CAurCamera* pCamera = m_cModel.m_cScene.m_pCamera;
    CSWCCreatureAppearance cAppearance = *m_pCreature->m_pAppearance;
    cAppearance.m_nAppearance = GetStats()->m_nAppearance;
    if (g_bDo3DGui) {
        m_pCreature->SetAppearance(&cAppearance, 3, 1);
        GetCreatureV()->SetGuiScene(m_cModel.m_cScene.m_pScene);
        GetCreatureV()->SetGuiPosition(Vector(0.0f, 0.0f, 0.0f));
        CAurCameraHook* pHook = pCamera->GetHook(0x3EA);
        if (pHook) {
            pHook->m_bEnabled = 1;
            pHook->m_vPosition = Vector(0.0f, 0.0f, GetCreatureV()->GetModelHeight());
        }
    }
    float fHeight = GetCreatureV()->GetModelHeight();
    Vector vEye(2.5f, 0.0f, fHeight);
    pCamera->SetPosition(vEye);
    vEye -= Vector(0.0f, 0.0f, 1.0f);
    vEye.Normalize();
    pCamera->SetOrientation(YawPitchRoll(FUN_004aa0f0(vEye) + 180.0f, 90.0f - FUN_004aa130(vEye), 0.0f));
}

// FUNCTION: SWKOTOR 0x006EAC30
void CSWGuiMainCharGen::AbilitiesChanged()
{
    GetStats()->SetFortSavingThrow();
    GetStats()->SetWillSavingThrow();
    GetStats()->SetReflexSavingThrow();
    {
        CExoString sValue(GetStats()->m_nCHABase);
        m_lblChaAb.m_cText.SetText(sValue);
    }
    {
        CExoString sValue(GetStats()->m_nSTRBase);
        m_lblStrAb.m_cText.SetText(sValue);
    }
    {
        CExoString sValue(GetStats()->m_nINTBase);
        m_lblIntAb.m_cText.SetText(sValue);
    }
    {
        CExoString sValue(GetStats()->m_nWISBase);
        m_lblWisAb.m_cText.SetText(sValue);
    }
    {
        CExoString sValue(GetStats()->m_nCONBase);
        m_lblConAb.m_cText.SetText(sValue);
    }
    {
        CExoString sValue(GetStats()->m_nDEXBase);
        m_lblDexAb.m_cText.SetText(sValue);
    }
    CSWCCreatureStats* pStats = GetStats();
    unsigned char nDex = pStats->m_nDEX;
    int nDefense = (char)(pStats->GetAbilityMod(nDex) + pStats->m_nACBonus) + 10;
    CSWClassL* pClass;
    for (int i = 0; i < GetStats()->m_nNumClasses; i++) {
        pClass = (CSWClassL*)&g_pRules->m_pClasses[GetStats()->GetClass(i)];
        nDefense += pClass->GetACBonus(GetStats()->GetClassLevel(i));
    }
    GetStats()->m_nACNaturalBase = nDefense;
    pStats = GetStats();
    unsigned char nHitDie = pClass->m_nHitDie;
    short nHitPoints = pStats->m_nBaseHitPoints;
    unsigned char nCon = pStats->m_nCON;
    int nVitality = pStats->GetAbilityMod(nCon) + nHitPoints + nHitDie;
    {
        CExoString sValue(GetStats()->GetArmorClass(0));
        m_lblDef.m_cText.SetText(sValue);
    }
    {
        CExoString sValue(GetStats()->m_nFortSave);
        m_lblNewFort.m_cText.SetText(sValue);
    }
    {
        CExoString sValue(GetStats()->m_nReflexSave);
        m_lblNewRefl.m_cText.SetText(sValue);
    }
    {
        CExoString sValue(nVitality);
        m_lblVit.m_cText.SetText(sValue);
    }
    {
        CExoString sValue(GetStats()->m_nWillSave);
        m_lblNewWill.m_cText.SetText(sValue);
    }
}

// FUNCTION: SWKOTOR 0x006EB010
void CSWGuiMainCharGen::ClearAbilities()
{
    m_lblChaAb.m_cText.SetText("");
    m_lblStrAb.m_cText.SetText("");
    m_lblIntAb.m_cText.SetText("");
    m_lblWisAb.m_cText.SetText("");
    m_lblConAb.m_cText.SetText("");
    m_lblDexAb.m_cText.SetText("");
    GetStats()->SetSTRBase(8);
    GetStats()->SetINTBase(8);
    GetStats()->SetWISBase(8);
    GetStats()->SetCONBase(8);
    GetStats()->SetDEXBase(8, 1);
    GetStats()->SetCHABase(8);
    GetStats()->SetFortSavingThrow();
    GetStats()->SetWillSavingThrow();
    GetStats()->SetReflexSavingThrow();
    m_lblDef.m_cText.SetText("");
    m_lblNewFort.m_cText.SetText("");
    m_lblNewRefl.m_cText.SetText("");
    m_lblVit.m_cText.SetText("");
    m_lblNewWill.m_cText.SetText("");
    GetStats()->ClearPackage();
}

// FUNCTION: SWKOTOR 0x006EB320
void CSWGuiMainCharGen::BeginPlayGame()
{
    RemoveSubPanels();
    m_pClassSelection->BeginPlayGame();
}

// FUNCTION: SWKOTOR 0x006EB340
void CSWGuiMainCharGen::Draw(float fDelta)
{
    if (m_pCreature && g_bDo3DGui && (m_nFlags & 0x80)) {
        CAurObject* pModel = m_pCreature->GetAnimBase()->GetModel(0xff);
        CAurScene* pScene = m_cModel.m_cScene.m_pScene;
        if (pModel->GetScene() != pScene) {
            GetCreatureV()->SetGuiScene(m_cModel.m_cScene.m_pScene);
            CAurCameraHook* pHook = m_cModel.m_cScene.m_pCamera->GetHook(0x3EA);
            if (pHook) {
                pHook->m_bEnabled = 1;
                pHook->m_vPosition = Vector(0.0f, 0.0f, GetCreatureV()->GetModelHeight());
            }
        }
        m_pCreature->CGPauseCycle(fDelta, 0);
    }
    CSWGuiPanel::Draw(fDelta);
}
