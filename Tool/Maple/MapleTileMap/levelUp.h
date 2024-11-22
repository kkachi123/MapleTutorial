#pragma once
#include "framework.h"
class LevelUpEffect
{
private:
    vector<CImage*> LevelUpImage;

    int curFrame;
    int maxFrame;
    BOOL IsStart;
    BOOL IsEnd;
    POINT point;

public:
    void Draw(HDC hdc);
    void Init();

    BOOL GetIsStart() { return IsStart; }
    void SetIsStart(bool start) { IsStart = start; }
    void SetPoint(POINT point) { this->point = point; }

};