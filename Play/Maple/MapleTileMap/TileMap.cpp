#include "TileMap.h"
#include "MapObject.h"
#include "Billage.h"
#include "PlayManager.h"
#include "FinalAttack.h"
int TILE_SIZE = 20;
extern vector<Skill> mageSkills;
extern vector<Skill>  WarriorSkills;
extern vector<CPlayer> otherPlayers;
extern BillageType billageType;
extern LevelUpEffect levelUp;
void TileRect::Draw(HDC hdc, int x, int y, int length)
{
    HBRUSH hBrush;
    HBRUSH hOldBrush;
    switch (type)
    {
    case MapType::Platform:
        hBrush = CreateSolidBrush(RGB(0, 0, 255));
        break;
    case MapType::Wall:
        hBrush = CreateSolidBrush(RGB(255, 0, 0));
        break;
    case MapType::Rope:
        hBrush = CreateSolidBrush(RGB(0, 255, 0));
        break;
    case MapType::Ladder:
        hBrush = CreateSolidBrush(RGB(255, 255, 0));
        break;
    case MapType::Attack:
        hBrush = CreateSolidBrush(RGB(255, 125, 0));
        break;
    default:
        hBrush = CreateSolidBrush(RGB(0, 0, 0));
        break;
    }
    hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    Rectangle(hdc, x, y, x + length, y + length);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
}
void InputTileMap(POINT mousePos, MapType mapType, vector<TileRect*>* tileMap)
{


    BOOL tileExist = false;
    for (auto& tile : *tileMap)
    {
        if (tile->GetPos().x == mousePos.x && tile->GetPos().y == mousePos.y)
        {
            if (tile->GetType() == mapType)
            {
                tileExist = true;
                break;
            }
        }
    }
    if (!tileExist)
    {
        tileMap->push_back(new TileRect(TILE_SIZE, mousePos, mapType));
    }
    InvalidateRect(g_hWnd, NULL, false);
}
void deleteTileMap(POINT mousePos, vector<TileRect*>* tileMap)
{
    for (auto& tile : *tileMap)
    {
        if (tile->GetPos().x == mousePos.x && tile->GetPos().y == mousePos.y)
        {
            tileMap->erase(find(tileMap->begin(), tileMap->end(), tile));
            InvalidateRect(g_hWnd, NULL, false);
            break;
        }
    }
}
void SaveTileMap(vector<TileRect*>* tileMap, Camera* camera, POINT* mapMove)
{

    TCHAR currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);

    OPENFILENAME ofn;
    TCHAR fileName[256] = L"";
    TCHAR lpStrFile[256] = L"";
    memset(&ofn, 0, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = g_hWnd;
    ofn.lpstrFilter = L"Every File(*.*)\0*.*\0Text File\0*.txt;*.doc\0";
    ofn.lpstrFile = lpStrFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 256;
    ofn.lpstrFileTitle = fileName;
    ofn.lpstrFileTitle[0] = '\0';
    ofn.lpstrInitialDir = L"..\\Data\\MapData";

    if (GetSaveFileName(&ofn) != 0)
    {
        MessageBox(NULL, L"Save", L"Save", MB_OK);
        ofstream out(ofn.lpstrFile);
        if (!out.is_open())
        {
            MessageBox(NULL, L"File Open Error", L"Error", MB_OK);
            return;
        }
        out << (int)billageType << endl;
        wstring mapfilePath(hBackImageFilename);
        string mapfileName(mapfilePath.begin(), mapfilePath.end());
        out << mapfileName << endl;
        out << camera->GetPos().x << " " << camera->GetPos().y << endl;
        out << mapMove->x << " " << mapMove->y << endl;
        out << zoomRatio << endl;
        for (auto& tile : *tileMap)
        {
            out << tile->GetPos().x << " " << tile->GetPos().y << " " << tile->GetLength() << " " << (int)tile->GetType() << endl;
        }
        out.close();
    }

    SetCurrentDirectory(currentDir);
}
void LoadTileMap(vector<TileRect*>* tileMap, Camera* camera, POINT* mapMove)
{

    TCHAR currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);

    tileMap->clear();
    OPENFILENAME ofn;
    TCHAR fileName[256] = L"";
    TCHAR lpStrFile[256] = L"";
    memset(&ofn, 0, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = g_hWnd;
    ofn.lpstrFilter = L"Every File(*.*)\0*.*\0Text File\0*.txt;*.doc\0";
    ofn.lpstrFile = lpStrFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 256;
    ofn.lpstrFileTitle = fileName;
    ofn.lpstrFileTitle[0] = '\0';
    ofn.lpstrInitialDir = L"..\\Data\\MapData";

    if (GetOpenFileName(&ofn) != 0)
    {
        MessageBox(NULL, L"Load", L"Load", MB_OK);
        ifstream in(ofn.lpstrFile);
        if (!in.is_open())
        {
            MessageBox(NULL, L"File Open Error", L"Error", MB_OK);
            return;
        }
        else
        {
            SetCurrentDirectory(currentDir);
            int billage;
            in >> billage;
            billageType = (BillageType)billage;
            string mapfileName;
            in >> mapfileName;
            wstring mapfilePath(mapfileName.begin(), mapfileName.end());
            if (_tcscmp(hBackImageFilename, mapfilePath.c_str()) != 0)
            {
                _tcscpy_s(hBackImageFilename, mapfilePath.c_str());
                hBackImage.Destroy();
                hBackImage.Load(hBackImageFilename);
                GetObject(hBackImage, sizeof(BITMAP), &bitBack);
            }
            int cameraX, cameraY;
            in >> cameraX >> cameraY;
            camera->SetPos({ cameraX, cameraY });
            int mapMoveX, mapMoveY;
            in >> mapMoveX >> mapMoveY;
            mapMove->x = mapMoveX;
            mapMove->y = mapMoveY;
            float zoom;
            in >> zoom;
            zoomRatio = zoom;

            while (!in.eof())
            {
                int x, y, length, type;
                in >> x >> y >> length >> type;
                tileMap->push_back(new TileRect(length, { x, y }, (MapType)type));
            }
        }
        in.close();
    }
    SetCurrentDirectory(currentDir);
}

