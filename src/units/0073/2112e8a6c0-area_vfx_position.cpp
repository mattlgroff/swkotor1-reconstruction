// Full original00690B00. All arithmetic at original x87 widths; real scene/model providers retained.
#include "32a11cddfd-area_vfx_native.hpp"
using namespace kotor_w3;
struct AreaVfxQuaternion { float w,x,y,z; };
struct V3W { unsigned x,y,z; };   // a Vector3 as its three words: the original passes / copies vectors with `mov` (clang would expand a by-value float struct into fld/fstp dword)
template<class F,class G> __forceinline F word_form(G* callee) { void* volatile address=reinterpret_cast<void*>(callee); return reinterpret_cast<F>(address); }
// A float the original stores (fstp dword) to a stack slot and uses again after calls: one volatile store, read back as its word (no 4-byte x87 spill).
struct StoredFloat { volatile float f; __forceinline unsigned word() const { return *reinterpret_cast<const volatile unsigned*>(&f); } };
extern "C" {
AreaVfxQuaternion* __cdecl rotation_arc_004ab630(AreaVfxQuaternion*,Vector3,Vector3);
double __cdecl quaternion_yaw_004a9f40(const AreaVfxQuaternion*);
AreaVfxQuaternion* __cdecl yaw_pitch_roll_004acac0(AreaVfxQuaternion*,float,float,float);
AreaVfxQuaternion* __cdecl quaternion_multiply_004a9ac0(AreaVfxQuaternion*,const AreaVfxQuaternion*,const AreaVfxQuaternion*);
void* __fastcall aurora_hit_info_construct_004578f0(void*,void*,U,U,U);
void __fastcall chit_info_destruct_00456b30(void*,void*);
void* __fastcall area_vfx_hitcheck_list_005ed8c0(void*,void*);
}
namespace {
// a2 (L3 t1): the inline-asm helpers are C++ under the writing rule (register values long double; one (float) per
// original fst/fstp dword). 00690BCB..00690C2C: direction = player - position, each component stored as a float; the
// squared length keeps the first x component as the stored float (reloaded, 00690C05) and y / z unrounded (ST0 kept).
bool player_direction(const Vector3* player,const Vector3* position,Vector3* out) {
    const long double dz = static_cast<long double>(player->z) - position->z;     // fld [ecx+8] ; fsub [edx+8]
    const long double dy = static_cast<long double>(player->y) - position->y;     // fld [ecx+4] ; fsub [edx+4]
    const long double dx = static_cast<long double>(player->x) - position->x;     // fld [ecx] ; fsub [edx]
    out->x = static_cast<float>(dx);                                              // 00690BE3 fstp dword [out]
    out->y = static_cast<float>(dy);                                              // 00690BEF fst dword [out+4] (ST0 kept)
    out->z = static_cast<float>(dz);                                              // 00690BFD fld st(1) ; fstp dword [out+8]
    const long double sx = static_cast<long double>(out->x);                      // 00690C05 fld dword [out] (the stored x)
    const long double length2 = (sx * sx + dy * dy) + dz * dz;                    // 00690C0D..00690C1B
    return length2 > static_cast<long double>(fbits(0x3727c5ac));                 // fcomp [00747504] ; test ah,41h ; jne -> skip
}
struct HitLocal {
    unsigned char bytes[0x64];
    HitLocal() { aurora_hit_info_construct_004578f0(bytes,nullptr,0,0xbfffffffu,0); }
    ~HitLocal() noexcept(false) { chit_info_destruct_00456b30(bytes,nullptr); }
};
}
extern "C" void* __fastcall area_vfx_hitcheck_list_005ed8c0(void* app,void*) { return rp(rp(app,4),0xa4); }
extern "C" void __fastcall area_vfx_set_position_00690b00(void* self,void*,const Vector3* position) {
    void* sound=rp(self,0xfc);
    if(sound!=nullptr)kotor_m15_sound_source_set_position_005d59e0(sound,nullptr,position,1.5f);
    area_vfx_base_set_position_0063f340(self,nullptr,position);
    AreaVfxQuaternion rotation{1.0f,0.0f,0.0f,0.0f};
    const U effect=ld<U>(self,0x108);
    if(effect==39 || effect==40) {
        if(client_get_player_creature_005ed540(client_app(),nullptr)!=nullptr) {
            void* player=client_get_player_creature_005ed540(client_app(),nullptr);
            Vector3 player_position,direction;
            std::memcpy(&player_position,at(player,0x24),12);
            if(player_direction(&player_position,position,&direction)) {
                Vector3 normalized;
                const Vector3* n=vector_normalized_004ab180(&normalized,&direction);
                const V3W forward{0u,0x3f800000u,0u};                           // 00690C6D..00690C98 mov dword 0 / 3f800000 / 0
                V3W direction_words; std::memcpy(&direction_words,n,12);        // 00690C58..00690C67 mov x3
                AreaVfxQuaternion arc;
                rotation=*word_form<AreaVfxQuaternion*(__cdecl*)(AreaVfxQuaternion*,V3W,V3W)>(&rotation_arc_004ab630)(&arc,forward,direction_words);
                AreaVfxQuaternion yaw;
                rotation=*yaw_pitch_roll_004acac0(&yaw,static_cast<float>(quaternion_yaw_004a9f40(&rotation)),0.0f,0.0f); // 00690CCC call Yaw ; 00690CD1 fstp dword [esp] (the yaw argument)
            }
        }
        vslot<U(__fastcall*)(void*,void*,U)>(self,0xa8)(self,nullptr,3);
    }
    if(ld<U>(self,0x120)!=0) {
        void* module=kotor_fastcall_005ed530(client_app(),nullptr);
        void* scene=rp(rp(module,0x48),0x184);
        if(scene!=nullptr) {
            HitLocal hit;
            Vector3 hit_position{0.0f,0.0f,0.0f};
            st<U>(hit.bytes,0,1);
            st<void*>(hit.bytes,4,area_vfx_hitcheck_list_005ed8c0(client_app(),nullptr));
            V3W start,end;
            std::memcpy(&start,position,12);std::memcpy(&end,position,12);
            const float height=10.0f;                                      // 007455C8
            StoredFloat start_z,end_z;
            start_z.f=static_cast<float>(static_cast<long double>(position->z)+height); // 00690D94..00690DA8: fld [..+8] ; fadd [007455C8] ; fstp dword
            end_z.f=static_cast<float>(static_cast<long double>(position->z)-height);   // 00690DB5..00690DC9: fld ; fsub [007455C8] ; fstp dword
            start.z=start_z.word(); end.z=end_z.word();
            if(vslot<U(__fastcall*)(void*,void*,V3W,V3W,Vector3*,void*)>(scene,0x38)(scene,nullptr,start,end,&hit_position,hit.bytes)!=0) {
                V3W normal;std::memcpy(&normal,at(hit.bytes,0x40),12);
                const V3W up{0u,0u,0x3f800000u};                            // 00690E1F..00690E52
                AreaVfxQuaternion arc,combined;
                const AreaVfxQuaternion* a=word_form<AreaVfxQuaternion*(__cdecl*)(AreaVfxQuaternion*,V3W,V3W)>(&rotation_arc_004ab630)(&arc,up,normal);
                rotation=*quaternion_multiply_004a9ac0(&combined,a,&rotation);
            }
        }
    }
    void* model=rp(self,0x68);
    if(model!=nullptr)vslot<U(__fastcall*)(void*,void*,const AreaVfxQuaternion*)>(model,0x60)(model,nullptr,&rotation);
}
