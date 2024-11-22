#pragma once
#include "framework.h"

class CCharacter : public CObject
{
protected:
    string name;
    float Speed;
    float Direction;
    float hp, mp;
    float maxHp, maxMp;
    unsigned int level;
    long long exp , maxExp;
    float Damage;
    float Defense;

    bool IsUp{ false }; // ����
    bool IsJump = false; // ��������
    int startjumpy = 0; // ���� ������ġ

    static const int sjumpForce{ -12 }; // ���� �� 

    int jumpForce = -8; // ���� ��
    int gravity = 5;
    int collisionX;
    int collisionY;

    virtual void Idle() {}
    virtual void Move() {}
    virtual void Jump() {}
    virtual void death() {}
    virtual void spawn() {}
    virtual void Attack() {}
    virtual void Init() {}

public:

    unsigned int HitDamage;
    string GetName() { return name; }
    void SetName(string name) { this->name = name; }
    float GetHp() { return hp; }
    void SetHp(float hp) { this->hp = hp; }
    float GetMp() { return mp; }
    void SetMp(float mp) { this->mp = mp; }
    float GetMaxHp() { return maxHp; }
    void SetMaxHp(float maxHp) { this->maxHp = maxHp; }
    float GetMaxMp() { return maxMp; }
    void SetMaxMp(float maxMp) { this->maxMp = maxMp; }
    long long GetExp() { return exp; }
    void SetExp(long long exp) { this->exp = exp; }
    long long GetMaxExp() { return maxExp; }
    void SetMaxExp(long long maxExp) { this->maxExp = maxExp; }
    unsigned int GetLevel() { return level; }
    void SetLevel(unsigned int level) { this->level = level; }
    void SetSpeed(float Speed) { this->Speed = Speed; }
    void AddExp(float addexp) { exp += addexp; }
    float GetDamage() { return Damage; }

};

