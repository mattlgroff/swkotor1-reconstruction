// Worker L matching headers: the workbench upgrade screens (swguiupgrade.o,
// 0x006C2650-0x006C7D10): CSWUpgradeItemEntry (0x3A4, vtable 0x00757108),
// CSWGuiUpgradeSelection (0x1158, vtable 0x007571B0), CSWGuiUpgradeItemSelect
// (0xC48, vtable 0x00757228) and the outer CSWGuiUpgrade panel (0x2FB8,
// vtable 0x00757298, members trimmed to what this batch uses).
#ifndef L_SW_GUI_UPGRADE_H
#define L_SW_GUI_UPGRADE_H

#include "l/sw_gui_inventory_equip.h"

// The original's CSWGuiExtent has a zeroing ctor (returned through a hidden pointer).
CSWGuiZeroExtent IntersectExtents(const CSWGuiExtent& a, const CSWGuiExtent& b);  // 0x0040A4A0

class CExoLocString {
public:
    CExoLocString();                                // 0x005E9E10
    CExoLocString(const CExoLocString& s);          // 0x005E9F80
    ~CExoLocString();                               // 0x005EA000
    int GetString(unsigned long nLanguage, CExoString* pOut, unsigned char nGender) const;  // 0x005EA130
    void* m_pInternal;
    unsigned long m_n4;
};

class CExoResMan {
public:
    int Exists(const CResRef& cResRef, unsigned short nType, unsigned long* pTable);  // 0x00408BC0
    int CreateDirectory2(const CExoString& sAlias); // 0x004068C0
    int GetFreeDiskSpace(const CExoString& sAlias, unsigned __int64* pFree);  // 0x004067B0
    int AddResourceDirectory(const CExoString& sAlias);     // 0x00408800
    int RemoveResourceDirectory(const CExoString& sAlias);  // 0x004088D0
    int CleanDirectory(const CExoString& sAlias, int bDirs, int bRecurse);   // 0x00409460
    int NukeDirectory(const CExoString& sAlias, int bDirs, int bRecurse);    // 0x00409480
};
extern CExoResMan* g_pExoResMan;                    // 0x007A39E8

class CSWBaseItem;
class CSWBaseItemArrayL {
public:
    CSWBaseItem* GetBaseItem(int nBaseItem);        // 0x005B31A0
};

// Per upgrade type (1-4) and slot: the empty-slot icon and slot name.
struct CSWUpgradeSlotInfo {
    const char* m_sIcon;
    int m_nNameStrRef;
    int m_n8;
};
extern CSWUpgradeSlotInfo g_aUpgradeSlots[4][4];    // 0x00756FB4

class CSWSItem;
class CSWInventory {
public:
    CSWSItem* GetItemInSlot(unsigned long nSlot);   // 0x005A4C20
    unsigned long GetSlotFromItem(CSWSItem* pItem); // 0x005A4980
};
class CSWSItem {
public:
    virtual ~CSWSItem();
    unsigned char GetUpgradableType(C2DA* p2DA);    // 0x005541C0
    CExoString GetPropertyDescription();            // 0x0055F340
    CExoString GetKeyedPropertyString(unsigned char nKey);  // 0x0055F510
    CSWSItem* SplitItem(int nCount);                // 0x0055F280
    CResRef GetIcon();                              // 0x005556B0
    unsigned long GetCost();                        // 0x00554000
    char pad_0x4[0xc - 0x4];
    int m_nBaseItem;                                // 0x0c
    int GetBaseItemType() { return m_nBaseItem; }
    char pad_0x10[0x4];
    unsigned long m_idSelf;                         // 0x14
    char pad_0x18[0x108 - 0x18];
    int m_bPlotItem;                                // 0x108
    char pad_0x10c[0x268 - 0x10c];
    unsigned long m_oidPossessor;                   // 0x268
    char pad_0x26c[0x280 - 0x26c];
    CExoLocString m_sLocName;                       // 0x280
    unsigned long m_bItemFlag0 : 1;                 // 0x288 bit 0
    unsigned long m_bItemFlag1 : 1;                 //       bit 1
    unsigned long m_bInfinite : 1;                  //       bit 2 (store stock)
    int m_nStackSize;                               // 0x28c
    int GetStackSize() { return m_nStackSize; }
    char pad_0x290[0x4];
    unsigned long m_nUpgrades;                      // 0x294
};
class CItemRepository {
public:
    CSWSItem* GetItemAt(int nIndex);                // 0x005560B0
    int AddItem(CSWSItem** ppItem, int nX, int nY, int bAllowMerge);   // 0x0055D330
    int RemoveItem(CSWSItem* pItem);                // 0x00555FD0
    char pad_0x0[0x10];
    unsigned long m_nItems;                         // 0x10
    unsigned long FindItemWithTag(const CExoString& sTag);  // 0x00555ED0
};

