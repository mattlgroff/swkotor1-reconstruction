// Player-to-server journal message.
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"

// FUNCTION: SWKOTOR 0x005278D0
int CSWSMessage::HandlePlayerToServerJournalMessage(CSWSPlayer* pPlayer, unsigned char nMinor)
{
    unsigned long nCalendarDay;
    unsigned long nTimeOfDay;
    CExoString sTitle;
    CExoString sEntry;
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    switch (nMinor) {
    case 0xa:
        pPlayer->m_pClientInfo->SetFullUpdate(1);
        SendServerToPlayerJournalFullUpdate(pPlayer);
        break;
    case 0xb:
        pPlayer->m_pClientInfo->SetFullUpdate(0);
        {
            CSWSPlayerClientInfo* pInfo = pPlayer->m_pClientInfo;
            pInfo->m_aEntries.SetSize(0);
        }
        break;
    case 0xd: {
        sTitle = ReadCExoString(0x20);
        sEntry = ReadCExoString(0x20);
        if (MessageReadOverflow(1) || MessageReadUnderflow(1)) {
            return 0;
        }
        if (pModule) {
            g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nCalendarDay,
                                                                            &nTimeOfDay);
            pModule->AddWorldJournalEntry(pModule->GetNextWorldJournalEntryID(), sEntry, sTitle,
                                          nCalendarDay, nTimeOfDay);
        }
        break;
    }
    case 0xe: {
        unsigned long nId = ReadDWORD(0x20);
        int bStrref = ReadBOOL();
        if (MessageReadOverflow(1) || MessageReadUnderflow(1)) {
            return 0;
        }
        if (pModule) {
            if (bStrref) {
                pModule->DeleteWorldJournalEntryStrref(nId);
            } else {
                pModule->DeleteWorldJournalEntry(nId);
            }
        }
        break;
    }
    }
    return 1;
}
