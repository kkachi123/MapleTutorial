#include "Snail2.h"
#include "TileMap.h"
Snail2::Snail2(int type)
{
	monstertype = (Sanil2Type)type;
	if(monstertype == Sanil2Type::blue)
		hMonster.Load(L"../Data/img/monster/snail2.bmp");
	else if(monstertype == Sanil2Type:: red)
		hMonster.Load(L"../Data/img/monster/snail3.bmp");
	if (hMonster == NULL)
		MessageBox(NULL, L"MonsterImage Failed", L"Error", MB_OK);
	else
		GetObject(hMonster, sizeof(BITMAP), &bitAnime);

	drawForward = TRUE;

	spriteSizeX = 60;
	spriteSizeY = 40;

	bitRow = 0;
	bitCol = 0;
	bitColMin = 0;
	bitColMax = 0;

	drawPoint = { 350, 250 };

	collsionYStart = 10;
	idleCount = 0;
    
    isAlive = true;
}

Snail2::~Snail2()
{
	hMonster.Destroy();
}

void Snail2::init(POINT drawPoint, int exp, int hp, int Damage, int Defense)
{
	isAlive = true;
	this->spawnPoint = drawPoint;
	this->drawPoint = drawPoint;

	update = moveState::move;
    prevUpdate = moveState::move;
	moveTick = 0;

	Speed = 3;
	this->hp = hp;
    maxHp = hp;
	mp = 0;
    this->exp = exp;
	this->Damage = Damage;
	this->Defense = Defense;
	IsLeft = true;


	collisionX = 60;
	collisionY = 40;
}

extern vector<TileRect*> tileMap;
void Snail2::Update(float deltaTime)
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
			bitRow = 3;
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
void Snail2::Idle()
{
    
	if(IsLeft)
	{
		bitCol = 0;
	}
	else
	{
		bitCol = 1;
	}
	idleCount++;
	if (idleCount == 10)
	{
		update = moveState::move;
		idleCount = 0;
	}
}

extern CPlayer player;
void Snail2::Move()
{
    if (hp < maxHp)
        Tracking(player.GetPoint());
	// : draw
	{
		if (IsLeft)
		{
			bitColMin = 0;
			bitColMax = 3;
			drawPoint.x -= Speed;
		}
		else
		{
			bitColMin = 4;
			bitColMax = 7;
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
		{
			bitCol++;
		}
		else
		{
			bitCol--;
		}
	}
	// : draw

	// : draw
	BOOL turn = true;
	int downTilePos = drawPoint.y + collisionY + 10; // 자체 raycast
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

void Snail2::Hit(int damage)
{
    playDamageSound();
    if(hp <= 0)
        playDeathSound();

    if(prevUpdate == moveState::death || prevUpdate == moveState::spawn)
    {
        return;
    }
	hp -= damage;
	bitRow = 2;
	if (IsLeft)
	{
		bitCol = 0;
	}
	else
	{
		bitCol = 1;
	}

	if (hp <= 0)
	{
        hp = 0;
        playDeathSound();
		update = moveState::death;
		if (IsLeft)
			bitCol = 0;
		else
			bitCol = 5;
	}
	else
		update = moveState::idle;
}

void Snail2::Death()
{
    GetExp();
	if (IsLeft)
	{
		bitColMin = 0;
		bitColMax = 2;
		if (bitCol == bitColMax)
		{
			bitCol = 2;
			drawPoint = spawnPoint;
			update = moveState::spawn;
		}
		else
			bitCol++;
	}
	else
	{
		bitColMin = 5;
		bitColMax = 3;
		if (bitCol == bitColMax)
		{
            isAlive = false;
			bitCol = 2;
			drawPoint = spawnPoint;
			IsLeft = true;
			update = moveState::spawn;
		}
		else
			bitCol--;
	}
}

void Snail2::Spawn()
{
	if (bitCol == 0)
	{
		hp = maxHp;
		update = moveState::idle;
		isAlive = true;
	}
	else
		bitCol--;
}

void Snail2::Draw(HDC& hdc, BOOL openGuide)
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
