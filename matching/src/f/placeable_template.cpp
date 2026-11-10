// CSWSPlaceable::LoadFromTemplate (0x00587A70) and CSWSPlaceable::EventHandler
// (0x00587BA0, vtable +0x78; its start is missing from functions.csv: it
// follows LoadFromTemplate's padding). Round s10. Own minimal layouts; most
// member names here are placeholders (offsets are from the original).

class CExoString {
public:
    CExoString();
    ~CExoString();
    CExoString &operator=(const CExoString &s);
    CExoString &operator=(const char *s);
    int operator==(const char *s) const;
    int IsEmpty() const;
    void Format(const char *szFormat, ...);
    char *CStr();
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    void CopyToString(CExoString &s) const;
    char m_resRef[16];
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

class CResGFF {
public:
    CResGFF(unsigned short nType, char *szFileType, const CResRef &cResRef);
    virtual ~CResGFF();
    void GetTopLevelStruct(CResStruct *pStruct);
    char pad_0x4[0x90];
    int m_bLoaded;                   // +0x94 (placeholder name)
    char pad_0x98[0x8];
};

class Vector {
public:
    float x, y, z;
};

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;
    int Contains(T t);
    void Remove(T t);
    void SetSize0()
    {
        if (element) {
            delete[] element;
            element = 0;
            array_size = 0;
        }
        num = 0;
    }
};

// Script event / feedback message payload (the original calls the same
// accessors for both).
class CScriptEvent {
public:
    CScriptEvent();
    ~CScriptEvent();
    int GetInteger(int nIndex);
    unsigned long GetObjectID(int nIndex);
    CExoString GetString(int nIndex);
    void SetObjectID(int nIndex, unsigned long oid);
    void SetString(int nIndex, const CExoString &s);
    unsigned char GetKind() { return *(unsigned char *)this; }
    unsigned short m_nType;          // +0x0
    char pad_0x2[0x32];
};

class CSWSCombatAttackData {
public:
    char pad_0x0[0x3a];
    unsigned char m_nWeaponAttackType;  // +0x3a
    unsigned char m_n3B;             // +0x3b
    char pad_0x3c[0x28];
    unsigned short m_n64;            // +0x64
    char pad_0x66[0x12];
    CExoString m_sDebug;             // +0x78
};

class CSWSSpellCastEvent {
public:
    unsigned long m_nSpellId;        // +0x0
    unsigned long m_oidCaster;       // +0x4
    unsigned long m_oidTarget;       // +0x8
    char pad_0xc[0x4];
    Vector m_vTarget;                // +0x10
    CExoString m_sScript;            // +0x1c
};

class CSWSItem {
public:
    char pad_0x0[0x14];
    unsigned long m_oidPossessor;    // +0x14 (placeholder name)
};

class CSWInventory {
public:
    CSWSItem *GetItemInSlot(unsigned long nSlot);
};

class CSWSCreatureStats {
public:
    int GetEffectImmunity(unsigned char nType, class CSWSCreature *pVersus);
    char pad_0x0[0x78];
    int m_nFactionId;                // +0x78
};

class CSWSArea {
public:
    char pad_0x0[0x12c];
    CExoArrayList<unsigned long> m_aTrapList;  // +0x12c (placeholder name)
};

class CGameEffect;

class CGameObject {
public:
    virtual ~CGameObject();
    unsigned long m_idSelf;          // +0x4
    unsigned char m_nObjectType;     // +0x8
    char pad_0x9[0x3];
};

