#include "MushMom.h"
#include "TileMap.h"
#include "EffectAnimation.h"
#include "SoundManager.h"


MushMom::MushMom()
{
	hMonster.Load(L"../Data/img/monster/MushMom.bmp");

	drawForward = TRUE;

	spriteSizeX = 140;
	spriteSizeY = 140;

	bitRow = 0;
	bitCol = 0;
	bitColMin = 0;
	bitColMax = 0;

	drawPoint = { 350, 250 };

	idleCount = 0;

	// >> Death
	hMonsterDeath.Load(L"../Data/img/monster/MushMomDeath.bmp");
	deathSpriteSizeX = 200;
	deathSpriteSizeY = 200;

	deathBitRow = 0;
	deathBitCol = 0;
	deathBitColMin = 0;
	deathBitColMax = 0;

	deathDrawForward = TRUE;
	// << Death

    wchar_t filename[] = L"../Data/img/monster/MushMomAttackEffect.bmp";
    attackEffect = new EffectAnimation(filename , IsLeft, 140, 100, 0, 0, 4, 0 ,9, 5);
    wchar_t filename2[] = L"../Data/img/monster/MushMomLandingEffect.bmp";
    attackLandingEffect = new EffectAnimation(filename2, IsLeft, 250, 100, 0, 0, 5, 1, 5, 0);

}

MushMom::~MushMom()
{
    if(hMonster != NULL)
	    hMonster.Destroy();
    delete attackEffect;
    delete attackLandingEffect;
}

void MushMom::init(POINT drawPoint)
{
	isAlive = true;
	this->spawnPoint = drawPoint;
	this->drawPoint = drawPoint;

    update = moveState::move;
    prevUpdate = moveState::move;
	moveTick = 0;

	Speed = 5;
	jumpForce = 30;
	hp = 17500;
	maxHp = 17500;
	mp = 2000;
	exp = 1650;
	Damage = 120;
	Defense = 320;
	IsLeft = true;


	collisionX = 140;
	collisionY = 140;
	rayFloor = 10;
	landingPosY = drawPoint.y;

    attackEffect->SettingEffectBit();
}
// >> : Sound
// PlaySound(damage, death 사운드)와 개별적 소리를 내기 위해 MCI를 사용
void MushMom::playAttackStandSound()
{
    MCI_OPEN_PARMS mushMomAttackStand;
    mushMomAttackStand.lpstrElementName = MUSHMOMATTACKSTAND;
    mushMomAttackStand.lpstrDeviceType = L"mpegvideo";
    mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&mushMomAttackStand);
    AttackStandSoundID = mushMomAttackStand.wDeviceID;

    mciSendCommand(AttackStandSoundID, MCI_PLAY, 0, (DWORD)(LPVOID)&mushMomAttackStand);
}
void MushMom::playAttackSound()
{
    MCI_OPEN_PARMS mushMomAttack;
    mushMomAttack.lpstrElementName = MUSHMOMATTACK;
    mushMomAttack.lpstrDeviceType = L"mpegvideo";
    mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&mushMomAttack);
    AttackSoundID = mushMomAttack.wDeviceID;

    mciSendCommand(AttackSoundID, MCI_PLAY, 0, (DWORD)(LPVOID)&mushMomAttack);
}
void MushMom::stopSound(int id)
{
    int soundID = -1;
    switch (id)
    {
    case 0:
        soundID = AttackSoundID;
        break;
    case 1:
        soundID = AttackStandSoundID;
        break;
    }
    if (soundID != -1)
        mciSendCommand(soundID, MCI_CLOSE, 0, (DWORD)(LPVOID)NULL);

}
// << : Sound
extern vector<TileRect*> tileMap;
void MushMom::Update(float deltaTime)
{
	moveTick += deltaTime;
	if(moveTick >= 0.1f)
	{
		switch (update)
		{
        case moveState::idle:
            prevUpdate = moveState::idle;
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
		case moveState::attack:
			bitRow = 3;
			Attack();
			break;
        case moveState::landing:
            Landing();
            break;
		}
		moveTick = 0;
	}
}
void MushMom::Idle()
{
    
    attackEffect->SetIsLeft(IsLeft);
    attackEffect->SettingEffectBit();
    drawPoint.y = landingPosY;
    rayFloor = 0;

   	if(hp < maxHp)
		bitRow = 3;
	else
		bitRow = 1;
	int chooseMotion = rand() % 2;
	if (IsLeft)
	{
		if (hp < maxHp)
			bitCol = 0;
		else
		{
			if (chooseMotion == 0)
				bitCol = 0;
			else
				bitCol = 3;
		}
	}
	else
	{
		if (hp < maxHp)
			bitCol = 11;
		else
		{
			if (chooseMotion == 0)
				bitCol = 9;
			else
				bitCol = 6;
		}
	}
	idleCount++;
	if (idleCount == 10)
	{
		if(hp < maxHp)
        {
            playAttackStandSound();
            update = moveState::attack;
        }
		else
			update = moveState::move;
		idleCount = 0;
	}
}
extern CPlayer player;

