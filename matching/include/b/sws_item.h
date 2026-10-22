// CSWItem / CSWSItem / CItemRepository / CSWBaseItem (only what worker B uses).
#ifndef B_SWS_ITEM_H
#define B_SWS_ITEM_H

#include "b/res_gff.h"
#include "b/sws_creature.h"
#include "b/res_gff.h"
#include "b/exo_array_list.h"
#include "b/server_exo_app.h"
#include "b/sw_rules.h"
#include "b/exo_loc_string.h"

class CSWSItem;
class CItemRepository;

class CSWBaseItem {
public:
    CResRef GetModelResRef(unsigned short nIcon, short nType); // 0x005B3050

    char pad_0x0[0x9];
    unsigned char m_bWeapon;                                // 0x9
    char pad_0xa[0xc - 0xa];
    unsigned long m_nDamageFlags;                           // 0xc
    char pad_0x10[0x18 - 0x10];
    unsigned char m_bIconVariant;                           // 0x18
    char pad_0x19[0x1a - 0x19];
    unsigned char m_nRangedWeapon;                          // 0x1a
    unsigned char m_nWeaponSize;                            // 0x1b
    char pad_0x1c[0x1e - 0x1c];
    unsigned char m_nCritThreat;                            // 0x1e
    unsigned char m_nCritMultiplier;                        // 0x1f
    char pad_0x20[0x24 - 0x20];
    unsigned short m_nStackSize;                            // 0x24
    char pad_0x26[0x34 - 0x26];
    float m_fMaxRange;                                      // 0x34
    char pad_0x38[0x70 - 0x38];
    int m_bContainer;                                       // 0x70
    char pad_0x74[0x8a - 0x74];
    unsigned char m_nDefaultModelPart;                      // 0x8a
    char pad_0x8b[0xac - 0x8b];
    unsigned char m_nBaseType;                              // 0xac
    char pad_0xad[0xc4 - 0xad];
    float m_fCostMultiplier;                                // 0xc4

    void SetCostMultiplier(float fMultiplier) { m_fCostMultiplier = fMultiplier; }

    int IsRangedWeapon() { return m_nRangedWeapon > 0; }
};

class CSWBaseItemArray {
public:
    CSWBaseItem* GetBaseItem(int nBaseItem);                // 0x005B31A0
};

struct CSWItemProperty {
    unsigned short m_nPropertyName;                         // 0x0
    unsigned short m_nSubType;                              // 0x2
    unsigned char m_nCostTable;                             // 0x4
    char pad_0x5[0x6 - 0x5];
    unsigned short m_nCostValue;                            // 0x6
    unsigned char m_nParam1;                                // 0x8
    unsigned char m_nParam1Value;                           // 0x9
    unsigned char m_nChanceAppear;                          // 0xa
    char pad_0xb[0xc - 0xb];
    union {
        unsigned char m_bUseable;                           // 0xc (read as a byte)
        int m_nUseable;                                     // 0xc (reset as a dword)
    };
    unsigned char m_nUsesLeft;                              // 0x10
    unsigned char m_nUpgradeSlot;                           // 0x11 (0xff = always active)
    char pad_0x12[0x14 - 0x12];
    unsigned long m_nLastUsedDay;                           // 0x14
    unsigned long m_nLastUsedTime;                          // 0x18
};

class CSWSPlaceable : public CSWSObject {
public:
    int RemoveItem(CSWSItem* pItem);                        // 0x00584AC0
    int AcquireItem(CSWSItem** ppItem, unsigned long oidPossessor, int bFlag); // 0x00584B10
    void CloseInventory(unsigned long oidCloser, int bArg);  // 0x00587560
    unsigned char GetBodyBag() { return m_n230; }
    int GetD4() { return m_nD4; }
    int GetF8() { return m_nF8; }
    int Get260() { return m_n260; }
    int Get280() { return m_n280; }
    int Get328() { return m_n328; }
    int Get32C() { return m_n32C; }
    int Get330() { return m_n330; }

