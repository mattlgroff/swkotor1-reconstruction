// CCombatInformation ctor/dtor/operator!= (0x00550E00-0x00552280).
#include "b/server_info.h"

// FUNCTION: SWKOTOR 0x00552280
CCombatInformation::CCombatInformation()
{
    m_n0 = 0;
    m_n1 = 0;
    m_n2 = 0;
    m_n3 = 0;
    m_n4 = 0;
    m_nC = 0;
    m_n5 = 0;
    m_n6 = 0;
    m_n7 = 0;
    m_n8 = 1;
    m_n9 = 3;
    m_nA = 0;
    m_nB = 2;
    m_n10 = 0;
    m_n11 = 2;
    m_oid2C = 0x7f000000;
    m_oid30 = 0x7f000000;
    m_s3C = "";
    m_s34 = "";
    m_n44 = 0;
    m_n45 = 0;
}

// FUNCTION: SWKOTOR 0x00550E00
CCombatInformation::~CCombatInformation()
{
    m_n0 = 0;
    m_n1 = 0;
    m_n2 = 0;
    m_n3 = 0;
    m_n4 = 0;
    m_nC = 0;
    m_n5 = 0;
    m_n6 = 0;
    m_n7 = 0;
    m_n8 = 1;
    m_n9 = 3;
    m_nA = 0;
    m_nB = 0;
    m_n10 = 0;
    m_n11 = 2;
    while (m_lNodes14.num > 0) {
        CCombatInformationNode* pNode = m_lNodes14[0];
        m_lNodes14.DelIndex(0);
        if (pNode) {
            delete pNode;
        }
    }
    while (m_lNodes20.num > 0) {
        CCombatInformationNode* pNode = m_lNodes20[0];
        m_lNodes20.DelIndex(0);
        if (pNode) {
            delete pNode;
        }
    }
}

// FUNCTION: SWKOTOR 0x005512D0
int CCombatInformation::operator!=(CCombatInformation& other)
{
    if (m_n0 != other.m_n0 || m_n1 != other.m_n1 || m_n2 != other.m_n2 ||
        m_n3 != other.m_n3 || m_n4 != other.m_n4 || m_nC != other.m_nC ||
        m_n5 != other.m_n5 || m_n6 != other.m_n6 || m_n7 != other.m_n7 ||
        m_n8 != other.m_n8 || m_n9 != other.m_n9 || m_nA != other.m_nA ||
        m_nB != other.m_nB || m_n10 != other.m_n10 || m_n11 != other.m_n11 ||
        m_oid30 != other.m_oid30 || m_oid2C != other.m_oid2C) {
        return 1;
    }
    int nCount14 = m_lNodes14.num;
    if (nCount14 != other.m_lNodes14.num) {
        return 1;
    }
    int nCount = m_lNodes20.num;
    if (nCount != other.m_lNodes20.num) {
        return 1;
    }
    int i = 0;
    if (nCount14 > 0) {
        CCombatInformationNode** pThis = m_lNodes14.element;
        CCombatInformationNode** pOther = other.m_lNodes14.element;
        for (; i < m_lNodes14.num; i++) {
            if (*pThis[i] != *pOther[i]) {
                return 1;
            }
        }
    }
    i = 0;
    if (nCount > 0) {
        CCombatInformationNode** pThis = m_lNodes20.element;
        CCombatInformationNode** pOther = other.m_lNodes20.element;
        do {
            if (*pThis[i] != *pOther[i]) {
                return 1;
            }
            i++;
        } while (i < nCount);
    }
    return 0;
}
