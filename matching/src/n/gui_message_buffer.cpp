class CGuiInGame {
public:
    char pad_0x00[0xf8];
    void* m_pMessageBuffer;
    char pad_0xfc[4];
    unsigned int m_nMessageBufferSize;

    void* GetMessageBuffer();
    unsigned int GetMessageBufferSize();
};

// FUNCTION: SWKOTOR 0x0062A9E0
void* CGuiInGame::GetMessageBuffer()
{
    return m_pMessageBuffer;
}

// FUNCTION: SWKOTOR 0x0062A9F0
unsigned int CGuiInGame::GetMessageBufferSize()
{
    return m_nMessageBufferSize;
}
