// CSWParty constructor (swparty.o).

#define N_SW_PARTY_ZERO_VECTOR
#include "n/sw_party.h"

// FUNCTION: SWKOTOR 0x00636170
// NONMATCH: 14 diff insns, constant-register tie only: the original keeps
// 1.5f in ebp (copying it straight into the first formation offset and
// leaving dead stores in the shared Vector temporary); ours keeps 1.0f in
// ebp and loads 1.5f into esi later. Tried named temp, float local,
// init-list Vector ctor.
CSWParty::CSWParty()
{
    Vector v(1.5f, -0.7f, 0.0f);
    m_aFormations[0][0] = v;
    v = Vector(-1.5f, 0.8f, 0.0f);
    m_aFormations[0][1] = v;
    v = Vector(0.0f, 0.0f, 0.0f);
    m_aFormations[1][0] = v;
    v = Vector(0.0f, 0.0f, 0.0f);
    m_aFormations[1][1] = v;
    v = Vector(-2.0f, 1.5f, 0.0f);
    m_aFormations[2][0] = v;
    v = Vector(2.0f, 4.5f, 0.0f);
    m_aFormations[2][1] = v;
    m_aFormationDistances[0] = 1.5f;
    m_aFormationDistances[1] = 1.5f;
    m_aFormationDistances[2] = 1.5f;
    m_bInverted = 0;
    m_nFormation = -9999;
    SetFormation(0);
    m_fFollowDistance = 1.0f;
    m_abCustomFormations[0] = 0;
    m_abCustomFormations[1] = 0;
    m_abCustomFormations[2] = 0;
    for (int i = 0; i < 3; i++)
        m_aMembers[i].m_oid = 0x7f000000;
    m_v0C = Vector(0.0f, 0.0f, 0.0f);
    m_v2B0 = Vector(0.0f, 0.0f, 0.0f);
    m_nCount = 0;
    m_n04 = 0;
    m_bChanged = 0;
    m_n1C = 0;
    m_n2BC = 0;
    m_aMembers[0].m_fFollowDistance = 0.0f;
    m_aMembers[1].m_fFollowDistance = 1.5f;
    m_n08 = 1;
    m_f20 = 0.5f;
    m_cFollow.m_pParty = this;
    m_aMembers[2].m_fFollowDistance = 3.0f;
}
