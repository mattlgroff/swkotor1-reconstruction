// CSWSCreature::CanEquipItem (nwscreatureai.cpp).
#include "a/sws_creature.h"

// NONMATCH: 15 diff: plain return-0 paths merge into the first feedback block instead of the feat one, and the
// final slot test is if-converted instead of stored and branched.
// FUNCTION: SWKOTOR 0x0051AA60
unsigned char CSWSCreature::CanEquipItem(CSWSItem* pItem, unsigned long* pSlot, int nMode, int bBypass,
                                         int bFeedback)
{
    if (nMode == 1 && m_pStats->m_bIsPC == 1 && !bBypass &&
        m_pStats->GetLevel(0) < pItem->GetMinEquipLevel() && GetServerExoApp()->GetServerInfo()->m_nC8) {
        if (bFeedback == 1) SendFeedbackMessage(0x62, 0);
        return 0;
    }
    if (pItem->m_n288 & 0x40) return 0;
    if (!CheckProficiencies(pItem, *pSlot)) return 0;
    if (!CheckItemAlignmentRestrictions(pItem)) {
        if (nMode == 1 && bFeedback == 1) SendFeedbackMessage(0xcf, 0);
        return 0;
    }
    if (!CheckItemClassRestrictions(pItem)) {
        if (nMode == 1 && bFeedback == 1) SendFeedbackMessage(0xd0, 0);
        return 0;
    }
    if (!CheckItemRaceRestrictions(pItem)) {
        if (nMode == 1 && bFeedback == 1) SendFeedbackMessage(0xd1, 0);
        return 0;
    }
    if (!CheckItemFeatRestrictions(pItem)) {
        if (nMode == 1 && bFeedback == 1) SendFeedbackMessage(0x6b, 0);
        return 0;
    }
    unsigned char nResult;
    if (*pSlot == 0x10 || *pSlot == 0x20) nResult = Fn510CD0(pItem, pSlot, nMode, bFeedback);
    else nResult = CheckWeaponSlot(pItem, pSlot, nMode, bFeedback);
    CSWBaseItem* pBase = g_pRules->GetBaseItem(pItem->m_nBaseItem);
    if (!pBase) return 0;
    nMode = (pBase->m_nEquipableSlots & *pSlot) != 0;
    if (!nMode) return 0;
    return nResult;
}
