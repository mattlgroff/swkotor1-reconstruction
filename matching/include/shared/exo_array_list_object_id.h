// exo_array_list_object_id.h -- CExoArrayListObjectId
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/m/sw_c_sound_object.h); the other copies' members fall on its members.
#ifndef SHARED_EXO_ARRAY_LIST_OBJECT_ID_H
#define SHARED_EXO_ARRAY_LIST_OBJECT_ID_H


class CExoArrayListObjectId {
public:
    unsigned long* element;
    int num;
    int array_size;
    void Add(unsigned long t);                      // 0x006449D0

    // declared by other copies
    void AddUnique(unsigned long oid);
};

#endif
