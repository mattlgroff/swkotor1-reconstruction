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

#pragma pack(push, 4)
class CGameEffect {
public:
    CGameEffect(int bCreateNewID);                          // 0x00503E40
    ~CGameEffect();                                         // 0x00503590
    void SetInteger(int nIndex, int nValue);                // 0x005036A0
    void SetFloat(int nIndex, float fValue);                // 0x005036D0
    void SetObjectID(int nIndex, unsigned long oid);        // 0x005036F0
    void SetCreator(unsigned long oid);                     // 0x00503A00

    unsigned short GetDurationType() { return m_nDurationType; }
    void SetSubType_Supernatural() { m_nSubType = (m_nSubType & 0xffe7) | 0x8; }

    unsigned __int64 m_nID;                                 // 0x00
    unsigned short m_nType;                                 // 0x08
    union {
        unsigned short m_nSubType;                          // 0x0a
        struct {
            unsigned short m_nDurationType : 3;             // 0x0a (low 3 bits)
            unsigned short m_nSubTypeHigh : 13;
        };
    };
    char pad_0xc[0x20 - 0xc];
    int m_n20;                                              // 0x20 (name guessed)
    char pad_0x24[0x8c - 0x24];
};
#pragma pack(pop)

class CScriptEvent {
public:
    CScriptEvent();                                         // 0x004D7540
    ~CScriptEvent();                                        // 0x004D7590
    void SetInteger(int nIndex, int nValue);                // 0x004D76B0
    void SetFloat(int nIndex, float fValue);                // 0x004D7720
    void SetObjectID(int nIndex, unsigned long oid);        // 0x004D7780

    unsigned short m_nType;                                 // 0x00
    char pad_0x2[0x34 - 0x2];
};

// 0x004CA960: accepts sixteen beam visual-effect ids (name guessed).
int __cdecl IsValidBeamVisualEffect(int nVisualEffect);

#endif
