// CSWGuiBorder / CSWGuiImage / CSWGuiText Load overrides, the
// border inner extent and fill drawing, CSWGuiImage SetExtent / Draw.
// Kept apart from gui_parts.cpp so the callees stay out of line.
#include "d/gui_parts.h"

class CResGFF {
public:
    int GetStructFromStruct(CResStruct* pOut, CResStruct* pStruct, char* szLabel); // 0x00411a10
    unsigned char ReadFieldBYTE(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                unsigned char nDefault); // 0x00411a60
    unsigned long ReadFieldDWORD(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                 unsigned long nDefault); // 0x00411c20
    int ReadFieldINT(CResStruct* pStruct, char* szLabel, int& bSuccess, int nDefault); // 0x00411c90
    float ReadFieldFLOAT(CResStruct* pStruct, char* szLabel, int& bSuccess, float fDefault); // 0x00411d00
    CResRef ReadFieldCResRef(CResStruct* pStruct, char* szLabel, int& bSuccess,
                             const CResRef& cDefault); // 0x00411e10
    CExoString ReadFieldCExoString(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                   const CExoString& sDefault); // 0x00411ec0
    Vector ReadFieldVector(CResStruct* pStruct, char* szLabel, int& bSuccess,
                           const Vector& vDefault); // 0x004122a0
};

extern float g_fGuiPulseAlpha;                      // 0x0078d3d4

#define GUI_MIN(a, b) ((a) < (b) ? (a) : (b))

#pragma auto_inline(off)

// FUNCTION: SWKOTOR 0x00415240
CSWGuiExtent CSWGuiBorder::GetInnerExtent()
{
    int nEdgeW;
    int nCornerW;
    int nCornerH;
    if (m_cParams.GetCornerSize()) {
        nCornerH = nCornerW = nEdgeW = m_cParams.GetCornerSize();
    }
    else if (m_pCorner && m_pEdge && m_extent.height && m_extent.width) {
        nCornerH = m_pCorner->GetHeight();
        nCornerW = m_pCorner->GetWidth();
        m_pEdge->GetHeight();
        nEdgeW = m_pEdge->GetWidth();
    }
    else {
        nEdgeW = 0;
        nCornerH = nCornerW = 0;
    }

    CSWGuiExtent r;
    int nX;
    int nWidth = m_extent.width;
    int nInnerW = nWidth - nCornerW * 2;
    int nTiles = nEdgeW ? nInnerW / nEdgeW : 0;
    if (nInnerW > 0 && (nTiles || nEdgeW / 2 <= nInnerW)) {
        nX = m_extent.left + nCornerW;
        r.width = 0 > nInnerW ? 0 : nInnerW;
    }
    else {
        nX = nWidth / 2 + m_extent.left;
        r.width = 0;
    }

    int nY;
    int nHeight = m_extent.height;
    int nInnerH = nHeight - nCornerH * 2;
    nTiles = nEdgeW ? nInnerH / nEdgeW : 0;
    if (nInnerH > 0 && (nTiles || nEdgeW / 2 <= nInnerH)) {
        nY = m_extent.top + nCornerH;
        r.height = 0 > nInnerH ? 0 : nInnerH;
    }
    else {
        nY = nHeight / 2 + m_extent.top;
        r.height = 0;
    }

    nY -= m_cParams.m_nPulse;
    nX -= GUI_MIN(m_cParams.m_nPulse, m_cParams.GetCornerSize());
    r.left = nX;
    r.top = nY;
    r.width += GUI_MIN(m_cParams.m_nPulse, m_cParams.GetCornerSize()) * 2;
    r.height += m_cParams.m_nPulse * 2;
    return r;
}

// FUNCTION: SWKOTOR 0x004153e0
void CSWGuiBorder::Load(CResGFF* pGFF, CResStruct* pStruct, const CExoString& sName)
{
    CResStruct cStruct;
    if (pGFF->GetStructFromStruct(&cStruct, pStruct, sName.CStr())) {
        int bSuccess;
        {
            CResRef cDefault("");
            m_cParams.SetCornerImage(pGFF->ReadFieldCResRef(&cStruct, "CORNER", bSuccess, cDefault));
        }
        CSWGuiBorderParams* pParams = &m_cParams;
        {
            CResRef cDefault("");
            pParams->SetEdgeImage(pGFF->ReadFieldCResRef(&cStruct, "EDGE", bSuccess, cDefault));
        }
        {
            CResRef cDefault("");
            pParams->SetFillImage(pGFF->ReadFieldCResRef(&cStruct, "FILL", bSuccess, cDefault));
        }
        pParams->m_nStyle = pGFF->ReadFieldINT(&cStruct, "FILLSTYLE", bSuccess, 0);
        pParams->m_nCornerSize = pGFF->ReadFieldINT(&cStruct, "DIMENSION", bSuccess, 0);
        m_cParams.m_nPulse = pGFF->ReadFieldINT(&cStruct, "INNEROFFSET", bSuccess, 0);
        m_cParams.m_vColor = pGFF->ReadFieldVector(&cStruct, "COLOR", bSuccess, Vector(1.0f, 1.0f, 1.0f));
        unsigned char nPulsing = pGFF->ReadFieldBYTE(&cStruct, "PULSING", bSuccess, 0);
        pParams->SetFlags(nPulsing > 0, nPulsing & 1, (nPulsing >> 1) & 1);
    }
}

