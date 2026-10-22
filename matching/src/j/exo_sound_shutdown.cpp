// CExoSoundInternal::ShutDown (0x005D6D30) and the destructor
// (0x005D9400), constructor (0x005D86E0).
#include "j/exo_sound_list.h"

#define EXO_MAX(a, b) (((a) > (b)) ? (a) : (b))

// FUNCTION: SWKOTOR 0x005D6D30
void CExoSoundInternal::ShutDown()
{
    CExoLinkedListPosition pos = m_lStreaming.GetHeadPos();
    CExoStreamingSoundSourceInternal* pStream = m_lStreaming.GetHead();
    while (pos) {
        m_lStreaming.GetNext(pos);
        pStream->Stop();
        if (pos) {
            pStream = m_lStreaming.GetAtPos(pos);
        }
    }

    while (m_lPlaying.GetHeadPos()) {
        CExoSoundSourceInternal* pSource = m_lPlaying.GetHead();
        if (!pSource->m_bDeleteWhenDone) {
            CExoString sName;
            pSource->m_cResRef.CopyToString(sName);
            sName.Format("CExoSoundSource %s not freed", sName.CStr());
        }
        delete pSource;
    }

    if (!g_bSoundDisabled) {
        if (m_p3DVoices) {
            for (int i = 0; i < m_nNumber3DVoices; i++) {
                if (m_p3DVoices[i].m_hSample) {
                    AIL_release_3D_sample_handle(m_p3DVoices[i].m_hSample);
                    m_p3DVoices[i].m_hSample = 0;
                }
            }
            delete[] m_p3DVoices;
            m_p3DVoices = 0;
        }
        if (m_p2DVoices) {
            for (unsigned char n = 0; n < m_nNumber2DVoicesX2; n++) {
                if (m_p2DVoices[n].m_hSample) {
                    AIL_release_sample_handle(m_p2DVoices[n].m_hSample);
                    m_p2DVoices[n].m_hSample = 0;
                }
            }
            delete[] m_p2DVoices;
            m_p2DVoices = 0;
        }
    }

    if (m_hProvider) {
        AIL_close_3D_provider(m_hProvider);
        m_hProvider = 0;
        m_n8 = 0;
        m_sProviderName = "";
    }
    AIL_quick_shutdown();
}

// FUNCTION: SWKOTOR 0x005D9400
CExoSoundInternal::~CExoSoundInternal()
{
    ShutDown();
    if (m_pPriorityGroups) {
        delete[] m_pPriorityGroups;
        m_pPriorityGroups = 0;
    }
    m_aArray108.Clear();
    m_aArray108.num = 0;
}

// FUNCTION: SWKOTOR 0x005D86E0
CExoSoundInternal::CExoSoundInternal()
{
    m_p3DVoices = 0;
    m_p2DVoices = 0;
    m_hProvider = 0;
    m_n8 = 0;
    m_pPriorityGroups = 0;
    m_n2A = 0;
    m_n58 = 0;
    InitPriorityGroups();
    m_f64 = 1.0f;
    m_f7C = 1.0f;
    m_f78 = 1.0f;
    m_f74 = 1.0f;
    m_f70 = 1.0f;
    m_f6C = 1.0f;
    m_n5C = 0;
    m_n54 = 0;
    m_n50 = 0;
    m_nC = 0;
    m_f60 = 1.0f;
    m_nNumber3DVoices = 0;
    m_nNumber2DVoicesX2 = 0;
    m_fEffectsLevel = 0.6f;
    m_n0 = 1;
    m_bUseEAX = 1;
    m_n10 = 0;
    AIL_set_preference(1, 0x40);
    m_nMaxEAX = 0;
    m_f14 = 1.0f;
    m_f18 = 1.0f;
    m_f1C = 1.0f;
    m_n20 = 0;
    m_f24 = 1.0f;
    m_nStartTime = (unsigned long)(g_pExoBase->m_pcExoTimers->GetHighResolutionTimer() / 1000);
    m_posIterator = 0;
    m_nAC = 0;
    m_nB0 = 0;
    m_nB4 = 0;
    m_nB8 = 0;
    m_nBC = 0;
    m_nC4 = 0;
    m_nC8 = 0;
    m_nCC = 0;
    m_nIteratorList = 1;
    m_nC0 = 3;
    g_bSoundPaused = 0;
    m_aModes[0] = g_nSoundModeBack;
    m_nMode = 0;
    m_bModePaused = 0;
    m_n148 = 0;
}

