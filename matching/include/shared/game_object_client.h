// game_object_client.h -- CGameObjectClient
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/b/sws_creature.h); the other copies' members fall on its members.
#ifndef SHARED_GAME_OBJECT_CLIENT_H
#define SHARED_GAME_OBJECT_CLIENT_H


class CSWCCreature;

class CGameObjectClient {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); 
    virtual CSWCCreature* AsSWCCreature();                       // +0x2c
    virtual void v12();
    virtual void* AsClientItem();                           // 0x34
};

#endif
