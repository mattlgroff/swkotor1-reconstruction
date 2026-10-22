// CExoKeyTable core (constructor, hashing, lookup).
#include <stdio.h>
#include <string.h>
#include "d/exo_key_table.h"

// FUNCTION: SWKOTOR 0x0040D030
CExoKeyTable::CExoKeyTable()
{
    m_nTableType = 0;
    m_n08 = 0;
    m_bRebuilding = 0;
    m_nTableSize = 0;
    m_pTable = 0;
    m_nKeys = 0;
    m_n18 = 0xffffffff;
    m_nKind = 0;
    m_sName = "";
    m_nFiles = 0;
    m_ppResFile = 0;

    for (unsigned long n = 0; n <= 0xff; n++) {
        unsigned long nValue = n;
        for (int nBit = 8; nBit > 0; nBit--) {
            if (nValue & 1) {
                nValue = (nValue >> 1) ^ 0xedb88320;
            } else {
                nValue = nValue >> 1;
            }
        }
        g_aCRCTable[n] = nValue;
    }
}

// FUNCTION: SWKOTOR 0x0040D490
int CExoKeyTable::GetTableIndex(unsigned long* pIndex, const CResRef& cResRef, unsigned short nType)
{
    if (m_pTable == 0) {
        return 0;
    }
    for (unsigned long i = 0; i < m_nTableSize; i++) {
        if (m_pTable[i].m_nType == nType && m_pTable[i].m_cResRef == cResRef) {
            *pIndex = i;
            return 1;
        }
    }
    return 0;
}

// NONMATCH: 3 insns; the CRC table load lands in ecx and is scheduled after the shift
// FUNCTION: SWKOTOR 0x0040D500
unsigned long CExoKeyTable::Hash(const CResRef& cResRef, unsigned short nType)
{
    unsigned long nHash = 0;
    for (unsigned long i = 0; i < 16; i++) {
        char c = cResRef.m_resRef[i];
        if (c == 0) {
            break;
        }
        nHash = (nHash >> 8) ^ g_aCRCTable[((tolower((unsigned char)c) + nType) ^ nHash) & 0xff];
    }
    return nHash % m_nTableSize;
}

// FUNCTION: SWKOTOR 0x0040EB80
CExoKeyEntry* CExoKeyTable::AllocateTable(unsigned long nSize)
{
    CExoKeyEntry* pTable = new CExoKeyEntry[nSize];
    for (unsigned long i = 0; i < nSize; i++) {
        CExoKeyEntry* pEntry = &pTable[i];
        if (pEntry) {
            pEntry->m_cResRef = "";
            pEntry->m_pRes = 0;
            pEntry->m_nID = 0xffffffff;
            pEntry->m_nRefs = 0;
            pEntry->m_nType = 0xffff;
        }
    }
    return pTable;
}

// NONMATCH: 14 insns; loop head layout (the original re-tests bWrapped at an
// aligned loop top); volatile only forces the memory-resident flag
// FUNCTION: SWKOTOR 0x0040EC50
CExoKeyEntry* CExoKeyTable::FindKey(const CResRef& cResRef, unsigned short nType)
{
    CExoKeyEntry* pResult = 0;
    volatile int bWrapped = 0;
    if (cResRef == "") {
        return 0;
    }
    if (m_nTableSize == 0) {
        return 0;
    }
    unsigned long nStart = Hash(cResRef, nType);
    unsigned long nIndex = nStart;
    for (;;) {
        if (bWrapped) {
            break;
        }
        if (m_pTable[nIndex].m_cResRef == cResRef && m_pTable[nIndex].m_nType == nType) {
            pResult = &m_pTable[nIndex];
            break;
        }
        if (m_pTable[nIndex].m_cResRef == "") {
            break;
        }
        nIndex++;
        if (nIndex >= m_nTableSize) {
            nIndex = 0;
        }
        if (nIndex == nStart) {
            bWrapped = 1;
        }
    }
    return pResult;
}

