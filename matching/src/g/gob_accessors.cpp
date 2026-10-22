// Gob's small virtual accessors (vtable 0x741268). Most are hidden
// functions between the reverse/functions.csv rows.
#include "g/gob.h"

// FUNCTION: SWKOTOR 0x0043e830
void Gob::Show() { m_b10 = 1; }

// FUNCTION: SWKOTOR 0x0043e840
void Gob::Hide() { m_b10 = 0; }

// FUNCTION: SWKOTOR 0x0043e850
unsigned char Gob::GetFlag10() { return m_b10; }

// FUNCTION: SWKOTOR 0x0043e860
void Gob::SetFlag12() { m_b12 = 1; }

// FUNCTION: SWKOTOR 0x0043e870
void Gob::ClearFlag12() { m_b12 = 0; }

// FUNCTION: SWKOTOR 0x0043e880
void Gob::SetFlag14() { m_b14 = 1; }

// FUNCTION: SWKOTOR 0x0043e890
void Gob::ClearFlag14() { m_b14 = 0; }

// FUNCTION: SWKOTOR 0x0043e8b0
unsigned char Gob::GetFlag12() { return m_b12; }

// FUNCTION: SWKOTOR 0x0043e8c0
void Gob::GetCachedBoundingBox(Vector* pMin, Vector* pMax)
{
    *pMin = m_vBoxMin;
    *pMax = m_vBoxMax;
}

// FUNCTION: SWKOTOR 0x0043e900
void Gob::SetFlag11()
{
    m_b11 = 1;
    DoEvent("SetFlag11", 0);
}

// FUNCTION: SWKOTOR 0x0043e920
void Gob::ClearFlag11() { m_b11 = 0; }

// FUNCTION: SWKOTOR 0x0043edd0
void Gob::SetFog(unsigned char b, int a, int b2, int c)
{
    m_bFog = b;
    m_nFog15c = a;
    m_nFog158 = b2;
    m_nFog160 = c;
}

// FUNCTION: SWKOTOR 0x0043ee00
void Gob::SetSelfIllum(const Vector& v)
{
    m_bSelfIllum = 1;
    m_vSelfIllum = v;
}

// FUNCTION: SWKOTOR 0x0043ee30
void Gob::ClearSelfIllum() { m_bSelfIllum = 0; }

// FUNCTION: SWKOTOR 0x0043ee50
void Gob::SetColorShift(AurColor c, int nMode, int bPropagate)
{
    m_cColorShift = c;
    m_nColorShiftMode = nMode;
    m_bColorShift = 1;
    if (bPropagate)
        DoEvent("ColorShift", 0);
}

// FUNCTION: SWKOTOR 0x0043eea0
void Gob::SetIllumination(AurColor c, int bPropagate)
{
    m_cIllumination = c;
    m_bIllumination = 1;
    if (bPropagate)
        DoEvent("Illumination", 0);
}

// FUNCTION: SWKOTOR 0x0043ef80
void Gob::GetBoundingBox(Vector* pMin, Vector* pMax)
{
    UpdateBoundingBox();
    *pMin = m_vBoxMin;
    *pMax = m_vBoxMax;
}

// FUNCTION: SWKOTOR 0x0043efd0
void Gob::GetBoundingSphere(Vector* pCenter, float* pRadius)
{
    UpdateBoundingSphere();
    *pCenter = m_vCenter;
    *pRadius = m_fRadius;
}

// FUNCTION: SWKOTOR 0x0043f0c0
Vector Gob::SetPosition(Vector v)
{
    m_vPosition = v;
    return v;
}

// FUNCTION: SWKOTOR 0x0043f0f0
Quaternion Gob::SetOrientation(Quaternion q)
{
    m_qOrientation = q;
    return q;
}

// FUNCTION: SWKOTOR 0x0043f130
GobAttachment* Gob::GetAttachment(int nId)
{
    if (nId != -1) {
        if (!m_pAttachment || m_pAttachment->GetId() != nId)
            return 0;
    }
    return m_pAttachment;
}

// FUNCTION: SWKOTOR 0x0043f1c0
char* Gob::GetModelName()
{
    if (!m_pModel)
        return 0;
    return m_pModel->m_szName;
}

