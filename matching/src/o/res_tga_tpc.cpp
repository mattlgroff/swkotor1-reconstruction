// CResLTR (ctor/dtor), CResTGA and CResTPC, 0x007121B0-0x00713170.
#include "o/aurora_res.h"

void operator delete(void* p);
extern "C" void* __cdecl memcpy(void* d, const void* s, unsigned int n);
#pragma intrinsic(memcpy)

// ---------------------------------------------------------------- CResLTR

// FUNCTION: SWKOTOR 0x007121B0
CResLTR::CResLTR()
{
    m_pHeader = 0;
    m_pLTR = 0;
    m_nLTRSize = 0;
    m_bLoaded = 0;
    m_pTripleStart = 0;
    m_pTripleMiddle = 0;
    m_pTripleEnd = 0;
    m_pDoubleStart = 0;
    m_pDoubleMiddle = 0;
    m_pDoubleEnd = 0;
    m_pSingleStart = 0;
    m_pSingleMiddle = 0;
    m_pSingleEnd = 0;
}

// FUNCTION: SWKOTOR 0x007121F0
CResLTR::~CResLTR()
{
}

// FUNCTION: SWKOTOR 0x00712200
int CResLTR::OnResourceFreed()
{
    m_bLoaded = 0;
    if (m_pSingleStart) {
        delete m_pSingleStart;
        m_pSingleStart = 0;
    }
    if (m_pSingleMiddle) {
        delete m_pSingleMiddle;
        m_pSingleMiddle = 0;
    }
    if (m_pSingleEnd) {
        delete m_pSingleEnd;
        m_pSingleEnd = 0;
    }
    unsigned char i;
    for (i = 0; i < m_pHeader->m_nLetters; i++) {
        if (m_pDoubleStart && m_pDoubleStart[i]) {
            delete m_pDoubleStart[i];
        }
        if (m_pDoubleMiddle && m_pDoubleMiddle[i]) {
            delete m_pDoubleMiddle[i];
        }
        if (m_pDoubleEnd && m_pDoubleEnd[i]) {
            delete m_pDoubleEnd[i];
        }
    }
    if (m_pDoubleStart) {
        delete m_pDoubleStart;
        m_pDoubleStart = 0;
    }
    if (m_pDoubleMiddle) {
        delete m_pDoubleMiddle;
        m_pDoubleMiddle = 0;
    }
    if (m_pDoubleEnd) {
        delete m_pDoubleEnd;
        m_pDoubleEnd = 0;
    }
    for (i = 0; i < m_pHeader->m_nLetters; i++) {
        for (unsigned char j = 0; j < m_pHeader->m_nLetters; j++) {
            if (m_pTripleStart[i][j]) {
                delete m_pTripleStart[i][j];
            }
            if (m_pTripleMiddle[i][j]) {
                delete m_pTripleMiddle[i][j];
            }
            if (m_pTripleEnd[i][j]) {
                delete m_pTripleEnd[i][j];
            }
        }
        if (m_pTripleStart[i]) {
            delete m_pTripleStart[i];
        }
        if (m_pTripleMiddle[i]) {
            delete m_pTripleMiddle[i];
        }
        if (m_pTripleEnd[i]) {
            delete m_pTripleEnd[i];
        }
    }
    if (m_pTripleStart) {
        delete m_pTripleStart;
        m_pTripleStart = 0;
    }
    if (m_pTripleMiddle) {
        delete m_pTripleMiddle;
        m_pTripleMiddle = 0;
    }
    if (m_pTripleEnd) {
        delete m_pTripleEnd;
        m_pTripleEnd = 0;
    }
    m_pHeader = 0;
    m_pLTR = 0;
    m_nLTRSize = 0;
    return 1;
}

