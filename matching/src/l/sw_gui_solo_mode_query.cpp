// CSWGuiInGameSoloModeQuery (vtable 0x00756F28, 0xFC8 bytes): the message box
// that asks whether to switch solo mode on or off. The destructor is in
// sw_gui_panel_dtors_novtable.cpp.
#include "l/sw_gui_inventory_equip.h"

class CSWSArea {
public:
    char pad_0x0[0x2c4];
    int m_bTransitioning;                           // 0x2c4 (placeholder name)
};

class CSWCArea {
public:
    CSWSArea* GetServerArea();                      // 0x00605640
};

class CSWCModule {
public:
    char pad_0x0[0x48];
    CSWCArea* m_pArea;                              // 0x48
    CSWCArea* GetArea() { return m_pArea; }
};

class CSWSCreature {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90();
    virtual int GetDead();                          // 0x94 (placeholder name)
    int GetDeadTemp();                              // 0x004EF890
};

class CSWGuiInGameSoloModeQuery : public CSWGuiMessageBox {
public:
    CSWGuiInGameSoloModeQuery(CSWGuiManager* pManager);    // 0x006C2270
    virtual ~CSWGuiInGameSoloModeQuery();           // 0x006C2560
    virtual void Draw(float fDelta);                // 0x34 (0x006C2310)
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006C2400)
    void SetQuery(int bForce);                      // 0x006C24A0 (descriptive)
    void HidePanel()
    {
        m_nFlags &= ~0x100;
        unsigned long nFlags = m_nFlags;
        if ((nFlags & 0x600) != 0x400) {
            m_nFlags = (nFlags & ~0x400) | 0x200;
        }
    }

    CSWGuiLabel m_lbl984;                           // 0x984
    CSWGuiLabel m_lblAC4;                           // 0xac4
    CSWGuiLabel m_lblC04;                           // 0xc04
    CSWGuiLabel m_lblD44;                           // 0xd44
    CSWGuiLabel m_lblE84;                           // 0xe84
    int m_bForce;                                   // 0xfc4 (descriptive)
};

// FUNCTION: SWKOTOR 0x006C2270
CSWGuiInGameSoloModeQuery::CSWGuiInGameSoloModeQuery(CSWGuiManager* pManager)
    : CSWGuiMessageBox(pManager)
{
    m_bForce = 0;
}

// FUNCTION: SWKOTOR 0x006C2310
void CSWGuiInGameSoloModeQuery::Draw(float fDelta)
{
    CSWCCreature* pPlayer = g_pAppManager->m_pClientExoApp->GetPlayerCreature();
    CSWSCreature* pCreature = 0;
    if (pPlayer) {
        pCreature = pPlayer->AsServerCreature();
    }
    CSWSArea* pArea = 0;
    if (g_pAppManager->m_pClientExoApp->GetModule()) {
        CSWCArea* pClientArea = g_pAppManager->m_pClientExoApp->GetModule()->GetArea();
        if (pClientArea) {
            pArea = pClientArea->GetServerArea();
        }
    }
    if (g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bB4 || (pArea && pArea->m_bTransitioning) ||
        g_pAppManager->m_pClientExoApp->GetAreaNotReadyToDisplay() ||
        (pCreature && (pCreature->GetDead() || pCreature->GetDeadTemp()))) {
        HidePanel();
    }
    CSWGuiPanel::Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x006C2400
void CSWGuiInGameSoloModeQuery::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x27:
        case 0x2d:
        case 0x1f6: {
            CSWCCreature* pPlayer = g_pAppManager->m_pClientExoApp->GetPlayerCreature();
            g_pAppManager->m_pClientExoApp->TogglePartyFollow();
            if (pPlayer && m_bForce) {
                pPlayer->PerformSkill(2, 0, pPlayer->GetId());
            }
        }
        case 0x28:
        case 0x2e:
        case 0x1f7:
            g_pAppManager->m_pClientExoApp->GetInGameGui()->HideSoloMode();
            return;
        }
    }
    CSWGuiMessageBox::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006C24A0
void CSWGuiInGameSoloModeQuery::SetQuery(int bForce)
{
    if (bForce) {
        SetMessageStrRef(0x9402);
    } else if (g_pAppManager->m_pServerExoApp->GetPartyTable()->m_bSoloMode) {
        if (g_pAppManager->m_pClientExoApp->GetPlayerCreature()->m_bSoloFlag) {
            SetMessageStrRef(0x9404);
        } else {
            SetMessageStrRef(0x9403);
        }
    } else {
        SetMessageStrRef(0x9401);
    }
    m_bForce = bForce;
}
