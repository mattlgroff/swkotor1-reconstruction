#include "m/sw_c_message_handlers.h"

// FUNCTION: SWKOTOR 0x00663810
int CSWCMessage::HandleServerToPlayerMapPin(unsigned char minor)
{
    switch (minor) {
    case 1: return HandleServerToPlayerMapPinEnabled();
    case 4: return HandleServerToPlayerMapPinReferenceNumber();
    default: return 0;
    }
}

// FUNCTION: SWKOTOR 0x00652D00
int CSWCMessage::HandleServerToPlayerMapPinEnabled()
{
    if (MessageReadOverflow(1)) return 0;
    unsigned long id = ReadOBJECTIDClient();
    int enabled = ReadBOOL();
    CSWCArea* area = g_pAppManager->m_pClientExoApp->GetModule()->m_pArea;
    CSWCAreaObject* pin = area->FindObject(id, 0);
    if (pin) pin->m_bEnabled = enabled;
    return !MessageReadUnderflow(1);
}

// NONMATCH: allocation/EH zero/one register reuse differs; three shapes tried.
// FUNCTION: SWKOTOR 0x00652D60
int CSWCMessage::HandleServerToPlayerMapPinReferenceNumber()
{
    Vector position = {0.0f, 0.0f, 0.0f};
    CExoString note;
    if (MessageReadOverflow(1)) return 0;
    position.x = ReadFLOAT(1.0f, 0x20);
    position.y = ReadFLOAT(1.0f, 0x20);
    position.z = ReadFLOAT(1.0f, 0x20);
    note = ReadCExoString(0x20);
    unsigned long reference = ReadDWORD(0x20);
    if (MessageReadUnderflow(1)) return 0;
    CSWCArea* area = g_pAppManager->m_pClientExoApp->GetModule()->m_pArea;
    CSWCAreaObject* pin = new CSWCAreaObject;
    pin->SetPosition(position);
    pin->m_sNote = note;
    pin->m_nReference = reference;
    pin->m_bReference = 1;
    pin->m_bEnabled = 1;
    area->AddObject(pin);
    return 1;
}
