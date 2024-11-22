#include"FinalAttack.h"
#include "TileMap.h"
void FinalAttack::Init(int attackType)
{
    this->attackType = attackType;
    TCHAR filePath[256];
    switch (attackType)
    {
    case 1:
        maxFrame = 29;
        break;
    case 2:
        maxFrame = 30;
        break;
    }
    for (int i = 0; i <= maxFrame; i++)
    {
        wsprintf(filePath, L"../Data/img/skills/FinalAttack%d/attack%d_%d.png", attackType, attackType, i);
        CImage* img = new CImage();
        img->Load(filePath);
        AttackImage.push_back(img);
    }
    attackStart = false;
}

void FinalAttack::Draw(HDC hdc)
{

    int bx = AttackImage[attackFrame]->GetWidth();
    int by = AttackImage[attackFrame]->GetHeight();
    switch (attackType)
    {
    case 1:
        AttackImage[attackFrame]->TransparentBlt(hdc, (point.x - bx / 2 * zoomRatio), (point.y - by / 2 * zoomRatio), bx * zoomRatio, by * zoomRatio, 0, 0, bx, by, NULL);
        break;
    case 2:
        AttackImage[attackFrame]->TransparentBlt(hdc, (point.x - bx / 4 * zoomRatio), (point.y - by / 4 * zoomRatio), bx * zoomRatio / 2, by * zoomRatio / 2, 0, 0, bx, by, NULL);
        break;
    }
    attackFrame++;

    if (attackFrame >= maxFrame)
    {
        attackFrame = 0;
        attackEnd = TRUE;
        attackStart = false;
    }


    else if (attackFrame == maxFrame / 2)
    {
        if (!hitMonsters.empty() || hitMonsters.size() > 0)
        {
            for (vector<CMonster*>::iterator it = hitMonsters.begin(); it != hitMonsters.end(); ++it)
            {
                MonsterType type = (*it)->GetMonsterType();
                switch (type)
                {
                case MonsterType::MushMom:
                    static_cast<MushMom*>((*it))->HitDamage = Damage;
                    //static_cast<MushMom*>((*it))->Hit(Damage);
                    static_cast<MushMom*>((*it))->Hit(20000);
                    break;
                case MonsterType::Snail1:
                    static_cast<Snail1*>((*it))->HitDamage = Damage;
                    static_cast<Snail1*>((*it))->Hit(Damage);
                    break;
                case MonsterType::Snail2:
                case MonsterType::Snail3:
                    static_cast<Snail2*>((*it))->HitDamage = Damage;
                    static_cast<Snail2*>((*it))->Hit(Damage);
                    break;
                }
            }
            hitMonsters.clear();
        }
    }

}
