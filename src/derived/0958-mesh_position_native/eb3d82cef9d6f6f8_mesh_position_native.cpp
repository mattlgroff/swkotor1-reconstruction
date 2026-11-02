#include "aca02343a18e65e3_mesh_position.hpp"
#include <cstring>
namespace{using namespace recovered_mesh_position;void* ptr(U a){return reinterpret_cast<void*>(a);}struct Native:Ports{U read(U a)override{U v;std::memcpy(&v,ptr(a),4);return v;}void write(U a,U v)override{std::memcpy(ptr(a),&v,4);}void shift(U self,U vertex,U translation)override{
 // a2 (L3 t1): C++ under the writing rule (register values long double, one (float) conversion per original fstp dword).
 // Original (00599F13..00599F5A, the same block at 00580AF0..): x and y add the bias and then the translation in the x87
 // registers and are stored once; z is stored (tempz) after the bias add, reloaded, and stored again after the translation add.
 const float* a=static_cast<float*>(ptr(vertex));const float* bias=static_cast<float*>(ptr(self+0x2c));const float* move=static_cast<float*>(ptr(translation));
 const long double x1=static_cast<long double>(a[0])+bias[0];                 // fld [eax+edi] ; fadd [ecx+2Ch]
 const long double y1=static_cast<long double>(a[1])+bias[1];                 // fld [eax+4] ; fadd [ecx+30h]
 const long double z1=static_cast<long double>(a[2])+bias[2];                 // fld [eax+8] ; fadd [ecx+34h]
 const float tempz=static_cast<float>(z1);                                     // 00599F2B fstp dword [esp+20h]
 const float x=static_cast<float>(x1+move[0]);                                 // 00599F31..00599F33 fadd [edx] ; fstp dword
 const float y=static_cast<float>(y1+move[1]);                                 // 00599F3B..00599F3E fadd [edx+4] ; fstp dword
 const float z=static_cast<float>(static_cast<long double>(tempz)+move[2]);    // 00599F42..00599F52 fld [tempz] ; fadd [edx+8] ; fstp dword
 float* out=static_cast<float*>(ptr(vertex));
 std::memcpy(&out[0],&x,4);std::memcpy(&out[1],&y,4);std::memcpy(&out[2],&z,4);   // mov [eax],ebx ; mov [eax+4],ebx ; mov [eax+8],ebx
}};}
extern "C" void __fastcall recovered_mesh_position_00581af0(void*s,void*,void*v){Native p;recovered_mesh_position::position(reinterpret_cast<unsigned>(s),reinterpret_cast<unsigned>(v),p);}