// FUNCTION: SWKOTOR 0x0040D0D0
CExoKeyTable::~CExoKeyTable()
{
    if (m_ppResFile) {
        for (unsigned long i = 0; i < m_nFiles; i++) {
            if (m_ppResFile[i]) {
                m_ppResFile[i]->UnloadHeader();
                delete m_ppResFile[i];
            }
        }
        delete m_ppResFile;
    }
    if (m_pTable) {
        delete m_pTable;
    }
    m_bRebuilding = 1;
    m_nTableType = 0;
    m_nTableSize = 0;
    m_pTable = 0;
    m_nKeys = 0;
    m_n18 = 0xffffffff;
    m_nKind = 0;
    m_sName = "";
    m_nFiles = 0;
    m_ppResFile = 0;
}

// FUNCTION: SWKOTOR 0x0040D200
int CExoKeyTable::LocateBifFile(const CExoString& sName)
{
    CExoFile* pFile = new CExoFile(sName, 0x270e, "rb");
    if (pFile == 0) {
        return 0;
    }
    if (!pFile->IsOpen()) {
        delete pFile;
        return 0;
    }
    delete pFile;
    return 1;
}

// FUNCTION: SWKOTOR 0x0040D2E0
void CExoKeyTable::DestroyTable()
{
    CExoKeyTable* pTable = 0;
    CExoKeyEntry* pEntry = 0;
    m_bRebuilding = 1;
    for (unsigned long i = 0; i < m_nTableSize; i++) {
        if (m_pTable[i].m_pRes) {
            if ((m_pTable[i].m_nID & 0xc0000000) != 0x40000000) {
                g_pExoResMan->GetKeyEntry(m_pTable[i].m_cResRef, m_pTable[i].m_nType, pTable, pEntry);
            } else {
                pEntry = 0;
                pTable = 0;
            }
            if (pEntry == 0) {
                if (m_pTable[i].m_pRes->m_nDemands > 0) {
                    CExoString sResRef;
                    char szMessage[256];
                    m_pTable[i].m_cResRef.CopyToString(sResRef);
                    sprintf(szMessage, "CExoKeyTable::DestroyTable: Resource %s still in demand during table deletion",
                            sResRef.CStr());
                    m_pTable[i].m_pRes->m_nStatus |= 0x200;
                } else {
                    m_pTable[i].m_pRes->Dump(0);
                    m_pTable[i].m_pRes->m_pKeyEntry = 0;
                    m_pTable[i].m_pRes->m_nID = 0xffffffff;
                }
            } else {
                pEntry->m_pRes = m_pTable[i].m_pRes;
                pEntry->m_pRes->m_nID = pEntry->m_nID;
                m_pTable[i].m_pRes->m_pKeyEntry = pEntry;
                m_pTable[i].m_pRes = 0;
                if (pEntry->m_pRes->m_nDemands > 0) {
                    pEntry->m_pRes->m_nStatus |= 0x100;
                } else {
                    pEntry->m_pRes->Dump(0);
                }
            }
        }
    }
}

