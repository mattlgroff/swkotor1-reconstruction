// Compact object-update dispatchers.
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"
#include "b/exo_array_list.h"

class CSWSPlayerCharSheetGUI {
public:
    unsigned long ComputeCharacterSheetUpdateRequired(CSWSPlayer* pPlayer); // 0x00561260
};

// FUNCTION: SWKOTOR 0x00576FC0
void CSWSMessage::StoreValuesInLastUpdateObject(CSWSPlayer* pPlayer,
    CLastUpdateObject* pLast, CSWSObject* pObject, unsigned long nFlags,
    unsigned long nAppearanceFlags)
{
    UpdateLastUpdateObject(pPlayer, pObject, pLast, nFlags);
    UpdateLastUpdateObjectAppearance(pObject, pLast, nAppearanceFlags);
}

// FUNCTION: SWKOTOR 0x00572990
void CSWSMessage::WriteGameObjUpdate_MinorGUIPanels(CSWSPlayer* pPlayer)
{
    CExoArrayList<unsigned long> aUpdates;
    CGameObject* pObject = pPlayer->GetGameObject();
    if (pObject && pObject->AsSWSCreature()) {
        unsigned long nRequired = ((CSWSPlayerCharSheetGUI*)pPlayer->m_pInventoryGUI)->
            ComputeCharacterSheetUpdateRequired(pPlayer);
        if (nRequired) {
            WriteGameObjUpdate_CharacterSheet(pPlayer, nRequired);
        }
    }
}

// FUNCTION: SWKOTOR 0x00577FA0
// NONMATCH: comparison materializes through EAX instead of the original SETE DL.
void CSWSMessage::TestObjectUpdateDifferences(CSWSPlayer* pPlayer, CSWSObject* pObject,
    CLastUpdateObject** ppLast, unsigned long* pFlags, unsigned long* pAppearanceFlags)
{
    CGameObject* pPlayerObject = pPlayer->GetGameObject();
    void* pPos = pPlayer->m_pLastUpdateObjects->GetHeadPos();
    while (pPos) {
        *ppLast = pPlayer->m_pLastUpdateObjects->GetAtPos(pPos);
        if ((*ppLast)->m_nId == pObject->m_idSelf) {
            (*ppLast)->m_bUpdated = 1;
            int bSelf = pObject->m_idSelf == pPlayerObject->m_idSelf;
            *pFlags = ComputeUpdateRequired(pPlayer, pObject, *ppLast, bSelf);
            *pAppearanceFlags = ComputeAppearanceUpdateRequired(pObject, *ppLast);
            return;
        }
        pPlayer->m_pLastUpdateObjects->GetNext(pPos);
    }
    *ppLast = CreateNewLastUpdateObject(pPlayer, pObject, pFlags, pAppearanceFlags);
}