class CSWSObject : public CGameObject {
public:
    virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20();
    virtual void v24(); virtual void v28(); virtual void v2c(); virtual void v30();
    virtual void v34(); virtual void v38(); virtual void v3c(); virtual void v40();
    virtual void v44(); virtual void v48(); virtual void v4c(); virtual void v50();
    virtual void v54(); virtual void v58(); virtual void v5c(); virtual void v60();
    virtual void v64(); virtual void v68(); virtual void v6c(); virtual void v70();
    virtual void v74();
    virtual void EventHandler(unsigned long nEventId, unsigned long oidCaller, void *pData,
                              unsigned long nCalendarDay, unsigned long nTimeOfDay);  // +0x78
    virtual void SetAnimation(int nAnimation);     // +0x7c
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90();
    virtual int GetDead();           // +0x94
    virtual void v98(); virtual void v9c(); virtual void va0(); virtual void va4();
    virtual void va8(); virtual void vac(); virtual void vb0(); virtual void vb4();
    virtual void vb8();
    virtual void SetLastSpellId(unsigned long nSpellId);  // +0xbc (placeholder name)

    CSWSArea *GetArea();
    void SetLastHostileActor(unsigned long oid, int bFlag);
    void ApplyEffect(CGameEffect *pEffect, int a, int b);
    void RemoveEffectById(unsigned long nLow, unsigned long nHigh);
    void SetSpellTargetPosition(const Vector &v);
    void ClearMatchedExpressionStrings();
    void AddMatchedExpressionString(const CExoString &s);
    void SpawnBodyBag();
    int GetReputation(unsigned long oid, int bFlag);

    char pad_0xc[0x84];
    Vector m_vPosition;              // +0x90
    char pad_0x9c[0xb0];
    unsigned long m_n14C;            // +0x14c
    int m_n150;                      // +0x150
    unsigned long m_oid154;          // +0x154
    char pad_0x158[0x4];
    unsigned long m_oidLastDamager;  // +0x15c
    unsigned long m_oidLastAttacker; // +0x160
    unsigned long m_oidLastDisturbed;   // +0x164
    char pad_0x168[0x4];
    unsigned short m_n16C;           // +0x16c
    unsigned char m_n16E;            // +0x16e
    char pad_0x16f;
    unsigned long m_oid170;          // +0x170
    unsigned long m_oid174;          // +0x174
    int m_n178;                      // +0x178
    unsigned long m_oidLastSpeaker;  // +0x17c
    int m_n180;                      // +0x180
    CExoString m_s184;               // +0x184
    char pad_0x18c[0xc];
    int m_n198;                      // +0x198
    char pad_0x19c[0x20];
    unsigned long m_oidSpellTarget;  // +0x1bc
    unsigned long m_nSpellId;        // +0x1c0
    char pad_0x1c4[0x14];
    unsigned long m_oid1D8;          // +0x1d8
    int m_n1DC;                      // +0x1dc
    int m_n1E0;                      // +0x1e0
    char pad_0x1e4[0x44];
};

class CSWSCreature : public CSWSObject {
public:
    void BroadcastSavingThrowData(CScriptEvent *pData);
    void SendFeedbackData(int nMessage, CScriptEvent *pData);
    void BroadcastDamageData(CScriptEvent *pMessage, CSWSCombatAttackData *pData);
    void RemoveCombatInvisibilityEffects();
    void BroadcastCombatStateToParty(unsigned long oid);
    void SetExcitedState(int nState);
    char pad_0x228[0x804];
    CSWInventory *m_pInventory;      // +0xa2c
    char pad_0xa30[0x44];
    CSWSCreatureStats *m_pStats;     // +0xa74
};

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long oid, CGameObject **ppObject);
};

class CServerExoAppInternal {
public:
    char pad_0x0[0x1005c];
    CGameObjectArray *m_pGameObjectArray;  // +0x1005c
};

class CServerAIMaster {
public:
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long oidCaller,
                          unsigned long oidObject, unsigned long nEvent, void *pData);
};

