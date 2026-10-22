// NvTriStrip classes (copied from include/e/nvstrip.h), used by
// matching/src/h/nvstrip_stl.cpp for the STL helper instances.
#ifndef MATCHING_H_NVSTRIP_H
#define MATCHING_H_NVSTRIP_H

#pragma warning(disable: 4786)
#include <vector>
#include <set>
#include <stdio.h>
#include <string.h>

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    void Allocate(int n)
    {
        T* old = element;
        array_size = n;
        element = new T[n];
        for (int i = 0; i < num; i++)
            element[i] = old[i];
        if (old)
            delete[] old;
    }
    void Add(T value)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num++] = value;
    }
};

class NvFaceInfo {
public:
    NvFaceInfo(int v0, int v1, int v2)
    {
        m_v0 = v0; m_v1 = v1; m_v2 = v2;
        m_stripId = -1;
        m_testStripId = -1;
        m_experimentId = -1;
    }

    int m_v0, m_v1, m_v2;
    int m_stripId;
    int m_testStripId;
    int m_experimentId;
};

class NvEdgeInfo {
public:
    NvEdgeInfo(int v0, int v1)
    {
        m_v0 = v0; m_v1 = v1;
        m_face0 = NULL; m_face1 = NULL;
        m_nextV0 = NULL; m_nextV1 = NULL;
        m_refCount = 2;
    }

    void Unref() { if (--m_refCount == 0) delete this; }

    unsigned int m_refCount;
    NvFaceInfo* m_face0;
    NvFaceInfo* m_face1;
    int m_v0, m_v1;
    NvEdgeInfo* m_nextV0;
    NvEdgeInfo* m_nextV1;
};

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

typedef std::vector<NvFaceInfo*> NvFaceInfoVec;
typedef std::vector<NvEdgeInfo*> NvEdgeInfoVec;
typedef std::vector<unsigned short> WordVec;

class NvStripInfo {
public:
    NvStripInfo(const NvStripStartInfo& startInfo, int stripId, int experimentId = -1)
        : m_startInfo(startInfo)
    {
        m_stripId = stripId;
        m_experimentId = experimentId;
        visited = false;
    }

    bool IsExperiment() const { return m_experimentId >= 0; }

    bool IsInStrip(const NvFaceInfo* faceInfo) const;  // 0x0047BA70 (out of line)

    bool SharesEdge(const NvFaceInfo* faceInfo, NvEdgeInfoVec& edgeInfos);
    void Combine(const NvFaceInfoVec& forward, const NvFaceInfoVec& backward);
    bool Unique(NvFaceInfoVec& faceVec, NvFaceInfo* face);

    bool IsMarked(NvFaceInfo* faceInfo)
    {
        return (faceInfo->m_stripId >= 0) ||
               (IsExperiment() && faceInfo->m_experimentId == m_experimentId);
    }
    void MarkTriangle(NvFaceInfo* faceInfo)
    {
        if (IsExperiment()) {
            faceInfo->m_experimentId = m_experimentId;
            faceInfo->m_testStripId = m_stripId;
        } else {
            faceInfo->m_experimentId = -1;
            faceInfo->m_stripId = m_stripId;
        }
    }
    void Build(NvEdgeInfoVec& edgeInfos, NvFaceInfoVec& faceInfos);

    NvStripStartInfo m_startInfo;   // +0x00
    NvFaceInfoVec m_faces;          // +0x0C
    int m_stripId;                  // +0x1C
    int m_experimentId;             // +0x20
    bool visited;                   // +0x24
};

typedef std::vector<NvStripInfo*> NvStripInfoVec;

class VertexCache {
public:
    VertexCache(int size);  // 0x004A9680
    ~VertexCache();         // 0x004A96C0

    bool InCache(int entry)
    {
        bool returnVal = false;
        for (int i = 0; i < numEntries; i++) {
            if (entries[i] == entry) {
                returnVal = true;
                break;
            }
        }
        return returnVal;
    }

    int AddEntry(int entry)
    {
        int removed;
        removed = entries[numEntries - 1];
        for (int i = numEntries - 2; i >= 0; i--)
            entries[i + 1] = entries[i];
        entries[0] = entry;
        return removed;
    }

    int* entries;
    int numEntries;
};

class NvStripifier {
public:
    static NvEdgeInfo* FindEdgeInfo(NvEdgeInfoVec& edgeInfos, int v0, int v1);
    static NvFaceInfo* FindOtherFace(NvEdgeInfoVec& edgeInfos, int v0, int v1, NvFaceInfo* faceInfo)
    {
        NvEdgeInfo* edgeInfo = FindEdgeInfo(edgeInfos, v0, v1);
        return (edgeInfo->m_face0 == faceInfo ? edgeInfo->m_face1 : edgeInfo->m_face0);
    }
    static int GetNextIndex(const WordVec& indices, NvFaceInfo* face);

