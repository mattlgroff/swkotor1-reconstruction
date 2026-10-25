// Exo foundation leftovers: CExoStringList::GetString/Add,
// CExoLinkedListInternal::GetPrev, CExoLocString::Compare/GetStringLoc,
// CExoFileInternal::ReadAsyncComplete, CExoTimersInternal::
// GetHighResolutionTimer, CExoRandInternal::GenSeed (b005e7, b005e9).

#include <string.h>

union LARGE_INTEGER { struct { unsigned long LowPart; long HighPart; } u; __int64 QuadPart; };
struct SYSTEM_INFO { unsigned long a[9]; };
extern "C" {
__declspec(dllimport) int __stdcall QueryPerformanceCounter(LARGE_INTEGER* p);   // IAT 0x0073D1A8
__declspec(dllimport) void __stdcall GetSystemInfo(SYSTEM_INFO* p);              // IAT 0x0073D168
__declspec(dllimport) unsigned long __stdcall GetTickCount();                    // IAT 0x0073D0F8
__declspec(dllimport) int __stdcall MessageBoxA(void* hWnd, const char* sText, const char* sCaption, unsigned int nType);  // IAT 0x0073D418
}

class CExoDebug {
public:
    void Warning(int nLine, char* sFile, char* sMessage);   // 0x005E9400
};

class CExoBase {
public:
    void* m_pcExoIni;                               // 0x00
    void* m_pcExoTimers;                            // 0x04
    CExoDebug* m_pcExoDebug;                        // 0x08
};

extern CExoBase* g_pExoBase;                        // 0x007A39E0

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    char* CStr() const;                             // 0x005E5670
};

class CExoStringList {
public:
    CExoString** m_pStrings;                        // 0x00
    int m_nCount;                                   // 0x04
    int m_nDuplicateMode;                           // 0x08
    int m_bSorted;                                  // 0x0c
    int m_nSize;                                    // 0x10

    CExoString* GetString(int i);
    void Add(CExoString* pString);
    void AddSorted(CExoString* pString);            // 0x005E95F0
    void Introduce(CExoString* pString, int nPosition);   // 0x005E94A0
};

// FUNCTION: SWKOTOR 0x005E95E0
CExoString* CExoStringList::GetString(int i)
{
    return m_pStrings[i];
}

// FUNCTION: SWKOTOR 0x005E98B0
void CExoStringList::Add(CExoString* pString)
{
    int nCount = m_nCount;
    int bAdd = 1;
    if (m_bSorted == 0) {
        AddSorted(pString);
        return;
    }
    if (m_nDuplicateMode == 1) {
        for (int i = 0; i < m_nCount; i++) {
            if (strcmp(pString->CStr(), m_pStrings[i]->CStr()) == 0) {
                i = m_nCount;
                bAdd = 0;
            }
        }
        if (!bAdd) {
            return;
        }
    } else if (m_nDuplicateMode == 2) {
        MessageBoxA(0, "Duplicate string found.", "Error: ExoStringList", 1);
        return;
    }
    Introduce(pString, nCount);
}

typedef void* CExoLinkedListPosition;

struct CExoLinkedListNode {
    CExoLinkedListNode* pPrev;                      // 0x00
    CExoLinkedListNode* pNext;                      // 0x04
    void* pObject;                                  // 0x08
};

class CExoLinkedListInternal {
public:
    CExoLinkedListNode* pHead;                      // 0x00
    CExoLinkedListNode* pTail;                      // 0x04
    unsigned long nCount;                           // 0x08

    void* GetPrev(CExoLinkedListPosition& pos);
};

// FUNCTION: SWKOTOR 0x005E9BD0
void* CExoLinkedListInternal::GetPrev(CExoLinkedListPosition& pos)
{
    if (pos == 0) {
        g_pExoBase->m_pcExoDebug->Warning(0x119, "exolinkedlistinternal.cpp",
            "CExoLinkedListInternal::GetPreviousg() Position paramter is NULL");
        return 0;
    }
    CExoLinkedListNode* pPrev = ((CExoLinkedListNode*)pos)->pPrev;
    if (pPrev == 0) {
        pos = pPrev;
        return 0;
    }
    pos = pPrev;
    return pPrev->pObject;
}

class CExoLocString;

class CExoLocStringInternal {
public:
    void* m_lstLocStrings;                          // 0x00
    unsigned long m_nCount;                         // 0x04
    unsigned long m_nUnknown8;                      // 0x08