// NONMATCH: 68 insns; the original keeps nCollisions/bWrapped in memory and
// does not rotate the probe loops
// FUNCTION: SWKOTOR 0x0040E990
CExoKeyEntry* CExoKeyTable::AddKey(CResRef* pResRef, unsigned short nType, unsigned long nID, int nFlags)
{
    unsigned long nCollisions = 0;
    int bFound = 0;
    int bWrapped;
    unsigned long nIndex;
    do {
        bWrapped = 0;
        unsigned long nStart = Hash(*pResRef, nType);
        nIndex = nStart;
        if (*pResRef == "") {
            return 0;
        }
        while (!bWrapped) {
            if (m_pTable[nIndex].m_cResRef == "" ||
                (m_pTable[nIndex].m_cResRef == *pResRef && m_pTable[nIndex].m_nType == nType)) {
                bFound = 1;
                break;
            }
            m_nKeys++;
            nCollisions++;
            nIndex++;
            if (nIndex >= m_nTableSize) {
                nIndex = 0;
            }
            if (nIndex == nStart) {
                bWrapped = 1;
            }
        }
    } while (bWrapped == 1);
    if (!bFound) {
        return 0;
    }
    if (m_pTable[nIndex].m_nID != 0xffffffff && m_pTable[nIndex].m_cResRef != "") {
        char szResRef[20];
        char szMessage[100] = "CExoKeyTable::AddKey: Duplicate Resource ";
        m_pTable[nIndex].m_cResRef.CopyToString(szResRef);
        strcat(szMessage, szResRef);
        strcat(szMessage, " - Ignoring Duplicate");
        return 0;
    }
    m_pTable[nIndex].m_nID = nID;
    m_pTable[nIndex].m_nID = (m_pTable[nIndex].m_nID & 0xfff03fff) | ((m_nTableID & 0x0fffffff) << 14);
    m_pTable[nIndex].m_cResRef = *pResRef;
    m_pTable[nIndex].m_nType = nType;
    m_nKeys++;
    return &m_pTable[nIndex];
}

// NONMATCH: 21 insns; ebx/ebp swapped between the list pointer and the loop index
// FUNCTION: SWKOTOR 0x0040ED20
CExoStringList* CExoKeyTable::GetKeysOfType(unsigned short nType)
{
    if (nType == 0xffff) {
        return 0;
    }
    if (m_nTableSize == 0) {
        return 0;
    }
    unsigned long i;
    CExoStringList* pList = new CExoStringList(0, 1, 5);
    CExoStringList* pResult = pList;
    for (i = 0; i < m_nTableSize; i++) {
        if (m_pTable[i].m_nType == nType) {
            char szResRef[17];
            m_pTable[i].m_cResRef.CopyToString(szResRef);
            pList->Add(new CExoString(szResRef));
        }
    }
    if (pList->GetCount() <= 0) {
        delete pList;
        pResult = 0;
    }
    return pResult;
}

