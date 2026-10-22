// CAurObject destructor (the SafePointer list destructor stays
// out of line: its body is not visible in this unit).
#define G_SAFE_POINTER_LIST_DECL_ONLY
#include "g/aur_texture.h"

// vtable 0x00741108
class CAurObject : public SafePointerTarget<CAurObject> {
public:
    virtual ~CAurObject();
};

// FUNCTION: SWKOTOR 0x004488c0
CAurObject::~CAurObject()
{
}

// FUNCTION: SWKOTOR 0x004488d0 SYMBOL: ??_GCAurObject@@UAEPAXI@Z
