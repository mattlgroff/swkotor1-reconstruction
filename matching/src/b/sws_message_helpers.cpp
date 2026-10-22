// CSWSMessage game object update helpers (0x00566DA0-0x005704F0).
#include "b/sws_message.h"
#include "b/sws_player.h"
#include "b/sws_creature.h"
#include "b/sws_item.h"

class CSWSDoor : public CSWSObject {
public:
    char pad_0xf4[0x2a0 - 0xf4];
    unsigned char m_nAnimationState;                // 0x2a0
    unsigned char m_nOpenState;                     // 0x2a1
    char pad_0x2a2[0x3c0 - 0x2a2];
    int m_bLocked;                                  // 0x3c0
};

class CLoopingVisualEffect {
public:
    int GetIsBeam();                                // 0x004CA9D0

    unsigned short m_nId;                           // 0x0
    char pad_0x2[0x4 - 0x2];
    unsigned long m_oidTarget;                      // 0x4
};

class CSWSTrigger : public CSWSObject {
public:
    char pad_0xf4[0x284 - 0xf4];
    int m_nVertices;                                // 0x284
    Vector* m_pVertices;                            // 0x288
};

// Object type of creatures (.rdata 0x00746634).
extern const unsigned char OBJECT_TYPE_CREATURE;

// Classes that use spells per day (cdecl).
int IsSpellCasterClass(int nClass);                 // 0x005BE4A0

// FUNCTION: SWKOTOR 0x00566DA0
void CSWSMessage::AddDoorAppearanceToMessage(CSWSDoor* pDoor)
{
    WriteBOOL(pDoor->m_bLocked);
    WriteBYTE(pDoor->m_nAnimationState, 8);
    if (pDoor->m_nAnimationState == 0) {
        WriteBYTE(pDoor->m_nOpenState, 8);
    }
}

// FUNCTION: SWKOTOR 0x00566DF0
void CSWSMessage::AddPlaceableAppearanceToMessage(CSWSPlaceable* pPlaceable)
{
    WriteBYTE(pPlaceable->GetBodyBag(), 8);
    WriteWORD(pPlaceable->GetD4(), 16);
    WriteBOOL(pPlaceable->GetF8());
    WriteBOOL(pPlaceable->Get328());
    WriteBOOL(pPlaceable->Get280());
    WriteBOOL(pPlaceable->Get330());
    WriteBOOL(pPlaceable->Get260());
    WriteBOOL(pPlaceable->Get32C());
    CExoString sDescription;
    GetLocStringServer(0xffffffff, pPlaceable->m_sDescription, CExoLocString(), sDescription, 0);
    int bDescription = 0;
    if (!sDescription.IsEmpty()) {
        bDescription = 1;
    }
    WriteBOOL(bDescription);
}

#pragma inline_depth(0)
// FUNCTION: SWKOTOR 0x00566F20
// NONMATCH: 3 diff insns, the original pushes nBits after the second and third
// GetActiveProperty calls (mine before).
void CSWSMessage::AddActiveItemPropertiesToMessage(CSWSItem* pItem, CSWSCreature* pCreature)
{
    if (pItem->GetBaseItem()->m_nBaseType >= 0x1f && pItem->GetBaseItem()->m_nBaseType <= 0x26) {
        WriteWORD(pItem->m_nUpgradeLevel, 16);
    }
    WriteCExoLocStringServer(pItem->m_lsLocalizedName, 0);
    WriteBOOL(1);
    WriteINT(pItem->GetCost(), 32);
    WriteINT(pItem->m_nStackCount, 32);
    WriteBOOL(pCreature ? pCreature->CanUseItem(pItem, 1) : 0);
    WriteBOOL(pItem->m_bStolen);
    int nActive = pItem->m_nActiveProperties;
    WriteBYTE(nActive, 8);
    for (int i = 0; i < nActive; i++) {
        WriteWORD(pItem->GetActiveProperty(i)->m_nPropertyName, 16);
        WriteWORD(pItem->GetActiveProperty(i)->m_nSubType, 16);
        WriteWORD(pItem->GetActiveProperty(i)->m_nCostValue, 16);
        WriteBYTE(pItem->GetActiveProperty(i)->m_nParam1Value, 8);
    }
    WriteBYTE(pItem->m_nUseableProperties, 8);
    WriteBYTE(0xff, 8);
    for (unsigned char j = 0; j < 8; j++) {
        WriteBYTE(pItem->m_aUsesLeft[j], 8);
    }
}

