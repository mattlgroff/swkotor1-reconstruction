// Mark journal entries viewed when the panel closes.
class CSWCJournalEntry {
public:
    char pad_0x00[0x2c];
    unsigned long m_nFlags;
};
template <class T> class CExoArrayList {
public:
    T* element; int num; int array_size;
    void Clear() {
        if (element) { delete[] element; element = 0; array_size = 0; }
        num = 0;
    }
};
class CSWGuiInGameJournal {
public:
    virtual ~CSWGuiInGameJournal();
    virtual void Slot01();
    virtual void Slot02();
    virtual void Slot03();
    virtual void Slot04();
    virtual void Slot05();
    virtual void Slot06();
    virtual void Slot07();
    virtual void Slot08();
    virtual void Slot09();
    virtual void Slot10();
    virtual void Slot11();
    virtual void Slot12();
    virtual void Slot13();
    virtual void Slot14();
    virtual void Slot15();
    virtual void Slot16();
    virtual void Slot17();
    virtual void Slot18();
    virtual void Slot19();
    virtual void Slot20();
    virtual void Slot21();
    virtual void Slot22();
    virtual void Slot23();
    virtual void Slot24();
    virtual void Slot25();
    virtual void Slot26();
    virtual CSWCJournalEntry* GetQuestEntryAt(int index); // vtable +0x6c
    char pad_0x04[0xfb8];
    CExoArrayList<unsigned long> m_lstViewed; // +0xfbc
    void MarkNewQuestsObserved();
};

// FUNCTION: SWKOTOR 0x00645610
void CSWGuiInGameJournal::MarkNewQuestsObserved()
{
    for (int i = 0; i < m_lstViewed.num; ++i) {
        CSWCJournalEntry* entry = GetQuestEntryAt(m_lstViewed.element[i]);
        if (entry) entry->m_nFlags &= ~4;
    }
    m_lstViewed.Clear();
}
