// MapleTileMap.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "MapleTileMap.h"
#include "MonsterManager.h"
#include "PlayManager.h"
#include "SceneManager.h"
#include "Billage.h"
#include "framework.h"
#include "FinalAttack.h"
#include "SoundManager.h"

#define MAX_LOADSTRING 100

// >> :
HWND g_hWnd;
int flag = 2;
MaplePlayManager* playManager;
MapleSceneManager* sceneManager;
MapleMonsterManager* monsterManager;

MapObject portal(0, 0, 0, 7, 100, 150, { 3560,840 }, 8);

Billage henesys;
BillageType billageType;
// << :
bool IsClear{ false };
// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


// >> : monster
CPlayer player;
vector<Snail1*> snail1s;
vector<Snail2*> snail2s;
vector<Snail2> seversnail2s;
MushMom* mushMom;
vector<TileRect*> tileMap;
vector<Skill> mageSkills(3);
vector<Skill> WarriorSkills(3);
vector<CPlayer> otherPlayers;
vector<CMonster*> monsters;
vector<CNPC*> npcs;
LevelUpEffect levelUp;


CMonster ArrayMonsters[20];
extern FinalAttack finalAttack1;
extern FinalAttack finalAttack2;
Gdiplus::Bitmap* mageBitmap = new Gdiplus::Bitmap(L"../Data/img/Player/mage/mage.png");
Gdiplus::Bitmap* mageBitmap_R = new Gdiplus::Bitmap(L"../Data/img/Player/mage/mage_Right.png");

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // Initialize GDI+.
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MAPLETILEMAP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    CreateBitmap(0);
    billageType = BillageType::Henesys;
    // 스킬 초기화
    mageSkills.resize(3);
    {
        mageSkills[0].SetSkillType(SkillType::three_snail);
        mageSkills[0].Init();
        mageSkills[1].SetSkillType(SkillType::double_step);
        mageSkills[1].Init();
        mageSkills[2].SetSkillType(SkillType::genesis);
        mageSkills[2].Init();
    }

    WarriorSkills.resize(3);
    {
        WarriorSkills[0].SetSkillType(SkillType::three_snail);
        WarriorSkills[0].Init();
        WarriorSkills[1].SetSkillType(SkillType::slash_blast);
        WarriorSkills[1].Init();
        WarriorSkills[2].SetSkillType(SkillType::double_step);
        WarriorSkills[2].Init();
    }
    //

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAPLETILEMAP));

    GetClientRect(g_hWnd, &rectView);
    playManager = new MaplePlayManager();
    sceneManager = new MapleSceneManager();
    monsterManager = new MapleMonsterManager();
    levelUp.Init();
    player.Init(2);
    playManager->SetUp();
    switch (flag)
    {
    case 1:
        playManager->SetUp();
        player.SetPoint({ 170,470 });
        LoadTileData(&tileMap, 2);
        player.mapNum = 2;
        monsterManager->LoadMonsterData(2);
        billageType = BillageType::Boss;
        break;
    case 2:
        sceneManager->SetUp();
        break;
    default:
        break;
    }


    MSG msg;


    DWORD previousTime = GetTickCount64();
    float deltaTime = 0.0f;
    player.playerHitTime = 0.0f;
    float visibleTime = 0.f;

    HDC hdc = GetDC(g_hWnd);
    while (1)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                return (int)msg.wParam;
            }

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        DWORD currentTime = GetTickCount64();
        deltaTime += (currentTime - previousTime) / 1000.0f;
        if (deltaTime > 0.01f)
        {

            if(!IsClear )
            {
                switch (flag)
                {
                case 1:
                    playManager->Render(hdc);
                    break;
                case 2:
                    sceneManager->Render(hdc);
                    break;
                }
            }
            else
            {
                sceneManager->DrawEnding(hdc);
                PlaySound(ENDING, NULL, SND_FILENAME | SND_ASYNC);
            }
            if (flag == 1)
            {
                if (finalAttack1.GetAttackStart() || finalAttack2.GetAttackStart());
                else
                    player.Update();
                playManager->MoveMap(&portal, &player);
            }

            portal.Update();

            for (vector<CMonster*>::iterator it = monsters.begin(); it != monsters.end(); ++it)
            {
                MonsterType type = (*it)->GetMonsterType();
                switch (type)
                {
                case MonsterType::MushMom:
                    static_cast<MushMom*>(*it)->Update(deltaTime);
                    if(static_cast<MushMom*>(*it)->GetIsAlive() == false)
                        IsClear = true;
                    break;
                case MonsterType::Snail1:
                    static_cast<Snail1*>(*it)->Update(deltaTime);
                    break;
                case MonsterType::Snail2:
                case MonsterType::Snail3:
                    static_cast<Snail2*>(*it)->Update(deltaTime);
                    break;

                }
            }

            for (CNPC* npc : npcs)
                npc->Update(deltaTime);


            // >> : mapmoving
            switch (flag)
            {
            case 1:
                playManager->PlayerCameraMove();
                break;
            default:
                break;
            }
            // << : mapmoving
            if (!mageSkills.empty())
            {
                for (Skill& s : mageSkills)
                {
                    if (s.IsStart)
                        s.Update();
                }
            }
            if (!WarriorSkills.empty())
            {
                for (std::vector<Skill>::iterator it = WarriorSkills.begin(); it != WarriorSkills.end(); ++it)
                {
                    if (it->IsStart)
                        (*it).Update();
                }
            }
            if (player.GetState() != playerState::death)
                if (player.playerHitTime > 0)
                {
                    if (visibleTime > 0.1f)
                    {
                        player.SetIsVisible(!player.GetIsVisible());
                        visibleTime = 0;
                    }
                    if (player.playerHitTime < 0.5f)
                        player.SetIsVisible(true);
                }
                else
                {
                    if (player.CollsionCheckToMonster())
                    {
                        player.playerHitTime = 2.1f;
                    }
                    player.SetIsVisible(true);
                }
            else
            {
                if(INPUTMGR->GetInstance()->OnceKeyDown(VK_RETURN))
                {

                    player.SetPoint({ 96,850 });
                    player.mapNum = 0;
                    player.SetHp(player.GetMaxHp());
                    portal.SetDrawPoint({ 3560,840 });
                    LoadTileData(&tileMap, 0);
                    billageType = BillageType::Henesys;
                    //monsterManager->LoadMonsterData(0);
                    player.SetState(playerState::idle);
                }
            }

            player.playerHitTime -= deltaTime;
            visibleTime += deltaTime;
            deltaTime = 0;

        }
        previousTime = currentTime;
    }

    delete playManager;
    delete sceneManager;
    delete monsterManager;
    delete mushMom;

    for (CMonster* monster : monsters)
    {
        delete monster;
    }
    for (TileRect* tile : tileMap)
    {
        delete tile;
    }
    DestroyBitmap();

    return (int)msg.wParam;
}




//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAPLETILEMAP));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MAPLETILEMAP);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX),
        0, 0, 1500, 1000, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }
    g_hWnd = hWnd;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}
//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (playManager && flag == 1)
    {
        playManager->WndProc(message, wParam, lParam);
    }
    if (sceneManager && flag == 2)
    {
        sceneManager->WndProc(message, wParam, lParam);
    }
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case ID_BUTTONGUIDE:
            openButtonGuide = !openButtonGuide;
            InvalidateRect(hWnd, NULL, false);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
