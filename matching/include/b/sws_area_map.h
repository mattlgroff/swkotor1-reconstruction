// CSWSAreaMap: the server-side explored-area bitmap of an area
// (b0057_area_map_room). The map is 440 x 256 pixels; the explored grid is
// m_nGridWidth x m_nGridHeight cells, one bit per cell.
#ifndef B_SWS_AREA_MAP_H
#define B_SWS_AREA_MAP_H

#include "b/vector.h"

// 0x004AA0F0: yaw (degrees) of a direction vector.
float __cdecl GetVectorYaw(const Vector& v);

class CSWSAreaMap {
public:
    CSWSAreaMap();                                                      // 0x00578BE0
    ~CSWSAreaMap();                                                     // 0x00578C30
    void Initialize(int bEnable, int nGridWidth, int nNorth,
                    float fWorldX0, float fWorldY0, float fWorldX1, float fWorldY1,
                    int nMapX0, int nMapY0, int nMapX1, int nMapY1,
                    int nDivisor);                                      // 0x00578C60
    int GetMapPixelFromWorldCoord(Vector vWorld, int* pMapX, int* pMapY); // 0x00578E00
    float GetMapRotateCCWFromWorldOrientation(Vector vOrientation);     // 0x00578ED0
    int GetGridPixelFromMapPixel(int nMapX, int nMapY, int* pGridX, int* pGridY); // 0x00578F10
    void SetAllExplored();                                              // 0x00578FA0
    void SetExploredData(unsigned long* pData, unsigned long nWords,
                         int nGridWidth, int nGridHeight);              // 0x00578FC0
    void SetMapPointExplored(int nMapX, int nMapY);                     // 0x00579000
    unsigned long* GetExploredData(unsigned long* pWords, int* pGridWidth,
                                   int* pGridHeight);                   // 0x00579090
    void SetPartyMemberWorldLocation(int nIndex, Vector vWorld);        // 0x005790C0
    void SetPartyMemberWorldOrientation(int nIndex, Vector vOrientation); // 0x00579130
    int GetPartyMemberMapPixel(int nIndex, int* pMapX, int* pMapY);     // 0x005791B0
    float GetPartyMemberMapRotation(int nIndex);                        // 0x005791F0
    int IsPartyIndexValid(int nIndex)
    {
        if (m_nWords <= 0 || nIndex >= 3)
            return 0;
        return 1;
    }
    int IsWorldPointExplored(Vector vWorld);                            // 0x00579210
    void SetWorldPointExplored(Vector vWorld, int nRadius);             // 0x005792D0

    unsigned long* m_pBits;         // +0x00
    unsigned long m_nWords;         // +0x04
    int m_nGridWidth;               // +0x08
    int m_nGridHeight;              // +0x0C
    int m_nNorth;                   // +0x10
    float m_fInvDivisor;            // +0x14
    float m_fScaleX;                // +0x18
    float m_fScaleY;                // +0x1C
    float m_fOriginX;               // +0x20
    float m_fOriginY;               // +0x24
    int m_nPartyMapX[3];            // +0x28
    int m_nPartyMapY[3];            // +0x34
    float m_fPartyRotation[3];      // +0x40
};

#endif
