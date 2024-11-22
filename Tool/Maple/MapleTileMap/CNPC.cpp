#include "framework.h"
#include "TileMap.h"
CNPC::CNPC(NPCType npcType, POINT drawPoint)
{
    this->npcType = npcType;
    this->drawPoint = drawPoint;
    isTalk = FALSE;
    moveTick = 0;
    switch (npcType)
    {
        case NPCType::Hinz:
        {
            hNPC.Load(L"../Data/img/UI/NPC/Hinz.png");
            spriteSizeX = 90;
            spriteSizeY = 160;

            bitRow = 0;
            bitCol = 0;
            bitColMin = 0;
            bitColMax = 2;
            drawForward = TRUE;
            CImage* talk1 = new CImage();
            talk1->Load(L"../Data/img/UI/NPC/Hinz1.png");
            npcTalk.push_back(talk1);
            talkScreenIndex = 0;
        }
        break;
        case NPCType::Lucas:
        {
            hNPC.Load(L"../Data/img/UI/NPC/Lucas.png");
            spriteSizeX = 55;
            spriteSizeY = 75;

            bitRow = 0;
            bitCol = 0;
            bitColMin = 0;
            bitColMax = 1;
            drawForward = TRUE;
            CImage* talk1 = new CImage();
            talk1->Load(L"../Data/img/UI/NPC/Lucas1.bmp");
            npcTalk.push_back(talk1);
            CImage* talk2 = new CImage();
            talk2->Load(L"../Data/img/UI/NPC/Lucas2.bmp");
            npcTalk.push_back(talk2);
            talkScreenIndex = 0;
        }
        break;
    }
}

void CNPC::Update(float deltaTime)
{
    moveTick += deltaTime;
    if(moveTick > 0.2)
    {
        if (drawForward)
        {
            bitCol++;
            if (bitCol > bitColMax)
            {
                bitCol = bitColMax;
                drawForward = FALSE;
            }
        }
        else
        {
            bitCol--;
            if (bitCol < bitColMin)
            {
                bitCol = bitColMin;
                drawForward = TRUE;
            }
        }
        moveTick = 0;
    }
}

void CNPC::Draw(HDC& hdc)
{
    int screenX = (drawPoint.x - mapMove.x / zoomRatio) * zoomRatio;
    int screenY = (drawPoint.y - mapMove.y / zoomRatio) * zoomRatio;
    hNPC.TransparentBlt(hdc, screenX, screenY, spriteSizeX * zoomRatio, spriteSizeY * zoomRatio, bitCol * spriteSizeX, bitRow * spriteSizeY, spriteSizeX, spriteSizeY, RGB(255, 0, 255));
    
}
void CNPC::DrawTalk(HDC& hdc)
{
    if (isTalk)
    {
        int bx = npcTalk[talkScreenIndex]->GetWidth();
        int by = npcTalk[talkScreenIndex]->GetHeight();
        npcTalk[talkScreenIndex]->TransparentBlt(hdc, 500, 200, bx, by, 0, 0, bx, by, RGB(255, 0, 255));
    }
}

BOOL CNPC::PointIsIn(POINT point)
{
    if (point.x >= drawPoint.x && point.x <= drawPoint.x + spriteSizeX)
    {
        if (point.y >= drawPoint.y && point.y <= drawPoint.y + spriteSizeY)
        {
            return TRUE;
        }
    }
    return FALSE;
}

