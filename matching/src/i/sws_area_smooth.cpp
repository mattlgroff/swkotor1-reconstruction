// CSWSArea time-sliced path smoothing passes (0x004C13E0-0x004C19FF).
// Self-contained declarations; only the members these functions use are
// placed.
#include <stddef.h>

extern const int g_nSmoothStateStart;   // 0x0074C444 (placeholder name)
extern const int g_nSmoothStatePass1;   // 0x0074C448 (placeholder name)
extern const int g_nSmoothStatePass2;   // 0x0074C44C (placeholder name)
extern const int g_nSmoothStatePass3;   // 0x0074C450 (placeholder name)

class CPathfindInformation {
public:
    void FUN_004bb480(float* p);                    // 0x004BB480
    void FUN_004bb4d0(float* p);                    // 0x004BB4D0
    void FUN_004bb520(float* p);                    // 0x004BB520
    // In-place copies of the three setters above; the original expands
    // them inline when clearing the buffers.
    void SetSmoothPass1(float* p)
    {
        if ((m_p21c != NULL || p == NULL) && m_p21c != NULL && m_p21c != p) {
            m_n218 = 0;
            delete[] m_p21c;
            m_p21c = NULL;
        }
        m_p21c = p;
    }
    void SetSmoothPass2(float* p)
    {
        if ((m_p228 != NULL || p == NULL) && m_p228 != NULL && m_p228 != p) {
            m_n220 = 0;
            delete[] m_p228;
            m_p228 = NULL;
        }
        m_p228 = p;
    }
    void SetSmoothPass3(float* p)
    {
        if ((m_p22c != NULL || p == NULL) && m_p22c != NULL && m_p22c != p) {
            m_n224 = 0;
            delete[] m_p22c;
            m_p22c = NULL;
        }
        m_p22c = p;
    }

    char m_pad0[0x1c];
    int m_bNoSmoothing;                             // +0x1c (placeholder name)
    char m_pad20[0x8c - 0x20];
    int m_nSmoothedPoints;                          // +0x8c (placeholder name)
    float* m_pSmoothedPoints;                       // +0x90 (placeholder name)
    int m_nPoints;                                  // +0x94 (placeholder name)
    float* m_pPoints;                               // +0x98 (placeholder name)
    char m_pad9c[0x214 - 0x9c];
    int m_nSmoothState;                             // +0x214 (placeholder name)
    int m_n218;                                     // +0x218
    float* m_p21c;                                  // +0x21c
    int m_n220;                                     // +0x220
    int m_n224;                                     // +0x224
    float* m_p228;                                  // +0x228
    float* m_p22c;                                  // +0x22c
    int m_n230;                                     // +0x230
    int m_n234;                                     // +0x234
};

class CSWSArea {
public:
    int SmoothCornerOptimize(int nCount, float* pPoints, int* pnOut,
                             float** ppOut);        // 0x004BFE20 (placeholder name)
    int SmoothSelection(int nCount, float* pPoints, int* pnOut, float** ppOut, int bArg5,
                        int bArg6, float fArg7);    // 0x004C04E0 (placeholder name)
    int SmoothSelectionRestartable(int nCount, float* pPoints, int* pnOut,
                                   float** ppOut);  // 0x004C13E0
    int SmoothPointsOnPathRestartable();            // 0x004C17A0

    char m_pad0[0x1ac];
    CPathfindInformation* m_pPathfindInformation;   // +0x1ac
};

