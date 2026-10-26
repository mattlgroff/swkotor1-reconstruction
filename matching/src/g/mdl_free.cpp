// Destructor callbacks stored into relocated binary trees (names not
// recorded; behavioural reference: src/units/0661/1c3a6b37f5-model_binary.cpp).
class MdlNode {
public:
    void InternalDestructor();          // 0x004488f0
};

class MaxTree {
public:
    char pad00[0x28];
    MdlNode* m_pRoot;                   // +0x28
};

class Model : public MaxTree {
public:
    void InternalDestructor();          // 0x00449f60
};

// FUNCTION: SWKOTOR 0x0044a8e0
void MaxTreeDestroyNodes(MaxTree* pTree)
{
    if (pTree->m_pRoot)
        pTree->m_pRoot->InternalDestructor();
}

// FUNCTION: SWKOTOR 0x0044aa60
void ModelDestroy(Model* pModel)
{
    pModel->InternalDestructor();
}
