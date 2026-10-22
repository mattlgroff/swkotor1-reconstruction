// SafePointer list teardown instantiated for CAurObject targets,
// and the destructor of the SafePointer<CAurTexture> array list.
#include "g/aur_texture.h"

// vtable 0x00741108: the scene object base (SafePointer target at +0x04).
class CAurObject : public SafePointerTarget<CAurObject> {
public:
    virtual ~CAurObject();
};

// FUNCTION: SWKOTOR 0x004487c0 SYMBOL: ??1?$SafePointerList@VCAurObject@@@@QAE@XZ
template class SafePointerList<CAurObject>;

// FUNCTION: SWKOTOR 0x00448890 SYMBOL: ??1?$CExoArrayList@V?$SafePointer@VCAurTexture@@@@@@QAE@XZ
template class CExoArrayList<SafePointer<CAurTexture> >;