class CSWSCreatureStats;
class CSWSCreature {
public:
    int GetGold();                                  // 0x004EDD60
    void SetGold(int nGold);                        // 0x004EDD90
    CItemRepository* GetItemRepository(int bParty); // 0x004EF770
    int UnequipItem(CSWSItem* pItem);               // 0x004FAA70
    void EquipItem(unsigned long nSlot, CSWSItem* pItem, int bUpdate, int b2, int b3);    // 0x004FEB90
    unsigned char CanEquipItem(CSWSItem* pItem, unsigned long* pSlot, int b1, int b2, int b3);  // 0x0051AA60
    char pad_0x0[0x4];
    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0xa2c - 0x8];
    CSWInventory* m_pInventory;                     // 0xa2c
    char pad_0xa30[0xa74 - 0xa30];
    CSWSCreatureStats* m_pStats;                    // 0xa74
    CSWSCreatureStats* GetStats() { return m_pStats; }
    CSWInventory* GetInventory() { return m_pInventory; }
};

class CGuiInGameUpgrade {
public:
    void HideUpgradeScreen();                       // 0x0062E870
};

extern Vector g_vDifficultyExitColor;               // 0x007A23B4
extern Vector g_vAbilityColorGranted;               // 0x007A23C0
extern Vector g_vKeyMapLockedColor;                 // 0x007A2384
extern Vector g_vKeyMapLockedTextColor;             // 0x007A2408
extern Vector g_vUpgradeAvailableColor;             // 0x007A2378 (descriptive)

// An upgradable item line in the item-select list box. vtable 0x00757108.
#ifdef L_UPGRADE_ENTRY_DTOR
class __declspec(novtable) CSWUpgradeItemEntry : public CSWGuiButton {
#else
class CSWUpgradeItemEntry : public CSWGuiButton {
#endif
public:
    CSWUpgradeItemEntry();                          // 0x006C3E00
    __declspec(noinline) virtual ~CSWUpgradeItemEntry(); // 0x006C5AD0
    void SetExtent(const CSWGuiExtent& extent);     // 0x04 (0x006C2650)
#ifdef L_GUI_CONTROL_DRAW_SLOT
    void Draw(float fDelta);                        // 0x38 (0x006C27C0)
#endif
    virtual void v9C();
    virtual void Initialize(CSWGuiText* pText, CSWGuiBorder* pBorder,
                            CSWGuiBorder* pHilight, int nWidth);    // 0xa0 (0x006C2830)
    void SetItem(unsigned long oidItem, int bEquipped, unsigned long oidOwner,
                 unsigned long nSlot);              // 0x006C3EA0

