// CSWCSoundObject, the client-side placeable sound (vtable 0x007522A8),
// 0x0066AD00-0x0066BF02 (b00646 neighbours).

#include "m/sw_c_sound_object.h"

#pragma auto_inline(off)

// FUNCTION: SWKOTOR 0x00648100 SYMBOL: ?DelIndex@?$CExoArrayList@VCResRef@@@@QAEXH@Z
template <>
void CExoArrayList<CResRef>::DelIndex(int i)
{
    num--;
    for (; i < num; i++) {
        element[i] = element[i + 1];
    }
}

// FUNCTION: SWKOTOR 0x0066AD00
void CSWCSoundObject::SetVolume(int nVolume)
{
    if (!m_bStreaming) {
        m_pSource->SetVolume(nVolume);
    }
    else {
        m_pStream->SetVolume(nVolume);
    }
    m_nVolume = nVolume;
}

// FUNCTION: SWKOTOR 0x0066AD40
void CSWCSoundObject::SetPitchVariation(float f)
{
    if (!m_bStreaming) {
        m_pSource->SetPitchVariation(f);
    }
    m_fPitchVariation = f;
}

// FUNCTION: SWKOTOR 0x0066AD80
void CSWCSoundObject::SetFixedVariance(float f)
{
    if (!m_bStreaming) {
        m_pSource->SetFixedVariance(f);
    }
    m_fFixedVariance = f;
}

// FUNCTION: SWKOTOR 0x0066ADC0
void CSWCSoundObject::SetPositionElevated(const Vector& v)
{
    Vector vPos = v;
    vPos.z = vPos.z + m_fElevation;
    if (!m_bStreaming) {
        m_pSource->SetPosition(vPos, 0);
    }
    else {
        m_pStream->SetPosition(vPos);
    }
    SetPosition(vPos);
}

// FUNCTION: SWKOTOR 0x0066AE30
void CSWCSoundObject::SetActive(int bActive)
{
    SetActiveInline(bActive);
}

// FUNCTION: SWKOTOR 0x0066AE90
void CSWCSoundObject::UpdateTimeOfDay()
{
    if (m_nTimes) {
        switch (g_pAppManager->m_pClientExoApp->GetModule()->m_nTimeOfDay) {
        case 0:
        case 1:
        case 3:
            m_bInTime = (m_nTimes != 2);
            break;
        case 2:
        case 4:
            m_bInTime = (m_nTimes != 1);
            break;
        }
    }
    else {
        unsigned long nHours = m_nHours;
        m_bInTime = (nHours & (1 << g_pAppManager->m_pClientExoApp->GetModule()->m_nHour)) != 0;
    }
}

// FUNCTION: SWKOTOR 0x0066AF30
unsigned long CSWCSoundObject::GetTimeDiff()
{
    unsigned long nDay;
    unsigned long nTime;
    unsigned long nDiffDay;
    unsigned long nDiffTime;
    g_pAppManager->GetClientExoApp()->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
    g_pAppManager->GetClientExoApp()->GetWorldTimer()->SubtractWorldTimes(
        nDay, nTime, m_nStartDay, m_nStartTime, &nDiffDay, &nDiffTime);
    return nDiffTime;
}

// FUNCTION: SWKOTOR 0x0066AFA0
void CSWCSoundObject::RandomizeVolume()
{
    int nVar = m_nVolumeVariation;
    if (nVar > 0) {
        int nBase = m_nVolume;
        int nVolume = rand() % (nVar * 2) - nVar + nBase;
        if (nVolume > 0x7f) {
            nVolume = 0x7f;
        }
        nVolume = nVolume < 0 ? 0 : nVolume;
        if (!m_bStreaming) {
            m_pSource->SetVolume(nVolume);
        }
        else {
            m_pStream->SetVolume(nVolume);
        }
    }
}

// FUNCTION: SWKOTOR 0x0066B000
void CSWCSoundObject::RandomizeInterval()
{
    RandomizeIntervalInline();
}

