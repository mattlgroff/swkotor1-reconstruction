// CSWSFaction (b005b_f2_faction, 0x005BE780..0x005BFA70).

#include <math.h>

class Vector {
public:
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    float x, y, z;
};

class CExoString {
public:
    CExoString();
    CExoString(const char *s);
    ~CExoString();
    CExoString &operator=(const CExoString &s);
    char *m_sString;
    unsigned long m_nBufferLength;
};

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
    void Allocate(int n);
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num++] = t;
    }
    void Remove(T t);
    void AddUnique(T t);
    void Insert(T t, int nIndex);
};

class CSWSArea;

class CSWSCreatureStats {
public:
    unsigned char GetLevel(int bFalse);
    unsigned char GetClass(int nMultiClass);
    short GetArmorClass(int a, int b);
    unsigned char GetSimpleAlignmentGoodEvil();
    char pad_0x0[0x68];
    int m_nExperience;               // +0x68
    unsigned long m_nPartyTag;       // +0x6c
    char pad_0x70[0x8];
    int m_nFactionId;                // +0x78
    char pad_0x7c[0x8];
    float m_fChallengeRating;        // +0x84
    char pad_0x88[0x1];
    unsigned char m_nNumMultiClasses;  // +0x89
};

class CSWSFaction;

class CSWSCreature {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90();
    virtual int GetDead();                              // +0x94
    virtual short GetCurrentHitPoints(int bExcludeTemp); // +0x98
    virtual short GetMaxHitPoints(int bExcludeTemp);     // +0x9c

    CSWSArea *GetArea();
    int GetIsDying();
    void SetPartyInvite(int b);
    void SendInvite(unsigned long oid);
    CSWSFaction *GetFaction();
    int GetReputation(unsigned long oid, int b);

    unsigned long m_idSelf;          // +0x4
    char pad_0x8[0x88];
    Vector m_vPosition;              // +0x90
    char pad_0x9c[0x890];
    unsigned long m_oidInviter;      // +0x92c
    char pad_0x930[0xa4];
    int m_bPlayerCharacter;          // +0x9d4
    char pad_0x9d8[0x9c];
    CSWSCreatureStats *m_pStats;     // +0xa74
};

class CSWSArea {
public:
    int ClearLineOfSight(Vector *a, Vector *b, Vector *pHit, unsigned long *pOid,
                         unsigned long oid1, unsigned long oid2, int c, int d);
};

class CSWSPlayer {
public:
    char pad_0x0[0x4];
    unsigned long m_nPlayerID;       // +0x4
};

class CSWSMessage {
public:
    int SendServerToPlayerFactionUpdate(unsigned long nPlayer, int nCount,
                                        unsigned long *pIds, int nType,
                                        unsigned long oid);
    int SendServerToPlayerChatMessage(unsigned long nPlayer, unsigned long a, unsigned long b);
};

class CFactionManager {
public:
    int GetIsNPCFaction(int nFaction);
    int IsValidFactionId(int nFaction);
    void DeleteFaction(int nFaction);
    char pad_0x0[0x1c];
    int m_nNumPlayerFactions;        // +0x1c
    char pad_0x20[0x4];
    CExoArrayList<CSWSFaction *> *m_pFactionList; // +0x24
};

class CServerExoAppInternal {
public:
    char pad_0x0[0x10054];
    CFactionManager *m_pFactionManager; // +0x10054
};

