// Action cancellation by group. Batch-local list wrapper exposes its genuine
// node links and payload; shared partial creature offsets remain unchanged.
class CSWSObjectActionNode {
public:
    ~CSWSObjectActionNode();                        // 0x004CC4D0
    unsigned long m_nActionId;
    char pad_4[0x6c - 4];
    unsigned short m_nGroupId;
};
struct CExoLinkedListNode {
    CExoLinkedListNode* next;
    CExoLinkedListNode* previous;
    void* data;
};
class CExoLinkedListInternal {
public:
    void* GetAtPos(void* pos);                      // 0x005E9B60
    void* GetNext(void*& pos);                      // 0x005E9B90
    void* Remove(void* pos);                        // 0x005E9C10
    CExoLinkedListNode* head;
};
template <class T> class CExoLinkedList {
public:
    T* GetHead() { return m_pInternal->head ? (T*)m_pInternal->head->data : 0; }
    void* GetHeadPosition() { return m_pInternal->head; }
    T* GetAtPos(void* pos) { return (T*)m_pInternal->GetAtPos(pos); }
    T* GetNext(void*& pos) { return (T*)m_pInternal->GetNext(pos); }
    T* Remove(void* pos) { return (T*)m_pInternal->Remove(pos); }
    CExoLinkedListInternal* m_pInternal;
};
class CSWSCreature {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual int RunAction(CSWSObjectActionNode* pNode, int bFirst); // 0x74
    int CancelAction(unsigned long nUnused, unsigned short nGroupId); // 0x004F1EF0 (descriptive reference name)
    char pad_4[0xfc - 4];
    CExoLinkedList<CSWSObjectActionNode> m_actions;
};

// NONMATCH: 29 diff: list-wrapper head/position coalescing and register tie; original unreachable null-wrapper branch reads its argument slot.
// FUNCTION: SWKOTOR 0x004F1EF0
int CSWSCreature::CancelAction(unsigned long, unsigned short nGroupId)
{
    CExoLinkedList<CSWSObjectActionNode>* pList = &m_actions;
    CSWSObjectActionNode* pAction = 0;
    void* pos = 0;
    if (pList) {
        pAction = pList->GetHead();
        pos = pList->GetHeadPosition();
    }
    int bResult = 0;
    while (pAction) {
        if (pAction->m_nGroupId == nGroupId) {
            void* current = pos;
            pAction = pList->GetNext(pos);
            CSWSObjectActionNode* pNode = pList->GetAtPos(current);
            if (RunAction(pNode, pNode == pList->GetHead()) == 1) {
                delete pList->Remove(current);
                bResult = 1;
            }
        } else {
            pAction = pList->GetNext(pos);
        }
    }
    return bResult;
}
