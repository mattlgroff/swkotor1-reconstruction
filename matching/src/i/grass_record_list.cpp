#include <string.h>
// CExoArrayList of the 0x54-byte aurgrass records (0x004A6C70). The
// record is unnamed; its fields are placeholders.
struct Vector {
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    float x, y, z;
};

struct CAurGrassRecord {
    CAurGrassRecord()
    {
        m_n0 = 0;
        m_n20 = 0.0f;
        m_n24 = 0.0f;
        m_n50 = 0;
        for (int i = 0; i < 4; i++) {
            m_af40[i] = 0.25f;
        }
    }
    char m_n0;                                      // 0x00 (name, 0x20 chars)
    char pad_0x1[0x20 - 0x1];
    float m_n20;                                    // 0x20
    float m_n24;                                    // 0x24
    Vector m_v28;                                   // 0x28
    Vector m_v34;                                   // 0x34
    float m_af40[4];                                // 0x40
    unsigned char m_n50;                            // 0x50
};

template <class T>
class CExoArrayList {
public:
    void Allocate(int n);
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 8);
        }
        element[num++] = t;
    }
    T* element;
    int num;
    int array_size;
};

template <class T>
void CExoArrayList<T>::Allocate(int n)
{
    T* pOld = element;
    array_size = n;
    element = new T[n];
    for (int i = 0; i < num; i++)
        element[i] = pOld[i];
    if (pOld)
        delete[] pOld;
}

// FUNCTION: SWKOTOR 0x004A6C70 SYMBOL: ?Allocate@?$CExoArrayList@UCAurGrassRecord@@@@QAEXH@Z
template void CExoArrayList<CAurGrassRecord>::Allocate(int n);

extern CExoArrayList<CAurGrassRecord> g_aAurGrassInfo;  // 0x00831E44 (placeholder name)

// Stores grass material nIndex (growing the table with defaults first),
// normalising its four probabilities to sum to 1 (placeholder name).
// FUNCTION: SWKOTOR 0x004A74F0
void AurGrassSetInfo(int nIndex, CAurGrassRecord* pInfo)
{
    while (nIndex >= g_aAurGrassInfo.num) {
        g_aAurGrassInfo.Add(CAurGrassRecord());
    }
    CAurGrassRecord* pDest = &g_aAurGrassInfo.element[nIndex];
    pDest->m_n20 = pInfo->m_n20;
    pDest->m_n24 = pInfo->m_n24;
    pDest->m_v34 = pInfo->m_v34;
    pDest->m_v28 = pInfo->m_v28;
    float fSum = pInfo->m_af40[0];
    pDest->m_af40[0] = pInfo->m_af40[0];
    fSum += pInfo->m_af40[1];
    pDest->m_af40[1] = pInfo->m_af40[1];
    fSum += pInfo->m_af40[2];
    pDest->m_af40[2] = pInfo->m_af40[2];
    fSum += pInfo->m_af40[3];
    pDest->m_af40[3] = pInfo->m_af40[3];
    if (fSum != 1.0f) {
        float fScale = 1.0f / fSum;
        pDest->m_af40[0] *= fScale;
        pDest->m_af40[1] *= fScale;
        pDest->m_af40[2] *= fScale;
        pDest->m_af40[3] *= fScale;
    }
    strcpy(&g_aAurGrassInfo.element[nIndex].m_n0, &pInfo->m_n0);
    g_aAurGrassInfo.element[nIndex].m_n50 = 1;
}
