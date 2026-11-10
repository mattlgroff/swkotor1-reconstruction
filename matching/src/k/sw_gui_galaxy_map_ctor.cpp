// CSWGuiInGameGalaxyMap constructor (0x2550 bytes, vtable 0x00754910).
// Member offsets as sw_gui_galaxy_map.cpp; control classes as
// sw_gui_in_game_map_ctor.cpp.

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    char* m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    CResRef(const char* s);                         // 0x00406D80
    CResRef(const CExoString& s);                   // 0x00406D60
    CResRef operator=(const CExoString& s);         // 0x004061F0
    char m_resRef[16];
};

struct CSWGuiExtent {
    CSWGuiExtent(int nLeft, int nTop, int nWidth, int nHeight)
    {
        left = nLeft;
        top = nTop;
        width = nWidth;
        height = nHeight;
    }
    int left;
    int top;
    int width;
    int height;
};

class CSWGuiControl;
class CSWGuiPanel;
class CSWGuiManager;

class Vector {
public:
    Vector(float fX, float fY, float fZ)
    {
        x = fX;
        y = fY;
        z = fZ;
    }
    float x, y, z;
};

class Quaternion {
public:
    Quaternion(float fX, float fY, float fZ, float fW)
    {
        x = fX;
        y = fY;
        z = fZ;
        w = fW;
    }
    float x, y, z, w;
};

class CSWGuiBorderParams {
public:
    void SetFillImage(const CResRef& cResRef, int bForce);  // 0x00414C00
    char pad_0x0[0x54];
};

class CSWGuiBorder {
public:
    virtual ~CSWGuiBorder();
    char pad_0x4[0x10];
    CSWGuiBorderParams m_cParams;                   // 0x14
    char pad_0x68[0x74 - 0x68];
};

class CAurObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14();
    virtual void PlayAnimation(const char* sAnim, float fSpeed, int a, int b);  // 0x18
};

class CAurCamera {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40();
    virtual void SetFieldOfView(float fFov);        // 0x44 (descriptive)
    virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70();
    virtual void AttachToObject(CAurObject* pObject, const char* sNode, int b);  // 0x74
};

class CAurScene {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void SetRoom(const char* sRoom, const Vector& vPosition,
                         const Quaternion& qOrientation);  // 0x70 (descriptive)
};

// 0x34 bytes.
class CSWGuiScene {
public:
    CSWGuiScene();                                  // 0x004174B0
    ~CSWGuiScene();
    CAurObject* AddModel(const CExoString& sModel, int n);  // 0x00417620
    char pad_0x0[0x14];
    CAurScene* m_pScene;                            // 0x14
    CAurCamera* m_pCamera;                          // 0x18
    char pad_0x1c[0x34 - 0x1c];
};

class CSWGuiObject {
public:
    virtual ~CSWGuiObject();
    virtual void SetExtent(const CSWGuiExtent& extent);  // 0x04
    char pad_0x4[0x10];
};

class CSWGuiControl : public CSWGuiObject {
public:
    CSWGuiControl();                                // 0x0041AA80
    virtual ~CSWGuiControl();                       // 0x00419A20
    void AddEvent(int nEvent, CSWGuiPanel* pObject,
                  void (CSWGuiPanel::*pfnCallback)(CSWGuiControl*));   // 0x0041AB20
    void SetNavigable(int b) { m_bNavigable = b; }
    void SetFlag1(int b) { m_bFlag1 = b; }

    char pad_0x14[0x44 - 0x14];
    unsigned long m_bFlag0 : 1;                     // 0x44 bit 0
    unsigned long m_bFlag1 : 1;                     //      bit 1
    unsigned long m_bNavigable : 1;                 //      bit 2
    char pad_0x48[0x58 - 0x48];
    int m_nId;                                      // 0x58
};

// 0x140 bytes, vtable 0x0073e5b8.
class CSWGuiLabel : public CSWGuiControl {
public:
    CSWGuiLabel();                                  // 0x0041ACD0
    virtual ~CSWGuiLabel();                         // 0x00419D10
    char pad_0x5c[0x140 - 0x5c];
};

