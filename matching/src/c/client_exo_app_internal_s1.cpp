// CClientExoAppInternal members, worker C s1 (intro/credits/load-screen
// helpers, quick save, keyboard modal reset, stream fade).

#include <string.h>
#include <math.h>
#include "c/client_exo_app.h"

struct Vector {
    float x, y, z;
    Vector() {}
    Vector(float fx, float fy, float fz) { x = fx; y = fy; z = fz; }
};

class CExoInput {
public:
    void UpdateJoysticks();                         // 0x005ED310 (shared empty body)
    void SetActive(int b);                          // 0x005DF540
    void PauseRumble();                             // 0x005DF570
    int GetActive();                                // 0x005DF510
    void ClearEvents();                             // 0x005DF4E0
    int SetRumble(int nMotor, float fLeft, float fRight, unsigned long nDuration);   // 0x005DF550
};

class CSWSCreature {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90();
    virtual int GetIsDead();                        // 0x94 (descriptive)

    int GetGold();                                  // 0x004EDD60
    int GetDeadTemp();                              // 0x004EF890

    unsigned long m_idSelf;                         // 0x04
};

class CSWSClient {
public:
    char pad_0x0[0x38];
    unsigned long m_oidCreature;                    // 0x38
};

class CServerExoAppS1Calls {
public:
    int GetModulePreviouslyVisited(const CExoString& sModule);  // 0x004AEBE0
    unsigned long GetPlayerCreatureId();            // 0x004AEA40
    CSWSClient* GetClientObjectByObjectId(unsigned long oid);   // 0x004AE910
};

class CSWGuiPazaakStart : public CSWGuiPanel {
public:
    CSWGuiPazaakStart(CSWGuiManager* pManager, int nWager, int nDeck, int a, unsigned long b);  // 0x00681A90
    char pad_0x4[0x8928 - 0x4];
};

class CSWGuiMessageBox : public CSWGuiPanel {
public:
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68();
    virtual void SetText(int nStrRef);              // 0x6c (descriptive)

    void SetAllowCancel(int b);                     // 0x00627130
    void SetCloseCallback(CSWGuiPanel* pObject, void (CSWGuiPanel::*pfnCallback)(CSWGuiControl*), int n);   // 0x00624A40
};

class CGuiInGame {
public:
    void StartDialogAmbientTrack(const CResRef& cTrack);    // 0x0062A8B0
    void AdjustHideCount(int n);                    // 0x0062B410
    void InitializeGlobal();                        // 0x00632720
    int IsGlobalFadeObscuring();                    // 0x0062DED0
    int IsGlobalFading();                           // 0x0062AC60
    void EndGlobalFade();                           // 0x0062AC40 (descriptive)
    void StartGlobalFade(int nType, float fTime, float fAlpha, const Vector& vColor);   // 0x0062ABF0

    char pad_0x0[0x98];
    CSWGuiMessageBox* m_pMessageBox;                // 0x98
    char pad_0x9c[0x18];
    int m_nCombatPauseMode;                         // 0xb4
    char pad_0xb8[0x8];
    void* m_pModalPanel;                            // 0xc0
};



class CSWGuiProgressBar {
public:
    void SetCurValue(int nValue);                   // 0x00417FB0
};

class CResRef {
public:
    CResRef(const CExoString& s);                   // 0x00406D60
    CResRef(const char* s);                         // 0x00406D80
    int operator!=(const CExoString& s) const;      // 0x00406DE0
    char m_resRef[16];
};

class CExoStreamingSoundSource {
public:
    CExoStreamingSoundSource();                     // 0x005D5A50
    void FadeAndStop(unsigned long nMs);            // 0x005D5B90
    CResRef GetFilePlaying();                       // 0x005D6230
    void SetLooping(int b);                         // 0x005D5B40
    void SetVolume(unsigned char nVolume);          // 0x005D5B50
    void SetPriorityGroup(unsigned char nGroup);    // 0x005D5BA0
    int Play(const CResRef& cRes, unsigned short nType, int a, int b, int c);   // 0x005D5B10
    int IsPlaying();                                // 0x005D5AF0
    void Resume();                                  // 0x005D5B30
    void Stop();                                    // 0x005D5B80
    ~CExoStreamingSoundSource();                    // 0x005D6200
    void* m_pInternal;                              // 0x00
};

class CSWGuiTextParams {
public:
    void SetStrRef(unsigned long nStrRef);          // 0x00415E50
};

class CSWGuiLoadScreenS1 : public CSWGuiLoadScreen {
public:
    CSWGuiLoadScreenS1(CSWGuiManager* pManager);    // 0x0067A710
    void SetImage(const CResRef& cImage);           // 0x0067A6D0

    char pad_0x4[0x60];
    CSWGuiProgressBar m_cProgressBar;               // 0x64
    char pad_0x65[0x51c - 0x65];
    CSWGuiTextParams m_cLoadingText;                // 0x51c
    char pad_0x51d[0x574 - 0x51d];
    int m_bShowHints;                               // 0x574
    char pad_0x578[0x6b8 - 0x578];

    CSWGuiProgressBar* GetProgressBar() { return &m_cProgressBar; }
};

class CSWGuiInGameCredits : public CSWGuiPanel {
public:
    CSWGuiInGameCredits(CSWGuiManager* pManager, const CExoString& sMovie);   // 0x0068F8D0
    char pad_0x4[0x4c];
    Vector m_vPosition;                             // 0x50
    char pad_0x5c[0x5f8 - 0x5c];
};

class CSWCAmbientSound {
public:
    void StopAll(int b);                            // 0x0068E1B0
};

class CServerExoAppS1 {
public:
    void* m_pUnknown00;
    void* m_pcInternal;                             // 0x04
};

