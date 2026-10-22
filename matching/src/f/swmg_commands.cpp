// SWMG mini-game helpers and small NWScript SWMG commands
// (0x005CB350-0x005CB730): CSWVirtualMachineCommands::ExecuteCommandSWMG_*,
// CSWMGGunBank::SetBulletModel and the float test at 0x005CB440.

class CExoString {
public:
    CExoString();
    CExoString(const char *s);
    ~CExoString();
    CExoString &operator=(const CExoString &s);
    CExoString &operator=(const char *s);
    char *CStr() const;
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef();
    CResRef(const CExoString &sName);
    CResRef operator=(const CResRef &cResRef);
    void CopyToString(CExoString &sStr) const;
    char m_resRef[16];
};

class Vector {
public:
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    float x, y, z;
};

class CSWMGGunBank;

class CVirtualMachine {
public:
    int StackPopInteger(int *nValue);
    int StackPopFloat(float *fValue);
    int StackPushFloat(float fValue);
    int StackPushVector(Vector vValue);
    int StackPopVector(Vector *vValue);
    int StackPopString(CExoString *sValue);
    int StackPushInteger(int nValue);
    int StackPopObject(unsigned long *oidValue);
    int StackPushString(const CExoString &sValue);
    int StackPushObject(unsigned long oidValue);
};

extern CVirtualMachine *g_pVirtualMachine;

class CAurNamed {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual char *GetName();
};

class CSWTrackFollower;

class CSWMiniEnemy {
public:
    char pad_0x0[0x1a0];
    unsigned char m_bActive;         // +0x1a0
};

class CSWMiniGameObject {
public:
    virtual void PlayAnimation(const char *szAnim, float fSpeed, int nFlags);
    virtual CExoString GetModelName();
    virtual void LoadScripts();
    virtual void GetScript();
    virtual void SetScript();
    virtual CSWTrackFollower *AsTrackFollower();
    virtual void *AsMiniPlayer();
    virtual CSWMiniEnemy *AsMiniEnemy();
    virtual void *AsMiniObstacle();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual void SetSphereRadius(int nValue);

    int m_nObjectType;               // +0x04
    unsigned long m_nId;             // +0x08
    char pad_0xc[0x4];
    CExoString m_sTag;               // +0x10
    CAurNamed *m_pModel;             // +0x18
    int m_n1c;                       // +0x1c
    char pad_0x20[0xc];
    int m_n2c;                       // +0x2c
    char pad_0x30[0x14];
    CExoString m_s44;                // +0x44
    char pad_0x4c[0x2c];
    int m_n78;                       // +0x78
    char pad_0x7c[0x4];
    int m_n80;                       // +0x80
    float m_f84;                     // +0x84
    char pad_0x88[0x4];
    int m_n8c;                       // +0x8c
    int m_n90;                       // +0x90
    char pad_0x94[0x4];
    float m_f98;                     // +0x98
    float m_f9c;                     // +0x9c
    float m_fa0;                     // +0xa0
    char pad_0xa4[0xf8];
    int m_n19c;                      // +0x19c
    char pad_0x1a0[0x24];
    Vector m_v1c4;                   // +0x1c4
    char pad_0x1d0[0x8];
    float m_f1d8;                    // +0x1d8
    float m_f1dc;                    // +0x1dc
    float m_f1e0;                    // +0x1e0
    Vector m_v1e4;                   // +0x1e4
    Vector m_v1f0;                   // +0x1f0
    Vector m_v1fc;                   // +0x1fc
    Vector m_v208;                   // +0x208

    float SetPlayerFloat0(float f);
    float SetPlayerFloat1(float f);
    float SetPlayerFloat2(float f);
    float SetPlayerFloat3(float f);
    Vector SetPlayerVector0(const Vector &v);
    Vector SetPlayerVector1(const Vector &v);
    int GetPositionStart(int n);
    int GetPositionMiddle(int n);
    int GetPositionEnd(int n);
    void SetPositionStart(int nA, int nB);
    void SetPositionMiddle(int nA, int nB);
    void SetPositionEnd(int nA, int nB);

    int GetIsActive();
    Vector GetHookPosition();
    void SetNumLoops(int nValue);
    float SetAccelerationPerSecond(float fValue);
    float SetMaxSpeed(float fValue);
    CSWMGGunBank *GetGunBank(int nIndex);

    CSWMiniGameObject *ValidateTarget4C();
    CSWMiniGameObject *ValidateTarget50();
    void RunHitBulletScript(int bHit);
    void SetSpeed(int nValue);
    void SetSoundSet(int nValue);
};

class CSWTrackFollower {
public:
    void RunHitFollowerScript(int bHit);
    void RunTrackScript(int bHit);
    void RunDeathScript(int bHit);
    CSWMiniGameObject *GetBulletTarget();

    char pad_0x0[0x174];
    int m_n174;                      // +0x174
    char pad_0x178[0xc];
    int m_n184;                      // +0x184
};

class CSWMiniGameObjectArray {
public:
    CSWMiniGameObject *GetObject(unsigned long nId, unsigned char nType);
    CSWMiniGameObject *GetObjectByTag(const CExoString &sTag);
};

