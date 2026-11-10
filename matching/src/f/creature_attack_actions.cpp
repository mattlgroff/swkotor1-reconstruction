// CSWSCreature::CreateNewAttackActions in its own TU, so that CancelAllCombatModes
// stays a call (the original inlines only InitializeNewAttackTarget).

struct CSWSCombatRoundAction {
    char pad_0x0[0x38];
    unsigned long m_oidTarget; // +0x38
};

struct CExoLinkedListNode;

struct CExoLinkedListInternal {
    CExoLinkedListNode *pHead;
};

class CExoLinkedList {
public:
    CExoLinkedListInternal *m_pcExoLinkedListInternal;
    CExoLinkedListNode *GetHeadPos() { return m_pcExoLinkedListInternal->pHead; }
};

class CSWSCombatRound {
public:
    void ClearAllSpecialAttacks();
    void UpdateAttackTargetForAllActions(unsigned long oidTarget);
    void AddAttackAction(unsigned long oidTarget, unsigned short nAnimation, int bOverride,
                         int nDuration, int bAttack);

    char pad_0x0[0x9b0];
    CExoLinkedList *m_pScheduledActions;      // +0x9b0
    char pad_0x9b4[0x9cc - 0x9b4];
    unsigned long m_oidNewAttackTarget;       // +0x9cc
};

class Vector {
public:
    Vector() {}
    float x, y, z;
};
Vector operator-(const Vector &a, const Vector &b);

class CSWSCreature;

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual CSWSCreature *AsSWSCreature();   // +0x30
};

class CServerExoApp {
public:
    CGameObject *GetGameObject(unsigned long oid);
    CSWSCreature *GetCreatureByGameObjectID(unsigned long oid);
};

class CClientExoApp {
public:
    unsigned long ServerToClientObjectId(unsigned long oid);
    void SetLastTarget(unsigned long oid, int bForce);
};

class CAppManager {
public:
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }

    char pad_0x0[4];
    CClientExoApp *m_pClientExoApp;           // +0x4
    CServerExoApp *m_pServerExoApp;           // +0x8
};
extern CAppManager *g_pAppManager;

class CSWSCreature {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78();
    virtual void SetAnimation(int nAnimation);   // +0x7c
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90();
    virtual int GetDead();                        // +0x94

    void SetLockOrientationToObject(unsigned long oidObject, int bLock);
    void SetCombatMode(unsigned char nMode, int bForce);

    void CancelAllCombatModes(int bUnused);
    void InitializeNewAttackTarget(unsigned long oidTarget, CSWSCombatRoundAction *pAction)
    {
        if (pAction)
            pAction->m_oidTarget = oidTarget;
        SetLockOrientationToObject(oidTarget, 0);
        m_oidAttackTarget = oidTarget;
        m_oidLastAttackTarget = oidTarget;
        m_pcCombatRound->UpdateAttackTargetForAllActions(oidTarget);
        m_bAttackTargetChanged = 1;
    }
    void CreateNewAttackActions(int bFindNewTarget, int bCleave, unsigned long oidTarget);
    int GetDeadTemp();
    int GetReputation(unsigned long oidTarget, int bUnused);
    float MaxCleaveRange(unsigned long oidTarget, int bAssumeEquipped, int bIncludeReach);
    unsigned long GetNearestEnemy(float fRange, unsigned long oidCenter, unsigned long oidExclude);

    unsigned long m_idSelf;                   // +0x4
    char pad_0x8[0x84 - 0x8];
    unsigned long m_oidAttackTarget;          // +0x84
    char pad_0x88[0x90 - 0x88];
    Vector m_vPosition;                       // +0x90
    char pad_0x9c[0x4e8 - 0x9c];
    int m_bAttackTargetChanged;               // +0x4e8
    char pad_0x4ec[0x20];
    unsigned long m_oidLastAttackTarget;      // +0x50c
    unsigned long m_oidAttemptedAttackTarget; // +0x510
    char pad_0x514[0x524 - 0x514];
    unsigned long m_oidCleaveTarget;          // +0x524 (descriptive name)
    char pad_0x528[0x9c8 - 0x528];
    CSWSCombatRound *m_pcCombatRound;         // +0x9c8
};

// Name from the behavioural source; target from the cleave target or the combat
// round, else the nearest enemy (InitializeNewAttackTarget inlined).
// FUNCTION: SWKOTOR 0x005B6980
void CSWSCreature::CreateNewAttackActions(int bFindNewTarget, int bCleave, unsigned long oidTarget)
{
    int bAttack = 0;
    unsigned long oidAttackTarget;
    if (bCleave)
        oidAttackTarget = m_oidCleaveTarget;
    else
        oidAttackTarget = m_pcCombatRound->m_oidNewAttackTarget;

    if (oidAttackTarget != 0x7f000000 && !m_pcCombatRound->m_pScheduledActions->GetHeadPos()) {
        bAttack = 1;
        CGameObject *pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidAttackTarget);
        if (pObject) {
            if (pObject->AsSWSCreature()) {
                CSWSCreature *pTarget = pObject->AsSWSCreature();
                if (pTarget->GetDead() || pTarget->GetDeadTemp() || GetDead() || GetDeadTemp())
                    bAttack = 0;
                else if (GetReputation(oidAttackTarget, 0) > 10)
                    bAttack = 0;
                else if (bCleave) {
                    Vector vDiff = pTarget->m_vPosition - m_vPosition;
                    unsigned long oid = pTarget->m_idSelf;
                    float fRange = MaxCleaveRange(oid, 0, 1);
                    if (vDiff.x * vDiff.x + vDiff.y * vDiff.y + vDiff.z * vDiff.z > fRange * fRange)
                        bAttack = 0;
                }
            }
        } else {
            m_pcCombatRound->m_oidNewAttackTarget = 0x7f000000;
            bAttack = 0;
        }
    }

    if ((oidAttackTarget == 0x7f000000 && !m_pcCombatRound->m_pScheduledActions->GetHeadPos()) ||
        bFindNewTarget) {
        unsigned long oidNearest;
        CSWSCreature *pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
        if (!pCreature ||
            (oidNearest = pCreature->GetNearestEnemy(MaxCleaveRange(m_idSelf, 0, 1), m_idSelf, m_idSelf)) ==
                0x7f000000)
            oidNearest = GetNearestEnemy(MaxCleaveRange(m_idSelf, 0, 1), m_idSelf, 0x7f000000);
        if (oidNearest != 0x7f000000) {
            if (!g_pAppManager->GetServerExoApp()->GetGameObject(oidNearest)->AsSWSCreature()) {
                CancelAllCombatModes(1);
                return;
            }
            InitializeNewAttackTarget(oidNearest, 0);
            oidAttackTarget = oidNearest;
            CClientExoApp *pClient = g_pAppManager->m_pClientExoApp;
            pClient->SetLastTarget(pClient->ServerToClientObjectId(oidNearest), 1);
        } else {
            SetLockOrientationToObject(0x7f000000, 0);
            SetAnimation(10000);
            SetCombatMode(0, 1);
            m_pcCombatRound->ClearAllSpecialAttacks();
            return;
        }
    } else if (!bAttack)
        return;

    m_pcCombatRound->AddAttackAction(oidAttackTarget, 0, 0, 1500, 1);
    if (m_oidAttemptedAttackTarget == oidAttackTarget)
        m_oidAttemptedAttackTarget = 0x7f000000;
}

