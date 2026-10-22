// Worker J matching headers: CPathfindInformation and CAvoidCreature
// (0x005D0000-0x005D0F40). Member names are offsets where the meaning is unknown.
#ifndef J_PATHFIND_H
#define J_PATHFIND_H

struct Vector {
    Vector() { x = 0; y = 0; z = 0; }               // 0x00409E70
    float x, y, z;
};

class CPathfindInformation {
public:
    CPathfindInformation();
    void Initialize();                              // 0x005CFDA0
    void DeleteFirstTileFValueAlternatives();       // 0x005CEB70
    void ResetGridSearchData();                     // 0x005CEFA0
    void ResetWayPointData();                       // 0x005CF060
    void ResetPathData();                           // 0x005CF130
    void ResetInterTileSearchData();
    void ResetInterAreaPathSearchData();
    void ResetInterAreaSearchData();
    void ResetAtEndOfPath();

    char pad_0x0[0x1c];
    int    m_n1C;                        // 0x1c
    int    m_n20;                        // 0x20
    int    m_n24;                        // 0x24
    char pad_0x28[0xc];
    int    m_n34;                        // 0x34
    char pad_0x38[0x8];
    int    m_n40;                        // 0x40
    int    m_n44;                        // 0x44
    int    m_n48;                        // 0x48
    int    m_n4C;                        // 0x4c
    char pad_0x50[0x4];
    int    m_n54;                        // 0x54
    int    m_n58;                        // 0x58
    int    m_n5C;                        // 0x5c
    int    m_n60;                        // 0x60
    int    m_n64;                        // 0x64
    int    m_n68;                        // 0x68
    char pad_0x6c[0x4];
    int    m_n70;                        // 0x70
    int    m_n74;                        // 0x74
    int    m_n78;                        // 0x78
    int    m_n7C;                        // 0x7c
    int    m_n80;                        // 0x80
    int    m_n84;                        // 0x84
    int    m_n88;                        // 0x88
    char pad_0x8c[0x14];
    int    m_nA0;                        // 0xa0
    int    m_nA4;                        // 0xa4
    int    m_nA8;                        // 0xa8
    int*   m_nAC;                        // 0xac
    int*   m_nB0;                        // 0xb0
    int    m_nB4;                        // 0xb4
    int*   m_nB8;                        // 0xb8
    char pad_0xbc[0x4];
    int    m_nC0;                        // 0xc0
    int*   m_nC4;                        // 0xc4
    int*   m_nC8;                        // 0xc8
    int    m_nCC;                        // 0xcc
    int*   m_nD0;                        // 0xd0
    int*   m_nD4;                        // 0xd4
    int    m_nD8;                        // 0xd8
    int    m_nDC;                        // 0xdc
    int    m_nE0;                        // 0xe0
    char pad_0xe4[0xc];
    int    m_nF0;                        // 0xf0
    int*   m_nF4;                        // 0xf4
    char pad_0xf8[0x10];
    int    m_n108;                       // 0x108
    int    m_n10C;                       // 0x10c
    int    m_n110;                       // 0x110
    int    m_n114;                       // 0x114
    int    m_n118;                       // 0x118
    int    m_n11C;                       // 0x11c
    char pad_0x120[0xc];
    int    m_n12C;                       // 0x12c
    int    m_n130;                       // 0x130
    int    m_n134;                       // 0x134
    int    m_n138;                       // 0x138
    int    m_n13C;                       // 0x13c
    int    m_n140;                       // 0x140
    int    m_n144;                       // 0x144
    int    m_n148;                       // 0x148
    int    m_n14C;                       // 0x14c
    int    m_n150;                       // 0x150
    int    m_n154;                       // 0x154
    int    m_n158;                       // 0x158
    char pad_0x15c[0x14];
    int    m_n170;                       // 0x170
    int    m_n174;                       // 0x174
    int    m_n178;                       // 0x178
    int    m_n17C;                       // 0x17c
    int    m_n180;                       // 0x180
    int    m_n184;                       // 0x184
    char pad_0x188[0x4];
    int    m_n18C;                       // 0x18c
    char pad_0x190[0x4];
    int    m_n194;                       // 0x194
    char pad_0x198[0x20];
    int    m_n1B8;                       // 0x1b8
    int    m_n1BC;                       // 0x1bc
    int    m_n1C0;                       // 0x1c0
    int    m_n1C4;                       // 0x1c4
    int    m_n1C8;                       // 0x1c8
    int    m_n1CC;                       // 0x1cc
    char pad_0x1d0[0x14];
    int    m_n1E4;                       // 0x1e4
    int    m_n1E8;                       // 0x1e8
    int    m_n1EC;                       // 0x1ec
    int    m_n1F0;                       // 0x1f0
    int    m_n1F4;                       // 0x1f4
    int    m_n1F8;                       // 0x1f8
    char pad_0x1fc[0x4c];
    int    m_n248;                       // 0x248
    int    m_n24C;                       // 0x24c
    int    m_n250;                       // 0x250
    char pad_0x254[0x4];
    int    m_n258;                       // 0x258
    int    m_n25C;                       // 0x25c
    int    m_n260;                       // 0x260
    char pad_0x264[0x4];
};

class CAvoidCreature {
public:
    Vector* FindPath(int* pnPoints);
    Vector* FindPath_Left(int* pnPoints);           // 0x005CF650
    Vector* FindPath_Right(int* pnPoints);          // 0x005CF7A0
    int IsSegmentClear(int nParam, Vector* pvStart, Vector* pvEnd, float* pfDistance); // 0x005CF8D0
    int IsPathClear(int nParam, Vector* pvPoints, int nPoints, float* pfDistance);

    int m_bLeft;                                    // 0x00
    char pad_0x4[0x4c];
    int m_n50;                                      // 0x50
    Vector m_vStart;                                // 0x54
    char pad_0x60[0x4];
    int m_n64;                                      // 0x64
    Vector m_vEnd;                                  // 0x68
};

#endif
