// reentrant_server_stats.h -- CSWReentrantServerStats, the app manager's
// save/load status record (CAppManager +0x14)
//
// One declaration for every unit (CODE_RULES.md, R1). CAppManager's
// constructor allocates it (d/app_misc.cpp); units also viewed it as
// CAppPad, CAppManager14, CSaveLoadStatus and CUnknown004b13e0. The layout
// is the union of those views, which agree at every offset; the Mac symbol
// tables name the destructor.
#ifndef SHARED_REENTRANT_SERVER_STATS_H
#define SHARED_REENTRANT_SERVER_STATS_H

#include "shared/exo_string.h"

class CSWReentrantServerStats
{
public:
    CSWReentrantServerStats();                  // 0x004010C0
    ~CSWReentrantServerStats();                 // 0x00401000
    void FUN_004b13e0(unsigned long nValue);    // 0x004B13E0, seeds +0x00, +0x04, +0x10, +0x38

    int m_bActive;                              // +0x00 (descriptive)
    unsigned long m_nKind;                      // +0x04 (descriptive)
    unsigned long m_nStep;                      // +0x08, load-bar step (descriptive)
    unsigned long m_nProgress;                  // +0x0C, load-bar progress (descriptive)
    int m_n10;                                  // +0x10
    int m_n14;                                  // +0x14
    CExoString m_sDirectory;                    // +0x18, save-game directory (descriptive)
    CExoString m_sSaveGame;                     // +0x20, save-game name (descriptive)
    CExoString m_s28;                           // +0x28
    char pad_0x30[0x38 - 0x30];
    int m_nLoadError;                           // +0x38 (descriptive)
};

#endif
