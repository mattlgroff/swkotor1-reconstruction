// CSWSItem / CItemRepository part A (0x00553000-0x00556000).
#include "b/sws_item.h"
#include "b/server_exo_app.h"
#include "b/sw_rules.h"

// FUNCTION: SWKOTOR 0x00553040
void CSWSObject::SetTag(CExoString& sTag)
{
    m_sTag = sTag.LowerCase();
}

// FUNCTION: SWKOTOR 0x005538A0
void CSWSItem::RemoveFromArea()
{
    if (GetArea()) {
        GetArea()->RemoveObjectFromArea(m_idSelf);
    }
    SetArea(NULL);
    Vector v;
    v.x = 0.0f;
    v.y = 0.0f;
    v.z = 0.0f;
    SetPosition(v, 1, 1, 0);
}

// FUNCTION: SWKOTOR 0x00553BB0
void CSWSItem::ApplyItemProperties(CSWSCreature* pCreature, unsigned long nSlot, int bArg)
{
    CServerAIMaster* pAI = g_pAppManager->GetServerExoApp()->GetServerAIMaster();
    if (!pAI) {
        return;
    }
    for (int i = 0; i < m_nPassiveProperties; i++) {
        CSWItemProperty* pProperty = GetPassiveProperty(i);
        if (pProperty->m_nUpgradeSlot == 0xff ||
            (m_nEquippedSlots & (1 << pProperty->m_nUpgradeSlot))) {
            pAI->OnItemPropertyApplied(this, pProperty, pCreature, nSlot, bArg);
        }
    }
}

// FUNCTION: SWKOTOR 0x00553C30
void CSWSItem::RemoveItemProperties(CSWSCreature* pCreature, unsigned long nSlot)
{
    CServerAIMaster* pAI = g_pAppManager->GetServerExoApp()->GetServerAIMaster();
    if (pAI) {
        for (int i = 0; i < m_nPassiveProperties; i++) {
            CSWItemProperty* pProperty = GetPassiveProperty(i);
            if (pProperty->m_nUpgradeSlot == 0xff ||
                (m_nEquippedSlots & (1 << pProperty->m_nUpgradeSlot))) {
                pAI->OnItemPropertyRemoved(this, pProperty, pCreature, nSlot);
            }
        }
    }
    pCreature->RemoveEffectByCreator(m_idSelf);
}

