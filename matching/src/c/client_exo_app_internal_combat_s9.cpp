// CClientExoAppInternal party combat-target upkeep, worker C s9
// (clientexoappinternal.o; translation-unit-local layout).

class CSWCObject;
class CSWCCreature;
class CSWCDoor;
class CSWCPlaceable;
class CSWSObject;
class CSWSCreature;

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual CSWCObject* AsSWCObject();              // 0x0c
    virtual CSWSObject* AsSWSObject();              // 0x10
    virtual CSWCDoor* AsSWCDoor();                  // 0x14
    virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual CSWCCreature* AsSWCCreature();          // 0x2c
    virtual CSWSCreature* AsSWSCreature();          // 0x30

    unsigned long m_idSelf;                         // 0x04
    unsigned char m_nObjectType;                    // 0x08
};

class CSWCObject : public CGameObject {
public:
    virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual CSWCPlaceable* AsSWCPlaceable();        // 0x4c
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual void vF0(); virtual void vF4(); virtual void vF8(); virtual void vFC();
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10C();
    virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11C();
    virtual void v120(); virtual void v124(); virtual void v128(); virtual void v12C();
    virtual void v130(); virtual void v134();
    virtual int GetDead();                          // 0x138 (descriptive)
};

class CSWSDoor {
public:
    char pad_0x0[0x2c4];
    int m_bLocked;                                  // 0x2c4 (descriptive)
};

class CSWCDoor {
public:
    CSWSDoor* GetServerDoor();                      // 0x00683EE0
};

class CSWSPlaceable {
public:
    char pad_0x0[0x260];
    int m_bLocked;                                  // 0x260 (descriptive)
};

class CSWCPlaceable {
public:
    CSWSPlaceable* GetServerPlaceable();            // 0x00682E00
};

class CSWSObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90();
    virtual int GetDead();                          // 0x94 (descriptive)
};

class CSWSCreature {
public:
    CSWCCreature* GetClientCreature();              // 0x004EFB20

    char pad_0x0[0x4];
    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0x50c - 0x8];
    unsigned long m_oidAttackTarget;                // 0x50c (descriptive)
    char pad_0x510[0x520 - 0x510];
    unsigned long m_oidAttemptedTarget;             // 0x520 (descriptive)
    unsigned long m_oidLastAttacker;                // 0x524 (descriptive)
};

class CSWCCreature : public CSWCObject {
public:
    CSWSCreature* GetServerCreature();              // 0x0060FB20
    void SetAttackTarget(int b);                    // 0x00610A10

    char pad_0xc[0x440 - 0xc];
    unsigned long m_nPartyFlags;                    // 0x440 (descriptive; bit 0: in combat)
};

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long id, CGameObject** ppObject);  // 0x004D8230
};

extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C
extern const unsigned char OBJECT_TYPE_CREATURE;    // 0x00746634

struct CSWPartyMemberEntry {
    char pad_0x0[0x80];
    unsigned long m_oidTarget;                      // 0x80 (descriptive)
    unsigned long m_nCombatMessage;                 // 0x84 (descriptive)
};

class CSWParty {
public:
    CSWCCreature* GetCharacter(int n);              // 0x006346C0
    int IsPartyMember(unsigned long oid);           // 0x006349C0

    int m_nCharacters;                              // 0x00
    char pad_0x4[0x24 - 0x4];
    CSWPartyMemberEntry m_aMembers[3];              // 0x24
};

class CGuiInGame {
public:
    void SetCombatMessage(unsigned long nMessage);  // 0x0062B110
};

class CServerExoApp {
public:
    CGameObject* GetGameObject(unsigned long oid);  // 0x004AE750
};

class CClientExoApp {
public:
    CSWCCreature* GetCreatureByGameObjectID(unsigned long oid);   // 0x005ED5B0
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
    CServerExoApp* GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CClientExoAppInternal {
public:
    void UpdatePartyCombatTargets();                // descriptive

    __forceinline CSWCCreature* GetCachedPlayerCreature()
    {
        if (m_oidCachedPlayer == m_oidPlayer) {
            return m_pCachedPlayerCreature;
        }
        CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_oidPlayer);
        m_pCachedPlayerCreature = pCreature;
        m_oidCachedPlayer = m_oidPlayer;
        return pCreature;
    }

    char pad_0x0[0x14];
    CGameObjectArray* m_pGameObjectArray;           // 0x14
    char pad_0x18[0x20 - 0x18];
    unsigned long m_oidPlayer;                      // 0x20
    char pad_0x24[0x40 - 0x24];
    CGuiInGame* m_pGuiInGame;                       // 0x40
    char pad_0x44[0x270 - 0x44];
    CSWParty* m_pParty;                             // 0x270
    char pad_0x274[0x378 - 0x274];
    float m_fCombatPauseTimer;                      // 0x378 (descriptive)
    char pad_0x37c[0x394 - 0x37c];
    float m_fUnknown394;                            // 0x394
    char pad_0x398[0x3bc - 0x398];
    unsigned long m_oidCachedPlayer;                // 0x3bc
    CSWCCreature* m_pCachedPlayerCreature;          // 0x3c0
};

