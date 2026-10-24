#pragma once
// Texture header epoch E1 (lane-texture-epoch PROPOSAL; the integrator is the canonical type owner).
// The ONE texture::Basic definition for every consumer of the 236-byte CAurTextureBasic object:
//   texture ctor/Init/find (00423150/00422AF0/00420AC0, Main texture-scalar-epoch-r1 texture.cpp),
//   destroy/scalar-delete/FreeImage/remove (00422F40/004233C0/0041F570/00421650), FontInfo (004221C0),
//   scheduler (00421EE0/004217F0/004216A0/0041FA30), LoadImageA/GetImageWrapper (0041F6A0/0041EA20),
//   future glImage/CreateProcessedTexture (00420970/00424DD0).
// Supersedes (must not coexist in one image):
//   R/task-33/texture-lookup-init/source/texture.hpp (2a68c665...), R/task-33/font-growth-join/source/texture.hpp
//   (137c99d7...), S/task/texture-scalar-epoch-r1/texture.hpp (4640303b...), and the type block of
//   S/task-6/image-acquisition-r1/source/abi_proposal.hpp (76a42dc2...). The scheduler's private vtable
//   carriers are replaced by the shared carriers below (texture_x86.asm).
// Field layout = Main candidate epoch (f20 nonvolatile float, f5c..f74 nonvolatile int32); every other
// field keeps the donor type and volatility.
#include "af0b40299bf1-font.hpp" // 6bd3ba97: void allocate_list (task-35 005E03C0 join)
#include "4f4bcdec53ed-allocator_live.h" // 4f4bcdec; header_root = M40 canonical-inputs/dependencies/reconstructed/task-23/sbh-slice2/live
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