#pragma inline_depth()

// FUNCTION: SWKOTOR 0x00567090
void CSWSMessage::AddItemAppearanceToMessage(CSWSItem* pItem)
{
    int nBaseItem = pItem->m_nBaseItem;
    WriteINT(nBaseItem, 32);
    CSWBaseItem* pBaseItem = g_pRules->m_pBaseItemArray->GetBaseItem(nBaseItem);
    if (pBaseItem) {
        WriteBYTE(pItem->m_nIcon, 8);
        if (pBaseItem->m_bIconVariant == 1) {
            WriteBYTE(pItem->m_n265, 8);
            WriteBYTE(pItem->m_nIconVariant, 8);
        }
    }
}

// FUNCTION: SWKOTOR 0x00567100
unsigned long CSWSMessage::ComputeAppearanceUpdateRequired(CGameObject* pObject,
                                                           CLastUpdateObject* pLast)
{
    unsigned long nFlags = 0;
    if (pObject->m_nObjectType != OBJECT_TYPE_CREATURE) {
        return 0;
    }
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    pCreature->UpdateAppearanceForEquippedItems();
    CSWSCreatureAppearanceInfo& cLast = pLast->m_cAppearance;
    CSWSCreatureAppearanceInfo& cNow = pCreature->m_cAppearance;
    if (cLast.m_nAppearance != cNow.m_nAppearance) nFlags = 1;
    if (cLast.m_n12 != cNow.m_n12) nFlags |= 2;
    if (cLast.m_n13 != cNow.m_n13) nFlags |= 4;
    if (cLast.m_n14 != cNow.m_n14) nFlags |= 8;
    if (cLast.m_n15 != cNow.m_n15) nFlags |= 0x10;
    if (cLast.m_n16 != cNow.m_n16) nFlags |= 0x20;
    if (cLast.m_n17 != cNow.m_n17) nFlags |= 0x40;
    if (cLast.m_n18 != cNow.m_n18) nFlags |= 0x80;
    if (cLast.m_n1C != cNow.m_n1C) nFlags |= 0x4000;
    if (cLast.m_n20 != cNow.m_n20) nFlags |= 0x8000;
    if (cLast.m_n19 != cNow.m_n19) nFlags |= 0x800;
    if (cLast.m_n1A != cNow.m_n1A) nFlags |= 0x1000;
    if (cLast.m_aParts[0] != cNow.m_aParts[0] || cLast.m_aParts[1] != cNow.m_aParts[1] ||
        cLast.m_aParts[2] != cNow.m_aParts[2] || cLast.m_aParts[3] != cNow.m_aParts[3]) {
        nFlags |= 0x200;
    }
    if (pLast->m_n2A != pCreature->m_n9D8) nFlags |= 0x2000;
    return nFlags;
}

