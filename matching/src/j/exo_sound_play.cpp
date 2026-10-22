// CExoSoundInternal one-shot playback (0x005D7550-0x005D78EC).
#include "j/exo_sound_source.h"

// FUNCTION: SWKOTOR 0x005D7550
int CExoSoundInternal::PlayOneShotSound(const CResRef& cResRef, unsigned char nGroup, unsigned long nDelay,
                                        unsigned char nVolume, float fFixedVariance, float fPitchVariance)
{
    int nResult = 0;
    if (cResRef == "") {
        return 0;
    }
    CExoSoundSourceInternal* pSource = new CExoSoundSourceInternal(this);
    pSource->SetResRef(cResRef, 0);
    pSource->m_n28 = 1;
    pSource->SetPriorityGroup(nGroup);
    if (nVolume) {
        pSource->SetVolume(nVolume, 0, 1.0f);
    }
    if (fFixedVariance != 0.0f) {
        pSource->SetFixedVariance(fFixedVariance);
    }
    if (fPitchVariance != 0.0f) {
        pSource->SetPitchVariance(fPitchVariance);
    }
    if (nDelay) {
        pSource->SetOneShotDelay(nDelay);
    } else {
        nResult = pSource->Play(0, 0);
        if (pSource->m_bPlayFailed) {
            delete pSource;
        }
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x005D7680
int CExoSoundInternal::PlayOneShotSoundAt(const CResRef& cResRef, Vector vPos, unsigned char nGroup,
                                          unsigned long nDelay, unsigned char nVolume, float fFixedVariance,
                                          float fPitchVariance)
{
    int nResult = 0;
    if (cResRef == "") {
        return 0;
    }
    char* szResRef = cResRef.GetResRefStr();
    char c = (char)toupper(szResRef[0]);
    if ((c == 'C' || c == 'P' || c == 'N') && szResRef[1] == '_') {
        if (m_pOneShotStream) {
            m_pOneShotStream->Stop();
            delete m_pOneShotStream;
        }
        m_pOneShotStream = new CExoStreamingSoundSource();
        if (m_pOneShotStream) {
            m_pOneShotStream->SetPriorityGroup(nGroup);
            m_pOneShotStream->Set3D(1);
            m_pOneShotStream->SetPosition(vPos);
            if (nVolume) {
                m_pOneShotStream->SetVolume(nVolume);
            }
            CPriorityGroup* pGroup = &m_pPriorityGroups[nGroup];
            m_pOneShotStream->SetDistances(pGroup->GetMinDistance(), pGroup->GetMaxDistance());
            nResult = m_pOneShotStream->Play(cResRef, 4, 0, 0, 0);
        }
        return nResult;
    }
    CExoSoundSourceInternal* pSource = new CExoSoundSourceInternal(this);
    pSource->SetResRef(cResRef, 0);
    pSource->m_n28 = 1;
    pSource->Set3D(1);
    pSource->SetPriorityGroup(nGroup);
    pSource->SetPosition(vPos);
    if (nVolume) {
        pSource->SetVolume(nVolume, 0, 1.0f);
    }
    if (fFixedVariance != 0.0f) {
        pSource->SetFixedVariance(fFixedVariance);
    }
    if (fPitchVariance != 0.0f) {
        pSource->SetPitchVariance(fPitchVariance);
    }
    CPriorityGroup* pGroup = &m_pPriorityGroups[nGroup];
    pSource->SetDistances(pGroup->GetMinDistance(), pGroup->GetMaxDistance());
    if (nDelay) {
        pSource->SetOneShotDelay(nDelay);
    } else {
        nResult = pSource->Play(0, 0);
        if (pSource->m_bPlayFailed) {
            delete pSource;
        }
    }
    return nResult;
}
