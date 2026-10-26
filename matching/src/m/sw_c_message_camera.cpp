#include <stdio.h>
class CAurCamera { public:
 virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
 virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
 virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
 virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
 virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
 virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
 virtual void RunCommand(const char*); // descriptive vtable slot 0x60
};
class CSWCModule { public: char pad_000[0x40]; CAurCamera* m_pCamera; };
class CClientOptions { public: void SetCameraMode(unsigned char); };
class CClientExoApp { public: CSWCModule* GetModule(); CClientOptions* GetOptions(); };
class CAppManager { public: void* m_pUnknown00; CClientExoApp* m_pClientExoApp; };
extern CAppManager* g_pAppManager;
class CSWCMessage { public:
 unsigned char ReadBYTE(int); float ReadFLOAT(float,int); int MessageReadOverflow(int);
 int HandleServerToPlayerCamera(unsigned char);
};
// FUNCTION: SWKOTOR 0x0064B8E0
int CSWCMessage::HandleServerToPlayerCamera(unsigned char minor)
{
 if (MessageReadOverflow(1)) return 0;
 if (minor == 1) {
  unsigned char flags = ReadBYTE(8);
  if (MessageReadOverflow(1)) return 0;
  unsigned char yawFlag = flags & 1;
  float yaw, distance, pitch;
  if (yawFlag) yaw = ReadFLOAT(1.0f,32);
  unsigned char distanceFlag = flags & 2;
  if (distanceFlag) distance = ReadFLOAT(1.0f,32);
  unsigned char pitchFlag = flags & 4;
  if (pitchFlag) pitch = ReadFLOAT(1.0f,32);
  if (flags & 8) ReadBYTE(8);
  if (MessageReadOverflow(1)) return 0;
  CAurCamera* camera;
  if (g_pAppManager->m_pClientExoApp->GetModule())
   camera = g_pAppManager->m_pClientExoApp->GetModule()->m_pCamera;
  if (!camera) return 0;
  char text[64];
  if (yawFlag) {
   sprintf(text,"yaw %f",(yaw-1.5707963705062866f)*57.295780181884766f);
   camera->RunCommand(text);
  }
  if (distanceFlag) {
   sprintf(text,"dist %f",distance);
   camera->RunCommand(text);
  }
  if (pitchFlag) {
   sprintf(text,"pitch %f",pitch);
   camera->RunCommand(text);
  }
 } else if (minor == 2) {
  unsigned char mode = ReadBYTE(8);
  if (MessageReadOverflow(1)) return 0;
  CClientOptions* options = g_pAppManager->m_pClientExoApp->GetOptions();
  if (options) options->SetCameraMode(mode);
 }
 return 0;
}
