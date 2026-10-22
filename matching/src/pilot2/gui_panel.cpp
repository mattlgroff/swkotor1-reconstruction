// CSWGuiPanel::GetLocalMouseCoords (pilot group 2).

struct CSWGuiExtent {
    int left;
    int top;
    int width;
    int height;
};

class CSWGuiManager {
public:
    int m_nMouseX;
    int m_nMouseY;
};

extern CSWGuiManager* g_pGuiManager;   // 0x007A39F4
extern int g_nScreenWidth;             // 0x0078D1D4
extern int g_nScreenHeight;            // 0x0078D1D8

class CSWGuiPanel {
public:
    void* vtable;
    CSWGuiExtent m_extent;          // 0x04
    char pad_0x14[0x30];
    unsigned long m_nFlags;         // 0x44

    void GetAbsoluteExtent(CSWGuiExtent* ext);
    void GetLocalMouseCoords(int* x, int* y);
};

// FUNCTION: SWKOTOR 0x0040BA20
void CSWGuiPanel::GetLocalMouseCoords(int* x, int* y)
{
    *x = g_pGuiManager->m_nMouseX;
    *y = g_pGuiManager->m_nMouseY;
    CSWGuiExtent ext = m_extent;
    if (!(m_nFlags & 1)) {
        *x -= (g_nScreenWidth - 640) / 2;
        *y -= (g_nScreenHeight - 480) / 2;
        *x -= ext.left;
        *y -= ext.top;
    } else {
        GetAbsoluteExtent(&ext);
        *x -= ext.left;
        *y -= ext.top;
    }
}
