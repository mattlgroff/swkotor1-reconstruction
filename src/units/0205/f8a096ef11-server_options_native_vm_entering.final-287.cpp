extern "C" void* __fastcall recovered_rules_server_construct_0057a1f0(void*,void*);   // W1 tier9
extern "C" int __fastcall recovered_vm_get_entering_object_00537ee0(void*,void*,int,unsigned);
extern "C" int __fastcall recovered_vm_get_is_pc_0053f1c0(void*,void*,int,unsigned);
extern "C" int __fastcall recovered_vm_execute_command_0052c0d0(void*,void*,int,unsigned);
#include "0ba57dfe7b-server_options.hpp"
#include "fb62d9a6ef-options_leaf.hpp"
#include "6cf5011aa5-debug_timestamp.hpp"
#include "fed6008d14-ai_construct.hpp"
#include "cadde52d92-ai_dispatch.hpp"
#include "dff91384ce-vm_construct.hpp"
#include "0faf589f51-vm_commands.hpp"
#include "85a32563b3-server_policy.hpp"
#include <sys/timeb.h>
#include <ctime>
#include <cstdio>
#include <cstddef>
#include "77fb10ec4f-startup_views.h"
#include "723b658e51-resources.h"
#include "fcbc7f08ae-menu_targets.h"
#include <new>
#include <cstring>
#if !defined(_M_IX86)
#error Native options bridge requires x86
#endif
extern "C" void __fastcall recovered_vm_script_trace_0052c100(void*,void*,void*);
extern "C" {
unsigned __fastcall recovered_effect_apply_assured_hit_004d83a0(void*,void*,void*,void*,unsigned);
unsigned __fastcall recovered_effect_remove_assured_hit_004d83f0(void*,void*,void*,void*);
unsigned __fastcall recovered_effect_dispatch_004d8320(void*,void*,void*,void*,unsigned);
unsigned __fastcall recovered_property_effect_004e60c0(void*,void*,void*,void*,void*,unsigned,unsigned);
unsigned __fastcall recovered_effect_type68_004dccc0(void*,void*,void*,void*,unsigned);
unsigned __fastcall recovered_item_property_dispatch_004e5410(void*,void*,void*,void*,void*,unsigned,unsigned);
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long,const char*);
void* __fastcall kotor_fastcall_004d60f0(void*,void*);
unsigned __fastcall camera_return_zero_00401590(void*,void*,unsigned,unsigned,unsigned,unsigned);
unsigned __fastcall gui_load_slot_zero8_00409eb0(void*,void*,unsigned,unsigned);
void* __fastcall c2da_default_ref_00405ed0(void*,void*);
void __fastcall GuiAssign005e5140(void*,void*,const char*);