// FUNCTION: SWKOTOR 0x004C13E0
int CSWSArea::SmoothSelectionRestartable(int nCount, float* pPoints, int* pnOut, float** ppOut)
{
    int nFirst = m_pPathfindInformation->m_n218;
    int nSecond = m_pPathfindInformation->m_n220;
    float* pSecond = m_pPathfindInformation->m_p228;
    float* pFirst = m_pPathfindInformation->m_p21c;
    int nResult;
    if (m_pPathfindInformation->m_nSmoothState == g_nSmoothStatePass1) {
        nResult = SmoothSelection(nCount, pPoints, &nFirst, &pFirst, 1, 0, 5.0f);
        m_pPathfindInformation->FUN_004bb480(pFirst);
        m_pPathfindInformation->m_n218 = nFirst;
        if (nResult == 1) {
            return 1;
        }
        m_pPathfindInformation->m_n234 = -1;
        m_pPathfindInformation->m_n230 = 0;
        m_pPathfindInformation->m_nSmoothState = g_nSmoothStatePass2;
    }
    if (m_pPathfindInformation->m_nSmoothState == g_nSmoothStatePass2) {
        nResult = SmoothSelection(nFirst, pFirst, &nSecond, &pSecond, 1, 1, 1.1f);
        m_pPathfindInformation->FUN_004bb4d0(pSecond);
        m_pPathfindInformation->m_n220 = nSecond;
        if (nResult == 1) {
            return 1;
        }
        m_pPathfindInformation->m_n234 = -1;
        m_pPathfindInformation->m_n230 = 0;
        m_pPathfindInformation->m_nSmoothState = g_nSmoothStatePass3;
    }
    if (m_pPathfindInformation->m_nSmoothState == g_nSmoothStatePass3) {
        if (SmoothSelection(nSecond, pSecond, pnOut, ppOut, 0, 1, 1.1f) == 1) {
            return 1;
        }
    }
    m_pPathfindInformation->SetSmoothPass1(NULL);
    m_pPathfindInformation->SetSmoothPass2(NULL);
    return 0;
}

// FUNCTION: SWKOTOR 0x004C17A0
int CSWSArea::SmoothPointsOnPathRestartable()
{
    int nPoints = m_pPathfindInformation->m_nPoints;
    float* pPoints = m_pPathfindInformation->m_pPoints;
    if (nPoints == 0) {
        m_pPathfindInformation->m_nSmoothedPoints = 0;
        m_pPathfindInformation->m_pSmoothedPoints = NULL;
        return 0;
    }
    if (m_pPathfindInformation->m_nSmoothState == g_nSmoothStateStart) {
        if (m_pPathfindInformation->m_pSmoothedPoints != NULL) {
            delete[] m_pPathfindInformation->m_pSmoothedPoints;
            m_pPathfindInformation->m_pSmoothedPoints = NULL;
            m_pPathfindInformation->m_nSmoothedPoints = 0;
        }
        if (m_pPathfindInformation->m_bNoSmoothing == 1) {
            float* pCopy = new float[nPoints];
            for (int i = 0; i < nPoints; i++) {
                pCopy[i] = pPoints[i];
            }
            m_pPathfindInformation->m_pSmoothedPoints = pCopy;
            m_pPathfindInformation->m_nSmoothedPoints = nPoints;
            return 0;
        }
        m_pPathfindInformation->m_n234 = -1;
        m_pPathfindInformation->m_n230 = 0;
        m_pPathfindInformation->m_nSmoothState = g_nSmoothStatePass1;
        m_pPathfindInformation->SetSmoothPass1(NULL);
        m_pPathfindInformation->SetSmoothPass2(NULL);
        m_pPathfindInformation->SetSmoothPass3(NULL);
    }
    int nOut = m_pPathfindInformation->m_n224;
    float* pOut = m_pPathfindInformation->m_p22c;
    float* pCorners = NULL;
    int nResult = SmoothSelectionRestartable(nPoints, pPoints, &nOut, &pOut);
    m_pPathfindInformation->m_n224 = nOut;
    m_pPathfindInformation->FUN_004bb520(pOut);
    if (nResult == 1) {
        return nResult;
    }
    SmoothCornerOptimize(nOut, pOut, &nOut, &pCorners);
    m_pPathfindInformation->m_pSmoothedPoints = pCorners;
    m_pPathfindInformation->m_nSmoothedPoints = nOut;
    m_pPathfindInformation->m_n224 = 0;
    m_pPathfindInformation->SetSmoothPass3(NULL);
    return 0;
}