class CServerExoApp {
public:
    CSWSCreature *GetCreatureByGameObjectID(unsigned long id);
    CSWSPlayer *GetClientObjectByObjectId(unsigned long id);
    CSWSMessage *GetNWSMessage();
    char pad_0x0[0x4];
    CServerExoAppInternal *m_pInternal; // +0x4
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;

class CSWSFaction {
public:
    CSWSFaction();
    CSWSFaction(unsigned long oidMember);
    ~CSWSFaction();
    void Initialise();
    int InviteMember(unsigned long oidInvitee, unsigned long oidInviter);
    void SendChatMessage(unsigned long a, unsigned long b);
    void SendFactionUpdateList();
    void SendFactionUpdateAdd(unsigned long oid);
    void SendFactionUpdateRemove(unsigned long oid);
    unsigned long GetWeakestMember(unsigned long oidTarget, int bMustSee);
    unsigned long GetStrongestMember(unsigned long oidTarget, int bMustSee);
    unsigned long GetMostDamagedMember(unsigned long oidTarget, int bMustSee);
    unsigned long GetLeastDamagedMember(unsigned long oidTarget, int bMustSee);
    unsigned long GetWorstAC(unsigned long oidTarget, int bMustSee);
    unsigned long GetBestAC(unsigned long oidTarget, int bMustSee);
    int GetAverageReputation(unsigned long oid);
    int GetAverageLevel();
    int GetMostFrequentClass();
    int GetSingletonParty();
    unsigned long GetFactionMember(int nIndex, unsigned long nTag);
    void RemoveMember(unsigned long oid);
    void AddMember(unsigned long oid, int bNoUpdate);
    int GetMemberList(unsigned long **ppList);
    int IsMember(unsigned long oid);
    int GetGold();
    int GetAverageGoodEvilAlignment();
    int GetAverageXP();
    unsigned long GetLeader();
    int TransferLeadership(unsigned long oidNew, unsigned long oidOld);

