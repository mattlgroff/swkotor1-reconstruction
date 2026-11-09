// Character list entry for the play-module character list.
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"

// FUNCTION: SWKOTOR 0x0056B400
int CSWSMessage::SendServerToPlayerPlayModuleCharacterListResponse(unsigned long nPlayerId,
                                                                   unsigned long oidCharacter,
                                                                   int bAdd)
{
    unsigned char* pData;
    unsigned long nSize = 4;
    CreateWriteMessage(nSize, 0xffffffff, 1);
    WriteBOOL(bAdd);
    WriteDWORD(oidCharacter, 0x20);
    if (bAdd) {
        CSWSCreature* pCreature =
            g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCharacter);
        if (!pCreature) {
            return 0;
        }
        CExoLocString sFirstName;
        CExoLocString sLastName;
        unsigned char nGender = pCreature->GetGender();
        sFirstName = *pCreature->GetFirstName();
        sLastName = *pCreature->GetLastName();
        WriteCExoLocStringServer(sFirstName, nGender);
        WriteCExoLocStringServer(sLastName, nGender);
        unsigned short nPortraitId = pCreature->GetPortraitId();
        WriteWORD(nPortraitId, 0x10);
        if (nPortraitId == 0xffff || nPortraitId == 0xfffe) {
            WriteCResRef(pCreature->GetPortrait(), 0x10);
        }
        unsigned char nClasses = pCreature->GetStats()->m_nNumMultiClasses;
        WriteBYTE(nClasses, 8);
        for (int i = 0; i < nClasses; i++) {
            unsigned char nClass = pCreature->GetStats()->GetClass(i);
            unsigned char nLevel = pCreature->GetStats()->GetClassLevel(i, 0);
            WriteBYTE(nClass, 8);
            WriteBYTE(nLevel, 8);
        }
    }
    if (!GetWriteMessage(&pData, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0x31, 3, pData, nSize);
}
