#include "SceneManager.h"
#include "SoundManager.h"
#include "TileMap.h"
#include "MonsterManager.h"

MapleSceneManager::MapleSceneManager()
{
    scene = 0;
    IntroTimer = 0;
    screenY = 0;
    IntroBackGround.Load(L"../Data/img/UI/Intro.png");
    loginBackGround.Load(L"../Data/img/UI/Login.png");
    choiceBackGround.Load(L"../Data/img/UI/choice.png");
    loginButton.Load(L"../Data/img/UI/Button/loginButton.png");
    ExitButton.Load(L"../Data/img/UI/Button/ExitButton.png");
    TurnBackButton.Load(L"../Data/img/UI/Button/turnBackButton.png");
    mouseCursor.Load(L"../Data/img/UI/Cursor.bmp");
    nameWarning.Load(L"../Data/img/UI/nameWarning.png");
    characterChoice.Load(L"../Data/img/UI/Button/choisePlayer.png");
    EndingScene.Load(L"../Data/img/UI/Ending.png");
    openNameWarning = FALSE;
    mouseClick = FALSE;
    loginClick = FALSE;
    exitClick = FALSE;
    mousePos = { 0, 0 };
    CursorWidth = 30;
    CursorHeight = 35;
    CursorAniCount = 0;
}

MapleSceneManager::~MapleSceneManager()
{
    stopBgm(dwID);
    IntroBackGround.Destroy();
    loginBackGround.Destroy();
    choiceBackGround.Destroy();
    loginButton.Destroy();
    ExitButton.Destroy();
    TurnBackButton.Destroy();
    mouseCursor.Destroy();
}
extern int flag;
extern CPlayer player;
extern MapleMonsterManager* monsterManager;
char namestr[100] = { 0 };
int msgCount = 0;
CPlayer choosePlayer1;
CPlayer choosePlayer2;
int choicePortalRow = 0;
int choiceTick = 0;
extern bool IsClear;
void MapleSceneManager::SetUp()
{
    choosePlayer1.Init(1);
    choosePlayer1.SetSpeed(0);
    choosePlayer1.SetPoint({ 450, 250 });
    choosePlayer1.SetWidth(60);
    choosePlayer1.SetHeight(80);
    choosePlayer2.Init(2);
    choosePlayer2.SetPoint({ 450, 520 });
    choosePlayer2.SetSpeed(0);
    choosePlayer2.SetWidth(70);
    choosePlayer2.SetHeight(80);

    ShowCursor(FALSE);
    playingEffect(SoundType::Intro);
}

BOOL MapleSceneManager::ButtonClick()
{
    switch (scene)
    {
    case 1:
        if (mousePos.x >= 905 && mousePos.x <= 905 + ExitButton.GetWidth() / 2 && mousePos.y >= 453 && mousePos.y <= 453 + ExitButton.GetHeight())
            exitClick = true;
        else
            exitClick = false;
        if (mousePos.x >= 895 && mousePos.x <= 895 + loginButton.GetWidth() / 2 && mousePos.y >= 288 && mousePos.y <= 288 + loginButton.GetHeight())
        {
            return TRUE;
        }
        break;
    case 2:
        if (mousePos.x >= 1000 && mousePos.x <= 1000 + TurnBackButton.GetWidth() / 2 && mousePos.y >= 100 && mousePos.y <= 100 + TurnBackButton.GetHeight())
        {
            return TRUE;
        }
        break;
    default:
        break;
    }
    return FALSE;
}

