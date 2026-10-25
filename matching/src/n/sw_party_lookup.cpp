// CSWParty lookup helpers. The fixed record stride and client-object lookup
// were confirmed in the original executable and both Mac builds.
class CSWCCreature {
};

class CClientExoApp {
public:
    CSWCCreature* GetCreatureByGameObjectID(unsigned long nObject); // 0x005ED5B0
};

class CAppManager {
public:
    char pad_0x0[4];
    CClientExoApp* m_pClientExoApp;                   // +0x04
};

extern CAppManager* g_pAppManager;                    // 0x007A39FC

class CSWPartyMember {
public:
    unsigned long m_oid;
    char pad_0x4[0x84];
};

class CSWParty {
public:
    int m_nMemberCount;                               // +0x00
    char pad_0x4[0x20];
    CSWPartyMember m_aMembers[1];                     // +0x24, stride 0x88

    CSWCCreature* GetPlayerCharacter();
    // Mac debug-map name at 0x006346C0 is GetCharacter(int). Some call sites
    // call it GetMember; its return points to CClientExoApp's creature result.
    CSWCCreature* GetCharacter(int nIndex);
    // Returns the matching index or -1 (0xFFFFFFFF), not a boolean.
    int IsPartyMember(unsigned long nObject);
};

// FUNCTION: SWKOTOR 0x00635460
CSWCCreature* CSWParty::GetPlayerCharacter()
{
    if (m_nMemberCount > 0) {
        unsigned long nObject = m_aMembers[0].m_oid;
        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
        return pClient->GetCreatureByGameObjectID(nObject);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006346C0
CSWCCreature* CSWParty::GetCharacter(int nIndex)
{
    if (nIndex >= 0 && nIndex < m_nMemberCount) {
        unsigned long nObject = m_aMembers[nIndex].m_oid;
        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
        return pClient->GetCreatureByGameObjectID(nObject);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006349C0
int CSWParty::IsPartyMember(unsigned long nObject)
{
    if (nObject != 0x7F000000ul) {
        int nCount = m_nMemberCount;
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
