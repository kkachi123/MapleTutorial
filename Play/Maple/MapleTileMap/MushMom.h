#pragma once
#include "framework.h"

class EffectAnimation;
class MushMom : public CMonster
{
	private:
        int AttackStandSoundID = -1;
        int AttackSoundID = -1;

		int idleCount;
		int rayFloor;
		int landingPosY;
		
		// >> : deathAnimation
		CImage hMonsterDeath;
		
	public:
        void playAttackStandSound();
        void playAttackSound();
        void stopSound(int id);

        int deathSpriteSizeX;
        int deathSpriteSizeY;

        int deathBitRow;
        int deathBitCol;

        int deathBitColMin;
        int deathBitColMax;

        BOOL deathDrawForward;
        // << : deathAnimation

        EffectAnimation* attackEffect;
        EffectAnimation* attackLandingEffect;

		MushMom();
		~MushMom();
        float GetMaxHp() { return maxHp; }
		void init(POINT drawPoint);
		void Update(float deltaTime);
		void Idle();
		void Move();
		void Attack();
        void Hit(int damage);
        void Landing();
		void Death();
		void Spawn();
		void Draw(HDC& hdc , BOOL openGuide);
};

