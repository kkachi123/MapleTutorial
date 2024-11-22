#include "Camera.h"

// 맵 확대 비율 100% 1200x800 기준
float zoomRatio = 1.0f;
int mapWidth = 1200;
int mapHeight = 800;
// : background image
CImage hBackImage;
TCHAR hBackImageFilename[100];
//HBITMAP hBackImage;
BITMAP bitBack;
CImage hUIImage;
CImage hMonsterIcon;
CImage hMonsterHp;


void Camera::SetWidth(long width)
{
	this->width = width; //rect.X = 0; rect.Width = width;
}
void Camera::SetHeight(long height)
{
	this->height = height; //rect.Y = 0; rect.Height = height;
}

void CreateBitmap(int imageLoad)
{
    // >> background image
    switch (imageLoad)
    {
    case 0:
        _tcscpy_s(hBackImageFilename, HENESYS);
        break;
    case 1:
        _tcscpy_s(hBackImageFilename, _T("../Data/map.bmp"));
        break;
    case 2:
        _tcscpy_s(hBackImageFilename, _T("../Data/Temple.bmp"));
        break;
    default:
        break;
    }
    //hBackImage = (HBITMAP)LoadImage(NULL, hBackImageFilename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    hBackImage.Load(hBackImageFilename);
    if (hBackImage == NULL)
    {
        DWORD dwError = GetLastError();
        MessageBox(NULL, _T("배경 이미지 파일을 열 수 없습니다."), _T("에러"), MB_OK);
    }
    else
    {
        GetObject(hBackImage, sizeof(BITMAP), &bitBack);
    }
    // <<
    
    hUIImage.Load(_T("../Data/img/UI/MainUI.bmp"));
    hMonsterIcon.Load(_T("../Data/img/UI/MushMom.bmp"));
    hMonsterHp.Load(_T("../Data/img/UI/HPBar.bmp"));
   
}