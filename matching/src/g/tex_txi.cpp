// CAurTextureBasic TXI handling (0x00422390-0x00422af0): reads the .txi
// text that sits next to a texture, or the TXI block stored with it.
#include "g/aur_texture.h"

class AurRes;

AurRes* __cdecl AurResOpen(char* szName, char* szExt, int* pSize, int bText);  // 0x0044c740
char* __cdecl AurResGetDataBytes(int nOffset, AurRes* pRes);                    // 0x0044bd10
char* __cdecl AurResGetNextLine();                                              // 0x0044bfa0
void __cdecl AurResClose(AurRes* pRes, int nSize);                              // 0x0044c8f0

extern "C" int __cdecl _stricmp(const char* a, const char* b);
extern "C" int __cdecl strcmp(const char* a, const char* b);
extern "C" size_t __cdecl strlen(const char* s);
#pragma intrinsic(strcmp, strlen)

char* __cdecl GetWord(char* s);                         // 0x00463530
void __cdecl ParseVector(char* s, float* pOut);         // 0x00463570
void __cdecl ParseString(char* s, char* pOut);          // 0x00463590
void __cdecl ParseFloat(char* s, float* pOut);          // 0x004635c0
void __cdecl ParseInt(char* s, int* pOut);              // 0x004635e0
void __cdecl ParseShort(char* s, short* pOut);          // 0x00463650
void __cdecl ParseBool(char* s, unsigned char* pOut);   // 0x00463680

class CAurFontInfoParse {
public:
    void ParseField(char* s);                           // 0x00422210
};

// Texture procedures (worker H's TextureController hierarchy; layout from
// matching/include/h/texctl.h). The g header's CAurTextureProcedure is the
// interface part.
class TextureController : public CAurTextureProcedure {
public:
    TextureController(CAurTexture* pTexture);           // 0x00462a70

    CAurTexture* m_pTexture;                // +0x04
    char pad08[0x38 - 0x08];
};

class WaterTextureController : public TextureController {
public:
    WaterTextureController(CAurTexture* pTexture);      // 0x004630b0

    char pad38[0x6c - 0x38];
};

class WaveTextureController : public TextureController {
public:
    WaveTextureController(CAurTexture* pTexture);       // 0x00463260

    char pad38[0x40 - 0x38];
};

class ArturoTextureController : public TextureController {
public:
    ArturoTextureController(CAurTexture* pTexture);     // 0x00463320

    char pad38[0x54 - 0x38];
};

class CycleTIDTextureController : public TextureController {
public:
    CycleTIDTextureController(CAurTexture* pTexture);   // 0x00463470

    char pad38[0x44 - 0x38];
};

// vtable 0x0073ed54, 0x3c bytes.
class LifeTextureController : public TextureController {
public:
    LifeTextureController(CAurTexture* pTexture) : TextureController(pTexture) {}
    virtual ~LifeTextureController();
    virtual void Update();
    virtual void Init();

    unsigned char* m_pCells;                // +0x38
};

// vtable 0x0073ed70, 0x44 bytes.
class PerlinTextureController : public TextureController {
public:
    PerlinTextureController(CAurTexture* pTexture) : TextureController(pTexture) {}
    virtual ~PerlinTextureController();
    virtual void Update();
    virtual void Init();

    unsigned char* m_pNoise;                // +0x38
    unsigned char* m_pGradient;             // +0x3c
    int m_n40;                              // +0x40
};

// vtable 0x0073ed90, 0x40 bytes.
class RandomTextureController : public TextureController {
public:
    RandomTextureController(CAurTexture* pTexture) : TextureController(pTexture)
    {
        m_p38 = 0;
        m_p3c = 0;
    }
    virtual ~RandomTextureController();
    virtual void Update();
    virtual void Init();

    unsigned char* m_p38;                   // +0x38
    unsigned char* m_p3c;                   // +0x3c
};

// vtable 0x0073edac, 0x40 bytes.
class RingDistortTextureController : public TextureController {
public:
    RingDistortTextureController(CAurTexture* pTexture) : TextureController(pTexture)
    {
        m_p38 = 0;
        m_n3c = 0;
    }
    virtual ~RingDistortTextureController();
    virtual void Update();
    virtual void Init();

    unsigned char* m_p38;                   // +0x38
    int m_n3c;                              // +0x3c
};

