// sw_message.h -- CSWMessage
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/b/sws_message.h); the other copies' members fall on its members.
#ifndef SHARED_SW_MESSAGE_H
#define SHARED_SW_MESSAGE_H

#include "shared/nw_message.h"

class CSWMessage : public CNWMessage {
public:
    unsigned long PeekAtWriteMessageSize();                             // 0x00405040
    void CreateWriteMessage(unsigned long nSize, unsigned long nPlayerId,
                            int bArg);                                  // 0x004D68A0
    CResRef ReadCResRef(int nBits);                                     // 0x004D6180

    // declared by other copies
    void CreateWriteMessage(unsigned long nSize, unsigned long nPlayerId, unsigned long nFlags);
};

#endif
