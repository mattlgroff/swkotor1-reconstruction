// Heartbeat updates of the server door, trigger and area-of-effect objects
// (vtable +0x70), plus small virtual members found next to them. None of
// these function starts is in functions.csv: each follows the previous
// function's padding.

class CExoString {
public:
    CExoString();
    ~CExoString();
    char *m_sString;
    unsigned long m_nBufferLength;
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
    int count(T t);
};

class CExoTimers {
public:
    unsigned __int64 GetHighResolutionTimer();
};

class CExoBase {
public:
    char pad_0x0[0x4];
    CExoTimers *m_pcExoTimers;       // +0x4
};

extern CExoBase *g_pExoBase;

class CWorldTimer {
public:
    void GetWorldTime(unsigned long *pDay, unsigned long *pTime);
    void SubtractWorldTimes(unsigned long nDay1, unsigned long nTime1, unsigned long nDay2,
                            unsigned long nTime2, unsigned long *pDayDiff,
                            unsigned long *pTimeDiff);
};

class CVirtualMachine {
public:
    int RunScript(CExoString *psFileName, unsigned long oid, int bFlag);
    int RunScriptSituation(void *pScript, unsigned long oid, int bFlag);
};

extern CVirtualMachine *g_pVirtualMachine;

class CGameObject;
class CSWSArea;
class CSWSCreature;

class CGameObjectArray {
public:
    char GetGameObject(unsigned long id, CGameObject **ppObject);
};

class CServerAIMaster {
public:
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long oidCaller,
                          unsigned long oidObject, unsigned long nEvent, void *pData);
};

class CServerExoApp {
public:
    CWorldTimer *GetWorldTimer();
    CSWSCreature *GetCreatureByGameObjectID(unsigned long id);
    unsigned long GetPlayerCreatureId();
    CGameObjectArray *GetObjectArray();
    CServerAIMaster *GetServerAIMaster();
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;
extern const unsigned long OBJECT_INVALID;         // 0x00746644
extern char g_cObjectArraySuccess;                 // 0x0074666C
extern const int EVENT_AI_LATER;                   // 0x007441E0
extern const unsigned long EVENT_SIGNAL_EVENT;     // 0x007441DC
extern const unsigned long EVENT_TIMED_EVENT;      // 0x007441B8

class CScriptEvent {
public:
    ~CScriptEvent();
    unsigned long GetObjectID(int nIndex);
    unsigned short m_nType;
};

class CGameObject {
public:
    virtual ~CGameObject();
    virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void AIUpdate();                   // +0x70
    virtual void v74(); virtual void v78();
    virtual void SetAnimation(int nAnimation); // +0x7c
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90();
    virtual int GetDead();                     // +0x94

    unsigned long GetID() { return m_idSelf; }
    unsigned long m_idSelf;          // +0x4
    unsigned char m_nObjectType;     // +0x8
    char pad_0x9[0x3];
};

class CSWSObject : public CGameObject {
public:
    CSWSArea *GetArea();
    int GetDead();
    void DamageHitPoints(int nDamage);
    void SetPosition(const Vector &v, int a, int b, int c);
    int GetReputation(unsigned long oid, int bFlag);
    void UpdateEffects(unsigned long nCalendarDay, unsigned long nTimeOfDay);
    void UpdateActions(unsigned long nCalendarDay, unsigned long nTimeOfDay,
                       unsigned __int64 nTimer);

    char pad_0xc[0x84];
    Vector m_vPosition;              // +0x90
    char pad_0x9c[0xc];
    unsigned long m_nLastUpdateDay;  // +0xa8
    unsigned long m_nLastUpdateTime; // +0xac
    char pad_0xb0[0x1c];
    unsigned long m_nUpdateDelta;    // +0xcc
    char pad_0xd0[0x4];
    int m_nAnimation;                // +0xd4
    char pad_0xd8[0x14];
    int m_bEC;                       // +0xec
    char pad_0xf0[0x8];
    int m_bPlot;                     // +0xf8
    char pad_0xfc[0x12c];
};

class CSWSCreatureStats {
public:
    char pad_0x0[0x78];
    int m_nFactionId;                // +0x78
};

class CSWSCreature : public CSWSObject {
public:
    char pad_0x228[0x84c];
    CSWSCreatureStats *m_pStats;     // +0xa74
};

// ---------------------------------------------------------------------------

class CSWSDoor : public CSWSObject {
public:
    void AIUpdate();
    int GetDead();
    void DamageHitPoints(int nDamage);

