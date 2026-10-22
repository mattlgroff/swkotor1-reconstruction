// C2DA (two-dimensional array resource) and its
// CResHelper<CRes2DA, 2017> base. Layout from docs/batches/b0041_c2da_lookup.md
// and b0041_gff_lifecycle.md.
#include "d/exo_key_table.h"

extern "C" int __cdecl _stricmp(const char* a, const char* b);
extern "C" int __cdecl sscanf(const char* s, const char* fmt, ...);

struct C2DAHeader {
    unsigned long m_nFileType;      // 0x00 "2DA "
    unsigned long m_nFileVersion;   // 0x04 "V2.0" / "V2.b"
};

// 0x3c bytes, vtable 0x0073ec9c
class CRes2DA : public CRes {
public:
    CRes2DA();                                      // 0x0041d730
    virtual ~CRes2DA();                             // 0x0041d760
    virtual int OnResourceFreed();                  // 0x0041d770
    virtual int OnResourceServiced();               // 0x0041d790

    int m_bLoaded;                  // 0x28
    unsigned long m_nDataSize;      // 0x2c
    char* m_pData;                  // 0x30
    C2DAHeader* m_pHeader;          // 0x34
    int m_bBinary;                  // 0x38
};

template <class T, int nType>
class CResHelper {
public:
    CResHelper()
    {
        m_pRes = 0;
        m_cResRef = "";
        m_bAutoRequest = 0;
    }
    virtual ~CResHelper();
    void SetResRef(const CResRef& cResRef, int bAutoRequest);

    int m_bAutoRequest;     // 0x04
    T* m_pRes;              // 0x08
    CResRef m_cResRef;      // 0x0c
};

// vtable 0x0073e2f8
class C2DA : public CResHelper<CRes2DA, 2017> {
public:
    C2DA(CResRef cResRef, int bAutoRequest);
    virtual ~C2DA();

    int GetColumnIndex(const CExoString& sColumn);
    int GetRowIndex(const CExoString& sRow);
    int GetCExoStringEntry(int nRow, int nColumn, CExoString* pValue);
    int GetCExoStringEntry(int nRow, const CExoString& sColumn, CExoString* pValue);
    int GetCExoStringEntry(const CExoString& sRow, const CExoString& sColumn, CExoString* pValue);
    int GetCExoStringEntry(const CExoString& sRow, int nColumn, CExoString* pValue);
    int GetFLOATEntry(int nRow, int nColumn, float* pValue);
    int GetFLOATEntry(int nRow, const CExoString& sColumn, float* pValue);
    int GetFLOATEntry(const CExoString& sRow, const CExoString& sColumn, float* pValue);
    int GetFLOATEntry(const CExoString& sRow, int nColumn, float* pValue);
    int GetINTEntry(int nRow, int nColumn, int* pValue);
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);
    int GetINTEntry(const CExoString& sRow, const CExoString& sColumn, int* pValue);
    int GetINTEntry(const CExoString& sRow, int nColumn, int* pValue);
    int GetNextToken(char** ppText, unsigned long* pRemaining, char* pToken, unsigned long* pSkipped);
    int SkipNewLines(char** ppText, unsigned long* pRemaining);
    unsigned long GetLineLength(char* pText, unsigned long nSize);
    void Unload2DArray();

    CExoString m_sDefaultValue;         // 0x1c
    int m_nNumRows;                     // 0x24
    int m_nNumColumns;                  // 0x28
    int m_bLoaded;                      // 0x2c
    CExoString* m_pRowLabel;            // 0x30
    CExoString* m_pColumnLabel;         // 0x34
    CExoString** m_pArrayData;          // 0x38
    int m_bBinary;                      // 0x3c
    char* m_pBinaryData;                // 0x40
    unsigned short* m_pCellOffsets;     // 0x44
    char* m_pLabelData;                 // 0x48
    unsigned short* m_pRowLabelOffsets; // 0x4c
    unsigned short* m_pColumnLabelOffsets; // 0x50
};

