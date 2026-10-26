// Scene line hit check that first clips the line against the server area's
// room walkmeshes (worker C s5). Descriptive name; the playable source calls
// it SceneHitCheck. 0x0060F1E0 is a real start missing from functions.csv.

struct Vector {
    float x, y, z;
};

class CSWCollisionResult {
public:
    CSWCollisionResult(unsigned long nMask);        // 0x00581320
    void ClearOutput();                             // 0x00580750

    unsigned long m_nMask;                          // 0x00
    char pad_0x4[0x4];
    int m_bHit;                                     // 0x08
    int m_nFace;                                    // 0x0c
    Vector m_vHit;                                  // 0x10
    char pad_0x1c[0x4];
};

class CSWSRoom {
public:
    int CheckAABBAll(Vector vStart, Vector vEnd, CSWCollisionResult* pResult);   // 0x00579610
};

class CSWSArea {
public:
    CSWSRoom* GetRoomByIndex(int nIndex);           // 0x004BB700
};

class CSWSModule {
public:
    CSWSArea* GetArea();                            // 0x004C30F0
};

class CServerExoApp {
public:
    CSWSModule* GetModule();                        // 0x004AE6B0
};

class CAppManager {
public:
    void* m_pUnknown00;
    void* m_pClientExoApp;                          // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC
extern int g_bHitWalkmesh;                          // 0x0078E5A8

class Scene {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34();
    virtual int HitCheck(Vector vStart, Vector vEnd, int nFlags, void* pHitInfo);   // 0x38
};

// FUNCTION: SWKOTOR 0x0060F1E0
int SceneHitCheck(Scene* pScene, Vector vStart, Vector vEnd, int nFlags, void* pHitInfo)
{
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWCollisionResult cResult(0xFFFFFFFF);
    int bHit = 0;
    if (pServer) {
        CSWSModule* pModule = pServer->GetModule();
        if (pModule) {
            CSWSArea* pArea = pModule->GetArea();
            if (pArea) {
                for (int i = 0; pArea->GetRoomByIndex(i); i++) {
                    cResult.ClearOutput();
                    if (pArea->GetRoomByIndex(i)->CheckAABBAll(vStart, vEnd, &cResult)) {
                        vEnd = cResult.m_vHit;
                        bHit = 1;
                    }
                }
            }
        }
    }
    int bSaved = g_bHitWalkmesh;
    g_bHitWalkmesh = 0;
    int nResult = pScene->HitCheck(vStart, vEnd, 0, pHitInfo);
    g_bHitWalkmesh = bSaved;
    if (nResult) {
        return nResult;
    }
    return bHit;
}
