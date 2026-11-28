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

#include "shared/app_manager.h"

#include "shared/globals.h"


class CSWSCreature {
public:
    int GetDeadTemp();
};

struct CSWSObjectActionNode {
    char pad_0x0[0x38];
    float m_fWaitTime;  // 0x38 (first action parameter)
};

#include "shared/game_object.h"

class CSWSObject : public CGameObject {
public:
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36();
    virtual int GetIsInConversation();           // 0x94

    int GetIsPaused();                           // 0x0063E7F0
    unsigned long AIActionWait(CSWSObjectActionNode* pNode);

    char pad_0xc[0xb0];
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
