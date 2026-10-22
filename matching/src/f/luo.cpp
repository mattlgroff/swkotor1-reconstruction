// player last-update-object helpers
// (CSWSPlayerLUOInventory, CSWSPlayerLastUpdateObject spell lists,
// CSWCreatureStatsUpdate, CSWLevelStats; 0x005CA030-0x005CB345).

extern "C" void *memset(void *pDest, int nValue, unsigned int nCount);

struct CExoLinkedListNode;

class CExoLinkedListInternal {
public:
    CExoLinkedListInternal();
    ~CExoLinkedListInternal();
    void *GetAtPos(CExoLinkedListNode *pos);
    void *GetPrev(CExoLinkedListNode *&pos);
    void *Remove(CExoLinkedListNode *pos);
    void *RemoveHead();
    CExoLinkedListNode *AddHead(void *pData);
    CExoLinkedListNode *pHead;
    CExoLinkedListNode *pTail;
    unsigned long nCount;
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedList();
    ~CExoLinkedList() { delete m_pcExoLinkedListInternal; }
    int IsEmpty() { return m_pcExoLinkedListInternal->pHead == 0; }
    CExoLinkedListNode *GetTailPos() { return m_pcExoLinkedListInternal->pTail; }
    T *GetAtPos(CExoLinkedListNode *pos) { return (T *)m_pcExoLinkedListInternal->GetAtPos(pos); }
    T *GetPrev(CExoLinkedListNode *&pos) { return (T *)m_pcExoLinkedListInternal->GetPrev(pos); }
    T *Remove(CExoLinkedListNode *pos) { return (T *)m_pcExoLinkedListInternal->Remove(pos); }
    T *RemoveHead() { return (T *)m_pcExoLinkedListInternal->RemoveHead(); }
    CExoLinkedListNode *AddHead(T *pData) { return m_pcExoLinkedListInternal->AddHead(pData); }
    CExoLinkedListInternal *m_pcExoLinkedListInternal;
};

template <class T>
CExoLinkedList<T>::CExoLinkedList()
{
    m_pcExoLinkedListInternal = new CExoLinkedListInternal();
}

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
    void Allocate(int n);
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num++] = t;
    }
    void DelIndex(int nIndex)
    {
        num--;
        for (; nIndex < num; nIndex++)
            element[nIndex] = element[nIndex + 1];
    }
    int Contains(T t)
    {
        int nCount = 0;
        for (int i = 0; i < num; i++) {
            if (element[i] == t)
                nCount++;
        }
        return nCount;
    }
    void Clear()
    {
        if (element) {
            delete[] element;
            element = 0;
            array_size = 0;
        }
        num = 0;
    }
};

extern const unsigned long g_oidInvalid;   // 0x00746644

// ---------------------------------------------------------------------------

class CExoString {
public:
    CExoString();
    ~CExoString();
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef();
    char m_resRef[16];
};

class Vector {
public:
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    float x, y, z;
};

struct CLastUpdateEffect;
struct CLastUpdateAction;

template <class T>
class CExoLinkedListOOL {
public:
    CExoLinkedListOOL();
    ~CExoLinkedListOOL();
    CExoLinkedListInternal *m_pcExoLinkedListInternal;
};

class CLastUpdateObject {
public:
    CLastUpdateObject();
    ~CLastUpdateObject();

    char pad_0x0[0x2a];
    unsigned short m_n2a;                               // +0x2a
    char pad_0x2c[0x4];
    Vector m_vPosition;                                 // +0x30
    char pad_0x3c[0x4];
    Vector m_vOrientation;                              // +0x40
    unsigned long m_n4c;                                // +0x4c
    float m_f50;                                        // +0x50
    Vector m_v54;                                       // +0x54
    char pad_0x60[0x1c];
    CResRef m_cResRef;                                  // +0x7c
    char pad_0x8c[0x4];
    CExoLinkedListOOL<CLastUpdateEffect> m_lEffects;    // +0x90
    char pad_0x94[0x4];
    CExoLinkedListOOL<CLastUpdateAction> m_lActions;    // +0x98
    char pad_0x9c[0xc];
    CExoArrayList<unsigned long> m_aA8;                 // +0xa8
    char pad_0xb4[0x20];
    CExoArrayList<unsigned long> m_aD4;                 // +0xd4
    char pad_0xe0[0x28];
    CExoString m_sName;                                 // +0x108
    char pad_0x110[0x2];
    short m_n112[1];                                    // +0x112
    short m_n114[1];                                    // +0x114
};

