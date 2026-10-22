// CExoSoundSourceInternal::Play (0x005DB4D0). Own TU so the small
// source helpers defined in exo_sound_source.cpp stay out-of-line calls.
#include "j/exo_sound_source.h"

extern int g_bSoundPaused;                          // 0x00832890

// Compiled-out trace: the arguments are still evaluated right to left.
inline void SoundTrace(int, const char*) {}

inline void InterruptGroup(CExoSoundInternal* pSound, unsigned char nGroup)
{
    pSound->InterruptPlayingSound(nGroup);
}

inline CExoSoundVoice* GetFree3DVoice(CExoSoundInternal* pSound, unsigned char nGroup)
{
    return pSound->GetFree3DVoice(nGroup);
}

inline CExoSoundVoice* GetFree2DVoice(CExoSoundInternal* pSound, unsigned char nGroup)
{
    return pSound->GetFree2DVoice(nGroup);
}

#define PLAY_FAILED()                                                          \
    {                                                                          \
        if (m_n28) {                                                           \
            m_bPlayFailed = 1;                                                 \
        }                                                                      \
        return m_n58;                                                          \
    }

#define PLAY_NOT_PLAYING()                                                     \
    {                                                                          \
        if (m_bLooping) {                                                      \
            if (!m_nList) {                                                    \
                m_pSound->AddToNonPlayingLoopingList(this);                    \
            }                                                                  \
            return m_n58;                                                      \
        }                                                                      \
        PLAY_FAILED()                                                          \
    }

// FUNCTION: SWKOTOR 0x005DB4D0
int CExoSoundSourceInternal::Play(int bNoInterrupt, int bRestart)
{
    CExoString sName;
    m_bPlayFailed = 0;
    m_nOneShotDelay = 0;
    m_nDelayStart = 0;
    if (!m_pSound->m_hDriver) {
        PLAY_FAILED()
    }
    long nLoopCount = m_bLooping == 0;
    if (!bRestart) {
        m_nRestartPosition = 0;
    }
    if (m_p3DVoice && m_p3DVoice->m_hSample) {
        AIL_stop_3D_sample(m_p3DVoice->m_hSample);
        AIL_set_3D_sample_loop_count(m_p3DVoice->m_hSample, nLoopCount);
        AIL_set_3D_sample_offset(m_p3DVoice->m_hSample, 0);
        AIL_resume_3D_sample(m_p3DVoice->m_hSample);
        return m_n58;
    }
    if (m_p2DVoice && m_p2DVoice->m_hSample) {
        AIL_stop_sample(m_p2DVoice->m_hSample);
        AIL_set_sample_loop_count(m_p2DVoice->m_hSample, nLoopCount);
        AIL_set_sample_position(m_p2DVoice->m_hSample, 0);
        AIL_resume_sample(m_p2DVoice->m_hSample);
        return m_n58;
    }
    if (!m_n58 && !m_bDemanded) {
        if (!Demand()) {
            PLAY_FAILED()
        }
        if (m_pRes) {
            SoundTrace(m_pRes->IsLoaded(), sName.CStr());
        }
        AILSOUNDINFO info;
        AIL_WAV_info(m_pRes->GetWaveData(), &info);
        m_n58 = (unsigned long)(info.data_len / info.channels * (8.0 / (info.rate * info.bits)) * 1000.0);
        if (m_b3D && info.channels != 1) {
            Release();
            PLAY_FAILED()
        }
        m_nSampleRate = info.rate;
        m_nPitchRate = info.rate;
        if (m_fPitchVariance != 0.0f) {
            ApplyPitchVariance();
        }
        if (m_fFixedVariance != 0.0f) {
            ApplyFixedVariance();
        }
    }
    if (!IsInAudibleRange()) {
        if (m_bDemanded) {
            m_bDemanded = 0;
            m_pRes->Release();
        }
        PLAY_NOT_PLAYING()
    }
    CPriorityGroup* pGroup = &m_pSound->m_pPriorityGroups[m_nPriorityGroup];
    if (pGroup->m_nPlaying >= pGroup->m_nMaxPlayer) {
        if (bNoInterrupt || !pGroup->m_n0) {
            Release();
            PLAY_NOT_PLAYING()
        }
        InterruptGroup(m_pSound, m_nPriorityGroup);
    }
    CExoSoundVoice* pVoice;
    if (m_b3D) {
        pVoice = GetFree3DVoice(m_pSound, m_nPriorityGroup);
        if (!pVoice) {
            Release();
            PLAY_NOT_PLAYING()
        }
        if (!m_bDemanded && !Demand()) {
            PLAY_FAILED()
        }
        m_pSound->Resume3DSource(this, pVoice);
    } else {
        pVoice = GetFree2DVoice(m_pSound, m_nPriorityGroup);
        if (!pVoice) {
            Release();
            PLAY_NOT_PLAYING()
        }
        if (!m_bDemanded && !Demand()) {
            PLAY_FAILED()
        }
        m_pSound->Resume2DSource(this, pVoice);
    }
    if (m_nList == 2) {
        m_pSound->RemoveFromNonPlayingLoopingList(this);
    }
    m_pSound->AddToPlayingList(this);
    if (m_pSound->m_bModePaused || (g_bSoundPaused && m_pSound->IsSourceAudible(this))) {
        Pause();
    }
    return m_n58;
}

