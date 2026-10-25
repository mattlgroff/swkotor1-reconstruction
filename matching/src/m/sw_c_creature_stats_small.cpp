// Small CSWCCreatureStats accessors and class setters.

class CSWCCreatureStatsClassInfo {
public:
    char pad_0x0[0x1c];
    unsigned char m_nClass;
    unsigned char m_nLevel;
    char pad_0x1e[2];
};

class CSWCCreatureStats {
public:
    short GetArmorClass(int nUnused);
    short GetMaxHitPoints();
    unsigned char GetClass(unsigned char nIndex);
    unsigned char GetClassLevel(unsigned char nIndex);
    void SetClass(unsigned char nIndex, unsigned char nClass);
    void SetClassLevel(unsigned char nIndex, unsigned char nLevel);

    char pad_0x0[0x4e];
    short m_nMaxHitPoints;
    short m_nArmorClass;
    char pad_0x52[0x82];
    CSWCCreatureStatsClassInfo m_ClassInfo[2];
};

// FUNCTION: SWKOTOR 0x00647720
short CSWCCreatureStats::GetArmorClass(int nUnused)
{
    return m_nArmorClass;
}

// FUNCTION: SWKOTOR 0x00647A80
short CSWCCreatureStats::GetMaxHitPoints()
{
    return m_nMaxHitPoints;
}

// FUNCTION: SWKOTOR 0x00647730
unsigned char CSWCCreatureStats::GetClass(unsigned char nIndex)
{
    if (nIndex >= 2) {
        return 0xff;
    }
    return m_ClassInfo[nIndex].m_nClass;
}

// FUNCTION: SWKOTOR 0x00647750
unsigned char CSWCCreatureStats::GetClassLevel(unsigned char nIndex)
{
    if (nIndex >= 2) {
        return 0;
    }
    return m_ClassInfo[nIndex].m_nLevel;
}

// FUNCTION: SWKOTOR 0x00647770
void CSWCCreatureStats::SetClass(unsigned char nIndex, unsigned char nClass)
{
    if (nIndex < 2) {
        m_ClassInfo[nIndex].m_nClass = nClass;
    }
}

// FUNCTION: SWKOTOR 0x00647790
void CSWCCreatureStats::SetClassLevel(unsigned char nIndex, unsigned char nLevel)
{
    if (nIndex < 2) {
        m_ClassInfo[nIndex].m_nLevel = nLevel;
    }
}
