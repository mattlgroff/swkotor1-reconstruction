#include "27a36b75249a4636_trigger_bounds.hpp"
#include <cstring>
namespace{using U=unsigned;void*ptr(U a){return reinterpret_cast<void*>(a);}U word(void*p){return reinterpret_cast<U>(p);}// a2 (L3 t1): the compares are C++ on long double register values (fld dword ; fcomp dword ; fnstsw ; test ah,mask):
// the original's masks reduce to the ordered outcomes (equal: C3 alone; less: C0 alone; greater: neither C0 nor C3;
// every one of them is false when unordered, which sets C0, C2 and C3).
long double ld_word(U w){float f;std::memcpy(&f,&w,4);return f;}   // fld dword
struct Native:recovered_trigger_bounds::Ports{U read(U a)override{U v;std::memcpy(&v,ptr(a),4);return v;}void write(U a,U v)override{std::memcpy(ptr(a),&v,4);}bool equal(U a,U b)override{return ld_word(a)==ld_word(b);}   /* status & 4400h == 4000h (or 0400h) */
bool less(U a,U b)override{return ld_word(a)<ld_word(b);}   /* status & 0500h == 0100h (or 0400h) */
bool greater(U a,U b)override{return ld_word(a)>ld_word(b);}   /* status & 4100h == 0 */};}
extern "C" void __fastcall recovered_trigger_bounds_0058cc60(void*self,void*,void*x0,void*y0,void*x1,void*y1){Native p;recovered_trigger_bounds::bounds(word(self),word(x0),word(y0),word(x1),word(y1),p);}
