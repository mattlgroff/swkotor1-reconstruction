extern unsigned long g_action_failed_00745dc4;
extern unsigned long g_action_in_progress_00745dbc;
extern unsigned long g_action_complete_00745dc0;

class CSWSCreature {
public:
    char pad_0x00[0x9d4];
    int m_nAreaWaitState;

    void* GetArea();
    unsigned long AIActionAreaWait(void* pNode);
};

// FUNCTION: SWKOTOR 0x00511080
unsigned long CSWSCreature::AIActionAreaWait(void*)
{
    if (m_nAreaWaitState == 0)
        return g_action_failed_00745dc4;
    if (GetArea() == 0)
        return g_action_in_progress_00745dbc;
    return g_action_complete_00745dc0;
}
