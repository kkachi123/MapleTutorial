#include "MonsterManager.h"
extern CPlayer player;
extern vector<Snail1*> snail1s;
extern vector<Snail2*> snail2s;
extern vector<Snail2> seversnail2s;
extern MushMom* mushMom;
extern vector<CPlayer> otherPlayers;
extern vector<CMonster*> monsters;
extern CMonster ArrayMonsters[20];

MapleMonsterManager::MapleMonsterManager()
{
    keys[255] = { FALSE };
    ctrlClick = FALSE;
    speed = 10;
    inputMousePos = new POINT;
    dragMousePos = new POINT;
    inputMonseterType = MonsterType::Snail1;
}

MapleMonsterManager::~MapleMonsterManager()
{
    delete inputMousePos;
}

void MapleMonsterManager::SetUp()
{
    camera.SetWidth(1500);
    camera.SetHeight(1000);
    camera.SetPos({ 0,0 });
    mapMove = { 0,0 };
    zoomRatio = 1.0f;
    SettingMonsters();
}

void MapleMonsterManager::MapMovingButton(UINT message, BOOL isDown, WPARAM wParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_CONTROL:
            ctrlClick = TRUE;
            break;
        case VK_LEFT:
            keys[VK_LEFT] = TRUE;
            keys[VK_RIGHT] = FALSE;
            keys[VK_UP] = FALSE;
            keys[VK_DOWN] = FALSE;
            break;
        case VK_RIGHT:
            keys[VK_RIGHT] = TRUE;
            keys[VK_LEFT] = FALSE;
            keys[VK_UP] = FALSE;
            keys[VK_DOWN] = FALSE;
            break;
        case VK_UP:
            keys[VK_UP] = TRUE;
            keys[VK_LEFT] = FALSE;
            keys[VK_RIGHT] = FALSE;
            keys[VK_DOWN] = FALSE;
            break;
        case VK_DOWN:
            keys[VK_DOWN] = TRUE;
            keys[VK_LEFT] = FALSE;
            keys[VK_RIGHT] = FALSE;
            keys[VK_UP] = FALSE;
            break;
        case 'S':
            if (GetKeyState(VK_CONTROL) & 0x8000)
                SaveMonster();
            break;
        case 'O':
            if (GetKeyState(VK_CONTROL) & 0x8000)
                LoadMonster();
            break;
        case 'R':
        {
            snail1s.clear();
            snail2s.clear();
            mushMom = nullptr;
            SettingMonsters();
        }
        break;
        case 'B':
            if (GetKeyState(VK_CONTROL) & 0x8000)
                openButtonGuide = !openButtonGuide;
            break;
        }
        InvalidateRect(g_hWnd, NULL, false);
    }
    break;
    case WM_KEYUP:
        if (wParam == VK_CONTROL)
            ctrlClick = FALSE;
        else
            keys[wParam] = FALSE;
        break;
    }
}

void MapleMonsterManager::MapMoving()
{
    BOOL canMove = true;
    if (keys[VK_LEFT])
    {
        camera.SetPos({ (long)(camera.GetPos().x - speed * zoomRatio), camera.GetPos().y });
        mapMove.x -= speed * zoomRatio;
    }
    if (keys[VK_RIGHT])
    {
        camera.SetPos({ (long)(camera.GetPos().x + speed * zoomRatio), camera.GetPos().y });
        mapMove.x += speed * zoomRatio;
    }
    if (keys[VK_UP])
    {
        camera.SetPos({ camera.GetPos().x, (long)(camera.GetPos().y - speed * zoomRatio) });
        mapMove.y -= speed * zoomRatio;
    }
    if (keys[VK_DOWN])
    {
        camera.SetPos({ camera.GetPos().x, (long)(camera.GetPos().y + speed * zoomRatio) });
        mapMove.y += speed * zoomRatio;
    }

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
}

void MapleMonsterManager::MapButtonChar(UINT message, WPARAM wParam)
{
    switch (message)
    {
    case WM_CHAR:
        switch (wParam)
        {
            case '1':
                inputMonseterType = MonsterType::Snail1;
            break;
            case '2':
                inputMonseterType = MonsterType::Snail2;
            break;
            case '3':
                inputMonseterType = MonsterType::Snail3;
            break;
            case '4':
                inputMonseterType = MonsterType::MushMom;
            break;
            case '5':
                inputMonseterType = (MonsterType)5;
            break;
            case '6':
                inputMonseterType = (MonsterType)6;
            break;
        }
        break;
    }
}

