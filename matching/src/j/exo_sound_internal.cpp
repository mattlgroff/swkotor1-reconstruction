// CExoSoundInternal / CExoSoundSourceInternal /
// CExoStreamingSoundSourceInternal / CPriorityGroup getters and small
// setters, 0x005D5FE0-0x005D6BC0.
#include "j/exo_sound.h"

#define EXO_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define EXO_MIN(a, b) (((a) < (b)) ? (a) : (b))

// FUNCTION: SWKOTOR 0x005D5FE0
unsigned char CExoSoundInternal::GetProperty0x46()
{
    return m_n46;
}

// FUNCTION: SWKOTOR 0x005D5FF0
unsigned char CExoStreamingSoundSourceInternal::GetPriorityGroupIndex()
{
    return m_nPriorityGroup;
}

// FUNCTION: SWKOTOR 0x005D6000
unsigned long CExoSoundInternal::GetProperty0x58()
{
    return m_n58;
}

// FUNCTION: SWKOTOR 0x005D6010
float CExoSoundInternal::GetMusicVolume(int bCurrent)
{
    if (bCurrent)
        return m_fMusicVolumeCurrent;
    return m_fMusicVolume;
}

// FUNCTION: SWKOTOR 0x005D6030
float CExoSoundInternal::GetSoundEffectVolume(int bCurrent)
{
    if (bCurrent)
        return m_fSoundEffectVolumeCurrent;
    return m_fSoundEffectVolume;
}

// FUNCTION: SWKOTOR 0x005D6050
float CExoSoundInternal::GetDialogVolume(int bCurrent)
{
    if (bCurrent)
        return m_fDialogVolumeCurrent;
    return m_fDialogVolume;
}

// FUNCTION: SWKOTOR 0x005D6070
unsigned char CExoSoundInternal::GetNumber2DVoices()
{
    return m_nNumber3DVoices;
}

// FUNCTION: SWKOTOR 0x005D6080
CPriorityGroup* CExoSoundInternal::GetPriorityGroup(unsigned char nGroup)
{
    return &m_pPriorityGroups[nGroup];
}

// FUNCTION: SWKOTOR 0x005D64F0
unsigned long CExoSoundSourceInternal::GetProperty0x28()
{
    return m_n28;
}

// FUNCTION: SWKOTOR 0x005D6500
unsigned long CExoSoundSourceInternal::GetOneShotDelay()
{
    return m_nOneShotDelay;
}

// FUNCTION: SWKOTOR 0x005D6510
unsigned long CExoSoundSourceInternal::GetProperty0x64()
{
    return m_n64;
}

// FUNCTION: SWKOTOR 0x005D6520
unsigned char CExoSoundSourceInternal::GetProperty0x85()
{
    return m_n85;
}

// FUNCTION: SWKOTOR 0x005D6530
unsigned long CExoStreamingSoundSourceInternal::GetProperty0x34()
{
    return m_n34;
}

// FUNCTION: SWKOTOR 0x005D6540
void CExoSoundInternal::SetListenerOrientation(const Vector& vFront, const Vector& vUp)
{
    if (m_vListenerFront == vFront && m_vListenerUp == vUp)
        return;
    m_vListenerFront = vFront;
    m_vListenerUp = vUp;
    if (m_hProvider) {
        H3DPOBJECT hListener = AIL_open_3D_listener(m_hProvider);
        AIL_set_3D_orientation(hListener, vFront.x, vFront.y, vFront.z * -1.0f,
                               vUp.x, vUp.y, vUp.z * -1.0f);
        AIL_close_3D_listener(hListener);
    }
}

// FUNCTION: SWKOTOR 0x005D6600
void CExoSoundInternal::SetListenerPosition(const Vector& vPos)
{
    if (m_vListenerPosition == vPos)
        return;
    m_vListenerPosition = vPos;
    if (m_hProvider) {
        H3DPOBJECT hListener = AIL_open_3D_listener(m_hProvider);
        if (hListener) {
            AIL_set_3D_position(hListener, vPos.x, vPos.y, vPos.z * -1.0f);
            AIL_close_3D_listener(hListener);
        }
    }
}

// FUNCTION: SWKOTOR 0x005D6670
CResRef* CExoStreamingSoundSourceInternal::GetFilePlaying()
{
    return &m_cFilePlaying;
}

// FUNCTION: SWKOTOR 0x005D6680
unsigned long CExoStreamingSoundSourceInternal::GetFileFormat()
{
    return m_nFileFormat;
}

// FUNCTION: SWKOTOR 0x005D6690
unsigned char CPriorityGroup::GetMaxPlayer()
{
    return m_nMaxPlayer;
}

// FUNCTION: SWKOTOR 0x005D66A0
unsigned char CPriorityGroup::GetByte5()
{
    return m_n5;
}

