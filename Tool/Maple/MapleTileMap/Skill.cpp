#include "Skill.h"
#include "TileMap.h"
std::random_device rd;
std::default_random_engine g(rd());
std::uniform_int_distribution<int> dis(0, 2);

Skill::Skill()
{
    Ishit = false;
    sanil = NULL;
    DrawImg = nullptr;
    IsStart = false;

    count = 0;
    width = 35;
    height = 35;
}

void Skill::Init()
{

    switch (type)
    {
    case SkillType::three_snail :
        if (skillImg == NULL)
            skillImg.Load(L"../Data/img/skills/three_snail/throwable.bmp");
        if (HitImg == NULL)
            HitImg.Load(L"../Data/img/skills/three_snail/hit.bmp");
        width = 55;
        break;
    case SkillType::double_step:
         if (HitImg == NULL)
            HitImg.Load(L"../Data/img/skills/double_step/hit.bmp");
        width = 75;
        break;
    case SkillType::genesis:
        break;
    case SkillType::slash_blast:
        if (skillImg == NULL)
            skillImg.Load(L"../Data/img/skills/slash_blast/charge.bmp");
        if(skillImg_L == NULL)
            skillImg_L.Load(L"../Data/img/skills/slash_blast/charge_L.bmp");
        if (skillImg_R == NULL)
            skillImg_R.Load(L"../Data/img/skills/slash_blast/charge_R.bmp");
        width = 75;
        break;
    /*case SkillType:::
        break;*/
    default :
        break;
    }

	
    /*if (skillImg == NULL)
        MessageBox(NULL, L"SkillImage Failed", L"Error", MB_OK);
    else
        GetObject(skillImg, sizeof(BITMAP), &skillBitmap);*/
    state = SkillState::idle;
    count = 0;
}

void Skill::SetThreeSnailRandNum()
{
    ThreeSnailRandNum = dis(g);
}



void Skill::Draw(HDC hdc)
{


    Gdiplus::Graphics graphics(hdc);
    Gdiplus::Rect rect;

    switch (type)
    {
    case SkillType::three_snail:
        DrawThreeSnail(rect);
        break;
    case SkillType::double_step :
        DrawDoubleStep(rect);
        break;
    case SkillType::slash_blast :
        DrawSlashBlast(rect);
        break;
    }
   


    if (state == SkillState::move)
    {
        DrawImg = &skillImg;
        if (type == SkillType::double_step)
            DrawImg = nullptr;
        else if (type == SkillType::slash_blast)
        {
            if (IsLeft)
            {
                DrawImg = &skillImg_L;
            }
            else
            {
                DrawImg = &skillImg_R;
            }
        }
    }
    else if (state == SkillState::hit)
    {
        DrawImg = &HitImg;
        if (type == SkillType::slash_blast)
            DrawImg = nullptr;
    }
    else
    {
        DrawImg = nullptr;
    }


    if (DrawImg != nullptr)
        DrawImg->TransparentBlt(hdc, rect.X - rect.Width / 2, rect.Y - rect.Height / 2, rect.Width, rect.Height, srcX, srcY, srcWidth, srcHeight, RGB(255, 255, 255));
    count++;

}




void Skill::DrawThreeSnail(Gdiplus::Rect& rect)
{

    if (count == 6)
        EndState = true;
    count %= 6;

    rect.Height = 35 * zoomRatio;
    rect.Width = 35 * zoomRatio;

    rect.X = (point.x - mapMove.x / zoomRatio) * zoomRatio;
    rect.Y = (point.y - mapMove.y / zoomRatio) * zoomRatio;







    switch (state)
    {
    case SkillState::move:
    {


        srcWidth = 35;
        srcHeight = 35;
        srcX = count * srcWidth;
        srcY = ThreeSnailRandNum * srcHeight;
    }
    break;
    case SkillState::hit:
    {


        srcWidth = 81;
        srcHeight = 79;

        srcX = count * srcWidth;
        srcY = ThreeSnailRandNum * srcHeight;
    }
    break;
    }

}
void Skill::DrawDoubleStep(Gdiplus::Rect& rect)
{

    if (count == 40)
    {
        EndState = true;
        return;
    }
    count %= 40;
    

    rect.Height = 100 * zoomRatio;
    rect.Width = 100 * zoomRatio;

    rect.X = (point.x - mapMove.x / zoomRatio) * zoomRatio;
    rect.Y = (point.y - mapMove.y / zoomRatio) * zoomRatio;







    switch (state)
    {
    case SkillState::hit:
    {

        srcWidth = 188 + (count / 8) *2;
        srcHeight = 194;

        srcX = (count / 8) * srcWidth;
        srcY = 0;
    }
    break;
    }
}

void Skill::DrawSlashBlast(Gdiplus::Rect& rect)
{
    if (count == 15)
    {
        EndState = true;
       
    }
    count %= 25;


    rect.Height = 150 * zoomRatio;
    rect.Width = 250 * zoomRatio;

    rect.X = (point.x - mapMove.x / zoomRatio) * zoomRatio;
    rect.Y = (point.y - mapMove.y / zoomRatio) * zoomRatio;


    switch (state)
    {
    case SkillState::move:
    {

        srcWidth = 300;
        srcHeight = 233;

        srcX = 1600 + count / 5 * srcWidth;
        srcY = 0;
    }
    break;
    }

    if (!IsLeft)
    {
        srcX = skillImg_R.GetWidth() - srcX - srcWidth;
        rect.X = (point.x + 30 - mapMove.x / zoomRatio) * zoomRatio;
    }





}

void Skill::Update()
{
    switch (state)

    {
    case SkillState::idle:
        Idle();


        break;
    case SkillState::move:
        Move();
        break;
    case SkillState::hit:
        Hit();
        break;

    }
   
}
void Skill::stateUpdate()
{
    
}
void Skill::Hit()

{
    
    if (EndState == true)
    {
        state = SkillState::idle;
        IsStart = false;
        count = 0;
        EndState = false;
    }
}

void Skill::Idle()
{



}
void Skill::Move()
{
    switch (type)
    {
    case SkillType::three_snail :
        if (IsLeft)
        {
            point.x -= 5;
            if (point.x <= endPoint.x)
            {
                point.x = endPoint.x;
                IsEndPoint = true;
            }
        }
        else
        {
            point.x += 5;
            if (point.x >= endPoint.x)
            {
                point.x = endPoint.x;
                IsEndPoint = true;

            }
        }
        break;
    case SkillType::double_step:
        IsEndPoint = true;
        break;
    case SkillType::slash_blast:
        if (EndState)
        {
            IsEndPoint = true;
            IsStart = false;
        }
        break;
   }
    
    if (IsEndPoint)
    {
        if (hitMonsters.empty()) // ���ͷ� ���� �ؾ���
        {
            state = SkillState::idle;
            IsStart = false;
        }
        else
        {
            if (!hitMonsters.empty() || hitMonsters.size() > 0)
                for (vector<CMonster*>::iterator it = hitMonsters.begin(); it != hitMonsters.end(); ++it)
                {
                    MonsterType type = (*it)->GetMonsterType();
                    switch (type)
                    {
                    case MonsterType::MushMom:
                        static_cast<MushMom*>((*it))->HitDamage = Damage;
                        static_cast<MushMom*>((*it))->Hit(Damage);
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
                    //�����ڵ�
                    
                }



            state = SkillState::hit;
        }

        

            hitMonsters.clear();
        

        //IsStart = false;
        count = 0;
        EndState = false;
        IsEndPoint = false;
    }


}
