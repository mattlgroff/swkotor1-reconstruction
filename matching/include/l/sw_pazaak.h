// Worker L matching header: the Pazaak rules objects in swpazaak.o
// (0x006E4360-0x006E4C60). CPazaakCard {value, flag}, CPazaakPlayer (0x70,
// no vtable) and CSWPazaak (0x230, no vtable). Layout copied from
// matching/include/m/sw_gui_pazaak.h (bucket m), trimmed.
#ifndef L_SW_PAZAAK_H
#define L_SW_PAZAAK_H

#include "shared/exo_string.h"

#include "shared/res_ref.h"

class C2DA {
public:
    C2DA(CResRef cResRef, int bPrivate);            // 0x00413CC0
    virtual ~C2DA();                                // 0x00413D80
    int Load2DArray();                              // 0x004143B0
    void Unload2DArray();                           // 0x004139E0
    int GetCExoStringEntry(int nRow, int nColumn, CExoString* pValue); // 0x00413190
    char pad_0x04[0x20];
    int m_nNumRows;                                 // 0x24
    char pad_0x28[0x54 - 0x28];
};

extern "C" int __cdecl rand(void);                  // 0x006FC447

// Card value by card code: 0..5 +1..+6, 6..11 -1..-6, 12..17 flip +-1..6,
// 18..27 the main-deck 1..10. Descriptive name.
#include "shared/globals.h"

// A Pazaak card {value, flag}; the constructor only clears the flag.
#include "shared/pazaak_card.h"

// 0x70 bytes, no vtable (ctor 0x006E4B50).
#include "shared/pazaak_player.h"

// 0x230 bytes, no vtable (ctor 0x006E4C00).
#include "shared/sw_pazaak.h"

#endif // L_SW_PAZAAK_H