namespace texture {
using U=uint32_t;using I=int32_t;using Ref=font::Texture;using List=font::List;
// CAurFontInfo (48 bytes; swkotor.exe.h recovered type; parse 00422210: 004635E0 int -> +00, 004635C0 "%f" ->
// +04..+14 (004222A9 +0C, 004222CD +10, 004222F5 +14), 00421FF0 Vector lists -> +18 (0042231D) and +24 (00422345);
// dtor 00420610 frees [+24] (0042062D) and [+18] (00420645); ctor 004221C0 zeroes all twelve DWORDs).
struct FontInfo {
 I numchars;                                             // +00 "numchars" (int target)
 float fontheight,baselineheight,texturewidth,spacing_r,spacing_b; // +04..+14 "%f" targets
 List upper_left_coords;                                 // +18 CExoArrayList<Vector> {data,size,capacity}
 List lower_right_coords;                                // +24 CExoArrayList<Vector>
};
struct Basic {
 Ref prefix;                       // +00 Ref24 member (not a base class); +00 vtable (Basic table 0073F160)
 float f18,f1c;                    // +18 bumpmapscaling, +1C bumpintensity: "%f" targets (004228D4, 004229A5 -> 004635C0)
 float f20;                        // +20 alpha: a real binary32 object (callback float* target, 0041F946/0041F738;
                                   //     TXI "alphamean" 0042283E). Integer-instruction sites use float_bits/set_float_bits.
 float f24,f28,f2c,f30;            // +24 gamma (004227D8), +28 envmapalpha (00422A4E), +2C diffusebumpintensity
                                   // (00422A76), +30 specularbumpintensity (00422A9E): "%f" targets of 004635C0
 U volatile f34;                   // +34 procedure/controller object pointer
 FontInfo* volatile info;          // +38
 U* volatile components;           // +3C
 U volatile f40,f44,f48;           // +40 GL id array, +44 CPU image
 float f4c,f50,f54;                // +4C..+54 specularcolor: "%f %f %f" targets (004228FC -> 00463570, fmt 0073EE40)
 U f58;                            // +58 first GL id: glGenTextures(1,&+58) out-param object (00420C83..00420C88), nonvolatile
 I f5c,f60,f64,f68,f6c,f70,f74;    // +5C compressed bytes/mip count, +60 p6, +64 p7, +68 width, +6C height,
                                   // +70 components, +74 format: real Signed callback targets. Ordered
                                   // observations use field_load/field_store (volatile-qualified access).
 char name[32],derived[32];        // +78, +98
 int16_t volatile b8,ba,bc,be,c0,c2,c4,c6,c8,ca,component_count,ce; // +C2/+C4 grid, +C6 multi count, +CE mip16
 uint16_t volatile hash;           // +D0
 uint8_t volatile d2,d3;
 I d4;                             // +D4 isbumpmap: int32 target (00422800 -> 004635E0 `mov dword ptr [esi],..`)
 uint8_t volatile flags[16];       // +D8: [2]=DA fill-alpha, [3]=DB cube, [8]=E0 attempted, [9]=E1 failed,
                                   // [10]=E2 special, [11]=E3 release protocol, [13]=E5 processed, [14]=E6
 I volatile e8;
};
struct Providers {
 I (__cdecl *compare)(const char*,const char*);
 U (__cdecl *hash)(const char*,I,const U*);
 U (__fastcall *add_ordered)(Basic*,void*);
 FontInfo* (__fastcall *info_ctor)(void*,void*);
 U (__fastcall *info_dtor)(FontInfo*,void*);
 void* (__cdecl *read)(const char*,const char*,I volatile*,U);
 void* (__cdecl *bytes)(U,void*);
 const char* (__cdecl *next_line)();
 U (__fastcall *parse_line)(Basic*,void*,const char*);
 U (__cdecl *resource_release)(void*,I);
 U (__cdecl *cube_available)();
 List* find_cache;
 U volatile* object_count;
 uint8_t volatile* default_flag;
 void** reference_table;
 void** basic_table;
};
extern Providers providers;
extern "C" {
 Ref* __cdecl find_00420ac0(const char*,const char*,const U*,I);
 Ref* __fastcall reference_ctor_00422e30(void*,void*);
 Ref* __fastcall basic_ctor_00423150(void*,void*,const char*,const char*);
 U __fastcall init_00422af0(Basic*,void*,const char*);
 // 00420970 Basic::glImage: ECX Basic*, EDX unused, one stack word, RET4 (00420993/00420ABA), returns +58 (00420AB6).
 // Body not in this packet (packet 2); single shared declaration.
 U __fastcall basic_gl_image_00420970(Basic*,void*,U);
 // 00420C20 AsNormalizeVectorCubeMap (thiscall RET0; texture/texture_normalize_cubemap.cpp, D32/D48).
 void __fastcall basic_as_normalize_vector_cube_map_00420c20(void*,void*);
}
static_assert(sizeof(void*)==4&&sizeof(Ref)==24&&sizeof(float)==4&&sizeof(I)==4,"x86 carriers");
static_assert(sizeof(Basic)==236&&sizeof(FontInfo)==48,"Basic236/FontInfo48");
static_assert(offsetof(FontInfo,fontheight)==4&&offsetof(FontInfo,spacing_b)==0x14&&offsetof(FontInfo,upper_left_coords)==0x18&&
              offsetof(FontInfo,lower_right_coords)==0x24,"CAurFontInfo layout");
static_assert(offsetof(Basic,f18)==0x18&&offsetof(Basic,f1c)==0x1c&&offsetof(Basic,f20)==0x20&&offsetof(Basic,f24)==0x24&&
              offsetof(Basic,f28)==0x28&&offsetof(Basic,f2c)==0x2c&&offsetof(Basic,f30)==0x30&&offsetof(Basic,f34)==0x34&&
              offsetof(Basic,f4c)==0x4c&&offsetof(Basic,f50)==0x50&&offsetof(Basic,f54)==0x54&&
              offsetof(Basic,info)==0x38&&offsetof(Basic,components)==0x3c&&offsetof(Basic,f40)==0x40&&
              offsetof(Basic,f44)==0x44&&offsetof(Basic,f58)==0x58,"scalar prefix offsets");
static_assert(offsetof(Basic,f5c)==0x5c&&offsetof(Basic,f60)==0x60&&offsetof(Basic,f64)==0x64&&offsetof(Basic,f68)==0x68&&
              offsetof(Basic,f6c)==0x6c&&offsetof(Basic,f70)==0x70&&offsetof(Basic,f74)==0x74,"Signed output offsets");
static_assert(offsetof(Basic,name)==0x78&&offsetof(Basic,derived)==0x98&&offsetof(Basic,c2)==0xc2&&offsetof(Basic,c4)==0xc4&&
              offsetof(Basic,c6)==0xc6&&offsetof(Basic,c8)==0xc8&&offsetof(Basic,ca)==0xca&&
              offsetof(Basic,component_count)==0xcc&&offsetof(Basic,ce)==0xce&&offsetof(Basic,hash)==0xd0&&
              offsetof(Basic,d4)==0xd4&&offsetof(Basic,flags)==0xd8&&offsetof(Basic,e8)==0xe8,"verified original layout");
static_assert(offsetof(Basic,flags)+3==0xdb&&offsetof(Basic,flags)+8==0xe0&&offsetof(Basic,flags)+11==0xe3&&
              offsetof(Basic,flags)+13==0xe5&&offsetof(Basic,flags)+14==0xe6,"+DB cube, +E0, +E3, +E5 processed, +E6");
static_assert(std::is_standard_layout<Basic>::value&&std::is_trivially_destructible<Basic>::value,"offsetof/byte views valid");

// Ordered observation of the nonvolatile Signed fields: every consumer read/write of f5c..f74 that is
// not a callback-pointer hand-off goes through a volatile-qualified access (adding volatile is permitted).
inline I field_load(const I& field){return static_cast<const volatile I&>(field);} // also +D4
inline void field_store(I& field,I value){static_cast<volatile I&>(field)=value;}

// Raw-DWORD transport for the float members (+18..+30, +4C..+54). Original integer instructions:
// cmp dword [ebx+20h],0BF800000h (004216F1); mov dword [..+20h],3F800000h (00421701, 0041FAC9, 0041FAE2);
// ctor 00423150 mov dword [esi+18h/1Ch/24h/28h/2Ch/30h/4Ch/50h/54h],3F800000h and [esi+20h],0BF800000h (00423211);
// gamma word pushed raw: 0041FA34 mov eax,[esi+24h] / push eax.
// memcpy is a byte copy of the float object representation: no float<->int conversion, no pointer pun.
constexpr U alpha_sentinel_bits=0xbf800000u; // -1.0f; compared as bits, never as float
constexpr U alpha_one_bits=0x3f800000u;      // 1.0f stored as bits
inline U float_bits(const float& field){U bits;std::memcpy(&bits,&field,sizeof bits);return bits;}
inline void set_float_bits(float& field,U bits){std::memcpy(&field,&bits,sizeof bits);}
inline U alpha_bits(const Basic& self){return float_bits(self.f20);}
// Zero store of FontInfo DWORD n (n = byte offset / 4), as ctor 004221C0 / Basic ctor 00423150 do word by word.
// Each case is one DWORD store to the named member (floats via raw bits): no array pun over the record.
inline void font_info_zero_word(FontInfo& info,unsigned n){
 switch(n){
  case 0:static_cast<volatile I&>(info.numchars)=0;break;
  case 1:set_float_bits(info.fontheight,0);break;   case 2:set_float_bits(info.baselineheight,0);break;
  case 3:set_float_bits(info.texturewidth,0);break; case 4:set_float_bits(info.spacing_r,0);break;
  case 5:set_float_bits(info.spacing_b,0);break;
  case 6:info.upper_left_coords.data=0;break;  case 7:info.upper_left_coords.size=0;break;  case 8:info.upper_left_coords.capacity=0;break;
  case 9:info.lower_right_coords.data=0;break; case 10:info.lower_right_coords.size=0;break; case 11:info.lower_right_coords.capacity=0;break;
  default:break;
 }
}
inline void set_alpha_bits(Basic& self,U bits){set_float_bits(self.f20,bits);}

// Receiver/vtable carriers (texture_x86.asm). Each reloads the live vtable at its call and tail-jumps the
// slot with ECX = receiver and the caller's return address; zero stack words; EAX clobbered.
extern "C" {
 void         __fastcall texture_call_free_b0(Basic*);           // [vtable+B0] FreeImage (0041F6AD, 00421748), result ignored
 std::uint8_t __fastcall texture_call_flag_0c_al(Basic*);        // [vtable+0C] AL only (0041F8CA/0041F8CD)
 std::uint8_t __fastcall texture_call_processed_30_al(Basic*);   // [vtable+30] AL only (0042173C/0042173F) -> 0041EC30
 // [vtable+B0] with full EAX (CAurTexture::FreeImage 0041F324 tail-jumps the Basic's own slot +B0).
 U            __fastcall texture_call_free_b0_eax(Basic*);
}
}

