// sws_creature_list_entry.h -- CSWSCreatureListEntry
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/a/sws_creature.h); the other copies' members fall on its members.
#ifndef SHARED_SWS_CREATURE_LIST_ENTRY_H
#define SHARED_SWS_CREATURE_LIST_ENTRY_H


struct CSWSCreatureListEntry {
    CSWSCreatureListEntry()
    {
        m_oid = 0x7f000000;
        m_b0 = 0;
        m_b1 = 0;
        m_n2 = 0;
        m_b4 = 0;
    }
    unsigned long m_oid;                            // 0x00
    union {
        unsigned long m_nFlags;                     // 0x04
        struct {
            unsigned char m_b0 : 1;                 // placeholder names
            unsigned char m_b1 : 1;
            unsigned char m_n2 : 2;
            unsigned char m_b4 : 1;
        };
    };
};

#endif