    int FindStartPoint(NvFaceInfoVec& faceInfos, NvEdgeInfoVec& edgeInfos);
    NvFaceInfo* FindGoodResetPoint(NvFaceInfoVec& faceInfos, NvEdgeInfoVec& edgeInfos);
    bool FindTraversal(NvFaceInfoVec& faceInfos, NvEdgeInfoVec& edgeInfos,
                       NvStripInfo* strip, NvStripStartInfo& startInfo);
    void UpdateCacheStrip(VertexCache* vcache, NvStripInfo* strip);
    void UpdateCacheFace(VertexCache* vcache, NvFaceInfo* face);
    float CalcNumHitsStrip(VertexCache* vcache, NvStripInfo* strip);
    int NumNeighbors(NvFaceInfo* face, NvEdgeInfoVec& edgeInfoVec);
    int CalcNumHitsFace(VertexCache* vcache, NvFaceInfo* face);  // 0x0047BC40

    static int GetUniqueVertexInB(NvFaceInfo* faceA, NvFaceInfo* faceB)
    {
        int facev0 = faceB->m_v0;
        if (facev0 != faceA->m_v0 && facev0 != faceA->m_v1 && facev0 != faceA->m_v2)
            return facev0;
        int facev1 = faceB->m_v1;
        if (facev1 != faceA->m_v0 && facev1 != faceA->m_v1 && facev1 != faceA->m_v2)
            return facev1;
        int facev2 = faceB->m_v2;
        if (facev2 != faceA->m_v0 && facev2 != faceA->m_v1 && facev2 != faceA->m_v2)
            return facev2;
        return -1;
    }
    static int GetSharedVertex(NvFaceInfo* faceA, NvFaceInfo* faceB)
    {
        int facev0 = faceB->m_v0;
        if (facev0 == faceA->m_v0 || facev0 == faceA->m_v1 || facev0 == faceA->m_v2)
            return facev0;
        int facev1 = faceB->m_v1;
        if (facev1 == faceA->m_v0 || facev1 == faceA->m_v1 || facev1 == faceA->m_v2)
            return facev1;
        int facev2 = faceB->m_v2;
        if (facev2 == faceA->m_v0 || facev2 == faceA->m_v1 || facev2 == faceA->m_v2)
            return facev2;
        return -1;
    }
    static bool IsCW(NvFaceInfo* faceInfo, int v0, int v1)
    {
        if (faceInfo->m_v0 == v0)
            return (faceInfo->m_v1 == v1);
        else if (faceInfo->m_v1 == v0)
            return (faceInfo->m_v2 == v1);
        else
            return (faceInfo->m_v0 == v1);
    }

    bool AlreadyExists(NvFaceInfo* faceInfo, NvFaceInfoVec& faceInfos);  // 0x0047C270
    float AvgStripSize(const NvStripInfoVec& strips);                    // 0x0047C830
    void BuildStripifyInfo(NvFaceInfoVec& faceInfos, NvEdgeInfoVec& edgeInfos);
    void CreateStrips(const NvStripInfoVec& allStrips, WordVec& stripIndices, WordVec& stripLengths);
    void CommitStrips(NvStripInfoVec& allStrips, const NvStripInfoVec& strips, int unused);
    void RemoveSmallStrips(NvStripInfoVec& allStrips, NvStripInfoVec& allBigStrips, NvFaceInfoVec& faceList);
    void FindAllStrips(NvStripInfoVec& allStrips, NvFaceInfoVec& allFaceInfos,
                       NvEdgeInfoVec& allEdgeInfos, int numSamples, int maxIndex);  // 0x00480490
    void SplitUpStripsAndOptimize(NvStripInfoVec& allStrips, NvStripInfoVec& outStrips,
                                  NvEdgeInfoVec& edgeInfos, NvFaceInfoVec& outFaceList);  // 0x0047F560
    void Stripify(const WordVec& in_indices, const int in_cacheSize, const int in_minStripLength,
                  int maxIndex, NvStripInfoVec& outStrips, NvFaceInfoVec& outFaceList);

    WordVec indices;            // +0x00
    int cacheSize;              // +0x10
    int minStripLength;         // +0x14
    float meshJump;             // +0x18
    bool bFirstTimeResetPoint;  // +0x1C
};

#define SWAP(a, b) { int t = a; a = b; b = t; }

extern bool g_bNvStripBadEdge;                      // 0x008302A4
extern CExoArrayList<NvFaceInfo*> g_lNvStripFaces;  // 0x008302A8
extern int g_nNvStripNumSamples;                    // 0x00798A94

#endif
