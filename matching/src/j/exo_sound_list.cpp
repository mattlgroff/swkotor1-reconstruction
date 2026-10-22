// CPriorityGroup ctor, CExoLinkedList<T> out-of-line members and the
// CExoSoundInternal source-list helpers (0x005D6C10-0x005D6D2F,
// 0x005D78F0-0x005D7BC6).
#include "j/exo_sound_list.h"

// FUNCTION: SWKOTOR 0x005D6C10
CPriorityGroup::CPriorityGroup()
{
    m_n6 = 0;
    m_nVolume = 0x7F;
    m_nMaxPlayer = 0xFF;
    m_n0 = 1;
    m_fC = 10.0f;
    m_fF8 = 20.0f;
    m_n14 = 0;
    m_nPlaying = 0;
    m_n10 = 0;
}

// The members below are identical for every T; the linker folded them.
// FUNCTION: SWKOTOR 0x005D6C40 SYMBOL: ??0?$CExoLinkedList@VCExoSoundSourceInternal@@@@QAE@XZ
// FUNCTION: SWKOTOR 0x005D6CB0 SYMBOL: ?AddHead@?$CExoLinkedList@VCExoSoundSourceInternal@@@@QAEPAXPAVCExoSoundSourceInternal@@@Z
// FUNCTION: SWKOTOR 0x005D6CC0 SYMBOL: ?Contains@?$CExoLinkedList@VCExoSoundSourceInternal@@@@QAEHPAVCExoSoundSourceInternal@@@Z
// FUNCTION: SWKOTOR 0x005D6CD0 SYMBOL: ?GetHead@?$CExoLinkedList@VCExoSoundSourceInternal@@@@QAEPAVCExoSoundSourceInternal@@XZ
// FUNCTION: SWKOTOR 0x005D6CE0 SYMBOL: ?GetHeadPos@?$CExoLinkedList@VCExoSoundSourceInternal@@@@QAEPAXXZ
// FUNCTION: SWKOTOR 0x005D6CF0 SYMBOL: ?Remove@?$CExoLinkedList@VCExoSoundSourceInternal@@@@QAEPAVCExoSoundSourceInternal@@PAX@Z
// FUNCTION: SWKOTOR 0x005D6D00 SYMBOL: ?GetCount@?$CExoLinkedList@VCExoSoundSourceInternal@@@@QAEKXZ
// FUNCTION: SWKOTOR 0x005D6D10 SYMBOL: ?GetAtPos@?$CExoLinkedList@VCExoSoundSourceInternal@@@@QAEPAVCExoSoundSourceInternal@@PAX@Z
// FUNCTION: SWKOTOR 0x005D6D20 SYMBOL: ?GetNext@?$CExoLinkedList@VCExoSoundSourceInternal@@@@QAEPAVCExoSoundSourceInternal@@AAPAX@Z
template class CExoLinkedList<CExoSoundSourceInternal>;

// FUNCTION: SWKOTOR 0x005D78F0
CExoLinkedListPosition CExoSoundInternal::RemoveFromPlayingList(CExoSoundSourceInternal* pSource)
{
    CExoLinkedListPosition pos = m_lPlaying.GetHeadPos();
    CExoSoundSourceInternal* p = m_lPlaying.GetHead();
    while (pos) {
        if (p == pSource) {
            if (pos == m_posIterator) {
                m_lPlaying.GetNext(m_posIterator);
                if (!m_posIterator) {
                    m_posIterator = m_lStreaming.GetHeadPos();
                    m_nIteratorList = 2;
                    if (!m_posIterator) {
                        m_nIteratorList = 1;
                        m_posIterator = 0;
                    }
                }
            }
            break;
        }
        p = m_lPlaying.GetNext(pos);
    }
    pos = pSource->m_pos;
    m_lPlaying.GetNext(pos);
    m_lPlaying.Remove(pSource->m_pos);
    pSource->m_nList = 0;
    pSource->m_pos = 0;
    m_pPriorityGroups[pSource->m_nPriorityGroup].RemovePlaying();
    return pos;
}

