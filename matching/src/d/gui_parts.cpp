// embedded GUI parts (borders, texts, images, scenes) and their
// parameter blocks. Semantics from src/batches/b0041_gui_params.cpp.
#include "d/gui_parts.h"

extern "C" char* __cdecl strcpy(char* dst, const char* src);
extern "C" int __cdecl _stricmp(const char* a, const char* b);
#pragma intrinsic(strcpy)

void operator delete(void* p);

#pragma auto_inline(off)

// FUNCTION: SWKOTOR 0x00414ae0
void CSWGuiBorderParams::SetCornerImage(const CResRef& cResRef, int bForce)
{
    if (cResRef != m_cCorner || bForce) {
        m_cCorner = cResRef;
        if (m_pBorder) {
            if (m_pBorder->m_pCorner) {
                m_pBorder->m_pCorner->Release(1);
                m_pBorder->m_pCorner = 0;
            }
            if (m_cCorner.IsValid()) {
                char szName[17];
                m_cCorner.CopyToString(szName);
                m_pBorder->m_pCorner = NewCAurGUIImage(szName);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00414b70
void CSWGuiBorderParams::SetEdgeImage(const CResRef& cResRef, int bForce)
{
    if (cResRef != m_cEdge || bForce) {
        m_cEdge = cResRef;
        if (m_pBorder) {
            if (m_pBorder->m_pEdge) {
                m_pBorder->m_pEdge->Release(1);
                m_pBorder->m_pEdge = 0;
            }
            if (m_cEdge.IsValid()) {
                char szName[17];
                m_cEdge.CopyToString(szName);
                m_pBorder->m_pEdge = NewCAurGUIImage(szName);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00414c00
void CSWGuiBorderParams::SetFillImage(const CResRef& cResRef, int bForce)
{
    if (cResRef != m_cFill || bForce) {
        m_cFill = cResRef;
        if (m_pBorder) {
            if (m_pBorder->m_pFill) {
                m_pBorder->m_pFill->Release(1);
                m_pBorder->m_pFill = 0;
            }
            if (m_cFill.IsValid()) {
                char szName[17];
                m_cFill.CopyToString(szName);
                m_pBorder->m_pFill = NewCAurGUIImage(szName);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00414c90
void CSWGuiBorderParams::SetBorder(CSWGuiBorder* pBorder)
{
    m_pBorder = pBorder;
    if (pBorder) {
        SetCornerImage(m_cCorner, 1);
        SetEdgeImage(m_cEdge, 1);
        SetFillImage(m_cFill, 1);
    }
}

// FUNCTION: SWKOTOR 0x00414cd0
int CSWGuiBorderParams::GetCornerSize()
{
    int nSize = m_nCornerSize;
    if (nSize) {
        return nSize;
    }
    nSize = 0;
    if (m_pBorder) {
        nSize = m_pBorder->GetCornerHeight();
    }
    return nSize;
}

// FUNCTION: SWKOTOR 0x00414d00
void CSWGuiBorderParams::SetFlags(int b2, int b3, int b4)
{
    m_b2 = b2;
    m_b3 = b3;
    m_b4 = b4;
}

// FUNCTION: SWKOTOR 0x00414d30
CSWGuiImageParams::CSWGuiImageParams(const CResRef& cResRef, int b1, int n2, float f10, int n6,
                                     float f14, const Vector& vColor)
{
    m_cResRef = cResRef;
    m_f10 = f10;
    m_f14 = f14;
    m_b0 = 0;
    m_b1 = b1;
    m_n2 = n2;
    m_n6 = n6;
    m_vColor = vColor;
    m_pImage = 0;
}

// FUNCTION: SWKOTOR 0x00414e10
void CSWGuiTextParams::SetColor(const Vector& vColor)
{
    m_vColor = vColor;
    if (m_pText && m_pText->m_pString) {
        m_pText->m_pString->SetColor(&m_vColor);
    }
}

// FUNCTION: SWKOTOR 0x00414e50
void CSWGuiTextParams::SetDefaultColor(const Vector& vColor)
{
    m_vDefaultColor = vColor;
    SetColor(vColor);
}

// FUNCTION: SWKOTOR 0x00414e80
void CSWGuiTextParams::RestoreDefaultColor()
{
    SetColor(m_vDefaultColor);
}

// FUNCTION: SWKOTOR 0x00414e90
void CSWGuiText::Rebuild()
{
    if (m_pString && m_extent.height && m_extent.width) {
        m_pString->Rebuild(-1);
    }
}

// FUNCTION: SWKOTOR 0x00414eb0
int CSWGuiText::GetHeight()
{
    if (m_pString) {
        if (m_pString->GetFontHeight() < 16) {
            return m_pString->GetLineCount(-1) + 1;
        }
        return m_pString->GetLineCount(-1);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00414ee0
int CSWGuiText::GetFontHeight()
{
    if (m_pString) {
        if (m_pString->GetFontHeight() < 16) {
            return m_pString->GetFontHeight() + 1;
        }
        return m_pString->GetFontHeight();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00414f10
CSWGuiExtent CSWGuiText::GetTextExtent()
{
    CSWGuiExtent r;
    int nLines = m_pString->GetLineCount(m_nMaxWidth);
    int nWidth = m_nMaxWidth;
    while (nLines == m_pString->GetLineCount(nWidth)) {
        nWidth -= 10;
        if (nWidth <= 0) {
            nWidth = m_nMaxWidth;
            break;
        }
    }
    nWidth += 10;
    if (m_pString->GetFontHeight() < 16) {
        nLines++;
    }
    r.width = nWidth;
    r.height = nLines;
    return r;
}

// FUNCTION: SWKOTOR 0x00415000
CSWGuiBorderParams::CSWGuiBorderParams(const CResRef& cCorner, const CResRef& cEdge,
                                       const CResRef& cFill, int nStyle, int nCornerSize,
                                       float fEdgeScale, const Vector& vColor, int nPulse)
{
    m_cCorner = cCorner;
    m_cEdge = cEdge;
    m_cFill = cFill;
    m_nCornerSize = nCornerSize;
    m_nStyle = nStyle;
    m_fEdgeScale = fEdgeScale;
    m_b2 = 0;
    m_vColor = vColor;
    m_b3 = 0;
    m_b4 = 0;
    m_pBorder = 0;
    m_f08 = 0.0f;
    m_nPulse = nPulse;
}

// FUNCTION: SWKOTOR 0x004150c0
CSWGuiBorderParams& CSWGuiBorderParams::operator=(const CSWGuiBorderParams& p)
{
    m_cCorner = p.m_cCorner;
    m_cEdge = p.m_cEdge;
    m_cFill = p.m_cFill;
    m_nStyle = p.m_nStyle;
    m_f08 = p.GetF08();
    m_nCornerSize = ((CSWGuiBorderParams&)p).GetCornerSize();
    m_fEdgeScale = p.GetEdgeScale();
    m_b2 = p.m_b2;
    m_vColor = p.m_vColor;
    m_b3 = p.m_b3;
    m_b4 = p.m_b4;
    m_nPulse = p.m_nPulse;
    SetBorder(m_pBorder);
    return *this;
}

// FUNCTION: SWKOTOR 0x00415190
void CSWGuiBorder::Initialize(const CSWGuiExtent& extent, const CSWGuiBorderParams& params)
{
    m_extent = extent;
    m_cParams = params;
}

// FUNCTION: SWKOTOR 0x004151c0
CSWGuiBorder::~CSWGuiBorder()
{
    if (m_pCorner) {
        m_pCorner->Release(1);
        m_pCorner = 0;
    }
    if (m_pEdge) {
        m_pEdge->Release(1);
        m_pEdge = 0;
    }
    if (m_pFill) {
        m_pFill->Release(1);
        m_pFill = 0;
    }
}

// FUNCTION: SWKOTOR 0x00415900
void CSWGuiImageParams::SetImage(const CResRef& cResRef, int bForce)
{
    if (cResRef != m_cResRef || bForce) {
        m_cResRef = cResRef;
        if (m_pImage) {
            if (m_pImage->m_pImage) {
                m_pImage->m_pImage->Release(1);
                m_pImage->m_pImage = 0;
            }
            if (m_cResRef.IsValid()) {
                char szName[17];
                m_cResRef.CopyToString(szName);
                m_pImage->m_pImage = NewCAurGUIImage(szName);
                if (m_pImage->m_pImage) {
                    m_pImage->SetExtent(m_pImage->m_extent);
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00415990
void CSWGuiImageParams::SetN6(int n)
{
    m_n6 = n;
    if (m_pImage) {
        m_pImage->SetExtent(m_pImage->m_extent);
    }
}

// FUNCTION: SWKOTOR 0x004159c0
void CSWGuiImageParams::SetB1(int b)
{
    m_b1 = b;
    if (m_pImage) {
        m_pImage->SetExtent(m_pImage->m_extent);
    }
}

// FUNCTION: SWKOTOR 0x004159f0
void CSWGuiImageParams::SetOwner(CSWGuiImage* pImage)
{
    m_pImage = pImage;
    if (pImage) {
        SetImage(m_cResRef, 1);
        SetN6(m_n6);
        SetB1(m_b1);
    }
}

// FUNCTION: SWKOTOR 0x00415a30
CSWGuiImage::CSWGuiImage()
    : m_cParams(CResRef(), 0, 0, 0.0f, 0x12, 1.0f, Vector(1.0f, 1.0f, 1.0f))
{
    m_pImage = 0;
    m_cParams.SetOwner(this);
}

// FUNCTION: SWKOTOR 0x00415ad0
CSWGuiImage::~CSWGuiImage()
{
    if (m_pImage) {
        m_pImage->Release(1);
        m_pImage = 0;
    }
}

// FUNCTION: SWKOTOR 0x00415d00
void CSWGuiTextParams::SetFont(const CResRef& cFont)
{
    m_cFont = cFont;
    if (m_pText && m_pText->m_pString && m_cFont.IsValid()) {
        char szFont[20];
        m_cFont.CopyToString(szFont);
        m_pText->m_pString->SetFont(szFont);
        m_pText->Rebuild();
    }
}

// FUNCTION: SWKOTOR 0x00415d60
void CSWGuiTextParams::ApplyFont()
{
    char szFont[20];
    strcpy(szFont, m_szBaseFont);
    g_pGuiManager->GetUpdatedFontName(szFont);
    char szCurrent[20];
    m_cFont.CopyToString(szCurrent);
    if (_stricmp(szFont, szCurrent)) {
        CResRef cFont(szFont);
        SetFont(cFont);
    }
}

// FUNCTION: SWKOTOR 0x00415dd0
void CSWGuiTextParams::SetBaseFont(const CResRef& cFont)
{
    cFont.CopyToString(m_szBaseFont);
    ApplyFont();
}

// FUNCTION: SWKOTOR 0x00415df0
void CSWGuiTextParams::UpdateFont()
{
    ApplyFont();
}

// FUNCTION: SWKOTOR 0x00415e00
void CSWGuiTextParams::SetText(const CExoString& sText)
{
    m_sText = sText;
    m_nStrRef = -1;
    if (m_pText && m_pText->m_pString) {
        m_pText->m_pString->SetText(m_sText.CStr());
        m_pText->Rebuild();
    }
}

// FUNCTION: SWKOTOR 0x00415e50
void CSWGuiTextParams::SetStrRef(int nStrRef)
{
    m_nStrRef = nStrRef;
    if (nStrRef != -1) {
        STR_RES strRes;
        g_pTlkTable->Fetch(m_nStrRef, strRes, g_nGender);
        m_sText = strRes.sString;
        if (m_pText && m_pText->m_pString) {
            m_pText->m_pString->SetText(m_sText.CStr());
            m_pText->Rebuild();
        }
    }
}

// FUNCTION: SWKOTOR 0x00415f20
void CSWGuiTextParams::SetAlignment(int nAlign)
{
    m_nAlign = nAlign;
    if (m_pText && m_pText->m_pString) {
        m_pText->m_pString->SetAlignment(m_nAlign);
        m_pText->Rebuild();
    }
}

// FUNCTION: SWKOTOR 0x00415f70
void CSWGuiTextParams::SetOwner(CSWGuiText* pText)
{
    m_pText = pText;
    if (pText) {
        if (!pText->m_pString) {
            char szFont[20];
            m_cFont.CopyToString(szFont);
            m_pText->m_pString = NewCAurGUIString(m_sText.CStr(), szFont, m_vColor, 1.0f);
        }
        int nStrRef = m_nStrRef;
        ApplyFont();
        SetText(m_sText);
        SetAlignment(m_nAlign);
        SetColor(m_vColor);
        m_nStrRef = nStrRef;
    }
}

// FUNCTION: SWKOTOR 0x00415ff0
CSWGuiText::~CSWGuiText()
{
    if (m_pString) {
        m_pString->Release(1);
    }
}

extern float g_fGuiPulse;                           // 0x0078d3d4

// FUNCTION: SWKOTOR 0x00416240
void CSWGuiText::Draw(float fDelta)
{
    if (m_pString) {
        float fAlpha = m_cParams.m_fAlpha;
        if (m_cParams.m_bHilight) {
            fAlpha = g_fGuiPulse;
        }
        m_pString->SetAlpha(fAlpha);
    }
}

// FUNCTION: SWKOTOR 0x00416280
void CSWGuiText::SetExtent(const CSWGuiExtent& extent)
{
    m_extent = extent;
    if (m_pString) {
        m_pString->SetPosition(extent.left, extent.top);
        m_pString->SetSize(extent.width, extent.height);
        Rebuild();
    }
}

// FUNCTION: SWKOTOR 0x004162e0
void CSWGuiEditText::RemoveLastChar()
{
    CExoString sCaret;
    if (m_sText.GetLength() > 0) {
        m_sText = m_sText.SubString(0, m_sText.GetLength() - 1);
        if (m_bCaret) {
            sCaret.Format("%s_", m_sText.CStr());
            m_cParams.SetText(sCaret);
        }
        else {
            m_cParams.SetText(m_sText);
        }
    }
}

// FUNCTION: SWKOTOR 0x004163a0
void CSWGuiEditText::AddNewChar(const char* sChar)
{
    CExoString sCaret;
    if (m_nMaxLength == -1 || m_sText.GetLength() < m_nMaxLength) {
        m_sText = m_sText + CExoString(sChar);
        if (m_bCaret) {
            sCaret.Format("%s_", m_sText.CStr());
            m_cParams.SetText(sCaret);
        }
        else {
            m_cParams.SetText(m_sText);
        }
    }
}

// FUNCTION: SWKOTOR 0x00416490
void CSWGuiEditText::SetText(const CExoString& sText)
{
    CExoString sCaret;
    m_sText = sText;
    if (m_bCaret) {
        sCaret.Format("%s_", sText.CStr());
        m_cParams.SetText(sCaret);
    }
    else {
        m_cParams.SetText(m_sText);
    }
}

// FUNCTION: SWKOTOR 0x00416530
void CSWGuiEditText::SetCaretVisible(int bVisible)
{
    m_bCaret = bVisible;
    if (bVisible) {
        CExoString sCaret;
        sCaret.Format("%s_", m_sText.CStr());
        m_cParams.SetText(sCaret);
    }
    else {
        m_cParams.SetText(m_sText);
    }
}

// FUNCTION: SWKOTOR 0x00416710
int CSWGuiScene::RemoveModel(int nIndex, int bDelete)
{
    if (nIndex >= 0 && nIndex < m_lstModels.num) {
        CAurObject* pModel = m_lstModels.element[nIndex];
        m_lstModels.DelIndex(nIndex);
        if (bDelete && pModel) {
            pModel->Destroy(1);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00416750
CAurObject* CSWGuiScene::GetModel(int nIndex)
{
    if (nIndex >= 0 && nIndex < m_lstModels.num) {
        return m_lstModels.element[nIndex];
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004167f0
CSWGuiBorder::CSWGuiBorder()
    : m_cParams(CResRef(), CResRef(), CResRef(), 0, 0, 1.0f, Vector(1.0f, 1.0f, 1.0f), 0)
{
    m_pCorner = 0;
    m_pEdge = 0;
    m_pFill = 0;
    m_cParams.SetBorder(this);
}

// FUNCTION: SWKOTOR 0x00416de0
CSWGuiImageParams& CSWGuiImageParams::operator=(const CSWGuiImageParams& p)
{
    m_cResRef = p.m_cResRef;
    m_b1 = p.m_b1;
    m_n2 = p.m_n2;
    m_f10 = p.GetRotation();
    m_n6 = p.m_n6;
    m_f14 = p.GetF14();
    m_b0 = p.m_b0;
    m_vColor = p.m_vColor;
    SetOwner(m_pImage);
    return *this;
}

// FUNCTION: SWKOTOR 0x00417050 SYMBOL: ??0CSWGuiTextParams@@QAE@XZ
CSWGuiTextParams::CSWGuiTextParams()
{
    m_vColor = m_vDefaultColor = Vector(1.0f, 1.0f, 1.0f);
    m_pText = 0;
    m_nStrRef = -1;
    m_fAlpha = 1.0f;
    m_nAlign = 9;
    m_bHilight = 0;
    {
        CResRef cFont("dialogfont16x16");
        SetBaseFont(cFont);
    }
}

// FUNCTION: SWKOTOR 0x00417120 SYMBOL: ??0CSWGuiTextParams@@QAE@ABVCExoString@@ABVCResRef@@ABVVector@@HM@Z
CSWGuiTextParams::CSWGuiTextParams(const CExoString& sText, const CResRef& cFont,
                                   const Vector& vColor, int nAlign, float fAlpha)
{
    m_pText = 0;
    m_sText = sText;
    m_vDefaultColor = vColor;
    SetBaseFont(cFont);
    m_vColor = m_vDefaultColor;
    m_fAlpha = fAlpha;
    m_nAlign = nAlign;
    m_bHilight = 0;
    m_nStrRef = -1;
}

// FUNCTION: SWKOTOR 0x004171e0
CSWGuiTextParams& CSWGuiTextParams::operator=(const CSWGuiTextParams& p)
{
    m_sText = p.m_sText;
    m_nStrRef = p.m_nStrRef;
    strcpy(m_szBaseFont, p.m_szBaseFont);
    m_vDefaultColor = p.m_vColor;
    m_vColor = p.m_vColor;
    m_nAlign = p.m_nAlign;
    m_fAlpha = p.GetAlpha();
    m_bHilight = p.m_bHilight;
    SetOwner(m_pText);
    return *this;
}

// FUNCTION: SWKOTOR 0x00417280
CSWGuiText::CSWGuiText()
{
    m_pString = 0;
    m_cParams.SetOwner(this);
    m_nMaxWidth = 400;
}

// FUNCTION: SWKOTOR 0x004172f0 SYMBOL: ??_GCSWGuiText@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00417310
void CSWGuiText::Initialize(const CSWGuiExtent& extent, const CSWGuiTextParams& params, float fScale)
{
    if (m_pString) {
        m_pString->Release(1);
    }
    char szFont[20];
    params.m_cFont.CopyToString(szFont);
    m_pString = NewCAurGUIString(params.m_sText.CStr(), szFont, params.m_vColor, fScale);
    m_cParams = params;
    m_extent = extent;
    if (m_pString) {
        m_pString->SetPosition(m_extent.left, m_extent.top);
        m_pString->SetSize(m_extent.width, m_extent.height);
        m_pString->SetAlignment(params.m_nAlign);
        Rebuild();
    }
}

// FUNCTION: SWKOTOR 0x004173d0
CSWGuiEditText::CSWGuiEditText()
{
    m_bCaret = 0;
    m_nMaxLength = -1;
    m_sText = "";
}

// FUNCTION: SWKOTOR 0x00417440 SYMBOL: ??_GCSWGuiEditText@@UAEPAXI@Z

// Compiler-generated: no vfptr store before the member/base destructors.
// FUNCTION: SWKOTOR 0x00417460 SYMBOL: ??1CSWGuiEditText@@UAE@XZ

// FUNCTION: SWKOTOR 0x004175c0
int CSWGuiScene::AddModel(CAurObject* pModel, int nIndex, int bAttach)
{
    if (!pModel) {
        return 0;
    }
    if (nIndex >= 0 && nIndex < m_lstModels.num) {
        m_lstModels.Insert(pModel, nIndex);
    }
    else {
        m_lstModels.Add(pModel);
    }
    if (bAttach) {
        pModel->AttachToScene(m_pScene);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00416e70 SYMBOL: ??_GCSWGuiImage@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00414dc0 SYMBOL: ??0STR_RES@@QAE@XZ

float g_fGuiPulseTimer;                 // 0x0078d3d0
float g_fGuiPulseAlpha;                 // 0x0078d3d4
int g_bGuiPulseRising;                  // 0x007a4578

// FUNCTION: SWKOTOR 0x00414a50
void __cdecl UpdateGuiPulse(float fDelta)
{
    g_fGuiPulseTimer -= fDelta;
    if (g_fGuiPulseTimer <= 0.0f) {
        g_bGuiPulseRising = !g_bGuiPulseRising;
        g_fGuiPulseTimer = 0.75f;
    }
    float fFrom;
    float fTo;
    if (g_bGuiPulseRising) {
        fTo = 1.0f;
        fFrom = 0.4f;
    } else {
        fTo = 0.4f;
        fFrom = 1.0f;
    }
    g_fGuiPulseAlpha = fFrom + ((0.75f - g_fGuiPulseTimer) * 1.3333334f) * (fTo - fFrom);
}

float g_fGuiHilightTimer;               // 0x0078d3e8
float g_fGuiHilightAlpha;               // 0x0078d3e4
bool g_bGuiHilightRising;               // 0x007a457c

// FUNCTION: SWKOTOR 0x00418300
void __cdecl UpdateGuiHilightPulse(float fDelta)
{
    g_fGuiHilightTimer -= fDelta;
    if (g_fGuiHilightTimer <= 0.0f) {
        g_bGuiHilightRising = !g_bGuiHilightRising;
        g_fGuiHilightTimer = 0.75f;
    }
    float fFrom;
    float fTo;
    if (g_bGuiHilightRising) {
        fTo = 1.0f;
        fFrom = 0.2f;
    } else {
        fTo = 0.2f;
        fFrom = 1.0f;
    }
    g_fGuiHilightAlpha = fFrom + ((0.75f - g_fGuiHilightTimer) * 1.3333334f) * (fTo - fFrom);
}

// FUNCTION: SWKOTOR 0x004168a0 SYMBOL: ??_GCSWGuiBorder@@UAEPAXI@Z
