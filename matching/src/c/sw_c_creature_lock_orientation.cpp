#include "c/client_exo_app.h"

class CSWSCreature : public CGameObject {
public:
    virtual void v38(); virtual void v3c(); virtual void v40(); virtual void v44();
    virtual void v48(); virtual void v4c(); virtual void v50(); virtual void v54();
    virtual void v58(); virtual void v5c(); virtual void v60(); virtual void v64();
    virtual void v68(); virtual void v6c(); virtual void v70(); virtual void v74();
    virtual void v78(); virtual void v7c(); virtual void v80(); virtual void v84();
    virtual void v88(); virtual void v8c(); virtual void v90();
    virtual int v94();                              // name unresolved; vtable slot 0x94
    int GetDeadTemp();                              // 0x004EF890
};

// FUNCTION: SWKOTOR 0x00610950
void CSWCCreature::SetLockOrientation(unsigned long oid, int bForce)
{
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    if (pServer == 0) return;
    unsigned long nServerId = pServer->ClientToServerObjectID(m_idSelf);
    CSWSCreature* pServerCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(nServerId);
    if (pServerCreature == 0) return;
    if (!bForce) {
        if (pServerCreature->v94()) return;
        if (pServerCreature->GetDeadTemp()) return;
    }
    m_oidLockOrientation = oid;
}