class C2DA {
public:
    int GetCExoStringEntry(const CExoString& sRow, const CExoString& sColumn, CExoString* pValue);   // 0x00413DE0
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);       // 0x00413660
    int GetFLOATEntry(int nRow, const CExoString& sColumn, float* pValue);   // 0x00413430

    C2DA(CResRef cResRef, int bUnknown);            // 0x00413CC0
    virtual ~C2DA();                                // vtable slot 0
    int Load2DArray();                              // 0x004143B0
    void Unload2DArray();                           // 0x004139E0

    char pad_0x4[0x20];
    int m_nNumRows;                                 // 0x24
    char pad_0x28[0x54 - 0x28];
};

class CTwoDimArrays {
public:
    char pad_0x0[0x14];
    C2DA* m_pSurfaceMaterials;                      // 0x14 (descriptive)
    C2DA* m_pModuleLoad;                            // 0x18 (descriptive: per-module load music/screens)
    char pad_0x1c[0x110 - 0x1c];
    C2DA* m_pVideoEffects;                          // 0x110

    C2DA* GetModuleLoad() { return m_pModuleLoad; }
    C2DA* GetSurfaceMaterials() { return m_pSurfaceMaterials; }
};

class CSWRules {
public:
    CSWRules();                                     // 0x00552C50
    char pad_0x0[0xb8];
    CTwoDimArrays* m_p2DArrays;                     // 0xb8
    char pad_0xbc[0xdc - 0xbc];

    CTwoDimArrays* Get2DArrays() { return m_p2DArrays; }
};

class CExoResMan {
public:
    int Exists(const CResRef& cRes, unsigned short nType, unsigned long* pSize);   // 0x00408BC0
    CExoString CacheResource(const CExoString& sName, unsigned short nType);       // 0x004085F0
};

extern CSWRules* g_pRules;                          // 0x007A3A28
extern CExoResMan* g_pExoResMan;                    // 0x007A39E8
extern CExoInput* g_pExoInput;                      // 0x007A39E4
extern const int SOUND_MODE_MOVIE;                  // 0x0074C5F8
extern const int MUSIC_STATE_COMBAT_PAUSED;         // 0x0074C5F4
extern int g_bCameraResetPending;                   // 0x007A2288
extern int g_bAppActive;                            // 0x007A3A38
extern const int MUSIC_STATE_NORMAL;                // 0x0074C5EC
void PumpMessages();                                // 0x00401510
void PauseTimers();                                 // 0x00403CF0
void ResumeTimers();                                // 0x00403DE0
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long nMilliseconds);   // IAT 0x0073D1E4

// Movie names converted from the queued move-to-module movie resrefs.
class CExoStringMovieList {
public:
    CExoStringMovieList(CExoArrayList<CResRef>* pMovies);   // 0x004B71E0
    ~CExoStringMovieList();                         // 0x00405920 (~CExoArrayList<CExoString>)

    CExoString* element;                            // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08
};

class CExoMoviePlayer {
public:
    int GetState();                                 // 0x0070C1F0
    void ShowFadePanel();                           // 0x0070C210
    void PlayMoviesAsync(const CExoStringMovieList& lstMovies, int a, int b);  // 0x0070C200
    void PlayMoviesAsync(CExoArrayList<CExoString>& lstMovies, unsigned long nFlags, CExoArrayList<int>* pMovieFlags);  // 0x0070C200
};
extern const int g_nSoundModeMenu;                  // 0x0074C5FC

// FUNCTION: SWKOTOR 0x00602C90
void CClientExoAppInternal::UpdateConnectionStall()
{
    CAppPad* pPad = g_pAppManager->m_pPad;
    if (pPad->m_n10 == 1) {
        pPad->m_n10 = 0;
        return;
    }
    if (pPad->m_n14 == 1) {
        pPad->m_n14 = 0;
        pPad = g_pAppManager->m_pPad;
        pPad->m_n00 = 0;
        pPad->m_n04 = 0;
        if (m_bConnectionStalled) {
            ShutDownToMainMenu();
        }
        m_bConnectionStalled = 0;
    }
}

// FUNCTION: SWKOTOR 0x005F4B50
void CClientExoAppInternal::DoQuickSave()
{
    CExoString sName("QUICKSAVE");
    CExoString sDescription("QUICKSAVE");
    CClientExoApp* pApp = g_pAppManager->m_pClientExoApp;
    pApp->SendSaveGameRequest(0, sDescription, sName);
}

// FUNCTION: SWKOTOR 0x00602DC0
void CClientExoAppInternal::BeginIntro()
{
    if (m_bIntroStarted) {
        return;
    }
    {
        CExoString sMovie("leclogo");
        AddMovieToMovieQueue(sMovie, 1);
    }
    {
        CExoString sMovie("biologo");
        AddMovieToMovieQueue(sMovie, 1);
    }
    {
        CExoString sMovie("legal");
        AddMovieToMovieQueue(sMovie, 1);
    }
    PlayMovieQueue(1);
}

// FUNCTION: SWKOTOR 0x005F7B20
void CClientExoAppInternal::UpdateLoadingScreenHint()
{
    // NONMATCH (9 insns): the original keeps the elapsed sum in eax.
    CServerExoAppS1* pServer = (CServerExoAppS1*)g_pAppManager->m_pServerExoApp;
    if (!pServer || !pServer->m_pcInternal) {
        return;
    }
    CSWGuiLoadScreenS1* pLoadScreen = (CSWGuiLoadScreenS1*)m_pLoadScreen;
    if (!pLoadScreen || !pLoadScreen->m_bShowHints) {
        return;
    }
    unsigned long nNow = g_pExoBase->m_pcExoTimers->GetLowResolutionTimer();
    unsigned long nDelta = nNow - m_nLoadScreenHintLastTime;
    m_nLoadScreenHintLastTime = nNow;
    m_nLoadScreenHintUpdateTimer += nDelta;
    if (m_nLoadScreenHintUpdateTimer >= 10000) {
        m_nLoadScreenHintUpdateTimer = 0;
        CExoString sModule("");
        int nHint = GetNextLoadScreenHintSTRREF(sModule, 2);
        if (m_pLoadScreen) {
            m_pLoadScreen->SetHint(nHint);
        }
    }
}

