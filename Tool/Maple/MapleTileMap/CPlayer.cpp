#include "CPlayer.h"
#include "TileMap.h"
#include "FinalAttack.h"
#include "SoundManager.h"

extern vector<Skill> mageSkills;
extern vector<Skill> WarriorSkills;
extern vector<CMonster*> monsters;
extern vector<CNPC*> npcs;
extern MapObject portal;
extern FinalAttack finalAttack1;
extern FinalAttack finalAttack2;

extern Gdiplus::Bitmap* mageBitmap;
extern Gdiplus::Bitmap* mageBitmap_R;
extern LevelUpEffect levelUp;
void CPlayer::KeyCheck()
{
    if (IsAttack)
    {

    }
    else if (InputMgr::GetInstance()->StayKeyDown(VK_LEFT))
    {

        IsLeft = true;


        if (InputMgr::GetInstance()->OnceKeyDown(VK_SPACE))
        {
            IsJump = true;
            state = playerState::jumpMove;
            PlaySound(PLAYERJUMP, NULL, SND_ASYNC);
        }
        else if (state == playerState::idle)
            state = playerState::move;
        else if (state == playerState::drop)
        {
            point.x -= Speed / 2;
        }
        else if (state == playerState::radder || state == playerState::rope)
        {

        }

    }
    else if (InputMgr::GetInstance()->StayKeyDown(VK_RIGHT))
    {

        IsLeft = false;

        if (InputMgr::GetInstance()->OnceKeyDown(VK_SPACE))
        {
            IsJump = true;
            state = playerState::jumpMove;
            PlaySound(PLAYERJUMP, NULL, SND_ASYNC);
        }
        else if (state == playerState::idle)
            state = playerState::move;
        else if (state == playerState::drop)
        {
            point.x += Speed / 2;
        }
        else if (state == playerState::radder || state == playerState::rope)
        {

        }
    }
    else if (InputMgr::GetInstance()->StayKeyDown(VK_DOWN))
    {
        if (CollsionCheck(MapType::Rope))
        {
            state = playerState::rope;
            IsUp = false;
            IsRopeMove = true;
        }
        else if (state == playerState::rope)
        {
            state = playerState::drop;

        }
        else if (CollsionCheck(MapType::Ladder))
        {
            state = playerState::radder;
            IsUp = false;
            IsRopeMove = true;
        }
        else if (state == playerState::radder)
        {
            state = playerState::drop;
        }


    }
    else if (InputMgr::GetInstance()->StayKeyDown(VK_UP))
    {
        if (portal.PointIsIn(point))
        {
            moveMap = TRUE;
            return;
        }
        if (CollsionCheck(MapType::Rope))
        {
            if (state != playerState::idle && state != playerState::move)

            {
                state = playerState::rope;
                IsUp = true;
                IsRopeMove = true;

            }
        }
        else if (state == playerState::rope)
        {
            state = playerState::drop;

        }
        else if (CollsionCheck(MapType::Ladder))
        {
            if (state != playerState::idle && state != playerState::move)
            {
                state = playerState::radder;
                IsUp = true;
                IsRopeMove = true;

            }
        }
        else if (state == playerState::radder)
        {
            state = playerState::drop;
        }

    }




    if (IsAttack)

    {

    }
    else if (InputMgr::GetInstance()->OnceKeyDown(VK_SPACE))
    {

        for (CNPC* npc : npcs)
        {
            if (npc->getIsTalk() == TRUE || npc->PointIsIn(point))
            {
                npc->setIsTalk();
                return;
            }
        }

        if (state == playerState::idle)
        {
            state = playerState::jump;
            PlaySound(PLAYERJUMP, NULL, SND_ASYNC);
            IsJump = true;
        }
        else if (state == playerState::sit)
        {
            state = playerState::idle;
        }
        else if (state == playerState::move)
        {
            state = playerState::jumpMove;
            PlaySound(PLAYERJUMP, NULL, SND_ASYNC);
            IsJump = true;
        }
        else if (state == playerState::rope || state == playerState::radder)
        {
            state = playerState::jump;
            jumpForce = sjumpForce / 3;
            PlaySound(PLAYERJUMP, NULL, SND_ASYNC);
            IsJump = true;
        }




    }
    else if (InputMgr::GetInstance()->OnceKeyUp(VK_LEFT))
    {
        if (state == playerState::move)
            state = playerState::idle;

    }
    else if (InputMgr::GetInstance()->OnceKeyUp(VK_RIGHT))
    {
        if (state == playerState::move)
            state = playerState::idle;

    }
    else if (InputMgr::GetInstance()->OnceKeyUp(VK_DOWN))
    {
        if (CollsionCheck(MapType::Rope))
        {
            state = playerState::rope;
            IsUp = false;
            IsRopeMove = false;
        }
        else if (state == playerState::rope)
        {
            state = playerState::drop;

        }
        else if (CollsionCheck(MapType::Ladder))
        {
            state = playerState::radder;
            IsUp = false;
            IsRopeMove = false;
        }
        else if (state == playerState::radder)
        {
            state = playerState::drop;
        }
    }
    else if (InputMgr::GetInstance()->OnceKeyUp(VK_UP))
    {
        if (CollsionCheck(MapType::Rope))
        {
            if (state != playerState::idle && state != playerState::move)
            {
                state = playerState::rope;
                IsUp = true;
                IsRopeMove = false;

            }
        }
        else if (state == playerState::rope)
        {
            state = playerState::drop;

        }
        else if (CollsionCheck(MapType::Ladder))
        {
            if (state != playerState::idle && state != playerState::move)
            {
                state = playerState::radder;
                IsUp = true;
                IsRopeMove = false;

            }
        }
        else if (state == playerState::radder)
        {
            state = playerState::drop;
        }
    }

    // ��ų
    if (IsAttack)
    {

    }
    else if (InputMgr::GetInstance()->OnceKeyDown('A'))
    {
        PlaySound(PLAYERATTACK, NULL, SND_ASYNC);
        if (IsAttack == false && (state == playerState::idle || state == playerState::move))
        {
            IsAttack = true;
            AttackRect.Y = point.y - 10;
            AttackRect.Width = 100;
            if (IsLeft)
            {
                AttackRect.X = (point.x + width - AttackRect.Width);

            }
            else
            {
                AttackRect.X = (point.x);
            }

            AttackRect.Height = height + 10;
            state = playerState::attack;
            count = 0;
        }
        if (state == playerState::jump)

        {
            state = playerState::jumpAttack;
            IsJump = true;
            count = 0;
        }
        else if (state == playerState::jumpMove)
        {
            state = playerState::jumpMoveAttack;
            IsJump = true;
            count = 0;
        }





    }

    else if (InputMgr::GetInstance()->OnceKeyDown(VK_DELETE))
    {
        PlaySound(PLAYERMAGIC, NULL, SND_ASYNC);

        if (level > 1 && IsAttack == false && (state == playerState::idle || state == playerState::move))
        {
            IsAttack = true;
            AttackRect.Y = point.y - 10;
            AttackRect.Width = mageSkills[0].GetSkillWidth();;
            if (IsLeft)
            {
                AttackRect.X = (point.x - AttackRect.Width);

            }
            else
            {
                AttackRect.X = (point.x + width / 2);
            }

            AttackRect.Height = height + 10;
            state = playerState::skill1;
            count = 0;

        }
    }

    else if (InputMgr::GetInstance()->OnceKeyDown(VK_END))
    {
        PlaySound(PLAYERMAGIC, NULL, SND_ASYNC);
        if (level > 2 && IsAttack == false && (state == playerState::idle || state == playerState::move))
        {
            IsAttack = true;
            AttackRect.Y = point.y - 10;
            AttackRect.Width = mageSkills[1].GetSkillWidth();
            if (ID == 2)
                AttackRect.Width = WarriorSkills[1].GetSkillWidth();
            if (IsLeft)
            {
                AttackRect.X = (point.x + width / 2 - AttackRect.Width);

            }
            else
            {
                AttackRect.X = (point.x + width / 2);
            }

            AttackRect.Height = height + 10;
            if (ID == 2)
                AttackRect.Height += 50;
            state = playerState::skill2;
            count = 0;
        }

    }
    else if (InputMgr::GetInstance()->OnceKeyDown(VK_NEXT))
    {
        PlaySound(PLAYERMAGIC, NULL, SND_ASYNC);
        if (level > 3 && IsAttack == false && (state == playerState::idle || state == playerState::move))
        {
            IsAttack = true;
            AttackRect.Y = point.y - 200;
            if (ID == 1)
                AttackRect.Width = 600;
            else if (ID == 2)
            {
                AttackRect.Width = 600;
            }


            AttackRect.X = (point.x - AttackRect.Width / 2);



            AttackRect.Height = 400;
            state = playerState::skill3;
            count = 0;
        }


    }
    else if (InputMgr::GetInstance()->OnceKeyDown('X'))
    {
        if (state == playerState::idle)
        {
            count = 0;
            state = playerState::sit;
        }
        else if (state == playerState::sit)
        {
            count = 0;
            state = playerState::idle;
        }


    }




}

