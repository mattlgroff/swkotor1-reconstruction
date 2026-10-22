// Pilot group 1: CTwoDimArrays loaders. Needs /GX (EH frame for new).

class CResRef {
public:
    char m_resRef[16];
    CResRef(const char* szName);
    CResRef(const CResRef& cResRef);
};

class C2DA {
public:
    char pad_0x0[0x54];
    C2DA(CResRef cResRef, int bPrivate);
    int Load2DArray();
};

class CTwoDimArrays {
public:
    char pad_0x0[0x7c];
    C2DA* m_pDifficultyOptions;  // 0x7c
    int Load2DArrays_DifficultyOptions();
};

// FUNCTION: SWKOTOR 0x005C2680
int CTwoDimArrays::Load2DArrays_DifficultyOptions()
{
    m_pDifficultyOptions = new C2DA(CResRef("difficultyopt"), 0);
    if (m_pDifficultyOptions && m_pDifficultyOptions->Load2DArray()) {
        return 1;
    }
    return 0;
}
