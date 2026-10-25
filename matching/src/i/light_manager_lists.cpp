class LightManager {
    char pad_0x00[0x5c];
    void* m_lBumpMapLights;
    char pad_0x60[0x64];
    void* m_lLensFlareLights;

public:
    void* GetBumpMapLights();
    void* GetLensFlareLights();
};

// FUNCTION: SWKOTOR 0x004A28A0
void* LightManager::GetBumpMapLights()
{
    return &m_lBumpMapLights;
}

// FUNCTION: SWKOTOR 0x004A28B0
void* LightManager::GetLensFlareLights()
{
    return &m_lLensFlareLights;
}
