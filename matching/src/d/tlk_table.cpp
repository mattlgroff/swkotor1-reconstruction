// CTlkFile / CTlkTable (talk table, dialog.tlk). Layout from
// docs/batches/b0041_gui_load_tlk.md and the disassembly.
#include "d/exo_key_table.h"

extern "C" void* __cdecl memset(void* p, int c, unsigned int n);

extern const char* g_sTlkFileVersion;       // 0x0078d3ec -> "V3.0"

#define TLK_FOURCC(p) \
    ((((((unsigned long)(unsigned char)(p)[3] << 8) | (unsigned char)(p)[2]) << 8 \
       | (unsigned char)(p)[1]) << 8) | (unsigned char)(p)[0])

struct TLK_HEADER {
    unsigned long nFileType;                // 0x00 "TLK "
    unsigned long nFileVersion;             // 0x04
    unsigned long nLanguageID;              // 0x08
    unsigned long nStringCount;             // 0x0c
    unsigned long nStringEntriesOffset;     // 0x10
};

struct TLK_STRING_DATA {
    unsigned long nFlags;                   // 0x00
    char cSoundResRef[16];                  // 0x04
    unsigned long nVolumeVariance;          // 0x14
    unsigned long nPitchVariance;           // 0x18
    unsigned long nOffsetToString;          // 0x1c
    unsigned long nStringSize;              // 0x20
    float fSoundLength;                     // 0x24 (V3.0 only)
};

// operator=(const char (&)[16]) is ambiguous with operator=(const char*)
// at a call site; select it with a cast of the member address.
typedef CResRef (CResRef::*PFN_RESREF_ASSIGN16)(const char (&)[16]);

struct STR_RES {
    CExoString sString;                     // 0x00
    CResRef cSoundResRef;                   // 0x08
    float fSoundLength;                     // 0x18
};

// 0x18 bytes
class CTlkFile : public CExoFile {
public:
    CTlkFile(const CExoString& sFileName);  // 0x0041d810
    ~CTlkFile();                            // 0x0041d900
    int ReadHeader();                       // 0x0041d890

    TLK_HEADER m_header;                    // 0x04
};

// 0x0c bytes (dtor 0x0059ea20)
struct STR_TOKEN_CUSTOM {
    unsigned long nNumber;                  // 0x00
    CExoString sValue;                      // 0x04
};

// 0x24 bytes (dtor 0x0059ea20)
struct STR_TOKEN {
    unsigned long nHash;                    // 0x00
    CExoString sToken;                      // 0x04
    char pad_0xc[0x18];
};

// vtable 0x0073ecdc
class CTlkTable {
public:
    CTlkTable();                            // 0x0041d8d0
    ~CTlkTable();                           // 0x0041e100
    virtual void ProcessToken(int a, int b);    // 0x005b5e90

    int AddFile(const CExoString& sFileName, int nIndex);
    void SetCustomToken(int nToken, const CExoString& sValue);
    void ClearCustomTokens();
    void ParseStr(CExoString& sString);     // 0x0041dd30
    unsigned long GetLanguageID();
    int FetchInternal(unsigned long nStrRef, STR_RES& strRes, int bParse);
    int Fetch(unsigned long nStrRef, STR_RES& strRes, unsigned char nGender);
    CExoString GetSimpleString(unsigned long nStrRef);

    CTlkFile* m_pFiles[14];                 // 0x04 (main, feminine) x 7
    unsigned char m_nGender;                // 0x3c
    STR_TOKEN* m_pTokens;                   // 0x40
    unsigned long m_nTokens;                // 0x44
    STR_TOKEN_CUSTOM* m_pTokensCustom;      // 0x48
    unsigned long m_nTokensCustom;          // 0x4c
    int m_nParseState;                      // 0x50
    int m_bShowStrRefs;                     // 0x54
};

// FUNCTION: SWKOTOR 0x0041d810
CTlkFile::CTlkFile(const CExoString& sFileName)
    : CExoFile(sFileName, 0x7e2, "rb")
{
    memset(&m_header, 0, sizeof(TLK_HEADER));
}

// FUNCTION: SWKOTOR 0x0041d890
int CTlkFile::ReadHeader()
{
    SeekBeginning();
    if (Read(&m_header, 1, sizeof(TLK_HEADER)) != sizeof(TLK_HEADER)) {
        return 0;
    }
    return m_header.nFileType == 0x204b4c54;
}

