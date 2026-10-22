// CExoResFile (BIF archives).
#include "d/exo_key_table.h"

// FUNCTION: SWKOTOR 0x0040EE80
CExoResFile::CExoResFile()
{
    m_nRefs = 0;
    Initialize();
}

// FUNCTION: SWKOTOR 0x0040D6F0
int CExoResFile::OpenFile()
{
    if (m_bOpen) {
        return 1;
    }
    m_pFile = new CExoFile(m_sFileName, 0x270e, "rb");
    if (m_pFile) {
        if (!m_pFile->IsOpen()) {
            delete m_pFile;
            m_pFile = 0;
        } else {
            m_sFileName.CStr();
            m_bOpen = 1;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0040D800
int CExoResFile::OpenAsyncFile()
{
    if (m_bAsyncOpen) {
        return 1;
    }
    m_pAsyncFile = new CExoFile(m_sFileName, 0x270e, "rb");
    if (m_pAsyncFile) {
        if (!m_pAsyncFile->IsOpen()) {
            delete m_pAsyncFile;
            m_pAsyncFile = 0;
        } else {
            m_bAsyncOpen = 1;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0040D910
int CExoResFile::LoadHeader(unsigned char nType)
{
    if (m_bLoaded) {
        return 1;
    }
    AddRefs();
    if (m_pFile == 0) {
        m_nRefs--;
        return 0;
    }
    m_sFileName.CStr();
    m_nSize = m_pFile->GetSize();
    m_pHeader = new BIF_HEADER;
    m_pFile->Read(m_pHeader, sizeof(BIF_HEADER), 1);
    if (m_pHeader->nFileType != 0x46464942) {
        delete m_pHeader;
        m_pHeader = 0;
        DelRefs();
        return 0;
    }
    if (m_pHeader->nVersion != 0x20203156) {
        delete m_pHeader;
        m_pHeader = 0;
        DelRefs();
        return 0;
    }
    m_pVarTable = 0;
    if (m_pHeader->nVarResCount) {
        m_pVarTable = new BIF_VAR_ENTRY[m_pHeader->nVarResCount];
    }
    if (m_pVarTable == 0) {
        delete m_pHeader;
        m_pHeader = 0;
        DelRefs();
        return 0;
    }
    m_pFile->Seek(m_pHeader->nVarTableOffset, 0);
    m_pFile->Read(m_pVarTable, sizeof(BIF_VAR_ENTRY), m_pHeader->nVarResCount);
    m_bLoaded = 1;
    return 1;
}

// NONMATCH: 2 insns; the original pushes edi after the two guard tests
// FUNCTION: SWKOTOR 0x0040DA20
unsigned long CExoResFile::ReadResource(unsigned long nID, void* pBuffer, unsigned long nSize, unsigned long nOffset)
{
    if (m_bOpen == 0 || pBuffer == 0) {
        return 0;
    }
    unsigned long nIndex = nID & 0x3fff;
    if (nIndex >= m_pHeader->nVarResCount) {
        return 0;
    }
    m_pFile->Seek(m_pVarTable[nIndex].nOffset, 0);
    unsigned long nResSize = m_pVarTable[nIndex].nSize;
    if (nResSize > nSize) {
        return m_pFile->Read(pBuffer, nSize, 1);
    }
    return m_pFile->Read(pBuffer, nResSize, 1);
}

// FUNCTION: SWKOTOR 0x0040DAA0
void CExoResFile::ReadResourceAsync(unsigned long nID, void* pBuffer, unsigned long nSize, unsigned long nOffset)
{
    if (m_bAsyncOpen == 0 || pBuffer == 0) {
        return;
    }
    unsigned long nIndex = nID & 0x3fff;
    if (nIndex >= m_pHeader->nVarResCount) {
        return;
    }
    m_pAsyncFile->Seek(m_pVarTable[nIndex].nOffset, 0);
    unsigned long nResSize = m_pVarTable[nIndex].nSize;
    if (nResSize > nSize) {
        m_pAsyncFile->ReadAsync(pBuffer, nSize, 1);
        return;
    }
    m_pAsyncFile->ReadAsync(pBuffer, nResSize, 1);
}

// CExoBaseResFile: the out-of-line inline dtor and its deleting dtor come
// from the base vtable (0x0073e128) emitted with the inline base ctor.
// FUNCTION: SWKOTOR 0x0040d560 SYMBOL: ??1CExoBaseResFile@@UAE@XZ
// FUNCTION: SWKOTOR 0x0040ee60 SYMBOL: ??_GCExoBaseResFile@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0040d570
CExoFile* CExoBaseResFile::GetFile()
{
    return m_pFile;
}

// Identical bodies of CExoEncapsulatedFile / CExoResourceImageFile fold here.
// FUNCTION: SWKOTOR 0x0040d580
void CExoResFile::AddRefs()
{
    if (m_nRefs <= 0) {
        m_nRefs = 0;
        OpenFile();
    }
    m_nRefs++;
}

// FUNCTION: SWKOTOR 0x0040d5a0
void CExoResFile::AddAsyncRefs()
{
    if (m_nAsyncRefs <= 0) {
        m_nAsyncRefs = 0;
        OpenAsyncFile();
    }
    m_nAsyncRefs++;
}

// FUNCTION: SWKOTOR 0x0040d5c0
int CExoResFile::CloseFile()
{
    if (m_bOpen) {
        m_sFileName.CStr();
        if (m_pFile) {
            delete m_pFile;
            m_pFile = 0;
        }
        m_bOpen = 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0040d600
int CExoResFile::UnloadHeader()
{
    if (m_bLoaded) {
        if (m_pVarTable) {
            delete m_pVarTable;
            m_pVarTable = 0;
        }
        if (m_pHeader) {
            delete m_pHeader;
            m_pHeader = 0;
        }
        m_bLoaded = 0;
        DelRefs();
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0040d650
void CExoResFile::DelRefs()
{
    if (m_nRefs <= 1) {
        m_nRefs = 0;
        CloseFile();
    } else {
        m_nRefs--;
    }
}

// FUNCTION: SWKOTOR 0x0040d670
void CExoResFile::DelAsyncRefs()
{
    if (m_nAsyncRefs <= 1) {
        m_nAsyncRefs = 0;
        CloseAsyncFile();
    } else {
        m_nAsyncRefs--;
    }
}

// FUNCTION: SWKOTOR 0x0040d690
unsigned long CExoResFile::GetResourceSize(unsigned long nID)
{
    return m_pVarTable[nID & 0x3fff].nSize;
}

// FUNCTION: SWKOTOR 0x0040d6b0 SYMBOL: ?Initialize@CExoResFile@@UAEHXZ

// FUNCTION: SWKOTOR 0x0040ef00
CExoResFile::~CExoResFile()
{
    CloseFile();
}

// FUNCTION: SWKOTOR 0x004104c0 SYMBOL: ??_GCExoResFile@@UAEPAXI@Z
