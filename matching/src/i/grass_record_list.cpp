// CExoArrayList of the 0x54-byte aurgrass records (0x004A6C70). The
// record is unnamed; its fields are placeholders.
struct CAurGrassRecord {
    CAurGrassRecord()
    {
        m_n0 = 0;
        m_n20 = 0;
        m_n24 = 0;
        m_n28 = 0;
        m_n2c = 0;
        m_a30[0] = 0;
        m_a30[1] = 0;
        m_a30[2] = 0;
        m_a30[3] = 0;
        m_af40[0] = 0.25f;
        m_af40[1] = 0.25f;
        m_af40[2] = 0.25f;
        m_af40[3] = 0.25f;
        m_n50 = 0;
    }
    unsigned char m_n0;                             // 0x00
    char pad_0x1[0x20 - 0x1];
    int m_n20;                                      // 0x20
    int m_n24;                                      // 0x24
    int m_n28;                                      // 0x28
    int m_n2c;                                      // 0x2c
    int m_a30[4];                                   // 0x30
    float m_af40[4];                                // 0x40
    unsigned char m_n50;                            // 0x50
};

template <class T>
class CExoArrayList {
public:
    void Allocate(int n);
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