// The tables follow the 9-byte header in order: single start / middle /
// end, then per letter the double ones, then per letter pair the triples.
// FUNCTION: SWKOTOR 0x00712410
int CResLTR::OnResourceServiced()
{
    if (!m_pResource) {
        return 0;
    }
    if (m_bLoaded) {
        return 1;
    }
    m_nLTRSize = m_nSize - 9;
    m_pHeader = (LTRHeader*)m_pResource;
    if (m_pHeader->m_nFileType != 0x2052544c) {
        OnResourceFreed();
        return 0;
    }
    if (m_pHeader->m_nVersion != 0x302e3156) {
        OnResourceFreed();
        return 0;
    }
    m_pSingleStart = new float[m_pHeader->m_nLetters];
    m_pSingleMiddle = new float[m_pHeader->m_nLetters];
    m_pSingleEnd = new float[m_pHeader->m_nLetters];
    if (!m_pSingleStart || !m_pSingleMiddle || !m_pSingleEnd) {
        return 0;
    }
    m_pDoubleStart = new float*[m_pHeader->m_nLetters];
    m_pDoubleMiddle = new float*[m_pHeader->m_nLetters];
    m_pDoubleEnd = new float*[m_pHeader->m_nLetters];
    if (!m_pDoubleStart || !m_pDoubleMiddle || !m_pDoubleEnd) {
        return 0;
    }
    unsigned char i;
    unsigned char j;
    for (i = 0; i < m_pHeader->m_nLetters; i++) {
        m_pDoubleStart[i] = new float[m_pHeader->m_nLetters];
        m_pDoubleMiddle[i] = new float[m_pHeader->m_nLetters];
        m_pDoubleEnd[i] = new float[m_pHeader->m_nLetters];
        if (!m_pDoubleStart[i] || !m_pDoubleMiddle[i] || !m_pDoubleEnd[i]) {
            return 0;
        }
    }
    m_pTripleStart = new float**[m_pHeader->m_nLetters];
    m_pTripleMiddle = new float**[m_pHeader->m_nLetters];
    m_pTripleEnd = new float**[m_pHeader->m_nLetters];
    if (!m_pTripleStart || !m_pTripleMiddle || !m_pTripleEnd) {
        return 0;
    }
    for (i = 0; i < m_pHeader->m_nLetters; i++) {
        m_pTripleStart[i] = new float*[m_pHeader->m_nLetters];
        m_pTripleMiddle[i] = new float*[m_pHeader->m_nLetters];
        m_pTripleEnd[i] = new float*[m_pHeader->m_nLetters];
        if (!m_pTripleStart[i] || !m_pTripleMiddle[i] || !m_pTripleEnd[i]) {
            return 0;
        }
        for (j = 0; j < m_pHeader->m_nLetters; j++) {
            m_pTripleStart[i][j] = new float[m_pHeader->m_nLetters];
            m_pTripleMiddle[i][j] = new float[m_pHeader->m_nLetters];
            m_pTripleEnd[i][j] = new float[m_pHeader->m_nLetters];
            if (!m_pTripleStart[i][j] || !m_pTripleMiddle[i][j] || !m_pTripleEnd[i][j]) {
                return 0;
            }
        }
    }
    unsigned char* pData = (unsigned char*)m_pResource + 9;
    m_pLTR = pData;
    unsigned long nOffset = 0;
    memcpy(m_pSingleStart, pData + nOffset, m_pHeader->m_nLetters * sizeof(float));
    nOffset += m_pHeader->m_nLetters * sizeof(float);
    memcpy(m_pSingleMiddle, pData + nOffset, m_pHeader->m_nLetters * sizeof(float));
    nOffset += m_pHeader->m_nLetters * sizeof(float);
    memcpy(m_pSingleEnd, pData + nOffset, m_pHeader->m_nLetters * sizeof(float));
    nOffset += m_pHeader->m_nLetters * sizeof(float);
    for (i = 0; i < m_pHeader->m_nLetters; i++) {
        memcpy(m_pDoubleStart[i], pData + nOffset, m_pHeader->m_nLetters * sizeof(float));
        nOffset += m_pHeader->m_nLetters * sizeof(float);
        memcpy(m_pDoubleMiddle[i], pData + nOffset, m_pHeader->m_nLetters * sizeof(float));
        nOffset += m_pHeader->m_nLetters * sizeof(float);
        memcpy(m_pDoubleEnd[i], pData + nOffset, m_pHeader->m_nLetters * sizeof(float));
        nOffset += m_pHeader->m_nLetters * sizeof(float);
    }
    for (i = 0; i < m_pHeader->m_nLetters; i++) {
        for (j = 0; j < m_pHeader->m_nLetters; j++) {
            memcpy(m_pTripleStart[i][j], pData + nOffset, m_pHeader->m_nLetters * sizeof(float));
            nOffset += m_pHeader->m_nLetters * sizeof(float);
            memcpy(m_pTripleMiddle[i][j], pData + nOffset, m_pHeader->m_nLetters * sizeof(float));
            nOffset += m_pHeader->m_nLetters * sizeof(float);
            memcpy(m_pTripleEnd[i][j], pData + nOffset, m_pHeader->m_nLetters * sizeof(float));
            nOffset += m_pHeader->m_nLetters * sizeof(float);
        }
    }
    if (nOffset > (unsigned long)m_nLTRSize) {
        return 0;
    }
    m_bLoaded = 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x00712980 SYMBOL: ??_GCResLTR@@UAEPAXI@Z

// ---------------------------------------------------------------- CResTGA

// FUNCTION: SWKOTOR 0x007129A0
CResTGA::CResTGA()
{
    m_bAllocated = 0;
    m_bColorMapped = 0;
    m_bRLE = 0;
    m_bLoaded = 0;
    m_nImageSize = 0;
    m_nRLESize = 0;
    m_pColorMap = 0;
    m_pImage = 0;
    m_pHeader = 0;
    m_bLargePrefix = 1;
}

// FUNCTION: SWKOTOR 0x007129E0
CResTGA::~CResTGA()
{
}

// FUNCTION: SWKOTOR 0x007129F0
int CResTGA::GetTGAAttrib(unsigned long* pWidth, unsigned long* pHeight, unsigned char* pBitsPerPixel)
{
    if (!m_pHeader) {
        return 0;
    }
    *pWidth = m_pHeader->m_nWidth;
    *pHeight = m_pHeader->m_nHeight;
    *pBitsPerPixel = m_pHeader->m_nBitsPerPixel;
    return 1;
}

// FUNCTION: SWKOTOR 0x00712A30
int CResTGA::OnResourceFreed()
{
    if (m_bAllocated && m_pImage) {
        delete m_pImage;
    }
    m_bAllocated = 0;
    m_bColorMapped = 0;
    m_bRLE = 0;
    m_bLoaded = 0;
    m_nImageSize = 0;
    m_nRLESize = 0;
    m_pColorMap = 0;
    m_pImage = 0;
    m_pHeader = 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x00712A70
int CResTGA::ReadColorMappedRLETGA()
{
    unsigned char* pRLE = m_pImage;
    m_pImage = new unsigned char[m_nImageSize + 10];
    if (!m_pImage) {
        return 0;
    }
    unsigned long nIn = 0;
    unsigned long nOut = 0;
    while (nOut < m_nImageSize) {
        unsigned char nCode = pRLE[nIn++];
        unsigned long nEnd = (nCode & 0x7f) + nOut + 1;
        if (nCode & 0x80) {
            unsigned char nValue = pRLE[nIn++];
            while (nOut < nEnd) {
                m_pImage[nOut++] = nValue;
            }
        } else {
            while (nOut < nEnd) {
                m_pImage[nOut++] = pRLE[nIn++];
            }
        }
    }
    m_bAllocated = 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x00712B00
int CResTGA::ReadUnmappedRLETGA()
{
    unsigned char* pRLE = m_pImage;
    m_pImage = new unsigned char[m_nImageSize + 10];
    if (!m_pImage) {
        return 0;
    }
    unsigned long nIn = 0;
    unsigned long nOut = 0;
    if (m_pHeader->m_nBitsPerPixel == 24) {
        while (nOut < m_nImageSize) {
            unsigned char nCode = pRLE[nIn++];
            unsigned long nEnd = nOut + ((nCode & 0x7f) + 1) * 3;
            if (nCode & 0x80) {
                while (nOut < nEnd) {
                    m_pImage[nOut++] = pRLE[nIn];
                    m_pImage[nOut++] = pRLE[nIn + 1];
                    m_pImage[nOut++] = pRLE[nIn + 2];
                }
                nIn += 3;
            } else {
                while (nOut < nEnd) {
                    m_pImage[nOut++] = pRLE[nIn++];
                    m_pImage[nOut++] = pRLE[nIn++];
                    m_pImage[nOut++] = pRLE[nIn++];
                }
            }
        }
    } else if (m_pHeader->m_nBitsPerPixel == 32) {
        while (nOut < m_nImageSize) {
            unsigned char nCode = pRLE[nIn++];
            unsigned long nEnd = nOut + ((nCode & 0x7f) + 1) * 4;
            if (nCode & 0x80) {
                while (nOut < nEnd) {
                    m_pImage[nOut++] = pRLE[nIn];
                    m_pImage[nOut++] = pRLE[nIn + 1];
                    m_pImage[nOut++] = pRLE[nIn + 2];
                    m_pImage[nOut++] = pRLE[nIn + 3];
                }
                nIn += 4;
            } else {
                while (nOut < nEnd) {
                    m_pImage[nOut++] = pRLE[nIn++];
                    m_pImage[nOut++] = pRLE[nIn++];
                    m_pImage[nOut++] = pRLE[nIn++];
                    m_pImage[nOut++] = pRLE[nIn++];
                }
            }
        }
    } else {
        return 0;
    }
    m_bAllocated = 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x00712C90 SYMBOL: ??_GCResTGA@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00712CB0
int CResTGA::OnResourceServiced()
{
    if (!m_pResource) {
        return 0;
    }
    if (m_bLoaded) {
        return 1;
    }
    if (m_pHeader) {
        return 0;
    }
    if (m_pImage) {
        return 0;
    }
    if (m_pColorMap) {
        return 0;
    }
    m_pHeader = (TGAHeader*)m_pResource;
    m_pColorMap = (unsigned char*)m_pResource + m_pHeader->m_nIDLength + sizeof(TGAHeader);
    unsigned short nColorMapLength = m_pHeader->m_nColorMapLength;
    int bRead = 1;
    switch (m_pHeader->m_nImageType) {
    case 1:
        m_nImageSize = m_nSize - (m_pHeader->m_nColorMapEntrySize >> 3) * nColorMapLength
                       - m_pHeader->m_nIDLength - sizeof(TGAHeader);
        m_pImage = m_pColorMap + (m_pHeader->m_nColorMapEntrySize >> 3) * nColorMapLength;
        m_bColorMapped = 1;
        break;
    case 2:
    case 3:
        m_nImageSize = m_nSize - (m_pHeader->m_nColorMapEntrySize >> 3) * nColorMapLength
                       - m_pHeader->m_nIDLength - sizeof(TGAHeader);
        m_pImage = m_pColorMap + (m_pHeader->m_nColorMapEntrySize >> 3) * nColorMapLength;
        break;
    case 9:
        m_nRLESize = m_nSize - (m_pHeader->m_nColorMapEntrySize >> 3) * nColorMapLength
                     - m_pHeader->m_nIDLength - sizeof(TGAHeader);
        m_nImageSize = (m_pHeader->m_nBitsPerPixel >> 3) * m_pHeader->m_nHeight * m_pHeader->m_nWidth;
        m_pImage = m_pColorMap + (m_pHeader->m_nColorMapEntrySize >> 3) * nColorMapLength;
        m_bColorMapped = 1;
        m_bRLE = 1;
        bRead = ReadColorMappedRLETGA();
        break;
    case 10:
        m_nRLESize = m_nSize - (m_pHeader->m_nColorMapEntrySize >> 3) * nColorMapLength
                     - m_pHeader->m_nIDLength - sizeof(TGAHeader);
        m_nImageSize = (m_pHeader->m_nBitsPerPixel >> 3) * m_pHeader->m_nHeight * m_pHeader->m_nWidth;
        m_pImage = m_pColorMap + (m_pHeader->m_nColorMapEntrySize >> 3) * nColorMapLength;
        m_bRLE = 1;
        bRead = ReadUnmappedRLETGA();
        break;
    default:
        m_pColorMap = 0;
        m_pHeader = 0;
        return 0;
    }
    if (!bRead) {
        OnResourceFreed();
        return 0;
    }
    if (m_pHeader->m_nImageType != 3) {
        unsigned long nBytesPerPixel = m_pHeader->m_nBitsPerPixel >> 3;
        unsigned long nPixels = m_pHeader->m_nHeight * m_pHeader->m_nWidth;
        unsigned char* pPixel = m_pImage;
        for (unsigned long i = 0; i < nPixels; i++) {
            unsigned char nTemp = pPixel[0];
            pPixel[0] = pPixel[2];
            pPixel[2] = nTemp;
            pPixel += nBytesPerPixel;
        }
    }
    m_bLoaded = 1;
    return 1;
}

// ---------------------------------------------------------------- CResTPC

// FUNCTION: SWKOTOR 0x00712EA0
CResTPC::CResTPC()
{
    m_bCompressed = 0;
    m_bLoaded = 0;
    m_nDataSize = 0;
    m_nCompressedSize = 0;
    m_nTXISize = 0;
    m_pTPC = 0;
    m_pHeader = 0;
    m_pTXI = 0;
    m_bLargePrefix = 1;
}

// FUNCTION: SWKOTOR 0x00712EE0
CResTPC::~CResTPC()
{
}

// FUNCTION: SWKOTOR 0x00712EF0
int CResTPC::GetTPCAttrib(int* pWidth, int* pHeight, char* pFormat, int* pCompressedSize, int* pDataSize,
                          int* pTXISize, float* pAlphaTest, char* pMipMaps)
{
    if (!m_pHeader) {
        return 0;
    }
    *pWidth = m_pHeader->m_nWidth;
    *pHeight = m_pHeader->m_nHeight;
    *pAlphaTest = m_pHeader->m_fAlphaTest;
    *pMipMaps = m_pHeader->m_nMipMaps;
    *pCompressedSize = m_nCompressedSize;
    *pDataSize = m_nDataSize;
    *pTXISize = m_nTXISize;
    if (m_pHeader->m_nEncoding & 1) {
        *pFormat = 1;
    } else if (m_pHeader->m_nEncoding & 2) {
        *pFormat = 3;
    } else if (m_pHeader->m_nEncoding & 4) {
        *pFormat = 4;
    }
    return 1;
}

// Also CResPLT::GetPLTDataPtr.
// FUNCTION: SWKOTOR 0x00712F90
unsigned char* CResTPC::GetTPCDataPtr()
{
    return m_pTPC;
}

// Also CResTGA::GetTGADataPtr.
// FUNCTION: SWKOTOR 0x00712FA0
unsigned char* CResTPC::GetTXIDataPtr()
{
    return m_pTXI;
}

// FUNCTION: SWKOTOR 0x00712FB0
int CResTPC::GetTPCDataSize()
{
    if (m_bCompressed) {
        return m_nCompressedSize;
    }
    return m_nDataSize;
}

// FUNCTION: SWKOTOR 0x00712FC0
int CResTPC::GetTXIDataSize()
{
    return m_nTXISize;
}

// FUNCTION: SWKOTOR 0x00712FD0
int CResTPC::OnResourceFreed()
{
    m_bCompressed = 0;
    m_bLoaded = 0;
    m_nDataSize = 0;
    m_nCompressedSize = 0;
    m_nTXISize = 0;
    m_pTPC = 0;
    m_pHeader = 0;
    m_pTXI = 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x00712FF0
int CResTPC::OnResourceServiced()
{
    int nFaces = 1;
    if (!m_pResource) {
        return 0;
    }
    if (!m_bLoaded) {
    if (m_pHeader) {
        return 0;
    }
    if (m_pTPC) {
        return 0;
    }
    unsigned char* pData = (unsigned char*)m_pResource + 0x80;
    m_pHeader = (TPCHeader*)m_pResource;
    m_pTPC = pData;
    int nFormat;
    if (m_pHeader->m_nEncoding & 1) {
        nFormat = 1;
    } else if (m_pHeader->m_nEncoding & 2) {
        nFormat = 3;
    } else if (m_pHeader->m_nEncoding & 4) {
        nFormat = 4;
    }
    if (m_pHeader->m_nDataSize == 0) {
        int nWidth = m_pHeader->m_nWidth;
        int nHeight = m_pHeader->m_nHeight;
        m_nDataSize = 0;
        m_nCompressedSize = 0;
        for (int i = 0; i < m_pHeader->m_nMipMaps; i++) {
            m_nDataSize += nHeight * nWidth * nFormat;
            nWidth >>= 1;
            nHeight >>= 1;
        }
        m_pTXI = pData + m_nDataSize;
        m_nTXISize = m_nSize - m_nDataSize - 0x80;
    } else {
        m_bCompressed = 1;
        m_nDataSize = 0;
        m_nCompressedSize = m_pHeader->m_nDataSize;
        int nBlockSize;
        if (nFormat == 3) {
            nBlockSize = 8;
        } else if (nFormat == 4) {
            nBlockSize = 16;
        }
        int nHeight = m_pHeader->m_nHeight;
        int nWidth = m_pHeader->m_nWidth;
        if (nHeight / nWidth == 6) {
            nFaces = 6;
            nHeight /= 6;
        }
        int nSize = m_pHeader->m_nDataSize * nFaces;
        for (int i = m_pHeader->m_nMipMaps - 1; i > 0; i--) {
            nHeight >>= 1;
            nWidth >>= 1;
            nSize += ((nHeight + 3) / 4) * ((nWidth + 3) / 4) * nBlockSize * nFaces;
        }
        m_pTXI = pData + nSize;
        m_nTXISize = m_nSize - nSize - 0x80;
    }
    m_bLoaded = 1;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00713170 SYMBOL: ??_GCResTPC@@UAEPAXI@Z
