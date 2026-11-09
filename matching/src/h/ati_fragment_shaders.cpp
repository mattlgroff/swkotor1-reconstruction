// ATI fragment shader program setup (GL_ATI_fragment_shader).
// Names without independent symbol evidence are descriptive.
extern "C" __declspec(dllimport) void __stdcall glEnable(unsigned int);
extern "C" __declspec(dllimport) void __stdcall glDisable(unsigned int);
extern "C" __declspec(dllimport) unsigned int __stdcall glGetError();
extern "C" const unsigned char* __stdcall gluErrorString(unsigned int);
int AurGetGLError(); // 0x0044EEE0

extern unsigned int (__stdcall *glGenFragmentShadersATI)(unsigned int); // 0x007BB754
extern void (__stdcall *glBindFragmentShaderATI)(unsigned int); // 0x007BB6CC
extern void (__stdcall *glBeginFragmentShaderATI)(); // 0x007BB7F4
extern void (__stdcall *glEndFragmentShaderATI)(); // 0x007BB554
extern void (__stdcall *glSampleMapATI)(unsigned int, unsigned int, unsigned int); // 0x007BB620
extern void (__stdcall *glPassTexCoordATI)(unsigned int, unsigned int, unsigned int); // 0x007BB804
extern void (__stdcall *glColorFragmentOp1ATI)(unsigned int, unsigned int, unsigned int, unsigned int,
    unsigned int, unsigned int, unsigned int); // 0x007BB6D4
extern void (__stdcall *glColorFragmentOp2ATI)(unsigned int, unsigned int, unsigned int, unsigned int,
    unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int); // 0x007BB840
extern void (__stdcall *glColorFragmentOp3ATI)(unsigned int, unsigned int, unsigned int, unsigned int,
    unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int,
    unsigned int, unsigned int, unsigned int); // 0x007BB7FC
extern void (__stdcall *glAlphaFragmentOp1ATI)(unsigned int, unsigned int, unsigned int,
    unsigned int, unsigned int, unsigned int); // 0x007BB710
extern void (__stdcall *glAlphaFragmentOp2ATI)(unsigned int, unsigned int, unsigned int,
    unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int); // 0x007BB5E8

extern unsigned int g_nATIShader_00830244;
extern unsigned int g_nATIShader_00830248;
extern unsigned int g_nATIShader_0083024C;
extern unsigned int g_nATIShader_00830250;
extern unsigned int g_nATIShader_00830254;
extern unsigned int g_nATIShader_00830258;
extern unsigned int g_nATIShader_0083025C;
extern unsigned int g_nATIShader_00830260;
extern unsigned int g_nATIShader_00830264;

inline void CheckGLError()
{
    unsigned int error = glGetError();
    if (error)
        gluErrorString(error);
}
#define CHECKERR() CheckGLError()