// FUNCTION: SWKOTOR 0x005FB3F0
void CClientExoAppInternal::SetLoadBarProgress(int nValue, int bUpdateHint)
{
    g_pExoInput->UpdateJoysticks();
    if (m_pLoadScreen) {
        if (bUpdateHint && nValue < 80) {
            UpdateLoadingScreenHint();
        }
        ((CSWGuiLoadScreenS1*)GetLoadScreen())->m_cProgressBar.SetCurValue(nValue);
        CExoString sValue;
        sValue.Format("Load Bar = %d", nValue);
    }
}

// FUNCTION: SWKOTOR 0x005F4C60
void CClientExoAppInternal::StartCreditSequence(const Vector& vPosition, const CExoString& sMovie)
{
    if (m_bCreditSequenceInProgress) {
        return;
    }
    CSWGuiInGameCredits* pCredits = new CSWGuiInGameCredits(g_pGuiManager, sMovie);
    if (pCredits) {
        if (!g_pGuiManager->PanelExists(pCredits)) {
            if (m_bEvilEndingHackInProgress) {
                g_pGuiManager->AddPanel(pCredits, 1, 1);
            } else {
                g_pGuiManager->AddPanel(pCredits, 3, 1);
            }
        }
        pCredits->m_vPosition = vPosition;
        if (m_pModule && m_pModule->m_pPlayer && m_pModule->m_pPlayer->m_pAmbientSound) {
            m_pModule->m_pPlayer->m_pAmbientSound->StopAll(0);
        }
        m_bCreditSequenceInProgress = 1;
    }
}

// FUNCTION: SWKOTOR 0x005F2980
void CClientExoAppInternal::TurnOffKeyboardModal()
{
    for (unsigned int i = 0; i < 7; i++) {
        if (m_anKeyboardModal[i] != 0) {
            memset(m_anKeyboardModal, 0, sizeof(m_anKeyboardModal));
            m_nUnknownFC = 1;
            m_nUnknownF4 = 0;
            m_nUnknownF8 = 0;
            m_pSaveLoad->SendPlayerToServer_06_21();
            break;
        }
    }
    if (m_pModule && m_pModule->m_pPlayer) {
        m_pModule->m_pPlayer->Unknown605C40(0);
    }
}

// FUNCTION: SWKOTOR 0x005F1B20
void CClientExoAppInternal::FadeAndStopStreamingSoundSource(CExoStreamingSoundSource* pSource, unsigned long nMs)
{
    unsigned long nStart = (unsigned long)(g_pExoBase->m_pcExoTimers->GetHighResolutionTimer() / 1000);
    pSource->FadeAndStop(nMs);
    unsigned long nElapsed = 0;
    while (nElapsed < nMs) {
        g_pExoSound->Update();
        nElapsed = (unsigned long)(g_pExoBase->m_pcExoTimers->GetHighResolutionTimer() / 1000) - nStart;
    }
}

// FUNCTION: SWKOTOR 0x005F3810
void CClientExoAppInternal::StartPazaakGame(int nDeck, const CExoString& sScript, int nWager, int a, unsigned long b)
{
    if (m_bPazaakIsRunning) {
        return;
    }
    m_bPazaakIsRunning = 1;
    CSWSCreature* pPC = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(
        g_pAppManager->m_pServerExoApp->ClientToServerObjectID(
            g_pAppManager->m_pClientExoApp->GetParty()->GetPC()->GetId()));
    if (pPC && nWager > 0) {
        int nGold = pPC->GetGold();
        if (nWager >= nGold) {
            nWager = nGold;
        }
    }
    m_bPazaakWon = 0;
    m_nPazaakWager = nWager;
    m_sPazaakScript = sScript;
    m_nPazaakDeck = nDeck;
    g_pExoSound->SetMusicState(g_nSoundModeMenu);
    CSWGuiPazaakStart* pPanel = new CSWGuiPazaakStart(m_pGuiManager, nWager, nDeck, a, b);
    SetInputClass(2, 1);
    m_pGuiManager->AddPanel(pPanel, 3, 1);
    m_pGuiManager->PlayGuiSound(4);
}

// FUNCTION: SWKOTOR 0x005F6C60
void CClientExoAppInternal::ShowLoadScreen(int bFade, int bStartMusic, int bMovie)
{
    if (g_pExoSound) {
        if (bMovie) {
            g_pExoSound->SetMusicState(SOUND_MODE_MOVIE);
        } else if (m_sLoadMusic != "") {
            g_pExoSound->SetMusicState(g_nSoundModeMenu);
        }
    }
    if (!m_bLoadScreenEnabled || !m_sLoadScreenImage.GetLength()) {
        return;
    }
    if (!m_pLoadScreen) {
        m_pLoadScreen = new CSWGuiLoadScreenS1(m_pGuiManager);
    }
    CResRef cImage(m_sLoadScreenImage);
    ((CSWGuiLoadScreenS1*)m_pLoadScreen)->SetImage(cImage);
    ((CSWGuiLoadScreenS1*)m_pLoadScreen)->m_cLoadingText.SetStrRef(bMovie ? 0xa620 : 0xa5fd);
    if (m_pGuiManager && !m_pGuiManager->PanelExists(m_pLoadScreen)) {
        m_nLoadScreenHintLastTime = g_pExoBase->m_pcExoTimers->GetLowResolutionTimer();
        if (bFade && m_pGuiInGame) {
            m_pGuiInGame->StartGlobalFade(0, 0.0f, 0.0f, Vector(0.0f, 0.0f, 0.0f));
        }
        m_pGuiManager->AddPanel(m_pLoadScreen, 1, 1);
    }
    if (g_pExoInput) {
        g_pExoInput->SetActive(0);
    }
    TurnOffKeyboardModal();
    SetInputState(2);
    if (bStartMusic) {
        StartLoadMusic();
    }
}

