// CSWSStore item trade (b005c_f2_journal_store): selling an item to a buyer
// and taking an item into the store inventory. Own CSWSItem / CSWSStore
// layout subset (store_waypoint.cpp keeps the lifetime members).

class CExoString {
public:
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CSWSItem;
class CSWSCreature;

class CGameObject {
public:
    virtual ~CGameObject();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual CSWSCreature *AsSWSCreature();   // +0x30
    virtual void v34();
    virtual CSWSItem *AsSWSItem();           // +0x38
    unsigned long m_idSelf;          // +0x4
};

class CSWSItem : public CGameObject {
public:
    CSWSItem(unsigned long oid);
    int CopyItem(CSWSItem *pItem);
    CSWSItem *SplitItem(int nCount);
    void SetItemFlag0(int bFlag);
    int CompareItem(CSWSItem *pItem);
    void SetPossessor(unsigned long oidPossessor, int a, int b, unsigned long oidPrevious);

    char pad_0x8[0x20];
    CExoString m_sTag;               // +0x28 (placeholder name)
    char pad_0x30[0xd8];
    int m_n108;                      // +0x108
    char pad_0x10c[0x15c];
    unsigned long m_oidPossessor;    // +0x268
    char pad_0x26c[0x1c];
    unsigned long m_nFlags;          // +0x288 (bit 2: infinite stock)
    int m_nStackSize;                // +0x28c
    char pad_0x290[0xc];
};

class CSWSCreature : public CGameObject {
public:
    int AcquireItem(CSWSItem **ppItem, unsigned long oidFrom, unsigned long oidContainer,
                    int bFlag);
    int RemoveItem(CSWSItem *pItem, int a, int b, int c);
};

class CItemRepository {
public:
    int RemoveItem(CSWSItem *pItem);
    unsigned long FindItemWithTag(const CExoString &sTag);
};

class CServerExoApp {
public:
    CGameObject *GetGameObject(unsigned long oid);
    CSWSItem *GetItemByGameObjectID(unsigned long oid);
    CSWSCreature *GetCreatureByGameObjectID(unsigned long oid);
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;

class CSWSStore : public CGameObject {
public:
    int SellItem(CSWSItem *pItem, CSWSCreature *pBuyer);
    int AcquireItem(CSWSItem *pItem);
    int AddItemToInventory(CSWSItem **ppItem);

    char pad_0x8[0x228];
    CItemRepository *m_pItemRepository; // +0x230
};

// NONMATCH (28): cl cross-jumps the three AcquireItem tails into one call
// (the original keeps three call/epilogue copies, pSplit in its own slot) and
// clears the infinite flag with a read-modify-write and.
// FUNCTION: SWKOTOR 0x005C6F70
int CSWSStore::SellItem(CSWSItem *pItem, CSWSCreature *pBuyer)
{
    if ((pItem->m_nFlags & 4)) {
        CSWSItem *pCopy = new CSWSItem(0x7f000000);
        pCopy->CopyItem(pItem);
        pCopy->m_nFlags = pCopy->m_nFlags & ~4;
        pCopy->m_nStackSize = 1;
        return pBuyer->AcquireItem(&pCopy, m_idSelf, 0x7f000000, 1);
    }
    if (pItem->m_nStackSize > 1) {
        CSWSItem *pSplit = pItem->SplitItem(1);
        return pBuyer->AcquireItem(&pSplit, m_idSelf, 0x7f000000, 1);
    }
    if (!m_pItemRepository->RemoveItem(pItem))
        return 0;
    return pBuyer->AcquireItem(&pItem, m_idSelf, 0x7f000000, 1);
}

// FUNCTION: SWKOTOR 0x005C7610
int CSWSStore::AcquireItem(CSWSItem *pItem)
{
    if (!pItem || pItem->m_n108)
        return 0;
    pItem->SetItemFlag0(1);
    unsigned long oidPossessor = pItem->m_oidPossessor;
    CGameObject *pPossessor = g_pAppManager->m_pServerExoApp->GetGameObject(oidPossessor);
    if (pPossessor) {
        if (pPossessor->AsSWSCreature()) {
            pPossessor->AsSWSCreature()->RemoveItem(pItem, 1, 1, 0);
        } else if (pPossessor->AsSWSItem()) {
            CSWSCreature *pCreature = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(
                pPossessor->AsSWSItem()->m_oidPossessor);
            if (pCreature)
                pCreature->RemoveItem(pItem, 1, 1, 0);
        }
    }
    CSWSItem *pFound = g_pAppManager->m_pServerExoApp->GetItemByGameObjectID(
        m_pItemRepository->FindItemWithTag(pItem->m_sTag));
    if (pFound && pFound->CompareItem(pItem)) {
        if (pFound->m_nFlags & 4) {
            delete pItem;
            return 1;
        }
        pFound->m_nStackSize++;
        delete pItem;
        return 1;
    }
    pItem->SetPossessor(m_idSelf, 1, 1, 0x7f000000);
    return AddItemToInventory(&pItem);
}

