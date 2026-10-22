// CSWInventory equipment slots (0x005A4930-0x005A4C20).

class CSWSItem {
public:
    char pad_0x0[0x14];
    unsigned long m_idSelf;          // +0x14
};

class CServerExoApp {
public:
    CSWSItem *GetItemByGameObjectID(unsigned long id);
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;

class CSWInventory {
public:
    CSWInventory();
    virtual ~CSWInventory() {}
    int GetItemInInventory(CSWSItem *pItem);
    unsigned long GetSlotFromItem(CSWSItem *pItem);
    unsigned long *GetItemSlotPtr(unsigned long nSlot);
    int RemoveItem(CSWSItem *pItem);
    void PutItemInSlot(unsigned long nSlot, CSWSItem *pItem);
    CSWSItem *GetItemInSlot(unsigned long nSlot);

    unsigned long m_aSlots[18];      // +0x4
};

// FUNCTION: SWKOTOR 0x005A4930
CSWInventory::CSWInventory()
{
    for (int i = 0; i < 18; i++)
        m_aSlots[i] = 0;
}

// FUNCTION: SWKOTOR 0x005A4950
int CSWInventory::GetItemInInventory(CSWSItem *pItem)
{
    unsigned long id = 0x7f000000;
    if (pItem)
        id = pItem->m_idSelf;
    for (int i = 0; i < 18; i++) {
        if (m_aSlots[i] == id)
            return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005A4980
unsigned long CSWInventory::GetSlotFromItem(CSWSItem *pItem)
{
    unsigned long id = 0x7f000000;
    if (pItem)
        id = pItem->m_idSelf;
    for (int i = 0; i < 18; i++) {
        if (m_aSlots[i] == id)
            return 1 << i;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005A49C0
unsigned long *CSWInventory::GetItemSlotPtr(unsigned long nSlot)
{
    switch (nSlot) {
    case 0x1:
        return &m_aSlots[0];
    case 0x2:
        return &m_aSlots[1];
    case 0x4:
        return &m_aSlots[2];
    case 0x8:
        return &m_aSlots[3];
    case 0x10:
        return &m_aSlots[4];
    case 0x20:
        return &m_aSlots[5];
    case 0x40:
        return &m_aSlots[6];
    case 0x80:
        return &m_aSlots[7];
    case 0x100:
        return &m_aSlots[8];
    case 0x200:
        return &m_aSlots[9];
    case 0x400:
        return &m_aSlots[10];
    case 0x800:
        return 0;
    case 0x1000:
        return 0;
    case 0x2000:
        return 0;
    case 0x4000:
        return &m_aSlots[11];
    case 0x8000:
        return &m_aSlots[12];
    case 0x10000:
        return &m_aSlots[13];
    case 0x20000:
        return &m_aSlots[14];
    default:
        return 0;
    }
}

// FUNCTION: SWKOTOR 0x005A4BA0
int CSWInventory::RemoveItem(CSWSItem *pItem)
{
    unsigned long id = 0x7f000000;
    if (pItem)
        id = pItem->m_idSelf;
    for (int i = 0; i < 18; i++) {
        if (m_aSlots[i] == id) {
            m_aSlots[i] = 0x7f000000;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005A4BE0
void CSWInventory::PutItemInSlot(unsigned long nSlot, CSWSItem *pItem)
{
    unsigned long *pSlot = GetItemSlotPtr(nSlot);
    if (pSlot)
        *pSlot = pItem->m_idSelf;
}

// FUNCTION: SWKOTOR 0x005A4C00 SYMBOL: ??_GCSWInventory@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x005A4C20
CSWSItem *CSWInventory::GetItemInSlot(unsigned long nSlot)
{
    unsigned long *pSlot = GetItemSlotPtr(nSlot);
    if (pSlot)
        return g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(*pSlot);
    return 0;
}