// FUNCTION: SWKOTOR 0x005F9AF0
void CClientExoAppInternal::SetMusic(int bTheme, int nVolume, int nPriorityGroup)
{
    CExoString sMusic(bTheme ? "mus_theme_cult" : "mus_theme_rep");
    CExoStreamingSoundSource* pMusic = (CExoStreamingSoundSource*)m_pMenuMusic;
    int bChanged = (pMusic && pMusic->GetFilePlaying() != sMusic) ? 1 : 0;
    if (!m_bStartMenuMusicOnMovieEnd && !bChanged) {
        return;
    }
    m_bStartMenuMusicOnMovieEnd = 0;
    if (bChanged) {
        StopMenuMusic();
    }
    if (!m_pMenuMusic) {
        m_pMenuMusic = (CExoSoundSource*)new CExoStreamingSoundSource();
        ((CExoStreamingSoundSource*)m_pMenuMusic)->SetLooping(1);
        ((CExoStreamingSoundSource*)m_pMenuMusic)->SetVolume(nVolume);
        ((CExoStreamingSoundSource*)m_pMenuMusic)->SetPriorityGroup(nPriorityGroup);
        CResRef cMusic(sMusic);
        ((CExoStreamingSoundSource*)m_pMenuMusic)->Play(cMusic, 8, 0, 1, 0);
    } else if (!((CExoStreamingSoundSource*)m_pMenuMusic)->IsPlaying()) {
        CResRef cMusic(sMusic);
        ((CExoStreamingSoundSource*)m_pMenuMusic)->Play(cMusic, 8, 0, 1, 1);
    }
}

// FUNCTION: SWKOTOR 0x005F3650
CExoString CClientExoAppInternal::GetLoadMusicByModuleName(const CExoString& sModule)
{
    CExoString sMusic;
    if (!sModule.GetLength()) {
        sMusic = "";
        return sMusic;
    }
    if (!g_pRules->Get2DArrays()->GetModuleLoad()->GetCExoStringEntry(sModule, "MusicResRef", &sMusic)) {
        sMusic.Format("load_%s", sModule);
        if (!g_pExoResMan->Exists(sMusic, 8, 0)) {
            g_pRules->Get2DArrays()->GetModuleLoad()->GetCExoStringEntry("DEFAULT", "MusicResRef", &sMusic);
        }
    }
    return sMusic;
}

// FUNCTION: SWKOTOR 0x005F3480
void CClientExoAppInternal::SetLoadScreenByModuleName(const CExoString& sModule)
{
    CExoString sImage;
    if (!sModule.GetLength()) {
        m_sLoadScreenImage = "";
        m_bLoadScreenEnabled = 1;
        return;
    }
    if (!g_pRules->Get2DArrays()->GetModuleLoad()->GetCExoStringEntry(sModule, "BMPResRef", &sImage)) {
        sImage.Format("load_%s", sModule);
        if (!g_pExoResMan->Exists(sImage, 3, 0) && !g_pExoResMan->Exists(sImage, 0xbbf, 0)) {
            g_pRules->Get2DArrays()->GetModuleLoad()->GetCExoStringEntry("DEFAULT", "BMPResRef", &sImage);
        }
    }
    if (!m_sLoadScreenImage.CompareNoCase(sImage)) {
        m_sLoadScreenImage = sImage;
        m_bLoadScreenEnabled = 1;
    }
}

// FUNCTION: SWKOTOR 0x005F6960
void CClientExoAppInternal::QueryExitProgram()
{
    if (!g_pAppManager->m_pClientExoApp || !g_pAppManager->m_pClientExoApp->GetGuiInGame()) {
        return;
    }
    if (g_pAppManager->m_pClientExoApp->GetGuiInGame()->m_pModalPanel) {
        return;
    }
    g_pAppManager->m_pClientExoApp->EmptyForwarder();
    if (m_pClientOptions->m_nCameraMode == 5 && GetPlayerCreature() && !GetPlayerCreature()->m_pUnknown138 &&
        !m_bGameOver && m_pModule) {
        g_bCameraResetPending = 1;
        m_pModule->ExitCombatMode();
        SetInputClass(0, 1);
    }
    if (!m_bAutoPaused) {
        // Inlined SetPausedByCombat(1, 4, 0).
        if (m_pGuiInGame->m_nCombatPauseMode == 0) {
            CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
            int nState = pServer ? pServer->GetPauseState(2) : 0;
            if (!m_bPausedByCombatSet && nState != 1) {
                m_bPausedByCombatSet = 1;
                m_nPausedByCombatFlag = 4;
                m_bPausedByCombat = 1;
                g_pExoInput->PauseRumble();
                g_pExoSound->SetMusicState(MUSIC_STATE_COMBAT_PAUSED);
                m_bSoundPausedByCombat = 1;
            }
        }
        SetAutoPaused(1, 0);
    }
    if (m_pModule && m_pModule->GetMiniGame()) {
        ClearInputState(1);
    }
    CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetGuiInGame()->m_pMessageBox;
    pBox->SetAllowCancel(1);
    pBox->SetText(0xa56c);
    g_pAppManager->m_pClientExoApp->GetGuiManager()->AddPanel(pBox, 1, 1);
    pBox->SetCloseCallback(pBox, reinterpret_cast<void (CSWGuiPanel::*)(CSWGuiControl*)>(&CClientExoAppInternal::OnQuitGameConfirmed), 0);
    g_pExoInput->SetActive(1);
    g_pAppManager->m_pClientExoApp->EnableInput();
    g_pAppManager->m_pClientExoApp->SetInputClass(2, 1);
}

