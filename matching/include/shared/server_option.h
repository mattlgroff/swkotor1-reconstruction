// server_option.h -- CServerOption
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/b/server_info.h); the other copies' members fall on its members.
#ifndef SHARED_SERVER_OPTION_H
#define SHARED_SERVER_OPTION_H


struct CServerOption {
    char* m_szName;                                 // 0x0
    char* m_szValue;                                // 0x4
    char pad_0x8[0x8];
};

#endif