    CExoString m_sScripts[15];       // +0x228
    char pad_0x2a0[0x6c];
    unsigned long m_nLastHeartbeatDay;  // +0x30c
    unsigned long m_nLastHeartbeatTime; // +0x310
};

// FUNCTION: SWKOTOR 0x005889C0
void CSWSDoor::AIUpdate()
{
    unsigned __int64 nTimer = g_pExoBase->m_pcExoTimers->GetHighResolutionTimer();
    unsigned long nTimeOfDay, nCalendarDay;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nCalendarDay, &nTimeOfDay);
    unsigned long nDayDiff, nTimeDiff;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->SubtractWorldTimes(
        nCalendarDay, nTimeOfDay, m_nLastHeartbeatDay, m_nLastHeartbeatTime, &nDayDiff, &nTimeDiff);
    if (nTimeDiff >= 6000) {
        if (m_nLastHeartbeatTime && !GetDead())
            g_pVirtualMachine->RunScript(&m_sScripts[5], m_idSelf, 1);
        m_nLastHeartbeatDay = nCalendarDay;
        m_nLastHeartbeatTime = nTimeOfDay;
    }
    UpdateEffects(nCalendarDay, nTimeOfDay);
    UpdateActions(nCalendarDay, nTimeOfDay, nTimer);
}

// Shared by the door and placeable vtables (+0x94); plot objects never die.
// FUNCTION: SWKOTOR 0x00588AB0
int CSWSDoor::GetDead()
{
    if (m_bPlot)
        return 0;
    return CSWSObject::GetDead();
}

// Also shared by the door and placeable vtables (+0xa0).
// FUNCTION: SWKOTOR 0x00589190
void CSWSDoor::DamageHitPoints(int nDamage)
{
    if (!m_bPlot)
        CSWSObject::DamageHitPoints(nDamage);
}

// ---------------------------------------------------------------------------

class CSWSTrigger : public CSWSObject {
public:
    void AIUpdate();

    char pad_0x228[0x1c];
    CExoString m_sScripts[7];        // +0x244
    char pad_0x27c[0x2c];
    CExoArrayList<unsigned long> m_aObjectsInside; // +0x2a8
    int m_bIsTransition;             // +0x2b4
    int m_nFactionId;                // +0x2b8
    int m_bIsTrap;                   // +0x2bc
    int m_bTrapDetectable;           // +0x2c0
    int m_bTrapDisarmable;           // +0x2c4
    int m_bTrapFlag;                 // +0x2c8
    char pad_0x2cc[0x34];
    unsigned long m_nLastHeartbeatTime; // +0x300
    unsigned long m_nLastHeartbeatDay;  // +0x304
};

// FUNCTION: SWKOTOR 0x0058D760
void CSWSTrigger::AIUpdate()
{
    unsigned __int64 nTimer = g_pExoBase->m_pcExoTimers->GetHighResolutionTimer();
    unsigned long nTimeOfDay, nCalendarDay;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nCalendarDay, &nTimeOfDay);
    unsigned long nDayDiff, nTimeDiff;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->SubtractWorldTimes(
        nCalendarDay, nTimeOfDay, m_nLastHeartbeatDay, m_nLastHeartbeatTime, &nDayDiff, &nTimeDiff);
    if (nTimeDiff >= 6000) {
        g_pVirtualMachine->RunScript(&m_sScripts[0], m_idSelf, 1);
        m_nLastHeartbeatDay = nCalendarDay;
        m_nLastHeartbeatTime = nTimeOfDay;
    }
    if (m_bIsTrap) {
        CServerExoApp *pServer = g_pAppManager->m_pServerExoApp;
        CSWSCreature *pPC = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
        if (!m_bTrapFlag &&
            (!pPC || (m_nFactionId != pPC->m_pStats->m_nFactionId &&
                      GetReputation(pPC->GetID(), 0) < 90 &&
                      !m_aObjectsInside.count(pPC->m_idSelf)))) {
            if (m_nAnimation != 10143)
                SetAnimation(10143);
        } else {
            if (m_nAnimation != 10144)
                SetAnimation(10144);
        }
    }
    UpdateActions(nCalendarDay, nTimeOfDay, nTimer);
}

// ---------------------------------------------------------------------------

class CSWSAreaOfEffectObject : public CSWSObject {
public:
    void AIUpdate();
    int GetLastSpellId();
    void SetLastSpellId(int nSpellId);
    void UpdateSubAreas(Vector *pvPrevious);
    void RemoveFromArea();
    void EventHandler(unsigned long nEventId, unsigned long oidCaller, void *pScript,
                      unsigned long nCalendarDay, unsigned long nTimeOfDay);