// FUNCTION: SWKOTOR 0x00553F90
// NONMATCH: register allocation (sum in ebx, max reloaded) differs.
int CSWSItem::MergeItem(CSWSItem* pOther)
{
    if (CompareItem(pOther)) {
        CSWBaseItem* pBase = g_pRules->m_pBaseItemArray->GetBaseItem(m_nBaseItem);
        int nMine = m_nStackCount;
        int nSum = pOther->m_nStackCount + nMine;
        if (nSum > pBase->m_nStackSize) {
            m_nStackCount = pBase->m_nStackSize;
            pOther->m_nStackCount += nMine - pBase->m_nStackSize;
        }
        else {
            m_nStackCount = nSum;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00554000
unsigned long CSWSItem::GetCost()
{
    if (m_bPlotItem == 1) {
        return 0;
    }
    if ((unsigned long)((double)GetBaseItem()->m_fCostMultiplier * m_nStackSize) > 1) {
        return (unsigned long)((double)GetBaseItem()->m_fCostMultiplier * m_nStackSize);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005556B0
CResRef CSWSItem::GetIcon()
{
    CSWBaseItem* pBase = GetBaseItem();
    if (!pBase) {
        return CResRef("");
    }
    if (pBase->m_bIconVariant) {
        return pBase->GetModelResRef(m_nIconVariant, 0x6d);
    }
    return pBase->GetModelResRef(m_nIcon, 0x6d);
}

// FUNCTION: SWKOTOR 0x00555720
void CSWSItem::AddToArea(CSWSArea* pArea, float fX, float fY, float fZ, int bArg)
{
    Vector v;
    v.x = fX;
    v.y = fY;
    v.z = fZ;
    RemoveFromArea();
    if (!pArea) {
        return;
    }
    SetArea(pArea);
    SetPosition(v, 0, 1, 0);
    GetArea()->AddObjectToArea(m_idSelf, bArg);
}

// FUNCTION: SWKOTOR 0x00555ED0
unsigned long CItemRepository::FindItemWithTag(const CExoString& sTag)
{
    for (int i = 0; i < m_aItems.num; i++) {
        CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(m_aItems.element[i]);
        if (pItem) {
            if (pItem->m_sTag == sTag) {
                return pItem->m_idSelf;
            }
            if (pItem->m_pRepository) {
                unsigned long nFound = pItem->m_pRepository->FindItemWithTag(sTag);
                if (nFound != INVALID_OBJECT_ID) {
                    return nFound;
                }
            }
        }
    }
    return INVALID_OBJECT_ID;
}

// FUNCTION: SWKOTOR 0x00555F40
int CItemRepository::GetItemInRepository(CSWSItem* pItem, int bRecurse)
{
    for (int i = 0; i < m_aItems.num; i++) {
        CSWSItem* pEntry = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(m_aItems.element[i]);
        if (pItem == pEntry) {
            return 1;
        }
        if (bRecurse && pEntry) {
            int nBaseItem = pEntry->m_nBaseItem;
            CSWBaseItem* pBase = g_pRules->m_pBaseItemArray->GetBaseItem(nBaseItem);
            if (pBase && pBase->m_bContainer && pEntry->m_pRepository &&
                pEntry->m_pRepository->GetItemInRepository(pItem, bRecurse)) {
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00555E00
// NONMATCH: original keeps nType in ebx and the loop index on the stack.
unsigned long CItemRepository::FindItemWithBaseItemType(unsigned char nType, int nNth)
{
    int nCount = 0;
    for (int i = 0; i < m_aItems.num; i++) {
        unsigned long nId = m_aItems.element[i];
        CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(nId);
        if (pItem) {
            if (pItem->m_pRepository) {
                unsigned long nFound = pItem->m_pRepository->FindItemWithBaseItemType(nType, nNth);
                if (nFound != INVALID_OBJECT_ID) {
                    return nFound;
                }
            }
            if (nType == pItem->GetBaseItem()->m_nBaseType && nCount == nNth) {
                return nId;
            }
            if (nType == pItem->GetBaseItem()->m_nBaseType && nCount != nNth) {
                nCount++;
            }
        }
    }
    return INVALID_OBJECT_ID;
}

// FUNCTION: SWKOTOR 0x00554080
// NONMATCH: prologue keeps this in ecx longer; arg temporaries use ecx/edx.
unsigned char CSWSItem::GetMinEquipLevel()
{
    int nLimit;
    C2DA* p2DA = g_pRules->Get2DArrays()->GetItemValue();
    int nRow;
    for (nRow = 0; nRow < p2DA->m_nNumRows; nRow++) {
        p2DA->GetINTEntry(nRow, "MAXSINGLEITEMVALUE", &nLimit);
        if (GetCost() <= (unsigned long)nLimit) {
            break;
        }
    }
    unsigned char nLevel = (unsigned char)nRow;
    nLevel++;
    return nLevel;
}

// FUNCTION: SWKOTOR 0x005541C0
unsigned char CSWSItem::GetUpgradableType(C2DA* p2DA)
{
    unsigned char nResult = 0;
    int nType;
    int i;
    for (i = 0; i < m_nActiveProperties; i++) {
        if (GetActiveProperty(i)->m_nUpgradeSlot != 0xff) {
            {
                CExoString sColumn("UpgradeType");
                unsigned char nSlot = GetActiveProperty(i)->m_nUpgradeSlot;
                p2DA->GetINTEntry(nSlot, sColumn, &nType);
            }
            switch (nType) {
            case 0: nResult = 1; break;
            case 4: case 5: case 6: case 7: nResult = 2; break;
            case 1: case 2: case 3: nResult = 3; break;
            case 8: case 9: nResult = 4; break;
            }
        }
    }
    for (i = 0; i < m_nPassiveProperties; i++) {
        if (GetPassiveProperty(i)->m_nUpgradeSlot != 0xff) {
            {
                CExoString sColumn("UpgradeType");
                unsigned char nSlot = GetPassiveProperty(i)->m_nUpgradeSlot;
                p2DA->GetINTEntry(nSlot, sColumn, &nType);
            }
            switch (nType) {
            case 0: nResult = 1; break;
            case 4: case 5: case 6: case 7: nResult = 2; break;
            case 1: case 2: case 3: nResult = 3; break;
            case 8: case 9: nResult = 4; break;
            }
        }
    }
    return nResult;
}

// The inline destructor's out-of-line copy is emitted for the unwind
// action of a local repository.
// FUNCTION: SWKOTOR 0x00555DE0 SYMBOL: ??1CItemRepository@@QAE@XZ
void ForceItemRepositoryDtor()
{
    CItemRepository cRepository(0, 0);
    cRepository.ClearItems();
}

// FUNCTION: SWKOTOR 0x00553CC0
int CSWSItem::GetDefaultModelPart()
{
    if (GetBaseItem()->m_bIconVariant != 1) {
        return 0;
    }
    return GetBaseItem()->m_nDefaultModelPart;
}

// FUNCTION: SWKOTOR 0x00554050
unsigned long CSWSItem::GetDamageFlags()
{
    int nBaseItem = m_nBaseItem;
    CSWBaseItem* pBase = g_pRules->m_pBaseItemArray->GetBaseItem(nBaseItem);
    if (pBase && pBase->m_nDamageFlags) {
        return pBase->m_nDamageFlags;
    }
    return 8;
}

// FUNCTION: SWKOTOR 0x00554190
void* CSWSItem::GetClientItem()
{
    if (GetClientObject()) {
        return GetClientObject()->AsClientItem();
    }
    return NULL;
}

// FUNCTION: SWKOTOR 0x00555380
void CSWSItem::GetPropertyDamageRange(unsigned short nCost, int* pMin, int* pMax)
{
    int nNumDice = 0;
    int nDie = 0;
    unsigned char bFound = g_pRules->Get2DArrays()->GetIPRPDamageCost()->GetINTEntry(nCost, "NumDice", &nNumDice) == 1;
    if (bFound) {
        if (nNumDice == 0) {
            *pMin = 1;
            *pMax = nCost;
            return;
        }
        g_pRules->Get2DArrays()->GetIPRPDamageCost()->GetINTEntry(nCost, "Die", &nDie);
        *pMin = nNumDice;
        *pMax = nDie * nNumDice;
        return;
    }
    *pMin = nCost;
    *pMax = nCost;
}

// FUNCTION: SWKOTOR 0x005554D0
void CSWSItem::GetPropertyChanceDurationDC(unsigned short nCost, unsigned char nParam,
                                           int* pChance, int* pDuration, int* pDC)
{
    int nValue;
    g_pRules->Get2DArrays()->GetIPRPOnHit()->GetINTEntry(nParam, "EffectChance", &nValue);
    *pChance = nValue;
    g_pRules->Get2DArrays()->GetIPRPOnHit()->GetINTEntry(nParam, "DurationRounds", &nValue);
    *pDuration = nValue;
    g_pRules->Get2DArrays()->GetIPRPCostTable(0x19)->GetINTEntry(nCost, "Value", &nValue);
    *pDC = nValue;
}

// FUNCTION: SWKOTOR 0x00555A60
// NONMATCH: case 6 moves the charges through edx (mov ax, dx) instead of ecx/eax.
unsigned short CSWSItem::GetUsedActivePropertyUsesLeft(unsigned char nIndex)
{
    unsigned short nResult = 0;
    CSWItemProperty* pProperty = GetActiveProperty(nIndex);
    if (pProperty && pProperty->m_nPropertyName == 10) {
        switch (pProperty->m_nCostValue) {
        case 1:
            if (pProperty->m_bUseable) {
                nResult = (unsigned char)m_nStackCount;
            }
            break;
        case 2:
            if (pProperty->m_bUseable && m_nCharges > 0) {
                nResult = m_nCharges / 5;
            }
            break;
        case 3:
            if (pProperty->m_bUseable && m_nCharges > 0) {
                nResult = m_nCharges / 4;
            }
            break;
        case 4:
            if (pProperty->m_bUseable && m_nCharges > 0) {
                nResult = m_nCharges / 3;
            }
            break;
        case 5:
            if (pProperty->m_bUseable && m_nCharges > 0) {
                nResult = m_nCharges / 2;
            }
            break;
        case 6:
            if (pProperty->m_bUseable && m_nCharges > 0) {
                nResult = m_nCharges;
            }
            break;
        case 8: case 9: case 10: case 11: case 12:
            if (pProperty->m_bUseable && pProperty->m_nUsesLeft > 0) {
                nResult = pProperty->m_nUsesLeft;
            }
            break;
        case 14: case 15: case 16: case 17: case 18:
            if (pProperty->m_bUseable) {
                nResult = 1;
            }
            break;
        }
    }
    return nResult;
}