void CPlayer::DrawSkill1()
{
    if (ID == 1)
    {

        maxCount = 3;
        Maxtick = 80;
        count %= 3;

        ImgX = count * 60 + 6;
        ImgY = 313;
        ImgWidth = 60 + 6;
        ImgHeight = 69;
        if (count == 0)
        {
            ImgX = count * 60;
            ImgWidth = 60;
        }
    }
    else if (ID == 2)
    {
        Maxtick = 80;
        count %= 3;
        maxCount = 3;
        ImgWidth = 100;
        ImgX = 200 * count + 50;
        ImgY = 2800 + 60;
        rect.X = (point.x - WidthCorrectionValue - mapMove.x / zoomRatio) * zoomRatio;

    }

}

void CPlayer::DrawSkill2()
{
    if (ID == 1)
    {
        int val = 0;
        ImgX = count * 58;
        count %= 4;
        if (state == playerState::skill2)
            maxCount = 3;
        else
            maxCount = 4;
        switch (count)
        {
        case 0:
            break;
        case 1:
            val = 18;
            break;
        case 2:
            val = 10;
            ImgX = count * 58 + 19;
            break;
        case 3:
            val = 27;
            ImgX = count * 58 + 25;
            break;
        default:
            return;
            break;
        }
        Maxtick = 80;


        ImgY = 383;
        ImgWidth = 60 + val;
        ImgHeight = 70;
    }
    else if (ID == 2)
    {
        Maxtick = 80;
        count %= 3;
        maxCount = 3;
        ImgWidth = 100;
        ImgX = 200 * count + 50;
        ImgY = 3200 + 60;
        rect.X = (point.x - WidthCorrectionValue - mapMove.x / zoomRatio) * zoomRatio;
    }
}

