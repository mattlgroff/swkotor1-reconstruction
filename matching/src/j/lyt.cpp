// CLYT ctor / dtor and the indexed orientation / position getters
// (0x005DE470-0x005DE8FD).
#include "j/lyt.h"

// Defined (and annotated) in matching/src/f/res.cpp; repeated here so the
// destructor can inline it.
void CLYT::UnloadLayout()
{
    if (m_bLoaded) {
        m_pRes->Release();
        m_bLoaded = 0;
    }
}

// FUNCTION: SWKOTOR 0x005DE470
Quaternion CLYT::GetRoomOrientation(int nIndex)
{
    return m_pRoomOrientations[nIndex];
}

// FUNCTION: SWKOTOR 0x005DE4A0
Vector CLYT::GetRoomPosition(int nIndex)
{
    return m_pRoomPositions[nIndex];
}

// FUNCTION: SWKOTOR 0x005DE4D0
Quaternion CLYT::GetTrackOrientation(int nIndex)
{
    return m_pTrackOrientations[nIndex];
}

// FUNCTION: SWKOTOR 0x005DE500
Vector CLYT::GetTrackPosition(int nIndex)
{
    return m_pTrackPositions[nIndex];
}

// FUNCTION: SWKOTOR 0x005DE530
Quaternion CLYT::GetObstacleOrientation(int nIndex)
{
    return m_pObstacleOrientations[nIndex];
}

// FUNCTION: SWKOTOR 0x005DE560
Vector CLYT::GetObstaclePosition(int nIndex)
{
    return m_pObstaclePositions[nIndex];
}

// FUNCTION: SWKOTOR 0x005DE770
CLYT::CLYT()
{
    m_nRooms = 0;
    m_pRoomOrientations = 0;
    m_pRoomPositions = 0;
    m_nTracks = 0;
    m_pTrackOrientations = 0;
    m_pTrackPositions = 0;
    m_nObstacles = 0;
    m_pObstacleOrientations = 0;
    m_pObstaclePositions = 0;
    m_nDoorHooks = 0;
    m_pDoorHookNames = 0;
    m_pDoorHookRooms = 0;
    m_p54 = 0;
    m_p58 = 0;
    m_p5C = 0;
}

// FUNCTION: SWKOTOR 0x005DE7E0
CLYT::~CLYT()
{
    UnloadLayout();
    delete[] m_pRoomOrientations;
    m_pRoomOrientations = 0;
    delete[] m_pRoomPositions;
    m_pRoomPositions = 0;
    delete[] m_pTrackOrientations;
    m_pTrackOrientations = 0;
    delete[] m_pTrackPositions;
    m_pTrackPositions = 0;
    delete[] m_pObstacleOrientations;
    m_pObstacleOrientations = 0;
    delete[] m_pObstaclePositions;
    m_pObstaclePositions = 0;
    delete[] m_pDoorHookNames;
    m_pDoorHookNames = 0;
    delete[] m_pDoorHookRooms;
    m_pDoorHookRooms = 0;
    delete[] m_p54;
    m_p54 = 0;
    delete[] m_p58;
    m_p58 = 0;
    delete[] m_p5C;
    m_p5C = 0;
}
