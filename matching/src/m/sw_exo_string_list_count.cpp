// CExoStringList count accessor.

class CExoStringList {
public:
    int GetCount();

    char m_pad_00[4];
    int m_nCount;                                     // +0x04
};

// FUNCTION: SWKOTOR 0x00675DD0
int CExoStringList::GetCount()
{
    return m_nCount;
}
