#pragma once
#include "framework.h"

class Camera
{
private:
	POINT pos = { 0, 0 };
	long width = 100;
	long height = 100;
public:
	Camera(POINT pos = { 0, 0 }, long width = 100 , long height = 100) : pos(pos), width(width), height(height)
	{}
	POINT GetPos() { return pos; }
	long GetWidth() { return width; }
	long GetHeight() { return height; }
	void SetPos(POINT pos) { this->pos = pos; }
	void SetWidth(long width);
	void SetHeight(long height);
};

// : background image
extern CImage hBackImage;
//extern HBITMAP hBackImage;
extern BITMAP bitBack;

// >> : ui image
extern CImage hUIImage;
extern CImage hMonsterIcon;
extern CImage hMonsterHp;
// << : ui image

extern float zoomRatio;
extern int mapWidth;
extern int mapHeight;

void CreateBitmap(int imageLoad);
