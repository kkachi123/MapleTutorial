#include "Billage.h"
#include "TileMap.h"
void Billage::Create()
{
    billageImage.Load(HENESYS);
    GetObject(billageImage, sizeof(BITMAP), &bitBack);
    billageBackGround.Load(HENESYSBACK);
}
extern Camera camera;
extern float zoomRatio;
void Billage::Draw(HDC hdc)
{
    int bx = billageImage.GetWidth() ;
    int by = billageImage.GetHeight();
    //billageBackGround.Draw(hdc, -camera.GetPos().x, -camera.GetPos().y, billageBackGround.GetWidth() * zoomRatio, billageBackGround.GetHeight() * zoomRatio, 0, 0, billageBackGround.GetWidth(), billageBackGround.GetHeight());
    //billageImage.TransparentBlt(hdc, -camera.GetPos().x, -camera.GetPos().y, bx * zoomRatio, by * zoomRatio, 0, 0, billageImage.GetWidth(), billageImage.GetHeight(), RGB(255, 255, 255));
    billageImage.Draw(hdc, -camera.GetPos().x, -camera.GetPos().y, bx * zoomRatio, by * zoomRatio, 0, 0, billageImage.GetWidth(), billageImage.GetHeight());
}

void Billage::SoundPlay()
{
}

void Billage::StopSound()
{
}
