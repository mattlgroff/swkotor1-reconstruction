// CSWGuiMainInterface minimap: InitializeMap and DrawMap.
#include "k/sw_gui_main_interface.h"

// Server-side explored-area bitmap (matching/include/b/sws_area_map.h).
class CSWSAreaMap {
public:
    int GetPartyMemberMapPixel(int nIndex, int* pMapX, int* pMapY);     // 0x005791B0
    float GetMapRotateCCWFromWorldOrientation(Vector vOrientation);     // 0x00578ED0
    unsigned long* m_pBits;                         // 0x00
    unsigned long m_nWords;                         // 0x04
    char pad_0x8[0xc];
    float m_fInvDivisor;                            // 0x14
};

class CSWSModule {
public:
    char pad_0x0[0x30];
    CResRef m_cResRef;                              // 0x30
    char pad_0x40[0x218 - 0x40];
    CSWSAreaMap* m_pAreaMap;                        // 0x218
};

// FUNCTION: SWKOTOR 0x00687E40
// NONMATCH: register tie only (the original keeps the module in edi and
// bBuilt in ebp); local order, map-local and init-order shapes tried.
void CSWGuiMainInterface::InitializeMap()
{
    int bBuilt = 0;
    CSWSModule* pModule = g_pAppManager->m_pServerExoApp->GetModule();
    CExoString sModule;
    CExoString sMap;
    pModule->m_cResRef.CopyToString(sModule);
    sMap.Format("lbl_map%s", sModule.CStr());
    if (pModule->m_pAreaMap->m_nWords > 0) {
        float fScale = pModule->m_pAreaMap->m_fInvDivisor;
        {
            CResRef cMap(sMap);
            CSWGuiBorderParams* pBorder = &m_lblMapBorder.m_cBorder;
            pBorder->SetFillImage(cMap, 0);
            pBorder->m_nFlags &= ~3;
        }
        int nHeight = (int)(m_n6094 * fScale);
        int nWidth = (int)(m_n6090 * fScale);
        {
            CSWGuiExtent ext;
            ext.left = 0;
            ext.top = 0;
            ext.width = nWidth;
            ext.height = nHeight;
            m_lblMapBorder.SetExtent(ext);
        }
        bBuilt = 1;
    }
    CClientOptions* pOptions = g_pAppManager->m_pClientExoApp->GetOptions();
    if (pOptions) {
        m_lblMap.SetVisible(bBuilt && pOptions->m_bShowMap);
        m_lblMapOverlay.SetVisible(bBuilt && pOptions->m_bShowMap);
    }
}

class Quaternion {
public:
    float w, x, y, z;
    Vector GetYAxis() const;                        // 0x0044F420
};

class CSWCModuleCamera {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20();
    virtual Quaternion GetOrientation();            // 0x24
};

// FUNCTION: SWKOTOR 0x0068AB10
// NONMATCH: register tie only (the original keeps the area map in edi and
// the border label in ebx, and loads the map scale between the extent
// copy stores); local-order and pointer-local shapes tried.
void CSWGuiMainInterface::DrawMap(float fDelta)
{
    CClientOptions* pOptions = g_pAppManager->m_pClientExoApp->GetOptions();
    if (pOptions && !pOptions->m_bShowMap) {
        m_btnMapArrow.m_bVisible = 0;
        return;
    }
    CSWSAreaMap* pMap = g_pAppManager->m_pServerExoApp->GetModule()->m_pAreaMap;
    if (!pMap || !(pMap->m_nWords > 0)) {
        m_btnMapArrow.m_bVisible = 0;
        return;
    }
    if (m_lblMapBorder.m_cBorder.m_cFillImage == "") {
        InitializeMap();
    }
    CSWGuiExtent ext = m_lblMapBorder.m_extent;
    float fScale = pMap->m_fInvDivisor;
    int nX;
    int nY;
    pMap->GetPartyMemberMapPixel(0, &nX, &nY);
    ext.left = (int)(m_nMapWidth / 2 - nX * fScale);
    ext.top = (int)(m_nMapHeight / 2 - nY * fScale);
    m_lblMapBorder.SetExtent(ext);
    CSWCModuleCamera* pCamera = g_pAppManager->m_pClientExoApp->GetModuleCamera();
    m_lblMapOverlay.m_cBorder.m_fRotation =
        pMap->GetMapRotateCCWFromWorldOrientation(pCamera->GetOrientation().GetYAxis());
    AurGUIStartLayer();
    if (AurGUISetupViewport(m_nMapLeft, m_nMapTop, m_nMapWidth, m_nMapWidth, &g_vBlack, 0, m_fAlpha)) {
        m_lblMapBorder.Draw(fDelta);
        DrawFogOfWar(&ext);
        m_lblMapOverlay.Draw(fDelta);
        AurGUICloseViewport();
    }
    AurGUIStopLayer();
    m_btnMapArrow.m_bVisible = 1;
}
