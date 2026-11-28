// PartLight material color.
extern "C" __declspec(dllimport) void __stdcall glColor4f(float, float, float, float);
#include "shared/vector.h"
#include "shared/globals.h"

inline float ReadColor(const float& x) { return x; }

class PartLight {
public:
    char pad_0x0[0x4c];
    Vector m_vColor;
    void Mat();
};

// FUNCTION: SWKOTOR 0x004738C0
void PartLight::Mat()
{
    glColor4f(m_vColor.x, ReadColor(m_vColor.y), ReadColor(m_vColor.z), 1.0f);
    g_vPartLightColor = m_vColor;
}
