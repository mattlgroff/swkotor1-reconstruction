// nw_message.h -- CNWMessage
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/b/sws_message.h); the other copies' members fall on its members.
#ifndef SHARED_NW_MESSAGE_H
#define SHARED_NW_MESSAGE_H

#include "shared/exo_string.h"
#include "shared/vector.h"

class CResRef;

class CNWMessage {
public:
    CNWMessage();                                                       // 0x004D60F0
    virtual ~CNWMessage();                                              // 0x004D6130

    void ExtendWriteBuffer(unsigned long nSize);                        // 0x004D6260
    int MessageReadOverflow(int bFlag);                                 // 0x004D6230
    int MessageReadUnderflow(int bFlag);                                // 0x004D6240
    int ReadBOOL();                                                     // 0x004D66F0
    unsigned char ReadBYTE(int nBits);                                  // 0x004D6720
    unsigned short ReadWORD(int nBits);                                 // 0x004D6750
    unsigned long ReadDWORD(int nBits);                                 // 0x004D6780
    float ReadFLOAT(float fScale, int nBits);                           // 0x004D67B0
    CExoString ReadCExoString(int nBits);                               // 0x004D67E0
    unsigned char* ReadVOIDPtr(unsigned long nSize);                    // 0x004D6200

    void WriteBOOL(int bValue);                                         // 0x004D68E0
    void WriteBYTE(unsigned char nValue, int nBits);                    // 0x004D62C0
    void WriteWORD(unsigned short nValue, int nBits);                   // 0x004D6300
    void WriteINT(int nValue, int nBits);                               // 0x004D6340
    void WriteDWORD(unsigned long nValue, int nBits);                   // 0x004D6340
    void WriteFLOAT(float fValue, float fScale, int nBits);             // 0x004D6920
    void WriteVector(const Vector& vValue);                            // 0x004D71B0
    void WriteCExoString(const CExoString& sValue, int nBits);          // 0x004D6960
    void WriteCResRef(CResRef cResRef, int nBits);                      // 0x004D6380
    int GetWriteMessage(unsigned char** ppMessage, unsigned long* pnSize); // 0x004D6440

    unsigned char* m_pnWriteBuffer;                                     // 0x04
    unsigned long m_nWriteBufferSize;                                   // 0x08
    unsigned long m_nWriteBufferPtr;                                    // 0x0c
    unsigned char * m_pnWriteFragments;          // 0x10
    unsigned long  m_n14;                        // 0x14
    unsigned long  m_n18;                        // 0x18
    unsigned long m_nWriteBytesTotal;                                   // 0x1c
    unsigned long  m_n20;                        // 0x20
    int  m_bWriting;                             // 0x24
    unsigned char* m_pnReadBuffer;                                      // 0x28
    unsigned long m_nReadBufferSize;                                    // 0x2c
    unsigned long m_nReadBufferPtr;                                     // 0x30
    unsigned long  m_n34;                        // 0x34
    unsigned long  m_n38;                        // 0x38
    unsigned long  m_n3c;                        // 0x3c
    unsigned long  m_n40;                        // 0x40
    unsigned char  m_n44;                        // 0x44
    unsigned char  m_n45;                        // 0x45
    char pad_0x46[0x2];
    unsigned long  m_nFlags;                     // 0x48
    unsigned long m_nPlayerId;                                          // 0x4c

    // declared by other copies
    void SetReadMessage(unsigned char* pBuffer, unsigned long nSize, unsigned long nPlayerId, unsigned long nFlags);
    int MoreDataToRead();
    void SetReadPositionToEnd();
    Vector ReadVector();
    void WriteVOIDPtr(void* pData, unsigned long nSize);
    void WriteOBJECTID(unsigned long nValue);
};

#endif
