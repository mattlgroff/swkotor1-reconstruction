#include "c/client_exo_app.h"

struct CSWGuiInterfaceAction {
    char pad_0x0[0x0c];
    unsigned long m_pfnAction;                                      // 0x0c
    unsigned long m_nUnknown10;                                      // 0x10
    unsigned long m_nUnknown14;                                      // 0x14
    unsigned long m_nUnknown18;                                      // 0x18
    unsigned long m_oidCreature;                                     // 0x1c
    char pad_0x20[0x18];
};

class CSWSCreature {
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
    virtual void v90(); virtual void v94(); virtual void v98();
    virtual int GetCurrentHitPoints(int nType);                       // 0x9c
    char pad_0x4[0xa70];
    void* m_pStats;                                                   // 0xa74
    char pad_0xa78[0x24];
    unsigned long m_bUseCurrentHitPoints;                             // 0xa9c
};

extern "C" int __fastcall CExoCreatureStats_GetEquippedMask(void*, void*); // 0x004EFA80
extern "C" void __fastcall CSWCCreatureStats_AppendForceActions(
    void*, void*, CExoArrayList<CSWGuiInterfaceAction>*, int, int, int, int, float); // 0x0064AF10

// FUNCTION: SWKOTOR 0x00616230
void CSWCCreature::GetSelfForcePowers(CExoArrayList<CSWGuiInterfaceAction>& actions)
{
    unsigned long nEquippedMask = 0;
    int nHitPoints = 0;
    CGameObject* pServerObject = GetServerObject();
    if (pServerObject != 0) {
        pServerObject = GetServerObject();
        CSWSCreature* pServerCreature = pServerObject->AsSWSCreature();
        if (pServerCreature != 0) {
            nEquippedMask = CExoCreatureStats_GetEquippedMask(pServerCreature, 0);
            if (pServerCreature->m_bUseCurrentHitPoints != 0) {
                nHitPoints = (short)pServerCreature->GetCurrentHitPoints(0);
            } else {
                unsigned short nVitality = *(unsigned short*)((char*)pServerCreature->m_pStats + 0x124);
                unsigned short nCurrent = *(unsigned short*)((char*)pServerCreature->m_pStats + 0x126);
                nHitPoints = (short)(nCurrent + nVitality);
            }
        }
    }

    int nIndex = actions.num;
    CSWCCreatureStats_AppendForceActions(*(void**)((char*)this + 0x2f8), 0,
        &actions, 3, nEquippedMask, 0, nHitPoints, 0.0001f);
    while (nIndex < actions.num) {
        CSWGuiInterfaceAction* pAction = &actions.element[nIndex];
        pAction->m_pfnAction = 0x0060F330;
        pAction->m_nUnknown10 = 0;
        pAction->m_nUnknown14 = 0;
        pAction->m_nUnknown18 = 0;
        pAction->m_oidCreature = m_idSelf;
        ++nIndex;
    }
}
