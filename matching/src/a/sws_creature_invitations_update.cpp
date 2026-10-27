// CSWSCreature invitation expiry (nwscreatureai.cpp). Descriptive name: no symbol record.
#include "a/sws_creature_invitations.h"

class CSWCCMessageData {
public:
    CSWCCMessageData();                             // 0x004D69F0
    void SetObjectID(int nIndex, unsigned long oid); // 0x004D7780
    char pad_0x0[0x34];
};

// The ignored list is compacted by an inlined removal (the offered list calls DelIndex).
static inline void RemoveInvitationAt(CExoArrayList<CSWSCreatureInvitation>* pList, int nIndex)
{
    pList->num--;
    for (int j = nIndex; j < pList->num; j++) pList->element[j] = pList->element[j + 1];
}

// NONMATCH: 18 diff: the inlined ignored-list removal keeps the decremented count in eax instead of ebp+copy.
// FUNCTION: SWKOTOR 0x00518660
void CSWSCreature::UpdateInvitations()
{
    unsigned long nDay, nTime, nDeltaDay, nDeltaTime;
    GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
    if (m_pInvitationsOffered) {
        int nCount = m_pInvitationsOffered->num;
        for (int i = nCount - 1; i >= 0; i--) {
            GetServerExoApp()->GetWorldTimer()->SubtractWorldTimes(nDay, nTime,
                m_pInvitationsOffered->element[i].m_nDay, m_pInvitationsOffered->element[i].m_nTime,
                &nDeltaDay, &nDeltaTime);
            if (nDeltaTime > 30000) {
                CSWSCreature* pOther =
                    GetServerExoApp()->GetCreatureByGameObjectID(m_pInvitationsOffered->element[i].m_oidCreature);
                if (pOther) pOther->SetInvitedToParty(0);
                m_pInvitationsOffered->DelIndex(i);
                CSWCCMessageData* pData = new CSWCCMessageData;
                pData->SetObjectID(0, m_idSelf);
                pOther->SendFeedbackMessage(0x2b, pData);
            }
        }
    }
    if (m_pInvitationsIgnored) {
        for (int i = m_pInvitationsIgnored->num - 1; i >= 0; i--) {
            GetServerExoApp()->GetWorldTimer()->SubtractWorldTimes(nDay, nTime,
                m_pInvitationsIgnored->element[i].m_nDay, m_pInvitationsIgnored->element[i].m_nTime,
                &nDeltaDay, &nDeltaTime);
            if (nDeltaTime > 60000) RemoveInvitationAt(m_pInvitationsIgnored, i);
        }
    }
    GetServerExoApp()->GetWorldTimer()->SubtractWorldTimes(nDay, nTime, m_nInvitedDay, m_nInvitedTime,
                                                          &nDeltaDay, &nDeltaTime);
    if (nDeltaTime > 30000) SetInvitedToParty(0);
}
