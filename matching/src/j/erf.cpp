// CERFFile / CERFKey / CERFRes / CERFString (0x005DCFC0-0x005DE44B).
#include <string.h>
#include <time.h>
#include "j/erf.h"

// FUNCTION: SWKOTOR 0x005DCFC0
int CERFFile::Create(CExoString& sFileName)
{
    m_pFile = new CExoFile(sFileName, 0x809, "wb");
    return m_pFile->IsOpen() != 0;
}

// FUNCTION: SWKOTOR 0x005DD080
int CERFFile::WriteHeader()
{
    if (m_pFile) {
        time_t nTime = time(0);
        struct tm* pTime = localtime(&nTime);
        if (pTime) {
            m_nBuildYear = pTime->tm_year;
            m_nBuildDay = pTime->tm_yday;
        }
        m_pFile->Write(this, 0xA0, 1);
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005DD0E0
int CERFFile::Finish()
{
    m_pFile->Flush();
    delete m_pFile;
    m_pFile = 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x005DD120
int CERFKey::Write(CExoFile* pFile)
{
    pFile->Write(m_cResRef, 0x10, 1);
    pFile->Write(&m_nResID, 4, 1);
    pFile->Write(&m_nResType, 2, 1);
    pFile->Write(&m_nUnused, 2, 1);
    return 1;
}

// FUNCTION: SWKOTOR 0x005DD170
int CERFRes::CopyToFile(CExoFile* pSrc, CExoFile* pDst, int nUnused)
{
    pSrc->Seek(m_nOffset, 0);
    char* pBuffer = g_pAppManager->GetLargeTempBuffer();
    int nLeft = m_nSize;
    while (nLeft > 0) {
        int nChunk = nLeft;
        if (nChunk >= 0x40000) {
            nChunk = 0x40000;
        }
        pSrc->Read(pBuffer, nChunk, 1);
        pDst->Write(pBuffer, nChunk, 1);
        nLeft -= nChunk;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005DD1E0
int CERFRes::Write(CExoFile* pFile, unsigned long nEntryOffset)
{
    m_nOffset = pFile->GetOffset();
    pFile->Write(m_pData, m_nSize, 1);
    pFile->Seek(nEntryOffset, 0);
    pFile->Write(&m_nOffset, 4, 1);
    pFile->Write(&m_nSize, 4, 1);
    pFile->SeekEnd();
    return 1;
}

// FUNCTION: SWKOTOR 0x005DD240
void CERFString::SetText(CExoString& sText)
{
    if (m_sText) {
        delete[] m_sText;
    }
    m_nLength = sText.GetLength();
    m_sText = new char[m_nLength + 1];
    memcpy(m_sText, sText.CStr(), m_nLength);
    m_sText[m_nLength] = 0;
}

// FUNCTION: SWKOTOR 0x005DD2B0
int CERFString::Read(CExoFile* pFile)
{
    CExoString sText;
    pFile->Read(&m_nLanguage, 4, 1);
    pFile->Read(&m_nLength, 4, 1);
    pFile->Read(sText, m_nLength);
    SetText(sText);
    return 1;
}

// FUNCTION: SWKOTOR 0x005DD350 SYMBOL: ??0?$CExoLinkedList@VCERFString@@@@QAE@XZ
// CExoLinkedList<T>::CExoLinkedList (j/erf.h)

// FUNCTION: SWKOTOR 0x005DCE50
int CERFFile::ReadHeader(CExoString& sFileName)
{
    CExoFile cFile(sFileName, 0x809, "rb");
    if (cFile.IsOpen()) {
        cFile.Seek(0, 0);
        m_nC4 = cFile.GetSize();
        cFile.Read(&m_nFileType, 8, 1);
        cFile.Read(&m_nLanguageCount, 4, 1);
        cFile.Read(&m_nLocalizedStringSize, 4, 1);
        cFile.Read(&m_nEntryCount, 4, 1);
        cFile.Read(&m_nOffsetToLocalizedString, 4, 1);
        cFile.Read(&m_nOffsetToKeyList, 4, 1);
        cFile.Read(&m_nOffsetToResourceList, 4, 1);
        cFile.Read(&m_nBuildYear, 4, 1);
        cFile.Read(&m_nBuildDay, 4, 1);
        cFile.Read(&m_nDescriptionStrRef, 4, 1);
        cFile.Read(m_pReserved, 0x74, 1);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005DD3C0
int CERFFile::Read(CExoString& sFileName)
{
    CExoFile cFile(sFileName, 0x809, "rb");
    if (cFile.IsOpen()) {
        cFile.Seek(0xA0, 0);
        unsigned long i;
        for (i = 0; i < m_nLanguageCount; i++) {
            CERFString* pString = new CERFString;
            pString->Read(&cFile);
            m_lStrings.AddTail(pString);
        }
        for (i = 0; i < m_nEntryCount; i++) {
            CERFKey* pKey = new CERFKey;
            cFile.Read(pKey->m_cResRef, 0x10, 1);
            cFile.Read(&pKey->m_nResID, 4, 1);
            cFile.Read(&pKey->m_nResType, 2, 1);
            cFile.Read(&pKey->m_nUnused, 2, 1);
            m_lKeys.AddTail(pKey);
        }
        for (i = 0; i < m_nEntryCount; i++) {
            CERFRes* pRes = new CERFRes;
            cFile.Read(&pRes->m_nOffset, 4, 1);
            cFile.Read(&pRes->m_nSize, 4, 1);
            m_lResources.AddTail(pRes);
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005DD5A0
int CERFFile::WriteStringTable()
{
    if (m_pFile) {
        unsigned long nCount = m_lStrings.GetCount();
        unsigned long nSize = 0;
        unsigned long nStart = m_pFile->GetOffset();
        CExoLinkedListPosition pos = m_lStrings.GetHeadPos();
        while (pos) {
            CERFString* pString = m_lStrings.GetAtPos(pos);
            nSize = pString->m_nLength + 8;
            CExoFile* pFile = m_pFile;
            pFile->Write(&pString->m_nLanguage, 4, 1);
            pFile->Write(&pString->m_nLength, 4, 1);
            pFile->Write(pString->m_sText, pString->m_nLength, 1);
            m_lStrings.GetNext(pos);
        }
        unsigned long nEnd = m_pFile->GetOffset();
        m_pFile->Seek(8, 0);
        m_pFile->Write(&nCount, 4, 1);
        m_pFile->Write(&nSize, 4, 1);
        unsigned long nOffset = m_pFile->GetOffset() + 4;
        m_pFile->Seek(nOffset, 0);
        m_nB0 = nStart;
        m_pFile->Write(&nStart, 4, 1);
        m_nB4 = nEnd;
        m_pFile->Write(&nEnd, 4, 1);
        m_pFile->SeekEnd();
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005DD710
void CERFFile::ExtractAll(CExoString& sFileName, CExoString& sDirectory, int nUnused,
                         ERFProgressCallback pCallback, void* pParam1, void* pParam2)
{
    CExoFile cFile(sFileName, 0x809, "rb");
    if (cFile.IsOpen()) {
        unsigned long i;
        CExoString sPath;
        CExoLinkedListPosition posKey = m_lKeys.GetHeadPos();
        CExoLinkedListPosition posRes = m_lResources.GetHeadPos();
        for (i = 0; i < m_nEntryCount; i++) {
            CERFKey* pKey = m_lKeys.GetAtPos(posKey);
            CERFRes* pRes = m_lResources.GetAtPos(posRes);
            sPath = sDirectory + pKey->m_cResRef;
            CExoFile* pFile = new CExoFile(sPath, pKey->m_nResType, "wb");
            pRes->CopyToFile(&cFile, pFile, nUnused);
            pFile->Flush();
            if (pCallback) {
                pCallback(i, m_nEntryCount, pParam1, pParam2);
            }
            delete pFile;
            m_lKeys.GetNext(posKey);
            m_lResources.GetNext(posRes);
        }
        if (pCallback) {
            pCallback(m_nEntryCount, m_nEntryCount, pParam1, pParam2);
        }
    }
}

// FUNCTION: SWKOTOR 0x005DD970
CERFKey::~CERFKey()
{
    m_nResID = 0;
    m_nResType = 0xFFFF;
    memset(m_cResRef, 0, 16);
}

// FUNCTION: SWKOTOR 0x005DD990
CERFString::~CERFString()
{
    if (m_sText) {
        delete[] m_sText;
        m_sText = 0;
    }
    m_nLanguage = 0;
    m_nLength = 0;
}

// FUNCTION: SWKOTOR 0x005DD9C0
CERFFile::CERFFile()
{
    memset(this, 0, 0xA0);
    m_pFile = 0;
    m_nMaxEntries = 0;
    m_nCurrentEntry = 0;
}

// FUNCTION: SWKOTOR 0x005DDA50
int CERFFile::ReserveEntries(unsigned long nEntries)
{
    CERFKey cKey;
    if (m_pFile) {
        m_nMaxEntries = nEntries;
        m_nCurrentEntry = 0;
        m_pFile->Seek(0x10, 0);
        m_pFile->Write(&nEntries, 4, 1);
        m_pFile->Seek(m_nB4, 0);
        unsigned long i;
        for (i = 0; i < nEntries; i++) {
            m_pFile->Write(&cKey, 0x18, 1);
        }
        m_nB8 = m_pFile->GetOffset();
        m_pFile->Seek(0x1C, 0);
        m_pFile->Write(&m_nB8, 4, 1);
        m_pFile->SeekEnd();
        unsigned long nPlaceholder = 0xBAADF00D;
        for (i = 0; i < nEntries; i++) {
            m_pFile->Write(&nPlaceholder, 4, 1);
            m_pFile->Write(&nPlaceholder, 4, 1);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005DDBC0
int CERFFile::AddFile(CExoString& sFileName, int nUnused)
{
    if (m_nCurrentEntry < m_nMaxEntries && m_pFile) {
        CERFKey cKey;
        CERFRes cRes;
        CExoFile* pFile = new CExoFile(sFileName, 0xFFFF, "rb");
        int nSlash = sFileName.Find("\\", 0);
        CExoString sName = sFileName.Right(sFileName.GetLength() - nSlash - 1);
        int nDot = sName.Find(".", 0);
        CExoString sExtension = sName.Right(sName.GetLength() - nDot - 1);
        sName = sName.Left(nDot);
        int nResType = g_pExoBase->GetResTypeFromExtension(sExtension);
        memset(cKey.m_cResRef, 0, 16);
        memcpy(cKey.m_cResRef, sName.CStr(), sName.GetLength());
        cKey.m_nResID = m_nCurrentEntry;
        cKey.m_nResType = (unsigned short)nResType;
        unsigned long nSize = pFile->GetSize();
        char* pData = new char[nSize + 1];
        unsigned long nOffset = m_pFile->GetOffset();
        pFile->Read(pData, nSize, 1);
        m_pFile->Write(pData, nSize, 1);
        m_pFile->Seek(m_nB8 + m_nCurrentEntry * 8, 0);
        m_pFile->Write(&nOffset, 4, 1);
        m_pFile->Write(&nSize, 4, 1);
        m_pFile->SeekEnd();
        delete pFile;
        m_pFile->Seek(m_nB4 + m_nCurrentEntry * 0x18, 0);
        cKey.Write(m_pFile);
        m_pFile->SeekEnd();
        m_nCurrentEntry++;
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005DDF30
int CERFFile::AddResource(const char* szResRef, unsigned short nResType, void* pResource, int bGFF,
                          int nUnused)
{
    CERFKey cKey;
    CERFRes cRes;
    if (m_nCurrentEntry < m_nMaxEntries && m_pFile) {
        CExoString sResRef(szResRef);
        memset(cKey.m_cResRef, 0, 16);
        memcpy(cKey.m_cResRef, sResRef.CStr(), sResRef.GetLength());
        cKey.m_nResID = m_nCurrentEntry;
        cKey.m_nResType = nResType;
        if (bGFF) {
            unsigned long nOffset = m_pFile->GetOffset();
            CResGFF* pGFF = (CResGFF*)pResource;
            pGFF->Pack(0, 0);
            pGFF->GetTotalSize();
            unsigned long nSize;
            pGFF->WriteGFFData(m_pFile, nSize);
            m_pFile->Seek(m_nB8 + m_nCurrentEntry * 8, 0);
            m_pFile->Write(&nOffset, 4, 1);
            m_pFile->Write(&nSize, 4, 1);
        }
        else {
            CERFRawResource* pRaw = (CERFRawResource*)pResource;
            cRes.m_nSize = pRaw->GetSize();
            char* pData = new char[pRaw->GetSize()];
            memcpy(pData, pRaw->GetData(), pRaw->GetSize());
            cRes.m_pData = pData;
            cRes.Write(m_pFile, m_nB8 + m_nCurrentEntry * 8);
        }
        m_pFile->Seek(m_nB4 + m_nCurrentEntry * 0x18, 0);
        cKey.Write(m_pFile);
        m_pFile->SeekEnd();
        m_nCurrentEntry++;
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005DE1A0
void CERFFile::AddDirectory(CExoString& sDirectory, int nUnused, ERFProgressCallback pCallback,
                            void* pParam1, void* pParam2)
{
    CExoArrayList<CExoString> aFiles;
    CExoString sPath;
    g_pExoBase->GetDirectoryList(&aFiles, sDirectory, 0xFFFF, 0, 0);
    ReserveEntries(aFiles.num);
    for (int i = 0; i < aFiles.num; i++) {
        sPath.Format("%s\\%s", sDirectory.CStr(), aFiles[i]);
        AddFile(sPath, nUnused);
        if (pCallback) {
            pCallback(i, aFiles.num, pParam1, pParam2);
        }
    }
}

// FUNCTION: SWKOTOR 0x005DE2C0
int CERFFile::Reset()
{
    memset(this, 0, 0xA0);
    while (!m_lStrings.IsEmpty()) {
        CERFString* pString = m_lStrings.RemoveHead();
        delete pString;
    }
    while (!m_lKeys.IsEmpty()) {
        CERFKey* pKey = m_lKeys.RemoveHead();
        delete pKey;
    }
    while (!m_lResources.IsEmpty()) {
        CERFRes* pRes = m_lResources.RemoveHead();
        delete pRes;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005DE3A0
CERFFile::~CERFFile()
{
    Reset();
}
