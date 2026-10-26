// CSWCTrigger lifecycle, model, animation and Aurora trigger-part methods.
#include "k/sw_c_trigger.h"

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
    void Allocate(int);                                              // 0x005E03C0 (unsigned long)
    void Remove(T);                                                  // 0x0066DCC0 (unsigned long)
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num++] = t;
    }
    int Contains(T t)
    {
        int found = 0;
        for (int i = 0; i < num; i++) {
            if (element[i] == t)
                found++;
        }
        return found;
    }
};

class CSWCArea {
public:
    char pad_0x0[0x11c];
    CExoArrayList<unsigned long> m_lTriggerIds;                      // +0x11C
    char pad_0x128[0x184 - 0x128];
    Scene* m_pScene;                                                 // +0x184
};

extern unsigned char g_nObjectTypeTrigger;                           // 0x00746636

// 0x00670B20 is a folded "delete if non-null" body (also matched as
// ExplosionDoneCallback); the trigger destructor passes only the part.
// 16-byte zeroed colour record passed to the trigger-part factory.
struct TriggerColor {
    float r, g, b, a;
    TriggerColor() { r = 0.0f; g = 0.0f; b = 0.0f; a = 0.0f; }
};

PartTrigger* __cdecl AurPartTriggerCreate(Scene*, Vector*, int, int, TriggerColor*,
                                          CSWCTrigger*);             // 0x0049E630, descriptive
void __cdecl AurPartTriggerDelete(Scene*, PartTrigger*, unsigned char); // 0x0049D7D0
void __cdecl AurPartTriggerSetColor(PartTrigger*, Vector*, float, unsigned char, float,
                                    unsigned char);                  // 0x0049D9B0
void __cdecl AurPartTriggerSetHighlight(PartTrigger*, unsigned char, float,
                                        unsigned char);              // 0x0049E610
void __cdecl AurPartTriggerFree(PartTrigger*);                       // 0x00670B20, descriptive

// FUNCTION: SWKOTOR 0x006911E0
CSWCTrigger::CSWCTrigger() : CSWCObject(g_nObjectTypeTrigger)
{
    unsigned long day, tick;
    CClientExoApp* app = g_pAppManager->m_pClientExoApp;
    app->GetWorldTimer()->GetWorldTime(&day, &tick);
    m_nDay = day;
    m_nTick = tick;
    m_nVertices = 0;
    m_pVertices = 0;
    m_bAreaTransition = 0;
    m_bHostile = 0;
    m_bDisplayed = 0;
    m_fHighlightWidth = 0;
    m_nFC = 0;
    m_pAurPartTrigger = 0;
    m_n148 = 0xff;
}

// Not in the function table: starts after the constructor's padding.
// FUNCTION: SWKOTOR 0x006912B0
CExoString CSWCTrigger::GetName()
{
    return m_sName;
}

// Not in the function table.
// FUNCTION: SWKOTOR 0x006912E0
unsigned long CSWCTrigger::GetTransitionMode()
{
    return m_bTransitionMode;
}

// FUNCTION: SWKOTOR 0x006912F0
CSWCTrigger::~CSWCTrigger()
{
    if (m_pModel) {
        m_pModel->UnloadModel();
        delete m_pModel;
        m_pModel = 0;
    }
    if (m_pVertices) {
        delete m_pVertices;
        m_pVertices = 0;
    }
    if (m_pAurPartTrigger) {
        AurPartTriggerFree(m_pAurPartTrigger);
        m_pAurPartTrigger = 0;
    }
}

