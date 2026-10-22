// Pilot group 1: CSWSObject AI actions.

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);
    int SubtractWorldTimes(unsigned long nDay1, unsigned long nTime1,
                           unsigned long nDay2, unsigned long nTime2,
                           unsigned long* pDay, unsigned long* pTime);
};

class CServerExoApp {
public:
    CWorldTimer* GetWorldTimer();
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp* m_pServerExoApp;  // 0x8
    CServerExoApp* GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager* g_pAppManager;

extern unsigned long ACTION_IN_PROGRESS;
extern unsigned long ACTION_COMPLETE;
extern unsigned long ACTION_FAILED;

class CSWSCreature {
public:
    int GetDeadTemp();
};

struct CSWSObjectActionNode {
    char pad_0x0[0x38];
    float m_fWaitTime;  // 0x38 (first action parameter)
};

class CSWSObject {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CSWSCreature* AsSWSCreature();      // 0x30
    virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36();
    virtual int GetIsInConversation();           // 0x94

    int GetIsPaused();                           // 0x0063E7F0
    unsigned long AIActionWait(CSWSObjectActionNode* pNode);

    char pad_0x4[0xb8];
    unsigned long m_nActionStartDay;    // 0xbc
    unsigned long m_nActionStartTime;   // 0xc0
};

// FUNCTION: SWKOTOR 0x0057B5B0
unsigned long CSWSObject::AIActionWait(CSWSObjectActionNode* pNode)
{
    int bDead = 0;
    if (AsSWSCreature()) {
        bDead = AsSWSCreature()->GetDeadTemp();
    }
    if (GetIsInConversation() == 1 || GetIsPaused() == 1 || bDead) {
        return ACTION_FAILED;
    }

    unsigned long nCurrentDay, nCurrentTime;
    unsigned long nElapsedDay, nElapsedTime;
    float fWaitTime = pNode->m_fWaitTime;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nCurrentDay, &nCurrentTime);
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->SubtractWorldTimes(
        nCurrentDay, nCurrentTime, m_nActionStartDay, m_nActionStartTime,
        &nElapsedDay, &nElapsedTime);
    if (nElapsedTime < (unsigned long)(fWaitTime * 1000.0f)) {
        return ACTION_IN_PROGRESS;
    }
    return ACTION_COMPLETE;
}
