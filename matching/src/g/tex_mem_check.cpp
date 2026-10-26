// Texture memory budget: down-samples the largest textures until the
// list fits (0x00421ac0).
#include "g/aur_texture.h"

extern float g_fFrameTime;                      // 0x0078e574
extern float g_fTexMemOverTime;                 // 0x007a474c
extern float g_fTexMemMaxOverTime;              // 0x0078d3fc

void AurTextureCalculateNewTexMem(CExoArrayList<CAurTextureBasic*>* pList, int* pMem);  // 0x00420db0

// FUNCTION: SWKOTOR 0x00421ac0
// NONMATCH (66): register tie only. The original keeps pMem in edi and
// the level count in ebp (ours swaps them), and jumps over the loop
// alignment padding. Tried: per-scope usage arrays, a loop for the
// five-way update.
void AurTextureMemCheck(CExoArrayList<CAurTextureBasic*>* pList, int nLimit,
                        int* pMem, float fScale)
{
    AurTextureCalculateNewTexMem(pList, pMem);
    int nUsed = pMem[0];
    float fSoftLimit = nLimit * fScale;
    bool bForce = false;
    if (nUsed > fSoftLimit) {
        g_fTexMemOverTime += g_fFrameTime;
        if (g_fTexMemOverTime > g_fTexMemMaxOverTime)
            bForce = true;
    } else {
        g_fTexMemOverTime = 0;
    }
    if (!bForce && nUsed <= nLimit)
        return;
    g_fTexMemOverTime = 0;
    while (nUsed > fSoftLimit) {
        CAurTextureBasic* pVictim = 0;
        int nBestMem = 0;
        int nBestLevels = 0;
        int aMem[5];
        for (int i = 0; i < pList->num; i++) {
            CAurTextureBasic* pTexture = pList->element[i];
            short nDownsample = pTexture->m_nDownsample;
            int nShift = nDownsample - pTexture->m_nAppliedDownsample;
            if (nDownsample < pTexture->m_nDownsampleMax &&
                (pTexture->m_nWidth >> nShift) > 2 &&
                (pTexture->m_nHeight >> nShift) > 2) {
                int nLevels = pTexture->m_nDownsampleMax - nDownsample;
                pTexture->GetMemoryUsage(aMem);
                if (aMem[0] > nBestMem || (aMem[0] == nBestMem && nLevels > nBestLevels)) {
                    nBestLevels = nLevels;
                    nBestMem = aMem[0];
                    pVictim = pTexture;
                }
            }
        }
        if (!pVictim)
            return;
        pVictim->GetMemoryUsage(aMem);
        nUsed -= aMem[0];
        pMem[0] -= aMem[0];
        pMem[1] -= aMem[1];
        pMem[2] -= aMem[2];
        pMem[3] -= aMem[3];
        pMem[4] -= aMem[4];
        int nNew = pVictim->m_nDownsample + 1;
        if (nNew >= pVictim->m_nDownsampleMax)
            nNew = pVictim->m_nDownsampleMax;
        pVictim->m_nDownsample = nNew;
        if (pVictim->m_nDownsample != pVictim->m_nAppliedDownsample) {
            pVictim->m_bMemoryDirty = 1;
            int nFound = 0;
            for (int j = 0; j < g_lTextures.num; j++) {
                if (g_lTextures.element[j] == pVictim)
                    nFound++;
            }
            if (!nFound)
                g_lTextures.Add(pVictim);
        }
        pVictim->GetMemoryUsage(aMem);
        nUsed += aMem[0];
        pMem[0] += aMem[0];
        pMem[1] += aMem[1];
        pMem[2] += aMem[2];
        pMem[3] += aMem[3];
        pMem[4] += aMem[4];
    }
}
