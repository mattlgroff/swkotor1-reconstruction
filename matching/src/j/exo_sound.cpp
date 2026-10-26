// the public sound facades CExoSoundSource (+0x04 internal),
// CExoStreamingSoundSource and CExoSound (+0x00 internal),
// 0x005D5870-0x005D64E0. The internal classes are defined in
// exo_sound_internal.cpp so nothing gets inlined here.
#include "j/exo_sound.h"

// FUNCTION: SWKOTOR 0x005D5870 SYMBOL: ??0CExoSoundSource@@QAE@XZ
CExoSoundSource::CExoSoundSource()
{
    if (g_bSoundDisabled) {
        m_pInternal = 0;
        return;
    }
    m_pInternal = new CExoSoundSourceInternal(g_pExoSound->m_pInternal);
}

// FUNCTION: SWKOTOR 0x005D5900
void CExoSoundSource::SetPriorityGroup(unsigned char nGroup)
{
    if (m_pInternal)
        m_pInternal->SetPriorityGroup(nGroup);
}

// FUNCTION: SWKOTOR 0x005D5910
void CExoSoundSource::Set3D(int b3D)
{
    if (m_pInternal)
        m_pInternal->Set3D(b3D);
}

// FUNCTION: SWKOTOR 0x005D5920
int CExoSoundSource::IsHardwarePlayingSoundPleaseDontUseThis()
{
    if (m_pInternal)
        return m_pInternal->IsHardwarePlayingSoundPleaseDontUseThis();
    return 0;
}

