// Complete selected-provider reconstruction. Provenance: EXTRACTION.json.
#include "4744cea60f0e-claude_minigame_t3_f2.hpp"
#include "fe8a6c6d9ecf-claude_minigame_t3_c1.hpp"
#include "54d1f1f44326-camera_stick.hpp"
#include "cb476bd89101-w5_physics_types.hpp"
#include <cstdint>
#include <cstring>
#include <cstddef>
extern "C" void __fastcall kotor_fastcall_0066d640(void*,void*,float,void*,void*);
extern "C" void __fastcall kotor_fastcall_0066ce60(void*,void*,std::int32_t,std::int32_t);
namespace {
template<class T>T load(const void*p,unsigned off){T v;std::memcpy(&v,static_cast<const unsigned char*>(p)+off,sizeof v);return v;}
template<class T>void store(void*p,unsigned off,T v){std::memcpy(static_cast<unsigned char*>(p)+off,&v,sizeof v);}
void words(void*out,const void*in,unsigned n){for(unsigned i=0;i<n;++i)store(out,4*i,load<std::uint32_t>(in,4*i));}
static_assert(sizeof(W4RK4State)==24 && offsetof(W4RK4State,velocity)==12,"selected RK4 layout");
static_assert(sizeof(w5::PhysicsState)==24 && offsetof(w5::PhysicsState,b)==12,"selected divide layout");
static_assert(alignof(W4RK4State)==alignof(w5::PhysicsState),"audited ABI view alignment");
// The only cross-nominal view is this retained divide seam. No arithmetic transport.
const W4RK4State* divide(W4RK4State*out,const W4RK4State*in,float factor){
 return reinterpret_cast<const W4RK4State*>(w5_physics_state_divide_006708c0(
  reinterpret_cast<w5::PhysicsState*>(out),reinterpret_cast<const w5::PhysicsState*>(in),factor));
}
void derivative(void*self,W4RK4State*out,const W4RK4State*state,const float*input){
 float acceleration[3];
 const float*result=kotor_fastcall_00670a90(self,nullptr,acceleration,reinterpret_cast<const float*>(state),input);
 words(out,result,3);words(reinterpret_cast<unsigned char*>(out)+12,state,3);
}
}
extern "C" void __fastcall kotor_fastcall_00670c30(void*self,void*,void*axis,float delta){
 auto*bytes=static_cast<unsigned char*>(self);
 auto*base=reinterpret_cast<const W4RK4State*>(bytes+0xC0);
 auto*oldInput=reinterpret_cast<const float*>(bytes+0xB0);
 float midpoint[3],roundedZ;
 const float half=0.5f;
 // Original X/Y stay extended until half multiplication; Z is first stored to float.
 __asm {
  mov eax,axis
  mov edx,oldInput
  fld dword ptr[eax]
  fadd dword ptr[edx]
  fld dword ptr[edx+4]
  fadd dword ptr[eax+4]
  fld dword ptr[edx+8]
  fadd dword ptr[eax+8]
  fstp roundedZ
  fxch st(1)
  fmul half
  fstp midpoint[0]
  fmul half
  fstp midpoint[4]
  fld roundedZ
  fmul half
  fstp midpoint[8]
 }
 W4RK4State derivativeState,k1,k2,k3,k4,scaleScratch,halfScratch,sumScratch,stage;
 derivative(self,&derivativeState,base,oldInput);
 words(&k1,kotor_cdecl_00670830(&scaleScratch,&derivativeState,delta),6);
 const W4RK4State*h=kotor_cdecl_00670830(&halfScratch,&k1,0.5f);
 words(&stage,kotor_cdecl_006707a0(&sumScratch,base,h),6);
 derivative(self,&derivativeState,&stage,midpoint);
 words(&k2,kotor_cdecl_00670830(&scaleScratch,&derivativeState,delta),6);
 h=kotor_cdecl_00670830(&halfScratch,&k2,0.5f);
 words(&stage,kotor_cdecl_006707a0(&sumScratch,base,h),6);
 derivative(self,&derivativeState,&stage,midpoint);
 words(&k3,kotor_cdecl_00670830(&scaleScratch,&derivativeState,delta),6);
 words(&stage,kotor_cdecl_006707a0(&sumScratch,base,&k3),6);
 derivative(self,&derivativeState,&stage,static_cast<const float*>(axis));
 words(&k4,kotor_cdecl_00670830(&scaleScratch,&derivativeState,delta),6);
 W4RK4State d4,d3,d2,d1,acc1,acc2,acc3,acc4;
 const W4RK4State*p4=divide(&d4,&k4,6.0f);
 const W4RK4State*p3=divide(&d3,&k3,3.0f);
 const W4RK4State*p2=divide(&d2,&k2,3.0f);
 const W4RK4State*p1=divide(&d1,&k1,6.0f);
 const W4RK4State*a=kotor_cdecl_006707a0(&acc1,base,p1);
 a=kotor_cdecl_006707a0(&acc2,a,p2);
 a=kotor_cdecl_006707a0(&acc3,a,p3);
 a=kotor_cdecl_006707a0(&acc4,a,p4);
 words(bytes+0xD8,a,6);words(bytes+0xB0,axis,3);
}
extern "C" void __fastcall kotor_fastcall_00670fb0(void*self,void*,void*axis,float delta){
 const int mode=load<int>(self,0x80);const float negativeOne=-1.0f;
 if(mode==2){__asm {
  mov eax,axis
  fld dword ptr[eax]
  fchs
  fld dword ptr[eax+8]
  fchs
  fstp dword ptr[eax]
  fstp dword ptr[eax+8]
  fld dword ptr[eax+4]
  fmul negativeOne
  fstp dword ptr[eax+4]
 }}else if(mode==1){__asm {
  mov eax,axis
  fld dword ptr[eax+4]
  fmul negativeOne
  fstp dword ptr[eax+4]
  fld dword ptr[eax+8]
  fmul negativeOne
  fstp dword ptr[eax+8]
 }}else return;
 const int selector=load<int>(self,0xAC);
 if(selector==0)kotor_fastcall_00670a10(self,nullptr,axis,delta);
 else if(selector==1)kotor_fastcall_00670c30(self,nullptr,axis,delta);
}
extern "C" void __fastcall kotor_fastcall_006710e0(void*self,void*,float delta){
 auto*bytes=static_cast<unsigned char*>(self);
 void*player=load<void*>(self,0x24);
 if(player){kotor_fastcall_0066d640(player,nullptr,delta,bytes+0xD8,bytes+0xC0);words(bytes+0xC0,bytes+0xD8,6);}
 float magnitude;std::int32_t direction,step=0;int adjust=0;
    const std::uint32_t constant_73d700=0x00000000u;
    const std::uint32_t constant_73edf0=0x3E800000u;
    const std::uint32_t constant_752910=0xBE800000u;
    const std::uint32_t constant_73d5bc=0x3E6B851Fu;
    const std::uint32_t constant_75290c=0x3E70A3D7u;
    const std::uint32_t constant_752908=0x3E2E147Bu;
    const std::uint32_t constant_752904=0x3E333333u;
    const std::uint32_t constant_73e798=0x3DCCCCCDu;
    const std::uint32_t constant_752900=0x3DD70A3Du;
    const std::uint32_t constant_7528fc=0x3D2C0831u;
    const std::uint32_t constant_7528f8=0x3D408312u;
    const std::uint32_t constant_7528f4=0x3E75C28Fu;
    const std::uint32_t constant_73e46c=0x3E4CCCCDu;
    const std::uint32_t constant_7528f0=0x3E0A3D71u;
    const std::uint32_t constant_7528ec=0x3D916873u;
    const std::uint32_t constant_7528e8=0x3CAC0831u;
 // Original arithmetic and parity decisions, including live x87 product and early90 write.
 // Absolute readonly constants are relocated local bit-identical values; no FP mode changes.
 __asm {
  mov ebx,self
        ; 00671110
        fld dword ptr [ebx + 0x8c]
        ; 00671116
        mov ecx, 1
        ; 0067111B
        fcomp dword ptr constant_73d700
        ; 00671121
        fnstsw ax
        ; 00671123
        test ah, 5
        ; 00671126
        jp control_67112d
        ; 00671128
        or edx, 0xffffffff
        ; 0067112B
        jmp control_67112f
        control_67112d:
        mov edx, ecx
        control_67112f:
        cmp dword ptr [ebx + 0x80], ecx
        ; 00671135
        mov dword ptr direction, edx
        ; 00671139
        jne control_67144e
        ; 0067113F
        fld dword ptr [ebx + 0x8c]
        ; 00671145
        fabs
        ; 00671147
        fstp dword ptr magnitude
        ; 0067114B
        fld dword ptr delta
        ; 0067114F
        fmul dword ptr [ebx + 0xb0]
        ; 00671155
        fld dword ptr [ebx + 0xb0]
        ; 0067115B
        fcomp dword ptr constant_73d700
        ; 00671161
        fnstsw ax
        ; 00671163
        test ah, 0x44
        ; 00671166
        jnp control_67122a
        ; 0067116C
        fld dword ptr [ebx + 0xb0]
        ; 00671172
        mov dword ptr [ebx + 0x90], 0
        ; 0067117C
        fcomp dword ptr constant_73d700
        ; 00671182
        fnstsw ax
        ; 00671184
        test ah, 5
        ; 00671187
        jp control_67119c
        ; 00671189
        fld dword ptr [ebx + 0x8c]
        ; 0067118F
        fcomp dword ptr constant_73d700
        ; 00671195
        fnstsw ax
        ; 00671197
        test ah, 5
        ; 0067119A
        jnp control_6711c2
        control_67119c:
        fld dword ptr [ebx + 0xb0]
        ; 006711A2
        fcomp dword ptr constant_73d700
        ; 006711A8
        fnstsw ax
        ; 006711AA
        test ah, 0x41
        ; 006711AD
        jne control_67121f
        ; 006711AF
        fld dword ptr [ebx + 0x8c]
        ; 006711B5
        fcomp dword ptr constant_73d700
        ; 006711BB
        fnstsw ax
        ; 006711BD
        test ah, 0x41
        ; 006711C0
        jne control_67121f
        control_6711c2:
        fld dword ptr magnitude
        ; 006711C6
        fcomp dword ptr constant_73edf0
        ; 006711CC
        fnstsw ax
        ; 006711CE
        test ah, 5
        ; 006711D1
        jp control_67134d
        ; 006711D7
        fadd dword ptr [ebx + 0x8c]
        ; 006711DD
        fcom dword ptr constant_73edf0
        ; 006711E3
        fst dword ptr [ebx + 0x8c]
        ; 006711E9
        fnstsw ax
        ; 006711EB
        test ah, 1
        ; 006711EE
        jne control_6711ff
        ; 006711F0
        mov dword ptr [ebx + 0x8c], 0x3e828f5c
        ; 006711FA
        jmp control_67134d
        control_6711ff:
        fcomp dword ptr constant_752910
        ; 00671205
        fnstsw ax
        ; 00671207
        test ah, 0x41
        ; 0067120A
        jp control_67134f
        ; 00671210
        mov dword ptr [ebx + 0x8c], 0xbe828f5c
        ; 0067121A
        jmp control_67134f
        control_67121f:
        fstp dword ptr [ebx + 0x8c]
        ; 00671225
        jmp control_67134f
        control_67122a:
        fstp st(0)
        ; 0067122C
        fld dword ptr [ebx + 0x8c]
        ; 00671232
        fcomp dword ptr constant_73d700
        ; 00671238
        fnstsw ax
        ; 0067123A
        test ah, 0x44
        ; 0067123D
        jnp control_67134f
        ; 00671243
        mov eax, dword ptr [ebx + 0x90]
        ; 00671249
        test eax, eax
        ; 0067124B
        jne control_671316
        ; 00671251
        fld dword ptr magnitude
        ; 00671255
        fcomp dword ptr constant_73d5bc
        ; 0067125B
        fnstsw ax
        ; 0067125D
        test ah, 1
        ; 00671260
        jne control_671282
        ; 00671262
        fld dword ptr magnitude
        ; 00671266
        fcomp dword ptr constant_75290c
        ; 0067126C
        fnstsw ax
        ; 0067126E
        test ah, 5
        ; 00671271
        jp control_671282
        ; 00671273
        fild dword ptr direction
        ; 00671277
        fmul dword ptr constant_75290c
        ; 0067127D
        jmp control_67130a
        control_671282:
        fld dword ptr magnitude
        ; 00671286
        fcomp dword ptr constant_752908
        ; 0067128C
        fnstsw ax
        ; 0067128E
        test ah, 1
        ; 00671291
        jne control_6712b0
        ; 00671293
        fld dword ptr magnitude
        ; 00671297
        fcomp dword ptr constant_752904
        ; 0067129D
        fnstsw ax
        ; 0067129F
        test ah, 5
        ; 006712A2
        jp control_6712b0
        ; 006712A4
        fild dword ptr direction
        ; 006712A8
        fmul dword ptr constant_752904
        ; 006712AE
        jmp control_67130a
        control_6712b0:
        fld dword ptr magnitude
        ; 006712B4
        fcomp dword ptr constant_73e798
        ; 006712BA
        fnstsw ax
        ; 006712BC
        test ah, 1
        ; 006712BF
        jne control_6712de
        ; 006712C1
        fld dword ptr magnitude
        ; 006712C5
        fcomp dword ptr constant_752900
        ; 006712CB
        fnstsw ax
        ; 006712CD
        test ah, 5
        ; 006712D0
        jp control_6712de
        ; 006712D2
        fild dword ptr direction
        ; 006712D6
        fmul dword ptr constant_752900
        ; 006712DC
        jmp control_67130a
        control_6712de:
        fld dword ptr magnitude
        ; 006712E2
        fcomp dword ptr constant_7528fc
        ; 006712E8
        fnstsw ax
        ; 006712EA
        test ah, 1
        ; 006712ED
        jne control_671310
        ; 006712EF
        fld dword ptr magnitude
        ; 006712F3
        fcomp dword ptr constant_7528f8
        ; 006712F9
        fnstsw ax
        ; 006712FB
        test ah, 5
        ; 006712FE
        jp control_671310
        ; 00671300
        fild dword ptr direction
        ; 00671304
        fmul dword ptr constant_7528fc
        control_67130a:
        fstp dword ptr [ebx + 0x8c]
        control_671310:
        mov dword ptr [ebx + 0x90], ecx
        control_671316:
        fild dword ptr direction
        ; 0067131A
        fmul dword ptr delta
        ; 0067131E
        fld st(0)
        ; 00671320
        fabs
        ; 00671322
        fld dword ptr [ebx + 0x8c]
        ; 00671328
        fabs
        ; 0067132A
        fcompp
        ; 0067132C
        fnstsw ax
        ; 0067132E
        test ah, 5
        ; 00671331
        jp control_67133f
        ; 00671333
        mov dword ptr [ebx + 0x8c], 0
        ; 0067133D
        jmp control_67134d
        control_67133f:
        fld dword ptr [ebx + 0x8c]
        ; 00671345
        fsub st(1)
        ; 00671347
        fstp dword ptr [ebx + 0x8c]
        control_67134d:
        fstp st(0)
        control_67134f:
        fld dword ptr [ebx + 0x8c]
        ; 00671355
        fabs
        ; 00671357
        fst dword ptr magnitude
        ; 0067135B
        fcomp dword ptr constant_73edf0
        ; 00671361
        fnstsw ax
        ; 00671363
        test ah, 1
        ; 00671366
        jne control_671372
        ; 00671368
        lea edx, [edx + edx*4]
        ; 0067136B
        shl edx, 1
        ; 0067136D
        jmp control_67143f
        control_671372:
        fld dword ptr magnitude
        ; 00671376
        fcomp dword ptr constant_7528f4
        ; 0067137C
        fnstsw ax
        ; 0067137E
        test ah, 1
        ; 00671381
        jne control_67138b
        ; 00671383
        lea edx, [edx + edx*8]
        ; 00671386
        jmp control_67143f
        control_67138b:
        fld dword ptr magnitude
        ; 0067138F
        fcomp dword ptr constant_73d5bc
        ; 00671395
        fnstsw ax
        ; 00671397
        test ah, 1
        ; 0067139A
        jne control_6713a4
        ; 0067139C
        shl edx, 3
        ; 0067139F
        jmp control_67143f
        control_6713a4:
        fld dword ptr magnitude
        ; 006713A8
        fcomp dword ptr constant_73e46c
        ; 006713AE
        fnstsw ax
        ; 006713B0
        test ah, 1
        ; 006713B3
        jne control_6713bd
        ; 006713B5
        imul edx, edx, 7
        ; 006713B8
        jmp control_67143f
        control_6713bd:
        fld dword ptr magnitude
        ; 006713C1
        fcomp dword ptr constant_752908
        ; 006713C7
        fnstsw ax
        ; 006713C9
        test ah, 1
        ; 006713CC
        jne control_6713d5
        ; 006713CE
        lea edx, [edx + edx*2]
        ; 006713D1
        shl edx, 1
        ; 006713D3
        jmp control_67143f
        control_6713d5:
        fld dword ptr magnitude
        ; 006713D9
        fcomp dword ptr constant_7528f0
        ; 006713DF
        fnstsw ax
        ; 006713E1
        test ah, 1
        ; 006713E4
        jne control_6713eb
        ; 006713E6
        lea edx, [edx + edx*4]
        ; 006713E9
        jmp control_67143f
        control_6713eb:
        fld dword ptr magnitude
        ; 006713EF
        fcomp dword ptr constant_73e798
        ; 006713F5
        fnstsw ax
        ; 006713F7
        test ah, 1
        ; 006713FA
        jne control_671401
        ; 006713FC
        shl edx, 2
        ; 006713FF
        jmp control_67143f
        control_671401:
        fld dword ptr magnitude
        ; 00671405
        fcomp dword ptr constant_7528ec
        ; 0067140B
        fnstsw ax
        ; 0067140D
        test ah, 1
        ; 00671410
        jne control_671417
        ; 00671412
        lea edx, [edx + edx*2]
        ; 00671415
        jmp control_67143f
        control_671417:
        fld dword ptr magnitude
        ; 0067141B
        fcomp dword ptr constant_7528fc
        ; 00671421
        fnstsw ax
        ; 00671423
        test ah, 1
        ; 00671426
        jne control_67142c
        ; 00671428
        add edx, edx
        ; 0067142A
        jmp control_67143f
        control_67142c:
        fld dword ptr magnitude
        ; 00671430
        fcomp dword ptr constant_7528e8
        ; 00671436
        fnstsw ax
        ; 00671438
        test ah, 1
        ; 0067143B
        je control_67143f
        ; 0067143D
        xor edx, edx
  control_67143f:
  mov step,edx
  mov adjust,1
  control_67144e:
 }
 if(adjust){player=load<void*>(self,0x24);if(player)kotor_fastcall_0066ce60(player,reinterpret_cast<void*>(step),step,0);}
}
