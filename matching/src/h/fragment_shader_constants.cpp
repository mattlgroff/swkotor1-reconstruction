// ATI fragment shader constant setup.
struct Vector { float x, y, z; };
float* AurMakeColor(const Vector& vColor, const float& fAlpha); // 0x004AAAB0
extern void (__stdcall *glSetFragmentShaderConstantATI)(unsigned int, const float*); // 0x007BB5D8

// FUNCTION: SWKOTOR 0x0047A280
void SetFragmentShaderConstant0(const Vector& color, float alpha)
{
    glSetFragmentShaderConstantATI(0x8941, AurMakeColor(color, alpha));
}

// FUNCTION: SWKOTOR 0x0047A2A0
void SetFragmentShaderConstant1(const Vector& color, float alpha)
{
    glSetFragmentShaderConstantATI(0x8942, AurMakeColor(color, alpha));
}

extern "C" __declspec(dllimport) const unsigned char* __stdcall glGetString(unsigned int);
extern "C" __declspec(dllimport) void __stdcall glGetIntegerv(unsigned int, int*);

// FUNCTION: SWKOTOR 0x0047A2C0
void ReportARBError(const char* currentProgram)
{
    int position[10];
    const unsigned char* error = glGetString(0x8874);
    if (*error)
        glGetIntegerv(0x864b, position);
}
