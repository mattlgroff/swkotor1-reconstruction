// Part::Project (aurmodelrender.o): empty virtual, vtable slot +0x28 of the
// light-manager/part vtables (ret 0x10).
struct Vector { float x,y,z; };
struct Plane;
class CAurShadowData;
class Part {
public:
    virtual void Project(const Vector& v,float f,const Plane& p,CAurShadowData* pShadow);
};

// FUNCTION: SWKOTOR 0x0063CFC0
void Part::Project(const Vector&,float,const Plane&,CAurShadowData*)
{
}
