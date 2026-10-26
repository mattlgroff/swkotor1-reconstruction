// Journal item highlighting.
struct Vector { float x, y, z; };
extern Vector g_vKeyMapChangedColor;     // 0x007A23FC
extern Vector g_vDifficultyExitColor;    // 0x007A23B4
extern Vector g_vAbilityColorGranted;    // 0x007A23C0

class CSWGuiJournalItemEntry {
public:
    char pad_0x00[0x8c];
    float m_fBorderAlpha;
    Vector m_vBorderColor;
    char pad_0x9c[0x68];
    Vector m_vHilightColor;
    char pad_0x110[0xb8];
    unsigned m_bEncouraged : 1;
    void SetEncouraged(int bEncouraged);
};

// FUNCTION: SWKOTOR 0x00644790
void CSWGuiJournalItemEntry::SetEncouraged(int bEncouraged)
{
    if (m_bEncouraged == bEncouraged) return;
    m_bEncouraged = bEncouraged;
    if (m_bEncouraged) {
        m_vBorderColor = g_vKeyMapChangedColor;
        m_fBorderAlpha = 0.5f;
        m_vHilightColor = g_vKeyMapChangedColor;
    } else {
        m_vBorderColor = g_vDifficultyExitColor;
        m_vHilightColor = g_vAbilityColorGranted;
    }
}
