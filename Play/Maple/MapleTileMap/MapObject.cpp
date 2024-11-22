#include "framework.h"
#include "TileMap.h"
#include "SoundManager.h"

MCI_OPEN_PARMS portalSound;
void MapObject::Update()
{
    timeCount++;
    if(timeCount == updateCount)
    {
        timeCount = 0;
        bitCol++;
        if (bitCol > bitColMax)
        {
            bitCol = bitColMin;
        }
    }
}

void MapObject::Draw(HDC hdc)
{
    objectImage.TransparentBlt(hdc, (drawPoint.x - mapMove.x / zoomRatio) * zoomRatio, (drawPoint.y - mapMove.y / zoomRatio) * zoomRatio, spriteWidth * zoomRatio, spriteHeight * zoomRatio, bitCol * spriteWidth, bitRow * spriteHeight, spriteWidth, spriteHeight, RGB(255, 0, 255));
}

void MapObject::SoundPlay()
{
    PlaySound(PORTAL, NULL, SND_ASYNC);
}

BOOL MapObject::PointIsIn(POINT point)
{
    if (point.x >= drawPoint.x && point.x <= drawPoint.x + spriteWidth && point.y >= drawPoint.y && point.y <= drawPoint.y + spriteHeight)
        return TRUE;
    return FALSE;
}
