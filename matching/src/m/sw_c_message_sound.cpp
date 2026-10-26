#include "m/sw_c_message_handlers.h"

// FUNCTION: SWKOTOR 0x0064F050
int CSWCMessage::HandleServerToPlayerSoundObject_Play()
{
    unsigned long id = ReadOBJECTIDClient();
    if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
    CSWCSoundObject* sound = g_pAppManager->m_pClientExoApp->GetSoundObjectByGameObjectID(id);
    if (sound) sound->Play();
    return 1;
}

// FUNCTION: SWKOTOR 0x0064F0A0
int CSWCMessage::HandleServerToPlayerSoundObject_Stop()
{
    unsigned long id = ReadOBJECTIDClient();
    if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
    CSWCSoundObject* sound = g_pAppManager->m_pClientExoApp->GetSoundObjectByGameObjectID(id);
    if (sound) sound->Stop();
    return 1;
}

// FUNCTION: SWKOTOR 0x0064F0F0
int CSWCMessage::HandleServerToPlayerSoundObject_ChangePosition()
{
    Vector position = {0.0f, 0.0f, 0.0f};
    unsigned long id = ReadOBJECTIDClient();
    position.x = ReadFLOAT(1.0f, 0x20);
    position.y = ReadFLOAT(1.0f, 0x20);
    position.z = ReadFLOAT(1.0f, 0x20);
    if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
    CSWCSoundObject* sound = g_pAppManager->m_pClientExoApp->GetSoundObjectByGameObjectID(id);
    if (sound) sound->SetPositionElevated(position);
    return 1;
}

// FUNCTION: SWKOTOR 0x00656BA0
int CSWCMessage::HandleServerToPlayerSound(unsigned char minor)
{
    switch (minor) {
    case 2: return HandleServerToPlayerSoundObject_Play();
    case 3: return HandleServerToPlayerSoundObject_Stop();
    case 5: return HandleServerToPlayerSoundObject_ChangePosition();
    default: return 0;
    }
}
