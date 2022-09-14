#pragma once
#include "Object.h"
class Boss;
class BossBattleBlock :
    public Object
{
public:
	BossBattleBlock(const Vector3& position, Boss* bossPtr);
	void Update()override;
private:
	Boss* boss = nullptr;
};