void CPlayer::DrawSkill3()
{
    if (ID == 1)
    {
        Maxtick = 80;
        count %= 3;
        maxCount = 3;
        ImgX = 3 + count * 60;
        ImgY = 457;
        ImgWidth = 60;
        ImgHeight = 75;
    }
    else if (ID == 2)
    {
        Maxtick = 80;
        count %= 4;
        maxCount = 4;
        ImgWidth = 110;
        ImgX = 200 * count + 40;
        ImgY = 200 + 50;
        rect.X = (point.x - WidthCorrectionValue - mapMove.x / zoomRatio) * zoomRatio;
        //rect.Y = (point.y + 1 - mapMove.x / zoomRatio) * zoomRatio;
    }
}

void CPlayer::DrawSkill4()
{
    if (ID == 1)
    {
    }
}

void CPlayer::idle()
{
    jumpForce = sjumpForce;
    point.y += 1 * zoomRatio;
    if (IwCollsinToWall())
    {
        point.x = collisionX;
        point.y = collisionY;

    }
    if (CollsionCheck(MapType::Platform))
    {
        point.y = CollsionY;
    }
    else if (!IsJump)
    {
        state = playerState::drop;
    }

}

void CPlayer::move()
{

    point.y += 1 * zoomRatio;
    if (IsLeft)
    {
        point.x -= Speed * zoomRatio;
    }
    else
    {
        point.x += Speed * zoomRatio;
    }
    if (IwCollsinToWall())
    {
        point.x = collisionX;
        point.y = collisionY;

    }
    if (CollsionCheck(MapType::Platform))
    {
        point.y = CollsionY;
    }
    else if (!IsJump)
    {
        state = playerState::drop;
    }

    if (!InputMgr::GetInstance()->StayKeyDown(VK_LEFT) && !InputMgr::GetInstance()->StayKeyDown(VK_RIGHT))
    {
        state = playerState::idle;
    }


}

void CPlayer::jump()
{
    JumpUpdate();

    if (IwCollsinToWall())
    {
        point.x = collisionX;
        point.y = collisionY;
    }

    if (CollsionCheck(MapType::Platform) && (jumpForce > -1))
    {
        state = playerState::idle;
        point.y = CollsionY;
        jumpForce = sjumpForce;
        IsJump = false;
    }
}

void CPlayer::jumpMove()
{
    JumpUpdate();
    if (IsLeft)
    {
        point.x -= Speed * zoomRatio * 2;
    }
    else
    {
        point.x += Speed * zoomRatio * 2;
    }
    if (IwCollsinToWall())
    {
        point.x = collisionX;
        point.y = collisionY;
    }
    if (CollsionCheck(MapType::Platform) && (jumpForce > -1))
    {

        state = playerState::move;
        point.y = CollsionY;
        jumpForce = sjumpForce;
        IsJump = false;
    }



}

void CPlayer::rope()
{
    jumpForce = sjumpForce;
    IsJump = false;
    point.x = CollsionX;

    if (IsRopeMove)
    {
        if (IsUp)
            point.y -= Speed * zoomRatio;
        else
            point.y += Speed * zoomRatio;
        //count++;
    }



}

void CPlayer::radder()
{
    jumpForce = sjumpForce;
    IsJump = false;
    point.x = CollsionX;

    if (IsRopeMove)
    {
        if (IsUp)
            point.y -= Speed * zoomRatio;
        else
            point.y += Speed * zoomRatio;
        count++;
    }
}

void CPlayer::sit()
{
}

