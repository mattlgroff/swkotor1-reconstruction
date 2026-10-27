// CSWGuiInGameMap::Draw / HitCheckMouse (swguiingamemap.o): greys out the unavailable return
// and party buttons, draws the panel, then the map image and the map hider
// inside a viewport at the map label's extent.

class Vector {
public:
    float x, y, z;
};

struct CSWGuiExtent {
    int left;
    int top;
    int width;
    int height;
};

void AurGUIStartLayer();                            // 0x004591B0
int AurGUISetupViewport(int x, int y, int w, int h, Vector* pColor, int n, float fAlpha); // 0x004592F0
void AurGUIStopLayer();                             // 0x004592B0
void AurGUICloseViewport();                         // 0x00459580
extern Vector g_vGuiViewportColor;                  // 0x0078D3D8
extern Vector g_vGuiDisabledTextColor;              // 0x007A2408 (descriptive)
extern Vector g_vGuiDisabledBorderColor;            // 0x007A2384 (descriptive)

class CSWGuiManager {
public:
    char pad_0x0[0x6c];
    short m_nWidth;                                 // 0x6c
    short m_nHeight;                                // 0x6e
};

class CSWGuiControl;

class CSWGuiTextParams {
public:
    void SetColor(const Vector& vColor);            // 0x00414E10
    char pad_0x0[0x58];
};

// 0x1c4 bytes.
class CSWGuiButton {
public:
    virtual void v00();
    char pad_0x4[0x90 - 0x4];
    Vector m_vBorderColor;                          // 0x90 (descriptive)
    char pad_0x9c[0x16c - 0x9c];
    CSWGuiTextParams m_cText;                       // 0x16c
};

// 0x54 bytes.
class CSWGuiImage {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual void Draw(float fDelta);                // 0x0c
    char pad_0x4[0x50];
};

// 0x248 bytes.
class CSWGuiMapHider {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34();
    virtual void Draw(float fDelta);                // 0x38 (0x006943D0)
    virtual void v3C(); virtual void v40();
    virtual CSWGuiControl* HitCheckMouse(int x, int y);     // 0x44 (0x00693300)
    char pad_0x4[0x244];
};

class CSWGuiPanel {
public:
    virtual ~CSWGuiPanel();                         // 0x00 (0x0040CF70)
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30();
    virtual void Draw(float fDelta);                // 0x34 (0x0040B760)
    virtual void v38(); virtual void v3C();
    virtual CSWGuiControl* HitCheckMouse(int x, int y);     // 0x40 (0x0040B690)

    char pad_0x4[0x18 - 0x4];
    CSWGuiManager* m_pManager;                      // 0x18
    char pad_0x1c[0x44 - 0x1c];
    unsigned long m_nFlags;                         // 0x44
    char pad_0x48[0x64 - 0x48];
};

// 0x140 bytes.
class CSWGuiLabel {
public:
    virtual void v00();
    CSWGuiExtent m_extent;                          // 0x04
    char pad_0x14[0x140 - 0x14];
};

class CSWGuiInGameMap : public CSWGuiPanel {
public:
    virtual void Draw(float fDelta);
    virtual CSWGuiControl* HitCheckMouse(int x, int y);

    CSWGuiLabel m_lblMap;                           // 0x64 (descriptive)
    char pad_0x1a4[0x564 - 0x1a4];
    CSWGuiButton m_btnReturn;                       // 0x564 (descriptive)
    CSWGuiButton m_btnParty;                        // 0x728 (descriptive)
    char pad_0x8ec[0xe38 - 0x8ec];
    CSWGuiMapHider m_cMapHider;                     // 0xe38
    CSWGuiImage m_cMapImage;                        // 0x1080
    char pad_0x10d4[0x10d8 - 0x10d4];
    unsigned char m_nMapFlags;                      // 0x10d8 (descriptive)
};

// FUNCTION: SWKOTOR 0x00692810
void CSWGuiInGameMap::Draw(float fDelta)
{
    if (!(m_nMapFlags & 2)) {
        m_btnReturn.m_cText.SetColor(g_vGuiDisabledTextColor);
        m_btnReturn.m_vBorderColor = g_vGuiDisabledBorderColor;
    }
    if (!(m_nMapFlags & 1)) {
        m_btnParty.m_cText.SetColor(g_vGuiDisabledTextColor);
        m_btnParty.m_vBorderColor = g_vGuiDisabledBorderColor;
    }
    CSWGuiExtent ext = m_lblMap.m_extent;
    CSWGuiPanel::Draw(fDelta);
    ext.left += (m_pManager->m_nWidth - 640) / 2;
    ext.top += (m_pManager->m_nHeight - 480) / 2;
    AurGUIStartLayer();
    if (AurGUISetupViewport(ext.left, ext.top, ext.width, ext.height, &g_vGuiViewportColor, 0, 1.0f)) {
        m_cMapImage.Draw(fDelta);
        m_cMapHider.Draw(fDelta);
        AurGUICloseViewport();
    }
    AurGUIStopLayer();
}

// FUNCTION: SWKOTOR 0x00692930
CSWGuiControl* CSWGuiInGameMap::HitCheckMouse(int x, int y)
{
    CSWGuiControl* pHit = CSWGuiPanel::HitCheckMouse(x, y);
    if (pHit) {
        return pHit;
    }
    if (!(m_nFlags & 1)) {
        x -= (m_pManager->m_nWidth - 640) / 2;
        y -= (m_pManager->m_nHeight - 480) / 2;
    }
    CSWGuiExtent ext = m_lblMap.m_extent;
    return m_cMapHider.HitCheckMouse(x - ext.left, y - ext.top);
}
