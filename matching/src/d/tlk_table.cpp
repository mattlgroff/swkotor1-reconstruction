// CTlkFile / CTlkTable (talk table, dialog.tlk). Layout from
// docs/batches/b0041_gui_load_tlk.md and the disassembly.
#include "d/exo_key_table.h"

extern "C" void* __cdecl memset(void* p, int c, unsigned int n);
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int n);
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int n);
#pragma intrinsic(memcmp)

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

// 0x24 bytes (dtor 0x0059ea20); a row of StringTokens.2da, sorted by nHash.
struct STR_TOKEN {
    unsigned long nHash;                    // 0x00
    CExoString sToken;                      // 0x04 (column 0)
    unsigned long nActionCode;              // 0x0c (column 1)
    unsigned long aStrRefs[4];              // 0x10 (columns 3-6)
    unsigned long nNumStrRefs;              // 0x20 (column 2)
};

class C2DA {
public:
    C2DA(CResRef cResRef, int bAutoRequest);    // 0x00413cc0
    virtual ~C2DA();                            // 0x00413d80
    int Load2DArray();                          // 0x004143b0
    int GetCExoStringEntry(int nRow, int nColumn, CExoString* pValue); // 0x00413190
    int GetINTEntry(int nRow, int nColumn, int* pValue);               // 0x00413510
    char pad_0x04[0x20];
    int m_nNumRows;                             // 0x24
    char pad_0x28[0x2c];
};

unsigned long __cdecl ComputeCRC(const char* pData, unsigned long nLength); // 0x00410560

// vtable 0x0073ecdc
class CTlkTable {
public:
    CTlkTable();                            // 0x0041d8d0
    ~CTlkTable();                           // 0x0041e100
    virtual void ProcessToken(STR_TOKEN* pToken, CExoString* pValue); // 0x005b5e90

    int AddFile(const CExoString& sFileName, int nIndex);
    int OpenFile(const CExoString& sFileName);  // 0x0041e5a0
    void SetCustomToken(int nToken, const CExoString& sValue);
    void ClearCustomTokens();
    void ParseTokens(CExoString* pString);  // 0x0041dd30
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
                                        ParseTokens(&sString);
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

// NONMATCH: 45 insns before the stack shifts; structure and calls line up.
// The original evaluates `sText + nTokenStart` separately in the CUSTOM and
// hash branches (cl hoists it above the '>' scan here), spills the token
// array / hash to the frame (0x50 vs 0x48) and keeps sText in ebx there.
// FUNCTION: SWKOTOR 0x0041dd30
void CTlkTable::ParseTokens(CExoString* pString)
{
    CExoString sResult;
    CExoString sToken;
    int nStart = 0;
    m_nParseState = 0;
    char* sText = pString->CStr();
    int i = 0;
    if (!sText[0]) {
        return;
    }
    do {
        if (sText[i] == '<') {
            if (sText[i + 1] == '<') {
                i++;
                if (!m_nParseState) {
                    sResult = sResult + CExoString(sText + nStart, i - nStart);
                }
                nStart = i + 1;
            }
            else {
                if (!m_nParseState) {
                    sResult = sResult + CExoString(sText + nStart, i - nStart);
                }
                sToken = "";
                int nTokenStart = i + 1;
                for (i = nTokenStart; sText[i]; i++) {
                    if (sText[i] != '>') {
                        continue;
                    }
                    if (nTokenStart < i) {
                        unsigned int nLength = i - nTokenStart;
                        if (nLength >= 7 && !memcmp(sText + nTokenStart, "CUSTOM", 6)) {
                            int nNumber = 0;
                            int nScale = 1;
                            int j = i - 1;
                            if (j >= nTokenStart + 6) {
                                do {
                                    if (sText[j] < '0' || sText[j] > '9') {
                                        goto done;
                                    }
                                    nNumber += (sText[j] - '0') * nScale;
                                    nScale *= 10;
                                    j--;
                                } while (j >= nTokenStart + 6);
                                if (nNumber == -1) {
                                    goto done;
                                }
                            }
                            unsigned long nLow = 0;
                            unsigned long nHigh = m_nTokensCustom;
                            while (nHigh > nLow) {
                                unsigned long nMid = (nLow + nHigh) >> 1;
                                if (m_pTokensCustom[nMid].nNumber > (unsigned long)nNumber) {
                                    nHigh = nMid;
                                }
                                else if (m_pTokensCustom[nMid].nNumber < (unsigned long)nNumber) {
                                    nLow = nMid + 1;
                                }
                                else {
                                    sToken = m_pTokensCustom[nMid].sValue;
                                    break;
                                }
                            }
                        }
                        else {
                            char* sName = sText + nTokenStart;
                            unsigned long nHash = ComputeCRC(sName, nLength);
                            unsigned long nLow = 0;
                            unsigned long nHigh = m_nTokens;
                            while (nHigh > nLow) {
                                unsigned long nMid = (nLow + nHigh) >> 1;
                                if (m_pTokens[nMid].nHash > nHash) {
                                    nHigh = nMid;
                                }
                                else if (m_pTokens[nMid].nHash < nHash) {
                                    nLow = nMid + 1;
                                }
                                else {
                                    if (m_pTokens[nMid].sToken.GetLength() == nLength &&
                                        !strncmp(m_pTokens[nMid].sToken.CStr(), sName, nLength)) {
                                        ProcessToken(&m_pTokens[nMid], &sToken);
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    break;
                }
            done:
                sResult = sResult + sToken;
                nStart = i + 1;
            }
        }
        i++;
    } while (sText[i]);
    if (nStart > 0) {
        if (nStart < i && !m_nParseState) {
            sResult = sResult + CExoString(sText + nStart);
        }
        *pString = sResult;
    }
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

// FUNCTION: SWKOTOR 0x0041e5a0
int CTlkTable::OpenFile(const CExoString& sFileName)
{
    if (!AddFile(sFileName, 0)) {
        return 0;
    }
    if (m_pTokens) {
        delete[] m_pTokens;
    }
    STR_TOKEN cSwap;
    C2DA cTokens(CResRef("StringTokens"), 0);
    if (!cTokens.Load2DArray()) {
        return 0;
    }
    m_nTokens = cTokens.m_nNumRows;
    m_pTokens = new STR_TOKEN[m_nTokens];
    int nValue;
    for (unsigned long i = 0; i < m_nTokens; i++) {
        cTokens.GetCExoStringEntry(i, 0, &m_pTokens[i].sToken);
        cTokens.GetINTEntry(i, 1, &nValue);
        m_pTokens[i].nActionCode = nValue;
        cTokens.GetINTEntry(i, 2, &nValue);
        m_pTokens[i].nNumStrRefs = nValue;
        for (unsigned long k = 0; k < 4; k++) {
            cTokens.GetINTEntry(i, k + 3, &nValue);
            m_pTokens[i].aStrRefs[k] = nValue;
        }
        m_pTokens[i].nHash = ComputeCRC(m_pTokens[i].sToken.CStr(), m_pTokens[i].sToken.GetLength());
        for (unsigned long j = i; j > 0; j--) {
            if (m_pTokens[j].nHash >= m_pTokens[j - 1].nHash) {
                break;
            }
            cSwap = m_pTokens[j - 1];
            m_pTokens[j - 1] = m_pTokens[j];
            m_pTokens[j] = cSwap;
        }
    }
    return 1;
}