    char pad_0xf4[0xf8 - 0xf4];
    int m_nF8;                                              // 0xf8
    char pad_0xfc[0x230 - 0xfc];
    unsigned char m_n230;                                   // 0x230
    char pad_0x231[0x234 - 0x231];
    CExoLocString m_sDescription;                           // 0x234
    int m_nFactionId;                                       // 0x23c
    char pad_0x240[0x258 - 0x240];
    unsigned char m_nAppearance;                            // 0x258
    char pad_0x259[0x260 - 0x259];
    int m_n260;                                             // 0x260
    char pad_0x264[0x280 - 0x264];
    int m_n280;                                             // 0x280
    char pad_0x284[0x314 - 0x284];
    unsigned char m_nHardness;                              // 0x314
    unsigned char m_nFortSave;                              // 0x315
    unsigned char m_nWillSave;                              // 0x316
    char pad_0x317[0x328 - 0x317];
    int m_n328;                                             // 0x328
    int m_n32C;                                             // 0x32c
    int m_n330;                                             // 0x330
    char pad_0x334[0x378 - 0x334];
    CResRef m_cOnUsed;                                      // 0x378
    char pad_0x388[0x398 - 0x388];
    int m_n398;                                             // 0x398
};

class CScriptEvent {
public:
    CScriptEvent();                                         // 0x004D7540

    unsigned short m_nType;                                 // 0x0
    char pad_0x2[0x34 - 0x2];
};

extern const unsigned long EVENT_DELAY_INVENTORY;          // 0x007441DC (10)
extern const unsigned long EVENT_DESTROY_OBJECT;           // 0x007441E0 (11)

// Minimal stub for the item TUs; b/sws_message.h has the full class.
#ifndef B_SWS_MESSAGE_H
class CSWSMessage {
public:
    int SendServerToPlayerOpenInventory(CGameObject* pPlayer, unsigned long nItem,
                                        int bHasItems);     // 0x00569F40
    int SendServerToPlayerItemUses(CGameObject* pPlayer, unsigned long nItem,
                                   unsigned char nUseable, unsigned char nChanged,
                                   unsigned char* pUses);   // 0x00569E10
    int SendServerPlayerItemUpdate_DestroyItem(CGameObject* pPlayer,
                                               unsigned long nItem); // 0x00569FC0
};
#endif

class CServerAIMaster {
public:
    void AddObject(CSWSObject* pObject, int bArg);          // 0x004B0850
    void RemoveObject(CSWSObject* pObject);                 // 0x004AF3D0
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long nCaller,
                          unsigned long nObject, unsigned long nType,
                          CScriptEvent* pEvent);            // 0x004B08D0
    void OnItemPropertyApplied(CSWSItem* pItem, CSWItemProperty* pProperty,
                               CSWSCreature* pCreature, unsigned long nSlot,
                               int bArg);                   // 0x004AF110
    void OnItemPropertyRemoved(CSWSItem* pItem, CSWItemProperty* pProperty,
                               CSWSCreature* pCreature, unsigned long nSlot); // 0x004AF120
    void AppendToScriptsRun(const CExoString& sScript);     // 0x0052BFC0

    char pad_0x0[0x60];
    CExoString m_sScriptsRun;                               // 0x60
};

class CSWItem {
public:
    CSWItem();                                              // 0x005B4660
    virtual ~CSWItem();                                     // 0x005B4680
    CSWBaseItem* GetBaseItem();                             // 0x005B4790

    unsigned char m_n4;                                     // 0x4
    unsigned char m_n5;                                     // 0x5
    unsigned char m_n6;                                     // 0x6
    unsigned char m_n7;                                     // 0x7
    unsigned char m_n8;                                     // 0x8
    unsigned char m_n9;                                     // 0x9
    char pad_0xa[0xc - 0xa];
    int m_nBaseItem;                                        // 0xc
};

class CItemRepository {
public:
    CItemRepository(unsigned long oidParent, int bReadOnly); // 0x0055D290
    ~CItemRepository() {}                                   // 0x00555DE0
    int ClearItems();                                       // 0x0055F330
    void ClearNewItems(CExoArrayList<unsigned long>* pNew); // 0x00556050
    unsigned long GetItemId(int n);                         // 0x005560E0
    int CalculateContentsWeight();                          // 0x00556100
    void SetParentId(unsigned long oidParent);              // 0x00556150
    unsigned long FindItemWithBaseItemType(unsigned char nType, int nNth); // 0x00555E00
    unsigned long FindItemWithTag(const CExoString& sTag); // 0x00555ED0
    int GetItemInRepository(CSWSItem* pItem, int bRecurse); // 0x00555F40
    void AddItemAt(CSWSItem* pItem, int nPos);              // 0x0055D2C0
    int AddItem(CSWSItem** ppItem, int a2, int a3, int a4); // 0x0055D330
    int RemoveItem(CSWSItem* pItem);                        // 0x00555FD0

