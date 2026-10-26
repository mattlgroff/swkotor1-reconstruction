// Texture replacement on a texture reference (0x004235f0). The SafePointer
// assignment stays out of line here; the clear is expanded.
#define G_SAFE_POINTER_ASSIGN_OUT_OF_LINE
#include "g/aur_texture.h"

extern "C" int __cdecl _stricmp(const char* a, const char* b);
extern "C" char* __cdecl strcpy(char* d, const char* s);
#pragma intrinsic(strcpy)

void __cdecl AurTextureRelease(CAurTexture* pTexture);      // 0x00420d70
CAurTexture* AurTextureGet(char* szName, CAurTexture* pTarget, char* szFile,
                           int* pInts, int nCount);         // 0x00423490

// FUNCTION: SWKOTOR 0x004235f0
int AurTextureReplaceSingleReference(CAurTexture* pTexture, char* szOld, char* szNew,
                                     char* szFile, int* pInts, int nCount)
{
    if (!pTexture)
        return 0;
    CAurTextureBasic* pOld = pTexture->GetTarget()->AsBasic();
    if (!pOld)
        return 0;
    if (szOld && _stricmp(szOld, pOld->m_szName))
        return 0;
    unsigned char bEnvironmentMapped = pTexture->IsEnvironmentMapped();
    if (!pOld->m_bOrderedA)
        g_bTextureModeDynamic = 0;
    bool bReleased = false;
    CAurTexture* pNew;
    if (szFile) {
        char szName[32];
        strcpy(szName, pOld->m_szName);
        if (!_stricmp(szFile, pOld->m_szFileName)) {
            pTexture->m_pTarget.Clear();
            AurTextureRelease(pOld);
            bReleased = true;
        }
        pNew = AurTextureGet(szName, 0, szFile, pInts, nCount);
    } else if (szOld) {
        if (!_stricmp(szOld, pOld->m_szName)) {
            pTexture->m_pTarget.Clear();
            AurTextureRelease(pOld);
            bReleased = true;
            pNew = AurTextureGet(szOld, 0, szNew, pInts, nCount);
        } else {
            g_bTextureModeDynamic = 1;
            return 0;
        }
    } else {
        pNew = AurTextureGet(pOld->m_szName, 0, szNew, pInts, nCount);
    }
    CAurTextureBasic* pNewBasic = pNew->GetTarget()->AsBasic();
    if (pNewBasic) {
        pNewBasic->m_bEnvironmentMapped = bEnvironmentMapped;
        if (bEnvironmentMapped)
            g_lTextures.AddUnique(pNewBasic);
    }
    CAurTextureBasic* pBasic = pNew->AsBasic();
    g_bTextureModeDynamic = 1;
    if (!bReleased && pBasic == pOld->AsBasic())
        return 0;
    pTexture->m_pTarget = pNew;
    if (!bReleased)
        AurTextureRelease(pOld);
    if (pInts && pBasic->m_bBuilt)
        return 0;
    return 1;
}
