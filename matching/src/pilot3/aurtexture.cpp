// Pilot group 3: CAurTextureBasic::LoadImage (exported as LoadImageA because
// the original source was compiled with <windows.h>'s LoadImage macro).

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void *GetImage(char *szName, int *pWidth, int *pHeight, int *pBpp, int *pDataSize,
               int *pMipMaps, int *pFormat, int *pType, int *pTXI,
               unsigned char *pAlpha, char *szEnvMap, int bSingle);

typedef void (*ImageHookFn)(void *pData, int nWidth, int nHeight, int nBpp, int nUnused,
                            int nMipMaps, int nFormat, unsigned char bAlpha);
extern ImageHookFn g_pfnImageHook;

class CAurTextureBasic {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual unsigned char IsCubeMap();          // +0x0c
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9c();
    virtual void va0(); virtual void va4(); virtual void va8(); virtual void vac();
    virtual void PreLoad();                     // +0xb0

    void *LoadImageA();

    char pad_0x4[0x1c];
    int m_nTXI;              // +0x20
    char pad_0x24[0x10];
    int m_n34;               // +0x34
    char pad_0x38[0xc];
    void *m_pData;           // +0x44
    char pad_0x48[0x14];
    int m_nDataSize;         // +0x5c
    int m_nMipMaps;          // +0x60
    int m_nFormat;           // +0x64
    int m_nWidth;            // +0x68
    int m_nHeight;           // +0x6c
    int m_nBpp;              // +0x70
    int m_nType;             // +0x74
    char m_szResName[0x20];  // +0x78
    char m_szName[0x2a];     // +0x98
    short m_nC2;             // +0xc2
    short m_nC4;             // +0xc4
    short m_nFrames;         // +0xc6
    char pad_0xc8[6];
    char m_szEnvMap[0xd];    // +0xce
    char m_bDB;              // +0xdb
    char pad_0xdc[7];
    char m_bHooked;          // +0xe3
    char pad_0xe4;
    unsigned char m_bAlpha;  // +0xe5
};

static inline bool ImageHookEnabled()
{
    return g_pfnImageHook != 0;
}

// FUNCTION: SWKOTOR 0x0041F6A0
void *CAurTextureBasic::LoadImageA()
{
    PreLoad();
    if (m_szName[0] == 0)
        strcpy(m_szName, m_szResName);

    if (m_nFrames != 0) {
        void **pFrames = (void **)malloc(m_nFrames * 4);
        m_nHeight = 0;
        for (int i = 0; i < m_nFrames; i++) {
            int nWidth;
            int nDataSize;
            int nHeight;
            int nBpp;
            char szFrame[128];
            pFrames[i] = 0;
            sprintf(szFrame, "%s_%d", m_szName, i);
            pFrames[i] = GetImage(szFrame, &nWidth, &nHeight, &nBpp, &nDataSize,
                                  &m_nMipMaps, &m_nFormat, &m_nType, &m_nTXI, &m_bAlpha,
                                  m_szEnvMap, 0);
            if (!pFrames[i]) {
                sprintf(szFrame, "%s%d", m_szName, i);
                pFrames[i] = GetImage(szFrame, &nWidth, &nHeight, &nBpp, &nDataSize,
                                      &m_nMipMaps, &m_nFormat, &m_nType, &m_nTXI, &m_bAlpha,
                                      m_szEnvMap, 0);
            }
            m_nWidth = nWidth;
            m_nBpp = nBpp;
            m_nHeight = nHeight;
        }

        m_pData = malloc(m_nFrames * m_nWidth * m_nHeight * m_nBpp);
        char *pDest = (char *)m_pData;
        for (int j = 0; j < m_nFrames; j++) {
            memcpy(pDest, pFrames[j], m_nWidth * m_nHeight * m_nBpp);
            pDest += m_nWidth * m_nHeight * m_nBpp;
            // NOTE: original materialises this test as a bool (mov cl,1 / jne / xor cl,cl)
            // and hoists pFrames[j] before the branch; not reproduced yet.
            if (ImageHookEnabled())
                g_pfnImageHook(pFrames[j], m_nWidth, m_nHeight, m_nBpp, 0, m_nMipMaps, m_nFormat, m_bAlpha);
            else
                free(pFrames[j]);
        }
        if (!IsCubeMap())
            m_nC4 = m_nFrames;
        m_bHooked = 0;
        free(pFrames);
        m_nHeight = m_nFrames * m_nHeight;
        return m_pData;
    }

    int bSingle = 0;
    if (!m_bDB && m_nC4 * m_nC2 == 1 && m_n34 == 0)
        bSingle = 1;
    m_pData = GetImage(m_szName, &m_nWidth, &m_nHeight, &m_nBpp, &m_nDataSize,
                       &m_nMipMaps, &m_nFormat, &m_nType, &m_nTXI, &m_bAlpha,
                       m_szEnvMap, bSingle);
    if (g_pfnImageHook && m_pData) {
        m_bHooked = 1;
        return m_pData;
    }
    // NOTE: original reloads m_pData here (mov eax,[ebp+0x44]); ours reuses eax.
    m_bHooked = 0;
    return m_pData;
}