// FUNCTION: SWKOTOR 0x0066B050
void CSWCSoundObject::Play()
{
    CResRef cUnused;
    if ((!m_bStreaming && !m_pSource->IsHardwarePlayingSoundPleaseDontUseThis()) ||
        (m_bStreaming && !m_pStream->IsPlaying())) {
        m_nPlayTimeLeft = 0;
        m_nIntervalLeft = 0;
        m_bFirstSound = 1;
        g_pAppManager->GetClientExoApp()->GetWorldTimer()->GetWorldTime(&m_nStartDay, &m_nStartTime);
    }
    SetActiveInline(1);
}

// FUNCTION: SWKOTOR 0x0066B130
void CSWCSoundObject::Stop()
{
    SetActiveInline(0);
    if (!m_bStreaming) {
        m_pSource->Stop();
    }
    else {
        m_pStream->Stop();
    }
    m_nPlayTimeLeft = 0;
    m_nIntervalLeft = 0;
}

// FUNCTION: SWKOTOR 0x0066B190
void CSWCSoundObject::FadeAndStop(unsigned long nFade)
{
    SetActiveInline(0);
    if (!m_bStreaming) {
        m_pSource->Stop();
    }
    else {
        m_pStream->FadeAndStop(nFade);
    }
    m_nPlayTimeLeft = 0;
    m_nIntervalLeft = 0;
}

// FUNCTION: SWKOTOR 0x0066B200
CSWCSoundObject::CSWCSoundObject()
    : CSWCObject(OBJECT_TYPE_SOUND)
{
    m_bStreaming = 0;
    m_pSource = 0;
    m_pStream = 0;
    m_nIntervalLeft = 0;
    m_bActive = 1;
    m_nVolume = 0x7f;
    m_nVolumeVariation = 0;
    m_nTimes = 3;
    m_fPitchVariation = 0.0f;
    m_fFixedVariance = 1.0f;
    m_nHours = 0;
    m_bInTime = 1;
    m_nPriority = 0;
    m_nInterval = 0;
    m_nIntervalVariation = 0;
    m_bContinuous = 0;
    m_bRandom = 0;
    m_pSounds = new CExoArrayList<CResRef>;
    m_nCurrentSound = 0;
    m_bFirstSound = 0;
    m_bRandomPosition = 0;
    m_fRandomRangeX = 0.0f;
    m_fRandomRangeY = 0.0f;
    m_bLoopPending = 0;
}

// FUNCTION: SWKOTOR 0x0066B310
void CSWCSoundObject::SetPosition(const Vector& v)
{
    CSWCObject::SetPosition(v);
    if (!m_bStreaming && m_pSource) {
        m_pSource->SetPosition(v, 0);
    }
    else if (m_pStream) {
        m_pStream->SetPosition(v);
    }
}

// FUNCTION: SWKOTOR 0x0066B360
int CSWCSoundObject::Unknown78(int a, int b, int c)
{
    return 0;
}

