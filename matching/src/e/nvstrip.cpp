// NvTriStrip (NVIDIA's public stripifier, NvTriStripObjects.cpp,
// as modified by BioWare), 0x0047BB80 .. 0x00481140. STL instances it pulls
// in are annotated in nvstrip_stl.cpp (split so matchcmp's UNDNAME command
// line stays under the Windows limit).

#include "e/nvstrip.h"

NvEdgeInfo* NvStripifier::FindEdgeInfo(NvEdgeInfoVec& edgeInfos, int v0, int v1)
{
    NvEdgeInfo* infoIter = edgeInfos[v0];
    while (infoIter != NULL) {
        if (infoIter->m_v0 == v0) {
            if (infoIter->m_v1 == v1)
                return infoIter;
            else
                infoIter = infoIter->m_nextV0;
        } else {
            if (infoIter->m_v0 == v1)
                return infoIter;
            else
                infoIter = infoIter->m_nextV1;
        }
    }
    return NULL;
}

// FUNCTION: SWKOTOR 0x0047BB80
void NvStripifier::UpdateCacheFace(VertexCache* vcache, NvFaceInfo* face)
{
    if (!vcache->InCache(face->m_v0))
        vcache->AddEntry(face->m_v0);
    if (!vcache->InCache(face->m_v1))
        vcache->AddEntry(face->m_v1);
    if (!vcache->InCache(face->m_v2))
        vcache->AddEntry(face->m_v2);
}

// FUNCTION: SWKOTOR 0x0047C2C0
int NvStripifier::FindStartPoint(NvFaceInfoVec& faceInfos, NvEdgeInfoVec& edgeInfos)
{
    for (unsigned int i = 0; i < faceInfos.size(); i++) {
        int ctr = 0;
        if (FindOtherFace(edgeInfos, faceInfos[i]->m_v0, faceInfos[i]->m_v1, faceInfos[i]) == NULL)
            ctr++;
        if (FindOtherFace(edgeInfos, faceInfos[i]->m_v1, faceInfos[i]->m_v2, faceInfos[i]) == NULL)
            ctr++;
        if (FindOtherFace(edgeInfos, faceInfos[i]->m_v2, faceInfos[i]->m_v0, faceInfos[i]) == NULL)
            ctr++;
        if (ctr > 1)
            return i;
    }
    return -1;
}

// FUNCTION: SWKOTOR 0x0047C400
NvFaceInfo* NvStripifier::FindGoodResetPoint(NvFaceInfoVec& faceInfos, NvEdgeInfoVec& edgeInfos)
{
    NvFaceInfo* result = NULL;
    int numFaces = faceInfos.size();
    int startPoint;
    if (bFirstTimeResetPoint) {
        startPoint = FindStartPoint(faceInfos, edgeInfos);
        bFirstTimeResetPoint = false;
    } else
        startPoint = (int)(((float)numFaces - 1) * meshJump);

    if (startPoint == -1)
        startPoint = (int)(((float)numFaces - 1) * meshJump);

    int i = startPoint;
    do {
        if (faceInfos[i]->m_stripId < 0) {
            result = faceInfos[i];
            break;
        }
        if (++i >= numFaces)
            i = 0;
    } while (i != startPoint);

    meshJump += 0.1f;
    if (meshJump > 1.0f)
        meshJump = .05f;

    return result;
}

// FUNCTION: SWKOTOR 0x0047C4C0
int NvStripifier::GetNextIndex(const WordVec& indices, NvFaceInfo* face)
{
    int numIndices = indices.size();
    int v0 = indices[numIndices - 2];
    int v1 = indices[numIndices - 1];

    int fv0 = face->m_v0;
    int fv1 = face->m_v1;
    int fv2 = face->m_v2;

    if (fv0 != v0 && fv0 != v1) {
        if ((fv1 != v0 && fv1 != v1) || (fv2 != v0 && fv2 != v1)) {
            printf("GetNextIndex: Triangle doesn't have all of its vertices\n");
            printf("GetNextIndex: Duplicate triangle probably got us derailed\n");
        }
        return fv0;
    }
    if (fv1 != v0 && fv1 != v1) {
        if ((fv0 != v0 && fv0 != v1) || (fv2 != v0 && fv2 != v1)) {
            printf("GetNextIndex: Triangle doesn't have all of its vertices\n");
            printf("GetNextIndex: Duplicate triangle probably got us derailed\n");
        }
        return fv1;
    }
    if (fv2 != v0 && fv2 != v1) {
        if ((fv0 != v0 && fv0 != v1) || (fv1 != v0 && fv1 != v1)) {
            printf("GetNextIndex: Triangle doesn't have all of its vertices\n");
            printf("GetNextIndex: Duplicate triangle probably got us derailed\n");
        }
        return fv2;
    }

    printf("GetNextIndex: Duplicate triangle sent\n");
    return -1;
}

