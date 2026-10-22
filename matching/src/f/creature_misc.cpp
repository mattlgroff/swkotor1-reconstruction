// small CSWSCreature leaves in 0x005B4880-0x005B8050 (debilitated check,
// three-way range state at +0x558, appearance test, attacker list iteration), two
// empty stubs (0x005B5E90, 0x005C5630) and the record lookup at 0x005BE6D0.

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
};

class CSWSCreature {
public:
    int IsDebilitated();
    int GetDeadTemp();
    int GetIsRangeState0(int nValue);
    int GetIsRangeState1(int nValue);
    int GetIsRangeState2(int nValue);
    void SetRangeState(int nValue);
    int GetIsSpecialAppearance(unsigned short nAppearance);
    void Stub_005B5E90(int a, int b);
    unsigned long GetNextAttacker();
    void ClearAttackerList();

    char pad_0x0[0xd4];
    unsigned short m_nAppearance;    // +0xd4
    char pad_0xd6[0x558 - 0xd6];
    int m_nRangeState;               // +0x558
    char pad_0x55c[0x8ed - 0x55c];
    unsigned char m_bStunned;        // +0x8ed
    char pad_0x8ee[0xa80 - 0x8ee];
    CExoArrayList<unsigned long> *m_pAttackerList; // +0xa80
    int m_nAttackerListIndex;        // +0xa84
};

// FUNCTION: SWKOTOR 0x005B4880
int CSWSCreature::IsDebilitated()
{
    int bDebilitated = 0;
    if (m_bStunned || GetDeadTemp())
        bDebilitated = 1;
    return bDebilitated;
}

// FUNCTION: SWKOTOR 0x005B5510
int CSWSCreature::GetIsRangeState0(int nValue)
{
    int bResult = 0;
    if (nValue == -1) {
        if (m_nRangeState == 0)
            return 1;
    } else if (nValue > 0 && nValue <= 3) {
        bResult = 1;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x005B5540
int CSWSCreature::GetIsRangeState1(int nValue)
{
    int bResult = 0;
    if (nValue == -1) {
        if (m_nRangeState == 1)
            return 1;
    } else if (nValue >= 4 && nValue <= 6) {
        bResult = 1;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x005B5570
int CSWSCreature::GetIsRangeState2(int nValue)
{
    int bResult = 0;
    if (nValue == -1) {
        if (m_nRangeState == 2)
            return 1;
    } else if (nValue >= 8 && nValue <= 10) {
        bResult = 1;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x005B55A0
int CSWSCreature::GetIsSpecialAppearance(unsigned short nAppearance)
{
    if (nAppearance == -1)
        nAppearance = m_nAppearance;
    int bResult = 0;
    switch (nAppearance) {
    case 10015:
    case 10016:
    case 10017:
    case 10018:
    case 10019:
    case 10020:
    case 10061:
    case 10070:
    case 10071:
    case 10401:
    case 11000:
        bResult = 1;
        break;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x005B5E90
void CSWSCreature::Stub_005B5E90(int a, int b)
{
}

// FUNCTION: SWKOTOR 0x005B7400
void CSWSCreature::SetRangeState(int nValue)
{
    if ((nValue == -1 && m_nRangeState == 0) || (nValue > 0 && nValue <= 3))
        m_nRangeState = 0;
    else if ((nValue == -1 && m_nRangeState == 1) || (nValue >= 4 && nValue <= 6))
        m_nRangeState = 1;
    else if (nValue == -1) {
        if (m_nRangeState == 2)
            m_nRangeState = 2;
    } else if (nValue >= 8 && nValue <= 10)
        m_nRangeState = 2;
}

// FUNCTION: SWKOTOR 0x005B7FE0
unsigned long CSWSCreature::GetNextAttacker()
{
    unsigned long oidAttacker = 0x7f000000;
    if (m_pAttackerList && m_nAttackerListIndex < m_pAttackerList->num) {
        int nIndex = m_nAttackerListIndex;
        m_nAttackerListIndex = nIndex + 1;
        oidAttacker = m_pAttackerList->element[nIndex];
    }
    return oidAttacker;
}

// FUNCTION: SWKOTOR 0x005B8010
void CSWSCreature::ClearAttackerList()
{
    if (m_pAttackerList) {
        delete m_pAttackerList;
        m_pAttackerList = 0;
        m_nAttackerListIndex = 0;
    }
}

class CSWSJournalStub {
public:
    void Stub_005C5630(int a, int b, int c);
};

// FUNCTION: SWKOTOR 0x005C5630
void CSWSJournalStub::Stub_005C5630(int a, int b, int c)
{
}

struct SRecordEntry {
    unsigned short m_nId;            // +0x0
    char pad_0x2[0x2];
    int m_nPrimaryKey;               // +0x4
    int m_nSecondaryKey;             // +0x8
    unsigned char m_nState;          // +0xc
    char pad_0xd[0x3];
};

class CRecordTable {
public:
    int FindRecord(int bPrimary, int nKey);

    char pad_0x0[0x174];
    SRecordEntry *m_pRecords;        // +0x174
    unsigned short m_nRecords;       // +0x178
};

// NONMATCH (6): only the scratch register of the two `return m_pRecords[i].m_nId` paths
// differs (original edx / eax, ours eax / ecx); the separate primary-key `continue` block
// reproduces the rest.
// FUNCTION: SWKOTOR 0x005BE6D0
int CRecordTable::FindRecord(int bPrimary, int nKey)
{
    unsigned short nCount = m_nRecords;
    int nResult = -1;
    int nFree = 0;
    for (unsigned short i = 0; i < nCount; i++) {
        SRecordEntry *pEntry = &m_pRecords[i];
        if (pEntry->m_nState == 1 || pEntry->m_nState == 5) {
            int nValue;
            if (bPrimary) {
                nValue = pEntry->m_nPrimaryKey;
                if (nValue == nKey)
                    return m_pRecords[i].m_nId;
                if (nValue == 0 && nFree != nKey) {
                    nResult = pEntry->m_nId;
                    nFree++;
                }
                continue;
            } else {
                nValue = pEntry->m_nSecondaryKey;
                if (nValue == nKey)
                    return m_pRecords[i].m_nId;
            }
            if (nValue == 0 && nFree != nKey) {
                nResult = pEntry->m_nId;
                nFree++;
            }
        }
    }
    return nResult;
}
