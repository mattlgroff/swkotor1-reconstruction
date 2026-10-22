// grass bin grid setup (0x00456E00).

extern "C" double __cdecl ceil(double);

class CAurTriangleBin;

class Vector {
public:
    float x, y, z;
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    void Allocate(int n);                   // 0x005E03C0 (pointer lists)
    void SetSize(int n);                    // 0x00456A00 (list of lists)
    void SetSizeInline(int n)
    {
        if (n == 0) {
            if (element) {
                delete[] element;
                element = 0;
            }
            array_size = 0;
        }
        else
            Allocate(n);
        num = n;
    }
};

typedef CExoArrayList<CAurTriangleBin*> TriangleBinList;

static inline void FloatToInt(int* pOut, float f)
{
    __asm {
        fld f
        mov edx, pOut
        frndint
        fistp dword ptr [edx]
    }
}

extern float g_fGrassBinSize;       // 0x0078E428

// FUNCTION: SWKOTOR 0x00456E00
void InitGrassBins(Vector vMin, Vector vMax, CExoArrayList<TriangleBinList>* pBins)
{
    pBins->num = 0;
    int nX;
    FloatToInt(&nX, (float)ceil((vMax.x - vMin.x) / g_fGrassBinSize));
    int nY;
    FloatToInt(&nY, (float)ceil((vMax.y - vMin.y) / g_fGrassBinSize));
    if (nX < 0 || nY < 0)
        return;
    pBins->SetSize(nX);
    for (int i = 0; i < nX; i++) {
        pBins->element[i].num = 0;
        pBins->element[i].SetSizeInline(nY);
        for (int j = 0; j < nY; j++)
            pBins->element[i].element[j] = 0;
    }
}
