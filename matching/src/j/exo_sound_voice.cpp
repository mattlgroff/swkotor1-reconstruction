// CExoSoundInternal helpers that bind a source to a Miles voice
// (0x005D71C0-0x005D754F).
#include "j/exo_sound_source.h"

// FUNCTION: SWKOTOR 0x005D7480
void CExoSoundInternal::Resume2DSource(CExoSoundSourceInternal* pSource, CExoSoundVoice* pVoice)
{
    int nLoopCount = pSource->m_bLooping == 0;
    pSource->m_p2DVoice = pVoice;
    pVoice->m_pOwner = pSource;
    AIL_init_sample(pVoice->m_hSample);
    AIL_set_sample_file(pVoice->m_hSample, pSource->m_pRes->GetWaveData(), 0);
    AIL_set_sample_loop_count(pVoice->m_hSample, nLoopCount);
    AIL_set_sample_position(pVoice->m_hSample, pSource->m_nRestartPosition);
    if (pSource->m_fPitchVariance != 0.0f) {
        AIL_set_sample_playback_rate(pVoice->m_hSample, pSource->m_nPitchRate);
    } else if (pSource->m_fFixedVariance != 0.0f) {
        AIL_set_sample_playback_rate(pVoice->m_hSample, pSource->m_nFixedRate);
    } else {
        AIL_set_sample_playback_rate(pVoice->m_hSample, pSource->m_nSampleRate);
    }
    pSource->SetVolume(pSource->m_nVolume, 0, GetSourceVolumeScale());
    AIL_resume_sample(pVoice->m_hSample);
}

// FUNCTION: SWKOTOR 0x005D71C0
void CExoSoundInternal::Resume3DSource(CExoSoundSourceInternal* pSource, CExoSoundVoice* pVoice)
{
    int nLoopCount = pSource->m_bLooping == 0;
    pVoice->m_pOwner = pSource;
    Vector vPos = pSource->m_vPosition;
    pSource->m_p3DVoice = pVoice;
    float fMinDistance = pSource->m_fMinDistance;
    float fMaxDistance = pSource->m_fMaxDistance;
    AIL_set_3D_sample_file(pVoice->m_hSample, pSource->m_pRes->GetWaveData());
    AIL_set_3D_sample_loop_count(pVoice->m_hSample, nLoopCount);
    pSource->SetVolume(pSource->m_nVolume, 0, 1.0f);
    AIL_set_3D_sample_distances(pVoice->m_hSample, fMaxDistance, fMinDistance);
    AIL_set_3D_sample_offset(pVoice->m_hSample, pSource->m_nRestartPosition);
    AIL_set_3D_position(pVoice->m_hSample, vPos.x, vPos.y, vPos.z * -1.0f);
    if (pSource->m_fPitchVariance != 0.0f) {
        AIL_set_3D_sample_playback_rate(pVoice->m_hSample, pSource->m_nPitchRate);
    } else if (pSource->m_fFixedVariance != 0.0f) {
        AIL_set_3D_sample_playback_rate(pVoice->m_hSample, pSource->m_nFixedRate);
    } else {
        AIL_set_3D_sample_playback_rate(pVoice->m_hSample, pSource->m_nSampleRate);
    }
    if (m_bUseEAX && AIL_3D_room_type(m_hProvider) && AIL_3D_room_type(m_hProvider) != -1) {
        AIL_set_3D_sample_effects_level(pVoice->m_hSample, GetEffectsLevel());
    } else {
        AIL_set_3D_sample_effects_level(pVoice->m_hSample, 0.0f);
    }
    Vector vSourcePos = pSource->m_vPosition;
    Vector vListener = m_vListenerPosition;
    unsigned long nStart = (unsigned long)(g_pExoBase->m_pcExoTimers->GetHighResolutionTimer() / 1000);
    float fObstruction = 0.0f;
    if (FindCachedObstruction(&vSourcePos, &vListener, &nStart, &fObstruction)) {
        pSource->m_v8C = vListener;
        pSource->m_n98 = nStart;
        pSource->m_fObstruction = fObstruction;
    } else {
        pSource->m_fObstruction = FUN_0050ccf0(vSourcePos, vListener);
        pSource->m_v8C = vListener;
        pSource->m_n98 = nStart;
        if (++m_nObstructionTests > m_nMaxObstructionTests) {
            m_nMaxObstructionTests = m_nObstructionTests;
        }
    }
    pSource->DoObstructionWork();
    m_nObstructionTime += (unsigned long)(g_pExoBase->m_pcExoTimers->GetHighResolutionTimer() / 1000) - nStart;
    AIL_resume_3D_sample(pVoice->m_hSample);
}

// FUNCTION: SWKOTOR 0x005D6EF0
int CExoSoundInternal::FindCachedObstruction(const Vector* pPos, Vector* pListener, unsigned long* pTime,
                                             float* pObstruction)
{
    Vector vPos = *pPos;
    Vector vListener = *pListener;
    unsigned long nTime = *pTime;
    CExoLinkedListPosition pos = m_lPlaying.GetHeadPos();
    while (pos) {
        CExoSoundSourceInternal* p = m_lPlaying.GetAtPos(pos);
        if (p && p->m_b3D) {
            unsigned long nCachedTime = p->m_n98;
            Vector vDiff = vPos - p->m_vPosition;
            Vector vCached = p->m_v8C;
            if (FUN_004ab100(&vDiff) < 0.5f) {
                if (FUN_004ab100(&(vListener - vCached)) < 0.5f && nCachedTime - nTime < 175) {
                    *pListener = vCached;
                    *pTime = nCachedTime;
                    *pObstruction = p->m_fObstruction;
                    return 1;
                }
            }
        }
        m_lPlaying.GetNext(pos);
    }
    pos = m_lStreaming.GetHeadPos();
    while (pos) {
        CExoStreamingSoundSourceInternal* p = m_lStreaming.GetAtPos(pos);
        if (p && p->m_b3D) {
            unsigned long nCachedTime = p->m_n7C;
            Vector vDiff = vPos - p->m_vPosition;
            Vector vCached = p->m_v70;
            if (FUN_004ab100(&vDiff) < 0.5f) {
                if (FUN_004ab100(&(vListener - vCached)) < 0.5f && nCachedTime - nTime < 175) {
                    *pListener = vCached;
                    *pTime = nCachedTime;
                    *pObstruction = *(float*)&p->m_n80;
                    return 1;
                }
            }
        }
        m_lStreaming.GetNext(pos);
    }
    return 0;
}
