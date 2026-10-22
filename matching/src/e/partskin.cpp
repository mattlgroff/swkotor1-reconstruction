// PartSkin::DrawBumpedOutShell / Render (0x0048D690, 0x0048D6C0).

bool __stdcall AurVertexProgramsAvailable();    // 0x0045FBD0

class SkinMesh {
public:
    virtual void v00(); virtual void v04();
    virtual void* GetVertexData();              // +0x08
};

void RenderSkinnedOutShell(void* pData, int nPass);     // 0x0048A0D0 (cdecl)

void RenderSkinVP(void* pData);                 // 0x0048A520 (cdecl)
void RenderSkinBumpedVP(void* pData);           // 0x0048C650 (cdecl)

extern int g_bRenderSkins;                      // 0x00798B68
extern int g_bUseVertexPrograms;                // 0x0078E8C0
extern void* g_pApp;                            // 0x007B92A0
extern const unsigned int g_nColorArray;        // 0x0073F3A0
extern "C" __declspec(dllimport) void __stdcall glDisableClientState(unsigned int array);

class SkinGob {
public:
    char pad_0x0[0x11c];
    float m_fOutShell;                          // +0x11c
};

class PartSkin {
public:
    char pad_0x0[0x40];
    SkinGob* m_pGob;                            // +0x40
    char pad_0x44[0x2c];
    SkinMesh* m_pMesh;                          // +0x70

    void DrawBumpedOutShellVP();                // 0x0048D040
    void DrawBumpedOutShell();
    void Render(unsigned char bBumped);         // vslot, 0x0048D6C0
};

// FUNCTION: SWKOTOR 0x0048D690
void PartSkin::DrawBumpedOutShell()
{
    if (AurVertexProgramsAvailable()) {
        DrawBumpedOutShellVP();
        return;
    }
    RenderSkinnedOutShell(m_pMesh->GetVertexData(), 1);
}

// FUNCTION: SWKOTOR 0x0048D6C0
void PartSkin::Render(unsigned char bBumped)
{
    if (!g_bRenderSkins)
        return;
    if (g_bUseVertexPrograms && AurVertexProgramsAvailable() && g_pApp) {
        if (!bBumped)
            RenderSkinVP(m_pMesh->GetVertexData());
        else
            RenderSkinBumpedVP(m_pMesh->GetVertexData());
    }
    else {
        glDisableClientState(g_nColorArray);
        RenderSkinnedOutShell(m_pMesh->GetVertexData(), 0);
    }
    if (m_pGob && m_pGob->m_fOutShell != 0.0f && g_pApp)
        DrawBumpedOutShell();
}