// FUNCTION: SWKOTOR 0x005CAA50
CLastUpdateObject::CLastUpdateObject()
    : m_vPosition(0.0f, 0.0f, 0.0f), m_vOrientation(0.0f, 0.0f, 0.0f), m_v54(0.0f, 0.0f, 0.0f)
{
    m_n2a = 0;
    m_f50 = 1.0f;
    m_n4c = 0x2711;
    memset(m_n112, 0xff, sizeof(m_n112));
    memset(m_n114, 0xff, sizeof(m_n114));
}

// FUNCTION: SWKOTOR 0x005CA6A0
CLastUpdateObject::~CLastUpdateObject()
{
}

// ---------------------------------------------------------------------------

struct CSWSPlayerLUOItem {
    unsigned long m_oidItem;
    unsigned long m_n4;
    unsigned long m_n8;
};

class CSWSPlayerLUOInventory {
public:
    CSWSPlayerLUOInventory();
    ~CSWSPlayerLUOInventory();
    void ClearSlots();
    void ClearRepository()
    {
        while (!m_lRepository.IsEmpty())
            delete m_lRepository.RemoveHead();
    }
    void ClearContainer()
    {
        while (!m_lContainer.IsEmpty())
            delete m_lContainer.RemoveHead();
    }
    void ClearOther()
    {
        while (!m_lOther.IsEmpty())
            delete m_lOther.RemoveHead();
    }
    CSWSPlayerLUOItem *ItemListGetUpdateItem(CExoLinkedListNode *pos, unsigned char nList);
    void ItemListGetTail(CExoLinkedListNode **pPos, unsigned char nList);
    void ItemListGetPrev(CExoLinkedListNode *&pos, unsigned char nList);
    void ItemListRemove(CExoLinkedListNode *pos, unsigned char nList);
    void ItemListAddHead(unsigned long oidItem, unsigned char nList, unsigned long n4, unsigned long n8);

    unsigned long m_oidSlots[18];                       // +0x0
    int m_nSlots[18];                                   // +0x48
    CExoLinkedList<CSWSPlayerLUOItem> m_lRepository;    // +0x90
    unsigned long m_oidContainer;                       // +0x94
    CExoLinkedList<CSWSPlayerLUOItem> m_lContainer;     // +0x98
    CExoLinkedList<CSWSPlayerLUOItem> m_lOther;         // +0x9c
};

// FUNCTION: SWKOTOR 0x005CA400 SYMBOL: ??0?$CExoLinkedList@UCSWSPlayerLUOItem@@@@QAE@XZ
template CExoLinkedList<CSWSPlayerLUOItem>::CExoLinkedList();

// FUNCTION: SWKOTOR 0x005CA030
void CSWSPlayerLUOInventory::ClearSlots()
{
    for (int i = 0; i < 18; i++) {
        m_oidSlots[i] = g_oidInvalid;
        m_nSlots[i] = -1;
    }
}

// FUNCTION: SWKOTOR 0x005CA4D0
CSWSPlayerLUOItem *CSWSPlayerLUOInventory::ItemListGetUpdateItem(CExoLinkedListNode *pos,
                                                                unsigned char nList)
{
    CSWSPlayerLUOItem *pItem = 0;
    if (nList == 0)
        pItem = m_lContainer.GetAtPos(pos);
    else if (nList == 1)
        pItem = m_lRepository.GetAtPos(pos);
    else if (nList == 2)
        pItem = m_lOther.GetAtPos(pos);
    return pItem;
}

// FUNCTION: SWKOTOR 0x005CA520
void CSWSPlayerLUOInventory::ItemListGetTail(CExoLinkedListNode **pPos, unsigned char nList)
{
    if (nList == 0)
        *pPos = m_lContainer.GetTailPos();
    else if (nList == 1)
        *pPos = m_lRepository.GetTailPos();
    else if (nList == 2)
        *pPos = m_lOther.GetTailPos();
}

// FUNCTION: SWKOTOR 0x005CA570
void CSWSPlayerLUOInventory::ItemListGetPrev(CExoLinkedListNode *&pos, unsigned char nList)
{
    if (nList == 0)
        m_lContainer.GetPrev(pos);
    else if (nList == 1)
        m_lRepository.GetPrev(pos);
    else if (nList == 2)
        m_lOther.GetPrev(pos);
}

// FUNCTION: SWKOTOR 0x005CA5C0
void CSWSPlayerLUOInventory::ItemListRemove(CExoLinkedListNode *pos, unsigned char nList)
{
    if (nList == 0)
        delete m_lContainer.Remove(pos);
    else if (nList == 1)
        delete m_lRepository.Remove(pos);
    else if (nList == 2)
        delete m_lOther.Remove(pos);
}