// FUNCTION: SWKOTOR 0x00567240
int CSWSMessage::ComputeInventoryUpdateRequired(CSWSPlayer* pPlayer, unsigned long nSlot)
{
    if (pPlayer && pPlayer->GetGameObject()) {
        CGameObject* pObject = pPlayer->GetGameObject();
        if (pObject->AsSWSCreature() && pObject->AsSWSCreature()->m_pInventory) {
            CSWInventory* pInventory = pObject->AsSWSCreature()->m_pInventory;
            CSWSItem* pItem = pInventory->GetItemInSlot(1 << nSlot);
            if (pItem) {
                if (pPlayer->m_pStoreGUI->m_pStore->m_aSlotItems[nSlot] != pItem->m_idSelf) {
                    return 1;
                }
                return pItem->m_nStackCount !=
                       pPlayer->m_pStoreGUI->m_pStore->m_aSlotStackSize[nSlot];
            }
            if (pPlayer->m_pStoreGUI->m_pStore->m_aSlotItems[nSlot] != OBJECT_INVALID_ID) {
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005672F0
int CSWSMessage::CompareVisualEffectLists(CExoArrayList<CLoopingVisualEffect*>* pList1,
                                          CExoArrayList<CLoopingVisualEffect*>* pList2)
{
    int bBeam;
    for (int i = 0; i < pList1->num; i++) {
        CLoopingVisualEffect* pEffect = pList1->element[i];
        unsigned short nId = pEffect->m_nId;
        int bFound = 0;
        bBeam = pEffect->GetIsBeam();
        for (int j = 0; j < pList2->num && !bFound; j++) {
            CLoopingVisualEffect* pOther = pList2->element[j];
            if (pOther->m_nId == nId && (!bBeam || pOther->m_oidTarget == pEffect->m_oidTarget)) {
                bFound = 1;
            }
        }
        if (!bFound) {
            return 0;
        }
    }
    for (int i = 0; i < pList2->num; i++) {
        CLoopingVisualEffect* pEffect = pList2->element[i];
        unsigned short nId = pEffect->m_nId;
        int bFound = 0;
        bBeam = pEffect->GetIsBeam();
        for (int j = 0; j < pList1->num && !bFound; j++) {
            CLoopingVisualEffect* pOther = pList1->element[j];
            if (pOther->m_nId == nId && (!bBeam || pOther->m_oidTarget == pEffect->m_oidTarget)) {
                bFound = 1;
            }
        }
        if (!bFound) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00567430
unsigned short CSWSMessage::ComputeLastUpdate_GuiSkills(CSWSCreature* pCreature)
{
    CSWSCreatureStats* pStats = pCreature->m_pStats;
    unsigned short nFlags = 0;
    if (pStats->GetCanUseSkill(1)) nFlags = 2;
    if (pStats->GetCanUseSkill(2)) nFlags |= 0x10;
    if (pStats->GetCanUseSkill(2)) nFlags |= 0x10;
    if (pStats->GetCanUseSkill(6)) nFlags |= 0x20;
    return nFlags;
}

// FUNCTION: SWKOTOR 0x00567490
int CSWSMessage::ComputeLastUpdate_GuiKnownSpellUses(CSWSCreature* pCreature,
                                                     CSWSPlayerLastUpdateObject* pLast)
{
    for (int i = 0; i < pCreature->m_pStats->m_nNumMultiClasses; i++) {
        if (IsSpellCasterClass(pCreature->m_pStats->GetClass(i))) {
            for (int j = 0; j < 1; j++) {
                unsigned char nUses = pLast->GetKnownSpellUsesLeft(i, j);
                if (nUses != pCreature->m_pStats->GetSpellUsesLeft(i, j)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00567520
// NONMATCH: 7 diff insns, the original walks the creature array from &m_n4
// (+0x3ac, fields at -4/-2) where mine starts at the entry (+0x3a8).
void CSWSMessage::StoreLastUpdate_ActionQueue(CSWSCreature* pCreature,
                                              CSWSPlayerLastUpdateObject* pLast)
{
    for (int i = 0; i < 10; i++) {
        pLast->m_aActionQueue[i].m_n2 = pCreature->m_aActionQueue[i].m_n2;
        pLast->m_aActionQueue[i].m_n0 = pCreature->m_aActionQueue[i].m_n0;
        pLast->m_aActionQueue[i].m_n4 = pCreature->m_aActionQueue[i].m_n4;
        pLast->m_aActionQueue[i].m_n18 = pCreature->m_aActionQueue[i].m_n18;
        pLast->m_aActionQueue[i].m_vC = pCreature->m_aActionQueue[i].m_vC;
        pLast->m_aActionQueue[i].m_n8 = pCreature->m_aActionQueue[i].m_n8;
    }
}

// FUNCTION: SWKOTOR 0x00567590
// NONMATCH: 5 diff insns, same +0x3ac vs +0x3a8 induction base as 0x00567520.
int CSWSMessage::ComputeLastUpdate_ActionQueue(CSWSCreature* pCreature,
                                               CSWSPlayerLastUpdateObject* pLast)
{
    for (int i = 0; i < 10; i++) {
        if (pCreature->m_aActionQueue[i].m_n2 != pLast->m_aActionQueue[i].m_n2 ||
            pCreature->m_aActionQueue[i].m_n0 != pLast->m_aActionQueue[i].m_n0 ||
            pCreature->m_aActionQueue[i].m_n4 != pLast->m_aActionQueue[i].m_n4 ||
            pCreature->m_aActionQueue[i].m_n18 != pLast->m_aActionQueue[i].m_n18 ||
            !(pCreature->m_aActionQueue[i].m_vC == pLast->m_aActionQueue[i].m_vC)) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00567600
unsigned short CSWSMessage::ComputeLastUpdate_PlayerState(CSWSCreature* pCreature)
{
    unsigned short nFlags = 0;
    if (pCreature->GetMode() == 1) nFlags = 1;
    if (pCreature->m_bPlayerCharacter == 1) nFlags |= 2;
    if (pCreature->m_n4D2 == 1) nFlags |= 4;
    if (pCreature->m_n4D2 == 2) nFlags |= 8;
    if (pCreature->m_n4D2 == 3) nFlags |= 0x10;
    if (pCreature->m_n4D2 == 5) nFlags |= 0x20;
    if (pCreature->m_n4D2 == 6) nFlags |= 0x40;
    if (pCreature->m_n8E8 == 1) nFlags |= 0x80;
    if (pCreature->m_nA10) nFlags |= 0x100;
    if (pCreature->m_n4D2 == 4) nFlags |= 0x200;
    if (pCreature->m_bDisguised) nFlags |= 0x400;
    if (pCreature->m_bCommandable) nFlags |= 0x800;
    if (pCreature->m_n384 > 0) nFlags |= 0x1000;
    return nFlags;
}

// FUNCTION: SWKOTOR 0x005676B0
unsigned char CSWSMessage::ComputeLastUpdate_AssociateState(CSWSCreature* pCreature)
{
    unsigned char nFlags = 0;
    if (pCreature->GetMode() == 1) nFlags = 1;
    if (pCreature->m_bPlayerCharacter == 1) nFlags |= 2;
    if (pCreature->m_n4D2 == 1) nFlags |= 4;
    if (pCreature->m_n4D2 == 2) nFlags |= 8;
    if (pCreature->m_n4D2 == 3) nFlags |= 0x10;
    if (pCreature->m_n4D2 == 5) nFlags |= 0x20;
    if (pCreature->m_n4D2 == 6) nFlags |= 0x40;
    if (pCreature->m_n4D2 == 4) nFlags |= 0x80;
    return nFlags;
}

// FUNCTION: SWKOTOR 0x00567710
void CSWSMessage::WriteGameObjUpdate_WriteInventorySlotAdd(CSWSCreature* pCreature,
                                                           CSWSItem* pItem,
                                                           unsigned long nSlot)
{
    WriteBYTE(0x41, 8);
    WriteOBJECTIDServer(pItem->m_idSelf);
    WriteDWORD(nSlot, 32);
    if (nSlot == 0x10 || nSlot == 0x20) {
        WriteBOOL(pCreature->GetWeaponSlotFlag(nSlot));
    }
    AddItemAppearanceToMessage(pItem);
    AddActiveItemPropertiesToMessage(pItem, pCreature);
}

// FUNCTION: SWKOTOR 0x005683D0
void CSWSMessage::UpdateLastUpdateInventory(CSWSPlayer* pPlayer, unsigned long nSlot)
{
    if (pPlayer && pPlayer->GetGameObject()) {
        CGameObject* pObject = pPlayer->GetGameObject();
        if (pObject->AsSWSCreature() && pObject->AsSWSCreature()->m_pInventory) {
            CSWInventory* pInventory = pObject->AsSWSCreature()->m_pInventory;
            CSWSItem* pItem = pInventory->GetItemInSlot(1 << nSlot);
            if (pItem) {
                pPlayer->m_pStoreGUI->m_pStore->m_aSlotItems[nSlot] = pItem->m_idSelf;
                pPlayer->m_pStoreGUI->m_pStore->m_aSlotStackSize[nSlot] = pItem->m_nStackCount;
            }
            else {
                pPlayer->m_pStoreGUI->m_pStore->m_aSlotItems[nSlot] = OBJECT_INVALID_ID;
                pPlayer->m_pStoreGUI->m_pStore->m_aSlotStackSize[nSlot] = -1;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00567780
// NONMATCH: 36 diff insns, esi/edi swapped (object vs category).
unsigned long CSWSMessage::SelectCategoryForGameObject(CGameObject* pObject,
                                                       CGameObject* pPlayerObject)
{
    unsigned long nCategory = 0xff;
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (pPlayerObject->m_idSelf == pObject->m_idSelf) {
            nCategory = 0;
        }
        else {
            nCategory = pCreature->m_n4E0 == 1 ? 1 : 4;
        }
    }
    else if (pObject->AsSWSDoor()) {
        nCategory = 6;
    }
    else if (pObject->AsSWSPlaceable()) {
        if (!pObject->AsSWSPlaceable()->m_n398) {
            nCategory = 8;
        }
    }
    else if (pObject->AsSWSItem()) {
        nCategory = 7;
    }
    else if (pObject->AsSWSTrigger()) {
        nCategory = 9;
    }
    else if (pObject->AsSWSAreaOfEffectObject()) {
        nCategory = 5;
    }
    return nCategory;
}

// FUNCTION: SWKOTOR 0x00567BA0
void CSWSMessage::WriteGameObjUpdate_WorkRemaining(CGameObject* pObject, int bOnce, int nDone,
                                                   int nTotal)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature && (!bOnce || !pCreature->m_n320)) {
        if (nDone != nTotal || !pCreature->m_n320) {
            WriteBYTE(0x57, 8);
            WriteBYTE(nDone, 8);
            WriteBYTE(nTotal, 8);
            if (nDone == nTotal) {
                pCreature->m_n320 = 1;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00567C20
// NONMATCH: 3 diff insns, address operand order [edi+ecx] vs [ecx+edi].
void CSWSMessage::AddTriggerGeometryToMessage(CSWSTrigger* pTrigger)
{
    WriteBYTE(pTrigger->m_nVertices, 8);
    for (int i = 0; i < pTrigger->m_nVertices; i++) {
        WriteFLOAT(pTrigger->m_pVertices[i].x, 1.0f, 32);
        WriteFLOAT(pTrigger->m_pVertices[i].y, 1.0f, 32);
        WriteFLOAT(pTrigger->m_pVertices[i].z, 1.0f, 32);
    }
}

// FUNCTION: SWKOTOR 0x00568480
void CSWSMessage::UpdateLastUpdateObjectAppearance(CGameObject* pObject,
                                                   CLastUpdateObject* pLast,
                                                   unsigned long nFlags)
{
    if (pObject->m_nObjectType == OBJECT_TYPE_CREATURE) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (nFlags & 0x400) {
            pLast->m_lsFirstName = pCreature->m_pStats->m_lsFirstName;
            pLast->m_lsLastName = pCreature->m_pStats->m_lsLastName;
        }
        pLast->m_cAppearance.m_aParts[0] = pCreature->m_cAppearance.m_aParts[0];
        pLast->m_cAppearance.m_aParts[1] = pCreature->m_cAppearance.m_aParts[1];
        pLast->m_cAppearance.m_aParts[2] = pCreature->m_cAppearance.m_aParts[2];
        pLast->m_cAppearance.m_aParts[3] = pCreature->m_cAppearance.m_aParts[3];
        pLast->m_cAppearance.m_nAppearance = pCreature->m_cAppearance.m_nAppearance;
        pLast->m_cAppearance.m_n12 = pCreature->m_cAppearance.m_n12;
        pLast->m_cAppearance.m_n13 = pCreature->m_cAppearance.m_n13;
        pLast->m_cAppearance.m_n14 = pCreature->m_cAppearance.m_n14;
        pLast->m_cAppearance.m_n15 = pCreature->m_cAppearance.m_n15;
        pLast->m_cAppearance.m_n16 = pCreature->m_cAppearance.m_n16;
        pLast->m_cAppearance.m_n17 = pCreature->m_cAppearance.m_n17;
        pLast->m_cAppearance.m_n18 = pCreature->m_cAppearance.m_n18;
        pLast->m_cAppearance.m_n19 = pCreature->m_cAppearance.m_n19;
        pLast->m_cAppearance.m_n1A = pCreature->m_cAppearance.m_n1A;
        pLast->m_cAppearance.m_n1C = pCreature->m_cAppearance.m_n1C;
        pLast->m_cAppearance.m_n20 = pCreature->m_cAppearance.m_n20;
    }
}

template class CExoArrayList<unsigned char>;

// FUNCTION: SWKOTOR 0x0056B6F0 SYMBOL: ??1?$CExoArrayList@E@@QAE@XZ
// FUNCTION: SWKOTOR 0x0056B7E0 SYMBOL: ?Allocate@?$CExoArrayList@E@@QAEXH@Z
// FUNCTION: SWKOTOR 0x005704F0 SYMBOL: ?Add@?$CExoArrayList@E@@QAEXE@Z

// FUNCTION: SWKOTOR 0x0056B710 SYMBOL: ?Allocate@?$CExoArrayList@UCSWSStats_SpellLikeAbility@@@@QAEXH@Z
template class CExoArrayList<CSWSStats_SpellLikeAbility>;

// FUNCTION: SWKOTOR 0x0056B830
int CSWSMessage::CompareObjectIdLists(CExoArrayList<unsigned long>* pList1,
                                      CExoArrayList<unsigned long>* pList2)
{
    if (pList1->num != pList2->num) {
        return 0;
    }
    for (int i = 0; i < pList1->num; i++) {
        if (pList1->element[i] != pList2->element[i]) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0056B880
// NONMATCH: 1 diff insn, nLastCount goes to the dead pCreature arg slot in the
// original, to a frame slot here.
int CSWSMessage::ComputeVisibilityLists(CSWSCreature* pCreature,
                                        CSWSPlayerLastUpdateObject* pLast)
{
    int nLastCount = pLast->m_aVisibility.num;
    int nCount = pCreature->m_aVisibility.num;
    if (nCount != nLastCount) {
        return 1;
    }
    for (int i = 0; i < nCount; i++) {
        CSWSCreatureVisibility* pEntry = pCreature->m_aVisibility.element[i];
        CSWSCreatureVisibility* pMatch;
        int bFound = 0;
        for (int j = 0; j < nLastCount; j++) {
            if (pEntry->m_oidTarget == pLast->m_aVisibility.element[i]->m_oidTarget) {
                pMatch = pLast->m_aVisibility.element[i];
                bFound = 1;
            }
        }
        if (bFound != 1) {
            return 1;
        }
        if (pEntry->m_bSeen != pMatch->m_bSeen) {
            return 1;
        }
        if (pEntry->m_bHeard != pMatch->m_bHeard) {
            return 1;
        }
        if (pEntry->m_bVisible != pMatch->m_bVisible) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0056C310
// NONMATCH: 14 diff insns, the original loads the feat count as a dword then
// movzx, and its first early return has its own epilogue before `push ebx`.
int CSWSMessage::ComputeLastUpdate_GuiFeats(CSWSCreature* pCreature,
                                            CSWSPlayerLastUpdateObject* pLast)
{
    if (pCreature->m_pStats->GetNumFeats() != pLast->m_aFeats.num ||
        pCreature->m_pStats->GetNumBonusFeats() != pLast->m_aBonusFeats.num ||
        pCreature->m_pStats->m_lstFeatUses.num != pLast->m_aFeatUses.num) {
        return 1;
    }
    unsigned short i;
    for (i = 0; i < pCreature->m_pStats->GetNumFeats(); i++) {
        unsigned short nFeat = pCreature->m_pStats->GetFeat(i);
        int bFound = 0;
        for (int j = 0; j < pLast->m_aFeats.num; j++) {
            if (pLast->m_aFeats.element[j] == nFeat) {
                bFound = 1;
            }
        }
        if (!bFound) {
            return 1;
        }
    }
    for (i = 0; i < pCreature->m_pStats->GetNumBonusFeats(); i++) {
        unsigned short nFeat = pCreature->m_pStats->GetBonusFeat(i);
        int bFound = 0;
        for (int j = 0; j < pLast->m_aBonusFeats.num; j++) {
            if (pLast->m_aBonusFeats.element[j] == nFeat) {
                bFound = 1;
            }
        }
        if (!bFound) {
            return 1;
        }
    }
    CExoArrayList<CSWSStats_FeatUses*>& lstUses = pCreature->m_pStats->m_lstFeatUses;
    int nUses = lstUses.num;
    for (unsigned short k = 0; k < nUses; k++) {
        if (lstUses.element[k]->m_nFeat != pLast->m_aFeatUses.element[k]->m_nFeat) {
            return 1;
        }
        if (lstUses.element[k]->m_nUsedToday != pLast->m_aFeatUses.element[k]->m_nUsedToday) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0056C430
// NONMATCH: 10 diff insns, the original has a 4-byte frame and loads pLast first.
int CSWSMessage::ComputeLastUpdate_CompareSpellLikeAbility(CSWSCreature* pCreature,
                                                           CSWSPlayerLastUpdateObject* pLast)
{
    CExoArrayList<CSWSStats_SpellLikeAbility>* pLastList = pLast->m_pSpellLikeAbilities;
    CExoArrayList<CSWSStats_SpellLikeAbility>* pList = pCreature->m_pStats->m_pSpellLikeAbilities;
    int nCount = pList->num;
    int nDiff = 0;
    int nLastCount = 0;
    if (pLastList) {
        nLastCount = pLastList->num;
    }
    if (nLastCount != nCount) {
        nDiff = 1;
    }
    for (int i = 0; i < pList->num; i++) {
        if (i >= nLastCount ||
            pList->element[i].m_bReadied != pLastList->element[i].m_bReadied ||
            pList->element[i].m_nCasterLevel != pLastList->element[i].m_nCasterLevel ||
            pList->element[i].m_nSpellId != pLastList->element[i].m_nSpellId) {
            nDiff++;
        }
    }
    return nDiff;
}

// FUNCTION: SWKOTOR 0x005706F0
void CSWSMessage::AssignCreatureLists(CExoArrayList<unsigned long>* pSource,
                                      CExoArrayList<unsigned long>* pTarget)
{
    pTarget->SetSize(0);
    for (int i = 0; i < pSource->num; i++) {
        pTarget->Add(pSource->element[i]);
    }
}

// FUNCTION: SWKOTOR 0x00570BA0
// NONMATCH: 13 diff insns, the copy loop counts down in ecx in the original
// (ebx here).
void CSWSMessage::ComputeLastUpdate_StoreUpdateSpellLikeAbility(CSWSCreature* pCreature,
                                                               CSWSPlayerLastUpdateObject* pLast)
{
    CSWSCreatureStats* pStats = pCreature->m_pStats;
    CExoArrayList<CSWSStats_SpellLikeAbility>* pLastList = pLast->m_pSpellLikeAbilities;
    CExoArrayList<CSWSStats_SpellLikeAbility>* pList = pStats->m_pSpellLikeAbilities;
    int nCount = pList->num;
    if (!pLastList) {
        pLast->m_pSpellLikeAbilities = new CExoArrayList<CSWSStats_SpellLikeAbility>;
        pLastList = pLast->m_pSpellLikeAbilities;
    }
    if (nCount) {
        pLastList->Allocate(nCount);
        pLastList->num = nCount;
        for (int i = 0; i < nCount; i++) {
            pLastList->element[i].m_bReadied = pList->element[i].m_bReadied;
            pLastList->element[i].m_nCasterLevel = pList->element[i].m_nCasterLevel;
            pLastList->element[i].m_nSpellId = pList->element[i].m_nSpellId;
        }
    }
}

// FUNCTION: SWKOTOR 0x0056C4D0
// NONMATCH: 27 diff insns, base/index operand order and register choice in the
// element compares (frame and layout match).
void CSWSMessage::ComputeLastUpdate_WriteSpellLikeAbility(CSWSCreature* pCreature,
                                                          CSWSPlayerLastUpdateObject* pLast)
{
    WriteWORD(ComputeLastUpdate_CompareSpellLikeAbility(pCreature, pLast), 16);
    CExoArrayList<CSWSStats_SpellLikeAbility>* pLastList = pLast->m_pSpellLikeAbilities;
    CExoArrayList<CSWSStats_SpellLikeAbility>* pList = pCreature->m_pStats->m_pSpellLikeAbilities;
    int nCount = pList->num;
    int nLastCount = 0;
    if (pLastList) {
        nLastCount = pLastList->num;
    }
    if (nLastCount != nCount) {
        WriteBYTE(0x53, 8);
        WriteWORD(nCount, 16);
    }
    for (int i = 0; i < nCount; i++) {
        if (i >= nLastCount) {
            WriteBYTE(0x55, 8);
            WriteWORD(i, 16);
            WriteBOOL((*pList)[i].m_bReadied);
            WriteBYTE((*pList)[i].m_nCasterLevel, 8);
            WriteWORD((*pList)[i].m_nSpellId, 16);
        }
        else {
            if ((*pList)[i].m_bReadied != pLastList->element[i].m_bReadied ||
                (*pList)[i].m_nCasterLevel != pLastList->element[i].m_nCasterLevel ||
                (*pList)[i].m_nSpellId != pLastList->element[i].m_nSpellId) {
                WriteBYTE(0x55, 8);
                WriteWORD(i, 16);
                WriteBOOL((*pList)[i].m_bReadied);
                WriteBYTE((*pList)[i].m_nCasterLevel, 8);
                WriteWORD((*pList)[i].m_nSpellId, 16);
            }
        }
    }
}
