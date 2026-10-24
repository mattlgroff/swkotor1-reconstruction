// Exact body subsets reused from historical PI donors; see EXTRACTION.json.
#include "fe8a6c6d9ecf-claude_minigame_t3_c1.hpp"
#include <cstring>
#include <cstdint>
namespace {
struct MiniVector3 { float x,y,z; };
struct SpringPTerms { double kpx; float kpy,kpz; };
template<class T> T ld(const void* base,std::uint32_t off) {
    T value; std::memcpy(&value,static_cast<const unsigned char*>(base)+off,sizeof value);return value;
}
template<class T> void st(void* base,std::uint32_t off,T value) {
    std::memcpy(static_cast<unsigned char*>(base)+off,&value,sizeof value);
}
__declspec(noinline) SpringPTerms spring_p_terms(
        double gain, double mass, const MiniVector3* p) {
    const double k = gain / mass;                                                       // fld gain1; fdiv mass
    SpringPTerms t;
    t.kpx = k * p->x;                                                                   // kept extended/double
    t.kpy = static_cast<float>(k * p->y);                                               // fstp float
    t.kpz = static_cast<float>(k * p->z);                                               // fstp float
    return t;
}

__declspec(noinline) void spring_q_finish(
        MiniVector3* out, double gain, const MiniVector3* q, const SpringPTerms& t) {
    const float gqx = static_cast<float>(gain * q->x);
    const float gqy = static_cast<float>(gain * q->y);
    const float gqz = static_cast<float>(gain * q->z);
    out->x = static_cast<float>(static_cast<double>(gqx) - t.kpx);
    out->y = static_cast<float>(static_cast<double>(gqy) - t.kpy);
    out->z = static_cast<float>(static_cast<double>(gqz) - t.kpz);
}
}

void __fastcall kotor_fastcall_00670a10(void* mini, void*, void* axis_ptr, float delta) {
    unsigned char* self = static_cast<unsigned char*>(mini);
    const float* axis = static_cast<const float*>(axis_ptr);

    const float s = ld<float>(self, 0x74);
    // Original x87 shape:
    //   x: (s*axis.x) stays extended through the delta multiply;
    //   y: (s*axis.y) is stored float, then multiplied by delta extended;
    //   z: (s*axis.z) is stored float, the delta product is stored float too.
    // Old accumulators are +0xCC/+0xD0/+0xD4; new are +0xE4/+0xE8/+0xEC.
    const double sx = static_cast<double>(s) * static_cast<double>(axis[0]);
    const float sy = static_cast<float>(static_cast<double>(s) * static_cast<double>(axis[1]));
    const float sz = static_cast<float>(static_cast<double>(s) * static_cast<double>(axis[2]));
    const double dx = sx * static_cast<double>(delta);
    const double dy = static_cast<double>(sy) * static_cast<double>(delta);
    const float dz = static_cast<float>(static_cast<double>(sz) * static_cast<double>(delta));

    const float new_x = static_cast<float>(static_cast<double>(ld<float>(self, 0xCC)) + dx);
    const float new_y = static_cast<float>(static_cast<double>(ld<float>(self, 0xD0)) + dy);
    const float new_z = static_cast<float>(static_cast<double>(ld<float>(self, 0xD4)) + static_cast<double>(dz));

    st<float>(self, 0xE4, new_x);
    st<float>(self, 0xE8, new_y);
    st<float>(self, 0xEC, new_z);
}

extern "C" __declspec(noinline) float* __fastcall kotor_fastcall_00670a90(
        void* mini, void*, float* out, const float* p, const float* v) {
    const unsigned char* base = static_cast<const unsigned char*>(mini);
    // Volatile reads are the minimal sequencing barrier: they preserve the
    // original two distinct [mini+0xBC] loads (gain1 before mass/p, gain2 after
    // p and before q) and prevent the compiler from reusing gain1 for q. They
    // are memory-ordering only; no FP state or value is forced.
    const volatile float* gain = reinterpret_cast<const volatile float*>(base + 0xBC);
    const volatile float* mass_ptr = reinterpret_cast<const volatile float*>(base + 0x74);
    const float gain1 = *gain;
    const float mass = *mass_ptr;
    const SpringPTerms t = spring_p_terms(
        static_cast<double>(gain1), static_cast<double>(mass),
        reinterpret_cast<const MiniVector3*>(p));
    const float gain2 = *gain;
    spring_q_finish(reinterpret_cast<MiniVector3*>(out),
                    static_cast<double>(gain2),
                    reinterpret_cast<const MiniVector3*>(v), t);
    return out;
}

void __fastcall kotor_fastcall_00671020(void* mini_game, void*, float x, float y, float delta) {
    unsigned char* self = static_cast<unsigned char*>(mini_game);
    const float scale = ld<float>(self, 0x74);
    const double sd = static_cast<double>(delta) * static_cast<double>(scale);
    float dx = static_cast<float>(static_cast<double>(x) * sd);
    double dy = static_cast<double>(y) * sd;

    int axis_x = ld<int>(self, 0x84);
    if (axis_x < 0) {
        dx = -dx;
        axis_x = static_cast<int>(0u - static_cast<unsigned>(axis_x));
    }
    switch (axis_x) {
        case 1: st<float>(self, 0xE4, static_cast<float>(static_cast<double>(ld<float>(self, 0xE4)) - static_cast<double>(dx))); break;
        case 2: st<float>(self, 0xE8, static_cast<float>(static_cast<double>(ld<float>(self, 0xE8)) - static_cast<double>(dx))); break;
        case 3: st<float>(self, 0xEC, static_cast<float>(static_cast<double>(ld<float>(self, 0xEC)) - static_cast<double>(dx))); break;
        default: break;
    }

    int axis_y = ld<int>(self, 0x88);
    if (axis_y < 0) {
        dy = -dy;
        axis_y = static_cast<int>(0u - static_cast<unsigned>(axis_y));
    }
    switch (axis_y) {
        case 1: st<float>(self, 0xE4, static_cast<float>(static_cast<double>(ld<float>(self, 0xE4)) - dy)); break;
        case 2: st<float>(self, 0xE8, static_cast<float>(static_cast<double>(ld<float>(self, 0xE8)) - dy)); break;
        case 3: st<float>(self, 0xEC, static_cast<float>(static_cast<double>(ld<float>(self, 0xEC)) - dy)); break;
        default: break;
    }
}
