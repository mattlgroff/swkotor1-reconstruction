// Batch-local declarations: avoid introducing a zeroing array constructor into
// the established shared partial layouts. Invitation is a descriptive record
// name for the binary's {object id, world day, world time} 12-byte entries.
#ifndef A_SWS_CREATURE_INVITATIONS_H
#define A_SWS_CREATURE_INVITATIONS_H

struct CSWSCreatureInvitation {
    unsigned long m_oidCreature;
    unsigned long m_nDay;
    unsigned long m_nTime;
};
template <class T> class CExoArrayList {
public:
    ~CExoArrayList();
    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    void Allocate(int nSize);                       // 0x004186E0 (folded 12-byte record instantiation)
    void DelIndex(int nIndex);                      // 0x0048DDE0 (folded 12-byte record instantiation)
    void Add(T t) {
        if (num == array_size) Allocate(array_size ? array_size * 2 : 16);
        element[num] = t;
        num++;
    }
    T* element;
    int num;
    int array_size;
};
class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime); // 0x004ADE40
    int SubtractWorldTimes(unsigned long d1, unsigned long t1, unsigned long d2,
                          unsigned long t2, unsigned long* pDay, unsigned long* pTime); // 0x004AE460
};
class CSWSFaction {
public:
    void AddMember(unsigned long oidCreature, int bArg); // 0x005BFA70
};
class CSWSCreature {
public:
    int GetIsInInvitationsOffered(unsigned long oid); // 0x00518450 (descriptive reference name)
    int GetIsInInvitationsIgnored(unsigned long oid); // 0x00518580 (descriptive reference name)
    void RemoveFromInvitationsOffered(unsigned long oid); // 0x00518530 (descriptive reference name)
    void SendInvite(unsigned long oid);             // 0x0051B580 (existing symbol)
    void AddToInvitationsIgnored(unsigned long oid); // 0x0051B650 (descriptive reference name)
    int ReplyToInvitation(unsigned long nReply, unsigned long oidInviter); // 0x0051D060 (descriptive reference name)
    CSWSFaction* GetFaction();                      // 0x00513FC0
    void SetInvitedToParty(int bInvited);            // 0x004EF520

    char pad_0[4];
    unsigned long m_idSelf;
    char pad_8[0x92c - 8];
    unsigned long m_oidInviter;                     // 0x92c
    char pad_930[0x93c - 0x930];
    CExoArrayList<CSWSCreatureInvitation>* m_pInvitationsOffered; // 0x93c
    CExoArrayList<CSWSCreatureInvitation>* m_pInvitationsIgnored; // 0x940
};
class CServerExoApp {
public:
    CWorldTimer* GetWorldTimer();                   // 0x004AEDE0
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid); // 0x004AE770
};
class CAppManager {
public:
    char pad_0[8];
    CServerExoApp* m_pServerExoApp;
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC
inline CServerExoApp* GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }
#endif
