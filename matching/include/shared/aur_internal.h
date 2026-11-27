// aur_internal.h -- CAurInternal
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/pilot1/tiny.cpp); the other copies' members fall on its members.
#ifndef SHARED_AUR_INTERNAL_H
#define SHARED_AUR_INTERNAL_H


class CAuroraInstance;

class CAurInternal {
public:
    CAuroraInstance* CreateInstance();
    CAuroraInstance* NewCAuroraInstance();

    // declared by other copies
    static void DestroyCAuroraInstance();
};

#endif
