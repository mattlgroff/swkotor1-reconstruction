// destructors of the Part subclasses and the object light
// container; the base destructors live in part.cpp, so each derived body
// calls (or tail-jumps to) them as the original does.
#include "g/part.h"

// FUNCTION: SWKOTOR 0x00445600
CAurObjectLightContainer::~CAurObjectLightContainer()
{
}

// FUNCTION: SWKOTOR 0x00445670
PartLightsaber::~PartLightsaber()
{
    m_n3bc = 0;
    m_n3ba = 0;
    m_n3b8 = 0;
}

// FUNCTION: SWKOTOR 0x004456a0
PartDanglyMesh::~PartDanglyMesh()
{
    delete m_pConstraints;
    AurTextureRelease(m_nDanglyTexture);
    m_nDanglyTexture = -1;
}

// FUNCTION: SWKOTOR 0x00445710
PartSkin::~PartSkin()
{
}

// The camera and AABB parts have compiler-generated destructors (no vptr
// store, a plain tail jump to the base); these ctors only anchor the
// vtables, ??_G and ??1 in this unit.
PartCamera::PartCamera()
{
}

PartAABB::PartAABB()
{
}

// FUNCTION: SWKOTOR 0x004477f0 SYMBOL: ??1PartCamera@@UAE@XZ
// FUNCTION: SWKOTOR 0x004478c0 SYMBOL: ??1PartAABB@@UAE@XZ

// FUNCTION: SWKOTOR 0x00447820
PartReference::~PartReference()
{
    if (m_pReferenced)
        delete m_pReferenced;
}



// FUNCTION: SWKOTOR 0x004455e0 SYMBOL: ??_GCAurObjectLightContainer@@UAEPAXI@Z
void DeleteLightContainer(CAurObjectLightContainer* p) { delete p; }
// FUNCTION: SWKOTOR 0x004477d0 SYMBOL: ??_GPartCamera@@UAEPAXI@Z
void DeletePartCamera(PartCamera* p) { delete p; }
// FUNCTION: SWKOTOR 0x00447800 SYMBOL: ??_GPartReference@@UAEPAXI@Z
void DeletePartReference(PartReference* p) { delete p; }
// FUNCTION: SWKOTOR 0x00447880 SYMBOL: ??_GPartSkin@@UAEPAXI@Z
void DeletePartSkin(PartSkin* p) { delete p; }
// FUNCTION: SWKOTOR 0x004478a0 SYMBOL: ??_GPartAABB@@UAEPAXI@Z
void DeletePartAABB(PartAABB* p) { delete p; }
// FUNCTION: SWKOTOR 0x00447960 SYMBOL: ??_GPartLightsaber@@UAEPAXI@Z
void DeletePartLightsaber(PartLightsaber* p) { delete p; }
// FUNCTION: SWKOTOR 0x00447b00 SYMBOL: ??_GPartDanglyMesh@@UAEPAXI@Z
void DeletePartDanglyMesh(PartDanglyMesh* p) { delete p; }
