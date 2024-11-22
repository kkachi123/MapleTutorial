#pragma once
#include "framework.h"

class Snail1 : public CMonster
{
	private: 
		int collsionYStart;
		int idleCount;
		
	public:
		Snail1();
		~Snail1();
		
		void init(POINT drawPoint);
		void Update(float deltaTime);
		void Idle();
		void Move();
        void Hit(int damage);
		void Death();
		void Spawn();
		void Draw(HDC& hdc , BOOL openGuide);
        Snail1(int collsionYStart, int idleCount)
            : collsionYStart(collsionYStart), idleCount(idleCount) {};
};

