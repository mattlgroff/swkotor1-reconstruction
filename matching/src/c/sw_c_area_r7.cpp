// CSWCArea V3 leaves (TestDirectLine, ClearLineOfSightOneWay,
// UpdateShadowingLights, GetIsDark, GetServerArea, ParentCorpseExists,
// SetFog, UpdateSoundObjectsTimeOfDay) and the small unnamed CSWCArea
// members around them (0x00604BC0-0x006075B0).

#include "c/client_exo_app.h"
#include "c/exo_base.h"

struct Vector {
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

extern float g_fFogDensity;                         // 0x0078E6A0
extern float g_fAmbientScale;                       // 0x0078E408

class CSWCAreaObject {
public:
    virtual ~CSWCAreaObject();
    char pad_0x4[0x104];
    unsigned long m_n108;                           // 0x108
    unsigned long m_n10c;                           // 0x10c
};

struct CPlaceableLight {
    unsigned long m_oidPlaceable;                   // 0x00
    void* m_pLight;                                 // 0x04
};

template <class T>
inline void FreeArray(CExoArrayList<T>& a)
{
    if (a.element) {
        delete[] a.element;
        a.element = 0;
    }
    a.array_size = 0;
    a.num = 0;
}

struct CSWCRoom {
    char pad_0x0[0x48];
};

extern float g_fAmbientBright;                      // 0x007A22AC
extern float g_fAmbientDark;                        // 0x007A22B0
extern const float g_fDarkThreshold;               // 0x00740C84 (3.0f)

class CSWSArea {
public:
    void AttachmentFromClientArea(CSWCArea* pArea); // 0x00506A60 RET 4
};

class CServerAreaLookup {
public:
    int TestDirectLine(float x1, void* pAreaPos, float y1, float z1, float x2, float y2, float z2, float r);   // 0x004AEE80 RET 0x20
    unsigned long ClientToServerObjectID(unsigned long oid);    // 0x004AEA30
    CSWSArea* GetAreaByGameObjectID(unsigned long oid);         // 0x004AE780
    int GetRoomIndex(void* pAreaPos, unsigned long a, unsigned long b);   // 0x004AEAF0 RET 0xc
};

class CSWCAreaScene {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40();
    virtual void SetFog(float fStart, float fEnd); // 0x44
};

class CSWCSoundObject {
public:
    void UpdateTimeOfDay();                         // 0x0066AE90
};

class CSWCArea {
public:
    virtual void v00();

    int TestDirectLine(float x1, float y1, float z1, float x2, float y2, float z2, float r);
    int ClearLineOfSightOneWay(float x1, float y1, float z1, float x2, float y2, float z2, float r);
    void UpdateShadowingLights();
    int GetIsDark();
    CSWSArea* GetServerArea();
    unsigned long GetId() { return m_idSelf; }
    int ParentCorpseExists(unsigned long oid);
    int SetFog(float fStart, float fEnd, unsigned long nColor);
    void UpdateSoundObjectsTimeOfDay();
    void SetFogColor(unsigned long nColor);         // 0x00605090 RET 4
    void SetAsDay();                                // 0x006058C0
    void SetAsNight();                              // 0x006059C0
    int IsCorpse(unsigned long oid);
    void SetUnknown1f1(unsigned char b);
    void ClearUnknown1f1(int n);
    int ReturnTrue(int n);
    CSWCRoom* GetRoom(int nIndex);
    int GetRoomIndex(unsigned long a, unsigned long b);
    void SetServerArea(CSWSArea* pArea);
    void ClearServerArea();
    CSWCAreaObject* FindObject(unsigned long a, unsigned long b);
    void* GetPlaceableObjectLight(unsigned long oid);
    CSWCRoom* GetRoomAt(unsigned long a);
    void ResetColors();
    void SetDayTransition();
    void SetNightTransition();
    void AddObject(CSWCAreaObject* p);
    void DeleteObjects();
    void ClearUnknownLists();

