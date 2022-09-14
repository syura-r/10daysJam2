#pragma once
#include "BaseEnemy.h"
class Boss;
class BossHead :
    public BaseEnemy
{
public:
    BossHead(const Vector3& pos, Boss* bossPtr);
    void Update();
private:
    Boss* boss = nullptr;
    int moveCounter = 0;
    bool bound = false;
};