// NONMATCH: 12 insns; register choice (eax/ecx/edx) for &cResRef and the array cookie
// FUNCTION: SWKOTOR 0x0040F200
int CExoKeyTable::AddDirectoryContents(int nFlags)
{
    CExoString sFile;
    CExoArrayList<CExoString> aFiles;
    CResRef cResRef;
    g_pExoBase->GetDirectoryList(&aFiles, m_sName, 0xffff, 0, 0);
    int nFiles = aFiles.num;
    m_nTableSize = nFiles * 2 + 1;
    m_pTable = AllocateTable(nFiles * 2 + 1);
    if (m_pTable == 0) {
        return 0;
    }
    for (int i = 0; i < nFiles; i++) {
        sFile = aFiles[i];
        unsigned short nType = g_pExoResMan->GetResTypeFromFile(sFile);
        if (nType != 0xffff) {
            g_pExoResMan->GetResRefFromFile(cResRef, sFile);
            if (cResRef != "") {
                AddKey(&cResRef, nType, (m_nTableID | 0xfffffc00) << 20, nFlags);
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00410190
int CExoKeyTable::BuildNewTable(unsigned long nKind, const CExoString& sName, unsigned long nTableID, int nFlags)
{
    if (sName == "") {
        return 0;
    }
    m_sName = sName;
    switch (nKind) {
    case 1:
        m_nTableID = nTableID;
        m_nKind = 1;
        return AddKeyTableContents(nFlags);
    case 2:
        m_nTableID = nTableID;
        m_nKind = 2;
        return AddDirectoryContents(nFlags);
    case 3:
        m_nTableID = nTableID;
        m_nKind = 3;
        return AddEncapsulatedContents(nFlags);
    case 4:
        m_nTableID = nTableID;
        m_nKind = 4;
        return AddResourceImageContents(nFlags);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00410260
void CExoKeyTable::RebuildTable()
{
    unsigned long nIndex = 0;
    CExoKeyEntry* pEntry = 0;
    CExoKeyTable* pTable = 0;
    CExoKeyEntry* pOldTable = m_pTable;
    if (pOldTable == 0) {
        if (m_nKind != 0 && m_sName != "") {
            BuildNewTable(m_nKind, m_sName, m_nTableID, 1);
        }
        return;
    }
    unsigned long nOldSize = m_nTableSize;
    if (m_nKind == 4 && m_ppResFile) {
        (*m_ppResFile)->CloseFile();
    }
    m_nTableSize = 0;
    m_pTable = 0;
    BuildNewTable(m_nKind, m_sName, m_nTableID, 0);
    m_bRebuilding = 1;
    for (unsigned long i = 0; i < nOldSize; i++) {
        CExoKeyEntry* pOld = &pOldTable[i];
        if (pOld->m_pRes) {
            if (GetTableIndex(&nIndex, pOld->m_cResRef, pOld->m_nType)) {
                m_pTable[nIndex].m_pRes = pOld->m_pRes;
                m_pTable[nIndex].m_pRes->m_nID = m_pTable[nIndex].m_nID;
                pOld->m_pRes = 0;
                m_pTable[nIndex].m_pRes->m_pKeyEntry = &m_pTable[nIndex];
            } else {
                g_pExoResMan->GetKeyEntry(pOld->m_cResRef, pOld->m_nType, pTable, pEntry);
                if (pEntry == 0) {
                    if (pOld->m_pRes->m_nDemands > 0) {
                        pOld->m_pRes->m_nStatus |= 0x200;
                    } else {
                        pOld->m_pRes->Dump(0);
                        pOld->m_pRes->m_pKeyEntry = 0;
                        pOld->m_pRes->m_nID = 0xffffffff;
                    }
                } else {
                    pEntry->m_pRes = pOld->m_pRes;
                    pEntry->m_pRes->m_nID = pEntry->m_nID;
                    pOld->m_pRes = 0;
                    pEntry->m_pRes->m_pKeyEntry = pEntry;
                    if (pEntry->m_pRes->m_nDemands > 0) {
                        pEntry->m_pRes->m_nStatus |= 0x100;
                    } else {
                        pEntry->m_pRes->Dump(0);
                    }
                }
            }
        }
    }
    delete pOldTable;
    for (unsigned long j = 0; j < m_nTableSize; j++) {
        if (m_pTable[j].m_pRes == 0) {
            g_pExoResMan->GetKeyEntry(m_pTable[j].m_cResRef, m_pTable[j].m_nType, pTable, pEntry);
            if (pEntry && pEntry->m_pRes && pTable->m_nTableID < m_nTableID) {
                if (pEntry->m_pRes->m_nDemands > 0) {
                    pEntry->m_pRes->m_nStatus |= 0x100;
                } else {
                    pEntry->m_pRes->Dump(0);
                }
                m_pTable[j].m_pRes = pEntry->m_pRes;
                m_pTable[j].m_pRes->m_nID = m_pTable[j].m_nID;
                pEntry->m_pRes = 0;
                m_pTable[j].m_pRes->m_pKeyEntry = &m_pTable[j];
            }
        }
    }
    m_bRebuilding = 0;
}

// FUNCTION: SWKOTOR 0x0040d1a0
unsigned long CExoKeyTable::GetEntryCount(int bAll)
{
    unsigned long nCount = 0;
    for (unsigned long i = 0; i < m_nTableSize; i++) {
        CExoKeyEntry* pEntry = &m_pTable[i];
        if (pEntry && pEntry->m_nType != 0xffff) {
            if (bAll || !g_pExoResMan->IsSpecialResType(pEntry->m_nType)) {
                nCount++;
            }
        }
    }
    return nCount;
}