void LoadTileData(vector<TileRect*>* tileMap, int filePath)
{
    string fileName;
    switch (filePath)
    {
    case 0:
        fileName = "../Data/MapData/BillageMap.txt";
        break;
    case 1:
        fileName = "../Data/MapData/MonsterMap.txt";
        break;
    case 2:
        fileName = "../Data/MapData/BossMap.txt";
        break;
    }
    ifstream in(fileName);
    if (in.is_open())
    {
        tileMap->clear();
        int billage;
        in >> billage;
        billageType = (BillageType)billage;
        string mapfileName;
        in >> mapfileName;
        wstring mapfilePath(mapfileName.begin(), mapfileName.end());
        if (_tcscmp(hBackImageFilename, mapfilePath.c_str()) != 0)
        {
            _tcscpy_s(hBackImageFilename, mapfilePath.c_str());
            hBackImage.Destroy();
            hBackImage.Load(hBackImageFilename);
            GetObject(hBackImage, sizeof(BITMAP), &bitBack);
        }
        int cameraX, cameraY;
        in >> cameraX >> cameraY;
        int mapMoveX, mapMoveY;
        in >> mapMoveX >> mapMoveY;
        float zoom;
        in >> zoom;

        while (!in.eof())
        {
            int x, y, length, type;
            in >> x >> y >> length >> type;
            tileMap->push_back(new TileRect(length, { x, y }, (MapType)type));
        }
    }
    in.close();
}
void SetTileSize(int size)
{
    TILE_SIZE = size;
}
POINT mapMove = { 0, 0 };
RECT rectView;
POINT mousePos = { -1,-1 };
BOOL openButtonGuide = false;
BOOL ctrlClick = false;
MapType mapType = MapType::Platform;
BOOL verticalDraw = true;
Camera camera({ 0, 0 }, 1200, 800);

extern MapObject portal;
extern CPlayer player;
extern CMonster ArrayMonsters[20];

extern vector<Snail1*> snail1s;
extern vector<Snail2*> snail2s;
extern MushMom* mushMom;
extern int flag;
extern Billage henesys;
extern BillageType billageType;