// FUNCTION: SWKOTOR 0x005DCA30
unsigned long CExoStreamingSoundSourceInternal::Play(const CResRef& cResRef, unsigned short nResType,
                                                     int bVoice, int bMusic, int nUnused)
{
    unsigned long nTotalMs;
    if (m_hStream && !(cResRef != m_cFilePlaying)) {
        Stop();
    } else {
        CExoString sName;
        cResRef.CopyToString(sName);
        SoundTrace(0, sName.CStr());
        m_cFilePlaying = cResRef;
        m_nResType = nResType;
        cResRef.CopyToString(m_sFile);
        if (bMusic) {
            m_sFile.Format("HD0:STREAMMUSIC\\%s", m_sFile.CStr());
        } else if (bVoice) {
            if ((m_sFile[0] == 'n' || m_sFile[0] == 'N') && m_sFile.GetLength() == 16 && m_sFile[1] != '_') {
                char szModule[8];
                char szSpeaker[7];
                int i;
                for (i = 0; i < 5; i++) {
                    szModule[i] = m_sFile[i + 1];
                }
                szModule[i] = 0;
                for (i = 0; i < 6; i++) {
                    szSpeaker[i] = m_sFile[i + 6];
                }
                szSpeaker[i] = 0;
                m_sFile.Format("HD0:STREAMWAVES\\%s\\%s\\%s", szModule, szSpeaker, m_sFile.CStr());
            } else {
                m_sFile.Format("HD0:STREAMWAVES\\%s", m_sFile.CStr());
            }
        } else {
            m_sFile.Format("HD0:STREAMSOUNDS\\%s", m_sFile.CStr());
        }
        m_sFile = g_pExoBase->m_pcExoAliasList->ResolveFileName(m_sFile, nResType);
        if (m_hStream) {
            AIL_pause_stream(m_hStream, 1);
        }
        m_nFadeTime = 0;
        m_nFadeStart = 0;
        m_nPausePosition = 0;
        if (m_bInStreamingList) {
            m_pSound->RemoveFromStreamingList(this);
        } else {
            m_pSound->RemoveFromNonPlayingStreamingLoopingList(this);
        }
        if (m_hStream) {
            AIL_close_stream(m_hStream);
            m_hStream = 0;
        }
        OpenStream();
    }
    if (!m_pSound->m_hDriver || !m_hStream) {
        return 0;
    }
    SetVolume(m_nVolume, 1, m_pSound->GetSourceVolumeScale());
    int nLoopCount = m_bLooping == 0;
    ApplyPositionalVolume();
    AIL_set_stream_loop_count(m_hStream, nLoopCount);
    m_nPausePosition = 0;
    AIL_stream_ms_position(m_hStream, (long*)&nTotalMs, 0);
    CPriorityGroup* pGroup = &m_pSound->m_pPriorityGroups[m_nPriorityGroup];
    if (pGroup->m_nPlaying >= pGroup->m_nMaxPlayer) {
        if (!pGroup->m_n0) {
            if (!m_bLooping) {
                return 0;
            }
            m_pSound->AddToNonPlayingStreamingLoopingList(this);
            return nTotalMs;
        }
        m_pSound->InterruptPlayingStream(m_nPriorityGroup);
    }
    if (m_bLooping && !IsInAudibleRange()) {
        m_pSound->AddToNonPlayingStreamingLoopingList(this);
        return nTotalMs;
    }
    if (!m_pSound->AddToStreamingList(this)) {
        return 0;
    }
    AIL_start_stream(m_hStream);
    if (m_pSound->m_bModePaused || (g_bSoundPaused && m_pSound->IsStreamAudible(this))) {
        Pause();
    }
    return nTotalMs;
}
