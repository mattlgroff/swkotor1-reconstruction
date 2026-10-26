// Compact combat/hostility utilities adjacent to the creature action family.
struct Vector { float x, y, z; };
class CGameEffect;
class CExoString {
public:
    CExoString(); // 0x005B3190
    ~CExoString(); // 0x005E5C20
    char* m_pString; unsigned long m_nLength;
};
class CServerAIMaster { public: void AddEventDeltaTime(unsigned long, unsigned long, unsigned long,
    unsigned long, unsigned long, void*); }; // 0x004B08D0
extern const int EVENT_AI_LATER; // 0x007441E0
extern const unsigned short EFFECT_TYPE_VISUALEFFECT; // 0x007466F4
extern const unsigned short DURATION_TYPE_7473B8; // 0x007473B8
class CSWSObject;
class CGameObject;
class CSWSCreature;
class CSWCCreature { public: void SetAttackTarget(int nTarget); }; // 0x00610A10
class CGameObjectClient {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual CSWCCreature* AsSWCCreature();
};
class CSWSCombatRound {
public:
    void EndCombatRound(int bCancel); // 0x004D4620
    char pad_0[0x9cc];
    unsigned long m_oidTarget;
};
class CSWSCreatureStats {
public:
    char pad_0[0x17e];
    unsigned short m_nGoodEvil;
};
class CSWSArea {
public:
    void ApplyEffect(CGameEffect*, const Vector&, const Vector&); // 0x0050C6B0
    char pad_0[0x190];
    unsigned long* m_pObjectIds;
    int m_nObjectCount;
};
class CServerExoApp {
public:
    class CSWSMessage* GetSWSMessage(); // 0x004AEDB0
    CServerAIMaster* GetServerAIMaster(); // 0x004AED80
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid); // 0x004AE770
};
class CAppManager { public: char pad_0[8]; CServerExoApp* m_pServer; };
extern CAppManager* g_pAppManager; // 0x007A39FC
const unsigned long OBJECT_INVALID = 0x7f000000;
#pragma pack(push, 4)
class CGameEffect {
public:
    CGameEffect(int bInitialize); // 0x00503E40
    void SetCreator(unsigned long oid); // 0x00503A00
    unsigned __int64 m_nId;
    unsigned short m_nType;
    union { unsigned short m_nSubType; struct { unsigned short m_nDuration : 3; unsigned short m_nMagicType : 2; unsigned short m_nFlags : 11; }; };
    char pad_c[0x1c-0xc];
    int m_nSpellId;
    char pad_20[0x8c-0x20];
};
#pragma pack(pop)
extern const int EFFECT_TYPE_0746828; // 0x00746828
extern const int EFFECT_TYPE_074682C; // 0x0074682C
extern const unsigned short DURATION_TYPE_7473C8; // 0x007473C8
class CSWSCreature {
public:
    int CalculateDamagePower(CSWSObject* pTarget);
    int GetTotalEffectBonus(unsigned char nType, CSWSObject* pTarget, int* pBonus,
        int, int, int, int, unsigned char, unsigned char, int); // 0x004F3FE0
    void ApplyEffect(CGameEffect*, int, int); // 0x004D14F0
    void UnsummonMyself(); // 0x005149B0
    void UpdateClassInfo(); // Established name at 0x00501D90, pure alignment powers.
    void AddPureGoodPowers(); // 0x004FDBD0
    void AddPureEvilPowers(); // 0x004FDC80
    void RemoveFirstEffect108(); // 0x004F77E0
    void RemoveFirstEffect109(); // 0x004F7830
    void PacifyCreature();
    void PacifyCreature(unsigned long oid);
    void CancelCombat(int bCancel);
    CSWSArea* GetArea(); // 0x004CB120
    void SetCombatState(int bCombat, unsigned char bUpdate); // 0x004F2610
    void SetLastHostileActor(unsigned long oid, int bUpdate); // 0x004CC0C0
    void RemoveTargetActions4F61D0(CGameObject* pTarget); // 0x004F61D0
    void ClearAllActions(int); // 0x004CCD80
    CGameObjectClient* GetClientObject(); // 0x004CC2B0
    char pad_0[4];
    unsigned long m_idSelf;
    char pad_8[0x90-8];
    Vector m_vPosition, m_vOrientation;
    char pad_a8[0x158-0xa8];
    unsigned long m_oidLastHostile;
    char pad_15c[0x504-0x15c];
    unsigned long m_oidAttemptedTarget;
    char pad_508[4];
    unsigned long m_oidAttackTarget;
    char pad_510[0x524-0x510];
    unsigned long m_oid524, m_oid528;
    char pad_52c[0x9c8-0x52c];
    CSWSCombatRound* m_pCombatRound;
    char pad_9cc[0xa74-0x9cc];
    CSWSCreatureStats* m_pStats;
};
// FUNCTION: SWKOTOR 0x004FD6F0
int CSWSCreature::CalculateDamagePower(CSWSObject* pTarget)
{
    int nBonus = 0;
    return GetTotalEffectBonus(1, pTarget, &nBonus, 0, 0, 0, 0, 0xff, 0xff, 0);
}
// FUNCTION: SWKOTOR 0x00501D90
void CSWSCreature::UpdateClassInfo()
{
    if (!m_pStats) return;
    unsigned short nAlignment = m_pStats->m_nGoodEvil;
    if (!nAlignment) {
        RemoveFirstEffect108();
        AddPureEvilPowers();
    } else if (nAlignment == 100) {
        RemoveFirstEffect109();
        AddPureGoodPowers();
    } else {
        RemoveFirstEffect108();
        RemoveFirstEffect109();
    }
}
// NONMATCH: 5 diff: initial receiver/argument scheduling and caller-clobbered register choice.
// FUNCTION: SWKOTOR 0x004FDA20 SYMBOL: ?PacifyCreature@CSWSCreature@@QAEXK@Z
void CSWSCreature::PacifyCreature(unsigned long oid)
{
    CAppManager* pApp = g_pAppManager;
    CSWSCreature* pOther = pApp->m_pServer->GetCreatureByGameObjectID(oid);
    if (pOther && (pOther->m_oidAttackTarget == m_idSelf ||
        pOther->m_oidAttemptedTarget == m_idSelf || pOther->m_oid524 == m_idSelf ||
        pOther->m_oid528 == m_idSelf || pOther->m_oidLastHostile == m_idSelf)) {
        pOther->SetCombatState(0, 1);
        if (pOther->m_oidLastHostile == m_idSelf)
            pOther->SetLastHostileActor(OBJECT_INVALID, 1);
        pOther->RemoveTargetActions4F61D0(reinterpret_cast<CGameObject*>(this));
    }
}
// NONMATCH: 7 diff: initial receiver/argument scheduling and caller-clobbered register choice.
// FUNCTION: SWKOTOR 0x004FD960 SYMBOL: ?PacifyCreature@CSWSCreature@@QAEXXZ
void CSWSCreature::PacifyCreature()
{
    CSWSArea* pArea = GetArea();
    if (!pArea) return;
    for (int i = 0; i < pArea->m_nObjectCount; i++) {
        CAppManager* pApp = g_pAppManager;
        CSWSCreature* pOther = pApp->m_pServer->GetCreatureByGameObjectID(pArea->m_pObjectIds[i]);
        if (pOther && (pOther->m_oidAttackTarget == m_idSelf ||
            pOther->m_oidAttemptedTarget == m_idSelf || pOther->m_oid524 == m_idSelf ||
            pOther->m_oid528 == m_idSelf || pOther->m_oidLastHostile == m_idSelf)) {
            pOther->SetCombatState(0, 1);
            if (pOther->m_oidLastHostile == m_idSelf)
                pOther->SetLastHostileActor(OBJECT_INVALID, 1);
            pOther->RemoveTargetActions4F61D0(reinterpret_cast<CGameObject*>(this));
        }
    }
}
// FUNCTION: SWKOTOR 0x004FDAA0
void CSWSCreature::CancelCombat(int bCancel)
{
    ClearAllActions(1);
    SetCombatState(0, 1);
    SetLastHostileActor(OBJECT_INVALID, 1);
    RemoveTargetActions4F61D0(0);
    m_pCombatRound->EndCombatRound(bCancel);
    m_pCombatRound->m_oidTarget = OBJECT_INVALID;
    if (GetClientObject()) {
        CSWCCreature* pCreature = GetClientObject()->AsSWCCreature();
        if (pCreature) pCreature->SetAttackTarget(0);
    }
}

