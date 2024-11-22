#pragma once
#include "framework.h"

class CNPC : public CCharacter
{
private:
    NPCType npcType;
    CImage hNPC;
    vector<CImage*> npcTalk;
    BOOL isTalk;
    int spriteSizeX;
    int spriteSizeY;

    int bitRow;
    int bitCol;
    int bitColMin;
    int bitColMax;
    BOOL drawForward;

    POINT drawPoint;
    float moveTick;
    int talkScreenIndex;
public:
    CNPC(NPCType npcType, POINT drawPoint);
    ~CNPC() 
    {
       hNPC.Destroy();
    }
    void Update(float deltaTime);
    void Draw(HDC& hdc);
    void DrawTalk(HDC& hdc);
    BOOL PointIsIn(POINT point);
    NPCType GetNPCType() { return npcType; }
    POINT GetDrawPoint() { return drawPoint; }
    void setIsTalk() 
    { 
        isTalk = !isTalk;
        if (isTalk)
            talkScreenIndex = rand() % npcTalk.size();
    }
    void setNotTalk() { isTalk = FALSE; }
    BOOL getIsTalk() { return isTalk; }
};