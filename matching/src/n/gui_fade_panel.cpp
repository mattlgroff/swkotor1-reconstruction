// CSWGuiFade (swguiingame.o, vtable 0x0074FC60, 0x1B8 bytes): the global
// fade panel. Standalone layout; +0x4C alpha and +0x50 colour belong to the
// CSWGuiPanel base (SetAlpha 0x00404B00).
struct Vector { float x,y,z; Vector() {} Vector(float a,float b,float c) { x=a; y=b; z=c; } };
struct CSWGuiExtent { int left,top,width,height; };
class CSWGuiControl;
class CExoString {
public:
    char* m_sString; unsigned long m_nLength;
    CExoString(); CExoString(const char* s); ~CExoString();
    CExoString& operator=(const CExoString& s);
};
class CResRef { public: char m_resRef[16]; CResRef(const char* s); };
class CTlkTable {
public:
    CExoString GetSimpleString(unsigned long nStrRef); // 0x0041E8F0
    void ParseTokens(CExoString* pString); // 0x0041DD30
};
extern CTlkTable* g_pTlkTable;
class CSWGuiTextParams { public: char pad_0x0[0x58]; void SetText(const CExoString& s); };
class CSWGuiControl { public: virtual ~CSWGuiControl(); char pad_0x4[0x4c]; };
class CSWGuiLabel : public CSWGuiControl {
public: CSWGuiLabel(); virtual ~CSWGuiLabel(); char pad_0x50[0x98]; CSWGuiTextParams m_text;
};
class CSWGuiManager { public: char pad_0x0[0x6c]; short m_nWidth,m_nHeight; };
class CWorldTimer { public: unsigned __int64 GetSnapshotTime(); }; // 0x004ADC70
class CClientExoAppInternal { public: char pad_0x0[0x2c]; CWorldTimer* m_pWorldTimer; };
class CGuiInGame { public: int GetFadeOverridePanelVisible(); }; // 0x0062FAD0
class CClientExoApp {
public:
    char pad_0x0[4]; CClientExoAppInternal* m_pInternal;
    CGuiInGame* GetInGameGui(); CSWGuiManager* GetGuiManager();
};
class CAppManager { public: char pad_0x0[4]; CClientExoApp* m_pClient; };
extern CAppManager* g_pAppManager;

class CSWGuiPanel {
public:
    CSWGuiPanel(CSWGuiManager* pManager);
    virtual ~CSWGuiPanel();
    virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20();
    virtual void v24(); virtual void v28(); virtual void v2c(); virtual void v30();
    virtual void Draw(float fDelta);
    virtual CSWGuiControl* HitCheckMouse(int nX,int nY);
    CSWGuiExtent m_extent; int m_n14; CSWGuiManager* m_pManager;
    char pad_0x1c[0x4c-0x1c];
    float m_fAlpha; Vector m_vColor;
    char pad_0x5c[0x64-0x5c];
    void SetAlpha(float fAlpha); // 0x00404B00
    void StartLoadFromLayout(const CResRef& cLayout);
    void InitControl(CSWGuiControl* pControl,const CExoString& sName,int bAdd);
    void UnloadResource();
};

class CSWGuiFade : public CSWGuiPanel {
public:
    CSWGuiFade(CSWGuiManager* pManager);
    virtual void Draw(float fDelta);
    virtual CSWGuiControl* HitCheckMouse(int nX,int nY);
    float m_fFadeWait,m_fFadeLength,m_fFadeTime; int m_bFadeIn;
    CSWGuiLabel m_label;
    unsigned long m_nLastTime;
    void StartFade(int bFadeIn,float fWait,float fLength,const Vector& vColor);
    void SetMessage(unsigned long nStrRef);
    void ResetFade();
};

