#pragma once
#include "framework.h"

class CPlayer : public CCharacter
{
private:
	
	int* inventory;
	unsigned int ID;// 직업
	playerState state = playerState:: idle;
	int CollsionY;
	float tick; // 현재 틱
	float Maxtick; // 이미지가 바뀔 틱
	bool IsRopeMove{ false }; // 로프 움직임 유무
    int CollsionX;
	POINT point; // 위치
  	//POINT WorldPoint; // 월드상의 위치
	int count{ 0 }; // 현재 그릴 이미지 순서
	int maxCount{ 0 }; // 해당 상태 이미지 최대
	int startjumpy = 0; // 점프 시작위치
	int ImgX = 0, ImgY = 70; // 이미지 그릴 위치
	int ImgWidth, ImgHeight; // 이미지 너비 높이
    int WidthCorrectionValue;
	bool IsAttack{ false };
    bool IsVisible{ true };
    int hitTick;
	Gdiplus::Rect AttackRect;
	Gdiplus::Rect rect;
    CImage* deathImg;
    CImage* levelUpImg;
    void hit();
    BOOL moveMap = FALSE;
public:
    BOOL GetMoveMap() { return moveMap; }
    void SetMoveMap(BOOL moveMap) { this->moveMap = moveMap; }
    int mapNum;
    float playerHitTime;
	POINT GetPoint() { return point; };
	void SetPoint(POINT point) {
		this->point.x = point.x; this->point.y = point.y;
	}

	void Draw(HDC& hdc);
	void Init(int id);	
    void DrawIdle();
	void DrawMove();
	void DrawJump();
    void DrawRadder();
    void DrawRope();

	void DrawDeath();
	void DrawSpawn();

    void LevelUp();
	void DrawSit();
    void DrawAttack();
    void DrawSkill1();
    void DrawSkill2();
    void DrawSkill3();
    void DrawSkill4();
    
    
    void idle();
    void move();
    void jump();
    void jumpMove();
    void rope();
    void radder();
    void sit();
    void attack();
    void drop();
    void jumpAttack();
    void jumpMoveAttack();
    void skill1();
    void skill2();
    void skill3();
    void death();
    void spawn();

    void Hit(int damage);

	void KeyCheck();
	void Update();
	void JumpUpdate();
	void StateUpdate();
	bool CollsionCheck(MapType mapType);
    bool CollsionCheckToMonster();
	bool IsCollsion(Gdiplus::Rect rect1, Gdiplus::Rect rect2);
    bool IwCollsinToWall(); 
	CPlayer() = default;
    void DrawSence(HDC& hdc);
    void CountUpdate();

    void SetId(int id) { this->ID = id; }
    int GetId() { return ID; }

    playerState GetState() {return state; }
    void SetState(playerState playerstate) { state = playerstate; }
    int GetCount() { return count; }
    void SetCount(int count) { this->count = count; }

    bool GetIsVisible() { return IsVisible; }
    void SetIsVisible(bool visible) { IsVisible = visible; }

    bool PointIsin(POINT point)
    {
        if (point.x > this->point.x && point.x < this->point.x + width)
        {
            if (point.y > this->point.y && point.y < this->point.y + height)
            {
                return true;
            }
        }
        return false;
    }
};

