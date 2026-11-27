// grass bin grid setup (0x00456E00).

#include "shared/float_to_int.h"

extern "C" double __cdecl ceil(double);

class CAurTriangleBin;

#include "shared/vector.h"

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

#include "shared/globals.h"

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
