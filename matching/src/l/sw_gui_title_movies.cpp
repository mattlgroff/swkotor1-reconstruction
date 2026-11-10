// CSWGuiTitleMovies (swguititlemovies.o): the title screen movie list
// (layout "titlemovie", rows from the MOVIES: and live%d:movies folders,
// sorted by the movies.2da Order column).
#define L_GUI_BUTTON_INLINE_CTOR
#include "l/sw_gui_title_movies.h"

static inline CClientExoApp* GetClientExoApp()
{
    return g_pAppManager->m_pClientExoApp;
}

// FUNCTION: SWKOTOR 0x006DCE80
void CSWGuiTitleMovies::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x28:
        case 0x2E:
            g_pAppManager->m_pClientExoApp->SetStartMenuMusicOnMovieEnd(1);
            g_pAppManager->m_pClientExoApp->StartMenuMusic(1, 0x7F, 2);
            m_pManager->PlayGuiSound(0);
            m_pManager->PopModalPanel();
            MarkForDelete();
            break;
        case 0x2D:
            m_pManager->PlayGuiSound(0);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006DCF10
void CSWGuiTitleMovies::OnMoviePicked(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        CExoString* pMovie = (CExoString*)pControl->GetButton()->m_pUserData;
        if (pMovie) {
            int nResult = 0;
            g_pAppManager->m_pClientExoApp->PlayMovie(*pMovie, &nResult, 0, 1);
        } else {
            CExoString sCredits("credits");
            GetClientExoApp()->StartCreditSequence(Vector(0.0f, 0.0f, 0.0f), sCredits);
        }
    }
}

// FUNCTION: SWKOTOR 0x006DCFE0
CSWGuiTitleMovies::~CSWGuiTitleMovies()
{
    for (int i = 0; i < m_lbMovies.m_nControls; i++) {
        CExoString* pMovie = (CExoString*)m_lbMovies.GetControl(i)->GetButton()->m_pUserData;
        if (pMovie) {
            delete pMovie;
        }
    }
}

// FUNCTION: SWKOTOR 0x006DDAF0 SYMBOL: ??_GCSWGuiTitleMovies@@UAEPAXI@Z
// Emitted with the vtable in this object.

// FUNCTION: SWKOTOR 0x006DD100
void CSWGuiTitleMovies::PopulateMovies()
{
    CExoString sName;
    CExoArrayList<CExoString> lstMovies;
    CExoArrayList<CSWGuiControl*> lstControls;
    int nOrder = 0;
    CSWGuiExtent cExtent = m_lbMovies.m_cItemExtent;
    g_pExoBase->GetDirectoryList(&lstMovies, "MOVIES:", 0x80F, 0, 0);
    CExoString sAlias;
    for (int nLive = 1; nLive < g_nLiveFolderCount; nLive++) {
        sAlias.Format("live%d", nLive);
        if (g_pExoBase->m_pAliasList->GetAliasPath(sAlias, 0)->GetLength() > 0) {
            sAlias = sAlias + ":movies";
            g_pExoBase->GetDirectoryList(&lstMovies, sAlias, 0x80F, 0, 0);
        }
    }
    int nMovies = lstMovies.num;
    CSWGuiButton* pProto = m_lbMovies.m_pProtoItem;
    CExoString sText;
    CSWGuiButton* pCredits = new CSWGuiButton;
    pCredits->CopyStyle(cExtent, pProto);
    pCredits->m_cText.m_cParams.SetText(g_pTlkTable->GetSimpleString(0xBB2E));
    pCredits->m_pUserData = 0;
    cExtent.height = 0x38;
    pCredits->SetExtent(cExtent);
    pCredits->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiTitleMovies::OnMoviePicked);
    pCredits->AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiTitleMovies::OnMoviePicked);
    lstControls.AddInline(pCredits);
    m_lstMovieNames.Add(CExoString("credits"));
    for (int i = 0; i < nMovies; i++) {
        sName = lstMovies.element[i];
        sName = sName.SubString(0, sName.GetLength() - 4);
        int nAlwaysShow;
        g_pRules->Get2DArrays()->GetMovies()->GetINTEntry(sName, "AlwaysShow", &nAlwaysShow);
        if (nAlwaysShow != 1) {
            CResRef cMovie(sName);
            if (!GetClientExoApp()->GetClientOptions()->GetMovieShown(cMovie)) {
                continue;
            }
        }
        CSWGuiButton* pButton = new CSWGuiButton;
        pButton->CopyStyle(cExtent, pProto);
        int nStrRef = 0;
        g_pRules->Get2DArrays()->GetMovies()->GetINTEntry(sName, "StrrefName", &nStrRef);
        if (nStrRef) {
            sText = g_pTlkTable->GetSimpleString(nStrRef);
        } else {
            sText = sName;
        }
        pButton->m_cText.m_cParams.SetText(sText);
        pButton->m_pUserData = new CExoString(sName);
        cExtent.height = 0x38;
        pButton->SetExtent(cExtent);
        pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiTitleMovies::OnMoviePicked);
        pButton->AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiTitleMovies::OnMoviePicked);
        g_pRules->Get2DArrays()->GetMovies()->GetINTEntry(sName, "Order", &nOrder);
        int nIndex = 1;
        if (lstControls.num > 1) {
            do {
                int nOther = 0;
                g_pRules->Get2DArrays()->GetMovies()->GetINTEntry(m_lstMovieNames.element[nIndex], "Order", &nOther);
                if (nOther > nOrder) {
                    break;
                }
                nIndex++;
            } while (nIndex < lstControls.num);
        }
        lstControls.InsertInline(pButton, nIndex);
        m_lstMovieNames.Insert(sName, nIndex);
    }
    m_lbMovies.AddControls(lstControls, 1, 0, 0);
}

// FUNCTION: SWKOTOR 0x006DD910
CSWGuiTitleMovies::CSWGuiTitleMovies(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    StartLoadFromLayout("titlemovie");
    InitControl(&m_lbMovies, "LB_MOVIES", 1);
    InitControl(&m_lblTitle, "LBL_TITLE", 1);
    InitControl(&m_btnBack, "BTN_BACK", 1);
    UnloadResource();
    CSWGuiButton* pProto = m_lbMovies.m_pProtoItem->GetButton();
    if (pProto) {
        pProto->m_cBorder.m_cParams.m_vColor = g_vPazaakButtonHilight;
        pProto->m_cHilight.m_cParams.m_vColor = g_vAbilityColorGranted;
        pProto->m_cHilight.m_cParams.SetFlags(1, 1, 0);
    }
    CSWGuiButton* pBack = &m_btnBack;
    pBack->m_bNavigable = 0;
    pBack->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::OnBButtonPressed);
    PopulateMovies();
    SetActiveControl(&m_lbMovies, 0);
}