void MapleMonsterManager::MapMouseControl(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
        GetCursorPos(inputMousePos);
        ScreenToClient(g_hWnd, inputMousePos);
        if((int)inputMonseterType <= 4)
            SettingMonster();
        else
            SettingNPC({ inputMousePos->x + camera.GetPos().x, inputMousePos->y + camera.GetPos().y }, (int)inputMonseterType);
        break;
    case WM_RBUTTONDOWN:
        GetCursorPos(inputMousePos);
        ScreenToClient(g_hWnd, inputMousePos);
        DeleteMonster();
        DeleteNPC();
        break;
    case WM_MOUSEMOVE:
        GetCursorPos(dragMousePos);    
        ScreenToClient(g_hWnd, dragMousePos);
        break;
    }
}

void MapleMonsterManager::SettingSnail1(POINT drawPoint)
{
    Snail1* snail1;
    snail1 = new Snail1();
    snail1->init(drawPoint);
    snail1->SetMonsterType(MonsterType::Snail1);
    snail1s.push_back(snail1);
}

void MapleMonsterManager::SettingSnail2(POINT drawPoint, int type, int exp, int hp, int Damage, int Defense)
{
    Snail2* snail2;
    snail2 = new Snail2(type);
    snail2->init(drawPoint, exp, hp, Damage, Defense);
    if(type == 0)
        snail2->SetMonsterType(MonsterType::Snail2);
    else
        snail2->SetMonsterType(MonsterType::Snail3);
    snail2s.push_back(snail2);
}
void MapleMonsterManager::SettingMushMom(POINT drawPoint)
{
    mushMom = new MushMom();
    mushMom->SetMonsterType(MonsterType::MushMom);
    mushMom->init(drawPoint);
}


void MapleMonsterManager::SettingMonsters()
{
    monsters.clear();
    for (int i = 0; i < snail1s.size(); ++i)
    {
        monsters.push_back(snail1s[i]);
    }
    for (int i = 0; i < snail2s.size(); ++i)
    {
        monsters.push_back(snail2s[i]);
    }

    if(mushMom != nullptr)
    {
        monsters.push_back(mushMom);
    }
}
extern vector<TileRect*> tileMap;
void MapleMonsterManager::SettingMonster()
{
    for (auto& tile : tileMap)
    {
        if (tile->GetPos().y <= inputMousePos->y + drawAreaHeight + camera.GetPos().y && inputMousePos->y + drawAreaHeight + camera.GetPos().y <= tile->GetPos().y + tile->GetLength())
        {
            if (tile->GetPos().x <= inputMousePos->x + camera.GetPos().x && inputMousePos->x + camera.GetPos().x <= tile->GetPos().x + tile->GetLength())
            {
                inputMousePos->y = tile->GetPos().y - camera.GetPos().y - drawAreaHeight;
            }
        }
    }
    switch (inputMonseterType)
    {
    case MonsterType::Snail1:
        SettingSnail1({ inputMousePos->x + camera.GetPos().x, inputMousePos->y + camera.GetPos().y });
        break;
    case MonsterType::Snail2:
        SettingSnail2({ inputMousePos->x + camera.GetPos().x, inputMousePos->y + camera.GetPos().y }, 0);
        break;
    case MonsterType::Snail3:
        SettingSnail2({ inputMousePos->x + camera.GetPos().x, inputMousePos->y + camera.GetPos().y }, 1, 8, 50, 15, 3);
        break;
    case MonsterType::MushMom:
        if(mushMom == nullptr)
            SettingMushMom({ inputMousePos->x + camera.GetPos().x, inputMousePos->y + camera.GetPos().y });
        break;
    default:
        break;
    }
    SettingMonsters();
}
void MapleMonsterManager::DeleteMonster()
{
    POINT mousePoint = { inputMousePos->x + camera.GetPos().x, inputMousePos->y + camera.GetPos().y };
    for(auto& monster : monsters)
    {
        if(monster->PointIsIn(mousePoint))
        {
            if (monster->GetMonsterType() == MonsterType::Snail1)
            {
                for (int i = 0; i < snail1s.size(); ++i)
                {
                    if (snail1s[i] == monster)
                    {
                        snail1s.erase(snail1s.begin() + i);
                        break;
                    }
                }
            }
            else if (monster->GetMonsterType() == MonsterType::Snail2 || monster->GetMonsterType() == MonsterType::Snail3)
            {
                for (int i = 0; i < snail2s.size(); ++i)
                {
                    if (snail2s[i] == monster)
                    {
                        snail2s.erase(snail2s.begin() + i);
                        break;
                    }
                }
            }
            else if (monster->GetMonsterType() == MonsterType::MushMom)
            {
                mushMom = nullptr;
            }
            break;
        }
    }
    SettingMonsters();
}
extern vector<CNPC*> npcs;
void MapleMonsterManager::SettingNPC(POINT drawPoint, int inputNPCType)
{
    CNPC* npc;
    NPCType npcType;
    npcType = (NPCType)(inputNPCType - 5);
    for (CNPC* npc : npcs)
    {
        if(npc->GetNPCType() == npcType)
            return;
    }
    
    switch (npcType)
    {
        case NPCType::Hinz:
            npc = new CNPC(NPCType::Hinz, { drawPoint.x, drawPoint.y });
        break;
        case NPCType::Lucas:
            npc = new CNPC(NPCType::Lucas, { drawPoint.x, drawPoint.y });
        break;
    }
    npcs.push_back(npc);
}

