// CSWCollisionMesh: text-line reader, world transforms, normals, bounds,
// outline and the binary walkmesh writer (0x00596670-0x0059A040).

#include <string.h>
#include "f/room_surface_mesh.h"

// FUNCTION: SWKOTOR 0x00596670
int CSWCollisionMesh::LoadMesh(int bTextOnly)
{
    int bResult = 1;
    CResRef cResRef;
    GetResRef(&cResRef);
    if (g_pExoResMan->Exists(cResRef, m_n48, NULL)) {
        CResBWM *pRes = (CResBWM *)g_pExoResMan->GetResObject(cResRef, m_n48);
        if (!pRes) {
            pRes = new CResBWM();
            g_pExoResMan->SetResObject(cResRef, m_n48, pRes);
        }
        if (pRes->Demand()) {
            if (pRes->IsBinary()) {
                if (bTextOnly) {
                    pRes->Release();
                    pRes->Dump(1);
                    return 0;
                }
                m_bLoaded = 1;
                bResult = LoadMeshBinary(pRes);
                m_pRes = pRes;
            } else {
                m_bLoaded = 0;
                bResult = LoadMeshText(pRes->GetBWMDataPtr(), pRes->GetBWMSize());
                pRes->Release();
                pRes->Dump(1);
            }
        }
    } else {
        GetResRef2(&cResRef);
        if (g_pExoResMan->Exists(cResRef, m_n4A, NULL)) {
            CResMDL *pRes = (CResMDL *)g_pExoResMan->GetResObject(cResRef, m_n4A);
            if (!pRes) {
                pRes = new CResMDL();
                g_pExoResMan->SetResObject(cResRef, m_n4A, pRes);
            }
            if (pRes->Demand()) {
                m_bLoaded = 0;
                bResult = LoadMeshText(pRes->GetMDLDataPtr(), pRes->GetMDLSize());
                pRes->Release();
                pRes->Dump(1);
            }
        }
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x00596890
int CSWCollisionMesh::LoadMeshText(char *pData, int nSize)
{
    m_bLoaded = 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x005968A0
int CSWCollisionMesh::GetLine(char **ppData, unsigned long *pnSize, char *pLine, unsigned long nMaxLength)
{
    unsigned long i = (unsigned long)-1;
    if (*pnSize == 0)
        return 0;
    do {
        i++;
        pLine[i] = **ppData;
        (*ppData)++;
        (*pnSize)--;
    } while (i < nMaxLength - 1 && pLine[i] != '\n' && *pnSize > 0);
    if (i >= nMaxLength)
        return 0;
    pLine[i] = '\0';
    return 1;
}

// FUNCTION: SWKOTOR 0x00596AA0
Vector CSWCollisionMesh::GetWorldPoint(const Vector &v)
{
    if (m_bTransformed)
        return v;
    return m_vPosition + m_qOrientation * (v + m_vOffset);
}

// FUNCTION: SWKOTOR 0x00596C10
void CSWCollisionMesh::ComputeNormals()
{
    if (m_bLoaded)
        return;
    for (int i = 0; i < m_nFaces; i++) {
        Vector v0 = m_pvVertices[m_pFaces[i].m_anVertex[0]];
        Vector v1 = m_pvVertices[m_pFaces[i].m_anVertex[1]];
        Vector v2 = m_pvVertices[m_pFaces[i].m_anVertex[2]];
        Vector vNormal = NormalOf(v0, v1, v2);
        m_pvNormals[i] = vNormal;
        m_pfPlaneDistances[i] = -(vNormal.x * v0.x + vNormal.y * v0.y + vNormal.z * v0.z);
    }
}

// FUNCTION: SWKOTOR 0x00596D10
int CSWCollisionMesh::GetMeshBoundingBox(Vector &vMin, Vector &vMax)
{
    if (!m_pvVertices)
        return 0;
    int bFound = 0;
    for (int i = 0; i < m_nVertices; i++) {
        Vector v;
        v = GetWorldPoint(m_pvVertices[i]);
        if (!bFound) {
            vMax = v;
            vMin = v;
            bFound = 1;
        } else {
            if (v.x > vMax.x)
                vMax.x = v.x;
            if (v.x < vMin.x)
                vMin.x = v.x;
            if (v.y > vMax.y)
                vMax.y = v.y;
            if (v.y < vMin.y)
                vMin.y = v.y;
            if (v.z > vMax.z)
                vMax.z = v.z;
            if (v.z < vMin.z)
                vMin.z = v.z;
        }
    }
    return bFound;
}

// FUNCTION: SWKOTOR 0x00596E40
int CSWCollisionMesh::GetMeshOutline(Vector *pCorner1, Vector *pCorner2, Vector *pCorner3, Vector *pCorner4)
{
    if (!m_pvVertices)
        return 0;
    int bFound = 0;
    Vector vMin;
    Vector vMax;
    for (int i = 0; i < m_nVertices; i++) {
        Vector v = m_pvVertices[i];
        if (!bFound) {
            vMax = v;
            vMin = v;
            bFound = 1;
        } else {
            if (v.x > vMax.x)
                vMax.x = v.x;
            if (v.x < vMin.x)
                vMin.x = v.x;
            if (v.y > vMax.y)
                vMax.y = v.y;
            if (v.y < vMin.y)
                vMin.y = v.y;
            if (v.z > vMax.z)
                vMax.z = v.z;
            if (v.z < vMin.z)
                vMin.z = v.z;
        }
    }
    *pCorner1 = vMin;
    pCorner1->y = vMax.y;
    pCorner1->z = vMin.z;
    *pCorner2 = vMin;
    pCorner2->z = vMin.z;
    *pCorner3 = vMax;
    pCorner3->y = vMin.y;
    pCorner3->z = vMin.z;
    *pCorner4 = vMax;
    pCorner4->z = vMin.z;
    *pCorner1 = GetWorldPoint(*pCorner1);
    *pCorner2 = GetWorldPoint(*pCorner2);
    *pCorner3 = GetWorldPoint(*pCorner3);
    *pCorner4 = GetWorldPoint(*pCorner4);
    return bFound;
}

// FUNCTION: SWKOTOR 0x00597090
void CSWCollisionMesh::GetVertex(int n, float *pX, float *pY, float *pZ)
{
    if (m_pvVertices && n < m_nVertices) {
        Vector v = GetWorldPoint(m_pvVertices[n]);
        *pX = v.x;
        *pY = v.y;
        *pZ = v.z;
    }
}

// FUNCTION: SWKOTOR 0x00599ED0
void CSWCollisionMesh::TransformToWorld(const Vector &vPosition)
{
    if (m_bLoaded)
        return;
    m_vPosition = vPosition;
    m_bTransformed = 1;
    for (int i = 0; i < m_nVertices; i++)
        m_pvVertices[i] = m_vOffset + m_pvVertices[i] + m_vPosition;
}

// FUNCTION: SWKOTOR 0x0059A040
int CSWCollisionMesh::SaveBinaryMesh(const CExoString &sFile)
{
    if (m_bLoaded)
        return -1;
    FILE *pFile = fopen(sFile.CStr(), "r");
    if (pFile) {
        CExoString sError;
        sError.Format("ERROR: opening a Binary walkmesh file for writeing that already exists (File: %s)", sFile.CStr());
        fclose(pFile);
        return -1;
    }
    pFile = fopen(sFile.CStr(), "wb+");
    if (!pFile)
        return 0;
    CSWWalkMeshHeader cHeader;
    memset(&cHeader, 0, sizeof(cHeader));
    strcpy(cHeader.m_sFileType, "BWM V1.0");
    cHeader.m_nType = m_bTransformed;
    cHeader.m_avUse[0] = m_avUse[0];
    cHeader.m_avUse[1] = m_avUse[1];
    cHeader.m_vPosition = m_vOffset;
    cHeader.m_nVertices = m_nVertices;
    cHeader.m_nFaces = m_nFaces;
    cHeader.m_nWalkableFaces = m_nWalkableFaces;
    fwrite(&cHeader, sizeof(cHeader), 1, pFile);
    cHeader.m_nVertexOffset = ftell(pFile);
    fwrite(m_pvVertices, sizeof(Vector), m_nVertices, pFile);
    cHeader.m_nFaceOffset = ftell(pFile);
    fwrite(m_pFaces, sizeof(int), m_nFaces * 3, pFile);
    cHeader.m_nMaterialOffset = ftell(pFile);
    fwrite(m_pnMaterials, sizeof(int), m_nFaces, pFile);
    cHeader.m_nNormalOffset = ftell(pFile);
    fwrite(m_pvNormals, sizeof(Vector), m_nFaces, pFile);
    cHeader.m_nPlaneOffset = ftell(pFile);
    fwrite(m_pfPlaneDistances, sizeof(float), m_nFaces, pFile);
    fseek(pFile, 0, SEEK_SET);
    fwrite(&cHeader, sizeof(cHeader), 1, pFile);
    fclose(pFile);
    return 1;
}
