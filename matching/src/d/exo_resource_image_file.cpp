// CExoResourceImageFile (RIM archives, loaded whole into memory).
#include "d/exo_key_table.h"
extern "C" void* __cdecl memcpy(void* d, const void* s, unsigned int n);
#pragma intrinsic(memcpy)
// FUNCTION: SWKOTOR 0x0040E790
int CExoResourceImageFile::OpenFile()
{
    if (m_bOpen) {
        return 1;
    }
    m_pFile = new CExoFile(m_sFileName, 0xbc1, "rb");
    if (m_pFile == 0) {
        return 0;
    }
    if (!m_pFile->IsOpen()) {
        delete m_pFile;
        m_pFile = 0;
        m_pFile = new CExoFile(m_sFileName, 0xbba, "rb");
        if (m_pFile == 0 || !m_pFile->IsOpen()) {
            return 0;
        }
    }
    unsigned long nSize = m_pFile->GetSize();
    m_nSize = nSize;
    m_pHeader = (RIM_HEADER*)new char[nSize];
    if (m_pHeader == 0) {
        delete m_pFile;
        m_pFile = 0;
        return 0;
    }
    if (m_pFile->Read(m_pHeader, nSize, 1) != nSize) {
        delete m_pFile;
        m_pFile = 0;
        return 0;
    }
    m_bOpen = 1;
    delete m_pFile;
    m_pFile = 0;
    return 1;
}

// Empty bodies fold into one shared `ret` (also DelAsyncRefs).
// FUNCTION: SWKOTOR 0x004015a0
void CExoResourceImageFile::AddAsyncRefs()
{
}

void CExoResourceImageFile::DelAsyncRefs()
{
}

// FUNCTION: SWKOTOR 0x0040e720
int CExoResourceImageFile::CloseFile()
{
    if (m_bOpen) {
        if (m_pHeader) {
            delete m_pHeader;
            m_pHeader = 0;
        }
        m_bOpen = 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0040e750
void CExoResourceImageFile::DelRefs()
{
    if (m_nRefs <= 1) {
        m_nRefs = 0;
    } else {
        m_nRefs--;
    }
}

// FUNCTION: SWKOTOR 0x0040e770
int CExoResourceImageFile::Initialize()
{
    m_pFile = 0;
    m_nResType = 0;
    m_nSize = 0;
    m_nRefs = 0;
    m_bOpen = 0;
    m_pHeader = 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x0040e980
void CExoResourceImageFile::ReadResourceAsync(unsigned long nID, void* pBuffer, unsigned long nSize, unsigned long nOffset)
{
    ReadResource(nID, pBuffer, nSize, nOffset);
}

// FUNCTION: SWKOTOR 0x0040f0a0
CExoResourceImageFile::~CExoResourceImageFile()
{
    CloseFile();
}

// FUNCTION: SWKOTOR 0x0040f120
unsigned long CExoResourceImageFile::GetResourceSize(unsigned long nID)
{
    RIM_ENTRY cEntry = *GetEntry(nID & 0x3fff);
    return cEntry.nSize;
}

// FUNCTION: SWKOTOR 0x0040f160
void* CExoResourceImageFile::GetResource(unsigned long nID)
{
    RIM_ENTRY cEntry = *GetEntry(nID & 0x3fff);
    return (char*)m_pHeader + cEntry.nOffset;
}

// FUNCTION: SWKOTOR 0x0040f1a0
unsigned long CExoResourceImageFile::ReadResource(unsigned long nID, void* pBuffer, unsigned long nSize, unsigned long nOffset)
{
    if (m_bOpen == 0 || pBuffer == 0) {
        return 0;
    }
    unsigned long nIndex = nID & 0x3fff;
    if (nIndex >= m_pHeader->nEntries) {
        return 0;
    }
    RIM_ENTRY* pEntry = ((RIM_ENTRY**)((char*)m_pHeader + m_pHeader->nOffsetToEntries))[nIndex];
    unsigned long nCopy = pEntry->nSize < nSize ? pEntry->nSize : nSize;
    memcpy(pBuffer, (char*)m_pHeader + pEntry->nOffset, nCopy);
    return nCopy;
}

// FUNCTION: SWKOTOR 0x00410500 SYMBOL: ??_GCExoResourceImageFile@@UAEPAXI@Z
