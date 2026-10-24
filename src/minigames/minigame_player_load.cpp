#include "7314e5f55d64-player_internal.hpp"
#include "d1ffedc48c11-gff_readers.h"
#include "0ae13273b4e7-claude_turret_t1_lifecycle.hpp"
#include "5f558436913e-minigame_player_leaves.hpp"
namespace {
using namespace t2_private;
// No provider calls or floating-point environment changes inside these x87 kernels.
__declspec(noinline) bool nonnegative87(float value){
 const float zero=0;unsigned short status;
 __asm { fld value
         fcomp zero
         fnstsw ax
         mov status,ax }
 return (status&0x100)==0;
}
__declspec(noinline) void acceleration87(void* output,float minimum,float maximum,float seconds){
 const float zero=0;
 __asm {
  fld seconds
  fcomp zero
  fnstsw ax
  test ah,44h
  jp test_positive
  fld maximum
  fsub minimum
  jmp validate
 test_positive:
  fld seconds
  fcomp zero
  fnstsw ax
  test ah,41h
  jne done
  fld maximum
  fsub minimum
  fdiv seconds
 validate:
  fcom zero
  fnstsw ax
  test ah,1
  jne discard
  mov ecx,output
  fstp dword ptr [ecx]
  jmp done
 discard:
  fstp st(0)
 done:
 }
}
gui_load::Vector read_xyz(gui_load::Gff* g,gui_load::Struct* s,U* ok,const char* z,const char* y,const char* x){
 gui_load::Vector v;
 v.z=gui_load_gff_float_00411d00(g,nullptr,s,z,ok,0);
 v.y=gui_load_gff_float_00411d00(g,nullptr,s,y,ok,0);
 v.x=gui_load_gff_float_00411d00(g,nullptr,s,x,ok,0);
 return v;
}
}
extern "C" int __fastcall kotor_fastcall_006702f0(void* self,void*,void* rawGff,void* rawStruct) noexcept(false){
 using namespace t2_private;
 if(!kotor_fastcall_0066fff0(self,nullptr,rawGff,rawStruct))return 0;
 auto* g=static_cast<gui_load::Gff*>(rawGff);auto* s=static_cast<gui_load::Struct*>(rawStruct);U ok;
 float minimum=gui_load_gff_float_00411d00(g,nullptr,s,"Minimum_Speed",&ok,-1);
 if(nonnegative87(minimum))wr(self,0x1d8,minimum);
 float maximum=gui_load_gff_float_00411d00(g,nullptr,s,"Maximum_Speed",&ok,100);
 if(nonnegative87(maximum))wr(self,0x1dc,maximum);
 float seconds=gui_load_gff_float_00411d00(g,nullptr,s,"Accel_Secs",&ok,-1);
 acceleration87(at(self,0x1e0),minimum,maximum,seconds);
 auto positive=read_xyz(g,s,&ok,"TunnelZPos","TunnelYPos","TunnelXPos");copy_words(at(self,0x1e4),&positive,12);
 auto negative=read_xyz(g,s,&ok,"TunnelZNeg","TunnelYNeg","TunnelXNeg");copy_words(at(self,0x1f0),&negative,12);
 gui_load::Vector fallback{0,0,0},infinite;
 auto* returned=gui_load_gff_vector_004122a0(g,nullptr,&infinite,s,"TunnelInfinite",&ok,&fallback);
 copy_words(at(self,0x1fc),returned,12);
 auto origin=read_xyz(g,s,&ok,"Start_Offset_Z","Start_Offset_Y","Start_Offset_X");gui_load::Vector out;
 kotor_fastcall_0066cfd0(self,nullptr,&out,&origin);
 auto target=read_xyz(g,s,&ok,"Target_Offset_Z","Target_Offset_Y","Target_Offset_X");copy_words(at(self,0x214),&target,12);
 return 1;
}
extern "C" int __fastcall kotor_fastcall_006705f0(void* self,void*,void* rawGff,void* rawStruct) noexcept(false){
 if(!kotor_fastcall_0066fff0(self,nullptr,rawGff,rawStruct))return 0;
 gui_load::Word ok;
 auto trigger=gui_load_gff_byte_00411a60(static_cast<gui_load::Gff*>(rawGff),nullptr,static_cast<gui_load::Struct*>(rawStruct),"Trigger",&ok,0);
 t2_private::wr(self,0x1a0,trigger);return 1;
}