// FUNCTION: SWKOTOR 0x00422390
void CAurTextureBasic::ParseField(char* s)
{
    char szProcedure[64];
    szProcedure[0] = 0;
    if (!_stricmp("proceduretype", GetWord(s)))
        ParseString(s + 13, szProcedure);
    if (strlen(szProcedure)) {
        if (m_pProcedure) {
            delete m_pProcedure;
            m_pProcedure = 0;
        }
    if (!strcmp(szProcedure, "water"))
        m_pProcedure = new WaterTextureController(this);
    if (!strcmp(szProcedure, "life"))
        m_pProcedure = new LifeTextureController(this);
    if (!strcmp(szProcedure, "perlin"))
        m_pProcedure = new PerlinTextureController(this);
    if (!strcmp(szProcedure, "arturo"))
        m_pProcedure = new ArturoTextureController(this);
    if (!strcmp(szProcedure, "wave"))
        m_pProcedure = new WaveTextureController(this);
    if (!strcmp(szProcedure, "cycle"))
        m_pProcedure = new CycleTIDTextureController(this);
    if (!strcmp(szProcedure, "random"))
        m_pProcedure = new RandomTextureController(this);
    if (!strcmp(szProcedure, "ringtexdistort"))
        m_pProcedure = new RingDistortTextureController(this);
    }
    if (!_stricmp("filerange", GetWord(s)))
        ParseShort(s + 9, &m_nc6);
    if (!_stricmp("defaultwidth", GetWord(s)))
        ParseShort(s + 12, &m_nc8);
    if (!_stricmp("defaultheight", GetWord(s)))
        ParseShort(s + 13, &m_nca);
    if (!_stricmp("downsamplemax", GetWord(s)))
        ParseShort(s + 13, &m_nDownsampleMax);
    if (!_stricmp("downsamplemin", GetWord(s)))
        ParseShort(s + 13, &m_nDownsampleMin);
    if (!_stricmp("mipmap", GetWord(s)))
        ParseBool(s + 6, &m_bMipmap);
    if (!_stricmp("filter", GetWord(s)))
        ParseBool(s + 6, &m_bFilter);
    if (!_stricmp("maptexelstopixels", GetWord(s)))
        ParseBool(s + 17, &m_bdf);
    if (!_stricmp("gamma", GetWord(s)))
        ParseFloat(s + 5, &m_f24);
    if (!_stricmp("isbumpmap", GetWord(s)))
        ParseInt(s + 9, &m_nd4);
    if (!_stricmp("clamp", GetWord(s)))
        ParseShort(s + 5, &m_nClamp);
    if (!_stricmp("alphamean", GetWord(s)))
        ParseFloat(s + 9, &m_fAlphaMean);
    if (!_stricmp("isdiffusebumpmap", GetWord(s)))
        ParseBool(s + 16, &m_bd8);
    if (!_stricmp("isspecularbumpmap", GetWord(s)))
        ParseBool(s + 17, &m_bd9);
    if (!_stricmp("bumpmapscaling", GetWord(s)))
        ParseFloat(s + 14, &m_f18);
    if (!_stricmp("specularcolor", GetWord(s)))
        ParseVector(s + 13, &m_v4c.x);
    if (!_stricmp("numx", GetWord(s)))
        ParseShort(s + 4, &m_nNumX);
    if (!_stricmp("numy", GetWord(s)))
        ParseShort(s + 4, &m_nNumY);
    if (!_stricmp("cube", GetWord(s)))
        ParseBool(s + 4, &m_bCube);
    if (!_stricmp("bumpintensity", GetWord(s)))
        ParseFloat(s + 13, &m_f1c);
    if (!_stricmp("temporary", GetWord(s)))
        ParseBool(s + 9, &m_bTemporary);
    if (!_stricmp("useglobalalpha", GetWord(s)))
        ParseBool(s + 14, &m_bde);
    if (!_stricmp("isenvironmentmapped", GetWord(s)))
        ParseBool(s + 19, &m_bEnvironmentMapped);
    if (!_stricmp("envmapalpha", GetWord(s)))
        ParseFloat(s + 11, &m_f28);
    if (!_stricmp("diffusebumpintensity", GetWord(s)))
        ParseFloat(s + 20, &m_f2c);
    if (!_stricmp("specularbumpintensity", GetWord(s)))
        ParseFloat(s + 21, &m_f30);
    ((CAurFontInfoParse*)m_pTxi)->ParseField(s);
    if (m_pProcedure)
        m_pProcedure->ParseField(s);
    if (m_nDownsampleMin > 0)
        m_nDownsample = m_nDownsampleMin;
}

// Copies one '\n'-terminated line of the stored TXI block.
inline char* TxiReadLine(unsigned char*& pData, int& nRemain, char* szLine)
{
    if (nRemain <= 0)
        return 0;
    int n = 0;
    do {
        szLine[n++] = *pData++;
    } while (n < nRemain && szLine[n - 1] != '\n');
    nRemain -= n;
    if (nRemain < 0)
        return 0;
    szLine[n] = 0;
    return szLine;
}

// FUNCTION: SWKOTOR 0x00422af0
// NONMATCH (25): the original keeps no zero register in the file branch
// (ebx holds the vtable there, so ebp is pushed only inside the inlined
// stored-block branch) and pushes the SEH handler after reading fs:[0].
// Tried: inline helpers for either branch, plain delete, swapped branches.
void CAurTextureBasic::Init(char* szName)
{
    int nSize = 0;
    if (!IsTxiLoaded()) {
        if (!m_pTxi)
            m_pTxi = new CAurFontInfo;
        AurRes* pRes = AurResOpen(szName, ".txi", &nSize, 1);
        if (!pRes) {
            delete m_pTxi;
            m_pTxi = 0;
            SetTxiLoaded(1);
            return;
        }
        int nLength = nSize < 0 ? -1 - nSize : nSize;
        if (nLength > 0) {
            SetTxiData(AurResGetDataBytes(0, pRes), nLength);
            for (char* s = AurResGetNextLine(); s; s = AurResGetNextLine()) {
                while (*s == ' ' || *s == '\t')
                    s++;
                ParseField(s);
            }
        }
        AurResClose(pRes, nSize);
        if (m_pTxi && m_pTxi->m_l18.num == 0) {
            delete m_pTxi;
            m_pTxi = 0;
        }
        SetTxiLoaded(1);
    } else {
        char szLine[0x2000];
        unsigned char* pData = (unsigned char*)GetTxiData();
        int nRemain = GetTxiDataSize();
        if (pData) {
            char* s;
            do {
                s = TxiReadLine(pData, nRemain, szLine);
                if (s) {
                    while (*s == ' ' || *s == '\t')
                        s++;
                    ParseField(s);
                }
            } while (nRemain && s);
        }
    }
    if (!AurHasCubeMaps())
        m_bCube = 0;
}
