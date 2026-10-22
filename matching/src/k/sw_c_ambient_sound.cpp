// CSWCAmbientSound, the client side of an area's music and
// ambient loops (vtable 0x007541C8), 0x0068D4A0-0x0068F0D9.
#include "k/sw_c_ambient_sound.h"

#include <stdlib.h>

// The mode-one ("truncated") body is written out in PlayMusic,
// PlayBattleMusic and PlayAmbientSound with a constant kind.
#define PLAY_TRUNCATED_BODY(bPlay, nKind)                                   \
    switch (nKind) {                                                        \
    case 1: m_bMusicPlaying = bPlay; break;                                 \
    case 2: m_bAmbientPlaying = bPlay; break;                               \
    case 4: m_bBattleMusicPlaying = bPlay; break;                           \
    }                                                                       \
    m_nTimer = 10;                                                          \
    if (m_pMusicSource->IsPlaying()) {                                      \
        if (nKind > m_nPlayingKind) {                                       \
            if (bPlay)                                                      \
                m_pMusicSource->FadeAndStop(1000);                          \
        }                                                                   \
        else if (nKind == m_nPlayingKind && !bPlay) {                       \
            m_pMusicSource->FadeAndStop(1000);                              \
            if (nKind == 4)                                                 \
                PlayStinger(&m_lstMusicBattle);                             \
        }                                                                   \
    }                                                                       \
    else if (bPlay) {                                                       \
        if (m_bBattleMusicPlaying) {                                        \
            CResRef cResRef(m_lstMusicBattle.m_sResource);                  \
            if (!m_pMusicSource->Play(cResRef, 8, 0, 1, 0))                 \
                m_nTimer = 10000;                                           \
            m_nPlayingKind = 4;                                             \
        }                                                                   \
        else if (m_bAmbientPlaying) {                                       \
            if (m_bDay) {                                                   \
                CResRef cResRef(m_lstAmbientDay.m_sResource);               \
                if (!m_pMusicSource->Play(cResRef, 4, 0, 1, 0))             \
                    m_nTimer = 10000;                                       \
            }                                                               \
            else {                                                          \
                CResRef cResRef(m_lstAmbientNight.m_sResource);             \
                if (!m_pMusicSource->Play(cResRef, 4, 0, 1, 0))             \
                    m_nTimer = 10000;                                       \
            }                                                               \
            m_nPlayingKind = 2;                                             \
        }                                                                   \
        else if (m_bMusicPlaying) {                                         \
            if (m_bDay) {                                                   \
                CResRef cResRef(m_lstMusicDay.m_sResource);                 \
                if (!m_pMusicSource->Play(cResRef, 8, 0, 1, 0))             \
                    m_nTimer = 10000;                                       \
            }                                                               \
            else {                                                          \
                CResRef cResRef(m_lstMusicNight.m_sResource);               \
                if (!m_pMusicSource->Play(cResRef, 8, 0, 1, 0))             \
                    m_nTimer = 10000;                                       \
            }                                                               \
            m_nPlayingKind = 1;                                             \
        }                                                                   \
        else {                                                              \
            m_nTimer = 10000;                                               \
            m_nPlayingKind = 0;                                             \
        }                                                                   \
    }

// FUNCTION: SWKOTOR 0x0068D4A0
void CSWCAmbientSound::Activate()
{
    m_bActive = 1;
    m_nMusicMode = g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nMusicFlags & 3;
    if (m_bMusicEnabled) {
        if (m_bBattleMusicPlaying)
            PlayBattleMusic(1);
        else
            PlayMusic(m_bMusicPlaying);
    }
    if (m_bAmbientEnabled)
        PlayAmbientSound(m_bAmbientPlaying);
}

// FUNCTION: SWKOTOR 0x0068D510
void CSWCAmbientSound::SetMusicDelay(int nDelay)
{
    if (nDelay > 0)
        m_nMusicDelayMs = nDelay;
}

// FUNCTION: SWKOTOR 0x0068D530
void CSWCAmbientSound::SetAmbientDayVolume(int nVolume)
{
    CExoString sPlaying;
    m_pAmbientSource->GetFilePlaying().CopyToString(sPlaying);
    if (m_nAmbientSndDayVol != nVolume) {
        if (m_pAmbientSource->IsPlaying() && sPlaying.CompareNoCase(m_lstAmbientDay.m_sResource))
            m_pAmbientSource->SetVolume(nVolume);
        m_nAmbientSndDayVol = nVolume;
    }
}

