// CGuiInGame placeable-camera record setter (guiingame.o, name unknown:
// placeholder SetPlaceableCamera). Records are 0x30 bytes at +0x1B4,
// count at +0x1B0.
struct Vector { float x,y,z; };
struct Quaternion { float w,x,y,z; };
struct CSWPlaceableCamera {
    unsigned long m_nId; Vector m_vPosition; Quaternion m_qOrientation;
    unsigned long m_n20,m_n24,m_n28; float m_f2C;
};
class CGuiInGame {
public:
    char pad_0x0[0x1b0]; int m_nPlaceableCameras;
    CSWPlaceableCamera m_cameras[51];
    void SetPlaceableCamera(int nIndex,unsigned long nId,const Vector& vPosition,const Quaternion& qOrientation,
                            unsigned long n20,unsigned long n24,unsigned long n28,float f2C);
};

// FUNCTION: SWKOTOR 0x0062ACA0
void CGuiInGame::SetPlaceableCamera(int nIndex,unsigned long nId,const Vector& vPosition,const Quaternion& qOrientation,
                                    unsigned long n20,unsigned long n24,unsigned long n28,float f2C)
{
    if (nIndex < m_nPlaceableCameras) {
        m_cameras[nIndex].m_nId = nId;
        m_cameras[nIndex].m_vPosition = vPosition;
        m_cameras[nIndex].m_qOrientation = qOrientation;
        m_cameras[nIndex].m_n20 = n20;
        m_cameras[nIndex].m_n24 = n24;
        m_cameras[nIndex].m_n28 = n28;
        m_cameras[nIndex].m_f2C = f2C;
    }
}
