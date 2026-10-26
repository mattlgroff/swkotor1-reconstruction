// CSWSAreaOfEffectObject shape, position, 2DA / GFF load and save
// (0x005945F0-0x00595450). The class layout here extends the one in
// aoe_collision.cpp (+0x22c area effect row, +0x230 shape, +0x238 radius,
// +0x23c width, +0x240 length, +0x244 rectangle corners, +0x260 scripts).

class Vector {
public:
    Vector();                                               // 0x00409E70
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator-(const Vector &v) const { return Vector(x - v.x, y - v.y, z - v.z); }
    Vector operator+(const Vector &v) const { return Vector(x + v.x, y + v.y, z + v.z); }
    Vector operator*(float f) const { return Vector(x * f, y * f, z * f); }
    Vector operator-() const { return Vector(-x, -y, -z); }
    float x, y, z;
};

Vector CrossProduct(const Vector &a, const Vector &b);      // 0x004AB210
Vector Normalize(const Vector &v);                          // 0x004AB180
Vector operator/(const Vector &v, float f);                 // 0x00437A90

class CExoString {
public:
    CExoString();                                           // 0x005B3190
    CExoString(const char *s);                              // 0x005E5A90
    ~CExoString();                                          // 0x005E5C20
    CExoString &operator=(const CExoString &s);             // 0x005E5C50
    int operator==(const char *s) const;                    // 0x005E5290
    char *m_sString;
    unsigned long m_nBufferLength;
};

struct CResStruct;

class CResGFF {
public:
    CExoString ReadFieldCExoString(CResStruct *pStruct, char *szLabel, int &bSuccess,
                                   const CExoString &sDefault);     // 0x00411EC0
    int ReadFieldINT(CResStruct *pStruct, char *szLabel, int &bSuccess, int nDefault);  // 0x00411C90
    unsigned long ReadFieldDWORD(CResStruct *pStruct, char *szLabel, int &bSuccess,
                                 unsigned long nDefault);           // 0x00411C20
    unsigned char ReadFieldBYTE(CResStruct *pStruct, char *szLabel, int &bSuccess,
                                unsigned char nDefault);            // 0x00411A60
    float ReadFieldFLOAT(CResStruct *pStruct, char *szLabel, int &bSuccess, float fDefault);  // 0x00411D00
    int WriteFieldCExoString(CResStruct *pStruct, const CExoString &sValue, char *szLabel);
    int WriteFieldINT(CResStruct *pStruct, int nValue, char *szLabel);
    int WriteFieldDWORD(CResStruct *pStruct, unsigned long nValue, char *szLabel);
    int WriteFieldBYTE(CResStruct *pStruct, unsigned char nValue, char *szLabel);
    int WriteFieldFLOAT(CResStruct *pStruct, float fValue, char *szLabel);
};

class C2DA {
public:
    int GetCExoStringEntry(int nRow, const CExoString &sColumn, CExoString *pValue);  // 0x00413270
    int GetFLOATEntry(int nRow, const CExoString &sColumn, float *pValue);            // 0x00413430
};

class C2DArrays {
public:
    char pad_0x0[0x1c];
    C2DA *m_pVisualEffectPersistent;    // 0x1c
    C2DA *GetVisualEffectPersistent() { return m_pVisualEffectPersistent; }
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    C2DArrays *m_p2DArrays;             // 0xb8
    C2DArrays *Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules *g_pRules;              // 0x007A3A28

class CSWSCreature;
class CGameObject;

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    void Allocate(int s);
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num] = t;
        num++;
    }
};

class CSWSArea {
public:
    void AddObjectToArea(unsigned long oid, int bFlag);     // 0x0050DFD0
    void AddObjectId(unsigned long oid) { m_aObjectIds.Add(oid); }

    char pad_0x0[0x1a0];
    CExoArrayList<unsigned long> m_aObjectIds;   // 0x1a0
};

class CGameObjectArray {
public:
    char GetGameObject(unsigned long id, CGameObject **ppObject);    // 0x004D8230
};

class CSWSModule {
public:
    int AddObjectToLookupTable(const CExoString &sTag, unsigned long oid);  // 0x004C7DE0
};

class CServerExoApp {
public:
    CSWSModule *GetModule();                                // 0x004AE6B0
    CGameObjectArray *GetObjectArray();                     // 0x004AED70
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;     // 0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;      // 0x007A39FC
extern char g_cObjectArraySuccess;
extern unsigned long g_oidInvalid;      // 0x00746644

class CGameObject {
public:
    virtual ~CGameObject();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual CSWSCreature *AsSWSCreature();     // +0x30
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4c();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5c();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6c();
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void SetAnimation(int nAnimation);   // +0x7c
    virtual void v80();
    virtual void v84();
    virtual void v88();
    virtual void v8c();
    virtual void v90();
    virtual void v94();
    virtual void v98();
    virtual void v9c();
    virtual void va0();
    virtual void va4();
    virtual void va8();
    virtual void vac();
    virtual void vb0();
    virtual void vb4();
    virtual int GetLastSpellId();              // +0xb8

