// CSWCMessage::HandleServerToPlayerArea (nwchandlemessage.o): area visual
// effects, area flags, and the client side of an area transition.

#include <stdlib.h>
#include <math.h>

class CExoString {
public:
    char* m_sString;
    int m_nBufferLength;
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    char* CStr() const;                             // 0x005E5670
};

struct Vector {
    float x, y, z;
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};


class C2DA {
public:
    int GetCExoStringEntry(int nRow, const CExoString& sColumn, CExoString* pValue);  // 0x00413270
    char pad_0x0[0x24];
    int m_nNumRows;                                 // 0x24
};

class CTwoDimArrays {
public:
    char pad_0x0[0x18];
    C2DA* m_pLoadScreens;                           // 0x18 (descriptive)
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    CTwoDimArrays* m_p2DArrays;                     // 0xb8
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CAurCamera;
class CGuiAreaLoadscreen;

class CSWCArea {
public:
    int ApplyFireAndForgetVisualEffect(unsigned short nEffect, Vector vPosition);  // 0x00604C90
    void SetUnknown1f1(unsigned char n);            // 0x00605380
    void ClearUnknown1f1(int n);                    // 0x006053A0
    int LoadArea(CAurCamera* pCamera, CGuiAreaLoadscreen* pLoadScreen, int bArg);  // 0x00607610
    void UnloadArea();                              // 0x00609410
    char pad_0x0[0x104];
    unsigned long m_idSelf;                         // 0x104
};

class CSWCModule {
public:
    void Update(float fDelta);                      // 0x00644650
    char pad_0x0[0x40];
    CAurCamera* m_pCamera;                          // 0x40
    char pad_0x44[4];
    CSWCArea* m_pArea;                              // 0x48
};

class CSWCCreature {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84();
    virtual void SetOrientation(const Vector& vOrientation);    // 0x88
    virtual void SetPosition(const Vector& vPosition);  // 0x8c

    void SetWayPoints(unsigned short nCount, Vector* pPoints);  // 0x0060C790

    char pad_0x4[0x1dc - 0x4];
    Vector m_vLastPosition;                         // 0x1dc (descriptive)
};

class CGameObject;

class CGameObjectArray {
public:
    unsigned char Delete(unsigned long nId, CGameObject** ppObject);   // 0x004D81A0
};

class CWorldTimer {
public:
    unsigned __int64 GetSnapshotTimeDifference();   // 0x004ADC80
};

class CClientExoAppInternal {
public:
    CGuiAreaLoadscreen* ShowLoadScreen(int nType, char* sImage, int bShow);  // 0x0066B360 (descriptive)
};

class CClientExoApp {
public:
    CWorldTimer* GetWorldTimer();                   // 0x005ED4F0
    CSWCModule* GetModule();                        // 0x005ED530
    CSWCCreature* GetPlayerCreature();              // 0x005ED540
    CGameObjectArray* GetGameObjectArray();         // 0x005ED570
    void EmptyHitcheckIgnoreList();                 // 0x005ED770

