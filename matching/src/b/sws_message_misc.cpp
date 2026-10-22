// CSWSMessage constructor/destructor and object id stream helpers.
#include "b/sws_message.h"

// .rdata object id constants.
extern const unsigned long OBJECT_INVALID_ID;       // 0x00746644
extern const unsigned long CLIENT_OBJECT_MASK;      // 0x0074665C

// FUNCTION: SWKOTOR 0x00520980
CSWSMessage::CSWSMessage()
{
}

// FUNCTION: SWKOTOR 0x005209A0
CSWSMessage::~CSWSMessage()
{
}

// FUNCTION: SWKOTOR 0x00525430 SYMBOL: ??_GCSWSMessage@@UAEPAXI@Z
// (emitted with the vtable of the constructor above)

// FUNCTION: SWKOTOR 0x00578520
void CSWSMessage::SendGameObjectUpdate(unsigned long nPlayerId)
{
    SendServerToPlayerGameObjUpdate(nPlayerId, INVALID_OBJECT_ID);
}

// FUNCTION: SWKOTOR 0x00578540
unsigned long CSWSMessage::ReadOBJECTIDServer()
{
    if (m_nReadBufferPtr + 4 > m_nReadBufferSize) {
        m_nReadBufferPtr += 4;
        return 0;
    }
    unsigned long oid = *(unsigned long*)(m_pnReadBuffer + m_nReadBufferPtr) & 0x7fffffff;
    m_nReadBufferPtr += 4;
    return oid;
}

// FUNCTION: SWKOTOR 0x00578570
void CSWSMessage::WriteOBJECTIDServer(unsigned long oid)
{
    if (m_nWriteBufferPtr + 4 > m_nWriteBufferSize) {
        ExtendWriteBuffer(4);
    }
    if (oid != OBJECT_INVALID_ID) {
        oid |= CLIENT_OBJECT_MASK;
    }
    *(unsigned long*)(m_pnWriteBuffer + m_nWriteBufferPtr) = oid;
    m_nWriteBufferPtr += 4;
    m_nWriteBytesTotal += 4;
}
