// CServerExoAppInternal::UpdateClientGameObjectsForPlayer (pilot group 2).

#define OBJECT_INVALID 0x7f000000

class CExoTimers {
public:
    unsigned __int64 GetHighResolutionTimer();      // 0x005E9260
};

class CExoBase {
public:
    void* m_pUnknown00;
    CExoTimers* m_pcExoTimers;                      // 0x04
};

extern CExoBase* g_pExoBase;                        // 0x007A39E0

class CSWSArea;
class CSWSCreature;

class CGameObject {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1C();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2C();
    virtual CSWSCreature* AsSWSCreature();          // 0x30

    unsigned long m_idSelf;                         // 0x04
};

class CSWSObject : public CGameObject {
public:
    CSWSArea* GetArea();                            // 0x004CB120
};

class CSWSCreature : public CSWSObject {
public:
    char pad_0x08[0x308];
    unsigned long m_oidArea;                        // 0x310
};

#pragma pack(push, 4)
class CSWSPlayer {
public:
    void* vtable;
    unsigned long m_nPlayerID;                      // 0x04
    char pad_0x08[0x1c];
    char m_nPlayerType;                             // 0x24
    char pad_0x25[0x7];
    unsigned __int64 m_nLastUpdateTime;             // 0x2c
    unsigned long m_oidLastObject;                  // 0x34
    char pad_0x38[0x44];
    int m_nState;                                   // 0x7c

    CSWSObject* GetGameObject();                    // 0x00561080
};
#pragma pack(pop)

class CSWSMessage {
public:
    void SendServerToPlayerUpdate(CSWSPlayer* pPlayer);                     // 0x00578520
    void SendServerToPlayerSetObject(unsigned long nPlayerID, unsigned long oid);  // 0x0056F430
};

class CServerExoAppInternal {
public:
    char pad_0x00[0x10010];
    CSWSMessage* m_pSWSMessage;                     // 0x10010

    void UpdateClientGameObjectsForPlayer(CSWSPlayer* pPlayer, int bForce, unsigned __int64 nTime);
};

// FUNCTION: SWKOTOR 0x004B3EC0
void CServerExoAppInternal::UpdateClientGameObjectsForPlayer(CSWSPlayer* pPlayer, int bForce, unsigned __int64 nTime)
{
    unsigned __int64 nCurrentTime = nTime;
    if (bForce == 1 && nTime == 0) {
        nCurrentTime = g_pExoBase->m_pcExoTimers->GetHighResolutionTimer();
    }

    char nType = pPlayer->m_nPlayerType;
    if (nType != 2) {
        if (nType != 1 || pPlayer->m_nState != 1 || pPlayer->GetGameObject() == 0) {
            return;
        }
        CSWSCreature* pCreature = pPlayer->GetGameObject()->AsSWSCreature();
        if (pCreature == 0 || pCreature->m_oidArea == OBJECT_INVALID) {
            return;
        }
    }

    if (bForce == 1 || nTime - pPlayer->m_nLastUpdateTime >= 200000) {
        m_pSWSMessage->SendServerToPlayerUpdate(pPlayer);
        if (pPlayer->GetGameObject()) {
            unsigned long oid = pPlayer->GetGameObject()->m_idSelf;
            if (pPlayer->m_oidLastObject != oid && pPlayer->GetGameObject()->GetArea()) {
                m_pSWSMessage->SendServerToPlayerSetObject(pPlayer->m_nPlayerID, oid);
                pPlayer->m_oidLastObject = oid;
                pPlayer->m_nLastUpdateTime = nCurrentTime;
            }
        }
        pPlayer->m_nLastUpdateTime = nCurrentTime;
    }

    if (pPlayer->GetGameObject() &&
        pPlayer->GetGameObject()->AsSWSCreature()->GetArea() == 0 &&
        pPlayer->m_oidLastObject != OBJECT_INVALID) {
        pPlayer->m_oidLastObject = OBJECT_INVALID;
        m_pSWSMessage->SendServerToPlayerSetObject(pPlayer->m_nPlayerID, OBJECT_INVALID);
        pPlayer->m_nLastUpdateTime = nCurrentTime;
    }
}