// FUNCTION: SWKOTOR 0x005F6480
void CClientExoAppInternal::StartLoadMusic()
{
    if (m_pMenuMusic) {
        FadeAndStopStreamingSoundSource((CExoStreamingSoundSource*)m_pMenuMusic, 500);
        delete (CExoStreamingSoundSource*)m_pMenuMusic;
        m_pMenuMusic = 0;
    }
    if (!g_pExoSound || m_lstMoveToModuleMovies.num) {
        return;
    }
    if (!m_pLoadMusic) {
        if (m_sLoadMusic != "") {
            CExoString sPath;
            sPath.Format("HD0:STREAMMUSIC\\%s", m_sLoadMusic.CStr());
            g_pExoResMan->CacheResource(sPath, 4);
            m_pLoadMusic = (CExoSoundSource*)new CExoStreamingSoundSource();
            ((CExoStreamingSoundSource*)m_pLoadMusic)->SetLooping(1);
            ((CExoStreamingSoundSource*)m_pLoadMusic)->SetPriorityGroup(2);
            ((CExoStreamingSoundSource*)m_pLoadMusic)->Play(m_sLoadMusic, 8, 0, 1, 0);
        } else {
            g_pExoSound->SetMusicState(SOUND_MODE_MOVIE);
        }
    } else if (!((CExoStreamingSoundSource*)m_pLoadMusic)->IsPlaying()) {
        ((CExoStreamingSoundSource*)m_pLoadMusic)->Resume();
    }
}

void AurEnableSaturation();                         // 0x0044F0C0
void AurEnableScanNoise();                          // 0x0044F0E0
void AurSetSaturationParams(Vector vModulation, float fSaturation);   // 0x0044F100

// FUNCTION: SWKOTOR 0x005F7710
void CClientExoAppInternal::EnableVideoEffect(int nEffect)
{
    float fSaturation = 0.0f;
    Vector vModulation(0.0f, 0.0f, 0.0f);
    int bEnableSaturation = 0;
    int bEnableScanNoise = 0;
    C2DA* pVideoEffects = g_pRules->Get2DArrays()->m_pVideoEffects;
    m_nVideoEffect = -2;
    AurDisableSaturation();
    AurDisableScanNoise();
    if (!pVideoEffects || nEffect < 0) {
        return;
    }
    int bResult = pVideoEffects->GetINTEntry(nEffect, "EnableScanNoise", &bEnableScanNoise);
    if (bResult && bEnableScanNoise) {
        m_nVideoEffect = nEffect;
        AurEnableScanNoise();
    }
    bResult = pVideoEffects->GetINTEntry(nEffect, "EnableSaturation", &bEnableSaturation);
    if (bResult && bEnableSaturation) {
        m_nVideoEffect = nEffect;
        pVideoEffects->GetFLOATEntry(nEffect, "ModulationRed", &vModulation.x);
        pVideoEffects->GetFLOATEntry(nEffect, "ModulationGreen", &vModulation.y);
        pVideoEffects->GetFLOATEntry(nEffect, "ModulationBlue", &vModulation.z);
        pVideoEffects->GetFLOATEntry(nEffect, "Saturation", &fSaturation);
        AurEnableSaturation();
        AurSetSaturationParams(vModulation, fSaturation);
    }
}

// FUNCTION: SWKOTOR 0x005F6F20
int CClientExoAppInternal::WaitForMovie(int bRestoreMusic)
{
    // NONMATCH (35 insns): the original keeps bStopped in a stack slot and
    // never uses ebp; this build enregisters it in ebp (frame 4 bytes smaller).
    int bStopped = 0;
    int nCount = 0;
    int bInputActive = g_pExoInput->GetActive();
    int bAppActive = g_bAppActive;
    if (g_pExoInput) {
        g_pExoInput->SetActive(1);
    }
    ClearInputState(0);
    if (m_bEvilEndingHackInProgress) {
        if (m_pEvilEndingSound) {
            m_pEvilEndingSound->Stop();
            delete m_pEvilEndingSound;
        }
        g_pAppManager->m_pClientExoApp->GetGuiInGame()->StartDialogAmbientTrack("");
        m_pEvilEndingSound = new CExoStreamingSoundSource();
        m_pEvilEndingSound->SetVolume((unsigned char)(g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nMusicVolume * 1.27f));
        m_pEvilEndingSound->Play("evil_ending", 8, 0, 1, 0);
    }
    while (m_pMoviePlayer && m_pMoviePlayer->GetState() == 1) {
        if (nCount == 500 && !bStopped) {
            PumpMessages();
            if (m_bUnknown254 == 1) {
                if (m_pMoviePlayer) {
                    m_pMoviePlayer->Stop(1, 0);
                }
                bStopped = 1;
            }
            nCount = 0;
        } else {
            nCount++;
        }
    }
    if (bRestoreMusic) {
        g_pAppManager->m_pClientExoApp->RemoveMoveToModuleMovies();
        g_pExoSound->SetMusicState(MUSIC_STATE_NORMAL);
        if (m_bUnknown200) {
            m_pGuiInGame->AdjustHideCount(0);
            g_pExoSound->SetMusicState(MUSIC_STATE_NORMAL);
            g_pAppManager->m_pClientExoApp->GetWorldTimer()->UnpauseWorldTimer();
            g_pAppManager->m_pServerExoApp->GetWorldTimer()->UnpauseWorldTimer();
            m_bUnknown200 = 0;
        }
    }
    if (g_bAppActive == bAppActive) {
        if (g_pExoInput) {
            g_pExoInput->SetActive(bInputActive);
        }
        if (!bInputActive) {
            TurnOffKeyboardModal();
        } else {
            ClearInputState(0);
        }
    }
    ClearInputState(8);
    return 0;
}

