// swc_save_load.h -- CSWCSaveLoad
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/client_exo_app_internal_dtor_s8.cpp); the other copies' members fall on its members.
#ifndef SHARED_SWC_SAVE_LOAD_H
#define SHARED_SWC_SAVE_LOAD_H


class CSWCSaveLoad {
public:
    virtual ~CSWCSaveLoad();

    // declared by other copies
    int SendPlayerToServer_06_21();
    int SendSaveGameRequest(int a, int b, int c);
    int SendLoadGameRequest(int a, int b, int c);
    void HandleMessage(unsigned char* pData, unsigned long nSize);
};

#endif