    int CompareStrings(const CExoLocString& o);     // 0x005EC7F0 RET 4
    int GetString(int nIndex, CExoString& s);       // 0x005EC9C0 RET 8
};

class CExoLocString {
public:
    CExoLocStringInternal* m_pcExoLocStringInternal;   // 0x00
    unsigned long m_dwStringRef;                    // 0x04

    int Compare(const CExoLocString& o);
    int GetStringLoc(int nLanguage, CExoString& s, unsigned char nGender);
};

// FUNCTION: SWKOTOR 0x005E9F10
int CExoLocString::Compare(const CExoLocString& o)
{
    // NONMATCH (4 diff insns): the original tests each branch's count in its
    // own register (ecx / eax) and joins at one `jne`; ours loads both into
    // ecx and tests once. Per-branch `return 0` duplicates the exit instead.
    CExoLocStringInternal* p = m_pcExoLocStringInternal;
    if (p->m_nUnknown8 != 0 && o.m_pcExoLocStringInternal->m_nUnknown8 != 0) {
        return m_dwStringRef == o.m_dwStringRef;
    }
    unsigned long nCount;
    if (p->m_nUnknown8 != 0) {
        nCount = o.m_pcExoLocStringInternal->m_nCount;
    } else if (o.m_pcExoLocStringInternal->m_nUnknown8 != 0) {
        nCount = p->m_nCount;
    } else {
        if (p->CompareStrings(o) && m_dwStringRef == o.m_dwStringRef) {
            return 1;
        }
        return 0;
    }
    if (nCount != 0) {
        return 0;
    }
    return m_dwStringRef == o.m_dwStringRef;
}

// FUNCTION: SWKOTOR 0x005EA240
int CExoLocString::GetStringLoc(int nLanguage, CExoString& s, unsigned char nGender)
{
    CExoLocStringInternal* p = m_pcExoLocStringInternal;
    unsigned char nIndex = nLanguage ? nGender : 0;
    return p->GetString(nLanguage * 2 + nIndex, s);
}

extern int g_bAsyncReadComplete;                    // 0x008328BC

class CExoFileInternal {
public:
    int ReadAsyncComplete();
};

// FUNCTION: SWKOTOR 0x005EBC50
int CExoFileInternal::ReadAsyncComplete()
{
    return g_bAsyncReadComplete;
}

class CExoTimersInternal {
public:
    int m_bHighResolution;                          // 0x00
    unsigned long m_nUnknown4;                      // 0x04
    char pad_0x8[0x10];
    double m_fScale;                                // 0x18

    unsigned long GetLowResolutionTimer();
    unsigned __int64 GetHighResolutionTimer();
};

// FUNCTION: SWKOTOR 0x005EC200
unsigned long CExoTimersInternal::GetLowResolutionTimer()
{
    return GetTickCount();
}

// FUNCTION: SWKOTOR 0x005EC210
unsigned __int64 CExoTimersInternal::GetHighResolutionTimer()
{
    // NONMATCH (5 diff insns): the original has a dead `mov eax, [esi+4]`
    // between fild and fmul before the tail jmp to _ftol.
    if (m_bHighResolution == 1) {
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        return (unsigned __int64)(li.QuadPart * m_fScale);
    }
    return (unsigned __int64)GetTickCount() * 1000;
}

class CExoRandInternal {
public:
    unsigned long m_z[4];                           // 0x00
    unsigned long m_c[4];                           // 0x10
    unsigned long m_ks[4];                          // 0x20
    unsigned long m_k[4];                           // 0x30
    unsigned long m_q[4];                           // 0x40
    unsigned long m_s[4];                           // 0x50
    int m_bSeeded;                                  // 0x60
    unsigned long* m_pState;                        // 0x64

    void GenSeed();
    void AddEntropy(void* p, unsigned long n);      // 0x005ECE20 RET 8
};

// FUNCTION: SWKOTOR 0x005ED080
void CExoRandInternal::GenSeed()
{
    unsigned long aNoise[0x20];
    LARGE_INTEGER li;
    SYSTEM_INFO si;
    memcpy(m_pState, aNoise, sizeof(aNoise));
    if (QueryPerformanceCounter(&li)) {
        AddEntropy(&li, 8);
    }
    GetSystemInfo(&si);
    AddEntropy(&si, sizeof(si));
}
