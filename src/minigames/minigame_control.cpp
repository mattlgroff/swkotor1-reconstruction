#include "4c7790600889-minigame_control.hpp"
#include "da6c2cf73dcd-minigame_player_storage.hpp"
#include <cstdint>
#include <cstddef>
#include <cstring>
extern "C" float __fastcall kotor_fastcall_0066c870(void*,void*,float);
extern "C" void __fastcall kotor_fastcall_0066cb40(void*,void*,void*);
extern "C" void* __fastcall kotor_fastcall_0066cd30(void*,void*,void*,const void*,const void*,const void*,float);
extern "C" void* __cdecl quaternion_multiply_004a9ac0(void*,const void*,const void*);
extern "C" void* __cdecl quaternion_rotate_004a9bb0(void*,const void*,const void*);
extern "C" void* __fastcall vector_add_assign_004a9740(void*,void*,const void*);
namespace {
struct Vec3 {float x,y,z;};
struct Quat {float w,x,y,z;};
struct HitEntry {float t;Vec3 p;Quat q;std::uint8_t f20,f21;std::uint16_t pad;};
static_assert(sizeof(HitEntry)==36,"original key record");
template<class T>T ld(const void* p,unsigned o){T v;std::memcpy(&v,static_cast<const unsigned char*>(p)+o,sizeof v);return v;}
template<class T>void st(void* p,unsigned o,T v){std::memcpy(static_cast<unsigned char*>(p)+o,&v,sizeof v);}
void copy_words(void* d,const void* s,unsigned bytes){for(unsigned o=0;o<bytes;o+=4)st(d,o,ld<std::uint32_t>(s,o));}
__declspec(noinline) unsigned cmp87(float a,float b){unsigned short s;__asm {
 fld a
 fcomp b
 fnstsw s
 } return s;}
bool eq87(float a,float b){return (cmp87(a,b)&0x4400u)==0x4000u;}
bool lt87(float a,float b){return (cmp87(a,b)&0x4500u)==0x0100u;}
bool le87(float a,float b){const unsigned s=cmp87(a,b)&0x4500u;return s==0x0100u||s==0x4000u;}
__declspec(noinline) float add87(float a,float b){float r;__asm {
 fld a
 fadd b
 fstp r
 }return r;}
__declspec(noinline) float sub87(float a,float b){float r;__asm {
 fld a
 fsub b
 fstp r
 }return r;}
// Compare an extended drain sum, not its rounded float copy. No CALL in asm.
__declspec(noinline) bool drain87(float previous,float value,float last,bool has_output,float* rounded,float* adjusted){
 unsigned short status;
 __asm {
  fld previous
  fadd value
  fcom last
  mov eax,rounded
  fst dword ptr [eax]
  fnstsw status
 }
 const bool duplicate=(status&0x4400u)==0x4000u&&has_output;
 if(duplicate){__asm{fstp st(0)}}
 else {__asm {
 fsub last
 mov eax,adjusted
 fstp dword ptr[eax]
 }}
 return duplicate;
}
__declspec(noinline) void speed_step(void* player,float dt){
 __asm {
  mov edx,player
  fld dword ptr[edx+98h]
  fcomp dword ptr[edx+1DCh]
  fld dt
  fmul dword ptr[edx+1E0h]
  fnstsw ax
  test ah,5
  jp decrease
  fadd dword ptr[edx+98h]
  fcom dword ptr[edx+1DCh]
  fnstsw ax
  test ah,5
  jnp store_speed
  jmp use_target
 decrease:
  fsubr dword ptr[edx+98h]
  fcom dword ptr[edx+1DCh]
  fnstsw ax
  test ah,41h
  je store_speed
 use_target:
  fstp st(0)
  fld dword ptr[edx+1DCh]
 store_speed:
  fstp dword ptr[edx+98h]
 }
}
using VtOne=void*(__fastcall*)(void*,void*,void*);
using VtTwo=std::uint32_t(__fastcall*)(void*,void*,void*,void*);
using VtInt=void*(__fastcall*)(void*,void*,int);
using VtStr=void*(__fastcall*)(void*,void*,const char*);
using VtType=int(__fastcall*)(void*,void*);
void vt_one(void* p,unsigned o,void* a){ld<VtOne>(ld<void*>(p,0),o)(p,nullptr,a);}
void vt_two(void* p,unsigned o,void* a,void* b){ld<VtTwo>(ld<void*>(p,0),o)(p,nullptr,a,b);}
void* vt_get_obj_int(void* p,unsigned o,int a){return ld<VtInt>(ld<void*>(p,0),o)(p,nullptr,a);}
void* vt_get_obj_str(void* p,unsigned o,const char* a){return ld<VtStr>(ld<void*>(p,0),o)(p,nullptr,a);}
int vt_get_type(void* p){return ld<VtType>(ld<void*>(p,0),0x10)(p,nullptr);}
int dec_wrap(int v){const auto u=static_cast<std::uint32_t>(v)-1u;int r;std::memcpy(&r,&u,4);return r;}

HitEntry* key_at(void* self,unsigned field,int index){
 const auto base=reinterpret_cast<std::uintptr_t>(ld<void*>(self,field));
 return reinterpret_cast<HitEntry*>(base+static_cast<std::uint32_t>(index)*36u);
}
void put_output(void* self,int index,HitEntry* entry){
 const auto base=reinterpret_cast<std::uintptr_t>(ld<void*>(self,0x1B8));
 st(reinterpret_cast<void*>(base+static_cast<std::uint32_t>(index)*4u),0,entry);
}
int merge_events_0066d640(void* self, int countA, int countB) {
    const int lastA = dec_wrap(countA);
    const int lastB = dec_wrap(countB);
    int ai = 0;
    int bi = 0;
    int oi = 0;
    float prevA = 0.0f;
    float prevB = 0.0f;
    float L = 0.0f;

    // Drain B with the original extended-compare/float-store asymmetry.
    auto drain_b = [&]() {
        while (bi < lastB) {
            HitEntry* const entry_b = key_at(self,0x1AC,bi);
            float bnew_f, adjusted;
            if (drain87(prevB, entry_b->t, L, oi != 0, &bnew_f, &adjusted)) {
                prevB = bnew_f;
                put_output(self,oi - 1,entry_b); // duplicate overwrites last slot
            } else {
                entry_b->t = adjusted;
                put_output(self,oi++,entry_b);
                L = bnew_f;
            }
            ++bi;
        }
    };

    // Original 0066D7DC-0066D7E6: the main merge loop is entered only when
    // countA-1 > 0. For countA 0/1, go straight to the drain-B block and never
    // read A[0]; the final selection handles A[-1]/A[0] later.
    if (lastA <= 0) {
        drain_b();
        return oi;
    }

    for (;;) {
        if (!(bi < lastB)) {
            // Drain A. The drain compares the extended sum with L directly
            // (unlike the main loop, which stores/reloads float first), then
            // rounds only the stored copy/prevA.
            while (ai < lastA) {
                HitEntry* const entry_a = key_at(self,0x1A0,ai);
                float anew_f, adjusted;
                if (drain87(prevA, entry_a->t, L, oi != 0, &anew_f, &adjusted)) {
                    prevA = anew_f;
                    put_output(self,oi - 1,entry_a); // duplicate overwrites last slot
                } else {
                    entry_a->t = adjusted;
                    put_output(self,oi++,entry_a);
                    L = anew_f;
                }
                ++ai;
            }
            break;
        }

        HitEntry* const entry_a = key_at(self,0x1A0,ai);
        const float anew = add87(prevA, entry_a->t);
        HitEntry* const entry_b = key_at(self,0x1AC,bi);
        const float bnew = add87(prevB, entry_b->t);

        if (le87(anew, bnew)) { // ordered <=; unordered goes to B
            if (eq87(anew, L) && oi != 0) {
                prevA = anew;
                put_output(self,oi - 1,entry_a); // duplicate overwrites last slot
            } else {
                entry_a->t = sub87(anew, L);
                put_output(self,oi++,entry_a);
                L = anew;
            }
            ++ai;
        } else {
            if (eq87(bnew, L) && oi != 0) {
                prevB = bnew;
                put_output(self,oi - 1,entry_b); // duplicate overwrites last slot
            } else {
                entry_b->t = sub87(bnew, L);
                put_output(self,oi++,entry_b);
                L = bnew;
            }
            ++bi;
        }

        if (!(ai < lastA)) {
            drain_b();
            break;
        }
    }
    return oi;
}


const HitEntry* hit_at(const HitEntry* base,int index){
 const std::uintptr_t address=reinterpret_cast<std::uintptr_t>(base)+static_cast<std::uint32_t>(index)*36u;return reinterpret_cast<const HitEntry*>(address);
}
}
extern "C" void __fastcall kotor_fastcall_0066d640(void* player, void*, float delta, void* src_ptr, void* dst_ptr) {
    unsigned char* self = static_cast<unsigned char*>(player);
    float* src = static_cast<float*>(src_ptr);
    float* dst = static_cast<float*>(dst_ptr);

    // diff2 = src.second - dst.second (3 floats, one float store each).
    float diff2[3];
    for (int i = 0; i < 3; ++i) {
        diff2[i] = sub87(src[3 + i], dst[3 + i]);
    }

    // Speed approach block. The equality test is ordered; unordered executes.
    if (ld<std::uint8_t>(self, 0x1D4) == 0 && g_mini_007a248c != 0) {
        const float speed = ld<float>(self, 0x98);
        const float target = ld<float>(self, 0x1DC);
        if (!eq87(speed, target)) {
            speed_step(self, delta);
            const float next = ld<float>(self, 0x98);
            (void)kotor_fastcall_0066c870(self, nullptr, next);
        }
    }
    st<std::uint8_t>(self, 0x1D4, 0);

    // Global notification. Ordered equality skips; unordered executes. The
    // original reloads 007A2490 for the call and again for the store.
    const float notification = *static_cast<volatile float*>(&g_mini_007a2490);
    const float notified = *static_cast<volatile float*>(&g_mini_007a2494);
    if (!eq87(notification, notified)) {
        const float call_value = *static_cast<volatile float*>(&g_mini_007a2490);
        (void)kotor_fastcall_0066c870(self, nullptr, call_value);
        g_mini_007a2494 = *static_cast<volatile float*>(&g_mini_007a2490);
    }

    void* track = ld<void*>(self, 0x60);
    if (ld<std::uint8_t>(track, 0x94) != 0) {
        if ((ld<std::uint8_t>(self, 0x88) & 1u) != 0) {
            const int count_a = ld<int>(self, 0x1A4);
            const int count_b = ld<int>(self, 0x1B0);
            int last_a = count_a, last_b = count_b;
            Vec3 pos = ld<Vec3>(self, 0x234);
            Vec3 pos_copy = pos;
            Vec3 scratch_a;

            if (count_a != 0 || count_b != 0) {
                last_a = dec_wrap(count_a); last_b = dec_wrap(count_b);

                void* query = ld<void*>(self, 0x64);
                Vec3 query_vec;
                Quat track_quat;
                vt_one(query, 0x64, &query_vec);   // original J+0x4C scratch
                vt_one(ld<void*>(self,0x64), 0x68, &track_quat);  // original J+0x64 quaternion

                const int out_count = merge_events_0066d640(self, count_a, count_b);

                for (int i = 0; i < out_count; ++i) {
                    HitEntry* entry = ld<HitEntry**>(self, 0x1B8)[i];
                    if (entry->f21 != 0) {
                        Quat q;
                        const void* returned = quaternion_multiply_004a9ac0(&q, &entry->q, &track_quat);
                        copy_words(self + 0x240, returned, sizeof(q));
                    }
                    if (entry->f20 != 0) {
                        Vec3 rotated;
                        const void* returned = quaternion_rotate_004a9bb0(&rotated, &track_quat, &entry->p);
                        Vec3 next_pos;
                        // Original position base is the vt[0x64] scratch
                        // (J+0x4C), not diff2 (J+0x58).
                        next_pos.x = add87(query_vec.x, ld<float>(returned, 0));
                        next_pos.y = add87(query_vec.y, ld<float>(returned, 4));
                        next_pos.z = add87(query_vec.z, ld<float>(returned, 8));
                        copy_words(self + 0x234, &next_pos, sizeof(next_pos));
                    }
                    if (!eq87(entry->t, 0.0f)) { // unordered executes
                        void* response = kotor_fastcall_0066cd30(
                            self, nullptr, &scratch_a, &pos_copy,
                            self + 0x234, self + 0x240, entry->t);
                        (void)vector_add_assign_004a9740(src, nullptr, response);
                    }
                    pos_copy = ld<Vec3>(self, 0x234);
                }
            }

            // Track get/set position+orientation.
            void* query = ld<void*>(self, 0x64);
            void* handle = vt_get_obj_str(query, 0x10C, "modelhook");
            vt_two(handle, 0x0C, self + 0x234, self + 0x240);

            // Final collision. If both last indices are 0 (counts 1/1), the
            // original skips the A/B compare but still calls 0066CD30 with
            // t = arg1 (delta). Otherwise it compares A[lastA].t and
            // B[lastB].t; equal/unordered chooses B. Both zero counts skip the decrement/array read entirely.
            // Mixed zero/nonzero counts retain original unchecked negative indices.
            float t = delta;
            if (last_a != 0 || last_b != 0) {
                HitEntry* a = static_cast<HitEntry*>(ld<void*>(self, 0x1A0));
                HitEntry* b = static_cast<HitEntry*>(ld<void*>(self, 0x1AC));
                const HitEntry* ea = hit_at(a, last_a);
                const HitEntry* eb = hit_at(b, last_b);
                t = lt87(ea->t, eb->t) ? ea->t : eb->t; // equal/unordered -> B
            }
            {
                void* response = kotor_fastcall_0066cd30(
                    self, nullptr, &scratch_a, &pos_copy,
                    self + 0x234, self + 0x240, t);
                (void)vector_add_assign_004a9740(src, nullptr, response);
            }
        }

        // Cleared for both the flags-bit-0 path and the post-track path.
        st<int>(self, 0x1A4, 0);
        st<int>(self, 0x1B0, 0);
    }

    // self+0x1C4 += diff2; clamp in place.
    (void)vector_add_assign_004a9740(self + 0x1C4, nullptr, diff2);
    kotor_fastcall_0066cb40(self, nullptr, self + 0x1C4);

    // Linked object position updates.
    const int object_count = ld<int>(self, 0x6C);
    for (int i = 0; i < object_count; ++i) {
        unsigned char* entries = static_cast<unsigned char*>(ld<void*>(self, 0x68));
        void* object = *reinterpret_cast<void**>(entries + static_cast<std::ptrdiff_t>(i) * 8);
        const std::uint8_t active = *(entries + static_cast<std::ptrdiff_t>(i) * 8 + 4);
        if (active == 0) {
            continue;
        }
        void* resolved = vt_get_obj_int(object, 0x108, -1);
        if (resolved == nullptr) {
            continue;
        }
        const int type = vt_get_type(resolved);
        if (type == 0x3EA || type == 0x3EB) {
            copy_words(static_cast<unsigned char*>(resolved) + 0x3C, self + 0x1C4, sizeof(Vec3));
        }
    }

    void* extra = ld<void*>(self, 0x220);
    if (extra != nullptr) {
        void* resolved = vt_get_obj_int(extra, 0x108, -1);
        if (resolved != nullptr && vt_get_type(resolved) == 0x3EB) {
            copy_words(static_cast<unsigned char*>(resolved) + 0x3C, self + 0x1C4, sizeof(Vec3));
        }
    }
}
