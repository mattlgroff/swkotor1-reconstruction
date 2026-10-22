// CAurTextureBasic (load/upload/memory accounting/ordering) and
// the Aur texture front end, 0x00420440-0x004235f0. The CAurTexture
// reference half lives in aur_texture_ref.cpp. Behavioural reference:
// src/batches/b0041_aur_texture.cpp.
#include "g/aur_texture.h"

extern int g_bAuroraInitialised;        // 0x007b90f0
extern int g_nTextureMem;               // 0x0078d3f4
extern int g_bTexturesLocked;           // 0x007a4714
extern unsigned int g_nLastBoundGLName; // 0x007a47b8
extern int g_bProceduralTextures;       // 0x0078d430

void AurInitFont();                     // 0x0044cc40
void AurFreeFont();                     // 0x0044cc60
int AurTextureSetMem(int nMem);         // 0x00421d90
void AurTextureBindTID(unsigned int nTID, unsigned char bCube);

namespace GLRender {
void BindTexture(int nTarget, unsigned int nTexture);
}

// FUNCTION: SWKOTOR 0x00420440
void GLRender::BindTexture(int nTarget, unsigned int nTexture)
{
    glBindTexture(g_aGLTextureTargets[nTarget], nTexture);
    glTexParameteri(g_aGLTextureTargets[nTarget], GL_TEXTURE_MAX_ANISOTROPY_EXT,
                    g_nAnisotropyRequested);
    g_aBoundTextures[g_nActiveTextureUnit] = nTexture;
}

// Hidden functions (not in functions.csv): suspend / resume the per-frame
// texture budget pass in AurTextureMakeCurrent.
// FUNCTION: SWKOTOR 0x00420490
void AurTextureSuspendMemCheck()
{
    g_b7a46d5 = 1;
}

// FUNCTION: SWKOTOR 0x004204a0
void AurTextureResumeMemCheck()
{
    g_b7a46d5 = 0;
}

// FUNCTION: SWKOTOR 0x004204b0
void AurTextureSetModeStatic()
{
    g_bTextureModeDynamic = 0;
}

// FUNCTION: SWKOTOR 0x004204c0
void AurTextureSetModeDynamic()
{
    g_bTextureModeDynamic = 1;
}

// FUNCTION: SWKOTOR 0x004204d0
unsigned char AurTextureEnvMapDisabled()
{
    return g_bBumpMaps;
}

// FUNCTION: SWKOTOR 0x004204f0
void AurTextureBindTID(unsigned int nTID, unsigned char bCube)
{
    unsigned char bUseCube = 0;
    if (AurHasCubeMaps()) {
        if (bCube) {
            bUseCube = 1;
            glEnable(g_nGLTextureCubeMap);
        } else {
            glDisable(g_nGLTextureCubeMap);
        }
    }
    if (nTID > 1 && g_nBindLimit < 0)
        return;
    if (bUseCube) {
        glBindTexture(g_nGLTextureCubeMap, nTID);
        glTexParameteri(g_nGLTextureCubeMap, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                        g_nAnisotropyRequested);
        g_aBoundTextures[g_nActiveTextureUnit] = nTID;
        g_nBindCount++;
    } else {
        glBindTexture(g_nGLTexture2D, nTID);
        glTexParameteri(g_nGLTexture2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                        g_nAnisotropyRequested);
        g_aBoundTextures[g_nActiveTextureUnit] = nTID;
        g_nBindCount++;
    }
}

// FUNCTION: SWKOTOR 0x004205c0
CAurTexture* CAurTexture::GetTarget()
{
    if (m_pTarget.m_p)
        return m_pTarget.m_p;
    return this;
}

// FUNCTION: SWKOTOR 0x004205d0
unsigned char CAurTexture::IsSkipOrdering()
{
    CAurTextureBasic* pBasic = GetTarget()->AsBasic();
    if (pBasic)
        return pBasic->m_bSkipOrdering;
    return this != g_pNullTexture.m_p;
}

