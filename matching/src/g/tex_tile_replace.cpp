// Replaces a texture on the tile references around a position (0x004237e0).
// This unit expands SafePointer assignment and CExoArrayList::Remove.
#define G_EXO_ARRAY_LIST_REMOVE
#define G_EXO_ARRAY_LIST_REMOVE_FORCEINLINE
#define G_SAFE_POINTER_ASSIGN_FORCEINLINE
#include "g/aur_texture.h"

class Scene;

CAurTextureBasic* AurTextureFind(char* szName, char* szFile, int* pInts, int nCount);  // 0x00420ac0
CAurTexture* AurTextureGet(char* szName, CAurTexture* pTarget, char* szFile,
                           int* pInts, int nCount);                 // 0x00423490
void __cdecl AurTextureRelease(CAurTexture* pTexture);              // 0x00420d70
void __cdecl SceneBSPGetTileTextures(Scene* pScene, void* pPosition,
                                     CExoArrayList<CAurTexture*>* pList);   // 0x0046c380

// Deletes a texture nobody points at any more (as in aur_texture_ref.cpp,
// without the null test).
inline void ReleaseUnreferencedTexture(CAurTexture* pTexture)
{
    if (pTexture->m_lRefs.num == 0) {
        if (pTexture->AsBasic())
            delete pTexture->AsBasic();
        else
            delete pTexture;
    }
}

// FUNCTION: SWKOTOR 0x004237e0
// NONMATCH (115): flow and calls agree; the original keeps zero in ebp and
// the old texture in ebx, spills the new texture and the inner counter
// (frame 0x20 vs 0x24), lays the found branch out right after the search
// loop and tail-merges the early release with the list cleanup. Tried an
// index variable, `continue`, forced inlining of operator= / Remove.
int AurTextureReplaceTileReferences(Scene* pScene, void* pPosition, char* szNew, char* szOld)
{
    CAurTextureBasic* pFound = AurTextureFind(szOld, szOld, 0, 0);
    if (!pFound)
        return 0;
    CAurTexture* pBasic = pFound->AsBasic();
    if (!((CAurTextureBasic*)pBasic)->m_bOrderedA)
        g_bTextureModeDynamic = 0;
    CAurTexture* pNew = AurTextureGet(szNew, 0, 0, 0, 0);
    g_bTextureModeDynamic = 1;
    if (pNew->IsSkipOrdering()) {
        AurTextureRelease(pNew);
        return 0;
    }
    CExoArrayList<CAurTexture*> lTiles;
    CExoArrayList<CAurTexture*> lMatched;
    SceneBSPGetTileTextures(pScene, pPosition, &lTiles);
    for (int i = 0; i < lTiles.num; i++) {
        if (lTiles.element[i] && lTiles.element[i]->m_pTarget.m_p == pBasic) {
            lMatched.AddUnique(lTiles.element[i]);
            if (lMatched.num <= 0)
                return 0;
            for (int j = 0; j < lMatched.num; j++) {
                lMatched.element[j]->m_pTarget = pNew;
                ReleaseUnreferencedTexture(pBasic);
            }
            return 1;
        }
    }
    return 0;
}
