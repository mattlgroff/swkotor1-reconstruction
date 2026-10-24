#include "7314e5f55d64-player_internal.hpp"
#include "d1ffedc48c11-gff_readers.h"
extern "C" {
menu_resources::Ref* __fastcall c2da_default_ref_00405ed0(menu_resources::Ref*,void*);
std::uint32_t __fastcall gui_load_resref_is_valid_00406030(const menu_resources::Ref*,void*);
void* __fastcall kotor_fastcall_00673a10(void*,void*);
void* __fastcall kotor_fastcall_00673a30(void*,void*);
}
namespace {
using namespace t2_private;
void read_ref(gui_load::Gff* g,gui_load::Struct* s,const char* label,U* ok,Ref* dest){
 Ref fallback,temporary,result;
 fallback.construct_00406d80("");
 Ref* returned=gui_load_gff_resref_00411e10(g,nullptr,&temporary,s,label,ok,&fallback);
 resource_ref_assignment::assign_00406120(dest,nullptr,&result,returned);
}
int load_gun(void* self,void* rawGff,void* rawStruct,bool enemy){
 auto* g=static_cast<gui_load::Gff*>(rawGff);auto* s=static_cast<gui_load::Struct*>(rawStruct);
 Ref bulletModel,gunModel,fireSound;
 c2da_default_ref_00405ed0(&bulletModel,nullptr);
 c2da_default_ref_00405ed0(&gunModel,nullptr);
 c2da_default_ref_00405ed0(&fireSound,nullptr);
 alignas(4) unsigned char bullet[0x28]; // +14 remains unwritten, as in the original constructor.
 kotor_fastcall_00673a10(bullet,nullptr);
 alignas(4) unsigned char target[16];
 if(enemy)kotor_fastcall_00673a30(target,nullptr);
 U ok;
 const U bank=gui_load_gff_dword_00411c20(g,nullptr,s,"BankID",&ok,0xffffffffu);
 if(bank==0xffffffffu)return 0;
 read_ref(g,s,"Gun_Model",&ok,&gunModel);
 if(!gui_load_resref_is_valid_00406030(&gunModel,nullptr))return 0;
 if(enemy){
  wr(target,4,gui_load_gff_float_00411d00(g,nullptr,s,"Sensing_Radius",&ok,0));if(!ok)return 0;
  wr(target,0,gui_load_gff_float_00411d00(g,nullptr,s,"Horiz_Spread",&ok,0));if(!ok)return 0;
  wr(target,8,gui_load_gff_float_00411d00(g,nullptr,s,"Vert_Spread",&ok,0));if(!ok)return 0;
  wr(target,12,gui_load_gff_float_00411d00(g,nullptr,s,"Inaccuracy",&ok,0));
 }
 gui_load::Struct child;
 ok=gui_load_gff_struct_00411a10(g,nullptr,&child,s,"Bullet");if(!ok)return 0;
 read_ref(g,&child,"Bullet_Model",&ok,&bulletModel);
 if(!gui_load_resref_is_valid_00406030(&bulletModel,nullptr))return 0;
 wr(bullet,0,gui_load_gff_dword_00411c20(g,nullptr,&child,"Damage",&ok,0));if(!ok)return 0;
 wr(bullet,12,gui_load_gff_float_00411d00(g,nullptr,&child,"Lifespan",&ok,0));if(!ok)return 0;
 wr(bullet,8,gui_load_gff_float_00411d00(g,nullptr,&child,"Rate_Of_Fire",&ok,0));if(!ok)return 0;
 wr(bullet,4,gui_load_gff_float_00411d00(g,nullptr,&child,"Speed",&ok,0));if(!ok)return 0;
 wr(bullet,16,gui_load_gff_dword_00411c20(g,nullptr,&child,"Target_Type",&ok,0));if(!ok)return 0;
 read_ref(g,&child,"Collision_Sound",&ok,reinterpret_cast<Ref*>(bullet+24));
 read_ref(g,s,"Fire_Sound",&ok,&fireSound);
 // Original call-site pushes put Bullet_Model before Gun_Model in both methods.
 if(enemy)kotor_fastcall_0066f5f0(self,nullptr,signed_bits(bank),&bulletModel,&gunModel,bullet,target,&fireSound);
 else kotor_fastcall_0066f440(self,nullptr,signed_bits(bank),&bulletModel,&gunModel,bullet,&fireSound);
 return 1; // CreateGunBank's null result is deliberately ignored.
}
}
extern "C" int __fastcall kotor_fastcall_0066f890(void* self,void*,void* g,void* s) noexcept(false){return load_gun(self,g,s,false);}
extern "C" int __fastcall kotor_fastcall_0066fb20(void* self,void*,void* g,void* s) noexcept(false){return load_gun(self,g,s,true);}
