// CRes subclasses of the Aurora resources, 0x00710B50-0x00711070 (PLT, VIS,
// TXI, MDX, DDS, 4PC).
#include "o/aurora_res.h"

void operator delete(void* p);

// ---------------------------------------------------------------- CResPLT

// FUNCTION: SWKOTOR 0x00710B50
CResPLT::CResPLT()
{
    m_bAllocated = 0;
    m_bRLE = 0;
    m_bLoaded = 0;
    m_nPLTSize = 0;
    m_nRLESize = 0;
    m_pPLT = 0;
    m_pHeader = 0;
    m_bLargePrefix = 1;
}

// FUNCTION: SWKOTOR 0x00710B80
CResPLT::~CResPLT()
{
}

// FUNCTION: SWKOTOR 0x00710B90
int CResPLT::GetPLTAttrib(unsigned long* pWidth, unsigned long* pHeight, unsigned long* pLayers)
{
    if (!m_pHeader) {
        return 0;
    }
    *pWidth = m_pHeader->m_nWidth;
    *pHeight = m_pHeader->m_nHeight;
    *pLayers = m_pHeader->m_nLayers;
    return 1;
}

// FUNCTION: SWKOTOR 0x00710BD0
int CResPLT::OnResourceFreed()
{
    if (m_bAllocated && m_pPLT) {
        delete m_pPLT;
    }
    m_bAllocated = 0;
    m_bRLE = 0;
    m_bLoaded = 0;
    m_nPLTSize = 0;
    m_nRLESize = 0;
    m_pPLT = 0;
    m_pHeader = 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x00710C10
int CResPLT::ReadRLEPLT()
{
    unsigned char* pRLE = m_pPLT;
    m_pPLT = new unsigned char[m_nPLTSize];
    if (!m_pPLT) {
        return 0;
    }
    unsigned long nIn = 0;
    unsigned long nOut = 0;
    while (nOut < m_nPLTSize) {
        unsigned char nCode = pRLE[nIn];
        unsigned char bRun = nCode & 0x80;
        nCode &= 0x7f;
        nIn++;
        unsigned long nEnd = nCode + nOut + 1;
        if (bRun) {
            unsigned char nValue = pRLE[nIn++];
            while (nOut < nEnd) {
                m_pPLT[nOut++] = nValue;
            }
        } else {
            while (nOut < nEnd) {
                m_pPLT[nOut++] = pRLE[nIn++];
            }
        }
    }
    m_bAllocated = 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x00710CC0
int CResPLT::OnResourceServiced()
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
    if (m_pPLT) {
        return 0;
    }
    m_pHeader = (PLTHeader*)m_pResource;
    if (m_pHeader->m_nFileType != 0x20544c50) {
        return 0;
    }
    if (m_pHeader->m_bRLE) {
        m_nRLESize = m_nSize - sizeof(PLTHeader);
        m_bRLE = 1;
        m_pPLT = (unsigned char*)m_pResource + sizeof(PLTHeader);
        if (!ReadRLEPLT()) {
            OnResourceFreed();
            return 0;
        }
    } else {
        m_bRLE = 0;
        m_bAllocated = 0;
        m_nPLTSize = m_nSize - sizeof(PLTHeader);
        m_nRLESize = m_nSize - sizeof(PLTHeader);
        m_pPLT = (unsigned char*)m_pResource + sizeof(PLTHeader);
    }
    m_bLoaded = 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x00710CA0 SYMBOL: ??_GCResPLT@@UAEPAXI@Z

// ---------------------------------------------------------------- CResVIS

// FUNCTION: SWKOTOR 0x00710D50
CResVIS::CResVIS()
{
    m_bLoaded = 0;
    m_nVISSize = 0;
    m_pVIS = 0;
    m_bHeaderPrefix = 1;
}

// FUNCTION: SWKOTOR 0x00710D80
CResVIS::~CResVIS()
{
}

// FUNCTION: SWKOTOR 0x00710D90 SYMBOL: ??_GCResVIS@@UAEPAXI@Z

// ---------------------------------------------------------------- CResTXI

// FUNCTION: SWKOTOR 0x00710DB0
CResTXI::CResTXI()
{
    m_bLoaded = 0;
    m_nTXISize = 0;
    m_pTXI = 0;
    m_bHeaderPrefix = 1;
}

// FUNCTION: SWKOTOR 0x00710DE0
CResTXI::~CResTXI()
{
}

// FUNCTION: SWKOTOR 0x00710DF0 SYMBOL: ??_GCResTXI@@UAEPAXI@Z

// ---------------------------------------------------------------- CResMDX

// FUNCTION: SWKOTOR 0x00710E10
CResMDX::CResMDX()
{
    m_bLoaded = 0;
    m_nMDXSize = 0;
    m_pMDX = 0;
    m_bHeaderPrefix = 1;
}

// FUNCTION: SWKOTOR 0x00710E40
CResMDX::~CResMDX()
{
}

// Identical code is shared with CResNCS/CResMDL/CRes2DA::Get*Size.
// FUNCTION: SWKOTOR 0x00710E50
int CResMDX::GetMDXSize()
{
    return m_nMDXSize;
}

// Shared by the LIP, VIS, TXI, MDX and MDL vtables.
// FUNCTION: SWKOTOR 0x00710E60
int CResMDX::OnResourceFreed()
{
    m_bLoaded = 0;
    m_nMDXSize = 0;
    m_pMDX = 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x00710E80 SYMBOL: ??_GCResMDX@@UAEPAXI@Z

// ---------------------------------------------------------------- CResDDS

// FUNCTION: SWKOTOR 0x00710EA0
CResDDS::CResDDS()
{
    m_bLoaded = 0;
    m_n2C = 0;
    m_pDDS = 0;
    m_pHeader = 0;
    m_bHeaderPrefix = 1;
}

// FUNCTION: SWKOTOR 0x00710ED0
CResDDS::~CResDDS()
{
}

// FUNCTION: SWKOTOR 0x00710EE0
int CResDDS::GetDDSAttrib(unsigned long* pWidth, unsigned long* pHeight, unsigned char* pBytesPerPixel,
                          unsigned long* pDataSize, float* pAlphaMean)
{
    if (!m_pHeader) {
        return 0;
    }
    *pWidth = m_pHeader->m_nWidth;
    *pHeight = m_pHeader->m_nHeight;
    *pBytesPerPixel = m_pHeader->m_nBytesPerPixel;
    *pDataSize = m_pHeader->m_nDataSize;
    *pAlphaMean = m_pHeader->m_fAlphaMean;
    return 1;
}

// FUNCTION: SWKOTOR 0x00710F30
int CResDDS::OnResourceServiced()
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
    if (m_pDDS) {
        return 0;
    }
    m_pHeader = (DDSHeader*)m_pResource;
    m_pDDS = (unsigned char*)m_pResource + sizeof(DDSHeader);
    m_bLoaded = 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x00710F70 SYMBOL: ??_GCResDDS@@UAEPAXI@Z

// ---------------------------------------------------------------- CRes4PC

// FUNCTION: SWKOTOR 0x00710F90
CRes4PC::CRes4PC()
{
    m_bLoaded = 0;
    m_n2C = 0;
    m_p4PC = 0;
    m_pHeader = 0;
    m_bHeaderPrefix = 1;
}

// FUNCTION: SWKOTOR 0x00710FC0
CRes4PC::~CRes4PC()
{
}

// FUNCTION: SWKOTOR 0x00710FD0
int CRes4PC::Get4PCAttrib(unsigned long* pWidth, unsigned long* pHeight, float* pAlphaMean, int* pDataSize)
{
    if (!m_pHeader) {
        return 0;
    }
    *pWidth = m_pHeader->m_nWidth;
    *pHeight = m_pHeader->m_nHeight;
    *pAlphaMean = m_pHeader->m_fAlphaMean;
    *pDataSize = m_pHeader->m_nDataSize;
    return 1;
}

// Also the CResDDS OnResourceFreed slot.
// FUNCTION: SWKOTOR 0x00711010
int CRes4PC::OnResourceFreed()
{
    m_bLoaded = 0;
    m_n2C = 0;
    m_p4PC = 0;
    m_pHeader = 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x00711030
int CRes4PC::OnResourceServiced()
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
    if (m_p4PC) {
        return 0;
    }
    m_pHeader = (FourPCHeader*)m_pResource;
    m_p4PC = (unsigned char*)m_pResource + sizeof(FourPCHeader);
    m_bLoaded = 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x00711070 SYMBOL: ??_GCRes4PC@@UAEPAXI@Z
