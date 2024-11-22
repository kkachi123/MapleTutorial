#pragma once
#include "framework.h"

class Billage
{
private:
    CImage billageImage;
    CImage billageBackGround;
public:
    Billage() {}
    ~Billage() 
    {
        billageImage.Destroy();
        billageBackGround.Destroy();
    }
    void Create();
    void Draw(HDC hdc);
    void SoundPlay();
    void StopSound();
};