    unsigned long m_idSelf;             // 0x4
};

class CSWSObject : public CGameObject {
public:
    void SetTag(CExoString &sTag);                          // 0x00553040
    CSWSArea *GetArea();                                    // 0x004CB120
    void SetArea(CSWSArea *pArea);                          // 0x004CB160
    void SetPosition(const Vector &v, int a, int b, int c); // 0x004CD1A0

    char pad_0x8[0x18 - 0x8];
    CExoString m_sTag;                  // 0x18
    char pad_0x20[0x90 - 0x20];
    Vector m_vPosition;                 // 0x90
    Vector m_vOrientation;              // 0x9c
    char pad_0xa8[0x1d5 - 0xa8];
    unsigned char m_nMetaMagicType;     // 0x1d5
    char pad_0x1d6[0x22c - 0x1d6];
};

class CSWSCreature : public CSWSObject {
};

class CSWSAreaOfEffectObject : public CSWSObject {
public:
    void SetShape(unsigned char nShape, float fA, float fB);
    Vector GetPosition();
    void LoadAreaEffect(int nAreaEffectId);
    int LoadEffect(CResGFF *pRes, CResStruct *pStruct);
    int SaveEffect(CResGFF *pRes, CResStruct *pStruct);
    int InSubAreas(Vector vPosition);
    void UpdateSubAreas(int bFlag);                         // 0x00595640
    void AddToArea(CSWSArea *pArea, float x, float y, float z, int bFlag);
    float GetRadius() { return m_fRadius; }

    int m_nAreaEffectId;                // 0x22c
    unsigned char m_nShape;             // 0x230
    int m_nSpellId;                     // 0x234
    float m_fRadius;                    // 0x238
    float m_fWidth;                     // 0x23c
    float m_fLength;                    // 0x240
    Vector *m_pvCorners;                // 0x244
    unsigned long m_oidCreator;         // 0x248
    unsigned long m_oidLinkedTo;        // 0x24c
    unsigned long m_oidLastEntered;     // 0x250
    unsigned long m_oidLastLeft;        // 0x254
    int m_nSpellSaveDC;                 // 0x258
    int m_nSpellLevel;                  // 0x25c
    CExoString m_sScriptHeartbeat;      // 0x260
    CExoString m_sScriptUserDefined;    // 0x268
    CExoString m_sScriptOnEnter;        // 0x270
    CExoString m_sScriptOnExit;         // 0x278
    unsigned long m_nLastHeartbeatDay;  // 0x280
    unsigned long m_nLastHeartbeatTime; // 0x284
    unsigned long m_nDuration;          // 0x288
    unsigned char m_nDurationType;      // 0x28c
};

extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

// FUNCTION: SWKOTOR 0x005945F0
void CSWSAreaOfEffectObject::SetShape(unsigned char nShape, float fA, float fB)
{
    m_nShape = nShape;
    switch (nShape) {
    case 0:
        m_fRadius = fA;
        break;
    case 1: {
        float fHalfA = fA * 0.5f;
        float fHalfB = fB * 0.5f;
        m_fWidth = fA;
        m_fLength = fB;
        m_fRadius = (float)sqrt(fHalfB * fHalfB + fHalfA * fHalfA);
        m_pvCorners = new Vector[4];
        m_oidLinkedTo = 0x7f000000;
        break;
    }
    }
}

// The original calls itself when the effect is not linked to an object.
// FUNCTION: SWKOTOR 0x005946E0
Vector CSWSAreaOfEffectObject::GetPosition()
{
    CGameObject *pObject;
    if (m_oidLinkedTo != g_oidInvalid && m_nShape == 0 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidLinkedTo, &pObject) ==
            g_cObjectArraySuccess &&
        pObject)
        return pObject->AsSWSCreature()->m_vPosition;
    return GetPosition();
}

