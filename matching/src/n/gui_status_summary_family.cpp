// Status-summary accumulation and retrieval, with the native 24-byte record.
struct CStatusSummary {
    unsigned long m_nFlags;
    unsigned char m_nLightShift, m_nDarkShift;
    char pad_0x6[2];
    int m_nCredits;
    unsigned long m_nXp, m_nStealthXp;
    int m_nSuppressEntries;
};
class CSWGuiStatusSummary {
public:
    char pad_0x0[0x64];
    CStatusSummary m_summary;
    void AddCredits(int nAmount);
    void AddXp(unsigned long nAmount);
    void AddStealthXp(unsigned long nAmount);
    void AddAlignmentShift(signed char nShift);
};
class CGuiInGame {
public:
    char pad_0x0[0xA8];
    CSWGuiStatusSummary* m_pStatusSummary;
    void UpdateStatus(unsigned char kind,int value);
    int GetPendingStatusSummary();
    void GetStatusSummary(CStatusSummary* pSummary);
    void SuppressStatusSummary(int nEntries);
};
// FUNCTION: SWKOTOR 0x00624AB0
void CSWGuiStatusSummary::AddCredits(int nAmount)
{
    if (nAmount) {
        if ((m_summary.m_nCredits > 0 && nAmount < 0) ||
            (m_summary.m_nCredits < 0 && nAmount > 0))
            m_summary.m_nFlags |= 0x100;
        m_summary.m_nCredits += nAmount;
        m_summary.m_nFlags |= 1;
    }
}
// FUNCTION: SWKOTOR 0x0062B580
void CSWGuiStatusSummary::AddXp(unsigned long nAmount)
{
    if (nAmount) {
        m_summary.m_nXp += nAmount;
        m_summary.m_nFlags |= 1;
    }
}
// FUNCTION: SWKOTOR 0x0062B5A0
void CSWGuiStatusSummary::AddStealthXp(unsigned long nAmount)
{
    if (nAmount) {
        m_summary.m_nStealthXp += nAmount;
        m_summary.m_nFlags |= 1;
    }
}
// FUNCTION: SWKOTOR 0x00624A70
void CSWGuiStatusSummary::AddAlignmentShift(signed char nShift)
{
    if (nShift) {
        if (nShift > 0) m_summary.m_nLightShift += nShift;
        else m_summary.m_nDarkShift -= nShift;
        m_summary.m_nFlags |= 1;
    }
}
// FUNCTION: SWKOTOR 0x0062EF70
int CGuiInGame::GetPendingStatusSummary()
{
    return m_pStatusSummary && (m_pStatusSummary->m_summary.m_nFlags & 3) ? 1 : 0;
}
// FUNCTION: SWKOTOR 0x0062F0A0
void CGuiInGame::GetStatusSummary(CStatusSummary* pSummary)
{
    if (m_pStatusSummary) *pSummary = m_pStatusSummary->m_summary;
}
// FUNCTION: SWKOTOR 0x0062F0C0
void CGuiInGame::SuppressStatusSummary(int nEntries)
{
    if (m_pStatusSummary && nEntries >= 0)
        m_pStatusSummary->m_summary.m_nSuppressEntries = nEntries;
}

// FUNCTION: SWKOTOR 0x0062EEB0
void CGuiInGame::UpdateStatus(unsigned char kind,int value)
{
    CSWGuiStatusSummary* p=m_pStatusSummary;
    if(!p) return;
    if(p->m_summary.m_nSuppressEntries>0) { --p->m_summary.m_nSuppressEntries; return; }
    switch(kind) {
    case 0: p->m_summary.m_nFlags|=0x11; return;
    case 1: p->AddCredits(value); return;
    case 2: p->AddXp(value); return;
    case 3: p->AddStealthXp(value); return;
    case 4: case 5: p->AddAlignmentShift((signed char)value); return;
    case 7: p->m_summary.m_nFlags|=5; return;
    case 8: p->m_summary.m_nFlags|=9; return;
    case 9: p->m_summary.m_nFlags|=0x22; return;
    case 10: p->m_summary.m_nFlags|=0x42; return;
    case 11: p->m_summary.m_nFlags|=0x82; return;
    }
}
