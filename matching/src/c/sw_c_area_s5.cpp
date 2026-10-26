// CSWCArea members, worker C s5.

class CSWCArea;

class Vector {
public:
    float x, y, z;
    Vector() {}
    Vector(float fx, float fy, float fz) { x = fx; y = fy; z = fz; }
};

class CResRef {
public:
    CResRef(const char* s);                         // 0x00406D80
    char m_resRef[16];
};

class CAurObject {
public:
    virtual ~CAurObject();                          // 0x00
    virtual void v04();
    virtual void v08();
    virtual char* GetName();                        // 0x0c
    virtual void v10(); virtual void v14();
    virtual int PlayAnimation(const char* szAnim, float fSpeed, int a, int b);   // 0x18
    virtual int GetEventTime(const char* szAnim, float* pfA, float* pfB);        // 0x1c
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual void SetScene(void* pScene);            // 0x4c
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60();
    virtual Vector GetPosition();                   // 0x64

    void AnimateExternal(float fTime);              // 0x0043E150
};

CAurObject* NewCAurObject(const char* szModel, const char* szName, int a, int b);   // 0x00449CC0

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const char* s);           // 0x005E5140
    char* CStr() const;                             // 0x005E5670

    char* m_sString;
    unsigned long m_nBufferLength;
};

struct CSWMiniTrackPoint {
    CResRef m_cName;                                // 0x00
    Vector m_vPos;                                  // 0x10
};

class CSWMiniGameObject {
public:
    char pad_0x0[0x60];
    CAurObject* m_pModel;                           // 0x60 (CSWMiniObstacle)
};

class CSWMiniCamera;
class CSWMiniObstacle;

class CSWCMiniGame {
public:
    CSWCMiniGame(CSWCArea* pArea);                  // 0x00671C40
    virtual ~CSWCMiniGame();                        // 0x00672EB0

    void Go();                                      // 0x006714E0
    CSWMiniTrackPoint* GetTrackPoint(int i);        // 0x00671610
    CSWMiniGameObject* GetObstacle(int i);          // 0x00671630
    CSWMiniCamera* SetCamera(CSWMiniCamera* p);     // 0x00671670
    CSWMiniTrackPoint* AddTrackPoint(const CResRef& cName, const Vector& vPos);   // 0x006720C0
    CSWMiniObstacle* AddObstacle(const CResRef& cModel, const Vector& vPos);      // 0x00672240
    int Load();                                     // 0x006723D0

    char pad_0x4[0x1c];
    CSWMiniCamera* m_pCamera;                       // 0x20
    char pad_0x24[0x3c - 0x24];
    int m_nTrackPoints;                             // 0x3c (m_lstTrackPoints.num)
    char pad_0x40[0x48 - 0x40];
    int m_nObstacles;                               // 0x48 (m_lstObstacles.num)
    char pad_0x4c[0xf0 - 0x4c];
};

class CSWCCreature {
public:
    void SetHilited(int b);                         // 0x0060E760 (folded empty body, RET 4)

    char pad_0x0[0x2dc];
    unsigned char m_nHiliteType;                    // 0x2dc (descriptive: 0 neutral, 1 friendly)
};

class CSWCObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual CSWCCreature* AsSWCCreature();          // 0x2c
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0();
    virtual int Hilite(const Vector& vColor, int a, int b, int c);   // 0xa4
    virtual int UnHilite(int a);                    // 0xa8

    char pad_0x4[0x4];
    unsigned char m_nObjectType;                    // 0x08
    char pad_0x9[0x90 - 0x9];
    unsigned char m_bHilitable;                     // 0x90 (descriptive)
};

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual CSWCObject* AsSWCObject();              // 0x0c
};

class CClientExoApp {
public:
    CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
    CSWCCreature* GetPlayerCreature();              // 0x005ED540
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    T& operator[](int i) { return element[i]; }
};

class CSWCArea {
public:
    CSWCMiniGame* RestartMiniGame();
    void HiliteAreaObjects(int bOn);
    int SetWeather(unsigned char nWeather, float fAmount);

    char pad_0x0[0xaa];
    unsigned char m_bLightning;                     // 0xaa
    char pad_0xab[0x184 - 0xab];
    void* m_pScene;                                 // 0x184
    char pad_0x188[0x4];
    CAurObject* m_pWeather[6];                      // 0x18c
    CAurObject* m_pLightning;                       // 0x1a4
    char pad_0x1a8[0x1f0 - 0x1a8];
    unsigned char m_nWeather;                       // 0x1f0
    char pad_0x1f1[0x20c - 0x1f1];
    CExoArrayList<unsigned long> m_aGameObjects;    // 0x20c
    char pad_0x218[0x264 - 0x218];
    CSWCMiniGame* m_pMiniGame;                      // 0x264
};

// FUNCTION: SWKOTOR 0x00605E50
CSWCMiniGame* CSWCArea::RestartMiniGame()
{
    if (m_pMiniGame) {
        CSWCMiniGame* pMiniGame = new CSWCMiniGame(this);
        int nTrackPoints = m_pMiniGame->m_nTrackPoints;
        int i;
        for (i = 0; i < nTrackPoints; i++) {
            CSWMiniTrackPoint* pPoint = m_pMiniGame->GetTrackPoint(i);
            pMiniGame->AddTrackPoint(pPoint->m_cName, pPoint->m_vPos);
        }
        int nObstacles = m_pMiniGame->m_nObstacles;
        for (i = 0; i < nObstacles; i++) {
            CAurObject* pModel = m_pMiniGame->GetObstacle(i)->m_pModel;
            pMiniGame->AddObstacle(pModel->GetName(), pModel->GetPosition());
        }
        if (!pMiniGame->Load()) {
            return 0;
        }
        pMiniGame->SetCamera(m_pMiniGame->m_pCamera);
        delete m_pMiniGame;
        m_pMiniGame = pMiniGame;
        pMiniGame->Go();
    }
    return m_pMiniGame;
}

