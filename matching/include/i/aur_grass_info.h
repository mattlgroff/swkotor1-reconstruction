#ifndef I_AUR_GRASS_INFO_H
#define I_AUR_GRASS_INFO_H

struct CAurGrassInfo {
    char name[0x20];
    unsigned int field_20;
    unsigned int field_24;
    unsigned int field_28;
    unsigned int field_2c;
    unsigned int field_30;
    unsigned int field_34;
    unsigned int field_38;
    unsigned int field_3c;
    float probabilities[4];
    unsigned char material_in_use;
    unsigned char pad_51[3];
};

extern unsigned int g_aur_grass_info_count;
extern CAurGrassInfo* g_aur_grass_info_data;

unsigned char AurGrassMaterialInUse(unsigned int index);

#endif
