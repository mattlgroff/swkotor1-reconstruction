class CSWSCreature {
public:
    float MaxCleaveRangeImpl(unsigned long nFlags, int nA, int nB);
    float MaxCleaveRange(unsigned long nFlags, int nA, int nB);
};

// FUNCTION: SWKOTOR 0x004FFAD0
float CSWSCreature::MaxCleaveRange(unsigned long nFlags, int nA, int nB)
{
    return MaxCleaveRangeImpl(nFlags, nA, nB) + 2.0f;
}
