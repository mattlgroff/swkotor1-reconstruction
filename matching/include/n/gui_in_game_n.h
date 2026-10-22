// Bucket n matching headers: CGuiInGame leaves that forward to the main
// interface.
#ifndef N_GUI_IN_GAME_N_H
#define N_GUI_IN_GAME_N_H

class CSWGuiMainInterface {
public:
    void UpdateActions();                           // 0x00689D80
};

class CGuiInGame {
public:
    char pad_0x0[0x90];
    CSWGuiMainInterface* m_pMainInterface;          // 0x90

    void UpdateCombatMode();                        // 0x0062B050
};

#endif
