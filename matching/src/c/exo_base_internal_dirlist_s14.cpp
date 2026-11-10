// CExoBaseInternal::GetDirectoryList (exobaseinternal.o, worker C s14). The
// CExoArrayList members are only declared here so that Add/Insert stay calls.

#include <string.h>

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    CExoString(const CExoString& s);                // 0x005E5B00
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString& operator=(const char* s);           // 0x005E5140
    char* CStr() const;                             // 0x005E5670
    int GetLength() const;                          // 0x005E5790
    char operator[](int n) const;                   // 0x005E5600
    void Format(const char* format, ...);           // 0x005E5680
    int operator==(const char* s) const;            // 0x005E5290
    int operator<(const CExoString& s) const;       // 0x005E5410
    int operator>=(const CExoString& s) const;      // 0x005E5580
    CExoString LowerCase() const;                   // 0x005E6080
};

class CExoAliasList {
public:
    CExoString ResolveFileName(const CExoString& sFileName, unsigned short nResType);   // 0x005E68A0
};

class CExoBase {
public:
    CExoString* GetResourceExtension(unsigned short nType);   // 0x005E6660

    void* m_pcExoIni;                               // 0x00
    void* m_pcExoTimers;                            // 0x04
    void* m_pcExoDebug;                             // 0x08
    CExoAliasList* m_pcExoAliasList;                // 0x0c
};

extern CExoBase* g_pExoBase;                        // 0x007A39E0

struct FILETIME {
    unsigned long dwLowDateTime;
    unsigned long dwHighDateTime;
};

struct WIN32_FIND_DATAA {
    unsigned long dwFileAttributes;                 // 0x00
    FILETIME ftCreationTime;                        // 0x04
    FILETIME ftLastAccessTime;                      // 0x0c
    FILETIME ftLastWriteTime;                       // 0x14
    unsigned long nFileSizeHigh;                    // 0x1c
    unsigned long nFileSizeLow;                     // 0x20
    unsigned long dwReserved0;                      // 0x24
    unsigned long dwReserved1;                      // 0x28
    char cFileName[260];                            // 0x2c
    char cAlternateFileName[14];                    // 0x130
};

extern "C" {
__declspec(dllimport) void* __stdcall FindFirstFileA(const char* sFileName, WIN32_FIND_DATAA* pData);   // IAT 0x0073D1B4
__declspec(dllimport) int __stdcall FindNextFileA(void* hFind, WIN32_FIND_DATAA* pData);   // IAT 0x0073D1B8
__declspec(dllimport) int __stdcall FindClose(void* hFind);   // IAT 0x0073D1BC
__declspec(dllimport) unsigned long __stdcall GetLastError();   // IAT 0x0073D208
__declspec(dllimport) long __stdcall CompareFileTime(const FILETIME* pTime1, const FILETIME* pTime2);   // IAT 0x0073D188
}

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    CExoArrayList()
    {
        num = 0;
        array_size = 0;
        element = 0;
    }
    ~CExoArrayList()
    {
        delete[] element;
    }
    void Add(T t);                                  // <CExoString>: 0x004059A0
    void Insert(T t, int pos);                      // <FILETIME>: 0x005E8BF0, <CExoString>: 0x005E8C60
};

class CExoBaseInternal {
public:
    int GetDirectoryList(CExoArrayList<CExoString>* pList, const CExoString& sDirectory,
                         unsigned short nResType, int bDirectories, int nSortMode);
};

// Lists the files (or, with bDirectories, the subdirectories) of an aliased
// directory; nSortMode 1 keeps the names sorted (binary search), 2 sorts the
// files by last write time, 0 keeps the FindNextFile order.
// NONMATCH (52 insns): same blocks, slots and EH states except that the
// original spills both binary-search bounds (nHigh at esp+0x28) and keeps
// the constant 0 in edi; ours keeps nHigh in edi and 0 in esi.
// FUNCTION: SWKOTOR 0x005E8CF0
int CExoBaseInternal::GetDirectoryList(CExoArrayList<CExoString>* pList, const CExoString& sDirectory,
                                       unsigned short nResType, int bDirectories, int nSortMode)
{
    CExoString sPath;
    CExoString sName;
    CExoString sExtension;
    WIN32_FIND_DATAA cFindData;
    sExtension = *g_pExoBase->GetResourceExtension(nResType);
    if (sExtension == "" || bDirectories) {
        sExtension = "*";
    }
    sPath = g_pExoBase->m_pcExoAliasList->ResolveFileName(sDirectory, 0xffff);
    if (sPath[sPath.GetLength() - 1] == '\\') {
        sPath.Format("%s*.%s", sPath.CStr(), sExtension.CStr());
    } else {
        sPath.Format("%s\\*.%s", sPath.CStr(), sExtension.CStr());
    }
    void* hFind = FindFirstFileA(sPath.CStr(), &cFindData);
    CExoArrayList<FILETIME> lstTimes;
    if (hFind != (void*)-1) {
        unsigned long nError = 0;
        do {
            sName = cFindData.cFileName;
            CExoString sLower = sName.LowerCase();
            int i;
            if (bDirectories) {
                if ((cFindData.dwFileAttributes & 0x10) && strcmp(sName.CStr(), ".") && strcmp(sName.CStr(), "..")) {
                    if (nSortMode) {
                        for (i = 0; i < pList->num; i++) {
                            if (pList->element[i].LowerCase() >= sLower) {
                                break;
                            }
                        }
                        pList->Insert(sName, i);
                    } else {
                        pList->Add(sName);
                    }
                }
            } else if (!(cFindData.dwFileAttributes & 0x10)) {
                if (nSortMode == 1) {
                    int nLow = 0;
                    int nHigh = pList->num - 1;
                    i = (nLow + nHigh) / 2;
                    while (nLow < nHigh) {
                        if (pList->element[i].LowerCase() < sLower) {
                            nLow = i + 1;
                        } else {
                            nHigh = i;
                        }
                        i = (nLow + nHigh) / 2;
                    }
                    if (nHigh >= 0 && pList->element[i].LowerCase() < sLower) {
                        pList->Insert(sName, i + 1);
                    } else {
                        pList->Insert(sName, i);
                    }
                } else if (nSortMode == 2) {
                    FILETIME* pTime = lstTimes.element;
                    for (i = 0; i < pList->num; i++, pTime++) {
                        if (CompareFileTime(&cFindData.ftLastWriteTime, pTime) == 1) {
                            break;
                        }
                    }
                    lstTimes.Insert(cFindData.ftLastWriteTime, i);
                    pList->Insert(sName, i);
                } else {
                    pList->Add(sName);
                }
            }
            if (!FindNextFileA(hFind, &cFindData)) {
                nError = GetLastError();
            }
        } while (nError != 18);
        FindClose(hFind);
        return 1;
    }
    return 0;
}