// FUNCTION: SWKOTOR 0x0047C5B0
void NvStripifier::UpdateCacheStrip(VertexCache* vcache, NvStripInfo* strip)
{
    for (unsigned int i = 0; i < strip->m_faces.size(); ++i) {
        if (!vcache->InCache(strip->m_faces[i]->m_v0))
            vcache->AddEntry(strip->m_faces[i]->m_v0);
        if (!vcache->InCache(strip->m_faces[i]->m_v1))
            vcache->AddEntry(strip->m_faces[i]->m_v1);
        if (!vcache->InCache(strip->m_faces[i]->m_v2))
            vcache->AddEntry(strip->m_faces[i]->m_v2);
    }
}

// FUNCTION: SWKOTOR 0x0047C6D0
float NvStripifier::CalcNumHitsStrip(VertexCache* vcache, NvStripInfo* strip)
{
    int numHits = 0;
    int numFaces = 0;
    for (unsigned int i = 0; i < strip->m_faces.size(); i++) {
        if (vcache->InCache(strip->m_faces[i]->m_v0))
            ++numHits;
        if (vcache->InCache(strip->m_faces[i]->m_v1))
            ++numHits;
        if (vcache->InCache(strip->m_faces[i]->m_v2))
            ++numHits;
        numFaces++;
    }
    return ((float)numHits / (float)numFaces);
}

// FUNCTION: SWKOTOR 0x0047C7A0
int NvStripifier::NumNeighbors(NvFaceInfo* face, NvEdgeInfoVec& edgeInfoVec)
{
    int numNeighbors = 0;
    if (FindOtherFace(edgeInfoVec, face->m_v0, face->m_v1, face) != NULL)
        numNeighbors++;
    if (FindOtherFace(edgeInfoVec, face->m_v1, face->m_v2, face) != NULL)
        numNeighbors++;
    if (FindOtherFace(edgeInfoVec, face->m_v2, face->m_v0, face) != NULL)
        numNeighbors++;
    return numNeighbors;
}

// FUNCTION: SWKOTOR 0x0047C960
bool NvStripInfo::SharesEdge(const NvFaceInfo* faceInfo, NvEdgeInfoVec& edgeInfos)
{
    NvEdgeInfo* currEdge = NvStripifier::FindEdgeInfo(edgeInfos, faceInfo->m_v0, faceInfo->m_v1);
    if (IsInStrip(currEdge->m_face0) || IsInStrip(currEdge->m_face1))
        return true;

    currEdge = NvStripifier::FindEdgeInfo(edgeInfos, faceInfo->m_v1, faceInfo->m_v2);
    if (IsInStrip(currEdge->m_face0) || IsInStrip(currEdge->m_face1))
        return true;

    currEdge = NvStripifier::FindEdgeInfo(edgeInfos, faceInfo->m_v2, faceInfo->m_v0);
    if (IsInStrip(currEdge->m_face0) || IsInStrip(currEdge->m_face1))
        return true;

    return false;
}

// FUNCTION: SWKOTOR 0x0047CCE0
bool NvStripifier::FindTraversal(NvFaceInfoVec& faceInfos, NvEdgeInfoVec& edgeInfos,
                                 NvStripInfo* strip, NvStripStartInfo& startInfo)
{
    int v = (strip->m_startInfo.m_toV1 ? strip->m_startInfo.m_startEdge->m_v1
                                       : strip->m_startInfo.m_startEdge->m_v0);
    NvFaceInfo* untouchedFace = NULL;
    NvEdgeInfo* edgeIter = edgeInfos[v];
    while (edgeIter != NULL) {
        NvFaceInfo* face0 = edgeIter->m_face0;
        NvFaceInfo* face1 = edgeIter->m_face1;
        if ((face0 != NULL && !strip->IsInStrip(face0)) && face1 != NULL && !strip->IsMarked(face1)) {
            untouchedFace = face1;
            break;
        }
        if ((face1 != NULL && !strip->IsInStrip(face1)) && face0 != NULL && !strip->IsMarked(face0)) {
            untouchedFace = face0;
            break;
        }
        edgeIter = (edgeIter->m_v0 == v ? edgeIter->m_nextV0 : edgeIter->m_nextV1);
    }

    startInfo.m_startFace = untouchedFace;
    startInfo.m_startEdge = edgeIter;
    if (edgeIter != NULL) {
        if (strip->SharesEdge(startInfo.m_startFace, edgeInfos))
            startInfo.m_toV1 = (edgeIter->m_v0 == v);
        else
            startInfo.m_toV1 = (edgeIter->m_v1 == v);
    }
    return (startInfo.m_startFace != NULL);
}

