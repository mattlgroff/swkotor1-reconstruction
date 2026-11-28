// sws_action_target.h -- CSWSActionTarget
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/a/game_effect.h); the other copies' members fall on its members.
#ifndef SHARED_SWS_ACTION_TARGET_H
#define SHARED_SWS_ACTION_TARGET_H


class CResGFF;
struct CResStruct;

class CSWSActionTarget {
public:
    CSWSActionTarget();                             // 0x005034D0
    int SaveData(CResGFF* pRes, CResStruct* pStruct);   // 0x005034F0

    unsigned long m_nActionId;                      // 0x00
    unsigned short m_nGroupId;                      // 0x04
    unsigned long m_oidArea;                        // 0x08
    float m_fX;                                     // 0x0c
    float m_fY;                                     // 0x10
    float m_fZ;                                     // 0x14
    unsigned long m_oidTarget;                      // 0x18
};

#endif
