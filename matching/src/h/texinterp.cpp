// bilinear lookup used by the texture controllers (0x004608A0).

static inline void FloatToInt(int* pOut, float f)
{
    __asm {
        fld f
        mov edx, pOut
        frndint
        fistp dword ptr [edx]
    }
}

// FUNCTION: SWKOTOR 0x004608A0
float FloatInterpolate(float* pData, float x, float y, int nWidth, int nHeight)
{
    int ix, iy;
    FloatToInt(&ix, x - 0.5f);
    FloatToInt(&iy, y - 0.5f);
    float fx = x - ix;
    int ix1 = (ix + 1) % nWidth;
    float fy = y - iy;
    int iy1 = (iy + 1) % nHeight;
    return (pData[iy * nWidth + ix] * (1.0f - fx) + pData[iy * nWidth + ix1] * fx) * (1.0f - fy) +
           (pData[iy1 * nWidth + ix] * (1.0f - fx) + pData[iy1 * nWidth + ix1] * fx) * fy;
}
