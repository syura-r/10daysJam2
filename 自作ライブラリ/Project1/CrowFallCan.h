#pragma once
#include "BaseEnemy.h"
class Boss;
class CrowFallCan :
    public BaseEnemy
{
public:
    CrowFallCan(const Vector3& pos, Boss* bossPtr);
    void Update()override;
private:
    const float rotVel = 8.0f;
    Boss* boss = nullptr;
};

