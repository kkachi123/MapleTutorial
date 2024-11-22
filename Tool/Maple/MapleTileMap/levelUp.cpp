#include "levelUp.h"

void LevelUpEffect::Draw(HDC hdc)
{
    int bx = LevelUpImage[curFrame/4]->GetWidth();
    int by = LevelUpImage[curFrame/4]->GetHeight();
    int valY = 0;
    int drawY = 0;
    if (curFrame / 4 > 6)
        drawY = 100;
    int valX = 0;
    if (curFrame / 4 > 11)
    {
        valX = 900;
        valY = 800;
        
    }
    LevelUpImage[curFrame/4]->TransparentBlt(hdc, point.x -110 - valX/2, point.y- 320 - drawY -valY/2, 300+valX, 400+valY, 0, 0, bx, by, NULL);
   
    curFrame++;

    if (curFrame/4 >= maxFrame)
    {
        curFrame = 0;
        IsEnd = TRUE;
        IsStart = false;
    }
}

void LevelUpEffect::Init()
{

    maxFrame = 21;
    TCHAR filePath[256];
    IsStart = false;
    for (int i = 0; i <= maxFrame; i++)
    {
        wsprintf(filePath, L"../Data/img/levelUp/LevelUp.%d.png", i);
        CImage* img = new CImage();
        img->Load(filePath);
        LevelUpImage.push_back(img);
    }

 
}