    char pad_0x228[0x4];
    int m_nAreaEffectId;                // 0x22c
    unsigned char m_nShape;             // 0x230
    int m_nSpellId;                     // 0x234
    char pad_0x238[0x14];
    unsigned long m_oidLinkedTo;        // 0x24c
    unsigned long m_oidLastEntered;     // 0x250
    unsigned long m_oidLastLeft;        // 0x254
    char pad_0x258[0x8];
    CExoString m_sScriptHeartbeat;      // 0x260
    CExoString m_sScriptUserDefined;    // 0x268
    CExoString m_sScriptOnEnter;        // 0x270
    CExoString m_sScriptOnExit;         // 0x278
    unsigned long m_nLastHeartbeatDay;  // 0x280
    unsigned long m_nLastHeartbeatTime; // 0x284
    unsigned long m_nDuration;          // 0x288
    unsigned char m_nDurationType;      // 0x28c
};

// FUNCTION: SWKOTOR 0x005945D0
int CSWSAreaOfEffectObject::GetLastSpellId()
{
    return m_nSpellId;
}

// FUNCTION: SWKOTOR 0x005945E0
void CSWSAreaOfEffectObject::SetLastSpellId(int nSpellId)
{
    m_nSpellId = nSpellId;
}

// NONMATCH (12): only the three stores after the second SubtractWorldTimes are
// scheduled differently (the original loads delta, day, time in source order);
// statement order, inline setters and local declaration order do not move them.
// FUNCTION: SWKOTOR 0x00595D10
void CSWSAreaOfEffectObject::AIUpdate()
{
    unsigned __int64 nTimer = g_pExoBase->m_pcExoTimers->GetHighResolutionTimer();
    unsigned long nTimeOfDay, nCalendarDay;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nCalendarDay, &nTimeOfDay);
    unsigned long nDayDiff, nTimeDiff;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->SubtractWorldTimes(
        nCalendarDay, nTimeOfDay, m_nLastUpdateDay, m_nLastUpdateTime, &nDayDiff, &nTimeDiff);
    unsigned long nHeartbeatDayDiff, nHeartbeatTimeDiff;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->SubtractWorldTimes(
        nCalendarDay, nTimeOfDay, m_nLastHeartbeatDay, m_nLastHeartbeatTime,
        &nHeartbeatDayDiff, &nHeartbeatTimeDiff);
    m_nUpdateDelta = nTimeDiff;
    m_nLastUpdateDay = nCalendarDay;
    m_nLastUpdateTime = nTimeOfDay;
    if (nHeartbeatTimeDiff >= 6000) {
        if (m_nLastHeartbeatTime)
            g_pVirtualMachine->RunScript(&m_sScriptHeartbeat, m_idSelf, 1);
        m_nLastHeartbeatTime = nTimeOfDay;
        m_nLastHeartbeatDay = nCalendarDay;
    }
    if (m_oidLinkedTo != OBJECT_INVALID && m_nShape == 0) {
        CGameObject *pObject;
        if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidLinkedTo, &pObject) ==
                g_cObjectArraySuccess && pObject) {
            Vector vPosition = ((CSWSObject *)pObject)->m_vPosition;
            if (GetArea())
                UpdateSubAreas(&vPosition);
            SetPosition(vPosition, 1, 1, 0);
        } else {
            unsigned long oid = m_idSelf;
            g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                0, 0, oid, oid, EVENT_AI_LATER, 0);
        }
    }
    if (m_nDurationType == 1) {
        if (m_nDuration <= nTimeDiff) {
            g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                0, 0, m_idSelf, m_idSelf, EVENT_AI_LATER, 0);
            return;
        }
        m_nDuration -= nTimeDiff;
    }
    UpdateActions(nCalendarDay, nTimeOfDay, nTimer);
}

// FUNCTION: SWKOTOR 0x005964E0
void CSWSAreaOfEffectObject::EventHandler(unsigned long nEventId, unsigned long oidCaller,
                                          void *pScript, unsigned long nCalendarDay,
                                          unsigned long nTimeOfDay)
{
    if (nEventId == EVENT_TIMED_EVENT) {
        if (pScript)
            g_pVirtualMachine->RunScriptSituation(pScript, m_idSelf, 1);
    } else if (nEventId == EVENT_SIGNAL_EVENT) {
        CScriptEvent *pEvent = (CScriptEvent *)pScript;
        unsigned short nType = pEvent->m_nType;
        if (nType == 0xc) {
            m_oidLastEntered = pEvent->GetObjectID(0);
            g_pVirtualMachine->RunScript(&m_sScriptOnEnter, m_idSelf, 1);
        } else if (nType == 0xd) {
            m_oidLastLeft = pEvent->GetObjectID(0);
            g_pVirtualMachine->RunScript(&m_sScriptOnExit, m_idSelf, 1);
        }
        delete pEvent;
    } else if (nEventId == EVENT_AI_LATER) {
        if (m_bEC) {
            RemoveFromArea();
            delete this;
        }
    }
}
