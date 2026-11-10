// model-side CExoArrayList members (doubling from 16).
#include "g/aur_array.h"

// Not the Part* list: every CExoArrayList<Part*> caller (Gob::EnumParts,
// Gob::CollectLights, CreateInstanceTreeR, the Part* SetSize at 0x00444fa0)
// reaches the scalar-new Allocate 0x005e03c0 and the Add at 0x00444fe0.
// 0x00438b40 is called only by MdlNode::CreateSkinNodeList (0x00438c40),
// which collects AsMdlNodeTriMesh() results.
// FUNCTION: SWKOTOR 0x00437c40 SYMBOL: ??A?$CExoArrayList@PAVMdlNodeTriMesh@@@@QAEAAPAVMdlNodeTriMesh@@H@Z
// FUNCTION: SWKOTOR 0x00437d60 SYMBOL: ?Allocate@?$CExoArrayList@PAVMdlNodeTriMesh@@@@QAEXH@Z
// FUNCTION: SWKOTOR 0x00438b40 SYMBOL: ?Add@?$CExoArrayList@PAVMdlNodeTriMesh@@@@QAEXPAVMdlNodeTriMesh@@@Z
template class CExoArrayList<MdlNodeTriMesh*>;

// FUNCTION: SWKOTOR 0x00437c50 SYMBOL: ?Allocate@?$CExoArrayList@UVector4@@@@QAEXH@Z
template class CExoArrayList<Vector4>;

// FUNCTION: SWKOTOR 0x00437cc0 SYMBOL: ?Allocate@?$CExoArrayList@VQuaternion@@@@QAEXH@Z
// FUNCTION: SWKOTOR 0x00438af0 SYMBOL: ?Add@?$CExoArrayList@VQuaternion@@@@QAEXVQuaternion@@@Z
template class CExoArrayList<Quaternion>;

// FUNCTION: SWKOTOR 0x00438a50 SYMBOL: ??4?$CExoArrayList@M@@QAEAAV0@ABV0@@Z
template class CExoArrayList<float>;

// FUNCTION: SWKOTOR 0x00438b80 SYMBOL: ??1?$CExoArrayList@V?$CExoArrayList@H@@@@QAE@XZ
template class CExoArrayList<CExoArrayList<int> >;

// FUNCTION: SWKOTOR 0x0043df60 SYMBOL: ?SetSize@?$CExoArrayList@VSafePointerTexture@@@@QAEXH@Z
template void CExoArrayList<SafePointerTexture>::SetSize(int n);
