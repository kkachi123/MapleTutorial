#pragma once


enum class SkillType
{
	three_snail,
    double_step,
    genesis,


    slash_blast,



};
enum class SkillState
{
	idle,
	move,
	hit,

};

enum class playerState
{

    idle,
    move,
    jump,
    jumpMove,
    rope,
    radder,
    sit,
    attack,
    drop,
    jumpAttack,
    jumpMoveAttack,
    skill1,
    skill2,
    skill3,
    death,
    hit,
};


enum class moveState
{
    idle,
    move,
    jump,
    landing,
    death,
    spawn,
    attack,
    hit,
};

enum class MonsterType
{
    NONE,
    Snail1,
    Snail2,
    Snail3,
    MushMom,

};

enum class MapType
{
    Platform,
    Wall,
    Rope,
    Ladder,
    Attack,
};


enum class Sanil2Type
{
    blue,
    red,
};

enum class SoundType
{
    Intro,
    lobby,
    button,
    play,
    monsterHit,
    monsterDead,
};

enum class BillageType
{
    Henesys,
    Monster,
    Boss,
};

enum class NPCType
{
    Hinz,
    Lucas,
};
#define HENESYS L"../Data/img/map/henesys/henesys.png"
#define HENESYSBACK L"../Data/img/map/henesys/henesys_background_1.jpg"

#define PLAYERATTACK L"../Data/sound/player/Attack.wav"
#define PLAYERJUMP L"../Data/sound/player/Jump.wav"
#define PLAYERDEATH L"../Data/sound/player/Dead.wav"
#define LEVELUP L"../Data/sound/player/LevelUp.wav"
#define PLAYERMAGIC L"../Data/sound/player/Magic.wav"

#define ENDING L"../Data/sound/scene/QuestClear.wav" 
