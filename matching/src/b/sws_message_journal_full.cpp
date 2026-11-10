// CSWSMessage::SendServerToPlayerJournalFullUpdate: full or incremental
// journal update (major 0x1c, minor 9).
#define B_PLAYER_CLIENT_ENTRY_FIELDS
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"
#include "b/party_table.h"

// FUNCTION: SWKOTOR 0x00577B60
int CSWSMessage::SendServerToPlayerJournalFullUpdate(CSWSPlayer* pPlayer)
{
    int bFull = 0;
    CExoArrayList<CSWSPlayerClientEntry>* pUpdates = NULL;
    CSWSCreature* pCreature = NULL;
    if (pPlayer->GetGameObject() != NULL) {
        pCreature = pPlayer->GetGameObject()->AsSWSCreature();
    }
    if (pCreature == NULL) {
        return SendServerToPlayerJournalFullUpdateNotNeeded(pPlayer, 1);
    }
    CSWSJournal* pJournal = pCreature->GetJournal();
    if (pJournal == NULL) {
        return SendServerToPlayerJournalFullUpdateNotNeeded(pPlayer, 1);
    }
    int nCount;
    int nJournalCount;
    if (pPlayer->m_pClientInfo->m_bDirty) {
        nCount = pJournal->m_aEntries.num;
        bFull = 1;
    } else {
        if (pPlayer->m_pClientInfo->m_aEntries.num <= 0) {
            return SendServerToPlayerJournalFullUpdateNotNeeded(pPlayer, 0);
        }
        pUpdates = &pPlayer->m_pClientInfo->m_aEntries;
        if (pUpdates == NULL) {
            return SendServerToPlayerJournalFullUpdateNotNeeded(pPlayer, 0);
        }
        nCount = pUpdates->num;
        nJournalCount = pJournal->m_aEntries.num;
    }
    unsigned long nSize = nCount * 0x1e + 0xc;
    CreateWriteMessage(nSize, 0xffffffff, 1);
    WriteBOOL(1);
    WriteBOOL(bFull);
    WriteDWORD(nCount, 0x20);
    if (bFull) {
        for (int i = 0; i < nCount; i++) {
            unsigned short nEntryFlags = 0x8000;
            if (!(pJournal->m_aEntries[i].m_nFlags & 4)) {
                nEntryFlags |= 0x100;
            }
            WriteWORD(nEntryFlags, 0x10);
            WriteCExoString(pJournal->m_aEntries[i].m_sPlotID, 0x20);
            WriteCExoLocStringServer(pJournal->m_aEntries[i].m_lsName, 0);
            WriteCExoLocStringServer(pJournal->m_aEntries[i].m_lsText, 0);
            WriteDWORD(pJournal->m_aEntries[i].m_nDate, 0x20);
            WriteDWORD(pJournal->m_aEntries[i].m_nTime, 0x20);
            WriteDWORD(pJournal->m_aEntries[i].m_nState, 0x20);
            WriteDWORD(pJournal->m_aEntries[i].m_n24, 0x20);
            WriteWORD(pJournal->m_aEntries[i].m_nPlanetID, 0x10);
            WriteBOOL(pJournal->m_aEntries[i].m_nFlags & 1);
            WriteDWORD(pJournal->m_aEntries[i].m_n30, 0x20);
            WriteDWORD(pJournal->m_aEntries[i].m_n34, 0x20);
            WriteDWORD(pJournal->m_aEntries[i].m_n38, 0x20);
            pJournal->m_aEntries[i].m_nFlags &= ~4;
        }
    } else {
        for (int i = 0; i < nCount; i++) {
            unsigned short nFlags = pUpdates->element[i].m_nFlags;
            if (nFlags & 0x4000) {
                WriteWORD(nFlags, 0x10);
                WriteCExoString(pUpdates->element[i].m_sTag, 0x20);
            } else {
                int j;
                int bFound = 0;
                for (j = 0; j < nJournalCount; j++) {
                    if (pJournal->m_aEntries[j].m_sPlotID == pUpdates->element[i].m_sTag) {
                        bFound = 1;
                        break;
                    }
                }
                if (bFound) {
                    WriteWORD(nFlags, 0x10);
                    WriteCExoString(pJournal->m_aEntries[j].m_sPlotID, 0x20);
                    if (nFlags & 0x8001) {
                        WriteCExoLocStringServer(pJournal->m_aEntries[j].m_lsName, 0);
                    }
                    if (nFlags & 0x8002) {
                        WriteCExoLocStringServer(pJournal->m_aEntries[j].m_lsText, 0);
                    }
                    if (nFlags & 0x8004) {
                        WriteDWORD(pJournal->m_aEntries[j].m_nDate, 0x20);
                    }
                    if (nFlags & 0x8008) {
                        WriteDWORD(pJournal->m_aEntries[j].m_nTime, 0x20);
                    }
                    if (nFlags & 0x8010) {
                        WriteDWORD(pJournal->m_aEntries[j].m_nState, 0x20);
                    }
                    if (nFlags & 0x8020) {
                        WriteDWORD(pJournal->m_aEntries[j].m_n24, 0x20);
                    }
                    if (nFlags & 0x8040) {
                        WriteWORD(pJournal->m_aEntries[j].m_nPlanetID, 0x10);
                    }
                    if (nFlags & 0x8080) {
                        WriteBOOL(pJournal->m_aEntries[j].m_nFlags & 1);
                    }
                    if (nFlags & 0x8200) {
                        WriteDWORD(pJournal->m_aEntries[j].m_n30, 0x20);
                    }
                    if (nFlags & 0x8400) {
                        WriteDWORD(pJournal->m_aEntries[j].m_n34, 0x20);
                    }
                    if (nFlags & 0x8800) {
                        WriteDWORD(pJournal->m_aEntries[j].m_n38, 0x20);
                    }
                }
            }
        }
    }
    unsigned char* pData;
    if (!GetWriteMessage(&pData, &nSize)) {
        return 0;
    }
    if (pUpdates != NULL) {
        pUpdates->SetSize(0);
    }
    pPlayer->m_pClientInfo->m_bDirty = 0;
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x1c, 9, pData, nSize);
}