// FUNCTION: SWKOTOR 0x005CA630
void CSWSPlayerLUOInventory::ItemListAddHead(unsigned long oidItem, unsigned char nList,
                                             unsigned long n4, unsigned long n8)
{
    CSWSPlayerLUOItem *pItem = new CSWSPlayerLUOItem;
    pItem->m_oidItem = oidItem;
    pItem->m_n4 = n4;
    pItem->m_n8 = n8;
    if (nList == 0)
        m_lContainer.AddHead(pItem);
    else if (nList == 1)
        m_lRepository.AddHead(pItem);
    else if (nList == 2)
        m_lOther.AddHead(pItem);
    else
        delete pItem;
}

// FUNCTION: SWKOTOR 0x005CA7F0
CSWSPlayerLUOInventory::CSWSPlayerLUOInventory()
{
    ClearSlots();
    ClearRepository();
    ClearContainer();
    ClearOther();
    m_oidContainer = g_oidInvalid;
}

// FUNCTION: SWKOTOR 0x005CA900
CSWSPlayerLUOInventory::~CSWSPlayerLUOInventory()
{
    ClearSlots();
    ClearRepository();
    ClearContainer();
    ClearOther();
    m_oidContainer = g_oidInvalid;
}

// ---------------------------------------------------------------------------

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

struct CSWSLUOBuffer {
    ~CSWSLUOBuffer() { delete[] m_pData; m_pData = 0; }
    unsigned char *m_pData;
};

class CSWSPlayerLastUpdateObject {
public:
    CSWSPlayerLastUpdateObject();
    ~CSWSPlayerLastUpdateObject();
    void ClearActionQueue();
    void ClearKnownSpellUsesLeft();
    unsigned char GetKnownSpellUsesLeft(unsigned char nClass, unsigned char nLevel);
    void SetKnownSpellUsesLeft(unsigned char nClass, unsigned char nLevel, unsigned char nUses);
    int GetKnownSpell(unsigned char nClass, unsigned char nLevel, unsigned char nIndex);
    void ClearVisibilityList();
    void ClearKnownSpells();
    void ClearFeats();
    void AddKnownSpell(unsigned char nClass, unsigned char nLevel, unsigned long nSpell);
    void ClearSpellAddDeleteLists();

    char pad_0x0[0x8];
    int m_n8;                                           // +0x8
    char pad_0xc[0xc];
    CExoArrayList<unsigned short> m_aFeats;             // +0x18
    CExoArrayList<unsigned short> m_aBonusFeats;        // +0x24
    CExoArrayList<unsigned short *> m_aFeatUses;        // +0x30
    char pad_0x3c[0x4];
    CSWSActionRecord m_pActionQueue[10];                // +0x40
    CExoArrayList<void *> m_aVisibility;                // +0x158
    CExoArrayList<unsigned long> m_aKnownSpells[2];     // +0x164
    CExoArrayList<unsigned long> m_aAddSpells[2];       // +0x17c
    CExoArrayList<unsigned long> m_aDelSpells[2];       // +0x194
    unsigned char m_nSpellUsesLeft[2][1];               // +0x1ac
    char pad_0x1ae[0x2];
    CSWSLUOBuffer *m_pBuffer;                           // +0x1b0
};

// FUNCTION: SWKOTOR 0x005CAC80
CSWSPlayerLastUpdateObject::CSWSPlayerLastUpdateObject()
{
    m_n8 = 0;
    m_pBuffer = 0;
}

// FUNCTION: SWKOTOR 0x005CAD70
CSWSPlayerLastUpdateObject::~CSWSPlayerLastUpdateObject()
{
    ClearKnownSpells();
    ClearVisibilityList();
    delete m_pBuffer;
    m_pBuffer = 0;
}

// FUNCTION: SWKOTOR 0x005CA0D0
void CSWSPlayerLastUpdateObject::ClearActionQueue()
{
    for (int i = 0; i < 10; i++)
        m_pActionQueue[i].Reset();
}

// FUNCTION: SWKOTOR 0x005CA060
void CSWSPlayerLastUpdateObject::ClearKnownSpellUsesLeft()
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 1; j++)
            m_nSpellUsesLeft[i][j] = 0;
}

// FUNCTION: SWKOTOR 0x005CA070
unsigned char CSWSPlayerLastUpdateObject::GetKnownSpellUsesLeft(unsigned char nClass,
                                                                unsigned char nLevel)
{
    if (nClass < 2 && nLevel < 1)
        return m_nSpellUsesLeft[nClass][nLevel];
    return 0;
}

