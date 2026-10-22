// CExoSoundSourceInternal / CExoStreamingSoundSourceInternal
// (0x005DAE50-0x005DCE22): Miles voice handling behind the public sound
// source facades in exo_sound.cpp.
#include "j/exo_sound_source.h"

extern "C" void __cdecl free(void* p);
extern "C" int __cdecl rand();
extern "C" double __cdecl pow(double x, double y);
#pragma intrinsic(pow)

// FUNCTION: SWKOTOR 0x005DAE50
int CExoSoundSourceInternal::Demand()
{
    if (!m_pRes) {
        return 0;
    }
    if (m_bDemanded && m_pRes->IsLoaded()) {
        return 1;
    }
    m_bDemanded = m_pRes->Demand() != 0;
    return m_bDemanded;
}

// FUNCTION: SWKOTOR 0x005DAE90
int CExoSoundSourceInternal::Release()
{
    if (m_bDemanded) {
        m_bDemanded = 0;
        return m_pRes->Release() != 0;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005DAEB0
void CExoSoundSourceInternal::DoObstructionWork()
{
    if (m_p3DVoice) {
        AIL_set_3D_sample_obstruction(m_p3DVoice->m_hSample, m_fObstruction);
    }
}

// FUNCTION: SWKOTOR 0x005DAF50
CExoSoundSourceInternal::CExoSoundSourceInternal(CExoSoundInternal* pSound)
{
    m_pSound = pSound;
    m_pos = 0;
    m_b3D = 0;
    m_bLooping = 0;
    m_nVolume = g_nDefaultSourceVolume;
    m_n28 = 0;
    m_p3DVoice = 0;
    m_p2DVoice = 0;
    m_nRestartPosition = 0;
    m_nList = 0;
    m_fMinDistance = 10.0f;
    m_fMaxDistance = 20.0f;
    m_nPriorityGroup = 23;
    m_nGroupVolume = pSound->m_pPriorityGroups[23].m_nVolume;
    m_fPitchVariance = 0.0f;
    m_fFixedVariance = 0.0f;
    m_bDemanded = 0;
    m_nOneShotDelay = 0;
    m_nDelayStart = 0;
    m_n58 = 0;
    m_nId = pSound->GetNextSourceId();
    m_v8C = Vector(0.0f, 0.0f, 0.0f);
    m_n98 = 0;
    m_fObstruction = 0.0f;
}

// FUNCTION: SWKOTOR 0x005DB050
int CExoSoundSourceInternal::IsInAudibleRange()
{
    if (!m_b3D) {
        return 1;
    }
    Vector vDelta = m_pSound->m_vListenerPosition - m_vPosition;
    if (FUN_004ab100(&vDelta) <= m_fMaxDistance) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005DB0E0
int CExoSoundSourceInternal::IsHardwarePlayingSoundPleaseDontUseThis()
{
    if (m_pSound->m_hDriver) {
        if (m_p3DVoice && m_p3DVoice->m_hSample) {
            return AIL_3D_sample_status(m_p3DVoice->m_hSample) == 4;
        }
        if (m_p2DVoice && m_p2DVoice->m_hSample) {
            return AIL_sample_status(m_p2DVoice->m_hSample) == 4;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005DB130
void CExoSoundSourceInternal::SetLooping(int bLooping)
{
    if (!m_nList) {
        m_bLooping = bLooping;
    }
}

// FUNCTION: SWKOTOR 0x005DB150
void CExoSoundSourceInternal::Pause()
{
    if (m_pSound->m_hDriver && IsHardwarePlayingSoundPleaseDontUseThis()) {
        if (m_p3DVoice && m_p3DVoice->m_hSample) {
            m_nRestartPosition = AIL_3D_sample_offset(m_p3DVoice->m_hSample);
            AIL_stop_3D_sample(m_p3DVoice->m_hSample);
        }
        else if (m_p2DVoice && m_p2DVoice->m_hSample) {
            m_nRestartPosition = AIL_sample_position(m_p2DVoice->m_hSample);
            AIL_stop_sample(m_p2DVoice->m_hSample);
        }
    }
}

// FUNCTION: SWKOTOR 0x005DB1C0
void CExoSoundSourceInternal::Resume()
{
    if (m_pSound->m_hDriver) {
        if (m_p3DVoice && m_p3DVoice->m_hSample) {
            m_pSound->Resume3DSource(this, m_p3DVoice);
        }
        else if (m_p2DVoice && m_p2DVoice->m_hSample) {
            m_pSound->Resume2DSource(this, m_p2DVoice);
        }
        m_nRestartPosition = 0;
    }
}

// FUNCTION: SWKOTOR 0x005DB2B0
void CExoSoundSourceInternal::Stop()
{
    ShutDownSource();
}

// FUNCTION: SWKOTOR 0x005DB210
void CExoSoundSourceInternal::ShutDownSource()
{
    if (m_nList == 1) {
        m_pSound->RemoveFromPlayingList(this);
    }
    else if (m_nList == 2) {
        m_pSound->RemoveFromNonPlayingLoopingList(this);
    }
    else if (m_nList == 3) {
        m_pSound->RemoveFromDelayedOneShotList(this);
    }
    if (m_pSound->m_hDriver) {
        if (m_p3DVoice && m_p3DVoice->m_hSample) {
            AIL_end_3D_sample(m_p3DVoice->m_hSample);
            m_p3DVoice->m_pOwner = 0;
            m_p3DVoice = 0;
        }
        else if (m_p2DVoice && m_p2DVoice->m_hSample) {
            AIL_end_sample(m_p2DVoice->m_hSample);
            m_p2DVoice->m_pOwner = 0;
            m_p2DVoice = 0;
        }
    }
    Release();
    m_nList = 0;
    m_pos = 0;
}

// FUNCTION: SWKOTOR 0x005DB2C0
void CExoSoundSourceInternal::SetPosition(const Vector& vPos)
{
    m_vPosition = vPos;
    if (m_b3D && IsHardwarePlayingSoundPleaseDontUseThis()) {
        AIL_set_3D_position(m_p3DVoice->m_hSample, vPos.x, vPos.y, vPos.z * -1.0f);
    }
}

// FUNCTION: SWKOTOR 0x005DB320
void CExoSoundSourceInternal::Set3D(int b3D)
{
    if (m_nList != 1 && !m_p3DVoice && !m_p2DVoice) {
        m_b3D = b3D;
    }
}

// FUNCTION: SWKOTOR 0x005DB340
void CExoSoundSourceInternal::SetOneShotDelay(unsigned long nDelay)
{
    if (m_pSound->m_hDriver) {
        m_nOneShotDelay = nDelay;
        m_nDelayStart = g_pExoBase->m_pcExoTimers->GetLowResolutionTimer();
        m_pSound->AddToDelayedOneShotList(this);
    }
}

// FUNCTION: SWKOTOR 0x005DB380
void CExoSoundSourceInternal::StoreRestartSoundPosition()
{
    if (m_pSound->m_hDriver) {
        if (m_b3D) {
            m_nRestartPosition = AIL_3D_sample_offset(m_p3DVoice->m_hSample);
        }
        else {
            m_nRestartPosition = AIL_sample_position(m_p2DVoice->m_hSample);
        }
    }
    else {
        m_nRestartPosition = 0;
    }
}

// FUNCTION: SWKOTOR 0x005DB3D0
void CExoSoundSourceInternal::ApplyPitchVariance()
{
    if (rand() % 2) {
        unsigned long nRange = (unsigned long)(m_fPitchVariance * m_nSampleRate);
        if (nRange) {
            m_nPitchRate += rand() % nRange;
        }
    }
    else {
        unsigned long nRange = (unsigned long)(m_fPitchVariance * m_nSampleRate * 0.5f);
        if (nRange) {
            m_nPitchRate -= rand() % nRange;
        }
    }
}

// FUNCTION: SWKOTOR 0x005DB450
void CExoSoundSourceInternal::ApplyFixedVariance()
{
    m_nFixedRate = (unsigned long)((double)m_fFixedVariance * m_nSampleRate);
}

// FUNCTION: SWKOTOR 0x005DB480
CExoSoundSourceInternal::~CExoSoundSourceInternal()
{
    ShutDownSource();
}

// FUNCTION: SWKOTOR 0x005DB800
void CExoSoundSourceInternal::SetVolume(unsigned char nVolume, int nMs, float fScale)
{
    int bCurrent = nMs;
    int nType = m_pSound->GetSourceVolumeType(this);
    if (!nMs) {
        m_nVolume = nVolume;
    }
    if (!m_pSound->IsSourceMuted(this) && !nMs) {
        float fCurrent = m_pSound->GetVolume(nType, 1);
        bCurrent = m_pSound->GetVolume(nType, 0) != fCurrent ? 1 : 0;
    }
    float fTypeVolume = m_pSound->GetVolume(nType, bCurrent);
    float fGroupScale;
    if (m_nPriorityGroup == 4 || (m_nPriorityGroup == 21 && m_p2DVoice)) {
        fGroupScale = fScale;
    } else {
        fGroupScale = 1.0f;
    }
    int nFinal = (int)((float)m_nGroupVolume * nVolume * fGroupScale * 0.007874016f);
    if (!m_pSound->m_hDriver) {
        return;
    }
    if (m_b3D) {
        if (m_p3DVoice && m_p3DVoice->m_hSample) {
            if (m_pSound->m_fScale60 < 1.0f) {
                fTypeVolume *= m_pSound->m_fScale60;
            }
            AIL_set_3D_sample_volume(m_p3DVoice->m_hSample,
                                     (float)pow(nFinal * fTypeVolume * 0.007874016f, 0.6f));
        }
    } else {
        if (m_p2DVoice && m_p2DVoice->m_hSample) {
            if (m_pSound->m_fScale60 > 1.0f) {
                fTypeVolume *= 2.0f - m_pSound->m_fScale60;
            }
            float fLevel = nFinal * fTypeVolume * 0.007874016f;
            AIL_set_sample_volume_levels(m_p2DVoice->m_hSample, fLevel, fLevel);
        }
    }
}

// FUNCTION: SWKOTOR 0x005DB980
int CExoSoundSourceInternal::SetFixedVariance(float fVariance)
{
    if (fVariance <= 2.0f && fVariance >= 0.5f) {
        m_fFixedVariance = fVariance;
        m_fPitchVariance = 0.0f;
        if (m_pSound->m_hDriver) {
            Pause();
            if (m_p3DVoice && m_p3DVoice->m_hSample) {
                ApplyFixedVariance();
                AIL_set_3D_sample_playback_rate(m_p3DVoice->m_hSample, m_nFixedRate);
                Resume();
                return 1;
            }
            if (m_p2DVoice && m_p2DVoice->m_hSample) {
                ApplyFixedVariance();
                AIL_set_sample_playback_rate(m_p2DVoice->m_hSample, m_nFixedRate);
            }
            Resume();
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005DBA40
void CExoSoundSourceInternal::SetPitchVariance(float fVariance)
{
    m_fPitchVariance = 0.0f <= fVariance ? (1.0f < fVariance ? 1.0f : fVariance) : 0.0f;
    m_fFixedVariance = 0.0f;
    if (m_pSound->m_hDriver) {
        Pause();
        if (m_p3DVoice && m_p3DVoice->m_hSample) {
            ApplyPitchVariance();
            AIL_set_3D_sample_playback_rate(m_p3DVoice->m_hSample, m_nPitchRate);
            Resume();
            return;
        }
        if (m_p2DVoice && m_p2DVoice->m_hSample) {
            ApplyPitchVariance();
            AIL_set_sample_playback_rate(m_p2DVoice->m_hSample, m_nPitchRate);
        }
        Resume();
    }
}

// FUNCTION: SWKOTOR 0x005DBB00 SYMBOL: ??_GCExoSoundSourceInternal@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x005DBB20
void CExoSoundSourceInternal::SetPriorityGroup(unsigned char nGroup)
{
    if (m_pSound->m_hDriver) {
        m_nPriorityGroup = nGroup;
        CPriorityGroup* pGroup = &m_pSound->m_pPriorityGroups[nGroup];
        m_nGroupVolume = pGroup->m_nVolume;
        SetVolume(m_nVolume, 0, 1.0f);
        SetPitchVariance(pGroup->GetPitchVariance());
        SetDistances(pGroup->GetMinDistance(), pGroup->GetMaxDistance());
    }
}

// FUNCTION: SWKOTOR 0x005DBB90
void CExoStreamingSoundSourceInternal::SetReverbEnabled(int bEnabled)
{
    if (m_bReverbEnabled != bEnabled) {
        m_bReverbEnabled = bEnabled;
        m_bReverbChanged = 1;
    }
}

// FUNCTION: SWKOTOR 0x005DBBB0
Vector CExoSoundInternal::GetListenerVector80()
{
    return m_vListener80;
}

// FUNCTION: SWKOTOR 0x005DBBE0
CExoStreamingSoundSourceInternal::CExoStreamingSoundSourceInternal(CExoSoundInternal* pSound)
{
    m_pSound = pSound;
    m_bLooping = 0;
    m_hStream = 0;
    m_nPausePosition = 0;
    m_nFadeTime = 0;
    m_nFadeStart = 0;
    m_bInStreamingList = 0;
    m_pBuffer = 0;
    m_nVolume = g_nDefaultSourceVolume;
    m_nId = pSound->GetNextSourceId();
    m_bReverbEnabled = 1;
    m_bReverbChanged = 1;
    m_b3D = 0;
    m_fMinDistance = 0.0f;
    m_fMaxDistance = 0.0f;
    m_f54 = -1.0f;
    m_nPriorityGroup = 2;
    m_nGroupVolume = m_pSound->m_pPriorityGroups[2].m_nVolume;
    m_v70 = Vector(0.0f, 0.0f, 0.0f);
    m_n7C = 0;
    m_n80 = 0;
}

// FUNCTION: SWKOTOR 0x005DBCA0
CExoStreamingSoundSourceInternal::~CExoStreamingSoundSourceInternal()
{
    if (m_bInStreamingList) {
        m_pSound->RemoveFromStreamingList(this);
    }
    else {
        m_pSound->RemoveFromNonPlayingStreamingLoopingList(this);
    }
    if (m_pSound->m_hDriver && m_hStream) {
        AIL_close_stream(m_hStream);
        if (m_pBuffer) {
            free(m_pBuffer);
        }
        m_hStream = 0;
    }
}

// FUNCTION: SWKOTOR 0x005DC300
int CExoStreamingSoundSourceInternal::IsPlaying()
{
    if (m_pSound->m_hDriver && m_hStream) {
        return AIL_stream_status(m_hStream) == 4;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005DC330
void CExoStreamingSoundSourceInternal::SetLooping(int bLooping)
{
    m_bLooping = bLooping;
    if (IsPlaying()) {
        AIL_set_stream_loop_count(m_hStream, m_bLooping == 0);
    }
}

// FUNCTION: SWKOTOR 0x005DC380
void CExoStreamingSoundSourceInternal::Stop()
{
    if (m_hStream) {
        AIL_pause_stream(m_hStream, 1);
    }
    m_nFadeTime = 0;
    m_nFadeStart = 0;
    m_nPausePosition = 0;
    if (m_bInStreamingList) {
        m_pSound->RemoveFromStreamingList(this);
    }
    else {
        m_pSound->RemoveFromNonPlayingStreamingLoopingList(this);
    }
}

// FUNCTION: SWKOTOR 0x005DC3C0
void CExoStreamingSoundSourceInternal::Pause()
{
    CExoString sFile;
    m_cFilePlaying.CopyToString(sFile);
    if (!sFile.CompareNoCase("evil_ending")
        || g_pAppManager->m_pClientExoApp->GetCreditSequenceInProgress()) {
        if (m_pSound->m_hDriver && m_hStream && m_pSound->m_hDriver
            && AIL_stream_status(m_hStream) == 4) {
            AIL_pause_stream(m_hStream, 1);
            m_nPausePosition = AIL_stream_position(m_hStream);
        }
    }
}

// FUNCTION: SWKOTOR 0x005DC480
void CExoStreamingSoundSourceInternal::FadeAndStop(unsigned long nMs)
{
    if (IsPlaying()) {
        m_nFadeStart = g_pExoBase->m_pcExoTimers->GetLowResolutionTimer();
        m_nFadeTime = nMs;
    }
}

// FUNCTION: SWKOTOR 0x005DC4D0
int CExoStreamingSoundSourceInternal::IsInAudibleRange()
{
    if (!m_b3D) {
        return 1;
    }
    Vector vDelta = m_pSound->m_vListenerPosition - m_vPosition;
    if (FUN_004ab100(&vDelta) <= m_fMaxDistance) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005DC930
void CExoStreamingSoundSourceInternal::SetVolume(unsigned char nVolume, int nMs, float fScale)
{
    int bCurrent = nMs;
    int nType = m_pSound->GetStreamVolumeType(this);
    float fGroupScale;
    if (m_nPriorityGroup == 4 || m_nPriorityGroup == 21) {
        fGroupScale = fScale;
    } else {
        fGroupScale = 1.0f;
    }
    int nFinal = (int)(nVolume * fGroupScale);
    if (!nMs) {
        m_nVolume = nVolume;
    }
    if (m_pSound->m_hDriver && m_hStream) {
        if (!m_pSound->IsStreamMuted(this) && !nMs) {
            float fCurrent = m_pSound->GetVolume(nType, 1);
            bCurrent = m_pSound->GetVolume(nType, 0) != fCurrent ? 1 : 0;
        }
        float fTypeVolume = m_pSound->GetVolume(nType, bCurrent);
        float fLevel = (float)nFinal * (m_nGroupVolume * fTypeVolume * 0.007874016f) * 0.007874016f;
        AIL_set_stream_volume_levels(m_hStream, fLevel, fLevel);
        m_f54 = fLevel;
        ApplyPositionalVolume();
    }
}

// FUNCTION: SWKOTOR 0x005DCD50
void CExoStreamingSoundSourceInternal::SetPriorityGroup(unsigned char nGroup)
{
    CPriorityGroup* pGroup = &m_pSound->m_pPriorityGroups[nGroup];
    if (pGroup) {
        m_nPriorityGroup = nGroup;
    }
    else {
        pGroup = &m_pSound->m_pPriorityGroups[m_nPriorityGroup];
    }
    m_nGroupVolume = pGroup->m_nVolume;
    SetVolume(m_nVolume, 0, 1.0f);
    SetDistances(pGroup->GetMinDistance(), pGroup->GetMaxDistance());
}

// FUNCTION: SWKOTOR 0x005DCDB0
void CExoStreamingSoundSourceInternal::Resume()
{
    if (m_pSound->m_hDriver && m_hStream && !IsPlaying()) {
        SetVolume(m_nVolume, 0, 1.0f);
        AIL_set_stream_loop_count(m_hStream, !m_bLooping);
        AIL_set_stream_position(m_hStream, m_nPausePosition);
        AIL_pause_stream(m_hStream, 0);
        m_nPausePosition = 0;
    }
}
