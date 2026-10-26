class CGuiInGame {
public:
    char pad_0x0[0x198]; unsigned long m_nDialogAnimations;
    char pad_0x19c[4]; unsigned long* m_pDialogObjects; unsigned short* m_pDialogAnimations;
    void StoreLastDialogAnimationData(); // 0x0062B960
    int InitializeDialogAnimationData(unsigned long count);
};
// NONMATCH: bounded source-shape attempts retained; see worker-n.md.
// FUNCTION: SWKOTOR 0x00630620
int CGuiInGame::InitializeDialogAnimationData(unsigned long count)
{
    int result=0;
    StoreLastDialogAnimationData();
    m_nDialogAnimations=0;
    if(m_pDialogObjects) { delete [] m_pDialogObjects; m_pDialogObjects=0; }
    if(m_pDialogAnimations) { delete [] m_pDialogAnimations; m_pDialogAnimations=0; }
    if(count) {
        m_nDialogAnimations=count;
        m_pDialogObjects=new unsigned long[m_nDialogAnimations];
        m_pDialogAnimations=new unsigned short[m_nDialogAnimations];
        if(m_pDialogObjects && m_pDialogAnimations) result=1;
        else { delete [] m_pDialogObjects; delete [] m_pDialogAnimations; }
    }
    return result;
}
