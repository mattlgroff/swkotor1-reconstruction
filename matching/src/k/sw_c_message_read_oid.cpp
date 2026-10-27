// CSWCMessage::ReadOBJECTIDClient (nwcmessage.o; sits after the trigger and
// waypoint code in the image).

class CSWCMessage {
public:
    char pad_0x0[0x28];
    unsigned char* m_pnReadBuffer;                                   // +0x28
    unsigned long m_nReadBufferSize;                                 // +0x2C
    unsigned long m_nReadBufferPtr;                                  // +0x30

    unsigned long ReadOBJECTIDClient();
};

// NONMATCH (3): the original keeps a dead `oid = 0` store in a stack slot
// (push ecx frame); memcpy, struct, and inline read-helper shapes all let
// cl drop the slot or change the success path.
// FUNCTION: SWKOTOR 0x00692630
unsigned long CSWCMessage::ReadOBJECTIDClient()
{
    unsigned long oid = 0;
    if (m_nReadBufferPtr + 4 > m_nReadBufferSize) {
        m_nReadBufferPtr += 4;
        return 0;
    }
    oid = *(unsigned long*)(m_pnReadBuffer + m_nReadBufferPtr);
    m_nReadBufferPtr += 4;
    return oid;
}
