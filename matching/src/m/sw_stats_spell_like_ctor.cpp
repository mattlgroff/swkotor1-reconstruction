// CSWSStats_SpellLikeAbility record initialization.
struct CSWSStats_SpellLikeAbility {
    CSWSStats_SpellLikeAbility();

    int m_nSpellId;
    int m_bReadied;
    unsigned char m_nCasterLevel;
};

// FUNCTION: SWKOTOR 0x006475C0 SYMBOL: ??0CSWSStats_SpellLikeAbility@@QAE@XZ
CSWSStats_SpellLikeAbility::CSWSStats_SpellLikeAbility()
    : m_nSpellId(0)
    , m_bReadied(1)
    , m_nCasterLevel(0)
{
}