class CSWCMiniGame {
public:
    CSWMiniGameObject *GetEnemy(int nIndex);
    CSWMiniGameObject *GetObstacle(int nIndex);
    void GetLateralLimits(float *pfMin, float *pfMax);
    void SetLateralLimits(float fMin, float fMax);

    char pad_0x0[0x24];
    CSWMiniGameObject *m_pPlayer;    // +0x24
    char pad_0x28[0x8];
    int m_n30;                       // +0x30
    char pad_0x34[0x14];
    int m_n48;                       // +0x48
    char pad_0x4c[0x70];
    float m_fFarClip;                // +0xbc

    void SetFarClip(float fValue) { m_fFarClip = fValue; }
    float GetFarClip() { return m_fFarClip; }
};

class CClientExoApp {
public:
    CSWMiniGameObjectArray *GetMiniGameObjects();
    CSWCMiniGame *GetMiniGame();
};

class CAppManager {
public:
    void *m_pUnknown00;
    CClientExoApp *m_pClientExoApp;  // +0x04
    CClientExoApp *GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager *g_pAppManager;

class CSWMGGunBank {
public:
    int GetIsActive();
    void SetBulletModel(const CResRef &cModel);

    virtual void AttachGun();
    virtual void Die();
    virtual int GetNumBullets();
    void SetGunModel(const CResRef &cModel);
    void SetBulletSpeed(int nValue);
    float SetFireDelay(float fValue);
    float SetSpread(float fValue);
    float SetInaccuracy(float fValue);
    float SetSensingRadius(float fValue);
    float SetDamage(float fValue);
    float SetTimeBetweenShots(float fValue);
    float SetBulletLifespan(float fValue);

    CResRef m_cBulletModel;          // +0x04
    CResRef m_cGunModel;             // +0x14
    char pad_0x24[0x1c];
    int m_n40;                       // +0x40
    float m_f44;                     // +0x44
    float m_f48;                     // +0x48
    float m_f4c;                     // +0x4c
    int m_n50;                       // +0x50
    char pad_0x54[0x18];
    float m_f6c;                     // +0x6c
    float m_f70;                     // +0x70
    float m_f74;                     // +0x74
    float m_f78;                     // +0x78
    char pad_0x7c[0x20];
    float m_f9c;                     // +0x9c
};

class CSWVirtualMachineCommands {
public:
    void *m_pVtable;                 // +0x00
    int m_bValidObjectRunScript;     // +0x04
    unsigned long m_oidObjectRunScript; // +0x08
    struct CVirtualMachineCommandTable *m_pCommands; // +0x0c

