// CSWPartyFollowDataArray: the party leader's follow-path ring (swparty.o).
#include "n/sw_party.h"

// FUNCTION: SWKOTOR 0x00635A90
CSWPartyFollowDataArray::CSWPartyFollowDataArray()
{
    Reset(Vector(0.0f, 0.0f, 0.0f), 0.0f);
}

// FUNCTION: SWKOTOR 0x00634B60
void CSWPartyFollowDataArray::Reset(const Vector& vPosition, float fFacing)
{
    CSWPartyFollowData* pData = m_aData;
    for (int i = 0; i < 10; ++i, pData += 10) {
        pData[0].m_vPosition = vPosition;
        pData[0].m_fFacing = fFacing;
        pData[1].m_vPosition = vPosition;
        pData[1].m_fFacing = fFacing;
        pData[2].m_vPosition = vPosition;
        pData[2].m_fFacing = fFacing;
        pData[3].m_vPosition = vPosition;
        pData[3].m_fFacing = fFacing;
        pData[4].m_vPosition = vPosition;
        pData[4].m_fFacing = fFacing;
        pData[5].m_vPosition = vPosition;
        pData[5].m_fFacing = fFacing;
        pData[6].m_vPosition = vPosition;
        pData[6].m_fFacing = fFacing;
        pData[7].m_vPosition = vPosition;
        pData[7].m_fFacing = fFacing;
        pData[8].m_vPosition = vPosition;
        pData[8].m_fFacing = fFacing;
        pData[9].m_vPosition = vPosition;
        pData[9].m_fFacing = fFacing;
    }
    m_nCount = 0;
    m_nHead = 0;
}

// FUNCTION: SWKOTOR 0x00634CC0
CSWPartyFollowData* CSWPartyFollowDataArray::GetPathData(int nIndex)
{
    if (nIndex < 0 || nIndex >= 100) {
        CExoString sMessage;
        sMessage.Format("PathFollowData requesting bad data position %d", nIndex);
    }
    return &m_aData[nIndex];
}

// FUNCTION: SWKOTOR 0x00634D40
int CSWPartyFollowDataArray::FindClosestPartyMemeber_Current()
{
    CSWParty* pParty = m_pParty;
    int nIndex1 = -1;
    int nIndex2 = -1;
    if (pParty->m_aMembers[1].m_oid != 0x7F000000)
        nIndex1 = pParty->m_aMembers[1].m_nPathIndex;
    if (pParty->m_aMembers[2].m_oid != 0x7F000000)
        nIndex2 = pParty->m_aMembers[2].m_nPathIndex;
    if (m_nHead > nIndex1 && nIndex1 != -1)
        nIndex1 += 100;
    if (m_nHead > nIndex2 && nIndex2 != -1)
        nIndex2 += 100;
    return (nIndex1 <= nIndex2) + 1;
}

// FUNCTION: SWKOTOR 0x00634DA0
int CSWPartyFollowDataArray::FindFurthestPartyMemeber_Current()
{
    CSWParty* pParty = m_pParty;
    int nIndex1 = -1;
    int nIndex2 = -1;
    if (pParty->m_aMembers[1].m_oid != 0x7F000000)
        nIndex1 = pParty->m_aMembers[1].m_nPathIndex;
    if (pParty->m_aMembers[2].m_oid != 0x7F000000)
        nIndex2 = pParty->m_aMembers[2].m_nPathIndex;
    if (m_nHead > nIndex1 && nIndex1 != -1)
        nIndex1 += 100;
    if (m_nHead > nIndex2 && nIndex2 != -1)
        nIndex2 += 100;
    return (nIndex1 >= nIndex2) + 1;
}

// FUNCTION: SWKOTOR 0x00634E00
float CSWPartyFollowDataArray::PointOnLineDistanceFromLeader(int nMember, int bSquared)
{
    CSWCCreature* pLeader = m_pParty->GetPlayerCharacter();
    Vector v = m_pParty->m_aMembers[nMember].m_vPathPoint - pLeader->m_vPosition;
    float fDistance = v.y * v.y + v.x * v.x + v.z * v.z;
    if (!bSquared)
        fDistance = (float)sqrt(fDistance);
    return fDistance;
}