// FUNCTION: SWKOTOR 0x004794E0
void InitializeATIFragmentShaders()
{
    glEnable(0x8920);
    g_nATIShader_00830244 = glGenFragmentShadersATI(1);
    glBindFragmentShaderATI(g_nATIShader_00830244);
    glBeginFragmentShaderATI();
    glSampleMapATI(0x8921, 0x84c0, 0x8976);
    glSampleMapATI(0x8922, 0x84c1, 0x8976);
    glColorFragmentOp2ATI(0x8963, 0x8923, 0x0, 0x0, 0x8577, 0x0, 0x0, 0x8922, 0x0, 0x0);
    glAlphaFragmentOp2ATI(0x8964, 0x8921, 0x0, 0x8577, 0x0, 0x0, 0x8921, 0x0, 0x0);
    glColorFragmentOp2ATI(0x8964, 0x8921, 0x0, 0x0, 0x8921, 0x0, 0x0, 0x8923, 0x0, 0x0);
    glEndFragmentShaderATI();
    g_nATIShader_0083024C = glGenFragmentShadersATI(1);
    glBindFragmentShaderATI(g_nATIShader_0083024C);
    glBeginFragmentShaderATI();
    glSampleMapATI(0x8921, 0x84c0, 0x8976);
    glSampleMapATI(0x8922, 0x84c1, 0x8976);
    glColorFragmentOp2ATI(0x8964, 0x8921, 0x7, 0x0, 0x8921, 0x0, 0x0, 0x8577, 0x0, 0x0);
    glColorFragmentOp3ATI(0x8969, 0x8921, 0x7, 0x0, 0x8921, 0x1906, 0x0, 0x8921, 0x0, 0x0, 0x8922, 0x0, 0x0);
    glColorFragmentOp3ATI(0x8969, 0x8921, 0x7, 0x0, 0x896d, 0x1903, 0x0, 0x8942, 0x0, 0x0, 0x8921, 0x0, 0x0);
    glAlphaFragmentOp1ATI(0x8961, 0x8921, 0x0, 0x8941, 0x0, 0x0);
    glEndFragmentShaderATI();
    g_nATIShader_00830248 = glGenFragmentShadersATI(1);
    glBindFragmentShaderATI(g_nATIShader_00830248);
    glBeginFragmentShaderATI();
    glSampleMapATI(0x8921, 0x84c0, 0x8976);
    glSampleMapATI(0x8922, 0x84c1, 0x8976);
    glColorFragmentOp2ATI(0x8964, 0x8921, 0x7, 0x0, 0x8921, 0x0, 0x0, 0x8577, 0x0, 0x0);
    glColorFragmentOp3ATI(0x8969, 0x8921, 0x7, 0x0, 0x8921, 0x1906, 0x0, 0x8921, 0x0, 0x0, 0x8922, 0x0, 0x0);
    glAlphaFragmentOp1ATI(0x8961, 0x8921, 0x0, 0x8941, 0x0, 0x0);
    glEndFragmentShaderATI();
    g_nATIShader_00830250 = glGenFragmentShadersATI(1);
    glBindFragmentShaderATI(g_nATIShader_00830250);
    glBeginFragmentShaderATI();
    glSampleMapATI(0x8921, 0x84c0, 0x8976);
    glSampleMapATI(0x8922, 0x84c1, 0x8976);
    glSampleMapATI(0x8923, 0x84c2, 0x8976);
    glSampleMapATI(0x8924, 0x84c3, 0x8976);
    glColorFragmentOp2ATI(0x8966, 0x8926, 0x0, 0x0, 0x8921, 0x0, 0x9, 0x8923, 0x0, 0x9);
    glColorFragmentOp2ATI(0x8966, 0x8925, 0x0, 0x0, 0x8921, 0x0, 0x9, 0x8922, 0x0, 0x9);
    glColorFragmentOp2ATI(0x8964, 0x8921, 0x0, 0x0, 0x8926, 0x0, 0x0, 0x8926, 0x0, 0x0);
    glColorFragmentOp1ATI(0x8961, 0x8926, 0x0, 0x0, 0x8926, 0x1905, 0x0);
    glColorFragmentOp2ATI(0x8964, 0x8921, 0x0, 0x0, 0x8925, 0x0, 0x0, 0x8942, 0x0, 0x0);
    glAlphaFragmentOp2ATI(0x8964, 0x8926, 0x0, 0x8926, 0x1903, 0x0, 0x8926, 0x1903, 0x0);
    glPassTexCoordATI(0x8921, 0x8921, 0x8976);
    glPassTexCoordATI(0x8922, 0x8922, 0x8976);
    glPassTexCoordATI(0x8923, 0x8923, 0x8976);
    glPassTexCoordATI(0x8924, 0x8924, 0x8976);
    glPassTexCoordATI(0x8925, 0x8925, 0x8976);
    glPassTexCoordATI(0x8926, 0x8926, 0x8976);
    glColorFragmentOp2ATI(0x8964, 0x8921, 0x0, 0x0, 0x8925, 0x0, 0x0, 0x896d, 0x0, 0x0);
    glColorFragmentOp2ATI(0x8964, 0x8922, 0x0, 0x0, 0x896d, 0x0, 0x0, 0x8941, 0x0, 0x0);
    glAlphaFragmentOp2ATI(0x8964, 0x8922, 0x0, 0x8926, 0x1906, 0x0, 0x8926, 0x1906, 0x0);
    glColorFragmentOp2ATI(0x8963, 0x8921, 0x0, 0x40, 0x8577, 0x0, 0x0, 0x8921, 0x0, 0x0);
    glAlphaFragmentOp2ATI(0x8964, 0x8922, 0x0, 0x8922, 0x1906, 0x0, 0x8922, 0x1906, 0x0);
    glColorFragmentOp2ATI(0x8964, 0x8921, 0x0, 0x0, 0x8921, 0x0, 0x0, 0x8924, 0x0, 0x0);
    glColorFragmentOp2ATI(0x8964, 0x8922, 0x0, 0x0, 0x8922, 0x1906, 0x0, 0x8922, 0x0, 0x0);
    glColorFragmentOp2ATI(0x8964, 0x8922, 0x0, 0x0, 0x8922, 0x0, 0x0, 0x8924, 0x1906, 0x2);
    glColorFragmentOp2ATI(0x8963, 0x8921, 0x0, 0x40, 0x8922, 0x0, 0x0, 0x8921, 0x0, 0x0);
    glAlphaFragmentOp1ATI(0x8961, 0x8921, 0x0, 0x8577, 0x1906, 0x0);
    glEndFragmentShaderATI();
    CHECKERR();
    g_nATIShader_00830254 = glGenFragmentShadersATI(1);
    glBindFragmentShaderATI(g_nATIShader_00830254);
    glBeginFragmentShaderATI();
    glSampleMapATI(0x8921, 0x84c0, 0x8976);
    glSampleMapATI(0x8922, 0x84c1, 0x8976);
    glSampleMapATI(0x8923, 0x84c2, 0x8976);
    glSampleMapATI(0x8924, 0x84c3, 0x8976);
    glColorFragmentOp2ATI(0x8966, 0x8926, 0x0, 0x0, 0x8922, 0x0, 0x9, 0x8924, 0x0, 0x9);
    glColorFragmentOp2ATI(0x8966, 0x8925, 0x0, 0x0, 0x8922, 0x0, 0x9, 0x8923, 0x0, 0x9);
    glColorFragmentOp2ATI(0x8964, 0x8922, 0x0, 0x0, 0x8926, 0x0, 0x0, 0x8926, 0x0, 0x0);
    glColorFragmentOp1ATI(0x8961, 0x8926, 0x0, 0x0, 0x8926, 0x1905, 0x0);
    glColorFragmentOp2ATI(0x8964, 0x8922, 0x0, 0x0, 0x8925, 0x0, 0x0, 0x8942, 0x0, 0x0);
    glAlphaFragmentOp2ATI(0x8964, 0x8926, 0x0, 0x8926, 0x1903, 0x0, 0x8926, 0x1903, 0x0);
    glPassTexCoordATI(0x8921, 0x8921, 0x8976);
    glPassTexCoordATI(0x8922, 0x8922, 0x8976);
    glPassTexCoordATI(0x8923, 0x8923, 0x8976);
    glPassTexCoordATI(0x8924, 0x8924, 0x8976);
    glPassTexCoordATI(0x8925, 0x8925, 0x8976);
    glPassTexCoordATI(0x8926, 0x8926, 0x8976);
    glColorFragmentOp2ATI(0x8964, 0x8922, 0x0, 0x0, 0x8925, 0x0, 0x0, 0x896d, 0x0, 0x0);
    glColorFragmentOp2ATI(0x8964, 0x8923, 0x0, 0x0, 0x896d, 0x0, 0x0, 0x8941, 0x0, 0x0);
    glAlphaFragmentOp2ATI(0x8964, 0x8923, 0x0, 0x8926, 0x1906, 0x0, 0x8926, 0x1906, 0x0);
    glColorFragmentOp2ATI(0x8963, 0x8922, 0x0, 0x40, 0x8577, 0x0, 0x0, 0x8922, 0x0, 0x0);
    glAlphaFragmentOp2ATI(0x8964, 0x8923, 0x0, 0x8923, 0x1906, 0x0, 0x8923, 0x1906, 0x0);
    glColorFragmentOp2ATI(0x8964, 0x8922, 0x0, 0x0, 0x8922, 0x0, 0x0, 0x8921, 0x0, 0x0);
    glColorFragmentOp2ATI(0x8964, 0x8923, 0x0, 0x0, 0x8923, 0x1906, 0x0, 0x8923, 0x0, 0x0);
    glColorFragmentOp2ATI(0x8964, 0x8923, 0x0, 0x0, 0x8923, 0x0, 0x0, 0x8921, 0x1906, 0x2);
    glColorFragmentOp2ATI(0x8963, 0x8921, 0x0, 0x40, 0x8923, 0x0, 0x0, 0x8922, 0x0, 0x0);
    glAlphaFragmentOp1ATI(0x8961, 0x8921, 0x0, 0x8577, 0x1906, 0x0);
    glEndFragmentShaderATI();
    CHECKERR();
    g_nATIShader_00830258 = glGenFragmentShadersATI(1);
    glBindFragmentShaderATI(g_nATIShader_00830258);
    glBeginFragmentShaderATI();
    glSampleMapATI(0x8921, 0x84c0, 0x8976);
    glSampleMapATI(0x8922, 0x84c1, 0x8976);
    glSampleMapATI(0x8923, 0x84c2, 0x8976);
    glColorFragmentOp2ATI(0x8964, 0x8924, 0x7, 0x0, 0x8577, 0x0, 0x0, 0x8921, 0x0, 0x0);
    glColorFragmentOp3ATI(0x8968, 0x8924, 0x7, 0x0, 0x8922, 0x0, 0x0, 0x8921, 0x0, 0x0, 0x8924, 0x0, 0x0);
    glColorFragmentOp2ATI(0x8964, 0x8925, 0x7, 0x0, 0x8923, 0x0, 0x0, 0x8921, 0x1906, 0x2);
    glColorFragmentOp2ATI(0x8963, 0x8921, 0x7, 0x0, 0x8924, 0x0, 0x0, 0x8925, 0x0, 0x0);
    glAlphaFragmentOp1ATI(0x8961, 0x8921, 0x0, 0x8941, 0x1906, 0x0);
    glEndFragmentShaderATI();
    g_nATIShader_0083025C = glGenFragmentShadersATI(1);
    glBindFragmentShaderATI(g_nATIShader_0083025C);
    glBeginFragmentShaderATI();
    glSampleMapATI(0x8921, 0x84c0, 0x8976);
    glSampleMapATI(0x8922, 0x84c1, 0x8976);
    glSampleMapATI(0x8923, 0x84c2, 0x8976);
    glSampleMapATI(0x8924, 0x84c3, 0x8976);
    glSampleMapATI(0x8925, 0x84c4, 0x8976);
    glColorFragmentOp2ATI(0x8966, 0x8926, 0x1, 0x0, 0x8922, 0x0, 0x9, 0x8921, 0x0, 0x9);
    glColorFragmentOp2ATI(0x8966, 0x8926, 0x2, 0x0, 0x8923, 0x0, 0x9, 0x8921, 0x0, 0x9);
    glColorFragmentOp2ATI(0x8966, 0x8926, 0x4, 0x0, 0x8924, 0x0, 0x9, 0x8921, 0x0, 0x9);
    glColorFragmentOp2ATI(0x8966, 0x8924, 0x0, 0x1, 0x8926, 0x0, 0x0, 0x8925, 0x0, 0x9);
    glColorFragmentOp2ATI(0x8964, 0x8924, 0x0, 0x0, 0x8926, 0x0, 0x0, 0x8924, 0x0, 0x0);
    glColorFragmentOp2ATI(0x8966, 0x8923, 0x0, 0x0, 0x8926, 0x0, 0x0, 0x8926, 0x0, 0x0);
    glColorFragmentOp3ATI(0x8968, 0x8924, 0x0, 0x0, 0x8925, 0x0, 0xd, 0x8923, 0x0, 0x0, 0x8924, 0x0, 0x0);
    glSampleMapATI(0x8926, 0x8924, 0x8976);
    glColorFragmentOp1ATI(0x8961, 0x8921, 0x0, 0x0, 0x8926, 0x0, 0x0);
    glAlphaFragmentOp1ATI(0x8961, 0x8921, 0x0, 0x8926, 0x0, 0x0);
    glAlphaFragmentOp2ATI(0x8964, 0x8921, 0x0, 0x8921, 0x0, 0x0, 0x8941, 0x0, 0x0);
    glEndFragmentShaderATI();
    CHECKERR();
    g_nATIShader_00830260 = glGenFragmentShadersATI(1);
    glBindFragmentShaderATI(g_nATIShader_00830260);
    glBeginFragmentShaderATI();
    glColorFragmentOp1ATI(0x8961, 0x8921, 0x0, 0x0, 0x8577, 0x0, 0x0);
    glAlphaFragmentOp1ATI(0x8961, 0x8921, 0x0, 0x8577, 0x1906, 0x0);
    glEndFragmentShaderATI();
    glDisable(0x8920);
    AurGetGLError();
    g_nATIShader_00830264 = glGenFragmentShadersATI(1);
    glBindFragmentShaderATI(g_nATIShader_00830264);
    glBeginFragmentShaderATI();
    glSampleMapATI(0x8921, 0x84c0, 0x8976);
    glColorFragmentOp2ATI(0x8964, 0x8921, 0x0, 0x0, 0x8577, 0x0, 0x0, 0x8921, 0x0, 0x0);
    glAlphaFragmentOp2ATI(0x8961, 0x8921, 0x0, 0x8577, 0x0, 0x0, 0x8921, 0x0, 0x0);
    glColorFragmentOp3ATI(0x8969, 0x8921, 0x7, 0x0, 0x896d, 0x1903, 0x0, 0x8942, 0x0, 0x0, 0x8921, 0x0, 0x0);
    glEndFragmentShaderATI();
    glDisable(0x8920);
}

