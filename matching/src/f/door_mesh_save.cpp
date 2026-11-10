// CSWDoorSurfaceMesh::SaveBinaryMesh (vtable override, start missing from
// functions.csv): the base writer, then the header's dock points are rewritten
// with the use points in world space.

#include "f/room_surface_mesh.h"

class CSWDoorSurfaceMesh : public CSWCollisionMesh {
public:
    virtual int SaveBinaryMesh(const CExoString &sFile);
};

// FUNCTION: SWKOTOR 0x005CE960
int CSWDoorSurfaceMesh::SaveBinaryMesh(const CExoString &sFile)
{
    int bResult = CSWCollisionMesh::SaveBinaryMesh(sFile);
    if (bResult != 1)
        return bResult;
    FILE *pFile = fopen(sFile.CStr(), "rb+");
    CSWWalkMeshHeader cHeader;
    fread(&cHeader, sizeof(cHeader), 1, pFile);
    fseek(pFile, 0, SEEK_END);
    {
        Vector v = GetWorldPoint(m_avUse[0]);
        cHeader.m_avDock[0] = v;
    }
    {
        Vector v = GetWorldPoint(m_avUse[1]);
        cHeader.m_avDock[1] = v;
    }
    fseek(pFile, 0, SEEK_SET);
    fwrite(&cHeader, sizeof(cHeader), 1, pFile);
    fclose(pFile);
    return 1;
}
