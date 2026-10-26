#include "m/sw_c_message_handlers.h"

// FUNCTION: SWKOTOR 0x00654380
int CSWCMessage::HandleServerToPlayerAmbientMusicPlay()
{
    if (MessageReadOverflow(1)) return 0;
    int value = ReadBOOL();
    if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
    g_pAppManager->m_pClientExoApp->GetModule()->m_pArea->m_pAmbientSound->PlayMusic(value);
    return 1;
}

// FUNCTION: SWKOTOR 0x006543E0
int CSWCMessage::HandleServerToPlayerAmbientMusicSetDelay()
{
    if (MessageReadOverflow(1)) return 0;
    int value = ReadDWORD(0x20);
    if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
    g_pAppManager->m_pClientExoApp->GetModule()->m_pArea->m_pAmbientSound->SetMusicDelay(value);
    return 1;
}

// FUNCTION: SWKOTOR 0x006544D0
int CSWCMessage::HandleServerToPlayerAmbientBattleMusicPlay()
{
    if (MessageReadOverflow(1)) return 0;
    int value = ReadBOOL();
    if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
    g_pAppManager->m_pClientExoApp->GetModule()->m_pArea->m_pAmbientSound->PlayBattleMusic(value);
    return 1;
}

// FUNCTION: SWKOTOR 0x00654530
int CSWCMessage::HandleServerToPlayerAmbientBattleMusicChange()
{
    if (MessageReadOverflow(1)) return 0;
    int value = ReadDWORD(0x20);
    if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
    g_pAppManager->m_pClientExoApp->GetModule()->m_pArea->m_pAmbientSound->SetBattleMusicTrack(value);
    return 1;
}

// FUNCTION: SWKOTOR 0x006545A0
int CSWCMessage::HandleServerToPlayerAmbientSoundLoopPlay()
{
    if (MessageReadOverflow(1)) return 0;
    int value = ReadBOOL();
    if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
    g_pAppManager->m_pClientExoApp->GetModule()->m_pArea->m_pAmbientSound->PlayAmbientSound(value);
    return 1;
}

// NONMATCH: register allocation and shared receiver/vtable scheduling, three shapes tried.
// FUNCTION: SWKOTOR 0x00654450
int CSWCMessage::HandleServerToPlayerAmbientMusicChangeTrack()
{
    if (MessageReadOverflow(1)) return 0;
    int day = ReadBOOL();
    int value = ReadDWORD(0x20);
    if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
    CSWCAmbientSound* ambient = g_pAppManager->m_pClientExoApp->GetModule()->m_pArea->m_pAmbientSound;
    if (day) ambient->SetMusicDayTrack(value);
    else ambient->SetMusicNightTrack(value);
    return 1;
}

// NONMATCH: register allocation and shared receiver/vtable scheduling, three shapes tried.
// FUNCTION: SWKOTOR 0x00654600
int CSWCMessage::HandleServerToPlayerAmbientSoundLoopChange()
{
    if (MessageReadOverflow(1)) return 0;
    int day = ReadBOOL();
    int value = ReadDWORD(0x20);
    if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
    CSWCAmbientSound* ambient = g_pAppManager->m_pClientExoApp->GetModule()->m_pArea->m_pAmbientSound;
    if (day) ambient->SetAmbientDayTrack(value);
    else ambient->SetAmbientNightTrack(value);
    return 1;
}

// NONMATCH: register allocation and shared receiver/vtable scheduling, three shapes tried.
// FUNCTION: SWKOTOR 0x00654680
int CSWCMessage::HandleServerToPlayerAmbientSoundVolume()
{
    if (MessageReadOverflow(1)) return 0;
    int day = ReadBOOL();
    int value = ReadDWORD(0x20);
    if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
    CSWCAmbientSound* ambient = g_pAppManager->m_pClientExoApp->GetModule()->m_pArea->m_pAmbientSound;
    if (day) ambient->SetAmbientDayVolume(value);
    else ambient->SetAmbientNightVolume(value);
    return 1;
}

// FUNCTION: SWKOTOR 0x00665510
int CSWCMessage::HandleServerToPlayerAmbient(unsigned char minor)
{
    switch (minor) {
    case 1: return HandleServerToPlayerAmbientMusicPlay();
    case 2: return HandleServerToPlayerAmbientMusicSetDelay();
    case 3: return HandleServerToPlayerAmbientMusicChangeTrack();
    case 4: return HandleServerToPlayerAmbientBattleMusicPlay();
    case 5: return HandleServerToPlayerAmbientBattleMusicChange();
    case 6: return HandleServerToPlayerAmbientSoundLoopPlay();
    case 7: return HandleServerToPlayerAmbientSoundLoopChange();
    case 8: return HandleServerToPlayerAmbientSoundVolume();
    default: return 0;
    }
}