void CPlayer::attack()
{

    if (IsAttack)
    {
        CMonster* attackMonster;
        int playerToMonsterLen = -1;
        for (CMonster* monster : monsters)
        {

            if (IsCollsion(AttackRect, monster->GetCollider()))
            {
                if (playerToMonsterLen > abs(this->point.x - monster->GetPoint().x))
                    continue;
                else
                {
                    attackMonster = monster;
                    playerToMonsterLen = abs(this->point.x - monster->GetPoint().x);
                }


            }
        }
        if (playerToMonsterLen > -1)
        {
            MonsterType type = attackMonster->GetMonsterType();
            switch (type)
            {
            case MonsterType::MushMom:
                static_cast<MushMom*>(attackMonster)->HitDamage = Damage;
                static_cast<MushMom*>(attackMonster)->Hit(Damage);
                break;
            case MonsterType::Snail1:
                static_cast<Snail1*>(attackMonster)->HitDamage = Damage;
                static_cast<Snail1*>(attackMonster)->Hit(Damage);
                break;
            case MonsterType::Snail2:
            case MonsterType::Snail3:
                static_cast<Snail2*>(attackMonster)->HitDamage = Damage;
                static_cast<Snail2*>(attackMonster)->Hit(Damage);
                break;

            }
            // return;
        }
        IsAttack = false;

    }


    if (count == maxCount)
    {
        count = 0;
        state = playerState::idle;

    }
}

void CPlayer::drop()
{
    jumpForce = gravity;
    point.y += jumpForce;

    if (IwCollsinToWall())
    {
        point.x = collisionX;
        point.y = collisionY;
    }

    if (CollsionCheck(MapType::Platform))
    {
        state = playerState::idle;
        jumpForce = sjumpForce;
        point.y = CollsionY;
        IsJump = false;
    }
}

void CPlayer::jumpAttack()
{
    JumpUpdate();




    if (CollsionCheck(MapType::Platform) && (jumpForce > -1))
    {
        if (count < maxCount)
            state = playerState::attack;
        else
            state = playerState::idle;
        jumpForce = sjumpForce;
        point.y = CollsionY;
        IsJump = false;
    }
    else
    {
        if (count == maxCount)
        {
            state = playerState::jump;
            count = 0;
        }
    }
}

void CPlayer::jumpMoveAttack()
{
    JumpUpdate();



    if (IsLeft)
    {
        point.x -= Speed * zoomRatio * 2;
    }
    else
    {
        point.x += Speed * zoomRatio * 2;
    }
    if (IwCollsinToWall())
    {

        point.x = collisionX;
        point.y = collisionY;
    }
    if (CollsionCheck(MapType::Platform) && (jumpForce > -1))
    {
        if (count < maxCount)
            state = playerState::attack;
        else
            state = playerState::idle;
        point.y = CollsionY;
        jumpForce = sjumpForce;
        IsJump = false;
    }
    else
    {
        if (count == maxCount)
        {
            state = playerState::jumpMove;
            count = 0;
        }
    }
}

void CPlayer::skill1()
{
    if (IsAttack && mageSkills[0].IsStart == false)
    {



        POINT skillPoint = point;
        skillPoint.y += height / 2 - mageSkills[0].GetHeight() / 2 + 16;
        if (!IsLeft)
            skillPoint.x += width / 2;

        mageSkills[0].SetStartPoint(skillPoint);
        mageSkills[0].SetThreeSnailRandNum();
        bool IsHit = false;

        for (CMonster* monster : monsters)
        {
            if (IsCollsion(AttackRect, monster->GetCollider()))
            {

                mageSkills[0].SetEndPoint({ monster->GetCollider().X + monster->GetCollider().Width / 2 , skillPoint.y });
                mageSkills[0].AddHitMonster(monster);
                mageSkills[0].Ishit = true;
                IsHit = true;




                break;
            }


        }
        mageSkills[0].SetIsLeft(IsLeft);
        if (!IsHit)
        {
            if (IsLeft)
                mageSkills[0].SetEndPoint({ skillPoint.x - mageSkills[0].GetSkillWidth() ,skillPoint.y });
            else
                mageSkills[0].SetEndPoint({ skillPoint.x + mageSkills[0].GetSkillWidth(),skillPoint.y });
        }
        if (!mageSkills[0].IsStart)
        {

            mageSkills[0].SetState(SkillState::move);
        }
        mageSkills[0].IsStart = true;

        IsAttack = false;
    }

    if (count == maxCount)
    {
        count = 0;
        state = playerState::idle;

    }
}

