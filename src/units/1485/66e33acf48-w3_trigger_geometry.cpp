#include "a43a781a46-w3_trigger_internal.h"
using namespace trigger_private;
extern "C" U __fastcall w5_area_intersect_line_segments_0058f840(void*,void*,float,float,float,float,float,float,float,float,float*,float*);
namespace {
// a2 (L3 t1): the inline-asm helpers are C++ under the writing rule (register values long double, one (float) per
// original fstp dword). The float arguments of IntersectLineSegments are the words the original pushes with `mov`
// (00691A82..00691AA6); a word-typed form of the callee passes the same stack bytes (clang would expand float
// parameters with fld/fstp dword).
template <class F, class G> __forceinline F word_form(G* callee) {
    void* volatile address = reinterpret_cast<void*>(callee);
    return reinterpret_cast<F>(address);
}
__forceinline long double ld_word(U bits) { float f; std::memcpy(&f, &bits, 4); return f; }   // fld dword
__forceinline void store_float(void* where, long double value) { const float f = static_cast<float>(value); std::memcpy(where, &f, 4); }   // fstp dword

// 00691560 center (0069165B..006916C0): the extents are max - min in the x87 registers; x is stored as (float)(dx * 0.5)
// and z as (float)dz (the original reloads both stored floats), y keeps the unrounded dy * 0.5; each component then adds
// the minimum and is stored as a float.
void center(const U* minimum, const U* maximum, void* output) {
    const U half_bits = 0x3f000000u;                                    // [half] 0.5f
    const long double half = ld_word(half_bits);
    const long double dx = ld_word(maximum[0]) - ld_word(minimum[0]);   // fld [edx] ; fsub [ecx]
    const long double dy = ld_word(maximum[1]) - ld_word(minimum[1]);   // fld [edx+4] ; fsub [ecx+4]
    const long double dz = ld_word(maximum[2]) - ld_word(minimum[2]);   // fld [edx+8] ; fsub [ecx+8]
    const float z_difference = static_cast<float>(dz);                  // fstp dword [z_difference]
    const float x_half = static_cast<float>(dx * half);                 // fmul [half] ; fstp dword [x_half]
    const long double y_half = dy * half;                               // fmul [half] (kept unrounded)
    const long double z_half = static_cast<long double>(z_difference) * half;   // fld [z_difference] ; fmul [half]
    store_float(static_cast<unsigned char*>(output) + 0, static_cast<long double>(x_half) + ld_word(minimum[0]));   // fld [x_half] ; fadd [ecx] ; fstp [eax]
    store_float(static_cast<unsigned char*>(output) + 4, y_half + ld_word(minimum[1]));                              // fadd [ecx+4] ; fstp [eax+4]
    store_float(static_cast<unsigned char*>(output) + 8, z_half + ld_word(minimum[2]));                              // fadd [ecx+8] ; fstp [eax+8]
}
}
extern "C" void __fastcall w3_trigger_00691560(void* self,void*) noexcept(false){
 void* vertices=ld<void*>(self,0x134);U minimum[3],maximum[3];
 // Original reads vertex0 even when the signed count is zero or negative.
 for(U d=0;d<3;++d){minimum[d]=ld<U>(vertices,d*4);maximum[d]=ld<U>(vertices,d*4);}
 const I count=ld<I>(self,0x130);
 for(I i=1;i<count;++i){
  for(U d=0;d<3;++d){
   void* value=at(vertices,static_cast<U>(i)*12+d*4);
   U value_bits; std::memcpy(&value_bits,value,4);
   if(ld_word(value_bits)<ld_word(minimum[d]))minimum[d]=value_bits; // fld [value] ; fcomp [minimum] ; test ah,5 ; jp: C0 alone = ordered less (NaN: both bits set, no update)
  }
  for(U d=0;d<3;++d){
   void* value=at(vertices,static_cast<U>(i)*12+d*4);
   U value_bits; std::memcpy(&value_bits,value,4);
   if(ld_word(value_bits)>ld_word(maximum[d]))maximum[d]=value_bits; // fld [value] ; fcomp [maximum] ; test ah,41h ; jne: neither C0 nor C3 = ordered greater (NaN sets both)
  }
 }
 center(minimum,maximum,at(self,0x24));
}
extern "C" U __fastcall w3_trigger_00691a30(void* self,void*,Vec* from,Vec* to,Vec* output) noexcept(false){
 void* const area=ld<void*>(self,0x20);
 if(!area)return 0;
 U index=0;U byte_offset=0;I signed_count=ld<I>(self,0x130);
 while(static_cast<I>(index)<signed_count){
  const U count=static_cast<U>(signed_count);const U following=index==count-1u?0u:index+1u;
  void* const vertices=ld<void*>(self,0x134);
  U current_x,current_y,next_x,next_y;std::memcpy(&current_x,at(vertices,byte_offset),4);std::memcpy(&current_y,at(vertices,byte_offset+4),4);std::memcpy(&next_x,at(vertices,following*12u),4);std::memcpy(&next_y,at(vertices,following*12u+4),4);
  U from_x,from_y,to_x,to_y;std::memcpy(&from_x,&from->x,4);std::memcpy(&from_y,&from->y,4);std::memcpy(&to_x,&to->x,4);std::memcpy(&to_y,&to->y,4);
  float hit_x,hit_y;
  if(word_form<U(__fastcall*)(void*,void*,U,U,U,U,U,U,U,U,float*,float*)>(&w5_area_intersect_line_segments_0058f840)(area,nullptr,from_x,from_y,to_x,to_y,current_x,current_y,next_x,next_y,&hit_x,&hit_y)==1){
   // Original x passes through FLD/FSTP (including NaN quieting); y uses MOV; z is literal0.
   output->x=hit_x;                                                   // 00691ADB fld dword [hit_x] ; 00691AE3 fstp dword [eax]
   std::memcpy(&output->y,&hit_y,4);st<U>(output,8,0);return 1;
  }
  ++index;byte_offset+=12;signed_count=ld<I>(self,0x130);
 }
 return 0;
}
