#include "PlayManager.h"
#include "SoundManager.h"   
#include "MonsterManager.h"
#include "FinalAttack.h"

MaplePlayManager::MaplePlayManager()
{
    ctrlClick = FALSE;
    mouseCursor.Load(L"../Data/img/UI/Cursor.bmp");
    hpBar.Load(L"../Data/img/UI/PlayerHPBar.bmp");
    mpBar.Load(L"../Data/img/UI/PlayerMPBar.bmp");
    expBar.Load(L"../Data/img/UI/PlayerExpBar.bmp");
    CursorWidth = 30;
    CursorHeight = 35;
    CursorAniCount = 0;
}

MaplePlayManager::~MaplePlayManager()
{
    mouseCursor.Destroy();
    hpBar.Destroy();
    mpBar.Destroy();
    expBar.Destroy();
}

extern CPlayer player;
extern vector<CNPC*> npcs;
FinalAttack finalAttack1;
FinalAttack finalAttack2;
void MaplePlayManager::SetUp()
{
    camera.SetWidth(1500);
    camera.SetHeight(1000);
    camera.SetPos({ 0,0 });
    mapMove = { 0,0 };
    zoomRatio = 1.0f;
    finalAttack1.Init(1);
    finalAttack2.Init(2);
}
void MaplePlayManager::DrawUI(HDC hdc)
{
    float hpPercent = player.GetHp() / player.GetMaxHp();
    float mpPercent = player.GetMp() / player.GetMaxMp();
    float expPercent = (float)player.GetExp() / player.GetMaxExp();
    if(expPercent > 1.0f)
        expPercent = 1.0f;

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 0));
    char playerLevel[100];
    sprintf_s(playerLevel, "%d", player.GetLevel());
    TextOutA(hdc, 88, 900, playerLevel, strlen(playerLevel));

    char playerName[100];
    sprintf_s(playerName, "%s", player.GetName().c_str());
    TextOutA(hdc, 150, 900, playerName, strlen(playerName));

    hpBar.TransparentBlt(hdc, 466, 883, 269 * hpPercent, 23, 0, 0, hpBar.GetWidth(), hpBar.GetHeight(), NULL);
    mpBar.TransparentBlt(hdc, 784, 883, 269 * mpPercent, 23, 0, 0, mpBar.GetWidth(), mpBar.GetHeight(), NULL);
    expBar.TransparentBlt(hdc, 466, 910, 586 * expPercent, 23, 0, 0, expBar.GetWidth(), expBar.GetHeight(), NULL);
    mouseCursor.TransparentBlt(hdc, mousePos.x, mousePos.y, CursorWidth, CursorHeight, CursorWidth * CursorAniCount, 0, CursorWidth, CursorHeight, RGB(255, 0, 255));
    if(ctrlClick)
    {
        POINT skillPoint = { player.GetPoint().x * zoomRatio - camera.GetPos().x / zoomRatio, player.GetPoint().y * zoomRatio - camera.GetPos().y / zoomRatio };
        //finalAttack1.Draw(hdc, skillPoint);
    }
}
void MaplePlayManager::PlayerCameraMove()
{
    camera.SetPos({ (long)(player.GetPoint().x * zoomRatio - camera.GetWidth() / 2), (long)(player.GetPoint().y * zoomRatio - camera.GetHeight() / 2) });
    if (camera.GetPos().x < 0)
    {
        camera.SetPos({ 0, camera.GetPos().y });
        mapMove.x = 0;
    }
    if (camera.GetPos().y < 0)
    {
        camera.SetPos({ camera.GetPos().x, 0 });
        mapMove.y = 0;
    }
    if (camera.GetPos().x + camera.GetWidth() > bitBack.bmWidth * zoomRatio)
    {
        camera.SetPos({ (long)(bitBack.bmWidth * zoomRatio - camera.GetWidth()), camera.GetPos().y });
        mapMove.x = bitBack.bmWidth * zoomRatio - camera.GetWidth();
    }
    if (camera.GetPos().y + camera.GetHeight() > bitBack.bmHeight * zoomRatio)
    {
        camera.SetPos({ camera.GetPos().x, (long)(bitBack.bmHeight * zoomRatio - camera.GetHeight()) });
        mapMove.y = bitBack.bmHeight * zoomRatio - camera.GetHeight();
    }
    mapMove = camera.GetPos();


}
void MaplePlayManager::ButtonChar(UINT message, WPARAM wParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        if (wParam == VK_CONTROL)
        {
            ctrlClick = !ctrlClick;
            finalAttack1.SetAttackFrame(0);
        }

    /*case WM_KEYUP:
        if (wParam == VK_CONTROL)
            ctrlClick = FALSE;*/
        break;
    case WM_CHAR:
        if(wParam == 'B')
            openButtonGuide = !openButtonGuide;
        break;
    default:
        break;
    }
}
void MaplePlayManager::Render(HDC hdc)
{
    DrawBitmapDoubleBuffering(hdc, &camera);
}
extern BillageType billageType;
extern MapleMonsterManager* monsterManager;
void MaplePlayManager::MoveMap(MapObject* portal, CPlayer* player)
{
    if(player->GetMoveMap() == TRUE)
    {
        player->SetMoveMap(FALSE);
        portal->SoundPlay();
        if (billageType == BillageType::Monster)
        {
            player->SetPoint({ 170,470 });
            LoadTileData(&tileMap, 2);
            player->mapNum = 2;
            monsterManager->LoadMonsterData(2);
            billageType = BillageType::Boss;
        }
        else if (billageType == BillageType::Henesys)
        {
            portal->SetDrawPoint({ 1500,1140 });
            player->SetPoint({ 300,1200 });
            LoadTileData(&tileMap, 1);
            player->mapNum = 1;
            monsterManager->LoadMonsterData(1);
            billageType = BillageType::Monster;
        }
    }
}
LRESULT MaplePlayManager::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_CHAR:
        ButtonChar(message, wParam);
        break;
    case WM_MOUSEMOVE:
    {
        mousePos = { LOWORD(lParam), HIWORD(lParam) };
    }
    break;
    case WM_LBUTTONDOWN:
    {
        mousePos = { LOWORD(lParam), HIWORD(lParam) };
        mousePos = { (long)(mousePos.x / zoomRatio + mapMove.x / zoomRatio), (long)(mousePos.y / zoomRatio + mapMove.y / zoomRatio) };
        playingEffect(SoundType::button);
        mouseClick = TRUE;
        CursorAniCount = 2;
        for (CNPC* npc : npcs)
        {
            if(npc->PointIsIn(mousePos))
            {
                for(CNPC* npc : npcs)
                    npc->setNotTalk();
                npc->setIsTalk();
            }
        }
    }
    break;
    case WM_LBUTTONUP:
    {
        stopEffect(EffectID);
        mouseClick = FALSE;
        CursorAniCount = 0;
    }
    break;
    case WM_MOUSEWHEEL:
    {
        if (ctrlClick)
        {
            short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            if (zDelta > 0)
            {
                zoomRatio += 0.1f;
                if (zoomRatio > 2.0f)
                    zoomRatio = 2.0f;
            }
            else
            {
                zoomRatio -= 0.1f;
                if (zoomRatio < 1.0f)
                    zoomRatio = 1.0;
            }
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(g_hWnd, message, wParam, lParam);
}