// FUNCTION: SWKOTOR 0x006910D0
int CSWCTrigger::LoadModel(const CResRef& res, unsigned char part, int)
{
    if (!m_pModel) {
        m_pModel = new CSWCAnimBaseTrigger;
        if (!m_pModel)
            return 0;
    }
    if (!m_pModel->LoadModel(res, part))
        return 0;
    CAurObject* model = m_pModel->GetModel(part);
    if (model) {
        model->SetClientObject(this);
        model->SetObjectType(g_nObjectTypeTrigger);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x006911B0
void CSWCTrigger::PlayAnimation(unsigned long animation, void* data, int)
{
    unsigned long duration = (unsigned long)GetAnimationLength(animation);
    AddFireForgetAnimation((unsigned short)animation, duration, 1.0f, data, 0);
}

// FUNCTION: SWKOTOR 0x00691560
void CSWCTrigger::CalculatePosition()
{
    Vector vMin = m_pVertices[0];
    Vector vMax = m_pVertices[0];
    for (int i = 1; i < m_nVertices; i++) {
        if (m_pVertices[i].x < vMin.x)
            vMin.x = m_pVertices[i].x;
        if (m_pVertices[i].y < vMin.y)
            vMin.y = m_pVertices[i].y;
        if (m_pVertices[i].z < vMin.z)
            vMin.z = m_pVertices[i].z;
        if (m_pVertices[i].x > vMax.x)
            vMax.x = m_pVertices[i].x;
        if (m_pVertices[i].y > vMax.y)
            vMax.y = m_pVertices[i].y;
        if (m_pVertices[i].z > vMax.z)
            vMax.z = m_pVertices[i].z;
    }
    m_vPosition = (vMax - vMin) * 0.5f + vMin;
}

// FUNCTION: SWKOTOR 0x00691970
void CSWCTrigger::UpdateAnimation()
{
    if (AnimationFireAndForget(m_nAnimation) == 1) {
        m_nFireForgetTime -= m_nElapsed;
        if (m_nFireForgetTime < 0) {
            m_nE4 = 1;
            m_n13C = 500;
            m_nFireForgetTime = 0;
            return;
        }
    }
    if (AnimationLooping(m_nAnimation) == 1) {
        unsigned short animation;
        float speed;
        unsigned long duration;
        void* data;
        if (GetTopFireForgetAnimation(&animation, &speed, &duration, &data) == 1) {
            m_nAnimation = animation;
            m_pModel->PlayAnim(animation, speed, 1, 0);
            m_nFireForgetTime = duration;
            RemoveTopFireForgetAnimation();
        }
        if (data)
            delete data;
    }
}

// FUNCTION: SWKOTOR 0x00691A30
int CSWCTrigger::IntersectSegment(const Vector& vStart, const Vector& vEnd, Vector* pResult)
{
    CSWSArea* area = m_pArea;
    if (area) {
        for (int i = 0; i < m_nVertices; i++) {
            int next = (i != m_nVertices - 1) ? i + 1 : 0;
            float x, y;
            if (area->LineSegmentsIntersect(vStart.x, vStart.y, vEnd.x, vEnd.y,
                                            m_pVertices[i].x, m_pVertices[i].y,
                                            m_pVertices[next].x, m_pVertices[next].y,
                                            &x, &y) == 1) {
                pResult->x = x;
                pResult->y = y;
                pResult->z = 0.0f;
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00691CC0
void CSWCTrigger::AddToRenderList(int nColor)
{
    CSWCArea* area = g_pAppManager->m_pClientExoApp->GetModule()->GetArea();
    area->m_lTriggerIds.Add(m_oid);
    Scene* scene = g_pAppManager->m_pClientExoApp->GetModule()->GetArea()->m_pScene;
    TriggerColor color;
    m_pAurPartTrigger = AurPartTriggerCreate(scene, m_pVertices, m_nVertices, 1, &color, this);
    UpdateTriggerColor(nColor);
}

// FUNCTION: SWKOTOR 0x00691D80
void CSWCTrigger::RemoveFromRenderList()
{
    CSWCArea* area = g_pAppManager->m_pClientExoApp->GetModule()->GetArea();
    unsigned long id = m_oid;
    if (area->m_lTriggerIds.Contains(id)) {
        area->m_lTriggerIds.Remove(id);
        AurPartTriggerDelete(g_pAppManager->m_pClientExoApp->GetModule()->GetArea()->m_pScene,
                             m_pAurPartTrigger, 0);
    }
}

// FUNCTION: SWKOTOR 0x00691E00
void CSWCTrigger::RefreshTriggerColor(int bHighlight)
{
    Vector color;
    float alpha;
    color.x = 0.0f;
    color.y = 0.0f;
    color.z = 0.0f;
    if (m_bAreaTransition)
        return;
    if (m_bHostile) {
        if (!m_pAurPartTrigger)
            AddToRenderList(2);
        color.x = 0.0f;
        color.y = 0.0f;
        color.z = 1.0f;
        alpha = 0.25f;
    } else {
        if (!m_pAurPartTrigger)
            AddToRenderList(1);
        color.x = 1.0f;
        color.y = 1.0f;
        color.z = 0.0f;
        alpha = 0.35f;
    }
    if (!bHighlight && !m_bDisplayed)
        alpha = 0.0f;
    AurPartTriggerSetColor(m_pAurPartTrigger, &color, alpha, 0, 0.0f, 0);
    AurPartTriggerSetHighlight(m_pAurPartTrigger, 1, GetHighlightWidth(), 1);
}

// FUNCTION: SWKOTOR 0x00691CA0 SYMBOL: ??_GCSWCTrigger@@UAEPAXI@Z
// CSWCTrigger::`scalar deleting destructor' (compiler generated).