// 0x1c4 bytes, vtable 0x0073e658.
class CSWGuiButton : public CSWGuiControl {
public:
    CSWGuiButton();                                 // 0x0041C0F0
    virtual ~CSWGuiButton();                        // 0x00419DA0
    virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void SetActive(int b);                  // 0x40
    char pad_0x5c[0x6c - 0x5c];
    CSWGuiBorder m_cBorder;                         // 0x6c
    CSWGuiBorder m_cHilight;                        // 0xe0
    char pad_0x154[0x1c4 - 0x154];
};

// 0x248 bytes, vtable 0x00754738.
class CSWGuiMapHider : public CSWGuiControl {
public:
    CSWGuiMapHider();                               // 0x00693F60
    virtual ~CSWGuiMapHider();                      // 0x006942D0
    char pad_0x5c[0x248 - 0x5c];
};

// 0x54 bytes, vtable 0x0073e390.
class CSWGuiImage {
public:
    CSWGuiImage();                                  // 0x00415A30
    virtual ~CSWGuiImage();                         // 0x00415AD0
    virtual void SetExtent(const CSWGuiExtent& extent);  // 0x04
    char pad_0x4[0x54 - 0x4];
};

// 0x90 bytes, vtable 0x00752e30.
class CSWGui3DSceneView : public CSWGuiControl {
public:
    virtual ~CSWGui3DSceneView();                   // 0x0067AC50
    CSWGuiScene m_cScene;                           // 0x5c
};

class C2DA {
public:
    int GetCExoStringEntry(int nRow, const CExoString& sColumn, CExoString* pValue);  // 0x00413270
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);  // 0x00413660
};

class C2DAs {
public:
    char pad_0x0[0x104];
    C2DA* m_pPlanetary;                             // 0x104
    C2DA* GetPlanetary() { return m_pPlanetary; }
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    C2DAs* m_p2DArrays;                             // 0xb8
    C2DAs* Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules* g_pRules;                          // 0x007A3A28
extern int g_bDo3DGui;                              // 0x0078D1E4

struct CSWGuiGalaxyPlanet {
    int m_nNameStrRef;                              // 0x00
    int m_nDescriptionStrRef;                       // 0x04
    CResRef m_cModel;                               // 0x08
};

class CSWGuiPanel : public CSWGuiObject {
public:
    CSWGuiPanel(CSWGuiManager* pManager);           // 0x0040B570
    virtual ~CSWGuiPanel();                         // 0x0040CF70
    virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void OnAButtonPressed(CSWGuiControl* pControl);  // 0x50 (descriptive)
    virtual void OnBButtonPressed(CSWGuiControl* pControl);  // 0x54 (descriptive)
    virtual void OnXButtonPressed(CSWGuiControl* pControl);  // 0x58 (descriptive)

    void StartLoadFromLayout(const CResRef& cResRef);   // 0x0040A680
    void UnloadResource();                          // 0x0040B8F0
    void InitControl(CSWGuiControl* pControl, const CExoString& sName, int bAdd);  // 0x0040B930
    void AddControl(CSWGuiControl* pControl);       // 0x0040B9A0

    char pad_0x14[0x48 - 0x14];
    unsigned char m_nResolution;                    // 0x48
    char pad_0x49[0x64 - 0x49];
};

class CSWGuiInGameGalaxyMap : public CSWGuiPanel {
public:
    CSWGuiInGameGalaxyMap(CSWGuiManager* pManager);
    virtual ~CSWGuiInGameGalaxyMap();               // 0x006958C0
    void OnPlanetClicked(CSWGuiControl* pControl);  // 0x006935A0