void MapleMonsterManager::DeleteNPC()
{
    POINT mousePoint = { inputMousePos->x + camera.GetPos().x, inputMousePos->y + camera.GetPos().y };
    for (CNPC* npc : npcs)
    {
        if (npc->PointIsIn(mousePoint))
        {
            for (int i = 0; i < npcs.size(); ++i)
            {
                if (npcs[i] == npc)
                {
                    npcs.erase(npcs.begin() + i);
                    break;
                }
            }
            break;
        }
    }
}

void MapleMonsterManager::SaveMonster()
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
    ofn.lpstrInitialDir = L"..\\Data\\MonsterData";

    if (GetSaveFileName(&ofn) != 0)
    {
        ofstream out(ofn.lpstrFile);
        if (out.is_open())
        {
            MessageBox(NULL, L"Save", L"Save", MB_OK);
            ofstream out(ofn.lpstrFile);
            if (!out.is_open())
            {
                MessageBox(NULL, L"File Open Error", L"Error", MB_OK);
                return;
            }
            int monsterType;
            POINT drawPoint;
            for(CMonster* monster : monsters)
            {
                monsterType = (int)monster->GetMonsterType();
                drawPoint = monster->GetDrawPoint();
                out << monsterType << " " << drawPoint.x << " " << drawPoint.y << endl;
            }
            for(CNPC* npc : npcs)
            {
                monsterType = (int)npc->GetNPCType() + 5;
                drawPoint = npc->GetDrawPoint();
                out << monsterType << " " << drawPoint.x << " " << drawPoint.y << endl;
            }
            out.close();
        }
    }
    SetCurrentDirectory(currentDir);
}
void MapleMonsterManager::LoadMonster()
{
    for(CNPC* npc : npcs)
        delete npc;
    npcs.clear();
    for(Snail1* snail : snail1s)
        delete snail;
    snail1s.clear();
    for(Snail2* snail : snail2s)
        delete snail;
    snail2s.clear();
    //mushMom = nullptr;
    delete mushMom;

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
    ofn.lpstrInitialDir = L"..\\Data\\MonsterData";

    if (GetOpenFileName(&ofn) != 0)
    {
        ifstream in(ofn.lpstrFile);
        if (in.is_open())
        {
            MessageBox(NULL, L"Load", L"Load", MB_OK);
            ifstream in(ofn.lpstrFile);
            if (!in.is_open())
            {
                MessageBox(NULL, L"File Open Error", L"Error", MB_OK);
                return;
            }
            SetCurrentDirectory(currentDir);
            int monsterType;
            POINT drawPoint = { 0,0 };
            while (!in.eof())
            {
                in >> monsterType >> drawPoint.x >> drawPoint.y;
                switch (monsterType)
                {
                case 1:
                    SettingSnail1(drawPoint);
                    break;
                case 2:
                    SettingSnail2(drawPoint, 0);
                    break;
                case 3:
                    SettingSnail2(drawPoint, 1, 8, 50, 15, 3);
                    break;
                case 4:
                    SettingMushMom(drawPoint);
                    break;
                case 5:
                case 6:
                    SettingNPC(drawPoint,monsterType);
                    break;
                }
            }
            in.close();
        }
    }
    SettingMonsters();
    SetCurrentDirectory(currentDir);
}