// FUNCTION: SWKOTOR 0x005CA0A0
void CSWSPlayerLastUpdateObject::SetKnownSpellUsesLeft(unsigned char nClass, unsigned char nLevel,
                                                       unsigned char nUses)
{
    if (nClass < 2 && nLevel < 1)
        m_nSpellUsesLeft[nClass][nLevel] = nUses;
}

// FUNCTION: SWKOTOR 0x005CA740
int CSWSPlayerLastUpdateObject::GetKnownSpell(unsigned char nClass, unsigned char nLevel,
                                              unsigned char nIndex)
{
    int nList = nClass + nLevel;
    if (nIndex >= m_aKnownSpells[nList].num)
        return -1;
    return m_aKnownSpells[nList].element[nIndex];
}

// FUNCTION: SWKOTOR 0x005CA780
void CSWSPlayerLastUpdateObject::ClearVisibilityList()
{
    if (m_aVisibility.num > 0) {
        for (int i = m_aVisibility.num - 1; i >= 0; i--) {
            void *pEntry = m_aVisibility.element[i];
            m_aVisibility.DelIndex(i);
            if (pEntry)
                delete pEntry;
        }
    }
}

// FUNCTION: SWKOTOR 0x005CAB20
void CSWSPlayerLastUpdateObject::ClearKnownSpells()
{
    for (int i = 0; i < 2; i++)
        m_aKnownSpells[i].Clear();
}

// FUNCTION: SWKOTOR 0x005CAB60
void CSWSPlayerLastUpdateObject::ClearFeats()
{
    m_aFeats.Clear();
    m_aBonusFeats.Clear();
    for (int i = 0; i < m_aFeatUses.num; i++) {
        if (m_aFeatUses.element[i]) {
            delete m_aFeatUses.element[i];
            m_aFeatUses.element[i] = 0;
        }
    }
    m_aFeatUses.Clear();
}

// FUNCTION: SWKOTOR 0x005CAC30
void CSWSPlayerLastUpdateObject::ClearSpellAddDeleteLists()
{
    for (int i = 0; i < 2; i++) {
        m_aAddSpells[i].Clear();
        m_aDelSpells[i].Clear();
    }
}

// FUNCTION: SWKOTOR 0x005CABE0
void CSWSPlayerLastUpdateObject::AddKnownSpell(unsigned char nClass, unsigned char nLevel,
                                               unsigned long nSpell)
{
    m_aKnownSpells[nClass + nLevel].Add(nSpell);
}

// ---------------------------------------------------------------------------

class CSWRules {
public:
    char pad_0x0[0xab];
    unsigned char m_nNumSkills;                         // +0xab
};

extern CSWRules *g_pRules;

class CSWCombatInformation {
public:
    CSWCombatInformation();
    ~CSWCombatInformation();
    void SetCombatInformation(CSWCombatInformation *pInfo);
    char pad_0x0[0x48];
};

class CSWCreatureStatsUpdate {
public:
    CSWCreatureStatsUpdate();
    ~CSWCreatureStatsUpdate();
    void SetCombatInformation(CSWCombatInformation *pInfo);
    void ClearEffectIcons();

    CExoArrayList<unsigned long> m_aList0;              // +0x0
    CExoArrayList<unsigned long> m_aListC;              // +0xc
    char pad_0x18[0x8];
    CExoArrayList<void *> m_aEffectIcons;               // +0x20
    unsigned char m_nAbility2c[6];                        // +0x2c
    unsigned char m_nAbility32[6];                        // +0x32
    signed char m_nAbility38[6];                        // +0x38
    signed char m_n3e;                                  // +0x3e
    signed char m_n3f[3];                               // +0x3f
    short m_n42;                                        // +0x42
    short m_n44;                                        // +0x44
    char pad_0x46[0x2];
    int m_n48;                                          // +0x48
    int m_n4c;                                          // +0x4c
    char pad_0x50[0x2];
    short m_n52;                                        // +0x52
    CSWCombatInformation *m_pCombatInformation;         // +0x54
    signed char *m_pSkillRanks;                         // +0x58
};

