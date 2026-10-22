// Pilot group 1: CExoResMan leaves.

class CResRef;
class CExoKeyTable;

struct CExoKeyEntry {
    char pad_0x0[0x1c];
    unsigned long m_nSize;  // 0x1c
};

class CExoResMan {
public:
    int GetKeyEntry(const CResRef& cResRef, unsigned short nType,
                    CExoKeyEntry** ppEntry, CExoKeyTable** ppTable);
    int Exists(const CResRef& cResRef, unsigned short nType, unsigned long* pSize);
};

// FUNCTION: SWKOTOR 0x00408BC0
int CExoResMan::Exists(const CResRef& cResRef, unsigned short nType, unsigned long* pSize)
{
    CExoKeyEntry* pEntry = 0;
    CExoKeyTable* pTable;
    int bFound = GetKeyEntry(cResRef, nType, &pEntry, &pTable);
    if (bFound && pSize) {
        *pSize = pEntry->m_nSize;
    }
    return bFound;
}
