#pragma once
#include <cstdint>
#include <cstddef>

namespace kotor_q05 {
using U32 = std::uint32_t;
// Minimal consumed layout; the untouched prefix/suffix have no recovered meaning.
struct Header {
    U32 file_type, version, struct_offset, struct_count;
    U32 field_offset, field_count, label_offset, label_count;
    U32 data_offset, data_size, indices_offset, indices_size;
    U32 list_offset, list_size;
};
struct StructRecord { U32 type, data_or_offset, field_count; };
struct FieldRecord { U32 type, label, data_or_offset; };
struct StructHandle { U32 index; };
struct CResGFF {
    unsigned char unknown00[0x40];
    Header* header;
    StructRecord* structs;
    U32 unknown48;
    FieldRecord* fields;
    U32 unknown50;
    void* labels;
    U32 unknown58;
    unsigned char* field_data;
    U32 unknown60;
    unsigned char* field_indices;
    U32 unknown68, unknown6c;
    unsigned char* list_indices;
    unsigned char unknown74[0x2c];

    __declspec(noinline) FieldRecord* GetField(StructRecord*, U32);
    __declspec(noinline) FieldRecord* GetField(StructHandle*, U32);
    // Saved int returns are addresses in EAX, preserved here as raw 32-bit bits.
    __declspec(noinline) U32 GetDataField(FieldRecord*, U32*);
    __declspec(noinline) U32 GetDataLayoutList(FieldRecord*, U32*);
    __declspec(noinline) U32 GetElementType(StructHandle*);
    __declspec(noinline) U32 GetFieldCount(StructHandle*);
    __declspec(noinline) U32 GetTopLevelStruct(StructHandle*);
    __declspec(noinline) U32 GetTotalSize();
};
static_assert(sizeof(void*) == 4, "MSVC x86 only");
static_assert(sizeof(Header) == 0x38 && sizeof(StructRecord) == 12 && sizeof(FieldRecord) == 12);
static_assert(sizeof(StructHandle) == 4 && sizeof(CResGFF) == 0xa0);
static_assert(offsetof(CResGFF,header)==0x40 && offsetof(CResGFF,structs)==0x44);
static_assert(offsetof(CResGFF,fields)==0x4c && offsetof(CResGFF,field_data)==0x5c);
static_assert(offsetof(CResGFF,field_indices)==0x64 && offsetof(CResGFF,list_indices)==0x70);
static_assert(offsetof(Header,struct_count)==0xc && offsetof(Header,field_count)==0x14);
static_assert(offsetof(Header,data_size)==0x24 && offsetof(Header,indices_size)==0x2c);
static_assert(offsetof(Header,list_size)==0x34);
}
