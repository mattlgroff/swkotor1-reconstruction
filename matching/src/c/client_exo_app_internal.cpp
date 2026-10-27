// CClientExoAppInternal members (b005f_client_exo_app_internal_leaves,
// b005f3_client_internal_lists, b005e2_client_app_lifecycle).

#include <string.h>
#include "c/client_exo_app.h"

inline CWorldTimer* CClientExoAppInternal::GetActiveTimer(unsigned long oid)
{
    if (oid != OBJECT_INVALID && ((m_nPauseFlags & 2) || (m_nPauseFlags & 1))) {
        if (!(m_nPauseFlags & 2)) {
            if (m_nPauseFlags & 1) {
                if (IsOnExclusionList(oid) == 1) {
                    return m_pExclusionTimer;
                }
            } else {
                return 0;
            }
        } else {
            if (IsOnExclusionList(oid) == 1) {
                return m_pPauseTimer;
            }
        }
    }
    return m_pWorldTimer;
}

extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long nMs);

extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C
extern const unsigned char OBJECT_TYPE_CREATURE;    // 0x00746634

class CGuiInGame {
public:
    char pad_0x0[0xb4];
    int m_nCombatPauseMode;                         // 0xb4
    char pad_0xb8[0xb4];
    int m_bBarkStarted;                             // 0x16c
    char pad_0x170[0x10];
    unsigned long m_oidBarkSpeaker;                 // 0x180
    char pad_0x184[0x9bc];
    CExoSoundSource* m_pBarkSound;                  // 0xb40

    void ShowBarkBubble(unsigned long oid, const CExoString& text, const CResRef& sound, int b);   // 0x0062AB30
    void UpdateCombatMode();                        // 0x0062B050
    void AdjustHideCount(int n);                    // 0x0062B410
    void ShutDown();                                // 0x0062C310
};

class CSWCCreatureBark {
public:
    void StopLipSync();                             // 0x0060F540
};

class CExoInput {
public:
    void PauseRumble();                             // 0x005DF570
    void UnpauseRumble();                           // 0x005DF580
    void SetActive(int b);                          // 0x005DF540
    void ClearEvents();                             // 0x005DF4E0
};

class CSWSCreature {
public:
    char pad_0x0[0xa88];
    int m_bIsPC;                                    // 0xa88
};

void PauseTimers();                                 // 0x00403CF0
void ResumeTimers();                                // 0x00403DE0

extern CExoInput* g_pExoInput;                      // 0x007A39E4
extern const int MUSIC_STATE_NORMAL;                // 0x0074C5EC (0)
extern const int MUSIC_STATE_COMBAT_PAUSED;         // 0x0074C5F4 (2)
extern const unsigned char OBJECT_TYPE_ITEM;        // 0x00746635
extern int g_nMusicStateMenu;                       // 0x0074C5F0
extern int g_nMusicStateNone;                       // 0x0074C5EC

class CDeletable {
public:
    virtual ~CDeletable();
};
extern CDeletable* g_pUnknown8338FC;                // 0x008338FC
extern CDeletable* g_pUnknown8338EC;                // 0x008338EC
extern CDeletable* g_pUnknown7A3A28;                // 0x007A3A28

// FUNCTION: SWKOTOR 0x005EE920
CSWCQuestJournal* CClientExoAppInternal::CreateQuestJournal()
{
    if (m_pQuestJournal) {
        return m_pQuestJournal;
    }
    return m_pQuestJournal = new CSWCQuestJournal();
}

// FUNCTION: SWKOTOR 0x005EE990
CSWCWorldJournal* CClientExoAppInternal::CreateWorldJournal()
{
    if (m_pWorldJournal) {
        return m_pWorldJournal;
    }
    return m_pWorldJournal = new CSWCWorldJournal();
}

// FUNCTION: SWKOTOR 0x005EEA00
void CClientExoAppInternal::JournalUpdated(int a, int b, int c)
{
    if (m_pQuestJournal) {
        m_pQuestJournal->JournalUpdated(a, b, c);
    }
}

