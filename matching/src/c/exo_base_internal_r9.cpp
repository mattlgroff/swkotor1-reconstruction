// CExoBaseInternal leftovers (b005e7, r9): AddAlias, GetResourceExtension,
// CExoArrayList<CExoString>::Insert.

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    CExoString(const CExoString& s);                // 0x005E5B00
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString& operator=(const char* s);           // 0x005E5140
    char* CStr() const;                             // 0x005E5670
    int GetLength() const;                          // 0x005E5790
    char operator[](int n) const;                   // 0x005E5600
    void Format(const char* format, ...);           // 0x005E5680
};

class CExoIni {
public:
    int ReadIniEntry(CExoString& sValue, const CExoString& sFile, const CExoString& sSection,
                     const CExoString& sKey);       // 0x005E67C0
};

class CExoAliasList {
public:
    int Add(const CExoString& sAlias, const CExoString& sPath);   // 0x005E6880
    CExoString ResolveFileName(const CExoString& sFileName, unsigned short nResType);   // 0x005E68A0
};

class CExoBase {
public:
    CExoIni* m_pcExoIni;                            // 0x00
    void* m_pcExoTimers;                            // 0x04
    void* m_pcExoDebug;                             // 0x08
    CExoAliasList* m_pcExoAliasList;                // 0x0c
};

extern CExoBase* g_pExoBase;                        // 0x007A39E0

class CExoBaseInternal {
public:
    int m_nResTypes;                                // 0x00
    unsigned short* m_pResTypes;                    // 0x04
    CExoString* m_pResTypeExtensions;               // 0x08

    void AddAlias(const CExoString& sIniFile, const CExoString& sAlias, const CExoString& sDefault,
                  const CExoString& sDescription);
    CExoString* GetResourceExtension(unsigned short nType);
    int GetDirectorySize(const CExoString& sDirectory);
};

// FUNCTION: SWKOTOR 0x005E7760
void CExoBaseInternal::AddAlias(const CExoString& sIniFile, const CExoString& sAlias,
                                const CExoString& sDefault, const CExoString& sDescription)
{
    CExoString sPath;
    bool bMissing = !g_pExoBase->m_pcExoIni->ReadIniEntry(sPath, sIniFile, "Alias", sAlias);
    if (bMissing) {
        sPath = sDefault;
    }
    g_pExoBase->m_pcExoAliasList->Add(sAlias, sPath);
}

// FUNCTION: SWKOTOR 0x005E7A00
CExoString* CExoBaseInternal::GetResourceExtension(unsigned short nType)
{
    for (int i = 0; i < m_nResTypes; i++) {
        if (m_pResTypes[i] == nType) {
            return &m_pResTypeExtensions[i];
        }
    }
    return &m_pResTypeExtensions[0x57];
}

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
}

// FUNCTION: SWKOTOR 0x005E7820
int CExoBaseInternal::GetDirectorySize(const CExoString& sDirectory)
{
    CExoString sPath;
    CExoString sFile;
    CExoString sUnused;
    int nSize = 0;
    WIN32_FIND_DATAA cFindData;
    sPath = g_pExoBase->m_pcExoAliasList->ResolveFileName(sDirectory, 0xffff);
    if (sPath[sPath.GetLength() - 1] == '\\') {
        sPath.Format("%s*.*", sPath.CStr());
    } else {
        sPath.Format("%s\\*.*", sPath.CStr());
    }
    void* hFind = FindFirstFileA(sPath.CStr(), &cFindData);
    if (hFind != (void*)-1) {
        unsigned long nError = 0;
        do {
            sFile = cFindData.cFileName;
            nSize += cFindData.nFileSizeLow;
            if (!FindNextFileA(hFind, &cFindData)) {
                nError = GetLastError();
            }
        } while (nError != 18);
        FindClose(hFind);
        return nSize;
    }
    return 0;
}

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    void Add(T t);                                  // 0x004059A0 for CExoString
    void Insert(T t, int pos);
};

// FUNCTION: SWKOTOR 0x005E8C60 SYMBOL: ?Insert@?$CExoArrayList@VCExoString@@@@QAEXVCExoString@@H@Z
template <class T>
void CExoArrayList<T>::Insert(T t, int pos)
{
    int i = num;
    Add(t);
    for (; i > pos; i--) {
        element[i] = element[i - 1];
    }
    element[pos] = t;
}

template void CExoArrayList<CExoString>::Insert(CExoString t, int pos);
