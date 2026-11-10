// The area-of-effect object allocates its instance animation bases as
// CSWCAnimBaseVisualEffect arrays; that instantiates the class's vector
// deleting destructor in this object.
#include "l/sw_c_area_of_effect.h"

// Visual-effect model animation base (vtable 0x00758B68), 0xF0 bytes.
class CSWCAnimBaseVisualEffect : public CSWCAnimBase {
public:
    CSWCAnimBaseVisualEffect();                     // 0x006E5CE0
    virtual ~CSWCAnimBaseVisualEffect();            // 0x006E5D00
};

// FUNCTION: SWKOTOR 0x006D9070 SYMBOL: ??_ECSWCAnimBaseVisualEffect@@UAEPAXI@Z

// Allocation as done by LoadEffect (0x006D8490).
CSWCAnimBase* NewVisualEffectBases(int nCount)
{
    return new CSWCAnimBaseVisualEffect[nCount];
}
