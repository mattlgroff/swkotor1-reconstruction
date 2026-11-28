// swc_journal_list.h -- CSWCJournalList
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/m/sw_c_journal_accessors.cpp); the other copies' members fall on its members.
#ifndef SHARED_SWC_JOURNAL_LIST_H
#define SHARED_SWC_JOURNAL_LIST_H


struct CSWCJournalEntry;

class CSWCJournalList {
public:
    CSWCJournalEntry* element;
    int num;
    int array_size;
    unsigned long* m_pIndices;
    int m_nNumIndices;
    int m_nIndexCapacity;
    int m_nSort;
    int m_bChanged;

    void SortList();                                // 0x00676530
};

#endif
