// CResGFF field lookup, typed readers and builders.
#include "d/res_gff.h"

inline CResRef::CResRef(const CResRef& cResRef)
{
    memcpy(m_resRef, cResRef.m_resRef, 16);
}

inline int LabelsEqual(const CResGFFLabel& a, const CResGFFLabel& b)
{
    const unsigned long* pA = (const unsigned long*)a.m_pLabel;
    const unsigned long* pB = (const unsigned long*)b.m_pLabel;
    return pA[0] == pB[0] && pA[1] == pB[1] && pA[2] == pB[2] && pA[3] == pB[3];
}

inline unsigned long MakeFourCC(const char* p)
{
    return (((((unsigned char)p[3] << 8) | (unsigned char)p[2]) << 8 | (unsigned char)p[1]) << 8)
           | (unsigned char)p[0];
}

// FUNCTION: SWKOTOR 0x00410930 SYMBOL: ?GetField@CResGFF@@QAEPAUCResGFFField@@PAUCResGFFStruct@@K@Z
CResGFFField* CResGFF::GetField(CResGFFStruct* pStruct, unsigned long nIndex)
{
    if (pStruct == 0 || m_pFieldBuffer == 0 || nIndex >= pStruct->m_nFieldCount) {
        return 0;
    }
    if (pStruct->m_nFieldCount != 1) {
        unsigned long nOffset = pStruct->m_nDataOrDataOffset + nIndex * 4;
        if (m_pFieldIndicesArray == 0) {
            return 0;
        }
        if (nOffset + 4 > m_pFileHeader->m_nFieldIndicesCount) {
            return 0;
        }
        return &m_pFieldBuffer[*(unsigned long*)(m_pFieldIndicesArray + nOffset)];
    }
    if (pStruct->m_nDataOrDataOffset >= m_pFileHeader->m_nFieldCount) {
        return 0;
    }
    return &m_pFieldBuffer[pStruct->m_nDataOrDataOffset];
}

// FUNCTION: SWKOTOR 0x00410990 SYMBOL: ?GetField@CResGFF@@QAEPAUCResGFFField@@PAUCResStruct@@K@Z
CResGFFField* CResGFF::GetField(CResStruct* pStruct, unsigned long nIndex)
{
    CResGFFHeader* pHeader;
    CResGFFStruct* pGFFStruct;
    if (pStruct == 0 || (pHeader = m_pFileHeader) == 0 || m_pStructBuffer == 0
        || pStruct->m_nStructIndex >= pHeader->m_nStructCount
        || (pGFFStruct = &m_pStructBuffer[pStruct->m_nStructIndex]) == 0
        || m_pFieldBuffer == 0 || nIndex >= pGFFStruct->m_nFieldCount) {
        return 0;
    }
    if (pGFFStruct->m_nFieldCount == 1) {
        if (pGFFStruct->m_nDataOrDataOffset >= pHeader->m_nFieldCount) {
            return 0;
        }
        return &m_pFieldBuffer[pGFFStruct->m_nDataOrDataOffset];
    }
    if (m_pFieldIndicesArray == 0) {
        return 0;
    }
    if (pGFFStruct->m_nDataOrDataOffset + nIndex * 4 + 4 > pHeader->m_nFieldIndicesCount) {
        return 0;
    }
    return &m_pFieldBuffer[*(unsigned long*)(m_pFieldIndicesArray + nIndex * 4 + pGFFStruct->m_nDataOrDataOffset)];
}

// FUNCTION: SWKOTOR 0x00410a20
void* CResGFF::GetDataField(CResGFFField* pField, unsigned long& nSize)
{
    if (pField == 0 || m_pFieldDataBuffer == 0) {
        return 0;
    }
    if (pField->m_nDataOrDataOffset >= m_pFileHeader->m_nFieldDataCount) {
        return 0;
    }
    nSize = m_pFileHeader->m_nFieldDataCount - pField->m_nDataOrDataOffset;
    return m_pFieldDataBuffer + pField->m_nDataOrDataOffset;
}

// FUNCTION: SWKOTOR 0x00410a60
void* CResGFF::GetDataLayoutList(CResGFFField* pField, unsigned long& nSize)
{
    if (pField == 0 || m_pListIndicesArray == 0) {
        return 0;
    }
    if (pField->m_nDataOrDataOffset >= m_pFileHeader->m_nListIndicesCount) {
        return 0;
    }
    nSize = m_pFileHeader->m_nListIndicesCount - pField->m_nDataOrDataOffset;
    return m_pListIndicesArray + pField->m_nDataOrDataOffset;
}

// FUNCTION: SWKOTOR 0x00410d60
unsigned long CResGFF::AddStruct(unsigned long nType)
{
    if (m_pFileHeader == 0 || m_pStructBuffer == 0) {
        return 0xffffffff;
    }
    unsigned long nCount = m_pFileHeader->m_nStructCount;
    if (nCount >= m_nStructBufferSize) {
        m_nStructBufferSize = m_nStructGrowth + nCount;
        m_nStructGrowth += m_nStructGrowth;
        CResGFFStruct* pNew = new CResGFFStruct[m_nStructBufferSize];
        memcpy(pNew, m_pStructBuffer, nCount * sizeof(CResGFFStruct));
        delete[] m_pStructBuffer;
        m_pStructBuffer = pNew;
    }
    m_pFileHeader->m_nStructCount = nCount + 1;
    m_pStructBuffer[nCount].m_nType = nType;
    m_pStructBuffer[nCount].m_nDataOrDataOffset = 0xffffffff;
    m_pStructBuffer[nCount].m_nFieldCount = 0;
    return nCount;
}

// FUNCTION: SWKOTOR 0x00410e20
unsigned long CResGFF::AddLabel(char* szLabel)
{
    if (m_pFileHeader == 0 || m_pLabelBuffer == 0) {
        return 0xffffffff;
    }
    unsigned long nCount = m_pFileHeader->m_nLabelCount;
    CResGFFLabel cLabel;
    memset(&cLabel, 0, sizeof(cLabel));
    strncpy(cLabel.m_pLabel, szLabel, 16);
    for (unsigned long i = nCount; i > 0; i--) {
        if (LabelsEqual(cLabel, m_pLabelBuffer[i - 1])) {
            return i - 1;
        }
    }
    if (nCount >= m_nLabelBufferSize) {
        m_nLabelBufferSize = m_nLabelGrowth + nCount;
        m_nLabelGrowth += m_nLabelGrowth;
        CResGFFLabel* pNew = new CResGFFLabel[m_nLabelBufferSize];
        memcpy(pNew, m_pLabelBuffer, nCount * sizeof(CResGFFLabel));
        delete[] m_pLabelBuffer;
        m_pLabelBuffer = pNew;
    }
    m_pFileHeader->m_nLabelCount = nCount + 1;
    m_pLabelBuffer[nCount] = cLabel;
    return nCount;
}

// FUNCTION: SWKOTOR 0x00410f50
void* CResGFF::AddDataField(unsigned long nOffset, unsigned long nOldSize, unsigned long nNewSize)
{
    if (m_pFileHeader == 0 || m_pFieldDataBuffer == 0) {
        return 0;
    }
    unsigned long nEnd = m_pFileHeader->m_nFieldDataCount;
    if (nEnd == nOffset + nOldSize) {
        nEnd = nOffset;
    }
    unsigned long nNewEnd = nNewSize + nEnd;
    if (nNewEnd >= m_nFieldDataBufferSize) {
        m_nFieldDataBufferSize = m_nFieldDataGrowth + nEnd + nNewSize;
        m_nFieldDataGrowth += m_nFieldDataGrowth;
        unsigned char* pNew = new unsigned char[m_nFieldDataBufferSize];
        memcpy(pNew, m_pFieldDataBuffer, m_pFileHeader->m_nFieldDataCount);
        delete[] m_pFieldDataBuffer;
        m_pFieldDataBuffer = pNew;
    }
    if (nEnd != nOffset) {
        memcpy(m_pFieldDataBuffer + nEnd, m_pFieldDataBuffer + nOffset, nOldSize);
    }
    m_pFileHeader->m_nFieldDataCount = nNewEnd;
    return m_pFieldDataBuffer + nEnd;
}

