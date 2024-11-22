#include "framework.h"
#include "SoundManager.h"

#include "CPlayer.h"
extern CPlayer player;
void CMonster::GetExp()
{
    player.AddExp(exp*100);
}
// 공통으로 내는 사운드는 PlaySound로 처리
void CMonster::playDamageSound()
{
    PlaySound(MONSTERHIT, NULL, SND_FILENAME | SND_ASYNC);
    /*MCI_OPEN_PARMS DamageSound;
    DamageSound.lpstrElementName = MONSTERHIT;
    DamageSound.lpstrDeviceType = L"mpegvideo";
    mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&DamageSound);
    DamageSoundID = DamageSound.wDeviceID;

    mciSendCommand(DamageSoundID, MCI_PLAY, 0, (DWORD)(LPVOID)&DamageSound);*/
}

void CMonster::playDeathSound()
{
    if(monsterType == MonsterType::MushMom)
        PlaySound(MUSHMOMDEAD, NULL, SND_FILENAME | SND_ASYNC);
    else
        PlaySound(MONSTERDEAD, NULL, SND_FILENAME | SND_ASYNC);
}

