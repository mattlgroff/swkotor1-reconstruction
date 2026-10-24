#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaReferencePorts {
 virtual ~AreaReferencePorts()=default;
 virtual Word same_004060b0(Word,Word)=0;
 virtual Word nonempty_00406060(Word,Word)=0;
 virtual void release_00409b00(Word)=0;
 virtual Word remove_00409cf0(Word,Word)=0;
 virtual void destroy_resource(Word,Word)=0;
 virtual Word empty_004060d0(Word,Word)=0;
 virtual void assign_literal_00406290(Word,Word,Word)=0;
 virtual Word lookup_004074d0(Word,Word,Word)=0;
 virtual Word allocate(Word)=0;
 virtual Word construct_00504830(Word)=0;
 virtual void insert_00407680(Word,Word,Word,Word)=0;
 virtual void demand_00408620(Word)=0;
 virtual void assign_00406120(Word,Word,Word)=0;
 virtual void free_006fa390(Word) noexcept=0;
};
void area_reference_00506c30(Memory&,AreaReferencePorts&,Word,Word,Word,Word);
}
