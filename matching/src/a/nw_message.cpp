// CNWMessage / CSWMessage read/write stream helpers and
// CSWCCMessageData (0x004D60F0-0x004D7780; behavioural reference:
// src/batches/b004d42_combat_round_lifecycle.cpp, b004ed_creature_misc.cpp).
#include "a/nw_message.h"

// FUNCTION: SWKOTOR 0x004D60F0
CNWMessage::CNWMessage()
{
    m_pnWriteBuffer = new unsigned char[0x80];
    m_nWriteBufferSize = 0x80;
    m_nWriteBufferPtr = 0;
    m_bWriting = 0;
    m_pnReadBuffer = NULL;
    m_nReadBufferSize = 0;
    m_nReadBufferPtr = 0;
    m_pnWriteFragments = NULL;
}

// FUNCTION: SWKOTOR 0x004D6130
CNWMessage::~CNWMessage()
{
    if (m_pnWriteBuffer) {
        delete[] m_pnWriteBuffer;
        m_pnWriteBuffer = NULL;
    }
    if (m_pnWriteFragments) {
        delete[] m_pnWriteFragments;
        m_pnWriteFragments = NULL;
    }
}

// FUNCTION: SWKOTOR 0x004D6660 SYMBOL: ??_GCNWMessage@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x004D6170
void CNWMessage::SetReadPositionToEnd()
{
    m_nReadBufferPtr = m_nReadBufferSize;
}

// FUNCTION: SWKOTOR 0x004D6180
CResRef CSWMessage::ReadCResRef(int nSize)
{
    char szResRef[16];
    if (nSize > 16) {
        nSize = 16;
    }
    if (m_nReadBufferPtr + nSize > m_nReadBufferSize) {
        m_nReadBufferPtr += nSize;
        return CResRef("");
    }
    memcpy(szResRef, m_pnReadBuffer + m_nReadBufferPtr, nSize);
    m_nReadBufferPtr += nSize;
    return *(CResRef*)szResRef;
}

// FUNCTION: SWKOTOR 0x004D6200
unsigned char* CNWMessage::ReadVOIDPtr(unsigned long nSize)
{
    if (m_nReadBufferPtr + nSize > m_nReadBufferSize) {
        m_nReadBufferPtr += nSize;
        return NULL;
    }
    unsigned char* pData = m_pnReadBuffer + m_nReadBufferPtr;
    m_nReadBufferPtr += nSize;
    return pData;
}

// FUNCTION: SWKOTOR 0x004D6230
int CNWMessage::MessageReadOverflow(int bFlag)
{
    return m_nReadBufferPtr > m_nReadBufferSize;
}

// FUNCTION: SWKOTOR 0x004D6240
int CNWMessage::MessageReadUnderflow(int bFlag)
{
    return m_nReadBufferPtr < m_nReadBufferSize;
}

// FUNCTION: SWKOTOR 0x004D6250
int CNWMessage::MoreDataToRead()
{
    return m_nReadBufferPtr < m_nReadBufferSize;
}

// FUNCTION: SWKOTOR 0x004D6260
void CNWMessage::ExtendWriteBuffer(unsigned long nSize)
{
    unsigned long nNewSize = nSize + m_nWriteBufferSize + 0x80;
    unsigned char* pNew = new unsigned char[nNewSize];
    memcpy(pNew, m_pnWriteBuffer, m_nWriteBufferPtr);
    if (m_pnWriteBuffer) {
        delete[] m_pnWriteBuffer;
    }
    m_nWriteBufferSize = nNewSize;
    m_pnWriteBuffer = pNew;
}

// FUNCTION: SWKOTOR 0x004D62C0
void CNWMessage::WriteBYTE(unsigned char nValue, int nBits)
{
    if (m_nWriteBufferPtr + 1 > m_nWriteBufferSize) {
        ExtendWriteBuffer(1);
    }
    m_pnWriteBuffer[m_nWriteBufferPtr] = nValue;
    m_nWriteBufferPtr += 1;
    m_nWriteBytesTotal += 1;
}

