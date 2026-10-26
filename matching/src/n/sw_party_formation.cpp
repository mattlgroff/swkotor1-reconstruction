// CSWParty formation helpers (swparty.o).
#include "n/sw_party.h"

// Never written by the original; -1 means "use the party table state".
extern int g_nFollowStateOverride;                  // 0x007A2414, descriptive

// FUNCTION: SWKOTOR 0x00634760
void CSWParty::SetFormation(int nFormation)
{
    if (nFormation == m_nFormation || nFormation >= 3)
        return;
    if (nFormation >= 0) {
        m_aMembers[1].m_vFormationOffset = m_aFormations[nFormation][0];
        m_aMembers[2].m_vFormationOffset = m_aFormations[nFormation][1];
        m_nFormation = nFormation;
        m_fFollowDistance = m_aFormationDistances[nFormation];
    }
    else if (nFormation == -9999) {
        int nNext = m_nFormation + 1;
        if (nNext >= 3)
            nNext = -3;
        for (; nNext < 0; ++nNext) {
            if (m_abCustomFormations[-nNext - 1])
                break;
        }
        SetFormation(nNext);
        return;
    }
    else {
        if (nFormation < -3)
            return;
        if (!m_abCustomFormations[-nFormation - 1])
            return;
        m_aMembers[1].m_vFormationOffset = m_aCustomFormations[-nFormation - 1][0];
        m_aMembers[2].m_vFormationOffset = m_aCustomFormations[-nFormation - 1][1];
        m_nFormation = nFormation;
        m_fFollowDistance = m_aFormationDistances[-nFormation - 1];
    }
    if (m_bInverted) {
        Vector vTemp = m_aMembers[1].m_vFormationOffset;
        m_aMembers[1].m_vFormationOffset = m_aMembers[2].m_vFormationOffset;
        m_aMembers[2].m_vFormationOffset = vTemp;
    }
}

// FUNCTION: SWKOTOR 0x006348C0
Vector CSWParty::GetWorldFormationPosition(const Vector& vPosition, const Vector& vOrientation, int nIndex)
{
    Vector vResult(0.0f, 0.0f, 0.0f);
    if (nIndex == 0) {
        vResult = vPosition;
    }
    else if (nIndex > 0 && nIndex < 3) {
        Vector vForward(0.0f, 1.0f, 0.0f);
        Quaternion qRotation = RotationArc(vForward, vOrientation);
        vResult = vPosition + qRotation * m_aMembers[nIndex].m_vFormationOffset;
    }
    return vResult;
}

// FUNCTION: SWKOTOR 0x00634A70
float CSWParty::GetFollowRange()
{
    float fRange = 5.0f;
    int nState = g_pAppManager->m_pServerExoApp->GetPartyTable()->m_nFollowState;
    if (g_nFollowStateOverride != -1)
        nState = g_nFollowStateOverride;
    switch (nState) {
    case 0: {
        CExoString sColumn("PrimaryRange");
        C2DA* pTable = g_pRules->m_p2DArrays->m_pPartyAI;
        pTable->GetFLOATEntry(0xE, sColumn, &fRange);
        break;
    }
    case 1: {
        CExoString sColumn("PrimaryRange");
        C2DA* pTable = g_pRules->m_p2DArrays->m_pPartyAI;
        pTable->GetFLOATEntry(0xF, sColumn, &fRange);
        break;
    }
    }
    return fRange;
}
