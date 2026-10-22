// CSWCMessage player-to-server senders (0x00677410-0x00678030).
#include "m/sw_c_message.h"

// FUNCTION: SWKOTOR 0x00677410
int CSWCMessage::SendPlayerToServerMessage(unsigned char nMajor, unsigned char nMinor,
                                           unsigned char* pData, unsigned long nSize)
{
    unsigned char* pBuffer;
    unsigned long nLength;
    if (nSize) {
        pBuffer = pData;
        nLength = nSize;
    }
    else {
        nLength = 3;
        pBuffer = new unsigned char[nLength];
    }
    if (!pBuffer) {
        return 0;
    }
    pBuffer[0] = 0x70;
    pBuffer[1] = nMajor;
    pBuffer[2] = nMinor;
    int bResult = g_pAppManager->m_pClientExoApp->GetNetLayer()->SendMessageToPlayer(
        PLAYERID_SERVER, pBuffer, nLength, 0);
    if (!nSize) {
        delete[] pBuffer;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x00677490
int CSWCMessage::SendPlayerToServer_03_02()
{
    return SendPlayerToServerMessage(3, 2, NULL, 0);
}

// FUNCTION: SWKOTOR 0x006774E0
int CSWCMessage::SendPlayerToServer_03_04(unsigned long a, const CExoString& b, const CExoString& c)
{
    unsigned char* pData;
    unsigned long nSize;
    CreateWriteMessage(0x80, -1, 1);
    WriteDWORD(a, 0x20);
    WriteCExoString(b, 0x20);
    WriteCExoString(c, 0x20);
    if (!GetWriteMessage(&pData, &nSize)) {
        return 0;
    }
    return SendPlayerToServerMessage(3, 4, pData, nSize);
}

// FUNCTION: SWKOTOR 0x00677560
int CSWCMessage::SendPlayerToServer_03_05(unsigned long a, const CExoString& b, const CExoString& c)
{
    unsigned char* pData;
    unsigned long nSize;
    CreateWriteMessage(0x80, -1, 1);
    WriteDWORD(a, 0x20);
    WriteCExoString(b, 0x20);
    WriteCExoString(c, 0x20);
    if (!GetWriteMessage(&pData, &nSize)) {
        return 0;
    }
    return SendPlayerToServerMessage(3, 5, pData, nSize);
}

// FUNCTION: SWKOTOR 0x006775E0
int CSWCMessage::SendPlayerToServer_01_00()
{
    return SendPlayerToServerMessage(1, 0, NULL, 0);
}

// FUNCTION: SWKOTOR 0x00677630
int CSWCMessage::SendPlayerToServer_19_02(unsigned long a, unsigned long b)
{
    unsigned char* pData;
    unsigned long nSize = 8;
    CreateWriteMessage(nSize, -1, 1);
    WriteOBJECTID(a);
    WriteOBJECTID(b);
    if (!GetWriteMessage(&pData, &nSize)) {
        return 0;
    }
    return SendPlayerToServerMessage(0x19, 2, pData, nSize);
}

// FUNCTION: SWKOTOR 0x006776A0
int CSWCMessage::SendPlayerToServerInput_CastSpell(unsigned long nSpell, unsigned char a,
                                                   unsigned char b, unsigned char c, int bFlag,
                                                   const Vector* pPos, unsigned long oidTarget)
{
    unsigned char* pData;
    unsigned char nFlags = 0;
    unsigned long nSize = 4;
    CreateWriteMessage(nSize, -1, 1);
    if (oidTarget != OBJECT_INVALID) {
        nFlags = 1;
    }
    float fMag = pPos->x * pPos->x + pPos->y * pPos->y + pPos->z * pPos->z;
    if (fMag >= 0.00001f) {
        nFlags += 2;
    }
    if (bFlag) {
        nFlags += 4;
    }
    WriteDWORD(nSpell, 0x20);
    WriteBYTE(a, 8);
    WriteBYTE(b, 8);
    WriteBYTE(c, 8);
    WriteBYTE(nFlags, 8);
    if (oidTarget != OBJECT_INVALID) {
        WriteOBJECTID(oidTarget);
    }
    if (fMag >= 0.00001f) {
        WriteFLOAT(pPos->x, 1.0f, 0x20);
        WriteFLOAT(pPos->y, 1.0f, 0x20);
        WriteFLOAT(pPos->z + 0.3f, 1.0f, 0x20);
    }
    if (!GetWriteMessage(&pData, &nSize)) {
        return 0;
    }
    return SendPlayerToServerMessage(6, 0x12, pData, nSize);
}

// FUNCTION: SWKOTOR 0x00677800
int CSWCMessage::SendPlayerToServerGuiPause()
{
    if (g_pAppManager->m_pServerExoApp) {
        g_pAppManager->m_pServerExoApp->TogglePauseState(2);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00677820
int CSWCMessage::SendPlayerToServer_1C_0A()
{
    return SendPlayerToServerMessage(0x1c, 0xa, NULL, 0);
}

// FUNCTION: SWKOTOR 0x00677870
int CSWCMessage::SendPlayerToServerInput_GiveItem(unsigned long a, unsigned long b,
                                                  unsigned long c, unsigned long d)
{
    unsigned char* pData;
    unsigned long nSize;
    CreateWriteMessage(0x10, -1, 1);
    WriteOBJECTID(a);
    WriteOBJECTID(b);
    WriteOBJECTID(c);
    WriteDWORD(d, 0x20);
    if (!GetWriteMessage(&pData, &nSize)) {
        return 0;
    }
    return SendPlayerToServerMessage(6, 0x24, pData, nSize);
}

// FUNCTION: SWKOTOR 0x006778F0
int CSWCMessage::SendPlayerToServer_04_03()
{
    return SendPlayerToServerMessage(4, 3, NULL, 0);
}

// FUNCTION: SWKOTOR 0x00677940
int CSWCMessage::SendPlayerToServer_70_0F()
{
    CResRef cSound("HEAL_PLAYER");
    g_pExoSound->PlayOneShotSound(cSound, 0x15, 0, 0, 0, 0);
    return SendPlayerToServerMessage(0xf, 2, NULL, 0);
}

// FUNCTION: SWKOTOR 0x006779D0
int CSWCMessage::SendPlayerToServer_0F_04()
{
    return SendPlayerToServerMessage(0xf, 4, NULL, 0);
}

// FUNCTION: SWKOTOR 0x00677A20
int CSWCMessage::SendPlayerToServer_0D_01(int a)
{
    unsigned char* pData;
    unsigned long nSize = 1;
    CreateWriteMessage(nSize, -1, 1);
    WriteBYTE(a == 1, 8);
    if (!GetWriteMessage(&pData, &nSize)) {
        return 0;
    }
    return SendPlayerToServerMessage(0xd, 1, pData, nSize);
}

// FUNCTION: SWKOTOR 0x00677A90
int CSWCMessage::SendPlayerToServerInput_Attack(unsigned long oid, unsigned short b)
{
    unsigned char* pData = NULL;
    unsigned long nSize = 6;
    CreateWriteMessage(nSize, -1, 1);
    WriteOBJECTID(oid);
    WriteWORD(b, 0x10);
    if (!GetWriteMessage(&pData, &nSize)) {
        return 0;
    }
    return SendPlayerToServerMessage(6, 2, pData, nSize);
}

// FUNCTION: SWKOTOR 0x00677B10
int CSWCMessage::SendPlayerToServerInput_UseSkill(unsigned char a, unsigned char b,
                                                  unsigned long oid, const Vector& v)
{
    unsigned char* pData;
    unsigned long nSize = 0x12;
    CreateWriteMessage(nSize, -1, 1);
    WriteBYTE(a, 8);
    WriteBYTE(b, 8);
    WriteOBJECTID(oid);
    WriteFLOAT(v.x, 1.0f, 0x20);
    WriteFLOAT(v.y, 1.0f, 0x20);
    WriteFLOAT(v.z, 1.0f, 0x20);
    if (!GetWriteMessage(&pData, &nSize)) {
        return 0;
    }
    return SendPlayerToServerMessage(6, 7, pData, nSize);
}

// FUNCTION: SWKOTOR 0x00677BD0
int CSWCMessage::SendPlayerToServerInput_UseItem(unsigned long oidItem, unsigned char nSlot,
                                                 unsigned char nSub, unsigned long oidArea,
                                                 const Vector* pPos)
{
    unsigned char* pData;
    unsigned long nSize = 0x15;
    CreateWriteMessage(nSize, -1, 1);
    WriteOBJECTID(oidItem);
    WriteBYTE(nSlot, 8);
    if (nSub != 0xff) {
        WriteBOOL(1);
        WriteBYTE(nSub, 8);
    }
    else {
        WriteBOOL(0);
    }
    if (oidArea != OBJECT_INVALID) {
        WriteBOOL(1);
        WriteOBJECTID(oidArea);
    }
    else {
        WriteBOOL(0);
    }
    Vector vZero;
    vZero.x = 0.0f;
    vZero.y = 0.0f;
    vZero.z = 0.0f;
    if (!(*pPos == vZero)) {
        WriteBOOL(1);
        WriteFLOAT(pPos->x, 1.0f, 0x20);
        WriteFLOAT(pPos->y, 1.0f, 0x20);
        WriteFLOAT(pPos->z, 1.0f, 0x20);
    }
    else {
        WriteBOOL(0);
    }
    if (!GetWriteMessage(&pData, &nSize)) {
        return 0;
    }
    return SendPlayerToServerMessage(6, 9, pData, nSize);
}

// FUNCTION: SWKOTOR 0x00677D10
int CSWCMessage::SendPlayerToServer_06_0B(unsigned long a)
{
    unsigned char* pData;
    unsigned long nSize = 4;
    CreateWriteMessage(nSize, -1, 1);
    WriteOBJECTID(a);
    if (!GetWriteMessage(&pData, &nSize)) {
        return 0;
    }
    return SendPlayerToServerMessage(6, 0xb, pData, nSize);
}

// FUNCTION: SWKOTOR 0x00677D70
int CSWCMessage::SendPlayerToServer_06_03(unsigned long a, unsigned short b)
{
    unsigned char* pData;
    unsigned long nSize = 6;
    CreateWriteMessage(nSize, -1, 1);
    WriteOBJECTID(a);
    WriteWORD(b, 0x10);
    if (!GetWriteMessage(&pData, &nSize)) {
        return 0;
    }
    return SendPlayerToServerMessage(6, 3, pData, nSize);
}

// FUNCTION: SWKOTOR 0x00677DE0
int CSWCMessage::SendPlayerToServer_06_0C(unsigned long a, unsigned long b)
{
    unsigned char* pData;
    unsigned long nSize = 8;
    CreateWriteMessage(nSize, -1, 1);
    WriteOBJECTID(a);
    WriteOBJECTID(b);
    if (!GetWriteMessage(&pData, &nSize)) {
        return 0;
    }
    return SendPlayerToServerMessage(6, 0xc, pData, nSize);
}

// FUNCTION: SWKOTOR 0x00677E50
int CSWCMessage::SendPlayerToServer_06_21()
{
    return SendPlayerToServerMessage(6, 0x21, NULL, 0);
}

// FUNCTION: SWKOTOR 0x00677EA0
int CSWCMessage::SendPlayerToServer_06_08(unsigned long a)
{
    if (!g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pB4) {
        unsigned char* pData;
        unsigned long nSize = 4;
        CreateWriteMessage(nSize, -1, 1);
        WriteOBJECTID(a);
        if (!GetWriteMessage(&pData, &nSize)) {
            return 0;
        }
        return SendPlayerToServerMessage(6, 8, pData, nSize);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00677F30
int CSWCMessage::SendPlayerToServer_0C_01(unsigned long a, unsigned long b)
{
    unsigned char* pData;
    unsigned long nSize = 8;
    CreateWriteMessage(nSize, -1, 1);
    WriteOBJECTID(a);
    WriteDWORD(b, 0x20);
    if (!GetWriteMessage(&pData, &nSize)) {
        return 0;
    }
    return SendPlayerToServerMessage(0xc, 1, pData, nSize);
}

// FUNCTION: SWKOTOR 0x00677FA0
int CSWCMessage::SendPlayerToServerLogin_LoginLocalCharacter(unsigned char* pData,
                                                             unsigned long nSize, int bFlag)
{
    unsigned char* pBuffer;
    unsigned long nLength;
    CreateWriteMessage(nSize + 4, -1, 1);
    WriteDWORD(nSize, 0x20);
    WriteVOIDPtr(pData, nSize);
    if (!GetWriteMessage(&pBuffer, &nLength)) {
        return 0;
    }
    if (!bFlag) {
        return SendPlayerToServerMessage(2, 1, pBuffer, nLength);
    }
    return SendPlayerToServerMessage(2, 0x13, pBuffer, nLength);
}

// FUNCTION: SWKOTOR 0x00678030
int CSWCMessage::SendPlayerToServer_02_0F()
{
    return SendPlayerToServerMessage(2, 0xf, NULL, 0);
}
