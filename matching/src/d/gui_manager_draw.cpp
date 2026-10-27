// CSWGuiManager::DrawPCBG / DrawSafeRegion: the full-screen background
// label and the debug safe-area frame (a panel with one bordered label),
// built on first use. The panel and label are declared here with just the
// slots and members they touch; they are the manager's CSWGuiPanel
// (0x0040b570, 0x64 bytes) and a CSWGuiLabel (0x0041acd0, 0x140 bytes).
#include "d/gui_parts.h"

void operator delete(void* p);
void* operator new(unsigned int n);

class CSWGuiSafeControl {
public:
    virtual ~CSWGuiSafeControl();                   // 0x00
    virtual void SetExtent(const CSWGuiExtent& extent); // 0x04
};

// vtable 0x0073e5b8 (CSWGuiLabel).
class CSWGuiSafeLabel : public CSWGuiSafeControl {
public:
    CSWGuiSafeLabel();                              // 0x0041acd0
    virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual void Draw(float fDelta);                // 0x38
    virtual void v3c(); virtual void v40(); virtual void v44();
    virtual void v48(); virtual void v4c(); virtual void v50(); virtual void v54();
    virtual void v58(); virtual void v5c(); virtual void v60(); virtual void v64();
    virtual void v68(); virtual void v6c(); virtual void v70(); virtual void v74();
    virtual void v78(); virtual void v7c(); virtual void v80(); virtual void v84();
    virtual void v88(); virtual void v8c(); virtual void v90(); virtual void v94();
    virtual void v98();
    virtual void SetStyle(const CSWGuiExtent& extent, CSWGuiTextParams* pText, CSWGuiBorderParams* pBorder,
                          float fScale);            // 0x9c
    char pad_0x04[0x58];
    CSWGuiBorder m_cBorder;                         // 0x5c
    char pad_0xd0[0x70];
};

// vtable 0x0073e010 (CSWGuiPanel).
class CSWGuiSafePanel {
public:
    CSWGuiSafePanel(CSWGuiManager* pManager);       // 0x0040b570
    virtual ~CSWGuiSafePanel();                     // 0x00
    virtual void SetExtent(const CSWGuiExtent& extent); // 0x04
    virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30();
    virtual void Draw(float fDelta);                // 0x34
    void AddControl(CSWGuiSafeControl* pControl);   // 0x0040b9a0
    char pad_0x04[0x4c];
    Vector m_vColor;                                // 0x50
    char pad_0x5c[0x08];
};

// AurGUI layer helpers (cdecl).
void AurGUIStartLayer();                            // 0x004591b0
int AurGUISetupViewport(int x, int y, int w, int h, Vector* pColor, int n, float fAlpha); // 0x004592f0
void AurGUIStopLayer();                             // 0x004592b0
void AurGUICloseViewport();                         // 0x00459580

// FUNCTION: SWKOTOR 0x0040ae80
void CSWGuiManager::DrawPCBG(float fDelta)
{
    AurGUIStartLayer();
    if (AurGUISetupViewport(0, 0, m_nWidth, m_nHeight, &Vector(0.0f, 0.0f, 0.0f), 0, 1.0f)) {
        if (!m_pBackground) {
            m_pBackground = new CSWGuiSafeLabel;
            CResRef cFill(m_sResolution);
            m_pBackground->SetStyle(CSWGuiExtent(0, 0, m_nWidth, m_nHeight), &CSWGuiTextParams(),
                                    &CSWGuiBorderParams(CResRef(), CResRef(), cFill, 0, 0, 1.0f,
                                                        Vector(1.0f, 1.0f, 1.0f), 0),
                                    1.0f);
        }
        {
            CResRef cImage(m_sResolution);
            m_pBackground->m_cBorder.m_cParams.SetFillImage(cImage, 0);
        }
        m_pBackground->m_cBorder.m_cParams.SetStyle(2);
        m_pBackground->Draw(fDelta);
        AurGUICloseViewport();
    }
    AurGUIStopLayer();
}

// NONMATCH: 3 insns; the original schedules `add eax, 0x50` (&m_vColor)
// after the first component of the Vector temporary is spilled.
// FUNCTION: SWKOTOR 0x0040bec0
void CSWGuiManager::DrawSafeRegion(float fDelta)
{
    if (!m_pSafePanel) {
        m_pSafePanel = new CSWGuiSafePanel(this);
        m_pSafeLabel = new CSWGuiSafeLabel;
        m_pSafePanel->AddControl(m_pSafeLabel);
        m_pSafePanel->m_vColor = Vector(-1.0f, -1.0f, -1.0f);
        CResRef cEdge("border1b");
        CResRef cCorner("border2b");
        m_pSafeLabel->SetStyle(CSWGuiExtent(), &CSWGuiTextParams(),
                               &CSWGuiBorderParams(cCorner, cEdge, CResRef(), 0, 0, 1.0f,
                                                   Vector(1.0f, 1.0f, 1.0f), 0),
                               1.0f);
    }
    {
        CSWGuiExtent cFull(0, 0, m_nWidth, m_nHeight);
        m_pSafePanel->SetExtent(cFull);
        int nX = 0x30 - m_pSafeLabel->m_cBorder.m_cParams.GetCornerSize();
        int nY = 0x24 - m_pSafeLabel->m_cBorder.m_cParams.GetCornerSize();
        CSWGuiExtent cSafe;
        cSafe.left = nX;
        cSafe.top = nY;
        cSafe.width = m_nWidth - 2 * nX;
        cSafe.height = m_nHeight - 2 * nY;
        m_pSafeLabel->SetExtent(cSafe);
    }
    m_pSafePanel->Draw(fDelta);
}
