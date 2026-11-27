// sw_gui_manager_panels.h -- CSWGuiManagerPanels
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/gui_in_game_dialog.cpp); the other copies' members fall on its members.
#ifndef SHARED_SW_GUI_MANAGER_PANELS_H
#define SHARED_SW_GUI_MANAGER_PANELS_H


class CSWGuiPanel;

class CSWGuiManagerPanels {
public:
    void PlayGuiSound(int n);                           // 0x0040A140
    void AddPanel(CSWGuiPanel* pPanel, int a, int b);   // 0x0040BC70
    void SendToBack(CSWGuiPanel* pPanel);               // 0x0040BD20
    int PanelExists(CSWGuiPanel* pPanel);               // 0x0040BD70
    void PopModalPanel();                               // 0x0040BE00
    void RemovePanel(CSWGuiPanel* pPanel);              // 0x0040C830

    char pad_0x0[0x98];
    int m_bLocked;                                      // 0x98
};

#endif
