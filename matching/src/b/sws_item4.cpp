// CSWSItem part D: construction / destruction, CompareItem, property
// strings and saving (0x00553020-0x0055FCB0).
#include "b/sws_item.h"
#include "b/server_exo_app.h"
#include "b/sw_rules.h"
#include "b/sws_player.h"

extern const unsigned char OBJECT_TYPE_CREATURE;            // 0x00746634
extern const unsigned char OBJECT_TYPE_ITEM;                // 0x00746635
extern const unsigned long OBJECT_INVALID;                  // 0x00746644
extern const unsigned char GAME_OBJECT_ARRAY_SUCCESS;       // 0x0074666C

// FUNCTION: SWKOTOR 0x005530A0
CSWSItem::CSWSItem(unsigned long nId)
    : CSWSObject(OBJECT_TYPE_ITEM, nId, 0)
{
    m_pRepository = NULL;
    m_bInventoryOpen = 0;
    g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddObject(this, 0);
    m_nD4 = 10001;
    m_oidPossessor = OBJECT_INVALID;
    m_nActiveProperties = 0;
    m_nPassiveProperties = 0;
    m_pActiveProperties = NULL;
    m_pPassiveProperties = NULL;
    m_nStackSize = 0;
    m_nUseableProperties = 0;
    m_nCharges = 50;
    for (int i = 0; i < 8; i++) {
        m_aUsesLeft[i] = 0;
    }
    m_bItemFlag0 = 0;
    m_n244 = 0;
    m_nStackCount = 1;
    m_nIcon = 1;
    m_bItemFlag2 = 0;
    m_bDroppable = 1;
    m_bPickpocketable = 1;
    m_bStolen = 0;
    m_bNonEquippable = 0;
    m_bNewItem = 0;
    m_bDeleting = 0;
    m_nEquippedSlots = 0;
    m_n265 = 0;
    m_nIconVariant = 0;
}

// FUNCTION: SWKOTOR 0x0055EC70
// NONMATCH: ecx/edx swapped in the module lookup-table call and eax/edx for
// the final AI-master load; everything else matches.
CSWSItem::~CSWSItem()
{
    unsigned long nId = GetId();
    unsigned long nClientId = g_pAppManager->GetClientExoApp()->ServerToClientObjectId(nId);
    CSWCItem* pClientItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(nClientId);
    if (pClientItem) {
        CGameObject* pObject;
        if (g_pAppManager->GetClientExoApp()->GetObjectArray()->Delete(nClientId, &pObject) ==
                GAME_OBJECT_ARRAY_SUCCESS &&
            pObject) {
            delete pClientItem;
        }
    }
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    if (pModule) {
        pModule->RemoveObjectFromLookupTable(m_sTag, GetId());
    }
    if (m_pRepository) {
        delete m_pRepository;
        m_pRepository = NULL;
    }
    if (m_pActiveProperties) {
        delete[] m_pActiveProperties;
        m_pActiveProperties = NULL;
    }
    if (m_pPassiveProperties) {
        delete[] m_pPassiveProperties;
        m_pPassiveProperties = NULL;
    }
    if (GetArea()) {
        RemoveFromArea();
    }
    g_pAppManager->GetServerExoApp()->GetServerAIMaster()->RemoveObject(this);
}

// FUNCTION: SWKOTOR 0x0055FCB0 SYMBOL: ??_GCSWSItem@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00553020 SYMBOL: ??_GCSWRules@@UAEPAXI@Z
// (emitted with the CSWRules vtable, which the inline ctor below references)
CSWRules* NewRulesForVtable()
{
    return new CSWRules;
}