// FUNCTION: SWKOTOR 0x00411020
void* CResGFF::AddDataLayoutField(unsigned long nOffset, unsigned long nOldSize, unsigned long nNewSize)
{
    if (m_pFileHeader == 0 || m_pFieldIndicesArray == 0) {
        return 0;
    }
    unsigned long nEnd = m_pFileHeader->m_nFieldIndicesCount;
    if (nEnd == nOffset + nOldSize) {
        nEnd = nOffset;
    }
    unsigned long nNewEnd = nNewSize + nEnd;
    if (nNewEnd >= m_nFieldIndicesArraySize) {
        m_nFieldIndicesArraySize = m_nFieldIndicesGrowth + nEnd + nNewSize;
        m_nFieldIndicesGrowth += m_nFieldIndicesGrowth;
        unsigned char* pNew = new unsigned char[m_nFieldIndicesArraySize];
        memcpy(pNew, m_pFieldIndicesArray, m_pFileHeader->m_nFieldIndicesCount);
        delete[] m_pFieldIndicesArray;
        m_pFieldIndicesArray = pNew;
    }
    if (nEnd != nOffset) {
        memcpy(m_pFieldIndicesArray + nEnd, m_pFieldIndicesArray + nOffset, nOldSize);
        m_nFieldIndicesWasted += nOldSize;
    }
    m_pFileHeader->m_nFieldIndicesCount = nNewEnd;
    return m_pFieldIndicesArray + nEnd;
}

// FUNCTION: SWKOTOR 0x004110f0
void* CResGFF::AddDataLayoutList(unsigned long nOffset, unsigned long nOldSize, unsigned long nNewSize)
{
    if (m_pFileHeader == 0 || m_pListIndicesArray == 0) {
        return 0;
    }
    unsigned long nEnd = m_pFileHeader->m_nListIndicesCount;
    if (nEnd == nOffset + nOldSize) {
        nEnd = nOffset;
    }
    unsigned long nNewEnd = nNewSize + nEnd;
    if (nNewEnd >= m_nListIndicesArraySize) {
        m_nListIndicesArraySize = m_nListIndicesGrowth + nEnd + nNewSize;
        m_nListIndicesGrowth += m_nListIndicesGrowth;
        unsigned char* pNew = new unsigned char[m_nListIndicesArraySize];
        memcpy(pNew, m_pListIndicesArray, m_pFileHeader->m_nListIndicesCount);
        delete[] m_pListIndicesArray;
        m_pListIndicesArray = pNew;
    }
    if (nEnd != nOffset) {
        memcpy(m_pListIndicesArray + nEnd, m_pListIndicesArray + nOffset, nOldSize);
        m_nListIndicesWasted += nOldSize;
    }
    m_pFileHeader->m_nListIndicesCount = nNewEnd;
    return m_pListIndicesArray + nEnd;
}

// FUNCTION: SWKOTOR 0x00411630
unsigned long CResGFF::GetFieldByLabel(CResStruct* pStruct, char* szLabel)
{
    if (szLabel == 0 || m_pLabelBuffer == 0) {
        return 0xffffffff;
    }
    CResGFFLabel cLabel;
    memset(&cLabel, 0, sizeof(cLabel));
    strncpy(cLabel.m_pLabel, szLabel, 16);
    CResGFFStruct* pGFFStruct = GetStruct(pStruct);
    unsigned long nCount;
    if (pGFFStruct == 0) {
        nCount = 0xffffffff;
    }
    else {
        nCount = pGFFStruct->m_nFieldCount;
    }
    for (unsigned long i = 0; i < nCount; i++) {
        CResGFFField* pField = GetField(pGFFStruct, i);
        if (pField == 0) {
            return 0xffffffff;
        }
        if (pField->m_nLabelIndex >= m_pFileHeader->m_nLabelCount) {
            return 0xffffffff;
        }
        if (LabelsEqual(cLabel, m_pLabelBuffer[pField->m_nLabelIndex])) {
            return i;
        }
    }
    return 0xffffffff;
}

// FUNCTION: SWKOTOR 0x00411730
CResGFFField* CResGFF::AddField(CResStruct* pStruct, char* szLabel, unsigned long nType)
{
    if (szLabel == 0) {
        return 0;
    }
    CResGFFStruct* pGFFStruct = GetStruct(pStruct);
    if (pGFFStruct == 0) {
        return 0;
    }
    unsigned long nField = m_pFileHeader->m_nFieldCount;
    if (nField >= m_nFieldBufferSize) {
        m_nFieldBufferSize = m_nFieldGrowth + nField;
        m_nFieldGrowth += m_nFieldGrowth;
        CResGFFField* pNew = new CResGFFField[m_nFieldBufferSize];
        memcpy(pNew, m_pFieldBuffer, nField * sizeof(CResGFFField));
        delete[] m_pFieldBuffer;
        m_pFieldBuffer = pNew;
    }
    m_pFileHeader->m_nFieldCount = nField + 1;
    unsigned long nCount = pGFFStruct->m_nFieldCount;
    if (nCount == 0) {
        pGFFStruct->m_nDataOrDataOffset = nField;
    }
    else {
        unsigned long* pIndices;
        if (nCount == 1) {
            pIndices = (unsigned long*)AddDataLayoutField(0xffffffff, 0, 8);
            if (pIndices == 0) {
                return 0;
            }
            pIndices[0] = pGFFStruct->m_nDataOrDataOffset;
        }
        else {
            pIndices = (unsigned long*)AddDataLayoutField(pGFFStruct->m_nDataOrDataOffset, nCount * 4, nCount * 4 + 4);
            if (pIndices == 0) {
                return 0;
            }
        }
        pIndices[nCount] = nField;
        pGFFStruct->m_nDataOrDataOffset = (unsigned char*)pIndices - m_pFieldIndicesArray;
    }
    pGFFStruct->m_nFieldCount = nCount + 1;
    m_pFieldBuffer[nField].m_nLabelIndex = AddLabel(szLabel);
    m_pFieldBuffer[nField].m_nType = nType;
    m_pFieldBuffer[nField].m_nDataOrDataOffset = 0xffffffff;
    return &m_pFieldBuffer[nField];
}

// FUNCTION: SWKOTOR 0x00411880
unsigned long CResGFF::GetFieldType(CResStruct* pStruct, char* szLabel, unsigned long nIndex)
{
    if (szLabel) {
        nIndex = GetFieldByLabel(pStruct, szLabel);
    }
    CResGFFField* pField = GetField(pStruct, nIndex);
    if (pField == 0) {
        return 0xffffffff;
    }
    return pField->m_nType;
}

// FUNCTION: SWKOTOR 0x004118c0
int CResGFF::GetList(CResList* pList, CResStruct* pStruct, char* szLabel)
{
    if (pList == 0 || pStruct == 0 || szLabel == 0) {
        return 0;
    }
    CResGFFField* pField = GetField(pStruct, GetFieldByLabel(pStruct, szLabel));
    if (pField == 0 || pField->m_nType != 15) {
        return 0;
    }
    unsigned long nSize;
    if (GetDataLayoutList(pField, nSize) == 0 || nSize < 4) {
        return 0;
    }
    pList->m_nStructIndex = pStruct->m_nStructIndex;
    strncpy(pList->m_pLabel, szLabel, 16);
    return 1;
}

