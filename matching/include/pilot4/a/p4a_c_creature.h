// Pilot 4 arm A: CSWCCreature melee animation selection (0x00613DA0).
#ifndef P4A_C_CREATURE_H
#define P4A_C_CREATURE_H

class CAnimBase {
public:
    virtual void Vfn00();
    virtual void Vfn04();
    virtual void Vfn08();
    virtual void Vfn0C();
    virtual void Vfn10();
    virtual void Vfn14();
    virtual void Vfn18();
    virtual void Vfn1C();
    virtual void Vfn20();
    virtual void Vfn24();
    virtual void Vfn28();
    virtual void Vfn2C();
    virtual void Vfn30();
    virtual void Vfn34();
    virtual void Vfn38();
    virtual void Vfn3C();
    virtual void Vfn40();
    virtual void Vfn44();
    virtual void Vfn48();
    virtual void Vfn4C();
    virtual int HasAnimation(int nAnimation);           // vtable +0x50
};

class CSWCCreature {
public:
    int GetIsSmallOrLargeModel();                       // 0x0060D320 (MODELTYPE starts with S or L)
    int GetMeleeAnimation(unsigned short nCurrent, int nWeapon, int bDual, int nCategory); // 0x00613DA0
    CAnimBase* GetAnimationBase();                      // 0x0063C070

    char pad_0x0[0x3c0];
    int m_nAnimationBase;                               // 0x3c0
};

#endif
