// Worker L matching headers: the in-game inventory (CSWGuiInGameInventory),
// equipment (CSWGuiInGameEquip) and container (CSWGuiContainer) screens,
// 0x006B2A80-0x006BB530. The panel layouts are in sw_gui_inventory_equip.h;
// this header adds the server/client declarations their methods use.
#ifndef L_SW_GUI_INVENTORY_H
#define L_SW_GUI_INVENTORY_H

#include "l/sw_gui_store.h"

extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C
extern const unsigned short RACIAL_TYPE_DROID;      // 0x0074BF10
extern unsigned long g_aInventoryFilterNames[6];    // 0x00756444 (descriptive)

// Per equipment slot (9 entries of 0x14 bytes at 0x00756560), e.g.
// {0x20, "INV_WEAP_L", "weap_l", 0x7A92, 0x7A92}.
struct CSWEquipSlotInfo {
    unsigned long m_nSlotMask;                      // 0x00
    const char* m_sControl;                         // 0x04 (GUI control tag)
    const char* m_sIcon;                            // 0x08
    unsigned long m_nStrRef;                        // 0x0c
    unsigned long m_nStrRef2;                       // 0x10
};
extern CSWEquipSlotInfo g_aEquipSlots[9];           // 0x00756560 (descriptive)

class CSWCCreatureStats {
public:
    char pad_0x0[0x24];
    unsigned short m_nRace;                         // 0x24
    unsigned short GetRace() { return m_nRace; }
};

// Virtual interface of the client creature (vtable slot 0x148 only); the
// shared CSWCCreature declaration carries no vtable.
class CSWCCreaturePortraitVirtuals {
public:
    virtual void v000();
    virtual void v004();
    virtual void v008();
    virtual void v00C();
    virtual void v010();
    virtual void v014();
    virtual void v018();
    virtual void v01C();
    virtual void v020();
    virtual void v024();
    virtual void v028();
    virtual void v02C();
    virtual void v030();
    virtual void v034();
    virtual void v038();
    virtual void v03C();
    virtual void v040();
    virtual void v044();
    virtual void v048();
    virtual void v04C();
    virtual void v050();
    virtual void v054();
    virtual void v058();
    virtual void v05C();
    virtual void v060();
    virtual void v064();
    virtual void v068();
    virtual void v06C();
    virtual void v070();
    virtual void v074();
    virtual void v078();
    virtual void v07C();
    virtual void v080();
    virtual void v084();
    virtual void v088();
    virtual void v08C();
    virtual void v090();
    virtual void v094();
    virtual void v098();
    virtual void v09C();
    virtual void v0A0();
    virtual void v0A4();
    virtual void v0A8();
    virtual void v0AC();
    virtual void v0B0();
    virtual void v0B4();
    virtual void v0B8();
    virtual void v0BC();
    virtual void v0C0();
    virtual void v0C4();
    virtual void v0C8();
    virtual void v0CC();
    virtual void v0D0();
    virtual void v0D4();
    virtual void v0D8();
    virtual void v0DC();
    virtual void v0E0();
    virtual void v0E4();
    virtual void v0E8();
    virtual void v0EC();
    virtual void v0F0();
    virtual void v0F4();
    virtual void v0F8();
    virtual void v0FC();
    virtual void v100();
    virtual void v104();
    virtual void v108();
    virtual void v10C();
    virtual void v110();
    virtual void v114();
    virtual void v118();
    virtual void v11C();
    virtual void v120();
    virtual void v124();
    virtual void v128();
    virtual void v12C();
    virtual void v130();
    virtual void v134();
    virtual void v138();
    virtual void v13C();
    virtual void v140();
    virtual void v144();
    virtual CResRef GetPortrait(int bSmall);        // 0x148
};

class CSWSCombatRound {
public:
    void RemoveAllActions();                        // 0x004D3770
    char pad_0x0[0x9cc];
    unsigned long m_oidAttackTarget;                // 0x9cc (descriptive)
    unsigned long GetAttackTarget() { return m_oidAttackTarget; }
};

extern int g_bClearTargetOnAction;                 // 0x008338F8
extern Vector g_vAbilityColorAvailable;             // 0x007A23D8

class CSWCMessage {
public:
    int SendPlayerToServerCloseContainer(unsigned long oidContainer, unsigned long bFlag);  // 0x00677630 (descriptive)
};

class CSWSArea {
public:
    char pad_0x0[0x2b0];
    int m_bNoEquipChange;                           // 0x2b0 (descriptive)
};

class CSWCArea {
public:
    CSWSArea* GetServerArea();                      // 0x00605640
};

class CSWCItem {
public:
    void SetPowered(int a, int b, int c, int d);    // 0x00647130
};

#endif // L_SW_GUI_INVENTORY_H