// FUNCTION: SWKOTOR 0x00411940
unsigned long CResGFF::GetListCount(CResList* pList)
{
    if (pList == 0) {
        return 0;
    }
    CResGFFField* pField = GetField(pList, GetFieldByLabel(pList, pList->m_pLabel));
    if (pField == 0 || pField->m_nType != 15) {
        return 0;
    }
    unsigned long nSize;
    unsigned long* pData = (unsigned long*)GetDataLayoutList(pField, nSize);
    if (pData == 0 || nSize < 4) {
        return 0;
    }
    return pData[0];
}

// FUNCTION: SWKOTOR 0x00411990
int CResGFF::GetListElement(CResStruct* pOut, CResList* pList, unsigned long nIndex)
{
    if (pOut == 0 || pList == 0) {
        return 0;
    }
    CResGFFField* pField = GetField(pList, GetFieldByLabel(pList, pList->m_pLabel));
    if (pField == 0 || pField->m_nType != 15) {
        return 0;
    }
    unsigned long nSize;
    unsigned long* pData = (unsigned long*)GetDataLayoutList(pField, nSize);
    if (pData == 0 || nSize < nIndex * 4 + 8) {
        return 0;
    }
    unsigned long nElement = pData[nIndex + 1];
    if (nIndex >= pData[0]) {
        return 0;
    }
    if (nElement >= m_pFileHeader->m_nStructCount) {
        return 0;
    }
    pOut->m_nStructIndex = nElement;
    return 1;
}

// FUNCTION: SWKOTOR 0x00411a10
int CResGFF::GetStructFromStruct(CResStruct* pOut, CResStruct* pStruct, char* szLabel)
{
    if (pOut == 0 || pStruct == 0 || szLabel == 0) {
        return 0;
    }
    CResGFFField* pField = GetField(pStruct, GetFieldByLabel(pStruct, szLabel));
    if (pField == 0 || pField->m_nType != 14) {
        return 0;
    }
    pOut->m_nStructIndex = pField->m_nDataOrDataOffset;
    return 1;
}

// FUNCTION: SWKOTOR 0x00411a60
unsigned char CResGFF::ReadFieldBYTE(CResStruct* pStruct, char* szLabel, int& bSuccess, unsigned char nDefault)
{
    if (pStruct && szLabel) {
        CResGFFField* pField = GetField(pStruct, GetFieldByLabel(pStruct, szLabel));
        if (pField && pField->m_nType == 0) {
            bSuccess = 1;
            return (unsigned char)pField->m_nDataOrDataOffset;
        }
        bSuccess = 0;
        return nDefault;
    }
    bSuccess = 0;
    return nDefault;
}

// FUNCTION: SWKOTOR 0x00411ad0
char CResGFF::ReadFieldCHAR(CResStruct* pStruct, char* szLabel, int& bSuccess, char nDefault)
{
    if (pStruct && szLabel) {
        CResGFFField* pField = GetField(pStruct, GetFieldByLabel(pStruct, szLabel));
        if (pField && pField->m_nType == 1) {
            bSuccess = 1;
            return (char)pField->m_nDataOrDataOffset;
        }
        bSuccess = 0;
        return nDefault;
    }
    bSuccess = 0;
    return nDefault;
}

// FUNCTION: SWKOTOR 0x00411b40
unsigned short CResGFF::ReadFieldWORD(CResStruct* pStruct, char* szLabel, int& bSuccess, unsigned short nDefault)
{
    if (pStruct && szLabel) {
        CResGFFField* pField = GetField(pStruct, GetFieldByLabel(pStruct, szLabel));
        if (pField && pField->m_nType == 2) {
            bSuccess = 1;
            return (unsigned short)pField->m_nDataOrDataOffset;
        }
        bSuccess = 0;
        return nDefault;
    }
    bSuccess = 0;
    return nDefault;
}

// FUNCTION: SWKOTOR 0x00411bb0
short CResGFF::ReadFieldSHORT(CResStruct* pStruct, char* szLabel, int& bSuccess, short nDefault)
{
    if (pStruct && szLabel) {
        CResGFFField* pField = GetField(pStruct, GetFieldByLabel(pStruct, szLabel));
        if (pField && pField->m_nType == 3) {
            bSuccess = 1;
            return (short)pField->m_nDataOrDataOffset;
        }
        bSuccess = 0;
        return nDefault;
    }
    bSuccess = 0;
    return nDefault;
}

// FUNCTION: SWKOTOR 0x00411c20
unsigned long CResGFF::ReadFieldDWORD(CResStruct* pStruct, char* szLabel, int& bSuccess, unsigned long nDefault)
{
    if (pStruct && szLabel) {
        CResGFFField* pField = GetField(pStruct, GetFieldByLabel(pStruct, szLabel));
        if (pField && pField->m_nType == 4) {
            bSuccess = 1;
            return (unsigned long)pField->m_nDataOrDataOffset;
        }
        bSuccess = 0;
        return nDefault;
    }
    bSuccess = 0;
    return nDefault;
}

// FUNCTION: SWKOTOR 0x00411c90
int CResGFF::ReadFieldINT(CResStruct* pStruct, char* szLabel, int& bSuccess, int nDefault)
{
    if (pStruct && szLabel) {
        CResGFFField* pField = GetField(pStruct, GetFieldByLabel(pStruct, szLabel));
        if (pField && pField->m_nType == 5) {
            bSuccess = 1;
            return (int)pField->m_nDataOrDataOffset;
        }
        bSuccess = 0;
        return nDefault;
    }
    bSuccess = 0;
    return nDefault;
}

// FUNCTION: SWKOTOR 0x00411d00
float CResGFF::ReadFieldFLOAT(CResStruct* pStruct, char* szLabel, int& bSuccess, float fDefault)
{
    if (pStruct && szLabel) {
        CResGFFField* pField = GetField(pStruct, GetFieldByLabel(pStruct, szLabel));
        if (pField && pField->m_nType == 8) {
            bSuccess = 1;
            unsigned long nData = pField->m_nDataOrDataOffset;
            return *(float*)&nData;
        }
        bSuccess = 0;
        return fDefault;
    }
    bSuccess = 0;
    return fDefault;
}

// FUNCTION: SWKOTOR 0x00411d70
unsigned __int64 CResGFF::ReadFieldDWORD64(CResStruct* pStruct, char* szLabel, int& bSuccess, unsigned __int64 nDefault)
{
    if (pStruct && szLabel) {
        CResGFFField* pField = GetField(pStruct, GetFieldByLabel(pStruct, szLabel));
        if (pField && pField->m_nType == 6) {
            unsigned long nSize;
            unsigned __int64* pData = (unsigned __int64*)GetDataField(pField, nSize);
            if (pData && nSize >= 8) {
                bSuccess = 1;
                return *pData;
            }
            bSuccess = 0;
            return nDefault;
        }
        bSuccess = 0;
        return nDefault;
    }
    bSuccess = 0;
    return nDefault;
}

// FUNCTION: SWKOTOR 0x00411e10
CResRef CResGFF::ReadFieldCResRef(CResStruct* pStruct, char* szLabel, int& bSuccess, const CResRef& cDefault)
{
    if (pStruct && szLabel) {
        CResGFFField* pField = GetField(pStruct, GetFieldByLabel(pStruct, szLabel));
        if (pField && pField->m_nType == 11) {
            unsigned long nSize;
            unsigned char* pData = (unsigned char*)GetDataField(pField, nSize);
            if (pData && nSize >= 1) {
                unsigned char nLength = pData[0];
                if (nSize >= (unsigned long)nLength + 1) {
                    bSuccess = 1;
                    return CResRef((char*)pData + 1, nLength);
                }
            }
        }
    }
    bSuccess = 0;
    return cDefault;
}