// FUNCTION: SWKOTOR 0x0043f1d0
void Gob::SetAttachment(GobAttachment* p)
{
    GobAttachment* pOld = m_pAttachment;
    m_pAttachment = p;
    if (pOld)
        delete pOld;
}

// FUNCTION: SWKOTOR 0x0043f200
void Gob::SetScene(Scene* pScene)
{
    if (m_pScene == pScene)
        return;
    if (m_pScene)
        m_pScene->RemoveGob(this);
    m_pScene = pScene;
    if (pScene)
        pScene->AddGob(this);
    DoEvent("SetScene", 0);
}

// FUNCTION: SWKOTOR 0x0043f250
void Gob::CopyUserData(Gob* pFrom)
{
    m_nUserData = pFrom->m_nUserData;
    m_nUserData2 = pFrom->m_nUserData2;
}

// FUNCTION: SWKOTOR 0x0043f5a0
void Gob::SetAttachmentLight(int n)
{
    if (m_pAttachment)
        m_pAttachment->SetLight(n);
}

// FUNCTION: SWKOTOR 0x00440870
void Gob::FogOff() { PartSetFog(m_pRoot, 0); }

// FUNCTION: SWKOTOR 0x00440880
void Gob::FogOn() { PartSetFog(m_pRoot, 1); }

// FUNCTION: SWKOTOR 0x004404b0
void Gob::SetImposter(int a, int b)
{
    m_n34 = a;
    m_n30 = SetUpImposters(a, b);
}

// FUNCTION: SWKOTOR 0x00449970
char* Gob::GetName() { return m_szName; }

// FUNCTION: SWKOTOR 0x00449980
Vector Gob::GetPosition() { return m_vPosition; }

// FUNCTION: SWKOTOR 0x004499a0
Quaternion Gob::GetOrientation() { return m_qOrientation; }

// FUNCTION: SWKOTOR 0x004499d0
Vector Gob::GetVelocity() { return m_vVelocity; }

// FUNCTION: SWKOTOR 0x00449a00
int Gob::GetUserData() { return m_nUserData; }

// FUNCTION: SWKOTOR 0x00449a10
void Gob::SetUserData(int n) { m_nUserData = n; }

// FUNCTION: SWKOTOR 0x00449a20
int Gob::GetUserData2() { return m_nUserData2; }

// FUNCTION: SWKOTOR 0x00449a30
void Gob::SetUserData2(int n) { m_nUserData2 = n; }

// FUNCTION: SWKOTOR 0x00449a40
void Gob::SetNoClip(unsigned char b) { m_bNoClip = b; }

// FUNCTION: SWKOTOR 0x00449a50
void Gob::ClearFadeFlag() { m_nFade = 0; }

// FUNCTION: SWKOTOR 0x00449a60
void Gob::SetFadeFlag() { m_nFade = 1; }

// FUNCTION: SWKOTOR 0x00449a70
unsigned char Gob::GetFlag17e() { return m_b17e; }

// FUNCTION: SWKOTOR 0x00449a80
unsigned char Gob::GetFlag11() { return m_b11; }

// FUNCTION: SWKOTOR 0x00449a90
void Gob::SetModelFlag4()
{
    if (m_pModel)
        m_pModel->m_nFlags |= 4;
}

// FUNCTION: SWKOTOR 0x00449aa0
void Gob::SetFlag154(unsigned char b) { m_b154 = b; }

// FUNCTION: SWKOTOR 0x00449ab0
void Gob::SetFlag155(unsigned char b) { m_b155 = b; }

// FUNCTION: SWKOTOR 0x00449ac0
unsigned char Gob::GetFlag155() { return m_b155; }

// FUNCTION: SWKOTOR 0x00449ad0
void Gob::SetFloat180(float f) { m_f180 = f; }

// FUNCTION: SWKOTOR 0x00449ae0
float Gob::GetFloat180() { return m_f180; }

// FUNCTION: SWKOTOR 0x00449af0
void Gob::SetHidden() { m_bHidden = 1; }

// FUNCTION: SWKOTOR 0x00449b00
void Gob::ClearHidden() { m_bHidden = 0; }
