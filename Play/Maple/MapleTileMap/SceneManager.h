#pragma once
#include "framework.h"

class MapleSceneManager
{
private:
    int scene;
    BOOL mouseClick;
    BOOL loginClick;
    BOOL exitClick;
    POINT mousePos;
    CImage IntroBackGround;
    int IntroTimer;
    int screenY;
    CImage loginBackGround;
    CImage choiceBackGround;
    CImage loginButton;
    CImage TurnBackButton;
    CImage ExitButton;
    CImage mouseCursor;
    CImage nameWarning;
    CImage characterChoice;
    CImage EndingScene;
    BOOL openNameWarning;
    int CursorWidth;
    int CursorHeight;
    int CursorAniCount;
public:
    MapleSceneManager();
    ~MapleSceneManager();
    void SetScene(int scene) { this->scene = scene; }
    void SetUp();
    BOOL ButtonClick();
    int GetScene() { return scene; }   
    void LButtonDownUp(UINT message);
    void DrawButton(HDC hdc , POINT buttonPos);
    void DrawMouseCursor(HDC hdc);
    void DrawScene0(HDC hdc);
    void DrawScene1(HDC hdc);
    void DrawScene2(HDC hdc);
    void DrawEnding(HDC hdc);
    void Render(HDC hdc);
    LRESULT CALLBACK WndProc(UINT message, WPARAM wParam, LPARAM lParam);
};