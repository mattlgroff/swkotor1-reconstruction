// The conversation camera behaviour CSWBehaviorCameraDialog (vtable
// 0x00756AE8). The scene returns it for camera id 0x106D; SetShot places a
// scripted (type 6) shot, and the line-of-action table keeps the camera on
// one side of each speaker/listener pair.
#include "l/sw_behavior_camera_dialog.h"

// FUNCTION: SWKOTOR 0x006BB590
void CSWBehaviorCameraDialog::SetShot(int nType, Vector vPosition, Quaternion qOrientation,
                                      float fFieldOfView)
{
    if (nType == 6) {
        m_n34 = 0;
        if (m_pGob->GetCameraView()) {
            m_pGob->GetCameraView()->SetViewAngle(fFieldOfView);
        }
        m_nShotType = 6;
        m_vPosition = vPosition;
        m_qOrientation = qOrientation;
    }
}

// FUNCTION: SWKOTOR 0x006BB610
int CSWBehaviorCameraDialog::GetLineOfActionSide(unsigned long oidA, unsigned long oidB)
{
    for (int i = 0; i < 4; i++) {
        if (m_aLines[i].m_nSide == 0) {
            break;
        }
        if (m_aLines[i].m_oidTo == oidA && m_aLines[i].m_oidFrom == oidB) {
            return m_aLines[i].m_nSide;
        }
        if (m_aLines[i].m_oidTo == oidB && m_aLines[i].m_oidFrom == oidA) {
            return m_aLines[i].m_nSide == 2 ? 1 : 2;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006BB670
// NONMATCH: the original forms &m_rListener in ebp before its zero store
// (ours stores first; 3 insns, scheduling only).
CSWBehaviorCameraDialog::CSWBehaviorCameraDialog(Gob* pGob, unsigned long nShot)
    : CAurBehavior(pGob), m_vPosition(0.0f, 0.0f, 0.0f)
{
    m_n14 = 0;
    m_n18 = 0;
    m_n1C = 0;
    m_n20 = 0;
    m_n24 = 0;
    m_rSpeaker.Clear();
    m_rListener.Clear();
    m_nShot = nShot;
    m_n2C = 0;
    m_n30 = 0;
    m_n34 = 0;
    for (int i = 0; i < 4; i++) {
        m_aLines[i].m_nSide = 0;
        m_aLines[i].m_oidFrom = (unsigned long)-1;
        m_aLines[i].m_oidTo = (unsigned long)-1;
    }
    m_bFirstFrame = 1;
}

// FUNCTION: SWKOTOR 0x006BB740
int CSWBehaviorCameraDialog::GetType()
{
    return 0x106d;
}

// FUNCTION: SWKOTOR 0x006BB750
CSWBehaviorCameraDialog::~CSWBehaviorCameraDialog()
{
}

// FUNCTION: SWKOTOR 0x006BB790 SYMBOL: ??_GCSWBehaviorCameraDialog@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006BCEC0
int CSWBehaviorCameraDialog::AddLineOfAction(unsigned long oidA, unsigned long oidB)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (m_aLines[i].m_nSide == 0) {
            break;
        }
        if ((m_aLines[i].m_oidTo == oidA && m_aLines[i].m_oidFrom == oidB) ||
            (m_aLines[i].m_oidTo == oidB && m_aLines[i].m_oidFrom == oidA)) {
            if (m_nShot == 0) {
                return GetLineOfActionSide(oidA, oidB);
            }
            return m_aLines[i].m_nSide;
        }
    }
    if (i == 4) {
        i = 0;
    }
    m_aLines[i].m_nSide = CalculateLineOfActionSide();
    m_aLines[i].m_oidFrom = oidB;
    m_aLines[i].m_oidTo = oidA;
    return m_aLines[i].m_nSide;
}
