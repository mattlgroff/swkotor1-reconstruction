// CSWSCreatureStats item power lookups (b005a_f2_dialog_stats).

class CSWItemProperty {
public:
    unsigned short m_nPropertyName;  // +0x0
    unsigned short m_nSubType;       // +0x2
    char pad_0x4[0x8];
    unsigned char m_nUsesPerDay;     // +0xc
    char pad_0xd[0x4];
    unsigned char m_nUpgradeType;    // +0x11
};

class CSWSItem {
public:
    int GetPropertyByTypeExists(unsigned short nType, unsigned short nSubType);
    CSWItemProperty *GetPassiveProperty(int nIndex);
    char pad_0x0[0xc];
    int m_nBaseItem;                 // +0xc
    int GetBaseItem() { return m_nBaseItem; }
    char pad_0x10[0x4];
    unsigned long m_idSelf;          // +0x14
    char pad_0x18[0x230];
    int m_nPassiveProperties;        // +0x248
    char pad_0x24c[0xc];
    int m_nCharges;                  // +0x258
    char pad_0x25c[0x38];
    unsigned long m_nUpgrades;       // +0x294
};

class CSWBaseItem {
public:
    char pad_0x0[0x4];
    int m_bNotSlotted;               // +0x4
};

class CSWBaseItemArray {
public:
    CSWBaseItem *GetBaseItem(int nBaseItem);
};

class CSWRules {
public:
    char pad_0x0[0x34];
    CSWBaseItemArray *m_pBaseItemArray;  // +0x34
    CSWBaseItemArray *GetBaseItemArray() { return m_pBaseItemArray; }
};

extern CSWRules *g_pRules;

class CItemRepository {
public:
    CSWSItem *ItemListGetItem(unsigned long nIndex);
    char pad_0x0[0x10];
    unsigned long m_nItemCount;      // +0x10
};

class CSWSInventory {
public:
    CSWSItem *GetItemInSlot(unsigned long nSlot);
};

class CSWSCreature {
public:
    CItemRepository *GetItemRepository(int bEquipped);
    CSWSInventory *GetInventory() { return m_pInventory; }
    char pad_0x0[0xa2c];
    CSWSInventory *m_pInventory;     // +0xa2c
};

class CSWSCreatureStats {
public:
    int GetHasPowerFromItem(int nPower);
    int GetItemDetailsForPower(int nPower, unsigned long *pItemId, int *pProperty);

    char pad_0x0[0x24];
    CSWSCreature *m_pBaseCreature;   // +0x24
};

// FUNCTION: SWKOTOR 0x005A72E0
int CSWSCreatureStats::GetHasPowerFromItem(int nPower)
{
    if (m_pBaseCreature->m_pInventory) {
        for (unsigned long nSlot = 0; nSlot < 18; nSlot++) {
            CSWSItem *pItem = m_pBaseCreature->GetInventory()->GetItemInSlot(1 << nSlot);
            if (pItem && pItem->GetPropertyByTypeExists(10, 0)) {
                for (int i = 0; i < pItem->m_nPassiveProperties; i++) {
                    if (pItem->GetPassiveProperty(i)->m_nPropertyName == 10 &&
                        pItem->GetPassiveProperty(i)->m_nUsesPerDay) {
                        CSWItemProperty *pProperty = pItem->GetPassiveProperty(i);
                        if ((pProperty->m_nUpgradeType == 0xff ||
                             (pItem->m_nUpgrades & (1 << pProperty->m_nUpgradeType))) &&
                            pItem->GetPassiveProperty(i)->m_nSubType == nPower)
                            return 1;
                    }
                }
            }
        }
    }
    CItemRepository *pRepository = m_pBaseCreature->GetItemRepository(1);
    if (pRepository) {
        for (unsigned long k = 0; k < pRepository->m_nItemCount; k++) {
            CSWSItem *pItem = pRepository->ItemListGetItem(k);
            if (pItem && !g_pRules->m_pBaseItemArray->GetBaseItem(pItem->GetBaseItem())->m_bNotSlotted &&
                pItem->GetPropertyByTypeExists(10, 0)) {
                for (int i = 0; i < pItem->m_nPassiveProperties; i++) {
                    if (pItem->GetPassiveProperty(i)->m_nPropertyName == 10 &&
                        (pItem->GetPassiveProperty(i)->m_nUsesPerDay || pItem->m_nCharges > 0)) {
                        CSWItemProperty *pProperty = pItem->GetPassiveProperty(i);
                        if ((pProperty->m_nUpgradeType == 0xff ||
                             (pItem->m_nUpgrades & (1 << pProperty->m_nUpgradeType))) &&
                            pItem->GetPassiveProperty(i)->m_nSubType == nPower)
                            return 1;
                    }
                }
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005A74A0
int CSWSCreatureStats::GetItemDetailsForPower(int nPower, unsigned long *pItemId, int *pProperty)
{
    if (m_pBaseCreature->m_pInventory) {
        for (unsigned long nSlot = 0; nSlot < 18; nSlot++) {
            CSWSItem *pItem = m_pBaseCreature->GetInventory()->GetItemInSlot(1 << nSlot);
            if (pItem && pItem->GetPropertyByTypeExists(10, 0)) {
                for (int i = 0; i < pItem->m_nPassiveProperties; i++) {
                    if (pItem->GetPassiveProperty(i)->m_nPropertyName == 10 &&
                        pItem->GetPassiveProperty(i)->m_nUsesPerDay) {
                        CSWItemProperty *pProp = pItem->GetPassiveProperty(i);
                        if ((pProp->m_nUpgradeType == 0xff ||
                             (pItem->m_nUpgrades & (1 << pProp->m_nUpgradeType))) &&
                            pItem->GetPassiveProperty(i)->m_nSubType == nPower) {
                            *pItemId = pItem->m_idSelf;
                            *pProperty = i;
                            return 1;
                        }
                    }
                }
            }
        }
    }
    CItemRepository *pRepository = m_pBaseCreature->GetItemRepository(1);
    if (pRepository) {
        for (unsigned long k = 0; k < pRepository->m_nItemCount; k++) {
            CSWSItem *pItem = pRepository->ItemListGetItem(k);
            if (pItem && !g_pRules->m_pBaseItemArray->GetBaseItem(pItem->GetBaseItem())->m_bNotSlotted &&
                pItem->GetPropertyByTypeExists(10, 0)) {
                for (int i = 0; i < pItem->m_nPassiveProperties; i++) {
                    if (pItem->GetPassiveProperty(i)->m_nPropertyName == 10 &&
                        (pItem->GetPassiveProperty(i)->m_nUsesPerDay || pItem->m_nCharges > 0)) {
                        CSWItemProperty *pProp = pItem->GetPassiveProperty(i);
                        if ((pProp->m_nUpgradeType == 0xff ||
                             (pItem->m_nUpgrades & (1 << pProp->m_nUpgradeType))) &&
                            pItem->GetPassiveProperty(i)->m_nSubType == nPower) {
                            *pItemId = pItem->m_idSelf;
                            *pProperty = i;
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