// FUNCTION: SWKOTOR 0x005947B0
void CSWSAreaOfEffectObject::LoadAreaEffect(int nAreaEffectId)
{
    m_nAreaEffectId = nAreaEffectId;
    C2DA *pTable = g_pRules->Get2DArrays()->m_pVisualEffectPersistent;
    CExoString sShape;
    pTable->GetCExoStringEntry(m_nAreaEffectId, "SHAPE", &sShape);
    if (sShape == "C") {
        float fRadius;
        pTable->GetFLOATEntry(m_nAreaEffectId, "RADIUS", &fRadius);
        SetShape(0, fRadius, 0.0f);
    } else if (sShape == "R") {
        float fWidth;
        float fLength;
        pTable->GetFLOATEntry(m_nAreaEffectId, "WIDTH", &fWidth);
        pTable->GetFLOATEntry(m_nAreaEffectId, "LENGTH", &fLength);
        SetShape(1, fWidth, fLength);
    }
    CExoString sOnEnter;
    pTable->GetCExoStringEntry(m_nAreaEffectId, "ONENTER", &sOnEnter);
    m_sScriptOnEnter = sOnEnter;
    CExoString sHeartbeat;
    pTable->GetCExoStringEntry(m_nAreaEffectId, "HEARTBEAT", &sHeartbeat);
    m_sScriptHeartbeat = sHeartbeat;
    CExoString sOnExit;
    pTable->GetCExoStringEntry(m_nAreaEffectId, "ONEXIT", &sOnExit);
    m_sScriptOnExit = sOnExit;
    CExoString sLabel;
    pTable->GetCExoStringEntry(m_nAreaEffectId, "LABEL", &sLabel);
    SetTag(sLabel);
    g_pAppManager->m_pServerExoApp->GetModule()->AddObjectToLookupTable(m_sTag, m_idSelf);
}

// FUNCTION: SWKOTOR 0x00594B00
int CSWSAreaOfEffectObject::LoadEffect(CResGFF *pRes, CResStruct *pStruct)
{
    int bSuccess;
    {
        CExoString sDefault("");
        SetTag(pRes->ReadFieldCExoString(pStruct, "Tag", bSuccess, sDefault));
    }
    m_nAreaEffectId = pRes->ReadFieldINT(pStruct, "AreaEffectId", bSuccess, 0);
    m_nSpellId = pRes->ReadFieldDWORD(pStruct, "SpellId", bSuccess, 0);
    unsigned char nShape = pRes->ReadFieldBYTE(pStruct, "Shape", bSuccess, 0);
    m_nMetaMagicType = pRes->ReadFieldBYTE(pStruct, "MetaMagicType", bSuccess, 0);
    m_nSpellSaveDC = pRes->ReadFieldINT(pStruct, "SpellSaveDC", bSuccess, 0);
    m_nSpellLevel = pRes->ReadFieldINT(pStruct, "SpellLevel", bSuccess, 0);
    if (nShape == 0) {
        float fRadius = pRes->ReadFieldFLOAT(pStruct, "Radius", bSuccess, 0.0f);
        SetShape(0, fRadius, 0.0f);
    } else if (nShape == 1) {
        float fLength = pRes->ReadFieldFLOAT(pStruct, "Length", bSuccess, 0.0f);
        float fWidth = pRes->ReadFieldFLOAT(pStruct, "Width", bSuccess, 0.0f);
        SetShape(1, fWidth, fLength);
    }
    m_oidCreator = pRes->ReadFieldDWORD(pStruct, "CreatorId", bSuccess, 0);
    m_oidLinkedTo = pRes->ReadFieldDWORD(pStruct, "LinkedToObject", bSuccess, 0);
    m_oidLastEntered = pRes->ReadFieldDWORD(pStruct, "LastEntered", bSuccess, 0);
    m_oidLastLeft = pRes->ReadFieldDWORD(pStruct, "LastLeft", bSuccess, 0);
    m_nDuration = pRes->ReadFieldDWORD(pStruct, "Duration", bSuccess, 0);
    m_nDurationType = pRes->ReadFieldBYTE(pStruct, "DurationType", bSuccess, 0);
    m_nLastHeartbeatDay = pRes->ReadFieldDWORD(pStruct, "LastHrtbtDay", bSuccess, 0);
    m_nLastHeartbeatTime = pRes->ReadFieldDWORD(pStruct, "LastHrtbtTime", bSuccess, 0);
    return 1;
}

