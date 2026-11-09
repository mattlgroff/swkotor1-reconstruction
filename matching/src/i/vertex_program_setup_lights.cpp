// SetupVertexProgramLights (aurvertexprog.o, 0x004A30E0, placeholder name):
// loads the camera and default light parameters and appends the per-light
// program text to szText. functions.csv splits it at 0x004A30F7 (the first
// jump target); it is one function.
#include <stdio.h>
#include <string.h>
#pragma intrinsic(strcpy, strcat, strlen)

class Vector {
public:
    Vector()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    float x;
    float y;
    float z;
};

class Matrix {
public:
    Matrix() throw();                               // 0x00437AF0
    float m[9];
};

class Quaternion {
public:
    Matrix GetMatrix();                             // 0x0044F470
    float w, x, y, z;
};

class Camera {
public:
    char pad_0x0[0x7c];
    Vector m_vPosition;                             // +0x7C
    Quaternion m_qOrientation;                      // +0x88
};

extern Camera* g_pRenderCamera;                     // 0x007BB4E8
extern int g_bUseARBVertexProgram;                  // 0x0078E5EC
extern char g_szLightText0[0x400];                  // 0x00830E08 (placeholder name)
extern char g_szLightText1[0x400];                  // 0x00830A08 (placeholder name)
extern char g_szLightText2[0x400];                  // 0x00830608 (placeholder name)
extern void(__stdcall* g_pfnProgramEnvParameter4fARB)(unsigned int, unsigned int, float, float, float,
                                                      float);  // 0x007BB850
extern void(__stdcall* g_pfnProgramParameter4fNV)(unsigned int, unsigned int, float, float, float,
                                                  float);      // 0x007BB7AC

void AurSetVertexProgramParameter(unsigned int nIndex, float a, float b, float c, float d);  // 0x0044F820

static inline void SetVertexProgramParameter(unsigned int nIndex, float x, float y, float z, float w)
{
    if (g_bUseARBVertexProgram)
        g_pfnProgramEnvParameter4fARB(0x8620, nIndex, x, y, z, w);
    else
        g_pfnProgramParameter4fNV(0x8620, nIndex, x, y, z, w);
}

