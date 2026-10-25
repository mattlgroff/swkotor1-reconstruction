// CSWCItem property accessors and compact field operations.

extern "C" void __cdecl free(void* p);
void* __cdecl operator new(unsigned int nSize);

class CSWCCreature;

class CClientExoApp {
public:
    CSWCCreature* GetCreatureByGameObjectID(unsigned long oid);
};

class CAppManager {
public:
    char pad_0x0[4];
    CClientExoApp* m_pClientExoApp;
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;

class CSWBaseItemData;
class CSWBaseItemArray {
public:
    CSWBaseItemData* GetBaseItem(int nBaseItem);    // 0x005B31A0
};

class CSWRules {
public:
    char pad_0x0[0x34];
    CSWBaseItemArray* m_pBaseItemArray;
};

class CSWCItemProperty {
public:
    unsigned short m_nPropertyType;
    unsigned short m_nSubtype;
    char pad_0x4[0x18];
};

class CSWCItem {
public:
    CSWCCreature* GetEquippedBy();
    void SetNumActiveProperties(unsigned long nCount);
    CSWCItemProperty* GetActiveProperty(int nIndex);
    int GetPropertyByType(int nIndex);
    CSWBaseItemData* SetStackSize(unsigned long nSize);
    void SetItemPropertyUsesLeft(int nIndex, unsigned short nUses);

    char pad_0x0[0xc];
    unsigned long m_nBaseItem;
    char pad_0x10[0x110];
    volatile unsigned long m_nNumActiveProperties;
    CSWCItemProperty* m_pActiveProperties;
    char pad_0x128[0x38];
    unsigned long m_oidEquippedBy;
    char pad_0x164[8];
    unsigned short m_nPropertyUses[8];
    unsigned char m_nActivePropertyIndex;
    char pad_0x17d[7];
    unsigned long m_nStackSize;
};

// FUNCTION: SWKOTOR 0x00646630
CSWCCreature* CSWCItem::GetEquippedBy()
{
    if (m_oidEquippedBy == 0x7f000000) {
        return 0;
    }
    return g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_oidEquippedBy);
}

// FUNCTION: SWKOTOR 0x00646660
void CSWCItem::SetNumActiveProperties(unsigned long nCount)
{
    m_nNumActiveProperties = nCount;
    if (m_pActiveProperties) {
        free(m_pActiveProperties);
    }
    m_pActiveProperties = (CSWCItemProperty*)::operator new(m_nNumActiveProperties * 0x1c);
}

// FUNCTION: SWKOTOR 0x006466A0
CSWCItemProperty* CSWCItem::GetActiveProperty(int nIndex)
{
    if (nIndex < 0 || nIndex >= (int)m_nNumActiveProperties) {
        return 0;
    }
    return &m_pActiveProperties[nIndex];
}

// FUNCTION: SWKOTOR 0x006467E0
int CSWCItem::GetPropertyByType(int nIndex)
{
    if (nIndex >= (int)m_nNumActiveProperties) {
        return -1;
    }
    if (m_pActiveProperties[nIndex].m_nPropertyType != 10) {
        return -1;
    }
    return m_pActiveProperties[nIndex].m_nSubtype;
}

// NONMATCH (1 diff insn): same tail call and operands, but MSVC schedules the
// rules global load after spilling the base-item argument.
// FUNCTION: SWKOTOR 0x00646810
CSWBaseItemData* CSWCItem::SetStackSize(unsigned long nSize)
{
    m_nStackSize = nSize;
    return (*(CSWRules**)0x007a3a28)->m_pBaseItemArray->GetBaseItem((int)m_nBaseItem);
}

// NONMATCH (10 diff insns): loop values/semantics agree, but the original
// keeps the byte offset in EDX and lays out its skip branches differently.
// FUNCTION: SWKOTOR 0x006469B0
void CSWCItem::SetItemPropertyUsesLeft(int nIndex, unsigned short nUses)
{
    if (nIndex >= 8) {
        return;
    }
    m_nPropertyUses[nIndex] = nUses;
    if (nIndex != m_nActivePropertyIndex || nUses != 0) {
        return;
    }

    int i = 0;
    unsigned long propertyOffset = 0;
    unsigned short* pUses = m_nPropertyUses;
    for (;;) {
        if ((int)propertyOffset >= 0 && i < (int)m_nNumActiveProperties) {
            CSWCItemProperty* pProperty = (CSWCItemProperty*)((char*)m_pActiveProperties + propertyOffset);
            if (pProperty && pProperty->m_nPropertyType == 10 && *pUses > 0) {
                m_nActivePropertyIndex = (unsigned char)i;
                break;
            }
        }
        propertyOffset += 0x1c;
        ++i;
        ++pUses;
        if ((int)propertyOffset >= 0xe0) {
            break;
        }
    }
    if (i == 8) {
        m_nActivePropertyIndex = 0xff;
    }
}
