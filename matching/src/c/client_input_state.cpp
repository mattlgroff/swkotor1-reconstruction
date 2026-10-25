class CClientExoAppInternal {
public:
    char pad_0x00[0x2bc];
    int m_bInputDisabled;

    void DisableInput();
    void EnableInput();
};

// FUNCTION: SWKOTOR 0x0061F7C0
void CClientExoAppInternal::DisableInput()
{
    m_bInputDisabled = 1;
}

// FUNCTION: SWKOTOR 0x0061F7D0
void CClientExoAppInternal::EnableInput()
{
    m_bInputDisabled = 0;
}
