// CSWGuiInGameAreaTransition (layout "areatransition", vtable 0x00757508):
// the banner naming the area a door or trigger leads to. The destructor is
// in sw_gui_panel_dtors_novtable.cpp.
#include "l/sw_gui_upgrade.h"

class CSWCTrigger {
public:
    char pad_0x0[0x140];
    CExoString m_sTransitionName;                   // 0x140 (descriptive)
};

class CSWGuiInGameAreaTransition : public CSWGuiPanel {
public:
    CSWGuiInGameAreaTransition(CSWGuiManager* pManager);   // 0x006C7D50
    virtual ~CSWGuiInGameAreaTransition();          // 0x006C8190
    virtual void Draw(float fDelta);                // 0x34 (0x006C8050)
    void SetTransitionObject(unsigned long oidObject, const Vector& vPosition);  // 0x006C7EC0

    CSWGuiLabel m_lblTextBG;                        // 0x64
    CSWGuiLabel m_lblIcon;                          // 0x1a4
    CSWGuiLabel m_lblDescription;                   // 0x2e4
    Vector m_vPosition;                             // 0x424
    unsigned long m_oidObject;                      // 0x430
    int m_nSavedTop;                                // 0x434 (descriptive)
};

// FUNCTION: SWKOTOR 0x006C7D20
CExoString CSWCDoor::GetTransitionName()
{
    return m_sTransitionName;
}

// FUNCTION: SWKOTOR 0x006C7D50
CSWGuiInGameAreaTransition::CSWGuiInGameAreaTransition(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    m_vPosition.x = 0.0f;
    m_vPosition.y = 0.0f;
    m_vPosition.z = 0.0f;
    StartLoadFromLayout("areatransition");
    InitControl(&m_lblTextBG, "LBL_TEXTBG", 1);
    InitControl(&m_lblIcon, "LBL_ICON", 1);
    InitControl(&m_lblDescription, "LBL_DESCRIPTION", 1);
    UnloadResource();
    m_oidObject = 0x7f000000;
    m_nFlags = (m_nFlags & ~0x40) | 0x20;
    m_nSavedTop = m_extent.top;
}

// FUNCTION: SWKOTOR 0x006C7EC0
void CSWGuiInGameAreaTransition::SetTransitionObject(unsigned long oidObject, const Vector& vPosition)
{
    if (m_oidObject != oidObject) {
        CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(oidObject);
        if (pGameObject) {
            CSWCObject* pObject = pGameObject->AsSWCObject();
            if (pObject) {
                CExoString sName;
                if (pObject->m_nObjectType == 0xa) {
                    sName = ((CGameObject*)pObject)->AsSWCDoor()->GetTransitionName();
                } else if (pObject->m_nObjectType == 7) {
                    sName = ((CGameObject*)pObject)->AsSWCTrigger()->m_sTransitionName;
                }
                int nDash;
                {
                    CExoString sDash("-");
                    nDash = sName.Find(sDash, 0);
                }
                if (nDash != -1) {
                    sName = sName.Right(sName.GetLength() - 2 - nDash);
                }
                m_lblDescription.m_cText.SetText(sName);
            }
        }
        m_oidObject = oidObject;
    }
    m_vPosition = vPosition;
}

// FUNCTION: SWKOTOR 0x006C8050
void CSWGuiInGameAreaTransition::Draw(float fDelta)
{
    CSWGuiExtent extent = m_extent;
    int nTop;
    if (g_pAppManager->GetClientExoApp()->GetInGameGui()->GetBarkBubbleVisible(&nTop)) {
        extent.top = nTop;
    } else {
        extent.top = m_nSavedTop;
    }
    SetExtent(extent);
    if (g_pAppManager->m_pClientExoApp->GetParty()->IsPartyTogether()) {
        m_lblTextBG.m_cBorder.SetColor(g_vUpgradeAvailableColor);
        m_lblIcon.m_cBorder.SetColor(g_vUpgradeAvailableColor);
        m_lblDescription.m_cText.SetColor(g_vDifficultyExitColor);
    } else {
        m_lblTextBG.m_cBorder.SetColor(g_vKeyMapLockedColor);
        m_lblIcon.m_cBorder.SetColor(g_vKeyMapLockedColor);
        m_lblDescription.m_cText.SetColor(g_vKeyMapLockedTextColor);
    }
    CSWGuiPanel::Draw(fDelta);
}