// FUNCTION: SWKOTOR 0x004D6300
void CNWMessage::WriteWORD(unsigned short nValue, int nBits)
{
    if (m_nWriteBufferPtr + 2 > m_nWriteBufferSize) {
        ExtendWriteBuffer(2);
    }
    *(unsigned short*)(m_pnWriteBuffer + m_nWriteBufferPtr) = nValue;
    m_nWriteBufferPtr += 2;
    m_nWriteBytesTotal += 2;
}

// FUNCTION: SWKOTOR 0x004D6340
void CNWMessage::WriteDWORD(unsigned long nValue, int nBits)
{
    if (m_nWriteBufferPtr + 4 > m_nWriteBufferSize) {
        ExtendWriteBuffer(4);
    }
    *(unsigned long*)(m_pnWriteBuffer + m_nWriteBufferPtr) = nValue;
    m_nWriteBufferPtr += 4;
    m_nWriteBytesTotal += 4;
}

// FUNCTION: SWKOTOR 0x004D6380
void CNWMessage::WriteCResRef(CResRef cResRef, int nSize)
{
    if (m_nWriteBufferPtr + nSize > m_nWriteBufferSize) {
        ExtendWriteBuffer(nSize);
    }
    memcpy(m_pnWriteBuffer + m_nWriteBufferPtr, cResRef.GetResRef(), nSize);
    m_nWriteBufferPtr += nSize;
    m_nWriteBytesTotal += nSize;
}

// FUNCTION: SWKOTOR 0x004D63E0
void CNWMessage::WriteVOIDPtr(void* pData, unsigned long nSize)
{
    if (m_nWriteBufferPtr + nSize > m_nWriteBufferSize) {
        ExtendWriteBuffer(nSize);
    }
    memcpy(m_pnWriteBuffer + m_nWriteBufferPtr, pData, nSize);
    m_nWriteBufferPtr += nSize;
    m_nWriteBytesTotal += nSize;
}