extern unsigned char kotor_rules_cell8_007a39dc;
extern void* kotor_cell_tlk_007a3a08;
extern void* kotor_cell_rules_007a3a28;
void __fastcall frame_LogStringCtor_005e5a90(void*,void*,const char*);
void __fastcall frame_LogStringDtor_005e5c20(void*,void*);
const char* __fastcall frame_LogCStr_005e5670(void*,void*);
unsigned __fastcall gui_load_resman_remove_file_00406b20(void*,void*,void*,unsigned);
unsigned __fastcall gui_load_resman_create_directory_004068c0(void*,void*,void*);
}
namespace kotor_reconstruction {void __cdecl StartupFormat005e5680(StartupString*,const char*,...);}
namespace menu_resources {Manager* __cdecl BorrowCanonicalManager007a39e8();}
namespace kotor_menu_window_cells {extern void* volatile manager_007a39fc;}
namespace frame_control {extern unsigned message_manager_007a3a00;extern unsigned resource_manager_007a39e8;}
namespace tlk_core {unsigned __fastcall OpenFile0041e5a0(kotor_menu_10_19::Tlk*,void*,kotor_reconstruction::StartupString*);}   // W6: 0387 provider
extern "C" {
int __fastcall recovered_vm_action_follow_leader_0052cbe0(void*,void*,int,unsigned);
int __fastcall recovered_vm_get_hitpoints_00539610(void*,void*,int,unsigned);
int __fastcall recovered_vm_get_effect_00549180(void*,void*,int,unsigned);
int __fastcall recovered_vm_get_is_effect_valid_0053ee00(void*,void*,int,unsigned);
int __fastcall recovered_vm_roll_dice_005420b0(void*,void*,int,unsigned);
int __fastcall recovered_vm_get_item_in_slot_0053a2c0(void*,void*,int,unsigned);
int __fastcall recovered_vm_int_to_float_0053eaa0(void*,void*,int,unsigned);
int __fastcall recovered_vm_get_creature_has_talent_00536ec0(void*,void*,int,unsigned);
int __fastcall recovered_vm_get_creature_talent_00536fe0(void*,void*,int,unsigned);
int __fastcall recovered_vm_get_is_talent_valid_00539f90(void*,void*,int,unsigned);
int __fastcall recovered_vm_talent_access_00544d70(void*,void*,int,unsigned);
int __fastcall recovered_vm_set_return_strref_005434c0(void*,void*,int,unsigned);
int __fastcall recovered_vm_give_xp_to_creature_0053e750(void*,void*,int,unsigned);
int __fastcall recovered_vm_play_movie_00540ed0(void*,void*,int,unsigned);
}
extern "C" int __fastcall recovered_vm_get_has_skill_00539380(void*,void*,int,unsigned);
// ---- closeC (0854 gates): providers bound by the reconstructed destructors / game-defined-structure slots ----
extern "C" {
void* __cdecl live_operator_new(std::uint32_t size);                                        // 006FA7E6 [0004]
void __cdecl live_free(void* block);                                                        // 006FA390 [0004]
int __cdecl _purecall(void);                                                                // 006FB8DE _purecall (CRT)
void __fastcall kotor_fastcall_004d6130(void*,void*);                                       // 004D6130 ~CSWMessage [0597]
void __fastcall recovered_vm_stack_clear_005d2140(void*,void*);                             // 005D2140 CVirtualMachineStack::ClearStack [1187]
void __stdcall c2da_destroy_array_006fa852(void*,unsigned,int,void*);                       // 006FA852 eh vector destructor iterator [0186]
unsigned __fastcall c2da_not_equal_text_00406060(void*,void*,const char*);                  // 00406060 CResRef::operator!=(char const*) [0264]
void __fastcall c2da_cancel_00409b00(void*,void*);                                          // 00409B00 CRes::CancelRequest [0264]
unsigned __fastcall c2da_release_00409cf0(void*,void*,void*);                               // 00409CF0 CExoResMan::ReleaseResObject [0264]
void* __fastcall recovered_effect_construct_00503e40(void*,void*,unsigned);                 // 00503E40 CGameEffect(int) [0886]
void* __fastcall recovered_event_construct_004d7540(void*,void*);                           // 004D7540 CScriptEvent() [1017]
void* __fastcall kotor_fastcall_004ca7a0(void*,void*);                                      // 004CA7A0 CScriptLocation() [claude_vm_actions_queries]
void* __fastcall recovered_talent_construct_004ca9e0(void*,void*);                          // 004CA9E0 CScriptTalent() [1194s]
unsigned __fastcall recovered_gff_004125b0(void*,void*,void*,void*,const char*,unsigned);   // 004125B0 CResGFF::AddStructToStruct [w2_save_gff_native]
unsigned __fastcall gui_load_gff_struct_00411a10(void*,void*,void*,void*,const char*);      // 00411A10 CResGFF::GetStructFromStruct [0292]
unsigned __fastcall recovered_save_game_effect_00503790(void*,void*,void*,void*);           // 00503790 CGameEffect::SaveGameEffect [w2_save_object_native]
unsigned __fastcall kotor_fastcall_004d73a0(void*,void*,void*,void*);                       // 004D73A0 CScriptEvent::SaveEvent [claude_save_module]
unsigned __fastcall recovered_save_location_004ca8e0(void*,void*,void*,void*);              // 004CA8E0 CScriptLocation::SaveLocation [w2_save_object_native]
void __fastcall kotor_fastcall_004ca800(void*,void*,void*,void*);                           // 004CA800 CScriptLocation::LoadLocation [claude_0886_bodies_vars]
unsigned __fastcall recovered_gff_00412620(void*,void*,void*,unsigned,const char*);         // 00412620 CResGFF::WriteFieldBYTE [0873]
unsigned __fastcall recovered_gff_00412760(void*,void*,void*,unsigned,const char*);         // 00412760 CResGFF::WriteFieldDWORD [0873]
unsigned __fastcall recovered_gff_004127b0(void*,void*,void*,unsigned,const char*);         // 004127B0 CResGFF::WriteFieldINT [0873]
unsigned __fastcall gff_list_004118c0(void*,void*,void*,void*,const char*);                 // 004118C0 CResGFF::GetList [0291]
unsigned __fastcall gff_list_count_00411940(void*,void*,void*);                             // 00411940 CResGFF::GetListCount [0291]
unsigned __fastcall gff_list_element_00411990(void*,void*,void*,void*,unsigned);            // 00411990 CResGFF::GetListElement [0291]
unsigned __fastcall gff_label_00411630(void*,void*,void*,const char*);                      // 00411630 CResGFF::GetFieldByLabel [0291]
unsigned char __fastcall gui_load_gff_byte_00411a60(void*,void*,void*,const char*,unsigned*,unsigned); // 00411A60 ReadFieldBYTE [0292]
unsigned short __fastcall recovered_gff_word_00411b40(void*,void*,void*,const char*,unsigned*,unsigned short); // 00411B40 ReadFieldWORD [0886]
unsigned __fastcall gui_load_gff_dword_00411c20(void*,void*,void*,const char*,unsigned*,unsigned); // 00411C20 ReadFieldDWORD [0292]
int __fastcall gui_load_gff_int_00411c90(void*,void*,void*,const char*,unsigned*,int);      // 00411C90 ReadFieldINT [0292]
float __fastcall gui_load_gff_float_00411d00(void*,void*,void*,const char*,unsigned*,float); // 00411D00 ReadFieldFLOAT [0292]
void* __fastcall gui_load_gff_string_00411ec0(void*,void*,void*,void*,const char*,unsigned*,void*); // 00411EC0 ReadFieldCExoString [0292]
void __fastcall recovered_effect_resize_00503630(void*,void*,unsigned);                     // 00503630 CGameEffect::SetNumIntegers [1091]
void __fastcall recovered_effect_creator_00503a00(void*,void*,unsigned);                    // 00503A00 CGameEffect::SetCreator [0886]
void* __fastcall async_assign_string_005e5c50(void*,void*,const void*);                     // 005E5C50 CExoString::operator= [0199]
void* __fastcall WindowCopy005e5b00(void*,void*,void*);                                     // 005E5B00 CExoString(CExoString const&) [0078]
struct Claude0886ExoString { unsigned data, length; };
void __fastcall claude_exo_string_list_add_004059a0(void*,void*,Claude0886ExoString);      // 004059A0 CExoArrayList<CExoString>::Add(by value) [claude_0886_bodies_vars]
unsigned __fastcall kotor_fastcall_004d8680(void*,void*,void*,void*,unsigned);              // 004D8680 OnApplySummonCreature [claude_closeC_0854]
unsigned __fastcall kotor_fastcall_004d8a10(void*,void*,void*,void*);                       // 004D8A10 OnRemoveSummonCreature [claude_closeC_0854]
}
namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, unsigned int); } // 005E03C0 CExoArrayList<int>::Allocate [0098]
namespace kotor_q05 { struct FieldRecord; struct StructHandle;
struct CResGFF { FieldRecord* GetField(StructHandle*, unsigned); unsigned GetDataField(FieldRecord*, unsigned*); }; } // 00410990 / 00410A20 [0290]
namespace {
using namespace recovered_server_options;
void* readptr(void*p,unsigned off){void*v;std::memcpy(&v,(char*)p+off,4);return v;}
__declspec(noreturn) void required(unsigned long address){kotor_unreachable_trap(address,"Server options required native provider");}
// closeC: 00525430 CSWSMessage scalar deleting destructor (RET 4) -> 005209A0 ~CSWSMessage: vptr = 00747E94, then the
// CSWMessage base destructor 004D6130 (tail JMP); bit 0 frees the block (006FA390).
void* __fastcall message_deleting_00525430(void*,void*,unsigned);
using DeletingDestructor=void(__fastcall*)(void*,void*,unsigned);
const DeletingDestructor message_vtable_00747e94[]={reinterpret_cast<DeletingDestructor>(reinterpret_cast<void(*)()>(&message_deleting_00525430))};
void* __fastcall message_deleting_00525430(void* self,void*,unsigned flags){
 *static_cast<const void**>(self)=message_vtable_00747e94;                       // 005209A0
 kotor_fastcall_004d6130(self,nullptr);                                         // 005209A6 jmp ~CSWMessage
 if((flags&1u)!=0)live_free(self);                                               // 00525438..45
 return self;                                                                    // 00525448
}
void* message_base(void*s){return kotor_fastcall_004d60f0(s,nullptr);}
struct Leaf:recovered_options_leaf::Ports {void*allocate(unsigned n)override{return ::operator new(n);}};
static_assert(offsetof(recovered_debug_timestamp::Timeb32,time)==0 && offsetof(recovered_debug_timestamp::Timeb32,millitm)==4 && offsetof(recovered_debug_timestamp::Timeb32,timezone)==6 && offsetof(recovered_debug_timestamp::Timeb32,dstflag)==8,"Original timeb32 field offsets");
#include "a3fed2164d-ai_dispatch_native_zero_66b360.inc"
struct Dispatch:recovered_ai_dispatch::Ports {
 void* allocate(unsigned n)override{return ::operator new(n);}
 const void*resolve(unsigned target)override{
  switch(target){
   case 0x004d83a0:return reinterpret_cast<const void*>(&recovered_effect_apply_assured_hit_004d83a0);
   case 0x004d83f0:return reinterpret_cast<const void*>(&recovered_effect_remove_assured_hit_004d83f0);
   default:return ai_resolve(target);
  }
 }
};
void __fastcall initialize_dispatch1(void*s,void*){Dispatch p;recovered_ai_dispatch::initialize_004e4a10(s,p);}
void __fastcall initialize_dispatch2(void*s,void*){Dispatch p;recovered_ai_dispatch::initialize_004ea9f0(s,p);}
// closeC: 004AF390 / 004AF3B0 scalar deleting destructors (RET 4) of CSWSEffectListHandler / CSWSItemPropertyHandler.
//   004D82B0 / 004E53A0: vptr = own table; free [+4] and [+8] (each zeroed); vptr = base table 00744254 / 00744264.
//   Base tables (CGameEffectApplierRemover / CItemPropertyApplierRemover): {004AF2E0 / 004AF310 deleting dtor, _purecall x3}.
void* __fastcall effect_handler_deleting_004af390(void*,void*,unsigned);
void* __fastcall property_handler_deleting_004af3b0(void*,void*,unsigned);
void* __fastcall effect_applier_deleting_004af2e0(void*,void*,unsigned);
void* __fastcall property_applier_deleting_004af310(void*,void*,unsigned);
using PureEntry=int(__cdecl*)();
struct BaseTable4 { void*(__fastcall* destroy)(void*,void*,unsigned); PureEntry slot1, slot2, slot3; };
const BaseTable4 base_table_00744254={effect_applier_deleting_004af2e0,_purecall,_purecall,_purecall};
const BaseTable4 base_table_00744264={property_applier_deleting_004af310,_purecall,_purecall,_purecall};
void* __fastcall effect_applier_deleting_004af2e0(void* self,void*,unsigned flags){
 *static_cast<const void**>(self)=&base_table_00744254;                         // 004AF2E8
 if((flags&1u)!=0)live_free(self);                                               // 004AF2E0, 004AF2EE..F6
 return self;
}
void* __fastcall property_applier_deleting_004af310(void* self,void*,unsigned flags){
 *static_cast<const void**>(self)=&base_table_00744264;                         // 004AF318
 if((flags&1u)!=0)live_free(self);                                               // 004AF310, 004AF31E..26
 return self;
}
void free_member_closec(void* self,unsigned offset){                             // test; push; call 006FA390; mov [x],0
 void*& member=*reinterpret_cast<void**>(static_cast<unsigned char*>(self)+offset);
 if(member!=nullptr){live_free(member);member=nullptr;}
}
static void(__fastcall* const ai_table1_destructor)(void*,void*)=reinterpret_cast<void(__fastcall*)(void*,void*)>(reinterpret_cast<void(*)()>(&effect_handler_deleting_004af390));
static void(__fastcall* const ai_table2_destructor)(void*,void*)=reinterpret_cast<void(__fastcall*)(void*,void*)>(reinterpret_cast<void(*)()>(&property_handler_deleting_004af3b0));
// 004D8360 CSWSEffectListHandler::OnEffectRemoved(CSWSObject*, CGameEffect*), thiscall RET 8: remove callback table at
// [this+8] indexed by the effect type word (+8; unsigned compare against 6Eh, no lower bound), called with ECX = this and
// (object, effect); 1 when there is no callback.
unsigned __fastcall ai_table1_slot3(void* self, void*, void* object, void* effect) {
    const unsigned short type = *reinterpret_cast<const unsigned short*>(static_cast<const unsigned char*>(effect) + 8); // 004D8365
    if (type < 0x6e) {                                                                                               // 004D8369..70
        void* const fn = (*reinterpret_cast<void* const* const*>(static_cast<const unsigned char*>(self) + 8))[type];   // 004D8372..75
        if (fn != nullptr)                                                                                            // 004D8378..7A
            return reinterpret_cast<unsigned(__fastcall*)(void*, void*, void*, void*)>(fn)(self, nullptr, object, effect); // 004D837C..85
    }
    return 1;                                                                                                         // 004D8388..8E
}
// 004E5450 CSWSItemPropertyHandler::OnItemPropertyRemoved(item, property, creature, slot), thiscall RET 10h: remove table at
// [this+8] indexed by the property type word (+0; unsigned compare against 3Ch); 0 when there is no callback.
unsigned __fastcall ai_table2_slot3(void* self, void*, void* item, void* property, void* creature, unsigned slot) {
    const unsigned short type = *static_cast<const unsigned short*>(property);                                         // 004E5454
    if (type < 0x3c) {                                                                                                // 004E5457..5C
        void* const fn = (*reinterpret_cast<void* const* const*>(static_cast<const unsigned char*>(self) + 8))[type];   // 004E545E..64
        if (fn != nullptr)                                                                                            // 004E5467..69
            return reinterpret_cast<unsigned(__fastcall*)(void*, void*, void*, void*, void*, unsigned)>(fn)(
                self, nullptr, item, property, creature, slot);                                                       // 004E546B..7E
    }
    return 0;                                                                                                         // 004E5481..84
}
using AiEntry=void(__fastcall*)(void*,void*);
struct EffectTable { AiEntry destroy, initialize; decltype(&recovered_effect_dispatch_004d8320) dispatch; decltype(&ai_table1_slot3) tail; };
const EffectTable ai_table_00744274={ai_table1_destructor,initialize_dispatch1,recovered_effect_dispatch_004d8320,ai_table1_slot3};
struct PropertyTable { AiEntry destroy, initialize; decltype(&recovered_item_property_dispatch_004e5410) dispatch; decltype(&ai_table2_slot3) tail; };
const PropertyTable ai_table_00744284={ai_table2_destructor,initialize_dispatch2,recovered_item_property_dispatch_004e5410,ai_table2_slot3};
void* __fastcall effect_handler_deleting_004af390(void* self,void*,unsigned flags){
 *static_cast<const void**>(self)=&ai_table_00744274;                           // 004D82CD
 free_member_closec(self,4);                                                     // 004D82D3..EB
 free_member_closec(self,8);                                                     // 004D82F2..02
 *static_cast<const void**>(self)=&base_table_00744254;                         // 004D830D
 if((flags&1u)!=0)live_free(self);                                               // 004AF398..A5
 return self;                                                                    // 004AF3A8
}
void* __fastcall property_handler_deleting_004af3b0(void* self,void*,unsigned flags){
 *static_cast<const void**>(self)=&ai_table_00744284;                           // 004E53BD
 free_member_closec(self,4);                                                     // 004E53C3..DB
 free_member_closec(self,8);                                                     // 004E53E2..F2
 *static_cast<const void**>(self)=&base_table_00744264;                         // 004E53FD
 if((flags&1u)!=0)live_free(self);                                               // 004AF3B8..C5
 return self;                                                                    // 004AF3C8
}
static_assert(sizeof(EffectTable)==16 && offsetof(EffectTable,dispatch)==8 && sizeof(PropertyTable)==16 && offsetof(PropertyTable,dispatch)==8,"Original four-slot tables");
struct Ai:recovered_ai_construct::Ports{
 void*allocate(unsigned n)override{return ::operator new(n);}
 void string_default(void*s)override{std::memset(s,0,8);}
 const void*table1()override{return &ai_table_00744274;}
 const void*table2()override{return &ai_table_00744284;}
 void initialize_table1(void*s)override{auto*t=static_cast<const AiEntry*>(readptr(s,0));t[1](s,nullptr);}
 void initialize_table2(void*s)override{auto*t=static_cast<const AiEntry*>(readptr(s,0));t[1](s,nullptr);}
};
#include "c7026f3d03-vm_commands_native_entering.inc"
extern "C" int __fastcall recovered_vm_action_start_conversation_0052d5b0(void*,void*,int,unsigned);
extern "C" int __fastcall recovered_vm_begin_conversation_0052e9f0(void*,void*,int,unsigned); // lane dialogue-start (claude_dialogue_start.cpp)
extern "C" int __fastcall recovered_vm_action_equip_most_damaging_melee_0052c7b0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0052C7B0
extern "C" int __fastcall recovered_vm_action_equip_most_damaging_ranged_0052c820(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0052C820
extern "C" int __fastcall recovered_vm_action_jump_to_object_0052cce0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0052CCE0
extern "C" int __fastcall recovered_vm_action_jump_to_point_0052cdc0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0052CDC0
extern "C" int __fastcall recovered_vm_action_random_walk_0052d440(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0052D440
extern "C" int __fastcall recovered_vm_action_use_talent_0052daf0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0052DAF0
extern "C" int __fastcall recovered_vm_action_attack_0052e7f0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0052E7F0
extern "C" int __fastcall recovered_vm_cutscene_attack_0052e890(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0052E890
extern "C" int __fastcall recovered_vm_cast_spell_0052ee50(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0052EE50
extern "C" int __fastcall recovered_vm_change_faction_0052f2f0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0052F2F0
extern "C" int __fastcall recovered_vm_do_door_action_00530360(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00530360
extern "C" int __fastcall recovered_vm_effect_beam_00531260(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00531260
extern "C" int __fastcall recovered_vm_effect_damage_00531ae0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00531AE0
extern "C" int __fastcall recovered_vm_effect_heal_00532a70(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00532A70
extern "C" int __fastcall recovered_vm_effect_link_effects_00532f80(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00532F80
extern "C" int __fastcall recovered_vm_effect_resurrection_00533660(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00533660
extern "C" int __fastcall recovered_vm_effect_visual_effect_005346b0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 005346B0
extern "C" int __fastcall recovered_vm_effect_force_push_targeted_00534ab0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00534AB0
extern "C" int __fastcall recovered_vm_effect_choke_005351e0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 005351E0
extern "C" int __fastcall recovered_vm_action_equip_item_005355f0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 005355F0
extern "C" int __fastcall recovered_vm_find_sub_string_00535c90(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00535C90
extern "C" int __fastcall recovered_vm_float_to_int_00535fb0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00535FB0
extern "C" int __fastcall recovered_vm_float_to_string_00536000(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00536000
extern "C" int __fastcall recovered_vm_get_area_005366a0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 005366A0
extern "C" int __fastcall recovered_vm_get_spell_target_00536950(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00536950
extern "C" int __fastcall recovered_vm_get_base_item_type_005369b0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 005369B0
extern "C" int __fastcall recovered_vm_get_distance_between_00537370(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00537370
extern "C" int __fastcall recovered_vm_get_distance_between_2d_00537470(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00537470
extern "C" int __fastcall recovered_vm_get_effect_type_00537c70(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00537C70
extern "C" int __fastcall recovered_vm_get_facing_00537fe0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00537FE0
extern "C" int __fastcall recovered_vm_get_has_feat_00539170(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00539170
extern "C" int __fastcall recovered_vm_get_has_spell_effect_005394d0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 005394D0
extern "C" int __fastcall recovered_vm_get_is_door_action_possible_00539940(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00539940
extern "C" int __fastcall recovered_vm_get_is_in_combat_00539d80(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00539D80
extern "C" int __fastcall recovered_vm_get_is_open_00539e20(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00539E20
extern "C" int __fastcall recovered_vm_get_item_possessed_by_0053a390(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053A390
extern "C" int __fastcall recovered_vm_get_last_attack_result_0053aad0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053AAD0
extern "C" int __fastcall recovered_vm_get_was_force_power_successful_0053ab40(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053AB40
extern "C" int __fastcall recovered_vm_get_last_opened_by_0053b170(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053B170
extern "C" int __fastcall recovered_vm_get_last_used_by_0053b570(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053B570
extern "C" int __fastcall recovered_vm_get_exiting_object_0053b640(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053B640
extern "C" int __fastcall recovered_vm_get_location_0053b840(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053B840
extern "C" int __fastcall recovered_vm_get_module_0053bdf0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053BDF0
extern "C" int __fastcall recovered_vm_get_module_name_0053bec0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053BEC0
extern "C" int __fastcall recovered_vm_get_num_stacked_items_0053c220(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053C220
extern "C" int __fastcall recovered_vm_get_skill_rank_0053d050(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053D050
extern "C" int __fastcall recovered_vm_get_gender_0053d5a0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053D5A0
extern "C" int __fastcall recovered_vm_get_string_by_strref_0053d610(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053D610
extern "C" int __fastcall recovered_vm_get_string_left_0053d720(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053D720
extern "C" int __fastcall recovered_vm_get_string_length_0053d860(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053D860
extern "C" int __fastcall recovered_vm_get_string_right_0053da40(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053DA40
extern "C" int __fastcall recovered_vm_get_sub_string_0053dda0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053DDA0
extern "C" int __fastcall recovered_vm_get_total_damage_dealt_0053e120(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053E120
extern "C" int __fastcall recovered_vm_get_xp_0053e600(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053E600
extern "C" int __fastcall recovered_vm_get_is_in_conversation_0053eec0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053EEC0
extern "C" int __fastcall recovered_vm_action_move_away_from_object_0053f990(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053F990
extern "C" int __fastcall recovered_vm_move_to_point_0053fe00(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053FE00
extern "C" int __fastcall recovered_vm_music_background_0053ffb0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0053FFB0
extern "C" int __fastcall recovered_vm_music_battle_005400c0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 005400C0
extern "C" int __fastcall recovered_vm_play_rumble_pattern_00541120(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00541120
extern "C" int __fastcall recovered_vm_print_string_005417c0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 005417C0
extern "C" int __fastcall recovered_vm_remove_available_npc_00541b40(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00541B40
extern "C" int __fastcall recovered_vm_remove_party_member_00541c00(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00541C00
extern "C" int __fastcall recovered_vm_set_lightsaber_powered_00542f10(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00542F10
extern "C" int __fastcall recovered_vm_set_plot_flag_005433d0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 005433D0
extern "C" int __fastcall recovered_vm_set_xp_005437b0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 005437B0
extern "C" int __fastcall recovered_vm_show_party_selection_gui_005438b0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 005438B0
extern "C" int __fastcall recovered_vm_sound_object_play_00543be0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00543BE0
extern "C" int __fastcall recovered_vm_sound_object_stop_00543e10(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00543E10
extern "C" int __fastcall recovered_vm_string_conversions_00544710(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00544710
extern "C" int __fastcall recovered_vm_action_unequip_item_00545050(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00545050
extern "C" int __fastcall recovered_vm_action_wait_00545800(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00545800
extern "C" int __fastcall recovered_vm_clear_all_effects_00545d40(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00545D40
extern "C" int __fastcall recovered_vm_give_plot_xp_00545e20(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00545E20
extern "C" int __fastcall recovered_vm_get_sub_screen_id_005464d0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 005464D0
extern "C" int __fastcall recovered_vm_pause_game_00546590(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00546590
extern "C" int __fastcall recovered_vm_get_item_stack_size_005465d0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 005465D0
extern "C" int __fastcall recovered_vm_set_item_stack_size_00546630(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00546630
extern "C" int __fastcall recovered_vm_set_dialog_placeable_camera_00546a80(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00546A80
extern "C" int __fastcall recovered_vm_award_stealth_xp_00546c80(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00546C80
extern "C" int __fastcall recovered_vm_get_blocking_creature_00546e00(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00546E00
extern "C" int __fastcall recovered_vm_get_found_enemy_creature_00546e70(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00546E70
extern "C" int __fastcall recovered_vm_set_lock_orientation_in_dialog_005470e0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 005470E0
extern "C" int __fastcall recovered_vm_set_lock_head_follow_in_dialog_00547160(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00547160
extern "C" int __fastcall recovered_vm_show_tutorial_window_00547340(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00547340
extern "C" int __fastcall recovered_vm_reset_dialog_state_00547bc0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00547BC0
extern "C" int __fastcall recovered_vm_no_clicks_for_00547dc0(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00547DC0
extern "C" int __fastcall recovered_vm_bark_string_00548900(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00548900
extern "C" int __fastcall recovered_vm_get_in_persistent_object_00549360(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00549360
extern "C" int __fastcall recovered_vm_get_item_in_inventory_00549b50(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 00549B50
extern "C" int __fastcall recovered_vm_get_object_in_shape_0054a260(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0054A260
extern "C" int __fastcall recovered_vm_set_custom_token_0054b090(void*,void*,int,unsigned); // lane endar-actions (srcw5/actions/claude_vm_actions_*.cpp) original 0054B090
extern "C" int __fastcall recovered_vm_get_position_0053cae0(void*,void*,int,unsigned);
extern "C" int __fastcall recovered_vm_get_last_attacker_0053a880(void*,void*,int,unsigned);
extern "C" int __fastcall recovered_vm_set_facing_00542830(void*,void*,int,unsigned);
extern "C" int __fastcall recovered_vm_get_solo_mode_00546af0(void*,void*,int,unsigned);
extern "C" int __fastcall recovered_vm_get_last_hostile_target_0053a900(void*,void*,int,unsigned);
extern "C" int __fastcall recovered_vm_get_nearest_object_0054b550(void*,void*,int,unsigned);
extern "C" int __fastcall recovered_vm_get_is_debilitated_005462d0(void*,void*,int,unsigned);
extern "C" int __fastcall recovered_vm_get_last_attack_action_0053a970(void*,void*,int,unsigned);
extern "C" int __fastcall recovered_vm_get_last_force_power_used_0053aed0(void*,void*,int,unsigned);
extern "C" int __fastcall recovered_vm_get_last_combat_feat_used_0053acd0(void*,void*,int,unsigned);
extern "C" int __fastcall recovered_vm_talent_00544c70(void*,void*,int,unsigned);
extern "C" int __fastcall recovered_vm_get_is_conversation_active_0053ef70(void*,void*,int,unsigned);
extern "C" int __fastcall recovered_vm_get_attempted_movement_target_00536900(void*,void*,int,unsigned);
extern "C" int __fastcall recovered_vm_clear_all_actions_0052f4a0(void*,void*,int,unsigned);
struct Commands:recovered_vm_commands::Ports {void*allocate(unsigned n)override{return ::operator new(n);}const void*resolve(unsigned a)override{if(a==0x00539380u)return reinterpret_cast<const void*>(&recovered_vm_get_has_skill_00539380);if(a==0x00539610u)return reinterpret_cast<const void*>(&recovered_vm_get_hitpoints_00539610);if(a==0x00549180u)return reinterpret_cast<const void*>(&recovered_vm_get_effect_00549180);if(a==0x0053ee00u)return reinterpret_cast<const void*>(&recovered_vm_get_is_effect_valid_0053ee00);if(a==0x005420b0u)return reinterpret_cast<const void*>(&recovered_vm_roll_dice_005420b0);if(a==0x0053a2c0u)return reinterpret_cast<const void*>(&recovered_vm_get_item_in_slot_0053a2c0);if(a==0x0053eaa0u)return reinterpret_cast<const void*>(&recovered_vm_int_to_float_0053eaa0);if(a==0x00536ec0u)return reinterpret_cast<const void*>(&recovered_vm_get_creature_has_talent_00536ec0);if(a==0x00536fe0u)return reinterpret_cast<const void*>(&recovered_vm_get_creature_talent_00536fe0);if(a==0x00539f90u)return reinterpret_cast<const void*>(&recovered_vm_get_is_talent_valid_00539f90);if(a==0x00544d70u)return reinterpret_cast<const void*>(&recovered_vm_talent_access_00544d70);if(a==0x005434c0u)return reinterpret_cast<const void*>(&recovered_vm_set_return_strref_005434c0);if(a==0x0053e750u)return reinterpret_cast<const void*>(&recovered_vm_give_xp_to_creature_0053e750);if(a==0x00540ed0u)return reinterpret_cast<const void*>(&recovered_vm_play_movie_00540ed0);if(a==0x0052cbe0u)return reinterpret_cast<const void*>(&recovered_vm_action_follow_leader_0052cbe0);if(a==0x0052f4a0u)return reinterpret_cast<const void*>(&recovered_vm_clear_all_actions_0052f4a0);if(a==0x00536900u)return reinterpret_cast<const void*>(&recovered_vm_get_attempted_movement_target_00536900);if(a==0x0053ef70u)return reinterpret_cast<const void*>(&recovered_vm_get_is_conversation_active_0053ef70);if(a==0x00544c70u)return reinterpret_cast<const void*>(&recovered_vm_talent_00544c70);if(a==0x0053aed0u)return reinterpret_cast<const void*>(&recovered_vm_get_last_force_power_used_0053aed0);if(a==0x0053acd0u)return reinterpret_cast<const void*>(&recovered_vm_get_last_combat_feat_used_0053acd0);if(a==0x0053a970u)return reinterpret_cast<const void*>(&recovered_vm_get_last_attack_action_0053a970);if(a==0x0052d5b0u)return reinterpret_cast<const void*>(&recovered_vm_action_start_conversation_0052d5b0);if(a==0x0052e9f0u)return reinterpret_cast<const void*>(&recovered_vm_begin_conversation_0052e9f0);if(a==0x0052c7b0u)return reinterpret_cast<const void*>(&recovered_vm_action_equip_most_damaging_melee_0052c7b0);if(a==0x0052c820u)return reinterpret_cast<const void*>(&recovered_vm_action_equip_most_damaging_ranged_0052c820);if(a==0x0052cce0u)return reinterpret_cast<const void*>(&recovered_vm_action_jump_to_object_0052cce0);if(a==0x0052cdc0u)return reinterpret_cast<const void*>(&recovered_vm_action_jump_to_point_0052cdc0);if(a==0x0052d440u)return reinterpret_cast<const void*>(&recovered_vm_action_random_walk_0052d440);if(a==0x0052daf0u)return reinterpret_cast<const void*>(&recovered_vm_action_use_talent_0052daf0);if(a==0x0052e7f0u)return reinterpret_cast<const void*>(&recovered_vm_action_attack_0052e7f0);if(a==0x0052e890u)return reinterpret_cast<const void*>(&recovered_vm_cutscene_attack_0052e890);if(a==0x0052ee50u)return reinterpret_cast<const void*>(&recovered_vm_cast_spell_0052ee50);if(a==0x0052f2f0u)return reinterpret_cast<const void*>(&recovered_vm_change_faction_0052f2f0);if(a==0x00530360u)return reinterpret_cast<const void*>(&recovered_vm_do_door_action_00530360);if(a==0x00531260u)return reinterpret_cast<const void*>(&recovered_vm_effect_beam_00531260);if(a==0x00531ae0u)return reinterpret_cast<const void*>(&recovered_vm_effect_damage_00531ae0);if(a==0x00532a70u)return reinterpret_cast<const void*>(&recovered_vm_effect_heal_00532a70);if(a==0x00532f80u)return reinterpret_cast<const void*>(&recovered_vm_effect_link_effects_00532f80);if(a==0x00533660u)return reinterpret_cast<const void*>(&recovered_vm_effect_resurrection_00533660);if(a==0x005346b0u)return reinterpret_cast<const void*>(&recovered_vm_effect_visual_effect_005346b0);if(a==0x00534ab0u)return reinterpret_cast<const void*>(&recovered_vm_effect_force_push_targeted_00534ab0);if(a==0x005351e0u)return reinterpret_cast<const void*>(&recovered_vm_effect_choke_005351e0);if(a==0x005355f0u)return reinterpret_cast<const void*>(&recovered_vm_action_equip_item_005355f0);if(a==0x00535c90u)return reinterpret_cast<const void*>(&recovered_vm_find_sub_string_00535c90);if(a==0x00535fb0u)return reinterpret_cast<const void*>(&recovered_vm_float_to_int_00535fb0);if(a==0x00536000u)return reinterpret_cast<const void*>(&recovered_vm_float_to_string_00536000);if(a==0x005366a0u)return reinterpret_cast<const void*>(&recovered_vm_get_area_005366a0);if(a==0x00536950u)return reinterpret_cast<const void*>(&recovered_vm_get_spell_target_00536950);if(a==0x005369b0u)return reinterpret_cast<const void*>(&recovered_vm_get_base_item_type_005369b0);if(a==0x00537370u)return reinterpret_cast<const void*>(&recovered_vm_get_distance_between_00537370);if(a==0x00537470u)return reinterpret_cast<const void*>(&recovered_vm_get_distance_between_2d_00537470);if(a==0x00537c70u)return reinterpret_cast<const void*>(&recovered_vm_get_effect_type_00537c70);if(a==0x00537fe0u)return reinterpret_cast<const void*>(&recovered_vm_get_facing_00537fe0);if(a==0x00539170u)return reinterpret_cast<const void*>(&recovered_vm_get_has_feat_00539170);if(a==0x005394d0u)return reinterpret_cast<const void*>(&recovered_vm_get_has_spell_effect_005394d0);if(a==0x00539940u)return reinterpret_cast<const void*>(&recovered_vm_get_is_door_action_possible_00539940);if(a==0x00539d80u)return reinterpret_cast<const void*>(&recovered_vm_get_is_in_combat_00539d80);if(a==0x00539e20u)return reinterpret_cast<const void*>(&recovered_vm_get_is_open_00539e20);if(a==0x0053a390u)return reinterpret_cast<const void*>(&recovered_vm_get_item_possessed_by_0053a390);if(a==0x0053aad0u)return reinterpret_cast<const void*>(&recovered_vm_get_last_attack_result_0053aad0);if(a==0x0053ab40u)return reinterpret_cast<const void*>(&recovered_vm_get_was_force_power_successful_0053ab40);if(a==0x0053b170u)return reinterpret_cast<const void*>(&recovered_vm_get_last_opened_by_0053b170);if(a==0x0053b570u)return reinterpret_cast<const void*>(&recovered_vm_get_last_used_by_0053b570);if(a==0x0053b640u)return reinterpret_cast<const void*>(&recovered_vm_get_exiting_object_0053b640);if(a==0x0053b840u)return reinterpret_cast<const void*>(&recovered_vm_get_location_0053b840);if(a==0x0053bdf0u)return reinterpret_cast<const void*>(&recovered_vm_get_module_0053bdf0);if(a==0x0053bec0u)return reinterpret_cast<const void*>(&recovered_vm_get_module_name_0053bec0);if(a==0x0053c220u)return reinterpret_cast<const void*>(&recovered_vm_get_num_stacked_items_0053c220);if(a==0x0053d050u)return reinterpret_cast<const void*>(&recovered_vm_get_skill_rank_0053d050);if(a==0x0053d5a0u)return reinterpret_cast<const void*>(&recovered_vm_get_gender_0053d5a0);if(a==0x0053d610u)return reinterpret_cast<const void*>(&recovered_vm_get_string_by_strref_0053d610);if(a==0x0053d720u)return reinterpret_cast<const void*>(&recovered_vm_get_string_left_0053d720);if(a==0x0053d860u)return reinterpret_cast<const void*>(&recovered_vm_get_string_length_0053d860);if(a==0x0053da40u)return reinterpret_cast<const void*>(&recovered_vm_get_string_right_0053da40);if(a==0x0053dda0u)return reinterpret_cast<const void*>(&recovered_vm_get_sub_string_0053dda0);if(a==0x0053e120u)return reinterpret_cast<const void*>(&recovered_vm_get_total_damage_dealt_0053e120);if(a==0x0053e600u)return reinterpret_cast<const void*>(&recovered_vm_get_xp_0053e600);if(a==0x0053eec0u)return reinterpret_cast<const void*>(&recovered_vm_get_is_in_conversation_0053eec0);if(a==0x0053f990u)return reinterpret_cast<const void*>(&recovered_vm_action_move_away_from_object_0053f990);if(a==0x0053fe00u)return reinterpret_cast<const void*>(&recovered_vm_move_to_point_0053fe00);if(a==0x0053ffb0u)return reinterpret_cast<const void*>(&recovered_vm_music_background_0053ffb0);if(a==0x005400c0u)return reinterpret_cast<const void*>(&recovered_vm_music_battle_005400c0);if(a==0x00541120u)return reinterpret_cast<const void*>(&recovered_vm_play_rumble_pattern_00541120);if(a==0x005417c0u)return reinterpret_cast<const void*>(&recovered_vm_print_string_005417c0);if(a==0x00541b40u)return reinterpret_cast<const void*>(&recovered_vm_remove_available_npc_00541b40);if(a==0x00541c00u)return reinterpret_cast<const void*>(&recovered_vm_remove_party_member_00541c00);if(a==0x00542f10u)return reinterpret_cast<const void*>(&recovered_vm_set_lightsaber_powered_00542f10);if(a==0x005433d0u)return reinterpret_cast<const void*>(&recovered_vm_set_plot_flag_005433d0);if(a==0x005437b0u)return reinterpret_cast<const void*>(&recovered_vm_set_xp_005437b0);if(a==0x005438b0u)return reinterpret_cast<const void*>(&recovered_vm_show_party_selection_gui_005438b0);if(a==0x00543be0u)return reinterpret_cast<const void*>(&recovered_vm_sound_object_play_00543be0);if(a==0x00543e10u)return reinterpret_cast<const void*>(&recovered_vm_sound_object_stop_00543e10);if(a==0x00544710u)return reinterpret_cast<const void*>(&recovered_vm_string_conversions_00544710);if(a==0x00545050u)return reinterpret_cast<const void*>(&recovered_vm_action_unequip_item_00545050);if(a==0x00545800u)return reinterpret_cast<const void*>(&recovered_vm_action_wait_00545800);if(a==0x00545d40u)return reinterpret_cast<const void*>(&recovered_vm_clear_all_effects_00545d40);if(a==0x00545e20u)return reinterpret_cast<const void*>(&recovered_vm_give_plot_xp_00545e20);if(a==0x005464d0u)return reinterpret_cast<const void*>(&recovered_vm_get_sub_screen_id_005464d0);if(a==0x00546590u)return reinterpret_cast<const void*>(&recovered_vm_pause_game_00546590);if(a==0x005465d0u)return reinterpret_cast<const void*>(&recovered_vm_get_item_stack_size_005465d0);if(a==0x00546630u)return reinterpret_cast<const void*>(&recovered_vm_set_item_stack_size_00546630);if(a==0x00546a80u)return reinterpret_cast<const void*>(&recovered_vm_set_dialog_placeable_camera_00546a80);if(a==0x00546c80u)return reinterpret_cast<const void*>(&recovered_vm_award_stealth_xp_00546c80);if(a==0x00546e00u)return reinterpret_cast<const void*>(&recovered_vm_get_blocking_creature_00546e00);if(a==0x00546e70u)return reinterpret_cast<const void*>(&recovered_vm_get_found_enemy_creature_00546e70);if(a==0x005470e0u)return reinterpret_cast<const void*>(&recovered_vm_set_lock_orientation_in_dialog_005470e0);if(a==0x00547160u)return reinterpret_cast<const void*>(&recovered_vm_set_lock_head_follow_in_dialog_00547160);if(a==0x00547340u)return reinterpret_cast<const void*>(&recovered_vm_show_tutorial_window_00547340);if(a==0x00547bc0u)return reinterpret_cast<const void*>(&recovered_vm_reset_dialog_state_00547bc0);if(a==0x00547dc0u)return reinterpret_cast<const void*>(&recovered_vm_no_clicks_for_00547dc0);if(a==0x00548900u)return reinterpret_cast<const void*>(&recovered_vm_bark_string_00548900);if(a==0x00549360u)return reinterpret_cast<const void*>(&recovered_vm_get_in_persistent_object_00549360);if(a==0x00549b50u)return reinterpret_cast<const void*>(&recovered_vm_get_item_in_inventory_00549b50);if(a==0x0054a260u)return reinterpret_cast<const void*>(&recovered_vm_get_object_in_shape_0054a260);if(a==0x0054b090u)return reinterpret_cast<const void*>(&recovered_vm_set_custom_token_0054b090);if(a==0x0053cae0u)return reinterpret_cast<const void*>(&recovered_vm_get_position_0053cae0);if(a==0x0053a880u)return reinterpret_cast<const void*>(&recovered_vm_get_last_attacker_0053a880);if(a==0x00542830u)return reinterpret_cast<const void*>(&recovered_vm_set_facing_00542830);if(a==0x00546af0u)return reinterpret_cast<const void*>(&recovered_vm_get_solo_mode_00546af0);if(a==0x0053a900u)return reinterpret_cast<const void*>(&recovered_vm_get_last_hostile_target_0053a900);return a==0x0054b550u?reinterpret_cast<const void*>(&recovered_vm_get_nearest_object_0054b550):a==0x005462d0u?reinterpret_cast<const void*>(&recovered_vm_get_is_debilitated_005462d0):vm_resolve(a);}};
void __fastcall initialize_commands(void*s,void*){Commands p;recovered_vm_commands::initialize_0054c960(s,p);}
#include "5a827be7df-vm_execute_native_exports.hpp"
// closeC: reconstructed former VM_GATE slots (thiscall; RET from the original).
//   005D1C20 CResHelper<CResNCS,2010> scalar deleting dtor (RET 4) -> 005D1A60 ~CResHelper
//   005D4C10 CVirtualMachineInternal scalar deleting dtor (RET 4) -> 005D49B0 ~CVirtualMachineInternal
//   004B16E0 CSWVirtualMachineCommands scalar deleting dtor (RET 4) -> 0052C070; base 007450AC {004B1340, _purecall x10}
//   0052C580 CreateGameDefinedStructure(int type) RET 4
//   0052C2D0 SaveGameDefinedStructure(int type, void* value, CResGFF*, CResStruct*) RET 10h
//   0052C360 LoadGameDefinedStructure(int type, void** out, CResGFF*, CResStruct*) RET 10h
// Quirks kept: unchecked allocations in Load (the null object is stored and its loader called, as the original);
//   type 0 creates CGameEffect(0) in Create but CGameEffect(1) in Load; Load type 1/2/3 return the loader's EAX
//   (LoadLocation / LoadTalent / LoadEvent always 1) and Save type 1/2/3 the saver's EAX (always 1).
//   LoadGameEffect 005043A0: legacy Type 26h/23h effects with fewer than 21 integers read "IntList" with the original
//   index walk (element index advances only for i < n-6 and i >= 15, zeros written at the stalled index for
//   n-6 <= i < 15; a list shorter than that walk never terminates, as the original) and resize to 21 afterwards;
//   other effects read every "IntList" element without a bound check; FloatList / ObjectList read 4 and StringList 6
//   elements without a count check.
void* __fastcall resource_helper_deleting_005d1c20(void*,void*,unsigned);
void* __fastcall vm_internal_deleting_005d4c10(void*,void*,unsigned);
void* __fastcall commands_deleting_004b16e0(void*,void*,unsigned);
void* __fastcall implementer_deleting_004b1340(void*,void*,unsigned);
void* __fastcall create_structure_0052c580(void*,void*,int);
unsigned __fastcall save_structure_0052c2d0(void*,void*,int,void*,void*,void*);
unsigned __fastcall load_structure_0052c360(void*,void*,int,void**,void*,void*);
static const AiEntry vm_base_destructor=reinterpret_cast<AiEntry>(reinterpret_cast<void(*)()>(&resource_helper_deleting_005d1c20));
static const AiEntry vm_derived_destructor=reinterpret_cast<AiEntry>(reinterpret_cast<void(*)()>(&vm_internal_deleting_005d4c10));
static const AiEntry command_slot0=reinterpret_cast<AiEntry>(reinterpret_cast<void(*)()>(&commands_deleting_004b16e0));
int __fastcall command_slot2(void* self,void* edx,int routine,unsigned count){return recovered_vm_execute_command_0052c0d0(self,edx,routine,count);}
void __fastcall command_slot3(void* self,void* edx,void* script){recovered_vm_script_trace_0052c100(self,edx,script);}
static const AiEntry command_slot4=reinterpret_cast<AiEntry>(reinterpret_cast<void(*)()>(&gui_load_slot_ret8_005b5e90));
static const AiEntry command_slot5=reinterpret_cast<AiEntry>(reinterpret_cast<void(*)()>(&create_structure_0052c580));
static const AiEntry command_slot6=reinterpret_cast<AiEntry>(reinterpret_cast<void(*)()>(&recovered_vm_commands_destroy_engine_00548360));
static const AiEntry command_slot7=reinterpret_cast<AiEntry>(reinterpret_cast<void(*)()>(&recovered_vm_commands_equal_engine_0052c510));
static const AiEntry command_slot8=reinterpret_cast<AiEntry>(reinterpret_cast<void(*)()>(&recovered_vm_commands_copy_engine_0052c140));
static const AiEntry command_slot9=reinterpret_cast<AiEntry>(reinterpret_cast<void(*)()>(&save_structure_0052c2d0));
static const AiEntry command_slot10=reinterpret_cast<AiEntry>(reinterpret_cast<void(*)()>(&load_structure_0052c360));
const AiEntry vm_base_table_0074c490[]={vm_base_destructor};
const AiEntry vm_derived_table_0074c4fc[]={vm_derived_destructor};
struct CommandTable {AiEntry slot0,slot1;int(__fastcall*slot2)(void*,void*,int,unsigned);void(__fastcall*slot3)(void*,void*,void*);AiEntry slot4,slot5,slot6,slot7,slot8,slot9,slot10;};
static_assert(sizeof(CommandTable)==44,"Canonical eleven-slot command table");
const CommandTable command_table_007450e4={command_slot0,initialize_commands,command_slot2,command_slot3,command_slot4,command_slot5,command_slot6,command_slot7,command_slot8,command_slot9,command_slot10};
// ---- closeC: bodies of the reconstructed VM slots (declared above the tables) ----
struct BaseTable11 { void*(__fastcall* destroy)(void*,void*,unsigned); PureEntry slot[10]; };
const BaseTable11 base_table_007450ac={implementer_deleting_004b1340,{_purecall,_purecall,_purecall,_purecall,_purecall,_purecall,_purecall,_purecall,_purecall,_purecall}};
inline unsigned rd32(const void* p,unsigned off){unsigned v;std::memcpy(&v,static_cast<const unsigned char*>(p)+off,4);return v;}
inline void wr32(void* p,unsigned off,unsigned v){std::memcpy(static_cast<unsigned char*>(p)+off,&v,4);}
inline void* at_ptr(void* p,unsigned off){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(rd32(p,off)));}
void* __fastcall implementer_deleting_004b1340(void* self,void*,unsigned flags){
 *static_cast<const void**>(self)=&base_table_007450ac;                         // 004B1348
 if((flags&1u)!=0)live_free(self);                                               // 004B1340, 004B134E..56
 return self;
}
void* __fastcall commands_deleting_004b16e0(void* self,void*,unsigned flags){
 *static_cast<const void**>(self)=&command_table_007450e4;                      // 0052C08D
 free_member_closec(self,0xc);                                                   // 0052C093..AB command table
 *static_cast<const void**>(self)=&base_table_007450ac;                         // 0052C0B6
 if((flags&1u)!=0)live_free(self);                                               // 004B16E8..F5
 return self;
}
// 005D1A60 ~CResHelper<CResNCS,2010>: +4 demanded flag, +8 CRes*, +C CResRef; release through the resource manager and
// delete the resource (virtual slot 0, flag 1) only when the manager did not take it back.
void resource_helper_destroy_005d1a60(void* self){
 *static_cast<const void**>(self)=vm_base_table_0074c490;                       // 005D1A68
 if(rd32(self,8)==0)return;                                                      // 005D1A63..6E
 if(c2da_not_equal_text_00406060(static_cast<unsigned char*>(self)+0xc,nullptr,"")==0)return; // 005D1A70..7F ([0073D71C])
 if(rd32(self,4)!=0){c2da_cancel_00409b00(at_ptr(self,8),nullptr);wr32(self,4,0);} // 005D1A81..90
 if(c2da_release_00409cf0(reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::resource_manager_007a39e8)),nullptr,at_ptr(self,8))!=0)return; // 005D1A97..A8
 void* const resource=at_ptr(self,8);                                            // 005D1AAA
 if(resource!=nullptr){using Destroy=void*(__thiscall*)(void*,unsigned);(void)(*static_cast<Destroy* const*>(resource))[0](resource,1u);} // 005D1AAD..B5
 wr32(self,8,0);                                                                 // 005D1AB7
}
void* __fastcall resource_helper_deleting_005d1c20(void* self,void*,unsigned flags){
 resource_helper_destroy_005d1a60(self);                                         // 005D1C23
 if((flags&1u)!=0)live_free(self);                                               // 005D1C28..35
 return self;
}
// 005D4550 ~CVirtualMachineScript (28h bytes): +0 stack (ClearStack + free), +10h code (free), +18h CExoString name.
void __fastcall vm_script_destroy_005d4550(void* script,void*){
 void* const stack=at_ptr(script,0);                                             // 005D456E
 if(stack!=nullptr){recovered_vm_stack_clear_005d2140(stack,nullptr);live_free(stack);} // 005D4570..89
 wr32(script,0,0);                                                               // 005D4591
 free_member_closec(script,0x10);                                                // 005D458C..A2
 frame_LogStringDtor_005e5c20(static_cast<unsigned char*>(script)+0x18,nullptr); // 005D45A9..B4
}
void* __fastcall vm_internal_deleting_005d4c10(void* self,void*,unsigned flags){
 *static_cast<const void**>(self)=vm_derived_table_0074c4fc;                    // 005D49D1
 for(unsigned k=0;k<8;++k){                                                      // 005D49E2..3A eight script slots at +2Ch
  void* const script=static_cast<unsigned char*>(self)+0x2c+k*0x28;
  void* const stack=at_ptr(script,0);                                            // 005D49F0
  if(stack!=nullptr){recovered_vm_stack_clear_005d2140(stack,nullptr);live_free(stack);wr32(script,0,0);} // 005D49F3..07
  free_member_closec(script,0x10);                                               // 005D4A0A..19
  wr32(script,4,0);                                                              // 005D4A1F
  wr32(script,8,0xd);                                                            // 005D4A22
  wr32(script,0xc,0);                                                            // 005D4A29
  wr32(script,0x14,0);                                                           // 005D4A2C
  wr32(script,0x24,0);                                                           // 005D4A2F
 }
 recovered_vm_stack_clear_005d2140(static_cast<unsigned char*>(self)+0x1ac,nullptr); // 005D4A3C..47 (+1ACh stack member)
 c2da_destroy_array_006fa852(static_cast<unsigned char*>(self)+0x2c,0x28,8,
                             reinterpret_cast<void*>(&vm_script_destroy_005d4550)); // 005D4A4C..5D ~CVirtualMachineScript[8]
 resource_helper_destroy_005d1a60(self);                                         // 005D4A6D..B7 (inlined base dtor)
 if((flags&1u)!=0)live_free(self);                                               // 005D4C18..25
 return self;
}
void* __fastcall create_structure_0052c580(void*,void*,int type){
 if(type==0){void* m=live_operator_new(0x8c);return m!=nullptr?recovered_effect_construct_00503e40(m,nullptr,0):nullptr;} // 0052C58E..C2
 if(type==1){void* m=live_operator_new(0x34);return m!=nullptr?recovered_event_construct_004d7540(m,nullptr):nullptr;}     // 0052C5D8..FD
 if(type==2){void* m=live_operator_new(0x18);return m!=nullptr?kotor_fastcall_004ca7a0(m,nullptr):nullptr;}                // 0052C613..34
 if(type==3){void* m=live_operator_new(0x18);return m!=nullptr?recovered_talent_construct_004ca9e0(m,nullptr):nullptr;}    // 0052C64A..6B
 return nullptr;                                                                 // 0052C681..91
}
// 004CAA50 CScriptTalent::SaveTalent(CResGFF*, CResStruct*) RET 8
unsigned save_talent_004caa50(void* talent,void* gff,void* st){
 recovered_gff_004127b0(gff,nullptr,st,rd32(talent,4),"ID");                    // 004CAA5D..69
 recovered_gff_004127b0(gff,nullptr,st,rd32(talent,0),"Type");                  // 004CAA6E..79
 recovered_gff_00412620(gff,nullptr,st,static_cast<const unsigned char*>(talent)[8],"MultiClass");        // 004CAA7E..8C
 recovered_gff_00412760(gff,nullptr,st,rd32(talent,0xc),"Item");                // 004CAA91..9D
 recovered_gff_004127b0(gff,nullptr,st,rd32(talent,0x10),"ItemPropertyIndex");  // 004CAAA2..AE
 recovered_gff_00412620(gff,nullptr,st,static_cast<const unsigned char*>(talent)[0x14],"CasterLevel");    // 004CAAB3..C1
 recovered_gff_00412620(gff,nullptr,st,static_cast<const unsigned char*>(talent)[0x15],"MetaType");       // 004CAAC6..D4
 return 1;                                                                       // 004CAADB
}
// 004CAAF0 CScriptTalent::LoadTalent(CResGFF*, CResStruct*) RET 8 (status out-parameter = structure argument slot)
unsigned load_talent_004caaf0(void* talent,void* gff,void* st){
 unsigned ok=0;
 wr32(talent,4,static_cast<unsigned>(gui_load_gff_int_00411c90(gff,nullptr,st,"ID",&ok,0)));            // 004CAAFB..20
 wr32(talent,0,static_cast<unsigned>(gui_load_gff_int_00411c90(gff,nullptr,st,"Type",&ok,0)));          // 004CAB11..37
 static_cast<unsigned char*>(talent)[8]=gui_load_gff_byte_00411a60(gff,nullptr,st,"MultiClass",&ok,0);  // 004CAB28..40
 wr32(talent,0xc,gui_load_gff_dword_00411c20(gff,nullptr,st,"Item",&ok,0));                             // 004CAB3E..64
 wr32(talent,0x10,static_cast<unsigned>(gui_load_gff_int_00411c90(gff,nullptr,st,"ItemPropertyIndex",&ok,0))); // 004CAB55..7B
 static_cast<unsigned char*>(talent)[0x14]=gui_load_gff_byte_00411a60(gff,nullptr,st,"CasterLevel",&ok,0); // 004CAB6C..83
 static_cast<unsigned char*>(talent)[0x15]=gui_load_gff_byte_00411a60(gff,nullptr,st,"MetaType",&ok,0);  // 004CAB83..9B
 return 1;                                                                       // 004CAB9F
}
// 00411D70 CResGFF::ReadFieldDWORD64(CResStruct*, char* label, int* ok, DWORD64 default) RET 14h
std::uint64_t read_dword64_00411d70(void* gff,void* st,const char* label,unsigned* ok,std::uint64_t fallback){
 if(st==nullptr||label==nullptr){*ok=0;return fallback;}                         // 00411D72..82, 00411DF9..0D
 kotor_q05::FieldRecord* const field=static_cast<kotor_q05::CResGFF*>(gff)->GetField(static_cast<kotor_q05::StructHandle*>(st),
                                       gff_label_00411630(gff,nullptr,st,label)); // 00411D84..8F
 if(field==nullptr||*reinterpret_cast<const unsigned*>(field)!=6){*ok=0;return fallback;} // 00411D94..9B, 00411DE2..F6
 unsigned size=0;                                                                // [esp+0Ch] (structure slot)
 const unsigned data=static_cast<kotor_q05::CResGFF*>(gff)->GetDataField(field,&size); // 00411D9D..A5
 if(data==0||size<8){*ok=0;return fallback;}                                     // 00411DAC..B5, 00411DCB..DF
 *ok=1;                                                                          // 00411DB7..BB
 std::uint64_t v;std::memcpy(&v,reinterpret_cast<const void*>(static_cast<std::uintptr_t>(data)),8);return v; // 00411DC1..C3
}
// inline CExoArrayList<4-byte>::Add as in LoadEvent's Float/Object lists: capacity doubles (16 when empty),
// new(cap*4), element copy, free(old); then data[size++] = value
void add4_inline(void* self,unsigned off,unsigned value){
 if(rd32(self,off+4)==rd32(self,off+8)){                                         // 004D7AF0..F6
  const unsigned cap=rd32(self,off+8)!=0?rd32(self,off+8)*2:0x10;               // 004D7AF8..05
  void* const old=at_ptr(self,off);                                              // 004D7B05
  wr32(self,off+8,cap);                                                          // 004D7B08
  void* const data=live_operator_new(cap*4);                                     // 004D7B0B..14
  wr32(self,off,static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(data)));  // 004D7B17
  for(int i=0;i<static_cast<int>(rd32(self,off+4));++i)
   wr32(at_ptr(self,off),static_cast<unsigned>(i)*4,rd32(old,static_cast<unsigned>(i)*4)); // 004D7B1D..33
  if(old!=nullptr)live_free(old);                                                // 004D7B35..3F
 }
 wr32(at_ptr(self,off),rd32(self,off+4)*4,value);                                // 004D7B42..4C
 wr32(self,off+4,rd32(self,off+4)+1);                                            // 004D7B4F..57
}
// 004D79B0 CScriptEvent::LoadEvent(CResGFF*, CResStruct*) RET 8
unsigned load_event_004d79b0(void* event,void* gff,void* st){
 unsigned ok=0;unsigned char list[0x14]={};unsigned char element[16]={};
 *static_cast<unsigned short*>(event)=recovered_gff_word_00411b40(gff,nullptr,st,"EventType",&ok,0);    // 004D79D4..F7
 if(gff_list_004118c0(gff,nullptr,list,st,"IntList")!=0){                        // 004D79EA..01
  for(unsigned i=0;i<gff_list_count_00411940(gff,nullptr,list);++i){             // 004D7A07..8C
   gff_list_element_00411990(gff,nullptr,element,list,i);                        // 004D7A20..2D
   const unsigned v=static_cast<unsigned>(gui_load_gff_int_00411c90(gff,nullptr,element,"Parameter",&ok,0)); // 004D7A32..4D
   void* const ints=static_cast<unsigned char*>(event)+4;                        // ebp = +4 CExoArrayList<int>
   if(rd32(ints,4)==rd32(ints,8))                                                // 004D7A4A..56
    menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(ints),nullptr,
                                        rd32(ints,8)!=0?rd32(ints,8)*2:0x10);    // 004D7A58..68
   wr32(at_ptr(ints,0),rd32(ints,4)*4,v);                                        // 004D7A6D..77
   wr32(ints,4,rd32(ints,4)+1);                                                  // 004D7A7A
  }
 }
 if(gff_list_004118c0(gff,nullptr,list,st,"FloatList")!=0){                      // 004D7A8E..A6
  for(unsigned i=0;i<gff_list_count_00411940(gff,nullptr,list);++i){             // 004D7AAC..65
   gff_list_element_00411990(gff,nullptr,element,list,i);                        // 004D7AC2..CF
   const float f=gui_load_gff_float_00411d00(gff,nullptr,element,"Parameter",&ok,0.0f); // 004D7AD4..EC
   unsigned bits;std::memcpy(&bits,&f,4);
   add4_inline(event,0x10,bits);                                                 // 004D7AF0..57 (+10h float list)
  }
 }
 if(gff_list_004118c0(gff,nullptr,list,st,"StringList")!=0){                     // 004D7B6B..83
  Claude0886ExoString s{0,0};                                                    // 004D7B89..8D [esp+18h]
  for(unsigned i=0;i<gff_list_count_00411940(gff,nullptr,list);++i){             // 004D7B92..49
   gff_list_element_00411990(gff,nullptr,element,list,i);                        // 004D7BB0..BD
   Claude0886ExoString fallback;frame_LogStringCtor_005e5a90(&fallback,nullptr,""); // 004D7BC2..CB ([0073D71C])
   Claude0886ExoString read;
   void* const r=gui_load_gff_string_00411ec0(gff,nullptr,&read,element,"Parameter",&ok,&fallback); // 004D7BD0..F0
   async_assign_string_005e5c50(&s,nullptr,r);                                   // 004D7BF5..FF
   frame_LogStringDtor_005e5c20(&read,nullptr);                                  // 004D7C04..0D
   frame_LogStringDtor_005e5c20(&fallback,nullptr);                              // 004D7C12..1B
   Claude0886ExoString copy;WindowCopy005e5b00(&copy,nullptr,&s);                // 004D7C20..2E by-value copy
   claude_exo_string_list_add_004059a0(static_cast<unsigned char*>(event)+0x1c,nullptr,copy); // 004D7C33..35 (+1Ch)
  }
  frame_LogStringDtor_005e5c20(&s,nullptr);                                      // 004D7C4F..5B
 }
 if(gff_list_004118c0(gff,nullptr,list,st,"ObjectList")!=0){                     // 004D7C60..78
  for(unsigned i=0;i<gff_list_count_00411940(gff,nullptr,list);++i){             // 004D7C7E..3E
   gff_list_element_00411990(gff,nullptr,element,list,i);                        // 004D7C94..A1
   const unsigned v=gui_load_gff_dword_00411c20(gff,nullptr,element,"Parameter",&ok,0); // 004D7CA6..C1
   add4_inline(event,0x28,v);                                                    // 004D7CBE..2C (+28h object list)
  }
 }
 return 1;                                                                       // 004D7D4B
}
// 005043A0 CGameEffect::LoadGameEffect(CResGFF*, CResStruct*) RET 8 (the original's EAX is the last CExoString
// destructor's; no caller reads it)
void load_game_effect_005043a0(void* effect,void* gff,void* st){
 Claude0886ExoString s{0,0};                                                     // 005043BE..C2 [esp+18h]
 unsigned ok=0;unsigned char list[0x14]={};unsigned char element[16]={};
 const std::uint64_t id=read_dword64_00411d70(gff,st,"Id",&ok,0);                // 005043CF..E8
 std::memcpy(effect,&id,8);                                                      // 005043FC..FE
 const unsigned short type=recovered_gff_word_00411b40(gff,nullptr,st,"Type",&ok,0); // 005043ED..01
 *reinterpret_cast<unsigned short*>(static_cast<unsigned char*>(effect)+8)=type; // 00504418
 const unsigned short sub=recovered_gff_word_00411b40(gff,nullptr,st,"SubType",&ok,0); // 00504406..1C
 *reinterpret_cast<unsigned short*>(static_cast<unsigned char*>(effect)+0xa)=sub; // 00504423
 const float duration=gui_load_gff_float_00411d00(gff,nullptr,st,"Duration",&ok,0.0f); // 00504421..34
 std::memcpy(static_cast<unsigned char*>(effect)+0xc,&duration,4);               // 0050443B
 wr32(effect,0x88,gui_load_gff_byte_00411a60(gff,nullptr,st,"SkipOnLoad",&ok,0)); // 00504439..62
 const unsigned day=gui_load_gff_dword_00411c20(gff,nullptr,st,"ExpireDay",&ok,0); // 00504450..6F
 const unsigned time=gui_load_gff_dword_00411c20(gff,nullptr,st,"ExpireTime",&ok,0); // 0050446D..7E
 wr32(effect,0x10,day);                                                          // 00504492
 wr32(effect,0x14,time);                                                         // 00504495
 recovered_effect_creator_00503a00(effect,nullptr,gui_load_gff_dword_00411c20(gff,nullptr,st,"CreatorId",&ok,0)); // 00504483..A0
 wr32(effect,0x1c,gui_load_gff_dword_00411c20(gff,nullptr,st,"SpellId",&ok,0));  // 005044A5..BB
 wr32(effect,0x20,static_cast<unsigned>(gui_load_gff_int_00411c90(gff,nullptr,st,"IsExposed",&ok,0))); // 005044B9..DF
 recovered_effect_resize_00503630(effect,nullptr,static_cast<unsigned>(gui_load_gff_int_00411c90(gff,nullptr,st,"NumIntegers",&ok,0))); // 005044D0..EA
 const unsigned t=*reinterpret_cast<const unsigned short*>(static_cast<const unsigned char*>(effect)+8); // 005044EF..F9
 const int count=static_cast<int>(rd32(effect,0x30));                            // 00504511
 if((t==38u||t==35u)&&count<0x15){                                               // 005044FC..17 [00746714]=26h [00746708]=23h, jge
  const unsigned limit=static_cast<unsigned>(count-6);                           // 00504527..2D -> [esp+60h]
  if(gff_list_004118c0(gff,nullptr,list,st,"IntList")==1){                       // 0050451D..39
   unsigned i=0,j=0;                                                             // 0050453F..45
   while(i<0x15){                                                                // 005045EE..F5 (jb)
    if(i<limit){                                                                 // 00504547..4B (jae)
     if(j>=gff_list_count_00411940(gff,nullptr,list))continue;                   // 0050454D..5B (no progress, as the original)
     gff_list_element_00411990(gff,nullptr,element,list,j);                      // 00504561..6E
     wr32(at_ptr(effect,0x34),j*4,static_cast<unsigned>(gui_load_gff_int_00411c90(gff,nullptr,element,"Value",&ok,0))); // 00504573..8E
     ++j;++i;                                                                    // 005045E4..EA
    }else if(i<0xf){                                                             // 00504593..96
     wr32(at_ptr(effect,0x34),j*4,0);                                            // 00504598..9B
     ++i;                                                                        // 005045E9..EA
    }else{
     if(j>=gff_list_count_00411940(gff,nullptr,list))continue;                   // 005045A4..B2
     gff_list_element_00411990(gff,nullptr,element,list,j);                      // 005045B4..C1
     wr32(at_ptr(effect,0x34),j*4,static_cast<unsigned>(gui_load_gff_int_00411c90(gff,nullptr,element,"Value",&ok,0))); // 005045C6..E1
     ++j;++i;                                                                    // 005045E4..EA
    }
   }
  }
  recovered_effect_resize_00503630(effect,nullptr,0x15);                         // 005045FB..FF
 }else if(count>0){                                                              // 00504606..0B (jle)
  if(gff_list_004118c0(gff,nullptr,list,st,"IntList")==1){                       // 0050460D..22
   for(unsigned j=0;j<gff_list_count_00411940(gff,nullptr,list);++j){            // 00504624..75
    gff_list_element_00411990(gff,nullptr,element,list,j);                       // 00504636..43
    wr32(at_ptr(effect,0x34),j*4,static_cast<unsigned>(gui_load_gff_int_00411c90(gff,nullptr,element,"Value",&ok,0))); // 00504648..63
   }
  }
 }
 if(gff_list_004118c0(gff,nullptr,list,st,"FloatList")==1){                      // 00504677..8C
  for(unsigned j=0;j<4;++j){                                                     // 0050468E..D2
   gff_list_element_00411990(gff,nullptr,element,list,j);                        // 00504697..A4
   const float f=gui_load_gff_float_00411d00(gff,nullptr,element,"Value",&ok,0.0f); // 005046A9..BC
   std::memcpy(static_cast<unsigned char*>(effect)+0x38+j*4,&f,4);               // 005046C1..C5
  }
 }
 if(gff_list_004118c0(gff,nullptr,list,st,"StringList")==1){                     // 005046D4..E9
  for(unsigned j=0;j<6;++j){                                                     // 005046EF..85
   gff_list_element_00411990(gff,nullptr,element,list,j);                        // 005046F8..05
   Claude0886ExoString fallback;frame_LogStringCtor_005e5a90(&fallback,nullptr,""); // 0050470A..13 ([0073D71C])
   Claude0886ExoString read;
   void* const r=gui_load_gff_string_00411ec0(gff,nullptr,&read,element,"Value",&ok,&fallback); // 00504718..38
   async_assign_string_005e5c50(&s,nullptr,r);                                   // 0050473D..47
   frame_LogStringDtor_005e5c20(&read,nullptr);                                  // 0050474C..55
   frame_LogStringDtor_005e5c20(&fallback,nullptr);                              // 0050475A..63
   async_assign_string_005e5c50(static_cast<unsigned char*>(effect)+0x48+j*8,nullptr,&s); // 00504768..71
  }
 }
 if(gff_list_004118c0(gff,nullptr,list,st,"ObjectList")==1){                     // 0050478B..A0
  for(unsigned j=0;j<4;++j){                                                     // 005047A2..DA
   gff_list_element_00411990(gff,nullptr,element,list,j);                        // 005047A7..B4
   wr32(effect,0x78+j*4,gui_load_gff_dword_00411c20(gff,nullptr,element,"Value",&ok,0)); // 005047B9..D1
  }
 }
 frame_LogStringDtor_005e5c20(&s,nullptr);                                       // 005047DC..E8
}
unsigned __fastcall save_structure_0052c2d0(void*,void*,int type,void* value,void* gff,void* parent){
 unsigned char child[16]={};                                                     // [esp+0Ch] (type argument slot)
 recovered_gff_004125b0(gff,nullptr,child,parent,"GameDefinedStrct",static_cast<unsigned>(type)); // 0052C2D0..EC
 if(type==0){recovered_save_game_effect_00503790(value,nullptr,gff,child);return 1;}           // 0052C2F1..0A
 if(type==1)return kotor_fastcall_004d73a0(value,nullptr,gff,child);                          // 0052C30E..24
 if(type==2)return recovered_save_location_004ca8e0(value,nullptr,gff,child);                 // 0052C327..3D
 if(type==3)return save_talent_004caa50(value,gff,child);                                      // 0052C340..56
 return 0;                                                                       // 0052C359..5D
}
unsigned __fastcall load_structure_0052c360(void*,void*,int type,void** out,void* gff,void* parent){
 unsigned char child[16]={};                                                     // [esp+28h] (structure argument slot)
 gui_load_gff_struct_00411a10(gff,nullptr,child,parent,"GameDefinedStrct");      // 0052C37A..8B
 if(type==0){                                                                    // 0052C390..96
  void* const m=live_operator_new(0x8c);                                         // 0052C398..A2
  void* const e=m!=nullptr?recovered_effect_construct_00503e40(m,nullptr,1):nullptr; // 0052C3A9..C0
  *out=e;                                                                        // 0052C3D6
  load_game_effect_005043a0(e,gff,child);                                        // 0052C3C2..D8
  return 1;                                                                      // 0052C3DD
 }
 if(type==1){
  void* const m=live_operator_new(0x34);                                         // 0052C3F9..00
  void* const e=m!=nullptr?recovered_event_construct_004d7540(m,nullptr):nullptr; // 0052C407..1C
  *out=e;                                                                        // 0052C432
  return load_event_004d79b0(e,gff,child);                                       // 0052C41E..34
 }
 if(type==2){
  void* const m=live_operator_new(0x18);                                         // 0052C450..57
  void* const l=m!=nullptr?kotor_fastcall_004ca7a0(m,nullptr):nullptr;          // 0052C45E..73
  *out=l;                                                                        // 0052C489
  kotor_fastcall_004ca800(l,nullptr,gff,child);                                  // 0052C475..8B
  return 1;                                                                      // LoadLocation's EAX (004CA8CD mov eax,1)
 }
 if(type==3){
  void* const m=live_operator_new(0x18);                                         // 0052C4A7..AE
  void* const t=m!=nullptr?recovered_talent_construct_004ca9e0(m,nullptr):nullptr; // 0052C4B5..CA
  *out=t;                                                                        // 0052C4E0
  return load_talent_004caaf0(t,gff,child);                                      // 0052C4CC..E2
 }
 return 0;                                                                       // 0052C4F9..0A
}
struct Vm:recovered_vm_construct::Ports{
 void*allocate(unsigned n)override{return ::operator new(n);}
 const void*base_table()override{return vm_base_table_0074c490;}
 const void*derived_table()override{return vm_derived_table_0074c4fc;}
 void reference_default(void*s)override{c2da_default_ref_00405ed0(s,nullptr);}
 void reference_empty(void*s,void*out)override{unresolved_ref_assign_00406290(static_cast<menu_resources::Ref*>(s),nullptr,static_cast<menu_resources::Ref*>(out),"");}
 void string_default(void*s)override{std::memset(s,0,8);}
 void string_empty(void*s)override{GuiAssign005e5140(s,nullptr,"");}
 void initialize_implementer(void*s)override{auto*t=static_cast<const AiEntry*>(readptr(s,0));t[1](s,nullptr);}
};
struct Stamp:recovered_debug_timestamp::Ports {
 void ftime32(recovered_debug_timestamp::Timeb32*p)override{static_assert(sizeof(*p)==sizeof(__timeb32),"32-bit CRT timeb layout");_ftime32(reinterpret_cast<__timeb32*>(p));}
 const char* ctime32(const std::int32_t*p)override{return _ctime32(reinterpret_cast<const __time32_t*>(p));}
 void render19(char*b,const char*t)override{std::sprintf(b,"%.19s",t);}
 void format(void*s,const char*t)override{kotor_reconstruction::StartupFormat005e5680(static_cast<kotor_reconstruction::StartupString*>(s),t);}
};
struct Policy:recovered_server_policy::Ports {
 void ini_construct(void*s)override{static_cast<base_ini_reconstruction::PublicIni*>(s)->construct_005e6750();}
 void ini_destroy(void*s)override{static_cast<base_ini_reconstruction::PublicIni*>(s)->destroy_005e67e0();}
 void string_default(recovered_server_policy::String*s)override{s->data=0;s->length=0;}
 void string_literal(recovered_server_policy::String*s,const char*t)override{frame_LogStringCtor_005e5a90(s,nullptr,t);}
 void string_destroy(recovered_server_policy::String*s)override{frame_LogStringDtor_005e5c20(s,nullptr);}
 void string_assign(void*s,const void*t)override{static_cast<kotor_reconstruction::StartupString*>(s)->Assign005e5c50(static_cast<kotor_reconstruction::StartupString*>(const_cast<void*>(t)));}
 void string_empty(void*s)override{GuiAssign005e5140(s,nullptr,"");}
};
struct Native final:Ports {
 void* allocate(unsigned n)override{return ::operator new(n);}
 void string_default(String*s)override{s->data=0;s->length=0;}
 void string_literal(String*s,const char*t)override{frame_LogStringCtor_005e5a90(s,nullptr,t);}
 void string_destroy(String*s)override{frame_LogStringDtor_005e5c20(s,nullptr);}
 const char* string_data(String*s)override{return frame_LogCStr_005e5670(s,nullptr);}
 void debug_copy_name(String*s)override{Stamp p;recovered_debug_timestamp::stamp_005e9410(readptr(kotor_menu_10_19::dependencies::base_007a39e0,8),s,p);}
 unsigned char mode()override{return kotor_rules_cell8_007a39dc;}
 void* construct_0052b8e0(void*s)override{Leaf p;return recovered_options_leaf::construct_0052b8e0(s,p);}
 void* construct_004ae4c0(void*s)override{return static_cast<kotor_menu_10_19::WorldTimer*>(s)->construct_004ae4c0();}
 void* construct_004d7d80(void*s,unsigned n)override{return static_cast<kotor_menu_10_19::ObjectArray*>(s)->construct_004d7d80(n);}
 void* construct_00409bf0(void*s)override{return static_cast<menu_resources::Manager*>(s)->construct_00409bf0();}
 void* construct_005d54a0(void*s)override{return static_cast<kotor_menu20::NetLayer*>(s)->construct_normal_005d54a0();}
 void* construct_0054eb10(void*s)override{return static_cast<kotor_menu_10_19::ServerInfo*>(s)->construct_0054eb10();}
 void* construct_00520980(void*s)override{return recovered_options_leaf::construct_00520980(s,message_base,message_vtable_00747e94);}
 void* construct_004b0780(void*s)override{Ai p;return recovered_ai_construct::construct_004b0780(s,p);}
 void set_app_00604790(void*s,void*a)override{std::memcpy(s,&a,4);}
 void* resource_manager()override{return menu_resources::BorrowCanonicalManager007a39e8();}
 void publish_resource_manager(void*s)override{frame_control::resource_manager_007a39e8=static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(s));}   // W6: cell owner 0151 (as 004B653A)
 void directory_00408800(void*m,String*s)override{static_cast<menu_resources::Manager*>(m)->directory_00408800(reinterpret_cast<kotor_reconstruction::StartupString*>(s));}
 void fixed_004087e0(void*m,String*s)override{static_cast<menu_resources::Manager*>(m)->fixed_004087e0(reinterpret_cast<kotor_reconstruction::StartupString*>(s));}
 void remove_file_00406b20(void*m,String*s,unsigned n)override{gui_load_resman_remove_file_00406b20(m,nullptr,s,n);}
 void create_directory_004068c0(void*m,String*s)override{gui_load_resman_create_directory_004068c0(m,nullptr,s);}
 void* talk_table()override{return kotor_cell_tlk_007a3a08;}
 void publish_talk_table(void*s)override{kotor_cell_tlk_007a3a08=s;}
 void* construct_0041d8d0(void*s)override{return static_cast<kotor_menu_10_19::Tlk*>(s)->construct_0041d8d0();}   // W6: 0028 provider; audited vs 0041D8D0: writes +0..+3C, +40..+4C, +54 only (< the original new(58h))
 void open_talk_0041e5a0(void*t,String*s)override{(void)tlk_core::OpenFile0041e5a0(static_cast<kotor_menu_10_19::Tlk*>(t),nullptr,reinterpret_cast<kotor_reconstruction::StartupString*>(s));}   // W6: 0387 CTlkTable::OpenFile
 void* construct_005d0f40(void*s)override{Vm p;return recovered_vm_construct::outer_005d0f40(s,p);}
 void publish_event_manager(void*s)override{frame_control::message_manager_007a3a00=static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(s));}
 void* event_manager()override{return reinterpret_cast<void*>(frame_control::message_manager_007a3a00);}
 const void* event_vtable_007450e4()override{return &command_table_007450e4;}
 void register_event_005d0ff0(void*s,void*i)override{Vm p;recovered_vm_construct::outer_set_005d0ff0(s,i,p);}
 void open_logs_005e93f0(String*s)override{static_cast<kotor_menu20::Debug*>(readptr(kotor_menu_10_19::dependencies::base_007a39e0,8))->open_logs_005e93f0(static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(s)));}
 void* application_server()override{return readptr(kotor_menu_window_cells::manager_007a39fc,8);}
 void initialize_net_005d5550(void*n,void*a)override{static_cast<kotor_menu20::NetLayer*>(n)->initialize_005d5550(a);}
 void initialize_004b1b00(void*s)override{Policy p;recovered_server_policy::initialize_004b1b00(s,p);}
 void* construct_0057a1f0(void* a)override{return recovered_rules_server_construct_0057a1f0(a,nullptr);}   // W1 tier9
 void publish_007a3a28(void*s)override{kotor_cell_rules_007a3a28=s;}
 void initialize_004b1c40(void*)override{String s;string_default(&s);debug_copy_name(&s);string_data(&s);string_destroy(&s);}
 std::uint64_t high_resolution_005e9260()override{return static_cast<kotor_menu_10_19::TimerDependency*>(readptr(kotor_menu_10_19::dependencies::base_007a39e0,4))->high_resolution_005e9260();}
};
}
extern "C" void __fastcall recovered_server_options_004b63e0(void*self,void*,unsigned options){Native p;recovered_server_options::set_options_004b63e0(self,options,p);}
extern "C" void __fastcall recovered_debug_timestamp_005e9410(void* self,void*,void* out){Stamp p;recovered_debug_timestamp::stamp_005e9410(self,out,p);}

// External typed fastcall binding for the existing internal ai_zero_0066b360
// body in this same translation unit.  Strong C linkage resolves the map-pin
// table import; the body is called, never copied, replaced, aliased or patched.
// disable_tail_calls keeps the forwarding call in non-tail position so this
// wrapper's own emitted body ends in RET 12 instead of a tail JMP.
extern "C" __attribute__((disable_tail_calls)) unsigned __fastcall
mp_slot30_ai_zero_0066b360(void* self, void* unused_edx, void* a1, void* a2,
                           unsigned a3) {
  return ai_zero_0066b360(self, unused_edx, a1, a2, a3);
}
// loadgame (Claude): C-linkage export of the existing CScriptEvent::LoadEvent body for CServerAIEventNode::LoadNode
// 004B0290 (claude_loadgame_queue). Original 004D79B0 thiscall(CResGFF*, CResStruct*) RET 8, EAX 1.
extern "C" unsigned __fastcall kotor_fastcall_004d79b0(void* event,void*,void* gff,void* st){return load_event_004d79b0(event,gff,st);}
