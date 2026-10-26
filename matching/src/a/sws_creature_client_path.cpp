// Client path cancellation from the server creature. Batch-local partial
// layouts preserve the existing creature and last-update base sizes.
struct Vector {
    Vector(float x1, float y1, float z1) { x = x1; y = y1; z = z1; }
    float x, y, z;
};
class CLastUpdateObject {
public:
    char pad_0[0x40];
    Vector m_vPathDestination;
    unsigned long m_nPathIndex;
    char pad_50[0x60 - 0x50];
    unsigned char m_nPathState;
};
class CSWSClient {
public:
    virtual void v00();
    unsigned long m_nPlayerId;
    char pad_8[4];
};
class CSWSPlayer : public CSWSClient {
public:
    CLastUpdateObject* GetLastUpdateObject(unsigned long oid); // 0x00561820
};
class CServerExoAppInternal {
public:
    CSWSClient* GetClientObjectByObjectID(unsigned long oid); // 0x004B2A50
};
class CServerExoApp {
public:
    char pad_0[4];
    CServerExoAppInternal* m_pInternal;
};
class CAppManager {
public:
    char pad_0[8];
    CServerExoApp* m_pServerExoApp;
};
extern CAppManager* g_pAppManager;                    // 0x007A39FC
class CSWSCreature {
public:
    char pad_0[4];
    unsigned long m_idSelf;
    void TerminateClientSidePath(int nStatus);
};
// NONMATCH: 5 diff: initial manager/receiver load order and register tie.
// FUNCTION: SWKOTOR 0x005110B0
void CSWSCreature::TerminateClientSidePath(int nStatus)
{
    CAppManager* pApp = g_pAppManager;
    CSWSPlayer* pPlayer = static_cast<CSWSPlayer*>(pApp->m_pServerExoApp->m_pInternal->GetClientObjectByObjectID(m_idSelf));
    if (pPlayer) {
        CLastUpdateObject* pUpdate = pPlayer->GetLastUpdateObject(m_idSelf);
        if (pUpdate) {
            pUpdate->m_vPathDestination = Vector(0.0f, 0.0f, 0.0f);
            pUpdate->m_nPathIndex = 0xffff;
            pUpdate->m_nPathState = (unsigned char)(nStatus - 1);
        }
    }
}