// FUNCTION: SWKOTOR 0x005D7BD0
void CExoSoundInternal::InitPriorityGroups()
{
    int nValue;
    float fMax;
    float fMin;
    float fVariance;
    C2DA* p2DA = new C2DA(CResRef("PriorityGroups"), 0);
    if (!p2DA->Load2DArray()) {
        delete p2DA;
        return;
    }
    m_n2A = p2DA->GetNumRows();
    m_pPriorityGroups = new CPriorityGroup[m_n2A];
    for (unsigned char i = 0; i < m_n2A; i++) {
        if (p2DA->GetINTEntry(i, "Priority", &nValue)) {
            m_pPriorityGroups[i].SetPriority((unsigned char)nValue);
        }
        if (p2DA->GetINTEntry(i, "Volume", &nValue)) {
            m_pPriorityGroups[i].SetVolume((unsigned char)nValue);
        }
        if (p2DA->GetINTEntry(i, "MaxPlaying", &nValue)) {
            m_pPriorityGroups[i].SetMaxPlaying((unsigned char)nValue);
        }
        if (p2DA->GetINTEntry(i, "Interrupt", &nValue)) {
            m_pPriorityGroups[i].SetInterrupt(nValue);
        }
        if (p2DA->GetINTEntry(i, "FadeTime", &nValue)) {
            m_pPriorityGroups[i].SetFadeTime((unsigned short)nValue);
        }
        if (p2DA->GetFLOATEntry(i, "MaxVolumeDist", &fMax) &&
            p2DA->GetFLOATEntry(i, "MinVolumeDist", &fMin)) {
            m_pPriorityGroups[i].SetVolumeDist(fMin, fMax);
        }
        if (p2DA->GetFLOATEntry(i, "PlaybackVariance", &fVariance)) {
            m_pPriorityGroups[i].SetPlaybackVariance(fVariance);
        }
    }
    delete p2DA;
}

// FUNCTION: SWKOTOR 0x005D9140
void CExoSoundInternal::LoadProviderSettings()
{
    C2DA* p2DA = new C2DA(CResRef("SoundProvider"), 0);
    if (!p2DA->Load2DArray()) {
        delete p2DA;
        return;
    }
    CExoString sName;
    for (unsigned char i = 0; i < m_lProviders.GetCount(); i++) {
        for (unsigned char nRow = 0; nRow < p2DA->m_nNumRows; nRow++) {
            if (p2DA->GetCExoStringEntry(nRow, "Provider", &sName)) {
                if (*m_lProviders.GetString(i) == sName) {
                    CSoundProviderInfo* pInfo = new CSoundProviderInfo;
                    pInfo->m_f2D3DBias = 1.0f;
                    pInfo->m_nProvider = i;
                    p2DA->GetINTEntry(nRow, "EAX", &pInfo->m_bEAX);
                    p2DA->GetINTEntry(nRow, "Hardware", &pInfo->m_bHardware);
                    p2DA->GetFLOATEntry(nRow, "2D3DBias", &pInfo->m_f2D3DBias);
                    m_aArray108.Add(pInfo);
                    m_nMaxEAX = EXO_MAX(m_nMaxEAX, pInfo->m_bEAX);
                    break;
                }
            }
        }
    }
    p2DA->Unload2DArray();
    delete p2DA;
}

// FUNCTION: SWKOTOR 0x005DA6D0
void CExoSoundInternal::EnumerateProviders()
{
    HPROENUM hEnum = 0;
    CExoString sKey;
    CExoString sValue;
    CExoIni cIni;
    HPROVIDER aProviders[64];
    char* aNames[64];
    m_lProviders.m_bSorted = 1;
    m_lProviders.Clear();
    m_aArrayEC.num = 0;
    int i = 0;
    while (AIL_enumerate_3D_providers(&hEnum, &aProviders[i], &aNames[i])) {
        int nFilter = 0;
        while (nFilter != -1) {
            sKey.Format("%s%d", "3D Provider Filter", nFilter);
            nFilter = -1;
        }
        if (AIL_open_3D_provider(aProviders[i]) == 0) {
            AIL_close_3D_provider(aProviders[i]);
            m_lProviders.Add(new CExoString(aNames[i]));
            m_aArrayEC.Add(aProviders[i]);
        }
        i++;
    }
    LoadProviderSettings();
}
