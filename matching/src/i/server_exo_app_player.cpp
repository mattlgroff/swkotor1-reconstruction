// CServerExoApp members that call other CServerExoApp members out of
// line (kept in their own file so /Ob2 does not inline the callees).
#include "i/server_exo_app.h"

// FUNCTION: SWKOTOR 0x004B2820
CSWSCreature* CServerExoApp::GetPlayerCreature()
{
    return GetCreatureByGameObjectID(GetPlayerCreatureID());
}
