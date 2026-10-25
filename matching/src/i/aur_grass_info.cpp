#include "i/aur_grass_info.h"

// FUNCTION: SWKOTOR 0x004A6840
unsigned char AurGrassMaterialInUse(unsigned int index)
{
    const int last = (int)g_aur_grass_info_count - 1;
    if ((int)index > last)
        return 0;
    return g_aur_grass_info_data[index].material_in_use;
}

// FUNCTION: SWKOTOR 0x004A6870
void* __fastcall aur_grass_info_construct_004a6870(CAurGrassInfo* self, void*)
{
    self->field_28 = 0;
    self->field_2c = 0;
    self->field_30 = 0;
    self->field_34 = 0;
    self->field_38 = 0;
    self->field_3c = 0;
    self->name[0] = 0;
    self->field_20 = 0;
    self->field_24 = 0;
    self->material_in_use = 0;
    self->probabilities[0] = 0.25f;
    self->probabilities[1] = 0.25f;
    self->probabilities[2] = 0.25f;
    self->probabilities[3] = 0.25f;
    return self;
}