extern "C" __declspec(dllimport) void __stdcall glBindTexture(unsigned int, unsigned int);
extern "C" __declspec(dllimport) void __stdcall glTexGeni(unsigned int, unsigned int, int);

class CAurTexture {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual unsigned char HasCubeMap();         // +0x0c
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9c();
    virtual void va0(); virtual void va4(); virtual void va8(); virtual void vac();
    virtual void vb0();
    virtual void Bind();                        // +0xb4
};

class GLRender {
public:
    static void ActivateTextureStage(int nStage);   // 0x0041FE80
};
void SetCurrentStageTID(int nStage, int nTID);      // 0x00426550
void MakeLookUpTexture(float fPower);               // 0x00423D80
CAurTexture* AurTextureGetNormalizationCubeMap();   // 0x004233E0

extern unsigned int g_nActiveTextureUnit;   // 0x007A6898
extern int g_aCurrentStageTID[];            // 0x007A683C
extern int g_bFSEnvMapOn;                   // 0x00827FA0, descriptive
extern int g_bFSTexture1On;                 // 0x00827FA4, descriptive

// FUNCTION: SWKOTOR 0x0047A2F0
void EnableFragmentShaders(int mode, CAurTexture* t0, CAurTexture* t1, CAurTexture* t2, CAurTexture* t3)
{
    switch (mode) {
    case 0:
        glBindFragmentShaderATI(0);
        glDisable(0x8920);
        glEnable(0xb50);
        GLRender::ActivateTextureStage(0);
        glBindTexture(0xde1, 0);
        GLRender::ActivateTextureStage(1);
        glBindTexture(0xde1, 0);
        GLRender::ActivateTextureStage(2);
        glBindTexture(0xde1, 0);
        GLRender::ActivateTextureStage(3);
        glBindTexture(0xde1, 0);
        GLRender::ActivateTextureStage(0);
        SetCurrentStageTID(0, 0);
        SetCurrentStageTID(1, 0);
        SetCurrentStageTID(2, 0);
        SetCurrentStageTID(3, 0);
        break;
    case 3:
        GLRender::ActivateTextureStage(0);
        t0->Bind();
        glEnable(0x8920);
        glBindFragmentShaderATI(g_nATIShader_00830264);
        break;
    case 6:
        g_bFSTexture1On = 1;
        glDisable(0xb50);
        GLRender::ActivateTextureStage(0);
        glEnable(0xde1);
        t0->Bind();
        GLRender::ActivateTextureStage(1);
        glEnable(0x8513);
        t1->Bind();
        GLRender::ActivateTextureStage(2);
        glEnable(0x8513);
        t2->Bind();
        GLRender::ActivateTextureStage(3);
        glEnable(0xde1);
        t3->Bind();
        glEnable(0x8920);
        glBindFragmentShaderATI(g_nATIShader_00830250);
        break;
    case 13:
        glDisable(0x8920);
        GLRender::ActivateTextureStage(1);
        glDisable(0x8513);
        glDisable(0xde1);
        g_aCurrentStageTID[g_nActiveTextureUnit] = 0;
        GLRender::ActivateTextureStage(2);
        glDisable(0x8513);
        glDisable(0xde1);
        g_aCurrentStageTID[g_nActiveTextureUnit] = 0;
        GLRender::ActivateTextureStage(3);
        glDisable(0xde1);
        glDisable(0x8513);
        g_aCurrentStageTID[g_nActiveTextureUnit] = 0;
        glEnable(0xb50);
        GLRender::ActivateTextureStage(0);
        break;
    case 7:
        g_bFSTexture1On = 1;
        glDisable(0xb50);
        GLRender::ActivateTextureStage(0);
        glEnable(0xde1);
        t0->Bind();
        GLRender::ActivateTextureStage(1);
        glEnable(0xde1);
        t1->Bind();
        GLRender::ActivateTextureStage(2);
        glEnable(0x8513);
        t2->Bind();
        GLRender::ActivateTextureStage(3);
        glEnable(0x8513);
        t3->Bind();
        glEnable(0x8920);
        glBindFragmentShaderATI(g_nATIShader_00830254);
        break;
    case 19:
        g_bFSEnvMapOn = 1;
        g_bFSTexture1On = 1;
        GLRender::ActivateTextureStage(0);
        t0->Bind();
        GLRender::ActivateTextureStage(1);
        t1->Bind();
        if (t1->HasCubeMap()) {
            glTexGeni(0x2000, 0x2500, 0x8512);
            glTexGeni(0x2001, 0x2500, 0x8512);
            glTexGeni(0x2002, 0x2500, 0x8512);
            glEnable(0xc60);
            glEnable(0xc61);
            glEnable(0xc62);
        } else {
            glEnable(0xde1);
            glTexGeni(0x2000, 0x2500, 0x2402);
            glTexGeni(0x2001, 0x2500, 0x2402);
            glEnable(0xc60);
            glEnable(0xc61);
        }
        glEnable(0x8920);
        glBindFragmentShaderATI(g_nATIShader_00830248);
        break;
    case 20:
        g_bFSEnvMapOn = 1;
        g_bFSTexture1On = 1;
        GLRender::ActivateTextureStage(0);
        t0->Bind();
        GLRender::ActivateTextureStage(1);
        t1->Bind();
        if (t1->HasCubeMap()) {
            glTexGeni(0x2000, 0x2500, 0x8512);
            glTexGeni(0x2001, 0x2500, 0x8512);
            glTexGeni(0x2002, 0x2500, 0x8512);
            glEnable(0xc60);
            glEnable(0xc61);
            glEnable(0xc62);
        } else {
            glEnable(0xde1);
            glTexGeni(0x2000, 0x2500, 0x2402);
            glTexGeni(0x2001, 0x2500, 0x2402);
            glEnable(0xc60);
            glEnable(0xc61);
        }
        glEnable(0x8920);
        glBindFragmentShaderATI(g_nATIShader_0083024C);
        break;
    case 32:
        if (g_bFSTexture1On) {
            GLRender::ActivateTextureStage(1);
            if (g_bFSEnvMapOn) {
                glDisable(0x8513);
                glDisable(0xc60);
                glDisable(0xc61);
                glDisable(0xc62);
                g_bFSEnvMapOn = 0;
                g_aCurrentStageTID[g_nActiveTextureUnit] = 0;
            }
            glDisable(0xde1);
            g_bFSTexture1On = 0;
        }
        GLRender::ActivateTextureStage(0);
        glDisable(0x8920);
        break;
    case 26:
        GLRender::ActivateTextureStage(0);
        glEnable(0xde1);
        t0->Bind();
        GLRender::ActivateTextureStage(1);
        glEnable(0xde1);
        t1->Bind();
        g_bFSTexture1On = 1;
        glEnable(0x8920);
        glBindFragmentShaderATI(g_nATIShader_00830244);
        break;
    case 22:
        g_bFSEnvMapOn = 1;
        g_bFSTexture1On = 1;
        GLRender::ActivateTextureStage(0);
        t0->Bind();
        GLRender::ActivateTextureStage(2);
        t1->Bind();
        if (t1->HasCubeMap()) {
            glTexGeni(0x2000, 0x2500, 0x8512);
            glTexGeni(0x2001, 0x2500, 0x8512);
            glTexGeni(0x2002, 0x2500, 0x8512);
            glEnable(0xc60);
            glEnable(0xc61);
            glEnable(0xc62);
        } else {
            glEnable(0xde1);
            glTexGeni(0x2000, 0x2500, 0x2402);
            glTexGeni(0x2001, 0x2500, 0x2402);
            glEnable(0xc60);
            glEnable(0xc61);
        }
        GLRender::ActivateTextureStage(1);
        glEnable(0xde1);
        t2->Bind();
        glEnable(0x8920);
        glBindFragmentShaderATI(g_nATIShader_00830258);
        break;
    case 37:
        GLRender::ActivateTextureStage(1);
        glDisable(0x8513);
        glDisable(0xde1);
        g_aCurrentStageTID[g_nActiveTextureUnit] = 0;
        GLRender::ActivateTextureStage(2);
        glDisable(0x8513);
        glDisable(0xde1);
        g_aCurrentStageTID[g_nActiveTextureUnit] = 0;
        glDisable(0xc60);
        glDisable(0xc61);
        glDisable(0xc62);
        glDisable(0x8920);
        GLRender::ActivateTextureStage(0);
        break;
    case 24:
        glEnable(0x8920);
        glBindFragmentShaderATI(g_nATIShader_0083025C);
        break;
    case 29:
        MakeLookUpTexture(5.0f);
        GLRender::ActivateTextureStage(0);
        glEnable(0xde1);
        t0->Bind();
        GLRender::ActivateTextureStage(1);
        glEnable(0x8513);
        AurTextureGetNormalizationCubeMap()->Bind();
        GLRender::ActivateTextureStage(2);
        glEnable(0x8513);
        AurTextureGetNormalizationCubeMap()->Bind();
        GLRender::ActivateTextureStage(3);
        glEnable(0x8513);
        AurTextureGetNormalizationCubeMap()->Bind();
        GLRender::ActivateTextureStage(4);
        glEnable(0x8513);
        AurTextureGetNormalizationCubeMap()->Bind();
        GLRender::ActivateTextureStage(5);
        glEnable(0x8513);
        t3->Bind();
        GLRender::ActivateTextureStage(0);
        break;
    case 2:
        glEnable(0x8920);
        glBindFragmentShaderATI(g_nATIShader_00830260);
        break;
    }
}
