// Safe projectile packets; field names not established by the records are descriptive.
#include "b/sws_message.h"
#include "b/sws_player.h"

// FUNCTION: SWKOTOR 0x0056E5F0
int CSWSMessage::SendServerToPlayerSafeProjectileProjectile(CSWSPlayer* pPlayer,
    unsigned long oidSource, unsigned long oidTarget, Vector vSource, Vector vTarget,
    unsigned long nProjectile, unsigned char nType, int nTravelTime,
    unsigned char nFlags, unsigned char nAttackType, int nExtra)
{
    int nResult = 0;
    unsigned char* pMessage = NULL;
    unsigned long nSize = 0;
    CreateWriteMessage(0x2f, 0xffffffff, 1);
    WriteOBJECTIDServer(oidSource);
    WriteOBJECTIDServer(oidTarget);
    WriteVector(vSource);
    WriteVector(vTarget);
    WriteDWORD(nProjectile, 32);
    WriteBYTE(nType, 8);
    WriteINT(nTravelTime, 32);
    WriteBYTE(nFlags, 8);
    WriteBYTE(nAttackType, 8);
    WriteINT(nExtra, 32);
    if (GetWriteMessage(&pMessage, &nSize)) {
        nResult = SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x22, 2, pMessage, nSize);
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x0056E6E0
int CSWSMessage::SendServerToPlayerSafeProjectileSpell(CSWSPlayer* pPlayer,
    unsigned long oidSource, unsigned long oidTarget, Vector vSource, Vector vTarget,
    unsigned long nProjectile, unsigned char nType, int nExtra,
    unsigned char nItem, unsigned char nFlags, unsigned char nAttackType,
    int nExtra1, int nExtra2)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(0x31, 0xffffffff, 1);
    WriteOBJECTIDServer(oidSource);
    WriteOBJECTIDServer(oidTarget);
    WriteFLOAT(vSource.x, 1.0f, 32);
    WriteFLOAT(vSource.y, 1.0f, 32);
    WriteFLOAT(vSource.z, 1.0f, 32);
    WriteFLOAT(vTarget.x, 1.0f, 32);
    WriteFLOAT(vTarget.y, 1.0f, 32);
    WriteFLOAT(vTarget.z, 1.0f, 32);
    WriteDWORD(nProjectile, 32);
    WriteBYTE(nType, 8);
    WriteINT(nExtra1, 32);
    WriteINT(nExtra2, 32);
    if (nType == 6) {
        WriteINT(nExtra, 32);
    } else if (nType == 7) {
        WriteINT(nExtra, 32);
        WriteBYTE(nAttackType, 8);
    } else {
        WriteBYTE(nAttackType, 8);
        WriteBYTE(nItem, 8);
        WriteBYTE(nFlags, 8);
    }
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x22, 1, pMessage, nSize);
}