// FUNCTION: SWKOTOR 0x00420600
unsigned char CAurTexture::IsNull()
{
    return GetTarget() == g_pNullTexture.m_p;
}

// FUNCTION: SWKOTOR 0x00420610
CAurFontInfo::~CAurFontInfo()
{
}

// FUNCTION: SWKOTOR 0x00420670
void CAurTextureBasic::Reset()
{
    FreeImage();
    m_n5c = 0;
    m_n60 = 0;
    m_n64 = 0;
    m_nce = 0;
    m_nAppliedDownsample = 0;
    m_nDownsample = m_nDownsampleMin;
    m_bMemoryDirty = 1;
    m_bBuilt = 0;
    m_be5 = 0;
    m_be6 = 0;
    if (m_p48) {
        delete m_p48;
        m_p48 = 0;
    }
    if (m_pGLNames) {
        glDeleteTextures(m_nNumY * m_nNumX, m_pGLNames);
        delete m_pGLNames;
        m_pGLNames = 0;
        m_nGLName = 0;
    }
}

// FUNCTION: SWKOTOR 0x00420bf0
unsigned int AurTextureGetGLName(CAurTexture* pTexture)
{
    if (pTexture && pTexture->GetTarget() != g_pNullTexture.m_p)
        return pTexture->GetTarget()->AsBasic()->m_nGLName;
    return 0;
}

// FUNCTION: SWKOTOR 0x00420e50
void AurTextureUpdateProcedural()
{
    for (int i = 0; i < g_lHashed.num; i++) {
        CAurTextureBasic* pTexture = g_lHashed.element[i];
        if (pTexture->m_bNeedsUpdate && pTexture->m_pProcedure) {
            pTexture->Bind();
            pTexture->m_bNeedsUpdate = 0;
        }
    }
}

// FUNCTION: SWKOTOR 0x00421310 SYMBOL: ?Allocate@?$CExoArrayList@VVector@@@@QAEXH@Z
template <>
void CExoArrayList<Vector>::Allocate(int n)
{
    Vector* pOld = element;
    array_size = n;
    element = new Vector[n];
    for (int i = 0; i < num; i++)
        element[i] = pOld[i];
    if (pOld)
        delete[] pOld;
}

// FUNCTION: SWKOTOR 0x004213a0 SYMBOL: ?SetSize@?$CExoArrayList@VVector@@@@QAEXH@Z
// FUNCTION: SWKOTOR 0x004213e0 SYMBOL: ?Add@?$CExoArrayList@VVector@@@@QAEXVVector@@@Z
// FUNCTION: SWKOTOR 0x00421fc0 SYMBOL: ??0?$CExoArrayList@VVector@@@@QAE@H@Z
template void CExoArrayList<Vector>::SetSize(int n);
template void CExoArrayList<Vector>::Add(Vector t);
template CExoArrayList<Vector>::CExoArrayList(int n);

