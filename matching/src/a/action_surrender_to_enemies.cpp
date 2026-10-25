extern unsigned long g_action_failed_00745dc4;
extern unsigned long g_action_complete_00745dc0;

class CSWSCreature {
public:
    char pad_0x00[0x9d4];
    int m_nSurrenderState;

    void SurrenderToEnemies(unsigned long nFlag);
    unsigned long AIActionSurrenderToEnemies(void* pNode);
};

// FUNCTION: SWKOTOR 0x0051B420
unsigned long CSWSCreature::AIActionSurrenderToEnemies(void*)
{
    if (m_nSurrenderState == 1)
        return g_action_failed_00745dc4;
    SurrenderToEnemies(0);
    return g_action_complete_00745dc0;
}