// FUNCTION: SWKOTOR 0x005D5930
int CExoSoundSource::Play()
{
    if (m_pInternal)
        return m_pInternal->Play(0, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x005D5950
void CExoSoundSource::SetVolume(unsigned char nVolume)
{
    if (nVolume > 127)
        nVolume = 127;
    if (m_pInternal)
        m_pInternal->SetVolume(nVolume, 0, 1.0f);
}

// FUNCTION: SWKOTOR 0x005D5980
void CExoSoundSource::SetPitchVariance(float fVariance)
{
    if (fVariance > 1.0f)
        fVariance = 1.0f;
    else if (fVariance < 0.0f)
        fVariance = 0.0f;
    if (m_pInternal)
        m_pInternal->SetPitchVariance(fVariance);
}

// FUNCTION: SWKOTOR 0x005D59D0
void CExoSoundSource::SetLooping(int bLooping)
{
    if (m_pInternal)
        m_pInternal->SetLooping(bLooping);
}

// FUNCTION: SWKOTOR 0x005D59E0
void CExoSoundSource::SetPosition(const Vector& vPos, float fHeight)
{
    if (m_pInternal) {
        Vector v = vPos;
        v.z += fHeight;
        m_pInternal->SetPosition(v);
    }
}

// FUNCTION: SWKOTOR 0x005D5A20
void CExoSoundSource::Stop()
{
    if (m_pInternal)
        m_pInternal->Stop();
}

// FUNCTION: SWKOTOR 0x005D5A30
int CExoSoundSource::SetFixedVariance(float fVariance)
{
    if (m_pInternal)
        return m_pInternal->SetFixedVariance(fVariance);
    return 0;
}

// FUNCTION: SWKOTOR 0x005D5A50
CExoStreamingSoundSource::CExoStreamingSoundSource()
{
    if (g_bSoundDisabled) {
        m_pInternal = 0;
        return;
    }
    m_pInternal = new CExoStreamingSoundSourceInternal(g_pExoSound->m_pInternal);
}

// FUNCTION: SWKOTOR 0x005D5AF0
int CExoStreamingSoundSource::IsPlaying()
{
    if (m_pInternal)
        return m_pInternal->IsPlaying();
    return 0;
}

// FUNCTION: SWKOTOR 0x005D5B00
int CExoStreamingSoundSource::GetPlayFailed()
{
    if (m_pInternal)
        return m_pInternal->GetPlayFailed();
    return 0;
}

// FUNCTION: SWKOTOR 0x005D5B10
int CExoStreamingSoundSource::Play(const CResRef& cResRef, unsigned short nResType,
                                   int bVoice, int bMusic, int nUnused)
{
    if (m_pInternal)
        return m_pInternal->Play(cResRef, nResType, bVoice, bMusic, nUnused);
    return 0;
}

// FUNCTION: SWKOTOR 0x005D5B20
void CExoStreamingSoundSource::Pause()
{
    if (m_pInternal)
        m_pInternal->Pause();
}

// FUNCTION: SWKOTOR 0x005D5B30
void CExoStreamingSoundSource::Resume()
{
    if (m_pInternal)
        m_pInternal->Resume();
}

// FUNCTION: SWKOTOR 0x005D5B40
void CExoStreamingSoundSource::SetLooping(int bLooping)
{
    if (m_pInternal)
        m_pInternal->SetLooping(bLooping);
}

// FUNCTION: SWKOTOR 0x005D5B50
void CExoStreamingSoundSource::SetVolume(unsigned char nVolume)
{
    if (nVolume > 127)
        nVolume = 127;
    if (m_pInternal)
        m_pInternal->SetVolume(nVolume, 0, 1.0f);
}

// FUNCTION: SWKOTOR 0x005D5B80
void CExoStreamingSoundSource::Stop()
{
    if (m_pInternal)
        m_pInternal->Stop();
}

// FUNCTION: SWKOTOR 0x005D5B90
void CExoStreamingSoundSource::FadeAndStop(unsigned long nMs)
{
    if (m_pInternal)
        m_pInternal->FadeAndStop(nMs);
}

// FUNCTION: SWKOTOR 0x005D5BA0
void CExoStreamingSoundSource::SetPriorityGroup(unsigned char nGroup)
{
    if (m_pInternal)
        m_pInternal->SetPriorityGroup(nGroup);
}

// FUNCTION: SWKOTOR 0x005D5BB0
void CExoStreamingSoundSource::SetReverbEnabled(int bEnabled)
{
    if (m_pInternal)
        m_pInternal->SetReverbEnabled(bEnabled);
}

// FUNCTION: SWKOTOR 0x005D5BC0
CExoSound::CExoSound(unsigned long a, unsigned long b, unsigned long c, unsigned long d)
{
    m_pInternal = 0;
    if (!g_bSoundDisabled) {
        m_pInternal = new CExoSoundInternal();
        if (m_pInternal)
            m_pInternal->Initialize(a, b, c, d);
    }
}

// FUNCTION: SWKOTOR 0x005D5C50
void CExoSound::SetMusicVolume(float fVolume, int bApply)
{
    if (m_pInternal) {
        float fClamped = fVolume < 1.0f ? (0.0f > fVolume ? 0.0f : (fVolume < 1.0f ? fVolume : 1.0f)) : 1.0f;
        m_pInternal->SetMusicVolume(fClamped, 0, bApply);
        m_pInternal->ApplyVolumes(0);
    }
}

// FUNCTION: SWKOTOR 0x005D5CD0
void CExoSound::SetSoundEffectVolume(float fVolume)
{
    if (m_pInternal) {
        float fClamped = fVolume < 1.0f ? (0.0f > fVolume ? 0.0f : (fVolume < 1.0f ? fVolume : 1.0f)) : 1.0f;
        m_pInternal->SetSoundEffectVolume(fClamped, 0);
        m_pInternal->ApplyVolumes(0);
    }
}

// FUNCTION: SWKOTOR 0x005D5D50
void CExoSound::SetDialogVolume(float fVolume)
{
    if (m_pInternal) {
        float fClamped = fVolume < 1.0f ? (0.0f > fVolume ? 0.0f : (fVolume < 1.0f ? fVolume : 1.0f)) : 1.0f;
        m_pInternal->SetDialogVolume(fClamped, 0);
        m_pInternal->ApplyVolumes(0);
    }
}

// FUNCTION: SWKOTOR 0x005D5DD0
void CExoSound::Render()
{
    if (m_pInternal)
        m_pInternal->Render();
}

// FUNCTION: SWKOTOR 0x005D5DE0
void CExoSound::SetListenerOrientation(const Vector& a, const Vector& b)
{
    if (m_pInternal)
        m_pInternal->SetListenerOrientation(a, b);
}

// FUNCTION: SWKOTOR 0x005D5DF0
void CExoSound::SetListenerPosition(const Vector& a)
{
    if (m_pInternal)
        m_pInternal->SetListenerPosition(a);
}

// FUNCTION: SWKOTOR 0x005D5E00
int CExoSound::PlayOneShotSound(unsigned long a, unsigned long b, unsigned long c,
                                unsigned long d, unsigned long e, unsigned long f)
{
    if (m_pInternal)
        return m_pInternal->PlayOneShotSound(a, b, c, d, e, f);
    return 0;
}

// FUNCTION: SWKOTOR 0x005D5E10
int CExoSound::PlayOneShotSoundAt(unsigned long a, Vector vPos, float fHeight,
                                  unsigned long c, unsigned long d, unsigned long e,
                                  unsigned long f, unsigned long g)
{
    if (m_pInternal) {
        vPos.z += fHeight;
        return m_pInternal->PlayOneShotSoundAt(a, vPos, c, d, e, f, g);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005D5E70
void CExoSound::StopAllOneShots()
{
    if (m_pInternal)
        m_pInternal->StopAllOneShots();
}

// FUNCTION: SWKOTOR 0x005D5E80
void CExoSound::SetSoundMode(unsigned long nMode)
{
    if (m_pInternal)
        m_pInternal->SetSoundMode(nMode);
}

CExoSoundSource::~CExoSoundSource()
{
    if (m_pInternal) {
        delete m_pInternal;
        m_pInternal = 0;
    }
}

// FUNCTION: SWKOTOR 0x005D60A0 SYMBOL: ??_GCExoSoundSource@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x005D60E0 SYMBOL: ??0CExoSoundSource@@QAE@ABVCResRef@@@Z
CExoSoundSource::CExoSoundSource(const CResRef& cResRef)
{
    if (g_bSoundDisabled) {
        m_pInternal = 0;
        return;
    }
    m_pInternal = new CExoSoundSourceInternal(g_pExoSound->m_pInternal);
    if (m_pInternal)
        m_pInternal->SetResRef(cResRef, 0);
}

// FUNCTION: SWKOTOR 0x005D6190
unsigned long CExoSoundSource::GetProperty0x24()
{
    if (m_pInternal)
        return m_pInternal->m_n24;
    return 0;
}

// FUNCTION: SWKOTOR 0x005D61A0
void CExoSoundSource::SetProperty0x70(unsigned long a, unsigned long b)
{
    CExoSoundSourceInternal* pInternal = m_pInternal;
    if (pInternal) {
        pInternal->m_n70 = a;
        pInternal->m_n6C = b;
    }
}

// FUNCTION: SWKOTOR 0x005D61C0
void CExoSoundSource::SetResRef(const CResRef& cResRef, int bFlag)
{
    if (m_pInternal) {
        m_pInternal->Stop();
        m_pInternal->m_n58 = 0;
        m_pInternal->m_pSound->Render();
        m_pInternal->SetResRef(cResRef, bFlag);
    }
}

// FUNCTION: SWKOTOR 0x005D6230
CResRef CExoStreamingSoundSource::GetFilePlaying()
{
    if (m_pInternal)
        return m_pInternal->m_cFilePlaying;
    return CResRef("");
}

// FUNCTION: SWKOTOR 0x005D6280
unsigned long CExoStreamingSoundSource::GetProperty0x04()
{
    if (m_pInternal)
        return m_pInternal->m_n4;
    return 0;
}

// FUNCTION: SWKOTOR 0x005D6290
void CExoStreamingSoundSource::SetProperty0x58(unsigned long n)
{
    if (m_pInternal)
        m_pInternal->m_n58 = n;
}

// FUNCTION: SWKOTOR 0x005D62A0
void CExoStreamingSoundSource::SetProperty0x48(unsigned long a, unsigned long b)
{
    CExoStreamingSoundSourceInternal* pInternal = m_pInternal;
    if (pInternal) {
        pInternal->m_n48 = a;
        pInternal->m_n4C = b;
    }
}

// FUNCTION: SWKOTOR 0x005D62C0
void CExoStreamingSoundSource::SetProperty0x5C(const Vector& v)
{
    CExoStreamingSoundSourceInternal* pInternal = m_pInternal;
    if (pInternal) {
        Vector vCopy = v;
        pInternal->m_v5C = vCopy;
    }
}

// FUNCTION: SWKOTOR 0x005D6320
void CExoSound::SetScale60(float fScale)
{
    if (m_pInternal)
        m_pInternal->m_fScale60 = fScale < 1.9f ? (0.1f > fScale ? 0.1f : (fScale < 1.9f ? fScale : 1.9f)) : 1.9f;
}

// FUNCTION: SWKOTOR 0x005D6380
float CExoSound::GetScale60()
{
    if (m_pInternal)
        return m_pInternal->m_fScale60;
    return 0.5f;
}

// FUNCTION: SWKOTOR 0x005D63A0
unsigned char CExoSound::GetNumber3DVoices()
{
    if (m_pInternal)
        return m_pInternal->m_nNumber3DVoices;
    return 24;
}

// FUNCTION: SWKOTOR 0x005D63B0
unsigned char CExoSound::GetNumber2DVoices()
{
    if (m_pInternal)
        return m_pInternal->m_nNumber2DVoices;
    return 16;
}

// FUNCTION: SWKOTOR 0x005D63C0
void CExoSound::GetListenerPosition(Vector* pOut)
{
    if (m_pInternal)
        *pOut = m_pInternal->m_vListenerPosition;
}

// FUNCTION: SWKOTOR 0x005D63F0
void CExoSound::ReInitialize(unsigned long a, unsigned long b)
{
    if (m_pInternal)
        m_pInternal->Initialize(
            m_pInternal->m_nNumber3DVoices < 24 ? 24 : m_pInternal->m_nNumber3DVoices,
            m_pInternal->m_nNumber2DVoices < 16 ? 16 : m_pInternal->m_nNumber2DVoices,
            a, b);
}

// FUNCTION: SWKOTOR 0x005D6430
void CExoSound::SetProperty0x00(unsigned long n)
{
    if (m_pInternal)
        m_pInternal->m_n0 = n;
}

// FUNCTION: SWKOTOR 0x005D6440
void CExoSound::SetProperty0x04(unsigned long n)
{
    if (m_pInternal)
        m_pInternal->m_n4 = n;
}

// FUNCTION: SWKOTOR 0x005D6450
unsigned long CExoSound::GetProperty0x100()
{
    if (m_pInternal)
        return m_pInternal->m_n100;
    return 0;
}

// FUNCTION: SWKOTOR 0x005D6460
unsigned long CExoSound::GetProperty0x104()
{
    if (m_pInternal)
        return m_pInternal->m_n104;
    return 0;
}

// FUNCTION: SWKOTOR 0x005D6470
void CExoSound::SetDesiredEnvironment(unsigned long n)
{
    if (m_pInternal)
        m_pInternal->m_nDesiredEnvironment = n;
}

// FUNCTION: SWKOTOR 0x005D6480
void CExoSound::SetDesiredAmbientScale(float f)
{
    if (m_pInternal)
        m_pInternal->m_fDesiredAmbientScale = f;
}

// FUNCTION: SWKOTOR 0x005D6490
int CExoSound::GetCurrentMode()
{
    if (m_pInternal)
        return m_pInternal->m_aModes[m_pInternal->m_nMode];
    return 0;
}

// FUNCTION: SWKOTOR 0x005D64B0
void CExoSound::GetPriorityGroupInfo(unsigned char nGroup, unsigned long* pC, unsigned long* p8)
{
    if (m_pInternal) {
        CPriorityGroup* pGroup = &m_pInternal->m_pPriorityGroups[nGroup];
        *pC = pGroup->m_nC;
        *p8 = pGroup->m_n8;
    }
}

// FUNCTION: SWKOTOR 0x005D64E0
unsigned long CExoSound::GetProperty0x144()
{
    if (m_pInternal)
        return m_pInternal->m_n144;
    return 0;
}