// FUNCTION: SWKOTOR 0x005F51C0
int CClientExoAppInternal::Initialize()
{
    if (!m_pLoadScreen) {
        m_pLoadScreen = new CSWGuiLoadScreenS1(m_pGuiManager);
    }
    if (!g_pRules) {
        g_pRules = new CSWRules();
    }
    if (g_pRules) {
        for (unsigned char i = 0; i < g_pRules->Get2DArrays()->GetSurfaceMaterials()->m_nNumRows; i++) {
            int bWalk;
            g_pRules->Get2DArrays()->GetSurfaceMaterials()->GetINTEntry(i, "Walk", &bWalk);
            m_nHitCheckMaterialMask |= bWalk << i;
        }
    }
    m_pGuiInGame->InitializeGlobal();
    SetEventDescriptions();
    if (m_pMoviePlayer && m_pMoviePlayer->GetState()) {
        m_pGuiInGame->AdjustHideCount(1);
        g_pExoInput->ClearEvents();
        g_pExoSound->SetMusicState(SOUND_MODE_MOVIE);
    }
    m_bClientInitialized = 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x00602650
int CClientExoAppInternal::PlayLoadModuleMovies()
{
    if (m_pMoviePlayer && !m_bMovieQueueActive) {
        CExoStringMovieList lstMovies(g_pAppManager->m_pClientExoApp->GetMoveToModuleMovies());
        if (lstMovies.num > 0) {
            if (g_pAppManager->m_pServerExoApp) {
                g_pAppManager->m_pClientExoApp->GetWorldTimer()->PauseWorldTimer();
                g_pAppManager->m_pServerExoApp->GetWorldTimer()->PauseWorldTimer();
                m_pGuiInGame->AdjustHideCount(1);
                g_pExoInput->ClearEvents();
                g_pExoSound->SetMusicState(SOUND_MODE_MOVIE);
            }
            PauseTimers();
            ShowLoadScreen(1, 1, 0);
            ((CExoMoviePlayer*)m_pMoviePlayer)->ShowFadePanel();
            ((CExoMoviePlayer*)m_pMoviePlayer)->PlayMoviesAsync(lstMovies, 0, 0);
            Sleep(2);
            m_bUnknown200 = 1;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005F7960
int CClientExoAppInternal::ChangeCharacterToNextLivingPartyMember(int bCheckTarget, int bPlaySound, unsigned char nCount)
{
    // NONMATCH (56 insns): register assignment (original keeps nCount in bl,
    // this in ebp, and coalesces the uninitialised pPC with the nCount slot).
    if (!nCount) {
        return 1;
    }
    if (!g_pAppManager->m_pServerExoApp) {
        return 0;
    }
    CServerExoAppS1Calls* pServer = (CServerExoAppS1Calls*)g_pAppManager->m_pServerExoApp;
    CSWSClient* pClient = ((CServerExoAppS1Calls*)g_pAppManager->m_pServerExoApp)->GetClientObjectByObjectId(
        pServer->GetPlayerCreatureId());
    int bTargetAlive = 0;
    if (!pClient) {
        return 0;
    }
    unsigned long oidOldPC = m_pParty->GetPC()->GetId();
    CSWSCreature* pTarget;
    if (bCheckTarget) {
        pTarget = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(pClient->m_oidCreature);
        if (pTarget && !pTarget->GetIsDead() && !pTarget->GetDeadTemp()) {
            bTargetAlive = 1;
        }
    }
    unsigned char nFound = 0;
    int nTries = nCount * 3;
    CSWCCreature* pPC;
    while (nTries > 0) {
        ChangeCharacter(-2, 0);
        pPC = m_pParty->GetPC();
        if (pPC) {
            int bDead = pPC->AsServerCreature()->GetIsDead();
            int bDeadTemp = pPC->AsServerCreature()->GetDeadTemp();
            if (bDead || bDeadTemp) {
                nTries--;
            }
            if (!bDead && !bDeadTemp) {
                if (!bTargetAlive || pTarget->m_idSelf == pClient->m_oidCreature) {
                    nFound++;
                }
            }
        }
        if (nFound == nCount) {
            break;
        }
    }
    if (nTries > 0) {
        if (oidOldPC != m_pParty->GetPC()->GetId()) {
            if (bPlaySound) {
                PlayCharacterSelectSound(pPC);
            }
            return 1;
        }
    } else if (nTries != 0) {
        ChangeCharacter(g_pAppManager->m_pClientExoApp->GetParty()->IsPartyMember(pClient->m_oidCreature), 0);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005F4760
int CClientExoAppInternal::GetNextLoadScreenHintSTRREF(const CExoString& sModule, unsigned char nType)
{
    int nStrRef = 0;
    int nValue = 0;
    C2DA* pHints = new C2DA("loadscreenhints", 0);
    if (!pHints) {
        return 0;
    }
    if (!pHints->Load2DArray()) {
        delete pHints;
        return 0;
    }
    int bStory;
    switch (nType) {
    case 1:
        bStory = 0;
        break;
    case 2:
        bStory = 1;
        break;
    default:
        if (g_pAppManager->m_pServerExoApp && ((CServerExoAppS1*)g_pAppManager->m_pServerExoApp)->m_pcInternal) {
            bStory = ((CServerExoAppS1Calls*)g_pAppManager->m_pServerExoApp)->GetModulePreviouslyVisited(sModule);
        }
        break;
    }
    if (bStory) {
        if (pHints->GetINTEntry(m_bStoryHint, "StoryHint", &nValue)) {
            nStrRef = nValue;
            m_bStoryHint++;
        } else {
            m_bStoryHint = 0;
            if (pHints->GetINTEntry(m_bStoryHint, "StoryHint", &nValue)) {
                nStrRef = nValue;
                m_bStoryHint++;
            }
        }
    } else {
        if (pHints->GetINTEntry(m_bGamePlayHint, "GamePlayHint", &nValue)) {
            nStrRef = nValue;
            m_bGamePlayHint++;
        } else {
            m_bGamePlayHint = 0;
            if (pHints->GetINTEntry(m_bGamePlayHint, "GamePlayHint", &nValue)) {
                nStrRef = nValue;
                m_bGamePlayHint++;
            }
        }
    }
    pHints->Unload2DArray();
    delete pHints;
    return nStrRef;
}

class CAreaScene {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void Resume();                          // 0x60 (descriptive)
    virtual void Pause();                           // 0x64 (descriptive)
};

// FUNCTION: SWKOTOR 0x005F7FA0
void CClientExoAppInternal::SetPauseState(unsigned char nState, int bPause, CExoLinkedList<unsigned long>* pList)
{
    if ((m_nPauseFlags & nState) == bPause) {
        return;
    }
    if (bPause == 1) {
        m_nPauseFlags |= nState;
        m_pWorldTimer->PauseWorldTimer();
        m_pWorldTimer->SetSnapshotTime();
        m_pWorldTimer->SetSnapshotTime();
        if (m_nPauseFlags & 2) {
            m_pExclusionTimer->PauseWorldTimer();
            m_pExclusionTimer->SetSnapshotTime();
            m_pExclusionTimer->SetSnapshotTime();
        }
        if (g_pAppManager->m_pClientExoApp->GetModule()) {
            CSWCModulePlayer* pArea = g_pAppManager->m_pClientExoApp->GetModule()->GetArea();
            if (pArea && pArea->GetAreaScene()) {
                ((CAreaScene*)pArea->GetAreaScene())->Pause();
            }
        }
    } else {
        m_nPauseFlags &= ~nState;
        if (nState == 2) {
            m_pPauseTimer->PauseWorldTimer();
            m_pAnimationTimer->PauseWorldTimer();
            m_pPauseTimer->SyncTo(m_pExclusionTimer);
            m_pAnimationTimer->SyncTo(m_pExclusionTimer);
            m_pPauseTimer->SetSnapshotTime();
            m_pAnimationTimer->SetSnapshotTime();
            m_pExclusionTimer->SetSnapshotTime();
            m_pPauseTimer->ResetLastSnapshotTime();
            m_pAnimationTimer->ResetLastSnapshotTime();
            m_pExclusionTimer->ResetLastSnapshotTime();
            if ((m_nPauseFlags & 2) || !(m_nPauseFlags & 1)) {
                m_pWorldTimer->SetSnapshotTime();
                m_pWorldTimer->ResetLastSnapshotTime();
                m_pWorldTimer->UnpauseWorldTimer();
                if (m_pModule) {
                    CSWCModulePlayer* pArea = m_pModule->m_pPlayer;
                    if (pArea) {
                        CAreaScene* pScene = (CAreaScene*)pArea->GetAreaScene();
                        if (pScene) {
                            pScene->Resume();
                        }
                    }
                }
            }
            m_pExclusionTimer->UnpauseWorldTimer();
            m_pPauseTimer->UnpauseWorldTimer();
            m_pAnimationTimer->UnpauseWorldTimer();
        } else if (nState == 1) {
            m_pExclusionTimer->PauseWorldTimer();
            m_pPauseTimer->PauseWorldTimer();
            m_pAnimationTimer->PauseWorldTimer();
            m_pExclusionTimer->SyncTo(m_pWorldTimer);
            m_pPauseTimer->SyncTo(m_pWorldTimer);
            m_pAnimationTimer->PauseWorldTimer();
            m_pWorldTimer->SetSnapshotTime();
            m_pExclusionTimer->SetSnapshotTime();
            m_pPauseTimer->SetSnapshotTime();
            m_pAnimationTimer->SetSnapshotTime();
            m_pPauseTimer->ResetLastSnapshotTime();
            m_pAnimationTimer->ResetLastSnapshotTime();
            m_pExclusionTimer->ResetLastSnapshotTime();
            m_pWorldTimer->ResetLastSnapshotTime();
            m_pWorldTimer->UnpauseWorldTimer();
            m_pExclusionTimer->UnpauseWorldTimer();
            m_pPauseTimer->UnpauseWorldTimer();
            m_pAnimationTimer->UnpauseWorldTimer();
            if (m_pModule) {
                CSWCModulePlayer* pArea = m_pModule->m_pPlayer;
                if (pArea) {
                    CAreaScene* pScene = (CAreaScene*)pArea->GetAreaScene();
                    if (pScene) {
                        pScene->Resume();
                    }
                }
            }
        }
    }
    ClearPauseExclusionList();
    if (pList) {
        CExoLinkedListPosition pos = pList->GetHeadPos();
        while (pos) {
            AddToPauseExclusionList((unsigned long)pList->m_pcExoLinkedListInternal->GetAtPos(pos));
            pList->GetNext(pos);
        }
        delete pList->m_pcExoLinkedListInternal;
        delete pList;
    }
}

// FUNCTION: SWKOTOR 0x00602AF0
int CClientExoAppInternal::PlayMoviesInExoArrayList(CExoArrayList<CExoString>* pList, int* pResult,
                                                    int bWait, unsigned long nFlags, CExoArrayList<int>* pMovieFlags)
{
    if (m_bMovieQueueActive) {
        return 0;
    }
    m_pGuiInGame->AdjustHideCount(1);
    *pResult = 0;
    int bInputActive = g_pExoInput->GetActive();
    if (!bInputActive) {
        if (g_pExoInput) {
            g_pExoInput->SetActive(1);
        }
        ClearInputState(0);
    }
    g_pExoInput->ClearEvents();
    g_pExoSound->SetMusicState(SOUND_MODE_MOVIE);
    SetInputState(8);
    PauseTimers();
    m_pMoviePlayer->ShowFadePanel();
    ((CExoMoviePlayer*)m_pMoviePlayer)->PlayMoviesAsync(*pList, nFlags, pMovieFlags);
    Sleep(2);
    *pResult = WaitForMovie(bWait);
    ResumeTimers();
    ClearInputState(8);
    m_pMoviePlayer->HideFadePanel();
    pList->num = 0;
    if (pMovieFlags) {
        pMovieFlags->num = 0;
    }
    if (!bInputActive) {
        if (g_pExoInput) {
            g_pExoInput->SetActive(0);
        }
        TurnOffKeyboardModal();
    }
    m_pGuiInGame->AdjustHideCount(0);
    return 1;
}

class CExoFile {
public:
    CExoFile(const CExoString& sFile, unsigned short nType, const CExoString& sMode);  // 0x005E68D0
    ~CExoFile();                                    // 0x005E69E0
    int FileOpened();                               // 0x005E6A10
    void* m_pcExoFileInternal;                      // 0x00
};

class CExoAliasList {
public:
    CExoString* GetAliasPath(const CExoString& sAlias, int n);                 // 0x005E6890
    CExoString ResolveFileName(const CExoString& sFile, unsigned short nType); // 0x005E68A0
};

class CClientOptionsMovies {
public:
    void SetMovieShown(const CExoString& sMovie, int bShown);   // 0x0061D230
};

extern unsigned char g_nNumLiveDirectories;         // 0x0073ECD2 (descriptive)

// FUNCTION: SWKOTOR 0x005FBBF0
int CClientExoAppInternal::AddMovieToExoArrayList(const CExoString& sMovie, CExoArrayList<CExoString>& lstMovies)
{
    if (sMovie != "") {
        int nColon = sMovie.Find(":", 0);
        if (nColon != -1) {
            lstMovies.AddUnique(sMovie);
            return 1;
        }
        CExoString sFile;
        CExoString sAlias;
        sFile.Format("MOVIES:%s", sMovie.CStr());
        int bFound = 0;
        CExoFile* pFile = new CExoFile(sFile, 0x80f, "rb");
        if (pFile->FileOpened()) {
            bFound = 1;
        } else {
            for (int i = 1; i < g_nNumLiveDirectories; i++) {
                if (pFile) {
                    delete pFile;
                    pFile = 0;
                }
                sAlias.Format("LIVE%d", i);
                if (g_pExoBase->m_pcExoAliasList->GetAliasPath(sAlias, 0)->GetLength()) {
                    sFile.Format("LIVE%d:movies\\%s", i, sMovie.CStr());
                    pFile = new CExoFile(sFile, 0x80f, "rb");
                    if (pFile->FileOpened()) {
                        bFound = 1;
                        break;
                    }
                }
            }
        }
        if (pFile) {
            delete pFile;
        }
        if (bFound) {
            sFile = g_pExoBase->m_pcExoAliasList->ResolveFileName(sFile, 0x80f);
            lstMovies.AddUnique(sFile);
            ((CClientOptionsMovies*)m_pClientOptions)->SetMovieShown(sFile, 1);
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005F7500
void CClientExoAppInternal::UpdateRumble(float fDelta)
{
    float fLeft = 0.0f;
    float fRight = 0.0f;
    if (fDelta > 0.0f && !m_bAreaNotReadyToDisplay && !m_pGuiInGame->IsGlobalFadeObscuring() &&
        !m_pGuiInGame->IsGlobalFading() && !LoadScreenExists()) {
        for (int i = m_lstActiveRumbles.num - 1; i >= 0; i--) {
            CActiveRumble* pRumble = &m_lstActiveRumbles[i];
            if (pRumble->m_bActive) {
                pRumble->m_bActive = 0;
            } else {
                pRumble->m_fTime += fDelta;
            }
            float fPatternLeft;
            float fPatternRight;
            if (!m_lstRumblePatterns[pRumble->m_nPattern].GetIntensity(pRumble->m_fTime, &fPatternLeft, &fPatternRight)) {
                m_lstActiveRumbles.RemoveIndex(i);
            } else {
                if (!(fLeft > fPatternLeft)) {
                    fLeft = fPatternLeft;
                }
                if (!(fRight > fPatternRight)) {
                    fRight = fPatternRight;
                }
            }
        }
    }
    g_pExoInput->SetRumble(0, fLeft, fRight, 600000);
}

// FUNCTION: SWKOTOR 0x005F7330
void CClientExoAppInternal::UpdateDeathSequence()
{
    // NONMATCH (31 insns): x87 scheduling. The original copies the speed
    // scale through an integer register into a float local, keeps the
    // elapsed time on the FPU stack and emits fchs/fdiv/fadd dword.
    unsigned long nNow = (unsigned long)(g_pExoBase->m_pcExoTimers->GetHighResolutionTimer() / 1000);
    unsigned long nDelta = nNow - m_nDeathLastTime;
    m_nDeathLastTime = nNow;
    if (m_bGameOver) {
        float fScale = m_fDeathSpeedScale;
        float fSpeed = 0.2f;
        float fElapsed = m_fDeathElapsed += nDelta * 0.001f;
        if (m_bGameOverSet) {
            m_bGameOverSet = 0;
            fScale = m_fDeathSpeedScale = (float)(log(fElapsed * 0.25f) * (-1.0 / (1.0 - 0.2f)));
        }
        if (fElapsed <= 4.0f) {
            fSpeed += -log(fElapsed * 0.25f) / fScale;
        }
        SetGameSpeed(fSpeed);
        if (!m_pGuiInGame->IsGlobalFading() || m_nUnknown1a8) {
            if (m_fUnknown374 <= 0.0f) {
                SetGameSpeed(1.0f);
                m_bGameOver = 0;
                m_bGameOverSet = 1;
                m_pGuiInGame->EndGlobalFade();
                m_nUnknown1a8 = 1;
                m_fUnknown374 = 0;
                m_nUnknown1ac = 1;
                CSWGuiMessageBox* pDeathPanel = m_pGuiInGame->m_pMessageBox;
                g_pGuiManager->PopModalPanel();
                g_pGuiManager->RemovePanel(pDeathPanel);
            }
        }
    }
}
