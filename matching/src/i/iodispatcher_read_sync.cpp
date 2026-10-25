#include <stdio.h>

class MaxTree;

class SyncInput {
public:
    MaxTree* Read(char* szName);
    MaxTree* Read(FILE* pMdl, FILE* pMdx);
};

struct SyncInputStorage {
    char pad_0x00[0x0c];
    int zero0c;
};

class IODispatcher {
public:
    MaxTree* ReadSync(char* szName);
    MaxTree* ReadSync(FILE* pMdl, FILE* pMdx);
};

// FUNCTION: SWKOTOR 0x004A15B0 SYMBOL: ?ReadSync@IODispatcher@@QAEPAVMaxTree@@PAD@Z
MaxTree* IODispatcher::ReadSync(char* szName)
{
    SyncInputStorage input;
    input.zero0c = 0;
    return ((SyncInput*)&input)->Read(szName);
}

// FUNCTION: SWKOTOR 0x004A15D0 SYMBOL: ?ReadSync@IODispatcher@@QAEPAVMaxTree@@PAU_iobuf@@0@Z
MaxTree* IODispatcher::ReadSync(FILE* pMdl, FILE* pMdx)
{
    SyncInputStorage input;
    input.zero0c = 0;
    return ((SyncInput*)&input)->Read(pMdl, pMdx);
}