    unsigned long m_oidItem;                        // 0x1c4
    unsigned long m_oidOwner;                       // 0x1c8
    unsigned long GetOwner() { return m_oidOwner; }
    unsigned long m_nSlot;                          // 0x1cc
    unsigned long GetSlot() { return m_nSlot; }
    unsigned long m_bEquipped : 1;                  // 0x1d0 bit 0
    unsigned char m_nType;                          // 0x1d4
    CSWGuiBorder m_cIcon;                           // 0x1d8
    CSWGuiBorder m_cIconHilight;                    // 0x24c
    CSWGuiBorder m_cIconOverlay;                    // 0x2c0
    CSWGuiText m_cCount;                            // 0x334
};

class CSWGuiUpgradeSelection;
class CSWGuiUpgradeItemSelect;

class Quaternion {
public:
    Quaternion(float fX, float fY, float fZ, float fW)
    {
        x = fX;
        y = fY;
        z = fZ;
        w = fW;
    }
    float x, y, z, w;
};

class CAurObject;
class CAurScene {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void SetRoom(const char* sRoom, const Vector& vPosition,
                         const Quaternion& qOrientation);   // 0x70 (descriptive)
};

// Model scene drawable (vtable 0x0073E3F0), 0x34 bytes.
class CSWGuiScene {
public:
    CSWGuiScene();                                  // 0x004174B0
    ~CSWGuiScene();
    CAurObject* AddModel(const CExoString& sModel, int nIndex);    // 0x00417620
    void* m_pVtable;
    char pad_0x4[0x10];
    CAurScene* m_pScene;                            // 0x14
    char pad_0x18[0x34 - 0x18];
};

extern int g_bDo3DGui;                              // 0x0078D1E4 (descriptive)

// Inline 3D model view control (vtable 0x00752E30); dtor 0x0067AC50, 0x90 bytes.
class CSWGui3DSceneView : public CSWGuiControl {
public:
    CSWGui3DSceneView() {}
    ~CSWGui3DSceneView();                           // 0x0067AC50
    CSWGuiScene m_cScene;                           // 0x5c
};

// vtable 0x00757298 (0x2FB8 bytes), layout "upgrade".
class CSWGuiUpgrade : public CSWGuiPanel {
public:
    CSWGuiUpgrade(CSWGuiManager* pManager);         // 0x006C6B60
    __declspec(noinline) virtual ~CSWGuiUpgrade();  // 0x006C4AD0
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006C6A80)
    virtual void OnPanelRemoved();                  // 0x4c (0x006C25F0)
    void ShowItems(int bShow);                      // 0x006C2F80
    void SetDescription(const CExoString& sText);   // 0x006C32E0
    void UpdateItems();                             // 0x006C3AA0
    void OnEnterSlot(CSWGuiControl* pControl);      // 0x006C3C30
    void CloseItems();                              // 0x006C4D30
    void OnAssemble(CSWGuiControl* pControl);       // 0x006C6190
    void CancelPanel();                             // 0x006C61F0
    void OnControlEntered(CSWGuiControl* pControl); // 0x006C5370
    void OnUpgradeSelected(CSWGuiControl* pControl);    // 0x006C5510
    CSWUpgradeItemEntry* CreateItemEntry(unsigned long oidItem, unsigned char nSlot);  // 0x006C58C0
    void InsertUpgrade(CSWSItem* pItem, int nSlot); // 0x006C59A0
    void VerifyUpgrade(CSWSItem* pItem);            // 0x006C6120
    void OnSlotSelected(CSWGuiControl* pControl);   // 0x006C6500
    void GetArmorModelAndTextureResRef(int nRow, int nModelVariation, int nTextureVariation,
                                       CExoString* pModel, CExoString* pTexture);  // 0x006C3460
    void HidePanel()
    {
        m_nFlags &= ~0x100;
        unsigned long nFlags = m_nFlags;
        if ((nFlags & 0x600) != 0x400) {
            m_nFlags = (nFlags & ~0x400) | 0x200;
        }
    }

    CSWGuiButton m_aSlots4[4];                      // 0x64
    CSWGuiButton m_aSlots3[3];                      // 0x774
    CSWGuiLabel m_aSlotLabels4[4];                  // 0xcc0
    CSWGuiLabel m_aSlotLabels3[3];                  // 0x11c0
    CSWGuiListBox m_lbItems;                        // 0x1580
    CSWGuiListBox m_lbDesc;                         // 0x1860
    CSWGuiListBox m_lbDescLS;                       // 0x1b40
    CSWGuiLabel m_lblTitle;                         // 0x1e20
    CSWGuiLabel m_lblDescription;                   // 0x1f60
    CSWGuiLabel m_lbl20a0;                          // 0x20a0
    CSWGuiLabel m_lblDescBG;                        // 0x21e0
    CSWGuiLabel m_lblDescBGLS;                      // 0x2320
    CSWGuiLabel m_lblProperty;                      // 0x2460
    CSWGuiLabel m_lblSlotName;                      // 0x25a0
    CSWGuiLabel m_lblLSSlotName;                    // 0x26e0
    CSWGuiLabel m_lblUpgrades;                      // 0x2820
    CSWGuiLabel m_lblUpgradeCount;                  // 0x2960
    CSWGuiButton m_btnAssemble;                     // 0x2aa0
    CSWGui3DSceneView m_cModel;                     // 0x2c64
    CSWGui3DSceneView m_cModelLS;                   // 0x2cf4
    CSWGuiButton m_btnBack;                         // 0x2d84
    unsigned long m_bItemsOpen : 1;                 // 0x2f48 bit 0
    unsigned char m_nType;                          // 0x2f4c
    char pad_0x2f4d[0x2f54 - 0x2f4d];
    CSWSItem* m_pItem;                              // 0x2f54
    CSWSItem* m_pOriginalItem;                      // 0x2f58
    CExoArrayList<CSWSItem*> m_lstRemoved;          // 0x2f5c
    CExoArrayList<CSWSItem*> m_lstAdded;            // 0x2f68
    CSWSItem* m_aSlotItems[4];                      // 0x2f74
    CExoString m_aSlotTags[4];                      // 0x2f84
    unsigned char m_aSlotRows[4];                   // 0x2fa4
    C2DA* m_pUpgrade2DA;                            // 0x2fa8
    C2DA* m_pCrystals2DA;                           // 0x2fac
    CSWGuiControl* m_pSelectedControl;              // 0x2fb0
    CSWGuiUpgradeItemSelect* m_pItemSelect;         // 0x2fb4
};

