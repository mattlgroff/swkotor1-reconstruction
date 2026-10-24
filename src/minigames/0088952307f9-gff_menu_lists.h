#pragma once
#include "6b82d9553186-gff_views.h"

namespace task17_gff {
using Word = kotor_q05::U32;
using Gff = kotor_q05::CResGFF;
using Struct = kotor_q05::StructHandle;
// Proposal only: compose the existing four-byte handle as a real subobject.
// Lane CResList is 20 bytes: CResStruct at +0, char[16] label at +4.
struct List { Struct record; char label[16]; };
static_assert(sizeof(List) == 20 && offsetof(List, label) == 4);
}

// ECX receiver, ignored EDX, original stack arguments and callee cleanup.
extern "C" {
__declspec(noinline) task17_gff::Word __fastcall gff_label_00411630(
    task17_gff::Gff*, void*, task17_gff::Struct*, const char*);
__declspec(noinline) task17_gff::Word __fastcall gff_list_004118c0(
    task17_gff::Gff*, void*, task17_gff::List*, task17_gff::Struct*, const char*);
__declspec(noinline) task17_gff::Word __fastcall gff_list_count_00411940(
    task17_gff::Gff*, void*, task17_gff::List*);
__declspec(noinline) task17_gff::Word __fastcall gff_list_element_00411990(
    task17_gff::Gff*, void*, task17_gff::Struct*, task17_gff::List*, task17_gff::Word);
}
