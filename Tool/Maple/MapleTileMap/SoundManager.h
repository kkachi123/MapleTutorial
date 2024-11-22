#pragma once
#include "framework.h"
// sound
#include <stdio.h>
#include <conio.h>
#pragma comment (lib, "winmm.lib")
#include <mmsystem.h>
#include <Digitalv.h>
// sound    

extern MCI_OPEN_PARMS openBgm;
extern MCI_OPEN_PARMS openEffect;
extern int dwID;
extern int EffectID;

void playingBgm(SoundType mode);
void stopBgm(int dwID);
void playingEffect(SoundType mode);
void stopEffect(int EffectID);

#define INTRO L"../Data/sound/scene/intro.wav"
#define LOBBY L"../Data/sound/scene/Start.wav"
#define PLAY L"../Data/sound/scene/Play.wav"
#define BUTTON L"../Data/sound/Button.wav"

#define MONSTERHIT L"../Data/sound/monster/MonsterDamaged.wav"
#define MONSTERDEAD L"../Data/sound/monster/MonsterDead.wav"
#define MUSHMOMDEAD L"../Data/sound/monster/mushMomDead.wav"
#define MUSHMOMATTACK L"../Data/sound/monster/mushMomAttack.wav"
#define MUSHMOMATTACKSTAND L"../Data/sound/monster/mushMomAttackStand.wav"

#define PORTAL L"../Data/sound/scene/Portal.wav"

