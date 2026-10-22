// CExoFile / CExoIni / CExoAliasList facades (b005e3_exo_file_wrappers,
// b005e4_exo_pimpl_ctors).

#include <stdio.h>
#include <string.h>
#include "c/exo_base.h"

class CExoFileInternal {
public:
    FILE* m_pFile;                                  // 0x00
    char pad_0x4[0x10];

    int Flush() { if (m_pFile) { return fflush(m_pFile) == 0; } return 0; }
    int Seek(int nOffset, int nOrigin) { if (m_pFile) { return fseek(m_pFile, nOffset, nOrigin) == 0; } return 0; }

    CExoFileInternal(const CExoString& sFileName, int nResType, const CExoString& sMode);   // 0x005EB840
    ~CExoFileInternal();                            // 0x005EB900
    unsigned long Write(const void* pData, unsigned long nSize, unsigned long nCount);   // 0x005EBC60
};

class CExoFile {
public:
    CExoFileInternal* m_pcExoFileInternal;          // 0x00

    CExoFile(const CExoString& sFileName, int nResType, const CExoString& sMode);
    ~CExoFile();
    int FileOpened();
    int Flush();
    int Write(const char* s);
    int Write(const CExoString& s);
    int Seek(int nOffset, int nOrigin);
    int SeekBeginning();
    int SeekEnd();
};

// FUNCTION: SWKOTOR 0x005E68D0
CExoFile::CExoFile(const CExoString& sFileName, int nResType, const CExoString& sMode)
{
    m_pcExoFileInternal = new CExoFileInternal(sFileName, nResType, sMode);
}

// FUNCTION: SWKOTOR 0x005E69E0
CExoFile::~CExoFile()
{
    if (m_pcExoFileInternal) {
        delete m_pcExoFileInternal;
        m_pcExoFileInternal = 0;
    }
}

// FUNCTION: SWKOTOR 0x005E6A10
int CExoFile::FileOpened()
{
    return m_pcExoFileInternal->m_pFile != 0;
}

// FUNCTION: SWKOTOR 0x005E6A20
int CExoFile::Flush()
{
    return m_pcExoFileInternal->Flush();
}

// FUNCTION: SWKOTOR 0x005E6A40 SYMBOL: ?Write@CExoFile@@QAEHPBD@Z
int CExoFile::Write(const char* s)
{
    CExoFileInternal* p = m_pcExoFileInternal;
    return p->Write(s, 1, strlen(s));
}

// FUNCTION: SWKOTOR 0x005E6A70 SYMBOL: ?Write@CExoFile@@QAEHABVCExoString@@@Z
int CExoFile::Write(const CExoString& s)
{
    CExoFileInternal* p = m_pcExoFileInternal;
    return p->Write(s.CStr(), 1, s.GetLength());
}

// FUNCTION: SWKOTOR 0x005E6AA0
int CExoFile::Seek(int nOffset, int nOrigin)
{
    return m_pcExoFileInternal->Seek(nOffset, nOrigin);
}

// FUNCTION: SWKOTOR 0x005E6AD0
int CExoFile::SeekBeginning()
{
    return m_pcExoFileInternal->Seek(0, SEEK_SET);
}

// FUNCTION: SWKOTOR 0x005E6AF0
int CExoFile::SeekEnd()
{
    return m_pcExoFileInternal->Seek(0, SEEK_END);
}
