// Pilot 4 arm A: CSWSCreature::SignalMeleeDamage (0x005B75D0) declarations.
#ifndef P4A_SIGNAL_MELEE_H
#define P4A_SIGNAL_MELEE_H

template <class T> class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
};

#pragma pack(push, 4)
class CGameEffect {
public:
    CGameEffect(int bCreateNewID);                      // 0x00503E40
    void SetCreator(unsigned long oid);                 // 0x00503A00
    void SetNumIntegers(int nNum);                      // 0x00503630
    void SetInteger(int nIndex, int nValue);            // 0x005036A0

    void SetDurationType(unsigned short nType) { m_nSubType = (m_nSubType & 0xfff8) | nType; }

    unsigned __int64 m_nID;                             // 0x00
    unsigned short m_nType;                             // 0x08
    unsigned short m_nSubType;                          // 0x0a
    char pad_0xc[0x8c - 0xc];
};
#pragma pack(pop)

extern const unsigned short EFFECT_TYPE_DAMAGE;         // 0x00746714
extern const unsigned short DURATION_TYPE_INSTANT;      // 0x007473B8
extern const unsigned long EVENT_ATTACK;                // 0x007441F0
extern const unsigned long EVENT_APPLY_EFFECT;          // 0x007441C8
extern const unsigned long EVENT_ON_HIT;                // 0x0074420C
extern const unsigned long EVENT_SELF;                  // 0x00744200

class CSWSCombatAttackData {
public:
    char pad_0x0[0x8];
    unsigned short m_nTime;                             // 0x08
    char pad_0xa[0x14 - 0xa];
    unsigned short m_nAnimationTime;                    // 0x14
    char pad_0x16[0x1c - 0x16];
    short m_nDamage[15];                                // 0x1c
    char pad_0x3a[0x5c - 0x3a];
    unsigned char m_nAttackResult;                      // 0x5c
    char pad_0x5d[0x88 - 0x5d];
    CExoArrayList<CGameEffect*> m_alstOnHitEffects;     // 0x88
    CExoArrayList<void*> m_alstSelfEvents;              // 0x94
    CExoArrayList<void*> m_alstOnHitEvents;             // 0xa0
};

class CSWSCombatRound {
public:
    CSWSCombatAttackData* GetAttack(int nAttack);       // 0x004D2B50
};

class CServerAIMaster {
public:
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long oidCaller,
                          unsigned long oidObject, unsigned long nEvent, void* pData); // 0x004B08D0
};

class CSWSCreature;

class CServerExoApp {
public:
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid); // 0x004AE770
    CServerAIMaster* GetServerAIMaster();               // 0x004AED80
};

class CClientExoApp {
public:
    unsigned long GetPlayerCreatureId();                // 0x005ED550
    unsigned long ServerToClientObjectId(unsigned long oid);    // 0x005EDA50
    void AddFloatyText(unsigned long oid, int nType, int nArg); // 0x005EDEA0 (placeholder name)
};

class CAppManager {
public:
    void* m_pUnknown00;                                 // 0x00
    CClientExoApp* m_pClientExoApp;                     // 0x04
    CServerExoApp* m_pServerExoApp;                     // 0x08

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
    CServerExoApp* GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager* g_pAppManager;                      // 0x007A39FC

class CSWSCreature {
public:
    void ApplyEffect(CGameEffect* pEffect, int bLoadingGame, int bFlag);   // 0x004D14F0
    void SignalMeleeDamage(unsigned long oidTarget, int nAttacks);         // 0x005B75D0

    void* m_pVtable;                                    // 0x00
    unsigned long m_idSelf;                             // 0x04
    char pad_0x8[0x558 - 0x8];
    int m_b558;                                         // 0x558
    char pad_0x55c[0x9c8 - 0x55c];
    CSWSCombatRound* m_pCombatRound;                    // 0x9c8
};

#endif