class CServerExoApp {
public:
    CSWSCreature *GetCreatureByGameObjectID(unsigned long oid);
    CSWSItem *GetItemByGameObjectID(unsigned long oid);
    CServerAIMaster *GetServerAIMaster();
    void SetLastConversation(const CExoString &s);
    void SetActionConversation(unsigned long n);
    void SetDialogState(int n);
    char pad_0x0[0x4];
    CServerExoAppInternal *m_pInternal;  // +0x4
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;

class CVirtualMachine {
public:
    int RunScript(CExoString *psFileName, unsigned long oid, int bOid);
    int RunScriptSituation(void *pSituation, unsigned long oid, int bOid);
};

extern CVirtualMachine *g_pVirtualMachine;
extern int g_bDebugEventStrings;                    // 0x008327E4 (placeholder name)
extern const unsigned char g_nImmunityTypeTrap;     // 0x00746910 (placeholder name)

extern const unsigned long EVENT_TIMED_EVENT;
extern const unsigned long EVENT_REMOVE_FROM_AREA;
extern const unsigned long EVENT_APPLY_EFFECT;
extern const unsigned long EVENT_OPEN_OBJECT;
extern const unsigned long EVENT_SPELL_IMPACT;
extern const unsigned long EVENT_PLAY_ANIMATION;
extern const unsigned long EVENT_SIGNAL_EVENT;      // 0x007441DC
extern const unsigned long EVENT_DESTROY_OBJECT;
extern const unsigned long EVENT_REMOVE_EFFECT;
extern const unsigned long EVENT_ON_MELEE_ATTACKED; // 0x007441F0
extern const unsigned long EVENT_FEEDBACK_MESSAGE;  // 0x0074420C (placeholder name)
extern const unsigned long EVENT_ACQUIRE_ITEM;      // 0x00744218

class CSWSPlaceable : public CSWSObject {
public:
    int LoadFromTemplate(const CResRef &cResRef, int bLoadInventory);
    int LoadPlaceable(CResGFF *pGFF, CResStruct *pStruct, int bLoadInventory);
    virtual void EventHandler(unsigned long nEventId, unsigned long oidCaller, void *pData,
                              unsigned long nCalendarDay, unsigned long nTimeOfDay);
    int AcquireItem(CSWSItem **ppItem, unsigned long oidPossessor, int bFlag);
    void ClosePlaceableForAllPlayers();
    void RemoveFromArea();
    void OpenInventory(unsigned long oidOpener, int bFlag);

