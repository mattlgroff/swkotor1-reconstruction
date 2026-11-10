// CSWSMessage::TestObjectVisible (round s13): whether an observer may get
// game-object updates for an object (perception range, visibility list,
// trap detection).
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/server_exo_app.h"
#include <stdlib.h>

extern const unsigned char OBJECT_TYPE_CREATURE;            // 0x00746634 (5)
extern const unsigned char OBJECT_TYPE_TRIGGER;             // 0x00746636 (7)
extern const unsigned char OBJECT_TYPE_WAYPOINT;            // 0x0074663B (12)
extern const unsigned char OBJECT_TYPE_STORE;               // 0x0074663D (14)

// Trap trigger (vtable slot 0x40, AsSWSTrigger).
class CSWSTrigger : public CSWSObject {
public:
    char pad_0xf4[0x2a8 - 0xf4];
    CExoArrayList<unsigned long> m_aDetectedBy;             // 0x2a8
    int m_b2B4;                                             // 0x2b4 (name unknown)
    int m_nTrapFaction;                                     // 0x2b8 (name guessed)
    int m_bTrapActive;                                      // 0x2bc
    char pad_0x2c0[0x2c8 - 0x2c0];
    int m_bFlagged;                                         // 0x2c8
    char pad_0x2cc[0x2fc - 0x2cc];
    int GetDetectedBy(unsigned long oid) { return m_aDetectedBy.count(oid); }
    unsigned char m_b2FC;                                   // 0x2fc (name unknown)
};

// Every trigger path ends with bResult = 1 (the trap tests do not change
// the outcome); kept as in the original.
// FUNCTION: SWKOTOR 0x0056DC60
int CSWSMessage::TestObjectVisible(CSWSObject* pObject, CSWSObject* pObserver)
{
    int bResult = 0;
    Vector vObject = pObject->m_vPosition;
    Vector vObserver = pObserver->m_vPosition;
    int bInArea = 1;
    if (!pObserver->GetArea() && pObserver->AsSWSCreature()) {
        bInArea = 0;
        vObserver = pObserver->AsSWSCreature()->m_vLastPosition;
    }
    float fDX = vObject.x - vObserver.x;
    float fDY = vObject.y - vObserver.y;
    float fRange = 250.0f;
    float fDistanceSq = fDX * fDX + fDY * fDY;
    if (pObserver->AsSWSCreature()) {
        fRange = (pObserver->AsSWSCreature()->GetSpotCheckDistance() >=
                  pObserver->AsSWSCreature()->GetListenCheckDistance())
                     ? pObserver->AsSWSCreature()->GetSpotCheckDistance()
                     : pObserver->AsSWSCreature()->GetListenCheckDistance();
    }
    if (pObject->m_nObjectType == OBJECT_TYPE_WAYPOINT || pObject->AsSWSEncounter() ||
        pObject->m_nObjectType == OBJECT_TYPE_STORE || pObject->AsSWSSoundObject()) {
        bResult = 0;
    }
    else if (pObject->m_nObjectType == OBJECT_TYPE_CREATURE) {
        pObject->GetArea();
        if (pObserver->AsSWSCreature()) {
            if (pObject->m_idSelf == pObserver->m_idSelf ||
                pObject->AsSWSCreature()->m_bInParty) {
                bResult = 1;
            }
            else {
                bResult = 0;
                if (!bInArea) {
                    if (fRange * fRange >= fDistanceSq) {
                        if (!pObject->AsSWSCreature()->GetInvisible(pObserver->AsSWSCreature())) {
                            bResult = 1;
                        }
                    }
                }
                else {
                    unsigned long oidObject = pObject->m_idSelf;
                    CSWSCreatureVisibility* pEntry =
                        pObserver->AsSWSCreature()->GetFromList920(oidObject);
                    if (pEntry && (pEntry->m_bSeen || pEntry->m_bHeard)) {
                        bResult = 1;
                    }
                }
            }
        }
    }
    else if (fRange * fRange >= fDistanceSq) {
        if (pObject->m_nObjectType != OBJECT_TYPE_TRIGGER) {
            bResult = 1;
        }
        else {
            g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(pObserver->m_idSelf);
            bResult = 1;
            if (pObject->AsSWSTrigger()->m_bTrapActive) {
                if (pObject->AsSWSTrigger()->m_bFlagged) {
                    bResult = 1;
                }
                else {
                    int nFaction = pObject->AsSWSTrigger()->m_nTrapFaction;
                    if (nFaction == pObserver->AsSWSCreature()->m_pStats->m_nFactionId ||
                        pObject->GetReputation(pObserver->GetId(), 0) >= 90) {
                        bResult = 1;
                    }
                    else {
                        unsigned long oidObserver = pObserver->m_idSelf;
                        if (pObject->AsSWSTrigger()->GetDetectedBy(oidObserver)) {
                            bResult = 1;
                        }
                    }
                }
            }
            else if (pObject->AsSWSTrigger()->m_b2B4 || pObject->AsSWSTrigger()->m_b2FC) {
                bResult = 1;
            }
        }
    }
    return bResult;
}
