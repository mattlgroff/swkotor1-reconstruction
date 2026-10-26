// FUNCTION: SWKOTOR 0x0047CE00 SYMBOL: ?what@runtime_error@std@@UBEPBDXZ
// FUNCTION: SWKOTOR 0x0047DA80 SYMBOL: ??0out_of_range@std@@QAE@ABV01@@Z
// FUNCTION: SWKOTOR 0x0047DDC0 SYMBOL: ?_Buy@?$vector@GV?$allocator@G@std@@@std@@IAE_NI@Z
// FUNCTION: SWKOTOR 0x0047E620 SYMBOL: ??0?$vector@PAVNvStripInfo@@V?$allocator@PAVNvStripInfo@@@std@@@std@@QAE@XZ
// the remaining VC7 Dinkumware STL instances emitted next to
// NvTriStrip (0x0047BAB0 .. 0x0047D7B0); worker e's nvstrip_stl.cpp has the
// rest. The bodies come from the stock <vector>, <set>, <xstring> and
// <stdexcept> headers in tools/msvc7/INCLUDE, instantiated explicitly here.
// The linker folds identical instances, so where several element types give
// the same code (pointer vectors, const/non-const iterators) one stands for
// all; the instance picked follows the original's call targets.
//
// vector<NvFaceInfo> (24-byte elements)
// FUNCTION: SWKOTOR 0x0047BAB0 SYMBOL: ?size@?$vector@VNvFaceInfo@@V?$allocator@VNvFaceInfo@@@std@@@std@@QBEIXZ
// FUNCTION: SWKOTOR 0x0047BE00 SYMBOL: ?_Copy_backward_opt@std@@YA?AV?$_Ptrit@VNvFaceInfo@@HPAV1@AAV1@PAV1@AAV1@@1@V21@00U_Nonscalar_ptr_iterator_tag@1@@Z
// FUNCTION: SWKOTOR 0x0047BEA0 SYMBOL: ?_Uninit_copy@std@@YAPAVNvFaceInfo@@V?$_Ptrit@VNvFaceInfo@@HPAV1@AAV1@PAV1@AAV1@@1@0PAV2@AAV?$allocator@VNvFaceInfo@@@1@U_Nonscalar_ptr_iterator_tag@1@@Z
// FUNCTION: SWKOTOR 0x0047C130 SYMBOL: ?fill@std@@YAXV?$_Ptrit@VNvFaceInfo@@HPAV1@AAV1@PAV1@AAV1@@1@0ABVNvFaceInfo@@@Z
// FUNCTION: SWKOTOR 0x0047C160 SYMBOL: ?copy_backward@std@@YA?AV?$_Ptrit@VNvFaceInfo@@HPAV1@AAV1@PAV1@AAV1@@1@V21@00@Z
// FUNCTION: SWKOTOR 0x0047C200 SYMBOL: ?_Uninit_fill_n@std@@YAXPAVNvFaceInfo@@IABV2@AAV?$allocator@VNvFaceInfo@@@1@U_Nonscalar_ptr_iterator_tag@1@@Z
// FUNCTION: SWKOTOR 0x0047CCB0 SYMBOL: ?_Ucopy@?$vector@VNvFaceInfo@@V?$allocator@VNvFaceInfo@@@std@@@std@@IAEPAVNvFaceInfo@@V?$_Ptrit@VNvFaceInfo@@HPAV1@AAV1@PAV1@AAV1@@2@0PAV3@@Z
// FUNCTION: SWKOTOR 0x0047CE70 SYMBOL: ?_Ufill@?$vector@VNvFaceInfo@@V?$allocator@VNvFaceInfo@@@std@@@std@@IAEPAVNvFaceInfo@@PAV3@IABV3@@Z
//
// pointer vectors (NvFaceInfo* / NvEdgeInfo* / NvStripInfo*)
// FUNCTION: SWKOTOR 0x0047BE40 SYMBOL: ?_Uninit_copy@std@@YAPAPAVNvFaceInfo@@V?$_Ptrit@PAVNvFaceInfo@@HPAPAV1@AAPAV1@PAPAV1@AAPAV1@@1@0PAPAV2@AAV?$allocator@PAVNvFaceInfo@@@1@U_Nonscalar_ptr_iterator_tag@1@@Z
// FUNCTION: SWKOTOR 0x0047C0B0 SYMBOL: ?copy_backward@std@@YA?AV?$_Ptrit@PAVNvStripInfo@@HPAPAV1@AAPAV1@PAPAV1@AAPAV1@@1@V21@00@Z
// FUNCTION: SWKOTOR 0x0047C110 SYMBOL: ?fill@std@@YAXV?$_Ptrit@PAVNvStripInfo@@HPAPAV1@AAPAV1@PAPAV1@AAPAV1@@1@0ABQAVNvStripInfo@@@Z
// FUNCTION: SWKOTOR 0x0047C1A0 SYMBOL: ?_Uninit_fill_n@std@@YAXPAPAVNvStripInfo@@IABQAV2@AAV?$allocator@PAVNvStripInfo@@@1@U_Nonscalar_ptr_iterator_tag@1@@Z
// FUNCTION: SWKOTOR 0x0047CC00 SYMBOL: ?_Ucopy@?$vector@PAVNvFaceInfo@@V?$allocator@PAVNvFaceInfo@@@std@@@std@@IAEPAPAVNvFaceInfo@@V?$_Ptrit@PAVNvFaceInfo@@HPAPAV1@AAPAV1@PAPAV1@AAPAV1@@2@0PAPAV3@@Z
// FUNCTION: SWKOTOR 0x0047CE10 SYMBOL: ?_Ufill@?$vector@PAVNvStripInfo@@V?$allocator@PAVNvStripInfo@@@std@@@std@@IAEPAPAVNvStripInfo@@PAPAV3@IABQAV3@@Z
// FUNCTION: SWKOTOR 0x0047D0B0 SYMBOL: ?erase@?$vector@PAVNvStripInfo@@V?$allocator@PAVNvStripInfo@@@std@@@std@@QAE?AV?$_Ptrit@PAVNvStripInfo@@HPAPAV1@AAPAV1@PAPAV1@AAPAV1@@2@V32@0@Z
// FUNCTION: SWKOTOR 0x0047D0F0 SYMBOL: ??1?$vector@PAVNvStripInfo@@V?$allocator@PAVNvStripInfo@@@std@@@std@@QAE@XZ
// FUNCTION: SWKOTOR 0x0047D2B0 SYMBOL: ?_Xlen@?$vector@PAVNvStripInfo@@V?$allocator@PAVNvStripInfo@@@std@@@std@@IBEXXZ
// FUNCTION: SWKOTOR 0x0047EC60 SYMBOL: ?push_back@?$vector@PAVNvStripInfo@@V?$allocator@PAVNvStripInfo@@@std@@@std@@QAEXABQAVNvStripInfo@@@Z
//
// vector<unsigned short>
// FUNCTION: SWKOTOR 0x0047BDE0 SYMBOL: ?fill@std@@YAXV?$_Ptrit@GHPAGAAGPAGAAG@1@0ABG@Z
// FUNCTION: SWKOTOR 0x0047BE70 SYMBOL: ?_Uninit_copy@std@@YAPAGV?$_Ptrit@GHPBGABGPAGAAG@1@0PAGAAV?$allocator@G@1@U_Nonscalar_ptr_iterator_tag@1@@Z
// FUNCTION: SWKOTOR 0x0047C0E0 SYMBOL: ?copy_backward@std@@YA?AV?$_Ptrit@GHPAGAAGPAGAAG@1@V21@00@Z
// FUNCTION: SWKOTOR 0x0047C1D0 SYMBOL: ?_Uninit_fill_n@std@@YAXPAGIABGAAV?$allocator@G@1@U_Nonscalar_ptr_iterator_tag@1@@Z
// FUNCTION: SWKOTOR 0x0047CBC0 SYMBOL: ?erase@?$vector@GV?$allocator@G@std@@@std@@QAE?AV?$_Ptrit@GHPAGAAGPAGAAG@2@V32@0@Z
// FUNCTION: SWKOTOR 0x0047CC30 SYMBOL: ?copy@std@@YAPAGV?$_Ptrit@GHPBGABGPAGAAG@1@0PAG@Z
// FUNCTION: SWKOTOR 0x0047CC60 SYMBOL: ?_Ucopy@?$vector@GV?$allocator@G@std@@@std@@IAEPAGV?$_Ptrit@GHPBGABGPAGAAG@2@0PAG@Z
// FUNCTION: SWKOTOR 0x0047CC80 SYMBOL: ?_Ucopy@?$vector@GV?$allocator@G@std@@@std@@IAEPAGV?$_Ptrit@GHPAGAAGPAGAAG@2@0PAG@Z
// FUNCTION: SWKOTOR 0x0047CE40 SYMBOL: ?_Ufill@?$vector@GV?$allocator@G@std@@@std@@IAEPAGPAGIABG@Z
// FUNCTION: SWKOTOR 0x0047DD40 SYMBOL: ?resize@?$vector@GV?$allocator@G@std@@@std@@QAEXIG@Z
// FUNCTION: SWKOTOR 0x0047E5B0 SYMBOL: ?push_back@?$vector@GV?$allocator@G@std@@@std@@QAEXABG@Z
//
// set<NvFaceInfo*>
// FUNCTION: SWKOTOR 0x0047BAE0 SYMBOL: ?_Max@?$_Tree@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@std@@KAPAU_Node@?$_Tree_nod@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@2@PAU342@@Z
// FUNCTION: SWKOTOR 0x0047BB00 SYMBOL: ?_Min@?$_Tree@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@std@@KAPAU_Node@?$_Tree_nod@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@2@PAU342@@Z
// FUNCTION: SWKOTOR 0x0047BB20 SYMBOL: ?_Inc@const_iterator@?$_Tree@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@std@@QAEXXZ
// FUNCTION: SWKOTOR 0x0047BCC0 SYMBOL: ?_Lrotate@?$_Tree@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@std@@IAEXPAU_Node@?$_Tree_nod@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@2@@Z
// FUNCTION: SWKOTOR 0x0047BD20 SYMBOL: ?_Rrotate@?$_Tree@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@std@@IAEXPAU_Node@?$_Tree_nod@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@2@@Z
// FUNCTION: SWKOTOR 0x0047BD80 SYMBOL: ?_Dec@const_iterator@?$_Tree@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@std@@QAEXXZ
// FUNCTION: SWKOTOR 0x0047BED0 SYMBOL: ?_Buynode@?$_Tree@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@std@@IAEPAU_Node@?$_Tree_nod@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@2@PAU342@00ABQAVNvFaceInfo@@D@Z
// FUNCTION: SWKOTOR 0x0047C030 SYMBOL: ?_Buynode@?$_Tree@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@std@@IAEPAU_Node@?$_Tree_nod@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@2@XZ
// FUNCTION: SWKOTOR 0x0047C070 SYMBOL: ?_Erase@?$_Tree@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@std@@IAEXPAU_Node@?$_Tree_nod@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@2@@Z
// FUNCTION: SWKOTOR 0x0047CA80 SYMBOL: ?find@?$_Tree@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@std@@QBE?AVconst_iterator@12@ABQAVNvFaceInfo@@@Z
// FUNCTION: SWKOTOR 0x0047D7B0 SYMBOL: ?erase@?$_Tree@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@std@@QAE?AViterator@12@V312@@Z
//
// basic_string<char> and the exceptions vector::_Xlen throws
// FUNCTION: SWKOTOR 0x0047CA50 SYMBOL: ??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ
// FUNCTION: SWKOTOR 0x0047CAF0 SYMBOL: ?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAE_NI_N@Z
// FUNCTION: SWKOTOR 0x0047CEA0 SYMBOL: ?erase@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@II@Z
// FUNCTION: SWKOTOR 0x0047CF60 SYMBOL: ?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@ABV12@II@Z
// FUNCTION: SWKOTOR 0x0047D010 SYMBOL: ?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@PBDI@Z
// FUNCTION: SWKOTOR 0x0047D120 SYMBOL: ??0runtime_error@std@@QAE@ABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@1@@Z
// FUNCTION: SWKOTOR 0x0047D180 SYMBOL: ??_Gruntime_error@std@@UAEPAXI@Z
// FUNCTION: SWKOTOR 0x0047D1A0 SYMBOL: ??1runtime_error@std@@UAE@XZ
// FUNCTION: SWKOTOR 0x0047D1E0 SYMBOL: ??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBD@Z
// FUNCTION: SWKOTOR 0x0047D220 SYMBOL: ??0runtime_error@std@@QAE@ABV01@@Z
// FUNCTION: SWKOTOR 0x0047D290 SYMBOL: ??0length_error@std@@QAE@ABV01@@Z

#include "h/nvstrip.h"
#include <string>
#include <stdexcept>

template class std::vector<NvFaceInfo*>;
template class std::vector<NvEdgeInfo*>;
template class std::vector<NvStripInfo*>;
template class std::vector<unsigned short>;
template class std::set<NvFaceInfo*>;
template class std::_Tree<std::_Tset_traits<NvFaceInfo*, std::less<NvFaceInfo*>,
                                            std::allocator<NvFaceInfo*>, false> >;
template class std::basic_string<char>;

// vector<NvFaceInfo> has no default-constructible element, so only the
// members AurStripify / NvStripInfo::Build use get instantiated.
void NvFaceInfoVectorUser(std::vector<NvFaceInfo>& v, const NvFaceInfo& f, int n)
{
    v.push_back(f);
    v.insert(v.begin(), n, f);
    v.erase(v.begin(), v.end());
    std::vector<NvFaceInfo> w(v);
    v = w;
}

void NvStripLengthErrorUser()
{
    throw std::length_error("vector<T> too long");
}

void NvStripRuntimeErrorUser(const std::string& s)
{
    std::runtime_error* e = new std::runtime_error(s);
    std::runtime_error copy(*e);
    delete e;
}