// FUNCTION: SWKOTOR 0x004FDBD0
void CSWSCreature::AddPureGoodPowers()
{
    RemoveFirstEffect108();
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = (unsigned short)EFFECT_TYPE_0746828;
    pEffect->m_nSubType = (pEffect->m_nSubType & 0xfff8) | DURATION_TYPE_7473C8;
    pEffect->SetCreator(m_idSelf);
    pEffect->m_nMagicType = 0;
    pEffect->m_nSpellId = -1;
    ApplyEffect(pEffect, 0, 0);
}
// FUNCTION: SWKOTOR 0x004FDC80
void CSWSCreature::AddPureEvilPowers()
{
    RemoveFirstEffect109();
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = (unsigned short)EFFECT_TYPE_074682C;
    pEffect->m_nSubType = (pEffect->m_nSubType & 0xfff8) | DURATION_TYPE_7473C8;
    pEffect->SetCreator(m_idSelf);
    pEffect->m_nMagicType = 0;
    pEffect->m_nSpellId = -1;
    ApplyEffect(pEffect, 0, 0);
}

// NONMATCH: 8 diff: caller-clobbered registers for event and app-manager loads.
// FUNCTION: SWKOTOR 0x005149B0
void CSWSCreature::UnsummonMyself()
{
    CExoString sUnused;
    g_pAppManager->m_pServer->GetSWSMessage();
    ClearAllActions(1);
    CSWSArea* pArea = GetArea();
    if (pArea) {
        CGameEffect* pEffect = new CGameEffect(1);
        pEffect->m_nType = EFFECT_TYPE_VISUALEFFECT;
        pEffect->m_nSubType = (pEffect->m_nSubType & 0xfff8) | DURATION_TYPE_7473B8;
        pArea->ApplyEffect(pEffect, m_vPosition, m_vOrientation);
    }
    unsigned long nEvent = EVENT_AI_LATER;
    unsigned long oidSelf = m_idSelf;
    CServerExoApp* pServer = g_pAppManager->m_pServer;
    pServer->GetServerAIMaster()->AddEventDeltaTime(0, 0, oidSelf, oidSelf, nEvent, 0);
}
