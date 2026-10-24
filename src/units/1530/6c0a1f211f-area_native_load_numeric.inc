#include "13ed0a7863-area_cameras_native.inc"
#include "20f9613eaf-area_properties_load_native.inc"
#include "020700425b-area_effect_objects_native.inc"
#include "847bcf99c7-area_stores_native.inc"
#include "e968483f84-door_fields_native_finish.inc"
#include "a77c45c852-door_load_native_overrides.inc"
#include "2fdc993894-area_doors_native_placement.inc"
#include "0729428d7e-area_items_native.inc"
#include "3378a74df0-trigger_fields_native_geometry.inc"
#include "3d06d66206-trigger_placement_native_bounds.inc"
#include "de9075b0dd-area_triggers_native_geometry.inc"
#include "7e32d2ae93-area_encounters_native.inc"
#include "fd38dccee0-area_waypoints_native.inc"
#include "56c448b0d1-area_sounds_native.inc"
#include "941dee9a43-area_placeables_native_placement.inc"
// Draft native area integration. Every unresolved provider terminates at its original address.
void* __fastcall area_as_area(void* p,void*){return static_cast<unsigned char*>(p)-0x11c;}
// 0886-gates (Claude): CSWSArea / CSWArea vtable slots bound to real bodies (exact original ABI, RET checked).
extern "C" {
void __cdecl packed_purecall_006fb8de();                                                              // 006FB8DE (0268)
unsigned __fastcall gui_load_slot_zero8_00409eb0(void*,void*,unsigned,unsigned);                      // 00409EB0 return 0, RET 8 (0296)
unsigned __fastcall kotor_fastcall_00508010(void*,void*,unsigned,void*);                              // 00508010 GetIsIDInExcludeList RET 8 (claude_0886_bodies_area)
unsigned __fastcall recovered_area_set_fog_0058fce0(void*,void*,unsigned,unsigned,unsigned,unsigned,unsigned); // 0058FCE0 SetFog RET 14h (claude_client_area_objects @28)
unsigned __fastcall recovered_area_set_is_night_0058fd10(void*,void*,unsigned);                       // 0058FD10 SetIsNight RET 4 (claude_client_area_objects)
unsigned __fastcall kotor_fastcall_0058fd30(void*,void*,unsigned);                                    // 0058FD30 SetWind RET 4 (claude_0886_bodies_area)
unsigned __fastcall kotor_fastcall_0050c1e0(void*,void*,void*,void*,float,float,float,unsigned,unsigned*,int,void*,int); // 0050C1E0 RET 28h (claude_0886_bodies_area)
}
// CSWSAmbientSound vtable 0074C268 slots, all RET 4 (claude_0886_bodies_ambient)
extern "C" {
void __fastcall kotor_fastcall_005c9900(void*,void*,unsigned);
void __fastcall kotor_fastcall_005c99a0(void*,void*,unsigned);
void __fastcall kotor_fastcall_005c9a40(void*,void*,unsigned);
void __fastcall kotor_fastcall_005c9ae0(void*,void*,unsigned);
void __fastcall kotor_fastcall_005c9b90(void*,void*,unsigned);
void __fastcall kotor_fastcall_005c9c30(void*,void*,unsigned);
void __fastcall kotor_fastcall_005c9cd0(void*,void*,unsigned);
void __fastcall kotor_fastcall_005c9d70(void*,void*,unsigned);
void __fastcall kotor_fastcall_005c9e20(void*,void*,unsigned);
}
// CSWSArea / CSWArea / CResHelper destruction and the object-array insert (claude_0886_bodies_area), RET checked.
extern "C" {
void* __fastcall kotor_fastcall_0050dfb0(void*,void*,unsigned);                                        // 0050DFB0 CSWSArea scalar deleting dtor RET 4
void* __fastcall kotor_fastcall_0050d350(void*,void*,unsigned);                                        // 0050D350 NVThunkAreaResHelper_destructor RET 4
void* __fastcall kotor_fastcall_0050d360(void*,void*,unsigned);                                        // 0050D360 NVThunkAreaGameObject_destructor RET 4
void* __fastcall kotor_fastcall_0058ff60(void*,void*,unsigned);                                        // 0058FF60 CSWArea scalar deleting dtor RET 4
void* __fastcall kotor_fastcall_00506e00(void*,void*,unsigned);                                        // 00506E00 CResHelper<CResARE,2012> scalar deleting dtor RET 4
unsigned char __fastcall kotor_fastcall_004d7e90(void*,void*,unsigned,void*);                          // 004D7E90 CGameObjectArray::AddObjectAtPos RET 8
void __fastcall case40_004c44d0(void*,void*,kotor_reconstruction::StartupString*);                    // 004C44D0 CSWSModule::RemoveModuleResources RET 4 (this unit)
extern const void* kotor_vtable_cgameobject_007457a0[];                                               // [007457A0] CGameObject vtable (0797)
}
#define AREA_GATE(name,address) void* __fastcall name(void*,void*){required(address);}
void __fastcall area_pure_call(void*,void*){packed_purecall_006fb8de();} // 006FB8DE _purecall (0268, never returns)
#undef AREA_GATE
#define A(x) reinterpret_cast<void*>(&x)
const void* const area_gff_vtable[]={A(ifo_destroy_required),A(gff_constant),A(gff_zero),A(gff_resource_freed_00410880),A(gff_resource_serviced_00410740),A(area_pure_call),A(area_pure_call),A(area_pure_call),A(area_pure_call),A(area_pure_call),A(area_pure_call),A(area_pure_call),A(area_pure_call),A(area_pure_call)};
const void* const area_effect_vtable[]={A(kotor_fastcall_005c9900),A(kotor_fastcall_005c99a0),A(kotor_fastcall_005c9a40),A(kotor_fastcall_005c9ae0),A(kotor_fastcall_005c9b90),A(kotor_fastcall_005c9c30),A(kotor_fastcall_005c9cd0),A(kotor_fastcall_005c9d70),A(kotor_fastcall_005c9e20)};
const void* const area_ref_vtable[]={A(kotor_fastcall_00506e00)};
const void* const area_base_vtable[]={A(kotor_fastcall_0058ff60),A(gui_load_slot_zero8_00409eb0),A(recovered_area_set_fog_0058fce0),A(recovered_area_set_is_night_0058fd10),A(kotor_fastcall_0058fd30),nullptr};
const void* const area_primary_vtable[]={A(kotor_fastcall_0050dfb0),A(kotor_fastcall_00508010),A(recovered_area_set_fog_0058fce0),A(recovered_area_set_is_night_0058fd10),A(kotor_fastcall_0058fd30),A(kotor_m15e_slot00c_self_00641db0),A(kotor_fastcall_0050c1e0)};
const void* const area_ref_final_vtable[]={A(kotor_fastcall_0050d350)}; // 00747BF8: 0050D350 thunk (this-100h)
#define Z A(c2da_resource_constant_word_0063e7f0)
const void* const area_object_vtable[]={A(kotor_fastcall_0050d360),A(slot_void_return_4_0060e760_a1),A(slot_void_return_8_005b5e90_a2),Z,Z,Z,Z,Z,Z,Z,A(area_as_area),Z,Z,Z,Z,Z,Z,Z,Z,Z,Z,Z,Z,Z,Z,Z,Z,Z};
#undef Z
#undef A
Word area_vtable(Word a){switch(a){case 0x74c268:return reinterpret_cast<Word>(area_effect_vtable);case 0x749b38:return reinterpret_cast<Word>(area_base_vtable);case 0x74767c:return reinterpret_cast<Word>(area_ref_vtable);case 0x747bfc:return reinterpret_cast<Word>(area_primary_vtable);case 0x747bf8:return reinterpret_cast<Word>(area_ref_final_vtable);case 0x747b88:return reinterpret_cast<Word>(area_object_vtable);case 0x74745c:return reinterpret_cast<Word>(area_gff_vtable);default:required(a);}}
// 0886-gates: original vtable address -> rebuilt table, for the destructor bodies in claude_0886_bodies_area.cpp.
// 00747470 is the all-_purecall CRes base table (= 0074745C+14h, slots 5.. of the CResGFF table).
extern "C" void* __cdecl claude_0886_area_table(unsigned original){if(original==0x747470)return const_cast<void*>(static_cast<const void*>(area_gff_vtable+5));if(original==0x7457a0)return const_cast<void*>(static_cast<const void*>(kotor_vtable_cgameobject_007457a0));return ptr(area_vtable(original));}
struct NativeAreaGff:ModuleGffPorts{NativeModuleGff base;void base_00406460(Word a)override{base.base_00406460(a);}Word vtable(Word a)override{return a==0x74745c?area_vtable(a):base.vtable(a);}void write_byte(Word a,Word b)override{base.write_byte(a,b);}};
struct NativeAreaReference:AreaReferencePorts {NativeMemory&m;NativeModuleReference p;explicit NativeAreaReference(NativeMemory&v):m(v),p(v){}
 Word same_004060b0(Word a,Word b)override{return p.same_004060b0(a,b);}Word nonempty_00406060(Word a,Word b)override{return p.nonempty_00406060(a,b);}void release_00409b00(Word a)override{p.release_00409b00(a);}Word remove_00409cf0(Word a,Word b)override{return p.remove_00409cf0(a,b);}void destroy_resource(Word a,Word b)override{p.destroy_resource(a,b);}Word empty_004060d0(Word a,Word b)override{return p.empty_004060d0(a,b);}void assign_literal_00406290(Word a,Word b,Word c)override{p.assign_literal_00406290(a,b,c);}Word lookup_004074d0(Word a,Word b,Word c)override{return p.lookup_004074d0(a,b,c);}Word allocate(Word a)override{return p.allocate(a);}Word construct_00504830(Word a)override{NativeAreaGff q;return area_gff_00504830(m,q,a);}void insert_00407680(Word a,Word b,Word c,Word d)override{p.insert_00407680(a,b,c,d);}void demand_00408620(Word a)override{p.demand_00408620(a);}void assign_00406120(Word a,Word b,Word c)override{p.assign_00406120(a,b,c);}
 void free_006fa390(Word a) noexcept override{live_free(ptr(a));}
};
extern "C" void __fastcall minigame_set_resref_00506c30(void* self,void*,const menu_resources::Ref* ref,unsigned request) noexcept(false){
 NativeMemory m; NativeAreaReference p(m); alignas(8) unsigned char scratch[0x50];
 area_reference_00506c30(m,p,word(self),reinterpret_cast<Word>(ref),request,word(scratch));
}
struct NativeAreaPrimitive:AreaPrimitivePorts {NativeMemory&m;explicit NativeAreaPrimitive(NativeMemory&v):m(v){}Word vtable(Word a)override{return area_vtable(a);}void byte(Word a,Word b)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(b);}void ref_00405ed0(Word a)override{(void)c2da_default_ref_00405ed0(ptr(a),nullptr);}Word allocate(Word a)override{return word(::operator new(a));}Word list_005e9aa0(Word a)override{m.write(a+4,0);m.write(a,0);m.write(a+8,0);return a;}};
struct NativeAreaAllocation:AreaAllocationPorts{void free(Word a)override{::operator delete(ptr(a));}void byte(Word a,Word b)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(b);}Word vtable(Word a)override{return area_vtable(a);}};
struct NativeAreaConstructor:AreaConstructorPorts{NativeMemory&m;NativeModuleConstructor p;NativeAreaPrimitive primitive;explicit NativeAreaConstructor(NativeMemory&v):m(v),p(v),primitive(v){}
 Word vtable(Word a)override{return area_vtable(a);}Word literal(Word a)override{return reinterpret_cast<Word>(module_literal(a));}void byte(Word a,Word b)override{primitive.byte(a,b);}void base_0058f6b0(Word a)override{(void)area_base_0058f6b0(m,primitive,a);}void ref_00405ed0(Word a)override{p.list_00405ed0(a);}void assign_00406290(Word a,Word b,Word c)override{(void)unresolved_ref_assign_00406290(ptr(a),nullptr,ptr(b),static_cast<const char*>(ptr(c)));}void object_004d7d60(Word a,Word b,Word c)override{p.object_004d7d60(a,b,c);}void small_00506d90(Word a)override{(void)area_small_00506d90(m,primitive,a);}void empty_005b3190(Word a)override{p.empty_005b3190(a);}void localized_005e9e10(Word a)override{p.linked_005e9e10(a);}void strings_006fb55f(Word a,Word b,Word c,Word d,Word e)override{p.strings_006fb55f(a,b,c,d,e);}void location_0059afe0(Word a)override{p.location_0059afe0(a);}Word allocate(Word a)override{return word(::operator new(a));}
 void resource_00506c30(Word a,Word b,Word c)override{NativeAreaReference q(m);alignas(8) unsigned char scratch[0x50];area_reference_00506c30(m,q,a,b,c,word(scratch));}
 Word collision_005d0ce0(Word a)override{NativeAreaAllocation q;return collision_construct_005d0ce0(m,q,a);}void initialize_0058f800(Word a)override{area_initialize_0058f800(m,a);}Word objects_004aed70(Word a)override{return p.array_004aed70(a);}void register_004d80e0(Word a,Word b,Word c,Word d)override{p.register_004d80e0(a,b,c,d);}void insert_004d7e90(Word a,Word b,Word c)override{(void)kotor_fastcall_004d7e90(ptr(a),nullptr,b,ptr(c));}Word effects_005c95a0(Word a,Word b)override{NativeAreaAllocation q;return area_effects_005c95a0(m,q,a,b);}Word timer_004aede0(Word a)override{return p.timer_004aede0(a);}void time_004ade40(Word a,Word b,Word c)override{p.time_004ade40(a,b,c);}
};
#include "b7747ed103-area_creatures_native_safe.inc"
struct NativeAreaContent:AreaContentPorts{NativeMemory&m;explicit NativeAreaContent(NativeMemory&v):m(v){}
 Word allocate(Word a)override{return word(::operator new(a));}Word layout_construct_005de770(Word a)override{NativeLayoutLifecycle p;alignas(8) unsigned char scratch[0x30];return kotor::server_mainloop::layout_construct_005de770(m,p,a,word(scratch));}Word layout_load_005df140(Word a,Word b)override{NativeLayoutLifecycle p;alignas(8) unsigned char scratch[0x10];return kotor::server_mainloop::layout_load_005df140(p,a,b,word(scratch));}Word layout_count_005a4c50(Word a)override{return kotor::server_mainloop::layout_count_005a4c50(m,a);}Word layout_extract_00504870(Word a)override{NativeLayoutExtract p(m);alignas(8) unsigned char scratch[0x40];return kotor::server_mainloop::layout_extract_00504870(m,p,a,word(scratch));}void layout_release_005de450(Word a)override{NativeLayoutLifecycle p;kotor::server_mainloop::layout_release_005de450(m,p,a);}
 void destroy(Word a,Word b)override{using F=void*(__thiscall*)(void*,Word);(void)reinterpret_cast<F>(m.read(m.read(a)))(ptr(a),b);}
 Word exists_00408bc0(Word a,Word b,Word c,Word d)override{return static_cast<menu_resources::Manager*>(ptr(a))->exists_00408bc0(static_cast<menu_resources::Ref*>(ptr(b)),c,static_cast<unsigned*>(ptr(d)));}
 Word gff_construct_00410630(Word a,Word b,Word c,Word d)override{return word(gui_load_gff_construct_00410630(static_cast<kotor_q05::CResGFF*>(ptr(a)),nullptr,b,load_literal(c),static_cast<const menu_resources::Ref*>(ptr(d))));}
 void release_00409b80(Word a)override{c2da_release_demand_00409b80(ptr(a),nullptr);}void root_00411240(Word a,Word b)override{(void)static_cast<kotor_q05::CResGFF*>(ptr(a))->GetTopLevelStruct(static_cast<kotor_q05::StructHandle*>(ptr(b)));}
 void variables_0059aa80(Word a,Word b,Word c)override{native_local_variables(a,b,c);}void effects_0059b0f0(Word a,Word b,Word c)override{native_builtin_variables(a,b,c);}
 Word gff_byte_00411a60(Word a,Word b,Word c,Word d,Word e)override{return gui_load_gff_byte_00411a60(ptr(a),nullptr,ptr(b),load_literal(c),static_cast<unsigned*>(ptr(d)),static_cast<unsigned char>(e));}
 void byte(Word a,Word b)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(b);}void group(Word a,Word b,Word c,Word d,Word e,Word f)override{if(a==0x505af0){/* closeB: full LoadAreaEffects body (aoe lane) */(void)kotor_fastcall_00505af0(ptr(b),nullptr,ptr(c),ptr(d),e,f);return;}if(a==0x5057a0){/* closeB: full LoadStores body (aoe lane) */(void)kotor_fastcall_005057a0(ptr(b),nullptr,ptr(c),ptr(d),e,f);return;}if(a==0x50a7b0){NativeAreaPlaceables p(m);alignas(8)unsigned char scratch[0x90];(void)area_placeables_0050a7b0(m,p,b,c,d,e,f,word(scratch));return;}if(a==0x505560){NativeAreaSounds p(m);alignas(8)unsigned char scratch[0x78];(void)area_sounds_00505560(m,p,b,c,d,e,f,word(scratch));return;}if(a==0x505360){NativeAreaWaypoints p(m);alignas(8)unsigned char scratch[0x50];(void)area_waypoints_00505360(m,p,b,c,d,e,f,word(scratch));return;}if(a==0x505060){/* encounter lane: full LoadEncounters body (claude_encounter_load) */(void)kotor_fastcall_00505060(ptr(b),nullptr,ptr(c),ptr(d),e,f);return;}if(a==0x50a350){NativeAreaTriggers p(m);alignas(8)unsigned char scratch[recovered_gff_scratch::triggers];(void)area_triggers_0050a350(m,p,b,c,d,e,f,word(scratch));return;}if(a==0x50a0e0){NativeAreaDoors p;unsigned char scratch[0x60];(void)area_doors_0050a0e0(m,p,b,c,d,e,f,word(scratch));return;}if(a==0x504de0){NativeAreaItems p;unsigned char scratch[0x74];(void)area_items_00504de0(m,p,b,c,d,e,f,word(scratch));return;}if(a==0x00504a70){NativeAreaCreatures p(m);alignas(8) unsigned char scratch[0x90];(void)area_creatures_00504a70(m,p,b,c,d,e,f,word(scratch));return;}required(a);}void doors_extra_00507490(Word a,Word b,Word c)override{(void)native_area_properties_load(a,b,c);}void extra_00505da0(Word a,Word b,Word c)override{NativeAreaMapLoad p;alignas(4)unsigned char scratch[0x1c];(void)area_map_load_00505da0(m,p,a,b,c,word(scratch));}void extra_00505eb0(Word a,Word b,Word c)override{NativeAreaCameras p(m);alignas(4)unsigned char scratch[recovered_gff_scratch::cameras];(void)area_cameras_00505eb0(m,p,a,b,c,word(scratch));}
};
struct NativeAreaPath:AreaPathPorts{NativeMemory&m;NativeAreaContent base;explicit NativeAreaPath(NativeMemory&v):m(v),base(v){}
 Word exists_00408bc0(Word a,Word b,Word c,Word d)override{return base.exists_00408bc0(a,b,c,d);}Word allocate(Word a)override{return base.allocate(a);}Word construct_00410630(Word a,Word b,Word c,Word d)override{return base.gff_construct_00410630(a,b,c,d);}void destroy(Word a,Word b)override{base.destroy(a,b);}void root_00411240(Word a,Word b)override{base.root_00411240(a,b);}
 Word list_004118c0(Word a,Word b,Word c,Word d)override{return gff_list_004118c0(ptr(a),nullptr,ptr(b),ptr(c),load_literal(d));}Word count_00411940(Word a,Word b)override{return gff_list_count_00411940(ptr(a),nullptr,ptr(b));}Word element_00411990(Word a,Word b,Word c,Word d)override{return gff_list_element_00411990(ptr(a),nullptr,ptr(b),ptr(c),d);}Word type_004111c0(Word a,Word b)override{return static_cast<kotor_q05::CResGFF*>(ptr(a))->GetElementType(static_cast<kotor_q05::StructHandle*>(ptr(b)));}
 Word float_bits_00411d00(Word a,Word b,Word c,Word d,Word e)override{float fallback;std::memcpy(&fallback,&e,4);float value=gui_load_gff_float_00411d00(ptr(a),nullptr,ptr(b),load_literal(c),static_cast<unsigned*>(ptr(d)),fallback);Word bits;std::memcpy(&bits,&value,4);return bits;}Word dword_00411c20(Word a,Word b,Word c,Word d,Word e)override{return gui_load_gff_dword_00411c20(ptr(a),nullptr,ptr(b),load_literal(c),static_cast<unsigned*>(ptr(d)),e);}
};
struct NativeAreaLoad:AreaLoadPorts{NativeMemory&m;explicit NativeAreaLoad(NativeMemory&v):m(v){}Word demand_00409b10(Word a)override{return c2da_current_resource_demand_00409b10(ptr(a),nullptr);}void root_00411240(Word a,Word b)override{(void)static_cast<kotor_q05::CResGFF*>(ptr(a))->GetTopLevelStruct(static_cast<kotor_q05::StructHandle*>(ptr(b)));}void properties_00508c50(Word a,Word b)override{NativeAreaProperties q(m);alignas(8) unsigned char scratch[0x110];(void)area_properties_00508c50(m,q,a,b,word(scratch));}void tiles_005073d0(Word a,Word b)override{NativeAreaContent q(m);(void)area_layout_005073d0(m,q,a,b);}void objects_0050dd80(Word a,Word b)override{NativeAreaContent q(m);alignas(8) unsigned char scratch[0x40];(void)area_objects_0050dd80(m,q,a,b,word(scratch));}void path_00508400(Word a,Word b)override{NativeAreaPath q(m);alignas(8) unsigned char scratch[0x50];(void)area_path_00508400(m,q,a,b,word(scratch));}void release_00409b80(Word a)override{c2da_release_demand_00409b80(ptr(a),nullptr);}Word module_004ae6b0(Word a)override{return word(server_internal_get_module_004b14f0(ptr(m.read(a+4)),nullptr));}void register_tag_004c7de0(Word a,Word b,Word c)override{(void)recovered_module_register_tag_004c7de0(ptr(a),nullptr,ptr(b),c);}Word numeric_tail(Word a,Word b)override{return recovered_area_load_numeric_0050e216(a,b);}void byte(Word a,Word b)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(b);}};
struct NativeAreaStep:AreaStepPorts{NativeMemory&m;explicit NativeAreaStep(NativeMemory&v):m(v){}Word allocate(Word a)override{return word(::operator new(a));}Word construct_0050cf80(Word a,Word b,Word c,Word d)override{NativeAreaConstructor p(m);alignas(8) unsigned char scratch[0x50];return area_constructor_0050cf80(m,p,a,b,c,d,word(scratch));}Word load_0050e190(Word a,Word b)override{NativeAreaLoad p(m);alignas(8) unsigned char scratch[8];return area_load_0050e190(m,p,a,b,word(scratch));}void release_00409b80(Word a)override{c2da_release_demand_00409b80(ptr(a),nullptr);}void release_path_004c44d0(Word a,Word b)override{case40_004c44d0(ptr(a),nullptr,static_cast<kotor_reconstruction::StartupString*>(ptr(b)));}void destroy(Word a,Word b)override{(void)kotor_fastcall_0050dfb0(ptr(a),nullptr,b);}Word message_getter_004aedb0(Word a)override{return m.read(m.read(a+4)+0x10010);}void message_date_0056c910(Word a,Word b,Word c)override{NativeModuleMessage p(m);unsigned scratch[2];kotor::server_mainloop::message_date_0056c910(m,p,a,b,c,word(scratch));}};
