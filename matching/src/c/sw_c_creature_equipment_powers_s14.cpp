// CSWCCreature::GetEquipmentForcePowers (nwccreature.o, worker C s14) with a
// translation-unit-local layout: the force powers granted by equipped items,
// best level per power group, appended to the radial action list.

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    char* m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    char m_resRef[16];
    int operator==(const char* s) const;            // 0x004060D0
    CResRef operator=(const CResRef& r);            // 0x00406120
};

class CSWCCreature;

#pragma pack(push, 4)
class CSWGuiInterfaceAction {
public:
    ~CSWGuiInterfaceAction();                       // 0x0059EA30

    CExoString m_sName;                             // 0x00
    unsigned long m_nActionId;                      // 0x08
    void (CSWCCreature::*m_pfnAction)(unsigned long oidTarget, CSWCCreature* pCreature);  // 0x0c
    unsigned long m_oidTarget;                      // 0x1c
    CResRef m_resIcon;                              // 0x20
    unsigned long m_bEnabled : 1;                   // 0x30 bit 0 (descriptive)
    unsigned long m_nDisabledReason : 4;            // 0x30 bits 1-4 (descriptive)
    unsigned long m_nFlagsUnused : 27;
    unsigned long m_n34;                            // 0x34
};
#pragma pack(pop)

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    CExoArrayList()
    {
        num = 0;
        array_size = 0;
        element = 0;
    }
    ~CExoArrayList()
    {
        delete[] element;
    }
    T& operator[](int i) { return element[i]; }
    void Allocate(int n);
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num++] = t;
    }
    void Insert(T t, int pos);                      // out of line (0x00616940, 0x006169A0)
    void SetSize(int n)
    {
        if (n == 0) {
            if (element) {
                delete[] element;
                element = 0;
                array_size = 0;
            }
        } else {
            Allocate(n);
        }
        num = n;
    }
};

// Inlined for the scalar lists; out of line for the action list (0x004F7920).
template <class T>
void CExoArrayList<T>::Allocate(int n)
{
    T* pOld = element;
    array_size = n;
    element = new T[n];
    T* pDst = element;
    T* pSrc = pOld;
    for (int i = num; i > 0; i--) {
        *pDst++ = *pSrc++;
    }
    if (pOld) {
        delete[] pOld;
    }
}

typedef CExoArrayList<CSWGuiInterfaceAction> CSWGuiActionList;

struct CSWItemProperty {
    char pad_0x0[0x6];
    unsigned short m_nPropertyName;                 // 0x06 (13 and 7: uses not limited)
};

class CSWSItem {
public:
    CSWItemProperty* GetActiveProperty(int n);      // 0x00553960
    unsigned short GetUsedActivePropertyUsesLeft(unsigned char nIndex);   // 0x00555A60
};

class CSWCItem {
public:
    int GetPropertyByType(int nType);               // 0x006467E0
    CSWSItem* GetServerItem();                      // 0x00646C90

    char pad_0x0[0x14];
    unsigned long m_idSelf;                         // 0x14
    char pad_0x18[0x138 - 0x18];
    CResRef m_resIcon;                              // 0x138
};

