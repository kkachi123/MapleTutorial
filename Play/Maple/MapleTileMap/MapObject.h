#pragma once
#include "framework.h"

class MapObject
{
private:
    int portalSoundID;
    CImage objectImage;
    POINT drawPoint;
    int bitCol;
    int bitRow;
    int bitColMin;
    int bitColMax;
    int spriteWidth;
    int spriteHeight;
    int updateCount;
    int timeCount;
public:
    MapObject(int bitCol = 0, int bitRow = 0, int bitColMin = 0, int bitColMax = 7, int spriteWidth = 0, int spriteHeight = 0, POINT drawPoint = { 0,0 },int updateCount = 0)
    {
        this->portalSoundID = 0;
        this->bitCol = bitCol;
        this->bitRow = bitRow;
        this->bitColMin = bitColMin;
        this->bitColMax = bitColMax;
        this->spriteWidth = spriteWidth;
        this->spriteHeight = spriteHeight;
        this->drawPoint = drawPoint;
        this->updateCount = updateCount;
        this->timeCount = 0;
        objectImage.Load(L"../Data/img/map/portal.bmp");
    }
    ~MapObject() 
    {
        objectImage.Destroy();
    }
    POINT GetDrawPoint() { return drawPoint; }
    void SetDrawPoint(POINT drawPoint) { this->drawPoint = drawPoint; }
    int GetSpriteWidth() { return spriteWidth; }
    int GetSpriteHeight() { return spriteHeight; }
    void Update();
    void Draw(HDC hdc);
    void SoundPlay();
    BOOL PointIsIn(POINT point);
};