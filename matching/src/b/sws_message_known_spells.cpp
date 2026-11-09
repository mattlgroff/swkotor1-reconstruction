// Known spell add/delete lists of the player's last update.
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"

// NONMATCH (15): stack slots. The original has one more local (frame 0x14):
// the matched count lives in a second byte inside the first loop and is
// copied out afterwards, and the spell id shares the creature argument slot.
// Chained/comma initialisers, a separate counter and an inline helper tried.
// FUNCTION: SWKOTOR 0x00570930
int CSWSMessage::ComputeLastUpdate_GuiKnownSpells(CSWSCreature* pCreature,
                                                  CSWSPlayerLastUpdateObject* pLast)
{
    CSWSCreatureStats* pStats = pCreature->m_pStats;
    unsigned char nList;
    for (nList = 0; nList < 2; nList++) {
        unsigned char nKnown = 0;
        unsigned char i;
        for (i = 0; i < pLast->GetNumberKnownSpells(nList); i++) {
            unsigned long nSpell = pLast->GetKnownSpell(nList, 0, i);
            if (nSpell == pStats->GetKnownSpell(nList, 0, nKnown)) {
                nKnown++;
            } else {
                pLast->m_aKnownSpellsDeleted[nList].Add(nSpell);
            }
        }
        for (i = nKnown; i < pStats->GetNumberKnownSpells(nList, 0); i++) {
            unsigned long nSpell = pStats->GetKnownSpell(nList, 0, i);
            pLast->m_aKnownSpellsAdded[nList].Add(nSpell);
        }
    }
    for (nList = 0; nList < 2; nList++) {
        if (pLast->m_aKnownSpellsAdded[nList].num != 0 ||
            pLast->m_aKnownSpellsDeleted[nList].num != 0) {
            return 1;
        }
    }
    return 0;
}