// FUNCTION: SWKOTOR 0x005EEA20
CSWCModule* CClientExoAppInternal::CreateModule()
{
    m_pModule = new CSWCModule();
    return m_pModule;
}

// FUNCTION: SWKOTOR 0x005EEA90
int CClientExoAppInternal::SendSaveGameRequest(int a, int b, int c)
{
    if (m_pSaveLoad) {
        return m_pSaveLoad->SendSaveGameRequest(a, b, c);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005EEAB0
int CClientExoAppInternal::SendLoadGameRequest(int a, int b, int c)
{
    if (m_pSaveLoad) {
        return m_pSaveLoad->SendLoadGameRequest(a, b, c);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005EEAE0
int CClientExoAppInternal::SetUnknown94(int n)
{
    m_nUnknown94 = n;
    return g_pAppManager->GetClientExoApp()->GetClientOptions()->SetOption(1, n);
}

// FUNCTION: SWKOTOR 0x005F1420
CSWCCreature* CClientExoAppInternal::GetPlayerCreature()
{
    if (m_oidCachedPlayer == m_oidPlayer) {
        return m_pCachedPlayerCreature;
    }
    CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_oidPlayer);
    m_pCachedPlayerCreature = pCreature;
    m_oidCachedPlayer = m_oidPlayer;
    return pCreature;
}

// FUNCTION: SWKOTOR 0x005F1490
void CClientExoAppInternal::SetUnknown194(int n)
{
    m_nUnknown194 = n;
}

// FUNCTION: SWKOTOR 0x005F2AE0
int CClientExoAppInternal::LoadScreenExists()
{
    if (m_pLoadScreen && m_pGuiManager) {
        return m_pGuiManager->PanelExists(m_pLoadScreen);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005F2B30
CExoString* CClientExoAppInternal::GetPlayerCharacterName()
{
    m_nPlayerCharacterSet = 0;
    return &m_sPlayerCharacterName;
}

// FUNCTION: SWKOTOR 0x005F2BD0
int CClientExoAppInternal::IsMoviePlaying()
{
    if (m_pMoviePlayer == 0) {
        return 0;
    }
    return m_pMoviePlayer->GetState();
}

// FUNCTION: SWKOTOR 0x005F2BF0
int CClientExoAppInternal::InitializeMoviePlayer()
{
    m_pMoviePlayer = 0;
    m_pMoviePlayer = new CMoviePlayer();
    return m_pMoviePlayer->Initialize(0);
}

// FUNCTION: SWKOTOR 0x005F2DF0
int CClientExoAppInternal::GetPausedByCombat()
{
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    if (pServer) {
        return pServer->GetPauseState(2);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005F2ED0
int CClientExoAppInternal::GetControlledCharacterDead()
{
    CSWPartyMember* pMember = g_pAppManager->m_pClientExoApp->GetParty()->GetMember(0);
    if (pMember) {
        CSWCCreature* pCreature = pMember->GetCreature();
        if (pCreature) {
            return pCreature->IsDead();
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005F2F00
void CClientExoAppInternal::SetGameOver(int b)
{
    m_bGameOver = b;
    m_bGameOverSet = 1;
}

// FUNCTION: SWKOTOR 0x005F37F0
void CClientExoAppInternal::SetLoadScreenHint(int nHint)
{
    if (m_pLoadScreen) {
        m_pLoadScreen->SetHint(nHint);
    }
}

// FUNCTION: SWKOTOR 0x005F40E0
void CClientExoAppInternal::DisableVideoEffect()
{
    m_nVideoEffect = -2;
    AurDisableSaturation();
    AurDisableScanNoise();
}

// FUNCTION: SWKOTOR 0x005F46F0
int CClientExoAppInternal::ForwardUnknown46f0()
{
    return g_pAppManager->m_pClientExoApp->GetUnknown700()->Unknown61b780();
}

// FUNCTION: SWKOTOR 0x005F4A00
void CClientExoAppInternal::CancelMovie()
{
    if (m_pMoviePlayer) {
        m_pMoviePlayer->Stop(1, 0);
    }
}

// FUNCTION: SWKOTOR 0x005F4A60
void CClientExoAppInternal::WaitSoundFade750()
{
    unsigned long nStart = (unsigned long)(g_pExoBase->m_pcExoTimers->GetHighResolutionTimer() / 1000);
    if (m_pMenuMusic) {
        m_pMenuMusic->SetFadeTime(2500);
    }
    do {
        g_pExoSound->Update();
    } while ((unsigned long)(g_pExoBase->m_pcExoTimers->GetHighResolutionTimer() / 1000) - nStart < 750);
    if (m_pMenuMusic) {
        m_pMenuMusic->Stop();
    }
}

// FUNCTION: SWKOTOR 0x005F4AE0
void CClientExoAppInternal::WaitSound500()
{
    unsigned long nStart = (unsigned long)(g_pExoBase->m_pcExoTimers->GetHighResolutionTimer() / 1000);
    g_pExoSound->SetMusicState(g_nMusicStateMenu);
    do {
        g_pExoSound->Update();
    } while ((unsigned long)(g_pExoBase->m_pcExoTimers->GetHighResolutionTimer() / 1000) - nStart < 500);
}

// FUNCTION: SWKOTOR 0x005F4C50
void CClientExoAppInternal::HideMoviePlayerFadePanel()
{
    if (m_pMoviePlayer) {
        m_pMoviePlayer->HideFadePanel();
    }
}

// FUNCTION: SWKOTOR 0x005F4D50
void CClientExoAppInternal::StopCreditSequence()
{
    m_bCreditSequenceInProgress = 0;
}

// FUNCTION: SWKOTOR 0x005F4DB0
void CClientExoAppInternal::UpdateMovieState()
{
    if (m_pMoviePlayer && m_pMoviePlayer->GetState()) {
        return;
    }
    if (m_bUnknown200) {
        m_bUnknown200 = 0;
        ClearInputState(8);
    }
}

// FUNCTION: SWKOTOR 0x005F6340
void CClientExoAppInternal::AddObjectToHitcheckIgnoreList(unsigned long oid)
{
    CExoLinkedListPosition pos = m_lstHitcheckIgnore.GetHeadPos();
    unsigned long* pEntry = m_lstHitcheckIgnore.GetHead();
    while (pos) {
        if (pEntry && *pEntry == oid) {
            return;
        }
        pEntry = m_lstHitcheckIgnore.GetNext(pos);
    }
    unsigned long* pNew = new unsigned long;
    *pNew = oid;
    m_lstHitcheckIgnore.AddTail(pNew);
}

// NONMATCH (12 insns): register choice only; original keeps pos in ecx and
// the list internal in eax, we swap them.
// FUNCTION: SWKOTOR 0x005F63A0
void CClientExoAppInternal::RemoveObjectFromHitcheckIgnoreList(unsigned long oid)
{
    CExoLinkedListPosition pos = m_lstHitcheckIgnore.GetHeadPos();
    unsigned long* pEntry = m_lstHitcheckIgnore.GetHead();
    while (pos) {
        if (pEntry && *pEntry == oid) {
            m_lstHitcheckIgnore.Remove(pos);
            delete pEntry;
            return;
        }
        pEntry = m_lstHitcheckIgnore.GetNext(pos);
    }
}

// FUNCTION: SWKOTOR 0x005F6410
void CClientExoAppInternal::EmptyHitcheckIgnoreList()
{
    while (!m_lstHitcheckIgnore.IsEmpty()) {
        unsigned long* pEntry = m_lstHitcheckIgnore.RemoveHead();
        if (pEntry) {
            delete pEntry;
        }
    }
}

// FUNCTION: SWKOTOR 0x005F6440
void CClientExoAppInternal::StopMenuMusic()
{
    if (m_pMenuMusic) {
        FadeOutMusic(m_pMenuMusic, 500);
        delete m_pMenuMusic;
        m_pMenuMusic = 0;
    }
}

// FUNCTION: SWKOTOR 0x005F7190
void CClientExoAppInternal::ShutDownMoviePlayer()
{
    if (m_pMoviePlayer) {
        while (m_pMoviePlayer->GetState() == 1) {
            m_pMoviePlayer->Stop(1, 1);
            Sleep(5);
        }
        delete m_pMoviePlayer;
        m_pMoviePlayer = 0;
    }
}

// NONMATCH (13 insns): pObject = 0 store and the num-- sequence are scheduled
// differently (original: edx = num - 1; eax = edx).
// FUNCTION: SWKOTOR 0x005F7640
void CClientExoAppInternal::UpdateHostileHilite(float fDelta)
{
    for (int i = m_lstHostileHilites.num - 1; i >= 0; i--) {
        m_lstHostileHilites[i].m_fTimeLeft -= fDelta;
        if (m_lstHostileHilites[i].m_fTimeLeft <= 0.0f) {
            CGameObject* pObject = 0;
            if (m_pGameObjectArray->GetGameObject(m_lstHostileHilites[i].m_oidTarget, &pObject) == GAME_OBJECT_VALID &&
                pObject && pObject->m_nObjectType == OBJECT_TYPE_CREATURE) {
                CSWCCreature* pCreature = pObject->AsSWCCreature();
                if (pCreature) {
                    pCreature->SetHostileHilite(0);
                }
            }
            m_lstHostileHilites.num--;
            for (int j = i; j < m_lstHostileHilites.num; j++) {
                m_lstHostileHilites[j] = m_lstHostileHilites[j + 1];
            }
        }
    }
}

// NONMATCH (22 insns, layout matches): the inlined GetActiveTimer tests
// flag 1 as `and al, 1; test al, al` in the original, `test al, 1` here;
// GetGameObject arg setup order also differs.
// FUNCTION: SWKOTOR 0x005F7E20
void CClientExoAppInternal::ClearPauseExclusionList()
{
    while (m_pPauseExclusionList->GetHead()) {
        unsigned long oid = (unsigned long)m_pPauseExclusionList->RemoveHead();
        CGameObject* pObject;
        m_pGameObjectArray->GetGameObject(oid, &pObject);
        if (pObject) {
            unsigned long nDay, nTime;
            GetActiveTimer(pObject->GetId())->GetWorldTime(&nDay, &nTime);
            pObject->SetWorldTime(nDay, nTime);
            pObject->AsSWCObject()->FindPart(0xff, 1)->Resume();
        }
    }
}

// FUNCTION: SWKOTOR 0x005F7F10
void CClientExoAppInternal::AddToPauseExclusionList(unsigned long oid)
{
    CGameObject* pObject;
    m_pGameObjectArray->GetGameObject(oid, &pObject);
    if (pObject) {
        pObject->AsSWCObject()->FindPart(0xff, 1)->Pause();
        m_pPauseExclusionList->AddTail((unsigned long*)oid);
        unsigned long nDay, nTime;
        GetActiveTimer(pObject->GetId())->GetWorldTime(&nDay, &nTime);
        pObject->SetWorldTime(nDay, nTime);
    }
}

// FUNCTION: SWKOTOR 0x005F8470
void CClientExoAppInternal::Uninitialize()
{
    ClearClientState();
    m_nUnknown248 = 0;
    while (!m_lstUnknown168.IsEmpty()) {
        CExoString* pString = m_lstUnknown168.RemoveHead();
        if (pString) {
            delete pString;
        }
    }
    EmptyHitcheckIgnoreList();
    if (g_pUnknown8338FC) {
        delete g_pUnknown8338FC;
        g_pUnknown8338FC = 0;
    }
    if (g_pUnknown8338EC) {
        delete g_pUnknown8338EC;
        g_pUnknown8338EC = 0;
    }
    if (g_pUnknown7A3A28) {
        delete g_pUnknown7A3A28;
        g_pUnknown7A3A28 = 0;
    }
    if (m_pLoadScreen) {
        delete m_pLoadScreen;
        m_pLoadScreen = 0;
    }
    if (m_pUnknownPanel27c) {
        delete m_pUnknownPanel27c;
        m_pUnknownPanel27c = 0;
    }
    SetCursorState(0xff, 1);
}

// FUNCTION: SWKOTOR 0x005FB490
int CClientExoAppInternal::PlayRumblePattern(int nPattern)
{
    if (nPattern < 0 || nPattern >= m_lstRumblePatterns.num) {
        return 0;
    }
    int nActive = m_lstActiveRumbles.num;
    if (m_lstRumblePatterns[nPattern].m_bExclusive) {
        for (int i = 0; i < nActive; i++) {
            if (m_lstActiveRumbles[i].m_nPattern == nPattern) {
                return 0;
            }
        }
    }
    m_lstActiveRumbles.SetSize(nActive + 1);
    m_lstActiveRumbles[nActive].m_nPattern = nPattern;
    m_lstActiveRumbles[nActive].m_nTime = 0;
    m_lstActiveRumbles[nActive].m_bActive = 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x005FCBD0
void CClientExoAppInternal::ShutDownToMainMenu()
{
    CGameObject* pDeleted = 0;
    if (m_bUnknown254) {
        m_bUnknown250 = 1;
    }
    g_pAppManager->Unknown401280();
    if (m_pModule) {
        CSWCModulePlayer* pPlayer = m_pModule->m_pPlayer;
        if (pPlayer) {
            m_pGameObjectArray->Delete(pPlayer->m_idPlayerObject, &pDeleted);
            pPlayer->Unknown609410();
        }
        UnloadModule();
    }
    if (m_bUnknown254 == 0) {
        ResetGuis(0);
        g_pExoSound->SetMusicState(g_nMusicStateNone);
        m_bStartMenuMusicOnMovieEnd = 1;
        SetMusic(1, 0x7f, 2);
        if (m_nLoadBarState) {
            g_pAppManager->m_pPad->m_n14 = 0;
            CAppPad* pPad = g_pAppManager->m_pPad;
            pPad->m_n00 = 0;
            pPad->m_n04 = 0;
            m_nLoadBarState = 0;
        }
    }
}

// FUNCTION: SWKOTOR 0x005FCC90
void CClientExoAppInternal::AddObjectToIgnoreList(unsigned long oid, CExoArrayList<CGob*>* pList)
{
    CGameObject* pObject = 0;
    if (m_pGameObjectArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID && pObject) {
        switch (pObject->m_nObjectType) {
        case 5:
            AddCreatureModelToIgnoreList(oid, pList);
            break;
        case 6:
            AddItemModelDirectlyToIgnoreList(oid, pList);
            break;
        default:
            AddOtherModelToIgnoreList(oid, pList);
            break;
        }
    }
}

// FUNCTION: SWKOTOR 0x005F7C90 SYMBOL: ?SetSize@?$CExoArrayList@UCHostileHilite@@@@QAEXH@Z
template class CExoArrayList<CHostileHilite>;

// FUNCTION: SWKOTOR 0x005F1A90
void CClientExoAppInternal::SetCharacterFile(unsigned long nSize, const char* pData, unsigned long nValue)
{
    if (m_pCharacterFile) {
        delete[] m_pCharacterFile;
        m_pCharacterFile = 0;
    }
    if (pData) {
        m_pCharacterFile = new char[nSize + 1];
        memcpy(m_pCharacterFile, pData, nSize);
        m_pCharacterFile[nSize] = 0;
    }
    m_nCharacterFileSize = nSize;
    m_nCharacterFileValue = nValue;
}

// FUNCTION: SWKOTOR 0x005F1B90
void CClientExoAppInternal::StopLoadMusic()
{
    if (m_pLoadMusic) {
        m_pLoadMusic->Stop();
        if (m_pLoadMusic) {
            delete m_pLoadMusic;
        }
        m_pLoadMusic = 0;
    }
}

// Native callers ignore the facade result; its fallback calls void
// ShowBarkBubble and leaves EAX unchanged by any explicit return value.
// FUNCTION: SWKOTOR 0x005F2C90
void CClientExoAppInternal::BarkString(unsigned long oid, const CExoString& text, const CResRef& sound, int b)
{
    CGameObject* pObject = g_pAppManager->GetClientExoApp()->GetGameObject(oid);
    if (pObject) {
        pObject->AsSWCObject()->BarkString(oid, text, sound, b);
    } else {
        m_pGuiInGame->ShowBarkBubble(0x7f000000, text, sound, b);
    }
}

// FUNCTION: SWKOTOR 0x005F2D00
int CClientExoAppInternal::CanPlayBarkSound()
{
    // NONMATCH (11 diff insns): the original keeps 0 in edi for every null
    // compare and the miss result but returns IsPlaying() directly; a direct
    // return here lets the compiler fold the 0 (test reg,reg, xor eax,eax).
    int bResult = 0;
    if (m_pGuiInGame->m_bBarkStarted != bResult) {
        CGameObject* pObject = 0;
        if (m_pGameObjectArray->GetGameObject(m_pGuiInGame->m_oidBarkSpeaker, &pObject) == GAME_OBJECT_VALID && pObject) {
            if (pObject->AsSWCObject() && m_pGuiInGame->m_pBarkSound) {
                bResult = m_pGuiInGame->m_pBarkSound->IsPlaying();
            }
        }
        return bResult;
    }
    m_pGuiInGame->m_bBarkStarted = 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x005F2D80
void CClientExoAppInternal::StopBarkSound()
{
    // NONMATCH (18 diff insns): esi/edi swapped (original: this in esi,
    // client object in edi) and the pObject = 0 store is scheduled after
    // the argument pushes in the original.
    CGameObject* pObject = 0;
    if (m_pGameObjectArray->GetGameObject(m_pGuiInGame->m_oidBarkSpeaker, &pObject) != GAME_OBJECT_VALID || !pObject) {
        return;
    }
    CSWCObject* pClientObject = pObject->AsSWCObject();
    if (!pClientObject) {
        return;
    }
    if (pClientObject->AsSWCCreature()) {
        ((CSWCCreatureBark*)pClientObject->AsSWCCreature())->StopLipSync();
    }
    if (m_pGuiInGame->m_pBarkSound) {
        m_pGuiInGame->m_pBarkSound->Stop();
    }
}

// FUNCTION: SWKOTOR 0x005F2E10
void CClientExoAppInternal::SetPausedByCombat(int bPaused, unsigned char nFlag, int bForce)
{
    if ((m_pGuiInGame->m_nCombatPauseMode == 1 && bPaused == 0) || m_pGuiInGame->m_nCombatPauseMode == 0 || bForce == 1) {
        CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
        int nState = pServer ? pServer->GetPauseState(2) : 0;
        if (!m_bPausedByCombatSet && nState != bPaused) {
            m_bPausedByCombatSet = 1;
            m_nPausedByCombatFlag = nFlag;
            m_bPausedByCombat = bPaused;
            if (bPaused == 1) {
                g_pExoInput->PauseRumble();
                g_pExoSound->SetMusicState(MUSIC_STATE_COMBAT_PAUSED);
                m_bSoundPausedByCombat = bPaused;
            } else {
                g_pExoInput->UnpauseRumble();
                g_pExoSound->SetMusicState(MUSIC_STATE_NORMAL);
                m_bSoundPausedByCombat = 0;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006027A0
int CClientExoAppInternal::LoadMoveToModuleMovies(int n)
{
    return LoadMovieList(n, &m_lstMoveToModuleMovies);
}

// FUNCTION: SWKOTOR 0x005F2FE0
void CClientExoAppInternal::SetPortraitCreature(unsigned long oid)
{
    if (oid != OBJECT_INVALID) {
        CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oid);
        if (pCreature && pCreature->m_bIsPC) {
            m_oidPortraitCreature = g_pAppManager->GetClientExoApp()->ServerToClientObjectId(oid);
        }
    }
}

// FUNCTION: SWKOTOR 0x005F3A80
void CClientExoAppInternal::SetCombatMode(int bCombat)
{
    if (!bCombat) {
        m_nAttackMashTutorialTimer = bCombat;
    }
    if (m_bCombatMode != bCombat) {
        m_bCombatMode = bCombat;
        m_pGuiInGame->UpdateCombatMode();
        if (m_pModule) {
            if (m_bCombatMode) {
                m_pModule->EnterCombatMode();
            } else {
                m_pModule->ExitCombatMode();
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x005F6B10
void CClientExoAppInternal::SetInputActive(int bActive, int bClear)
{
    if (g_pExoInput) {
        g_pExoInput->SetActive(bActive);
    }
    if (!bActive) {
        ResetInput();
    }
    if (bClear) {
        g_pExoInput->ClearEvents();
    }
    if (bActive) {
        ClearInputState(0);
    }
}

// FUNCTION: SWKOTOR 0x005F9780
void CClientExoAppInternal::RestartMoviesIfNecessary()
{
    if (m_pMoviePlayer && m_pMoviePlayer->GetFrameCount() > 0) {
        SetInputState(8);
        PauseTimers();
        m_pMoviePlayer->Play();
        ShowLoadScreen(1);
        ResumeTimers();
        ClearInputState(8);
        m_pMoviePlayer->HideFadePanel();
    }
}

// Worker C s1 additions.

extern int g_bCameraFollowFlag;                     // 0x007A2484

class CSWPlayerControlCamRelative {
public:
    CSWPlayerControlCamRelative(unsigned long oidCreature, CSWCModuleScene* pCamera);  // 0x00679780
    void SetCreature(unsigned long oidCreature);    // 0x00679340 (descriptive)
    void SetCamera(CSWCModuleScene* pCamera);       // 0x006793B0 (descriptive)
    void Reset();                                   // 0x006792D0 (descriptive)
    char pad_0x0[0xcc];
};

class CSWGuiMainMenu : public CSWGuiPanel {
public:
    CSWGuiMainMenu(CSWGuiManager* pManager);        // 0x0067C4C0
    char pad_0x4[0x40];
    unsigned long m_nFlagsLow : 8;                  // 0x44
    unsigned long m_bFlag8 : 1;
    unsigned long m_bFlag9 : 1;
    unsigned long m_bFlag10 : 1;
    char pad_0x48[0x1414 - 0x48];
};

// FUNCTION: SWKOTOR 0x005F5CF0
void CClientExoAppInternal::AttachPlayerGameObjectToArea()
{
    // NONMATCH (5 insns): the original loads the area-scene argument into ecx.
    CSWCCreature* pCreature = GetPlayerCreature();
    if (!pCreature) {
        return;
    }
    pCreature->v94(m_pModule->GetArea()->m_pAreaScene);
    if (g_bCameraFollowFlag) {
        if (pCreature->m_nUnknown310 > 0) {
            pCreature->m_bUnknown314 = 1;
            pCreature->m_bUnknown318 = 1;
        }
    } else {
        m_pModule->SetMapCharacter(pCreature->GetId(), 1);
    }
    if (m_pCharacterPanel) {
        ((CSWPlayerControlCamRelative*)m_pCharacterPanel)->SetCreature(pCreature->GetId());
        ((CSWPlayerControlCamRelative*)m_pCharacterPanel)->SetCamera(m_pModule->GetScene());
    } else {
        m_pCharacterPanel = (CSWGuiPanel*)new CSWPlayerControlCamRelative(pCreature->GetId(), m_pModule->GetScene());
    }
}

// FUNCTION: SWKOTOR 0x005FCA30
void CClientExoAppInternal::ResetGuis(int n)
{
    if (m_pMainMenu && m_pGuiManager->PanelExists(m_pMainMenu)) {
        return;
    }
    if (m_pPreviousMainMenu) {
        if (m_pGuiManager->PanelExists(m_pPreviousMainMenu)) {
            CSWGuiMainMenu* pMenu = (CSWGuiMainMenu*)m_pPreviousMainMenu;
            pMenu->m_bFlag8 = 0;
            pMenu->m_bFlag9 = 0;
            pMenu->m_bFlag10 = 1;
        } else {
            delete m_pPreviousMainMenu;
        }
        m_pPreviousMainMenu = 0;
    }
    m_pMainMenu = new CSWGuiMainMenu(m_pGuiManager);
    SetInputClass(2, 1);
    m_pGuiManager->AddPanel(m_pMainMenu, 2, 1);
    SetMusic(1, 0x7f, 2);
    if (m_nUnknown494 == 1) {
        m_nUnknown494 = 0;
    }
    m_pGuiInGame->AdjustHideCount(0);
}

void SetClearColor(const char* sColor);             // 0x00401690
extern const unsigned long OBJECT_INVALID_ID;       // 0x00746644

// FUNCTION: SWKOTOR 0x005F8290
void CClientExoAppInternal::UnloadModule()
{
    SetClearColor("0 0 0");
    m_oidPlayer = OBJECT_INVALID_ID;
    if (m_pParty) {
        m_pParty->SetPlayerCreature(m_oidPlayer);
    }
    SetPausedByCombat(0, 0, 0);
    SetPauseState(2, 0, 0);
    SetPauseState(1, 0, 0);
    ClearPauseExclusionList();
    if (!m_pModule) {
        return;
    }
    delete m_pModule;
    m_pModule = 0;
    g_pAppManager->GetObjectTableManager(0)->RemovePlayer(0);
    delete m_pGameObjectArray;
    m_nUnknown248 = 0;
    if (m_pWorldJournal) {
        m_pWorldJournal->DeleteAllEntries();
    }
    if (m_pQuestJournal) {
        m_pQuestJournal->ClearQuests();
    }
    if (m_pGuiInGame) {
        m_pGuiInGame->ShutDown();
    }
    if (m_pCharacterPanel) {
        ((CSWPlayerControlCamRelative*)m_pCharacterPanel)->Reset();
    }
    m_pGameObjectArray = new CGameObjectArray(1);
    g_pAppManager->GetClientExoApp()->SetAreaNotReadyToDisplay(1);
    m_fUnknown38c = 0;
    m_fUnknown390 = 0;
    m_nUnknown394 = 0;
    m_nUnknown3a0 = 0;
    m_nAutoPauseFlag = 0xff;
    m_bAutoPaused = 0;
    m_bEnemySighted = 0;
    m_bUnknown328 = 0;
    m_bCombatMode = 0;
}

// FUNCTION: SWKOTOR 0x005F3F10
void CClientExoAppInternal::SetAutoPaused(int bPaused, int nFlag)
{
    if (m_bAutoPaused == bPaused) {
        return;
    }
    if (bPaused) {
        if (GetPausedByCombat()) {
            return;
        }
        if (m_fUnknown38c > 0.0f) {
            m_fUnknown38c = 0;
            m_fUnknown390 = 1.0f;
            m_nAutoPauseFlag = (unsigned char)nFlag;
            return;
        }
        if (m_pGuiInGame->m_nCombatPauseMode != 0) {
            return;
        }
        if ((unsigned char)nFlag == 9) {
            if (m_fUnknown39c > 0.0f) {
                return;
            }
            m_fUnknown39c = 2.0f;
        }
        m_bAutoPaused = 1;
        SetPausedByCombat(1, (unsigned char)nFlag, 0);
    } else {
        m_bAutoPaused = 0;
        if (GetPausedByCombat()) {
            SetPausedByCombat(0, 0, 0);
        }
    }
}