// FUNCTION: SWKOTOR 0x006244E0
void CSWGuiFade::StartFade(int bFadeIn,float fWait,float fLength,const Vector& vColor)
{
    m_vColor = vColor;
    m_extent.width = m_pManager->m_nWidth;
    m_extent.height = m_pManager->m_nHeight;
    if (bFadeIn) SetAlpha(1.0f); else SetAlpha(0.0f);
    m_fFadeWait = fWait;
    m_fFadeLength = fLength;
    m_bFadeIn = bFadeIn;
    m_fFadeTime = 0.1f;
    m_nLastTime = (unsigned long)(g_pAppManager->m_pClient->m_pInternal->m_pWorldTimer->GetSnapshotTime() / 1000);
}

// FUNCTION: SWKOTOR 0x00624570
void CSWGuiFade::Draw(float fDelta)
{
    unsigned long nNow = (unsigned long)(g_pAppManager->m_pClient->m_pInternal->m_pWorldTimer->GetSnapshotTime() / 1000);
    unsigned long nDiff = nNow - m_nLastTime;
    m_nLastTime = nNow;
    float fElapsed = nDiff * 0.001f;
    if (g_pAppManager->m_pClient->GetInGameGui() &&
        g_pAppManager->m_pClient->GetInGameGui()->GetFadeOverridePanelVisible())
        return;
    if (m_fFadeTime > 0.0f) {
        if (fElapsed < 5.0f) m_fFadeTime += fElapsed;
        if (m_fFadeTime > m_fFadeWait) {
            float fRatio = (m_fFadeTime - m_fFadeWait) / m_fFadeLength;
            float fAlpha;
            if (m_bFadeIn) {
                fRatio = 1.0f - fRatio;
                fAlpha = (0.0f > fRatio) ? 0.0f : fRatio;
            } else {
                fAlpha = (1.0f < fRatio) ? 1.0f : fRatio;
            }
            SetAlpha(fAlpha);
        }
    }
    CSWGuiPanel::Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x006246A0
CSWGuiControl* CSWGuiFade::HitCheckMouse(int nX,int nY)
{
    if (m_fAlpha > 0.001f) return CSWGuiPanel::HitCheckMouse(nX,nY);
    return 0;
}

// FUNCTION: SWKOTOR 0x006246C0
void CSWGuiFade::SetMessage(unsigned long nStrRef)
{
    CExoString sText;
    if (nStrRef) {
        sText = g_pTlkTable->GetSimpleString(nStrRef);
        g_pTlkTable->ParseTokens(&sText);
        m_label.m_text.SetText(sText);
    } else {
        CExoString sEmpty("");
        m_label.m_text.SetText(sEmpty);
    }
}

// NONMATCH: the original zeroes edi first and copies the colour from it while
// still materialising a dead 12-byte Vector temporary; ctor, POD, named-local
// and const-reference setter shapes all store then reload the temporary.
// FUNCTION: SWKOTOR 0x006247A0
void CSWGuiFade::ResetFade()
{
    m_vColor = Vector(0.0f,0.0f,0.0f);
    m_fAlpha = 0.0f;
    SetMessage(0);
    m_fFadeWait = 0.0f;
    m_fFadeLength = 0.0f;
    m_fFadeTime = 0.0f;
    m_bFadeIn = 0;
    m_nLastTime = (unsigned long)(g_pAppManager->m_pClient->m_pInternal->m_pWorldTimer->GetSnapshotTime() / 1000);
}

// FUNCTION: SWKOTOR 0x00624810
CSWGuiFade::CSWGuiFade(CSWGuiManager* pManager) : CSWGuiPanel(pManager)
{
    if (!m_pManager) m_pManager = g_pAppManager->m_pClient->GetGuiManager();
    {
        CResRef cLayout("fade");
        StartLoadFromLayout(cLayout);
    }
    {
        CExoString sName("LBL_MSG");
        InitControl(&m_label,sName,1);
    }
    UnloadResource();
    m_extent.top = 0;
    m_extent.left = 0;
    m_extent.width = m_pManager->m_nWidth;
    m_extent.height = m_pManager->m_nHeight;
    ResetFade();
}
