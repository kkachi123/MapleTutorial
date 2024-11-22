#pragma once
#include "framework.h"

class Snail2 : public CMonster
{
	private:
		int collsionYStart;
		int idleCount;
		
        
	public:
        Sanil2Type monstertype;

		Snail2(int type = 0);
		~Snail2();

		void init(POINT drawPoint,int exp = 4, int hp = 20, int Damage = 3, int Defense = 0);
		void Update(float deltaTime);
		void Idle();
		void Move();
		void Hit(int damage);
		void Death();
		void Spawn();
		void Draw(HDC& hdc , BOOL openGuide);
};

