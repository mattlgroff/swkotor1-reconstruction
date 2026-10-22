// Worker J matching headers: full CLYT layout (layout resource holder,
// 0x005DE470-0x005DE8FD). j/res.h keeps the short CLYT for res.cpp.
#ifndef J_LYT_H
#define J_LYT_H

#include "j/res.h"

struct Vector {
    float x;
    float y;
    float z;
};

struct Quaternion {
    float w;
    float x;
    float y;
    float z;
};

class CLYT : public CResHelper<CResLYT, 3000> {
public:
    CLYT();
    virtual ~CLYT();
    void UnloadLayout();                            // 0x005DE450
    Quaternion GetRoomOrientation(int nIndex);
    Vector GetRoomPosition(int nIndex);
    Quaternion GetTrackOrientation(int nIndex);
    Vector GetTrackPosition(int nIndex);
    Quaternion GetObstacleOrientation(int nIndex);
    Vector GetObstaclePosition(int nIndex);

    int m_n1C;                                      // 0x1c
    int m_bLoaded;                                  // 0x20
    int m_nRooms;                                   // 0x24
    int m_nTracks;                                  // 0x28
    int m_nObstacles;                               // 0x2c
    int m_nDoorHooks;                               // 0x30
    Quaternion* m_pRoomOrientations;                // 0x34
    Quaternion* m_pTrackOrientations;               // 0x38
    Quaternion* m_pObstacleOrientations;            // 0x3c
    Vector* m_pRoomPositions;                       // 0x40
    Vector* m_pTrackPositions;                      // 0x44
    Vector* m_pObstaclePositions;                   // 0x48
    CExoString* m_pDoorHookNames;                   // 0x4c
    CExoString* m_pDoorHookRooms;                   // 0x50
    char* m_p54;                                    // 0x54
    char* m_p58;                                    // 0x58
    char* m_p5C;                                    // 0x5c
};

#endif
