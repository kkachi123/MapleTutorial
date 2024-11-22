#pragma once
#include "framework.h"

class FinalAttack
{
private:
    vector<CImage*> AttackImage;
    
    int attackFrame;
    int maxFrame;
    int attackType;
    BOOL attackStart;
    BOOL attackEnd;
    POINT point;
   
public:
    std::vector<CMonster*> hitMonsters; // 히트 몬스터
    float Damage;
    FinalAttack() 
    {
        attackType = 0;
        attackFrame = 0;
        maxFrame = 0;
        attackStart = FALSE;
        attackEnd = TRUE;
    }
    void Init(int attackType);
    void Draw(HDC hdc);
    void SetPoint(POINT point) { this->point = point; }
    void SetAttackFrame(int attackFrame) { this->attackFrame = attackFrame; }
    BOOL GetAttackEnd() { return attackEnd; }
    void SetAttackEnd(BOOL attackEnd) { this->attackEnd = attackEnd; }
    void SetAttackStart(BOOL attackStart) { this->attackStart = attackStart; }
    BOOL GetAttackStart() { return attackStart; }
};