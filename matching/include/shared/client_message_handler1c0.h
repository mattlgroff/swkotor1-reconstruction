// client_message_handler1c0.h -- CClientMessageHandler1c0
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/client_exo_app_internal_dtor_s8.cpp); the other copies' members fall on its members.
#ifndef SHARED_CLIENT_MESSAGE_HANDLER1C0_H
#define SHARED_CLIENT_MESSAGE_HANDLER1C0_H


struct CClientMessageHandler1c0 {
    int m_n00;

    // declared by other copies
    void HandleMessage(unsigned char* pData, unsigned long nSize);
};

#endif
