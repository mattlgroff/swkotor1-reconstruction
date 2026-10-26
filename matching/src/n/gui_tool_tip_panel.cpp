// CSWGuiToolTipPanel (swguiingame.o, vtable 0x00750030): layout selection and
// mouse-relative positioning. Standalone layout; label at +0x64.
struct CSWGuiExtent { int left,top,width,height; };
class CSWGuiControl;
class CExoString { public: char* m_sString; unsigned long m_nLength; CExoString(const char* s); ~CExoString(); };
class CResRef { public: char m_resRef[16]; CResRef(const char* s); };
class CSWGuiTextLayout { public: CSWGuiExtent GetExtent(); }; // 0x00414F10, descriptive
class CSWGuiTextParams { public: char pad_0x0[0x50]; CSWGuiTextLayout* m_pLayout; char pad_0x54[4]; };
class CSWGuiControl {
public:
    virtual ~CSWGuiControl();
    virtual void SetExtent(const CSWGuiExtent& extent);
    CSWGuiExtent m_extent; char pad_0x14[0x3c];
};
class CSWGuiLabel : public CSWGuiControl {
public: CSWGuiLabel(); virtual ~CSWGuiLabel(); char pad_0x50[0x98]; CSWGuiTextParams m_text;
};
class CSWGuiManager { public: int m_nMouseX,m_nMouseY; };
extern CSWGuiManager* g_pGuiManager;
extern int g_nScreenWidth;  // 0x0078D1D4
extern int g_nScreenHeight; // 0x0078D1D8
class CSWGuiPanel {
public:
    CSWGuiPanel(CSWGuiManager* pManager); // 0x0040B570
    virtual ~CSWGuiPanel();
    CSWGuiExtent m_extent; int m_n14; CSWGuiManager* m_pManager;
    char pad_0x1c[0x44-0x1c]; unsigned long m_nFlags; char pad_0x48[0x64-0x48];
    void StartLoadFromLayout(const CResRef& cLayout); // 0x0040A680
    void InitControl(CSWGuiControl* pControl,const CExoString& sName,int bAdd); // 0x0040B930
    void UnloadResource(); // 0x0040B8F0
};
class CSWGuiToolTipPanel : public CSWGuiPanel {
public:
    CSWGuiToolTipPanel(CSWGuiManager* pManager);
    CSWGuiLabel m_label;
    void PositionToolTip();
};

// FUNCTION: SWKOTOR 0x00624AF0
void CSWGuiToolTipPanel::PositionToolTip()
{
    CSWGuiExtent extent;
    extent.left = extent.top = extent.width = extent.height = 0;
    if (m_label.m_text.m_pLayout) {
        extent = m_label.m_text.m_pLayout->GetExtent();
        extent.height += 8;
        extent.width += 8;
        int nX = g_pGuiManager->m_nMouseX + 0xf;
        int nY = g_pGuiManager->m_nMouseY + 0xf;
        if (nX + extent.width >= g_nScreenWidth) nX = g_nScreenWidth - extent.width - 2;
        if (nY + extent.height >= g_nScreenHeight) nY = g_nScreenHeight - extent.height - 2;
        extent.top = nY;
        extent.left = nX;
        m_label.SetExtent(extent);
    }
}

// FUNCTION: SWKOTOR 0x006277C0
CSWGuiToolTipPanel::CSWGuiToolTipPanel(CSWGuiManager* pManager) : CSWGuiPanel(pManager)
{
    if (g_nScreenWidth == 1600) {
        CResRef cLayout("tooltip16X12");
        StartLoadFromLayout(cLayout);
    }
    if (g_nScreenWidth == 1280) {
        if (g_nScreenHeight == 960) {
            CResRef cLayout("tooltip12X9");
            StartLoadFromLayout(cLayout);
        } else if (g_nScreenHeight == 1024) {
            CResRef cLayout("tooltip12x10");
            StartLoadFromLayout(cLayout);
        }
    } else if (g_nScreenWidth == 1024) {
        CResRef cLayout("tooltip10X8");
        StartLoadFromLayout(cLayout);
    } else if (g_nScreenWidth == 800) {
        CResRef cLayout("tooltip8X6");
        StartLoadFromLayout(cLayout);
    } else {
        CResRef cLayout("tooltip6X4");
        StartLoadFromLayout(cLayout);
    }
    m_nFlags |= 1;
    {
        CExoString sName("tooltip");
        InitControl(&m_label,sName,1);
    }
    UnloadResource();
}
