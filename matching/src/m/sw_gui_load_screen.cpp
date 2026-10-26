// The load screen panel CSWGuiLoadScreen (vtable 0x00752DC0,
// 0x0067A6D0-0x0067ABFF).

#include "m/sw_gui_main_menu.h"

// FUNCTION: SWKOTOR 0x0067A6D0
void CSWGuiLoadScreen::SetImage(const CResRef& cImage)
{
    if (g_pExoResMan->Exists(cImage, 3, 0) || g_pExoResMan->Exists(cImage, 0xbbf, 0)) {
        SetBackground(cImage);
    }
}

// FUNCTION: SWKOTOR 0x0067A710
CSWGuiLoadScreen::CSWGuiLoadScreen(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    {
        CResRef cLayout("loadscreen");
        StartLoadFromLayout(cLayout);
    }
    {
        CExoString sName("PB_PROGRESS");
        InitControl(&m_pbProgress, sName, 1);
    }
    {
        CExoString sName("LBL_HINT");
        InitControl(&m_lblHint, sName, 1);
    }
    {
        CExoString sName("LBL_LOGO");
        InitControl(&m_lblLogo, sName, 1);
    }
    {
        CExoString sName("LBL_LOADING");
        InitControl(&m_lblLoading, sName, 1);
    }
    UnloadResource();
    m_pbProgress.SetCurValue(0);
    {
        CExoString sEmpty("");
        m_lblHint.m_cText.m_cParams.SetText(sEmpty);
    }
    m_lblImage.SetExtent(CSWGuiExtent(0, 0, pManager->m_nWidth, pManager->m_nHeight));
    CExoString sImage = CSWGuiManager::GetScreenResolutionString(g_nScreenWidth, g_nScreenHeight);
    {
        CExoString sLoad("load");
        sImage = sImage + sLoad;
    }
    CSWGuiBorderParams* pParams;
    {
        CResRef cImage(sImage);
        pParams = &m_lblImage.m_cBorder.m_cParams;
        pParams->SetFillImage(cImage, 0);
    }
    pParams->m_bFlag0 = 0;
    pParams->m_bFlag1 = 1;
    m_nFlags |= 0x60;
}

// FUNCTION: SWKOTOR 0x0067A9C0
void CSWGuiLoadScreen::SetHint(int nHint)
{
    CExoString sText;
    if (nHint) {
        m_nHint = nHint;
        sText = g_pTlkTable->GetSimpleString(nHint);
        g_pTlkTable->ParseTokens(&sText);
    }
    else {
        m_nHint = 0;
    }
    m_lblHint.m_cText.m_cParams.SetText(sText);
}

// FUNCTION: SWKOTOR 0x0067AA80
void CSWGuiLoadScreen::AppendHintToDialogBuffer()
{
    CExoString sEmpty("");
    g_pAppManager->m_pClientExoApp->GetInGameGui()->AppendToDialogBuffer(m_lblHint.m_cText.m_cParams.m_sText, sEmpty);
}

// FUNCTION: SWKOTOR 0x0067AB00
void CSWGuiLoadScreen::Draw(float fDelta)
{
    AurGUIStartLayer();
    if (AurGUISetupViewport(0, 0, m_pManager->m_nWidth, m_pManager->m_nHeight, &g_vBlack, 0, 1.0f)) {
        m_lblImage.Draw(fDelta);
        AurGUICloseViewport();
    }
    AurGUIStopLayer();
    CSWGuiPanel::Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x0067AB60 SYMBOL: ??_GCSWGuiLoadScreen@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0067AB80 SYMBOL: ??1CSWGuiLoadScreen@@UAE@XZ
