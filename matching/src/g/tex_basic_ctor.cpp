// CAurTextureBasic constructor. Its own unit: the inlined CAurFontInfo
// lists need the sized list constructor (empty-state SEH store).
#define G_EXO_LIST_SIZED_DEFAULT
#define G_AUR_FONT_INFO_INLINE_CTOR
#include "g/aur_texture.h"

extern "C" char* __cdecl strcpy(char* d, const char* s);
#pragma intrinsic(strcpy)

unsigned short AurTextureCalculateHashID(char* szName, unsigned short nCount, int* pInts);  // 0x0041f4d0
extern int g_nTextureCount;                     // 0x007a4718
extern unsigned char g_bTextureModeDynamic;     // 0x0078d400

// FUNCTION: SWKOTOR 0x00423150
CAurTextureBasic::CAurTextureBasic(char* szName, char* szFile)
{
    m_szFileName[0] = 0;
    if (szName)
        strcpy(m_szName, szName);
    else
        strcpy(m_szName, szFile);
    strcpy(m_szFileName, szFile);
    m_f24 = 1.0f;
    m_f18 = 1.0f;
    m_f1c = 1.0f;
    m_f28 = 1.0f;
    m_f2c = 1.0f;
    m_f30 = 1.0f;
    m_nWidth = 2;
    m_nHeight = 2;
    m_nc8 = 2;
    m_nca = 2;
    m_nNumX = 1;
    m_nNumY = 1;
    m_fAlphaMean = -1.0f;
    m_nDownsampleMax = 15;
    m_nDownsampleMin = 0;
    m_nDownsample = 0;
    m_nAppliedDownsample = 0;
    m_nBytesPerPixel = 0;
    m_nGLName = 0;
    m_nClamp = 0;
    m_nSchemeCount = 0;
    m_nc6 = 0;
    m_nce = 0;
    m_n5c = 0;
    m_n60 = 0;
    m_n64 = 0;
    m_nFormat = 7;
    m_bOrderedA = g_bTextureModeDynamic;
    m_bTemporary = 0;
    m_bCube = 0;
    m_bd8 = 1;
    m_bd9 = 1;
    m_bEnvironmentMapped = 0;
    m_bMipmap = 1;
    m_bFilter = 1;
    m_bdf = 0;
    m_bde = 0;
    m_bSkipOrdering = 0;
    m_bBuilt = 0;
    m_be2 = 0;
    m_be3 = 0;
    m_nd4 = 0;
    m_be5 = 0;
    m_be6 = 0;
    m_bNeedsUpdate = 1;
    m_v4c = Vector(1.0f, 1.0f, 1.0f);
    m_pTxi = new CAurFontInfo;
    m_pImage = 0;
    m_pProcedure = 0;
    m_pGLNames = 0;
    m_pScheme = 0;
    m_p48 = 0;
    m_nHash = 0;
    m_nHash = AurTextureCalculateHashID(m_szFileName, m_nSchemeCount, 0);
    AddToOrderedLists();
    g_nTextureCount++;
    m_nMemoryUsage = -1;
    m_bMemoryDirty = 1;
}
