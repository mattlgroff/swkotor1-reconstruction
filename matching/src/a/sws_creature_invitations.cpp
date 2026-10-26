#include "a/sws_creature_invitations.h"

// FUNCTION: SWKOTOR 0x00518450
int CSWSCreature::GetIsInInvitationsOffered(unsigned long oid)
{
    if (m_pInvitationsOffered) {
        for (int i = m_pInvitationsOffered->num - 1; i >= 0; i--) {
            if (m_pInvitationsOffered->element[i].m_oidCreature == oid) {
                unsigned long nDay, nTime, nDeltaDay, nDeltaTime;
                GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
                GetServerExoApp()->GetWorldTimer()->SubtractWorldTimes(nDay, nTime,
                    m_pInvitationsOffered->element[i].m_nDay, m_pInvitationsOffered->element[i].m_nTime,
                    &nDeltaDay, &nDeltaTime);
                if (nDeltaTime <= 30000) return 1;
                m_pInvitationsOffered->DelIndex(i);
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00518580
int CSWSCreature::GetIsInInvitationsIgnored(unsigned long oid)
{
    if (m_pInvitationsIgnored) {
        for (int i = m_pInvitationsIgnored->num - 1; i >= 0; i--) {
            if (m_pInvitationsIgnored->element[i].m_oidCreature == oid) {
                unsigned long nDay, nTime, nDeltaDay, nDeltaTime;
                GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
                GetServerExoApp()->GetWorldTimer()->SubtractWorldTimes(nDay, nTime,
                    m_pInvitationsIgnored->element[i].m_nDay, m_pInvitationsIgnored->element[i].m_nTime,
                    &nDeltaDay, &nDeltaTime);
                if (nDeltaTime <= 60000) return 1;
                m_pInvitationsIgnored->DelIndex(i);
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00518530
void CSWSCreature::RemoveFromInvitationsOffered(unsigned long oid)
{
    if (m_pInvitationsOffered) {
        for (int i = m_pInvitationsOffered->num - 1; i >= 0; i--) {
            if (m_pInvitationsOffered->element[i].m_oidCreature == oid)
                m_pInvitationsOffered->DelIndex(i);
        }
    }
}

// NONMATCH: 24 diff: compiler omits original empty constructor EH envelope.
// FUNCTION: SWKOTOR 0x0051B580
void CSWSCreature::SendInvite(unsigned long oid)
{
    CSWSCreatureInvitation invitation;
    GetServerExoApp()->GetWorldTimer()->GetWorldTime(&invitation.m_nDay, &invitation.m_nTime);
    invitation.m_oidCreature = oid;
    if (!m_pInvitationsOffered) m_pInvitationsOffered = new CExoArrayList<CSWSCreatureInvitation>;
    m_pInvitationsOffered->Add(invitation);
}

// NONMATCH: 24 diff: compiler omits original empty constructor EH envelope.
// FUNCTION: SWKOTOR 0x0051B650
void CSWSCreature::AddToInvitationsIgnored(unsigned long oid)
{
    CSWSCreatureInvitation invitation;
    GetServerExoApp()->GetWorldTimer()->GetWorldTime(&invitation.m_nDay, &invitation.m_nTime);
    invitation.m_oidCreature = oid;
    if (!m_pInvitationsIgnored) m_pInvitationsIgnored = new CExoArrayList<CSWSCreatureInvitation>;
    m_pInvitationsIgnored->Add(invitation);
}

// NONMATCH: 21 diff: EBX/EBP lifetime and switch failure-tail layout.
// FUNCTION: SWKOTOR 0x0051D060
int CSWSCreature::ReplyToInvitation(unsigned long nReply, unsigned long oidInviter)
{
    unsigned long oid = oidInviter;
    int bResult = 0;
    CSWSCreature* pInviter = GetServerExoApp()->GetCreatureByGameObjectID(oid);
    if (pInviter && pInviter->GetIsInInvitationsOffered(m_idSelf) == 1 && m_oidInviter == oid) {
        switch (nReply) {
        case 2: AddToInvitationsIgnored(oid); break;
        case 0:
            if (!pInviter->GetFaction()) return 0;
            pInviter->GetFaction()->AddMember(m_idSelf, 0);
            break;
        case 1: break;
        default: return 0;
        }
        bResult = 1;
        unsigned long oidSelf = m_idSelf;
        pInviter->RemoveFromInvitationsOffered(oidSelf);
        SetInvitedToParty(0);
        m_oidInviter = 0x7f000000;
    }
    return bResult;
}
