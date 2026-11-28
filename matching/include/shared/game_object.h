// game_object.h -- CGameObject, the root of every server and client game object
//
// One declaration for every unit (CODE_RULES.md, R1). 41 copies agree on the
// 12-byte layout. The vtable is the 28 slots the copies share; each copy's
// later slots belong to CSWSObject or CSWCObject, which append their own.
// Slot names are the ones most copies use; slots no two copies name the same
// way keep placeholders (V44-style names are open naming work).
#ifndef SHARED_GAME_OBJECT_H
#define SHARED_GAME_OBJECT_H

class CSWCObject;
class CSWSObject;
class CSWCDoor;
class CSWSDoor;
class CSWCModule;
class CSWSModule;
class CSWCArea;
class CSWSArea;
class CSWCCreature;
class CSWSCreature;
class CSWCItem;
class CSWSItem;
class CSWCTrigger;
class CSWSTrigger;
class CSWCProjectile;
class CSWSPlaceable;
class CSWCPlaceable;
class CSWSAreaOfEffectObject;
class CSWCAreaOfEffectObject;
class CSWCSoundObject;
class CSWSWaypoint;
class CSWSEncounter;
class CSWSStore;
class CSWSSoundObject;

class CGameObject {
public:
    CGameObject(unsigned char nObjectType, unsigned long nId);
    virtual ~CGameObject() {}                                          // +0x00
    virtual void SetId(unsigned long nId);                             // +0x04
    virtual void ResetUpdateTimes(unsigned long nDay, unsigned long nTime); // +0x08
    virtual CSWCObject* AsSWCObject();                                 // +0x0c
    virtual CSWSObject* AsSWSObject();                                 // +0x10
    virtual CSWCDoor* AsSWCDoor();                                     // +0x14
    virtual CSWSDoor* AsSWSDoor();                                     // +0x18
    virtual CSWCModule* AsSWCModule();                                 // +0x1c
    virtual CSWSModule* AsSWSModule();                                 // +0x20
    virtual CSWCArea* AsSWCArea();                                     // +0x24
    virtual CSWSArea* AsSWSArea();                                     // +0x28
    virtual CSWCCreature* AsSWCCreature();                             // +0x2c
    virtual CSWSCreature* AsSWSCreature();                             // +0x30
    virtual CSWCItem* AsSWCItem();                                     // +0x34
    virtual CSWSItem* AsSWSItem();                                     // +0x38
    virtual CSWCTrigger* AsSWCTrigger();                               // +0x3c
    virtual CSWSTrigger* AsSWSTrigger();                               // +0x40
    virtual CSWCProjectile* AsSWCProjectile();                         // +0x44
    virtual CSWSPlaceable* AsSWSPlaceable();                           // +0x48
    virtual CSWCPlaceable* AsSWCPlaceable();                           // +0x4c
    virtual CSWSAreaOfEffectObject* AsSWSAreaOfEffectObject();         // +0x50
    virtual CSWCAreaOfEffectObject* AsSWCAreaOfEffectObject();         // +0x54
    virtual CSWSWaypoint* AsSWSWaypoint();                             // +0x58
    virtual CSWSEncounter* AsSWSEncounter();                           // +0x5c
    virtual void* V60();                                               // +0x60
    virtual CSWSStore* AsSWSStore();                                   // +0x64
    virtual CSWSSoundObject* AsSWSSoundObject();                       // +0x68
    virtual CSWCSoundObject* AsSWCSoundObject();                       // +0x6c

    unsigned long GetId() { return m_idSelf; }

    unsigned long m_idSelf;                                            // +0x04
    unsigned char m_nObjectType;                                       // +0x08
};

#endif
