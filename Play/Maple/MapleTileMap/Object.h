#pragma once
#include "InputMgr.h"


class CObject
{
protected:
	Gdiplus::Bitmap* bitmap_Left; // 좌
	Gdiplus::Bitmap* bitmap_Right; //우
	Gdiplus::Bitmap* bitmap; // 그릴 비트맵

    CImage* img_L{nullptr};
    CImage* img_R{nullptr};
    CImage* drawImg;
	int bitmapWidth; // 비트맵 크기
	int width, height;// 콜라이더 너비,높이
	POINT point;
	float tick; // 현재 틱
	float Maxtick; // 이미지가 바뀔 틱

	int count{ 0 }; // 현재 그릴 이미지 순서
	bool IsLeft{ false }; // 좌우
	virtual void Draw(HDC& hdc) {};
	virtual void Sound() {};
	virtual void Init() {};
public:

	void SetPoint(POINT point) {
		this->point.x = point.x;
		this->point.y = point.y;
	}
    POINT GetPoint() { return point; }
	float GetWidth() { return width; }
	void SetWidth(int width) { this->width = width; }

	float GetHeight() { return height; }
	void SetHeight(int height) { this->height = height; }
	
	void SetIsLeft(bool left) { IsLeft = left; }
	bool GetIsLeft() { return IsLeft; }

	
};



