// VC7 Dinkumware STL instances emitted next to NvTriStrip
// (0x0047BF10 .. 0x0047FD40). The bodies come from the stock <vector>,
// <set> and <xstring> headers in tools/msvc7/INCLUDE; the small users below
// only exist to instantiate them the way NvTriStripObjects.cpp and
// AurStripify (0x00481140) do. Identical pointer-vector instances are folded
// by the linker, so one element type stands for all of them.
//
// FUNCTION: SWKOTOR 0x0047BF10 SYMBOL: ?_Copy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXII@Z
// FUNCTION: SWKOTOR 0x0047D330 SYMBOL: ?_Insert@?$_Tree@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@std@@IAE?AViterator@12@_NPAU_Node@?$_Tree_nod@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@2@ABQAVNvFaceInfo@@@Z
// FUNCTION: SWKOTOR 0x0047D510 SYMBOL: ?_Insert_n@?$vector@GV?$allocator@G@std@@@std@@IAEXV?$_Ptrit@GHPAGAAGPAGAAG@2@IABG@Z
// FUNCTION: SWKOTOR 0x0047DAA0 SYMBOL: ?reserve@?$vector@PAVNvFaceInfo@@V?$allocator@PAVNvFaceInfo@@@std@@@std@@QAEXI@Z
// FUNCTION: SWKOTOR 0x0047DB90 SYMBOL: ?reserve@?$vector@GV?$allocator@G@std@@@std@@QAEXI@Z
// FUNCTION: SWKOTOR 0x0047DC80 SYMBOL: ?insert@?$_Tree@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@std@@QAE?AU?$pair@Viterator@?$_Tree@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@std@@_N@2@ABQAVNvFaceInfo@@@Z
// FUNCTION: SWKOTOR 0x0047DE10 SYMBOL: ?_Insert_n@?$vector@PAVNvFaceInfo@@V?$allocator@PAVNvFaceInfo@@@std@@@std@@IAEXV?$_Ptrit@PAVNvFaceInfo@@HPAPAV1@AAPAV1@PAPAV1@AAPAV1@@2@IABQAVNvFaceInfo@@@Z
// FUNCTION: SWKOTOR 0x0047E0B0 SYMBOL: ?erase@?$_Tree@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@std@@QAE?AViterator@12@V312@0@Z
// FUNCTION: SWKOTOR 0x0047E170 SYMBOL: ?_Insert_n@?$vector@VNvFaceInfo@@V?$allocator@VNvFaceInfo@@@std@@@std@@IAEXV?$_Ptrit@VNvFaceInfo@@HPAV1@AAV1@PAV1@AAV1@@2@IABVNvFaceInfo@@@Z
// FUNCTION: SWKOTOR 0x0047E470 SYMBOL: ??4?$vector@GV?$allocator@G@std@@@std@@QAEAAV01@ABV01@@Z
// FUNCTION: SWKOTOR 0x0047E630 SYMBOL: ?resize@?$vector@PAVNvEdgeInfo@@V?$allocator@PAVNvEdgeInfo@@@std@@@std@@QAEXIPAVNvEdgeInfo@@@Z
// FUNCTION: SWKOTOR 0x0047E6B0 SYMBOL: ?insert@?$vector@VNvFaceInfo@@V?$allocator@VNvFaceInfo@@@std@@@std@@QAE?AV?$_Ptrit@VNvFaceInfo@@HPAV1@AAV1@PAV1@AAV1@@2@V32@ABVNvFaceInfo@@@Z
// FUNCTION: SWKOTOR 0x0047ECD0 SYMBOL: ?push_back@?$vector@VNvFaceInfo@@V?$allocator@VNvFaceInfo@@@std@@@std@@QAEXABVNvFaceInfo@@@Z
// FUNCTION: SWKOTOR 0x0047FD40 SYMBOL: ?_Tidy@?$_Tree@V?$_Tset_traits@PAVNvFaceInfo@@U?$less@PAVNvFaceInfo@@@std@@V?$allocator@PAVNvFaceInfo@@@3@$0A@@std@@@std@@IAEXXZ

#include "e/nvstrip.h"

// NvStripifier::FindAllStrips (0x00480490) keeps a set of the faces it has
// already tried as strip starts.
void NvStripSetUser(std::set<NvFaceInfo*>& tried, NvFaceInfo* face)
{
    std::set<NvFaceInfo*> local;
    local.insert(face);
    tried.insert(face);
}

// NvStripifier::BuildStripifyInfo / Stripify use these on the edge and index
// vectors.
void NvStripVectorUser(NvFaceInfoVec& faces, NvEdgeInfoVec& edges, WordVec& dst, const WordVec& src)
{
    faces.reserve(src.size());
    edges.resize(src.size());
    dst = src;
    faces.push_back(0);
    dst.push_back(0);
}

// AurStripify (0x00481140) collects whole faces and reserves its index list.
void AurStripifyVectorUser(std::vector<NvFaceInfo>& faces, const NvFaceInfo& face, WordVec& indices, int n)
{
    indices.reserve(n);
    faces.push_back(face);
}
