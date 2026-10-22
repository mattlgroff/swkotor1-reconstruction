// CExoEncapsulatedFile (ERF / HAK / MOD / SAV archives).
#include "d/exo_key_table.h"

// FUNCTION: SWKOTOR 0x0040EF90
CExoEncapsulatedFile::CExoEncapsulatedFile()
{
    Initialize();
}

// FUNCTION: SWKOTOR 0x0040DC30
int CExoEncapsulatedFile::OpenFile()
{
    if (m_bOpen) {
        return 1;
    }
    m_pFile = 0;
    if (m_nType == 0) {
        m_pFile = new CExoFile(m_sFileName, 0x7db, "rb");
    } else if (m_nType == 1) {
        m_pFile = new CExoFile(m_sFileName, 0x809, "rb");
    } else if (m_nType == 2) {
        m_pFile = new CExoFile(m_sFileName, 0x270d, "rb");
    } else if (m_nType == 3) {
        m_pFile = new CExoFile(m_sFileName, 0x80d, "rb");
    } else if (m_nType == 4) {
        m_pFile = new CExoFile(m_sFileName, 0x80e, "rb");
    }
    if (m_pFile) {
        if (!m_pFile->IsOpen()) {
            delete m_pFile;
            m_pFile = 0;
        } else {
            m_bOpen = 1;
            return 1;
        }
    }
    m_bOpen = 0;
    return 0;
}

// FUNCTION: SWKOTOR 0x0040DF10
int CExoEncapsulatedFile::OpenAsyncFile()
{
    if (m_bAsyncOpen) {
        return 1;
    }
    m_pAsyncFile = 0;
    if (m_nType == 0) {
        m_pAsyncFile = new CExoFile(m_sFileName, 0x7db, "rb");
    } else if (m_nType == 1) {
        m_pAsyncFile = new CExoFile(m_sFileName, 0x809, "rb");
    } else if (m_nType == 2) {
        m_pAsyncFile = new CExoFile(m_sFileName, 0x270d, "rb");
    } else if (m_nType == 3) {
        m_pAsyncFile = new CExoFile(m_sFileName, 0x80d, "rb");
    } else if (m_nType == 4) {
        m_pAsyncFile = new CExoFile(m_sFileName, 0x80e, "rb");
    }
    if (m_pAsyncFile) {
        if (!m_pAsyncFile->IsOpen()) {
            delete m_pAsyncFile;
            m_pAsyncFile = 0;
        } else {
            m_bAsyncOpen = 1;
            return 1;
        }
    }
    m_bAsyncOpen = 0;
    return 0;
}

// FUNCTION: SWKOTOR 0x0040E640
unsigned long CExoEncapsulatedFile::ReadResource(unsigned long nID, void* pBuffer, unsigned long nSize, unsigned long nOffset)
{
    if (m_bOpen == 0 || pBuffer == 0) {
        return 0;
    }
    unsigned long nIndex = nID & 0x3fff;
    if (nIndex >= m_pHeader->nEntryCount) {
        return 0;
    }
    m_pFile->Seek(m_pResList[nIndex].nOffset, 0);
    unsigned long nResSize = m_pResList[nIndex].nSize;
    if (nResSize > nSize) {
        return m_pFile->Read(pBuffer, nSize, 1);
    }
    return m_pFile->Read(pBuffer, nResSize, 1);
}

// FUNCTION: SWKOTOR 0x0040E6B0
void CExoEncapsulatedFile::ReadResourceAsync(unsigned long nID, void* pBuffer, unsigned long nSize, unsigned long nOffset)
{
    if (m_bAsyncOpen == 0 || pBuffer == 0) {
        return;
    }
    unsigned long nIndex = nID & 0x3fff;
    if (nIndex >= m_pHeader->nEntryCount) {
        return;
    }
    m_pAsyncFile->Seek(m_pResList[nIndex].nOffset, 0);
    unsigned long nResSize = m_pResList[nIndex].nSize;
    if (nResSize > nSize) {
        m_pAsyncFile->ReadAsync(pBuffer, nSize, 1);
        return;
    }
    m_pAsyncFile->ReadAsync(pBuffer, nResSize, 1);
}