    CExoArrayList<unsigned long> m_listFactionMembers;  // +0x0
    int m_nFactionId;                // +0xc
    int m_n10;                       // +0x10
    int m_nIterator;                 // +0x14
    CExoString m_sFactionName;       // +0x18
    int m_bAmIAPartyFaction;         // +0x20
};

// FUNCTION: SWKOTOR 0x005BE790
int CSWSFaction::InviteMember(unsigned long oidInvitee, unsigned long oidInviter)
{
    g_pAppManager->GetServerExoApp()->GetNWSMessage();
    CSWSCreature *pInvitee = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidInvitee);
    if (pInvitee) {
        pInvitee->SetPartyInvite(1);
        pInvitee->m_oidInviter = oidInviter;
    }
    CSWSCreature *pInviter = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidInviter);
    if (pInviter) {
        pInviter->SendInvite(oidInvitee);
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005BE830
void CSWSFaction::Initialise()
{
    {
        CExoString sEmpty("");
        m_sFactionName = sEmpty;
    }
    m_nIterator = 0;
    m_bAmIAPartyFaction = 1;
}

// FUNCTION: SWKOTOR 0x005BE8A0
CSWSFaction::~CSWSFaction()
{
}

// FUNCTION: SWKOTOR 0x005BE900
void CSWSFaction::SendChatMessage(unsigned long a, unsigned long b)
{
    CSWSMessage *pMessage = g_pAppManager->GetServerExoApp()->GetNWSMessage();
    for (int i = 0; i < m_listFactionMembers.num; i++) {
        CSWSPlayer *pPlayer = g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(
            m_listFactionMembers.element[i]);
        if (pPlayer)
            pMessage->SendServerToPlayerChatMessage(pPlayer->m_nPlayerID, a, b);
    }
}

// FUNCTION: SWKOTOR 0x005BE960
void CSWSFaction::SendFactionUpdateList()
{
    CSWSMessage *pMessage = g_pAppManager->GetServerExoApp()->GetNWSMessage();
    for (int i = 0; i < m_listFactionMembers.num; i++) {
        CSWSPlayer *pPlayer = g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(
            m_listFactionMembers.element[i]);
        if (pPlayer)
            pMessage->SendServerToPlayerFactionUpdate(pPlayer->m_nPlayerID, m_listFactionMembers.num,
                                                      m_listFactionMembers.element, 1,
                                                      m_listFactionMembers.element[i]);
    }
}

// FUNCTION: SWKOTOR 0x005BE9C0
void CSWSFaction::SendFactionUpdateAdd(unsigned long oid)
{
    CSWSMessage *pMessage = g_pAppManager->GetServerExoApp()->GetNWSMessage();
    for (int i = 0; i < m_listFactionMembers.num; i++) {
        CSWSPlayer *pPlayer = g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(
            m_listFactionMembers.element[i]);
        if (pPlayer)
            pMessage->SendServerToPlayerFactionUpdate(pPlayer->m_nPlayerID, 1, &oid, 3,
                                                      m_listFactionMembers.element[i]);
    }
}

// FUNCTION: SWKOTOR 0x005BEA20
void CSWSFaction::SendFactionUpdateRemove(unsigned long oid)
{
    CSWSMessage *pMessage = g_pAppManager->GetServerExoApp()->GetNWSMessage();
    for (int i = 0; i < m_listFactionMembers.num; i++) {
        CSWSPlayer *pPlayer = g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(
            m_listFactionMembers.element[i]);
        if (pPlayer)
            pMessage->SendServerToPlayerFactionUpdate(pPlayer->m_nPlayerID, 1, &oid, 4, 0x7f000000);
    }
}

// FUNCTION: SWKOTOR 0x005BEA80
unsigned long CSWSFaction::GetWeakestMember(unsigned long oidTarget, int bMustSee)
{
    int nBest = 999;
    unsigned long oidResult = 0x7f000000;
    for (int i = 0; i < m_listFactionMembers.num; i++) {
        CSWSCreature *pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(
            m_listFactionMembers.element[i]);
        if (pCreature) {
            unsigned char nLevel = pCreature->m_pStats->GetLevel(0);
            if (nLevel < nBest) {
                if (oidTarget == 0x7f000000 || !bMustSee) {
                    nBest = nLevel;
                    oidResult = m_listFactionMembers.element[i];
                } else {
                    Vector vHit;
                    unsigned long oidHit;
                    CSWSCreature *pTarget = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
                    if (pTarget && pCreature->GetArea() &&
                        pCreature->GetArea()->ClearLineOfSight(&pCreature->m_vPosition, &pTarget->m_vPosition,
                                                               &vHit, &oidHit, 0x7f000000, 0x7f000000, 0, 0) == 1) {
                        nBest = nLevel;
                        oidResult = m_listFactionMembers.element[i];
                    }
                }
            }
        }
    }
    return oidResult;
}

// FUNCTION: SWKOTOR 0x005BEBC0
unsigned long CSWSFaction::GetStrongestMember(unsigned long oidTarget, int bMustSee)
{
    int nBest = 0;
    unsigned long oidResult = 0x7f000000;
    for (int i = 0; i < m_listFactionMembers.num; i++) {
        CSWSCreature *pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(
            m_listFactionMembers.element[i]);
        if (pCreature) {
            unsigned char nLevel = pCreature->m_pStats->GetLevel(0);
            if (nLevel > nBest) {
                if (oidTarget == 0x7f000000 || !bMustSee) {
                    nBest = nLevel;
                    oidResult = m_listFactionMembers.element[i];
                } else {
                    Vector vHit;
                    unsigned long oidHit;
                    CSWSCreature *pTarget = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
                    if (pTarget && pCreature->GetArea() &&
                        pCreature->GetArea()->ClearLineOfSight(&pCreature->m_vPosition, &pTarget->m_vPosition,
                                                               &vHit, &oidHit, 0x7f000000, 0x7f000000, 0, 0) == 1) {
                        nBest = nLevel;
                        oidResult = m_listFactionMembers.element[i];
                    }
                }
            }
        }
    }
    return oidResult;
}

// FUNCTION: SWKOTOR 0x005BED00
unsigned long CSWSFaction::GetMostDamagedMember(unsigned long oidTarget, int bMustSee)
{
    int nBest = -1;
    unsigned long oidResult = 0x7f000000;
    for (int i = 0; i < m_listFactionMembers.num; i++) {
        CSWSCreature *pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(
            m_listFactionMembers.element[i]);
        if (pCreature && !pCreature->GetDead() && !pCreature->GetIsDying()) {
            int nCurrent = pCreature->GetCurrentHitPoints(1);
            int nDamage = nCurrent - pCreature->GetMaxHitPoints(0);
            if (nDamage > nBest) {
                if (oidTarget == 0x7f000000 || !bMustSee) {
                    oidResult = m_listFactionMembers.element[i];
                    nBest = nDamage;
                } else {
                    Vector vHit;
                    unsigned long oidHit;
                    CSWSCreature *pTarget = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
                    if (pTarget && pCreature->GetArea() &&
                        pCreature->GetArea()->ClearLineOfSight(&pCreature->m_vPosition, &pTarget->m_vPosition,
                                                               &vHit, &oidHit, 0x7f000000, 0x7f000000, 0, 0) == 1) {
                        oidResult = m_listFactionMembers.element[i];
                        nBest = nDamage;
                    }
                }
            }
        }
    }
    return oidResult;
}

// FUNCTION: SWKOTOR 0x005BEE70
unsigned long CSWSFaction::GetLeastDamagedMember(unsigned long oidTarget, int bMustSee)
{
    int nBest = 999;
    unsigned long oidResult = 0x7f000000;
    for (int i = 0; i < m_listFactionMembers.num; i++) {
        CSWSCreature *pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(
            m_listFactionMembers.element[i]);
        if (!pCreature->GetDead() && !pCreature->GetIsDying()) {
            int nCurrent = pCreature->GetCurrentHitPoints(1);
            int nDamage = nCurrent - pCreature->GetMaxHitPoints(0);
            if (nDamage < nBest) {
                if (oidTarget == 0x7f000000 || !bMustSee) {
                    oidResult = m_listFactionMembers.element[i];
                    nBest = nDamage;
                } else {
                    Vector vHit;
                    unsigned long oidHit;
                    CSWSCreature *pTarget = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
                    if (pTarget && pCreature->GetArea() &&
                        pCreature->GetArea()->ClearLineOfSight(&pCreature->m_vPosition, &pTarget->m_vPosition,
                                                               &vHit, &oidHit, 0x7f000000, 0x7f000000, 0, 0) == 1) {
                        oidResult = m_listFactionMembers.element[i];
                        nBest = nDamage;
                    }
                }
            }
        }
    }
    return oidResult;
}

// FUNCTION: SWKOTOR 0x005BF340
unsigned long CSWSFaction::GetWorstAC(unsigned long oidTarget, int bMustSee)
{
    short nBest = 999;
    unsigned long oidResult = 0x7f000000;
    for (int i = 0; i < m_listFactionMembers.num; i++) {
        CSWSCreature *pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(
            m_listFactionMembers.element[i]);
        if (pCreature) {
            CSWSCreatureStats *pStats = pCreature->m_pStats;
            short nAC = pStats->GetArmorClass(0, 0);
            if (nAC < nBest) {
                if (oidTarget == 0x7f000000 || !bMustSee) {
                    nBest = nAC;
                    oidResult = m_listFactionMembers.element[i];
                } else {
                    Vector vHit;
                    unsigned long oidHit;
                    CSWSCreature *pTarget = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
                    if (pTarget && pCreature->GetArea() &&
                        pCreature->GetArea()->ClearLineOfSight(&pCreature->m_vPosition, &pTarget->m_vPosition,
                                                               &vHit, &oidHit, 0x7f000000, 0x7f000000, 0, 0) == 1) {
                        nBest = nAC;
                        oidResult = m_listFactionMembers.element[i];
                    }
                }
            }
        }
    }
    return oidResult;
}

// FUNCTION: SWKOTOR 0x005BF480
unsigned long CSWSFaction::GetBestAC(unsigned long oidTarget, int bMustSee)
{
    short nBest = 0;
    unsigned long oidResult = 0x7f000000;
    for (int i = 0; i < m_listFactionMembers.num; i++) {
        CSWSCreature *pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(
            m_listFactionMembers.element[i]);
        if (pCreature) {
            CSWSCreatureStats *pStats = pCreature->m_pStats;
            short nAC = pStats->GetArmorClass(0, 0);
            if (nAC > nBest) {
                if (oidTarget == 0x7f000000 || !bMustSee) {
                    nBest = nAC;
                    oidResult = m_listFactionMembers.element[i];
                } else {
                    Vector vHit;
                    unsigned long oidHit;
                    CSWSCreature *pTarget = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
                    if (pTarget && pCreature->GetArea() &&
                        pCreature->GetArea()->ClearLineOfSight(&pCreature->m_vPosition, &pTarget->m_vPosition,
                                                               &vHit, &oidHit, 0x7f000000, 0x7f000000, 0, 0) == 1) {
                        nBest = nAC;
                        oidResult = m_listFactionMembers.element[i];
                    }
                }
            }
        }
    }
    return oidResult;
}

// FUNCTION: SWKOTOR 0x005BF020
int CSWSFaction::GetAverageReputation(unsigned long oid)
{
    int nTotal = 0;
    CSWSCreature *pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oid);
    if (pCreature) {
        for (int i = 0; i < m_listFactionMembers.num; i++)
            nTotal += pCreature->GetReputation(m_listFactionMembers.element[i], 0);
    }
    int nAverage = nTotal / m_listFactionMembers.num;
    if (nAverage < 0)
        return 0;
    if (nAverage > 100)
        return 100;
    return nAverage;
}

