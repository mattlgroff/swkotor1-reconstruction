#include "61e916c4d432-claude_minigame_vm_f.hpp"
#include "617feb9cb553-claude_vm_actions_common.hpp"
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void*,void*); }
extern "C" {
void* __fastcall kotor_fastcall_005edaf0(void*,void*);
void* __fastcall kotor_fastcall_0066bf30(void*,void*,vmx::U,vmx::U);
void* __fastcall frame_LogStringCtor_005e5a90(void*,void*,const char*);
}
namespace {
struct String {
 vmx::ExoString value;
 String(){kotor_movie::string_element_construct_005b3190(&value,nullptr);}
 explicit String(const char* text){frame_LogStringCtor_005e5a90(&value,nullptr,text);}
 ~String() noexcept(false){frame_LogStringDtor_005e5c20(&value,nullptr);}
 String(const String&)=delete;
 String& operator=(const String&)=delete;
};
vmx::U read(const void* p,unsigned offset){vmx::U v;std::memcpy(&v,static_cast<const unsigned char*>(p)+offset,4);return v;}
void* lookup(void* commands){
 void* manager=kotor_menu_window_cells::manager_007a39fc;
 vmx::U id=read(commands,8);
 void* client=vmx::ptr(read(manager,4));
 return kotor_fastcall_0066bf30(kotor_fastcall_005edaf0(client,nullptr),nullptr,id,0);
}
}
extern "C" int __fastcall kotor_fastcall_005cb480(void* commands,void*,int,int){
 void* object=lookup(commands);
 String first;
 if(object)async_assign_string_005e5c50(&first.value,nullptr,static_cast<unsigned char*>(object)+0x10);
 String second(WindowCStr005e5670(&first.value,nullptr));
 return recovered_vm_outer_push_string_005d1090(vmx::vm(),nullptr,&second.value)?0:vmx::k_push_failed;
}
extern "C" int __fastcall kotor_fastcall_005ccbb0(void* commands,void*,int,int){
 String first;
 void* object=lookup(commands);
 if(object)async_assign_string_005e5c50(&first.value,nullptr,static_cast<unsigned char*>(object)+0x44);
 return recovered_vm_outer_push_string_005d1090(vmx::vm(),nullptr,&first.value)?0:vmx::k_push_failed;
}
