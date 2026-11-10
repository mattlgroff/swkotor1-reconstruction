// Scene::SpawnRoom (0x00456F30, CAurScene slot +0x70): instances a room
// model, flattens its part tree and sorts the parts into the room's
// nodedata lists and grass bins.

extern "C" char* __cdecl strcpy(char*, const char*);
extern "C" char* __cdecl strcat(char*, const char*);
#pragma intrinsic(strcpy, strcat)
extern "C" int __cdecl sprintf(char*, const char*, ...);

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList() { if (element) delete[] element; }
    void Allocate(int n);                   // 0x005E03C0 (pointer lists)
    T& operator[](int i) { return element[i]; }
    void Add(T value)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = value;
    }
    __forceinline void AddCopy(T value)
    {
        if (num == array_size) {
            int n = array_size ? array_size * 2 : 8;
            T* old = element;
            array_size = n;
            element = new T[n];
            for (int i = 0; i < num; i++)
                element[i] = old[i];
            if (old)
                delete[] old;
        }
        element[num++] = value;
    }
};

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

class Vector0 : public Vector {
public:
    Vector0(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

class Quaternion {
public:
    float w, x, y, z;
};

Vector Max(Vector a, Vector b);             // 0x004AAEE0, descriptive
Vector Min(Vector a, Vector b);             // 0x004AAE70, descriptive

class MdlNode;
class Scene;
class Gob;
class Part;
class PartTriMesh;
class PartLight;
class PartEmitter;
class PartAABB;
class CAurTriangleBin {
public:
    void FinishGeometry();                  // 0x004A67B0
};

typedef CExoArrayList<CAurTriangleBin*> TriangleBinList;

class Model {
public:
    char pad_0x0[8];
    char m_szName[0x20];                    // +0x08
    MdlNode* m_pRoot;                       // +0x28
    char pad_0x2c[0x51 - 0x2c];
    unsigned char m_nFlags;                 // +0x51

    int AddRef(int n);                      // 0x0043E240
};

class MdlNodeTriMesh {
public:
    char pad_0x0[0x186];
    unsigned char m_bNoBounds;              // +0x186, descriptive
};

class MdlNode {
public:
    MdlNodeTriMesh* AsMdlNodeTriMesh();     // 0x0043E400
};

class Part {
public:
    virtual ~Part();                        // +0x00
    virtual void V04();
    virtual void V08();
    virtual void V0C();
    virtual void V10();
    virtual void V14();
    virtual void V18();
    virtual void V1C();
    virtual void V20();
    virtual PartLight* AsLight();           // +0x24
    virtual void V28();
    virtual PartTriMesh* AsTriMesh();       // +0x2c
    virtual void* AsSkin();                 // +0x30, descriptive
    virtual PartEmitter* AsEmitter();       // +0x34
    virtual void V38();
    virtual void V3C();
    virtual void V40();
    virtual void V44();
    virtual PartAABB* AsAABB();             // +0x48

    MdlNode* m_pNode;                       // +0x04
    char pad_0x8[0x4c - 0x8];
    Vector m_vMin;                          // +0x4c
    Vector m_vMax;                          // +0x58
    char pad_0x64[0x70 - 0x64];
    int m_n70;                              // +0x70
};

class PartTriMesh : public Part {};
class PartLight : public Part {};
class PartEmitter : public Part {};
class PartAABB : public Part {};

class Gob {
public:
    char pad_0x0[0x6c];
    Part* m_pRootPart;                      // +0x6c
    char pad_0x70[8];
    Vector m_vPosition;                     // +0x78
    Quaternion m_qOrientation;              // +0x84

    void AttachToScene(Scene* pScene);      // 0x0043F200
};

class nodedata {
public:
    CExoArrayList<PartLight*> m_lLights;    // +0x00
    CExoArrayList<Gob*> m_lGobs;            // +0x0c
    CExoArrayList<PartTriMesh*> m_lMeshes;  // +0x18
    CExoArrayList<PartEmitter*> m_lEmitters;// +0x24
    char pad_0x30[0x60 - 0x30];
    CExoArrayList<CAurTriangleBin*> m_lBins;// +0x60
    PartAABB* m_pAABB;                      // +0x6c
    char pad_0x70[0x98 - 0x70];

    nodedata();                             // 0x0046BFB0
    ~nodedata();
};

class CAurRoom {
public:
    char pad_0x0[0x1c];
    Vector m_vMin;                          // +0x1c
    Vector m_vMax;                          // +0x28
    char pad_0x34[0x58 - 0x34];
    nodedata* m_pData;                      // +0x58
    char pad_0x5c[0x74 - 0x5c];
    Gob* m_pGob;                            // +0x74
    char pad_0x78[0xb8 - 0x78];

    CAurRoom(char* szName);                 // 0x004548A0
    ~CAurRoom();
};

class LightManager {
public:
    virtual void V00();
    virtual void V04();
    virtual void V08();
    virtual void AddLight(PartLight* pLight);   // +0x0c
};

Model* FindModel(char* szName);                                         // 0x00464110
MdlNode* findnode(MdlNode* pRoot, char* szName);                        // 0x00452000
Gob* NewCAurObject(char* szModel, char* szName, int a, int b);          // 0x00449CC0
Part* CreateInstanceTree(MdlNode* pRoot, int n);                        // 0x00449410
void PartInitMaterial(int n, Part* pPart, char* szName);                // 0x00442D70
void AurTextureSetModeStatic();                                         // 0x004204B0
void AurTextureSetModeDynamic();                                        // 0x004204C0
void CollapsePartTree(Gob* pGob, Part* pTree, CExoArrayList<Part*>* pParts, Vector vPosition,
                      Quaternion qOrientation, Scene* pScene, CAurRoom* pRoom);     // 0x00453F10
void ComputePartBBox(PartTriMesh* pPart);                               // 0x00451DC0
void SceneBSPAddPart(Scene* pScene, Part* pPart, Model* pModel);        // 0x0046E290
void InitGrassBins(Vector vMin, Vector vMax, CExoArrayList<TriangleBinList>* pBins);   // 0x00456E00

class Scene {
public:
    virtual void V00(); virtual void V04(); virtual void V08(); virtual void V0C();
    virtual void V10(); virtual void V14(); virtual void V18(); virtual void V1C();
    virtual void V20(); virtual void V24(); virtual void V28(); virtual void V2C();
    virtual void V30(); virtual void V34(); virtual void V38(); virtual void V3C();
    virtual void V40(); virtual void V44(); virtual void V48(); virtual void V4C();
    virtual void V50(); virtual void V54(); virtual void V58(); virtual void V5C();
    virtual void V60(); virtual void V64(); virtual void V68(); virtual void V6C();
    virtual Gob* SpawnRoom(char* szName, const Vector& vPosition, const Quaternion& qOrientation);  // +0x70
    virtual void V74(); virtual void V78(); virtual void V7C();
    virtual void V80(); virtual void V84(); virtual void V88(); virtual void V8C();
    virtual void V90(); virtual void V94(); virtual void V98(); virtual void V9C();
    virtual void VA0(); virtual void VA4(); virtual void VA8(); virtual void VAC();
    virtual void VB0(); virtual void VB4(); virtual void VB8(); virtual void VBC();
    virtual void VC0(); virtual void VC4(); virtual void VC8(); virtual void VCC();
    virtual void VD0(); virtual void VD4(); virtual void VD8(); virtual void VDC();
    virtual void VE0(); virtual void VE4(); virtual void VE8(); virtual void VEC();
    virtual void VF0(); virtual void VF4(); virtual void VF8(); virtual void VFC();
    virtual void V100(); virtual void V104(); virtual void V108(); virtual void V10C();
    virtual void V110(); virtual void V114(); virtual void V118(); virtual void V11C();
    virtual void V120(); virtual void V124();
    virtual void AddFacesToBins(PartAABB* pPart, const Vector& vMin,
                                CExoArrayList<TriangleBinList>& lBins);         // +0x128

    char pad_0x4[0x80 - 0x4];
    LightManager* m_pLights;                // +0x80
    char pad_0x84[0xc8 - 0x84];
    CExoArrayList<CAurRoom*> m_lRooms;      // +0xc8
};

// NONMATCH: first draft, same size as the original (2288 B); not yet tuned.
// FUNCTION: SWKOTOR 0x00456F30
Gob* Scene::SpawnRoom(char* szName, const Vector& vPosition, const Quaternion& qOrientation)
{
    AurTextureSetModeStatic();
    Model* pModel = FindModel(szName);
    if (!pModel)
        return 0;
    pModel->m_nFlags |= 1;
    Gob* pGob = 0;
    CAurRoom* pRoom = new CAurRoom(szName);
    nodedata* pData = new nodedata;
    bool bGrass = false;
    Vector vGrassMin(1000000.0f, 1000000.0f, 1000000.0f);
    Vector vGrassMax(-1000000.0f, -1000000.0f, -1000000.0f);
    char szNode[0x40];
    strcpy(szNode, pModel->m_szName);
    strcat(szNode, "a");
    Part* pTree;
    if (findnode(pModel->m_pRoot, szNode)) {
        pGob = NewCAurObject(szName, "", 0, 0);
        pTree = pGob->m_pRootPart;
        pGob->m_vPosition = vPosition;
        pGob->m_qOrientation = qOrientation;
        pGob->AttachToScene(this);
        pRoom->m_pGob = pGob;
    } else {
        pTree = CreateInstanceTree(pModel->m_pRoot, 0);
        char szPrefix[0x20];
        sprintf(szPrefix, "%s", szName);
        PartInitMaterial(0, pTree, szPrefix);
    }
    CExoArrayList<Part*> lParts;
    CExoArrayList<PartAABB*> lGrass;
    CollapsePartTree(pGob, pTree, &lParts, vPosition, qOrientation, this, pRoom);
    Vector& vRoomMax = pRoom->m_vMax;
    vRoomMax = Vector0(-100000.0f, -100000.0f, -100000.0f);
    Vector& vRoomMin = pRoom->m_vMin;
    vRoomMin = Vector0(100000.0f, 100000.0f, 100000.0f);
    for (int i = 0; i < lParts.num; i++) {
        PartTriMesh* pMesh = lParts[i]->AsTriMesh();
        PartLight* pLight = lParts[i]->AsLight();
        PartEmitter* pEmitter = lParts[i]->AsEmitter();
        PartAABB* pAABB = lParts[i]->AsAABB();
        if (pMesh) {
            if (!pMesh->AsSkin()) {
                ComputePartBBox(pMesh);
                if (!pMesh->m_pNode->AsMdlNodeTriMesh()->m_bNoBounds) {
                    vRoomMax = Max(vRoomMax, pMesh->m_vMax);
                    vRoomMin = Min(vRoomMin, pMesh->m_vMin);
                }
            }
            pModel->AddRef(0);
            pData->m_lMeshes.Add(pMesh);
            if (pAABB) {
                pData->m_pAABB = pAABB;
                lGrass.Add(pAABB);
                vGrassMax = Max(vGrassMax, pAABB->m_vMax);
                vGrassMin = Min(vGrassMin, pAABB->m_vMin);
                bGrass = true;
            }
        } else if (pLight) {
            m_pLights->AddLight(pLight);
            pData->m_lLights.Add(pLight);
            if (pLight->m_n70)
                SceneBSPAddPart(this, pLight, 0);
            pModel->AddRef(0);
        } else if (pEmitter) {
            pData->m_lEmitters.Add(pEmitter);
            pModel->AddRef(0);
        } else {
            if (lParts[i])
                delete lParts[i];
        }
    }
    if (lGrass.num > 0) {
        CExoArrayList<TriangleBinList> lBins;
        InitGrassBins(vGrassMin, vGrassMax, &lBins);
        int nGrass = lGrass.num;
        for (int j = 0; j < nGrass; j++)
            AddFacesToBins(lGrass[j], vGrassMin, lBins);
        for (int x = 0; x < lBins.num; x++) {
            for (int y = 0; y < lBins[x].num; y++) {
                if (lBins[x][y]) {
                    lBins[x][y]->FinishGeometry();
                    pData->m_lBins.AddCopy(lBins[x][y]);
                }
            }
        }
    }
    pRoom->m_pData = pData;
    m_lRooms.AddCopy(pRoom);
    AurTextureSetModeDynamic();
    if (!bGrass) {
        vRoomMax = Vector0(-100000.0f, -100000.0f, -100000.0f);
        vRoomMin = Vector0(100000.0f, 100000.0f, 100000.0f);
    }
    return pGob;
}
