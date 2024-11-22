#pragma once
#include "framework.h"

class CMonster : public CCharacter
{
protected:
    BOOL isAlive;
    // : >> Image


// : >> Image
    CImage hMonster;
    //HBITMAP hMonster;
    BITMAP bitAnime;
    int spriteSizeX;
    int spriteSizeY;


    // : << Image




    float moveTick;

    virtual void Update() {}
    virtual void Init() {}
    virtual void Idle() {}
    virtual void Move() {}
    virtual void MoveLeft() {}
    virtual void MoveRight() {}
    virtual void Jump() {}
    virtual void Death() {}
    virtual void Spawn() {}
    virtual void Attack() {}
    virtual void Draw(HDC& hdc) {}

    MonsterType monsterType;
    Gdiplus::Rect rect;


public:

    int bitRow;
    int bitCol;
    int bitColMin;
    int bitColMax;
    BOOL drawForward;

    POINT drawPoint;
    POINT spawnPoint;
    moveState update;
    moveState prevUpdate;

    
    void playDamageSound();
    void playDeathSound();
    void Tracking(POINT playerPoint)
    {
        if (drawPoint.x < playerPoint.x)
            IsLeft = false;
        else
            IsLeft = true;
    }
    virtual void Hit(int damage) {}
    void GetExp();
    BOOL PointIsIn(POINT point)
    {
        if (point.x >= drawPoint.x && point.x <= drawPoint.x + collisionX)
        {
            if (point.y >= drawPoint.y && point.y <= drawPoint.y + collisionY)
            {
                return TRUE;
            }
        }
        return FALSE;
    }
    void setState(moveState s)
    {
        update = s;
    }
    moveState GetState()
    {
        return update;
    }
    void SetMonsterType(MonsterType type) { monsterType = type; }
    MonsterType GetMonsterType() {
        return monsterType;
    }
    Gdiplus::Rect GetCollider()
    {
        rect.X = drawPoint.x;
        rect.Y = drawPoint.y;
        rect.Width = spriteSizeX;
        rect.Height = spriteSizeY;
        return rect;
    };


    void SetDrawPoint(POINT draw) { drawPoint = draw; }
    void SetspawnPoint(POINT spawn) { spawnPoint = spawn; }

    POINT GetDrawPoint() { return drawPoint; }
    POINT GetspawnPoint() { return spawnPoint; }


    BOOL GetIsAlive() { return isAlive; }
    void SetIsAlive(BOOL al) { isAlive = al; }
    CMonster() = default;
    CMonster(const BOOL& isAlive, const CImage& hMonster, const BITMAP& bitAnime, const MonsterType& monsterType, int spriteSizeX, int spriteSizeY, int bitRow, int bitCol, int bitColMin, int bitColMax, const BOOL& drawForward, const POINT& drawPoint, const POINT& spawnPoint, const moveState& update, float moveTick, const Gdiplus::Rect& rect)
        :isAlive(isAlive), hMonster(hMonster), bitAnime(bitAnime), monsterType(monsterType), spriteSizeX(spriteSizeX), spriteSizeY(spriteSizeY), bitRow(bitRow), bitCol(bitCol), bitColMin(bitColMin), bitColMax(bitColMax), drawForward(drawForward), drawPoint(drawPoint), spawnPoint(spawnPoint), prevUpdate(update), update(update), moveTick(moveTick), rect(rect)
    {

    }

};

