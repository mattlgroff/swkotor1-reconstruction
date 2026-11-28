// sws_player_store.h -- CSWSPlayerStore
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/b/sws_player.h); the other copies' members fall on its members.
#ifndef SHARED_SWS_PLAYER_STORE_H
#define SHARED_SWS_PLAYER_STORE_H


class CSWSPlayerStore {
public:
    CSWSPlayerStore();                                          // 0x005CA7F0
    ~CSWSPlayerStore();                                         // 0x005CA900
    void ClearItems();                                          // 0x005CA030 (name guessed)
    void ClearRepository();                                     // 0x005CA470
    void Clear();                                               // 0x005CA4A0

    unsigned long m_aSlotItems[18];                             // 0x00
    int m_aSlotStackSize[18];                                   // 0x48
    char pad_0x90[0x10];
};

#endif
