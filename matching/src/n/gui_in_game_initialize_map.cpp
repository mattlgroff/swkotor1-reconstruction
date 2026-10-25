class CGuiMainInterface {
public:
    void InitializeMap();
    void ChangeCharacter();
    void SetupPauseGuiExtent(void* pExtent);
};

class CGuiInGame {
public:
    char pad_0x00[0x90];
    CGuiMainInterface* m_pMainInterface;

    void InitializeMap();
    void ChangeCharacter();
    void SetupPauseGuiExtent(void* pExtent);
};

// FUNCTION: SWKOTOR 0x0062B0A0
void CGuiInGame::InitializeMap()
{
    if (m_pMainInterface)
        m_pMainInterface->InitializeMap();
}

// FUNCTION: SWKOTOR 0x0062B0F0
void CGuiInGame::ChangeCharacter()
{
    m_pMainInterface->ChangeCharacter();
}

// FUNCTION: SWKOTOR 0x0062B100
void CGuiInGame::SetupPauseGuiExtent(void* pExtent)
{
    m_pMainInterface->SetupPauseGuiExtent(pExtent);
}