    char pad_0x228[0x14];
    int m_nFactionId;                // +0x23c
    char pad_0x240[0x38];
    int m_bTrapFlag;                 // +0x278
    char pad_0x27c[0xc];
    int m_bTrapOneShot;              // +0x288
    char pad_0x28c[0x8];
    CExoString m_sScripts[16];       // +0x294
    char pad_0x314[0x4];
    CExoArrayList<unsigned long> m_aList;  // +0x318
    char pad_0x324[0x4];
    int m_bUseable;                  // +0x328
    char pad_0x32c[0x1c];
    unsigned long m_nLastHeartbeatDay;  // +0x348
    unsigned long m_nLastHeartbeatTime; // +0x34c
    unsigned long m_oid350;          // +0x350
    unsigned long m_oid354;          // +0x354
    unsigned long m_oid358;          // +0x358
    unsigned long m_oid35C;          // +0x35c
    unsigned long m_oid360;          // +0x360
    unsigned long m_oid364;          // +0x364
    unsigned long m_oid368;          // +0x368
};

// FUNCTION: SWKOTOR 0x00587A70
int CSWSPlaceable::LoadFromTemplate(const CResRef &cResRef, int bLoadInventory)
{
    CResGFF *pGFF = new CResGFF(0x7fc, "UTP ", cResRef);
    if (!pGFF->m_bLoaded) {
        delete pGFF;
        CExoString sError;
        CExoString sResRef;
        cResRef.CopyToString(sResRef);
        sError.Format("Item template %s doesn't exist.\n", sResRef.CStr());
        return 0;
    }
    CResStruct cTop;
    pGFF->GetTopLevelStruct(&cTop);
    LoadPlaceable(pGFF, &cTop, bLoadInventory);
    delete pGFF;
    return 1;
}

// NONMATCH (175, first draft, round s10): the control flow follows the original
// (feedback, damaged, acquire, the 0..0x1d signal-event switch, then the
// simple events); register roles and the shared RunScript tail still differ.
// FUNCTION: SWKOTOR 0x00587BA0
void CSWSPlaceable::EventHandler(unsigned long nEventId, unsigned long oidCaller, void *pData,
                                 unsigned long nCalendarDay, unsigned long nTimeOfDay)
{
    if (nEventId == EVENT_FEEDBACK_MESSAGE) {
        CScriptEvent *pMessage = (CScriptEvent *)pData;
        if (pMessage) {
            CSWSCreature *pCreature =
                g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCaller);
            if (pMessage->GetKind() == 1) {
                if (pCreature)
                    pCreature->BroadcastSavingThrowData(pMessage);
            } else if (pCreature) {
                pCreature->SendFeedbackData(pMessage->GetInteger(0), pMessage);
                return;
            }
            delete pMessage;
        }
        return;
    }
    if (nEventId == EVENT_ON_MELEE_ATTACKED) {
        CSWSCombatAttackData *pAttack = (CSWSCombatAttackData *)pData;
        CSWSCreature *pCreature;
        CSWSItem *pWeapon = 0;
        if (pAttack) {
            CScriptEvent *pMessage = new CScriptEvent;
            pMessage->SetObjectID(0, oidCaller);
            pMessage->SetObjectID(1, m_idSelf);
            if (g_bDebugEventStrings)
                pMessage->SetString(0, pAttack->m_sDebug);
            pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCaller);
            if (pCreature) {
                pCreature->BroadcastDamageData(pMessage, pAttack);
                pCreature->RemoveCombatInvisibilityEffects();
            }
            delete pMessage;
            m_n16C = pAttack->m_n64;
            m_n16E = pAttack->m_n3B;
            unsigned char nType = pAttack->m_nWeaponAttackType;
            if (nType == 1 || nType == 6)
                pWeapon = pCreature->m_pInventory->GetItemInSlot(0x10);
            else if (nType == 2)
                pWeapon = pCreature->m_pInventory->GetItemInSlot(0x20);
            else if (nType == 7 || nType == 8)
                pWeapon = pCreature->m_pInventory->GetItemInSlot(8);
            else if (nType == 3)
                pWeapon = pCreature->m_pInventory->GetItemInSlot(0x4000);
            else if (nType == 4)
                pWeapon = pCreature->m_pInventory->GetItemInSlot(0x8000);
            else if (nType == 5)
                pWeapon = pCreature->m_pInventory->GetItemInSlot(0x10000);
        } else {
            // The original reuses the caller id as the creature pointer here.
            pCreature = (CSWSCreature *)oidCaller;
        }
        m_oidLastDamager = oidCaller;
        SetLastHostileActor(oidCaller, 0);
        if (pCreature) {
            pCreature->BroadcastCombatStateToParty(0x7f000000);
            pCreature->RemoveCombatInvisibilityEffects();
            if (pWeapon)
                pCreature->m_oid170 = pWeapon->m_oidPossessor;
            else
                pCreature->m_oid170 = 0x7f000000;
        }
        g_pVirtualMachine->RunScript(&m_sScripts[7], m_idSelf, 1);
        if (m_bTrapFlag == 1 && pCreature) {
            float fZ = pCreature->m_vPosition.z - m_vPosition.z;
            float fY = pCreature->m_vPosition.y - m_vPosition.y;
            float fX = pCreature->m_vPosition.x - m_vPosition.x;
            if (fX * fX + fY * fY + fZ * fZ <= 16.0f) {
                CScriptEvent *pEvent = new CScriptEvent;
                pEvent->m_nType = 0x1a;
                g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                    0, 0, oidCaller, m_idSelf, EVENT_SIGNAL_EVENT, pEvent);
            }
        }
        return;
    }
    if (nEventId == EVENT_ACQUIRE_ITEM) {
        CSWSItem *pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidCaller);
        if (pItem)
            AcquireItem(&pItem, 0x7f000000, 1);
        return;
    }
    if (nEventId == EVENT_SIGNAL_EVENT) {
        CScriptEvent *pEvent = (CScriptEvent *)pData;
        switch (pEvent->m_nType) {
        case 0x16:
            m_oid350 = oidCaller;
            g_pVirtualMachine->RunScript(&m_sScripts[8], m_idSelf, 1);
            break;
        case 0x17:
            m_oid354 = oidCaller;
            g_pVirtualMachine->RunScript(&m_sScripts[0], m_idSelf, 1);
            break;
        case 4:
            m_oidLastAttacker = oidCaller;
            SetLastHostileActor(oidCaller, 0);
            g_pVirtualMachine->RunScript(&m_sScripts[1], m_idSelf, 1);
            break;
        case 0xa:
            SetAnimation(10072);
            m_oid154 = oidCaller;
            g_pVirtualMachine->RunScript(&m_sScripts[2], m_idSelf, 1);
            break;
        case 0x18:
            if (m_bTrapFlag) {
                m_bTrapFlag = 0;
                m_oid360 = oidCaller;
                g_pVirtualMachine->RunScript(&m_sScripts[3], m_idSelf, 1);
            }
            if (m_aList.num > 0)
                m_aList.SetSize0();
            break;
        case 0:
            m_nLastHeartbeatDay = nCalendarDay;
            m_nLastHeartbeatTime = nTimeOfDay;
            g_pVirtualMachine->RunScript(&m_sScripts[4], m_idSelf, 1);
            break;
        case 0x19:
            if (m_bUseable) {
                m_oid358 = oidCaller;
                g_pVirtualMachine->RunScript(&m_sScripts[12], m_idSelf, 1);
            }
            break;
        case 0x1a: {
            int bForce = pEvent->GetInteger(0);
            CSWSCreature *pCreature =
                g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCaller);
            if (pCreature && pCreature->m_pStats->GetEffectImmunity(g_nImmunityTypeTrap, 0))
                break;
            if (!bForce) {
                if (GetReputation(oidCaller, 0) >= 90)
                    break;
                if (m_nFactionId == pCreature->m_pStats->m_nFactionId)
                    break;
            }
            if (!m_bTrapFlag)
                break;
            if (pCreature)
                pCreature->SendFeedbackData(0x52, 0);
            m_oid35C = oidCaller;
            g_pVirtualMachine->RunScript(&m_sScripts[10], m_idSelf, 1);
            if (m_bTrapOneShot) {
                m_bTrapFlag = 0;
                if (GetArea() && &GetArea()->m_aTrapList) {
                    if (GetArea()->m_aTrapList.Contains(m_idSelf) == 1)
                        GetArea()->m_aTrapList.Remove(m_idSelf);
                }
            }
            if (m_aList.num > 0)
                m_aList.SetSize0();
            break;
        }
        case 7: {
            m_oidLastSpeaker = oidCaller;
            m_n198 = pEvent->GetInteger(1);
            m_s184 = pEvent->GetString(0);
            g_pAppManager->GetServerExoApp()->SetLastConversation(pEvent->GetString(0));
            m_n180 = pEvent->GetInteger(2);
            g_pAppManager->GetServerExoApp()->SetActionConversation(1);
            g_pAppManager->GetServerExoApp()->SetDialogState(pEvent->GetInteger(3));
            ClearMatchedExpressionStrings();
            for (int i = 1; i < pEvent->GetInteger(0); i++)
                AddMatchedExpressionString(pEvent->GetString(i));
            CExoString *pScript = &m_sScripts[14];
            if (pScript) {
                if (pScript->IsEmpty() || *pScript == "default")
                    *pScript = "k_hen_dialogue01";
            }
            g_pVirtualMachine->RunScript(pScript, m_idSelf, 1);
            break;
        }
        case 0x1b:
            m_oidLastDisturbed = oidCaller;
            m_n178 = pEvent->GetInteger(0);
            m_oid174 = pEvent->GetObjectID(0);
            g_pVirtualMachine->RunScript(&m_sScripts[5], m_idSelf, 1);
            break;
        case 0x1c:
            m_oid364 = oidCaller;
            g_pVirtualMachine->RunScript(&m_sScripts[6], m_idSelf, 1);
            break;
        case 0x1d:
            m_oid368 = oidCaller;
            g_pVirtualMachine->RunScript(&m_sScripts[11], m_idSelf, 1);
            break;
        case 2:
            m_oid1D8 = pEvent->GetObjectID(0);
            m_n1DC = pEvent->GetInteger(0);
            m_n1E0 = pEvent->GetInteger(1);
            if (m_n1E0) {
                SetLastHostileActor(m_oid1D8, 0);
                CSWSCreature *pAttacker =
                    g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oid1D8);
                if (pAttacker) {
                    pAttacker->BroadcastCombatStateToParty(0x7f000000);
                    pAttacker->SetExcitedState(2);
                    pAttacker->RemoveCombatInvisibilityEffects();
                }
            }
            g_pVirtualMachine->RunScript(&m_sScripts[9], m_idSelf, 1);
            break;
        case 0xb:
            m_n150 = pEvent->GetInteger(0);
            g_pVirtualMachine->RunScript(&m_sScripts[13], m_idSelf, 1);
            break;
        }
        delete pEvent;
        return;
    }
    if (nEventId == EVENT_DESTROY_OBJECT) {
        ClosePlaceableForAllPlayers();
        SpawnBodyBag();
        RemoveFromArea();
        delete this;
        return;
    }
    if (nEventId == EVENT_REMOVE_FROM_AREA) {
        RemoveFromArea();
        return;
    }
    if (nEventId == EVENT_APPLY_EFFECT) {
        if (pData)
            ApplyEffect((CGameEffect *)pData, 0, 0);
        return;
    }
    if (nEventId == EVENT_REMOVE_EFFECT) {
        if (pData)
            RemoveEffectById(((unsigned long *)pData)[0], ((unsigned long *)pData)[1]);
        return;
    }
    if (nEventId == EVENT_PLAY_ANIMATION) {
        if (pData && !GetDead())
            SetAnimation((int)pData);
        return;
    }
    if (nEventId == EVENT_TIMED_EVENT) {
        if (pData)
            g_pVirtualMachine->RunScriptSituation(pData, m_idSelf, 1);
        return;
    }
    if (nEventId == EVENT_SPELL_IMPACT) {
        CSWSSpellCastEvent *pSpell = (CSWSSpellCastEvent *)pData;
        if (pSpell && pSpell->m_oidCaster == oidCaller) {
            m_nSpellId = pSpell->m_nSpellId;
            m_oidSpellTarget = pSpell->m_oidTarget;
            SetSpellTargetPosition(pSpell->m_vTarget);
            if (pSpell->m_oidTarget != 0x7f000000) {
                CGameObject *pTarget;
                if (g_pAppManager->GetServerExoApp()->m_pInternal->m_pGameObjectArray->GetGameObject(
                        pSpell->m_oidTarget, &pTarget) == 0) {
                    if (pTarget->m_nObjectType > 4) {
                        if (GetArea() == ((CSWSObject *)pTarget)->GetArea())
                            SetSpellTargetPosition(((CSWSObject *)pTarget)->m_vPosition);
                    }
                }
            }
            SetLastSpellId(pSpell->m_nSpellId);
            m_n14C = pSpell->m_nSpellId;
            g_pVirtualMachine->RunScript(&pSpell->m_sScript, m_idSelf, 1);
            SetLastSpellId(0xffffffff);
        }
        delete pSpell;
        return;
    }
    if (nEventId == EVENT_OPEN_OBJECT) {
        if (pData) {
            OpenInventory(*(unsigned long *)pData, 0);
            delete pData;
        }
    }
}
