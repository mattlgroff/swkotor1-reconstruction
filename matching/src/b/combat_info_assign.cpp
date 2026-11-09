// CCombatInformation::operator=: copy the scalars and clone both node lists.
#define B_COMBAT_INFO_NODE_INLINE
#include "b/server_info.h"

// FUNCTION: SWKOTOR 0x00552A00
CCombatInformation& CCombatInformation::operator=(CCombatInformation& other)
{
    if (this == &other) {
        return *this;
    }
    m_n0 = other.m_n0;
    m_n1 = other.m_n1;
    m_n2 = other.m_n2;
    m_n3 = other.m_n3;
    m_n4 = other.m_n4;
    m_nC = other.m_nC;
    m_n5 = other.m_n5;
    m_n6 = other.m_n6;
    m_n7 = other.m_n7;
    m_n8 = other.m_n8;
    m_n9 = other.m_n9;
    m_nA = other.m_nA;
    m_nB = other.m_nB;
    m_n10 = other.m_n10;
    m_n11 = other.m_n11;
    m_oid30 = other.m_oid30;
    m_oid2C = other.m_oid2C;
    m_lNodes14.SetSize(0);
    m_lNodes20.SetSize(0);
    int i;
    for (i = 0; i < other.m_lNodes14.num; i++) {
        CCombatInformationNode* pNode = new CCombatInformationNode;
        *pNode = *other.m_lNodes14[i];
        m_lNodes14.Add(pNode);
    }
    for (i = 0; i < other.m_lNodes20.num; i++) {
        CCombatInformationNode* pNode = new CCombatInformationNode;
        *pNode = *other.m_lNodes20[i];
        m_lNodes20.Add(pNode);
    }
    return *this;
}