// FUNCTION: SWKOTOR 0x00413100
int C2DA::GetColumnIndex(const CExoString& sColumn)
{
    int i;
    if (m_bBinary) {
        for (i = 0; i < m_nNumColumns; i++) {
            if (_stricmp(m_pLabelData + m_pColumnLabelOffsets[i], sColumn.CStr()) == 0) {
                return i;
            }
        }
        return -1;
    }
    for (i = 0; i < m_nNumColumns; i++) {
        if (m_pColumnLabel[i].CompareNoCase(sColumn)) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: SWKOTOR 0x00413190 SYMBOL: ?GetCExoStringEntry@C2DA@@QAEHHHPAVCExoString@@@Z
int C2DA::GetCExoStringEntry(int nRow, int nColumn, CExoString* pValue)
{
    if (nRow < 0 || nRow >= m_nNumRows || nColumn < 0 || nColumn >= m_nNumColumns) {
        *pValue = m_sDefaultValue;
        return 0;
    }
    if (m_bBinary) {
        int nOffset = m_pCellOffsets[m_nNumColumns * nRow + nColumn];
        return (*pValue = m_pBinaryData + nOffset)[0] != 0;
    }
    if (m_pArrayData[nRow][nColumn] == "") {
        *pValue = "";
        return 0;
    }
    *pValue = m_pArrayData[nRow][nColumn];
    return pValue->GetLength() > 0;
}

// FUNCTION: SWKOTOR 0x00413270 SYMBOL: ?GetCExoStringEntry@C2DA@@QAEHHABVCExoString@@PAV2@@Z
int C2DA::GetCExoStringEntry(int nRow, const CExoString& sColumn, CExoString* pValue)
{
    int nColumn = GetColumnIndex(sColumn);
    if (nRow < 0 || nRow >= m_nNumRows || nColumn == -1) {
        *pValue = m_sDefaultValue;
        return 0;
    }
    if (m_bBinary) {
        int nOffset = m_pCellOffsets[m_nNumColumns * nRow + nColumn];
        return (*pValue = m_pBinaryData + nOffset)[0] != 0;
    }
    if (m_pArrayData[nRow][nColumn] == "") {
        *pValue = "";
        return 0;
    }
    *pValue = m_pArrayData[nRow][nColumn];
    return pValue->GetLength() > 0;
}

// FUNCTION: SWKOTOR 0x00413350 SYMBOL: ?GetFLOATEntry@C2DA@@QAEHHHPAM@Z
int C2DA::GetFLOATEntry(int nRow, int nColumn, float* pValue)
{
    if (nRow < 0 || nRow >= m_nNumRows || nColumn < 0 || nColumn >= m_nNumColumns) {
        *pValue = m_sDefaultValue.AsFLOAT();
        return 0;
    }
    if (m_bBinary) {
        int nOffset = m_pCellOffsets[m_nNumColumns * nRow + nColumn];
        *pValue = 0.0f;
        sscanf(m_pBinaryData + nOffset, "%f", pValue);
        return m_pBinaryData[nOffset] != 0;
    }
    if (m_pArrayData[nRow][nColumn] == "") {
        *pValue = 0.0f;
        return 0;
    }
    *pValue = m_pArrayData[nRow][nColumn].AsFLOAT();
    return 1;
}

// FUNCTION: SWKOTOR 0x00413430 SYMBOL: ?GetFLOATEntry@C2DA@@QAEHHABVCExoString@@PAM@Z
int C2DA::GetFLOATEntry(int nRow, const CExoString& sColumn, float* pValue)
{
    int nColumn = GetColumnIndex(sColumn);
    if (nRow < 0 || nRow >= m_nNumRows || nColumn == -1) {
        *pValue = m_sDefaultValue.AsFLOAT();
        return 0;
    }
    if (m_bBinary) {
        int nOffset = m_pCellOffsets[m_nNumColumns * nRow + nColumn];
        *pValue = 0.0f;
        sscanf(m_pBinaryData + nOffset, "%f", pValue);
        return m_pBinaryData[nOffset] != 0;
    }
    if (m_pArrayData[nRow][nColumn] == "") {
        *pValue = 0.0f;
        return 0;
    }
    *pValue = m_pArrayData[nRow][nColumn].AsFLOAT();
    return 1;
}

// FUNCTION: SWKOTOR 0x00413510 SYMBOL: ?GetINTEntry@C2DA@@QAEHHHPAH@Z
int C2DA::GetINTEntry(int nRow, int nColumn, int* pValue)
{
    if (nRow < 0 || nRow >= m_nNumRows || nColumn < 0 || nColumn >= m_nNumColumns) {
        *pValue = m_sDefaultValue.AsINT();
        return 0;
    }
    if (m_bBinary) {
        int nOffset = m_pCellOffsets[m_nNumColumns * nRow + nColumn];
        *pValue = 0;
        sscanf(m_pBinaryData + nOffset, "%i", pValue);
        return m_pBinaryData[nOffset] != 0;
    }
    if (m_pArrayData[nRow][nColumn] == "") {
        *pValue = 0;
        return 0;
    }
    if (m_pArrayData[nRow][nColumn].GetLength() > 2 && m_pArrayData[nRow][nColumn].CStr()[0] == '0'
        && (m_pArrayData[nRow][nColumn].CStr()[1] == 'x' || m_pArrayData[nRow][nColumn].CStr()[1] == 'X')) {
        sscanf(m_pArrayData[nRow][nColumn].CStr(), "%x", pValue);
        return 1;
    }
    *pValue = m_pArrayData[nRow][nColumn].AsINT();
    return 1;
}

// FUNCTION: SWKOTOR 0x00413660 SYMBOL: ?GetINTEntry@C2DA@@QAEHHABVCExoString@@PAH@Z
int C2DA::GetINTEntry(int nRow, const CExoString& sColumn, int* pValue)
{
    int nColumn = GetColumnIndex(sColumn);
    if (nRow < 0 || nRow >= m_nNumRows || nColumn == -1) {
        *pValue = m_sDefaultValue.AsINT();
        return 0;
    }
    if (m_bBinary) {
        int nOffset = m_pCellOffsets[m_nNumColumns * nRow + nColumn];
        *pValue = 0;
        sscanf(m_pBinaryData + nOffset, "%i", pValue);
        return m_pBinaryData[nOffset] != 0;
    }
    if (m_pArrayData[nRow][nColumn] == "") {
        *pValue = 0;
        return 0;
    }
    if (m_pArrayData[nRow][nColumn].GetLength() > 2 && m_pArrayData[nRow][nColumn].CStr()[0] == '0'
        && (m_pArrayData[nRow][nColumn].CStr()[1] == 'x' || m_pArrayData[nRow][nColumn].CStr()[1] == 'X')) {
        sscanf(m_pArrayData[nRow][nColumn].CStr(), "%x", pValue);
        return 1;
    }
    *pValue = m_pArrayData[nRow][nColumn].AsINT();
    return 1;
}

// FUNCTION: SWKOTOR 0x004137b0
unsigned long C2DA::GetLineLength(char* pText, unsigned long nSize)
{
    unsigned long i = 0;
    while (i < nSize && *pText != '\n' && *pText != '\r') {
        pText++;
        i++;
    }
    return i;
}

// FUNCTION: SWKOTOR 0x004137e0
int C2DA::GetNextToken(char** ppText, unsigned long* pRemaining, char* pToken, unsigned long* pSkipped)
{
    if (*pRemaining == 0) {
        *pToken = 0;
        return 0;
    }
    char* p = *ppText;
    unsigned long nConsumed = 0;
    unsigned long nSkipped = 0;
    if (*p == 0) {
        p++;
    }
    while ((nSkipped < *pRemaining && *p == ' ') || *p == '\t') {
        p++;
        nSkipped++;
    }
    *pSkipped = nSkipped;
    *pRemaining -= nSkipped;
    if (*p == '"') {
        p++;
        while (nConsumed < *pRemaining && *p != 0 && *p != '"') {
            *pToken++ = *p++;
            nConsumed++;
        }
        if (*p == '"') {
            p++;
            nConsumed++;
        }
    }
    else {
        while (nConsumed < *pRemaining && *p != 0 && *p != ' ' && *p != '\t' && *p != '\n' && *p != '\r') {
            *pToken++ = *p++;
            nConsumed++;
        }
    }
    *ppText = p;
    *pRemaining -= nConsumed;
    *pToken = 0;
    return nConsumed > 0;
}

// FUNCTION: SWKOTOR 0x004138b0
int C2DA::GetRowIndex(const CExoString& sRow)
{
    int i;
    if (m_bBinary) {
        for (i = 0; i < m_nNumRows; i++) {
            if (_stricmp(m_pLabelData + m_pRowLabelOffsets[i], sRow.CStr()) == 0) {
                return i;
            }
        }
        return -1;
    }
    for (i = 0; i < m_nNumRows; i++) {
        if (m_pRowLabel[i].CompareNoCase(sRow)) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: SWKOTOR 0x00413940
int C2DA::SkipNewLines(char** ppText, unsigned long* pRemaining)
{
    char* p = *ppText;
    unsigned long nTotal = *pRemaining;
    unsigned long nConsumed = 0;
    unsigned long nScan = 0;
    while (nConsumed < *pRemaining && (*p == '\n' || *p == '\r' || *p == ' ' || *p == '\t')) {
        nConsumed = nScan;
        while (nConsumed < *pRemaining && (*p == '\n' || *p == '\r')) {
            p++;
            nConsumed++;
        }
        nScan = nConsumed;
        while (nScan < *pRemaining && (*p == ' ' || *p == '\t')) {
            p++;
            nScan++;
        }
        if (nScan == nTotal) {
            nConsumed = nScan;
            break;
        }
    }
    *ppText += nConsumed;
    *pRemaining -= nConsumed;
    return nConsumed > 0;
}

// FUNCTION: SWKOTOR 0x004139e0
void C2DA::Unload2DArray()
{
    if (!m_bLoaded) {
        return;
    }
    if (m_pArrayData) {
        for (int i = 0; i < m_nNumRows; i++) {
            if (m_pArrayData[i]) {
                delete[] m_pArrayData[i];
            }
        }
        delete[] m_pArrayData;
        m_pArrayData = 0;
    }
    if (m_pColumnLabel) {
        delete[] m_pColumnLabel;
        m_pColumnLabel = 0;
    }
    if (m_pRowLabel) {
        delete[] m_pRowLabel;
        m_pRowLabel = 0;
    }
    m_nNumRows = 0;
    m_nNumColumns = 0;
    m_sDefaultValue = "";
    m_pRes->Release();
    m_bLoaded = 0;
    m_bBinary = 0;
    m_pBinaryData = 0;
    m_pCellOffsets = 0;
    delete[] m_pRowLabelOffsets;
    m_pRowLabelOffsets = 0;
    delete[] m_pColumnLabelOffsets;
    m_pColumnLabelOffsets = 0;
    m_pLabelData = 0;
}

// FUNCTION: SWKOTOR 0x00413ca0 SYMBOL: ??_G?$CResHelper@VCRes2DA@@$0HOB@@@UAEPAXI@Z
// FUNCTION: SWKOTOR 0x00413ae0 SYMBOL: ??1?$CResHelper@VCRes2DA@@$0HOB@@@UAE@XZ
template <class T, int nType>
CResHelper<T, nType>::~CResHelper()
{
    if (m_pRes && m_cResRef != "") {
        if (m_bAutoRequest) {
            m_pRes->CancelRequest();
            m_bAutoRequest = 0;
        }
        if (!g_pExoResMan->ReleaseResObject(m_pRes)) {
            if (m_pRes) {
                delete m_pRes;
            }
            m_pRes = 0;
        }
    }
}

// FUNCTION: SWKOTOR 0x00413b40 SYMBOL: ?SetResRef@?$CResHelper@VCRes2DA@@$0HOB@@@QAEXABVCResRef@@H@Z
template <class T, int nType>
void CResHelper<T, nType>::SetResRef(const CResRef& cResRef, int bAutoRequest)
{
    if (cResRef == m_cResRef) {
        return;
    }
    if (m_pRes && m_cResRef != "") {
        if (m_bAutoRequest) {
            m_pRes->CancelRequest();
        }
        if (!g_pExoResMan->ReleaseResObject(m_pRes)) {
            if (m_pRes) {
                delete m_pRes;
            }
            m_pRes = 0;
        }
    }
    if (cResRef == "") {
        m_pRes = 0;
        m_cResRef = "";
        return;
    }
    T* pRes = (T*)g_pExoResMan->GetResObject(cResRef, nType);
    if (!pRes) {
        pRes = new T();
        g_pExoResMan->SetResObject(cResRef, nType, pRes);
        if (!pRes) {
            return;
        }
    }
    m_pRes = pRes;
    if (bAutoRequest) {
        m_bAutoRequest = 1;
        m_pRes->Request();
    }
    m_cResRef = cResRef;
}

template class CResHelper<CRes2DA, 2017>;

// FUNCTION: SWKOTOR 0x00413cc0
C2DA::C2DA(CResRef cResRef, int bAutoRequest)
{
    m_pArrayData = 0;
    m_pRowLabel = 0;
    m_pColumnLabel = 0;
    m_nNumRows = 0;
    m_nNumColumns = 0;
    m_sDefaultValue = "";
    m_bLoaded = 0;
    m_bBinary = 0;
    m_pBinaryData = 0;
    m_pCellOffsets = 0;
    m_pRowLabelOffsets = 0;
    m_pColumnLabelOffsets = 0;
    m_pLabelData = 0;
    SetResRef(cResRef, bAutoRequest);
}

// FUNCTION: SWKOTOR 0x00414a30 SYMBOL: ??_GC2DA@@UAEPAXI@Z
// FUNCTION: SWKOTOR 0x00413d80
C2DA::~C2DA()
{
    Unload2DArray();
}

// FUNCTION: SWKOTOR 0x00413de0 SYMBOL: ?GetCExoStringEntry@C2DA@@QAEHABVCExoString@@0PAV2@@Z
int C2DA::GetCExoStringEntry(const CExoString& sRow, const CExoString& sColumn, CExoString* pValue)
{
    int nRow = GetRowIndex(sRow);
    int nColumn = GetColumnIndex(sColumn);
    if (nRow == -1 || nColumn == -1) {
        *pValue = m_sDefaultValue;
        return 0;
    }
    if (m_bBinary) {
        int nOffset = m_pCellOffsets[m_nNumColumns * nRow + nColumn];
        return (*pValue = m_pBinaryData + nOffset)[0] != 0;
    }
    if (m_pArrayData[nRow][nColumn] == "") {
        *pValue = "";
        return 0;
    }
    *pValue = m_pArrayData[nRow][nColumn];
    return pValue->GetLength() > 0;
}

// FUNCTION: SWKOTOR 0x00413ec0 SYMBOL: ?GetCExoStringEntry@C2DA@@QAEHABVCExoString@@HPAV2@@Z
int C2DA::GetCExoStringEntry(const CExoString& sRow, int nColumn, CExoString* pValue)
{
    int nRow = GetRowIndex(sRow);
    if (nRow == -1 || nColumn < 0 || nColumn >= m_nNumColumns) {
        *pValue = m_sDefaultValue;
        return 0;
    }
    if (m_bBinary) {
        int nOffset = m_pCellOffsets[m_nNumColumns * nRow + nColumn];
        return (*pValue = m_pBinaryData + nOffset)[0] != 0;
    }
    if (m_pArrayData[nRow][nColumn] == "") {
        *pValue = "";
        return 0;
    }
    *pValue = m_pArrayData[nRow][nColumn];
    return pValue->GetLength() > 0;
}

// FUNCTION: SWKOTOR 0x00413fa0 SYMBOL: ?GetFLOATEntry@C2DA@@QAEHABVCExoString@@0PAM@Z
int C2DA::GetFLOATEntry(const CExoString& sRow, const CExoString& sColumn, float* pValue)
{
    int nRow = GetRowIndex(sRow);
    int nColumn = GetColumnIndex(sColumn);
    if (nRow == -1 || nColumn == -1) {
        *pValue = m_sDefaultValue.AsFLOAT();
        return 0;
    }
    if (m_bBinary) {
        int nOffset = m_pCellOffsets[m_nNumColumns * nRow + nColumn];
        *pValue = 0.0f;
        sscanf(m_pBinaryData + nOffset, "%f", pValue);
        return m_pBinaryData[nOffset] != 0;
    }
    if (m_pArrayData[nRow][nColumn] == "") {
        *pValue = 0.0f;
        return 0;
    }
    *pValue = m_pArrayData[nRow][nColumn].AsFLOAT();
    return 1;
}

// FUNCTION: SWKOTOR 0x00414080 SYMBOL: ?GetFLOATEntry@C2DA@@QAEHABVCExoString@@HPAM@Z
int C2DA::GetFLOATEntry(const CExoString& sRow, int nColumn, float* pValue)
{
    int nRow = GetRowIndex(sRow);
    if (nRow == -1 || nColumn < 0 || nColumn >= m_nNumColumns) {
        *pValue = m_sDefaultValue.AsFLOAT();
        return 0;
    }
    if (m_bBinary) {
        int nOffset = m_pCellOffsets[m_nNumColumns * nRow + nColumn];
        *pValue = 0.0f;
        sscanf(m_pBinaryData + nOffset, "%f", pValue);
        return 1;
    }
    *pValue = m_pArrayData[nRow][nColumn].AsFLOAT();
    return 1;
}

// FUNCTION: SWKOTOR 0x00414110 SYMBOL: ?GetINTEntry@C2DA@@QAEHABVCExoString@@0PAH@Z
int C2DA::GetINTEntry(const CExoString& sRow, const CExoString& sColumn, int* pValue)
{
    int nRow = GetRowIndex(sRow);
    int nColumn = GetColumnIndex(sColumn);
    if (nRow == -1 || nColumn == -1) {
        *pValue = m_sDefaultValue.AsINT();
        return 0;
    }
    if (m_bBinary) {
        int nOffset = m_pCellOffsets[m_nNumColumns * nRow + nColumn];
        *pValue = 0;
        sscanf(m_pBinaryData + nOffset, "%i", pValue);
        return m_pBinaryData[nOffset] != 0;
    }
    if (m_pArrayData[nRow][nColumn] == "") {
        *pValue = 0;
        return 0;
    }
    if (m_pArrayData[nRow][nColumn].GetLength() > 2 && m_pArrayData[nRow][nColumn].CStr()[0] == '0'
        && (m_pArrayData[nRow][nColumn].CStr()[1] == 'x' || m_pArrayData[nRow][nColumn].CStr()[1] == 'X')) {
        sscanf(m_pArrayData[nRow][nColumn].CStr(), "%x", pValue);
        return 1;
    }
    *pValue = m_pArrayData[nRow][nColumn].AsINT();
    return 1;
}

// FUNCTION: SWKOTOR 0x00414260 SYMBOL: ?GetINTEntry@C2DA@@QAEHABVCExoString@@HPAH@Z
int C2DA::GetINTEntry(const CExoString& sRow, int nColumn, int* pValue)
{
    int nRow = GetRowIndex(sRow);
    if (nRow == -1 || nColumn < 0 || nColumn >= m_nNumColumns) {
        *pValue = m_sDefaultValue.AsINT();
        return 0;
    }
    if (m_bBinary) {
        int nOffset = m_pCellOffsets[m_nNumColumns * nRow + nColumn];
        *pValue = 0;
        sscanf(m_pBinaryData + nOffset, "%i", pValue);
        return m_pBinaryData[nOffset] != 0;
    }
    if (m_pArrayData[nRow][nColumn] == "") {
        *pValue = 0;
        return 0;
    }
    if (m_pArrayData[nRow][nColumn].GetLength() > 2 && m_pArrayData[nRow][nColumn].CStr()[0] == '0'
        && (m_pArrayData[nRow][nColumn].CStr()[1] == 'x' || m_pArrayData[nRow][nColumn].CStr()[1] == 'X')) {
        sscanf(m_pArrayData[nRow][nColumn].CStr(), "%x", pValue);
        return 1;
    }
    *pValue = m_pArrayData[nRow][nColumn].AsINT();
    return 1;
}

// FUNCTION: SWKOTOR 0x0041d730
CRes2DA::CRes2DA()
{
    m_bLoaded = 0;
    m_nDataSize = 0;
    m_pData = 0;
    m_pHeader = 0;
    m_bBinary = 0;
}

// FUNCTION: SWKOTOR 0x0041d7f0 SYMBOL: ??_GCRes2DA@@UAEPAXI@Z
// FUNCTION: SWKOTOR 0x0041d760
CRes2DA::~CRes2DA()
{
}

// FUNCTION: SWKOTOR 0x0041d770
int CRes2DA::OnResourceFreed()
{
    m_bLoaded = 0;
    m_nDataSize = 0;
    m_pData = 0;
    m_pHeader = 0;
    m_bBinary = 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x0041d790
int CRes2DA::OnResourceServiced()
{
    if (!m_pResource) {
        return 0;
    }
    if (m_bLoaded) {
        return 1;
    }
    m_nDataSize = m_nSize - 8;
    m_pHeader = (C2DAHeader*)m_pResource;
    if (m_pHeader->m_nFileType != 0x20414432) {
        OnResourceFreed();
        return 0;
    }
    if (m_pHeader->m_nFileVersion == 0x622e3256) {
        m_bBinary = 1;
    }
    else if (m_pHeader->m_nFileVersion != 0x302e3256) {
        OnResourceFreed();
        return 0;
    }
    m_pData = (char*)m_pResource + 8;
    m_bLoaded = 1;
    return 1;
}