// FUNCTION: SWKOTOR 0x005D79C0
CExoLinkedListPosition CExoSoundInternal::RemoveFromNonPlayingLoopingList(CExoSoundSourceInternal* pSource)
{
    CExoLinkedListPosition pos = pSource->m_pos;
    m_lNonPlayingLooping.GetNext(pos);
    m_lNonPlayingLooping.Remove(pSource->m_pos);
    pSource->m_nList = 0;
    pSource->m_pos = 0;
    return pos;
}

// FUNCTION: SWKOTOR 0x005D7A00
CExoLinkedListPosition CExoSoundInternal::RemoveFromNonPlayingStreamingLoopingList(CExoStreamingSoundSourceInternal* pSource)
{
    CExoLinkedListPosition pos = m_lNonPlayingStreamingLooping.GetHeadPos();
    CExoStreamingSoundSourceInternal* p = m_lNonPlayingStreamingLooping.GetHead();
    while (pos) {
        if (pSource == p) {
            CExoLinkedListPosition posRemove = pos;
            m_lNonPlayingStreamingLooping.GetNext(pos);
            m_lNonPlayingStreamingLooping.Remove(posRemove);
            return pos;
        }
        p = m_lNonPlayingStreamingLooping.GetNext(pos);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005D7A70
CExoLinkedListPosition CExoSoundInternal::RemoveFromDelayedOneShotList(CExoSoundSourceInternal* pSource)
{
    CExoLinkedListPosition pos = pSource->m_pos;
    m_lDelayedOneShots.GetNext(pos);
    m_lDelayedOneShots.Remove(pSource->m_pos);
    pSource->m_nList = 0;
    pSource->m_pos = 0;
    return pos;
}

// FUNCTION: SWKOTOR 0x005D7AB0
CExoLinkedListPosition CExoSoundInternal::RemoveFromStreamingList(CExoStreamingSoundSourceInternal* pSource)
{
    CExoLinkedListPosition pos = pSource->m_pos;
    if (pos) {
        if (pos == m_posIterator) {
            m_lStreaming.GetNext(m_posIterator);
            if (!m_posIterator) {
                m_posIterator = m_lPlaying.GetHeadPos();
                m_nIteratorList = 1;
                if (!m_posIterator) {
                    m_nIteratorList = 1;
                    m_posIterator = 0;
                }
            }
        }
        m_lStreaming.GetNext(pos);
        m_lStreaming.Remove(pSource->m_pos);
        pSource->m_pos = 0;
        m_pPriorityGroups[pSource->m_nPriorityGroup].RemovePlaying();
    }
    return pos;
}

// FUNCTION: SWKOTOR 0x005D7B40
void CExoSoundInternal::AddToPlayingList(CExoSoundSourceInternal* pSource)
{
    pSource->m_nList = 1;
    pSource->m_pos = m_lPlaying.AddHead(pSource);
    m_pPriorityGroups[pSource->m_nPriorityGroup].AddPlaying();
}

// FUNCTION: SWKOTOR 0x005D7B80
void CExoSoundInternal::AddToNonPlayingLoopingList(CExoSoundSourceInternal* pSource)
{
    pSource->m_nList = 2;
    pSource->m_pos = m_lNonPlayingLooping.AddHead(pSource);
}

// FUNCTION: SWKOTOR 0x005D7BA0
CExoLinkedListPosition CExoSoundInternal::AddToNonPlayingStreamingLoopingList(CExoStreamingSoundSourceInternal* pSource)
{
    return m_lNonPlayingStreamingLooping.AddHead(pSource);
}

// FUNCTION: SWKOTOR 0x005D7BB0
void CExoSoundInternal::AddToDelayedOneShotList(CExoSoundSourceInternal* pSource)
{
    pSource->m_nList = 3;
    pSource->m_pos = m_lDelayedOneShots.AddHead(pSource);
}

// FUNCTION: SWKOTOR 0x005D7FE0
void CExoSoundInternal::InterruptPlayingSound(unsigned char nGroup)
{
    CExoLinkedListPosition pos = m_lPlaying.GetHeadPos();
    CExoSoundSourceInternal* p = m_lPlaying.GetHead();
    while (pos) {
        if (p->m_nPriorityGroup == nGroup) {
            if (p->m_bDeleteWhenDone) {
                if (p->IsHardwarePlayingSoundPleaseDontUseThis()) {
                    if (!m_lPendingDelete.Contains(p)) {
                        m_lPendingDelete.AddHead(p);
                    }
                } else if (!m_lPendingDelete.Contains(p)) {
                    delete p;
                }
            } else {
                p->ShutDownSource();
                if (p->m_bLooping) {
                    AddToNonPlayingLoopingList(p);
                }
            }
            return;
        }
        p = m_lPlaying.GetNext(pos);
    }
}

// FUNCTION: SWKOTOR 0x005D80A0
void CExoSoundInternal::InterruptPlayingStream(unsigned char nGroup)
{
    CExoLinkedListPosition pos = m_lStreaming.GetHeadPos();
    CExoStreamingSoundSourceInternal* p = m_lStreaming.GetHead();
    while (pos) {
        if (p->m_nPriorityGroup == nGroup) {
            p->Stop();
            if (p->m_bLooping) {
                AddToNonPlayingStreamingLoopingList(p);
            }
            return;
        }
        p = m_lStreaming.GetNext(pos);
    }
}

// FUNCTION: SWKOTOR 0x005D8220
void CExoSoundInternal::StopAllOneShots()
{
    CExoLinkedListPosition pos = m_lPlaying.GetHeadPos();
    CExoSoundSourceInternal* p = m_lPlaying.GetHead();
    while (pos) {
        if (p) {
            if (p->m_bDeleteWhenDone) {
                pos = RemoveFromPlayingList(p);
                if (p->IsHardwarePlayingSoundPleaseDontUseThis()) {
                    if (!m_lPendingDelete.Contains(p)) {
                        m_lPendingDelete.AddHead(p);
                    }
                } else if (!m_lPendingDelete.Contains(p)) {
                    delete p;
                }
                if (!pos) {
                    return;
                }
                p = m_lPlaying.GetAtPos(pos);
            } else {
                p = m_lPlaying.GetNext(pos);
            }
        }
    }
}
// FUNCTION: SWKOTOR 0x005D82C0
int CExoSoundInternal::PauseAllSounds(int bAll)
{
    g_bSoundPaused = 1;
    CExoLinkedListPosition pos = m_lStreaming.GetHeadPos();
    CExoStreamingSoundSourceInternal* pStream = m_lStreaming.GetHead();
    for (; pos; pStream = m_lStreaming.GetNext(pos)) {
        if (!bAll) {
            if (m_aModes[m_nMode] == g_nSoundModeQuiet1) {
                if (pStream->m_nPriorityGroup == 1 || pStream->m_nPriorityGroup == 2
                    || pStream->m_nPriorityGroup == 4 || pStream->m_nPriorityGroup == 11) {
                    continue;
                }
            } else if (m_aModes[m_nMode] == g_nSoundModeQuiet3) {
                if (pStream->m_nPriorityGroup == 1 || pStream->m_nPriorityGroup == 2
                    || pStream->m_nPriorityGroup == 11) {
                    continue;
                }
            }
        }
        pStream->Pause();
    }
    pos = m_lPlaying.GetHeadPos();
    CExoSoundSourceInternal* p = m_lPlaying.GetHead();
    for (; pos; p = m_lPlaying.GetNext(pos)) {
        if (p->GetIgnorePause()) {
            continue;
        }
        if (m_aModes[m_nMode] == g_nSoundModeQuiet1) {
            if (p->m_nPriorityGroup == 1 || p->m_nPriorityGroup == 2
                || p->m_nPriorityGroup == 4 || p->m_nPriorityGroup == 11) {
                continue;
            }
        } else if (m_aModes[m_nMode] == g_nSoundModeQuiet3) {
            if (p->m_nPriorityGroup == 1 || p->m_nPriorityGroup == 2
                || p->m_nPriorityGroup == 11) {
                continue;
            }
        }
        p->Pause();
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005D83F0
int CExoSoundInternal::ResumeAllSounds()
{
    g_bSoundPaused = 0;
    CExoLinkedListPosition pos = m_lStreaming.GetHeadPos();
    CExoStreamingSoundSourceInternal* pStream = m_lStreaming.GetHead();
    while (pos) {
        pStream->Resume();
        pStream = m_lStreaming.GetNext(pos);
    }
    pos = m_lPlaying.GetHeadPos();
    CExoSoundSourceInternal* p = m_lPlaying.GetHead();
    while (pos) {
        p->Resume();
        p = m_lPlaying.GetNext(pos);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005D8470
int CExoSoundInternal::AddToStreamingList(CExoStreamingSoundSourceInternal* pSource)
{
    if (m_lStreaming.GetCount() >= (unsigned long)(m_nNumber2DVoicesX2 - m_nNumber2DVoicesX2 / 2)) {
        CPriorityGroup* pGroup = &m_pPriorityGroups[pSource->m_nPriorityGroup];
        CExoStreamingSoundSourceInternal* pVictim = 0;
        unsigned char nBest = 0;
        if (pGroup) {
        CExoLinkedListPosition pos = m_lStreaming.GetHeadPos();
        CExoStreamingSoundSourceInternal* p = m_lStreaming.GetHead();
        while (pos) {
            if (!p->m_bLooping) {
                p = m_lStreaming.GetNext(pos);
            } else {
                CPriorityGroup* pOther = &m_pPriorityGroups[p->m_nPriorityGroup];
                if (pOther && pOther->m_n6 > pGroup->m_n6 && pOther->m_n6 > nBest) {
                    pVictim = p;
                    nBest = pOther->m_n6;
                }
                p = m_lStreaming.GetNext(pos);
            }
        }
        }
        if (pVictim) {
            pVictim->Stop();
            if (pVictim->m_bLooping) {
                AddToNonPlayingStreamingLoopingList(pVictim);
            }
        } else {
            return 0;
        }
    }
    pSource->m_pos = m_lStreaming.AddHead(pSource);
    m_pPriorityGroups[pSource->m_nPriorityGroup].AddPlaying();
    return 1;
}

// FUNCTION: SWKOTOR 0x005D8560
void CExoSoundInternal::SetSoundMode(int nMode)
{
    if (nMode != g_nSoundModePauseAll) {
        if (nMode == g_nSoundModeResumeAll) {
            m_bModePaused = 0;
            ResumeAllSounds();
            nMode = m_aModes[m_nMode];
            if (m_nMode) {
                m_nMode--;
                SetSoundMode(nMode);
            }
        } else {
            if (nMode != g_nSoundModeBack && nMode == m_aModes[m_nMode]) {
                return;
            }
            if (m_aModes[m_nMode] == g_nSoundModeMuted) {
                UnMuteSound();
            }
            if (m_aModes[m_nMode] == g_nSoundModeQuiet1 || m_aModes[m_nMode] == g_nSoundModeQuiet2
                || m_aModes[m_nMode] == g_nSoundModeQuiet3) {
                ResumeAllSounds();
            }
            if (nMode == g_nSoundModeBack) {
                if (--m_nMode < 0) {
                    m_nMode = 0;
                }
                nMode = m_aModes[m_nMode];
            } else {
                if (++m_nMode > 9) {
                    m_nMode = 9;
                }
                m_aModes[m_nMode] = nMode;
            }
            if (nMode == g_nSoundModeMuted) {
                MuteSound();
            }
            if (nMode == g_nSoundModeQuiet1 || nMode == g_nSoundModeQuiet2 || nMode == g_nSoundModeQuiet3) {
                PauseAllSounds(0);
            }
        }
    } else {
        PauseAllSounds(1);
        m_bModePaused = 1;
    }
}


// FUNCTION: SWKOTOR 0x005D8EB0
CExoSoundVoice* CExoSoundInternal::GetFree3DVoice(unsigned char nGroup)
{
    CPriorityGroup* pGroup = &m_pPriorityGroups[nGroup];
    unsigned char nBest = 0;
    int i = 0;
    int nVictim;
    if (pGroup) {
        CExoSoundVoice* pVoices = m_p3DVoices;
        CExoSoundVoice* pVoice = pVoices;
        for (; i < m_nNumber3DVoices; i++, pVoice++) {
            if (!pVoice->m_bReserved) {
                if (!pVoice->m_pSource) {
                    return &pVoices[i];
                }
                CPriorityGroup* pOther = &m_pPriorityGroups[pVoice->m_pSource->m_nPriorityGroup];
                if (pOther && pOther->m_n6 > pGroup->m_n6 && pOther->m_n6 > nBest) {
                    nVictim = i;
                    nBest = pOther->m_n6;
                }
            }
        }
        if (nBest > 0) {
            CExoSoundSourceInternal* p = pVoices[nVictim].m_pSource;
            if (p->m_bDeleteWhenDone) {
                if (p->IsHardwarePlayingSoundPleaseDontUseThis()) {
                    if (!m_lPendingDelete.Contains(p)) {
                        m_lPendingDelete.AddHead(p);
                    }
                } else if (!m_lPendingDelete.Contains(p)) {
                    delete p;
                }
            } else {
                p->ShutDownSource();
                if (p->m_bLooping) {
                    AddToNonPlayingLoopingList(p);
                }
            }
            return &m_p3DVoices[nVictim];
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005D8FF0
CExoSoundVoice* CExoSoundInternal::GetFree2DVoice(unsigned char nGroup)
{
    CPriorityGroup* pGroup = &m_pPriorityGroups[nGroup];
    unsigned char nBest = 0;
    int i = 0;
    int nVictim;
    if (pGroup) {
        CExoSoundVoice* pVoices = m_p2DVoices;
        CExoSoundVoice* pVoice = pVoices;
        for (; i < m_nNumber2DVoicesX2 / 2; i++, pVoice++) {
            if (!pVoice->m_bReserved) {
                if (!pVoice->m_pSource) {
                    return &pVoices[i];
                }
                CPriorityGroup* pOther = &m_pPriorityGroups[pVoice->m_pSource->m_nPriorityGroup];
                if (pOther && pOther->m_n6 > pGroup->m_n6 && pOther->m_n6 > nBest) {
                    nVictim = i;
                    nBest = pOther->m_n6;
                }
            }
        }
        if (nBest > 0) {
            CExoSoundSourceInternal* p = pVoices[nVictim].m_pSource;
            if (p->m_bDeleteWhenDone) {
                if (p->IsHardwarePlayingSoundPleaseDontUseThis()) {
                    if (!m_lPendingDelete.Contains(p)) {
                        m_lPendingDelete.AddHead(p);
                    }
                } else if (!m_lPendingDelete.Contains(p)) {
                    delete p;
                }
            } else {
                p->ShutDownSource();
                if (p->m_bLooping) {
                    AddToNonPlayingLoopingList(p);
                }
            }
            return &m_p2DVoices[nVictim];
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005D8110
void CExoSoundInternal::ApplyVolumes(int nMs)
{
    CExoLinkedListPosition pos = m_lStreaming.GetHeadPos();
    CExoStreamingSoundSourceInternal* pStream = m_lStreaming.GetHead();
    while (pos) {
        if (pStream) {
            pStream->SetVolume(pStream->m_nVolume, nMs, 1.0f);
        }
        pStream = m_lStreaming.GetNext(pos);
    }
    pos = m_lPlaying.GetHeadPos();
    CExoSoundSourceInternal* p = m_lPlaying.GetHead();
    while (pos) {
        if (p) {
            p->SetVolume(p->m_nVolume, nMs, 1.0f);
            if (p->m_n1C) {
                if (m_bUseEAX && AIL_3D_room_type(m_hProvider) && AIL_3D_room_type(m_hProvider) != -1) {
                    AIL_set_3D_sample_effects_level(p->m_pVoice->m_hSample, GetEffectsLevel());
                } else {
                    AIL_set_3D_sample_effects_level(p->m_pVoice->m_hSample, 0.0f);
                }
            }
        }
        p = m_lPlaying.GetNext(pos);
    }
}