// FUNCTION: SWKOTOR 0x00411ec0
CExoString CResGFF::ReadFieldCExoString(CResStruct* pStruct, char* szLabel, int& bSuccess, const CExoString& sDefault)
{
    if (pStruct && szLabel) {
        CResGFFField* pField = GetField(pStruct, GetFieldByLabel(pStruct, szLabel));
        if (pField && pField->m_nType == 10) {
            unsigned long nSize;
            unsigned long* pData = (unsigned long*)GetDataField(pField, nSize);
            if (pData && nSize >= 4) {
                unsigned long nLength = pData[0];
                if (nSize < nLength + 4) {
                    bSuccess = 0;
                    return sDefault;
                }
                bSuccess = 1;
                return CExoString((char*)(pData + 1), nLength);
            }
            bSuccess = 0;
            return sDefault;
        }
        bSuccess = 0;
        return sDefault;
    }
    bSuccess = 0;
    return sDefault;
}

// FUNCTION: SWKOTOR 0x004121b0
Quaternion CResGFF::ReadFieldQuaternion(CResStruct* pStruct, char* szLabel, int& bSuccess, const Quaternion& qDefault)
{
    if (pStruct && szLabel) {
        CResGFFField* pField = GetField(pStruct, GetFieldByLabel(pStruct, szLabel));
        if (pField && pField->m_nType == 16) {
            unsigned long nSize;
            Quaternion* pData = (Quaternion*)GetDataField(pField, nSize);
            if (pData && nSize >= sizeof(Quaternion)) {
                bSuccess = 1;
                return *pData;
            }
            bSuccess = 0;
            return qDefault;
        }
        bSuccess = 0;
        return qDefault;
    }
    bSuccess = 0;
    return qDefault;
}

// FUNCTION: SWKOTOR 0x004122a0
Vector CResGFF::ReadFieldVector(CResStruct* pStruct, char* szLabel, int& bSuccess, const Vector& vDefault)
{
    if (pStruct && szLabel) {
        CResGFFField* pField = GetField(pStruct, GetFieldByLabel(pStruct, szLabel));
        if (pField && pField->m_nType == 17) {
            unsigned long nSize;
            Vector* pData = (Vector*)GetDataField(pField, nSize);
            if (pData && nSize >= sizeof(Vector)) {
                bSuccess = 1;
                return *pData;
            }
            bSuccess = 0;
            return vDefault;
        }
        bSuccess = 0;
        return vDefault;
    }
    bSuccess = 0;
    return vDefault;
}

// FUNCTION: SWKOTOR 0x00412380
void* CResGFF::ReadFieldVOID(CResStruct* pStruct, void* pBuffer, unsigned long nBufferSize, char* szLabel, int& bSuccess, void* pDefault)
{
    if (pStruct && szLabel && pBuffer) {
        CResGFFField* pField = GetField(pStruct, GetFieldByLabel(pStruct, szLabel));
        if (pField && pField->m_nType == 13) {
            unsigned long nSize;
            unsigned long* pData = (unsigned long*)GetDataField(pField, nSize);
            if (pData && nSize >= 4) {
                unsigned long nLength = pData[0];
                if (nSize >= nLength + 4) {
                    bSuccess = 1;
                    if (nLength >= nBufferSize) {
                        nLength = nBufferSize;
                    }
                    memcpy(pBuffer, pData + 1, nLength);
                    return pBuffer;
                }
            }
            else {
                bSuccess = 0;
                return pDefault;
            }
        }
        bSuccess = 0;
        return pDefault;
    }
    bSuccess = 0;
    return pDefault;
}

// FUNCTION: SWKOTOR 0x00412450
int CResGFF::AddList(CResList* pList, CResStruct* pStruct, char* szLabel)
{
    if (pList == 0 || pStruct == 0 || szLabel == 0) {
        return 0;
    }
    if (m_bResourceLoaded) {
        InitializeForWriting();
    }
    CResGFFField* pField = AddField(pStruct, szLabel, 15);
    if (pField == 0) {
        return 0;
    }
    unsigned long* pData = (unsigned long*)AddDataLayoutList(pField->m_nDataOrDataOffset, 0, 4);
    if (pData == 0) {
        return 0;
    }
    pField->m_nDataOrDataOffset = (unsigned char*)pData - m_pListIndicesArray;
    pData[0] = 0;
    pList->m_nStructIndex = pStruct->m_nStructIndex;
    strncpy(pList->m_pLabel, szLabel, 16);
    return 1;
}

// FUNCTION: SWKOTOR 0x004124e0
int CResGFF::AddListElement(CResStruct* pOut, CResList* pList, unsigned long nType)
{
    if (pList == 0 || pOut == 0) {
        return 0;
    }
    if (m_bResourceLoaded) {
        InitializeForWriting();
    }
    unsigned long nField = GetFieldByLabel(pList, pList->m_pLabel);
    CResGFFField* pField = GetField(pList, nField);
    if (pField == 0 || pField->m_nType != 15) {
        return 0;
    }
    unsigned long nSize;
    unsigned long* pData = (unsigned long*)GetDataLayoutList(pField, nSize);
    if (pData == 0 || nSize < 4) {
        return 0;
    }
    unsigned long nCount = pData[0];
    if (nSize < nCount * 4 + 4) {
        return 0;
    }
    pData = (unsigned long*)AddDataLayoutList(pField->m_nDataOrDataOffset, nCount * 4 + 4, nCount * 4 + 8);
    if (pData == 0) {
        return 0;
    }
    pField->m_nDataOrDataOffset = (unsigned char*)pData - m_pListIndicesArray;
    pOut->m_nStructIndex = AddStruct(nType);
    if (pOut->m_nStructIndex == 0xffffffff) {
        return 0;
    }
    pData[0] = nCount + 1;
    pData[nCount + 1] = pOut->m_nStructIndex;
    return 1;
}

// FUNCTION: SWKOTOR 0x004125b0
int CResGFF::AddStructToStruct(CResStruct* pOut, CResStruct* pStruct, char* szLabel, unsigned long nType)
{
    if (pOut == 0 || pStruct == 0 || szLabel == 0) {
        return 0;
    }
    if (m_bResourceLoaded) {
        InitializeForWriting();
    }
    CResGFFField* pField = AddField(pStruct, szLabel, 14);
    if (pField == 0) {
        return 0;
    }
    unsigned long nStruct = AddStruct(nType);
    if (nStruct == 0xffffffff) {
        return 0;
    }
    pField->m_nDataOrDataOffset = nStruct;
    pOut->m_nStructIndex = nStruct;
    return 1;
}

// FUNCTION: SWKOTOR 0x00412620
int CResGFF::WriteFieldBYTE(CResStruct* pStruct, unsigned char nValue, char* szLabel)
{
    if (pStruct == 0 || szLabel == 0) {
        return 0;
    }
    if (m_bResourceLoaded) {
        InitializeForWriting();
    }
    CResGFFField* pField = AddField(pStruct, szLabel, 0);
    if (pField == 0) {
        return 0;
    }
    pField->m_nDataOrDataOffset = nValue;
    return 1;
}

// FUNCTION: SWKOTOR 0x00412670
int CResGFF::WriteFieldCHAR(CResStruct* pStruct, char nValue, char* szLabel)
{
    if (pStruct == 0 || szLabel == 0) {
        return 0;
    }
    if (m_bResourceLoaded) {
        InitializeForWriting();
    }
    CResGFFField* pField = AddField(pStruct, szLabel, 1);
    if (pField == 0) {
        return 0;
    }
    pField->m_nDataOrDataOffset = nValue;
    return 1;
}

// FUNCTION: SWKOTOR 0x004126c0
int CResGFF::WriteFieldWORD(CResStruct* pStruct, unsigned short nValue, char* szLabel)
{
    if (pStruct == 0 || szLabel == 0) {
        return 0;
    }
    if (m_bResourceLoaded) {
        InitializeForWriting();
    }
    CResGFFField* pField = AddField(pStruct, szLabel, 2);
    if (pField == 0) {
        return 0;
    }
    pField->m_nDataOrDataOffset = nValue;
    return 1;
}

