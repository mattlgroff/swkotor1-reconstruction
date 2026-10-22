// small helpers in 0x005D0000-0x005DFFFF whose owner class is not
// known yet.

// Two-dword value copied member by member (0x005DCE30; callers 0x004B37F4,
// 0x004B8FD2, 0x004C8A24). The class name is a placeholder.
class CExoDwordPair {
public:
    void Copy(const CExoDwordPair& other);

    unsigned long m_n0;                             // 0x00
    unsigned long m_n4;                             // 0x04
};

// FUNCTION: SWKOTOR 0x005DCE30
void CExoDwordPair::Copy(const CExoDwordPair& other)
{
    m_n0 = other.m_n0;
    m_n4 = other.m_n4;
}