// FUNCTION: SWKOTOR 0x004155b0
void CSWGuiBorder::DrawFillTiled(int nHeight, int nWidth, int nX, int nY, float fAlpha,
                                 const Vector* pColor)
{
    int nTileW = m_pFill->GetWidth();
    int nTileH = m_pFill->GetHeight();
    if (nTileW > 0 && nTileH > 0) {
        int nCols = nWidth / nTileW;
        int nRows = nHeight / nTileH;
        float fRotate = m_cParams.m_f08;
        nHeight -= nRows * nTileH;
        nWidth -= nCols * nTileW;
        int nExtraH;
        if (nRows) {
            nExtraH = nHeight / nRows;
            nHeight -= nExtraH * nRows;
        }
        if (nCols) {
            int nExtraW = nWidth / nCols;
            nWidth -= nExtraW * nCols;
            int nRemH = nHeight;
            int x = nX;
            for (int i = 0; i < nCols; i++) {
                int nColW = nExtraW + nTileW;
                if (nWidth) {
                    nWidth--;
                    nColW++;
                }
                if (nRows) {
                    nHeight = nRemH;
                    int y = nY;
                    for (int j = 0; j < nRows; j++) {
                        int nRowH = nExtraH + nTileH;
                        if (nHeight) {
                            nHeight--;
                            nRowH++;
                        }
                        m_pFill->Draw(x, y, nColW, nRowH, 0, fRotate, pColor, fAlpha);
                        y += nRowH;
                    }
                }
                else {
                    m_pFill->Draw(x, nY, nColW, nHeight, 0, fRotate, pColor, fAlpha);
                }
                x += nColW;
            }
        }
        else if (nRows) {
            int y = nY;
            for (int j = 0; j < nRows; j++) {
                int nRowH = nExtraH + nTileH;
                if (nHeight) {
                    nHeight--;
                    nRowH++;
                }
                m_pFill->Draw(nX, y, nWidth, nRowH, 0, fRotate, pColor, fAlpha);
                y += nRowH;
            }
        }
        else {
            m_pFill->Draw(nX, nY, nWidth, nHeight, 0, fRotate, pColor, fAlpha);
        }
    }
}

// FUNCTION: SWKOTOR 0x004157f0
void CSWGuiBorder::DrawFillCentered(int nHeight, int nWidth, int nX, int nY, float fAlpha,
                                    const Vector* pColor)
{
    int nTileW = m_pFill->GetWidth();
    int nTileH = m_pFill->GetHeight();
    float fRotate = m_cParams.m_f08;
    if (nTileW < nWidth) {
        if (nTileH < nHeight) {
            m_pFill->Draw((nWidth - nTileW) / 2 + nX, (nHeight - nTileH) / 2 + nY, nTileW, nTileH,
                          0, fRotate, pColor, fAlpha);
        }
        else {
            m_pFill->Draw((nWidth - nTileW) / 2 + nX, nY, nTileW, nHeight, 0, fRotate, pColor,
                          fAlpha);
        }
    }
    else if (nTileH < nHeight) {
        m_pFill->Draw(nX, (nHeight - nTileH) / 2 + nY, nWidth, nTileH, 0, fRotate, pColor, fAlpha);
    }
    else {
        m_pFill->Draw(nX, nY, nWidth, nHeight, 0, fRotate, pColor, fAlpha);
    }
}

// FUNCTION: SWKOTOR 0x00415b70
void CSWGuiImage::SetExtent(const CSWGuiExtent& extent)
{
    CSWGuiExtent& ext = m_extent;
    ext = extent;
    if (m_pImage) {
        switch (m_cParams.m_b1) {
        case 0:
            m_cInner = extent;
            break;
        case 1:
            m_cInner.height = m_pImage->GetHeight();
            m_cInner.width = m_pImage->GetWidth();
            if (m_cInner.width >= m_extent.width) {
                m_cInner.width = m_extent.width;
                m_cInner.left = m_extent.left;
            }
            else {
                switch (m_cParams.m_n6 & 7) {
                case 1:
                    m_cInner.left = m_extent.left;
                    break;
                case 2:
                    m_cInner.left = (m_extent.width - m_cInner.width) / 2 + m_extent.left;
                    break;
                case 4:
                    m_cInner.left = ext.width - m_cInner.width + ext.left;
                    break;
                }
            }
            if (m_cInner.height >= m_extent.height) {
                m_cInner.height = m_extent.height;
                m_cInner.top = m_extent.top;
            }
            else {
                switch (m_cParams.m_n6 & 0x38) {
                case 8:
                    m_cInner.top = m_extent.top;
                    break;
                case 0x10:
                    m_cInner.top = (m_extent.height - m_cInner.height) / 2 + m_extent.top;
                    break;
                case 0x20:
                    m_cInner.top = ext.height + ext.top - m_cInner.height;
                    break;
                }
            }
            break;
        }
    }
}

