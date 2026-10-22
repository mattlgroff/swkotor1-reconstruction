// TextureController and its Water / Wave / Arturo / CycleTID /
// Random / RingDistort subclasses (0x004609F0 .. 0x004634D0).

#include "h/texctl.h"

#define PARSE_FIELD(name, func, dst) \
    if (!_stricmp(name, GetWord(szField))) \
        func(szField + sizeof(name) - 1, dst)

// FUNCTION: SWKOTOR 0x004609F0
void RandomTextureController::Init()
{
    int nWidth = m_pTexture->GetWidth();
    int nHeight = m_pTexture->GetHeight();
    int nBpp = m_pTexture->GetBytesPerPixel();
    if (m_bDistort)
        Distort();
    m_p38 = new unsigned char[nWidth * nHeight];
    m_p3c = new unsigned char[nWidth * nHeight * nBpp];
}

// FUNCTION: SWKOTOR 0x00460A50
void RingDistortTextureController::Init()
{
    int nWidth = m_pTexture->GetWidth();
    int nHeight = m_pTexture->GetHeight();
    int nBpp = m_pTexture->GetBytesPerPixel();
    m_n3c = 0;
    m_p38 = new unsigned char[nWidth * nHeight * nBpp];
}

// FUNCTION: SWKOTOR 0x00460CF0
TextureController::~TextureController()
{
    if (m_pDistortData)
        delete[] m_pDistortData;
}

// FUNCTION: SWKOTOR 0x00462AE0 SYMBOL: ??_GTextureController@@UAEPAXI@Z

// Sets one channel of a 4-channel list, filling it with 1.0 on first use.
#define CHANNEL_FIELD(name, list, idx, setsize) \
    { \
        float f = -1.0f; \
        PARSE_FIELD(name, ParseFloat, &f); \
        if (f >= 0.0f) { \
            if (list.num == 0) { \
                list.setsize(4); \
                list[0] = 1.0f; \
                list[1] = 1.0f; \
                list[2] = 1.0f; \
                list[3] = 1.0f; \
            } \
            list[idx] = f; \
        } \
    }

// FUNCTION: SWKOTOR 0x00462B00 SYMBOL: ?ParseField@TextureController@@UAEXPAD@Z
void TextureController::ParseField(char* szField)
{
    PARSE_FIELD("channelscale", ParseFloatList, &m_lChannelScale);
    PARSE_FIELD("channeltranslate", ParseFloatList, &m_lChannelTranslate);
    PARSE_FIELD("distort", ParseInt, &m_bDistort);
    PARSE_FIELD("distortangle", ParseInt, &m_nDistortAngle);
    PARSE_FIELD("distortionamplitude", ParseFloat, &m_fDistortionAmplitude);
    PARSE_FIELD("speed", ParseFloat, &m_fSpeed);
    CHANNEL_FIELD("channelscale0", m_lChannelScale, 0, SetSizeOutOfLine);
    CHANNEL_FIELD("channelscale1", m_lChannelScale, 1, SetSizeOutOfLine);
    CHANNEL_FIELD("channelscale2", m_lChannelScale, 2, SetSizeOutOfLine);
    CHANNEL_FIELD("channelscale3", m_lChannelScale, 3, SetSizeOutOfLine);
    CHANNEL_FIELD("channeltranslate0", m_lChannelTranslate, 0, SetSizeInline);
    CHANNEL_FIELD("channeltranslate1", m_lChannelTranslate, 1, SetSizeInline);
    CHANNEL_FIELD("channeltranslate2", m_lChannelTranslate, 2, SetSizeInline);
    CHANNEL_FIELD("channeltranslate3", m_lChannelTranslate, 3, SetSizeInline);
}

// NONMATCH (4 insns): the original loads 31 into ecx before the zero
// stores and stores the vptr after them; store order permutations did not help.
// FUNCTION: SWKOTOR 0x004630B0
WaterTextureController::WaterTextureController(AurTexture* pTexture)
    : TextureController(pTexture)
{
    m_p40 = 0;
    m_p48 = 0;
    m_p44 = 0;
    m_p5c = 0;
    m_p60 = 0;
    m_fForceCycleSpeed = 57.29578f;
    m_nWaterWidth = 31;
    m_fAngleCycleSpeed = 2.864789f;
    m_nWaterHeight = 31;
    m_n68 = 2;
}