// FUNCTION: SWKOTOR 0x0068D5E0
void CSWCAmbientSound::SetAmbientNightVolume(int nVolume)
{
    CExoString sPlaying;
    m_pAmbientSource->GetFilePlaying().CopyToString(sPlaying);
    if (m_nAmbientSndNitVol != nVolume) {
        if (m_pAmbientSource->IsPlaying() && sPlaying.CompareNoCase(m_lstAmbientNight.m_sResource))
            m_pAmbientSource->SetVolume(nVolume);
        m_nAmbientSndNitVol = nVolume;
    }
}

// FUNCTION: SWKOTOR 0x0068D690
void CSWCAmbientSound::UpdateDayNight()
{
    CExoString sMusic;
    CExoString sAmbient;
    m_pMusicSource->GetFilePlaying().CopyToString(sMusic);
    m_pAmbientSource->GetFilePlaying().CopyToString(sAmbient);
    switch (g_pAppManager->m_pClientExoApp->GetModule()->m_nDayState) {
    case 0:
    case 1:
    case 3:
        if (m_bDay != 1) {
            if (sMusic.CompareNoCase(m_lstMusicNight.m_sResource) ||
                (m_nMusicMode && sMusic.CompareNoCase(m_lstAmbientNight.m_sResource))) {
                PlayMusic(0);
                PlayMusic(1);
            }
            if (sAmbient.CompareNoCase(m_lstAmbientNight.m_sResource)) {
                PlayAmbientSound(0);
                PlayAmbientSound(1);
            }
        }
        m_bDay = 1;
        break;
    case 2:
    case 4:
        if (m_bDay) {
            if (sMusic.CompareNoCase(m_lstMusicDay.m_sResource) ||
                (m_nMusicMode && sMusic.CompareNoCase(m_lstAmbientDay.m_sResource))) {
                PlayMusic(0);
                PlayMusic(1);
            }
            if (sAmbient.CompareNoCase(m_lstAmbientDay.m_sResource)) {
                PlayAmbientSound(0);
                PlayAmbientSound(1);
            }
        }
        m_bDay = 0;
        break;
    }
}

// NONMATCH (56): the original keeps a separate stack slot for the first
// "Resource" column temp (frame 0x20, ours 0x18) and reloads nRow per call;
// the bFound variable / plain-condition forms give the same code.
// FUNCTION: SWKOTOR 0x0068D860
void CSWCAmbientSound::LoadSoundList(int bMusic, int nRow, CSWCAmbientSoundList* pList)
{
    CExoString sValue;
    int bFound;
    if (bMusic) {
        C2DA* p2DA = g_pRules->m_p2DArrays->m_pAmbientMusic;
        bFound = p2DA->GetCExoStringEntry(nRow, CExoString("Resource"), &sValue);
        if (bFound)
            pList->m_sResource = sValue.UpperCase();
        else
            pList->m_sResource = "";
        bFound = p2DA->GetCExoStringEntry(nRow, CExoString("Stinger1"), &sValue);
        if (bFound)
            pList->m_sStinger1 = sValue.UpperCase();
        else
            pList->m_sStinger1 = "";
        bFound = p2DA->GetCExoStringEntry(nRow, CExoString("Stinger2"), &sValue);
        if (bFound)
            pList->m_sStinger2 = sValue.UpperCase();
        else
            pList->m_sStinger2 = "";
        bFound = p2DA->GetCExoStringEntry(nRow, CExoString("Stinger3"), &sValue);
        if (bFound)
            pList->m_sStinger3 = sValue.UpperCase();
        else
            pList->m_sStinger3 = "";
    }
    else {
        C2DA* p2DA = g_pRules->m_p2DArrays->m_pAmbientSound;
        bFound = p2DA->GetCExoStringEntry(nRow, CExoString("Resource"), &sValue);
        if (bFound) {
            pList->m_sResource = sValue.UpperCase();
            m_bAmbientFailed = 0;
        }
        else
            pList->m_sResource = "";
    }
}