// FUNCTION: SWKOTOR 0x00412710
int CResGFF::WriteFieldSHORT(CResStruct* pStruct, short nValue, char* szLabel)
{
    if (pStruct == 0 || szLabel == 0) {
        return 0;
    }
    if (m_bResourceLoaded) {
        InitializeForWriting();
    }
    CResGFFField* pField = AddField(pStruct, szLabel, 3);
    if (pField == 0) {
        return 0;
    }
    pField->m_nDataOrDataOffset = nValue;
    return 1;
}

// FUNCTION: SWKOTOR 0x00412760
int CResGFF::WriteFieldDWORD(CResStruct* pStruct, unsigned long nValue, char* szLabel)
{
    if (pStruct == 0 || szLabel == 0) {
        return 0;
    }
    if (m_bResourceLoaded) {
        InitializeForWriting();
    }
    CResGFFField* pField = AddField(pStruct, szLabel, 4);
    if (pField == 0) {
        return 0;
    }
    pField->m_nDataOrDataOffset = nValue;
    return 1;
}

// FUNCTION: SWKOTOR 0x004127b0
int CResGFF::WriteFieldINT(CResStruct* pStruct, int nValue, char* szLabel)
{
    if (pStruct == 0 || szLabel == 0) {
        return 0;
    }
    if (m_bResourceLoaded) {
        InitializeForWriting();
    }
    CResGFFField* pField = AddField(pStruct, szLabel, 5);
    if (pField == 0) {
        return 0;
    }
    pField->m_nDataOrDataOffset = nValue;
    return 1;
}

// FUNCTION: SWKOTOR 0x00412800
int CResGFF::WriteFieldDWORD64(CResStruct* pStruct, unsigned __int64 nValue, char* szLabel)
{
    if (pStruct == 0 || szLabel == 0) {
        return 0;
    }
    if (m_bResourceLoaded) {
        InitializeForWriting();
    }
    CResGFFField* pField = AddField(pStruct, szLabel, 6);
    if (pField == 0) {
        return 0;
    }
    unsigned __int64* pData = (unsigned __int64*)AddDataField(0xffffffff, 0, 8);
    if (pData == 0) {
        return 0;
    }
    pField->m_nDataOrDataOffset = (unsigned char*)pData - m_pFieldDataBuffer;
    *pData = nValue;
    return 1;
}

// FUNCTION: SWKOTOR 0x00412870
int CResGFF::WriteFieldFLOAT(CResStruct* pStruct, float fValue, char* szLabel)
{
    if (pStruct == 0 || szLabel == 0) {
        return 0;
    }
    if (m_bResourceLoaded) {
        InitializeForWriting();
    }
    CResGFFField* pField = AddField(pStruct, szLabel, 8);
    if (pField == 0) {
        return 0;
    }
    pField->m_nDataOrDataOffset = *(unsigned long*)&fValue;
    return 1;
}

