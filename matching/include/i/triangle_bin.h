#ifndef I_TRIANGLE_BIN_H
#define I_TRIANGLE_BIN_H

class CAurTriangleBin {
public:
    void BuildGrassPolys();
    __declspec(noinline) void CreateArrays(); // 0x004A87C0

    unsigned short vertex_format;
    unsigned char flags;
    unsigned char pad_03;
    unsigned int pad_04;
    unsigned int pad_08;
    int polygon_count;
};

#endif
