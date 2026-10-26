// Worker B matching header: CGameEffect / CScriptEvent as used by the
// CSWVirtualMachineCommands effect and event commands (kept apart from
// vm_commands.h so VM sources can also include sws_creature.h / sws_item.h).
#ifndef B_VM_EFFECT_H
#define B_VM_EFFECT_H

#include "b/vm_commands.h"

// .rdata effect type words (loaded from memory, not immediates)
extern const unsigned short EFFECT_TYPE_INVALIDEFFECT;      // 0x0074667C (0)
extern const unsigned short EFFECT_TYPE_RESURRECTION;       // 0x0074668C (4)
extern const unsigned short EFFECT_TYPE_SETSTATE;           // 0x0074669C (8)
extern const unsigned short EFFECT_TYPE_DAMAGE_IMMUNITY_INCREASE; // 0x007466C0 (0x11)
extern const unsigned short EFFECT_TYPE_SAVING_THROW_INCREASE; // 0x007466E4 (0x1A)
extern const unsigned short EFFECT_TYPE_SAVING_THROW_DECREASE; // 0x007466E8 (0x1B)
extern const unsigned short EFFECT_TYPE_VISUALEFFECT;       // 0x007466F4 (0x1E)
extern const unsigned short EFFECT_TYPE_BEAM;               // 0x007466FC (0x20)
extern const unsigned short EFFECT_TYPE_FORCE_PUSHED;       // 0x0074676C (0x3C)
extern const unsigned short EFFECT_TYPE_TRUESEEING;         // 0x0074679C (0x48)
extern const unsigned short EFFECT_TYPE_BODY_FUEL;          // 0x00746800 (0x62)
extern const unsigned short EFFECT_TYPE_PSYCHIC_STATIC;     // 0x00746804 (0x63)
extern const unsigned short EFFECT_TYPE_FORCE_JUMP;         // 0x00746810 (0x66)
extern const int DAMAGE_TYPE_MAX;                           // 0x00746884 (0x2000)
extern const int DAMAGE_TYPE_DEFAULT;                       // 0x0074685C (8)

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