namespace acquisition {
using Signed=std::int32_t; using Word=std::uint32_t;
static_assert(std::is_same<std::make_unsigned_t<Signed>,Word>::value,"Word is the corresponding unsigned type of Signed");
static_assert(std::is_same<Signed,texture::I>::value,"Basic outputs are Signed objects");
// [basic.lval]/8.3: a Signed object may be accessed through its corresponding unsigned type.
inline Word* as_corresponding_unsigned(Signed* p){return reinterpret_cast<Word*>(p);}

using ProcessedRaster     =char* (__cdecl*)(const char*,Signed*,Signed*,char*,Signed*,Signed*,Signed*,float*,char*); // [0078D420] -> 0070D510
using CompressedRaster    =char* (__cdecl*)(const char*,Signed*,Signed*,Signed*,Signed*,float*);                    // [0078D40C] -> 0070D380
using Raster16            =char* (__cdecl*)(const char*,Signed*,Signed*,float*,Signed*);                            // [0078D414] -> 0070D6B0
using Raster              =char* (__cdecl*)(const char*,Word*,Word*,Word*);                                         // [0078D404] -> 0070C790
using ReleaseRaster       =void  (__cdecl*)(char*,Signed,Signed,Signed,Word,Signed,Signed,Word);                    // [007A46F8] -> 0070C870

// Main-owned borrowed view of the installed cells (installer 0070D9C0). No storage here; each cell is
// reloaded at its original read site.
struct Cells {
 ProcessedRaster const volatile* processed_0078d420;
 CompressedRaster const volatile* compressed_0078d40c;
 Raster16 const volatile* raster16_0078d414;
 Raster const volatile* raster_0078d404;
 ReleaseRaster const volatile* release_007a46f8;
 Word const volatile* force_zero_request_007a4700;
};
extern const Cells& cells;

// 0041EA20 GetImageWrapper: physical cdecl, twelve caller-owned argument cells, RET0, full EAX pointer.
// WrapperCells is the exact image of those cells at entry [esp+4..esp+30h].
struct WrapperCells {
 const char* name;                    // [esp+04]
 Signed* width;                       // [esp+08]
 Signed* height;                      // [esp+0C]
 Signed* components;                  // [esp+10]
 Signed* compressed;                  // [esp+14] low byte reused as the processed encoding byte (0041EA67)
 Signed* p6;                          // [esp+18] low byte reused as the processed mip byte (0041EA4D)
 Signed* p7;                          // [esp+1C]
 Signed* format;                      // [esp+20]
 float* alpha;                        // [esp+24]
 std::uint8_t volatile* processed;    // [esp+28] -> Basic+E5
 std::int16_t volatile* mip16;        // [esp+2C] -> Basic+CE
 Signed request;                      // [esp+30] forced to 0 in place when [007A4700]!=0 (0041EA29)
};
static_assert(sizeof(WrapperCells)==48&&offsetof(WrapperCells,compressed)==0x10&&offsetof(WrapperCells,p6)==0x14&&
              offsetof(WrapperCells,request)==0x2c&&std::is_standard_layout<WrapperCells>::value,"twelve DWORD cells");

extern "C" {
 // texture_x86.asm entry: lea eax,[esp+4] / push eax / call core / add esp,4 / ret (EAX from core).
 char* __cdecl get_image_wrapper_0041ea20(const char*,Signed*,Signed*,Signed*,Signed*,Signed*,Signed*,Signed*,
                                          float*,std::uint8_t volatile*,std::int16_t volatile*,Signed);
 char* __cdecl get_image_wrapper_core_0041ea20(WrapperCells* caller_cells);
 // 0045F950 compression capability: no arguments, full EAX tested (0041EB10/0041EB15).
 Word __stdcall acquisition_compression_available_0045f950();
 // 0041F6A0 LoadImageA: ECX Basic*, EDX ignored, zero stack, RET0, FULL pointer EAX. Single shared declaration.
 char* __fastcall load_image_a_0041f6a0(texture::Basic*,void*);
}
}
