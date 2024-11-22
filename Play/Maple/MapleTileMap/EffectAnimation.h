#pragma once
#include "framework.h"
class EffectAnimation
{
private:
    // >> : landing 
    BOOL generate;
    // << : landing
    
    // >> : draw variable
    CImage hEffect;
    int spriteSizeX;
    int spriteSizeY;

    

    

    // << : draw variable

    // >> : chose direction
    BOOL isLeft;
    int leftBitRow;
    int leftBitColMin;
    int leftBitColMax;

    int rightBitRow;
    int rightBitColMin;
    int rightBitColMax;
    // << : chose direction
public:
    int bitRow;
    int bitCol;
    int bitColMin;
    int bitColMax;

    EffectAnimation(wchar_t* filename , BOOL isLeft , int sizeX , int sizeY ,int leftBitRow , int leftBitColMin , int leftBitColMax , int rightBitRow ,int rightBitColMin , int rightBitColMax)
        : bitRow(0) , bitCol(0) , bitColMin(0) , bitColMax(0)
    {
        generate = FALSE;
        hEffect.Load(filename);
        this->isLeft = isLeft;
        this->spriteSizeX = sizeX;
        this->spriteSizeY = sizeY;
        this->leftBitRow = leftBitRow;
        this->leftBitColMin = leftBitColMin;
        this->leftBitColMax = leftBitColMax;
        this->rightBitRow = rightBitRow;
        this->rightBitColMin = rightBitColMin;
        this->rightBitColMax = rightBitColMax;
    }
    ~EffectAnimation();
    EffectAnimation() = default;
    BOOL GetGenerate() { return generate; }
    void SetGenerate(BOOL generate) { this->generate = generate; }
    void SetIsLeft(BOOL isLeft) { this->isLeft = isLeft; }
    void SettingEffectBit();
    void GenrateEffect();
    void Draw(HDC& hdc , POINT drawPoint , float zoomRatio);
};