class CClientExoApp {
public:
    CSWCItem* GetItemByGameObjectID(unsigned long oid);   // 0x005ED5A0
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWSpell {
public:
    int GetSubSpell(unsigned char nIndex);          // 0x0059B6A0
    CExoString GetSpellNameText();                  // 0x0059B940

    char pad_0x0[0x10];
    CResRef m_resIcon;                              // 0x10
    char pad_0x20[0x130 - 0x20];
    unsigned char m_nSubSpellCount;                 // 0x130
    char pad_0x131[0x188 - 0x131];
    unsigned long m_nTargetMask;                    // 0x188 (descriptive)
    unsigned long m_nForbiddenItemMask;             // 0x18c (descriptive)
    unsigned long m_nRequiredItemMask;              // 0x190 (descriptive)
};

class CSWSpellArray {
public:
    CSWSpell* GetSpell(int nSpell);                 // 0x0059B6D0
};

class CSWRules {
public:
    char pad_0x0[0x8c];
    CSWSpellArray* m_pSpellArray;                   // 0x8c
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CSWCCreature {
public:
    void GetEquipmentForcePowers(CSWGuiActionList& lstActions, unsigned long nType, int nTargetType,
                                 unsigned long nItemMask);

    void* m_vtable;                                 // 0x00
    char pad_0x4[0x220 - 0x4];
    unsigned long m_aEquippedItems[15];             // 0x220 (by slot bit)
};

// Sub-spell rows encode type * 1000 + group * 10 + level.
// NONMATCH (187 insns): same statements and EH states, but the original
// lowers the level/group remainders as imul -10/-100 plus add and copies in
// Allocate with a source-minus-destination pointer loop; ours uses lea/shl
// and two pointers, so registers and slots differ from there on.
// FUNCTION: SWKOTOR 0x00618C20
void CSWCCreature::GetEquipmentForcePowers(CSWGuiActionList& lstActions, unsigned long nType, int nTargetType,
                                           unsigned long nItemMask)
{
    CExoArrayList<CSWCItem*> lstItems;
    CExoArrayList<unsigned short> lstLevels;
    CExoArrayList<unsigned short> lstGroups;
    int i;
    for (i = 0; i < 15; i++) {
        CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_aEquippedItems[i]);
        if (!pItem) {
            continue;
        }
        CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpell(pItem->GetPropertyByType(0));
        if (!pSpell || (pSpell->m_nTargetMask & nTargetType)) {
            continue;
        }
        int nCount = pSpell->m_nSubSpellCount;
        for (int j = 0; j < nCount; j++) {
            short nSubSpell = (short)pSpell->GetSubSpell(j);
            int nQuot = nSubSpell / 10;
            int nLevel = nSubSpell - nQuot * 10;
            short nKind = nQuot / 100;
            int nGroup = nQuot - nKind * 100;
            if (nQuot / 100 != nType) {
                continue;
            }
            int bFound = 0;
            for (int k = 0; k < lstItems.num; k++) {
                if (bFound) {
                    break;
                }
                if ((short)lstGroups[k] == nGroup) {
                    bFound = 1;
                    if ((short)lstLevels[k] < nLevel) {
                        lstLevels[k] = nLevel;
                        lstItems[k] = pItem;
                    }
                } else if ((short)lstGroups[k] > nGroup) {
                    bFound = 1;
                    lstLevels.Insert(nLevel, k);
                    lstGroups.Insert(nGroup, k);
                    lstItems.Insert(pItem, k);
                }
            }
            if (!bFound) {
                lstItems.Add(pItem);
                lstLevels.Add(nLevel);
                lstGroups.Add(nGroup);
            }
        }
    }

    int nIndex = lstActions.num;
    for (i = 0; i < lstItems.num; i++) {
        CSWCItem* pItem = lstItems[i];
        CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpell(pItem->GetPropertyByType(0));
        lstActions.SetSize(nIndex + 1);
        lstActions[nIndex].m_sName = pSpell->GetSpellNameText();
        lstActions[nIndex].m_nActionId = pItem->m_idSelf | 0x20000000;
        if (pSpell->m_resIcon == "") {
            lstActions[nIndex].m_resIcon = pItem->m_resIcon;
        } else {
            lstActions[nIndex].m_resIcon = pSpell->m_resIcon;
        }
        lstActions[nIndex].m_nDisabledReason = 0;
        CSWSItem* pServerItem = pItem->GetServerItem();
        CSWItemProperty* pProperty = 0;
        if (pServerItem) {
            pProperty = pServerItem->GetActiveProperty(0);
        }
        int bEnabled = 1;
        if (pServerItem && pProperty && pServerItem->GetUsedActivePropertyUsesLeft(0) < 1 &&
            pProperty->m_nPropertyName != 13 && pProperty->m_nPropertyName != 7) {
            lstActions[nIndex].m_nDisabledReason = 1;
            bEnabled = 0;
        }
        if (pSpell->m_nForbiddenItemMask & nItemMask) {
            lstActions[nIndex].m_nDisabledReason = 2;
            bEnabled = 0;
        }
        if ((pSpell->m_nRequiredItemMask & nItemMask) != pSpell->m_nRequiredItemMask) {
            lstActions[nIndex].m_nDisabledReason = 3;
            bEnabled = 0;
        }
        lstActions[nIndex].m_bEnabled = bEnabled;
        nIndex++;
    }
}
