// CExoResMan key-table lookups.
#include "d/exo_key_table.h"

struct MEMORYSTATUS {
    unsigned long dwLength;
    unsigned long dwMemoryLoad;
    unsigned long dwTotalPhys;
    unsigned long dwAvailPhys;
    unsigned long dwTotalPageFile;
    unsigned long dwAvailPageFile;
    unsigned long dwTotalVirtual;
    unsigned long dwAvailVirtual;
};

extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long dwMilliseconds);
extern "C" __declspec(dllimport) void __stdcall GlobalMemoryStatus(MEMORYSTATUS* pBuffer);
extern "C" __declspec(dllimport) void* __stdcall CreateThread(void* pAttributes, unsigned long nStackSize,
                                                             unsigned long(__stdcall* pStart)(void*), void* pParam,
                                                             unsigned long nFlags, unsigned long* pThreadID);
extern "C" __declspec(dllimport) unsigned long __stdcall ResumeThread(void* hThread);
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObject(void* hHandle, unsigned long nMs);
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* hObject);

extern "C" __declspec(dllimport) int __stdcall CreateDirectoryA(const char* pPath, void* pAttributes);
extern "C" __declspec(dllimport) void* __stdcall GetModuleHandleA(const char* pModule);
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* hModule, const char* pName);
extern "C" __declspec(dllimport) int __stdcall GetDiskFreeSpaceExA(const char* pPath, unsigned __int64* pFreeToCaller,
                                                                  unsigned __int64* pTotal, unsigned __int64* pTotalFree);

struct _stat {
    unsigned int st_dev;
    unsigned short st_ino;
    unsigned short st_mode;
    short st_nlink;
    short st_uid;
    short st_gid;
    unsigned int st_rdev;
    long st_size;
    long st_atime;
    long st_mtime;
    long st_ctime;
};
extern "C" int __cdecl _stat(const char* pPath, struct _stat* pStat);
extern "C" int __cdecl _chmod(const char* pPath, int nMode);
extern "C" int __cdecl remove(const char* pPath);

unsigned long __stdcall AsyncLoaderThread(void* pParam);    // 0x00409b90
void InitializeCRCTable();                                  // 0x00410520

// Debug trace compiled out in the retail build; its arguments are still evaluated.
inline void ResManTrace(const char*, const char*, const char*, const char*)
{
}