void MapleMonsterManager::LoadMonsterData(int type)
{
   
    string fileName;
    switch (type)
    {
    case 0:
        fileName = "../Data/MonsterData/NPCs.txt";
        break;
    case 1:
        fileName = "../Data/MonsterData/MapMonsters.txt";
        break;
    case 2:
        fileName = "../Data/MonsterData/BossMapMonsters.txt";
        break;
    default:
        break;
    }
    ifstream in(fileName);
    if (in.is_open())
    {
        for (CNPC* npc : npcs)
            delete npc;
        npcs.clear();
        for (Snail1* snail : snail1s)
            delete snail;
        snail1s.clear();
        for (Snail2* snail : snail2s)
            delete snail;
        snail2s.clear();
        //mushMom = nullptr;
       // delete mushMom;
        while (!in.eof())
        {
            int monsterType;
            POINT drawPoint = { 0,0 };
            while (!in.eof())
            {
                in >> monsterType >> drawPoint.x >> drawPoint.y;
                switch (monsterType)
                {
                case 1:
                    SettingSnail1(drawPoint);
                    break;
                case 2:
                    SettingSnail2(drawPoint, 0);
                    break;
                case 3:
                    SettingSnail2(drawPoint, 1, 8, 50, 15, 3);
                    break;
                case 4:
                    SettingMushMom(drawPoint);
                    break;
                case 5:
                case 6:
                    SettingNPC(drawPoint , monsterType);
                    break;
                }
            }
        }
        SettingMonsters();
    }
    in.close();
}

void MapleMonsterManager::Render(HDC hdc)
{
    DrawBitmapDoubleBuffering(hdc, &camera);
    TCHAR str[256];
    
    switch (inputMonseterType)
    {
    case MonsterType::Snail1:
        wsprintf(str, L"Monster type : Snail1");
        drawAreaWidth = 45;
        drawAreaHeight = 45;
        break;
    case MonsterType::Snail2:
        wsprintf(str, L"Monster type : Snail2");
        drawAreaWidth = 60;
        drawAreaHeight = 40;
        break;
    case MonsterType::Snail3:
        wsprintf(str, L"Monster type : Snail3");
        drawAreaWidth = 60;
        drawAreaHeight = 40;
        break;
    case MonsterType::MushMom:
        wsprintf(str, L"Monster type : MushMom");
        drawAreaWidth = 140;
        drawAreaHeight = 130;
        break;
    case (MonsterType)5:
        wsprintf(str, L"NPC type : Hinz");
        drawAreaWidth = 90;
        drawAreaHeight = 160;
        break;
    case (MonsterType)6:
        wsprintf(str, L"NPC type : Lucas");
        drawAreaWidth = 55;
        drawAreaHeight = 75;
        break;
    }
    // >> : collision
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    HGDIOBJ hOldPen = SelectObject(hdc, hPen);

    HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);

    Rectangle(hdc, dragMousePos->x, dragMousePos->y, dragMousePos->x + drawAreaWidth, dragMousePos->y + drawAreaHeight);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, hOldPen);

    DeleteObject(myBrush);
    DeleteObject(hPen);
    // << : collision

    TextOut(hdc, 0, 0, str, wcslen(str));
}

LRESULT MapleMonsterManager::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
    case WM_KEYUP:
        MapMovingButton(message, message == WM_KEYDOWN, wParam);
        break;
    case WM_CHAR:
        MapButtonChar(message, wParam);
        break;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MOUSEMOVE:
        MapMouseControl(message, wParam, lParam);
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
                    zoomRatio = 1.0f;
            }
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(g_hWnd, message, wParam, lParam);
    return LRESULT();
}

