// object_table_manager.h -- CObjectTableManager
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/i/world_timer.h); the other copies' members fall on its members.
#ifndef SHARED_OBJECT_TABLE_MANAGER_H
#define SHARED_OBJECT_TABLE_MANAGER_H


struct CObjectTableEntry;

class CObjectTableManager {
public:
    CObjectTableManager();                          // 0x004AE5D0
    ~CObjectTableManager();                         // 0x004AE640
    void ClearAll();                                // 0x004AE5F0
    int RemovePlayer(unsigned long nPlayer);        // 0x004AE650

    CObjectTableEntry* m_pTables[0x60];             // +0x000
    unsigned long m_nTables;                        // +0x180
};

#endif