// Descriptive name (playable source): highlight toggle for every object in
// the area.
// FUNCTION: SWKOTOR 0x00605C40
void CSWCArea::HiliteAreaObjects(int bOn)
{
    for (int i = 0; i < m_aGameObjects.num; i++) {
        CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_aGameObjects[i]);
        if (!pGameObject) {
            continue;
        }
        CSWCObject* pObject = pGameObject->AsSWCObject();
        if (!pObject || !pObject->m_bHilitable) {
            continue;
        }
        switch (pObject->m_nObjectType) {
        case 5: {
            CSWCCreature* pCreature = pObject->AsSWCCreature();
            if (bOn) {
                if (pCreature == g_pAppManager->m_pClientExoApp->GetPlayerCreature()) {
                    pObject->Hilite(Vector(1.0f, 1.0f, 1.0f), 2, 0, 1);
                } else if (pCreature->m_nHiliteType == 1) {
                    pObject->Hilite(Vector(0.3f, 1.0f, 0.3f), 2, 0, 1);
                } else if (pCreature->m_nHiliteType == 0) {
                    pObject->Hilite(Vector(0.4f, 0.4f, 1.0f), 2, 0, 1);
                } else {
                    pObject->Hilite(Vector(1.0f, 0.1f, 0.1f), 2, 0, 1);
                }
            } else {
                pCreature->SetHilited(0);
                pObject->UnHilite(2);
            }
            break;
        }
        case 6:
            if (bOn) {
                pObject->Hilite(Vector(0.3f, 1.0f, 1.0f), 2, 0, 1);
            } else {
                pObject->UnHilite(2);
            }
            break;
        case 9:
            if (bOn) {
                pObject->Hilite(Vector(0.3f, 1.0f, 1.0f), 2, 0, 1);
            } else {
                pObject->UnHilite(2);
            }
            break;
        case 10:
            if (bOn) {
                pObject->Hilite(Vector(0.3f, 1.0f, 1.0f), 2, 0, 1);
            } else {
                pObject->UnHilite(2);
            }
            break;
        }
    }
}

// NONMATCH (33 diff insns): vtable/scratch register naming only (eax vs
// edx for the deleting-dtor calls, ecx for the new lightning object, the
// order of the [esi] reload around the rate multiply); block layout and
// frame match. A local for the lightning object changes nothing.
// FUNCTION: SWKOTOR 0x00604DB0
int CSWCArea::SetWeather(unsigned char nWeather, float fAmount)
{
    CExoString sAnimation;
    float fStart;
    float fEnd;
    int i;
    if (fAmount != 0.0f) {
        if (m_nWeather != nWeather) {
            for (i = 0; i < 6; i++) {
                if (m_pWeather[i]) {
                    delete m_pWeather[i];
                    m_pWeather[i] = 0;
                }
            }
            unsigned char j;
            switch (nWeather) {
            case 0:
                break;
            case 2:
                for (j = 0; j < 6; j++) {
                    m_pWeather[j] = NewCAurObject("fx_snow", "weather", 0, 0);
                    if (!m_pWeather[j]) {
                        return 0;
                    }
                }
                break;
            case 1:
                for (j = 0; j < 6; j++) {
                    m_pWeather[j] = NewCAurObject("fx_rain", "weather", 0, 0);
                    if (!m_pWeather[j]) {
                        return 0;
                    }
                }
                break;
            default:
                return 0;
            }
            for (i = 0; i < 6; i++) {
                if (m_pWeather[i]) {
                    m_pWeather[i]->SetScene(m_pScene);
                }
            }
            m_nWeather = nWeather;
        }
        if (m_bLightning > 0) {
            if (m_pLightning) {
                delete m_pLightning;
                m_pLightning = 0;
            }
            switch (m_nWeather) {
            case 1:
                m_pLightning = NewCAurObject("fx_lightning01", "weatherlight", 0, 0);
                if (m_pLightning) {
                    m_pLightning->SetScene(m_pScene);
                }
                break;
            case 2:
                m_pLightning = NewCAurObject("fx_lightning01", "weatherlight", 0, 0);
                if (m_pLightning) {
                    m_pLightning->SetScene(m_pScene);
                }
                break;
            default:
                return 0;
            }
        } else if (m_pLightning) {
            delete m_pLightning;
            m_pLightning = 0;
        }
    } else {
        for (i = 0; i < 6; i++) {
            if (m_pWeather[i]) {
                delete m_pWeather[i];
                m_pWeather[i] = 0;
            }
        }
        if (m_pLightning) {
            delete m_pLightning;
            m_pLightning = 0;
        }
        m_nWeather = 0;
    }
    switch (nWeather) {
    case 1:
        sAnimation = "rain";
        break;
    case 2:
        sAnimation = "snow";
        break;
    }
    for (i = 0; i < 6; i++) {
        if (m_pWeather[i]) {
            m_pWeather[i]->PlayAnimation(sAnimation.CStr(), 0.0f, 0x80, 0);
            m_pWeather[i]->PlayAnimation(sAnimation.CStr(), fAmount * 0.01f, 0, 0);
            m_pWeather[i]->GetEventTime(sAnimation.CStr(), &fStart, &fEnd);
            m_pWeather[i]->AnimateExternal(fEnd);
            m_pWeather[i]->PlayAnimation(sAnimation.CStr(), 0.0f, 0, 0);
        }
    }
    return 1;
}
