#include "../../units/1530/ae2b8bbe38-area_reference.hpp"
namespace kotor::server_mainloop {
void area_reference_00506c30(Memory& m,AreaReferencePorts& p,Word self,Word ref,Word demand,Word scratch){
 Word own=self+0xc;
 if(p.same_004060b0(ref,own))return;
 if(m.read(self+8)&&p.nonempty_00406060(own,0x73d71c)){
  if(m.read(self+4))p.release_00409b00(m.read(self+8));
  if(!p.remove_00409cf0(m.read(0x7a39e8),m.read(self+8))){
   Word resource=m.read(self+8);if(resource)p.destroy_resource(resource,1);
   m.write(self+8,0);
  }
 }
 if(p.empty_004060d0(ref,0x73d71c)){
  m.write(self+8,0);p.assign_literal_00406290(own,scratch+0x10,0x73d71c);return;
 }
 Word resource=p.lookup_004074d0(m.read(0x7a39e8),ref,0x7dc);
 if(!resource){
  Word allocated=p.allocate(0xa0);
  if(allocated){
   try{resource=p.construct_00504830(allocated);}
   catch(...){p.free_006fa390(allocated);throw;}
  }else resource=0;
  p.insert_00407680(m.read(0x7a39e8),ref,0x7dc,resource);
  if(!resource)return;
 }
 m.write(self+8,resource);
 if(demand){m.write(self+4,1);p.demand_00408620(resource);}
 p.assign_00406120(own,scratch+0x20,ref);
}
}