void CPlayer::skill2()
{
    if (IsAttack)
    {
        if (ID == 1 && mageSkills[1].IsStart == true)
        {
            return;
        }
        else if (ID == 2 && WarriorSkills[1].IsStart == true)
        {
            return;
        }
        bool IsHit = false;

        POINT skillPoint = point;




        for (CMonster* monster : monsters)
        {
            if (monster->GetIsAlive() && IsCollsion(AttackRect, monster->GetCollider()))
            {
                if (ID == 1)
                {
                    skillPoint.y = monster->GetCollider().Y + monster->GetCollider().Height / 2;
                    skillPoint.x = monster->GetCollider().X + monster->GetCollider().Width / 2;
                    mageSkills[1].SetStartPoint(skillPoint);

                    mageSkills[1].SetEndPoint(skillPoint);
                    mageSkills[1].AddHitMonster(monster);
                    mageSkills[1].Ishit = true;
                    IsHit = true;

                    mageSkills[1].SetIsLeft(IsLeft);
                    mageSkills[1].IsStart = true;
                    mageSkills[1].SetState(SkillState::move);
                    break;
                }
                else if (ID == 2)
                {



                    WarriorSkills[1].AddHitMonster(monster);
                    WarriorSkills[1].Ishit = true;
                    IsHit = true;


                }
            }


        }
        if (ID == 2)
        {
            WarriorSkills[1].SetStartPoint({ point.x + 10,point.y + 30 });
            if (!IsLeft)
                WarriorSkills[1].SetStartPoint({ point.x + 30,point.y + 30 });
            WarriorSkills[1].SetEndPoint(point);
            WarriorSkills[1].SetIsLeft(IsLeft);
            WarriorSkills[1].IsStart = true;
            WarriorSkills[1].SetState(SkillState::move);
        }

        IsAttack = false;
    }

    if (count == maxCount)
    {

        count = 0;
        state = playerState::idle;

    }
}

void CPlayer::skill3()
{
    if (IsAttack)
    {


        bool IsHit = false;



        POINT skillPoint = { (point.x - mapMove.x / zoomRatio) * zoomRatio, (point.y - mapMove.y / zoomRatio) * zoomRatio };

        if (ID == 1 && finalAttack1.GetAttackEnd())
        {
            finalAttack1.SetAttackStart(true);
            finalAttack1.SetPoint(skillPoint);
        }
        else if (ID == 2 && finalAttack2.GetAttackEnd())
        {

            finalAttack2.SetAttackStart(true);
            finalAttack2.SetPoint(skillPoint);
        }

        for (CMonster* monster : monsters)
        {

            if (monster->GetIsAlive() && IsCollsion(AttackRect, monster->GetCollider()))
            {

                if (ID == 1)
                    finalAttack1.hitMonsters.push_back(monster);
                else if (ID == 2)
                    finalAttack2.hitMonsters.push_back(monster);





            }


        }

        IsAttack = false;
    }
    if (count == maxCount)
    {
        count = 0;
        state = playerState::idle;
    }
}

void CPlayer::death()
{

}

void CPlayer::spawn()
{
}

void CPlayer::Hit(int damage)
{
    hp -= damage;
    if (hp <= 0)
    {
        hp = 0;
        state = playerState::death;
        PlaySound(PLAYERDEATH, NULL, SND_ASYNC);

    }
    else state = playerState::hit;
    hitTick = 0;
    point.y -= 5;
}

void CPlayer::DrawRadder()

{
    if (ID == 1)
    {
        Maxtick = 999999;
        count %= 2;
        maxCount = 2;
        ImgX = count * 60;
        ImgY = 685;
        ImgWidth = 60;
        ImgHeight = 75;
    }
    else if (ID == 2)
    {
        maxCount = 2;
        count %= 2;
        ImgX = 200 * count + 70;
        ImgY = 800 + 60;

    }

}

void CPlayer::DrawRope()
{
    if (ID == 1)
    {
        Maxtick = 999999;
        count %= 2;
        maxCount = 2;
        ImgX = count * 58;
        ImgY = 760;
        ImgWidth = 60;
        ImgHeight = 80;
    }
    else if (ID == 2)
    {
        Maxtick = 999999;
        maxCount = 2;
        count %= 2;
        ImgX = 200 * count + 70;
        ImgY = 600 + 60;

    }
}

void CPlayer::LevelUp()
{

    while (exp > maxExp)
    {
        exp -= maxExp;
        level++;
        maxExp = level * 100;
        maxHp = 400 + level * 100;
        hp = maxHp;
        Damage = 3 + 2 * level;
        levelUp.SetIsStart(true);
        playingEffect((SoundType)5);

    }

    if (levelUp.GetIsStart())
    {
        for (Skill& skill : mageSkills)
        {
            switch (skill.GetSkillType())
            {
            case SkillType::three_snail:
                skill.Damage = Damage * 2;
                break;
            case SkillType::double_step:
                skill.Damage = Damage * 3;
                break;
            }
        }
        for (Skill& skill : WarriorSkills)
        {
            switch (skill.GetSkillType())
            {
            case SkillType::three_snail:
                skill.Damage = Damage * 2;
                break;
            case SkillType::slash_blast:
                skill.Damage = Damage * 3;
                break;
            }
        }
        finalAttack1.Damage = Damage * 5;
        finalAttack2.Damage = Damage * 5;
    }
}

void CPlayer::DrawSit()
{
    if (ID == 1)
    {
        count = 0;
        ImgX = 0;
        ImgY = 610;
        ImgWidth = 60;
        ImgHeight = 75;
    }
    else if (ID == 2)
    {
        count = 0;
        ImgX = 200 * count + 70;
        ImgY = 1800 + 60;
    }
}

