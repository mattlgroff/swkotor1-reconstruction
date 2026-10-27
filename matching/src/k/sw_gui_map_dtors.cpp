// Compiler-generated destructors of the in-game map and galaxy map panels
// (swguiingamemap.o), declared with sized member classes. The destructors
// store no vtable (nothing virtual is called), hence novtable.

class CSWGuiPanel {
public:
    virtual ~CSWGuiPanel();                         // 0x0040CF70
    char pad_0x4[0x60];
};

// 0x140 bytes.
class CSWGuiLabel {
public:
    virtual ~CSWGuiLabel();                         // 0x00419D10
    char pad_0x4[0x13c];
};

// 0x1c4 bytes.
class CSWGuiButton {
public:
    virtual ~CSWGuiButton();                        // 0x00419DA0
    char pad_0x4[0x1c0];
};

// 0x54 bytes.
class CSWGuiImage {
public:
    ~CSWGuiImage();                                 // 0x00415AD0
    virtual void v00();
    char pad_0x4[0x50];
};

// 0x90 bytes.
class CSWGui3DSceneView {
public:
    ~CSWGui3DSceneView();                           // 0x0067AC50
    virtual void v00();
    char pad_0x4[0x8c];
};

// 0x248 bytes.
class CSWGuiMapHider {
public:
    virtual ~CSWGuiMapHider();                      // 0x006942D0
    char pad_0x4[0x244];
};

// Planet record: name / description strrefs and model resref (descriptive).
struct CSWGuiGalaxyPlanet {
    unsigned long m_nNameStrRef;
    unsigned long m_nDescriptionStrRef;
    char m_cModel[16];
};

class __declspec(novtable) CSWGuiInGameMap : public CSWGuiPanel {
public:
    ~CSWGuiInGameMap();

    CSWGuiLabel m_lblTitle;                         // 0x64
    CSWGuiLabel m_lbl1A4;                           // 0x1a4
    CSWGuiLabel m_lbl2E4;                           // 0x2e4
    CSWGuiLabel m_lblMapNote;                       // 0x424
    CSWGuiButton m_btn564;                          // 0x564
    CSWGuiButton m_btn728;                          // 0x728
    CSWGuiButton m_btn8EC;                          // 0x8ec
    CSWGuiButton m_btnAB0;                          // 0xab0
    CSWGuiButton m_btnC74;                          // 0xc74
    CSWGuiMapHider m_cMapHider;                     // 0xe38
    CSWGuiImage m_cMapImage;                        // 0x1080
    char pad_0x10d4[0x8];
};

class __declspec(novtable) CSWGuiInGameGalaxyMap : public CSWGuiPanel {
public:
    ~CSWGuiInGameGalaxyMap();

    CSWGuiButton m_aPlanetButtons[16];              // 0x64
    CSWGuiLabel m_lblPlanetName;                    // 0x1ca4
    CSWGuiLabel m_lblPlanetDescription;             // 0x1de4
    CSWGuiButton m_btn1F24;                         // 0x1f24
    CSWGuiButton m_btn20E8;                         // 0x20e8
    CSWGui3DSceneView m_cScene22AC;                 // 0x22ac
    CSWGui3DSceneView m_cPlanetScene;               // 0x233c
    CSWGuiGalaxyPlanet m_aPlanets[16];              // 0x23cc
    int m_nDisplayedPlanet;                         // 0x254c
};

// FUNCTION: SWKOTOR 0x00695BF0
CSWGuiInGameMap::~CSWGuiInGameMap()
{
}

// FUNCTION: SWKOTOR 0x006958C0
CSWGuiInGameGalaxyMap::~CSWGuiInGameGalaxyMap()
{
}
