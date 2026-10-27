// CSWCMessage::SendPlayerToServerLogin_LoginLocalCharacter(const CResRef&, int):
// loads the character file and forwards it to the data overload (0x00677FA0).

class CExoString {
public:
    char* m_sString;
    int m_nBufferLength;
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString operator+(const CExoString& s) const;    // 0x005E5D10
    int CompareNoCase(const char* s) const;         // 0x005E58C0
};

class CResRef {
public:
    void CopyToString(CExoString& s) const;         // 0x00405F70
    char m_resRef[16];
};

class CExoFile {
public:
    CExoFile(const CExoString& sFile, int nType, const CExoString& sMode);   // 0x005E68D0
    ~CExoFile();                                    // 0x005E69E0
    unsigned long GetSize();                        // 0x005E6950
    unsigned long Read(void* pBuffer, unsigned long nSize, unsigned long nCount);   // 0x005E6960
    int FileOpened();                               // 0x005E6A10
    void* m_pInternal;
};

void* operator new(unsigned int nSize);
void operator delete(void* p);
void* operator new[](unsigned int nSize);
void operator delete[](void* p);

class CSWCMessage {
public:
    int SendPlayerToServerLogin_LoginLocalCharacter(unsigned char* pData, unsigned long nSize,
                                                    int bFlag);   // 0x00677FA0
    int SendPlayerToServerLogin_LoginLocalCharacter(const CResRef& cCharacter, int bFlag);
};

// FUNCTION: SWKOTOR 0x00678080
// NONMATCH (15 diff insns): instructions agree; frame slots differ. The
// original starts its locals 8-aligned (unused 4 bytes at esp+0xc), ours packs
// the alias temporary there. Named alias locals did not help.
int CSWCMessage::SendPlayerToServerLogin_LoginLocalCharacter(const CResRef& cCharacter, int bFlag)
{
    CExoString sFile;
    cCharacter.CopyToString(sFile);
    if (sFile.CompareNoCase("test")) {
        sFile = CExoString("LOCALVAULT:") + sFile;
    }
    else {
        sFile = CExoString("TEMP:") + sFile;
    }
    CExoFile* pFile = new CExoFile(sFile, 0x7df, CExoString("rb"));
    if (!pFile->FileOpened()) {
        delete pFile;
        return 0;
    }
    unsigned long nSize = pFile->GetSize();
    unsigned char* pData = new unsigned char[nSize + 4];
    pFile->Read(pData, 1, nSize);
    delete pFile;
    int bResult = SendPlayerToServerLogin_LoginLocalCharacter(pData, nSize, bFlag);
    delete[] pData;
    return bResult;
}
