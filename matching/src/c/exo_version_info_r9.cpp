// Reads the executable's version resource into four global strings
// (0x005E6B10, called once from WinMain's setup; worker C r9).

#include "c/exo_base.h"

extern "C" {
__declspec(dllimport) unsigned long __stdcall GetModuleFileNameA(void* hModule, char* sFileName, unsigned long nSize);   // IAT 0x0073D18C
unsigned long __stdcall GetFileVersionInfoSizeA(const char* sFileName, unsigned long* pHandle);   // 0x0070C13E
int __stdcall GetFileVersionInfoA(const char* sFileName, unsigned long nHandle, unsigned long nLen, void* pData);   // 0x0070C138
int __stdcall VerQueryValueA(const void* pBlock, const char* sSubBlock, void** ppBuffer, unsigned int* pLen);   // 0x0070C132
}

extern void* g_hInstance;                           // 0x007A39C0
extern CExoString g_sPrivateBuild;                  // 0x007A3CC4
extern CExoString g_sInternalName;                  // 0x007A3CCC
extern CExoString g_sProductVersion;                // 0x007A3CD4
extern CExoString g_sLegalCopyright;                // 0x007A3CDC

// FUNCTION: SWKOTOR 0x005E6B10
void LoadVersionInfo()
{
    // NONMATCH (38 diff insns): esi/edi swapped for nSize/pData only.
    char sFileName[0x100];
    unsigned long nHandle = 0;
    void* pValue;
    unsigned int nLen;
    GetModuleFileNameA(g_hInstance, sFileName, 0x100);
    unsigned long nSize = GetFileVersionInfoSizeA(sFileName, &nHandle);
    if (nSize) {
        char* pData = new char[nSize];
        GetFileVersionInfoA(sFileName, nHandle, nSize, pData);
        if (VerQueryValueA(pData, "\\StringFileInfo\\040904B0\\PrivateBuild", &pValue, &nLen)) {
            g_sPrivateBuild = CExoString((char*)pValue);
        } else {
            g_sPrivateBuild = "";
        }
        if (VerQueryValueA(pData, "\\StringFileInfo\\040904B0\\ProductVersion", &pValue, &nLen)) {
            g_sProductVersion = CExoString((char*)pValue);
        } else {
            g_sProductVersion = "";
        }
        if (VerQueryValueA(pData, "\\StringFileInfo\\040904B0\\InternalName", &pValue, &nLen)) {
            g_sInternalName = CExoString((char*)pValue);
        } else {
            g_sInternalName = "";
        }
        if (VerQueryValueA(pData, "\\StringFileInfo\\040904B0\\LegalCopyright", &pValue, &nLen)) {
            g_sLegalCopyright = CExoString((char*)pValue);
        } else {
            g_sLegalCopyright = "";
        }
        if (pData) {
            delete pData;
        }
    }
}