    void InitializeSWMGCommands();
    int ExecuteCommandSWMG_GetPlayerFloat(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetPlayerVector(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_SetPlayerFloat(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_SetPlayerVector(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetFollowerPosition(int nCommandId, int nParameters);

    int ExecuteCommandSWMG_SetLateralAccelerationPerSecond(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetObjectName(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetObjectModelName(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetObjectByName(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetModelName(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetLastEvent(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_SetFollowerPosition(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_SetFarClip(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetFarClip(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetGunBankTargetFloat(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_SetGunBankModel(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_SetGunBankInt(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_SetGunBankFloat(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_SetGunBankTargetFloat(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetGunBankInt(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetGunBankFloat(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetHookPosition(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_SetObjectInt(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_SetObjectFloat(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetGunBankModel(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetGameObject(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetObjectInt(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_ResetSpeed(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetFollowerFloatInfo(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_PlayAnimationOnce(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetLateralLimit(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_SetLateralLimits(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetPlayer(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetGameInt(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_SetFollowerInt(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_RunScript(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetTarget2(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_PlayAnimation(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetFollowerInt(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetTarget(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_IsType(int nCommandId, int nParameters);
    int ExecuteCommandSWMG_GetEnemyValue(int nCommandId, int nParameters);
};

// FUNCTION: SWKOTOR 0x005CB350
int CSWVirtualMachineCommands::ExecuteCommandSWMG_SetLateralAccelerationPerSecond(int nCommandId,
                                                                                int nParameters)
{
    int nA;
    int nB;
    float fC;
    CExoString sName;
    if (!g_pVirtualMachine->StackPopString(&sName))
        return -2001;
    if (!g_pVirtualMachine->StackPopInteger(&nA))
        return -2001;
    if (!g_pVirtualMachine->StackPopInteger(&nB))
        return -2001;
    if (!g_pVirtualMachine->StackPopFloat(&fC))
        return -2001;
    sName.CStr();
    return 0;
}

// FUNCTION: SWKOTOR 0x005CB730
int CSWVirtualMachineCommands::ExecuteCommandSWMG_PlayAnimation(int nCommandId, int nParameters)
{
    int nFlags = 0;
    unsigned long oidObject;
    int nLooping;
    int nOverlay;
    int nQueue;
    CExoString sAnimation;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    if (!g_pVirtualMachine->StackPopString(&sAnimation))
        return -2001;
    if (!g_pVirtualMachine->StackPopInteger(&nLooping))
        return -2001;
    if (!g_pVirtualMachine->StackPopInteger(&nQueue))
        return -2001;
    if (!g_pVirtualMachine->StackPopInteger(&nOverlay))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 0);
    if (pObject) {
        if (!nLooping)
            nFlags |= 1;
        if (nOverlay)
            nFlags |= 8;
        if (nQueue)
            nFlags |= 0x20;
        pObject->PlayAnimation(sAnimation.CStr(), 1.0f, nFlags);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005CB870
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetFollowerInt(int nCommandId, int nParameters)
{
    int nValue = -1;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(m_oidObjectRunScript, 0);
    if (pObject) {
        switch (nCommandId) {
        case 0x24b:
            nValue = pObject->m_n1c;
            break;
        case 0x24c:
            nValue = pObject->m_n2c;
            break;
        case 0x253:
            if (pObject->AsTrackFollower())
                nValue = pObject->AsTrackFollower()->m_n174;
            break;
        case 0x254:
            if (pObject->AsTrackFollower())
                nValue = pObject->AsTrackFollower()->m_n184;
            break;
        }
    }
    if (!g_pVirtualMachine->StackPushInteger(nValue))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CB930
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetTarget(int nCommandId, int nParameters)
{
    unsigned long oidTarget = 0xff;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(m_oidObjectRunScript, 0);
    if (pObject) {
        CSWMiniGameObject *pTarget = pObject->ValidateTarget4C();
        if (pTarget)
            oidTarget = pTarget->m_nId;
    }
    if (!g_pVirtualMachine->StackPushObject(oidTarget))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CB990
int CSWVirtualMachineCommands::ExecuteCommandSWMG_SetFollowerInt(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    int nValue;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    if (!g_pVirtualMachine->StackPopInteger(&nValue))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 4);
    if (pObject) {
        switch (nCommandId) {
        case 0x24e:
            pObject->SetSpeed(nValue);
            break;
        case 0x25c:
            pObject->SetSoundSet(nValue);
            return 0;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005CBA20
int CSWVirtualMachineCommands::ExecuteCommandSWMG_RunScript(int nCommandId, int nParameters)
{
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(m_oidObjectRunScript, 0);
    if (pObject) {
        switch (nCommandId) {
        case 0x24f:
            pObject->RunHitBulletScript(1);
            return 0;
        case 0x250:
            if (pObject->AsTrackFollower())
                pObject->AsTrackFollower()->RunHitFollowerScript(1);
            return 0;
        case 0x256:
            if (pObject->AsTrackFollower())
                pObject->AsTrackFollower()->RunTrackScript(1);
            return 0;
        case 0x25d:
            if (pObject->AsTrackFollower())
                pObject->AsTrackFollower()->RunDeathScript(1);
            return 0;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005CBB00
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetTarget2(int nCommandId, int nParameters)
{
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(m_oidObjectRunScript, 0);
    unsigned long oidTarget = 0xff;
    if (pObject) {
        CSWMiniGameObject *pTarget = 0;
        switch (nCommandId) {
        case 0x251:
            pTarget = pObject->ValidateTarget50();
            break;
        case 0x252: {
            CSWTrackFollower *pFollower = pObject->AsTrackFollower();
            if (pFollower)
                pTarget = pFollower->GetBulletTarget();
            break;
        }
        }
        if (pTarget)
            oidTarget = pTarget->m_nId;
    }
    if (!g_pVirtualMachine->StackPushObject(oidTarget))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CBDB0
int CSWVirtualMachineCommands::ExecuteCommandSWMG_PlayAnimationOnce(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    CExoString sAnimation;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    if (!g_pVirtualMachine->StackPopString(&sAnimation))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 0);
    if (pObject)
        pObject->PlayAnimation(sAnimation.CStr(), 1.0f, 0x80);
    return 0;
}

// FUNCTION: SWKOTOR 0x005CBE90
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetLateralLimit(int nCommandId, int nParameters)
{
    float fValue = 0.0f;
    CSWCMiniGame *pGame = g_pAppManager->GetClientExoApp()->GetMiniGame();
    if (pGame) {
        switch (nCommandId) {
        case 0x260:
            pGame->GetLateralLimits(&fValue, 0);
            break;
        case 0x261:
            pGame->GetLateralLimits(0, &fValue);
            break;
        }
    }
    if (!g_pVirtualMachine->StackPushFloat(fValue))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CBF00
int CSWVirtualMachineCommands::ExecuteCommandSWMG_SetLateralLimits(int nCommandId, int nParameters)
{
    float fMin;
    float fMax;
    CSWCMiniGame *pGame = g_pAppManager->GetClientExoApp()->GetMiniGame();
    if (!g_pVirtualMachine->StackPopFloat(&fMin))
        return -2001;
    if (!g_pVirtualMachine->StackPopFloat(&fMax))
        return -2001;
    if (pGame)
        pGame->SetLateralLimits(fMin, fMax);
    return 0;
}

// FUNCTION: SWKOTOR 0x005CBF70
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetPlayer(int nCommandId, int nParameters)
{
    CSWCMiniGame *pGame = g_pAppManager->GetClientExoApp()->GetMiniGame();
    unsigned long oidPlayer = 0xff;
    if (pGame && pGame->m_pPlayer)
        oidPlayer = pGame->m_pPlayer->m_nId;
    if (!g_pVirtualMachine->StackPushObject(oidPlayer))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CBFB0
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGameInt(int nCommandId, int nParameters)
{
    CSWCMiniGame *pGame = g_pAppManager->GetClientExoApp()->GetMiniGame();
    int nValue = 0;
    if (pGame) {
        switch (nCommandId) {
        case 0x264:
            nValue = pGame->m_n30;
            break;
        case 0x266:
            nValue = pGame->m_n48;
            break;
        }
    }
    if (!g_pVirtualMachine->StackPushInteger(nValue))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CC000
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGameObject(int nCommandId, int nParameters)
{
    int nIndex;
    CSWCMiniGame *pGame = g_pAppManager->GetClientExoApp()->GetMiniGame();
    unsigned long oidObject = 0xff;
    if (!g_pVirtualMachine->StackPopInteger(&nIndex))
        return -2001;
    if (pGame) {
        CSWMiniGameObject *pObject;
        switch (nCommandId) {
        case 0x265:
            pObject = pGame->GetEnemy(nIndex);
            break;
        case 0x267:
            pObject = pGame->GetObstacle(nIndex);
            break;
        }
        if (pObject)
            oidObject = pObject->m_nId;
    }
    if (!g_pVirtualMachine->StackPushObject(oidObject))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CC0A0
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetObjectInt(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    int nValue = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 4);
    if (pObject) {
        switch (nCommandId) {
        case 0x268:
            nValue = pObject->m_n8c;
            break;
        case 0x269:
            nValue = pObject->m_n90;
            break;
        case 0x26d:
            nValue = pObject->m_n80;
            break;
        case 0x270:
            nValue = pObject->m_n78;
            break;
        case 0x299:
            nValue = pObject->GetIsActive();
            break;
        }
    }
    if (!g_pVirtualMachine->StackPushInteger(nValue))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CC190
int CSWVirtualMachineCommands::ExecuteCommandSWMG_ResetSpeed(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 4);
    if (pObject)
        pObject->m_f9c = pObject->m_fa0;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CC1E0
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetFollowerFloatInfo(int nCommandId,
                                                                      int nParameters)
{
    float fValue = 0.0f;
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 4);
    if (pObject) {
        switch (nCommandId) {
        case 0x26b:
            fValue = pObject->m_f84;
            break;
        case 0x282:
            fValue = pObject->m_fa0;
            break;
        }
    }
    if (!g_pVirtualMachine->StackPushFloat(fValue))
        return -2000;
    return 0;
}


// FUNCTION: SWKOTOR 0x005CC280
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetHookPosition(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    Vector vPosition;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 4);
    if (pObject)
        vPosition = pObject->GetHookPosition();
    if (!g_pVirtualMachine->StackPushVector(vPosition))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CC330
int CSWVirtualMachineCommands::ExecuteCommandSWMG_SetObjectInt(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    int nValue;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    if (!g_pVirtualMachine->StackPopInteger(&nValue))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 4);
    if (pObject) {
        switch (nCommandId) {
        case 0x26a:
            pObject->SetNumLoops(nValue);
            break;
        case 0x26e:
            pObject->SetSphereRadius(nValue);
            return 0;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005CC3C0
int CSWVirtualMachineCommands::ExecuteCommandSWMG_SetObjectFloat(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    float fValue;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    if (!g_pVirtualMachine->StackPopFloat(&fValue))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 4);
    if (pObject) {
        switch (nCommandId) {
        case 0x26c:
            pObject->SetAccelerationPerSecond(fValue);
            break;
        case 0x288:
            pObject->SetMaxSpeed(fValue);
            return 0;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005CC450
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGunBankModel(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    int nBank;
    CExoString sModel;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    if (!g_pVirtualMachine->StackPopInteger(&nBank))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 4);
    if (pObject) {
        CSWMGGunBank *pBank = pObject->GetGunBank(nBank);
        if (pBank) {
            switch (nCommandId) {
            case 0x271:
                pBank->m_cBulletModel.CopyToString(sModel);
                break;
            case 0x272:
                pBank->m_cGunModel.CopyToString(sModel);
                break;
            }
        }
    }
    if (!g_pVirtualMachine->StackPushString(sModel))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CC580
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGunBankInt(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    int nBank;
    int nValue = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    if (!g_pVirtualMachine->StackPopInteger(&nBank))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 4);
    if (pObject) {
        CSWMGGunBank *pBank = pObject->GetGunBank(nBank);
        if (pBank) {
            switch (nCommandId) {
            case 0x273:
                nValue = pBank->m_n40;
                break;
            case 0x277:
                nValue = pBank->m_n50;
                break;
            case 0x280:
                nValue = pBank->GetNumBullets();
                break;
            }
        }
    }
    if (!g_pVirtualMachine->StackPushInteger(nValue))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CC640
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGunBankFloat(int nCommandId, int nParameters)
{
    float fValue = 0.0f;
    unsigned long oidObject;
    int nBank;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    if (!g_pVirtualMachine->StackPopInteger(&nBank))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 4);
    if (pObject) {
        CSWMGGunBank *pBank = pObject->GetGunBank(nBank);
        if (pBank) {
            switch (nCommandId) {
            case 0x274:
                fValue = pBank->m_f48;
                break;
            case 0x275:
                fValue = pBank->m_f4c;
                break;
            case 0x276:
                fValue = pBank->m_f44;
                break;
            }
        }
    }
    if (!g_pVirtualMachine->StackPushFloat(fValue))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CC700
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGunBankTargetFloat(int nCommandId,
                                                                       int nParameters)
{
    float fValue = 0.0f;
    unsigned long oidObject;
    int nBank;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    if (!g_pVirtualMachine->StackPopInteger(&nBank))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 4);
    if (pObject) {
        CSWMGGunBank *pBank = pObject->GetGunBank(nBank);
        if (pBank && pBank->GetNumBullets()) {
            switch (nCommandId) {
            case 0x295:
                fValue = pBank->m_f6c;
                break;
            case 0x296:
                fValue = pBank->m_f74;
                break;
            case 0x297:
                fValue = pBank->m_f70;
                break;
            case 0x298:
                fValue = pBank->m_f78;
                break;
            }
        }
    }
    if (!g_pVirtualMachine->StackPushFloat(fValue))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CC7F0
int CSWVirtualMachineCommands::ExecuteCommandSWMG_SetGunBankModel(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    int nBank;
    CExoString sModel;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    if (!g_pVirtualMachine->StackPopInteger(&nBank))
        return -2001;
    if (!g_pVirtualMachine->StackPopString(&sModel))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 4);
    if (pObject) {
        CSWMGGunBank *pBank = pObject->GetGunBank(nBank);
        if (pBank) {
            switch (nCommandId) {
            case 0x278:
                pBank->SetBulletModel(sModel);
                break;
            case 0x279:
                pBank->SetGunModel(sModel);
                break;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005CC920
int CSWVirtualMachineCommands::ExecuteCommandSWMG_SetGunBankInt(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    int nBank;
    int nValue;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    if (!g_pVirtualMachine->StackPopInteger(&nBank))
        return -2001;
    if (!g_pVirtualMachine->StackPopInteger(&nValue))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 4);
    if (pObject) {
        CSWMGGunBank *pBank = pObject->GetGunBank(nBank);
        if (pBank) {
            switch (nCommandId) {
            case 0x27a:
                pBank->m_n40 = nValue;
                break;
            case 0x27e:
                pBank->SetBulletSpeed(nValue);
                return 0;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005CC9D0
int CSWVirtualMachineCommands::ExecuteCommandSWMG_SetGunBankFloat(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    int nBank;
    float fValue;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    if (!g_pVirtualMachine->StackPopInteger(&nBank))
        return -2001;
    if (!g_pVirtualMachine->StackPopFloat(&fValue))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 4);
    if (pObject) {
        CSWMGGunBank *pBank = pObject->GetGunBank(nBank);
        if (pBank) {
            switch (nCommandId) {
            case 0x27b:
                pBank->SetFireDelay(fValue);
                break;
            case 0x27c:
                pBank->SetSpread(fValue);
                return 0;
            case 0x27d:
                pBank->SetInaccuracy(fValue);
                return 0;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005CCAA0
int CSWVirtualMachineCommands::ExecuteCommandSWMG_SetGunBankTargetFloat(int nCommandId,
                                                                       int nParameters)
{
    unsigned long oidObject;
    int nBank;
    float fValue;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    if (!g_pVirtualMachine->StackPopInteger(&nBank))
        return -2001;
    if (!g_pVirtualMachine->StackPopFloat(&fValue))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 4);
    if (pObject) {
        CSWMGGunBank *pBank = pObject->GetGunBank(nBank);
        if (pBank && pBank->GetNumBullets()) {
            switch (nCommandId) {
            case 0x295:
                pBank->SetSensingRadius(fValue);
                break;
            case 0x296:
                pBank->SetDamage(fValue);
                break;
            case 0x297:
                pBank->SetTimeBetweenShots(fValue);
                break;
            case 0x298:
                pBank->SetBulletLifespan(fValue);
                break;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005CCBB0
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetLastEvent(int nCommandId, int nParameters)
{
    CExoString sEvent;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(m_oidObjectRunScript, 0);
    if (pObject)
        sEvent = pObject->m_s44;
    if (!g_pVirtualMachine->StackPushString(sEvent))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CD160
int CSWVirtualMachineCommands::ExecuteCommandSWMG_SetFollowerPosition(int nCommandId,
                                                                     int nParameters)
{
    unsigned long oidObject;
    int nA;
    int nB;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    if (!g_pVirtualMachine->StackPopInteger(&nA))
        return -2001;
    if (!g_pVirtualMachine->StackPopInteger(&nB))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 4);
    if (pObject) {
        switch (nCommandId) {
        case 0x2ac:
            pObject->SetPositionStart(nA, nB);
            break;
        case 0x2ae:
            pObject->SetPositionMiddle(nA, nB);
            return 0;
        case 0x2b0:
            pObject->SetPositionEnd(nA, nB);
            return 0;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005CD230
int CSWVirtualMachineCommands::ExecuteCommandSWMG_SetFarClip(int nCommandId, int nParameters)
{
    float fValue;
    if (!g_pVirtualMachine->StackPopFloat(&fValue))
        return -2001;
    g_pAppManager->GetClientExoApp()->GetMiniGame()->SetFarClip(fValue);
    return 0;
}

// FUNCTION: SWKOTOR 0x005CD280
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetFarClip(int nCommandId, int nParameters)
{
    float fValue = g_pAppManager->GetClientExoApp()->GetMiniGame()->GetFarClip();
    if (!g_pVirtualMachine->StackPushFloat(fValue))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CCC60
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetPlayerFloat(int nCommandId, int nParameters)
{
    float fValue = 0.0f;
    CSWMiniGameObject *pPlayer = g_pAppManager->GetClientExoApp()->GetMiniGame()->m_pPlayer;
    if (pPlayer) {
        switch (nCommandId) {
        case 0x283:
            fValue = pPlayer->m_f98;
            break;
        case 0x284:
            fValue = pPlayer->m_f1d8;
            break;
        case 0x285:
            fValue = pPlayer->m_f1e0;
            break;
        case 0x29b:
            fValue = pPlayer->m_f1dc;
            break;
        }
    }
    if (!g_pVirtualMachine->StackPushFloat(fValue))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CCD20
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetPlayerVector(int nCommandId, int nParameters)
{
    Vector vValue;
    CSWMiniGameObject *pPlayer = g_pAppManager->GetClientExoApp()->GetMiniGame()->m_pPlayer;
    if (pPlayer) {
        switch (nCommandId) {
        case 0x281:
            vValue = pPlayer->m_v1c4;
            break;
        case 0x286:
            vValue = pPlayer->m_v1e4;
            break;
        case 0x28d:
            vValue = pPlayer->m_v1f0;
            break;
        case 0x28f:
            vValue = pPlayer->m_v208;
            break;
        case 0x2cd:
            vValue = pPlayer->m_v1fc;
            break;
        }
    }
    if (!g_pVirtualMachine->StackPushVector(vValue))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CCE70
int CSWVirtualMachineCommands::ExecuteCommandSWMG_SetPlayerFloat(int nCommandId, int nParameters)
{
    float fValue;
    if (!g_pVirtualMachine->StackPopFloat(&fValue))
        return -2001;
    CSWMiniGameObject *pPlayer = g_pAppManager->GetClientExoApp()->GetMiniGame()->m_pPlayer;
    if (pPlayer) {
        switch (nCommandId) {
        case 0x289:
            pPlayer->SetPlayerFloat0(fValue);
            return 0;
        case 0x28a:
            pPlayer->SetPlayerFloat1(fValue);
            return 0;
        case 0x28b:
            pPlayer->SetPlayerFloat2(fValue);
            return 0;
        case 0x29c:
            pPlayer->SetPlayerFloat3(fValue);
            break;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005CCF30
int CSWVirtualMachineCommands::ExecuteCommandSWMG_SetPlayerVector(int nCommandId, int nParameters)
{
    Vector vValue;
    if (!g_pVirtualMachine->StackPopVector(&vValue))
        return -2001;
    CSWMiniGameObject *pPlayer = g_pAppManager->GetClientExoApp()->GetMiniGame()->m_pPlayer;
    if (pPlayer) {
        switch (nCommandId) {
        case 0x287:
            pPlayer->SetPlayerVector0(vValue);
            return 0;
        case 0x28c:
            pPlayer->m_v1e4 = vValue;
            return 0;
        case 0x28e:
            pPlayer->m_v1f0 = vValue;
            return 0;
        case 0x290:
            pPlayer->SetPlayerVector1(vValue);
        case 0x2ce:
            pPlayer->m_v1fc = vValue;
            break;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005CD0A0
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetFollowerPosition(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    int nIndex;
    int nValue = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    if (!g_pVirtualMachine->StackPopInteger(&nIndex))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 4);
    if (pObject) {
        switch (nCommandId) {
        case 0x2ab:
            nValue = pObject->GetPositionStart(nIndex);
            break;
        case 0x2ad:
            nValue = pObject->GetPositionMiddle(nIndex);
            break;
        case 0x2af:
            nValue = pObject->GetPositionEnd(nIndex);
            break;
        }
    }
    if (!g_pVirtualMachine->StackPushInteger(nValue))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CB440
int CSWMGGunBank::GetIsActive()
{
    if (m_f9c > 0.0f)
        return 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CB460
void CSWMGGunBank::SetBulletModel(const CResRef &cModel)
{
    m_cBulletModel = cModel;
}

// FUNCTION: SWKOTOR 0x005CB480
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetObjectName(int nCommandId, int nParameters)
{
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(m_oidObjectRunScript, 0);
    CExoString sName;
    if (pObject)
        sName = pObject->m_sTag;
    if (!g_pVirtualMachine->StackPushString(sName.CStr()))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CB560
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetObjectModelName(int nCommandId,
                                                                    int nParameters)
{
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(m_oidObjectRunScript, 0);
    CExoString sName;
    if (pObject && pObject->m_pModel)
        sName = pObject->m_pModel->GetName();
    if (!g_pVirtualMachine->StackPushString(sName.CStr()))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CB650
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetObjectByName(int nCommandId, int nParameters)
{
    CExoString sName;
    unsigned long oidObject = 0xff;
    if (!g_pVirtualMachine->StackPopString(&sName))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObjectByTag(sName);
    if (pObject)
        oidObject = pObject->m_nId;
    if (!g_pVirtualMachine->StackPushObject(oidObject))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CBB80
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetModelName(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    CExoString sName;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 0);
    if (pObject)
        sName = pObject->GetModelName();
    if (!g_pVirtualMachine->StackPushString(sName))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CBC80
int CSWVirtualMachineCommands::ExecuteCommandSWMG_IsType(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    int bResult = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidObject))
        return -2001;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(oidObject, 0);
    if (pObject) {
        switch (nCommandId) {
        case 0x257:
            if (pObject->AsTrackFollower())
                bResult = 1;
            break;
        case 0x258:
            if (pObject->AsMiniPlayer())
                bResult = 1;
            break;
        case 0x259:
            if (pObject->AsMiniEnemy())
                bResult = 1;
            break;
        case 0x25b:
            if (pObject->AsMiniObstacle())
                bResult = 1;
        case 0x25a:
            if (pObject->AsMiniEnemy() && pObject->AsMiniEnemy()->m_bActive)
                bResult = 1;
            break;
        }
    }
    if (!g_pVirtualMachine->StackPushInteger(bResult))
        return -2000;
    return 0;
}

// FUNCTION: SWKOTOR 0x005CBD60
int CSWVirtualMachineCommands::ExecuteCommandSWMG_GetEnemyValue(int nCommandId, int nParameters)
{
    int nValue = 0;
    CSWMiniGameObject *pObject =
        g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(m_oidObjectRunScript, 4);
    if (pObject)
        nValue = pObject->m_n19c;
    if (!g_pVirtualMachine->StackPushInteger(nValue))
        return -2000;
    return 0;
}

struct CVirtualMachineCommandTable {
    int (CSWVirtualMachineCommands::*m_pfnCommands[0x300])(int nCommandId, int nParameters);
};

// FUNCTION: SWKOTOR 0x005CD2C0
void CSWVirtualMachineCommands::InitializeSWMGCommands()
{
    m_pCommands->m_pfnCommands[0x246] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetLateralAccelerationPerSecond;
    m_pCommands->m_pfnCommands[0x247] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetObjectName;
    m_pCommands->m_pfnCommands[0x248] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetObjectModelName;
    m_pCommands->m_pfnCommands[0x249] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetObjectByName;
    m_pCommands->m_pfnCommands[0x24a] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_PlayAnimation;
    m_pCommands->m_pfnCommands[0x24b] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetFollowerInt;
    m_pCommands->m_pfnCommands[0x24c] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetFollowerInt;
    m_pCommands->m_pfnCommands[0x24d] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetTarget;
    m_pCommands->m_pfnCommands[0x24e] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetFollowerInt;
    m_pCommands->m_pfnCommands[0x24f] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_RunScript;
    m_pCommands->m_pfnCommands[0x250] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_RunScript;
    m_pCommands->m_pfnCommands[0x251] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetTarget2;
    m_pCommands->m_pfnCommands[0x252] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetTarget2;
    m_pCommands->m_pfnCommands[0x253] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetFollowerInt;
    m_pCommands->m_pfnCommands[0x254] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetFollowerInt;
    m_pCommands->m_pfnCommands[0x255] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetModelName;
    m_pCommands->m_pfnCommands[0x256] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_RunScript;
    m_pCommands->m_pfnCommands[0x257] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_IsType;
    m_pCommands->m_pfnCommands[0x258] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_IsType;
    m_pCommands->m_pfnCommands[0x259] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_IsType;
    m_pCommands->m_pfnCommands[0x25a] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_IsType;
    m_pCommands->m_pfnCommands[0x25b] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_IsType;
    m_pCommands->m_pfnCommands[0x25c] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetFollowerInt;
    m_pCommands->m_pfnCommands[0x25d] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_RunScript;
    m_pCommands->m_pfnCommands[0x25e] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetEnemyValue;
    m_pCommands->m_pfnCommands[0x25f] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_PlayAnimationOnce;
    m_pCommands->m_pfnCommands[0x260] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetLateralLimit;
    m_pCommands->m_pfnCommands[0x261] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetLateralLimit;
    m_pCommands->m_pfnCommands[0x262] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetLateralLimits;
    m_pCommands->m_pfnCommands[0x263] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetPlayer;
    m_pCommands->m_pfnCommands[0x264] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGameInt;
    m_pCommands->m_pfnCommands[0x265] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGameObject;
    m_pCommands->m_pfnCommands[0x266] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGameInt;
    m_pCommands->m_pfnCommands[0x267] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGameObject;
    m_pCommands->m_pfnCommands[0x268] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetObjectInt;
    m_pCommands->m_pfnCommands[0x269] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetObjectInt;
    m_pCommands->m_pfnCommands[0x26a] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetObjectInt;
    m_pCommands->m_pfnCommands[0x26b] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetFollowerFloatInfo;
    m_pCommands->m_pfnCommands[0x26c] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetObjectFloat;
    m_pCommands->m_pfnCommands[0x26d] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetObjectInt;
    m_pCommands->m_pfnCommands[0x26e] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetObjectInt;
    m_pCommands->m_pfnCommands[0x26f] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetHookPosition;
    m_pCommands->m_pfnCommands[0x270] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetObjectInt;
    m_pCommands->m_pfnCommands[0x271] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGunBankModel;
    m_pCommands->m_pfnCommands[0x272] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGunBankModel;
    m_pCommands->m_pfnCommands[0x273] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGunBankInt;
    m_pCommands->m_pfnCommands[0x274] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGunBankFloat;
    m_pCommands->m_pfnCommands[0x275] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGunBankFloat;
    m_pCommands->m_pfnCommands[0x276] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGunBankFloat;
    m_pCommands->m_pfnCommands[0x277] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGunBankInt;
    m_pCommands->m_pfnCommands[0x278] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetGunBankModel;
    m_pCommands->m_pfnCommands[0x279] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetGunBankModel;
    m_pCommands->m_pfnCommands[0x27a] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetGunBankInt;
    m_pCommands->m_pfnCommands[0x27b] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetGunBankFloat;
    m_pCommands->m_pfnCommands[0x27c] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetGunBankFloat;
    m_pCommands->m_pfnCommands[0x27d] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetGunBankFloat;
    m_pCommands->m_pfnCommands[0x27e] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetGunBankInt;
    m_pCommands->m_pfnCommands[0x27f] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetLastEvent;
    m_pCommands->m_pfnCommands[0x280] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGunBankInt;
    m_pCommands->m_pfnCommands[0x281] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetPlayerVector;
    m_pCommands->m_pfnCommands[0x282] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetFollowerFloatInfo;
    m_pCommands->m_pfnCommands[0x283] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetPlayerFloat;
    m_pCommands->m_pfnCommands[0x284] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetPlayerFloat;
    m_pCommands->m_pfnCommands[0x29b] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetPlayerFloat;
    m_pCommands->m_pfnCommands[0x285] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetPlayerFloat;
    m_pCommands->m_pfnCommands[0x286] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetPlayerVector;
    m_pCommands->m_pfnCommands[0x287] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetPlayerVector;
    m_pCommands->m_pfnCommands[0x288] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetObjectFloat;
    m_pCommands->m_pfnCommands[0x289] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetPlayerFloat;
    m_pCommands->m_pfnCommands[0x28a] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetPlayerFloat;
    m_pCommands->m_pfnCommands[0x29c] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetPlayerFloat;
    m_pCommands->m_pfnCommands[0x28b] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetPlayerFloat;
    m_pCommands->m_pfnCommands[0x28c] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetPlayerVector;
    m_pCommands->m_pfnCommands[0x28d] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetPlayerVector;
    m_pCommands->m_pfnCommands[0x28e] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetPlayerVector;
    m_pCommands->m_pfnCommands[0x28f] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetPlayerVector;
    m_pCommands->m_pfnCommands[0x290] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetPlayerVector;
    m_pCommands->m_pfnCommands[0x291] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGunBankTargetFloat;
    m_pCommands->m_pfnCommands[0x292] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGunBankTargetFloat;
    m_pCommands->m_pfnCommands[0x293] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGunBankTargetFloat;
    m_pCommands->m_pfnCommands[0x294] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetGunBankTargetFloat;
    m_pCommands->m_pfnCommands[0x295] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetGunBankTargetFloat;
    m_pCommands->m_pfnCommands[0x296] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetGunBankTargetFloat;
    m_pCommands->m_pfnCommands[0x297] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetGunBankTargetFloat;
    m_pCommands->m_pfnCommands[0x298] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetGunBankTargetFloat;
    m_pCommands->m_pfnCommands[0x299] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetObjectInt;
    m_pCommands->m_pfnCommands[0x29a] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_ResetSpeed;
    m_pCommands->m_pfnCommands[0x2ab] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetFollowerPosition;
    m_pCommands->m_pfnCommands[0x2ac] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetFollowerPosition;
    m_pCommands->m_pfnCommands[0x2ad] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetFollowerPosition;
    m_pCommands->m_pfnCommands[0x2ae] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetFollowerPosition;
    m_pCommands->m_pfnCommands[0x2af] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetFollowerPosition;
    m_pCommands->m_pfnCommands[0x2b0] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetFollowerPosition;
    m_pCommands->m_pfnCommands[0x2ce] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetPlayerVector;
    m_pCommands->m_pfnCommands[0x2cd] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetPlayerVector;
    m_pCommands->m_pfnCommands[0x209] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_GetFarClip;
    m_pCommands->m_pfnCommands[0x208] = &CSWVirtualMachineCommands::ExecuteCommandSWMG_SetFarClip;
}