void DrawBitmap(HDC hdc, Camera* camera)
{
    int bx = bitBack.bmWidth;
    int by = bitBack.bmHeight;
    hBackImage.Draw(hdc, -camera->GetPos().x, -camera->GetPos().y, bx * zoomRatio, by * zoomRatio, 0, 0, hBackImage.GetWidth(), hBackImage.GetHeight());
    char str[100];
    sprintf_s(str, "Open MapManager : Ctrl + B");
    if (openButtonGuide)
        DrawGuide(hdc, *camera);
    else
    {
        if (flag == 0)
            TextOutA(hdc, 0, 20, str, strlen(str));
    }
    mapWidth = camera->GetWidth() * 1.0f / zoomRatio;
    mapHeight = camera->GetHeight() * 1.0f / zoomRatio;
}

extern vector<CMonster*> monsters;
extern vector<CNPC*> npcs;
extern MaplePlayManager* playManager;
extern FinalAttack finalAttack1;
extern FinalAttack finalAttack2;
void DrawBitmapDoubleBuffering(HDC hdc, Camera* camera)
{
    HDC hDoubleBufferDC;
    HBITMAP hDoubleBufferImage;
    HBITMAP hOldDoubleBufferBitmap;
    hDoubleBufferDC = CreateCompatibleDC(hdc);
    hDoubleBufferImage = CreateCompatibleBitmap(hdc, bitBack.bmWidth, bitBack.bmHeight);
    hOldDoubleBufferBitmap = (HBITMAP)SelectObject(hDoubleBufferDC, hDoubleBufferImage);
    SetStretchBltMode(hDoubleBufferDC, HALFTONE);

    DrawBitmap(hDoubleBufferDC, camera);
    for (CNPC* npc : npcs)
        npc->Draw(hDoubleBufferDC);

    for (CNPC* npc : npcs)
        npc->DrawTalk(hDoubleBufferDC);
    portal.Draw(hDoubleBufferDC);

    if (!monsters.empty())
        for (vector<CMonster*>::iterator it = monsters.begin(); it != monsters.end(); ++it)
        {
            if ((*it)->GetIsAlive())
            {
                MonsterType type = (*it)->GetMonsterType();
                switch (type)
                {
                case MonsterType::MushMom:
                    static_cast<MushMom*>(*it)->Draw(hDoubleBufferDC, openButtonGuide);
                    break;
                case MonsterType::Snail1:
                    static_cast<Snail1*>(*it)->Draw(hDoubleBufferDC, openButtonGuide);
                    break;
                case MonsterType::Snail2:
                case MonsterType::Snail3:
                    static_cast<Snail2*>(*it)->Draw(hDoubleBufferDC, openButtonGuide);
                    break;
                }

            }
        }

    if (!mageSkills.empty())
    {
        for (std::vector<Skill>::iterator it = mageSkills.begin(); it != mageSkills.end(); ++it)
        {
            if (it->IsStart)
                (*it).Draw(hDoubleBufferDC);
        }
    }
    if (!WarriorSkills.empty())
    {
        for (std::vector<Skill>::iterator it = WarriorSkills.begin(); it != WarriorSkills.end(); ++it)
        {
            if (it->IsStart)
                (*it).Draw(hDoubleBufferDC);
        }
    }
    if (finalAttack1.GetAttackStart() && player.GetId() == 1)
        finalAttack1.Draw(hDoubleBufferDC);
    else if (finalAttack2.GetAttackStart() && player.GetId() == 2)
        finalAttack2.Draw(hDoubleBufferDC);
    else
        player.Draw(hDoubleBufferDC);

    if (levelUp.GetIsStart())
    {
        levelUp.Draw(hDoubleBufferDC);
    }

    for (CPlayer& otherPlayer : otherPlayers)
    {
        otherPlayer.Draw(hDoubleBufferDC);
    }
    if (flag == 1)
    {
        hUIImage.TransparentBlt(hDoubleBufferDC, 0, -50, camera->GetWidth(), camera->GetHeight(), 0, 0, hUIImage.GetWidth(), hUIImage.GetHeight(), RGB(0, 0, 255));
        float percent = 0;

        if (mushMom != nullptr && mushMom->GetHp() > 0)
        {
            hMonsterIcon.Draw(hDoubleBufferDC, 20, 30, 70, 70, 0, 0, hMonsterIcon.GetWidth(), hMonsterIcon.GetHeight());
            percent = mushMom->GetHp() * 1.0f / mushMom->GetMaxHp();
            if (percent >16000)
                percent = 1;
            else if (percent < 0)
            {
                percent = 0;
            }

            hMonsterHp.Draw(hDoubleBufferDC, 100, 50, 600 * percent, 30, 0, 0, hMonsterHp.GetWidth(), hMonsterHp.GetHeight());
        }
        playManager->DrawUI(hDoubleBufferDC);
    }
    int bx = bitBack.bmWidth;
    int by = bitBack.bmHeight;
    BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, hDoubleBufferDC, 0, 0, SRCCOPY);

    SelectObject(hDoubleBufferDC, hOldDoubleBufferBitmap);
    DeleteObject(hDoubleBufferImage);
    DeleteDC(hDoubleBufferDC);
}
void DestroyBitmap()
{
    hBackImage.Destroy();
    hUIImage.Destroy();
    hMonsterIcon.Destroy();
    hMonsterHp.Destroy();
}
extern vector<TileRect*> tileMap;
void DrawGuide(HDC hdc, Camera camera)
{
    for (auto& tile : tileMap)
    {
        int screenX = (tile->GetPos().x - mapMove.x / zoomRatio) * zoomRatio;
        int screenY = (tile->GetPos().y - mapMove.y / zoomRatio) * zoomRatio;
        tile->Draw(hdc, screenX, screenY, tile->GetLength() * zoomRatio);
    }
    //Rectangle(hdc, rectView.right / 2 - 5, rectView.bottom / 2 - 5, rectView.right / 2 + 5, rectView.bottom / 2 + 5);
    char str[100];
    int y = 0;
    sprintf_s(str, "MousePos : x = %d, y = %d", mousePos.x, mousePos.y);
    TextOutA(hdc, rectView.right - 180, 0, str, strlen(str));
    sprintf_s(str, "CameraPos : x = %d, y = %d", camera.GetPos().x, camera.GetPos().y);
    TextOutA(hdc, rectView.right - 180, 20, str, strlen(str));
    sprintf_s(str, "MapMove : x = %d, y = %d", mapMove.x, mapMove.y);
    TextOutA(hdc, rectView.right - 180, 40, str, strlen(str));
    sprintf_s(str, "ZoomRatio : %.2f", zoomRatio);
    TextOutA(hdc, rectView.right - 180, 60, str, strlen(str));
    sprintf_s(str, "Resolution : %d x %d", mapWidth, mapHeight);
    TextOutA(hdc, rectView.right - 180, 80, str, strlen(str));
    sprintf_s(str, "Player point : %d , %d", player.GetPoint().x, player.GetPoint().y);
    TextOutA(hdc, rectView.right - 180, 100, str, strlen(str));
    sprintf_s(str, "Player col size : %0.1f , %0.1f", player.GetWidth(), player.GetHeight());
    TextOutA(hdc, rectView.right - 180, 120, str, strlen(str));
    if (flag == 0)
    {
        sprintf_s(str, "Save : Ctrl+s , Load : Ctrl + O");
        TextOutA(hdc, 10, y += 20, str, strlen(str));
        sprintf_s(str, "Zoom (Q : In, E : Out)");
        TextOutA(hdc, 10, y += 20, str, strlen(str));
        sprintf_s(str, "RectSize (Q : 5, W : 10, E : 20)");
        TextOutA(hdc, 10, y += 20, str, strlen(str));
        sprintf_s(str, "MapType (1 : Platform, 2 : Wall, 3 : Rope, 4 : Ladder)");
        TextOutA(hdc, 10, y += 20, str, strlen(str));
        sprintf_s(str, "R : ClearTile , Vertical Draw (Z : On/Off)");
        TextOutA(hdc, 10, y += 20, str, strlen(str));
        SetTextColor(hdc, RGB(255, 0, 0));
        sprintf_s(str, "MapType : ");
        switch (mapType)
        {
        case MapType::Platform:
            strcat_s(str, "Platform");
            break;
        case MapType::Wall:
            strcat_s(str, "Wall");
            break;
        case MapType::Rope:
            strcat_s(str, "Rope");
            break;
        case MapType::Ladder:
            strcat_s(str, "Ladder");
            break;
        }
        TextOutA(hdc, 10, y += 20, str, strlen(str));
        sprintf_s(str, "TileSize : %d", TILE_SIZE);
        TextOutA(hdc, 10, y += 20, str, strlen(str));
        if (verticalDraw)
            sprintf_s(str, "Vertical Draw : On");
        else
            sprintf_s(str, "Vertical Draw : Off");
        TextOutA(hdc, 10, y += 20, str, strlen(str));
    }

}