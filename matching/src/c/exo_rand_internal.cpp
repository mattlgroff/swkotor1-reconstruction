// CExoRandInternal destructor (b005e0_exo_foundation_leaves).

#include <string.h>

extern unsigned long g_nRandMask;                   // 0x007A2280

class CExoRandInternal {
public:
    unsigned long m_z[4];                           // 0x00
    unsigned long m_c[4];                           // 0x10
    unsigned long m_ks[4];                          // 0x20
    unsigned long m_k[4];                           // 0x30
    unsigned long m_q[4];                           // 0x40
    unsigned long m_s[4];                           // 0x50
    int m_bSeeded;                                  // 0x60
    unsigned long* m_pState;                        // 0x64 (0x80-byte entropy pool)
    unsigned short m_nPoolPos;                      // 0x68

    ~CExoRandInternal();
    unsigned long Rand();
    unsigned long ReSeed(unsigned long s0, unsigned long s1, unsigned long s2, unsigned long s3);
    void AddEntropy(const void* pData, int nLength);
};

// FUNCTION: SWKOTOR 0x005ECC80
CExoRandInternal::~CExoRandInternal()
{
    if (m_pState) {
        delete m_pState;
        m_pState = 0;
    }
}

// FUNCTION: SWKOTOR 0x005ECCA0
unsigned long CExoRandInternal::Rand()
{
    unsigned long z0 = (((m_z[0] << m_q[0]) ^ m_z[0]) >> m_ks[0]) ^ ((m_z[0] & m_c[0]) << m_s[0]);
    m_z[0] = z0;
    unsigned long z1 = (((m_z[1] << m_q[1]) ^ m_z[1]) >> m_ks[1]) ^ ((m_z[1] & m_c[1]) << m_s[1]);
    m_z[1] = z1;
    unsigned long z2 = (((m_z[2] << m_q[2]) ^ m_z[2]) >> m_ks[2]) ^ ((m_z[2] & m_c[2]) << m_s[2]);
    m_z[2] = z2;
    unsigned long z3 = (((m_z[3] << m_q[3]) ^ m_z[3]) >> m_ks[3]) ^ ((m_z[3] & m_c[3]) << m_s[3]);
    m_z[3] = z3;
    return z0 ^ z1 ^ z2 ^ z3;
}

// FUNCTION: SWKOTOR 0x005ECD40
unsigned long CExoRandInternal::ReSeed(unsigned long s0, unsigned long s1, unsigned long s2, unsigned long s3)
{
    // NONMATCH (13 diff insns): scheduling of the k/s loads only; the original
    // loads m_k[0] first and keeps it for 32 - m_k[0], loads the mask after
    // the m_ks[3] store. Store-order variants (A..I) did not get closer.
    unsigned long ks0 = m_k[0] - m_s[0];
    unsigned long ks1 = m_k[1] - m_s[1];
    unsigned long ks2 = m_k[2] - m_s[2];
    unsigned long ks3 = m_k[3] - m_s[3];
    unsigned long nMask = g_nRandMask;
    m_bSeeded = 1;
    m_ks[0] = ks0;
    m_ks[1] = ks1;
    m_ks[2] = ks2;
    m_ks[3] = ks3;
    m_c[0] = nMask << (32 - m_k[0]);
    m_c[1] = nMask << (32 - m_k[1]);
    m_c[2] = nMask << (32 - m_k[2]);
    m_c[3] = nMask << (32 - m_k[3]);
    if (s0 > (unsigned long)(1 << ks0)) {
        m_z[0] = s0;
    }
    if (s1 > (unsigned long)(1 << ks1)) {
        m_z[1] = s1;
    }
    if (s2 > (unsigned long)(1 << ks2)) {
        m_z[2] = s2;
    }
    if (s3 > (unsigned long)(1 << ks3)) {
        m_z[3] = s3;
    }
    return Rand();
}

// FUNCTION: SWKOTOR 0x005ECE20
void CExoRandInternal::AddEntropy(const void* pData, int nLength)
{
    if (m_nPoolPos + nLength < 0x80) {
        memcpy((char*)m_pState + m_nPoolPos, pData, nLength);
        m_nPoolPos += nLength;
    } else {
        memcpy((char*)m_pState + m_nPoolPos, pData, 0x80 - m_nPoolPos);
        memcpy(m_pState, (const char*)pData + 0x80 - m_nPoolPos, m_nPoolPos + nLength - 0x80);
        m_nPoolPos += nLength - 0x80;
    }
}
