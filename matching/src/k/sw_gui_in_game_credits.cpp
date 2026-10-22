// CSWGuiInGameCredits: the scrolling / fading credits panel (credits.2da
// "Name" string refs, LB_CREDITS list box, streamed music).
#include "k/sw_gui_in_game_credits.h"

// FUNCTION: SWKOTOR 0x0068F0E0
CSWGuiInGameCredits::~CSWGuiInGameCredits()
{
    if (m_pMusic) {
        delete m_pMusic;
    }
    if (m_pStrRefs) {
        delete[] m_pStrRefs;
    }
    CGuiInGame* pGui = g_pAppManager->m_pClientExoApp->GetInGameGui();
    if (pGui) {
        pGui->FadeAndStopDialogAmbientTrack();
    }
    g_pAppManager->m_pClientExoApp->StopCreditSequence();
    g_pAppManager->GetClientExoApp()->SetInputClass(m_nPrevInputClass, 1);
    if (pGui) {
        pGui->AdjustHideCount(0);
    }
    if (g_pAppManager->GetClientExoApp()->GetInternal()->m_bEvilEndingHackInProgress) {
        g_nScreenWidth = g_nCreditsSavedScreenWidth;
        g_nScreenHeight = g_nCreditsSavedScreenHeight;
        g_pGuiManager->SetSize(g_nCreditsSavedGuiWidth, g_nCreditsSavedGuiHeight);
        g_pAppManager->GetClientExoApp()->GetInternal()->SetEvilEndingHackInProgress(0);
        ResumeTimers();
    }
    g_pAppManager->m_pClientExoApp->ShowMouse(0x80);
}

// FUNCTION: SWKOTOR 0x0068F260
void CSWGuiInGameCredits::OnPanelAdded()
{
    g_pAppManager->m_pClientExoApp->HideMouse(0x80);
    g_pAppManager->m_pClientExoApp->GetInGameGui()->AdjustHideCount(1);
    m_nPrevInputClass = g_pAppManager->m_pClientExoApp->GetInputClass();
    CSWGuiPanel::OnPanelAdded();
    g_pAppManager->m_pClientExoApp->SetInputClass(2, 1);
    m_pMusic = new CExoStreamingSoundSource;
    if (m_pMusic) {
        m_pMusic->SetPriorityGroup(4);
        CResRef cMusic(m_sMusic.CStr());
        m_pMusic->Play(cMusic, 8, 0, 1, 0);
    }
}

// FUNCTION: SWKOTOR 0x0068F350
void CSWGuiInGameCredits::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x27:
        case 0x28:
        case 0x2d:
        case 0x2e:
            Close();
            break;
        }
    }
}

// FUNCTION: SWKOTOR 0x0068F3A0
void CSWGuiInGameCredits::Show(unsigned long nStrRef, int bLast)
{
    CSWGuiExtent ext;
    ext.left = ext.top = ext.width = ext.height = 0;
    if (m_bFading) {
        m_lblText.m_cText.SetStrRef(nStrRef);
    } else {
        STR_RES strRes;
        CExoString sText;
        CExoString sPad;
        g_pTlkTable->Fetch(nStrRef, strRes, g_nGender);
        float fHeight = (float)m_lblText.m_extent.height;
        int nLines = (int)(fHeight / ((float)m_lblText.m_cTextBox.GetLineHeight() * m_extent.height * (1.0f / 480.0f))) + 1;
        sPad = "";
        for (int i = 0; i < nLines; i++) {
            CExoString sNewLine("\n");
            sPad = sPad + sNewLine;
        }
        sText = sPad + strRes.sString + sPad;
        m_lblText.m_cText.SetText(sText);
    }
    m_lbCredits.ClearControls();
    if (bLast) {
        ext = m_lblText.m_extent;
        ext.height = m_lblText.m_cTextBox.GetHeight();
        m_lblText.SetExtent(ext);
    }
    CSWGuiControl* pControl = &m_lblText;
    m_lbCredits.AddControls(&pControl, 1, 0, 0, 0);
    m_lbCredits.SetActiveControl(&m_lblText, 0);
    ext = m_lbCredits.m_extent;
    m_lbCredits.m_fScale = (float)ext.height;
}

// FUNCTION: SWKOTOR 0x0068F630
void CSWGuiInGameCredits::UpdateScroll(float fDelta)
{
    if (m_lbCredits.m_nScrollPos == m_lbCredits.m_nScrollMax) {
        if (!m_pMusic->IsPlaying()) {
            Close();
        }
        return;
    }
    m_fScrollTime += fDelta;
    while (m_fScrollTime >= m_fScrollStep) {
        m_lbCredits.HandleInputEvent(0x32, 1);
        m_fScrollTime -= m_fScrollStep;
        if (m_lbCredits.m_nScrollPos == m_lbCredits.m_nScrollMax) {
            break;
        }
    }
}

// FUNCTION: SWKOTOR 0x0068F6F0
void CSWGuiInGameCredits::SetNextText()
{
    int bLast = 0;
    if (m_nCurLine >= m_nLines - 1) {
        m_bFading = 0;
        m_lblText.m_cText.SetAlignment(10);
        m_lblText.m_cText.m_fAlpha = 1.0f;
        bLast = 1;
    }
    Show(m_pStrRefs[m_nCurLine], bLast);
    if (!m_bFading) {
        CSWGuiExtent ext = m_lblText.m_extent;
        m_fScrollStep = g_fCreditsScrollRate / ext.height;
    }
    m_nCurLine++;
}

