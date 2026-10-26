// CSWCameraOnAStick camera-object position forwards (descriptive names);
// they sit after CSWCObject in nwcobject.o and are called from CSWCModule.

#include "n/sw_behavior_camera.h"

// FUNCTION: SWKOTOR 0x0063F600
Vector CSWCameraOnAStick::GetCameraPosition()
{
    if (m_pCamera)
        return m_pCamera->GetPosition();
    return Vector(0.0f, 0.0f, 0.0f);
}

// FUNCTION: SWKOTOR 0x0063F630
void CSWCameraOnAStick::SetCameraPosition(Vector vPosition)
{
    if (m_pCamera)
        m_pCamera->SetPosition(vPosition);
}
