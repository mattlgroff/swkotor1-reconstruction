// NvTriStrip leaf members the original keeps out of line
// (NvTriStripObjects.cpp); worker e's nvstrip.cpp has the callers.

#include "h/nvstrip.h"

// FUNCTION: SWKOTOR 0x0047BA70
bool NvStripInfo::IsInStrip(const NvFaceInfo* faceInfo) const
{
    if (faceInfo == NULL)
        return false;
    return (m_experimentId >= 0 ? faceInfo->m_testStripId == m_stripId
                                : faceInfo->m_stripId == m_stripId);
}

// FUNCTION: SWKOTOR 0x0047BC40
int NvStripifier::CalcNumHitsFace(VertexCache* vcache, NvFaceInfo* face)
{
    int numHits = 0;
    if (vcache->InCache(face->m_v0))
        numHits++;
    if (vcache->InCache(face->m_v1))
        numHits++;
    if (vcache->InCache(face->m_v2))
        numHits++;
    return numHits;
}

// FUNCTION: SWKOTOR 0x0047C230
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

// FUNCTION: SWKOTOR 0x0047C270
bool NvStripifier::AlreadyExists(NvFaceInfo* faceInfo, NvFaceInfoVec& faceInfos)
{
    for (unsigned int i = 0; i < faceInfos.size(); ++i) {
        if ((faceInfos[i]->m_v0 == faceInfo->m_v0) &&
            (faceInfos[i]->m_v1 == faceInfo->m_v1) &&
            (faceInfos[i]->m_v2 == faceInfo->m_v2))
            return true;
    }
    return false;
}

// FUNCTION: SWKOTOR 0x0047C830
float NvStripifier::AvgStripSize(const NvStripInfoVec& strips)
{
    int sizeAccum = 0;
    int numStrips = strips.size();
    for (int i = 0; i < numStrips; i++) {
        NvStripInfo* strip = strips[i];
        sizeAccum += strip->m_faces.size();
    }
    return ((float)sizeAccum) / ((float)numStrips);
}

// FUNCTION: SWKOTOR 0x0047C8A0
bool NvStripInfo::Unique(NvFaceInfoVec& faceVec, NvFaceInfo* face)
{
    bool bv0, bv1, bv2;
    bv0 = bv1 = bv2 = false;

    for (unsigned int i = 0; i < faceVec.size(); i++) {
        if (!bv0) {
            if ((faceVec[i]->m_v0 == face->m_v0) ||
                (faceVec[i]->m_v1 == face->m_v0) ||
                (faceVec[i]->m_v2 == face->m_v0))
                bv0 = true;
        }
        if (!bv1) {
            if ((faceVec[i]->m_v0 == face->m_v1) ||
                (faceVec[i]->m_v1 == face->m_v1) ||
                (faceVec[i]->m_v2 == face->m_v1))
                bv1 = true;
        }
        if (!bv2) {
            if ((faceVec[i]->m_v0 == face->m_v2) ||
                (faceVec[i]->m_v1 == face->m_v2) ||
                (faceVec[i]->m_v2 == face->m_v2))
                bv2 = true;
        }
        if (bv0 && bv1 && bv2)
            return false;
    }
    return true;
}

// FUNCTION: SWKOTOR 0x0047F560
void NvStripifier::SplitUpStripsAndOptimize(NvStripInfoVec& allStrips, NvStripInfoVec& outStrips,
                                            NvEdgeInfoVec& edgeInfos, NvFaceInfoVec& outFaceList)
{
    int threshold = 1000;
    NvStripInfoVec tempStrips;

    // split up strips into threshold-sized pieces
    for (int i = 0; i < allStrips.size(); i++) {
        NvStripStartInfo startInfo(NULL, NULL, false);
        NvStripInfo* currentStrip;

        if (allStrips[i]->m_faces.size() > threshold) {
            unsigned int numTimes = allStrips[i]->m_faces.size() / threshold;
            unsigned int numLeftover = allStrips[i]->m_faces.size() % threshold;

            for (unsigned int j = 0; j < numTimes; j++) {
                currentStrip = new NvStripInfo(startInfo, 0, -1);

                for (unsigned int faceCtr = j * threshold; faceCtr < threshold + (j * threshold); faceCtr++)
                    currentStrip->m_faces.push_back(allStrips[i]->m_faces[faceCtr]);

                tempStrips.push_back(currentStrip);
            }

            unsigned int leftOff = numTimes * threshold;

            if (numLeftover != 0) {
                currentStrip = new NvStripInfo(startInfo, 0, -1);

                for (unsigned int k = 0; k < numLeftover; k++)
                    currentStrip->m_faces.push_back(allStrips[i]->m_faces[leftOff++]);

                tempStrips.push_back(currentStrip);
            }
        } else {
            currentStrip = new NvStripInfo(startInfo, 0, -1);

            for (int j = 0; j < allStrips[i]->m_faces.size(); j++)
                currentStrip->m_faces.push_back(allStrips[i]->m_faces[j]);

            tempStrips.push_back(currentStrip);
        }
    }

    // add small strips to face list
    {
    NvStripInfoVec tempStrips2;
    RemoveSmallStrips(tempStrips, tempStrips2, outFaceList);

    outStrips.clear();
    if (tempStrips2.size() != 0) {
        // optimize for the vertex cache
        VertexCache* vcache = new VertexCache(cacheSize);

        float bestNumHits = -1.0f;
        float numHits;
        int bestIndex;

        int firstIndex = 0;
        float minCost = 10000.0f;

        for (int i = 0; i < tempStrips2.size(); i++) {
            int numNeighbors = 0;

            // find strip with least number of neighbors per face
            for (int j = 0; j < tempStrips2[i]->m_faces.size(); j++)
                numNeighbors += NumNeighbors(tempStrips2[i]->m_faces[j], edgeInfos);

            float currCost = (float)numNeighbors / (float)tempStrips2[i]->m_faces.size();
            if (currCost < minCost) {
                minCost = currCost;
                firstIndex = i;
            }
        }

        UpdateCacheStrip(vcache, tempStrips2[firstIndex]);
        outStrips.push_back(tempStrips2[firstIndex]);

        tempStrips2[firstIndex]->visited = true;

        while (1) {
            bestNumHits = -1.0f;

            // find best strip to add next, given the current cache
            for (int i = 0; i < tempStrips2.size(); i++) {
                if (tempStrips2[i]->visited)
                    continue;

                numHits = CalcNumHitsStrip(vcache, tempStrips2[i]);
                if (numHits > bestNumHits) {
                    bestNumHits = numHits;
                    bestIndex = i;
                }
            }

            if (bestNumHits == -1.0f)
                break;
            tempStrips2[bestIndex]->visited = true;
            UpdateCacheStrip(vcache, tempStrips2[bestIndex]);
            outStrips.push_back(tempStrips2[bestIndex]);
        }

        delete vcache;
    }
    }
}
