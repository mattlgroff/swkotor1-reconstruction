// Pilot 4 arm A: CNWMessage / CSWMessage / CSWCMessage subset.
#ifndef P4A_MESSAGE_H
#define P4A_MESSAGE_H

#include "shared/exo_string.h"

class CResRef {
public:
    CResRef();                                          // 0x00405F70

    char m_resRef[16];
};

class CClientExoApp {
public:
    void* GetSomething();                               // 0x005ED330
    void JournalUpdated(int a, int b, const CExoString& sText); // 0x005ED340
};

#include "shared/app_manager.h"

#include "shared/globals.h"

struct STR_RES {
    CExoString sString;                                 // 0x00
    CResRef cSoundResRef;                               // 0x08
    float fSoundLength;                                 // 0x18
};

#include "shared/nw_message.h"

class CSWMessage : public CNWMessage {
};

class CSWCMessage : public CSWMessage {
public:
    CExoString ReadCExoLocStringClient(CResRef& cSound, unsigned long* pLength); // 0x00692660
    int HandleServerToPlayerJournalUpdated();                       // 0x006526E0
    int HandlePlayerToServerShutDownServer(unsigned char nMinor);   // 0x00654970
};

#endif
