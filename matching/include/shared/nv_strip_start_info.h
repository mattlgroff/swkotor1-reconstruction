// nv_strip_start_info.h -- NvStripStartInfo
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/e/nvstrip.h); the other copies' members fall on its members.
#ifndef SHARED_NV_STRIP_START_INFO_H
#define SHARED_NV_STRIP_START_INFO_H


class NvEdgeInfo;
class NvFaceInfo;

class NvStripStartInfo {
public:
    NvStripStartInfo(NvFaceInfo* startFace, NvEdgeInfo* startEdge, bool toV1)
    {
        m_startFace = startFace;
        m_startEdge = startEdge;
        m_toV1 = toV1;
    }
    NvFaceInfo* m_startFace;
    NvEdgeInfo* m_startEdge;
    bool m_toV1;
};

#endif
