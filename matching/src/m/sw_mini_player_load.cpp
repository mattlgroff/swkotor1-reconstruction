// CSWMiniPlayer::Load (vtable 0x007525F0 slot 0x2c): speeds, tunnel limits
// and offsets. The Vector(x, y, z) arguments are read right to left.

#include "m/sw_mini_game_object.h"

// FUNCTION: SWKOTOR 0x006702F0
int CSWMiniPlayer::Load(CResGFF* pGFF, CResStruct* pStruct)
{
    if (!CSWTrackFollower::Load(pGFF, pStruct)) {
        return 0;
    }
    int bSuccess;
    float fMin = pGFF->ReadFieldFLOAT(pStruct, "Minimum_Speed", bSuccess, -1.0f);
    if (fMin >= 0.0f) {
        m_f1D8 = fMin;
    }
    float fMax = pGFF->ReadFieldFLOAT(pStruct, "Maximum_Speed", bSuccess, 100.0f);
    if (fMax >= 0.0f) {
        m_f1DC = fMax;
    }
    float fAccel = pGFF->ReadFieldFLOAT(pStruct, "Accel_Secs", bSuccess, -1.0f);
    if (fAccel == 0.0f) {
        float f = fMax - fMin;
        if (f >= 0.0f) {
            m_f1E0 = f;
        }
    }
    else if (fAccel > 0.0f) {
        float f = (fMax - fMin) / fAccel;
        if (f >= 0.0f) {
            m_f1E0 = f;
        }
    }
    m_vMin = Vector(pGFF->ReadFieldFLOAT(pStruct, "TunnelXPos", bSuccess, 0.0f),
                    pGFF->ReadFieldFLOAT(pStruct, "TunnelYPos", bSuccess, 0.0f),
                    pGFF->ReadFieldFLOAT(pStruct, "TunnelZPos", bSuccess, 0.0f));
    m_vMax = Vector(pGFF->ReadFieldFLOAT(pStruct, "TunnelXNeg", bSuccess, 0.0f),
                    pGFF->ReadFieldFLOAT(pStruct, "TunnelYNeg", bSuccess, 0.0f),
                    pGFF->ReadFieldFLOAT(pStruct, "TunnelZNeg", bSuccess, 0.0f));
    m_vClamp = pGFF->ReadFieldVector(pStruct, "TunnelInfinite", bSuccess, Vector());
    SetOffset(Vector(pGFF->ReadFieldFLOAT(pStruct, "Start_Offset_X", bSuccess, 0.0f),
                     pGFF->ReadFieldFLOAT(pStruct, "Start_Offset_Y", bSuccess, 0.0f),
                     pGFF->ReadFieldFLOAT(pStruct, "Start_Offset_Z", bSuccess, 0.0f)));
    m_v214 = Vector(pGFF->ReadFieldFLOAT(pStruct, "Target_Offset_X", bSuccess, 0.0f),
                    pGFF->ReadFieldFLOAT(pStruct, "Target_Offset_Y", bSuccess, 0.0f),
                    pGFF->ReadFieldFLOAT(pStruct, "Target_Offset_Z", bSuccess, 0.0f));
    return 1;
}