// FUNCTION: SWKOTOR 0x005BF140
int CSWSFaction::GetAverageLevel()
{
    int nTotal = 0;
    for (int i = 0; i < m_listFactionMembers.num; i++) {
        CSWSCreature *pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(
            m_listFactionMembers.element[i]);
        if (pCreature) {
            CSWSCreatureStats *pStats = pCreature->m_pStats;
            nTotal += pStats->GetLevel(0);
        }
    }
    if (m_listFactionMembers.num > 0)
        return nTotal / m_listFactionMembers.num;
    return 0;
}

// FUNCTION: SWKOTOR 0x005BF1F0
int CSWSFaction::GetMostFrequentClass()
{
    int nResult = 0;
    int nMax = 0;
    int nCount = 0;
    for (int i = 0; i < m_listFactionMembers.num; i++) {
        CSWSCreature *pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(
            m_listFactionMembers.element[i]);
        if (pCreature) {
            for (int j = 0; j < pCreature->m_pStats->m_nNumMultiClasses; j++) {
                int nClass = pCreature->m_pStats->GetClass(j);
                for (int k = 0; k < m_listFactionMembers.num; k++) {
                    CSWSCreature *pOther = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(
                        m_listFactionMembers.element[k]);
                    if (pOther) {
                        for (int l = 0; l < pOther->m_pStats->m_nNumMultiClasses; l++) {
                            if (pOther->m_pStats->GetClass(l) == nClass)
                                nCount++;
                        }
                    }
                }
                if (nCount > nMax) {
                    nMax = nCount;
                    nResult = nClass;
                }
            }
        }
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x005BF690
int CSWSFaction::GetSingletonParty()
{
    int nPlayers;
    int i;
    for (i = 0, nPlayers = 0; i < m_listFactionMembers.num; i++) {
        CSWSCreature *pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(
            m_listFactionMembers.element[i]);
        if (pCreature && pCreature->m_bPlayerCharacter)
            nPlayers++;
        if (nPlayers > 1)
            return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005BF6E0
unsigned long CSWSFaction::GetFactionMember(int nIndex, unsigned long nTag)
{
    if (m_listFactionMembers.num >= 1) {
        for (int i = nIndex; i < m_listFactionMembers.num; i++) {
            CSWSCreature *pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(
                m_listFactionMembers.element[i]);
            if (pCreature && pCreature->m_pStats->m_nPartyTag == nTag) {
                m_nIterator = i + 1;
                return m_listFactionMembers.element[i];
            }
        }
    }
    return 0x7f000000;
}

// FUNCTION: SWKOTOR 0x005BF750 SYMBOL: ??0CSWSFaction@@QAE@K@Z
CSWSFaction::CSWSFaction(unsigned long oidMember)
{
    CSWSCreature *pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidMember);
    if (pCreature->GetFaction())
        pCreature->GetFaction()->RemoveMember(oidMember);
    m_listFactionMembers.Add(oidMember);
    CServerExoAppInternal *pInternal = g_pAppManager->GetServerExoApp()->m_pInternal;
    if (pInternal && pInternal->m_pFactionManager) {
        CFactionManager *pManager = pInternal->m_pFactionManager;
        pManager->m_pFactionList->Add(this);
        m_nFactionId = pManager->m_pFactionList->num - 1;
        pCreature->m_pStats->m_nFactionId = m_nFactionId;
    }
    Initialise();
}

// FUNCTION: SWKOTOR 0x005BF860 SYMBOL: ??0CSWSFaction@@QAE@XZ
CSWSFaction::CSWSFaction()
{
    CServerExoAppInternal *pInternal = g_pAppManager->GetServerExoApp()->m_pInternal;
    if (pInternal && pInternal->m_pFactionManager) {
        CFactionManager *pManager = pInternal->m_pFactionManager;
        pManager->m_pFactionList->Add(this);
        m_nFactionId = pManager->m_pFactionList->num - 1;
        if (m_nFactionId > 0)
            pManager->m_nNumPlayerFactions++;
    }
    Initialise();
}

// NONMATCH (7): in the DeleteFaction tail the original loads m_pStats into
// eax and m_nFactionId into ecx (ours ecx/edx); everything else matches.
// FUNCTION: SWKOTOR 0x005BF920
void CSWSFaction::RemoveMember(unsigned long oid)
{
    CSWSCreature *pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oid);
    if (!pCreature)
        return;
    int nFound = 0;
    for (int i = 0; i < m_listFactionMembers.num; i++) {
        if (m_listFactionMembers.element[i] == oid)
            nFound++;
    }
    if (!nFound)
        return;
    m_listFactionMembers.Remove(oid);
    int bPlayer = pCreature->m_bPlayerCharacter;
    if (bPlayer == 1) {
        CSWSFaction *pFaction = new CSWSFaction(oid);
        pFaction->SendFactionUpdateList();
    }
    if (!m_listFactionMembers.num) {
        CServerExoAppInternal *pInternal = g_pAppManager->GetServerExoApp()->m_pInternal;
        if (pInternal && pInternal->m_pFactionManager) {
            CFactionManager *pManager = pInternal->m_pFactionManager;
            if (!pManager->GetIsNPCFaction(m_nFactionId) && pCreature->m_pStats->m_nPartyTag == 1)
                pManager->DeleteFaction(m_nFactionId);
        }
    } else {
        SendFactionUpdateRemove(oid);
    }
}

// FUNCTION: SWKOTOR 0x005BFA70
void CSWSFaction::AddMember(unsigned long oid, int bNoUpdate)
{
    CSWSCreature *pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oid);
    if (!pCreature)
        return;
    CSWSFaction *pOld = pCreature->GetFaction();
    int nCount = m_listFactionMembers.num;
    unsigned long *pList = m_listFactionMembers.element;
    for (int i = 0; i < nCount; i++) {
        if (pList[i] == oid)
            return;
    }
    if (pOld)
        pOld->RemoveMember(oid);
    if (!bNoUpdate)
        SendFactionUpdateAdd(oid);
    m_listFactionMembers.AddUnique(oid);
    pCreature->m_pStats->m_nFactionId = m_nFactionId;
    CSWSMessage *pMessage = g_pAppManager->GetServerExoApp()->GetNWSMessage();
    CSWSPlayer *pPlayer = g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(oid);
    if (pPlayer)
        pMessage->SendServerToPlayerFactionUpdate(pPlayer->m_nPlayerID, m_listFactionMembers.num,
                                                  m_listFactionMembers.element, 1, oid);
}

// FUNCTION: SWKOTOR 0x005BE780
int CSWSFaction::GetMemberList(unsigned long **ppList)
{
    *ppList = m_listFactionMembers.element;
    return m_listFactionMembers.num;
}

// FUNCTION: SWKOTOR 0x005BE800
int CSWSFaction::IsMember(unsigned long oid)
{
    unsigned long *pList = m_listFactionMembers.element;
    int nCount = m_listFactionMembers.num;
    for (int i = 0; i < nCount; i++) {
        if (pList[i] == oid)
            return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005BEFE0
int CSWSFaction::GetGold()
{
    for (int i = 0; i < m_listFactionMembers.num; i++)
        g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(m_listFactionMembers.element[i]);
    return 0;
}

// FUNCTION: SWKOTOR 0x005BF090
int CSWSFaction::GetAverageGoodEvilAlignment()
{
    int nTotal = 0;
    for (int i = 0; i < m_listFactionMembers.num; i++) {
        CSWSCreature *pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(
            m_listFactionMembers.element[i]);
        if (pCreature)
            nTotal += pCreature->m_pStats->GetSimpleAlignmentGoodEvil();
    }
    int nNum = m_listFactionMembers.num;
    if (nNum > 0) {
        if (nNum == 1)
            return nTotal;
        float fAverage = (float)(nTotal / nNum);
        if (fAverage == 2.5f)
            return 1;
        if (fabs(fAverage - 2.0f) < fabs(fAverage - 3.0f))
            return 2;
        return 3;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005BF1A0
int CSWSFaction::GetAverageXP()
{
    int nTotal = 0;
    for (int i = 0; i < m_listFactionMembers.num; i++) {
        CSWSCreature *pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(
            m_listFactionMembers.element[i]);
        if (pCreature)
            nTotal += pCreature->m_pStats->m_nExperience;
    }
    if (m_listFactionMembers.num > 0)
        return nTotal / m_listFactionMembers.num;
    return 0;
}

// FUNCTION: SWKOTOR 0x005BFA30
int CSWSFaction::TransferLeadership(unsigned long oidNew, unsigned long oidOld)
{
    if (GetLeader() == oidOld) {
        m_listFactionMembers.Remove(oidNew);
        m_listFactionMembers.Insert(oidNew, 0);
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005BF5C0
unsigned long CSWSFaction::GetLeader()
{
    unsigned long oidLeader = 0x7f000000;
    CServerExoAppInternal *pInternal = g_pAppManager->GetServerExoApp()->m_pInternal;
    if (pInternal && pInternal->m_pFactionManager) {
        int nFactionId = m_nFactionId;
        if (pInternal->m_pFactionManager->IsValidFactionId(nFactionId)) {
            float fBest = 0.0f;
            for (int i = 0; i < m_listFactionMembers.num; i++) {
                CSWSCreature *pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(
                    m_listFactionMembers.element[i]);
                if (pCreature && pCreature->m_pStats->m_fChallengeRating > fBest) {
                    fBest = pCreature->m_pStats->m_fChallengeRating;
                    oidLeader = m_listFactionMembers.element[i];
                }
            }
        } else {
            for (int i = 0; i < m_listFactionMembers.num; i++) {
                CSWSCreature *pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(
                    m_listFactionMembers.element[i]);
                if (pCreature && pCreature->m_bPlayerCharacter)
                    return pCreature->m_idSelf;
            }
        }
    }
    return oidLeader;
}