    char pad_0x4[0x5c];
    float m_fFogStart;                              // 0x60
    float m_fFogEnd;                                // 0x64
    char pad_0x68[0x30];
    unsigned char m_bUnknown98;                     // 0x98
    char pad_0x99[0x7];
    int m_bIsNight;                                 // 0xa0
    int m_bDayNightPending;                         // 0xa4
    char pad_0xa8[0xc];
    unsigned char m_nAmbientScale;                  // 0xb4
    char pad_0xb5[0x4f];
    unsigned long m_idSelf;                         // 0x104
    char pad_0x108[0x4];
    char m_cAreaPos[0x1c];                          // 0x10c
    Vector m_vNightMoonColor;                       // 0x128
    Vector m_vNightSunColor;                        // 0x134
    char pad_0x140[0xc];
    Vector m_vMoonColor;                            // 0x14c
    Vector m_vSunColor;                             // 0x158
    char pad_0x164[0x20];
    CSWCAreaScene* m_pScene;                        // 0x184
    int m_bShadowingLights;                         // 0x188
    char pad_0x18c[0x28];
    int m_nUnknown1b4;                              // 0x1b4
    int m_nUnknown1b8;                              // 0x1b8
    char pad_0x1bc[0x4];
    int m_bUnknown1c0;                              // 0x1c0
    CExoArrayList<CSWCAreaObject*> m_aObjects;      // 0x1c4
    unsigned long* m_pSoundObjects;                 // 0x1d0
    int m_nSoundObjects;                            // 0x1d4
    char pad_0x1d8[0x14];
    int m_nUnknown1ec;                              // 0x1ec
    char pad_0x1f0[0x1];
    unsigned char m_bUnknown1f1;                    // 0x1f1
    char pad_0x1f2[0x26];
    CExoArrayList<CPlaceableLight*> m_aPlaceableLights;// 0x218
    char pad_0x224[0x10];
    int m_bIsDark;                                  // 0x234
    CExoArrayList<unsigned long> m_aUnknown238;     // 0x238
    CExoArrayList<unsigned long> m_aUnknown244;     // 0x244
    char pad_0x250[0x10];
    CSWCRoom* m_pRooms;                             // 0x260
    char pad_0x264[0x4];
    unsigned long m_aCorpses[4];                    // 0x268
    unsigned char m_nCorpseRead;                    // 0x278
    unsigned char m_nCorpseWrite;                   // 0x279
    char pad_0x27a[0x2];
    CSWSArea* m_pServerArea;                        // 0x27c
};

// FUNCTION: SWKOTOR 0x00604C40
int CSWCArea::TestDirectLine(float x1, float y1, float z1, float x2, float y2, float z2, float r)
{
    CServerAreaLookup* pServer = (CServerAreaLookup*)g_pAppManager->m_pServerExoApp;
    if (pServer) {
        return pServer->TestDirectLine(x1, m_cAreaPos, y1, z1, x2, y2, z2, r);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00605370
int CSWCArea::ClearLineOfSightOneWay(float x1, float y1, float z1, float x2, float y2, float z2, float r)
{
    return 1;
}

// FUNCTION: SWKOTOR 0x00605410
void CSWCArea::UpdateShadowingLights()
{
    if (m_bShadowingLights) {
        CSWCCreature* pCreature = g_pAppManager->m_pClientExoApp->GetPlayerCreature();
        if (pCreature) {
            pCreature->GetCreatureLight(1);
        }
    }
}

// FUNCTION: SWKOTOR 0x00605440
int CSWCArea::GetIsDark()
{
    if (m_pScene == 0) {
        return 0;
    }
    CSWCCreature* pCreature = g_pAppManager->m_pClientExoApp->GetPlayerCreature();
    if (pCreature) {
        pCreature->GetCreatureLight(0);
    }
    int bDark;
    if ((g_fAmbientBright * 1.732) * (g_fAmbientBright * 1.732) >= g_fDarkThreshold) {
        bDark = 1;
    } else if ((g_fAmbientDark * 1.732) * (g_fAmbientDark * 1.732) <= g_fDarkThreshold) {
        bDark = 0;
    } else {
        bDark = m_bIsDark;
    }
    m_bIsDark = bDark;
    return bDark;
}

// FUNCTION: SWKOTOR 0x00605640
CSWSArea* CSWCArea::GetServerArea()
{
    if (m_pServerArea == 0) {
        CServerAreaLookup* pServer = (CServerAreaLookup*)g_pAppManager->GetServerExoApp();
        m_pServerArea = pServer->GetAreaByGameObjectID(pServer->ClientToServerObjectID(GetId()));
        if (m_pServerArea) {
            m_pServerArea->AttachmentFromClientArea(this);
        }
    }
    return m_pServerArea;
}

// FUNCTION: SWKOTOR 0x006057B0
int CSWCArea::ParentCorpseExists(unsigned long oid)
{
    for (unsigned char i = 0; i < 4; i++) {
        CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_aCorpses[i]);
        if (pCreature && pCreature->m_oidCorpseContainer == oid) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00605870
int CSWCArea::SetFog(float fStart, float fEnd, unsigned long nColor)
{
    if (m_pScene == 0) {
        return 0;
    }
    m_pScene->SetFog(fStart, fEnd);
    m_fFogStart = fStart;
    m_fFogEnd = fEnd;
    SetFogColor(nColor);
    return 1;
}

// FUNCTION: SWKOTOR 0x00605B10
void CSWCArea::UpdateSoundObjectsTimeOfDay()
{
    int n = m_nSoundObjects;
    for (int i = 0; i < n; i++) {
        CSWCSoundObject* pSound = g_pAppManager->GetClientExoApp()->GetSoundObjectByGameObjectID(m_pSoundObjects[i]);
        if (pSound) {
            pSound->UpdateTimeOfDay();
        }
    }
}

// FUNCTION: SWKOTOR 0x00604BC0
int CSWCArea::IsCorpse(unsigned long oid)
{
    for (int i = 0; i < 4; i++) {
        if (m_aCorpses[i] == oid) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00605380
void CSWCArea::SetUnknown1f1(unsigned char b)
{
    m_bUnknown1f1 = b;
    m_nUnknown1ec = 6000;
}

// FUNCTION: SWKOTOR 0x006053A0
void CSWCArea::ClearUnknown1f1(int n)
{
    m_bUnknown1f1 = 0;
    m_nUnknown1ec = 6000;
}

// FUNCTION: SWKOTOR 0x006054E0
int CSWCArea::ReturnTrue(int n)
{
    return 1;
}

// FUNCTION: SWKOTOR 0x006054F0
CSWCRoom* CSWCArea::GetRoom(int nIndex)
{
    CSWCRoom* pRoom = 0;
    if (nIndex != -1) {
        pRoom = &m_pRooms[nIndex];
    }
    return pRoom;
}

// FUNCTION: SWKOTOR 0x00605610
int CSWCArea::GetRoomIndex(unsigned long a, unsigned long b)
{
    return ((CServerAreaLookup*)g_pAppManager->GetServerExoApp())->GetRoomIndex(m_cAreaPos, a, b);
}

// FUNCTION: SWKOTOR 0x00605690
void CSWCArea::SetServerArea(CSWSArea* pArea)
{
    m_pServerArea = pArea;
}

// FUNCTION: SWKOTOR 0x006056A0
void CSWCArea::ClearServerArea()
{
    m_pServerArea = 0;
}

// FUNCTION: SWKOTOR 0x00605AC0
CSWCAreaObject* CSWCArea::FindObject(unsigned long a, unsigned long b)
{
    for (int i = 0; i < m_aObjects.num; i++) {
        CSWCAreaObject* p = m_aObjects[i];
        if (p && p->m_n108 == a && p->m_n10c == b) {
            return p;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00605BF0
void* CSWCArea::GetPlaceableObjectLight(unsigned long oid)
{
    for (int i = 0; i < m_aPlaceableLights.num; i++) {
        if (m_aPlaceableLights[i] && m_aPlaceableLights[i]->m_oidPlaceable == oid) {
            return m_aPlaceableLights[i]->m_pLight;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00605FA0
CSWCRoom* CSWCArea::GetRoomAt(unsigned long a)
{
    CSWCRoom* pRoom = 0;
    int nIndex = GetRoomIndex(a, 0);
    if (nIndex != -1) {
        pRoom = &m_pRooms[nIndex];
    }
    return pRoom;
}

// FUNCTION: SWKOTOR 0x00605FE0
void CSWCArea::ResetColors()
{
    m_vNightMoonColor = Vector(0.0f, 0.0f, 0.0f);
    m_vMoonColor = m_vNightMoonColor;
    m_vNightSunColor = Vector(1.0f, 1.0f, 1.0f);
    m_vSunColor = m_vNightSunColor;
    m_bUnknown98 = 0;
    if (m_bDayNightPending) {
        SetAsNight();
    } else {
        SetAsDay();
    }
}

// FUNCTION: SWKOTOR 0x00606C50
void CSWCArea::SetDayTransition()
{
    if (!m_bIsNight) {
        if (m_bDayNightPending) {
            SetAsNight();
        } else {
            SetAsDay();
        }
        return;
    }
    g_fFogDensity = 0.05f;
    g_fAmbientScale = (float)m_nAmbientScale * 0.01f;
    m_bUnknown1c0 = 1;
    m_nUnknown1b8 = 1;
    m_nUnknown1b4 = 0;
    m_bDayNightPending = 0;
    if (m_bShadowingLights) {
        CSWCCreature* pPlayer = g_pAppManager->m_pClientExoApp->GetPlayerCreature();
        if (pPlayer) {
            pPlayer->GetCreatureLight(1);
        }
    }
}

// FUNCTION: SWKOTOR 0x00606CF0
void CSWCArea::SetNightTransition()
{
    if (!m_bIsNight) {
        if (m_bDayNightPending) {
            SetAsNight();
        } else {
            SetAsDay();
        }
        return;
    }
    g_fFogDensity = 0.0f;
    g_fAmbientScale = (float)m_nAmbientScale * 0.01f;
    m_bUnknown1c0 = 1;
    m_nUnknown1b4 = 1;
    m_nUnknown1b8 = 0;
    m_bDayNightPending = 1;
    if (m_bShadowingLights) {
        CSWCCreature* pPlayer = g_pAppManager->m_pClientExoApp->GetPlayerCreature();
        if (pPlayer) {
            pPlayer->GetCreatureLight(1);
        }
    }
}

// FUNCTION: SWKOTOR 0x00606D90
void CSWCArea::AddObject(CSWCAreaObject* p)
{
    m_aObjects.Add(p);
}

// FUNCTION: SWKOTOR 0x00606DD0
void CSWCArea::DeleteObjects()
{
    int n = m_aObjects.num;
    for (int i = 0; i < n; i++) {
        if (m_aObjects[i]) {
            delete m_aObjects[i];
        }
    }
    m_aObjects.SetSize(0);
}

// FUNCTION: SWKOTOR 0x006075B0
void CSWCArea::ClearUnknownLists()
{
    FreeArray(m_aUnknown244);
    FreeArray(m_aUnknown238);
}
