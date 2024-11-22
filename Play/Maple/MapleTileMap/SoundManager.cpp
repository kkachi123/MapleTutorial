#include "SoundManager.h"
MCI_OPEN_PARMS openBgm;
MCI_OPEN_PARMS openEffect;

int dwID;
int EffectID;

void playingBgm(SoundType mode)
{
    switch (mode)
    {
    case SoundType::lobby:
        openBgm.lpstrElementName = LOBBY;
        break;
    case SoundType::play:
        openBgm.lpstrElementName = PLAY;
        break;
    }

    openBgm.lpstrDeviceType = L"mpegvideo";
    mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&openBgm);
    dwID = openBgm.wDeviceID;
    mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&openBgm); // 반복 재생
}
void stopBgm(int dwID)
{
    mciSendCommand(dwID, MCI_CLOSE, 0, (DWORD)(LPVOID)NULL);
}

void playingEffect(SoundType mode)
{
    switch (mode)
    {
    case SoundType::Intro:
        openEffect.lpstrElementName = INTRO;
        break;
    case SoundType::button:
        openEffect.lpstrElementName = BUTTON;
        break;
    case (SoundType)5:
        openEffect.lpstrElementName = LEVELUP;
        break;
    case (SoundType)6:
    {
        if (EffectID != 0)
            stopEffect(EffectID);
        openEffect.lpstrElementName = PLAYERDEATH;
    }
        break;
    }
    openEffect.lpstrDeviceType = L"mpegvideo";
    mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&openEffect);
    EffectID = openEffect.wDeviceID;
    mciSendCommand(EffectID, MCI_PLAY, 0, (DWORD)(LPVOID)&openEffect);
}
void stopEffect(int EffectID)
{
    mciSendCommand(EffectID, MCI_CLOSE, 0, (DWORD)(LPVOID)NULL);
}