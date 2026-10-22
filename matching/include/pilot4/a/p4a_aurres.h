// Pilot 4 arm A: AurRes script file descriptors (dofile, 0x0044C980).
#ifndef P4A_AURRES_H
#define P4A_AURRES_H

#include <stdio.h>
#include <string.h>

struct AurResFile {
    FILE* m_pFile;                                      // 0x00
    int m_nLine;                                        // 0x04
    char* m_pScratch;                                   // 0x08
    char pad_0xc[0x14 - 0xc];
    int m_nScratchSize;                                 // 0x14
};

struct AurResFileList {
    AurResFile** m_pData;                               // 0x007B8FD8
    int m_nCount;                                       // 0x007B8FDC

    AurResFile* Find(AurResFile* pFile)
    {
        for (int i = 0; i < m_nCount; i++) {
            if (pFile == m_pData[i]) {
                return m_pData[i];
            }
        }
        return 0;
    }
};

extern AurResFileList g_AurResOpenFiles;                // 0x007B8FD8
extern char g_szAurResDoFileMessage[];                  // 0x007B8FE8

AurResFile* AurResOpen(const char* szBase, const char* szExt, int a, int b);   // 0x0044C740
void AurResClose(AurResFile* pFile, int a);             // 0x0044C8F0
AurResFile* AurResGetFile(AurResFile* pFile);           // 0x0044BD70
char* AurResReadMemoryLine();                           // 0x0044BE70
void AurResInterpret(char* szLine);                     // 0x0044C1F0
char* AurResDoFile(const char* szFileName);             // 0x0044C980

#endif