// FUNCTION: SWKOTOR 0x004128c0
int CResGFF::WriteFieldCResRef(CResStruct* pStruct, const CResRef& cValue, char* szLabel)
{
    if (pStruct == 0 || szLabel == 0) {
        return 0;
    }
    if (m_bResourceLoaded) {
        InitializeForWriting();
    }
    CResGFFField* pField = AddField(pStruct, szLabel, 11);
    if (pField == 0) {
        return 0;
    }
    unsigned char nLength = cValue.GetLength();
    unsigned char* pData = (unsigned char*)AddDataField(0xffffffff, 0, nLength + 1);
    if (pData == 0) {
        return 0;
    }
    pField->m_nDataOrDataOffset = pData - m_pFieldDataBuffer;
    *pData++ = nLength;
    unsigned char* pSource = (unsigned char*)cValue.GetResRef();
    for (unsigned long i = 0; i < nLength; i++) {
        unsigned char c = pSource[i];
        if (c >= 'A' && c <= 'Z') {
            c += 'a' - 'A';
        }
        pData[i] = c;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00412970
int CResGFF::WriteFieldCExoString(CResStruct* pStruct, const CExoString& sValue, char* szLabel)
{
    if (pStruct == 0 || szLabel == 0) {
        return 0;
    }
    if (m_bResourceLoaded) {
        InitializeForWriting();
    }
    CResGFFField* pField = AddField(pStruct, szLabel, 10);
    if (pField == 0) {
        return 0;
    }
    unsigned long nLength = sValue.GetLength();
    unsigned long* pData = (unsigned long*)AddDataField(0xffffffff, 0, nLength + 4);
    if (pData == 0) {
        return 0;
    }
    pField->m_nDataOrDataOffset = (unsigned char*)pData - m_pFieldDataBuffer;
    pData[0] = nLength;
    memcpy(pData + 1, sValue.CStr(), nLength);
    return 1;
}

// FUNCTION: SWKOTOR 0x00412c10
int CResGFF::WriteFieldVOID(CResStruct* pStruct, void* pValue, unsigned long nSize, char* szLabel)
{
    if (pStruct == 0 || szLabel == 0) {
        return 0;
    }
    if (m_bResourceLoaded) {
        InitializeForWriting();
    }
    CResGFFField* pField = AddField(pStruct, szLabel, 13);
    if (pField == 0) {
        return 0;
    }
    unsigned long* pData = (unsigned long*)AddDataField(0xffffffff, 0, nSize + 4);
    if (pData == 0) {
        return 0;
    }
    pField->m_nDataOrDataOffset = (unsigned char*)pData - m_pFieldDataBuffer;
    pData[0] = nSize;
    memcpy(pData + 1, pValue, nSize);
    return 1;
}

// FUNCTION: SWKOTOR 0x00412ca0
int CResGFF::WriteFieldQuaternion(CResStruct* pStruct, const Quaternion& qValue, char* szLabel)
{
    if (pStruct == 0 || szLabel == 0) {
        return 0;
    }
    if (m_bResourceLoaded) {
        InitializeForWriting();
    }
    CResGFFField* pField = AddField(pStruct, szLabel, 16);
    if (pField == 0) {
        return 0;
    }
    Quaternion* pData = (Quaternion*)AddDataField(0xffffffff, 0, sizeof(Quaternion));
    if (pData == 0) {
        return 0;
    }
    Quaternion cValue = qValue;
    pField->m_nDataOrDataOffset = (unsigned char*)pData - m_pFieldDataBuffer;
    *pData = cValue;
    return 1;
}

// FUNCTION: SWKOTOR 0x00412d30
int CResGFF::WriteFieldVector(CResStruct* pStruct, const Vector& vValue, char* szLabel)
{
    if (pStruct == 0 || szLabel == 0) {
        return 0;
    }
    if (m_bResourceLoaded) {
        InitializeForWriting();
    }
    CResGFFField* pField = AddField(pStruct, szLabel, 17);
    if (pField == 0) {
        return 0;
    }
    Vector* pData = (Vector*)AddDataField(0xffffffff, 0, sizeof(Vector));
    if (pData == 0) {
        return 0;
    }
    Vector cValue = vValue;
    pField->m_nDataOrDataOffset = (unsigned char*)pData - m_pFieldDataBuffer;
    *pData = cValue;
    return 1;
}

// FUNCTION: SWKOTOR 0x004105a0 SYMBOL: ??0CResGFF@@QAE@XZ
CResGFF::CResGFF()
{
    m_pFileHeader = 0;
    m_pStructBuffer = 0;
    m_nStructBufferSize = 0;
    m_pFieldBuffer = 0;
    m_nFieldBufferSize = 0;
    m_pLabelBuffer = 0;
    m_nLabelBufferSize = 0;
    m_pFieldDataBuffer = 0;
    m_nFieldDataBufferSize = 0;
    m_pFieldIndicesArray = 0;
    m_nFieldIndicesArraySize = 0;
    m_nFieldIndicesWasted = 0;
    m_pListIndicesArray = 0;
    m_nListIndicesArraySize = 0;
    m_nListIndicesWasted = 0;
    m_bLoaded = 0;
    m_bValidationFailed = 0;
    m_bResourceLoaded = 0;
    m_bOwnsData = 0;
    m_nStructGrowth = 0x6e;
    m_nFieldGrowth = 0x286;
    m_nLabelGrowth = 0x62;
    m_nFieldDataGrowth = 0x72c;
    m_nFieldIndicesGrowth = 0xfd4;
    m_nListIndicesGrowth = 0xfd4;
    memset(m_pFileType, 0, 4);
}

// FUNCTION: SWKOTOR 0x00410630 SYMBOL: ??0CResGFF@@QAE@GPADABVCResRef@@@Z
CResGFF::CResGFF(unsigned short nType, char* pFileType, const CResRef& cResRef)
{
    m_pFileHeader = 0;
    m_pStructBuffer = 0;
    m_nStructBufferSize = 0;
    m_pFieldBuffer = 0;
    m_nFieldBufferSize = 0;
    m_pLabelBuffer = 0;
    m_nLabelBufferSize = 0;
    m_pFieldDataBuffer = 0;
    m_nFieldDataBufferSize = 0;
    m_pFieldIndicesArray = 0;
    m_nFieldIndicesArraySize = 0;
    m_nFieldIndicesWasted = 0;
    m_pListIndicesArray = 0;
    m_nListIndicesArraySize = 0;
    m_nListIndicesWasted = 0;
    m_nStructGrowth = 0x6e;
    m_nFieldGrowth = 0x286;
    m_nLabelGrowth = 0x62;
    m_nFieldDataGrowth = 0x72c;
    m_nFieldIndicesGrowth = 0xfd4;
    m_nListIndicesGrowth = 0xfd4;
    m_bLoaded = 0;
    m_bValidationFailed = 0;
    m_bResourceLoaded = 0;
    m_bOwnsData = 0;
    memset(m_pFileType, 0, 4);
    strncpy(m_pFileType, pFileType, 4);
    g_pExoResMan->SetResObject(cResRef, nType, this);
    Demand();
    if (m_pResource == 0) {
        g_pExoResMan->ReleaseResObject(this);
    }
    else {
        m_bOwnsData = 1;
    }
}

// FUNCTION: SWKOTOR 0x00410740
int CResGFF::OnResourceServiced()
{
    if (m_pResource == 0) {
        return 0;
    }
    if (m_pFileHeader && !m_bResourceLoaded) {
        delete[] m_pListIndicesArray;
        m_pListIndicesArray = 0;
        delete[] m_pFieldIndicesArray;
        m_pFieldIndicesArray = 0;
        delete[] m_pFieldDataBuffer;
        m_pFieldDataBuffer = 0;
        delete[] m_pLabelBuffer;
        m_pLabelBuffer = 0;
        delete[] m_pFieldBuffer;
        m_pFieldBuffer = 0;
        delete[] m_pStructBuffer;
        m_pStructBuffer = 0;
        delete m_pFileHeader;
        m_pFileHeader = 0;
    }
    m_pFileHeader = (CResGFFHeader*)m_pResource;
    if (m_pFileHeader->m_nFileType != (((((unsigned char)m_pFileType[3] << 8) | (unsigned char)m_pFileType[2]) << 8 | (unsigned char)m_pFileType[1]) << 8 | (unsigned char)m_pFileType[0])) {
        m_pFileHeader = 0;
        return 0;
    }
    if (m_pFileHeader->m_nFileVersion != MakeFourCC(g_sGFFVersion)) {
        m_pFileHeader = 0;
        return 0;
    }
    if (m_pFileHeader->m_nStructCount) {
        m_pStructBuffer = (CResGFFStruct*)((unsigned char*)m_pFileHeader + m_pFileHeader->m_nStructOffset);
    }
    if (m_pFileHeader->m_nFieldCount) {
        m_pFieldBuffer = (CResGFFField*)((unsigned char*)m_pFileHeader + m_pFileHeader->m_nFieldOffset);
    }
    if (m_pFileHeader->m_nLabelCount) {
        m_pLabelBuffer = (CResGFFLabel*)((unsigned char*)m_pFileHeader + m_pFileHeader->m_nLabelOffset);
    }
    if (m_pFileHeader->m_nFieldDataCount) {
        m_pFieldDataBuffer = (unsigned char*)m_pFileHeader + m_pFileHeader->m_nFieldDataOffset;
    }
    if (m_pFileHeader->m_nFieldIndicesCount) {
        m_pFieldIndicesArray = (unsigned char*)m_pFileHeader + m_pFileHeader->m_nFieldIndicesOffset;
    }
    if (m_pFileHeader->m_nListIndicesCount) {
        m_pListIndicesArray = (unsigned char*)m_pFileHeader + m_pFileHeader->m_nListIndicesOffset;
    }
    m_bResourceLoaded = 1;
    m_bValidationFailed = 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x004108c0
void CResGFF::ReleaseResource()
{
    if (m_bResourceLoaded) {
        if (m_bOwnsData) {
            Release();
            g_pExoResMan->ReleaseResObject(this);
            m_bOwnsData = 0;
        }
        m_bResourceLoaded = 0;
    }
    m_pFileHeader = 0;
    m_pStructBuffer = 0;
    m_nStructBufferSize = 0;
    m_pFieldBuffer = 0;
    m_nFieldBufferSize = 0;
    m_pLabelBuffer = 0;
    m_nLabelBufferSize = 0;
    m_pFieldDataBuffer = 0;
    m_nFieldDataBufferSize = 0;
    m_pFieldIndicesArray = 0;
    m_nFieldIndicesArraySize = 0;
    m_nFieldIndicesWasted = 0;
    m_pListIndicesArray = 0;
    m_nListIndicesArraySize = 0;
    m_nListIndicesWasted = 0;
}

// FUNCTION: SWKOTOR 0x00410aa0
void CResGFF::InitializeForWriting()
{
    CResGFFHeader* pHeader = new CResGFFHeader;
    pHeader->m_nStructOffset = 0;
    pHeader->m_nFieldOffset = 0;
    pHeader->m_nLabelOffset = 0;
    pHeader->m_nFieldDataOffset = 0;
    pHeader->m_nFieldIndicesOffset = 0;
    pHeader->m_nListIndicesOffset = 0;
    if (m_pFileHeader) {
        pHeader->m_nFileType = m_pFileHeader->m_nFileType;
        pHeader->m_nFileVersion = m_pFileHeader->m_nFileVersion;
        pHeader->m_nStructCount = m_pFileHeader->m_nStructCount;
        m_nStructBufferSize = m_pFileHeader->m_nStructCount + m_nStructGrowth;
        pHeader->m_nFieldCount = m_pFileHeader->m_nFieldCount;
        m_nFieldBufferSize = m_pFileHeader->m_nFieldCount + m_nFieldGrowth;
        pHeader->m_nLabelCount = m_pFileHeader->m_nLabelCount;
        m_nLabelBufferSize = m_pFileHeader->m_nLabelCount + m_nLabelGrowth;
        pHeader->m_nFieldDataCount = m_pFileHeader->m_nFieldDataCount;
        m_nFieldDataBufferSize = m_pFileHeader->m_nFieldDataCount + m_nFieldDataGrowth;
        pHeader->m_nFieldIndicesCount = m_pFileHeader->m_nFieldIndicesCount;
        m_nFieldIndicesArraySize = m_pFileHeader->m_nFieldIndicesCount + m_nFieldIndicesGrowth;
        pHeader->m_nListIndicesCount = m_pFileHeader->m_nListIndicesCount;
        m_nListIndicesArraySize = m_pFileHeader->m_nListIndicesCount + m_nListIndicesGrowth;
    }
    else {
        pHeader->m_nFileType = 0;
        pHeader->m_nFileVersion = 0;
        pHeader->m_nStructCount = 0;
        m_nStructBufferSize = 0x6e;
        pHeader->m_nFieldCount = 0;
        m_nFieldBufferSize = 0x286;
        pHeader->m_nLabelCount = 0;
        m_nLabelBufferSize = 0x62;
        pHeader->m_nFieldDataCount = 0;
        m_nFieldDataBufferSize = 0x72c;
        pHeader->m_nFieldIndicesCount = 0;
        m_nFieldIndicesArraySize = 0xfd4;
        pHeader->m_nListIndicesCount = 0;
        m_nListIndicesArraySize = 0xfd4;
    }
    CResGFFStruct* pStructs = new CResGFFStruct[m_nStructBufferSize];
    if (m_pStructBuffer) {
        memcpy(pStructs, m_pStructBuffer, m_pFileHeader->m_nStructCount * sizeof(CResGFFStruct));
    }
    CResGFFField* pFields = new CResGFFField[m_nFieldBufferSize];
    if (m_pFieldBuffer) {
        memcpy(pFields, m_pFieldBuffer, m_pFileHeader->m_nFieldCount * sizeof(CResGFFField));
    }
    CResGFFLabel* pLabels = new CResGFFLabel[m_nLabelBufferSize];
    if (m_pLabelBuffer) {
        memcpy(pLabels, m_pLabelBuffer, m_pFileHeader->m_nLabelCount * sizeof(CResGFFLabel));
    }
    unsigned char* pFieldData = new unsigned char[m_nFieldDataBufferSize];
    if (m_pFieldDataBuffer) {
        memcpy(pFieldData, m_pFieldDataBuffer, m_pFileHeader->m_nFieldDataCount);
    }
    unsigned char* pFieldIndices = new unsigned char[m_nFieldIndicesArraySize];
    if (m_pFieldIndicesArray) {
        memcpy(pFieldIndices, m_pFieldIndicesArray, m_pFileHeader->m_nFieldIndicesCount);
    }
    unsigned char* pListIndices = new unsigned char[m_nListIndicesArraySize];
    if (m_pListIndicesArray) {
        memcpy(pListIndices, m_pListIndicesArray, m_pFileHeader->m_nListIndicesCount);
    }
    if (m_bResourceLoaded) {
        ReleaseResource();
    }
    else {
        delete[] m_pListIndicesArray;
        delete[] m_pFieldIndicesArray;
        delete[] m_pFieldDataBuffer;
        delete[] m_pLabelBuffer;
        delete[] m_pFieldBuffer;
        delete[] m_pStructBuffer;
        delete m_pFileHeader;
    }
    m_pListIndicesArray = pListIndices;
    m_pFieldIndicesArray = pFieldIndices;
    m_pFieldDataBuffer = pFieldData;
    m_pFileHeader = pHeader;
    m_pLabelBuffer = pLabels;
    m_pFieldBuffer = pFields;
    m_pStructBuffer = pStructs;
}

// FUNCTION: SWKOTOR 0x00411260
int CResGFF::CreateGFFFile(CResStruct* pStruct, const CExoString& sFileType, const CExoString& sVersion)
{
    CExoString sGFFVersion(g_sGFFVersion);
    if (pStruct && (unsigned long)sFileType.GetLength() >= 4 && (unsigned long)sGFFVersion.GetLength() >= 4) {
        if (m_bResourceLoaded) {
            ReleaseResource();
        }
        InitializeForWriting();
        m_pFileHeader->m_nFileType = (((((unsigned char)sFileType[3] << 8) | (unsigned char)sFileType[2]) << 8
                                       | (unsigned char)sFileType[1]) << 8) | (unsigned char)sFileType[0];
        m_pFileHeader->m_nFileVersion = (((((unsigned char)sGFFVersion[3] << 8) | (unsigned char)sGFFVersion[2]) << 8
                                          | (unsigned char)sGFFVersion[1]) << 8) | (unsigned char)sGFFVersion[0];
        pStruct->m_nStructIndex = AddStruct(0xffffffff);
        if (pStruct->m_nStructIndex == 0xffffffff) {
            return 0;
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004113d0
int CResGFF::WriteGFFData(CExoFile& file, unsigned long& nSize)
{
    if (m_pFileHeader == 0) {
        return 0;
    }
    nSize = sizeof(CResGFFHeader);
    m_pFileHeader->m_nStructOffset = sizeof(CResGFFHeader);
    nSize += m_pFileHeader->m_nStructCount * sizeof(CResGFFStruct);
    m_pFileHeader->m_nFieldOffset = nSize;
    nSize += m_pFileHeader->m_nFieldCount * sizeof(CResGFFField);
    m_pFileHeader->m_nLabelOffset = nSize;
    nSize += m_pFileHeader->m_nLabelCount * sizeof(CResGFFLabel);
    m_pFileHeader->m_nFieldDataOffset = nSize;
    nSize += m_pFileHeader->m_nFieldDataCount;
    m_pFileHeader->m_nFieldIndicesOffset = nSize;
    nSize += m_pFileHeader->m_nFieldIndicesCount;
    m_pFileHeader->m_nListIndicesOffset = nSize;
    nSize += m_pFileHeader->m_nListIndicesCount;
    if (file.Write(m_pFileHeader, sizeof(CResGFFHeader), 1) != sizeof(CResGFFHeader)) {
        return 0;
    }
    unsigned long nBytes = m_pFileHeader->m_nStructCount * sizeof(CResGFFStruct);
    if (file.Write(m_pStructBuffer, nBytes, 1) != nBytes) {
        return 0;
    }
    nBytes = m_pFileHeader->m_nFieldCount * sizeof(CResGFFField);
    if (file.Write(m_pFieldBuffer, nBytes, 1) != nBytes) {
        return 0;
    }
    nBytes = m_pFileHeader->m_nLabelCount * sizeof(CResGFFLabel);
    if (file.Write(m_pLabelBuffer, nBytes, 1) != nBytes) {
        return 0;
    }
    nBytes = m_pFileHeader->m_nFieldDataCount;
    if (file.Write(m_pFieldDataBuffer, nBytes, 1) != nBytes) {
        return 0;
    }
    nBytes = m_pFileHeader->m_nFieldIndicesCount;
    if (file.Write(m_pFieldIndicesArray, nBytes, 1) != nBytes) {
        return 0;
    }
    nBytes = m_pFileHeader->m_nListIndicesCount;
    return file.Write(m_pListIndicesArray, nBytes, 1) == nBytes;
}

// FUNCTION: SWKOTOR 0x00411540
unsigned long CResGFF::GetTotalSize()
{
    if (m_pFileHeader == 0) {
        return 0;
    }
    return sizeof(CResGFFHeader) + m_pFileHeader->m_nStructCount * sizeof(CResGFFStruct)
           + m_pFileHeader->m_nFieldCount * sizeof(CResGFFField) + m_pFileHeader->m_nLabelCount * sizeof(CResGFFLabel)
           + m_pFileHeader->m_nFieldDataCount + m_pFileHeader->m_nFieldIndicesCount
           + m_pFileHeader->m_nListIndicesCount;
}

// FUNCTION: SWKOTOR 0x00413010 SYMBOL: ??_GCResGFF@@UAEPAXI@Z
// FUNCTION: SWKOTOR 0x00411570
CResGFF::~CResGFF()
{
    if (!m_bResourceLoaded) {
        delete[] m_pListIndicesArray;
        m_pListIndicesArray = 0;
        delete[] m_pFieldIndicesArray;
        m_pFieldIndicesArray = 0;
        delete[] m_pFieldDataBuffer;
        m_pFieldDataBuffer = 0;
        delete[] m_pLabelBuffer;
        m_pLabelBuffer = 0;
        delete[] m_pFieldBuffer;
        m_pFieldBuffer = 0;
        delete[] m_pStructBuffer;
        m_pStructBuffer = 0;
        delete m_pFileHeader;
        m_pFileHeader = 0;
    }
    else {
        ReleaseResource();
    }
}

// FUNCTION: SWKOTOR 0x00411fd0
// NONMATCH: MSVC cross-jumps the "push; mov ecx, esi; call copy ctor" suffix
// of the four default-return tails; the original keeps each tail whole.
CExoLocString CResGFF::ReadFieldCExoLocString(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                              const CExoLocString& lsDefault)
{
    CExoLocString lsValue;
    if (pStruct && szLabel) {
        unsigned long nField = GetFieldByLabel(pStruct, szLabel);
        CResGFFField* pField = GetField(pStruct, nField);
        if (pField && pField->m_nType == 12) {
            unsigned long nSize;
            unsigned char* pData = (unsigned char*)GetDataField(pField, nSize);
            if (pData && nSize >= 4 && nSize >= *(unsigned long*)pData + 4) {
                unsigned long nRemaining = *(unsigned long*)pData;
                pData += 4;
                lsValue.m_nStrRef = *(unsigned long*)pData;
                pData += 4;
                unsigned long nCount = *(unsigned long*)pData;
                pData += 4;
                nRemaining -= 8;
                for (unsigned long i = 0; i < nCount; i++) {
                    int nID = *(int*)pData;
                    pData += 4;
                    unsigned long nLength = *(unsigned long*)pData;
                    pData += 4;
                    nRemaining -= 8;
                    lsValue.AddString(nID / 2, CExoString((char*)pData, nLength), nID % 2);
                    pData += nLength;
                    nRemaining -= nLength;
                }
                if (nRemaining != 0) {
                    bSuccess = 0;
                    return lsDefault;
                }
                bSuccess = 1;
                return lsValue;
            }
            bSuccess = 0;
            return lsDefault;
        }
        bSuccess = 0;
        return lsDefault;
    }
    bSuccess = 0;
    return lsDefault;
}

// FUNCTION: SWKOTOR 0x00412a10
int CResGFF::WriteFieldCExoLocString(CResStruct* pStruct, CExoLocString& lsValue, char* szLabel)
{
    int bResult = 1;
    CExoString sString;
    int nLanguage;
    unsigned char nGender;
    if (pStruct == 0 || szLabel == 0) {
        return 0;
    }
    if (m_bResourceLoaded) {
        InitializeForWriting();
    }
    CResGFFField* pField = AddField(pStruct, szLabel, 12);
    if (pField == 0) {
        return 0;
    }
    unsigned long nSize = 8;
    unsigned long i;
    for (i = 0; i < lsValue.GetStringCount(); i++) {
        nSize += lsValue.GetStringLength(i) + 8;
    }
    unsigned long* pData = (unsigned long*)AddDataField(0xffffffff, 0, nSize + 4);
    if (pData == 0) {
        return 0;
    }
    pField->m_nDataOrDataOffset = (unsigned char*)pData - m_pFieldDataBuffer;
    *pData = nSize;
    pData++;
    *pData = lsValue.m_nStrRef;
    pData++;
    *pData = lsValue.GetStringCount();
    pData++;
    for (i = 0; i < lsValue.GetStringCount(); i++) {
        unsigned long nLength;
        if (!lsValue.GetString(i, &nLanguage, &sString, &nGender)) {
            bResult = 0;
            nLength = 0;
            nLanguage = -1;
            nGender = 0;
        }
        else {
            nLength = sString.GetLength();
        }
        *pData = nGender + nLanguage * 2;
        pData++;
        *pData = nLength;
        pData++;
        memcpy(pData, sString.CStr(), nLength);
        pData = (unsigned long*)((unsigned char*)pData + nLength);
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x00412db0
// NONMATCH: register allocation; the original keeps the header in edi and
// reuses nOut (ebp) as its zero, we spill the header and keep 0 in esi.
void CResGFF::Pack(unsigned char nPercent, unsigned long nMinBytes)
{
    unsigned long nFieldWasted = m_nFieldIndicesWasted;
    if (nFieldWasted == 0 && m_nListIndicesWasted == 0) {
        return;
    }
    CResGFFHeader* pHeader = m_pFileHeader;
    if (pHeader == 0 || m_pFieldIndicesArray == 0 || m_pListIndicesArray == 0 || m_pStructBuffer == 0
        || m_pFieldBuffer == 0) {
        return;
    }
    unsigned long nWasted = nFieldWasted + m_nListIndicesWasted;
    if (nWasted <= nMinBytes) {
        return;
    }
    if (nWasted * 100 / GetTotalSize() <= nPercent) {
        return;
    }
    unsigned long nOut = 0;
    if (nFieldWasted > 0) {
        unsigned long nBytes = pHeader->m_nFieldIndicesCount - nFieldWasted;
        if (nBytes) {
            unsigned char* pNew = new unsigned char[nBytes];
            for (unsigned long i = 0; i < m_pFileHeader->m_nStructCount; i++) {
                CResGFFStruct* pS = &m_pStructBuffer[i];
                if (pS->m_nFieldCount > 1) {
                    unsigned long nOffset = pS->m_nDataOrDataOffset;
                    unsigned long nLimit = m_pFileHeader->m_nFieldIndicesCount;
                    if (nOffset < nLimit && nLimit - nOffset >= pS->m_nFieldCount * 4) {
                        memcpy(pNew + nOut, m_pFieldIndicesArray + nOffset, pS->m_nFieldCount * 4);
                        m_pStructBuffer[i].m_nDataOrDataOffset = nOut;
                        nOut += m_pStructBuffer[i].m_nFieldCount * 4;
                    }
                    else {
                        pS->m_nDataOrDataOffset = 0xffffffff;
                        m_pStructBuffer[i].m_nFieldCount = 0;
                    }
                }
            }
            delete[] m_pFieldIndicesArray;
            m_pFieldIndicesArray = pNew;
            m_pFileHeader->m_nFieldIndicesCount = nOut;
            nOut = 0;
        }
        else {
            pHeader->m_nFieldIndicesCount = 0;
        }
        m_nFieldIndicesWasted = 0;
    }
    if (m_nListIndicesWasted > 0) {
        unsigned long nBytes = m_pFileHeader->m_nListIndicesCount - m_nListIndicesWasted;
        if (nBytes) {
            unsigned char* pNew = new unsigned char[nBytes];
            for (unsigned long i = 0; i < m_pFileHeader->m_nFieldCount; i++) {
                CResGFFField* pF = &m_pFieldBuffer[i];
                if (pF->m_nType == 15) {
                    unsigned long nOffset = pF->m_nDataOrDataOffset;
                    unsigned long nLimit = m_pFileHeader->m_nListIndicesCount;
                    if (nOffset < nLimit && nLimit - nOffset >= 4) {
                        unsigned long* pSrc = (unsigned long*)(m_pListIndicesArray + nOffset);
                        unsigned long n = *pSrc * 4;
                        if (nLimit - nOffset >= n + 4) {
                            memcpy(pNew + nOut, pSrc, n + 4);
                            m_pFieldBuffer[i].m_nDataOrDataOffset = nOut;
                            nOut = n + nOut + 4;
                        }
                        else {
                            pF->m_nDataOrDataOffset = 0xffffffff;
                        }
                    }
                    else {
                        pF->m_nDataOrDataOffset = 0xffffffff;
                    }
                }
            }
            delete[] m_pListIndicesArray;
            m_pListIndicesArray = pNew;
            m_pFileHeader->m_nListIndicesCount = nOut;
            m_nListIndicesWasted = 0;
        }
        else {
            m_pFileHeader->m_nListIndicesCount = 0;
            m_nListIndicesWasted = 0;
        }
    }
}

// FUNCTION: SWKOTOR 0x00413030
int CResGFF::WriteGFFFile(const CExoString& sFileName, unsigned short nResType)
{
    CExoFile file(sFileName, nResType, "wb");
    if (!file.IsOpen()) {
        return 0;
    }
    Pack(0, 0);
    unsigned long nSize;
    return WriteGFFData(file, nSize);
}
