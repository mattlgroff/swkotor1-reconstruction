// CSWSDoor (only the lock/trap/open state the VM commands read).
#ifndef B_SWS_DOOR_H
#define B_SWS_DOOR_H

#include "b/exo_string.h"

class CSWSDoor {
public:
    class CSWSObject* GetLinkedObject();                    // 0x00589580
    void SetLocked(int bLocked) { m_bLocked = bLocked; }

    char pad_0x0[0xd4];
    int m_nAnimationState;                                  // 0xd4 (0x2726 closed)
    char pad_0xd8[0xf8 - 0xd8];
    int m_bPlot;                                            // 0xf8
    char pad_0xfc[0x2bf - 0xfc];
    unsigned char m_nOpenLockDC;                            // 0x2bf (GetLockUnlockDC)
    unsigned char m_nCloseLockDC;                           // 0x2c0 (GetLockLockDC)
    char pad_0x2c1[0x2c4 - 0x2c1];
    int m_bLocked;                                          // 0x2c4
    int m_bLockable;                                        // 0x2c8
    unsigned char m_bOpen;                                  // 0x2cc (name guessed)
    char pad_0x2cd[0x2d0 - 0x2cd];
    CExoString m_sKeyName;                                  // 0x2d0
    int m_bKeyRequired;                                     // 0x2d8
    char pad_0x2dc[0x2e8 - 0x2dc];
    int m_bTrapped;                                         // 0x2e8 (name guessed)
    char pad_0x2ec[0x3c0 - 0x2ec];
    int m_n3C0;                                             // 0x3c0 (no door actions when set)
};

#endif
