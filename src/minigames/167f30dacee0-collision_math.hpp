#pragma once
// Actual collision arithmetic kernels: no calls and no FP environment changes.
namespace t2_collision {
static __declspec(noinline) void subtract(V* out,const V* a,const V* b){
 __asm {mov eax,a
 mov ecx,b
 mov edx,out
 fld dword ptr[eax]
 fsub dword ptr[ecx]
 fstp dword ptr[edx]
 fld dword ptr[eax+4]
 fsub dword ptr[ecx+4]
 fstp dword ptr[edx+4]
 fld dword ptr[eax+8]
 fsub dword ptr[ecx+8]
 fstp dword ptr[edx+8]}
}
static __declspec(noinline) float plane_distance(const V* p,const V* n){float out;
 __asm {mov eax,p
 mov ecx,n
 fld dword ptr[eax+8]
 fmul dword ptr[ecx+8]
 fld dword ptr[eax+4]
 fmul dword ptr[ecx+4]
 faddp st(1),st(0)
 fld dword ptr[eax]
 fmul dword ptr[ecx]
 faddp st(1),st(0)
 fchs
 fstp out}
 return out;
}
static __declspec(noinline) float radius(float other,const float* player){float out;
 __asm {mov eax,player
 fld other
 fadd dword ptr[eax]
 fstp out}
 return out;
}
static __declspec(noinline) float separation(float other,const float* player){const float tenth=0.10000000149011612f;float out;
 __asm {mov eax,player
 fld other
 fadd dword ptr[eax]
 fadd tenth
 fstp out}
 return out;
}
static __declspec(noinline) void correction(V* out,const V* unit,float scale,const V* projected,const V* player){float zproduct,xpoint;
 __asm {mov eax,unit
 fld scale
 fmul dword ptr[eax]
 fld scale
 fmul dword ptr[eax+4]
 fld scale
 fmul dword ptr[eax+8]
 fstp zproduct
 fxch st(1)
 mov ecx,projected
 fadd dword ptr[ecx]
 fstp xpoint
 fadd dword ptr[ecx+4]
 fld zproduct
 fadd dword ptr[ecx+8]
 fld xpoint
 mov eax,player
 mov edx,out
 fsub dword ptr[eax]
 fstp dword ptr[edx]
 fxch st(1)
 fsub dword ptr[eax+4]
 fstp dword ptr[edx+4]
 fsub dword ptr[eax+8]
 fstp dword ptr[edx+8]}
}
static __declspec(noinline) float count_float(I count){float out;__asm{fild count
 fstp out}return out;}
static __declspec(noinline) void offset_sum(V* out,const V* average,const V* old){
 __asm {mov eax,average
 mov ecx,old
 mov edx,out
 fld dword ptr[eax]
 fadd dword ptr[ecx]
 fstp dword ptr[edx]
 fld dword ptr[eax+4]
 fadd dword ptr[ecx+4]
 fstp dword ptr[edx+4]
 fld dword ptr[eax+8]
 fadd dword ptr[ecx+8]
 fstp dword ptr[edx+8]}
}
static __declspec(noinline) void new_position(V* out,const V* offset_,const V* position,const V* old){float zpoint;
 __asm {mov eax,offset_
 mov ecx,position
 fld dword ptr[eax]
 fadd dword ptr[ecx]
 fld dword ptr[eax+4]
 fadd dword ptr[ecx+4]
 fld dword ptr[eax+8]
 fadd dword ptr[ecx+8]
 fstp zpoint
 fxch st(1)
 mov ecx,old
 mov edx,out
 fsub dword ptr[ecx]
 fstp dword ptr[edx]
 fsub dword ptr[ecx+4]
 fstp dword ptr[edx+4]
 fld zpoint
 fsub dword ptr[ecx+8]
 fstp dword ptr[edx+8]}
}
static __declspec(noinline) bool cooldown_zero(const float* value){const float zero=0;unsigned short sw;
 __asm {mov ecx,value
 fld dword ptr[ecx]
 fcomp zero
 fnstsw ax
 mov sw,ax}
 // TEST AH,44h; JP skips: only ordered equal (one bit set) proceeds.
 return (sw&0x4400)==0x4000;
}
static __declspec(noinline) float load_float(const float* value){float out;__asm{mov eax,value
 fld dword ptr[eax]
 fstp out}return out;}
static __declspec(noinline) float track_speed(float speed){const float factor=0.009999999776482582f;float out;__asm{fld speed
 fmul factor
 fstp out}return out;}
}