    unsigned long m_oidParent;                              // 0x0
    int m_nNewItems;                                        // 0x4 (items with m_bNewItem set)
    unsigned long m_bReadOnly : 1;                          // 0x8 bit 0
    CExoArrayList<unsigned long> m_aItems;                  // 0xc

    CSWSItem* GetItemAt(int n)
    {
        if (n < 0 || n >= m_aItems.num) return NULL;
        return g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(m_aItems.element[n]);
    }
};

class CSWSItem : public CSWItem, public CSWSObject {
public:
    void RemoveFromArea();                                  // 0x005538A0
    void ApplyItemProperties(CSWSCreature* pCreature, unsigned long nSlot, int bArg); // 0x00553BB0
    void RemoveItemProperties(CSWSCreature* pCreature, unsigned long nSlot); // 0x00553C30
    int GetDefaultModelPart();                              // 0x00553CC0
    int CompareItem(CSWSItem* pOther);                      // 0x00553CF0
    void AddRangeProperties(CExoString& sOut);              // 0x005543B0
    void AddWeaponSizeProperties(CExoString& sOut);         // 0x005544F0
    void AddCriticalThreatProperties(CExoString& sOut);     // 0x00558950
    void AddAttackBonusStrings(int nPropertyName, CExoString& sOut); // 0x005584C0
    void AddAttackModifierProperties(CExoString& sOut);     // 0x0055E930
    void GetSortedPropertyStrings(CExoArrayList<CExoString>& aStrings,
                                  CExoString& sOut);        // 0x005562D0
    void ReadContainerItemsFromGff(CResGFF* pGFF, CResStruct* pStruct); // 0x0055F0F0
    void DecrementUses(int nIndex, unsigned long oidPlayer); // 0x0055DE20
    CExoString GetPropertyDescription();                    // 0x0055F340
    // Description helpers not reconstructed yet (names are placeholders).
    void AddDescription556490(CExoString& sOut);            // 0x00556490
    void AddDescription556DE0(CExoString& sOut);            // 0x00556DE0
    void AddDescription558C10(CExoString& sOut);            // 0x00558C10
    void AddDescription5599D0(CExoString& sOut);            // 0x005599D0
    void AddDescription55A510(CExoString& sOut);            // 0x0055A510
    int SaveItemProperties(CResGFF* pGFF, CResStruct* pStruct); // 0x00555790
    int MergeItem(CSWSItem* pOther);                        // 0x00553F90
    unsigned long GetCost();                                        // 0x00554000
    unsigned long GetDamageFlags();                         // 0x00554050
    unsigned char GetMinEquipLevel();                       // 0x00554080
    void* GetClientItem();                                  // 0x00554190
    unsigned char GetUpgradableType(C2DA* p2DA);            // 0x005541C0
    void GetPropertyDamageRange(unsigned short nCost, int* pMin, int* pMax); // 0x00555380
    void GetPropertyChanceDurationDC(unsigned short nCost, unsigned char nParam,
                                     int* pChance, int* pDuration, int* pDC); // 0x005554D0
    unsigned short GetUsedActivePropertyUsesLeft(unsigned char nIndex); // 0x00555A60
    CResRef GetIcon();                                      // 0x005556B0
    void AddToArea(CSWSArea* pArea, float fX, float fY, float fZ, int bArg); // 0x00555720
    int SaveContainerItems(CResGFF* pGFF, CResStruct* pStruct); // 0x0055CFA0
    int SaveItem(CResGFF* pGFF, CResStruct* pStruct);       // 0x0055CCD0
    void CloseContainerForAll();                            // 0x0055DC10
    void CloseInventory(unsigned long nObject, int bArg);   // 0x0055D800
    void OpenInventory(unsigned long nOpener);              // 0x005561A0
    void SetPossessor(unsigned long oidPossessor, int a2, int a3, unsigned long oidArea); // 0x00553210
    void UpdateUsedActiveProperties();                      // 0x0055D040
    int AcquireItem(CSWSItem** ppItem, unsigned long oidFrom, int nArg); // 0x0055DCA0
    int IsChargePropertyName(unsigned short nName);         // 0x00553900
    int FindProperty(CSWItemProperty** ppProperty, unsigned short nName,
                     unsigned short nSubType);              // 0x005539C0
    void SetItemFlag0(int bValue);                          // 0x00554170
    int HasPassivePropertyAbove(unsigned short nName, unsigned short nSubType,
                                unsigned short nCost);      // 0x00555610
    int IsLightsaber();                                     // 0x00555690
    void ResetUsesPerDay(int bOnlyUnset);                   // 0x00555BF0
    int HasProperty(unsigned short nName, unsigned short nSubType); // 0x00555C90
    int GetWeight();                                        // 0x005562A0
    void InitRepository(unsigned long oidParent);           // 0x0055F050
    CSWSItem* SplitItem(int nCount);                        // 0x0055F280
    CSWSItem(unsigned long nId);                            // 0x005530A0
    virtual ~CSWSItem();                                    // 0x0055EC70
    int CopyItem(CSWSItem* pSource);                        // 0x0055D950
    int LoadFromTemplate(const CResRef& cResRef);           // 0x005608B0
    int LoadItem(CResGFF* pGFF, CResStruct* pStruct);       // 0x00560970
    int LoadItemData(CResGFF* pGFF, CResStruct* pStruct);   // 0x0055FCD0