// NONMATCH (3): the non-null branch swaps ecx/edx for buffer/ppBuffer.
// Writing the null branch as `*ppBuffer = NULL; bResult = 0;` fixes that
// branch but loses the shared xor-eax store instead; early returns are worse.
// FUNCTION: SWKOTOR 0x004D6440
int CNWMessage::GetWriteMessage(unsigned char** ppBuffer, unsigned long* pSize)
{
    int bResult = 1;
    m_bWriting = 0;
    *pSize = m_nWriteBufferPtr;
    if (m_nWriteBufferPtr == 0) {
        bResult = 0;
        *ppBuffer = NULL;
    }
    else {
        *ppBuffer = m_pnWriteBuffer;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x004D6480
int CSWCCMessageData::GetInteger(int nIndex)
{
    if (nIndex < m_lstIntegers.num) {
        return m_lstIntegers.element[nIndex];
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004D64A0
float CSWCCMessageData::GetFloat(int nIndex)
{
    if (nIndex < m_lstFloats.num) {
        return m_lstFloats.element[nIndex];
    }
    return g_fMessageZero;
}

// FUNCTION: SWKOTOR 0x004D64C0
unsigned long CSWCCMessageData::GetObjectID(int nIndex)
{
    if (nIndex < m_lstObjects.num) {
        return m_lstObjects.element[nIndex];
    }
    return 0x7f000000;
}

// FUNCTION: SWKOTOR 0x004D64E0
int CSWCCMessageData::SaveData(CResGFF* pRes, CResStruct* pStruct)
{
    CResList cList;
    int i;

    pRes->WriteFieldBYTE(pStruct, m_nType, "Type");

    pRes->AddList(&cList, pStruct, "IntList");
    for (i = 0; i < m_lstIntegers.num; i++) {
        CResStruct cElement;
        pRes->AddListElement(&cElement, &cList, 0xbaad);
        pRes->WriteFieldINT(&cElement, m_lstIntegers.element[i], "IntegerValue");
    }

    pRes->AddList(&cList, pStruct, "FloatList");
    for (i = 0; i < m_lstFloats.num; i++) {
        CResStruct cElement;
        pRes->AddListElement(&cElement, &cList, 0xbaad);
        pRes->WriteFieldFLOAT(&cElement, m_lstFloats.element[i], "FloatValue");
    }

    pRes->AddList(&cList, pStruct, "ObjectIDList");
    for (i = 0; i < m_lstObjects.num; i++) {
        CResStruct cElement;
        pRes->AddListElement(&cElement, &cList, 0xbaad);
        pRes->WriteFieldDWORD(&cElement, m_lstObjects.element[i], "ObjectValue");
    }

    pRes->AddList(&cList, pStruct, "StringList");
    for (i = 0; i < m_lstStrings.num; i++) {
        CResStruct cElement;
        pRes->AddListElement(&cElement, &cList, 0xbaad);
        pRes->WriteFieldCExoString(&cElement, m_lstStrings.element[i], "StringValue");
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004D66B0
void CNWMessage::SetReadMessage(unsigned char* pBuffer, unsigned long nSize,
                                unsigned long nPlayerId, unsigned long nFlags)
{
    m_pnReadBuffer = pBuffer;
    m_nReadBufferSize = nSize;
    m_nReadBufferPtr = 0;
    m_n38 = 0;
    m_n3c = 0;
    m_nPlayerId = nPlayerId;
    m_nFlags = nFlags;
    if (nSize == 0) {
        m_n44 = m_n45;
    }
}

// FUNCTION: SWKOTOR 0x004D66F0
int CNWMessage::ReadBOOL()
{
    if (m_nReadBufferPtr + 4 > m_nReadBufferSize) {
        m_nReadBufferPtr += 4;
        return 0;
    }
    unsigned char nValue = m_pnReadBuffer[m_nReadBufferPtr];
    m_nReadBufferPtr += 4;
    return nValue;
}

// FUNCTION: SWKOTOR 0x004D6720
unsigned char CNWMessage::ReadBYTE(int nBits)
{
    if (m_nReadBufferPtr + 1 > m_nReadBufferSize) {
        m_nReadBufferPtr += 1;
        return 0;
    }
    unsigned char nValue = m_pnReadBuffer[m_nReadBufferPtr];
    m_nReadBufferPtr += 1;
    return nValue;
}

// FUNCTION: SWKOTOR 0x004D6750
unsigned short CNWMessage::ReadWORD(int nBits)
{
    if (m_nReadBufferPtr + 2 > m_nReadBufferSize) {
        m_nReadBufferPtr += 2;
        return 0;
    }
    unsigned short nValue = *(unsigned short*)(m_pnReadBuffer + m_nReadBufferPtr);
    m_nReadBufferPtr += 2;
    return nValue;
}

// FUNCTION: SWKOTOR 0x004D6780
unsigned long CNWMessage::ReadDWORD(int nBits)
{
    if (m_nReadBufferPtr + 4 > m_nReadBufferSize) {
        m_nReadBufferPtr += 4;
        return 0;
    }
    unsigned long nValue = *(unsigned long*)(m_pnReadBuffer + m_nReadBufferPtr);
    m_nReadBufferPtr += 4;
    return nValue;
}

// FUNCTION: SWKOTOR 0x004D67B0
float CNWMessage::ReadFLOAT(float fScale, int nBits)
{
    float fValue;
    if (m_nReadBufferPtr + 4 > m_nReadBufferSize) {
        fValue = g_fMessageZero;
    }
    else {
        fValue = *(float*)(m_pnReadBuffer + m_nReadBufferPtr);
    }
    m_nReadBufferPtr += 4;
    return fValue;
}

// FUNCTION: SWKOTOR 0x004D67E0
CExoString CNWMessage::ReadCExoString(int nBits)
{
    int nLength = ReadDWORD(32);
    if (m_nReadBufferPtr + nLength > m_nReadBufferSize) {
        m_nReadBufferPtr += nLength;
        return CExoString("");
    }
    CExoString sValue((char*)(m_pnReadBuffer + m_nReadBufferPtr), nLength);
    m_nReadBufferPtr += nLength;
    return sValue;
}

// FUNCTION: SWKOTOR 0x004D68A0
void CSWMessage::CreateWriteMessage(unsigned long nSize, unsigned long nPlayerId, unsigned long nFlags)
{
    m_nFlags = nFlags;
    m_nPlayerId = nPlayerId;
    m_bWriting = 1;
    m_nWriteBufferPtr = 3;
    nSize += 3;
    if (m_nWriteBufferSize < nSize) {
        ExtendWriteBuffer(nSize - m_nWriteBufferSize);
    }
}

// FUNCTION: SWKOTOR 0x004D68E0
void CNWMessage::WriteBOOL(int bValue)
{
    if (m_nWriteBufferPtr + 4 > m_nWriteBufferSize) {
        ExtendWriteBuffer(4);
    }
    *(int*)(m_pnWriteBuffer + m_nWriteBufferPtr) = bValue;
    m_nWriteBufferPtr += 4;
    m_nWriteBytesTotal += 4;
}

// FUNCTION: SWKOTOR 0x004D6920
void CNWMessage::WriteFLOAT(float fValue, float fScale, int nBits)
{
    if (m_nWriteBufferPtr + 4 > m_nWriteBufferSize) {
        ExtendWriteBuffer(4);
    }
    *(float*)(m_pnWriteBuffer + m_nWriteBufferPtr) = fValue;
    m_nWriteBufferPtr += 4;
    m_nWriteBytesTotal += 4;
}

// FUNCTION: SWKOTOR 0x004D6960
void CNWMessage::WriteCExoString(const CExoString& sValue, int nBits)
{
    int nLength = sValue.GetLength();
    WriteDWORD(nLength, 32);
    if (m_nWriteBufferPtr + nLength > m_nWriteBufferSize) {
        ExtendWriteBuffer(nLength);
    }
    memcpy(m_pnWriteBuffer + m_nWriteBufferPtr, sValue.CStr(), nLength);
    m_nWriteBufferPtr += nLength;
    m_nWriteBytesTotal += nLength;
}

// FUNCTION: SWKOTOR 0x004D69F0
CSWCCMessageData::CSWCCMessageData()
{
    m_nType = 0;
}

// FUNCTION: SWKOTOR 0x004D6A40
CSWCCMessageData::~CSWCCMessageData()
{
}

// FUNCTION: SWKOTOR 0x004D6AE0
void CSWCCMessageData::SetString(int nIndex, const CExoString& sValue)
{
    if (nIndex >= m_lstStrings.num) {
        m_lstStrings.SetSize(nIndex + 1);
    }
    m_lstStrings.element[nIndex] = sValue;
}

// FUNCTION: SWKOTOR 0x004D7120
Vector CNWMessage::ReadVector()
{
    Vector v;
    v.x = ReadFLOAT(1.0f, 32);
    v.y = ReadFLOAT(1.0f, 32);
    v.z = ReadFLOAT(1.0f, 32);
    return v;
}

// FUNCTION: SWKOTOR 0x004D71B0
void CNWMessage::WriteVector(const Vector& v)
{
    WriteFLOAT(v.x, 1.0f, 32);
    WriteFLOAT(v.y, 1.0f, 32);
    WriteFLOAT(v.z, 1.0f, 32);
}

// FUNCTION: SWKOTOR 0x004D7360
CExoString CSWCCMessageData::GetString(int nIndex)
{
    if (nIndex < m_lstStrings.num) {
        return m_lstStrings.element[nIndex];
    }
    return CExoString("");
}

// FUNCTION: SWKOTOR 0x004D76B0
void CSWCCMessageData::SetInteger(int nIndex, int nValue)
{
    if (nIndex >= m_lstIntegers.num) {
        m_lstIntegers.SetSize(nIndex + 1);
    }
    m_lstIntegers.element[nIndex] = nValue;
}

// FUNCTION: SWKOTOR 0x004D7720
void CSWCCMessageData::SetFloat(int nIndex, float fValue)
{
    if (nIndex >= m_lstFloats.num) {
        m_lstFloats.SetSize(nIndex + 1);
    }
    m_lstFloats.element[nIndex] = fValue;
}

// FUNCTION: SWKOTOR 0x004D7780
void CSWCCMessageData::SetObjectID(int nIndex, unsigned long oidValue)
{
    if (nIndex >= m_lstObjects.num) {
        m_lstObjects.SetSize(nIndex + 1);
    }
    m_lstObjects.element[nIndex] = oidValue;
}