// FUNCTION: SWKOTOR 0x0066B420
void CSWCSoundObject::LoadFromMessage(CSWCMessage* pMsg)
{
    Vector vPosition;
    SetId(pMsg->ReadOBJECTIDClient());
    int bActive = pMsg->ReadBOOL();
    int b3D = pMsg->ReadBOOL();
    int bStreaming = pMsg->ReadBOOL();
    if (bStreaming) {
        m_pStream = new CExoStreamingSoundSource;
        m_bStreaming = 1;
    }
    else {
        m_pSource = new CExoSoundSource;
    }
    SetActive(bActive);
    Set3DInline(b3D);
    SetLoopingInline(bStreaming);
    if (bStreaming) {
        if (b3D) {
            m_pStream->SetLooping(1);
            m_pStream->Set3D(1);
            m_pStream->SetPriorityGroup(5);
        }
        else {
            m_pStream->SetLooping(1);
            m_pStream->Set3D(0);
            m_pStream->SetPriorityGroup(4);
        }
    }
    else {
        if (b3D) {
            m_pSource->SetPriorityGroup(0x16);
        }
        else {
            m_pSource->SetPriorityGroup(0x15);
        }
    }
    m_nVolume = pMsg->ReadBYTE(8);
    m_nVolumeVariation = pMsg->ReadBYTE(8);
    m_nTimes = pMsg->ReadBYTE(8);
    m_fPitchVariation = pMsg->ReadFLOAT(1.0f, 32);
    float fFixedVariance = pMsg->ReadFLOAT(1.0f, 32);
    m_fFixedVariance = fFixedVariance;
    m_nHours = pMsg->ReadDWORD(32);
    m_nPriority = pMsg->ReadBYTE(8);
    m_nInterval = pMsg->ReadDWORD(32);
    m_nIntervalVariation = pMsg->ReadDWORD(32);
    float fMinDistance = pMsg->ReadFLOAT(1.0f, 32);
    float fMaxDistance = pMsg->ReadFLOAT(1.0f, 32);
    SetDistancesInline(fMinDistance, fMaxDistance);
    m_bContinuous = pMsg->ReadBOOL();
    m_bRandom = pMsg->ReadBOOL();
    m_bRandomPosition = pMsg->ReadBOOL();
    float fRangeX = pMsg->ReadFLOAT(1.0f, 32);
    m_fRandomRangeX = fRangeX;
    float fRangeY = pMsg->ReadFLOAT(1.0f, 32);
    m_fRandomRangeY = fRangeY;
    vPosition.x = pMsg->ReadFLOAT(1.0f, 32);
    vPosition.y = pMsg->ReadFLOAT(1.0f, 32);
    m_fElevation = vPosition.z = pMsg->ReadFLOAT(1.0f, 32);
    SetPosition(vPosition);
    unsigned short nSounds = pMsg->ReadWORD(16);
    for (unsigned short i = 0; i < nSounds; i++) {
        m_pSounds->Add(pMsg->ReadCResRef(16));
    }
    m_bFirstSound = 1;
    m_nPlayTimeLeft = 0;
    if (m_bContinuous && nSounds == 1 && !m_bRandomPosition && !m_nInterval) {
        SetLoopingInline(1);
    }
    RandomizeIntervalInline();
}

