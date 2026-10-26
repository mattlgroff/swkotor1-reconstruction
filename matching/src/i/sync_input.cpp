// The model reader behind IODispatcher::ReadSync (aurio.o; the Mac debug
// map calls it (anonymous namespace)::Input). Named SyncInput to match the
// call sites in iodispatcher_read_sync.cpp.
#include "i/input_binary.h"
#include <stdio.h>

extern "C" size_t __cdecl strlen(const char* s);

AurRes* AurResOpen(const char* szName, const char* szExt, int* pSize, int bFlag);  // 0x0044C740
AurRes* AurResPushFile(FILE* pFile);                                               // 0x0044C870
char* AurResDoFile(const char* szName);                                            // 0x0044C980
char* AurResGetNextLine();                                                         // 0x0044BFA0
void AurResInterpret(char* szLine);                                                // 0x0044C1F0

extern int g_nModelsRead;                   // 0x007B932C
extern MaxTree* g_pCurrentTree;             // 0x007FBAE4

class SyncInput : public InputBinary {
public:
    MaxTree* Read(char* szName);
    MaxTree* Read(FILE* pMdl, FILE* pMdx);
};

// NONMATCH: register tie (szName in ebx, the header pointer in edi).
// FUNCTION: SWKOTOR 0x004A13A0 SYMBOL: ?Read@SyncInput@@QAEPAVMaxTree@@PAD@Z
MaxTree* SyncInput::Read(char* szName)
{
    g_nModelsRead++;
    AurRes* pMdl = AurResOpen(szName, ".mdl", 0, 1);
    AurRes* pMdx = AurResOpen(szName, ".mdx", 0, 1);
    if (!pMdl)
        return 0;
    char* pHead = AurResGetDataBytes(4, pMdl);
    if (!pHead)
        return 0;
    if (*pHead == 0) {
        AurResFreeDataBytes(pMdl, pHead);
        return InputBinary::Read(pMdl, pMdx, false)->AsModel();
    }
    if (pMdx)
        AurResClose(pMdx, 0);
    AurResFreeDataBytes(pMdl, pHead);
    AurResClose(pMdl, 0);
    char szFile[0x80];
    sprintf(szFile, "%s%s", szName, ".mdl");
    MaxTree* pSaved = g_pCurrentTree;
    AurResDoFile(szFile);
    MaxTree* pTree = g_pCurrentTree;
    g_pCurrentTree = pSaved;
    return pTree;
}

// NONMATCH: merged `return 0` block placement (original puts it after the
// first test).
// FUNCTION: SWKOTOR 0x004A14B0 SYMBOL: ?Read@SyncInput@@QAEPAVMaxTree@@PAU_iobuf@@0@Z
MaxTree* SyncInput::Read(FILE* pMdlFile, FILE* pMdxFile)
{
    AurRes* pMdl = AurResPushFile(pMdlFile);
    if (!pMdl)
        return 0;
    char* pHead = AurResGetDataBytes(4, pMdl);
    if (!pHead)
        return 0;
    if (*pHead == 0) {
        AurResFreeDataBytes(pMdl, pHead);
        AurRes* pMdx = AurResPushFile(pMdxFile);
        return InputBinary::Read(pMdl, pMdx, false)->AsModel();
    }
    AurResFreeDataBytes(pMdl, pHead);
    MaxTree* pSaved = g_pCurrentTree;
    char* szLine;
    while ((szLine = AurResGetNextLine()) != 0) {
        while (*szLine == ' ' || *szLine == '\t')
            szLine++;
        if (*szLine == 0 || *szLine == '#')
            continue;
        char* pEnd = szLine + strlen(szLine) - 1;
        while (pEnd >= szLine && (*pEnd == '\n' || *pEnd == '\r' || *pEnd == '\t' || *pEnd == ' ')) {
            *pEnd = 0;
            pEnd--;
        }
        AurResInterpret(szLine);
    }
    AurResClose(pMdl, 0);
    MaxTree* pTree = g_pCurrentTree;
    g_pCurrentTree = pSaved;
    return pTree;
}
