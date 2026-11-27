// sws_action_record.h -- CSWSActionRecord
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/f/luo.cpp); the other copies' members fall on its members.
#ifndef SHARED_SWS_ACTION_RECORD_H
#define SHARED_SWS_ACTION_RECORD_H

#include "shared/vector.h"

class CSWSActionRecord {
public:
    CSWSActionRecord();
    ~CSWSActionRecord();
    void Reset()
    {
        m_nAction = 0xffff;
        m_nParam2 = 0xffff;
        m_nParam4 = 0xffff;
        m_vTarget = Vector(0.0f, 0.0f, 0.0f);
        m_oidTarget = 0x7f000000;
        m_nGroup = -1;
    }
    unsigned short m_nAction;        // +0x0
    unsigned short m_nParam2;        // +0x2
    unsigned short m_nParam4;        // +0x4
    char pad_0x6[0x2];
    int m_nGroup;                    // +0x8
    Vector m_vTarget;                // +0xc
    unsigned long m_oidTarget;       // +0x18
};

#endif