// NONMATCH: 32 insns; the original keeps 0 in edi throughout and tests the signatures with je/jmp
// FUNCTION: SWKOTOR 0x0040E1F0
int CExoEncapsulatedFile::LoadHeader(unsigned char nType)
{
    CExoString sString;
    if (m_bLoaded) {
        return 1;
    }
    m_pFile = 0;
    if (nType == 0) {
        m_pFile = new CExoFile(m_sFileName, 0x7db, "rb");
    } else if (nType == 1) {
        m_pFile = new CExoFile(m_sFileName, 0x809, "rb");
    } else if (nType == 2) {
        m_pFile = new CExoFile(m_sFileName, 0x270d, "rb");
    } else if (nType == 3) {
        m_pFile = new CExoFile(m_sFileName, 0x80d, "rb");
    } else if (nType == 4) {
        m_pFile = new CExoFile(m_sFileName, 0x80e, "rb");
    }
    if (m_pFile) {
        if (m_pFile->IsOpen()) {
            m_nType = nType;
            m_nSize = m_pFile->GetSize();
            m_pHeader = new ERF_HEADER;
            if (m_pHeader) {
                m_pFile->Read(m_pHeader, sizeof(ERF_HEADER), 1);
                if (nType == 2) {
                    if (m_pHeader->nFileType != 0x20465245) {
                        goto failed;
                    }
                } else if (nType == 3) {
                    if (m_pHeader->nFileType != 0x204b4148) {
                        goto failed;
                    }
                } else if (m_pHeader->nFileType != 0x20444f4d) {
                    goto failed;
                }
                if (m_pHeader->nVersion != 0x302e3156) {
                    goto failed;
                }
                m_pFile->Seek(m_pHeader->nOffsetToLocalizedString, 0);
                if (m_pHeader->nLocalizedStringSize) {
                    char* pStrings = new char[m_pHeader->nLocalizedStringSize];
                    if (pStrings == 0) {
                        goto failed;
                    }
                    m_pFile->Read(pStrings, 1, m_pHeader->nLocalizedStringSize);
                    char* pPos = pStrings;
                    for (unsigned long i = 0; i < m_pHeader->nLanguageCount; i++) {
                        int nLanguage = *(int*)pPos;
                        pPos += 4;
                        int nLength = *(int*)pPos;
                        pPos += 4;
                        sString = CExoString(pPos, nLength);
                        m_lsDescription.AddString(nLanguage, sString, 0);
                        pPos += nLength;
                    }
                    delete[] pStrings;
                }
                m_pResList = new ERF_RES_ENTRY[m_pHeader->nEntryCount];
                if (m_pResList == 0) {
                    goto failed;
                }
                m_pFile->Seek(m_pHeader->nOffsetToResourceList, 0);
                m_pFile->Read(m_pResList, sizeof(ERF_RES_ENTRY), m_pHeader->nEntryCount);
                m_bLoaded = 1;
                delete m_pFile;
                return 1;
failed:
                delete m_pHeader;
                m_pHeader = 0;
            }
        }
        delete m_pFile;
        m_pFile = 0;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0040db10
int CExoEncapsulatedFile::CloseFile()
{
    if (m_bOpen) {
        if (m_pFile) {
            delete m_pFile;
            m_pFile = 0;
        }
        m_bOpen = 0;
    }
    return 1;
}

// CExoResFile::CloseAsyncFile folds here.
// FUNCTION: SWKOTOR 0x0040db50
int CExoEncapsulatedFile::CloseAsyncFile()
{
    if (m_bAsyncOpen) {
        if (m_pAsyncFile) {
            delete m_pAsyncFile;
            m_pAsyncFile = 0;
        }
        m_bAsyncOpen = 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0040db90
int CExoEncapsulatedFile::UnloadHeader()
{
    if (m_bLoaded) {
        if (m_pResList) {
            delete m_pResList;
            m_pResList = 0;
        }
        m_lsDescription.ClearLocString();
        if (m_pHeader) {
            delete m_pHeader;
            m_pHeader = 0;
        }
        m_bLoaded = 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0040dbe0
unsigned long CExoEncapsulatedFile::GetResourceSize(unsigned long nID)
{
    return m_pResList[nID & 0x3fff].nSize;
}

// FUNCTION: SWKOTOR 0x0040dc00 SYMBOL: ?Initialize@CExoEncapsulatedFile@@UAEHXZ

// FUNCTION: SWKOTOR 0x0040f010
CExoEncapsulatedFile::~CExoEncapsulatedFile()
{
    CloseFile();
}

// FUNCTION: SWKOTOR 0x004104e0 SYMBOL: ??_GCExoEncapsulatedFile@@UAEPAXI@Z