    CSWBaseItem* GetBaseItemFromRules()
    {
        int nBaseItem = m_nBaseItem;
        return g_pRules->m_pBaseItemArray->GetBaseItem(nBaseItem);
    }
    CSWBaseItem* GetBaseItemFromRules2()
    {
        int nBaseItem = m_nBaseItem;
        return g_pRules->GetBaseItemArray()->GetBaseItem(nBaseItem);
    }
    CSWItemProperty* GetActiveProperty(int n)
    {
        if (n < 0 || n >= m_nActiveProperties) return NULL;
        return &m_pActiveProperties[n];
    }
    CSWItemProperty* GetPassiveProperty(int n)
    {
        if (n < 0 || n >= m_nPassiveProperties) return NULL;
        return &m_pPassiveProperties[n];
    }

    char pad_0x10f4[0x108 - 0x10 - 0xf4];
    int m_bPlotItem;                                        // 0x108
    char pad_0x10c[0x238 - 0x10c];
    unsigned char m_nUseableProperties;                     // 0x238 (bit per active property)
    unsigned char m_aUsesLeft[8];                           // 0x239
    char pad_0x241[0x244 - 0x241];
    union {
        int m_n244;                                         // 0x244
        unsigned short m_nUpgradeLevel;                     // 0x244 (read as a word)
    };
    int m_nActiveProperties;                               // 0x248
    int m_nPassiveProperties;                                // 0x24c
    CSWItemProperty* m_pActiveProperties;                  // 0x250
    CSWItemProperty* m_pPassiveProperties;                   // 0x254
    int m_nCharges;                                         // 0x258
    int m_nMaxCharges;                                      // 0x25c
    int m_nStackSize;                                       // 0x260 (saved as AddCost)
    unsigned char m_nIcon;                                  // 0x264
    unsigned char m_n265;                                   // 0x265
    unsigned char m_nIconVariant;                           // 0x266
    char pad_0x267[0x268 - 0x267];
    unsigned long m_oidPossessor;                           // 0x268
    CItemRepository* m_pRepository;                         // 0x26c
    CExoLocString m_lsDescIdentified;                       // 0x270
    CExoLocString m_lsDescription;                          // 0x278
    CExoLocString m_lsLocalizedName;                        // 0x280
    unsigned long m_bItemFlag0 : 1;                         // 0x288 bit 0
    unsigned long m_bInventoryOpen : 1;                     // 0x288 bit 1
    unsigned long m_bItemFlag2 : 1;                         // 0x288 bit 2
    unsigned long m_bDroppable : 1;                         // 0x288 bit 3
    unsigned long m_bPickpocketable : 1;                    // 0x288 bit 4
    unsigned long m_bStolen : 1;                            // 0x288 bit 5
    unsigned long m_bNonEquippable : 1;                     // 0x288 bit 6
    unsigned long m_bNewItem : 1;                           // 0x288 bit 7
    unsigned long m_bDeleting : 1;                          // 0x288 bit 8
    int m_nStackCount;                                      // 0x28c
    int m_nWeight;                                          // 0x290
    unsigned long m_nEquippedSlots;                         // 0x294
    char pad_0x298[0x29c - 0x298];
};

#endif
