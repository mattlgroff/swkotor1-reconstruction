// Animation (MaxTree subclass) ctor/dtor, CustomAnimation /
// LookAtAnimation destructors (0x00484E00 .. 0x00487170).
// Self-contained: AnimTreeRun here is the 0x00743234 base of
// LookAtAnimation (anim.cpp keeps its older split shapes).

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList();
    ~CExoArrayList() { if (element) delete[] element; }
    void Allocate(int n);
    void DelIndex(int i)
    {
        num--;
        for (; i < num; i++)
            element[i] = element[i + 1];
    }
    void Remove(T value) throw();   // 0x00445080
    void SetSize(int n)
    {
        if (n == 0) {
            if (element) {
                delete[] element;
                element = 0;
            }
            array_size = 0;
        } else {
            Allocate(n);
        }
        num = n;
    }
};

struct AnimEvent {
    float fTime;                // +0x00
    char szName[0x20];          // +0x04
};

class MaxTree;
typedef void (*MaxTreeFunc)(MaxTree* pTree, void* pData);

class MaxTree {
public:
    MaxTree(char* szName);                  // 0x0044A900
    ~MaxTree();                             // 0x0043F730
    static void operator delete(void* p);   // 0x0044AEC0

    MaxTreeFunc m_pfnDestroy;               // +0x00
    MaxTreeFunc m_pfnProcess;               // +0x04
    char m_szName[0x20];                    // +0x08
    char pad_0x28[0x24];
    unsigned char m_nType;                  // +0x4c
    char pad_0x4d[3];
};

extern const unsigned char g_nAnimationTreeType;    // 0x00740A12
void AnimationProcess(MaxTree* pTree, void* pData);  // 0x00481880

class Animation : public MaxTree {
public:
    void Process(void* pData);              // 0x0046A410
    float m_fLength;                        // +0x50
    float m_fTransition;                    // +0x54
    unsigned char m_b58;                    // +0x58
    char pad_0x59[0x1f];
    CExoArrayList<AnimEvent> m_lEvents;     // +0x78
    int m_n84;                              // +0x84

    Animation(char* szName);
    ~Animation();
};

// FUNCTION: SWKOTOR 0x004850A0
Animation::~Animation()
{
    if (m_nType) {
        m_nType = 0;
        if (m_lEvents.num)
            m_lEvents.SetSize(0);
    }
}

// FUNCTION: SWKOTOR 0x004851E0
Animation::Animation(char* szName) : MaxTree(szName)
{
    m_n84 = 0;
    m_nType = g_nAnimationTreeType;
    m_b58 = 0;
    m_fLength = 1.0f;
    m_fTransition = 0.25f;
    m_pfnProcess = AnimationProcess;
}

template <class T>
CExoArrayList<T>::CExoArrayList()
{
    num = 0;
    array_size = 0;
    element = 0;
}

class AnimRun;
class Model;

class Model {
public:
    void* m_p00;
    CExoArrayList<Model**> m_lWatchers;     // +0x04
};

class ModelRef {
public:
    Model* m_p;
    ~ModelRef()
    {
        if (m_p)
            m_p->m_lWatchers.Remove(&m_p);
    }
};

class Gob {
public:
    char pad_0x0[0xb0];
    CExoArrayList<AnimRun*> m_lRuns;        // +0xb0
};

class AnimTreeRun {
public:
    virtual ~AnimTreeRun()
    {
        if (m_pTree)
            delete m_pTree;
    }
    unsigned char m_bActive;                // +0x04
    MaxTree* m_pTree;                       // +0x08
    AnimRun* m_pRun;                        // +0x0c
};

// FUNCTION: SWKOTOR 0x00484E00 SYMBOL: ??_GAnimTreeRun@@UAEPAXI@Z

class LookAtAnimation : public AnimTreeRun {
public:
    virtual ~LookAtAnimation();
    ModelRef m_rModel;                      // +0x10
    Gob* m_pGob;                            // +0x14
};

// FUNCTION: SWKOTOR 0x00486090
LookAtAnimation::~LookAtAnimation()
{
    for (int i = m_pGob->m_lRuns.num; i != 0; ) {
        i--;
        if (m_pGob->m_lRuns.element[i] == m_pRun)
            m_pGob->m_lRuns.DelIndex(i);
    }
}

// FUNCTION: SWKOTOR 0x00487170 SYMBOL: ??_GLookAtAnimation@@UAEPAXI@Z

// Make sure the AnimTreeRun vtable (and its scalar deleting dtor) is emitted.
AnimTreeRun* NewAnimTreeRun() { return new AnimTreeRun; }

// FUNCTION: SWKOTOR 0x00481880
void AnimationProcess(MaxTree* pTree, void* pData)
{
    ((Animation*)pTree)->Process(pData);
}
