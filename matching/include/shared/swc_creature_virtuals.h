// swc_creature_virtuals.h -- CSWCCreatureVirtuals
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/o/sw_gui_in_game_abilities.h); the other copies' members fall on its members.
#ifndef SHARED_SWC_CREATURE_VIRTUALS_H
#define SHARED_SWC_CREATURE_VIRTUALS_H

#include "shared/exo_string.h"
#include "shared/vector.h"

class CResRef;

class CSWCCreatureVirtuals {
public:
    virtual void v000();
    virtual void v004();
    virtual void v008();
    virtual void v00C();
    virtual void v010();
    virtual void v014();
    virtual void v018();
    virtual void v01C();
    virtual void v020();
    virtual void v024();
    virtual void v028();
    virtual void v02C();
    virtual void v030();
    virtual void v034();
    virtual void v038();
    virtual void v03C();
    virtual void v040();
    virtual void v044();
    virtual void v048();
    virtual void v04C();
    virtual void v050();
    virtual void v054();
    virtual void v058();
    virtual void v05C();
    virtual void v060();
    virtual void v064();
    virtual void v068();
    virtual void v06C();
    virtual CExoString GetDisplayName();           // 0x70 (descriptive)
    virtual void v074();
    virtual void v078();
    virtual void v07C();
    virtual void v080();
    virtual void v084();
    virtual void SetOrientation(const Vector& vOrientation);     // +0x88
    virtual void SetGuiPosition(const Vector& vPosition);  // 0x8c (descriptive)
    virtual void v090();
    virtual void SetGuiScene(class CAurScene* pScene);   // 0x94 (descriptive)
    virtual void v098();
    virtual void v09C();
    virtual void v0A0();
    virtual void v0A4();
    virtual void v0A8();
    virtual void v0AC();
    virtual void v0B0();
    virtual void v0B4();
    virtual void v0B8();
    virtual void v0BC();
    virtual void v0C0();
    virtual void v0C4();
    virtual void v0C8();
    virtual void v0CC();
    virtual void v0D0();
    virtual void v0D4();
    virtual float GetModelHeight();                // 0xd8 (descriptive)
    virtual void v0DC();
    virtual void v0E0();
    virtual void v0E4();
    virtual void v0E8();
    virtual void SetPortrait(const CResRef& cPortrait);    // 0xec
    virtual void v0F0();
    virtual void SetPortraitId(unsigned short nPortraitId);  // 0xf4
    virtual void v0F8();
    virtual void v0FC();
    virtual void v100();
    virtual void v104();
    virtual void v108();
    virtual void v10C();
    virtual void v110();
    virtual void v114();
    virtual void v118();
    virtual void v11C();
    virtual void v120();
    virtual void v124();
    virtual void v128();
    virtual void v12C();
    virtual void v130();
    virtual void v134();
    virtual void v138();
    virtual void v13C();
    virtual void v140();
    virtual void v144();
    virtual CResRef GetPortrait(int bLarge);        // 0x148
};

#endif
