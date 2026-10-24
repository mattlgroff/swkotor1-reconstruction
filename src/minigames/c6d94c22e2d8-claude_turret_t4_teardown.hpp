#pragma once
struct KotorT4BankSlots {
 void* (__fastcall* add)(void*,void*,void*,void*);
 void (__fastcall* die)(void*,void*);
 unsigned (__fastcall* query)(void*,void*);
};
extern "C" {
extern const KotorT4BankSlots kotor_t4_bank_table_00752ae4;
void __fastcall kotor_fastcall_00673fd0(void*,void*) noexcept(false);
void* __fastcall kotor_fastcall_00674020(void*,void*,void*) noexcept(false);
int __fastcall kotor_fastcall_006744d0(void*,void*) noexcept(false);
void __fastcall kotor_fastcall_00674800(void*,void*) noexcept(false);
void __cdecl kotor_cdecl_00674930(void*,void*,void*) noexcept(false);
void __fastcall kotor_fastcall_00674990(void*,void*) noexcept(false);
void __fastcall kotor_fastcall_00674a10(void*,void*) noexcept(false);
}

