// Ordered scene Gob comparison. Names describe their role.
class ModelSortView {
public:
    char pad_0x0[0x50];
    unsigned char m_nFlags;
};
class GobSortView {
public:
    char pad_0x0[0x58];
    ModelSortView* m_pModel;
    char pad_0x5c[0x7c];
    float m_fDistance;
    char pad_0xdc[0x28];
    float m_fOrder;
    char pad_0x108[0x77];
    unsigned char m_bPriority;
};
extern int g_bOrderByPriority; // 0x0078E3B4, descriptive

// FUNCTION: SWKOTOR 0x00450270
int GobOrderCmp(const GobSortView** pa, const GobSortView** pb)
{
    const GobSortView* a = *pa;
    const GobSortView* b = *pb;
    if (a->m_fDistance < b->m_fDistance) return -1;
    if (a->m_fDistance > b->m_fDistance) return 1;
    unsigned char fa = a->m_pModel->m_nFlags;
    unsigned char fb = b->m_pModel->m_nFlags;
    if (fa & 1) { if (!(fb & 1)) return -1; }
    else if (fb & 1) return 1;
    if (g_bOrderByPriority) {
        if (a->m_bPriority) { if (!b->m_bPriority) return -1; }
        else if (b->m_bPriority) return 1;
    }
    if (fa & 4) { if (!(fb & 4)) return 1; }
    else if (fb & 4) return -1;
    if (fa & 16) { if (!(fb & 16)) return -1; }
    else if (fb & 16) return 1;
    if (fa & 8) { if (!(fb & 8)) return -1; }
    else if (fb & 8) return 1;
    return a->m_fOrder > b->m_fOrder ? 1 : -1;
}