// FUNCTION: SWKOTOR 0x0066B8A0
int CSWCSoundObject::AddToAreaAndAI(CSWCArea* pArea, CGameObjectArray* pArray)
{
    CClientAIMaster* pAI = g_pAppManager->m_pClientExoApp->GetClientAIMaster();
    AddToArea(pArea);
    pArea->m_lstSoundObjects.Add(m_idSelf);
    if (pArray->AddExternalObject(&m_idSelf, this, 0) != GAME_OBJECT_ADD_OK) {
        return 0;
    }
    if (!pAI->AddObject(this)) {
        return 0;
    }
    g_pAppManager->GetClientExoApp()->GetWorldTimer()->GetWorldTime(&m_nStartDay, &m_nStartTime);
    if (GetLoopingInline() && m_bActive) {
        m_bLoopPending = 1;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0066BAD0
CResRef CSWCSoundObject::GetNextSound()
{
    CResRef cRes;
    if (m_bFirstSound) {
        m_bFirstSound = 0;
        m_nCurrentSound = 0;
        cRes = m_pSounds->element[0];
    }
    else if (m_nCurrentSound < m_pSounds->num - 1) {
        m_nCurrentSound++;
        cRes = m_pSounds->element[m_nCurrentSound];
    }
    else if (m_bContinuous) {
        if (m_bRandom) {
            ShuffleSounds();
        }
        m_nCurrentSound = 0;
        cRes = m_pSounds->element[0];
    }
    else {
        cRes = "";
    }
    return cRes;
}

// FUNCTION: SWKOTOR 0x00641DB0
CSWCSoundObject* CSWCSoundObject::AsSWCSoundObject()
{
    return this;
}

// FUNCTION: SWKOTOR 0x0066BBD0
void CSWCSoundObject::AIUpdate()
{
    CResRef cRes;
    unsigned long nLength;
    if (!m_bActive) {
        StopIfPlaying();
        return;
    }
    if (!m_nTimes) {
        UpdateTimeOfDay();
    }
    if (!m_bInTime) {
        if (m_bStreaming) {
            m_pStream->Stop();
        }
        else if (m_pSource->IsHardwarePlayingSoundPleaseDontUseThis()) {
            m_pSource->Stop();
        }
        return;
    }
    if (GetLoopingInline()) {
        if (!m_bLoopPending) {
            return;
        }
        m_bLoopPending = 0;
        m_bFirstSound = 1;
        if (!m_bStreaming) {
            m_pSource->SetResRef(GetNextSound(), 0);
            m_pSource->SetVolume((unsigned char)m_nVolume);
            nLength = m_pSource->Play();
        }
        else {
            m_pStream->SetVolume((unsigned char)m_nVolume);
            nLength = m_pStream->Play(GetNextSound(), 4, 0, 0, 0);
        }
        m_nPlayTimeLeft = nLength;
        if (!nLength) {
            SetActiveInline(0);
        }
        return;
    }
    unsigned long nDiff = GetTimeDiff();
    g_pAppManager->GetClientExoApp()->GetWorldTimer()->GetWorldTime(&m_nStartDay, &m_nStartTime);
    if (m_nPlayTimeLeft) {
        if (nDiff >= m_nPlayTimeLeft) {
            m_nPlayTimeLeft = 0;
            nDiff = 0;
        }
        else {
            m_nPlayTimeLeft -= nDiff;
            return;
        }
    }
    if (m_nIntervalLeft) {
        if (nDiff >= m_nIntervalLeft) {
            m_nIntervalLeft = 0;
        }
        else {
            m_nIntervalLeft -= nDiff;
            return;
        }
    }
    if ((cRes = GetNextSound()) == "") {
        SetActiveInline(0);
        return;
    }
    if (!m_bStreaming) {
        m_pSource->SetResRef(cRes, 0);
        m_pSource->SetVolume((unsigned char)m_nVolume);
        RandomizeVolume();
        m_pSource->SetPitchVariation(GetPitchVariation());
        m_pSource->SetFixedVariance(GetFixedVariance());
        nLength = m_pSource->Play();
    }
    else {
        m_pStream->SetVolume((unsigned char)m_nVolume);
        RandomizeVolume();
        nLength = m_pStream->Play(cRes, 4, 0, 0, 0);
    }
    m_nPlayTimeLeft = nLength;
    if (!nLength) {
        if (m_pSounds->num <= 1) {
            SetActiveInline(0);
            return;
        }
        m_pSounds->DelIndex(m_nCurrentSound);
        if (m_nCurrentSound > 0) {
            m_nCurrentSound--;
        }
    }
    if (m_bContinuous || m_nCurrentSound < m_pSounds->num - 1) {
        RandomizeInterval();
    }
}

#pragma auto_inline(on)

// Defined with auto_inline on: the scalar deleting destructors of the
// streaming source and the resref list are expanded in place.
// FUNCTION: SWKOTOR 0x0066B370
CSWCSoundObject::~CSWCSoundObject()
{
    if (m_pSource) {
        delete m_pSource;
        m_pSource = 0;
    }
    if (m_pStream) {
        delete m_pStream;
        m_pStream = 0;
    }
    CExoArrayList<CResRef>* pSounds = m_pSounds;
    if (pSounds) {
        delete[] pSounds->element;
        pSounds->element = 0;
        operator delete(pSounds);
        m_pSounds = 0;
    }
}

// FUNCTION: SWKOTOR 0x0066B980
void CSWCSoundObject::ShuffleSounds()
{
    CExoArrayList<CResRef>* pNew = new CExoArrayList<CResRef>;
    int nCount = m_pSounds->num;
    for (int i = 0; i < nCount; i++) {
        int nIndex = rand() % (nCount - i);
        pNew->Add(m_pSounds->element[nIndex]);
        m_pSounds->DelIndex(nIndex);
    }
    delete m_pSounds;
    m_pSounds = pNew;
}