    CSWGuiButton m_aPlanetButtons[16];              // 0x64
    CSWGuiLabel m_lblPlanetName;                    // 0x1ca4
    CSWGuiLabel m_lblDescription;                   // 0x1de4
    CSWGuiButton m_btnAccept;                       // 0x1f24
    CSWGuiButton m_btnBack;                         // 0x20e8
    CSWGui3DSceneView m_cPlanetDisplay;             // 0x22ac
    CSWGui3DSceneView m_cPlanetModel;               // 0x233c
    CSWGuiGalaxyPlanet m_aPlanets[16];              // 0x23cc
    int m_nDisplayedPlanet;                         // 0x254c (descriptive)
};

typedef char KGalaxyMapSize[(sizeof(CSWGuiInGameGalaxyMap) == 0x2550) ? 1 : -1];

typedef void (CSWGuiPanel::*CSWGuiCallback)(CSWGuiControl*);

// FUNCTION: SWKOTOR 0x00695180
CSWGuiInGameGalaxyMap::CSWGuiInGameGalaxyMap(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    CExoString sIcon;
    CExoString sModel;
    int nName;
    CExoString sTag;
    int nDescription;
    m_nDisplayedPlanet = -1;
    {
        CResRef cLayout("galaxymap");
        StartLoadFromLayout(cLayout);
    }
    {
        CExoString sName("LBL_DESC");
        InitControl(&m_lblDescription, sName, 1);
    }
    {
        CExoString sName("LBL_PLANETNAME");
        InitControl(&m_lblPlanetName, sName, 1);
    }
    {
        CExoString sName("3D_PlanetModel");
        InitControl(&m_cPlanetModel, sName, 1);
    }
    {
        CExoString sName("3D_PlanetDisplay");
        InitControl(&m_cPlanetDisplay, sName, 1);
    }
    {
        CExoString sName("BTN_ACCEPT");
        InitControl(&m_btnAccept, sName, 1);
    }
    {
        CExoString sName("BTN_BACK");
        InitControl(&m_btnBack, sName, 1);
    }
    int i;
    for (i = 0; i < 16; i++) {
        if (g_pRules->Get2DArrays()->GetPlanetary()->GetCExoStringEntry(i, "GUITag", &sTag)) {
            InitControl(&m_aPlanetButtons[i], sTag, 1);
            m_aPlanetButtons[i].SetNavigable(0);
            m_aPlanetButtons[i].AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiInGameGalaxyMap::OnPlanetClicked);
            m_aPlanetButtons[i].m_nId = i;
        }
    }
    UnloadResource();
    m_btnBack.SetNavigable(0);
    m_btnBack.AddEvent(0x27, this, &CSWGuiPanel::OnBButtonPressed);
    m_btnAccept.SetNavigable(0);
    m_btnAccept.AddEvent(0x27, this, &CSWGuiPanel::OnAButtonPressed);
    CAurCamera* pCamera = m_cPlanetDisplay.m_cScene.m_pCamera;
    CAurScene* pScene = m_cPlanetDisplay.m_cScene.m_pScene;
    if (g_bDo3DGui) {
        pScene->SetRoom("gui3D_room", Vector(0.0f, 0.0f, 0.0f),
                                                    Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
        CAurObject* pPlanet = m_cPlanetDisplay.m_cScene.AddModel("galaxy", -1);
        if (pPlanet) {
            pPlanet->PlayAnimation("default", 1.0f, 0, 0);
            pCamera->AttachToObject(pPlanet, "camerahook", 0);
            pCamera->SetFieldOfView(22.726f);
        }
    }
    pScene = m_cPlanetModel.m_cScene.m_pScene;
    pScene->SetRoom("gui3D_room", Vector(0.0f, 0.0f, 0.0f), Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
    for (i = 0; i < 16; i++) {
        g_pRules->Get2DArrays()->GetPlanetary()->GetCExoStringEntry(i, "Icon", &sIcon);
        g_pRules->Get2DArrays()->GetPlanetary()->GetCExoStringEntry(i, "Model", &sModel);
        g_pRules->Get2DArrays()->GetPlanetary()->GetINTEntry(i, "Name", &nName);
        g_pRules->Get2DArrays()->GetPlanetary()->GetINTEntry(i, "Description", &nDescription);
        m_aPlanetButtons[i].m_cBorder.m_cParams.SetFillImage(sIcon, 0);
        m_aPlanetButtons[i].m_cHilight.m_cParams.SetFillImage(sIcon, 0);
        m_aPlanetButtons[i].SetActive(0);
        m_aPlanets[i].m_nNameStrRef = nName;
        m_aPlanets[i].m_nDescriptionStrRef = nDescription;
        m_aPlanets[i].m_cModel = sModel;
    }
    m_nResolution = 3;
}