// vtable 0x00757228; layout "upgradeitems".
#ifdef L_UPGRADE_PANEL_DTORS
class __declspec(novtable) CSWGuiUpgradeItemSelect : public CSWGuiPanel {
#else
class CSWGuiUpgradeItemSelect : public CSWGuiPanel {
#endif
public:
    CSWGuiUpgradeItemSelect(CSWGuiManager* pManager);  // 0x006C7630
    __declspec(noinline) virtual ~CSWGuiUpgradeItemSelect(); // 0x006C47A0
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006C2D30)
    unsigned long GetSelectedOwner();               // 0x006C2A70 (descriptive)
    unsigned long GetSelectedSlot();                // 0x006C2A90 (descriptive)
    void SetDescription(const CExoString& sText);   // 0x006C2C80
    virtual void OnPanelAdded();                    // 0x48 (0x006C6330)
    void OnControlEntered(CSWGuiControl* pControl); // 0x006C4880
    void OnUpgradableSelected(CSWGuiControl* pControl); // 0x006C2DF0
    void BuildItemList(CSWSCreature* pCreature, CExoArrayList<CSWGuiControl*>* pList, C2DA* p2DA);   // 0x006C4960
    void BuildUpgradeList();                        // 0x006C5B90
    void SetUpgradeType(int nType) { m_nUpgradeType = nType; }
    void FinishUpgrading();                         // 0x006C5E90
    void HidePanel()
    {
        m_nFlags &= ~0x100;
        unsigned long nFlags = m_nFlags;
        if ((nFlags & 0x600) != 0x400) {
            m_nFlags = (nFlags & ~0x400) | 0x200;
        }
    }

    CSWGuiListBox m_lbItems;                        // 0x64
    CSWGuiListBox m_lbDescription;                  // 0x344
    CSWGuiLabel m_lblTitle;                         // 0x624
    CSWGuiLabel m_lblDescription;                   // 0x764
    CSWGuiButton m_btnUpgradeItem;                  // 0x8a4
    CSWGuiButton m_btnBack;                         // 0xa68
    unsigned char m_nUpgradeType;                   // 0xc2c
    char pad_0xc2d[0x3];
    unsigned long m_bUnequipped : 1;                // 0xc30 bit 0
    unsigned long m_bOtherUnequipped : 1;           //       bit 1
    unsigned long m_oidOtherItem;                   // 0xc34
    CSWSItem* m_pItem;                              // 0xc38
    CSWUpgradeItemEntry* m_pSelectedEntry;          // 0xc3c
    CSWGuiUpgrade* m_pUpgrade;                      // 0xc40
    CSWGuiUpgradeSelection* m_pSelection;           // 0xc44
};

// vtable 0x007571B0; layout "upgradesel".
#ifdef L_UPGRADE_PANEL_DTORS
class __declspec(novtable) CSWGuiUpgradeSelection : public CSWGuiPanel {
#else
class CSWGuiUpgradeSelection : public CSWGuiPanel {
#endif
public:
    CSWGuiUpgradeSelection(CSWGuiManager* pManager);   // 0x006C78D0
    __declspec(noinline) virtual ~CSWGuiUpgradeSelection(); // 0x006C4450
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006C2B10)
    virtual void OnPanelAdded();                    // 0x48 (0x006C4520)
    void GetUpgradableItems(CSWSCreature* pCreature, int* pAvailable, C2DA* p2DA); // 0x006C2AB0
    void OnTypeSelected(CSWGuiControl* pControl);   // 0x006C2B60 (descriptive)
    void OnEnter(CSWGuiControl* pControl);          // 0x006C2BD0 (descriptive)

    CSWGuiLabel m_lblTitle;                         // 0x64
    CSWGuiButton m_btnUpgradeItems;                 // 0x1a4
    CSWGuiButton m_btnBack;                         // 0x368
    CSWGuiRadioButton m_aTypes[4];                  // 0x52c
    CSWGuiLabel m_aTypeLabels[4];                   // 0xc3c
    unsigned long m_oidItem;                        // 0x113c
    int m_aAvailable[4];                            // 0x1140
    CSWGuiUpgradeItemSelect* m_pItemSelect;         // 0x1150
    CSWGuiRadioButton* m_pEntered;                  // 0x1154
};

#endif // L_SW_GUI_UPGRADE_H
