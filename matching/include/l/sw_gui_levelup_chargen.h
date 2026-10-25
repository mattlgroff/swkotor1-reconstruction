// CSWGuiLevelUpCharGen state and lifecycle leaves (vtable 0x00758F50).
#ifndef L_SW_GUI_LEVELUP_CHARGEN_H
#define L_SW_GUI_LEVELUP_CHARGEN_H

#include "l/sw_gui_in_game_abilities.h"

class CSWGuiInGameCharacter : public CSWGuiPanel {
public:
    void AttachModel();                             // inferred, 0x006AEBF0
};

class CSWGuiLevelUpCharGen : public CSWGuiPanel {
public:
    CSWCCreature* m_pSnapshot;                      // 0x64
    CSWGuiInGameCharacter* m_pCharacter;            // 0x68
    CSWGuiPanel* m_pLevelUpPanel;                   // 0x6C
    char pad_0x70[0x2300];
    char pad_0x2370[0x90];                          // scene view
    CSWGuiLabel m_lblPortrait;                      // 0x2400
    char pad_0x2540[4];
    int m_nInputClass;                              // 0x2544
    CExoString m_sClassName;                        // 0x2548
    CExoString m_sName;                             // 0x2550
    char pad_0x2558[4];
    unsigned long m_oidCreature;                   // 0x255C

    void ClearPowers();
    void ClearFeats();
    void CancelLevelUp();
    int CopyCreatureStats(CSWCCreature* pFrom, CSWCCreature* pTo); // 0x006E89F0
};

#endif