// FUNCTION: SWKOTOR 0x00463100 SYMBOL: ??_GWaterTextureController@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00463120
WaterTextureController::~WaterTextureController()
{
    delete[] m_p40;
    delete[] m_p48;
    delete[] m_p44;
    delete[] m_p5c;
    delete[] m_p60;
}

// FUNCTION: SWKOTOR 0x004631A0
void WaterTextureController::ParseField(char* szField)
{
    TextureController::ParseField(szField);
    PARSE_FIELD("forcecyclespeed", ParseFloat, &m_fForceCycleSpeed);
    PARSE_FIELD("anglecyclespeed", ParseFloat, &m_fAngleCycleSpeed);
    PARSE_FIELD("waterwidth", ParseInt, &m_nWaterWidth);
    PARSE_FIELD("waterheight", ParseInt, &m_nWaterHeight);
}

// FUNCTION: SWKOTOR 0x00463260
WaveTextureController::WaveTextureController(AurTexture* pTexture)
    : TextureController(pTexture)
{
    m_p38 = 0;
    m_p3c = 0;
}

// FUNCTION: SWKOTOR 0x00463290 SYMBOL: ??_GWaveTextureController@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x004632B0
WaveTextureController::~WaveTextureController()
{
    delete[] m_p38;
    delete[] m_p3c;
}

// FUNCTION: SWKOTOR 0x00463310
void WaveTextureController::ParseField(char* szField)
{
    TextureController::ParseField(szField);
}

// FUNCTION: SWKOTOR 0x00463320
ArturoTextureController::ArturoTextureController(AurTexture* pTexture)
    : TextureController(pTexture)
{
    m_nArturoWidth = 15;
    m_nArturoHeight = 15;
    m_p50 = 0;
    m_p40 = 0;
    m_p44 = 0;
    m_p48 = 0;
    m_nFrame = -1;
}

// FUNCTION: SWKOTOR 0x00463360 SYMBOL: ??_GArturoTextureController@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00463380
ArturoTextureController::~ArturoTextureController()
{
    delete[] m_p44;
    delete[] m_p48;
    delete[] m_p40;
    delete[] m_p50;
}

// FUNCTION: SWKOTOR 0x00463400
void ArturoTextureController::ParseField(char* szField)
{
    TextureController::ParseField(szField);
    PARSE_FIELD("arturowidth", ParseInt, &m_nArturoWidth);
    PARSE_FIELD("arturoheight", ParseInt, &m_nArturoHeight);
}

// FUNCTION: SWKOTOR 0x00463470
CycleTIDTextureController::CycleTIDTextureController(AurTexture* pTexture)
    : TextureController(pTexture)
{
    m_nFrameIndex = 0;
    m_fTime = 0;
    m_fFps = 1.0f;
}

// FUNCTION: SWKOTOR 0x004634A0 SYMBOL: ??_GCycleTIDTextureController@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x004634C0
CycleTIDTextureController::~CycleTIDTextureController()
{
}

// FUNCTION: SWKOTOR 0x004634D0
void CycleTIDTextureController::ParseField(char* szField)
{
    PARSE_FIELD("fps", ParseFloat, &m_fFps);
    TextureController::ParseField(szField);
}

extern int g_nAurFrameCounter;          // 0x007A46F4
extern float g_fAurFrameDelta;          // 0x0078E574

// NONMATCH (16 insns): the original also spills m_pTexture to a stack slot
// (push ecx frame) it never reads; ours keeps it in edi only.
// FUNCTION: SWKOTOR 0x00460970
void CycleTIDTextureController::Update()
{
    if (m_nFrame != g_nAurFrameCounter) {
        m_nFrame = g_nAurFrameCounter;
        m_fTime += g_fAurFrameDelta;
        if (m_fTime > 1.0f / m_fFps) {
            m_fTime = 0;
            m_nFrameIndex = (m_nFrameIndex + 1) % m_pTexture->GetNumFrames();
            m_pTexture->SetFrame(m_nFrameIndex);
        }
    }
}