// FUNCTION: SWKOTOR 0x0041d900
CTlkFile::~CTlkFile()
{
}

// FUNCTION: SWKOTOR 0x0041d8d0
CTlkTable::CTlkTable()
{
    m_nGender = 0;
    for (int i = 0; i < 14; i++) {
        m_pFiles[i] = 0;
    }
    m_pTokens = 0;
    m_nTokens = 0;
    m_pTokensCustom = 0;
    m_nTokensCustom = 0;
    m_bShowStrRefs = 0;
}

// FUNCTION: SWKOTOR 0x0041d920
int CTlkTable::AddFile(const CExoString& sFileName, int nIndex)
{
    if (nIndex >= 0 && nIndex <= 7 && !(sFileName == "")) {
        if (m_pFiles[nIndex * 2 + 1] == m_pFiles[nIndex * 2]) {
            m_pFiles[nIndex * 2 + 1] = 0;
        }
        if (m_pFiles[nIndex * 2 + 1]) {
            delete m_pFiles[nIndex * 2 + 1];
        }
        if (m_pFiles[nIndex * 2]) {
            delete m_pFiles[nIndex * 2];
        }
        m_pFiles[nIndex * 2] = new CTlkFile(sFileName);
        if (m_pFiles[nIndex * 2]->IsOpen()) {
            if (!m_pFiles[nIndex * 2]->ReadHeader()) {
                if (m_pFiles[nIndex * 2]) {
                    delete m_pFiles[nIndex * 2];
                }
                m_pFiles[nIndex * 2] = 0;
            }
            else {
                m_pFiles[nIndex * 2 + 1] = new CTlkFile(sFileName + CExoString("F"));
                if (!m_pFiles[nIndex * 2 + 1]->IsOpen()) {
                    if (m_pFiles[nIndex * 2 + 1]) {
                        delete m_pFiles[nIndex * 2 + 1];
                    }
                    m_pFiles[nIndex * 2 + 1] = m_pFiles[nIndex * 2];
                    return 1;
                }
                if (m_pFiles[nIndex * 2 + 1]->ReadHeader()) {
                    return 1;
                }
                if (m_pFiles[nIndex * 2]) {
                    delete m_pFiles[nIndex * 2];
                }
                m_pFiles[nIndex * 2] = 0;
                if (m_pFiles[nIndex * 2 + 1]) {
                    delete m_pFiles[nIndex * 2 + 1];
                }
                m_pFiles[nIndex * 2 + 1] = 0;
                return 0;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0041db50
void CTlkTable::SetCustomToken(int nToken, const CExoString& sValue)
{
    if (nToken < 0) {
        return;
    }
    unsigned long nLow = 0;
    unsigned long nHigh = m_nTokensCustom;
    while (nHigh > nLow) {
        unsigned long nMid = (nLow + nHigh) >> 1;
        if (m_pTokensCustom[nMid].nNumber > (unsigned long)nToken) {
            nHigh = nMid;
        }
        else if (m_pTokensCustom[nMid].nNumber < (unsigned long)nToken) {
            nLow = nMid + 1;
        }
        else {
            m_pTokensCustom[nMid].sValue = sValue;
            return;
        }
    }
    STR_TOKEN_CUSTOM* pNew = new STR_TOKEN_CUSTOM[m_nTokensCustom + 1];
    unsigned long i;
    for (i = 0; i < nLow; i++) {
        pNew[i] = m_pTokensCustom[i];
    }
    pNew[i].nNumber = nToken;
    pNew[i].sValue = sValue;
    for (; i < m_nTokensCustom; i++) {
        pNew[i + 1] = m_pTokensCustom[i];
    }
    delete[] m_pTokensCustom;
    m_pTokensCustom = pNew;
    m_nTokensCustom++;
}

// FUNCTION: SWKOTOR 0x0041dcf0
void CTlkTable::ClearCustomTokens()
{
    delete[] m_pTokensCustom;
    m_pTokensCustom = 0;
    m_nTokensCustom = 0;
}

// FUNCTION: SWKOTOR 0x0041e0f0
unsigned long CTlkTable::GetLanguageID()
{
    if (m_pFiles[0]) {
        return m_pFiles[0]->m_header.nLanguageID;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0041e100
CTlkTable::~CTlkTable()
{
    delete[] m_pTokensCustom;
    delete[] m_pTokens;
    CTlkFile** ppFile = &m_pFiles[1];
    for (int i = 0; i < 7; i++) {
        if (ppFile[0] && ppFile[0] != ppFile[-1]) {
            delete ppFile[0];
        }
        if (ppFile[-1]) {
            delete ppFile[-1];
        }
        ppFile += 2;
    }
}

// FUNCTION: SWKOTOR 0x0041e1a0
int CTlkTable::FetchInternal(unsigned long nStrRef, STR_RES& strRes, int bParse)
{
    CExoString sString;
    int bFound = 0;
    CExoString sError;
    sError.Format("Invalid STRREF %d passed to Fetch", nStrRef);
    if (nStrRef == 0xffffffff || m_nGender > 1) {
        strRes.sString = "";
        strRes.cSoundResRef = "";
        return 1;
    }
    for (int i = 0; i < 14; i += 2) {
        CTlkFile* pFile = m_pFiles[m_nGender + i];
        if (pFile) {
            unsigned long nEntrySize = (pFile->m_header.nFileVersion == TLK_FOURCC(g_sTlkFileVersion)) ? 0x28 : 0x24;
            nStrRef &= 0xffffff;
            if (nStrRef < pFile->m_header.nStringCount) {
                unsigned long nOffset = nEntrySize * nStrRef + sizeof(TLK_HEADER);
                if (nOffset <= pFile->GetSize() && pFile->Seek(nOffset, 0)) {
                    TLK_STRING_DATA cData;
                    cData.fSoundLength = 0;
                    if (pFile->Read(&cData, 1, nEntrySize) == nEntrySize && !(cData.nFlags & 0x8000)) {
                        bFound = 1;
                        if (cData.nFlags & 4) {
                            strRes.fSoundLength = cData.fSoundLength;
                        }
                        else {
                            strRes.fSoundLength = 0;
                        }
                        if (cData.nFlags & 2) {
                            (strRes.cSoundResRef.*(PFN_RESREF_ASSIGN16)&CResRef::operator=)(cData.cSoundResRef);
                        }
                        else {
                            strRes.cSoundResRef = "";
                        }
                        if (cData.nFlags & 1) {
                            unsigned long nStringStart = pFile->m_header.nStringEntriesOffset;
                            if (nStringStart + cData.nOffsetToString <= pFile->GetSize()) {
                                if (pFile->Seek(pFile->m_header.nStringEntriesOffset + cData.nOffsetToString, 0)) {
                                    char* pText = new char[cData.nStringSize];
                                    pFile->Read(pText, 1, cData.nStringSize);
                                    sString = CExoString(pText, cData.nStringSize);
                                    delete[] pText;
                                    if (bParse) {
                                        ParseStr(sString);
                                    }
                                    strRes.sString = sString;
                                }
                                else {
                                    strRes.sString = "";
                                    strRes.cSoundResRef = "";
                                    return 0;
                                }
                            }
                            else {
                                strRes.sString = CExoString((int)nStrRef);
                                strRes.cSoundResRef = "";
                                return 0;
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
    if (m_nGender == 1) {
        if (!bFound) {
            m_nGender = 0;
            return FetchInternal(nStrRef, strRes, bParse);
        }
    }
    else if (!bFound) {
        strRes.sString = "";
        strRes.cSoundResRef = "";
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0041e550
int CTlkTable::Fetch(unsigned long nStrRef, STR_RES& strRes, unsigned char nGender)
{
    m_nGender = nGender;
    int bResult = FetchInternal(nStrRef, strRes, 0);
    if (m_bShowStrRefs) {
        strRes.sString.Format("[%d]%s", nStrRef, strRes.sString.CStr());
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x0041e8f0
CExoString CTlkTable::GetSimpleString(unsigned long nStrRef)
{
    STR_RES strRes;
    m_nGender = 0;
    FetchInternal(nStrRef, strRes, 0);
    if (m_bShowStrRefs) {
        strRes.sString.Format("[%d]%s", nStrRef, strRes.sString.CStr());
    }
    return strRes.sString;
}

// FUNCTION: SWKOTOR 0x0041d910 SYMBOL: ??0STR_TOKEN_CUSTOM@@QAE@XZ
