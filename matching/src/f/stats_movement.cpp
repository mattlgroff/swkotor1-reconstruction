// CSWSCreatureStats::SetMovementRate (nwscreaturestats.o): resolves the
// "default" movement rate 7 through appearance.2da MOVERATE and the
// creaturespeed.2da 2DAName column, then caches the walk / run rates.

class CExoString {
public:
    CExoString();                                           // 0x005B3190
    CExoString(const char *s);                              // 0x005E5A90
    ~CExoString();                                          // 0x005E5C20
    int operator==(const CExoString &s) const;              // 0x005E5200
    CExoString LowerCase() const;                           // 0x005E6080
    char *m_sString;
    unsigned long m_nBufferLength;
};

class C2DA {
public:
    int GetCExoStringEntry(int nRow, const CExoString &sColumn, CExoString *pValue);  // 0x00413270
    int GetFLOATEntry(int nRow, int nColumn, float *pValue);  // 0x00413350

    char pad_0x0[0x24];
    int m_nNumRows;                     // 0x24
    int GetNumRows() { return m_nNumRows; }
};

class C2DArrays {
public:
    char pad_0x0[0xc];
    C2DA *m_pAppearanceTable;           // 0xc
    char pad_0x10[0x20 - 0x10];
    C2DA *m_pCreatureSpeedTable;        // 0x20
    C2DA *GetCreatureSpeedTable() { return m_pCreatureSpeedTable; }
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    C2DArrays *m_p2DArrays;             // 0xb8
    C2DArrays *Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules *g_pRules;              // 0x007A3A28
extern int g_nCreatureSpeedWalkColumn;  // 0x007A2248 (placeholder name)
extern int g_nCreatureSpeedRunColumn;   // 0x007A2244 (placeholder name)

class CSWSCreature {
public:
    char pad_0x0[0x9f0];
    short m_n9F0;                       // 0x9f0 (placeholder name)
};

class CSWSCreatureStats {
public:
    void SetMovementRate(int nRate);

    char pad_0x0[0x24];
    CSWSCreature *m_pBaseCreature;      // 0x24
    char pad_0x28[0x186 - 0x28];
    unsigned short m_nAppearanceType;   // 0x186
    unsigned short GetAppearanceType() { return m_nAppearanceType; }
    char pad_0x188[0x194 - 0x188];
    int m_nMovementRate;                // 0x194
    float m_fRunRate;                   // 0x198
    float m_fWalkRate;                  // 0x19c
};

// FUNCTION: SWKOTOR 0x005A5680
void CSWSCreatureStats::SetMovementRate(int nRate)
{
    if (nRate == 7) {
        CExoString sRate;
        CExoString sName;
        if (g_pRules->Get2DArrays()->m_pAppearanceTable->GetCExoStringEntry(GetAppearanceType(), "MOVERATE",
                                                                         &sRate)) {
            for (nRate = 0; nRate < g_pRules->Get2DArrays()->GetCreatureSpeedTable()->m_nNumRows; nRate++) {
                if (g_pRules->Get2DArrays()->GetCreatureSpeedTable()->GetCExoStringEntry(nRate, "2DAName",
                                                                                    &sName)) {
                    if (sRate.LowerCase() == sName.LowerCase())
                        break;
                }
            }
            if (nRate >= g_pRules->Get2DArrays()->GetCreatureSpeedTable()->m_nNumRows)
                nRate = 0;
        } else {
            nRate = 0;
        }
    }
    m_nMovementRate = nRate;
    g_pRules->Get2DArrays()->m_pCreatureSpeedTable->GetFLOATEntry(nRate, g_nCreatureSpeedWalkColumn,
                                                                &m_fWalkRate);
    g_pRules->Get2DArrays()->m_pCreatureSpeedTable->GetFLOATEntry(m_nMovementRate, g_nCreatureSpeedRunColumn,
                                                                &m_fRunRate);
    if (nRate == 1)
        m_pBaseCreature->m_n9F0 = -3;
}
