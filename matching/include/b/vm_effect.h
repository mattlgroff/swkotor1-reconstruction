// Worker B matching header: CGameEffect / CScriptEvent as used by the
// CSWVirtualMachineCommands effect and event commands (kept apart from
// vm_commands.h so VM sources can also include sws_creature.h / sws_item.h).
#ifndef B_VM_EFFECT_H
#define B_VM_EFFECT_H

#include "b/vm_commands.h"

// .rdata effect type words (loaded from memory, not immediates)
#include "shared/globals.h"

#include "b/game_effect.h"

class CScriptEvent {
public:
    CScriptEvent();                                         // 0x004D7540
    ~CScriptEvent();                                        // 0x004D7590
    void SetInteger(int nIndex, int nValue);                // 0x004D76B0
    void SetFloat(int nIndex, float fValue);                // 0x004D7720
    void SetObjectID(int nIndex, unsigned long oid);        // 0x004D7780
    int operator==(const CScriptEvent& other);              // 0x004D7270
    int SaveEvent(CResGFF* pRes, CResStruct* pStruct);      // 0x004D73A0
    void CopyScriptEvent(CScriptEvent* pOther);             // 0x004D77F0
    int LoadEvent(CResGFF* pRes, CResStruct* pStruct);      // 0x004D79B0

    unsigned short m_nType;                                 // 0x00
    char pad_0x2[0x34 - 0x2];
};

// 0x004CA960: accepts sixteen beam visual-effect ids (name guessed).
int __cdecl IsValidBeamVisualEffect(int nVisualEffect);

#endif