void CPlayer::Init(int id)
{
    this->ID = id;
    if (ID == 1)
    {
        /*if (bitmap_Left == nullptr)
            bitmap_Left = mageBitmap;
        if (bitmap_Right == nullptr)
            bitmap_Right = mageBitmap_R;*/
        bitmapWidth = mageBitmap->GetWidth();
        WidthCorrectionValue = 0;
    }
    else if (ID == 2)
    {
        if (img_L != nullptr)
        {
            delete img_L;

        }
        img_L = new CImage();
        img_L->Load(L"../Data/img/Player/Warrier/player_L_.bmp");
        if (img_R == nullptr)
        {
            delete img_R;

        }
        img_R = new CImage();
        img_R->Load(L"../Data/img/Player/Warrier/player_R.bmp");
        WidthCorrectionValue = 10;
        bitmapWidth = img_L->GetWidth();

    }
    deathImg = new CImage();
    deathImg->Load(L"../Data/img/Player/death.bmp");

    exp = 0;
    Speed = 2;
    point = { 0,0 };
    IsJump = false;
    width = 60; height = 70 * 0.9f;
    jumpForce = sjumpForce;
    moveMap = FALSE;
    Damage = 5;
    name = "A";
    hp = 500;
    mp = 100;
    maxHp = hp;
    maxMp = mp;
    level = 1;
    exp = 20;
    maxExp = 100;

    for (Skill& skill : mageSkills)
    {
        switch (skill.GetSkillType())
        {
        case SkillType::three_snail:
            skill.Damage = Damage * 2;
            break;
        case SkillType::double_step:
            skill.Damage = Damage * 3;
            break;
        }
    }
    for (Skill& skill : WarriorSkills)
    {
        switch (skill.GetSkillType())
        {
        case SkillType::three_snail:
            skill.Damage = Damage * 2;
            break;
        case SkillType::slash_blast:
            skill.Damage = Damage * 3;
            break;
        }
    }
    finalAttack1.Damage = Damage * 5;
    finalAttack2.Damage = Damage * 5;
}
void CPlayer::DrawIdle()
{

    if (ID == 1)
    {
        count %= 3;
        maxCount = 3;
        ImgX = 60 * count;
        ImgY = 0;
        ImgWidth = 60;
        ImgHeight = 80;
    }
    else if (ID == 2)
    {
        count %= 5;
        maxCount = 5;
        ImgX = 200 * count + 60;
        ImgY = 60;

    }

}
extern BillageType billageType;
void CPlayer::Update()
{
    LevelUp();

    if (BillageType::Henesys == billageType)
        Speed = 10;
    else Speed = 2;
    KeyCheck();
    if (tick > Maxtick)
    {
        tick = 0;
        count++;
    }
    StateUpdate();
    if (!levelUp.GetIsStart())
        stopEffect(EffectID);
    levelUp.SetPoint({ (long)((point.x - mapMove.x / zoomRatio) * zoomRatio) , (long)((point.y + 3 - mapMove.y / zoomRatio) * zoomRatio) });


    tick += 10;

}
void CPlayer::JumpUpdate()
{
    point.y += jumpForce;
    if (jumpForce <= -sjumpForce / 2)
        jumpForce += 1;


}
void CPlayer::StateUpdate()
{
    switch (state)
    {
    case playerState::idle:
        CPlayer::idle();
        break;
    case playerState::move:
        CPlayer::move();
        break;
    case playerState::jump:
        CPlayer::jump();
        break;
    case playerState::jumpAttack:
        CPlayer::jumpAttack();
        break;
    case playerState::jumpMove:
        CPlayer::jumpMove();
        break;
    case playerState::jumpMoveAttack:
        CPlayer::jumpMoveAttack();
        break;
    case playerState::drop:
        CPlayer::drop();
        break;
    case playerState::hit:
        CPlayer::hit();
        break;
    case playerState::rope:
    case playerState::radder:
        CPlayer::radder();
        break;
    case playerState::sit:
        break;
    case playerState::attack:

        CPlayer::attack();
        break;
    case playerState::skill1:
        CPlayer::skill1();

        break;
    case playerState::skill2:
        CPlayer::skill2();
        break;
    case playerState::skill3:
        CPlayer::skill3();
        break;
    case playerState::death:
        CPlayer::death();
        break;
    }
    if (IsAttack && mageSkills[0].IsStart == false)
        IsAttack = false;
}

