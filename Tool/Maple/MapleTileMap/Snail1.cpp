#include "Snail1.h"
#include "TileMap.h"
Snail1::Snail1()
{
    hMonster.Load(L"../Data/img/monster/snail1_2.bmp");
    if (hMonster == NULL)
    {
        //MessageBox(NULL, L"MonsterImage Failed", L"Error", MB_OK);
        //return;
    }
    else
        GetObject(hMonster, sizeof(BITMAP), &bitAnime);

    drawForward = TRUE;

    spriteSizeX = 45;
    spriteSizeY = 45;

    bitRow = 0;
    bitCol = 0;
    bitColMin = 0;
    bitColMax = 0;

    drawPoint = { 350, 250 };

    collsionYStart = 10;
    idleCount = 0;

    isAlive = true;
}

Snail1::~Snail1()
{
    hMonster.Destroy();
}

void Snail1::init(POINT drawPoint)
{
    isAlive = true;
    this->spawnPoint = drawPoint;
    this->drawPoint = drawPoint;

    update = moveState::move;
    prevUpdate = moveState::move;
    moveTick = 0;

    Speed = 3;
    hp = 15;
    maxHp = hp;
    mp = 0;
    exp = 2;
    Damage = 3;
    Defense = 0;
    IsLeft = false;


    collisionX = 40;
    collisionY = 40;
}

extern vector<TileRect*> tileMap;
void Snail1::Update(float deltaTime)
{
    moveTick += deltaTime;
    if (moveTick >= 0.1f)
    {
        switch (update)
        {
        case moveState::idle:
            prevUpdate = moveState::idle;
            bitRow = 0;
            Idle();
            break;
        case moveState::move:
            prevUpdate = moveState::move;
            bitRow = 1;
            Move();
            break;
        case moveState::death:
            prevUpdate = moveState::death;
            Death();
            break;
        case moveState::spawn:
            prevUpdate = moveState::spawn;
            Spawn();
            break;
        case moveState::hit:
            Hit(5);
            break;
        }
        moveTick = 0;
    }
}
void Snail1::Idle()
{
    idleCount++;

    if (IsLeft)
        bitCol = 0;
    else
        bitCol = 1;
    if (idleCount == 10)
    {
        update = moveState::move;
        idleCount = 0;
    }
}

extern CPlayer player;
void Snail1::Move()
{
    if (hp < maxHp)
        Tracking(player.GetPoint());
    // : draw
    {
        if (IsLeft)
        {
            bitColMin = 0;
            bitColMax = 4;
            drawPoint.x -= Speed;
        }
        else
        {
            bitColMin = 5;
            bitColMax = 9;
            drawPoint.x += Speed;
        }

        if (bitCol >= bitColMax)
        {
            drawForward = false;
            bitCol = bitColMax;
        }
        else if (bitCol <= bitColMin)
        {
            drawForward = true;
            bitCol = bitColMin;
        }
        if (drawForward)
            bitCol++;
        else
            bitCol--;
    }
    // : draw
    BOOL turn = true;
    int downTilePos = drawPoint.y + collisionY + 10; // ��ü raycast
    int nextXEnd = 0;
    if (IsLeft)
        nextXEnd = drawPoint.x - Speed;
    else
        nextXEnd = drawPoint.x + collisionX + Speed;
    for (auto& tile : tileMap)
    {
        if (tile->GetPos().y <= downTilePos && downTilePos <= tile->GetPos().y + tile->GetLength())
        {
            if (tile->GetPos().x <= nextXEnd && nextXEnd <= tile->GetPos().x + tile->GetLength())
            {
                if (tile->GetType() == MapType::Platform)
                    turn = false;
            }
        }
    }
    if (turn)
    {
        IsLeft = !IsLeft;
        update = moveState::idle;
    }
}
void Snail1::Hit(int damage)
{
    playDamageSound();
    if (prevUpdate == moveState::death || prevUpdate == moveState::spawn)
    {
        return;
    }
    hp -= damage;
    if (IsLeft)
    {
        bitRow = 2;
        bitCol = 0;
    }
    else
    {
        bitRow = 3;
        bitCol = 8;
    }

    if (hp <= 0)
    {
        hp = 0;
        update = moveState::death;
        playDeathSound();
        if (IsLeft)
            bitCol = 0;
        else
            bitCol = 8;
    }
    else
    {
        update = moveState::idle;
    }
}
void Snail1::Death()
{
    GetExp();
    if (IsLeft)
    {
        bitRow = 2;
        bitColMin = 0;
        bitColMax = 8;
        if (bitCol == bitColMax)
        {
            bitCol = 8;
            drawPoint = spawnPoint;
            update = moveState::spawn;
        }
        else
            bitCol++;
    }
    else
    {
        bitRow = 3;
        bitColMin = 8;
        bitColMax = 0;
        if (bitCol == bitColMax)
        {
            isAlive = false;
            bitRow = 2;
            bitCol = 8;
            drawPoint = spawnPoint;
            IsLeft = true;
            update = moveState::spawn;
        }
        else
            bitCol--;
    }

}

void Snail1::Spawn()
{
    if (bitCol == 1)
    {
        hp = maxHp;
        update = moveState::idle;
        isAlive = true;
    }
    else
        bitCol--;
}

void Snail1::Draw(HDC& hdc, BOOL openGuide)
{
    // camera 내부가 아닌 경우 그리지 않기
    if ((drawPoint.x + spriteSizeX) * zoomRatio < camera.GetPos().x || (drawPoint.x + spriteSizeX) * zoomRatio> camera.GetPos().x + camera.GetWidth() || (drawPoint.y + spriteSizeY) * zoomRatio< camera.GetPos().y || (drawPoint.y + spriteSizeY) * zoomRatio> camera.GetPos().y + camera.GetHeight())
        return;

    int screenX = (drawPoint.x - mapMove.x / zoomRatio) * zoomRatio;
    int screenY = (drawPoint.y - mapMove.y / zoomRatio) * zoomRatio;



    if (hp < maxHp)
    {
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
        float percent = hp / maxHp;
        Rectangle(hdc, screenX, screenY - 10, screenX + spriteSizeX * percent, screenY - 5);
        SelectObject(hdc, oldBrush);
        DeleteObject(hBrush);
    }
    hMonster.TransparentBlt(hdc, screenX, screenY, spriteSizeX * zoomRatio, spriteSizeY * zoomRatio, bitCol * spriteSizeX, bitRow * spriteSizeY, spriteSizeX, spriteSizeY, RGB(255, 0, 255));

    if (openGuide)
    {
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
        HGDIOBJ hOldPen = SelectObject(hdc, hPen);

        HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);

        Rectangle(hdc, screenX, screenY, screenX + spriteSizeX * zoomRatio, screenY + spriteSizeY * zoomRatio);

        SelectObject(hdc, oldBrush);
        SelectObject(hdc, hOldPen);

        DeleteObject(myBrush);
        DeleteObject(hPen);
    }
}