// NONMATCH (~119 insns): register assignment. The original keeps this in
// edi, the sticky door/placeable target in ebx (spilled and reloaded around
// the server-object checks) and the creature target in ebp, pushes ebp only
// inside the loop and needs a 0x18-byte frame; ours uses ebx for this and a
// 0x20-byte frame. Control flow and calls follow the original.
// Name unresolved (descriptive): clears the attack-target flag of party
// members whose targets are gone and sets it for those under attack.
// FUNCTION: SWKOTOR 0x005F3AD0
void CClientExoAppInternal::UpdatePartyCombatTargets()
{
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CSWCObject* pTargetObject = 0;
    for (int i = 0; i < m_pParty->m_nCharacters; i++) {
        CSWCCreature* pMember = m_pParty->GetCharacter(i);
        if (!pMember) {
            continue;
        }
        if (pMember->m_nPartyFlags & 1) {
            CSWCCreature* pTargetCreature = 0;
            CGameObject* pGameObject = 0;
            if (m_pGameObjectArray->GetGameObject(m_pParty->m_aMembers[i].m_oidTarget, &pGameObject) ==
                    GAME_OBJECT_VALID &&
                pGameObject && pGameObject->m_nObjectType == OBJECT_TYPE_CREATURE) {
                pTargetCreature = pGameObject->AsSWCCreature();
            }
            if (!pTargetCreature) {
                CGameObject* pOther = 0;
                if (m_pGameObjectArray->GetGameObject(m_pParty->m_aMembers[i].m_oidTarget, &pOther) ==
                        GAME_OBJECT_VALID &&
                    pOther) {
                    pTargetObject = pOther->AsSWCObject();
                }
            }
            if (i == 0 && m_fCombatPauseTimer > 0.0f) {
                continue;
            }
            if (pTargetCreature && pTargetCreature->GetDead()) {
                continue;
            }
            if (pTargetObject) {
                if (pTargetObject->AsSWCDoor() && pTargetObject->AsSWCDoor()->GetServerDoor() &&
                    pTargetObject->AsSWCDoor()->GetServerDoor()->m_bLocked) {
                    continue;
                }
                if (pTargetObject->AsSWCPlaceable() && pTargetObject->AsSWCPlaceable()->GetServerPlaceable() &&
                    pTargetObject->AsSWCPlaceable()->GetServerPlaceable()->m_bLocked) {
                    continue;
                }
            }
            CSWSCreature* pServerMember = pMember->GetServerCreature();
            if (pServerMember) {
                unsigned long oidAttack = pServerMember->m_oidAttackTarget;
                unsigned long oidAttacker = pServerMember->m_oidLastAttacker;
                unsigned long oidAttempted = pServerMember->m_oidAttemptedTarget;
                if (oidAttack != 0x7f000000) {
                    CGameObject* pObject = pServer->GetGameObject(oidAttack);
                    if (pObject && pObject->AsSWSObject() && !pObject->AsSWSObject()->GetDead()) {
                        continue;
                    }
                }
                if (oidAttacker != 0x7f000000 && oidAttacker != oidAttack) {
                    CGameObject* pObject = pServer->GetGameObject(oidAttacker);
                    if (pObject && pObject->AsSWSObject() && !pObject->AsSWSObject()->GetDead()) {
                        continue;
                    }
                }
                if (oidAttempted != 0x7f000000 && oidAttempted != oidAttack && oidAttempted != oidAttacker) {
                    CGameObject* pObject = pServer->GetGameObject(oidAttempted);
                    if (pObject && pObject->AsSWSObject() && !pObject->AsSWSObject()->GetDead()) {
                        if (!pObject->AsSWSCreature()) {
                            continue;
                        }
                        CSWCCreature* pClient = pObject->AsSWSCreature()->GetClientCreature();
                        if (!pClient || pClient->GetDead()) {
                            continue;
                        }
                    }
                }
            }
            m_fUnknown394 = -1.0f;
            pMember->SetAttackTarget(0);
        } else {
            CSWSCreature* pServerMember = pMember->GetServerCreature();
            if (!pServerMember) {
                continue;
            }
            unsigned long oidAttack = pServerMember->m_oidAttackTarget;
            unsigned long oidAttacker = pServerMember->m_oidLastAttacker;
            if (oidAttack != 0x7f000000) {
                CGameObject* pObject = pServer->GetGameObject(oidAttack);
                if (pObject && pObject->AsSWSObject() && !pObject->AsSWSObject()->GetDead()) {
                    pMember->SetAttackTarget(1);
                    continue;
                }
            }
            if (oidAttacker != 0x7f000000 && oidAttacker != pServerMember->m_idSelf && oidAttacker != oidAttack) {
                CGameObject* pObject = pServer->GetGameObject(oidAttacker);
                if (pObject && pObject->AsSWSObject() && !pObject->AsSWSObject()->GetDead()) {
                    pMember->SetAttackTarget(1);
                }
            }
        }
    }
    if (GetCachedPlayerCreature() && (GetCachedPlayerCreature()->m_nPartyFlags & 1)) {
        int nIndex = m_pParty->IsPartyMember(m_oidPlayer);
        if (nIndex >= 0 && m_pParty->m_aMembers[nIndex].m_nCombatMessage != 0xbc50) {
            m_pGuiInGame->SetCombatMessage(m_pParty->m_aMembers[nIndex].m_nCombatMessage);
        }
    }
}