void MushMom::Move()
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
		if (bitCol >= 3 && bitCol <= 6)
		{
			if ((drawForward && IsLeft)||(!drawForward && !IsLeft))
			{
				drawPoint.y -= jumpForce;
				rayFloor += jumpForce;
			}
			else if((drawForward && !IsLeft) || (!drawForward && IsLeft))
			{
				drawPoint.y += jumpForce;
				rayFloor -= jumpForce;
			}
		}
		if (bitCol == 2 || bitCol == 7)
		{
			drawPoint.y = landingPosY;
			rayFloor = 0;
		}
	}
	// : draw

	BOOL turn = true;
	int downTilePos = drawPoint.y + collisionY + rayFloor; // 자체 raycast
	int nextXEnd = 0;
	if (IsLeft)
		nextXEnd = drawPoint.x - Speed;
	else
		nextXEnd = drawPoint.x + collisionX + Speed;
	for (auto& tile : tileMap)
	{
        if (tile->GetType() == MapType::Platform)
		    if (tile->GetPos().y <= downTilePos && downTilePos <= tile->GetPos().y + tile->GetLength())
		    {
			    if (tile->GetPos().x <= nextXEnd && nextXEnd <= tile->GetPos().x + tile->GetLength())
			    {
                    {
                        turn = false;
                    }
			    }
		    }
	}
    int limitIdleCount = 0;
    if(hp < maxHp)
        limitIdleCount = 20;
    else
        limitIdleCount = 150;
    idleCount++;
    if (idleCount == limitIdleCount)
    {
        turn = true;
    }
	if (turn)
	{
        if (hp >= maxHp)
            IsLeft = !IsLeft;
		drawPoint.y = landingPosY;
		rayFloor = 0;
        idleCount = 0;
		update = moveState::idle;
	}

}

void MushMom::Hit(int damage)
{
    playDamageSound();
    if (hp - HitDamage <= 0)
    {
        stopSound(0);
        stopSound(1);
        playDeathSound();
    }
    if(prevUpdate == moveState::death || prevUpdate == moveState::spawn)
    {
        return;
    }
	hp -= damage;
    if(prevUpdate == moveState::move)
    {
        bitRow = 2;
        if (IsLeft)
        {
            bitCol = 0;
        }
        else
        {
            bitCol = 1;
        }
        idleCount = 0;
        update = moveState::idle;
    }

	if (hp <= 0)
	{
        stopSound(0);
        stopSound(1);
        playDeathSound();
		update = moveState::death;

        if (IsLeft)
		{
			deathBitRow = 0;
			deathBitCol = 0;
		}
		else
		{
			deathBitRow = 1;
			deathBitCol = 5;
		}
	}
}

void MushMom::Attack()
{
    prevUpdate = moveState::attack;
	// : draw
	{
		if (IsLeft)
		{
			bitColMin = 0;
			bitColMax = 5;
			drawForward = true;
		}
		else
		{
			bitColMin = 11;
			bitColMax = 6;
			drawForward = false;
		}

		BOOL jumping = false;
		if (bitCol == bitColMin)
		{
			idleCount++;
			attackEffect->GenrateEffect();
			if (idleCount == 20)
			{
				jumping = true;
				idleCount = 0;
			}
		}
		else
		{
			jumping = true;
		}

		if(jumping)
		{
            stopSound(1);
            playAttackSound();
			if (drawForward)
				bitCol++;
			else
				bitCol--;
		}



		if (bitCol == 1 || bitCol == 3|| bitCol == 8 || bitCol == 10)
		{
			if (bitCol == 1 || bitCol == 10)
			{
				drawPoint.y -= jumpForce * 3;
				rayFloor += jumpForce * 3;
			}
			else
			{
				drawPoint.y += jumpForce * 3;
				rayFloor -= jumpForce * 3;
			}
		}

		if (bitCol == 4 || bitCol == 7)
		{
			drawPoint.y = landingPosY;
			rayFloor = 0;
		}
        if(bitCol == 5 || bitCol == 6)
        {
            attackLandingEffect->SetGenerate(TRUE);
            int downTilePos = drawPoint.y + collisionY + rayFloor; // 자체 raycast
            for (auto& tile : tileMap)
            {
                if (tile->GetPos().y <= downTilePos && downTilePos <= tile->GetPos().y + tile->GetLength())
                {
                    if(tile->GetType() == MapType::Platform)
                        tile->SetType(MapType::Attack);
                }
            }
        }

		if ((IsLeft && bitCol >= bitColMax) || (!IsLeft && bitCol <= bitColMax))
		{
			bitCol = bitColMin;
            if(attackLandingEffect->GetGenerate())
            {
                update = moveState::landing;
                attackLandingEffect->SetIsLeft(IsLeft);
                attackLandingEffect->SettingEffectBit();
            }
            else
            {
                IsLeft = !IsLeft;
                update = moveState::idle;
            }
		}
	}
	// : draw
}

