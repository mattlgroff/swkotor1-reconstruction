// sws_dialog_node_data.h -- CSWSDialogNodeData
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/f/dialog_base.cpp); the other copies' members fall on its members.
#ifndef SHARED_SWS_DIALOG_NODE_DATA_H
#define SHARED_SWS_DIALOG_NODE_DATA_H

#include "shared/exo_string.h"

class CSWSDialogNodeData {
public:
    CSWSDialogNodeData()
    {
        m_n8 = 0;
        m_nC = 0;
        m_n10 = 0;
        m_n14 = 0;
        m_n18 = 0x7fff;
        m_f1C = -1.0f;
        m_n20 = -1;
    }
    CExoString m_sComment;           // +0x0
    unsigned long m_n8;              // +0x8
    unsigned long m_nC;              // +0xc
    unsigned long m_n10;             // +0x10
    unsigned long m_n14;             // +0x14
    unsigned short m_n18;            // +0x18
    char pad_0x1a[0x2];
    float m_f1C;                     // +0x1c
    int m_n20;                       // +0x20
};

#endif