    void* m_p00;
    CClientExoAppInternal* m_pInternal;             // 0x04
};

class CAppManager {
public:
    void* m_p00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CExoTimers {
public:
    unsigned long GetHighResolutionTimer();         // 0x005E9260
};

class CExoBase {
public:
    void* m_p00;
    CExoTimers* m_pTimers;                          // 0x04
};

extern CExoBase* g_pExoBase;                        // 0x007A39E0

void SetClearColorFromString(const char* sColor);   // 0x00401690
void RunFrame(float fDelta, int a, int b);          // 0x00401C10

class CSWCMessage {
public:
    void SetReadPositionToEnd();                    // 0x004D6170
    int MessageReadOverflow(int bError);            // 0x004D6230
    int ReadBOOL();                                 // 0x004D66F0
    unsigned char ReadBYTE(int nBits);              // 0x004D6720
    unsigned short ReadWORD(int nBits);             // 0x004D6750
    float ReadFLOAT(float fScale, int nBits);       // 0x004D67B0
    CExoString ReadCExoString(int nBits);           // 0x004D67E0
    int HandleServerToPlayerArea(unsigned char nMinor);
};

// FUNCTION: SWKOTOR 0x0064BAB0
// NONMATCH: the original has an 8-aligned EBP frame (sub esp,0x58) with an
// epilogue per return; ours gets a plain frame. Statement order and callees
// follow the original; the frame-alignment trigger is not yet known (moving
// the image string before the module checks aligns it but breaks order).
int CSWCMessage::HandleServerToPlayerArea(unsigned char nMinor)
{
    if (MessageReadOverflow(1)) {
        return 0;
    }
    switch (nMinor) {
    case 1: {
        CSWCModule* pModule = g_pAppManager->m_pClientExoApp->GetModule();
        if (!pModule) {
            SetReadPositionToEnd();
            return 0;
        }
        CSWCArea* pArea = pModule->m_pArea;
        CAurCamera* pCamera = pModule->m_pCamera;
        if (!pArea || !pCamera) {
            SetReadPositionToEnd();
            return 0;
        }
        CGameObject* pRemoved;
        g_pAppManager->m_pClientExoApp->GetGameObjectArray()->Delete(pArea->m_idSelf, &pRemoved);
        CExoString sImage;
        unsigned char nLoadScreen = ReadBYTE(8);
        if (nLoadScreen == 1) {
            sImage = ReadCExoString(32);
        }
        CGuiAreaLoadscreen* pLoadScreen;
        {
            CExoString sResRef;
            if (!nLoadScreen) {
                nLoadScreen = rand() % (g_pRules->m_p2DArrays->m_pLoadScreens->m_nNumRows - 2) + 2;
            }
            if (nLoadScreen != 1) {
                CExoString sColumn("BMPResRef");
                g_pRules->m_p2DArrays->m_pLoadScreens->GetCExoStringEntry(nLoadScreen, sColumn, &sResRef);
            }
            pLoadScreen = g_pAppManager->m_pClientExoApp->m_pInternal->ShowLoadScreen(
                2, (nLoadScreen == 1 ? sImage : sResRef).CStr(), 1);
            g_pAppManager->m_pClientExoApp->GetModule()->Update(1e-8f);
            SetClearColorFromString("0 0 0");
            g_pExoBase->m_pTimers->GetHighResolutionTimer();
            g_pAppManager->m_pClientExoApp->GetWorldTimer()->GetSnapshotTimeDifference();
        }
        g_pAppManager->m_pClientExoApp->EmptyHitcheckIgnoreList();
        RunFrame(0.05f, 0, 0);
        pArea->UnloadArea();
        float fX = ReadFLOAT(1.0f, 0x20);
        float fY = ReadFLOAT(1.0f, 0x20);
        float fZ = ReadFLOAT(1.0f, 0x20);
        float fFacing = ReadFLOAT(1.0f, 0x20);
        ReadBOOL();
        if (MessageReadOverflow(1) || !pArea->LoadArea(pCamera, pLoadScreen, 1)) {
            SetReadPositionToEnd();
            return 0;
        }
        CSWCCreature* pPlayer = g_pAppManager->m_pClientExoApp->GetPlayerCreature();
        if (pPlayer) {
            Vector vPosition(fX, fY, fZ);
            pPlayer->SetPosition(vPosition);
            pPlayer->m_vLastPosition = vPosition;
            pPlayer->SetOrientation(Vector(cos(fFacing), sin(fFacing), 0.0f));
            pPlayer->SetWayPoints(0, 0);
        }
        return 1;
    }
    case 2: {
        unsigned short nEffect = ReadWORD(16);
        float fX = ReadFLOAT(1.0f, 0x20);
        float fY = ReadFLOAT(1.0f, 0x20);
        float fZ = ReadFLOAT(1.0f, 0x20);
        CSWCModule* pModule = g_pAppManager->m_pClientExoApp->GetModule();
        if (!pModule) {
            return 0;
        }
        pModule->m_pArea->ApplyFireAndForgetVisualEffect(nEffect, Vector(fX, fY, fZ));
        return 1;
    }
    case 4: {
        unsigned char nFlag = ReadBYTE(8);
        int bSet = ReadBOOL();
        CSWCModule* pModule = g_pAppManager->m_pClientExoApp->GetModule();
        if (!pModule || !pModule->m_pArea) {
            return 0;
        }
        if (bSet) {
            pModule->m_pArea->SetUnknown1f1(nFlag);
        }
        else {
            pModule->m_pArea->ClearUnknown1f1(nFlag);
        }
        return 1;
    }
    }
    return 0;
}
