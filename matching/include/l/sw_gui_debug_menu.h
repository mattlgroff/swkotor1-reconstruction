// Worker L matching headers: the powers/feats/skills debug menu
// CSWGuiPowersFeatsSkillsDebugMenu (layout "debug", vtable 0x00757A78,
// 0x006CEA90-0x006CF890).
#ifndef L_SW_GUI_DEBUG_MENU_H
#define L_SW_GUI_DEBUG_MENU_H

#include "l/sw_gui_skill_info.h"

extern CExoString g_sBuildNumber;                   // 0x007A3CC4

class CGuiInGameDebug : public CGuiInGame {
public:
    void HidePowersFeatsSkillsDebugMenu();          // 0x0062DAE0
    void RefreshDebug();                            // 0x004015A0 (an empty function)
    void HideLoadModuleDebugMenu();                 // 0x0062DA70
    void HideItemCreateMenu();                      // 0x0062D2B0
    void ShowSubItemCreateMenu(int nBaseItem);      // 0x0062D320
};

// vtable 0x00757A78; ctor 0x006CF5A0, ??_G 0x006CF6F0, implicit dtor
// 0x006CF710; 0x490 bytes.
class CSWGuiPowersFeatsSkillsDebugMenu : public CSWGuiPanel {
public:
    CSWGuiPowersFeatsSkillsDebugMenu(CSWGuiManager* pManager);  // 0x006CF5A0
    void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006CF7B0)

    CSWGuiButton* CreateButton(const CExoString& sText, PFN_GUI_CALLBACK pfnCallback,
                               CSWGuiButton* pProto);   // 0x006CEA90
    void AddSpell(int nSpell, CSWSCreature* pCreature);     // 0x006CEBA0
    void AddFeat(unsigned short nFeat, CSWSCreature* pCreature);    // 0x006CEC40
    void OnSkillSelected(CSWGuiControl* pControl);  // 0x006CECD0
    void ShowSkills();                              // 0x006CED60
    void OnPowerSelected(CSWGuiControl* pControl);  // 0x006CEF00
    void OnFeatSelected(CSWGuiControl* pControl);   // 0x006CEF80
    void OnShowSkills(CSWGuiControl* pControl);     // 0x006CF020
    void ShowPowers();                              // 0x006CF040
    void ShowFeats();                               // 0x006CF220
    void OnShowPowers(CSWGuiControl* pControl);     // 0x006CF3D0
    void OnShowFeats(CSWGuiControl* pControl);      // 0x006CF3F0
    void ShowMain();                                // 0x006CF410
    void OnBack();                                  // 0x006CF770
    void CenterPanel();                             // 0x0040A600

    static CSWSCreature* GetPlayerCreature()
    {
        CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
        return pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureID());
    }
    static void HideMenu()
    {
        ((CGuiInGameDebug*)g_pAppManager->m_pClientExoApp->GetInGameGui())->HidePowersFeatsSkillsDebugMenu();
    }
    int GetOptionIndex(CSWGuiControl* pControl)
    {
        int i = 0;
        while (m_lbOptions.GetControl(i) != pControl) {
            i++;
        }
        return i;
    }
    __forceinline void FinishList(CExoArrayListL<CSWGuiControl*>& lstButtons, int nMode)
    {
        m_lbOptions.AddControls(lstButtons, 1, 0, 0);
        CSWGuiListBox* pList = &m_lbOptions;
        pList->m_nListFlags |= 0x40;
        SetActiveControl(pList, 0);
        m_nMode = nMode;
        pList->SetActiveControlV(pList->GetControl(m_nSelected), 0);
    }

    CSWGuiListBox m_lbOptions;                      // 0x64
    CSWGuiLabel m_lblBuild;                         // 0x344
    int m_nSelected;                                // 0x484
    int m_nOptions;                                 // 0x488
    int m_nMode;                                    // 0x48c (0 main, 1 powers, 2 skills, 3 feats)
};

inline CGuiInGameDebug* GetDebugGui()
{
    return (CGuiInGameDebug*)g_pAppManager->m_pClientExoApp->GetInGameGui();
}

// vtable 0x00757AE8; ctor 0x006CFA70 (not matched yet), ??_G 0x006D0240,
// implicit dtor 0x006D0260. Lists the module directories as buttons.
class CSWGuiLoadModuleDebugMenu : public CSWGuiPanel {
public:
    CSWGuiLoadModuleDebugMenu(CSWGuiManager* pManager);     // 0x006CFA70
    CSWGuiLoadModuleDebugMenu(CSWGuiManager* pManager, int bUnused);  // vtable emitter only
    void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006D02C0)

    CSWGuiButton* CreateButton(const CExoString& sText, PFN_GUI_CALLBACK pfnCallback,
                               CSWGuiButton* pProto);   // 0x006CF890
    void Close();                                   // 0x006CF9A0
    void OnModuleSelected(CSWGuiControl* pControl); // 0x006CF9D0

    CSWGuiListBox m_lbOptions;                      // 0x64
    CSWGuiLabel m_lblBuild;                         // 0x344
    int m_nSelected;                                // 0x484
    int m_nOptions;                                 // 0x488
};

class CSWBaseItemArrayL {
public:
    int m_nNumBaseItems;                            // 0x00
};

// vtable 0x00757B68; ctor 0x006D0620 (not matched yet), ??_G 0x006D09C0,
// implicit dtor 0x006D09E0. Lists the base items (one button each).
class CSWGuiItemCreateMenu : public CSWGuiPanel {
public:
    CSWGuiItemCreateMenu(CSWGuiManager* pManager);  // 0x006D0620
    CSWGuiItemCreateMenu(CSWGuiManager* pManager, int bUnused);   // vtable emitter only
    void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006D03A0)

    CSWGuiButton* CreateButton(const CExoString& sText, PFN_GUI_CALLBACK pfnCallback,
                               CSWGuiButton* pProto);   // 0x006D0490
    void OnBack(CSWGuiControl* pControl);           // 0x006D05A0
    void OnItemSelected(CSWGuiControl* pControl);   // 0x006D05E0

    CSWGuiListBox m_lbOptions;                      // 0x64
    CSWGuiLabel m_lblBuild;                         // 0x344
    int m_nSelected;                                // 0x484
};

#endif
