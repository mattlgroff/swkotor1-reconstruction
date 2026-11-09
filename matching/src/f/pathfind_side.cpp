// CAvoidCreature::SideClearOfObstructions (round s10, 0x005CF8D0): direct-line
// walk test between two points of the avoidance hexagon. Own minimal layouts;
// several names are placeholders.

class Vector {
public:
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    float x, y, z;
};

class CWalkHitInfo {
public:
    CWalkHitInfo()
    {
        m_n0 = 0;
        m_n44 = 0;
        m_oidBlocking = 0x7f000000;
    }
    int m_n0;                        // +0x0
    Vector m_v04;                    // +0x4 (placeholder names)
    Vector m_v10;                    // +0x10
    Vector m_v1C;                    // +0x1c
    Vector m_v28;                    // +0x28
    Vector m_v34;                    // +0x34
    unsigned long m_oidBlocking;     // +0x40
    int m_n44;                       // +0x44
};

class CResRef {
public:
    char m_resRef[16];
};

class CSWAreaHead {
public:
    char pad_0x0[0x100];
};

class CSWAreaRes {
public:
    char pad_0x0[0xc];
    CResRef m_cResRef;               // +0xc (placeholder name)
    CResRef *GetResRef() { return &m_cResRef; }
};

class CSWSArea : public CSWAreaHead, public CSWAreaRes {
};

class CSWSModule {
public:
    CSWSArea *GetArea();
};

class CPathfindInformation {
public:
    char pad_0x0[0x4];
    float m_fPersonalSpace;          // +0x4 (placeholder name)
    char pad_0x8[0xc];
    float m_fHeight;                 // +0x14 (placeholder name)
};

class CSWSCreature {
public:
    char pad_0x0[0x340];
    CPathfindInformation *m_pPathfindInformation;  // +0x340
};

class CServerExoApp {
public:
    CSWSCreature *GetCreatureByGameObjectID(unsigned long oid);
    CSWSModule *GetModule();
    float ComputeHeight(CResRef *pArea, Vector *pvPosition, int bFlag);
    int TestDirectLine(unsigned long oid, CResRef *pArea, const Vector *pvFrom,
                       const Vector *pvTo, float *pfPersonalSpace, float fHeight, int bFlag,
                       CWalkHitInfo *pHit);
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;
extern int g_bPathfindDebug;         // 0x00832880 (placeholder name)

class CAvoidCreature {
public:
    int SideClearOfObstructions(unsigned long oidCreature, const Vector *pvFrom,
                                const Vector *pvTo, unsigned long *poidBlocking);
};

// FUNCTION: SWKOTOR 0x005CF8D0
int CAvoidCreature::SideClearOfObstructions(unsigned long oidCreature, const Vector *pvFrom,
                                            const Vector *pvTo, unsigned long *poidBlocking)
{
    *poidBlocking = 0x7f000000;
    CWalkHitInfo cHit;
    CSWSCreature *pCreature = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(oidCreature);
    int nResult = -4;
    if (pCreature) {
        float fPersonalSpace = pCreature->m_pPathfindInformation->m_fPersonalSpace;
        float fHeight = pCreature->m_pPathfindInformation->m_fHeight;
        nResult = g_pAppManager->GetServerExoApp()->TestDirectLine(
            oidCreature, g_pAppManager->m_pServerExoApp->GetModule()->GetArea()->GetResRef(),
            pvFrom, pvTo, &fPersonalSpace, fHeight, 1, &cHit);
    }
    if (g_bPathfindDebug) {
        Vector vFrom = *pvFrom;
        Vector vTo = *pvTo;
        vFrom.z = g_pAppManager->GetServerExoApp()->ComputeHeight(
                      g_pAppManager->m_pServerExoApp->GetModule()->GetArea()->GetResRef(), &vFrom, 0)
                  + 0.1f;
        g_pAppManager->m_pServerExoApp->ComputeHeight(
            g_pAppManager->m_pServerExoApp->GetModule()->GetArea()->GetResRef(), &vTo, 0);
    }
    switch (nResult) {
    case 1:
        return 1;
    case -3:
        *poidBlocking = cHit.m_oidBlocking;
        return 0;
    case 0:
    case -1:
    case -2:
        return 0;
    }
    return 0;
}