// FUNCTION: SWKOTOR 0x0068F790
void CSWGuiInGameCredits::UpdateFading(float fDelta)
{
    float fTime = fDelta + m_fFadeTime;
    m_fFadeTime = fTime;
    if (fTime >= 1.0f) {
        if (m_bFadeIn) {
            m_lblText.m_cText.m_fAlpha = 1.0f;
            if (fTime >= 3.0f) {
                int n = (int)(0.33333334f * fTime);
                m_fFadeTime = fTime - n * 3.0f;
                m_bFadeIn = 0;
            }
        } else {
            m_lblText.m_cText.m_fAlpha = 0.0f;
            if (fTime >= 2.0f) {
                int n = (int)(0.5f * fTime);
                m_fFadeTime = fTime - n * 2.0f;
                m_bFadeIn = 1;
                SetNextText();
            }
        }
    } else {
        if (m_bFadeIn) {
            m_lblText.m_cText.m_fAlpha = fTime;
        } else {
            m_lblText.m_cText.m_fAlpha = 1.0f - fTime;
        }
    }
}

// FUNCTION: SWKOTOR 0x0068F880
void CSWGuiInGameCredits::Draw(float fDelta)
{
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    if (!pServer || !pServer->GetPauseState(2)) {
        if (m_bFading) {
            UpdateFading(fDelta);
        } else {
            UpdateScroll(fDelta);
        }
    }
    CSWGuiPanel::Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x0068F8D0
// NONMATCH: 2 insns, the original loads m_nHeight before storing the saved
// width (temps / pManager local / declaration order tried).
CSWGuiInGameCredits::CSWGuiInGameCredits(CSWGuiManager* pManager, const CExoString& sMusic)
    : CSWGuiPanel(pManager)
{
    m_nFlags |= 1;
    {
        CResRef cLayout("credits");
        StartLoadFromLayout(cLayout);
    }
    {
        CExoString sName("LB_CREDITS");
        InitControl(&m_lbCredits, sName, 1);
    }
    UnloadResource();
    if (g_pAppManager->GetClientExoApp()->GetInternal()->m_bEvilEndingHackInProgress) {
        CSWGuiManager* pManager = g_pGuiManager;
        g_nCreditsSavedGuiWidth = pManager->m_nWidth;
        g_nCreditsSavedGuiHeight = pManager->m_nHeight;
        g_nCreditsSavedScreenWidth = g_nScreenWidth;
        g_nCreditsSavedScreenHeight = g_nScreenHeight;
        g_nScreenWidth = 640;
        g_nScreenHeight = 480;
        g_pGuiManager->SetSize(640, 480);
    }
    CSWGuiExtent ext;
    ext.top = ext.left = ext.width = ext.height = 0;
    ext.width = g_pGuiManager->m_nWidth;
    ext.height = g_pGuiManager->m_nHeight;
    SetExtent(ext);
    ext.left = 0x28;
    ext.top = 0x28;
    ext.width = m_extent.width - 0x50;
    ext.height = m_extent.height - 0x50;
    m_lbCredits.SetExtent(ext);
    ext.left += 4;
    ext.top += 4;
    ext.width -= 8;
    ext.height -= 8;
    CSWGuiControl* pProto = m_lbCredits.m_pProtoItem;
    m_lblText.CopyStyle(ext, pProto, m_extent.height * (1.0f / 480.0f));
    ext.left = 0;
    ext.top = 0;
    ext.width = g_pGuiManager->m_nWidth;
    ext.height = g_pGuiManager->m_nHeight;
    m_lblBackground.CopyStyle(ext, pProto, 1.0f);
    AddControl(&m_lblBackground);
    m_lblBackground.AddEvent(0x27, this, &CSWGuiPanel::OnCloseClicked);
    {
        CResRef cFont("fnt_credits");
        m_lblText.m_cText.SetBaseFont(cFont);
    }
    m_nLines = 0;
    m_pStrRefs = 0;
    int nStrRef;
    C2DA* p2DA = new C2DA("credits", 0);
    if (p2DA->Load2DArray()) {
        m_nLines = p2DA->GetNumRows();
        m_pStrRefs = new unsigned long[m_nLines];
        for (unsigned char i = 0; i < m_nLines; i++) {
            int bFound;
            {
                CExoString sColumn("Name");
                bFound = p2DA->GetINTEntry(i, sColumn, &nStrRef);
            }
            if (bFound) {
                m_pStrRefs[i] = nStrRef;
            } else {
                m_pStrRefs[i] = 0;
            }
        }
        m_fScrollStep = 0.0f;
        m_sMusic = sMusic;
        m_pMusic = 0;
        m_fScrollTime = 0.0f;
        m_fFadeTime = 0.0f;
        m_nPrevInputClass = 2;
        m_nCurLine = 0;
        m_bFading = 1;
        m_bFadeIn = 1;
        m_fScrollStep = 0.0f;
        SetNextText();
        m_lblText.m_cText.SetAlignment(0x12);
        m_lblText.m_cText.m_fAlpha = 0.0f;
    }
}