// FUNCTION: SWKOTOR 0x00553CF0
int CSWSItem::CompareItem(CSWSItem* pOther)
{
    if (m_nEquippedSlots != pOther->m_nEquippedSlots ||
        m_nActiveProperties != pOther->m_nActiveProperties ||
        m_nPassiveProperties != pOther->m_nPassiveProperties ||
        m_bPlotItem != pOther->m_bPlotItem ||
        m_nCharges != pOther->m_nCharges ||
        m_nBaseItem != pOther->m_nBaseItem ||
        m_bStolen != pOther->m_bStolen ||
        m_nIcon != pOther->m_nIcon ||
        m_n265 != pOther->m_n265 ||
        m_nIconVariant != pOther->m_nIconVariant ||
        m_sTag != pOther->m_sTag ||
        m_lsLocalizedName != pOther->m_lsLocalizedName) {
        return 0;
    }
    int i;
    for (i = 0; i < m_nActiveProperties; i++) {
        CSWItemProperty* pMine = GetActiveProperty(i);
        CSWItemProperty* pTheirs = pOther->GetActiveProperty(i);
        if (pMine->m_nPropertyName != pTheirs->m_nPropertyName ||
            pMine->m_nSubType != pTheirs->m_nSubType ||
            pMine->m_nCostTable != pTheirs->m_nCostTable ||
            pMine->m_nCostValue != pTheirs->m_nCostValue ||
            pMine->m_nParam1 != pTheirs->m_nParam1 ||
            pMine->m_nParam1Value != pTheirs->m_nParam1Value ||
            pMine->m_nUsesLeft != pTheirs->m_nUsesLeft ||
            pMine->m_nUpgradeSlot != pTheirs->m_nUpgradeSlot) {
            return 0;
        }
    }
    for (i = 0; i < m_nPassiveProperties; i++) {
        CSWItemProperty* pMine = GetPassiveProperty(i);
        CSWItemProperty* pTheirs = pOther->GetPassiveProperty(i);
        if (pMine->m_nPropertyName != pTheirs->m_nPropertyName ||
            pMine->m_nSubType != pTheirs->m_nSubType ||
            pMine->m_nCostTable != pTheirs->m_nCostTable ||
            pMine->m_nCostValue != pTheirs->m_nCostValue ||
            pMine->m_nParam1 != pTheirs->m_nParam1 ||
            pMine->m_nParam1Value != pTheirs->m_nParam1Value ||
            pMine->m_nUsesLeft != pTheirs->m_nUsesLeft ||
            pMine->m_nUpgradeSlot != pTheirs->m_nUpgradeSlot) {
            return 0;
        }
    }
    for (i = 0; i < m_nPassiveProperties; i++) {
        CSWItemProperty* pMine = GetPassiveProperty(i);
        CSWItemProperty* pTheirs = pOther->GetPassiveProperty(i);
        if (pMine->m_nPropertyName != pTheirs->m_nPropertyName ||
            pMine->m_nSubType != pTheirs->m_nSubType ||
            pMine->m_nCostTable != pTheirs->m_nCostTable ||
            pMine->m_nCostValue != pTheirs->m_nCostValue ||
            pMine->m_nParam1 != pTheirs->m_nParam1 ||
            pMine->m_nParam1Value != pTheirs->m_nParam1Value ||
            pMine->m_nUsesLeft != pTheirs->m_nUsesLeft ||
            pMine->m_nUpgradeSlot != pTheirs->m_nUpgradeSlot) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005543B0
void CSWSItem::AddRangeProperties(CExoString& sOut)
{
    CExoString sLine;
    int bRanged = GetBaseItem()->IsRangedWeapon();
    if (bRanged > 0) {
        int nRange = (int)GetBaseItem()->m_fMaxRange;
        sLine.Format("%s: %dm\n", g_pAppManager->GetClientExoApp()->GetGUIString(0x96a5).CStr(),
                     nRange);
        sOut = sOut + sLine + "\n";
    }
}

// FUNCTION: SWKOTOR 0x005544F0
void CSWSItem::AddWeaponSizeProperties(CExoString& sOut)
{
    unsigned char nWeaponSize = GetBaseItem()->m_nWeaponSize;
    if (nWeaponSize == 2) {
        CExoString sNewLines("\n\n");
        sOut = sOut + g_pAppManager->GetClientExoApp()->GetGUIString(0xa4a1) + sNewLines;
    }
}

// FUNCTION: SWKOTOR 0x00558950
// NONMATCH: the original loads the property name into ecx and passes that
// register to HasPassivePropertyAbove (we propagate the 0x31 constant), and it
// keeps sOut in ebp with no hoisted zero register.
void CSWSItem::AddCriticalThreatProperties(CExoString& sOut)
{
    CExoString sLine;
    sOut = sOut + g_pAppManager->GetClientExoApp()->GetGUIString(0x96ab) + ":\n";
    int nThreat = GetBaseItem()->m_nCritThreat;
    int nMultiplier = GetBaseItem()->m_nCritMultiplier;
    if (HasProperty(0x1c, 0)) {
        nThreat = nThreat * 2;
    }
    int nMin = 0;
    int nMax = 0;
    if (HasProperty(0x31, 0)) {
        for (int i = 0; i < m_nPassiveProperties; i++) {
            CSWItemProperty* pProperty = GetPassiveProperty(i);
            if (!pProperty) {
                continue;
            }
            if (pProperty->m_nUpgradeSlot != 0xff &&
                !(m_nEquippedSlots & (1 << pProperty->m_nUpgradeSlot))) {
                continue;
            }
            unsigned short nName = pProperty->m_nPropertyName;
            if (nName != 0x31) {
                continue;
            }
            unsigned short nCost = pProperty->m_nCostValue;
            if (!HasPassivePropertyAbove(nName, pProperty->m_nSubType, nCost)) {
                int nDamageMin;
                int nDamageMax;
                GetPropertyDamageRange(nCost, &nDamageMin, &nDamageMax);
                nMin += nDamageMin;
                nMax += nDamageMax;
            }
        }
    }
    if (nMin == 0 && nMax == 0) {
        sLine.Format("%d-20,x%d\n", 21 - nThreat, nMultiplier);
    }
    else {
        sLine.Format("%d-20,x%d +%d-%d\n", 21 - nThreat, nMultiplier, nMin, nMax);
    }
    sOut = sOut + sLine + "\n";
}

// FUNCTION: SWKOTOR 0x0055E930
// NONMATCH: register allocation: the original keeps nModifier in ebp (reused
// as the 0 argument) and the loop counter in the arg slot; ours hoists a zero
// register and spills nModifier.
void CSWSItem::AddAttackModifierProperties(CExoString& sOut)
{
    CExoString sLine;
    int nModifier = 0;
    int i;
    if (HasProperty(0x26, 0) || HasProperty(8, 0) || HasProperty(0x29, 0) ||
        HasProperty(5, 0)) {
        for (i = 0; i < m_nPassiveProperties; i++) {
            CSWItemProperty* pProperty = GetPassiveProperty(i);
            if (pProperty &&
                (pProperty->m_nUpgradeSlot == 0xff ||
                 (m_nEquippedSlots & (1 << pProperty->m_nUpgradeSlot)))) {
                if (pProperty->m_nPropertyName == 5 || pProperty->m_nPropertyName == 0x26) {
                    nModifier += pProperty->m_nCostValue;
                }
                if (pProperty->m_nPropertyName == 8 || pProperty->m_nPropertyName == 0x29) {
                    nModifier -= pProperty->m_nCostValue;
                }
            }
        }
        if (nModifier < 0) {
            sLine.Format("%s: -%d\n",
                         g_pAppManager->GetClientExoApp()->GetGUIString(0x96a6).CStr(),
                         nModifier);
            sOut = sOut + sLine;
        }
        else if (nModifier > 0) {
            sLine.Format("%s: +%d\n",
                         g_pAppManager->GetClientExoApp()->GetGUIString(0x96a6).CStr(),
                         nModifier);
            sOut = sOut + sLine;
        }
    }
    if (HasProperty(6, 0) || HasProperty(7, 0) || HasProperty(0x27, 0) ||
        HasProperty(0x28, 0)) {
        if (nModifier == 0) {
            sOut = sOut + g_pAppManager->GetClientExoApp()->GetGUIString(0x96a6) + ": ";
        }
        AddAttackBonusStrings(6, sOut);
        AddAttackBonusStrings(7, sOut);
        AddAttackBonusStrings(0x27, sOut);
        AddAttackBonusStrings(0x28, sOut);
    }
    sOut = sOut + "\n";
}

// FUNCTION: SWKOTOR 0x005562D0
void CSWSItem::GetSortedPropertyStrings(CExoArrayList<CExoString>& aStrings, CExoString& sOut)
{
    if (aStrings.num) {
        sOut = g_pAppManager->GetClientExoApp()->GetGUIString(0x609) + ":\n";
        while (aStrings.num) {
            int nFirst = 0;
            for (int i = 1; i < aStrings.num; i++) {
                if (aStrings.element[i] < aStrings.element[nFirst]) {
                    nFirst = i;
                }
            }
            sOut = sOut + aStrings.element[nFirst] + "\n";
            aStrings.DelIndex(nFirst);
        }
    }
}

// FUNCTION: SWKOTOR 0x0055F0F0
void CSWSItem::ReadContainerItemsFromGff(CResGFF* pGFF, CResStruct* pStruct)
{
    if (m_pRepository) {
        CResList cList;
        if (pGFF->GetList(&cList, pStruct, "ItemList") == 1) {
            for (unsigned long i = 0; i < pGFF->GetListCount(&cList); i++) {
                CResStruct cItem;
                int bSuccess;
                pGFF->GetListElement(&cItem, &cList, i);
                unsigned long oidItem =
                    pGFF->ReadFieldDWORD(&cItem, "ObjectId", bSuccess, INVALID_OBJECT_ID);
                CSWSItem* pItem = new CSWSItem(oidItem);
                CSWSItem* pAddItem = pItem;
                if (pItem) {
                    if (!pItem->LoadItem(pGFF, &cItem)) {
                        delete pItem;
                    }
                    else {
                        int bRead;
                        pGFF->ReadFieldBYTE(&cItem, "Repos_PosX", bRead, 0xff);
                        pGFF->ReadFieldBYTE(&cItem, "Repos_Posy", bRead, 0xff);
                        if (m_pRepository->AddItem(&pAddItem, 1, 0, 1) && pAddItem) {
                            pAddItem->SetPossessor(m_idSelf, 1, 0, INVALID_OBJECT_ID);
                        }
                    }
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00555790
int CSWSItem::SaveItemProperties(CResGFF* pGFF, CResStruct* pStruct)
{
    CResList cList;
    CResStruct cProperty;
    pGFF->AddList(&cList, pStruct, "PropertiesList");
    int i;
    for (i = 0; i < m_nActiveProperties; i++) {
        pGFF->AddListElement(&cProperty, &cList, i);
        CSWItemProperty* pProperty = GetActiveProperty(i);
        pGFF->WriteFieldWORD(&cProperty, pProperty->m_nPropertyName, "PropertyName");
        pGFF->WriteFieldWORD(&cProperty, pProperty->m_nSubType, "Subtype");
        pGFF->WriteFieldBYTE(&cProperty, pProperty->m_nCostTable, "CostTable");
        pGFF->WriteFieldWORD(&cProperty, pProperty->m_nCostValue, "CostValue");
        pGFF->WriteFieldBYTE(&cProperty, pProperty->m_nParam1, "Param1");
        pGFF->WriteFieldBYTE(&cProperty, pProperty->m_nParam1Value, "Param1Value");
        pGFF->WriteFieldBYTE(&cProperty, pProperty->m_nChanceAppear, "ChanceAppear");
        pGFF->WriteFieldBYTE(&cProperty, pProperty->m_nUsesLeft, "UsesPerDay");
        pGFF->WriteFieldBYTE(&cProperty, pProperty->m_bUseable, "Useable");
        pGFF->WriteFieldBYTE(&cProperty, pProperty->m_nUpgradeSlot, "UpgradeType");
    }
    for (i = 0; i < m_nPassiveProperties; i++) {
        pGFF->AddListElement(&cProperty, &cList, m_nActiveProperties + i);
        CSWItemProperty* pProperty = GetPassiveProperty(i);
        pGFF->WriteFieldWORD(&cProperty, pProperty->m_nPropertyName, "PropertyName");
        pGFF->WriteFieldWORD(&cProperty, pProperty->m_nSubType, "Subtype");
        pGFF->WriteFieldBYTE(&cProperty, pProperty->m_nCostTable, "CostTable");
        pGFF->WriteFieldWORD(&cProperty, pProperty->m_nCostValue, "CostValue");
        pGFF->WriteFieldBYTE(&cProperty, pProperty->m_nParam1, "Param1");
        pGFF->WriteFieldBYTE(&cProperty, pProperty->m_nParam1Value, "Param1Value");
        pGFF->WriteFieldBYTE(&cProperty, pProperty->m_nChanceAppear, "ChanceAppear");
        pGFF->WriteFieldBYTE(&cProperty, pProperty->m_nUsesLeft, "UsesPerDay");
        pGFF->WriteFieldBYTE(&cProperty, pProperty->m_bUseable, "Useable");
        pGFF->WriteFieldBYTE(&cProperty, pProperty->m_nUpgradeSlot, "UpgradeType");
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0055CCD0
int CSWSItem::SaveItem(CResGFF* pGFF, CResStruct* pStruct)
{
    pGFF->WriteFieldINT(pStruct, m_nBaseItem, "BaseItem");
    pGFF->WriteFieldCExoString(pStruct, m_sTag, "Tag");
    pGFF->WriteFieldBYTE(pStruct, 1, "Identified");
    pGFF->WriteFieldCExoLocString(pStruct, m_lsDescription, "Description");
    pGFF->WriteFieldCExoLocString(pStruct, m_lsDescIdentified, "DescIdentified");
    pGFF->WriteFieldCExoLocString(pStruct, m_lsLocalizedName, "LocalizedName");
    pGFF->WriteFieldWORD(pStruct, (unsigned short)m_nStackCount, "StackSize");
    pGFF->WriteFieldBYTE(pStruct, m_bStolen, "Stolen");
    pGFF->WriteFieldDWORD(pStruct, m_nEquippedSlots, "Upgrades");
    pGFF->WriteFieldBYTE(pStruct, m_bDroppable, "Dropable");
    pGFF->WriteFieldBYTE(pStruct, m_bPickpocketable, "Pickpocketable");
    pGFF->WriteFieldBYTE(pStruct, m_nIcon, "ModelVariation");
    switch (GetBaseItemFromRules()->m_bIconVariant) {
    case 1:
        pGFF->WriteFieldBYTE(pStruct, m_n265, "BodyVariation");
        pGFF->WriteFieldBYTE(pStruct, m_nIconVariant, "TextureVar");
        break;
    }
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)m_nCharges, "Charges");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)m_nMaxCharges, "MaxCharges");
    pGFF->WriteFieldDWORD(pStruct, GetCost(), "Cost");
    pGFF->WriteFieldDWORD(pStruct, m_nStackSize, "AddCost");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)m_bPlotItem, "Plot");
    if (GetBaseItemFromRules()->m_bContainer) {
        SaveContainerItems(pGFF, pStruct);
    }
    SaveItemProperties(pGFF, pStruct);
    pGFF->WriteFieldFLOAT(pStruct, m_vPosition.x, "XPosition");
    pGFF->WriteFieldFLOAT(pStruct, m_vPosition.y, "YPosition");
    pGFF->WriteFieldFLOAT(pStruct, m_vPosition.z, "ZPosition");
    pGFF->WriteFieldFLOAT(pStruct, m_vOrientation.x, "XOrientation");
    pGFF->WriteFieldFLOAT(pStruct, m_vOrientation.y, "YOrientation");
    pGFF->WriteFieldFLOAT(pStruct, m_vOrientation.z, "ZOrientation");
    pGFF->WriteFieldBYTE(pStruct, m_bNonEquippable, "NonEquippable");
    pGFF->WriteFieldBYTE(pStruct, m_bNewItem, "NewItem");
    pGFF->WriteFieldBYTE(pStruct, m_bDeleting, "DELETING");
    return 1;
}

// FUNCTION: SWKOTOR 0x0055DE20
// NONMATCH: the original keeps a `bCharges = 1` store per charge case (the
// compiler here sinks it into the shared block), so registers rotate.
void CSWSItem::DecrementUses(int nIndex, unsigned long oidPlayer)
{
    if (g_pAppManager->GetClientExoApp()->GetOptions()->m_bInfiniteUses) {
        return;
    }
    CSWItemProperty* pProperty = GetActiveProperty(nIndex);
    int bCharges = 0;
    int bDestroy = 0;
    int bRecharges = 0;
    int nCharges;
    switch (pProperty->m_nCostValue) {
    case 1:
        if (m_nStackCount > 1) {
            m_nStackCount--;
        }
        else {
            pProperty->m_nUseable = 0;
            bDestroy = 1;
        }
        break;
    case 2:
        bCharges = 1;
        nCharges = 5;
        goto charges;
    case 3:
        bCharges = 1;
        nCharges = 4;
        goto charges;
    case 4:
        bCharges = 1;
        nCharges = 3;
        goto charges;
    case 5:
        bCharges = 1;
        nCharges = 2;
        goto charges;
    case 6:
        bCharges = 1;
        nCharges = 1;
    charges:
        {
            if (m_nCharges >= nCharges) {
                m_nCharges -= nCharges;
            }
            for (int i = 0; i < m_nActiveProperties; i++) {
                CSWItemProperty* pCharge = GetActiveProperty(i);
                if (pCharge && pCharge->m_nPropertyName == 10) {
                    int nCost = pCharge->m_nCostValue;
                    if (nCost == 2) {
                        if (m_nCharges < 5) {
                            pCharge->m_nUseable = 0;
                        }
                    }
                    else if (nCost == 3) {
                        if (m_nCharges < 4) {
                            pCharge->m_nUseable = 0;
                        }
                    }
                    else if (nCost == 4) {
                        if (m_nCharges < 3) {
                            pCharge->m_nUseable = 0;
                        }
                    }
                    else if (nCost == 5) {
                        if (m_nCharges < 2) {
                            pCharge->m_nUseable = 0;
                        }
                    }
                    else if (nCost == 6) {
                        if (m_nCharges < 1) {
                            pCharge->m_nUseable = 0;
                        }
                    }
                }
            }
        }
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        bRecharges = 1;
        if (pProperty->m_nUsesLeft > 0) {
            pProperty->m_nUsesLeft--;
        }
        if (pProperty->m_nUsesLeft < 1) {
            pProperty->m_nUseable = 0;
        }
        break;
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
        {
            bRecharges = 1;
            unsigned long nDay;
            unsigned long nTime;
            g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
            pProperty->m_nLastUsedDay = nDay;
            pProperty->m_nLastUsedTime = nTime;
            pProperty->m_nUseable = 0;
        }
        break;
    }
    if (!pProperty->m_bUseable) {
        if (GetBaseItem()->m_nBaseType == 0x19 || GetBaseItem()->m_nBaseType == 0x2d ||
            GetBaseItem()->m_nBaseType == 0x2f) {
            bDestroy = 1;
        }
        else if (bCharges) {
            int nCount = m_nActiveProperties;
            int i;
            for (i = 0; i < m_nActiveProperties; i++) {
                CSWItemProperty* pCharge = GetActiveProperty(i);
                if (pCharge && pCharge->m_nPropertyName == 10) {
                    int nCost = pCharge->m_nCostValue;
                    if ((nCost == 2 || nCost == 3 || nCost == 4 || nCost == 5 || nCost == 6) &&
                        pCharge->m_bUseable) {
                        break;
                    }
                }
            }
            if (i == nCount) {
                bDestroy = 1;
            }
        }
    }
    if (bDestroy && !bRecharges) {
        CGameObject* pPlayer =
            g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(oidPlayer);
        if (pPlayer) {
            g_pAppManager->GetServerExoApp()->GetSWSMessage()
                ->SendServerPlayerItemUpdate_DestroyItem(pPlayer, GetId());
        }
        m_bDeleting = 1;
        g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
            0, 0, oidPlayer, GetId(), EVENT_DESTROY_OBJECT, NULL);
        return;
    }
    UpdateUsedActiveProperties();
}

// FUNCTION: SWKOTOR 0x0055D330
// NONMATCH: the four gold / pazaak-card exits are tail-merged differently
// (the original keeps one delete block per parent kind) and registers rotate.
int CItemRepository::AddItem(CSWSItem** ppItem, int bMergeStacks, int bArg, int bNewItem)
{
    CGameObject* pParent = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidParent);
    if (pParent) {
        if (pParent->AsSWSCreature()) {
            if ((*ppItem)->GetBaseItem()->m_nBaseType == 0x17) {
                pParent->AsSWSCreature()->AddGold((*ppItem)->GetStackSize(), 1);
                delete *ppItem;
                *ppItem = NULL;
                return 1;
            }
            else if ((*ppItem)->GetBaseItem()->m_nBaseType == 0x2a) {
                pParent->AsSWSCreature()->AddPazaakCard(*ppItem);
                delete *ppItem;
                *ppItem = NULL;
                return 1;
            }
        }
        else if (pParent->AsSWSItem()) {
            CSWSItem* pContainer = pParent->AsSWSItem();
            if (g_pRules->m_pBaseItemArray->GetBaseItem(pContainer->m_nBaseItem)->m_bContainer == 1 &&
                pContainer->GetPossessor() != INVALID_OBJECT_ID) {
                CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()
                                              ->GetCreatureByGameObjectID(pContainer->GetPossessor());
                if (pCreature) {
                    if ((*ppItem)->GetBaseItem()->m_nBaseType == 0x17) {
                        pCreature->AddGold((*ppItem)->GetStackSize(), 1);
                        delete *ppItem;
                        *ppItem = NULL;
                        return 1;
                    }
                    else if ((*ppItem)->GetBaseItem()->m_nBaseType == 0x2a) {
                        pCreature->AddPazaakCard(*ppItem);
                        delete *ppItem;
                        *ppItem = NULL;
                        return 1;
                    }
                }
            }
        }
    }
    int nInsertPos = 0;
    int bFoundPos = 0;
    int nLanguage = g_pAppManager->GetClientExoApp()->GetLanguage();
    CExoString sNewName;
    CExoString sName;
    if (m_bReadOnly) {
        (*ppItem)->m_lsLocalizedName.GetString(nLanguage, &sNewName, 0);
    }
    if (bMergeStacks == 1) {
        for (int i = 0; i < m_aItems.num; i++) {
            unsigned long oidItem = m_aItems.element[i];
            CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem);
            if (pItem) {
                if (m_bReadOnly && !bFoundPos) {
                    pItem->m_lsLocalizedName.GetString(nLanguage, &sName, 0);
                    if ((unsigned long)(*ppItem)->m_nBaseItem < (unsigned long)pItem->m_nBaseItem ||
                        ((*ppItem)->m_nBaseItem == pItem->m_nBaseItem && sNewName < sName)) {
                        nInsertPos = i;
                        bFoundPos = 1;
                    }
                }
                if (pItem->m_nBaseItem == (*ppItem)->m_nBaseItem &&
                    pItem->m_nIcon == (*ppItem)->m_nIcon && pItem->MergeItem(*ppItem) == 1) {
                    if (pParent) {
                        unsigned long oidOwner;
                        if (pParent->m_nObjectType == OBJECT_TYPE_CREATURE) {
                            oidOwner = m_oidParent;
                        }
                        else if (pParent->m_nObjectType == OBJECT_TYPE_ITEM) {
                            oidOwner = pParent->AsSWSItem()->GetPossessor();
                        }
                        else {
                            goto merged;
                        }
                        CGameObject* pPlayer =
                            g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(oidOwner);
                        if (pPlayer) {
                            g_pAppManager->GetServerExoApp()->GetSWSMessage()
                                ->SendServerPlayerItemUpdate_DestroyItem(pPlayer,
                                                                         (*ppItem)->m_idSelf);
                        }
                    }
                merged:
                    delete *ppItem;
                    *ppItem = pItem;
                    pItem->SetPossessor(pItem->GetPossessor(), bArg, bArg, INVALID_OBJECT_ID);
                    if (bNewItem) {
                        (*ppItem)->m_bNewItem = 1;
                        m_nNewItems++;
                    }
                    return 1;
                }
            }
        }
    }
    else if (m_bReadOnly) {
        for (int i = 0; i < m_aItems.num; i++) {
            unsigned long oidItem = m_aItems.element[i];
            CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem);
            if (pItem && m_bReadOnly) {
                pItem->m_lsLocalizedName.GetString(nLanguage, &sName, 0);
                if (sNewName < sName) {
                    nInsertPos = i;
                    bFoundPos = 1;
                    break;
                }
            }
        }
    }
    CExoArrayList<CSWSItem*> aAdded;
    aAdded.Add(*ppItem);
    if (m_bReadOnly) {
        CSWSPlayer* pPlayer = g_pAppManager->GetServerExoApp()->GetClientObjectByPlayerID(0, 1);
        if (pPlayer) {
            (*ppItem)->SetPossessor(pPlayer->m_oid38, bArg, bArg, INVALID_OBJECT_ID);
        }
        if (bFoundPos) {
            m_aItems.InsertAt((*ppItem)->m_idSelf, nInsertPos);
        }
        else {
            m_aItems.Add((*ppItem)->m_idSelf);
        }
    }
    else {
        m_aItems.InsertAt((*ppItem)->m_idSelf, 0);
    }
    if (bNewItem) {
        (*ppItem)->m_bNewItem = 1;
    }
    if ((*ppItem)->m_bNewItem) {
        m_nNewItems++;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0055F340
// NONMATCH: the original's unused local list (three zeroed dwords, EH state
// 1) is never freed, so its element type has a destructor but no
// constructor; with <unsigned long> we emit a trailing operator delete(NULL),
// and a dtor-only element type instead moves the zero register to edi.
CExoString CSWSItem::GetPropertyDescription()
{
    CExoArrayList<unsigned long> aUnused;
    CExoString sProperties;
    if (GetBaseItem()->m_nBaseType != 0x2e && GetBaseItem()->m_nBaseType != 6) {
        AddDescription556490(sProperties);
        if (GetBaseItem()->m_bWeapon) {
            AddDescription556DE0(sProperties);
            AddRangeProperties(sProperties);
            AddCriticalThreatProperties(sProperties);
            AddDescription558C10(sProperties);
            AddWeaponSizeProperties(sProperties);
        }
        AddAttackModifierProperties(sProperties);
        AddDescription5599D0(sProperties);
        AddDescription55A510(sProperties);
    }
    CExoString sDescription;
    CExoLocString* pDescription = &m_lsDescIdentified;
    pDescription->GetString(g_pAppManager->GetClientExoApp()->GetLanguage(), &sDescription, 0);
    if (sDescription == "") {
        sDescription = g_pAppManager->GetClientExoApp()->GetGUIString(0x7dac);
    }
    sDescription = sProperties + sDescription;
    return sDescription;
}
