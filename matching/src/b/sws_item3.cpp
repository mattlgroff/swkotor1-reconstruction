// CSWSItem / CItemRepository part C: property scans, repository helpers,
// loading (0x00553900-0x00560B30).
#include "b/sws_item.h"
#include "b/server_exo_app.h"
#include "b/sw_rules.h"
#include "b/sws_player.h"

// FUNCTION: SWKOTOR 0x00553900
int CSWSItem::IsChargePropertyName(unsigned short nName)
{
    switch (nName) {
    case 10:
    case 0x25:
    case 0x2e:
    case 0x35:
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00553960 SYMBOL: ?GetActiveProperty@CSWSItem@@QAEPAUCSWItemProperty@@H@Z
CSWItemProperty* (CSWSItem::*g_pfnGetActiveProperty)(int) = &CSWSItem::GetActiveProperty;
// FUNCTION: SWKOTOR 0x00553990 SYMBOL: ?GetPassiveProperty@CSWSItem@@QAEPAUCSWItemProperty@@H@Z
CSWItemProperty* (CSWSItem::*g_pfnGetPassiveProperty)(int) = &CSWSItem::GetPassiveProperty;

// FUNCTION: SWKOTOR 0x005539C0
int CSWSItem::FindProperty(CSWItemProperty** ppProperty, unsigned short nName,
                           unsigned short nSubType)
{
    int i;
    for (i = 0; i < m_nActiveProperties; i++) {
        unsigned char nSlot = GetActiveProperty(i)->m_nUpgradeSlot;
        if (nSlot == 0xff || (m_nEquippedSlots & (1 << nSlot))) {
            if (GetActiveProperty(i)->m_nPropertyName == nName) {
                if (nSubType != 0) {
                    if (GetActiveProperty(i)->m_nSubType == nSubType) {
                        *ppProperty = GetActiveProperty(i);
                        return 1;
                    }
                }
                else {
                    *ppProperty = GetActiveProperty(i);
                    return 1;
                }
            }
        }
    }
    for (i = 0; i < m_nPassiveProperties; i++) {
        unsigned char nSlot = GetPassiveProperty(i)->m_nUpgradeSlot;
        if (nSlot == 0xff || (m_nEquippedSlots & (1 << nSlot))) {
            if (GetPassiveProperty(i)->m_nPropertyName == nName) {
                if (nSubType != 0) {
                    if (GetPassiveProperty(i)->m_nSubType == nSubType) {
                        *ppProperty = GetPassiveProperty(i);
                        return 1;
                    }
                }
                else {
                    *ppProperty = GetPassiveProperty(i);
                    return 1;
                }
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00554170
void CSWSItem::SetItemFlag0(int bValue)
{
    m_bItemFlag0 = bValue;
}

// FUNCTION: SWKOTOR 0x00555610
int CSWSItem::HasPassivePropertyAbove(unsigned short nName, unsigned short nSubType,
                                      unsigned short nCost)
{
    for (int i = 0; i < m_nPassiveProperties; i++) {
        CSWItemProperty* pProperty = GetPassiveProperty(i);
        if (pProperty &&
            (pProperty->m_nUpgradeSlot == 0xff ||
             (m_nEquippedSlots & (1 << pProperty->m_nUpgradeSlot))) &&
            pProperty->m_nPropertyName == nName && pProperty->m_nSubType == nSubType &&
            pProperty->m_nCostValue > nCost) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00555690
int CSWSItem::IsLightsaber()
{
    int bResult = 0;
    if (m_nBaseItem == 8 || m_nBaseItem == 9 || m_nBaseItem == 10) {
        bResult = 1;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x00555BF0
void CSWSItem::ResetUsesPerDay(int bOnlyUnset)
{
    for (int i = 0; i < m_nActiveProperties; i++) {
        CSWItemProperty* pProperty = GetActiveProperty(i);
        if (pProperty && pProperty->m_nPropertyName == 10 &&
            (!bOnlyUnset || pProperty->m_nUsesLeft == 0xff)) {
            switch (pProperty->m_nCostValue) {
            case 8:
                pProperty->m_nUsesLeft = 1;
                break;
            case 9:
                pProperty->m_nUsesLeft = 2;
                break;
            case 10:
                pProperty->m_nUsesLeft = 3;
                break;
            case 11:
                pProperty->m_nUsesLeft = 4;
                break;
            case 12:
                pProperty->m_nUsesLeft = 5;
                break;
            }
            pProperty->m_nUseable = 1;
        }
    }
}

// FUNCTION: SWKOTOR 0x00555C90
int CSWSItem::HasProperty(unsigned short nName, unsigned short nSubType)
{
    int i;
    for (i = 0; i < m_nActiveProperties; i++) {
        unsigned char nSlot = GetActiveProperty(i)->m_nUpgradeSlot;
        if (nSlot == 0xff || (m_nEquippedSlots & (1 << nSlot))) {
            if (GetActiveProperty(i)->m_nPropertyName == nName) {
                if (nSubType == 0 || GetActiveProperty(i)->m_nSubType == nSubType) {
                    return 1;
                }
            }
        }
    }
    for (i = 0; i < m_nPassiveProperties; i++) {
        unsigned char nSlot = GetPassiveProperty(i)->m_nUpgradeSlot;
        if (nSlot == 0xff || (m_nEquippedSlots & (1 << nSlot))) {
            if (GetPassiveProperty(i)->m_nPropertyName == nName) {
                if (nSubType == 0 || GetPassiveProperty(i)->m_nSubType == nSubType) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00555FD0
int CItemRepository::RemoveItem(CSWSItem* pItem)
{
    if (pItem) {
        for (int i = 0; i < m_aItems.num; i++) {
            if (pItem->m_idSelf == m_aItems.element[i]) {
                m_aItems.DelIndex(i);
                if (pItem->m_bNewItem) {
                    m_nNewItems--;
                    pItem->m_bNewItem = 0;
                }
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00556050
void CItemRepository::ClearNewItems(CExoArrayList<unsigned long>* pNew)
{
    for (int i = pNew->num - 1; i >= 0; i--) {
        CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(pNew->element[i]);
        if (pItem) {
            pItem->m_bNewItem = 0;
            m_nNewItems--;
            pNew->DelIndex(i);
        }
    }
}

// FUNCTION: SWKOTOR 0x005560B0 SYMBOL: ?GetItemAt@CItemRepository@@QAEPAVCSWSItem@@H@Z
CSWSItem* (CItemRepository::*g_pfnGetItemAt)(int) = &CItemRepository::GetItemAt;

// FUNCTION: SWKOTOR 0x005560E0
unsigned long CItemRepository::GetItemId(int n)
{
    if (n < 0 || n >= m_aItems.num) {
        return INVALID_OBJECT_ID;
    }
    return m_aItems.element[n];
}

// FUNCTION: SWKOTOR 0x00556100
int CItemRepository::CalculateContentsWeight()
{
    int nWeight = 0;
    for (int i = 0; i < m_aItems.num; i++) {
        CSWSItem* pItem = GetItemAt(i);
        if (pItem) {
            nWeight += pItem->GetWeight();
        }
    }
    return nWeight;
}

// FUNCTION: SWKOTOR 0x00556150
void CItemRepository::SetParentId(unsigned long oidParent)
{
    m_oidParent = oidParent;
    for (int i = 0; i < m_aItems.num; i++) {
        CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(m_aItems.element[i]);
        if (pItem) {
            pItem->SetPossessor(m_oidParent, 0, 0, INVALID_OBJECT_ID);
        }
    }
}

// FUNCTION: SWKOTOR 0x005562A0
int CSWSItem::GetWeight()
{
    if (m_pRepository) {
        return m_pRepository->CalculateContentsWeight() + m_nWeight;
    }
    if (m_nStackCount > 1) {
        return m_nWeight * m_nStackCount;
    }
    return m_nWeight;
}

// FUNCTION: SWKOTOR 0x0055D290
CItemRepository::CItemRepository(unsigned long oidParent, int bReadOnly)
{
    m_oidParent = oidParent;
    m_nNewItems = 0;
    m_bReadOnly = bReadOnly;
}

// FUNCTION: SWKOTOR 0x0055D800
// NONMATCH: eax/edx swapped in the loop and event arguments; the `new`
// temporary lands in the nObject arg slot instead of bReturnItems'.
void CSWSItem::CloseInventory(unsigned long nObject, int bReturnItems)
{
    if (m_bInventoryOpen) {
        if (bReturnItems) {
            CSWSPlayer* pPlayer =
                (CSWSPlayer*)g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(nObject);
            if (pPlayer) {
                CSWSCreature* pCreature = pPlayer->GetGameObject()->AsSWSCreature();
                while ((unsigned long)m_pRepository->m_aItems.num > 0) {
                    CItemRepository* pRepository = m_pRepository;
                    CSWSItem* pItem = pRepository->GetItemAt(pRepository->m_aItems.num - 1);
                    m_pRepository->RemoveItem(pItem);
                    pCreature->GetItemRepository(1)->AddItem(&pItem, 1, 0, 1);
                }
            }
        }
        CScriptEvent* pEvent = new CScriptEvent();
        pEvent->m_nType = 0x17;
        g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
            0, 0, nObject, GetId(), EVENT_DELAY_INVENTORY, pEvent);
        m_bInventoryOpen = 0;
    }
}

// FUNCTION: SWKOTOR 0x0055F050
void CSWSItem::InitRepository(unsigned long oidParent)
{
    if (m_pRepository) {
        delete m_pRepository;
        m_pRepository = NULL;
    }
    if (GetBaseItemFromRules()->m_bContainer) {
        m_pRepository = new CItemRepository(oidParent, 0);
    }
}

// FUNCTION: SWKOTOR 0x0055F280
CSWSItem* CSWSItem::SplitItem(int nCount)
{
    if (nCount > 0 && nCount < m_nStackCount) {
        CSWSItem* pNew = new CSWSItem(INVALID_OBJECT_ID);
        pNew->CopyItem(this);
        if (pNew) {
            m_nStackCount -= nCount;
            pNew->m_nStackCount = nCount;
        }
        return pNew;
    }
    return NULL;
}

// FUNCTION: SWKOTOR 0x0055F330
int CItemRepository::ClearItems()
{
    m_aItems.num = 0;
    m_nNewItems = 0;
    return 0;
}

// FUNCTION: SWKOTOR 0x005608B0
int CSWSItem::LoadFromTemplate(const CResRef& cResRef)
{
    CResGFF* pGFF = new CResGFF(0x7e9, "UTI ", cResRef);
    if (!pGFF->m_bLoaded) {
        delete pGFF;
        return 0;
    }
    CResStruct cTop;
    pGFF->GetTopLevelStruct(&cTop);
    LoadItemData(pGFF, &cTop);
    delete pGFF;
    return 1;
}

// FUNCTION: SWKOTOR 0x00560970
int CSWSItem::LoadItem(CResGFF* pGFF, CResStruct* pStruct)
{
    int bSuccess = 0;
    CResRef cEquipped = pGFF->ReadFieldCResRef(pStruct, "EquippedRes", bSuccess, "");
    if (bSuccess) {
        bSuccess = LoadFromTemplate(cEquipped);
        int bRead;
        int nValue = pGFF->ReadFieldBYTE(pStruct, "Dropable", bRead, 0);
        if (bRead) {
            m_bDroppable = nValue;
        }
        nValue = pGFF->ReadFieldBYTE(pStruct, "Pickpocketable", bRead, 0);
        if (bRead) {
            m_bPickpocketable = nValue;
        }
        return bSuccess;
    }
    bSuccess = 0;
    CResRef cInventory = pGFF->ReadFieldCResRef(pStruct, "InventoryRes", bSuccess, "");
    if (bSuccess) {
        bSuccess = LoadFromTemplate(cInventory);
        int bRead;
        int nValue = pGFF->ReadFieldBYTE(pStruct, "Dropable", bRead, 0);
        if (bRead) {
            m_bDroppable = nValue;
        }
        nValue = pGFF->ReadFieldBYTE(pStruct, "Pickpocketable", bRead, 0);
        if (bRead) {
            m_bPickpocketable = nValue;
        }
        return bSuccess;
    }
    LoadItemData(pGFF, pStruct);
    return 1;
}

// FUNCTION: SWKOTOR 0x0055D950
// NONMATCH: register rotation, and the flag word at +0x288 is reloaded
// around the bit 2 / bit 0 copies where the original keeps it in edi.
int CSWSItem::CopyItem(CSWSItem* pSource)
{
    if (!pSource) {
        return 0;
    }
    if (pSource->m_pRepository && (unsigned long)pSource->m_pRepository->m_aItems.num > 0) {
        return 0;
    }
    m_lsDescIdentified = pSource->m_lsDescIdentified;
    m_lsDescription = pSource->m_lsDescription;
    m_lsLocalizedName = pSource->m_lsLocalizedName;
    m_sTag = pSource->m_sTag;
    m_oidPossessor = pSource->GetPossessor();
    if (pSource->m_pRepository) {
        m_pRepository = new CItemRepository(GetId(), 0);
    }
    m_nIcon = pSource->m_nIcon;
    int i;
    m_nActiveProperties = pSource->m_nActiveProperties;
    if (m_nActiveProperties > 0) {
        m_pActiveProperties = new CSWItemProperty[m_nActiveProperties];
        for (i = 0; i < m_nActiveProperties; i++) {
            m_pActiveProperties[i] = pSource->m_pActiveProperties[i];
        }
    }
    m_nPassiveProperties = pSource->m_nPassiveProperties;
    if (m_nPassiveProperties > 0) {
        m_pPassiveProperties = new CSWItemProperty[m_nPassiveProperties];
        for (i = 0; i < m_nPassiveProperties; i++) {
            m_pPassiveProperties[i] = pSource->m_pPassiveProperties[i];
        }
    }
    m_nStackSize = pSource->m_nStackSize;
    m_bItemFlag2 = pSource->m_bItemFlag2;
    m_nCharges = pSource->m_nCharges;
    m_nMaxCharges = pSource->m_nMaxCharges;
    m_n244 = pSource->m_n244;
    m_nStackCount = pSource->m_nStackCount;
    m_nWeight = pSource->m_nWeight;
    m_bItemFlag0 = pSource->m_bItemFlag0;
    m_nEquippedSlots = pSource->m_nEquippedSlots;
    m_n265 = pSource->m_n265;
    m_nIconVariant = pSource->m_nIconVariant;
    m_bNonEquippable = pSource->m_bNonEquippable;
    m_nBaseItem = pSource->m_nBaseItem;
    m_n4 = pSource->m_n4;
    m_n5 = pSource->m_n5;
    m_n6 = pSource->m_n6;
    m_n7 = pSource->m_n7;
    m_n8 = pSource->m_n8;
    m_n9 = pSource->m_n9;
    g_pAppManager->GetServerExoApp()->GetModule()->AddObjectToLookupTable(m_sTag, GetId());
    return 1;
}
