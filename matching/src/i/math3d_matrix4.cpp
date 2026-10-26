// 4x4 output-buffer transform (0x004AA9C0).
#include "i/vector_math.h"

// FUNCTION: SWKOTOR 0x004AA9C0
void MakeMatrix4x4(float* m, const Vector& t, const Quaternion& q)
{
    float xx = q.x * q.x;
    float yy = q.y * q.y;
    float zz = q.z * q.z;
    float yx = q.y * q.x;
    float zx = q.z * q.x;
    float wx = q.w * q.x;
    float zy = q.z * q.y;
    float wy = q.w * q.y;
    float wz = q.w * q.z;
    m[3] = 0.0f;
    m[7] = 0.0f;
    m[11] = 0.0f;
    m[0] = g_fOne - (zz + yy) * 2.0f;
    m[1] = (wz + yx) * 2.0f;
    m[2] = (zx - wy) * 2.0f;
    m[4] = (yx - wz) * 2.0f;
    m[5] = g_fOne - (zz + xx) * 2.0f;
    m[6] = (zy + wx) * 2.0f;
    m[8] = (wy + zx) * 2.0f;
    m[9] = (zy - wx) * 2.0f;
    m[10] = g_fOne - (yy + xx) * 2.0f;
    m[12] = t.x;
    m[13] = t.y;
    m[14] = t.z;
    m[15] = 1.0f;
}