void MapleSceneManager::LButtonDownUp(UINT message)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
        mouseClick = TRUE;
        playingEffect(SoundType::button);
        if (scene == 1)
        {
            if (ButtonClick())
                loginClick = TRUE;
        }
        else if (scene == 2)
        {
            if (choosePlayer1.PointIsin(mousePos))
            {
                
                choosePlayer1.SetState(playerState::move);
                choosePlayer2.SetState(playerState::idle);

            }

            if (choosePlayer2.PointIsin(mousePos))
            {
                choosePlayer2.SetState(playerState::move);
                choosePlayer1.SetState(playerState::idle);
            }
        }
        break;
    
    case WM_LBUTTONUP:
        mouseClick = FALSE;
        stopEffect(EffectID);
        if (scene == 1)
        {
            if (exitClick)
            {
                ButtonClick();
                if (exitClick)
                    PostQuitMessage(0);
            }
            if (loginClick)
            {
                loginClick = FALSE;
                if (ButtonClick())
                {
                    switch (scene)
                    {
                    case 1:
                        if (msgCount > 0)
                        {
                            string playerName = "";
                            for (int i = 0; i < msgCount; i++)
                                playerName += namestr[i];
                            player.SetName(playerName);
                            scene = 2;
                        }
                        else
                            openNameWarning = TRUE;
                        break;
                    case 2:
                        scene = 1;
                        break;
                    default:
                        break;
                    }
                }
            }
        }
        break;
    }
}
void MapleSceneManager::DrawButton(HDC hdc, POINT buttonPos)
{
    int buttonCol = loginClick ? 1 : 0;
    int exitButtonCol = exitClick ? 1 : 0;
    switch (scene)
    {
    case 1:
        loginButton.TransparentBlt(hdc, buttonPos.x, buttonPos.y, loginButton.GetWidth() / 2, loginButton.GetHeight(), loginButton.GetWidth() / 2 * buttonCol, 0, loginButton.GetWidth() / 2, loginButton.GetHeight(), NULL);
        ExitButton.TransparentBlt(hdc, buttonPos.x + 10, buttonPos.y + 165, ExitButton.GetWidth() / 2, ExitButton.GetHeight(), ExitButton.GetWidth() / 2 * exitButtonCol, 0, ExitButton.GetWidth() / 2, ExitButton.GetHeight(), NULL);
        break;
    case 2:
        TurnBackButton.TransparentBlt(hdc, buttonPos.x, buttonPos.y, TurnBackButton.GetWidth() / 2, TurnBackButton.GetHeight(), TurnBackButton.GetWidth() / 2 * buttonCol, 0, TurnBackButton.GetWidth() / 2, TurnBackButton.GetHeight(), RGB(0, 0, 255));
        break;
    default:
        break;
    }
}
void MapleSceneManager::DrawMouseCursor(HDC hdc)
{
    if (mouseClick)
        CursorAniCount = 2;
    else
        CursorAniCount = 0;
    mouseCursor.TransparentBlt(hdc, mousePos.x, mousePos.y, CursorWidth, CursorHeight, CursorWidth * CursorAniCount, 0, CursorWidth, CursorHeight, RGB(255, 0, 255));
}
void MapleSceneManager::DrawScene0(HDC hdc)
{
    IntroTimer++;
    if (IntroTimer == 5)
    {
        screenY++;
        IntroTimer = 0;
        if (screenY >= 42)
        {
            screenY = 0;
            scene = 1;
            stopEffect(EffectID);
            playingBgm(SoundType::lobby);
        }
    }
    int width = 455;
    int height = 285;
    IntroBackGround.TransparentBlt(hdc, 200, 0, 800, 500, 0, height * screenY, width, height, RGB(255, 0, 255));
}
void MapleSceneManager::DrawScene1(HDC hdc)
{
    loginBackGround.TransparentBlt(hdc, 0, 0, 1200, 800, 0, 0, loginBackGround.GetWidth(), loginBackGround.GetHeight(), RGB(255, 0, 255));
    SetBkMode(hdc, TRANSPARENT);
    TextOutA(hdc, 670, 310, namestr, strlen(namestr));
    DrawButton(hdc, { 895, 288 });
    if (openNameWarning)
        nameWarning.TransparentBlt(hdc, 300, 250, nameWarning.GetWidth(), nameWarning.GetHeight(), 0, 0, nameWarning.GetWidth(), nameWarning.GetHeight(), RGB(255, 0, 255));
}
void MapleSceneManager::DrawScene2(HDC hdc)
{
    choiceBackGround.TransparentBlt(hdc, 0, 0, 1200, 800, 0, 0, choiceBackGround.GetWidth(), choiceBackGround.GetHeight(), RGB(255, 0, 255));
    DrawButton(hdc, { 1000, 100 });
    
    if (choosePlayer1.GetState() == playerState::move)
        characterChoice.TransparentBlt(hdc, choosePlayer1.GetPoint().x- choosePlayer1.GetWidth() / 2, choosePlayer1.GetPoint().y - choosePlayer1.GetHeight(), choosePlayer1.GetWidth()*2, choosePlayer1.GetHeight()*2, 0, 170 * choicePortalRow, 180, 170, RGB(255, 0, 255));
    else if(choosePlayer2.GetState() == playerState::move)
        characterChoice.TransparentBlt(hdc, choosePlayer2.GetPoint().x - choosePlayer2.GetWidth() / 2, choosePlayer2.GetPoint().y - choosePlayer2.GetHeight(), choosePlayer2.GetWidth()*2, choosePlayer2.GetHeight()*2, 0, 170 * choicePortalRow, 180, 170, RGB(255, 0, 255));
    choiceTick++;
    if (choiceTick == 10)
    {
        choiceTick = 0;
        choicePortalRow++;
    }
    if (choicePortalRow >= 8)
        choicePortalRow = 0;
    choosePlayer1.DrawSence(hdc);

    if (choosePlayer1.GetState() == playerState::move)
        choosePlayer1.CountUpdate();
    choosePlayer2.DrawSence(hdc);
    if (choosePlayer2.GetState() == playerState::move)
        choosePlayer2.CountUpdate();
}
void MapleSceneManager::DrawEnding(HDC hdc)
{
    EndingScene.Draw(hdc, 0, 0, 1500, 1000, 0, 0, EndingScene.GetWidth(), EndingScene.GetHeight());
}
void MapleSceneManager::Render(HDC hdc)
{
    HDC hDoubleBufferDC;
    HBITMAP hDoubleBufferImage;
    HBITMAP hOldDoubleBufferBitmap;
    hDoubleBufferDC = CreateCompatibleDC(hdc);
    hDoubleBufferImage = CreateCompatibleBitmap(hdc, rectView.right, rectView.bottom);
    hOldDoubleBufferBitmap = (HBITMAP)SelectObject(hDoubleBufferDC, hDoubleBufferImage);
    SetStretchBltMode(hDoubleBufferDC, HALFTONE);
    // 더블 버퍼 DC의 배경을 흰색으로 설정
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hDoubleBufferDC, &rectView, hBrush);
    DeleteObject(hBrush); // 브러시 메모리 해제
    switch (scene)
    {
    case 0:
        DrawScene0(hDoubleBufferDC);
        break;
    case 1:
        DrawScene1(hDoubleBufferDC);
        break;
    case 2:
        DrawScene2(hDoubleBufferDC);
        break;
    }
    if (scene != 0)
        DrawMouseCursor(hDoubleBufferDC);

    BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, hDoubleBufferDC, 0, 0, SRCCOPY);

    SelectObject(hDoubleBufferDC, hOldDoubleBufferBitmap);
    DeleteObject(hDoubleBufferImage);
    DeleteDC(hDoubleBufferDC);
}


