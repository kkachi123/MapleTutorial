#pragma once
#include "MapManager.h"
class MapleMonsterManager
{
private:
    BOOL keys[256];
    BOOL ctrlClick;
    int speed;
    LPPOINT inputMousePos;
    LPPOINT dragMousePos;
    MonsterType inputMonseterType;
    int drawAreaWidth = 0;
    int drawAreaHeight = 0;
public:
    MapleMonsterManager();
    ~MapleMonsterManager();
    void SetUp();
    void MapMovingButton(UINT message, BOOL isDown, WPARAM wParam);
    void MapMoving(); 
    void MapButtonChar(UINT message, WPARAM wParam);
    void MapMouseControl(UINT message, WPARAM wParam, LPARAM lParam);
    int GetSpeed() { return speed; }
    void SettingSnail1(POINT drawPoint);
    void SettingSnail2(POINT drawPoint, int type, int exp = 4, int hp = 20, int Damage = 3, int Defense = 0);
    void SettingMushMom(POINT drawPoint);
    void SettingMonster();
    void SettingMonsters();
    void DeleteMonster();
    void SettingNPC(POINT drawPoint,int inputNPCType);
    void DeleteNPC();
    void SaveMonster();
    void LoadMonster();
    void LoadMonsterData(int type);
    void Render(HDC hdc);

    LRESULT CALLBACK WndProc(UINT message, WPARAM wParam, LPARAM lParam);
};



