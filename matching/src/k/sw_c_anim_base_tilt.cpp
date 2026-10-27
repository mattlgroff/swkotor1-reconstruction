// CSWCAnimBase::SetGroundTilt (nwcanimbase.o): tilts the model towards the
// ground normal, halfway from the previous tilt, under the given heading.
#include "k/sw_c_anim_base.h"

Vector CrossProduct(const Vector& a, const Vector& b);                  // 0x004AB210
Quaternion Slerp(Quaternion qFrom, const Quaternion& qTo, float fT);    // 0x004A9E00
Quaternion operator*(const Quaternion& a, const Quaternion& b);         // 0x004A9AC0
float Pitch(const Vector& vDirection);              // 0x004AA130

// FUNCTION: SWKOTOR 0x0069DE90
void CSWCAnimBase::SetGroundTilt(Vector* pNormal, Vector* pOrientation)
{
    if (!m_pModel) {
        return;
    }
    float fTilt = 90.0f - Pitch(*pNormal);
    Vector vAxis = CrossProduct(g_vZAxis, *pNormal);
    Quaternion qTarget(vAxis, fTilt * 0.017453292f);
    Quaternion qHalf = Slerp(qTarget, m_qE0, 0.5f);
    m_qE0 = qTarget;
    Quaternion qResult = qHalf * EulerToQuaternion(GetYaw(*pOrientation), 0.0f, 0.0f);
    m_pModel->SetOrientation(qResult);
}