void MushMom::Landing()
{
    attackLandingEffect->GenrateEffect();
    idleCount++;
    if (idleCount == 3)
    {
        for (auto& tile : tileMap)
        {
            if (tile->GetType() == MapType::Attack)
            {
                tile->SetType(MapType::Platform);
            }
        }
    }
    if (idleCount == 15)
    {
        stopSound(0);

        attackLandingEffect->SetGenerate(FALSE);
        idleCount = 0;
        update = moveState::move;
    }
}
void MushMom::Death()
{
    GetExp();
    
	if (IsLeft)
	{
		deathBitColMin = 0;	
		deathBitColMax = 5;
		deathBitCol++;
	}
	else
	{
		deathBitColMin = 5;
		deathBitColMax = 0;
		deathBitCol--;
	}

	if (deathBitCol == deathBitColMax)
	{
        isAlive = false;
		deathBitRow = 0;
		deathBitCol = 5;
		drawPoint = spawnPoint;
		IsLeft = true;
		//update = moveState::spawn;

	}
}

void MushMom::Spawn()
{
	if (deathBitCol == 0)
	{
		hp = maxHp;
        idleCount = 0;
		update = moveState::idle;
		isAlive = true;

		bitCol = 0;
		bitRow = 0;

		deathBitCol = 0;
		deathBitRow = 0;
	}
	else
		deathBitCol--;
}

void MushMom::Draw(HDC& hdc , BOOL openGuide)
{
    // camera 내부가 아닌 경우 그리지 않기
    if ((drawPoint.x + spriteSizeX) * zoomRatio < camera.GetPos().x || (drawPoint.x + spriteSizeX) * zoomRatio> camera.GetPos().x + camera.GetWidth() || (drawPoint.y + spriteSizeY) * zoomRatio< camera.GetPos().y || (drawPoint.y + spriteSizeY) * zoomRatio> camera.GetPos().y + camera.GetHeight())
        return;
	int screenX = (drawPoint.x - mapMove.x / zoomRatio) * zoomRatio;
	int screenY = (drawPoint.y - mapMove.y / zoomRatio) * zoomRatio;
	if (update == moveState::death || update == moveState::spawn)
		hMonsterDeath.TransparentBlt(hdc, screenX - 20 * zoomRatio, screenY - 40 * zoomRatio, deathSpriteSizeX * zoomRatio, deathSpriteSizeY * zoomRatio, deathBitCol * deathSpriteSizeX, deathBitRow * deathSpriteSizeY, deathSpriteSizeX, deathSpriteSizeY, RGB(255, 0, 255));
	else
	{
		hMonster.TransparentBlt(hdc, screenX, screenY, spriteSizeX * zoomRatio, spriteSizeY * zoomRatio, bitCol * spriteSizeX, bitRow * spriteSizeY, spriteSizeX, spriteSizeY, RGB(255, 0, 255));
		if(update == moveState::attack)
		{
            int effectScreenY = 0;
            effectScreenY = (drawPoint.y - mapMove.y / zoomRatio + 25) * zoomRatio;
            attackEffect->Draw(hdc, { screenX, effectScreenY }, zoomRatio);
		}
        else if(update == moveState::landing)
        {
            if (attackLandingEffect->GetGenerate())
            {
                int landingScreenX = 0;
                int landingScreenY = 0;
                if (IsLeft)
                {
                    landingScreenX = (drawPoint.x - mapMove.x / zoomRatio - 20) * zoomRatio;
                    landingScreenY = (drawPoint.y - mapMove.y / zoomRatio + 50) * zoomRatio;
                }
                else
                {
                    landingScreenX = (drawPoint.x - mapMove.x / zoomRatio - 50) * zoomRatio;
                    landingScreenY = (drawPoint.y - mapMove.y / zoomRatio + 50) * zoomRatio;

                }
                attackLandingEffect->Draw(hdc, { landingScreenX, landingScreenY }, zoomRatio);
            }
        }
	}
    
    if(openGuide)
    {
        // >> : collision
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
        HGDIOBJ hOldPen = SelectObject(hdc, hPen);

        HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);

        Rectangle(hdc, screenX, screenY, screenX + spriteSizeX * zoomRatio, screenY + spriteSizeY * zoomRatio);

        SelectObject(hdc, oldBrush);
        SelectObject(hdc, hOldPen);

        DeleteObject(myBrush);
        DeleteObject(hPen);
        // << : collision
        char str[100];
        sprintf_s(str, "MushMom DrawPoint : %d %d", drawPoint.x, drawPoint.y);
        TextOutA(hdc, 0, 20, str, strlen(str));
    }
}