// FUNCTION: SWKOTOR 0x00415c90
void CSWGuiImage::Draw(float fDelta)
{
    if (m_pImage && m_extent.width && m_extent.height) {
        float fAlpha = m_cParams.m_f14;
        if (m_cParams.m_b0) {
            fAlpha = g_fGuiPulseAlpha;
        }
        m_pImage->Draw(m_cInner.left, m_cInner.top, m_cInner.width, m_cInner.height,
                       m_cParams.m_n2, m_cParams.GetRotation(), &m_cParams.m_vColor, fAlpha);
    }
}

// FUNCTION: SWKOTOR 0x00416050
void CSWGuiText::Load(CResGFF* pGFF, CResStruct* pStruct, const CExoString& sName)
{
    CResStruct cStruct;
    if (pGFF->GetStructFromStruct(&cStruct, pStruct, sName.CStr())) {
        int bSuccess;
        {
            CResRef cDefault("dialogfont16x16");
            m_cParams.SetBaseFont(pGFF->ReadFieldCResRef(&cStruct, "FONT", bSuccess, cDefault));
        }
        CSWGuiTextParams* pParams = &m_cParams;
        pParams->SetDefaultColor(pGFF->ReadFieldVector(&cStruct, "COLOR", bSuccess,
                                                        Vector(1.0f, 1.0f, 1.0f)));
        {
            CExoString sDefault("");
            pParams->SetText(pGFF->ReadFieldCExoString(&cStruct, "TEXT", bSuccess, sDefault));
        }
        pParams->SetStrRef(pGFF->ReadFieldDWORD(&cStruct, "STRREF", bSuccess, -1));
        pParams->SetAlignment(pGFF->ReadFieldINT(&cStruct, "ALIGNMENT", bSuccess, 9));
        pParams->SetHilight(pGFF->ReadFieldBYTE(&cStruct, "PULSING", bSuccess, 0));
        if (m_pString) {
            m_pString->Release(1);
            m_pString = 0;
        }
        char szFont[20];
        m_cParams.m_cFont.CopyToString(szFont);
        m_pString = NewCAurGUIString(pParams->m_sText.CStr(), szFont, m_cParams.m_vColor, 1.0f);
        pParams->SetOwner(this);
    }
}

// FUNCTION: SWKOTOR 0x00416e90
void CSWGuiImage::Initialize(const CSWGuiExtent& extent, const CSWGuiImageParams& params)
{
    m_extent = extent;
    m_cParams = params;
}

// FUNCTION: SWKOTOR 0x00416ec0
void CSWGuiImage::Load(CResGFF* pGFF, CResStruct* pStruct, const CExoString& sName)
{
    CResStruct cStruct;
    if (pGFF->GetStructFromStruct(&cStruct, pStruct, sName.CStr())) {
        int bSuccess;
        {
            CResRef cDefault("");
            m_cParams.SetImage(pGFF->ReadFieldCResRef(&cStruct, "IMAGE", bSuccess, cDefault));
        }
        CSWGuiImageParams* pParams = &m_cParams;
        pParams->SetB1(pGFF->ReadFieldINT(&cStruct, "DRAWSTYLE", bSuccess, 0));
        pParams->m_n2 = pGFF->ReadFieldINT(&cStruct, "FLIPSTYLE", bSuccess, 0);
        pParams->SetN6(pGFF->ReadFieldINT(&cStruct, "ALIGNMENT", bSuccess, 0x12));
        float fRotate = pGFF->ReadFieldFLOAT(&cStruct, "ROTATE", bSuccess, 0.0f);
        if (!bSuccess) {
            switch (pGFF->ReadFieldINT(&cStruct, "ROTATESTYLE", bSuccess, 0)) {
            case 0:
                fRotate = 0.0f;
                break;
            case 1:
                fRotate = 90.0f;
                break;
            case 2:
                fRotate = 180.0f;
                break;
            case 3:
                fRotate = 270.0f;
                break;
            }
        }
        m_cParams.m_f10 = fRotate;
    }
}