LRESULT MapleSceneManager::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CHAR:
    {
        if (scene == 1)
        {
            if (wParam == VK_RETURN)
            {
                if (openNameWarning)
                    openNameWarning = FALSE;
                else if (msgCount > 0)
                {
                    string playerName = "";
                    for (int i = 0; i < msgCount; i++)
                        playerName += namestr[i];
                    player.SetName(playerName);
                    scene = 2;
                }
                else
                    openNameWarning = TRUE;
            }
            else if (wParam == VK_BACK)
            {
                if (msgCount > 0)
                {
                    msgCount--;
                    namestr[msgCount] = NULL;
                }
            }
            else if (wParam == VK_SPACE)
            {
                if (openNameWarning)
                    openNameWarning = FALSE;
            }
            else
            {
                if (!openNameWarning)
                {
                    namestr[msgCount++] = wParam;
                    namestr[msgCount] = NULL;
                }
            }
        }
    }
    break;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        LButtonDownUp(message);
        break;
    case WM_LBUTTONDBLCLK:

        if (scene == 2)
        {
            if (choosePlayer1.PointIsin(mousePos))
            {
                player.Init(1);
            }

            if (choosePlayer2.PointIsin(mousePos))
            {
                player.Init(2);
            }

            flag = 1;
            string playerName = "";
            for (int i = 0; i < msgCount; i++)
                playerName += namestr[i];
            player.SetName(playerName);
            PlaySound(PORTAL, NULL, SND_ASYNC);

           
            stopBgm(dwID);
            playingBgm(SoundType::play);
            player.SetPoint({ 96,850 });
            LoadTileData(&tileMap, 0);

            monsterManager->LoadMonsterData(0);
        }
        break;
    case WM_MOUSEMOVE:
        mousePos = { LOWORD(lParam), HIWORD(lParam) };
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(g_hWnd, message, wParam, lParam);
}
