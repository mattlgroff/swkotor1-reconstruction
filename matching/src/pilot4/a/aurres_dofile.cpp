// Pilot 4 arm A: AurRes dofile (0x0044C980).
#include "pilot4/a/p4a_aurres.h"

// FUNCTION: SWKOTOR 0x0044C980
char* AurResDoFile(const char* p)
{
    char szExt[16];
    char szBase[64];
    int n = 0;
    char c = *p;
    while (c != 0 && c != '.') {
        p++;
        szBase[n] = c;
        c = *p;
        n++;
    }
    szBase[n] = 0;
    n = 0;
    c = *p;
    while (c != 0) {
        szExt[n++] = c;
        c = *++p;
    }
    szExt[n] = 0;
    AurResFile* pDesc = AurResOpen(szBase, szExt, 0, 0);
    if (!pDesc) {
        sprintf(g_szAurResDoFileMessage, "ERROR:  file '%s' doesn't exist", p);
        return g_szAurResDoFileMessage;
    }
    for (;;) {
        AurResFile* pEntry = g_AurResOpenFiles.Find(pDesc);
        if (!pEntry) {
            break;
        }
        char* szLine;
        if (pEntry->m_pFile) {
            AurResFile* pFile = AurResGetFile(pEntry);
            pFile->m_nLine++;
            if (!pFile->m_pScratch) {
                pFile->m_pScratch = new char[pFile->m_nScratchSize];
                pFile->m_pScratch[0] = 0;
            }
            szLine = fgets(pFile->m_pScratch, 4000, pFile->m_pFile);
        } else {
            szLine = AurResReadMemoryLine();
        }
        if (!szLine) {
            break;
        }
        while (*szLine == ' ' || *szLine == '\t') {
            szLine++;
        }
        if (*szLine == 0 || *szLine == '#') {
            continue;
        }
        char* q = szLine + strlen(szLine) - 1;
        while ((int)q >= (int)szLine) {
            char t = *q;
            if (t != '\n' && t != '\r' && t != '\t' && t != ' ') {
                break;
            }
            *q = 0;
            q--;
        }
        AurResInterpret(szLine);
    }
    AurResClose(pDesc, 0);
    sprintf(g_szAurResDoFileMessage, "Done executing file '%s'", p);
    return g_szAurResDoFileMessage;
}