// NONMATCH (35): the original copies GetPosition's returned temporary into
// registers (x ecx, y ebp, z stack) instead of using the local in place.
// FUNCTION: SWKOTOR 0x00594D80
int CSWSAreaOfEffectObject::SaveEffect(CResGFF *pRes, CResStruct *pStruct)
{
    pRes->WriteFieldCExoString(pStruct, m_sTag, "Tag");
    pRes->WriteFieldINT(pStruct, m_nAreaEffectId, "AreaEffectId");
    pRes->WriteFieldDWORD(pStruct, GetLastSpellId(), "SpellId");
    pRes->WriteFieldBYTE(pStruct, m_nShape, "Shape");
    pRes->WriteFieldBYTE(pStruct, m_nMetaMagicType, "MetaMagicType");
    pRes->WriteFieldINT(pStruct, m_nSpellSaveDC, "SpellSaveDC");
    pRes->WriteFieldINT(pStruct, m_nSpellLevel, "SpellLevel");
    if (m_nShape == 0) {
        pRes->WriteFieldFLOAT(pStruct, GetRadius(), "Radius");
    } else if (m_nShape == 1) {
        pRes->WriteFieldFLOAT(pStruct, m_fLength, "Length");
        pRes->WriteFieldFLOAT(pStruct, m_fWidth, "Width");
    }
    pRes->WriteFieldDWORD(pStruct, m_oidCreator, "CreatorId");
    pRes->WriteFieldDWORD(pStruct, m_oidLinkedTo, "LinkedToObject");
    pRes->WriteFieldDWORD(pStruct, m_oidLastEntered, "LastEntered");
    pRes->WriteFieldDWORD(pStruct, m_oidLastLeft, "LastLeft");
    pRes->WriteFieldDWORD(pStruct, m_nDuration, "Duration");
    pRes->WriteFieldBYTE(pStruct, m_nDurationType, "DurationType");
    pRes->WriteFieldDWORD(pStruct, m_nLastHeartbeatDay, "LastHrtbtDay");
    pRes->WriteFieldDWORD(pStruct, m_nLastHeartbeatTime, "LastHrtbtTime");
    pRes->WriteFieldCExoString(pStruct, m_sScriptHeartbeat, "OnHeartbeat");
    pRes->WriteFieldCExoString(pStruct, m_sScriptUserDefined, "OnUserDefined");
    pRes->WriteFieldCExoString(pStruct, m_sScriptOnEnter, "OnObjEnter");
    pRes->WriteFieldCExoString(pStruct, m_sScriptOnExit, "OnObjExit");
    Vector vPosition = GetPosition();
    pRes->WriteFieldFLOAT(pStruct, vPosition.x, "PositionX");
    pRes->WriteFieldFLOAT(pStruct, vPosition.y, "PositionY");
    pRes->WriteFieldFLOAT(pStruct, vPosition.z, "PositionZ");
    Vector vOrientation = m_vOrientation;
    pRes->WriteFieldFLOAT(pStruct, vOrientation.x, "OrientationX");
    pRes->WriteFieldFLOAT(pStruct, vOrientation.y, "OrientationY");
    pRes->WriteFieldFLOAT(pStruct, vOrientation.z, "OrientationZ");
    return 1;
}

// FUNCTION: SWKOTOR 0x00595450
int CSWSAreaOfEffectObject::InSubAreas(Vector vPosition)
{
    Vector vDiff = vPosition - GetPosition();
    if (vDiff.x * vDiff.x + vDiff.y * vDiff.y + vDiff.z * vDiff.z <= m_fRadius * m_fRadius) {
        if (m_nShape != 0) {
            for (int i = 0; i < 4; i++) {
                int j = (i == 3) ? 0 : i + 1;
                if (CrossProduct(m_pvCorners[j] - m_pvCorners[i], vPosition - m_pvCorners[i]).z > 0.0f)
                    return 0;
            }
        }
        return 1;
    }
    return 0;
}

// NONMATCH (30): the corner loop keeps &m_pvCorners[i] in edi (offset in ebp)
// across the operator/ call; here the address is recomputed.
// FUNCTION: SWKOTOR 0x00595F30
void CSWSAreaOfEffectObject::AddToArea(CSWSArea *pArea, float x, float y, float z, int bFlag)
{
    if (!pArea)
        return;
    SetArea(pArea);
    SetPosition(Vector(x, y, z), 0, 1, 0);
    if (m_nShape == 1 && m_pvCorners) {
        Vector vDir = Normalize(m_vOrientation);
        Vector vPerp(-vDir.y, vDir.x, vDir.z);
        m_pvCorners[0] = vDir * m_fLength + vPerp * m_fWidth;
        m_pvCorners[1] = vDir * m_fLength - vPerp * m_fWidth;
        m_pvCorners[2] = -vDir * m_fLength - vPerp * m_fWidth;
        m_pvCorners[3] = -vDir * m_fLength + vPerp * m_fWidth;
        for (int i = 0; i < 4; i++)
            m_pvCorners[i] = m_pvCorners[i] / 2.0f + GetPosition();
    }
    GetArea()->AddObjectToArea(m_idSelf, bFlag);
    CSWSArea *pCurArea = GetArea();
    pCurArea->AddObjectId(m_idSelf);
    SetAnimation(1);
    UpdateSubAreas(0);
}
