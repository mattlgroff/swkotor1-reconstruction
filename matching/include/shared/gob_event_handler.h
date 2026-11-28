// gob_event_handler.h -- GobEventHandler
//
// One declaration for every unit (CODE_RULES.md, R1): an animation event
// callback registered on a Gob (Gob::AddCallback).
#ifndef SHARED_GOB_EVENT_HANDLER_H
#define SHARED_GOB_EVENT_HANDLER_H

class Gob;

typedef void (*GobCallback)(Gob* pGob, char* szEvent, void* pData);

struct GobEventHandler {
    char szEvent[0x20];                                 // +0x00
    void (*pfnCallback)(Gob* pGob, char* szEvent, void* pData);  // +0x20
    void* pDefaultData;                                 // +0x24
    float fTime;                                        // +0x28
    unsigned char bAlways;                              // +0x2c

    GobEventHandler(GobCallback pfn, char* szName, void* pData, float fTime, unsigned char bAlways) throw();
};

#endif
