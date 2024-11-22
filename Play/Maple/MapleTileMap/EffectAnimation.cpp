#include "EffectAnimation.h"

EffectAnimation::~EffectAnimation()
{
   hEffect.Destroy();
}

void EffectAnimation::SettingEffectBit()
{
    if(isLeft)
    {
        bitRow = leftBitRow;
        bitColMin = leftBitColMin;
        bitColMax = leftBitColMax;
        bitCol = leftBitColMin;
    }
    else
    {
        bitRow = rightBitRow;
        bitColMin = rightBitColMin;
        bitColMax = rightBitColMax;
        bitCol = rightBitColMin;
    }
}

void EffectAnimation::GenrateEffect()
{
    if (isLeft)
    {
        bitCol++;
        if (bitCol >= bitColMax)
        {
            bitCol = bitColMin;
        }
    }
    else
    {
        bitCol--;
        if (bitCol <= bitColMax)
        {
            bitCol = bitColMin;
        }
    }

    
}

void EffectAnimation::Draw(HDC& hdc , POINT drawPoint , float zoomRatio)
{
    hEffect.TransparentBlt(hdc, drawPoint.x, drawPoint.y, spriteSizeX * zoomRatio, spriteSizeY * zoomRatio, bitCol * spriteSizeX, bitRow * spriteSizeY, spriteSizeX, spriteSizeY, RGB(255, 0, 255));
}