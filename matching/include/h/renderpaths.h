// GL render helpers and PartTriMesh render paths
// (0x0046EF10-0x00471500).
#ifndef MATCHING_H_RENDERPATHS_H
#define MATCHING_H_RENDERPATHS_H

extern "C" __declspec(dllimport) void __stdcall glDisable(unsigned int);
extern "C" __declspec(dllimport) void __stdcall glEnable(unsigned int);
extern "C" __declspec(dllimport) void __stdcall glBegin(unsigned int);
extern "C" __declspec(dllimport) void __stdcall glEnd();
extern "C" __declspec(dllimport) void __stdcall glVertex3f(float, float, float);
extern "C" __declspec(dllimport) void __stdcall glColor3f(float, float, float);
extern "C" __declspec(dllimport) void __stdcall glColor4f(float, float, float, float);
extern "C" __declspec(dllimport) void __stdcall glTranslatef(float, float, float);
extern "C" __declspec(dllimport) void __stdcall glRotatef(float, float, float, float);
extern "C" __declspec(dllimport) void __stdcall glMultMatrixf(const float*);
extern "C" __declspec(dllimport) void __stdcall glMatrixMode(unsigned int);
extern "C" __declspec(dllimport) void __stdcall glPushMatrix();
extern "C" __declspec(dllimport) void __stdcall glPopMatrix();
extern "C" __declspec(dllimport) void __stdcall glLoadIdentity();
extern "C" __declspec(dllimport) void __stdcall glEnableClientState(unsigned int);
extern "C" __declspec(dllimport) void __stdcall glDisableClientState(unsigned int);
extern "C" __declspec(dllimport) void __stdcall glBlendFunc(unsigned int, unsigned int);
extern "C" __declspec(dllimport) void __stdcall glDepthMask(unsigned char);

extern "C" double __cdecl acos(double);
#pragma intrinsic(acos)

#include "shared/vector_ops.h"

class Quaternion {
public:
    float w, x, y, z;

    Quaternion() {}
    Quaternion(float fW, float fX, float fY, float fZ) { z = fZ; y = fY; x = fX; w = fW; }
    Vector GetAxis();                           // 0x00423C40
};

struct Plane {
    Vector n;
    float d;
};

int operator==(const Vector& a, const Vector& b);           // 0x004AA980
Vector cmul(const Vector& a, const Vector& b);                 // 0x004AB1E0
Vector PlaneIntersect(const Vector& n, float d, const Vector& a,
                      const Vector& b);                     // 0x004AB250
void BuildMatrix(float* pOut, const Vector* pPos, const Quaternion* pOri);  // 0x004AA9C0

extern float g_fZeroEpsilon;                // 0x0073D700

#endif
