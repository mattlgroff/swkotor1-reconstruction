// CServerInfo (0x0054E9D0-0x0054EAA0) and CCombatInformationNode (0x0054F260-0x0054F280).
#include "b/server_info.h"
#include "b/sw_rules.h"
#include <string.h>

// FUNCTION: SWKOTOR 0x0054E9D0
int CServerInfo::FindOptionIndex(CExoString& sName, CExoString& sValue)
{
    for (int i = 1; i < 50; i++) {
        if (strcmp(sName.CStr(), m_aOptions[i].m_szName) == 0 &&
            strcmp(sValue.CStr(), m_aOptions[i].m_szValue) == 0) {
            return i;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0054EA70
void CServerInfo::SetDifficulty(int nLevel)
{
    if (nLevel > 4) {
        nLevel = 4;
    }
    else if (nLevel < 0) {
        nLevel = 0;
    }
    m_nDifficulty = nLevel;
}

// FUNCTION: SWKOTOR 0x0054EAA0
void CServerInfo::SetModuleName(CExoString& sName)
{
    m_sModuleName = sName;
    m_bSpecialModule = (m_sModuleName == "ebo_m12aa" || m_sModuleName == "tar_m02af" ||
                        m_sModuleName == "ebo_m40ad");
}

// FUNCTION: SWKOTOR 0x0054F260
CCombatInformationNode::CCombatInformationNode()
{
    m_n0 = 0;
    m_n1 = 0;
    m_n2 = 0;
    m_n3 = 0;
    m_n4 = g_pRules->m_nAA;
}

// FUNCTION: SWKOTOR 0x0054F280
int CCombatInformationNode::operator!=(CCombatInformationNode& other)
{
    if (m_n0 == other.m_n0 && m_n1 == other.m_n1 && m_n2 == other.m_n2 &&
        m_n3 == other.m_n3 && m_n4 == other.m_n4) {
        return 0;
    }
    return 1;
}
