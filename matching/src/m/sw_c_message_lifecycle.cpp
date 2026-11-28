// CSWCMessage lifecycle over the shared CNWMessage implementation.

#include "shared/nw_message.h"

class CSWCMessage : public CNWMessage {
public:
    CSWCMessage();
    virtual ~CSWCMessage();
};

// FUNCTION: SWKOTOR 0x0064B510
CSWCMessage::CSWCMessage()
{
}

// FUNCTION: SWKOTOR 0x0064B530
CSWCMessage::~CSWCMessage()
{
}
