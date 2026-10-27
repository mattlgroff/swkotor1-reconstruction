// CSWCArea::AddPlaceableObjectStatic / AddPlaceableObjectLight, worker C s8
// (translation-unit-local layout; nwcarea.o).

#include "c/exo_base.h"

struct Vector {
    float x, y, z;
};

struct Quaternion {
    float w, x, y, z;
};

float GetFacing(const Vector& v);                   // 0x004AA0F0
Quaternion EulerToQuaternion(float fYaw, float fPitch, float fRoll);  // 0x004ACAC0

class C2DA {
public:
    int GetCExoStringEntry(int nRow, const CExoString& sColumn, CExoString* pValue);  // 0x00413270
};

class CTwoDimArrays {
public:
    char pad_0x0[0x2c];
    C2DA* m_pPlaceables;                            // 0x2c
    C2DA* GetPlaceables() { return m_pPlaceables; }
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    CTwoDimArrays* m_p2DArrays;                     // 0xb8
    CTwoDimArrays* Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CSWCArea;

class CSWCObject {
public:
    virtual ~CSWCObject();
    virtual void SetObjectID(unsigned long nId);    // 0x04
    virtual void v08(); virtual void v0C(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1C(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2C(); virtual void v30(); virtual void v34();
    virtual void v38(); virtual void v3C(); virtual void v40(); virtual void v44();
    virtual void v48(); virtual void v4C(); virtual void v50(); virtual void v54();
    virtual void v58(); virtual void v5C(); virtual void v60(); virtual void v64();
    virtual void v68(); virtual void v6C(); virtual void v70(); virtual void v74();
    virtual void v78(); virtual void v7C(); virtual void v80();
    virtual void SetArea(CSWCArea* pArea);          // 0x84
    virtual void SetOrientation(Vector* pOrientation);  // 0x88
    virtual void SetPosition(Vector* pPosition);    // 0x8c
    virtual void v90(); virtual void v94(); virtual void v98();
    virtual void SetAnimation(unsigned short nAnimation, int a, int b);   // 0x9c

    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0x104 - 0x8];
};

class CSWCPlaceable : public CSWCObject {
public:
    CSWCPlaceable();                                // 0x00682270
    void SetAppearance(unsigned char nAppearance);  // 0x00682E20

    char pad_0x104[0x110 - 0x104];
    int m_bUseable;                                 // 0x110 (descriptive)
    int m_n114;                                     // 0x114
    int m_n118;                                     // 0x118
    char pad_0x11c[4];
    int m_n120;                                     // 0x120
    int m_n124;                                     // 0x124
    int m_bHitCheck;                                // 0x128 (descriptive)
    int m_n12C;                                     // 0x12c
    int m_n130;                                     // 0x130
    char pad_0x134[0x150 - 0x134];
};

class CGameObjectArray {
public:
    unsigned char AddExternalObject(unsigned long* pId, CSWCObject* pObject, int bClient);  // 0x004D7FF0
};

struct CClientOptions {
    char pad_0x0[0x72];
    unsigned char m_bGore;                          // 0x72 (descriptive)
};

class CClientExoApp {
public:
    CGameObjectArray* GetGameObjectArray();         // 0x005ED570
    CSWCPlaceable* GetPlaceableByGameObjectID(unsigned long nId);   // 0x005ED5F0
    CClientOptions* GetClientOptions();             // 0x005ED700
    void RemoveObjectFromHitcheckIgnoreList(unsigned long nId);  // 0x005ED760
};

class CAppManager {
public:
    void* m_p00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
    void* m_pServerExoApp;                          // 0x08
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC
extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C

class CSWCArea {
public:
    int AddPlaceableObjectStatic(unsigned long nId, unsigned char nAppearance, Vector vPosition, Vector vOrientation);

    char pad_0x0[0x224];
    CExoArrayList<unsigned long> m_aStaticPlaceables;   // 0x224
};

// FUNCTION: SWKOTOR 0x006072D0
int CSWCArea::AddPlaceableObjectStatic(unsigned long nId, unsigned char nAppearance, Vector vPosition,
                                       Vector vOrientation)
{
    CGameObjectArray* pObjects = g_pAppManager->GetClientExoApp()->GetGameObjectArray();
    CSWCPlaceable* pPlaceable = g_pAppManager->GetClientExoApp()->GetPlaceableByGameObjectID(nId);
    if (!pPlaceable) {
        CExoString sModel;
        if (!g_pAppManager->GetClientExoApp()->GetClientOptions()->m_bGore) {
            unsigned char bMissing;
            {
                CExoString sColumn("LowGore");
                C2DA* pTable = g_pRules->Get2DArrays()->GetPlaceables();
                bMissing = !pTable->GetCExoStringEntry(nAppearance, sColumn, &sModel);
            }
            if (bMissing) {
                CExoString sColumn("ModelName");
                C2DA* pTable = g_pRules->Get2DArrays()->GetPlaceables();
                pTable->GetCExoStringEntry(nAppearance, sColumn, &sModel);
            }
        }
        else {
            g_pRules->Get2DArrays()->GetPlaceables()->GetCExoStringEntry(nAppearance, CExoString("ModelName"), &sModel);
        }
        pPlaceable = new CSWCPlaceable;
        if (!pPlaceable) {
            return 0;
        }
        pPlaceable->SetAppearance(nAppearance);
        pPlaceable->SetObjectID(nId);
        pPlaceable->SetArea(this);
        if (pObjects->AddExternalObject(&nId, pPlaceable, 1) != GAME_OBJECT_VALID) {
            return 0;
        }
        pPlaceable->SetPosition(&vPosition);
        pPlaceable->SetOrientation(&vOrientation);
        Quaternion qOrientation = EulerToQuaternion(GetFacing(vOrientation), 0.0f, 0.0f);
    }
    pPlaceable->m_bUseable = 1;
    pPlaceable->m_bHitCheck = 0;
    pPlaceable->m_n130 = 0;
    pPlaceable->m_n114 = 0;
    pPlaceable->m_n118 = 0;
    pPlaceable->m_n12C = 0;
    pPlaceable->m_n120 = 0;
    pPlaceable->m_n124 = 0;
    pPlaceable->SetAnimation(0, 0, 0);
    g_pAppManager->m_pClientExoApp->RemoveObjectFromHitcheckIgnoreList(nId);
    m_aStaticPlaceables.Add(nId);
    return 1;
}
