// CSWCObject forwards action clearing to its corresponding server object.
class CServerExoApp {
public:
    unsigned long ClientToServerObjectID(unsigned long nClientObject); // 0x004AEA30
    void ClearAllActions(unsigned long nServerObject);                 // 0x004AEF40
    void ClearAllQueuedCombatActions(unsigned long nServerObject);     // 0x004AEFA0
};

class CAppManager {
public:
    char pad_0x0[8];
    CServerExoApp* m_pServerExoApp;                  // +0x08
};

class CSWCObject {
public:
    void* m_pVtable;
    unsigned long m_oidSelf;                         // +0x04

    void ClearAllActions();
    void ClearAllQueuedCombatActions();
};

// FUNCTION: SWKOTOR 0x0063D470
void CSWCObject::ClearAllActions()
{
    CAppManager* pAppManager = *(CAppManager**)0x007A39FC;
    CServerExoApp* pServer = pAppManager->m_pServerExoApp;
    unsigned long nServerObject = pServer->ClientToServerObjectID(m_oidSelf);
    pServer->ClearAllActions(nServerObject);
}

// FUNCTION: SWKOTOR 0x0063D490
void CSWCObject::ClearAllQueuedCombatActions()
{
    CAppManager* pAppManager = *(CAppManager**)0x007A39FC;
    CServerExoApp* pServer = pAppManager->m_pServerExoApp;
    unsigned long nServerObject = pServer->ClientToServerObjectID(m_oidSelf);
    pServer->ClearAllQueuedCombatActions(nServerObject);
}
