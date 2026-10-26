// CSWParty membership and formation helpers (swparty.o).
#include "n/sw_party.h"

// Boolean membership test by client object id. 0x006349C0 (IsPartyMember)
// returns the slot index instead; this name is descriptive.
// FUNCTION: SWKOTOR 0x00634620
int CSWParty::GetIsCharacterInParty(unsigned long nObject)
{
    for (int i = 0; i < m_nCount; ++i) {
        if (m_aMembers[i].m_oid == nObject)
            return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00634650 SYMBOL: ?RemoveCharacter@CSWParty@@QAEHH@Z
int CSWParty::RemoveCharacter(int nIndex)
{
    CAppManager* pApp = g_pAppManager;
    if (pApp == 0)
        return 0;
    CClientExoApp* pClient = pApp->m_pClientExoApp;
    CSWPartyMember* pMember = &m_aMembers[nIndex];
    int nResult = (int)pClient->GetCreatureByGameObjectID(pMember->m_oid);
    for (int i = nIndex; i < 2; ++i)
        m_aMembers[i].m_oid = m_aMembers[i + 1].m_oid;
    m_aMembers[2].m_oid = 0x7F000000;
    --m_nCount;
    return nResult;
}

// FUNCTION: SWKOTOR 0x006346F0
void CSWParty::InvertFormation()
{
    Vector v = m_aMembers[1].m_vFormationOffset;
    v.x = -v.x;
    m_aMembers[1].m_vFormationOffset = v;
    v = m_aMembers[2].m_vFormationOffset;
    v.x = -v.x;
    m_aMembers[2].m_vFormationOffset = v;
    m_bInverted = !m_bInverted;
}

// Same body as the exact 0x006349C0 definition in sw_party_lookup.cpp; kept
// here (unannotated) so the caller below can inline it.
int CSWParty::IsPartyMember(unsigned long nObject)
{
    if (nObject != 0x7F000000ul) {
        int nCount = m_nCount;
        int nIndex = 0;
        if (nCount > 0) {
            do {
                if (m_aMembers[nIndex].m_oid == nObject)
                    return nIndex;
                ++nIndex;
            } while (nIndex < nCount);
        }
    }
    return -1;
}

// FUNCTION: SWKOTOR 0x006349F0 SYMBOL: ?RemoveCharacter@CSWParty@@QAEHK@Z
int CSWParty::RemoveCharacter(unsigned long nObject)
{
    int nIndex = IsPartyMember(nObject);
    if (nIndex != -1 && RemoveCharacter(nIndex))
        return 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x00634A40
void CSWParty::ClearParty()
{
    for (int i = 0; i < m_nCount; ++i)
        m_aMembers[i].m_oid = 0x7F000000;
    m_nCount = 0;
}
