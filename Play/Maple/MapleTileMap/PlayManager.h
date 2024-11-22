#pragma once
#include "TileMap.h"
#include "MapObject.h"
class MaplePlayManager
{
private:
    BOOL ctrlClick;
    BOOL mouseClick;
    POINT mousePos;
    CImage mouseCursor;
    CImage hpBar;
    CImage mpBar;
    CImage expBar;
    int CursorWidth;
    int CursorHeight;
    int CursorAniCount;
public:
    MaplePlayManager();
    ~MaplePlayManager();
    void DrawUI(HDC hdc);
    void SetUp();
    void PlayerCameraMove();
    void ButtonChar(UINT message , WPARAM wParam);
    void Render(HDC hdc);
    void MoveMap(MapObject* portal,CPlayer* player);
    LRESULT CALLBACK WndProc(UINT message, WPARAM wParam, LPARAM lParam);
};