// FUNCTION: SWKOTOR 0x004071A0
int CExoResMan::GetKeyEntryFromTable(CExoLinkedList<CExoKeyTable>* pList, const CResRef& cResRef, unsigned short nType,
                                     CExoKeyTable** ppTable, CExoKeyEntry** ppEntry, unsigned long nFilter)
{
    CExoLinkedListPosition pos = pList->GetHeadPos();
    if (pos) {
    CExoKeyTable* pTable = pList->GetAtPos(pos);
    while (pos) {
        if ((nFilter == 0 || pTable->m_n08 == nFilter) && pTable && !pTable->m_bRebuilding) {
            CExoKeyEntry* pEntry = pTable->FindKey(cResRef, nType);
            if (pEntry) {
                *ppTable = pTable;
                *ppEntry = pEntry;
                return 1;
            }
        }
        pTable = pList->GetNext(pos);
    }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00407230
int CExoResMan::GetKeyEntry(const CResRef& cResRef, unsigned short nType, CExoKeyTable*& pTable, CExoKeyEntry*& pEntry)
{
    if (GetKeyEntryFromTable(&m_lstOverrideKeyTables, cResRef, nType, &pTable, &pEntry, 0)) {
        return 1;
    }
    if (GetKeyEntryFromTable(&m_lstEncapsulatedKeyTables, cResRef, nType, &pTable, &pEntry, 1)) {
        return 1;
    }
    if (GetKeyEntryFromTable(&m_lstDirectoryKeyTables, cResRef, nType, &pTable, &pEntry, 0)) {
        return 1;
    }
    if (GetKeyEntryFromTable(&m_lstEncapsulatedKeyTables, cResRef, nType, &pTable, &pEntry, 2)) {
        return 1;
    }
    if (GetKeyEntryFromTable(&m_lstFixedKeyTables, cResRef, nType, &pTable, &pEntry, 0)) {
        return 1;
    }
    pTable = 0;
    pEntry = 0;
    return 0;
}

// NONMATCH: 3 insns; ecx = pTable is set after pushing nType
// FUNCTION: SWKOTOR 0x004072E0
CExoStringList* CExoResMan::GetResOfTypeFromTable(CExoLinkedList<CExoKeyTable>* pList, unsigned short nType)
{
    CExoStringList* pResult = 0;
    CExoLinkedListPosition pos = pList->GetHeadPos();
    if (pos) {
        CExoKeyTable* pTable = pList->GetAtPos(pos);
        while (pos) {
            if (pTable && !pTable->m_bRebuilding) {
                CExoStringList* pKeys = pTable->GetKeysOfType(nType);
                if (pResult) {
                    CExoStringList* pMerged = pResult->Merge(pKeys, 0, 1);
                    delete pResult;
                    if (pKeys) {
                        delete pKeys;
                    }
                    pResult = pMerged;
                } else {
                    pResult = pKeys;
                }
            }
            pTable = pList->GetNext(pos);
        }
    }
    return pResult;
}

// FUNCTION: SWKOTOR 0x004076E0
CExoKeyTable* CExoResMan::GetTable(CRes* pRes)
{
    CExoLinkedList<CExoKeyTable>* pList;
    switch (pRes->m_nID >> 30) {
    case 3:
        pList = &m_lstOverrideKeyTables;
        break;
    case 2:
        pList = &m_lstEncapsulatedKeyTables;
        break;
    case 0:
        pList = &m_lstFixedKeyTables;
        break;
    case 1:
        pList = &m_lstDirectoryKeyTables;
        break;
    }
    CExoLinkedListPosition pos = pList->GetHeadPos();
    if (pos == 0) {
        return 0;
    }
    CExoKeyTable* pTable = pList->GetAtPos(pos);
    while (pos) {
        if (pTable && (pTable->m_nTableID & 0x0fffffff) == ((pRes->m_nID >> 14) & 0x3f)) {
            return pTable;
        }
        pTable = pList->GetNext(pos);
    }
    return 0;
}

struct RES_PREFIX {
    char m_aData[6];
};

// FUNCTION: SWKOTOR 0x00406540
void CExoResMan::FreeResourceData(CRes* pRes)
{
    if (pRes && pRes->m_pResource) {
        if ((pRes->m_nID & 0xc0000000) != 0x40000000) {
            char* pData = (char*)pRes->m_pResource;
            delete[] (pRes->m_bHeaderPrefix ? pData - sizeof(RES_PREFIX) : pData);
            m_nAvailableMemory += pRes->m_nSize;
        }
        pRes->m_nSize = 0;
        pRes->m_pResource = 0;
    }
}

// NONMATCH: 0 insns (1 branch target); original does not thread the empty-list `je` past the loop test
// FUNCTION: SWKOTOR 0x00407020
int CExoResMan::Free(CRes* pRes)
{
    if (pRes && pRes->m_pResource) {
        CExoLinkedListPosition pos = m_lstToBeFreed.GetHeadPos();
        CRes* pCur = m_lstToBeFreed.GetHead();
        while (pCur) {
            if (pCur == pRes) {
                pRes->OnResourceFreed();
                if (pRes->m_nStatus & 0x10) {
                    return 0;
                }
                m_lstToBeFreed.Remove(pos);
                FreeResourceData(pRes);
                pRes->m_nStatus &= ~0x114;
                return 1;
            }
            pCur = m_lstToBeFreed.GetNext(pos);
        }
    }
    return 0;
}

// NONMATCH: 19 insns; head ternary materialised with `xor eax,eax; jmp` in the original, prologue order differs
// FUNCTION: SWKOTOR 0x004070a0
int CExoResMan::FreeChunk()
{
    CExoLinkedListPosition pos = m_lstToBeFreed.GetHeadPos();
    unsigned long nFreed = 0;
    CRes* pRes = m_lstToBeFreed.GetHead();
    while (pRes && m_lstToBeFreed.GetHeadPos() && nFreed < 0x100000) {
        if (pRes->m_nStatus & 0x10) {
            pRes = m_lstToBeFreed.GetNext(pos);
        } else {
            if (pRes->m_nDemands > 0) {
                pRes->m_nStatus &= ~0x100;
            } else {
                pRes->OnResourceFreed();
                nFreed -= m_nAvailableMemory;
                if (pRes->m_pResource) {
                    if ((pRes->m_nID & 0xc0000000) != 0x40000000) {
                        char* pData = (char*)pRes->m_pResource;
                        if (pRes->m_bHeaderPrefix) {
                            pData -= sizeof(RES_PREFIX);
                        }
                        delete[] pData;
                        m_nAvailableMemory += pRes->m_nSize;
                    }
                    pRes->m_nSize = 0;
                    pRes->m_pResource = 0;
                }
                nFreed += m_nAvailableMemory;
                pRes->m_nStatus &= ~0x114;
            }
            CExoLinkedListPosition posOld = pos;
            pRes = m_lstToBeFreed.GetNext(pos);
            m_lstToBeFreed.Remove(posOld);
        }
    }
    return nFreed > 0;
}

// FUNCTION: SWKOTOR 0x00407680
void CExoResMan::SetResObject(const CResRef& cResRef, unsigned short nType, CRes* pRes)
{
    CExoKeyEntry* pEntry = 0;
    CExoKeyTable* pTable;
    if (pRes && GetKeyEntry(cResRef, nType, pTable, pEntry) && !pEntry->m_pRes) {
        pEntry->m_pRes = pRes;
        pRes->m_pKeyEntry = pEntry;
        pEntry->m_pRes->m_nRefs = 1;
        pEntry->m_pRes->m_nID = pEntry->m_nID;
    }
}

// FUNCTION: SWKOTOR 0x004077b0
int CExoResMan::Malloc(CRes* pRes)
{
    while (pRes->m_nSize > m_nAvailableMemory && FreeChunk()) {
    }
    m_nAvailableMemory -= pRes->m_nSize;
    if (pRes->m_bHeaderPrefix) {
        pRes->m_pResource = new char[pRes->m_nSize + 6] + 6;
    } else if (pRes->m_bLargePrefix) {
        pRes->m_pResource = new char[pRes->m_nSize + 10];
    } else {
        pRes->m_pResource = new char[pRes->m_nSize];
    }
    if (!pRes->m_pResource) {
        m_nAvailableMemory += pRes->m_nSize;
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00408b90
void CExoResMan::Dump(CRes* pRes, int bRemove)
{
    if (pRes) {
        if (pRes->m_nDemands > 0) {
            pRes->m_nStatus |= 0x100;
            return;
        }
        if ((pRes->m_nStatus & 0x14) && pRes->m_pResource) {
            Free(pRes);
        }
    }
}

// FUNCTION: SWKOTOR 0x00408c00
int CExoResMan::Exists(const CResRef& cResRef, unsigned short nType, unsigned long nTableKind)
{
    CExoLinkedList<CExoKeyTable>* pList;
    switch (nTableKind) {
    case 1:
        pList = &m_lstFixedKeyTables;
        break;
    case 2:
        pList = &m_lstOverrideKeyTables;
        break;
    case 3:
        pList = &m_lstEncapsulatedKeyTables;
        break;
    case 4:
        pList = &m_lstDirectoryKeyTables;
        break;
    default:
        return 0;
    }
    if (!pList) {
        return 0;
    }
    CExoKeyTable* pTable;
    CExoKeyEntry* pEntry;
    return GetKeyEntryFromTable(pList, cResRef, nType, &pTable, &pEntry, 0);
}

// FUNCTION: SWKOTOR 0x00408c60 SYMBOL: ?GetResOfType@CExoResMan@@QAEPAVCExoStringList@@GPAVCRes@@@Z
CExoStringList* CExoResMan::GetResOfType(unsigned short nType, CRes* pRes)
{
    CExoKeyTable* pTable = GetTable(pRes);
    CExoStringList* pResult = 0;
    if (pTable && !pTable->m_bRebuilding) {
        pResult = pTable->GetKeysOfType(nType);
    }
    return pResult;
}

// FUNCTION: SWKOTOR 0x00408e30
int CExoResMan::ReadRaw(CRes* pRes, int nSize, char* pBuffer)
{
    switch (pRes->m_nID >> 30) {
    case 3:
        return ServiceFromDirectoryRaw(pRes, nSize, pBuffer);
    case 2:
        return ServiceFromEncapsulatedRaw(pRes, nSize, pBuffer);
    case 1:
        return ServiceFromImageRaw(pRes, nSize, pBuffer);
    case 0:
        return ServiceFromResFileRaw(pRes, nSize, pBuffer);
    }
    return (int)pBuffer;
}

// FUNCTION: SWKOTOR 0x00409cf0
unsigned long CExoResMan::ReleaseResObject(CRes* pRes)
{
    if (pRes) {
        if (pRes->m_nRefs > 1) {
            return --pRes->m_nRefs;
        }
        pRes->m_nRefs = 0;
        if (pRes->m_nDemands <= 0) {
            if (pRes->m_pKeyEntry) {
                pRes->m_pKeyEntry->m_pRes = 0;
            }
            pRes->Dump(1);
            if (pRes->m_nStatus & 8) {
                pRes->m_nStatus |= 0x200;
            }
        }
    }
    return 0;
}


// FUNCTION: SWKOTOR 0x00407830
int CExoResMan::RemoveKeyTable(const CExoString& sName, unsigned long nTableKind)
{
    CExoLinkedList<CExoKeyTable>* pList;
    switch (nTableKind) {
    case 1:
        pList = &m_lstFixedKeyTables;
        break;
    case 2:
        pList = &m_lstOverrideKeyTables;
        break;
    case 3:
        pList = &m_lstEncapsulatedKeyTables;
        break;
    case 4:
        pList = &m_lstDirectoryKeyTables;
        break;
    default:
        return 0;
    }
    CExoLinkedListPosition pos = pList->GetHeadPos();
    if (pos) {
        CExoKeyTable* pTable = pList->GetAtPos(pos);
        while (pos) {
            if (pTable && pTable->m_sName == sName) {
                pTable->DestroyTable();
                pList->Remove(pos);
                delete pTable;
                return 1;
            }
            pTable = pList->GetNext(pos);
        }
    }
    return 0;
}

// NONMATCH: 28 insns; failure return laid out after the GetTable test in the original, ours at the end
// FUNCTION: SWKOTOR 0x004088f0
int CExoResMan::CancelRequest(CRes* pRes)
{
    if (!pRes || (pRes->m_nStatus & 4) || pRes->m_nRequests == 0) {
        return 0;
    }
    if (--pRes->m_nRequests == 0 && (pRes->m_nStatus & 0x10)) {
        CExoKeyTable* pTable = GetTable(pRes);
        if (!pTable) {
            return 0;
        }
        switch (pRes->m_nID >> 30) {
        case 3:
            if (m_pAsyncFile && pRes->m_nRequests == 0) {
                delete m_pAsyncFile;
            }
            break;
        case 2:
        case 1:
            pTable->m_ppResFile[0]->DelRefs();
            break;
        case 0:
            pTable->m_ppResFile[(pRes->m_nID >> 20) & 0x3ff]->DelRefs();
            break;
        }
        m_pCurrentAsyncRes = 0;
        m_pAsyncFile = 0;
    }
    CExoLinkedListPosition pos = m_lstAsyncResQueue.GetHeadPos();
    CRes* pCur = m_lstAsyncResQueue.GetHead();
    while (pos) {
        if (pCur == pRes) {
            if (pRes->m_nRequests == 0) {
                m_lstAsyncResQueue.Remove(pos);
            }
            break;
        }
        pCur = m_lstAsyncResQueue.GetNext(pos);
    }
    return pRes->m_nRequests;
}

// NONMATCH: 10 insns; same unthreaded empty-list `je` as Free
// FUNCTION: SWKOTOR 0x004089f0
void* CExoResMan::Demand(CRes* pRes)
{
    CExoLinkedListPosition pos;
    if (!pRes || pRes->m_nID == 0xffffffff) {
        return 0;
    }
    if ((pRes->m_nStatus & 4) && pRes->m_pResource) {
        if (pRes->m_nDemands == 0 && (pRes->m_nStatus & 0x100)) {
            pos = m_lstToBeFreed.GetHeadPos();
            CRes* pCur = m_lstToBeFreed.GetHead();
            while (pCur) {
                if (pCur == pRes) {
                    m_lstToBeFreed.Remove(pos);
                    pRes->m_nStatus &= ~0x100;
                    break;
                }
                pCur = m_lstToBeFreed.GetNext(pos);
            }
        }
        pRes->m_nDemands++;
        return pRes->m_pResource;
    }
    if (pRes->m_nStatus & 0x10) {
        if (!m_pAsyncFile) {
            return pRes->m_pResource;
        }
        while (!m_pAsyncFile->ReadAsyncComplete()) {
            Sleep(5);
        }
        if (!ServiceCurrentAsyncRes()) {
            return 0;
        }
        pRes->m_nDemands++;
        return pRes->m_pResource;
    }
    int bResult;
    switch (pRes->m_nID >> 30) {
    case 3:
        bResult = ServiceFromDirectory(pRes, 0);
        break;
    case 2:
        bResult = ServiceFromEncapsulated(pRes, 0);
        break;
    case 0:
        bResult = ServiceFromResFile(pRes, 0);
        break;
    case 1:
        bResult = ServiceFromImage(pRes, 0);
        break;
    }
    if (!bResult) {
        return 0;
    }
    pos = m_lstAsyncResQueue.GetHeadPos();
    CRes* pCur = m_lstAsyncResQueue.GetHead();
    while (pos) {
        if (pCur == pRes) {
            m_lstAsyncResQueue.Remove(pos);
            break;
        }
        pCur = m_lstAsyncResQueue.GetNext(pos);
    }
    pRes->m_nDemands++;
    return pRes->m_pResource;
}

// FUNCTION: SWKOTOR 0x00408c90
int CExoResMan::Release(CRes* pRes)
{
    if (!pRes) {
        return -1;
    }
    {
        if ((pRes->m_nID & 0xc0000000) == 0x40000000) {
            GetTable(pRes)->m_ppResFile[0]->DelRefs();
        }
        if (pRes->m_nDemands > 1) {
            return pRes->m_nDemands--;
        }
        if (pRes->m_nDemands == 0) {
            return -1;
        }
        {
            pRes->m_nDemands = 0;
            if (pRes->m_nStatus & 0x100) {
                Dump(pRes, 0);
                if (pRes->m_pResource) {
                    m_lstToBeFreed.AddHead(pRes);
                    Dump(pRes, 0);
                }
                pRes->m_nStatus &= ~0x100;
                return 0;
            }
            m_lstToBeFreed.AddHead(pRes);
            pRes->m_nStatus |= 0x100;
            return 0;
        }
    }
    return -1;
}

// NONMATCH: 33 insns; original keeps 0 in ebx and the queue internal in ecx, RemoveHead path laid out first
// FUNCTION: SWKOTOR 0x00408d40
void CExoResMan::Update(int nUnused)
{
    if (m_bAsyncSuspended == 0 && m_lstAsyncResQueue.GetHeadPos()) {
        if (!m_pCurrentAsyncRes) {
            m_pCurrentAsyncRes = m_lstAsyncResQueue.GetHead();
        }
        CRes* pRes = m_pCurrentAsyncRes;
        unsigned long nID = pRes->m_nID;
        if (nID == 0xffffffff || (pRes->m_nStatus & 4)) {
            m_lstAsyncResQueue.RemoveHead();
            m_pCurrentAsyncRes = 0;
            m_pAsyncFile = 0;
            return;
        }
        if (pRes->m_nStatus & 0x10) {
            if (m_pAsyncFile && m_pAsyncFile->ReadAsyncComplete()) {
                ServiceCurrentAsyncRes();
            }
            return;
        }
        int bResult;
        switch (nID >> 30) {
        case 3:
            bResult = ServiceFromDirectory(pRes, 1);
            break;
        case 2:
            bResult = ServiceFromEncapsulated(pRes, 1);
            break;
        case 0:
            bResult = ServiceFromResFile(pRes, 1);
            break;
        case 1:
            bResult = ServiceFromImage(pRes, 1);
            break;
        }
        if (bResult) {
            CRes* pCur = m_pCurrentAsyncRes;
            if (!(pCur->m_nStatus & 4)) {
                pCur->m_nStatus |= 0x10;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004064f0
void CExoResMan::AddResourceImageFilesAsync(const CExoString& sName, unsigned long a, unsigned long b)
{
    while (m_bImageAddBusy == 1) {
    }
    m_csAsync.EnterCriticalSection();
    m_sImageName = sName;
    m_n54 = a;
    m_n58 = b;
    m_bImageAddBusy = 1;
    m_b5c = 0;
    ResumeThread(m_hThread);
    m_csAsync.LeaveCriticalSection();
}

// NONMATCH: 3 insns; original shares the "" CResRef temporary slot with the SubString temporary (frame 0x20 vs 0x28)
// FUNCTION: SWKOTOR 0x004065a0
void CExoResMan::GetResRefFromFile(CResRef& cResRef, const CExoString& sFile)
{
    int nDot = sFile.Find('.', 0);
    if (nDot <= 0) {
        cResRef = "";
        return;
    }
    if (nDot > 16) {
        nDot = 16;
    }
    cResRef = sFile.SubString(0, nDot);
}

// FUNCTION: SWKOTOR 0x00406650
unsigned short CExoResMan::GetResTypeFromFile(const CExoString& sFile)
{
    CExoString sExtension;
    int nDot = sFile.Find('.', 0);
    if (nDot < 0) {
        return 0xffff;
    }
    sExtension = sFile.SubString(nDot + 1, 3);
    return g_pExoBase->GetResTypeFromExtension(sExtension);
}

// FUNCTION: SWKOTOR 0x00407390 SYMBOL: ?GetResOfType@CExoResMan@@QAEPAVCExoStringList@@GH@Z
CExoStringList* CExoResMan::GetResOfType(unsigned short nType, int bOnlyEncapsulated)
{
    CExoStringList* pList = GetResOfTypeFromTable(&m_lstEncapsulatedKeyTables, nType);
    if (bOnlyEncapsulated) {
        return pList;
    }
    CExoStringList* pNew;
    if (pList) {
        pNew = GetResOfTypeFromTable(&m_lstDirectoryKeyTables, nType);
        CExoStringList* pMerged = pList->Merge(pNew, 0, 1);
        delete pList;
        if (pNew) {
            delete pNew;
        }
        pList = pMerged;
    } else {
        pList = GetResOfTypeFromTable(&m_lstDirectoryKeyTables, nType);
    }
    if (pList) {
        pNew = GetResOfTypeFromTable(&m_lstOverrideKeyTables, nType);
        CExoStringList* pMerged = pList->Merge(pNew, 0, 1);
        delete pList;
        if (pNew) {
            delete pNew;
        }
        pList = pMerged;
    } else {
        pList = GetResOfTypeFromTable(&m_lstOverrideKeyTables, nType);
    }
    if (pList) {
        pNew = GetResOfTypeFromTable(&m_lstFixedKeyTables, nType);
        CExoStringList* pMerged = pList->Merge(pNew, 0, 1);
        delete pList;
        if (pNew) {
            delete pNew;
        }
        pList = pMerged;
    } else {
        pList = GetResOfTypeFromTable(&m_lstFixedKeyTables, nType);
    }
    return pList;
}

// FUNCTION: SWKOTOR 0x00408670
CExoResMan::~CExoResMan()
{
    m_bShutdown = 1;
    ResumeThread(m_hThread);
    if (m_hThread != (void*)-1) {
        WaitForSingleObject(m_hThread, 0xffffffff);
        CloseHandle(m_hThread);
    }
}

// FUNCTION: SWKOTOR 0x00408830
int CExoResMan::RemoveResourceImageFile(const CExoString& sName)
{
    CExoLinkedListPosition pos = m_lstDirectoryKeyTables.GetHeadPos();
    if (pos) {
        CExoKeyTable* pTable = m_lstDirectoryKeyTables.GetAtPos(pos);
        while (pos) {
            if (pTable && pTable->m_sName == sName) {
                pTable->DestroyTable();
                m_lstDirectoryKeyTables.Remove(pos);
                delete pTable;
                return 1;
            }
            pTable = m_lstDirectoryKeyTables.GetNext(pos);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00409bf0
CExoResMan::CExoResMan()
{
    InitializeCRCTable();
    MEMORYSTATUS ms;
    m_pCurrentAsyncRes = 0;
    m_bAsyncSuspended = 0;
    m_b34 = 0;
    GlobalMemoryStatus(&ms);
    m_nTotalPhysicalMemory = ms.dwTotalPhys;
    long nBudget = (long)ms.dwTotalPhys >> 1;
    if (nBudget > 0x1000000) {
        m_nMemoryBudget = nBudget;
        m_nAvailableMemory = nBudget;
    } else {
        m_nMemoryBudget = 0x1000000;
        m_nAvailableMemory = 0x1000000;
    }
    m_bImageAddBusy = 0;
    m_b5c = 0;
    m_bShutdown = 0;
    int nParam = 0;
    m_hThread = CreateThread(0, 0x10000, AsyncLoaderThread, &nParam, 4, &m_nThreadID);
}

// FUNCTION: SWKOTOR 0x00407d50
int CExoResMan::ServiceFromImage(CRes* pRes, int bAsync)
{
    if (pRes == 0) {
        return 0;
    }
    if (pRes->m_nStatus & 4) {
        return 0;
    }
    CExoLinkedListPosition pos = m_lstDirectoryKeyTables.GetHeadPos();
    if (pos) {
        CExoKeyTable* pTable = m_lstDirectoryKeyTables.GetAtPos(pos);
        while (pos) {
            if (pTable && (pTable->m_nTableID & 0x0fffffff) == ((pRes->m_nID >> 14) & 0x3f)) {
                unsigned long nIndex = pRes->m_nID & 0x3fff;
                pRes->m_nSize = pTable->m_ppResFile[0]->GetResourceSize(nIndex);
                pRes->m_pResource = ((CExoResourceImageFile*)pTable->m_ppResFile[0])->GetResource(nIndex);
                pRes->m_nStatus |= 4;
                return pRes->OnResourceServiced();
            }
            pTable = m_lstDirectoryKeyTables.GetNext(pos);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004082d0
int CExoResMan::ServiceFromEncapsulatedRaw(CRes* pRes, int nSize, char* pBuffer)
{
    if (!pRes) {
        return 0;
    }
    CExoLinkedListPosition pos = m_lstEncapsulatedKeyTables.GetHeadPos();
    if (pos) {
        CExoKeyTable* pTable = m_lstEncapsulatedKeyTables.GetAtPos(pos);
        while (pos) {
            if (pTable && (pTable->m_nTableID & 0x0fffffff) == ((pRes->m_nID >> 14) & 0x3f)) {
                pTable->m_ppResFile[0]->AddRefs();
                pRes->m_nSize = nSize;
                if (pTable->m_ppResFile[0]->ReadResource(pRes->m_nID, pBuffer, nSize, 0) != nSize) {
                    return 0;
                }
                Free(pRes);
                pTable->m_ppResFile[0]->DelRefs();
                return 1;
            }
            pTable = m_lstEncapsulatedKeyTables.GetNext(pos);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00408390
int CExoResMan::ServiceFromImageRaw(CRes* pRes, int nSize, char* pBuffer)
{
    if (!pRes) {
        return 0;
    }
    CExoLinkedListPosition pos = m_lstDirectoryKeyTables.GetHeadPos();
    if (pos) {
        CExoKeyTable* pTable = m_lstDirectoryKeyTables.GetAtPos(pos);
        while (pos) {
            if (pTable && (pTable->m_nTableID & 0x0fffffff) == ((pRes->m_nID >> 14) & 0x3f)) {
                pTable->m_ppResFile[0]->GetResourceSize(pRes->m_nID & 0x3fff);
                pRes->m_nSize = nSize;
                if (!Malloc(pRes)) {
                    return 0;
                }
                return pTable->m_ppResFile[0]->ReadResource(pRes->m_nID, pBuffer, nSize, 0) == nSize;
            }
            pTable = m_lstDirectoryKeyTables.GetNext(pos);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00408450
int CExoResMan::ServiceFromResFileRaw(CRes* pRes, int nSize, char* pBuffer)
{
    if (!pRes) {
        return 0;
    }
    unsigned long nFile = (pRes->m_nID >> 20) & 0x3ff;
    CExoLinkedListPosition pos = m_lstFixedKeyTables.GetHeadPos();
    if (pos) {
        CExoKeyTable* pTable = m_lstFixedKeyTables.GetAtPos(pos);
        while (pos) {
            if (pTable && (pTable->m_nTableID & 0x0fffffff) == ((pRes->m_nID >> 14) & 0x3f)) {
                pTable->m_ppResFile[nFile]->AddRefs();
                int nResSize = pTable->m_ppResFile[nFile]->GetResourceSize(pRes->m_nID & 0x3fff);
                if (nSize > nResSize) {
                    nSize = nResSize;
                }
                pRes->m_nSize = nSize;
                if (pTable->m_ppResFile[nFile]->ReadResource(pRes->m_nID, pBuffer, nSize, 0) != nSize) {
                    return 0;
                }
                pTable->m_ppResFile[nFile]->DelRefs();
                return 1;
            }
            pTable = m_lstFixedKeyTables.GetNext(pos);
        }
    }
    return 0;
}

// NONMATCH: 15 insns (jump targets only); original keeps a separate return-0 epilogue after the loop, ours cross-jumps it with the read-failure path
// FUNCTION: SWKOTOR 0x00407e00
int CExoResMan::ServiceFromResFile(CRes* pRes, int bAsync)
{
    if (pRes == 0) {
        return 0;
    }
    if (pRes->m_nStatus & 4) {
        return 0;
    }
    unsigned long nFile = (pRes->m_nID >> 20) & 0x3ff;
    CExoLinkedListPosition pos = m_lstFixedKeyTables.GetHeadPos();
    if (pos) {
        CExoKeyTable* pTable = m_lstFixedKeyTables.GetAtPos(pos);
        while (pos) {
            if (pTable && (pTable->m_nTableID & 0x0fffffff) == ((pRes->m_nID >> 14) & 0x3f)) {
                if (bAsync) {
                    pTable->m_ppResFile[nFile]->AddAsyncRefs();
                    int nSize = pTable->m_ppResFile[nFile]->GetResourceSize(pRes->m_nID & 0x3fff);
                    pRes->m_nSize = nSize;
                    if (!Malloc(pRes)) {
                        pTable->m_ppResFile[nFile]->DelAsyncRefs();
                        return 0;
                    }
                    m_pAsyncFile = pTable->m_ppResFile[nFile]->GetAsyncFile();
                    pTable->m_ppResFile[nFile]->ReadResourceAsync(pRes->m_nID, pRes->m_pResource, nSize, 0);
                    return 1;
                }
                pTable->m_ppResFile[nFile]->AddRefs();
                int nSize = pTable->m_ppResFile[nFile]->GetResourceSize(pRes->m_nID & 0x3fff);
                pRes->m_nSize = nSize;
                if (Malloc(pRes) &&
                    pTable->m_ppResFile[nFile]->ReadResource(pRes->m_nID, pRes->m_pResource, nSize, 0) == nSize) {
                    pTable->m_ppResFile[nFile]->DelRefs();
                    int bResult = pRes->OnResourceServiced();
                    if (bResult) {
                        ResManTrace(pRes->m_pKeyEntry->m_cResRef.GetResRefStr(),
                                    pTable->m_ppResFile[nFile]->m_sFileName.CStr(),
                                    g_pExoBase->GetResourceExtension(pRes->m_pKeyEntry->m_nType).CStr(),
                                    pRes->m_pKeyEntry->m_cResRef.GetResRefStr());
                    }
                    pRes->m_nStatus = (pRes->m_nStatus & ~4) | (bResult ? 4 : 0);
                    return bResult;
                }
                pTable->m_ppResFile[nFile]->DelRefs();
                return 0;
            }
            pTable = m_lstFixedKeyTables.GetNext(pos);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00408530
int CExoResMan::ServiceCurrentAsyncRes()
{
    int bResult = 0;
    CExoKeyTable* pTable = GetTable(m_pCurrentAsyncRes);
    if (!pTable) {
        return 0;
    }
    switch (m_pCurrentAsyncRes->m_nID >> 30) {
    case 3:
        if (m_pAsyncFile) {
            delete m_pAsyncFile;
            m_pAsyncFile = 0;
        }
        break;
    case 2:
    case 1:
        pTable->m_ppResFile[0]->DelAsyncRefs();
        break;
    case 0:
        pTable->m_ppResFile[(m_pCurrentAsyncRes->m_nID >> 20) & 0x3ff]->DelAsyncRefs();
        break;
    }
    m_pCurrentAsyncRes->m_nStatus &= ~0x10;
    m_pCurrentAsyncRes->m_nStatus |= 4;
    if (m_pCurrentAsyncRes->m_pResource) {
        m_pCurrentAsyncRes->m_nRequests = 0;
        bResult = m_pCurrentAsyncRes->OnResourceServiced();
        m_lstAsyncResQueue.RemoveHead();
        m_pCurrentAsyncRes = 0;
        m_pAsyncFile = 0;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x004067b0
int CExoResMan::GetFreeDiskSpace(const CExoString& sPath, unsigned __int64* pFreeBytes)
{
    CExoString sResolved;
    unsigned __int64 nFreeToCaller;
    unsigned __int64 nTotal;
    unsigned __int64 nTotalFree;
    sResolved = g_pExoBase->m_pcExoAliasList->ResolveFileName(sPath, 0xffff);
    if (sResolved.GetLength() == 0) {
        return 0;
    }
    int bResult;
    if (GetProcAddress(GetModuleHandleA("kernel32.dll"), "GetDiskFreeSpaceExA")) {
        GetDiskFreeSpaceExA(sResolved.CStr(), &nFreeToCaller, &nTotal, &nTotalFree);
        bResult = 1;
        *pFreeBytes = nFreeToCaller;
    } else {
        bResult = 0;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x004068c0
int CExoResMan::CreateDirectory2(const CExoString& sPath)
{
    CExoString sResolved;
    sResolved = g_pExoBase->m_pcExoAliasList->ResolveFileName(sPath, 0xffff);
    if (sResolved.GetLength() == 0) {
        return 0;
    }
    return CreateDirectoryA(sResolved.CStr(), 0);
}

// NONMATCH: 29 insns; original keeps two copies of the return-0 cleanup (stat failure / directory), ours merges them
// FUNCTION: SWKOTOR 0x00406b20
int CExoResMan::RemoveFile(const CExoString& sName, unsigned short nResType)
{
    CExoString sResolved;
    struct _stat st;
    sResolved = g_pExoBase->m_pcExoAliasList->ResolveFileName(sName, nResType);
    if (sResolved.GetLength() != 0 && _stat(sResolved.CStr(), &st) != -1) {
        if (st.st_mode & 0x4000) {
            return 0;
        }
        _chmod(sResolved.CStr(), 0x80);
        return remove(sResolved.CStr()) != -1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00406990
int CExoResMan::FileCopy(const CExoString& sSource, const CExoString& sDest, unsigned short nResType,
                         unsigned short nDestResType)
{
    if (sSource == sDest) {
        return 0;
    }
    if (nDestResType == 0xffff) {
        nDestResType = nResType;
    }
    CExoFile cSource(sSource, nResType, "rb");
    CExoFile cDest(sDest, nDestResType, "wb");
    if (!cSource.IsOpen() || !cDest.IsOpen()) {
        return 0;
    }
    char* pBuffer = new char[cSource.GetSize()];
    cSource.Read(pBuffer, cSource.GetSize(), 1);
    cDest.Write(pBuffer, cSource.GetSize(), 1);
    cDest.Flush();
    delete[] pBuffer;
    return 1;
}

// FUNCTION: SWKOTOR 0x00408080
int CExoResMan::ServiceFromDirectoryRaw(CRes* pRes, int nSize, char* pBuffer)
{
    CExoString sPath;
    CExoString sResRef;
    if (pRes && pRes->m_pKeyEntry) {
        CExoLinkedListPosition pos = m_lstOverrideKeyTables.GetHeadPos();
        if (pos) {
            CExoKeyTable* pTable = m_lstOverrideKeyTables.GetAtPos(pos);
            while (pos) {
                if (pTable && (pTable->m_nTableID & 0x0fffffff) == ((pRes->m_nID >> 14) & 0x3f)) {
                    pRes->m_pKeyEntry->m_cResRef.CopyToString(sResRef);
                    sPath = pTable->m_sName + "\\" + sResRef;
                    CExoFile* pFile = new CExoFile(sPath, pRes->m_pKeyEntry->m_nType, "rb");
                    if (pFile) {
                        if (!pFile->IsOpen()) {
                            delete pFile;
                            return 0;
                        }
                        pRes->m_nSize = nSize;
                        pFile->Read(pBuffer, nSize, 1);
                        delete pFile;
                        return 1;
                    }
                    return 0;
                }
                pTable = m_lstOverrideKeyTables.GetNext(pos);
            }
        }
    }
    return 0;
}

// NONMATCH: 40 insns; return-0 cleanup laid out at the end instead of after the loop (same cross-jump issue)
// FUNCTION: SWKOTOR 0x004078f0
int CExoResMan::ServiceFromDirectory(CRes* pRes, int bAsync)
{
    CExoString sPath;
    CExoString sResRef;
    if (pRes && !(pRes->m_nStatus & 4) && pRes->m_pKeyEntry) {
        CExoLinkedListPosition pos = m_lstOverrideKeyTables.GetHeadPos();
        if (pos) {
            CExoKeyTable* pTable = m_lstOverrideKeyTables.GetAtPos(pos);
            while (pos) {
                if (pTable && (pTable->m_nTableID & 0x0fffffff) == ((pRes->m_nID >> 14) & 0x3f)) {
                    pRes->m_pKeyEntry->m_cResRef.CopyToString(sResRef);
                    sPath = pTable->m_sName + "\\" + sResRef;
                    pTable->m_sName == "REBOOTDATA:";
                    CExoFile* pFile = new CExoFile(sPath, pRes->m_pKeyEntry->m_nType, "rb");
                    if (pFile) {
                        if (!pFile->IsOpen()) {
                            delete pFile;
                            return 0;
                        }
                        unsigned long nSize = pFile->GetSize();
                        pRes->m_nSize = nSize;
                        if (!nSize || !Malloc(pRes)) {
                            delete pFile;
                            return 0;
                        }
                        if (bAsync) {
                            m_pAsyncFile = pFile;
                            pFile->ReadAsync(pRes->m_pResource, nSize, 1);
                            return 1;
                        }
                        pFile->Read(pRes->m_pResource, nSize, 1);
                        delete pFile;
                        int bResult = pRes->OnResourceServiced();
                        pRes->m_nStatus = (pRes->m_nStatus & ~4) | (bResult ? 4 : 0);
                        return bResult;
                    }
                    return 0;
                }
                pTable = m_lstOverrideKeyTables.GetNext(pos);
            }
        }
    }
    return 0;
}

// NONMATCH: 30 insns; switch blocks laid out case 2 first, original lays out case 3/default first
// FUNCTION: SWKOTOR 0x004074d0
CRes* CExoResMan::GetResObject(const CResRef& cResRef, unsigned short nType)
{
    CExoKeyTable* pTable = 0;
    CExoKeyEntry* pEntry = 0;
    if (GetKeyEntry(cResRef, nType, pTable, pEntry)) {
        if (pEntry->m_pRes) {
            pEntry->m_pRes->m_nRefs++;
        }
        if (pTable->m_nKind != 4 && (!pEntry->m_pRes || pEntry->m_pRes->m_nRefs == 1)) {
            CExoString sResRef;
            CExoString sSource;
            CExoString sExtension;
            switch (pTable->m_nKind) {
            case 3:
                sSource = "ERF";
            default:
            trace:
                sExtension = g_pExoBase->GetResourceExtension(pEntry->m_nType);
                cResRef.CopyToString(sResRef);
                ResManTrace(pTable->m_sName.CStr(), sSource.CStr(), sExtension.CStr(), sResRef.CStr());
                break;
            case 2: {
                CExoKeyTable* pOtherTable;
                CExoKeyEntry* pOtherEntry;
                if (GetKeyEntryFromTable(&m_lstDirectoryKeyTables, cResRef, nType, &pOtherTable, &pOtherEntry, 0)) {
                    break;
                }
                sSource = "RIM";
                goto trace;
            }
            case 1:
                sSource = "BIF";
                break;
            }
        }
        return pEntry->m_pRes;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00406e20
int CExoResMan::AddKeyTable(const CExoString& sName, unsigned long nTableKind, unsigned long nFilter)
{
    unsigned long nTableID = 0;
    CExoLinkedList<CExoKeyTable>* pList;
    unsigned long nKindBits;
    switch (nTableKind) {
    case 1:
        pList = &m_lstFixedKeyTables;
        nKindBits = 0;
        break;
    case 2:
        pList = &m_lstOverrideKeyTables;
        nKindBits = 0x80000000;
        break;
    case 3:
        pList = &m_lstEncapsulatedKeyTables;
        nKindBits = 0x40000000;
        break;
    case 4:
        pList = &m_lstDirectoryKeyTables;
        nKindBits = 0x20000000;
        break;
    default:
        return 0;
    }
    unsigned long aUsed[2];
    aUsed[0] = 0;
    aUsed[1] = 0;
    CExoLinkedListPosition pos = pList->GetHeadPos();
    if (pos) {
        CExoKeyTable* pTable = pList->GetAtPos(pos);
        while (pos) {
            if (pTable) {
                if (pTable->m_sName == sName) {
                    CExoString sMessage;
                    if (nTableKind == 4) {
                        sMessage.Format("Rebuilding %s", sName.m_sString, sName.m_nBufferLength);
                    }
                    pTable->RebuildTable();
                    return 1;
                }
                unsigned long nID = pTable->m_nTableID & 0x0fffffff;
                aUsed[nID >> 5] |= 1 << (nID & 0x1f);
            }
            pTable = pList->GetNext(pos);
        }
    }
    for (int i = 1; i < 64; i++) {
        if (!(aUsed[i >> 5] & (1 << (i & 0x1f)))) {
            nTableID = i;
            break;
        }
    }
    nTableID |= nKindBits;
    CExoKeyTable* pNewTable = new CExoKeyTable();
    CExoLinkedListPosition posNew = pList->AddHead(pNewTable);
    int bResult = pNewTable->BuildNewTable(nTableKind, sName, nTableID, 1);
    pNewTable->m_n08 = nFilter;
    if (!bResult) {
        pList->Remove(posNew);
        delete pNewTable;
    }
    return bResult;
}

// Resource types the entry counts skip unless asked for all of them.
// FUNCTION: SWKOTOR 0x00406720
int CExoResMan::IsSpecialResType(unsigned short nType)
{
    switch (nType) {
    case 2009:
    case 2010:
    case 2012:
    case 2025:
    case 2027:
    case 2029:
    case 2030:
    case 2032:
    case 2035:
    case 2040:
    case 2042:
    case 2044:
    case 2046:
    case 2051:
    case 2056:
    case 2058:
    case 3003:
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00406c60
int CExoResMan::CreateSignatureFile(const CExoString& sA, const CExoString& sB, unsigned long nA, unsigned long nB)
{
    return 1;
}

// FUNCTION: SWKOTOR 0x00407780
int CExoResMan::GetTableCount(CRes* pRes, int bAll)
{
    CExoKeyTable* pTable = GetTable(pRes);
    if (pTable) {
        return pTable->GetEntryCount(bAll);
    }
    return -1;
}

// FUNCTION: SWKOTOR 0x004087a0
int CExoResMan::AddEncapsulatedResourceFile(const CExoString& sName, unsigned long nFilter)
{
    return AddKeyTable(sName, 3, nFilter);
}

// FUNCTION: SWKOTOR 0x004087c0
int CExoResMan::AddResourceImageFile(const CExoString& sName)
{
    return AddKeyTable(sName, 4, 0);
}

// FUNCTION: SWKOTOR 0x004087e0
int CExoResMan::AddFixedKeyTableFile(const CExoString& sName)
{
    return AddKeyTable(sName, 1, 0);
}

// FUNCTION: SWKOTOR 0x00408800
int CExoResMan::AddResourceDirectory(const CExoString& sName)
{
    return AddKeyTable(sName, 2, 0);
}

// FUNCTION: SWKOTOR 0x00408820
int CExoResMan::RemoveEncapsulatedResourceFile(const CExoString& sName)
{
    return RemoveKeyTable(sName, 3);
}

// FUNCTION: SWKOTOR 0x004088c0
int CExoResMan::RemoveFixedKeyTableFile(const CExoString& sName)
{
    return RemoveKeyTable(sName, 1);
}

// FUNCTION: SWKOTOR 0x004088d0
int CExoResMan::RemoveResourceDirectory(const CExoString& sName)
{
    return RemoveKeyTable(sName, 2);
}

// FUNCTION: SWKOTOR 0x00407fe0
int CExoResMan::UpdateKeyTable(const CExoString& sName, unsigned long nTableKind)
{
    CExoLinkedList<CExoKeyTable>* pList;
    switch (nTableKind) {
    case 1:
        pList = &m_lstFixedKeyTables;
        break;
    case 2:
        pList = &m_lstOverrideKeyTables;
        break;
    case 3:
        pList = &m_lstEncapsulatedKeyTables;
        break;
    case 4:
        pList = &m_lstDirectoryKeyTables;
        break;
    default:
        return 0;
    }
    CExoLinkedListPosition pos = pList->GetHeadPos();
    if (pos) {
        CExoKeyTable* pTable = pList->GetAtPos(pos);
        while (pos) {
            if (pTable && sName.CompareNoCase(pTable->m_sName)) {
                pTable->RebuildTable();
                return 1;
            }
            pTable = pList->GetNext(pos);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004088e0
int CExoResMan::UpdateResourceDirectory(const CExoString& sName)
{
    return UpdateKeyTable(sName, 2);
}