// FUNCTION: SWKOTOR 0x00421430
int CAurTexture::SetImage(int a, void* pImage)
{
    CAurTextureBasic* pBasic = GetTarget()->AsBasic();
    int nResult = 0;
    if (pBasic) {
        if (pImage) {
            pBasic->m_pImage = pImage;
            nResult = pBasic->glImage(a);
            pBasic->m_pImage = 0;
            return nResult;
        }
        nResult = pBasic->glImage(a);
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x00421480
void CAurTexture::SetIsEnvironmentMapped(unsigned char b)
{
    CAurTextureBasic* pBasic = GetTarget()->AsBasic();
    if (pBasic) {
        pBasic->m_bEnvironmentMapped = b;
        if (b)
            g_lTextures.AddUnique(pBasic);
    }
}

// FUNCTION: SWKOTOR 0x00421560
void CAurTextureBasic::AddToOrderedLists()
{
    int i = 0;
    for (; i < g_lHashed.num; i++) {
        if (m_nHash <= g_lHashed.element[i]->m_nHash)
            break;
    }
    for (; i < g_lHashed.num; i++) {
        if (g_lHashed.element[i]->m_nHash != m_nHash)
            break;
    }
    g_lHashed.Insert(this, i);
    if (m_bOrderedA)
        g_lOrderedA.Add(this);
    else
        g_lOrderedB.Add(this);
}

#pragma inline_depth(0)
// FUNCTION: SWKOTOR 0x004214b0
void CAurTexture::Bind()
{
    if (g_bTexturesLocked || GetTarget() == g_pNullTexture.m_p) {
        AurTextureBindTID(0, 0);
        return;
    }
    CAurTextureBasic* pBasic = AsBasic();
    if (!pBasic) {
        pBasic = GetTarget()->AsBasic();
        if (!pBasic) {
            AurTextureBindTID(0, 0);
            return;
        }
    }
    g_nLastBoundGLName = AurTextureGetGLName(pBasic);
    AurTextureBindTID(g_nLastBoundGLName, pBasic->IsCube());
    CAurTextureProcedure* pProcedure = pBasic->GetTarget()->AsBasic()->m_pProcedure;
    if (pProcedure && g_bProceduralTextures)
        pProcedure->Update();
    pBasic->m_bNeedsUpdate = 1;
}
#pragma inline_depth()

// FUNCTION: SWKOTOR 0x00421650
void CAurTextureBasic::RemoveFromOrderedLists()
{
    int nFound = 0;
    for (int i = 0; i < g_lHashed.num; i++) {
        if (g_lHashed.element[i] == this)
            nFound++;
    }
    if (nFound)
        g_lHashed.Remove(this);
    if (m_bOrderedA)
        g_lOrderedA.Remove(this);
    else
        g_lOrderedB.Remove(this);
}


// FUNCTION: SWKOTOR 0x00421d50
void AurTextureResetAll()
{
    g_lTextures = g_lHashed;
    for (int i = 0; i < g_lTextures.num; i++)
        g_lTextures.element[i]->Reset();
}

// FUNCTION: SWKOTOR 0x00422360
void AurTextureRefreshAll()
{
    if (g_bAuroraInitialised) {
        AurFreeFont();
        AurInitFont();
    }
    AurTextureSetMem(g_nTextureMem);
}

// FUNCTION: SWKOTOR 0x00422ce0
int AurTextureSetAttributes(CAurTexture* pHolder, char* szText)
{
    if (!pHolder)
        return 0;
    CAurTextureBasic* pBasic = pHolder->GetTarget()->AsBasic();
    pBasic->Init(szText);
    g_lTextures.AddUnique(pBasic);
    return 1;
}

// FUNCTION: SWKOTOR 0x00422d20
int AurTextureMessage(CAurTexture* pHolder, char* szField)
{
    if (!pHolder)
        return 0;
    CAurTextureBasic* pBasic = pHolder->GetTarget()->AsBasic();
    if (!pBasic)
        return 0;
    pBasic->ParseField(szField);
    g_lTextures.AddUnique(pBasic);
    return 1;
}

// --- image building ---------------------------------------------------------

extern "C" __declspec(dllimport) void __stdcall glGenTextures(int n, unsigned int* tex);
extern "C" int __cdecl strcmp(const char* a, const char* b);
extern "C" char* __cdecl strcpy(char* d, const char* s);

extern const unsigned int g_nGLTex2DConst;       // 0x0073f23c
extern const unsigned int g_nGLMagFilterConst;   // 0x0073f364
extern const unsigned int g_nGLMinFilterConst;   // 0x0073f368
extern unsigned int g_nGLTextureCubeMapEnable;   // 0x0078d47c
extern void(__stdcall* g_pglActiveTextureARB)(unsigned int nUnit);  // 0x007bb80c

int AurHasMultiTexture();                        // 0x0045f710
void MakeNormalizationCubeMap(int nSize);        // 0x00425ea0
float AurImageAlphaMean(void* pImage, int nWidth, int nHeight, int nBpp);  // 0x0045e790
void BuildNormalMap(void* pDst, int nWidth, int nHeight, int nOldWidth,
                    int nOldHeight, void* pSrc, int c, int d);           // 0x004260a0
unsigned short AurTextureCalculateHashID(char* szName, unsigned short nCount, int* pInts);  // 0x0041f4d0

// FUNCTION: SWKOTOR 0x00420c20
void CAurTextureBasic::AsNormalizeVectorCubeMap()
{
    m_nDownsample = 0;
    m_nDownsampleMax = 0;
    m_nDownsampleMin = 0;
    m_be2 = 1;
    m_nBytesPerPixel = 3;
    m_bMipmap = 1;
    m_bFilter = 1;
    m_bCube = 1;
    m_nWidth = 0x100;
    m_nHeight = 0x600;
    if (!m_pGLNames)
        m_pGLNames = new unsigned int[1];
    glGenTextures(1, &m_nGLName);
    m_pGLNames[0] = m_nGLName;
    glEnable(g_nGLTextureCubeMapEnable);
    g_nActiveTextureUnit = 1;
    if (AurHasMultiTexture())
        g_pglActiveTextureARB(g_nActiveTextureUnit + 0x84c0);
    unsigned int nTexture = m_nGLName;
    glBindTexture(g_nGLTextureCubeMap, nTexture);
    glTexParameteri(g_nGLTextureCubeMap, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                    g_nAnisotropyRequested);
    g_aBoundTextures[g_nActiveTextureUnit] = nTexture;
    MakeNormalizationCubeMap(0x100);
    glTexParameteri(g_nGLTex2DConst, g_nGLMagFilterConst, 0x2601);
    glTexParameteri(g_nGLTex2DConst, g_nGLMinFilterConst, 0x2703);
    glDisable(g_nGLTextureCubeMap);
    g_nActiveTextureUnit = 0;
    if (AurHasMultiTexture())
        g_pglActiveTextureARB(g_nActiveTextureUnit + 0x84c0);
}

// FUNCTION: SWKOTOR 0x00420db0
void AurTextureCalculateNewTexMem(CExoArrayList<CAurTextureBasic*>* pList, int* pOut)
{
    pOut[4] = 0;
    pOut[2] = 0;
    pOut[3] = 0;
    pOut[1] = 0;
    pOut[0] = 0;
    for (int i = 0; i < pList->num; i++) {
        CAurTextureBasic* pTexture = pList->element[i];
        if (!pTexture->m_bBuilt)
            pTexture->ConstructImage();
        if (!pTexture->m_bTemporary) {
            int aMem[5];
            pTexture->GetMemoryUsage(aMem);
            pOut[4] += aMem[4];
            pOut[2] += aMem[2];
            pOut[3] += aMem[3];
            pOut[1] += aMem[1];
            pOut[0] += aMem[0];
        }
    }
}

// FUNCTION: SWKOTOR 0x00420ea0
int BuildTextureNormalMap(void* pDst, void* pSrc, int* pWidth, int* pHeight,
                          int nUnused, int c, int d)
{
    int nWidth = *pWidth;
    int bBad = 0;
    if (nWidth & (nWidth - 1)) {
        if ((nWidth - 1) & (nWidth - 2))
            bBad = 1;
        else
            *pWidth = nWidth - 1;
    }
    int nHeight = *pHeight;
    if (nHeight & (nHeight - 1)) {
        if ((nHeight - 1) & (nHeight - 2))
            return 0;
        *pHeight = nHeight - 1;
    }
    if (bBad)
        return 0;
    BuildNormalMap(pDst, *pWidth, *pHeight, nWidth, nHeight, pSrc, c, d);
    return 1;
}

// FUNCTION: SWKOTOR 0x004216a0
void CAurTextureBasic::BuildAndStoreTexture()
{
    if (m_be2) {
        if (!strcmp(m_szName, "NormCubeMap"))
            AsNormalizeVectorCubeMap();
        return;
    }
    if (!m_bBuilt)
        ConstructImage();
    if (m_pImage) {
        glImage(0);
        if (m_fAlphaMean == -1.0f) {
            if (m_n5c > 0)
                m_fAlphaMean = 1.0f;
            else
                m_fAlphaMean = AurImageAlphaMean(m_pImage, m_nWidth, m_nHeight,
                                                 m_nBytesPerPixel);
        }
    }
    if (!m_n5c && m_pProcedure)
        m_pProcedure->Init();
    if (!KeepImage())
        FreeImage();
}

// FUNCTION: SWKOTOR 0x00421750
void AurTextureSetLayered(CAurTexture* pHolder, char* szName, int* pInts, int n)
{
    if (!pHolder)
        return;
    CAurTextureBasic* pBasic = pHolder->GetTarget()->AsBasic();
    if (szName && *szName)
        strcpy(pBasic->m_szFileName, szName);
    pBasic->SetScheme(pInts, n);
    pBasic->m_nBytesPerPixel = 4;
    pBasic->m_nHash = AurTextureCalculateHashID(pBasic->m_szFileName,
                                                pBasic->m_nSchemeCount,
                                                pBasic->m_pScheme);
    pBasic->RemoveFromOrderedLists();
    pBasic->AddToOrderedLists();
    g_lTextures.AddUnique(pBasic);
}

// --- memory accounting, upload, lookup -----------------------------------

extern "C" char* __cdecl strstr(const char* s, const char* sub);
extern "C" int __cdecl _stricmp(const char* a, const char* b);
extern "C" __declspec(dllimport) void __stdcall glFinish();

extern short g_nColourDepth;                     // 0x007b9220

int AurS3TCSize(int nWidth, int nHeight, int nBpp);              // 0x0045e270
unsigned char AurNeedFinish();                                   // 0x0045f640
void CreateCubeTexture(CAurTextureBasic* p, int a, int i);       // 0x00423f90
void CreateProcessedCubeTexture(CAurTextureBasic* p, int a, int i);  // 0x00424230
void CreateTexture(CAurTextureBasic* p, int a, int i);           // 0x00424980
void CreateProcessedTexture(CAurTextureBasic* p, int a, int i);  // 0x00424dd0

// Splits the cached memory use into the [total, lightmap, compressed, cube,
// other] counters.
__forceinline void CAurTextureBasic::StoreMemoryUsage(int* pOut)
{
    for (int i = 0; i < 5; i++)
        pOut[i] = 0;
    pOut[0] = m_nMemoryUsage;
    if (strstr(m_szFileName, "_lm") || strstr(m_szFileName, "_a00")) {
        pOut[1] = m_nMemoryUsage;
    } else if (m_bCube) {
        pOut[3] = m_nMemoryUsage;
    } else if (IsLightmap()) {
        pOut[4] = m_nMemoryUsage;
    } else if (m_n5c > 0) {
        pOut[2] = m_nMemoryUsage;
    }
}

// FUNCTION: SWKOTOR 0x00420710
void CAurTextureBasic::GetMemoryUsage(int* pOut)
{
    if (!m_bMemoryDirty) {
        StoreMemoryUsage(pOut);
        return;
    }
    if (!m_bBuilt) {
        for (int i = 0; i < 5; i++)
            pOut[i] = 0;
        return;
    }
    if (this == g_pNullTexture.m_p) {
        for (int i = 0; i < 5; i++)
            pOut[i] = 0;
        return;
    }
    int nShift = m_nDownsample - m_nAppliedDownsample;
    if (m_n5c > 0) {
        m_nMemoryUsage = AurS3TCSize(m_nWidth >> nShift, m_nHeight >> nShift,
                                     m_nBytesPerPixel);
    } else {
        int nWidth = (m_nWidth >> nShift) < 2 ? 2 : (m_nWidth >> nShift);
        int nHeight = (m_nHeight >> nShift) < 2 ? 2 : (m_nHeight >> nShift);
        int nBpp = m_nBytesPerPixel;
        if (g_nColourDepth == 16 && nBpp > 2)
            nBpp = 2;
        else if (nBpp == 3)
            nBpp = 4;
        m_nMemoryUsage = nBpp * nHeight * nWidth;
        if (m_bMipmap)
            m_nMemoryUsage = m_nMemoryUsage * 4 / 3;
    }
    m_bMemoryDirty = 0;
    StoreMemoryUsage(pOut);
}

// FUNCTION: SWKOTOR 0x00420970
unsigned int CAurTextureBasic::glImage(int a)
{
    if (AurNeedFinish())
        glFinish();
    if (!m_pImage) {
        m_nGLName = 0;
        return 0;
    }
    if (!m_nGLName || !m_pGLNames) {
        if (m_pGLNames)
            delete m_pGLNames;
        m_pGLNames = new unsigned int[m_nNumY * m_nNumX];
        glGenTextures(m_nNumY * m_nNumX, m_pGLNames);
        m_nGLName = m_pGLNames[0];
    }
    for (int i = 0; i < m_nNumY * m_nNumX; i++) {
        AurTextureBindTID(m_pGLNames[i], m_bCube);
        if (m_bCube) {
            if (KeepImage())
                CreateProcessedCubeTexture(this, a, i);
            else
                CreateCubeTexture(this, a, i);
        } else if (KeepImage()) {
            CreateProcessedTexture(this, a, i);
        } else {
            if (m_pProcedure)
                m_pProcedure->Prepare();
            CreateTexture(this, a, i);
        }
    }
    if (m_bCube && AurHasCubeMaps())
        glDisable(g_nGLTextureCubeMap);
    return m_nGLName;
}

// Same file name and layer scheme (count checked by the caller too).
__forceinline int CAurTextureBasic::IsScheme(char* szFile, int* pInts, int nCount)
{
    if (nCount != m_nSchemeCount)
        return 0;
    if (_stricmp(szFile, m_szFileName))
        return 0;
    for (int i = 0; i < nCount; i++) {
        if (pInts[i] != m_pScheme[i])
            return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00420ac0
CAurTextureBasic* AurTextureFind(char* szName, char* szFile, int* pInts, int nCount)
{
    if (!szFile) {
        for (int i = 0; i < g_lHashed.num; i++) {
            if (!_stricmp(szName, g_lHashed.element[i]->m_szName))
                return g_lHashed.element[i];
        }
        return 0;
    }
    unsigned short nHash = AurTextureCalculateHashID(szFile, nCount, pInts);
    int i = 0;
    for (; i < g_lHashed.num; i++) {
        if (nHash <= g_lHashed.element[i]->m_nHash)
            break;
    }
    for (; i < g_lHashed.num; i++) {
        CAurTextureBasic* pTexture = g_lHashed.element[i];
        if (nHash != pTexture->m_nHash)
            return 0;
        if (nCount == pTexture->m_nSchemeCount) {
            if (pTexture->IsScheme(szFile, pInts, nCount))
                return pTexture;
        }
    }
    return 0;
}

// --- GL name ordering ---------------------------------------------------------

extern CExoArrayList<int> g_lGLNameOrdering;    // 0x007a478c
extern unsigned int g_nMaxGLName;               // 0x007a46bc
extern int g_nHashedCount;                      // 0x007a4748

void AurTextureSortOrdered(CAurTextureBasic** pList, int nCount);  // 0x0041e9c0

template <>
inline void CExoArrayList<int>::Allocate(int n)
{
    int* pOld = element;
    array_size = n;
    element = new int[n];
    for (int i = 0; i < num; i++)
        element[i] = pOld[i];
    if (pOld)
        delete[] pOld;
}

// FUNCTION: SWKOTOR 0x004217f0
void AurTextureBuildAndStoreAll()
{
    int n = g_lTextures.num;
    if (!n)
        return;
    do {
        CAurTextureBasic* pTexture = g_lTextures.element[n - 1];
        n--;
        pTexture->BuildAndStoreTexture();
        g_lTextures.num = n;
    } while (n);
    AurTextureSortOrdered(g_lOrderedB.element, g_lOrderedB.num);
    g_nHashedCount = g_lHashed.num;
    g_lGLNameOrdering.num = 0;
    g_nMaxGLName = 0;
    for (int i = 0; i < g_lOrderedB.num; i++) {
        CAurTextureBasic* pTexture = g_lOrderedB.element[i];
        if (pTexture->m_bSkipOrdering)
            continue;
        for (int j = 0; j < pTexture->m_nNumY * pTexture->m_nNumX; j++) {
            g_lGLNameOrdering.Add(pTexture->m_pGLNames[j]);
            if (pTexture->m_pGLNames[j] > g_nMaxGLName)
                g_nMaxGLName = pTexture->m_pGLNames[j];
        }
    }
}

// FUNCTION: SWKOTOR 0x00421970
CExoArrayList<int>* AurTextureGetOrdering()
{
    g_lGLNameOrdering.num = 0;
    g_nMaxGLName = 0;
    for (int i = 0; i < g_lOrderedB.num; i++) {
        CAurTextureBasic* pTexture = g_lOrderedB.element[i];
        if (pTexture->m_bSkipOrdering)
            continue;
        for (int j = 0; j < pTexture->m_nNumY * pTexture->m_nNumX; j++) {
            int nName = pTexture->m_pGLNames[j];
            if (nName < 5000 && nName > 0) {
                g_lGLNameOrdering.Add(nName);
                if (pTexture->m_pGLNames[j] > g_nMaxGLName)
                    g_nMaxGLName = pTexture->m_pGLNames[j];
            }
        }
    }
    return &g_lGLNameOrdering;
}

// --- texture memory budget ------------------------------------------------------

extern float g_fTextureMemScale;                // 0x0078d3f0
extern float g_fLightmapMemScale;               // 0x0078d3f8
extern int g_aMemLightmaps[5];                  // 0x007a469c
extern int g_aMemTextures[5];                   // 0x007a46c0
extern int g_nMemTotal;                         // 0x007a46d8
extern int g_nMemLightmaps;                     // 0x007a46dc
extern int g_nMemTextures;                      // 0x007a46e0
extern int g_aMemByKind[4];                     // 0x007a46e4
extern int g_nMemFrame;                         // 0x007a46f4
extern int g_nBindCountPeak;                    // 0x007a46b0
extern int g_nStat724;                          // 0x007a4724
extern int g_nStat728;                          // 0x007a4728
extern int g_nStat72c;                          // 0x007a472c
extern int g_nStat730;                          // 0x007a4730
extern int g_nStatPeak754;                      // 0x007a4754
extern int g_nStatPeak758;                      // 0x007a4758
extern int g_nStatPeak75c;                      // 0x007a475c
extern int g_nStatPeak760;                      // 0x007a4760
extern float g_fFrameTime;                      // 0x0078e574
extern float g_fFrameTimePeak;                  // 0x007a4750

void AurTextureMemCheck(CExoArrayList<CAurTextureBasic*>* pList, int nLimit,
                        int* pOut, float fScale);   // 0x00421ac0

// Rounds to the nearest integer with the current FPU mode.
inline void AurRoundToInt(int* pOut, float f)
{
    __asm {
        fld f
        mov edx, pOut
        frndint
        fistp dword ptr [edx]
    }
}

// FUNCTION: SWKOTOR 0x00421d90
int AurTextureSetMem(int nMem)
{
    g_nTextureMem = nMem;
    AurRoundToInt(&nMem, nMem * g_fTextureMemScale);
    g_lTextures = g_lHashed;
    for (int i = 0; i < g_lTextures.num; i++)
        g_lTextures.element[i]->Reset();
    AurTextureMemCheck(&g_lOrderedA, nMem, g_aMemLightmaps, g_fLightmapMemScale);
    g_nMemLightmaps = g_aMemLightmaps[0];
    AurTextureMemCheck(&g_lOrderedB, g_nTextureMem - nMem, g_aMemTextures, 1.0f);
    g_nMemTextures = g_aMemTextures[0];
    g_aMemByKind[3] = g_aMemTextures[4] + g_aMemLightmaps[4];
    g_aMemByKind[1] = g_aMemTextures[2] + g_aMemLightmaps[2];
    g_aMemByKind[2] = g_aMemTextures[3] + g_aMemLightmaps[3];
    g_aMemByKind[0] = g_aMemTextures[1] + g_aMemLightmaps[1];
    g_nMemTotal = g_aMemTextures[0] + g_nMemLightmaps;
    g_lTextures = g_lHashed;
    AurTextureBuildAndStoreAll();
    return g_nMemTotal <= g_nTextureMem;
}

// FUNCTION: SWKOTOR 0x00421ee0
void AurTextureMakeCurrent()
{
    if (!g_b7a46d5) {
        int nLimit;
        AurRoundToInt(&nLimit, g_nTextureMem * g_fTextureMemScale);
        g_nStat724 = 0;
        g_nStat730 = 0;
        g_nStat728 = 0;
        if (g_lTextures.num) {
            AurTextureBuildAndStoreAll();
            if (g_nStatPeak754 < g_nBindCountPeak)
                g_nStatPeak754 = g_nBindCountPeak;
            if (g_nStatPeak758 < g_nStat72c)
                g_nStatPeak758 = g_nStat72c;
            if (g_nStatPeak75c < g_nStat730)
                g_nStatPeak75c = g_nStat730;
            if (g_nStatPeak760 < g_nStat728)
                g_nStatPeak760 = g_nStat728;
            g_nStat72c = 0;
        }
    }
    if (g_fFrameTime > g_fFrameTimePeak)
        g_fFrameTimePeak = g_fFrameTime;
    g_nMemFrame++;
}

// --- CAurTextureBasic lifetime ---------------------------------------------

extern int g_nTextureCount;                     // 0x007a4718

// FUNCTION: SWKOTOR 0x00422f40
CAurTextureBasic::~CAurTextureBasic()
{
    if (m_pTxi)
        delete m_pTxi;
    delete m_pScheme;
    if (m_pProcedure)
        delete m_pProcedure;
    if (!m_bTemporary) {
        int aMem[5];
        GetMemoryUsage(aMem);
        g_nMemTotal -= aMem[0];
        g_aMemByKind[0] -= aMem[1];
        g_aMemByKind[1] -= aMem[2];
        g_aMemByKind[2] -= aMem[3];
        g_aMemByKind[3] -= aMem[4];
    }
    if (m_pGLNames) {
        glDeleteTextures(m_nNumY * m_nNumX, m_pGLNames);
        delete m_pGLNames;
        m_pGLNames = 0;
        m_nGLName = 0;
    }
    if (m_p48) {
        delete m_p48;
        m_p48 = 0;
    }
    FreeImage();
    g_nTextureCount--;
    RemoveFromOrderedLists();
    int nFound = 0;
    for (int i = 0; i < g_lTextures.num; i++) {
        if (g_lTextures.element[i] == this)
            nFound++;
    }
    if (nFound)
        g_lTextures.Remove(this);
}

// Finds or creates the texture for a name and layer scheme.
__forceinline CAurTextureBasic* AurTextureGetBasic(char* szName, char* szBuffer,
                                                  char* szFile, int* pInts,
                                                  int nCount)
{
    if (szFile && *szFile)
        strcpy(szBuffer, szFile);
    else
        strcpy(szBuffer, szName);
    CAurTextureBasic* pBasic = AurTextureFind(szName, szBuffer, pInts, nCount);
    if (pBasic)
        return pBasic;
    pBasic = new CAurTextureBasic(szName, szBuffer);
    pBasic->Init(pBasic->m_szFileName);
    g_lTextures.Add(pBasic);
    return pBasic;
}

// FUNCTION: SWKOTOR 0x00423490
CAurTexture* AurTextureGet(char* szName, CAurTexture* pTarget, char* szFile,
                           int* pInts, int nCount)
{
    char szBuffer[32];
    szBuffer[0] = 0;
    if (!pTarget)
        return AurTextureGetBasic(szName, szBuffer, szFile, pInts, nCount);
    CAurTexture* pTexture = new CAurTexture;
    pTexture->m_pTarget = pTarget;
    return pTexture;
}
