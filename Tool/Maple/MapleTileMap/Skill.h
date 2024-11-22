#pragma once
#include "framework.h"

class CMonster;
class Snail1;
class Skill :   public CObject
{
private :
	
	SkillType type;
	bool IsHit{ false };
	int SkillWidth{ 100 };
	POINT startPoint;
	POINT endPoint;
	CImage skillImg;
	BITMAP skillBitmap;
	CImage HitImg;
	BITMAP HitBitmap;
    
    CImage skillImg_L;
    CImage skillImg_R;

	CImage* DrawImg;
	SkillState state;
    
	int srcX, srcY, srcWidth, srcHeight;
	bool EndState;
	bool IsEndPoint{ false };
	int ThreeSnailRandNum{ 0 }; // ÃÊ »¡ ÆÄ ·£´ý

public :
    float Damage;
	Skill();
	std::vector<CMonster*> hitMonsters; // È÷Æ® ¸ó½ºÅÍ
	Snail1* sanil; // debug
	bool IsStart;
	bool Ishit;
	void SetStartPoint(POINT point) { startPoint.x = point.x; startPoint.y = point.y; this->point.x = point.x, this->point.y = point.y; }
	void SetEndPoint(POINT point) { endPoint.x = point.x; endPoint.y = point.y; }
	void AddHitMonster(CMonster* monster) { hitMonsters.push_back(monster); }
	void Init();
	void SetThreeSnailRandNum();
	void Draw(HDC hdc);
	void Move();
	
    SkillType GetSkillType() { return type; }
	void SetSkillType(SkillType type) { this->type = type; }

	void SetThreeSnailRandNum(int num) { ThreeSnailRandNum = num; }
	void SetState(SkillState state) { this->state = state; }
	int GetSkillWidth() { return SkillWidth; }

	void DrawThreeSnail(Gdiplus::Rect& rect);
    void DrawDoubleStep(Gdiplus::Rect& rect);
    void DrawSlashBlast(Gdiplus::Rect& rect);
	void Update();
	void stateUpdate();
	void Hit();
	void Idle();
};