// FUNCTION: SWKOTOR 0x005D66B0
float CExoSoundInternal::GetDesiredAmbientScale()
{
    return m_fDesiredAmbientScale;
}

// FUNCTION: SWKOTOR 0x005D66C0
unsigned long CExoSoundInternal::DesiredEnvironment()
{
    return m_nDesiredEnvironment;
}

// FUNCTION: SWKOTOR 0x005D6940
void CExoSoundInternal::SetMusicVolume(float fVolume, int nMode, int bApply)
{
    if (!nMode)
        m_fMusicVolume = fVolume;
    m_fMusicVolumeCurrent = fVolume;
    if (bApply == 1) {
        CPriorityGroup* pGroup = &m_pPriorityGroups[2];
        if (pGroup) {
            pGroup->m_nVolume = (unsigned char)EXO_MIN(127.0f, EXO_MAX(0.0f, fVolume * 127.0f));
        }
    }
}

// FUNCTION: SWKOTOR 0x005D69D0
void CExoSoundInternal::SetSoundEffectVolume(float fVolume, int nMode)
{
    if (!nMode)
        m_fSoundEffectVolume = fVolume;
    m_fSoundEffectVolumeCurrent = fVolume;
}

// FUNCTION: SWKOTOR 0x005D69F0
void CExoSoundInternal::SetDialogVolume(float fVolume, int nMode)
{
    if (!nMode)
        m_fDialogVolume = fVolume;
    m_fDialogVolumeCurrent = fVolume;
}

// FUNCTION: SWKOTOR 0x005D6A10
void CExoSoundInternal::MuteSound()
{
    if (!m_bMuted) {
        m_nFadeInTime = 0;
        m_bMuted = 1;
        m_nFadeOutTime = 500;
        m_nFadeStart = (unsigned long)g_pExoBase->m_pcExoTimers->GetHighResolutionTimer() / 1000;
    }
}

// FUNCTION: SWKOTOR 0x005D6A50
void CExoSoundInternal::UnMuteSound()
{
    if (m_bMuted) {
        m_nFadeOutTime = 0;
        m_bMuted = 0;
        m_nFadeInTime = 500;
        m_nFadeStart = (unsigned long)g_pExoBase->m_pcExoTimers->GetHighResolutionTimer() / 1000;
    }
}

// FUNCTION: SWKOTOR 0x005D6A90
int CExoSoundInternal::GetSourceVolumeType(CExoSoundSourceInternal* pSource)
{
    unsigned char n = pSource->m_nPriorityGroup;
    if (n == 8 || n == 9 || n == 16 || n == 17 || n == 26)
        return 1;
    if (n == 1 || n == 2)
        return 3;
    return 2;
}

// FUNCTION: SWKOTOR 0x005D6AD0
int CExoSoundInternal::GetStreamVolumeType(CExoStreamingSoundSourceInternal* pSource)
{
    unsigned char n = pSource->m_nPriorityGroup;
    if (n == 8 || n == 9 || n == 16 || n == 17 || n == 26)
        return 1;
    if (n == 1 || n == 2)
        return 3;
    return 2;
}

// FUNCTION: SWKOTOR 0x005D6B10
float CExoSoundInternal::GetVolume(int nType, int bCurrent)
{
    switch (nType) {
    case 3:
        if (bCurrent)
            return m_fMusicVolumeCurrent;
        return m_fMusicVolume;
    case 2:
        if (bCurrent)
            return m_fSoundEffectVolumeCurrent;
        return m_fSoundEffectVolume;
    case 1:
        if (bCurrent)
            return m_fDialogVolumeCurrent;
        return m_fDialogVolume;
    }
    return 1.0f;
}

// FUNCTION: SWKOTOR 0x005D6B70
int CExoSoundInternal::IsSourceAudible(CExoSoundSourceInternal* pSource)
{
    int nMode = m_aModes[m_nMode];
    if (nMode == g_n74C5F4) {
        unsigned char n = pSource->m_nPriorityGroup;
        if (n == 1 || n == 2 || n == 4 || n == 11)
            return 0;
    }
    else if (nMode == g_n74C5FC) {
        unsigned char n = pSource->m_nPriorityGroup;
        if (n == 1 || n == 2 || n == 11)
            return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005D6BC0
int CExoSoundInternal::IsStreamAudible(CExoStreamingSoundSourceInternal* pSource)
{
    int nMode = m_aModes[m_nMode];
    if (nMode == g_n74C5F4) {
        unsigned char n = pSource->m_nPriorityGroup;
        if (n == 1 || n == 2 || n == 4 || n == 11)
            return 0;
    }
    else if (nMode == g_n74C5FC) {
        unsigned char n = pSource->m_nPriorityGroup;
        if (n == 1 || n == 2 || n == 11)
            return 0;
    }
    return 1;
}
