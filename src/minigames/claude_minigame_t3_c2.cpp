// Donor-only BumpPlane extraction; no direction implementation duplicated.
#include "b89d92983330-claude_minigame_t3_c2.hpp"
#include <cstdint>
#include <cstring>
struct Vector3; struct Quaternion; struct W4Vector;
extern "C" Vector3* __fastcall quaternion_xdir_0044f3d0(const Quaternion*,void*,Vector3*);
extern "C" W4Vector* __fastcall quaternion_ydir_0044f420(const void*,void*,W4Vector*);
extern "C" W4Vector* __fastcall quaternion_zdir_0043e250(const void*,void*,W4Vector*);
namespace {
template<class T> T ld(const void* p,unsigned off) { T v;std::memcpy(&v,static_cast<const unsigned char*>(p)+off,sizeof v);return v; }
template<class T> void st(void* p,unsigned off,T v) { std::memcpy(static_cast<unsigned char*>(p)+off,&v,sizeof v); }
}
extern "C" void* __fastcall kotor_fastcall_00670bb0(void* mini, void*, void* out,
                                         void* q) {
    unsigned char* const self = static_cast<unsigned char*>(mini);
    const int type = ld<int>(self, 0x78);

    if (type == 1) {
        (void)quaternion_xdir_0044f3d0(
            static_cast<const Quaternion*>(q), nullptr,
            static_cast<Vector3*>(out));
        return out;
    }
    if (type == 2) {
        (void)quaternion_ydir_0044f420(
            q, nullptr,
            static_cast<W4Vector*>(out));
        return out;
    }
    if (type == 3) {
        (void)quaternion_zdir_0043e250(
            q, nullptr,
            static_cast<W4Vector*>(out));
        return out;
    }

    // Default: exactly three 32-bit zero dwords; q is untouched and unread.
    st<std::uint32_t>(out, 0, 0u);
    st<std::uint32_t>(out, 4, 0u);
    st<std::uint32_t>(out, 8, 0u);
    return out;
}