// FUNCTION: SWKOTOR 0x005CAF10
CSWCreatureStatsUpdate::CSWCreatureStatsUpdate()
{
    m_n48 = -20;
    m_n42 = -20;
    m_n44 = -20;
    m_nAbility32[0] = 0xec;
    m_nAbility32[1] = 0xec;
    m_nAbility32[2] = 0xec;
    m_nAbility32[3] = 0xec;
    m_nAbility32[4] = 0xec;
    m_nAbility32[5] = 0xec;
    m_nAbility2c[0] = 0xec;
    m_nAbility2c[1] = 0xec;
    m_nAbility2c[2] = 0xec;
    m_nAbility2c[3] = 0xec;
    m_nAbility2c[4] = 0xec;
    m_nAbility2c[5] = 0xec;
    m_n3e = -20;
    m_n52 = -20;
    m_nAbility38[0] = -20;
    m_nAbility38[1] = -20;
    m_nAbility38[2] = -20;
    m_nAbility38[3] = -20;
    m_nAbility38[4] = -20;
    m_nAbility38[5] = -20;
    m_n3f[0] = -20;
    m_n3f[1] = -20;
    m_n3f[2] = -20;
    m_pSkillRanks = 0;
    m_n4c = 0;
    m_pCombatInformation = new CSWCombatInformation();
    if (g_pRules->m_nNumSkills) {
        m_pSkillRanks = new signed char[g_pRules->m_nNumSkills];
        memset(m_pSkillRanks, -20, g_pRules->m_nNumSkills);
    }
}

// FUNCTION: SWKOTOR 0x005CB040
CSWCreatureStatsUpdate::~CSWCreatureStatsUpdate()
{
    m_aList0.num = 0;
    m_aListC.num = 0;
    if (m_pSkillRanks)
        delete[] m_pSkillRanks;
    if (m_pCombatInformation) {
        delete m_pCombatInformation;
        m_pCombatInformation = 0;
    }
    ClearEffectIcons();
}

// FUNCTION: SWKOTOR 0x005CAEA0
void CSWCreatureStatsUpdate::SetCombatInformation(CSWCombatInformation *pInfo)
{
    m_pCombatInformation->SetCombatInformation(pInfo);
}

// FUNCTION: SWKOTOR 0x005CAEB0
void CSWCreatureStatsUpdate::ClearEffectIcons()
{
    for (int i = m_aEffectIcons.num - 1; i >= 0; i--) {
        if (m_aEffectIcons.element[i]) {
            delete m_aEffectIcons.element[i];
            m_aEffectIcons.element[i] = 0;
        }
        m_aEffectIcons.DelIndex(i);
    }
}

// ---------------------------------------------------------------------------

class CSWLevelStats {
public:
    CSWLevelStats();
    ~CSWLevelStats();
    unsigned char GetSkillRank(unsigned short nSkill);
    void SetSkillRank(unsigned short nSkill, unsigned char nRank);
    void AddFeat(unsigned short nFeat);

    CExoArrayList<unsigned long> m_pAddedKnownSpellList[1];   // +0x0
    CExoArrayList<unsigned long> m_pRemovedKnownSpellList[1]; // +0xc
    CExoArrayList<unsigned short> m_lstFeats;           // +0x18
    unsigned char *m_lstSkillRanks;                     // +0x24
    unsigned short m_nSkillPointsRemaining;             // +0x28
    unsigned char m_nHitDie;                            // +0x2a
    unsigned char m_nAbilityGain;                       // +0x2b
    unsigned char m_nClass;                             // +0x2c
    unsigned char m_bEpicLevel;                         // +0x2d
};

// FUNCTION: SWKOTOR 0x005CB1B0
CSWLevelStats::CSWLevelStats()
{
    m_lstSkillRanks = new unsigned char[g_pRules->m_nNumSkills];
    memset(m_lstSkillRanks, 0, g_pRules->m_nNumSkills);
    m_nAbilityGain = 0;
    m_nClass = 0;
    m_bEpicLevel = 0;
    m_nSkillPointsRemaining = 0;
    m_nHitDie = 6;
}

// FUNCTION: SWKOTOR 0x005CB270
CSWLevelStats::~CSWLevelStats()
{
    if (m_lstSkillRanks) {
        delete[] m_lstSkillRanks;
        m_lstSkillRanks = 0;
    }
    m_lstFeats.Clear();
    m_pAddedKnownSpellList[0].Clear();
    m_pRemovedKnownSpellList[0].Clear();
}

// FUNCTION: SWKOTOR 0x005CB0F0
unsigned char CSWLevelStats::GetSkillRank(unsigned short nSkill)
{
    if (nSkill >= g_pRules->m_nNumSkills)
        return 0;
    return m_lstSkillRanks[nSkill];
}

// FUNCTION: SWKOTOR 0x005CB120
void CSWLevelStats::SetSkillRank(unsigned short nSkill, unsigned char nRank)
{
    if (nSkill < g_pRules->m_nNumSkills)
        m_lstSkillRanks[nSkill] = nRank;
}

// FUNCTION: SWKOTOR 0x005CB150
void CSWLevelStats::AddFeat(unsigned short nFeat)
{
    if (!m_lstFeats.Contains(nFeat))
        m_lstFeats.Add(nFeat);
}
