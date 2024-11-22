// MapleTileMap.h : 애플리케이션에 대한 매니저 클래스를 정의합니다.
//

#pragma once
#include "TileMap.h"

class MapleTileMapManager
{
private:
    BOOL keys[256];
    BOOL ctrlClick;
    int speed;
    POINT inputMousePos;
    BOOL mouseLeftDown;
    BOOL mouseRightDown;
    LPPOINT dragMousePos;
public:
    MapleTileMapManager();
    ~MapleTileMapManager();
    void SetUp();
    void MapMovingButton(UINT message, BOOL isDown, WPARAM wParam);
    void MapMoving(); // timer마다 호출
    void MapButtonChar(UINT message , WPARAM wParam);
    void MapMouseControl(UINT message, WPARAM wParam, LPARAM lParam);
    int GetSpeed() { return speed; }
    void Render(HDC hdc);

    LRESULT CALLBACK WndProc(UINT message, WPARAM wParam, LPARAM lParam);
};
