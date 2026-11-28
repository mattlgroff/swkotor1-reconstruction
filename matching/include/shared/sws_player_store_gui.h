// sws_player_store_gui.h -- CSWSPlayerStoreGUI
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/b/sws_player.h); the other copies' members fall on its members.
#ifndef SHARED_SWS_PLAYER_STORE_GUI_H
#define SHARED_SWS_PLAYER_STORE_GUI_H


class CSWSPlayerStore;

class CSWSPlayerStoreGUI {
public:
    CSWSPlayerStoreGUI();                                       // 0x00560C20
    void SetOpen(int bOpen);                                    // 0x00560C90
    ~CSWSPlayerStoreGUI() { delete m_pStore; }

    CSWSPlayerStore* m_pStore;                                  // 0x00
    int m_bOpen;                                                // 0x04
};

#endif