// FUNCTION: SWKOTOR 0x004A30E0
char* SetupVertexProgramLights(char* szText, int nLights, int bCondition)
{
    if (nLights < 1) {
        return "";
    }
    Matrix m;
    Vector pos;
    if (g_pRenderCamera) {
        m = g_pRenderCamera->m_qOrientation.GetMatrix();
        pos = g_pRenderCamera->m_vPosition;
    }
    AurSetVertexProgramParameter(0x5C, m.m[0], m.m[3], m.m[6], pos.x);
    AurSetVertexProgramParameter(0x5B, m.m[1], m.m[4], m.m[7], pos.y);
    AurSetVertexProgramParameter(0x5A, m.m[2], m.m[5], m.m[8], pos.z);
    SetVertexProgramParameter(0x59, 0.0f, 0.0f, 0.0f, 1.0f);
    SetVertexProgramParameter(0x0F, 1.0f, 1.0f, 1.0f, 1.0f);
    AurSetVertexProgramParameter(0x58, pos.x, pos.y, pos.z, 0.0f);
    int nIndex = 0x57;
    if (g_bUseARBVertexProgram) {
        sprintf(g_szLightText0,
                "MOV vFinalColour, program.env[89].xxxw;\n"
                "DP4 vReg0.x, vReg10, program.env[92];\n"
                "DP4 vReg0.y, vReg10, program.env[91];\n"
                "DP4 vReg0.z, vReg10, program.env[90];\n"
                "DP4 vReg0.w, vReg10, program.env[89];\n"
                "MOV vReg10, vReg0;\n"
                "DP3 vReg0.x, vNormalWorld, program.env[92];\n"
                "DP3 vReg0.y, vNormalWorld, program.env[91];\n"
                "DP3 vReg0.z, vNormalWorld, program.env[90];\n"
                "MOV vNormalWorld, vReg0;\n");
    } else {
        sprintf(g_szLightText0,
                "MOV R9.xyzw, c[89].xxxw;DP4 R0.x, R10, c[92];DP4 R0.y, R10, c[91];"
                "DP4 R0.z, R10, c[90];DP4 R0.w, R10, c[89];MOV R10, R0;"
                "DP3 R0.x, R11, c[92];DP3 R0.y, R11, c[91];DP3 R0.z, R11, c[90];MOV R11, R0;");
    }
    strcat(szText, g_szLightText0);
    for (int i = 0; i < nLights; i++) {
        SetVertexProgramParameter(nIndex, 0.0f, 0.0f, 0.0f, 0.0f);
        nIndex--;
        SetVertexProgramParameter(nIndex, 0.0f, 0.0f, 0.0f, 1.0f);
        nIndex--;
        SetVertexProgramParameter(nIndex, 0.0f, 0.0f, 0.0f, 1.0f);
        nIndex--;
        SetVertexProgramParameter(nIndex, 0.0f, 0.0f, 0.0f, 1.0f);
        nIndex--;
        SetVertexProgramParameter(nIndex, 1.0f, 1.0f, 1.0f, 1.0f);
        nIndex--;
        if (g_bUseARBVertexProgram) {
            sprintf(g_szLightText0,
                    "ADD vReg0, -vReg10, program.env[%d];\n"
                    "DP3 vReg0.w, vReg0, vReg0;\n"
                    "RSQ vReg1.w, vReg0.w;\n"
                    "MUL vReg2, vReg0, vReg1.w;\n"
                    "DST vReg3, vReg0.w, vReg1.w;\n"
                    "DP3 vReg3.w, vReg3, program.env[%d];\n"
                    "RCP vReg3.w, vReg3.w;\n"
                    "DP3 vReg0.x, vNormalWorld, vReg2;\n"
                    "LIT vReg0, vReg0;\n"
                    "MUL vReg0, vReg0, vReg3.w;\n",
                    nIndex + 5, nIndex + 1);
            sprintf(g_szLightText1,
                    "MUL vReg1, vReg0.x, program.env[%d];\n"
                    "MAD vFinalColour.xyzw, vReg1, program.env[95], vFinalColour;\n",
                    nIndex + 4);
            sprintf(g_szLightText2,
                    "MUL vReg1, vReg0.y, program.env[%d];\n"
                    "MAD vFinalColour.xyzw, vReg1, program.env[94], vFinalColour;\n",
                    nIndex + 3);
        } else {
            sprintf(g_szLightText0,
                    "ADD R0, -R10, c[%d];DP3 R0.w, R0, R0;RSQ R1.w, R0.w;MUL R2, R0, R1.w;"
                    "DST R3, R0.w, R1.w;DP3 R3.w, R3, c[%d];RCP R3.w, R3.w;DP3 R0.x, R11, R2;"
                    "LIT R0, R0;MUL R0, R0, R3.w;",
                    nIndex + 5, nIndex + 1);
            sprintf(g_szLightText1, "MUL R1, R0.x, c[%d];MAD R9.xyzw, R1, c[95], R9;", nIndex + 4);
            sprintf(g_szLightText2, "MUL R1, R0.y, c[%d];MAD R9.xyzw, R1, c[94], R9;", nIndex + 3);
        }
        strcat(szText, g_szLightText0);
        strcat(szText, g_szLightText1);
        strcat(szText, g_szLightText2);
    }
    if (g_bUseARBVertexProgram) {
        strcat(szText, "ADD vFinalColour, vFinalColour, program.env[93];\n");
        strcat(szText, "MOV vFinalColour.w, program.env[94];\n");
        strcat(szText, "MOV result.color, vFinalColour;\n");
    } else {
        strcat(szText, "ADD R9.xyzw, R9.xyzw, c[93].xyzw;");
        strcat(szText, "MOV R9.w, c[94].w;");
        strcat(szText, "MUL o[COL0].xyzw, R9.xyzw, c[15].xxxx;");
    }
    return szText;
}