bool CPlayer::CollsionCheck(MapType mapType)
{


    for (TileRect* tile : tileMap)

    {
        MapType tileMapType = tile->GetType();




        if (tileMapType == mapType || tileMapType == MapType::Attack)
        {
            POINT tilePoint = tile->GetPos();
            int x = (point.x + width / 2);

            int tileLeng = tile->GetLength();
            if (x >= tilePoint.x && x <= (tilePoint.x + tileLeng))
            {

                int charFootPosY = (point.y + height) + Speed;

                if (charFootPosY > tilePoint.y && charFootPosY <= (tilePoint.y + tileLeng))
                {
                    CollsionX = tilePoint.x + tileLeng / 2 - width / 2;
                    CollsionY = (tilePoint.y - height);
                    if (mapType == MapType::Rope || mapType == MapType::Ladder)
                    {

                        CollsionX += 3;
                        if (!IsLeft)
                            CollsionX += 2;
                        return true;
                    }

                    if (tileMapType == MapType::Attack && playerHitTime <= 0)
                    {
                        playerHitTime = 2.1f;
                        CPlayer::Hit(5);
                    }
                    return true;
                }
            }
        }

    }



    return false;
}
bool  CPlayer::IwCollsinToWall()
{
    for (TileRect* tile : tileMap)

    {
        MapType tileMapType = tile->GetType();




        if (tileMapType == MapType::Wall)
        {
            POINT tilePoint = tile->GetPos();
            int x = (point.x + width / 2);

            int tileLeng = tile->GetLength();
            if (x >= tilePoint.x && x <= (tilePoint.x + tileLeng))
            {

                int charFootPosY = point.y + height * 2 / 3;

                if (charFootPosY > tilePoint.y && charFootPosY <= (tilePoint.y + tileLeng))
                {
                    collisionX = tilePoint.x + (IsLeft ? tileLeng + 1 - width / 2 : -width / 2 - 1);


                    collisionY = point.y;
                    return true;
                }
            }
        }

    }
    return false;
}
void CPlayer::DrawSence(HDC& hdc)
{
    Gdiplus::Graphics graphics(hdc);


    ImgWidth = 60, ImgHeight = 80;
    if (ID == 2)
        ImgWidth = 80, ImgHeight = 90;

    rect.X = (point.x - mapMove.x / zoomRatio) * zoomRatio;
    rect.Y = (point.y + 3 - mapMove.y / zoomRatio) * zoomRatio;

    switch (state)
    {
    case playerState::idle:

        DrawIdle();


        break;
    case playerState::move:

        DrawMove();

        break;
    }

    rect.Height = height * zoomRatio * 0.8f;
    rect.Width = width * zoomRatio * 0.8f;
    if (ID == 1)
    {
        if (!IsLeft)
        {
            ImgX = bitmapWidth - ImgX - ImgWidth;
            bitmap = mageBitmap_R;
        }
        else
        {
            bitmap = mageBitmap;
        }
        graphics.DrawImage(bitmap, rect, ImgX, ImgY, ImgWidth, ImgHeight, Gdiplus::UnitPixel);
    }
    else if (ID == 2)
    {
        /* RECT imgRect;
         imgRect.left = rect.X;
         imgRect.top = rect.Y;
         imgRect.right = rect.X + rect.Width;
         imgRect.bottom = rect.Y + rect.Height;*/

        if (IsLeft)
        {
            ImgX = bitmapWidth - ImgX - ImgWidth;
            drawImg = img_L;

        }
        else
        {
            drawImg = img_R;

        }
        drawImg->TransparentBlt(hdc, rect.X, rect.Y, rect.Width, rect.Height, ImgX, ImgY, ImgWidth, ImgHeight, RGB(255, 0, 255));

    }

}
void CPlayer::CountUpdate()
{

    tick += 100;
    if (tick > Maxtick)
    {
        if (state == playerState::move)
        {
            maxCount = 4;
            count %= 4;
            Maxtick = 1000;

        }
        else if (state == playerState::idle)
        {
            count %= 3;
            maxCount = 3;
            Maxtick = 1000;
        }
        tick = 0;
        count++;
    }

}
bool CPlayer::CollsionCheckToMonster()
{
    for (CMonster* monster : monsters)
    {
        if (monster->GetIsAlive() && monster->GetState() != moveState::death && monster->GetState() != moveState::spawn)
        {
            if (IsCollsion(monster->GetCollider(), { point.x,point.y + 3,(int)(width * 0.8),height }))
            {
                CPlayer::Hit(monster->GetDamage());
                return true;
            }
        }
    }
    return false;
}
void CPlayer::DrawMove()
{
    if (ID == 1)
    {
        ImgY = 80;
        count %= 4;

        switch (count)
        {
        case  0:
            ImgX = -5;

            break;
        case 1:
            ImgX = ImgWidth * count + 15;

            break;
        case 2:
            ImgX = ImgWidth * count + 15;

            break;
        case 3:
            ImgX = ImgWidth * count + 25;

            break;
        default:
            ImgX = 0;
            count = 0;
        }
    }
    else if (ID == 2)
    {
        maxCount = 4;
        count %= 4;
        //ImgWidth = 85;
        ImgX = 200 * count + 60;
        ImgY = 400 + 60;
    }
}
void CPlayer::DrawJump()
{
    tick += 10;
    if (ID == 1)
    {
        count = 0;
        ImgX = 130;
        ImgY = 530;
    }
    else if (ID == 2)
    {
        maxCount = 4;
        count = 0;
        ImgWidth = 85;
        ImgX = 200 * count + 60;
        ImgY = 1000 + 60;
    }

}
void CPlayer::DrawDeath()
{
    tick += 10;
    Maxtick = 133;
    maxCount = 12;
    count %= 12;
    ImgWidth = 40;
    ImgHeight = 48;
    ImgX = 40 * count;
    ImgY = 0;
}
void CPlayer::DrawSpawn()
{
}
void CPlayer::DrawAttack()
{

    if (ID == 1)
    {
        Maxtick = 80;
        count %= 3;
        maxCount = 3;
        ImgX = count * 60;
        ImgY = 238;
        ImgWidth = 60;
        ImgHeight = 75;
        if (count == 2)
            ImgWidth = 70;
    }
    else if (ID == 2)
    {
        Maxtick = 100;
        count %= 3;
        maxCount = 3;
        ImgWidth = 120;
        ImgX = 200 * count + 40;
        ImgY = 3000 + 60;
        rect.X = (point.x - WidthCorrectionValue - mapMove.x / zoomRatio) * zoomRatio;



    }

}



