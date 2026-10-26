// CSWCArea environment slots of the primary (0x0074EEF0) vtable and the
// AsSWCArea override of its CGameObject subobject at +0x100 (worker c, s4).
// 0x00605160, 0x006051F0, 0x00605260 and 0x00606470 are real starts missing
// from functions.csv. Names follow the playable source.

#include <stdio.h>

class Vector {
public:
    float x, y, z;
    Vector() {}
    Vector(float fx, float fy, float fz) { x = fx; y = fy; z = fz; }
};

inline Vector operator*(const Vector& v, float f)
{
    Vector r;
    r.x = v.x * f;
    r.y = v.y * f;
    r.z = v.z * f;
    return r;
}

Vector Normalize(const Vector& v);                  // 0x004AB180
bool AurSetGlobalWind(Vector vWind);                // 0x004530D0
bool AurSetGlobalWindVariation(float fA, float fB); // 0x00453140
unsigned char AurControlPart(char* sCommand);       // 0x0044B900
void SetClearColor(const char* sColor);             // 0x00401690

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    char* CStr() const;                             // 0x005E5670
    int CompareNoCase(const char* s) const;         // 0x005E58C0
    void Format(const char* sFormat, ...);          // 0x005E5680
    char* m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    void CopyToString(CExoString& s) const;         // 0x00405F70
    char m_resRef[16];
};

class CSWCRoom {
public:
    virtual ~CSWCRoom();
    char pad_0x4[0x1c];
    CResRef m_cResRef;                              // 0x20
    char pad_0x30[0x18];

    CResRef GetResRef() { return m_cResRef; }
};

class CAurScene {
public:
    virtual void v00(); virtual void v04();
    virtual void SetFogColor(const Vector* pColor); // 0x08
};

class CAurLight {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual void AttachToScene(CAurScene* pScene);  // 0x4c
};

class CSWCArea;

class CSWCAreaEnvironment {
public:
    virtual ~CSWCAreaEnvironment();
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual int SetWind(unsigned char nStrength);   // 0x10
    virtual int SetGlobalLightDiffuse(int bOn, Vector vColor);  // 0x14
    virtual int SetGlobalLightAmbient(int bOn, Vector vColor);  // 0x18
    virtual void v1C();
    virtual void v20();

    char pad_0x4[0x50];
    Vector m_vFogColor;                             // 0x54
    char pad_0x60[0x4b];
    unsigned char m_nWindStrength;                  // 0xab
    char pad_0xac[0x54];
};

class CGameObject {
public:
    virtual ~CGameObject();
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20();
    virtual CSWCArea* AsSWCArea();                  // 0x24

    unsigned long m_idSelf;                         // 0x04
    unsigned char m_nObjectType;                    // 0x08
};

class CSWCArea : public CSWCAreaEnvironment, public CGameObject {
public:
    virtual int SetWind(unsigned char nStrength);
    virtual int SetGlobalLightDiffuse(int bOn, Vector vColor);
    virtual int SetGlobalLightAmbient(int bOn, Vector vColor);
    virtual CSWCArea* AsSWCArea();
    virtual ~CSWCArea();

    int SetFogColor(const Vector* pColor);
    CSWCRoom* GetRoom(const CExoString& sName);

    char pad_0x10c[0x78];
    CAurScene* m_pScene;                            // 0x184
    CAurLight* m_pSunLight;                         // 0x188
    char pad_0x18c[0xd0];
    int m_nRooms;                                   // 0x25c
    CSWCRoom* m_pRooms;                             // 0x260
};

extern const float AREA_WIND_SCALE_NONE;            // 0x0073D700
extern const float AREA_WIND_SCALE_LIGHT;           // 0x0073D764
extern const float AREA_WIND_SCALE_STRONG;          // 0x00741BE0

// FUNCTION: SWKOTOR 0x00605090
int CSWCArea::SetFogColor(const Vector* pColor)
{
    if (!m_pScene) {
        return 0;
    }
    m_pScene->SetFogColor(pColor);
    CExoString sColor;
    sColor.Format("%f %f %f", pColor->x, pColor->y, pColor->z);
    SetClearColor(sColor.CStr());
    m_vFogColor = *pColor;
    return 1;
}

// FUNCTION: SWKOTOR 0x00605510
CSWCRoom* CSWCArea::GetRoom(const CExoString& sName)
{
    CSWCRoom* pRoom = 0;
    for (int i = 0; i < m_nRooms; i++) {
        CResRef cResRef = m_pRooms[i].GetResRef();
        CExoString sRoom;
        cResRef.CopyToString(sRoom);
        if (sName.CompareNoCase(sRoom.CStr())) {
            pRoom = &m_pRooms[i];
            break;
        }
    }
    return pRoom;
}

// FUNCTION: SWKOTOR 0x00605160
int CSWCArea::SetGlobalLightDiffuse(int bOn, Vector vColor)
{
    char sCommand[0x80];
    if (!m_pSunLight) {
        return 0;
    }
    if (!bOn) {
        m_pSunLight->AttachToScene(0);
        return 1;
    }
    sprintf(sCommand, "sun gidy_sun_diff color %f %f %f", vColor.x, vColor.y, vColor.z);
    AurControlPart(sCommand);
    return 1;
}

// FUNCTION: SWKOTOR 0x006051F0
int CSWCArea::SetGlobalLightAmbient(int bOn, Vector vColor)
{
    char sCommand[0x80];
    if (!m_pScene) {
        return 0;
    }
    sprintf(sCommand, "sun gidy_sun_amb color %f %f %f", vColor.x, vColor.y, vColor.z);
    AurControlPart(sCommand);
    return 1;
}

// FUNCTION: SWKOTOR 0x00605260
int CSWCArea::SetWind(unsigned char nStrength)
{
    float fVariationA = 0.0f;
    float fVariationB = 0.0f;
    Vector vDirection(1.0f, 1.0f, 0.0f);
    vDirection = Normalize(vDirection);
    float fScale;
    switch (nStrength) {
    case 0:
        fScale = AREA_WIND_SCALE_NONE;
        break;
    case 1:
        fScale = AREA_WIND_SCALE_LIGHT;
        fVariationA = 100.0f;
        fVariationB = 3.0f;
        break;
    case 2:
        fScale = AREA_WIND_SCALE_STRONG;
        fVariationA = 150.0f;
        fVariationB = 5.0f;
        break;
    default:
        return 0;
    }
    AurSetGlobalWind(vDirection * fScale);
    AurSetGlobalWindVariation(fVariationA, fVariationB);
    m_nWindStrength = nStrength;
    return 1;
}

// FUNCTION: SWKOTOR 0x00606470
CSWCArea* CSWCArea::AsSWCArea()
{
    return this;
}