// FUNCTION: SWKOTOR 0x0068DB30
void CSWCAmbientSound::PlayStinger(CSWCAmbientSoundList* pList)
{
    CResRef cResRef;
    if (m_bActive) {
        if (pList->m_sStinger1 != "") {
            int nCount = 1;
            if (pList->m_sStinger2 != "") {
                nCount = 2;
                if (pList->m_sStinger3 != "")
                    nCount = 3;
            }
            switch (rand() % nCount) {
            case 0:
                cResRef = pList->m_sStinger1;
                break;
            case 1:
                cResRef = pList->m_sStinger2;
                break;
            case 2:
                cResRef = pList->m_sStinger3;
                break;
            }
            if (m_pStingerSource)
                delete m_pStingerSource;
            m_pStingerSource = new CExoStreamingSoundSource();
            m_pStingerSource->SetPriorityGroup(1);
            if (!m_pStingerSource->Play(cResRef, 4, 0, 0, 0)) {
                if (m_pStingerSource)
                    delete m_pStingerSource;
                m_pStingerSource = 0;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0068DC90
void CSWCAmbientSound::ResetTimer()
{
    CClientExoApp* pApp = g_pAppManager->m_pClientExoApp;
    pApp->GetPauseTimer()->GetWorldTime(&m_nTimerDay, &m_nTimerTime);
}

// FUNCTION: SWKOTOR 0x0068DCC0
unsigned long CSWCAmbientSound::GetElapsedTime()
{
    unsigned long nDay, nTime;
    unsigned long nDiffDay, nDiffTime;
    CClientExoApp* pApp = g_pAppManager->m_pClientExoApp;
    pApp->GetPauseTimer()->GetWorldTime(&nDay, &nTime);
    pApp = g_pAppManager->m_pClientExoApp;
    pApp->GetPauseTimer()->SubtractWorldTimes(
        nDay, nTime, m_nTimerDay, m_nTimerTime, &nDiffDay, &nDiffTime);
    return nDiffTime;
}

// NONMATCH (21): day / night blocks are cross-jumped the other way (the
// original lays the night copy after the return and jumps back into the
// day copy's Play call); PlayMusic, with the same text, matches.
// FUNCTION: SWKOTOR 0x0068DD30
void CSWCAmbientSound::PlayTruncated(int bPlay, int nKind)
{
    PLAY_TRUNCATED_BODY(bPlay, nKind)
}

// FUNCTION: SWKOTOR 0x0068DF40
CSWCAmbientSound::CSWCAmbientSound(unsigned long oidArea)
{
    m_nTimer = 0;
    m_nMusicDelayMs = 30000;
    m_pMusicSource = new CExoStreamingSoundSource();
    m_pAmbientSource = new CExoStreamingSoundSource();
    m_pMusicSource->SetLooping(0);
    m_pAmbientSource->SetLooping(1);
    m_pAmbientSource->SetPriorityGroup(4);
    m_bMusicEnabled = 1;
    m_bAmbientEnabled = 1;
    m_bDay = 1;
    m_oidArea = oidArea;
    m_bActive = 0;
    m_nMusicMode = 0;
    m_nPlayingKind = 0;
    m_pStingerSource = 0;
    m_bAmbientFailed = 0;
}

// FUNCTION: SWKOTOR 0x0068E0D0
CSWCAmbientSound::~CSWCAmbientSound()
{
    if (m_pMusicSource) {
        delete m_pMusicSource;
        m_pMusicSource = 0;
    }
    if (m_pAmbientSource) {
        delete m_pAmbientSource;
        m_pAmbientSource = 0;
    }
}

// FUNCTION: SWKOTOR 0x0068E1B0
void CSWCAmbientSound::StopAll(int bImmediate)
{
    PlayMusic(0);
    m_bBattleMusicPlaying = 0;
    m_nTimer = 0;
    if (bImmediate) {
        m_pAmbientSource->Stop();
        m_bAmbientPlaying = 0;
        m_bAmbientFailed = 0;
    }
    else {
        PlayAmbientSound(0);
    }
}

// FUNCTION: SWKOTOR 0x0068E1F0
void CSWCAmbientSound::PlayMusic(int bPlay)
{
    if (!m_bActive)
        return;
    if (m_nMusicMode) {
        PLAY_TRUNCATED_BODY(bPlay, 1)
        return;
    }
    if (!bPlay) {
        m_pMusicSource->FadeAndStop(800);
        m_bMusicPlaying = 0;
        return;
    }
    if (!m_bMusicEnabled)
        return;
    if (!m_pMusicSource->IsPlaying()) {
        m_nTimer = 0;
        m_pMusicSource->SetPriorityGroup(2);
        if (m_bDay) {
            CResRef cResRef(m_lstMusicDay.m_sResource);
            if (!m_pMusicSource->Play(cResRef, 8, 0, 1, 0))
                m_nTimer = 10000;
        }
        else {
            CResRef cResRef(m_lstMusicNight.m_sResource);
            if (!m_pMusicSource->Play(cResRef, 8, 0, 1, 0))
                m_nTimer = 10000;
        }
    }
    m_bMusicPlaying = 1;
}

// FUNCTION: SWKOTOR 0x0068E490
void CSWCAmbientSound::SetMusicDayTrack(int nTrack)
{
    CExoString sPlaying;
    m_pMusicSource->GetFilePlaying().CopyToString(sPlaying);
    if (m_pMusicSource->IsPlaying() && sPlaying.CompareNoCase(m_lstMusicDay.m_sResource))
        PlayMusic(0);
    LoadSoundList(1, nTrack, &m_lstMusicDay);
    m_nMusicDay = nTrack;
    PlayMusic(1);
    m_nTimer = 1;
}

// FUNCTION: SWKOTOR 0x0068E550
void CSWCAmbientSound::SetMusicNightTrack(int nTrack)
{
    CExoString sPlaying;
    m_pMusicSource->GetFilePlaying().CopyToString(sPlaying);
    if (m_pMusicSource->IsPlaying() && sPlaying.CompareNoCase(m_lstMusicNight.m_sResource))
        PlayMusic(0);
    LoadSoundList(1, nTrack, &m_lstMusicNight);
    m_nMusicNight = nTrack;
    PlayMusic(1);
    m_nTimer = 1;
}

// NONMATCH (14): same day / night cross-jump direction as PlayTruncated.
// FUNCTION: SWKOTOR 0x0068E610
void CSWCAmbientSound::PlayBattleMusic(int bPlay)
{
    CResRef cUnused;
    CExoString sPlaying;
    if (m_nMusicMode) {
        PLAY_TRUNCATED_BODY(bPlay, 4)
    }
    else if (!bPlay) {
        if (m_bBattleMusicPlaying) {
            m_pMusicSource->FadeAndStop(800);
            if (m_bMusicEnabled) {
                PlayStinger(&m_lstMusicBattle);
                PlayMusic(1);
            }
            m_bBattleMusicPlaying = 0;
        }
    }
    else if (m_bMusicEnabled && m_bActive && m_lstMusicBattle.m_sResource != "") {
        m_bBattleMusicPlaying = 1;
        if (m_pMusicSource->IsPlaying()) {
            m_pMusicSource->GetFilePlaying().CopyToString(sPlaying);
            if (!sPlaying.CompareNoCase(m_lstMusicBattle.m_sResource)) {
                m_pMusicSource->FadeAndStop(800);
                m_bMusicPlaying = 0;
                m_nTimer = 10;
                ResetTimer();
            }
        }
        else {
            m_pMusicSource->SetPriorityGroup(2);
            CResRef cResRef(m_lstMusicBattle.m_sResource);
            m_nTimer = m_pMusicSource->Play(cResRef, 8, 0, 1, 0) ? 0 : 10000;
        }
    }
}

// NONMATCH (13): the original keeps bRestart in ebx (saved in the
// prologue) and moves the `bRestart = 0` arm after the epilogue; if/else,
// initialised and ?: forms all give edi with the arm inline.
// FUNCTION: SWKOTOR 0x0068E940
void CSWCAmbientSound::SetBattleMusicTrack(int nTrack)
{
    int bRestart;
    CExoString sPlaying;
    m_pMusicSource->GetFilePlaying().CopyToString(sPlaying);
    if (m_pMusicSource->IsPlaying() && sPlaying.CompareNoCase(m_lstMusicBattle.m_sResource)) {
        bRestart = 1;
        PlayBattleMusic(0);
    }
    else {
        bRestart = 0;
    }
    LoadSoundList(1, nTrack, &m_lstMusicBattle);
    m_nMusicBattle = nTrack;
    if (bRestart)
        PlayBattleMusic(1);
}

// NONMATCH (32): day / night cross-jump direction (see PlayTruncated);
// the original also hoists the volume `xor ecx, ecx` above the m_bDay test
// and loads m_pAmbientSource before the Play pushes.
// FUNCTION: SWKOTOR 0x0068EA10
void CSWCAmbientSound::PlayAmbientSound(int bPlay)
{
    if (!m_bActive)
        return;
    if (m_nMusicMode) {
        PLAY_TRUNCATED_BODY(bPlay, 2)
        return;
    }
    if (!bPlay) {
        m_pAmbientSource->FadeAndStop(6000);
        m_bAmbientPlaying = 0;
        m_bAmbientFailed = 0;
        return;
    }
    if (!m_bAmbientEnabled)
        return;
    if (!m_pAmbientSource->IsPlaying()) {
        CExoString* pResource;
        if (m_bDay) {
            m_pAmbientSource->SetVolume(m_nAmbientSndDayVol);
            m_pAmbientSource->SetPriorityGroup(4);
            pResource = &m_lstAmbientDay.m_sResource;
        }
        else {
            m_pAmbientSource->SetVolume(m_nAmbientSndNitVol);
            m_pAmbientSource->SetPriorityGroup(4);
            pResource = &m_lstAmbientNight.m_sResource;
        }
        {
            CResRef cResRef(*pResource);
            m_bAmbientFailed = m_pAmbientSource->Play(cResRef, 8, 0, 1, 0) == 0;
        }
        if (m_bAmbientFailed) {
            CResRef cResRef(*pResource);
            m_bAmbientFailed = m_pAmbientSource->Play(cResRef, 4, 0, 1, 0) == 0;
        }
    }
    m_bAmbientPlaying = 1;
}

// FUNCTION: SWKOTOR 0x0068ED00
void CSWCAmbientSound::SetAmbientDayTrack(int nTrack)
{
    CExoString sPlaying;
    m_pAmbientSource->GetFilePlaying().CopyToString(sPlaying);
    if (m_pAmbientSource->IsPlaying() && sPlaying.CompareNoCase(m_lstAmbientDay.m_sResource))
        PlayAmbientSound(0);
    LoadSoundList(0, nTrack, &m_lstAmbientDay);
    m_nAmbientSndDay = nTrack;
    PlayAmbientSound(1);
}

// FUNCTION: SWKOTOR 0x0068EDC0
void CSWCAmbientSound::SetAmbientNightTrack(int nTrack)
{
    CExoString sPlaying;
    m_pAmbientSource->GetFilePlaying().CopyToString(sPlaying);
    if (m_pAmbientSource->IsPlaying() && sPlaying.CompareNoCase(m_lstAmbientNight.m_sResource))
        PlayAmbientSound(0);
    LoadSoundList(0, nTrack, &m_lstAmbientNight);
    m_nAmbientSndNight = nTrack;
    PlayAmbientSound(1);
}

// FUNCTION: SWKOTOR 0x0068EE80
void CSWCAmbientSound::LoadFromMessage(CNWMessage* pMessage)
{
    m_bMusicPlaying = pMessage->ReadBOOL();
    m_bBattleMusicPlaying = pMessage->ReadBOOL();
    m_bAmbientPlaying = pMessage->ReadBOOL();
    m_nMusicDelayMs = pMessage->ReadDWORD(32);
    m_nMusicDelayMs = m_nMusicDelayMs > 1 ? m_nMusicDelayMs : 1;
    m_nMusicDay = pMessage->ReadDWORD(32);
    m_nMusicNight = pMessage->ReadDWORD(32);
    m_nMusicBattle = pMessage->ReadDWORD(32);
    m_nAmbientSndDay = pMessage->ReadDWORD(32);
    m_nAmbientSndNight = pMessage->ReadDWORD(32);
    m_nAmbientSndDayVol = pMessage->ReadBYTE(8);
    m_nAmbientSndNitVol = pMessage->ReadBYTE(8);
    LoadSoundList(1, m_nMusicDay, &m_lstMusicDay);
    LoadSoundList(1, m_nMusicNight, &m_lstMusicNight);
    LoadSoundList(1, m_nMusicBattle, &m_lstMusicBattle);
    LoadSoundList(0, m_nAmbientSndDay, &m_lstAmbientDay);
    LoadSoundList(0, m_nAmbientSndNight, &m_lstAmbientNight);
}

// FUNCTION: SWKOTOR 0x0068EF80
void CSWCAmbientSound::Update()
{
    if (!m_pMusicSource->IsPlaying() &&
        g_pExoSound->GetCurrentMode() != g_nSoundModeMovie &&
        !g_pExoSound->GetProperty0x144()) {
        if (m_nTimer > 0) {
            unsigned long nElapsed = GetElapsedTime();
            if (nElapsed >= m_nTimer) {
                m_nTimer = 0;
                if (!m_nMusicMode) {
                    if (m_bBattleMusicPlaying)
                        PlayBattleMusic(1);
                    else
                        PlayMusic(1);
                }
                else
                    PlayTruncated(1, 0);
            }
            else {
                m_nTimer -= nElapsed;
                ResetTimer();
            }
        }
        else if (m_bMusicPlaying || m_bBattleMusicPlaying) {
            if (!m_nMusicMode) {
                if (m_bBattleMusicPlaying)
                    m_nTimer = 1000;
                else
                    m_nTimer = m_nMusicDelayMs > 1 ? m_nMusicDelayMs : 1;
            }
            else
                m_nTimer = 10;
            ResetTimer();
        }
    }
    if (!m_nMusicMode && !m_pAmbientSource->IsPlaying() &&
        g_pExoSound->GetCurrentMode() != g_nSoundModeMovie &&
        g_pExoSound->GetCurrentMode() != g_nSoundModeMenu &&
        m_bAmbientPlaying && !m_bAmbientFailed &&
        !g_pExoSound->GetProperty0x144())
        PlayAmbientSound(1);
}
