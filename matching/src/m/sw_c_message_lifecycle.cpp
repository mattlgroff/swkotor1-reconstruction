// CSWCMessage lifecycle over the shared CNWMessage implementation.

class CNWMessage {
public:
    CNWMessage();                                  // 0x004D60F0
    virtual ~CNWMessage();                         // 0x004D6130
};

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
