// CRes members (resource object base class).
#include "d/exo_key_table.h"

// FUNCTION: SWKOTOR 0x00405040
unsigned long CRes::GetStatus()
{
    return m_nStatus;
}

// FUNCTION: SWKOTOR 0x004064a0
void* CRes::GetData()
{
    return m_pResource;
}

// FUNCTION: SWKOTOR 0x004064c0
void CRes::GetResRef(CResRef& cResRef, unsigned short& nType)
{
    cResRef = m_pKeyEntry->m_cResRef;
    nType = m_pKeyEntry->m_nType;
}

// FUNCTION: SWKOTOR 0x00408620
int CRes::Request()
{
    CExoResMan* pResMan = g_pExoResMan;
    if (this == 0) {
        return 0;
    }
    if (m_nStatus & 4) {
        return 0;
    }
    if (m_nRequests > 0) {
        m_nRequests++;
        return 1;
    }
    m_nRequests = 0;
    pResMan->m_lstAsyncResQueue.AddTail(this);
    m_nRequests++;
    return 1;
}

// FUNCTION: SWKOTOR 0x00409b00
int CRes::CancelRequest()
{
    return g_pExoResMan->CancelRequest(this);
}

// FUNCTION: SWKOTOR 0x00409b10
void* CRes::Demand()
{
    return g_pExoResMan->Demand(this);
}

// FUNCTION: SWKOTOR 0x00409b80
int CRes::Release()
{
    return g_pExoResMan->Release(this);
}

// FUNCTION: SWKOTOR 0x00409b40
void CRes::Dump(int bRemove)
{
    if (this) {
        if (m_nDemands > 0) {
            m_nStatus |= 0x100;
            return;
        }
        if ((m_nStatus & 0x14) && m_pResource) {
            g_pExoResMan->Free(this);
        }
    }
}

// FUNCTION: SWKOTOR 0x00409d40
CRes::~CRes()
{
    if (m_nDemands > 0) {
        m_nStatus |= 0x100;
        return;
    }
    if ((m_nStatus & 0x14) && m_pResource) {
        g_pExoResMan->Free(this);
    }
}

// FUNCTION: SWKOTOR 0x00409d70 SYMBOL: ??_GCRes@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00406460
CRes::CRes()
{
    m_nDemands = 0;
    m_nID = 0xffffffff;
    m_nRequests = 0;
    m_nSize = 0;
    m_pResource = 0;
    m_pKeyEntry = 0;
    m_nRefs = 0;
    m_bHeaderPrefix = 0;
    m_bLargePrefix = 0;
    m_nStatus = 0;
}

// FUNCTION: SWKOTOR 0x00406490
int CRes::GetFixedResourceSize()
{
    return -1;
}

// FUNCTION: SWKOTOR 0x004064b0
int CRes::GetDemands()
{
    return m_nDemands;
}

// FUNCTION: SWKOTOR 0x00409b20
int CRes::ReadRaw(int nSize, char* pBuffer)
{
    return g_pExoResMan->ReadRaw(this, nSize, pBuffer);
}
