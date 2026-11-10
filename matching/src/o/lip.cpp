// CLIP and CResLIP, 0x0070C280-0x0070C6C0.
#include <string.h>

#include "o/lip.h"

void operator delete(void* p);

// FUNCTION: SWKOTOR 0x0070C280
float CLIP::GetAnimLength()
{
    return m_fAnimLength;
}

// FUNCTION: SWKOTOR 0x0070C290
LIPViseme CLIP::GetVisemeIndex(unsigned long nIndex)
{
    if (nIndex < m_nKeyFrames) {
        LIPViseme eViseme = LIP_VISEME_EE;
        memcpy(&eViseme, m_pKeyFrames + nIndex * 5 + 4, 1);
        return eViseme;
    }
    return LIP_VISEME_EE;
}

// FUNCTION: SWKOTOR 0x0070C2C0
float CLIP::GetTimeIndex(unsigned long nIndex)
{
    if (nIndex < m_nKeyFrames) {
        float fTime;
        memcpy(&fTime, m_pKeyFrames + nIndex * 5, 4);
        return fTime;
    }
    return 0.0f;
}

// FUNCTION: SWKOTOR 0x0070C2F0 SYMBOL: ??1?$CResHelper@VCResLIP@@$0LLM@@@UAE@XZ
// FUNCTION: SWKOTOR 0x0070C350 SYMBOL: ?SetResRef@?$CResHelper@VCResLIP@@$0LLM@@@QAEXABVCResRef@@H@Z
// FUNCTION: SWKOTOR 0x0070C4B0 SYMBOL: ??_G?$CResHelper@VCResLIP@@$0LLM@@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0070C4D0
CLIP::CLIP()
{
    m_nKeyFrames = 0;
    m_fAnimLength = 0.0f;
    m_pKeyFrames = 0;
}

// FUNCTION: SWKOTOR 0x0070C520
CLIP::~CLIP()
{
    if (m_bLoaded) {
        m_pRes->Release();
        m_bLoaded = 0;
    }
    m_pKeyFrames = 0;
}

// FUNCTION: SWKOTOR 0x0070C590
int CLIP::LoadLip(CExoString sLip)
{
    CResRef cResRef(sLip);
    SetResRef(cResRef, 0);
    if (!m_pRes->Demand()) {
        return 0;
    }
    m_pKeyFrames = m_pRes->GetLIPDataPtr();
    if (strncmp("LIP V1.0", m_pKeyFrames, 8)) {
        return 0;
    }
    m_pKeyFrames += 8;
    m_fAnimLength = *(float*)m_pKeyFrames;
    m_pKeyFrames += 4;
    m_nKeyFrames = *(unsigned long*)m_pKeyFrames;
    m_pKeyFrames += 4;
    m_bLoaded = 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x0070C670 SYMBOL: ??_GCLIP@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0070C690
CResLIP::CResLIP()
{
    m_bLoaded = 0;
    m_nLIPSize = 0;
    m_pLIP = 0;
}

// FUNCTION: SWKOTOR 0x0070C6B0
CResLIP::~CResLIP()
{
}

// FUNCTION: SWKOTOR 0x0070C6C0 SYMBOL: ??_GCResLIP@@UAEPAXI@Z

template class CResHelper<CResLIP, 3004>;
