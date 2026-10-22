// Pilot group 1: CSWSItemPropertyHandler. BEST ATTEMPT (not matching):
// register allocation (pProperty in edi, reloaded at the latch), frame (sub esp,8)
// and the reloaded m_nType compare differ. The bitfield m_nSubType:3 is right.

extern int EFFECT_TYPE_DAMAGE_INCREASE;            // 0x007466B0
extern unsigned short SUBTYPE_EQUIPPED;            // 0x007473BC
extern int ITEM_SLOT_DAMAGE_TYPE_RIGHT_WEAPON;     // 0x007468E4
extern int ITEM_SLOT_DAMAGE_TYPE_LEFT_WEAPON;      // 0x007468E8
extern int ITEM_SLOT_DAMAGE_TYPE_CLAW1;            // 0x007468EC
extern int ITEM_SLOT_DAMAGE_TYPE_CLAW2;            // 0x007468F0
extern int ITEM_SLOT_DAMAGE_TYPE_CLAW3;            // 0x007468F4
extern int ITEM_SLOT_DAMAGE_TYPE_ONHAND;           // 0x007468F8

class CSWRules {
public:
    char pad_0x0[0xaa];
    unsigned char m_nDifficultyRacialType;  // 0xaa
};
extern CSWRules* g_pRules;

class CGameEffect {
public:
    unsigned long m_nIDLow;     // 0x0
    unsigned long m_nIDHigh;    // 0x4
    unsigned short m_nType;     // 0x8
    unsigned short m_nSubType : 3;  // 0xa
    unsigned short m_nSubTypeRest : 13;
    char pad_0xc[0xc];
    unsigned long m_oidCreator; // 0x18
    int GetInteger(int nIndex);
};

struct CSWItemProperty {
    unsigned short m_nPropertyName;    // 0x0
    unsigned short m_nSubType;         // 0x2
    char pad_0x4[0x2];
    unsigned short m_nCostTableValue;  // 0x6
    char pad_0x8[0x1];
    unsigned char m_nParam1Value;      // 0x9
};

class CSWSItem {
public:
    char pad_0x0[0x14];
    unsigned long m_idSelf;     // 0x14
};

class CSWSCreatureStats {
public:
    char pad_0x0[0x136];
    unsigned short m_nDamageBonusEffectStart;  // 0x136
};

template <class T> class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
};

class CSWSCreature {
public:
    char pad_0x0[0x124];
    CExoArrayList<CGameEffect*> m_appliedEffects;   // 0x124
    char pad_0x130[0xa74 - 0x130];
    CSWSCreatureStats* m_pStats;                    // 0xa74
    void RemoveEffectById(unsigned long nIDLow, unsigned long nIDHigh);
};

class CSWSItemPropertyHandler {
public:
    int RemoveDamageBonus(CSWSItem* pItem, CSWItemProperty* pProperty,
                          CSWSCreature* pCreature, unsigned long nSlot);
};

// FUNCTION: SWKOTOR 0x004E86F0
int CSWSItemPropertyHandler::RemoveDamageBonus(CSWSItem* pItem, CSWItemProperty* pProperty,
                                               CSWSCreature* pCreature, unsigned long nSlot)
{
    int nDamageType;
    int nValue = pProperty->m_nCostTableValue;
    if (pProperty->m_nPropertyName == 0xb) {
        nDamageType = pProperty->m_nSubType;
    }
    else {
        nDamageType = pProperty->m_nParam1Value;
    }

    for (int i = pCreature->m_pStats->m_nDamageBonusEffectStart; i < pCreature->m_appliedEffects.num; i++) {
        CGameEffect* pEffect = pCreature->m_appliedEffects.element[i];
        if (pEffect->m_nType > EFFECT_TYPE_DAMAGE_INCREASE) {
            return 0;
        }
        if (pEffect->m_nType == EFFECT_TYPE_DAMAGE_INCREASE &&
            pEffect->m_nSubType == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetInteger(0) == nValue) {
            int bMatch = 0;
            if (nSlot == 0x10) {
                bMatch = pEffect->GetInteger(5) == ITEM_SLOT_DAMAGE_TYPE_RIGHT_WEAPON;
            }
            else if (nSlot == 0x20) {
                bMatch = pEffect->GetInteger(5) == ITEM_SLOT_DAMAGE_TYPE_LEFT_WEAPON;
            }
            else if (nSlot == 0x4000) {
                bMatch = pEffect->GetInteger(5) == ITEM_SLOT_DAMAGE_TYPE_CLAW1;
            }
            else if (nSlot == 0x8000) {
                bMatch = pEffect->GetInteger(5) == ITEM_SLOT_DAMAGE_TYPE_CLAW2;
            }
            else if (nSlot == 0x10000) {
                bMatch = pEffect->GetInteger(5) == ITEM_SLOT_DAMAGE_TYPE_CLAW3;
            }
            else if (nSlot == 8) {
                bMatch = pEffect->GetInteger(5) == ITEM_SLOT_DAMAGE_TYPE_ONHAND;
            }
            if (bMatch && pEffect->GetInteger(1) == 1 << nDamageType) {
                int bRemove = 1;
                if (pProperty->m_nPropertyName == 0xb || pProperty->m_nPropertyName == 0xc) {
                    if (pEffect->GetInteger(2) != g_pRules->m_nDifficultyRacialType) {
                        bRemove = 0;
                    }
                }
                if (bRemove) {
                    if (pProperty->m_nPropertyName == 0xc) {
                        if (pProperty->m_nSubType == 1) {
                            if (pEffect->GetInteger(3) != 1) bRemove = 0;
                        }
                        else if (pProperty->m_nSubType == 2) {
                            if (pEffect->GetInteger(4) != 2) bRemove = 0;
                        }
                        else if (pProperty->m_nSubType == 3) {
                            if (pEffect->GetInteger(4) != 3) bRemove = 0;
                        }
                    }
                    else if (pProperty->m_nPropertyName == 0xd) {
                        if (pEffect->GetInteger(2) != pProperty->m_nSubType) bRemove = 0;
                    }
                }
                if (bRemove) {
                    pCreature->RemoveEffectById(pEffect->m_nIDLow, pEffect->m_nIDHigh);
                    return 0;
                }
            }
        }
    }
    return 0;
}