void CPlayer::hit()
{

    hitTick += 1;
    if (10 == hitTick)
    {
        state = playerState::drop;
        IsVisible = true;
        return;
    }
    if (IsLeft)
    {
        point.x += 2;
    }
    else
    {
        point.x -= 2;
    }
    if (hitTick % 2 == 0)
    {
        point.y += 1;

    }

}

void CPlayer::Draw(HDC& hdc)
{
    if (!IsVisible)
        return;
    Maxtick = 133;
    Gdiplus::Graphics graphics(hdc);


    ImgWidth = 60, ImgHeight = 80;
    if (ID == 2)
        ImgWidth = 80, ImgHeight = 90;

    rect.X = (point.x - mapMove.x / zoomRatio) * zoomRatio;
    rect.Y = (point.y + 3 - mapMove.y / zoomRatio) * zoomRatio;

    switch (state)
    {
    case playerState::idle:

        DrawIdle();


        break;
    case playerState::move:

        DrawMove();

        break;
    case playerState::jump:
    case playerState::jumpMove:
    case playerState::drop:
    case playerState::hit:
        DrawJump();
        break;
    case playerState::rope:
        DrawRope();
        break;
    case playerState::radder:
        DrawRadder();
        break;
    case playerState::sit:
        DrawSit();
        break;
    case playerState::attack:
    case playerState::jumpAttack:
    case playerState::jumpMoveAttack:
        DrawAttack();
        break;
    case playerState::skill1:
        DrawSkill1();
        break;
    case playerState::skill2:
        switch (ID)
        {
        case 1: DrawSkill2();
            break;
        case 2: DrawAttack();
            break;
        }


        break;
    case playerState::skill3:
        DrawSkill3();
        state = playerState::idle;
        return;
        break;
    case playerState::death:
        DrawDeath();
        break;
    }



    rect.Height = ImgHeight * zoomRatio * 0.8f;
    rect.Width = ImgWidth * zoomRatio * 0.8f;


    if (state == playerState::death)
    {
        rect.Height = 80 * zoomRatio * 0.8f;
        rect.Width = 60 * zoomRatio * 0.8f;
        deathImg->TransparentBlt(hdc, rect.X, rect.Y, rect.Width, rect.Height, ImgX, ImgY, ImgWidth, ImgHeight, RGB(255, 0, 255));
    }



    else if (ID == 1)
    {
        if (!IsLeft)
        {
            ImgX = bitmapWidth - ImgX - ImgWidth;
            bitmap = mageBitmap_R;
        }
        else
        {
            bitmap = mageBitmap;
        }
        graphics.DrawImage(bitmap, rect, ImgX, ImgY, ImgWidth, ImgHeight, Gdiplus::UnitPixel);
    }
    else if (ID == 2)
    {
        /* RECT imgRect;
         imgRect.left = rect.X;
         imgRect.top = rect.Y;
         imgRect.right = rect.X + rect.Width;
         imgRect.bottom = rect.Y + rect.Height;*/

        if (IsLeft)
        {
            ImgX = bitmapWidth - ImgX - ImgWidth;
            drawImg = img_L;

        }
        else
        {
            drawImg = img_R;

        }
        drawImg->TransparentBlt(hdc, rect.X, rect.Y, rect.Width, rect.Height, ImgX, ImgY, ImgWidth, ImgHeight, RGB(255, 0, 255));

    }

}
bool CPlayer::IsCollsion(Gdiplus::Rect rect1, Gdiplus::Rect rect2)
{
    if (rect1.IntersectsWith(rect2))
    {
        return true;
    }

    if (rect1.Contains(rect2) || rect2.Contains(rect1))
    {
        return true;
    }

    return false;
}