// FUNCTION: SWKOTOR 0x0047ED60
void NvStripifier::BuildStripifyInfo(NvFaceInfoVec& faceInfos, NvEdgeInfoVec& edgeInfos)
{
    int numIndices = indices.size();
    faceInfos.reserve(numIndices);

    edgeInfos.resize(numIndices);
    for (int i = 0; i < numIndices; i++)
        edgeInfos[i] = NULL;

    int numTriangles = numIndices / 3;
    int index = 0;
    for (i = 0; i < numTriangles; i++) {
        int v0 = indices[index++];
        int v1 = indices[index++];
        int v2 = indices[index++];

        NvFaceInfo* faceInfo = new NvFaceInfo(v0, v1, v2);
        if (!AlreadyExists(faceInfo, faceInfos)) {
            faceInfos.push_back(faceInfo);

            NvEdgeInfo* edgeInfo01 = FindEdgeInfo(edgeInfos, v0, v1);
            if (edgeInfo01 == NULL) {
                edgeInfo01 = new NvEdgeInfo(v0, v1);
                edgeInfo01->m_nextV0 = edgeInfos[v0];
                edgeInfo01->m_nextV1 = edgeInfos[v1];
                edgeInfos[v0] = edgeInfo01;
                edgeInfos[v1] = edgeInfo01;
                edgeInfo01->m_face0 = faceInfo;
            } else {
                if (edgeInfo01->m_face1 != NULL) {
                    printf("BuildStripifyInfo: > 2 triangles on an edge... uncertain consequences\n");
                    g_bNvStripBadEdge = true;
                } else
                    edgeInfo01->m_face1 = faceInfo;
            }

            NvEdgeInfo* edgeInfo12 = FindEdgeInfo(edgeInfos, v1, v2);
            if (edgeInfo12 == NULL) {
                edgeInfo12 = new NvEdgeInfo(v1, v2);
                edgeInfo12->m_nextV0 = edgeInfos[v1];
                edgeInfo12->m_nextV1 = edgeInfos[v2];
                edgeInfos[v1] = edgeInfo12;
                edgeInfos[v2] = edgeInfo12;
                edgeInfo12->m_face0 = faceInfo;
            } else {
                if (edgeInfo12->m_face1 != NULL)
                    printf("BuildStripifyInfo: > 2 triangles on an edge... uncertain consequences\n");
                else
                    edgeInfo12->m_face1 = faceInfo;
            }

            NvEdgeInfo* edgeInfo20 = FindEdgeInfo(edgeInfos, v2, v0);
            if (edgeInfo20 == NULL) {
                edgeInfo20 = new NvEdgeInfo(v2, v0);
                edgeInfo20->m_nextV0 = edgeInfos[v2];
                edgeInfo20->m_nextV1 = edgeInfos[v0];
                edgeInfos[v2] = edgeInfo20;
                edgeInfos[v0] = edgeInfo20;
                edgeInfo20->m_face0 = faceInfo;
            } else {
                if (edgeInfo20->m_face1 != NULL)
                    printf("BuildStripifyInfo: > 2 triangles on an edge... uncertain consequences\n");
                else
                    edgeInfo20->m_face1 = faceInfo;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0047EB30
void NvStripInfo::Combine(const NvFaceInfoVec& forward, const NvFaceInfoVec& backward)
{
    int numFaces = backward.size();
    for (int i = numFaces - 1; i >= 0; i--)
        m_faces.push_back(backward[i]);

    numFaces = forward.size();
    for (i = 0; i < numFaces; i++)
        m_faces.push_back(forward[i]);
}

// FUNCTION: SWKOTOR 0x0047F0D0
void NvStripifier::CommitStrips(NvStripInfoVec& allStrips, const NvStripInfoVec& strips, int unused)
{
    int numStrips = strips.size();
    for (int i = 0; i < numStrips; i++) {
        NvStripInfo* strip = strips[i];
        strip->m_experimentId = -1;

        allStrips.push_back(strip);

        const NvFaceInfoVec& faces = strips[i]->m_faces;
        int numFaces = faces.size();

        if (faces[0]->m_v0 == 2302 && faces[0]->m_v1 == 3215 && faces[0]->m_v2 == 2603)
            printf("BLEH");

        for (int j = 0; j < numFaces; j++)
            strip->MarkTriangle(faces[j]);
    }
}

// FUNCTION: SWKOTOR 0x0047F220
void NvStripifier::RemoveSmallStrips(NvStripInfoVec& allStrips, NvStripInfoVec& allBigStrips, NvFaceInfoVec& faceList)
{
    faceList.clear();
    allBigStrips.clear();

    NvFaceInfoVec tempFaceList;

    for (int i = 0; i < allStrips.size(); i++) {
        if ((int)allStrips[i]->m_faces.size() < minStripLength) {
            for (int j = 0; j < allStrips[i]->m_faces.size(); j++)
                tempFaceList.push_back(allStrips[i]->m_faces[j]);
            delete allStrips[i];
        } else {
            allBigStrips.push_back(allStrips[i]);
        }
    }

    bool* bVisitedList = NULL;
    if (tempFaceList.size() > 0) {
        bVisitedList = new bool[tempFaceList.size()];
        memset(bVisitedList, 0, tempFaceList.size() * sizeof(bool));
    }

    VertexCache* vcache = new VertexCache(cacheSize);

    int bestNumHits = -1;
    int numHits;
    int bestIndex;

    while (1) {
        bestNumHits = -1;

        for (i = 0; i < tempFaceList.size(); i++) {
            if (bVisitedList[i])
                continue;

            numHits = CalcNumHitsFace(vcache, tempFaceList[i]);
            if (numHits > bestNumHits) {
                bestNumHits = numHits;
                bestIndex = i;
            }
        }

        if (bestNumHits == -1.0f)
            break;
        bVisitedList[bestIndex] = true;
        UpdateCacheFace(vcache, tempFaceList[bestIndex]);
        faceList.push_back(tempFaceList[bestIndex]);
    }

    delete vcache;
    delete[] bVisitedList;
}

// FUNCTION: SWKOTOR 0x0047E720
void NvStripifier::CreateStrips(const NvStripInfoVec& allStrips, WordVec& stripIndices, WordVec& stripLengths)
{
    int nStripCount = allStrips.size();
    if (nStripCount == 0)
        return;

    stripLengths.resize(nStripCount, 0);

    NvFaceInfo tLastFace(0, 0, 0);
    int prevSize = 0;

    for (int i = 0; i < nStripCount; i++) {
        NvStripInfo* strip = allStrips[i];
        int nStripFaceCount = strip->m_faces.size();

        NvFaceInfo tFirstFace(strip->m_faces[0]->m_v0, strip->m_faces[0]->m_v1, strip->m_faces[0]->m_v2);

        if (nStripFaceCount > 1) {
            int nUnique = GetUniqueVertexInB(strip->m_faces[1], &tFirstFace);
            if (nUnique == tFirstFace.m_v1) {
                SWAP(tFirstFace.m_v0, tFirstFace.m_v1);
            } else if (nUnique == tFirstFace.m_v2) {
                SWAP(tFirstFace.m_v0, tFirstFace.m_v2);
            }

            if (nStripFaceCount > 2) {
                int nShared = GetSharedVertex(strip->m_faces[2], &tFirstFace);
                if (nShared == tFirstFace.m_v1) {
                    SWAP(tFirstFace.m_v1, tFirstFace.m_v2);
                }
            }
        }

        if (!IsCW(strip->m_faces[0], tFirstFace.m_v0, tFirstFace.m_v1))
            stripIndices.push_back(tFirstFace.m_v0);

        stripIndices.push_back(tFirstFace.m_v0);
        stripIndices.push_back(tFirstFace.m_v1);
        stripIndices.push_back(tFirstFace.m_v2);

        tLastFace = tFirstFace;

        for (int j = 1; j < nStripFaceCount; j++) {
            int nUnique = GetUniqueVertexInB(&tLastFace, strip->m_faces[j]);
            if (nUnique != -1) {
                stripIndices.push_back(nUnique);
                tLastFace.m_v0 = tLastFace.m_v1;
                tLastFace.m_v1 = tLastFace.m_v2;
                tLastFace.m_v2 = nUnique;
            }
        }

        stripLengths[i] = stripIndices.size() - prevSize;
        prevSize = stripIndices.size();
    }
}

// The original keeps scratchIndices as a function-local static (guard bit
// 0x008302C4, vector at 0x008302B4). Its atexit thunk gets the short COFF name
// `_$E2`, which matchcmp's read_coff mis-parses (it strips all 18 bytes of
// the symbol entry, not the 8-byte name), so the static lives outside here.
extern WordVec g_nvScratchIndices;

// FUNCTION: SWKOTOR 0x0047FD70
// NONMATCH: guard/atexit prologue (see above); with the real static the body
// still differs by register allocation: MSVC inlines FindEdgeInfo into the
// first forward FindOtherFace, the original calls 0x0047C230 there.
void NvStripInfo::Build(NvEdgeInfoVec& edgeInfos, NvFaceInfoVec& faceInfos)
{
    WordVec& scratchIndices = g_nvScratchIndices;
    scratchIndices.resize(0);

    NvFaceInfoVec forwardFaces, backwardFaces;
    forwardFaces.push_back(m_startInfo.m_startFace);

    MarkTriangle(m_startInfo.m_startFace);

    int v0 = (m_startInfo.m_toV1 ? m_startInfo.m_startEdge->m_v0 : m_startInfo.m_startEdge->m_v1);
    int v1 = (m_startInfo.m_toV1 ? m_startInfo.m_startEdge->m_v1 : m_startInfo.m_startEdge->m_v0);

    scratchIndices.push_back(v0);
    scratchIndices.push_back(v1);
    int v2 = NvStripifier::GetNextIndex(scratchIndices, m_startInfo.m_startFace);
    scratchIndices.push_back(v2);

    int nv0 = v1;
    int nv1 = v2;

    NvFaceInfo* nextFace = NvStripifier::FindOtherFace(edgeInfos, nv0, nv1, m_startInfo.m_startFace);
    while (nextFace != NULL && !IsMarked(nextFace)) {
        if (!Unique(forwardFaces, nextFace))
            break;

        forwardFaces.push_back(nextFace);
        MarkTriangle(nextFace);

        nv0 = nv1;
        nv1 = NvStripifier::GetNextIndex(scratchIndices, nextFace);
        scratchIndices.push_back(nv1);

        nextFace = NvStripifier::FindOtherFace(edgeInfos, nv0, nv1, nextFace);
    }

    NvFaceInfoVec tempAllFaces;
    for (unsigned int i = 0; i < forwardFaces.size(); i++)
        tempAllFaces.push_back(forwardFaces[i]);

    scratchIndices.resize(0);
    scratchIndices.push_back(v2);
    scratchIndices.push_back(v1);
    scratchIndices.push_back(v0);
    nv0 = v1;
    nv1 = v0;
    nextFace = NvStripifier::FindOtherFace(edgeInfos, nv0, nv1, m_startInfo.m_startFace);
    while (nextFace != NULL && !IsMarked(nextFace)) {
        if (!Unique(tempAllFaces, nextFace))
            break;

        backwardFaces.push_back(nextFace);
        tempAllFaces.push_back(nextFace);
        MarkTriangle(nextFace);

        nv0 = nv1;
        nv1 = NvStripifier::GetNextIndex(scratchIndices, nextFace);
        scratchIndices.push_back(nv1);

        nextFace = NvStripifier::FindOtherFace(edgeInfos, nv0, nv1, nextFace);
    }

    Combine(forwardFaces, backwardFaces);
}

// FUNCTION: SWKOTOR 0x00480EF0
// NONMATCH: the three cleanup loops keep _Mylast in a register and reload
// _Myfirst each pass in the original; ours does the opposite.
void NvStripifier::Stripify(const WordVec& in_indices, const int in_cacheSize, const int in_minStripLength,
                            int maxIndex, NvStripInfoVec& outStrips, NvFaceInfoVec& outFaceList)
{
    meshJump = 0.0f;
    bFirstTimeResetPoint = true;
    cacheSize = in_cacheSize;
    minStripLength = in_minStripLength;
    indices = in_indices;

    NvFaceInfoVec allFaceInfos;
    NvEdgeInfoVec allEdgeInfos;
    BuildStripifyInfo(allFaceInfos, allEdgeInfos);

    NvStripInfoVec allStrips;
    FindAllStrips(allStrips, allFaceInfos, allEdgeInfos, g_nNvStripNumSamples, maxIndex);

    SplitUpStripsAndOptimize(allStrips, outStrips, allEdgeInfos, outFaceList);

    for (int i = 0; i < allStrips.size(); i++)
        delete allStrips[i];

    for (i = 0; i < allEdgeInfos.size(); i++) {
        NvEdgeInfo* info = allEdgeInfos[i];
        while (info != NULL) {
            NvEdgeInfo* next = (info->m_v0 == i ? info->m_nextV0 : info->m_nextV1);
            info->Unref();
            info = next;
        }
    }

    g_lNvStripFaces.num = 0;
    for (i = 0; i < allFaceInfos.size(); i++)
        g_lNvStripFaces.Add(allFaceInfos[i]);
}
